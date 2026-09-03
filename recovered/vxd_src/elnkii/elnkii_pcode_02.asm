; elnkii_pcode_02.asm -- ELNKII object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    word ptr [ebp - 10h], 7
        mov    word ptr [ebp - 0eh], 7
        db  0C7h, 045h, 0F4h, 098h, 01Eh, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        db  0E8h, 0BDh, 01Bh, 0FEh, 0FFh
        db  089h, 035h, 062h, 01Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 020h, 01Eh, 000h, 000h
        db  0C6h, 005h, 066h, 01Eh, 000h, 000h, 000h
        db  0C7h, 005h, 05Ah, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 024h, 01Eh, 000h, 000h, 003h
        db  0C6h, 005h, 025h, 01Eh, 000h, 000h, 000h
        db  0C7h, 005h, 02Ah, 01Eh, 000h, 000h, 020h, 008h, 000h, 000h
        db  0C7h, 005h, 02Eh, 01Eh, 000h, 000h, 020h, 00Ah, 000h, 000h
        db  0C7h, 005h, 032h, 01Eh, 000h, 000h, 0B0h, 012h, 000h, 000h
        db  0C7h, 005h, 036h, 01Eh, 000h, 000h, 0D0h, 010h, 000h, 000h
        db  0C7h, 005h, 03Ah, 01Eh, 000h, 000h, 080h, 015h, 000h, 000h
        db  0C7h, 005h, 03Eh, 01Eh, 000h, 000h, 080h, 00Bh, 000h, 000h
        db  0C7h, 005h, 042h, 01Eh, 000h, 000h, 0F0h, 013h, 000h, 000h
        db  0C7h, 005h, 046h, 01Eh, 000h, 000h, 0C0h, 014h, 000h, 000h
        db  0C7h, 005h, 04Ah, 01Eh, 000h, 000h, 020h, 001h, 000h, 000h
        db  0C7h, 005h, 04Eh, 01Eh, 000h, 000h, 0F0h, 014h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A3h, 052h, 01Eh, 000h, 000h
        db  089h, 00Dh, 056h, 01Eh, 000h, 000h
        push   36h
        db  068h, 024h, 01Eh, 000h, 000h
        db  068h, 01Ch, 01Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 01Ch, 01Eh, 000h, 000h
        push   eax
        db  0E8h, 009h, 01Bh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_0108
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0F2h, 01Ah, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0108:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 58h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 40h], 0ah
        mov    word ptr [ebp - 3eh], 0ah
        db  0C7h, 045h, 0C4h, 0ACh, 01Eh, 000h, 000h
        mov    word ptr [ebp - 38h], 0ah
        mov    word ptr [ebp - 36h], 0ah
        db  0C7h, 045h, 0CCh, 0C4h, 01Eh, 000h, 000h
        mov    word ptr [ebp - 30h], 0ch
        mov    word ptr [ebp - 2eh], 0ch
        mov    esi, 300h
        db  0C7h, 045h, 0D4h, 03Ch, 01Fh, 000h, 000h
        mov    word ptr [ebp - 28h], 9
        mov    word ptr [ebp - 26h], 9
        db  0C7h, 045h, 0DCh, 054h, 01Fh, 000h, 000h
        mov    bl, 3
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [ebp - 18h], 0
        mov    byte ptr [ebp - 2], 0
        db  0A1h, 018h, 01Eh, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   230h
        push   eax
        db  0E8h, 035h, 01Bh, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_2_01B1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_01B1:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 8ch
        rep stosd
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 086h, 01Bh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_01F2
        push   0
        push   230h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0F8h, 01Ah, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_01F2:
        push   1
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 051h, 01Bh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0215
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [eax + 4]
loc_2_0215:
        sub    al, al
loc_2_0217:
        xor    ecx, ecx
        mov    cl, al
        db  039h, 034h, 08Dh, 06Ch, 01Eh, 000h, 000h
        je     SHORT loc_2_022A
        inc    al
        cmp    al, 8
        jb     SHORT loc_2_0217
loc_2_022A:
        cmp    al, 8
        jne    SHORT loc_2_023A
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 18h], esi
        jmp    NEAR PTR loc_2_02F9
loc_2_023A:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 009h, 01Bh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_025D
        mov    eax, dword ptr [ebp - 14h]
        mov    bl, byte ptr [eax + 4]
loc_2_025D:
        sub    al, al
        nop
loc_2_0260:
        xor    ecx, ecx
        mov    cl, al
        db  038h, 099h, 08Ch, 01Eh, 000h, 000h
        je     SHORT loc_2_0272
        inc    al
        cmp    al, 4
        jb     SHORT loc_2_0260
loc_2_0272:
        cmp    al, 4
        jne    SHORT loc_2_0283
        xor    eax, eax
        mov    al, bl
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_2_02F9
loc_2_0283:
        push   2
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0C0h, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_02BA
        push   1
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4
        push   eax
        db  0E8h, 0B4h, 01Ah, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_02BA
        mov    byte ptr [ebp - 2], 1
loc_2_02BA:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 08Dh, 019h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 20h], 6
        jne    SHORT loc_2_02F9
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_02F9
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, edx
        add    ecx, 19dh
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_2_02F9:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 05Eh, 01Ah, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 38h], 2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 3ch], esi
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax + 49h], cl
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 48h], bl
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4ah], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 44h], 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4ch], 10h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0380
        or     eax, eax
        je     SHORT loc_2_0374
        push   0
        push   230h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 076h, 019h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0374:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
sub_2_0380:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 38h], 2
        jbe    SHORT loc_2_03A5
        mov    dword ptr [ebp - 8], 0c000009ah
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_03A5:
        mov    dword ptr [ebx + 28h], 0
        mov    dword ptr [ebx + 2ch], 0
        db  0A1h, 018h, 01Eh, 000h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   0
        push   32h
        push   eax
        db  0E8h, 010h, 019h, 0FEh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_2_03D6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_03D6:
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 0ah], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 16h], 2
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebx + 3ch]
        mov    dword ptr [eax + 1ah], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 1eh], 10h
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ebx + 50h]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [ebx + 3ch]
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 400h
        mov    dword ptr [ecx + 26h], eax
        mov    eax, dword ptr [ebp - 0ch]
        lea    edi, [ebx + 4]
        mov    dword ptr [eax + 2ah], 10h
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ebx + 54h]
        mov    dword ptr [ecx + 2eh], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ebx
        db  0A1h, 01Ch, 01Eh, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0B1h, 017h, 0FEh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        push   0
        push   32h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        je     SHORT loc_2_0471
        db  0E8h, 077h, 018h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0471:
        db  0E8h, 066h, 018h, 0FEh, 0FFh
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_04A0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        push   1
        push   0aa00000ah
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 019h, 019h, 0FEh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_2_0796
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_04A0:
        mov    dword ptr [ebx + 1f6h], 0
        mov    dword ptr [ebx + 1fah], 0
        mov    dword ptr [ebx + 1feh], 0
        db  0C7h, 083h, 00Eh, 002h, 000h, 000h, 050h, 016h, 000h, 000h
        mov    dword ptr [ebx + 30h], 0
        mov    dword ptr [ebx + 202h], 0
        mov    eax, dword ptr [ebx + 20eh]
        mov    dword ptr [ebx + 206h], eax
        lea    eax, [ebp - 1]
        mov    dword ptr [ebx + 20ah], ebx
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   ebx
        call   sub_2_1A90
        mov    dword ptr [ebx + 40h], eax
        cmp    byte ptr [ebp - 2], 0
        jne    SHORT loc_2_0520
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 097h, 018h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 8], 0c0010006h
        jmp    NEAR PTR loc_2_0796
loc_2_0520:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_2_0544
        push   0
        push   0aa00000dh
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 073h, 018h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 8], 0c0010006h
        jmp    NEAR PTR loc_2_0796
loc_2_0544:
        cmp    byte ptr [ebx + 4ah], 0
        je     SHORT loc_2_0593
        cmp    dword ptr [ebx + 40h], 0
        jne    SHORT loc_2_0554
        mov    byte ptr [ebx + 4ah], 0
loc_2_0554:
        cmp    byte ptr [ebx + 4ah], 0
        je     SHORT loc_2_0593
        push   2000h
        mov    eax, dword ptr [ebx + 40h]
        push   eax
        mov    eax, dword ptr [edi]
        lea    esi, [ebx + 86h]
        push   eax
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 02Ch, 018h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_059F
        push   0
        push   0aa000001h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 01Dh, 018h, 0FEh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_2_0796
loc_2_0593:
        lea    esi, [ebx + 86h]
        mov    dword ptr [esi], 2000h
loc_2_059F:
        mov    eax, dword ptr [ebx + 38h]
        shl    eax, 9
        mov    byte ptr [ebx + 92h], 20h
        lea    eax, [eax + eax*2]
        add    eax, dword ptr [esi]
        mov    dword ptr [ebx + 8ah], eax
        mov    cl, 6
        mov    al, byte ptr [ebx + 38h]
        imul   cl
        mov    cl, byte ptr [ebx + 92h]
        add    cl, al
        mov    byte ptr [ebx + 93h], cl
        mov    eax, dword ptr [esi]
        add    eax, 2000h
        mov    dword ptr [ebx + 8eh], eax
        mov    al, byte ptr [ebx + 92h]
        add    al, 20h
        mov    byte ptr [ebx + 94h], al
        mov    byte ptr [ebx + 1e4h], 0
        mov    byte ptr [ebx + 1aah], 0
        mov    word ptr [ebx + 6ah], 0ffffh
        sub    ecx, ecx
        mov    byte ptr [ebx + 6ch], 0
        mov    byte ptr [ebx + 1a9h], 0
        mov    byte ptr [ebx + 6dh], 0
        cmp    dword ptr [ebx + 38h], ecx
        jbe    SHORT loc_2_0623
        lea    eax, [ebx + 6eh]
loc_2_0614:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [ebx + 38h], ecx
        ja     SHORT loc_2_0614
loc_2_0623:
        mov    byte ptr [ebx + 1cbh], 0
        mov    byte ptr [ebx + 6ch], 0
        mov    byte ptr [ebx + 22eh], 0
        mov    byte ptr [ebx + 5ch], 0
        push   ebx
        call   sub_2_1B60
        lea    eax, [ebx + 1d4h]
        push   eax
        push   ebx
        lea    eax, [ebx + 19dh]
        push   eax
        db  068h, 080h, 01Ah, 000h, 000h
        db  068h, 000h, 01Ah, 000h, 000h
        db  068h, 080h, 019h, 000h, 000h
        mov    eax, dword ptr [ebx + 4ch]
        push   eax
        call   sub_2_2490
        or     al, al
        jne    SHORT loc_2_0688
        push   0
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 02Fh, 017h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_2_078D
loc_2_0688:
        mov    byte ptr [ebx + 1e5h], 1bh
        db  0C7h, 043h, 020h, 01Ch, 01Eh, 000h, 000h
        db  0A1h, 05Ah, 01Eh, 000h, 000h
        mov    dword ptr [ebx + 24h], eax
        lea    esi, [ebx + 8]
        db  089h, 01Dh, 05Ah, 01Eh, 000h, 000h
        push   ebx
        db  0E8h, 023h, 0FEh, 0FDh, 0FFh
        mov    byte ptr [ebx + 4bh], 1
        movsx  eax, byte ptr [ebx + 48h]
        push   1
        push   0
        push   eax
        push   eax
        db  068h, 0D0h, 006h, 000h, 000h
        push   ebx
        db  068h, 0A0h, 006h, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        db  0E8h, 0EBh, 015h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_06F0
        push   0
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0C0h, 016h, 0FEh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_2_074F
        db  08Dh, 049h, 000h
loc_2_06F0:
        push   ebx
        call   sub_2_1C20
        or     al, al
        jne    SHORT loc_2_0715
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 09Fh, 016h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 8], 0c0010006h
        jmp    SHORT loc_2_0749
loc_2_0715:
        push   ebx
        call   sub_2_1DA0
        or     al, al
        jne    NEAR PTR loc_2_07B0
        db  080h, 03Dh, 068h, 01Eh, 000h, 000h, 000h
        je     NEAR PTR loc_2_07B0
        push   0
        push   0aa000003h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 069h, 016h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 8], 0c0010008h
loc_2_0749:
        push   esi
        db  0E8h, 075h, 015h, 0FEh, 0FFh
loc_2_074F:
        db  039h, 01Dh, 05Ah, 01Eh, 000h, 000h
        jne    SHORT loc_2_0761
        mov    eax, dword ptr [ebx + 24h]
        db  0A3h, 05Ah, 01Eh, 000h, 000h
        jmp    SHORT loc_2_0781
loc_2_0761:
        db  08Bh, 00Dh, 05Ah, 01Eh, 000h, 000h
        cmp    dword ptr [ecx + 24h], ebx
        je     SHORT loc_2_0778
        lea    esp, [esp]
loc_2_0770:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], ebx
        jne    SHORT loc_2_0770
loc_2_0778:
        mov    eax, dword ptr [ecx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], eax
loc_2_0781:
        mov    eax, dword ptr [ebx + 1d4h]
        push   eax
        call   sub_2_25B0
loc_2_078D:
        mov    edx, dword ptr [ebx + 54h]
        add    edx, 8
        sub    al, al
        out    dx, al
loc_2_0796:
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 06Ah, 014h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_07B0:
        mov    byte ptr [ebx + 4bh], 0
        mov    dword ptr [ebx + 212h], 0
        db  0C7h, 083h, 016h, 002h, 000h, 000h, 0D0h, 006h, 000h, 000h
        mov    dword ptr [ebx + 21ah], ebx
        lea    eax, [ebx + 222h]
        db  0C7h, 083h, 01Eh, 002h, 000h, 000h, 020h, 015h, 000h, 000h
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [ebx + 21eh]
        mov    dword ptr [ebx + 226h], ecx
        mov    dword ptr [ebx + 22ah], ebx
        push   7d0h
        push   eax
        db  0E8h, 0B1h, 014h, 0FEh, 0FFh
        mov    byte ptr [ebx + 22fh], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0820:
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
loc_2_0843:
        or     edx, edx
        je     SHORT loc_2_0852
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_0843
        mov    dword ptr [ecx], edx
loc_2_0852:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_0864
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0864:
        db  0A1h, 018h, 01Eh, 000h, 000h
        push   eax
        push   0
        push   75h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 05Fh, 014h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_08A0
        push   0
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 01Ah, 015h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_2_08A0:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 30h]
        mov    ecx, dword ptr [esi + 28h]
        or     ecx, ecx
        jne    SHORT loc_2_08BC
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_2_08BC
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0dh
        sub    al, al
        out    dx, al
loc_2_08BC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 28h], eax
        cmp    byte ptr [esi + 1cbh], 0
        jne    NEAR PTR loc_2_09A1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_2_2600
        or     al, al
        je     NEAR PTR loc_2_09A1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 20h]
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
        mov    dword ptr [esi + 1e6h], 0eeh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_0980
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_2_0963
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_2_0963
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_0980
loc_2_0963:
        push   1
        add    esi, 202h
        push   esi
        db  0E8h, 041h, 013h, 0FEh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h
loc_2_0980:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_2_0997
        movsx  eax, word ptr [esi + 66h]
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    SHORT loc_2_0997
        push   esi
        db  0E8h, 069h, 00Ah, 0FEh, 0FFh
loc_2_0997:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_09A1:
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 28h], eax
        push   75h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 022h, 013h, 0FEh, 0FFh
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0DEh, 013h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_09E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [eax + 28h]
        cmp    esi, edx
        je     SHORT loc_2_0A00
loc_2_09F0:
        mov    ecx, dword ptr [esi + 18h]
        cmp    edx, ecx
        jae    SHORT loc_2_09F9
        mov    edx, ecx
loc_2_09F9:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_2_09F0
loc_2_0A00:
        or     edx, edx
        jne    SHORT loc_2_0A09
        mov    edx, 0eeh
loc_2_0A09:
        mov    dword ptr [eax + 1e6h], edx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_2_0A43
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A43:
        inc    dword ptr [edi + 30h]
        inc    dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [edi + 28h]
        cmp    ecx, ebx
        jne    SHORT loc_2_0A58
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 28h], eax
        jmp    SHORT loc_2_0A71
loc_2_0A58:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_2_0A68
loc_2_0A60:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_2_0A60
loc_2_0A68:
        lea    eax, [ebx + 0ch]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_2_0A71:
        push   0
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        mov    eax, dword ptr [edi + 1d4h]
        push   eax
        call   sub_2_26D0
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_0AA8
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_2_0AA0
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    esi, 103h
        jmp    SHORT loc_2_0AD4
loc_2_0AA0:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_2_0AD4
loc_2_0AA8:
        cmp    esi, 103h
        jne    SHORT loc_2_0AC0
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_2_0AD4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0AC0:
        cmp    esi, 0c001000eh
        jne    SHORT loc_2_0AD1
        mov    byte ptr [ebx + 20h], 1
        mov    esi, 103h
loc_2_0AD1:
        dec    dword ptr [ebx + 1ch]
loc_2_0AD4:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_2_0B09
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [edi + 1e6h], eax
        jne    SHORT loc_2_0AEB
        push   edi
        call   sub_2_09E0
loc_2_0AEB:
        push   0
        push   75h
        push   ebx
        db  0E8h, 0E7h, 011h, 0FEh, 0FFh
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_2_0B17
        cmp    dword ptr [edi + 2ch], 0
        jne    SHORT loc_2_0B17
        push   edi
        db  0E8h, 0F9h, 0F4h, 0FDh, 0FFh
        jmp    SHORT loc_2_0B17
loc_2_0B09:
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [edi + 2ch], ebx
        mov    esi, 103h
loc_2_0B17:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_0B51
        cmp    byte ptr [edi + 1cbh], 0
        jne    SHORT loc_2_0B38
        cmp    dword ptr [edi + 1f6h], 0
        jne    SHORT loc_2_0B38
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_0B51
loc_2_0B38:
        push   1
        add    edi, 202h
        push   edi
        db  0E8h, 06Ch, 011h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0B51:
        cmp    dword ptr [edi + 5eh], 0
        je     SHORT loc_2_0B68
        movsx  eax, word ptr [edi + 66h]
        cmp    dword ptr [edi + eax*4 + 6eh], 0
        jne    SHORT loc_2_0B68
        push   edi
        db  0E8h, 098h, 008h, 0FEh, 0FFh
loc_2_0B68:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0B80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [edi + 30h]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_2_0BA0
        mov    esi, 0c0010002h
        jmp    SHORT loc_2_0BDC
        db  090h
loc_2_0BA0:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        or     eax, eax
        jne    SHORT loc_2_0BB6
        push   ecx
        push   ebx
        push   edi
        call   sub_2_0FF0
        mov    esi, eax
        jmp    SHORT loc_2_0BDC
loc_2_0BB6:
        cmp    eax, 1
        jne    SHORT loc_2_0BD7
        cmp    byte ptr [edi + 1cbh], 0
        je     SHORT loc_2_0BCB
        mov    esi, 0c001000dh
        jmp    SHORT loc_2_0BDC
loc_2_0BCB:
        push   ecx
        push   ebx
        push   edi
        call   sub_2_1040
        mov    esi, eax
        jmp    SHORT loc_2_0BDC
loc_2_0BD7:
        mov    esi, 10001h
loc_2_0BDC:
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_0C17
        cmp    byte ptr [edi + 1cbh], 0
        jne    SHORT loc_2_0C00
        cmp    dword ptr [edi + 1f6h], 0
        jne    SHORT loc_2_0C00
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_0C17
loc_2_0C00:
        push   1
        add    edi, 202h
        push   edi
        db  0E8h, 0A4h, 010h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_0C17:
        cmp    dword ptr [edi + 5eh], 0
        je     SHORT loc_2_0C2E
        movsx  eax, word ptr [edi + 66h]
        cmp    dword ptr [edi + eax*4 + 6eh], 0
        jne    SHORT loc_2_0C2E
        push   edi
        db  0E8h, 0D2h, 007h, 0FEh, 0FFh
loc_2_0C2E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C40:
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
        ja     SHORT loc_2_0C90
        je     NEAR PTR loc_2_0E43
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_2_0CB1
        shl    eax, 2
        db  0FFh, 0A0h, 0A4h, 00Fh, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C90:
        cmp    eax, 1010102h
        je     NEAR PTR loc_2_0E65
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_0E87
        cmp    eax, 1010104h
        je     NEAR PTR loc_2_0F52
loc_2_0CB1:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_2_0F5B
        db  08Dh, 049h, 000h
loc_2_0CC0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_2_0CD5
        db  0BEh, 078h, 000h, 000h, 000h
        mov    edi, 58h
        jmp    NEAR PTR loc_2_0F5B
loc_2_0CD5:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 78h
        jmp    NEAR PTR loc_2_0F5B
loc_2_0CE4:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 1cbh], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_2_0CFE
        mov    dword ptr [ebp - 14h], 0
loc_2_0CFE:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_0F5B
        db  005h, 000h, 000h, 000h, 000h
loc_2_0D10:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_0F5B
        db  08Dh, 049h, 000h
loc_2_0D20:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_2_0F5B
        db  08Dh, 064h, 024h, 000h
loc_2_0D30:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_2_0F5B
        db  08Dh, 064h, 024h, 000h
loc_2_0D40:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_2_0F5B
        db  08Dh, 064h, 024h, 000h
loc_2_0D50:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 38h]
        shl    eax, 9
        lea    eax, [eax + eax*2]
        jmp    NEAR PTR loc_2_0F58
loc_2_0D61:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 2000h
        mov    eax, dword ptr [ebx + 38h]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 8
        neg    eax
        add    eax, 2000h
        mov    dword ptr [ebp - 8], eax
        shr    eax, 8
        shl    eax, 2
        sub    dword ptr [ebp - 8], eax
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_2_0F5B
loc_2_0D97:
        mov    dword ptr [ebp - 8], 600h
        jmp    NEAR PTR loc_2_0F5B
loc_2_0DA3:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_2_0F5B
        db  090h
loc_2_0DB0:
        db  0BEh, 060h, 01Fh, 000h, 000h
        mov    edi, 16h
        jmp    NEAR PTR loc_2_0F5B
        db  090h
loc_2_0DC0:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0DD7
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1d4h]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_2_0F58
loc_2_0DD7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_2_0F58
loc_2_0DE5:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0DF9
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1e6h]
        jmp    NEAR PTR loc_2_0F58
loc_2_0DF9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        jmp    NEAR PTR loc_2_0F58
loc_2_0E04:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_2_0F5B
loc_2_0E17:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_2_0F5B
loc_2_0E23:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0eh
        cmp    byte ptr [ebx + 4ah], 0
        jne    NEAR PTR loc_2_0F5B
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_2_0F5B
loc_2_0E43:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 1a3h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 064h, 00Fh, 0FEh, 0FFh
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_0F5B
loc_2_0E65:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 19dh
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 042h, 00Fh, 0FEh, 0FFh
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_0F5B
loc_2_0E87:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_2_0EEA
        mov    eax, dword ptr [ebx + 1d4h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_0EBC
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_2_0F5B
loc_2_0EBC:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebx + 1d4h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2BF0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_2_0F5B
loc_2_0EEA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 1d4h]
        push   eax
        call   sub_2_2AF0
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_0F20
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_2_0F5B
        db  08Dh, 049h, 000h
loc_2_0F20:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 1d4h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2B30
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_2_0F5B
loc_2_0F52:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 4ch]
loc_2_0F58:
        mov    dword ptr [ebp - 8], eax
loc_2_0F5B:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0F97
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_2_0F97
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_2_0F87
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
loc_2_0F87:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 02Ah, 00Eh, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_2_0F97:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  090h
loc_2_0FA4:
        ror    byte ptr [eax + eax], 0
        in     al, 0ch
        add    byte ptr [eax], al
        db  010h, 00Dh, 000h, 000h, 010h, 00Dh
        add    byte ptr [eax], al
        db  020h, 00Dh, 000h, 000h, 030h, 00Dh
        add    byte ptr [eax], al
        inc    eax
        db  00Dh, 000h, 000h, 050h, 00Dh
        add    byte ptr [eax], al
        popad
        db  00Dh, 000h, 000h, 097h, 00Dh
        add    byte ptr [eax], al
        db  0A3h, 00Dh, 000h, 000h, 0B1h
        or     al, 0
        db  000h, 0B0h, 00Dh, 000h, 000h, 0C0h
        db  00Dh, 000h, 000h, 0E5h, 00Dh
        add    byte ptr [eax], al
        add    al, 0eh
        add    byte ptr [eax], al
        pop    ss
        push   cs
        add    byte ptr [eax], al
        mov    cl, 0ch
        add    byte ptr [eax], al
        and    ecx, dword ptr [esi]
        add    byte ptr [eax], al
sub_2_0FF0:
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
        call   sub_2_0C40
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_1040:
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
        je     SHORT loc_2_1097
        cmp    eax, 1010fh
        je     NEAR PTR loc_2_10F8
        cmp    eax, 10112h
        je     NEAR PTR loc_2_1180
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_11B0
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_11F1
loc_2_1097:
        cmp    edi, 4
        je     SHORT loc_2_10B4
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_11F1
loc_2_10B4:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0FCh, 00Ch, 0FEh, 0FFh
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_2_10E1
        mov    ebx, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_11F1
loc_2_10E1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1210
        jmp    NEAR PTR loc_2_11EF
loc_2_10F8:
        cmp    edi, 4
        je     SHORT loc_2_1115
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_11F1
loc_2_1115:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 09Bh, 00Ch, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_2_1179
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 1e6h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_2_1150
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 1e6h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_2_11F1
        db  090h
loc_2_1150:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_2_1171
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_2_1171
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        push   edx
        call   sub_2_09E0
        jmp    NEAR PTR loc_2_11F1
loc_2_1171:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_2_11F1
loc_2_1179:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_2_11F1
loc_2_1180:
        cmp    edi, 4
        je     SHORT loc_2_119A
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_2_11F1
loc_2_119A:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        db  0E8h, 013h, 00Ch, 0FEh, 0FFh
        sub    ebx, ebx
        jmp    SHORT loc_2_11F1
        db  08Dh, 049h, 000h
loc_2_11B0:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_2_11D4
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_2_11F1
loc_2_11D4:
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
        call   sub_2_1270
loc_2_11EF:
        mov    ebx, eax
loc_2_11F1:
        or     ebx, ebx
        jne    SHORT loc_2_11FF
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_2_11FF:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1210:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 1cbh], 0
        jne    SHORT loc_2_125A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1250
        inc    dword ptr [esi + 1ch]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 1d4h]
        push   eax
        call   sub_2_2A70
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_2_1250:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_2_125A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1270:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 1cbh], 0
        jne    SHORT loc_2_12C0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_12B2
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
        mov    eax, dword ptr [ecx + 1d4h]
        push   eax
        call   sub_2_2790
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_2_12B2:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_2_12C0:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_12D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], 0
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_0C40
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_2_13C9
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_1350
        je     SHORT loc_2_1390
        cmp    eax, 20101h
        je     SHORT loc_2_1370
        cmp    eax, 20102h
        je     SHORT loc_2_1378
        cmp    eax, 20103h
        je     SHORT loc_2_1380
        cmp    eax, 20104h
        je     SHORT loc_2_1388
        jmp    SHORT loc_2_1365
loc_2_1350:
        cmp    eax, 1020101h
        je     SHORT loc_2_1398
        cmp    eax, 1020102h
        je     SHORT loc_2_13A0
        cmp    eax, 1020103h
        je     SHORT loc_2_13A8
loc_2_1365:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    SHORT loc_2_13B1
        db  08Bh, 0FFh
loc_2_1370:
        mov    eax, dword ptr [ebx + 1abh]
        jmp    SHORT loc_2_13AE
loc_2_1378:
        mov    eax, dword ptr [ebx + 1afh]
        jmp    SHORT loc_2_13AE
loc_2_1380:
        mov    eax, dword ptr [ebx + 1b3h]
        jmp    SHORT loc_2_13AE
loc_2_1388:
        mov    eax, dword ptr [ebx + 1c3h]
        jmp    SHORT loc_2_13AE
loc_2_1390:
        mov    eax, dword ptr [ebx + 1c7h]
        jmp    SHORT loc_2_13AE
loc_2_1398:
        mov    eax, dword ptr [ebx + 1bfh]
        jmp    SHORT loc_2_13AE
loc_2_13A0:
        mov    eax, dword ptr [ebx + 1b7h]
        jmp    SHORT loc_2_13AE
loc_2_13A8:
        mov    eax, dword ptr [ebx + 1bbh]
loc_2_13AE:
        mov    dword ptr [ebp - 4], eax
loc_2_13B1:
        cmp    edi, 4
        jb     SHORT loc_2_13C9
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 0F7h, 009h, 0FEh, 0FFh
        add    dword ptr [ebp - 0ch], 4
loc_2_13C9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_13F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_1429
        cmp    eax, 20101h
        jae    SHORT loc_2_144A
        cmp    eax, 10101h
        jb     SHORT loc_2_1445
        cmp    eax, 1010bh
        jbe    SHORT loc_2_144A
        cmp    eax, 1010dh
        jb     SHORT loc_2_1445
        cmp    eax, 10113h
        jbe    SHORT loc_2_144A
        jmp    SHORT loc_2_1445
loc_2_1429:
        cmp    eax, 1010101h
        jb     SHORT loc_2_1445
        cmp    eax, 1010104h
        jbe    SHORT loc_2_144A
        cmp    eax, 1020101h
        jb     SHORT loc_2_1445
        cmp    eax, 1020103h
        jbe    SHORT loc_2_144A
loc_2_1445:
        mov    esi, 0c0010017h
loc_2_144A:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 30h]
        or     esi, esi
        jne    SHORT loc_2_145D
        push   ecx
        push   edi
        call   sub_2_12D0
        mov    esi, eax
loc_2_145D:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_1494
        cmp    byte ptr [edi + 1cbh], 0
        jne    SHORT loc_2_147E
        cmp    dword ptr [edi + 1f6h], 0
        jne    SHORT loc_2_147E
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_1494
loc_2_147E:
        push   1
        add    edi, 202h
        push   edi
        db  0E8h, 026h, 008h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_2_1494:
        cmp    dword ptr [edi + 5eh], 0
        je     SHORT loc_2_14AB
        movsx  eax, word ptr [edi + 66h]
        cmp    dword ptr [edi + eax*4 + 6eh], 0
        jne    SHORT loc_2_14AB
        push   edi
        db  0E8h, 055h, 0FFh, 0FDh, 0FFh
loc_2_14AB:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_14C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 01Ch, 01Eh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 027h, 007h, 0FEh, 0FFh
        push   0
        db  0A1h, 020h, 01Eh, 000h, 000h
        push   eax
        db  0E8h, 00Eh, 007h, 0FEh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_14F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 22fh], 1
        db  039h, 035h, 05Ah, 01Eh, 000h, 000h
        jne    SHORT loc_2_1513
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 05Ah, 01Eh, 000h, 000h
        jmp    SHORT loc_2_1531
loc_2_1513:
        db  08Bh, 00Dh, 05Ah, 01Eh, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_2_1528
        mov    edi, edi
loc_2_1520:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_2_1520
loc_2_1528:
        mov    eax, dword ptr [ecx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], eax
loc_2_1531:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 222h]
        push   eax
        db  0E8h, 077h, 007h, 0FEh, 0FFh
        push   2625a0h
        db  0E8h, 085h, 007h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_2_25B0
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 064h, 007h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 09Fh, 006h, 0FEh, 0FFh
        push   0
        push   230h
        push   esi
        db  0E8h, 066h, 007h, 0FEh, 0FFh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1580:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_2_15A0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_15A0:
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_2_15C1
        dec    dword ptr [ebx + 1ch]
        dec    dword ptr [esi + 30h]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_15C1:
        mov    edi, dword ptr [esi + 28h]
        or     edi, edi
        je     SHORT loc_2_15E8
loc_2_15C8:
        inc    dword ptr [edi + 1ch]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 062h, 006h, 0FEh, 0FFh
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        or     eax, eax
        jne    SHORT loc_2_15C8
loc_2_15E8:
        mov    byte ptr [esi + 1cbh], 1
        mov    dword ptr [esi + 1cch], 0
        mov    dword ptr [esi + 1d0h], ebx
        push   esi
        call   sub_2_1670
        mov    edi, eax
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_1642
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_2_162B
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_2_162B
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_1642
loc_2_162B:
        push   1
        add    esi, 202h
        push   esi
        db  0E8h, 079h, 006h, 0FEh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_1642:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_2_1659
        movsx  eax, word ptr [esi + 66h]
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    SHORT loc_2_1659
        push   esi
        db  0E8h, 0A7h, 0FDh, 0FDh, 0FFh
loc_2_1659:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1670:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 1f6h], 0
        je     SHORT loc_2_16BC
loc_2_1684:
        mov    ecx, dword ptr [esi + 1f6h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 1f6h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_2_16B0
        cmp    eax, 8
        je     SHORT loc_2_16B0
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 07Ch, 005h, 0FEh, 0FFh
loc_2_16B0:
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_2_1684
loc_2_16BC:
        cmp    word ptr [esi + 6ah], -1
        je     SHORT loc_2_16DB
        mov    dword ptr [esi + 1cch], 2
        dec    dword ptr [esi + 30h]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_16DB:
        dec    dword ptr [esi + 30h]
        push   esi
        call   sub_2_16F0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_16F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    word ptr [eax + 66h], 0
        inc    dword ptr [eax + 30h]
        mov    word ptr [eax + 68h], 0
        mov    edx, dword ptr [eax + 38h]
        cmp    edx, ecx
        je     SHORT loc_2_171F
        lea    esi, [eax + 6eh]
loc_2_1712:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_2_1730
        add    esi, 4
        inc    ecx
        cmp    ecx, edx
        jb     SHORT loc_2_1712
loc_2_171F:
        dec    dword ptr [eax + 30h]
        push   eax
        call   sub_2_1750
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1730:
        mov    dword ptr [eax + 1cch], 3
        dec    dword ptr [eax + 30h]
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1750:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 38h], 0
        jbe    SHORT loc_2_179E
        lea    edi, [esi + 76h]
        nop
loc_2_1770:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_2_1790
        mov    ebx, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 093h, 004h, 0FEh, 0FFh
        dec    dword ptr [ebx + 1ch]
        mov    dword ptr [edi], 0
loc_2_1790:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 38h], eax
        ja     SHORT loc_2_1770
loc_2_179E:
        mov    edx, dword ptr [esi + 5eh]
        or     edx, edx
        je     SHORT loc_2_17C9
loc_2_17A5:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 5eh], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 05Bh, 004h, 0FEh, 0FFh
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 5eh]
        or     edx, edx
        jne    SHORT loc_2_17A5
loc_2_17C9:
        mov    edx, dword ptr [esi + 1eah]
        or     edx, edx
        je     SHORT loc_2_17FD
loc_2_17D3:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 1eah], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 02Ah, 004h, 0FEh, 0FFh
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 1eah]
        or     edx, edx
        jne    SHORT loc_2_17D3
loc_2_17FD:
        cmp    byte ptr [esi + 1aah], 0
        je     SHORT loc_2_1819
loc_2_1806:
        push   2710h
        db  0E8h, 0C0h, 004h, 0FEh, 0FFh
        cmp    byte ptr [esi + 1aah], 0
        jne    SHORT loc_2_1806
loc_2_1819:
        mov    byte ptr [esi + 1e5h], 1bh
        push   esi
        call   sub_2_2160
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 1cbh], 0
        mov    edi, dword ptr [esi + 28h]
        or     edi, edi
        je     SHORT loc_2_1880
        sub    ebx, ebx
loc_2_1842:
        inc    dword ptr [edi + 1ch]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_2_1859
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0E5h, 003h, 0FEh, 0FFh
loc_2_1859:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0D2h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0D0h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        cmp    eax, ebx
        jne    SHORT loc_2_1842
loc_2_1880:
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_1890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 1cch], eax
        jne    NEAR PTR loc_2_1977
        cmp    eax, 1
        je     SHORT loc_2_1912
        cmp    eax, 2
        je     SHORT loc_2_191A
        cmp    eax, 3
        je     SHORT loc_2_1922
        mov    eax, dword ptr [ebp - 4]
loc_2_18BC:
        cmp    eax, 103h
        je     SHORT loc_2_18DB
        push   eax
        mov    eax, dword ptr [esi + 1d0h]
        mov    eax, dword ptr [eax]
        push   eax
        db  0E8h, 054h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 1d0h]
        dec    dword ptr [eax + 1ch]
loc_2_18DB:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_1960
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_2_18FC
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_2_18FC
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_1960
loc_2_18FC:
        push   1
        add    esi, 202h
        push   esi
        db  0E8h, 0A8h, 003h, 0FEh, 0FFh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_1912:
        push   esi
        call   sub_2_1670
        jmp    SHORT loc_2_18BC
loc_2_191A:
        push   esi
        call   sub_2_16F0
        jmp    SHORT loc_2_18BC
loc_2_1922:
        mov    ecx, dword ptr [esi + 30h]
        inc    ecx
        mov    dword ptr [esi + 30h], ecx
        sub    edx, edx
        mov    eax, dword ptr [esi + 38h]
        cmp    eax, edx
        je     SHORT loc_2_1942
        lea    edi, [esi + 6eh]
loc_2_1935:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_2_1951
        add    edi, 4
        inc    edx
        cmp    edx, eax
        jb     SHORT loc_2_1935
loc_2_1942:
        dec    ecx
        mov    dword ptr [esi + 30h], ecx
        push   esi
        call   sub_2_1750
        jmp    NEAR PTR loc_2_18BC
loc_2_1951:
        dec    ecx
        mov    dword ptr [esi + 30h], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_1960:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_2_1977
        movsx  eax, word ptr [esi + 66h]
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    SHORT loc_2_1977
        push   esi
        db  0E8h, 089h, 0FAh, 0FDh, 0FFh
loc_2_1977:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_1980:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_2_1993
        lea    edx, [eax + 49h]
loc_2_1993:
        cmp    byte ptr [ecx + 1cbh], 0
        je     SHORT loc_2_19A5
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_2_19A5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 1f6h], 0
        jne    SHORT loc_2_19E0
        mov    dword ptr [ecx + 1fah], edx
        mov    dword ptr [ecx + 1f6h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_2_19E0:
        mov    eax, dword ptr [ecx + 1fah]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 1fah], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1A00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_2_1A13
        lea    edx, [eax + 21h]
loc_2_1A13:
        cmp    byte ptr [ecx + 1cbh], 0
        je     SHORT loc_2_1A25
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_2_1A25:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 1f6h], 0
        jne    SHORT loc_2_1A60
        mov    dword ptr [ecx + 1fah], edx
        mov    dword ptr [ecx + 1f6h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_2_1A60:
        mov    eax, dword ptr [ecx + 1fah]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 1fah], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1A80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 1ch]
        ret    4
        db  08Dh, 049h, 000h
sub_2_1A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 54h]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, al
        je     NEAR PTR loc_2_1B46
        and    eax, 0ffh
        lea    edx, [eax - 1]
        test   eax, edx
        jne    NEAR PTR loc_2_1B46
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 1
        mov    byte ptr [eax], 1
        sub    al, al
loc_2_1AC8:
        test   byte ptr [ebp - 1], dl
        jne    SHORT loc_2_1AE0
        shr    byte ptr [ebp - 1], 1
        inc    al
        cmp    al, 8
        jne    SHORT loc_2_1AC8
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_1AE0:
        xor    edx, edx
        mov    dl, al
        mov    eax, dword ptr [ecx + 3ch]
        db  039h, 004h, 095h, 0D8h, 01Fh, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        je     SHORT loc_2_1B00
        mov    byte ptr [eax], 0
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_1B00:
        mov    byte ptr [eax], 1
        mov    edx, dword ptr [ecx + 54h]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 1], al
        sub    al, al
        mov    edx, 10h
loc_2_1B14:
        test   byte ptr [ebp - 1], dl
        jne    SHORT loc_2_1B30
        shr    byte ptr [ebp - 1], 1
        inc    al
        cmp    al, 4
        jne    SHORT loc_2_1B14
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1B30:
        mov    byte ptr [ecx + 4ah], 1
        and    eax, 0ffh
        db  08Bh, 004h, 085h, 0F8h, 01Fh, 000h, 000h
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_1B46:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 6
        mov    edx, dword ptr [esi + 54h]
        add    edx, 6
        out    dx, al
        sub    ecx, ecx
loc_2_1B72:
        mov    edx, dword ptr [esi + 50h]
        add    edx, ecx
        in     al, dx
        mov    byte ptr [esi + ecx + 1a3h], al
        inc    ecx
        cmp    ecx, 6
        jb     SHORT loc_2_1B72
        mov    edx, dword ptr [esi + 54h]
        add    edx, 6
        mov    al, 2
        out    dx, al
        cmp    byte ptr [esi + 19dh], 0
        jne    SHORT loc_2_1C0C
        cmp    byte ptr [esi + 19eh], 0
        jne    SHORT loc_2_1C0C
        cmp    byte ptr [esi + 19fh], 0
        jne    SHORT loc_2_1C0C
        cmp    byte ptr [esi + 1a0h], 0
        jne    SHORT loc_2_1C0C
        cmp    byte ptr [esi + 1a1h], 0
        jne    SHORT loc_2_1C0C
        cmp    byte ptr [esi + 1a2h], 0
        jne    SHORT loc_2_1C0C
        mov    al, byte ptr [esi + 1a3h]
        mov    byte ptr [esi + 19dh], al
        mov    al, byte ptr [esi + 1a4h]
        mov    byte ptr [esi + 19eh], al
        mov    al, byte ptr [esi + 1a5h]
        mov    byte ptr [esi + 19fh], al
        mov    al, byte ptr [esi + 1a6h]
        mov    byte ptr [esi + 1a0h], al
        mov    al, byte ptr [esi + 1a7h]
        mov    byte ptr [esi + 1a1h], al
        mov    al, byte ptr [esi + 1a8h]
        mov    byte ptr [esi + 1a2h], al
loc_2_1C0C:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 3
        mov    edx, dword ptr [esi + 54h]
        add    dx, 6
        out    dx, al
        sub    al, al
        out    dx, al
        mov    al, 2
        out    dx, al
        cmp    byte ptr [esi + 49h], 1
        sbb    al, al
        and    al, 2
        mov    byte ptr [esi + 95h], al
        or     al, 20h
        mov    edi, dword ptr [esi + 54h]
        mov    byte ptr [esi + 95h], al
        lea    edx, [edi + 6]
        out    dx, al
        mov    dx, di
        mov    al, byte ptr [esi + 93h]
        out    dx, al
        lea    edx, [edi + 1]
        mov    al, byte ptr [esi + 94h]
        out    dx, al
        mov    al, 4
        mov    cl, byte ptr [esi + 48h]
        shl    al, cl
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 2]
        mov    al, 10h
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, 20h
        out    dx, al
        lea    edx, [edi + 0ah]
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 4ah], al
        lea    edx, [edi + 5]
        je     SHORT loc_2_1C96
        mov    al, 49h
        jmp    SHORT loc_2_1C98
loc_2_1C96:
        mov    al, 40h
loc_2_1C98:
        out    dx, al
        mov    dx, word ptr [esi + 50h]
        mov    al, 21h
        out    dx, al
        in     al, dx
        cmp    al, 21h
        je     SHORT loc_2_1CB0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1CB0:
        mov    edi, dword ptr [esi + 50h]
        mov    al, 48h
        lea    edx, [edi + 0eh]
        out    dx, al
        lea    edx, [edi + 0bh]
        sub    al, al
        out    dx, al
        lea    edx, [edi + 0ah]
        out    dx, al
        lea    edx, [edi + 0ch]
        mov    al, byte ptr [esi + 1e4h]
        out    dx, al
        lea    edx, [edi + 0dh]
        mov    al, 2
        out    dx, al
        mov    cl, byte ptr [esi + 93h]
        lea    edx, [edi + 3]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 1]
        out    dx, al
        lea    edx, [edi + 2]
        mov    al, byte ptr [esi + 94h]
        out    dx, al
        inc    cl
        mov    byte ptr [esi + 97h], cl
        mov    al, byte ptr [esi + 93h]
        inc    al
        mov    byte ptr [esi + 96h], al
        mov    ecx, edi
        mov    byte ptr [esi + 1a9h], 0
        lea    eax, [ecx + 7]
        mov    word ptr [ebp - 8], ax
        mov    dx, ax
        mov    al, 0ffh
        out    dx, al
        lea    edx, [ecx + 0fh]
        mov    al, byte ptr [esi + 1e5h]
        out    dx, al
        mov    ax, cx
        mov    dx, ax
        mov    al, 61h
        out    dx, al
        sub    edi, edi
        mov    edi, edi
loc_2_1D30:
        lea    edx, [ecx + edi]
        inc    edx
        mov    al, byte ptr [esi + edi + 19dh]
        out    dx, al
        inc    edi
        cmp    edi, 6
        jb     SHORT loc_2_1D30
        mov    eax, dword ptr [esi + 1d4h]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 0ch]
        and    edi, 4
loc_2_1D50:
        or     edi, edi
        mov    eax, 0ffh
        jne    SHORT loc_2_1D62
        xor    eax, eax
        mov    al, byte ptr [esi + ebx + 1d8h]
loc_2_1D62:
        lea    edx, [ecx + ebx]
        add    edx, 8
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_2_1D50
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [esi + 97h]
        out    dx, al
        mov    ax, cx
        mov    dx, ax
        mov    al, 21h
        out    dx, al
        mov    ax, cx
        mov    dx, ax
        mov    al, 22h
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 5], 1
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    eax, [esi + 19dh]
        push   eax
        db  068h, 030h, 020h, 000h, 000h
        db  0E8h, 0F6h, 0FFh, 0FDh, 0FFh
        push   6
        lea    eax, [esi + 19dh]
        push   eax
        db  068h, 036h, 020h, 000h, 000h
        db  0E8h, 0E3h, 0FFh, 0FDh, 0FFh
        db  0C6h, 005h, 03Ch, 020h, 000h, 000h, 000h
        db  0C6h, 005h, 03Dh, 020h, 000h, 000h, 000h
        db  0C6h, 005h, 06Bh, 020h, 000h, 000h, 092h
        mov    ecx, dword ptr [esi + 50h]
        mov    al, 40h
        lea    edi, [ecx + 0eh]
        mov    dx, di
        out    dx, al
        lea    edx, [ecx + 0ch]
        sub    al, al
        out    dx, al
        sub    edi, edi
loc_2_1E02:
        mov    ecx, dword ptr [esi + 50h]
        mov    dx, cx
        mov    al, 21h
        out    dx, al
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        db  08Ah, 087h, 014h, 020h, 000h, 000h
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        push   3ch
        push   0
        push   0
        db  068h, 030h, 020h, 000h, 000h
        out    dx, al
        push   esi
        db  0E8h, 061h, 0E4h, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        lea    eax, [ecx + 4]
        mov    dx, ax
        mov    al, byte ptr [esi + 92h]
        out    dx, al
        lea    edx, [ecx + 6]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 3ch
        out    dx, al
        mov    dx, cx
        mov    al, 26h
        out    dx, al
        sub    ebx, ebx
        lea    edx, [ecx + 7]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 0ah
        jne    SHORT loc_2_1E85
        mov    edi, edi
loc_2_1E60:
        inc    ebx
        cmp    ebx, 64h
        ja     NEAR PTR loc_2_2100
        push   2af8h
        db  0E8h, 05Ch, 0FEh, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        add    ecx, 7
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 0ah
        je     SHORT loc_2_1E60
loc_2_1E85:
        sub    ebx, ebx
loc_2_1E87:
        mov    dx, word ptr [esi + 50h]
        mov    al, 21h
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 21h
        je     SHORT loc_2_1EA9
        push   3e8h
        db  0E8h, 030h, 0FEh, 0FDh, 0FFh
        inc    ebx
        cmp    ebx, 7d0h
        jb     SHORT loc_2_1E87
loc_2_1EA9:
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 008h, 020h, 000h, 000h
        db  0E8h, 001h, 0FEh, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        mov    al, 0ah
        lea    ebx, [ecx + 7]
        mov    dx, bx
        out    dx, al
        lea    edx, [ecx + 4]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    al, 10h
        db  038h, 087h, 01Ch, 020h, 000h, 000h
        jne    NEAR PTR loc_2_2100
        mov    al, byte ptr [ebp - 3]
        and    al, 40h
        db  038h, 087h, 018h, 020h, 000h, 000h
        je     SHORT loc_2_1EF2
        db  080h, 0BFh, 014h, 020h, 000h, 000h, 006h
        jne    NEAR PTR loc_2_2100
loc_2_1EF2:
        db  080h, 0BFh, 014h, 020h, 000h, 000h, 006h
        je     SHORT loc_2_1F3E
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0ch
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   al, 2
        je     NEAR PTR loc_2_2100
        sub    ecx, ecx
        nop
loc_2_1F10:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 6
        in     al, dx
        mov    byte ptr [ebp - 1], al
        db  038h, 081h, 020h, 020h, 000h, 000h
        jne    NEAR PTR loc_2_2100
        inc    ecx
        cmp    ecx, 4
        jb     SHORT loc_2_1F10
        mov    ecx, 4
loc_2_1F31:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 6
        in     al, dx
        mov    byte ptr [ebp - 1], al
        dec    ecx
        jne    SHORT loc_2_1F31
loc_2_1F3E:
        inc    edi
        cmp    edi, 2
        jb     NEAR PTR loc_2_1E02
        mov    ecx, dword ptr [esi + 50h]
        mov    dx, cx
        mov    al, 21h
        out    dx, al
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        mov    al, 3
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        sub    edi, edi
        mov    ebx, 0ah
loc_2_1F67:
        db  080h, 0BFh, 028h, 020h, 000h, 000h, 000h
        push   6
        je     SHORT loc_2_1F80
        lea    eax, [esi + 19dh]
        push   eax
        jmp    SHORT loc_2_1F85
        db  005h, 000h, 000h, 000h, 000h
loc_2_1F80:
        db  068h, 06Ch, 020h, 000h, 000h
loc_2_1F85:
        db  068h, 030h, 020h, 000h, 000h
        db  0E8h, 02Dh, 0FEh, 0FDh, 0FFh
        push   3ch
        push   0
        push   0
        db  068h, 030h, 020h, 000h, 000h
        push   esi
        db  0E8h, 0F0h, 0E2h, 0FDh, 0FFh
        lea    eax, [ebp - 0ch]
        push   eax
        push   3ch
        db  068h, 030h, 020h, 000h, 000h
        call   sub_2_2210
        db  080h, 0BFh, 024h, 020h, 000h, 000h, 000h
        jne    SHORT loc_2_1FC1
        mov    al, byte ptr [ebp - 0ch]
        not    al
        mov    byte ptr [ebp - 0ch], al
loc_2_1FC1:
        push   4
        push   3ch
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        db  0E8h, 0BFh, 0E2h, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        lea    eax, [ecx + 4]
        mov    dx, ax
        mov    al, byte ptr [esi + 92h]
        out    dx, al
        lea    edx, [ecx + 6]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 40h
        out    dx, al
        mov    dx, cx
        mov    al, 26h
        out    dx, al
        push   3e8h
        db  068h, 008h, 020h, 000h, 000h
        db  0E8h, 0B0h, 0FCh, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        add    ecx, 7
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   bl, al
        jne    SHORT loc_2_2021
loc_2_2013:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   bl, al
        je     SHORT loc_2_2013
loc_2_2021:
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 008h, 020h, 000h, 000h
        db  0E8h, 089h, 0FCh, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        add    ecx, 7
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0ch
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    al, 3
        db  038h, 087h, 02Ch, 020h, 000h, 000h
        jne    NEAR PTR loc_2_2100
        inc    edi
        cmp    edi, 3
        jb     NEAR PTR loc_2_1F67
        sub    eax, eax
        nop
loc_2_2060:
        mov    ecx, eax
        neg    cl
        mov    byte ptr [ebp + eax - 20ch], cl
        inc    eax
        cmp    eax, 100h
        jb     SHORT loc_2_2060
        sub    ebx, ebx
loc_2_2075:
        push   100h
        mov    ecx, 600h
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        push   edx
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        push   eax
        lea    eax, [ebp - 20ch]
        push   eax
        push   esi
        db  0E8h, 0F6h, 0E1h, 0FDh, 0FFh
        lea    edi, [ebp - 10ch]
        mov    eax, 4d4d4d4dh
        mov    ecx, 40h
        rep stosd
        push   100h
        mov    eax, dword ptr [esi + 86h]
        add    eax, ebx
        push   eax
        lea    eax, [ebp - 10ch]
        push   eax
        push   esi
        db  0E8h, 0C9h, 0E2h, 0FDh, 0FFh
        sub    eax, eax
        lea    esp, [esp]
loc_2_20D0:
        mov    cl, byte ptr [ebp + eax - 20ch]
        cmp    byte ptr [ebp + eax - 10ch], cl
        jne    SHORT loc_2_2100
        inc    eax
        cmp    eax, 100h
        jb     SHORT loc_2_20D0
        add    ebx, 100h
        cmp    ebx, 2000h
        jb     NEAR PTR loc_2_2075
        jmp    SHORT loc_2_2104
        db  08Dh, 064h, 024h, 000h
loc_2_2100:
        mov    byte ptr [ebp - 5], 0
loc_2_2104:
        mov    ecx, dword ptr [esi + 50h]
        mov    dx, cx
        mov    al, 21h
        out    dx, al
        lea    edx, [ecx + 0eh]
        mov    al, 48h
        out    dx, al
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0ah]
        out    dx, al
        lea    edx, [ecx + 0ch]
        mov    al, byte ptr [esi + 1e4h]
        out    dx, al
        lea    edx, [ecx + 0dh]
        mov    al, 2
        out    dx, al
        push   0c350h
        db  0E8h, 099h, 0FBh, 0FDh, 0FFh
        mov    ecx, dword ptr [esi + 50h]
        mov    al, 0ffh
        lea    esi, [ecx + 7]
        mov    dx, si
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        mov    al, byte ptr [ebp - 5]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2160:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 093h, 0DEh, 0FDh, 0FFh
        push   esi
        call   sub_2_1C20
        or     al, al
        jne    SHORT loc_2_2195
        push   1
        push   1
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 01Dh, 0FCh, 0FDh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_2_2195:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0dh
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_21B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_2_21FD
loc_2_21C2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_2_21D0:
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
        je     SHORT loc_2_21F4
        xor    eax, 4c11db6h
        or     eax, edi
loc_2_21F4:
        dec    esi
        jne    SHORT loc_2_21D0
        inc    edx
        cmp    edx, dword ptr [ebp + 0ch]
        jb     SHORT loc_2_21C2
loc_2_21FD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2210:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        sub    esi, esi
        push   eax
        call   sub_2_21B0
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp + 10h]
        lea    esp, [esp]
loc_2_2230:
        lea    eax, [ebp - 1]
        sub    eax, esi
        mov    al, byte ptr [eax]
        mov    dl, al
        and    edx, 0fh
        shr    al, 4
        db  08Ah, 092h, 074h, 020h, 000h, 000h
        and    eax, 0ffh
        shl    dl, 4
        db  002h, 090h, 074h, 020h, 000h, 000h
        not    dl
        mov    byte ptr [ecx + esi], dl
        inc    esi
        cmp    esi, 4
        jb     SHORT loc_2_2230
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_21B0
        shr    eax, 1ah
        mov    ebx, eax
        mov    edx, dword ptr [ebp + 0ch]
        shr    ebx, 3
        mov    byte ptr [edx], bl
        mov    dl, 1
        and    al, 7
        mov    cl, al
        mov    eax, dword ptr [ebp + 10h]
        shl    dl, cl
        mov    byte ptr [eax], dl
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_22A0:
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
loc_2_22C0:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 1d8h], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_2_22C0
        db  068h, 078h, 01Fh, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2BF0
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_2_2327
        sub    edi, edi
loc_2_22F5:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 078h, 01Fh, 000h, 000h
        push   eax
        call   sub_2_2270
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 1d8h], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_2_22F5
loc_2_2327:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_2330:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 50h]
        add    edx, 0ch
        mov    al, byte ptr [eax + 1e4h]
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2350:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edi, dword ptr [eax + 50h]
        mov    si, di
        mov    dx, si
        mov    al, 62h
        out    dx, al
loc_2_2366:
        lea    edx, [ecx + edi]
        add    edx, 8
        mov    al, 0ffh
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     SHORT loc_2_2366
        mov    dx, si
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2390:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 62h
        mov    ebx, dword ptr [ecx + 50h]
        mov    si, bx
        mov    dx, si
        out    dx, al
        sub    edi, edi
loc_2_23A7:
        lea    edx, [ebx + edi]
        add    edx, 8
        mov    al, byte ptr [ecx + edi + 1d8h]
        out    dx, al
        inc    edi
        cmp    edi, 8
        jb     SHORT loc_2_23A7
        mov    dx, si
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_23D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 1d4h]
        mov    edi, dword ptr [eax + 0ch]
        test   edi, 24h
        je     SHORT loc_2_2400
        db  068h, 050h, 023h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 0D2h, 0F8h, 0FDh, 0FFh
        jmp    SHORT loc_2_2405
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_2400:
        call   sub_2_2460
loc_2_2405:
        test   edi, 26h
        lea    eax, [esi + 1e4h]
        je     SHORT loc_2_2418
        or     byte ptr [eax], 8
        jmp    SHORT loc_2_241B
loc_2_2418:
        and    byte ptr [eax], 0f7h
loc_2_241B:
        test   edi, 20h
        je     SHORT loc_2_2428
        or     byte ptr [eax], 10h
        jmp    SHORT loc_2_242B
loc_2_2428:
        and    byte ptr [eax], 0efh
loc_2_242B:
        test   edi, 28h
        je     SHORT loc_2_2438
        or     byte ptr [eax], 4
        jmp    SHORT loc_2_243B
loc_2_2438:
        and    byte ptr [eax], 0fbh
loc_2_243B:
        push   esi
        db  068h, 030h, 023h, 000h, 000h
        add    esi, 8
        push   esi
        db  0E8h, 080h, 0F8h, 0FDh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_22A0
        push   esi
        db  068h, 090h, 023h, 000h, 000h
        add    esi, 8
        push   esi
        db  0E8h, 04Eh, 0F8h, 0FDh, 0FFh
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2490:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 020h, 021h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 029h, 0F8h, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_24C4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_24C4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_24DE
        mov    esi, 2
loc_2_24DE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0E5h, 0F7h, 0FDh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_2512
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_25B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2512:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0ADh, 0F7h, 0FDh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_2541
        push   eax
        call   sub_2_25B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2541:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_25B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_25D0
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 00Ch, 0F7h, 0FDh, 0FFh
loc_2_25D0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_25E6
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 0F6h, 0F6h, 0FDh, 0FFh
loc_2_25E6:
        push   0
        push   32h
        push   esi
        db  0E8h, 0ECh, 0F6h, 0FDh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2600:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_2619
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2619:
        db  0A1h, 020h, 021h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0AAh, 0F6h, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_2639
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2639:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_2640:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_2653
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_2640
        jmp    SHORT loc_2_2662
loc_2_2653:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_2662:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_2678
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_2678:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_2_26D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2A70
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_26F9
        cmp    ebx, 103h
        jne    SHORT loc_2_2710
loc_2_26F9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_2790
        or     eax, eax
        je     SHORT loc_2_2710
        mov    ebx, eax
loc_2_2710:
        or     ebx, ebx
        je     SHORT loc_2_271C
        cmp    ebx, 103h
        jne    SHORT loc_2_277B
loc_2_271C:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_2776
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_2732
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_2732:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_2741
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_2747
loc_2_2741:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_2747:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_2756
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0E2h, 0F4h, 0FDh, 0FFh
loc_2_2756:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 06Fh, 0F5h, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_2776:
        mov    ebx, 0c001000eh
loc_2_277B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2790:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 20h]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 011h, 0F6h, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 0F9h, 0F5h, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_2814
        sub    eax, eax
        nop
loc_2_27F0:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 1ch]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 24h], edi
        ja     SHORT loc_2_27F0
loc_2_2814:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_287F
loc_2_2821:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_2874
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        db  0E8h, 08Eh, 0F5h, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        db  0E8h, 06Dh, 0F5h, 0FDh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_2877
loc_2_2874:
        inc    dword ptr [ebp - 4]
loc_2_2877:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_2821
loc_2_287F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_296A
        mov    edi, dword ptr [ebp + 18h]
loc_2_2893:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_2C50
        or     al, al
        je     SHORT loc_2_28CA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_2958
loc_2_28CA:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_29A4
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 0E8h, 0F4h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 24h]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 0B0h, 0F4h, 0FDh, 0FFh
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 14h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 19h]
        mov    edx, dword ptr [ebp - 14h]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 24h]
loc_2_2958:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_2_2893
loc_2_296A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_29F5
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_2980:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_29FB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_2_29E2
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_2_29E2
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_29E9
loc_2_29A4:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 022h, 0F4h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 00Eh, 0F4h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_29E2:
        mov    dword ptr [ebp - 8], 1
loc_2_29E9:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_2_2980
loc_2_29F5:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_2A60
loc_2_29FB:
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_2A62
        cmp    edi, 103h
        je     SHORT loc_2_2A62
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 097h, 0F3h, 0FDh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 089h, 0F3h, 0FDh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_2A60:
        sub    edi, edi
loc_2_2A62:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_2A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        mov    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_2AAD
        lea    esp, [esp]
loc_2_2AA0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_2AA0
loc_2_2AAD:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_2_2AE1
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_2_2AE3
        cmp    eax, 103h
        je     SHORT loc_2_2AE3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2AE1:
        sub    eax, eax
loc_2_2AE3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_2AF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 19h]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffh
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_2_2B22
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2B17:
        test   dword ptr [esi], edx
        je     SHORT loc_2_2B1C
        inc    eax
loc_2_2B1C:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_2B17
loc_2_2B22:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 19h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 24h], 0
        jbe    SHORT loc_2_2BAC
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_2B70:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_2B9A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_2BC6
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 048h, 0F2h, 0FDh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_2B9A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_2B70
loc_2_2BAC:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_2BC6:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 18h]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2BF0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_2C20
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2C20:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 098h, 0F1h, 0FDh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     NEAR PTR loc_2_2CE9
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_2CE9
        nop
loc_2_2C70:
        cmp    edx, ecx
        jb     SHORT loc_2_2CE9
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_2C93
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2CC7
loc_2_2C93:
        jbe    SHORT loc_2_2CA0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_2CC7
        db  08Bh, 0FFh
loc_2_2CA0:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_2CB7
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2CC7
loc_2_2CB7:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_2CC7
        mov    dword ptr [ebp - 4], 0
loc_2_2CC7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_2CF9
        jle    SHORT loc_2_2CD8
        or     edx, edx
        je     SHORT loc_2_2CE9
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_2CDB
loc_2_2CD8:
        lea    ecx, [edx + 1]
loc_2_2CDB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_2C70
loc_2_2CE9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2CF9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
