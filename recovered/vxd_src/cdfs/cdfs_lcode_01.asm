; cdfs_lcode_01.asm -- CDFS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    ah, 15h
        jne    SHORT loc_1_004B
        movzx  eax, al
        cmp    eax, 11h
        jge    SHORT loc_1_003E
        push   dword ptr [ebp + 2ch]
        or     dword ptr [ebp + 2ch], 200h
        push   ebx
        push   ebp
        db  0FFh, 014h, 085h, 0E0h, 09Dh, 000h, 000h
        lea    esp, [esp + 8]
        pop    ecx
        and    dword ptr [ebp + 2ch], 0fffffdffh
        and    ecx, 200h
        or     dword ptr [ebp + 2ch], ecx
        cmp    eax, 1
        cmc
        ret
loc_1_003E:
        or     word ptr [ebp + 2ch], 1
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_1_004B:
        stc
        ret
CDFS_Control:
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 001h
        jne    SHORT loc_1_0062
        cmp    eax, 1bh
        db  00Fh, 084h, 04Eh, 000h, 0FFh, 0FFh
        clc
        ret
loc_1_0061:
        clc
loc_1_0062:
        ret
sub_1_0063:
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
sub_1_0069:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
loc_1_006F:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 2
        ja     SHORT loc_1_007B
        xor    eax, eax
        ret
loc_1_007B:
        mov    eax, 1
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0090:
        push   50h
        mov    eax, dword ptr [esp + 8]
        push   2
        push   eax
        db  0E8h, 042h, 055h, 0FFh, 0FFh
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_00B0:
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 001h
        je     SHORT loc_1_00C6
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
loc_1_00C6:
        push   3bh
        mov    eax, dword ptr [esp + 8]
        push   1
        push   eax
        db  0E8h, 00Ch, 055h, 0FFh, 0FFh
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_00E0:
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 001h
        mov    eax, dword ptr [esp + 4]
        jne    SHORT loc_1_0101
        cmp    byte ptr [eax + 4], 2
        jne    SHORT loc_1_0101
        push   34h
        push   3
        push   eax
        db  0E8h, 0E3h, 054h, 0FFh, 0FFh
        add    esp, 0ch
        ret
loc_1_0101:
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0110:
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 001h
        jne    SHORT loc_1_0130
        push   34h
        mov    eax, dword ptr [esp + 8]
        push   3
        push   eax
        db  0E8h, 0B9h, 054h, 0FFh, 0FFh
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0130:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
sub_1_0140:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        sub    esi, esi
        db  066h, 039h, 035h, 09Ch, 008h, 000h, 000h
        jne    SHORT loc_1_0165
        push   1
        db  0A1h, 098h, 008h, 000h, 000h
        push   eax
        db  0E8h, 0B3h, 085h, 0FFh, 0FFh
        add    esp, 8
        mov    esi, 1
loc_1_0165:
        db  066h, 083h, 03Dh, 09Ch, 008h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0309
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 080h
        je     SHORT loc_1_01A4
        mov    ebx, 80h
loc_1_0181:
        db  0FEh, 005h, 0A0h, 008h, 000h, 000h
        push   1
        db  068h, 0D0h, 003h, 000h, 000h
        call   sub_1_0063
        add    esp, 8
        db  0FEh, 00Dh, 0A0h, 008h, 000h, 000h
        db  084h, 01Dh, 0A4h, 008h, 000h, 000h
        jne    SHORT loc_1_0181
loc_1_01A4:
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 010h
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 004h
        jne    NEAR PTR loc_1_0309
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 001h
        jne    NEAR PTR loc_1_0287
        db  0B8h, 0C0h, 09Bh, 000h, 000h
        db  03Dh, 0E0h, 057h, 000h, 000h
        je     NEAR PTR loc_1_0287
        db  0B8h, 0E0h, 057h, 000h, 000h
        shr    eax, 0ch
        push   0
        db  0B9h, 0C0h, 09Bh, 000h, 000h
        add    ecx, 1000h
        shr    ecx, 0ch
        sub    ecx, eax
        push   ecx
        push   eax
        db  0E8h, 05Ah, 04Fh, 0FFh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_0280
loc_1_0201:
        mov    edi, 8
loc_1_0206:
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 001h
        je     SHORT loc_1_0233
        db  0B8h, 0E0h, 057h, 000h, 000h
        shr    eax, 0ch
        push   0
        db  0B9h, 0C0h, 09Bh, 000h, 000h
        add    ecx, 1000h
        shr    ecx, 0ch
        sub    ecx, eax
        push   ecx
        push   eax
        db  0E8h, 050h, 04Fh, 0FFh, 0FFh
        add    esp, 0ch
loc_1_0233:
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 002h
        je     SHORT loc_1_0260
        db  0B8h, 0C0h, 09Bh, 000h, 000h
        shr    eax, 0ch
        push   0
        db  0B9h, 034h, 0A3h, 000h, 000h
        add    ecx, 1000h
        shr    ecx, 0ch
        sub    ecx, eax
        push   ecx
        push   eax
        db  0E8h, 023h, 04Fh, 0FFh, 0FFh
        add    esp, 0ch
loc_1_0260:
        db  080h, 025h, 0A4h, 008h, 000h, 000h, 0FCh
loc_1_0267:
        or     esi, esi
        je     SHORT loc_1_0279
        db  0A1h, 098h, 008h, 000h, 000h
        push   eax
        db  0E8h, 08Ah, 084h, 0FFh, 0FFh
        add    esp, 4
loc_1_0279:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0280:
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 001h
loc_1_0287:
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 002h
        jne    SHORT loc_1_02CB
        db  0B8h, 0C0h, 09Bh, 000h, 000h
        db  03Dh, 034h, 0A3h, 000h, 000h
        je     SHORT loc_1_02CB
        shr    eax, 0ch
        push   0
        db  0B9h, 034h, 0A3h, 000h, 000h
        add    ecx, 1000h
        shr    ecx, 0ch
        sub    ecx, eax
        push   ecx
        push   eax
        db  0E8h, 098h, 04Eh, 0FFh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_02C4
        jmp    NEAR PTR loc_1_0201
loc_1_02C4:
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 002h
loc_1_02CB:
        mov    eax, 1
        db  08Bh, 00Dh, 0D8h, 09Bh, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_02F4
        push   0
        mov    eax, dword ptr [ecx + 44h]
        add    eax, 0fffh
        shr    eax, 0ch
        push   eax
        shr    ecx, 0ch
        push   ecx
        db  0E8h, 05Fh, 04Eh, 0FFh, 0FFh
        add    esp, 0ch
loc_1_02F4:
        or     eax, eax
        jne    SHORT loc_1_0300
        jmp    NEAR PTR loc_1_0201
        db  08Dh, 049h, 000h
loc_1_0300:
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 004h
        sub    edi, edi
loc_1_0309:
        or     edi, edi
        jne    NEAR PTR loc_1_0206
        db  066h, 0FFh, 005h, 09Ch, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_0267
        db  08Dh, 049h, 000h
sub_1_0320:
        db  066h, 0A1h, 09Ch, 008h, 000h, 000h
        dec    ax
        db  066h, 0A3h, 09Ch, 008h, 000h, 000h
        jne    SHORT loc_1_0337
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 020h
loc_1_0337:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0340:
        db  066h, 0FFh, 005h, 09Ch, 008h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0350:
        sub    eax, eax
        db  066h, 039h, 005h, 09Ch, 008h, 000h, 000h
        jne    SHORT loc_1_03CC
        db  08Ah, 00Dh, 0A4h, 008h, 000h, 000h
        and    cl, 70h
        cmp    cl, 20h
        jne    SHORT loc_1_03A2
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 040h
        db  081h, 025h, 004h, 000h, 000h, 000h, 0FFh, 0EFh, 0FFh, 0FFh
        db  0F6h, 005h, 005h, 000h, 000h, 000h, 020h
        jne    SHORT loc_1_039B
        db  081h, 00Dh, 004h, 000h, 000h, 000h, 000h, 020h, 000h, 000h
        push   eax
        db  068h, 078h, 000h, 000h, 000h
        call   sub_1_07F0
        add    esp, 8
loc_1_039B:
        mov    eax, 1
        jmp    SHORT loc_1_03B2
loc_1_03A2:
        db  0F6h, 005h, 0A4h, 008h, 000h, 000h, 010h
        je     SHORT loc_1_03B2
        db  080h, 025h, 0A4h, 008h, 000h, 000h, 0EFh
loc_1_03B2:
        db  080h, 03Dh, 0A0h, 008h, 000h, 000h, 000h
        je     SHORT loc_1_03CC
        or     eax, eax
        je     SHORT loc_1_03CC
        db  068h, 0D0h, 003h, 000h, 000h
        call   sub_1_0069
        add    esp, 4
loc_1_03CC:
        ret
        db  08Dh, 049h, 000h
sub_1_03D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 0D8h, 09Bh, 000h, 000h
        db  066h, 083h, 03Dh, 09Ch, 008h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_052F
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 001h
        jne    NEAR PTR loc_1_052F
        db  0A0h, 0A4h, 008h, 000h, 000h
        and    al, 14h
        cmp    al, 4
        jne    NEAR PTR loc_1_052F
        db  080h, 00Dh, 0A4h, 008h, 000h, 000h, 080h
        db  0B8h, 0E0h, 057h, 000h, 000h
        shr    eax, 0ch
        push   0
        db  0B9h, 0C0h, 09Bh, 000h, 000h
        add    ecx, 1000h
        shr    ecx, 0ch
        sub    ecx, eax
        push   ecx
        push   eax
        db  0E8h, 054h, 04Dh, 0FFh, 0FFh
        add    esp, 0ch
        db  0B8h, 0C0h, 09Bh, 000h, 000h
        shr    eax, 0ch
        push   0
        db  0B9h, 034h, 0A3h, 000h, 000h
        add    ecx, 1000h
        shr    ecx, 0ch
        sub    ecx, eax
        push   ecx
        push   eax
        db  0E8h, 030h, 04Dh, 0FFh, 0FFh
        add    esp, 0ch
        db  080h, 025h, 0A4h, 008h, 000h, 000h, 0FCh
        or     esi, esi
        je     SHORT loc_1_04C9
        sub    ebx, ebx
        cmp    dword ptr [esi + 64h], ebx
        jbe    SHORT loc_1_04AF
        sub    edi, edi
        sub    ebp, ebp
loc_1_0469:
        mov    eax, dword ptr [esi + 4]
        add    eax, edi
        cmp    word ptr [eax + 4], bp
        jne    SHORT loc_1_04A6
        cmp    word ptr [eax + 6], bp
        je     SHORT loc_1_04A0
loc_1_047A:
        push   ebp
        push   1
        mov    eax, dword ptr [esi + 68h]
        shr    eax, 0ch
        add    eax, ebx
        push   eax
        db  0E8h, 0F5h, 04Ch, 0FFh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi + 4]
        dec    word ptr [edi + eax + 6]
        mov    eax, dword ptr [esi + 4]
        cmp    word ptr [edi + eax + 6], bp
        jne    SHORT loc_1_047A
loc_1_04A0:
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + eax], ebp
loc_1_04A6:
        add    edi, 8
        inc    ebx
        cmp    dword ptr [esi + 64h], ebx
        ja     SHORT loc_1_0469
loc_1_04AF:
        push   0
        mov    eax, dword ptr [esi + 44h]
        add    eax, 0fffh
        shr    eax, 0ch
        push   eax
        shr    esi, 0ch
        push   esi
        db  0E8h, 0BAh, 04Ch, 0FFh, 0FFh
        add    esp, 0ch
loc_1_04C9:
        db  0A1h, 0DCh, 09Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_04FB
        push   0
        mov    ecx, dword ptr [eax + 64h]
        push   ecx
        mov    eax, dword ptr [eax + 68h]
        shr    eax, 0ch
        push   eax
        db  0E8h, 09Ch, 04Ch, 0FFh, 0FFh
        add    esp, 0ch
        db  0A1h, 0DCh, 09Bh, 000h, 000h
        db  0A3h, 0E0h, 09Bh, 000h, 000h
        db  0C7h, 005h, 0DCh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_04FB:
        db  068h, 08Ch, 09Ch, 000h, 000h
        db  0E8h, 01Bh, 063h, 0FFh, 0FFh
        add    esp, 4
        db  0A1h, 014h, 09Ch, 000h, 000h
        db  0C7h, 005h, 014h, 09Ch, 000h, 000h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0528
        push   8
        push   0
        push   eax
        db  0E8h, 02Bh, 082h, 0FFh, 0FFh
        add    esp, 0ch
loc_1_0528:
        db  080h, 025h, 0A4h, 008h, 000h, 000h, 05Bh
loc_1_052F:
        db  0A1h, 0F8h, 09Bh, 000h, 000h
        and    eax, 0fffffffch
        db  0A3h, 0F8h, 09Bh, 000h, 000h
        db  080h, 025h, 0A4h, 008h, 000h, 000h, 0BFh
        db  080h, 03Dh, 0A0h, 008h, 000h, 000h, 000h
        je     SHORT loc_1_0559
        db  068h, 0D0h, 003h, 000h, 000h
        call   sub_1_0069
        add    esp, 4
loc_1_0559:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_0560:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        xor    eax, eax
        mov    word ptr [esi + 2], 0ffffh
        mov    ax, word ptr [esi]
        cmp    eax, 0eh
        ja     NEAR PTR loc_1_0769
        db  0FFh, 024h, 085h, 074h, 007h, 000h, 000h
loc_1_0586:
        mov    word ptr [esi + 2], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0594:
        db  066h, 083h, 03Dh, 09Ch, 008h, 000h, 000h, 000h
        jne    SHORT loc_1_05F9
        db  080h, 03Dh, 0A0h, 008h, 000h, 000h, 000h
        jne    SHORT loc_1_05F9
        db  08Bh, 015h, 090h, 008h, 000h, 000h
        add    edx, 14h
        sub    eax, eax
        inc    edx
loc_1_05B3:
        mov    cl, byte ptr [edx]
        and    cl, 18h
        cmp    cl, 18h
        je     SHORT loc_1_05C8
        add    edx, 6
        inc    eax
        cmp    eax, 1ah
        jb     SHORT loc_1_05B3
        jmp    SHORT loc_1_05CE
loc_1_05C8:
        mov    word ptr [esi + 2], 0
loc_1_05CE:
        cmp    word ptr [esi + 2], 0
        je     NEAR PTR loc_1_0769
        db  080h, 025h, 07Ch, 008h, 000h, 000h, 07Fh
        db  0E8h, 01Bh, 0FAh, 0FEh, 0FFh
        db  0A0h, 07Ch, 008h, 000h, 000h
        and    al, 0efh
        db  0A2h, 07Ch, 008h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_05F9:
        mov    word ptr [esi + 2], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0607:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 10h], eax
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 010h
        je     NEAR PTR loc_1_06BC
        cmp    byte ptr [eax + 40h], 5
        jne    NEAR PTR loc_1_06BC
        test   byte ptr [eax + 21h], 40h
        je     NEAR PTR loc_1_06BC
        db  08Bh, 015h, 090h, 008h, 000h, 000h
        sub    ebx, ebx
        lea    edi, [edx + 14h]
        db  08Bh, 00Dh, 074h, 008h, 000h, 000h
loc_1_0640:
        lea    eax, [ebx + ebx*2]
        lea    ebp, [edi + eax*2]
        mov    al, byte ptr [ebp + 1]
        and    al, 18h
        cmp    al, 18h
        jne    SHORT loc_1_0660
        xor    eax, eax
        mov    al, byte ptr [ebp]
        cmp    eax, ecx
        jne    SHORT loc_1_0660
        inc    ecx
        sub    ebx, ebx
        jmp    SHORT loc_1_0661
        db  08Dh, 049h, 000h
loc_1_0660:
        inc    ebx
loc_1_0661:
        cmp    ebx, 1ah
        jb     SHORT loc_1_0640
        mov    eax, dword ptr [esp + 10h]
        db  089h, 00Dh, 074h, 008h, 000h, 000h
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        lea    eax, [eax + eax*2]
        lea    edx, [edx + eax*2 + 14h]
        db  0A1h, 08Ch, 008h, 000h, 000h
        mov    dword ptr [edx + 2], eax
        mov    byte ptr [edx], cl
        db  0A0h, 078h, 008h, 000h, 000h
        or     al, 38h
        mov    byte ptr [edx + 1], al
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 080h
        db  0A1h, 088h, 008h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_06B7
        db  089h, 00Dh, 074h, 008h, 000h, 000h
        xor    edx, edx
        mov    dl, byte ptr [eax + 15h]
        cmp    edx, ecx
        ja     SHORT loc_1_06B7
        inc    cl
        mov    byte ptr [eax + 15h], cl
loc_1_06B7:
        call   sub_1_07B0
loc_1_06BC:
        mov    word ptr [esi + 2], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_06D0:
        mov    eax, dword ptr [esi + 0ch]
        cmp    byte ptr [eax + 40h], 5
        jne    NEAR PTR loc_1_0769
        test   byte ptr [eax + 21h], 40h
        je     NEAR PTR loc_1_0769
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        db  08Bh, 015h, 090h, 008h, 000h, 000h
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2 + 14h]
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax], 0
        mov    dword ptr [eax + 2], 0
        db  0C7h, 005h, 074h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2], 0
        call   sub_1_07B0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0727:
        db  080h, 00Dh, 084h, 008h, 000h, 000h, 001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0736:
        db  08Bh, 00Dh, 070h, 008h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_075B
loc_1_0740:
        mov    eax, dword ptr [ecx + 0ch]
        db  0A3h, 070h, 008h, 000h, 000h
        push   ecx
        db  0E8h, 032h, 07Eh, 0FFh, 0FFh
        add    esp, 4
        db  08Bh, 00Dh, 070h, 008h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_0740
loc_1_075B:
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 080h
        je     SHORT loc_1_0769
        call   sub_1_0350
loc_1_0769:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h, 086h, 005h, 000h, 000h, 069h, 007h, 000h, 000h, 094h, 005h, 000h, 000h, 007h
        db  006h, 000h, 000h, 0D0h, 006h, 000h, 000h, 069h, 007h, 000h, 000h, 069h, 007h, 000h, 000h, 069h
        db  007h, 000h, 000h, 036h, 007h, 000h, 000h, 069h, 007h, 000h, 000h, 069h, 007h, 000h, 000h, 069h
        db  007h, 000h, 000h, 069h, 007h, 000h, 000h, 069h, 007h, 000h, 000h, 027h, 007h, 000h, 000h
sub_1_07B0:
        push   ebx
        sub    edx, edx
        db  08Bh, 00Dh, 090h, 008h, 000h, 000h
        add    ecx, 14h
        sub    eax, eax
        inc    ecx
        nop
loc_1_07C0:
        mov    bl, byte ptr [ecx]
        and    bl, 18h
        cmp    bl, 18h
        je     SHORT loc_1_07D5
        add    ecx, 6
        inc    eax
        cmp    eax, 1ah
        jb     SHORT loc_1_07C0
        jmp    SHORT loc_1_07D7
loc_1_07D5:
        mov    edx, eax
loc_1_07D7:
        db  0A1h, 088h, 008h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_07E5
        inc    dl
        mov    byte ptr [eax + 14h], dl
loc_1_07E5:
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 000h, 000h, 000h, 043h, 044h, 046h, 053h
        db  020h, 020h, 020h, 020h, 000h, 001h, 001h, 0A0h, 04Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 007h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_08A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_08B8:
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
loc_1_08DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    al, byte ptr [eax]
        outsd
        add    byte ptr [eax], al
        db  000h, 030h
        sbb    dword ptr [eax], eax
        db  000h, 061h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0BCh, 031h, 000h, 000h, 0C7h
        xor    dword ptr [eax], eax
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        db  008h, 080h, 03Bh, 000h, 000h, 0E0h
        add    byte ptr [eax], al
        db  000h, 0A0h, 01Ah, 000h, 000h, 0B0h
        sbb    al, byte ptr [eax]
        db  000h, 0A0h, 03Bh, 000h, 000h, 040h
        cmp    al, 0
        db  000h, 040h, 03Ch
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], bh
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        db  008h, 040h, 03Ch
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        db  000h, 040h, 03Ch
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        db  000h, 040h, 03Ch
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        db  000h, 040h, 03Ch
        add    byte ptr [eax], al
loc_1_0944:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        sbb    byte ptr [edx], 0
        db  000h, 0A0h, 03Ah, 000h, 000h, 0D0h
        cmp    al, byte ptr [eax]
        db  000h, 090h, 000h, 000h, 000h, 090h
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        db  000h, 000h
        dec    edx
        add    byte ptr [eax], al
        db  020h, 005h, 000h, 000h, 0B0h, 000h
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        db  000h, 090h, 01Ah, 000h, 000h, 040h
        cmp    al, 0
        db  000h, 020h
        dec    edx
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 57h], dl
        mov    esi, dword ptr [esp + 0ch]
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 14h
        jg     SHORT loc_1_09C3
        je     NEAR PTR loc_1_0A9C
        sub    eax, 3
        cmp    eax, 0dh
        ja     NEAR PTR loc_1_0A9C
        shl    eax, 2
        db  0FFh, 0A0h, 0A4h, 00Ah, 000h, 000h
loc_1_09C3:
        cmp    eax, 18h
        je     NEAR PTR loc_1_0A9C
        cmp    eax, 80h
        jl     NEAR PTR loc_1_0A9C
        cmp    eax, 88h
        jle    NEAR PTR loc_1_0A8A
        mov    eax, edi
        pop    edi
        pop    esi
        ret
loc_1_09E7:
        mov    eax, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 0eh]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 061h, 04Fh, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    ax, word ptr [eax + 12h]
        and    eax, 0ffffh
        dec    eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esi + 14h]
        cmp    byte ptr [eax + 2], 3
        mov    eax, dword ptr [esi + 18h]
        mov    al, byte ptr [eax]
        jne    SHORT loc_1_0A42
        and    eax, 0ffh
        or     eax, eax
        jl     SHORT loc_1_0A9C
        cmp    eax, 0fh
        jg     SHORT loc_1_0A9C
        mov    eax, dword ptr [esi + 18h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Bh, 03Ch, 085h, 024h, 09Eh, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        ret
loc_1_0A42:
        and    eax, 0ffh
        or     eax, eax
        jl     SHORT loc_1_0A9C
        cmp    eax, 5
        jg     SHORT loc_1_0A9C
        mov    eax, dword ptr [esi + 18h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Bh, 03Ch, 085h, 064h, 09Eh, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        ret
loc_1_0A66:
        db  0BFh, 080h, 00Bh, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        ret
loc_1_0A70:
        db  0BFh, 0B0h, 00Bh, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0A80:
        db  0BFh, 0B0h, 00Bh, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        ret
loc_1_0A8A:
        mov    eax, dword ptr [esi + 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Bh, 03Ch, 085h, 07Ch, 09Ch, 000h, 000h
loc_1_0A9C:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_0AA4:
        out    9, eax
        add    byte ptr [eax], al
        pushfd
        or     al, byte ptr [eax]
        db  000h, 09Ch, 00Ah, 000h, 000h, 09Ch, 00Ah
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        db  000h, 09Ch, 00Ah, 000h, 000h, 09Ch, 00Ah
        add    byte ptr [eax], al
        pushfd
        or     al, byte ptr [eax]
        db  000h, 09Ch, 00Ah, 000h, 000h, 0E7h, 009h
        add    byte ptr [eax], al
        db  070h, 00Ah
        add    byte ptr [eax], al
        or     byte ptr [edx], 0
        db  000h, 09Ch, 00Ah, 000h, 000h, 09Ch, 00Ah
        add    byte ptr [eax], al
        lea    esp, [esp]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
