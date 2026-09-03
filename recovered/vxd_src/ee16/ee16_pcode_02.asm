; ee16_pcode_02.asm -- EE16 object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        mov    word ptr [ebp - 18h], 5
        mov    word ptr [ebp - 16h], 5
        db  0C7h, 045h, 0ECh, 0D4h, 01Bh, 000h, 000h
        mov    word ptr [ebp - 10h], 0
        mov    word ptr [ebp - 0eh], 1
        mov    word ptr [ebp - 0ch], 628bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 08Ch, 00Fh, 0FEh, 0FFh
        lea    eax, [ebp - 10h]
        db  0A3h, 098h, 01Bh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 018h, 01Eh, 000h, 000h
        mov    byte ptr [ebp - 50h], 3
        mov    byte ptr [ebp - 4fh], 0
        db  0C7h, 045h, 0B6h, 070h, 008h, 000h, 000h
        db  0C7h, 045h, 0BAh, 0F0h, 009h, 000h, 000h
        db  0C7h, 045h, 0CAh, 050h, 012h, 000h, 000h
        db  0C7h, 045h, 0BEh, 060h, 007h, 000h, 000h
        db  0C7h, 045h, 0C2h, 030h, 00Bh, 000h, 000h
        db  0C7h, 045h, 0C6h, 050h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0CEh, 0E0h, 012h, 000h, 000h
        db  0C7h, 045h, 0D2h, 080h, 00Ah, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 001h, 000h, 000h
        db  0C7h, 045h, 0DAh, 090h, 005h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 22h], eax
        lea    eax, [ebp - 50h]
        mov    dword ptr [ebp - 1eh], ecx
        db  0C7h, 005h, 020h, 01Eh, 000h, 000h, 01Ch, 01Eh, 000h, 000h
        db  0C7h, 005h, 01Ch, 01Eh, 000h, 000h, 01Ch, 01Eh, 000h, 000h
        push   36h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 014h, 01Eh, 000h, 000h
        push   eax
        db  0E8h, 000h, 00Fh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_00F4
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0E7h, 00Eh, 0FEh, 0FFh
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_00F4:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_2_0100:
        push   ebp
        mov    ebp, esp
        sub    esp, 7ch
        push   esi
        push   edi
        mov    word ptr [ebp - 7ch], 8
        mov    word ptr [ebp - 7ah], 8
        db  0C7h, 045h, 088h, 0E4h, 01Bh, 000h, 000h
        mov    word ptr [ebp - 6ch], 4
        mov    word ptr [ebp - 6ah], 4
        db  0C7h, 045h, 098h, 0F0h, 01Bh, 000h, 000h
        mov    word ptr [ebp - 74h], 0ah
        mov    word ptr [ebp - 72h], 0ah
        db  0C7h, 045h, 090h, 000h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 64h], 0ch
        mov    word ptr [ebp - 62h], 0ch
        db  0C7h, 045h, 0A0h, 018h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 44h], 8
        mov    word ptr [ebp - 42h], 8
        db  0C7h, 045h, 0C0h, 02Ch, 01Ch, 000h, 000h
        mov    word ptr [ebp - 54h], 13h
        mov    word ptr [ebp - 52h], 13h
        db  0C7h, 045h, 0B0h, 048h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 5ch], 14h
        mov    word ptr [ebp - 5ah], 14h
        db  0C7h, 045h, 0A8h, 070h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 4ch], 14h
        mov    word ptr [ebp - 4ah], 14h
        db  0C7h, 045h, 0B8h, 098h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 3ch], 6
        mov    word ptr [ebp - 3ah], 6
        db  0C7h, 045h, 0C8h, 0B4h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 34h], 5
        mov    word ptr [ebp - 32h], 5
        db  0C7h, 045h, 0D0h, 0C4h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 2ch], 6
        mov    word ptr [ebp - 2ah], 6
        db  0C7h, 045h, 0D8h, 0D4h, 01Ch, 000h, 000h
        db  0A1h, 094h, 01Bh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   25dh
        push   eax
        db  0E8h, 0C7h, 00Eh, 0FEh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_2_0203
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0203:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 97h
        rep stosd
        stosb
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 014h, 01Eh, 000h, 000h
        mov    dword ptr [ecx + 1aeh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 155h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 27h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 192h
        mov    dword ptr [ecx + 196h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 196h]
        mov    dword ptr [ecx + 192h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 19ah
        mov    dword ptr [ecx + 19eh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 19eh]
        mov    dword ptr [ecx + 19ah], eax
        lea    eax, [ebp - 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0AEh, 00Eh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_02A6
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 023h, 00Eh, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_02A6:
        push   1
        lea    eax, [ebp - 7ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 07Dh, 00Eh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_02D1
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 2], eax
        jmp    SHORT loc_2_02DB
loc_2_02D1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2], 2
loc_2_02DB:
        push   1
        lea    eax, [ebp - 74h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 048h, 00Eh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0306
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ecx], ax
        jmp    SHORT loc_2_030E
loc_2_0306:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 310h
loc_2_030E:
        push   0
        lea    eax, [ebp - 6ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 015h, 00Eh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0340
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ecx + 1e7h], ax
        jmp    SHORT loc_2_034C
        db  08Dh, 049h, 000h
loc_2_0340:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1e7h], 5
loc_2_034C:
        mov    eax, dword ptr [ebp - 4]
        mov    cl, 1fh
        sub    cl, byte ptr [eax + 1e7h]
        mov    byte ptr [eax + 1e6h], cl
        push   2
        lea    eax, [ebp - 64h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0C6h, 00Dh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_03E7
        push   1
        lea    eax, [ebp - 5ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        db  0E8h, 0BAh, 00Dh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_039B
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 9], 1
        jmp    SHORT loc_2_03E7
loc_2_039B:
        push   1
        lea    eax, [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        db  0E8h, 099h, 00Dh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_03C0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 9], 3
        jmp    SHORT loc_2_03E7
        db  08Dh, 064h, 024h, 000h
loc_2_03C0:
        push   1
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        db  0E8h, 074h, 00Dh, 0FEh, 0FFh
        or     al, al
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_03E1
        mov    word ptr [eax + 9], 2
        jmp    SHORT loc_2_03E7
loc_2_03E1:
        mov    word ptr [eax + 9], 0ffffh
loc_2_03E7:
        push   2
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 03Ch, 00Dh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0465
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        db  0E8h, 030h, 00Dh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0425
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0bh], 1
        jmp    SHORT loc_2_0465
loc_2_0425:
        push   1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        db  0E8h, 00Fh, 00Dh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0446
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0bh], 2
        jmp    SHORT loc_2_0465
loc_2_0446:
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        db  0E8h, 0EEh, 00Ch, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0465
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0bh], 3
loc_2_0465:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0C2h, 00Bh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_2_04A1
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_04A1
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, ecx
        add    edx, 2eh
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_2_04A1:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 096h, 00Ch, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 2], 3
        jne    NEAR PTR loc_2_055D
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 067h, 00Bh, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_055D
        cmp    word ptr [ebp - 24h], 628bh
        jne    NEAR PTR loc_2_055D
        mov    al, byte ptr [ebp - 21h]
        and    eax, 0fh
        mov    ecx, dword ptr [ebp - 4]
        db  066h, 08Bh, 004h, 045h, 09Ch, 01Bh, 000h, 000h
        mov    word ptr [ecx], ax
        mov    al, byte ptr [ebp - 21h]
        and    al, 70h
        shr    al, 3
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 4]
        db  066h, 08Bh, 080h, 0BCh, 01Bh, 000h, 000h
        mov    word ptr [ecx + 1e7h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, 1fh
        sub    cl, byte ptr [eax + 1e7h]
        mov    byte ptr [eax + 1e6h], cl
        mov    al, byte ptr [ebp - 22h]
        and    al, 4
        shr    al, 2
        mov    ecx, dword ptr [ebp - 4]
        and    ax, 0ffh
        mov    word ptr [ecx + 9], ax
        test   byte ptr [ebp - 22h], 4
        jne    SHORT loc_2_0546
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 9], 1
        jmp    SHORT loc_2_055D
loc_2_0546:
        test   byte ptr [ebp - 21h], 80h
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_2_0557
        mov    word ptr [eax + 9], 2
        jmp    SHORT loc_2_055D
loc_2_0557:
        mov    word ptr [eax + 9], 3
loc_2_055D:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0610
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_2_057F
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 03Dh, 00Bh, 0FEh, 0FFh
loc_2_057F:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0590:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        mov    byte ptr [esi + 155h], 1
        push   eax
        lea    eax, [esi + 1ceh]
        push   eax
        db  0E8h, 0E7h, 00Ah, 0FEh, 0FFh
        lea    eax, [esi + 1b6h]
        push   eax
        db  0E8h, 0E7h, 00Ah, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 1e9h]
        push   eax
        call   sub_2_2170
        mov    eax, dword ptr [esi + 1b2h]
        push   eax
        db  0E8h, 013h, 00Ah, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 1a2h]
        mov    ecx, dword ptr [esi + 1a6h]
        lea    edx, [esi + 1a2h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0
        push   esi
        db  0E8h, 0C1h, 00Ah, 0FEh, 0FFh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 094h, 01Bh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   56h
        push   eax
        db  0E8h, 08Ah, 00Ah, 0FEh, 0FFh
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 0
        cmp    dword ptr [esi + 2], 3
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_0660
        mov    dword ptr [eax + 0ah], 1
        jmp    SHORT loc_2_0667
        db  090h
loc_2_0660:
        mov    dword ptr [eax + 0ah], 3
loc_2_0667:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 12h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 16h], 5
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [esi]
        mov    dword ptr [ecx + 1ah], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1eh], 10h
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 3000h
        mov    dword ptr [ecx + 26h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2ah], 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2eh], 0
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 4000h
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 36h], 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 3ah], 0
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 8000h
        mov    dword ptr [ecx + 3eh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 42h], 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 46h], 0
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0c000h
        mov    dword ptr [ecx + 4ah], eax
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [esi + 1b2h]
        mov    dword ptr [eax + 4eh], 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 52h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 1aeh]
        push   eax
        push   ebx
        db  0E8h, 098h, 008h, 0FEh, 0FFh
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_0768
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 05Fh, 009h, 0FEh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0768:
        push   esi
        call   sub_2_0B00
        push   esi
        call   sub_2_0C10
        push   esi
        call   sub_2_0EF0
        or     al, al
        jne    SHORT loc_2_07C0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 031h, 009h, 0FEh, 0FFh
        push   0
        push   1
        push   2
        push   0aa000003h
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 0EAh, 009h, 0FEh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 03Fh, 008h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_07C0:
        mov    dword ptr [esi + 1ceh], 0
        db  0C7h, 086h, 0D2h, 001h, 000h, 000h, 0C0h, 006h, 000h, 000h
        mov    dword ptr [esi + 1d6h], esi
        lea    eax, [esi + 1e9h]
        push   eax
        lea    eax, [esi + 1aah]
        push   eax
        lea    eax, [esi + 2eh]
        push   eax
        db  068h, 030h, 01Bh, 000h, 000h
        db  068h, 070h, 01Ah, 000h, 000h
        db  068h, 0E0h, 01Ah, 000h, 000h
        push   10h
        call   sub_2_2050
        or     al, al
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jne    SHORT loc_2_0830
        db  0E8h, 0A9h, 008h, 0FEh, 0FFh
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 0CDh, 007h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0830:
        db  0E8h, 087h, 008h, 0FEh, 0FFh
        db  0C7h, 086h, 0A2h, 001h, 000h, 000h, 01Ch, 01Eh, 000h, 000h
        lea    ecx, [esi + 1a2h]
        db  0A1h, 020h, 01Eh, 000h, 000h
        mov    dword ptr [esi + 1a6h], eax
        db  0A1h, 020h, 01Eh, 000h, 000h
        pop    edi
        mov    dword ptr [eax], ecx
        sub    eax, eax
        db  089h, 00Dh, 020h, 01Eh, 000h, 000h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 155h], 0
        je     SHORT loc_2_0890
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
loc_2_0890:
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, edx
        je     SHORT loc_2_08B1
        mov    eax, dword ptr [ebp + 14h]
        lea    esp, [esp]
loc_2_08A0:
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_08AD
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_2_08A0
loc_2_08AD:
        cmp    ecx, edx
        jne    SHORT loc_2_08C0
loc_2_08B1:
        mov    eax, 0c0010019h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h
loc_2_08C0:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], edx
        inc    dword ptr [esi + 137h]
        db  0A1h, 094h, 01Bh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   47h
        push   eax
        db  0E8h, 0D8h, 007h, 0FEh, 0FFh
        or     eax, eax
        jne    NEAR PTR loc_2_09E9
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 41h], 100h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 43h], 0
        mov    word ptr [esi + 1edh], 100h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 1e9h]
        push   eax
        call   sub_2_21C0
        or     al, al
        jne    SHORT loc_2_0980
        mov    esi, 0c0000001h
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 04Bh, 007h, 0FEh, 0FFh
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0980:
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [esi + 192h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 196h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 196h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 196h], eax
        inc    dword ptr [esi + 18eh]
        cmp    byte ptr [esi + 136h], 0
        je     SHORT loc_2_09E5
        mov    byte ptr [esi + 136h], 0
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, byte ptr [esi + 8]
        or     al, 8
        out    dx, al
        push   esi
        call   sub_2_1530
        push   0ea60h
        add    esi, 1ceh
        push   esi
        db  0E8h, 0ADh, 006h, 0FEh, 0FFh
loc_2_09E5:
        sub    esi, esi
        mov    eax, esi
loc_2_09E9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_09F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 18h], al
        jne    SHORT loc_2_0A6F
        mov    byte ptr [esi + 18h], 1
        dec    dword ptr [edi + 18eh]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 1e9h]
        push   eax
        call   sub_2_2290
        cmp    eax, 0c001000eh
        jne    SHORT loc_2_0A30
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_0A30:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [esi + 14h]
        push   edi
        call   sub_2_0AB0
        push   0
        push   0
        push   esi
        db  0E8h, 06Ah, 006h, 0FEh, 0FFh
        cmp    dword ptr [edi + 18eh], 0
        jne    SHORT loc_2_0A6D
        mov    dx, word ptr [edi]
        add    dx, 7
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    byte ptr [edi + 136h], 1
loc_2_0A6D:
        sub    eax, eax
loc_2_0A6F:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 014h, 01Eh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 047h, 005h, 0FEh, 0FFh
        push   0
        db  0A1h, 018h, 01Eh, 000h, 000h
        push   eax
        db  0E8h, 02Eh, 005h, 0FEh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        sub    si, si
        mov    ecx, dword ptr [edx + 192h]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0fffffe6eh
        je     SHORT loc_2_0AE9
        add    eax, 0
loc_2_0AD0:
        mov    ax, word ptr [ecx + 41h]
        cmp    si, ax
        jae    SHORT loc_2_0ADC
        mov    si, ax
loc_2_0ADC:
        mov    ecx, dword ptr [ecx]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0fffffe6eh
        jne    SHORT loc_2_0AD0
loc_2_0AE9:
        mov    word ptr [edx + 1edh], si
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0B00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18eh], 0
        mov    dword ptr [eax + 137h], 1
        mov    byte ptr [eax + 154h], 0
        mov    dword ptr [eax + 156h], 0
        mov    byte ptr [eax + 136h], 1
        mov    byte ptr [eax + 135h], 1
        mov    dword ptr [eax + 15ah], 12345678h
        mov    dword ptr [eax + 15eh], 0
        mov    dword ptr [eax + 162h], 0
        mov    dword ptr [eax + 16ah], 0
        mov    dword ptr [eax + 16eh], 0
        mov    dword ptr [eax + 172h], 0
        mov    dword ptr [eax + 176h], 0
        mov    dword ptr [eax + 17ah], 0
        mov    dword ptr [eax + 17eh], 0
        mov    dword ptr [eax + 182h], 0
        mov    dword ptr [eax + 186h], 0
        mov    dword ptr [eax + 18ah], 0
        mov    word ptr [eax + 25h], 0
        mov    byte ptr [eax + 1f1h], 0ch
        mov    byte ptr [eax + 1f2h], 0eh
        mov    byte ptr [eax + 1f3h], 40h
        mov    byte ptr [eax + 1f4h], 26h
        mov    byte ptr [eax + 1f5h], 0
        mov    byte ptr [eax + 1f6h], 60h
        mov    byte ptr [eax + 1f7h], 0
        mov    byte ptr [eax + 1f8h], 0f2h
        mov    byte ptr [eax + 1f9h], 0
        mov    byte ptr [eax + 1fah], 0
        mov    byte ptr [eax + 1fbh], 40h
        mov    byte ptr [eax + 1fch], 0
        pop    ebp
        ret    4
        db  090h
sub_2_0C10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1bh], 0
        mov    word ptr [eax + 25h], 0
        mov    dword ptr [eax + 143h], 0
        mov    dword ptr [eax + 147h], 0
        mov    dword ptr [eax + 14bh], 0
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    ebx, dword ptr [esi + 8]
        cmp    byte ptr [ebx + 154h], 0
        jne    SHORT loc_2_0CB0
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_2_0CA0
        mov    byte ptr [ebx + 153h], 1
        push   ebx
        call   sub_2_0D70
        push   ebx
        call   sub_2_0CD0
        push   ebx
        call   sub_2_0E40
        push   esi
        push   ebx
        call   sub_2_0CC0
        push   edi
        push   ebx
        call   sub_2_0DD0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0CA0:
        mov    edi, 0c0010002h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_0CB0:
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_0CC0:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0CD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 143h], 0
        je     SHORT loc_2_0D1D
        sub    edi, edi
loc_2_0CE4:
        mov    edx, dword ptr [esi + 143h]
        cmp    dword ptr [edx + 24h], edi
        lea    eax, [edx + 24h]
        jne    SHORT loc_2_0CF8
        mov    dword ptr [esi + 147h], edi
loc_2_0CF8:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 143h], ecx
        mov    ebx, dword ptr [eax + 8]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 0E8h, 002h, 0FEh, 0FFh
        dec    dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 143h], edi
        jne    SHORT loc_2_0CE4
loc_2_0D1D:
        cmp    dword ptr [esi + 13bh], 0
        je     SHORT loc_2_0D61
        sub    edi, edi
loc_2_0D28:
        mov    edx, dword ptr [esi + 13bh]
        cmp    dword ptr [edx + 24h], edi
        lea    eax, [edx + 24h]
        jne    SHORT loc_2_0D3C
        mov    dword ptr [esi + 13fh], edi
loc_2_0D3C:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 13bh], ecx
        mov    ebx, dword ptr [eax + 8]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 0A4h, 002h, 0FEh, 0FFh
        dec    dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 13bh], edi
        jne    SHORT loc_2_0D28
loc_2_0D61:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0D70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, edi
        mov    esi, dword ptr [edi + 192h]
        sub    eax, esi
        cmp    eax, 0fffffe6eh
        je     SHORT loc_2_0DB7
loc_2_0D89:
        inc    dword ptr [esi + 14h]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [esi + 10h]
        push   eax
        db  0E8h, 080h, 002h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 10h]
        push   eax
        db  0E8h, 07Dh, 002h, 0FEh, 0FFh
        dec    dword ptr [esi + 14h]
        mov    esi, dword ptr [esi]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0fffffe6eh
        jne    SHORT loc_2_0D89
loc_2_0DB7:
        mov    byte ptr [edi + 154h], 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 192h]
        sub    eax, edi
        cmp    eax, 0fffffe6eh
        je     SHORT loc_2_0E2C
        sub    ebx, ebx
        lea    esp, [esp]
loc_2_0DF0:
        inc    dword ptr [edi + 14h]
        cmp    dword ptr [ebp + 0ch], ebx
        je     SHORT loc_2_0E08
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        db  0E8h, 016h, 002h, 0FEh, 0FFh
loc_2_0E08:
        push   4
        lea    eax, [ebp + 0ch]
        push   eax
        push   40010005h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        db  0E8h, 002h, 002h, 0FEh, 0FFh
        dec    dword ptr [edi + 14h]
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0fffffe6eh
        jne    SHORT loc_2_0DF0
loc_2_0E2C:
        mov    byte ptr [esi + 154h], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, 0ah
        mov    dx, word ptr [edi]
        add    dx, 7
        mov    al, byte ptr [edi + 8]
        out    dx, al
        lea    esi, [edi + 1ceh]
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        db  0E8h, 028h, 002h, 0FEh, 0FFh
loc_2_0E70:
        cmp    word ptr [edi + 1bh], 0
        je     SHORT loc_2_0E84
        push   edi
        db  0E8h, 083h, 0FBh, 0FDh, 0FFh
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     SHORT loc_2_0E70
loc_2_0E84:
        push   edi
        call   sub_2_1580
        cmp    dword ptr [edi + 143h], 0
        je     SHORT loc_2_0EA2
loc_2_0E93:
        push   edi
        db  0E8h, 027h, 0F2h, 0FDh, 0FFh
        cmp    dword ptr [edi + 143h], 0
        jne    SHORT loc_2_0E93
loc_2_0EA2:
        push   edi
        call   sub_2_0C10
        mov    cx, word ptr [edi]
        add    cx, 0eh
        mov    dx, cx
        mov    al, 80h
        out    dx, al
        push   edi
        call   sub_2_1180
        mov    bl, al
        mov    cx, word ptr [edi]
        add    cx, 7
        mov    al, byte ptr [edi + 8]
        or     al, 8
        mov    dx, cx
        out    dx, al
        push   edi
        call   sub_2_1530
        push   1388h
        push   esi
        db  0E8h, 0B4h, 001h, 0FEh, 0FFh
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0EF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], 0
        mov    ax, word ptr [esi]
        push   eax
        mov    eax, dword ptr [esi + 1b2h]
        push   eax
        call   sub_2_1810
        mov    word ptr [esi + 0dh], ax
        sub    eax, eax
        mov    cx, word ptr [esi]
        db  066h, 039h, 00Dh, 09Ch, 01Bh, 000h, 000h
        je     SHORT loc_2_0F39
        db  0BAh, 09Ch, 01Bh, 000h, 000h
        nop
loc_2_0F30:
        add    edx, 2
        inc    eax
        cmp    word ptr [edx], cx
        jne    SHORT loc_2_0F30
loc_2_0F39:
        mov    word ptr [ebp - 2], ax
        cmp    dword ptr [esi + 2], 3
        mov    ax, word ptr [esi]
        mov    word ptr [ebp - 6], ax
        je     SHORT loc_2_0FB9
        cmp    word ptr [esi + 0dh], 0
        jne    SHORT loc_2_0F9D
        db  066h, 08Bh, 03Dh, 0ECh, 01Dh, 000h, 000h
        or     di, di
        je     SHORT loc_2_0F9D
        db  0BBh, 0ECh, 01Dh, 000h, 000h
loc_2_0F62:
        mov    ax, word ptr [ebx]
        push   eax
        mov    eax, dword ptr [esi + 1b2h]
        push   eax
        call   sub_2_1810
        mov    word ptr [esi + 0dh], ax
        or     ax, ax
        jne    SHORT loc_2_0F92
        mov    di, word ptr [ebx + 2]
        add    ebx, 2
        or     di, di
        jne    SHORT loc_2_0F62
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0F92:
        mov    word ptr [esi], di
        sub    di, di
        or     di, di
        jne    SHORT loc_2_0F62
loc_2_0F9D:
        xor    eax, eax
        mov    ax, word ptr [esi + 1e7h]
        sub    eax, 2
        cmp    eax, 0dh
        ja     NEAR PTR loc_2_1135
        db  0FFh, 024h, 085h, 040h, 011h, 000h, 000h
loc_2_0FB9:
        cmp    word ptr [esi + 0dh], 0
        jne    SHORT loc_2_0F9D
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0FCB:
        mov    al, 1
        jmp    SHORT loc_2_1006
        db  090h
loc_2_0FD0:
        mov    al, 2
        jmp    SHORT loc_2_1006
loc_2_0FD4:
        mov    al, 3
        jmp    SHORT loc_2_1006
loc_2_0FD8:
        mov    al, 4
        jmp    SHORT loc_2_1006
loc_2_0FDC:
        mov    al, 1
        jmp    SHORT loc_2_1006
loc_2_0FE0:
        mov    al, 5
        jmp    SHORT loc_2_1006
loc_2_0FE4:
        mov    al, 6
        jmp    SHORT loc_2_1006
loc_2_0FE8:
        cmp    word ptr [esi + 0dh], 0babch
        jne    NEAR PTR loc_2_1135
        sub    al, al
        jmp    SHORT loc_2_1006
loc_2_0FF8:
        cmp    word ptr [esi + 0dh], 0babch
        jne    NEAR PTR loc_2_1135
        mov    al, 7
loc_2_1006:
        mov    byte ptr [esi + 8], al
        and    ax, 0ffh
        shl    ax, 0dh
        or     word ptr [ebp - 2], ax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1AD0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1B30
        mov    cx, word ptr [esi]
        add    cx, 7
        mov    dx, cx
        mov    al, byte ptr [esi + 8]
        out    dx, al
        push   esi
        call   sub_2_1580
        mov    ax, word ptr [esi]
        add    ax, 0eh
        mov    dx, ax
        mov    al, 80h
        out    dx, al
        mov    ax, word ptr [ebp - 6]
        push   eax
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   esi
        call   sub_2_19F0
        push   esi
        call   sub_2_1C10
        push   esi
        call   sub_2_1950
        push   esi
        call   sub_2_1870
        cmp    dword ptr [esi + 2], 3
        je     SHORT loc_2_1086
        mov    eax, 1
        jmp    SHORT loc_2_108B
loc_2_1086:
        mov    eax, 2
loc_2_108B:
        sub    cl, cl
        xor    edx, edx
        mov    dx, word ptr [esi + 1e7h]
        push   eax
        push   ecx
        push   edx
        push   edx
        lea    edi, [esi + 1b6h]
        db  068h, 0F0h, 003h, 000h, 000h
        push   esi
        db  068h, 0D0h, 003h, 000h, 000h
        mov    eax, dword ptr [esi + 1b2h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   edi
        push   eax
        db  0E8h, 0E2h, 0FFh, 0FDh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_1105
        push   esi
        call   sub_2_1180
        or     al, al
        jne    SHORT loc_2_10FA
        push   0
        db  068h, 0F0h, 00Eh, 000h, 000h
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 1b2h]
        push   eax
        db  0E8h, 0A2h, 000h, 0FEh, 0FFh
        add    esp, 14h
        push   edi
        db  0E8h, 0B5h, 0FFh, 0FDh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_10FA:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1105:
        xor    eax, eax
        mov    ax, word ptr [esi + 1e7h]
        push   eax
        db  068h, 0F0h, 00Eh, 000h, 000h
        push   2
        push   0aa000005h
        mov    eax, dword ptr [esi + 1b2h]
        push   eax
        db  0E8h, 061h, 000h, 0FEh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1135:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  0CBh, 00Fh, 000h, 000h, 0D0h, 00Fh, 000h, 000h, 0D4h, 00Fh, 000h, 000h, 0D8h, 00Fh, 000h, 000h
        db  035h, 011h, 000h, 000h, 035h, 011h, 000h, 000h, 035h, 011h, 000h, 000h, 0DCh, 00Fh, 000h, 000h
        db  0E0h, 00Fh, 000h, 000h, 0E4h, 00Fh, 000h, 000h, 0E8h, 00Fh, 000h, 000h, 035h, 011h, 000h, 000h
        db  035h, 011h, 000h, 000h, 0F8h, 00Fh, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, 0fff6h
        mov    di, word ptr [esi]
        lea    ecx, [edi + 2]
        mov    dx, cx
        out    dx, ax
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        mov    dx, cx
        out    dx, ax
        mov    dx, di
        mov    ax, 1
        out    dx, ax
        mov    ax, 8
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        mov    dx, cx
        mov    ax, 8
        out    dx, ax
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        mov    word ptr [esi + 15h], 0e0h
        mov    word ptr [esi + 13h], 0e0h
        mov    word ptr [esi + 17h], 0e0h
        mov    di, 0e0h
        mov    dx, word ptr [esi]
        sub    dx, 3ff4h
        mov    ax, word ptr [esi + 13h]
        out    dx, ax
        sub    cx, cx
        cmp    word ptr [esi + 0fh], cx
        jbe    SHORT loc_2_1284
loc_2_1212:
        mov    bx, word ptr [esi]
        mov    ax, di
        lea    edx, [ebx + 2]
        out    dx, ax
        mov    dx, bx
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        mov    ax, di
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        mov    ax, 0ffffh
        out    dx, ax
        lea    eax, [edi + 1eh]
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        mov    ax, 2004h
        out    dx, ax
        mov    ax, word ptr [esi + 0fh]
        sub    ax, cx
        cmp    ax, 1
        jne    SHORT loc_2_1260
        mov    ax, 0e0h
        jmp    SHORT loc_2_1269
        db  08Dh, 064h, 024h, 000h
loc_2_1260:
        lea    eax, [edi + 600h]
        mov    dx, bx
loc_2_1269:
        out    dx, ax
        lea    eax, [edi + 6]
        mov    dx, bx
        out    dx, ax
        mov    word ptr [esi + 19h], di
        add    di, 600h
        inc    cx
        cmp    word ptr [esi + 0fh], cx
        ja     SHORT loc_2_1212
loc_2_1284:
        mov    word ptr [esi + 1dh], di
        mov    word ptr [esi + 21h], di
        mov    dx, word ptr [esi]
        sub    dx, 3ff2h
        mov    ax, di
        add    ax, 0ah
        out    dx, ax
        sub    cx, cx
        cmp    word ptr [esi + 11h], cx
        jbe    SHORT loc_2_131E
loc_2_12A6:
        mov    bx, word ptr [esi]
        mov    ax, di
        lea    edx, [ebx + 2]
        out    dx, ax
        mov    dx, bx
        sub    ax, ax
        out    dx, ax
        mov    ax, 0ffffh
        out    dx, ax
        lea    eax, [edi + 20h]
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        mov    ax, 85dch
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        mov    ax, word ptr [esi + 11h]
        sub    ax, cx
        cmp    ax, 1
        jne    SHORT loc_2_12F1
        mov    ax, 8000h
        out    dx, ax
        mov    ax, word ptr [esi + 21h]
        add    ax, 0ah
        jmp    SHORT loc_2_12FF
loc_2_12F1:
        mov    dx, bx
        sub    ax, ax
        out    dx, ax
        lea    eax, [edi + 60ah]
loc_2_12FF:
        out    dx, ax
        mov    dx, bx
        mov    ax, di
        out    dx, ax
        mov    word ptr [esi + 23h], di
        mov    word ptr [esi + 1fh], di
        add    di, 600h
        inc    cx
        cmp    word ptr [esi + 11h], cx
        ja     SHORT loc_2_12A6
loc_2_131E:
        mov    dx, word ptr [esi]
        add    dx, 0dh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 82h
        mov    cx, word ptr [esi]
        mov    al, byte ptr [ebp - 1]
        lea    edx, [ecx + 0dh]
        out    dx, al
        lea    edx, [ecx + 0eh]
        sub    al, al
        out    dx, al
        push   esi
        call   sub_2_15C0
        or     al, al
        jne    SHORT loc_2_1352
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1352:
        push   esi
        call   sub_2_15E0
        or     al, al
        jne    SHORT loc_2_1367
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1367:
        push   esi
        call   sub_2_1620
        or     al, al
        jne    SHORT loc_2_1380
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_1380:
        push   esi
        call   sub_2_1700
        or     al, al
        jne    SHORT loc_2_1395
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1395:
        mov    dx, word ptr [esi]
        add    dx, 0dh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0fdh
        or     byte ptr [ebp - 1], 80h
        mov    dx, word ptr [esi]
        add    dx, 0dh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_13C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 15h]
        lea    eax, [edi + 600h]
        cmp    word ptr [esi + 19h], ax
        mov    word ptr [esi + 15h], ax
        jae    SHORT loc_2_13E5
        mov    ax, word ptr [esi + 17h]
        mov    word ptr [esi + 15h], ax
loc_2_13E5:
        mov    cx, word ptr [esi]
        mov    ax, 20h
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        mov    ax, 2002h
        out    dx, ax
        mov    ax, word ptr [esi + 15h]
        out    dx, ax
        push   6
        lea    eax, [esi + 1f1h]
        push   eax
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        db  0E8h, 049h, 0FBh, 0FDh, 0FFh
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 15h]
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 4004h]
        out    dx, ax
        lea    edx, [ecx + 8]
        mov    ax, di
        out    dx, ax
        lea    edx, [ecx + 4004h]
        inc    word ptr [esi + 1bh]
        mov    ax, 20h
        out    dx, ax
        lea    edx, [ecx + 400eh]
        mov    ax, 0a000h
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_1460:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 15h]
        mov    word ptr [ebp - 2], ax
        add    ax, 600h
        cmp    word ptr [esi + 19h], ax
        mov    word ptr [esi + 15h], ax
        jae    SHORT loc_2_148A
        mov    ax, word ptr [esi + 17h]
        mov    word ptr [esi + 15h], ax
loc_2_148A:
        mov    cx, word ptr [esi]
        mov    ax, 40h
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        mov    ax, 2003h
        out    dx, ax
        mov    ax, word ptr [esi + 15h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0ch]
        imul   ax, ax, 6
        out    dx, ax
        sub    bx, bx
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_2_14E9
        mov    edi, dword ptr [ebp + 10h]
        nop
loc_2_14C0:
        mov    eax, ebx
        and    eax, 0ffffh
        push   3
        lea    eax, [eax + eax*2]
        lea    eax, [edi + eax*2]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        inc    bx
        db  0E8h, 087h, 0FAh, 0FDh, 0FFh
        mov    eax, ebx
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp + 0ch]
        jb     SHORT loc_2_14C0
loc_2_14E9:
        mov    di, word ptr [esi]
        mov    ax, word ptr [esi + 15h]
        lea    ecx, [edi + 8]
        mov    dx, cx
        out    dx, ax
        lea    ebx, [edi + 4004h]
        mov    dx, bx
        out    dx, ax
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        inc    word ptr [esi + 1bh]
        mov    dx, bx
        mov    ax, 40h
        out    dx, ax
        lea    edx, [edi + 400eh]
        mov    ax, 0a000h
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_2_1530:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 0D3h, 0F1h, 0FDh, 0FFh
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 13h]
        lea    edx, [ecx - 3ff4h]
        out    dx, ax
        lea    edx, [ecx - 3ff2h]
        mov    ax, word ptr [esi + 1dh]
        add    ax, 0ah
        out    dx, ax
        lea    edx, [ecx - 3ff6h]
        mov    ax, 110h
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1580:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, byte ptr [esi + 8]
        out    dx, al
        db  0E8h, 078h, 0F1h, 0FDh, 0FFh
        mov    cx, word ptr [esi]
        mov    ax, 440h
        lea    edx, [ecx - 3ff6h]
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   esi
        db  0E8h, 05Dh, 0F1h, 0FDh, 0FFh
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_15C0:
        push   ebp
        mov    ebp, esp
        push   0a000h
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1780
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_15E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   8002h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1740
        push   0ch
        push   26h
        lea    eax, [esi + 1f1h]
        push   eax
        push   esi
        db  0E8h, 05Dh, 0EAh, 0FDh, 0FFh
        push   2000h
        push   100h
        push   esi
        call   sub_2_1780
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   8007h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1740
        push   2000h
        push   100h
        push   esi
        call   sub_2_1780
        or     al, al
        jne    SHORT loc_2_1652
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1652:
        mov    cx, word ptr [esi]
        mov    ax, 20h
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 4000h]
        in     ax, dx
        test   ah, 8
        je     SHORT loc_2_1674
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1674:
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   8003h
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1740
        mov    cx, word ptr [edi]
        mov    ax, 26h
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dx, cx
        imul   ax, ax, 6
        out    dx, ax
        sub    bx, bx
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_2_16E2
        mov    esi, dword ptr [ebp + 10h]
loc_2_16B8:
        mov    eax, ebx
        and    eax, 0ffffh
        push   3
        lea    eax, [eax + eax*2]
        movsx  eax, byte ptr [esi + eax*2]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi]
        push   eax
        inc    bx
        db  0E8h, 08Eh, 0F8h, 0FDh, 0FFh
        mov    eax, ebx
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp + 0ch]
        jb     SHORT loc_2_16B8
loc_2_16E2:
        push   2000h
        push   100h
        push   edi
        call   sub_2_1780
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1700:
        push   ebp
        mov    ebp, esp
        push   esi
        push   8001h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1740
        push   6
        push   26h
        lea    eax, [esi + 2eh]
        push   eax
        push   esi
        db  0E8h, 040h, 0E9h, 0FDh, 0FFh
        push   2000h
        push   100h
        push   esi
        call   sub_2_1780
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1740:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax]
        mov    ax, 20h
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 4002h]
        mov    ax, word ptr [ebp + 0ch]
        out    dx, ax
        lea    edx, [ecx + 4004h]
        mov    ax, 20h
        out    dx, ax
        lea    edx, [ecx - 3ff4h]
        out    dx, ax
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    bx, bx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 07Bh, 0EFh, 0FDh, 0FFh
        cmp    al, bl
        je     SHORT loc_2_17DE
        mov    cx, word ptr [esi]
        mov    ax, word ptr [ebp + 0ch]
        lea    edx, [ecx - 3ff6h]
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        mov    di, word ptr [ebp + 10h]
loc_2_17B2:
        mov    dx, word ptr [esi]
        sub    dx, 3ff8h
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    ax, 0f000h
        cmp    ax, di
        je     SHORT loc_2_17E9
        push   3e8h
        db  0E8h, 0DDh, 0F8h, 0FDh, 0FFh
        mov    ax, bx
        inc    bx
        cmp    ax, 64h
        jne    SHORT loc_2_17B2
loc_2_17DE:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_17E9:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [ebp - 2]
        lea    edx, [ecx - 3ff6h]
        and    ax, 0f000h
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_2_1810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        sub    esi, esi
        mov    dx, word ptr [ebp + 0ch]
        add    dx, 300fh
        mov    bl, 4
loc_2_1825:
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 3
        lea    ecx, [eax*4]
        shr    byte ptr [ebp - 1], 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        shl    eax, cl
        or     esi, eax
        dec    bl
        jne    SHORT loc_2_1825
        mov    eax, esi
        and    eax, 0fff0h
        cmp    eax, 0bab0h
        je     SHORT loc_2_185C
        sub    ax, ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_185C:
        mov    ax, si
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [esi]
        add    dx, 2
        sub    ax, ax
        out    dx, ax
        mov    word ptr [ebp - 2], ax
        mov    ecx, 10h
        lea    ebx, [ebx]
loc_2_18A0:
        mov    dx, word ptr [esi]
        inc    word ptr [ebp - 2]
        sub    ax, ax
        cmp    word ptr [ebp - 2], cx
        out    dx, ax
        jb     SHORT loc_2_18A0
        mov    dx, word ptr [esi]
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        sub    ax, ax
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        mov    al, 55h
        out    dx, al
        lea    edx, [ecx + 0fh]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        mov    ax, 8000h
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        mov    al, 55h
        out    dx, al
        lea    edx, [ecx + 0fh]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        sub    ax, ax
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        mov    al, 0aah
        out    dx, al
        lea    edx, [ecx + 0fh]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        mov    ax, 8000h
        lea    edx, [ecx + 4]
        out    dx, ax
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    word ptr [ebp - 2], 0aah
        jne    SHORT loc_2_1934
        mov    word ptr [esi + 0fh], 8
        mov    word ptr [esi + 11h], 0dh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1934:
        cmp    word ptr [ebp - 2], 55h
        jne    SHORT loc_2_1947
        mov    word ptr [esi + 0fh], 0ch
        mov    word ptr [esi + 11h], 1eh
loc_2_1947:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_1950:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    edi, edi
loc_2_1960:
        mov    ax, di
        add    ax, 2
        push   eax
        push   esi
        call   sub_2_1CB0
        mov    ch, al
        mov    cl, ah
        lea    eax, [ebp - 4]
        mov    edx, edi
        and    edx, 0ffffh
        add    edx, edx
        sub    eax, edx
        inc    di
        mov    word ptr [eax], cx
        cmp    di, 3
        jb     SHORT loc_2_1960
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [esi + 28h], al
        mov    ax, word ptr [ebp - 8]
        mov    byte ptr [esi + 29h], ah
        lea    ecx, [esi + 28h]
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [esi + 2ah], al
        mov    ax, word ptr [ebp - 6]
        mov    byte ptr [esi + 2bh], ah
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 2ch], al
        mov    ax, word ptr [ebp - 4]
        mov    byte ptr [esi + 2dh], ah
        lea    eax, [esi + 2eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_19E5
        cmp    byte ptr [esi + 2fh], 0
        jne    SHORT loc_2_19E5
        cmp    byte ptr [esi + 30h], 0
        jne    SHORT loc_2_19E5
        cmp    byte ptr [esi + 31h], 0
        jne    SHORT loc_2_19E5
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_2_19E5
        cmp    byte ptr [esi + 33h], 0
        jne    SHORT loc_2_19E5
        push   6
        push   ecx
        push   eax
        db  0E8h, 0B7h, 0F7h, 0FDh, 0FFh
loc_2_19E5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_2_19F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        sub    si, si
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1CB0
        mov    cx, word ptr [ebp + 0ch]
        and    cx, 0fc0fh
        mov    dx, ax
        and    dx, 0fc0fh
        cmp    dx, cx
        je     SHORT loc_2_1A2E
        mov    bl, 1
        and    ax, 3f0h
        or     ax, cx
        push   eax
        push   0
        push   edi
        call   sub_2_1EE0
loc_2_1A2E:
        push   5
        push   edi
        call   sub_2_1CB0
        mov    dx, ax
        and    dx, 1
        mov    cx, word ptr [ebp + 10h]
        cmp    dx, cx
        je     SHORT loc_2_1A58
        mov    bl, 1
        and    ax, 0fffeh
        or     ax, cx
        push   eax
        push   5
        push   edi
        call   sub_2_1EE0
loc_2_1A58:
        or     bl, bl
        je     SHORT loc_2_1ABA
        push   edi
        call   sub_2_1C70
        mov    cx, word ptr [edi]
        add    cx, 0eh
        mov    dx, cx
        mov    al, 40h
        out    dx, al
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi], ax
        push   3e8h
        db  0E8h, 030h, 0F6h, 0FDh, 0FFh
loc_2_1A80:
        mov    ax, word ptr [edi]
        push   eax
        mov    eax, dword ptr [edi + 1b2h]
        push   eax
        call   sub_2_1810
        or     ax, ax
        jne    SHORT loc_2_1AAA
        push   3e8h
        db  0E8h, 011h, 0F6h, 0FDh, 0FFh
        mov    ax, si
        inc    si
        cmp    ax, 64h
        jne    SHORT loc_2_1A80
loc_2_1AAA:
        mov    cx, word ptr [edi]
        mov    al, byte ptr [edi + 8]
        lea    edx, [ecx + 7]
        out    dx, al
        lea    edx, [ecx + 0eh]
        mov    al, 80h
        out    dx, al
loc_2_1ABA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1AD0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 9]
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ecx, -1
        je     SHORT loc_2_1B23
        cmp    ax, 1
        mov    eax, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_1AF5
        and    word ptr [eax], 0efffh
loc_2_1AF5:
        cmp    word ptr [edx + 9], 2
        jne    SHORT loc_2_1B10
        mov    ecx, dword ptr [ebp + 10h]
        or     word ptr [eax], 1000h
        and    word ptr [ecx], 0fffeh
        jmp    SHORT loc_2_1B13
        db  005h, 000h, 000h, 000h, 000h
loc_2_1B10:
        mov    ecx, dword ptr [ebp + 10h]
loc_2_1B13:
        cmp    word ptr [edx + 9], 3
        jne    SHORT loc_2_1B23
        or     word ptr [eax], 1000h
        or     word ptr [ecx], 1
loc_2_1B23:
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0bh]
        mov    edx, eax
        and    edx, 0ffffh
        cmp    edx, -1
        je     NEAR PTR loc_2_1BF2
        cmp    ax, 1
        mov    esi, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_1B6A
        mov    ax, word ptr [esi]
        or     ax, 400h
        mov    word ptr [esi], ax
        and    ax, 0f7ffh
        mov    word ptr [esi], ax
loc_2_1B6A:
        cmp    word ptr [ecx + 0bh], 3
        jne    SHORT loc_2_1B82
        mov    ax, word ptr [esi]
        and    ax, 0fbffh
        mov    word ptr [esi], ax
        or     ax, 800h
        mov    word ptr [esi], ax
loc_2_1B82:
        cmp    word ptr [ecx + 0bh], 2
        jne    SHORT loc_2_1BFA
        mov    ax, word ptr [esi]
        or     ax, 400h
        mov    word ptr [esi], ax
        or     ax, 800h
        mov    word ptr [esi], ax
        mov    dx, word ptr [ecx]
        add    dx, 0dh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 30h
        mov    di, word ptr [ecx]
        mov    al, byte ptr [ebp - 1]
        lea    edx, [edi + 0dh]
        out    dx, al
        mov    dx, di
        in     ax, dx
        mov    dx, word ptr [ecx]
        add    dx, 0dh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0dfh
        mov    dx, word ptr [ecx]
        add    dx, 0dh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        test   al, 40h
        je     SHORT loc_2_1BFA
        and    word ptr [esi], 0fbffh
        and    byte ptr [ebp - 1], 0f7h
        mov    dx, word ptr [ecx]
        add    dx, 0dh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_1BF2:
        mov    esi, dword ptr [ebp + 0ch]
        or     word ptr [esi], 0c00h
loc_2_1BFA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 2], 3
        je     SHORT loc_2_1C68
        cmp    word ptr [ecx + 0dh], 0babah
        je     SHORT loc_2_1C68
        mov    dx, word ptr [ecx]
        add    dx, 300eh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    word ptr [ecx + 9], 1
        jne    SHORT loc_2_1C3E
        and    byte ptr [ebp - 1], 7fh
loc_2_1C3E:
        cmp    word ptr [ecx + 9], 2
        jne    SHORT loc_2_1C4D
        or     byte ptr [ebp - 1], 80h
        and    byte ptr [ebp - 1], 0fdh
loc_2_1C4D:
        cmp    word ptr [ecx + 9], 3
        jne    SHORT loc_2_1C5C
        or     byte ptr [ebp - 1], 80h
        or     byte ptr [ebp - 1], 2
loc_2_1C5C:
        mov    dx, word ptr [ecx]
        add    dx, 300eh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_2_1C68:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_1C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    di, di
        sub    bx, bx
        mov    esi, dword ptr [ebp + 8]
        nop
loc_2_1C80:
        push   ebx
        push   esi
        call   sub_2_1CB0
        add    di, ax
        inc    bx
        cmp    bx, 3fh
        jb     SHORT loc_2_1C80
        mov    ax, 0babah
        sub    ax, di
        push   eax
        push   3fh
        push   esi
        call   sub_2_1EE0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        or     word ptr [ebp - 2], 80h
        and    word ptr [ebp - 2], 0ffbfh
        mov    dx, word ptr [edi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffb2h
        or     word ptr [ebp - 2], 2
        mov    dx, word ptr [edi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   3
        push   6
        push   edi
        call   sub_2_1D30
        push   6
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        push   edi
        call   sub_2_1D30
        push   edi
        call   sub_2_1E20
        mov    si, ax
        push   edi
        call   sub_2_1E90
        mov    ax, si
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bx, 1
        mov    cx, word ptr [ebp + 10h]
        dec    cl
        mov    esi, dword ptr [ebp + 8]
        shl    bx, cl
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffb3h
        mov    di, word ptr [ebp + 0ch]
        mov    edi, edi
loc_2_1D60:
        and    word ptr [ebp - 2], 0fffbh
        test   di, bx
        je     SHORT loc_2_1D70
        or     word ptr [ebp - 2], 4
loc_2_1D70:
        mov    dx, word ptr [esi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   3e8h
        db  0E8h, 02Bh, 0F3h, 0FDh, 0FFh
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DC0
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DF0
        shr    bx, 1
        or     bx, bx
        jne    SHORT loc_2_1D60
        and    word ptr [ebp - 2], 0fffbh
        mov    dx, word ptr [esi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_2_1DC0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx]
        or     ax, 1
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        push   3e8h
        mov    dx, word ptr [ecx]
        add    dx, 0eh
        out    dx, al
        db  0E8h, 0CBh, 0F2h, 0FDh, 0FFh
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1DF0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx]
        and    ax, 0fffeh
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        push   3e8h
        mov    dx, word ptr [ecx]
        add    dx, 0eh
        out    dx, al
        db  0E8h, 09Bh, 0F2h, 0FDh, 0FFh
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    di, di
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffb3h
        mov    bx, 10h
loc_2_1E44:
        add    di, di
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DC0
        mov    cx, word ptr [esi]
        add    cx, 0eh
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffbbh
        test   byte ptr [ebp - 2], 8
        je     SHORT loc_2_1E6F
        or     di, 1
loc_2_1E6F:
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DF0
        dec    bx
        jne    SHORT loc_2_1E44
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffbfh
        and    word ptr [ebp - 2], 0fff9h
        mov    dx, word ptr [esi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DC0
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DF0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1EE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffb2h
        or     word ptr [ebp - 2], 2
        mov    dx, word ptr [esi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   5
        push   13h
        push   esi
        call   sub_2_1D30
        push   4
        mov    di, word ptr [ebp + 0ch]
        push   edi
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_2000
        push   3
        push   7
        push   esi
        call   sub_2_1D30
        push   6
        push   edi
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_1FA0
        or     ax, ax
        je     SHORT loc_2_1F97
        push   esi
        call   sub_2_2000
        push   3
        push   5
        push   esi
        call   sub_2_1D30
        push   6
        push   edi
        push   esi
        call   sub_2_1D30
        push   10h
        mov    ax, word ptr [ebp + 10h]
        push   eax
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_1FA0
        or     ax, ax
        je     SHORT loc_2_1F97
        push   esi
        call   sub_2_2000
        push   5
        push   10h
        push   esi
        call   sub_2_1D30
        push   4
        push   edi
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_1E90
loc_2_1F97:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
sub_2_1FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    di, di
        push   esi
        call   sub_2_2000
        mov    ebx, 8
        lea    ebx, [ebx]
loc_2_1FC0:
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], bx
        jne    SHORT loc_2_1FF0
        push   3e8h
        db  0E8h, 0D5h, 0F0h, 0FDh, 0FFh
        inc    di
        cmp    di, 14h
        jb     SHORT loc_2_1FC0
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_1FF0:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_2_2000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0ffbch
        mov    dx, word ptr [esi]
        add    dx, 0eh
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   3e8h
        db  0E8h, 080h, 0F0h, 0FDh, 0FFh
        or     word ptr [ebp - 2], 2
        mov    cx, word ptr [esi]
        add    cx, 0eh
        mov    dx, cx
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_2050:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 010h, 01Eh, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 049h, 0F0h, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_2084
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2084:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_209E
        mov    esi, 2
loc_2_209E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 005h, 0F0h, 0FDh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_20D2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2170
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_20D2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0CDh, 0EFh, 0FDh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_2101
        push   eax
        call   sub_2_2170
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2101:
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
sub_2_2170:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_2190
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 02Ch, 0EFh, 0FDh, 0FFh
loc_2_2190:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_21A6
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 016h, 0EFh, 0FDh, 0FFh
loc_2_21A6:
        push   0
        push   32h
        push   esi
        db  0E8h, 00Ch, 0EFh, 0FDh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_21C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_21D9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_21D9:
        db  0A1h, 010h, 01Eh, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0CAh, 0EEh, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_21F9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_21F9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_2200:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_2213
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_2200
        jmp    SHORT loc_2_2222
loc_2_2213:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_2222:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_2238
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_2238:
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
sub_2_2290:
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
        call   sub_2_2630
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_22B9
        cmp    ebx, 103h
        jne    SHORT loc_2_22D0
loc_2_22B9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_2350
        or     eax, eax
        je     SHORT loc_2_22D0
        mov    ebx, eax
loc_2_22D0:
        or     ebx, ebx
        je     SHORT loc_2_22DC
        cmp    ebx, 103h
        jne    SHORT loc_2_233B
loc_2_22DC:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_2336
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_22F2
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_22F2:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_2301
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_2307
loc_2_2301:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_2307:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_2316
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 002h, 0EDh, 0FDh, 0FFh
loc_2_2316:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 08Fh, 0EDh, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_2336:
        mov    ebx, 0c001000eh
loc_2_233B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2350:
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
        db  0E8h, 031h, 0EEh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 019h, 0EEh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_23D4
        sub    eax, eax
        nop
loc_2_23B0:
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
        ja     SHORT loc_2_23B0
loc_2_23D4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_243F
loc_2_23E1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_2434
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
        db  0E8h, 0AEh, 0EDh, 0FDh, 0FFh
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
        db  0E8h, 08Dh, 0EDh, 0FDh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_2437
loc_2_2434:
        inc    dword ptr [ebp - 4]
loc_2_2437:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_23E1
loc_2_243F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_252A
        mov    edi, dword ptr [ebp + 18h]
loc_2_2453:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_2810
        or     al, al
        je     SHORT loc_2_248A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_2518
loc_2_248A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_2564
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
        db  0E8h, 008h, 0EDh, 0FDh, 0FFh
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
        db  0E8h, 0D0h, 0ECh, 0FDh, 0FFh
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
loc_2_2518:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_2_2453
loc_2_252A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_25B5
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_2540:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_25BB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_2_25A2
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_2_25A2
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_25A9
loc_2_2564:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 042h, 0ECh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 02Eh, 0ECh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_25A2:
        mov    dword ptr [ebp - 8], 1
loc_2_25A9:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_2_2540
loc_2_25B5:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_2620
loc_2_25BB:
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
        je     SHORT loc_2_2622
        cmp    edi, 103h
        je     SHORT loc_2_2622
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0B7h, 0EBh, 0FDh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0A9h, 0EBh, 0FDh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_2620:
        sub    edi, edi
loc_2_2622:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_2630:
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
        je     SHORT loc_2_266D
        lea    esp, [esp]
loc_2_2660:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_2660
loc_2_266D:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_2_26A1
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
        je     SHORT loc_2_26A3
        cmp    eax, 103h
        je     SHORT loc_2_26A3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_26A1:
        sub    eax, eax
loc_2_26A3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_26B0:
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
        je     SHORT loc_2_26E2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_26D7:
        test   dword ptr [esi], edx
        je     SHORT loc_2_26DC
        inc    eax
loc_2_26DC:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_26D7
loc_2_26E2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_26F0:
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
        jbe    SHORT loc_2_276C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_2730:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_275A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_2786
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 068h, 0EAh, 0FDh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_275A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_2730
loc_2_276C:
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
loc_2_2786:
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
sub_2_27B0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_27E0
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_27E0:
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
        db  0E8h, 0B8h, 0E9h, 0FDh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2810:
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
        je     NEAR PTR loc_2_28A9
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_28A9
        nop
loc_2_2830:
        cmp    edx, ecx
        jb     SHORT loc_2_28A9
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_2853
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2887
loc_2_2853:
        jbe    SHORT loc_2_2860
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_2887
        db  08Bh, 0FFh
loc_2_2860:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_2877
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2887
loc_2_2877:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_2887
        mov    dword ptr [ebp - 4], 0
loc_2_2887:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_28B9
        jle    SHORT loc_2_2898
        or     edx, edx
        je     SHORT loc_2_28A9
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_289B
loc_2_2898:
        lea    ecx, [edx + 1]
loc_2_289B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_2830
loc_2_28A9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_28B9:
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
