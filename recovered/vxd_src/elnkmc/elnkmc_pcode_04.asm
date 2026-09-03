; elnkmc_pcode_04.asm -- ELNKMC object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        mov    word ptr [ebp - 10h], 0
        mov    word ptr [ebp - 0eh], 1
        mov    word ptr [ebp - 0ch], 6042h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0EFh, 003h, 0FCh, 0FFh
        lea    eax, [ebp - 10h]
        db  0A3h, 028h, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 01Ch, 037h, 000h, 000h
        mov    byte ptr [ebp - 48h], 3
        mov    byte ptr [ebp - 47h], 0
        db  0C7h, 045h, 0BEh, 040h, 00Bh, 000h, 000h
        db  0C7h, 045h, 0C2h, 070h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0D2h, 090h, 01Ch, 000h, 000h
        db  0C7h, 045h, 0C6h, 030h, 033h, 000h, 000h
        db  0C7h, 045h, 0CAh, 020h, 035h, 000h, 000h
        db  0C7h, 045h, 0CEh, 080h, 028h, 000h, 000h
        db  0C7h, 045h, 0D6h, 0F0h, 026h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 00Dh, 000h, 000h
        db  0C7h, 045h, 0DEh, 0B0h, 005h, 000h, 000h
        db  0C7h, 045h, 0E2h, 030h, 00Dh, 000h, 000h
        db  0A1h, 0ECh, 035h, 000h, 000h
        db  08Bh, 00Dh, 0F0h, 035h, 000h, 000h
        mov    dword ptr [ebp - 1ah], eax
        lea    eax, [ebp - 48h]
        mov    dword ptr [ebp - 16h], ecx
        db  0C7h, 005h, 024h, 037h, 000h, 000h, 020h, 037h, 000h, 000h
        db  0C7h, 005h, 020h, 037h, 000h, 000h, 020h, 037h, 000h, 000h
        push   36h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 030h, 037h, 000h, 000h
        push   eax
        db  0E8h, 05Eh, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_00D6
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_00D6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 03Fh, 003h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   edi
        db  0A1h, 0E0h, 035h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   2d2h
        push   eax
        db  0E8h, 0F8h, 003h, 0FCh, 0FFh
        or     eax, eax
        jne    NEAR PTR loc_4_04AA
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0b4h
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0edh], eax
        mov    edx, dword ptr [ebp + 20h]
        or     edx, edx
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_4_0157
        mov    byte ptr [eax + 55h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 2fh], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 33h], cl
        add    eax, 2fh
        mov    cl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], cl
        jmp    SHORT loc_4_015B
loc_4_0157:
        mov    byte ptr [eax + 55h], 0
loc_4_015B:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebp + 1ch]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 87h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 56h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1ah], 0c000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ah], 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 162h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 21h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 25h], 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4fh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0d1h
        mov    dword ptr [ecx + 0d5h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0d5h]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0d9h
        mov    dword ptr [ecx + 0ddh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ddh]
        mov    dword ptr [ecx + 0d9h], eax
        mov    cl, 1fh
        mov    eax, dword ptr [ebp - 4]
        sub    cl, bl
        mov    byte ptr [eax + 161h], cl
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0560
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0A5h, 008h, 0FCh, 0FFh
        lea    edi, [ebp - 2ch]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 2ch], eax
        mov    byte ptr [ebp - 28h], al
        mov    dword ptr [ebp - 22h], 3
        mov    dword ptr [ebp - 1eh], eax
        mov    dword ptr [ebp - 1ah], eax
        mov    dword ptr [ebp - 16h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 12h], eax
        mov    dword ptr [ebp - 0eh], 10h
        mov    dword ptr [ebp - 0ah], 0
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 0edh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0f1h
        push   eax
        db  0E8h, 0AEh, 001h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_02C0
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 036h, 003h, 0FCh, 0FFh
        add    esp, 14h
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 05Ah, 002h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
        db  090h
loc_4_02C0:
        cmp    byte ptr [ebp + 2ch], 0
        je     SHORT loc_4_030B
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp + 30h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0F9h, 002h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 047h, 001h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 00Eh, 002h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
loc_4_030B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_04C0
        or     al, al
        jne    SHORT loc_4_0360
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0A6h, 002h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0F4h, 000h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0BBh, 001h, 0FCh, 0FFh
        mov    eax, 0c000009ah
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
        db  08Bh, 0FFh
loc_4_0360:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 149h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 080h, 04Dh, 001h, 000h, 000h, 020h, 016h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 151h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 155h], 0
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        db  0C7h, 080h, 059h, 001h, 000h, 000h, 090h, 00Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 15dh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 166h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0780
        or     al, al
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_4_03F0
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 064h, 000h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 02Bh, 001h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
        db  08Bh, 0FFh
loc_4_03F0:
        add    eax, 166h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0e9h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2fh
        push   eax
        db  068h, 080h, 01Ch, 000h, 000h
        db  068h, 050h, 01Ah, 000h, 000h
        db  068h, 070h, 01Bh, 000h, 000h
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        db  0E8h, 0E2h, 0FBh, 0FBh, 0FFh
        or     al, al
        jne    SHORT loc_4_0468
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 09Ch, 001h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0EAh, 0FFh, 0FBh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0B1h, 000h, 0FCh, 0FFh
        mov    eax, 0c000009ah
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
loc_4_0468:
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 080h, 0E1h, 000h, 000h, 000h, 020h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 024h, 037h, 000h, 000h
        mov    dword ptr [ecx + 0e5h], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 024h, 037h, 000h, 000h
        add    eax, 0e1h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0e1h
        db  0A3h, 024h, 037h, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
loc_4_04AA:
        mov    eax, 0c000009ah
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    2ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_04C0:
        push   ebp
        mov    ebp, esp
        push   esi
        db  0A1h, 0E0h, 035h, 000h, 000h
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 21h]
        lea    eax, [eax + eax*4 + 5]
        lea    eax, [eax + eax*4]
        push   eax
        lea    eax, [esi + 16ah]
        push   eax
        db  0E8h, 020h, 000h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0511
        push   0
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 0D7h, 000h, 0FCh, 0FFh
        add    esp, 14h
        push   esi
        db  0E8h, 076h, 005h, 0FCh, 0FFh
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0511:
        db  0A1h, 0E0h, 035h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [esi + 25h]
        shl    eax, 4
        push   eax
        lea    eax, [esi + 16eh]
        push   eax
        db  0E8h, 0DAh, 0FFh, 0FBh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0557
        push   1
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 091h, 000h, 0FCh, 0FFh
        add    esp, 14h
        push   esi
        db  0E8h, 030h, 005h, 0FCh, 0FFh
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0557:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_0560:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 59h], 1
        mov    byte ptr [eax + 8fh], 0
        mov    dword ptr [eax + 93h], 0
        mov    word ptr [eax + 0cbh], 2
        mov    byte ptr [eax + 54h], 1
        mov    byte ptr [eax + 53h], 1
        mov    byte ptr [eax + 50h], 0
        mov    byte ptr [eax + 51h], 0
        mov    dword ptr [eax + 97h], 12345678h
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_05B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 24h], 0
        sub    edi, edi
        sub    bl, bl
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0AFh, 0FFh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], edi
        je     SHORT loc_4_05E8
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_05E8:
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 086h, 0FEh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_0610
        mov    bl, 1
        mov    eax, 0aa00000ah
        jmp    NEAR PTR loc_4_0744
        db  08Dh, 064h, 024h, 000h
loc_4_0610:
        cmp    word ptr [ebp - 2ch], 6042h
        je     SHORT loc_4_0624
        mov    bl, 1
        mov    eax, 0aa000004h
        jmp    NEAR PTR loc_4_0744
loc_4_0624:
        mov    al, byte ptr [ebp - 2ah]
        and    al, 6
        shr    al, 1
        and    eax, 0ffh
        or     eax, eax
        je     SHORT loc_4_0645
        cmp    eax, 1
        je     SHORT loc_4_0650
        cmp    eax, 2
        je     SHORT loc_4_0658
        cmp    eax, 3
        je     SHORT loc_4_0660
        jmp    SHORT loc_4_0666
loc_4_0645:
        mov    word ptr [ebp - 2], 300h
        jmp    SHORT loc_4_0666
        db  08Dh, 049h, 000h
loc_4_0650:
        mov    word ptr [ebp - 2], 1300h
        jmp    SHORT loc_4_0666
loc_4_0658:
        mov    word ptr [ebp - 2], 2300h
        jmp    SHORT loc_4_0666
loc_4_0660:
        mov    word ptr [ebp - 2], 3300h
loc_4_0666:
        mov    al, byte ptr [ebp - 2ah]
        and    al, 18h
        shr    al, 3
        and    eax, 0ffh
        or     eax, eax
        je     SHORT loc_4_0688
        cmp    eax, 1
        je     SHORT loc_4_0691
        cmp    eax, 2
        je     SHORT loc_4_06A0
        cmp    eax, 3
        je     SHORT loc_4_06B0
        jmp    SHORT loc_4_06B7
loc_4_0688:
        mov    dword ptr [ebp - 10h], 0c0000h
        jmp    SHORT loc_4_06B7
loc_4_0691:
        mov    dword ptr [ebp - 10h], 0c8000h
        jmp    SHORT loc_4_06B7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_06A0:
        mov    dword ptr [ebp - 10h], 0d0000h
        jmp    SHORT loc_4_06B7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_06B0:
        mov    dword ptr [ebp - 10h], 0d8000h
loc_4_06B7:
        mov    al, byte ptr [ebp - 2ah]
        shr    al, 6
        and    eax, 0ffh
        or     eax, eax
        je     SHORT loc_4_06D7
        cmp    eax, 1
        je     SHORT loc_4_06E0
        cmp    eax, 2
        je     SHORT loc_4_06F0
        cmp    eax, 3
        je     SHORT loc_4_0700
        jmp    SHORT loc_4_0707
loc_4_06D7:
        mov    dword ptr [ebp - 0ch], 0ch
        jmp    SHORT loc_4_0707
loc_4_06E0:
        mov    dword ptr [ebp - 0ch], 7
        jmp    SHORT loc_4_0707
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_06F0:
        mov    dword ptr [ebp - 0ch], 3
        jmp    SHORT loc_4_0707
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0700:
        mov    dword ptr [ebp - 0ch], 9
loc_4_0707:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 070h, 0FDh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 20h], 6
        jne    SHORT loc_4_0741
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_0741
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, ecx
        lea    edi, [ebp - 34h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 34h], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 30h], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 2fh], al
loc_4_0741:
        mov    eax, dword ptr [ebp - 18h]
loc_4_0744:
        push   eax
        push   ebx
        push   20h
        push   10h
        push   edi
        mov    ax, word ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        db  0A1h, 030h, 037h, 000h, 000h
        push   eax
        call   sub_4_00F0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 01Ch, 0FEh, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_4_0780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 06Eh, 024h, 0FCh, 0FFh
        push   2
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        lea    edi, [esi + 131h]
        push   ebx
        db  068h, 090h, 00Eh, 000h, 000h
        push   esi
        db  068h, 0C0h, 00Dh, 000h, 000h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        lea    eax, [ebp - 4]
        push   edi
        push   eax
        db  0E8h, 031h, 0FDh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0803
        push   esi
        db  0E8h, 007h, 023h, 0FCh, 0FFh
        or     al, al
        je     SHORT loc_4_07D8
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_07D8:
        push   0
        push   0
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 0E9h, 0FDh, 0FBh, 0FFh
        add    esp, 14h
        push   edi
        db  0E8h, 0FCh, 0FCh, 0FBh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0803:
        push   ebx
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 0BFh, 0FDh, 0FBh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 032h, 045h, 06Eh, 068h
_PTEXT ends
        end
