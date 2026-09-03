; pe3ndis_lcode_01_01.asm -- PE3NDIS object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 068h, 006h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 068h, 006h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        call   sub_1_1B70
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
PE3NDIS_Control:
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
        call   sub_1_04DC
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
        call   sub_1_04DC
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
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_04B2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_04B8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_04BE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_04C4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_04CA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_04D0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_04D6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_04DC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0500:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0524:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0548:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0554:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_055A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0560:
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
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_05A8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_05BC:
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
sub_1_05DC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_05FC
        or     ecx, ecx
        je     SHORT loc_1_0612
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_0610
loc_1_05FC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_0610
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_0610:
        rep movsb
loc_1_0612:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 050h, 045h, 033h, 04Eh
        db  044h, 049h, 053h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0670:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    edi, [esi + 20h]
        push   edi
        call   dword ptr [esi + 7ch]
        cmp    al, 72h
        je     SHORT loc_1_06E0
        push   72h
        push   6
        push   edi
        call   dword ptr [esi + 70h]
        push   6
        push   edi
        call   dword ptr [esi + 7ch]
        cmp    al, 72h
        je     SHORT loc_1_06E0
        push   esi
        call   sub_1_09F0
        push   72h
        push   6
        push   edi
        call   dword ptr [esi + 70h]
        push   6
        push   edi
        call   dword ptr [esi + 7ch]
        cmp    al, 72h
        je     SHORT loc_1_06E0
        mov    byte ptr [esi + 76bh], 1
        push   edi
        call   dword ptr [esi + 5ch]
        push   6
        push   3
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06E0:
        mov    byte ptr [esi + 76bh], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_06F0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 4eh]
        mov    byte ptr [ecx + 6e1h], al
        mov    al, byte ptr [ecx + 4fh]
        mov    byte ptr [ecx + 6e2h], al
        mov    al, byte ptr [ecx + 50h]
        mov    byte ptr [ecx + 6e3h], al
        mov    al, byte ptr [ecx + 45h]
        mov    byte ptr [ecx + 6e4h], al
        mov    al, byte ptr [ecx + 44h]
        mov    byte ptr [ecx + 6e5h], al
        mov    al, byte ptr [ecx + 43h]
        mov    byte ptr [ecx + 6e6h], al
        cmp    byte ptr [ecx + 6dbh], 0
        jne    SHORT loc_1_07AA
        cmp    byte ptr [ecx + 6dch], 0
        jne    SHORT loc_1_07AA
        cmp    byte ptr [ecx + 6ddh], 0
        jne    SHORT loc_1_07AA
        cmp    byte ptr [ecx + 6deh], 0
        jne    SHORT loc_1_07AA
        cmp    byte ptr [ecx + 6dfh], 0
        jne    SHORT loc_1_07AA
        cmp    byte ptr [ecx + 6e0h], 0
        jne    SHORT loc_1_07AA
        mov    al, byte ptr [ecx + 6e1h]
        mov    byte ptr [ecx + 6dbh], al
        mov    al, byte ptr [ecx + 6e2h]
        mov    byte ptr [ecx + 6dch], al
        mov    al, byte ptr [ecx + 6e3h]
        mov    byte ptr [ecx + 6ddh], al
        mov    al, byte ptr [ecx + 6e4h]
        mov    byte ptr [ecx + 6deh], al
        mov    al, byte ptr [ecx + 6e5h]
        mov    byte ptr [ecx + 6dfh], al
        mov    al, byte ptr [ecx + 6e6h]
        mov    byte ptr [ecx + 6e0h], al
loc_1_07AA:
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_07B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0f2h
        lea    edi, [esi + 20h]
        push   6
        push   edi
        call   dword ptr [esi + 70h]
        push   0b0h
        push   7
        push   edi
        call   dword ptr [esi + 70h]
        push   83h
        push   0
        push   edi
        call   dword ptr [esi + 70h]
        push   0c1h
        push   1
        push   edi
        call   dword ptr [esi + 70h]
        push   83h
        push   2
        push   edi
        call   dword ptr [esi + 70h]
        push   0c1h
        push   3
        push   edi
        call   dword ptr [esi + 70h]
        push   6
        push   4
        push   edi
        call   dword ptr [esi + 70h]
        mov    al, byte ptr [esi + 0d0h]
        push   eax
        push   5
        sub    bl, bl
        push   edi
        call   dword ptr [esi + 70h]
        push   20h
        push   7
        push   edi
        call   dword ptr [esi + 70h]
loc_1_0820:
        xor    eax, eax
        mov    al, bl
        mov    al, byte ptr [esi + eax + 6dbh]
        push   eax
        mov    al, bl
        add    al, 8
        push   eax
        push   edi
        call   dword ptr [esi + 70h]
        inc    bl
        cmp    bl, 6
        jb     SHORT loc_1_0820
        push   0b4h
        push   7
        push   edi
        call   dword ptr [esi + 70h]
        sub    bl, bl
        mov    eax, dword ptr [esi + 708h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 4
        mov    dword ptr [ebp - 4], eax
loc_1_0858:
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 0ffh
        jne    SHORT loc_1_0873
        xor    eax, eax
        mov    al, bl
        mov    al, byte ptr [esi + eax + 70ch]
        and    eax, 0ffh
loc_1_0873:
        push   eax
        mov    al, bl
        add    al, 8
        push   eax
        push   edi
        call   dword ptr [esi + 70h]
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0858
        push   0b8h
        push   7
        push   edi
        call   dword ptr [esi + 70h]
        push   7
        push   0bh
        push   edi
        call   dword ptr [esi + 70h]
        push   0
        push   0dh
        push   edi
        call   dword ptr [esi + 70h]
        push   72h
        push   6
        push   edi
        call   dword ptr [esi + 70h]
        push   edi
        call   dword ptr [esi + 58h]
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 108h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   4
        lea    eax, [ebx + 20h]
        push   4
        push   eax
        call   dword ptr [ebx + 70h]
        push   3
        lea    eax, [ebx + 20h]
        push   5
        push   eax
        call   dword ptr [ebx + 70h]
        push   0
        lea    eax, [ebx + 20h]
        push   8
        push   eax
        call   dword ptr [ebx + 70h]
        push   1
        lea    eax, [ebx + 20h]
        push   8
        push   eax
        call   dword ptr [ebx + 70h]
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_3750
        push   100h
        db  068h, 0C0h, 04Ch, 000h, 000h
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 74h]
        push   81h
        push   0ah
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 70h]
        sub    esi, esi
loc_1_0925:
        push   0
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 7ch]
        test   al, 80h
        jne    SHORT loc_1_093B
        inc    esi
        cmp    esi, 100h
        jb     SHORT loc_1_0925
loc_1_093B:
        mov    byte ptr [ebp - 1], 1
        cmp    esi, 100h
        jae    SHORT loc_1_09BB
        push   0ffh
        push   0
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 70h]
        push   0ffh
        push   1
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 70h]
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_3720
        push   4
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 80h]
        cmp    word ptr [ebp - 6], 100h
        jne    SHORT loc_1_09BB
        push   100h
        lea    eax, [ebp - 108h]
        push   eax
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 80h]
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_3770
        db  0BEh, 0C0h, 04Ch, 000h, 000h
        lea    edi, [ebp - 108h]
        mov    ecx, 40h
        repe cmpsd
        jne    SHORT loc_1_09BB
        mov    byte ptr [ebp - 1], 0
loc_1_09BB:
        mov    al, byte ptr [ebx + 0d0h]
        push   eax
        push   5
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 70h]
        push   6
        push   4
        lea    eax, [ebx + 20h]
        push   eax
        call   dword ptr [ebx + 70h]
        mov    al, byte ptr [ebp - 1]
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
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_07B0
        or     al, al
        jne    SHORT loc_1_0A20
        push   1
        push   4
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_1_0A20:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A30:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 2ah]
        or     bl, bl
        je     SHORT loc_1_0A4A
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 5ch]
loc_1_0A4A:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_1_0AA5
loc_1_0A66:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_0A80
        jmp    SHORT loc_1_0A92
        db  090h
loc_1_0A80:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 74h]
        add    edi, dword ptr [ebp - 8]
loc_1_0A92:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0A66
loc_1_0AA5:
        or     bl, bl
        je     SHORT loc_1_0AB0
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 58h]
loc_1_0AB0:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_0AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_0B0B
loc_1_0AD2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0AE0:
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
        je     SHORT loc_1_0B02
        xor    eax, 4c11db7h
loc_1_0B02:
        dec    esi
        jne    SHORT loc_1_0AE0
        inc    edx
        cmp    edx, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_0AD2
loc_1_0B0B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B20:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0AC0
        sub    ecx, ecx
        mov    edx, 1
loc_1_0B35:
        add    ecx, ecx
        test   eax, edx
        je     SHORT loc_1_0B3E
        or     ecx, 1
loc_1_0B3E:
        add    edx, edx
        cmp    edx, 40h
        jb     SHORT loc_1_0B35
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
sub_1_0B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
        lea    esp, [esp]
loc_1_0B80:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 70ch], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_1_0B80
        db  068h, 060h, 04Ch, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 708h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0B3h, 0FBh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_1_0BE7
        sub    edi, edi
loc_1_0BB5:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 060h, 04Ch, 000h, 000h
        push   eax
        call   sub_1_0B20
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 70ch], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_1_0BB5
loc_1_0BE7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0BF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 20h]
        mov    al, byte ptr [esi + 0d0h]
        push   eax
        push   5
        push   edi
        call   dword ptr [esi + 70h]
        cmp    byte ptr [esi + 2ah], 0
        je     SHORT loc_1_0C12
        push   edi
        call   dword ptr [esi + 60h]
loc_1_0C12:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0C20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0f2h
        lea    edi, [esi + 20h]
        push   6
        push   edi
        call   dword ptr [esi + 70h]
        push   0b4h
        sub    bl, bl
        push   7
        push   edi
        call   dword ptr [esi + 70h]
loc_1_0C44:
        push   0ffh
        mov    al, bl
        add    al, 8
        push   eax
        push   edi
        call   dword ptr [esi + 70h]
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0C44
        push   72h
        push   6
        push   edi
        call   dword ptr [esi + 70h]
        push   0b8h
        push   7
        push   edi
        call   dword ptr [esi + 70h]
        cmp    byte ptr [esi + 2ah], 0
        je     SHORT loc_1_0C76
        push   edi
        call   dword ptr [esi + 60h]
loc_1_0C76:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_0C80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   0f2h
        lea    esi, [edi + 20h]
        push   6
        push   esi
        call   dword ptr [edi + 70h]
        push   0b4h
        sub    ebx, ebx
        push   7
        push   esi
        call   dword ptr [edi + 70h]
loc_1_0CA4:
        mov    al, byte ptr [edi + ebx + 70ch]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        push   esi
        call   dword ptr [edi + 70h]
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_0CA4
        push   72h
        push   6
        push   esi
        call   dword ptr [edi + 70h]
        push   0b8h
        push   7
        push   esi
        call   dword ptr [edi + 70h]
        cmp    byte ptr [edi + 2ah], 0
        je     SHORT loc_1_0CD7
        push   esi
        call   dword ptr [edi + 60h]
loc_1_0CD7:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0CE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        mov    edx, dword ptr [esi]
        or     al, 60h
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_0D05:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_0D05
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 33h]
        or     al, byte ptr [esi + 8]
        out    dx, al
        mov    ecx, 1
loc_1_0D1D:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_0D1D
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        sar    al, 3
        and    al, 47h
        mov    byte ptr [ebp - 3], al
        mov    ecx, 1
loc_1_0D44:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_0D44
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0b8h
        mov    al, byte ptr [ebp - 4]
        or     al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [ebp - 3], al
        mov    cl, byte ptr [edi + 33h]
        xor    cl, al
        mov    byte ptr [ebp - 4], cl
        or     ebx, ebx
        je     SHORT loc_1_0DE9
        mov    esi, dword ptr [ebp + 0ch]
loc_1_0D94:
        mov    edx, dword ptr [edi]
        add    edx, 2
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    ecx, 1
loc_1_0DA2:
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_0DA2
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [edi]
        add    edx, 2
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        sar    al, 3
        and    al, 47h
        mov    byte ptr [ebp - 1], al
        mov    ecx, 1
loc_1_0DC9:
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_0DC9
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    byte ptr [ebp - 2], 0b8h
        mov    al, byte ptr [ebp - 1]
        or     al, byte ptr [ebp - 2]
        dec    ebx
        mov    byte ptr [esi], al
        inc    esi
        or     ebx, ebx
        jne    SHORT loc_1_0D94
loc_1_0DE9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [eax + 8]
        mov    cl, byte ptr [eax + 33h]
        xor    cl, bl
        or     esi, esi
        je     SHORT loc_1_0E26
        mov    edx, dword ptr [eax]
        add    dx, 2
loc_1_0E1D:
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    esi
        jne    SHORT loc_1_0E1D
loc_1_0E26:
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_0E30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 36h], 68h
        db  0C7h, 046h, 05Ch, 0E0h, 00Ch, 000h, 000h
        db  0C7h, 046h, 060h, 070h, 00Dh, 000h, 000h
        db  0C7h, 046h, 064h, 000h, 00Eh, 000h, 000h
        mov    dword ptr [esi + 0fh], 1
        call   sub_1_3E30
        push   0
        push   0
        push   esi
        call   sub_1_39B0
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0E70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 2ah], 0
        jne    SHORT loc_1_0E82
        sub    al, al
        pop    ebp
        ret    4
loc_1_0E82:
        lea    ecx, [eax + 20h]
        push   ecx
        call   dword ptr [eax + 5ch]
        mov    al, 1
        pop    ebp
        ret    4
        db  090h
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 730h], 0
        jne    NEAR PTR loc_1_105A
        mov    byte ptr [esi + 730h], 1
        inc    dword ptr [esi + 0ach]
        mov    dword ptr [ebp - 8], 0
        lea    edi, [esi + 20h]
loc_1_0EC0:
        push   0
        push   edi
        call   dword ptr [esi + 7ch]
        mov    byte ptr [ebp - 1], al
        push   1
        push   edi
        call   dword ptr [esi + 7ch]
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_1_0EDF
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_0F9B
loc_1_0EDF:
        cmp    bl, 0ffh
        je     SHORT loc_1_0EEF
        cmp    byte ptr [ebp - 1], 0ffh
        je     SHORT loc_1_0EEF
        cmp    bl, byte ptr [ebp - 1]
        jne    SHORT loc_1_0EFD
loc_1_0EEF:
        push   esi
        call   sub_1_0670
        or     al, al
        je     NEAR PTR loc_1_0F9B
loc_1_0EFD:
        test   bl, 81h
        je     SHORT loc_1_0F19
        test   bl, 1
        je     SHORT loc_1_0F0D
        inc    dword ptr [esi + 6fbh]
loc_1_0F0D:
        push   esi
        call   sub_1_1070
        push   esi
        call   sub_1_1990
loc_1_0F19:
        test   bl, 40h
        je     SHORT loc_1_0F28
        and    bl, 7eh
        push   ebx
        push   1
        push   edi
        call   dword ptr [esi + 70h]
loc_1_0F28:
        test   byte ptr [ebp - 1], 83h
        jne    SHORT loc_1_0F37
        cmp    byte ptr [esi + 0cah], 0
        je     SHORT loc_1_0F8E
loc_1_0F37:
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   0
        push   edi
        call   dword ptr [esi + 70h]
        mov    dword ptr [esi + 0b0h], 0
        cmp    byte ptr [esi + 0cah], 0
        je     SHORT loc_1_0F64
        mov    byte ptr [esi + 0cbh], 2
        mov    byte ptr [esi + 0cah], 0
        jmp    SHORT loc_1_0F6D
loc_1_0F64:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0cbh], al
loc_1_0F6D:
        mov    byte ptr [esi + 769h], 0
        cmp    dword ptr [esi + 0c6h], 0
        push   esi
        je     SHORT loc_1_0F85
        call   sub_1_1260
        jmp    SHORT loc_1_0F8E
loc_1_0F85:
        call   sub_1_0670
        or     al, al
        je     SHORT loc_1_0F9B
loc_1_0F8E:
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 8
        jl     NEAR PTR loc_1_0EC0
loc_1_0F9B:
        cmp    dword ptr [esi + 718h], 0
        je     SHORT loc_1_0FBC
loc_1_0FA4:
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_0FBC
        push   esi
        call   sub_1_1070
        cmp    dword ptr [esi + 718h], 0
        jne    SHORT loc_1_0FA4
loc_1_0FBC:
        cmp    byte ptr [esi + 76bh], 0
        je     SHORT loc_1_0FCB
        push   esi
        call   sub_1_1360
loc_1_0FCB:
        mov    byte ptr [esi + 730h], 0
        cmp    byte ptr [esi + 76bh], 0
        jne    SHORT loc_1_0FDF
        push   edi
        call   dword ptr [esi + 58h]
loc_1_0FDF:
        mov    eax, dword ptr [esi + 0ach]
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        jne    SHORT loc_1_1020
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_1009
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_1009
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_1020
loc_1_1009:
        push   1
        lea    eax, [esi + 731h]
        push   eax
        call   sub_1_04B2
        jmp    SHORT loc_1_1049
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1020:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_1049
        cmp    dword ptr [esi + 0c6h], 0
        jne    SHORT loc_1_1049
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_1049
        push   edi
        call   dword ptr [esi + 5ch]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 58h]
loc_1_1049:
        push   7d0h
        add    esi, 75dh
        push   esi
        call   sub_1_04B2
loc_1_105A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1070:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        mov    esi, dword ptr [ebp + 8]
loc_1_1084:
        cmp    byte ptr [esi + 6ffh], 0
        jne    NEAR PTR loc_1_11DC
        lea    edi, [esi + 20h]
        push   81h
        push   1
        push   edi
        call   dword ptr [esi + 70h]
        push   5
        push   edi
        call   dword ptr [esi + 7ch]
        test   al, 40h
        jne    NEAR PTR loc_1_11DC
        inc    dword ptr [esi + 0b0h]
        mov    dword ptr [esi + 720h], 0
        push   edi
        call   sub_1_3720
        push   4
        lea    eax, [esi + 6d3h]
        push   eax
        push   edi
        call   dword ptr [esi + 80h]
        mov    ax, word ptr [esi + 6d5h]
        mov    word ptr [esi + 0d1h], ax
        mov    ebx, dword ptr [esi + 714h]
        add    bx, 0eh
        cmp    ax, bx
        jae    SHORT loc_1_10F3
        mov    bx, ax
loc_1_10F3:
        mov    byte ptr [ebp - 1], 1
        test   byte ptr [esi + 0cch], 28h
        jne    SHORT loc_1_1150
        push   6
        lea    eax, [esi + 0d3h]
        push   eax
        push   edi
        call   dword ptr [esi + 80h]
        sub    word ptr [esi + 0d1h], 6
        sub    bx, 6
        cmp    dword ptr [esi + 0d5h], -1
        jne    SHORT loc_1_1140
        cmp    byte ptr [esi + 0d3h], 0ffh
        jne    SHORT loc_1_1140
        cmp    byte ptr [esi + 0d4h], 0ffh
        jne    SHORT loc_1_1140
        mov    byte ptr [ebp - 1], 0
        jmp    SHORT loc_1_116D
        db  08Dh, 049h, 000h
loc_1_1140:
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        lea    eax, [esi + 0d9h]
        jmp    SHORT loc_1_115E
loc_1_1150:
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        lea    eax, [esi + 0d3h]
loc_1_115E:
        push   eax
        push   edi
        call   dword ptr [esi + 80h]
        sub    word ptr [esi + 0d1h], bx
loc_1_116D:
        push   edi
        call   sub_1_3770
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_118E
        push   esi
        call   sub_1_1590
        cmp    eax, 3
        je     SHORT loc_1_118E
        inc    dword ptr [esi + 6ebh]
        mov    byte ptr [ebp - 2], 1
loc_1_118E:
        mov    ax, word ptr [esi + 0d1h]
        or     ax, ax
        je     SHORT loc_1_11C1
        cmp    ax, 8
        ja     SHORT loc_1_11B9
        mov    ebx, eax
        and    ebx, 0ffffh
        or     ebx, ebx
        jle    SHORT loc_1_11C1
loc_1_11AC:
        push   8
        push   edi
        call   dword ptr [esi + 7ch]
        dec    ebx
        or     ebx, ebx
        jg     SHORT loc_1_11AC
        jmp    SHORT loc_1_11C1
loc_1_11B9:
        push   4
        push   0eh
        push   edi
        call   dword ptr [esi + 70h]
loc_1_11C1:
        cmp    dword ptr [esi + 0b0h], 5
        jbe    NEAR PTR loc_1_1084
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 0b0h], 0
loc_1_11DC:
        cmp    dword ptr [esi + 718h], 0
        je     SHORT loc_1_1227
loc_1_11E5:
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_1230
        mov    edi, dword ptr [esi + 718h]
        push   edi
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 718h], eax
        mov    dword ptr [esi + 720h], edi
        mov    ebx, dword ptr [edi + 28h]
        push   esi
        call   sub_1_13F0
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 718h], 0
        jne    SHORT loc_1_11E5
loc_1_1227:
        cmp    byte ptr [esi + 6ffh], 0
        je     SHORT loc_1_1240
loc_1_1230:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1240:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_1252
        mov    eax, dword ptr [esi + 708h]
        push   eax
        call   sub_1_0290
loc_1_1252:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1260:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0c6h]
        or     edi, edi
        jne    SHORT loc_1_1291
        push   2
        push   6
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1291:
        mov    al, byte ptr [esi + 0cbh]
        test   al, 80h
        je     SHORT loc_1_12D1
        test   al, 4
        je     SHORT loc_1_12C9
        push   4
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 7ch]
        and    eax, 0ffh
        and    eax, 0fffffff0h
        cmp    eax, 10h
        jne    SHORT loc_1_12C3
        inc    dword ptr [esi + 6f3h]
        inc    dword ptr [esi + 6e7h]
        jmp    SHORT loc_1_12D7
loc_1_12C3:
        inc    dword ptr [esi + 6f7h]
loc_1_12C9:
        inc    dword ptr [esi + 6e7h]
        jmp    SHORT loc_1_12D7
loc_1_12D1:
        inc    dword ptr [esi + 6efh]
loc_1_12D7:
        cmp    byte ptr [edi + 2ch], 0
        jne    SHORT loc_1_1300
        mov    al, byte ptr [esi + 0cbh]
        and    al, 80h
        cmp    al, 1
        mov    ebx, dword ptr [edi + 28h]
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 1ch]
        jmp    SHORT loc_1_1327
loc_1_1300:
        cmp    dword ptr [esi + 718h], 0
        jne    SHORT loc_1_1311
        mov    dword ptr [esi + 718h], edi
        jmp    SHORT loc_1_131A
loc_1_1311:
        mov    eax, dword ptr [esi + 71ch]
        mov    dword ptr [eax + 24h], edi
loc_1_131A:
        mov    dword ptr [esi + 71ch], edi
        mov    dword ptr [edi + 24h], 0
loc_1_1327:
        mov    dword ptr [esi + 0c6h], 0
        cmp    byte ptr [esi + 6ffh], 0
        je     SHORT loc_1_1349
        push   2
        push   esi
        call   sub_1_3440
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1349:
        push   esi
        call   sub_1_1990
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1360:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0c6h]
        or     ecx, ecx
        je     SHORT loc_1_1396
        mov    dword ptr [esi + 0c6h], 0
        inc    dword ptr [esi + 6efh]
        mov    edi, dword ptr [ecx + 28h]
        push   0c0000001h
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 1ch]
loc_1_1396:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_13D5
        nop
loc_1_13A0:
        mov    edx, dword ptr [esi + 0beh]
        push   0c0000001h
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 0beh], ecx
        mov    edi, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 6efh]
        cmp    dword ptr [esi + 0beh], 0
        jne    SHORT loc_1_13A0
loc_1_13D5:
        cmp    byte ptr [esi + 6ffh], 0
        je     SHORT loc_1_13E6
        push   2
        push   esi
        call   sub_1_3440
loc_1_13E6:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_13F0:
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
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], 6
        jae    SHORT loc_1_1420
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1420:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 714h]
        add    edi, 0eh
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_1434
        mov    edi, dword ptr [ebp - 4]
loc_1_1434:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0d3h]
        push   eax
        call   sub_1_14B0
        cmp    edi, 0eh
        jae    SHORT loc_1_1470
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0d3h]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 708h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1470:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        sub    edi, 0eh
        push   edi
        lea    eax, [esi + 0e1h]
        push   eax
        push   0eh
        lea    eax, [esi + 0d3h]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 708h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14B0:
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
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 10h]
        je     SHORT loc_1_1504
loc_1_14D5:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    eax, ebx
        ja     SHORT loc_1_1504
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_14D5
loc_1_1504:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1515
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1515:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0ch], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_1524:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_1533
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_1536
loc_1_1533:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1536:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05BC
        add    esi, ebx
        cmp    esi, edi
        je     SHORT loc_1_1572
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_157D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        jmp    SHORT loc_1_1524
loc_1_1572:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_157D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1590:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 6d5h]
        cmp    eax, 5eah
        jbe    SHORT loc_1_15B0
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_15B0:
        mov    dword ptr [edx + 6d7h], eax
        mov    ecx, dword ptr [edx + 714h]
        add    ecx, 0eh
        cmp    ecx, eax
        jbe    SHORT loc_1_15C5
        mov    ecx, eax
loc_1_15C5:
        cmp    ecx, 0eh
        jae    SHORT loc_1_15EC
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 0d3h]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 708h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_15EC:
        sub    eax, 0eh
        push   eax
        sub    ecx, 0eh
        push   ecx
        lea    eax, [edx + 0e1h]
        push   eax
        push   0eh
        lea    eax, [edx + 0d3h]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 708h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 720h], 0
        je     NEAR PTR loc_1_16C7
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, edi
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_169A
loc_1_165A:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 720h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_14B0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_169A
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_165A
loc_1_169A:
        cmp    edi, ebx
        jbe    SHORT loc_1_16B5
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_16B5:
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
loc_1_16C7:
        cmp    word ptr [esi + 0d1h], 0
        jbe    SHORT loc_1_1708
        lea    ebx, [esi + 20h]
        push   ebx
        call   sub_1_3720
        xor    eax, eax
        mov    ax, word ptr [esi + 0d1h]
        push   eax
        mov    eax, dword ptr [esi + 714h]
        add    eax, esi
        add    eax, 0e1h
        push   eax
        push   ebx
        call   dword ptr [esi + 80h]
        mov    word ptr [esi + 0d1h], 0
        push   ebx
        call   sub_1_3770
loc_1_1708:
        mov    ecx, dword ptr [esi + 6d7h]
        mov    eax, dword ptr [ebp + 14h]
        lea    edx, [edi + eax]
        cmp    edx, ecx
        jbe    SHORT loc_1_173A
        cmp    edi, ecx
        jbe    SHORT loc_1_1733
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1733:
        sub    ecx, edi
        mov    dword ptr [ebp - 18h], ecx
        jmp    SHORT loc_1_173D
loc_1_173A:
        mov    dword ptr [ebp - 18h], eax
loc_1_173D:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_0500
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_17C1
loc_1_1770:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     SHORT loc_1_177D
        mov    ebx, dword ptr [ebp - 8]
loc_1_177D:
        push   ebx
        lea    eax, [edi + esi]
        add    eax, 0d3h
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05BC
        add    edi, ebx
        sub    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_17C1
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_17C1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1770
loc_1_17C1:
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
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 8]
        lea    ebx, [edi + 24h]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_1810
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1810:
        cmp    byte ptr [esi + 6ffh], 0
        je     SHORT loc_1_1827
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1827:
        cmp    byte ptr [esi + 76bh], 0
        je     SHORT loc_1_1848
        push   esi
        call   sub_1_0670
        or     al, al
        jne    SHORT loc_1_1848
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1848:
        mov    eax, dword ptr [ebp + 8]
        push   0
        inc    dword ptr [eax + 1ch]
        inc    dword ptr [esi + 0ach]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0500
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0524
        mov    ecx, dword ptr [esi + 6ddh]
        lea    edx, [esi + 6dbh]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    ecx, eax
        jae    SHORT loc_1_18A0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_18D7
        db  08Dh, 049h, 000h
loc_1_18A0:
        jbe    SHORT loc_1_18B0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_18D7
        db  005h, 000h, 000h, 000h, 000h
loc_1_18B0:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    SHORT loc_1_18C7
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_18D7
loc_1_18C7:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_1_18D7
        mov    dword ptr [ebp - 4], 0
loc_1_18D7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_18F2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 708h]
        push   eax
        call   sub_1_0330
        mov    byte ptr [ebx + 8], al
        jmp    SHORT loc_1_18F6
loc_1_18F2:
        mov    byte ptr [ebx + 8], 0
loc_1_18F6:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_1910
        mov    eax, dword ptr [esi + 0c2h]
        mov    dword ptr [eax + 24h], edi
        jmp    SHORT loc_1_1916
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1910:
        mov    dword ptr [esi + 0beh], edi
loc_1_1916:
        mov    dword ptr [esi + 0c2h], edi
        mov    eax, dword ptr [esi + 0ach]
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        jne    SHORT loc_1_1956
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_1946
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_1946
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_1956
loc_1_1946:
        push   1
        add    esi, 731h
        push   esi
        call   sub_1_04B2
        jmp    SHORT loc_1_1982
loc_1_1956:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_1982
        cmp    dword ptr [esi + 0c6h], 0
        jne    SHORT loc_1_1982
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_1982
        lea    edi, [esi + 20h]
        push   edi
        call   dword ptr [esi + 5ch]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 58h]
loc_1_1982:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_1990:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 2ah]
        or     al, al
        mov    byte ptr [ebp - 1], al
        je     SHORT loc_1_19AD
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 5ch]
loc_1_19AD:
        cmp    dword ptr [esi + 0c6h], 0
        jne    NEAR PTR loc_1_1AAF
        lea    ebx, [ebx]
loc_1_19C0:
        mov    edi, dword ptr [esi + 0beh]
        or     edi, edi
        je     NEAR PTR loc_1_1AAF
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 0beh], eax
        mov    dword ptr [esi + 0c6h], edi
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_0500
        mov    ebx, dword ptr [ebp - 8]
        cmp    ebx, 3ch
        jae    SHORT loc_1_19FA
        mov    ebx, 3ch
loc_1_19FA:
        push   0b8h
        push   7
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 70h]
        push   ebx
        push   8
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 70h]
        shr    ebx, 8
        push   ebx
        push   8
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 70h]
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_3750
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_0A30
        or     al, al
        jne    SHORT loc_1_1A57
        mov    ebx, dword ptr [edi + 28h]
        push   0c0000001h
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 0c6h], 0
        jmp    SHORT loc_1_1AA2
loc_1_1A57:
        cmp    dword ptr [ebp - 8], 3ch
        jae    SHORT loc_1_1A72
        mov    eax, 3ch
        sub    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 0C0h, 04Dh, 000h, 000h
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 74h]
loc_1_1A72:
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_3840
        mov    al, byte ptr [esi + 0d0h]
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_1_1A8B
        mov    byte ptr [edi + 2ch], 0
loc_1_1A8B:
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_1AC5
        push   81h
        push   0ah
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 70h]
loc_1_1AA2:
        cmp    dword ptr [esi + 0c6h], 0
        je     NEAR PTR loc_1_19C0
loc_1_1AAF:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1ABC
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 58h]
loc_1_1ABC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1AC5:
        mov    ebx, dword ptr [edi + 28h]
        push   0
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 0c6h], 0
        push   3
        push   esi
        call   sub_1_3440
        dec    dword ptr [ebx + 1ch]
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1ABC
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 58h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1B00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 769h], 0
        je     SHORT loc_1_1B45
        cmp    dword ptr [esi + 0c6h], 0
        je     SHORT loc_1_1B55
        mov    byte ptr [esi + 0cah], 1
        mov    byte ptr [esi + 769h], 0
        push   7
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 10h
        lea    eax, [esi + 20h]
        push   eax
        call   dword ptr [esi + 5ch]
        jmp    SHORT loc_1_1B55
loc_1_1B45:
        cmp    dword ptr [esi + 0c6h], 0
        je     SHORT loc_1_1B55
        mov    byte ptr [esi + 769h], 1
loc_1_1B55:
        push   7d0h
        add    esi, 75dh
        push   esi
        call   sub_1_04B2
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    word ptr [ebp - 10h], 8
        mov    word ptr [ebp - 0eh], 8
        db  0C7h, 045h, 0F4h, 03Ch, 04Fh, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_1_041A
        db  089h, 035h, 046h, 04Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 004h, 04Eh, 000h, 000h
        db  0C6h, 005h, 04Ah, 04Eh, 000h, 000h, 000h
        db  0C7h, 005h, 03Eh, 04Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 008h, 04Eh, 000h, 000h, 003h
        db  0C6h, 005h, 009h, 04Eh, 000h, 000h, 000h
        db  0C7h, 005h, 00Ah, 04Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 00Eh, 04Eh, 000h, 000h, 0F0h, 022h, 000h, 000h
        db  0C7h, 005h, 012h, 04Eh, 000h, 000h, 010h, 025h, 000h, 000h
        db  0C7h, 005h, 016h, 04Eh, 000h, 000h, 0E0h, 017h, 000h, 000h
        db  0C7h, 005h, 01Ah, 04Eh, 000h, 000h, 020h, 016h, 000h, 000h
        db  0C7h, 005h, 01Eh, 04Eh, 000h, 000h, 0F0h, 030h, 000h, 000h
        db  0C7h, 005h, 022h, 04Eh, 000h, 000h, 080h, 026h, 000h, 000h
        db  0C7h, 005h, 026h, 04Eh, 000h, 000h, 010h, 02Fh, 000h, 000h
        db  0C7h, 005h, 02Ah, 04Eh, 000h, 000h, 000h, 030h, 000h, 000h
        db  0C7h, 005h, 02Eh, 04Eh, 000h, 000h, 090h, 01Ch, 000h, 000h
        db  0C7h, 005h, 032h, 04Eh, 000h, 000h, 030h, 030h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A3h, 036h, 04Eh, 000h, 000h
        db  089h, 00Dh, 03Ah, 04Eh, 000h, 000h
        push   36h
        db  068h, 008h, 04Eh, 000h, 000h
        db  068h, 000h, 04Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 000h, 04Eh, 000h, 000h
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1C82
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1C82:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_1C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 3ch], 0ah
        mov    word ptr [ebp - 3ah], 0ah
        db  0C7h, 045h, 0C8h, 050h, 04Fh, 000h, 000h
        mov    word ptr [ebp - 34h], 4
        mov    word ptr [ebp - 32h], 4
        db  0C7h, 045h, 0D0h, 060h, 04Fh, 000h, 000h
        mov    word ptr [ebp - 2ch], 15h
        sub    esi, esi
        mov    word ptr [ebp - 2ah], 15h
        db  0C7h, 045h, 0D8h, 07Ch, 04Fh, 000h, 000h
        mov    word ptr [ebp - 44h], 0eh
        mov    word ptr [ebp - 42h], 0eh
        sub    bl, bl
        db  0C7h, 045h, 0C0h, 0D4h, 04Fh, 000h, 000h
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [ebp - 18h], 10h
        db  0A1h, 0FCh, 04Dh, 000h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   esi
        push   76ch
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 8], eax
        cmp    eax, esi
        je     SHORT loc_1_1D19
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1D19:
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    ecx, 1dbh
        rep stosd
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1D5A
        push   0
        push   76ch
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_04DC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1D5A:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1D7D
        mov    eax, dword ptr [ebp - 10h]
        mov    bl, byte ptr [eax + 4]
loc_1_1D7D:
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1DAB
        mov    byte ptr [ebp - 1], 1
        mov    esi, 0c1020001h
        mov    edi, dword ptr [ebp - 24h]
        jmp    NEAR PTR loc_1_1E47
loc_1_1DAB:
        mov    eax, dword ptr [ebp - 10h]
        push   1
        mov    edi, dword ptr [eax + 4]
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1DD9
        mov    byte ptr [ebp - 1], 1
        mov    esi, 0c1020002h
        jmp    SHORT loc_1_1E47
loc_1_1DD9:
        mov    eax, dword ptr [ebp - 10h]
        push   0
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 2], al
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1E08
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], ecx
loc_1_1E08:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1E47
        cmp    dword ptr [ebp - 20h], 6
        jne    SHORT loc_1_1E47
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, edx
        add    ecx, 6dbh
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_1E47:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0560
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0bdh], bl
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 0b4h], edi
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx + 0b8h], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 0b9h], ecx
        push   esi
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1EC0
        or     eax, eax
        je     SHORT loc_1_1EB2
        push   0
        push   76ch
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_04DC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1EB2:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_1EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0FCh, 04Dh, 000h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   0
        push   26h
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    NEAR PTR loc_1_2260
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ah], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [edi + 0b4h]
        mov    dword ptr [ecx + 1ah], eax
        mov    eax, dword ptr [ebp - 0ch]
        lea    esi, [edi + 4]
        mov    dword ptr [eax + 1eh], 3
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ebp - 10h]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 16h], 1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        lea    ebx, [edi + 20h]
        push   edi
        db  0A1h, 000h, 04Eh, 000h, 000h
        push   eax
        push   esi
        call   sub_1_0432
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebx], eax
        push   0
        push   26h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_04DC
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2260
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_1F85
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_2250
loc_1_1F85:
        db  0C7h, 087h, 03Dh, 007h, 000h, 000h, 020h, 03Bh, 000h, 000h
        mov    dword ptr [edi + 731h], 0
        mov    eax, dword ptr [edi + 73dh]
        mov    dword ptr [edi + 735h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [edi + 739h], edi
        push   eax
        push   ebx
        call   sub_1_3890
        or     al, al
        je     NEAR PTR loc_1_2044
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 2
        je     SHORT loc_1_1FE2
        cmp    eax, 3
        je     SHORT loc_1_1FF0
        cmp    eax, 4
        je     SHORT loc_1_2000
        cmp    eax, 5
        je     SHORT loc_1_2010
        cmp    eax, 6
        je     SHORT loc_1_2020
        mov    dword ptr [ebp - 8], 0c1020008h
        jmp    SHORT loc_1_2027
loc_1_1FE2:
        mov    dword ptr [ebp - 8], 0c1020003h
        jmp    SHORT loc_1_2027
        db  005h, 000h, 000h, 000h, 000h
loc_1_1FF0:
        mov    dword ptr [ebp - 8], 0c1020006h
        jmp    SHORT loc_1_2027
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2000:
        mov    dword ptr [ebp - 8], 0c1020005h
        jmp    SHORT loc_1_2027
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2010:
        mov    dword ptr [ebp - 8], 0c1020004h
        jmp    SHORT loc_1_2027
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2020:
        mov    dword ptr [ebp - 8], 0c1020007h
loc_1_2027:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_2250
loc_1_2044:
        cmp    byte ptr [edi + 0bdh], 0
        je     NEAR PTR loc_1_20C3
        mov    eax, dword ptr [edi + 2fh]
        test   al, 8
        je     SHORT loc_1_2060
        mov    eax, 41020009h
        jmp    SHORT loc_1_20B5
        db  090h
loc_1_2060:
        test   al, 4
        je     SHORT loc_1_206B
        mov    eax, 4102000ah
        jmp    SHORT loc_1_20B5
loc_1_206B:
        test   ah, 4
        je     SHORT loc_1_2077
        mov    eax, 4102000bh
        jmp    SHORT loc_1_20B5
loc_1_2077:
        test   ah, 8
        je     SHORT loc_1_2083
        mov    eax, 4102000ch
        jmp    SHORT loc_1_20B5
loc_1_2083:
        test   ah, 1
        je     SHORT loc_1_2090
        mov    eax, 4102000dh
        jmp    SHORT loc_1_20B5
        db  090h
loc_1_2090:
        test   ah, 2
        je     SHORT loc_1_209C
        mov    eax, 4102000eh
        jmp    SHORT loc_1_20B5
loc_1_209C:
        test   al, 2
        je     SHORT loc_1_20A7
        mov    eax, 4102000fh
        jmp    SHORT loc_1_20B5
loc_1_20A7:
        test   al, 1
        mov    eax, 41020010h
        jne    SHORT loc_1_20B5
        mov    eax, 41020011h
loc_1_20B5:
        push   0
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
loc_1_20C3:
        mov    byte ptr [edi + 0d0h], 0
        mov    byte ptr [edi + 730h], 0
        mov    dword ptr [edi + 0c6h], 0
        mov    byte ptr [edi + 769h], 0
        push   edi
        call   sub_1_06F0
        lea    eax, [edi + 708h]
        push   eax
        push   edi
        lea    eax, [edi + 6dbh]
        push   eax
        db  068h, 050h, 036h, 000h, 000h
        db  068h, 0D0h, 035h, 000h, 000h
        db  068h, 050h, 035h, 000h, 000h
        mov    eax, dword ptr [edi + 0b9h]
        push   eax
        db  0E8h, 0EEh, 0DEh, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_2140
        push   3
        push   2
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_2250
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2140:
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 000h, 04Eh, 000h, 000h
        db  0A1h, 03Eh, 04Eh, 000h, 000h
        mov    dword ptr [edi + 0a0h], eax
        db  089h, 03Dh, 03Eh, 04Eh, 000h, 000h
        push   edi
        call   sub_1_07B0
        or     al, al
        jne    SHORT loc_1_2184
        push   1
        push   2
        push   2
        push   0aa000004h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_21F4
loc_1_2184:
        push   edi
        call   sub_1_08C0
        or     al, al
        je     SHORT loc_1_21B0
        push   0
        push   0c1020007h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_21F4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_21B0:
        movsx  eax, byte ptr [edi + 0b8h]
        push   1
        push   0
        push   eax
        push   eax
        db  068h, 090h, 00Eh, 000h, 000h
        push   edi
        db  068h, 070h, 00Eh, 000h, 000h
        mov    eax, dword ptr [esi]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04BE
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2270
        push   0
        push   0aa000005h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
loc_1_21F4:
        db  039h, 03Dh, 03Eh, 04Eh, 000h, 000h
        jne    SHORT loc_1_2210
        mov    eax, dword ptr [edi + 0a0h]
        db  0A3h, 03Eh, 04Eh, 000h, 000h
        jmp    SHORT loc_1_2240
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2210:
        db  08Bh, 00Dh, 03Eh, 04Eh, 000h, 000h
        cmp    dword ptr [ecx + 0a0h], edi
        je     SHORT loc_1_222E
        mov    edi, edi
loc_1_2220:
        mov    ecx, dword ptr [ecx + 0a0h]
        cmp    dword ptr [ecx + 0a0h], edi
        jne    SHORT loc_1_2220
loc_1_222E:
        mov    eax, dword ptr [ecx + 0a0h]
        mov    eax, dword ptr [eax + 0a0h]
        mov    dword ptr [ecx + 0a0h], eax
loc_1_2240:
        push   ebx
        call   dword ptr [edi + 5ch]
        mov    eax, dword ptr [edi + 708h]
        push   eax
        db  0E8h, 0D0h, 0DEh, 0FEh, 0FFh
loc_1_2250:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0438
loc_1_2260:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_1_2270:
        mov    dword ptr [edi + 741h], 0
        db  0C7h, 087h, 045h, 007h, 000h, 000h, 090h, 00Eh, 000h, 000h
        mov    dword ptr [edi + 749h], edi
        mov    dword ptr [edi + 74dh], 0
        lea    eax, [edi + 75dh]
        db  0C7h, 087h, 051h, 007h, 000h, 000h, 090h, 00Eh, 000h, 000h
        mov    dword ptr [edi + 755h], edi
        db  0C7h, 087h, 059h, 007h, 000h, 000h, 000h, 01Bh, 000h, 000h
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [edi + 759h]
        mov    dword ptr [edi + 761h], ecx
        mov    dword ptr [edi + 765h], edi
        push   7d0h
        push   eax
        call   sub_1_04B2
        mov    byte ptr [edi + 76ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_22F0:
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
loc_1_2313:
        or     edx, edx
        je     SHORT loc_1_2322
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_2313
        mov    dword ptr [ecx], edx
loc_1_2322:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_2334
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2334:
        db  0A1h, 0FCh, 04Dh, 000h, 000h
        push   eax
        push   0
        push   71h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D6
        or     eax, eax
        je     SHORT loc_1_2372
        push   4
        push   1
        push   2
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2372:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 0ach]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0a4h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0a4h], eax
        cmp    byte ptr [esi + 6ffh], 0
        jne    NEAR PTR loc_1_2486
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 708h]
        push   eax
        db  0E8h, 0B8h, 0DDh, 0FEh, 0FFh
        or     al, al
        je     NEAR PTR loc_1_2486
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 9ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0eeh
        mov    dword ptr [esi + 714h], 0eeh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0ach]
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        jne    SHORT loc_1_2450
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_2437
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_2437
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_2450
loc_1_2437:
        push   1
        add    esi, 731h
        push   esi
        call   sub_1_04B2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
loc_1_2450:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_247C
        cmp    dword ptr [esi + 0c6h], 0
        jne    SHORT loc_1_247C
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_247C
        lea    edi, [esi + 20h]
        push   edi
        call   dword ptr [esi + 5ch]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 58h]
loc_1_247C:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2486:
        dec    dword ptr [esi + 0ach]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0a4h], eax
        push   71h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04DC
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_24D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0a4h]
        cmp    esi, edx
        je     SHORT loc_1_24F3
loc_1_24E3:
        mov    eax, dword ptr [esi + 18h]
        cmp    edx, eax
        jae    SHORT loc_1_24EC
        mov    edx, eax
loc_1_24EC:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_24E3
loc_1_24F3:
        or     edx, edx
        jne    SHORT loc_1_24FC
        mov    edx, 0eeh
loc_1_24FC:
        mov    dword ptr [ecx + 714h], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2510:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_2533
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2533:
        inc    dword ptr [esi + 0ach]
        inc    dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [esi + 0a4h]
        cmp    ecx, ebx
        jne    SHORT loc_1_2551
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 0a4h], eax
        jmp    SHORT loc_1_2567
loc_1_2551:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_1_255E
loc_1_2556:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_2556
loc_1_255E:
        lea    eax, [ebx + 0ch]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_1_2567:
        push   0
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        mov    eax, dword ptr [esi + 708h]
        push   eax
        db  0E8h, 0C7h, 0DCh, 0FEh, 0FFh
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_25A0
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_1_2596
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_1_25C6
loc_1_2596:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_25C6
        db  08Bh, 0FFh
loc_1_25A0:
        cmp    edi, 103h
        jne    SHORT loc_1_25B2
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_1_25C6
loc_1_25B2:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_25C3
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_1_25C3:
        dec    dword ptr [ebx + 1ch]
loc_1_25C6:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_1_25F0
        mov    eax, dword ptr [esi + 714h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_25DD
        push   esi
        call   sub_1_24D0
loc_1_25DD:
        push   0
        push   71h
        push   ebx
        call   sub_1_04DC
        jmp    SHORT loc_1_2604
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_25F0:
        mov    eax, dword ptr [esi + 0a8h]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 0a8h], ebx
        mov    edi, 103h
loc_1_2604:
        mov    eax, dword ptr [esi + 0ach]
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        jne    SHORT loc_1_2647
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_262E
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_262E
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_2647
loc_1_262E:
        push   1
        add    esi, 731h
        push   esi
        call   sub_1_04B2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2647:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_2673
        cmp    dword ptr [esi + 0c6h], 0
        jne    SHORT loc_1_2673
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_2673
        lea    ebx, [esi + 20h]
        push   ebx
        call   dword ptr [esi + 5ch]
        push   esi
        call   sub_1_1990
        push   ebx
        call   dword ptr [esi + 58h]
loc_1_2673:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2680:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [edi + 0ach]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_26A2
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_26E5
loc_1_26A2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        jne    SHORT loc_1_26B8
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B30
        mov    esi, eax
        jmp    SHORT loc_1_26E5
loc_1_26B8:
        cmp    ecx, 1
        jne    SHORT loc_1_26E0
        cmp    byte ptr [edi + 6ffh], 0
        je     SHORT loc_1_26CD
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_26E5
loc_1_26CD:
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B80
        mov    esi, eax
        jmp    SHORT loc_1_26E5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_26E0:
        mov    esi, 10001h
loc_1_26E5:
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 0ach]
        dec    eax
        mov    dword ptr [edi + 0ach], eax
        jne    SHORT loc_1_2730
        cmp    byte ptr [edi + 6ffh], 0
        jne    SHORT loc_1_2712
        cmp    dword ptr [edi + 724h], 0
        jne    SHORT loc_1_2712
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_1_2730
loc_1_2712:
        push   1
        add    edi, 731h
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2730:
        cmp    dword ptr [edi + 0beh], 0
        je     SHORT loc_1_275C
        cmp    dword ptr [edi + 0c6h], 0
        jne    SHORT loc_1_275C
        cmp    byte ptr [edi + 730h], 0
        jne    SHORT loc_1_275C
        lea    ebx, [edi + 20h]
        push   ebx
        call   dword ptr [edi + 5ch]
        push   edi
        call   sub_1_1990
        push   ebx
        call   dword ptr [edi + 58h]
loc_1_275C:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2770:
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
        ja     SHORT loc_1_27C0
        je     NEAR PTR loc_1_2970
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_27E1
        shl    eax, 2
        db  0FFh, 0A0h, 0DCh, 02Ah, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27C0:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2992
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_29B4
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2A86
loc_1_27E1:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 049h, 000h
loc_1_27F0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2805
        db  0BEh, 0C8h, 04Eh, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_2A92
loc_1_2805:
        db  0BEh, 04Ch, 04Eh, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_2A92
loc_1_2814:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 6ffh], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_1_282E
        mov    dword ptr [ebp - 14h], 0
loc_1_282E:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A92
        db  005h, 000h, 000h, 000h, 000h
loc_1_2840:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 049h, 000h
loc_1_2850:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_2860:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_2870:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_2880:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0c6h], 1
        sbb    eax, eax
        and    eax, 5eeh
        jmp    NEAR PTR loc_1_2A8F
loc_1_2896:
        mov    dword ptr [ebp - 8], 2000h
        jmp    NEAR PTR loc_1_2A92
loc_1_28A2:
        mov    dword ptr [ebp - 8], 5eeh
        jmp    NEAR PTR loc_1_2A92
        db  08Bh, 0FFh
loc_1_28B0:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_28C0:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 6e1h
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05BC
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A92
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_28F0:
        db  0BEh, 0E4h, 04Fh, 000h, 000h
        mov    edi, 23h
        jmp    NEAR PTR loc_1_2A92
        db  090h
loc_1_2900:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2917
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 708h]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_2A8F
loc_1_2917:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_2A8F
loc_1_2925:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2939
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 714h]
        jmp    NEAR PTR loc_1_2A8F
loc_1_2939:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        jmp    NEAR PTR loc_1_2A8F
loc_1_2944:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_1_2A92
loc_1_2957:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2A92
loc_1_2963:
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_1_2A92
        db  090h
loc_1_2970:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 6e1h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2A92
loc_1_2992:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 6dbh
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2A92
loc_1_29B4:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_2A21
        mov    eax, dword ptr [ebx + 708h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_29E9
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_1_2A92
loc_1_29E9:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebx + 708h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 05Bh, 0DDh, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_2A92
        jmp    NEAR PTR loc_1_2A92
loc_1_2A21:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 708h]
        push   eax
        db  0E8h, 02Ch, 0DCh, 0FEh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_2A54
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_1_2A92
loc_1_2A54:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 708h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 029h, 0DCh, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2A92
loc_1_2A86:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0b9h]
loc_1_2A8F:
        mov    dword ptr [ebp - 8], eax
loc_1_2A92:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2ACE
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_2ACE
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_2ABE
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
loc_1_2ABE:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_05BC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_2ACE:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh, 0F0h, 027h, 000h, 000h, 014h, 028h, 000h, 000h, 040h, 028h, 000h, 000h, 040h, 028h
        db  000h, 000h, 050h, 028h, 000h, 000h, 060h, 028h, 000h, 000h, 070h, 028h, 000h, 000h, 080h, 028h
        db  000h, 000h, 096h, 028h, 000h, 000h, 0A2h, 028h, 000h, 000h, 0B0h, 028h, 000h, 000h, 0C0h, 028h
        db  000h, 000h, 0F0h, 028h, 000h, 000h, 000h, 029h, 000h, 000h, 025h, 029h, 000h, 000h, 044h, 029h
        db  000h, 000h, 057h, 029h, 000h, 000h, 0E1h, 027h, 000h, 000h, 063h, 029h, 000h, 000h, 08Dh, 064h
        db  024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2B30:
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
        call   sub_1_2770
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_2B80:
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
        je     SHORT loc_1_2BD7
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_2C38
        cmp    eax, 10112h
        je     NEAR PTR loc_1_2CC0
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2CC4
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2D05
loc_1_2BD7:
        cmp    edi, 4
        je     SHORT loc_1_2BF4
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 4
        jmp    NEAR PTR loc_1_2D05
loc_1_2BF4:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05BC
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_2C21
        mov    ebx, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2D05
loc_1_2C21:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2D20
        jmp    NEAR PTR loc_1_2D03
loc_1_2C38:
        cmp    edi, 4
        je     SHORT loc_1_2C55
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2D05
loc_1_2C55:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05BC
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_2CB8
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 714h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_2C90
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 714h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_2D05
        db  08Dh, 064h, 024h, 000h
loc_1_2C90:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_1_2CB0
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_2CB0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        push   edx
        call   sub_1_24D0
        jmp    SHORT loc_1_2D05
        db  08Bh, 0FFh
loc_1_2CB0:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_2D05
loc_1_2CB8:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_2D05
        db  090h
loc_1_2CC0:
        sub    ebx, ebx
        jmp    SHORT loc_1_2D05
loc_1_2CC4:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_1_2CE8
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_2D05
loc_1_2CE8:
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
        call   sub_1_2D80
loc_1_2D03:
        mov    ebx, eax
loc_1_2D05:
        or     ebx, ebx
        jne    SHORT loc_1_2D13
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_1_2D13:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_2D20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6ffh], 0
        jne    SHORT loc_1_2D6A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_2D60
        inc    dword ptr [esi + 1ch]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 708h]
        push   eax
        db  0E8h, 08Ah, 0D8h, 0FEh, 0FFh
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_2D60:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_2D6A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6ffh], 0
        jne    SHORT loc_1_2DD0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_2DC2
        inc    dword ptr [esi + 1ch]
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 708h]
        push   eax
        db  0E8h, 046h, 0D5h, 0FEh, 0FFh
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_1_2DC2:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_1_2DD0:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2DE0:
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
        call   sub_1_2770
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_1_2EF6
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2E66
        je     SHORT loc_1_2EB0
        cmp    eax, 20101h
        je     SHORT loc_1_2E82
        cmp    eax, 20102h
        je     SHORT loc_1_2E90
        cmp    eax, 20103h
        je     SHORT loc_1_2E98
        cmp    eax, 20104h
        je     SHORT loc_1_2EA0
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2ED9
loc_1_2E66:
        cmp    eax, 1020101h
        je     SHORT loc_1_2EB8
        cmp    eax, 1020102h
        je     SHORT loc_1_2EC1
        cmp    eax, 1020103h
        je     SHORT loc_1_2ED0
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2ED9
loc_1_2E82:
        mov    eax, dword ptr [ebx + 6e7h]
        jmp    SHORT loc_1_2ED6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2E90:
        mov    eax, dword ptr [ebx + 6ebh]
        jmp    SHORT loc_1_2ED6
loc_1_2E98:
        mov    eax, dword ptr [ebx + 6efh]
        jmp    SHORT loc_1_2ED6
loc_1_2EA0:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2ED9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EB0:
        mov    eax, dword ptr [ebx + 6fbh]
        jmp    SHORT loc_1_2ED6
loc_1_2EB8:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2ED9
loc_1_2EC1:
        mov    eax, dword ptr [ebx + 6f3h]
        jmp    SHORT loc_1_2ED6
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2ED0:
        mov    eax, dword ptr [ebx + 6f7h]
loc_1_2ED6:
        mov    dword ptr [ebp - 4], eax
loc_1_2ED9:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_2EF6
        or     edi, edi
        jne    SHORT loc_1_2EF6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05BC
        add    dword ptr [ebp - 8], 4
loc_1_2EF6:
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
loc_1_2F10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2F43
        cmp    eax, 20101h
        jae    SHORT loc_1_2F64
        cmp    eax, 10101h
        jb     SHORT loc_1_2F5F
        cmp    eax, 10111h
        jbe    SHORT loc_1_2F64
        cmp    eax, 10113h
        je     SHORT loc_1_2F64
        jmp    SHORT loc_1_2F5F
loc_1_2F43:
        cmp    eax, 1010101h
        jb     SHORT loc_1_2F5F
        cmp    eax, 1010104h
        jbe    SHORT loc_1_2F64
        cmp    eax, 1020101h
        jb     SHORT loc_1_2F5F
        cmp    eax, 1020103h
        jbe    SHORT loc_1_2F64
loc_1_2F5F:
        mov    esi, 0c0010017h
loc_1_2F64:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0ach]
        or     esi, esi
        jne    SHORT loc_1_2F7A
        push   ecx
        push   edi
        call   sub_1_2DE0
        mov    esi, eax
loc_1_2F7A:
        mov    eax, dword ptr [edi + 0ach]
        dec    eax
        mov    dword ptr [edi + 0ach], eax
        jne    SHORT loc_1_2FC0
        cmp    byte ptr [edi + 6ffh], 0
        jne    SHORT loc_1_2FA4
        cmp    dword ptr [edi + 724h], 0
        jne    SHORT loc_1_2FA4
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_1_2FC0
loc_1_2FA4:
        push   1
        add    edi, 731h
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_2FC0:
        cmp    dword ptr [edi + 0beh], 0
        je     SHORT loc_1_2FEC
        cmp    dword ptr [edi + 0c6h], 0
        jne    SHORT loc_1_2FEC
        cmp    byte ptr [edi + 730h], 0
        jne    SHORT loc_1_2FEC
        lea    ebx, [edi + 20h]
        push   ebx
        call   dword ptr [edi + 5ch]
        push   edi
        call   sub_1_1990
        push   ebx
        call   dword ptr [edi + 58h]
loc_1_2FEC:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 000h, 04Eh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        db  0A1h, 004h, 04Eh, 000h, 000h
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3030:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 76ah], 1
        db  039h, 035h, 03Eh, 04Eh, 000h, 000h
        jne    SHORT loc_1_3057
        mov    eax, dword ptr [esi + 0a0h]
        db  0A3h, 03Eh, 04Eh, 000h, 000h
        jmp    SHORT loc_1_3085
loc_1_3057:
        db  08Bh, 00Dh, 03Eh, 04Eh, 000h, 000h
        cmp    dword ptr [ecx + 0a0h], esi
        je     SHORT loc_1_3073
loc_1_3065:
        mov    ecx, dword ptr [ecx + 0a0h]
        cmp    dword ptr [ecx + 0a0h], esi
        jne    SHORT loc_1_3065
loc_1_3073:
        mov    eax, dword ptr [ecx + 0a0h]
        mov    eax, dword ptr [eax + 0a0h]
        mov    dword ptr [ecx + 0a0h], eax
loc_1_3085:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 75dh]
        push   eax
        call   sub_1_04B8
        mov    edi, 61a8h
        lea    ebx, [ebx]
loc_1_30A0:
        push   64h
        call   sub_1_04D0
        dec    edi
        jne    SHORT loc_1_30A0
        mov    eax, dword ptr [esi + 708h]
        push   eax
        db  0E8h, 06Ah, 0D0h, 0FEh, 0FFh
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_04C4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0438
        push   0
        push   76ch
        push   esi
        call   sub_1_04DC
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_30F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_3113
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3113:
        inc    dword ptr [esi + 1ch]
        inc    dword ptr [edi + 0ach]
        cmp    byte ptr [edi + 6ffh], 0
        je     SHORT loc_1_3140
        dec    dword ptr [esi + 1ch]
        dec    dword ptr [edi + 0ach]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_3140:
        mov    ebx, dword ptr [edi + 0a4h]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     SHORT loc_1_3173
loc_1_3151:
        inc    dword ptr [ebx + 1ch]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010004h
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [ebx + 0ch]
        dec    dword ptr [ebx + 1ch]
        mov    ebx, eax
        or     eax, eax
        jne    SHORT loc_1_3151
loc_1_3173:
        mov    byte ptr [edi + 6ffh], 1
        mov    dword ptr [edi + 700h], 0
        mov    dword ptr [edi + 704h], esi
        push   edi
        call   sub_1_3220
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 1ch]
        mov    eax, dword ptr [edi + 0ach]
        dec    eax
        mov    dword ptr [edi + 0ach], eax
        jne    SHORT loc_1_31E0
        cmp    byte ptr [edi + 6ffh], 0
        jne    SHORT loc_1_31C0
        cmp    dword ptr [edi + 724h], 0
        jne    SHORT loc_1_31C0
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_1_31E0
loc_1_31C0:
        push   1
        add    edi, 731h
        push   edi
        call   sub_1_04B2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_31E0:
        cmp    dword ptr [edi + 0beh], 0
        je     SHORT loc_1_320C
        cmp    dword ptr [edi + 0c6h], 0
        jne    SHORT loc_1_320C
        cmp    byte ptr [edi + 730h], 0
        jne    SHORT loc_1_320C
        lea    esi, [edi + 20h]
        push   esi
        call   dword ptr [edi + 5ch]
        push   edi
        call   sub_1_1990
        push   esi
        call   dword ptr [edi + 58h]
loc_1_320C:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3220:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0ach]
        cmp    dword ptr [esi + 724h], 0
        je     SHORT loc_1_326F
loc_1_3237:
        mov    ecx, dword ptr [esi + 724h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 724h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_1_3263
        cmp    eax, 8
        je     SHORT loc_1_3263
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_3263:
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_3237
loc_1_326F:
        cmp    dword ptr [esi + 0c6h], 0
        je     SHORT loc_1_3293
        mov    dword ptr [esi + 700h], 2
        dec    dword ptr [esi + 0ach]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3293:
        dec    dword ptr [esi + 0ach]
        push   esi
        call   sub_1_32B0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0ach]
        inc    ecx
        mov    dword ptr [eax + 0ach], ecx
        cmp    dword ptr [eax + 0c6h], 0
        je     SHORT loc_1_32E5
        mov    dword ptr [eax + 700h], 3
        dec    dword ptr [eax + 0ach]
        mov    eax, 103h
        pop    ebp
        ret    4
loc_1_32E5:
        dec    ecx
        mov    dword ptr [eax + 0ach], ecx
        push   eax
        call   sub_1_3300
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3300:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0ach]
        mov    ecx, dword ptr [esi + 0c6h]
        or     ecx, ecx
        je     SHORT loc_1_333A
        mov    edi, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 1ch]
        mov    dword ptr [esi + 0c6h], 0
loc_1_333A:
        mov    edx, dword ptr [esi + 0beh]
        or     edx, edx
        je     SHORT loc_1_336E
loc_1_3344:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 0beh], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 0beh]
        or     edx, edx
        jne    SHORT loc_1_3344
loc_1_336E:
        mov    edx, dword ptr [esi + 718h]
        or     edx, edx
        je     SHORT loc_1_33A2
loc_1_3378:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 718h], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 718h]
        or     edx, edx
        jne    SHORT loc_1_3378
loc_1_33A2:
        cmp    byte ptr [esi + 730h], 0
        je     SHORT loc_1_33C3
loc_1_33AB:
        mov    edi, 64h
loc_1_33B0:
        push   64h
        call   sub_1_04D0
        dec    edi
        jne    SHORT loc_1_33B0
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_33AB
loc_1_33C3:
        push   esi
        call   sub_1_09F0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 6ffh], 0
        mov    edi, dword ptr [esi + 0a4h]
        or     edi, edi
        je     SHORT loc_1_3426
        sub    ebx, ebx
loc_1_33E8:
        inc    dword ptr [edi + 1ch]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_33FF
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_33FF:
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
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        cmp    eax, ebx
        jne    SHORT loc_1_33E8
loc_1_3426:
        dec    dword ptr [esi + 0ach]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3440:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 700h], eax
        jne    NEAR PTR loc_1_3543
        cmp    eax, 1
        je     SHORT loc_1_34D0
        cmp    eax, 2
        je     SHORT loc_1_34D8
        cmp    eax, 3
        je     SHORT loc_1_34E0
        mov    eax, dword ptr [ebp - 4]
loc_1_346C:
        cmp    eax, 103h
        je     SHORT loc_1_348B
        push   eax
        mov    eax, dword ptr [esi + 704h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 704h]
        dec    dword ptr [eax + 1ch]
loc_1_348B:
        mov    eax, dword ptr [esi + 0ach]
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        jne    SHORT loc_1_3517
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_34B5
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_34B5
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_3517
loc_1_34B5:
        push   1
        add    esi, 731h
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_34D0:
        push   esi
        call   sub_1_3220
        jmp    SHORT loc_1_346C
loc_1_34D8:
        push   esi
        call   sub_1_32B0
        jmp    SHORT loc_1_346C
loc_1_34E0:
        mov    eax, dword ptr [esi + 0ach]
        inc    eax
        mov    dword ptr [esi + 0ach], eax
        cmp    dword ptr [esi + 0c6h], 0
        je     SHORT loc_1_3505
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3505:
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        push   esi
        call   sub_1_3300
        jmp    NEAR PTR loc_1_346C
loc_1_3517:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_3543
        cmp    dword ptr [esi + 0c6h], 0
        jne    SHORT loc_1_3543
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_3543
        lea    edi, [esi + 20h]
        push   edi
        call   dword ptr [esi + 5ch]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 58h]
loc_1_3543:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3550:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_3563
        lea    edx, [eax + 49h]
loc_1_3563:
        cmp    byte ptr [ecx + 6ffh], 0
        je     SHORT loc_1_3575
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_3575:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 724h], 0
        jne    SHORT loc_1_35B0
        mov    dword ptr [ecx + 728h], edx
        mov    dword ptr [ecx + 724h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_35B0:
        mov    eax, dword ptr [ecx + 728h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 728h], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_35E3
        lea    edx, [eax + 21h]
loc_1_35E3:
        cmp    byte ptr [ecx + 6ffh], 0
        je     SHORT loc_1_35F5
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_35F5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 724h], 0
        jne    SHORT loc_1_3630
        mov    dword ptr [ecx + 728h], edx
        mov    dword ptr [ecx + 724h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_3630:
        mov    eax, dword ptr [ecx + 728h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 728h], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3650:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 1ch]
        ret    4
        db  08Dh, 049h, 000h
sub_1_3660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0ah], 1
        call   sub_1_4210
        mov    al, 1
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_367B
        mov    al, 3
loc_1_367B:
        push   eax
        push   10h
        push   esi
        call   dword ptr [esi + 50h]
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3690:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0ah], 0
        call   sub_1_4240
        sub    al, al
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_36AB
        mov    al, 2
loc_1_36AB:
        push   eax
        push   10h
        push   esi
        call   dword ptr [esi + 50h]
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_36C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3690
        push   esi
        call   sub_1_3660
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, 1
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_36F0
        mov    cl, 3
loc_1_36F0:
        push   ecx
        push   10h
        push   eax
        call   dword ptr [eax + 50h]
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_3700:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3710:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3720:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 36h]
        push   eax
        push   esi
        call   dword ptr [esi + 4ch]
        test   byte ptr [esi + 0fh], 6
        je     SHORT loc_1_3749
        push   0
        push   esi
        call   dword ptr [esi + 4ch]
        mov    al, 1
        test   byte ptr [esi + 33h], al
        je     SHORT loc_1_3744
        mov    al, 5
loc_1_3744:
        push   eax
        push   esi
        call   dword ptr [esi + 4ch]
loc_1_3749:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3750:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 37h]
        push   ecx
        push   eax
        call   dword ptr [eax + 4ch]
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3770:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0fh], 6
        je     NEAR PTR loc_1_3829
        mov    bl, byte ptr [esi + 9]
        mov    cl, byte ptr [esi + 33h]
        and    cl, bl
        or     cl, 8
        or     cl, byte ptr [esi + 8]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        out    dx, al
        mov    al, byte ptr [esi + 34h]
        and    al, bl
        mov    byte ptr [esi + 9], al
        mov    edi, 1
loc_1_37A9:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    edi
        jne    SHORT loc_1_37A9
        and    cl, byte ptr [esi + 34h]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_37C0:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_37C0
        mov    dx, word ptr [esi]
        mov    al, 58h
        out    dx, al
        mov    ecx, 1
loc_1_37D2:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_37D2
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    ecx, 1
loc_1_37E7:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_37E7
        push   0
        push   esi
        call   dword ptr [esi + 4ch]
        mov    al, 2
        test   byte ptr [esi + 33h], 1
        je     SHORT loc_1_37FE
        mov    al, 6
loc_1_37FE:
        push   eax
        push   esi
        call   dword ptr [esi + 4ch]
        mov    edi, dword ptr [esi]
        add    edi, 2
        mov    cl, byte ptr [esi + 8]
        or     cl, byte ptr [esi + 33h]
        mov    dx, di
        mov    al, cl
        out    dx, al
        mov    ecx, 3
loc_1_3819:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_3819
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
loc_1_3829:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3840:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0fh], 0ch
        jne    SHORT loc_1_3886
        mov    cl, byte ptr [esi + 8]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    bl, byte ptr [esi + 34h]
        and    bl, cl
        mov    edi, dword ptr [esi]
        mov    byte ptr [esi + 8], bl
        lea    ecx, [edi + 2]
        mov    al, bl
        or     al, 8
        mov    dx, cx
        out    dx, al
        mov    dx, di
        mov    al, 18h
        out    dx, al
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [esi + 33h]
        or     al, bl
        out    dx, al
        mov    al, bl
        out    dx, al
loc_1_3886:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 36h], 0e8h
        mov    byte ptr [esi + 37h], 0c8h
        call   sub_1_41A0
        or     eax, eax
        jne    SHORT loc_1_38C2
        mov    eax, dword ptr [ebp + 0ch]
        pop    edi
        mov    dword ptr [eax], 2
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_38C2:
        cmp    eax, 113h
        je     SHORT loc_1_38E0
        mov    eax, dword ptr [ebp + 0ch]
        pop    edi
        mov    dword ptr [eax], 3
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_38E0:
        mov    eax, dword ptr [esi + 0bh]
        test   ah, 40h
        je     SHORT loc_1_38ED
        test   ah, 80h
        je     SHORT loc_1_38F5
loc_1_38ED:
        mov    byte ptr [esi + 33h], 2
        mov    byte ptr [esi + 34h], 0fdh
loc_1_38F5:
        sub    ebx, ebx
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [ebx]
loc_1_3900:
        mov    edx, dword ptr [esi + 0bh]
        mov    edi, 80h
        mov    cl, bl
        sar    edi, cl
        test   edx, edi
        je     SHORT loc_1_3927
        mov    eax, dword ptr [esi + 0fh]
        and    eax, edx
        push   eax
        push   esi
        db  0FFh, 014h, 09Dh, 008h, 050h, 000h, 000h
        and    eax, 0ffh
        or     eax, eax
        je     SHORT loc_1_3945
loc_1_3927:
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_3900
        or     eax, eax
        je     SHORT loc_1_3945
        mov    eax, dword ptr [ebp + 0ch]
        pop    edi
        mov    dword ptr [eax], 5
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3945:
        db  0C7h, 046h, 038h, 060h, 036h, 000h, 000h
        db  0C7h, 046h, 03Ch, 090h, 036h, 000h, 000h
        db  0C7h, 046h, 040h, 0C0h, 036h, 000h, 000h
        db  0C7h, 046h, 044h, 000h, 037h, 000h, 000h
        db  0C7h, 046h, 048h, 010h, 037h, 000h, 000h
        db  0C7h, 046h, 070h, 090h, 03Ah, 000h, 000h
        db  0C7h, 046h, 06Ch, 0B0h, 03Ah, 000h, 000h
        db  0C7h, 046h, 074h, 0F0h, 03Ah, 000h, 000h
        db  0C7h, 046h, 078h, 0D0h, 03Ah, 000h, 000h
        push   esi
        call   sub_1_3A30
        or     al, al
        je     SHORT loc_1_39A2
        mov    eax, dword ptr [ebp + 0ch]
        pop    edi
        mov    dword ptr [eax], 4
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_39A2:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_39B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        add    dx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 1
        out    dx, al
        mov    al, cl
        out    dx, al
        push   98h
        push   esi
        call   dword ptr [esi + 4ch]
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        push   esi
        call   dword ptr [esi + 4ch]
        mov    al, byte ptr [ebp + 10h]
        test   byte ptr [esi + 33h], 2
        jne    SHORT loc_1_39E7
        or     al, 4
loc_1_39E7:
        push   eax
        push   esi
        call   dword ptr [esi + 4ch]
        mov    eax, dword ptr [esi + 0fh]
        and    eax, 8
        cmp    eax, 1
        cmc
        sbb    al, al
        and    al, 3
        push   eax
        push   esi
        call   dword ptr [esi + 4ch]
        mov    ecx, dword ptr [esi]
        mov    dx, cx
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 2]
        mov    al, byte ptr [esi + 8]
        or     al, byte ptr [esi + 33h]
        out    dx, al
        mov    ecx, 1
loc_1_3A16:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_3A16
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_3A30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 13h]
        push   eax
        push   esi
        call   sub_1_3FA0
        or     ax, ax
        je     SHORT loc_1_3A50
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_3A50:
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_1_3A60
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_3A60:
        cmp    byte ptr [esi + 2fh], 80h
        je     SHORT loc_1_3A70
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_3A70:
        cmp    byte ptr [esi + 30h], 0c7h
        je     SHORT loc_1_3A7D
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_3A7D:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3A90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   4
        push   11h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 50h]
        or     byte ptr [esi + 35h], 4
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   11h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 50h]
        mov    byte ptr [esi + 35h], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3AD0:
        push   ebp
        mov    ebp, esp
        push   11h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 5ch]
        mov    cl, al
        and    cl, 1
        mov    al, cl
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3AF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 35h]
        or     bl, byte ptr [ebp + 0ch]
        push   ebx
        push   11h
        push   esi
        call   dword ptr [esi + 50h]
        mov    al, bl
        or     al, 2
        push   eax
        push   11h
        push   esi
        call   dword ptr [esi + 50h]
        push   ebx
        push   11h
        push   esi
        call   dword ptr [esi + 50h]
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_3B20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 0ach]
        cmp    byte ptr [esi + 6ffh], 0
        jne    NEAR PTR loc_1_3BE4
        cmp    dword ptr [esi + 72ch], 0
        jne    NEAR PTR loc_1_3BE4
        lea    esp, [esp]
loc_1_3B50:
        mov    edi, dword ptr [esi + 724h]
        or     edi, edi
        je     SHORT loc_1_3BBC
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 724h], eax
        cmp    dword ptr [esi + 728h], edi
        jne    SHORT loc_1_3B74
        mov    dword ptr [esi + 728h], 0
loc_1_3B74:
        mov    dword ptr [esi + 72ch], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_3B90
        cmp    eax, 0ah
        je     SHORT loc_1_3B90
        push   esi
        call   sub_1_3CC0
        jmp    SHORT loc_1_3B96
        db  090h
loc_1_3B90:
        push   esi
        call   sub_1_3D40
loc_1_3B96:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_3BB0
        cmp    ecx, 4
        je     SHORT loc_1_3BB0
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_3BB0:
        dec    dword ptr [ebx + 1ch]
        cmp    byte ptr [esi + 6ffh], 0
        je     SHORT loc_1_3B50
loc_1_3BBC:
        mov    dword ptr [esi + 72ch], 0
        cmp    byte ptr [esi + 6ffh], 0
        je     SHORT loc_1_3BE4
        dec    dword ptr [esi + 0ach]
        push   1
        push   esi
        call   sub_1_3440
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3BE4:
        mov    edi, dword ptr [esi + 0a8h]
        or     edi, edi
        je     SHORT loc_1_3C48
        sub    ebx, ebx
loc_1_3BF0:
        cmp    dword ptr [edi + 1ch], 0
        jbe    SHORT loc_1_3C00
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_3C44
        db  08Dh, 049h, 000h
loc_1_3C00:
        mov    eax, dword ptr [esi + 714h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_3C11
        push   esi
        call   sub_1_24D0
loc_1_3C11:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [edi + 0ch]
        je     SHORT loc_1_3C30
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_3C36
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3C30:
        mov    dword ptr [esi + 0a8h], eax
loc_1_3C36:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   71h
        push   eax
        call   sub_1_04DC
loc_1_3C44:
        or     edi, edi
        jne    SHORT loc_1_3BF0
loc_1_3C48:
        mov    eax, dword ptr [esi + 0ach]
        dec    eax
        mov    dword ptr [esi + 0ach], eax
        jne    SHORT loc_1_3C87
        cmp    byte ptr [esi + 6ffh], 0
        jne    SHORT loc_1_3C72
        cmp    dword ptr [esi + 724h], 0
        jne    SHORT loc_1_3C72
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_3C87
loc_1_3C72:
        push   1
        add    esi, 731h
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3C87:
        cmp    dword ptr [esi + 0beh], 0
        je     SHORT loc_1_3CB3
        cmp    dword ptr [esi + 0c6h], 0
        jne    SHORT loc_1_3CB3
        cmp    byte ptr [esi + 730h], 0
        jne    SHORT loc_1_3CB3
        lea    edi, [esi + 20h]
        push   edi
        call   dword ptr [esi + 5ch]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 58h]
loc_1_3CB3:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 708h]
        mov    eax, dword ptr [eax + 0ch]
        test   al, 24h
        mov    dword ptr [esi + 0cch], eax
        je     SHORT loc_1_3CF0
        db  068h, 020h, 00Ch, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_04CA
        jmp    SHORT loc_1_3CF5
        db  005h, 000h, 000h, 000h, 000h
loc_1_3CF0:
        call   sub_1_3D40
loc_1_3CF5:
        mov    eax, dword ptr [esi + 0cch]
        test   al, 20h
        je     SHORT loc_1_3D08
        mov    byte ptr [esi + 0d0h], 3
        jmp    SHORT loc_1_3D27
loc_1_3D08:
        test   al, 6
        je     SHORT loc_1_3D15
        mov    byte ptr [esi + 0d0h], 2
        jmp    SHORT loc_1_3D27
loc_1_3D15:
        test   al, 9
        mov    byte ptr [esi + 0d0h], 1
        jne    SHORT loc_1_3D27
        mov    byte ptr [esi + 0d0h], 0
loc_1_3D27:
        push   esi
        db  068h, 0F0h, 00Bh, 000h, 000h
        add    esi, 8
        push   esi
        call   sub_1_04CA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3D40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0B60
        push   esi
        db  068h, 080h, 00Ch, 000h, 000h
        add    esi, 8
        push   esi
        call   sub_1_04CA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3D70:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx]
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [ecx + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3D90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        or     al, 40h
        mov    esi, dword ptr [edi]
        mov    dx, si
        out    dx, al
        lea    ecx, [esi + 2]
        mov    bl, byte ptr [edi + 8]
        mov    al, bl
        or     al, 8
        mov    dx, cx
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        mov    al, byte ptr [edi + 33h]
        or     al, bl
        mov    dx, cx
        out    dx, al
        mov    al, byte ptr [edi + 34h]
        and    al, bl
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        or     esi, esi
        mov    cl, byte ptr [eax + 8]
        je     SHORT loc_1_3E1E
        mov    edi, dword ptr [eax]
        mov    al, byte ptr [eax + 33h]
        mov    byte ptr [ebp - 8], al
        lea    eax, [edi + 2]
        mov    word ptr [ebp - 4], ax
        mov    ebx, dword ptr [ebp + 0ch]
loc_1_3E08:
        mov    dx, di
        mov    al, byte ptr [ebx]
        out    dx, al
        xor    cl, byte ptr [ebp - 8]
        mov    dx, word ptr [ebp - 4]
        mov    al, cl
        out    dx, al
        dec    esi
        inc    ebx
        or     esi, esi
        jne    SHORT loc_1_3E08
loc_1_3E1E:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    byte ptr [eax + 8], cl
        sub    ax, ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_3E30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        db  0C7h, 040h, 050h, 090h, 03Dh, 000h, 000h
        db  0C7h, 040h, 054h, 0E0h, 03Dh, 000h, 000h
        db  0C7h, 040h, 04Ch, 070h, 03Dh, 000h, 000h
        ret    4
        db  090h
loc_1_3E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        mov    edx, dword ptr [esi]
        or     al, 60h
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_3E75:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_3E75
        mov    ecx, dword ptr [esi]
        mov    al, byte ptr [esi + 9]
        lea    edx, [ecx + 2]
        or     al, byte ptr [esi + 33h]
        out    dx, al
        test   byte ptr [esi + 0ch], 2
        je     SHORT loc_1_3E94
        mov    dx, cx
        mov    al, 0ffh
        out    dx, al
loc_1_3E94:
        mov    ecx, 1
loc_1_3E99:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_3E99
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 9]
        test   byte ptr [esi + 0ch], 2
        je     SHORT loc_1_3EDB
        mov    dx, word ptr [esi]
        mov    al, 0ffh
        out    dx, al
loc_1_3EDB:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_3F0E
        mov    dword ptr [ebp - 8], eax
loc_1_3EE8:
        xor    cl, byte ptr [esi + 33h]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        out    dx, al
        mov    ebx, 1
loc_1_3EF8:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        dec    ebx
        jne    SHORT loc_1_3EF8
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [edi], al
        inc    edi
        dec    dword ptr [ebp - 8]
        jne    SHORT loc_1_3EE8
loc_1_3F0E:
        mov    byte ptr [esi + 9], cl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_3F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        mov    bl, byte ptr [ecx + 9]
        je     SHORT loc_1_3F50
loc_1_3F36:
        xor    bl, byte ptr [ecx + 33h]
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, bl
        out    dx, al
        mov    edi, 1
loc_1_3F46:
        mov    edx, dword ptr [ecx]
        inc    edx
        in     al, dx
        dec    edi
        jne    SHORT loc_1_3F46
        dec    esi
        jne    SHORT loc_1_3F36
loc_1_3F50:
        mov    byte ptr [ecx + 9], bl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3F60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0C7h, 046h, 05Ch, 050h, 03Eh, 000h, 000h
        db  0C7h, 046h, 060h, 0C0h, 03Eh, 000h, 000h
        db  0C7h, 046h, 064h, 020h, 03Fh, 000h, 000h
        mov    dword ptr [esi + 0fh], 2
        call   sub_1_3E30
        push   2
        push   0
        push   esi
        call   sub_1_39B0
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 0fh
        mov    esi, dword ptr [ebp + 8]
loc_1_3FBF:
        push   esi
        call   sub_1_4110
        mov    eax, dword ptr [ebp - 8]
        or     eax, 80h
        push   eax
        push   esi
        call   sub_1_4140
        sub    di, di
        mov    ebx, 10h
loc_1_3FDC:
        push   0
        push   esi
        call   dword ptr [esi + 74h]
        add    di, di
        push   esi
        call   dword ptr [esi + 78h]
        and    ax, 0ffh
        or     di, ax
        dec    ebx
        jne    SHORT loc_1_3FDC
        mov    eax, dword ptr [ebp - 0ch]
        add    word ptr [ebp - 2], di
        mov    word ptr [eax], di
        add    dword ptr [ebp - 0ch], 2
        push   0
        push   esi
        call   dword ptr [esi + 74h]
        dec    dword ptr [ebp - 8]
        jns    SHORT loc_1_3FBF
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_1_4035:
        push   30h
        push   esi
        call   sub_1_40E0
        push   20h
        push   esi
        call   sub_1_40E0
        push   esi
        call   sub_1_40B0
        push   esi
        call   sub_1_4110
        push   40h
        push   esi
        call   sub_1_40E0
        mov    eax, dword ptr [ebp + 0ch]
        xor    ebx, ebx
        mov    bl, byte ptr [eax + edi*2]
        add    dword ptr [ebp - 4], ebx
        cmp    edi, 0fh
        jne    SHORT loc_1_406C
        sub    ebx, dword ptr [ebp - 4]
loc_1_406C:
        mov    eax, ebx
        shr    eax, 8
        push   eax
        push   esi
        call   sub_1_4140
        push   ebx
        push   esi
        call   sub_1_4140
        push   esi
        call   sub_1_4180
        push   esi
        call   sub_1_40B0
        inc    edi
        cmp    edi, 10h
        jl     SHORT loc_1_4035
        push   0
        push   esi
        call   sub_1_40E0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   1388h
        call   sub_1_04D0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4110
        push   esi
        call   sub_1_4180
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4110
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_1_4140
        push   esi
        call   sub_1_4180
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4110:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 6ch]
        push   0
        push   esi
        call   dword ptr [esi + 74h]
        push   esi
        call   dword ptr [esi + 70h]
        push   0
        push   esi
        call   dword ptr [esi + 74h]
        push   1
        push   esi
        call   dword ptr [esi + 74h]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [ebp - 1], al
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
loc_1_4153:
        mov    al, byte ptr [ebp - 1]
        mov    cl, 7
        sub    cl, bl
        shr    al, cl
        and    eax, 1
        push   eax
        push   esi
        call   dword ptr [esi + 74h]
        inc    ebx
        cmp    ebx, 8
        jl     SHORT loc_1_4153
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 6ch]
        push   0
        push   esi
        call   dword ptr [esi + 74h]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_41A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 4
        mov    byte ptr [edi + 8], 4
        mov    ecx, dword ptr [edi]
        mov    byte ptr [edi + 9], 24h
        lea    edx, [ecx + 2]
        out    dx, al
        mov    dx, cx
        sub    al, al
        out    dx, al
        push   61a8h
        call   sub_1_04D0
        mov    dx, word ptr [edi]
        mov    al, 0ffh
        out    dx, al
        push   1
        call   sub_1_04D0
        db  0BEh, 028h, 050h, 000h, 000h
        db  083h, 03Dh, 028h, 050h, 000h, 000h, 000h
        je     SHORT loc_1_4207
loc_1_41E3:
        push   edi
        call   dword ptr [esi + 4]
        or     al, al
        je     SHORT loc_1_4203
        mov    edx, dword ptr [edi]
        add    edx, 2
        mov    al, 0ch
        add    esi, 10h
        out    dx, al
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_41E3
        mov    eax, dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4203:
        push   edi
        call   dword ptr [esi + 8]
loc_1_4207:
        mov    eax, dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_4210:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx]
        inc    edx
        in     al, dx
        or     byte ptr [ecx + 8], 10h
        or     byte ptr [ecx + 9], 10h
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, byte ptr [ecx + 8]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4240:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        and    byte ptr [eax + 8], 0efh
        and    byte ptr [eax + 9], 0efh
        mov    edx, dword ptr [eax]
        add    edx, 2
        mov    al, byte ptr [eax + 8]
        out    dx, al
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4260:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4670
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_4480
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_45B0
        or     al, al
        je     SHORT loc_1_4292
        mov    al, 1
        pop    esi
        pop    ebp
        ret    0ch
loc_1_4292:
        push   7fh
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_4540
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_45B0
        cmp    al, 7ch
        je     SHORT loc_1_42B1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    0ch
loc_1_42B1:
        sub    al, al
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_42C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_43C0
        or     dword ptr [esi + 0bh], eax
        or     dword ptr [esi + 0bh], 1
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_4430
        or     al, al
        jne    SHORT loc_1_42E9
        or     dword ptr [esi + 0bh], 2000h
loc_1_42E9:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_42F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 24h
        lea    ecx, [esi + 2]
        mov    dx, cx
        out    dx, al
        mov    edi, 1
        lea    edx, [esi + 1]
loc_1_430C:
        in     al, dx
        dec    edi
        jne    SHORT loc_1_430C
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, cx
        mov    al, 0ch
        out    dx, al
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4330:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0e4h
        lea    esi, [edi + 2]
        mov    dx, si
        out    dx, al
        mov    ecx, 1
        lea    edx, [edi + 1]
loc_1_434C:
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_434C
        mov    dx, di
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, si
        mov    al, 0ch
        out    dx, al
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4370:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, si
        mov    al, 0ffh
        out    dx, al
        mov    ecx, 1
        lea    edi, [esi + 1]
loc_1_438A:
        mov    dx, di
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_438A
        lea    ecx, [esi + 2]
        mov    dx, cx
        mov    al, 4
        out    dx, al
        mov    ebx, 1
        nop
loc_1_43A0:
        mov    dx, di
        in     al, dx
        dec    ebx
        jne    SHORT loc_1_43A0
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    dx, cx
        mov    al, 0ch
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_43C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_44E0
        push   7fh
        push   0
        push   edi
        call   sub_1_4540
        mov    dx, di
        mov    al, 1
        out    dx, al
        lea    edx, [edi + 2]
        mov    al, 0ch
        out    dx, al
        push   edi
        call   sub_1_42F0
        cmp    al, 7dh
        jne    SHORT loc_1_43F7
        mov    esi, 2
        jmp    SHORT loc_1_441C
loc_1_43F7:
        push   edi
        call   sub_1_4330
        cmp    al, 7dh
        jne    SHORT loc_1_4408
        mov    esi, 102h
        jmp    SHORT loc_1_441C
loc_1_4408:
        push   edi
        call   sub_1_4370
        cmp    al, 7dh
        mov    esi, 202h
        je     SHORT loc_1_441C
        mov    esi, 1
loc_1_441C:
        push   0
        push   edi
        call   sub_1_4480
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_4430:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    bl, 20h
        mov    esi, dword ptr [ebp + 8]
loc_1_443D:
        push   ebx
        push   esi
        call   sub_1_4480
        cmp    edi, 7fh
        jge    SHORT loc_1_4476
loc_1_4449:
        push   edi
        push   ebx
        push   esi
        call   sub_1_4540
        push   esi
        call   sub_1_45B0
        cmp    al, 0f8h
        je     SHORT loc_1_4461
        inc    edi
        cmp    edi, 7fh
        jl     SHORT loc_1_4449
loc_1_4461:
        cmp    edi, 7fh
        jge    SHORT loc_1_4476
        inc    bl
        cmp    bl, 2eh
        jb     SHORT loc_1_443D
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_4476:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_1_4480:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0fh
        lea    esi, [edi + 2]
        mov    dx, si
        out    dx, al
        push   64h
        call   sub_1_04D0
        mov    bl, byte ptr [ebp + 0ch]
        mov    cl, bl
        or     cl, 10h
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   64h
        call   sub_1_04D0
        mov    dx, si
        mov    al, 4
        out    dx, al
        and    ebx, 0fh
        xor    edx, edx
        db  08Ah, 093h, 054h, 05Bh, 000h, 000h
        add    edx, edi
        db  08Ah, 083h, 044h, 05Bh, 000h, 000h
        out    dx, al
        push   64h
        call   sub_1_04D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_44E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0fh
        lea    esi, [edi + 2]
        mov    dx, si
        out    dx, al
        push   64h
        call   sub_1_04D0
        mov    bl, byte ptr [ebp + 0ch]
        mov    cl, bl
        or     cl, 0d0h
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   64h
        call   sub_1_04D0
        mov    dx, si
        mov    al, 0ch
        out    dx, al
        push   64h
        call   sub_1_04D0
        and    ebx, 0fh
        xor    ecx, ecx
        db  08Ah, 08Bh, 054h, 05Bh, 000h, 000h
        add    ecx, edi
        mov    dx, cx
        db  08Ah, 083h, 044h, 05Bh, 000h, 000h
        out    dx, al
        push   64h
        call   sub_1_04D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
sub_1_4540:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    al, byte ptr [ebp + 0ch]
        and    eax, 0fh
        xor    edx, edx
        db  08Ah, 088h, 034h, 05Bh, 000h, 000h
        db  08Ah, 098h, 044h, 05Bh, 000h, 000h
        db  08Ah, 090h, 054h, 05Bh, 000h, 000h
        add    edx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_4582
        mov    esi, 3eh
loc_1_456D:
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    esi
        jne    SHORT loc_1_456D
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_4582:
        mov    esi, 1fh
loc_1_4587:
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    esi
        jne    SHORT loc_1_4587
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_45B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [ecx + 1]
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    al, 78h
        shr    al, 3
        mov    byte ptr [ebp - 2], al
        mov    dx, cx
        mov    al, 80h
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 0f8h
        add    al, al
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 2]
        or     al, byte ptr [ebp - 1]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_45F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0ah
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        or     al, 0bh
        out    dx, al
        call   sub_1_04D0
        mov    dx, word ptr [esi]
        mov    al, 38h
        out    dx, al
        push   0ah
        call   sub_1_04D0
        mov    ecx, dword ptr [esi]
        add    ecx, 2
        mov    dx, cx
        mov    al, byte ptr [esi + 8]
        out    dx, al
        push   0ah
        call   sub_1_04D0
        mov    dx, word ptr [esi]
        sub    al, al
        out    dx, al
        push   0ah
        call   sub_1_04D0
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_465F
        mov    edi, 4
loc_1_4642:
        mov    dx, word ptr [esi]
        mov    al, 1
        out    dx, al
        push   0ah
        call   sub_1_04D0
        mov    dx, word ptr [esi]
        sub    al, al
        out    dx, al
        push   0ah
        call   sub_1_04D0
        dec    edi
        jne    SHORT loc_1_4642
loc_1_465F:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4670:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    al, al
        mov    edx, dword ptr [esi]
        out    dx, al
        add    edx, 2
        out    dx, al
        push   6
        call   sub_1_04D0
        mov    ecx, dword ptr [esi]
        add    ecx, 2
        mov    dx, cx
        mov    al, byte ptr [esi + 8]
        out    dx, al
        push   1f4h
        call   sub_1_04D0
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_46DE
        mov    edx, dword ptr [esi]
        add    dx, 2
loc_1_46AB:
        mov    eax, dword ptr [ebp + 0ch]
        sub    ecx, ecx
        movzx  edi, byte ptr [eax]
loc_1_46B3:
        mov    eax, edi
        shl    eax, cl
        and    eax, 80h
        cmp    eax, 1
        sbb    bl, bl
        and    bl, 2
        or     bl, byte ptr [esi + 8]
        mov    al, bl
        out    dx, al
        or     al, 8
        out    dx, al
        mov    al, bl
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jl     SHORT loc_1_46B3
        inc    dword ptr [ebp + 0ch]
        dec    dword ptr [ebp + 10h]
        jne    SHORT loc_1_46AB
loc_1_46DE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_46F0:
        push   ebp
        mov    ebp, esp
        push   0adch
        db  068h, 058h, 050h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4260
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4710:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   0
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_49B0
        push   0a5h
        push   13h
        push   esi
        call   sub_1_4A40
        push   13h
        push   esi
        call   sub_1_4AA0
        sub    al, 0a5h
        cmp    al, 1
        sbb    al, al
        inc    al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4750:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4B20
        push   0
        push   0
        push   esi
        call   sub_1_4A40
        push   esi
        call   sub_1_4780
        or     dword ptr [esi + 0bh], eax
        or     dword ptr [esi + 0bh], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4780:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   2
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_49B0
        push   0a5h
        push   13h
        push   esi
        call   sub_1_4A40
        push   esi
        call   sub_1_47F0
        cmp    al, 0a5h
        jne    SHORT loc_1_47B4
        mov    eax, 2
        pop    esi
        pop    ebp
        ret    4
loc_1_47B4:
        push   esi
        call   sub_1_4850
        cmp    al, 0a5h
        jne    SHORT loc_1_47C8
        mov    eax, 102h
        pop    esi
        pop    ebp
        ret    4
loc_1_47C8:
        push   esi
        call   sub_1_48A0
        cmp    al, 0a5h
        jne    SHORT loc_1_47E0
        mov    eax, 202h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_47E0:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
_LTEXT ends
        end
