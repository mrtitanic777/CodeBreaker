; v86mmgr_pcode_15.asm -- V86MMGR object 15 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_15_0000:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        movzx  eax, byte ptr [ebp + 1dh]
        shl    eax, 1
        cmp    al, 26h
        jae    SHORT loc_15_0049
        mov    dl, ah
        xor    ah, ah
        db  08Bh, 03Ch, 045h, 018h, 012h, 000h, 000h
        shl    edi, 1
        sbb    dl, 0
        ja     SHORT loc_15_0049
        stc
        rcr    edi, 1
        test   byte ptr [esi + ebx + 2fh], 40h
        je     SHORT loc_15_005B
loc_15_002C:
        call   edi
loc_15_002E:
        jb     SHORT loc_15_0037
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
loc_15_0037:
        db  08Bh, 00Dh, 09Ch, 012h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_15_0049:
        db  068h, 02Eh, 000h, 000h, 000h
        mov    al, 80h
loc_15_0050:
        mov    byte ptr [ebp + 10h], al
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_15_005B:
        or     byte ptr [esi + ebx + 2fh], 40h
        mov    dword ptr [esi + ebx + 3ch], 0
        mov    dword ptr [esi + ebx + 40h], 200h
        jmp    SHORT loc_15_002C
        db  066h, 0C7h, 045h, 01Ch, 000h, 003h, 066h, 0C7h, 045h, 010h, 05Fh, 003h, 033h, 0D2h, 0F6h, 005h
        db  004h, 012h, 000h, 000h, 080h, 075h, 008h, 0F6h, 044h, 01Eh, 02Ch, 004h, 075h, 001h, 042h, 066h
        db  089h, 055h, 014h, 0F8h, 0C3h
sub_15_0097:
        db  08Bh, 015h, 030h, 013h, 000h, 000h
        cmp    edx, -1
        je     SHORT loc_15_00BA
        mov    ecx, ebx
loc_15_00A4:
        mov    edi, dword ptr [esi + ebx + 60h]
        or     edi, edi
        je     SHORT loc_15_00B0
        mov    word ptr [edi + 16h], dx
loc_15_00B0:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, ecx
        jne    SHORT loc_15_00A4
loc_15_00BA:
        db  0BEh, 064h, 012h, 000h, 000h
        db  0BFh, 01Ch, 012h, 000h, 000h
        mov    ecx, 6
        cld
        rep movsd
        db  0C7h, 005h, 028h, 013h, 000h, 000h, 066h, 030h, 000h, 000h
        db  080h, 00Dh, 004h, 012h, 000h, 000h, 060h
loc_15_00DD:
        ret
sub_15_00DE:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        stc
        je     SHORT loc_15_011C
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 080h
        stc
        jne    SHORT loc_15_011C
        push   esi
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        test   byte ptr [esi + ebx + 2ch], 4
        jne    SHORT loc_15_011B
        cmp    dword ptr [esi + ebx + 60h], 0
        je     SHORT loc_15_0118
        mov    esi, dword ptr [esi + ebx + 60h]
        test   byte ptr [esi + 14h], 2
        je     SHORT loc_15_0118
        mov    eax, dword ptr [esi + 4]
        xor    esi, esi
        jmp    SHORT loc_15_011B
loc_15_0118:
        xor    esi, esi
        inc    esi
loc_15_011B:
        pop    esi
loc_15_011C:
        ret
sub_15_011D:
        pushfd
        sti
        cld
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
        popfd
        ret
sub_15_0137:
        pushad
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        or     eax, eax
        je     SHORT loc_15_014A
        btr    dword ptr [ecx + ebx + 2ch], 2
        jmp    SHORT loc_15_0167
loc_15_014A:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        je     SHORT loc_15_0167
        mov    edi, dword ptr [ecx + ebx + 60h]
        or     edi, edi
        je     SHORT loc_15_0161
        test   byte ptr [edi + 14h], 2
        jne    SHORT loc_15_0167
loc_15_0161:
        bts    dword ptr [ecx + ebx + 2ch], 2
loc_15_0167:
        popad
        ret
        db  000h, 000h, 000h
loc_15_016C:
        push   3
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        ret
        db  0A1h, 02Ch, 013h, 000h, 000h, 0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h
        db  00Fh, 0BAh, 02Dh, 004h, 012h, 000h, 000h, 009h, 0F5h, 073h, 005h, 0E8h, 08Eh, 000h, 000h, 000h
        db  09Ch, 0A1h, 02Ch, 013h, 000h, 000h, 0CDh, 020h, 028h, 000h, 001h, 000h, 09Dh, 0B0h, 082h, 00Fh
        db  082h, 090h, 001h, 000h, 000h, 033h, 0C0h, 088h, 045h, 010h, 00Ah, 0C0h, 066h, 0C7h, 045h, 01Ch
        db  001h, 000h, 0F8h, 0C3h, 0A1h, 02Ch, 013h, 000h, 000h, 0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h
        db  027h, 000h, 001h, 000h, 00Fh, 0BAh, 035h, 004h, 012h, 000h, 000h, 009h, 073h, 005h, 0E8h, 0B2h
        db  000h, 000h, 000h, 09Ch, 0A1h, 02Ch, 013h, 000h, 000h, 0CDh, 020h, 028h, 000h, 001h, 000h, 09Dh
        db  00Fh, 082h, 04Fh, 001h, 000h, 000h, 0E9h, 024h, 001h, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 000h
        db  00Fh, 082h, 03Fh, 001h, 000h, 000h, 0E9h, 014h, 001h, 000h, 000h
loc_15_0205:
        db  0A1h, 02Ch, 013h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        call   sub_15_0228
        pushfd
        db  0A1h, 02Ch, 013h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        popfd
        ret
sub_15_0228:
        db  083h, 03Dh, 030h, 013h, 000h, 000h, 0FEh
        jae    SHORT loc_15_025A
        db  0FFh, 005h, 030h, 013h, 000h, 000h
        db  083h, 03Dh, 030h, 013h, 000h, 000h, 001h
        jne    SHORT loc_15_0258
        push   ebx
loc_15_0241:
        push   3
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_15_0241
        pop    ebx
loc_15_0258:
        clc
        ret
loc_15_025A:
        stc
        ret
        db  0E8h, 00Bh, 000h, 000h, 000h, 00Fh, 082h, 0D8h, 000h, 000h, 000h, 0E9h, 0ADh, 000h, 000h, 000h
loc_15_026C:
        db  0A1h, 02Ch, 013h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        call   sub_15_028F
        pushfd
        db  0A1h, 02Ch, 013h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        popfd
        ret
sub_15_028F:
        db  083h, 03Dh, 030h, 013h, 000h, 000h, 0FFh
        je     SHORT loc_15_02C3
        db  083h, 03Dh, 030h, 013h, 000h, 000h, 000h
        je     SHORT loc_15_02C3
        db  0FFh, 00Dh, 030h, 013h, 000h, 000h
        jne    SHORT loc_15_02C1
        push   ebx
loc_15_02AA:
        push   4
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_15_02AA
        pop    ebx
loc_15_02C1:
        clc
        ret
loc_15_02C3:
        stc
        ret
        db  0B0h, 090h, 0F6h, 044h, 01Eh, 02Ch, 004h, 00Fh, 085h, 07Eh, 0FDh, 0FFh, 0FFh, 08Bh, 07Ch, 01Eh
        db  060h, 0F6h, 047h, 014h, 002h, 075h, 049h, 068h, 000h, 001h, 000h, 000h, 06Ah, 000h, 06Ah, 000h
        db  06Ah, 000h, 06Ah, 000h, 053h, 06Ah, 000h, 06Ah, 010h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h
        db  0C4h, 020h, 00Bh, 0C0h, 074h, 02Ah, 089h, 007h, 089h, 057h, 004h, 080h, 04Fh, 014h, 002h, 06Ah
        db  008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h, 000h, 059h, 059h, 00Bh, 0C0h, 074h, 006h, 0FFh, 015h
        db  028h, 013h, 000h, 000h
loc_15_0319:
        mov    byte ptr [ebp + 10h], 0
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_15_0325:
        mov    al, 91h
        jmp    NEAR PTR loc_15_0050
        db  08Bh, 07Ch, 01Eh, 060h, 066h, 00Fh, 0BAh, 06Fh, 014h, 000h, 072h, 0E1h, 0E8h, 033h, 000h, 000h
        db  000h, 073h, 0DAh, 0B0h, 082h, 0E9h, 00Ah, 0FDh, 0FFh, 0FFh, 08Bh, 07Ch, 01Eh, 060h, 066h, 00Fh
        db  0BAh, 077h, 014h, 000h, 073h, 0C7h, 0E8h, 045h, 000h, 000h, 000h, 072h, 0E6h, 0EBh, 0BEh, 0E8h
        db  004h, 000h, 000h, 000h, 072h, 0DDh, 0EBh, 0B5h
loc_15_0364:
        push   edi
        mov    edi, dword ptr [esi + ebx + 60h]
        call   sub_15_0370
        pop    edi
        ret
sub_15_0370:
        cmp    word ptr [edi + 16h], 0
        jne    SHORT loc_15_037F
        db  0FFh, 015h, 028h, 013h, 000h, 000h
        je     SHORT loc_15_0385
loc_15_037F:
        inc    word ptr [edi + 16h]
        clc
        ret
loc_15_0385:
        stc
        ret
        db  0E8h, 004h, 000h, 000h, 000h, 072h, 0B1h, 0EBh, 089h
loc_15_0390:
        push   edi
        mov    edi, dword ptr [esi + ebx + 60h]
        call   sub_15_039C
        pop    edi
        ret
sub_15_039C:
        cmp    word ptr [edi + 16h], 0
        je     SHORT loc_15_03C0
        dec    word ptr [edi + 16h]
        jne    SHORT loc_15_03BE
        push   4
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   byte ptr [edi + 14h], 2
        je     SHORT loc_15_03BE
        and    byte ptr [edi + 14h], 0fbh
loc_15_03BE:
        clc
        ret
loc_15_03C0:
        stc
        ret
        db  0C6h, 045h, 010h, 000h, 0C6h, 045h, 01Dh, 000h, 06Ah, 008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h
        db  000h, 059h, 059h, 00Bh, 0C0h, 00Fh, 095h, 045h, 01Ch, 0C3h
sub_15_03DC:
        db  005h, 002h, 000h, 000h, 06Ch
        add    al, byte ptr [eax]
        db  000h, 064h, 003h, 000h
        db  000h, 090h, 003h, 000h, 000h
V86MMGR_Toggle_HMA:
        push   esi
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 080h
        jne    SHORT loc_15_041A
        bsf    eax, eax
        stc
        je     SHORT loc_15_041A
        dec    eax
        cmp    al, 2
        cmc
        jb     SHORT loc_15_041A
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 040h
        je     SHORT loc_15_040D
        add    al, 2
loc_15_040D:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        db  0FFh, 014h, 085h, 0DCh, 003h, 000h, 000h
loc_15_041A:
        sbb    eax, eax
        pop    esi
        inc    eax
        ret
        db  000h
sub_15_0420:
        push   esi
        push   edi
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        xor    edx, edx
        bt     dword ptr [edi + ebx + 2ch], 4
        rcl    edx, 8
        mov    eax, dword ptr [edi + ebx + 60h]
        mov    eax, dword ptr [eax + 8]
        add    eax, ecx
        mov    ecx, dword ptr [edi + ebx + 30h]
        mov    esi, dword ptr [edi + ebx + 38h]
        sub    esi, eax
        jbe    SHORT loc_15_047C
        cmp    esi, dword ptr [edi + ebx + 34h]
        stc
        je     SHORT loc_15_0496
        jecxz  loc_15_045F
        push   edx
        push   esi
        push   ecx
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_15_0475
loc_15_045F:
        push   edx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   esi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
loc_15_0475:
        or     eax, eax
        jne    SHORT loc_15_048E
        stc
        jmp    SHORT loc_15_0496
loc_15_047C:
        stc
        jecxz  loc_15_0496
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        xor    eax, eax
        xor    esi, esi
loc_15_048E:
        mov    dword ptr [edi + ebx + 30h], eax
        mov    dword ptr [edi + ebx + 34h], esi
loc_15_0496:
        pop    edi
        pop    esi
        ret
sub_15_0499:
        push   edi
        push   eax
        push   -1
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        xor    ecx, ecx
        bt     dword ptr [edi + ebx + 2ch], 4
        rcl    ecx, 8
        push   ecx
        mov    ecx, edi
        mov    edx, dword ptr [edi + ebx + 60h]
        push   edx
        mov    edi, dword ptr [esi + 0ch]
        or     edi, dword ptr [esp + 0ch]
        je     NEAR PTR loc_15_0562
        mov    edi, dword ptr [esp + 0ch]
        add    edi, 3
        rcr    edi, 1
        shr    edi, 1
        mov    eax, edi
        sub    eax, dword ptr [esi + 8]
        je     SHORT loc_15_0545
        add    eax, dword ptr [edx + 8]
        cmp    eax, dword ptr [ecx + ebx + 40h]
        ja     SHORT loc_15_053D
loc_15_04DE:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_15_050C
        or     edi, edi
        jne    SHORT loc_15_04FA
        push   0
        push   dword ptr [esi]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        xor    eax, eax
        xor    edx, edx
        jmp    SHORT loc_15_0540
loc_15_04FA:
        push   dword ptr [esp + 4]
        push   edi
        push   dword ptr [esi]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_15_0525
loc_15_050C:
        push   dword ptr [esp + 4]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
loc_15_0525:
        or     eax, eax
        jne    SHORT loc_15_0540
        mov    ecx, edi
        sub    ecx, dword ptr [esi + 8]
        jbe    SHORT loc_15_053D
        inc    dword ptr [esp + 8]
        jne    SHORT loc_15_053D
        call   sub_15_0420
        jae    SHORT loc_15_04DE
loc_15_053D:
        stc
        jmp    SHORT loc_15_0562
loc_15_0540:
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
loc_15_0545:
        xchg   dword ptr [esi + 8], edi
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [esi + 8]
        sub    ecx, edi
        mov    edi, dword ptr [esp]
        add    dword ptr [edi + 8], ecx
        xor    ecx, ecx
        call   sub_15_0420
        clc
loc_15_0562:
        lea    esp, [esp + 10h]
        pop    edi
        ret
sub_15_0568:
        mov    edi, dword ptr [esi + ebx + 60h]
        pushad
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        test   eax, eax
        je     SHORT loc_15_0585
        db  0CDh, 020h, 006h, 000h, 007h, 000h
        jae    SHORT loc_15_0585
        popad
        jmp    NEAR PTR loc_15_062A
loc_15_0585:
        popad
        push   0
        db  0CDh, 020h, 059h, 000h, 001h, 000h
        add    esp, 4
        sub    ecx, 30h
        cmp    eax, ecx
        ja     SHORT loc_15_059D
        mov    ecx, eax
loc_15_059D:
        mov    eax, 80h
        test   byte ptr [esi + ebx + 2ch], 10h
        je     SHORT loc_15_05B6
        xor    eax, eax
        cmp    ecx, edx
        jbe    SHORT loc_15_05B6
        mov    eax, 10h
        mov    ecx, edx
loc_15_05B6:
        sub    ecx, eax
        jae    SHORT loc_15_05BC
        xor    ecx, ecx
loc_15_05BC:
        mov    edx, dword ptr [esi + ebx + 40h]
        sub    edx, dword ptr [edi + 8]
        jb     SHORT loc_15_062A
        shl    edx, 2
        shl    ecx, 2
        cmp    edx, ecx
        jbe    SHORT loc_15_05D1
        mov    edx, ecx
loc_15_05D1:
        mov    eax, dword ptr [esi + ebx + 34h]
        or     eax, eax
        je     SHORT loc_15_05E2
        shl    eax, 2
        cmp    eax, edx
        jbe    SHORT loc_15_05E2
        mov    edx, eax
loc_15_05E2:
        mov    eax, edx
        cmp    eax, 800h
        jbe    SHORT loc_15_05F0
        mov    eax, 800h
loc_15_05F0:
        test   byte ptr [ebp + 1dh], 80h
        je     SHORT loc_15_060F
        mov    dword ptr [ebp + 14h], edx
        mov    dword ptr [ebp + 1ch], eax
        mov    edx, dword ptr [esi + ebx + 40h]
        add    edx, 110h
        shl    edx, 0ch
        dec    edx
        mov    dword ptr [ebp + 18h], edx
        jmp    SHORT loc_15_0624
loc_15_060F:
        test   edx, 0ffff0000h
        je     SHORT loc_15_061C
        mov    edx, 0ffffh
loc_15_061C:
        mov    word ptr [ebp + 14h], dx
        mov    word ptr [ebp + 1ch], ax
loc_15_0624:
        mov    byte ptr [ebp + 10h], 0
        clc
        ret
loc_15_062A:
        xor    edx, edx
        mov    eax, edx
        jmp    SHORT loc_15_05E2
loc_15_0630:
        mov    edi, dword ptr [esi + ebx + 60h]
        mov    eax, dword ptr [ebp + 14h]
        test   byte ptr [ebp + 1dh], 80h
        jne    SHORT loc_15_0642
        and    eax, 0ffffh
loc_15_0642:
        add    eax, 3
        rcr    eax, 1
        shr    eax, 1
        add    eax, dword ptr [edi + 8]
        cmp    eax, dword ptr [esi + ebx + 40h]
        ja     SHORT loc_15_06BA
        lea    esi, [edi + 1ch]
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_15_0665
        cld
loc_15_065E:
        lodsd
        or     eax, eax
        jg     SHORT loc_15_066C
        loop   loc_15_065E
loc_15_0665:
        mov    al, 0a1h
        jmp    NEAR PTR loc_15_0050
loc_15_066C:
        sub    esi, 4
        push   101h
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_15_06BA
        mov    edi, esi
        mov    esi, eax
        xchg   dword ptr [edi], eax
        mov    dword ptr [esi + 10h], eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ebp + 14h], ax
        mov    eax, ecx
        test   byte ptr [ebp + 1dh], 80h
        jne    SHORT loc_15_069F
        and    eax, 0ffffh
loc_15_069F:
        call   sub_15_0499
        jae    NEAR PTR loc_15_0319
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi], eax
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_15_06BA:
        mov    word ptr [ebp + 14h], 0
loc_15_06C0:
        mov    al, 0a0h
        jmp    NEAR PTR loc_15_0050
        db  08Bh, 07Ch, 01Eh, 060h, 00Fh, 0B7h, 055h, 014h, 0E8h, 06Bh, 004h, 000h, 000h, 00Fh, 082h, 09Ah
        db  000h, 000h, 000h, 075h, 032h, 08Bh, 0F2h, 033h, 0C0h, 0E8h, 0B4h, 0FDh, 0FFh, 0FFh, 06Ah, 000h
        db  056h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h, 08Bh, 0C6h, 08Dh, 07Fh, 01Ch, 08Bh, 00Dh
        db  0ACh, 012h, 000h, 000h, 0E3h, 00Ch, 0FCh, 0F2h, 0AFh, 075h, 007h, 00Fh, 0B7h, 055h, 014h, 089h
        db  057h, 0FCh, 0E9h, 00Bh, 0FCh, 0FFh, 0FFh
loc_15_070E:
        movzx  ecx, word ptr [edx + 0ah]
        jecxz  loc_15_075E
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
loc_15_0728:
        mov    ax, word ptr [edx + 8]
        mov    word ptr [esi + 14h], ax
        mov    byte ptr [esi + 1dh], 0dh
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0BBh, 0FEh, 0F0h, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [esi + 1ch], 1
        jne    SHORT loc_15_074D
        loop   loc_15_0728
loc_15_074D:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
loc_15_075E:
        btr    word ptr [edx + 0ch], 1
        jae    SHORT loc_15_076C
        or     byte ptr [edx + 0ch], 4
        jmp    SHORT loc_15_0772
loc_15_076C:
        mov    word ptr [edx + 0ch], 0
loc_15_0772:
        stc
        ret
loc_15_0774:
        mov    al, 0a2h
        jmp    NEAR PTR loc_15_0050
        db  083h, 0ECh, 070h, 056h, 057h, 083h, 07Ch, 024h, 07Ch, 000h, 075h, 029h, 00Fh, 0B7h, 094h, 024h
        db  080h, 000h, 000h, 000h, 00Fh, 0B7h, 084h, 024h, 082h, 000h, 000h, 000h, 0C1h, 0E0h, 004h, 003h
        db  0C2h, 089h, 084h, 024h, 080h, 000h, 000h, 000h, 0BEh, 0F0h, 0FFh, 010h, 000h, 08Bh, 043h, 004h
        db  0E9h, 0ACh, 000h, 000h, 000h, 08Bh, 054h, 024h, 07Ch, 0E8h, 086h, 003h, 000h, 000h, 00Fh, 082h
        db  0B3h, 000h, 000h, 000h, 00Fh, 084h, 08Eh, 000h, 000h, 000h, 066h, 083h, 07Ah, 00Ah, 000h, 075h
        db  07Dh, 066h, 00Fh, 0BAh, 072h, 00Ch, 004h, 072h, 006h, 0F6h, 042h, 00Ch, 002h, 075h, 06Fh, 08Dh
        db  07Ch, 024h, 008h, 08Bh, 044h, 024h, 07Ch, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 0CDh, 020h, 082h
        db  000h, 001h, 000h, 066h, 089h, 045h, 014h, 0C6h, 045h, 01Dh, 00Ch, 0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0F9h, 0FDh, 0F0h, 0FFh, 0CDh, 020h, 085h, 000h, 001h, 000h, 08Bh, 07Dh, 014h, 0C1h, 0E7h
        db  010h, 066h, 08Bh, 07Dh, 010h, 00Fh, 0B7h, 045h, 01Ch, 0CDh, 020h, 086h, 000h, 001h, 000h, 08Dh
        db  074h, 024h, 008h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 083h, 0F8h, 001h, 075h, 049h, 08Bh, 0F2h
        db  08Bh, 046h, 004h, 0C1h, 0E0h, 00Ah, 06Ah, 000h, 050h, 057h, 0CDh, 020h, 06Ch, 000h, 001h, 000h
        db  083h, 0C4h, 00Ch, 083h, 0F8h, 0FFh, 089h, 006h, 066h, 0FFh, 046h, 00Ah, 08Bh, 0D6h, 08Bh, 002h
        db  08Bh, 072h, 004h, 0C1h, 0E6h, 00Ah, 0EBh, 009h, 08Bh, 042h, 004h, 08Bh, 072h, 008h, 0C1h, 0E6h
        db  00Ch, 08Bh, 094h, 024h, 080h, 000h, 000h, 000h, 003h, 0C2h, 02Bh, 0F2h, 076h, 00Dh, 02Bh, 0B4h
        db  024h, 084h, 000h, 000h, 000h, 073h, 00Bh, 0B0h, 0A3h, 0EBh, 006h, 0B0h, 0A4h, 0EBh, 002h, 0B0h
        db  0A7h, 0F9h, 05Fh, 05Eh, 08Dh, 064h, 024h, 070h, 0C2h, 00Ch, 000h, 083h, 0ECh, 00Ch, 08Bh, 07Ch
        db  01Eh, 060h, 066h, 0B8h, 004h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 096h, 08Bh, 00Eh, 089h
        db  04Ch, 024h, 008h, 00Fh, 0B7h, 056h, 004h, 051h, 0FFh, 076h, 006h, 052h, 0E8h, 0CFh, 0FEh, 0FFh
        db  0FFh, 00Fh, 082h, 0A4h, 000h, 000h, 000h, 089h, 044h, 024h, 004h, 00Fh, 0B7h, 056h, 00Ah, 0FFh
        db  074h, 024h, 008h, 0FFh, 076h, 00Ch, 052h, 0E8h, 0B4h, 0FEh, 0FFh, 0FFh, 00Fh, 082h, 083h, 000h
        db  000h, 000h, 089h, 004h, 024h, 08Bh, 035h, 0FCh, 011h, 000h, 000h, 08Bh, 07Ch, 01Eh, 060h, 0F6h
        db  005h, 004h, 012h, 000h, 000h, 040h, 075h, 010h, 0A1h, 02Ch, 013h, 000h, 000h, 0B9h, 005h, 000h
        db  000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 06Ah, 008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h
        db  000h, 059h, 059h, 00Bh, 0C0h, 09Ch, 057h, 075h, 00Ah, 0FFh, 015h, 028h, 013h, 000h, 000h, 0B0h
        db  082h, 074h, 048h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 010h, 0E8h
        db  0FEh, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 05Fh, 09Dh, 075h, 00Bh, 06Ah, 004h, 053h, 0CDh, 020h
        db  07Dh, 000h, 001h, 000h, 059h, 059h, 0F6h, 005h, 004h, 012h, 000h, 000h, 040h, 075h, 00Bh, 0A1h
        db  02Ch, 013h, 000h, 000h, 0CDh, 020h, 028h, 000h, 001h, 000h, 066h, 0C7h, 045h, 01Ch, 001h, 000h
        db  083h, 0C4h, 00Ch, 0F8h, 0C3h, 03Ch, 0A7h, 074h, 002h, 004h, 002h, 088h, 045h, 010h, 066h, 083h
        db  065h, 01Ch, 000h, 0EBh, 0EBh, 08Bh, 07Ch, 01Eh, 060h, 00Fh, 0B7h, 055h, 014h, 0E8h, 0D2h, 001h
        db  000h, 000h, 00Fh, 082h, 001h, 0FEh, 0FFh, 0FFh, 00Fh, 085h, 0F9h, 0FDh, 0FFh, 0FFh, 06Ah, 020h
        db  053h, 0CDh, 020h, 019h, 000h, 017h, 000h, 059h, 059h, 0B0h, 0ADh, 0E9h, 0C5h, 0F6h, 0FFh, 0FFh
        db  08Bh, 07Ch, 01Eh, 060h, 00Fh, 0B7h, 055h, 014h, 0E8h, 0A7h, 001h, 000h, 000h, 00Fh, 082h, 0D6h
        db  0FDh, 0FFh, 0FFh, 075h, 007h, 0B0h, 0AAh, 0E9h, 0A9h, 0F6h, 0FFh, 0FFh, 066h, 083h, 07Ah, 00Ah
        db  000h, 00Fh, 084h, 0C0h, 0FDh, 0FFh, 0FFh, 083h, 0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h
        db  001h, 000h, 0CDh, 020h, 082h, 000h, 001h, 000h, 08Bh, 04Bh, 008h, 00Fh, 0B7h, 042h, 008h, 066h
        db  089h, 041h, 014h, 0C6h, 041h, 01Dh, 00Eh, 0A1h, 09Ch, 012h, 000h, 000h, 0E8h, 01Dh, 0FCh, 0F0h
        db  0FFh, 0CDh, 020h, 085h, 000h, 001h, 000h, 00Fh, 0B7h, 041h, 010h, 00Fh, 0B7h, 049h, 01Ch, 0CDh
        db  020h, 086h, 000h, 001h, 000h, 08Bh, 0F4h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 083h, 0C4h, 06Ch
        db  083h, 0F9h, 001h, 00Fh, 085h, 04Ch, 0F6h, 0FFh, 0FFh, 00Fh, 0B6h, 0C4h, 066h, 03Bh, 042h, 00Ah
        db  076h, 093h, 0E9h, 060h, 0FDh, 0FFh, 0FFh
loc_15_0A12:
        mov    edi, dword ptr [esi + ebx + 60h]
        movzx  edx, word ptr [ebp + 14h]
        call   sub_15_0B3F
        jb     NEAR PTR loc_15_0774
        jne    SHORT loc_15_0A7B
        mov    byte ptr [ebp + 11h], 0
        mov    eax, dword ptr [edx + 0ch]
        test   byte ptr [ebp + 1dh], 80h
        je     SHORT loc_15_0A39
        mov    dword ptr [ebp + 14h], eax
        jmp    SHORT loc_15_0A48
loc_15_0A39:
        cmp    eax, 10000h
        jae    NEAR PTR loc_15_0774
        mov    word ptr [ebp + 14h], ax
loc_15_0A48:
        xor    edx, edx
        lea    esi, [edi + 1ch]
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_15_0A5E
loc_15_0A55:
        cld
        lodsd
        or     eax, eax
        jle    SHORT loc_15_0A5C
        inc    edx
loc_15_0A5C:
        loop   loc_15_0A55
loc_15_0A5E:
        test   byte ptr [ebp + 1dh], 80h
        je     SHORT loc_15_0A6A
        mov    word ptr [ebp + 18h], dx
        jmp    SHORT loc_15_0A73
loc_15_0A6A:
        or     dh, dh
        je     SHORT loc_15_0A70
        mov    dl, 0ffh
loc_15_0A70:
        mov    byte ptr [ebp + 10h], dl
loc_15_0A73:
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_15_0A7B:
        cmp    word ptr [edx + 0ah], 0
        je     NEAR PTR loc_15_0772
        mov    eax, dword ptr [ebp + 1ch]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        mov    esi, dword ptr [ebp + 14h]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    ecx, dword ptr [ebx + 8]
        mov    si, word ptr [edx + 8]
        mov    dword ptr [ecx + 14h], esi
        mov    dword ptr [ecx + 1ch], eax
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 041h, 0FBh, 0F0h, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [ecx + 1ch], 1
        lahf
        movzx  esi, word ptr [ecx + 10h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    di, word ptr [ecx + 18h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        sahf
        mov    eax, esi
        jne    NEAR PTR loc_15_0050
        mov    dword ptr [ebp + 14h], ecx
        movzx  eax, ah
        sub    ax, word ptr [edx + 0ah]
        jae    SHORT loc_15_0AFB
        xor    al, al
loc_15_0AFB:
        mov    byte ptr [ebp + 11h], al
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_15_0B06:
        mov    edi, dword ptr [esi + ebx + 60h]
        movzx  edx, word ptr [ebp + 14h]
        call   sub_15_0B3F
        jb     NEAR PTR loc_15_0774
        jne    NEAR PTR loc_15_070E
        mov    esi, edx
        mov    eax, dword ptr [ebp + 10h]
        test   byte ptr [ebp + 1dh], 80h
        jne    SHORT loc_15_0B2F
        and    eax, 0ffffh
loc_15_0B2F:
        call   sub_15_0499
        jae    NEAR PTR loc_15_0319
        jmp    NEAR PTR loc_15_06C0
sub_15_0B3F:
        push   eax
        push   ecx
        push   esi
        lea    esi, [edi + 1ch]
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_15_0B69
loc_15_0B4D:
        cld
        lodsd
        cmp    eax, edx
        je     NEAR PTR loc_15_0C04
        or     eax, eax
        jns    SHORT loc_15_0B67
        cmp    edx, dword ptr [eax + 10h]
        jne    SHORT loc_15_0B67
        mov    edx, eax
        jmp    NEAR PTR loc_15_0C71
loc_15_0B67:
        loop   loc_15_0B4D
loc_15_0B69:
        db  0A1h, 0B4h, 012h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 012h, 000h, 000h
        xor    esi, esi
        jecxz  loc_15_0B9B
loc_15_0B78:
        test   byte ptr [eax + 0ch], 1
        jne    SHORT loc_15_0B82
        mov    esi, eax
        jmp    SHORT loc_15_0B88
loc_15_0B82:
        cmp    dx, word ptr [eax + 8]
        je     SHORT loc_15_0B8F
loc_15_0B88:
        add    eax, 10h
        loop   loc_15_0B78
        jmp    SHORT loc_15_0B9B
loc_15_0B8F:
        test   byte ptr [eax + 0ch], 4
        je     NEAR PTR loc_15_0C6C
        mov    esi, eax
loc_15_0B9B:
        or     esi, esi
        je     SHORT loc_15_0C04
        db  083h, 03Dh, 07Ch, 012h, 000h, 000h, 000h
        je     SHORT loc_15_0C07
        test   byte ptr [esi + 0ch], 4
        jne    SHORT loc_15_0C07
        mov    eax, esi
        db  08Bh, 00Dh, 07Ch, 012h, 000h, 000h
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        db  003h, 035h, 080h, 012h, 000h, 000h
loc_15_0BC2:
        cmp    dx, word ptr [esi]
        jne    SHORT loc_15_0BFF
        test   byte ptr [esi + 2], 1
        jne    SHORT loc_15_0BFF
        or     byte ptr [eax + 0ch], 3
        mov    word ptr [eax + 8], dx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        shl    ecx, 0ah
        mov    edx, dword ptr [esi + 8]
        xchg   esi, eax
        push   0
        push   ecx
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_15_0C04
        mov    dword ptr [esi], eax
        mov    word ptr [esi + 0ah], 0
        xchg   esi, eax
        jmp    SHORT loc_15_0C6C
loc_15_0BFF:
        add    esi, 0ch
        loop   loc_15_0BC2
loc_15_0C04:
        stc
        jmp    SHORT loc_15_0C71
loc_15_0C07:
        push   esi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
        mov    word ptr [esi + 14h], dx
        mov    byte ptr [esi + 1dh], 0eh
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0C7h, 0F9h, 0F0h, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        movzx  ecx, word ptr [esi + 14h]
        movzx  esi, word ptr [esi + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    eax
        cmp    esi, 1
        jne    SHORT loc_15_0C04
        or     byte ptr [eax + 0ch], 1
        mov    word ptr [eax + 8], dx
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 0ah], 0
loc_15_0C6C:
        mov    edx, eax
        or     eax, 1
loc_15_0C71:
        pop    esi
        pop    ecx
        pop    eax
        ret
        db  024h, 056h, 038h
_PTEXT ends
        end
