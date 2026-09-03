; vcomm_pcode_07.asm -- VCOMM object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  056h, 057h, 053h, 0C7h, 005h, 001h, 003h, 000h, 000h, 000h, 001h, 000h, 000h, 06Ah, 000h, 068h
        db  001h, 003h, 000h, 000h, 068h, 0A3h, 001h, 000h, 000h, 06Ah, 001h, 068h, 0E2h, 000h, 000h, 000h
        db  06Ah, 000h, 0FFh, 036h, 0CDh, 020h, 03Eh, 000h, 033h, 000h, 083h, 0C4h, 01Ch, 00Bh, 0C0h, 075h
        db  027h, 0A1h, 001h, 003h, 000h, 000h, 005h, 00Ch, 000h, 000h, 000h, 0A3h, 097h, 001h, 000h, 000h
        db  0FFh, 074h, 024h, 010h, 068h, 097h, 001h, 000h, 000h, 0FFh, 074h, 024h, 01Ch, 0CDh, 020h, 058h
        db  000h, 033h, 000h, 083h, 0C4h, 00Ch, 0EBh, 053h, 0C7h, 005h, 001h, 003h, 000h, 000h, 000h, 001h
        db  000h, 000h, 06Ah, 000h, 068h, 001h, 003h, 000h, 000h, 068h, 0A3h, 001h, 000h, 000h, 06Ah, 001h
        db  068h, 0CEh, 000h, 000h, 000h, 06Ah, 000h, 0FFh, 036h, 0CDh, 020h, 03Eh, 000h, 033h, 000h, 083h
        db  0C4h, 01Ch, 00Bh, 0C0h, 075h, 025h, 0A1h, 001h, 003h, 000h, 000h, 005h, 00Ch, 000h, 000h, 000h
        db  0A3h, 097h, 001h, 000h, 000h, 0FFh, 074h, 024h, 010h, 068h, 097h, 001h, 000h, 000h, 0FFh, 074h
        db  024h, 01Ch, 0CDh, 020h, 058h, 000h, 033h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 02Ah, 0F9h, 074h
        db  001h, 0F8h, 05Bh, 05Fh, 05Eh, 0C3h
sub_7_00B6:
        sub    esp, 4
        push   esi
        xor    eax, eax
        mov    ecx, dword ptr [edi + 4]
        jecxz  loc_7_0101
        mov    edi, dword ptr [ecx + 4]
        lea    eax, [esp + 4]
        push   0
        push   eax
        db  0CDh, 020h, 057h, 000h, 033h, 000h
        add    esp, 8
        mov    eax, 7
        test   dword ptr [esp + 4], 1
        je     SHORT loc_7_0101
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 10001034h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
loc_7_0101:
        pop    esi
        add    esp, 4
        cmp    eax, 7
        ret
loc_7_0109:
        sub    esp, 14h
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 24h]
        cmp    eax, 1
        jne    NEAR PTR loc_7_025A
        mov    ebx, dword ptr [esp + 2ch]
        db  0B8h, 009h, 001h, 000h, 000h
        mov    edx, ebx
        db  0E8h, 021h, 010h, 0F9h, 0FFh
        db  0BAh, 0CCh, 015h, 000h, 000h
        jne    SHORT loc_7_01A3
        cmp    byte ptr [esi], 2ah
        jne    SHORT loc_7_0171
        mov    edi, esi
        inc    edi
        mov    cl, 8
loc_7_013D:
        mov    al, byte ptr [edi]
        or     al, al
        je     SHORT loc_7_0154
        cmp    al, 61h
        jb     SHORT loc_7_014F
        cmp    al, 7ah
        ja     SHORT loc_7_014F
        and    al, 0dfh
        mov    byte ptr [edi], al
loc_7_014F:
        inc    edi
        dec    cl
        jne    SHORT loc_7_013D
loc_7_0154:
        or     cl, cl
        je     SHORT loc_7_0160
        mov    byte ptr [edi], 20h
        inc    edi
        dec    cl
        jne    SHORT loc_7_0154
loc_7_0160:
        mov    edi, esi
        inc    edi
        xor    eax, eax
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_7_0171
        mov    eax, ecx
        jmp    SHORT loc_7_0188
loc_7_0171:
        mov    edx, esi
        xor    eax, eax
        inc    eax
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jae    SHORT loc_7_0185
        db  0B8h, 0CCh, 015h, 000h, 000h
        jmp    SHORT loc_7_01A1
loc_7_0185:
        mov    eax, dword ptr [edx + 5]
loc_7_0188:
        mov    ecx, dword ptr [eax + 18h]
        db  0BAh, 0CCh, 015h, 000h, 000h
        jecxz  loc_7_01A3
        push   edx
        xor    eax, eax
        mov    al, 30h
        call   ecx
        pop    edx
        jb     SHORT loc_7_01A3
        cmp    eax, 30h
        je     SHORT loc_7_01A3
loc_7_01A1:
        mov    edx, eax
loc_7_01A3:
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_7_041E
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, eax
        xor    eax, eax
        mov    edi, esi
        mov    ecx, 34h
        rep stosb
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 8], 5245454bh
        push   0
        push   esi
        push   ebx
        db  0CDh, 020h, 049h, 000h, 033h, 000h
        add    esp, 0ch
        db  080h, 03Dh, 00Dh, 003h, 000h, 000h, 000h
        je     SHORT loc_7_01F1
        push   esi
        db  0E8h, 042h, 00Dh, 0F9h, 0FFh
        pop    ecx
loc_7_01F1:
        mov    edi, dword ptr [esi]
        push   edi
        db  0E8h, 0D7h, 00Ch, 0F9h, 0FFh
        pop    ecx
        mov    ebx, eax
        test   eax, eax
        je     SHORT loc_7_0247
        push   0
        push   0
        push   4
        db  0FFh, 035h, 0FDh, 002h, 000h, 000h
        db  068h, 001h, 003h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        xor    ecx, ecx
        test   eax, eax
        jne    SHORT loc_7_022A
        db  08Bh, 00Dh, 001h, 003h, 000h, 000h
        movzx  ecx, word ptr [ecx + 2]
loc_7_022A:
        push   ecx
        push   ebx
        push   edi
        push   4
        call   dword ptr [esi + 4]
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_7_0247
        mov    eax, 29h
        or     byte ptr [esi + 1eh], 2
        jmp    NEAR PTR loc_7_0417
loc_7_0247:
        push   8000h
        push   0
        call   0
        pop    ecx
        pop    ecx
        jmp    NEAR PTR loc_7_0415
loc_7_025A:
        mov    esi, dword ptr [esp + 30h]
        test   esi, esi
        je     NEAR PTR loc_7_0424
        cmp    eax, 4
        je     SHORT loc_7_0270
        cmp    eax, 2
        jne    SHORT loc_7_02A3
loc_7_0270:
        or     byte ptr [esi + 1eh], 2
        push   8004h
        push   0
        call   0
        pop    ecx
        pop    ecx
        mov    edi, dword ptr [esi]
        push   edi
        db  0E8h, 046h, 00Ch, 0F9h, 0FFh
        pop    ecx
        test   eax, eax
        je     SHORT loc_7_0297
        push   eax
        push   5
        call   dword ptr [esi + 4]
        pop    ecx
        pop    ecx
loc_7_0297:
        mov    edx, esi
        db  0E8h, 01Ah, 014h, 0F9h, 0FFh
        jmp    NEAR PTR loc_7_0415
loc_7_02A3:
        cmp    eax, 3
        je     NEAR PTR loc_7_03AA
        cmp    eax, 9
        je     NEAR PTR loc_7_03F7
        cmp    eax, 0ah
        je     NEAR PTR loc_7_0407
        cmp    eax, 8
        je     SHORT loc_7_02C9
        jne    NEAR PTR loc_7_0424
loc_7_02C9:
        mov    ebx, dword ptr [esp + 28h]
        and    ebx, 7fffffffh
        cmp    ebx, 1
        je     SHORT loc_7_02E1
        cmp    ebx, 0
        jne    NEAR PTR loc_7_0424
loc_7_02E1:
        mov    edx, dword ptr [esp + 2ch]
        db  0C7h, 005h, 001h, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        push   1
        db  068h, 001h, 003h, 000h, 000h
        db  068h, 049h, 003h, 000h, 000h
        push   3
        db  068h, 04Ah, 003h, 000h, 000h
        push   0
        push   edx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  080h, 03Dh, 049h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_7_0424
        mov    eax, dword ptr [esi + 0ch]
        or     eax, dword ptr [esi + 15h]
        je     NEAR PTR loc_7_0424
        lea    eax, [esp + 18h]
        push   eax
        db  0CDh, 020h, 018h, 000h, 026h, 000h
        add    esp, 4
        cmp    eax, 0
        jne    NEAR PTR loc_7_0424
        cmp    dword ptr [esp + 18h], 1
        jne    NEAR PTR loc_7_0424
        lea    eax, [esp + 14h]
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 026h, 000h
        add    esp, 4
        cmp    eax, 0
        jne    NEAR PTR loc_7_0424
        mov    dword ptr [esp + 0ch], 0
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 10h]
        push   0
        push   0
        push   eax
        push   ecx
        push   dword ptr [esi]
        db  0CDh, 020h, 044h, 000h, 033h, 000h
        add    esp, 14h
        mov    eax, 5
        cmp    dword ptr [esp + 10h], 8
        jne    SHORT loc_7_0392
        mov    eax, 7
loc_7_0392:
        cmp    ebx, 0
        jne    SHORT loc_7_0398
        dec    eax
loc_7_0398:
        bt     dword ptr [esp + 14h], eax
        jb     NEAR PTR loc_7_0424
        mov    eax, 13h
        jmp    SHORT loc_7_0417
loc_7_03AA:
        mov    dword ptr [esp + 1ch], 2
        or     byte ptr [esp + 1ch], 1
        push   8001h
        push   1
        call   0
        pop    ecx
        pop    ecx
        jb     SHORT loc_7_03E3
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, dword ptr [esi + 15h]
        je     SHORT loc_7_0415
        lea    edi, [esi + 15h]
        call   sub_7_00B6
        je     SHORT loc_7_03E3
        lea    edi, [esi + 0ch]
        call   sub_7_00B6
        jne    SHORT loc_7_0415
loc_7_03E3:
        push   8002h
        push   0
        call   0
        pop    ecx
        pop    ecx
        xor    eax, eax
        mov    al, 17h
        jmp    SHORT loc_7_0417
loc_7_03F7:
        push   8002h
        push   0
        call   0
        pop    ecx
        pop    ecx
        jmp    SHORT loc_7_0415
loc_7_0407:
        push   8003h
        push   0
        call   0
        pop    ecx
        pop    ecx
loc_7_0415:
        xor    eax, eax
loc_7_0417:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 14h
        ret
loc_7_041E:
        xor    eax, eax
        mov    al, 2
        jmp    SHORT loc_7_0417
loc_7_0424:
        xor    eax, eax
        inc    eax
        jmp    SHORT loc_7_0417
loc_7_0429:
        cmp    edx, 1
        mov    eax, 13h
        jne    SHORT loc_7_04A6
        db  080h, 03Dh, 0FCh, 002h, 000h, 000h, 0FFh
        je     SHORT loc_7_0448
        db  0E8h, 02Fh, 0FCh, 0F8h, 0FFh
        mov    eax, 13h
        jb     SHORT loc_7_04A6
loc_7_0448:
        push   3
        push   0
        db  068h, 009h, 001h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        db  0E8h, 086h, 012h, 0F9h, 0FFh
        jb     SHORT loc_7_04A6
        db  0C7h, 005h, 001h, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        push   eax
        push   1
        db  068h, 001h, 003h, 000h, 000h
        db  068h, 049h, 003h, 000h, 000h
        push   3
        db  068h, 04Ah, 003h, 000h, 000h
        push   0
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        pop    eax
        db  080h, 03Dh, 049h, 003h, 000h, 000h, 000h
        je     SHORT loc_7_04A6
        push   eax
        push   0
        push   8
        push   ebx
        db  0CDh, 020h, 064h, 000h, 033h, 000h
        add    esp, 0ch
        pop    eax
loc_7_04A6:
        stc
        ret
_PTEXT ends
        end
