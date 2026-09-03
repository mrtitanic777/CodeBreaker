; hpisa_pcode_04.asm -- HPISA object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0020:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        test   ecx, ecx
        jne    SHORT loc_4_004C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        lea    edx, [ebp - 4]
        push   3
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0043:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_004C:
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        mov    ecx, dword ptr [edx + 201h]
        lea    eax, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0068:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0080:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 0fh
        sub    esp, 3ch
        lea    edx, [ebp - 3ch]
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 3ch]
        rep stosd
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 10h]
        cmp    byte ptr [eax], cl
        je     SHORT loc_4_00B3
loc_4_00A1:
        cmp    ecx, 3ch
        jae    SHORT loc_4_00B3
        mov    bl, byte ptr [eax + ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [eax + ecx], 0
        jne    SHORT loc_4_00A1
loc_4_00B3:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 10h]
        push   edx
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 18h]
        push   ecx
        mov    eax, dword ptr [ebp - 1ch]
        push   edx
        mov    ecx, dword ptr [ebp - 20h]
        push   eax
        mov    edx, dword ptr [ebp - 24h]
        push   ecx
        mov    eax, dword ptr [ebp - 28h]
        push   edx
        mov    ecx, dword ptr [ebp - 2ch]
        push   eax
        mov    edx, dword ptr [ebp - 30h]
        push   ecx
        mov    eax, dword ptr [ebp - 34h]
        push   edx
        mov    ecx, dword ptr [ebp - 38h]
        push   eax
        mov    edx, dword ptr [ebp - 3ch]
        push   ecx
        mov    esi, dword ptr [ebp + 8]
        push   edx
        push   12h
        push   0aa000003h
        mov    eax, dword ptr [esi + 201h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0111:
        mov    di, word ptr [ebp + 0ch]
        add    esp, 54h
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax], di
        je     SHORT loc_4_012E
        push   eax
        call   sub_4_2C70
        mov    eax, dword ptr [esi]
        mov    word ptr [eax], di
        or     dword ptr [esi + 47h], 2
loc_4_012E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0140:
        push   3e8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_014A:
        db  0FFh, 005h, 052h, 000h, 000h, 000h
        db  0A1h, 052h, 000h, 000h, 000h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh
        db  0ECh, 05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   esi
        mov    ebx, dword ptr [ebp + 14h]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 18h]
        push   ebx
        mov    edi, dword ptr [esi + 4bh]
        and    ecx, 0ffffh
        push   eax
        lea    edx, [esi + 53h]
        mov    dword ptr [esi + 63h], ebx
        mov    dword ptr [esi + 5fh], ecx
        lea    ecx, [ebp - 4]
        mov    eax, dword ptr [esi + 4fh]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01A7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_01BB
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_01BB:
        mov    ax, word ptr [ebp + 18h]
        dec    ax
        mov    ecx, dword ptr [esi + 53h]
        cmp    ax, 1
        sbb    al, al
        lea    edx, [ebp - 4]
        inc    al
        push   eax
        push   ebx
        push   0
        push   ecx
        push   edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01DB:
        mov    eax, 0
        pop    edi
        cmp    dword ptr [ebp - 4], 1
        adc    eax, -1
        pop    esi
        and    eax, 0fffffffeh
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 048h, 04Bh, 051h, 0E8h, 000h, 000h, 000h, 000h, 048h, 05Dh, 0C2h
        db  008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 5fh]
        mov    ecx, dword ptr [esi + 63h]
        dec    eax
        mov    edx, dword ptr [esi + 53h]
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [esi + 4bh]
        push   edx
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_024D:
        mov    ecx, dword ptr [esi + 53h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0256:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0260:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        in     al, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_0270:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        in     ax, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0280:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    al, byte ptr [esp + 10h]
        out    dx, al
        pop    ebp
        ret    0ch
sub_4_0290:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ax, word ptr [esp + 10h]
        mov    ebp, esp
        out    dx, ax
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_02B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3B40
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_02D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 47h], 0bh
        jne    SHORT loc_4_0300
        push   esi
        call   sub_4_19F0
        test   al, al
        je     SHORT loc_4_0300
        mov    dword ptr [esi + 0b3h], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3960
        push   esi
        call   sub_4_1A20
loc_4_0300:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0310:
        push   ebp
        mov    eax, 0dh
        mov    ebp, esp
        sub    esp, 48h
        mov    word ptr [ebp - 30h], ax
        push   ebx
        mov    word ptr [ebp - 2eh], ax
        push   esi
        push   edi
        mov    eax, 0ah
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 40h], ax
        mov    word ptr [ebp - 3eh], ax
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 38h], ax
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 36h], ax
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 48h], ax
        mov    esi, dword ptr [edi + 4]
        mov    word ptr [ebp - 46h], ax
        mov    ebx, dword ptr [edi + 8]
        mov    eax, 18h
        mov    ecx, dword ptr [edi + 14h]
        mov    word ptr [ebp - 28h], ax
        mov    edx, dword ptr [edi + 18h]
        mov    word ptr [ebp - 26h], ax
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [edi + 10h]
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 10h], edx
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 4]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_039F:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_03B3
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_03B3:
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03CA:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_03D6
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax + 4]
loc_4_03D6:
        push   1
        lea    eax, [ebp - 40h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03ED:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_04AA
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    edx, [ebp - 38h]
        mov    ecx, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 20h], ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0417:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_04AA
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    edx, [ebp - 30h]
        mov    ecx, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 1ch], ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0441:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0450
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], ecx
loc_4_0450:
        push   1
        lea    eax, [ebp - 28h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0467:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0473
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 4]
loc_4_0473:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_047C:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], ebx
        mov    dword ptr [edi + 0ch], ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [edi + 10h], edx
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 18h], ecx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_04AA:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04B3:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_04D0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 1ch
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    dword ptr [ebp - 0ch], eax
        push   edx
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 8], 0ffffffffh
        mov    dword ptr [ebp - 4], 14h
        push   eax
        call   sub_4_0310
        test   eax, eax
        je     SHORT loc_4_0515
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0515:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 205h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 21dh], edx
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 219h], eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx + 211h], edx
        jne    SHORT loc_4_054B
        mov    dword ptr [ecx + 20h], 1
        jmp    SHORT loc_4_0558
loc_4_054B:
        cmp    dword ptr [ebp - 0ch], 1
        jne    SHORT loc_4_0558
        mov    dword ptr [ecx + 20h], 2
loc_4_0558:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 209h], eax
        mov    dword ptr [ecx + 20dh], edx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0580:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     SHORT loc_4_05A1
        mov    eax, 0c0010002h
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05A1:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 10h]
        test   ecx, ecx
        jne    SHORT loc_4_05BB
        push   edx
        push   esi
        push   edi
        call   sub_4_09D0
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05BB:
        cmp    ecx, 1
        jne    SHORT loc_4_05F3
        mov    ecx, dword ptr [edi + 47h]
        test   ecx, ecx
        je     SHORT loc_4_05E3
        test   cl, 2
        je     SHORT loc_4_05D1
        mov    eax, 0c0000001h
loc_4_05D1:
        test   cl, 1
        je     SHORT loc_4_05F8
        mov    eax, 0c001000dh
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05E3:
        push   edx
        push   esi
        push   edi
        call   sub_4_0A20
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05F3:
        mov    eax, 10001h
loc_4_05F8:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_0600:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 24h
        mov    word ptr [ebp - 2], ax
        push   ebx
        push   esi
        mov    dword ptr [ebp - 1ch], eax
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 18h], eax
        cmp    esi, 1010101h
        mov    eax, esi
        ja     SHORT loc_4_063F
        je     NEAR PTR loc_4_0825
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_4_0660
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_063F:
        cmp    eax, 1010102h
        je     NEAR PTR loc_4_0844
        cmp    eax, 1010103h
        je     NEAR PTR loc_4_0863
        cmp    eax, 1010104h
        je     NEAR PTR loc_4_0913
loc_4_0660:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_4_0929
loc_4_066C:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_4_0681
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_4_092F
loc_4_0681:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_4_092F
loc_4_0690:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 47h]
        test   al, 2
        je     SHORT loc_4_06A3
        mov    dword ptr [ebp - 18h], 4
        jmp    SHORT loc_4_06B5
loc_4_06A3:
        mov    dword ptr [ebp - 18h], 2
        test   al, 1
        jne    SHORT loc_4_06B5
        mov    dword ptr [ebp - 18h], 0
loc_4_06B5:
        lea    eax, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_4_092F
loc_4_06C2:
        lea    eax, [ebp - 1ch]
        mov    edi, 4
        jmp    NEAR PTR loc_4_092F
loc_4_06CF:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_4_092F
loc_4_06E3:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_4_092F
loc_4_06F7:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_4_092F
loc_4_070B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a3h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_092F
loc_4_0724:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a7h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_092F
loc_4_073D:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_092F
loc_4_0751:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0abh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_092F
loc_4_076A:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0781:
        mov    byte ptr [ebp - 21h], 0
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_092F
loc_4_078D:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 14h
        jmp    NEAR PTR loc_4_092F
loc_4_079C:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_07AD
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7bh]
        mov    ecx, dword ptr [eax + 0ch]
        jmp    SHORT loc_4_07B6
loc_4_07AD:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [eax + 8]
loc_4_07B6:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_4_092F
loc_4_07C6:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_07D4
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7fh]
        jmp    SHORT loc_4_07DA
loc_4_07D4:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 21h]
loc_4_07DA:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_092F
loc_4_07EA:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_4_092F
loc_4_07FD:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_092F
loc_4_0811:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_4_092F
loc_4_0825:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_083C:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_092F
loc_4_0844:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 67h
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_085B:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_092F
loc_4_0863:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_08B9
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 7bh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_0890
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_0929
loc_4_0890:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08A6:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ebp - 0ch], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0929
loc_4_08B9:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [edi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08CC:
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_08E7
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0929
loc_4_08E7:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebx + 1dh]
        push   edx
        mov    ecx, dword ptr [edi + 7bh]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0904:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0929
loc_4_0913:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 20dh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    SHORT loc_4_092F
loc_4_0929:
        mov    eax, dword ptr [ebp - 14h]
        mov    edi, dword ptr [ebp - 14h]
loc_4_092F:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_096A
        cmp    esi, 1010103h
        je     SHORT loc_4_096A
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_4_095A
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    ecx, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_4_095A:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0965:
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_4_096A:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_09D0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_4_0600
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_4_0A5D
        je     NEAR PTR loc_4_0B2F
        cmp    eax, 1010eh
        je     SHORT loc_4_0A6F
        cmp    eax, 1010fh
        je     SHORT loc_4_0AA9
        cmp    eax, 10112h
        je     NEAR PTR loc_4_0B1B
loc_4_0A5D:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0B6A
loc_4_0A6F:
        cmp    edi, 4
        je     SHORT loc_4_0A86
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0B6A
loc_4_0A86:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A92:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0B90
        jmp    NEAR PTR loc_4_0B68
loc_4_0AA9:
        cmp    edi, 4
        je     SHORT loc_4_0AC0
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0B6A
loc_4_0AC0:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0ACC:
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_4_0B14
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 7fh]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_4_0AEE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 7fh], eax
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0B6A
loc_4_0AEE:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx + 21h]
        jne    SHORT loc_4_0B0C
        mov    eax, dword ptr [ecx + 21h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_4_0B0C
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 21h], eax
        call   sub_4_0DE0
        jmp    SHORT loc_4_0B6A
loc_4_0B0C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0B6A
loc_4_0B14:
        mov    ebx, 0c0000001h
        jmp    SHORT loc_4_0B6A
loc_4_0B1B:
        cmp    edi, 4
        je     SHORT loc_4_0B6A
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0B6A
loc_4_0B2F:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_4_0B4D
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0B6A
loc_4_0B4D:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_4_0BF0
loc_4_0B68:
        mov    ebx, eax
loc_4_0B6A:
        test   ebx, ebx
        jne    SHORT loc_4_0B78
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_4_0B78:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B90:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0BD2
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0BC8
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 1dh]
        push   eax
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0BC1:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    10h
loc_4_0BC8:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_4_0BD2:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0BF0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0C36
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0C2C
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [esi + 1dh]
        push   edx
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C25:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    14h
loc_4_0C2C:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_4_0C36:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    14h
        db  08Dh, 049h, 000h
sub_4_0C50:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        mov    dword ptr [ebp - 10h], ecx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 0ch]
        lea    ebx, [ebp - 10h]
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        push   ebx
        mov    edx, dword ptr [esi + 14h]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        mov    dword ptr [ebp - 4], ecx
        push   1
        push   edx
        push   ecx
        push   ebx
        call   sub_4_0600
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_4_0D25
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_4_0CB5
        je     SHORT loc_4_0CF4
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_4_0CC3
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0CB5:
        cmp    eax, 1020102h
        je     SHORT loc_4_0CFC
        cmp    eax, 1020103h
        je     SHORT loc_4_0D04
loc_4_0CC3:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    SHORT loc_4_0D0D
loc_4_0CCC:
        mov    eax, dword ptr [ebx + 83h]
        jmp    SHORT loc_4_0D0A
loc_4_0CD4:
        mov    eax, dword ptr [ebx + 87h]
        jmp    SHORT loc_4_0D0A
loc_4_0CDC:
        mov    eax, dword ptr [ebx + 8bh]
        jmp    SHORT loc_4_0D0A
loc_4_0CE4:
        mov    eax, dword ptr [ebx + 9bh]
        jmp    SHORT loc_4_0D0A
loc_4_0CEC:
        mov    eax, dword ptr [ebx + 9fh]
        jmp    SHORT loc_4_0D0A
loc_4_0CF4:
        mov    eax, dword ptr [ebx + 97h]
        jmp    SHORT loc_4_0D0A
loc_4_0CFC:
        mov    eax, dword ptr [ebx + 8fh]
        jmp    SHORT loc_4_0D0A
loc_4_0D04:
        mov    eax, dword ptr [ebx + 93h]
loc_4_0D0A:
        mov    dword ptr [ebp - 4], eax
loc_4_0D0D:
        cmp    edi, 4
        jb     SHORT loc_4_0D25
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D21:
        add    dword ptr [ebp - 0ch], 4
loc_4_0D25:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
loc_4_0D60:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [ecx + 14h]
        cmp    edx, 10113h
        ja     SHORT loc_4_0D88
        je     SHORT loc_4_0DBF
        cmp    edx, 10101h
        jb     SHORT loc_4_0DBA
        cmp    edx, 10111h
        jbe    SHORT loc_4_0DBF
        jmp    SHORT loc_4_0DBA
loc_4_0D88:
        cmp    edx, 1010104h
        ja     SHORT loc_4_0DAA
        cmp    edx, 1010101h
        jae    SHORT loc_4_0DBF
        cmp    edx, 20101h
        jb     SHORT loc_4_0DBA
        cmp    edx, 20105h
        jbe    SHORT loc_4_0DBF
        jmp    SHORT loc_4_0DBA
loc_4_0DAA:
        cmp    edx, 1020101h
        jb     SHORT loc_4_0DBA
        cmp    edx, 1020103h
        jbe    SHORT loc_4_0DBF
loc_4_0DBA:
        mov    eax, 0c0010017h
loc_4_0DBF:
        test   eax, eax
        jne    SHORT loc_4_0DCD
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0C50
loc_4_0DCD:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0DE0:
        push   ebp
        mov    ecx, 0eeh
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 1c5h]
        test   edx, edx
        je     SHORT loc_4_0E06
loc_4_0DF6:
        mov    esi, dword ptr [edx + 21h]
        cmp    ecx, esi
        jae    SHORT loc_4_0DFF
        mov    ecx, esi
loc_4_0DFF:
        mov    edx, dword ptr [edx + 11h]
        test   edx, edx
        jne    SHORT loc_4_0DF6
loc_4_0E06:
        pop    esi
        mov    dword ptr [eax + 7fh], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0E10:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        lea    ecx, [ebp - 14h]
        push   esi
        lea    edx, [ebp - 10h]
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   edx
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E37:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     SHORT loc_4_0E4E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E4E:
        mov    eax, dword ptr [esi + 47h]
        test   eax, eax
        je     SHORT loc_4_0E79
        test   al, 0ah
        je     SHORT loc_4_0E67
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E67:
        test   al, 1
        je     SHORT loc_4_0E79
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E79:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_0EA0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1030
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0EA0:
        push   esi
        call   sub_4_19F0
        test   al, al
        jne    SHORT loc_4_0ECA
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1030
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 46h], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0ECA:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EDB:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_34E0
        test   eax, eax
        je     SHORT loc_4_0F29
        jle    SHORT loc_4_0F17
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1030
        push   esi
        call   sub_4_1A20
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F17:
        test   eax, eax
        jge    SHORT loc_4_0F29
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F29:
        xor    edi, edi
        xor    eax, eax
loc_4_0F2D:
        cmp    eax, dword ptr [ebp - 8]
        jae    SHORT loc_4_0F47
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0b6h], cl
        jb     SHORT loc_4_0F2D
loc_4_0F47:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        je     SHORT loc_4_0FAE
loc_4_0F4F:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F5C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F6D:
        xor    ecx, ecx
        cmp    edi, 6
        jae    SHORT loc_4_0F8E
loc_4_0F74:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_0F8E
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0b6h], al
        jb     SHORT loc_4_0F74
loc_4_0F8E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F9E:
        add    esp, 0ch
        test   eax, eax
        jl     SHORT loc_4_0FC8
        inc    ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, ebx
        ja     SHORT loc_4_0F4F
loc_4_0FAE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3600
        test   eax, eax
        jge    SHORT loc_4_0FD6
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FC8:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FD6:
        lea    eax, [esi + 0b7h]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FE6:
        test   al, al
        je     SHORT loc_4_100C
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1070
        push   esi
        call   sub_4_1A20
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_100C:
        push   esi
        inc    dword ptr [esi + 83h]
        call   sub_4_1A20
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_1770
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_1030:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_105C
        pop    ebp
        mov    dword ptr [eax + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
loc_4_105C:
        mov    ecx, dword ptr [eax + 28h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_1070:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 38h], 0
        jne    SHORT loc_4_1099
        pop    ebp
        mov    dword ptr [eax + 38h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
loc_4_1099:
        mov    ecx, dword ptr [eax + 34h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10B0:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 24h], 0
        mov    eax, dword ptr [ecx + 24h]
        test   eax, eax
        jne    SHORT loc_4_10D5
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [ecx + 28h], edx
        inc    dword ptr [ecx + 2ch]
        ret    8
loc_4_10D5:
        pop    ebp
        mov    dword ptr [edx + 24h], eax
        mov    dword ptr [ecx + 24h], edx
        inc    dword ptr [ecx + 2ch]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    al, byte ptr [ebp + 0ch]
        push   esi
        test   al, al
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0b3h], 0
        je     SHORT loc_4_1148
        test   al, 1
        je     SHORT loc_4_1114
        inc    dword ptr [esi + 30h]
loc_4_1114:
        test   al, 2
        je     SHORT loc_4_1121
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 93h]
loc_4_1121:
        test   al, 4
        je     SHORT loc_4_112E
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_112E:
        test   al, 8
        je     SHORT loc_4_113B
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_113B:
        test   al, 10h
        je     SHORT loc_4_1148
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_1148:
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_4_115C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_115C:
        mov    edi, dword ptr [esi + 24h]
        lea    ecx, [ebp - 18h]
        push   ecx
        lea    edx, [ebp - 0ch]
        mov    eax, dword ptr [edi + 24h]
        push   edx
        mov    dword ptr [esi + 24h], eax
        lea    eax, [ebp - 14h]
        push   eax
        dec    dword ptr [esi + 2ch]
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_117C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_118D:
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_34E0
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_11D7
        push   edi
        push   esi
        call   sub_4_10B0
        test   ebx, ebx
        jge    SHORT loc_4_11C9
        add    ebx, 2
        pop    edi
        pop    esi
        cmp    ebx, 1
        sbb    eax, eax
        pop    ebx
        and    eax, 0fffffffch
        mov    esp, ebp
        add    eax, 2
        pop    ebp
        ret    8
loc_4_11C9:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_11D7:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_11DB:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_11F5
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0b6h], al
        jb     SHORT loc_4_11DB
loc_4_11F5:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 14h]
        dec    eax
        je     SHORT loc_4_1265
loc_4_1202:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_120F:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1220:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    SHORT loc_4_1241
loc_4_1227:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_1241
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0b6h], al
        jb     SHORT loc_4_1227
loc_4_1241:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1251:
        add    esp, 0ch
        cmp    eax, -2
        je     SHORT loc_4_1287
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_4_1202
loc_4_1265:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3600
        cmp    eax, -2
        jne    SHORT loc_4_129C
        push   edi
        push   esi
        call   sub_4_10B0
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1287:
        push   edi
        push   esi
        call   sub_4_10B0
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_129C:
        lea    eax, [esi + 0b7h]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12AC:
        test   al, al
        je     SHORT loc_4_12CC
        push   edi
        mov    eax, dword ptr [edi + 28h]
        push   eax
        push   esi
        call   sub_4_1070
        push   esi
        call   sub_4_3E00
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_12CC:
        push   0
        inc    dword ptr [esi + 83h]
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12E1:
        push   esi
        call   sub_4_1770
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1300:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_4_1325:
        test   edx, edx
        je     SHORT loc_4_1334
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_4_1325
        mov    dword ptr [ecx], edx
loc_4_1334:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_4_1347
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_1347:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   25h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_135A:
        test   eax, eax
        je     SHORT loc_4_136C
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_136C:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        add    eax, 1dh
        mov    esi, dword ptr [ebp + 20h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1387:
        test   al, al
        jne    SHORT loc_4_13A6
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   25h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1398:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_13A6:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_13C8
        mov    eax, dword ptr [esi + 1cdh]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 103h
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1cdh], ecx
        jmp    SHORT loc_4_13DF
loc_4_13C8:
        mov    eax, dword ptr [esi + 1c5h]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1c5h], ecx
loc_4_13DF:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0dh], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 1c1h]
        mov    dword ptr [eax + 19h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax + 5], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 9], edx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 7fh]
        mov    dword ptr [eax + 21h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [esi + 1c9h], 0
        jne    SHORT loc_4_1432
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
loc_4_1432:
        mov    eax, ebx
        pop    edi
        inc    dword ptr [esi + 1c9h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1450:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        xor    eax, eax
loc_4_145F:
        mov    dl, byte ptr [esi + eax + 67h]
        mov    ecx, dword ptr [esi + 73h]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     SHORT loc_4_145F
        mov    edi, dword ptr [esi + 73h]
        add    edi, 6
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_1497
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 10h]
loc_4_1482:
        xor    ecx, ecx
loc_4_1484:
        mov    dl, byte ptr [eax + ecx]
        inc    edi
        inc    ecx
        mov    byte ptr [edi - 1], dl
        cmp    ecx, 6
        jb     SHORT loc_4_1484
        add    eax, 6
        dec    ebx
        jne    SHORT loc_4_1482
loc_4_1497:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 77h], eax
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_14B0
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 44h], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_14B0:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14BF:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E70
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2F20
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3250
        test   eax, eax
        jge    SHORT loc_4_14FE
        mov    eax, 0c0000001h
        pop    edi
        or     dword ptr [esi + 47h], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_14FE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  090h
loc_4_1510:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax + 0dh]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 0ffffffd0h
        je     SHORT loc_4_1530
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_1530:
        test   al, 1
        je     SHORT loc_4_1539
        mov    ecx, 1
loc_4_1539:
        test   al, 2
        je     SHORT loc_4_1540
        or     ecx, 4
loc_4_1540:
        test   al, 4
        je     SHORT loc_4_1547
        or     ecx, 8
loc_4_1547:
        test   al, 8
        je     SHORT loc_4_154E
        or     ecx, 2
loc_4_154E:
        test   al, 20h
        je     SHORT loc_4_1555
        or     ecx, 10h
loc_4_1555:
        test   dword ptr [esi + 1bdh], ecx
        je     SHORT loc_4_1567
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_1567:
        mov    dword ptr [esi + 1b9h], ecx
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_157E
        mov    byte ptr [esi + 43h], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_4_157E:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_158D:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 1b9h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3250
        test   eax, eax
        jge    SHORT loc_4_15B6
        or     dword ptr [esi + 47h], 2
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_4_15B6:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0dh]
        push   1
        add    ecx, 1d1h
        push   ecx
        mov    edx, dword ptr [ecx + 1dh]
        mov    dword ptr [eax + 15h], edx
        mov    dword ptr [ecx + 1dh], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_15F1:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1600:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 1eeh]
        test   ecx, ecx
        je     SHORT loc_4_1630
loc_4_1611:
        mov    eax, dword ptr [esi + 1eeh]
        push   ecx
        mov    edx, dword ptr [eax + 15h]
        mov    dword ptr [esi + 1eeh], edx
        call   sub_4_1770
        mov    ecx, dword ptr [esi + 1eeh]
        test   ecx, ecx
        jne    SHORT loc_4_1611
loc_4_1630:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1640:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 47h], 8
        jne    SHORT loc_4_1676
        push   esi
        call   sub_4_19F0
        test   al, al
        je     SHORT loc_4_1665
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3690
        push   esi
        call   sub_4_1A20
loc_4_1665:
        push   3e8h
        add    esi, 1ddh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1676:
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_4_1680:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 4], 0
        mov    esi, dword ptr [edi + 0dh]
        je     SHORT loc_4_169E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_169E:
        inc    dword ptr [edi]
        mov    eax, dword ptr [esi + 1c5h]
        cmp    edi, eax
        jne    SHORT loc_4_16B5
        mov    eax, dword ptr [edi + 11h]
        mov    dword ptr [esi + 1c5h], eax
        jmp    SHORT loc_4_16C8
loc_4_16B5:
        cmp    dword ptr [eax + 11h], edi
        je     SHORT loc_4_16C2
loc_4_16BA:
        mov    eax, dword ptr [eax + 11h]
        cmp    dword ptr [eax + 11h], edi
        jne    SHORT loc_4_16BA
loc_4_16C2:
        mov    ecx, dword ptr [edi + 11h]
        mov    dword ptr [eax + 11h], ecx
loc_4_16C8:
        push   0
        mov    eax, dword ptr [edi + 1dh]
        push   eax
        mov    ecx, dword ptr [esi + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16D7:
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_16EF
        cmp    ebx, 103h
        je     SHORT loc_4_1703
        cmp    ebx, 0c001000eh
        je     SHORT loc_4_170C
        jmp    SHORT loc_4_1715
loc_4_16EF:
        mov    eax, dword ptr [edi]
        sub    eax, 2
        mov    dword ptr [edi], eax
        je     SHORT loc_4_1717
        mov    byte ptr [edi + 4], 1
        mov    ebx, 103h
        jmp    SHORT loc_4_1717
loc_4_1703:
        mov    byte ptr [edi + 4], 1
        sub    dword ptr [edi], 2
        jmp    SHORT loc_4_1717
loc_4_170C:
        mov    byte ptr [edi + 4], 1
        mov    ebx, 103h
loc_4_1715:
        dec    dword ptr [edi]
loc_4_1717:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_4_175B
        mov    eax, dword ptr [edi + 21h]
        cmp    dword ptr [esi + 7fh], eax
        jne    SHORT loc_4_172A
        push   esi
        call   sub_4_0DE0
loc_4_172A:
        cmp    dword ptr [esi + 1c5h], 0
        jne    SHORT loc_4_1742
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1742:
        push   0
        dec    dword ptr [esi + 1c9h]
        push   25h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1752:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_175B:
        mov    ebx, 103h
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1770:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    SHORT loc_4_17C3
        mov    eax, dword ptr [esi + 21h]
        cmp    dword ptr [edi + 7fh], eax
        jne    SHORT loc_4_1790
        push   edi
        call   sub_4_0DE0
loc_4_1790:
        cmp    dword ptr [edi + 1c5h], 0
        jne    SHORT loc_4_17A8
        push   edi
        lea    eax, [edi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17A8:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17B3:
        push   0
        push   25h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17BD:
        dec    dword ptr [edi + 1c9h]
loc_4_17C3:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_17D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        cmp    byte ptr [eax + 4], 0
        je     SHORT loc_4_17ED
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17ED:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_17FE
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17FE:
        push   40010004h
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 1eah], eax
        call   sub_4_1990
        or     dword ptr [esi + 47h], 1
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        je     SHORT loc_4_1847
loc_4_181C:
        mov    eax, dword ptr [ecx + 24h]
        push   0c001000ch
        push   ecx
        mov    dword ptr [esi + 24h], eax
        dec    dword ptr [esi + 2ch]
        inc    dword ptr [esi + 30h]
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_183A:
        push   edi
        call   sub_4_1770
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        jne    SHORT loc_4_181C
loc_4_1847:
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_1873
loc_4_184E:
        mov    eax, dword ptr [ecx + 24h]
        push   0c001000ch
        push   ecx
        mov    dword ptr [esi + 38h], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1866:
        push   edi
        call   sub_4_1770
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        jne    SHORT loc_4_184E
loc_4_1873:
        mov    byte ptr [esi + 1ffh], 1
        push   esi
        call   sub_4_19F0
        test   al, al
        je     SHORT loc_4_189B
        push   esi
        call   sub_4_18B0
        push   esi
        call   sub_4_1A20
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_189B:
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 1ffh], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_18B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 1eah]
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 1ffh], 0
        mov    byte ptr [esi + 42h], 1
        and    dword ptr [esi + 47h], 0fffffffdh
        push   eax
        call   sub_4_2C70
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E70
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2F20
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3250
        test   eax, eax
        jge    SHORT loc_4_1900
        or     dword ptr [esi + 47h], 2
loc_4_1900:
        push   40010005h
        push   esi
        call   sub_4_1990
        mov    eax, dword ptr [esi + 47h]
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 47h], eax
        test   al, 2
        je     SHORT loc_4_191F
        push   80010004h
        jmp    SHORT loc_4_1921
loc_4_191F:
        push   0
loc_4_1921:
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_192A:
        push   edi
        call   sub_4_1770
        push   esi
        call   sub_4_1940
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1940:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 1cdh]
        test   ecx, ecx
        je     SHORT loc_4_1986
loc_4_1951:
        mov    eax, dword ptr [esi + 1cdh]
        push   0
        push   0
        mov    edx, dword ptr [eax + 11h]
        mov    eax, dword ptr [esi + 1c5h]
        mov    dword ptr [esi + 1cdh], edx
        mov    dword ptr [ecx + 11h], eax
        mov    dword ptr [esi + 1c5h], ecx
        mov    ecx, dword ptr [ecx + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_197C:
        mov    ecx, dword ptr [esi + 1cdh]
        test   ecx, ecx
        jne    SHORT loc_4_1951
loc_4_1986:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1990:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 1c5h]
        test   edi, edi
        je     SHORT loc_4_19DB
        mov    esi, dword ptr [ebp + 0ch]
loc_4_19AA:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi]
        mov    ebx, dword ptr [edi + 11h]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19C6:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19CF:
        push   edi
        call   sub_4_1770
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_4_19AA
loc_4_19DB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_4_1A03
        xor    al, al
        pop    ebp
        ret    4
loc_4_1A03:
        mov    byte ptr [eax + 40h], 1
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A16:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1A20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 38h], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 46h]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 41h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 1ffh]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 43h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 44h]
        or     ecx, eax
        je     NEAR PTR loc_4_1B4E
loc_4_1A5D:
        cmp    byte ptr [esi + 1ffh], 0
        je     SHORT loc_4_1A6E
        mov    bl, 1
        push   esi
        call   sub_4_18B0
loc_4_1A6E:
        cmp    byte ptr [esi + 44h], 0
        je     SHORT loc_4_1AB1
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E70
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2F20
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3250
        test   eax, eax
        jge    SHORT loc_4_1AAF
        or     dword ptr [esi + 47h], 2
loc_4_1AAF:
        mov    bl, 1
loc_4_1AB1:
        cmp    byte ptr [esi + 43h], 0
        je     SHORT loc_4_1ADC
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 1b9h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3250
        test   eax, eax
        jge    SHORT loc_4_1ADA
        or     dword ptr [esi + 47h], 2
loc_4_1ADA:
        mov    bl, 1
loc_4_1ADC:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_1AFA
        mov    byte ptr [esi + 46h], 0
loc_4_1AE6:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_1AF8
        push   0
        push   esi
        call   sub_4_10F0
        test   eax, eax
        je     SHORT loc_4_1AE6
loc_4_1AF8:
        mov    bl, 1
loc_4_1AFA:
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_4_1B08
        push   esi
        mov    bl, 1
        call   sub_4_3E00
loc_4_1B08:
        cmp    byte ptr [esi + 41h], 0
        je     SHORT loc_4_1B1B
        mov    byte ptr [esi + 41h], 0
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B1B:
        cmp    dword ptr [esi + 38h], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 46h]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 41h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 1ffh]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 43h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 44h]
        or     ecx, eax
        jne    NEAR PTR loc_4_1A5D
loc_4_1B4E:
        mov    byte ptr [esi + 40h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1B70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3B40
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 0F1h, 01Fh, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1BB0:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 0ch], ax
        push   ebx
        mov    word ptr [ebp - 0ah], ax
        xor    ebx, ebx
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 00Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1BE0:
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        db  089h, 015h, 046h, 000h, 000h, 000h
        db  089h, 00Dh, 04Ah, 000h, 000h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 01Dh, 052h, 000h, 000h, 000h
        call   sub_4_23B0
        db  0A3h, 04Eh, 000h, 000h, 000h
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 003h
        db  088h, 01Dh, 011h, 000h, 000h, 000h
        db  089h, 01Dh, 012h, 000h, 000h, 000h
        db  0C7h, 005h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   36h
        db  089h, 015h, 03Eh, 000h, 000h, 000h
        db  068h, 010h, 000h, 000h, 000h
        lea    edx, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 042h, 000h, 000h, 000h
        db  0A1h, 00Ch, 000h, 000h, 000h
        push   eax
        db  068h, 008h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CB3:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_1CE0
        db  039h, 01Dh, 004h, 000h, 000h, 000h
        jbe    SHORT loc_4_1CC9
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1CC9:
        push   0
        db  0A1h, 00Ch, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CD6:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1CE0:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1CF0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   225h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D0E:
        test   eax, eax
        je     SHORT loc_4_1D1F
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1D1F:
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_04D0
        test   eax, eax
        je     SHORT loc_4_1D51
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   225h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D44:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1D51:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_1D90
        test   eax, eax
        jne    SHORT loc_4_1D6A
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1D6A:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   225h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D7A:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_2240
        push   esi
        call   sub_4_2130
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_4_209E
        lea    edi, [ebp - 48h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 48h], 0
        mov    eax, dword ptr [esi + 209h]
        cmp    eax, -1
        je     SHORT loc_4_1DD6
        mov    dword ptr [ebp - 48h], eax
loc_4_1DD6:
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 205h]
        mov    dword ptr [ebp - 3ah], ebx
        mov    dword ptr [ebp - 2eh], eax
        mov    dword ptr [ebp - 3eh], 1
        lea    ecx, [ebp - 48h]
        mov    edx, dword ptr [ebp + 10h]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        push   eax
        lea    edx, [esi + 201h]
        mov    dword ptr [ebp - 36h], 600h
        mov    dword ptr [ebp - 32h], 1
        mov    dword ptr [ebp - 2ah], 20h
        push   esi
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E20:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_4_2098
        mov    eax, dword ptr [esi + 209h]
        cmp    eax, -1
        je     SHORT loc_4_1EA1
        lea    ecx, [ebp - 20h]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], ebx
        mov    dword ptr [ecx + 0ch], ebx
        push   600h
        mov    word ptr [ecx + 10h], bx
        push   ecx
        mov    byte ptr [ecx + 12h], bl
        lea    edx, [esi + 4bh]
        mov    byte ptr [ebp - 20h], 1
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 15h], eax
        mov    dword ptr [ebp - 1dh], 2
        mov    dword ptr [ebp - 19h], 3
        mov    eax, dword ptr [esi + 201h]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E7A:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_1EA1
        mov    eax, dword ptr [esi + 209h]
        mov    ecx, dword ptr [esi + 201h]
        push   eax
        push   1
        push   0aa000001h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E99:
        add    esp, 10h
        jmp    NEAR PTR loc_4_208C
loc_4_1EA1:
        mov    edx, dword ptr [esi + 21dh]
        mov    ecx, dword ptr [esi + 205h]
        lea    eax, [ebp - 0ch]
        mov    ebx, dword ptr [esi + 209h]
        push   eax
        lea    edi, [edx + 800h]
        push   ebx
        lea    eax, [ecx + 20h]
        push   edi
        push   edx
        push   eax
        mov    edx, dword ptr [esi + 73h]
        push   ecx
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 211h]
        push   ecx
        push   edx
        push   esi
        push   eax
        call   sub_4_2B80
        test   eax, eax
        jge    SHORT loc_4_1F0D
        mov    eax, dword ptr [esi + 205h]
        mov    ecx, dword ptr [esi + 211h]
        push   eax
        mov    edx, dword ptr [esi + 201h]
        push   ecx
        push   2
        push   0aa000004h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EFE:
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 14h
        jmp    NEAR PTR loc_4_207A
loc_4_1F0D:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 20h], eax
loc_4_1F1E:
        lea    eax, [esi + ecx]
        mov    edx, dword ptr [esi + 73h]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 6dh], dl
        mov    byte ptr [eax + 67h], dl
        jb     SHORT loc_4_1F1E
        lea    ebx, [esi + 7bh]
        lea    eax, [esi + 4]
        push   ebx
        lea    ecx, [esi + 67h]
        push   eax
        mov    edx, dword ptr [esi + 20dh]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F5B:
        test   al, al
        jne    SHORT loc_4_1F6B
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_4_207A
loc_4_1F6B:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2E70
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2F20
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3770
        mov    edi, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F9A:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_1FAD
        mov    dword ptr [ebp - 4], 0c000009ah
        jmp    NEAR PTR loc_4_2072
loc_4_1FAD:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3790
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   edi
        push   ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 0a3h], edx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 0a7h], eax
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0abh], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FDF:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3BE0
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 1bdh], eax
        mov    dword ptr [esi + 1b9h], 0
        call   sub_4_3250
        test   eax, eax
        jge    SHORT loc_4_200F
        or     dword ptr [esi + 47h], 2
        jmp    SHORT loc_4_2072
loc_4_200F:
        lea    edx, [esi + 8]
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 6
        rep stosd
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esi + 211h]
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [esi + 201h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2047:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_20AA
        mov    eax, dword ptr [esi + 211h]
        mov    ecx, dword ptr [esi + 201h]
        push   eax
        push   1
        push   0aa000005h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2067:
        add    esp, 10h
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2D70
loc_4_2072:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_207A:
        cmp    dword ptr [esi + 209h], -1
        je     SHORT loc_4_208C
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_208C:
        mov    eax, dword ptr [esi + 201h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2098:
        push   esi
        call   sub_4_2200
loc_4_209E:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_20AA:
        db  0C7h, 086h, 0C1h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 45h], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 1c9h], ecx
        mov    dword ptr [esi + 1c5h], ecx
        mov    dword ptr [esi + 1cdh], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 221h], eax
        lea    eax, [esi + 1ddh]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 1d1h], ecx
        mov    dword ptr [esi + 1d9h], esi
        push   3e8h
        mov    dword ptr [eax], ecx
        push   eax
        mov    dword ptr [esi + 1e5h], esi
        db  0C7h, 086h, 0D5h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0E1h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2116:
        mov    ecx, dword ptr [esi + 1c1h]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2130:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        db  08Bh, 00Dh, 04Eh, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   3
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2151:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_2166
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2166:
        lea    ebx, [esi + 73h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi + 20dh]
        push   0
        lea    eax, [ecx + ecx*2 + 3]
        add    eax, eax
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2184:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_21D5
        mov    edx, dword ptr [esi + 20dh]
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        lea    edx, [edx + edx*2 + 3]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        push   2
        and    ecx, 3
        rep stosb
        lea    ecx, [ebp - 4]
        lea    eax, [esi + 4fh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21B8:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_21F3
        push   0
        mov    eax, dword ptr [esi + 20dh]
        mov    ecx, dword ptr [ebx]
        lea    eax, [eax + eax*2 + 3]
        add    eax, eax
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21D5:
        push   3
        db  0A1h, 04Eh, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21E5:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_21F3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_2200:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2210:
        push   3
        db  0A1h, 04Eh, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2220:
        push   0
        mov    ecx, dword ptr [esi + 20dh]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 73h]
        add    eax, eax
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2238:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_2240:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 1c9h], ecx
        mov    byte ptr [eax + 40h], cl
        mov    byte ptr [eax + 44h], cl
        mov    byte ptr [eax + 1feh], cl
        mov    byte ptr [eax + 45h], cl
        mov    byte ptr [eax + 1e9h], cl
        mov    dword ptr [eax + 47h], ecx
        mov    dword ptr [eax + 1b9h], ecx
        mov    dword ptr [eax + 77h], ecx
        mov    byte ptr [eax + 43h], cl
        mov    byte ptr [eax + 1ffh], cl
        mov    byte ptr [eax + 200h], cl
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 30h], ecx
        mov    byte ptr [eax + 46h], cl
        mov    dword ptr [eax + 34h], ecx
        pop    ebp
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [eax + 7fh], 0eeh
        mov    dword ptr [eax + 3ch], ecx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_22B0:
        push   ebp
        db  0A1h, 008h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22C5:
        push   0
        db  08Bh, 00Dh, 00Ch, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22D3:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_22E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   4c4b40h
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 47h], 8
        lea    edi, [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22F9:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2305:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_230B:
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_2320
        mov    eax, dword ptr [esi + 221h]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_234E
loc_4_2320:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 221h], esi
        je     SHORT loc_4_233C
loc_4_232E:
        mov    ecx, dword ptr [ecx + 221h]
        cmp    dword ptr [ecx + 221h], esi
        jne    SHORT loc_4_232E
loc_4_233C:
        mov    eax, dword ptr [ecx + 221h]
        mov    edx, dword ptr [eax + 221h]
        mov    dword ptr [ecx + 221h], edx
loc_4_234E:
        cmp    dword ptr [esi + 209h], -1
        je     SHORT loc_4_2360
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2360:
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2369:
        mov    eax, dword ptr [esi + 201h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2375:
        push   esi
        call   sub_4_2200
        push   0
        push   225h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2388:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_2390:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2C70
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_23B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_23CA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_4_23CA:
        mov    eax, 1f2h
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_23E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    eax, dword ptr [esi + 40h]
        mov    word ptr [ebp - 2], di
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        cmp    al, 50h
        jne    NEAR PTR loc_4_24A5
        mov    eax, dword ptr [esi + 40h]
        mov    ecx, dword ptr [esi + 4]
        inc    eax
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 48h
        jne    NEAR PTR loc_4_24A5
        mov    eax, dword ptr [esi + 40h]
        mov    ecx, dword ptr [esi + 4]
        add    eax, 2
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0f0h
        jne    SHORT loc_4_24A5
        mov    eax, dword ptr [esi + 40h]
        mov    ecx, dword ptr [esi + 4]
        add    eax, 3
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 53h
        jne    SHORT loc_4_24A5
        push   1
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ebx, dword ptr [ebp + 0ch]
loc_4_2457:
        mov    eax, dword ptr [esi + 60h]
        mov    ecx, dword ptr [esi + 4]
        add    eax, edi
        inc    ebx
        push   eax
        inc    edi
        push   ecx
        call   sub_4_0260
        mov    cl, al
        mov    byte ptr [ebx - 1], al
        and    ecx, 0ffff00ffh
        add    word ptr [ebp - 2], cx
        cmp    edi, 6
        jl     SHORT loc_4_2457
        mov    eax, dword ptr [esi + 78h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    cl, al
        pop    edi
        mov    ax, word ptr [ebp - 2]
        and    ecx, 0ffff00ffh
        add    ax, cx
        pop    esi
        and    ax, 0ffh
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_24A5:
        mov    ax, 0ff00h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_24C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        jne    SHORT loc_4_24DE
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_24DE:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    word ptr [ebp - 2], ax
        test   al, 40h
        jne    SHORT loc_4_2524
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 2]
        push   eax
        push   ebx
        xor    edx, edx
        mov    dx, word ptr [esi]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_2524:
        mov    ecx, dword ptr [esi + 50h]
        mov    edx, dword ptr [esi + 4]
        and    word ptr [ebp - 2], 0ffe3h
        or     word ptr [ebp - 2], 1023h
        mov    eax, dword ptr [ebp - 2]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0290
        push   2
        mov    ecx, dword ptr [esi + 48h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 68h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0270
        and    eax, 0ffh
        mov    ecx, dword ptr [esi + 4]
        mov    edi, eax
        mov    eax, dword ptr [esi + 60h]
        shl    edi, 10h
        push   eax
        push   ecx
        call   sub_4_0270
        and    eax, 0ff00h
        or     edi, eax
        cmp    ebx, edi
        je     SHORT loc_4_25AA
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        push   edi
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_25AA:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, eax
        sub    ecx, ebx
        cmp    ecx, 7ffh
        jae    SHORT loc_4_25E4
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        push   ebx
        xor    eax, eax
        mov    edx, dword ptr [esi + 4]
        mov    ax, word ptr [esi + 2]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_25E4:
        push   800h
        mov    eax, dword ptr [esi + 4]
        push   edi
        push   eax
        call   sub_4_0020
        mov    dword ptr [esi + 20h], eax
        test   eax, eax
        jne    SHORT loc_4_2625
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        push   edi
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_2625:
        push   0
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 48h]
        push   eax
        push   ecx
        call   sub_4_0290
        xor    bx, bx
        push   0
        mov    ecx, dword ptr [esi + 60h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_2649:
        push   ebx
        mov    eax, dword ptr [esi + 70h]
        inc    bx
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        cmp    bx, 400h
        jb     SHORT loc_4_2649
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    edx, dword ptr [esi + 50h]
        mov    eax, dword ptr [esi + 4]
        and    word ptr [ebp - 2], 0efffh
        xor    bx, bx
        mov    ecx, dword ptr [ebp - 2]
        push   ecx
        push   edx
        push   eax
        call   sub_4_0290
loc_4_2689:
        mov    cx, bx
        mov    eax, dword ptr [ebp - 8]
        and    ecx, 1
        cmp    word ptr [eax + ecx*2], bx
        jne    SHORT loc_4_26A1
        inc    bx
        cmp    bx, 400h
        jb     SHORT loc_4_2689
loc_4_26A1:
        mov    ecx, dword ptr [esi + 50h]
        mov    edx, dword ptr [esi + 4]
        or     byte ptr [ebp - 1], 10h
        mov    eax, dword ptr [ebp - 2]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0290
        cmp    bx, 400h
        jae    SHORT loc_4_26F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, ebx
        push   eax
        and    ecx, 0ffffh
        push   ecx
        xor    edx, edx
        mov    dx, word ptr [esi + 2]
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_26F3:
        mov    ax, 3
        db  08Bh, 015h, 00Ch, 000h, 000h, 000h
        shr    edi, 8
        add    edx, 16h
loc_4_2703:
        shr    edi, 4
        dec    edx
        mov    ecx, edi
        and    ecx, 0fh
        dec    ax
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        mov    byte ptr [edx + 1], cl
        jne    SHORT loc_4_2703
        mov    eax, 3
        pop    edi
        or     byte ptr [esi + 8], 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_4_2730:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        test   al, 1fh
        push   ebx
        push   esi
        push   edi
        jne    NEAR PTR loc_4_2930
        cmp    eax, 100h
        jb     NEAR PTR loc_4_2930
        cmp    eax, 3c0h
        ja     NEAR PTR loc_4_2930
        mov    ecx, dword ptr [ebp + 18h]
        sub    ecx, eax
        cmp    ecx, 1fh
        jb     NEAR PTR loc_4_2930
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   edi
        push   esi
        call   sub_4_23E0
        cmp    ax, 0ffh
        je     SHORT loc_4_278F
        mov    cx, 0ffh
        pop    edi
        cmp    cx, ax
        pop    esi
        sbb    eax, eax
        pop    ebx
        and    eax, 0fffffffbh
        pop    ebp
        add    eax, 9
        ret    1ch
loc_4_278F:
        push   6
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 74h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    byte ptr [esi + 0ah], al
        test   al, 0f1h
        je     SHORT loc_4_27BE
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_27BE:
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    ax, 0fff0h
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        push   3
        mov    ecx, dword ptr [esi + 48h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 60h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   1
        mov    ecx, dword ptr [esi + 60h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        mov    ebx, 4e20h
        call   sub_4_0290
loc_4_280F:
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    SHORT loc_4_280F
        push   edi
        push   esi
        call   sub_4_23E0
        cmp    ax, 0ffh
        je     SHORT loc_4_2836
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_2836:
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    ax, 0ffe3h
        mov    ecx, dword ptr [esi + 50h]
        or     ax, 1023h
        mov    edx, dword ptr [esi + 4]
        push   eax
        mov    edi, 320h
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_285E:
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    SHORT loc_4_285E
        mov    eax, dword ptr [ebp + 20h]
        mov    ecx, dword ptr [ebp + 1ch]
        push   eax
        push   ecx
        push   esi
        call   sub_4_24C0
        mov    edi, eax
        mov    ebx, dword ptr [ebp + 10h]
        cmp    ebx, 3
        jb     NEAR PTR loc_4_2924
        cmp    ebx, 8
        je     NEAR PTR loc_4_2924
        cmp    ebx, 0dh
        je     NEAR PTR loc_4_2924
        cmp    ebx, 0eh
        je     NEAR PTR loc_4_2924
        cmp    ebx, 0fh
        ja     SHORT loc_4_2924
        push   2
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 74h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    eax, 0fh
        cmp    eax, ebx
        je     SHORT loc_4_28D7
        mov    eax, 7
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_28D7:
        push   0
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    byte ptr [esi + 0eh], 20h
        mov    dword ptr [esi + 1ceh], 100h
        mov    dword ptr [esi + 1c6h], 2000h
        mov    dword ptr [esi + 1cah], 6000h
        mov    eax, 80h
        or     byte ptr [esi + 8], 2
        mov    word ptr [esi + 0c0h], ax
        mov    byte ptr [esi + 0fh], al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_2924:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_2930:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h
sub_4_2940:
        push   ebp
        db  0B8h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    ecx, dword ptr [ebp + 14h]
        push   esi
        xor    bl, bl
        push   edi
loc_4_2953:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_4_2963
        add    eax, 4
        db  03Dh, 01Ch, 000h, 000h, 000h
        jb     SHORT loc_4_2953
        jmp    SHORT loc_4_2965
loc_4_2963:
        mov    bl, 1
loc_4_2965:
        xor    edx, edx
        test   bl, bl
        je     NEAR PTR loc_4_2B6B
        mov    eax, dword ptr [ebp + 18h]
        sub    eax, ecx
        cmp    eax, 1fh
        jb     NEAR PTR loc_4_2B6B
        mov    word ptr [ebp - 2], dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, esi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], edx
loc_4_298C:
        mov    eax, dword ptr [ebx + 40h]
        mov    ecx, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        inc    edi
        push   eax
        push   ecx
        call   sub_4_0260
        mov    cl, al
        mov    byte ptr [edi - 1], al
        and    ecx, 0ffff00ffh
        inc    dword ptr [ebp - 8]
        add    word ptr [ebp - 2], cx
        cmp    dword ptr [ebp - 8], 6
        jb     SHORT loc_4_298C
        mov    eax, dword ptr [ebx + 58h]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        and    ax, 0ffh
        mov    ecx, dword ptr [ebx + 4]
        add    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebx + 40h]
        add    eax, 0ah
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 1
        jne    SHORT loc_4_2A00
        mov    eax, dword ptr [ebx + 40h]
        mov    ecx, dword ptr [ebx + 4]
        add    eax, 0bh
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 4
        jne    SHORT loc_4_2A00
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2A00:
        cmp    byte ptr [ebp - 2], 0ffh
        je     SHORT loc_4_2A22
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 3
        repe cmpsb
        je     SHORT loc_4_2A22
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2A22:
        mov    eax, dword ptr [ebx + 5ch]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    byte ptr [ebx + 0ah], al
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 3
        jb     NEAR PTR loc_4_2B5D
        cmp    eax, 0fh
        ja     NEAR PTR loc_4_2B5D
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebx + 0bh], cl
        test   cl, cl
        je     NEAR PTR loc_4_2B5D
        cmp    eax, 0ah
        je     SHORT loc_4_2A62
        cmp    eax, 0bh
        jne    SHORT loc_4_2A75
loc_4_2A62:
        mov    al, byte ptr [ebx + 0ah]
        test   al, 80h
        je     NEAR PTR loc_4_2B4F
        test   al, 7
        je     NEAR PTR loc_4_2B4F
loc_4_2A75:
        cmp    byte ptr [ebx + 0ah], 0
        jne    SHORT loc_4_2A89
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2A89:
        push   0
        mov    eax, dword ptr [ebx + 60h]
        push   eax
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [ebx + 60h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    ecx, dword ptr [ebx + 60h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        push   1
        mov    ecx, dword ptr [ebx + 60h]
        push   ecx
        mov    edx, dword ptr [ebx + 4]
        push   edx
        call   sub_4_0280
        mov    al, byte ptr [ebx + 0ah]
        and    al, 80h
        cmp    al, 1
        sbb    eax, eax
        inc    ax
        mov    word ptr [ebx + 8], ax
        test   al, 1
        je     SHORT loc_4_2B13
        mov    byte ptr [ebx + 0eh], 40h
        mov    dword ptr [ebx + 1ceh], 100h
        mov    eax, 0ffh
        pop    edi
        mov    word ptr [ebx + 0c0h], ax
        pop    esi
        mov    dword ptr [ebx + 1c6h], 4000h
        mov    dword ptr [ebx + 1cah], 0c000h
        mov    byte ptr [ebx + 0fh], al
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2B13:
        mov    byte ptr [ebx + 0eh], 20h
        mov    dword ptr [ebx + 1ceh], 100h
        mov    eax, 80h
        pop    edi
        mov    word ptr [ebx + 0c0h], ax
        pop    esi
        mov    dword ptr [ebx + 1c6h], 2000h
        mov    dword ptr [ebx + 1cah], 6000h
        mov    byte ptr [ebx + 0fh], al
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2B4F:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2B5D:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_2B6B:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2B80:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 7ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        rep stosd
        stosw
        xor    ecx, ecx
        mov    edi, dword ptr [ebp + 18h]
        mov    word ptr [esi], cx
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 2], di
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 4], eax
        mov    eax, edx
        mov    dword ptr [esi + 10h], edx
        mov    dword ptr [eax], ecx
        mov    word ptr [eax + 4], cx
loc_4_2BB6:
        lea    eax, [ecx + edi]
        inc    ecx
        cmp    ecx, 20h
        mov    dword ptr [esi + ecx*4 + 3ch], eax
        jl     SHORT loc_4_2BB6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 24h]
        mov    ecx, dword ptr [ebp + 20h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   edx
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        push   eax
        push   ecx
        push   esi
        call   sub_4_2730
        cmp    eax, 4
        mov    ebx, eax
        jne    SHORT loc_4_2C0D
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edi
        push   ecx
        push   edx
        push   esi
        call   sub_4_2940
        cmp    eax, 5
        mov    ebx, eax
        jne    SHORT loc_4_2C0D
        mov    ebx, 4
loc_4_2C0D:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
        cmp    ebx, 4
        jge    SHORT loc_4_2C39
        test   ebx, ebx
        jl     SHORT loc_4_2C39
        mov    word ptr [esi], 2
        mov    ecx, dword ptr [ebp + 2ch]
        db  08Bh, 004h, 09Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    28h
loc_4_2C39:
        cmp    ebx, 0ah
        jae    SHORT loc_4_2C50
        test   ebx, ebx
        jl     SHORT loc_4_2C50
        db  08Bh, 004h, 09Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 2ch]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_2C59
loc_4_2C50:
        mov    ecx, dword ptr [ebp + 2ch]
        db  0C7h, 001h, 000h, 000h, 000h, 000h
loc_4_2C59:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    28h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 32h], 0
        mov    word ptr [esi + 34h], 0
        mov    word ptr [esi], 2
        lea    edi, [esi + 4]
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_2CEB
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [edi]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    bx, ax
        mov    ecx, dword ptr [edi]
        and    bx, 0fff0h
        mov    eax, dword ptr [esi + 50h]
        or     bh, 10h
        push   ebx
        push   eax
        push   ecx
        call   sub_4_0290
        push   61h
        mov    ecx, dword ptr [edi]
        push   ecx
        or     bl, 3
        call   sub_4_0260
        push   61h
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_4_0260
        push   ebx
        mov    ecx, dword ptr [esi + 50h]
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 48h]
        push   ecx
        mov    edx, dword ptr [edi]
        push   edx
        call   sub_4_0280
        jmp    SHORT loc_4_2D2B
loc_4_2CEB:
        push   0
        mov    eax, dword ptr [esi + 60h]
        push   eax
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 60h]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    ecx, dword ptr [esi + 60h]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
        push   1
        mov    ecx, dword ptr [esi + 60h]
        push   ecx
        mov    edx, dword ptr [edi]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 60h]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
loc_4_2D2B:
        mov    ebx, 3e8h
loc_4_2D30:
        push   61h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    SHORT loc_4_2D30
        push   21h
        mov    eax, dword ptr [esi + 80h]
        push   eax
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
        push   esi
        call   sub_4_3B40
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2D70:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 32h], ax
        mov    word ptr [esi + 34h], ax
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 4
        push   21h
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        mov    byte ptr [ebp - 1], al
        push   edx
        mov    edi, 0c7fh
        call   sub_4_0280
        mov    word ptr [esi], 4
        push   esi
        call   sub_4_3B40
loc_4_2DBD:
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    SHORT loc_4_2DBD
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0a8h]
        push   ebx
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0280
        push   ebx
        mov    ecx, dword ptr [esi + 0ach]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   2
        mov    ecx, dword ptr [esi + 0b4h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 9ch]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        test   al, 0ah
        je     SHORT loc_4_2E61
        push   0ah
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        mov    word ptr [esi + 30h], bx
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 39h]
        mov    dl, byte ptr [esi + ecx*4 + 0c7h]
        lea    eax, [esi + ecx*4]
        mov    byte ptr [esi + 39h], dl
        mov    byte ptr [eax + 0c7h], bl
        mov    word ptr [eax + 0c8h], bx
        mov    al, byte ptr [esi + 38h]
        test   al, al
        je     SHORT loc_4_2E56
        dec    al
        mov    byte ptr [esi + 38h], al
loc_4_2E56:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2E61:
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_2E70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    esi, esi
        mov    ebx, dword ptr [ebp + 0ch]
        lea    eax, [edi + 18h]
        mov    dword ptr [edi + 14h], esi
        cmp    ebx, esi
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], esi
        je     SHORT loc_4_2EA2
loc_4_2E8D:
        mov    eax, dword ptr [edi + 10h]
        add    eax, esi
        add    esi, 6
        add    eax, 6
        push   eax
        push   edi
        call   sub_4_2EB0
        dec    ebx
        jne    SHORT loc_4_2E8D
loc_4_2EA2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2EB0:
        push   ebp
        mov    ecx, 0ffffffffh
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
loc_4_2EC8:
        mov    dl, byte ptr [edi]
        inc    edi
        mov    eax, 8
loc_4_2ED0:
        mov    esi, ecx
        mov    bl, dl
        shr    dl, 1
        and    esi, 80000000h
        shr    esi, 1fh
        and    ebx, 1
        xor    esi, ebx
        add    ecx, ecx
        test   esi, esi
        je     SHORT loc_4_2EF3
        xor    ecx, 4c11db6h
        or     ecx, esi
loc_4_2EF3:
        dec    eax
        jne    SHORT loc_4_2ED0
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_4_2EC8
        shr    ecx, 1ah
        mov    bl, 1
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 8]
        shr    edx, 3
        and    cl, 7
        shl    bl, cl
        pop    edi
        or     byte ptr [edx + eax + 18h], bl
        pop    esi
        inc    dword ptr [eax + 14h]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_2F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    word ptr [esi + 30h], bx
        mov    word ptr [esi + 32h], bx
        mov    byte ptr [esi + 38h], bl
        mov    byte ptr [esi + 39h], bl
        mov    byte ptr [esi + 3ah], bl
        mov    byte ptr [esi + 3bh], bl
        test   byte ptr [esi + 8], 2
        je     NEAR PTR loc_4_3226
        push   6
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 70h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    byte ptr [ebp - 2], al
        test   al, 80h
        je     NEAR PTR loc_4_3226
        push   esi
        call   sub_4_2C70
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    word ptr [ebp - 4], ax
        push   ebx
        mov    ecx, dword ptr [esi + 48h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        push   ebx
        mov    ecx, dword ptr [esi + 60h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        mov    edi, 3
        call   sub_4_0290
        mov    ebx, dword ptr [esi + 10h]
loc_4_2FAE:
        mov    ax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 70h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        add    ebx, 2
        push   edx
        call   sub_4_0290
        dec    edi
        jne    SHORT loc_4_2FAE
        mov    ebx, dword ptr [esi + 10h]
        mov    edi, 3
loc_4_2FCD:
        mov    ax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 70h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        add    ebx, 2
        push   edx
        call   sub_4_0290
        dec    edi
        jne    SHORT loc_4_2FCD
        push   2e00h
        mov    eax, dword ptr [esi + 70h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 70h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   0f3h
        mov    ecx, dword ptr [esi + 70h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   1
        push   esi
        call   sub_4_3250
        test   eax, eax
        je     SHORT loc_4_3042
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3042:
        mov    word ptr [esi], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [esi + 50h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        mov    al, byte ptr [ebp - 2]
        push   4
        and    al, 40h
        mov    ecx, dword ptr [esi + 48h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0ffffffc1h
        add    ax, 40h
        mov    word ptr [ebp - 6], ax
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 60h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0270
        mov    word ptr [ebp - 2], ax
        or     ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 2], ax
loc_4_3094:
        mov    eax, dword ptr [ebp - 2]
        mov    ecx, dword ptr [esi + 60h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        call   sub_4_0140
        mov    ecx, eax
        sub    edx, edx
        mov    eax, 12ch
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        lea    edi, [ecx + eax + 1]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
loc_4_30C6:
        call   sub_4_0140
        cmp    eax, edi
        jl     SHORT loc_4_30C6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        mov    dword ptr [ebp - 0ch], 0
loc_4_30DF:
        push   0ffh
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   3ch
        mov    ecx, dword ptr [esi + 94h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 98h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 90h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   26h
        mov    ecx, dword ptr [esi + 80h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        xor    bl, bl
        call   sub_4_0280
        call   sub_4_0140
        mov    ecx, eax
        sub    edx, edx
        mov    eax, 64h
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        lea    edi, [ecx + eax + 1]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
loc_4_315F:
        call   sub_4_0140
        cmp    eax, edi
        jge    SHORT loc_4_3180
        mov    eax, dword ptr [esi + 9ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        and    al, 0ah
        mov    bl, al
        test   bl, bl
        je     SHORT loc_4_315F
loc_4_3180:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        test   bl, bl
        je     SHORT loc_4_31BF
        mov    eax, dword ptr [esi + 90h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 38h
        jne    SHORT loc_4_31E7
        push   esi
        call   sub_4_2C70
        push   0
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_31BF:
        push   esi
        call   sub_4_2C70
        push   1
        push   esi
        call   sub_4_3250
        test   eax, eax
        jne    SHORT loc_4_322F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        mov    word ptr [esi], 4
loc_4_31E7:
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 6
        jl     NEAR PTR loc_4_30DF
        mov    ax, word ptr [ebp - 2]
        xor    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 2], ax
        test   word ptr [ebp - 6], ax
        je     NEAR PTR loc_4_3094
        push   esi
        call   sub_4_2C70
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
loc_4_3226:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_322F:
        push   1
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_3250:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 32h], bx
        test   al, 2
        mov    word ptr [esi + 36h], bx
        mov    dword ptr [esi + 0c2h], eax
        je     SHORT loc_4_3275
        mov    bl, 4
loc_4_3275:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 4
        mov    dword ptr [ebp - 4], eax
        jne    SHORT loc_4_3286
        test   byte ptr [ebp + 0ch], 8
        je     SHORT loc_4_3289
loc_4_3286:
        or     bl, 8
loc_4_3289:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 10h
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_4_3297
        or     bl, 1ch
loc_4_3297:
        test   byte ptr [ebp + 0ch], 20h
        je     SHORT loc_4_32A0
        or     bl, 3
loc_4_32A0:
        push   21h
        mov    eax, dword ptr [esi + 80h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   esi
        call   sub_4_3B40
        mov    ax, word ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0b8h]
        and    ax, 1
        mov    edx, dword ptr [esi + 4]
        cmp    ax, 1
        sbb    al, al
        add    al, 49h
        push   eax
        push   ecx
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0a8h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0ach]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   ebx
        mov    ecx, dword ptr [esi + 0b0h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   2
        mov    ecx, dword ptr [esi + 0b4h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    cl, byte ptr [esi + 0eh]
        mov    edx, dword ptr [esi + 84h]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        push   eax
        call   sub_4_0280
        mov    cl, byte ptr [esi + 0eh]
        mov    edx, dword ptr [esi + 8ch]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        push   eax
        call   sub_4_0280
        mov    cl, byte ptr [esi + 0fh]
        mov    edx, dword ptr [esi + 88h]
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0280
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_33A8
        push   2
        mov    eax, dword ptr [esi + 48h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    al, byte ptr [esi + 0fh]
        mov    cl, byte ptr [esi + 0eh]
        and    eax, 0ffff00ffh
        and    ecx, 0ffff00ffh
        dec    ax
        mov    edx, dword ptr [esi + 78h]
        shl    ax, 8
        add    ax, cx
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   edx
        push   ecx
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 48h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
loc_4_33A8:
        push   0ffh
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   61h
        mov    ecx, dword ptr [esi + 80h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        xor    ebx, ebx
        call   sub_4_0280
        mov    edi, dword ptr [esi + 10h]
loc_4_33D4:
        mov    al, byte ptr [edi]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        inc    edi
        mov    eax, dword ptr [esi + 84h]
        add    eax, ebx
        inc    ebx
        push   eax
        push   ecx
        call   sub_4_0280
        cmp    ebx, 6
        jl     SHORT loc_4_33D4
        test   byte ptr [ebp + 0ch], 8
        jne    SHORT loc_4_3425
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_3425
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_3444
        lea    edi, [esi + 18h]
        xor    ebx, ebx
loc_4_3407:
        mov    al, byte ptr [edi]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        inc    edi
        mov    eax, dword ptr [esi + 0a0h]
        add    eax, ebx
        inc    ebx
        push   eax
        push   ecx
        call   sub_4_0280
        cmp    ebx, 8
        jl     SHORT loc_4_3407
        jmp    SHORT loc_4_3444
loc_4_3425:
        xor    edi, edi
loc_4_3427:
        push   0ffh
        mov    eax, dword ptr [esi + 0a0h]
        add    eax, edi
        mov    ecx, dword ptr [esi + 4]
        push   eax
        inc    edi
        push   ecx
        call   sub_4_0280
        cmp    edi, 8
        jl     SHORT loc_4_3427
loc_4_3444:
        mov    al, byte ptr [esi + 0eh]
        mov    ecx, dword ptr [esi + 9ch]
        inc    al
        mov    edx, dword ptr [esi + 4]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0280
        xor    eax, eax
        push   2
        mov    al, byte ptr [esi + 0eh]
        inc    eax
        mov    dword ptr [esi + 24h], eax
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0b4h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 0c2h
        cmp    al, 2
        je     SHORT loc_4_34AB
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_34AB:
        mov    word ptr [esi], 8
        test   byte ptr [esi + 8], 2
        jne    SHORT loc_4_34C7
        mov    al, byte ptr [esi + 0bh]
        mov    ecx, dword ptr [esi + 60h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0280
loc_4_34C7:
        cmp    byte ptr [esi + 38h], 0
        je     SHORT loc_4_34D3
        push   esi
        call   sub_4_3590
loc_4_34D3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_34E0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [edx], 8
        push   esi
        jne    SHORT loc_4_34F9
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_34F9:
        mov    esi, dword ptr [ebp + 14h]
        xor    eax, eax
        add    esi, 0ffh
        mov    al, byte ptr [edx + 3ah]
        shr    esi, 8
        xor    ebx, ebx
        cmp    byte ptr [edx + 38h], 0
        mov    bl, byte ptr [edx + eax*4 + 0c7h]
        je     SHORT loc_4_3563
        xor    eax, eax
        mov    al, byte ptr [edx + 39h]
        cmp    ebx, eax
        jbe    SHORT loc_4_354C
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0eh]
        sub    ecx, ebx
        cmp    ecx, esi
        jae    SHORT loc_4_3544
        cmp    esi, eax
        jbe    SHORT loc_4_3542
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 1e6h]
        pop    ebp
        ret    10h
loc_4_3542:
        xor    ebx, ebx
loc_4_3544:
        inc    dword ptr [edx + 1eah]
        jmp    SHORT loc_4_356B
loc_4_354C:
        sub    eax, ebx
        cmp    eax, esi
        jae    SHORT loc_4_3544
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 1e6h]
        pop    ebp
        ret    10h
loc_4_3563:
        xor    ebx, ebx
        mov    byte ptr [edx + 3ah], bl
        mov    byte ptr [edx + 39h], bl
loc_4_356B:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    byte ptr [edx + 3bh], bl
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_357C:
        add    esp, 0ch
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3590:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 39h]
        mov    ecx, dword ptr [esi + 90h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        xor    edi, edi
        push   edx
        call   sub_4_0280
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 94h]
        mov    cl, byte ptr [esi + 39h]
        mov    di, word ptr [esi + ecx*4 + 0c8h]
        mov    ecx, dword ptr [esi + 4]
        push   edi
        push   eax
        shr    edi, 8
        push   ecx
        call   sub_4_0280
        push   edi
        mov    eax, dword ptr [esi + 98h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   26h
        mov    ecx, dword ptr [esi + 80h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    word ptr [esi + 32h], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_3600:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_360D:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_3681
        xor    ecx, ecx
        mov    dl, byte ptr [esi + 3bh]
        mov    cx, word ptr [esi + 36h]
        lea    eax, [ecx + 0ffh]
        shr    eax, 8
        add    al, dl
        cmp    byte ptr [esi + 0eh], al
        ja     SHORT loc_4_362F
        xor    al, al
loc_4_362F:
        and    edx, 0ffh
        mov    byte ptr [esi + edx*4 + 0c7h], al
        xor    eax, eax
        mov    al, byte ptr [esi + 3bh]
        mov    word ptr [esi + eax*4 + 0c8h], cx
        mov    word ptr [esi + 36h], 0
        mov    al, byte ptr [esi + 38h]
        test   al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [esi + 38h], cl
        jne    SHORT loc_4_366A
        mov    al, byte ptr [esi + 3bh]
        push   esi
        mov    byte ptr [esi + 39h], al
        call   sub_4_3590
        jmp    SHORT loc_4_3679
loc_4_366A:
        mov    cl, byte ptr [esi + 3bh]
        xor    eax, eax
        mov    al, byte ptr [esi + 3ah]
        mov    byte ptr [esi + eax*4 + 0c7h], cl
loc_4_3679:
        mov    al, byte ptr [esi + 3bh]
        mov    byte ptr [esi + 3ah], al
        xor    eax, eax
loc_4_3681:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     NEAR PTR loc_4_375C
        mov    ax, word ptr [esi + 32h]
        test   ax, ax
        jle    NEAR PTR loc_4_375C
        dec    ax
        mov    word ptr [esi + 32h], ax
        test   ax, ax
        jg     NEAR PTR loc_4_375C
        push   esi
        call   sub_4_2C70
        inc    dword ptr [esi + 1eeh]
        mov    ax, word ptr [esi + 30h]
        inc    ax
        mov    word ptr [esi + 30h], ax
        cmp    ax, 2
        jbe    SHORT loc_4_370C
        mov    word ptr [esi + 30h], bx
        xor    eax, eax
        mov    al, byte ptr [esi + 39h]
        mov    cl, byte ptr [esi + eax*4 + 0c7h]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 39h], cl
        mov    byte ptr [eax + 0c7h], bl
        mov    word ptr [eax + 0c8h], bx
        mov    al, byte ptr [esi + 38h]
        test   al, al
        je     SHORT loc_4_370A
        dec    al
        mov    byte ptr [esi + 38h], al
loc_4_370A:
        mov    bl, 10h
loc_4_370C:
        mov    eax, dword ptr [esi + 0c2h]
        push   eax
        push   esi
        call   sub_4_3250
        test   eax, eax
        je     SHORT loc_4_3739
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_3739:
        test   bl, bl
        je     SHORT loc_4_375C
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10F0
        cmp    eax, -1
        jge    SHORT loc_4_375C
loc_4_374C:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10F0
        cmp    eax, -1
        jl     SHORT loc_4_374C
loc_4_375C:
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3770:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 8
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 1d2h
        rep stosd
        mov    eax, 2ch
        pop    edi
        pop    ebp
        ret    4
sub_4_3790:
        push   ebp
        mov    ecx, 0bh
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 1c6h
        mov    edi, dword ptr [ebp + 0ch]
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_37B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        inc    dword ptr [esi + 1dah]
        call   sub_4_2D70
        push   2
        mov    eax, dword ptr [esi + 0b4h]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   22h
        mov    ecx, dword ptr [esi + 80h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 0c2h
        cmp    al, 2
        je     SHORT loc_4_3821
        push   0
        mov    edi, dword ptr [ebp + 0ch]
        push   0
        push   0ch
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   edi
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3821:
        mov    word ptr [esi], 8
        xor    eax, eax
        mov    al, byte ptr [esi + 0fh]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0eh]
        xor    ebx, ebx
        sub    eax, ecx
        mov    edi, dword ptr [ebp + 0ch]
        inc    eax
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        je     NEAR PTR loc_4_38FE
loc_4_3843:
        mov    ax, word ptr [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_384D:
        add    esp, 4
        and    eax, 0ffh
        cmp    eax, dword ptr [esi + 24h]
        je     NEAR PTR loc_4_38FE
        cmp    dword ptr [esi + 0c2h], 0
        mov    eax, 2
        je     SHORT loc_4_3872
        push   edi
        call   sub_4_3C00
loc_4_3872:
        cmp    eax, 1
        jne    SHORT loc_4_38A0
        mov    al, byte ptr [esi + 2dh]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 24h], ecx
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_4_388D
        mov    al, byte ptr [esi + 0fh]
        dec    al
loc_4_388D:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        push   ecx
        call   sub_4_0280
        jmp    SHORT loc_4_38F4
loc_4_38A0:
        cmp    eax, 2
        jne    SHORT loc_4_38F0
        mov    ax, word ptr [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_38AF:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 24h], ecx
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_4_38C5
        mov    al, byte ptr [esi + 0fh]
        dec    al
loc_4_38C5:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        push   ecx
        call   sub_4_0280
        push   5
        mov    ecx, dword ptr [esi + 9ch]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        inc    dword ptr [esi + 1e2h]
        jmp    SHORT loc_4_38F4
loc_4_38F0:
        test   eax, eax
        jl     SHORT loc_4_3956
loc_4_38F4:
        inc    ebx
        cmp    dword ptr [ebp - 4], ebx
        ja     NEAR PTR loc_4_3843
loc_4_38FE:
        push   10h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0b4h]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        cmp    byte ptr [esi + 38h], 0
        je     SHORT loc_4_3949
        push   0ah
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   esi
        call   sub_4_3590
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3949:
        push   0
        push   edi
        call   sub_4_10F0
        cmp    eax, -1
        jl     SHORT loc_4_3949
loc_4_3956:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_4_3960:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        mov    byte ptr [ebp - 2], 0
        mov    edi, dword ptr [ebp + 8]
loc_4_397C:
        mov    eax, dword ptr [esi + 9ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 10h
        jne    NEAR PTR loc_4_3B16
        push   5
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        mov    cx, word ptr [esi + 2]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_39B0:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        cmp    ecx, dword ptr [esi + 24h]
        je     NEAR PTR loc_4_3A63
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 1
        cmp    dword ptr [esi + 0c2h], 0
        mov    eax, 2
        je     SHORT loc_4_39DC
        push   edi
        call   sub_4_3C00
loc_4_39DC:
        cmp    eax, 1
        jne    SHORT loc_4_3A0A
        mov    cl, byte ptr [esi + 2dh]
        xor    eax, eax
        mov    al, cl
        dec    cl
        mov    dword ptr [esi + 24h], eax
        cmp    byte ptr [esi + 0eh], cl
        jbe    SHORT loc_4_39F7
        mov    cl, byte ptr [esi + 0fh]
        dec    cl
loc_4_39F7:
        push   ecx
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        jmp    SHORT loc_4_3A67
loc_4_3A0A:
        cmp    eax, 2
        jne    SHORT loc_4_3A5A
        mov    ax, word ptr [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A19:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 24h], ecx
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_4_3A2F
        mov    al, byte ptr [esi + 0fh]
        dec    al
loc_4_3A2F:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        push   ecx
        call   sub_4_0280
        push   5
        mov    ecx, dword ptr [esi + 9ch]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        inc    dword ptr [esi + 1e2h]
        jmp    SHORT loc_4_3A63
loc_4_3A5A:
        test   eax, eax
        jge    SHORT loc_4_3A67
        jmp    NEAR PTR loc_4_3B21
loc_4_3A63:
        mov    byte ptr [ebp - 1], 1
loc_4_3A67:
        mov    eax, dword ptr [esi + 9ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0ah
        je     NEAR PTR loc_4_3B09
        mov    word ptr [esi + 32h], 0
        mov    word ptr [esi + 30h], 0
        push   0ah
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 90h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    bl, al
        xor    eax, eax
        and    bl, 0fh
        mov    al, byte ptr [esi + 39h]
        mov    byte ptr [ebp - 2], 1
        mov    dl, byte ptr [esi + eax*4 + 0c7h]
        lea    ecx, [esi + eax*4]
        mov    byte ptr [esi + 39h], dl
        mov    byte ptr [ecx + 0c7h], 0
        mov    word ptr [ecx + 0c8h], 0
        mov    al, byte ptr [esi + 38h]
        test   al, al
        je     SHORT loc_4_3AEC
        dec    al
        mov    byte ptr [esi + 38h], al
        je     SHORT loc_4_3AEC
        push   esi
        call   sub_4_3590
loc_4_3AEC:
        push   ebx
        push   edi
        call   sub_4_10F0
        cmp    eax, -1
        jge    SHORT loc_4_3B05
loc_4_3AF8:
        push   0
        push   edi
        call   sub_4_10F0
        cmp    eax, -1
        jl     SHORT loc_4_3AF8
loc_4_3B05:
        xor    al, al
        jmp    SHORT loc_4_3B0B
loc_4_3B09:
        mov    al, 1
loc_4_3B0B:
        test   byte ptr [ebp - 1], al
        je     NEAR PTR loc_4_397C
        jmp    SHORT loc_4_3B21
loc_4_3B16:
        push   edi
        push   esi
        call   sub_4_37B0
        mov    byte ptr [ebp - 2], 1
loc_4_3B21:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 2]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_3B40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_3B6C
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    ax, 0fffbh
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_3B6C:
        push   0
        mov    eax, dword ptr [esi + 0bch]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3B90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   1fh
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0bch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0280
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_3BCC
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        or     al, 4
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_3BCC:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3BE0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_3BF3
        mov    eax, 3fh
        pop    ebp
        ret    8
loc_4_3BF3:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0c2h]
        ret    8
sub_4_3C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0b3h]
        inc    eax
        mov    dword ptr [esi + 0b3h], eax
        cmp    eax, 0ah
        jbe    SHORT loc_4_3C3D
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_3C3D
        push   0
        push   esi
        call   sub_4_10F0
        cmp    eax, -1
        jne    SHORT loc_4_3C3D
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C3D:
        lea    edi, [esi + 0bdh]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0fch
        push   edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3C55:
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     SHORT loc_4_3CA6
        add    eax, 3
        cmp    eax, 3
        ja     SHORT loc_4_3CA6
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_3C6E:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C78:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C82:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C8F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_3FB0
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3CA6:
        sub    dword ptr [ebp - 4], 0eh
        mov    ecx, dword ptr [esi + 7fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     SHORT loc_4_3CB6
        mov    ecx, eax
loc_4_3CB6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 1ch], esi
        push   eax
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 0ch], 0
        push   ecx
        inc    dword ptr [esi + 87h]
        lea    ecx, [esi + 0cbh]
        lea    edx, [ebp - 1ch]
        mov    byte ptr [esi + 41h], 1
        push   ecx
        mov    eax, dword ptr [esi + 7bh]
        push   0eh
        push   edi
        push   edi
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3CEA:
        cmp    dword ptr [ebp - 18h], 0
        sbb    eax, eax
        pop    edi
        inc    eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 08Dh, 064h, 024h, 000h
loc_4_3D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    ecx, dword ptr [ebp + 1ch]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ecx], 0
        mov    dword ptr [esi + 4], 0
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_3D5B
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        add    edx, 0eh
        mov    ecx, dword ptr [ebp + 14h]
        push   edx
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3D50:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_4_3D5B:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [ebp + 10h]
        lea    edi, [ecx + edx]
        cmp    edi, eax
        jbe    SHORT loc_4_3D72
        sub    eax, edx
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_4_3D75
loc_4_3D72:
        mov    dword ptr [ebp - 18h], ecx
loc_4_3D75:
        mov    edi, dword ptr [ebp - 18h]
        add    edx, 0eh
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    dword ptr [ebp - 8], edx
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3D91:
        test   edi, edi
        je     SHORT loc_4_3DE7
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3DA8:
        mov    eax, dword ptr [ebp - 10h]
        cmp    ebx, eax
        jb     SHORT loc_4_3DB1
        mov    ebx, eax
loc_4_3DB1:
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3DC7:
        add    esp, 14h
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     SHORT loc_4_3DE7
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3DE1:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_3D91
loc_4_3DE7:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, edi
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h
sub_4_3E00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_3E22
loc_4_3E0E:
        mov    ecx, dword ptr [eax + 24h]
        push   eax
        push   esi
        mov    dword ptr [esi + 38h], ecx
        call   sub_4_3E30
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        jne    SHORT loc_4_3E0E
loc_4_3E22:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3E4C:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 7fh]
        add    ebx, 0eh
        cmp    ebx, eax
        jb     SHORT loc_4_3E5E
        mov    ebx, eax
loc_4_3E5E:
        lea    eax, [ebx - 0eh]
        push   0
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 4], 0eh
        push   ecx
        push   0
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3E78:
        lea    ecx, [ebp - 14h]
        lea    edx, [ebp - 10h]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        lea    ecx, [esi + 0bdh]
        mov    dword ptr [ebp - 10h], 0
        push   eax
        push   ebx
        push   ecx
        call   sub_4_3F00
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [esi + 0cbh]
        push   eax
        mov    dword ptr [ebp - 28h], esi
        push   edx
        mov    dword ptr [ebp - 20h], ecx
        push   0eh
        lea    ecx, [esi + 0bdh]
        push   ecx
        lea    edx, [ebp - 28h]
        push   ecx
        mov    dword ptr [ebp - 18h], edi
        push   edx
        mov    ecx, dword ptr [esi + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3EC7:
        push   0
        inc    dword ptr [esi + 83h]
        mov    byte ptr [esi + 41h], 1
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3EE0:
        push   esi
        call   sub_4_1770
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3F00:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        lea    edx, [ebp - 4]
        push   ecx
        mov    esi, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3F26:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 14h]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_3FA4
loc_4_3F33:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax]
        lea    edx, [ecx + ebx]
        cmp    edx, dword ptr [ebp + 0ch]
        jae    SHORT loc_4_3F7E
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3F50:
        mov    ecx, dword ptr [ebp + 18h]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3F5E:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3F74:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_3F33
loc_4_3F7E:
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_3FA4
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [eax]
        sub    esi, eax
        add    ecx, dword ptr [ebp - 4]
        push   esi
        add    eax, dword ptr [ebp + 8]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3F9D:
        mov    ecx, dword ptr [ebp + 18h]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_3FA4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_4_3FB0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        test   eax, 208h
        jne    SHORT loc_4_3FFC
        test   eax, 206h
        je     SHORT loc_4_3FD8
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        inc    dword ptr [eax + 9bh]
        ret    8
loc_4_3FD8:
        test   eax, 204h
        je     SHORT loc_4_3FEC
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        ret    8
loc_4_3FEC:
        test   eax, 202h
        je     SHORT loc_4_3FFC
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 9bh]
loc_4_3FFC:
        pop    ebp
        ret    8
_PTEXT ends
        end
