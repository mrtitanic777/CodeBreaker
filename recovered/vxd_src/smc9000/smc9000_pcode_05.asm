; smc9000_pcode_05.asm -- SMC9000 object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    word ptr [ebp - 14h], 0
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        lea    ecx, [ebp - 14h]
        push   0
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 12h], 0
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0027:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 4]
        push   ecx
        push   0
        push   14h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_003B:
        test   eax, eax
        je     SHORT loc_5_004C
        mov    eax, 0c0000001h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_004C:
        mov    eax, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [eax + 10h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [ebp - 1eh], esi
        mov    byte ptr [ebp - 4bh], bl
        mov    byte ptr [ebp - 4ch], 3
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        push   36h
        mov    dword ptr [ebp - 1ah], ecx
        lea    eax, [ebp - 4ch]
        mov    edx, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   edx
        lea    eax, [edx + 4]
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h
        push   ecx
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00E1:
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_5_00FC
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   14h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00F2:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00FC:
        mov    eax, 0
        pop    esi
        cmp    dword ptr [ebp - 0ch], 1
        adc    eax, -1
        pop    ebx
        and    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0138:
        mov    edi, dword ptr [esi]
        push   0
        push   14h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0144:
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_014C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0160:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 78h
        mov    word ptr [ebp - 50h], ax
        push   ebx
        mov    word ptr [ebp - 4eh], ax
        push   esi
        push   edi
        mov    eax, 0ah
        db  0C7h, 045h, 0B4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 48h], ax
        mov    word ptr [ebp - 46h], ax
        mov    eax, 0ch
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 40h], ax
        mov    word ptr [ebp - 3eh], ax
        mov    eax, 0bh
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], ax
        mov    word ptr [ebp - 1eh], ax
        mov    eax, 8
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 28h], ax
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 26h], ax
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, 4
        push   esi
        mov    word ptr [ebp - 38h], ax
        lea    ecx, [ebp - 0ch]
        mov    word ptr [ebp - 36h], ax
        push   ecx
        mov    word ptr [ebp - 30h], ax
        mov    word ptr [ebp - 2eh], ax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0201:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_0213
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0213:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   118h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_022A:
        test   eax, eax
        je     SHORT loc_5_0245
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0237:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0245:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 46h
        push   ebx
        rep stosd
        push   eax
        push   5f0h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9fh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0266:
        test   eax, eax
        je     SHORT loc_5_0291
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0273:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   118h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0283:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0291:
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 6eh], ebx
        mov    eax, dword ptr [ebp - 4]
        push   1
        lea    ecx, [ebp - 50h]
        mov    word ptr [eax + 76h], 9
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    dword ptr [eax + 78h], 300h
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebp - 8]
        mov    dword ptr [eax + 7dh], 5eah
        mov    edx, dword ptr [ebp - 0ch]
        lea    eax, [ebp - 10h]
        push   edx
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02CE:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_032C
        mov    eax, dword ptr [ebp - 10h]
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 78h], edx
        mov    ecx, dword ptr [ebp - 4]
        test   dword ptr [ecx + 78h], 1c1fh
        je     SHORT loc_5_032C
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02F8:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   5f0h
        mov    ecx, dword ptr [eax + 9fh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_030E:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_031E:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_032C:
        lea    edi, [ebp - 78h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, 1
        mov    dword ptr [ebp - 6eh], eax
        mov    dword ptr [ebp - 62h], eax
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ebp - 78h]
        mov    ecx, dword ptr [eax + 78h]
        add    eax, 78h
        push   edx
        mov    dword ptr [ebp - 5eh], ecx
        mov    dword ptr [ebp - 56h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 5ah], 10h
        push   eax
        mov    edx, dword ptr [ebx + 4]
        push   esi
        lea    eax, [ecx + 8]
        push   ecx
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0375:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_5_03B7
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0385:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   5f0h
        mov    ecx, dword ptr [eax + 9fh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_039B:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03AB:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_03B7:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_16B0
        push   0
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_5_0424
        push   0aa000004h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03DA:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03E6:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03F2:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   5f0h
        mov    edx, dword ptr [ecx + 9fh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0408:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0418:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0424:
        lea    eax, [ebp - 48h]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        lea    edx, [ebp - 10h]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0439:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_04FD
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        xor    eax, eax
        mov    word ptr [ecx + 76h], dx
        mov    ecx, dword ptr [ebp - 4]
        and    word ptr [ecx + 81h], 0fff9h
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 76h]
        sub    eax, 3
        cmp    eax, 8
        ja     SHORT loc_5_047C
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_5_047C:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa00000ah
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_048F:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_049B:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04A7:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   5f0h
        mov    edx, dword ptr [ecx + 9fh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04BD:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04CD:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_04DB:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 81h], 2
        jmp    SHORT loc_5_04FD
loc_5_04E7:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 81h], 4
        jmp    SHORT loc_5_04FD
loc_5_04F3:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 81h], 6
loc_5_04FD:
        push   2
        lea    eax, [ebp - 40h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0514:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_053E
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_052C:
        test   al, al
        je     NEAR PTR loc_5_06C1
loc_5_0534:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 82h], 1
loc_5_053E:
        push   0
        lea    eax, [ebp - 20h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0555:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_05B5
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_5_05B5
        mov    eax, dword ptr [ebp - 0ch]
        lea    ecx, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 14h]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0579:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_05B5
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 49h], 1
        cmp    dword ptr [ebp - 18h], 6
        jne    NEAR PTR loc_5_0761
        mov    eax, dword ptr [ebp - 14h]
        test   byte ptr [eax], 1
        jne    NEAR PTR loc_5_0761
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx + 91h], ecx
        add    edx, 91h
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
loc_5_05B5:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05BE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0ch
        xor    esi, esi
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 10h]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], esi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 14h
        mov    dword ptr [eax + 18h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1ch
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 40h], 1
        mov    eax, dword ptr [ebp - 4]
        push   1388h
        mov    dword ptr [eax + 34h], esi
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 038h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 3ch], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 34h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0639:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 72h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 30h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 91h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0666:
        test   al, al
        jne    NEAR PTR loc_5_07C0
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0680:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_068F:
        push   esi
        mov    ecx, dword ptr [ebp - 4]
        push   5f0h
        mov    edx, dword ptr [ecx + 9fh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06A4:
        push   esi
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06B3:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_06C1:
        push   1
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06D3:
        test   al, al
        jne    NEAR PTR loc_5_0534
        push   1
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06ED:
        test   al, al
        je     SHORT loc_5_0702
        mov    eax, dword ptr [ebp - 4]
        and    word ptr [eax + 81h], 0feffh
        jmp    NEAR PTR loc_5_053E
loc_5_0702:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa00000ah
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0715:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0721:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_072D:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   5f0h
        mov    edx, dword ptr [ecx + 9fh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0743:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0753:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0761:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa000009h
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0774:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0780:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_078C:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   5f0h
        mov    edx, dword ptr [ecx + 9fh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07A2:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07B2:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_07C0:
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    ax, word ptr [eax + 76h]
        push   0
        and    eax, 0ffffh
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 8]
        add    eax, 54h
        push   ecx
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07F1:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_084D
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa000005h
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_080A:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0819:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 72h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0825:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   5f0h
        mov    edx, dword ptr [ecx + 9fh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_083B:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   118h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_084B:
        jmp    SHORT loc_5_0856
loc_5_084D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_1910
loc_5_0856:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    word ptr [eax + 6ch], 1
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 001h, 002h, 003h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
loc_5_0890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_5_08B6
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 34h]
        mov    byte ptr [esi + 40h], 0
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08B6:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_5_08DB
        mov    edi, 0ea60h
loc_5_08C1:
        push   3e8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08CB:
        dec    edi
        jne    SHORT loc_5_08C1
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 34h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08DB:
        mov    ecx, dword ptr [esi + 78h]
        mov    eax, 2
        lea    edx, [ecx + 0eh]
        out    dx, ax
        lea    edx, [ecx + 0dh]
        xor    al, al
        out    dx, al
        lea    eax, [esi + 54h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08F7:
        mov    ecx, dword ptr [esi + 72h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0900:
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0909:
        push   0
        push   118h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0916:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_0920:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 6ch], 2
        mov    ebp, esp
        jne    SHORT loc_5_0937
        mov    eax, 0c001000dh
        pop    ebp
        ret    8
loc_5_0937:
        push   0
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_1EC0
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0950:
        push   ebp
        mov    edx, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [ebp + 14h]
        cmp    edx, esi
        je     SHORT loc_5_097B
        mov    ecx, ebx
loc_5_0968:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_5_097B
        cmp    eax, 5
        je     SHORT loc_5_097B
        add    ecx, 4
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_5_0968
loc_5_097B:
        cmp    esi, edx
        jne    SHORT loc_5_098D
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_098D:
        mov    eax, dword ptr [ebp + 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        lea    edx, [ebp - 4]
        push   0
        mov    dword ptr [eax], esi
        push   20h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09A6:
        test   eax, eax
        je     SHORT loc_5_09CE
        push   0
        mov    edi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [edi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09BD:
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_09CE:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 8
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    edi, dword ptr [ebp + 20h]
        mov    dword ptr [eax + 18h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1ch
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edi + 72h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A01:
        test   al, al
        jne    SHORT loc_5_0A33
        push   0
        mov    eax, dword ptr [edi + 8]
        push   0aa000002h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A15:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   20h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A25:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_0A33:
        inc    dword ptr [edi + 4ah]
        cmp    word ptr [edi + 6ch], 2
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_5_0A4D
        mov    word ptr [eax + 0ch], 8
        mov    eax, 103h
        jmp    SHORT loc_5_0A55
loc_5_0A4D:
        mov    word ptr [eax + 0ch], 1
        xor    eax, eax
loc_5_0A55:
        lea    ecx, [edi + 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [edi + 10h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 10h]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 10h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebx + esi*4]
        mov    dword ptr [ecx + 14h], ebx
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 14h], 0
        jne    SHORT loc_5_0A8B
        inc    byte ptr [edi + 53h]
        jmp    SHORT loc_5_0A8E
loc_5_0A8B:
        inc    byte ptr [edi + 52h]
loc_5_0A8E:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], edx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
loc_5_0AA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 0ch], 1
        mov    edi, dword ptr [esi + 8]
        je     SHORT loc_5_0ABD
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0ABD:
        cmp    word ptr [edi + 6ch], 2
        jne    SHORT loc_5_0AD5
        mov    eax, 103h
        pop    edi
        mov    word ptr [esi + 0ch], 5
        pop    esi
        pop    ebp
        ret    4
loc_5_0AD5:
        dec    dword ptr [edi + 4ah]
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], edx
        cmp    dword ptr [edi + 4ah], 0
        jne    SHORT loc_5_0AF3
        push   edi
        call   sub_5_1910
loc_5_0AF3:
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_5_0AFE
        dec    byte ptr [edi + 53h]
        jmp    SHORT loc_5_0B01
loc_5_0AFE:
        dec    byte ptr [edi + 52h]
loc_5_0B01:
        push   0
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    ecx, dword ptr [edi + 72h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B10:
        cmp    eax, 0c001000eh
        jne    SHORT loc_5_0B22
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0B22:
        push   0
        push   20h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B2C:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0B40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ax, word ptr [esi + 6ch]
        cmp    ax, 2
        je     NEAR PTR loc_5_0C66
        cmp    ax, 3
        je     NEAR PTR loc_5_0C66
        mov    word ptr [esi + 6ch], 3
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0ch
        je     SHORT loc_5_0B97
loc_5_0B7A:
        push   0
        mov    eax, dword ptr [edi + 18h]
        push   0
        push   40010004h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B8C:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0ch
        jne    SHORT loc_5_0B7A
loc_5_0B97:
        push   esi
        lea    eax, [esi + 54h]
        mov    word ptr [esi + 6ch], 2
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BAC:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0ch
        je     NEAR PTR loc_5_0C55
loc_5_0BC0:
        mov    edi, dword ptr [ebp - 4]
        push   0
        lea    ebx, [edi + 18h]
        push   0
        push   40010005h
        mov    ecx, dword ptr [ebx]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BD7:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BDF:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [edi + 0ch]
        mov    dword ptr [ebp - 4], ecx
        cmp    ax, 5
        jne    SHORT loc_5_0C2F
        push   0
        dec    dword ptr [esi + 4ah]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [esi + 72h]
        mov    ecx, dword ptr [edi + 1ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C12:
        cmp    eax, 0c001000eh
        je     SHORT loc_5_0C47
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C23:
        push   0
        push   20h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C2D:
        jmp    SHORT loc_5_0C47
loc_5_0C2F:
        cmp    ax, 8
        jne    SHORT loc_5_0C47
        mov    word ptr [edi + 0ch], 1
        push   0
        mov    eax, dword ptr [ebx]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C47:
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0ch
        jne    NEAR PTR loc_5_0BC0
loc_5_0C55:
        xor    eax, eax
        pop    edi
        mov    word ptr [esi + 6ch], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0C66:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0C80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    word ptr [ecx + 6ch], 2
        jne    SHORT loc_5_0C9A
        mov    eax, 0c001000dh
        pop    ebp
        ret    8
loc_5_0C9A:
        cmp    word ptr [eax + 0ch], 1
        je     SHORT loc_5_0CAA
        mov    eax, 0c0000001h
        pop    ebp
        ret    8
loc_5_0CAA:
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_1EC0
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0CC0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [edx + 8]
        cmp    word ptr [esi + 6ch], 2
        jne    SHORT loc_5_0CE1
        mov    eax, 0c001000dh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0CE1:
        cmp    word ptr [edx + 0ch], 1
        je     SHORT loc_5_0CF4
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0CF4:
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ecx + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ebp - 8]
        mov    dword ptr [eax + 4], ecx
        push   edx
        push   0
        push   0
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D16:
        cmp    dword ptr [ebp - 8], 5eah
        ja     SHORT loc_5_0D66
        cmp    dword ptr [ebp - 8], 0eh
        jb     SHORT loc_5_0D66
        inc    dword ptr [esi + 0a8h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        lea    ecx, [ebp - 4]
        push   ecx
        push   esi
        call   sub_5_1B00
        test   al, al
        je     SHORT loc_5_0D5A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_5_1C20
loc_5_0D5A:
        mov    eax, 103h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0D66:
        mov    eax, 0c001000fh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0D80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    ecx, dword ptr [ebp + 10h]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edx, dword ptr [esi + 9fh]
        cmp    word ptr [esi + 6ch], 2
        lea    edi, [edx + ecx + 0eh]
        jne    SHORT loc_5_0DB2
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0DB2:
        cmp    word ptr [eax + 0ch], 1
        je     SHORT loc_5_0DC7
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0DC7:
        cmp    byte ptr [esi + 45h], 0
        jne    SHORT loc_5_0DDB
        mov    eax, 0c0010013h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0DDB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 4]
        sub    esi, dword ptr [ebp + 10h]
        sub    esi, 0eh
        jns    SHORT loc_5_0DF7
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0DF7:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 1ch]
        lea    ecx, [ebp - 10h]
        lea    edx, [ebp - 8]
        push   ecx
        mov    dword ptr [eax], ebx
        push   edx
        mov    eax, dword ptr [ebp + 18h]
        push   ebx
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E11:
        mov    eax, dword ptr [ebp - 10h]
        cmp    esi, eax
        jle    SHORT loc_5_0E1A
        mov    esi, eax
loc_5_0E1A:
        mov    eax, dword ptr [ebp + 14h]
        cmp    esi, eax
        jle    SHORT loc_5_0E23
        mov    esi, eax
loc_5_0E23:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     SHORT loc_5_0E4F
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        push   eax
        push   ecx
        push   esi
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E44:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0E4F:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_0EA2
loc_5_0E55:
        test   esi, esi
        je     SHORT loc_5_0EA2
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E6A:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_0E8F
        cmp    dword ptr [ebp - 4], esi
        jbe    SHORT loc_5_0E78
        mov    dword ptr [ebp - 4], esi
loc_5_0E78:
        sub    esi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E8C:
        add    edi, dword ptr [ebp - 4]
loc_5_0E8F:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E9C:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_0E55
loc_5_0EA2:
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        pop    esi
        mov    dword ptr [eax], ebx
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0EC0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 4
        xor    ecx, ecx
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        mov    ebx, dword ptr [edi + 78h]
        jne    SHORT loc_5_0F26
        lea    esi, [ebx + 0eh]
        mov    edx, esi
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    eax, 3
        out    dx, ax
        xor    eax, eax
loc_5_0EF0:
        mov    edx, ecx
        inc    cx
        and    edx, 0ffffh
        cmp    cx, 4
        mov    byte ptr [edi + edx*2 + 98h], al
        mov    byte ptr [edi + edx*2 + 97h], al
        lea    edx, [ebx + edx*2]
        out    dx, ax
        jb     SHORT loc_5_0EF0
        mov    ax, word ptr [ebp - 2]
        mov    edx, esi
        out    dx, ax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_5_0F26:
        lea    ecx, [esi + esi*2]
        mov    eax, dword ptr [ebp + 14h]
        lea    ebx, [eax + ecx*2 - 6]
loc_5_0F30:
        push   ebx
        call   sub_5_1DD0
        push   1
        sub    ebx, 6
        push   eax
        push   edi
        call   sub_5_1E40
        dec    esi
        jne    SHORT loc_5_0F30
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_5_0F50:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [eax + 8]
        xor    si, si
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        test   al, 1
        je     SHORT loc_5_0F6B
        mov    si, 100h
loc_5_0F6B:
        test   al, 2
        je     SHORT loc_5_0F73
        mov    si, 100h
loc_5_0F73:
        test   al, 8
        je     SHORT loc_5_0F7B
        mov    si, 100h
loc_5_0F7B:
        test   al, 4
        je     SHORT loc_5_0F83
        mov    si, 104h
loc_5_0F83:
        test   al, 20h
        je     SHORT loc_5_0F91
        mov    byte ptr [ecx + 48h], 1
        mov    si, 102h
        jmp    SHORT loc_5_0F95
loc_5_0F91:
        mov    byte ptr [ecx + 48h], 0
loc_5_0F95:
        or     si, 200h
        mov    ebx, dword ptr [ecx + 78h]
        lea    ecx, [ebx + 0eh]
        mov    edx, ecx
        in     ax, dx
        mov    di, ax
        xor    eax, eax
        out    dx, ax
        mov    ax, si
        lea    edx, [ebx + 4]
        out    dx, ax
        mov    ax, di
        mov    edx, ecx
        out    dx, ax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0FD0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FE2:
        push   0
        push   20h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FEC:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1000:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [ecx + 78h]
        lea    edi, [esi + 0eh]
        mov    edx, edi
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    eax, 2
        out    dx, ax
        lea    edx, [esi + 0ch]
        in     al, dx
        test   al, 3bh
        je     SHORT loc_5_103A
        lea    edx, [esi + 0dh]
        xor    al, al
        out    dx, al
        mov    ebx, 1
        mov    byte ptr [ecx + 47h], bl
loc_5_103A:
        mov    edx, edi
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        pop    edi
        out    dx, ax
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_5_1050:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + 47h], 1
        mov    byte ptr [esi + 43h], 1
        mov    edi, dword ptr [esi + 78h]
        lea    eax, [edi + 0ch]
        lea    ecx, [edi + 0eh]
        mov    edx, ecx
        mov    dword ptr [ebp - 10h], eax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    edx, ecx
        mov    eax, 2
        mov    dword ptr [ebp - 0ch], ecx
        out    dx, ax
        lea    edx, [edi + 6]
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        lea    edx, [edi + 2]
        in     al, dx
        mov    byte ptr [ebp - 1], al
loc_5_1094:
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 10h]
        xor    ebx, ebx
        in     al, dx
        cmp    byte ptr [esi + 41h], 0
        mov    bl, al
        je     SHORT loc_5_10B7
        cmp    byte ptr [esi + 44h], 0
        jne    SHORT loc_5_10B7
        and    ebx, 3bh
        jmp    SHORT loc_5_10BA
loc_5_10B7:
        and    ebx, 33h
loc_5_10BA:
        test   ebx, ebx
        je     SHORT loc_5_1102
        test   bl, 1
        je     SHORT loc_5_10CB
        push   esi
        call   sub_5_1180
        jmp    SHORT loc_5_1094
loc_5_10CB:
        test   bl, 2
        je     SHORT loc_5_10D8
        push   esi
        call   sub_5_1300
        jmp    SHORT loc_5_1094
loc_5_10D8:
        test   bl, 8
        je     SHORT loc_5_10E5
        push   esi
        call   sub_5_1590
        jmp    SHORT loc_5_1094
loc_5_10E5:
        test   bl, 30h
        je     SHORT loc_5_1094
        test   bl, 20h
        je     SHORT loc_5_10F5
        push   esi
        call   sub_5_1660
loc_5_10F5:
        test   bl, 10h
        je     SHORT loc_5_1094
        push   esi
        call   sub_5_1620
        jmp    SHORT loc_5_1094
loc_5_1102:
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 2
        out    dx, ax
        lea    edx, [edi + 6]
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    byte ptr [esi + 41h], 0
        out    dx, ax
        lea    edx, [edi + 2]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 33h
        je     SHORT loc_5_112E
        cmp    byte ptr [esi + 44h], 0
        jne    SHORT loc_5_112E
        mov    al, 3bh
loc_5_112E:
        mov    byte ptr [esi + 43h], 0
        lea    edx, [edi + 0dh]
        out    dx, al
        cmp    byte ptr [esi + 46h], 0
        mov    ax, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0ch]
        out    dx, ax
        je     SHORT loc_5_1152
        mov    byte ptr [esi + 46h], 0
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1152:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_5_1170
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        call   sub_5_1B00
        test   al, al
        je     SHORT loc_5_1170
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   esi
        call   sub_5_1C20
loc_5_1170:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 8], eax
        add    eax, 8
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 5
        mov    edx, eax
        mov    dword ptr [ebp - 14h], eax
        in     al, dx
        test   al, 80h
        jne    NEAR PTR loc_5_12E9
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        mov    dword ptr [ebp - 18h], eax
loc_5_11B5:
        mov    ebx, dword ptr [esi + 9fh]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, 0e000h
        push   2
        out    dx, ax
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11D1:
        mov    di, word ptr [ebx]
        test   di, 0ac00h
        mov    cx, word ptr [ebx + 2]
        jne    NEAR PTR loc_5_12B6
        sub    cx, 6
        test   di, 1000h
        je     SHORT loc_5_11F0
        inc    cx
loc_5_11F0:
        lea    eax, [ecx - 0eh]
        mov    edx, dword ptr [esi + 9fh]
        mov    word ptr [ebp - 2], ax
        mov    ebx, ecx
        and    ebx, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], edx
        lea    eax, [ebx + 3]
        sar    eax, 2
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1219:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 80h
        mov    dword ptr [ebp - 20h], 0
        mov    byte ptr [esi + 45h], 1
        out    dx, ax
        xor    eax, eax
        mov    dword ptr [ebp - 1ch], ebx
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        lea    ecx, [ebp - 20h]
        add    eax, 0eh
        mov    edx, dword ptr [esi + 72h]
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1254:
        mov    byte ptr [esi + 45h], 0
        mov    byte ptr [esi + 46h], 1
        inc    dword ptr [esi + 0b0h]
        test   di, 4000h
        je     SHORT loc_5_1277
        add    dword ptr [esi + 0e8h], ebx
        inc    dword ptr [esi + 0ech]
        jmp    SHORT loc_5_1298
loc_5_1277:
        test   di, 1
        je     SHORT loc_5_128C
        add    dword ptr [esi + 0e0h], ebx
        inc    dword ptr [esi + 0e4h]
        jmp    SHORT loc_5_1298
loc_5_128C:
        add    dword ptr [esi + 0d8h], ebx
        inc    dword ptr [esi + 0dch]
loc_5_1298:
        mov    edx, dword ptr [ebp - 8]
        in     ax, dx
        test   al, 1
        jne    SHORT loc_5_1298
        mov    edx, dword ptr [ebp - 14h]
        in     al, dx
        test   al, 80h
        je     NEAR PTR loc_5_11B5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_12B6:
        inc    dword ptr [esi + 0b8h]
        test   di, 8000h
        je     SHORT loc_5_12C9
        inc    dword ptr [esi + 0f4h]
loc_5_12C9:
        test   di, 2000h
        je     SHORT loc_5_12D6
        inc    dword ptr [esi + 0f0h]
loc_5_12D6:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 80h
        out    dx, ax
loc_5_12E0:
        mov    edx, dword ptr [ebp - 8]
        in     ax, dx
        test   al, 1
        jne    SHORT loc_5_12E0
loc_5_12E9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1300:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], 0
        mov    ebx, dword ptr [esi + 78h]
        dec    dword ptr [esi + 0a4h]
        lea    edx, [ebx + 4]
        in     al, dx
        lea    edx, [ebx + 2]
        out    dx, al
        lea    edx, [ebx + 6]
        mov    eax, 6000h
        out    dx, ax
        lea    edx, [ebx + 8]
        in     ax, dx
        mov    cx, ax
        in     ax, dx
        mov    edx, ebx
        lea    edi, [eax - 6]
        test   cx, 8e10h
        mov    eax, 0a0h
        out    dx, ax
        lea    edx, [ebx + 0ch]
        mov    al, 2
        out    dx, al
        je     SHORT loc_5_1397
        mov    dword ptr [ebp - 14h], 0c0000001h
        inc    dword ptr [esi + 0b4h]
        test   ch, 80h
        je     SHORT loc_5_1369
        inc    dword ptr [esi + 10ch]
loc_5_1369:
        test   ch, 4
        je     SHORT loc_5_1374
        inc    dword ptr [esi + 114h]
loc_5_1374:
        test   ch, 2
        je     SHORT loc_5_137F
        inc    dword ptr [esi + 104h]
loc_5_137F:
        test   cl, 10h
        je     SHORT loc_5_138A
        inc    dword ptr [esi + 100h]
loc_5_138A:
        test   cl, 20h
        je     SHORT loc_5_1405
        inc    dword ptr [esi + 110h]
        jmp    SHORT loc_5_1405
loc_5_1397:
        inc    dword ptr [esi + 0ach]
        test   cl, 40h
        je     SHORT loc_5_13B7
        mov    eax, edi
        inc    dword ptr [esi + 0d4h]
        and    eax, 0ffffh
        add    dword ptr [esi + 0d0h], eax
        jmp    SHORT loc_5_13E4
loc_5_13B7:
        test   cl, 8
        je     SHORT loc_5_13D1
        mov    eax, edi
        inc    dword ptr [esi + 0cch]
        and    eax, 0ffffh
        add    dword ptr [esi + 0c8h], eax
        jmp    SHORT loc_5_13E4
loc_5_13D1:
        mov    eax, edi
        inc    dword ptr [esi + 0c4h]
        and    eax, 0ffffh
        add    dword ptr [esi + 0c0h], eax
loc_5_13E4:
        test   cl, 80h
        je     SHORT loc_5_13EF
        inc    dword ptr [esi + 108h]
loc_5_13EF:
        test   cl, 4
        je     SHORT loc_5_13FA
        inc    dword ptr [esi + 0fch]
loc_5_13FA:
        test   cl, 2
        je     SHORT loc_5_1405
        inc    dword ptr [esi + 0f8h]
loc_5_1405:
        lea    ecx, [esi + 24h]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_5_142A
        dec    dword ptr [esi + 0a8h]
        mov    dword ptr [ebp - 0ch], eax
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_5_142A
        mov    dword ptr [esi + 28h], ecx
loc_5_142A:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_5_1534
        mov    eax, dword ptr [ebp - 0ch]
        lea    edx, [ebp - 18h]
        push   edx
        mov    ecx, dword ptr [eax + 4]
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 1ch], ecx
        push   eax
        push   0
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_144F:
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1460:
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [esi + 72h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_146D:
        test   al, al
        je     NEAR PTR loc_5_151D
        cmp    byte ptr [esi + 48h], 0
        jne    NEAR PTR loc_5_151D
        mov    ebx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 18h], ebx
        jbe    SHORT loc_5_14F1
        lea    eax, [ebx - 0eh]
        cmp    eax, dword ptr [esi + 7dh]
        jae    SHORT loc_5_14F1
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 9fh]
loc_5_149A:
        mov    eax, dword ptr [esi + 7dh]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, ebx
        cmp    ecx, eax
        jbe    SHORT loc_5_14AB
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
loc_5_14AB:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        lea    edx, [edi + ebx]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14BC:
        add    ebx, dword ptr [ebp - 4]
        lea    ecx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14CC:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_14F1
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14E3:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_14F1
        lea    eax, [ebx - 0eh]
        cmp    eax, dword ptr [esi + 7dh]
        jb     SHORT loc_5_149A
loc_5_14F1:
        mov    eax, dword ptr [ebp - 18h]
        sub    ebx, 0eh
        sub    eax, 0eh
        lea    ecx, [edi + 0eh]
        mov    byte ptr [esi + 45h], 1
        push   eax
        push   ebx
        push   ecx
        lea    eax, [ebp - 1ch]
        push   0eh
        mov    edx, dword ptr [esi + 72h]
        push   edi
        push   edi
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1515:
        mov    byte ptr [esi + 45h], 0
        mov    byte ptr [esi + 46h], 1
loc_5_151D:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 1ch]
        push   edx
        mov    ecx, dword ptr [ecx + 8]
        mov    eax, dword ptr [ecx + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1534:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_5_1558
loc_5_153A:
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        call   sub_5_1B00
        test   al, al
        je     SHORT loc_5_1558
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_5_1C20
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_5_153A
loc_5_1558:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 050h, 078h, 083h, 0C2h, 00Ch, 0B0h, 004h, 0EEh, 05Dh
        db  0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1590:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    dword ptr [ebp - 4], ecx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 78h]
        lea    esi, [edi + 14h]
        mov    byte ptr [edi + 41h], cl
        lea    edx, [ebx + 3]
        in     al, dx
        mov    edx, dword ptr [esi]
        test   edx, edx
        je     SHORT loc_5_15C5
        mov    dword ptr [ebp - 4], edx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi], edx
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [edx], ecx
        jne    SHORT loc_5_15C5
        mov    dword ptr [edi + 18h], esi
loc_5_15C5:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_15DD
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0ch], al
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   edi
        call   sub_5_1C20
        jmp    SHORT loc_5_15EA
loc_5_15DD:
        lea    edx, [ebx + 2]
        out    dx, al
        mov    edx, ebx
        mov    eax, 0a0h
        out    dx, ax
loc_5_15EA:
        cmp    dword ptr [esi], 0
        je     SHORT loc_5_160C
loc_5_15EF:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_5_1B00
        test   al, al
        je     SHORT loc_5_160C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        call   sub_5_1C20
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_15EF
loc_5_160C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1620:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    eax, eax
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 78h]
        lea    esi, [ecx + 0eh]
        mov    edx, esi
        out    dx, ax
        lea    edx, [ecx + 4]
        inc    dword ptr [edi + 0bch]
        in     ax, dx
        and    ax, 0fffeh
        out    dx, ax
        mov    edx, esi
        mov    eax, 2
        out    dx, ax
        lea    edx, [ecx + 0ch]
        mov    al, 10h
        out    dx, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_1660:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    edi, dword ptr [ecx + 78h]
        lea    ebx, [edi + 0eh]
        mov    edx, ebx
        out    dx, ax
        lea    edx, [edi + 2]
        in     ax, dx
        mov    si, ax
        mov    edx, edi
        in     ax, dx
        or     al, 1
        out    dx, ax
        mov    edx, ebx
        mov    eax, 2
        out    dx, ax
        test   si, 10h
        je     SHORT loc_5_169C
        inc    dword ptr [ecx + 100h]
loc_5_169C:
        test   si, 20h
        je     SHORT loc_5_16A9
        inc    dword ptr [ecx + 110h]
loc_5_16A9:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_5_16B0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 20h
        mov    eax, dword ptr [ecx + 78h]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 14h], eax
        push   edi
        lea    edx, [eax + 0eh]
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, 0ah
        mov    dword ptr [ebp - 0ch], edx
        mov    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0ffffh
        out    dx, ax
        mov    edx, ecx
        mov    dword ptr [ebp - 18h], ecx
        in     ax, dx
        inc    ax
        je     SHORT loc_5_16FC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_16FC:
        mov    edx, dword ptr [ebp - 18h]
        xor    eax, eax
        out    dx, ax
        in     ax, dx
        test   ax, ax
        je     SHORT loc_5_1718
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1718:
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        in     ax, dx
        mov    word ptr [ecx + 81h], ax
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, 40h
        out    dx, ax
        mov    edx, dword ptr [ebp - 0ch]
        xor    eax, eax
        out    dx, ax
        mov    edi, dword ptr [ebp - 14h]
        add    edi, 8
        mov    edx, edi
        in     ax, dx
        mov    bx, ax
        mov    si, ax
        and    ebx, 0ffh
        cmp    ebx, 0ffh
        jne    SHORT loc_5_1761
        inc    ebx
loc_5_1761:
        shl    ebx, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, 20h
        out    dx, ax
        mov    edx, dword ptr [ebp - 0ch]
        xor    eax, eax
        and    esi, 0ff00h
        mov    dword ptr [ecx + 83h], ebx
        out    dx, ax
        shr    ebx, 0ch
        mov    edx, edi
        in     ax, dx
        and    eax, 0ff00h
        mov    byte ptr [ecx + 0a3h], bl
        sub    esi, eax
        cmp    bl, 1
        mov    dword ptr [ecx + 87h], esi
        jbe    SHORT loc_5_17B7
        dec    bl
        mov    byte ptr [ecx + 0a3h], bl
        jmp    SHORT loc_5_17C3
loc_5_17B7:
        jae    SHORT loc_5_17C3
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0a3h], 1
loc_5_17C3:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 81h]
        and    eax, 6
        cmp    eax, 3
        ja     SHORT loc_5_1806
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_17DC:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 76h], 9
        jmp    SHORT loc_5_1806
loc_5_17E7:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 76h], 3
        jmp    SHORT loc_5_1806
loc_5_17F2:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 76h], 0ah
        jmp    SHORT loc_5_1806
loc_5_17FD:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 76h], 0bh
loc_5_1806:
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 14h]
        add    ecx, 8bh
        add    edx, 6
        mov    esi, ecx
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], edx
        add    ecx, 0ch
        mov    dword ptr [ebp - 8], ecx
loc_5_182D:
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 2
        out    dx, ax
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 1ch]
        add    eax, 20h
        out    dx, ax
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 1
        out    dx, ax
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 6
        out    dx, ax
        xor    ebx, ebx
loc_5_1858:
        mov    edx, dword ptr [ebp - 8]
        in     ax, dx
        test   ax, 3
        mov    di, ax
        je     SHORT loc_5_187D
        push   2710h
        inc    ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1871:
        cmp    ebx, 3
        jl     SHORT loc_5_1858
        test   di, 3
        jne    SHORT loc_5_1887
loc_5_187D:
        mov    edx, dword ptr [ebp - 18h]
        in     ax, dx
        mov    word ptr [esi], ax
        jmp    SHORT loc_5_1890
loc_5_1887:
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [esi], 0
loc_5_1890:
        add    esi, 2
        inc    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 10h], 3
        jl     SHORT loc_5_182D
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 49h], 0
        jne    SHORT loc_5_18BA
        mov    eax, dword ptr [ebp - 20h]
        lea    edx, [ecx + 91h]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    word ptr [edx + 4], ax
loc_5_18BA:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_5_18D6
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   0aa000009h
        mov    eax, dword ptr [ecx + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_18D3:
        add    esp, 0ch
loc_5_18D6:
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        add    edx, 0dh
        pop    edi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1910:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 2
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 78h]
        lea    edx, [esi + 0eh]
        out    dx, ax
        lea    edx, [esi + 0dh]
        xor    eax, eax
        out    dx, ax
        lea    edx, [esi + 0eh]
        mov    ebx, 10h
        out    dx, ax
        lea    edx, [esi + 4]
        mov    eax, 8000h
        out    dx, ax
loc_5_1941:
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_194B:
        dec    ebx
        jne    SHORT loc_5_1941
        lea    edx, [esi + 4]
        xor    eax, eax
        out    dx, ax
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_195F:
        lea    ecx, [edi + 91h]
        lea    edx, [esi + 0eh]
        mov    eax, 1
        add    ecx, 2
        out    dx, ax
        mov    ax, word ptr [ecx - 2]
        lea    edx, [esi + 4]
        out    dx, ax
        mov    ax, word ptr [ecx]
        lea    edx, [esi + 6]
        out    dx, ax
        mov    ax, word ptr [ecx + 2]
        lea    edx, [esi + 8]
        out    dx, ax
        mov    ax, word ptr [edi + 81h]
        mov    edx, esi
        out    dx, ax
        lea    edx, [esi + 0eh]
        mov    eax, 3
        out    dx, ax
        xor    ecx, ecx
loc_5_19A3:
        lea    edx, [ecx + esi]
        mov    al, byte ptr [edi + ecx + 97h]
        out    dx, al
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_5_19A3
        lea    edx, [esi + 0eh]
        mov    eax, 1
        out    dx, ax
        lea    edx, [esi + 0ch]
        mov    eax, 20h
        out    dx, ax
        lea    edx, [esi + 0eh]
        xor    eax, eax
        out    dx, ax
        mov    edx, esi
        mov    eax, 1481h
        out    dx, ax
        lea    edx, [esi + 4]
        mov    eax, 300h
        out    dx, ax
        lea    edx, [esi + 0ah]
        xor    eax, eax
        out    dx, ax
        lea    edx, [esi + 0eh]
        mov    eax, 2
        out    dx, ax
        lea    edx, [esi + 0dh]
        mov    al, 33h
        out    dx, al
        push   edi
        call   sub_5_1A10
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_1A10:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], edx
        lea    eax, [ebp - 8]
        mov    byte ptr [ecx + 42h], dl
        mov    byte ptr [ecx + 44h], dl
        mov    byte ptr [ecx + 43h], dl
        mov    byte ptr [ecx + 41h], dl
        lea    esi, [ecx + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ecx + 0a8h], edx
        mov    dword ptr [ecx + 0a4h], edx
        cmp    dword ptr [esi], edx
        je     SHORT loc_5_1A65
loc_5_1A44:
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        cmp    dword ptr [edx], 0
        jne    SHORT loc_5_1A52
        mov    dword ptr [ecx + 28h], esi
loc_5_1A52:
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], edx
        mov    dword ptr [eax], edx
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_1A44
loc_5_1A65:
        lea    edx, [ecx + 1ch]
        cmp    dword ptr [edx], 0
        je     SHORT loc_5_1A8E
loc_5_1A6D:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_1A7B
        mov    dword ptr [ecx + 20h], edx
loc_5_1A7B:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    SHORT loc_5_1A6D
loc_5_1A8E:
        lea    edx, [ecx + 14h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_5_1AB7
loc_5_1A96:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_1AA4
        mov    dword ptr [ecx + 18h], edx
loc_5_1AA4:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    SHORT loc_5_1A96
loc_5_1AB7:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_1AEB
        xor    esi, esi
loc_5_1ABF:
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 8], edx
        cmp    dword ptr [ecx], esi
        jne    SHORT loc_5_1AD1
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_5_1AD1:
        push   0c001000ch
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 18h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1AE6:
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_5_1ABF
loc_5_1AEB:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 41h], 0
        jne    NEAR PTR loc_5_1C15
        mov    edi, dword ptr [esi + 14h]
        test   edi, edi
        je     NEAR PTR loc_5_1C15
        mov    ecx, dword ptr [esi + 0a4h]
        xor    eax, eax
        mov    al, byte ptr [esi + 0a3h]
        cmp    eax, ecx
        jbe    NEAR PTR loc_5_1C15
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 0ch], eax
        add    eax, 0eh
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [ebp - 0ch]
        add    edx, 8
        in     ax, dx
        mov    edx, dword ptr [ebp - 8]
        cmp    ah, 8
        jae    SHORT loc_5_1B75
        xor    eax, eax
        pop    edi
        mov    ax, word ptr [ebp - 2]
        pop    esi
        out    dx, ax
        xor    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_1B75:
        mov    eax, 2
        inc    ecx
        mov    byte ptr [esi + 44h], 1
        mov    byte ptr [esi + 41h], 1
        out    dx, ax
        lea    eax, [ebp - 10h]
        mov    dword ptr [esi + 0a4h], ecx
        push   eax
        mov    ecx, dword ptr [edi + 4]
        push   0
        push   0
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B9E:
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        add    ecx, 6
        mov    ebx, 0ah
        shr    ecx, 8
        or     cl, 20h
        mov    ax, cx
        out    dx, ax
        add    edx, 0ch
loc_5_1BBA:
        in     ax, dx
        test   al, 8
        jne    SHORT loc_5_1BC3
        dec    ebx
        jne    SHORT loc_5_1BBA
loc_5_1BC3:
        test   ebx, ebx
        je     SHORT loc_5_1BF3
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 0ch]
        add    edx, 3
        mov    dword ptr [eax], edi
        in     al, dx
        mov    byte ptr [edi + 0ch], al
        mov    eax, dword ptr [esi + 14h]
        mov    edi, dword ptr [eax]
        mov    dword ptr [esi + 14h], edi
        cmp    dword ptr [eax], 0
        jne    SHORT loc_5_1BE9
        lea    eax, [esi + 14h]
        mov    dword ptr [esi + 18h], eax
loc_5_1BE9:
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 41h], 0
        jmp    SHORT loc_5_1BFF
loc_5_1BF3:
        cmp    byte ptr [esi + 43h], 0
        jne    SHORT loc_5_1BFF
        push   esi
        call   sub_5_2810
loc_5_1BFF:
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        pop    edi
        out    dx, ax
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_1C15:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_1C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        cmp    byte ptr [esi + 42h], 0
        jne    NEAR PTR loc_5_1DB4
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + 42h], 1
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [esi + 28h], edi
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 20h], ecx
        lea    ebx, [ecx + 8]
        add    ecx, 0eh
        mov    edx, ecx
        mov    dword ptr [ebp - 2ch], ecx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    edx, ecx
        mov    eax, 2
        mov    ecx, dword ptr [ebp - 20h]
        out    dx, ax
        add    ecx, 6
        mov    edx, ecx
        mov    dword ptr [ebp - 24h], ecx
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp - 20h]
        add    ecx, 2
        mov    edx, ecx
        mov    dword ptr [ebp - 28h], ecx
        in     al, dx
        mov    byte ptr [ebp - 2], al
loc_5_1C8F:
        mov    edx, dword ptr [ebp - 28h]
        mov    al, byte ptr [edi + 0ch]
        out    dx, al
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, 4000h
        out    dx, ax
        lea    eax, [ebp - 1ch]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [edi + 4]
        push   ecx
        push   0
        push   0
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1CB5:
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        mov    ecx, dword ptr [ebp - 1ch]
        add    cx, 6
        mov    ax, cx
        cmp    dword ptr [ebp - 14h], 0
        out    dx, ax
        je     SHORT loc_5_1D39
loc_5_1CCD:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1CDE:
        cmp    dword ptr [ebp - 0ch], 7
        jbe    SHORT loc_5_1D09
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        and    eax, 7ffch
        shr    eax, 2
        push   eax
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1CFA:
        mov    eax, dword ptr [ebp - 0ch]
        and    dword ptr [ebp - 0ch], 3
        and    eax, 7ffch
        add    dword ptr [ebp - 10h], eax
loc_5_1D09:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_1D22
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        and    eax, 7fffh
        push   eax
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D22:
        lea    eax, [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D2F:
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], ecx
        test   ecx, ecx
        jne    SHORT loc_5_1CCD
loc_5_1D39:
        test   byte ptr [ebp - 1ch], 1
        mov    edx, ebx
        je     SHORT loc_5_1D46
        mov    al, 30h
        out    dx, al
        jmp    SHORT loc_5_1D4D
loc_5_1D46:
        mov    eax, 10h
        out    dx, ax
loc_5_1D4D:
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, 0c0h
        out    dx, ax
        mov    eax, dword ptr [esi + 1ch]
        test   eax, eax
        jne    SHORT loc_5_1D68
        mov    byte ptr [esi + 42h], 0
        mov    byte ptr [ebp - 1], 0
        jmp    SHORT loc_5_1D88
loc_5_1D68:
        mov    edi, eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 1ch], eax
        cmp    dword ptr [edi], 0
        jne    SHORT loc_5_1D7A
        lea    eax, [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
loc_5_1D7A:
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [esi + 28h], edi
        mov    dword ptr [eax], edi
loc_5_1D88:
        cmp    byte ptr [ebp - 1], 0
        jne    NEAR PTR loc_5_1C8F
        mov    edx, dword ptr [ebp - 28h]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    ax, word ptr [ebp - 6]
        mov    edx, dword ptr [ebp - 24h]
        out    dx, ax
        mov    ax, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 2ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    8
loc_5_1DB4:
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [eax], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_5_1DD0:
        push   ebp
        mov    eax, 0ffffffffh
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 4], 6
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        push   edi
loc_5_1DE7:
        mov    si, 8
        mov    bl, byte ptr [ecx]
        inc    ecx
loc_5_1DEE:
        mov    edx, eax
        and    edx, 80000000h
        cmp    edx, 1
        mov    dl, bl
        sbb    edi, edi
        add    eax, eax
        and    dx, 1
        inc    edi
        shr    bl, 1
        xor    di, dx
        test   di, di
        je     SHORT loc_5_1E1E
        xor    eax, 4c11db6h
        mov    edx, edi
        and    edx, 0ffffh
        or     eax, edx
loc_5_1E1E:
        dec    si
        jne    SHORT loc_5_1DEE
        dec    word ptr [ebp - 4]
        jne    SHORT loc_5_1DE7
        shr    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E40:
        push   ebp
        xor    dl, dl
        mov    ebp, esp
        sub    esp, 4
        mov    cx, word ptr [ebp + 0ch]
        push   ebx
        mov    ax, cx
        push   esi
        shr    ax, 3
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, eax
        and    esi, 0ffffh
        mov    al, 1
        mov    ebx, dword ptr [edi + 78h]
        and    cl, 7
        shl    al, cl
        lea    edi, [esi + edi + 97h]
        cmp    byte ptr [ebp + 10h], dl
        mov    cl, byte ptr [edi]
        je     SHORT loc_5_1E85
        test   al, cl
        jne    SHORT loc_5_1E7F
        mov    dl, 1
loc_5_1E7F:
        or     cl, al
        mov    byte ptr [edi], cl
        jmp    SHORT loc_5_1E91
loc_5_1E85:
        test   al, cl
        je     SHORT loc_5_1E8B
        mov    dl, 1
loc_5_1E8B:
        not    al
        and    al, cl
        mov    byte ptr [edi], al
loc_5_1E91:
        test   dl, dl
        je     SHORT loc_5_1EB5
        lea    ecx, [ebx + 0eh]
        mov    edx, ecx
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    eax, 3
        out    dx, ax
        lea    edx, [esi + ebx]
        mov    al, byte ptr [edi]
        out    dx, al
        mov    ax, word ptr [ebp - 2]
        mov    edx, ecx
        out    dx, ax
loc_5_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_5_1EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 0ch], edi
        cmp    eax, edi
        je     SHORT loc_5_1EF6
        cmp    eax, 1
        je     NEAR PTR loc_5_2131
        cmp    eax, 2
        je     NEAR PTR loc_5_2274
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_26DD
loc_5_1EF6:
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010101h
        ja     SHORT loc_5_1F17
        je     NEAR PTR loc_5_20AE
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_5_1F38
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_1F17:
        cmp    eax, 1010102h
        je     NEAR PTR loc_5_20C1
        cmp    eax, 1010103h
        je     NEAR PTR loc_5_20D4
        cmp    eax, 1010104h
        je     NEAR PTR loc_5_211D
loc_5_1F38:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        xor    edi, edi
        jmp    NEAR PTR loc_5_26DD
loc_5_1F46:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_5_26E0
loc_5_1F55:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 6ch]
        lea    ecx, [ebp - 8]
        sub    ax, 2
        mov    edi, 4
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_1F79:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 8
        jmp    NEAR PTR loc_5_26E0
loc_5_1F88:
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, 4
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], ecx
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_26E0
loc_5_1F9E:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_5_26E0
loc_5_1FB2:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_5_26E0
loc_5_1FC6:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 83h]
        lea    ecx, [ebp - 8]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_1FE2:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 87h]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_1FFB:
        push   3
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        lea    eax, [ebp - 14h]
        push   ecx
        mov    edi, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2015:
        mov    byte ptr [ebp - 11h], 0
        lea    ecx, [ebp - 14h]
        jmp    NEAR PTR loc_5_26E0
loc_5_2021:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 39h
        jmp    NEAR PTR loc_5_26E0
loc_5_2030:
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, 4
        mov    ecx, dword ptr [eax + 1ch]
        mov    edx, dword ptr [ecx + 8]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_5_26E0
loc_5_2049:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 7dh]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_205F:
        lea    ecx, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_5_26E0
loc_5_2072:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_5_26E0
loc_5_2086:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_26E0
loc_5_209A:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_5_26E0
loc_5_20AE:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        jmp    NEAR PTR loc_5_26E0
loc_5_20C1:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 91h
        jmp    NEAR PTR loc_5_26E0
loc_5_20D4:
        mov    eax, dword ptr [esi + 18h]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [esi + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 10h]
        push   edx
        mov    ecx, dword ptr [eax + 1ch]
        push   ecx
        mov    edx, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 0ch]
        mov    eax, dword ptr [edx + 72h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_20F7:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_210F
        mov    eax, dword ptr [ebp - 8]
        xor    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_5_26DD
loc_5_210F:
        mov    dword ptr [esi + 20h], 0
        xor    edi, edi
        jmp    NEAR PTR loc_5_26DD
loc_5_211D:
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 10h
        jmp    NEAR PTR loc_5_26E0
loc_5_2131:
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_5_215A
        je     NEAR PTR loc_5_221E
        cmp    eax, 1010eh
        je     SHORT loc_5_2166
        cmp    eax, 1010fh
        je     SHORT loc_5_21B8
        cmp    eax, 10112h
        je     NEAR PTR loc_5_21EE
loc_5_215A:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_5_26DD
loc_5_2166:
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_5_217F
        mov    dword ptr [esi + 24h], 4
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_5_26DD
loc_5_217F:
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_218E:
        push   1
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 1ch]
        mov    edx, dword ptr [ecx + 72h]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_21A9:
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_26DD
loc_5_21B8:
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_21C7:
        cmp    dword ptr [ebp - 8], 5eah
        jbe    SHORT loc_5_21D7
        mov    dword ptr [ebp - 0ch], 0c000009ah
loc_5_21D7:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 7dh], eax
        jae    NEAR PTR loc_5_26DD
        mov    dword ptr [ecx + 7dh], eax
        jmp    NEAR PTR loc_5_26DD
loc_5_21EE:
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_5_2207
        mov    dword ptr [esi + 24h], 4
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_5_26DD
loc_5_2207:
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 10h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2219:
        jmp    NEAR PTR loc_5_26DD
loc_5_221E:
        mov    ecx, dword ptr [esi + 1ch]
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_5_223F
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [esi + 20h], ecx
        jmp    NEAR PTR loc_5_26DD
loc_5_223F:
        push   1
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   0
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [edx + 72h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2266:
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [esi + 20h], ecx
        jmp    NEAR PTR loc_5_26DD
loc_5_2274:
        mov    edi, 4
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20101h
        ja     SHORT loc_5_229A
        je     NEAR PTR loc_5_24EF
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_5_2304
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_229A:
        cmp    eax, 20201h
        ja     SHORT loc_5_22B8
        je     NEAR PTR loc_5_2535
        sub    eax, 20102h
        cmp    eax, 3
        ja     SHORT loc_5_2304
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_22B8:
        cmp    eax, 1010101h
        ja     SHORT loc_5_22D6
        je     NEAR PTR loc_5_25F9
        sub    eax, 20202h
        cmp    eax, 0ch
        ja     SHORT loc_5_2304
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_22D6:
        cmp    eax, 1020101h
        ja     SHORT loc_5_2312
        je     NEAR PTR loc_5_266F
        cmp    eax, 1010102h
        je     NEAR PTR loc_5_260C
        cmp    eax, 1010103h
        je     NEAR PTR loc_5_261F
        cmp    eax, 1010104h
        je     NEAR PTR loc_5_265E
loc_5_2304:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        xor    edi, edi
        jmp    NEAR PTR loc_5_26DD
loc_5_2312:
        cmp    eax, 1020201h
        ja     SHORT loc_5_2337
        je     NEAR PTR loc_5_2690
        cmp    eax, 1020102h
        je     NEAR PTR loc_5_267A
        cmp    eax, 1020103h
        je     NEAR PTR loc_5_2685
        jmp    SHORT loc_5_2304
loc_5_2337:
        sub    eax, 1020202h
        cmp    eax, 5
        ja     SHORT loc_5_2304
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_2348:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 0d0h
        jmp    NEAR PTR loc_5_26E0
loc_5_2357:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 6ch]
        lea    ecx, [ebp - 8]
        sub    ax, 2
        mov    edi, 4
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_237B:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 8
        jmp    NEAR PTR loc_5_26E0
loc_5_238A:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx + 52h]
        test   al, al
        je     SHORT loc_5_23C1
        cmp    byte ptr [ecx + 53h], 0
        je     SHORT loc_5_23A9
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 8
        jmp    NEAR PTR loc_5_26E0
loc_5_23A9:
        test   al, al
        je     SHORT loc_5_23C1
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5
        jmp    NEAR PTR loc_5_26E0
loc_5_23C1:
        cmp    byte ptr [ecx + 53h], 0
        je     NEAR PTR loc_5_26DD
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_26E0
loc_5_23DF:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_5_26E0
loc_5_23F3:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_5_26E0
loc_5_2407:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 83h]
        lea    ecx, [ebp - 8]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_2423:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 87h]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_243C:
        push   3
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        lea    eax, [ebp - 14h]
        push   ecx
        mov    edi, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2456:
        mov    byte ptr [ebp - 11h], 0
        lea    ecx, [ebp - 14h]
        jmp    NEAR PTR loc_5_26E0
loc_5_2462:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 39h
        jmp    NEAR PTR loc_5_26E0
loc_5_2471:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 72h]
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_5_26E0
loc_5_248A:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 7dh]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_26E0
loc_5_24A0:
        lea    ecx, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_5_26E0
loc_5_24B3:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_5_26E0
loc_5_24C7:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_26E0
loc_5_24DB:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_5_26E0
loc_5_24EF:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0ach
        jmp    NEAR PTR loc_5_26E0
loc_5_24FD:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0b0h
        jmp    NEAR PTR loc_5_26E0
loc_5_250B:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0b4h
        jmp    NEAR PTR loc_5_26E0
loc_5_2519:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0b8h
        jmp    NEAR PTR loc_5_26E0
loc_5_2527:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0bch
        jmp    NEAR PTR loc_5_26E0
loc_5_2535:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0c0h
        jmp    NEAR PTR loc_5_26E0
loc_5_2543:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0c4h
        jmp    NEAR PTR loc_5_26E0
loc_5_2551:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0c8h
        jmp    NEAR PTR loc_5_26E0
loc_5_255F:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0cch
        jmp    NEAR PTR loc_5_26E0
loc_5_256D:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0d0h
        jmp    NEAR PTR loc_5_26E0
loc_5_257B:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0d4h
        jmp    NEAR PTR loc_5_26E0
loc_5_2589:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0d8h
        jmp    NEAR PTR loc_5_26E0
loc_5_2597:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0dch
        jmp    NEAR PTR loc_5_26E0
loc_5_25A5:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0e0h
        jmp    NEAR PTR loc_5_26E0
loc_5_25B3:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0e4h
        jmp    NEAR PTR loc_5_26E0
loc_5_25C1:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0e8h
        jmp    NEAR PTR loc_5_26E0
loc_5_25CF:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0ech
        jmp    NEAR PTR loc_5_26E0
loc_5_25DD:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0f0h
        jmp    NEAR PTR loc_5_26E0
loc_5_25EB:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0a8h
        jmp    NEAR PTR loc_5_26E0
loc_5_25F9:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        jmp    NEAR PTR loc_5_26E0
loc_5_260C:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 91h
        jmp    NEAR PTR loc_5_26E0
loc_5_261F:
        mov    eax, dword ptr [esi + 18h]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [esi + 1ch]
        push   ecx
        mov    ebx, dword ptr [ebp + 0ch]
        push   edx
        lea    ecx, [ebp - 0ch]
        mov    eax, dword ptr [ebx + 72h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_263B:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_2653
        mov    eax, dword ptr [ebp - 8]
        xor    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_5_26DD
loc_5_2653:
        mov    dword ptr [esi + 20h], 0
        xor    edi, edi
        jmp    SHORT loc_5_26DD
loc_5_265E:
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 10h
        jmp    SHORT loc_5_26E0
loc_5_266F:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0f4h
        jmp    SHORT loc_5_26E0
loc_5_267A:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0f8h
        jmp    SHORT loc_5_26E0
loc_5_2685:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0fch
        jmp    SHORT loc_5_26E0
loc_5_2690:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 108h
        jmp    SHORT loc_5_26E0
loc_5_269B:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 100h
        jmp    SHORT loc_5_26E0
loc_5_26A6:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0bch
        jmp    SHORT loc_5_26E0
loc_5_26B1:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 10ch
        jmp    SHORT loc_5_26E0
loc_5_26BC:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 110h
        jmp    SHORT loc_5_26E0
loc_5_26C7:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 114h
        jmp    SHORT loc_5_26E0
loc_5_26D2:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 104h
        jmp    SHORT loc_5_26E0
loc_5_26DD:
        mov    ecx, dword ptr [ebp - 10h]
loc_5_26E0:
        test   edi, edi
        je     SHORT loc_5_270A
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, edi
        jae    SHORT loc_5_26F7
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [esi + 24h], edi
        mov    edi, eax
loc_5_26F7:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esi + 20h], edi
        test   edi, edi
        je     SHORT loc_5_270A
loc_5_2701:
        mov    dl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [eax], dl
        inc    eax
        dec    edi
        jne    SHORT loc_5_2701
loc_5_270A:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_5_2810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 78h]
        lea    ecx, [ebx + 0eh]
        mov    edx, ecx
        in     ax, dx
        mov    di, ax
        lea    edx, [ebx + 0dh]
        in     al, dx
        mov    bl, al
        xor    al, al
        out    dx, al
        or     bl, 8
        mov    byte ptr [esi + 44h], 0
        mov    al, bl
        out    dx, al
        mov    edx, ecx
        mov    eax, edi
        and    eax, 0ffffh
        pop    edi
        out    dx, ax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_2850:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_5_288E
        cmp    byte ptr [esi + 43h], 0
        jne    SHORT loc_5_2878
        cmp    byte ptr [esi + 47h], 0
        jne    SHORT loc_5_2878
        push   esi
        lea    eax, [esi + 54h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2878:
        push   1388h
        lea    eax, [esi + 34h]
        mov    byte ptr [esi + 47h], 0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_288A:
        mov    byte ptr [esi + 47h], 0
loc_5_288E:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_28A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 78h]
        lea    ecx, [esi + 0eh]
        mov    edx, ecx
        in     ax, dx
        mov    di, ax
        mov    eax, 2
        out    dx, ax
        lea    edx, [esi + 0dh]
        in     al, dx
        mov    bl, al
        xor    al, al
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    edx, ecx
        mov    eax, edi
        and    eax, 0ffffh
        pop    edi
        out    dx, ax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
_PTEXT ends
        end
