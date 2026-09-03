; am1500t_pcode_02.asm -- AM1500T object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 02Ch, 029h, 000h, 000h
        db  0E8h, 0B0h, 00Ah, 0FEh, 0FFh
        push   -1
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 08Dh, 00Bh, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_0040
        mov    eax, 0c000009ah
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_2_0040:
        mov    ecx, dword ptr [ebp - 8]
        db  0A1h, 02Ch, 029h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 40h], 3
        mov    byte ptr [ebp - 3fh], 0
        mov    dword ptr [ebp - 3eh], 0
        db  0C7h, 045h, 0C6h, 000h, 009h, 000h, 000h
        db  0C7h, 045h, 0CAh, 070h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 040h, 021h, 000h, 000h
        db  0C7h, 045h, 0D2h, 030h, 015h, 000h, 000h
        db  0C7h, 045h, 0D6h, 0B0h, 001h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 016h, 000h, 000h
        db  0C7h, 045h, 0DEh, 010h, 019h, 000h, 000h
        db  0C7h, 045h, 0E2h, 010h, 001h, 000h, 000h
        db  0C7h, 045h, 0E6h, 000h, 001h, 000h, 000h
        db  0C7h, 045h, 0EAh, 040h, 001h, 000h, 000h
        db  0A1h, 048h, 027h, 000h, 000h
        db  08Bh, 00Dh, 04Ch, 027h, 000h, 000h
        mov    dword ptr [ebp - 12h], eax
        lea    eax, [ebp - 40h]
        mov    dword ptr [ebp - 0eh], ecx
        push   36h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0A1h, 02Ch, 029h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        db  068h, 030h, 029h, 000h, 000h
        push   eax
        db  0E8h, 008h, 00Ah, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp - 8]
        db  0A1h, 030h, 029h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_00E7
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_00E7:
        push   0
        db  0A1h, 02Ch, 029h, 000h, 000h
        push   eax
        db  0E8h, 0DCh, 009h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_0100:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 38h], 12h
        mov    word ptr [ebp - 36h], 12h
        db  0C7h, 045h, 0CCh, 064h, 027h, 000h, 000h
        mov    word ptr [ebp - 20h], 0ah
        mov    word ptr [ebp - 1eh], 0ah
        db  0C7h, 045h, 0E4h, 09Ch, 027h, 000h, 000h
        mov    word ptr [ebp - 30h], 0ah
        mov    word ptr [ebp - 2eh], 0ah
        db  0C7h, 045h, 0D4h, 0B4h, 027h, 000h, 000h
        mov    word ptr [ebp - 28h], 0bh
        mov    word ptr [ebp - 26h], 0bh
        db  0C7h, 045h, 0DCh, 0CCh, 027h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        jne    SHORT loc_2_0190
        push   20h
        push   5
        push   3
        push   300h
        lea    eax, [ebp - 38h]
        push   0
        push   eax
        push   esi
        db  0A1h, 030h, 029h, 000h, 000h
        push   eax
        call   sub_2_02A0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_2_0190:
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 096h, 00Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_01B0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_01B0:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 073h, 00Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_01D8
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 1], al
        jmp    SHORT loc_2_01DC
loc_2_01D8:
        mov    byte ptr [ebp - 1], 3
loc_2_01DC:
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 047h, 00Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0201
        mov    eax, dword ptr [ebp - 14h]
        mov    bl, byte ptr [eax + 4]
        jmp    SHORT loc_2_0203
loc_2_0201:
        mov    bl, 5
loc_2_0203:
        push   1
        lea    eax, [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 020h, 00Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        mov    edi, 300h
        jne    SHORT loc_2_022B
        mov    eax, dword ptr [ebp - 14h]
        mov    edi, dword ptr [eax + 4]
loc_2_022B:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0FCh, 008h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0265
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_2_0265
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        or     al, byte ptr [ecx + 3]
        or     al, byte ptr [ecx + 4]
        or     al, byte ptr [ecx + 5]
        or     al, byte ptr [ecx + 1]
        or     al, byte ptr [ecx]
        jne    SHORT loc_2_026C
loc_2_0265:
        mov    dword ptr [ebp - 0ch], 0
loc_2_026C:
        push   20h
        push   ebx
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        db  0A1h, 030h, 029h, 000h, 000h
        push   eax
        call   sub_2_02A0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 0ACh, 009h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_2_02A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   esi
        push   edi
        push   -1
        push   0
        lea    eax, [ebp - 8]
        push   248h
        push   eax
        db  0E8h, 0FCh, 008h, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        jne    NEAR PTR loc_2_08C5
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 92h
        rep stosd
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 6dh], eax
        lea    edi, [ebp - 34h]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        xor    eax, eax
        mov    al, byte ptr [ebp + 20h]
        mov    byte ptr [ebp - 30h], 1
        mov    dword ptr [ebp - 34h], eax
        mov    dword ptr [ebp - 26h], 1
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 22h], 600h
        mov    dword ptr [ebp - 1eh], 1
        mov    esi, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 1ah], esi
        mov    dword ptr [ebp - 16h], 20h
        mov    dword ptr [ebp - 2ah], 1
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [eax + 6dh]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 71h
        push   eax
        db  0E8h, 0A5h, 007h, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        jne    NEAR PTR loc_2_0890
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8dh], 10h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0b2h], 20h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 45h], esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0B50
        or     al, al
        je     NEAR PTR loc_2_0842
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0A60
        or     al, al
        jne    SHORT loc_2_03BB
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0C1h, 002h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 04Dh, 007h, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], eax
        push   0
        push   248h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 00Eh, 008h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_03BB:
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        jne    SHORT loc_2_03C8
        mov    eax, dword ptr [ebp - 8]
        add    eax, 3
loc_2_03C8:
        push   6
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9
        push   eax
        db  0E8h, 0C5h, 008h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        sub    cl, cl
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 0
loc_2_03E7:
        mov    esi, ecx
        and    esi, 0ffh
        mov    eax, dword ptr [ebp - 8]
        mov    dl, byte ptr [esi + eax + 9]
        mov    eax, dword ptr [eax + 0c8h]
        mov    byte ptr [esi + eax + 2], dl
        inc    cl
        cmp    cl, 6
        jb     SHORT loc_2_03E7
        sub    cl, cl
        sub    edx, edx
loc_2_040B:
        mov    esi, ecx
        mov    eax, dword ptr [ebp - 8]
        and    esi, 0ffh
        mov    eax, dword ptr [eax + 0c8h]
        mov    byte ptr [esi + eax + 8], dl
        inc    cl
        cmp    cl, 8
        jb     SHORT loc_2_040B
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0c2h]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 10h], cx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0b2h]
        shr    ecx, 1
        or     ecx, ecx
        je     SHORT loc_2_0463
        mov    edx, 2000h
        mov    edi, edi
loc_2_0450:
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 1
        mov    eax, dword ptr [eax + 0c8h]
        add    word ptr [eax + 12h], dx
        or     ecx, ecx
        jne    SHORT loc_2_0450
loc_2_0463:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0c8h]
        mov    eax, dword ptr [eax + 0c2h]
        shr    eax, 10h
        and    ax, 0ffh
        or     word ptr [ecx + 12h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 9dh]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 14h], cx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 8dh]
        shr    ecx, 1
        or     ecx, ecx
        je     SHORT loc_2_04B7
        mov    edx, 2000h
loc_2_04A4:
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 1
        mov    eax, dword ptr [eax + 0c8h]
        add    word ptr [eax + 16h], dx
        or     ecx, ecx
        jne    SHORT loc_2_04A4
loc_2_04B7:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0c8h]
        mov    eax, dword ptr [eax + 9dh]
        shr    eax, 10h
        and    ax, 0ffh
        or     word ptr [ecx + 16h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 4dh
        mov    dword ptr [ecx + 51h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 51h]
        mov    dword ptr [ecx + 4dh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 59h
        mov    dword ptr [ecx + 5dh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ecx + 59h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 79h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 7dh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 81h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 55h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0fh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 11h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0d0h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0d4h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0d8h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0dch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0e0h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 49h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0e8h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0e9h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0eah], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0e4h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0ebh], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ech
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    ecx, 0ch
        mov    dword ptr [eax + 10h], 0
        mov    edi, dword ptr [ebp - 8]
        add    edi, 100h
        sub    eax, eax
        rep stosd
        mov    edi, dword ptr [ebp - 8]
        add    edi, 130h
        mov    ecx, 6
        rep stosd
        mov    eax, dword ptr [ebp - 8]
        add    eax, 148h
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 150h
        mov    ecx, 7
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    edi, dword ptr [ebp - 8]
        add    edi, 15ch
        sub    eax, eax
        rep stosd
        mov    eax, dword ptr [ebp - 8]
        add    eax, 75h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 61h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9
        push   eax
        db  068h, 030h, 021h, 000h, 000h
        db  068h, 030h, 01Fh, 000h, 000h
        db  068h, 010h, 020h, 000h, 000h
        push   10h
        call   sub_2_0C60
        or     al, al
        jne    SHORT loc_2_06D2
        push   2
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0aa000002h
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 0EBh, 005h, 0FEh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0A7h, 0FFh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 033h, 004h, 0FEh, 0FFh
        push   0
        push   248h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0F7h, 004h, 0FEh, 0FFh
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_06D2:
        mov    edi, dword ptr [ebp - 8]
        add    edi, 2bh
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        movsx  eax, byte ptr [ebp + 1ch]
        push   1
        push   1
        push   eax
        push   eax
        db  068h, 000h, 00Fh, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 090h, 00Eh, 000h, 000h
        mov    eax, dword ptr [eax + 71h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2bh
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 091h, 004h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_0770
        push   1
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0aa000005h
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 05Eh, 005h, 0FEh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 75h]
        push   eax
        call   sub_2_0D80
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 00Eh, 0FFh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 09Ah, 003h, 0FEh, 0FFh
        push   0
        push   248h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 05Eh, 004h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0770:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 12h], 0
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 016h, 010h, 010h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [ecx + 1ah], eax
        mov    eax, dword ptr [ebp - 8]
        push   1388h
        mov    dword ptr [eax + 1eh], 0
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 022h, 040h, 013h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [ecx + 26h], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 1eh
        push   eax
        db  0E8h, 0D9h, 003h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2ah], 0
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 45h]
        add    cx, 14h
        mov    dx, cx
        in     ax, dx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 028h, 0F8h, 0FDh, 0FFh
        or     al, al
        jne    SHORT loc_2_0826
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2bh
        push   eax
        db  0E8h, 0BCh, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 75h]
        push   eax
        call   sub_2_0D80
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 053h, 0FEh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 0DFh, 002h, 0FEh, 0FFh
        push   0
        push   248h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0A3h, 003h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_0826:
        db  068h, 030h, 006h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 056h, 004h, 0FEh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_0842:
        push   3
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0aa000002h
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 02Fh, 004h, 0FEh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0EBh, 0FDh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 077h, 002h, 0FEh, 0FFh
        push   0
        push   248h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 03Bh, 003h, 0FEh, 0FFh
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh
loc_2_0890:
        push   4
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0aa000002h
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 0E1h, 003h, 0FEh, 0FFh
        add    esp, 14h
        push   0
        push   248h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 002h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_08C5:
        push   248h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   6
        push   0
        push   4
        mov    eax, dword ptr [ebp - 8]
        push   0aa000002h
        mov    eax, dword ptr [eax + 71h]
        push   eax
        db  0E8h, 0A3h, 003h, 0FEh, 0FFh
        add    esp, 1ch
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0900:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_2_0920
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_2_0920:
        sub    edx, edx
        mov    eax, dword ptr [ebp + 18h]
        cmp    eax, edx
        je     SHORT loc_2_0941
        mov    ecx, dword ptr [ebp + 14h]
        lea    esp, [esp]
loc_2_0930:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_2_093D
        add    ecx, 4
        inc    edx
        cmp    eax, edx
        ja     SHORT loc_2_0930
loc_2_093D:
        cmp    eax, edx
        jne    SHORT loc_2_0950
loc_2_0941:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_2_0950:
        mov    eax, dword ptr [ebp + 10h]
        push   -1
        mov    dword ptr [eax], edx
        inc    dword ptr [esi + 49h]
        lea    eax, [ebp - 4]
        push   0
        push   45h
        push   eax
        db  0E8h, 04Fh, 002h, 0FEh, 0FFh
        or     eax, eax
        jne    SHORT loc_2_09E0
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
        mov    byte ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 41h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 28h], 3
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        push   eax
        push   esi
        db  0E8h, 0D7h, 00Ch, 0FEh, 0FFh
        mov    edi, 103h
        jmp    SHORT loc_2_09FC
loc_2_09E0:
        push   9
        push   1
        push   2
        push   0aa000002h
        mov    edi, 0c000009ah
        mov    eax, dword ptr [esi + 71h]
        push   eax
        db  0E8h, 08Fh, 002h, 0FEh, 0FFh
        add    esp, 14h
loc_2_09FC:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_2_0A46
        cmp    dword ptr [esi + 49h], 2
        jne    SHORT loc_2_0A11
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_2_0A2D
loc_2_0A11:
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_2_0A2D
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_2_0A2D
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_2_0A46
loc_2_0A2D:
        mov    byte ptr [esi + 11h], 1
        push   0
        add    esi, 12h
        push   esi
        db  0E8h, 056h, 001h, 0FEh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0A46:
        dec    dword ptr [esi + 49h]
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        sub    cx, cx
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_2_0A70:
        mov    edx, dword ptr [esi + 45h]
        add    dx, di
        in     al, dx
        and    ax, 0ffh
        add    cx, ax
        inc    edi
        cmp    edi, 0ch
        jb     SHORT loc_2_0A70
        mov    edi, 0eh
loc_2_0A89:
        mov    edx, dword ptr [esi + 45h]
        add    dx, di
        in     al, dx
        and    ax, 0ffh
        add    cx, ax
        inc    edi
        cmp    edi, 10h
        jb     SHORT loc_2_0A89
        mov    edx, dword ptr [esi + 45h]
        add    dx, 0ch
        in     ax, dx
        cmp    ax, cx
        je     SHORT loc_2_0AD1
        push   0c0000001h
        push   0bh
        push   2
        push   3
        push   0aa000004h
        mov    eax, dword ptr [esi + 71h]
        push   eax
        db  0E8h, 0C4h, 001h, 0FEh, 0FFh
        add    esp, 18h
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0AD1:
        sub    ecx, ecx
loc_2_0AD3:
        mov    edx, dword ptr [esi + 45h]
        add    dx, cx
        in     al, dx
        mov    byte ptr [esi + ecx + 3], al
        inc    ecx
        cmp    ecx, 6
        jb     SHORT loc_2_0AD3
        push   esi
        db  0E8h, 066h, 0F5h, 0FDh, 0FFh
        mov    cx, word ptr [esi + 45h]
        mov    ax, 59h
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        in     ax, dx
        mov    di, word ptr [esi + 45h]
        mov    ecx, eax
        and    ecx, 0ffffh
        shl    ecx, 10h
        lea    edx, [edi + 12h]
        mov    ax, 58h
        out    dx, ax
        lea    edx, [edi + 10h]
        in     ax, dx
        and    eax, 0ffffh
        add    ecx, eax
        mov    eax, ecx
        and    eax, 0fffffffh
        cmp    eax, 3003h
        jne    SHORT loc_2_0B41
        mov    byte ptr [esi], 2
        shr    ecx, 1ch
        mov    byte ptr [esi + 1], cl
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0B41:
        mov    byte ptr [esi], 1
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_0B50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0b2h]
        mov    eax, edx
        shl    edx, 7
        sub    edx, eax
        lea    edx, [eax + edx*4]
        lea    edx, [edx + edx*2]
        sub    edx, eax
        mov    eax, dword ptr [esi + 8dh]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        lea    ebx, [edx + eax + 1fh]
        lea    eax, [esi + 85h]
        push   eax
        lea    eax, [esi + 89h]
        push   eax
        push   0
        push   ebx
        mov    eax, dword ptr [esi + 71h]
        push   eax
        db  0E8h, 0A0h, 0FFh, 0FDh, 0FFh
        mov    edi, dword ptr [esi + 89h]
        or     edi, edi
        jne    SHORT loc_2_0BB5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_0BB5:
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    edx, dword ptr [esi + 89h]
        add    edx, 7
        and    edx, 0fffffff8h
        mov    ecx, dword ptr [esi + 85h]
        add    ecx, 7
        and    ecx, 0fffffff8h
        mov    dword ptr [esi + 0c8h], edx
        mov    dword ptr [esi + 0cch], ecx
        add    edx, 18h
        add    ecx, 18h
        mov    dword ptr [esi + 99h], edx
        mov    dword ptr [esi + 9dh], ecx
        mov    eax, dword ptr [esi + 8dh]
        shl    eax, 3
        add    edx, eax
        add    ecx, eax
        mov    dword ptr [esi + 0beh], edx
        mov    dword ptr [esi + 0c2h], ecx
        mov    eax, dword ptr [esi + 0b2h]
        shl    eax, 3
        add    edx, eax
        add    ecx, eax
        mov    dword ptr [esi + 91h], edx
        mov    eax, dword ptr [esi + 8dh]
        mov    edi, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edi + eax*4]
        mov    dword ptr [esi + 95h], ecx
        lea    eax, [eax + eax*8]
        lea    eax, [edi + eax*4]
        add    eax, eax
        add    edx, eax
        add    ecx, eax
        mov    dword ptr [esi + 0b6h], edx
        mov    dword ptr [esi + 0bah], ecx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_0C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 028h, 029h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 039h, 0FFh, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_0C94
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_0C94:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_0CAE
        mov    esi, 2
loc_2_0CAE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0F5h, 0FEh, 0FDh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_0CE2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0D80
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_0CE2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0BDh, 0FEh, 0FDh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_0D11
        push   eax
        call   sub_2_0D80
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_0D11:
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
sub_2_0D80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_0DA0
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 01Ch, 0FEh, 0FDh, 0FFh
loc_2_0DA0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_0DB6
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 006h, 0FEh, 0FDh, 0FFh
loc_2_0DB6:
        push   0
        push   32h
        push   esi
        db  0E8h, 0FCh, 0FDh, 0FDh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_0DE9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0DE9:
        db  0A1h, 028h, 029h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0BAh, 0FDh, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_0E09
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0E09:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_0E10:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_0E23
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_0E10
        jmp    SHORT loc_2_0E32
loc_2_0E23:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_0E32:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_0E48
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_0E48:
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
sub_2_0EA0:
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
        call   sub_2_1240
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_0EC9
        cmp    ebx, 103h
        jne    SHORT loc_2_0EE0
loc_2_0EC9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_0F60
        or     eax, eax
        je     SHORT loc_2_0EE0
        mov    ebx, eax
loc_2_0EE0:
        or     ebx, ebx
        je     SHORT loc_2_0EEC
        cmp    ebx, 103h
        jne    SHORT loc_2_0F4B
loc_2_0EEC:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_0F46
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_0F02
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_0F02:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_0F11
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_0F17
loc_2_0F11:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_0F17:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_0F26
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0F2h, 0FBh, 0FDh, 0FFh
loc_2_0F26:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 07Fh, 0FCh, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_0F46:
        mov    ebx, 0c001000eh
loc_2_0F4B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0F60:
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
        db  0E8h, 021h, 0FDh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 009h, 0FDh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_0FE4
        sub    eax, eax
        nop
loc_2_0FC0:
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
        ja     SHORT loc_2_0FC0
loc_2_0FE4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_104F
loc_2_0FF1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_1044
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
        db  0E8h, 09Eh, 0FCh, 0FDh, 0FFh
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
        db  0E8h, 07Dh, 0FCh, 0FDh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_1047
loc_2_1044:
        inc    dword ptr [ebp - 4]
loc_2_1047:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_0FF1
loc_2_104F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_113A
        mov    edi, dword ptr [ebp + 18h]
loc_2_1063:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_1420
        or     al, al
        je     SHORT loc_2_109A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_1128
loc_2_109A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_1174
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
        db  0E8h, 0F8h, 0FBh, 0FDh, 0FFh
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
        db  0E8h, 0C0h, 0FBh, 0FDh, 0FFh
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
loc_2_1128:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_2_1063
loc_2_113A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_11C5
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_1150:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_11CB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_2_11B2
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_2_11B2
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_11B9
loc_2_1174:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 032h, 0FBh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 01Eh, 0FBh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_11B2:
        mov    dword ptr [ebp - 8], 1
loc_2_11B9:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_2_1150
loc_2_11C5:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_1230
loc_2_11CB:
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
        je     SHORT loc_2_1232
        cmp    edi, 103h
        je     SHORT loc_2_1232
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0A7h, 0FAh, 0FDh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 099h, 0FAh, 0FDh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_1230:
        sub    edi, edi
loc_2_1232:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_1240:
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
        je     SHORT loc_2_127D
        lea    esp, [esp]
loc_2_1270:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_1270
loc_2_127D:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_2_12B1
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
        je     SHORT loc_2_12B3
        cmp    eax, 103h
        je     SHORT loc_2_12B3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_12B1:
        sub    eax, eax
loc_2_12B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_12C0:
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
        je     SHORT loc_2_12F2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_12E7:
        test   dword ptr [esi], edx
        je     SHORT loc_2_12EC
        inc    eax
loc_2_12EC:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_12E7
loc_2_12F2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1300:
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
        jbe    SHORT loc_2_137C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_1340:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_136A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_1396
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 058h, 0F9h, 0FDh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_136A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_1340
loc_2_137C:
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
loc_2_1396:
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
sub_2_13C0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_13F0
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_13F0:
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
        db  0E8h, 0A8h, 0F8h, 0FDh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1420:
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
        je     NEAR PTR loc_2_14B9
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_14B9
        nop
loc_2_1440:
        cmp    edx, ecx
        jb     SHORT loc_2_14B9
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_1463
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_1497
loc_2_1463:
        jbe    SHORT loc_2_1470
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_1497
        db  08Bh, 0FFh
loc_2_1470:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_1487
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_1497
loc_2_1487:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_1497
        mov    dword ptr [ebp - 4], 0
loc_2_1497:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_14C9
        jle    SHORT loc_2_14A8
        or     edx, edx
        je     SHORT loc_2_14B9
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_14AB
loc_2_14A8:
        lea    ecx, [edx + 1]
loc_2_14AB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_1440
loc_2_14B9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_14C9:
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
