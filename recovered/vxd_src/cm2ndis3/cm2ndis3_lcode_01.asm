; cm2ndis3_lcode_01.asm -- CM2NDIS3 object 1 segment _LTEXT
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
        call   sub_1_1A90
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
CM2NDIS3_Control:
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
sub_1_04A4:
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
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 04Dh, 032h, 04Eh
        db  044h, 049h, 053h, 033h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0670:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        lea    ecx, [eax + 3f8h]
        cmp    eax, ecx
        jae    SHORT loc_1_069C
        mov    bl, byte ptr [ebp + 10h]
loc_1_0684:
        mov    dl, byte ptr [eax]
        cmp    dl, 0ffh
        je     SHORT loc_1_069C
        cmp    bl, dl
        je     SHORT loc_1_069C
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        lea    eax, [eax + edx*2 + 4]
        cmp    ecx, eax
        ja     SHORT loc_1_0684
loc_1_069C:
        cmp    byte ptr [eax], 0ffh
        jne    SHORT loc_1_06A3
        sub    eax, eax
loc_1_06A3:
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_06B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 51h]
        mov    al, 62h
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 7]
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_06D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 038h, 039h, 000h, 000h
        mov    ecx, dword ptr [edx + 5dh]
        mov    eax, dword ptr [edx + 61h]
        cmp    dword ptr [edx + 3ch], 2
        jne    SHORT loc_1_06F0
        add    eax, 0f08h
        jmp    SHORT loc_1_06F3
loc_1_06F0:
        lea    eax, [ecx + 18h]
loc_1_06F3:
        db  080h, 03Dh, 038h, 039h, 000h, 000h, 000h
        je     SHORT loc_1_0714
        lea    esp, [esp]
loc_1_0700:
        mov    bl, byte ptr [esi]
        cmp    bl, 20h
        je     SHORT loc_1_0714
        inc    esi
        cmp    byte ptr [eax], bl
        jne    SHORT loc_1_0727
        add    eax, 2
        cmp    byte ptr [esi], 0
        jne    SHORT loc_1_0700
loc_1_0714:
        test   byte ptr [edx + 45h], 10h
        mov    eax, dword ptr [edx + 41h]
        je     SHORT loc_1_0730
        add    eax, 10h
        mov    dword ptr [edx + 51h], eax
        mov    al, 41h
        jmp    SHORT loc_1_073E
loc_1_0727:
        sub    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_0730:
        mov    dword ptr [edx + 51h], eax
        add    eax, 10h
        mov    dword ptr [edx + 41h], eax
        mov    dword ptr [edx + 49h], eax
        mov    al, 61h
loc_1_073E:
        mov    byte ptr [ecx + 800h], al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 5dh]
        mov    ecx, dword ptr [esi + 61h]
        cmp    dword ptr [esi + 3ch], 2
        jne    SHORT loc_1_07B0
        mov    dx, word ptr [esi + 51h]
        sub    al, al
        out    dx, al
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [ebp - 2], al
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [ecx + eax], 70h
        inc    word ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx + 3], al
        mov    al, 1
        mov    dx, word ptr [esi + 51h]
        out    dx, al
        mov    al, 5
        out    dx, al
        mov    byte ptr [esi + 6dh], 0
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_07B0:
        mov    byte ptr [edi + 806h], 0d4h
        push   esi
        call   sub_1_08D0
        or     al, al
        jne    SHORT loc_1_07D0
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_07D0:
        push   30d40h
        call   sub_1_04D0
        test   byte ptr [edi + 806h], 1
        jne    SHORT loc_1_07F1
        mov    byte ptr [esi + 6dh], 0
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_07F1:
        mov    byte ptr [esi + 6dh], 1
        mov    byte ptr [edi + 806h], 55h
        push   30d40h
        call   sub_1_04D0
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_0810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 3ch], 2
        jne    SHORT loc_1_087A
        mov    dx, word ptr [esi + 51h]
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [esi + 61h]
        mov    al, byte ptr [eax + 0f36h]
        mov    byte ptr [esi + 79h], al
        mov    eax, dword ptr [esi + 61h]
        mov    al, byte ptr [eax + 0f38h]
        mov    byte ptr [esi + 7ah], al
        mov    eax, dword ptr [esi + 61h]
        mov    al, byte ptr [eax + 0f3ah]
        mov    byte ptr [esi + 7bh], al
        mov    eax, dword ptr [esi + 61h]
        mov    al, byte ptr [eax + 0f24h]
        mov    byte ptr [esi + 7ch], al
        mov    eax, dword ptr [esi + 61h]
        pop    edi
        mov    al, byte ptr [eax + 0f22h]
        mov    byte ptr [esi + 7dh], al
        mov    eax, dword ptr [esi + 61h]
        mov    al, byte ptr [eax + 0f20h]
        mov    byte ptr [esi + 7eh], al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_087A:
        mov    ecx, dword ptr [esi + 5dh]
        sub    edi, edi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, ebx
loc_1_0884:
        cmp    byte ptr [ecx], 0ffh
        je     SHORT loc_1_08B5
        push   89h
        push   ecx
        push   esi
        call   sub_1_0670
        cmp    byte ptr [eax + 8], bl
        jne    SHORT loc_1_08A6
        cmp    byte ptr [eax + 0ah], 80h
        jne    SHORT loc_1_08A6
        cmp    byte ptr [eax + 0ch], 0c7h
        je     SHORT loc_1_08B5
loc_1_08A6:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        lea    ecx, [eax + ecx*2 + 4]
        inc    edi
        cmp    edi, 0ah
        jl     SHORT loc_1_0884
loc_1_08B5:
        sub    ecx, ecx
loc_1_08B7:
        mov    dl, byte ptr [eax + ecx*2 + 8]
        mov    byte ptr [esi + ecx + 79h], dl
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_08B7
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_08D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 3ch], 3
        jne    SHORT loc_1_08F8
        mov    si, word ptr [ecx + 51h]
        mov    dx, si
        mov    al, 11h
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 62h
        out    dx, al
        lea    edx, [esi + 7]
        mov    al, 4
        out    dx, al
loc_1_08F8:
        mov    byte ptr [ecx + 6eh], 52h
        mov    eax, dword ptr [ecx + 3ch]
        cmp    eax, 1
        je     SHORT loc_1_0909
        cmp    eax, 2
        jne    SHORT loc_1_0917
loc_1_0909:
        cmp    byte ptr [ecx + 6dh], 0
        je     SHORT loc_1_0917
        lea    edi, [ecx + 6fh]
        mov    byte ptr [edi], 20h
        jmp    SHORT loc_1_091D
loc_1_0917:
        lea    edi, [ecx + 6fh]
        mov    byte ptr [edi], 0a0h
loc_1_091D:
        mov    si, word ptr [ecx + 49h]
        mov    al, byte ptr [ecx + 6eh]
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
        mov    dx, word ptr [ecx + 49h]
        out    dx, al
        inc    dx
        mov    si, word ptr [ecx + 49h]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 4]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 5]
        out    dx, al
        sub    bl, bl
        lea    ebx, [ebx]
loc_1_0960:
        mov    al, bl
        and    eax, 0ffff00ffh
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0ffffh
        mov    al, byte ptr [ecx + eax + 7fh]
        out    dx, al
        inc    bl
        cmp    bl, 6
        jb     SHORT loc_1_0960
        mov    al, byte ptr [edi]
        or     al, 4
        lea    edx, [esi + 7]
        out    dx, al
        mov    eax, dword ptr [ecx + 0a7h]
        test   al, 20h
        jne    SHORT loc_1_09B2
        test   al, 4
        je     SHORT loc_1_09B2
        sub    bl, bl
loc_1_0997:
        mov    dl, bl
        and    edx, 0ffff00ffh
        add    dx, si
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_0997
        jmp    SHORT loc_1_09D3
loc_1_09B2:
        sub    bl, bl
loc_1_09B4:
        mov    al, bl
        and    eax, 0ffff00ffh
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0ffffh
        mov    al, byte ptr [ecx + eax + 71h]
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_1_09B4
loc_1_09D3:
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
        lea    edx, [esi + 0dh]
        mov    al, 80h
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [ecx + 6eh]
        out    dx, al
        mov    bl, 0fah
loc_1_09F6:
        push   3e8h
        call   sub_1_04D0
        dec    bl
        jne    SHORT loc_1_09F6
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 49h]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0ffh
        je     SHORT loc_1_0A2D
        or     al, al
        jne    SHORT loc_1_0A64
loc_1_0A2D:
        mov    edx, dword ptr [esi + 49h]
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
        jne    SHORT loc_1_0A62
        mov    edx, dword ptr [esi + 49h]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0A62:
        sub    al, al
loc_1_0A64:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 2
        mov    si, word ptr [ecx + 49h]
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [ecx + 70h]
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 80h
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 81h
        out    dx, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    cx, word ptr [eax + 49h]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 2
        mov    edx, dword ptr [esi + 49h]
        add    dx, 5
        out    dx, al
        db  0BFh, 030h, 039h, 000h, 000h
        add    eax, 0
loc_1_0AE0:
        mov    ecx, dword ptr [esi + 49h]
        mov    ax, 3dh
        lea    edx, [ecx + 8]
        out    dx, ax
        push   1fh
        mov    eax, dword ptr [edi]
        push   eax
        add    ecx, 8
        push   ecx
        call   sub_1_03B4
        add    edi, 4
        db  081h, 0FFh, 038h, 039h, 000h, 000h
        jb     SHORT loc_1_0AE0
        push   3e8h
        call   sub_1_04D0
        mov    ecx, dword ptr [esi + 49h]
        add    cx, 0ah
        mov    dx, cx
        mov    al, 82h
        out    dx, al
        push   1f4h
        call   sub_1_04D0
        sub    edi, edi
        mov    ebx, 40h
loc_1_0B30:
        mov    edx, dword ptr [esi + 49h]
        add    dx, 5
        in     al, dx
        test   bl, al
        jne    SHORT loc_1_0B54
        push   2
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        inc    edi
        cmp    edi, 2
        jb     SHORT loc_1_0B30
loc_1_0B54:
        sub    edi, 2
        cmp    edi, 1
        sbb    cl, cl
        neg    cl
        mov    dx, word ptr [esi + 49h]
        in     al, dx
        mov    dx, word ptr [esi + 49h]
        out    dx, al
        inc    dx
        mov    si, word ptr [esi + 49h]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 5]
        sub    al, al
        out    dx, al
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0AA0
        push   esi
        call   sub_1_08D0
        push   esi
        call   sub_1_0A70
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
sub_1_0BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_0BFB
loc_1_0BC2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0BD0:
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
        je     SHORT loc_1_0BF2
        xor    eax, 4c11db7h
loc_1_0BF2:
        dec    esi
        jne    SHORT loc_1_0BD0
        inc    edx
        cmp    dword ptr [ebp + 0ch], edx
        ja     SHORT loc_1_0BC2
loc_1_0BFB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C10:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0BB0
        sub    ecx, ecx
        mov    edx, 1
loc_1_0C25:
        add    ecx, ecx
        test   eax, edx
        je     SHORT loc_1_0C2E
        or     ecx, 1
loc_1_0C2E:
        add    edx, edx
        cmp    edx, 40h
        jb     SHORT loc_1_0C25
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
sub_1_0C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
loc_1_0C69:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 71h], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_1_0C69
        db  068h, 050h, 038h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0D0h, 0FAh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_1_0CC7
        sub    edi, edi
loc_1_0C98:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 050h, 038h, 000h, 000h
        push   eax
        call   sub_1_0C10
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 71h], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_1_0C98
loc_1_0CC7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0CD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 49h]
        mov    al, byte ptr [esi + 6eh]
        or     al, 80h
        lea    edx, [edi + 6]
        out    dx, al
        lea    ebx, [edi + 7]
        mov    al, byte ptr [esi + 6fh]
        or     al, 4
        mov    dx, bx
        out    dx, al
        sub    ecx, ecx
loc_1_0CF4:
        lea    edx, [edi + ecx]
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     SHORT loc_1_0CF4
        mov    al, byte ptr [esi + 6fh]
        or     al, 8
        mov    dx, bx
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 6eh]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0D20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 49h]
        mov    al, byte ptr [esi + 6eh]
        or     al, 80h
        lea    edx, [edi + 6]
        out    dx, al
        lea    ecx, [edi + 7]
        mov    al, byte ptr [esi + 6fh]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0D44:
        lea    edx, [edi + ebx]
        add    dx, 8
        mov    al, byte ptr [esi + ebx + 71h]
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_1_0D44
        mov    al, byte ptr [esi + 6fh]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 6eh]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_0D70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 49h]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 6f2h], 0
        jne    NEAR PTR loc_1_0EE6
        mov    byte ptr [esi + 6f2h], 1
        inc    dword ptr [esi + 8dh]
        sub    edi, edi
        sub    ebx, ebx
loc_1_0DBA:
        mov    dx, word ptr [esi + 49h]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 49h]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, bl
        jne    SHORT loc_1_0DD4
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_0E4C
loc_1_0DD4:
        test   byte ptr [ebp - 2], 81h
        je     SHORT loc_1_0DF2
        test   byte ptr [ebp - 2], 1
        je     SHORT loc_1_0DE6
        inc    dword ptr [esi + 6c9h]
loc_1_0DE6:
        push   esi
        call   sub_1_0EF0
        push   esi
        call   sub_1_1790
loc_1_0DF2:
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_0E00
        cmp    byte ptr [esi + 0a5h], bl
        je     SHORT loc_1_0E42
loc_1_0E00:
        mov    dx, word ptr [esi + 49h]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dword ptr [esi + 91h], ebx
        cmp    byte ptr [esi + 0a5h], bl
        je     SHORT loc_1_0E25
        mov    byte ptr [esi + 0a6h], 2
        mov    byte ptr [esi + 0a5h], bl
        jmp    SHORT loc_1_0E2E
loc_1_0E25:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0a6h], al
loc_1_0E2E:
        mov    byte ptr [esi + 717h], bl
        cmp    dword ptr [esi + 0a1h], ebx
        je     SHORT loc_1_0E42
        push   esi
        call   sub_1_1110
loc_1_0E42:
        inc    edi
        cmp    edi, 8
        jl     NEAR PTR loc_1_0DBA
loc_1_0E4C:
        cmp    dword ptr [esi + 6dah], 0
        je     SHORT loc_1_0E6D
loc_1_0E55:
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_0E6D
        push   esi
        call   sub_1_0EF0
        cmp    dword ptr [esi + 6dah], 0
        jne    SHORT loc_1_0E55
loc_1_0E6D:
        mov    cx, word ptr [esi + 49h]
        mov    byte ptr [esi + 6f2h], 0
        lea    edx, [ecx + 2]
        mov    al, 80h
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 81h
        out    dx, al
        mov    eax, dword ptr [esi + 8dh]
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        jne    SHORT loc_1_0EC5
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_0EAE
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_0EAE
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_0EC5
loc_1_0EAE:
        push   1
        add    esi, 6f3h
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0EC5:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_0EE6
        cmp    dword ptr [esi + 0a1h], 0
        jne    SHORT loc_1_0EE6
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_0EE6
        push   esi
        call   sub_1_1790
loc_1_0EE6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_1_0EF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        mov    esi, dword ptr [ebp + 8]
loc_1_0F04:
        cmp    byte ptr [esi + 6cdh], 0
        jne    NEAR PTR loc_1_108A
        mov    cx, word ptr [esi + 49h]
        mov    al, 81h
        lea    edx, [ecx + 1]
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        test   al, 40h
        jne    NEAR PTR loc_1_108A
        inc    dword ptr [esi + 91h]
        mov    dword ptr [esi + 6e2h], 0
        push   2
        lea    eax, [esi + 6adh]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    ax, word ptr [esi + 6afh]
        mov    word ptr [esi + 0abh], ax
        cmp    ax, 6
        jb     NEAR PTR loc_1_101A
        mov    edi, dword ptr [esi + 6d6h]
        add    di, 0eh
        cmp    di, ax
        jbe    SHORT loc_1_0F76
        mov    di, ax
loc_1_0F76:
        mov    bl, 1
        test   byte ptr [esi + 0a7h], 28h
        jne    SHORT loc_1_0FF0
        push   3
        lea    eax, [esi + 0adh]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0abh], 6
        sub    di, 6
        cmp    dword ptr [esi + 0afh], -1
        jne    SHORT loc_1_0FC0
        cmp    byte ptr [esi + 0adh], 0ffh
        jne    SHORT loc_1_0FC0
        cmp    byte ptr [esi + 0aeh], 0ffh
        jne    SHORT loc_1_0FC0
        jmp    SHORT loc_1_101A
        db  090h
loc_1_0FC0:
        mov    eax, edi
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0b3h]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0abh], di
        jmp    SHORT loc_1_101C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0FF0:
        mov    eax, edi
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0adh]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0abh], di
        jmp    SHORT loc_1_101C
loc_1_101A:
        sub    bl, bl
loc_1_101C:
        or     bl, bl
        je     SHORT loc_1_1035
        push   esi
        call   sub_1_13B0
        cmp    eax, 3
        je     SHORT loc_1_1035
        inc    dword ptr [esi + 6b9h]
        mov    byte ptr [ebp - 2], 1
loc_1_1035:
        mov    ax, word ptr [esi + 0abh]
        or     ax, ax
        je     SHORT loc_1_106F
        cmp    ax, 8
        ja     SHORT loc_1_1065
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        jmp    SHORT loc_1_106F
loc_1_1065:
        mov    edx, dword ptr [esi + 49h]
        add    dx, 0eh
        mov    al, 4
        out    dx, al
loc_1_106F:
        cmp    dword ptr [esi + 91h], 5
        jbe    NEAR PTR loc_1_0F04
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 91h], 0
loc_1_108A:
        cmp    dword ptr [esi + 6dah], 0
        je     SHORT loc_1_10D5
loc_1_1093:
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_10DE
        mov    ebx, dword ptr [esi + 6dah]
        push   ebx
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 6dah], eax
        mov    dword ptr [esi + 6e2h], ebx
        mov    edi, dword ptr [ebx + 28h]
        push   esi
        call   sub_1_1210
        push   0
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        cmp    dword ptr [esi + 6dah], 0
        jne    SHORT loc_1_1093
loc_1_10D5:
        cmp    byte ptr [esi + 6cdh], 0
        je     SHORT loc_1_10EA
loc_1_10DE:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_10EA:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_10F9
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_0290
loc_1_10F9:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0a1h]
        or     ecx, ecx
        jne    SHORT loc_1_1144
        push   2
        push   6
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1144:
        mov    al, byte ptr [esi + 0a6h]
        test   al, 80h
        je     SHORT loc_1_1183
        test   al, 4
        je     SHORT loc_1_117B
        mov    edx, dword ptr [esi + 49h]
        add    dx, 4
        in     al, dx
        and    eax, 0ffh
        and    eax, 0fffffff0h
        cmp    eax, 10h
        jne    SHORT loc_1_1175
        inc    dword ptr [esi + 6c1h]
        inc    dword ptr [esi + 6b5h]
        jmp    SHORT loc_1_1189
loc_1_1175:
        inc    dword ptr [esi + 6c5h]
loc_1_117B:
        inc    dword ptr [esi + 6b5h]
        jmp    SHORT loc_1_1189
loc_1_1183:
        inc    dword ptr [esi + 6bdh]
loc_1_1189:
        cmp    byte ptr [ecx + 2ch], 0
        jne    SHORT loc_1_11B2
        mov    al, byte ptr [esi + 0a6h]
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
        jmp    SHORT loc_1_11D9
loc_1_11B2:
        cmp    dword ptr [esi + 6dah], 0
        jne    SHORT loc_1_11C3
        mov    dword ptr [esi + 6dah], ecx
        jmp    SHORT loc_1_11CC
loc_1_11C3:
        mov    eax, dword ptr [esi + 6deh]
        mov    dword ptr [eax + 24h], ecx
loc_1_11CC:
        mov    dword ptr [esi + 6deh], ecx
        mov    dword ptr [ecx + 24h], 0
loc_1_11D9:
        mov    dword ptr [esi + 0a1h], 0
        cmp    byte ptr [esi + 6cdh], 0
        je     SHORT loc_1_1200
        push   2
        push   esi
        call   sub_1_3410
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1200:
        push   esi
        call   sub_1_1790
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1210:
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
        jae    SHORT loc_1_1240
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1240:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 6d6h]
        add    edi, 0eh
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_1254
        mov    edi, dword ptr [ebp - 4]
loc_1_1254:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0adh]
        push   eax
        call   sub_1_12D0
        cmp    edi, 0eh
        jae    SHORT loc_1_1290
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0adh]
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
loc_1_1290:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        sub    edi, 0eh
        push   edi
        lea    eax, [esi + 0bbh]
        push   eax
        push   0eh
        lea    eax, [esi + 0adh]
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
sub_1_12D0:
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
        je     SHORT loc_1_1324
loc_1_12F5:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_1324
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_12F5
loc_1_1324:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1335
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1335:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0ch], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_1344:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_1353
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_1356
loc_1_1353:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1356:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05BC
        add    esi, ebx
        cmp    esi, edi
        je     SHORT loc_1_1392
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_139D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        jmp    SHORT loc_1_1344
loc_1_1392:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_139D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_13B0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 6afh]
        cmp    eax, 5eah
        jbe    SHORT loc_1_13D0
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_13D0:
        mov    dword ptr [edx + 6b1h], eax
        mov    ecx, dword ptr [edx + 6d6h]
        add    ecx, 0eh
        cmp    ecx, eax
        jbe    SHORT loc_1_13E5
        mov    ecx, eax
loc_1_13E5:
        cmp    ecx, 0eh
        jae    SHORT loc_1_1409
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 0adh]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 24h]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1409:
        sub    eax, 0eh
        push   eax
        sub    ecx, 0eh
        push   ecx
        lea    eax, [edx + 0bbh]
        push   eax
        push   0eh
        lea    eax, [edx + 0adh]
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
loc_1_1440:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 6e2h], 0
        je     NEAR PTR loc_1_14E7
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
        je     SHORT loc_1_14BA
loc_1_147A:
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
        mov    eax, dword ptr [esi + 6e2h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_12D0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_14BA
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_147A
loc_1_14BA:
        cmp    edi, ebx
        jbe    SHORT loc_1_14D5
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_14D5:
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
loc_1_14E7:
        mov    ax, word ptr [esi + 0abh]
        or     ax, ax
        je     SHORT loc_1_1522
        and    eax, 0ffffh
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        mov    eax, dword ptr [esi + 6d6h]
        add    eax, esi
        add    eax, 0bbh
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    word ptr [esi + 0abh], 0
loc_1_1522:
        mov    eax, dword ptr [esi + 6b1h]
        mov    ecx, dword ptr [ebp + 14h]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    SHORT loc_1_1557
        cmp    edi, eax
        jbe    SHORT loc_1_1550
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
loc_1_1550:
        sub    eax, edi
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_1_155A
loc_1_1557:
        mov    dword ptr [ebp - 18h], ecx
loc_1_155A:
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
        je     SHORT loc_1_15DB
loc_1_158A:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     SHORT loc_1_1597
        mov    ebx, dword ptr [ebp - 8]
loc_1_1597:
        push   ebx
        lea    eax, [edi + esi]
        add    eax, 0adh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05BC
        add    edi, ebx
        sub    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_15DB
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_15DB
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_158A
loc_1_15DB:
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
loc_1_1600:
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
        je     SHORT loc_1_1630
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1630:
        cmp    byte ptr [esi + 6cdh], 0
        je     SHORT loc_1_1647
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1647:
        inc    dword ptr [ebx + 18h]
        inc    dword ptr [esi + 8dh]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], ebx
        lea    eax, [ebp - 0ch]
        push   0
        push   eax
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        call   sub_1_0500
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0524
        mov    ecx, dword ptr [esi + 81h]
        lea    edx, [esi + 7fh]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    ecx, eax
        jae    SHORT loc_1_16A0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_16D7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16A0:
        jbe    SHORT loc_1_16B0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_16D7
        db  005h, 000h, 000h, 000h, 000h
loc_1_16B0:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    SHORT loc_1_16C7
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_16D7
loc_1_16C7:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_1_16D7
        mov    dword ptr [ebp - 4], 0
loc_1_16D7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_16F7
        test   byte ptr [ebx + 1dh], 2
        je     SHORT loc_1_16E5
        jmp    SHORT loc_1_16F7
loc_1_16E5:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_0330
        mov    byte ptr [edi + 8], al
        jmp    SHORT loc_1_16FB
loc_1_16F7:
        mov    byte ptr [edi + 8], 0
loc_1_16FB:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_1712
        mov    eax, dword ptr [esi + 9dh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_171B
loc_1_1712:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 99h], ecx
loc_1_171B:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 9dh], ecx
        mov    eax, dword ptr [esi + 8dh]
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        jne    SHORT loc_1_1760
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_174E
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_174E
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_1760
loc_1_174E:
        push   1
        add    esi, 6f3h
        push   esi
        call   sub_1_04B2
        jmp    SHORT loc_1_1781
        db  08Bh, 0FFh
loc_1_1760:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_1781
        cmp    dword ptr [esi + 0a1h], 0
        jne    SHORT loc_1_1781
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_1781
        push   esi
        call   sub_1_1790
loc_1_1781:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_1790:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0a1h], 0
        jne    NEAR PTR loc_1_1936
        mov    di, word ptr [ebp - 4]
        sub    ebx, ebx
        nop
loc_1_17B0:
        mov    eax, dword ptr [esi + 99h]
        cmp    eax, ebx
        je     NEAR PTR loc_1_1936
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esi + 99h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 0a1h], eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 14h], 3ch
        mov    eax, 3ch
        jb     SHORT loc_1_17F4
        mov    eax, dword ptr [ebp - 14h]
loc_1_17F4:
        mov    edx, dword ptr [esi + 49h]
        add    dx, 8
        cmp    dword ptr [ebp - 8], ebx
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        je     NEAR PTR loc_1_189B
loc_1_1809:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0ch], ebx
        jne    SHORT loc_1_1821
        jmp    SHORT loc_1_1885
loc_1_1821:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_1853
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        inc    dword ptr [ebp - 10h]
        shl    ax, 8
        dec    dword ptr [ebp - 0ch]
        or     di, ax
        mov    edx, dword ptr [esi + 49h]
        add    dx, 8
        cmp    dword ptr [ebp - 0ch], ebx
        mov    ax, di
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        jne    SHORT loc_1_1853
        jmp    SHORT loc_1_1885
loc_1_1853:
        cmp    dword ptr [ebp - 0ch], 1
        jbe    SHORT loc_1_186F
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_186F:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_1885
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        movzx  di, byte ptr [eax + ecx - 1]
        mov    byte ptr [ebp - 1], 1
loc_1_1885:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 8], ebx
        jne    NEAR PTR loc_1_1809
loc_1_189B:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_18B4
        or     di, 2000h
        mov    edx, dword ptr [esi + 49h]
        inc    dword ptr [ebp - 14h]
        add    dx, 8
        mov    ax, di
        out    dx, ax
loc_1_18B4:
        cmp    dword ptr [ebp - 14h], 3ch
        jae    SHORT loc_1_18D6
        mov    eax, 3ch
        sub    eax, dword ptr [ebp - 14h]
        shr    eax, 1
        push   eax
        db  068h, 040h, 039h, 000h, 000h
        mov    eax, dword ptr [esi + 49h]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_18D6:
        test   byte ptr [esi + 0a7h], 20h
        je     SHORT loc_1_18E5
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 2ch], bl
loc_1_18E5:
        cmp    byte ptr [esi + 6cdh], bl
        jne    SHORT loc_1_1910
        mov    edx, dword ptr [esi + 49h]
        add    dx, 0ah
        mov    al, 81h
        out    dx, al
        cmp    dword ptr [esi + 0a1h], ebx
        je     NEAR PTR loc_1_17B0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1910:
        mov    eax, dword ptr [ebp - 18h]
        push   0
        mov    edi, dword ptr [eax + 28h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 0a1h], 0
        push   3
        push   esi
        call   sub_1_3410
        dec    dword ptr [edi + 18h]
loc_1_1936:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1940:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 717h], 0
        je     SHORT loc_1_1990
        cmp    dword ptr [esi + 0a1h], 0
        je     SHORT loc_1_19A0
        mov    byte ptr [esi + 0a5h], 1
        mov    byte ptr [esi + 717h], 0
        push   7
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 10h
        push   esi
        call   sub_1_0AA0
        push   esi
        call   sub_1_0A70
        jmp    SHORT loc_1_19A0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1990:
        cmp    dword ptr [esi + 0a1h], 0
        je     SHORT loc_1_19A0
        mov    byte ptr [esi + 717h], 1
loc_1_19A0:
        push   7d0h
        add    esi, 6ffh
        push   esi
        call   sub_1_04B2
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [esi + 51h]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi + 40h], 0
        jne    SHORT loc_1_1A33
        or     al, al
        jne    SHORT loc_1_1A02
        mov    eax, dword ptr [esi + 724h]
        shl    eax, 2
        add    esi, 718h
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        push   eax
        push   esi
        call   sub_1_04B2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1A02:
        mov    byte ptr [esi + 40h], 1
        mov    dx, word ptr [esi + 51h]
        mov    al, 10h
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 40h
        je     SHORT loc_1_1A33
        mov    edx, dword ptr [esi + 51h]
        add    dx, 9
        sub    al, al
        out    dx, al
        push   32h
        call   sub_1_04D0
        mov    ecx, dword ptr [esi + 51h]
        add    cx, 9
        mov    dx, cx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_1A33:
        mov    edx, dword ptr [esi + 51h]
        inc    dx
        in     al, dx
        test   al, 8
        je     SHORT loc_1_1A59
        push   esi
        call   sub_1_0D70
        or     al, al
        je     SHORT loc_1_1A59
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0D90
loc_1_1A59:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_1A75
        mov    edx, dword ptr [esi + 51h]
        add    dx, 0ah
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    byte ptr [esi + 40h], 0
        mov    dx, word ptr [esi + 51h]
        mov    al, 11h
        out    dx, al
loc_1_1A75:
        mov    eax, dword ptr [esi + 724h]
        push   eax
        add    esi, 718h
        push   esi
        call   sub_1_04B2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_1_1A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        mov    word ptr [ebp - 14h], 9
        mov    word ptr [ebp - 12h], 9
        db  0C7h, 045h, 0F0h, 098h, 03Ah, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        db  0A1h, 0B0h, 039h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_1AE1
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1AE1:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 54h], 3
        mov    byte ptr [ebp - 53h], 0
        mov    dword ptr [ebp - 52h], 0
        db  0C7h, 045h, 0B2h, 0A0h, 022h, 000h, 000h
        db  0C7h, 045h, 0B6h, 0D0h, 024h, 000h, 000h
        db  0C7h, 045h, 0BAh, 000h, 016h, 000h, 000h
        db  0C7h, 045h, 0BEh, 040h, 014h, 000h, 000h
        db  0C7h, 045h, 0C2h, 0D0h, 030h, 000h, 000h
        db  0C7h, 045h, 0C6h, 090h, 026h, 000h, 000h
        db  0C7h, 045h, 0CAh, 040h, 02Fh, 000h, 000h
        db  0C7h, 045h, 0CEh, 020h, 030h, 000h, 000h
        db  0C7h, 045h, 0D2h, 090h, 01Bh, 000h, 000h
        db  0C7h, 045h, 0D6h, 050h, 030h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 26h], eax
        lea    eax, [ebp - 54h]
        mov    dword ptr [ebp - 22h], ecx
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
        jne    SHORT loc_1_1B76
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1B76:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_1B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0c8h
        push   esi
        push   edi
        mov    word ptr [ebp - 50h], 0ch
        mov    word ptr [ebp - 4eh], 0ch
        db  0C7h, 045h, 0B4h, 0D0h, 03Ah, 000h, 000h
        mov    word ptr [ebp - 40h], 10h
        mov    word ptr [ebp - 3eh], 10h
        db  0C7h, 045h, 0C4h, 034h, 03Bh, 000h, 000h
        mov    word ptr [ebp - 48h], 9
        mov    word ptr [ebp - 46h], 9
        db  0C7h, 045h, 0BCh, 050h, 03Bh, 000h, 000h
        mov    word ptr [ebp - 38h], 15h
        mov    word ptr [ebp - 36h], 15h
        db  0C7h, 045h, 0CCh, 074h, 03Bh, 000h, 000h
        mov    word ptr [ebp - 30h], 10h
        mov    word ptr [ebp - 2eh], 10h
        db  0C7h, 045h, 0D4h, 0B4h, 03Bh, 000h, 000h
        db  0A1h, 0B0h, 039h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   728h
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_1C20
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_1_1C20:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 1cah
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebp - 0ch]
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1DEF
        push   0
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1DE6
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 3ch]
        cmp    eax, 1
        je     SHORT loc_1_1C99
        cmp    eax, 2
        je     SHORT loc_1_1C99
        cmp    eax, 3
        jne    NEAR PTR loc_1_1DE6
loc_1_1C99:
        cmp    eax, 3
        jne    NEAR PTR loc_1_1DAC
        mov    word ptr [ebp - 0c8h], 0
        lea    edi, [ebp - 0c6h]
        sub    eax, eax
        mov    ecx, 7
        rep stosd
        stosw
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   20h
        lea    eax, [ebp - 0c8h]
        push   eax
        push   2
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_04A4
        add    esp, 1ch
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1DE6
        mov    ecx, dword ptr [ebp - 0c4h]
        and    ecx, 0ffffh
        xor    eax, eax
        mov    ax, word ptr [ebp - 0c2h]
        sub    eax, ecx
        inc    eax
        cmp    eax, 8
        jne    SHORT loc_1_1D09
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 45h], ecx
loc_1_1D09:
        mov    ecx, dword ptr [ebp - 0b4h]
        and    ecx, 0ffffh
        xor    eax, eax
        mov    ax, word ptr [ebp - 0b2h]
        sub    eax, ecx
        inc    eax
        cmp    eax, 20h
        jne    SHORT loc_1_1D2C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 41h], ecx
loc_1_1D2C:
        mov    word ptr [ebp - 0a8h], 0
        lea    edi, [ebp - 0a6h]
        sub    eax, eax
        mov    ecx, 7
        rep stosd
        stosw
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   20h
        lea    eax, [ebp - 0a8h]
        push   eax
        push   1
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_04A4
        add    esp, 1ch
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1DE6
        mov    eax, dword ptr [ebp - 0a0h]
        sub    eax, dword ptr [ebp - 0a4h]
        inc    eax
        cmp    eax, 1000h
        jne    SHORT loc_1_1D8C
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0a4h]
        mov    dword ptr [ecx + 55h], eax
loc_1_1D8C:
        mov    eax, dword ptr [ebp - 90h]
        sub    eax, dword ptr [ebp - 94h]
        inc    eax
        cmp    eax, 4000h
        jne    SHORT loc_1_1DAC
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 94h]
        mov    dword ptr [ecx + 59h], eax
loc_1_1DAC:
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1E10
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1E10
loc_1_1DE6:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0560
loc_1_1DEF:
        push   0
        push   728h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04DC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_1E10:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 65h], eax
        lea    eax, [ebp - 38h]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1E50
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 95h], eax
        jmp    SHORT loc_1_1E5D
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1E50:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 95h], 10h
loc_1_1E5D:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1E99
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_1_1E99
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, ecx
        add    edx, 7fh
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_1_1E99:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1EC5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 724h], 5
        jmp    SHORT loc_1_1ED4
loc_1_1EC5:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 724h], eax
loc_1_1ED4:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0560
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1FB0
        or     eax, eax
        jne    NEAR PTR loc_1_1DEF
        sub    eax, eax
        sub    ecx, ecx
        cmp    ecx, 7
        je     SHORT loc_1_1F15
        cmp    eax, 8
        je     SHORT loc_1_1F39
        cmp    eax, 9
        je     SHORT loc_1_1F60
        cmp    eax, 0ah
        je     SHORT loc_1_1F84
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F15:
        push   7
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000003h
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F39:
        push   8
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000005h
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_1F60:
        push   9
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa00000ch
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F84:
        push   0ah
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa00000ah
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0B0h, 039h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   26h
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    NEAR PTR loc_1_2213
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
        mov    eax, dword ptr [esi + 41h]
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [esi + 8]
        mov    dword ptr [ecx + 1ah], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1eh], 20h
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 49h]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
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
        call   sub_1_04DC
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2213
        mov    dword ptr [esi + 6f3h], 0
        db  0C7h, 086h, 0F7h, 006h, 000h, 000h, 020h, 036h, 000h, 000h
        mov    dword ptr [esi + 6fbh], esi
        push   1000h
        mov    eax, dword ptr [esi + 55h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 5dh]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05A2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2091
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_21FD
loc_1_2091:
        push   esi
        call   sub_1_06D0
        or     al, al
        jne    SHORT loc_1_20B9
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_21FD
loc_1_20B9:
        push   esi
        call   sub_1_0810
        lea    ecx, [esi + 7fh]
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_2108
        cmp    byte ptr [esi + 80h], 0
        jne    SHORT loc_1_2108
        cmp    byte ptr [esi + 81h], 0
        jne    SHORT loc_1_2108
        cmp    byte ptr [esi + 82h], 0
        jne    SHORT loc_1_2108
        cmp    byte ptr [esi + 83h], 0
        jne    SHORT loc_1_2108
        cmp    byte ptr [esi + 84h], 0
        jne    SHORT loc_1_2108
        mov    eax, dword ptr [esi + 79h]
        lea    edx, [esi + 79h]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_2108:
        lea    ebx, [esi + 24h]
        push   ebx
        push   esi
        push   ecx
        db  068h, 010h, 036h, 000h, 000h
        db  068h, 090h, 035h, 000h, 000h
        db  068h, 010h, 035h, 000h, 000h
        mov    eax, dword ptr [esi + 95h]
        push   eax
        db  0E8h, 0D7h, 0DEh, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_2150
        push   3
        push   2
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_21FD
        db  090h
loc_1_2150:
        mov    eax, dword ptr [esi + 3ch]
        cmp    eax, 1
        je     SHORT loc_1_215D
        cmp    eax, 2
        jne    SHORT loc_1_2163
loc_1_215D:
        push   esi
        call   sub_1_0750
loc_1_2163:
        push   esi
        call   sub_1_08D0
        or     al, al
        jne    SHORT loc_1_2190
        mov    dword ptr [ebp - 4], 0c0010011h
        push   1
        push   2
        push   2
        push   0aa000003h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        jmp    SHORT loc_1_21F5
        db  08Dh, 064h, 024h, 000h
loc_1_2190:
        push   esi
        call   sub_1_0AC0
        or     al, al
        jne    SHORT loc_1_21B9
        mov    dword ptr [ebp - 4], 0c0010011h
        push   1
        push   2
        push   2
        push   0aa000003h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        jmp    SHORT loc_1_21F5
loc_1_21B9:
        push   1
        push   1
        mov    eax, dword ptr [esi + 65h]
        push   eax
        push   eax
        db  068h, 090h, 00Dh, 000h, 000h
        push   esi
        db  068h, 070h, 00Dh, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04BE
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2220
        push   0
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0ch
loc_1_21F5:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 023h, 0DFh, 0FEh, 0FFh
loc_1_21FD:
        push   esi
        call   sub_1_06B0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0438
loc_1_2213:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_1_2220:
        mov    dword ptr [esi + 70bh], 0
        db  0C7h, 086h, 00Fh, 007h, 000h, 000h, 090h, 00Dh, 000h, 000h
        lea    eax, [esi + 6ffh]
        mov    dword ptr [esi + 713h], esi
        mov    dword ptr [eax], 0
        db  0C7h, 086h, 003h, 007h, 000h, 000h, 040h, 019h, 000h, 000h
        mov    dword ptr [esi + 707h], esi
        push   7d0h
        push   eax
        call   sub_1_04B2
        lea    ecx, [esi + 718h]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esi + 724h]
        shl    eax, 2
        mov    dword ptr [esi + 720h], esi
        lea    eax, [eax + eax*4]
        db  0C7h, 086h, 01Ch, 007h, 000h, 000h, 0C0h, 019h, 000h, 000h
        lea    eax, [eax + eax*4]
        push   eax
        push   ecx
        call   sub_1_04B2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_22A0:
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
loc_1_22C3:
        or     edx, edx
        je     SHORT loc_1_22D2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_22C3
        mov    dword ptr [ecx], edx
loc_1_22D2:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_22E4
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_22E4:
        db  0A1h, 0B0h, 039h, 000h, 000h
        push   eax
        push   0
        push   71h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D6
        or     eax, eax
        je     SHORT loc_1_2322
        push   4
        push   1
        push   2
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2322:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 8dh]
        cmp    dword ptr [esi + 85h], 0
        jne    SHORT loc_1_2343
        cmp    dword ptr [esi + 89h], 0
        jne    SHORT loc_1_2343
        push   esi
        call   sub_1_0A70
loc_1_2343:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 85h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 85h], eax
        cmp    byte ptr [esi + 6cdh], 0
        jne    NEAR PTR loc_1_2440
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0F3h, 0DDh, 0FEh, 0FFh
        or     al, al
        je     NEAR PTR loc_1_2440
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
        mov    dword ptr [esi + 6d6h], 0eeh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 8dh]
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        jne    SHORT loc_1_2412
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_23FA
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_23FA
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_2412
loc_1_23FA:
        push   1
        add    esi, 6f3h
        push   esi
        call   sub_1_04B2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2412:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_2433
        cmp    dword ptr [esi + 0a1h], 0
        jne    SHORT loc_1_2433
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_2433
        push   esi
        call   sub_1_1790
loc_1_2433:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h
loc_1_2440:
        dec    dword ptr [esi + 8dh]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 85h], eax
        push   71h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04DC
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 14h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2490:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 85h]
        cmp    esi, edx
        je     SHORT loc_1_24B3
loc_1_24A3:
        mov    eax, dword ptr [esi + 14h]
        cmp    edx, eax
        jae    SHORT loc_1_24AC
        mov    edx, eax
loc_1_24AC:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_24A3
loc_1_24B3:
        or     edx, edx
        jne    SHORT loc_1_24BC
        mov    edx, 0eeh
loc_1_24BC:
        mov    dword ptr [ecx + 6d6h], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_24D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_24F3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_24F3:
        inc    dword ptr [esi + 8dh]
        inc    dword ptr [ebx + 18h]
        mov    ecx, dword ptr [esi + 85h]
        cmp    ecx, ebx
        jne    SHORT loc_1_2511
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 85h], eax
        jmp    SHORT loc_1_2527
loc_1_2511:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_1_251E
loc_1_2516:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_2516
loc_1_251E:
        lea    eax, [ebx + 0ch]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_1_2527:
        push   esi
        call   sub_1_0A10
        or     eax, eax
        jne    SHORT loc_1_2560
        mov    eax, dword ptr [esi + 24h]
        push   0
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [esi + 24h]
        push   71h
        mov    dword ptr [eax + 10h], 0
        push   ebx
        call   sub_1_04DC
        dec    dword ptr [esi + 8dh]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2560:
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0D1h, 0DCh, 0FEh, 0FFh
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_2598
        mov    eax, dword ptr [ebx + 18h]
        cmp    eax, 2
        je     SHORT loc_1_2590
        mov    byte ptr [ebx + 1ch], 1
        sub    dword ptr [ebx + 18h], 2
        mov    edi, 103h
        jmp    SHORT loc_1_25C4
        db  08Dh, 064h, 024h, 000h
loc_1_2590:
        sub    eax, 2
        mov    dword ptr [ebx + 18h], eax
        jmp    SHORT loc_1_25C4
loc_1_2598:
        cmp    edi, 103h
        jne    SHORT loc_1_25B0
        mov    byte ptr [ebx + 1ch], 1
        sub    dword ptr [ebx + 18h], 2
        jmp    SHORT loc_1_25C4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_25B0:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_25C1
        mov    byte ptr [ebx + 1ch], 1
        mov    edi, 103h
loc_1_25C1:
        dec    dword ptr [ebx + 18h]
loc_1_25C4:
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_1_2600
        mov    eax, dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 6d6h], eax
        jne    SHORT loc_1_25DB
        push   esi
        call   sub_1_2490
loc_1_25DB:
        push   0
        push   71h
        push   ebx
        call   sub_1_04DC
        cmp    dword ptr [esi + 85h], 0
        jne    SHORT loc_1_2614
        cmp    dword ptr [esi + 89h], 0
        jne    SHORT loc_1_2614
        push   esi
        call   sub_1_0AA0
        jmp    SHORT loc_1_2614
        db  090h
loc_1_2600:
        mov    eax, dword ptr [esi + 89h]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 89h], ebx
        mov    edi, 103h
loc_1_2614:
        mov    eax, dword ptr [esi + 8dh]
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        jne    SHORT loc_1_2657
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_263E
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_263E
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_2657
loc_1_263E:
        push   1
        add    esi, 6f3h
        push   esi
        call   sub_1_04B2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2657:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_2678
        cmp    dword ptr [esi + 0a1h], 0
        jne    SHORT loc_1_2678
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_2678
        push   esi
        call   sub_1_1790
loc_1_2678:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 18h]
        inc    dword ptr [edi + 8dh]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_1_26B2
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_26F5
loc_1_26B2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        jne    SHORT loc_1_26C8
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B30
        mov    esi, eax
        jmp    SHORT loc_1_26F5
loc_1_26C8:
        cmp    ecx, 1
        jne    SHORT loc_1_26F0
        cmp    byte ptr [edi + 6cdh], 0
        je     SHORT loc_1_26DD
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_26F5
loc_1_26DD:
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B80
        mov    esi, eax
        jmp    SHORT loc_1_26F5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_26F0:
        mov    esi, 10001h
loc_1_26F5:
        dec    dword ptr [ebx + 18h]
        mov    eax, dword ptr [edi + 8dh]
        dec    eax
        mov    dword ptr [edi + 8dh], eax
        jne    SHORT loc_1_2740
        cmp    byte ptr [edi + 6cdh], 0
        jne    SHORT loc_1_2722
        cmp    dword ptr [edi + 6e6h], 0
        jne    SHORT loc_1_2722
        cmp    dword ptr [edi + 89h], 0
        je     SHORT loc_1_2740
loc_1_2722:
        push   1
        add    edi, 6f3h
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2740:
        cmp    dword ptr [edi + 99h], 0
        je     SHORT loc_1_2761
        cmp    dword ptr [edi + 0a1h], 0
        jne    SHORT loc_1_2761
        cmp    byte ptr [edi + 6f2h], 0
        jne    SHORT loc_1_2761
        push   edi
        call   sub_1_1790
loc_1_2761:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
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
        je     NEAR PTR loc_1_2990
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_27E1
        shl    eax, 2
        db  0FFh, 0A0h, 0E4h, 02Ah, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27C0:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_29B0
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_29D0
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2A90
loc_1_27E1:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 049h, 000h
loc_1_27F0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2805
        db  0BEh, 030h, 03Ah, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_2A9C
loc_1_2805:
        db  0BEh, 0B4h, 039h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_2A9C
loc_1_2814:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 6cdh], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_1_282E
        mov    dword ptr [ebp - 14h], 0
loc_1_282E:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A9C
        db  005h, 000h, 000h, 000h, 000h
loc_1_2840:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 049h, 000h
loc_1_2850:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_2860:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_2870:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_2880:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0a1h], 1
        sbb    eax, eax
        and    eax, 5eeh
        jmp    NEAR PTR loc_1_2A99
loc_1_2896:
        mov    dword ptr [ebp - 8], 2000h
        jmp    NEAR PTR loc_1_2A9C
loc_1_28A2:
        mov    dword ptr [ebp - 8], 5eeh
        jmp    NEAR PTR loc_1_2A9C
        db  08Bh, 0FFh
loc_1_28B0:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_28C0:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 79h
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05BC
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A9C
loc_1_28E6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 3ch]
        cmp    eax, 2
        je     SHORT loc_1_2905
        cmp    eax, 3
        je     SHORT loc_1_2914
        db  0BEh, 078h, 03Ch, 000h, 000h
        mov    edi, 23h
        jmp    NEAR PTR loc_1_2A9C
loc_1_2905:
        db  0BEh, 01Ch, 03Ch, 000h, 000h
        mov    edi, 29h
        jmp    NEAR PTR loc_1_2A9C
loc_1_2914:
        db  0BEh, 048h, 03Ch, 000h, 000h
        mov    edi, 2ch
        jmp    NEAR PTR loc_1_2A9C
loc_1_2923:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2937
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 24h]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_2A99
loc_1_2937:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_2A99
loc_1_2945:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2959
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 6d6h]
        jmp    NEAR PTR loc_1_2A99
loc_1_2959:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_2A99
loc_1_2964:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_1_2A9C
loc_1_2977:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2A9C
loc_1_2983:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_2A9C
        db  090h
loc_1_2990:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 79h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2A9C
        db  090h
loc_1_29B0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 7fh
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_1_2A9C
        db  090h
loc_1_29D0:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_2A35
        mov    eax, dword ptr [ebx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_1_2A00
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_1_2A9C
        db  08Dh, 049h, 000h
loc_1_2A00:
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
        db  0E8h, 047h, 0DDh, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_2A9C
        jmp    NEAR PTR loc_1_2A9C
loc_1_2A35:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        db  0E8h, 01Bh, 0DCh, 0FEh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_1_2A60
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_1_2A9C
loc_1_2A60:
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
        db  0E8h, 020h, 0DCh, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2A9C
        db  090h
loc_1_2A90:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 95h]
loc_1_2A99:
        mov    dword ptr [ebp - 8], eax
loc_1_2A9C:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2AD8
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_2AD8
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_1_2AC8
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], edi
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_2AC8:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_05BC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_2AD8:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  0F0h, 027h, 000h, 000h, 014h, 028h, 000h, 000h, 040h, 028h, 000h, 000h, 040h, 028h, 000h, 000h
        db  050h, 028h, 000h, 000h, 060h, 028h, 000h, 000h, 070h, 028h, 000h, 000h, 080h, 028h, 000h, 000h
        db  096h, 028h, 000h, 000h, 0A2h, 028h, 000h, 000h, 0B0h, 028h, 000h, 000h, 0C0h, 028h, 000h, 000h
        db  0E6h, 028h, 000h, 000h, 023h, 029h, 000h, 000h, 045h, 029h, 000h, 000h, 064h, 029h, 000h, 000h
        db  077h, 029h, 000h, 000h, 0E1h, 027h, 000h, 000h, 083h, 029h, 000h, 000h
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
        je     NEAR PTR loc_1_2CC7
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2CF4
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2D35
loc_1_2BD7:
        cmp    edi, 4
        je     SHORT loc_1_2BF4
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 4
        jmp    NEAR PTR loc_1_2D35
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
        jmp    NEAR PTR loc_1_2D35
loc_1_2C21:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2D50
        jmp    NEAR PTR loc_1_2D33
loc_1_2C38:
        cmp    edi, 4
        je     SHORT loc_1_2C55
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_1_2D35
loc_1_2C55:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05BC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0fffffffeh
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_2CC0
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 6d6h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_2C96
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 6d6h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_2D35
loc_1_2C96:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 14h], eax
        jne    SHORT loc_1_2CB7
        mov    eax, dword ptr [ecx + 14h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_2CB7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        push   edx
        call   sub_1_2490
        jmp    NEAR PTR loc_1_2D35
loc_1_2CB7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    SHORT loc_1_2D35
        db  090h
loc_1_2CC0:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_2D35
loc_1_2CC7:
        cmp    edi, 4
        je     SHORT loc_1_2CE1
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_2D35
loc_1_2CE1:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 1dh
        push   ecx
        call   sub_1_05BC
        sub    ebx, ebx
        jmp    SHORT loc_1_2D35
loc_1_2CF4:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_1_2D18
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_2D35
loc_1_2D18:
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
        call   sub_1_2DB0
loc_1_2D33:
        mov    ebx, eax
loc_1_2D35:
        or     ebx, ebx
        jne    SHORT loc_1_2D43
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_1_2D43:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_2D50:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6cdh], 0
        jne    SHORT loc_1_2D9A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_2D90
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
        db  0E8h, 05Dh, 0D8h, 0FEh, 0FFh
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2D90:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_2D9A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 6cdh], 0
        jne    SHORT loc_1_2DFA
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_2DF0
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
        db  0E8h, 019h, 0D5h, 0FEh, 0FFh
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    14h
        db  090h
loc_1_2DF0:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_1_2DFA:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E10:
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
        jne    NEAR PTR loc_1_2F26
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2E96
        je     SHORT loc_1_2EE0
        cmp    eax, 20101h
        je     SHORT loc_1_2EB2
        cmp    eax, 20102h
        je     SHORT loc_1_2EC0
        cmp    eax, 20103h
        je     SHORT loc_1_2EC8
        cmp    eax, 20104h
        je     SHORT loc_1_2ED0
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2F09
loc_1_2E96:
        cmp    eax, 1020101h
        je     SHORT loc_1_2EE8
        cmp    eax, 1020102h
        je     SHORT loc_1_2EF1
        cmp    eax, 1020103h
        je     SHORT loc_1_2F00
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2F09
loc_1_2EB2:
        mov    eax, dword ptr [ebx + 6b5h]
        jmp    SHORT loc_1_2F06
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2EC0:
        mov    eax, dword ptr [ebx + 6b9h]
        jmp    SHORT loc_1_2F06
loc_1_2EC8:
        mov    eax, dword ptr [ebx + 6bdh]
        jmp    SHORT loc_1_2F06
loc_1_2ED0:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2F09
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EE0:
        mov    eax, dword ptr [ebx + 6c9h]
        jmp    SHORT loc_1_2F06
loc_1_2EE8:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_2F09
loc_1_2EF1:
        mov    eax, dword ptr [ebx + 6c1h]
        jmp    SHORT loc_1_2F06
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2F00:
        mov    eax, dword ptr [ebx + 6c5h]
loc_1_2F06:
        mov    dword ptr [ebp - 4], eax
loc_1_2F09:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_2F26
        or     edi, edi
        jne    SHORT loc_1_2F26
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_05BC
        add    dword ptr [ebp - 8], 4
loc_1_2F26:
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
loc_1_2F40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2F72
        cmp    eax, 20101h
        jae    SHORT loc_1_2F93
        cmp    eax, 10101h
        jb     SHORT loc_1_2F8E
        cmp    eax, 10111h
        jbe    SHORT loc_1_2F93
        cmp    eax, 10113h
        je     SHORT loc_1_2F93
        jmp    SHORT loc_1_2F8E
loc_1_2F72:
        cmp    eax, 1010101h
        jb     SHORT loc_1_2F8E
        cmp    eax, 1010104h
        jbe    SHORT loc_1_2F93
        cmp    eax, 1020101h
        jb     SHORT loc_1_2F8E
        cmp    eax, 1020103h
        jbe    SHORT loc_1_2F93
loc_1_2F8E:
        mov    esi, 0c0010017h
loc_1_2F93:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 8dh]
        or     esi, esi
        jne    SHORT loc_1_2FA9
        push   ecx
        push   edi
        call   sub_1_2E10
        mov    esi, eax
loc_1_2FA9:
        mov    eax, dword ptr [edi + 8dh]
        dec    eax
        mov    dword ptr [edi + 8dh], eax
        jne    SHORT loc_1_2FF0
        cmp    byte ptr [edi + 6cdh], 0
        jne    SHORT loc_1_2FD3
        cmp    dword ptr [edi + 6e6h], 0
        jne    SHORT loc_1_2FD3
        cmp    dword ptr [edi + 89h], 0
        je     SHORT loc_1_2FF0
loc_1_2FD3:
        push   1
        add    edi, 6f3h
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2FF0:
        cmp    dword ptr [edi + 99h], 0
        je     SHORT loc_1_3011
        cmp    dword ptr [edi + 0a1h], 0
        jne    SHORT loc_1_3011
        cmp    byte ptr [edi + 6f2h], 0
        jne    SHORT loc_1_3011
        push   edi
        call   sub_1_1790
loc_1_3011:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3020:
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
loc_1_3050:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 6ffh]
        push   eax
        call   sub_1_04B8
        push   2625a0h
        call   sub_1_04D0
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 718h]
        push   eax
        call   sub_1_04B8
        push   2625a0h
        call   sub_1_04D0
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 089h, 0D0h, 0FEh, 0FFh
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_1_04C4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0438
        push   0
        push   728h
        push   esi
        call   sub_1_04DC
        push   esi
        call   sub_1_06B0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_30D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 1ch], 0
        je     SHORT loc_1_30F3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_30F3:
        inc    dword ptr [esi + 18h]
        inc    dword ptr [edi + 8dh]
        cmp    byte ptr [edi + 6cdh], 0
        je     SHORT loc_1_3120
        dec    dword ptr [esi + 18h]
        dec    dword ptr [edi + 8dh]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_3120:
        mov    ebx, dword ptr [edi + 85h]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     SHORT loc_1_3153
loc_1_3131:
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
        jne    SHORT loc_1_3131
loc_1_3153:
        mov    byte ptr [edi + 6cdh], 1
        mov    dword ptr [edi + 6ceh], 0
        mov    dword ptr [edi + 6d2h], esi
        push   edi
        call   sub_1_31F0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 18h]
        mov    eax, dword ptr [edi + 8dh]
        dec    eax
        mov    dword ptr [edi + 8dh], eax
        jne    SHORT loc_1_31C0
        cmp    byte ptr [edi + 6cdh], 0
        jne    SHORT loc_1_31A0
        cmp    dword ptr [edi + 6e6h], 0
        jne    SHORT loc_1_31A0
        cmp    dword ptr [edi + 89h], 0
        je     SHORT loc_1_31C0
loc_1_31A0:
        push   1
        add    edi, 6f3h
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
loc_1_31C0:
        cmp    dword ptr [edi + 99h], 0
        je     SHORT loc_1_31E1
        cmp    dword ptr [edi + 0a1h], 0
        jne    SHORT loc_1_31E1
        cmp    byte ptr [edi + 6f2h], 0
        jne    SHORT loc_1_31E1
        push   edi
        call   sub_1_1790
loc_1_31E1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_31F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 8dh]
        cmp    dword ptr [esi + 6e6h], 0
        je     SHORT loc_1_323F
loc_1_3207:
        mov    ecx, dword ptr [esi + 6e6h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 6e6h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_1_3233
        cmp    eax, 8
        je     SHORT loc_1_3233
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_3233:
        dec    dword ptr [edi + 18h]
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_3207
loc_1_323F:
        cmp    dword ptr [esi + 0a1h], 0
        je     SHORT loc_1_3263
        mov    dword ptr [esi + 6ceh], 2
        dec    dword ptr [esi + 8dh]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3263:
        dec    dword ptr [esi + 8dh]
        push   esi
        call   sub_1_3280
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3280:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8dh]
        inc    ecx
        mov    dword ptr [eax + 8dh], ecx
        cmp    dword ptr [eax + 0a1h], 0
        je     SHORT loc_1_32B5
        mov    dword ptr [eax + 6ceh], 3
        dec    dword ptr [eax + 8dh]
        mov    eax, 103h
        pop    ebp
        ret    4
loc_1_32B5:
        dec    ecx
        mov    dword ptr [eax + 8dh], ecx
        push   eax
        call   sub_1_32D0
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 8dh]
        mov    ecx, dword ptr [esi + 0a1h]
        or     ecx, ecx
        je     SHORT loc_1_330A
        mov    edi, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    dword ptr [esi + 0a1h], 0
loc_1_330A:
        mov    edx, dword ptr [esi + 99h]
        or     edx, edx
        je     SHORT loc_1_333E
loc_1_3314:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 99h], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    edx, dword ptr [esi + 99h]
        or     edx, edx
        jne    SHORT loc_1_3314
loc_1_333E:
        mov    edx, dword ptr [esi + 6dah]
        or     edx, edx
        je     SHORT loc_1_3372
loc_1_3348:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 6dah], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 18h]
        mov    edx, dword ptr [esi + 6dah]
        or     edx, edx
        jne    SHORT loc_1_3348
loc_1_3372:
        cmp    byte ptr [esi + 6f2h], 0
        je     SHORT loc_1_3393
        add    eax, 0
loc_1_3380:
        push   2710h
        call   sub_1_04D0
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_3380
loc_1_3393:
        push   esi
        call   sub_1_0B90
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 6cdh], 0
        mov    edi, dword ptr [esi + 85h]
        or     edi, edi
        je     SHORT loc_1_33F6
        sub    ebx, ebx
loc_1_33B8:
        inc    dword ptr [edi + 18h]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_33CF
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_33CF:
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
        jne    SHORT loc_1_33B8
loc_1_33F6:
        dec    dword ptr [esi + 8dh]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3410:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 6ceh], eax
        jne    NEAR PTR loc_1_3503
        cmp    eax, 1
        je     SHORT loc_1_3499
        cmp    eax, 2
        je     SHORT loc_1_34A1
        cmp    eax, 3
        je     SHORT loc_1_34A9
        mov    eax, dword ptr [ebp - 4]
loc_1_343B:
        cmp    eax, 103h
        je     SHORT loc_1_345A
        push   eax
        mov    eax, dword ptr [esi + 6d2h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 6d2h]
        dec    dword ptr [eax + 18h]
loc_1_345A:
        mov    eax, dword ptr [esi + 8dh]
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        jne    SHORT loc_1_34E2
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_3484
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_3484
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_34E2
loc_1_3484:
        push   1
        add    esi, 6f3h
        push   esi
        call   sub_1_04B2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3499:
        push   esi
        call   sub_1_31F0
        jmp    SHORT loc_1_343B
loc_1_34A1:
        push   esi
        call   sub_1_3280
        jmp    SHORT loc_1_343B
loc_1_34A9:
        mov    eax, dword ptr [esi + 8dh]
        inc    eax
        mov    dword ptr [esi + 8dh], eax
        cmp    dword ptr [esi + 0a1h], 0
        je     SHORT loc_1_34D0
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_34D0:
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        push   esi
        call   sub_1_32D0
        jmp    NEAR PTR loc_1_343B
loc_1_34E2:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_3503
        cmp    dword ptr [esi + 0a1h], 0
        jne    SHORT loc_1_3503
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_3503
        push   esi
        call   sub_1_1790
loc_1_3503:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3510:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_3523
        lea    edx, [eax + 49h]
loc_1_3523:
        cmp    byte ptr [ecx + 6cdh], 0
        je     SHORT loc_1_3535
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_3535:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 18h]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 6e6h], 0
        jne    SHORT loc_1_3570
        mov    dword ptr [ecx + 6eah], edx
        mov    dword ptr [ecx + 6e6h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_3570:
        mov    eax, dword ptr [ecx + 6eah]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 6eah], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3590:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_1_35A3
        lea    edx, [eax + 21h]
loc_1_35A3:
        cmp    byte ptr [ecx + 6cdh], 0
        je     SHORT loc_1_35B5
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_35B5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 18h]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 6e6h], 0
        jne    SHORT loc_1_35F0
        mov    dword ptr [ecx + 6eah], edx
        mov    dword ptr [ecx + 6e6h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_35F0:
        mov    eax, dword ptr [ecx + 6eah]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 6eah], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3610:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 18h]
        ret    4
        db  08Dh, 049h, 000h
loc_1_3620:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 8dh]
        cmp    byte ptr [esi + 6cdh], 0
        jne    NEAR PTR loc_1_36E4
        cmp    dword ptr [esi + 6eeh], 0
        jne    NEAR PTR loc_1_36E4
        lea    esp, [esp]
loc_1_3650:
        mov    edi, dword ptr [esi + 6e6h]
        or     edi, edi
        je     SHORT loc_1_36BC
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 6e6h], eax
        cmp    dword ptr [esi + 6eah], edi
        jne    SHORT loc_1_3674
        mov    dword ptr [esi + 6eah], 0
loc_1_3674:
        mov    dword ptr [esi + 6eeh], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_3690
        cmp    eax, 0ah
        je     SHORT loc_1_3690
        push   esi
        call   sub_1_37D0
        jmp    SHORT loc_1_3696
        db  090h
loc_1_3690:
        push   esi
        call   sub_1_3830
loc_1_3696:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_36B0
        cmp    ecx, 4
        je     SHORT loc_1_36B0
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_36B0:
        dec    dword ptr [ebx + 18h]
        cmp    byte ptr [esi + 6cdh], 0
        je     SHORT loc_1_3650
loc_1_36BC:
        mov    dword ptr [esi + 6eeh], 0
        cmp    byte ptr [esi + 6cdh], 0
        je     SHORT loc_1_36E4
        dec    dword ptr [esi + 8dh]
        push   1
        push   esi
        call   sub_1_3410
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_36E4:
        mov    edi, dword ptr [esi + 89h]
        or     edi, edi
        je     SHORT loc_1_3760
        sub    ebx, ebx
loc_1_36F0:
        cmp    dword ptr [edi + 18h], 0
        jbe    SHORT loc_1_3700
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_3744
        db  08Dh, 049h, 000h
loc_1_3700:
        mov    eax, dword ptr [esi + 6d6h]
        cmp    dword ptr [edi + 14h], eax
        jne    SHORT loc_1_3711
        push   esi
        call   sub_1_2490
loc_1_3711:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [edi + 0ch]
        je     SHORT loc_1_3730
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_3736
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3730:
        mov    dword ptr [esi + 89h], eax
loc_1_3736:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   71h
        push   eax
        call   sub_1_04DC
loc_1_3744:
        or     edi, edi
        jne    SHORT loc_1_36F0
        cmp    dword ptr [esi + 89h], 0
        jne    SHORT loc_1_3760
        cmp    dword ptr [esi + 85h], 0
        jne    SHORT loc_1_3760
        push   esi
        call   sub_1_0AA0
loc_1_3760:
        mov    eax, dword ptr [esi + 8dh]
        dec    eax
        mov    dword ptr [esi + 8dh], eax
        jne    SHORT loc_1_37A0
        cmp    byte ptr [esi + 6cdh], 0
        jne    SHORT loc_1_378A
        cmp    dword ptr [esi + 6e6h], 0
        jne    SHORT loc_1_378A
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_37A0
loc_1_378A:
        push   1
        add    esi, 6f3h
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  090h
loc_1_37A0:
        cmp    dword ptr [esi + 99h], 0
        je     SHORT loc_1_37C1
        cmp    dword ptr [esi + 0a1h], 0
        jne    SHORT loc_1_37C1
        cmp    byte ptr [esi + 6f2h], 0
        jne    SHORT loc_1_37C1
        push   esi
        call   sub_1_1790
loc_1_37C1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_37D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 24h]
        mov    eax, dword ptr [eax + 0ch]
        test   al, 20h
        mov    dword ptr [esi + 0a7h], eax
        je     SHORT loc_1_37F0
        lea    eax, [esi + 70h]
        mov    byte ptr [eax], 3
        jmp    SHORT loc_1_3819
        db  090h
loc_1_37F0:
        test   al, 4
        push   esi
        je     SHORT loc_1_3800
        call   sub_1_0CD0
        jmp    SHORT loc_1_3805
        db  08Dh, 064h, 024h, 000h
loc_1_3800:
        call   sub_1_3830
loc_1_3805:
        cmp    dword ptr [esi + 0a7h], 0
        lea    eax, [esi + 70h]
        je     SHORT loc_1_3816
        mov    byte ptr [eax], 2
        jmp    SHORT loc_1_3819
loc_1_3816:
        mov    byte ptr [eax], 0
loc_1_3819:
        mov    edx, dword ptr [esi + 49h]
        add    dx, 5
        mov    al, byte ptr [eax]
        out    dx, al
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0C50
        push   esi
        call   sub_1_0D20
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
loc_1_3930:
        mov    al, 38h
        add    byte ptr [eax], al
        db  0F0h, 038h, 000h, 000h
loc_1_3938:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 6d6fh
loc_1_3940:
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
        pop    eax
        imul   esi, dword ptr [edx + 63h], 45206d6fh
        je     SHORT loc_1_39EE
        jb     NEAR PTR loc_1_39F7
        db  065h, 074h, 02Bh
        dec    ebp
        outsd
        insd
        and    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        and    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_1_3A03
        db  072h, 02Ch
        and    byte ptr [esi + 31h], dh
        xor    byte ptr cs:[eax], dh
        and    byte ptr [eax], ch
        xor    byte ptr [ecx], bh
        xor    dword ptr [ecx + edi], esi
        xor    al, 29h
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_39B4:
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
loc_1_39EE:
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
loc_1_39F7:
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
loc_1_3A03:
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
loc_1_3A30:
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
        dec    ebp
        xor    cl, byte ptr [esi + 44h]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4dh], al
        xor    cl, byte ptr [esi + 44h]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], al
        dec    ebp
        sub    eax, 76204949h
        xor    dword ptr [esi], ebp
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        inc    ebp
        dec    ebp
        sub    eax, 76204949h
        xor    dword ptr [esi], ebp
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 054h
        jns    SHORT loc_1_3B3E
        add    byte ptr gs:[ecx + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_3B2C
        db  079h, 070h
        add    byte ptr gs:[ecx + 6fh], cl
        inc    edx
        popad
        jae    SHORT loc_1_3B47
        inc    ecx
        jb     NEAR PTR loc_1_3B4C
        jae    SHORT loc_1_3B5C
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], cl
        inc    edx
        popad
        jae    SHORT loc_1_3B57
        inc    ecx
        jb     NEAR PTR loc_1_3B5C
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        jb     SHORT loc_1_3B7B
        inc    edx
        popad
        jae    SHORT loc_1_3B6B
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_3B80
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        jb     SHORT loc_1_3B8F
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_3B8D
        db  072h, 072h
        db  075h, 070h
loc_1_3B2C:
        je     SHORT loc_1_3B7C
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_3B8C
loc_1_3B3E:
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 52h], cl
        push   ecx
loc_1_3B47:
        dec    esp
        db  065h, 076h, 065h
        insb
loc_1_3B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3B50:
        dec    ecx
        push   edx
        push   ecx
        dec    esp
        db  065h, 076h, 065h
loc_1_3B57:
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3B5C:
        dec    ebp
        popad
        js     SHORT loc_1_3BC9
        insd
        db  075h, 06Dh
        dec    ebp
        db  075h, 06Ch
        je     SHORT loc_1_3BD1
        arpl   word ptr [ecx + 73h], sp
loc_1_3B6B:
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_3B74:
        dec    ebp
        popad
        js     SHORT loc_1_3BE1
        insd
        jne    SHORT loc_1_3BE8
loc_1_3B7B:
        dec    ebp
loc_1_3B7C:
        jne    SHORT loc_1_3BEA
        je     SHORT loc_1_3BE9
loc_1_3B80:
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_3BD1
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_3B8C:
        inc    ebx
loc_1_3B8D:
        dec    edi
        dec    ebp
loc_1_3B8F:
        add    byte ptr [ebx + 4fh], al
        dec    ebp
        add    byte ptr [ebx + 6fh], al
        insd
        push   eax
        outsd
        db  072h, 074h
        add    byte ptr [ebx + 6fh], al
        insd
        push   eax
        outsd
        db  072h, 074h
        add    byte ptr [eax + 6fh], dl
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 65746e49h
        db  072h, 076h
        popad
        insb
        add    byte ptr [eax + 6fh], dl
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 65746e49h
        jb     SHORT loc_1_3C37
        popad
        insb
        add    byte ptr [eax + 43h], dl
        dec    ecx
        inc    ebx
        inc    edx
loc_1_3BC9:
        popad
        jae    SHORT loc_1_3C31
        inc    ecx
        jb     NEAR PTR loc_1_3BD1
loc_1_3BD1:
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    ecx
        inc    ebx
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_3BE1
loc_1_3BE1:
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax*2 + 49h], dl
        inc    ebx
loc_1_3BE8:
        inc    edx
loc_1_3BE9:
        popad
loc_1_3BEA:
        jae    SHORT loc_1_3C51
        inc    ecx
        jb     NEAR PTR loc_1_3BF1
loc_1_3BF1:
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax*2 + 49h], dl
        inc    ebx
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_3C01
loc_1_3C01:
        add    byte ptr [eax], al
        add    byte ptr [edi + 61h], dl
        imul   esi, dword ptr [ebx + edx*2 + 74h], 73657461h
        add    byte ptr [eax], al
        push   edi
        popad
        imul   esi, dword ptr [ebx + edx*2 + 74h], 73657461h
        add    byte ptr [eax], al
loc_1_3C1C:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 43206d6fh
        jb     SHORT loc_1_3C8B
        imul   esi, dword ptr fs:[ebx + eax*2 + 61h], 45206472h
        db  074h, 068h
loc_1_3C31:
        db  065h, 072h, 06Eh
        db  065h, 074h, 02Bh
loc_1_3C37:
        dec    ebp
        outsd
        insd
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax + 69h], bl
        db  072h, 063h
        outsd
        insd
        and    byte ptr [ebx + 72h], al
loc_1_3C51:
        imul   esi, dword ptr fs:[ebx + eax*2 + 61h], 45206472h
        db  074h, 068h
        db  065h, 072h, 06Eh
        db  065h, 074h, 02Bh
        dec    ebp
        outsd
        insd
        and    byte ptr [ecx + 49h], cl
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3C78:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 43206d6fh
        db  072h, 065h
        imul   esi, dword ptr fs:[ebx + eax*2 + 61h], 45206472h
loc_1_3C8B:
        db  074h, 068h
        db  065h, 072h, 06Eh
        db  065h, 074h, 020h
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
_LTEXT ends
        end
