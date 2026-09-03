; vtcp_icode_02.asm -- VTCP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0FEh, 005h, 0E8h, 092h, 000h, 000h
        jne    SHORT loc_2_0024
        db  0BFh, 0B8h, 094h, 000h, 000h
        db  0B9h, 060h, 097h, 000h, 000h
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld
        rep stosd
        call   sub_2_0038
        cmp    eax, 1
loc_2_0024:
        ret
sub_2_0025:
        db  0CDh, 020h, 000h, 000h, 089h, 004h
        sbb    al, al
        not    al
        ret
sub_2_0030:
        db  0CDh, 020h, 001h, 080h, 089h, 004h
        add    byte ptr [eax], al
sub_2_0038:
        sub    esp, 80h
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        call   sub_2_06E8
        cmp    eax, esi
        jne    SHORT loc_2_0058
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
loc_2_0058:
        call   sub_2_0025
        or     al, al
        jne    SHORT loc_2_0070
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
        db  08Dh, 049h, 000h
loc_2_0070:
        db  0E8h, 09Bh, 01Fh, 0FFh, 0FFh
        or     al, al
        jne    SHORT loc_2_0088
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
        db  08Dh, 049h, 000h
loc_2_0088:
        db  0E8h, 03Bh, 021h, 0FFh, 0FFh
        db  0A3h, 0B8h, 094h, 000h, 000h
        mov    ecx, 64h
        db  0A1h, 01Ch, 096h, 000h, 000h
        sub    edx, edx
        div    ecx
        db  0A3h, 01Ch, 096h, 000h, 000h
        db  0A1h, 0FCh, 095h, 000h, 000h
        sub    edx, edx
        div    ecx
        db  0A3h, 0FCh, 095h, 000h, 000h
        db  0E8h, 04Ch, 021h, 0FFh, 0FFh
        push   48h
        db  068h, 090h, 095h, 000h, 000h
        call   sub_2_0030
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_2_0394
        db  083h, 03Dh, 090h, 095h, 000h, 000h, 001h
        jne    NEAR PTR loc_2_0394
        push   1
        db  068h, 064h, 001h, 000h, 000h
        db  0E8h, 030h, 0FFh, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_0394
        call   sub_2_03D8
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0E8h, 0FFh, 020h, 0FFh, 0FFh
        call   sub_2_0470
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0A1h, 0E8h, 095h, 000h, 000h
        sub    eax, 0fffffeh
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0fffffeh
        db  0A3h, 0E8h, 095h, 000h, 000h
        db  0E8h, 0D5h, 020h, 0FFh, 0FFh
        call   sub_2_05C4
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0E8h, 0C3h, 020h, 0FFh, 0FFh
        call   sub_2_0560
        or     eax, eax
        je     NEAR PTR loc_2_0394
        mov    esi, 1
        db  0E8h, 0ACh, 020h, 0FFh, 0FFh
        call   sub_2_04E8
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0E8h, 09Ah, 020h, 0FFh, 0FFh
        call   sub_2_0618
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0BFh, 040h, 095h, 000h, 000h
        sub    eax, eax
        mov    ecx, 0fh
        rep stosd
        db  0C7h, 005h, 040h, 095h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 095h, 000h, 000h, 02Ch, 001h, 000h, 000h
        db  0C7h, 005h, 048h, 095h, 000h, 000h, 080h, 0A9h, 003h, 000h
        db  0C7h, 005h, 04Ch, 095h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0BFh, 07Ch, 095h, 000h, 000h
        stosd
        stosd
        stosd
        stosd
        stosd
        push   eax
        db  068h, 0F8h, 01Ch, 000h, 000h
        db  068h, 0E0h, 014h, 000h, 000h
        db  068h, 040h, 01Bh, 000h, 000h
        push   11h
        db  0E8h, 032h, 0FEh, 0FDh, 0FFh
        add    esp, 14h
        db  0A3h, 0E4h, 095h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0C7h, 005h, 0A0h, 096h, 000h, 000h, 000h, 008h, 000h, 000h
        db  0C7h, 005h, 0A4h, 096h, 000h, 000h, 0D8h, 00Bh, 000h, 000h
        db  0C7h, 005h, 0D0h, 096h, 000h, 000h, 084h, 017h, 000h, 000h
        db  0C7h, 005h, 0D4h, 096h, 000h, 000h, 0CCh, 018h, 000h, 000h
        db  0C7h, 005h, 0D8h, 096h, 000h, 000h, 048h, 010h, 000h, 000h
        db  0C7h, 005h, 0A8h, 096h, 000h, 000h, 004h, 064h, 000h, 000h
        db  0C7h, 005h, 0ACh, 096h, 000h, 000h, 0DCh, 064h, 000h, 000h
        db  0C7h, 005h, 0B0h, 096h, 000h, 000h, 0B0h, 065h, 000h, 000h
        db  0C7h, 005h, 0B4h, 096h, 000h, 000h, 00Ch, 066h, 000h, 000h
        db  0C7h, 005h, 0B8h, 096h, 000h, 000h, 0ACh, 067h, 000h, 000h
        db  0C7h, 005h, 0BCh, 096h, 000h, 000h, 0CCh, 06Dh, 000h, 000h
        db  0C7h, 005h, 0C0h, 096h, 000h, 000h, 084h, 06Ah, 000h, 000h
        db  0C7h, 005h, 0C4h, 096h, 000h, 000h, 0D8h, 06Ch, 000h, 000h
        db  0C7h, 005h, 0C8h, 096h, 000h, 000h, 074h, 051h, 000h, 000h
        db  0C7h, 005h, 0CCh, 096h, 000h, 000h, 0D8h, 08Dh, 000h, 000h
        db  0C7h, 005h, 0DCh, 096h, 000h, 000h, 0D4h, 071h, 000h, 000h
        db  0C7h, 005h, 0E0h, 096h, 000h, 000h, 0E0h, 074h, 000h, 000h
        db  0C7h, 005h, 0E4h, 096h, 000h, 000h, 0E8h, 074h, 000h, 000h
        db  0C7h, 005h, 0E8h, 096h, 000h, 000h, 0F0h, 074h, 000h, 000h
        db  0C7h, 005h, 0ECh, 096h, 000h, 000h, 078h, 078h, 000h, 000h
        db  068h, 0A0h, 096h, 000h, 000h
        db  068h, 0ECh, 092h, 000h, 000h
        db  0E8h, 057h, 0FDh, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_0394
        db  0E8h, 03Fh, 01Fh, 0FFh, 0FFh
        push   100h
        db  0E8h, 01Dh, 01Fh, 0FFh, 0FFh
        add    esp, 4
        db  0A3h, 048h, 096h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0394
        mov    dword ptr [eax], 400h
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 401h
        mov    dword ptr [eax + 0ch], 0
        db  0C7h, 005h, 03Ch, 097h, 000h, 000h, 002h, 000h, 000h, 000h
        db  068h, 03Ch, 097h, 000h, 000h
        push   eax
        db  0FFh, 015h, 0D0h, 095h, 000h, 000h
        add    esp, 8
        db  0E8h, 0ECh, 01Eh, 0FFh, 0FFh
        db  0A1h, 004h, 096h, 000h, 000h
        sub    eax, 20h
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 20h
        db  0A3h, 004h, 096h, 000h, 000h
        mov    ebx, 0
        je     SHORT loc_2_035C
        inc    esi
        lea    esi, [esp + 0ch]
loc_2_0338:
        push   4000h
        db  0E8h, 0AAh, 01Eh, 0FFh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_035C
        mov    dword ptr [esi], eax
        db  0E8h, 0B4h, 01Eh, 0FFh, 0FFh
        add    esi, 4
        inc    ebx
        db  03Bh, 01Dh, 004h, 096h, 000h, 000h
        jb     SHORT loc_2_0338
loc_2_035C:
        sub    edi, edi
        db  089h, 01Dh, 004h, 096h, 000h, 000h
        cmp    ebx, edi
        je     SHORT loc_2_0382
        lea    esi, [esp + 0ch]
loc_2_036C:
        push   dword ptr [esi]
        db  0E8h, 085h, 01Eh, 0FFh, 0FFh
        add    esp, 4
        add    esi, 4
        inc    edi
        db  03Bh, 03Dh, 004h, 096h, 000h, 000h
        jb     SHORT loc_2_036C
loc_2_0382:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
        db  08Dh, 049h, 000h
loc_2_0394:
        db  083h, 03Dh, 0E4h, 095h, 000h, 000h, 000h
        je     SHORT loc_2_03AF
        push   0
        push   0
        push   0
        push   0
        push   11h
        db  0E8h, 054h, 0FCh, 0FDh, 0FFh
        add    esp, 14h
loc_2_03AF:
        call   sub_2_06BC
        call   sub_2_055C
        or     esi, esi
        je     SHORT loc_2_03C2
        call   sub_2_05B4
loc_2_03C2:
        call   sub_2_05CC
        db  0E8h, 038h, 01Eh, 0FFh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
sub_2_03D8:
        push   esi
        push   edi
        db  066h, 0C7h, 005h, 058h, 096h, 000h, 000h, 001h, 000h
        db  0C7h, 005h, 0F8h, 095h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 084h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        push   80h
        db  0E8h, 0EBh, 01Dh, 0FFh, 0FFh
        add    esp, 4
        db  0A3h, 020h, 096h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_2_0414
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_2_0414:
        push   78h
        db  0E8h, 0D1h, 01Dh, 0FFh, 0FFh
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_0438
        db  0FFh, 035h, 020h, 096h, 000h, 000h
        db  0E8h, 0C9h, 01Dh, 0FFh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
loc_2_0438:
        inc    edi
        mov    edi, 5
        db  0C7h, 005h, 09Ch, 096h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0448:
        push   esi
        db  0E8h, 03Ah, 013h, 0FEh, 0FFh
        add    esp, 4
        add    esi, 18h
        dec    edi
        jne    SHORT loc_2_0448
        db  0BFh, 0F8h, 096h, 000h, 000h
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_2_0470:
        db  0C7h, 005h, 094h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 057h, 020h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_04E4
        push   eax
        db  0E8h, 0BDh, 022h, 0FEh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 05Ch, 096h, 000h, 000h, 05Ch, 096h, 000h, 000h
        db  0C7h, 005h, 060h, 096h, 000h, 000h, 05Ch, 096h, 000h, 000h
        db  0C7h, 005h, 0ECh, 095h, 000h, 000h, 0ECh, 095h, 000h, 000h
        db  0C7h, 005h, 0F0h, 095h, 000h, 000h, 0ECh, 095h, 000h, 000h
        db  0C7h, 005h, 058h, 097h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 050h, 097h, 000h, 000h, 060h, 014h, 000h, 000h
        db  0A1h, 0B4h, 094h, 000h, 000h
        db  0A3h, 05Ch, 097h, 000h, 000h
        mov    eax, 1
        ret
        db  08Bh, 0FFh
loc_2_04E4:
        sub    eax, eax
        ret
        db  090h
sub_2_04E8:
        db  0C7h, 005h, 044h, 097h, 000h, 000h, 044h, 097h, 000h, 000h
        db  0C7h, 005h, 048h, 097h, 000h, 000h, 044h, 097h, 000h, 000h
        db  0C7h, 005h, 010h, 096h, 000h, 000h, 010h, 096h, 000h, 000h
        db  0C7h, 005h, 014h, 096h, 000h, 000h, 010h, 096h, 000h, 000h
        db  0C7h, 005h, 0DCh, 095h, 000h, 000h, 0DCh, 095h, 000h, 000h
        db  0C7h, 005h, 0E0h, 095h, 000h, 000h, 0DCh, 095h, 000h, 000h
        db  0C7h, 005h, 050h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 080h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 07Ch, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
sub_2_055C:
        ret
        db  08Dh, 049h, 000h
sub_2_0560:
        push   edi
        db  0BFh, 0BCh, 094h, 000h, 000h
        sub    eax, eax
        mov    ecx, 20h
        rep stosd
        db  0A3h, 018h, 096h, 000h, 000h
        db  0A3h, 06Ch, 096h, 000h, 000h
        db  0C7h, 005h, 03Ch, 095h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  0A3h, 038h, 096h, 000h, 000h
        db  0A3h, 040h, 096h, 000h, 000h
        db  0A1h, 0B4h, 094h, 000h, 000h
        db  0A3h, 03Ch, 096h, 000h, 000h
        push   0
        db  068h, 0B4h, 055h, 000h, 000h
        push   64h
        db  068h, 02Ch, 096h, 000h, 000h
        db  0E8h, 006h, 01Ch, 0FFh, 0FFh
        add    esp, 10h
        mov    eax, 1
        pop    edi
        ret
sub_2_05B4:
        db  068h, 02Ch, 096h, 000h, 000h
        db  0E8h, 0FEh, 01Bh, 0FFh, 0FFh
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_2_05C4:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
sub_2_05CC:
        ret
        db  08Dh, 049h, 000h
sub_2_05D0:
        push   esi
        db  08Bh, 035h, 08Ch, 096h, 000h, 000h
        db  0C7h, 005h, 08Ch, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_2_0614
        nop
loc_2_05F0:
        push   dword ptr [esi + 4]
        db  0E8h, 0D0h, 01Dh, 0FFh, 0FFh
        push   dword ptr [esi + 8]
        db  0E8h, 0F8h, 01Bh, 0FFh, 0FFh
        add    esp, 4
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        db  0E8h, 0EBh, 01Bh, 0FFh, 0FFh
        add    esp, 4
        or     esi, esi
        jne    SHORT loc_2_05F0
loc_2_0614:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_2_0618:
        sub    esp, 4
        db  0C7h, 005h, 08Ch, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 084h, 0F0h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_06B4
        push   eax
        db  0E8h, 032h, 0F3h, 0FEh, 0FFh
        add    esp, 4
        push   64h
        db  068h, 040h, 097h, 000h, 000h
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 06Ah, 01Dh, 0FFh, 0FFh
        cmp    dword ptr [esp], 0
        je     SHORT loc_2_0664
        call   sub_2_05D0
        sub    eax, eax
        add    esp, 4
        ret
loc_2_0664:
        db  068h, 0C8h, 023h, 000h, 000h
        db  068h, 008h, 085h, 000h, 000h
        db  068h, 024h, 07Dh, 000h, 000h
        db  068h, 0D0h, 02Eh, 000h, 000h
        push   6
        db  0E8h, 081h, 0F9h, 0FDh, 0FFh
        add    esp, 14h
        db  0A3h, 0D8h, 095h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_2_06A4
        call   sub_2_05D0
        db  0FFh, 035h, 040h, 097h, 000h, 000h
        db  0E8h, 02Dh, 01Dh, 0FFh, 0FFh
        sub    eax, eax
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_2_06A4:
        mov    eax, 1
        db  0C6h, 005h, 040h, 093h, 000h, 000h, 001h
        add    esp, 4
        ret
loc_2_06B4:
        sub    eax, eax
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_2_06BC:
        db  080h, 03Dh, 040h, 093h, 000h, 000h, 000h
        je     SHORT loc_2_06E7
        push   0
        push   0
        push   0
        push   0
        push   6
        db  0E8h, 02Ch, 0F9h, 0FDh, 0FFh
        add    esp, 14h
        call   sub_2_05D0
        db  0FFh, 035h, 040h, 097h, 000h, 000h
        db  0E8h, 0E1h, 01Ch, 0FFh, 0FFh
loc_2_06E7:
        ret
sub_2_06E8:
        db  0CDh, 020h, 000h, 000h, 088h, 004h
        jb     SHORT loc_2_0704
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_0704
        db  0CDh, 020h, 00Ah, 000h, 088h, 004h
        db  0A3h, 0B4h, 094h, 000h, 000h
        clc
loc_2_0704:
        cmc
        sbb    eax, eax
        ret
        db  048h, 04Ch, 053h, 020h
_ITEXT ends
        end
