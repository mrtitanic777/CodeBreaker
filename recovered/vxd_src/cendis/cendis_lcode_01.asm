; cendis_lcode_01.asm -- CENDIS object 1 segment _LTEXT
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
        call   sub_1_1A00
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
CENDIS_Control:
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
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [ecx]
        test   al, 1
        je     SHORT loc_1_0120
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_0115
        cmp    al, 0ffh
        jne    SHORT loc_1_0115
        cmp    byte ptr [ecx + 1], 0ffh
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
        mov    ecx, dword ptr [ebp + 10h]
        lea    eax, [esi + 2ch]
        mov    ecx, dword ptr [ecx + 2]
        cmp    dword ptr [eax + 2], ecx
        jne    SHORT loc_1_0149
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_0150
loc_1_0149:
        mov    dword ptr [ebp - 10h], 1
loc_1_0150:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0160
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_018A
        db  090h
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
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
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
sub_1_0330:
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
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_03B4:
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
sub_1_03F1:
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
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_048C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
sub_1_049E:
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
sub_1_04D4:
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 045h, 04Eh, 044h
        db  049h, 053h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 40h]
        lea    ecx, [eax + 3f8h]
        cmp    ecx, eax
        jbe    SHORT loc_1_06D3
        mov    bl, byte ptr [ebp + 0ch]
loc_1_06B7:
        mov    dl, byte ptr [eax]
        cmp    dl, bl
        je     SHORT loc_1_06D3
        or     dl, dl
        jne    SHORT loc_1_06C6
        add    eax, 4
        jmp    SHORT loc_1_06CF
loc_1_06C6:
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        lea    eax, [eax + edx*2 + 4]
loc_1_06CF:
        cmp    ecx, eax
        ja     SHORT loc_1_06B7
loc_1_06D3:
        cmp    byte ptr [eax], 0ffh
        jne    SHORT loc_1_06DA
        sub    eax, eax
loc_1_06DA:
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_06E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0BBh, 0C8h, 035h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        db  0BFh, 0D0h, 035h, 000h, 000h
        mov    edx, dword ptr [esi + 40h]
        cmp    dword ptr [esi + 28h], 2
        je     SHORT loc_1_0705
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0705:
        add    edx, 14h
        db  080h, 03Dh, 0C8h, 035h, 000h, 000h, 000h
        je     SHORT loc_1_0722
loc_1_0711:
        mov    eax, ebx
        inc    ebx
        mov    cl, byte ptr [edx]
        cmp    byte ptr [eax], cl
        jne    SHORT loc_1_0757
        add    edx, 2
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_1_0711
loc_1_0722:
        push   15h
        push   esi
        call   sub_1_06A0
        add    eax, 2ch
        mov    dword ptr [esi + 2ch], 1
        db  080h, 03Dh, 0D0h, 035h, 000h, 000h, 000h
        je     SHORT loc_1_0767
loc_1_073D:
        mov    edx, edi
        inc    edi
        mov    cl, byte ptr [eax]
        cmp    byte ptr [edx], cl
        jne    SHORT loc_1_0760
        add    eax, 2
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_073D
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0757:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0760:
        mov    dword ptr [esi + 2ch], 0
loc_1_0767:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_0770:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 40h]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_0790
        mov    byte ptr [edi + 800h], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0790:
        mov    byte ptr [edi + 806h], 0d4h
        push   esi
        call   sub_1_0870
        or     al, al
        jne    SHORT loc_1_07B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07B0:
        push   30d40h
        call   sub_1_0500
        mov    eax, dword ptr [esi + 4dh]
        or     eax, eax
        jne    SHORT loc_1_07D8
        test   byte ptr [edi + 806h], 1
        jne    SHORT loc_1_07D0
        jmp    SHORT loc_1_07E5
        db  08Dh, 064h, 024h, 000h
loc_1_07D0:
        add    esi, 4ch
        mov    byte ptr [esi], 1
        jmp    SHORT loc_1_07EB
loc_1_07D8:
        cmp    eax, 2
        jne    SHORT loc_1_07E5
        add    esi, 4ch
        mov    byte ptr [esi], 1
        jmp    SHORT loc_1_07EB
loc_1_07E5:
        add    esi, 4ch
        mov    byte ptr [esi], 0
loc_1_07EB:
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_07F7
        mov    byte ptr [edi + 806h], 55h
loc_1_07F7:
        push   30d40h
        call   sub_1_0500
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0810:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 28h]
        cmp    eax, 1
        jne    SHORT loc_1_083B
        sub    ecx, ecx
loc_1_0821:
        mov    edx, dword ptr [esi + 34h]
        add    dx, cx
        add    dx, 1ah
        in     al, dx
        mov    byte ptr [esi + ecx + 5ch], al
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_0821
        pop    esi
        pop    ebp
        ret    4
loc_1_083B:
        cmp    eax, 2
        jne    SHORT loc_1_085E
        push   22h
        push   esi
        call   sub_1_06A0
        sub    ecx, ecx
        lea    ebx, [ebx]
loc_1_0850:
        mov    dl, byte ptr [eax + ecx*2 + 0ah]
        mov    byte ptr [esi + ecx + 5ch], dl
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_0850
loc_1_085E:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 2ch], 0
        je     SHORT loc_1_088C
        mov    edx, dword ptr [ecx + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_088C:
        mov    byte ptr [ecx + 51h], 52h
        cmp    dword ptr [ecx + 28h], 2
        jne    SHORT loc_1_08A4
        cmp    byte ptr [ecx + 4ch], 0
        je     SHORT loc_1_08A4
        lea    edi, [ecx + 52h]
        mov    byte ptr [edi], 20h
        jmp    SHORT loc_1_08AA
loc_1_08A4:
        lea    edi, [ecx + 52h]
        mov    byte ptr [edi], 0a0h
loc_1_08AA:
        mov    si, word ptr [ecx + 34h]
        mov    al, byte ptr [ecx + 51h]
        lea    edx, [esi + 6]
        or     al, 80h
        out    dx, al
        lea    edx, [esi + 7]
        mov    al, byte ptr [edi]
        out    dx, al
        lea    edx, [esi + 2]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 3]
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [ecx + 34h]
        out    dx, al
        inc    dx
        mov    si, word ptr [ecx + 34h]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 4]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, 4
        out    dx, al
        sub    bl, bl
loc_1_08E9:
        mov    al, bl
        and    eax, 0ffff00ffh
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0ffffh
        mov    al, byte ptr [ecx + eax + 62h]
        out    dx, al
        inc    bl
        cmp    bl, 6
        jb     SHORT loc_1_08E9
        mov    al, byte ptr [edi]
        or     al, 4
        lea    edx, [esi + 7]
        out    dx, al
        mov    eax, dword ptr [ecx + 8ah]
        test   al, 20h
        jne    SHORT loc_1_093B
        test   al, 4
        je     SHORT loc_1_093B
        sub    bl, bl
loc_1_0920:
        mov    dl, bl
        and    edx, 0ffff00ffh
        add    dx, si
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0920
        jmp    SHORT loc_1_095F
loc_1_093B:
        sub    bl, bl
        lea    ecx, [ecx]
loc_1_0940:
        mov    al, bl
        and    eax, 0ffff00ffh
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0ffffh
        mov    al, byte ptr [ecx + eax + 54h]
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0940
loc_1_095F:
        mov    al, byte ptr [edi]
        or     al, 8
        lea    edx, [esi + 7]
        out    dx, al
        lea    edx, [esi + 0bh]
        mov    al, 7
        out    dx, al
        lea    edx, [esi + 0ch]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [ecx + 51h]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_09AA
        mov    edx, dword ptr [esi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_09AA:
        mov    edx, dword ptr [esi + 34h]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0ffh
        je     SHORT loc_1_09BD
        or     al, al
        jne    SHORT loc_1_09F2
loc_1_09BD:
        mov    edx, dword ptr [esi + 34h]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        not    al
        out    dx, al
        in     al, dx
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1]
        not    ecx
        and    eax, 0ffh
        cmp    ecx, eax
        jne    SHORT loc_1_09F0
        mov    edx, dword ptr [esi + 34h]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_09F0:
        sub    al, al
loc_1_09F2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 2ch], 0
        je     SHORT loc_1_0A17
        mov    edx, dword ptr [ecx + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0A17:
        mov    si, word ptr [ecx + 34h]
        mov    al, 2
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [ecx + 53h]
        or     al, 4
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 80h
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 81h
        out    dx, al
        cmp    dword ptr [ecx + 28h], 1
        jne    SHORT loc_1_0A42
        lea    edx, [esi + 10h]
        mov    al, 1dh
        out    dx, al
loc_1_0A42:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A50:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 2ch], 0
        je     SHORT loc_1_0A66
        mov    edx, dword ptr [ecx + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0A66:
        mov    cx, word ptr [ecx + 34h]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 28h], 1
        jne    SHORT loc_1_0A9C
        mov    edx, dword ptr [ebx + 34h]
        add    dx, 10h
        mov    al, 0fh
        out    dx, al
loc_1_0A9C:
        cmp    dword ptr [ebx + 2ch], 0
        je     SHORT loc_1_0AAC
        mov    edx, dword ptr [ebx + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0AAC:
        mov    edx, dword ptr [ebx + 34h]
        add    dx, 5
        mov    al, 6
        out    dx, al
        db  0BEh, 0C0h, 035h, 000h, 000h
        add    eax, 0
loc_1_0AC0:
        mov    ecx, dword ptr [ebx + 34h]
        mov    ax, 3dh
        lea    edx, [ecx + 8]
        out    dx, ax
        push   1fh
        mov    eax, dword ptr [esi]
        push   eax
        add    ecx, 8
        push   ecx
        call   sub_1_03B4
        add    esi, 4
        db  081h, 0FEh, 0C8h, 035h, 000h, 000h
        jb     SHORT loc_1_0AC0
        mov    edx, dword ptr [ebx + 34h]
        add    dx, 0ah
        mov    al, 82h
        out    dx, al
        push   0fah
        call   sub_1_0500
        mov    dword ptr [ebp - 10h], 0
        db  0C7h, 045h, 0F8h, 0C0h, 035h, 000h, 000h
loc_1_0B07:
        mov    edx, dword ptr [ebx + 34h]
        add    dx, 5
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0B65
        push   2
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebx + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        cmp    word ptr [ebp - 0ah], 3dh
        jne    SHORT loc_1_0B65
        push   1fh
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebx + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        lea    edi, [ebp - 50h]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 0fh
        mov    esi, dword ptr [eax]
        repe cmpsd
        jne    SHORT loc_1_0B65
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_1_0B65
        add    dword ptr [ebp - 8], 4
        inc    dword ptr [ebp - 10h]
        db  081h, 07Dh, 0F8h, 0C8h, 035h, 000h, 000h
        jb     SHORT loc_1_0B07
loc_1_0B65:
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 2
        cmp    eax, 1
        sbb    cl, cl
        neg    cl
        mov    dx, word ptr [ebx + 34h]
        in     al, dx
        mov    dx, word ptr [ebx + 34h]
        out    dx, al
        inc    dx
        cmp    dword ptr [ebx + 28h], 1
        mov    si, word ptr [ebx + 34h]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, 4
        out    dx, al
        jne    SHORT loc_1_0B99
        lea    edx, [esi + 10h]
        mov    al, 0dh
        out    dx, al
loc_1_0B99:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0A50
        push   esi
        call   sub_1_0870
        push   esi
        call   sub_1_0A00
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
sub_1_0BD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_0C1B
loc_1_0BE2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0BF0:
        mov    ecx, eax
        and    ecx, 80000000h
        cmp    ecx, 1
        sbb    edi, edi
        inc    edi
        mov    cl, bl
        and    ecx, 1
        xor    edi, ecx
        add    eax, eax
        shr    bl, 1
        or     edi, edi
        je     SHORT loc_1_0C12
        xor    eax, 4c11db7h
loc_1_0C12:
        dec    esi
        jne    SHORT loc_1_0BF0
        inc    edx
        cmp    edx, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_0BE2
loc_1_0C1B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C30:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0BD0
        sub    ecx, ecx
        mov    edx, 1
loc_1_0C45:
        add    ecx, ecx
        test   eax, edx
        je     SHORT loc_1_0C4E
        or     ecx, 1
loc_1_0C4E:
        add    edx, edx
        cmp    edx, 40h
        jb     SHORT loc_1_0C45
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 0ch]
        shr    edx, 3
        mov    byte ptr [eax], dl
        mov    dl, 1
        and    cl, 7
        mov    eax, dword ptr [ebp + 10h]
        shl    dl, cl
        mov    byte ptr [eax], dl
        pop    ebp
        ret    0ch
        db  090h
sub_1_0C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
loc_1_0C89:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 54h], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_1_0C89
        db  068h, 0E0h, 034h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0B0h, 0FAh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_1_0CE7
        sub    edi, edi
loc_1_0CB8:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 0E0h, 034h, 000h, 000h
        push   eax
        call   sub_1_0C30
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 54h], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_1_0CB8
loc_1_0CE7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_0D09
        mov    edx, dword ptr [edi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0D09:
        mov    si, word ptr [edi + 34h]
        mov    al, byte ptr [edi + 51h]
        or     al, 80h
        lea    edx, [esi + 6]
        out    dx, al
        lea    ecx, [esi + 7]
        mov    al, byte ptr [edi + 52h]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0D24:
        lea    edx, [ebx + esi]
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_0D24
        mov    al, byte ptr [edi + 52h]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [edi + 51h]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0D50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_0D69
        mov    edx, dword ptr [edi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0D69:
        mov    si, word ptr [edi + 34h]
        mov    al, byte ptr [edi + 51h]
        or     al, 80h
        lea    edx, [esi + 6]
        out    dx, al
        lea    ecx, [esi + 7]
        mov    al, byte ptr [edi + 52h]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0D84:
        lea    edx, [esi + ebx]
        add    dx, 8
        mov    al, byte ptr [edi + ebx + 54h]
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_0D84
        mov    al, byte ptr [edi + 52h]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [edi + 51h]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0DB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 34h]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 6d5h], 0
        jne    NEAR PTR loc_1_0F25
        mov    byte ptr [esi + 6d5h], 1
        inc    dword ptr [esi + 70h]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_0E03
        mov    edx, dword ptr [esi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0E03:
        sub    edi, edi
        sub    ebx, ebx
loc_1_0E07:
        mov    dx, word ptr [esi + 34h]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 34h]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, bl
        jne    SHORT loc_1_0E21
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_0E97
loc_1_0E21:
        test   byte ptr [ebp - 2], 81h
        je     SHORT loc_1_0E3F
        test   byte ptr [ebp - 2], 1
        je     SHORT loc_1_0E33
        inc    dword ptr [esi + 6ach]
loc_1_0E33:
        push   esi
        call   sub_1_0F30
        push   esi
        call   sub_1_17E0
loc_1_0E3F:
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_0E4D
        cmp    byte ptr [esi + 88h], bl
        je     SHORT loc_1_0E8D
loc_1_0E4D:
        mov    dx, word ptr [esi + 34h]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dword ptr [esi + 74h], ebx
        cmp    byte ptr [esi + 88h], bl
        je     SHORT loc_1_0E70
        mov    byte ptr [esi + 89h], 2
        mov    byte ptr [esi + 88h], bl
        jmp    SHORT loc_1_0E79
        db  090h
loc_1_0E70:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 89h], al
loc_1_0E79:
        mov    byte ptr [esi + 6fah], bl
        cmp    dword ptr [esi + 84h], ebx
        je     SHORT loc_1_0E8D
        push   esi
        call   sub_1_1160
loc_1_0E8D:
        inc    edi
        cmp    edi, 8
        jl     NEAR PTR loc_1_0E07
loc_1_0E97:
        cmp    dword ptr [esi + 6bdh], 0
        je     SHORT loc_1_0EB8
loc_1_0EA0:
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_0EB8
        push   esi
        call   sub_1_0F30
        cmp    dword ptr [esi + 6bdh], 0
        jne    SHORT loc_1_0EA0
loc_1_0EB8:
        mov    cx, word ptr [esi + 34h]
        mov    byte ptr [esi + 6d5h], 0
        lea    edx, [ecx + 2]
        mov    al, 80h
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 81h
        out    dx, al
        mov    eax, dword ptr [esi + 70h]
        dec    eax
        mov    dword ptr [esi + 70h], eax
        jne    SHORT loc_1_0F07
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_0EF0
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_0EF0
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_0F07
loc_1_0EF0:
        push   1
        add    esi, 6d6h
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0F07:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_0F25
        cmp    dword ptr [esi + 84h], 0
        jne    SHORT loc_1_0F25
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_0F25
        push   esi
        call   sub_1_17E0
loc_1_0F25:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_0F30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 3], 1
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_0F54
        mov    edx, dword ptr [esi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_0F54:
        cmp    byte ptr [esi + 6b0h], 0
        jne    NEAR PTR loc_1_10D4
        mov    cx, word ptr [esi + 34h]
        mov    al, 81h
        lea    edx, [ecx + 1]
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        test   al, 40h
        jne    NEAR PTR loc_1_10D4
        inc    dword ptr [esi + 74h]
        mov    dword ptr [esi + 6c5h], 0
        push   2
        lea    eax, [esi + 690h]
        push   eax
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    ax, word ptr [esi + 692h]
        mov    word ptr [esi + 8eh], ax
        cmp    ax, 6
        jb     NEAR PTR loc_1_106A
        mov    edi, dword ptr [esi + 6b9h]
        add    di, 0eh
        cmp    di, ax
        jbe    SHORT loc_1_0FC3
        mov    di, ax
loc_1_0FC3:
        mov    bl, 1
        test   byte ptr [esi + 8ah], 28h
        jne    SHORT loc_1_1040
        push   3
        lea    eax, [esi + 90h]
        push   eax
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 8eh], 6
        sub    di, 6
        cmp    dword ptr [esi + 92h], -1
        jne    SHORT loc_1_1010
        cmp    byte ptr [esi + 90h], 0ffh
        jne    SHORT loc_1_1010
        cmp    byte ptr [esi + 91h], 0ffh
        jne    SHORT loc_1_1010
        jmp    SHORT loc_1_106A
        db  08Dh, 064h, 024h, 000h
loc_1_1010:
        mov    eax, edi
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 96h]
        push   eax
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 8eh], di
        jmp    SHORT loc_1_106C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1040:
        mov    eax, edi
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 90h]
        push   eax
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 8eh], di
        jmp    SHORT loc_1_106C
loc_1_106A:
        sub    bl, bl
loc_1_106C:
        or     bl, bl
        je     SHORT loc_1_1085
        push   esi
        call   sub_1_1410
        cmp    eax, 3
        je     SHORT loc_1_1085
        inc    dword ptr [esi + 69ch]
        mov    byte ptr [ebp - 2], 1
loc_1_1085:
        mov    ax, word ptr [esi + 8eh]
        or     ax, ax
        je     SHORT loc_1_10BF
        cmp    ax, 8
        ja     SHORT loc_1_10B5
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        jmp    SHORT loc_1_10BF
loc_1_10B5:
        mov    edx, dword ptr [esi + 34h]
        add    dx, 0eh
        mov    al, 4
        out    dx, al
loc_1_10BF:
        cmp    dword ptr [esi + 74h], 5
        jbe    NEAR PTR loc_1_0F54
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 74h], 0
loc_1_10D4:
        cmp    dword ptr [esi + 6bdh], 0
        je     SHORT loc_1_1122
        lea    ecx, [ecx]
loc_1_10E0:
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_112B
        mov    edi, dword ptr [esi + 6bdh]
        push   edi
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 6bdh], eax
        mov    dword ptr [esi + 6c5h], edi
        mov    ebx, dword ptr [edi + 28h]
        push   esi
        call   sub_1_1270
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 6bdh], 0
        jne    SHORT loc_1_10E0
loc_1_1122:
        cmp    byte ptr [esi + 6b0h], 0
        je     SHORT loc_1_1137
loc_1_112B:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1137:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_1146
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_0290
loc_1_1146:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 84h]
        or     ecx, ecx
        jne    SHORT loc_1_1194
        push   2
        push   6
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 14h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1194:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_11A4
        mov    edx, dword ptr [esi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_11A4:
        mov    al, byte ptr [esi + 89h]
        test   al, 80h
        je     SHORT loc_1_11E3
        test   al, 4
        je     SHORT loc_1_11DB
        mov    edx, dword ptr [esi + 34h]
        add    dx, 4
        in     al, dx
        and    eax, 0ffh
        and    eax, 0fffffff0h
        cmp    eax, 10h
        jne    SHORT loc_1_11D5
        inc    dword ptr [esi + 6a4h]
        inc    dword ptr [esi + 698h]
        jmp    SHORT loc_1_11E9
loc_1_11D5:
        inc    dword ptr [esi + 6a8h]
loc_1_11DB:
        inc    dword ptr [esi + 698h]
        jmp    SHORT loc_1_11E9
loc_1_11E3:
        inc    dword ptr [esi + 6a0h]
loc_1_11E9:
        cmp    byte ptr [ecx + 2ch], 0
        jne    SHORT loc_1_1212
        mov    al, byte ptr [esi + 89h]
        and    al, 80h
        cmp    al, 1
        mov    edi, dword ptr [ecx + 28h]
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        jmp    SHORT loc_1_1239
loc_1_1212:
        cmp    dword ptr [esi + 6bdh], 0
        jne    SHORT loc_1_1223
        mov    dword ptr [esi + 6bdh], ecx
        jmp    SHORT loc_1_122C
loc_1_1223:
        mov    eax, dword ptr [esi + 6c1h]
        mov    dword ptr [eax + 24h], ecx
loc_1_122C:
        mov    dword ptr [esi + 6c1h], ecx
        mov    dword ptr [ecx + 24h], 0
loc_1_1239:
        mov    dword ptr [esi + 84h], 0
        cmp    byte ptr [esi + 6b0h], 0
        je     SHORT loc_1_1260
        push   2
        push   esi
        call   sub_1_30D0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1260:
        push   esi
        call   sub_1_17E0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1270:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], 6
        jae    SHORT loc_1_12A0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12A0:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 6b9h]
        add    edi, 0eh
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_12B4
        mov    edi, dword ptr [ebp - 4]
loc_1_12B4:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 90h]
        push   eax
        call   sub_1_1330
        cmp    edi, 0eh
        jae    SHORT loc_1_12F0
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 90h]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_12F0:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        sub    edi, 0eh
        push   edi
        lea    eax, [esi + 9eh]
        push   eax
        push   0eh
        lea    eax, [esi + 90h]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1330:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 10h]
        je     SHORT loc_1_1384
loc_1_1355:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    eax, ebx
        ja     SHORT loc_1_1384
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1355
loc_1_1384:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1395
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1395:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0ch], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_13A4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_13B3
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_13B6
loc_1_13B3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_13B6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05EC
        add    esi, ebx
        cmp    esi, edi
        je     SHORT loc_1_13F2
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_13FD
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        jmp    SHORT loc_1_13A4
loc_1_13F2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_13FD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1410:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 692h]
        cmp    eax, 5eah
        jbe    SHORT loc_1_1430
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_1430:
        mov    dword ptr [edx + 694h], eax
        mov    ecx, dword ptr [edx + 6b9h]
        add    ecx, 0eh
        cmp    ecx, eax
        jbe    SHORT loc_1_1445
        mov    ecx, eax
loc_1_1445:
        cmp    ecx, 0eh
        jae    SHORT loc_1_1469
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 90h]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 24h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1469:
        sub    eax, 0eh
        push   eax
        sub    ecx, 0eh
        push   ecx
        lea    eax, [edx + 9eh]
        push   eax
        push   0eh
        lea    eax, [edx + 90h]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 24h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_14A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        add    esi, 0eh
        cmp    dword ptr [edi + 6c5h], 0
        je     NEAR PTR loc_1_1547
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, esi
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_151A
loc_1_14DA:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        mov    eax, dword ptr [edi + 6c5h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1330
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_151A
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_14DA
loc_1_151A:
        cmp    esi, ebx
        jbe    SHORT loc_1_1535
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1535:
        mov    ecx, dword ptr [ebp + 1ch]
        sub    ebx, esi
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1547:
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_1557
        mov    edx, dword ptr [edi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_1557:
        mov    ax, word ptr [edi + 8eh]
        or     ax, ax
        je     SHORT loc_1_1592
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        mov    eax, dword ptr [edi + 6b9h]
        add    eax, edi
        add    eax, 9eh
        push   eax
        mov    eax, dword ptr [edi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    word ptr [edi + 8eh], 0
loc_1_1592:
        mov    ecx, dword ptr [edi + 694h]
        mov    eax, dword ptr [ebp + 14h]
        lea    edx, [esi + eax]
        cmp    edx, ecx
        jbe    SHORT loc_1_15C7
        cmp    esi, ecx
        jbe    SHORT loc_1_15C0
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
loc_1_15C0:
        sub    ecx, esi
        mov    dword ptr [ebp - 18h], ecx
        jmp    SHORT loc_1_15CA
loc_1_15C7:
        mov    dword ptr [ebp - 18h], eax
loc_1_15CA:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_0530
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_164B
loc_1_15FA:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     SHORT loc_1_1607
        mov    ebx, dword ptr [ebp - 8]
loc_1_1607:
        push   ebx
        lea    eax, [esi + edi]
        add    eax, 90h
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05EC
        add    esi, ebx
        sub    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_164B
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_164B
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_15FA
loc_1_164B:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1670:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 24h
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_16A0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16A0:
        cmp    byte ptr [esi + 6b0h], 0
        je     SHORT loc_1_16B7
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16B7:
        inc    dword ptr [ebx + 18h]
        inc    dword ptr [esi + 70h]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], ebx
        lea    eax, [ebp - 0ch]
        push   0
        push   eax
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        call   sub_1_0530
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0554
        mov    ecx, dword ptr [esi + 64h]
        lea    edx, [esi + 62h]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    ecx, eax
        jae    SHORT loc_1_1703
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1737
loc_1_1703:
        jbe    SHORT loc_1_1710
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_1737
        db  08Bh, 0FFh
loc_1_1710:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    SHORT loc_1_1727
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1737
loc_1_1727:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_1_1737
        mov    dword ptr [ebp - 4], 0
loc_1_1737:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1757
        test   byte ptr [ebx + 1dh], 2
        je     SHORT loc_1_1745
        jmp    SHORT loc_1_1757
loc_1_1745:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_0330
        mov    byte ptr [edi + 8], al
        jmp    SHORT loc_1_175B
loc_1_1757:
        mov    byte ptr [edi + 8], 0
loc_1_175B:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_1770
        mov    eax, dword ptr [esi + 80h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_1776
        db  090h
loc_1_1770:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 7ch], ecx
loc_1_1776:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 80h], ecx
        mov    eax, dword ptr [esi + 70h]
        dec    eax
        mov    dword ptr [esi + 70h], eax
        jne    SHORT loc_1_17B0
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_17A0
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_17A0
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_17B0
loc_1_17A0:
        push   1
        add    esi, 6d6h
        push   esi
        call   sub_1_04E2
        jmp    SHORT loc_1_17CE
loc_1_17B0:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_17CE
        cmp    dword ptr [esi + 84h], 0
        jne    SHORT loc_1_17CE
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_17CE
        push   esi
        call   sub_1_17E0
loc_1_17CE:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_17FC
        mov    edx, dword ptr [esi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_17FC:
        cmp    dword ptr [esi + 84h], 0
        jne    NEAR PTR loc_1_198F
        mov    di, word ptr [ebp - 4]
        sub    ebx, ebx
        nop
loc_1_1810:
        mov    eax, dword ptr [esi + 7ch]
        cmp    eax, ebx
        je     NEAR PTR loc_1_198F
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esi + 7ch], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 84h], eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 14h], 3ch
        mov    eax, 3ch
        jb     SHORT loc_1_184E
        mov    eax, dword ptr [ebp - 14h]
loc_1_184E:
        mov    edx, dword ptr [esi + 34h]
        add    dx, 8
        cmp    dword ptr [ebp - 8], ebx
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        je     NEAR PTR loc_1_18F8
loc_1_1863:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0ch], ebx
        jne    SHORT loc_1_187B
        jmp    SHORT loc_1_18E2
loc_1_187B:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_18B0
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        inc    dword ptr [ebp - 10h]
        shl    ax, 8
        dec    dword ptr [ebp - 0ch]
        or     di, ax
        mov    edx, dword ptr [esi + 34h]
        add    dx, 8
        cmp    dword ptr [ebp - 0ch], ebx
        mov    ax, di
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        jne    SHORT loc_1_18B0
        jmp    SHORT loc_1_18E2
        db  08Dh, 049h, 000h
loc_1_18B0:
        cmp    dword ptr [ebp - 0ch], 1
        jbe    SHORT loc_1_18CC
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_18CC:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_18E2
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        movzx  di, byte ptr [eax + ecx - 1]
        mov    byte ptr [ebp - 1], 1
loc_1_18E2:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], ebx
        jne    NEAR PTR loc_1_1863
loc_1_18F8:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_1911
        or     di, 2000h
        mov    edx, dword ptr [esi + 34h]
        inc    dword ptr [ebp - 14h]
        add    dx, 8
        mov    ax, di
        out    dx, ax
loc_1_1911:
        cmp    dword ptr [ebp - 14h], 3ch
        jae    SHORT loc_1_1933
        mov    eax, 3ch
        sub    eax, dword ptr [ebp - 14h]
        shr    eax, 1
        push   eax
        db  068h, 0E0h, 035h, 000h, 000h
        mov    eax, dword ptr [esi + 34h]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_1933:
        test   byte ptr [esi + 8ah], 20h
        je     SHORT loc_1_1942
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 2ch], bl
loc_1_1942:
        cmp    byte ptr [esi + 6b0h], bl
        jne    SHORT loc_1_1969
        mov    edx, dword ptr [esi + 34h]
        add    dx, 0ah
        mov    al, 81h
        out    dx, al
        cmp    dword ptr [esi + 84h], ebx
        je     NEAR PTR loc_1_1810
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1969:
        mov    eax, dword ptr [ebp - 18h]
        push   0
        mov    edi, dword ptr [eax + 28h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 84h], 0
        push   3
        push   esi
        call   sub_1_30D0
        dec    dword ptr [edi + 18h]
loc_1_198F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_19A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 6fah], 0
        je     SHORT loc_1_19D7
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_1_19E7
        mov    byte ptr [esi + 88h], 1
        mov    byte ptr [esi + 6fah], 0
        push   0
        lea    eax, [esi + 6eeh]
        push   eax
        call   sub_1_04E2
        jmp    SHORT loc_1_19E7
loc_1_19D7:
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_1_19E7
        mov    byte ptr [esi + 6fah], 1
loc_1_19E7:
        push   7d0h
        add    esi, 6e2h
        push   esi
        call   sub_1_04E2
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h
sub_1_1A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    word ptr [ebp - 14h], 7
        mov    word ptr [ebp - 12h], 7
        db  0C7h, 045h, 0F0h, 000h, 037h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        db  0A1h, 01Ch, 036h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_1A51
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A51:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 4ch], 3
        mov    byte ptr [ebp - 4bh], 0
        mov    dword ptr [ebp - 4ah], 0
        db  0C7h, 045h, 0BAh, 090h, 020h, 000h, 000h
        db  0C7h, 045h, 0BEh, 090h, 022h, 000h, 000h
        db  0C7h, 045h, 0C2h, 070h, 016h, 000h, 000h
        db  0C7h, 045h, 0C6h, 0A0h, 014h, 000h, 000h
        db  0C7h, 045h, 0CAh, 0E0h, 02Dh, 000h, 000h
        db  0C7h, 045h, 0CEh, 0F0h, 023h, 000h, 000h
        db  0C7h, 045h, 0D2h, 080h, 02Ch, 000h, 000h
        db  0C7h, 045h, 0D6h, 050h, 02Dh, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 01Bh, 000h, 000h
        db  0C7h, 045h, 0DEh, 080h, 02Dh, 000h, 000h
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 1eh], ecx
        mov    dword ptr [ebp - 1ah], eax
        lea    eax, [ebp - 4ch]
        push   36h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1AE6
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1AE6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_1B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 70h
        push   esi
        push   edi
        mov    word ptr [ebp - 48h], 0ch
        mov    word ptr [ebp - 46h], 0ch
        db  0C7h, 045h, 0BCh, 014h, 037h, 000h, 000h
        mov    word ptr [ebp - 40h], 0eh
        mov    word ptr [ebp - 3eh], 0eh
        db  0C7h, 045h, 0C4h, 030h, 037h, 000h, 000h
        mov    word ptr [ebp - 30h], 10h
        mov    word ptr [ebp - 2eh], 10h
        db  0C7h, 045h, 0D4h, 078h, 037h, 000h, 000h
        mov    word ptr [ebp - 28h], 15h
        mov    word ptr [ebp - 26h], 15h
        db  0C7h, 045h, 0DCh, 0A0h, 037h, 000h, 000h
        mov    word ptr [ebp - 38h], 0ch
        mov    word ptr [ebp - 36h], 0ch
        db  0C7h, 045h, 0CCh, 0C4h, 037h, 000h, 000h
        db  0A1h, 01Ch, 036h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   6fbh
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_1B90
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_1B90:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 1beh
        rep stosd
        stosw
        stosb
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebp - 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0584
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1D12
        push   0
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1D09
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 28h]
        cmp    eax, 1
        je     SHORT loc_1_1C07
        cmp    eax, 2
        jne    NEAR PTR loc_1_1D09
loc_1_1C07:
        push   1
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1D09
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 28h], 2
        jne    SHORT loc_1_1CB2
        mov    word ptr [ebp - 70h], 0
        lea    edi, [ebp - 6eh]
        sub    eax, eax
        mov    ecx, 7
        rep stosd
        stosw
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   20h
        lea    eax, [ebp - 70h]
        push   eax
        push   1
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D4
        add    esp, 1ch
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1D09
        mov    eax, dword ptr [ebp - 68h]
        sub    eax, dword ptr [ebp - 6ch]
        inc    eax
        cmp    eax, 1000h
        jb     SHORT loc_1_1C8F
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 6ch]
        mov    dword ptr [ecx + 38h], eax
loc_1_1C8F:
        mov    eax, dword ptr [ebp - 58h]
        sub    eax, dword ptr [ebp - 5ch]
        inc    eax
        cmp    eax, 4000h
        jne    SHORT loc_1_1CA8
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 5ch]
        mov    dword ptr [ecx + 3ch], eax
        jmp    SHORT loc_1_1CB2
loc_1_1CA8:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 3ch], 0
loc_1_1CB2:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1CE0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4dh], 0
        jmp    SHORT loc_1_1CEC
        db  005h, 000h, 000h, 000h, 000h
loc_1_1CE0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4dh], eax
loc_1_1CEC:
        push   1
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1D30
loc_1_1D09:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0590
loc_1_1D12:
        push   0
        push   6fbh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_1_1D30:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 48h], eax
        lea    eax, [ebp - 28h]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1D67
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 78h], eax
        jmp    SHORT loc_1_1D71
loc_1_1D67:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 78h], 10h
loc_1_1D71:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1DAD
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_1_1DAD
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, ecx
        add    edx, 62h
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_1_1DAD:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0590
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1DE0
        or     eax, eax
        jne    NEAR PTR loc_1_1D12
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 01Ch, 036h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   26h
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    NEAR PTR loc_1_2037
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ah], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 16h], 1
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [ecx + 1ah], eax
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_1_1E50
        cmp    dword ptr [esi + 2ch], 0
        mov    eax, dword ptr [ebp - 8]
        je     SHORT loc_1_1E46
        jmp    SHORT loc_1_1E53
loc_1_1E46:
        mov    dword ptr [eax + 1eh], 10h
        jmp    SHORT loc_1_1E5A
        db  090h
loc_1_1E50:
        mov    eax, dword ptr [ebp - 8]
loc_1_1E53:
        mov    dword ptr [eax + 1eh], 20h
loc_1_1E5A:
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 34h]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    edi, [esi + 8]
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   edi
        call   sub_1_0432
        mov    dword ptr [ebp - 4], eax
        push   0
        push   26h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2037
        mov    dword ptr [esi + 6d6h], 0
        db  0C7h, 086h, 0DAh, 006h, 000h, 000h, 0C0h, 032h, 000h, 000h
        mov    dword ptr [esi + 6deh], esi
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_1_1F12
        push   1000h
        mov    eax, dword ptr [esi + 38h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 40h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1EE2
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_2027
loc_1_1EE2:
        lea    ecx, [esi + 44h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_1F12
        push   4000h
        mov    eax, dword ptr [esi + 3ch]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1F12
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_2027
loc_1_1F12:
        push   esi
        call   sub_1_06E0
        or     al, al
        jne    SHORT loc_1_1F28
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_2027
loc_1_1F28:
        push   esi
        call   sub_1_0810
        lea    ecx, [esi + 62h]
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_1F68
        cmp    byte ptr [esi + 63h], 0
        jne    SHORT loc_1_1F68
        cmp    byte ptr [esi + 64h], 0
        jne    SHORT loc_1_1F68
        cmp    byte ptr [esi + 65h], 0
        jne    SHORT loc_1_1F68
        cmp    byte ptr [esi + 66h], 0
        jne    SHORT loc_1_1F68
        cmp    byte ptr [esi + 67h], 0
        jne    SHORT loc_1_1F68
        mov    eax, dword ptr [esi + 5ch]
        lea    edx, [esi + 5ch]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_1F68:
        lea    ebx, [esi + 24h]
        push   ebx
        push   esi
        push   ecx
        db  068h, 0B0h, 032h, 000h, 000h
        db  068h, 030h, 032h, 000h, 000h
        db  068h, 0B0h, 031h, 000h, 000h
        mov    eax, dword ptr [esi + 78h]
        push   eax
        db  0E8h, 07Ah, 0E0h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_1FB0
        push   3
        push   2
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 14h
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_2027
        db  08Dh, 064h, 024h, 000h
loc_1_1FB0:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_1_1FBC
        push   esi
        call   sub_1_0770
loc_1_1FBC:
        push   esi
        call   sub_1_0870
        or     al, al
        jne    SHORT loc_1_1FD0
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_201F
        db  090h
loc_1_1FD0:
        push   esi
        call   sub_1_0A80
        or     al, al
        jne    SHORT loc_1_1FE3
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_201F
loc_1_1FE3:
        push   1
        push   0
        mov    eax, dword ptr [esi + 48h]
        push   eax
        push   eax
        db  068h, 0D0h, 00Dh, 000h, 000h
        push   esi
        db  068h, 0B0h, 00Dh, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2043
        push   0
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
loc_1_201F:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 0F9h, 0E0h, 0FEh, 0FFh
loc_1_2027:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0438
loc_1_2037:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2043:
        mov    dword ptr [esi + 6eeh], 0
        db  0C7h, 086h, 0F2h, 006h, 000h, 000h, 0D0h, 00Dh, 000h, 000h
        lea    eax, [esi + 6e2h]
        mov    dword ptr [esi + 6f6h], esi
        mov    dword ptr [eax], 0
        db  0C7h, 086h, 0E6h, 006h, 000h, 000h, 0A0h, 019h, 000h, 000h
        mov    dword ptr [esi + 6eah], esi
        push   7d0h
        push   eax
        call   sub_1_04E2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_1_2090:
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
loc_1_20B3:
        or     edx, edx
        je     SHORT loc_1_20C2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_20B3
        mov    dword ptr [ecx], edx
loc_1_20C2:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_20D4
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_20D4:
        db  0A1h, 01Ch, 036h, 000h, 000h
        push   eax
        push   0
        push   71h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     SHORT loc_1_2112
        push   4
        push   1
        push   2
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 14h
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2112:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 70h]
        cmp    dword ptr [esi + 68h], 0
        jne    SHORT loc_1_212A
        cmp    dword ptr [esi + 6ch], 0
        jne    SHORT loc_1_212A
        push   esi
        call   sub_1_0A00
loc_1_212A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 68h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 68h], eax
        cmp    byte ptr [esi + 6b0h], 0
        jne    NEAR PTR loc_1_2212
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 012h, 0E0h, 0FEh, 0FFh
        or     al, al
        je     NEAR PTR loc_1_2212
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0eeh
        mov    dword ptr [esi + 6b9h], 0eeh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 70h]
        dec    eax
        mov    dword ptr [esi + 70h], eax
        jne    SHORT loc_1_21EA
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_21D2
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_21D2
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_21EA
loc_1_21D2:
        push   1
        add    esi, 6d6h
        push   esi
        call   sub_1_04E2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_21EA:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_2208
        cmp    dword ptr [esi + 84h], 0
        jne    SHORT loc_1_2208
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_2208
        push   esi
        call   sub_1_17E0
loc_1_2208:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2212:
        dec    dword ptr [esi + 70h]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 68h], eax
        push   71h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 14h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
sub_1_2250:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edx, dword ptr [eax + 68h]
        cmp    edx, ecx
        je     SHORT loc_1_2270
loc_1_2260:
        mov    esi, dword ptr [edx + 14h]
        cmp    ecx, esi
        jae    SHORT loc_1_2269
        mov    ecx, esi
loc_1_2269:
        mov    edx, dword ptr [edx + 0ch]
        or     edx, edx
        jne    SHORT loc_1_2260
loc_1_2270:
        or     ecx, ecx
        jne    SHORT loc_1_2279
        mov    ecx, 0eeh
loc_1_2279:
        mov    dword ptr [eax + 6b9h], ecx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2290:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_22B3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_22B3:
        inc    dword ptr [esi + 70h]
        inc    dword ptr [ebx + 18h]
        mov    ecx, dword ptr [esi + 68h]
        cmp    ebx, ecx
        jne    SHORT loc_1_22C8
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 68h], eax
        jmp    SHORT loc_1_22E1
loc_1_22C8:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_1_22D8
loc_1_22D0:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_22D0
loc_1_22D8:
        lea    eax, [ebx + 0ch]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_1_22E1:
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 050h, 0DFh, 0FEh, 0FFh
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_2318
        mov    eax, dword ptr [ebx + 18h]
        cmp    eax, 2
        je     SHORT loc_1_2310
        mov    byte ptr [ebx + 1ch], 1
        sub    dword ptr [ebx + 18h], 2
        mov    edi, 103h
        jmp    SHORT loc_1_2344
        db  08Dh, 049h, 000h
loc_1_2310:
        sub    eax, 2
        mov    dword ptr [ebx + 18h], eax
        jmp    SHORT loc_1_2344
loc_1_2318:
        cmp    edi, 103h
        jne    SHORT loc_1_2330
        mov    byte ptr [ebx + 1ch], 1
        sub    dword ptr [ebx + 18h], 2
        jmp    SHORT loc_1_2344
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2330:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_2341
        mov    byte ptr [ebx + 1ch], 1
        mov    edi, 103h
loc_1_2341:
        dec    dword ptr [ebx + 18h]
loc_1_2344:
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_1_2379
        mov    eax, dword ptr [esi + 6b9h]
        cmp    dword ptr [ebx + 14h], eax
        jne    SHORT loc_1_235B
        push   esi
        call   sub_1_2250
loc_1_235B:
        push   0
        push   71h
        push   ebx
        call   sub_1_050C
        cmp    dword ptr [esi + 68h], 0
        jne    SHORT loc_1_2387
        cmp    dword ptr [esi + 6ch], 0
        jne    SHORT loc_1_2387
        push   esi
        call   sub_1_0A50
        jmp    SHORT loc_1_2387
loc_1_2379:
        mov    eax, dword ptr [esi + 6ch]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 6ch], ebx
        mov    edi, 103h
loc_1_2387:
        mov    eax, dword ptr [esi + 70h]
        dec    eax
        mov    dword ptr [esi + 70h], eax
        jne    SHORT loc_1_23C1
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_23A8
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_23A8
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_23C1
loc_1_23A8:
        push   1
        add    esi, 6d6h
        push   esi
        call   sub_1_04E2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_23C1:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_23DF
        cmp    dword ptr [esi + 84h], 0
        jne    SHORT loc_1_23DF
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_23DF
        push   esi
        call   sub_1_17E0
loc_1_23DF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_23F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 18h]
        inc    dword ptr [edi + 70h]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_2410
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_244C
        db  090h
loc_1_2410:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        or     eax, eax
        jne    SHORT loc_1_2426
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2870
        mov    esi, eax
        jmp    SHORT loc_1_244C
loc_1_2426:
        cmp    eax, 1
        jne    SHORT loc_1_2447
        cmp    byte ptr [edi + 6b0h], 0
        je     SHORT loc_1_243B
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_244C
loc_1_243B:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_28C0
        mov    esi, eax
        jmp    SHORT loc_1_244C
loc_1_2447:
        mov    esi, 10001h
loc_1_244C:
        dec    dword ptr [ebx + 18h]
        mov    eax, dword ptr [edi + 70h]
        dec    eax
        mov    dword ptr [edi + 70h], eax
        jne    SHORT loc_1_2487
        cmp    byte ptr [edi + 6b0h], 0
        jne    SHORT loc_1_2470
        cmp    dword ptr [edi + 6c9h], 0
        jne    SHORT loc_1_2470
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_1_2487
loc_1_2470:
        push   1
        add    edi, 6d6h
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2487:
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_1_24A5
        cmp    dword ptr [edi + 84h], 0
        jne    SHORT loc_1_24A5
        cmp    byte ptr [edi + 6d5h], 0
        jne    SHORT loc_1_24A5
        push   edi
        call   sub_1_17E0
loc_1_24A5:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_24B0:
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
        ja     SHORT loc_1_2500
        je     NEAR PTR loc_1_26D0
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_2521
        shl    eax, 2
        db  0FFh, 0A0h, 01Ch, 028h, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2500:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_26F0
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2710
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_27C9
loc_1_2521:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 049h, 000h
loc_1_2530:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2545
        db  0BEh, 09Ch, 036h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_27D2
loc_1_2545:
        db  0BEh, 020h, 036h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_27D2
loc_1_2554:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 6b0h], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_1_256E
        mov    dword ptr [ebp - 14h], 0
loc_1_256E:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_27D2
        db  005h, 000h, 000h, 000h, 000h
loc_1_2580:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 049h, 000h
loc_1_2590:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_25A0:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_25B0:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_25C0:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 84h], 1
        sbb    eax, eax
        and    eax, 5eeh
        jmp    NEAR PTR loc_1_27CF
loc_1_25D6:
        mov    dword ptr [ebp - 8], 2000h
        jmp    NEAR PTR loc_1_27D2
loc_1_25E2:
        mov    dword ptr [ebp - 8], 5eeh
        jmp    NEAR PTR loc_1_27D2
        db  08Bh, 0FFh
loc_1_25F0:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_2600:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 5ch
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_1_27D2
loc_1_2626:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 28h]
        cmp    eax, 2
        jne    SHORT loc_1_2640
        db  0BEh, 0D0h, 037h, 000h, 000h
        mov    edi, 23h
        jmp    NEAR PTR loc_1_27D2
loc_1_2640:
        cmp    eax, 1
        jne    NEAR PTR loc_1_27D2
        db  0BEh, 0F4h, 037h, 000h, 000h
        mov    edi, 1fh
        jmp    NEAR PTR loc_1_27D2
loc_1_2658:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2670
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 24h]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_27CF
        db  08Dh, 064h, 024h, 000h
loc_1_2670:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_27CF
        db  08Bh, 0FFh
loc_1_2680:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2694
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 6b9h]
        jmp    NEAR PTR loc_1_27CF
loc_1_2694:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_27CF
        db  090h
loc_1_26A0:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_1_27D2
loc_1_26B3:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_27D2
        db  090h
loc_1_26C0:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_26D0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 5ch
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_27D2
        db  090h
loc_1_26F0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 62h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_27D2
        db  090h
loc_1_2710:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_276A
        mov    eax, dword ptr [ebx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_2742
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_1_27D2
loc_1_2742:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 005h, 0E0h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_27D2
loc_1_276A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        db  0E8h, 0E6h, 0DEh, 0FEh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_279A
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_1_27D2
loc_1_279A:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0E6h, 0DEh, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_27D2
loc_1_27C9:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 78h]
loc_1_27CF:
        mov    dword ptr [ebp - 8], eax
loc_1_27D2:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_280E
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_280E
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_1_27FE
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
loc_1_27FE:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_280E:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh
loc_1_281C:
        db  030h, 025h, 000h, 000h, 054h, 025h
        add    byte ptr [eax], al
        db  080h, 025h, 000h, 000h, 080h, 025h, 000h
        db  000h, 090h, 025h, 000h, 000h, 0A0h
        db  025h, 000h, 000h, 0B0h, 025h
        add    byte ptr [eax], al
        db  0C0h, 025h, 000h, 000h, 0D6h, 025h, 000h
        db  000h, 0E2h
        db  025h, 000h, 000h, 0F0h, 025h
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [eax], al
        add    byte ptr es:[eax], al
        pop    eax
        add    byte ptr es:[eax], al
        and    byte ptr [esi], 0
        db  000h, 0A0h, 026h, 000h, 000h, 0B3h
        add    byte ptr es:[eax], al
        db  021h, 025h, 000h, 000h, 0C0h, 026h
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
sub_1_2870:
        push   ebp
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
        call   sub_1_24B0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_28C0:
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
        je     SHORT loc_1_2917
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_2978
        cmp    eax, 10112h
        je     NEAR PTR loc_1_2A07
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2A34
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2A75
loc_1_2917:
        cmp    edi, 4
        je     SHORT loc_1_2934
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 4
        jmp    NEAR PTR loc_1_2A75
loc_1_2934:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_2961
        mov    ebx, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2A75
loc_1_2961:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2A90
        jmp    NEAR PTR loc_1_2A73
loc_1_2978:
        cmp    edi, 4
        je     SHORT loc_1_2995
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2A75
loc_1_2995:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0fffffffeh
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_2A00
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 6b9h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_29D6
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 6b9h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_2A75
loc_1_29D6:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 14h], eax
        jne    SHORT loc_1_29F7
        mov    eax, dword ptr [ecx + 14h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_29F7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        push   edx
        call   sub_1_2250
        jmp    NEAR PTR loc_1_2A75
loc_1_29F7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    SHORT loc_1_2A75
        db  090h
loc_1_2A00:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_2A75
loc_1_2A07:
        cmp    edi, 4
        je     SHORT loc_1_2A21
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_2A75
loc_1_2A21:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 1dh
        push   ecx
        call   sub_1_05EC
        sub    ebx, ebx
        jmp    SHORT loc_1_2A75
loc_1_2A34:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_1_2A58
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_2A75
loc_1_2A58:
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2AF0
loc_1_2A73:
        mov    ebx, eax
loc_1_2A75:
        or     ebx, ebx
        jne    SHORT loc_1_2A83
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_1_2A83:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_2A90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6b0h], 0
        jne    SHORT loc_1_2ADA
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_2AD0
        inc    dword ptr [esi + 18h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [ecx + 24h]
        push   eax
        db  0E8h, 01Dh, 0DBh, 0FEh, 0FFh
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2AD0:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_2ADA:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2AF0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6b0h], 0
        jne    SHORT loc_1_2B3A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_2B30
        inc    dword ptr [esi + 18h]
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [ecx + 24h]
        push   eax
        db  0E8h, 0D9h, 0D7h, 0FEh, 0FFh
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    14h
        db  090h
loc_1_2B30:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_1_2B3A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B50:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_24B0
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_1_2C66
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2BD6
        je     SHORT loc_1_2C20
        cmp    eax, 20101h
        je     SHORT loc_1_2BF2
        cmp    eax, 20102h
        je     SHORT loc_1_2C00
        cmp    eax, 20103h
        je     SHORT loc_1_2C08
        cmp    eax, 20104h
        je     SHORT loc_1_2C10
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2C49
loc_1_2BD6:
        cmp    eax, 1020101h
        je     SHORT loc_1_2C28
        cmp    eax, 1020102h
        je     SHORT loc_1_2C31
        cmp    eax, 1020103h
        je     SHORT loc_1_2C40
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2C49
loc_1_2BF2:
        mov    eax, dword ptr [ebx + 698h]
        jmp    SHORT loc_1_2C46
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2C00:
        mov    eax, dword ptr [ebx + 69ch]
        jmp    SHORT loc_1_2C46
loc_1_2C08:
        mov    eax, dword ptr [ebx + 6a0h]
        jmp    SHORT loc_1_2C46
loc_1_2C10:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2C49
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C20:
        mov    eax, dword ptr [ebx + 6ach]
        jmp    SHORT loc_1_2C46
loc_1_2C28:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2C49
loc_1_2C31:
        mov    eax, dword ptr [ebx + 6a4h]
        jmp    SHORT loc_1_2C46
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C40:
        mov    eax, dword ptr [ebx + 6a8h]
loc_1_2C46:
        mov    dword ptr [ebp - 4], eax
loc_1_2C49:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_2C66
        or     edi, edi
        jne    SHORT loc_1_2C66
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 8], 4
loc_1_2C66:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_2C80:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2CB2
        cmp    eax, 20101h
        jae    SHORT loc_1_2CD3
        cmp    eax, 10101h
        jb     SHORT loc_1_2CCE
        cmp    eax, 10111h
        jbe    SHORT loc_1_2CD3
        cmp    eax, 10113h
        je     SHORT loc_1_2CD3
        jmp    SHORT loc_1_2CCE
loc_1_2CB2:
        cmp    eax, 1010101h
        jb     SHORT loc_1_2CCE
        cmp    eax, 1010104h
        jbe    SHORT loc_1_2CD3
        cmp    eax, 1020101h
        jb     SHORT loc_1_2CCE
        cmp    eax, 1020103h
        jbe    SHORT loc_1_2CD3
loc_1_2CCE:
        mov    esi, 0c0010017h
loc_1_2CD3:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 70h]
        or     esi, esi
        jne    SHORT loc_1_2CE6
        push   ecx
        push   edi
        call   sub_1_2B50
        mov    esi, eax
loc_1_2CE6:
        mov    eax, dword ptr [edi + 70h]
        dec    eax
        mov    dword ptr [edi + 70h], eax
        jne    SHORT loc_1_2D20
        cmp    byte ptr [edi + 6b0h], 0
        jne    SHORT loc_1_2D07
        cmp    dword ptr [edi + 6c9h], 0
        jne    SHORT loc_1_2D07
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_1_2D20
loc_1_2D07:
        push   1
        add    edi, 6d6h
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_2D20:
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_1_2D3E
        cmp    dword ptr [edi + 84h], 0
        jne    SHORT loc_1_2D3E
        cmp    byte ptr [edi + 6d5h], 0
        jne    SHORT loc_1_2D3E
        push   edi
        call   sub_1_17E0
loc_1_2D3E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0420
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2D80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 6e2h]
        push   eax
        call   sub_1_04E8
        push   2625a0h
        call   sub_1_0500
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 073h, 0D3h, 0FEh, 0FFh
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_1_04F4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0438
        push   0
        push   6fbh
        push   esi
        call   sub_1_050C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_2DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 1ch], 0
        je     SHORT loc_1_2E03
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2E03:
        inc    dword ptr [esi + 18h]
        inc    dword ptr [edi + 70h]
        cmp    byte ptr [edi + 6b0h], 0
        je     SHORT loc_1_2E26
        dec    dword ptr [esi + 18h]
        dec    dword ptr [edi + 70h]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2E26:
        mov    ebx, dword ptr [edi + 68h]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     SHORT loc_1_2E56
loc_1_2E34:
        inc    dword ptr [ebx + 18h]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010004h
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [ebx + 0ch]
        dec    dword ptr [ebx + 18h]
        mov    ebx, eax
        or     eax, eax
        jne    SHORT loc_1_2E34
loc_1_2E56:
        mov    byte ptr [edi + 6b0h], 1
        mov    dword ptr [edi + 6b1h], 0
        mov    dword ptr [edi + 6b5h], esi
        push   edi
        call   sub_1_2EE0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 18h]
        mov    eax, dword ptr [edi + 70h]
        dec    eax
        mov    dword ptr [edi + 70h], eax
        jne    SHORT loc_1_2EB4
        cmp    byte ptr [edi + 6b0h], 0
        jne    SHORT loc_1_2E9A
        cmp    dword ptr [edi + 6c9h], 0
        jne    SHORT loc_1_2E9A
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_1_2EB4
loc_1_2E9A:
        push   1
        add    edi, 6d6h
        push   edi
        call   sub_1_04E2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2EB4:
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_1_2ED2
        cmp    dword ptr [edi + 84h], 0
        jne    SHORT loc_1_2ED2
        cmp    byte ptr [edi + 6d5h], 0
        jne    SHORT loc_1_2ED2
        push   edi
        call   sub_1_17E0
loc_1_2ED2:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_2EE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 70h]
        cmp    dword ptr [esi + 6c9h], 0
        je     SHORT loc_1_2F2C
loc_1_2EF4:
        mov    ecx, dword ptr [esi + 6c9h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 6c9h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_1_2F20
        cmp    eax, 8
        je     SHORT loc_1_2F20
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_2F20:
        dec    dword ptr [edi + 18h]
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_2EF4
loc_1_2F2C:
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_1_2F50
        mov    dword ptr [esi + 6b1h], 2
        dec    dword ptr [esi + 70h]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2F50:
        dec    dword ptr [esi + 70h]
        push   esi
        call   sub_1_2F60
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_2F60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 70h]
        inc    ecx
        mov    dword ptr [eax + 70h], ecx
        cmp    dword ptr [eax + 84h], 0
        je     SHORT loc_1_2F8C
        mov    dword ptr [eax + 6b1h], 3
        dec    dword ptr [eax + 70h]
        mov    eax, 103h
        pop    ebp
        ret    4
loc_1_2F8C:
        dec    ecx
        mov    dword ptr [eax + 70h], ecx
        push   eax
        call   sub_1_2FA0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 70h]
        mov    ecx, dword ptr [esi + 84h]
        or     ecx, ecx
        je     SHORT loc_1_2FD7
        mov    edi, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    dword ptr [esi + 84h], 0
loc_1_2FD7:
        mov    edx, dword ptr [esi + 7ch]
        or     edx, edx
        je     SHORT loc_1_3004
        mov    edi, edi
loc_1_2FE0:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 7ch], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    edx, dword ptr [esi + 7ch]
        or     edx, edx
        jne    SHORT loc_1_2FE0
loc_1_3004:
        mov    edx, dword ptr [esi + 6bdh]
        or     edx, edx
        je     SHORT loc_1_303A
        mov    edi, edi
loc_1_3010:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 6bdh], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    edx, dword ptr [esi + 6bdh]
        or     edx, edx
        jne    SHORT loc_1_3010
loc_1_303A:
        cmp    byte ptr [esi + 6d5h], 0
        je     SHORT loc_1_3056
loc_1_3043:
        push   2710h
        call   sub_1_0500
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_3043
loc_1_3056:
        push   esi
        call   sub_1_0BB0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 6b0h], 0
        mov    edi, dword ptr [esi + 68h]
        or     edi, edi
        je     SHORT loc_1_30B6
        sub    ebx, ebx
loc_1_3078:
        inc    dword ptr [edi + 18h]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_308F
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_308F:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0474
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 18h]
        mov    edi, eax
        cmp    eax, ebx
        jne    SHORT loc_1_3078
loc_1_30B6:
        dec    dword ptr [esi + 70h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 6b1h], eax
        jne    NEAR PTR loc_1_31A8
        cmp    eax, 1
        je     SHORT loc_1_3150
        cmp    eax, 2
        je     SHORT loc_1_3158
        cmp    eax, 3
        je     SHORT loc_1_3160
        mov    eax, dword ptr [ebp - 4]
loc_1_30FB:
        cmp    eax, 103h
        je     SHORT loc_1_311A
        push   eax
        mov    eax, dword ptr [esi + 6b5h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 6b5h]
        dec    dword ptr [eax + 18h]
loc_1_311A:
        mov    eax, dword ptr [esi + 70h]
        dec    eax
        mov    dword ptr [esi + 70h], eax
        jne    SHORT loc_1_318A
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_313B
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_313B
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_318A
loc_1_313B:
        push   1
        add    esi, 6d6h
        push   esi
        call   sub_1_04E2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3150:
        push   esi
        call   sub_1_2EE0
        jmp    SHORT loc_1_30FB
loc_1_3158:
        push   esi
        call   sub_1_2F60
        jmp    SHORT loc_1_30FB
loc_1_3160:
        mov    eax, dword ptr [esi + 70h]
        inc    eax
        mov    dword ptr [esi + 70h], eax
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_1_317B
        dec    eax
        mov    dword ptr [esi + 70h], eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_317B:
        dec    eax
        mov    dword ptr [esi + 70h], eax
        push   esi
        call   sub_1_2FA0
        jmp    NEAR PTR loc_1_30FB
loc_1_318A:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_31A8
        cmp    dword ptr [esi + 84h], 0
        jne    SHORT loc_1_31A8
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_31A8
        push   esi
        call   sub_1_17E0
loc_1_31A8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_31B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_31C3
        lea    edx, [eax + 49h]
loc_1_31C3:
        cmp    byte ptr [ecx + 6b0h], 0
        je     SHORT loc_1_31D5
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_31D5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 18h]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 6c9h], 0
        jne    SHORT loc_1_3210
        mov    dword ptr [ecx + 6cdh], edx
        mov    dword ptr [ecx + 6c9h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_3210:
        mov    eax, dword ptr [ecx + 6cdh]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 6cdh], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3230:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_3243
        lea    edx, [eax + 21h]
loc_1_3243:
        cmp    byte ptr [ecx + 6b0h], 0
        je     SHORT loc_1_3255
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_3255:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 18h]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 6c9h], 0
        jne    SHORT loc_1_3290
        mov    dword ptr [ecx + 6cdh], edx
        mov    dword ptr [ecx + 6c9h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_3290:
        mov    eax, dword ptr [ecx + 6cdh]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 6cdh], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_32B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 18h]
        ret    4
        db  08Dh, 049h, 000h
loc_1_32C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 70h]
        cmp    byte ptr [esi + 6b0h], 0
        jne    NEAR PTR loc_1_3376
        cmp    dword ptr [esi + 6d1h], 0
        jne    NEAR PTR loc_1_3376
loc_1_32E6:
        mov    edi, dword ptr [esi + 6c9h]
        or     edi, edi
        je     SHORT loc_1_3351
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 6c9h], eax
        cmp    dword ptr [esi + 6cdh], edi
        jne    SHORT loc_1_330A
        mov    dword ptr [esi + 6cdh], 0
loc_1_330A:
        mov    dword ptr [esi + 6d1h], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_3325
        cmp    eax, 0ah
        je     SHORT loc_1_3325
        push   esi
        call   sub_1_3450
        jmp    SHORT loc_1_332B
loc_1_3325:
        push   esi
        call   sub_1_34C0
loc_1_332B:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_3345
        cmp    ecx, 4
        je     SHORT loc_1_3345
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_3345:
        dec    dword ptr [ebx + 18h]
        cmp    byte ptr [esi + 6b0h], 0
        je     SHORT loc_1_32E6
loc_1_3351:
        mov    dword ptr [esi + 6d1h], 0
        cmp    byte ptr [esi + 6b0h], 0
        je     SHORT loc_1_3376
        dec    dword ptr [esi + 70h]
        push   1
        push   esi
        call   sub_1_30D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3376:
        mov    edi, dword ptr [esi + 6ch]
        or     edi, edi
        je     SHORT loc_1_33F1
        sub    ebx, ebx
        nop
loc_1_3380:
        cmp    dword ptr [edi + 18h], 0
        jbe    SHORT loc_1_33A0
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_1_3395
        push   esi
        call   sub_1_1160
loc_1_3395:
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_33DB
        db  08Dh, 064h, 024h, 000h
loc_1_33A0:
        mov    eax, dword ptr [edi + 14h]
        cmp    dword ptr [esi + 6b9h], eax
        jne    SHORT loc_1_33B1
        push   esi
        call   sub_1_2250
loc_1_33B1:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [edi + 0ch]
        je     SHORT loc_1_33CA
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_33CD
loc_1_33CA:
        mov    dword ptr [esi + 6ch], eax
loc_1_33CD:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   71h
        push   eax
        call   sub_1_050C
loc_1_33DB:
        or     edi, edi
        jne    SHORT loc_1_3380
        cmp    dword ptr [esi + 6ch], 0
        jne    SHORT loc_1_33F1
        cmp    dword ptr [esi + 68h], 0
        jne    SHORT loc_1_33F1
        push   esi
        call   sub_1_0A50
loc_1_33F1:
        mov    eax, dword ptr [esi + 70h]
        dec    eax
        mov    dword ptr [esi + 70h], eax
        jne    SHORT loc_1_3427
        cmp    byte ptr [esi + 6b0h], 0
        jne    SHORT loc_1_3412
        cmp    dword ptr [esi + 6c9h], 0
        jne    SHORT loc_1_3412
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_3427
loc_1_3412:
        push   1
        add    esi, 6d6h
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3427:
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_1_3445
        cmp    dword ptr [esi + 84h], 0
        jne    SHORT loc_1_3445
        cmp    byte ptr [esi + 6d5h], 0
        jne    SHORT loc_1_3445
        push   esi
        call   sub_1_17E0
loc_1_3445:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_1_3450:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 24h]
        mov    eax, dword ptr [eax + 0ch]
        test   al, 20h
        mov    dword ptr [esi + 8ah], eax
        je     SHORT loc_1_3470
        lea    ecx, [esi + 53h]
        mov    byte ptr [ecx], 7
        jmp    SHORT loc_1_3499
        db  090h
loc_1_3470:
        test   al, 4
        push   esi
        je     SHORT loc_1_3480
        call   sub_1_0CF0
        jmp    SHORT loc_1_3485
        db  08Dh, 064h, 024h, 000h
loc_1_3480:
        call   sub_1_34C0
loc_1_3485:
        cmp    dword ptr [esi + 8ah], 0
        lea    ecx, [esi + 53h]
        je     SHORT loc_1_3496
        mov    byte ptr [ecx], 6
        jmp    SHORT loc_1_3499
loc_1_3496:
        mov    byte ptr [ecx], 4
loc_1_3499:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_34A9
        mov    edx, dword ptr [esi + 34h]
        add    dx, 10h
        mov    al, 5
        out    dx, al
loc_1_34A9:
        mov    edx, dword ptr [esi + 34h]
        add    dx, 5
        mov    al, byte ptr [ecx]
        out    dx, al
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_34C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0C70
        push   esi
        call   sub_1_0D50
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 02Fh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 018h, 019h, 01Ah, 01Bh
        db  01Ch, 01Dh, 01Eh, 01Fh, 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h, 038h, 039h, 03Ah, 03Bh
        db  03Ch, 03Dh, 03Eh, 03Fh, 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h, 058h, 059h, 05Ah, 05Bh
        db  05Ch, 05Dh, 05Eh, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 02Fh, 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h, 0B8h, 0B9h, 0BAh, 0BBh
        db  0BCh, 0BDh, 0BEh, 00Fh, 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h, 0D8h, 0D9h, 0DAh, 0DBh
        db  0DCh, 0DDh, 0DEh, 0DFh, 0E0h, 0E1h, 0E2h, 0E3h, 0E4h, 0E5h, 0E6h, 0E7h, 0F8h, 0F9h, 0FAh, 0FBh
        db  0FCh, 0FDh, 0FEh, 000h, 000h, 000h
loc_1_35C0:
        inc    eax
        db  035h, 000h, 000h, 080h, 035h
        add    byte ptr [eax], al
loc_1_35C8:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 6d6fh
loc_1_35D0:
        inc    ebp
        je     SHORT loc_1_363B
        db  065h, 072h, 06Eh
        je     NEAR PTR loc_1_3604
        dec    ebp
        outsd
        insd
        add    byte ptr [eax], al
loc_1_35E0:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_3604:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_3620:
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
loc_1_363B:
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
loc_1_369C:
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
        inc    ebx
        inc    ebp
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
loc_1_3700:
        inc    ebx
        inc    ebp
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 054h
        jns    SHORT loc_1_3782
        add    byte ptr gs:[ecx + 64h], al
        popad
        jo     SHORT loc_1_378D
        jb     NEAR PTR loc_1_3770
        db  079h, 070h
        add    byte ptr gs:[ecx + 6fh], cl
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_3790
        jae    SHORT loc_1_37A0
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], cl
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_37A0
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        db  072h, 079h
        inc    edx
        popad
        jae    SHORT loc_1_37AF
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_37C4
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        db  072h, 079h
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_37D8
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_37D1
        db  072h, 072h
        db  075h, 070h
loc_1_3770:
        db  074h, 04Eh
        jne    SHORT loc_1_37E1
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_37E1
        jb     SHORT loc_1_37F0
        jne    SHORT loc_1_37F0
        je     SHORT loc_1_37D0
loc_1_3782:
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ebp + 61h], cl
        js     SHORT loc_1_37F5
        insd
loc_1_378D:
        db  075h, 06Dh
        dec    ebp
loc_1_3790:
        jne    SHORT loc_1_37FE
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_37A0:
        dec    ebp
        popad
        js     SHORT loc_1_380D
        insd
        db  075h, 06Dh
        dec    ebp
        db  075h, 06Ch
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
loc_1_37AF:
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 53h], 63656c65h
        je     SHORT loc_1_37C4
loc_1_37C4:
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 53h], 63656c65h
        je     SHORT loc_1_37D0
loc_1_37D0:
        pop    eax
loc_1_37D1:
        imul   esi, dword ptr [edx + 63h], 50206d6fh
loc_1_37D8:
        outsd
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_37FE
        inc    ebp
        db  074h, 068h
loc_1_37E1:
        db  065h, 072h, 06Eh
        je     NEAR PTR loc_1_3807
        dec    ecx
        dec    ecx
        dec    ecx
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
loc_1_37F0:
        db  065h, 072h, 02Eh
        db  000h
loc_1_37F4:
        inc    esi
loc_1_37F5:
        db  075h, 06Ah
        imul   esi, dword ptr [ebx + 74h], 424d2075h
loc_1_37FE:
        dec    eax
        xor    dword ptr [eax], esi
        xor    esi, dword ptr [eax]
        xor    ah, byte ptr [eax]
        inc    edx
        popad
loc_1_3807:
        db  073h, 065h
        and    byte ptr fs:[ecx + 64h], al
loc_1_380D:
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
_LTEXT ends
        end
