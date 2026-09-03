; nice_lcode_01.asm -- NICE object 1 segment _LTEXT
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
        call   sub_1_17E0
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
NICE_Control:
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 04Eh, 049h, 043h, 045h
        db  020h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        db  0BFh, 0E8h, 032h, 000h, 000h
        mov    eax, dword ptr [edx + 38h]
        cmp    dword ptr [edx + 28h], 2
        je     SHORT loc_1_06C0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_06C0:
        lea    esi, [eax + 14h]
        db  080h, 03Dh, 0E8h, 032h, 000h, 000h, 000h
        je     SHORT loc_1_06DD
loc_1_06CC:
        mov    ecx, edi
        inc    edi
        mov    bl, byte ptr [esi]
        cmp    byte ptr [ecx], bl
        jne    SHORT loc_1_06F3
        add    esi, 2
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_06CC
loc_1_06DD:
        mov    byte ptr [eax + 806h], 0c4h
        test   byte ptr [eax + 806h], 2
        je     SHORT loc_1_0700
        mov    byte ptr [edx + 40h], 0
        jmp    SHORT loc_1_070B
loc_1_06F3:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0700:
        mov    byte ptr [edx + 40h], 1
        mov    byte ptr [eax + 806h], 45h
loc_1_070B:
        push   30d40h
        call   sub_1_0500
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0720:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 28h]
        cmp    eax, 1
        jne    SHORT loc_1_074B
        sub    ecx, ecx
loc_1_0731:
        mov    edx, dword ptr [esi + 30h]
        add    dx, cx
        add    dx, 1ah
        in     al, dx
        mov    byte ptr [esi + ecx + 4ch], al
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_0731
        pop    esi
        pop    ebp
        ret    4
loc_1_074B:
        cmp    eax, 2
        jne    SHORT loc_1_0766
        sub    eax, eax
loc_1_0752:
        mov    ecx, dword ptr [esi + 38h]
        inc    eax
        mov    cl, byte ptr [ecx + eax*2 + 0cch]
        mov    byte ptr [esi + eax + 4bh], cl
        cmp    eax, 6
        jl     SHORT loc_1_0752
loc_1_0766:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0770:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 41h], 52h
        cmp    dword ptr [ecx + 28h], 2
        jne    SHORT loc_1_0794
        cmp    byte ptr [ecx + 40h], 0
        je     SHORT loc_1_0794
        lea    edi, [ecx + 42h]
        mov    byte ptr [edi], 20h
        jmp    SHORT loc_1_079A
loc_1_0794:
        lea    edi, [ecx + 42h]
        mov    byte ptr [edi], 0a0h
loc_1_079A:
        mov    si, word ptr [ecx + 30h]
        mov    al, byte ptr [ecx + 41h]
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
        mov    dx, word ptr [ecx + 30h]
        out    dx, al
        inc    dx
        mov    si, word ptr [ecx + 30h]
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
loc_1_07D9:
        mov    al, bl
        and    eax, 0ffff00ffh
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0ffffh
        mov    al, byte ptr [ecx + eax + 52h]
        out    dx, al
        inc    bl
        cmp    bl, 6
        jb     SHORT loc_1_07D9
        mov    al, byte ptr [edi]
        or     al, 4
        lea    edx, [esi + 7]
        out    dx, al
        mov    eax, dword ptr [ecx + 7ah]
        test   al, 20h
        jne    SHORT loc_1_082B
        test   al, 4
        je     SHORT loc_1_082B
        sub    bl, bl
        lea    ecx, [ecx]
loc_1_0810:
        mov    dl, bl
        and    edx, 0ffff00ffh
        add    dx, si
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0810
        jmp    SHORT loc_1_084F
loc_1_082B:
        sub    bl, bl
        lea    ecx, [ecx]
loc_1_0830:
        mov    al, bl
        and    eax, 0ffff00ffh
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0ffffh
        mov    al, byte ptr [ecx + eax + 44h]
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0830
loc_1_084F:
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
        mov    al, byte ptr [ecx + 41h]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0880:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 30h]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0ffh
        je     SHORT loc_1_089D
        or     al, al
        jne    SHORT loc_1_08D4
loc_1_089D:
        mov    edx, dword ptr [esi + 30h]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        not    al
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 2]
        not    eax
        cmp    eax, ecx
        jne    SHORT loc_1_08D2
        mov    edx, dword ptr [esi + 30h]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_08D2:
        sub    al, al
loc_1_08D4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_08E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 2
        mov    si, word ptr [ecx + 30h]
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [ecx + 43h]
        or     al, 4
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 80h
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 81h
        out    dx, al
        cmp    dword ptr [ecx + 28h], 1
        jne    SHORT loc_1_0912
        lea    edx, [esi + 10h]
        mov    al, 1dh
        out    dx, al
loc_1_0912:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0920:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    cx, word ptr [eax + 30h]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 28h], 1
        jne    SHORT loc_1_095C
        mov    edx, dword ptr [ebx + 30h]
        add    dx, 10h
        mov    al, 0fh
        out    dx, al
loc_1_095C:
        mov    edx, dword ptr [ebx + 30h]
        add    dx, 5
        mov    al, 6
        out    dx, al
        db  0BEh, 0E0h, 032h, 000h, 000h
        add    eax, 0
loc_1_0970:
        mov    ecx, dword ptr [ebx + 30h]
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
        db  081h, 0FEh, 0E8h, 032h, 000h, 000h
        jb     SHORT loc_1_0970
        mov    edx, dword ptr [ebx + 30h]
        add    dx, 0ah
        mov    al, 82h
        out    dx, al
        push   0fah
        call   sub_1_0500
        mov    dword ptr [ebp - 10h], 0
        db  0C7h, 045h, 0F8h, 0E0h, 032h, 000h, 000h
loc_1_09B7:
        mov    edx, dword ptr [ebx + 30h]
        add    dx, 5
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0A15
        push   2
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebx + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        cmp    word ptr [ebp - 0ah], 3dh
        jne    SHORT loc_1_0A15
        push   1fh
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebx + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        lea    edi, [ebp - 50h]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 0fh
        mov    esi, dword ptr [eax]
        repe cmpsd
        jne    SHORT loc_1_0A15
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_1_0A15
        add    dword ptr [ebp - 8], 4
        inc    dword ptr [ebp - 10h]
        db  081h, 07Dh, 0F8h, 0E8h, 032h, 000h, 000h
        jb     SHORT loc_1_09B7
loc_1_0A15:
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 2
        cmp    eax, 1
        sbb    cl, cl
        neg    cl
        mov    dx, word ptr [ebx + 30h]
        in     al, dx
        mov    dx, word ptr [ebx + 30h]
        out    dx, al
        inc    dx
        cmp    dword ptr [ebx + 28h], 1
        mov    si, word ptr [ebx + 30h]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, 4
        out    dx, al
        jne    SHORT loc_1_0A49
        lea    edx, [esi + 10h]
        mov    al, 0dh
        out    dx, al
loc_1_0A49:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0920
        push   esi
        call   sub_1_0770
        push   esi
        call   sub_1_08E0
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_0ACB
loc_1_0A92:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0AA0:
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
        je     SHORT loc_1_0AC2
        xor    eax, 4c11db7h
loc_1_0AC2:
        dec    esi
        jne    SHORT loc_1_0AA0
        inc    edx
        cmp    edx, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_0A92
loc_1_0ACB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AE0:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0A80
        sub    ecx, ecx
        mov    edx, 1
loc_1_0AF5:
        add    ecx, ecx
        test   eax, edx
        je     SHORT loc_1_0AFE
        or     ecx, 1
loc_1_0AFE:
        add    edx, edx
        cmp    edx, 40h
        jb     SHORT loc_1_0AF5
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
sub_1_0B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
loc_1_0B39:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 44h], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_1_0B39
        db  068h, 000h, 032h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 0FCh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_1_0B97
        sub    edi, edi
loc_1_0B68:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 000h, 032h, 000h, 000h
        push   eax
        call   sub_1_0AE0
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 44h], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_1_0B68
loc_1_0B97:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0BA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 30h]
        mov    al, byte ptr [esi + 41h]
        or     al, 80h
        lea    edx, [edi + 6]
        out    dx, al
        lea    ecx, [edi + 7]
        mov    al, byte ptr [esi + 42h]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0BC4:
        lea    edx, [edi + ebx]
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_0BC4
        mov    al, byte ptr [esi + 42h]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 41h]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0BF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    si, word ptr [edi + 30h]
        mov    al, byte ptr [edi + 41h]
        or     al, 80h
        lea    edx, [esi + 6]
        out    dx, al
        lea    ecx, [esi + 7]
        mov    al, byte ptr [edi + 42h]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0C14:
        lea    edx, [ebx + esi]
        add    dx, 8
        mov    al, byte ptr [edi + ebx + 44h]
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_0C14
        mov    al, byte ptr [edi + 42h]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [edi + 41h]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0C40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 30h]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 6c5h], 0
        jne    NEAR PTR loc_1_0D92
        mov    byte ptr [esi + 6c5h], 1
        inc    dword ptr [esi + 60h]
        sub    edi, edi
        sub    ebx, ebx
loc_1_0C87:
        mov    dx, word ptr [esi + 30h]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 30h]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, bl
        jne    SHORT loc_1_0CA1
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_0D00
loc_1_0CA1:
        test   byte ptr [ebp - 2], 81h
        je     SHORT loc_1_0CBF
        test   byte ptr [ebp - 2], 1
        je     SHORT loc_1_0CB3
        inc    dword ptr [esi + 69ch]
loc_1_0CB3:
        push   esi
        call   sub_1_0DA0
        push   esi
        call   sub_1_15E0
loc_1_0CBF:
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_0CCA
        cmp    byte ptr [esi + 78h], bl
        je     SHORT loc_1_0CFA
loc_1_0CCA:
        mov    dx, word ptr [esi + 30h]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dword ptr [esi + 64h], ebx
        cmp    byte ptr [esi + 78h], bl
        je     SHORT loc_1_0CE3
        mov    byte ptr [esi + 79h], 2
        mov    byte ptr [esi + 78h], bl
        jmp    SHORT loc_1_0CE9
loc_1_0CE3:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 79h], al
loc_1_0CE9:
        mov    byte ptr [esi + 6eah], bl
        cmp    dword ptr [esi + 74h], ebx
        je     SHORT loc_1_0CFA
        push   esi
        call   sub_1_0F90
loc_1_0CFA:
        inc    edi
        cmp    edi, 8
        jl     SHORT loc_1_0C87
loc_1_0D00:
        cmp    dword ptr [esi + 6adh], 0
        je     SHORT loc_1_0D28
        lea    esp, [esp]
loc_1_0D10:
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_0D28
        push   esi
        call   sub_1_0DA0
        cmp    dword ptr [esi + 6adh], 0
        jne    SHORT loc_1_0D10
loc_1_0D28:
        mov    cx, word ptr [esi + 30h]
        mov    byte ptr [esi + 6c5h], 0
        lea    edx, [ecx + 2]
        mov    al, 80h
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 81h
        out    dx, al
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    dword ptr [esi + 60h], eax
        jne    SHORT loc_1_0D77
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_0D60
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_0D60
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_0D77
loc_1_0D60:
        push   1
        add    esi, 6c6h
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0D77:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_0D92
        cmp    dword ptr [esi + 74h], 0
        jne    SHORT loc_1_0D92
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_0D92
        push   esi
        call   sub_1_15E0
loc_1_0D92:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_0DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        mov    esi, dword ptr [ebp + 8]
loc_1_0DB4:
        cmp    byte ptr [esi + 6a0h], 0
        jne    NEAR PTR loc_1_0F00
        mov    cx, word ptr [esi + 30h]
        mov    al, 81h
        lea    edx, [ecx + 1]
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        test   al, 40h
        jne    NEAR PTR loc_1_0F00
        inc    dword ptr [esi + 64h]
        mov    dword ptr [esi + 6b5h], 0
        push   2
        lea    eax, [esi + 680h]
        push   eax
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    ax, word ptr [esi + 682h]
        mov    word ptr [esi + 7eh], ax
        mov    edi, dword ptr [esi + 6a9h]
        add    di, 0eh
        cmp    di, ax
        jbe    SHORT loc_1_0E16
        mov    di, ax
loc_1_0E16:
        mov    bl, 1
        test   byte ptr [esi + 7ah], 28h
        jne    SHORT loc_1_0E76
        push   3
        lea    eax, [esi + 80h]
        push   eax
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 7eh], 6
        sub    di, 6
        cmp    dword ptr [esi + 82h], -1
        jne    SHORT loc_1_0E60
        cmp    byte ptr [esi + 80h], 0ffh
        jne    SHORT loc_1_0E60
        cmp    byte ptr [esi + 81h], 0ffh
        jne    SHORT loc_1_0E60
        sub    bl, bl
        jmp    SHORT loc_1_0E9B
        db  005h, 000h, 000h, 000h, 000h
loc_1_0E60:
        mov    eax, edi
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 86h]
        jmp    SHORT loc_1_0E8A
loc_1_0E76:
        mov    eax, edi
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 80h]
loc_1_0E8A:
        push   eax
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 7eh], di
loc_1_0E9B:
        or     bl, bl
        je     SHORT loc_1_0EB4
        push   esi
        call   sub_1_1230
        cmp    eax, 3
        je     SHORT loc_1_0EB4
        inc    dword ptr [esi + 68ch]
        mov    byte ptr [ebp - 2], 1
loc_1_0EB4:
        mov    ax, word ptr [esi + 7eh]
        or     ax, ax
        je     SHORT loc_1_0EEB
        cmp    ax, 8
        ja     SHORT loc_1_0EE1
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        jmp    SHORT loc_1_0EEB
loc_1_0EE1:
        mov    edx, dword ptr [esi + 30h]
        add    dx, 0eh
        mov    al, 4
        out    dx, al
loc_1_0EEB:
        cmp    dword ptr [esi + 64h], 5
        jbe    NEAR PTR loc_1_0DB4
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 64h], 0
loc_1_0F00:
        cmp    dword ptr [esi + 6adh], 0
        je     SHORT loc_1_0F52
        lea    esp, [esp]
loc_1_0F10:
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_0F5B
        mov    ebx, dword ptr [esi + 6adh]
        push   ebx
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 6adh], eax
        mov    dword ptr [esi + 6b5h], ebx
        mov    edi, dword ptr [ebx + 28h]
        push   esi
        call   sub_1_1090
        push   0
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        cmp    dword ptr [esi + 6adh], 0
        jne    SHORT loc_1_0F10
loc_1_0F52:
        cmp    byte ptr [esi + 6a0h], 0
        je     SHORT loc_1_0F67
loc_1_0F5B:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0F67:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_0F76
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_0290
loc_1_0F76:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 74h]
        or     ecx, ecx
        jne    SHORT loc_1_0FC1
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
loc_1_0FC1:
        mov    al, byte ptr [esi + 79h]
        test   al, 80h
        je     SHORT loc_1_1000
        test   al, 4
        je     SHORT loc_1_0FF6
        mov    edx, dword ptr [esi + 30h]
        add    dx, 4
        in     al, dx
        and    eax, 0ffh
        and    eax, 0fffffff0h
        cmp    eax, 10h
        jne    SHORT loc_1_0FF0
        inc    dword ptr [esi + 694h]
        inc    dword ptr [esi + 688h]
        jmp    SHORT loc_1_1006
        db  090h
loc_1_0FF0:
        inc    dword ptr [esi + 698h]
loc_1_0FF6:
        inc    dword ptr [esi + 688h]
        jmp    SHORT loc_1_1006
        db  08Bh, 0FFh
loc_1_1000:
        inc    dword ptr [esi + 690h]
loc_1_1006:
        cmp    byte ptr [ecx + 2ch], 0
        jne    SHORT loc_1_1030
        mov    al, byte ptr [esi + 79h]
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
        jmp    SHORT loc_1_1057
        db  08Dh, 064h, 024h, 000h
loc_1_1030:
        cmp    dword ptr [esi + 6adh], 0
        jne    SHORT loc_1_1041
        mov    dword ptr [esi + 6adh], ecx
        jmp    SHORT loc_1_104A
loc_1_1041:
        mov    eax, dword ptr [esi + 6b1h]
        mov    dword ptr [eax + 24h], ecx
loc_1_104A:
        mov    dword ptr [esi + 6b1h], ecx
        mov    dword ptr [ecx + 24h], 0
loc_1_1057:
        mov    dword ptr [esi + 74h], 0
        cmp    byte ptr [esi + 6a0h], 0
        je     SHORT loc_1_1077
        push   2
        push   esi
        call   sub_1_2E10
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1077:
        push   esi
        call   sub_1_15E0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1090:
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
        jae    SHORT loc_1_10C0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_10C0:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 6a9h]
        add    edi, 0eh
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_10D4
        mov    edi, dword ptr [ebp - 4]
loc_1_10D4:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 80h]
        push   eax
        call   sub_1_1150
        cmp    edi, 0eh
        jae    SHORT loc_1_1110
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 80h]
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
loc_1_1110:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        sub    edi, 0eh
        push   edi
        lea    eax, [esi + 8eh]
        push   eax
        push   0eh
        lea    eax, [esi + 80h]
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
sub_1_1150:
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
        je     SHORT loc_1_11A4
loc_1_1175:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_11A4
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1175
loc_1_11A4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_11B5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_11B5:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0ch], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_11C4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_11D3
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_11D6
loc_1_11D3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_11D6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05EC
        add    esi, ebx
        cmp    esi, edi
        je     SHORT loc_1_1212
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_121D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        jmp    SHORT loc_1_11C4
loc_1_1212:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_121D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1230:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 682h]
        cmp    eax, 5eah
        jbe    SHORT loc_1_1250
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_1250:
        mov    dword ptr [edx + 684h], eax
        mov    ecx, dword ptr [edx + 6a9h]
        add    ecx, 0eh
        cmp    ecx, eax
        jbe    SHORT loc_1_1265
        mov    ecx, eax
loc_1_1265:
        cmp    ecx, 0eh
        jae    SHORT loc_1_1289
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 80h]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 24h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1289:
        sub    eax, 0eh
        push   eax
        sub    ecx, 0eh
        push   ecx
        lea    eax, [edx + 8eh]
        push   eax
        push   0eh
        lea    eax, [edx + 80h]
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
loc_1_12C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 6b5h], 0
        je     NEAR PTR loc_1_1367
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, edi
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_133A
loc_1_12FA:
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
        mov    eax, dword ptr [esi + 6b5h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1150
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_133A
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_12FA
loc_1_133A:
        cmp    edi, ebx
        jbe    SHORT loc_1_1355
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1355:
        mov    ecx, dword ptr [ebp + 1ch]
        sub    ebx, edi
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1367:
        mov    ax, word ptr [esi + 7eh]
        or     ax, ax
        je     SHORT loc_1_139C
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        mov    eax, dword ptr [esi + 6a9h]
        add    eax, esi
        add    eax, 8eh
        push   eax
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    word ptr [esi + 7eh], 0
loc_1_139C:
        mov    eax, dword ptr [esi + 684h]
        mov    ecx, dword ptr [ebp + 14h]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    SHORT loc_1_13D0
        cmp    eax, edi
        jae    SHORT loc_1_13C7
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_13C7:
        sub    eax, edi
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_1_13D3
        db  08Bh, 0FFh
loc_1_13D0:
        mov    dword ptr [ebp - 18h], ecx
loc_1_13D3:
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
        je     SHORT loc_1_1454
loc_1_1403:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     SHORT loc_1_1410
        mov    ebx, dword ptr [ebp - 8]
loc_1_1410:
        push   ebx
        lea    eax, [edi + esi]
        add    eax, 80h
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05EC
        add    edi, ebx
        sub    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_1454
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1454
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1403
loc_1_1454:
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1470:
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
        je     SHORT loc_1_14A0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_14A0:
        cmp    byte ptr [esi + 6a0h], 0
        je     SHORT loc_1_14B7
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_14B7:
        inc    dword ptr [ebx + 18h]
        inc    dword ptr [esi + 60h]
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
        mov    edx, dword ptr [esi + 54h]
        lea    ecx, [esi + 52h]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    edx, eax
        jae    SHORT loc_1_1503
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1537
loc_1_1503:
        jbe    SHORT loc_1_1510
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_1537
        db  08Bh, 0FFh
loc_1_1510:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    SHORT loc_1_1527
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1537
loc_1_1527:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_1_1537
        mov    dword ptr [ebp - 4], 0
loc_1_1537:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1557
        test   byte ptr [ebx + 1dh], 2
        je     SHORT loc_1_1545
        jmp    SHORT loc_1_1557
loc_1_1545:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_0330
        mov    byte ptr [edi + 8], al
        jmp    SHORT loc_1_155B
loc_1_1557:
        mov    byte ptr [edi + 8], 0
loc_1_155B:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_1570
        mov    eax, dword ptr [esi + 70h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_1576
        db  08Dh, 064h, 024h, 000h
loc_1_1570:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 6ch], ecx
loc_1_1576:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 70h], ecx
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    dword ptr [esi + 60h], eax
        jne    SHORT loc_1_15B0
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_159D
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_159D
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_15B0
loc_1_159D:
        push   1
        add    esi, 6c6h
        push   esi
        call   sub_1_04E2
        jmp    SHORT loc_1_15CB
        db  08Dh, 049h, 000h
loc_1_15B0:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_15CB
        cmp    dword ptr [esi + 74h], 0
        jne    SHORT loc_1_15CB
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_15CB
        push   esi
        call   sub_1_15E0
loc_1_15CB:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 74h], 0
        jne    NEAR PTR loc_1_176C
        mov    di, word ptr [ebp - 4]
        sub    ebx, ebx
loc_1_15FC:
        mov    eax, dword ptr [esi + 6ch]
        cmp    eax, ebx
        je     NEAR PTR loc_1_176C
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esi + 6ch], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 74h], eax
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
        jb     SHORT loc_1_1637
        mov    eax, dword ptr [ebp - 14h]
loc_1_1637:
        mov    edx, dword ptr [esi + 30h]
        add    dx, 8
        cmp    dword ptr [ebp - 8], ebx
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        je     NEAR PTR loc_1_16DE
loc_1_164C:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0ch], ebx
        jne    SHORT loc_1_1664
        jmp    SHORT loc_1_16C8
loc_1_1664:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_1696
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        inc    dword ptr [ebp - 10h]
        shl    ax, 8
        dec    dword ptr [ebp - 0ch]
        or     di, ax
        mov    edx, dword ptr [esi + 30h]
        add    dx, 8
        cmp    dword ptr [ebp - 0ch], ebx
        mov    ax, di
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        jne    SHORT loc_1_1696
        jmp    SHORT loc_1_16C8
loc_1_1696:
        cmp    dword ptr [ebp - 0ch], 1
        jbe    SHORT loc_1_16B2
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_16B2:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_16C8
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        movzx  di, byte ptr [eax + ecx - 1]
        mov    byte ptr [ebp - 1], 1
loc_1_16C8:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], ebx
        jne    NEAR PTR loc_1_164C
loc_1_16DE:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_16F7
        or     di, 2000h
        mov    edx, dword ptr [esi + 30h]
        inc    dword ptr [ebp - 14h]
        add    dx, 8
        mov    ax, di
        out    dx, ax
loc_1_16F7:
        cmp    dword ptr [ebp - 14h], 3ch
        jae    SHORT loc_1_1719
        mov    eax, 3ch
        sub    eax, dword ptr [ebp - 14h]
        shr    eax, 1
        push   eax
        db  068h, 0F0h, 032h, 000h, 000h
        mov    eax, dword ptr [esi + 30h]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_1719:
        test   byte ptr [esi + 7ah], 20h
        je     SHORT loc_1_1725
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 2ch], bl
loc_1_1725:
        cmp    byte ptr [esi + 6a0h], bl
        jne    SHORT loc_1_1749
        mov    edx, dword ptr [esi + 30h]
        add    dx, 0ah
        mov    al, 81h
        out    dx, al
        cmp    dword ptr [esi + 74h], ebx
        je     NEAR PTR loc_1_15FC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1749:
        mov    eax, dword ptr [ebp - 18h]
        push   0
        mov    edi, dword ptr [eax + 28h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 74h], 0
        push   3
        push   esi
        call   sub_1_2E10
        dec    dword ptr [edi + 18h]
loc_1_176C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1780:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 6eah], 0
        je     SHORT loc_1_17B8
        cmp    dword ptr [esi + 74h], 0
        je     SHORT loc_1_17C5
        mov    byte ptr [esi + 78h], 1
        mov    byte ptr [esi + 6eah], 0
        push   7
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 10h
        jmp    SHORT loc_1_17C5
loc_1_17B8:
        cmp    dword ptr [esi + 74h], 0
        je     SHORT loc_1_17C5
        mov    byte ptr [esi + 6eah], 1
loc_1_17C5:
        push   7d0h
        add    esi, 6d2h
        push   esi
        call   sub_1_04E2
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    word ptr [ebp - 14h], 5
        mov    word ptr [ebp - 12h], 5
        db  0C7h, 045h, 0F0h, 010h, 034h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        db  0A1h, 02Ch, 033h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_1831
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1831:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 4ch], 3
        mov    byte ptr [ebp - 4bh], 0
        mov    dword ptr [ebp - 4ah], 0
        db  0C7h, 045h, 0BAh, 0A0h, 01Dh, 000h, 000h
        db  0C7h, 045h, 0BEh, 0A0h, 01Fh, 000h, 000h
        db  0C7h, 045h, 0C2h, 070h, 014h, 000h, 000h
        db  0C7h, 045h, 0C6h, 0C0h, 012h, 000h, 000h
        db  0C7h, 045h, 0CAh, 020h, 02Bh, 000h, 000h
        db  0C7h, 045h, 0CEh, 030h, 021h, 000h, 000h
        db  0C7h, 045h, 0D2h, 0C0h, 029h, 000h, 000h
        db  0C7h, 045h, 0D6h, 090h, 02Ah, 000h, 000h
        db  0C7h, 045h, 0DAh, 0E0h, 018h, 000h, 000h
        db  0C7h, 045h, 0DEh, 0C0h, 02Ah, 000h, 000h
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
        jne    SHORT loc_1_18C6
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18C6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_18E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   esi
        push   edi
        mov    word ptr [ebp - 40h], 0ch
        mov    word ptr [ebp - 3eh], 0ch
        db  0C7h, 045h, 0C4h, 024h, 034h, 000h, 000h
        mov    word ptr [ebp - 38h], 0eh
        mov    word ptr [ebp - 36h], 0eh
        db  0C7h, 045h, 0CCh, 040h, 034h, 000h, 000h
        mov    word ptr [ebp - 30h], 12h
        mov    word ptr [ebp - 2eh], 12h
        db  0C7h, 045h, 0D4h, 064h, 034h, 000h, 000h
        mov    word ptr [ebp - 28h], 10h
        mov    word ptr [ebp - 26h], 10h
        db  0C7h, 045h, 0DCh, 088h, 034h, 000h, 000h
        mov    word ptr [ebp - 20h], 15h
        mov    word ptr [ebp - 1eh], 15h
        db  0C7h, 045h, 0E4h, 0B0h, 034h, 000h, 000h
        db  0A1h, 02Ch, 033h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   6ebh
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_1970
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_1970:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 1bah
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
        jne    NEAR PTR loc_1_1A64
        push   0
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
        jne    NEAR PTR loc_1_1A5B
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 28h]
        cmp    eax, 1
        je     SHORT loc_1_19E3
        cmp    eax, 2
        jne    SHORT loc_1_1A5B
loc_1_19E3:
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
        jne    SHORT loc_1_1A5B
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 28h], 2
        jne    SHORT loc_1_1A3E
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
        jne    SHORT loc_1_1A5B
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 34h], eax
loc_1_1A3E:
        push   1
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1A81
loc_1_1A5B:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0590
loc_1_1A64:
        push   0
        push   6ebh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1A81:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 3ch], eax
        lea    eax, [ebp - 20h]
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
        jne    SHORT loc_1_1AB8
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 68h], eax
        jmp    SHORT loc_1_1AC2
loc_1_1AB8:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 68h], 10h
loc_1_1AC2:
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
        jne    SHORT loc_1_1AFE
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_1_1AFE
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, ecx
        add    edx, 52h
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_1_1AFE:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0590
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1B30
        or     eax, eax
        jne    NEAR PTR loc_1_1A64
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 02Ch, 033h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   26h
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    NEAR PTR loc_1_1D3B
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
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [ecx + 1ah], eax
        cmp    dword ptr [esi + 28h], 2
        mov    eax, dword ptr [ebp - 8]
        jne    SHORT loc_1_1B97
        mov    dword ptr [eax + 1eh], 10h
        jmp    SHORT loc_1_1B9E
loc_1_1B97:
        mov    dword ptr [eax + 1eh], 20h
loc_1_1B9E:
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [esi + 30h]
        mov    dword ptr [eax + 22h], ecx
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
        jne    NEAR PTR loc_1_1D3B
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_1_1C10
        push   1000h
        mov    eax, dword ptr [esi + 34h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 38h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1C10
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_1D2B
        db  08Dh, 064h, 024h, 000h
loc_1_1C10:
        mov    dword ptr [esi + 6c6h], 0
        db  0C7h, 086h, 0CAh, 006h, 000h, 000h, 000h, 030h, 000h, 000h
        mov    dword ptr [esi + 6ceh], esi
        push   esi
        call   sub_1_06A0
        or     al, al
        jne    SHORT loc_1_1C40
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_1D2B
loc_1_1C40:
        push   esi
        call   sub_1_0720
        lea    ecx, [esi + 52h]
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_1C80
        cmp    byte ptr [esi + 53h], 0
        jne    SHORT loc_1_1C80
        cmp    byte ptr [esi + 54h], 0
        jne    SHORT loc_1_1C80
        cmp    byte ptr [esi + 55h], 0
        jne    SHORT loc_1_1C80
        cmp    byte ptr [esi + 56h], 0
        jne    SHORT loc_1_1C80
        cmp    byte ptr [esi + 57h], 0
        jne    SHORT loc_1_1C80
        mov    eax, dword ptr [esi + 4ch]
        lea    edx, [esi + 4ch]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_1C80:
        lea    ebx, [esi + 24h]
        push   ebx
        push   esi
        push   ecx
        db  068h, 0F0h, 02Fh, 000h, 000h
        db  068h, 070h, 02Fh, 000h, 000h
        db  068h, 0F0h, 02Eh, 000h, 000h
        mov    eax, dword ptr [esi + 68h]
        push   eax
        db  0E8h, 062h, 0E3h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_1CC1
        push   3
        push   2
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 14h
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_1_1D2B
loc_1_1CC1:
        push   esi
        call   sub_1_0770
        or     al, al
        jne    SHORT loc_1_1CD4
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_1D23
loc_1_1CD4:
        push   esi
        call   sub_1_0940
        or     al, al
        jne    SHORT loc_1_1CE7
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_1D23
loc_1_1CE7:
        push   1
        push   0
        mov    eax, dword ptr [esi + 3ch]
        push   eax
        push   eax
        db  068h, 060h, 00Ch, 000h, 000h
        push   esi
        db  068h, 040h, 00Ch, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1D50
        push   0
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 0ch
loc_1_1D23:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 0F5h, 0E3h, 0FEh, 0FFh
loc_1_1D2B:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0438
loc_1_1D3B:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D50:
        mov    dword ptr [esi + 6deh], 0
        db  0C7h, 086h, 0E2h, 006h, 000h, 000h, 060h, 00Ch, 000h, 000h
        lea    eax, [esi + 6d2h]
        mov    dword ptr [esi + 6e6h], esi
        mov    dword ptr [eax], 0
        db  0C7h, 086h, 0D6h, 006h, 000h, 000h, 080h, 017h, 000h, 000h
        mov    dword ptr [esi + 6dah], esi
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
        db  08Dh, 064h, 024h, 000h
loc_1_1DA0:
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
loc_1_1DC3:
        or     edx, edx
        je     SHORT loc_1_1DD2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_1DC3
        mov    dword ptr [ecx], edx
loc_1_1DD2:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_1DE4
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_1DE4:
        db  0A1h, 02Ch, 033h, 000h, 000h
        push   eax
        push   0
        push   71h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     SHORT loc_1_1E22
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
loc_1_1E22:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 60h]
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_1E3A
        cmp    dword ptr [esi + 5ch], 0
        jne    SHORT loc_1_1E3A
        push   esi
        call   sub_1_08E0
loc_1_1E3A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 58h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 58h], eax
        cmp    byte ptr [esi + 6a0h], 0
        jne    NEAR PTR loc_1_1F20
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 002h, 0E3h, 0FEh, 0FFh
        or     al, al
        je     NEAR PTR loc_1_1F20
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
        mov    dword ptr [esi + 6a9h], 0eeh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    dword ptr [esi + 60h], eax
        jne    SHORT loc_1_1EFA
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_1EE2
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_1EE2
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_1EFA
loc_1_1EE2:
        push   1
        add    esi, 6c6h
        push   esi
        call   sub_1_04E2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_1EFA:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_1F15
        cmp    dword ptr [esi + 74h], 0
        jne    SHORT loc_1_1F15
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_1F15
        push   esi
        call   sub_1_15E0
loc_1_1F15:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
loc_1_1F20:
        dec    dword ptr [esi + 60h]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 58h], eax
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
        db  08Dh, 049h, 000h
sub_1_1F60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edx, dword ptr [eax + 58h]
        cmp    edx, ecx
        je     SHORT loc_1_1F80
loc_1_1F70:
        mov    esi, dword ptr [edx + 14h]
        cmp    ecx, esi
        jae    SHORT loc_1_1F79
        mov    ecx, esi
loc_1_1F79:
        mov    edx, dword ptr [edx + 0ch]
        or     edx, edx
        jne    SHORT loc_1_1F70
loc_1_1F80:
        or     ecx, ecx
        jne    SHORT loc_1_1F89
        mov    ecx, 0eeh
loc_1_1F89:
        mov    dword ptr [eax + 6a9h], ecx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_1FC3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1FC3:
        inc    dword ptr [esi + 60h]
        inc    dword ptr [ebx + 18h]
        mov    ecx, dword ptr [esi + 58h]
        cmp    ecx, ebx
        jne    SHORT loc_1_1FD8
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 58h], eax
        jmp    SHORT loc_1_1FF1
loc_1_1FD8:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_1_1FE8
loc_1_1FE0:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_1FE0
loc_1_1FE8:
        lea    eax, [ebx + 0ch]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_1_1FF1:
        push   esi
        call   sub_1_0880
        or     al, al
        jne    SHORT loc_1_2027
        mov    eax, dword ptr [esi + 24h]
        push   0
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [esi + 24h]
        push   71h
        mov    dword ptr [eax + 10h], 0
        push   ebx
        call   sub_1_050C
        dec    dword ptr [esi + 60h]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2027:
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 00Ah, 0E2h, 0FEh, 0FFh
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_2060
        mov    eax, dword ptr [ebx + 18h]
        cmp    eax, 2
        je     SHORT loc_1_2053
        mov    byte ptr [ebx + 1ch], 1
        sub    dword ptr [ebx + 18h], 2
        mov    edi, 103h
        jmp    SHORT loc_1_2086
loc_1_2053:
        sub    eax, 2
        mov    dword ptr [ebx + 18h], eax
        jmp    SHORT loc_1_2086
        db  005h, 000h, 000h, 000h, 000h
loc_1_2060:
        cmp    edi, 103h
        jne    SHORT loc_1_2072
        mov    byte ptr [ebx + 1ch], 1
        sub    dword ptr [ebx + 18h], 2
        jmp    SHORT loc_1_2086
loc_1_2072:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_2083
        mov    byte ptr [ebx + 1ch], 1
        mov    edi, 103h
loc_1_2083:
        dec    dword ptr [ebx + 18h]
loc_1_2086:
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_1_20C0
        mov    eax, dword ptr [esi + 6a9h]
        cmp    dword ptr [ebx + 14h], eax
        jne    SHORT loc_1_209D
        push   esi
        call   sub_1_1F60
loc_1_209D:
        push   0
        push   71h
        push   ebx
        call   sub_1_050C
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_20CE
        cmp    dword ptr [esi + 5ch], 0
        jne    SHORT loc_1_20CE
        push   esi
        call   sub_1_0920
        jmp    SHORT loc_1_20CE
        db  005h, 000h, 000h, 000h, 000h
loc_1_20C0:
        mov    eax, dword ptr [esi + 5ch]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 5ch], ebx
        mov    edi, 103h
loc_1_20CE:
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    dword ptr [esi + 60h], eax
        jne    SHORT loc_1_2108
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_20EF
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_20EF
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_2108
loc_1_20EF:
        push   1
        add    esi, 6c6h
        push   esi
        call   sub_1_04E2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2108:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_2123
        cmp    dword ptr [esi + 74h], 0
        jne    SHORT loc_1_2123
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_2123
        push   esi
        call   sub_1_15E0
loc_1_2123:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2130:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 18h]
        inc    dword ptr [edi + 60h]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_2150
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_218C
        db  090h
loc_1_2150:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        or     eax, eax
        jne    SHORT loc_1_2166
        push   ecx
        push   ebx
        push   edi
        call   sub_1_25B0
        mov    esi, eax
        jmp    SHORT loc_1_218C
loc_1_2166:
        cmp    eax, 1
        jne    SHORT loc_1_2187
        cmp    byte ptr [edi + 6a0h], 0
        je     SHORT loc_1_217B
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_218C
loc_1_217B:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2600
        mov    esi, eax
        jmp    SHORT loc_1_218C
loc_1_2187:
        mov    esi, 10001h
loc_1_218C:
        dec    dword ptr [ebx + 18h]
        mov    eax, dword ptr [edi + 60h]
        dec    eax
        mov    dword ptr [edi + 60h], eax
        jne    SHORT loc_1_21C7
        cmp    byte ptr [edi + 6a0h], 0
        jne    SHORT loc_1_21B0
        cmp    dword ptr [edi + 6b9h], 0
        jne    SHORT loc_1_21B0
        cmp    dword ptr [edi + 5ch], 0
        je     SHORT loc_1_21C7
loc_1_21B0:
        push   1
        add    edi, 6c6h
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_21C7:
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_1_21E2
        cmp    dword ptr [edi + 74h], 0
        jne    SHORT loc_1_21E2
        cmp    byte ptr [edi + 6c5h], 0
        jne    SHORT loc_1_21E2
        push   edi
        call   sub_1_15E0
loc_1_21E2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_21F0:
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
        ja     SHORT loc_1_2240
        je     NEAR PTR loc_1_2410
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_2261
        shl    eax, 2
        db  0FFh, 0A0h, 05Ch, 025h, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2240:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2430
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2450
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2509
loc_1_2261:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 049h, 000h
loc_1_2270:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2285
        db  0BEh, 0ACh, 033h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_2512
loc_1_2285:
        db  0BEh, 030h, 033h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_2512
loc_1_2294:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 6a0h], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_1_22AE
        mov    dword ptr [ebp - 14h], 0
loc_1_22AE:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2512
        db  005h, 000h, 000h, 000h, 000h
loc_1_22C0:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 049h, 000h
loc_1_22D0:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_22E0:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_22F0:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2300:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 74h], 1
        sbb    eax, eax
        and    eax, 5eeh
        jmp    NEAR PTR loc_1_250F
loc_1_2313:
        mov    dword ptr [ebp - 8], 2000h
        jmp    NEAR PTR loc_1_2512
        db  090h
loc_1_2320:
        mov    dword ptr [ebp - 8], 5eeh
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2330:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2340:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4ch
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2512
loc_1_2366:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 28h]
        cmp    eax, 2
        jne    SHORT loc_1_2380
        db  0BEh, 0C8h, 034h, 000h, 000h
        mov    edi, 23h
        jmp    NEAR PTR loc_1_2512
loc_1_2380:
        cmp    eax, 1
        jne    NEAR PTR loc_1_2512
        db  0BEh, 0ECh, 034h, 000h, 000h
        mov    edi, 1fh
        jmp    NEAR PTR loc_1_2512
loc_1_2398:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_23B0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 24h]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_250F
        db  08Dh, 064h, 024h, 000h
loc_1_23B0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_250F
        db  08Bh, 0FFh
loc_1_23C0:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_23D4
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 6a9h]
        jmp    NEAR PTR loc_1_250F
loc_1_23D4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_250F
        db  090h
loc_1_23E0:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_1_2512
loc_1_23F3:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2512
        db  090h
loc_1_2400:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2410:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4ch
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2512
        db  090h
loc_1_2430:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 52h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2512
        db  090h
loc_1_2450:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_24AA
        mov    eax, dword ptr [ebx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_2482
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_1_2512
loc_1_2482:
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
        db  0E8h, 0C5h, 0E2h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2512
loc_1_24AA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        db  0E8h, 0A6h, 0E1h, 0FEh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_24DA
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_1_2512
loc_1_24DA:
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
        db  0E8h, 0A6h, 0E1h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2512
loc_1_2509:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 68h]
loc_1_250F:
        mov    dword ptr [ebp - 8], eax
loc_1_2512:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_254E
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_254E
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_253E
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
loc_1_253E:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_254E:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh
loc_1_255C:
        db  070h, 022h
        add    byte ptr [eax], al
        xchg   esp, eax
        and    al, byte ptr [eax]
        db  000h, 0C0h
        and    al, byte ptr [eax]
        db  000h, 0C0h
        and    al, byte ptr [eax]
        db  000h, 0D0h
        and    al, byte ptr [eax]
        db  000h, 0E0h
        and    al, byte ptr [eax]
        db  000h, 0F0h
        and    al, byte ptr [eax]
        db  000h, 000h
        and    eax, dword ptr [eax]
        db  000h, 013h
        and    eax, dword ptr [eax]
        db  000h, 020h
        and    eax, dword ptr [eax]
        db  000h, 030h
        and    eax, dword ptr [eax]
        db  000h, 040h, 023h
        add    byte ptr [eax], al
        and    ax, word ptr [eax]
        db  000h, 098h, 023h, 000h, 000h, 0C0h
        and    eax, dword ptr [eax]
        db  000h, 0E0h
        and    eax, dword ptr [eax]
        db  000h, 0F3h
        and    eax, dword ptr [eax]
        db  000h, 061h, 022h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [ebp - 72ffdb9ch], cl
        and    al, 0
sub_1_25B0:
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
        call   sub_1_21F0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_2600:
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
        je     SHORT loc_1_2657
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_26B8
        cmp    eax, 10112h
        je     NEAR PTR loc_1_2747
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2774
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_27B5
loc_1_2657:
        cmp    edi, 4
        je     SHORT loc_1_2674
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 4
        jmp    NEAR PTR loc_1_27B5
loc_1_2674:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_26A1
        mov    ebx, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_27B5
loc_1_26A1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_27D0
        jmp    NEAR PTR loc_1_27B3
loc_1_26B8:
        cmp    edi, 4
        je     SHORT loc_1_26D5
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_27B5
loc_1_26D5:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0fffffffeh
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_2740
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 6a9h]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_2716
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 6a9h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_27B5
loc_1_2716:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 14h], eax
        jne    SHORT loc_1_2737
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_1_2737
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        push   edx
        call   sub_1_1F60
        jmp    NEAR PTR loc_1_27B5
loc_1_2737:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    SHORT loc_1_27B5
        db  090h
loc_1_2740:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_27B5
loc_1_2747:
        cmp    edi, 4
        je     SHORT loc_1_2761
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_27B5
loc_1_2761:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 1dh
        push   ecx
        call   sub_1_05EC
        sub    ebx, ebx
        jmp    SHORT loc_1_27B5
loc_1_2774:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_1_2798
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_27B5
loc_1_2798:
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
        call   sub_1_2830
loc_1_27B3:
        mov    ebx, eax
loc_1_27B5:
        or     ebx, ebx
        jne    SHORT loc_1_27C3
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_1_27C3:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_27D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6a0h], 0
        jne    SHORT loc_1_281A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_2810
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
        db  0E8h, 0DDh, 0DDh, 0FEh, 0FFh
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2810:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_281A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6a0h], 0
        jne    SHORT loc_1_287A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_2870
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
        db  0E8h, 099h, 0DAh, 0FEh, 0FFh
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    14h
        db  090h
loc_1_2870:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_1_287A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2890:
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
        call   sub_1_21F0
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_1_29A6
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2916
        je     SHORT loc_1_2960
        cmp    eax, 20101h
        je     SHORT loc_1_2932
        cmp    eax, 20102h
        je     SHORT loc_1_2940
        cmp    eax, 20103h
        je     SHORT loc_1_2948
        cmp    eax, 20104h
        je     SHORT loc_1_2950
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2989
loc_1_2916:
        cmp    eax, 1020101h
        je     SHORT loc_1_2968
        cmp    eax, 1020102h
        je     SHORT loc_1_2971
        cmp    eax, 1020103h
        je     SHORT loc_1_2980
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2989
loc_1_2932:
        mov    eax, dword ptr [ebx + 688h]
        jmp    SHORT loc_1_2986
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2940:
        mov    eax, dword ptr [ebx + 68ch]
        jmp    SHORT loc_1_2986
loc_1_2948:
        mov    eax, dword ptr [ebx + 690h]
        jmp    SHORT loc_1_2986
loc_1_2950:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2989
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2960:
        mov    eax, dword ptr [ebx + 69ch]
        jmp    SHORT loc_1_2986
loc_1_2968:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2989
loc_1_2971:
        mov    eax, dword ptr [ebx + 694h]
        jmp    SHORT loc_1_2986
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2980:
        mov    eax, dword ptr [ebx + 698h]
loc_1_2986:
        mov    dword ptr [ebp - 4], eax
loc_1_2989:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_29A6
        or     edi, edi
        jne    SHORT loc_1_29A6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 8], 4
loc_1_29A6:
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
loc_1_29C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_29F2
        cmp    eax, 20101h
        jae    SHORT loc_1_2A13
        cmp    eax, 10101h
        jb     SHORT loc_1_2A0E
        cmp    eax, 10111h
        jbe    SHORT loc_1_2A13
        cmp    eax, 10113h
        je     SHORT loc_1_2A13
        jmp    SHORT loc_1_2A0E
loc_1_29F2:
        cmp    eax, 1010101h
        jb     SHORT loc_1_2A0E
        cmp    eax, 1010104h
        jbe    SHORT loc_1_2A13
        cmp    eax, 1020101h
        jb     SHORT loc_1_2A0E
        cmp    eax, 1020103h
        jbe    SHORT loc_1_2A13
loc_1_2A0E:
        mov    esi, 0c0010017h
loc_1_2A13:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 60h]
        or     esi, esi
        jne    SHORT loc_1_2A26
        push   ecx
        push   edi
        call   sub_1_2890
        mov    esi, eax
loc_1_2A26:
        mov    eax, dword ptr [edi + 60h]
        dec    eax
        mov    dword ptr [edi + 60h], eax
        jne    SHORT loc_1_2A60
        cmp    byte ptr [edi + 6a0h], 0
        jne    SHORT loc_1_2A47
        cmp    dword ptr [edi + 6b9h], 0
        jne    SHORT loc_1_2A47
        cmp    dword ptr [edi + 5ch], 0
        je     SHORT loc_1_2A60
loc_1_2A47:
        push   1
        add    edi, 6c6h
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_2A60:
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_1_2A7B
        cmp    dword ptr [edi + 74h], 0
        jne    SHORT loc_1_2A7B
        cmp    byte ptr [edi + 6c5h], 0
        jne    SHORT loc_1_2A7B
        push   edi
        call   sub_1_15E0
loc_1_2A7B:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2A90:
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
loc_1_2AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 6d2h]
        push   eax
        call   sub_1_04E8
        push   2625a0h
        call   sub_1_0500
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 033h, 0D6h, 0FEh, 0FFh
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
        push   6ebh
        push   esi
        call   sub_1_050C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_2B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 1ch], 0
        je     SHORT loc_1_2B43
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B43:
        inc    dword ptr [esi + 18h]
        inc    dword ptr [edi + 60h]
        cmp    byte ptr [edi + 6a0h], 0
        je     SHORT loc_1_2B66
        dec    dword ptr [esi + 18h]
        dec    dword ptr [edi + 60h]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B66:
        mov    ebx, dword ptr [edi + 58h]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     SHORT loc_1_2B96
loc_1_2B74:
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
        jne    SHORT loc_1_2B74
loc_1_2B96:
        mov    byte ptr [edi + 6a0h], 1
        mov    dword ptr [edi + 6a1h], 0
        mov    dword ptr [edi + 6a5h], esi
        push   edi
        call   sub_1_2C20
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 18h]
        mov    eax, dword ptr [edi + 60h]
        dec    eax
        mov    dword ptr [edi + 60h], eax
        jne    SHORT loc_1_2BF4
        cmp    byte ptr [edi + 6a0h], 0
        jne    SHORT loc_1_2BDA
        cmp    dword ptr [edi + 6b9h], 0
        jne    SHORT loc_1_2BDA
        cmp    dword ptr [edi + 5ch], 0
        je     SHORT loc_1_2BF4
loc_1_2BDA:
        push   1
        add    edi, 6c6h
        push   edi
        call   sub_1_04E2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2BF4:
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_1_2C0F
        cmp    dword ptr [edi + 74h], 0
        jne    SHORT loc_1_2C0F
        cmp    byte ptr [edi + 6c5h], 0
        jne    SHORT loc_1_2C0F
        push   edi
        call   sub_1_15E0
loc_1_2C0F:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2C20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 60h]
        cmp    dword ptr [esi + 6b9h], 0
        je     SHORT loc_1_2C6C
loc_1_2C34:
        mov    ecx, dword ptr [esi + 6b9h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 6b9h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_1_2C60
        cmp    eax, 8
        je     SHORT loc_1_2C60
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_2C60:
        dec    dword ptr [edi + 18h]
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_2C34
loc_1_2C6C:
        cmp    dword ptr [esi + 74h], 0
        je     SHORT loc_1_2C8A
        mov    dword ptr [esi + 6a1h], 2
        dec    dword ptr [esi + 60h]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_2C8A:
        dec    dword ptr [esi + 60h]
        push   esi
        call   sub_1_2CA0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CA0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 60h]
        inc    eax
        mov    dword ptr [ecx + 60h], eax
        cmp    dword ptr [ecx + 74h], 0
        je     SHORT loc_1_2CC9
        mov    dword ptr [ecx + 6a1h], 3
        dec    dword ptr [ecx + 60h]
        mov    eax, 103h
        pop    ebp
        ret    4
loc_1_2CC9:
        dec    eax
        mov    dword ptr [ecx + 60h], eax
        push   ecx
        call   sub_1_2CE0
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 60h]
        mov    ecx, dword ptr [esi + 74h]
        or     ecx, ecx
        je     SHORT loc_1_2D11
        mov    edi, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    dword ptr [esi + 74h], 0
loc_1_2D11:
        mov    edx, dword ptr [esi + 6ch]
        or     edx, edx
        je     SHORT loc_1_2D3C
loc_1_2D18:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 6ch], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    edx, dword ptr [esi + 6ch]
        or     edx, edx
        jne    SHORT loc_1_2D18
loc_1_2D3C:
        mov    edx, dword ptr [esi + 6adh]
        or     edx, edx
        je     SHORT loc_1_2D70
loc_1_2D46:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 6adh], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    edx, dword ptr [esi + 6adh]
        or     edx, edx
        jne    SHORT loc_1_2D46
loc_1_2D70:
        cmp    byte ptr [esi + 6c5h], 0
        je     SHORT loc_1_2D93
        lea    esp, [esp]
loc_1_2D80:
        push   2710h
        call   sub_1_0500
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_2D80
loc_1_2D93:
        push   esi
        call   sub_1_0A60
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 6a0h], 0
        mov    edi, dword ptr [esi + 58h]
        or     edi, edi
        je     SHORT loc_1_2DF3
        sub    ebx, ebx
loc_1_2DB5:
        inc    dword ptr [edi + 18h]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_2DCC
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_2DCC:
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
        jne    SHORT loc_1_2DB5
loc_1_2DF3:
        dec    dword ptr [esi + 60h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 6a1h], eax
        jne    NEAR PTR loc_1_2EE2
        cmp    eax, 1
        je     SHORT loc_1_2E90
        cmp    eax, 2
        je     SHORT loc_1_2E98
        cmp    eax, 3
        je     SHORT loc_1_2EA0
        mov    eax, dword ptr [ebp - 4]
loc_1_2E3B:
        cmp    eax, 103h
        je     SHORT loc_1_2E5A
        push   eax
        mov    eax, dword ptr [esi + 6a5h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 6a5h]
        dec    dword ptr [eax + 18h]
loc_1_2E5A:
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    dword ptr [esi + 60h], eax
        jne    SHORT loc_1_2EC7
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_2E7B
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_2E7B
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_2EC7
loc_1_2E7B:
        push   1
        add    esi, 6c6h
        push   esi
        call   sub_1_04E2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2E90:
        push   esi
        call   sub_1_2C20
        jmp    SHORT loc_1_2E3B
loc_1_2E98:
        push   esi
        call   sub_1_2CA0
        jmp    SHORT loc_1_2E3B
loc_1_2EA0:
        mov    eax, dword ptr [esi + 60h]
        inc    eax
        mov    dword ptr [esi + 60h], eax
        cmp    dword ptr [esi + 74h], 0
        je     SHORT loc_1_2EB8
        dec    eax
        mov    dword ptr [esi + 60h], eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2EB8:
        dec    eax
        mov    dword ptr [esi + 60h], eax
        push   esi
        call   sub_1_2CE0
        jmp    NEAR PTR loc_1_2E3B
loc_1_2EC7:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_2EE2
        cmp    dword ptr [esi + 74h], 0
        jne    SHORT loc_1_2EE2
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_2EE2
        push   esi
        call   sub_1_15E0
loc_1_2EE2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_2F03
        lea    edx, [eax + 49h]
loc_1_2F03:
        cmp    byte ptr [ecx + 6a0h], 0
        je     SHORT loc_1_2F15
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_2F15:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 18h]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 6b9h], 0
        jne    SHORT loc_1_2F50
        mov    dword ptr [ecx + 6bdh], edx
        mov    dword ptr [ecx + 6b9h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_2F50:
        mov    eax, dword ptr [ecx + 6bdh]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 6bdh], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2F70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_2F83
        lea    edx, [eax + 21h]
loc_1_2F83:
        cmp    byte ptr [ecx + 6a0h], 0
        je     SHORT loc_1_2F95
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_2F95:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 18h]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 6b9h], 0
        jne    SHORT loc_1_2FD0
        mov    dword ptr [ecx + 6bdh], edx
        mov    dword ptr [ecx + 6b9h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2FD0:
        mov    eax, dword ptr [ecx + 6bdh]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 6bdh], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2FF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 18h]
        ret    4
        db  08Dh, 049h, 000h
loc_1_3000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 60h]
        cmp    byte ptr [esi + 6a0h], 0
        jne    NEAR PTR loc_1_30B6
        cmp    dword ptr [esi + 6c1h], 0
        jne    NEAR PTR loc_1_30B6
loc_1_3026:
        mov    edi, dword ptr [esi + 6b9h]
        or     edi, edi
        je     SHORT loc_1_3091
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 6b9h], eax
        cmp    dword ptr [esi + 6bdh], edi
        jne    SHORT loc_1_304A
        mov    dword ptr [esi + 6bdh], 0
loc_1_304A:
        mov    dword ptr [esi + 6c1h], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_3065
        cmp    eax, 0ah
        je     SHORT loc_1_3065
        push   esi
        call   sub_1_3180
        jmp    SHORT loc_1_306B
loc_1_3065:
        push   esi
        call   sub_1_31E0
loc_1_306B:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_3085
        cmp    ecx, 4
        je     SHORT loc_1_3085
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_3085:
        dec    dword ptr [ebx + 18h]
        cmp    byte ptr [esi + 6a0h], 0
        je     SHORT loc_1_3026
loc_1_3091:
        mov    dword ptr [esi + 6c1h], 0
        cmp    byte ptr [esi + 6a0h], 0
        je     SHORT loc_1_30B6
        dec    dword ptr [esi + 60h]
        push   1
        push   esi
        call   sub_1_2E10
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_30B6:
        mov    edi, dword ptr [esi + 5ch]
        or     edi, edi
        je     SHORT loc_1_3121
        sub    ebx, ebx
        nop
loc_1_30C0:
        cmp    dword ptr [edi + 18h], 0
        jbe    SHORT loc_1_30D0
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_310B
        db  08Dh, 049h, 000h
loc_1_30D0:
        mov    eax, dword ptr [esi + 6a9h]
        cmp    dword ptr [edi + 14h], eax
        jne    SHORT loc_1_30E1
        push   esi
        call   sub_1_1F60
loc_1_30E1:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [edi + 0ch]
        je     SHORT loc_1_30FA
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_30FD
loc_1_30FA:
        mov    dword ptr [esi + 5ch], eax
loc_1_30FD:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   71h
        push   eax
        call   sub_1_050C
loc_1_310B:
        or     edi, edi
        jne    SHORT loc_1_30C0
        cmp    dword ptr [esi + 5ch], 0
        jne    SHORT loc_1_3121
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_3121
        push   esi
        call   sub_1_0920
loc_1_3121:
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    dword ptr [esi + 60h], eax
        jne    SHORT loc_1_3157
        cmp    byte ptr [esi + 6a0h], 0
        jne    SHORT loc_1_3142
        cmp    dword ptr [esi + 6b9h], 0
        jne    SHORT loc_1_3142
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_3157
loc_1_3142:
        push   1
        add    esi, 6c6h
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3157:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_3172
        cmp    dword ptr [esi + 74h], 0
        jne    SHORT loc_1_3172
        cmp    byte ptr [esi + 6c5h], 0
        jne    SHORT loc_1_3172
        push   esi
        call   sub_1_15E0
loc_1_3172:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 24h]
        mov    eax, dword ptr [eax + 0ch]
        test   al, 20h
        mov    dword ptr [esi + 7ah], eax
        je     SHORT loc_1_319C
        lea    eax, [esi + 43h]
        mov    byte ptr [eax], 7
        jmp    SHORT loc_1_31C3
loc_1_319C:
        test   al, 4
        push   esi
        je     SHORT loc_1_31A8
        call   sub_1_0BA0
        jmp    SHORT loc_1_31AD
loc_1_31A8:
        call   sub_1_31E0
loc_1_31AD:
        cmp    dword ptr [esi + 7ah], 0
        lea    eax, [esi + 43h]
        je     SHORT loc_1_31C0
        mov    byte ptr [eax], 6
        jmp    SHORT loc_1_31C3
        db  005h, 000h, 000h, 000h, 000h
loc_1_31C0:
        mov    byte ptr [eax], 4
loc_1_31C3:
        mov    edx, dword ptr [esi + 30h]
        add    dx, 5
        mov    al, byte ptr [eax]
        out    dx, al
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0B20
        push   esi
        call   sub_1_0BF0
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
loc_1_32E0:
        pushad
        xor    al, byte ptr [eax]
        db  000h, 0A0h, 032h, 000h, 000h
loc_1_32E8:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 6d6fh
loc_1_32F0:
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
loc_1_3330:
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
loc_1_33AC:
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
        dec    esi
        dec    ecx
        inc    ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3410:
        dec    esi
        dec    ecx
        inc    ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 054h
        jns    SHORT loc_1_3492
        add    byte ptr gs:[ecx + 64h], al
        popad
        jo     SHORT loc_1_349D
        jb     NEAR PTR loc_1_3480
        db  079h, 070h
        add    byte ptr gs:[ecx + 6fh], cl
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_34A0
        jae    SHORT loc_1_34B0
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], cl
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_34B0
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        db  072h, 079h
        inc    edx
        popad
        jae    SHORT loc_1_34BF
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_34D4
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
        jae    SHORT loc_1_34E8
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_34E1
        jb     SHORT loc_1_34F0
        jne    SHORT loc_1_34F0
loc_1_3480:
        je     SHORT loc_1_34D0
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_34E0
loc_1_3492:
        jne    SHORT loc_1_3501
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ebp + 61h], cl
        js     SHORT loc_1_3505
        insd
loc_1_349D:
        db  075h, 06Dh
        dec    ebp
loc_1_34A0:
        db  075h, 06Ch
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_34F5
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_34B0:
        dec    ebp
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
        dec    ebp
        db  075h, 06Ch
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
loc_1_34BF:
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_34C8:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 50206d6fh
loc_1_34D0:
        outsd
        arpl   word ptr [ebx + 65h], bp
loc_1_34D4:
        je     SHORT loc_1_34F6
        inc    ebp
        db  074h, 068h
        db  065h, 072h, 06Eh
        je     NEAR PTR loc_1_34FF
        dec    ecx
loc_1_34E0:
        dec    ecx
loc_1_34E1:
        dec    ecx
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
loc_1_34E8:
        db  065h, 072h, 02Eh
        add    byte ptr [esi + 75h], al
        push   69h
loc_1_34F0:
        db  073h, 074h
        db  075h, 020h
        dec    ebp
loc_1_34F5:
        inc    edx
loc_1_34F6:
        dec    eax
        xor    dword ptr [eax], esi
        xor    esi, dword ptr [eax]
        xor    ah, byte ptr [eax]
        inc    edx
        popad
loc_1_34FF:
        db  073h, 065h
loc_1_3501:
        and    byte ptr fs:[ecx + 64h], al
loc_1_3505:
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
_LTEXT ends
        end
