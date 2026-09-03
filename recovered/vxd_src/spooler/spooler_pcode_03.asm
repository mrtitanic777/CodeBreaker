; spooler_pcode_03.asm -- SPOOLER object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   ebx
        push   ebp
        mov    ebp, dword ptr [esp + 0ch]
        mov    ebx, dword ptr [ebp + 1ch]
        mov    al, byte ptr [ebp + 4]
        mov    ecx, dword ptr [ebp + 28h]
        and    word ptr [ecx + 2ch], 0fffeh
        cmp    al, 0
        je     SHORT loc_3_0037
        cmp    al, 1
        je     SHORT loc_3_0042
        cmp    al, 6
        je     SHORT loc_3_0046
        cmp    al, 7
        je     SHORT loc_3_0051
        cmp    al, 10h
        je     SHORT loc_3_005C
        cmp    al, 0ah
        jne    SHORT loc_3_0068
        mov    dword ptr [ecx + 14h], 10e0h
        xor    eax, eax
        jmp    SHORT loc_3_0079
loc_3_0037:
        mov    dword ptr [ecx + 14h], 80e0h
        xor    eax, eax
        jmp    SHORT loc_3_0079
loc_3_0042:
        xor    eax, eax
        jmp    SHORT loc_3_0079
loc_3_0046:
        mov    dword ptr [ecx + 1ch], 0
        xor    eax, eax
        jmp    SHORT loc_3_0079
loc_3_0051:
        mov    dword ptr [ecx + 1ch], 0ffh
        xor    eax, eax
        jmp    SHORT loc_3_0079
loc_3_005C:
        mov    eax, dword ptr [ecx + 18h]
        cmp    ah, 5
        jne    SHORT loc_3_0068
        xor    eax, eax
        jmp    SHORT loc_3_0079
loc_3_0068:
        or     word ptr [ecx + 2ch], 1
        mov    dword ptr [ecx + 1ch], 1
        mov    eax, 1fh
loc_3_0079:
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    ebx
        ret
loc_3_0080:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        db  080h, 03Dh, 0DBh, 002h, 000h, 000h, 001h
        jne    SHORT loc_3_0109
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_0102
        mov    esi, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_3_00AD
        cmp    esi, ebx
        je     SHORT loc_3_0102
loc_3_00AD:
        mov    ebx, dword ptr [ebp + 28h]
        cmp    ebx, 9
        jae    SHORT loc_3_0109
        cmp    byte ptr [ebp + 4], 0
        jne    SHORT loc_3_0109
        db  08Bh, 01Ch, 09Dh, 04Dh, 000h, 000h, 000h
        test   dword ptr [ebx + 10h], 1
        jne    SHORT loc_3_00EA
        mov    eax, dword ptr [ebx + 14h]
        db  085h, 005h, 049h, 000h, 000h, 000h
        je     SHORT loc_3_0102
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebp + 1ch], ebx
        or     dword ptr [ebx + 10h], 1
        xor    eax, eax
        jmp    SHORT loc_3_00F5
loc_3_00EA:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp + 1ch], eax
        mov    eax, 25eh
loc_3_00F5:
        mov    word ptr [ebp + 1ah], ax
        db  0C7h, 045h, 010h, 01Ch, 020h, 000h, 000h
        jmp    SHORT loc_3_0112
loc_3_0102:
        mov    eax, 5
        jmp    SHORT loc_3_010E
loc_3_0109:
        mov    eax, 1fh
loc_3_010E:
        mov    word ptr [ebp + 1ah], ax
loc_3_0112:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0117:
        push   ebx
        push   ebp
        mov    ebp, dword ptr [esp + 0ch]
        mov    ebx, dword ptr [ebp + 1ch]
        cmp    byte ptr [ebp + 4], 3
        jne    SHORT loc_3_013E
        push   esi
        push   edi
        mov    esi, dword ptr [ebx + 18h]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, 2
        add    cx, word ptr [esi]
        rep movsb
        pop    edi
        pop    esi
        xor    eax, eax
        jmp    SHORT loc_3_0143
loc_3_013E:
        mov    eax, 1fh
loc_3_0143:
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    ebx
        ret
loc_3_014A:
        push   ebx
        push   ebp
        mov    ebp, dword ptr [esp + 0ch]
        mov    ebx, dword ptr [ebp + 1ch]
        and    dword ptr [ebx + 10h], 0fffffffeh
        xor    eax, eax
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    ebx
        ret
loc_3_0160:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_3_024B
        mov    esi, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_3_018C
        cmp    esi, ebx
        je     NEAR PTR loc_3_024B
loc_3_018C:
        mov    ebx, dword ptr [ebp + 1ch]
        cmp    dword ptr [ebx], 0badbeefh
        jne    NEAR PTR loc_3_0262
        test   dword ptr [ebx + 10h], 1
        je     NEAR PTR loc_3_0262
        lea    edi, [ebp + 20h]
        mov    esi, dword ptr [ebx + 4]
        push   0
        push   edi
        push   esi
        push   3
        push   401h
        call   sub_3_11E5
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0262
        xor    eax, eax
        inc    dword ptr [ebx + 0ch]
        mov    byte ptr [ebp + 4], 1
        mov    dword ptr [ebp], eax
        mov    word ptr [ebp + 18h], 1
        mov    dword ptr [ebp + 24h], eax
        mov    word ptr [ebp + 28h], ax
        mov    word ptr [ebp + 2ah], ax
        mov    edi, dword ptr [ebp + 10h]
        db  0C7h, 007h, 0DDh, 002h, 000h, 000h
        db  0C7h, 047h, 004h, 0EDh, 002h, 000h, 000h
        db  0C7h, 047h, 008h, 05Ch, 020h, 000h, 000h
        push   0
        push   1000h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_0232
        mov    esi, dword ptr [ebp + 20h]
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], 14h
        mov    dword ptr [ebp + 20h], eax
        mov    edx, eax
        mov    dword ptr [edx + 8], 1
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 10h], eax
        jmp    SHORT loc_3_0267
loc_3_0232:
        push   dword ptr [ebp + 20h]
        push   407h
        call   sub_3_0566
        pop    ecx
        pop    ecx
        dec    dword ptr [ebx + 0ch]
        mov    eax, 8
        jmp    SHORT loc_3_0267
loc_3_024B:
        mov    ebx, dword ptr [ebp + 1ch]
        cmp    dword ptr [ebx + 0ch], 0
        jne    SHORT loc_3_025B
        mov    eax, 70bh
        jmp    SHORT loc_3_0267
loc_3_025B:
        mov    eax, 5
        jmp    SHORT loc_3_0267
loc_3_0262:
        mov    eax, 37h
loc_3_0267:
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0270:
        test   dword ptr [edx + 8], 2
        je     SHORT loc_3_0292
        db  0BEh, 070h, 002h, 000h, 000h
        mov    eax, 1388h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 0ch], esi
        and    dword ptr [edx + 8], 0fffffffdh
        jmp    SHORT loc_3_02AA
loc_3_0292:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        mov    ecx, 40h
        db  0BEh, 0ABh, 002h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [edx + 10h], esi
loc_3_02AA:
        ret
loc_3_02AB:
        xor    eax, eax
        mov    dword ptr [edx + 10h], eax
        test   dword ptr [edx + 8], 2
        je     SHORT loc_3_02C0
        call   sub_3_0270
        jmp    SHORT loc_3_02DC
loc_3_02C0:
        push   edx
        mov    eax, edx
        call   sub_3_03A1
        pop    edx
        push   edx
        push   dword ptr [edx]
        call   sub_3_1436
        pop    ecx
        pop    edx
        or     dword ptr [edx + 8], 1
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
loc_3_02DC:
        ret
loc_3_02DD:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, 5
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        ret
loc_3_02ED:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 20h]
        or     dword ptr [eax + 8], 2
        test   dword ptr [eax + 8], 1
        je     SHORT loc_3_032E
        push   eax
        mov    esi, dword ptr [eax]
        db  0BBh, 07Fh, 002h, 000h, 000h
        push   ebx
        push   2
        push   esi
        call   sub_3_13FA
        add    esp, 0ch
        test   eax, eax
        pop    eax
        jne    SHORT loc_3_0393
        and    dword ptr [eax + 8], 0fffffffeh
        mov    edx, eax
        call   sub_3_0270
        mov    eax, edx
loc_3_032E:
        mov    edi, dword ptr [eax + 4]
        add    edi, ecx
        cmp    edi, 1000h
        jbe    SHORT loc_3_0372
        call   sub_3_03A1
        jb     SHORT loc_3_0393
        mov    ecx, dword ptr [ebp]
        cmp    ecx, 0fech
        jbe    SHORT loc_3_0372
        mov    esi, dword ptr [eax]
        mov    edi, dword ptr [ebp + 14h]
        lea    ebx, [eax + 4]
        push   eax
        push   ebx
        push   ecx
        push   edi
        push   esi
        call   sub_3_1416
        add    esp, 10h
        test   eax, eax
        pop    eax
        mov    dword ptr [eax + 4], 14h
        jne    SHORT loc_3_0393
        xor    eax, eax
        jmp    SHORT loc_3_0398
loc_3_0372:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp]
        mov    edi, dword ptr [eax + 4]
        add    edi, eax
        pushf
        cld
        rep movsb
        popf
        sub    edi, eax
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp]
        add    dword ptr [ebp + 24h], ecx
        xor    eax, eax
        jmp    SHORT loc_3_0398
loc_3_0393:
        mov    eax, 1dh
loc_3_0398:
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_03A1:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [eax + 4]
        sub    ecx, 14h
        cmp    ecx, 0
        je     SHORT loc_3_03D2
        lea    edi, [eax + 14h]
        mov    esi, dword ptr [eax]
        lea    ebx, [eax + 4]
        push   eax
        push   ebx
        push   ecx
        push   edi
        push   esi
        call   sub_3_1416
        add    esp, 10h
        test   eax, eax
        pop    eax
        mov    dword ptr [eax + 4], 14h
        clc
        je     SHORT loc_3_03D2
        stc
loc_3_03D2:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_03D6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        mov    ebp, dword ptr [esp + 14h]
        mov    eax, dword ptr [ebp + 20h]
        mov    ebx, dword ptr [ebp + 1ch]
        cmp    dword ptr [ebx], 0badbeefh
        jne    NEAR PTR loc_3_04A6
        cmp    dword ptr [ebx + 0ch], 0
        je     NEAR PTR loc_3_04A6
        xor    esi, esi
        xchg   dword ptr [eax + 0ch], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [eax + 10h], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_3_0428
        cmp    esi, ebx
        jne    SHORT loc_3_0428
        jmp    SHORT loc_3_0450
loc_3_0428:
        mov    eax, dword ptr [ebp + 20h]
        call   sub_3_03A1
        jae    SHORT loc_3_0433
        inc    edi
loc_3_0433:
        test   dword ptr [eax + 8], 1
        jne    SHORT loc_3_0450
        push   dword ptr [eax]
        call   sub_3_1436
        pop    ecx
        test   eax, eax
        je     SHORT loc_3_0449
        inc    edi
loc_3_0449:
        mov    eax, dword ptr [ebp + 20h]
        or     dword ptr [eax + 8], 1
loc_3_0450:
        cmp    byte ptr [ebp + 4], 2
        jne    SHORT loc_3_049E
        mov    esi, dword ptr [eax]
        mov    dword ptr [ebp + 20h], esi
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_3_046B
        inc    edi
loc_3_046B:
        push   dword ptr [ebp + 20h]
        push   407h
        call   sub_3_0566
        pop    ecx
        pop    ecx
        mov    ebx, dword ptr [ebp + 1ch]
        dec    dword ptr [ebx + 0ch]
        cmp    dword ptr [ebx + 0ch], 0
        jne    SHORT loc_3_049E
        mov    eax, dword ptr [ebx + 14h]
        db  085h, 005h, 049h, 000h, 000h, 000h
        jne    SHORT loc_3_049E
        push   dword ptr [ebx + 8]
        db  0CDh, 020h, 050h, 000h, 040h, 000h
        pop    ecx
        jae    SHORT loc_3_049E
        inc    edi
loc_3_049E:
        test   edi, edi
        jne    SHORT loc_3_04A6
        xor    eax, eax
        jmp    SHORT loc_3_04AB
loc_3_04A6:
        mov    eax, 1fh
loc_3_04AB:
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
SPOOLER_W32_DEVICEIOCONTROL:
        mov    ecx, dword ptr [esi + 0ch]
        jecxz  loc_3_04D8
        cmp    ecx, -1
        jne    SHORT loc_3_04C2
        sub    eax, eax
        jmp    SHORT loc_3_04D7
loc_3_04C2:
        cmp    ecx, 5
        ja     SHORT loc_3_04D2
        dec    ecx
        db  0FFh, 024h, 08Dh, 0DCh, 002h, 000h, 000h
loc_3_04D2:
        mov    eax, 32h
loc_3_04D7:
        ret
loc_3_04D8:
        xor    eax, eax
        mov    ecx, 100h
        ret
SPOOLER_Service_00:
        mov    eax, 100h
        clc
        ret
SPOOLER_Service_01:
        jmp    NEAR PTR loc_3_07A0
SPOOLER_Service_02:
        jmp    NEAR PTR loc_3_081C
SPOOLER_Service_03:
        jmp    NEAR PTR loc_3_0A69
SPOOLER_Service_04:
        jmp    NEAR PTR loc_3_0B2B
SPOOLER_Service_05:
        jmp    NEAR PTR loc_3_0B90
SPOOLER_Service_06:
        jmp    NEAR PTR sub_3_0BA4
SPOOLER_Service_07:
        jmp    NEAR PTR loc_3_0C4E
SPOOLER_Service_08:
        jmp    NEAR PTR loc_3_0DB6
SPOOLER_Service_09:
        jmp    NEAR PTR loc_3_0EB8
SPOOLER_Service_10:
        jmp    NEAR PTR loc_3_1053
SPOOLER_Service_11:
        jmp    NEAR PTR loc_3_112D
SPOOLER_Service_12:
        jmp    NEAR PTR loc_3_12EB
SPOOLER_Service_13:
        jmp    NEAR PTR loc_3_13D0
SPOOLER_Service_14:
        jmp    NEAR PTR loc_3_13E5
SPOOLER_Service_15:
        jmp    NEAR PTR loc_3_144A
SPOOLER_Service_16:
        xor    eax, eax
        db  03Bh, 005h, 024h, 004h, 000h, 000h
        je     SHORT loc_3_053F
        stc
        jmp    SHORT loc_3_0547
loc_3_053F:
        push   eax
        call   sub_3_0584
        pop    ecx
        clc
loc_3_0547:
        ret
sub_3_0548:
        push   edx
        db  08Bh, 015h, 028h, 004h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 02Ah, 000h
        pop    edx
        ret
sub_3_0557:
        push   edx
        db  08Bh, 015h, 028h, 004h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 02Ah, 000h
        pop    edx
        ret
sub_3_0566:
        xor    eax, eax
        push   eax
        push   eax
        push   dword ptr [esp + 10h]
        push   1
        push   dword ptr [esp + 14h]
        db  0FFh, 035h, 02Ch, 004h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        ret
sub_3_0584:
        push   ebx
        mov    al, 0
        db  086h, 005h, 0D9h, 002h, 000h, 000h
        or     al, al
        je     SHORT loc_3_05A3
        push   0
        push   0
        db  068h, 00Ch, 020h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
loc_3_05A3:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_3_05E2
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_05E0
        push   5
        db  068h, 00Ch, 020h, 000h, 000h
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  080h, 03Dh, 0DAh, 002h, 000h, 000h, 000h
        jne    SHORT loc_3_05E0
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        je     SHORT loc_3_05A3
        mov    eax, 1
        jmp    SHORT loc_3_05E2
loc_3_05E0:
        xor    eax, eax
loc_3_05E2:
        pop    ebx
        ret
sub_3_05E4:
        push   dword ptr [esp + 4]
        db  0CDh, 020h, 007h, 000h, 017h, 000h
        pop    ecx
        cmp    eax, 20h
        ja     SHORT loc_3_0607
        db  0C6h, 005h, 0DAh, 002h, 000h, 000h, 001h
        db  068h, 00Ch, 020h, 000h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_3_0607:
        ret
        db  056h, 057h, 051h, 0FCh, 033h, 0C9h, 049h, 08Bh, 07Ch, 024h, 014h, 08Bh, 074h, 024h, 010h, 08Bh
        db  0D7h, 032h, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 08Bh, 0FAh, 0F3h, 0A6h, 00Fh, 0B6h, 046h, 0FFh, 00Fh
        db  0B6h, 057h, 0FFh, 02Bh, 0C2h, 059h, 05Fh, 05Eh, 0C3h, 056h, 057h, 051h, 0FCh, 08Bh, 074h, 024h
        db  010h, 033h, 0C9h, 049h, 032h, 0C0h, 08Bh, 0FEh, 0F2h, 0AEh, 0F7h, 0D1h, 08Ah, 044h, 024h, 014h
        db  08Bh, 0FEh, 0F2h, 0AEh, 074h, 004h, 033h, 0C0h, 0EBh, 003h, 04Fh, 08Bh, 0C7h, 059h, 05Fh, 05Eh
        db  0C3h, 056h, 057h, 051h, 0FCh, 033h, 0C9h, 049h, 08Bh, 07Ch, 024h, 014h, 08Bh, 074h, 024h, 010h
        db  08Bh, 0D7h, 032h, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 08Bh, 0FAh, 0ACh, 03Ch, 061h, 07Ch, 006h, 03Ch
        db  07Ah, 07Fh, 002h, 02Ch, 020h, 026h, 038h, 007h, 075h, 005h, 047h, 0E2h, 0EDh, 033h, 0C0h, 059h
        db  05Fh, 05Eh, 0C3h, 06Ah, 0FFh, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 00Ch, 0CDh, 020h, 07Bh
        db  001h, 001h, 000h, 083h, 0C4h, 00Ch, 0C3h, 0FFh, 074h, 024h, 004h, 0CDh, 020h, 07Ch, 001h, 001h
        db  000h, 059h, 0C3h, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 00Ch, 0CDh
        db  020h, 07Dh, 001h, 001h, 000h, 083h, 0C4h, 00Ch, 0C3h
sub_3_06C1:
        push   edi
        push   ecx
        push   edx
        mov    edi, dword ptr [esp + 10h]
        db  0CDh, 020h, 006h, 000h, 040h, 000h
        push   eax
        xor    edx, edx
        mov    ecx, 15180h
        div    ecx
        add    eax, 4
        xor    edx, edx
        mov    ecx, 7
        div    ecx
        mov    word ptr [edi + 4], dx
        db  0CDh, 020h, 060h, 000h, 040h, 000h
        shl    eax, 2
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        pop    ecx
        add    eax, ecx
        push   eax
        db  0CDh, 020h, 037h, 000h, 040h, 000h
        pop    ecx
        mov    ecx, eax
        shr    eax, 19h
        add    ax, 7bch
        mov    word ptr [edi], ax
        mov    eax, ecx
        shr    eax, 15h
        and    ax, 0fh
        mov    word ptr [edi + 2], ax
        mov    eax, ecx
        shr    eax, 10h
        and    ax, 1fh
        mov    word ptr [edi + 6], ax
        mov    eax, ecx
        shr    ax, 0bh
        and    ax, 1fh
        mov    word ptr [edi + 8], ax
        mov    eax, ecx
        shr    ax, 5
        and    ax, 3fh
        mov    word ptr [edi + 0ah], ax
        mov    eax, ecx
        and    ax, 1fh
        shl    ax, 1
        mov    word ptr [edi + 0ch], ax
        pop    edx
        pop    ecx
        pop    edi
        ret
sub_3_0755:
        push   ebx
        push   ecx
        push   edx
        mov    edx, dword ptr [esp + 10h]
        mov    al, 40h
        mov    ah, 3dh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_079A
        push   eax
        mov    ebx, eax
        xor    ecx, ecx
        xor    edx, edx
        mov    ax, 2
        mov    ah, 42h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
        jae    SHORT loc_3_0786
        xor    eax, eax
        xor    edx, edx
loc_3_0786:
        shl    edx, 10h
        mov    dx, ax
        push   edx
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
        jmp    SHORT loc_3_079C
loc_3_079A:
        xor    eax, eax
loc_3_079C:
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_3_07A0:
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 024h, 004h, 000h, 000h
        jne    SHORT loc_3_07B3
        mov    eax, 871h
        jmp    SHORT loc_3_0819
loc_3_07B3:
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        je     SHORT loc_3_07C7
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_07C7
        mov    eax, 7bh
        jmp    SHORT loc_3_0819
loc_3_07C7:
        call   sub_3_0548
        push   esi
        call   sub_3_1591
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_080F
        test   byte ptr [esi + 14h], 4
        je     SHORT loc_3_07E3
        jmp    SHORT loc_3_0801
loc_3_07E3:
        or     esi, esi
        je     SHORT loc_3_0801
        push   dword ptr [esp + 14h]
        push   3
        push   esi
        call   sub_3_1542
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        inc    dword ptr [esi + 0ch]
        jmp    SHORT loc_3_0806
loc_3_0801:
        mov    edi, 7bh
loc_3_0806:
        call   sub_3_0557
        mov    eax, edi
        jmp    SHORT loc_3_0819
loc_3_080F:
        call   sub_3_0557
        mov    eax, 709h
loc_3_0819:
        pop    edi
        pop    esi
        ret
loc_3_081C:
        push   ebp
        mov    ebp, esp
        sub    esp, 110h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        jne    SHORT loc_3_0842
        mov    eax, 871h
        jmp    NEAR PTR loc_3_0A62
loc_3_0842:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 464dh
        je     SHORT loc_3_0859
        mov    dword ptr [ebp - 4], 6
        jmp    NEAR PTR loc_3_0A5F
loc_3_0859:
        call   sub_3_0548
        mov    esi, dword ptr [esi + 10h]
        or     esi, esi
        je     NEAR PTR loc_3_0A53
        test   byte ptr [esi + 18h], 2
        jne    NEAR PTR loc_3_0A53
        db  0FFh, 005h, 070h, 003h, 000h, 000h
        db  081h, 03Dh, 070h, 003h, 000h, 000h, 000h, 003h, 000h, 000h
        jbe    SHORT loc_3_088F
        db  0C7h, 005h, 070h, 003h, 000h, 000h, 000h, 002h, 000h, 000h
loc_3_088F:
        lea    eax, [ebp - 110h]
        push   eax
        db  0FFh, 035h, 070h, 003h, 000h, 000h
        call   sub_3_166F
        add    esp, 8
        lea    edi, [ebp - 110h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, 8
        mov    edi, dword ptr [ebp + 14h]
        cmp    ecx, edi
        jbe    SHORT loc_3_08D0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 4], 7ah
        mov    dword ptr [eax], ecx
        jmp    NEAR PTR loc_3_0A5A
loc_3_08D0:
        mov    dword ptr [ebp - 10h], 1
        mov    dword ptr [ebp - 0ch], 0ach
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    ebx, dword ptr [ebp - 8]
        or     ebx, ebx
        je     NEAR PTR loc_3_0A5A
        mov    eax, ebx
        db  068h, 04Ch, 003h, 000h, 000h
        mov    dword ptr [eax], 494ah
        mov    dword ptr [ebx + 4], 0ach
        db  0A1h, 070h, 003h, 000h, 000h
        mov    dword ptr [ebx + 1ch], eax
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 24h], eax
        db  068h, 04Ch, 003h, 000h, 000h
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 20h], eax
        db  068h, 060h, 003h, 000h, 000h
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 10h], eax
        db  068h, 04Ch, 003h, 000h, 000h
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 28h], eax
        db  068h, 04Ch, 003h, 000h, 000h
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 6ch], eax
        lea    eax, [ebx + 40h]
        push   eax
        call   sub_3_06C1
        add    esp, 4
        db  068h, 050h, 003h, 000h, 000h
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 38h], eax
        inc    dword ptr [esi + 60h]
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [ebx + 2ch], eax
        mov    eax, dword ptr [esi + 34h]
        push   ebx
        inc    dword ptr [eax + 0ch]
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebx + 34h], eax
        mov    eax, dword ptr [esi + 24h]
        push   esi
        inc    dword ptr [eax + 0ch]
        mov    dword ptr [ebx + 8], 0
        call   sub_3_1719
        add    esp, 8
        mov    dword ptr [ebx + 14h], 5000008h
        mov    dword ptr [ebx + 3ch], 0
        mov    dword ptr [ebx + 80h], 0
        mov    dword ptr [ebx + 8ch], 0
        mov    dword ptr [ebx + 5ch], 0
        mov    dword ptr [ebx + 84h], 0
        mov    dword ptr [ebx + 60h], 0
        mov    dword ptr [ebx + 88h], 0
        mov    dword ptr [ebx + 58h], 0
        mov    dword ptr [ebx + 54h], 0
        mov    dword ptr [ebx + 30h], 0
        mov    edx, dword ptr [ebp + 10h]
        add    edx, edi
        lea    edi, [ebp - 110h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edx, ecx
        lea    edi, [ebp - 110h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_3_0A5A
loc_3_0A53:
        mov    dword ptr [ebp - 4], 0ch
loc_3_0A5A:
        call   sub_3_0557
loc_3_0A5F:
        mov    eax, dword ptr [ebp - 4]
loc_3_0A62:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0A69:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 024h, 004h, 000h, 000h
        jne    SHORT loc_3_0A86
        mov    eax, 871h
        jmp    NEAR PTR loc_3_0B24
loc_3_0A86:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 464dh
        je     SHORT loc_3_0A9B
        mov    edi, 6
        jmp    NEAR PTR loc_3_0B22
loc_3_0A9B:
        call   sub_3_0548
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 10h]
        call   sub_3_1748
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0B18
        mov    dword ptr [ebp - 0ch], 1
        mov    dword ptr [ebp - 8], 34h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, ecx
        add    ebx, 0ch
        mov    eax, ecx
        mov    dword ptr [eax], 6060h
        mov    dword ptr [ecx + 4], 0
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 8], eax
        push   dword ptr [ebp + 0ch]
        push   ecx
        push   ebx
        push   2
        call   sub_3_1778
        add    esp, 10h
        push   ebx
        push   41eh
        call   sub_3_0566
        add    esp, 8
        jmp    SHORT loc_3_0B1D
loc_3_0B18:
        mov    edi, 6
loc_3_0B1D:
        call   sub_3_0557
loc_3_0B22:
        mov    eax, edi
loc_3_0B24:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0B2B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 024h, 004h, 000h, 000h
        jne    SHORT loc_3_0B44
        mov    eax, 871h
        jmp    SHORT loc_3_0B8A
loc_3_0B44:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 464dh
        jne    SHORT loc_3_0B83
        call   sub_3_0548
        mov    eax, dword ptr [esi + 10h]
        dec    dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        call   sub_3_0557
        jmp    SHORT loc_3_0B88
loc_3_0B83:
        mov    edi, 6
loc_3_0B88:
        mov    eax, edi
loc_3_0B8A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0B90:
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        mov    eax, 871h
        je     SHORT loc_3_0BA3
        mov    eax, 78h
loc_3_0BA3:
        ret
sub_3_0BA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 10h]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 24h]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 20h]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 38h]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 28h]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 6ch]
        call   sub_3_14E4
        add    esp, 4
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0C1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0C4E:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        jne    SHORT loc_3_0C70
        mov    eax, 871h
        jmp    NEAR PTR loc_3_0DAE
loc_3_0C70:
        mov    esi, dword ptr [esp + 30h]
        cmp    esi, 2
        je     SHORT loc_3_0C83
        mov    eax, 7ch
        jmp    NEAR PTR loc_3_0DAE
loc_3_0C83:
        call   sub_3_0548
        push   dword ptr [esp + 24h]
        call   sub_3_17A6
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_3_0D9D
        mov    eax, dword ptr [eax + 58h]
        or     eax, eax
        je     SHORT loc_3_0CBC
        test   byte ptr [eax + 14h], 20h
        jne    SHORT loc_3_0CBC
        call   sub_3_0557
        mov    eax, 6
        jmp    NEAR PTR loc_3_0DAE
loc_3_0CBC:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 18h], 0
        mov    ebx, dword ptr [eax + 6ch]
        or     ebx, ebx
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, edi
        je     SHORT loc_3_0CE3
loc_3_0CD5:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        je     SHORT loc_3_0CE3
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    SHORT loc_3_0CD5
loc_3_0CE3:
        mov    ebp, ebx
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 10h], eax
        or     ebx, ebx
        je     SHORT loc_3_0D21
loc_3_0CF1:
        mov    eax, dword ptr [esp + 10h]
        dec    dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_3_0D21
        mov    eax, dword ptr [ebp + 14h]
        test   al, 4
        je     SHORT loc_3_0D08
        test   al, 10h
        je     SHORT loc_3_0D1D
loc_3_0D08:
        push   ebp
        push   esi
        push   dword ptr [esp + 1ch]
        call   sub_3_17C4
        add    dword ptr [esp + 24h], eax
        add    esp, 0ch
        mov    ebp, dword ptr [ebp + 8]
loc_3_0D1D:
        or     ebp, ebp
        jne    SHORT loc_3_0CF1
loc_3_0D21:
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 38h]
        cmp    eax, ecx
        jb     SHORT loc_3_0D93
        mov    ebp, dword ptr [esp + 34h]
        mov    ecx, dword ptr [esp + 40h]
        add    eax, ebp
        mov    dword ptr [ecx], 0
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 10h], ecx
        or     ebx, ebx
        je     SHORT loc_3_0DA5
loc_3_0D4F:
        mov    ecx, dword ptr [esp + 10h]
        dec    dword ptr [esp + 10h]
        or     ecx, ecx
        je     SHORT loc_3_0DA5
        mov    ecx, dword ptr [ebx + 14h]
        test   cl, 4
        je     SHORT loc_3_0D68
        test   cl, 10h
        je     SHORT loc_3_0D8D
loc_3_0D68:
        push   eax
        push   ebp
        push   esi
        push   ebx
        inc    edi
        push   dword ptr [esp + 24h]
        call   sub_3_194E
        add    esp, 14h
        cmp    esi, 2
        jne    SHORT loc_3_0D84
        mov    dword ptr [ebp + 3ch], edi
        add    ebp, 68h
loc_3_0D84:
        mov    ecx, dword ptr [esp + 40h]
        mov    ebx, dword ptr [ebx + 8]
        inc    dword ptr [ecx]
loc_3_0D8D:
        or     ebx, ebx
        jne    SHORT loc_3_0D4F
        jmp    SHORT loc_3_0DA5
loc_3_0D93:
        mov    dword ptr [esp + 1ch], 7ah
        jmp    SHORT loc_3_0DA5
loc_3_0D9D:
        mov    dword ptr [esp + 1ch], 6
loc_3_0DA5:
        call   sub_3_0557
        mov    eax, dword ptr [esp + 1ch]
loc_3_0DAE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_0DB6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        sub    esi, esi
        db  039h, 035h, 024h, 004h, 000h, 000h
        jne    SHORT loc_3_0DD9
        mov    eax, 871h
        jmp    NEAR PTR loc_3_0EB0
loc_3_0DD9:
        mov    edi, dword ptr [esp + 24h]
        cmp    edi, 2
        je     SHORT loc_3_0DEC
        mov    eax, 7ch
        jmp    NEAR PTR loc_3_0EB0
loc_3_0DEC:
        call   sub_3_0548
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_3_0E6E
        push   eax
        call   sub_3_17A6
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_3_0EA4
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 24h]
        push   ebp
        call   sub_3_1748
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     ebp, ebp
        je     SHORT loc_3_0EA4
loc_3_0E26:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_0E9D
        push   dword ptr [esp + 10h]
        push   edi
        push   ebp
        call   sub_3_17C4
        mov    ecx, dword ptr [esp + 3ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 2ch]
        cmp    eax, ecx
        ja     SHORT loc_3_0E96
        mov    ebx, dword ptr [esp + 28h]
        lea    eax, [ecx + ebx]
        push   eax
        push   ebx
        push   edi
        push   dword ptr [esp + 1ch]
        push   ebp
        call   sub_3_194E
        add    esp, 14h
        cmp    edi, 2
        jne    SHORT loc_3_0EA9
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 3ch], eax
        jmp    SHORT loc_3_0EA9
loc_3_0E6E:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   dword ptr [esp + 24h]
        push   eax
        call   sub_3_1B78
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_3_0E26
        call   sub_3_0557
        mov    eax, 57h
        jmp    SHORT loc_3_0EB0
loc_3_0E96:
        mov    esi, 7ah
        jmp    SHORT loc_3_0EA9
loc_3_0E9D:
        mov    esi, 57h
        jmp    SHORT loc_3_0EA9
loc_3_0EA4:
        mov    esi, 6
loc_3_0EA9:
        call   sub_3_0557
        mov    eax, esi
loc_3_0EB0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_0EB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        jne    SHORT loc_3_0EE2
        mov    eax, 871h
        jmp    NEAR PTR loc_3_104C
loc_3_0EE2:
        call   sub_3_0548
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_3_0F34
        push   eax
        call   sub_3_17A6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_0F10
        lea    eax, [ebp - 18h]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_3_1748
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_3_0F10:
        or     edi, edi
        je     NEAR PTR loc_3_103D
        mov    eax, dword ptr [edi + 58h]
        or     eax, eax
        je     SHORT loc_3_0F5C
        test   byte ptr [eax + 14h], 20h
        jne    SHORT loc_3_0F5C
        call   sub_3_0557
        mov    eax, 6
        jmp    NEAR PTR loc_3_104C
loc_3_0F34:
        lea    eax, [ebp - 18h]
        push   eax
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_3_1B78
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_0F10
        call   sub_3_0557
        mov    eax, 57h
        jmp    NEAR PTR loc_3_104C
loc_3_0F5C:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_3_1034
        mov    ebx, dword ptr [ebp + 18h]
        or     ebx, ebx
        je     SHORT loc_3_0F8D
        cmp    ebx, 1
        je     SHORT loc_3_0FA3
        cmp    ebx, 2
        je     SHORT loc_3_0FAC
        cmp    ebx, 3
        je     SHORT loc_3_0FB5
        cmp    ebx, 4
        je     SHORT loc_3_0FCE
        mov    dword ptr [ebp - 8], 57h
        jmp    NEAR PTR loc_3_1044
loc_3_0F8D:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp - 4]
        call   sub_3_1BB3
        add    esp, 0ch
        jmp    NEAR PTR loc_3_1044
loc_3_0FA3:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 14h], 1
        jmp    SHORT loc_3_0FCE
loc_3_0FAC:
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 14h], 0fffffffeh
        jmp    SHORT loc_3_0FCE
loc_3_0FB5:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 14h]
        add    ecx, 14h
        test   al, 8
        je     SHORT loc_3_0FCE
        and    eax, 0fffffff7h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 14h], 4
loc_3_0FCE:
        mov    dword ptr [ebp - 14h], 1
        mov    dword ptr [ebp - 10h], 34h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, ecx
        add    esi, 0ch
        mov    eax, ecx
        mov    dword ptr [eax], 6060h
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], edi
        push   ebx
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        push   esi
        push   5
        call   sub_3_1778
        add    esp, 1ch
        push   esi
        push   40ah
        call   sub_3_0566
        add    esp, 8
        jmp    SHORT loc_3_1044
loc_3_1034:
        mov    dword ptr [ebp - 8], 57h
        jmp    SHORT loc_3_1044
loc_3_103D:
        mov    dword ptr [ebp - 8], 6
loc_3_1044:
        call   sub_3_0557
        mov    eax, dword ptr [ebp - 8]
loc_3_104C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1053:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        jne    SHORT loc_3_1076
        mov    eax, 871h
        jmp    NEAR PTR loc_3_1126
loc_3_1076:
        cmp    dword ptr [ebp + 0ch], 2
        je     SHORT loc_3_1086
        mov    eax, 7ch
        jmp    NEAR PTR loc_3_1126
loc_3_1086:
        call   sub_3_0548
        push   dword ptr [ebp + 8]
        call   sub_3_17A6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_1117
        mov    edi, dword ptr [esi + 3ch]
        mov    ebx, dword ptr [esi + 38h]
        mov    dword ptr [esi + 3ch], 0
        mov    dword ptr [esi + 38h], 0
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_1DC2
        mov    ecx, dword ptr [ebp + 18h]
        add    esp, 8
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx], eax
        jbe    SHORT loc_3_10D1
        mov    dword ptr [ebp - 4], 7ah
        jmp    SHORT loc_3_10E4
loc_3_10D1:
        mov    ecx, dword ptr [ebp + 10h]
        add    eax, ecx
        push   eax
        push   ecx
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_1F14
        add    esp, 10h
loc_3_10E4:
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_3_110F
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
loc_3_110F:
        mov    dword ptr [esi + 3ch], edi
        mov    dword ptr [esi + 38h], ebx
        jmp    SHORT loc_3_111E
loc_3_1117:
        mov    dword ptr [ebp - 4], 6
loc_3_111E:
        call   sub_3_0557
        mov    eax, dword ptr [ebp - 4]
loc_3_1126:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_112D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        db  039h, 035h, 024h, 004h, 000h, 000h
        jne    SHORT loc_3_114A
        mov    eax, 871h
        jmp    NEAR PTR loc_3_11DE
loc_3_114A:
        call   sub_3_0548
        push   dword ptr [ebp + 8]
        call   sub_3_17A6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_3_11D2
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_3_11CB
        mov    dword ptr [ebp - 0ch], 1
        mov    dword ptr [ebp - 8], 34h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, ecx
        add    edi, 0ch
        mov    eax, ecx
        mov    dword ptr [eax], 6060h
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], ebx
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        push   edi
        push   4
        call   sub_3_1778
        add    esp, 18h
        push   edi
        push   411h
        call   sub_3_0566
        add    esp, 8
        jmp    SHORT loc_3_11D7
loc_3_11CB:
        mov    esi, 32h
        jmp    SHORT loc_3_11D7
loc_3_11D2:
        mov    esi, 6
loc_3_11D7:
        call   sub_3_0557
        mov    eax, esi
loc_3_11DE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_11E5:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 024h, 004h, 000h, 000h, 000h
        jne    SHORT loc_3_120F
        push   1
        call   sub_3_0584
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_120F
        mov    eax, 871h
        jmp    NEAR PTR loc_3_12E4
loc_3_120F:
        mov    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 14h], 34h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp - 8]
        mov    esi, edi
        add    esi, 0ch
        lea    ecx, [ebp + 10h]
        mov    edx, esi
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_3_1260
loc_3_124C:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        add    ecx, 4
        add    edx, 4
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [ebp + 0ch]
        or     eax, eax
        jne    SHORT loc_3_124C
loc_3_1260:
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 415h
        je     SHORT loc_3_1278
        cmp    ebx, 41fh
        je     SHORT loc_3_1278
        mov    dword ptr [ebp - 4], esi
        jmp    SHORT loc_3_128F
loc_3_1278:
        mov    dword ptr [edi], 6060h
        mov    dword ptr [edi + 4], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi], edi
        mov    dword ptr [ebp - 4], edi
loc_3_128F:
        push   esi
        push   ebx
        call   sub_3_0566
        add    esp, 8
        push   5
        push   dword ptr [ebp - 4]
        call   sub_3_2216
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_12BB
        cmp    ebx, 415h
        jne    SHORT loc_3_12B7
        mov    esi, dword ptr [edi + 4]
        jmp    SHORT loc_3_12C0
loc_3_12B7:
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_3_12C0
loc_3_12BB:
        mov    esi, 5
loc_3_12C0:
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, esi
loc_3_12E4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_12EB:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   6
        push   415h
        call   sub_3_11E5
        add    esp, 20h
        ret
sub_3_1313:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 2ch], eax
        db  0C7h, 045h, 0D8h, 074h, 003h, 000h, 000h
        mov    dword ptr [ebp - 24h], 80000002h
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_13CA
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 20h], eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], esi
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], 0
        db  0C7h, 045h, 0F0h, 0A4h, 003h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        cmp    edi, 0eah
        je     SHORT loc_3_13C5
        or     esi, esi
        jne    SHORT loc_3_13C1
        or     edi, edi
        je     SHORT loc_3_13C5
loc_3_13C1:
        mov    eax, edi
        jmp    SHORT loc_3_13CA
loc_3_13C5:
        mov    eax, 7ah
loc_3_13CA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_13D0:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_3_1313
        add    esp, 0ch
        ret
loc_3_13E5:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_3_1313
        add    esp, 0ch
        ret
sub_3_13FA:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   3
        push   402h
        call   sub_3_11E5
        add    esp, 14h
        ret
sub_3_1416:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   4
        push   404h
        call   sub_3_11E5
        add    esp, 18h
        ret
sub_3_1436:
        push   dword ptr [esp + 4]
        push   1
        push   406h
        call   sub_3_11E5
        add    esp, 0ch
        ret
loc_3_144A:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   6
        push   41fh
        call   sub_3_11E5
        add    esp, 20h
        ret
sub_3_1472:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_3_1485
        sub    eax, eax
        jmp    SHORT loc_3_14DE
loc_3_1485:
        mov    dword ptr [ebp - 0ch], 1
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp - 4]
        or     edx, edx
        je     SHORT loc_3_14DC
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_14DC:
        mov    eax, edx
loc_3_14DE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_14E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_3_1518
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_151A
loc_3_1518:
        sub    eax, eax
loc_3_151A:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_151E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esp + 0ch]
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [esi], eax
        mov    eax, 1
        pop    esi
        ret
sub_3_1542:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 0ch], 1
        mov    dword ptr [ebp - 8], 20h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, eax
        je     SHORT loc_3_158B
        mov    ecx, eax
        mov    dword ptr [ecx], 464dh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        jmp    SHORT loc_3_158D
loc_3_158B:
        sub    eax, eax
loc_3_158D:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1591:
        push   ebx
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        or     edx, edx
        jne    SHORT loc_3_15A2
        sub    eax, eax
        jmp    NEAR PTR loc_3_166C
loc_3_15A2:
        db  0A1h, 024h, 004h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_3_15EB
loc_3_15AE:
        mov    ecx, edx
        mov    esi, dword ptr [eax + 10h]
loc_3_15B3:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_3_15D3
        or     bl, bl
        je     SHORT loc_3_15CF
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_3_15D3
        add    ecx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_3_15B3
loc_3_15CF:
        sub    ecx, ecx
        jmp    SHORT loc_3_15D8
loc_3_15D3:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_15D8:
        or     ecx, ecx
        je     SHORT loc_3_15E3
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_3_15AE
loc_3_15E3:
        or     eax, eax
        jne    NEAR PTR loc_3_166C
loc_3_15EB:
        db  0A1h, 024h, 004h, 000h, 000h
        mov    eax, dword ptr [eax + 1ch]
        or     eax, eax
        je     SHORT loc_3_1628
        mov    ecx, dword ptr [eax + 58h]
        mov    esi, edx
        mov    ecx, dword ptr [ecx + 10h]
loc_3_15FF:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [ecx]
        jne    SHORT loc_3_161F
        or     bl, bl
        je     SHORT loc_3_161B
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    SHORT loc_3_161F
        add    esi, 2
        add    ecx, 2
        or     bl, bl
        jne    SHORT loc_3_15FF
loc_3_161B:
        sub    ecx, ecx
        jmp    SHORT loc_3_1624
loc_3_161F:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_1624:
        or     ecx, ecx
        je     SHORT loc_3_166C
loc_3_1628:
        db  0A1h, 024h, 004h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_3_166C
loc_3_1634:
        mov    ecx, dword ptr [eax + 58h]
        mov    esi, edx
        mov    ecx, dword ptr [ecx + 10h]
loc_3_163C:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [ecx]
        jne    SHORT loc_3_165C
        or     bl, bl
        je     SHORT loc_3_1658
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    SHORT loc_3_165C
        add    esi, 2
        add    ecx, 2
        or     bl, bl
        jne    SHORT loc_3_163C
loc_3_1658:
        sub    ecx, ecx
        jmp    SHORT loc_3_1661
loc_3_165C:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_1661:
        or     ecx, ecx
        je     SHORT loc_3_166C
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_3_1634
loc_3_166C:
        pop    esi
        pop    ebx
        ret
sub_3_166F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_3_16DF
        add    esp, 4
        mov    ecx, eax
        mov    edx, dword ptr [esp + 18h]
        mov    byte ptr [ecx + edx], 5ch
        inc    ecx
        lea    ebx, [edx + ecx + 5]
        db  0BEh, 0ECh, 003h, 000h, 000h
        mov    edi, ebx
        movsd
        movsb
        mov    edi, 4
        mov    esi, dword ptr [esp + 14h]
loc_3_16A0:
        or     esi, esi
        je     SHORT loc_3_16BF
        dec    ebx
        mov    ebp, 0ah
        mov    eax, esi
        sub    edx, edx
        div    ebp
        add    dl, 30h
        mov    byte ptr [ebx], dl
        mov    eax, esi
        sub    edx, edx
        div    ebp
        mov    esi, eax
        jmp    SHORT loc_3_16C3
loc_3_16BF:
        dec    ebx
        mov    byte ptr [ebx], 30h
loc_3_16C3:
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    SHORT loc_3_16A0
loc_3_16CA:
        mov    eax, ecx
        inc    ecx
        mov    edx, dword ptr [esp + 18h]
        cmp    byte ptr [eax + edx], 0
        jne    SHORT loc_3_16CA
        lea    eax, [ecx - 1]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_16DF:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 00Dh, 024h, 004h, 000h, 000h
        sub    eax, eax
        mov    edi, dword ptr [ecx]
        mov    esi, dword ptr [esp + 10h]
loc_3_16F0:
        mov    ecx, eax
        inc    eax
        mov    dl, byte ptr [edi]
        mov    byte ptr [esi + ecx], dl
        inc    edi
        or     dl, dl
        jne    SHORT loc_3_16F0
        mov    byte ptr [esi + eax - 1], 5ch
        db  0BAh, 054h, 003h, 000h, 000h
loc_3_1707:
        mov    ecx, eax
        inc    eax
        mov    bl, byte ptr [edx]
        mov    byte ptr [esi + ecx], bl
        inc    edx
        or     bl, bl
        jne    SHORT loc_3_1707
        dec    eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1719:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 6ch]
        or     edx, edx
        jne    SHORT loc_3_172D
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 6ch], ecx
        jmp    SHORT loc_3_1747
loc_3_172D:
        cmp    dword ptr [edx + 8], 0
        je     SHORT loc_3_173D
        sub    eax, eax
loc_3_1735:
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 8], eax
        jne    SHORT loc_3_1735
loc_3_173D:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 18h], edx
        mov    dword ptr [edx + 8], ecx
loc_3_1747:
        ret
sub_3_1748:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 6ch]
        mov    dword ptr [edx], 1
        or     eax, eax
        je     SHORT loc_3_176F
        mov    ecx, dword ptr [esp + 8]
loc_3_1761:
        cmp    dword ptr [eax + 1ch], ecx
        je     SHORT loc_3_1777
        mov    eax, dword ptr [eax + 8]
        inc    dword ptr [edx]
        or     eax, eax
        jne    SHORT loc_3_1761
loc_3_176F:
        mov    dword ptr [edx], 0
        sub    eax, eax
loc_3_1777:
        ret
sub_3_1778:
        push   esi
        push   edi
        lea    edi, [esp + 14h]
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_3_17A3
        mov    edx, 4
loc_3_178E:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        add    edi, edx
        add    dword ptr [esp + 10h], edx
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    SHORT loc_3_178E
loc_3_17A3:
        pop    edi
        pop    esi
        ret
sub_3_17A6:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_3_17C1
        cmp    dword ptr [eax], 464dh
        jne    SHORT loc_3_17C1
        mov    eax, dword ptr [eax + 10h]
        cmp    dword ptr [eax], 4951h
        je     SHORT loc_3_17C3
loc_3_17C1:
        sub    eax, eax
loc_3_17C3:
        ret
sub_3_17C4:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 2ch], 2
        je     SHORT loc_3_17D9
        sub    eax, eax
        jmp    NEAR PTR loc_3_1906
loc_3_17D9:
        mov    edx, dword ptr [esp + 30h]
        mov    edi, dword ptr [edx + 28h]
        or     edi, edi
        je     SHORT loc_3_17F5
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 20h], ecx
        jmp    SHORT loc_3_17FD
loc_3_17F5:
        mov    dword ptr [esp + 20h], 0
loc_3_17FD:
        mov    edi, dword ptr [edx + 24h]
        or     edi, edi
        je     SHORT loc_3_1815
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 1ch], ecx
        jmp    SHORT loc_3_181D
loc_3_1815:
        mov    dword ptr [esp + 1ch], 0
loc_3_181D:
        mov    edi, dword ptr [edx + 10h]
        or     edi, edi
        je     SHORT loc_3_1835
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 18h], ecx
        jmp    SHORT loc_3_183D
loc_3_1835:
        mov    dword ptr [esp + 18h], 0
loc_3_183D:
        mov    edi, dword ptr [edx + 20h]
        or     edi, edi
        je     SHORT loc_3_1855
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 14h], ecx
        jmp    SHORT loc_3_185D
loc_3_1855:
        mov    dword ptr [esp + 14h], 0
loc_3_185D:
        mov    edi, dword ptr [edx + 38h]
        or     edi, edi
        je     SHORT loc_3_1875
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_3_187D
loc_3_1875:
        mov    dword ptr [esp + 10h], 0
loc_3_187D:
        mov    edi, dword ptr [edx + 3ch]
        or     edi, edi
        je     SHORT loc_3_1893
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
        jmp    SHORT loc_3_1895
loc_3_1893:
        sub    ebx, ebx
loc_3_1895:
        mov    edi, dword ptr [edx + 6ch]
        or     edi, edi
        je     SHORT loc_3_18AB
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    esi, ecx
        jmp    SHORT loc_3_18AD
loc_3_18AB:
        sub    esi, esi
loc_3_18AD:
        mov    eax, dword ptr [edx + 34h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 10h]
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edx, dword ptr [esp + 28h]
        lea    ebp, [ecx - 1]
        mov    eax, dword ptr [edx + 34h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 10h]
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        mov    edi, dword ptr [edx + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        add    ebp, dword ptr [esp + 20h]
        add    ebp, dword ptr [esp + 1ch]
        add    ebp, dword ptr [esp + 18h]
        add    ebp, dword ptr [esp + 14h]
        add    ebp, dword ptr [esp + 10h]
        add    ebp, ebx
        lea    eax, [esi + ebp + 6bh]
loc_3_1906:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_3_190E:
        sub    esp, 100h
        push   esi
        mov    esi, dword ptr [esp + 108h]
        test   byte ptr [esi + 14h], 8
        je     SHORT loc_3_1946
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esi + 1ch]
        call   sub_3_166F
        lea    eax, [esp + 0ch]
        add    esp, 8
        push   eax
        call   sub_3_0755
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_1946
        mov    dword ptr [esi + 5ch], eax
loc_3_1946:
        pop    esi
        add    esp, 100h
        ret
sub_3_194E:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        cmp    esi, 2
        je     SHORT loc_3_1967
        mov    eax, dword ptr [ebp + 18h]
        jmp    NEAR PTR loc_3_1AF8
loc_3_1967:
        mov    ebx, dword ptr [ebp + 0ch]
        sub    ecx, ecx
        mov    eax, dword ptr [ebx + 14h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        db  083h, 03Dh, 0F0h, 002h, 000h, 000h, 0FFh
        je     SHORT loc_3_198E
        db  0B8h, 0F0h, 002h, 000h, 000h
loc_3_1985:
        add    eax, 4
        inc    ecx
        cmp    dword ptr [eax], -1
        jne    SHORT loc_3_1985
loc_3_198E:
        mov    dword ptr [ebp - 1ch], 1
        lea    eax, [ecx*4]
        mov    dword ptr [ebp - 18h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    dword ptr [ebp - 8], eax
        or     ecx, ecx
        je     NEAR PTR loc_3_1AF5
        cmp    esi, 2
        je     SHORT loc_3_19D4
        mov    eax, dword ptr [ebp + 18h]
        jmp    NEAR PTR loc_3_1AF8
loc_3_19D4:
        mov    eax, dword ptr [ebp + 8]
        add    ecx, 4
        mov    edx, dword ptr [eax + 10h]
        mov    eax, dword ptr [ebp - 4]
        add    ecx, 4
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebx + 28h]
        mov    dword ptr [ecx - 4], eax
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 38h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 34h]
        add    ecx, 4
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 3ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 2ch]
        add    ecx, 4
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 6ch]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 28h], 0
        mov    eax, dword ptr [ebx + 8ch]
        mov    dword ptr [ecx + 38h], eax
        mov    dword ptr [ecx + 3ch], 0
        mov    eax, dword ptr [ebx + 54h]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebx + 58h]
        mov    dword ptr [ecx + 44h], eax
        mov    eax, dword ptr [ebx + 84h]
        mov    dword ptr [ecx + 48h], eax
        mov    eax, dword ptr [ebx + 88h]
        or     eax, eax
        jne    SHORT loc_3_1A77
        test   byte ptr [ebx + 17h], 1
        je     SHORT loc_3_1A77
        mov    dword ptr [ecx + 48h], 0
        mov    eax, dword ptr [ebx + 60h]
        jmp    SHORT loc_3_1A7A
loc_3_1A77:
        mov    ecx, dword ptr [ebp + 14h]
loc_3_1A7A:
        mov    dword ptr [ecx + 64h], eax
        test   dword ptr [ebx + 14h], 4800000h
        je     SHORT loc_3_1A8F
        push   ebx
        call   sub_3_190E
        add    esp, 4
loc_3_1A8F:
        mov    eax, dword ptr [ebx + 5ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 4ch], eax
        lea    esi, [ebx + 40h]
        mov    edi, ecx
        add    edi, 50h
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 60h], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx], eax
        push   dword ptr [ebp + 18h]
        db  068h, 0F0h, 002h, 000h, 000h
        push   ecx
        push   dword ptr [ebp - 8]
        call   sub_3_1AFF
        add    esp, 10h
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 20h], eax
loc_3_1AF5:
        mov    eax, dword ptr [ebp + 18h]
loc_3_1AF8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1AFF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx], -1
        je     SHORT loc_3_1B6D
        mov    edx, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esp + 20h]
loc_3_1B14:
        mov    edi, dword ptr [edx]
        or     edi, edi
        je     SHORT loc_3_1B53
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    ebp, ecx
        mov    edi, dword ptr [edx]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + ecx], ebp
        jmp    SHORT loc_3_1B60
loc_3_1B53:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + ecx], 0
loc_3_1B60:
        add    ebx, 4
        add    edx, 4
        cmp    dword ptr [ebx], -1
        jne    SHORT loc_3_1B14
        jmp    SHORT loc_3_1B71
loc_3_1B6D:
        mov    ebp, dword ptr [esp + 20h]
loc_3_1B71:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1B78:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 024h, 004h, 000h, 000h
        mov    ebp, dword ptr [eax + 4]
        or     ebp, ebp
        je     SHORT loc_3_1BAC
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
loc_3_1B94:
        push   ebx
        push   edi
        push   ebp
        call   sub_3_1748
        add    esp, 0ch
        mov    dword ptr [esi], eax
        or     eax, eax
        jne    SHORT loc_3_1BAC
        mov    ebp, dword ptr [ebp + 8]
        or     ebp, ebp
        jne    SHORT loc_3_1B94
loc_3_1BAC:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1BB3:
        push   esi
        push   edi
        cmp    dword ptr [esp + 10h], 2
        je     SHORT loc_3_1BC6
        mov    eax, 7ch
        jmp    NEAR PTR loc_3_1C50
loc_3_1BC6:
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 38h]
        cmp    eax, 63h
        jb     SHORT loc_3_1BDC
        mov    dword ptr [esi + 8ch], eax
loc_3_1BDC:
        mov    eax, dword ptr [edi + 40h]
        cmp    eax, 5a0h
        jae    SHORT loc_3_1BF8
        cmp    dword ptr [edi + 44h], 5a0h
        jae    SHORT loc_3_1BF8
        mov    dword ptr [esi + 54h], eax
        mov    eax, dword ptr [edi + 44h]
        mov    dword ptr [esi + 58h], eax
loc_3_1BF8:
        push   dword ptr [edi + 14h]
        lea    eax, [esi + 20h]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 2ch]
        lea    eax, [esi + 6ch]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 10h]
        lea    eax, [esi + 10h]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 8]
        lea    eax, [esi + 28h]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 0ch]
        lea    eax, [esi + 24h]
        push   eax
        call   sub_3_151E
        add    esp, 8
        mov    eax, dword ptr [edi + 34h]
        and    eax, 0ffh
        or     dword ptr [esi + 14h], eax
        sub    eax, eax
loc_3_1C50:
        pop    edi
        pop    esi
        ret
sub_3_1C53:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 68h], eax
        db  0C7h, 045h, 09Ch, 0F4h, 003h, 000h, 000h
        mov    dword ptr [ebp - 60h], 80000002h
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_1DBC
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 5ch], eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 58h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 54h], eax
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_1DA4
        lea    edi, [esi + 38h]
        mov    dword ptr [ebp - 50h], edi
        mov    dword ptr [ebp - 4ch], 0
        lea    eax, [ebp - 2ch]
        mov    dword ptr [ebp - 48h], eax
        mov    dword ptr [ebp - 44h], 0
        db  0C7h, 045h, 0C0h, 0ACh, 003h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 3ch], eax
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_1D85
        cmp    dword ptr [edi], 0
        je     SHORT loc_3_1D85
        mov    dword ptr [ebp - 38h], 1
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 34h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [esi + 3ch], eax
        mov    dword ptr [ebp - 28h], edi
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 2ch]
        mov    dword ptr [ebp - 20h], eax
        mov    dword ptr [ebp - 1ch], 0
        db  0C7h, 045h, 0E8h, 0ACh, 003h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        jmp    SHORT loc_3_1D8C
loc_3_1D85:
        mov    dword ptr [esi + 3ch], 0
loc_3_1D8C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_3_1DA4:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_3_1DBC:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1DC2:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 28h], 2
        je     SHORT loc_3_1DD7
        sub    edi, edi
        jmp    NEAR PTR loc_3_1F0A
loc_3_1DD7:
        mov    esi, dword ptr [esp + 24h]
        mov    edi, dword ptr [esi + 1ch]
        or     edi, edi
        je     SHORT loc_3_1DF3
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 1ch], ecx
        jmp    SHORT loc_3_1DFB
loc_3_1DF3:
        mov    dword ptr [esp + 1ch], 0
loc_3_1DFB:
        mov    edi, dword ptr [esi + 30h]
        or     edi, edi
        je     SHORT loc_3_1E13
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 18h], ecx
        jmp    SHORT loc_3_1E1B
loc_3_1E13:
        mov    dword ptr [esp + 18h], 0
loc_3_1E1B:
        mov    edi, dword ptr [esi + 54h]
        or     edi, edi
        je     SHORT loc_3_1E33
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 14h], ecx
        jmp    SHORT loc_3_1E3B
loc_3_1E33:
        mov    dword ptr [esp + 14h], 0
loc_3_1E3B:
        mov    edi, dword ptr [esi + 50h]
        or     edi, edi
        je     SHORT loc_3_1E53
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_3_1E5B
loc_3_1E53:
        mov    dword ptr [esp + 10h], 0
loc_3_1E5B:
        mov    edi, dword ptr [esi + 28h]
        or     edi, edi
        je     SHORT loc_3_1E71
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
        jmp    SHORT loc_3_1E73
loc_3_1E71:
        sub    ebx, ebx
loc_3_1E73:
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_3_1E89
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edx, ecx
        jmp    SHORT loc_3_1E8B
loc_3_1E89:
        sub    edx, edx
loc_3_1E8B:
        mov    eax, dword ptr [esi + 24h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 10h]
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esi + 34h]
        lea    ebp, [ecx - 1]
        mov    edi, dword ptr [eax + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esi + 58h]
        lea    ebp, [ebp + ecx - 1]
        mov    edi, dword ptr [eax + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        mov    edi, dword ptr [esi + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        add    ebp, dword ptr [esp + 1ch]
        add    ebp, dword ptr [esp + 18h]
        add    ebp, dword ptr [esp + 14h]
        add    ebp, dword ptr [esp + 10h]
        add    ebp, ebx
        lea    edi, [edx + ebp + 58h]
        push   esi
        call   sub_3_1C53
        add    esp, 4
        cmp    dword ptr [esi + 3ch], 0
        je     SHORT loc_3_1F0A
        mov    eax, dword ptr [esi + 38h]
        lea    edi, [edi + eax + 3]
        and    edi, 0fffffffch
loc_3_1F0A:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_3_1F14:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 6ch]
        cmp    eax, 2
        je     SHORT loc_3_1F29
        jmp    NEAR PTR loc_3_2000
loc_3_1F29:
        mov    ebx, dword ptr [esp + 68h]
        mov    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [ebx + 58h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebx + 30h]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [ebx + 54h]
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [ebx + 50h]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [ebx + 24h]
        push   dword ptr [esp + 74h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [ebx + 28h]
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [esp + 3ch], eax
        mov    ebp, dword ptr [esp + 74h]
        db  068h, 01Ch, 003h, 000h, 000h
        lea    eax, [esp + 18h]
        push   ebp
        push   eax
        call   sub_3_1AFF
        add    esp, 10h
        cmp    dword ptr [ebx + 3ch], 0
        je     SHORT loc_3_1FC6
        sub    eax, dword ptr [ebx + 38h]
        and    eax, 0fffffffch
        mov    dword ptr [ebp + 1ch], eax
        mov    edx, dword ptr [ebx + 38h]
        mov    edi, eax
        mov    esi, dword ptr [ebx + 3ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_1FCD
loc_3_1FC6:
        mov    dword ptr [ebp + 1ch], 0
loc_3_1FCD:
        mov    ecx, dword ptr [ebx + 18h]
        mov    dword ptr [ebp + 34h], ecx
        mov    ecx, dword ptr [ebx + 40h]
        mov    dword ptr [ebp + 38h], ecx
        mov    dword ptr [ebp + 3ch], 0
        mov    ecx, dword ptr [ebx + 48h]
        mov    dword ptr [ebp + 40h], ecx
        mov    ecx, dword ptr [ebx + 4ch]
        mov    dword ptr [ebp + 44h], ecx
        mov    ecx, dword ptr [ebx + 60h]
        mov    dword ptr [ebp + 4ch], ecx
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [ebp + 48h], ecx
        mov    ecx, dword ptr [ebx + 64h]
        mov    dword ptr [ebp + 50h], ecx
        jmp    SHORT loc_3_2004
loc_3_2000:
        mov    eax, dword ptr [esp + 74h]
loc_3_2004:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_3_200C:
        db  068h, 0BCh, 003h, 000h, 000h
        call   sub_3_05E4
        add    esp, 4
        sub    eax, eax
        ret
loc_3_201C:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        pop    ss
        and    dword ptr [eax], eax
        db  000h, 017h
        add    dword ptr [eax], eax
        db  000h, 005h, 021h, 000h, 000h, 083h
        and    byte ptr [eax], al
        db  000h, 08Ah, 020h, 000h, 000h, 060h
        add    dword ptr [eax], eax
        db  000h, 091h, 020h, 000h, 000h, 098h
        and    byte ptr [eax], al
        db  000h, 09Fh, 020h, 000h, 000h, 04Ah
        add    dword ptr [eax], eax
        db  000h, 0A6h, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h, 0B4h, 020h, 000h, 000h, 0BBh, 020h
        add    byte ptr [eax], al
        ret    20h
        db  000h
loc_3_205C:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        pop    ss
        and    dword ptr [eax], eax
        db  000h, 0D6h
        add    eax, dword ptr [eax]
        db  000h, 0D0h
        and    byte ptr [eax], al
        db  000h, 0D7h
        and    byte ptr [eax], al
        db  000h, 0DEh
        and    byte ptr [eax], al
        db  000h, 0E5h
        and    byte ptr [eax], al
        db  000h, 0ECh
        and    byte ptr [eax], al
        add    byte ptr [eax + 1], bh
        jmp    SHORT loc_3_20F3
loc_3_2083:
        mov    eax, 2
        jmp    SHORT loc_3_20F3
loc_3_208A:
        mov    eax, 3
        jmp    SHORT loc_3_20F3
loc_3_2091:
        mov    eax, 4
        jmp    SHORT loc_3_20F3
loc_3_2098:
        mov    eax, 5
        jmp    SHORT loc_3_20F3
loc_3_209F:
        mov    eax, 6
        jmp    SHORT loc_3_20F3
loc_3_20A6:
        mov    eax, 7
        jmp    SHORT loc_3_20F3
        db  0B8h, 008h, 000h, 000h, 000h, 0EBh, 03Fh
loc_3_20B4:
        mov    eax, 9
        jmp    SHORT loc_3_20F3
loc_3_20BB:
        mov    eax, 0ah
        jmp    SHORT loc_3_20F3
loc_3_20C2:
        mov    eax, 0bh
        jmp    SHORT loc_3_20F3
        db  0B8h, 00Ch, 000h, 000h, 000h, 0EBh, 023h
loc_3_20D0:
        mov    eax, 0dh
        jmp    SHORT loc_3_20F3
loc_3_20D7:
        mov    eax, 0eh
        jmp    SHORT loc_3_20F3
loc_3_20DE:
        mov    eax, 0fh
        jmp    SHORT loc_3_2117
loc_3_20E5:
        mov    eax, 10h
        jmp    SHORT loc_3_20F3
loc_3_20EC:
        mov    eax, 11h
        jmp    SHORT loc_3_20F3
loc_3_20F3:
        push   ebp
        push   edi
        mov    ebp, dword ptr [esp + 0ch]
        mov    eax, 1fh
        mov    word ptr [ebp + 1ah], ax
        pop    edi
        pop    ebp
        ret
loc_3_2105:
        push   ebp
        push   edi
        mov    ebp, dword ptr [esp + 0ch]
        mov    eax, 2
        mov    word ptr [ebp + 1ah], ax
        pop    edi
        pop    ebp
        ret
loc_3_2117:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        xor    eax, eax
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        ret
loc_3_2124:
        push   ebx
        push   esi
        mov    ecx, dword ptr [esi + 10h]
        db  0A1h, 049h, 000h, 000h, 000h
        mov    edx, eax
        xor    eax, ecx
        and    eax, edx
        db  089h, 00Dh, 049h, 000h, 000h, 000h
        je     SHORT loc_3_2175
        mov    ecx, 9
        db  0BAh, 04Dh, 000h, 000h, 000h
loc_3_2146:
        mov    ebx, dword ptr [edx]
        test   dword ptr [ebx + 14h], eax
        je     SHORT loc_3_216F
        test   dword ptr [ebx + 10h], 1
        je     SHORT loc_3_216F
        mov    esi, dword ptr [ebx + 0ch]
        test   esi, esi
        jne    SHORT loc_3_216F
        push   ecx
        push   edx
        push   dword ptr [ebx + 8]
        db  0CDh, 020h, 050h, 000h, 040h, 000h
        pop    ecx
        pop    edx
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_216F
loc_3_216F:
        add    edx, 4
        dec    ecx
        jne    SHORT loc_3_2146
loc_3_2175:
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 004h, 000h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_3_21BE
        push   4
        db  068h, 049h, 000h, 000h, 000h
        push   3
        push   0
        db  068h, 034h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        pop    ecx
loc_3_21BE:
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
loc_3_21C6:
        mov    eax, dword ptr [esi + 10h]
        db  0A3h, 024h, 004h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        db  0A3h, 028h, 004h, 000h, 000h
        mov    eax, dword ptr [esi + 18h]
        db  0A3h, 02Ch, 004h, 000h, 000h
        db  080h, 03Dh, 0D9h, 002h, 000h, 000h, 000h
        jne    SHORT loc_3_21F3
        db  068h, 00Ch, 020h, 000h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_3_21F3:
        ret
loc_3_21F4:
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_3_0BA4
        pop    ecx
        ret
loc_3_21FF:
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_3_0C1C
        pop    ecx
        ret
loc_3_220A:
        mov    eax, dword ptr [esi + 10h]
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
        ret
sub_3_2216:
        push   ebx
        push   esi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_3_222C
        cmp    esi, ebx
        je     SHORT loc_3_2243
loc_3_222C:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    eax, 1
        jmp    SHORT loc_3_2245
loc_3_2243:
        xor    eax, eax
loc_3_2245:
        pop    esi
        pop    ebx
        ret
_PTEXT ends
        end
