; ubnei_lcode_01.asm -- UBNEI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 098h, 006h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 098h, 006h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        call   sub_1_3930
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
UBNEI_Control:
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
        mov    eax, dword ptr [ebp + 24h]
        push   eax
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
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_0180
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_0120
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_0115
        cmp    al, 0ffh
        jne    SHORT loc_1_0115
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_0115
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_0187
loc_1_0115:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_0187
        db  08Bh, 0FFh
loc_1_0120:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_0170
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0146
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_014D
loc_1_0146:
        mov    dword ptr [ebp - 10h], 1
loc_1_014D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0160
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_018A
        db  08Dh, 064h, 024h, 000h
loc_1_0160:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_018A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0170:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_018A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0180:
        mov    dword ptr [ebp - 4], 20h
loc_1_0187:
        mov    esi, dword ptr [ebp + 8]
loc_1_018A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_0281
loc_1_0195:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_01ED
        test   al, 2
        je     SHORT loc_1_01D9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 006h, 006h, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_01D9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_01ED
loc_1_01D9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_01E4
        test   bl, 4
        jne    SHORT loc_1_01ED
loc_1_01E4:
        test   bl, 20h
        je     NEAR PTR loc_1_0276
loc_1_01ED:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_0462
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0276
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_022F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_022F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0240
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0246
        db  08Bh, 0FFh
loc_1_0240:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0246:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0255
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
loc_1_0255:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_050C
        mov    edi, ebx
        jmp    SHORT loc_1_0279
loc_1_0276:
        mov    edi, dword ptr [edi + 10h]
loc_1_0279:
        or     edi, edi
        jne    NEAR PTR loc_1_0195
loc_1_0281:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_02A0
        pop    ebp
        ret    4
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_031E
loc_1_02B0:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0317
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0317
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_02DC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_02DC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_02F0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_02F6
        db  005h, 000h, 000h, 000h, 000h
loc_1_02F0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_02F6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_050C
        mov    edi, ebx
        jmp    SHORT loc_1_031A
loc_1_0317:
        mov    edi, dword ptr [edi + 10h]
loc_1_031A:
        or     edi, edi
        jne    SHORT loc_1_02B0
loc_1_031E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_0345
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0345:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_0381
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_0372
        cmp    bl, 0ffh
        jne    SHORT loc_1_0372
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_0372
        test   al, 8
        je     SHORT loc_1_036B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_036B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0372:
        test   al, 4
        jne    SHORT loc_1_037A
        test   al, 2
        je     SHORT loc_1_0399
loc_1_037A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0381:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0399
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_0399
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0399:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_041A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0420:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0426:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_042C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0432:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0438:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_043E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0444:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_044A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_0456:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_045C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0462:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0468:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_046E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_0474:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_0480:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_048C:
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
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_04E2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_04E8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_04EE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_04F4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_04FA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_0500:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0506:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_050C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0530:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0554:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0578:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0584:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_058A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0590:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
sub_1_0596:
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
sub_1_05D2:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_05D8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_05EC:
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
sub_1_060C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_062C
        or     ecx, ecx
        je     SHORT loc_1_0642
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_0640
loc_1_062C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_0640
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_0640:
        rep movsb
loc_1_0642:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 055h, 042h, 04Eh, 045h
        db  049h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0ffffffffh
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 18h]
        lea    eax, [eax + edx*4]
loc_1_06C3:
        or     edx, edx
        je     SHORT loc_1_06D2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_06C3
        mov    dword ptr [ecx], edx
loc_1_06D2:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_06E4
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_06E4:
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 0dbh], 0
        je     SHORT loc_1_0700
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h
loc_1_0700:
        db  0A1h, 060h, 040h, 000h, 000h
        push   eax
        push   0
        push   25h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     SHORT loc_1_0724
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0724:
        mov    eax, dword ptr [esi + 0aah]
        cmp    dword ptr [esi + 4ch], eax
        ja     SHORT loc_1_0749
        push   0
        push   25h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0749:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0cah]
        push   eax
        db  0E8h, 002h, 0FAh, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_0790
        push   0
        push   25h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h
loc_1_0790:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 9eh]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 42h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [esi + 42h]
        mov    dword ptr [esi + 98h], eax
        mov    eax, dword ptr [esi + 0aah]
        inc    eax
        mov    dword ptr [esi + 0aah], eax
        cmp    eax, 1
        jne    SHORT loc_1_0834
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp - 4]
        add    esi, 5ch
        push   eax
        db  068h, 040h, 009h, 000h, 000h
        push   eax
        call   sub_1_1D80
        push   esi
        push   0
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 050h, 008h, 000h, 000h
        push   eax
        call   sub_1_1D80
        or     al, al
        je     SHORT loc_1_0834
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0834:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        call   sub_1_0850
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h
sub_1_0850:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 8]
        cmp    dword ptr [ecx + 0aeh], 0
        je     SHORT loc_1_0875
        mov    eax, dword ptr [ecx + 0ech]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [ecx + 0ech], edx
        pop    ebp
        ret    8
loc_1_0875:
        mov    eax, dword ptr [ecx + 0a6h]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [ecx + 0a6h], edx
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_043E
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 18h]
        cmp    byte ptr [ebp + 20h], 0
        mov    eax, dword ptr [ecx + 8]
        je     SHORT loc_1_08F2
        cmp    word ptr [eax + 100h], 0ah
        jae    SHORT loc_1_08E9
        mov    eax, dword ptr [ebp + 1ch]
        push   0
        mov    dword ptr [eax + 4], ecx
        push   0
        push   8
        push   eax
        db  068h, 020h, 009h, 000h, 000h
        push   ecx
        call   sub_1_1D80
        or     al, al
        je     SHORT loc_1_08E0
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_08E0:
        mov    eax, 0c0000001h
        pop    ebp
        ret    1ch
loc_1_08E9:
        mov    eax, 0c000009ah
        pop    ebp
        ret    1ch
loc_1_08F2:
        push   0
        push   0
        push   8
        push   0
        db  068h, 040h, 009h, 000h, 000h
        push   ecx
        call   sub_1_1D80
        or     al, al
        je     SHORT loc_1_0912
        mov    eax, 103h
        pop    ebp
        ret    1ch
loc_1_0912:
        sub    eax, eax
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0920:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_045C
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0940:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0950:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        sub    si, si
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 0f010h
        je     SHORT loc_1_0971
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_1_0971:
        test   al, 24h
        je     SHORT loc_1_0980
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
        db  090h
loc_1_0980:
        test   al, 3
        je     SHORT loc_1_0988
        mov    si, 1
loc_1_0988:
        test   al, 8
        je     SHORT loc_1_0990
        or     si, 2
loc_1_0990:
        mov    word ptr [ecx + 9ch], si
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_1_09E4
        cmp    word ptr [ecx + 100h], 0ah
        jae    SHORT loc_1_09DA
        mov    eax, dword ptr [ebp + 14h]
        push   0
        mov    dword ptr [eax + 4], edx
        push   esi
        push   7
        push   eax
        db  068h, 020h, 009h, 000h, 000h
        push   edx
        call   sub_1_1D80
        or     al, al
        je     SHORT loc_1_09D0
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_1_09D0:
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_1_09DA:
        mov    eax, 0c000009ah
        pop    esi
        pop    ebp
        ret    14h
loc_1_09E4:
        push   0
        push   esi
        push   7
        push   0
        db  068h, 040h, 009h, 000h, 000h
        push   edx
        call   sub_1_1D80
        or     al, al
        je     SHORT loc_1_0A04
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
loc_1_0A04:
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_0A10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   1
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0dch]
        add    ecx, 0e0h
        mov    dword ptr [eax + 10h], edx
        mov    dword ptr [ecx - 4], eax
        push   ecx
        call   sub_1_04E2
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0A40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 0dch]
        or     edi, edi
        je     SHORT loc_1_0AC9
loc_1_0A53:
        mov    eax, dword ptr [esi + 0dch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esi + 0dch], eax
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 20h]
        dec    eax
        mov    dword ptr [edi + 20h], eax
        jne    SHORT loc_1_0ABF
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [ebx + 98h], eax
        jne    SHORT loc_1_0A7F
        push   ebx
        call   sub_1_3380
loc_1_0A7F:
        cmp    dword ptr [ebx + 0a6h], 0
        jne    SHORT loc_1_0AA5
        cmp    byte ptr [ebx + 0dbh], 0
        jne    SHORT loc_1_0AA5
        push   0
        push   0
        push   5
        push   edi
        db  068h, 090h, 00Dh, 000h, 000h
        push   edi
        call   sub_1_1D80
        jmp    SHORT loc_1_0ABF
loc_1_0AA5:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0aah]
loc_1_0ABF:
        mov    edi, dword ptr [esi + 0dch]
        or     edi, edi
        jne    SHORT loc_1_0A53
loc_1_0AC9:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_0AD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 0dbh], 0
        jne    NEAR PTR loc_1_0B72
        cmp    dword ptr [esi + 115h], 0
        jne    SHORT loc_1_0B00
        cmp    word ptr [esi + 100h], 0
        jne    SHORT loc_1_0B00
        mov    byte ptr [esi + 0dah], 0
        jmp    SHORT loc_1_0B72
loc_1_0B00:
        cmp    byte ptr [esi + 0dah], 1
        jne    SHORT loc_1_0B40
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_0B30
        cmp    dword ptr [esi + 0fch], 0
        jne    SHORT loc_1_0B30
        mov    byte ptr [esi + 0dah], 0
        push   esi
        call   sub_1_0B90
        jmp    SHORT loc_1_0B72
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0B30:
        mov    byte ptr [esi + 0dah], 0
        jmp    SHORT loc_1_0B72
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B40:
        mov    byte ptr [esi + 0dah], 1
        mov    dword ptr [esi + 119h], 0
        mov    dword ptr [esi + 0fch], 0
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        or     al, 2
        out    dx, al
        push   esi
        call   sub_1_2690
loc_1_0B72:
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_1_0B89
        push   1388h
        add    esi, 0ceh
        push   esi
        call   sub_1_04E2
loc_1_0B89:
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_0B90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
        mov    byte ptr [esi + 0dbh], 1
        mov    ecx, dword ptr [esi + 10dh]
        or     ecx, ecx
        je     NEAR PTR loc_1_0C4C
loc_1_0BC1:
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 10dh], eax
        dec    dword ptr [esi + 115h]
        mov    edi, dword ptr [ecx + 28h]
        push   0c0000001h
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 20h]
        dec    eax
        mov    dword ptr [edi + 20h], eax
        jne    SHORT loc_1_0C3E
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [ebx + 98h], eax
        jne    SHORT loc_1_0BFE
        push   ebx
        call   sub_1_3380
loc_1_0BFE:
        cmp    dword ptr [ebx + 0a6h], 0
        jne    SHORT loc_1_0C24
        cmp    byte ptr [ebx + 0dbh], 0
        jne    SHORT loc_1_0C24
        push   0
        push   0
        push   5
        push   edi
        db  068h, 090h, 00Dh, 000h, 000h
        push   edi
        call   sub_1_1D80
        jmp    SHORT loc_1_0C3E
loc_1_0C24:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0aah]
loc_1_0C3E:
        mov    ecx, dword ptr [esi + 10dh]
        or     ecx, ecx
        jne    NEAR PTR loc_1_0BC1
loc_1_0C4C:
        push   esi
        call   sub_1_2190
        push   40010007h
        push   esi
        call   sub_1_1120
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 24h], 0
        je     SHORT loc_1_0C90
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0C90:
        inc    dword ptr [esi + 20h]
        mov    eax, dword ptr [edi + 0a6h]
        cmp    esi, eax
        jne    SHORT loc_1_0CA8
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 0a6h], eax
        jmp    SHORT loc_1_0CBE
loc_1_0CA8:
        cmp    dword ptr [eax + 0ch], esi
        je     SHORT loc_1_0CB8
loc_1_0CB0:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], esi
        jne    SHORT loc_1_0CB0
loc_1_0CB8:
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0CBE:
        push   0
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 0cah]
        push   eax
        db  0E8h, 070h, 0F5h, 0FEh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_0CE8
        cmp    ebx, 103h
        je     SHORT loc_1_0D00
        cmp    ebx, 0c001000eh
        je     SHORT loc_1_0D10
        jmp    SHORT loc_1_0D19
loc_1_0CE8:
        mov    eax, dword ptr [esi + 20h]
        sub    eax, 2
        mov    dword ptr [esi + 20h], eax
        je     SHORT loc_1_0D1C
        mov    byte ptr [esi + 24h], 1
        mov    ebx, 103h
        jmp    SHORT loc_1_0D1C
        db  08Bh, 0FFh
loc_1_0D00:
        mov    byte ptr [esi + 24h], 1
        sub    dword ptr [esi + 20h], 2
        jmp    SHORT loc_1_0D1C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0D10:
        mov    byte ptr [esi + 24h], 1
        mov    ebx, 103h
loc_1_0D19:
        dec    dword ptr [esi + 20h]
loc_1_0D1C:
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_0D80
        mov    eax, dword ptr [edi + 98h]
        cmp    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_0D33
        push   edi
        call   sub_1_3380
loc_1_0D33:
        cmp    dword ptr [edi + 0a6h], 0
        jne    SHORT loc_1_0D65
        cmp    byte ptr [edi + 0dbh], 0
        jne    SHORT loc_1_0D65
        push   0
        push   0
        push   5
        push   esi
        mov    ebx, 103h
        db  068h, 090h, 00Dh, 000h, 000h
        push   esi
        call   sub_1_1D80
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D65:
        dec    dword ptr [edi + 0aah]
        push   0
        push   25h
        push   esi
        call   sub_1_050C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0D80:
        mov    ebx, 103h
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0D90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   esi
        call   sub_1_050C
        dec    dword ptr [edi + 0aah]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_0DC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [eax + 24h], 0
        je     SHORT loc_1_0DE0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0DE0:
        cmp    dword ptr [esi + 0aeh], 0
        je     SHORT loc_1_0DF5
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0DF5:
        mov    eax, dword ptr [ebp + 8]
        push   40010004h
        inc    dword ptr [eax + 20h]
        push   esi
        call   sub_1_1120
        mov    dword ptr [esi + 0aeh], 1
        mov    ecx, dword ptr [esi + 10dh]
        or     ecx, ecx
        je     NEAR PTR loc_1_0EAB
        mov    edi, edi
loc_1_0E20:
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 10dh], eax
        dec    dword ptr [esi + 115h]
        mov    edi, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 20h]
        dec    eax
        mov    dword ptr [edi + 20h], eax
        jne    SHORT loc_1_0E9D
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [ebx + 98h], eax
        jne    SHORT loc_1_0E5D
        push   ebx
        call   sub_1_3380
loc_1_0E5D:
        cmp    dword ptr [ebx + 0a6h], 0
        jne    SHORT loc_1_0E83
        cmp    byte ptr [ebx + 0dbh], 0
        jne    SHORT loc_1_0E83
        push   0
        push   0
        push   5
        push   edi
        db  068h, 090h, 00Dh, 000h, 000h
        push   edi
        call   sub_1_1D80
        jmp    SHORT loc_1_0E9D
loc_1_0E83:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0aah]
loc_1_0E9D:
        mov    ecx, dword ptr [esi + 10dh]
        or     ecx, ecx
        jne    NEAR PTR loc_1_0E20
loc_1_0EAB:
        cmp    byte ptr [esi + 0dbh], 0
        jne    SHORT loc_1_0ED5
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 020h, 010h, 000h, 000h
        push   eax
        call   sub_1_1D80
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0ED5:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0F10
        or     al, al
        jne    SHORT loc_1_0F00
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   80010004h
        call   sub_1_1020
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0F00:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0F10:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        push   edi
        call   sub_1_1340
        or     al, al
        je     NEAR PTR loc_1_1010
        push   edi
        call   sub_1_1B70
        or     al, al
        je     NEAR PTR loc_1_1010
        mov    word ptr [edi + 100h], 0
        mov    word ptr [edi + 102h], 0
        mov    word ptr [edi + 104h], 0
        mov    word ptr [edi + 106h], 0
        mov    dword ptr [edi + 0fch], 0
        mov    byte ptr [edi + 10ch], 0
        mov    byte ptr [edi + 125h], 0
        mov    dword ptr [edi + 10dh], 0
        mov    dword ptr [edi + 111h], 0
        mov    dword ptr [edi + 115h], 0
        mov    dword ptr [edi + 119h], 0
        mov    byte ptr [edi + 0dbh], 0
        push   0
        push   0
        push   4
        push   esi
        db  068h, 040h, 009h, 000h, 000h
        push   esi
        call   sub_1_1D80
        lea    eax, [edi + 5ch]
        push   eax
        push   0
        push   3
        push   esi
        db  068h, 040h, 009h, 000h, 000h
        push   esi
        call   sub_1_1D80
        push   0
        mov    ax, word ptr [edi + 9ch]
        push   eax
        push   7
        push   0
        db  068h, 040h, 009h, 000h, 000h
        push   esi
        call   sub_1_1D80
        push   0
        push   0
        push   8
        push   0
        db  068h, 040h, 009h, 000h, 000h
        push   esi
        call   sub_1_1D80
        push   0
        push   0
        push   6
        push   esi
        db  068h, 020h, 010h, 000h, 000h
        push   esi
        call   sub_1_1D80
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1010:
        mov    byte ptr [edi + 0dbh], 1
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_1020:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   40010005h
        mov    edi, dword ptr [esi + 8]
        push   edi
        call   sub_1_1120
        mov    dword ptr [edi + 0aeh], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0c0000001h
        jne    SHORT loc_1_104F
        mov    eax, 80010004h
loc_1_104F:
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0456
        push   edi
        call   sub_1_10D0
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 20h]
        dec    eax
        mov    dword ptr [esi + 20h], eax
        jne    SHORT loc_1_10BF
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [edi + 98h], eax
        jne    SHORT loc_1_107B
        push   edi
        call   sub_1_3380
loc_1_107B:
        cmp    dword ptr [edi + 0a6h], 0
        jne    SHORT loc_1_10A5
        cmp    byte ptr [edi + 0dbh], 0
        jne    SHORT loc_1_10A5
        push   0
        push   0
        push   5
        push   esi
        db  068h, 090h, 00Dh, 000h, 000h
        push   esi
        call   sub_1_1D80
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_10A5:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   esi
        call   sub_1_050C
        dec    dword ptr [edi + 0aah]
loc_1_10BF:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0ech]
        or     ecx, ecx
        je     SHORT loc_1_1115
loc_1_10E1:
        mov    eax, dword ptr [esi + 0ech]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0ech], eax
        mov    eax, dword ptr [esi + 0a6h]
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [esi + 0a6h], ecx
        push   0
        mov    eax, dword ptr [ecx]
        push   eax
        call   sub_1_043E
        mov    ecx, dword ptr [esi + 0ech]
        or     ecx, ecx
        jne    SHORT loc_1_10E1
loc_1_1115:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0a6h]
        or     esi, esi
        je     NEAR PTR loc_1_11C8
loc_1_113A:
        inc    dword ptr [esi + 20h]
        mov    edi, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 4], 0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0474
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 20h]
        dec    eax
        mov    dword ptr [esi + 20h], eax
        jne    SHORT loc_1_11BE
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [ebx + 98h], eax
        jne    SHORT loc_1_117E
        push   ebx
        call   sub_1_3380
loc_1_117E:
        cmp    dword ptr [ebx + 0a6h], 0
        jne    SHORT loc_1_11A4
        cmp    byte ptr [ebx + 0dbh], 0
        jne    SHORT loc_1_11A4
        push   0
        push   0
        push   5
        push   esi
        db  068h, 090h, 00Dh, 000h, 000h
        push   esi
        call   sub_1_1D80
        jmp    SHORT loc_1_11BE
loc_1_11A4:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   esi
        call   sub_1_050C
        dec    dword ptr [ebx + 0aah]
loc_1_11BE:
        mov    esi, edi
        or     edi, edi
        jne    NEAR PTR loc_1_113A
loc_1_11C8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 54h]
        and    eax, 8000h
        add    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [esi + 0f4h], eax
        mov    eax, dword ptr [esi + 54h]
        and    eax, 0ff00h
        add    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [esi + 0f8h], eax
        mov    ecx, dword ptr [esi + 46h]
        mov    eax, ecx
        shl    eax, 4
        add    eax, ecx
        lea    edi, [eax*4]
        xor    eax, eax
        db  066h, 08Bh, 087h, 054h, 058h, 000h, 000h
        cmp    eax, 2
        jne    SHORT loc_1_1264
        cmp    ecx, 3
        jne    SHORT loc_1_1245
        mov    edx, dword ptr [esi + 0c6h]
        inc    dx
        mov    eax, dword ptr [esi + 2eh]
        shr    eax, 14h
        and    al, 0fh
        out    dx, al
loc_1_1245:
        mov    eax, dword ptr [esi + 2eh]
        shr    eax, 0ch
        and    al, 0fch
        mov    dx, word ptr [esi + 0c6h]
        out    dx, al
        cmp    ecx, 4
        jne    SHORT loc_1_1264
        mov    dx, word ptr [esi + 0c2h]
        mov    al, 1
        out    dx, al
loc_1_1264:
        xor    eax, eax
        db  066h, 08Bh, 087h, 05Eh, 058h, 000h, 000h
        add    eax, 1000h
        shl    eax, 4
        sub    eax, dword ptr [esi + 50h]
        shr    eax, 4
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0b2h], al
        mov    ecx, dword ptr [esi + 46h]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        db  066h, 08Bh, 004h, 08Dh, 05Ah, 058h, 000h, 000h
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0b3h], al
        mov    ecx, dword ptr [esi + 46h]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        db  066h, 08Bh, 004h, 08Dh, 05Ah, 058h, 000h, 000h
        add    eax, 800h
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0b4h], al
        mov    ecx, dword ptr [esi + 46h]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        db  066h, 08Bh, 004h, 08Dh, 058h, 058h, 000h, 000h
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0b5h], al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1300:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 46h]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        lea    esi, [eax*4]
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 4
        xor    ecx, ecx
        db  066h, 08Bh, 08Eh, 056h, 058h, 000h, 000h
        sub    edx, edx
        div    ecx
        xor    ecx, ecx
        mov    cl, al
        db  08Ah, 08Ch, 00Eh, 034h, 058h, 000h, 000h
        and    cl, 0fdh
        mov    al, cl
        pop    esi
        pop    ebp
        ret    8
sub_1_1340:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        sub    al, al
        cmp    dword ptr [esi + 46h], 4
        jne    SHORT loc_1_1378
        push   esi
        call   sub_1_13E0
        or     al, al
        jne    SHORT loc_1_137C
        push   esi
        call   sub_1_13E0
        or     al, al
        jne    SHORT loc_1_137C
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_1378:
        or     al, al
        je     SHORT loc_1_1382
loc_1_137C:
        cmp    dword ptr [esi + 46h], 4
        je     SHORT loc_1_138E
loc_1_1382:
        push   esi
        call   sub_1_1520
        cmp    dword ptr [esi + 46h], 4
        jne    SHORT loc_1_1394
loc_1_138E:
        cmp    byte ptr [esi + 4ah], 0
        je     SHORT loc_1_13A5
loc_1_1394:
        push   esi
        call   sub_1_15C0
        or     al, al
        jne    SHORT loc_1_13A5
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13A5:
        push   esi
        call   sub_1_16D0
        or     al, al
        jne    SHORT loc_1_13B6
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13B6:
        push   esi
        call   sub_1_17B0
        or     al, al
        jne    SHORT loc_1_13C7
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13C7:
        push   esi
        call   sub_1_1810
        or     al, al
        jne    SHORT loc_1_13D8
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13D8:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_13E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   7d0h
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 8], eax
        mov    al, 1
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
        call   sub_1_0500
        mov    dx, word ptr [esi + 0beh]
        sub    al, al
        out    dx, al
        push   7d0h
        call   sub_1_0500
        mov    dx, word ptr [esi + 0c2h]
        mov    al, 81h
        out    dx, al
        mov    al, byte ptr [esi + 0b2h]
        or     al, 2
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        mov    ax, word ptr [eax + 96h]
        mov    word ptr [ebp - 2], ax
        mov    ebx, 0aah
        lea    esp, [esp]
loc_1_1450:
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_1_1477
        push   3e8h
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        inc    edi
        mov    ax, word ptr [eax + 96h]
        mov    word ptr [ebp - 2], ax
        cmp    edi, 5dch
        jb     SHORT loc_1_1450
loc_1_1477:
        cmp    word ptr [ebp - 2], 0aah
        jne    SHORT loc_1_14F0
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        mov    word ptr [eax + 92h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, 55h
        mov    ax, word ptr [eax + 96h]
        mov    word ptr [ebp - 2], ax
loc_1_14A0:
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_1_14C7
        push   3e8h
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        inc    edi
        mov    ax, word ptr [eax + 96h]
        mov    word ptr [ebp - 2], ax
        cmp    edi, 1f4h
        jb     SHORT loc_1_14A0
loc_1_14C7:
        cmp    word ptr [ebp - 2], 55h
        jne    SHORT loc_1_14F0
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        mov    dx, word ptr [esi + 0c2h]
        mov    al, 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_14F0:
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        mov    dx, word ptr [esi + 0c2h]
        mov    al, 1
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1520:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 46h]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        xor    ecx, ecx
        db  066h, 08Bh, 00Ch, 085h, 062h, 058h, 000h, 000h
        mov    edi, dword ptr [esi + 0f0h]
        and    ecx, dword ptr [esi + 54h]
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ecx + edi], 0
        mov    word ptr [eax + 92h], 0
        mov    eax, dword ptr [ebp - 4]
        push   3e8h
        mov    word ptr [eax + 94h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 96h], 0
        mov    al, 1
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
        call   sub_1_0500
        mov    dx, word ptr [esi + 0beh]
        sub    al, al
        out    dx, al
        push   7a120h
        call   sub_1_0500
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_15C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 46h]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 2
        xor    edi, edi
        db  066h, 08Bh, 0B8h, 062h, 058h, 000h, 000h
        and    edi, dword ptr [esi + 54h]
        add    edi, dword ptr [esi + 0f0h]
        mov    dword ptr [ebp - 0ch], 1
        db  0F6h, 080h, 076h, 058h, 000h, 000h, 080h
        je     SHORT loc_1_160B
        mov    dword ptr [ebp - 0ch], 2
loc_1_160B:
        sub    ebx, ebx
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        sub    esi, esi
        lea    ecx, [ecx]
loc_1_1620:
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [ebp - 1], 80h
        mov    ax, word ptr [eax + 94h]
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_1_1692
        cmp    byte ptr [ebp - 1], 0ffh
        je     SHORT loc_1_1692
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 4], al
        jne    SHORT loc_1_1692
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        jne    SHORT loc_1_1692
        cmp    al, 0aah
        jne    SHORT loc_1_16C0
        inc    ebx
        cmp    ebx, dword ptr [ebp - 0ch]
        je     SHORT loc_1_16B4
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi], 0
        mov    word ptr [eax + 92h], 0eh
        push   3d090h
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi], 0ffh
        mov    word ptr [eax + 94h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 92h], 3
loc_1_1692:
        push   3e8h
        call   sub_1_0500
        inc    esi
        cmp    esi, 4e20h
        jb     NEAR PTR loc_1_1620
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_16B4:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_16C0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_16D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0f8h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 46h]
        mov    edx, eax
        shl    eax, 4
        add    eax, edx
        db  066h, 08Bh, 004h, 085h, 054h, 058h, 000h, 000h
        and    eax, 0ffffh
        cmp    eax, 1
        je     SHORT loc_1_1757
        cmp    eax, 2
        je     SHORT loc_1_1781
        mov    esi, dword ptr [ebp - 0ch]
loc_1_1709:
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        mov    ax, word ptr [eax + 92h]
        mov    word ptr [ebp - 2], ax
        cmp    esi, edi
        je     SHORT loc_1_1745
        mov    ebx, 1
loc_1_1722:
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_1_1745
        push   3e8h
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        inc    edi
        mov    ax, word ptr [eax + 92h]
        mov    word ptr [ebp - 2], ax
        cmp    edi, esi
        jb     SHORT loc_1_1722
loc_1_1745:
        cmp    word ptr [ebp - 2], 1
        je     SHORT loc_1_17A5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1757:
        mov    dx, word ptr [ecx + 0beh]
        mov    al, byte ptr [ecx + 0b2h]
        out    dx, al
        mov    esi, 1f4h
        mov    edx, dword ptr [ecx + 54h]
        and    edx, 0fefah
        mov    eax, dword ptr [ecx + 0f0h]
        mov    word ptr [edx + eax], 5a5ah
        jmp    SHORT loc_1_1709
loc_1_1781:
        mov    dx, word ptr [ecx + 0beh]
        mov    al, byte ptr [ecx + 0b2h]
        out    dx, al
        mov    eax, dword ptr [ebp - 8]
        mov    esi, 1388h
        mov    word ptr [eax + 92h], 0dh
        jmp    NEAR PTR loc_1_1709
loc_1_17A5:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_17B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    edi, [esi + 62h]
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 4], eax
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0f0h
        push   eax
        push   edi
        call   sub_1_05EC
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_1806
        cmp    byte ptr [esi + 63h], 0ddh
        jne    SHORT loc_1_1806
        mov    al, byte ptr [esi + 64h]
        cmp    al, 1
        je     SHORT loc_1_17FC
        or     al, al
        jne    SHORT loc_1_1806
loc_1_17FC:
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1806:
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_1810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, 1
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b5h]
        out    dx, al
        mov    edi, dword ptr [esi + 0f0h]
        sub    edx, edx
        test   dword ptr [esi + 50h], 0fffffffch
        jbe    SHORT loc_1_1854
        mov    eax, edi
        nop
loc_1_1840:
        mov    dword ptr [eax], 0f55ccf0h
        add    eax, 4
        inc    edx
        mov    ebx, dword ptr [esi + 50h]
        shr    ebx, 2
        cmp    ebx, edx
        ja     SHORT loc_1_1840
loc_1_1854:
        sub    eax, eax
        test   dword ptr [esi + 50h], 0fffffffch
        jbe    SHORT loc_1_187A
        mov    edx, 0f55ccf0h
loc_1_1864:
        mov    ebx, dword ptr [edi]
        cmp    ebx, edx
        je     SHORT loc_1_186C
        sub    cl, cl
loc_1_186C:
        add    edi, 4
        inc    eax
        mov    ebx, dword ptr [esi + 50h]
        shr    ebx, 2
        cmp    ebx, eax
        ja     SHORT loc_1_1864
loc_1_187A:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1890:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0f0h]
        mov    dword ptr [ebp - 8], edi
        mov    ebx, dword ptr [esi + 0f4h]
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi + 46h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        mov    edx, dword ptr [esi + 50h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 90h], 40h
        mov    word ptr [eax + 92h], 1
        mov    dword ptr [eax + 0b0h], 0ffffffffh
        mov    dword ptr [eax + 0c0h], 0ffffffffh
        mov    dword ptr [eax + 0c4h], 0ffffffffh
        mov    dword ptr [eax + 44h], 0ffffffffh
        mov    dword ptr [eax + 1ch], 0
        mov    ecx, 2a6h
        mov    byte ptr [eax + 0e0h], 0ffh
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        mov    edi, dword ptr [esi + 0f0h]
        sub    eax, eax
        rep stosd
        mov    al, byte ptr [ebp - 0ch]
        mov    byte ptr [ebx + 8c1h], al
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 4
        add    eax, dword ptr [ebp - 0ch]
        lea    ecx, [eax*4]
        db  0F6h, 081h, 076h, 058h, 000h, 000h, 020h
        je     SHORT loc_1_197E
        mov    ax, word ptr [ebx + 8a2h]
        mov    word ptr [ebp - 2], ax
        or     word ptr [ebp - 2], 4000h
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebx + 8a2h], ax
loc_1_197E:
        db  08Ah, 081h, 06Ch, 058h, 000h, 000h
        mov    byte ptr [ebx + 933h], al
        db  08Ah, 081h, 06Dh, 058h, 000h, 000h
        mov    byte ptr [ebx + 934h], al
        sub    al, al
        cmp    dword ptr [ebp - 0ch], 4
        jne    SHORT loc_1_19A8
        movsx  eax, byte ptr [esi + 4bh]
        db  08Ah, 080h, 054h, 055h, 000h, 000h
loc_1_19A8:
        mov    byte ptr [ebx + 935h], al
        or     al, 1
        mov    byte ptr [ebx + 936h], al
        mov    eax, dword ptr [esi + 54h]
        mov    word ptr [ebx + 8b6h], ax
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ebx + 8bah], eax
        db  066h, 08Bh, 081h, 066h, 058h, 000h, 000h
        mov    word ptr [ebx + 8a8h], ax
        db  066h, 08Bh, 081h, 068h, 058h, 000h, 000h
        mov    word ptr [ebx + 8aah], ax
        db  066h, 08Bh, 081h, 064h, 058h, 000h, 000h
        mov    word ptr [ebx + 8ach], ax
        db  066h, 08Bh, 081h, 06Ah, 058h, 000h, 000h
        mov    word ptr [ebx + 8aeh], ax
        db  066h, 08Bh, 081h, 06Eh, 058h, 000h, 000h
        mov    word ptr [ebx + 8b0h], ax
        db  066h, 08Bh, 081h, 070h, 058h, 000h, 000h
        mov    word ptr [ebx + 8b2h], ax
        xor    eax, eax
        mov    word ptr [ebx + 8c8h], 3e8h
        mov    word ptr [ebx + 8d4h], 7d0h
        db  066h, 08Bh, 081h, 060h, 058h, 000h, 000h
        shl    eax, 10h
        add    eax, 0fff0h
        mov    dword ptr [ebx + 94ah], eax
        xor    eax, eax
        db  066h, 08Bh, 081h, 060h, 058h, 000h, 000h
        shl    eax, 10h
        add    eax, 0fff6h
        mov    dword ptr [ebx + 94eh], eax
        mov    eax, dword ptr [esi + 32h]
        mov    word ptr [ebx + 8e0h], ax
        mov    eax, dword ptr [esi + 36h]
        mov    word ptr [ebx + 8e6h], ax
        mov    eax, dword ptr [esi + 42h]
        mov    word ptr [ebx + 8eeh], ax
        db  066h, 08Bh, 081h, 058h, 058h, 000h, 000h
        mov    word ptr [ebx + 8e8h], ax
        mov    word ptr [ebx + 8f2h], 5eah
        mov    word ptr [ebx + 920h], 5eah
        mov    word ptr [ebx + 922h], 3ch
        mov    word ptr [ebx + 8ceh], 2710h
        mov    byte ptr [ebx + 926h], 0fh
        mov    byte ptr [ebx + 927h], 2
        mov    byte ptr [ebx + 928h], 0
        mov    byte ptr [ebx + 929h], 60h
        mov    word ptr [ebx + 92ah], 200h
        mov    byte ptr [ebx + 92ch], 0fh
        mov    word ptr [ebx + 924h], 10h
        mov    byte ptr [ebx + 92dh], 0
        mov    byte ptr [ebx + 92eh], 0
        mov    byte ptr [ebx + 92fh], 0
        mov    byte ptr [ebx + 930h], 0
        mov    byte ptr [ebx + 931h], 0
        mov    byte ptr [ebx + 932h], 3ch
        db  066h, 08Bh, 081h, 056h, 058h, 000h, 000h
        mov    word ptr [ebx + 8beh], ax
        db  08Dh, 081h, 034h, 058h, 000h, 000h
        push   20h
        push   eax
        add    ebx, 900h
        push   ebx
        call   sub_1_05EC
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b5h]
        out    dx, al
        push   14eeh
        db  068h, 064h, 040h, 000h, 000h
        mov    eax, dword ptr [esi + 0f0h]
        push   eax
        call   sub_1_05EC
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        mov    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [ebp - 0ch], eax
        mov    edi, dword ptr [esi + 0f4h]
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [esi + 9]
        push   esi
        db  068h, 010h, 024h, 000h, 000h
        push   eax
        call   sub_1_04FA
        push   esi
        call   sub_1_1890
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        mov    byte ptr [edi + 0a20h], 8ch
        mov    byte ptr [edi + 0a21h], 0c8h
        mov    byte ptr [edi + 0a22h], 8eh
        mov    byte ptr [edi + 0a23h], 0d8h
        out    dx, al
        mov    byte ptr [edi + 0a24h], 0eah
        mov    byte ptr [edi + 0a25h], 0
        mov    byte ptr [edi + 0a26h], 0
        mov    eax, dword ptr [esi + 46h]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        db  066h, 08Bh, 004h, 085h, 058h, 058h, 000h, 000h
        mov    word ptr [edi + 0a27h], ax
        mov    dword ptr [esi + 25h], 0
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        mov    eax, dword ptr [esi + 46h]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        db  066h, 08Bh, 004h, 085h, 05Ah, 058h, 000h, 000h
        mov    word ptr [ecx + 0dah], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebp - 4], 8a20h
        mov    word ptr [eax + 0d8h], 8a20h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 92h], 2
        sub    ebx, ebx
        mov    al, byte ptr [esi + 0b4h]
        or     al, 2
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
        mov    ax, word ptr [edi + 8a0h]
        mov    word ptr [ebp - 4], ax
loc_1_1C71:
        test   byte ptr [ebp - 3], 80h
        je     SHORT loc_1_1C7D
        cmp    dword ptr [esi + 25h], 0
        jne    SHORT loc_1_1C9B
loc_1_1C7D:
        push   3e8h
        call   sub_1_0500
        mov    ax, word ptr [edi + 8a0h]
        mov    word ptr [ebp - 4], ax
        inc    ebx
        cmp    ebx, 3e8h
        jb     SHORT loc_1_1C71
loc_1_1C9B:
        test   byte ptr [ebp - 3], 80h
        je     SHORT loc_1_1CA7
        cmp    dword ptr [esi + 25h], 0
        jne    SHORT loc_1_1CAB
loc_1_1CA7:
        mov    byte ptr [ebp - 1], 0
loc_1_1CAB:
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        push   esi
        db  068h, 020h, 024h, 000h, 000h
        lea    eax, [esi + 9]
        push   eax
        call   sub_1_04FA
        cmp    dword ptr [esi + 25h], 0
        jne    SHORT loc_1_1CEA
        movsx  eax, byte ptr [esi + 4bh]
        push   eax
        push   1
        push   0aa000005h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 10h
        mov    byte ptr [ebp - 1], 0
loc_1_1CEA:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1D48
        mov    al, byte ptr [esi + 0b4h]
        or     al, 2
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
        xor    eax, eax
        xor    ecx, ecx
        mov    cx, word ptr [edi + 8f2h]
        mov    ax, word ptr [edi + 8f4h]
        imul   eax, ecx
        mov    dword ptr [esi + 68h], eax
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 8eeh]
        mov    cx, word ptr [edi + 8f0h]
        imul   ecx, eax
        mov    dword ptr [esi + 6ch], ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 8f2h]
        mov    dword ptr [esi + 70h], eax
        xor    eax, eax
        mov    ax, word ptr [edi + 8eeh]
        mov    dword ptr [esi + 74h], eax
loc_1_1D48:
        mov    al, byte ptr [esi + 0b3h]
        or     al, 2
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        mov    byte ptr [eax + 0e0h], 0
        mov    byte ptr [eax + 0e1h], 0
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 100h]
        cmp    eax, dword ptr [ecx + 36h]
        jb     SHORT loc_1_1DA3
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    18h
loc_1_1DA3:
        cmp    byte ptr [ecx + 0dbh], 0
        je     SHORT loc_1_1DB5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    18h
loc_1_1DB5:
        xor    esi, esi
        inc    dword ptr [edi + 20h]
        mov    si, word ptr [ecx + 102h]
        inc    word ptr [ecx + 100h]
        lea    esi, [esi + esi*2]
        shl    esi, 3
        xor    eax, eax
        add    esi, dword ptr [ecx + 108h]
        mov    ax, word ptr [ecx + 102h]
        inc    eax
        sub    edx, edx
        div    dword ptr [ecx + 36h]
        mov    word ptr [ecx + 102h], dx
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    ax, word ptr [ebp + 14h]
        cmp    ax, 3
        mov    word ptr [esi + 0ch], ax
        mov    dx, word ptr [ebp + 18h]
        mov    word ptr [esi + 10h], dx
        je     SHORT loc_1_1E14
        cmp    ax, 9
        jne    SHORT loc_1_1E37
loc_1_1E14:
        sub    di, di
        mov    eax, dword ptr [ebp + 1ch]
        lea    ebx, [ebx]
loc_1_1E20:
        mov    ebx, edi
        and    ebx, 0ffffh
        mov    dl, byte ptr [eax + ebx]
        mov    byte ptr [ebx + esi + 12h], dl
        inc    di
        cmp    di, 6
        jb     SHORT loc_1_1E20
loc_1_1E37:
        push   ecx
        call   sub_1_2690
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0f4h]
        mov    dword ptr [ebp - 8], edx
        cmp    byte ptr [esi + 0dbh], 0
        jne    NEAR PTR loc_1_1FAD
        mov    ax, word ptr [esi + 102h]
        cmp    word ptr [esi + 106h], ax
        je     NEAR PTR loc_1_1F9D
loc_1_1E86:
        mov    cx, word ptr [esi + 106h]
        mov    eax, ecx
        and    eax, 0ffffh
        lea    ebx, [eax + eax*2]
        shl    ebx, 3
        add    ebx, dword ptr [esi + 108h]
        cmp    word ptr [ebx + 0ch], 8
        jne    SHORT loc_1_1EB4
        cmp    byte ptr [esi + 10ch], 0
        jne    NEAR PTR loc_1_1FAD
loc_1_1EB4:
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 36h]
        mov    word ptr [esi + 106h], dx
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        mov    eax, dword ptr [esi + 54h]
        mov    edx, dword ptr [ebp - 8]
        xor    edi, edi
        mov    di, word ptr [edx + 4eh]
        and    edi, eax
        mov    edx, dword ptr [esi + 0f0h]
        add    edx, edi
        mov    dword ptr [ebp - 4], edx
        movzx  edi, byte ptr [edx + 5]
        mov    dl, byte ptr [edx]
        and    edx, 0ffh
        imul   edi, edx
        mov    edx, dword ptr [ebp - 4]
        mov    dx, word ptr [edx + 6]
        and    edx, 0ffffh
        and    edx, eax
        add    edi, edx
        add    edi, dword ptr [esi + 0f0h]
        mov    ax, word ptr [ebx + 0ch]
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], cx
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [edi + 4], ax
        cmp    word ptr [ebx + 0ch], 8
        jne    SHORT loc_1_1F39
        mov    byte ptr [esi + 10ch], 1
        push   esi
        call   sub_1_22A0
        mov    word ptr [edi + 4], ax
loc_1_1F39:
        mov    ax, word ptr [ebx + 0ch]
        cmp    ax, 3
        je     SHORT loc_1_1F49
        cmp    ax, 9
        jne    SHORT loc_1_1F68
loc_1_1F49:
        sub    dx, dx
        lea    esp, [esp]
loc_1_1F50:
        mov    ecx, edx
        and    ecx, 0ffffh
        mov    al, byte ptr [ebx + ecx + 12h]
        mov    byte ptr [ecx + edi + 6], al
        inc    dx
        cmp    dx, 6
        jb     SHORT loc_1_1F50
loc_1_1F68:
        mov    edx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        inc    al
        cmp    byte ptr [edx + 4], al
        jae    SHORT loc_1_1F76
        sub    al, al
loc_1_1F76:
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx], al
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        mov    ax, word ptr [esi + 102h]
        cmp    word ptr [esi + 106h], ax
        jne    NEAR PTR loc_1_1E86
loc_1_1F9D:
        mov    al, byte ptr [esi + 0b3h]
        or     al, 2
        mov    dx, word ptr [esi + 0beh]
        out    dx, al
loc_1_1FAD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0f4h]
        cmp    byte ptr [ecx + 0dbh], 0
        jne    NEAR PTR loc_1_2184
        mov    ax, word ptr [ecx + 104h]
        cmp    word ptr [ecx + 106h], ax
        je     NEAR PTR loc_1_2184
        mov    dx, word ptr [ecx + 0beh]
        mov    al, byte ptr [ecx + 0b4h]
        out    dx, al
        mov    ax, word ptr [esi + 5eh]
        mov    word ptr [ebp - 6], ax
        mov    edx, eax
        and    edx, 0ffffh
        and    edx, dword ptr [ecx + 54h]
        add    edx, dword ptr [ecx + 0f0h]
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ebp - 3], al
        movzx  esi, byte ptr [ebp - 3]
        mov    ax, word ptr [edx + 6]
        mov    word ptr [ebp - 6], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        imul   esi, eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        and    eax, dword ptr [ecx + 54h]
        add    esi, eax
        add    esi, dword ptr [ecx + 0f0h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 3], al
        jne    SHORT loc_1_2071
        mov    al, byte ptr [ecx + 0b3h]
        or     al, 2
        mov    dx, word ptr [ecx + 0beh]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2071:
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebp - 6], ax
        or     ax, ax
        mov    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_208C
        mov    dword ptr [ebp - 0ch], 0c0000001h
loc_1_208C:
        mov    ax, word ptr [esi]
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ebp - 1], al
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        jae    SHORT loc_1_20AE
        mov    byte ptr [ebp - 1], 0
loc_1_20AE:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edx + 2], al
        mov    al, byte ptr [ecx + 0b3h]
        or     al, 2
        mov    dx, word ptr [ecx + 0beh]
        mov    ebx, dword ptr [ebp - 8]
        and    ebx, 0ffffh
        out    dx, al
        lea    ebx, [ebx + ebx*2]
        shl    ebx, 3
        add    ebx, dword ptr [ecx + 108h]
        mov    esi, dword ptr [ebx]
        mov    di, word ptr [ebx + 0ch]
        cmp    di, 8
        jne    SHORT loc_1_20EC
        mov    byte ptr [ecx + 10ch], 0
loc_1_20EC:
        xor    eax, eax
        dec    word ptr [ecx + 100h]
        mov    ax, word ptr [ecx + 104h]
        inc    eax
        sub    edx, edx
        div    dword ptr [ecx + 36h]
        mov    word ptr [ecx + 104h], dx
        inc    dword ptr [ecx + 0fch]
        mov    eax, dword ptr [ebx + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [ebx + 8]
        cmp    di, 5
        je     SHORT loc_1_2184
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 20h]
        dec    eax
        mov    dword ptr [esi + 20h], eax
        jne    SHORT loc_1_2184
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [edi + 98h], eax
        jne    SHORT loc_1_213D
        push   edi
        call   sub_1_3380
loc_1_213D:
        cmp    dword ptr [edi + 0a6h], 0
        jne    SHORT loc_1_216A
        cmp    byte ptr [edi + 0dbh], 0
        jne    SHORT loc_1_216A
        push   0
        push   0
        push   5
        push   esi
        db  068h, 090h, 00Dh, 000h, 000h
        push   esi
        call   sub_1_1D80
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_216A:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   esi
        call   sub_1_050C
        dec    dword ptr [edi + 0aah]
loc_1_2184:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2190:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 102h]
        cmp    word ptr [esi + 104h], ax
        je     NEAR PTR loc_1_227E
loc_1_21B0:
        xor    eax, eax
        mov    ax, word ptr [esi + 104h]
        lea    eax, [eax + eax*2]
        shl    eax, 3
        add    eax, dword ptr [esi + 108h]
        mov    ebx, dword ptr [eax]
        mov    di, word ptr [eax + 0ch]
        cmp    di, 8
        jne    SHORT loc_1_21D8
        mov    byte ptr [esi + 10ch], 0
loc_1_21D8:
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        push   0c0000001h
        call   dword ptr [eax + 8]
        xor    eax, eax
        inc    dword ptr [esi + 0fch]
        mov    ax, word ptr [esi + 104h]
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 36h]
        mov    word ptr [esi + 104h], dx
        dec    word ptr [esi + 100h]
        cmp    di, 5
        je     SHORT loc_1_226A
        mov    edi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 20h]
        dec    eax
        mov    dword ptr [ebx + 20h], eax
        jne    SHORT loc_1_226A
        mov    eax, dword ptr [ebx + 1ch]
        cmp    dword ptr [edi + 98h], eax
        jne    SHORT loc_1_222A
        push   edi
        call   sub_1_3380
loc_1_222A:
        cmp    dword ptr [edi + 0a6h], 0
        jne    SHORT loc_1_2250
        cmp    byte ptr [edi + 0dbh], 0
        jne    SHORT loc_1_2250
        push   0
        push   0
        push   5
        push   ebx
        db  068h, 090h, 00Dh, 000h, 000h
        push   ebx
        call   sub_1_1D80
        jmp    SHORT loc_1_226A
loc_1_2250:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   ebx
        call   sub_1_050C
        dec    dword ptr [edi + 0aah]
loc_1_226A:
        mov    ax, word ptr [esi + 102h]
        cmp    word ptr [esi + 104h], ax
        jne    NEAR PTR loc_1_21B0
loc_1_227E:
        mov    ax, word ptr [esi + 104h]
        mov    word ptr [esi + 106h], ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_22A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0f4h]
        mov    eax, dword ptr [esi + 32h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        lea    eax, [edi + ecx]
        add    eax, 0a98h
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0cah]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 084h, 0E4h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 3
        jne    SHORT loc_1_2310
        sub    edx, edx
loc_1_22E4:
        mov    eax, dword ptr [esi + 32h]
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2]
        mov    al, byte ptr [edi + eax + 0aa4h]
        mov    cl, al
        mov    eax, dword ptr [esi + 32h]
        inc    edx
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2 - 1]
        mov    byte ptr [edi + eax + 0aaah], cl
        cmp    edx, 6
        jb     SHORT loc_1_22E4
        inc    dword ptr [ebp - 8]
loc_1_2310:
        mov    eax, dword ptr [ebp - 8]
        imul   ax, ax, 6
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2320:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0f0h]
        cmp    byte ptr [eax + 29h], 0
        je     SHORT loc_1_2350
        inc    dword ptr [eax + 25h]
        mov    dx, word ptr [eax + 0beh]
        mov    al, byte ptr [eax + 0b4h]
        out    dx, al
        or     al, 2
        out    dx, al
        sub    al, al
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2350:
        cmp    byte ptr [eax + 125h], 0
        je     SHORT loc_1_2360
        sub    al, al
        pop    ebx
        pop    ebp
        ret    4
loc_1_2360:
        mov    dx, word ptr [eax + 0beh]
        mov    bl, byte ptr [eax + 0b3h]
        mov    al, bl
        or     al, 2
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    byte ptr [ecx + 0e0h], 0ffh
        mov    byte ptr [ecx + 0e1h], 0
        mov    byte ptr [ecx + 0e2h], 0
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_23A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 0f0h]
        cmp    byte ptr [esi + 125h], 0
        jne    SHORT loc_1_2405
        mov    byte ptr [esi + 125h], 1
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        push   esi
        call   sub_1_27D0
        push   esi
        call   sub_1_26D0
        push   esi
        call   sub_1_1E50
        push   esi
        call   sub_1_1FC0
        mov    byte ptr [esi + 125h], 0
        mov    cl, byte ptr [esi + 0b3h]
        or     cl, 2
        mov    dx, word ptr [esi + 0beh]
        mov    al, cl
        out    dx, al
        mov    byte ptr [edi + 0e0h], 0
loc_1_2405:
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2410:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 29h], 1
        mov    al, 1
        ret    4
loc_1_2420:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 29h], 0
        mov    al, 1
        ret    4
loc_1_2430:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebx + 8]
        lea    eax, [esi + 24h]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_0530
        cmp    dword ptr [ebp - 8], 0ch
        jb     NEAR PTR loc_1_2500
        cmp    dword ptr [ebp - 8], 5eah
        ja     NEAR PTR loc_1_2500
        cmp    byte ptr [ebx + 24h], 0
        je     SHORT loc_1_2483
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2483:
        cmp    byte ptr [edi + 0dbh], 0
        je     SHORT loc_1_24A0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_24A0:
        cmp    dword ptr [edi + 0aeh], 0
        je     SHORT loc_1_24B7
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_24B7:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [ebx + 20h]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        cmp    dword ptr [edi + 10dh], 0
        jne    SHORT loc_1_24D7
        mov    dword ptr [edi + 10dh], esi
        jmp    SHORT loc_1_24E0
loc_1_24D7:
        mov    eax, dword ptr [edi + 111h]
        mov    dword ptr [eax + 24h], esi
loc_1_24E0:
        mov    dword ptr [edi + 111h], esi
        inc    dword ptr [edi + 115h]
        push   edi
        call   sub_1_2690
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2500:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_2510:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0f4h]
        mov    dx, word ptr [edi + 0beh]
        mov    al, byte ptr [edi + 0b4h]
        out    dx, al
        mov    al, byte ptr [esi + 482h]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [esi + 480h]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        jne    SHORT loc_1_2563
        mov    dx, word ptr [edi + 0beh]
        mov    al, byte ptr [edi + 0b3h]
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2563:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        mov    ax, word ptr [esi + eax*2 + 490h]
        mov    ebx, eax
        and    ebx, 0ffffh
        add    ebx, dword ptr [edi + 0f0h]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 8], 3ch
        jae    SHORT loc_1_259E
        mov    dword ptr [ebp - 8], 3ch
loc_1_259E:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [ebp - 8]
        or     ax, 8000h
        mov    word ptr [ebx], ax
        mov    ax, word ptr [ebx + 0eh]
        mov    ecx, eax
        and    ecx, 0ffffh
        add    ecx, dword ptr [edi + 0f0h]
        mov    al, byte ptr [ebx + 7]
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [edi + 0beh]
        out    dx, al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   sub_1_2620
        mov    dx, word ptr [edi + 0beh]
        mov    al, byte ptr [edi + 0b4h]
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        inc    al
        mov    byte ptr [esi + 482h], al
        mov    al, byte ptr [esi + 690h]
        mov    byte ptr [ebp - 1], al
        inc    al
        mov    byte ptr [esi + 690h], al
        mov    dx, word ptr [edi + 0beh]
        mov    al, byte ptr [edi + 0b3h]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_2620:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        push   eax
        call   sub_1_0530
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_1_267F
        sub    ebx, ebx
loc_1_2648:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_05EC
        add    esi, dword ptr [ebp - 8]
        add    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_1_2648
loc_1_267F:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2690:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0f0h]
        cmp    byte ptr [ecx + 125h], 0
        jne    SHORT loc_1_26C4
        mov    al, byte ptr [ecx + 0b3h]
        or     al, 2
        mov    dx, word ptr [ecx + 0beh]
        out    dx, al
        mov    byte ptr [esi + 0e1h], 0
        mov    byte ptr [esi + 0e3h], 0ffh
loc_1_26C4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 1], 1
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    ebx, dword ptr [esi + 10dh]
        or     ebx, ebx
        je     NEAR PTR loc_1_27BB
loc_1_26F2:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_27BB
        push   ebx
        push   esi
        call   sub_1_2510
        mov    byte ptr [ebp - 1], al
        or     al, al
        je     NEAR PTR loc_1_27A0
        mov    byte ptr [ebp - 2], al
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 10dh], eax
        dec    dword ptr [esi + 115h]
        mov    eax, dword ptr [esi + 0f0h]
        inc    dword ptr [esi + 119h]
        mov    byte ptr [eax + 0e4h], 0
        mov    edi, dword ptr [ebx + 28h]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 20h]
        dec    eax
        mov    dword ptr [edi + 20h], eax
        jne    SHORT loc_1_27AD
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [ebx + 98h], eax
        jne    SHORT loc_1_275E
        push   ebx
        call   sub_1_3380
loc_1_275E:
        cmp    dword ptr [ebx + 0a6h], 0
        jne    SHORT loc_1_2784
        cmp    byte ptr [ebx + 0dbh], 0
        jne    SHORT loc_1_2784
        push   0
        push   0
        push   5
        push   edi
        db  068h, 090h, 00Dh, 000h, 000h
        push   edi
        call   sub_1_1D80
        jmp    SHORT loc_1_27AD
loc_1_2784:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   25h
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0aah]
        jmp    SHORT loc_1_27AD
loc_1_27A0:
        mov    eax, dword ptr [esi + 0f0h]
        mov    byte ptr [eax + 0e4h], 1
loc_1_27AD:
        mov    ebx, dword ptr [esi + 10dh]
        or     ebx, ebx
        jne    NEAR PTR loc_1_26F2
loc_1_27BB:
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_27D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], 0
        mov    edi, dword ptr [esi + 0f4h]
        cmp    dword ptr [esi + 0aeh], 0
        je     SHORT loc_1_2800
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2800:
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        mov    al, byte ptr [edi + 62h]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 60h]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        jne    SHORT loc_1_2840
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_2840:
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        je     NEAR PTR loc_1_297A
        lea    esp, [esp]
loc_1_2850:
        cmp    dword ptr [esi + 0aeh], 0
        jne    NEAR PTR loc_1_297A
        cmp    dword ptr [ebp - 10h], 0ah
        jbe    SHORT loc_1_288C
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        push   esi
        call   sub_1_26D0
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        mov    dword ptr [ebp - 10h], 0
loc_1_288C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        inc    dword ptr [ebp - 10h]
        mov    ax, word ptr [edi + eax*2 + 70h]
        mov    word ptr [ebp - 6], ax
        mov    ecx, eax
        and    ecx, 0ffffh
        add    ecx, dword ptr [esi + 0f0h]
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        mov    ax, word ptr [ecx + 4]
        mov    ebx, dword ptr [esi + 42h]
        mov    edx, eax
        and    edx, 0ffffh
        add    edx, dword ptr [esi + 0f0h]
        mov    ax, word ptr [ecx + 0ch]
        and    eax, 0ffffh
        cmp    ebx, eax
        jb     SHORT loc_1_28DD
        mov    ebx, eax
loc_1_28DD:
        mov    dword ptr [ebp - 0ch], edx
        or     edx, edx
        je     SHORT loc_1_2926
        mov    dword ptr [esi + 11dh], ecx
        mov    dword ptr [esi + 121h], eax
        cmp    eax, 0eh
        jae    SHORT loc_1_2903
        cmp    eax, 6
        jb     SHORT loc_1_2926
        push   0
        push   0
        push   0
        push   eax
        jmp    SHORT loc_1_2914
loc_1_2903:
        sub    eax, 0eh
        push   eax
        sub    ebx, 0eh
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0eh
        push   eax
        push   0eh
loc_1_2914:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0cah]
        push   eax
        call   sub_1_00B0
loc_1_2926:
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b4h]
        out    dx, al
        inc    byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi + 62h], al
        mov    al, byte ptr [edi + 270h]
        xor    ecx, ecx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 6]
        mov    cl, byte ptr [ebp - 1]
        mov    word ptr [edi + ecx*2 + 280h], ax
        mov    al, byte ptr [ebp - 1]
        inc    al
        mov    byte ptr [edi + 270h], al
        mov    al, byte ptr [edi + 62h]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 60h]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        jne    NEAR PTR loc_1_2850
loc_1_297A:
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b3h]
        out    dx, al
        mov    eax, dword ptr [esi + 0cah]
        push   eax
        call   sub_1_0290
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_29A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
        add    ebx, 0eh
        mov    edi, dword ptr [ecx + 11dh]
        mov    eax, dword ptr [ecx + 42h]
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [ecx + 121h]
        mov    eax, dword ptr [ebp + 14h]
        lea    edx, [ebx + eax]
        cmp    edx, ecx
        jbe    SHORT loc_1_29D5
        sub    ecx, ebx
        mov    dword ptr [ebp - 24h], ecx
        jmp    SHORT loc_1_29D8
loc_1_29D5:
        mov    dword ptr [ebp - 24h], eax
loc_1_29D8:
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dx, word ptr [ecx + 0beh]
        mov    al, byte ptr [ecx + 0b3h]
        out    dx, al
        mov    eax, ebx
        sub    edx, edx
        div    dword ptr [ebp - 14h]
        or     eax, eax
        je     SHORT loc_1_2A18
        mov    edx, eax
        lea    esp, [esp]
loc_1_2A00:
        mov    cx, word ptr [edi + 2]
        mov    edi, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    edi, 0ffffh
        add    edi, dword ptr [ecx + 0f0h]
        dec    edx
        jne    SHORT loc_1_2A00
loc_1_2A18:
        mov    ecx, dword ptr [ebp - 14h]
        imul   ecx, eax
        sub    ebx, ecx
        mov    ax, word ptr [edi + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        add    eax, dword ptr [ecx + 0f0h]
        mov    dword ptr [ebp - 20h], eax
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_0530
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0554
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_2B0D
loc_1_2A6A:
        mov    esi, dword ptr [ebp - 18h]
        sub    esi, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0ch], esi
        jae    SHORT loc_1_2A78
        mov    esi, dword ptr [ebp - 0ch]
loc_1_2A78:
        lea    eax, [ebx + esi]
        cmp    eax, dword ptr [ebp - 14h]
        jbe    SHORT loc_1_2A85
        mov    esi, dword ptr [ebp - 14h]
        sub    esi, ebx
loc_1_2A85:
        push   esi
        mov    eax, dword ptr [ebp - 20h]
        add    eax, ebx
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_05EC
        add    ebx, esi
        sub    dword ptr [ebp - 0ch], esi
        je     SHORT loc_1_2B0D
        cmp    dword ptr [ebp - 14h], ebx
        jne    SHORT loc_1_2ACD
        mov    ax, word ptr [edi + 2]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, eax
        and    edi, 0ffffh
        add    edi, dword ptr [ecx + 0f0h]
        sub    ebx, ebx
        mov    ax, word ptr [edi + 4]
        and    eax, 0ffffh
        add    eax, dword ptr [ecx + 0f0h]
        mov    dword ptr [ebp - 20h], eax
loc_1_2ACD:
        add    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 18h]
        jne    SHORT loc_1_2B03
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_2B0D
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0554
        mov    dword ptr [ebp - 8], 0
loc_1_2B03:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_2A6A
loc_1_2B0D:
        mov    ecx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp + 1ch]
        sub    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2B30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 8]
        inc    dword ptr [esi + 20h]
        cmp    byte ptr [esi + 24h], 0
        je     SHORT loc_1_2B50
        mov    eax, 0c0010002h
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B50:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 10h]
        or     edx, edx
        jne    SHORT loc_1_2B6A
        push   eax
        push   esi
        push   ecx
        db  0E8h, 0EEh, 003h, 000h, 000h
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B6A:
        cmp    edx, 1
        jne    SHORT loc_1_2B95
        cmp    dword ptr [ecx + 0aeh], 0
        je     SHORT loc_1_2B85
        mov    eax, 0c001000dh
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B85:
        push   eax
        push   esi
        push   ecx
        call   sub_1_2FA0
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B95:
        mov    eax, 10001h
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        lea    esi, [ebp - 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    edi, 4
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010101h
        ja     SHORT loc_1_2C00
        je     NEAR PTR loc_1_2DA0
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_2C21
        shl    eax, 2
        db  0FFh, 0A0h, 004h, 02Fh, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C00:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2DC0
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2DE0
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2EB0
loc_1_2C21:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2EB9
        db  08Dh, 049h, 000h
loc_1_2C30:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2C45
        db  0BEh, 0E0h, 055h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_2EB9
loc_1_2C45:
        db  0BEh, 064h, 055h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_2EB9
loc_1_2C54:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0aeh], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_1_2C6E
        mov    dword ptr [ebp - 14h], 0
loc_1_2C6E:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2EB9
        db  005h, 000h, 000h, 000h, 000h
loc_1_2C80:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2EB9
        db  08Dh, 049h, 000h
loc_1_2C90:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 42h]
        sub    eax, 0eh
        jmp    NEAR PTR loc_1_2EB6
        db  08Bh, 0FFh
loc_1_2CA0:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2EB9
        db  08Dh, 064h, 024h, 000h
loc_1_2CB0:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2EB9
        db  08Dh, 064h, 024h, 000h
loc_1_2CC0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 68h]
        jmp    NEAR PTR loc_1_2EB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_2CD0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 6ch]
        jmp    NEAR PTR loc_1_2EB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_2CE0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 70h]
        jmp    NEAR PTR loc_1_2EB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_2CF0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 74h]
        jmp    NEAR PTR loc_1_2EB6
loc_1_2CFB:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 62h
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2EB9
loc_1_2D21:
        db  0BEh, 03Ch, 056h, 000h, 000h
        mov    edi, 15h
        jmp    NEAR PTR loc_1_2EB9
loc_1_2D30:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2D47
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0cah]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_2EB6
loc_1_2D47:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_2EB6
loc_1_2D55:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2D69
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 98h]
        jmp    NEAR PTR loc_1_2EB6
loc_1_2D69:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        jmp    NEAR PTR loc_1_2EB6
loc_1_2D74:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_1_2EB9
loc_1_2D87:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2EB9
loc_1_2D93:
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_2EB9
        db  090h
loc_1_2DA0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 62h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2EB9
        db  090h
loc_1_2DC0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 5ch
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2EB9
        db  090h
loc_1_2DE0:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_2E46
        mov    eax, dword ptr [ebx + 0cah]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_2E15
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_1_2EB9
loc_1_2E15:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebx + 0cah]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 02Fh, 0D9h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_2EB9
        jmp    SHORT loc_1_2EB9
loc_1_2E46:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        mov    eax, dword ptr [ebx + 0cah]
        push   eax
        db  0E8h, 007h, 0D8h, 0FEh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_2E79
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_1_2EB9
loc_1_2E79:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        mov    eax, dword ptr [ebx + 0cah]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 004h, 0D8h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2EB9
        db  005h, 000h, 000h, 000h, 000h
loc_1_2EB0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 32h]
loc_1_2EB6:
        mov    dword ptr [ebp - 8], eax
loc_1_2EB9:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2EF5
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_2EF5
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_2EE5
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edi
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_2EE5:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_2EF5:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h
loc_1_2F04:
        xor    byte ptr [eax + eax], ch
        db  000h, 054h, 02Ch, 000h
        db  000h, 080h, 02Ch, 000h, 000h, 080h
        sub    al, 0
        db  000h, 090h, 02Ch, 000h, 000h, 0A0h
        sub    al, 0
        db  000h, 0B0h, 02Ch, 000h, 000h, 0C0h
        sub    al, 0
        db  000h, 0D0h
        sub    al, 0
        db  000h, 0E0h
        sub    al, 0
        db  000h, 0F0h
        sub    al, 0
        db  000h, 0FBh
        sub    al, 0
        db  000h, 021h
        db  02Dh, 000h, 000h, 030h, 02Dh
        add    byte ptr [eax], al
        push   ebp
        db  02Dh, 000h, 000h, 074h, 02Dh
        add    byte ptr [eax], al
        db  087h, 02Dh, 000h, 000h, 021h, 02Ch
        add    byte ptr [eax], al
        xchg   ebx, eax
        sub    eax, 8b550000h
        in     al, dx
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
        call   sub_1_2BB0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_2FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_1_2FF7
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_3058
        cmp    eax, 10112h
        je     NEAR PTR loc_1_30E0
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_30E4
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_3125
loc_1_2FF7:
        cmp    edi, 4
        je     SHORT loc_1_3014
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_3125
loc_1_3014:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_3041
        mov    ebx, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_3125
loc_1_3041:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3140
        jmp    NEAR PTR loc_1_3123
loc_1_3058:
        cmp    edi, 4
        je     SHORT loc_1_3075
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_3125
loc_1_3075:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 42h]
        sub    eax, 0eh
        cmp    eax, dword ptr [ebp - 4]
        jb     SHORT loc_1_30D8
        mov    eax, dword ptr [ecx + 98h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_30B0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 98h], eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 1ch], eax
        jmp    SHORT loc_1_3125
        db  08Bh, 0FFh
loc_1_30B0:
        mov    edx, dword ptr [ebp + 0ch]
        cmp    dword ptr [edx + 1ch], eax
        jne    SHORT loc_1_30D0
        mov    eax, dword ptr [edx + 1ch]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_30D0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 1ch], eax
        push   ecx
        call   sub_1_3380
        jmp    SHORT loc_1_3125
        db  08Bh, 0FFh
loc_1_30D0:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 1ch], eax
        jmp    SHORT loc_1_3125
loc_1_30D8:
        mov    ebx, 0c0000001h
        jmp    SHORT loc_1_3125
        db  090h
loc_1_30E0:
        sub    ebx, ebx
        jmp    SHORT loc_1_3125
loc_1_30E4:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_1_3108
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_3125
loc_1_3108:
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_31A0
loc_1_3123:
        mov    ebx, eax
loc_1_3125:
        or     ebx, ebx
        jne    SHORT loc_1_3133
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_1_3133:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_3140:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0aeh], 0
        jne    SHORT loc_1_318A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_3180
        inc    dword ptr [esi + 20h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [ecx + 0cah]
        push   eax
        db  0E8h, 06Ah, 0D4h, 0FEh, 0FFh
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_3180:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_318A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0aeh], 0
        jne    SHORT loc_1_31F0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_31E2
        inc    dword ptr [esi + 20h]
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [ecx + 0cah]
        push   eax
        db  0E8h, 026h, 0D1h, 0FEh, 0FFh
        dec    dword ptr [esi + 20h]
        pop    esi
        pop    ebp
        ret    14h
loc_1_31E2:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_1_31F0:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3200:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 0f0h]
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2BB0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_1_32EF
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_3289
        je     SHORT loc_1_32C0
        cmp    eax, 20101h
        je     SHORT loc_1_32A7
        cmp    eax, 20102h
        je     SHORT loc_1_32B0
        cmp    eax, 20103h
        je     SHORT loc_1_32D0
        cmp    eax, 20104h
        je     SHORT loc_1_32B8
        jmp    SHORT loc_1_329E
loc_1_3289:
        cmp    eax, 1020101h
        je     SHORT loc_1_32D0
        cmp    eax, 1020102h
        je     SHORT loc_1_32D0
        cmp    eax, 1020103h
        je     SHORT loc_1_32D0
loc_1_329E:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    SHORT loc_1_32D7
loc_1_32A7:
        mov    eax, dword ptr [ebx + 54h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_32D7
        db  090h
loc_1_32B0:
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_32D7
loc_1_32B8:
        mov    eax, dword ptr [ebx + 3ch]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_32D7
loc_1_32C0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 94h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_32D7
        db  08Bh, 0FFh
loc_1_32D0:
        mov    dword ptr [ebp - 4], 0
loc_1_32D7:
        cmp    edi, 4
        jb     SHORT loc_1_32EF
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 0ch], 4
loc_1_32EF:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3310:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 14h]
        cmp    edx, 20105h
        ja     SHORT loc_1_3345
        cmp    edx, 20101h
        jae    SHORT loc_1_336A
        cmp    edx, 10101h
        jb     SHORT loc_1_3365
        cmp    edx, 10111h
        jbe    SHORT loc_1_336A
        cmp    edx, 10113h
        je     SHORT loc_1_336A
        jmp    SHORT loc_1_3365
loc_1_3345:
        cmp    edx, 1010101h
        jb     SHORT loc_1_3365
        cmp    edx, 1010104h
        jbe    SHORT loc_1_336A
        cmp    edx, 1020101h
        jb     SHORT loc_1_3365
        cmp    edx, 1020103h
        jbe    SHORT loc_1_336A
loc_1_3365:
        mov    eax, 0c0010017h
loc_1_336A:
        or     eax, eax
        jne    SHORT loc_1_3378
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3200
loc_1_3378:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3380:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0a6h]
        cmp    esi, edx
        je     SHORT loc_1_33A3
loc_1_3393:
        mov    eax, dword ptr [esi + 1ch]
        cmp    edx, eax
        jae    SHORT loc_1_339C
        mov    edx, eax
loc_1_339C:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_3393
loc_1_33A3:
        mov    dword ptr [ecx + 98h], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_33B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0bch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 7ch], 8
        mov    word ptr [ebp - 7ah], 8
        db  0C7h, 045h, 088h, 0E4h, 056h, 000h, 000h
        mov    word ptr [ebp - 6ch], 0ah
        mov    word ptr [ebp - 6ah], 0ah
        db  0C7h, 045h, 098h, 0F8h, 056h, 000h, 000h
        mov    word ptr [ebp - 0a4h], 0ch
        mov    word ptr [ebp - 0a2h], 0ch
        db  0C7h, 085h, 060h, 0FFh, 0FFh, 0FFh, 010h, 057h, 000h, 000h
        mov    word ptr [ebp - 64h], 0dh
        mov    word ptr [ebp - 62h], 0dh
        db  0C7h, 045h, 0A0h, 02Ch, 057h, 000h, 000h
        mov    word ptr [ebp - 74h], 0dh
        mov    word ptr [ebp - 72h], 0dh
        db  0C7h, 045h, 090h, 04Ch, 057h, 000h, 000h
        mov    word ptr [ebp - 5ch], 0fh
        mov    word ptr [ebp - 5ah], 0fh
        db  0C7h, 045h, 0A8h, 084h, 057h, 000h, 000h
        mov    word ptr [ebp - 54h], 0dh
        lea    eax, [ebp - 3bh]
        mov    word ptr [ebp - 52h], 0dh
        db  0C7h, 045h, 0B0h, 0A4h, 057h, 000h, 000h
        mov    word ptr [ebp - 4ch], 0ch
        mov    word ptr [ebp - 4ah], 0ch
        mov    esi, 368h
        db  0C7h, 045h, 0B8h, 0C0h, 057h, 000h, 000h
        mov    byte ptr [ebp - 3ch], 0
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 4], 0
        mov    edi, 0d8000h
        mov    dword ptr [ebp - 30h], 0
        mov    dword ptr [ebp - 24h], 8000h
        mov    dword ptr [ebp - 20h], 10h
        mov    dword ptr [ebp - 1ch], 200h
        mov    dword ptr [ebp - 14h], 4
        mov    dword ptr [ebp - 18h], 0ah
        mov    bl, 5
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [ebp - 9ch], 6
        mov    word ptr [ebp - 9ah], 6
        db  0C7h, 085h, 068h, 0FFh, 0FFh, 0FFh, 0D4h, 057h, 000h, 000h
        mov    word ptr [ebp - 94h], 6
        mov    word ptr [ebp - 92h], 6
        db  0C7h, 085h, 070h, 0FFh, 0FFh, 0FFh, 0E4h, 057h, 000h, 000h
        mov    word ptr [ebp - 8ch], 0bh
        mov    word ptr [ebp - 8ah], 0bh
        db  0C7h, 085h, 078h, 0FFh, 0FFh, 0FFh, 008h, 058h, 000h, 000h
        mov    word ptr [ebp - 84h], 8
        mov    word ptr [ebp - 82h], 8
        db  0C7h, 045h, 080h, 01Ch, 058h, 000h, 000h
        mov    word ptr [ebp - 0ach], 8
        mov    word ptr [ebp - 0aah], 8
        db  0C7h, 085h, 058h, 0FFh, 0FFh, 0FFh, 02Ch, 058h, 000h, 000h
        mov    dword ptr [ebp - 28h], 1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0584
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3560
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_3560:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 34h], 6
        jne    SHORT loc_1_3597
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3597
        mov    ecx, dword ptr [ebp - 2ch]
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 3ch], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 38h], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 37h], al
loc_1_3597:
        push   1
        lea    eax, [ebp - 0ach]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_3680
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 4], 3
        jne    SHORT loc_1_35D0
        mov    dword ptr [ebp - 14h], 3
        jmp    NEAR PTR loc_1_3680
loc_1_35D0:
        push   2
        lea    eax, [ebp - 0a4h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_3680
        push   1
        lea    eax, [ebp - 9ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     SHORT loc_1_3616
        mov    dword ptr [ebp - 14h], 0
        jmp    SHORT loc_1_3680
loc_1_3616:
        push   1
        lea    eax, [ebp - 94h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     SHORT loc_1_3638
        mov    dword ptr [ebp - 14h], 2
        jmp    SHORT loc_1_3680
loc_1_3638:
        push   1
        lea    eax, [ebp - 8ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     SHORT loc_1_3660
        mov    dword ptr [ebp - 14h], 4
        jmp    SHORT loc_1_3680
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3660:
        push   1
        lea    eax, [ebp - 84h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     SHORT loc_1_3680
        mov    dword ptr [ebp - 14h], 5
loc_1_3680:
        cmp    dword ptr [ebp - 14h], 3
        jne    SHORT loc_1_3701
        lea    eax, [ebp - 44h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0480
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_377E
        test   byte ptr [ebp - 42h], 1
        je     NEAR PTR loc_1_377E
        test   byte ptr [ebp - 42h], 80h
        mov    bl, 0ch
        jne    SHORT loc_1_36B9
        mov    bl, 3
loc_1_36B9:
        mov    dword ptr [ebp - 28h], 2
        mov    al, byte ptr [ebp - 42h]
        and    al, 60h
        shr    al, 5
        cmp    al, 3
        je     NEAR PTR loc_1_377E
        and    eax, 0ffh
        db  08Bh, 004h, 085h, 054h, 056h, 000h, 000h
        mov    dword ptr [ebp - 24h], eax
        mov    al, byte ptr [ebp - 41h]
        and    eax, 0fh
        db  08Bh, 03Ch, 085h, 064h, 056h, 000h, 000h
        mov    al, byte ptr [ebp - 3fh]
        and    eax, 0fh
        xor    esi, esi
        db  066h, 08Bh, 034h, 045h, 0A4h, 056h, 000h, 000h
        jmp    NEAR PTR loc_1_37ED
loc_1_3701:
        push   1
        lea    eax, [ebp - 7ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3724
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax + 4]
loc_1_3724:
        sub    al, al
loc_1_3726:
        xor    ecx, ecx
        mov    cl, al
        db  039h, 034h, 08Dh, 0C4h, 056h, 000h, 000h
        je     SHORT loc_1_3739
        inc    al
        cmp    al, 4
        jb     SHORT loc_1_3726
loc_1_3739:
        cmp    al, 4
        je     SHORT loc_1_377E
        push   1
        lea    eax, [ebp - 74h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3760
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [eax + 4]
loc_1_3760:
        cmp    dword ptr [ebp - 14h], 4
        jne    SHORT loc_1_3795
        cmp    edi, 80000h
        jb     SHORT loc_1_377E
        cmp    edi, 0e8000h
        ja     SHORT loc_1_377E
        test   edi, 7fffh
        je     SHORT loc_1_37B2
loc_1_377E:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0590
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3795:
        cmp    edi, 88000h
        jb     SHORT loc_1_377E
        cmp    edi, 0e8000h
        ja     SHORT loc_1_377E
        lea    eax, [edi + 8000h]
        test   eax, 0ffffh
        jne    SHORT loc_1_377E
loc_1_37B2:
        push   1
        lea    eax, [ebp - 6ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_37D5
        mov    eax, dword ptr [ebp - 0ch]
        mov    bl, byte ptr [eax + 4]
loc_1_37D5:
        sub    al, al
loc_1_37D7:
        xor    ecx, ecx
        mov    cl, al
        db  038h, 099h, 0D4h, 056h, 000h, 000h
        je     SHORT loc_1_37E9
        inc    al
        cmp    al, 7
        jb     SHORT loc_1_37D7
loc_1_37E9:
        cmp    al, 7
        je     SHORT loc_1_377E
loc_1_37ED:
        push   0
        lea    eax, [ebp - 64h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3813
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 20h], eax
loc_1_3813:
        push   0
        lea    eax, [ebp - 5ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_384E
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 1ch], eax
        cmp    eax, 100h
        jb     SHORT loc_1_3847
        cmp    eax, 5eah
        jbe    SHORT loc_1_384E
loc_1_3847:
        mov    dword ptr [ebp - 1ch], 100h
loc_1_384E:
        push   1
        lea    eax, [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3885
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], eax
        cmp    eax, 4
        jb     SHORT loc_1_387E
        cmp    eax, 14h
        jbe    SHORT loc_1_3885
loc_1_387E:
        mov    dword ptr [ebp - 18h], 0ah
loc_1_3885:
        push   1
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_38AB
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 1], al
loc_1_38AB:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0590
        cmp    dword ptr [ebp - 20h], 3
        jne    SHORT loc_1_38C1
        mov    dword ptr [ebp - 20h], 4
loc_1_38C1:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 2ah], esi
        mov    byte ptr [ecx + 4bh], bl
        mov    dword ptr [ecx + 2eh], edi
        mov    dword ptr [ecx + 46h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ecx + 21h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 50h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ecx + 32h], eax
        mov    dword ptr [ecx + 36h], 0ah
        mov    dword ptr [ecx + 3ah], 6
        mov    dword ptr [ecx + 3eh], 40h
        add    ecx, 5ch
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx - 1ah], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx - 10h], eax
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx - 12h], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [ebp - 38h]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [ebp - 37h]
        mov    byte ptr [ecx + 5], al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_3930:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    word ptr [ebp - 14h], 6
        mov    word ptr [ebp - 12h], 6
        db  0C7h, 045h, 0F0h, 0D4h, 059h, 000h, 000h
        mov    word ptr [ebp - 0ch], 0
        mov    word ptr [ebp - 0ah], 1
        mov    word ptr [ebp - 8], 7012h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 0E4h, 059h, 000h, 000h
        call   sub_1_041A
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        db  0A3h, 01Eh, 05Ah, 000h, 000h
        db  089h, 00Dh, 022h, 05Ah, 000h, 000h
        db  0C7h, 005h, 026h, 05Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 0E8h, 059h, 000h, 000h, 003h
        db  0C6h, 005h, 0E9h, 059h, 000h, 000h, 000h
        db  0C7h, 005h, 0EAh, 059h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0EEh, 059h, 000h, 000h, 0A0h, 006h, 000h, 000h
        db  0C7h, 005h, 0F2h, 059h, 000h, 000h, 070h, 00Ch, 000h, 000h
        db  0C7h, 005h, 0F6h, 059h, 000h, 000h, 030h, 024h, 000h, 000h
        db  0C7h, 005h, 0FAh, 059h, 000h, 000h, 0A0h, 029h, 000h, 000h
        db  0C7h, 005h, 0FEh, 059h, 000h, 000h, 0C0h, 00Dh, 000h, 000h
        db  0C7h, 005h, 002h, 05Ah, 000h, 000h, 030h, 02Bh, 000h, 000h
        db  0C7h, 005h, 006h, 05Ah, 000h, 000h, 010h, 033h, 000h, 000h
        db  0C7h, 005h, 00Ah, 05Ah, 000h, 000h, 050h, 03Fh, 000h, 000h
        db  0C7h, 005h, 00Eh, 05Ah, 000h, 000h, 070h, 03Ah, 000h, 000h
        db  0C7h, 005h, 012h, 05Ah, 000h, 000h, 080h, 03Fh, 000h, 000h
        db  0A3h, 016h, 05Ah, 000h, 000h
        db  089h, 00Dh, 01Ah, 05Ah, 000h, 000h
        push   36h
        db  068h, 0E8h, 059h, 000h, 000h
        db  068h, 0E0h, 059h, 000h, 000h
        db  0A1h, 0E4h, 059h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        db  068h, 0E0h, 059h, 000h, 000h
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_3A60
        db  083h, 03Dh, 026h, 05Ah, 000h, 000h, 000h
        jbe    SHORT loc_1_3A48
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3A48:
        push   0
        db  0A1h, 0E4h, 059h, 000h, 000h
        push   eax
        call   sub_1_0420
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3A60:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        db  0A1h, 060h, 040h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   126h
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     SHORT loc_1_3AA0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_1_3AA0:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 49h
        rep stosd
        stosw
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_33B0
        or     eax, eax
        je     SHORT loc_1_3AE0
        push   0
        push   126h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3AE0:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3B20
        or     eax, eax
        jne    SHORT loc_1_3AF9
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3AF9:
        push   0
        push   126h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0c0010008h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    bl, bl
        mov    eax, dword ptr [esi + 46h]
        cmp    eax, 2
        je     SHORT loc_1_3B50
        cmp    eax, 3
        je     SHORT loc_1_3B50
        cmp    eax, 4
        je     SHORT loc_1_3B50
        mov    bl, 1
        mov    dword ptr [ebp - 8], 0aa00000ah
        jmp    NEAR PTR loc_1_3C01
        db  08Bh, 0FFh
loc_1_3B50:
        lea    edi, [esi + 108h]
        db  0A1h, 060h, 040h, 000h, 000h
        push   eax
        mov    eax, dword ptr [esi + 4ch]
        push   0
        lea    eax, [eax + eax*8]
        shl    eax, 4
        add    eax, 0f0h
        push   eax
        push   edi
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_3B90
        mov    dword ptr [edi], 0
        mov    bl, 1
        mov    dword ptr [ebp - 8], 0aa000002h
        jmp    SHORT loc_1_3C01
        db  005h, 000h, 000h, 000h, 000h
loc_1_3B90:
        mov    eax, dword ptr [esi + 4ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, 0ah
        mov    dword ptr [esi + 36h], eax
        mov    eax, dword ptr [esi + 50h]
        dec    eax
        mov    dword ptr [esi + 54h], eax
        not    eax
        mov    dword ptr [esi + 58h], eax
        mov    ecx, dword ptr [esi + 46h]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        db  08Ah, 004h, 08Dh, 073h, 058h, 000h, 000h
        add    eax, dword ptr [esi + 2ah]
        mov    dword ptr [esi + 0beh], eax
        mov    ecx, dword ptr [esi + 46h]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        db  08Ah, 004h, 08Dh, 074h, 058h, 000h, 000h
        add    eax, dword ptr [esi + 2ah]
        mov    dword ptr [esi + 0c2h], eax
        mov    eax, dword ptr [esi + 46h]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        db  08Ah, 004h, 085h, 075h, 058h, 000h, 000h
        and    eax, 0ffh
        add    eax, dword ptr [esi + 2ah]
        mov    dword ptr [esi + 0c6h], eax
loc_1_3C01:
        lea    edi, [ebp - 30h]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        cmp    dword ptr [esi + 46h], 3
        mov    dword ptr [ebp - 26h], 3
        je     SHORT loc_1_3C23
        mov    dword ptr [ebp - 26h], 1
loc_1_3C23:
        mov    dword ptr [ebp - 1ah], 1
        mov    eax, dword ptr [esi + 2ah]
        mov    dword ptr [ebp - 16h], eax
        mov    dword ptr [ebp - 12h], 4
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        db  0A1h, 0E0h, 059h, 000h, 000h
        push   eax
        lea    eax, [esi + 4]
        push   eax
        call   sub_1_0432
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    NEAR PTR loc_1_3EC5
        or     bl, bl
        je     SHORT loc_1_3C79
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_3EBC
loc_1_3C79:
        mov    eax, dword ptr [esi + 50h]
        push   eax
        mov    eax, dword ptr [esi + 2eh]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [esi + 0f0h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_3CB7
        mov    eax, dword ptr [esi + 2eh]
        push   eax
        push   1
        push   0aa000001h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 10h
        jmp    NEAR PTR loc_1_3EBC
loc_1_3CB7:
        push   esi
        call   sub_1_11E0
        or     al, al
        jne    SHORT loc_1_3CE0
        mov    dword ptr [ebp - 4], 0c0000001h
        push   0
        push   0aa000004h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_3EBC
loc_1_3CE0:
        push   esi
        call   sub_1_1340
        or     al, al
        jne    SHORT loc_1_3D24
        push   esi
        call   sub_1_1340
        or     al, al
        jne    SHORT loc_1_3D24
        mov    dword ptr [ebp - 4], 0c0000001h
        mov    eax, dword ptr [esi + 2eh]
        push   eax
        mov    eax, dword ptr [esi + 2ah]
        push   eax
        movsx  eax, byte ptr [esi + 4bh]
        push   eax
        mov    eax, dword ptr [esi + 46h]
        push   eax
        push   4
        push   0aa000004h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 1ch
        jmp    NEAR PTR loc_1_3EBC
loc_1_3D24:
        mov    al, byte ptr [esi + 5dh]
        or     al, byte ptr [esi + 5eh]
        or     al, byte ptr [esi + 5fh]
        or     al, byte ptr [esi + 60h]
        lea    edx, [esi + 5ch]
        or     al, byte ptr [esi + 61h]
        or     al, byte ptr [edx]
        jne    SHORT loc_1_3D4E
        mov    ecx, dword ptr [esi + 62h]
        lea    eax, [esi + 62h]
        mov    dword ptr [edx], ecx
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [edx + 4], cl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 5], al
loc_1_3D4E:
        lea    eax, [esi + 0cah]
        push   eax
        push   esi
        push   edx
        db  068h, 010h, 00Ah, 000h, 000h
        db  068h, 050h, 009h, 000h, 000h
        db  068h, 0A0h, 008h, 000h, 000h
        mov    eax, dword ptr [esi + 32h]
        push   eax
        db  0E8h, 091h, 0C2h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_3D92
        mov    dword ptr [ebp - 4], 0c0000001h
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_3EBC
loc_1_3D92:
        mov    dx, word ptr [esi + 0beh]
        mov    al, byte ptr [esi + 0b2h]
        out    dx, al
        or     al, 2
        out    dx, al
        mov    byte ptr [esi + 29h], 1
        lea    ebx, [esi + 9]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        movsx  eax, byte ptr [esi + 4bh]
        mov    ecx, dword ptr [esi + 21h]
        push   ecx
        push   1
        push   eax
        push   eax
        db  068h, 0A0h, 023h, 000h, 000h
        push   esi
        db  068h, 020h, 023h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   ebx
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_3E00
        movsx  eax, byte ptr [esi + 4bh]
        push   eax
        push   1
        push   0aa000005h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 10h
        jmp    NEAR PTR loc_1_3EB0
        db  08Dh, 049h, 000h
loc_1_3E00:
        db  0C7h, 086h, 09Eh, 000h, 000h, 000h, 0E0h, 059h, 000h, 000h
        mov    dword ptr [esi + 0aah], 0
        mov    dword ptr [esi + 0a6h], 0
        db  0A1h, 02Ah, 05Ah, 000h, 000h
        mov    dword ptr [esi + 0a2h], eax
        db  089h, 035h, 02Ah, 05Ah, 000h, 000h
        mov    dword ptr [esi + 0a6h], 0
        push   esi
        call   sub_1_1B70
        or     al, al
        jne    NEAR PTR loc_1_3EF0
        mov    dword ptr [ebp - 4], 0c0000001h
        push   0
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
        push   ebx
        call   sub_1_04F4
        db  039h, 035h, 02Ah, 05Ah, 000h, 000h
        jne    SHORT loc_1_3E80
        mov    eax, dword ptr [esi + 0a2h]
        db  0A3h, 02Ah, 05Ah, 000h, 000h
        jmp    SHORT loc_1_3EB0
        db  08Dh, 064h, 024h, 000h
loc_1_3E80:
        db  08Bh, 00Dh, 02Ah, 05Ah, 000h, 000h
        cmp    dword ptr [ecx + 0a2h], esi
        je     SHORT loc_1_3E9E
        mov    edi, edi
loc_1_3E90:
        mov    ecx, dword ptr [ecx + 0a2h]
        cmp    dword ptr [ecx + 0a2h], esi
        jne    SHORT loc_1_3E90
loc_1_3E9E:
        mov    eax, dword ptr [ecx + 0a2h]
        mov    eax, dword ptr [eax + 0a2h]
        mov    dword ptr [ecx + 0a2h], eax
loc_1_3EB0:
        mov    eax, dword ptr [esi + 0cah]
        push   eax
        db  0E8h, 064h, 0C2h, 0FEh, 0FFh
loc_1_3EBC:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0438
loc_1_3EC5:
        mov    ecx, dword ptr [esi + 108h]
        or     ecx, ecx
        je     SHORT loc_1_3EE1
        mov    eax, dword ptr [esi + 36h]
        push   0
        lea    eax, [eax + eax*2]
        shl    eax, 3
        push   eax
        push   ecx
        call   sub_1_050C
loc_1_3EE1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_3EF0:
        mov    dword ptr [esi + 0e0h], 0
        db  0C7h, 086h, 0E4h, 000h, 000h, 000h, 040h, 00Ah, 000h, 000h
        lea    eax, [esi + 0ceh]
        mov    dword ptr [esi + 0e8h], esi
        mov    dword ptr [eax], 0
        db  0C7h, 086h, 0D2h, 000h, 000h, 000h, 0D0h, 00Ah, 000h, 000h
        mov    dword ptr [esi + 0d6h], esi
        push   2710h
        push   eax
        call   sub_1_04E2
        mov    eax, dword ptr [esi + 9eh]
        pop    edi
        inc    dword ptr [eax + 46h]
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 0E0h, 059h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        db  0A1h, 0E4h, 059h, 000h, 000h
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3F80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 9]
        push   eax
        call   sub_1_04F4
        mov    byte ptr [esi + 8], 1
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0ceh]
        push   eax
        call   sub_1_04E8
        db  039h, 035h, 02Ah, 05Ah, 000h, 000h
        jne    SHORT loc_1_3FC0
        mov    eax, dword ptr [esi + 0a2h]
        db  0A3h, 02Ah, 05Ah, 000h, 000h
        jmp    SHORT loc_1_3FF0
        db  08Dh, 064h, 024h, 000h
loc_1_3FC0:
        db  08Bh, 00Dh, 02Ah, 05Ah, 000h, 000h
        cmp    dword ptr [ecx + 0a2h], esi
        je     SHORT loc_1_3FDE
        mov    edi, edi
loc_1_3FD0:
        mov    ecx, dword ptr [ecx + 0a2h]
        cmp    dword ptr [ecx + 0a2h], esi
        jne    SHORT loc_1_3FD0
loc_1_3FDE:
        mov    eax, dword ptr [ecx + 0a2h]
        mov    eax, dword ptr [eax + 0a2h]
        mov    dword ptr [ecx + 0a2h], eax
loc_1_3FF0:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4000
        push   1f4h
        call   sub_1_0500
loc_1_4000:
        mov    eax, dword ptr [esi + 0cah]
        push   eax
        db  0E8h, 014h, 0C1h, 0FEh, 0FFh
        cmp    dword ptr [esi + 46h], 3
        je     SHORT loc_1_4026
        mov    dx, word ptr [esi + 0beh]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [esi + 0c2h]
        sub    al, al
        out    dx, al
loc_1_4026:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0438
        mov    eax, dword ptr [esi + 36h]
        push   0
        lea    eax, [eax + eax*2]
        shl    eax, 3
        push   eax
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_1_050C
        push   0
        push   126h
        push   esi
        call   sub_1_050C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_4064:
        cli
        db  0E9h, 08Ch, 000h, 032h, 0FFh
        db  008h, 000h, 050h, 0FFh, 000h, 000h, 052h, 0FFh, 000h, 000h, 054h, 0FFh, 000h, 000h, 056h, 0FFh
        db  009h, 0C0h, 058h, 0FFh, 000h, 000h, 05Ah, 0FFh, 000h, 000h, 05Ch, 0FFh, 000h, 000h, 05Eh, 0FFh
        db  000h, 040h, 060h, 0FFh, 000h, 000h, 062h, 0FFh, 0D0h, 007h, 066h, 0FFh, 001h, 0C0h, 000h, 000h
        db  002h, 000h, 072h, 08Ah, 00Ch, 00Fh, 040h, 06Eh, 000h, 060h, 000h, 0F2h, 000h, 000h, 00Ch, 000h
        db  000h, 000h, 001h, 000h, 07Eh, 08Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h
        db  090h, 08Ah, 00Ch, 00Fh, 040h, 02Eh, 000h, 060h, 000h, 0F2h, 008h, 000h, 00Ch, 000h, 000h, 000h
        db  003h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 036h, 08Ah, 000h, 000h, 007h, 000h
        db  03Ch, 08Ah, 000h, 000h, 005h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h
        db  001h, 000h, 09Ch, 000h, 030h, 000h, 094h, 000h, 028h, 000h, 0FCh, 08Ch, 0D8h, 08Eh, 0C0h, 08Eh
        db  0D0h, 0BCh, 020h, 08Ah, 08Bh, 016h, 0AEh, 088h, 0A0h, 034h, 089h, 0EEh, 08Bh, 016h, 0B0h, 088h
        db  085h, 0D2h, 074h, 004h, 0A0h, 036h, 089h, 0EEh, 033h, 0C0h, 0A3h, 0A4h, 088h, 0B9h, 00Ch, 000h
        db  0BEh, 004h, 000h, 01Eh, 00Eh, 01Fh, 0ADh, 092h, 0ADh, 0F0h, 0EFh, 0E2h, 0F9h, 01Fh, 0E8h, 0E7h
        db  00Eh, 08Bh, 016h, 0ACh, 088h, 0B0h, 040h, 0EEh, 0E8h, 03Bh, 007h, 08Bh, 016h, 0AEh, 088h, 0A0h
        db  033h, 089h, 0EEh, 08Bh, 016h, 0B0h, 088h, 085h, 0D2h, 074h, 004h, 0A0h, 035h, 089h, 0EEh, 081h
        db  00Eh, 0A0h, 088h, 000h, 0A0h, 0EBh, 01Eh, 0C7h, 006h, 0C2h, 088h, 008h, 000h, 0BEh, 008h, 001h
        db  0BFh, 052h, 089h, 0B9h, 004h, 000h, 0F3h, 0A7h, 074h, 011h, 0E8h, 089h, 000h, 0EBh, 00Ch, 0FFh
        db  006h, 0F2h, 000h, 0EBh, 018h, 0FFh, 00Eh, 0C2h, 088h, 07Eh, 0DCh, 0BAh, 050h, 0FFh, 0EDh, 08Bh
        db  0D8h, 087h, 01Eh, 0A4h, 088h, 02Bh, 0C3h, 001h, 006h, 0F0h, 000h, 072h, 0E2h, 0E8h, 0CEh, 000h
        db  0BEh, 070h, 082h, 08Bh, 05Ch, 002h, 03Ah, 01Ch, 075h, 043h, 08Bh, 036h, 03Eh, 089h, 081h, 03Ch
        db  000h, 0A0h, 074h, 03Eh, 0F7h, 006h, 000h, 001h, 040h, 000h, 074h, 03Bh, 083h, 03Eh, 0CEh, 088h
        db  000h, 07Fh, 039h, 08Bh, 036h, 04Eh, 080h, 08Bh, 044h, 002h, 03Ah, 004h, 075h, 033h, 033h, 0C0h
        db  039h, 006h, 0E0h, 000h, 075h, 0AFh, 039h, 006h, 0E2h, 000h, 074h, 0A9h, 0A2h, 0E3h, 000h, 08Bh
        db  016h, 0ACh, 088h, 0B0h, 040h, 0C6h, 006h, 0E1h, 000h, 0FFh, 0EEh, 0EBh, 098h, 0E8h, 0F8h, 004h
        db  0EBh, 0B8h, 0E8h, 028h, 003h, 0EBh, 0BDh, 0E8h, 0D0h, 005h, 0EBh, 0C0h, 0E8h, 044h, 006h, 0EBh
        db  0C2h, 0E8h, 078h, 00Ah, 0EBh, 0C8h, 08Bh, 044h, 0FEh, 08Bh, 0D0h, 03Dh, 0FFh, 0FFh, 075h, 005h
        db  033h, 0D2h, 089h, 054h, 0FEh, 02Bh, 045h, 0FEh, 089h, 055h, 0FEh, 08Bh, 0D9h, 0D1h, 0E3h, 02Eh
        db  08Bh, 097h, 080h, 000h, 009h, 016h, 098h, 000h, 02Eh, 08Bh, 09Fh, 088h, 000h, 033h, 0D2h, 001h
        db  007h, 011h, 057h, 002h, 001h, 006h, 03Ch, 000h, 011h, 016h, 03Eh, 000h, 001h, 006h, 024h, 000h
        db  011h, 016h, 026h, 000h, 083h, 0F9h, 001h, 074h, 008h, 001h, 006h, 050h, 000h, 011h, 016h, 052h
        db  000h, 0E3h, 004h, 0F3h, 0A7h, 075h, 0AFh, 0C3h, 08Bh, 0EAh, 0D1h, 0E5h, 08Bh, 06Ah, 010h, 089h
        db  02Dh, 089h, 05Dh, 002h, 088h, 044h, 002h, 001h, 00Eh, 0EAh, 000h, 0EBh, 070h, 090h, 08Bh, 036h
        db  01Eh, 080h, 08Bh, 054h, 002h, 08Bh, 004h, 03Ah, 0D0h, 074h, 027h, 08Bh, 0C8h, 02Ah, 0CAh, 08Bh
        db  0D8h, 0FEh, 0CBh, 0D1h, 0E3h, 08Bh, 058h, 010h, 0C6h, 047h, 003h, 080h, 0BFh, 046h, 089h, 083h
        db  03Dh, 000h, 074h, 0C4h, 087h, 05Dh, 002h, 0C6h, 047h, 003h, 000h, 088h, 044h, 002h, 001h, 00Eh
        db  0EAh, 000h, 0E9h, 0BBh, 000h, 03Bh, 01Eh, 0C4h, 088h, 075h, 025h, 0A1h, 0F0h, 000h, 02Bh, 006h
        db  0C6h, 088h, 03Bh, 006h, 0C8h, 088h, 07Eh, 015h, 0FFh, 006h, 0F6h, 000h, 0E8h, 0D9h, 006h, 080h
        db  067h, 001h, 0DFh, 080h, 04Fh, 001h, 088h, 0C7h, 006h, 0C4h, 088h, 000h, 000h, 0E9h, 023h, 001h
        db  089h, 01Eh, 0C4h, 088h, 0A1h, 0F0h, 000h, 0A3h, 0C6h, 088h, 0E9h, 016h, 001h, 08Bh, 01Eh, 046h
        db  089h, 085h, 0DBh, 074h, 03Ch, 0F6h, 047h, 001h, 040h, 075h, 0BAh, 0BFh, 000h, 001h, 0F7h, 005h
        db  000h, 002h, 075h, 02Dh, 083h, 07Dh, 002h, 000h, 075h, 027h, 0F7h, 006h, 0A0h, 088h, 000h, 004h
        db  075h, 01Fh, 0F6h, 047h, 001h, 0C0h, 075h, 019h, 089h, 05Dh, 004h, 0C7h, 045h, 002h, 000h, 001h
        db  08Bh, 016h, 0A8h, 088h, 0EEh, 0FFh, 006h, 0ECh, 000h, 0EBh, 006h, 090h, 0EBh, 0BFh, 0E9h, 0B9h
        db  000h, 0E9h, 0CFh, 000h, 0E9h, 0E0h, 000h, 0E9h, 037h, 001h, 0E9h, 0C7h, 000h, 087h, 046h, 000h
        db  088h, 047h, 00Bh, 0E8h, 02Eh, 011h, 089h, 056h, 000h, 0EBh, 078h, 090h, 011h, 054h, 04Ah, 0EBh
        db  05Dh, 090h, 011h, 054h, 046h, 0EBh, 05Dh, 090h, 011h, 054h, 02Eh, 0EBh, 066h, 090h, 011h, 054h
        db  014h, 072h, 005h, 084h, 0C0h, 0EBh, 054h, 090h, 011h, 054h, 016h, 084h, 0C0h, 0EBh, 04Ch, 090h
        db  08Bh, 02Eh, 046h, 089h, 085h, 0EDh, 074h, 0B9h, 0F6h, 046h, 001h, 080h, 074h, 0AEh, 08Eh, 006h
        db  0ECh, 088h, 08Bh, 05Eh, 006h, 033h, 0C0h, 0BEh, 010h, 000h, 08Bh, 07Fh, 004h, 026h, 08Bh, 055h
        db  004h, 03Bh, 016h, 07Ch, 08Ah, 074h, 09Dh, 026h, 0F6h, 005h, 001h, 075h, 09Ah, 099h, 087h, 046h
        db  000h, 0A9h, 03Fh, 05Bh, 075h, 097h, 08Bh, 04Fh, 008h, 001h, 04Ch, 048h, 072h, 09Eh, 0F9h, 011h
        db  054h, 044h, 072h, 09Eh, 0BEh, 090h, 000h, 0D0h, 0E0h, 072h, 0A3h, 074h, 006h, 0F9h, 011h, 054h
        db  02Ch, 072h, 095h, 042h, 001h, 016h, 0EEh, 000h, 0BEh, 080h, 084h, 000h, 014h, 029h, 016h, 0EAh
        db  000h, 074h, 01Eh, 08Bh, 06Eh, 004h, 0F6h, 046h, 001h, 080h, 075h, 0A6h, 089h, 02Eh, 046h, 089h
        db  089h, 016h, 0C4h, 088h, 039h, 016h, 0E4h, 000h, 073h, 012h, 08Ch, 0D8h, 08Eh, 0C0h, 0E9h, 0FCh
        db  0FEh, 088h, 076h, 003h, 0C7h, 006h, 046h, 089h, 000h, 000h, 0EBh, 0E4h, 0C6h, 006h, 0E2h, 000h
        db  0FFh, 0EBh, 0E7h, 0C3h, 0E8h, 06Dh, 010h, 0F6h, 046h, 001h, 020h, 074h, 005h, 0E8h, 0AAh, 006h
        db  033h, 0D2h, 089h, 056h, 000h, 0EBh, 0ACh, 0BEh, 078h, 08Ah, 0B9h, 002h, 000h, 0F3h, 0A7h, 08Bh
        db  07Fh, 004h, 075h, 002h, 00Ch, 001h, 026h, 023h, 015h, 026h, 023h, 055h, 002h, 042h, 075h, 005h
        db  00Ch, 002h, 0EBh, 029h, 090h, 026h, 0F6h, 005h, 001h, 074h, 022h, 00Ch, 020h, 08Bh, 00Eh, 096h
        db  08Ah, 0E3h, 01Ah, 08Bh, 0D1h, 0BEh, 098h, 08Ah, 057h, 0B9h, 006h, 000h, 0F3h, 0A6h, 05Fh, 074h
        db  00Ah, 003h, 0F1h, 083h, 0EAh, 006h, 075h, 0F0h, 0EBh, 003h, 090h, 00Ch, 011h, 088h, 047h, 00Bh
        db  0BEh, 010h, 000h, 084h, 044h, 00Ah, 075h, 09Ch, 0E8h, 009h, 010h, 089h, 056h, 000h, 0E9h, 052h
        db  0FFh, 026h, 023h, 015h, 026h, 023h, 055h, 002h, 042h, 075h, 0BAh, 0F6h, 044h, 00Ah, 002h, 075h
        db  024h, 087h, 046h, 000h, 0A9h, 03Fh, 05Bh, 075h, 023h, 08Bh, 04Fh, 008h, 001h, 04Ch, 054h, 072h
        db  009h, 0F9h, 011h, 054h, 050h, 072h, 008h, 0E9h, 00Fh, 0FFh, 011h, 054h, 056h, 0EBh, 0F2h, 011h
        db  054h, 052h, 0E9h, 004h, 0FFh, 0C6h, 047h, 00Bh, 002h, 0E9h, 058h, 0FFh, 089h, 046h, 000h, 0C6h
        db  047h, 00Bh, 002h, 0E8h, 0BEh, 00Fh, 0E9h, 00Ah, 0FFh, 050h, 08Bh, 0C3h, 08Bh, 05Fh, 004h, 0F6h
        db  007h, 001h, 075h, 00Dh, 08Bh, 0D8h, 0C6h, 047h, 00Eh, 000h, 0E8h, 042h, 010h, 058h, 0E9h, 088h
        db  001h, 08Bh, 00Fh, 023h, 04Fh, 002h, 023h, 04Fh, 004h, 041h, 08Bh, 0D8h, 0C6h, 047h, 00Eh, 010h
        db  075h, 0E8h, 0C6h, 047h, 00Eh, 002h, 0EBh, 0E2h, 0C3h, 0FFh, 006h, 0F4h, 000h, 080h, 04Fh, 001h
        db  080h, 080h, 0E5h, 03Fh, 0EBh, 016h, 090h, 0F6h, 047h, 009h, 080h, 075h, 0ECh, 080h, 0E5h, 03Fh
        db  003h, 0D1h, 08Bh, 05Fh, 002h, 08Bh, 00Fh, 080h, 0C5h, 040h, 073h, 0EBh, 003h, 0CAh, 093h, 089h
        db  04Fh, 00Ch, 03Bh, 00Eh, 020h, 089h, 077h, 0A1h, 033h, 0D2h, 0F9h, 0EBh, 042h, 090h, 0E9h, 038h
        db  001h, 011h, 054h, 016h, 0EBh, 03Eh, 090h, 011h, 054h, 01Eh, 0EBh, 03Dh, 090h, 08Bh, 02Eh, 03Eh
        db  089h, 081h, 07Eh, 000h, 000h, 0A0h, 075h, 0B0h, 0BEh, 010h, 000h, 033h, 0D2h, 0BBh, 060h, 080h
        db  08Bh, 03Fh, 0D1h, 0E7h, 08Dh, 079h, 010h, 089h, 03Eh, 0CAh, 088h, 090h, 089h, 056h, 000h, 08Bh
        db  046h, 006h, 0ABh, 08Bh, 0D8h, 08Bh, 00Fh, 080h, 0C5h, 040h, 073h, 09Bh, 089h, 04Fh, 00Ch, 011h
        db  054h, 014h, 072h, 0BDh, 001h, 04Ch, 01Ch, 072h, 0BEh, 08Bh, 05Fh, 004h, 0F6h, 007h, 001h, 075h
        db  068h, 0F6h, 044h, 00Ah, 005h, 074h, 0A7h, 0F9h, 010h, 016h, 060h, 080h, 072h, 047h, 04Ah, 089h
        db  056h, 006h, 042h, 08Bh, 0CDh, 08Bh, 06Eh, 004h, 081h, 07Eh, 000h, 000h, 0A0h, 074h, 0BDh, 093h
        db  08Bh, 047h, 002h, 0A3h, 042h, 089h, 0B8h, 0FFh, 0FFh, 0A2h, 0E2h, 000h, 023h, 006h, 0E0h, 000h
        db  074h, 028h, 089h, 02Eh, 03Eh, 089h, 08Bh, 0D9h, 0C6h, 047h, 003h, 080h, 087h, 01Eh, 040h, 089h
        db  03Bh, 0D9h, 074h, 004h, 0C6h, 047h, 003h, 000h, 081h, 026h, 0A0h, 088h, 0FFh, 0F7h, 0A1h, 0F0h
        db  000h, 0A3h, 0CCh, 088h, 0C3h, 0BFh, 070h, 080h, 0EBh, 0B4h, 0A2h, 0E3h, 000h, 08Bh, 016h, 0ACh
        db  088h, 0B0h, 040h, 0A2h, 0E1h, 000h, 0EEh, 0EBh, 0C9h, 08Bh, 017h, 023h, 057h, 002h, 023h, 057h
        db  004h, 042h, 075h, 013h, 0F9h, 011h, 054h, 028h, 072h, 05Bh, 001h, 04Ch, 03Ch, 072h, 05Bh, 0F6h
        db  044h, 00Ah, 006h, 074h, 064h, 0EBh, 080h, 033h, 0D2h, 0F9h, 011h, 054h, 024h, 072h, 050h, 001h
        db  04Ch, 038h, 072h, 050h, 08Bh, 00Eh, 096h, 08Ah, 0E3h, 021h, 0BEh, 098h, 08Ah, 08Bh, 017h, 02Bh
        db  014h, 075h, 010h, 08Bh, 057h, 002h, 02Bh, 054h, 002h, 075h, 008h, 08Bh, 057h, 004h, 02Bh, 054h
        db  004h, 074h, 016h, 083h, 0C6h, 006h, 083h, 0E9h, 006h, 075h, 0E2h, 08Bh, 0D1h, 0BEh, 010h, 000h
        db  0F6h, 044h, 00Ah, 004h, 074h, 023h, 0E9h, 03Eh, 0FFh, 0BEh, 010h, 000h, 0F6h, 044h, 00Ah, 005h
        db  074h, 017h, 0E9h, 032h, 0FFh, 011h, 054h, 02Ah, 0EBh, 0A0h, 011h, 054h, 03Eh, 0EBh, 0A0h, 011h
        db  054h, 026h, 0EBh, 0ABh, 011h, 054h, 03Ah, 0EBh, 0ABh, 04Fh, 04Fh, 0BEh, 042h, 089h, 093h, 08Bh
        db  047h, 002h, 089h, 004h, 08Bh, 04Eh, 006h, 03Bh, 0CBh, 074h, 00Dh, 08Bh, 0D9h, 089h, 017h, 08Bh
        db  05Fh, 002h, 0F6h, 047h, 001h, 080h, 074h, 0F5h, 089h, 017h, 0B8h, 010h, 001h, 089h, 047h, 002h
        db  080h, 04Fh, 009h, 080h, 039h, 004h, 074h, 033h, 087h, 05Ch, 002h, 089h, 04Fh, 002h, 0EBh, 000h
        db  0EBh, 000h, 0EBh, 000h, 080h, 067h, 009h, 07Fh, 0BEh, 010h, 000h, 08Bh, 0CDh, 0C7h, 046h, 006h
        db  0FFh, 0FFh, 08Bh, 06Eh, 004h, 081h, 07Eh, 000h, 000h, 0A0h, 075h, 003h, 0E9h, 09Dh, 0FEh, 03Bh
        db  03Eh, 0CAh, 088h, 074h, 003h, 0E9h, 0DEh, 0FEh, 0E9h, 0E7h, 0FEh, 089h, 00Ch, 089h, 05Ch, 002h
        db  0EBh, 0D6h, 0E8h, 006h, 020h, 007h, 018h, 007h, 010h, 007h, 008h, 007h, 000h, 007h, 0F8h, 006h
        db  0F0h, 006h, 0C3h, 074h, 0FDh, 0B9h, 06Eh, 006h, 051h, 0B9h, 000h, 001h, 02Bh, 0C8h, 0C6h, 047h
        db  002h, 000h, 0EBh, 022h, 080h, 067h, 001h, 03Fh, 08Bh, 05Fh, 002h, 001h, 017h, 073h, 0F5h, 0EBh
        db  028h, 090h, 0BFh, 002h, 000h, 0E9h, 0BCh, 000h, 0BBh, 070h, 082h, 08Bh, 007h, 08Bh, 0C8h, 086h
        db  047h, 002h, 02Ah, 0C8h, 076h, 0CDh, 08Bh, 0F0h, 0D1h, 0E6h, 08Dh, 070h, 010h, 0ADh, 08Bh, 0D8h
        db  08Bh, 0E8h, 0BAh, 000h, 040h, 001h, 017h, 073h, 0CBh, 049h, 074h, 0D6h, 08Bh, 0F9h, 083h, 0E7h
        db  007h, 0D1h, 0E7h, 083h, 0C1h, 007h, 0D1h, 0E9h, 0D1h, 0E9h, 0D1h, 0E9h, 02Eh, 08Bh, 085h, 038h
        db  006h, 0BFh, 002h, 000h, 0FFh, 0E0h, 0B8h, 0F0h, 006h, 0EBh, 02Bh, 090h, 0B8h, 0F8h, 006h, 0EBh
        db  025h, 090h, 0B8h, 000h, 007h, 0EBh, 01Fh, 090h, 0B8h, 008h, 007h, 0EBh, 019h, 090h, 0B8h, 010h
        db  007h, 0EBh, 013h, 090h, 0B8h, 018h, 007h, 0EBh, 00Dh, 090h, 0B8h, 020h, 007h, 0EBh, 007h, 090h
        db  0B8h, 028h, 007h, 0EBh, 001h, 090h, 080h, 067h, 001h, 03Fh, 08Bh, 019h, 001h, 017h, 073h, 0F6h
        db  0FFh, 0E0h, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0BCh, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0BAh, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0B8h, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0B6h, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0B4h, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0B2h, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0B0h, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0AEh, 0E2h, 0BEh, 0B8h, 010h, 001h, 089h, 001h, 080h, 04Fh, 009h, 080h, 0BEh, 042h, 089h
        db  039h, 004h, 074h, 010h, 087h, 05Ch, 002h, 089h, 029h, 0EBh, 000h, 0EBh, 000h, 0EBh, 000h, 080h
        db  067h, 009h, 07Fh, 0C3h, 089h, 02Ch, 089h, 05Ch, 002h, 0C3h, 0F7h, 006h, 000h, 001h, 040h, 000h
        db  074h, 001h, 0C3h, 0F7h, 006h, 0A0h, 088h, 000h, 010h, 074h, 0F7h, 0E8h, 00Ah, 0FFh, 081h, 03Eh
        db  042h, 089h, 010h, 001h, 074h, 0ECh, 083h, 03Eh, 002h, 001h, 000h, 075h, 0E5h, 0E8h, 02Dh, 0FDh
        db  083h, 03Eh, 002h, 001h, 000h, 075h, 04Bh, 0BFh, 03Eh, 089h, 08Bh, 01Dh, 03Bh, 05Dh, 002h, 074h
        db  041h, 0BFh, 042h, 089h, 08Bh, 005h, 03Dh, 010h, 001h, 074h, 037h, 03Bh, 045h, 002h, 074h, 032h
        db  089h, 047h, 006h, 093h, 0BBh, 000h, 001h, 089h, 047h, 006h, 0C7h, 047h, 002h, 010h, 000h, 08Bh
        db  016h, 0A8h, 088h, 0EEh, 033h, 0C9h, 0F7h, 007h, 040h, 000h, 075h, 006h, 0E2h, 0F8h, 0FFh, 006h
        db  0A6h, 088h, 0FFh, 006h, 0E6h, 000h, 0A1h, 0F0h, 000h, 0A3h, 0CCh, 088h, 081h, 026h, 0A0h, 088h
        db  0FFh, 0F7h, 0C3h, 0F7h, 006h, 0A0h, 088h, 000h, 010h, 074h, 03Ah, 083h, 03Eh, 0CEh, 088h, 000h
        db  07Eh, 033h, 0F7h, 006h, 0A0h, 088h, 000h, 008h, 075h, 00Eh, 0A1h, 0F0h, 000h, 02Bh, 006h, 0CCh
        db  088h, 03Bh, 006h, 0CEh, 088h, 07Fh, 001h, 0C3h, 083h, 03Eh, 0EAh, 000h, 000h, 075h, 010h, 0F7h
        db  006h, 0A0h, 088h, 000h, 004h, 075h, 008h, 0FFh, 006h, 0F8h, 000h, 0E8h, 01Ah, 001h, 0C3h, 081h
        db  00Eh, 0A0h, 088h, 000h, 008h, 0C3h, 0E8h, 08Bh, 001h, 033h, 0C0h, 08Ch, 0D9h, 0C1h, 0E9h, 00Ch
        db  0BEh, 070h, 089h, 01Eh, 0C5h, 01Eh, 04Eh, 089h, 088h, 007h, 089h, 077h, 006h, 088h, 04Fh, 008h
        db  01Fh, 0BBh, 000h, 001h, 089h, 05Ch, 002h, 089h, 044h, 004h, 088h, 04Ch, 006h, 089h, 007h, 08Dh
        db  07Fh, 008h, 0B9h, 004h, 000h, 0F3h, 0ABh, 048h, 089h, 047h, 004h, 089h, 047h, 006h, 01Eh, 00Eh
        db  01Fh, 0BEh, 034h, 000h, 0BFh, 060h, 08Ah, 0B9h, 01Ch, 000h, 0EBh, 000h, 0F3h, 0A5h, 0BEh, 06Ch
        db  000h, 0BFh, 030h, 08Ah, 0B9h, 00Ah, 000h, 0EBh, 000h, 0F3h, 0A5h, 01Fh, 0BBh, 07Eh, 08Ah, 0A0h
        db  026h, 089h, 088h, 047h, 007h, 0A0h, 02Fh, 089h, 0C0h, 0E0h, 003h, 00Ah, 006h, 02Eh, 089h, 0C0h
        db  0E0h, 004h, 00Ah, 006h, 02Dh, 089h, 088h, 047h, 00Ah, 0A0h, 027h, 089h, 0C0h, 0E0h, 004h, 080h
        db  067h, 009h, 0CFh, 008h, 047h, 009h, 0A0h, 029h, 089h, 088h, 047h, 00Bh, 032h, 0C0h, 08Ah, 026h
        db  02Ch, 089h, 0C0h, 0E4h, 004h, 00Bh, 006h, 02Ah, 089h, 088h, 067h, 00Dh, 088h, 047h, 00Ch, 0A0h
        db  028h, 089h, 0C0h, 0E0h, 005h, 080h, 067h, 00Eh, 0DFh, 008h, 047h, 00Eh, 0A0h, 031h, 089h, 0C0h
        db  0E0h, 004h, 00Ah, 006h, 030h, 089h, 080h, 067h, 00Fh, 088h, 008h, 047h, 00Fh, 0B0h, 004h, 080h
        db  03Eh, 028h, 089h, 000h, 074h, 002h, 0B0h, 002h, 002h, 006h, 032h, 089h, 088h, 047h, 010h, 0F7h
        db  006h, 0A2h, 088h, 000h, 040h, 074h, 00Ah, 0B0h, 0BFh, 020h, 006h, 068h, 08Ah, 020h, 006h, 086h
        db  08Ah, 01Eh, 0C5h, 01Eh, 04Eh, 089h, 08Ah, 047h, 002h, 01Fh, 03Ch, 0CCh, 074h, 019h, 0A8h, 080h
        db  074h, 015h, 081h, 00Eh, 0A2h, 088h, 000h, 080h, 0BBh, 07Eh, 08Ah, 080h, 04Fh, 00Eh, 040h, 080h
        db  04Fh, 00Ah, 080h, 0C6h, 047h, 00Bh, 055h, 01Eh, 0C5h, 036h, 04Ah, 089h, 0BFh, 078h, 08Ah, 0B9h
        db  003h, 000h, 0EBh, 000h, 0F3h, 0A5h, 01Fh, 0C3h, 050h, 053h, 051h, 052h, 056h, 0E8h, 074h, 000h
        db  0BBh, 000h, 001h, 08Bh, 047h, 00Eh, 0C7h, 007h, 000h, 000h, 0BEh, 070h, 089h, 0C6h, 004h, 001h
        db  08Bh, 016h, 0A8h, 088h, 0EEh, 033h, 0C9h, 0E2h, 0FEh, 089h, 047h, 00Eh, 033h, 0C9h, 080h, 03Ch
        db  000h, 0E0h, 0FBh, 0BEh, 000h, 001h, 08Bh, 004h, 03Dh, 000h, 0A0h, 0E0h, 0F9h, 089h, 044h, 002h
        db  0EEh, 083h, 07Ch, 002h, 000h, 0E0h, 0FAh, 083h, 026h, 0A0h, 088h, 0BFh, 0BBh, 060h, 08Ah, 0C7h
        db  007h, 000h, 000h, 0F7h, 047h, 002h, 000h, 080h, 075h, 005h, 08Bh, 05Fh, 004h, 0EBh, 0F0h, 0C7h
        db  044h, 002h, 000h, 001h, 0C7h, 044h, 004h, 060h, 08Ah, 0EEh, 033h, 0C9h, 0F7h, 007h, 000h, 080h
        db  0E1h, 0FAh, 081h, 026h, 0A0h, 088h, 0FFh, 0F7h, 0A1h, 0F0h, 000h, 0A3h, 0CCh, 088h, 05Eh, 05Ah
        db  059h, 05Bh, 058h, 0C3h, 083h, 03Eh, 0AAh, 088h, 000h, 074h, 027h, 050h, 051h, 052h, 08Bh, 016h
        db  0AAh, 088h, 033h, 0C0h, 0EEh, 0B9h, 064h, 000h, 0E2h, 0FEh, 040h, 0EEh, 0B9h, 064h, 000h, 0E2h
        db  0FEh, 08Bh, 016h, 0AEh, 088h, 0A0h, 033h, 089h, 0EEh, 083h, 00Eh, 0A0h, 088h, 040h, 05Ah, 059h
        db  058h, 0C3h, 0F7h, 006h, 0A0h, 088h, 040h, 000h, 074h, 001h, 0C3h, 050h, 053h, 051h, 052h, 056h
        db  006h, 0C4h, 01Eh, 04Eh, 089h, 026h, 08Ah, 047h, 008h, 026h, 08Bh, 05Fh, 006h, 0C1h, 0E0h, 00Ch
        db  08Eh, 0C0h, 026h, 08Bh, 057h, 002h, 026h, 08Ah, 047h, 006h, 026h, 08Bh, 05Fh, 004h, 003h, 0DAh
        db  015h, 000h, 000h, 0C1h, 0E0h, 00Ch, 08Eh, 0C0h, 026h, 0C7h, 047h, 002h, 080h, 000h, 08Bh, 016h
        db  0A8h, 088h, 0BEh, 00Ah, 000h, 0EEh, 033h, 0C9h, 026h, 083h, 07Fh, 002h, 000h, 074h, 00Fh, 0E2h
        db  0F7h, 0FFh, 006h, 0FAh, 000h, 04Eh, 075h, 0EDh, 026h, 0C7h, 047h, 002h, 000h, 000h, 083h, 00Eh
        db  0A0h, 088h, 040h, 007h, 05Eh, 05Ah, 059h, 05Bh, 058h, 0C3h, 050h, 051h, 052h, 056h, 057h, 055h
        db  006h, 053h, 08Bh, 0EBh, 0E8h, 0CFh, 000h, 089h, 01Eh, 0D6h, 088h, 074h, 077h, 0C7h, 006h, 0D8h
        db  088h, 000h, 000h, 080h, 04Fh, 001h, 0C0h, 08Bh, 07Fh, 004h, 08Bh, 047h, 008h, 025h, 0FFh, 03Fh
        db  08Bh, 076h, 004h, 08Bh, 056h, 000h, 081h, 0E2h, 0FFh, 03Fh, 08Bh, 0C8h, 03Bh, 0CAh, 07Eh, 002h
        db  08Bh, 0CAh, 051h, 01Eh, 006h, 08Eh, 006h, 0EAh, 088h, 08Eh, 01Eh, 0ECh, 088h, 0D1h, 0E9h, 0EBh
        db  000h, 0F3h, 0A5h, 073h, 001h, 0A4h, 007h, 01Fh, 059h, 001h, 00Fh, 001h, 00Eh, 0D8h, 088h, 02Bh
        db  0C1h, 02Bh, 0D1h, 074h, 036h, 095h, 08Bh, 0EBh, 0E8h, 07Bh, 000h, 089h, 05Eh, 002h, 074h, 014h
        db  080h, 066h, 001h, 07Fh, 095h, 080h, 04Fh, 001h, 0C0h, 08Bh, 07Fh, 004h, 08Bh, 047h, 008h, 025h
        db  0FFh, 03Fh, 0EBh, 0B6h, 095h, 08Bh, 01Eh, 0D6h, 088h, 0FFh, 077h, 002h, 0E8h, 0F9h, 000h, 03Bh
        db  0D8h, 05Bh, 075h, 0F5h, 0FFh, 006h, 0DAh, 088h, 0EBh, 043h, 090h, 0F6h, 046h, 001h, 080h, 075h
        db  014h, 08Bh, 06Eh, 002h, 08Bh, 076h, 004h, 08Bh, 056h, 000h, 081h, 0E2h, 0FFh, 03Fh, 03Dh, 000h
        db  000h, 074h, 0B2h, 0EBh, 085h, 05Eh, 08Bh, 03Eh, 0D6h, 088h, 0A1h, 0D8h, 088h, 089h, 045h, 00Ch
        db  08Ah, 044h, 00Bh, 088h, 045h, 00Eh, 056h, 053h, 08Bh, 0DFh, 0E8h, 092h, 009h, 08Bh, 0C3h, 05Bh
        db  0E8h, 088h, 000h, 083h, 006h, 0DCh, 088h, 001h, 083h, 016h, 0DEh, 088h, 000h, 05Bh, 007h, 05Dh
        db  05Fh, 05Eh, 05Ah, 059h, 058h, 0C3h, 050h, 051h, 056h, 057h, 055h, 08Bh, 01Eh, 042h, 089h, 081h
        db  0FBh, 010h, 001h, 074h, 050h, 0B8h, 080h, 040h, 0BEh, 002h, 000h, 0BFh, 009h, 000h, 084h, 067h
        db  001h, 074h, 008h, 084h, 001h, 075h, 04Fh, 08Bh, 018h, 0EBh, 0F3h, 084h, 001h, 075h, 047h, 08Bh
        db  018h, 08Bh, 0EBh, 084h, 001h, 075h, 03Fh, 08Bh, 018h, 084h, 001h, 075h, 039h, 08Bh, 018h, 084h
        db  001h, 075h, 033h, 08Bh, 018h, 084h, 001h, 075h, 02Dh, 08Bh, 038h, 084h, 045h, 009h, 075h, 026h
        db  08Bh, 04Dh, 002h, 084h, 066h, 001h, 075h, 017h, 089h, 008h, 08Bh, 0DFh, 0C7h, 007h, 000h, 000h
        db  0C7h, 047h, 002h, 000h, 000h, 05Dh, 05Fh, 05Eh, 059h, 058h, 081h, 0FBh, 010h, 001h, 0C3h, 08Bh
        db  0DDh, 0BFh, 009h, 000h, 0EBh, 0A8h, 0BBh, 010h, 001h, 0EBh, 0EAh, 0C7h, 047h, 002h, 000h, 000h
        db  0BFh, 060h, 080h, 08Bh, 01Dh, 0D1h, 0E3h, 089h, 041h, 010h, 0FEh, 005h, 0C6h, 006h, 0E2h, 000h
        db  0FFh, 0A1h, 0E0h, 000h, 085h, 0C0h, 075h, 00Fh, 0A2h, 0E3h, 000h, 08Bh, 016h, 0ACh, 088h, 0B0h
        db  040h, 0C6h, 006h, 0E1h, 000h, 0FFh, 0EEh, 0C3h, 050h, 053h, 057h, 0C7h, 007h, 000h, 000h, 0C7h
        db  047h, 002h, 010h, 001h, 080h, 04Fh, 009h, 080h, 0BFh, 042h, 089h, 081h, 03Dh, 010h, 001h, 074h
        db  016h, 08Bh, 0C3h, 087h, 05Dh, 002h, 089h, 047h, 002h, 0EBh, 000h, 0EBh, 000h, 0EBh, 000h, 080h
        db  067h, 009h, 07Fh, 05Fh, 05Bh, 058h, 0C3h, 089h, 01Dh, 089h, 05Dh, 002h, 0EBh, 0F5h, 0E4h, 00Eh
        db  055h, 00Ch, 023h, 00Dh, 026h, 00Dh, 07Dh, 00Dh, 096h, 00Dh, 0B0h, 00Dh, 0CCh, 00Dh, 00Fh, 00Eh
        db  089h, 00Eh, 094h, 00Eh, 097h, 00Eh, 0D5h, 00Eh, 0D8h, 00Eh, 0DBh, 00Eh, 08Bh, 036h, 04Eh, 080h
        db  08Bh, 044h, 002h, 03Ah, 004h, 075h, 001h, 0C3h, 0BFh, 000h, 001h, 083h, 07Dh, 002h, 000h, 075h
        db  0F6h, 0F7h, 005h, 000h, 002h, 075h, 0F0h, 083h, 03Eh, 0EAh, 000h, 000h, 075h, 0E9h, 0F7h, 006h
        db  0A0h, 088h, 000h, 008h, 075h, 0E1h, 08Bh, 01Eh, 05Eh, 080h, 08Bh, 007h, 0FEh, 0C0h, 03Ah, 047h
        db  004h, 076h, 002h, 0B0h, 000h, 03Ah, 047h, 002h, 074h, 0CDh, 08Bh, 044h, 002h, 0F6h, 064h, 005h
        db  003h, 044h, 006h, 08Bh, 0E8h, 08Bh, 05Eh, 000h, 0D1h, 0E3h, 02Eh, 0FFh, 0A7h, 0E4h, 00Bh, 0F7h
        db  006h, 0A0h, 088h, 000h, 004h, 075h, 03Ch, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 074h, 003h, 0E8h
        db  0B6h, 0FCh, 0BBh, 030h, 08Ah, 033h, 0C0h, 089h, 007h, 0A3h, 036h, 08Ah, 0A3h, 03Ch, 08Ah, 0A3h
        db  042h, 08Ah, 089h, 01Eh, 004h, 001h, 0C7h, 006h, 002h, 001h, 000h, 001h, 08Bh, 016h, 0A8h, 088h
        db  0EEh, 0A1h, 0F0h, 000h, 0A3h, 0D2h, 088h, 081h, 00Eh, 0A0h, 088h, 000h, 004h, 080h, 00Eh, 016h
        db  000h, 020h, 0C3h, 0F7h, 006h, 03Ch, 08Ah, 000h, 080h, 075h, 015h, 0A1h, 0F0h, 000h, 02Bh, 006h
        db  0D2h, 088h, 03Bh, 006h, 0D4h, 088h, 07Fh, 001h, 0C3h, 0FFh, 006h, 0D0h, 088h, 0E8h, 068h, 0FCh
        db  081h, 026h, 0A0h, 088h, 0FFh, 0FBh, 0B2h, 007h, 081h, 03Eh, 030h, 08Ah, 000h, 0A0h, 074h, 002h
        db  0B2h, 004h, 081h, 03Eh, 036h, 08Ah, 000h, 0A0h, 074h, 002h, 0B2h, 004h, 081h, 03Eh, 03Ch, 08Ah
        db  000h, 0A0h, 075h, 020h, 0A0h, 043h, 08Ah, 0A8h, 080h, 075h, 01Bh, 0A8h, 040h, 075h, 015h, 0A8h
        db  020h, 074h, 005h, 080h, 00Eh, 0C8h, 000h, 008h, 0A8h, 010h, 074h, 00Ah, 080h, 00Eh, 0C8h, 000h
        db  004h, 0EBh, 003h, 090h, 0B2h, 004h, 080h, 026h, 016h, 000h, 0F8h, 008h, 016h, 016h, 000h, 080h
        db  026h, 016h, 000h, 0DFh, 0B8h, 000h, 000h, 080h, 0FAh, 007h, 074h, 003h, 0B8h, 00Ah, 000h, 0F7h
        db  006h, 0A0h, 088h, 000h, 060h, 074h, 003h, 0E8h, 07Ah, 0FCh, 0E9h, 0CAh, 001h, 0E9h, 0B8h, 001h
        db  08Dh, 076h, 006h, 0BFh, 078h, 08Ah, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0BBh, 060h, 08Ah, 033h
        db  0C0h, 089h, 007h, 0A3h, 072h, 08Ah, 0A3h, 07Eh, 08Ah, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 075h
        db  030h, 081h, 00Eh, 080h, 08Ah, 000h, 080h, 089h, 01Eh, 004h, 001h, 0C7h, 006h, 002h, 001h, 000h
        db  001h, 08Bh, 016h, 0A8h, 088h, 0EEh, 033h, 0C9h, 0F7h, 006h, 07Eh, 08Ah, 000h, 080h, 0E1h, 0F8h
        db  081h, 026h, 080h, 08Ah, 0FFh, 07Fh, 0B8h, 0FFh, 000h, 0F7h, 006h, 07Eh, 08Ah, 000h, 020h, 074h
        db  003h, 0B8h, 000h, 000h, 0E9h, 070h, 001h, 083h, 03Eh, 01Ah, 000h, 000h, 074h, 006h, 081h, 00Eh
        db  0A0h, 088h, 000h, 010h, 0E8h, 091h, 0FBh, 081h, 026h, 0A0h, 088h, 0FFh, 09Fh, 0E9h, 054h, 001h
        db  0E8h, 001h, 0FCh, 033h, 0C0h, 0A3h, 096h, 08Ah, 0A3h, 0E4h, 088h, 081h, 026h, 0A0h, 088h, 0FFh
        db  0EFh, 081h, 00Eh, 0A0h, 088h, 000h, 040h, 0E9h, 03Ah, 001h, 055h, 0E8h, 0E6h, 0FBh, 0E8h, 013h
        db  004h, 0C7h, 006h, 0E4h, 000h, 000h, 000h, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 075h, 003h, 0E8h
        db  056h, 0FBh, 05Dh, 0E9h, 01Eh, 001h, 08Bh, 046h, 004h, 0A9h, 0F8h, 0FFh, 075h, 035h, 085h, 0C0h
        db  074h, 00Eh, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 075h, 006h, 081h, 00Eh, 0A0h, 088h, 000h, 010h
        db  0A3h, 01Ah, 000h, 0BBh, 07Eh, 08Ah, 0C7h, 007h, 000h, 000h, 080h, 067h, 00Eh, 0FCh, 0A9h, 002h
        db  000h, 075h, 004h, 080h, 04Fh, 00Eh, 002h, 0A9h, 004h, 000h, 074h, 004h, 080h, 04Fh, 00Eh, 001h
        db  0E9h, 036h, 0FFh, 0B8h, 007h, 000h, 0E9h, 0DEh, 000h, 09Ch, 0FCh, 051h, 052h, 050h, 056h, 057h
        db  0A1h, 0E0h, 088h, 0B9h, 006h, 000h, 0F7h, 0E1h, 0BFh, 098h, 08Ah, 08Bh, 0F7h, 003h, 0F0h, 08Bh
        db  0C8h, 0D1h, 0E9h, 0F3h, 0A5h, 0BBh, 090h, 08Ah, 08Bh, 046h, 004h, 089h, 047h, 006h, 05Fh, 05Eh
        db  058h, 05Ah, 059h, 09Dh, 0BBh, 090h, 08Ah, 0C7h, 007h, 000h, 000h, 0F7h, 006h, 0A0h, 088h, 000h
        db  060h, 075h, 020h, 089h, 01Eh, 004h, 001h, 0C7h, 006h, 002h, 001h, 000h, 001h, 08Bh, 016h, 0A8h
        db  088h, 0EEh, 033h, 0C9h, 0F7h, 007h, 000h, 080h, 0E1h, 0FAh, 0B8h, 0FFh, 000h, 0F7h, 007h, 000h
        db  020h, 074h, 003h, 0B8h, 000h, 000h, 0EBh, 07Fh, 090h, 08Dh, 07Fh, 008h, 003h, 07Fh, 006h, 08Dh
        db  075h, 0FAh, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0B8h, 006h, 000h, 001h, 047h, 006h, 0A3h, 0E4h
        db  088h, 0EBh, 0B4h, 08Dh, 076h, 006h, 0E8h, 0CFh, 000h, 073h, 0ACh, 0EBh, 05Ah, 090h, 0EBh, 054h
        db  090h, 033h, 0C0h, 0BBh, 090h, 000h, 08Dh, 07Fh, 004h, 08Dh, 04Fh, 040h, 02Bh, 0CFh, 0D1h, 0E9h
        db  0F3h, 0ABh, 048h, 089h, 047h, 020h, 089h, 047h, 022h, 089h, 047h, 030h, 089h, 047h, 032h, 089h
        db  047h, 034h, 089h, 047h, 036h, 040h, 0BBh, 010h, 000h, 08Dh, 07Fh, 014h, 08Dh, 04Fh, 064h, 02Bh
        db  0CFh, 0D1h, 0E9h, 0F3h, 0ABh, 048h, 089h, 047h, 034h, 089h, 047h, 036h, 0EBh, 016h, 090h, 0EBh
        db  013h, 090h, 0EBh, 004h, 090h, 0EBh, 00Dh, 090h, 0B8h, 009h, 000h, 0EBh, 00Ah, 090h, 0B8h, 008h
        db  000h, 0EBh, 004h, 090h, 0B8h, 000h, 000h, 08Bh, 04Eh, 002h, 096h, 08Bh, 01Eh, 04Eh, 080h, 08Bh
        db  047h, 002h, 0FEh, 0C0h, 03Ah, 047h, 004h, 076h, 002h, 0B0h, 000h, 088h, 047h, 002h, 08Bh, 01Eh
        db  05Eh, 080h, 08Bh, 007h, 0F6h, 067h, 005h, 003h, 047h, 006h, 096h, 089h, 044h, 002h, 089h, 00Ch
        db  08Bh, 007h, 0FEh, 0C0h, 03Ah, 047h, 004h, 076h, 002h, 0B0h, 000h, 088h, 007h, 0C6h, 006h, 0E2h
        db  000h, 0FFh, 0C3h, 0F6h, 004h, 001h, 074h, 026h, 0E8h, 055h, 000h, 075h, 021h, 08Bh, 047h, 006h
        db  02Bh, 006h, 0E4h, 088h, 03Bh, 006h, 0E2h, 088h, 07Dh, 019h, 005h, 006h, 000h, 089h, 047h, 006h
        db  0C7h, 006h, 0E4h, 088h, 000h, 000h, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0F8h, 0C3h, 0B8h, 007h
        db  000h, 0F9h, 0C3h, 0B8h, 008h, 000h, 0F9h, 0C3h, 0E8h, 025h, 000h, 074h, 01Eh, 08Bh, 077h, 006h
        db  083h, 0EEh, 006h, 02Bh, 036h, 0E4h, 088h, 089h, 077h, 006h, 0C7h, 006h, 0E4h, 088h, 000h, 000h
        db  08Dh, 070h, 008h, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0F8h, 0C3h, 0B8h, 007h, 000h, 0F9h, 0C3h
        db  0BBh, 090h, 08Ah, 08Bh, 047h, 006h, 08Dh, 07Fh, 008h, 085h, 0C0h, 074h, 016h, 02Bh, 006h, 0E4h
        db  088h, 056h, 0B9h, 003h, 000h, 0F3h, 0A7h, 05Eh, 074h, 00Ah, 0D1h, 0E1h, 003h, 0F9h, 02Dh, 006h
        db  000h, 075h, 0EEh, 0C3h, 083h, 0EFh, 006h, 0C3h, 0A1h, 0B6h, 088h, 0F7h, 0D0h, 0A3h, 0B8h, 088h
        db  0B9h, 010h, 000h, 0A1h, 0E8h, 088h, 0F7h, 0E1h, 005h, 0FDh, 014h, 072h, 06Ch, 025h, 0F0h, 0FFh
        db  089h, 016h, 0ECh, 088h, 0C1h, 026h, 0ECh, 088h, 00Ch, 0A3h, 0FCh, 088h, 08Bh, 0FAh, 08Bh, 0F0h
        db  023h, 006h, 0B8h, 088h, 003h, 006h, 0BAh, 088h, 013h, 016h, 0BCh, 088h, 08Bh, 0D8h, 08Bh, 0CAh
        db  02Bh, 0C6h, 01Bh, 0D7h, 08Bh, 036h, 0F2h, 088h, 083h, 0C6h, 003h, 083h, 0E6h, 0FCh, 0F7h, 0F6h
        db  08Bh, 0E8h, 085h, 0C0h, 075h, 004h, 089h, 01Eh, 0FCh, 088h, 083h, 03Eh, 0BAh, 088h, 000h, 074h
        db  01Ch, 08Bh, 0D1h, 033h, 0C0h, 042h, 02Bh, 0C3h, 01Bh, 0D1h, 0F7h, 036h, 0BAh, 088h, 08Bh, 0C8h
        db  0A1h, 0BAh, 088h, 08Bh, 016h, 0BCh, 088h, 0F7h, 0F6h, 0F7h, 0E1h, 003h, 0E8h, 083h, 0FDh, 020h
        db  076h, 003h, 0BDh, 020h, 000h, 089h, 02Eh, 0F4h, 088h, 0B8h, 000h, 080h, 083h, 03Eh, 0BCh, 088h
        db  000h, 075h, 009h, 03Bh, 006h, 0BAh, 088h, 076h, 003h, 0A1h, 0BAh, 088h, 02Dh, 020h, 001h, 08Bh
        db  00Eh, 0EEh, 088h, 083h, 0C1h, 003h, 080h, 0E1h, 0FCh, 083h, 0C1h, 010h, 033h, 0D2h, 0F7h, 0F1h
        db  0A3h, 0F0h, 088h, 089h, 016h, 0FEh, 088h, 02Dh, 0FFh, 000h, 07Eh, 00Ch, 0F7h, 0E1h, 001h, 006h
        db  0FEh, 088h, 0B8h, 0FFh, 000h, 0A3h, 0F0h, 088h, 08Bh, 00Eh, 0F0h, 088h, 0BEh, 020h, 001h, 089h
        db  036h, 0DEh, 000h, 0B8h, 010h, 000h, 0F7h, 0E1h, 003h, 0F0h, 083h, 0C6h, 00Fh, 083h, 0E6h, 0F0h
        db  089h, 036h, 0F6h, 088h, 08Ch, 01Eh, 0EAh, 088h, 0A1h, 0EEh, 088h, 005h, 003h, 000h, 024h, 0FCh
        db  0F7h, 0E1h, 003h, 0F0h, 089h, 036h, 03Ah, 089h, 0BFh, 098h, 08Ah, 0A1h, 0E0h, 088h, 0B9h, 006h
        db  000h, 0F7h, 0E1h, 0A3h, 0E2h, 088h, 03Dh, 012h, 000h, 075h, 003h, 005h, 006h, 000h, 0D1h, 0E0h
        db  003h, 0F8h, 0A1h, 0F0h, 088h, 0B9h, 008h, 000h, 0F7h, 0E1h, 08Bh, 0D8h, 083h, 0C3h, 00Fh, 083h
        db  0E3h, 0F0h, 03Bh, 01Eh, 0FEh, 088h, 077h, 016h, 029h, 01Eh, 0FEh, 088h, 083h, 0C6h, 00Fh, 083h
        db  0E6h, 0F0h, 089h, 036h, 0D6h, 000h, 003h, 0F0h, 089h, 036h, 03Ah, 089h, 0EBh, 00Ch, 083h, 0C7h
        db  00Fh, 083h, 0E7h, 0F0h, 089h, 03Eh, 0D6h, 000h, 003h, 0F8h, 083h, 0C7h, 00Fh, 083h, 0E7h, 0F0h
        db  089h, 03Eh, 0F8h, 088h, 08Bh, 00Eh, 0F4h, 088h, 0B8h, 008h, 000h, 0F7h, 0E1h, 003h, 0F8h, 083h
        db  0C7h, 00Fh, 083h, 0E7h, 0F0h, 089h, 03Eh, 0FAh, 088h, 0B8h, 010h, 000h, 0F7h, 0E1h, 003h, 0F8h
        db  08Bh, 00Eh, 0E6h, 088h, 041h, 0B8h, 00Ch, 000h, 0F7h, 0E1h, 005h, 010h, 000h, 083h, 0C7h, 00Fh
        db  083h, 0E7h, 0F0h, 089h, 03Eh, 04Eh, 080h, 003h, 0F8h, 0B8h, 004h, 000h, 0F7h, 0E1h, 005h, 010h
        db  000h, 083h, 0C7h, 00Fh, 083h, 0E7h, 0F0h, 089h, 03Eh, 05Eh, 080h, 003h, 0F8h, 089h, 03Eh, 03Ch
        db  089h, 0BFh, 098h, 08Ah, 08Bh, 00Eh, 03Ch, 089h, 02Bh, 0CFh, 033h, 0C0h, 0D1h, 0E9h, 0F3h, 0ABh
        db  0BBh, 080h, 084h, 089h, 01Eh, 00Eh, 080h, 0BEh, 090h, 086h, 089h, 036h, 01Eh, 080h, 08Ch, 0D8h
        db  0C1h, 0E8h, 00Ch, 08Bh, 03Eh, 0FAh, 088h, 0E8h, 013h, 002h, 088h, 047h, 004h, 088h, 044h, 004h
        db  0BBh, 060h, 080h, 089h, 01Eh, 02Eh, 080h, 0BEh, 070h, 082h, 089h, 036h, 03Eh, 080h, 08Ch, 0D8h
        db  0C1h, 0E8h, 00Ch, 08Bh, 03Eh, 0DEh, 000h, 0E8h, 0F3h, 001h, 088h, 047h, 004h, 088h, 044h, 004h
        db  08Bh, 00Eh, 0E6h, 088h, 08Bh, 01Eh, 04Eh, 080h, 088h, 04Fh, 004h, 0C6h, 047h, 005h, 00Ch, 08Dh
        db  07Fh, 010h, 089h, 07Fh, 006h, 023h, 03Eh, 0B6h, 088h, 089h, 07Fh, 008h, 08Bh, 01Eh, 05Eh, 080h
        db  088h, 04Fh, 004h, 0C6h, 047h, 005h, 004h, 08Dh, 07Fh, 010h, 089h, 07Fh, 006h, 023h, 03Eh, 0B6h
        db  088h, 089h, 07Fh, 008h, 033h, 0C0h, 0BEh, 080h, 084h, 088h, 004h, 088h, 044h, 002h, 0BEh, 090h
        db  086h, 088h, 004h, 088h, 044h, 002h, 0BEh, 060h, 080h, 088h, 004h, 088h, 044h, 002h, 0BEh, 070h
        db  082h, 088h, 004h, 088h, 044h, 002h, 0BBh, 010h, 001h, 0C7h, 047h, 002h, 0FFh, 0FFh, 0C7h, 047h
        db  004h, 01Ah, 001h, 0A1h, 0EAh, 088h, 0C1h, 0E8h, 00Ch, 088h, 047h, 006h, 0C7h, 047h, 008h, 002h
        db  080h, 0C7h, 006h, 042h, 089h, 010h, 001h, 0C7h, 006h, 044h, 089h, 000h, 000h, 08Bh, 00Eh, 0F0h
        db  088h, 08Bh, 03Eh, 0F6h, 088h, 08Bh, 01Eh, 0DEh, 000h, 08Bh, 016h, 0EEh, 088h, 089h, 07Fh, 004h
        db  0A1h, 0EAh, 088h, 0C1h, 0E8h, 00Ch, 088h, 047h, 006h, 0C6h, 047h, 007h, 000h, 089h, 057h, 008h
        db  0E8h, 075h, 0F9h, 083h, 0C3h, 010h, 003h, 0FAh, 083h, 0C7h, 003h, 083h, 0E7h, 0FCh, 0E2h, 0DDh
        db  08Bh, 00Eh, 0F0h, 088h, 0A1h, 0D6h, 000h, 08Bh, 0D8h, 0C7h, 007h, 000h, 000h, 0C6h, 047h, 003h
        db  000h, 005h, 008h, 000h, 089h, 047h, 004h, 0C7h, 047h, 006h, 0FFh, 0FFh, 0E2h, 0E9h, 0C6h, 047h
        db  003h, 080h, 0A1h, 0D6h, 000h, 089h, 047h, 004h, 0A3h, 03Eh, 089h, 089h, 01Eh, 040h, 089h, 08Bh
        db  00Eh, 0F4h, 088h, 08Eh, 006h, 0ECh, 088h, 08Bh, 03Eh, 0FCh, 088h, 08Bh, 036h, 0FAh, 088h, 08Bh
        db  016h, 0F2h, 088h, 083h, 0C2h, 003h, 080h, 0E2h, 0FCh, 08Bh, 0EFh, 023h, 02Eh, 0B6h, 088h, 003h
        db  0EAh, 085h, 02Eh, 0B8h, 088h, 074h, 00Eh, 085h, 02Eh, 0B6h, 088h, 074h, 008h, 023h, 03Eh, 0B8h
        db  088h, 003h, 03Eh, 0BAh, 088h, 08Bh, 0DEh, 083h, 0C6h, 010h, 0C7h, 007h, 000h, 000h, 0C7h, 047h
        db  002h, 0FFh, 0FFh, 089h, 07Fh, 004h, 08Ch, 0C0h, 0C1h, 0E8h, 00Ch, 088h, 047h, 006h, 0E8h, 0BCh
        db  000h, 088h, 047h, 007h, 08Bh, 0C7h, 023h, 006h, 0B6h, 088h, 089h, 047h, 00Eh, 0F7h, 006h, 0A0h
        db  088h, 000h, 080h, 075h, 00Ah, 057h, 051h, 033h, 0C0h, 08Bh, 0CAh, 0F3h, 0AAh, 059h, 05Fh, 003h
        db  0FAh, 0E2h, 0A6h, 08Ch, 0D8h, 08Eh, 0C0h, 0C7h, 006h, 046h, 089h, 000h, 000h, 08Bh, 00Eh, 0F4h
        db  088h, 08Bh, 03Eh, 0FAh, 088h, 08Bh, 01Eh, 0F8h, 088h, 08Bh, 0F3h, 083h, 0C3h, 008h, 0C7h, 044h
        db  002h, 004h, 000h, 089h, 05Ch, 004h, 089h, 07Ch, 006h, 083h, 0C7h, 010h, 0E2h, 0EBh, 0A1h, 0F8h
        db  088h, 089h, 044h, 004h, 0BEh, 080h, 084h, 08Dh, 074h, 010h, 0BFh, 090h, 086h, 08Dh, 07Dh, 010h
        db  0B9h, 000h, 001h, 08Bh, 01Eh, 0FAh, 088h, 08Bh, 02Eh, 0F8h, 088h, 08Bh, 016h, 0F4h, 088h, 08Bh
        db  0C3h, 023h, 006h, 0B6h, 088h, 089h, 004h, 089h, 02Dh, 083h, 0C3h, 010h, 083h, 0C5h, 008h, 04Ah
        db  075h, 00Ch, 08Bh, 01Eh, 0FAh, 088h, 08Bh, 02Eh, 0F8h, 088h, 08Bh, 016h, 0F4h, 088h, 083h, 0C6h
        db  002h, 083h, 0C7h, 002h, 0E2h, 0D9h, 0BEh, 080h, 084h, 0A1h, 0F4h, 088h, 088h, 004h, 0C3h, 053h
        db  051h, 0B9h, 010h, 000h, 033h, 0DBh, 0D1h, 0D3h, 0D1h, 0E8h, 0E0h, 0FAh, 0F7h, 0D9h, 083h, 0C1h
        db  010h, 040h, 0D3h, 0E0h, 085h, 0DBh, 075h, 002h, 0D1h, 0E8h, 059h, 05Bh, 0C3h, 052h, 08Bh, 0D0h
        db  08Bh, 0C7h, 0F7h, 036h, 0BEh, 088h, 053h, 0BBh, 000h, 089h, 0D7h, 05Bh, 05Ah, 0C3h, 0F6h, 046h
        db  001h, 008h, 075h, 009h, 001h, 044h, 060h, 011h, 054h, 062h, 0EBh, 065h, 090h, 001h, 044h, 05Ch
        db  011h, 054h, 05Eh, 0EBh, 05Ch, 090h, 011h, 054h, 046h, 0EBh, 048h, 090h, 011h, 054h, 04Ah, 0EBh
        db  04Ah, 090h, 011h, 054h, 052h, 0EBh, 012h, 090h, 011h, 054h, 04Eh, 0EBh, 01Ch, 090h, 0F6h, 047h
        db  00Bh, 020h, 075h, 010h, 001h, 044h, 050h, 072h, 0E9h, 001h, 04Ch, 054h, 073h, 033h, 011h, 054h
        db  056h, 0EBh, 02Eh, 090h, 001h, 044h, 04Ch, 072h, 0DFh, 001h, 04Ch, 058h, 073h, 023h, 011h, 054h
        db  05Ah, 0EBh, 01Eh, 090h, 033h, 0C0h, 099h, 040h, 0F6h, 046h, 001h, 020h, 074h, 0A0h, 001h, 044h
        db  044h, 072h, 0B3h, 08Bh, 04Fh, 008h, 001h, 04Ch, 048h, 072h, 0B1h, 0F6h, 047h, 00Bh, 022h, 075h
        db  0BDh, 08Bh, 04Eh, 000h, 091h, 080h, 0E4h, 0FBh, 03Dh, 000h, 0A0h, 075h, 001h, 0C3h, 0BEh, 090h
        db  000h, 0A8h, 080h, 074h, 00Eh, 001h, 04Ch, 014h, 011h, 054h, 016h, 024h, 07Fh, 03Dh, 000h, 0A0h
        db  075h, 001h, 0C3h, 0A8h, 040h, 074h, 012h, 0A8h, 02Fh, 075h, 00Eh, 001h, 04Ch, 02Ch, 011h, 054h
        db  02Eh, 024h, 0BFh, 03Dh, 000h, 0A0h, 075h, 001h, 0C3h, 024h, 02Fh, 074h, 035h, 0A8h, 020h, 074h
        db  00Fh, 083h, 04Ch, 038h, 001h, 001h, 04Ch, 018h, 011h, 054h, 01Ah, 0A0h, 024h, 089h, 0EBh, 018h
        db  001h, 04Ch, 010h, 011h, 054h, 012h, 03Ch, 001h, 075h, 008h, 001h, 04Ch, 024h, 011h, 054h, 026h
        db  0EBh, 006h, 001h, 04Ch, 028h, 011h, 054h, 02Ah, 08Ah, 0C8h, 001h, 04Ch, 01Ch, 011h, 054h, 01Eh
        db  0B1h, 001h, 08Ah, 0C4h, 0A8h, 001h, 074h, 006h, 001h, 04Ch, 03Ch, 011h, 054h, 03Eh, 0C3h, 033h
        db  0C0h, 099h, 040h, 0BEh, 010h, 000h, 001h, 044h, 014h, 011h, 054h, 016h, 08Bh, 04Fh, 00Ch, 001h
        db  04Ch, 01Ch, 011h, 054h, 01Eh, 03Bh, 00Eh, 022h, 089h, 07Ch, 00Dh, 03Bh, 00Eh, 020h, 089h, 07Fh
        db  01Ah, 0F6h, 047h, 00Eh, 012h, 075h, 021h, 0C3h, 001h, 044h, 034h, 011h, 054h, 036h, 001h, 044h
        db  02Ch, 011h, 054h, 02Eh, 0F6h, 047h, 00Eh, 012h, 075h, 00Eh, 0C3h, 001h, 044h, 030h, 011h, 054h
        db  032h, 083h, 00Eh, 098h, 000h, 004h, 0EBh, 0E6h, 0F6h, 047h, 00Eh, 010h, 075h, 00Dh, 001h, 044h
        db  028h, 011h, 054h, 02Ah, 001h, 04Ch, 03Ch, 011h, 054h, 03Eh, 0C3h, 001h, 044h, 024h, 011h, 054h
        db  026h, 001h, 04Ch, 038h, 011h, 054h, 03Ah, 0C3h, 000h, 000h, 000h, 000h, 010h, 020h, 030h, 040h
        db  000h, 050h, 000h, 010h, 000h, 000h, 060h, 000h, 000h, 000h
loc_1_5564:
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
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_55E0:
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
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_563C:
        push   ebp
        inc    edx
        and    byte ptr [ebp + 74h], al
        push   656e7265h
        je     SHORT loc_1_5668
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5654:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
loc_1_5668:
        add    byte ptr [eax + 0ch], al
        add    byte ptr [eax], al
        or     byte ptr [eax + eax], 0
        ror    byte ptr [eax + eax], 0
        add    byte ptr [0d400000h], cl
        add    byte ptr [eax], al
        or     byte ptr [0dc00000h], 0
        add    byte ptr [eax + 400000e2h], al
        or     al, 0
        add    byte ptr [eax - 3ffffff4h], al
        or     al, 0
        add    byte ptr [eax], al
        or     eax, 0d400000h
        add    byte ptr [eax], al
        or     byte ptr [0dc00000h], 0
loc_1_56A4:
        push   eax
        add    edx, dword ptr [eax + 3]
        pop    eax
        add    ebx, dword ptr [eax + 3]
        pushad
        add    esp, dword ptr [eax + 3]
        push   68036803h
        add    ebp, dword ptr [eax + 3]
        push   68036803h
        add    ebp, dword ptr [eax + 3]
        push   50036803h
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 3], bl
        add    byte ptr [eax], al
        pushad
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 3], ch
        add    byte ptr [eax], al
loc_1_56D4:
        add    al, byte ptr [ebx]
        add    al, 5
        pop    es
        or     dword ptr [eax + eax], ecx
        dec    ecx
        dec    edi
        pop    edi
        push   eax
        outsd
        db  072h, 074h
        add    byte ptr [ecx + 4fh], cl
        pop    edi
        push   eax
        outsd
        jb     SHORT loc_1_575F
        add    byte ptr [ecx + 52h], cl
        push   ecx
        pop    edi
        dec    esp
        db  065h, 076h, 065h
        insb
        add    byte ptr [eax], al
        db  000h
loc_1_56F8:
        dec    ecx
        push   edx
        push   ecx
        pop    edi
        dec    esp
        db  065h, 076h, 065h
        insb
        add    byte ptr [eax], al
        add    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 054h
        jns    SHORT loc_1_577E
        add    byte ptr gs:[ecx + 64h], al
        popad
        jo     SHORT loc_1_5789
        db  065h, 072h, 054h
        db  079h, 070h
        add    byte ptr gs:[ebp + 61h], cl
        js     SHORT loc_1_576D
        jne    SHORT loc_1_578E
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_5729
loc_1_5729:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], cl
        db  078h, 04Dh
        jne    SHORT loc_1_579E
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_5739
loc_1_5739:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        db  072h, 079h
        push   edi
        imul   ebp, dword ptr [esi + 64h], 776fh
        add    byte ptr [eax], al
loc_1_574C:
        dec    ebp
        insd
        outsd
        db  072h, 079h
        push   edi
        imul   ebp, dword ptr [esi + 64h], 776fh
        add    byte ptr [eax], al
        push   ebx
        insb
        outsd
loc_1_575F:
        je     SHORT loc_1_57AF
        jne    SHORT loc_1_57D0
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        push   ebx
        insb
        outsd
        db  074h, 04Eh
loc_1_576D:
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
        db  076h, 065h
        inc    edx
        db  075h, 066h
loc_1_577E:
        push   ebx
        imul   edi, dword ptr [edx + 65h], 65520000h
        arpl   word ptr [ebp + 69h], sp
loc_1_5789:
        db  076h, 065h
        inc    edx
        db  075h, 066h
loc_1_578E:
        push   ebx
        imul   edi, dword ptr [edx + 65h], 614d0000h
        js     SHORT loc_1_5801
        insd
        jne    SHORT loc_1_5808
        dec    edi
        jo     SHORT loc_1_5803
loc_1_579E:
        outsb
        jae    SHORT loc_1_57A1
loc_1_57A1:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], cl
        js     SHORT loc_1_5811
        insd
        jne    SHORT loc_1_5818
        dec    edi
        db  070h, 065h
        outsb
loc_1_57AF:
        jae    SHORT loc_1_57B1
loc_1_57B1:
        add    byte ptr [eax], al
        add    byte ptr [ecx + ebp*2 + 61h], al
        outsb
        outsd
        jae    SHORT loc_1_5830
        imul   esp, dword ptr [ebx + 73h], 61694400h
        outsb
        outsd
        db  073h, 074h
        imul   esp, dword ptr [ebx + 73h], 4e435000h
        dec    ecx
loc_1_57D0:
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    esi
        dec    ecx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [esi + 49h], cl
        push   ebp
        db  070h, 063h
        add    byte ptr [eax], al
        add    byte ptr [esi + 49h], cl
        push   ebp
        db  070h, 063h
        add    byte ptr [eax], al
        add    byte ptr [esi + 49h], cl
        push   ebp
        db  070h, 073h
        add    byte ptr [eax], al
        add    byte ptr [esi + 49h], cl
        push   ebp
        db  070h, 073h
        add    byte ptr [eax], al
        add    byte ptr [esi + 49h], cl
        push   ebp
        db  070h, 063h
loc_1_5801:
        das
        inc    ebp
loc_1_5803:
        dec    edi
        push   esp
        push   eax
        add    byte ptr [eax], al
loc_1_5808:
        dec    esi
        dec    ecx
        push   ebp
        db  070h, 063h
        das
        inc    ebp
        dec    edi
        push   esp
loc_1_5811:
        push   eax
        add    byte ptr [eax], al
        push   eax
        inc    ebx
        dec    esi
        dec    ecx
loc_1_5818:
        push   ebp
        js     NEAR PTR loc_1_581C
loc_1_581C:
        push   eax
        inc    ebx
        dec    esi
        dec    ecx
        push   ebp
        js     NEAR PTR loc_1_5824
loc_1_5824:
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        db  073h, 054h
loc_1_5830:
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_586D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5873:
        add    byte ptr [eax], al
loc_1_5875:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58AF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        adc    al, byte ptr [esi]
        push   ss
        or     bl, byte ptr [edx]
        push   cs
        push   ds
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax + 30002000h], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], dh
        add    byte ptr [eax], dh
        clc
        inc    byte ptr [eax]
        add    dword ptr [eax + 28000h], eax
loc_1_58F3:
        add    al, byte ptr [edi]
        push   es
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 2], dl
        push   es
        or     cl, byte ptr [esi]
        adc    dl, byte ptr [esi]
        sbb    bl, byte ptr [esi]
        and    ah, byte ptr [esi]
        sub    ch, byte ptr [esi]
        xor    dh, byte ptr [esi]
        cmp    bh, byte ptr [esi]
        inc    edx
        inc    esi
        dec    edx
        dec    esi
        push   edx
        push   esi
        pop    edx
        pop    esi
        bound  esp, qword ptr [esi + 6ah]
        outsb
        db  072h, 076h
        db  07Ah, 07Eh
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        xor    byte ptr [eax], al
        xor    byte ptr [eax], al
        jo     SHORT loc_1_592D
loc_1_592D:
        jo     SHORT loc_1_58AF
        inc    dword ptr [eax]
        xor    byte ptr [eax + 28000h], al
        add    al, byte ptr [edx]
        add    eax, dword ptr [eax + 100601h]
        add    byte ptr [ecx], al
        add    ah, byte ptr [eax + 0a060259h]
        push   cs
        adc    dl, byte ptr [esi]
        sbb    bl, byte ptr [esi]
        and    ah, byte ptr [esi]
        sub    ch, byte ptr [esi]
        xor    dh, byte ptr [esi]
        cmp    bh, byte ptr [esi]
        inc    edx
        inc    esi
        dec    edx
        dec    esi
        push   edx
        push   esi
        pop    edx
        pop    esi
        bound  esp, qword ptr [esi + 6ah]
        outsb
        jb     SHORT loc_1_59D8
        jp     SHORT loc_1_59E2
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        xor    byte ptr [eax], al
        xor    byte ptr [eax], al
        jo     SHORT loc_1_5971
loc_1_5971:
        jo     SHORT loc_1_58F3
        inc    dword ptr [eax]
        xor    byte ptr [eax + 28000h], al
        add    al, byte ptr [edx]
        add    eax, dword ptr [eax + 100601h]
        add    byte ptr [ecx], al
        add    ah, byte ptr [eax + 47h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 42h], dl
        dec    esi
        inc    ebp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 42h], dl
        dec    esi
        inc    ebp
loc_1_59D8:
        dec    ecx
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_59E2:
        add    byte ptr [eax], al
loc_1_59E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59E8:
        add    byte ptr [eax], al
loc_1_59EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59F2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59F6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59FA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59FE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A02:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A06:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A0A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A0E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A12:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A16:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A1E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A22:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
