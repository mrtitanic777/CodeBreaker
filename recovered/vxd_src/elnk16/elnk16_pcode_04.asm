; elnk16_pcode_04.asm -- ELNK16 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   esi
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        db  0E8h, 000h, 004h, 0FCh, 0FFh
        db  089h, 035h, 07Ch, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 070h, 037h, 000h, 000h
        mov    byte ptr [ebp - 40h], 3
        mov    byte ptr [ebp - 3fh], 0
        db  0C7h, 045h, 0C6h, 030h, 00Bh, 000h, 000h
        db  0C7h, 045h, 0CAh, 060h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0DAh, 070h, 01Ch, 000h, 000h
        db  0C7h, 045h, 0CEh, 0C0h, 032h, 000h, 000h
        db  0C7h, 045h, 0D2h, 0B0h, 034h, 000h, 000h
        db  0C7h, 045h, 0D6h, 010h, 028h, 000h, 000h
        db  0C7h, 045h, 0DEh, 0D0h, 026h, 000h, 000h
        db  0C7h, 045h, 0E2h, 0F0h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0E6h, 0D0h, 005h, 000h, 000h
        db  0C7h, 045h, 0EAh, 020h, 00Dh, 000h, 000h
        db  0A1h, 07Ch, 035h, 000h, 000h
        db  08Bh, 00Dh, 080h, 035h, 000h, 000h
        mov    dword ptr [ebp - 12h], eax
        lea    eax, [ebp - 40h]
        mov    dword ptr [ebp - 0eh], ecx
        db  0C7h, 005h, 078h, 037h, 000h, 000h, 074h, 037h, 000h, 000h
        db  0C7h, 005h, 074h, 037h, 000h, 000h, 074h, 037h, 000h, 000h
        push   36h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 084h, 037h, 000h, 000h
        push   eax
        db  0E8h, 071h, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_00C4
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_00C4:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 051h, 003h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_4_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   edi
        db  0A1h, 070h, 035h, 000h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   2d2h
        push   eax
        db  0E8h, 009h, 004h, 0FCh, 0FFh
        or     eax, eax
        jne    NEAR PTR loc_4_04D1
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0b4h
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0edh], eax
        mov    edx, dword ptr [ebp + 2ch]
        or     edx, edx
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_4_0146
        mov    byte ptr [eax + 55h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 2fh], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 33h], cl
        add    eax, 2fh
        mov    cl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], cl
        jmp    SHORT loc_4_014A
loc_4_0146:
        mov    byte ptr [eax + 55h], 0
loc_4_014A:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebp + 20h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 84h
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 6], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 0ah], eax
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
        mov    edx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp - 4]
        sub    cl, dl
        mov    byte ptr [eax + 161h], cl
        mov    eax, dword ptr [ebp - 4]
        lea    edi, [ebp - 2ch]
        mov    dword ptr [eax + 162h], edx
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 2ch], eax
        mov    byte ptr [ebp - 28h], al
        mov    dword ptr [ebp - 22h], 1
        mov    dword ptr [ebp - 1eh], eax
        mov    dword ptr [ebp - 1ah], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 16h], 1
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
        db  0E8h, 0F6h, 001h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0276
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 07Eh, 003h, 0FCh, 0FFh
        add    esp, 14h
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0A2h, 002h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_0276:
        cmp    byte ptr [ebp + 38h], 0
        je     SHORT loc_4_02C0
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp + 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 043h, 003h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 091h, 001h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 058h, 002h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_02C0:
        mov    al, byte ptr [ebp + 28h]
        push   eax
        mov    al, byte ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_07F0
        or     al, al
        jne    SHORT loc_4_031A
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000004h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0E9h, 002h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 037h, 001h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0FEh, 001h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_031A:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_04E0
        or     al, al
        jne    SHORT loc_4_036C
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 097h, 002h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0E5h, 000h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0ACh, 001h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_036C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0580
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 032h, 007h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 149h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 080h, 04Dh, 001h, 000h, 000h, 000h, 016h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 151h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 155h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 080h, 059h, 001h, 000h, 000h, 090h, 00Eh, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    dword ptr [eax + 15dh], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 162h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0740
        or     al, al
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_4_0410
        push   eax
        db  0E8h, 088h, 006h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 041h, 000h, 0FCh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 008h, 001h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_0410:
        add    eax, 166h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0e9h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2fh
        push   eax
        db  068h, 060h, 01Ch, 000h, 000h
        db  068h, 030h, 01Ah, 000h, 000h
        db  068h, 050h, 01Bh, 000h, 000h
        mov    eax, dword ptr [ebp + 30h]
        push   eax
        db  0E8h, 0C2h, 0FBh, 0FBh, 0FFh
        or     al, al
        jne    SHORT loc_4_0490
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 07Ch, 001h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 008h, 006h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        push   eax
        db  0E8h, 0C1h, 0FFh, 0FBh, 0FFh
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 088h, 000h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_0490:
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 080h, 0E1h, 000h, 000h, 000h, 074h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 078h, 037h, 000h, 000h
        mov    dword ptr [ecx + 0e5h], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 078h, 037h, 000h, 000h
        add    eax, 0e1h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0e1h
        db  0A3h, 078h, 037h, 000h, 000h
        sub    eax, eax
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
loc_4_04D1:
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    38h
        db  08Dh, 049h, 000h
sub_4_04E0:
        push   ebp
        mov    ebp, esp
        push   esi
        db  0A1h, 070h, 035h, 000h, 000h
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 21h]
        lea    eax, [eax + eax*4 + 5]
        lea    eax, [eax + eax*4]
        push   eax
        lea    eax, [esi + 16ah]
        push   eax
        db  0E8h, 000h, 000h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0531
        push   0
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 0B7h, 000h, 0FCh, 0FFh
        add    esp, 14h
        push   esi
        db  0E8h, 046h, 005h, 0FCh, 0FFh
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0531:
        db  0A1h, 070h, 035h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [esi + 25h]
        shl    eax, 4
        push   eax
        lea    eax, [esi + 16eh]
        push   eax
        db  0E8h, 0BAh, 0FFh, 0FBh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0577
        push   1
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 071h, 000h, 0FCh, 0FFh
        add    esp, 14h
        push   esi
        db  0E8h, 000h, 005h, 0FCh, 0FFh
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0577:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_0580:
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
loc_4_05D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 38h], 7
        sub    esi, esi
        mov    word ptr [ebp - 36h], 7
        db  0C7h, 045h, 0CCh, 0DCh, 035h, 000h, 000h
        sub    bl, bl
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 083h, 0FFh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_4_0614
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0614:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 063h, 0FEh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 20h], 6
        jne    SHORT loc_4_064E
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_064E
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, ecx
        lea    esi, [ebp - 30h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 30h], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 2ch], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 2bh], al
loc_4_064E:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 025h, 0FFh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_0674
        mov    eax, dword ptr [ebp - 1ch]
        mov    dx, word ptr [eax + 4]
        jmp    SHORT loc_4_0678
loc_4_0674:
        mov    dx, 300h
loc_4_0678:
        mov    ecx, 200h
        lea    ecx, [ecx]
loc_4_0680:
        cmp    ecx, 270h
        je     SHORT loc_4_06BB
        cmp    ecx, 2f0h
        je     SHORT loc_4_06BB
        cmp    ecx, 370h
        je     SHORT loc_4_06BB
        cmp    ecx, 3b0h
        je     SHORT loc_4_06BB
        cmp    ecx, 3c0h
        je     SHORT loc_4_06BB
        cmp    ecx, 3d0h
        je     SHORT loc_4_06BB
        mov    eax, edx
        and    eax, 0ffffh
        cmp    eax, ecx
        je     SHORT loc_4_06C6
loc_4_06BB:
        add    ecx, 10h
        cmp    ecx, 3f0h
        jb     SHORT loc_4_0680
loc_4_06C6:
        mov    eax, edx
        and    eax, 0ffffh
        cmp    eax, ecx
        je     SHORT loc_4_06E0
        mov    bl, 1
        mov    edi, 0aa00000ah
        mov    ecx, dword ptr [ebp - 24h]
        mov    al, byte ptr [ebp - 2]
        jmp    SHORT loc_4_06FB
loc_4_06E0:
        mov    ecx, 5
        mov    dword ptr [ebp - 14h], 0d0000h
        mov    dword ptr [ebp - 18h], 8000h
        mov    al, 1
        mov    byte ptr [ebp - 1], al
        mov    edi, dword ptr [ebp - 28h]
loc_4_06FB:
        push   edi
        push   ebx
        push   20h
        push   10h
        push   esi
        mov    bl, byte ptr [ebp - 1]
        push   ebx
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0A1h, 084h, 037h, 000h, 000h
        push   eax
        call   sub_4_00E0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 060h, 0FEh, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_0740:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 03Eh, 024h, 0FCh, 0FFh
        push   1
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        lea    edi, [esi + 131h]
        push   ebx
        db  068h, 090h, 00Eh, 000h, 000h
        push   esi
        db  068h, 0E0h, 00Dh, 000h, 000h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        lea    eax, [ebp - 4]
        push   edi
        push   eax
        db  0E8h, 071h, 0FDh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_07C3
        push   esi
        db  0E8h, 0D7h, 022h, 0FCh, 0FFh
        or     al, al
        je     SHORT loc_4_0798
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0798:
        push   0
        push   0
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 029h, 0FEh, 0FBh, 0FFh
        add    esp, 14h
        push   edi
        db  0E8h, 03Ch, 0FDh, 0FBh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_07C3:
        push   ebx
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [esi + 0f1h]
        push   eax
        db  0E8h, 0FFh, 0FDh, 0FBh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        db  080h, 03Dh, 068h, 037h, 000h, 000h, 000h
        jne    SHORT loc_4_0820
        mov    dx, 100h
        sub    al, al
        out    dx, al
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 080h, 02Ah, 0FCh, 0FFh
        mov    dx, 100h
        sub    al, al
        out    dx, al
        db  0C6h, 005h, 068h, 037h, 000h, 000h, 001h
        jmp    SHORT loc_4_0823
loc_4_0820:
        mov    esi, dword ptr [ebp + 8]
loc_4_0823:
        mov    cx, word ptr [esi]
        sub    al, al
        lea    edx, [ecx + 6]
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    dx, word ptr [esi]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    dx, word ptr [esi]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    dx, word ptr [esi]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    dx, word ptr [esi]
        add    dx, 4
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    dx, word ptr [esi]
        add    dx, 5
        in     al, dx
        cmp    byte ptr [ebp - 7], 2ah
        jne    NEAR PTR loc_4_0A6D
        cmp    byte ptr [ebp - 6], 33h
        jne    NEAR PTR loc_4_0A6D
        cmp    byte ptr [ebp - 5], 43h
        jne    NEAR PTR loc_4_0A6D
        cmp    byte ptr [ebp - 4], 4fh
        jne    NEAR PTR loc_4_0A6D
        cmp    byte ptr [ebp - 3], 4dh
        jne    NEAR PTR loc_4_0A6D
        cmp    al, 2ah
        jne    NEAR PTR loc_4_0A6D
        mov    dx, word ptr [esi]
        add    dx, 0dh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, al
        and    cl, 80h
        cmp    cl, 1
        sbb    cl, cl
        neg    cl
        mov    byte ptr [esi + 56h], cl
        mov    dx, word ptr [esi]
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, al
        and    ecx, 0fh
        mov    dword ptr [esi + 162h], ecx
        mov    dx, word ptr [esi]
        add    dx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 20h
        je     SHORT loc_4_0950
        lea    eax, [esi + 0ah]
        mov    dword ptr [eax], 40h
        mov    cl, byte ptr [ebp - 1]
        and    ecx, 0fh
        je     SHORT loc_4_0910
        cmp    ecx, 1
        je     SHORT loc_4_0920
        cmp    ecx, 2
        je     SHORT loc_4_0930
        cmp    ecx, 3
        je     SHORT loc_4_0940
        mov    dword ptr [esi + 6], 0f80000h
        jmp    NEAR PTR loc_4_09D7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0910:
        mov    dword ptr [esi + 6], 0f00000h
        jmp    NEAR PTR loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0920:
        mov    dword ptr [esi + 6], 0f20000h
        jmp    NEAR PTR loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0930:
        mov    dword ptr [esi + 6], 0f40000h
        jmp    NEAR PTR loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0940:
        mov    dword ptr [esi + 6], 0f60000h
        jmp    NEAR PTR loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0950:
        mov    al, byte ptr [ebp - 1]
        and    eax, 3
        je     SHORT loc_4_0970
        cmp    eax, 1
        je     SHORT loc_4_0980
        cmp    eax, 2
        lea    eax, [esi + 0ah]
        je     SHORT loc_4_0990
        mov    dword ptr [eax], 40h
        jmp    SHORT loc_4_0996
        db  08Dh, 049h, 000h
loc_4_0970:
        lea    eax, [esi + 0ah]
        mov    dword ptr [eax], 10h
        jmp    SHORT loc_4_0996
        db  005h, 000h, 000h, 000h, 000h
loc_4_0980:
        lea    eax, [esi + 0ah]
        mov    dword ptr [eax], 20h
        jmp    SHORT loc_4_0996
        db  005h, 000h, 000h, 000h, 000h
loc_4_0990:
        mov    dword ptr [eax], 30h
loc_4_0996:
        mov    cl, byte ptr [ebp - 1]
        and    ecx, 18h
        je     SHORT loc_4_09B1
        cmp    ecx, 8
        je     SHORT loc_4_09C0
        cmp    ecx, 10h
        je     SHORT loc_4_09D0
        mov    dword ptr [esi + 6], 0d8000h
        jmp    SHORT loc_4_09D7
loc_4_09B1:
        mov    dword ptr [esi + 6], 0c0000h
        jmp    SHORT loc_4_09D7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_09C0:
        mov    dword ptr [esi + 6], 0c8000h
        jmp    SHORT loc_4_09D7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_09D0:
        mov    dword ptr [esi + 6], 0d0000h
loc_4_09D7:
        mov    eax, dword ptr [eax]
        cmp    eax, 10h
        je     SHORT loc_4_09F6
        cmp    eax, 20h
        je     SHORT loc_4_0A13
        cmp    eax, 30h
        je     SHORT loc_4_0A30
        cmp    eax, 40h
        je     SHORT loc_4_0A50
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_09F6:
        mov    word ptr [esi + 1ah], 0c000h
        mov    dword ptr [esi + 21h], 2
        mov    dword ptr [esi + 25h], 8
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0A13:
        mov    word ptr [esi + 1ah], 8000h
        mov    dword ptr [esi + 21h], 5
        mov    dword ptr [esi + 25h], 10h
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0A30:
        mov    word ptr [esi + 1ah], 4000h
        mov    dword ptr [esi + 21h], 7
        mov    dword ptr [esi + 25h], 18h
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_4_0A50:
        mov    word ptr [esi + 1ah], 0
        mov    dword ptr [esi + 21h], 0ah
        mov    dword ptr [esi + 25h], 20h
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0A6D:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 032h, 045h, 06Eh, 068h
_PTEXT ends
        end
