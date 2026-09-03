; elnk16_lcode_01.asm -- ELNK16 object 1 segment _LTEXT
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
        db  0E8h, 0E0h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
ELNK16_Control:
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
sub_1_047A:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
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
sub_1_057E:
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 045h, 04Ch, 04Eh, 04Bh
        db  031h, 036h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    eax, dword ptr [edx + 45h]
        mov    si, word ptr [edx + 1ah]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [edx + 16ah]
        mov    ax, word ptr [eax + ecx + 4]
        mov    ecx, dword ptr [edx + 2]
        sub    ecx, esi
        mov    word ptr [ecx + 0ffc4h], ax
        xor    ecx, ecx
        mov    cx, word ptr [edx + 1ah]
        mov    eax, dword ptr [edx + 2]
        sub    eax, ecx
        pop    esi
        mov    word ptr [eax + 0ffc2h], 100h
        mov    dx, word ptr [edx]
        add    dx, 0bh
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0700:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [ecx + 24h]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [edx + edx*4]
        lea    ebx, [eax + eax*4]
        mov    eax, dword ptr [edi + 16ah]
        add    eax, ebx
        mov    esi, dword ptr [eax]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [edi + 16ah]
        mov    dword ptr [ebx + eax + 0ch], 0ffffh
        mov    eax, dword ptr [edi + 16ah]
        mov    dword ptr [ebx + eax + 14h], 0
        lea    eax, [ebp - 10h]
        mov    word ptr [esi], 0
        mov    word ptr [esi + 4], 0ffffh
        mov    word ptr [esi + 2], 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   ecx
        call   sub_1_0530
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        push   6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 0ch], 0ch
        mov    eax, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 4], 0eh
        mov    ax, word ptr [eax]
        mov    word ptr [esi + 0eh], ax
        add    dword ptr [ebp - 0ch], 2
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_07C9
        dec    dword ptr [ebp - 14h]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
loc_1_07C9:
        mov    eax, dword ptr [edi + 16ah]
        mov    edi, dword ptr [ebx + eax + 6]
        mov    ebx, dword ptr [ebp - 14h]
        or     ebx, ebx
        je     SHORT loc_1_0817
loc_1_07DA:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_07EE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        call   sub_1_05EC
loc_1_07EE:
        add    edi, dword ptr [ebp - 4]
        cmp    ebx, 1
        jbe    SHORT loc_1_0814
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
loc_1_0814:
        dec    ebx
        jne    SHORT loc_1_07DA
loc_1_0817:
        cmp    dword ptr [ebp - 10h], 3ch
        jae    SHORT loc_1_083C
        mov    edx, 3ch
        sub    edx, dword ptr [ebp - 10h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    dword ptr [ebp - 10h], 3ch
loc_1_083C:
        mov    eax, dword ptr [ebp - 10h]
        sub    ax, 0eh
        or     ax, 8000h
        mov    word ptr [esi + 10h], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 8], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_0860:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [esi + esi*4]
        lea    ecx, [eax + eax*4]
        mov    eax, dword ptr [edi + 16ah]
        mov    eax, dword ptr [ecx + eax]
        mov    edx, dword ptr [edi + 71h]
        mov    bx, word ptr [eax + 2]
        or     bx, 0a000h
        mov    word ptr [eax + 2], bx
        mov    eax, dword ptr [edi + 16ah]
        mov    byte ptr [ecx + eax + 18h], 0
        mov    eax, dword ptr [edi + 16ah]
        cmp    edx, 0ffffh
        mov    dword ptr [eax + ecx + 0ch], 0ffffh
        mov    dword ptr [edi + 71h], esi
        je     SHORT loc_1_08D0
        lea    ecx, [edx + edx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [edi + 16ah]
        mov    dword ptr [ecx + eax + 0ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_08D0:
        cmp    dword ptr [edi + 6dh], 0ffffh
        jne    SHORT loc_1_08DC
        mov    dword ptr [edi + 6dh], esi
loc_1_08DC:
        sub    ebx, ebx
        mov    edi, edi
loc_1_08E0:
        xor    ecx, ecx
        mov    cx, word ptr [edi + 1ah]
        mov    eax, dword ptr [edi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        or     ax, ax
        je     SHORT loc_1_0907
        push   32h
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 4e20h
        jbe    SHORT loc_1_08E0
loc_1_0907:
        mov    dword ptr [edi + 45h], esi
        push   edi
        db  068h, 0A0h, 006h, 000h, 000h
        add    edi, 131h
        push   edi
        call   sub_1_04FA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0930:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 1dh]
        mov    ax, word ptr [edi + 2]
        or     ax, 8000h
        mov    word ptr [edi + 2], ax
        mov    word ptr [edi], 0
loc_1_0952:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        or     ax, ax
        je     SHORT loc_1_0979
        push   32h
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 4e20h
        jbe    SHORT loc_1_0952
loc_1_0979:
        mov    eax, dword ptr [esi + 21h]
        mov    dword ptr [esi + 45h], eax
        push   esi
        db  068h, 0A0h, 006h, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        sub    ebx, ebx
loc_1_0993:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        or     ax, ax
        je     SHORT loc_1_09BA
        push   32h
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 4e20h
        jbe    SHORT loc_1_0993
loc_1_09BA:
        sub    esi, esi
        mov    ebx, 8000h
loc_1_09C1:
        mov    ax, word ptr [edi]
        test   bx, ax
        jne    SHORT loc_1_09D9
        push   32h
        call   sub_1_0500
        inc    esi
        cmp    esi, 4e20h
        jbe    SHORT loc_1_09C1
loc_1_09D9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09F0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + 65h], 0
        je     SHORT loc_1_0A20
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 69h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 69h]
        inc    eax
        mov    dword ptr [edx + 69h], eax
        cmp    dword ptr [edx + 21h], eax
        ja     SHORT loc_1_0A17
        mov    dword ptr [edx + 69h], 0
loc_1_0A17:
        dec    dword ptr [edx + 65h]
        mov    al, 1
        pop    ebp
        ret    8
loc_1_0A20:
        mov    byte ptr [edx + 85h], 0
        sub    al, al
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_0A30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 16ah]
        lea    ecx, [esi + esi*4]
        lea    ecx, [ecx + ecx*4]
        add    eax, ecx
        cmp    dword ptr [edx + 71h], esi
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [edx + 6dh], eax
        jne    SHORT loc_1_0A5C
        mov    dword ptr [edx + 71h], 0ffffh
loc_1_0A5C:
        mov    word ptr [ecx + 4], 0ffffh
        mov    word ptr [ecx], 0
        inc    dword ptr [edx + 65h]
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_0A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 16ah]
        or     eax, eax
        je     SHORT loc_1_0A8B
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0A8B:
        mov    eax, dword ptr [esi + 16eh]
        or     eax, eax
        je     SHORT loc_1_0A9F
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0A9F:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0a3h], 0
        mov    dword ptr [eax + 35h], 0
        mov    dword ptr [eax + 39h], 0
        mov    dword ptr [eax + 6dh], 0ffffh
        mov    dword ptr [eax + 71h], 0ffffh
        mov    dword ptr [eax + 69h], 0
        mov    ecx, dword ptr [eax + 21h]
        mov    dword ptr [eax + 65h], ecx
        mov    dword ptr [eax + 75h], 0
        mov    dword ptr [eax + 79h], 0
        mov    dword ptr [eax + 7dh], 0
        mov    dword ptr [eax + 81h], 0
        mov    byte ptr [eax + 85h], 1
        mov    byte ptr [eax + 86h], 0
        mov    dword ptr [eax + 87h], 0
        mov    dword ptr [eax + 8bh], 0
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        sub    edx, edx
        inc    dword ptr [esi + 59h]
        mov    ecx, dword ptr [ebp + 18h]
        cmp    edx, ecx
        je     SHORT loc_1_0B5B
        mov    eax, dword ptr [ebp + 14h]
loc_1_0B4A:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_0B57
        add    eax, 4
        inc    edx
        cmp    edx, ecx
        jb     SHORT loc_1_0B4A
loc_1_0B57:
        cmp    edx, ecx
        jne    SHORT loc_1_0B65
loc_1_0B5B:
        mov    edi, 0c0010019h
        jmp    NEAR PTR loc_1_0C1E
loc_1_0B65:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], edx
        db  0A1h, 070h, 035h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   45h
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     SHORT loc_1_0B90
        mov    edi, 0c000009ah
        jmp    NEAR PTR loc_1_0C1E
        db  005h, 000h, 000h, 000h, 000h
loc_1_0B90:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 41h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 29h], 3
        mov    eax, dword ptr [ebp - 4]
        add    eax, 19h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 19h
        push   eax
        push   esi
        call   sub_1_1D10
        mov    edi, 103h
        cmp    byte ptr [esi + 54h], 0
        je     SHORT loc_1_0C1E
        mov    byte ptr [esi + 54h], 0
        push   7d0h
        lea    eax, [esi + 149h]
        push   eax
        call   sub_1_04E2
loc_1_0C1E:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_0C56
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_0C48
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_0C48
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_0C56
loc_1_0C48:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_0C56:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0C60:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [edi + 18h], 0
        jne    SHORT loc_1_0CA0
        lea    eax, [edi + 19h]
        mov    dword ptr [edi + 29h], 4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        push   eax
        push   esi
        call   sub_1_1D10
        dec    dword ptr [edi + 14h]
        mov    edi, 103h
        jmp    SHORT loc_1_0CA5
        db  005h, 000h, 000h, 000h, 000h
loc_1_0CA0:
        mov    edi, 0c0010002h
loc_1_0CA5:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_0CDD
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_0CCF
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_0CCF
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_0CDD
loc_1_0CCF:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_0CDD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 084h, 037h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        db  0A1h, 070h, 037h, 000h, 000h
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0D20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 149h]
        push   eax
        call   sub_1_04E8
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04F4
        mov    eax, dword ptr [esi + 166h]
        push   eax
        db  0E8h, 0CEh, 0F3h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        call   sub_1_0438
        mov    eax, dword ptr [esi + 0e1h]
        mov    ecx, dword ptr [esi + 0e5h]
        lea    edx, [esi + 0e1h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0
        push   esi
        call   sub_1_050C
        mov    dx, 100h
        sub    al, al
        out    dx, al
        push   esi
        call   sub_1_3290
        mov    dx, 100h
        sub    al, al
        out    dx, al
        push   esi
        call   sub_1_3290
        push   esi
        call   sub_1_3290
        mov    dx, 100h
        mov    al, 0ffh
        out    dx, al
        mov    al, 10h
        out    0fh, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DC0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [ecx + 1ch]
        or     al, 4
        mov    byte ptr [ecx + 1ch], al
        mov    dx, word ptr [ecx]
        add    dx, 6
        out    dx, al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc0h]
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [esi]
        add    dx, 6
        in     al, dx
        test   al, 8
        je     SHORT loc_1_0E17
        mov    dx, word ptr [esi]
        add    dx, 0ah
        mov    al, 0ffh
        out    dx, al
loc_1_0E17:
        test   byte ptr [ebp - 4], 40h
        je     SHORT loc_1_0E27
        cmp    byte ptr [esi + 4eh], 0
        je     SHORT loc_1_0E27
        mov    byte ptr [esi + 4eh], 0
loc_1_0E27:
        mov    byte ptr [esi + 58h], 0
        mov    ax, word ptr [ebp - 4]
        and    ax, 0f000h
        je     SHORT loc_1_0E80
        cmp    byte ptr [esi + 53h], 0
        je     SHORT loc_1_0E63
        xor    edi, edi
        mov    di, word ptr [esi + 1ah]
        mov    edx, dword ptr [esi + 2]
        sub    edx, edi
        sub    cl, cl
        mov    word ptr [edx + 0ffc2h], ax
        mov    dx, word ptr [esi]
        add    dx, 0bh
        sub    al, al
        out    dx, al
        mov    al, cl
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0E63:
        mov    cl, 1
        mov    al, byte ptr [esi + 1ch]
        and    al, 0fbh
        mov    byte ptr [esi + 1ch], al
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        mov    al, cl
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0E80:
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 6], di
        cmp    byte ptr [esi + 50h], 0
        jne    NEAR PTR loc_1_1125
        mov    byte ptr [esi + 50h], 1
        mov    al, byte ptr [esi + 1ch]
        and    al, 0fbh
        mov    byte ptr [esi + 1ch], al
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        cmp    byte ptr [esi + 57h], 0
        je     SHORT loc_1_0ECF
        mov    byte ptr [esi + 57h], 0
        mov    di, 8000h
loc_1_0ECF:
        sub    ebx, ebx
loc_1_0ED1:
        test   di, 0f000h
        jne    SHORT loc_1_0F53
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc0h]
        mov    word ptr [ebp - 6], ax
        test   al, 40h
        je     SHORT loc_1_0EFA
        cmp    byte ptr [esi + 4eh], bl
        je     SHORT loc_1_0EFA
        mov    byte ptr [esi + 4eh], bl
loc_1_0EFA:
        and    word ptr [ebp - 6], 0f000h
        je     SHORT loc_1_0F53
        mov    dword ptr [ebp - 0ch], ebx
loc_1_0F05:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_0F2F
        push   32h
        call   sub_1_0500
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 4e20h
        jbe    SHORT loc_1_0F05
loc_1_0F2F:
        xor    edx, edx
        mov    dx, word ptr [esi + 1ah]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 0ffc2h], ax
        or     di, word ptr [ebp - 6]
        mov    dx, word ptr [esi]
        add    dx, 0bh
        mov    al, bl
        out    dx, al
loc_1_0F53:
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 0ffffh
        je     SHORT loc_1_0F75
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_1_0F79
loc_1_0F75:
        mov    word ptr [ebp - 2], bx
loc_1_0F79:
        test   di, 0e000h
        jne    SHORT loc_1_0FCF
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_1_0F8C
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_1_0FCF
loc_1_0F8C:
        test   di, 1000h
        je     SHORT loc_1_0F98
        cmp    byte ptr [esi + 4eh], bl
        je     SHORT loc_1_0FCF
loc_1_0F98:
        cmp    dword ptr [esi + 75h], ebx
        jne    SHORT loc_1_0FCF
        cmp    byte ptr [esi + 8fh], bl
        je     SHORT loc_1_0FAB
        cmp    dword ptr [esi + 59h], 1
        je     SHORT loc_1_0FCF
loc_1_0FAB:
        cmp    byte ptr [esi + 86h], bl
        jne    NEAR PTR loc_1_109B
        cmp    dword ptr [esi + 87h], ebx
        je     NEAR PTR loc_1_109B
        cmp    byte ptr [esi + 85h], bl
        je     NEAR PTR loc_1_109B
loc_1_0FCF:
        mov    eax, dword ptr [esi + 59h]
        inc    eax
        mov    dword ptr [esi + 59h], eax
        cmp    byte ptr [esi + 8fh], bl
        je     SHORT loc_1_0FE7
        cmp    eax, 2
        je     NEAR PTR loc_1_1092
loc_1_0FE7:
        test   di, 4000h
        jne    SHORT loc_1_0FFA
        test   di, 1000h
        je     SHORT loc_1_1010
        cmp    byte ptr [esi + 4eh], bl
        jne    SHORT loc_1_1010
loc_1_0FFA:
        push   esi
        call   sub_1_1130
        cmp    al, bl
        je     SHORT loc_1_1010
        and    di, 0afffh
        lea    esp, [esp]
loc_1_1010:
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 0ffffh
        je     SHORT loc_1_1050
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        test   ah, 40h
        jne    SHORT loc_1_1050
        cmp    ax, bx
        je     SHORT loc_1_1050
        push   esi
        call   sub_1_1300
        test   di, 0a000h
        je     SHORT loc_1_1055
        test   di, 1000h
        jne    SHORT loc_1_1010
        jmp    SHORT loc_1_1055
loc_1_1050:
        and    di, 5fffh
loc_1_1055:
        cmp    byte ptr [esi + 86h], bl
        jne    SHORT loc_1_1073
        cmp    dword ptr [esi + 87h], ebx
        je     SHORT loc_1_1073
        cmp    byte ptr [esi + 85h], bl
        je     SHORT loc_1_1073
        push   esi
        call   sub_1_3430
loc_1_1073:
        push   esi
        call   sub_1_1730
        dec    dword ptr [esi + 59h]
        test   di, 1000h
        je     NEAR PTR loc_1_0ED1
        cmp    byte ptr [esi + 4eh], bl
        je     NEAR PTR loc_1_0ED1
        jmp    SHORT loc_1_109B
loc_1_1092:
        push   esi
        call   sub_1_29F0
        dec    dword ptr [esi + 59h]
loc_1_109B:
        mov    edi, dword ptr [esi + 0d9h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_10F9
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_10F9
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0444
        mov    eax, dword ptr [esi + 35h]
        or     eax, eax
        je     SHORT loc_1_10DA
        cmp    dword ptr [eax + 10h], 4
        jne    SHORT loc_1_10DA
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_10DA
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 35h], eax
        push   esi
        call   sub_1_1D50
loc_1_10DA:
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0
        push   edi
        call   sub_1_050C
        dec    dword ptr [esi + 0cdh]
loc_1_10F9:
        mov    byte ptr [esi + 50h], 0
        push   esi
        db  068h, 0C0h, 00Dh, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        cmp    byte ptr [esi + 4dh], 0
        je     SHORT loc_1_1125
        mov    byte ptr [esi + 4dh], 0
        mov    eax, dword ptr [esi + 166h]
        push   eax
        call   sub_1_0290
loc_1_1125:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_1130:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    ecx, dword ptr [esi + 3dh]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 16eh]
        mov    ebx, dword ptr [ecx]
loc_1_1150:
        mov    byte ptr [esi + 91h], 1
        mov    ax, word ptr [ebx]
        mov    word ptr [ebp - 2], ax
        test   ax, 0bfffh
        je     NEAR PTR loc_1_1252
        cmp    edi, 0ah
        ja     NEAR PTR loc_1_12A0
        test   ah, 20h
        jne    SHORT loc_1_11B5
        test   al, 80h
        je     SHORT loc_1_1182
        inc    dword ptr [esi + 0c3h]
        jmp    SHORT loc_1_118E
loc_1_1182:
        test   byte ptr [ebp - 2], 40h
        je     SHORT loc_1_118E
        inc    dword ptr [esi + 0c7h]
loc_1_118E:
        push   ebx
        push   esi
        call   sub_1_12C0
        mov    eax, dword ptr [esi + 3dh]
        cmp    dword ptr [esi + 25h], eax
        jne    SHORT loc_1_11A4
        mov    dword ptr [esi + 3dh], 0
loc_1_11A4:
        mov    ecx, dword ptr [esi + 3dh]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 16eh]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    SHORT loc_1_1150
loc_1_11B5:
        inc    dword ptr [esi + 9fh]
        cmp    byte ptr [esi + 8fh], 0
        jne    NEAR PTR loc_1_12AB
        mov    ax, word ptr [ebx + 16h]
        mov    word ptr [ebp - 2], ax
        and    eax, 3fffh
        lea    edx, [ebx + 8]
        mov    dword ptr [ebp - 8], edx
        or     edx, edx
        je     SHORT loc_1_1220
        or     eax, eax
        je     SHORT loc_1_1200
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 0ch], edx
        or     edx, edx
        je     SHORT loc_1_1220
        or     eax, eax
        je     SHORT loc_1_1200
        cmp    eax, 5dch
        ja     SHORT loc_1_1220
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        jmp    SHORT loc_1_1206
loc_1_1200:
        push   0
        push   0
        push   0
loc_1_1206:
        push   0eh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        mov    eax, dword ptr [ecx + 8]
        or     eax, 1
        push   eax
        mov    eax, dword ptr [esi + 166h]
        push   eax
        call   sub_1_00B0
loc_1_1220:
        push   ebx
        push   esi
        call   sub_1_12C0
        mov    byte ptr [esi + 4dh], 1
        mov    eax, dword ptr [esi + 3dh]
        inc    eax
        mov    dword ptr [esi + 3dh], eax
        cmp    dword ptr [esi + 25h], eax
        jne    SHORT loc_1_123E
        mov    dword ptr [esi + 3dh], 0
loc_1_123E:
        mov    ecx, dword ptr [esi + 3dh]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 16eh]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    NEAR PTR loc_1_1150
loc_1_1252:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc0h]
        and    ax, 40h
        cmp    ax, 1
        sbb    al, al
        neg    al
        or     al, al
        jne    SHORT loc_1_1280
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1280:
        mov    eax, dword ptr [esi + 3dh]
        shl    eax, 4
        add    eax, dword ptr [esi + 16eh]
        push   eax
        push   esi
        call   sub_1_2AF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_12A0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_12AB:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12C0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 41h]
        shl    eax, 4
        mov    ecx, dword ptr [edx + 16eh]
        mov    ecx, dword ptr [eax + ecx]
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax], 0
        mov    word ptr [eax + 2], 0c000h
        mov    word ptr [ecx + 2], 0
        mov    eax, dword ptr [edx + 41h]
        inc    eax
        mov    dword ptr [edx + 41h], eax
        cmp    dword ptr [edx + 25h], eax
        jne    SHORT loc_1_12FC
        mov    dword ptr [edx + 41h], 0
loc_1_12FC:
        pop    ebp
        ret    8
sub_1_1300:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, 1
        mov    eax, dword ptr [esi + 6dh]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    edx, dword ptr [esi + 16ah]
        mov    eax, dword ptr [eax + edx]
        mov    dx, word ptr [eax]
        mov    word ptr [ebp - 2], dx
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebp - 8], ax
        and    dword ptr [ebp - 8], 7
        jne    SHORT loc_1_1352
        push   0
        push   0aa00000bh
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1352:
        mov    byte ptr [esi + 90h], 1
        cmp    dword ptr [ebp - 8], 4
        je     SHORT loc_1_13A0
        mov    edi, dword ptr [esi + 6dh]
        push   esi
        call   sub_1_14C0
        mov    eax, dword ptr [esi + 6dh]
        push   eax
        push   esi
        call   sub_1_1520
        lea    eax, [edi + edi*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    eax, dword ptr [eax + ecx + 0ch]
        mov    dword ptr [esi + 6dh], eax
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_1_14AF
        mov    dword ptr [esi + 71h], 0ffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_13A0:
        mov    edx, dword ptr [esi + 6dh]
        test   byte ptr [ebp - 1], 20h
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    edx, dword ptr [edx + eax + 10h]
        lea    edi, [edx + 24h]
        je     SHORT loc_1_13F0
        inc    dword ptr [esi + 9bh]
        test   byte ptr [ebp - 2], 80h
        je     SHORT loc_1_13D0
        inc    dword ptr [esi + 0b7h]
        jmp    SHORT loc_1_142E
loc_1_13D0:
        mov    ax, word ptr [ebp - 2]
        and    ax, 0fh
        cmp    ax, 1
        jne    SHORT loc_1_13E6
        inc    dword ptr [esi + 0bbh]
        jmp    SHORT loc_1_142E
loc_1_13E6:
        jbe    SHORT loc_1_142E
        inc    dword ptr [esi + 0bfh]
        jmp    SHORT loc_1_142E
loc_1_13F0:
        test   byte ptr [ebp - 2], 20h
        je     SHORT loc_1_1400
        inc    dword ptr [esi + 0a7h]
        jmp    SHORT loc_1_142C
        db  08Bh, 0FFh
loc_1_1400:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_1410
        inc    dword ptr [esi + 0abh]
        jmp    SHORT loc_1_142C
        db  08Bh, 0FFh
loc_1_1410:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_1420
        inc    dword ptr [esi + 0b3h]
        jmp    SHORT loc_1_142C
        db  08Bh, 0FFh
loc_1_1420:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_142C
        inc    dword ptr [esi + 0afh]
loc_1_142C:
        sub    cl, cl
loc_1_142E:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_143D
        mov    dword ptr [esi + 81h], 0
loc_1_143D:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 7dh], eax
        cmp    byte ptr [edi + 0dh], 0
        jne    SHORT loc_1_1465
        mov    edi, dword ptr [edi + 4]
        cmp    cl, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 14h]
        jmp    SHORT loc_1_1482
loc_1_1465:
        mov    byte ptr [edi + 0ch], cl
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_1473
        mov    dword ptr [esi + 75h], edx
        jmp    SHORT loc_1_1479
loc_1_1473:
        mov    eax, dword ptr [esi + 79h]
        mov    dword ptr [eax + 24h], edx
loc_1_1479:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 79h], edx
loc_1_1482:
        mov    eax, dword ptr [esi + 6dh]
        push   eax
        push   esi
        call   sub_1_1520
        mov    eax, dword ptr [esi + 6dh]
        push   eax
        push   esi
        call   sub_1_0A30
        cmp    byte ptr [esi + 85h], 0
        jne    SHORT loc_1_14AF
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_14AF
        mov    byte ptr [esi + 85h], 1
loc_1_14AF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 6dh]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [esi + 16ah]
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        jne    SHORT loc_1_1510
        mov    eax, dword ptr [esi + 35h]
        or     eax, eax
        je     SHORT loc_1_1518
        mov    edi, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 35h], ecx
        push   0
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_045C
        dec    dword ptr [edi + 14h]
        push   esi
        call   sub_1_1D50
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1510:
        cmp    eax, 1
        je     SHORT loc_1_1518
        dec    dword ptr [eax + 14h]
loc_1_1518:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1520:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 16ah]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    ecx, eax
        mov    eax, dword ptr [ecx + 0ch]
        cmp    eax, 0ffffh
        je     SHORT loc_1_158B
        mov    byte ptr [ecx + 18h], 0
        mov    dword ptr [esi + 45h], eax
        sub    edi, edi
        sub    ebx, ebx
loc_1_1552:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_1579
        push   32h
        call   sub_1_0500
        inc    edi
        cmp    edi, 4e20h
        jbe    SHORT loc_1_1552
loc_1_1579:
        push   esi
        db  068h, 0A0h, 006h, 000h, 000h
        add    esi, 131h
        push   esi
        call   sub_1_04FA
loc_1_158B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    al, al
        cmp    byte ptr [ecx + 50h], al
        jne    SHORT loc_1_15E2
        cmp    byte ptr [ecx + 8fh], al
        jne    SHORT loc_1_15E2
        cmp    byte ptr [ecx + 90h], al
        jne    SHORT loc_1_15E2
        cmp    byte ptr [ecx + 91h], al
        jne    SHORT loc_1_15E2
        mov    al, byte ptr [ecx + 92h]
        cmp    al, 0ah
        lea    edx, [eax + 1]
        mov    byte ptr [ecx + 92h], dl
        jae    SHORT loc_1_15E0
        sub    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_15E0:
        mov    al, 1
loc_1_15E2:
        mov    byte ptr [ecx + 92h], 0
        mov    byte ptr [ecx + 90h], 0
        mov    byte ptr [ecx + 91h], 0
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1600:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 0ffffh
        je     NEAR PTR loc_1_16BF
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [esi + 16ah]
        cmp    byte ptr [eax + 18h], 0
        je     NEAR PTR loc_1_16BB
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [esi + 6dh]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    eax, dword ptr [eax + ecx]
        mov    ax, word ptr [eax]
        test   ah, 40h
        jne    SHORT loc_1_1670
        or     ax, ax
        je     SHORT loc_1_1670
        cmp    byte ptr [esi + 58h], 0
        jne    SHORT loc_1_1670
        mov    byte ptr [esi + 57h], 1
        mov    byte ptr [esi + 58h], 1
        push   1
        lea    eax, [esi + 155h]
        push   eax
        call   sub_1_04E2
        jmp    SHORT loc_1_16BF
loc_1_1670:
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_16BF
        mov    byte ptr [esi + 53h], 1
        push   0
        push   esi
        call   sub_1_2EA0
        cmp    byte ptr [esi + 50h], 0
        jne    SHORT loc_1_16BF
        mov    al, byte ptr [esi + 1ch]
        and    al, 0fbh
        mov    byte ptr [esi + 1ch], al
        inc    dword ptr [esi + 59h]
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        push   esi
        call   sub_1_29F0
        dec    dword ptr [esi + 59h]
        push   esi
        db  068h, 0C0h, 00Dh, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        jmp    SHORT loc_1_16BF
loc_1_16BB:
        mov    byte ptr [eax + 18h], 1
loc_1_16BF:
        push   esi
        call   sub_1_15A0
        or     al, al
        je     SHORT loc_1_1709
        mov    byte ptr [esi + 53h], 1
        push   0
        push   esi
        call   sub_1_2EA0
        mov    cl, byte ptr [esi + 1ch]
        and    cl, 0fbh
        mov    byte ptr [esi + 1ch], cl
        inc    dword ptr [esi + 59h]
        mov    di, word ptr [esi]
        add    di, 6
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   esi
        call   sub_1_29F0
        dec    dword ptr [esi + 59h]
        push   esi
        db  068h, 0C0h, 00Dh, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
loc_1_1709:
        push   3e8h
        add    esi, 149h
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1730:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 75h]
        or     esi, esi
        je     NEAR PTR loc_1_181A
        lea    edi, [esi + 24h]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_1756
        mov    dword ptr [ebx + 79h], 0
loc_1_1756:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 75h], eax
        mov    byte ptr [ebx + 4dh], 1
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_0530
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 4], 100h
        jae    SHORT loc_1_17BA
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_1_17BA
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebx + 172h]
        push   eax
        push   100h
        push   0
        push   esi
        call   sub_1_1870
        lea    eax, [ebx + 172h]
        mov    dword ptr [ebp - 8], eax
loc_1_17BA:
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_17D2
        cmp    dword ptr [ebp - 4], 5
        jbe    SHORT loc_1_17FB
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    SHORT loc_1_17E9
loc_1_17D2:
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0eh
        push   eax
        push   0eh
loc_1_17E9:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 166h]
        push   eax
        call   sub_1_00B0
loc_1_17FB:
        cmp    byte ptr [edi + 0ch], 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_044A
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
loc_1_181A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [edx + 24h]
        mov    eax, dword ptr [ecx + 81h]
        or     eax, eax
        je     SHORT loc_1_184A
        mov    dword ptr [eax + 24h], edx
loc_1_184A:
        mov    dword ptr [esi], 0
        mov    dword ptr [ecx + 81h], edx
        cmp    dword ptr [ecx + 7dh], 0
        jne    SHORT loc_1_185F
        mov    dword ptr [ecx + 7dh], edx
loc_1_185F:
        dec    dword ptr [ecx + 0a3h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1870:
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
        je     NEAR PTR loc_1_193A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_193A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_1935
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_18C0:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_18EC
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1935
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    SHORT loc_1_1930
loc_1_18EC:
        or     edi, edi
        je     SHORT loc_1_1909
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_1901
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1930
loc_1_1901:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1909:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_1917
        mov    ebx, eax
loc_1_1917:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    esi, ebx
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1930:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_1_18C0
loc_1_1935:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_193A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1950:
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
        je     NEAR PTR loc_1_1A1B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_1_1A1B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_1A16
        mov    edi, dword ptr [ebp + 14h]
loc_1_19A4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_19D0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1A16
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    SHORT loc_1_1A11
loc_1_19D0:
        or     edi, edi
        je     SHORT loc_1_19ED
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_19E5
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1A11
loc_1_19E5:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_19ED:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, esi
        mov    ebx, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_19FB
        mov    ebx, eax
loc_1_19FB:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    esi, ebx
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1A11:
        cmp    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_19A4
loc_1_1A16:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_1A1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 8fh], 0
        je     SHORT loc_1_1A4B
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_1A4B:
        mov    cl, byte ptr [ebp + 18h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_1_1A80
        or     al, al
        je     SHORT loc_1_1A6B
        mov    eax, 103h
        pop    ebp
        ret    14h
loc_1_1A6B:
        sub    eax, eax
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1A80:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    al, al
        mov    dx, 2
        mov    ecx, dword ptr [ebp + 10h]
        test   cl, 24h
        je     SHORT loc_1_1A98
        mov    dx, 3
        jmp    SHORT loc_1_1AA0
loc_1_1A98:
        test   cl, 8
        je     SHORT loc_1_1AA0
        sub    dx, dx
loc_1_1AA0:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 0cbh], dx
        je     NEAR PTR loc_1_1B46
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [ecx + 0cbh], dx
        mov    word ptr [eax + 2], 2
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 6], 60ch
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 8], 2600h
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 0ah], 6000h
        mov    eax, dword ptr [ecx + 1dh]
        cmp    byte ptr [ebp + 18h], 0
        mov    word ptr [eax + 0ch], 0f200h
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 0eh], dx
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 10h], 40h
        jne    SHORT loc_1_1B23
        mov    esi, dword ptr [ecx + 21h]
        mov    edx, dword ptr [ecx + 16ah]
        lea    esi, [esi + esi*4]
        mov    eax, dword ptr [ebp + 14h]
        lea    esi, [esi + esi*4]
        mov    dword ptr [edx + esi + 14h], eax
        mov    byte ptr [ecx + 52h], 1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    14h
loc_1_1B23:
        mov    edx, dword ptr [ecx + 21h]
        mov    eax, dword ptr [ecx + 16ah]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 14h], 0
        mov    eax, dword ptr [ecx + 21h]
        push   eax
        push   ecx
        call   sub_1_0860
        mov    al, 1
loc_1_1B46:
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_1B50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 8fh], 0
        je     SHORT loc_1_1B6B
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_1B6B:
        mov    cl, byte ptr [ebp + 20h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edx
        call   sub_1_1B90
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_1_1BCD
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
loc_1_1BAD:
        sub    edi, edi
        nop
loc_1_1BB0:
        lea    edx, [eax + edi]
        mov    dl, byte ptr [esi + edx]
        mov    ebx, dword ptr [ecx + 1dh]
        add    ebx, edi
        mov    byte ptr [esi + ebx + 8], dl
        inc    edi
        cmp    edi, 6
        jb     SHORT loc_1_1BB0
        add    esi, 6
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_1BAD
loc_1_1BCD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 1dh]
        imul   ax, ax, 6
        mov    word ptr [edx + 6], ax
        mov    eax, dword ptr [ecx + 1dh]
        cmp    byte ptr [ebp + 18h], 0
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 2], 3
        jne    SHORT loc_1_1C15
        mov    esi, dword ptr [ecx + 21h]
        mov    edx, dword ptr [ecx + 16ah]
        lea    esi, [esi + esi*4]
        mov    eax, dword ptr [ebp + 14h]
        lea    esi, [esi + esi*4]
        mov    dword ptr [edx + esi + 14h], eax
        mov    byte ptr [ecx + 52h], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1C15:
        mov    edx, dword ptr [ecx + 21h]
        mov    eax, dword ptr [ecx + 16ah]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 14h], 0
        cmp    byte ptr [ecx + 53h], 0
        je     SHORT loc_1_1C41
        push   ecx
        call   sub_1_0930
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1C41:
        mov    eax, dword ptr [ecx + 21h]
        push   eax
        push   ecx
        call   sub_1_0860
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 14h]
        ret    4
        db  08Dh, 049h, 000h
loc_1_1C70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_1CC7
        cmp    byte ptr [eax + 18h], 0
        jne    SHORT loc_1_1CC0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 10h]
        or     edx, edx
        jl     SHORT loc_1_1C9C
        cmp    edx, 1
        jle    SHORT loc_1_1CA3
loc_1_1C9C:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_1CCC
loc_1_1CA3:
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0
        push   ecx
        mov    edi, 103h
        push   esi
        call   sub_1_1D10
        jmp    SHORT loc_1_1CCC
        db  08Dh, 049h, 000h
loc_1_1CC0:
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_1CCC
loc_1_1CC7:
        mov    edi, 0c001000dh
loc_1_1CCC:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_1D04
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_1CF6
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_1CF6
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_1D04
loc_1_1CF6:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_1D04:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_1D10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 35h], 0
        je     SHORT loc_1_1D30
        mov    edx, dword ptr [eax + 39h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 39h], ecx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D30:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 35h], ecx
        mov    dword ptr [eax + 39h], ecx
        push   eax
        call   sub_1_1D50
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 51h], 0
        jne    NEAR PTR loc_1_1F70
        mov    byte ptr [esi + 51h], 1
        mov    edi, dword ptr [esi + 35h]
        or     edi, edi
        je     NEAR PTR loc_1_1F6C
loc_1_1D75:
        cmp    byte ptr [esi + 8fh], 0
        jne    NEAR PTR loc_1_1F6C
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_1DA6
        cmp    eax, 1
        je     SHORT loc_1_1DCC
        cmp    eax, 2
        je     SHORT loc_1_1DF2
        cmp    eax, 3
        je     SHORT loc_1_1E16
        cmp    eax, 4
        je     NEAR PTR loc_1_1E92
        jmp    NEAR PTR loc_1_1F15
loc_1_1DA6:
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_2100
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1F15
loc_1_1DCC:
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_1F90
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1F15
loc_1_1DF2:
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 14h]
        push   eax
        push   0
        push   esi
        call   sub_1_2100
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1F15
loc_1_1E16:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax + 166h]
        push   eax
        db  0E8h, 03Fh, 0E3h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_1E51
        push   0
        push   0c0000001h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_043E
        push   0
        push   0
        push   ebx
        call   sub_1_050C
        jmp    SHORT loc_1_1E86
loc_1_1E51:
        lea    eax, [esi + 0d1h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0d5h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0d5h]
        push   0
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0d5h], ebx
        inc    dword ptr [esi + 0cdh]
        inc    dword ptr [ebx + 14h]
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_043E
loc_1_1E86:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1F15
loc_1_1E92:
        mov    byte ptr [esi + 52h], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 166h]
        push   eax
        db  0E8h, 095h, 0E3h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_1_1EB7
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_1ED0
loc_1_1EB7:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_1ED0
        cmp    dword ptr [ebp - 4], 0c001000eh
        jne    SHORT loc_1_1ED0
        mov    dword ptr [ebp - 4], 103h
loc_1_1ED0:
        cmp    byte ptr [esi + 52h], 0
        je     SHORT loc_1_1EE0
        mov    eax, dword ptr [esi + 21h]
        push   eax
        push   esi
        call   sub_1_0860
loc_1_1EE0:
        mov    byte ptr [ebx + 18h], 1
        dec    dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0d9h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0ddh]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0ddh]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0ddh], ebx
loc_1_1F15:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_1F79
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 35h], eax
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 2
        jne    SHORT loc_1_1F44
        inc    dword ptr [esi + 59h]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        call   sub_1_047A
        dec    dword ptr [esi + 59h]
        jmp    SHORT loc_1_1F61
loc_1_1F44:
        or     eax, eax
        je     SHORT loc_1_1F4D
        cmp    eax, 1
        jne    SHORT loc_1_1F61
loc_1_1F4D:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 14h]
loc_1_1F61:
        mov    edi, dword ptr [esi + 35h]
        or     edi, edi
        jne    NEAR PTR loc_1_1D75
loc_1_1F6C:
        mov    byte ptr [esi + 51h], 0
loc_1_1F70:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1F79:
        mov    byte ptr [esi + 51h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010eh
        mov    dword ptr [edi], 0
        je     SHORT loc_1_1FD9
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_2053
        cmp    eax, 10112h
        je     NEAR PTR loc_1_2066
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_20A5
        mov    eax, 0c0010017h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1FD9:
        mov    esi, dword ptr [ebp + 18h]
        cmp    esi, 4
        je     SHORT loc_1_1FF4
        mov    dword ptr [edi], 4
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1FF4:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 4], 0f050h
        je     SHORT loc_1_2028
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c00000bbh
        mov    dword ptr [ecx], 4
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2028:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 166h]
        push   eax
        db  0E8h, 09Ah, 0E5h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2053:
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2066:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_2080
        mov    dword ptr [edi], 4
        mov    eax, 0c0010014h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_1_2080:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 41h
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_20A5:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, esi
        cdq
        idiv   ecx
        or     edx, edx
        je     SHORT loc_1_20C5
        mov    dword ptr [edi], ecx
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_20C5:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq
        idiv   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 166h]
        push   eax
        db  0E8h, 012h, 0E2h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_2100:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 1ch], 0
        lea    esi, [ebp - 8]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [eax], 0
        mov    ecx, 4
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [eax], 0
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, edx
        cmp    eax, 1010101h
        ja     SHORT loc_1_2160
        je     NEAR PTR loc_1_2553
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_2181
        shl    eax, 2
        db  0FFh, 0A0h, 07Ch, 026h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_2160:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2578
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_25A0
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2630
loc_1_2181:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_2193
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2637
loc_1_2193:
        cmp    edx, 1020102h
        ja     SHORT loc_1_2210
        je     NEAR PTR loc_1_2364
        cmp    edx, 20105h
        ja     SHORT loc_1_21E4
        je     NEAR PTR loc_1_2314
        cmp    edx, 20101h
        je     NEAR PTR loc_1_2274
        cmp    edx, 20102h
        je     NEAR PTR loc_1_2285
        cmp    edx, 20103h
        je     NEAR PTR loc_1_2296
        cmp    edx, 20104h
        je     NEAR PTR loc_1_22B9
        jmp    NEAR PTR loc_1_2268
loc_1_21E4:
        cmp    edx, 2020dh
        je     NEAR PTR loc_1_2327
        cmp    edx, 2020eh
        je     NEAR PTR loc_1_2340
        cmp    edx, 1020101h
        je     NEAR PTR loc_1_2351
        jmp    SHORT loc_1_2268
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2210:
        cmp    edx, 1020203h
        ja     SHORT loc_1_2244
        je     NEAR PTR loc_1_23A8
        cmp    edx, 1020103h
        je     NEAR PTR loc_1_2375
        cmp    edx, 1020201h
        je     NEAR PTR loc_1_2386
        cmp    edx, 1020202h
        je     NEAR PTR loc_1_2397
        jmp    SHORT loc_1_2268
loc_1_2244:
        cmp    edx, 1020204h
        je     NEAR PTR loc_1_23C0
        cmp    edx, 1020205h
        je     NEAR PTR loc_1_23D1
        cmp    edx, 1020206h
        je     NEAR PTR loc_1_23E2
loc_1_2268:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2637
loc_1_2274:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 9bh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2285:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 9fh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2296:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0afh]
        add    eax, dword ptr [edi + 0a7h]
        add    eax, dword ptr [edi + 0abh]
        add    eax, dword ptr [edi + 0b3h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_22B9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 2], ax
        and    eax, 0ffffh
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 2], ax
        and    eax, 0ffffh
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 2], ax
        and    eax, 0ffffh
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 0c3h]
        add    eax, dword ptr [edi + 0c7h]
        add    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2314:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2637
loc_1_2327:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2637
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2340:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a3h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2351:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2637
loc_1_2364:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0bbh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2375:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0bfh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2386:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0b7h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2397:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a7h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_23A8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2637
        db  005h, 000h, 000h, 000h, 000h
loc_1_23C0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0afh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_23D1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0b3h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_23E2:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0abh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_23F3:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_2408
        db  0BEh, 05Ch, 036h, 000h, 000h
        mov    ecx, 0a0h
        jmp    NEAR PTR loc_1_2637
loc_1_2408:
        db  0BEh, 0FCh, 036h, 000h, 000h
        mov    ecx, 5ch
        jmp    NEAR PTR loc_1_2637
loc_1_2417:
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 8fh], 0
        je     SHORT loc_1_2430
        mov    dword ptr [ebp - 10h], 2
        jmp    SHORT loc_1_2444
        db  08Dh, 064h, 024h, 000h
loc_1_2430:
        cmp    byte ptr [edi + 53h], 0
        mov    dword ptr [ebp - 10h], 1
        jne    SHORT loc_1_2444
        mov    dword ptr [ebp - 10h], 0
loc_1_2444:
        lea    esi, [ebp - 10h]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_2637
loc_1_2451:
        lea    esi, [ebp - 1ch]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_2637
        db  08Bh, 0FFh
loc_1_2460:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2637
        db  08Dh, 064h, 024h, 000h
loc_1_2470:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2637
        db  08Dh, 064h, 024h, 000h
loc_1_2480:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 21h]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
        db  08Bh, 0FFh
loc_1_24A0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 25h]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
        db  08Bh, 0FFh
loc_1_24C0:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2637
        db  08Dh, 064h, 024h, 000h
loc_1_24D0:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 29h
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    esi, [ebp - 8]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_2637
loc_1_24F6:
        db  0BEh, 058h, 037h, 000h, 000h
        mov    ecx, 0fh
        jmp    NEAR PTR loc_1_2637
loc_1_2505:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_2520
        mov    eax, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2520:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 166h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2637
loc_1_2534:
        mov    word ptr [ebp - 4], 300h
        lea    esi, [ebp - 4]
        mov    ecx, 2
        jmp    NEAR PTR loc_1_2637
loc_1_2547:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_2637
loc_1_2553:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 29h]
        add    edi, 29h
        mov    dword ptr [ebp - 28h], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 24h], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 28h]
        mov    byte ptr [ebp - 23h], al
        mov    ecx, 6
        jmp    NEAR PTR loc_1_2637
loc_1_2578:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 2fh]
        add    edi, 2fh
        mov    dword ptr [ebp - 28h], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 24h], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 28h]
        mov    byte ptr [ebp - 23h], al
        mov    ecx, 6
        jmp    NEAR PTR loc_1_2637
        db  08Dh, 049h, 000h
loc_1_25A0:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_25D8
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 166h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 09Ah, 0E1h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp + 14h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_2637
loc_1_25D8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 166h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        db  0E8h, 0A5h, 0E0h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 18h], 0
        mov    esi, dword ptr [ebp + 14h]
        jne    SHORT loc_1_2613
        mov    eax, dword ptr [ebp - 14h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_2637
loc_1_2613:
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 166h]
        push   eax
        db  0E8h, 03Dh, 0E0h, 0FEh, 0FFh
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_2637
        db  090h
loc_1_2630:
        mov    dword ptr [ebp - 8], 10h
loc_1_2637:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_266E
        cmp    dword ptr [ebp + 18h], ecx
        jae    SHORT loc_1_265A
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010016h
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_265A:
        mov    eax, dword ptr [ebp + 1ch]
        or     ecx, ecx
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_266E
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_05EC
loc_1_266E:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_267C:
        and    eax, dword ptr [eax]
        db  000h, 017h
        and    al, 0
        db  000h, 051h, 024h
        add    byte ptr [eax], al
        push   ecx
        and    al, 0
        db  000h, 060h, 024h
        add    byte ptr [eax], al
        pushad
        and    al, 0
        db  000h, 070h, 024h
        add    byte ptr [eax], al
        and    byte ptr [eax + eax], 0
        db  0A0h, 024h, 000h, 000h, 0C0h
        and    al, 0
        db  000h, 0C0h
        and    al, 0
        db  000h, 0D0h
        and    al, 0
        db  000h, 0F6h
        and    al, 0
        db  000h, 005h, 025h, 000h, 000h, 060h
        and    al, 0
        db  000h, 034h, 025h, 000h, 000h, 0C0h, 024h
        add    byte ptr [eax], al
        db  081h, 021h, 000h, 000h, 047h, 025h
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
loc_1_26D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_2710
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 2
        je     SHORT loc_1_26F4
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_2715
loc_1_26F4:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        push   eax
        push   esi
        call   sub_1_1D10
        mov    edi, 103h
        jmp    SHORT loc_1_2715
        db  090h
loc_1_2710:
        mov    edi, 0c001000dh
loc_1_2715:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_274D
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_273F
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_273F
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_274D
loc_1_273F:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_274D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2760:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [esi + 1ah]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    eax, dword ptr [esi + 49h]
        mov    word ptr [ecx + 0ffc6h], ax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    word ptr [eax + 0ffc2h], 10h
        mov    dx, word ptr [esi]
        add    dx, 0bh
        sub    al, al
        out    dx, al
        mov    byte ptr [esi + 4eh], 1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 1ah]
        mov    ecx, dword ptr [eax + 2]
        sub    ecx, edx
        pop    ebp
        mov    word ptr [ecx + 0ffc2h], 440h
        mov    dx, word ptr [eax]
        add    dx, 0bh
        sub    al, al
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 1ah]
        mov    ecx, dword ptr [eax + 2]
        sub    ecx, edx
        pop    ebp
        mov    word ptr [ecx + 0ffc2h], 80h
        mov    dx, word ptr [eax]
        add    dx, 0bh
        sub    al, al
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 103h
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 59h]
        cmp    byte ptr [edi + 8fh], 0
        jne    SHORT loc_1_2888
        cmp    byte ptr [ebx + 18h], 0
        jne    SHORT loc_1_2842
        inc    dword ptr [ebx + 14h]
        push   ebx
        push   edi
        call   sub_1_2EA0
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_2847
loc_1_2842:
        mov    esi, 0c0010002h
loc_1_2847:
        mov    eax, dword ptr [edi + 59h]
        dec    eax
        mov    dword ptr [edi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_287F
        cmp    byte ptr [edi + 8fh], 0
        jne    SHORT loc_1_2871
        cmp    dword ptr [edi + 75h], 0
        jne    SHORT loc_1_2871
        mov    eax, dword ptr [edi + 0d9h]
        sub    eax, edi
        cmp    eax, 0d9h
        je     SHORT loc_1_287F
loc_1_2871:
        push   0
        add    edi, 155h
        push   edi
        call   sub_1_04E2
loc_1_287F:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2888:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_28A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 1dh]
        add    ecx, 6
        mov    edi, 0
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    word ptr [esi + 0cbh], 2
        mov    eax, dword ptr [esi + 166h]
        or     eax, eax
        je     SHORT loc_1_28E0
        mov    edi, dword ptr [eax + 0ch]
loc_1_28E0:
        test   edi, 20h
        je     SHORT loc_1_28F3
        mov    word ptr [esi + 0cbh], 3
        jmp    SHORT loc_1_2917
loc_1_28F3:
        test   edi, 4
        je     SHORT loc_1_2906
        mov    word ptr [esi + 0cbh], 3
        jmp    SHORT loc_1_2917
loc_1_2906:
        test   edi, 8
        je     SHORT loc_1_2917
        mov    word ptr [esi + 0cbh], 0
loc_1_2917:
        cmp    byte ptr [esi + 56h], 0
        jne    SHORT loc_1_2926
        or     word ptr [esi + 0cbh], 8800h
loc_1_2926:
        mov    word ptr [ecx], 60ch
        mov    word ptr [ecx + 2], 2600h
        mov    word ptr [ecx + 4], 6000h
        mov    word ptr [ecx + 6], 0f200h
        mov    word ptr [ecx + 0ah], 40h
        mov    ax, word ptr [esi + 0cbh]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esi + 1dh]
        push   esi
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 2], 2
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 4], 0ffffh
        call   sub_1_0930
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 2], 1
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 4], 0ffffh
        sub    eax, eax
        lea    ebx, [ebx]
loc_1_2990:
        mov    cl, byte ptr [eax + esi + 2fh]
        mov    edx, dword ptr [esi + 1dh]
        mov    byte ptr [eax + edx + 6], cl
        inc    eax
        cmp    eax, 6
        jb     SHORT loc_1_2990
        push   esi
        call   sub_1_0930
        test   edi, 2
        je     SHORT loc_1_29E1
        lea    edi, [esi + 272h]
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 166h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 094h, 0DDh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_29E1
        push   1
        push   1
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_1B90
loc_1_29E1:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_29F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 4]
loc_1_2A01:
        or     edi, edi
        je     SHORT loc_1_2A11
        cmp    edi, 1
        je     SHORT loc_1_2A16
        cmp    edi, 2
        je     SHORT loc_1_2A20
        jmp    SHORT loc_1_2A26
loc_1_2A11:
        mov    ebx, dword ptr [esi + 75h]
        jmp    SHORT loc_1_2A26
loc_1_2A16:
        mov    ebx, dword ptr [esi + 7dh]
        jmp    SHORT loc_1_2A26
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A20:
        mov    ebx, dword ptr [esi + 87h]
loc_1_2A26:
        or     ebx, ebx
        je     SHORT loc_1_2A4A
loc_1_2A2A:
        lea    eax, [ebx + 24h]
        mov    ecx, ebx
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   0c001000ch
        dec    dword ptr [eax + 14h]
        push   ecx
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_044A
        or     ebx, ebx
        jne    SHORT loc_1_2A2A
loc_1_2A4A:
        inc    edi
        cmp    edi, 3
        jb     SHORT loc_1_2A01
        push   esi
        call   sub_1_2A60
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_2A60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0AB0
        push   esi
        call   sub_1_2BD0
        mov    bl, al
        or     bl, bl
        je     SHORT loc_1_2AD2
        push   esi
        call   sub_1_2FB0
        push   esi
        call   sub_1_28A0
        mov    cl, byte ptr [esi + 1ch]
        or     cl, 4
        mov    byte ptr [esi + 1ch], cl
        mov    di, word ptr [esi]
        add    di, 6
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   0
        push   esi
        call   sub_1_2D50
        cmp    byte ptr [esi + 53h], 0
        jne    SHORT loc_1_2AC5
        push   1388h
        add    esi, 149h
        push   esi
        call   sub_1_04E2
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2AC5:
        mov    byte ptr [esi + 53h], 0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2AD2:
        push   0c0000001h
        push   esi
        call   sub_1_2D50
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2AF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 4fh], 0
        je     SHORT loc_1_2B79
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    dword ptr [esi + 49h], eax
        sub    ebx, ebx
loc_1_2B15:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_2B3C
        push   32h
        call   sub_1_0500
        inc    edi
        cmp    edi, 4e20h
        jbe    SHORT loc_1_2B15
loc_1_2B3C:
        push   esi
        db  068h, 060h, 027h, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        sub    edi, edi
        sub    ebx, ebx
loc_1_2B52:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_2B79
        push   32h
        call   sub_1_0500
        inc    edi
        cmp    edi, 4e20h
        jbe    SHORT loc_1_2B52
loc_1_2B79:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 53h], 0
        jne    SHORT loc_1_2BB2
        push   esi
        db  068h, 0B0h, 027h, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
loc_1_2BB2:
        mov    dx, word ptr [esi]
        add    dx, 6
        in     al, dx
        mov    dx, word ptr [esi]
        add    dx, 6
        and    al, 0fbh
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2BD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2F40
        cmp    byte ptr [esi + 4fh], 0
        jne    SHORT loc_1_2C34
        mov    eax, dword ptr [esi + 0ah]
        shl    eax, 0ah
        push   eax
        mov    eax, dword ptr [esi + 6]
        push   eax
        mov    eax, dword ptr [esi + 0f1h]
        lea    ecx, [esi + 2]
        push   eax
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2C30
        push   0
        push   1
        push   2
        push   0aa000001h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        call   sub_1_05D8
        add    esp, 14h
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2C30:
        mov    byte ptr [esi + 4fh], 1
loc_1_2C34:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0fff4h
        mov    dword ptr [esi + 0eh], eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0ffe0h
        mov    dword ptr [esi + 12h], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 1ah]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, eax
        add    ecx, 0ffc0h
        mov    dword ptr [esi + 16h], ecx
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0ff58h
        mov    dword ptr [esi + 1dh], eax
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [esi + 61h], eax
        mov    ecx, dword ptr [esi + 21h]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 3
        lea    ecx, [edx + ecx*8]
        lea    ecx, [eax + ecx*8]
        mov    eax, dword ptr [esi + 0eh]
        mov    dword ptr [esi + 5dh], ecx
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [esi + 0eh]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [esi + 0eh]
        mov    word ptr [eax + 8], 0ffe0h
        mov    eax, dword ptr [esi + 12h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 12h]
        mov    word ptr [eax + 2], 0ffc0h
        mov    eax, dword ptr [esi + 12h]
        mov    word ptr [eax], 1
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 4], 0ffffh
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 6], 0ffffh
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [esi + 16h]
        mov    word ptr [eax + 0eh], 0
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, 80h
        out    dx, al
        push   3e8h
        call   sub_1_0500
        mov    si, word ptr [esi]
        mov    al, 4
        lea    ecx, [esi + 6]
        mov    dx, cx
        out    dx, al
        mov    al, 84h
        out    dx, al
        lea    edx, [esi + 0bh]
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2D50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 93h]
        mov    ebx, dword ptr [esi + 0d1h]
        sub    eax, ebx
        cmp    eax, 0ffffff2fh
        je     SHORT loc_1_2DB8
loc_1_2D70:
        inc    dword ptr [ebx + 14h]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_2D8B
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_046E
loc_1_2D8B:
        push   4
        lea    eax, [ebp + 0ch]
        push   eax
        push   40010005h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0474
        dec    dword ptr [ebx + 14h]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0ffffff2fh
        jne    SHORT loc_1_2D70
loc_1_2DB8:
        or     edi, edi
        je     SHORT loc_1_2DCC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0456
        dec    dword ptr [edi + 14h]
loc_1_2DCC:
        mov    byte ptr [esi + 8fh], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_2DF9
        mov    eax, dword ptr [esi + 3dh]
        shl    eax, 4
        add    eax, dword ptr [esi + 16eh]
        push   eax
        push   esi
        call   sub_1_2AF0
        push   esi
        call   sub_1_1D50
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2DF9:
        push   1
        push   esi
        call   sub_1_2E10
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2E10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 35h]
        add    esi, 35h
        or     edi, edi
        je     SHORT loc_1_2E96
        cmp    dword ptr [edi + 10h], 4
        jne    SHORT loc_1_2E35
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 14h]
        mov    edi, dword ptr [esi]
loc_1_2E35:
        or     edi, edi
        je     SHORT loc_1_2E96
loc_1_2E39:
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 4
        jne    SHORT loc_1_2E45
        mov    esi, edi
        jmp    SHORT loc_1_2E90
loc_1_2E45:
        cmp    eax, 3
        jne    SHORT loc_1_2E77
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_2E73
        mov    ebx, dword ptr [edi + 4]
        push   0
        push   0c0000001h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_043E
        push   0
        push   0
        push   ebx
        call   sub_1_050C
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_2E90
loc_1_2E73:
        mov    esi, edi
        jmp    SHORT loc_1_2E90
loc_1_2E77:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    ebx, dword ptr [edi + 4]
        push   0c001000ch
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 14h]
loc_1_2E90:
        mov    edi, dword ptr [esi]
        or     edi, edi
        jne    SHORT loc_1_2E39
loc_1_2E96:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_2EA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 149h]
        push   eax
        call   sub_1_04E8
        push   esi
        call   sub_1_2B90
        mov    byte ptr [esi + 1ch], 84h
        mov    edi, dword ptr [esi + 0d1h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff2fh
        je     SHORT loc_1_2F04
loc_1_2ED6:
        inc    dword ptr [edi + 14h]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0474
        dec    dword ptr [edi + 14h]
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff2fh
        jne    SHORT loc_1_2ED6
loc_1_2F04:
        mov    byte ptr [esi + 8fh], 1
        mov    byte ptr [esi + 85h], 0
        push   0
        push   esi
        call   sub_1_2E10
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 93h], eax
        or     eax, eax
        je     SHORT loc_1_2F2A
        inc    dword ptr [eax + 14h]
loc_1_2F2A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2F40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 1
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 6]
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [esi + 29h], al
        mov    dx, cx
        inc    dx
        in     al, dx
        mov    byte ptr [esi + 2ah], al
        mov    dx, cx
        add    dx, 2
        in     al, dx
        mov    byte ptr [esi + 2bh], al
        mov    dx, cx
        add    dx, 3
        in     al, dx
        mov    byte ptr [esi + 2ch], al
        mov    dx, cx
        add    dx, 4
        in     al, dx
        mov    byte ptr [esi + 2dh], al
        mov    dx, cx
        add    dx, 5
        in     al, dx
        mov    byte ptr [esi + 2eh], al
        cmp    byte ptr [esi + 55h], 0
        jne    SHORT loc_1_2FA7
        mov    ecx, dword ptr [esi + 29h]
        mov    dword ptr [esi + 2fh], ecx
        mov    cl, byte ptr [esi + 2dh]
        mov    byte ptr [esi + 33h], cl
        lea    eax, [esi + 2fh]
        mov    cl, byte ptr [esi + 2eh]
        mov    byte ptr [eax + 5], cl
loc_1_2FA7:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_2FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    ebx, dword ptr [esi + 61h]
        cmp    dword ptr [esi + 21h], 0
        jbe    NEAR PTR loc_1_3093
        sub    edx, edx
        sub    eax, eax
loc_1_2FD4:
        mov    edi, ebx
        mov    ecx, 6
        rep stosd
        mov    ecx, dword ptr [esi + 16ah]
        mov    dword ptr [edx + ecx + 0ch], 0ffffh
        mov    ecx, dword ptr [esi + 16ah]
        mov    dword ptr [edx + ecx + 10h], eax
        mov    ecx, dword ptr [esi + 16ah]
        mov    dword ptr [edx + ecx + 14h], eax
        mov    ecx, dword ptr [esi + 16ah]
        mov    dword ptr [edx + ecx], ebx
        mov    cx, word ptr [esi + 1ah]
        sub    cx, word ptr [esi + 2]
        add    cx, bx
        mov    edi, dword ptr [esi + 16ah]
        mov    word ptr [edx + edi + 4], cx
        lea    ecx, [ebx + 18h]
        mov    edi, dword ptr [esi + 16ah]
        mov    dword ptr [edx + edi + 6], ecx
        mov    ecx, dword ptr [esi + 16ah]
        add    ecx, edx
        mov    edi, dword ptr [ecx + 6]
        sub    di, word ptr [esi + 2]
        add    di, word ptr [esi + 1ah]
        mov    word ptr [ecx + 0ah], di
        lea    ecx, [ebx + 10h]
        sub    cx, word ptr [esi + 2]
        add    cx, word ptr [esi + 1ah]
        mov    word ptr [ebx + 6], cx
        mov    word ptr [ebx + 4], 0ffffh
        mov    word ptr [ebx + 12h], 0ffffh
        mov    ecx, dword ptr [esi + 16ah]
        add    edx, 19h
        inc    dword ptr [ebp - 0ch]
        mov    cx, word ptr [ecx + edx - 0fh]
        and    ecx, 0ffffh
        mov    dword ptr [ebx + 14h], ecx
        mov    ecx, dword ptr [esi + 16ah]
        mov    ebx, dword ptr [edx + ecx - 13h]
        add    ebx, 5f0h
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [esi + 21h], ecx
        ja     NEAR PTR loc_1_2FD4
loc_1_3093:
        mov    edi, dword ptr [esi + 1dh]
        sub    eax, eax
        mov    ecx, 1ah
        rep stosd
        mov    ecx, dword ptr [esi + 21h]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    dword ptr [ecx + eax + 0ch], 0ffffh
        mov    ecx, dword ptr [esi + 21h]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    dword ptr [ecx + eax + 10h], 0
        mov    ecx, dword ptr [esi + 21h]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    dword ptr [ecx + eax + 14h], 0
        mov    eax, dword ptr [esi + 21h]
        mov    edx, dword ptr [esi + 1dh]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    dword ptr [eax + ecx], edx
        mov    eax, dword ptr [esi + 21h]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    word ptr [eax + ecx + 4], 0ff58h
        mov    ecx, dword ptr [esi + 21h]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    dword ptr [ecx + eax + 6], 0
        mov    ecx, dword ptr [esi + 21h]
        sub    edx, edx
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    word ptr [ecx + eax + 0ah], 0ffffh
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 4], 0ffffh
        mov    ebx, dword ptr [esi + 5dh]
        cmp    dword ptr [esi + 25h], edx
        jbe    NEAR PTR loc_1_321D
        mov    dword ptr [ebp - 4], edx
loc_1_3156:
        mov    eax, dword ptr [esi + 16eh]
        add    eax, dword ptr [ebp - 4]
        mov    edi, ebx
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        mov    eax, dword ptr [esi + 16eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + ecx], ebx
        mov    di, word ptr [esi + 1ah]
        sub    di, word ptr [esi + 2]
        add    di, bx
        mov    eax, dword ptr [esi + 16eh]
        mov    word ptr [ecx + eax + 4], di
        lea    edi, [ebx + 16h]
        sub    di, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 16eh]
        mov    eax, dword ptr [ecx + eax]
        add    di, word ptr [esi + 1ah]
        mov    word ptr [eax + 6], di
        lea    eax, [edx + 1]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        div    dword ptr [esi + 25h]
        mov    eax, dword ptr [esi + 16eh]
        mov    dword ptr [ecx + eax + 0ch], edx
        add    ebx, 20h
        mov    eax, dword ptr [esi + 16eh]
        mov    dword ptr [ecx + eax + 8], ebx
        mov    eax, dword ptr [esi + 16eh]
        add    eax, ecx
        mov    ecx, dword ptr [eax + 8]
        sub    cx, word ptr [esi + 2]
        add    cx, word ptr [esi + 1ah]
        mov    word ptr [eax + 6], cx
        mov    eax, dword ptr [esi + 16eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + ecx + 8]
        add    ebx, 5f0h
        add    dword ptr [ebp - 4], 10h
        mov    edx, dword ptr [ebp - 8]
        mov    eax, edx
        cmp    dword ptr [esi + 25h], eax
        ja     NEAR PTR loc_1_3156
loc_1_321D:
        sub    edx, edx
        cmp    dword ptr [esi + 25h], edx
        jbe    SHORT loc_1_326D
        sub    ecx, ecx
loc_1_3226:
        mov    edi, dword ptr [esi + 16eh]
        add    edi, ecx
        mov    eax, dword ptr [edi + 0ch]
        mov    ebx, dword ptr [edi]
        mov    di, word ptr [edi + 6]
        and    edi, 0ffffh
        mov    dword ptr [ebx + 1ah], edi
        mov    word ptr [ebx + 1eh], 85eah
        mov    word ptr [ebx + 16h], 0
        mov    word ptr [ebx + 18h], 0ffffh
        shl    eax, 4
        mov    edi, dword ptr [esi + 16eh]
        mov    ax, word ptr [eax + edi + 4]
        mov    word ptr [ebx + 4], ax
        add    ecx, 10h
        inc    edx
        cmp    dword ptr [esi + 25h], edx
        ja     SHORT loc_1_3226
loc_1_326D:
        mov    word ptr [ebx + 2], 0c000h
        mov    dword ptr [esi + 3dh], 0
        mov    eax, dword ptr [esi + 25h]
        dec    eax
        mov    dword ptr [esi + 41h], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3290:
        mov    al, 0ffh
        mov    ecx, 0ffh
loc_1_3297:
        mov    dx, 100h
        out    dx, al
        test   al, 80h
        je     SHORT loc_1_32AA
        add    al, al
        xor    al, 0e7h
        dec    ecx
        jne    SHORT loc_1_3297
        ret    4
loc_1_32AA:
        add    al, al
        dec    ecx
        jne    SHORT loc_1_3297
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_32C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 103h
        mov    esi, dword ptr [ebx + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    NEAR PTR loc_1_33E0
        cmp    byte ptr [ebx + 18h], 0
        jne    NEAR PTR loc_1_33D0
        inc    dword ptr [ebx + 14h]
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        push   0
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        call   sub_1_0530
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_33C4
        cmp    dword ptr [ebp - 0ch], 5eah
        ja     NEAR PTR loc_1_33C4
        lea    ecx, [edi + 24h]
        mov    dword ptr [ebp - 8], ecx
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0530
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0554
        test   byte ptr [ebx + 41h], 2
        je     SHORT loc_1_3353
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0dh], 0
        jmp    SHORT loc_1_3369
loc_1_3353:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 166h]
        push   eax
        call   sub_1_0330
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0dh], al
loc_1_3369:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], ebx
        mov    eax, dword ptr [esi + 8bh]
        or     eax, eax
        jne    SHORT loc_1_3381
        mov    dword ptr [esi + 87h], edi
        jmp    SHORT loc_1_3384
loc_1_3381:
        mov    dword ptr [eax + 24h], edi
loc_1_3384:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 8bh], edi
        mov    dword ptr [ecx], 0
        inc    dword ptr [esi + 0a3h]
        cmp    byte ptr [esi + 86h], 0
        jne    SHORT loc_1_33E7
        sub    ebx, ebx
loc_1_33A4:
        cmp    dword ptr [esi + 87h], ebx
        je     SHORT loc_1_33E7
        cmp    byte ptr [esi + 85h], bl
        je     SHORT loc_1_33E7
        push   esi
        call   sub_1_3430
        cmp    byte ptr [esi + 86h], bl
        je     SHORT loc_1_33A4
        jmp    SHORT loc_1_33E7
loc_1_33C4:
        dec    dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 4], 0c000009ah
        jmp    SHORT loc_1_33E7
loc_1_33D0:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_33E7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_33E0:
        mov    dword ptr [ebp - 4], 0c001000dh
loc_1_33E7:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_341F
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_3411
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_3411
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_341F
loc_1_3411:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_341F:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_3430:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [esi + 87h]
        mov    byte ptr [esi + 86h], 1
        push   eax
        push   esi
        call   sub_1_09F0
        or     al, al
        je     SHORT loc_1_3498
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 87h], eax
        or     eax, eax
        jne    SHORT loc_1_346D
        mov    dword ptr [esi + 8bh], 0
loc_1_346D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_1_0700
        push   edi
        push   esi
        call   sub_1_1830
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_0860
        mov    byte ptr [esi + 86h], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3498:
        mov    byte ptr [esi + 86h], 0
        mov    byte ptr [esi + 85h], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_34B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_3520
        cmp    byte ptr [edi + 18h], 0
        jne    SHORT loc_1_3517
        inc    dword ptr [edi + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        test   cl, 1
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        jne    SHORT loc_1_34FA
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_057E
        dec    dword ptr [edi + 14h]
        sub    edi, edi
        jmp    SHORT loc_1_3525
loc_1_34FA:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        and    ecx, 0fffffffeh
        push   eax
        add    ecx, dword ptr [ebp + 10h]
        push   ecx
        call   sub_1_1950
        dec    dword ptr [edi + 14h]
        sub    edi, edi
        jmp    SHORT loc_1_3525
loc_1_3517:
        mov    edi, 0c001000ch
        jmp    SHORT loc_1_3525
        db  08Bh, 0FFh
loc_1_3520:
        mov    edi, 0c001000dh
loc_1_3525:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_355D
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_354F
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_354F
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_355D
loc_1_354F:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_355D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_3574:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
loc_1_357C:
        pop    es
        add    byte ptr [edi], al
        db  000h, 074h, 035h, 000h
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        push   esp
        jb     SHORT loc_1_3600
        outsb
        db  073h, 063h
        imul   esi, dword ptr gs:[esi + 65h], 72540072h
        popad
        outsb
        jae    SHORT loc_1_3611
        imul   esi, dword ptr gs:[esi + 65h], 6e490072h
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_360C
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        jb     SHORT loc_1_363C
        jne    SHORT loc_1_363C
        db  074h, 04Eh
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 4fh], cl
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_35DC:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ebp
        insd
        outsd
        db  072h, 079h
        dec    ebp
        popad
        jo     SHORT loc_1_365E
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_365E
        db  073h, 073h
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
loc_1_3600:
        jb     SHORT loc_1_367B
        dec    ebp
        popad
        db  070h, 070h
        inc    edx
        popad
        jae    SHORT loc_1_3671
loc_1_360C:
        inc    ecx
        db  064h, 064h, 072h, 065h
loc_1_3611:
        db  073h, 073h
        add    byte ptr [edx + 65h], bl
        jb     SHORT loc_1_3687
        push   edi
        popad
        imul   esi, dword ptr [ebx + edx*2 + 74h], 657461h
        add    byte ptr [eax], al
        pop    edx
        db  065h, 072h, 06Fh
        push   edi
        popad
        imul   esi, dword ptr [ebx + edx*2 + 74h], 657461h
        add    byte ptr [eax], al
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_36B3
        dec    ebp
        popad
loc_1_363C:
        db  070h, 070h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_36C7
        dec    ebp
        popad
        db  070h, 070h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
loc_1_365C:
        add    dword ptr [ecx], eax
loc_1_365E:
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
loc_1_3671:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
loc_1_367B:
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
loc_1_3687:
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
loc_1_36B3:
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0d000201h
        add    al, byte ptr [edx]
        add    byte ptr [esi], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
loc_1_36C7:
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
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    al, 2
        add    al, byte ptr [ecx]
        add    eax, 6010202h
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
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
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_3758:
        inc    ebp
        insb
        outsb
        imul   esi, dword ptr [ecx], 36h
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_3767
loc_1_3767:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
