; irmatrac_pcode_05.asm -- IRMATRAC object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  055h, 08Bh, 044h, 024h, 010h, 08Bh, 0ECh, 083h, 0ECh, 008h, 050h, 08Bh, 04Dh, 00Ch, 051h, 08Bh
        db  055h, 008h, 08Dh, 04Dh, 0FCh, 08Bh, 042h, 055h, 050h, 08Dh, 055h, 0F8h, 051h, 052h, 0E8h, 000h
        db  000h, 000h, 000h, 083h, 07Dh, 0F8h, 000h, 074h, 007h, 0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        db  08Bh, 045h, 0FCh, 08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0040:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0a7h]
        cmp    cx, 18h
        jbe    SHORT loc_5_0065
        mov    dword ptr [esi + 187h], 18h
        jmp    SHORT loc_5_0071
loc_5_0065:
        and    ecx, 0ffffh
        mov    dword ptr [esi + 187h], ecx
loc_5_0071:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 187h]
        mov    ax, word ptr [esi + 0abh]
        lea    edx, [eax + ecx + 1]
        lea    eax, [esi + 55h]
        push   edx
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0092:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_00A3
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_00A3:
        lea    edi, [esi + 0afh]
        push   -1
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0a7h]
        lea    ecx, [eax + eax*2]
        lea    edx, [ecx + ecx*8]
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00C3:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_5_00D5
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_00D5:
        xor    edx, edx
        mov    edi, dword ptr [edi]
        mov    dx, word ptr [esi + 0a7h]
        lea    eax, [edx + edx*2]
        lea    edx, [eax + eax*8]
        xor    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        push   -1
        and    ecx, 3
        push   eax
        rep stosb
        xor    eax, eax
        lea    edx, [esi + 206h]
        mov    ax, word ptr [esi + 0abh]
        shl    eax, 2
        lea    ecx, [eax + eax*4]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0115:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_5_0127
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0127:
        xor    eax, eax
        mov    edi, dword ptr [esi + 206h]
        mov    ax, word ptr [esi + 0abh]
        push   -1
        shl    eax, 2
        lea    ecx, [eax + eax*4]
        xor    eax, eax
        shr    ecx, 2
        push   eax
        rep stosd
        xor    eax, eax
        lea    ecx, [esi + 20eh]
        mov    ax, word ptr [esi + 0abh]
        shl    eax, 2
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_015F:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_5_0171
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0171:
        cmp    byte ptr [esi + 243h], 0
        je     NEAR PTR loc_5_02A6
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        mov    ecx, dword ptr [esi + 81h]
        imul   eax, dword ptr [esi + 247h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_01A5:
        test   byte ptr [ebp - 5], 0ffh
        je     SHORT loc_5_01BF
        xor    al, al
        pop    edi
        mov    word ptr [esi + 0abh], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_01BF:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_5_01D9
        xor    al, al
        pop    edi
        mov    word ptr [esi + 0abh], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_01D9:
        xor    ebx, ebx
        cmp    word ptr [esi + 0abh], bx
        jbe    NEAR PTR loc_5_0304
        xor    edi, edi
        mov    dword ptr [ebp - 10h], ebx
loc_5_01ED:
        mov    eax, dword ptr [esi + 247h]
        mov    edx, dword ptr [esi + 20eh]
        imul   eax, ebx
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [edx + ecx], eax
        mov    eax, dword ptr [esi + 247h]
        imul   eax, ebx
        add    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 206h]
        mov    dword ptr [ecx + edi + 8], eax
        mov    eax, dword ptr [esi + 247h]
        imul   eax, ebx
        add    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 206h]
        shr    eax, 10h
        and    ax, 0ffh
        mov    word ptr [ecx + edi], ax
        mov    eax, dword ptr [esi + 247h]
        imul   ax, bx
        add    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 206h]
        mov    word ptr [ecx + edi + 2], ax
        mov    eax, dword ptr [esi + 247h]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        mov    edx, dword ptr [esi + 20eh]
        mov    eax, dword ptr [edx + ecx]
        push   eax
        push   esi
        call   0
        mov    ecx, dword ptr [esi + 206h]
        mov    dword ptr [ecx + edi + 4], eax
        mov    ecx, dword ptr [esi + 206h]
        cmp    dword ptr [ecx + edi + 4], 0
        je     SHORT loc_5_029D
        add    edi, 14h
        inc    ebx
        xor    eax, eax
        add    dword ptr [ebp - 10h], 4
        mov    ax, word ptr [esi + 0abh]
        cmp    eax, ebx
        ja     NEAR PTR loc_5_01ED
        jmp    SHORT loc_5_0304
loc_5_029D:
        mov    word ptr [esi + 0abh], bx
        jmp    SHORT loc_5_0304
loc_5_02A6:
        push   -1
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        push   0
        imul   eax, dword ptr [esi + 247h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02C4:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_5_02D6
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_02D6:
        xor    edx, edx
        cmp    word ptr [esi + 0abh], dx
        jbe    SHORT loc_5_0304
loc_5_02E1:
        mov    eax, dword ptr [esi + 247h]
        mov    ecx, dword ptr [esi + 20eh]
        imul   eax, edx
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + edx*4], eax
        inc    edx
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        cmp    eax, edx
        ja     SHORT loc_5_02E1
loc_5_0304:
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_0370
        lea    edi, [esi + 0bfh]
        lea    eax, [esi + 0bbh]
        push   eax
        push   edi
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0a7h]
        mov    ecx, dword ptr [esi + 81h]
        shl    eax, 8
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0336:
        mov    eax, dword ptr [edi]
        test   eax, eax
        jne    SHORT loc_5_0347
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0347:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0a7h]
        shl    ecx, 8
        push   ecx
        push   eax
        push   esi
        call   0
        mov    dword ptr [esi + 0c3h], eax
        test   eax, eax
        jne    SHORT loc_5_039F
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0370:
        push   -1
        xor    eax, eax
        mov    ax, word ptr [esi + 0a7h]
        push   0
        shl    eax, 8
        lea    ecx, [esi + 0bfh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_038D:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_5_039F
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_039F:
        cmp    byte ptr [esi + 243h], 0
        je     NEAR PTR loc_5_0468
        xor    ebx, ebx
        cmp    dword ptr [esi + 187h], ebx
        jbe    SHORT loc_5_041D
        lea    edi, [esi + 18bh]
loc_5_03BC:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        xor    edx, edx
        mov    dx, word ptr [esi + 245h]
        push   ecx
        push   1
        mov    eax, dword ptr [esi + 81h]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03DC:
        test   byte ptr [ebp - 5], 0ffh
        jne    SHORT loc_5_044A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_0455
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        xor    edx, edx
        mov    dword ptr [edi], eax
        mov    dword ptr [edi - 0c4h], ecx
        mov    dx, word ptr [esi + 245h]
        mov    eax, dword ptr [edi]
        push   edx
        push   eax
        push   esi
        call   0
        mov    dword ptr [edi - 64h], eax
        cmp    dword ptr [edi], 0
        je     SHORT loc_5_045D
        add    edi, 4
        inc    ebx
        cmp    dword ptr [esi + 187h], ebx
        ja     SHORT loc_5_03BC
loc_5_041D:
        cmp    byte ptr [esi + 243h], 0
        lea    edi, [esi + 21dh]
        je     NEAR PTR loc_5_04B8
        lea    eax, [esi + 221h]
        mov    ecx, dword ptr [esi + 81h]
        push   eax
        push   edi
        push   0
        push   6
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0448:
        jmp    SHORT loc_5_04C7
loc_5_044A:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0455:
        mov    dword ptr [esi + 187h], ebx
        jmp    SHORT loc_5_041D
loc_5_045D:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0468:
        xor    edi, edi
        cmp    dword ptr [esi + 187h], edi
        jbe    SHORT loc_5_041D
        lea    ebx, [esi + 18bh]
loc_5_0478:
        push   -1
        xor    eax, eax
        mov    ax, word ptr [esi + 245h]
        push   0
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_048F:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        jne    SHORT loc_5_04AD
        mov    eax, dword ptr [ebp - 4]
        add    ebx, 4
        inc    edi
        mov    dword ptr [ebx - 4], eax
        cmp    dword ptr [esi + 187h], edi
        ja     SHORT loc_5_0478
        jmp    NEAR PTR loc_5_041D
loc_5_04AD:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_04B8:
        push   -1
        push   0
        push   6
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04C4:
        mov    dword ptr [ebp - 0ch], eax
loc_5_04C7:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_5_04D7
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_04D7:
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_050E
        lea    edi, [esi + 0b3h]
        lea    eax, [esi + 0b7h]
        push   eax
        mov    ecx, dword ptr [esi + 81h]
        push   edi
        push   1
        push   1ah
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04FE:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_5_0532
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_050E:
        push   -1
        lea    eax, [esi + 0b3h]
        push   0
        push   1ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0520:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_5_0532
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0532:
        cmp    byte ptr [esi + 243h], 0
        lea    edi, [esi + 25bh]
        je     SHORT loc_5_055B
        lea    eax, [esi + 25fh]
        mov    ecx, dword ptr [esi + 81h]
        push   eax
        push   edi
        push   0
        push   0dh
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0559:
        jmp    SHORT loc_5_0567
loc_5_055B:
        push   -1
        push   0
        push   0dh
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0567:
        cmp    dword ptr [edi], 1
        pop    edi
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0580:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 55h]
        test   eax, eax
        je     SHORT loc_5_0599
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0599:
        mov    eax, dword ptr [esi + 0afh]
        test   eax, eax
        je     SHORT loc_5_05BB
        push   0
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0a7h]
        lea    edx, [ecx + ecx*2]
        lea    ebx, [edx + edx*8]
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05BB:
        mov    ecx, dword ptr [esi + 18bh]
        test   ecx, ecx
        je     SHORT loc_5_0634
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_05F7
        mov    eax, dword ptr [esi + 0c7h]
        push   eax
        push   ecx
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 245h]
        mov    ecx, dword ptr [esi + 81h]
        imul   eax, dword ptr [esi + 187h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05F5:
        jmp    SHORT loc_5_0610
loc_5_05F7:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 245h]
        imul   eax, dword ptr [esi + 187h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0610:
        xor    edi, edi
        cmp    dword ptr [esi + 187h], edi
        jbe    SHORT loc_5_0634
        lea    ebx, [esi + 127h]
loc_5_0620:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        push   eax
        inc    edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_062C:
        cmp    dword ptr [esi + 187h], edi
        ja     SHORT loc_5_0620
loc_5_0634:
        mov    ecx, dword ptr [esi + 0bfh]
        test   ecx, ecx
        je     SHORT loc_5_0681
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_066C
        mov    eax, dword ptr [esi + 0bbh]
        push   eax
        push   ecx
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0a7h]
        mov    ecx, dword ptr [esi + 81h]
        shl    eax, 8
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_066A:
        jmp    SHORT loc_5_0681
loc_5_066C:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0a7h]
        shl    eax, 8
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0681:
        mov    eax, dword ptr [esi + 0c3h]
        test   eax, eax
        je     SHORT loc_5_0691
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0691:
        xor    edi, edi
        cmp    dword ptr [esi + 206h], edi
        je     NEAR PTR loc_5_076F
        mov    ax, word ptr [esi + 0abh]
        test   ax, ax
        je     NEAR PTR loc_5_076F
        jbe    SHORT loc_5_06D8
        xor    ebx, ebx
loc_5_06B3:
        mov    eax, dword ptr [esi + 206h]
        mov    eax, dword ptr [eax + ebx + 4]
        test   eax, eax
        je     SHORT loc_5_06C7
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06C7:
        add    ebx, 14h
        inc    edi
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        cmp    eax, edi
        ja     SHORT loc_5_06B3
loc_5_06D8:
        mov    eax, dword ptr [esi + 20eh]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     SHORT loc_5_0736
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_071B
        mov    eax, dword ptr [esi + 206h]
        mov    ecx, dword ptr [eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 247h]
        push   1
        mov    edx, dword ptr [esi + 81h]
        imul   ecx, eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0719:
        jmp    SHORT loc_5_0736
loc_5_071B:
        push   0
        mov    eax, dword ptr [esi + 247h]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0abh]
        imul   eax, ecx
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0736:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        mov    ecx, dword ptr [esi + 20eh]
        shl    eax, 2
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0751:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0abh]
        mov    edx, dword ptr [esi + 206h]
        shl    eax, 2
        lea    ecx, [eax + eax*4]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_076F:
        mov    eax, dword ptr [esi + 0b3h]
        test   eax, eax
        je     SHORT loc_5_07A6
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_079C
        mov    ecx, dword ptr [esi + 0b7h]
        push   ecx
        push   eax
        push   0
        mov    eax, dword ptr [esi + 81h]
        push   1ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_079A:
        jmp    SHORT loc_5_07A6
loc_5_079C:
        push   0
        push   1ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07A6:
        mov    eax, dword ptr [esi + 21dh]
        test   eax, eax
        je     SHORT loc_5_07DD
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_07D3
        mov    ecx, dword ptr [esi + 221h]
        push   ecx
        push   eax
        push   0
        mov    eax, dword ptr [esi + 81h]
        push   6
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07D1:
        jmp    SHORT loc_5_07DD
loc_5_07D3:
        push   0
        push   6
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07DD:
        mov    eax, dword ptr [esi + 25bh]
        test   eax, eax
        je     SHORT loc_5_081B
        cmp    byte ptr [esi + 243h], 0
        je     SHORT loc_5_0811
        mov    ecx, dword ptr [esi + 25fh]
        push   ecx
        push   eax
        push   0
        mov    eax, dword ptr [esi + 81h]
        push   0dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0808:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0811:
        push   0
        push   0dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_081B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 034h, 045h, 06Eh, 068h
_PTEXT ends
        end
