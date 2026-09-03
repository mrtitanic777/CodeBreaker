; smc8000w_pcode_02.asm -- SMC8000W object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    eax, 9
        mov    ebp, esp
        sub    esp, 28h
        mov    word ptr [ebp - 10h], ax
        push   ebx
        mov    word ptr [ebp - 0eh], ax
        xor    ebx, ebx
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 24h], 6fc0h
        mov    word ptr [ebp - 22h], 6fc1h
        mov    word ptr [ebp - 20h], 6fc2h
        mov    word ptr [ebp - 1eh], 0efe5h
        mov    word ptr [ebp - 1ch], 61c8h
        mov    word ptr [ebp - 1ah], 61c9h
        mov    word ptr [ebp - 18h], 6fc6h
        mov    word ptr [ebp - 28h], bx
        push   ebx
        mov    word ptr [ebp - 26h], ax
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        lea    eax, [ebp - 28h]
        push   eax
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 16h], 0efd5h
        mov    word ptr [ebp - 14h], 0efd4h
        push   edx
        db  0E8h, 015h, 01Bh, 000h, 000h
        lea    ecx, [ebp - 28h]
        db  089h, 00Dh, 04Ah, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        db  089h, 015h, 004h, 000h, 000h, 000h
        db  088h, 01Dh, 04Eh, 000h, 000h, 000h
        db  089h, 01Dh, 03Eh, 000h, 000h, 000h
        db  089h, 01Dh, 042h, 000h, 000h, 000h
        db  0C6h, 005h, 008h, 000h, 000h, 000h, 003h
        db  088h, 01Dh, 009h, 000h, 000h, 000h
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0C7h, 005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   36h
        db  0A3h, 036h, 000h, 000h, 000h
        db  068h, 008h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 03Ah, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 057h, 01Ah, 000h, 000h
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_2_0150
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 042h, 01Ah, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0150:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0160:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 74h
        mov    word ptr [ebp - 3ch], ax
        push   ebx
        mov    word ptr [ebp - 3ah], ax
        push   esi
        push   edi
        mov    eax, 0dh
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4ch], ax
        mov    word ptr [ebp - 4ah], ax
        mov    eax, 4
        db  0C7h, 045h, 0D8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 34h], ax
        mov    word ptr [ebp - 32h], ax
        mov    eax, 0bh
        mov    word ptr [ebp - 2ch], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 2ah], ax
        mov    byte ptr [ebp - 1], bl
        mov    eax, 8
        mov    byte ptr [ebp - 2], bl
        mov    word ptr [ebp - 44h], ax
        mov    byte ptr [ebp - 24h], bl
        mov    word ptr [ebp - 42h], ax
        mov    esi, 10h
        lea    eax, [ebp - 23h]
        mov    ecx, dword ptr [ebp + 0ch]
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        push   ecx
        lea    edx, [ebp - 0ch]
        mov    word ptr [ebp - 4], 280h
        mov    dword ptr [eax], ebx
        mov    byte ptr [eax + 4], bl
        lea    eax, [ebp - 10h]
        push   eax
        push   edx
        db  0E8h, 0F0h, 01Ah, 000h, 000h
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_2_020F
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_020F:
        push   0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0CCh, 01Ah, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0232
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [eax + 4]
loc_2_0232:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0A9h, 01Ah, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0262
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        sub    eax, 3
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 2], al
loc_2_0262:
        cmp    byte ptr [ebp - 2], 1
        je     SHORT loc_2_028F
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 073h, 01Ah, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_028F
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [ebp - 4], cx
loc_2_028F:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 18h]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 050h, 019h, 000h, 000h
        cmp    dword ptr [ebp - 1ch], 6
        jne    SHORT loc_2_02C6
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_02C6
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 24h], edx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 20h], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 1fh], cl
loc_2_02C6:
        xor    edi, edi
loc_2_02C8:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 8]
        push   eax
        push   edi
        push   31bh
        push   ecx
        db  0E8h, 091h, 019h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_02EF
        mov    byte ptr [ebp - 1], 1
        mov    ebx, 0aa000002h
        jmp    SHORT loc_2_02C8
loc_2_02EF:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0c6h
        rep stosd
        stosw
        stosb
        cmp    byte ptr [ebp - 1], al
        jne    SHORT loc_2_032E
        cmp    byte ptr [ebp - 2], 1
        jne    SHORT loc_2_032E
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_2_1930
        test   ax, ax
        je     SHORT loc_2_032E
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [ebp - 4], 0
        mov    ebx, 0aa00000ah
loc_2_032E:
        lea    edi, [ebp - 74h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    al, byte ptr [ebp - 2]
        mov    dword ptr [ebp - 5eh], 1
        dec    al
        mov    ecx, dword ptr [ebp - 4]
        cmp    al, 1
        mov    dword ptr [ebp - 56h], 20h
        sbb    eax, eax
        lea    edx, [ebp - 74h]
        and    eax, 2
        and    ecx, 0ffffh
        inc    eax
        push   edx
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 6ah], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 5ah], ecx
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        lea    eax, [edx + 52h]
        push   eax
        db  0E8h, 015h, 018h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_2_03B3
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 063h, 019h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   31bh
        push   eax
        db  0E8h, 0CFh, 018h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_03B3:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_2_03FE
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 52h]
        push   ecx
        db  0E8h, 078h, 019h, 000h, 000h
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 024h, 019h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 52h]
        push   edx
        db  0E8h, 0C0h, 017h, 000h, 000h
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   31bh
        push   ecx
        db  0E8h, 084h, 018h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_03FE:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    al, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [edx], al
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 5ch], 0
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0cdh], 4000h
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_2_1A10
        cmp    ax, 0ffffh
        jne    SHORT loc_2_047C
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000004h
        mov    ecx, dword ptr [eax + 52h]
        push   ecx
        db  0E8h, 0FAh, 018h, 000h, 000h
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 0A6h, 018h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 52h]
        push   edx
        db  0E8h, 042h, 017h, 000h, 000h
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   31bh
        push   ecx
        db  0E8h, 006h, 018h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_047C:
        cmp    ax, 2
        jne    SHORT loc_2_04DF
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 059h, 018h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_04AC
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 0ah], dx
loc_2_04AC:
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 02Fh, 018h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_04D5
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 18h], edx
loc_2_04D5:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 146h], 0
loc_2_04DF:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 010h, 018h, 000h, 000h
        lea    eax, [ebp - 24h]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   esi
        push   2
        push   ecx
        call   sub_2_0540
        test   eax, eax
        je     SHORT loc_2_0526
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 52h]
        push   ecx
        db  0E8h, 098h, 016h, 000h, 000h
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   31bh
        push   ecx
        db  0E8h, 05Ch, 017h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0526:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0540:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 163h], edi
        cmp    ebx, 2
        jbe    SHORT loc_2_0583
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000002h
        push   eax
        db  0E8h, 0D3h, 017h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c000009ah
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0583:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 203h], esi
        mov    dword ptr [esi + 207h], eax
        mov    dword ptr [esi + 1ffh], eax
        mov    dword ptr [esi + 15bh], 0
        mov    dword ptr [esi + 1fbh], 0
        db  0C7h, 086h, 053h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        mov    dword ptr [esi + 157h], ecx
        db  089h, 035h, 042h, 000h, 000h, 000h
        movsx  eax, byte ptr [esi + 0ah]
        cmp    byte ptr [esi], 1
        sbb    ecx, ecx
        add    ecx, 2
        push   ecx
        push   0
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 52h]
        push   esi
        lea    edx, [esi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 05Ch, 016h, 000h, 000h
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_061F
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000005h
        push   eax
        db  0E8h, 030h, 017h, 000h, 000h
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_061F:
        cmp    byte ptr [esi + 146h], 0
        je     NEAR PTR loc_2_0785
        mov    dword ptr [esi + 1ch], 0
loc_2_0633:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 14h]
loc_2_0638:
        mov    al, byte ptr [ecx + edx]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [esi + edx + 23h], al
        jb     SHORT loc_2_0638
        mov    word ptr [esi + 3ah], bx
        lea    eax, [esi + 1b4h]
        lea    ecx, [esi + 1bch]
        lea    edx, [esi + 1b8h]
        mov    dword ptr [esi + 69h], eax
        mov    dword ptr [esi + 99h], ecx
        lea    eax, [esi + 1b0h]
        lea    ecx, [esi + 1c0h]
        mov    dword ptr [esi + 6dh], edx
        mov    dword ptr [esi + 9dh], eax
        lea    edx, [esi + 1c4h]
        lea    eax, [esi + 1c8h]
        mov    dword ptr [esi + 0a1h], ecx
        mov    dword ptr [esi + 71h], edx
        lea    ecx, [esi + 1cch]
        lea    edx, [esi + 1d0h]
        mov    dword ptr [esi + 75h], eax
        mov    dword ptr [esi + 79h], ecx
        lea    eax, [esi + 1d4h]
        lea    ecx, [esi + 1d8h]
        mov    dword ptr [esi + 7dh], edx
        mov    dword ptr [esi + 81h], eax
        lea    edx, [esi + 1dch]
        lea    eax, [esi + 1e0h]
        mov    dword ptr [esi + 85h], ecx
        mov    dword ptr [esi + 89h], edx
        lea    ecx, [esi + 1e4h]
        lea    edx, [esi + 1e8h]
        mov    dword ptr [esi + 8dh], eax
        mov    dword ptr [esi + 91h], ecx
        lea    eax, [esi + 1a0h]
        lea    ecx, [esi + 194h]
        mov    dword ptr [esi + 95h], edx
        mov    dword ptr [esi + 0a5h], eax
        lea    edx, [esi + 198h]
        lea    eax, [esi + 19ch]
        mov    dword ptr [esi + 0a9h], ecx
        mov    dword ptr [esi + 0adh], edx
        lea    ecx, [esi + 1a4h]
        lea    edx, [esi + 1a8h]
        mov    dword ptr [esi + 0b1h], eax
        mov    dword ptr [esi + 0b5h], ecx
        lea    eax, [esi + 1ach]
        lea    ebx, [esi + 147h]
        mov    dword ptr [esi + 0b9h], edx
        mov    dword ptr [esi + 0c1h], eax
        push   esi
        mov    dword ptr [ebx], 0
        mov    word ptr [esi + 36h], 5eah
        mov    word ptr [esi + 38h], 100h
        call   sub_2_1BB0
        test   ax, ax
        je     SHORT loc_2_07C3
        push   0
        mov    eax, dword ptr [esi + 52h]
        mov    byte ptr [esi + 1ech], 1
        push   0aa000003h
        push   eax
        db  0E8h, 0C7h, 015h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c0010006h
        add    esp, 0ch
        jmp    SHORT loc_2_0802
loc_2_0785:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 18h]
        mov    ax, word ptr [esi + 16h]
        mov    edx, dword ptr [esi + 52h]
        shl    eax, 0ah
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 1ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 096h, 015h, 000h, 000h
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0633
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000001h
        push   eax
        db  0E8h, 082h, 015h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_2_0802
loc_2_07C3:
        push   ebx
        lea    eax, [esi + 14fh]
        push   eax
        lea    ecx, [esi + 24h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_2_2C90
        test   al, al
        jne    SHORT loc_2_0867
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000002h
        push   eax
        db  0E8h, 048h, 015h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 0ch
loc_2_0802:
        lea    eax, [esi + 0d5h]
        push   eax
        db  0E8h, 04Eh, 014h, 000h, 000h
        db  03Bh, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_2_082D
        mov    eax, dword ptr [esi + 157h]
        pop    edi
        pop    esi
        db  0A3h, 042h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_082D:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 157h], esi
        je     SHORT loc_2_0849
loc_2_083B:
        mov    ecx, dword ptr [ecx + 157h]
        cmp    dword ptr [ecx + 157h], esi
        jne    SHORT loc_2_083B
loc_2_0849:
        mov    eax, dword ptr [ecx + 157h]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 157h]
        mov    dword ptr [ecx + 157h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0867:
        db  0B9h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 30eh]
        push   1388h
        mov    dword ptr [esi + 30ah], ecx
        push   eax
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 312h], ecx
        mov    dword ptr [esi + 316h], esi
        db  0E8h, 0B5h, 013h, 000h, 000h
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_08A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_2_08C4:
        test   edx, edx
        je     SHORT loc_2_08D3
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_08C4
        mov    dword ptr [ecx], edx
loc_2_08D3:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_08E5
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_08E5:
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 1ech], 0
        je     SHORT loc_2_08FE
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_08FE:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   75h
        push   ecx
        db  0E8h, 05Dh, 013h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0935
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000002h
        push   eax
        db  0E8h, 01Bh, 014h, 000h, 000h
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0935:
        inc    dword ptr [esi + 15fh]
        cmp    dword ptr [esi + 15bh], 0
        jne    SHORT loc_2_096F
        push   esi
        call   sub_2_1E90
        test   ax, ax
        je     SHORT loc_2_096F
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000003h
        push   eax
        db  0E8h, 0E1h, 013h, 000h, 000h
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_096F:
        mov    eax, dword ptr [esi + 15bh]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 15bh], ecx
        mov    ecx, dword ptr [esi + 147h]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_2E00
        test   al, al
        jne    SHORT loc_2_09D5
        push   0
        dec    dword ptr [esi + 15fh]
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 15bh], ecx
        mov    edx, dword ptr [esi + 52h]
        push   edx
        db  0E8h, 07Bh, 013h, 000h, 000h
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_09D5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 153h]
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 0eeh
        mov    byte ptr [eax + 20h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 71h], ecx
        mov    dword ptr [esi + 1f7h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 15fh]
        dec    eax
        mov    dword ptr [esi + 15fh], eax
        jne    SHORT loc_2_0A6C
        cmp    byte ptr [esi + 1edh], cl
        je     SHORT loc_2_0A47
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], cl
        je     SHORT loc_2_0A5B
loc_2_0A47:
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_2_0A6C
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0A6C
loc_2_0A5B:
        push   0
        add    esi, 1fbh
        push   esi
        mov    byte ptr [eax], 1
        db  0E8h, 0DEh, 011h, 000h, 000h
loc_2_0A6C:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 18h]
        lea    edx, [esi + 0eh]
        in     al, dx
        xor    ebx, ebx
        mov    dword ptr [edi], 0
        mov    byte ptr [ebp - 4], al
        and    al, 1eh
        shr    al, 1
        mov    bl, al
        test   ebx, ebx
        je     NEAR PTR loc_2_0BCF
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_2_0AB9
        mov    dword ptr [edi], 8
loc_2_0AB9:
        push   ebx
        mov    al, byte ptr [ebp + 10h]
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_2_0BE0
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_0AD4
        or     dword ptr [edi], 2
        jmp    SHORT loc_2_0AE1
loc_2_0AD4:
        cmp    ebx, 1
        je     SHORT loc_2_0ADE
        or     dword ptr [edi], 3
        jmp    SHORT loc_2_0AE1
loc_2_0ADE:
        or     dword ptr [edi], 1
loc_2_0AE1:
        cmp    ebx, 2
        jb     SHORT loc_2_0B06
        test   byte ptr [ebp - 4], 20h
        je     SHORT loc_2_0B06
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    al, 12h
        cmp    al, 12h
        je     SHORT loc_2_0AFE
        mov    eax, ecx
        and    al, 13h
        cmp    al, 13h
        jne    SHORT loc_2_0B06
loc_2_0AFE:
        or     ecx, 200000h
        mov    dword ptr [edi], ecx
loc_2_0B06:
        cmp    ebx, 3
        jb     SHORT loc_2_0B52
        cmp    byte ptr [ebp + 10h], 0
        jne    SHORT loc_2_0B4C
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        and    dword ptr [edi], 0ffd0ffb0h
        call   sub_2_10C0
        test   eax, eax
        je     SHORT loc_2_0B2D
        or     dword ptr [edi], 1000000h
        jmp    SHORT loc_2_0B33
loc_2_0B2D:
        or     dword ptr [edi], 400000h
loc_2_0B33:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   esi
        push   edx
        call   sub_2_0D30
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [edi], ecx
        jmp    SHORT loc_2_0B62
loc_2_0B4C:
        or     dword ptr [edi], 800000h
loc_2_0B52:
        push   edi
        mov    al, byte ptr [ebp + 10h]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_2_0EC0
loc_2_0B62:
        cmp    ebx, 4
        jb     SHORT loc_2_0BBB
        mov    ecx, dword ptr [edi]
        or     ecx, 20h
        test   ecx, 1000000h
        mov    dword ptr [edi], ecx
        je     SHORT loc_2_0BBB
        lea    edx, [esi + 7]
        and    ecx, 0fc3fffffh
        in     al, dx
        shr    al, 4
        mov    dword ptr [edi], ecx
        cmp    al, 2
        jne    SHORT loc_2_0B9A
        or     ecx, 9000000h
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0B9A:
        cmp    al, 4
        jne    SHORT loc_2_0BAF
        or     ecx, 9000800h
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0BAF:
        cmp    al, 3
        jne    SHORT loc_2_0BBB
        or     ecx, 1000000h
        mov    dword ptr [edi], ecx
loc_2_0BBB:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1050
        test   al, al
        je     SHORT loc_2_0BCF
        or     dword ptr [edi], 100000h
loc_2_0BCF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        cmp    dword ptr [ebp + 18h], 3
        mov    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_2_0C11
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_10C0
        test   eax, eax
        je     SHORT loc_2_0C11
        mov    edx, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        or     dword ptr [edx], 10h
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0C11:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_2_0C59
        lea    edi, [esi + 7]
        mov    edx, edi
        in     al, dx
        mov    cl, al
        mov    al, 35h
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, edi
        in     al, dx
        cmp    al, 35h
        jne    SHORT loc_2_0C3F
        mov    al, 3ah
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, edi
        in     al, dx
        cmp    al, 3ah
        jne    SHORT loc_2_0C3F
        mov    edx, dword ptr [ebp + 14h]
        or     dword ptr [edx], 10h
loc_2_0C3F:
        mov    edx, dword ptr [ebp + 14h]
        test   byte ptr [edx], 10h
        je     NEAR PTR loc_2_0D1B
        mov    edx, edi
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    14h
loc_2_0C59:
        mov    edi, 1
loc_2_0C5E:
        lea    edx, [esi + edi]
        in     al, dx
        mov    cl, al
        add    edx, 8
        in     al, dx
        cmp    al, cl
        jne    SHORT loc_2_0C72
        inc    edi
        cmp    edi, 5
        jb     SHORT loc_2_0C5E
loc_2_0C72:
        cmp    edi, 5
        jne    SHORT loc_2_0C81
        lea    edx, [esi + 7]
        in     al, dx
        mov    cl, al
        lea    edx, [esi + 0fh]
        in     al, dx
loc_2_0C81:
        cmp    al, cl
        je     NEAR PTR loc_2_0D1B
        lea    ecx, [esi + 7]
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    al, 35h
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, ecx
        in     al, dx
        cmp    al, 35h
        jne    SHORT loc_2_0CAE
        mov    al, 3ah
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, ecx
        in     al, dx
        cmp    al, 3ah
        jne    SHORT loc_2_0CAE
        mov    bl, 1
loc_2_0CAE:
        mov    edx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [edx]
        test   edi, 10h
        je     SHORT loc_2_0CC1
        mov    edx, ecx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_2_0CC1:
        test   bl, bl
        je     SHORT loc_2_0CD6
        or     edi, 10h
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0CD6:
        lea    ebx, [esi + 1]
        mov    edx, ebx
        in     al, dx
        mov    cl, al
        xor    al, 1
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, ebx
        in     al, dx
        xor    al, cl
        test   al, 1
        jne    SHORT loc_2_0D16
        and    cl, 0feh
        or     edi, 40h
        mov    al, cl
        out    dx, al
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], edi
        mov    edx, ebx
        in     al, dx
        test   al, 1
        je     SHORT loc_2_0D1B
        or     edi, 80000h
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D16:
        mov    edx, ebx
        mov    al, cl
        out    dx, al
loc_2_0D1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0D30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    esi, [edi + 1]
        mov    dword ptr [ebx], 0
        call   sub_2_10C0
        mov    edx, esi
        test   eax, eax
        je     SHORT loc_2_0D61
        mov    al, 4ah
        out    dx, al
loc_2_0D58:
        mov    edx, esi
        in     al, dx
        test   al, 40h
        jne    SHORT loc_2_0D58
        jmp    SHORT loc_2_0D84
loc_2_0D61:
        in     al, dx
        and    al, 0ch
        or     al, 2
        out    dx, al
        lea    edx, [edi + 3]
        in     al, dx
        and    al, 0fh
        or     al, 0a0h
        out    dx, al
        mov    edx, esi
        in     al, dx
        and    al, 0ch
        or     al, 12h
        out    dx, al
        in     al, dx
        test   al, 10h
        je     SHORT loc_2_0D84
loc_2_0D7D:
        mov    edx, esi
        in     al, dx
        test   al, 10h
        jne    SHORT loc_2_0D7D
loc_2_0D84:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_2_0D8D
        or     dword ptr [ebx], 8
loc_2_0D8D:
        lea    edx, [edi + 9]
        in     al, dx
        mov    cl, al
        and    cl, 0c0h
        test   cl, 40h
        je     SHORT loc_2_0DA1
        or     dword ptr [ebx], 80h
loc_2_0DA1:
        test   cl, 80h
        je     SHORT loc_2_0DAC
        or     dword ptr [ebx], 100h
loc_2_0DAC:
        and    al, 18h
        cmp    al, 8
        jne    SHORT loc_2_0DDA
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        or     dword ptr [ebx], 40h
        call   sub_2_10C0
        test   eax, eax
        je     SHORT loc_2_0DCD
        lea    edx, [edi + 4]
        in     al, dx
        test   al, 10h
        je     SHORT loc_2_0DDA
        jmp    SHORT loc_2_0DD4
loc_2_0DCD:
        mov    edx, esi
        in     al, dx
        test   al, 1
        je     SHORT loc_2_0DDA
loc_2_0DD4:
        or     dword ptr [ebx], 80000h
loc_2_0DDA:
        lea    edx, [edi + 8]
        in     al, dx
        mov    cl, al
        and    cl, 7
        jne    SHORT loc_2_0DEA
        or     dword ptr [ebx], 1
        jmp    SHORT loc_2_0E01
loc_2_0DEA:
        cmp    cl, 2
        jne    SHORT loc_2_0DF4
        or     dword ptr [ebx], 3
        jmp    SHORT loc_2_0E01
loc_2_0DF4:
        cmp    cl, 3
        jne    SHORT loc_2_0DFE
        or     dword ptr [ebx], 4
        jmp    SHORT loc_2_0E01
loc_2_0DFE:
        or     dword ptr [ebx], 2
loc_2_0E01:
        test   al, 18h
        je     SHORT loc_2_0E0B
        or     dword ptr [ebx], 200000h
loc_2_0E0B:
        and    al, 0e0h
        cmp    al, 40h
        jne    SHORT loc_2_0E19
        or     dword ptr [ebx], 20000h
        jmp    SHORT loc_2_0E6A
loc_2_0E19:
        cmp    al, 60h
        jne    SHORT loc_2_0E3F
        mov    eax, dword ptr [ebx]
        test   al, 40h
        jne    SHORT loc_2_0E2A
        or     eax, 30000h
        jmp    SHORT loc_2_0E68
loc_2_0E2A:
        test   eax, 80000h
        jne    SHORT loc_2_0E38
        or     eax, 20000h
        jmp    SHORT loc_2_0E68
loc_2_0E38:
        or     eax, 30000h
        jmp    SHORT loc_2_0E68
loc_2_0E3F:
        cmp    al, 80h
        jne    SHORT loc_2_0E4B
        or     dword ptr [ebx], 40000h
        jmp    SHORT loc_2_0E6A
loc_2_0E4B:
        cmp    al, 0a0h
        jne    SHORT loc_2_0E6A
        mov    eax, dword ptr [ebx]
        test   al, 40h
        je     SHORT loc_2_0E63
        test   eax, 80000h
        jne    SHORT loc_2_0E63
        or     eax, 40000h
        jmp    SHORT loc_2_0E68
loc_2_0E63:
        or     eax, 50000h
loc_2_0E68:
        mov    dword ptr [ebx], eax
loc_2_0E6A:
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_10C0
        test   eax, eax
        je     SHORT loc_2_0E92
        lea    edx, [edi + 4]
        in     al, dx
        and    al, 43h
        out    dx, al
        mov    edx, esi
        mov    al, 46h
        out    dx, al
loc_2_0E84:
        mov    edx, esi
        in     al, dx
        test   al, 40h
        jne    SHORT loc_2_0E84
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_2_0E92:
        mov    edx, esi
        in     al, dx
        and    al, 0ch
        or     al, 2
        out    dx, al
        lea    edx, [edi + 3]
        in     al, dx
        and    al, 0fh
        or     al, 80h
        out    dx, al
        mov    edx, esi
        in     al, dx
        and    al, 0ch
        or     al, 10h
        out    dx, al
        in     al, dx
        test   al, 10h
        je     SHORT loc_2_0EB7
loc_2_0EB0:
        mov    edx, esi
        in     al, dx
        test   al, 10h
        jne    SHORT loc_2_0EB0
loc_2_0EB7:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_2_0EC0:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    dword ptr [ebp + 14h], 2
        jae    SHORT loc_2_0F3C
        cmp    byte ptr [ebp + 10h], 0
        mov    esi, dword ptr [ebp + 18h]
        je     SHORT loc_2_0EDE
        or     dword ptr [esi], 30000h
        pop    esi
        pop    ebp
        ret    14h
loc_2_0EDE:
        mov    ecx, dword ptr [esi]
        test   cl, 40h
        je     SHORT loc_2_0F07
        test   ecx, 80000h
        je     SHORT loc_2_0EFA
        or     ecx, 30000h
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0EFA:
        or     ecx, 20000h
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F07:
        test   cl, 10h
        jne    SHORT loc_2_0F19
        or     ecx, 20000h
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F19:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        test   al, 8
        je     SHORT loc_2_0F2F
        or     ecx, 40000h
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F2F:
        or     ecx, 20000h
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F3C:
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 18h]
        add    edx, 0eh
        in     al, dx
        mov    edx, dword ptr [esi]
        test   dl, 8
        je     SHORT loc_2_0F6B
        test   al, 40h
        je     SHORT loc_2_0F5E
        or     edx, 50000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F5E:
        or     edx, 30000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F6B:
        mov    ecx, edx
        and    ecx, 0ffffh
        dec    ecx
        cmp    ecx, 41h
        ja     SHORT loc_2_0FE9
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        and    ecx, 0ffh
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_2_0F8C:
        test   al, 40h
        je     SHORT loc_2_0F9D
        or     edx, 40000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0F9D:
        or     edx, 20000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0FAA:
        test   edx, 80000h
        je     SHORT loc_2_0FD0
        test   al, 40h
        je     SHORT loc_2_0FC3
        or     edx, 50000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0FC3:
        or     edx, 30000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0FD0:
        test   al, 40h
        je     SHORT loc_2_0FE1
        or     edx, 40000h
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    14h
loc_2_0FE1:
        or     edx, 20000h
        mov    dword ptr [esi], edx
loc_2_0FE9:
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 001h, 001h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 002h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_2_1050:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edi, [esi + 10h]
        mov    edx, edi
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0fbh
        mov    bl, byte ptr [ebp - 1]
        add    esi, 1dh
        and    bl, 3fh
        mov    cl, bl
        or     cl, 80h
        mov    al, cl
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, edi
        mov    byte ptr [ebp - 2], al
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, 18h
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    cl, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 1]
        and    cl, 18h
        pop    edi
        pop    esi
        cmp    cl, 18h
        out    dx, al
        pop    ebx
        mov    esp, ebp
        setne  al
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_10C0:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        add    edx, 18h
        in     al, dx
        push   ebx
        mov    bl, al
        mov    al, 55h
        out    dx, al
        in     al, dx
        mov    cl, al
        mov    al, bl
        sub    cl, 55h
        pop    ebx
        pop    ebp
        cmp    cl, 1
        out    dx, al
        sbb    eax, eax
        neg    eax
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_10F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 18h]
        cmp    byte ptr [ebp + 10h], 0
        mov    byte ptr [esi + 27h], 0
        je     NEAR PTR loc_2_1364
        mov    word ptr [esi + 0ch], 1
        mov    ax, word ptr [esi + 28h]
        mov    word ptr [esi + 1ch], ax
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ecx, 6fc2h
        jg     SHORT loc_2_1152
        cmp    ecx, 6fc0h
        jge    NEAR PTR loc_2_12AF
        cmp    ecx, 61c8h
        jl     SHORT loc_2_1184
        cmp    ecx, 61c9h
        jle    SHORT loc_2_1192
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1152:
        cmp    ecx, 0efd5h
        jg     SHORT loc_2_1178
        cmp    ecx, 0efd4h
        jge    SHORT loc_2_1192
        cmp    ecx, 6fc6h
        je     SHORT loc_2_1192
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1178:
        cmp    ecx, 0efe5h
        je     NEAR PTR loc_2_12AF
loc_2_1184:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1192:
        lea    edi, [esi + 0eh]
        mov    cl, byte ptr [esi + 2ah]
        and    cl, 0f8h
        mov    bl, byte ptr [esi + 2dh]
        mov    byte ptr [esi + 1fh], 5
        or     cl, 8
        and    cx, 0ffh
        shl    cx, 8
        mov    word ptr [edi], cx
        mov    cl, bl
        and    cl, 0ch
        jne    SHORT loc_2_11C0
        mov    word ptr [esi + 16h], 3
        jmp    SHORT loc_2_11DE
loc_2_11C0:
        cmp    cl, 4
        jne    SHORT loc_2_11CD
        mov    word ptr [esi + 16h], 4
        jmp    SHORT loc_2_11DE
loc_2_11CD:
        mov    word ptr [esi + 16h], 0ah
        cmp    cl, 8
        je     SHORT loc_2_11DE
        mov    word ptr [esi + 16h], 0eh
loc_2_11DE:
        mov    cl, byte ptr [esi + 2bh]
        test   cl, 8
        je     SHORT loc_2_1200
        test   cl, 80h
        mov    dl, cl
        je     SHORT loc_2_11F8
        and    edx, 7
        add    edx, 7e8h
        jmp    SHORT loc_2_1218
loc_2_11F8:
        and    edx, 7
        add    edx, 68h
        jmp    SHORT loc_2_1218
loc_2_1200:
        test   cl, 80h
        mov    dl, cl
        je     SHORT loc_2_1212
        and    edx, 7
        add    edx, 7e0h
        jmp    SHORT loc_2_1218
loc_2_1212:
        and    edx, 7
        add    edx, 60h
loc_2_1218:
        shl    edx, 0dh
        and    cl, 30h
        shr    cl, 4
        mov    dword ptr [esi + 4], edx
        mov    word ptr [esi + 12h], 40h
        mov    dx, 8
        shl    dx, cl
        mov    word ptr [esi + 14h], dx
        cmp    ax, 6fc6h
        je     SHORT loc_2_123F
        mov    word ptr [esi + 12h], dx
loc_2_123F:
        mov    al, byte ptr [esi + 2ch]
        mov    cl, al
        test   al, 8
        je     SHORT loc_2_1250
        and    ecx, 7
        add    ecx, 68h
        jmp    SHORT loc_2_1256
loc_2_1250:
        and    ecx, 7
        add    ecx, 60h
loc_2_1256:
        shl    ecx, 0dh
        shr    al, 4
        mov    dword ptr [esi + 8], ecx
        test   al, al
        jne    SHORT loc_2_1274
        mov    word ptr [esi + 18h], 8
        and    bl, 7
        mov    byte ptr [esi + 1eh], bl
        jmp    NEAR PTR loc_2_1341
loc_2_1274:
        cmp    al, 1
        jne    SHORT loc_2_1289
        mov    word ptr [esi + 18h], 10h
        and    bl, 7
        mov    byte ptr [esi + 1eh], bl
        jmp    NEAR PTR loc_2_1341
loc_2_1289:
        cmp    al, 2
        jne    SHORT loc_2_129E
        mov    word ptr [esi + 18h], 20h
        and    bl, 7
        mov    byte ptr [esi + 1eh], bl
        jmp    NEAR PTR loc_2_1341
loc_2_129E:
        mov    word ptr [esi + 18h], 0
        and    bl, 7
        mov    byte ptr [esi + 1eh], bl
        jmp    NEAR PTR loc_2_1341
loc_2_12AF:
        lea    edi, [esi + 0eh]
        mov    al, byte ptr [esi + 2ah]
        mov    byte ptr [esi + 1fh], 4
        and    ax, 0feh
        shl    ax, 4
        mov    word ptr [edi], ax
        mov    al, byte ptr [esi + 2dh]
        and    al, 3
        jne    SHORT loc_2_12D3
        mov    word ptr [esi + 16h], 3
        jmp    SHORT loc_2_12EF
loc_2_12D3:
        cmp    al, 1
        jne    SHORT loc_2_12DF
        mov    word ptr [esi + 16h], 4
        jmp    SHORT loc_2_12EF
loc_2_12DF:
        mov    word ptr [esi + 16h], 0ah
        cmp    al, 2
        je     SHORT loc_2_12EF
        mov    word ptr [esi + 16h], 0fh
loc_2_12EF:
        mov    al, byte ptr [esi + 2bh]
        mov    edx, 10h
        mov    word ptr [esi + 12h], dx
        and    eax, 0fch
        shl    eax, 0ch
        mov    word ptr [esi + 14h], dx
        mov    dword ptr [esi + 4], eax
        mov    al, byte ptr [esi + 2ch]
        mov    cl, al
        and    ecx, 0fch
        shl    ecx, 0ch
        and    al, 3
        mov    dword ptr [esi + 8], ecx
        jne    SHORT loc_2_1325
        mov    word ptr [esi + 18h], dx
        jmp    SHORT loc_2_1341
loc_2_1325:
        cmp    al, 1
        jne    SHORT loc_2_1331
        mov    word ptr [esi + 18h], 20h
        jmp    SHORT loc_2_1341
loc_2_1331:
        mov    word ptr [esi + 18h], 0
        cmp    al, 2
        je     SHORT loc_2_1341
        mov    word ptr [esi + 18h], 40h
loc_2_1341:
        push   esi
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    cl, byte ptr [ebp + 10h]
        push   ecx
        xor    edx, edx
        mov    dx, word ptr [edi]
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   eax
        call   sub_2_0A80
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1364:
        xor    bx, bx
        xor    ecx, ecx
        mov    word ptr [esi + 0ch], cx
        xor    edi, edi
        mov    di, word ptr [esi + 0eh]
loc_2_1373:
        mov    eax, ebx
        inc    bx
        and    eax, 0ffffh
        lea    edx, [eax + edi + 8]
        in     al, dx
        add    cl, al
        cmp    bx, 8
        jb     SHORT loc_2_1373
        cmp    cl, 0ffh
        je     SHORT loc_2_139C
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_139C:
        push   esi
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    cl, byte ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edi
        push   edx
        call   sub_2_0A80
        mov    eax, dword ptr [esi]
        and    eax, 70000h
        cmp    eax, 30000h
        ja     SHORT loc_2_13C8
        je     SHORT loc_2_13E0
        cmp    eax, 20000h
        je     SHORT loc_2_13D8
        jmp    SHORT loc_2_13F6
loc_2_13C8:
        cmp    eax, 40000h
        je     SHORT loc_2_13E8
        cmp    eax, 50000h
        je     SHORT loc_2_13F0
        jmp    SHORT loc_2_13F6
loc_2_13D8:
        mov    word ptr [esi + 12h], 8
        jmp    SHORT loc_2_13F6
loc_2_13E0:
        mov    word ptr [esi + 12h], 10h
        jmp    SHORT loc_2_13F6
loc_2_13E8:
        mov    word ptr [esi + 12h], 20h
        jmp    SHORT loc_2_13F6
loc_2_13F0:
        mov    word ptr [esi + 12h], 40h
loc_2_13F6:
        mov    ax, word ptr [esi + 12h]
        test   byte ptr [esi], 10h
        mov    word ptr [esi + 14h], ax
        jne    SHORT loc_2_1450
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0eh]
        mov    byte ptr [esi + 1fh], 0
        lea    edx, [ecx + 8]
        in     al, dx
        test   al, al
        je     SHORT loc_2_1423
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1423:
        lea    edx, [ecx + 9]
        in     al, dx
        test   al, al
        je     SHORT loc_2_1439
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1439:
        lea    edx, [ecx + 0ah]
        pop    edi
        in     al, dx
        sub    al, 0c0h
        pop    esi
        pop    ebx
        cmp    al, 1
        sbb    eax, eax
        mov    esp, ebp
        and    eax, 2
        pop    ebp
        dec    eax
        ret    14h
loc_2_1450:
        mov    edi, dword ptr [esi]
        and    edi, 3c00000h
        jne    SHORT loc_2_1460
        mov    byte ptr [esi + 1fh], 1
        jmp    SHORT loc_2_1470
loc_2_1460:
        mov    byte ptr [esi + 1fh], 3
        cmp    edi, 1000000h
        je     SHORT loc_2_1470
        mov    byte ptr [esi + 1fh], 2
loc_2_1470:
        xor    cl, cl
        cmp    edi, 1000000h
        jne    SHORT loc_2_14E1
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0eh]
        lea    edx, [ebx + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0c3h
        mov    al, byte ptr [ebp - 4]
        or     al, 80h
        out    dx, al
        lea    edx, [ebx + 0dh]
        in     al, dx
        mov    cl, al
        lea    edx, [ebx + 4]
        and    cl, 0ch
        and    al, 40h
        shr    cl, 2
        shr    al, 4
        or     cl, al
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        test   byte ptr [esi], 40h
        lea    edx, [esi + 16h]
        je     SHORT loc_2_14CA
        xor    eax, eax
        mov    al, cl
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffff00ffh
        mov    word ptr [edx], ax
        jmp    NEAR PTR loc_2_1576
loc_2_14CA:
        xor    eax, eax
        mov    al, cl
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffff00ffh
        mov    word ptr [edx], ax
        jmp    NEAR PTR loc_2_1576
loc_2_14E1:
        test   edi, edi
        je     SHORT loc_2_14F2
        xor    edx, edx
        mov    dx, word ptr [esi + 0eh]
        inc    edx
        in     al, dx
        mov    cl, al
        and    cl, 4
loc_2_14F2:
        xor    edx, edx
        mov    dx, word ptr [esi + 0eh]
        add    edx, 4
        in     al, dx
        and    al, 60h
        shr    al, 5
        test   al, al
        jne    SHORT loc_2_151A
        lea    edx, [esi + 16h]
        test   cl, cl
        jne    SHORT loc_2_1513
        mov    word ptr [edx], 2
        jmp    SHORT loc_2_1576
loc_2_1513:
        mov    word ptr [edx], 0ah
        jmp    SHORT loc_2_1576
loc_2_151A:
        cmp    al, 1
        jne    SHORT loc_2_152C
        lea    edx, [esi + 16h]
        test   cl, cl
        je     SHORT loc_2_1571
        mov    word ptr [edx], 0bh
        jmp    SHORT loc_2_1576
loc_2_152C:
        cmp    al, 2
        jne    SHORT loc_2_1555
        test   cl, cl
        jne    SHORT loc_2_154B
        test   byte ptr [esi + 2], 20h
        lea    edx, [esi + 16h]
        je     SHORT loc_2_1544
        mov    word ptr [edx], 5
        jmp    SHORT loc_2_1576
loc_2_1544:
        mov    word ptr [edx], 4
        jmp    SHORT loc_2_1576
loc_2_154B:
        lea    edx, [esi + 16h]
        mov    word ptr [edx], 0fh
        jmp    SHORT loc_2_1576
loc_2_1555:
        cmp    al, 3
        jne    SHORT loc_2_156E
        lea    edx, [esi + 16h]
        test   cl, cl
        jne    SHORT loc_2_1567
        mov    word ptr [edx], 7
        jmp    SHORT loc_2_1576
loc_2_1567:
        mov    word ptr [edx], 4
        jmp    SHORT loc_2_1576
loc_2_156E:
        lea    edx, [esi + 16h]
loc_2_1571:
        mov    word ptr [edx], 3
loc_2_1576:
        mov    cx, word ptr [esi + 1ah]
        and    cx, 7fffh
        cmp    edi, 1000000h
        mov    word ptr [esi + 1ah], cx
        jne    SHORT loc_2_15AC
        cmp    word ptr [edx], 0
        jne    SHORT loc_2_15A5
        xor    edx, edx
        mov    dx, word ptr [esi + 0eh]
        add    edx, 4
        in     al, dx
        and    eax, 0ffh
        test   ah, 80h
        je     SHORT loc_2_15AC
loc_2_15A5:
        or     ch, 80h
        mov    word ptr [esi + 1ah], cx
loc_2_15AC:
        mov    dx, word ptr [esi + 0eh]
        in     al, dx
        mov    cl, al
        and    cl, 3fh
        test   edi, edi
        jne    SHORT loc_2_15C6
        or     cl, 40h
        xor    eax, eax
        mov    al, cl
        jmp    NEAR PTR loc_2_165D
loc_2_15C6:
        cmp    edi, 1000000h
        jne    SHORT loc_2_163A
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0eh]
        lea    edx, [ebx + 4]
        in     al, dx
        mov    ecx, edx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0c3h
        mov    al, byte ptr [ebp - 4]
        or     al, 80h
        out    dx, al
        mov    word ptr [ebp - 8], 8
        lea    edx, [ebx + 0bh]
        in     al, dx
        mov    cl, al
        in     al, dx
        and    cl, 30h
        shr    cl, 4
        mov    dl, al
        shl    word ptr [ebp - 8], cl
        mov    cx, word ptr [ebp - 8]
        and    dl, 0fh
        mov    word ptr [esi + 14h], cx
        mov    cl, al
        and    cl, 0c3h
        shr    cl, 2
        or     cl, dl
        or     cl, 60h
        and    ecx, 0ffh
        shl    ecx, 0dh
        test   al, 80h
        mov    dword ptr [esi + 4], ecx
        je     SHORT loc_2_1631
        or     ecx, 0f00000h
        mov    dword ptr [esi + 4], ecx
loc_2_1631:
        lea    edx, [ebx + 4]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    SHORT loc_2_1663
loc_2_163A:
        xor    edx, edx
        mov    dx, word ptr [esi + 0eh]
        add    edx, 5
        in     al, dx
        and    al, 1fh
        mov    dl, cl
        shl    al, 3
        and    dl, 38h
        shr    dl, 3
        xor    ebx, ebx
        or     al, dl
        and    ecx, 7
        mov    bl, al
        lea    eax, [ecx + ebx*8]
loc_2_165D:
        shl    eax, 0dh
        mov    dword ptr [esi + 4], eax
loc_2_1663:
        cmp    edi, 1000000h
        jne    SHORT loc_2_16A6
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0eh]
        lea    eax, [ebx + 4]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        in     al, dx
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0c3h
        mov    al, byte ptr [ebp - 4]
        or     al, 80h
        out    dx, al
        lea    edx, [ebx + 0ch]
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [ebp - 8]
        and    cl, 0c3h
        and    al, 0fh
        shr    cl, 2
        or     cl, al
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        or     cl, 60h
        jmp    SHORT loc_2_16B8
loc_2_16A6:
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0eh]
        lea    edx, [ebx + 3]
        in     al, dx
        mov    cl, al
        and    cl, 3eh
        or     cl, 40h
loc_2_16B8:
        xor    eax, eax
        mov    al, cl
        shl    eax, 0dh
        cmp    edi, 1000000h
        mov    dword ptr [esi + 8], eax
        jne    SHORT loc_2_1710
        lea    eax, [ebx + 4]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        in     al, dx
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0c3h
        mov    al, byte ptr [ebp - 4]
        or     al, 80h
        out    dx, al
        lea    edx, [ebx + 0ch]
        in     al, dx
        mov    cl, al
        and    cl, 30h
        cmp    cl, 30h
        jne    SHORT loc_2_16F9
        mov    word ptr [esi + 18h], 0
        jmp    SHORT loc_2_1707
loc_2_16F9:
        shr    cl, 4
        mov    ax, 8
        shl    ax, cl
        mov    word ptr [esi + 18h], ax
loc_2_1707:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    SHORT loc_2_1731
loc_2_1710:
        lea    edx, [ebx + 3]
        in     al, dx
        mov    cl, al
        and    cl, 0c0h
        jne    SHORT loc_2_1723
        mov    word ptr [esi + 18h], 0
        jmp    SHORT loc_2_1731
loc_2_1723:
        shr    cl, 6
        mov    ax, 8
        shl    ax, cl
        mov    word ptr [esi + 18h], ax
loc_2_1731:
        mov    cx, word ptr [esi + 1ah]
        lea    edx, [ebx + 7]
        in     al, dx
        and    cx, 9fffh
        test   al, 0fh
        mov    word ptr [esi + 1ah], cx
        je     SHORT loc_2_174D
        or     ch, 20h
        mov    word ptr [esi + 1ah], cx
loc_2_174D:
        mov    cx, word ptr [esi + 1ah]
        lea    eax, [ebx + 4]
        and    cx, 0e7ffh
        mov    edx, eax
        mov    word ptr [esi + 1ah], cx
        cmp    edi, 1000000h
        mov    dword ptr [ebp - 8], eax
        in     al, dx
        jne    SHORT loc_2_1793
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0c3h
        mov    al, byte ptr [ebp - 4]
        or     al, 80h
        out    dx, al
        lea    edx, [ebx + 0dh]
        in     al, dx
        test   al, 20h
        je     SHORT loc_2_178A
        or     ch, 18h
        mov    word ptr [esi + 1ah], cx
loc_2_178A:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    SHORT loc_2_17AF
loc_2_1793:
        test   al, 1
        je     SHORT loc_2_179E
        or     ch, 10h
        mov    word ptr [esi + 1ah], cx
loc_2_179E:
        test   byte ptr [esi], 40h
        je     SHORT loc_2_17AF
        lea    edx, [ebx + 5]
        in     al, dx
        test   al, 20h
        je     SHORT loc_2_17AF
        or     byte ptr [esi + 1bh], 8
loc_2_17AF:
        mov    cx, word ptr [esi + 1ah]
        mov    edx, dword ptr [ebp - 8]
        in     al, dx
        and    cx, 0f8ffh
        cmp    byte ptr [ebp + 14h], 0
        mov    word ptr [esi + 1ah], cx
        je     SHORT loc_2_1812
        and    al, 6
        mov    dl, 1
        cmp    dl, al
        mov    edx, 1
        adc    edx, -1
        cmp    edx, 3
        jne    SHORT loc_2_17E5
        or     cl, 6
        mov    word ptr [esi + 1ah], cx
        jmp    NEAR PTR loc_2_18A5
loc_2_17E5:
        cmp    edx, 2
        jne    SHORT loc_2_17F6
        or     cl, 4
        mov    word ptr [esi + 1ah], cx
        jmp    NEAR PTR loc_2_18A5
loc_2_17F6:
        cmp    al, 1
        jbe    SHORT loc_2_1806
        or     cl, 7
        mov    word ptr [esi + 1ah], cx
        jmp    NEAR PTR loc_2_18A5
loc_2_1806:
        or     cl, 5
        mov    word ptr [esi + 1ah], cx
        jmp    NEAR PTR loc_2_18A5
loc_2_1812:
        cmp    edi, 1000000h
        jne    SHORT loc_2_1873
        cmp    byte ptr [esi + 1fh], 3
        je     SHORT loc_2_1826
        mov    byte ptr [esi + 1eh], 0ffh
        jmp    SHORT loc_2_18A5
loc_2_1826:
        mov    edx, dword ptr [ebp - 8]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 43h
        mov    al, byte ptr [ebp - 4]
        or     al, 80h
        out    dx, al
        lea    edx, [ebx + 0dh]
        in     al, dx
        and    al, 3
        cmp    al, 1
        jne    SHORT loc_2_184E
        mov    byte ptr [esi + 1eh], 1
        or     ch, 2
        mov    word ptr [esi + 1ah], cx
        jmp    SHORT loc_2_186A
loc_2_184E:
        cmp    al, 3
        jne    SHORT loc_2_185F
        mov    byte ptr [esi + 1eh], 2
        or     ch, 4
        mov    word ptr [esi + 1ah], cx
        jmp    SHORT loc_2_186A
loc_2_185F:
        mov    byte ptr [esi + 1eh], 0
        or     ch, 1
        mov    word ptr [esi + 1ah], cx
loc_2_186A:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    SHORT loc_2_18A5
loc_2_1873:
        test   byte ptr [esi], 20h
        je     SHORT loc_2_187F
        or     ch, 10h
        mov    word ptr [esi + 1ah], cx
loc_2_187F:
        and    al, 6
        cmp    al, 2
        jne    SHORT loc_2_188F
        or     byte ptr [esi + 1bh], 1
        mov    byte ptr [esi + 1eh], 0
        jmp    SHORT loc_2_18A5
loc_2_188F:
        cmp    al, 4
        jne    SHORT loc_2_189D
        or     byte ptr [esi + 1bh], 4
        mov    byte ptr [esi + 1eh], 2
        jmp    SHORT loc_2_18A5
loc_2_189D:
        or     byte ptr [esi + 1bh], 2
        mov    byte ptr [esi + 1eh], 1
loc_2_18A5:
        test   byte ptr [esi + 3], 8
        je     SHORT loc_2_1923
        lea    ecx, [ebx + 1]
        mov    al, 4ah
        mov    edx, ecx
        out    dx, al
        in     al, dx
        test   al, 40h
        je     SHORT loc_2_18BF
loc_2_18B8:
        mov    edx, ecx
        in     al, dx
        test   al, 40h
        jne    SHORT loc_2_18B8
loc_2_18BF:
        test   byte ptr [esi + 1], 8
        je     SHORT loc_2_18D3
        lea    edx, [ebx + 0bh]
        in     al, dx
        test   al, 10h
        je     SHORT loc_2_18D3
        mov    byte ptr [esi + 27h], 1
        jmp    SHORT loc_2_18D7
loc_2_18D3:
        mov    byte ptr [esi + 27h], 0
loc_2_18D7:
        lea    edx, [ebx + 0bh]
        in     al, dx
        test   al, 8
        je     SHORT loc_2_18E8
        mov    eax, dword ptr [esi]
        or     eax, 400h
        mov    dword ptr [esi], eax
loc_2_18E8:
        lea    edx, [ebx + 0ch]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        test   al, al
        mov    word ptr [esi + 21h], dx
        je     SHORT loc_2_1906
        test   al, 4
        jne    SHORT loc_2_1906
        or     dword ptr [esi], 4000h
loc_2_1906:
        lea    edx, [ebx + 0eh]
        in     al, dx
        test   al, 1
        je     SHORT loc_2_1912
        or     byte ptr [esi + 1ah], 8
loc_2_1912:
        mov    edx, ecx
        mov    al, 46h
        out    dx, al
        in     al, dx
        test   al, 40h
        je     SHORT loc_2_1923
loc_2_191C:
        mov    edx, ecx
        in     al, dx
        test   al, 40h
        jne    SHORT loc_2_191C
loc_2_1923:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
sub_2_1930:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 8]
        lea    edi, [esi + 56h]
        push   edi
        push   eax
        push   ecx
        push   edx
        db  0E8h, 098h, 002h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_196A
        mov    eax, dword ptr [ebp + 10h]
        pop    edi
        mov    word ptr [eax], 0
        pop    esi
        mov    ax, 0ffffh
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_196A:
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 1], al
        xor    eax, eax
        mov    ax, word ptr [edi]
        cmp    eax, 6fc2h
        jg     SHORT loc_2_1993
        cmp    eax, 6fc0h
        jge    SHORT loc_2_19E9
        cmp    eax, 61c8h
        jl     SHORT loc_2_19B1
        cmp    eax, 61c9h
        jle    SHORT loc_2_19C5
        jmp    SHORT loc_2_19B1
loc_2_1993:
        cmp    eax, 0efd5h
        jg     SHORT loc_2_19AA
        cmp    eax, 0efd4h
        jge    SHORT loc_2_19C5
        cmp    eax, 6fc6h
        je     SHORT loc_2_19C5
        jmp    SHORT loc_2_19B1
loc_2_19AA:
        cmp    eax, 0efe5h
        je     SHORT loc_2_19E9
loc_2_19B1:
        mov    eax, dword ptr [ebp + 10h]
        pop    edi
        mov    word ptr [eax], 0
        pop    esi
        mov    ax, 0ffffh
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_19C5:
        mov    al, byte ptr [esi + 58h]
        and    al, 0f8h
        or     al, 8
        pop    edi
        mov    cl, al
        pop    esi
        and    ecx, 0ffff00ffh
        mov    eax, dword ptr [ebp + 10h]
        shl    cx, 8
        mov    word ptr [eax], cx
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    0ch
loc_2_19E9:
        mov    cl, byte ptr [esi + 58h]
        mov    eax, dword ptr [ebp + 10h]
        and    cx, 0feh
        pop    edi
        shl    cx, 4
        pop    esi
        mov    word ptr [eax], cx
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 16h], bx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 11h], bl
        mov    byte ptr [ebp - 10h], bl
        mov    cx, word ptr [esi + 4]
        mov    al, byte ptr [esi]
        mov    word ptr [ebp - 22h], cx
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 24h], ax
        mov    dl, byte ptr [esi + 1]
        and    edx, 0ffff00ffh
        lea    eax, [esi + 56h]
        mov    word ptr [ebp - 20h], dx
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        lea    edx, [ebp - 8]
        mov    dword ptr [ebp - 2ch], 0d0000h
        push   esi
        mov    dword ptr [edx], ecx
        mov    word ptr [ebp - 1ah], 3
        mov    word ptr [ebp - 1eh], 8
        mov    word ptr [edx + 4], ax
        call   sub_2_1D90
        lea    ecx, [ebp - 30h]
        mov    dl, byte ptr [esi + 5ch]
        push   ecx
        mov    al, byte ptr [esi]
        push   edx
        dec    al
        cmp    al, 1
        mov    edx, dword ptr [esi + 52h]
        sbb    al, al
        xor    ecx, ecx
        neg    al
        mov    cx, word ptr [esi + 4]
        push   eax
        push   ecx
        push   edx
        call   sub_2_10F0
        mov    edi, eax
        push   esi
        mov    word ptr [esi + 3ah], 1
        mov    word ptr [esi + 0cdh], 600h
        call   sub_2_1D30
        mov    ax, word ptr [ebp - 22h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dx, word ptr [ebp - 1ah]
        mov    dword ptr [esi + 0c5h], ecx
        mov    word ptr [esi + 4], ax
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [esi + 0ah], dx
        mov    dword ptr [esi + 6], ebx
        mov    dword ptr [esi + 18h], ecx
        mov    ax, word ptr [ebp - 1eh]
        mov    dx, word ptr [ebp - 1ch]
        mov    word ptr [esi + 16h], ax
        mov    word ptr [esi + 20h], dx
        cmp    ax, dx
        jae    SHORT loc_2_1AE8
        mov    word ptr [esi + 20h], ax
loc_2_1AE8:
        mov    cx, word ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 28h]
        mov    word ptr [esi + 0ch], cx
        mov    dl, byte ptr [ebp - 12h]
        mov    cx, word ptr [ebp - 16h]
        and    edx, 0ffff00ffh
        mov    word ptr [esi + 4ah], dx
        mov    dword ptr [esi + 0eh], eax
        mov    word ptr [esi + 0c9h], cx
        mov    al, byte ptr [ebp - 20h]
        mov    dl, byte ptr [ebp - 11h]
        mov    byte ptr [esi + 1], al
        mov    cx, word ptr [ebp - 14h]
        and    edx, 0ffff00ffh
        mov    word ptr [esi + 4ch], dx
        mov    al, byte ptr [ebp - 10h]
        mov    dx, word ptr [ebp - 0fh]
        and    eax, 0ffff00ffh
        mov    word ptr [esi + 4eh], ax
        test   edi, edi
        mov    word ptr [esi + 2], cx
        mov    al, byte ptr [ebp - 9]
        mov    word ptr [esi + 3eh], dx
        mov    byte ptr [esi + 146h], al
        jne    SHORT loc_2_1B57
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1B57:
        dec    edi
        cmp    edi, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        and    eax, 0ffff0003h
        pop    ebx
        add    eax, 0ffffh
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1B70:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 3ch], 4
        mov    ebp, esp
        jae    SHORT loc_2_1B94
loc_2_1B8E:
        mov    word ptr [eax + 3ch], 4
loc_2_1B94:
        xor    edx, edx
        pop    ebp
        mov    dx, word ptr [eax + 4]
        mov    al, byte ptr [eax + 3ch]
        add    edx, 18h
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1D90
        mov    eax, 0ffffffffh
        xor    ecx, ecx
        mov    word ptr [esi + 0f2h], ax
        mov    byte ptr [esi + 0f4h], cl
        mov    word ptr [esi + 0eeh], cx
        mov    byte ptr [esi + 139h], cl
        test   byte ptr [esi + 0c8h], 8
        mov    word ptr [esi + 0f0h], ax
        je     SHORT loc_2_1BF8
        and    word ptr [esi + 48h], 0fbffh
        jmp    SHORT loc_2_1BFE
loc_2_1BF8:
        mov    word ptr [esi + 48h], 400h
loc_2_1BFE:
        mov    byte ptr [esi + 11dh], 3
        mov    dword ptr [esi + 119h], 180h
        xor    eax, eax
        cmp    word ptr [esi + 3ah], ax
        jbe    SHORT loc_2_1C32
        lea    ecx, [esi + 0fdh]
        xor    edx, edx
loc_2_1C1F:
        mov    dword ptr [ecx], edx
        add    ecx, 4
        inc    eax
        xor    edi, edi
        mov    dword ptr [ecx - 0ch], edx
        mov    di, word ptr [esi + 3ah]
        cmp    edi, eax
        ja     SHORT loc_2_1C1F
loc_2_1C32:
        mov    edx, dword ptr [esi + 1ch]
        xor    eax, eax
        mov    ax, word ptr [esi + 0cdh]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 3ah]
        xor    edi, edi
        imul   eax, ecx
        mov    di, word ptr [esi + 16h]
        add    eax, edx
        mov    ecx, edi
        mov    dword ptr [esi + 120h], eax
        shl    ecx, 0ah
        sub    eax, edx
loc_2_1C5C:
        mov    byte ptr [esi + 0edh], 0
        lea    ebx, [edx + ecx]
        and    edi, 3fffffh
        mov    dword ptr [esi + 124h], ebx
        xor    ebx, ebx
loc_2_1C74:
        cdq
        mov    bx, word ptr [esi + 38h]
        idiv   ebx
        mov    byte ptr [esi + 13ah], al
        cmp    edi, 40h
        jne    SHORT loc_2_1C8F
        mov    byte ptr [esi + 13bh], 0ffh
        jmp    SHORT loc_2_1CA0
loc_2_1C8F:
        mov    eax, ecx
        cdq
        and    edx, 0ffh
loc_2_1C98:
        add    eax, edx
        mov    byte ptr [esi + 13bh], ah
loc_2_1CA0:
        xor    eax, eax
        mov    dword ptr [esi + 13ch], eax
        mov    dword ptr [esi + 109h], eax
        mov    dword ptr [esi + 10dh], eax
        mov    dword ptr [esi + 111h], eax
        mov    dword ptr [esi + 115h], eax
        mov    eax, dword ptr [esi + 0c5h]
        test   al, 8
        jne    SHORT loc_2_1CE0
        test   eax, 8000000h
        je     SHORT loc_2_1CE0
        push   esi
        call   sub_2_1B80
        mov    word ptr [esi + 11eh], 187h
loc_2_1CE0:
        push   esi
        db  0E8h, 01Ah, 004h, 000h, 000h
        test   al, al
        jne    SHORT loc_2_1CFC
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0d1h], 4
        pop    esi
        pop    ebx
loc_2_1CF8:
        pop    ebp
        ret    4
loc_2_1CFC:
        push   esi
        db  0E8h, 0FEh, 012h, 000h, 000h
        push   esi
        db  0E8h, 0F8h, 003h, 000h, 000h
        test   al, al
        jne    SHORT loc_2_1D1E
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0d1h], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_1D1E:
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0d1h], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_2_1D30:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0d1h], 1
        je     SHORT loc_2_1D49
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_2_1D49:
        test   byte ptr [ecx + 0c8h], 8
        lea    esi, [ecx + 4]
        je     SHORT loc_2_1D61
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [esi]
        add    edx, 10h
        jmp    SHORT loc_2_1D6B
loc_2_1D61:
        xor    edx, edx
        mov    al, 20h
        mov    dx, word ptr [esi]
        add    edx, 10h
loc_2_1D6B:
        out    dx, al
        xor    edx, edx
        mov    al, byte ptr [ecx + 0d3h]
        mov    dx, word ptr [esi]
        pop    esi
        add    edx, 1fh
        pop    ebp
        out    dx, al
        xor    ax, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1D90:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        test   byte ptr [ecx + 0c8h], 8
        je     SHORT loc_2_1DAF
        add    ecx, 4
        xor    edx, edx
        mov    dx, word ptr [ecx]
        xor    al, al
        add    edx, 10h
        jmp    SHORT loc_2_1DBC
loc_2_1DAF:
        add    ecx, 4
        xor    edx, edx
        mov    dx, word ptr [ecx]
        mov    al, 20h
        add    edx, 10h
loc_2_1DBC:
        out    dx, al
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [ecx]
        pop    ebp
        add    edx, 1fh
        out    dx, al
        xor    ax, ax
        ret    4
        db  090h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 080h, 0A0h, 0D3h, 000h, 000h, 000h, 0F5h, 066h
        db  033h, 0C0h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h
sub_2_1DF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0d3h]
        and    cl, 0cah
        test   byte ptr [eax + 0c8h], 8
        mov    byte ptr [eax + 0d3h], cl
        je     SHORT loc_2_1E1E
        test   byte ptr [eax + 49h], 4
        jne    SHORT loc_2_1E1E
        and    cl, 0bfh
        mov    byte ptr [eax + 0d3h], cl
loc_2_1E1E:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 080h, 088h, 0D3h, 000h, 000h, 000h, 00Ah, 066h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0d3h]
        or     cl, 35h
        test   byte ptr [eax + 0c8h], 8
        mov    byte ptr [eax + 0d3h], cl
        je     SHORT loc_2_1E7E
        test   byte ptr [eax + 49h], 4
        jne    SHORT loc_2_1E7E
        or     cl, 40h
        mov    byte ptr [eax + 0d3h], cl
loc_2_1E7E:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [edi + 0d1h]
        cmp    al, 1
        jne    SHORT loc_2_1EAB
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1EAB:
        cmp    al, 3
        jne    SHORT loc_2_1EBA
        push   edi
        call   sub_2_1BB0
        test   ax, ax
        jne    SHORT loc_2_1F35
loc_2_1EBA:
        cmp    byte ptr [edi + 0d1h], 2
        je     SHORT loc_2_1ECD
        mov    ax, 7
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1ECD:
        test   byte ptr [edi + 0c8h], 8
        lea    esi, [edi + 4]
        je     SHORT loc_2_1EE5
        xor    edx, edx
        mov    al, 2
        mov    dx, word ptr [esi]
        add    edx, 10h
        jmp    SHORT loc_2_1EEF
loc_2_1EE5:
        xor    edx, edx
        mov    al, 22h
        mov    dx, word ptr [esi]
        add    edx, 10h
loc_2_1EEF:
        out    dx, al
        test   byte ptr [edi + 0c9h], 10h
        je     SHORT loc_2_1F05
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi]
        add    edx, 1dh
        jmp    SHORT loc_2_1F0F
loc_2_1F05:
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [esi]
        add    edx, 1dh
loc_2_1F0F:
        out    dx, al
        mov    byte ptr [edi + 0d1h], 1
        push   edi
        call   sub_2_1D30
        test   byte ptr [edi + 0c8h], 8
        je     SHORT loc_2_1F32
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 6
        in     al, dx
        or     al, 1
        out    dx, al
loc_2_1F32:
        xor    ax, ax
loc_2_1F35:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_1F40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0d1h], 1
        je     SHORT loc_2_1F5B
        mov    ax, 0ah
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1F5B:
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_2_1F71
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 6
        in     al, dx
        and    al, 0feh
        out    dx, al
loc_2_1F71:
        push   esi
        call   sub_2_1D90
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_2_1F8D
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
        jmp    SHORT loc_2_1F98
loc_2_1F8D:
        xor    edx, edx
        mov    al, 21h
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
loc_2_1F98:
        out    dx, al
        mov    edi, 40h
loc_2_1F9E:
        push   3e8h
        db  0E8h, 0C0h, 0FCh, 0FFh, 0FFh
        dec    edi
        jne    SHORT loc_2_1F9E
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0d1h], 3
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_1FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    edx, edx
        push   ebx
        push   esi
        xor    bl, bl
        push   edi
        mov    al, 20h
        mov    edi, dword ptr [ebp + 8]
        mov    esi, 0
        mov    dx, word ptr [edi + 4]
        add    edx, 10h
        out    dx, al
        mov    eax, dword ptr [edi + 147h]
        test   eax, eax
        je     SHORT loc_2_1FEC
        mov    esi, dword ptr [eax + 0ch]
loc_2_1FEC:
        test   esi, 6
        jne    SHORT loc_2_2014
        test   dword ptr [edi + 0c5h], 8100000h
        jne    SHORT loc_2_2034
        push   edi
        lea    eax, [edi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 050h, 0FCh, 0FFh, 0FFh
        jmp    SHORT loc_2_2034
loc_2_2014:
        test   dword ptr [edi + 0c5h], 8100000h
        jne    SHORT loc_2_2032
        push   edi
        lea    eax, [edi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 030h, 0FCh, 0FFh, 0FFh
loc_2_2032:
        mov    bl, 8
loc_2_2034:
        test   esi, 8
        je     SHORT loc_2_203F
        or     bl, 4
loc_2_203F:
        test   esi, 20h
        je     SHORT loc_2_204A
        or     bl, 1ch
loc_2_204A:
        test   byte ptr [edi + 0c8h], 8
        je     SHORT loc_2_2056
        or     bl, 1
loc_2_2056:
        xor    edx, edx
        mov    al, bl
        mov    dx, word ptr [edi + 4]
        pop    edi
        add    edx, 1ch
        pop    esi
        out    dx, al
        xor    ax, ax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 066h, 0B8h, 080h, 000h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_2_2080:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    edx, edx
        push   ebx
        mov    byte ptr [ebp - 2], 1
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        in     al, dx
        and    al, 75h
        je     NEAR PTR loc_2_2351
        xor    ebx, ebx
        mov    byte ptr [ebp - 2], bl
loc_2_20A7:
        test   al, 10h
        je     NEAR PTR loc_2_2155
        mov    byte ptr [esi + 138h], 1
        test   dword ptr [esi + 0c5h], 8100000h
        jne    NEAR PTR loc_2_214A
        xor    ecx, ecx
        mov    al, 21h
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 10h]
        out    dx, al
        lea    edx, [ecx + 1ah]
        xor    al, al
        out    dx, al
        lea    edx, [ecx + 1bh]
        out    dx, al
        lea    edx, [ecx + 17h]
        in     al, dx
        test   al, 80h
        jne    SHORT loc_2_20F6
        mov    edi, 400h
loc_2_20E9:
        push   3e8h
        db  0E8h, 075h, 0FBh, 0FFh, 0FFh
        dec    edi
        jne    SHORT loc_2_20E9
loc_2_20F6:
        xor    ecx, ecx
        mov    al, 2
        mov    cx, word ptr [esi + 4]
        cmp    byte ptr [esi + 0f4h], bl
        lea    edx, [ecx + 1dh]
        out    dx, al
        lea    edx, [ecx + 10h]
        mov    al, 22h
        out    dx, al
        je     SHORT loc_2_214A
        mov    byte ptr [esi + 0f4h], bl
        mov    byte ptr [esi + 31ah], bl
        movsx  eax, word ptr [esi + 0f2h]
        mov    dword ptr [esi + eax*4 + 0fdh], ebx
        mov    word ptr [esi + 0f2h], 0ffffh
        cmp    word ptr [esi + 0f0h], -1
        je     SHORT loc_2_2143
        push   esi
        db  0E8h, 04Dh, 003h, 000h, 000h
loc_2_2143:
        push   esi
        push   ebx
        db  0E8h, 036h, 00Ch, 000h, 000h
loc_2_214A:
        push   esi
        db  0E8h, 0B0h, 007h, 000h, 000h
        jmp    NEAR PTR loc_2_233F
loc_2_2155:
        test   al, 20h
        je     SHORT loc_2_21AE
        xor    edx, edx
        mov    ecx, dword ptr [esi + 9dh]
        mov    dx, word ptr [esi + 4]
        add    edx, 1dh
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0ffh
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 69h]
        mov    dx, word ptr [esi + 4]
        add    edx, 1eh
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0ffh
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 6dh]
        mov    dx, word ptr [esi + 4]
        add    edx, 1fh
        in     al, dx
        and    eax, 0ffh
        xor    edx, edx
        add    dword ptr [ecx], eax
        mov    al, 20h
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        jmp    NEAR PTR loc_2_233E
loc_2_21AE:
        test   al, 1
        je     SHORT loc_2_21BD
        push   esi
        db  0E8h, 048h, 007h, 000h, 000h
        jmp    NEAR PTR loc_2_233F
loc_2_21BD:
        test   al, 40h
        je     NEAR PTR loc_2_22D8
        test   byte ptr [esi + 49h], 4
        je     SHORT loc_2_21DB
        xor    edx, edx
        mov    al, 40h
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        jmp    NEAR PTR loc_2_233E
loc_2_21DB:
        cmp    byte ptr [esi + 146h], bl
        je     SHORT loc_2_220E
        push   esi
        lea    eax, [esi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 06Dh, 0FAh, 0FFh, 0FFh
        push   ebx
        lea    eax, [ebp - 8]
        push   2
        push   eax
        mov    eax, dword ptr [esi + 13ch]
        add    eax, 10h
        push   eax
        push   esi
        db  0E8h, 004h, 00Ah, 000h, 000h
        jmp    SHORT loc_2_225D
loc_2_220E:
        mov    ecx, dword ptr [esi + 0c5h]
        and    ecx, 8000000h
        je     SHORT loc_2_2235
        xor    edi, edi
        mov    al, byte ptr [esi + 144h]
        mov    di, word ptr [esi + 4]
        lea    edx, [edi + 5]
        out    dx, al
        mov    edx, edi
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_2235:
        mov    eax, dword ptr [esi + 13ch]
        test   ecx, ecx
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ebp - 8], ax
        je     SHORT loc_2_225D
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_2_225D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 8], ecx
        cmp    ecx, 5dch
        jbe    SHORT loc_2_2288
        xor    edx, edx
        mov    al, 40h
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        jmp    NEAR PTR loc_2_233E
loc_2_2288:
        add    dword ptr [ebp - 8], 12h
        cmp    dword ptr [ebp - 8], 40h
        jae    SHORT loc_2_2299
        mov    dword ptr [ebp - 8], 40h
loc_2_2299:
        cmp    dword ptr [ebp - 8], 100h
        jae    SHORT loc_2_22A7
        mov    cl, byte ptr [esi + 3ch]
        jmp    SHORT loc_2_22B0
loc_2_22A7:
        mov    ecx, dword ptr [ebp - 8]
        lea    ecx, [ecx + ecx*2]
        shr    ecx, 4
loc_2_22B0:
        xor    edi, edi
        mov    al, 40h
        mov    di, word ptr [esi + 4]
        lea    edx, [edi + 17h]
        out    dx, al
        lea    edx, [edi + 18h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 17h]
        in     al, dx
        test   al, 1
        je     SHORT loc_2_22D2
        push   esi
        db  0E8h, 030h, 006h, 000h, 000h
        jmp    SHORT loc_2_233F
loc_2_22D2:
        test   al, 4
        je     SHORT loc_2_233F
        jmp    SHORT loc_2_22DC
loc_2_22D8:
        test   al, 4
        je     SHORT loc_2_233F
loc_2_22DC:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 1ch
        in     al, dx
        test   al, 2
        mov    cl, al
        je     SHORT loc_2_22F1
        mov    eax, dword ptr [esi + 69h]
        inc    dword ptr [eax]
loc_2_22F1:
        test   cl, 4
        je     SHORT loc_2_230D
        xor    edx, edx
        mov    edi, dword ptr [esi + 9dh]
        mov    dx, word ptr [esi + 4]
        add    edx, 1dh
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [edi], eax
loc_2_230D:
        test   cl, 8
        je     SHORT loc_2_231A
        mov    eax, dword ptr [esi + 0a1h]
        inc    dword ptr [eax]
loc_2_231A:
        test   cl, 10h
        je     SHORT loc_2_2333
        xor    edx, edx
        mov    ecx, dword ptr [esi + 6dh]
        mov    dx, word ptr [esi + 4]
        add    edx, 1fh
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [ecx], eax
loc_2_2333:
        xor    edx, edx
        mov    al, 4
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
loc_2_233E:
        out    dx, al
loc_2_233F:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        in     al, dx
        and    al, 75h
        jne    NEAR PTR loc_2_20A7
loc_2_2351:
        cmp    byte ptr [esi + 0d4h], 0
        je     SHORT loc_2_236D
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0d4h], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_236D:
        mov    eax, 0
        pop    edi
        cmp    byte ptr [ebp - 2], 1
        adc    eax, -1
        pop    esi
        and    ax, 3
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2390:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        in     al, dx
        and    al, 1ah
        test   al, 0ah
        je     NEAR PTR loc_2_254A
loc_2_23AD:
        test   al, 10h
        je     SHORT loc_2_23C1
        test   dword ptr [esi + 0c5h], 8100000h
        je     NEAR PTR loc_2_253C
loc_2_23C1:
        test   al, 2
        jne    NEAR PTR loc_2_24B4
        test   al, 8
        je     NEAR PTR loc_2_251E
        xor    ecx, ecx
        mov    al, 8
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 17h]
        out    dx, al
        lea    edx, [ecx + 14h]
        in     al, dx
        mov    bl, al
        and    bl, 8
        je     SHORT loc_2_23F0
        mov    eax, dword ptr [esi + 0a9h]
        jmp    SHORT loc_2_23FA
loc_2_23F0:
        test   al, 20h
        je     SHORT loc_2_23FC
        mov    eax, dword ptr [esi + 0c1h]
loc_2_23FA:
        inc    dword ptr [eax]
loc_2_23FC:
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_2_241C
        mov    al, byte ptr [esi + 11dh]
        dec    al
        mov    byte ptr [esi + 11dh], al
        jne    SHORT loc_2_2476
        mov    byte ptr [esi + 11dh], 3
loc_2_241C:
        mov    byte ptr [esi + 0f4h], 0
        mov    byte ptr [esi + 31ah], 0
        movsx  eax, word ptr [esi + 0f2h]
        mov    dword ptr [esi + eax*4 + 0fdh], 0
        mov    word ptr [esi + 0f2h], 0ffffh
        cmp    word ptr [esi + 0f0h], -1
        je     SHORT loc_2_2455
        push   esi
        db  0E8h, 03Bh, 000h, 000h, 000h
loc_2_2455:
        push   esi
        mov    eax, 0ch
        cmp    bl, 1
        adc    eax, -1
        push   eax
        db  0E8h, 019h, 009h, 000h, 000h
        cmp    ax, 5
        je     NEAR PTR loc_2_254A
        jmp    NEAR PTR loc_2_251E
loc_2_2476:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 4]
        movsx  ebx, word ptr [esi + 0f2h]
        mov    cx, word ptr [esi + ebx*4 + 0f5h]
        lea    edx, [edi + 14h]
loc_2_2490:
        mov    ax, word ptr [esi + 0cdh]
        imul   eax, ebx
        add    eax, dword ptr [esi + 18h]
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 15h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 16h]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 10h]
        mov    al, 26h
        out    dx, al
        jmp    SHORT loc_2_251E
loc_2_24B4:
        xor    ecx, ecx
        mov    al, 2
        mov    cx, word ptr [esi + 4]
        mov    byte ptr [esi + 11dh], 3
        lea    edx, [ecx + 17h]
        out    dx, al
        lea    edx, [ecx + 14h]
        in     al, dx
        test   al, 10h
        je     SHORT loc_2_24D7
        mov    eax, dword ptr [esi + 0bdh]
        inc    dword ptr [eax]
loc_2_24D7:
        mov    byte ptr [esi + 0f4h], 0
        mov    byte ptr [esi + 31ah], 0
        movsx  eax, word ptr [esi + 0f2h]
        mov    dword ptr [esi + eax*4 + 0fdh], 0
        mov    word ptr [esi + 0f2h], 0ffffh
        cmp    word ptr [esi + 0f0h], -1
        je     SHORT loc_2_2510
        push   esi
        db  0E8h, 080h, 0FFh, 0FFh, 0FFh
loc_2_2510:
        push   esi
        push   0
        db  0E8h, 068h, 008h, 000h, 000h
        cmp    ax, 5
        je     SHORT loc_2_254A
loc_2_251E:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 17h
        in     al, dx
        and    al, 1ah
        test   al, 0ah
        jne    NEAR PTR loc_2_23AD
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_253C:
        xor    eax, eax
        mov    byte ptr [esi + 0f4h], al
        mov    byte ptr [esi + 31ah], al
loc_2_254A:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2560:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    di, word ptr [esi + 0eeh]
        movsx  eax, di
        lea    ebx, [esi + eax*4 + 0fdh]
        cmp    dword ptr [ebx], 0
        je     SHORT loc_2_258A
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_258A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 1
        push   eax
        push   esi
        db  0E8h, 0A6h, 0FFh, 0FFh, 0FFh
        mov    ax, word ptr [esi + 0eeh]
        inc    ax
        mov    word ptr [esi + 0eeh], ax
        cmp    ax, 2
        jne    SHORT loc_2_25B9
        mov    word ptr [esi + 0eeh], 0
loc_2_25B9:
        mov    ax, word ptr [esi + 48h]
        test   al, 20h
        jne    SHORT loc_2_25F2
        mov    dword ptr [ebx], 2
        cmp    word ptr [esi + 0f0h], -1
        jne    SHORT loc_2_25FA
        cmp    word ptr [esi + 0f2h], -1
        mov    word ptr [esi + 0f0h], di
        jne    SHORT loc_2_25FA
        push   esi
        db  0E8h, 0A8h, 0FEh, 0FFh, 0FFh
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_25F2:
        and    ax, 0ffdfh
        mov    word ptr [esi + 48h], ax
loc_2_25FA:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2610:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 34h
        mov    dword ptr [ecx], 0
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 18h]
        lea    ecx, [esi + edx]
        mov    eax, dword ptr [edi + 140h]
        cmp    ecx, eax
        jbe    SHORT loc_2_263D
        sub    eax, edx
        mov    esi, eax
loc_2_263D:
        mov    eax, dword ptr [edi + 134h]
        push   0
        add    eax, edx
        lea    ecx, [ebp - 24h]
        push   ecx
        mov    edx, dword ptr [ebp + 14h]
        push   0
        mov    dword ptr [ebp - 20h], esi
        push   0
        mov    dword ptr [ebp - 18h], eax
        push   edx
        db  0E8h, 03Ah, 0F6h, 0FFh, 0FFh
        lea    ecx, [ebp - 2ch]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   edx
        push   eax
        db  0E8h, 04Dh, 0F6h, 0FFh, 0FFh
        mov    dword ptr [ebp - 28h], 0
        test   esi, esi
        je     NEAR PTR loc_2_2930
loc_2_267E:
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 20h]
        sub    eax, dword ptr [ebp - 28h]
        cmp    eax, dword ptr [ebp - 20h]
        ja     SHORT loc_2_268E
        mov    ecx, eax
loc_2_268E:
        mov    eax, dword ptr [edi + 124h]
        mov    dword ptr [ebp - 10h], ecx
        mov    ecx, dword ptr [ebp - 18h]
        add    ecx, dword ptr [ebp - 10h]
        cmp    ecx, eax
        jb     SHORT loc_2_26A7
        sub    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 10h], eax
loc_2_26A7:
        mov    ecx, dword ptr [edi + 0c5h]
        test   cl, 8
        je     NEAR PTR loc_2_2840
        test   ecx, 8100000h
        jne    NEAR PTR loc_2_2840
        mov    eax, dword ptr [ebp - 0ch]
        xor    edx, edx
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 18h]
        and    al, 3
        mov    dl, al
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 30h], edx
        sub    eax, edx
        and    ecx, 8000000h
        mov    dword ptr [ebp - 1ch], eax
        je     SHORT loc_2_2700
        xor    esi, esi
        mov    al, byte ptr [edi + 144h]
        mov    si, word ptr [edi + 4]
        lea    edx, [esi + 5]
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_2700:
        mov    eax, dword ptr [ebp - 1ch]
        test   ecx, ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 14h], edx
        je     SHORT loc_2_2722
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 145h]
        out    dx, al
loc_2_2722:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, 4
        sub    eax, dword ptr [ebp - 30h]
        cmp    dword ptr [ebp - 10h], eax
        jae    SHORT loc_2_2735
        mov    eax, dword ptr [ebp - 10h]
loc_2_2735:
        xor    edx, edx
        test   eax, eax
        je     SHORT loc_2_2751
loc_2_273B:
        mov    ebx, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 8]
        add    cl, dl
        inc    esi
        shl    cl, 3
        inc    edx
        shr    ebx, cl
        mov    byte ptr [esi - 1], bl
        cmp    eax, edx
        ja     SHORT loc_2_273B
loc_2_2751:
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], esi
        add    dword ptr [ebp - 1ch], 4
        sub    ecx, eax
        mov    dword ptr [ebp - 8], ecx
        shr    ecx, 2
        test   ecx, ecx
        je     SHORT loc_2_27BA
        mov    esi, dword ptr [edi + 0c5h]
        mov    eax, dword ptr [ebp - 1ch]
        and    esi, 8000000h
        lea    edx, [eax + ecx*4]
        mov    dword ptr [ebp - 1ch], edx
loc_2_277C:
        test   esi, esi
        je     SHORT loc_2_27B3
        xor    ebx, ebx
        mov    al, byte ptr [edi + 144h]
        mov    bx, word ptr [edi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
        test   esi, esi
        je     SHORT loc_2_27B3
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 145h]
        out    dx, al
loc_2_27B3:
        add    dword ptr [ebp - 0ch], 4
        dec    ecx
        jne    SHORT loc_2_277C
loc_2_27BA:
        test   byte ptr [ebp - 8], 3
        je     SHORT loc_2_2835
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 0ch]
        and    cl, 3
        mov    ebx, dword ptr [edi + 0c5h]
        and    ebx, 8000000h
        je     SHORT loc_2_27F4
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        mov    dword ptr [ebp - 8], eax
        lea    edx, [eax + 5]
        mov    al, byte ptr [edi + 144h]
        out    dx, al
        mov    edx, dword ptr [ebp - 8]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_27F4:
        mov    eax, dword ptr [ebp - 1ch]
        test   ebx, ebx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 14h], edx
        je     SHORT loc_2_2816
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 145h]
        out    dx, al
loc_2_2816:
        xor    edx, edx
        test   cl, cl
        je     SHORT loc_2_2835
        xor    ebx, ebx
        mov    bl, cl
loc_2_2820:
        lea    ecx, [edx*8]
        mov    eax, dword ptr [ebp - 14h]
        inc    esi
        inc    edx
        shr    eax, cl
        mov    byte ptr [esi - 1], al
        cmp    ebx, edx
        ja     SHORT loc_2_2820
loc_2_2835:
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_28C2
loc_2_2840:
        cmp    byte ptr [edi + 146h], 0
        je     SHORT loc_2_2871
        push   edi
        lea    eax, [edi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 007h, 0F4h, 0FFh, 0FFh
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        mov    edx, dword ptr [ebp - 18h]
        push   edx
        push   edi
        db  0E8h, 0A1h, 003h, 000h, 000h
        jmp    SHORT loc_2_28C2
loc_2_2871:
        test   ecx, 8000000h
        je     SHORT loc_2_2892
        xor    ecx, ecx
        mov    al, byte ptr [edi + 144h]
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_2892:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 0ADh, 0F7h, 0FFh, 0FFh
        test   byte ptr [edi + 0c8h], 8
        je     SHORT loc_2_28C2
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 145h]
        out    dx, al
loc_2_28C2:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ebp - 18h], eax
        sub    dword ptr [ebp - 20h], eax
        add    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_2_2930
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [edi + 124h], eax
        jne    SHORT loc_2_28EA
        mov    eax, dword ptr [edi + 120h]
        mov    dword ptr [ebp - 18h], eax
loc_2_28EA:
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 28h], eax
        mov    ecx, dword ptr [ebp - 28h]
        cmp    ecx, dword ptr [ebp - 2ch]
        jne    SHORT loc_2_2926
        lea    eax, [ebp - 24h]
        mov    ecx, dword ptr [ebp - 24h]
        push   eax
        push   ecx
        db  0E8h, 0D8h, 0F3h, 0FFh, 0FFh
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_2_2930
        lea    eax, [ebp - 2ch]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 24h]
        push   ecx
        push   edx
        db  0E8h, 09Dh, 0F3h, 0FFh, 0FFh
        mov    dword ptr [ebp - 28h], 0
loc_2_2926:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_2_267E
loc_2_2930:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_2940:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 10h]
        lea    ebx, [edx + eax]
        mov    ecx, dword ptr [esi + 140h]
        cmp    ebx, ecx
        jbe    SHORT loc_2_2966
        sub    ecx, eax
        mov    edx, ecx
loc_2_2966:
        mov    eax, dword ptr [esi + 134h]
        mov    dword ptr [ebp - 1ch], edx
        mov    dword ptr [ebp - 18h], eax
        test   edx, edx
        je     NEAR PTR loc_2_2BC6
loc_2_297A:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 1ch]
        add    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 124h]
        cmp    ecx, eax
        jb     SHORT loc_2_2996
        sub    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], eax
loc_2_2996:
        mov    ecx, dword ptr [esi + 0c5h]
        test   cl, 8
        je     NEAR PTR loc_2_2B1E
        test   ecx, 8100000h
        jne    NEAR PTR loc_2_2B1E
        mov    eax, dword ptr [ebp - 18h]
        xor    edx, edx
        and    al, 3
        mov    dword ptr [ebp - 24h], edi
        mov    dl, al
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 20h], edx
        sub    eax, edx
        and    ecx, 8000000h
        mov    dword ptr [ebp - 0ch], eax
        je     SHORT loc_2_29EC
        xor    ebx, ebx
        mov    al, byte ptr [esi + 144h]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_29EC:
        mov    eax, dword ptr [ebp - 0ch]
        test   ecx, ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 10h], edx
        je     SHORT loc_2_2A0E
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_2_2A0E:
        mov    edx, 4
        sub    edx, dword ptr [ebp - 20h]
        cmp    dword ptr [ebp - 14h], edx
        jae    SHORT loc_2_2A1E
        mov    edx, dword ptr [ebp - 14h]
loc_2_2A1E:
        xor    ebx, ebx
        test   edx, edx
        je     SHORT loc_2_2A3A
loc_2_2A24:
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ebp - 8]
        add    cl, bl
        inc    edi
        shl    cl, 3
        inc    ebx
        shr    eax, cl
        mov    byte ptr [edi - 1], al
        cmp    ebx, edx
        jb     SHORT loc_2_2A24
loc_2_2A3A:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 14h]
        sub    eax, edx
        mov    ecx, eax
        mov    dword ptr [ebp - 8], eax
        shr    ecx, 2
        test   ecx, ecx
        je     SHORT loc_2_2AA1
loc_2_2A4F:
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_2_2A71
        xor    ebx, ebx
        mov    al, byte ptr [esi + 144h]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_2A71:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [eax]
        mov    dword ptr [edi], edx
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_2_2A97
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_2_2A97:
        add    dword ptr [ebp - 0ch], 4
        add    edi, 4
        dec    ecx
        jne    SHORT loc_2_2A4F
loc_2_2AA1:
        test   byte ptr [ebp - 8], 3
        je     SHORT loc_2_2B19
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0c5h]
        and    al, 3
        and    ecx, 8000000h
        mov    byte ptr [ebp - 8], al
        je     SHORT loc_2_2AD6
        xor    ebx, ebx
        mov    al, byte ptr [esi + 144h]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_2AD6:
        mov    eax, dword ptr [ebp - 0ch]
        test   ecx, ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 10h], edx
        je     SHORT loc_2_2AF8
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_2_2AF8:
        xor    ebx, ebx
        cmp    byte ptr [ebp - 8], bl
        je     SHORT loc_2_2B19
        xor    edx, edx
        mov    dl, byte ptr [ebp - 8]
loc_2_2B04:
        lea    ecx, [ebx*8]
        mov    eax, dword ptr [ebp - 10h]
        inc    edi
        inc    ebx
        shr    eax, cl
        mov    byte ptr [edi - 1], al
        cmp    edx, ebx
        ja     SHORT loc_2_2B04
loc_2_2B19:
        mov    edi, dword ptr [ebp - 24h]
        jmp    SHORT loc_2_2B9A
loc_2_2B1E:
        cmp    byte ptr [esi + 146h], 0
        je     SHORT loc_2_2B4C
        push   esi
        lea    eax, [esi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 029h, 0F1h, 0FFh, 0FFh
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    ecx, dword ptr [ebp - 18h]
        push   edi
        push   ecx
        push   esi
        db  0E8h, 0C6h, 000h, 000h, 000h
        jmp    SHORT loc_2_2B9A
loc_2_2B4C:
        test   ecx, 8000000h
        je     SHORT loc_2_2B6D
        xor    ecx, ecx
        mov    al, byte ptr [esi + 144h]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_2_2B6D:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        push   eax
        push   ecx
        push   edi
        db  0E8h, 0D5h, 0F4h, 0FFh, 0FFh
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_2_2B9A
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_2_2B9A:
        mov    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 18h], eax
        add    dword ptr [ebp + 10h], eax
        sub    dword ptr [ebp - 1ch], eax
        je     SHORT loc_2_2BC6
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [esi + 124h], eax
        jne    SHORT loc_2_2BBC
        mov    eax, dword ptr [esi + 120h]
        mov    dword ptr [ebp - 18h], eax
loc_2_2BBC:
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_2_297A
loc_2_2BC6:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2BE0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    al, 60h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 10h]
        out    dx, al
        lea    edx, [ecx + 17h]
        in     al, dx
        xor    edx, edx
        mov    byte ptr [esi + 0edh], al
        mov    dx, word ptr [esi + 4]
        mov    al, 20h
        add    edx, 10h
        pop    esi
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
loc_2_2C10:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    di, word ptr [eax + 4]
        mov    cl, 18h
        mov    al, 60h
        lea    esi, [edi + 10h]
        mov    edx, esi
        out    dx, al
loc_2_2C29:
        xor    eax, eax
        mov    al, cl
        inc    cl
        cmp    cl, 1fh
        lea    edx, [eax + edi]
        mov    al, 0ffh
        out    dx, al
        jbe    SHORT loc_2_2C29
        mov    edx, esi
        mov    al, 20h
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2C50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    di, word ptr [eax + 4]
        mov    cl, 18h
        mov    al, 60h
        lea    esi, [edi + 10h]
        mov    edx, esi
        out    dx, al
loc_2_2C69:
        xor    eax, eax
        mov    al, cl
        inc    cl
        lea    edx, [eax + edi]
        xor    al, al
        out    dx, al
        cmp    cl, 1fh
        jbe    SHORT loc_2_2C69
        mov    edx, esi
        mov    al, 20h
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 0C1h, 0EFh, 0FFh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_2CC4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2CC4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_2CDE
        mov    esi, 2
loc_2_2CDE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 07Dh, 0EFh, 0FFh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_2D12
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2DB0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2D12:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 045h, 0EFh, 0FFh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_2D41
        push   eax
        call   sub_2_2DB0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2D41:
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
loc_2_2D80:
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
sub_2_2DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_2DD0
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 0A4h, 0EEh, 0FFh, 0FFh
loc_2_2DD0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_2DE6
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 08Eh, 0EEh, 0FFh, 0FFh
loc_2_2DE6:
        push   0
        push   32h
        push   esi
        db  0E8h, 084h, 0EEh, 0FFh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_2E19
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2E19:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 042h, 0EEh, 0FFh, 0FFh
        or     eax, eax
        je     SHORT loc_2_2E39
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2E39:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_2E40:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_2E53
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_2E40
        jmp    SHORT loc_2_2E62
loc_2_2E53:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_2E62:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_2E78
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_2E78:
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
sub_2_2ED0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_3270
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_2EF9
        cmp    edi, 103h
        jne    SHORT loc_2_2F10
loc_2_2EF9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        push   ebx
        call   sub_2_2F90
        or     eax, eax
        je     SHORT loc_2_2F10
        mov    edi, eax
loc_2_2F10:
        or     edi, edi
        je     SHORT loc_2_2F1C
        cmp    edi, 103h
        jne    SHORT loc_2_2F7B
loc_2_2F1C:
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        jne    SHORT loc_2_2F76
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_2F32
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_2F32:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_2F41
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_2F47
loc_2_2F41:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 10h], eax
loc_2_2F47:
        cmp    byte ptr [esi + 18h], 0
        je     SHORT loc_2_2F56
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 07Ah, 0ECh, 0FFh, 0FFh
loc_2_2F56:
        mov    eax, 1
        mov    cl, byte ptr [esi + 19h]
        shl    eax, cl
        or     dword ptr [ebx + 28h], eax
        push   0
        push   1ah
        push   esi
        db  0E8h, 007h, 0EDh, 0FFh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_2F76:
        mov    edi, 0c001000eh
loc_2_2F7B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2F90:
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
        db  0E8h, 0A5h, 0F0h, 0FFh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 08Dh, 0F0h, 0FFh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_3014
        sub    eax, eax
        nop
loc_2_2FF0:
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
        ja     SHORT loc_2_2FF0
loc_2_3014:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_307F
loc_2_3021:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_3074
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
        db  0E8h, 022h, 0F0h, 0FFh, 0FFh
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
        db  0E8h, 001h, 0F0h, 0FFh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_3077
loc_2_3074:
        inc    dword ptr [ebp - 4]
loc_2_3077:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_3021
loc_2_307F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_316A
        mov    edi, dword ptr [ebp + 18h]
loc_2_3093:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_3450
        or     al, al
        je     SHORT loc_2_30CA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_3158
loc_2_30CA:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_31A4
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
        db  0E8h, 07Ch, 0EFh, 0FFh, 0FFh
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
        db  0E8h, 044h, 0EFh, 0FFh, 0FFh
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
loc_2_3158:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 14h], eax
        ja     NEAR PTR loc_2_3093
loc_2_316A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0ch], edx
        je     SHORT loc_2_31F5
        mov    edi, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_3180:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_31FB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    SHORT loc_2_31E2
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    SHORT loc_2_31E2
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_31E9
loc_2_31A4:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0B6h, 0EEh, 0FFh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 0A2h, 0EEh, 0FFh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_31E2:
        mov    dword ptr [ebp - 8], 1
loc_2_31E9:
        add    edi, 6
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0ch], edx
        ja     SHORT loc_2_3180
loc_2_31F5:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_3260
loc_2_31FB:
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
        je     SHORT loc_2_3262
        cmp    edi, 103h
        je     SHORT loc_2_3262
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 02Bh, 0EEh, 0FFh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 01Dh, 0EEh, 0FFh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_3260:
        sub    edi, edi
loc_2_3262:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_3270:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        mov    dword ptr [edi + 0ch], 0
        je     SHORT loc_2_32AD
        lea    esp, [esp]
loc_2_32A0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_32A0
loc_2_32AD:
        mov    ecx, dword ptr [edi + 0ch]
        cmp    esi, ecx
        je     SHORT loc_2_32E1
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_2_32E3
        cmp    eax, 103h
        je     SHORT loc_2_32E3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_32E1:
        sub    eax, eax
loc_2_32E3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_32F0:
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
        je     SHORT loc_2_3322
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_3317:
        test   dword ptr [esi], edx
        je     SHORT loc_2_331C
        inc    eax
loc_2_331C:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_3317
loc_2_3322:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_3330:
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
        jbe    SHORT loc_2_33AC
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_3370:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_339A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_33C6
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 0DCh, 0ECh, 0FFh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_339A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_3370
loc_2_33AC:
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
loc_2_33C6:
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
sub_2_33F0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_3420
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_3420:
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
        db  0E8h, 02Ch, 0ECh, 0FFh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_3450:
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
        je     NEAR PTR loc_2_34E9
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_34E9
        nop
loc_2_3470:
        cmp    edx, ecx
        jb     SHORT loc_2_34E9
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_3493
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_34C7
loc_2_3493:
        jbe    SHORT loc_2_34A0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_34C7
        db  08Bh, 0FFh
loc_2_34A0:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_34B7
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_34C7
loc_2_34B7:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_34C7
        mov    dword ptr [ebp - 4], 0
loc_2_34C7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_34F9
        jle    SHORT loc_2_34D8
        or     edx, edx
        je     SHORT loc_2_34E9
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_34DB
loc_2_34D8:
        lea    ecx, [edx + 1]
loc_2_34DB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_3470
loc_2_34E9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_34F9:
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
