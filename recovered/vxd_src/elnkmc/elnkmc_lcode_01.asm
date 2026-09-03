; elnkmc_lcode_01.asm -- ELNKMC object 1 segment _LTEXT
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
ELNKMC_Control:
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
sub_1_057E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0584:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
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
        db  04Dh, 043h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    eax, dword ptr [esi + 45h]
        mov    dx, word ptr [esi + 1ah]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    ax, word ptr [eax + ecx + 4]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    word ptr [ecx + 0ffc4h], ax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        push   1
        mov    word ptr [eax + 0ffc2h], 100h
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0710:
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
        jne    SHORT loc_1_07D9
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
loc_1_07D9:
        mov    eax, dword ptr [edi + 16ah]
        mov    edi, dword ptr [ebx + eax + 6]
        mov    ebx, dword ptr [ebp - 14h]
        or     ebx, ebx
        je     SHORT loc_1_0827
loc_1_07EA:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_07FE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        call   sub_1_05EC
loc_1_07FE:
        add    edi, dword ptr [ebp - 4]
        cmp    ebx, 1
        jbe    SHORT loc_1_0824
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
loc_1_0824:
        dec    ebx
        jne    SHORT loc_1_07EA
loc_1_0827:
        cmp    dword ptr [ebp - 10h], 3ch
        jae    SHORT loc_1_084C
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
loc_1_084C:
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
sub_1_0870:
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
        je     SHORT loc_1_08E0
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
loc_1_08E0:
        cmp    dword ptr [edi + 6dh], 0ffffh
        jne    SHORT loc_1_08EC
        mov    dword ptr [edi + 6dh], esi
loc_1_08EC:
        sub    ebx, ebx
        mov    edi, edi
loc_1_08F0:
        xor    ecx, ecx
        mov    cx, word ptr [edi + 1ah]
        mov    eax, dword ptr [edi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        or     ax, ax
        je     SHORT loc_1_0917
        push   32h
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 4e20h
        jbe    SHORT loc_1_08F0
loc_1_0917:
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
sub_1_0940:
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
loc_1_0962:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        or     ax, ax
        je     SHORT loc_1_0989
        push   32h
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 4e20h
        jbe    SHORT loc_1_0962
loc_1_0989:
        mov    eax, dword ptr [esi + 21h]
        mov    dword ptr [esi + 45h], eax
        push   esi
        db  068h, 0A0h, 006h, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        sub    ebx, ebx
loc_1_09A3:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        or     ax, ax
        je     SHORT loc_1_09CA
        push   32h
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 4e20h
        jbe    SHORT loc_1_09A3
loc_1_09CA:
        sub    esi, esi
        mov    ebx, 8000h
loc_1_09D1:
        mov    ax, word ptr [edi]
        test   bx, ax
        jne    SHORT loc_1_09E9
        push   32h
        call   sub_1_0500
        inc    esi
        cmp    esi, 4e20h
        jbe    SHORT loc_1_09D1
loc_1_09E9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + 65h], 0
        je     SHORT loc_1_0A30
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 69h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 69h]
        inc    eax
        mov    dword ptr [edx + 69h], eax
        cmp    dword ptr [edx + 21h], eax
        ja     SHORT loc_1_0A27
        mov    dword ptr [edx + 69h], 0
loc_1_0A27:
        dec    dword ptr [edx + 65h]
        mov    al, 1
        pop    ebp
        ret    8
loc_1_0A30:
        mov    byte ptr [edx + 85h], 0
        sub    al, al
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_0A40:
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
        jne    SHORT loc_1_0A6C
        mov    dword ptr [edx + 71h], 0ffffh
loc_1_0A6C:
        mov    word ptr [ecx + 4], 0ffffh
        mov    word ptr [ecx], 0
        inc    dword ptr [edx + 65h]
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 16ah]
        or     eax, eax
        je     SHORT loc_1_0A9B
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0A9B:
        mov    eax, dword ptr [esi + 16eh]
        or     eax, eax
        je     SHORT loc_1_0AAF
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0AAF:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AC0:
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
loc_1_0B40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        sub    edx, edx
        inc    dword ptr [esi + 59h]
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, edx
        je     SHORT loc_1_0B6B
        mov    eax, dword ptr [ebp + 14h]
loc_1_0B5A:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_0B67
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_1_0B5A
loc_1_0B67:
        cmp    ecx, edx
        jne    SHORT loc_1_0B75
loc_1_0B6B:
        mov    edi, 0c0010019h
        jmp    NEAR PTR loc_1_0C2E
loc_1_0B75:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], edx
        db  0A1h, 0E0h, 035h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   45h
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     SHORT loc_1_0BA0
        mov    edi, 0c000009ah
        jmp    NEAR PTR loc_1_0C2E
        db  005h, 000h, 000h, 000h, 000h
loc_1_0BA0:
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
        call   sub_1_1D30
        mov    edi, 103h
        cmp    byte ptr [esi + 54h], 0
        je     SHORT loc_1_0C2E
        mov    byte ptr [esi + 54h], 0
        push   7d0h
        lea    eax, [esi + 149h]
        push   eax
        call   sub_1_04E2
loc_1_0C2E:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_0C66
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_0C58
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_0C58
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_0C66
loc_1_0C58:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_0C66:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0C70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [edi + 18h], 0
        jne    SHORT loc_1_0CB0
        lea    eax, [edi + 19h]
        mov    dword ptr [edi + 29h], 4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        push   eax
        push   esi
        call   sub_1_1D30
        dec    dword ptr [edi + 14h]
        mov    edi, 103h
        jmp    SHORT loc_1_0CB5
        db  005h, 000h, 000h, 000h, 000h
loc_1_0CB0:
        mov    edi, 0c0010002h
loc_1_0CB5:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_0CED
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_0CDF
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_0CDF
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_0CED
loc_1_0CDF:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_0CED:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 030h, 037h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        db  0A1h, 01Ch, 037h, 000h, 000h
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0D30:
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
        db  0E8h, 0BEh, 0F3h, 0FEh, 0FFh
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
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0DA0:
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
loc_1_0DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
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
        je     SHORT loc_1_0DF8
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, 40h
        out    dx, al
        in     al, dx
loc_1_0DF8:
        test   byte ptr [ebp - 4], 40h
        je     SHORT loc_1_0E08
        cmp    byte ptr [esi + 4eh], 0
        je     SHORT loc_1_0E08
        mov    byte ptr [esi + 4eh], 0
loc_1_0E08:
        mov    byte ptr [esi + 58h], 0
        mov    ax, word ptr [ebp - 4]
        and    ax, 0f000h
        je     SHORT loc_1_0E80
        cmp    byte ptr [esi + 53h], 0
        je     SHORT loc_1_0E60
        xor    edx, edx
        mov    dx, word ptr [esi + 1ah]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        sub    bl, bl
        mov    word ptr [ecx + 0ffc2h], ax
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        push   1
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0E60:
        mov    al, byte ptr [esi + 1ch]
        and    al, 0fbh
        mov    byte ptr [esi + 1ch], al
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        mov    bl, 1
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0E80:
        mov    byte ptr [esi + 57h], 1
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
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
        jne    NEAR PTR loc_1_113A
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
        jne    NEAR PTR loc_1_0F6F
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc0h]
        mov    word ptr [ebp - 6], ax
        test   al, 40h
        je     SHORT loc_1_0EFE
        cmp    byte ptr [esi + 4eh], bl
        je     SHORT loc_1_0EFE
        mov    byte ptr [esi + 4eh], bl
loc_1_0EFE:
        and    word ptr [ebp - 6], 0f000h
        je     SHORT loc_1_0F6F
        mov    dword ptr [ebp - 0ch], ebx
loc_1_0F09:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_0F33
        push   32h
        call   sub_1_0500
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 4e20h
        jbe    SHORT loc_1_0F09
loc_1_0F33:
        xor    edx, edx
        mov    dx, word ptr [esi + 1ah]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 0ffc2h], ax
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        push   1
        call   sub_1_0500
        or     di, word ptr [ebp - 6]
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
loc_1_0F6F:
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 0ffffh
        je     SHORT loc_1_0F91
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_1_0F95
loc_1_0F91:
        mov    word ptr [ebp - 2], bx
loc_1_0F95:
        test   di, 0e000h
        jne    SHORT loc_1_0FEB
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_1_0FA8
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_1_0FEB
loc_1_0FA8:
        test   di, 1000h
        je     SHORT loc_1_0FB4
        cmp    byte ptr [esi + 4eh], bl
        je     SHORT loc_1_0FEB
loc_1_0FB4:
        cmp    dword ptr [esi + 75h], ebx
        jne    SHORT loc_1_0FEB
        cmp    byte ptr [esi + 8fh], bl
        je     SHORT loc_1_0FC7
        cmp    dword ptr [esi + 59h], 1
        je     SHORT loc_1_0FEB
loc_1_0FC7:
        cmp    byte ptr [esi + 86h], bl
        jne    NEAR PTR loc_1_10B0
        cmp    dword ptr [esi + 87h], ebx
        je     NEAR PTR loc_1_10B0
        cmp    byte ptr [esi + 85h], bl
        je     NEAR PTR loc_1_10B0
loc_1_0FEB:
        mov    eax, dword ptr [esi + 59h]
        inc    eax
        mov    dword ptr [esi + 59h], eax
        cmp    byte ptr [esi + 8fh], bl
        je     SHORT loc_1_1003
        cmp    eax, 2
        je     NEAR PTR loc_1_10A7
loc_1_1003:
        test   di, 4000h
        jne    SHORT loc_1_1016
        test   di, 1000h
        je     SHORT loc_1_1025
        cmp    byte ptr [esi + 4eh], bl
        jne    SHORT loc_1_1025
loc_1_1016:
        push   esi
        call   sub_1_1150
        cmp    al, bl
        je     SHORT loc_1_1025
        and    di, 0afffh
loc_1_1025:
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 0ffffh
        je     SHORT loc_1_1065
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        test   ah, 40h
        jne    SHORT loc_1_1065
        cmp    ax, bx
        je     SHORT loc_1_1065
        push   esi
        call   sub_1_1320
        test   di, 0a000h
        je     SHORT loc_1_106A
        test   di, 1000h
        jne    SHORT loc_1_1025
        jmp    SHORT loc_1_106A
loc_1_1065:
        and    di, 5fffh
loc_1_106A:
        cmp    byte ptr [esi + 86h], bl
        jne    SHORT loc_1_1088
        cmp    dword ptr [esi + 87h], ebx
        je     SHORT loc_1_1088
        cmp    byte ptr [esi + 85h], bl
        je     SHORT loc_1_1088
        push   esi
        call   sub_1_34A0
loc_1_1088:
        push   esi
        call   sub_1_1750
        dec    dword ptr [esi + 59h]
        test   di, 1000h
        je     NEAR PTR loc_1_0ED1
        cmp    byte ptr [esi + 4eh], bl
        je     NEAR PTR loc_1_0ED1
        jmp    SHORT loc_1_10B0
loc_1_10A7:
        push   esi
        call   sub_1_2A60
        dec    dword ptr [esi + 59h]
loc_1_10B0:
        mov    edi, dword ptr [esi + 0d9h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_110E
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_110E
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0444
        mov    eax, dword ptr [esi + 35h]
        or     eax, eax
        je     SHORT loc_1_10EF
        cmp    dword ptr [eax + 10h], 4
        jne    SHORT loc_1_10EF
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_10EF
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 35h], eax
        push   esi
        call   sub_1_1D70
loc_1_10EF:
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
loc_1_110E:
        mov    byte ptr [esi + 50h], 0
        push   esi
        db  068h, 0A0h, 00Dh, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        cmp    byte ptr [esi + 4dh], 0
        je     SHORT loc_1_113A
        mov    byte ptr [esi + 4dh], 0
        mov    eax, dword ptr [esi + 166h]
        push   eax
        call   sub_1_0290
loc_1_113A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1150:
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
loc_1_1170:
        mov    byte ptr [esi + 91h], 1
        mov    ax, word ptr [ebx]
        mov    word ptr [ebp - 2], ax
        test   ax, 0bfffh
        je     NEAR PTR loc_1_1272
        cmp    edi, 0ah
        ja     NEAR PTR loc_1_12C0
        test   ah, 20h
        jne    SHORT loc_1_11D5
        test   al, 80h
        je     SHORT loc_1_11A2
        inc    dword ptr [esi + 0c3h]
        jmp    SHORT loc_1_11AE
loc_1_11A2:
        test   byte ptr [ebp - 2], 40h
        je     SHORT loc_1_11AE
        inc    dword ptr [esi + 0c7h]
loc_1_11AE:
        push   ebx
        push   esi
        call   sub_1_12E0
        mov    eax, dword ptr [esi + 3dh]
        cmp    dword ptr [esi + 25h], eax
        jne    SHORT loc_1_11C4
        mov    dword ptr [esi + 3dh], 0
loc_1_11C4:
        mov    ecx, dword ptr [esi + 3dh]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 16eh]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    SHORT loc_1_1170
loc_1_11D5:
        inc    dword ptr [esi + 9fh]
        cmp    byte ptr [esi + 8fh], 0
        jne    NEAR PTR loc_1_12CB
        mov    ax, word ptr [ebx + 16h]
        mov    word ptr [ebp - 2], ax
        and    eax, 3fffh
        lea    edx, [ebx + 8]
        mov    dword ptr [ebp - 8], edx
        or     edx, edx
        je     SHORT loc_1_1240
        or     eax, eax
        je     SHORT loc_1_1220
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 0ch], edx
        or     edx, edx
        je     SHORT loc_1_1240
        or     eax, eax
        je     SHORT loc_1_1220
        cmp    eax, 5dch
        ja     SHORT loc_1_1240
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        jmp    SHORT loc_1_1226
loc_1_1220:
        push   0
        push   0
        push   0
loc_1_1226:
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
loc_1_1240:
        push   ebx
        push   esi
        call   sub_1_12E0
        mov    byte ptr [esi + 4dh], 1
        mov    eax, dword ptr [esi + 3dh]
        inc    eax
        mov    dword ptr [esi + 3dh], eax
        cmp    dword ptr [esi + 25h], eax
        jne    SHORT loc_1_125E
        mov    dword ptr [esi + 3dh], 0
loc_1_125E:
        mov    ecx, dword ptr [esi + 3dh]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 16eh]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    NEAR PTR loc_1_1170
loc_1_1272:
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
        jne    SHORT loc_1_12A0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_12A0:
        mov    eax, dword ptr [esi + 3dh]
        shl    eax, 4
        add    eax, dword ptr [esi + 16eh]
        push   eax
        push   esi
        call   sub_1_2B60
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_12C0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_12CB:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12E0:
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
        jne    SHORT loc_1_131C
        mov    dword ptr [edx + 41h], 0
loc_1_131C:
        pop    ebp
        ret    8
sub_1_1320:
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
        jne    SHORT loc_1_1372
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
loc_1_1372:
        mov    byte ptr [esi + 90h], 1
        cmp    dword ptr [ebp - 8], 4
        je     SHORT loc_1_13C0
        mov    edi, dword ptr [esi + 6dh]
        push   esi
        call   sub_1_14E0
        mov    eax, dword ptr [esi + 6dh]
        push   eax
        push   esi
        call   sub_1_1540
        lea    eax, [edi + edi*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    eax, dword ptr [eax + ecx + 0ch]
        mov    dword ptr [esi + 6dh], eax
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_1_14CF
        mov    dword ptr [esi + 71h], 0ffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_13C0:
        mov    edx, dword ptr [esi + 6dh]
        test   byte ptr [ebp - 1], 20h
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    edx, dword ptr [edx + eax + 10h]
        lea    edi, [edx + 24h]
        je     SHORT loc_1_1410
        inc    dword ptr [esi + 9bh]
        test   byte ptr [ebp - 2], 80h
        je     SHORT loc_1_13F0
        inc    dword ptr [esi + 0b7h]
        jmp    SHORT loc_1_144E
loc_1_13F0:
        mov    ax, word ptr [ebp - 2]
        and    ax, 0fh
        cmp    ax, 1
        jne    SHORT loc_1_1406
        inc    dword ptr [esi + 0bbh]
        jmp    SHORT loc_1_144E
loc_1_1406:
        jbe    SHORT loc_1_144E
        inc    dword ptr [esi + 0bfh]
        jmp    SHORT loc_1_144E
loc_1_1410:
        test   byte ptr [ebp - 2], 20h
        je     SHORT loc_1_1420
        inc    dword ptr [esi + 0a7h]
        jmp    SHORT loc_1_144C
        db  08Bh, 0FFh
loc_1_1420:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_1430
        inc    dword ptr [esi + 0abh]
        jmp    SHORT loc_1_144C
        db  08Bh, 0FFh
loc_1_1430:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_1440
        inc    dword ptr [esi + 0b3h]
        jmp    SHORT loc_1_144C
        db  08Bh, 0FFh
loc_1_1440:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_144C
        inc    dword ptr [esi + 0afh]
loc_1_144C:
        sub    cl, cl
loc_1_144E:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_145D
        mov    dword ptr [esi + 81h], 0
loc_1_145D:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 7dh], eax
        cmp    byte ptr [edi + 0dh], 0
        jne    SHORT loc_1_1485
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
        jmp    SHORT loc_1_14A2
loc_1_1485:
        mov    byte ptr [edi + 0ch], cl
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_1493
        mov    dword ptr [esi + 75h], edx
        jmp    SHORT loc_1_1499
loc_1_1493:
        mov    eax, dword ptr [esi + 79h]
        mov    dword ptr [eax + 24h], edx
loc_1_1499:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 79h], edx
loc_1_14A2:
        mov    eax, dword ptr [esi + 6dh]
        push   eax
        push   esi
        call   sub_1_1540
        mov    eax, dword ptr [esi + 6dh]
        push   eax
        push   esi
        call   sub_1_0A40
        cmp    byte ptr [esi + 85h], 0
        jne    SHORT loc_1_14CF
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_14CF
        mov    byte ptr [esi + 85h], 1
loc_1_14CF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14E0:
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
        jne    SHORT loc_1_1530
        mov    eax, dword ptr [esi + 35h]
        or     eax, eax
        je     SHORT loc_1_1538
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
        call   sub_1_1D70
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1530:
        cmp    eax, 1
        je     SHORT loc_1_1538
        dec    dword ptr [eax + 14h]
loc_1_1538:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1540:
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
        je     SHORT loc_1_15AB
        mov    byte ptr [ecx + 18h], 0
        mov    dword ptr [esi + 45h], eax
        sub    edi, edi
        sub    ebx, ebx
loc_1_1572:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_1599
        push   32h
        call   sub_1_0500
        inc    edi
        cmp    edi, 4e20h
        jbe    SHORT loc_1_1572
loc_1_1599:
        push   esi
        db  068h, 0A0h, 006h, 000h, 000h
        add    esi, 131h
        push   esi
        call   sub_1_04FA
loc_1_15AB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15C0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    al, al
        cmp    byte ptr [ecx + 50h], al
        jne    SHORT loc_1_1602
        cmp    byte ptr [ecx + 8fh], al
        jne    SHORT loc_1_1602
        cmp    byte ptr [ecx + 90h], al
        jne    SHORT loc_1_1602
        cmp    byte ptr [ecx + 91h], al
        jne    SHORT loc_1_1602
        mov    al, byte ptr [ecx + 92h]
        cmp    al, 0ah
        lea    edx, [eax + 1]
        mov    byte ptr [ecx + 92h], dl
        jae    SHORT loc_1_1600
        sub    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1600:
        mov    al, 1
loc_1_1602:
        mov    byte ptr [ecx + 92h], 0
        mov    byte ptr [ecx + 90h], 0
        mov    byte ptr [ecx + 91h], 0
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 0ffffh
        je     NEAR PTR loc_1_16DF
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [esi + 16ah]
        cmp    byte ptr [eax + 18h], 0
        je     NEAR PTR loc_1_16DB
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [esi + 6dh]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 16ah]
        mov    eax, dword ptr [eax + ecx]
        mov    ax, word ptr [eax]
        test   ah, 40h
        jne    SHORT loc_1_1690
        or     ax, ax
        je     SHORT loc_1_1690
        cmp    byte ptr [esi + 58h], 0
        jne    SHORT loc_1_1690
        mov    byte ptr [esi + 57h], 1
        mov    byte ptr [esi + 58h], 1
        push   1
        lea    eax, [esi + 155h]
        push   eax
        call   sub_1_04E2
        jmp    SHORT loc_1_16DF
loc_1_1690:
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_16DF
        mov    byte ptr [esi + 53h], 1
        push   0
        push   esi
        call   sub_1_2F40
        cmp    byte ptr [esi + 50h], 0
        jne    SHORT loc_1_16DF
        mov    al, byte ptr [esi + 1ch]
        and    al, 0fbh
        mov    byte ptr [esi + 1ch], al
        inc    dword ptr [esi + 59h]
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        push   esi
        call   sub_1_2A60
        dec    dword ptr [esi + 59h]
        push   esi
        db  068h, 0A0h, 00Dh, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        jmp    SHORT loc_1_16DF
loc_1_16DB:
        mov    byte ptr [eax + 18h], 1
loc_1_16DF:
        push   esi
        call   sub_1_15C0
        or     al, al
        je     SHORT loc_1_1729
        mov    byte ptr [esi + 53h], 1
        push   0
        push   esi
        call   sub_1_2F40
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
        call   sub_1_2A60
        dec    dword ptr [esi + 59h]
        push   esi
        db  068h, 0A0h, 00Dh, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
loc_1_1729:
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
sub_1_1750:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 75h]
        or     esi, esi
        je     NEAR PTR loc_1_183A
        lea    edi, [esi + 24h]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_1776
        mov    dword ptr [ebx + 79h], 0
loc_1_1776:
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
        jae    SHORT loc_1_17DA
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_1_17DA
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebx + 172h]
        push   eax
        push   100h
        push   0
        push   esi
        call   sub_1_1890
        lea    eax, [ebx + 172h]
        mov    dword ptr [ebp - 8], eax
loc_1_17DA:
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_17F2
        cmp    dword ptr [ebp - 4], 5
        jbe    SHORT loc_1_181B
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    SHORT loc_1_1809
loc_1_17F2:
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
loc_1_1809:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 166h]
        push   eax
        call   sub_1_00B0
loc_1_181B:
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
loc_1_183A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1850:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [ecx + 24h]
        mov    edx, dword ptr [eax + 81h]
        or     edx, edx
        je     SHORT loc_1_186A
        mov    dword ptr [edx + 24h], ecx
loc_1_186A:
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 81h], ecx
        cmp    dword ptr [eax + 7dh], 0
        jne    SHORT loc_1_187F
        mov    dword ptr [eax + 7dh], ecx
loc_1_187F:
        dec    dword ptr [eax + 0a3h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1890:
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
        je     NEAR PTR loc_1_195A
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
        je     NEAR PTR loc_1_195A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_1955
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_18E0:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_190C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1955
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    SHORT loc_1_1950
loc_1_190C:
        or     edi, edi
        je     SHORT loc_1_1929
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_1921
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1950
loc_1_1921:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1929:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_1937
        mov    ebx, eax
loc_1_1937:
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
loc_1_1950:
        cmp    esi, dword ptr [ebp + 10h]
        jb     SHORT loc_1_18E0
loc_1_1955:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_195A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1970:
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
        je     NEAR PTR loc_1_1A3B
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
        je     NEAR PTR loc_1_1A3B
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
        je     SHORT loc_1_1A36
        mov    edi, dword ptr [ebp + 14h]
loc_1_19C4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_19F0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1A36
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    SHORT loc_1_1A31
loc_1_19F0:
        or     edi, edi
        je     SHORT loc_1_1A0D
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_1A05
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1A31
loc_1_1A05:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1A0D:
        mov    ebx, dword ptr [ebp + 0ch]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_1A1B
        mov    ebx, eax
loc_1_1A1B:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    esi, ebx
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1A31:
        cmp    dword ptr [ebp + 0ch], esi
        ja     SHORT loc_1_19C4
loc_1_1A36:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_1A3B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 8fh], 0
        je     SHORT loc_1_1A6B
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_1A6B:
        mov    cl, byte ptr [ebp + 18h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_1_1AA0
        or     al, al
        je     SHORT loc_1_1A8B
        mov    eax, 103h
        pop    ebp
        ret    14h
loc_1_1A8B:
        sub    eax, eax
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AA0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    al, al
        mov    dx, 2
        mov    ecx, dword ptr [ebp + 10h]
        test   cl, 24h
        je     SHORT loc_1_1AB8
        mov    dx, 3
        jmp    SHORT loc_1_1AC0
loc_1_1AB8:
        test   cl, 8
        je     SHORT loc_1_1AC0
        sub    dx, dx
loc_1_1AC0:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 0cbh], dx
        je     NEAR PTR loc_1_1B66
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
        jne    SHORT loc_1_1B43
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
loc_1_1B43:
        mov    edx, dword ptr [ecx + 21h]
        mov    eax, dword ptr [ecx + 16ah]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 14h], 0
        mov    eax, dword ptr [ecx + 21h]
        push   eax
        push   ecx
        call   sub_1_0870
        mov    al, 1
loc_1_1B66:
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_1B70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 8fh], 0
        je     SHORT loc_1_1B8B
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_1B8B:
        mov    cl, byte ptr [ebp + 20h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edx
        call   sub_1_1BB0
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_1_1BEB
        sub    edx, edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
loc_1_1BCA:
        sub    esi, esi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, edx
loc_1_1BD1:
        mov    bl, byte ptr [esi + eax]
        mov    edi, dword ptr [ecx + 1dh]
        add    edi, edx
        mov    byte ptr [esi + edi + 8], bl
        inc    esi
        cmp    esi, 6
        jb     SHORT loc_1_1BD1
        add    edx, 6
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_1BCA
loc_1_1BEB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 1dh]
        imul   ax, ax, 6
        mov    word ptr [edx + 6], ax
        mov    eax, dword ptr [ecx + 1dh]
        cmp    byte ptr [ebp + 18h], 0
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ecx + 1dh]
        mov    word ptr [eax + 2], 3
        jne    SHORT loc_1_1C33
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
loc_1_1C33:
        mov    edx, dword ptr [ecx + 21h]
        mov    eax, dword ptr [ecx + 16ah]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 14h], 0
        cmp    byte ptr [ecx + 53h], 0
        je     SHORT loc_1_1C60
        push   ecx
        call   sub_1_0940
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
loc_1_1C60:
        mov    eax, dword ptr [ecx + 21h]
        push   eax
        push   ecx
        call   sub_1_0870
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 14h]
        ret    4
        db  08Dh, 049h, 000h
loc_1_1C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_1CE7
        cmp    byte ptr [edx + 18h], 0
        jne    SHORT loc_1_1CE0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        jl     SHORT loc_1_1CBC
        cmp    ecx, 1
        jle    SHORT loc_1_1CC3
loc_1_1CBC:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_1CEC
loc_1_1CC3:
        inc    dword ptr [edx + 14h]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax], 0
        push   eax
        mov    edi, 103h
        push   esi
        call   sub_1_1D30
        jmp    SHORT loc_1_1CEC
        db  08Dh, 049h, 000h
loc_1_1CE0:
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_1CEC
loc_1_1CE7:
        mov    edi, 0c001000dh
loc_1_1CEC:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_1D24
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_1D16
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_1D16
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_1D24
loc_1_1D16:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_1D24:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_1D30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 35h], 0
        je     SHORT loc_1_1D50
        mov    edx, dword ptr [eax + 39h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 39h], ecx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D50:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 35h], ecx
        mov    dword ptr [eax + 39h], ecx
        push   eax
        call   sub_1_1D70
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 51h], 0
        jne    NEAR PTR loc_1_1F90
        mov    byte ptr [esi + 51h], 1
        mov    edi, dword ptr [esi + 35h]
        or     edi, edi
        je     NEAR PTR loc_1_1F8C
loc_1_1D95:
        cmp    byte ptr [esi + 8fh], 0
        jne    NEAR PTR loc_1_1F8C
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_1DC6
        cmp    eax, 1
        je     SHORT loc_1_1DEC
        cmp    eax, 2
        je     SHORT loc_1_1E12
        cmp    eax, 3
        je     SHORT loc_1_1E36
        cmp    eax, 4
        je     NEAR PTR loc_1_1EB2
        jmp    NEAR PTR loc_1_1F35
loc_1_1DC6:
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
        call   sub_1_2120
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1F35
loc_1_1DEC:
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
        call   sub_1_1FB0
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1F35
loc_1_1E12:
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
        call   sub_1_2120
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1F35
loc_1_1E36:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax + 166h]
        push   eax
        db  0E8h, 01Fh, 0E3h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_1E71
        push   0
        push   0c0000001h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_043E
        push   0
        push   0
        push   ebx
        call   sub_1_050C
        jmp    SHORT loc_1_1EA6
loc_1_1E71:
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
loc_1_1EA6:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1F35
loc_1_1EB2:
        mov    byte ptr [esi + 52h], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 166h]
        push   eax
        db  0E8h, 075h, 0E3h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_1_1ED7
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_1EF0
loc_1_1ED7:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_1EF0
        cmp    dword ptr [ebp - 4], 0c001000eh
        jne    SHORT loc_1_1EF0
        mov    dword ptr [ebp - 4], 103h
loc_1_1EF0:
        cmp    byte ptr [esi + 52h], 0
        je     SHORT loc_1_1F00
        mov    eax, dword ptr [esi + 21h]
        push   eax
        push   esi
        call   sub_1_0870
loc_1_1F00:
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
loc_1_1F35:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_1F99
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 35h], eax
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 2
        jne    SHORT loc_1_1F64
        inc    dword ptr [esi + 59h]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        call   sub_1_047A
        dec    dword ptr [esi + 59h]
        jmp    SHORT loc_1_1F81
loc_1_1F64:
        or     eax, eax
        je     SHORT loc_1_1F6D
        cmp    eax, 1
        jne    SHORT loc_1_1F81
loc_1_1F6D:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 14h]
loc_1_1F81:
        mov    edi, dword ptr [esi + 35h]
        or     edi, edi
        jne    NEAR PTR loc_1_1D95
loc_1_1F8C:
        mov    byte ptr [esi + 51h], 0
loc_1_1F90:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1F99:
        mov    byte ptr [esi + 51h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010eh
        mov    dword ptr [edi], 0
        je     SHORT loc_1_1FF9
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_2073
        cmp    eax, 10112h
        je     NEAR PTR loc_1_2086
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_20C5
        mov    eax, 0c0010017h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1FF9:
        mov    esi, dword ptr [ebp + 18h]
        cmp    esi, 4
        je     SHORT loc_1_2014
        mov    dword ptr [edi], 4
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2014:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 4], 0f050h
        je     SHORT loc_1_2048
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c00000bbh
        mov    dword ptr [ecx], 4
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2048:
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
        db  0E8h, 07Ah, 0E5h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2073:
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_2086:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_20A0
        mov    dword ptr [edi], 4
        mov    eax, 0c0010014h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_1_20A0:
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
loc_1_20C5:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, esi
        cdq
        idiv   ecx
        or     edx, edx
        je     SHORT loc_1_20E5
        mov    dword ptr [edi], ecx
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_20E5:
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
        db  0E8h, 0F2h, 0E1h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_2120:
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
        ja     SHORT loc_1_2180
        je     NEAR PTR loc_1_2573
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_21A1
        shl    eax, 2
        db  0FFh, 0A0h, 09Ch, 026h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_2180:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2598
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_25C0
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2650
loc_1_21A1:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_21B3
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2657
loc_1_21B3:
        cmp    edx, 1020102h
        ja     SHORT loc_1_2230
        je     NEAR PTR loc_1_2384
        cmp    edx, 20105h
        ja     SHORT loc_1_2204
        je     NEAR PTR loc_1_2334
        cmp    edx, 20101h
        je     NEAR PTR loc_1_2294
        cmp    edx, 20102h
        je     NEAR PTR loc_1_22A5
        cmp    edx, 20103h
        je     NEAR PTR loc_1_22B6
        cmp    edx, 20104h
        je     NEAR PTR loc_1_22D9
        jmp    NEAR PTR loc_1_2288
loc_1_2204:
        cmp    edx, 2020dh
        je     NEAR PTR loc_1_2347
        cmp    edx, 2020eh
        je     NEAR PTR loc_1_2360
        cmp    edx, 1020101h
        je     NEAR PTR loc_1_2371
        jmp    SHORT loc_1_2288
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2230:
        cmp    edx, 1020203h
        ja     SHORT loc_1_2264
        je     NEAR PTR loc_1_23C8
        cmp    edx, 1020103h
        je     NEAR PTR loc_1_2395
        cmp    edx, 1020201h
        je     NEAR PTR loc_1_23A6
        cmp    edx, 1020202h
        je     NEAR PTR loc_1_23B7
        jmp    SHORT loc_1_2288
loc_1_2264:
        cmp    edx, 1020204h
        je     NEAR PTR loc_1_23E0
        cmp    edx, 1020205h
        je     NEAR PTR loc_1_23F1
        cmp    edx, 1020206h
        je     NEAR PTR loc_1_2402
loc_1_2288:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2657
loc_1_2294:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 9bh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_22A5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 9fh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_22B6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0afh]
        add    eax, dword ptr [edi + 0b3h]
        add    eax, dword ptr [edi + 0a7h]
        add    eax, dword ptr [edi + 0abh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_22D9:
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
        jmp    NEAR PTR loc_1_2657
loc_1_2334:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2657
loc_1_2347:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2657
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2360:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a3h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_2371:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2657
loc_1_2384:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0bbh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_2395:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0bfh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_23A6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0b7h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_23B7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a7h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_23C8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_2657
        db  005h, 000h, 000h, 000h, 000h
loc_1_23E0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0afh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_23F1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0b3h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_2402:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0abh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_2413:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_2428
        db  0BEh, 00Ch, 036h, 000h, 000h
        mov    ecx, 0a0h
        jmp    NEAR PTR loc_1_2657
loc_1_2428:
        db  0BEh, 0ACh, 036h, 000h, 000h
        mov    ecx, 5ch
        jmp    NEAR PTR loc_1_2657
loc_1_2437:
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 8fh], 0
        je     SHORT loc_1_2450
        mov    dword ptr [ebp - 10h], 2
        jmp    SHORT loc_1_2464
        db  08Dh, 064h, 024h, 000h
loc_1_2450:
        cmp    byte ptr [edi + 53h], 0
        mov    dword ptr [ebp - 10h], 1
        jne    SHORT loc_1_2464
        mov    dword ptr [ebp - 10h], 0
loc_1_2464:
        lea    esi, [ebp - 10h]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_2657
loc_1_2471:
        lea    esi, [ebp - 1ch]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_2657
        db  08Bh, 0FFh
loc_1_2480:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2657
        db  08Dh, 064h, 024h, 000h
loc_1_2490:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2657
        db  08Dh, 064h, 024h, 000h
loc_1_24A0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 21h]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
        db  08Bh, 0FFh
loc_1_24C0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 25h]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
        db  08Bh, 0FFh
loc_1_24E0:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2657
        db  08Dh, 064h, 024h, 000h
loc_1_24F0:
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
        jmp    NEAR PTR loc_1_2657
loc_1_2516:
        db  0BEh, 008h, 037h, 000h, 000h
        mov    ecx, 0fh
        jmp    NEAR PTR loc_1_2657
loc_1_2525:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_2540
        mov    eax, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2540:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 166h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2657
loc_1_2554:
        mov    word ptr [ebp - 4], 300h
        lea    esi, [ebp - 4]
        mov    ecx, 2
        jmp    NEAR PTR loc_1_2657
loc_1_2567:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_2657
loc_1_2573:
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
        jmp    NEAR PTR loc_1_2657
loc_1_2598:
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
        jmp    NEAR PTR loc_1_2657
        db  08Dh, 049h, 000h
loc_1_25C0:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_25F8
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
        db  0E8h, 07Ah, 0E1h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp + 14h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_2657
loc_1_25F8:
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
        db  0E8h, 085h, 0E0h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 18h], 0
        mov    esi, dword ptr [ebp + 14h]
        jne    SHORT loc_1_2633
        mov    eax, dword ptr [ebp - 14h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_2657
loc_1_2633:
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 166h]
        push   eax
        db  0E8h, 01Dh, 0E0h, 0FEh, 0FFh
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_2657
        db  090h
loc_1_2650:
        mov    dword ptr [ebp - 8], 10h
loc_1_2657:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_268E
        cmp    dword ptr [ebp + 18h], ecx
        jae    SHORT loc_1_267A
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
loc_1_267A:
        mov    eax, dword ptr [ebp + 1ch]
        or     ecx, ecx
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_268E
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_05EC
loc_1_268E:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_269C:
        adc    esp, dword ptr [eax + eax]
        db  000h, 037h
        and    al, 0
        db  000h, 071h, 024h
        add    byte ptr [eax], al
        db  071h, 024h
        add    byte ptr [eax], al
        and    byte ptr [eax + eax], 0
        and    byte ptr [eax + eax], 0
        nop
        and    al, 0
        db  000h, 0A0h, 024h, 000h, 000h, 0C0h
        and    al, 0
        db  000h, 0E0h
        and    al, 0
        db  000h, 0E0h
        and    al, 0
        db  000h, 0F0h
        and    al, 0
        db  000h, 016h
        db  025h, 000h, 000h, 025h, 025h
        add    byte ptr [eax], al
        and    byte ptr [eax + eax], 0
        push   esp
        db  025h, 000h, 000h, 0E0h, 024h
        add    byte ptr [eax], al
        db  0A1h, 021h, 000h, 000h, 067h
        and    eax, 648d0000h
        and    al, 0
        lea    esp, [esp]
loc_1_26F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_2730
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 2
        je     SHORT loc_1_2714
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_2735
loc_1_2714:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        push   eax
        push   esi
        call   sub_1_1D30
        mov    edi, 103h
        jmp    SHORT loc_1_2735
        db  090h
loc_1_2730:
        mov    edi, 0c001000dh
loc_1_2735:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_276D
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_275F
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_275F
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_276D
loc_1_275F:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_276D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2780:
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
        push   1
        mov    word ptr [eax + 0ffc2h], 10h
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
        mov    byte ptr [esi + 4eh], 1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_27E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        push   1
        mov    word ptr [eax + 0ffc2h], 440h
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        push   1
        mov    word ptr [eax + 0ffc2h], 80h
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2880:
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
        jne    SHORT loc_1_28F8
        cmp    byte ptr [ebx + 18h], 0
        jne    SHORT loc_1_28B2
        inc    dword ptr [ebx + 14h]
        push   ebx
        push   edi
        call   sub_1_2F40
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_28B7
loc_1_28B2:
        mov    esi, 0c0010002h
loc_1_28B7:
        mov    eax, dword ptr [edi + 59h]
        dec    eax
        mov    dword ptr [edi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_28EF
        cmp    byte ptr [edi + 8fh], 0
        jne    SHORT loc_1_28E1
        cmp    dword ptr [edi + 75h], 0
        jne    SHORT loc_1_28E1
        mov    eax, dword ptr [edi + 0d9h]
        sub    eax, edi
        cmp    eax, 0d9h
        je     SHORT loc_1_28EF
loc_1_28E1:
        push   0
        add    edi, 155h
        push   edi
        call   sub_1_04E2
loc_1_28EF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_28F8:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2910:
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
        je     SHORT loc_1_2950
        mov    edi, dword ptr [eax + 0ch]
loc_1_2950:
        test   edi, 20h
        je     SHORT loc_1_2963
        mov    word ptr [esi + 0cbh], 3
        jmp    SHORT loc_1_2987
loc_1_2963:
        test   edi, 4
        je     SHORT loc_1_2976
        mov    word ptr [esi + 0cbh], 3
        jmp    SHORT loc_1_2987
loc_1_2976:
        test   edi, 8
        je     SHORT loc_1_2987
        mov    word ptr [esi + 0cbh], 0
loc_1_2987:
        cmp    byte ptr [esi + 56h], 0
        jne    SHORT loc_1_2996
        or     word ptr [esi + 0cbh], 8800h
loc_1_2996:
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
        call   sub_1_0940
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 2], 1
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 4], 0ffffh
        sub    eax, eax
        lea    ebx, [ebx]
loc_1_2A00:
        mov    cl, byte ptr [esi + eax + 2fh]
        mov    edx, dword ptr [esi + 1dh]
        mov    byte ptr [eax + edx + 6], cl
        inc    eax
        cmp    eax, 6
        jb     SHORT loc_1_2A00
        push   esi
        call   sub_1_0940
        test   edi, 2
        je     SHORT loc_1_2A51
        lea    edi, [esi + 272h]
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 166h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 024h, 0DDh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_2A51
        push   1
        push   1
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_1BB0
loc_1_2A51:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 4]
loc_1_2A71:
        or     edi, edi
        je     SHORT loc_1_2A81
        cmp    edi, 1
        je     SHORT loc_1_2A86
        cmp    edi, 2
        je     SHORT loc_1_2A90
        jmp    SHORT loc_1_2A96
loc_1_2A81:
        mov    ebx, dword ptr [esi + 75h]
        jmp    SHORT loc_1_2A96
loc_1_2A86:
        mov    ebx, dword ptr [esi + 7dh]
        jmp    SHORT loc_1_2A96
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A90:
        mov    ebx, dword ptr [esi + 87h]
loc_1_2A96:
        or     ebx, ebx
        je     SHORT loc_1_2ABA
loc_1_2A9A:
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
        jne    SHORT loc_1_2A9A
loc_1_2ABA:
        inc    edi
        cmp    edi, 3
        jb     SHORT loc_1_2A71
        push   esi
        call   sub_1_2AD0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_2AD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0AC0
        push   esi
        call   sub_1_2C40
        mov    bl, al
        or     bl, bl
        je     SHORT loc_1_2B42
        push   esi
        call   sub_1_3040
        push   esi
        call   sub_1_2910
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
        call   sub_1_2DF0
        cmp    byte ptr [esi + 53h], 0
        jne    SHORT loc_1_2B35
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
loc_1_2B35:
        mov    byte ptr [esi + 53h], 0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2B42:
        push   0c0000001h
        push   esi
        call   sub_1_2DF0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 4fh], 0
        je     SHORT loc_1_2BE9
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    dword ptr [esi + 49h], eax
        sub    ebx, ebx
loc_1_2B85:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_2BAC
        push   32h
        call   sub_1_0500
        inc    edi
        cmp    edi, 4e20h
        jbe    SHORT loc_1_2B85
loc_1_2BAC:
        push   esi
        db  068h, 080h, 027h, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
        sub    edi, edi
        sub    ebx, ebx
loc_1_2BC2:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0ffc2h]
        cmp    ax, bx
        je     SHORT loc_1_2BE9
        push   32h
        call   sub_1_0500
        inc    edi
        cmp    edi, 4e20h
        jbe    SHORT loc_1_2BC2
loc_1_2BE9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 53h], 0
        jne    SHORT loc_1_2C22
        push   esi
        db  068h, 0E0h, 027h, 000h, 000h
        lea    eax, [esi + 131h]
        push   eax
        call   sub_1_04FA
loc_1_2C22:
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
sub_1_2C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 6
        in     al, dx
        test   al, 8
        je     SHORT loc_1_2C61
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, 40h
        out    dx, al
        in     al, dx
loc_1_2C61:
        push   esi
        call   sub_1_2FE0
        cmp    byte ptr [esi + 4fh], 0
        jne    SHORT loc_1_2CB9
        mov    eax, dword ptr [esi + 0ah]
        shl    eax, 0ah
        push   eax
        mov    eax, dword ptr [esi + 6]
        push   eax
        mov    eax, dword ptr [esi + 0f1h]
        lea    ecx, [esi + 2]
        push   eax
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2CB5
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
loc_1_2CB5:
        mov    byte ptr [esi + 4fh], 1
loc_1_2CB9:
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
        xor    ecx, ecx
        mov    cx, word ptr [esi + 1ah]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0ffc0h
        mov    dword ptr [esi + 16h], eax
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
        mov    dword ptr [esi + 5dh], ecx
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, 83h
        out    dx, al
        push   3e8h
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, 7
        out    dx, al
        mov    eax, dword ptr [esi + 0eh]
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
        push   1
        mov    word ptr [eax + 0eh], 0
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 1ch]
        or     al, 40h
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 1ch]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2DF0:
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
        je     SHORT loc_1_2E58
loc_1_2E10:
        inc    dword ptr [ebx + 14h]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_2E2B
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_046E
loc_1_2E2B:
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
        jne    SHORT loc_1_2E10
loc_1_2E58:
        or     edi, edi
        je     SHORT loc_1_2E6C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0456
        dec    dword ptr [edi + 14h]
loc_1_2E6C:
        mov    byte ptr [esi + 8fh], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_2E99
        mov    eax, dword ptr [esi + 3dh]
        shl    eax, 4
        add    eax, dword ptr [esi + 16eh]
        push   eax
        push   esi
        call   sub_1_2B60
        push   esi
        call   sub_1_1D70
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2E99:
        push   1
        push   esi
        call   sub_1_2EB0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2EB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 35h]
        add    esi, 35h
        or     edi, edi
        je     SHORT loc_1_2F36
        cmp    dword ptr [edi + 10h], 4
        jne    SHORT loc_1_2ED5
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 14h]
        mov    edi, dword ptr [esi]
loc_1_2ED5:
        or     edi, edi
        je     SHORT loc_1_2F36
loc_1_2ED9:
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 4
        jne    SHORT loc_1_2EE5
        mov    esi, edi
        jmp    SHORT loc_1_2F30
loc_1_2EE5:
        cmp    eax, 3
        jne    SHORT loc_1_2F17
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_2F13
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
        jmp    SHORT loc_1_2F30
loc_1_2F13:
        mov    esi, edi
        jmp    SHORT loc_1_2F30
loc_1_2F17:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    ebx, dword ptr [edi + 4]
        push   0c001000ch
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 14h]
loc_1_2F30:
        mov    edi, dword ptr [esi]
        or     edi, edi
        jne    SHORT loc_1_2ED9
loc_1_2F36:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_2F40:
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
        call   sub_1_2C00
        mov    byte ptr [esi + 1ch], 87h
        mov    edi, dword ptr [esi + 0d1h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff2fh
        je     SHORT loc_1_2FA4
loc_1_2F76:
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
        jne    SHORT loc_1_2F76
loc_1_2FA4:
        mov    byte ptr [esi + 8fh], 1
        mov    byte ptr [esi + 85h], 0
        push   0
        push   esi
        call   sub_1_2EB0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 93h], eax
        or     eax, eax
        je     SHORT loc_1_2FCA
        inc    dword ptr [eax + 14h]
loc_1_2FCA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2FE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [esi + 29h], al
        inc    dx
        in     al, dx
        mov    byte ptr [esi + 2ah], al
        mov    dx, word ptr [esi]
        add    dx, 2
        in     al, dx
        mov    byte ptr [esi + 2bh], al
        mov    dx, word ptr [esi]
        add    dx, 3
        in     al, dx
        mov    byte ptr [esi + 2ch], al
        mov    dx, word ptr [esi]
        add    dx, 4
        in     al, dx
        mov    byte ptr [esi + 2dh], al
        mov    dx, word ptr [esi]
        add    dx, 5
        in     al, dx
        mov    byte ptr [esi + 2eh], al
        cmp    byte ptr [esi + 55h], 0
        jne    SHORT loc_1_303B
        mov    ecx, dword ptr [esi + 29h]
        mov    dword ptr [esi + 2fh], ecx
        mov    cl, byte ptr [esi + 2dh]
        mov    byte ptr [esi + 33h], cl
        lea    eax, [esi + 2fh]
        mov    cl, byte ptr [esi + 2eh]
        mov    byte ptr [eax + 5], cl
loc_1_303B:
        pop    esi
        pop    ebp
        ret    4
sub_1_3040:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [esi + 61h]
        cmp    dword ptr [esi + 21h], 0
        jbe    NEAR PTR loc_1_3123
        sub    edx, edx
        sub    eax, eax
loc_1_3064:
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
        inc    dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + edx - 0fh]
        and    ecx, 0ffffh
        mov    dword ptr [ebx + 14h], ecx
        mov    ecx, dword ptr [esi + 16ah]
        mov    ebx, dword ptr [edx + ecx - 13h]
        add    ebx, 5f0h
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 21h], ecx
        ja     NEAR PTR loc_1_3064
loc_1_3123:
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
        mov    dword ptr [ebp - 8], 0
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 16ah]
        mov    word ptr [ecx + eax + 0ah], 0ffffh
        mov    eax, dword ptr [esi + 1dh]
        mov    word ptr [eax + 4], 0ffffh
        mov    ebx, dword ptr [esi + 5dh]
        cmp    dword ptr [esi + 25h], 0
        jbe    NEAR PTR loc_1_32B2
        mov    dword ptr [ebp - 4], 0
loc_1_31F0:
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
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [eax + edi], ebx
        mov    cx, word ptr [esi + 1ah]
        sub    cx, word ptr [esi + 2]
        add    cx, bx
        mov    eax, dword ptr [esi + 16eh]
        mov    word ptr [edi + eax + 4], cx
        lea    ecx, [ebx + 16h]
        sub    cx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 16eh]
        mov    eax, dword ptr [edi + eax]
        sub    edx, edx
        add    cx, word ptr [esi + 1ah]
        mov    word ptr [eax + 6], cx
        mov    ecx, dword ptr [ebp - 8]
        inc    ecx
        mov    eax, ecx
        div    dword ptr [esi + 25h]
        mov    eax, dword ptr [esi + 16eh]
        mov    dword ptr [edi + eax + 0ch], edx
        add    ebx, 20h
        mov    eax, dword ptr [esi + 16eh]
        mov    dword ptr [edi + eax + 8], ebx
        mov    eax, dword ptr [esi + 16eh]
        add    eax, edi
        mov    edx, dword ptr [eax + 8]
        add    dword ptr [ebp - 4], 10h
        sub    dx, word ptr [esi + 2]
        add    dx, word ptr [esi + 1ah]
        mov    word ptr [eax + 6], dx
        mov    eax, dword ptr [esi + 16eh]
        mov    ebx, dword ptr [edi + eax + 8]
        add    ebx, 5f0h
        mov    dword ptr [ebp - 8], ecx
        cmp    dword ptr [esi + 25h], ecx
        ja     NEAR PTR loc_1_31F0
loc_1_32B2:
        sub    ecx, ecx
        cmp    dword ptr [esi + 25h], ecx
        jbe    SHORT loc_1_3307
        sub    edx, edx
        add    eax, 0
loc_1_32C0:
        mov    edi, dword ptr [esi + 16eh]
        add    edi, edx
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
        add    edx, 10h
        inc    ecx
        cmp    dword ptr [esi + 25h], ecx
        ja     SHORT loc_1_32C0
loc_1_3307:
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
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3330:
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
        jne    NEAR PTR loc_1_3450
        cmp    byte ptr [ebx + 18h], 0
        jne    NEAR PTR loc_1_3440
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
        je     NEAR PTR loc_1_3434
        cmp    dword ptr [ebp - 0ch], 5eah
        ja     NEAR PTR loc_1_3434
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
        je     SHORT loc_1_33C3
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0dh], 0
        jmp    SHORT loc_1_33D9
loc_1_33C3:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 166h]
        push   eax
        call   sub_1_0330
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0dh], al
loc_1_33D9:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], ebx
        mov    eax, dword ptr [esi + 8bh]
        or     eax, eax
        jne    SHORT loc_1_33F1
        mov    dword ptr [esi + 87h], edi
        jmp    SHORT loc_1_33F4
loc_1_33F1:
        mov    dword ptr [eax + 24h], edi
loc_1_33F4:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 8bh], edi
        mov    dword ptr [ecx], 0
        inc    dword ptr [esi + 0a3h]
        cmp    byte ptr [esi + 86h], 0
        jne    SHORT loc_1_3457
        sub    ebx, ebx
loc_1_3414:
        cmp    dword ptr [esi + 87h], ebx
        je     SHORT loc_1_3457
        cmp    byte ptr [esi + 85h], bl
        je     SHORT loc_1_3457
        push   esi
        call   sub_1_34A0
        cmp    byte ptr [esi + 86h], bl
        je     SHORT loc_1_3414
        jmp    SHORT loc_1_3457
loc_1_3434:
        dec    dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 4], 0c000009ah
        jmp    SHORT loc_1_3457
loc_1_3440:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_3457
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3450:
        mov    dword ptr [ebp - 4], 0c001000dh
loc_1_3457:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_348F
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_3481
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_3481
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_348F
loc_1_3481:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_348F:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_34A0:
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
        call   sub_1_0A00
        or     al, al
        je     SHORT loc_1_3508
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 87h], eax
        or     eax, eax
        jne    SHORT loc_1_34DD
        mov    dword ptr [esi + 8bh], 0
loc_1_34DD:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_1_0710
        push   edi
        push   esi
        call   sub_1_1850
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_0870
        mov    byte ptr [esi + 86h], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3508:
        mov    byte ptr [esi + 86h], 0
        mov    byte ptr [esi + 85h], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_3520:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 59h]
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_3590
        cmp    byte ptr [edi + 18h], 0
        jne    SHORT loc_1_3587
        inc    dword ptr [edi + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        test   cl, 1
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        jne    SHORT loc_1_356A
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
        jmp    SHORT loc_1_3595
loc_1_356A:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        and    ecx, 0fffffffeh
        push   eax
        add    ecx, dword ptr [ebp + 10h]
        push   ecx
        call   sub_1_1970
        dec    dword ptr [edi + 14h]
        sub    edi, edi
        jmp    SHORT loc_1_3595
loc_1_3587:
        mov    edi, 0c001000ch
        jmp    SHORT loc_1_3595
        db  08Bh, 0FFh
loc_1_3590:
        mov    edi, 0c001000dh
loc_1_3595:
        mov    eax, dword ptr [esi + 59h]
        dec    eax
        mov    dword ptr [esi + 59h], eax
        cmp    eax, 1
        jne    SHORT loc_1_35CD
        cmp    byte ptr [esi + 8fh], 0
        jne    SHORT loc_1_35BF
        cmp    dword ptr [esi + 75h], 0
        jne    SHORT loc_1_35BF
        mov    eax, dword ptr [esi + 0d9h]
        sub    eax, esi
        cmp    eax, 0d9h
        je     SHORT loc_1_35CD
loc_1_35BF:
        push   0
        add    esi, 155h
        push   esi
        call   sub_1_04E2
loc_1_35CD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_35E4:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        dec    ebp
        inc    ebx
        add    byte ptr [eax], al
loc_1_35EC:
        pop    es
        add    byte ptr [edi], al
        db  000h
loc_1_35F0:
        in     al, 35h
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
loc_1_360C:
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
        add    eax, 0d000201h
        add    al, byte ptr [edx]
        add    byte ptr [esi], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
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
loc_1_3708:
        inc    ebp
        insb
        outsb
        imul   ecx, dword ptr [ebp + 43h], 20h
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_3717
loc_1_3717:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
