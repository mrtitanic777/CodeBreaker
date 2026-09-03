; hpfend_lcode_01.asm -- HPFEND object 1 segment _LTEXT
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
        db  0E8h, 010h, 01Dh, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
HPFEND_Control:
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
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h
sub_1_03C9:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        cld
        rep outsd
        pop    esi
        ret    0ch
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch
        db  05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_0406:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        cld
        rep insd
        pop    edi
        ret    0ch
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
sub_1_0486:
        db  0CDh, 020h, 008h, 080h, 028h, 000h
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_04B2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
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
sub_1_0506:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_050C:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_0512:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_051E:
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
sub_1_054E:
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
sub_1_0584:
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
sub_1_058A:
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
sub_1_0590:
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
loc_1_0596:
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
sub_1_059C:
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
sub_1_05A2:
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
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 048h, 050h, 046h, 045h
        db  04Eh, 044h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 061h, 078h, 04Dh, 075h, 06Ch, 074h, 069h
        db  063h, 061h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0680:
        dec    ebp
        popad
        db  078h, 04Dh
        jne    SHORT loc_1_06F2
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_068D
loc_1_068D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], ch
        popad
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        db  000h
loc_1_069C:
        imul   ebp, dword ptr [edi + 61h], 65726464h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], ch
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_06B2
loc_1_06B2:
        add    byte ptr [eax], al
loc_1_06B4:
        imul   ebp, dword ptr [esi + 74h], 75727265h
        db  070h, 074h
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        jae    SHORT loc_1_0712
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        jae    SHORT loc_1_071E
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        jb     SHORT loc_1_0757
        dec    ebp
        popad
        db  070h, 070h
        inc    edx
        popad
        jae    SHORT loc_1_074D
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [ebp + 65h], cl
loc_1_06F2:
        insd
        outsd
        jb     SHORT loc_1_076F
        dec    ebp
        popad
        db  070h, 070h
        inc    edx
        popad
        jae    SHORT loc_1_0765
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [ecx + 64h], al
        popad
        jo     SHORT loc_1_0781
        db  065h, 072h, 054h
        db  079h, 070h
loc_1_0712:
        add    byte ptr gs:[ecx + 64h], al
        popad
        jo     SHORT loc_1_078D
        db  065h, 072h, 054h
        db  079h, 070h
loc_1_071E:
        add    byte ptr gs:[eax + 72h], dl
        imul   ebp, dword ptr [edi + 72h], 797469h
        add    byte ptr [eax], al
        add    byte ptr [eax + 72h], dl
        imul   ebp, dword ptr [edi + 72h], 797469h
        add    byte ptr [eax], al
        db  000h
loc_1_0738:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
loc_1_074D:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
loc_1_0757:
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
loc_1_0765:
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
loc_1_076F:
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
loc_1_0781:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
loc_1_078D:
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
loc_1_07B4:
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
loc_1_0810:
        dec    eax
        push   eax
        and    byte ptr [ecx], dh
        xor    byte ptr [edi], ch
        xor    dword ptr [eax], esi
        xor    byte ptr [esi + 47h], dl
        and    byte ptr [ecx + eax*2 + 4eh], cl
        and    byte ptr [ecx + 64h], al
        popad
        jo     SHORT loc_1_0899
        jb     NEAR PTR loc_1_0856
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 046h, 045h, 04Eh, 044h, 000h, 000h
loc_1_0838:
        dec    eax
        push   eax
        inc    esi
        inc    ebp
        dec    esi
        inc    esp
        add    byte ptr [eax], al
loc_1_0840:
        dec    eax
        push   eax
        das
        push   esi
        inc    edi
        and    byte ptr [ebp + 49h], al
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
loc_1_084C:
        dec    eax
        push   eax
        das
        push   esi
        inc    edi
        and    byte ptr [ebp + 49h], al
        push   ebx
        inc    ecx
loc_1_0856:
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        cmp    eax, 3f3f7830h
        aas
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [eax + 50h], cl
        das
        push   esi
        inc    edi
        and    byte ptr [eax], ah
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
loc_1_0874:
        dec    eax
        push   eax
        das
        push   esi
        inc    edi
        and    byte ptr [eax], ah
        dec    ecx
        push   ebx
        inc    ecx
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        cmp    eax, 3f3f7830h
        aas
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [esi + 4fh], cl
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
loc_1_0899:
        popad
        jo     SHORT loc_1_0911
        jb     NEAR PTR loc_1_08C0
        outsb
        outsd
        je     SHORT loc_1_08C4
        jo     SHORT loc_1_0918
        db  065h, 073h, 065h
        outsb
        je     SHORT loc_1_08AC
loc_1_08AC:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        jbe    SHORT loc_1_0918
        insb
        imul   esp, dword ptr [eax + 49h], 61204f2fh
loc_1_08C0:
        db  064h, 064h, 072h, 065h
loc_1_08C4:
        db  073h, 073h
        sub    byte ptr [ebp + 73h], ah
        sub    dword ptr [eax], eax
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        jbe    SHORT loc_1_0938
        insb
        imul   esp, dword ptr [eax + 69h], 7265746eh
        jb     SHORT loc_1_0957
        db  070h, 074h
        and    byte ptr [esi + 65h], dh
        arpl   word ptr [edi + ebp*2 + 72h], si
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        je     SHORT loc_1_095C
        db  072h, 072h
        db  075h, 070h
        db  074h, 020h
        jbe    SHORT loc_1_0964
        arpl   word ptr [edi + ebp*2 + 72h], si
        and    byte ptr [ebp + 69h], ch
        jae    SHORT loc_1_0975
        popad
        db  074h, 063h
        push   0
loc_1_0910:
        inc    ebp
loc_1_0911:
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
loc_1_0918:
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        outsb
        outsd
        db  074h, 020h
        db  073h, 075h
        jo     SHORT loc_1_0997
        outsd
        db  072h, 074h
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_0930:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_0938:
        db  074h, 06Fh
        db  072h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_1_09B2
        outsd
        outsb
        and    byte ptr [ebx + 6fh], ah
        jb     SHORT loc_1_09C2
        jne    SHORT loc_1_09C2
        db  074h, 065h
        add    byte ptr fs:[eax], al
loc_1_0957:
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
loc_1_095C:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebp
        inc    ebp
        push   eax
        push   edx
        dec    edi
loc_1_0964:
        dec    ebp
        and    byte ptr [edi + ebp*2 + 61h], ch
        and    byte ptr fs:[esi + 61h], ah
        db  069h, 06Ch, 065h, 064h, 000h, 000h, 000h, 000h
loc_1_0975:
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
loc_1_0997:
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_09A0:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        popad
        bound  esp, qword ptr [ebx + 64h]
        add    byte ptr gs:[eax], al
loc_1_09B2:
        add    byte ptr [eax], al
        push   ebp
        db  073h, 069h
        outsb
        and    byte ptr [bx + si], bh
        xor    byte ptr [edx], dh
        xor    esp, dword ptr cs:[eax]
        db  070h, 06Fh
loc_1_09C2:
        jb     SHORT loc_1_0A38
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebp
        db  073h, 069h
        outsb
        and    byte ptr [bx + si], bh
        xor    byte ptr [edx], dh
        xor    dword ptr cs:[edx], esi
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax], bh
        xor    byte ptr [edx], dh
        xor    esp, dword ptr cs:[eax]
        insd
        outsd
        db  064h, 065h, 000h
loc_1_09E8:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 65h], dh
        db  072h, 066h
        outsd
        db  072h, 06Dh
        and    byte ptr [ecx + eax*2 + 4eh], cl
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [ecx + 75h], ah
        db  074h, 06Fh
        sub    eax, 736e6573h
        add    byte ptr gs:[eax], al
        add    byte ptr [ecx + 75h], al
        db  074h, 06Fh
        sub    eax, 736e6573h
        and    byte ptr gs:[ecx + eax*2 + 4eh], cl
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 6556202eh
        jb     SHORT loc_1_0A99
        jns    NEAR PTR loc_1_0A53
        arpl   word ptr [ecx + 62h], sp
        insb
        db  069h
loc_1_0A38:
        outsb
        db  067h, 02Eh, 000h
loc_1_0A3C:
        push   esp
        db  072h, 061h
        imul   ebp, dword ptr [esi + 69h], 6620676eh
        popad
        imul   ebp, dword ptr [ebp + 64h], 6854202eh
        and    byte ptr gs:[ebx + 61h], ah
loc_1_0A53:
        bound  ebp, qword ptr [ebp + 20h]
        insd
        popad
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + ebp*2 + 73h], ah
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[ebp + 64h], si
        add    byte ptr cs:[eax], al
loc_1_0A74:
        push   esp
        db  072h, 061h
        imul   ebp, dword ptr [esi + 69h], 6620676eh
        popad
        imul   ebp, dword ptr [ebp + 64h], 6854202eh
        and    byte ptr gs:[ebx + 61h], ah
        bound  ebp, qword ptr [ebp + 20h]
        push   74207361h
        outsd
        outsd
        and    byte ptr [ebp + 75h], ch
loc_1_0A99:
        arpl   word ptr [eax + 20h], bp
        outsb
        outsd
        imul   esi, dword ptr [ebx + 65h], 5400002eh
        db  072h, 061h
        imul   ebp, dword ptr [esi + 69h], 6620676eh
        popad
        imul   ebp, dword ptr [ebp + 64h], 65756420h
        and    byte ptr [edi + ebp*2 + 20h], dh
        ja     SHORT loc_1_0B2F
        outsd
        outsb
        and    byte ptr [bp + 72h], ah
        popad
        insd
        and    byte ptr gs:[ecx + edi*2 + 70h], dh
        and    byte ptr gs:[edi + 72h], ch
        and    byte ptr [ebx + 65h], dh
        arpl   word ptr [ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 20h], 6c6f6976h
        popad
        je     SHORT loc_1_0B48
        outsd
        outsb
        add    byte ptr cs:[eax], al
loc_1_0AE4:
        push   ebp
        outsb
        jae    SHORT loc_1_0B5D
        jo     SHORT loc_1_0B5A
        outsd
        db  072h, 074h
        and    byte ptr fs:[eax + 72h], dh
        sub    eax, 656c6572h
        popad
        jae    SHORT loc_1_0B60
        and    byte ptr [eax + 50h], cl
        and    byte ptr [ecx + 64h], ah
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_0B27
        outsw
        jne    SHORT loc_1_0B79
        and    byte ptr cs:[eax + 6ch], dl
        popad
        jae    SHORT loc_1_0B79
        and    byte ptr [edx + 65h], dh
        db  074h, 075h
        db  072h, 06Eh
        and    byte ptr [edi + ebp*2 + 20h], dh
        dec    eax
        push   eax
        add    byte ptr cs:[eax], al
loc_1_0B24:
        dec    ebp
        insd
loc_1_0B27:
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        jo     SHORT loc_1_0B4F
loc_1_0B2F:
        outsb
        outsd
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        imul   sp, word ptr [edi + 75h], 6572h
        cmp    esp, dword ptr fs:[eax]
        db  075h, 073h
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
loc_1_0B48:
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        jae    SHORT loc_1_0BB5
loc_1_0B4F:
        db  065h, 072h, 073h
        add    byte ptr [eax], al
loc_1_0B54:
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_0BD3
loc_1_0B5A:
        and    byte ptr [ebp + 61h], ch
loc_1_0B5D:
        db  070h, 020h
        popad
loc_1_0B60:
        jb     NEAR PTR loc_1_0BC9
        jae    SHORT loc_1_0BD9
        and    byte ptr [edi + ebp*2 + 65h], ah
        jae    SHORT loc_1_0B8C
        outsb
        outsd
        db  074h, 020h
        insd
        popad
        je     SHORT loc_1_0BD7
        push   61646120h
loc_1_0B79:
        jo     SHORT loc_1_0BEF
        db  065h, 072h, 03Bh
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
loc_1_0B8C:
        outsb
        db  073h, 066h
        jb     NEAR PTR loc_1_0C05
        add    byte ptr [eax], al
loc_1_0B94:
        dec    ebp
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        db  070h, 020h
        jb     SHORT loc_1_0C02
        outsb
        and    byte ptr gs:[di + 75h], ch
        db  073h, 074h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [eax], bh
        xor    dword ptr [ecx], edi
        xor    ah, byte ptr [eax]
        bound  edi, qword ptr [ecx + 74h]
loc_1_0BB5:
        db  065h, 073h, 03Bh
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 066h
loc_1_0BC9:
        db  065h, 072h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD0:
        inc    ebx
        popad
        outsb
loc_1_0BD3:
        outsb
        outsd
        db  074h, 020h
loc_1_0BD7:
        insd
        popad
loc_1_0BD9:
        db  070h, 020h
        insd
        insd
        outsd
        jb     SHORT loc_1_0C5A
        and    byte ptr [ecx + 6eh], ch
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], ch
        db  072h, 06Eh
        insb
        cmp    esp, dword ptr [eax]
loc_1_0BEF:
        jne    SHORT loc_1_0C64
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        jae    SHORT loc_1_0C65
        db  065h, 072h, 073h
loc_1_0C02:
        add    byte ptr [eax], al
loc_1_0C04:
        dec    ebp
loc_1_0C05:
        insd
        outsd
        jb     SHORT loc_1_0C83
        and    byte ptr [ebp + 61h], ch
        db  070h, 020h
        arpl   word ptr [edi + 6eh], bp
        insb
        imul   esp, dword ptr [ebx + 74h], 7375203bh
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 066h
        jb     NEAR PTR loc_1_0C9F
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C30:
        inc    esi
        inc    ecx
        push   esp
        inc    ecx
        dec    esp
        cmp    ah, byte ptr [eax]
        inc    edx
        popad
        and    byte ptr fs:[eax + 2fh], cl
        push   edi
        and    byte ptr [ecx + 6eh], ch
        imul   esi, dword ptr [eax + 63h], 65646fh
        add    byte ptr [eax], al
        add    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        db  072h, 065h
        jae    SHORT loc_1_0CCE
loc_1_0C5A:
        popad
        db  072h, 074h
        add    byte ptr cs:[eax], al
loc_1_0C60:
        inc    ebp
        push   edx
        push   edx
        dec    edi
loc_1_0C64:
        push   edx
loc_1_0C65:
        cmp    ah, byte ptr [eax]
        dec    ecx
        insb
        insb
        popad
        insb
        and    byte ptr [ecx + 52h], cl
        push   ecx
        and    byte ptr [eax], ch
        xor    byte ptr [eax + 2eh], bh
        sub    dword ptr cs:[eax], eax
loc_1_0C80:
        inc    ebp
        push   edx
        push   edx
loc_1_0C83:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        insb
        insb
        popad
        insb
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [ecx + 64h], ah
        jb     NEAR PTR loc_1_0CFD
        jae    SHORT loc_1_0D0D
        and    byte ptr [eax], ch
        xor    byte ptr [eax + 2eh], bh
loc_1_0C9F:
        sub    dword ptr cs:[eax], eax
loc_1_0CA8:
        dec    esi
        dec    edi
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_0CD8
        outsb
        outsd
        je     SHORT loc_1_0CDC
        jo     SHORT loc_1_0D30
        jae    NEAR PTR loc_1_0D26
        outsb
        je     SHORT loc_1_0CC4
loc_1_0CC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0CC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0CCC:
        add    byte ptr [eax], al
loc_1_0CCE:
        add    byte ptr [eax], al
loc_1_0CD0:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
loc_1_0CD8:
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
loc_1_0CDC:
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0CE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_0CFD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D0D:
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
        db  000h
loc_1_0D26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D2E:
        add    byte ptr [eax], al
loc_1_0D30:
        add    byte ptr [eax], al
loc_1_0D32:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D36:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D3A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D3E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
