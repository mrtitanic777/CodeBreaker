; hpmca_pcode_04.asm -- HPMCA object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 04Ch
        db  024h, 00Ch, 08Bh, 0ECh, 083h, 0ECh, 008h, 085h, 0C9h, 075h, 01Eh, 0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 04Dh, 010h, 050h, 08Dh, 055h, 0FCh, 06Ah, 003h, 051h, 052h, 0E8h, 000h, 000h, 000h, 000h
        db  08Bh, 045h, 0FCh, 08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Bh, 045h, 010h, 08Bh, 055h, 008h, 050h
        db  051h, 08Bh, 08Ah, 001h, 002h, 000h, 000h, 08Dh, 045h, 0FCh, 051h, 08Dh, 055h, 0F8h, 050h, 052h
        db  0E8h, 000h, 000h, 000h, 000h, 08Bh, 045h, 0FCh, 08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 064h
        db  024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
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
        call   sub_4_2660
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
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 068h, 0E8h, 003h, 000h, 000h, 0E8h, 000h
        db  000h, 000h, 000h, 0FFh, 005h, 052h, 000h, 000h, 000h, 0A1h, 052h, 000h, 000h, 000h, 0C3h, 08Dh
        db  049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh
        db  05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h
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
        call   sub_4_31F0
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
        call   sub_4_19E0
        test   al, al
        je     SHORT loc_4_0300
        mov    dword ptr [esi + 0b3h], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3010
        push   esi
        call   sub_4_1A10
loc_4_0300:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0310:
        push   ebp
        mov    eax, 15h
        mov    ebp, esp
        sub    esp, 38h
        mov    word ptr [ebp - 30h], ax
        push   ebx
        mov    word ptr [ebp - 2eh], ax
        push   esi
        push   edi
        mov    eax, 0ah
        mov    word ptr [ebp - 38h], ax
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 36h], ax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [esi + 18h]
        xor    ebx, ebx
        mov    dword ptr [ebp - 18h], edx
        mov    dword ptr [ebp - 10h], ecx
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 8]
        push   edi
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 20h], ebx
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0372:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_0385
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0385:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_039C:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_03AB
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], ecx
loc_4_03AB:
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 20h]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03BD:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_047F
        cmp    word ptr [ebp - 28h], 63cah
        jne    NEAR PTR loc_4_047F
        mov    al, byte ptr [ebp - 26h]
        and    al, 0eh
        shr    al, 1
        and    eax, 0ffh
        cmp    eax, 7
        ja     SHORT loc_4_040A
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_03EC:
        mov    bl, 3
        jmp    SHORT loc_4_040A
loc_4_03F0:
        mov    bl, 4
        jmp    SHORT loc_4_040A
loc_4_03F4:
        mov    bl, 5
        jmp    SHORT loc_4_040A
loc_4_03F8:
        mov    bl, 7
        jmp    SHORT loc_4_040A
loc_4_03FC:
        mov    bl, 9
        jmp    SHORT loc_4_040A
loc_4_0400:
        mov    bl, 0ah
        jmp    SHORT loc_4_040A
loc_4_0404:
        mov    bl, 0bh
        jmp    SHORT loc_4_040A
loc_4_0408:
        mov    bl, 0ch
loc_4_040A:
        mov    edi, ebx
        push   0
        and    edi, 0ffh
        mov    bl, byte ptr [ebp - 26h]
        and    ebx, 0e0h
        lea    eax, [ebp - 30h]
        add    ebx, 8
        push   eax
        shl    ebx, 7
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0438:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0447
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], ecx
loc_4_0447:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0450:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 4], eax
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0ch], edi
        pop    edi
        mov    dword ptr [esi + 14h], edx
        mov    dword ptr [esi + 10h], 1
        mov    dword ptr [esi + 18h], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_047F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0488:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_04C0:
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
        je     SHORT loc_4_0505
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0505:
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
        jne    SHORT loc_4_053B
        mov    dword ptr [ecx + 20h], 1
        jmp    SHORT loc_4_0548
loc_4_053B:
        cmp    dword ptr [ebp - 0ch], 1
        jne    SHORT loc_4_0548
        mov    dword ptr [ecx + 20h], 2
loc_4_0548:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 209h], eax
        mov    dword ptr [ecx + 20dh], edx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0570:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     SHORT loc_4_0591
        mov    eax, 0c0010002h
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0591:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 10h]
        test   ecx, ecx
        jne    SHORT loc_4_05AB
        push   edx
        push   esi
        push   edi
        call   sub_4_09C0
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05AB:
        cmp    ecx, 1
        jne    SHORT loc_4_05E3
        mov    ecx, dword ptr [edi + 47h]
        test   ecx, ecx
        je     SHORT loc_4_05D3
        test   cl, 2
        je     SHORT loc_4_05C1
        mov    eax, 0c0000001h
loc_4_05C1:
        test   cl, 1
        je     SHORT loc_4_05E8
        mov    eax, 0c001000dh
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05D3:
        push   edx
        push   esi
        push   edi
        call   sub_4_0A10
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05E3:
        mov    eax, 10001h
loc_4_05E8:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_05F0:
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
        ja     SHORT loc_4_062F
        je     NEAR PTR loc_4_0815
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_4_0650
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_062F:
        cmp    eax, 1010102h
        je     NEAR PTR loc_4_0834
        cmp    eax, 1010103h
        je     NEAR PTR loc_4_0853
        cmp    eax, 1010104h
        je     NEAR PTR loc_4_0903
loc_4_0650:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_4_0919
loc_4_065C:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_4_0671
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_4_091F
loc_4_0671:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_4_091F
loc_4_0680:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 47h]
        test   al, 2
        je     SHORT loc_4_0693
        mov    dword ptr [ebp - 18h], 4
        jmp    SHORT loc_4_06A5
loc_4_0693:
        mov    dword ptr [ebp - 18h], 2
        test   al, 1
        jne    SHORT loc_4_06A5
        mov    dword ptr [ebp - 18h], 0
loc_4_06A5:
        lea    eax, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_4_091F
loc_4_06B2:
        lea    eax, [ebp - 1ch]
        mov    edi, 4
        jmp    NEAR PTR loc_4_091F
loc_4_06BF:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_4_091F
loc_4_06D3:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_4_091F
loc_4_06E7:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_4_091F
loc_4_06FB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a3h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_091F
loc_4_0714:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a7h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_091F
loc_4_072D:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_091F
loc_4_0741:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0abh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_091F
loc_4_075A:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0771:
        mov    byte ptr [ebp - 21h], 0
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_091F
loc_4_077D:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 1fh
        jmp    NEAR PTR loc_4_091F
loc_4_078C:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_079D
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7bh]
        mov    ecx, dword ptr [eax + 0ch]
        jmp    SHORT loc_4_07A6
loc_4_079D:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [eax + 8]
loc_4_07A6:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_4_091F
loc_4_07B6:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_07C4
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7fh]
        jmp    SHORT loc_4_07CA
loc_4_07C4:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 21h]
loc_4_07CA:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_091F
loc_4_07DA:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_4_091F
loc_4_07ED:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_091F
loc_4_0801:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_4_091F
loc_4_0815:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_082C:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_091F
loc_4_0834:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 67h
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_084B:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_091F
loc_4_0853:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_08A9
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 7bh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_0880
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_0919
loc_4_0880:
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
loc_4_0896:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ebp - 0ch], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0919
loc_4_08A9:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [edi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08BC:
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_08D7
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0919
loc_4_08D7:
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
loc_4_08F4:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0919
loc_4_0903:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 20dh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    SHORT loc_4_091F
loc_4_0919:
        mov    eax, dword ptr [ebp - 14h]
        mov    edi, dword ptr [ebp - 14h]
loc_4_091F:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_095A
        cmp    esi, 1010103h
        je     SHORT loc_4_095A
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_4_094A
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
loc_4_094A:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0955:
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_4_095A:
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
sub_4_09C0:
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
        call   sub_4_05F0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0A10:
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
        ja     SHORT loc_4_0A4D
        je     NEAR PTR loc_4_0B1F
        cmp    eax, 1010eh
        je     SHORT loc_4_0A5F
        cmp    eax, 1010fh
        je     SHORT loc_4_0A99
        cmp    eax, 10112h
        je     NEAR PTR loc_4_0B0B
loc_4_0A4D:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0B5A
loc_4_0A5F:
        cmp    edi, 4
        je     SHORT loc_4_0A76
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0B5A
loc_4_0A76:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A82:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0B80
        jmp    NEAR PTR loc_4_0B58
loc_4_0A99:
        cmp    edi, 4
        je     SHORT loc_4_0AB0
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0B5A
loc_4_0AB0:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0ABC:
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_4_0B04
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 7fh]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_4_0ADE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 7fh], eax
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0B5A
loc_4_0ADE:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx + 21h]
        jne    SHORT loc_4_0AFC
        mov    eax, dword ptr [ecx + 21h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_4_0AFC
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 21h], eax
        call   sub_4_0DD0
        jmp    SHORT loc_4_0B5A
loc_4_0AFC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0B5A
loc_4_0B04:
        mov    ebx, 0c0000001h
        jmp    SHORT loc_4_0B5A
loc_4_0B0B:
        cmp    edi, 4
        je     SHORT loc_4_0B5A
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0B5A
loc_4_0B1F:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_4_0B3D
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0B5A
loc_4_0B3D:
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
        call   sub_4_0BE0
loc_4_0B58:
        mov    ebx, eax
loc_4_0B5A:
        test   ebx, ebx
        jne    SHORT loc_4_0B68
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_4_0B68:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B80:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0BC2
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0BB8
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
loc_4_0BB1:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    10h
loc_4_0BB8:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_4_0BC2:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0BE0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0C26
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0C1C
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
loc_4_0C15:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    14h
loc_4_0C1C:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_4_0C26:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    14h
        db  08Dh, 049h, 000h
sub_4_0C40:
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
        call   sub_4_05F0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_4_0D15
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_4_0CA5
        je     SHORT loc_4_0CE4
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_4_0CB3
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0CA5:
        cmp    eax, 1020102h
        je     SHORT loc_4_0CEC
        cmp    eax, 1020103h
        je     SHORT loc_4_0CF4
loc_4_0CB3:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    SHORT loc_4_0CFD
loc_4_0CBC:
        mov    eax, dword ptr [ebx + 83h]
        jmp    SHORT loc_4_0CFA
loc_4_0CC4:
        mov    eax, dword ptr [ebx + 87h]
        jmp    SHORT loc_4_0CFA
loc_4_0CCC:
        mov    eax, dword ptr [ebx + 8bh]
        jmp    SHORT loc_4_0CFA
loc_4_0CD4:
        mov    eax, dword ptr [ebx + 9bh]
        jmp    SHORT loc_4_0CFA
loc_4_0CDC:
        mov    eax, dword ptr [ebx + 9fh]
        jmp    SHORT loc_4_0CFA
loc_4_0CE4:
        mov    eax, dword ptr [ebx + 97h]
        jmp    SHORT loc_4_0CFA
loc_4_0CEC:
        mov    eax, dword ptr [ebx + 8fh]
        jmp    SHORT loc_4_0CFA
loc_4_0CF4:
        mov    eax, dword ptr [ebx + 93h]
loc_4_0CFA:
        mov    dword ptr [ebp - 4], eax
loc_4_0CFD:
        cmp    edi, 4
        jb     SHORT loc_4_0D15
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D11:
        add    dword ptr [ebp - 0ch], 4
loc_4_0D15:
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
loc_4_0D50:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [ecx + 14h]
        cmp    edx, 10113h
        ja     SHORT loc_4_0D78
        je     SHORT loc_4_0DAF
        cmp    edx, 10101h
        jb     SHORT loc_4_0DAA
        cmp    edx, 10111h
        jbe    SHORT loc_4_0DAF
        jmp    SHORT loc_4_0DAA
loc_4_0D78:
        cmp    edx, 1010104h
        ja     SHORT loc_4_0D9A
        cmp    edx, 1010101h
        jae    SHORT loc_4_0DAF
        cmp    edx, 20101h
        jb     SHORT loc_4_0DAA
        cmp    edx, 20105h
        jbe    SHORT loc_4_0DAF
        jmp    SHORT loc_4_0DAA
loc_4_0D9A:
        cmp    edx, 1020101h
        jb     SHORT loc_4_0DAA
        cmp    edx, 1020103h
        jbe    SHORT loc_4_0DAF
loc_4_0DAA:
        mov    eax, 0c0010017h
loc_4_0DAF:
        test   eax, eax
        jne    SHORT loc_4_0DBD
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0C40
loc_4_0DBD:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0DD0:
        push   ebp
        mov    ecx, 0eeh
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 1c5h]
        test   edx, edx
        je     SHORT loc_4_0DF6
loc_4_0DE6:
        mov    esi, dword ptr [edx + 21h]
        cmp    ecx, esi
        jae    SHORT loc_4_0DEF
        mov    ecx, esi
loc_4_0DEF:
        mov    edx, dword ptr [edx + 11h]
        test   edx, edx
        jne    SHORT loc_4_0DE6
loc_4_0DF6:
        pop    esi
        mov    dword ptr [eax + 7fh], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0E00:
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
loc_4_0E27:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     SHORT loc_4_0E3E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E3E:
        mov    eax, dword ptr [esi + 47h]
        test   eax, eax
        je     SHORT loc_4_0E69
        test   al, 0ah
        je     SHORT loc_4_0E57
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E57:
        test   al, 1
        je     SHORT loc_4_0E69
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E69:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_0E90
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1020
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E90:
        push   esi
        call   sub_4_19E0
        test   al, al
        jne    SHORT loc_4_0EBA
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1020
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 46h], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0EBA:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0ECB:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2B90
        test   eax, eax
        je     SHORT loc_4_0F19
        jle    SHORT loc_4_0F07
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1020
        push   esi
        call   sub_4_1A10
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F07:
        test   eax, eax
        jge    SHORT loc_4_0F19
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F19:
        xor    edi, edi
        xor    eax, eax
loc_4_0F1D:
        cmp    eax, dword ptr [ebp - 8]
        jae    SHORT loc_4_0F37
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0b6h], cl
        jb     SHORT loc_4_0F1D
loc_4_0F37:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        je     SHORT loc_4_0F9E
loc_4_0F3F:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F4C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F5D:
        xor    ecx, ecx
        cmp    edi, 6
        jae    SHORT loc_4_0F7E
loc_4_0F64:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_0F7E
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0b6h], al
        jb     SHORT loc_4_0F64
loc_4_0F7E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F8E:
        add    esp, 0ch
        test   eax, eax
        jl     SHORT loc_4_0FB8
        inc    ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, ebx
        ja     SHORT loc_4_0F3F
loc_4_0F9E:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2CB0
        test   eax, eax
        jge    SHORT loc_4_0FC6
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FB8:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FC6:
        lea    eax, [esi + 0b7h]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FD6:
        test   al, al
        je     SHORT loc_4_0FFC
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1060
        push   esi
        call   sub_4_1A10
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FFC:
        push   esi
        inc    dword ptr [esi + 83h]
        call   sub_4_1A10
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_1760
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_1020:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_104C
        pop    ebp
        mov    dword ptr [eax + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
loc_4_104C:
        mov    ecx, dword ptr [eax + 28h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_1060:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 38h], 0
        jne    SHORT loc_4_1089
        pop    ebp
        mov    dword ptr [eax + 38h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
loc_4_1089:
        mov    ecx, dword ptr [eax + 34h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10A0:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 24h], 0
        mov    eax, dword ptr [ecx + 24h]
        test   eax, eax
        jne    SHORT loc_4_10C5
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [ecx + 28h], edx
        inc    dword ptr [ecx + 2ch]
        ret    8
loc_4_10C5:
        pop    ebp
        mov    dword ptr [edx + 24h], eax
        mov    dword ptr [ecx + 24h], edx
        inc    dword ptr [ecx + 2ch]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10E0:
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
        je     SHORT loc_4_1138
        test   al, 1
        je     SHORT loc_4_1104
        inc    dword ptr [esi + 30h]
loc_4_1104:
        test   al, 2
        je     SHORT loc_4_1111
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 93h]
loc_4_1111:
        test   al, 4
        je     SHORT loc_4_111E
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_111E:
        test   al, 8
        je     SHORT loc_4_112B
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_112B:
        test   al, 10h
        je     SHORT loc_4_1138
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_1138:
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_4_114C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_114C:
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
loc_4_116C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_117D:
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2B90
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_11C7
        push   edi
        push   esi
        call   sub_4_10A0
        test   ebx, ebx
        jge    SHORT loc_4_11B9
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
loc_4_11B9:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_11C7:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_11CB:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_11E5
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0b6h], al
        jb     SHORT loc_4_11CB
loc_4_11E5:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 14h]
        dec    eax
        je     SHORT loc_4_1255
loc_4_11F2:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_11FF:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1210:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    SHORT loc_4_1231
loc_4_1217:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_1231
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0b6h], al
        jb     SHORT loc_4_1217
loc_4_1231:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1241:
        add    esp, 0ch
        cmp    eax, -2
        je     SHORT loc_4_1277
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_4_11F2
loc_4_1255:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2CB0
        cmp    eax, -2
        jne    SHORT loc_4_128C
        push   edi
        push   esi
        call   sub_4_10A0
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1277:
        push   edi
        push   esi
        call   sub_4_10A0
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_128C:
        lea    eax, [esi + 0b7h]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_129C:
        test   al, al
        je     SHORT loc_4_12BC
        push   edi
        mov    eax, dword ptr [edi + 28h]
        push   eax
        push   esi
        call   sub_4_1060
        push   esi
        call   sub_4_34A0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_12BC:
        push   0
        inc    dword ptr [esi + 83h]
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12D1:
        push   esi
        call   sub_4_1760
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_12F0:
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
loc_4_1315:
        test   edx, edx
        je     SHORT loc_4_1324
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_4_1315
        mov    dword ptr [ecx], edx
loc_4_1324:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_4_1337
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_1337:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   25h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_134A:
        test   eax, eax
        je     SHORT loc_4_135C
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_135C:
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
loc_4_1377:
        test   al, al
        jne    SHORT loc_4_1396
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   25h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1388:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_1396:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_13B8
        mov    eax, dword ptr [esi + 1cdh]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 103h
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1cdh], ecx
        jmp    SHORT loc_4_13CF
loc_4_13B8:
        mov    eax, dword ptr [esi + 1c5h]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1c5h], ecx
loc_4_13CF:
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
        jne    SHORT loc_4_1422
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
loc_4_1422:
        mov    eax, ebx
        pop    edi
        inc    dword ptr [esi + 1c9h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1440:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        xor    eax, eax
loc_4_144F:
        mov    dl, byte ptr [esi + eax + 67h]
        mov    ecx, dword ptr [esi + 73h]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     SHORT loc_4_144F
        mov    edi, dword ptr [esi + 73h]
        add    edi, 6
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_1487
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 10h]
loc_4_1472:
        xor    ecx, ecx
loc_4_1474:
        mov    dl, byte ptr [eax + ecx]
        inc    edi
        inc    ecx
        mov    byte ptr [edi - 1], dl
        cmp    ecx, 6
        jb     SHORT loc_4_1474
        add    eax, 6
        dec    ebx
        jne    SHORT loc_4_1472
loc_4_1487:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 77h], eax
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_14A0
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 44h], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_14A0:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14AF:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2840
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28F0
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2920
        test   eax, eax
        jge    SHORT loc_4_14EE
        mov    eax, 0c0000001h
        pop    edi
        or     dword ptr [esi + 47h], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_14EE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  090h
loc_4_1500:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax + 0dh]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 0ffffffd0h
        je     SHORT loc_4_1520
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_1520:
        test   al, 1
        je     SHORT loc_4_1529
        mov    ecx, 1
loc_4_1529:
        test   al, 2
        je     SHORT loc_4_1530
        or     ecx, 4
loc_4_1530:
        test   al, 4
        je     SHORT loc_4_1537
        or     ecx, 8
loc_4_1537:
        test   al, 8
        je     SHORT loc_4_153E
        or     ecx, 2
loc_4_153E:
        test   al, 20h
        je     SHORT loc_4_1545
        or     ecx, 10h
loc_4_1545:
        test   dword ptr [esi + 1bdh], ecx
        je     SHORT loc_4_1557
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_1557:
        mov    dword ptr [esi + 1b9h], ecx
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_156E
        mov    byte ptr [esi + 43h], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_4_156E:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_157D:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 1b9h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2920
        test   eax, eax
        jge    SHORT loc_4_15A6
        or     dword ptr [esi + 47h], 2
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_4_15A6:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15C0:
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
loc_4_15E1:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 1eeh]
        test   ecx, ecx
        je     SHORT loc_4_1620
loc_4_1601:
        mov    eax, dword ptr [esi + 1eeh]
        push   ecx
        mov    edx, dword ptr [eax + 15h]
        mov    dword ptr [esi + 1eeh], edx
        call   sub_4_1760
        mov    ecx, dword ptr [esi + 1eeh]
        test   ecx, ecx
        jne    SHORT loc_4_1601
loc_4_1620:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1630:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 47h], 8
        jne    SHORT loc_4_1666
        push   esi
        call   sub_4_19E0
        test   al, al
        je     SHORT loc_4_1655
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D40
        push   esi
        call   sub_4_1A10
loc_4_1655:
        push   3e8h
        add    esi, 1ddh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1666:
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_4_1670:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 4], 0
        mov    esi, dword ptr [edi + 0dh]
        je     SHORT loc_4_168E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_168E:
        inc    dword ptr [edi]
        mov    eax, dword ptr [esi + 1c5h]
        cmp    edi, eax
        jne    SHORT loc_4_16A5
        mov    eax, dword ptr [edi + 11h]
        mov    dword ptr [esi + 1c5h], eax
        jmp    SHORT loc_4_16B8
loc_4_16A5:
        cmp    dword ptr [eax + 11h], edi
        je     SHORT loc_4_16B2
loc_4_16AA:
        mov    eax, dword ptr [eax + 11h]
        cmp    dword ptr [eax + 11h], edi
        jne    SHORT loc_4_16AA
loc_4_16B2:
        mov    ecx, dword ptr [edi + 11h]
        mov    dword ptr [eax + 11h], ecx
loc_4_16B8:
        push   0
        mov    eax, dword ptr [edi + 1dh]
        push   eax
        mov    ecx, dword ptr [esi + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16C7:
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_16DF
        cmp    ebx, 103h
        je     SHORT loc_4_16F3
        cmp    ebx, 0c001000eh
        je     SHORT loc_4_16FC
        jmp    SHORT loc_4_1705
loc_4_16DF:
        mov    eax, dword ptr [edi]
        sub    eax, 2
        mov    dword ptr [edi], eax
        je     SHORT loc_4_1707
        mov    byte ptr [edi + 4], 1
        mov    ebx, 103h
        jmp    SHORT loc_4_1707
loc_4_16F3:
        mov    byte ptr [edi + 4], 1
        sub    dword ptr [edi], 2
        jmp    SHORT loc_4_1707
loc_4_16FC:
        mov    byte ptr [edi + 4], 1
        mov    ebx, 103h
loc_4_1705:
        dec    dword ptr [edi]
loc_4_1707:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_4_174B
        mov    eax, dword ptr [edi + 21h]
        cmp    dword ptr [esi + 7fh], eax
        jne    SHORT loc_4_171A
        push   esi
        call   sub_4_0DD0
loc_4_171A:
        cmp    dword ptr [esi + 1c5h], 0
        jne    SHORT loc_4_1732
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1732:
        push   0
        dec    dword ptr [esi + 1c9h]
        push   25h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1742:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_174B:
        mov    ebx, 103h
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1760:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    SHORT loc_4_17B3
        mov    eax, dword ptr [esi + 21h]
        cmp    dword ptr [edi + 7fh], eax
        jne    SHORT loc_4_1780
        push   edi
        call   sub_4_0DD0
loc_4_1780:
        cmp    dword ptr [edi + 1c5h], 0
        jne    SHORT loc_4_1798
        push   edi
        lea    eax, [edi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1798:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17A3:
        push   0
        push   25h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17AD:
        dec    dword ptr [edi + 1c9h]
loc_4_17B3:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_17C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        cmp    byte ptr [eax + 4], 0
        je     SHORT loc_4_17DD
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17DD:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_17EE
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17EE:
        push   40010004h
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 1eah], eax
        call   sub_4_1980
        or     dword ptr [esi + 47h], 1
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        je     SHORT loc_4_1837
loc_4_180C:
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
loc_4_182A:
        push   edi
        call   sub_4_1760
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        jne    SHORT loc_4_180C
loc_4_1837:
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_1863
loc_4_183E:
        mov    eax, dword ptr [ecx + 24h]
        push   0c001000ch
        push   ecx
        mov    dword ptr [esi + 38h], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1856:
        push   edi
        call   sub_4_1760
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        jne    SHORT loc_4_183E
loc_4_1863:
        mov    byte ptr [esi + 1ffh], 1
        push   esi
        call   sub_4_19E0
        test   al, al
        je     SHORT loc_4_188B
        push   esi
        call   sub_4_18A0
        push   esi
        call   sub_4_1A10
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_188B:
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 1ffh], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_18A0:
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
        call   sub_4_2660
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2840
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28F0
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2920
        test   eax, eax
        jge    SHORT loc_4_18F0
        or     dword ptr [esi + 47h], 2
loc_4_18F0:
        push   40010005h
        push   esi
        call   sub_4_1980
        mov    eax, dword ptr [esi + 47h]
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 47h], eax
        test   al, 2
        je     SHORT loc_4_190F
        push   80010004h
        jmp    SHORT loc_4_1911
loc_4_190F:
        push   0
loc_4_1911:
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_191A:
        push   edi
        call   sub_4_1760
        push   esi
        call   sub_4_1930
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1930:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 1cdh]
        test   ecx, ecx
        je     SHORT loc_4_1976
loc_4_1941:
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
loc_4_196C:
        mov    ecx, dword ptr [esi + 1cdh]
        test   ecx, ecx
        jne    SHORT loc_4_1941
loc_4_1976:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1980:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 1c5h]
        test   edi, edi
        je     SHORT loc_4_19CB
        mov    esi, dword ptr [ebp + 0ch]
loc_4_199A:
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
loc_4_19B6:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19BF:
        push   edi
        call   sub_4_1760
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_4_199A
loc_4_19CB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_4_19F3
        xor    al, al
        pop    ebp
        ret    4
loc_4_19F3:
        mov    byte ptr [eax + 40h], 1
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A06:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1A10:
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
        je     NEAR PTR loc_4_1B3E
loc_4_1A4D:
        cmp    byte ptr [esi + 1ffh], 0
        je     SHORT loc_4_1A5E
        mov    bl, 1
        push   esi
        call   sub_4_18A0
loc_4_1A5E:
        cmp    byte ptr [esi + 44h], 0
        je     SHORT loc_4_1AA1
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2840
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28F0
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2920
        test   eax, eax
        jge    SHORT loc_4_1A9F
        or     dword ptr [esi + 47h], 2
loc_4_1A9F:
        mov    bl, 1
loc_4_1AA1:
        cmp    byte ptr [esi + 43h], 0
        je     SHORT loc_4_1ACC
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 1b9h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2920
        test   eax, eax
        jge    SHORT loc_4_1ACA
        or     dword ptr [esi + 47h], 2
loc_4_1ACA:
        mov    bl, 1
loc_4_1ACC:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_1AEA
        mov    byte ptr [esi + 46h], 0
loc_4_1AD6:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_1AE8
        push   0
        push   esi
        call   sub_4_10E0
        test   eax, eax
        je     SHORT loc_4_1AD6
loc_4_1AE8:
        mov    bl, 1
loc_4_1AEA:
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_4_1AF8
        push   esi
        mov    bl, 1
        call   sub_4_34A0
loc_4_1AF8:
        cmp    byte ptr [esi + 41h], 0
        je     SHORT loc_4_1B0B
        mov    byte ptr [esi + 41h], 0
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B0B:
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
        jne    NEAR PTR loc_4_1A4D
loc_4_1B3E:
        mov    byte ptr [esi + 40h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1B60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_31F0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 0B1h, 016h, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1BA0:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 14h
        mov    word ptr [ebp - 14h], ax
        push   ebx
        mov    word ptr [ebp - 12h], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0ch], bx
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 0ch]
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 0ah], 1
        mov    word ptr [ebp - 8], 63cah
        push   ecx
        push   eax
        db  068h, 00Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1BE0:
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
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
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
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
        call   sub_4_04C0
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
        jne    NEAR PTR loc_4_2095
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
        mov    edi, 3
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 205h]
        mov    dword ptr [ebp - 3eh], edi
        mov    dword ptr [ebp - 3ah], ebx
        mov    dword ptr [ebp - 2eh], eax
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
        mov    dword ptr [ebp - 2ah], 1fh
        push   esi
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E21:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_4_208F
        mov    eax, dword ptr [esi + 209h]
        cmp    eax, -1
        je     SHORT loc_4_1E9E
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
        mov    dword ptr [ebp - 19h], edi
        mov    dword ptr [ebp - 15h], eax
        mov    eax, dword ptr [esi + 201h]
        lea    ecx, [ebp - 4]
        mov    byte ptr [ebp - 20h], 1
        mov    dword ptr [ebp - 1dh], 2
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E77:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_1E9E
        mov    eax, dword ptr [esi + 209h]
        mov    ecx, dword ptr [esi + 201h]
        push   eax
        push   1
        push   0aa000001h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E96:
        add    esp, 10h
        jmp    NEAR PTR loc_4_2083
loc_4_1E9E:
        mov    ecx, dword ptr [esi + 205h]
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [esi + 209h]
        push   edx
        mov    ebx, dword ptr [esi + 21dh]
        push   ebx
        lea    eax, [ecx + 1fh]
        push   ebx
        mov    edx, dword ptr [esi + 73h]
        push   eax
        push   ecx
        mov    ecx, dword ptr [esi + 211h]
        mov    eax, dword ptr [esi]
        push   ecx
        push   edx
        push   esi
        push   eax
        call   sub_4_23E0
        test   eax, eax
        jge    SHORT loc_4_1F04
        mov    eax, dword ptr [esi + 205h]
        mov    ecx, dword ptr [esi + 211h]
        push   eax
        mov    edx, dword ptr [esi + 201h]
        push   ecx
        push   2
        push   0aa000004h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EF5:
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 14h
        jmp    NEAR PTR loc_4_2071
loc_4_1F04:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 20h], eax
loc_4_1F15:
        lea    eax, [esi + ecx]
        mov    edx, dword ptr [esi + 73h]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 6dh], dl
        mov    byte ptr [eax + 67h], dl
        jb     SHORT loc_4_1F15
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
loc_4_1F52:
        test   al, al
        jne    SHORT loc_4_1F62
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_4_2071
loc_4_1F62:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2840
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28F0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2E20
        mov    edi, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F91:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_1FA4
        mov    dword ptr [ebp - 4], 0c000009ah
        jmp    NEAR PTR loc_4_2069
loc_4_1FA4:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E40
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
loc_4_1FD6:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3280
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 1bdh], eax
        mov    dword ptr [esi + 1b9h], 0
        call   sub_4_2920
        test   eax, eax
        jge    SHORT loc_4_2006
        or     dword ptr [esi + 47h], 2
        jmp    SHORT loc_4_2069
loc_4_2006:
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
loc_4_203E:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_20A1
        mov    eax, dword ptr [esi + 211h]
        mov    ecx, dword ptr [esi + 201h]
        push   eax
        push   1
        push   0aa000005h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_205E:
        add    esp, 10h
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2740
loc_4_2069:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2071:
        cmp    dword ptr [esi + 209h], -1
        je     SHORT loc_4_2083
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2083:
        mov    eax, dword ptr [esi + 201h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_208F:
        push   esi
        call   sub_4_2200
loc_4_2095:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_20A1:
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
loc_4_210D:
        mov    ecx, dword ptr [esi + 1c1h]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
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
        call   sub_4_2740
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2660
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
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0ch
        mov    ax, word ptr [ebp + 18h]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], cx
        mov    edi, dword ptr [ebp + 10h]
        mov    word ptr [esi + 2], ax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 10h], edi
        mov    dword ptr [edi], ecx
        mov    word ptr [edi + 4], cx
loc_4_240B:
        mov    eax, dword ptr [ebp + 18h]
        add    eax, ecx
        inc    ecx
        cmp    ecx, 20h
        mov    dword ptr [esi + ecx*4 + 3ch], eax
        jb     SHORT loc_4_240B
        xor    ecx, ecx
        db  0B8h, 000h, 000h, 000h, 000h
loc_4_2421:
        xor    edx, edx
        mov    dx, word ptr [eax]
        cmp    edx, dword ptr [ebp + 18h]
        je     SHORT loc_4_2436
        add    eax, 2
        inc    ecx
        db  03Dh, 010h, 000h, 000h, 000h
        jb     SHORT loc_4_2421
loc_4_2436:
        cmp    ecx, 8
        jae    NEAR PTR loc_4_2643
        mov    eax, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp + 18h]
        cmp    eax, 1fh
        jb     NEAR PTR loc_4_2643
        xor    eax, eax
        mov    cl, byte ptr [ebp + 14h]
loc_4_2453:
        db  038h, 088h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_2461
        inc    eax
        cmp    eax, 8
        jb     SHORT loc_4_2453
loc_4_2461:
        cmp    eax, 8
        jb     SHORT loc_4_247D
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        pop    esi
        pop    ebx
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_247D:
        mov    eax, dword ptr [esi + 4]
        xor    ebx, ebx
        push   eax
        call   0
        mov    dword ptr [ebp - 8], 1
loc_4_248F:
        mov    eax, ebx
        mov    ecx, dword ptr [esi + 4]
        or     al, 8
        push   eax
        push   96h
        push   ecx
        call   sub_4_0280
        push   0ffh
        mov    ecx, dword ptr [esi + 4]
        push   94h
        push   ecx
        call   sub_4_0280
        push   100h
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        cmp    al, 0cah
        jne    SHORT loc_4_2501
        push   101h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        cmp    al, 63h
        jne    SHORT loc_4_2501
        push   102h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        xor    ecx, ecx
        xor    edx, edx
        mov    byte ptr [ebp - 1], al
        and    al, 0efh
        shr    al, 4
        mov    cl, al
        db  066h, 08Bh, 091h, 000h, 000h, 000h, 000h
        cmp    edx, dword ptr [ebp + 18h]
        je     SHORT loc_4_2509
loc_4_2501:
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_4_248F
        jmp    SHORT loc_4_2532
loc_4_2509:
        mov    al, byte ptr [ebp - 1]
        xor    ecx, ecx
        and    al, 0eh
        xor    edx, edx
        shr    al, 1
        mov    cl, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        cmp    edx, dword ptr [ebp + 14h]
        je     SHORT loc_4_2532
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [ebp - 8], 0ffffffffh
        db  0C7h, 000h, 000h, 000h, 000h, 000h
loc_4_2532:
        cmp    ebx, 8
        jae    NEAR PTR loc_4_260E
        cmp    dword ptr [ebp - 8], 1
        jne    NEAR PTR loc_4_2609
        mov    al, byte ptr [ebp - 1]
        or     al, 1
        mov    byte ptr [ebp - 0ch], al
        mov    byte ptr [esi + 0bh], al
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0ch], bl
        and    al, 0feh
        mov    ecx, dword ptr [esi + 4]
        push   eax
        mov    ebx, 7
        push   102h
        push   ecx
        call   sub_4_0280
loc_4_256C:
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    SHORT loc_4_256C
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   102h
        push   ecx
        xor    ebx, ebx
        call   sub_4_0280
        push   102h
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
loc_4_259C:
        mov    eax, dword ptr [esi + 40h]
        mov    ecx, dword ptr [esi + 4]
        add    eax, ebx
        inc    edi
        push   eax
        inc    ebx
        push   ecx
        call   sub_4_0260
        mov    byte ptr [edi - 1], al
        cmp    ebx, 6
        jb     SHORT loc_4_259C
        mov    eax, dword ptr [esi + 5ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    byte ptr [esi + 0ah], al
        mov    eax, 100h
        mov    word ptr [esi + 8], 1
        mov    byte ptr [esi + 0eh], 40h
        mov    byte ptr [esi + 0fh], 0ffh
        mov    word ptr [esi + 0c0h], ax
        mov    ecx, dword ptr [ebp + 2ch]
        mov    dword ptr [esi + 1ceh], eax
        mov    dword ptr [esi + 1c6h], 4000h
        mov    dword ptr [esi + 1cah], 0c000h
        mov    word ptr [esi], 2
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_261E
loc_4_2609:
        cmp    ebx, 8
        jb     SHORT loc_4_261E
loc_4_260E:
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [ebp - 8], 0ffffffffh
        db  0C7h, 000h, 000h, 000h, 000h, 000h
loc_4_261E:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   96h
        push   eax
        call   sub_4_0280
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_2643:
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        pop    esi
        pop    ebx
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        ret    28h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2660:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 32h], ax
        mov    word ptr [esi + 34h], ax
        mov    word ptr [esi], 2
        mov    al, byte ptr [esi + 0ch]
        mov    ecx, dword ptr [esi + 4]
        or     al, 8
        push   eax
        push   96h
        push   ecx
        call   sub_4_0280
        push   0ffh
        mov    ecx, dword ptr [esi + 4]
        push   94h
        push   ecx
        call   sub_4_0280
        push   102h
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        mov    bl, al
        mov    ecx, dword ptr [esi + 4]
        and    al, 0feh
        mov    edi, 7
        push   eax
        push   102h
        push   ecx
        call   sub_4_0280
loc_4_26C5:
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    SHORT loc_4_26C5
        or     bl, 1
        mov    eax, dword ptr [esi + 4]
        push   ebx
        push   102h
        push   eax
        call   sub_4_0280
        push   102h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   0
        mov    eax, dword ptr [esi + 4]
        push   96h
        push   eax
        call   sub_4_0280
        push   21h
        mov    eax, dword ptr [esi + 80h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2711:
        add    esp, 8
        mov    eax, dword ptr [esi + 80h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        push   esi
        call   sub_4_31F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2740:
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
        mov    byte ptr [ebp - 1], al
        push   ecx
        mov    edi, 1f3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_277E:
        mov    word ptr [esi], 4
        add    esp, 8
        push   esi
        call   sub_4_31F0
loc_4_278C:
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    SHORT loc_4_278C
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0a8h]
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_27A9:
        add    esp, 8
        mov    eax, dword ptr [esi + 0ach]
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_27B9:
        add    esp, 8
        mov    eax, dword ptr [esi + 0b4h]
        push   2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_27CA:
        add    esp, 8
        mov    eax, dword ptr [esi + 9ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0ah
        je     SHORT loc_4_282C
        push   0ah
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_27EF:
        mov    word ptr [esi + 30h], bx
        add    esp, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 39h]
        mov    cl, byte ptr [esi + eax*4 + 0c7h]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 39h], cl
        mov    byte ptr [eax + 0c7h], bl
        mov    word ptr [eax + 0c8h], bx
        mov    al, byte ptr [esi + 38h]
        test   al, al
        je     SHORT loc_4_2821
        dec    al
        mov    byte ptr [esi + 38h], al
loc_4_2821:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_282C:
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2840:
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
        je     SHORT loc_4_2872
loc_4_285D:
        mov    eax, dword ptr [edi + 10h]
        add    eax, esi
        add    esi, 6
        add    eax, 6
        push   eax
        push   edi
        call   sub_4_2880
        dec    ebx
        jne    SHORT loc_4_285D
loc_4_2872:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2880:
        push   ebp
        mov    ecx, 0ffffffffh
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
loc_4_2898:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    eax, 8
loc_4_28A0:
        mov    edi, ecx
        mov    bl, dl
        shr    dl, 1
        and    edi, 80000000h
        shr    edi, 1fh
        and    ebx, 1
        xor    edi, ebx
        add    ecx, ecx
        test   edi, edi
        je     SHORT loc_4_28C3
        xor    ecx, 4c11db6h
        or     ecx, edi
loc_4_28C3:
        dec    eax
        jne    SHORT loc_4_28A0
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_4_2898
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
sub_4_28F0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 30h], cx
        pop    ebp
        mov    word ptr [eax + 32h], cx
        mov    byte ptr [eax + 38h], cl
        mov    byte ptr [eax + 39h], cl
        mov    byte ptr [eax + 3ah], cl
        mov    byte ptr [eax + 3bh], cl
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2920:
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
        je     SHORT loc_4_2945
        mov    bl, 4
loc_4_2945:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 4
        mov    dword ptr [ebp - 4], eax
        jne    SHORT loc_4_2956
        test   byte ptr [ebp + 0ch], 8
        je     SHORT loc_4_2959
loc_4_2956:
        or     bl, 8
loc_4_2959:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 10h
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_4_2967
        or     bl, 1ch
loc_4_2967:
        test   byte ptr [ebp + 0ch], 20h
        je     SHORT loc_4_2970
        or     bl, 3
loc_4_2970:
        push   21h
        mov    eax, dword ptr [esi + 80h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_297E:
        add    esp, 8
        push   esi
        call   sub_4_31F0
        mov    ax, word ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0b8h]
        and    ax, 1
        cmp    ax, 1
        sbb    al, al
        add    al, 49h
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29A4:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0a8h]
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29B5:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0ach]
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29C6:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0b0h]
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29D6:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0b4h]
        push   2
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29E7:
        add    esp, 8
        mov    cl, byte ptr [esi + 0eh]
        mov    edx, dword ptr [esi + 84h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29FA:
        add    esp, 8
        mov    cl, byte ptr [esi + 0eh]
        mov    edx, dword ptr [esi + 8ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2A0D:
        add    esp, 8
        mov    cl, byte ptr [esi + 0fh]
        mov    edx, dword ptr [esi + 88h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2A20:
        add    esp, 8
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_2A6F
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
loc_4_2A6F:
        push   0ffh
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2A80:
        add    esp, 8
        mov    eax, dword ptr [esi + 80h]
        xor    ebx, ebx
        push   61h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2A93:
        add    esp, 8
        mov    edi, dword ptr [esi + 10h]
loc_4_2A99:
        mov    al, byte ptr [edi]
        inc    edi
        push   eax
        mov    eax, dword ptr [esi + 84h]
        add    ax, bx
        inc    ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2AAD:
        add    esp, 8
        cmp    ebx, 6
        jl     SHORT loc_4_2A99
        test   byte ptr [ebp + 0ch], 8
        jne    SHORT loc_4_2AEA
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_2AEA
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_2B09
        lea    edi, [esi + 18h]
        xor    ebx, ebx
loc_4_2ACC:
        mov    al, byte ptr [edi]
        inc    edi
        push   eax
        mov    eax, dword ptr [esi + 0a0h]
        add    ax, bx
        inc    ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2AE0:
        add    esp, 8
        cmp    ebx, 8
        jl     SHORT loc_4_2ACC
        jmp    SHORT loc_4_2B09
loc_4_2AEA:
        xor    edi, edi
loc_4_2AEC:
        push   0ffh
        mov    eax, dword ptr [esi + 0a0h]
        add    ax, di
        inc    edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B01:
        add    esp, 8
        cmp    edi, 8
        jl     SHORT loc_4_2AEC
loc_4_2B09:
        mov    al, byte ptr [esi + 0eh]
        mov    ecx, dword ptr [esi + 9ch]
        inc    al
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B1B:
        add    esp, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 0eh]
        push   2
        inc    eax
        mov    dword ptr [esi + 24h], eax
        mov    ecx, dword ptr [esi + 80h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B35:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0b4h]
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B46:
        add    esp, 8
        mov    ecx, dword ptr [esi + 80h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 0c2h
        cmp    al, 2
        je     SHORT loc_4_2B6D
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2B6D:
        mov    word ptr [esi], 8
        cmp    byte ptr [esi + 38h], 0
        je     SHORT loc_4_2B7E
        push   esi
        call   sub_4_2C40
loc_4_2B7E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2B90:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [edx], 8
        push   esi
        jne    SHORT loc_4_2BA9
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_2BA9:
        mov    esi, dword ptr [ebp + 14h]
        xor    eax, eax
        add    esi, 0ffh
        mov    al, byte ptr [edx + 3ah]
        shr    esi, 8
        xor    ebx, ebx
        cmp    byte ptr [edx + 38h], 0
        mov    bl, byte ptr [edx + eax*4 + 0c7h]
        je     SHORT loc_4_2C13
        xor    eax, eax
        mov    al, byte ptr [edx + 39h]
        cmp    ebx, eax
        jbe    SHORT loc_4_2BFC
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0eh]
        sub    ecx, ebx
        cmp    ecx, esi
        jae    SHORT loc_4_2BF4
        cmp    esi, eax
        jbe    SHORT loc_4_2BF2
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 1e6h]
        pop    ebp
        ret    10h
loc_4_2BF2:
        xor    ebx, ebx
loc_4_2BF4:
        inc    dword ptr [edx + 1eah]
        jmp    SHORT loc_4_2C1B
loc_4_2BFC:
        sub    eax, ebx
        cmp    eax, esi
        jae    SHORT loc_4_2BF4
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 1e6h]
        pop    ebp
        ret    10h
loc_4_2C13:
        xor    ebx, ebx
        mov    byte ptr [edx + 3ah], bl
        mov    byte ptr [edx + 39h], bl
loc_4_2C1B:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    byte ptr [edx + 3bh], bl
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C2C:
        add    esp, 0ch
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2C40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 39h]
        mov    ecx, dword ptr [esi + 90h]
        push   eax
        xor    edi, edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C5A:
        add    esp, 8
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 39h]
        mov    eax, dword ptr [esi + 94h]
        mov    di, word ptr [esi + ecx*4 + 0c8h]
        push   edi
        push   eax
        shr    edi, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C7A:
        add    esp, 8
        mov    eax, dword ptr [esi + 98h]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C8A:
        add    esp, 8
        mov    eax, dword ptr [esi + 80h]
        push   26h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C9B:
        add    esp, 8
        mov    word ptr [esi + 32h], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2CB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2CBD:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_2D31
        xor    ecx, ecx
        mov    dl, byte ptr [esi + 3bh]
        mov    cx, word ptr [esi + 36h]
        lea    eax, [ecx + 0ffh]
        shr    eax, 8
        add    al, dl
        cmp    byte ptr [esi + 0eh], al
        ja     SHORT loc_4_2CDF
        xor    al, al
loc_4_2CDF:
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
        jne    SHORT loc_4_2D1A
        mov    al, byte ptr [esi + 3bh]
        push   esi
        mov    byte ptr [esi + 39h], al
        call   sub_4_2C40
        jmp    SHORT loc_4_2D29
loc_4_2D1A:
        mov    cl, byte ptr [esi + 3bh]
        xor    eax, eax
        mov    al, byte ptr [esi + 3ah]
        mov    byte ptr [esi + eax*4 + 0c7h], cl
loc_4_2D29:
        mov    al, byte ptr [esi + 3bh]
        mov    byte ptr [esi + 3ah], al
        xor    eax, eax
loc_4_2D31:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2D40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     NEAR PTR loc_4_2E0C
        mov    ax, word ptr [esi + 32h]
        test   ax, ax
        jle    NEAR PTR loc_4_2E0C
        dec    ax
        mov    word ptr [esi + 32h], ax
        test   ax, ax
        jg     NEAR PTR loc_4_2E0C
        push   esi
        call   sub_4_2660
        inc    dword ptr [esi + 1eeh]
        mov    ax, word ptr [esi + 30h]
        inc    ax
        mov    word ptr [esi + 30h], ax
        cmp    ax, 2
        jbe    SHORT loc_4_2DBC
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
        je     SHORT loc_4_2DBA
        dec    al
        mov    byte ptr [esi + 38h], al
loc_4_2DBA:
        mov    bl, 10h
loc_4_2DBC:
        mov    eax, dword ptr [esi + 0c2h]
        push   eax
        push   esi
        call   sub_4_2920
        test   eax, eax
        je     SHORT loc_4_2DE9
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
loc_4_2DE9:
        test   bl, bl
        je     SHORT loc_4_2E0C
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10E0
        cmp    eax, -1
        jge    SHORT loc_4_2E0C
loc_4_2DFC:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10E0
        cmp    eax, -1
        jl     SHORT loc_4_2DFC
loc_4_2E0C:
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2E20:
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
sub_4_2E40:
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
sub_4_2E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        inc    dword ptr [esi + 1dah]
        call   sub_4_2740
        push   2
        mov    eax, dword ptr [esi + 0b4h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2E86:
        add    esp, 8
        mov    eax, dword ptr [esi + 80h]
        push   22h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2E97:
        add    esp, 8
        mov    eax, dword ptr [esi + 80h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        and    al, 0c2h
        cmp    al, 2
        je     SHORT loc_4_2ECF
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
loc_4_2ECF:
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
        je     NEAR PTR loc_4_2FA9
loc_4_2EF1:
        mov    ax, word ptr [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2EFB:
        add    esp, 4
        and    eax, 0ffh
        cmp    eax, dword ptr [esi + 24h]
        je     NEAR PTR loc_4_2FA9
        cmp    dword ptr [esi + 0c2h], 0
        mov    eax, 2
        je     SHORT loc_4_2F20
        push   edi
        call   sub_4_32A0
loc_4_2F20:
        cmp    eax, 1
        jne    SHORT loc_4_2F4D
        mov    al, byte ptr [esi + 2dh]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 24h], ecx
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_4_2F3B
        mov    al, byte ptr [esi + 0fh]
        dec    al
loc_4_2F3B:
        push   eax
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2F48:
        add    esp, 8
        jmp    SHORT loc_4_2F9F
loc_4_2F4D:
        cmp    eax, 2
        jne    SHORT loc_4_2F9B
        mov    ax, word ptr [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2F5C:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 24h], ecx
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_4_2F72
        mov    al, byte ptr [esi + 0fh]
        dec    al
loc_4_2F72:
        push   eax
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2F7F:
        add    esp, 8
        mov    eax, dword ptr [esi + 9ch]
        push   5
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2F90:
        add    esp, 8
        inc    dword ptr [esi + 1e2h]
        jmp    SHORT loc_4_2F9F
loc_4_2F9B:
        test   eax, eax
        jl     SHORT loc_4_2FFE
loc_4_2F9F:
        inc    ebx
        cmp    dword ptr [ebp - 4], ebx
        ja     NEAR PTR loc_4_2EF1
loc_4_2FA9:
        push   10h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2FB7:
        add    esp, 8
        mov    eax, dword ptr [esi + 0b4h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2FC8:
        add    esp, 8
        cmp    byte ptr [esi + 38h], 0
        je     SHORT loc_4_2FF1
        push   0ah
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2FDF:
        add    esp, 8
        push   esi
        call   sub_4_2C40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2FF1:
        push   0
        push   edi
        call   sub_4_10E0
        cmp    eax, -1
        jl     SHORT loc_4_2FF1
loc_4_2FFE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3010:
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
loc_4_302C:
        mov    eax, dword ptr [esi + 9ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 10h
        jne    NEAR PTR loc_4_31C2
        push   5
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3052:
        mov    ax, word ptr [esi + 2]
        add    esp, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_305F:
        add    esp, 4
        and    eax, 0ffh
        cmp    eax, dword ptr [esi + 24h]
        je     NEAR PTR loc_4_3110
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 1
        cmp    dword ptr [esi + 0c2h], 0
        mov    eax, 2
        je     SHORT loc_4_308C
        push   edi
        call   sub_4_32A0
loc_4_308C:
        cmp    eax, 1
        jne    SHORT loc_4_30B9
        mov    cl, byte ptr [esi + 2dh]
        xor    eax, eax
        mov    al, cl
        dec    cl
        mov    dword ptr [esi + 24h], eax
        cmp    byte ptr [esi + 0eh], cl
        jbe    SHORT loc_4_30A7
        mov    cl, byte ptr [esi + 0fh]
        dec    cl
loc_4_30A7:
        push   ecx
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30B4:
        add    esp, 8
        jmp    SHORT loc_4_3114
loc_4_30B9:
        cmp    eax, 2
        jne    SHORT loc_4_3107
        mov    ax, word ptr [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30C8:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 24h], ecx
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_4_30DE
        mov    al, byte ptr [esi + 0fh]
        dec    al
loc_4_30DE:
        push   eax
        mov    eax, dword ptr [esi + 8ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30EB:
        add    esp, 8
        mov    eax, dword ptr [esi + 9ch]
        push   5
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30FC:
        add    esp, 8
        inc    dword ptr [esi + 1e2h]
        jmp    SHORT loc_4_3110
loc_4_3107:
        test   eax, eax
        jge    SHORT loc_4_3114
        jmp    NEAR PTR loc_4_31CD
loc_4_3110:
        mov    byte ptr [ebp - 1], 1
loc_4_3114:
        mov    eax, dword ptr [esi + 9ch]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0ah
        je     NEAR PTR loc_4_31B5
        mov    word ptr [esi + 32h], 0
        mov    word ptr [esi + 30h], 0
        push   0ah
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3146:
        add    esp, 8
        mov    eax, dword ptr [esi + 90h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
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
        je     SHORT loc_4_3198
        dec    al
        mov    byte ptr [esi + 38h], al
        je     SHORT loc_4_3198
        push   esi
        call   sub_4_2C40
loc_4_3198:
        push   ebx
        push   edi
        call   sub_4_10E0
        cmp    eax, -1
        jge    SHORT loc_4_31B1
loc_4_31A4:
        push   0
        push   edi
        call   sub_4_10E0
        cmp    eax, -1
        jl     SHORT loc_4_31A4
loc_4_31B1:
        xor    al, al
        jmp    SHORT loc_4_31B7
loc_4_31B5:
        mov    al, 1
loc_4_31B7:
        test   byte ptr [ebp - 1], al
        je     NEAR PTR loc_4_302C
        jmp    SHORT loc_4_31CD
loc_4_31C2:
        push   edi
        push   esi
        call   sub_4_2E60
        mov    byte ptr [ebp - 2], 1
loc_4_31CD:
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
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_31F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_321C
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
loc_4_321C:
        push   0
        mov    eax, dword ptr [esi + 0bch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_322A:
        add    esp, 8
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3240:
        push   ebp
        mov    ebp, esp
        push   esi
        push   1fh
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0bch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3255:
        add    esp, 8
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_327B
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
loc_4_327B:
        pop    esi
        pop    ebp
        ret    4
sub_4_3280:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_3293
        mov    eax, 3fh
        pop    ebp
        ret    8
loc_4_3293:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0c2h]
        ret    8
sub_4_32A0:
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
        jbe    SHORT loc_4_32DD
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_32DD
        push   0
        push   esi
        call   sub_4_10E0
        cmp    eax, -1
        jne    SHORT loc_4_32DD
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_32DD:
        lea    edi, [esi + 0bdh]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0fch
        push   edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_32F5:
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     SHORT loc_4_3346
        add    eax, 3
        cmp    eax, 3
        ja     SHORT loc_4_3346
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_330E:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3318:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3322:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_332F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_3650
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3346:
        sub    dword ptr [ebp - 4], 0eh
        mov    ecx, dword ptr [esi + 7fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     SHORT loc_4_3356
        mov    ecx, eax
loc_4_3356:
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
loc_4_338A:
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
loc_4_33B0:
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
        je     SHORT loc_4_33FB
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
loc_4_33F0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_4_33FB:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [ebp + 10h]
        lea    edi, [ecx + edx]
        cmp    edi, eax
        jbe    SHORT loc_4_3412
        sub    eax, edx
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_4_3415
loc_4_3412:
        mov    dword ptr [ebp - 18h], ecx
loc_4_3415:
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
loc_4_3431:
        test   edi, edi
        je     SHORT loc_4_3487
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3448:
        mov    eax, dword ptr [ebp - 10h]
        cmp    ebx, eax
        jb     SHORT loc_4_3451
        mov    ebx, eax
loc_4_3451:
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
loc_4_3467:
        add    esp, 14h
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     SHORT loc_4_3487
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3481:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_3431
loc_4_3487:
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
sub_4_34A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_34C2
loc_4_34AE:
        mov    ecx, dword ptr [eax + 24h]
        push   eax
        push   esi
        mov    dword ptr [esi + 38h], ecx
        call   sub_4_34D0
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        jne    SHORT loc_4_34AE
loc_4_34C2:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_34D0:
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
loc_4_34EC:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 7fh]
        add    ebx, 0eh
        cmp    ebx, eax
        jb     SHORT loc_4_34FE
        mov    ebx, eax
loc_4_34FE:
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
loc_4_3518:
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
        call   sub_4_35A0
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
loc_4_3567:
        push   0
        inc    dword ptr [esi + 83h]
        mov    byte ptr [esi + 41h], 1
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3580:
        push   esi
        call   sub_4_1760
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_35A0:
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
loc_4_35C6:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 14h]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_3644
loc_4_35D3:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax]
        lea    edx, [ecx + ebx]
        cmp    edx, dword ptr [ebp + 0ch]
        jae    SHORT loc_4_361E
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35F0:
        mov    ecx, dword ptr [ebp + 18h]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35FE:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3614:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_35D3
loc_4_361E:
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_3644
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
loc_4_363D:
        mov    ecx, dword ptr [ebp + 18h]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_3644:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_4_3650:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        test   eax, 208h
        jne    SHORT loc_4_369C
        test   eax, 206h
        je     SHORT loc_4_3678
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        inc    dword ptr [eax + 9bh]
        ret    8
loc_4_3678:
        test   eax, 204h
        je     SHORT loc_4_368C
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        ret    8
loc_4_368C:
        test   eax, 202h
        je     SHORT loc_4_369C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 9bh]
loc_4_369C:
        pop    ebp
        ret    8
_PTEXT ends
        end
