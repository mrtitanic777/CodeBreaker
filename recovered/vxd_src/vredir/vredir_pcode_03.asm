; vredir_pcode_03.asm -- VREDIR object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        and    word ptr [ebx + 18h], 0ffh
        mov    ecx, dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 10h], ecx
        cmp    eax, 8
        je     NEAR PTR loc_3_0511
        test   al, 80h
        jne    NEAR PTR loc_3_0511
        inc    dword ptr [ecx + 55h]
        cmp    byte ptr [ecx + 14h], 4
        jne    SHORT loc_3_005C
        mov    dword ptr [ebp - 4], 0
        push   0
        push   ebx
        call   sub_3_0800
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    NEAR PTR loc_3_00F2
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_005C:
        push   1
        push   ebx
        call   sub_3_0590
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_0079
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0079:
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 18h]
        inc    byte ptr [edi + 2bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0088:
        mov    ax, word ptr [ebx + 18h]
        add    esp, 4
        and    al, 0fh
        cmp    al, 1
        jne    SHORT loc_3_00A9
        mov    edi, dword ptr [ebp - 4]
        push   edi
        push   ebx
        call   sub_3_0770
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_3_00F2
loc_3_00A9:
        mov    edi, dword ptr [ebp - 4]
        push   edi
        push   ebx
        call   sub_3_0800
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_3_00F2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_3_00E7
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00CF:
        add    esp, 4
        mov    edi, dword ptr [ebp - 4]
        dec    byte ptr [edi + 2bh]
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_3_00E7
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00E4:
        add    esp, 4
loc_3_00E7:
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_00F2:
        mov    ecx, dword ptr [ebp - 8]
        cmp    word ptr [ecx + 35h], 0ffffh
        jne    NEAR PTR loc_3_0417
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0106:
        mov    edx, 1
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [eax + 14h]
        cmp    cl, dl
        jne    SHORT loc_3_0126
        mov    esi, dword ptr [eax + 10h]
        test   byte ptr [esi + 1ah], 80h
        je     SHORT loc_3_0126
        mov    edx, 7
loc_3_0126:
        cmp    cl, 4
        je     NEAR PTR loc_3_0398
        mov    eax, dword ptr [ebp - 10h]
        mov    esi, dword ptr [eax + 10h]
        mov    ax, word ptr [esi + 1ah]
        test   al, 20h
        je     SHORT loc_3_0145
        test   al, 4
        je     NEAR PTR loc_3_0398
loc_3_0145:
        cmp    cl, 2
        jne    SHORT loc_3_0192
        xor    eax, eax
        mov    al, byte ptr [ebx + 5]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_015F
        add    eax, 13h
        jmp    SHORT loc_3_0164
loc_3_015F:
        db  0A1h, 000h, 000h, 000h, 000h
loc_3_0164:
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 2ah]
        push   eax
        xor    eax, eax
        or     byte ptr [ecx + 34h], 8
        mov    ax, word ptr [ebx + 28h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0c0h
        push   ebx
        call   sub_3_3DA0
        add    esp, 18h
        jmp    NEAR PTR loc_3_041D
loc_3_0192:
        xor    eax, eax
        mov    ax, word ptr [ebx + 18h]
        mov    word ptr [ebx + 18h], 1
        mov    dword ptr [ebp - 14h], eax
loc_3_01A1:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + 31h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 14h]
        dec    eax
        cmp    eax, 11h
        ja     SHORT loc_3_01C4
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_3_01C4:
        mov    word ptr [ebx + 1ah], 1
        jmp    NEAR PTR loc_3_0319
loc_3_01CF:
        test   byte ptr [ebp - 0ch], 7
        jne    SHORT loc_3_01D9
        or     dword ptr [ebp - 0ch], 2
loc_3_01D9:
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   word ptr [ecx + 1ah], 108h
        je     SHORT loc_3_0212
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_3_0212
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    esi, dword ptr [ebx + 28h]
        lea    edi, [ecx + eax]
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    edi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_0212:
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   16h
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   ebx
        call   sub_3_3DA0
        add    esp, 18h
        cmp    eax, 2
        je     SHORT loc_3_027A
        cmp    dword ptr [ebp - 14h], 2
        jne    NEAR PTR loc_3_0319
        cmp    word ptr [ebx + 1ah], 0
        jne    NEAR PTR loc_3_041D
        push   0
        mov    eax, dword ptr [ebx + 24h]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0bh
        push   ebx
        call   sub_3_3DA0
        mov    word ptr [ebx + 18h], 3
        add    esp, 1ch
        jmp    NEAR PTR loc_3_0319
loc_3_0273:
        mov    edx, 0fh
        jmp    SHORT loc_3_0289
loc_3_027A:
        cmp    dword ptr [ebp - 14h], 11h
        jne    NEAR PTR loc_3_0319
loc_3_0284:
        mov    edx, 3
loc_3_0289:
        cmp    dword ptr [ebp - 4], 0
        mov    word ptr [ebx + 18h], 2
        je     SHORT loc_3_02BF
        mov    edi, dword ptr [ebp - 4]
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_3_02A2
        or     byte ptr [edi + 2ah], 1
loc_3_02A2:
        mov    eax, dword ptr [ebx]
        mov    edi, dword ptr [ebp - 4]
        mov    word ptr [edi + 28h], ax
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [edi + 30h], 0
        mov    dword ptr [edi + 38h], 0
        mov    dword ptr [edi + 2ch], ecx
loc_3_02BF:
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   word ptr [ecx + 1ah], 108h
        je     SHORT loc_3_02F8
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_3_02F8
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    esi, dword ptr [ebx + 28h]
        lea    edi, [ecx + eax]
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    edi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_02F8:
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        push   eax
        mov    esi, dword ptr [ebx]
        push   ecx
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   ebx
        call   sub_3_3DA0
        mov    dword ptr [ebp - 0ch], 2
        add    esp, 18h
loc_3_0319:
        cmp    word ptr [ebx + 1ah], 0
        jne    NEAR PTR loc_3_041D
        mov    ecx, dword ptr [ebp - 8]
        mov    dl, byte ptr [ecx + 31h]
        test   dl, 8
        jne    SHORT loc_3_0382
        mov    al, dl
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_3_033D
        and    dl, 0fch
        mov    byte ptr [ecx + 31h], dl
loc_3_033D:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + 31h]
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_3_041D
        push   -1
        xor    eax, eax
        mov    ax, word ptr [ecx + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4
        push   ebx
        call   sub_3_3DA0
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], 1
        mov    word ptr [ecx + 35h], 0ffffh
        mov    word ptr [ebx + 1ah], 0
        jmp    NEAR PTR loc_3_01A1
loc_3_0382:
        cmp    dword ptr [ebp - 0ch], 2
        jne    NEAR PTR loc_3_041D
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 31h], 2
        jmp    NEAR PTR loc_3_041D
loc_3_0398:
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   word ptr [ecx + 1ah], 108h
        je     SHORT loc_3_03D1
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_3_03D1
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    esi, dword ptr [ebx + 28h]
        lea    edi, [ecx + eax]
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    edi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_03D1:
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebx + 24h]
        push   eax
        push   ecx
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ebx + 18h]
        mov    esi, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        xor    eax, eax
        push   16h
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + 31h]
        push   eax
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   2dh
        push   ebx
        call   sub_3_3DA0
        add    esp, 2ch
        cmp    eax, 2
        jne    SHORT loc_3_041D
        cmp    word ptr [ebx + 18h], 10h
        jne    SHORT loc_3_041D
        mov    word ptr [ebx + 1ah], 50h
        jmp    SHORT loc_3_041D
loc_3_0417:
        mov    word ptr [ebx + 18h], 1
loc_3_041D:
        cmp    dword ptr [ebp - 4], 0
        movsx  esi, word ptr [ebx + 1ah]
        je     SHORT loc_3_0462
        mov    edi, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi + 2ah]
        test   cl, 1
        jne    SHORT loc_3_043B
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_044D
loc_3_043B:
        and    cl, 0feh
        mov    edi, dword ptr [ebp - 4]
        push   edi
        mov    byte ptr [edi + 2ah], cl
        call   sub_3_0B40
        add    esp, 4
loc_3_044D:
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0459:
        add    esp, 4
        mov    edi, dword ptr [ebp - 4]
        dec    byte ptr [edi + 2bh]
loc_3_0462:
        test   esi, esi
        je     SHORT loc_3_047D
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_046C:
        mov    word ptr [ebx + 1ah], si
        add    esp, 4
        movsx  eax, si
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_047D:
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        mov    al, byte ptr [ecx + 31h]
        mov    esi, dword ptr [edi + 30h]
        mov    byte ptr [ebx + 4], al
        mov    dword ptr [ebx + 24h], esi
        xor    eax, eax
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [edi + 28h]
        mov    dword ptr [ebx], eax
        je     SHORT loc_3_04B3
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_04A5:
        shl    eax, 0ch
        cmp    eax, esi
        jbe    SHORT loc_3_04B3
        mov    ecx, dword ptr [ebp - 8]
        or     byte ptr [ecx + 34h], 80h
loc_3_04B3:
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 2ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_04BF:
        add    esp, 4
        mov    esi, dword ptr [ebx + 10h]
        mov    dword ptr [ebx + 28h], eax
        test   esi, esi
        je     SHORT loc_3_0506
        xor    edx, edx
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dl, byte ptr [eax + 14h]
        mov    al, byte ptr [ecx + 31h]
        xor    ecx, ecx
        and    al, 3
        mov    cl, al
        lea    edi, [ecx + edx*4]
        db  08Bh, 00Ch, 0BDh, 000h, 000h, 000h, 000h
        lea    eax, [edi*4]
        mov    dword ptr [esi], ecx
        db  08Bh, 0B8h, 000h, 000h, 000h, 000h
        db  08Bh, 004h, 095h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], eax
loc_3_0506:
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0511:
        mov    eax, 5
        pop    edi
        mov    word ptr [ebx + 1ah], 5
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 002h, 003h, 004h, 08Bh, 0FFh
loc_3_0550:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 1ah], 5
        mov    ebp, esp
        mov    eax, 5
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0570:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    dword ptr [eax], 0
        xor    eax, eax
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0590:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [eax + 1ch]
        push   ebx
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 10h], 0
        cmp    byte ptr [edx + 14h], 1
        push   edi
        jne    SHORT loc_3_0624
        push   ecx
        call   sub_3_0930
        add    esp, 4
        xor    ecx, ecx
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    cx, word ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [edx + 24h]
        add    ecx, 2
        xor    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 10h], ecx
        and    eax, 1eh
        db  08Dh, 00Ch, 085h, 000h, 000h, 000h, 000h
        mov    eax, ecx
        mov    dword ptr [ebp - 4], ecx
loc_3_05EB:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_3_0624
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 49h], ecx
        jne    SHORT loc_3_05EB
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_3_05EB
        lea    edi, [eax + 4dh]
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, edx
        mov    esi, dword ptr [ebp - 0ch]
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_3_05EB
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_3_05EB
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0624:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_3_0633
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0633:
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 4dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_063F:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_3_06D8
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 13h
        push   1
        rep stosd
        stosb
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 8], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0665:
        add    esp, 4
        mov    dword ptr [ebx + 18h], eax
        test   eax, eax
        je     SHORT loc_3_06BB
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 14h], 1
        jne    SHORT loc_3_06AD
        mov    eax, dword ptr [ebp - 10h]
        lea    edi, [ebx + 4dh]
        mov    esi, dword ptr [ebp - 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 49h], ecx
        mov    dword ptr [ebx + 4], edx
        mov    eax, dword ptr [edx]
        pop    edi
        pop    esi
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [edx], ebx
        mov    eax, ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_06AD:
        mov    eax, ebx
        pop    edi
        pop    esi
        mov    dword ptr [ebx + 4], ebx
        mov    dword ptr [ebx], ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_06BB:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06C1:
        add    esp, 4
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 24h
        mov    eax, ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_06D8:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 24h
        mov    eax, ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_06F0:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [esi + 30h]
        cmp    edi, 1
        sbb    dl, dl
        inc    dl
        cmp    dword ptr [esi + 2ch], eax
        je     SHORT loc_3_071B
        test   byte ptr [esi + 2ah], 4
        jne    SHORT loc_3_071B
        mov    cl, 1
        mov    dword ptr [esi + 2ch], eax
loc_3_071B:
        mov    eax, dword ptr [ebp + 14h]
        cmp    eax, edi
        je     SHORT loc_3_072D
        test   byte ptr [esi + 2ah], 2
        jne    SHORT loc_3_072D
        inc    cl
        mov    dword ptr [esi + 30h], eax
loc_3_072D:
        xor    edi, edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    di, word ptr [esi + 28h]
        cmp    eax, edi
        je     SHORT loc_3_074D
        mov    ebx, eax
        xor    ebx, edi
        cmp    ebx, 1
        jne    SHORT loc_3_0747
        test   al, 1
        jne    SHORT loc_3_0749
loc_3_0747:
        inc    cl
loc_3_0749:
        mov    word ptr [esi + 28h], ax
loc_3_074D:
        mov    eax, dword ptr [ebp + 18h]
        test   cl, cl
        jne    SHORT loc_3_0759
        cmp    dword ptr [esi + 38h], eax
        je     SHORT loc_3_076A
loc_3_0759:
        test   dl, dl
        je     SHORT loc_3_0767
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_3_0767
        or     byte ptr [esi + 2ah], 1
loc_3_0767:
        mov    dword ptr [esi + 38h], eax
loc_3_076A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_3_0770:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    dl, byte ptr [ecx + 4]
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_3_07C5
        mov    esi, eax
loc_3_0789:
        cmp    byte ptr [eax + 31h], dl
        jne    SHORT loc_3_07BF
        test   byte ptr [eax + 34h], 4
        jne    SHORT loc_3_07BF
        mov    bl, dl
        xor    ecx, ecx
        and    bl, 3
        mov    edi, 1
        mov    cl, bl
        xor    ebx, ebx
        db  08Ah, 099h, 000h, 000h, 000h, 000h
        mov    cl, dl
        and    cl, 70h
        shr    cl, 4
        shl    edi, cl
        test   edi, ebx
        jne    SHORT loc_3_07CC
        cmp    word ptr [eax + 26h], 0
        je     SHORT loc_3_07CC
loc_3_07BF:
        mov    eax, dword ptr [eax]
        cmp    esi, eax
        jne    SHORT loc_3_0789
loc_3_07C5:
        xor    eax, eax
loc_3_07C7:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_07CC:
        inc    word ptr [eax + 26h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        mov    cl, byte ptr [eax + 32h]
        cmp    cl, 0ffh
        je     SHORT loc_3_07C7
        xor    edx, edx
        pop    edi
        mov    dl, cl
        pop    esi
        mov    byte ptr [eax + 32h], 0ffh
        db  0C7h, 004h, 095h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0800:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   3eh
        mov    ebx, dword ptr [esi + 1ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0813:
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     NEAR PTR loc_3_08FC
        mov    edi, edx
        xor    eax, eax
        mov    ecx, 0fh
        mov    dword ptr [esi + 20h], edx
        rep stosd
        stosw
        mov    word ptr [edx + 35h], 0ffffh
        mov    al, byte ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        mov    byte ptr [edx + 31h], al
        mov    dword ptr [edx + 2ch], ecx
        mov    byte ptr [edx + 32h], 0ffh
        mov    word ptr [edx + 26h], 1
        mov    cl, byte ptr [esi + 5]
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [edx + 30h], cl
        mov    dword ptr [edx + 14h], esi
        test   esi, esi
        je     SHORT loc_3_08CD
        mov    ecx, dword ptr [esi + 0ch]
        test   ecx, ecx
        jne    SHORT loc_3_086D
        mov    dword ptr [edx + 4], edx
        mov    dword ptr [edx], edx
        mov    dword ptr [esi + 0ch], edx
        jmp    SHORT loc_3_087A
loc_3_086D:
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], edi
        mov    dword ptr [edi], edx
        mov    dword ptr [ecx + 4], edx
loc_3_087A:
        and    al, 70h
        xor    ecx, ecx
        shr    al, 4
        mov    cl, al
        cmp    ecx, 7
        ja     SHORT loc_3_08A9
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_3_088F:
        or     byte ptr [esi + 2ah], 10h
        inc    word ptr [esi + 24h]
        jmp    SHORT loc_3_08A9
loc_3_0899:
        or     byte ptr [esi + 2ah], 10h
        inc    word ptr [esi + 24h]
loc_3_08A1:
        or     byte ptr [esi + 2ah], 20h
        inc    word ptr [esi + 26h]
loc_3_08A9:
        cmp    byte ptr [ebx + 14h], 2
        je     SHORT loc_3_08C9
        test   byte ptr [ebx + 1ch], 40h
        je     SHORT loc_3_08C9
        cmp    word ptr [esi + 24h], 0
        je     SHORT loc_3_08C3
        cmp    word ptr [esi + 26h], 0
        jne    SHORT loc_3_08CD
loc_3_08C3:
        and    byte ptr [esi + 2ah], 0cfh
        jmp    SHORT loc_3_08CD
loc_3_08C9:
        or     byte ptr [esi + 2ah], 8
loc_3_08CD:
        mov    eax, dword ptr [ebx + 30h]
        test   eax, eax
        je     SHORT loc_3_08E5
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0ch], ecx
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [eax + 0ch], edx
        jmp    SHORT loc_3_08EB
loc_3_08E5:
        mov    dword ptr [edx + 0ch], edx
        mov    dword ptr [edx + 8], edx
loc_3_08EB:
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        mov    eax, edx
        pop    edi
        mov    dword ptr [ebx + 30h], edx
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_08FC:
        mov    eax, edx
        pop    edi
        mov    word ptr [esi + 1ah], 24h
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_3_0930:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        cmp    word ptr [ecx], ax
        je     SHORT loc_3_094F
loc_3_093E:
        xor    edx, edx
        add    ecx, 2
        mov    dx, word ptr [ecx - 2]
        add    eax, edx
        cmp    word ptr [ecx], 0
        jne    SHORT loc_3_093E
loc_3_094F:
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0960:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    edx, dword ptr [eax + 30h]
        test   edx, edx
        je     SHORT loc_3_099A
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, edx
        mov    esi, 0ffffh
        xor    ebx, ebx
loc_3_097F:
        mov    word ptr [ecx + 35h], si
        mov    byte ptr [ecx + 33h], bl
        test   ax, ax
        je     SHORT loc_3_098D
        dec    ax
loc_3_098D:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 8]
        cmp    ecx, edx
        jne    SHORT loc_3_097F
loc_3_099A:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_09A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        cmp    dword ptr [edi + 14h], 0
        je     SHORT loc_3_09BE
        mov    byte ptr [esi + 4], 0
        push   esi
        call   sub_3_0AD0
        add    esp, 4
loc_3_09BE:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_09D5
        mov    word ptr [esi + 1ah], 0
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_3_09D5:
        mov    eax, dword ptr [esi + 1ch]
        inc    dword ptr [eax + 55h]
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   esi
        call   sub_3_3DA0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_0A09
        mov    ax, word ptr [edi + 37h]
        test   ax, ax
        je     SHORT loc_3_0A09
        mov    word ptr [edi + 37h], 0
        mov    word ptr [esi + 1ah], ax
loc_3_0A09:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 20h]
        and    word ptr [esi + 2], 0fffeh
        mov    word ptr [eax + 1ah], cx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A3D:
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A46:
        add    esp, 4
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_0A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ecx + 30h]
        test   edi, edi
        je     SHORT loc_3_0ABA
loc_3_0A6F:
        mov    eax, edi
loc_3_0A71:
        mov    ebx, dword ptr [eax + 14h]
        mov    cl, byte ptr [ebx + 2ah]
        mov    dl, cl
        and    dl, 30h
        cmp    dl, 30h
        je     SHORT loc_3_0A86
        test   cl, 38h
        jne    SHORT loc_3_0A9A
loc_3_0A86:
        mov    eax, dword ptr [eax + 8]
        cmp    edi, eax
        jne    SHORT loc_3_0A71
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        mov    dword ptr [esi + 20h], eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0A9A:
        push   esi
        mov    dword ptr [esi + 20h], eax
        and    byte ptr [ebx + 2ah], 0c7h
        call   sub_3_0AD0
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 1ah], 0
        mov    edi, dword ptr [eax + 30h]
        test   edi, edi
        jne    SHORT loc_3_0A6F
loc_3_0ABA:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        mov    dword ptr [esi + 20h], eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0AD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 4]
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [eax + 14h]
        cmp    dword ptr [edi + 10h], 0
        je     SHORT loc_3_0AF7
loc_3_0AE8:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AEF:
        add    esp, 8
        cmp    eax, -3
        je     SHORT loc_3_0B25
loc_3_0AF7:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AFD:
        add    esp, 4
        cmp    eax, -3
        je     SHORT loc_3_0B2F
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_3_0AE8
        test   bl, bl
        jne    SHORT loc_3_0B15
        test   byte ptr [edi + 2ah], 18h
        jne    SHORT loc_3_0B1E
loc_3_0B15:
        push   edi
        call   sub_3_0B40
        add    esp, 4
loc_3_0B1E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_0B25:
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_0B2F:
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0B40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_3_0B5F
loc_3_0B4D:
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B56:
        add    esp, 4
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_3_0B4D
loc_3_0B5F:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0B70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [edi + 28h]
        test   ebx, ebx
        je     SHORT loc_3_0BE7
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 28h], 0
        sub    ecx, eax
        inc    ecx
        push   ecx
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        push   1
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   824h
        push   esi
        call   sub_3_3DA0
        add    esp, 24h
        test   eax, eax
        je     SHORT loc_3_0BD8
        cmp    word ptr [esi + 1ah], -3
        jne    SHORT loc_3_0BD2
        mov    eax, 0fffffffdh
        mov    dword ptr [edi + 28h], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_0BD2:
        mov    word ptr [esi + 1ah], 0
loc_3_0BD8:
        push   ebx
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0BE4:
        add    esp, 4
loc_3_0BE7:
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [eax + 51h], 0
        jne    SHORT loc_3_0BFF
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0BFF
        and    word ptr [eax + 1ch], 0ffbfh
loc_3_0BFF:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_0C10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 20h]
        mov    esi, dword ptr [eax + 14h]
        test   esi, esi
        je     SHORT loc_3_0C69
        lea    ebx, [esi + 1ch]
        mov    ecx, ebx
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_3_0C69
loc_3_0C2E:
        cmp    byte ptr [edi + 4], 2
        je     SHORT loc_3_0C3C
        mov    edx, dword ptr [eax + 10h]
        cmp    dword ptr [edi + 8], edx
        jne    SHORT loc_3_0C61
loc_3_0C3C:
        mov    dx, word ptr [edi + 6]
        cmp    word ptr [eax + 14h], dx
        jne    SHORT loc_3_0C61
        mov    edx, dword ptr [eax]
        push   eax
        push   edi
        mov    dword ptr [ecx], edx
        dec    dword ptr [esi + 20h]
        call   sub_3_0C70
        mov    word ptr [edi + 1ah], 0
        add    esp, 8
        mov    ecx, ebx
        jmp    SHORT loc_3_0C63
loc_3_0C61:
        mov    ecx, eax
loc_3_0C63:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        jne    SHORT loc_3_0C2E
loc_3_0C69:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_0C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 20h]
        mov    cx, word ptr [ebx + 35h]
        cmp    cx, 0ffffh
        jne    SHORT loc_3_0CB1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0C90:
        add    esp, 4
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [eax + 51h], 0
        jne    SHORT loc_3_0CA2
        and    word ptr [eax + 1ch], 0ffbfh
loc_3_0CA2:
        mov    word ptr [esi + 1ah], 6
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_0CB1:
        test   byte ptr [edi + 16h], 80h
        jne    SHORT loc_3_0CC9
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0CBD:
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_0CC9:
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [eax + 10h]
        test   byte ptr [edx + 1ah], 20h
        je     SHORT loc_3_0D26
        cmp    byte ptr [ebx + 3dh], 4
        je     SHORT loc_3_0D26
        cmp    dword ptr [ebx + 28h], 0
        je     SHORT loc_3_0CF0
loc_3_0CE1:
        push   esi
        call   sub_3_0B70
        add    esp, 4
        cmp    dword ptr [ebx + 28h], 0
        jne    SHORT loc_3_0CE1
loc_3_0CF0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 28h], edi
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        test   al, al
        jne    SHORT loc_3_0D71
        inc    al
        push   64h
        db  068h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D1A:
        add    esp, 8
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_0D26:
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 10h]
        mov    edx, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 8]
        push   eax
        sub    edx, eax
        movzx  eax, cx
        inc    edx
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   80dh
        push   esi
        call   sub_3_3DA0
        add    esp, 1ch
        mov    dword ptr [esi + 8], ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D5F:
        add    esp, 4
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [eax + 51h], 0
        jne    SHORT loc_3_0D71
        and    word ptr [eax + 1ch], 0ffbfh
loc_3_0D71:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0D80:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        xor    eax, eax
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        mov    ecx, dword ptr [ebx + 1ch]
        mov    al, byte ptr [ebx + 4]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, 2
        jne    NEAR PTR loc_3_0E8D
        mov    eax, dword ptr [ebx + 0ch]
        cmp    word ptr [eax], 4
        jne    SHORT loc_3_0DB5
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0DB5:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], 4010h
        test   byte ptr [ecx + 1ch], 80h
        jne    NEAR PTR loc_3_0E8D
        cmp    dword ptr [ecx + 69h], 0
        je     NEAR PTR loc_3_0E8D
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0DD8:
        mov    ecx, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ecx + 6dh]
        ja     NEAR PTR loc_3_0E8D
        mov    eax, dword ptr [ecx + 55h]
        cmp    dword ptr [ecx + 61h], eax
        jne    NEAR PTR loc_3_0E8D
        mov    eax, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [ecx + 69h]
        mov    si, word ptr [eax]
        add    eax, 4
        mov    cx, word ptr [edx]
        add    edx, 4
        lea    edi, [esi - 4]
        mov    word ptr [ebp - 2], di
        lea    edi, [ecx - 4]
        mov    word ptr [ebp - 4], di
        movzx  edi, cx
        movzx  ecx, si
        sub    edi, 4
        sub    ecx, 4
        cmp    edi, ecx
        jb     SHORT loc_3_0E22
        mov    edi, ecx
loc_3_0E22:
        movzx  ecx, di
        mov    esi, edx
        mov    edi, eax
        mov    dword ptr [ebp - 10h], ecx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_3_0E8D
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_3_0E8D
        xor    si, si
        xor    ecx, ecx
        mov    di, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], di
        jne    SHORT loc_3_0E59
        mov    word ptr [ebx + 1ah], cx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0E59:
        mov    di, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], di
        jbe    SHORT loc_3_0E8D
loc_3_0E63:
        cmp    word ptr [eax], 0
        jne    SHORT loc_3_0E73
        cmp    word ptr [ebp - 2], cx
        je     NEAR PTR loc_3_0F65
loc_3_0E73:
        mov    di, word ptr [edx]
        add    cx, di
        movzx  edi, di
        add    si, word ptr [eax]
        add    edx, edi
        xor    edi, edi
        mov    di, word ptr [eax]
        add    eax, edi
        cmp    si, cx
        je     SHORT loc_3_0E63
loc_3_0E8D:
        mov    eax, dword ptr [ebx + 1ch]
        inc    dword ptr [eax + 55h]
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_3_0ECF
        mov    eax, dword ptr [eax + 10h]
        test   word ptr [eax + 1ah], 108h
        je     SHORT loc_3_0ECF
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_3_0ECF
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    esi, dword ptr [ebx + 28h]
        lea    edi, [ecx + eax]
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    edi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_0ECF:
        cmp    dword ptr [ebp - 0ch], 3
        je     NEAR PTR loc_3_0F93
        cmp    dword ptr [ebp - 0ch], 4
        je     NEAR PTR loc_3_0F93
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   ebx
        call   sub_3_3DA0
        add    esp, 10h
        cmp    word ptr [ebx + 1ah], 0
        jne    SHORT loc_3_0F74
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     eax, 4000h
        je     NEAR PTR loc_3_0FAC
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 69h]
        test   eax, eax
        je     SHORT loc_3_0F35
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F28:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 69h], 0
loc_3_0F35:
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        call   sub_3_0FF0
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        test   eax, eax
        mov    dword ptr [ecx + 69h], eax
        je     SHORT loc_3_0FAC
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F50:
        add    eax, 2
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 6dh], eax
        mov    eax, dword ptr [ecx + 55h]
        dec    eax
        mov    dword ptr [ecx + 55h], eax
        mov    dword ptr [ecx + 61h], eax
        jmp    SHORT loc_3_0FAC
loc_3_0F65:
        xor    eax, eax
        pop    edi
        mov    word ptr [ebx + 1ah], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0F74:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 69h]
        test   eax, eax
        je     SHORT loc_3_0FAC
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F84:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 69h], 0
        jmp    SHORT loc_3_0FAC
loc_3_0F93:
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 3
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   ebx
        call   sub_3_35B0
        add    esp, 0ch
loc_3_0FAC:
        cmp    word ptr [ebx + 1ah], 0
        jne    SHORT loc_3_0FE3
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_3_0FCA
        push   ebx
        inc    byte ptr [ebx + 4]
        call   sub_3_0A50
        add    esp, 4
        dec    byte ptr [ebx + 4]
        jmp    SHORT loc_3_0FDD
loc_3_0FCA:
        cmp    dword ptr [ebp - 0ch], 1
        jne    SHORT loc_3_0FDD
        mov    byte ptr [ebx + 4], 0
        push   ebx
        call   sub_3_0A50
        add    esp, 4
loc_3_0FDD:
        mov    word ptr [ebx + 1ah], 0
loc_3_0FE3:
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_0FF0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        add    eax, 2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_100A:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_101D
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_101D:
        mov    eax, dword ptr [ebp - 4]
        add    esi, 4
        mov    word ptr [eax + 2], 4
        mov    word ptr [eax + 4], 0
        mov    word ptr [eax], 4
        cmp    word ptr [esi], 0
        je     SHORT loc_3_1067
loc_3_103A:
        xor    eax, eax
        push   0
        mov    ax, word ptr [esi]
        add    esi, 2
        push   esi
        lea    edi, [eax + esi - 2]
        mov    bx, word ptr [edi]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi], 0
        push   eax
        mov    esi, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_105C:
        mov    word ptr [edi], bx
        add    esp, 0ch
        test   bx, bx
        jne    SHORT loc_3_103A
loc_3_1067:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1080:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi]
        test   cl, 80h
        je     SHORT loc_3_10A4
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_3_10A4
        mov    eax, 5
        pop    edi
        mov    word ptr [esi + 1ah], 5
        pop    esi
        pop    ebp
        ret
loc_3_10A4:
        mov    eax, dword ptr [esi + 1ch]
        inc    dword ptr [eax + 55h]
        mov    al, byte ptr [esi + 4]
        cmp    al, 2
        jne    SHORT loc_3_10C0
        mov    eax, 1
        pop    edi
        mov    word ptr [esi + 1ah], 1
        pop    esi
        pop    ebp
        ret
loc_3_10C0:
        test   al, al
        je     SHORT loc_3_10F9
        mov    eax, dword ptr [esi + 2ch]
        mov    edx, dword ptr [esi + 0ch]
        mov    word ptr [eax], 4
        push   eax
        mov    word ptr [eax + 2], 4
        mov    word ptr [eax + 4], 0
        push   edx
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   4009h
        push   esi
        call   sub_3_3DA0
        add    esp, 18h
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_3_10F9:
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4008h
        push   esi
        call   sub_3_3DA0
        add    esp, 10h
        mov    eax, dword ptr [esi + 1ch]
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_3_115F
        mov    eax, dword ptr [eax + 72h]
        test   eax, eax
        je     SHORT loc_3_1134
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1127:
        add    esp, 4
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [eax + 72h], 0
loc_3_1134:
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_3_0FF0
        add    esp, 4
        mov    ecx, dword ptr [esi + 1ch]
        mov    edi, ecx
        mov    dword ptr [ecx + 72h], eax
        mov    eax, dword ptr [edi + 55h]
        mov    dword ptr [edi + 65h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1153:
        inc    eax
        mov    dword ptr [edi + 76h], eax
        pop    edi
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebp
        ret
loc_3_115F:
        pop    edi
        dec    dword ptr [eax + 55h]
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1170:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        mov    al, byte ptr [esi + 4]
        cmp    eax, 7
        ja     SHORT loc_3_118C
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_3_118C:
        xor    eax, eax
        pop    edi
        mov    word ptr [esi + 1ah], 1
        pop    esi
        pop    ebp
        ret
loc_3_1198:
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [eax + 2ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11A4:
        add    esp, 4
        mov    dword ptr [esi + 28h], eax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_3_11B0:
        mov    eax, dword ptr [esi + 28h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11B9:
        add    esp, 4
        mov    ecx, dword ptr [edi + 14h]
        or     byte ptr [ecx + 2ah], 4
        mov    dword ptr [ecx + 2ch], eax
        mov    al, byte ptr [edi + 33h]
        or     al, 10h
        mov    byte ptr [edi + 33h], al
        and    al, 0f7h
        mov    byte ptr [edi + 33h], al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_3_11D9:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11E0:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_11F5
        xor    eax, eax
        pop    edi
        mov    word ptr [esi + 1ah], 8
        pop    esi
        pop    ebp
        ret
loc_3_11F5:
        push   1
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11FE:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_122F
        cmp    byte ptr [esi + 4], 4
        jne    SHORT loc_3_1217
        mov    edx, dword ptr [edi + 0ch]
        lea    eax, [edi + 8]
        push   edx
        mov    ecx, dword ptr [eax]
        push   ecx
        jmp    SHORT loc_3_121E
loc_3_1217:
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        push   eax
loc_3_121E:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1223:
        add    esp, 8
        mov    dword ptr [esi + 28h], eax
        mov    word ptr [esi + 18h], 0
loc_3_122F:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1235:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_3_123E:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1245:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_125A
        xor    eax, eax
        pop    edi
        mov    word ptr [esi + 1ah], 8
        pop    esi
        pop    ebp
        ret
loc_3_125A:
        push   1
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1263:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_129F
        cmp    byte ptr [esi + 4], 5
        mov    eax, dword ptr [esi + 28h]
        push   eax
        jne    SHORT loc_3_1286
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1279:
        add    esp, 4
        lea    ecx, [edi + 8]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], edx
        jmp    SHORT loc_3_1293
loc_3_1286:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_128B:
        add    esp, 4
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
loc_3_1293:
        push   0
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_129C:
        add    esp, 0ch
loc_3_129F:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_12A5:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
sub_3_12D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        lea    eax, [ebp - 2ch]
        push   esi
        push   edi
        push   2bh
        push   eax
        call   sub_3_3C80
        add    esp, 8
        mov    edi, eax
        mov    esi, dword ptr [ebp + 8]
        push   edi
        push   2bh
        db  068h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [esi + 0ch]
        push   8
        mov    dword ptr [esi + 48h], 0
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   4881h
        push   esi
        call   sub_3_3DA0
        add    esp, 20h
        push   edi
        call   sub_3_3CD0
        mov    ax, word ptr [esi + 1ah]
        add    esp, 4
        test   ax, ax
        mov    dword ptr [esi + 0ch], ebx
        jne    SHORT loc_3_1354
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 14h]
        mov    cx, word ptr [ebp - 16h]
        and    eax, 0ffffh
        shl    ecx, 10h
        mov    edx, dword ptr [ebp + 0ch]
        or     ecx, eax
        pop    edi
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        mov    dword ptr [edx], ecx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1354:
        cmp    ax, 12h
        jne    SHORT loc_3_1369
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 1ah], 0
        mov    dword ptr [eax], 0
loc_3_1369:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1380:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebx + 20h]
        mov    esi, dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ebp - 4], ecx
        mov    edx, dword ptr [eax + 10h]
        xor    eax, eax
        mov    al, byte ptr [ebx + 4]
        test   eax, eax
        je     SHORT loc_3_13BC
        cmp    eax, 1
        je     NEAR PTR loc_3_149A
        xor    eax, eax
        pop    edi
        mov    word ptr [ebx + 1ah], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_13BC:
        mov    edi, esi
        xor    eax, eax
        mov    ecx, 0dh
        rep stosd
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 2ch], eax
        mov    ax, word ptr [edx + 1ah]
        mov    ecx, dword ptr [ecx + 14h]
        test   ah, 1
        mov    dword ptr [esi + 30h], ecx
        je     SHORT loc_3_13EE
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_13E2:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_13EE:
        test   al, 20h
        je     SHORT loc_3_1455
        push   61h
        mov    dword ptr [ebx], 34h
        push   ebx
        call   sub_3_3A30
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_1412
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1412:
        mov    edi, dword ptr [ebx + 50h]
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4823h
        push   ebx
        call   sub_3_3DA0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_1443
        add    esi, 1ch
        push   esi
        push   ebx
        call   sub_3_12D0
        add    esp, 8
loc_3_1443:
        push   edi
        call   sub_3_3CD0
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1455:
        xor    eax, eax
        mov    ax, word ptr [ecx + 28h]
        mov    ecx, dword ptr [ecx + 2ch]
        push   ecx
        mov    dword ptr [esi], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1466:
        add    esp, 4
        lea    ecx, [esi + 14h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], edx
        lea    edx, [esi + 1ch]
        push   edx
        push   ebx
        call   sub_3_12D0
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 14h]
        pop    edi
        mov    dword ptr [esi + 28h], 1
        mov    eax, dword ptr [edx + 30h]
        mov    dword ptr [esi + 24h], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_149A:
        mov    ecx, dword ptr [ebp - 4]
        xor    eax, eax
        mov    edi, dword ptr [ebx + 0ch]
        mov    esi, dword ptr [ecx + 14h]
        mov    ax, word ptr [esi + 4dh]
        add    esi, 4dh
        add    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_14D0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        mov    di, word ptr [eax + 22h]
        test   di, di
        je     SHORT loc_3_1516
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi], 0
        jne    SHORT loc_3_14F2
        cmp    word ptr [esi + 2], 0
        je     SHORT loc_3_1516
loc_3_14F2:
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_14FA:
        movsx  ecx, di
        shl    ecx, 2
        add    esp, 4
        lea    edx, [ecx + ecx*2]
        lea    edi, [edx + edx*4]
        add    eax, edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1511:
        add    esp, 4
        mov    dword ptr [esi], eax
loc_3_1516:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_1520:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        mov    di, word ptr [eax + 22h]
        test   di, di
        je     SHORT loc_3_1566
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi], 0
        jne    SHORT loc_3_1542
        cmp    word ptr [esi + 2], 0
        je     SHORT loc_3_1566
loc_3_1542:
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_154A:
        movsx  ecx, di
        shl    ecx, 2
        add    esp, 4
        lea    edx, [ecx + ecx*2]
        lea    edi, [edx + edx*4]
        sub    eax, edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1561:
        add    esp, 4
        mov    dword ptr [esi], eax
loc_3_1566:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_1570:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     SHORT loc_3_159F
        cmp    edi, -1
        je     SHORT loc_3_159F
        mov    eax, dword ptr [ebp + 0ch]
        movsx  ebx, word ptr [eax + 22h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1590:
        sub    ebx, eax
        shl    ebx, 2
        lea    eax, [ebx + ebx*2]
        lea    ebx, [eax + eax*4]
        add    ebx, edi
        mov    dword ptr [esi], ebx
loc_3_159F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_15B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     SHORT loc_3_15DE
        cmp    edi, -1
        je     SHORT loc_3_15DE
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_15C8:
        mov    ecx, dword ptr [ebp + 0ch]
        movsx  edx, word ptr [ecx + 22h]
        sub    eax, edx
        shl    eax, 2
        lea    ecx, [eax + eax*2]
        lea    eax, [ecx + ecx*4]
        add    eax, edi
        mov    dword ptr [esi], eax
loc_3_15DE:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_15F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        mov    ebx, dword ptr [edi + 10h]
        je     SHORT loc_3_1638
        mov    word ptr [eax], 4
        push   0
        mov    word ptr [eax + 2], 4
        mov    word ptr [eax + 4], 0
        mov    ecx, dword ptr [ebx + 62h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1623:
        add    esp, 0ch
        lea    eax, [edi + 7bh]
        mov    ecx, dword ptr [esi + 0ch]
        push   0
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1635:
        add    esp, 0ch
loc_3_1638:
        mov    ax, word ptr [esi + 18h]
        cmp    ax, 2
        jne    SHORT loc_3_1654
        push   esi
        call   sub_3_3190
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_1654:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_3_16C7
        mov    cx, word ptr [edi + 1ch]
        mov    edx, dword ptr [edi + 2ch]
        or     cl, 2
        mov    dword ptr [esi + 20h], edx
        mov    word ptr [edi + 1ch], cx
        test   ax, ax
        je     SHORT loc_3_16C7
        and    cx, 80h
        jne    SHORT loc_3_1684
        test   byte ptr [ebx + 1ah], 20h
        je     SHORT loc_3_1684
        mov    word ptr [esi + 1ah], 0
loc_3_1684:
        cmp    cx, 1
        mov    eax, 0
        adc    eax, -1
        mov    cl, byte ptr [edi + 14h]
        and    ax, 2
        mov    byte ptr [esi + 4], cl
        mov    word ptr [esi + 18h], ax
        xor    eax, eax
        mov    ax, word ptr [edi + 22h]
        xor    ecx, ecx
        mov    dword ptr [esi + 34h], eax
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        mov    dword ptr [esi + 38h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    cl, byte ptr [eax + 7ah]
        mov    edx, dword ptr [eax + 76h]
        movzx  ax, byte ptr [edx + 5ah]
        mov    word ptr [esi + 6], ax
        mov    dword ptr [esi + 3ch], ecx
loc_3_16C7:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_3_16D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 0ch]
loc_3_16DF:
        lea    ebx, [edi + 7bh]
        push   1
        push   2eh
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16EC:
        add    esp, 4
        lea    eax, [eax*2 + 2]
        lea    ecx, [ebp - 30h]
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1701:
        add    esp, 14h
        lea    eax, [ebp - 30h]
        mov    edx, dword ptr [ebp + 10h]
loc_3_170A:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_3_172A
        or     cl, cl
        je     SHORT loc_3_1726
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_3_172A
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_3_170A
loc_3_1726:
        xor    eax, eax
        jmp    SHORT loc_3_172F
loc_3_172A:
        sbb    eax, eax
        sbb    eax, -1
loc_3_172F:
        test   eax, eax
        je     SHORT loc_3_1744
        mov    edi, dword ptr [edi + 4]
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_3_16DF
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1744:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_3_1750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 1dh], 80h
        jne    SHORT loc_3_1798
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_3_178B
        mov    eax, dword ptr [esi + 72h]
        mov    ecx, dword ptr [esi + 76h]
        mov    dword ptr [esi + 72h], 0
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 7ah]
        push   eax
        push   ecx
        call   dword ptr [ecx + 2ch]
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_1816
loc_3_178B:
        xor    eax, eax
        or     byte ptr [esi + 1dh], 80h
        mov    word ptr [esi + 1ah], ax
        mov    dword ptr [esi + 76h], eax
loc_3_1798:
        mov    edi, dword ptr [esi + 0ch]
        test   edi, edi
        je     SHORT loc_3_17BD
        mov    ebx, 37h
loc_3_17A4:
        or     byte ptr [edi + 1ch], 80h
        mov    word ptr [edi + 26h], bx
        push   edi
        call   sub_3_0960
        add    esp, 4
        mov    edi, dword ptr [edi + 4]
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_3_17A4
loc_3_17BD:
        mov    dword ptr [ebp - 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_1816
        db  0BFh, 000h, 000h, 000h, 000h
loc_3_17D2:
        mov    ebx, dword ptr [edi]
        mov    cx, word ptr [ebx + 2]
        test   cl, 1
        je     SHORT loc_3_180B
        test   cl, 20h
        jne    SHORT loc_3_180B
        cmp    dword ptr [ebx + 10h], esi
        jne    SHORT loc_3_180B
        cmp    byte ptr [ebx + 7], 0
        je     SHORT loc_3_17F6
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17F3:
        add    esp, 4
loc_3_17F6:
        mov    dword ptr [ebx + 20h], 37h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1803:
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
loc_3_180B:
        add    edi, 4
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], eax
        jb     SHORT loc_3_17D2
loc_3_1816:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_3_1820:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp + 10h], 1
        push   esi
        push   edi
        sbb    esi, esi
        and    esi, 40dh
        add    esi, 25h
        cmp    dword ptr [ebp + 10h], 2
        jne    SHORT loc_3_185B
        mov    ecx, dword ptr [ebp + 8]
        or     esi, 800h
        mov    dword ptr [ebp - 2ch], 200h
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ecx + 1ch], eax
        jmp    SHORT loc_3_186D
loc_3_185B:
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    edx, dword ptr [ecx + 1ch]
        mov    ax, word ptr [edx + 22h]
        mov    dword ptr [ebp - 18h], edx
        mov    dword ptr [ebp - 2ch], eax
loc_3_186D:
        mov    byte ptr [ebp - 1], 2
        or     esi, 2000h
        mov    dword ptr [ebp - 30h], esi
loc_3_187A:
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1887:
        add    esp, 8
        mov    ecx, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 10h], ecx
        test   ecx, ecx
        jne    NEAR PTR loc_3_1D1F
        cmp    dword ptr [ebp + 10h], 2
        je     SHORT loc_3_18B4
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [edx + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_18B4
        mov    esi, dword ptr [ebp - 8]
        or     byte ptr [esi + 47h], 80h
loc_3_18B4:
        mov    esi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 7
        mov    edx, dword ptr [esi + 38h]
        mov    edi, edx
        mov    dword ptr [ebp - 14h], edx
        rep stosd
        stosb
        cmp    dword ptr [ebp + 10h], 2
        jne    SHORT loc_3_18D6
        mov    edx, dword ptr [ebp - 14h]
        or     byte ptr [edx + 0bh], 2
loc_3_18D6:
        lea    eax, [ebp + 18h]
        mov    ebx, dword ptr [ebp - 14h]
        add    ebx, 1dh
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 24h], eax
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_3_1B89
loc_3_18FF:
        mov    eax, dword ptr [ebp - 24h]
        movsx  eax, byte ptr [eax]
        sub    eax, 30h
        cmp    eax, 44h
        ja     NEAR PTR loc_3_1B7A
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_3_1920:
        mov    word ptr [ebx], 0
        add    ebx, 2
        jmp    NEAR PTR loc_3_1B7A
loc_3_192D:
        mov    eax, ebx
        mov    edx, dword ptr [ebp - 14h]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 20h], ebx
        sar    eax, 1
        add    ebx, 2
        mov    byte ptr [edx], al
        sub    al, 0eh
        cmp    dword ptr [ebp + 10h], 0
        mov    byte ptr [edx + 1bh], al
        jne    SHORT loc_3_1951
        mov    byte ptr [ebx], 0
        inc    ebx
        jmp    SHORT loc_3_199B
loc_3_1951:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 18h], 0
        mov    ecx, dword ptr [eax - 4]
        je     SHORT loc_3_1977
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [edx + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_1977
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 8]
        and    eax, 1
        add    ebx, eax
loc_3_1977:
        push   1
        mov    esi, dword ptr [ebp - 8]
        push   esi
        push   208h
        push   ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_198D:
        add    esp, 18h
        cmp    eax, -1
        je     NEAR PTR loc_3_1C48
        add    ebx, eax
loc_3_199B:
        sub    ebx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 14h]
        sub    ebx, 3bh
        mov    esi, dword ptr [ebp - 8]
        and    ebx, 0fffffffeh
        mov    word ptr [edx + 15h], bx
        lea    ebx, [ebx + esi + 3ch]
        jmp    NEAR PTR loc_3_1B7A
loc_3_19B7:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [ebx], ecx
        jmp    NEAR PTR loc_3_1B77
loc_3_19C8:
        push   ebx
        mov    esi, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], 4
        push   esi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [eax - 4]
        add    esp, 0ch
        mov    esi, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, dword ptr [esi + 38h]
        cmp    dword ptr [esi + 20h], 0
        je     NEAR PTR loc_3_1B7A
        mov    esi, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 20h]
        jmp    NEAR PTR loc_3_1D1C
loc_3_19F9:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax - 4]
        or     word ptr [esi + 46h], cx
        jmp    NEAR PTR loc_3_1B7A
loc_3_1A0F:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 4
        mov    esi, dword ptr [eax - 4]
        mov    eax, dword ptr [ebp - 0ch]
        test   esi, esi
        mov    eax, dword ptr [eax - 4]
        je     NEAR PTR loc_3_1B7A
        mov    ecx, ebx
        sub    ecx, dword ptr [ebp - 8]
        add    ecx, esi
        sub    ecx, 5ch
        cmp    ecx, 460h
        ja     NEAR PTR loc_3_1C56
        push   esi
        push   eax
        push   ebx
        call   sub_3_3D60
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        jne    NEAR PTR loc_3_1D1F
        add    ebx, esi
        jmp    NEAR PTR loc_3_1B7A
loc_3_1A5E:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [ebp - 28h], ecx
        jmp    NEAR PTR loc_3_1B7A
loc_3_1A70:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [ebp - 1ch], ecx
        jmp    NEAR PTR loc_3_1B7A
loc_3_1A82:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax - 4]
        mov    eax, dword ptr [edx + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_1AA2
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 8]
        and    eax, 1
        add    ebx, eax
loc_3_1AA2:
        push   0
        mov    esi, dword ptr [ebp - 8]
        push   esi
        push   208h
        push   ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AB8:
        add    esp, 18h
        cmp    eax, -1
        je     NEAR PTR loc_3_1C62
        add    ebx, eax
        jmp    NEAR PTR loc_3_1B7A
loc_3_1ACB:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [edx + 0dh], ecx
        jmp    NEAR PTR loc_3_1B7A
loc_3_1AE0:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AF0:
        add    esp, 4
        lea    edi, [eax + 1]
        mov    edx, dword ptr [ebp - 18h]
        lea    eax, [edi*2]
        mov    ecx, dword ptr [edx + 10h]
        test   byte ptr [ecx + 1bh], 2
        je     SHORT loc_3_1B27
        mov    ecx, ebx
        sub    ecx, dword ptr [ebp - 8]
        and    ecx, 1
        add    ebx, ecx
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        add    ebx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_1B7A
loc_3_1B27:
        push   1
        add    edi, edi
        push   edi
        push   eax
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B34:
        mov    byte ptr [ebx + eax], 0
        add    esp, 14h
        inc    eax
        add    ebx, eax
        jmp    SHORT loc_3_1B7A
loc_3_1B40:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        add    ebx, 2
        mov    ecx, dword ptr [eax - 4]
        mov    word ptr [ebx - 2], cx
        jmp    SHORT loc_3_1B7A
loc_3_1B53:
        mov    dword ptr [ebx], 0
        jmp    SHORT loc_3_1B77
loc_3_1B5B:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax - 4]
        mov    eax, dword ptr [edx + 10h]
        push   eax
        mov    dword ptr [ebx], ecx
        push   ebx
        call   sub_3_1570
        add    esp, 8
loc_3_1B77:
        add    ebx, 4
loc_3_1B7A:
        inc    dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_3_18FF
loc_3_1B89:
        mov    ecx, ebx
        mov    edx, dword ptr [ebp - 14h]
        sub    ecx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, ecx
        sub    ax, word ptr [edx + 15h]
        sub    ax, 3ch
        mov    word ptr [edx + 13h], ax
        mov    word ptr [edx + 1], ax
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [edx + 3], ax
        test   ax, ax
        je     SHORT loc_3_1BEE
        mov    ebx, dword ptr [esi + 0ch]
        sub    ecx, 39h
        mov    esi, dword ptr [ebp - 8]
        and    ecx, 0fffffffch
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 28h], ebx
        mov    word ptr [edx + 19h], cx
        lea    ebx, [ecx + esi + 3ch]
        mov    edx, dword ptr [ebp - 2ch]
        sub    edx, ecx
        movzx  ecx, ax
        cmp    ecx, edx
        jbe    SHORT loc_3_1BDC
        mov    ax, dx
loc_3_1BDC:
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 17h], ax
        mov    word ptr [ecx + 1ah], ax
        sub    word ptr [ecx + 18h], ax
loc_3_1BEE:
        mov    esi, dword ptr [ebp - 8]
        mov    edx, ebx
        movzx  ecx, ax
        sub    edx, dword ptr [ebp - 20h]
        mov    dword ptr [esi + 2ch], ecx
        add    dx, ax
        mov    esi, dword ptr [ebp + 0ch]
        sub    dx, 2
        mov    eax, dword ptr [ebp - 20h]
        mov    di, word ptr [esi + 1ch]
        mov    word ptr [eax], dx
        mov    edx, dword ptr [ebp - 14h]
        mov    word ptr [edx + 5], di
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        mov    edi, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 7], ax
        mov    esi, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 10h], 2
        mov    dword ptr [esi + 34h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_3_1C70
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 38h], ebx
        push   eax
        push   esi
        call   sub_3_1E00
        jmp    NEAR PTR loc_3_1D19
loc_3_1C48:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 8
        jmp    NEAR PTR loc_3_1D1F
loc_3_1C56:
        mov    dword ptr [ebp - 10h], 57h
        jmp    NEAR PTR loc_3_1D1F
loc_3_1C62:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 8
        jmp    NEAR PTR loc_3_1D1F
loc_3_1C70:
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 38h], edx
        mov    edx, dword ptr [ebp - 18h]
        or     byte ptr [esi + 2], 1
        mov    eax, dword ptr [edx + 10h]
        db  0C7h, 046h, 024h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [eax + 76h]
        test   edx, edx
        je     SHORT loc_3_1CA5
        push   ecx
        sub    ebx, esi
        sub    ebx, 3ch
        push   ebx
        push   eax
        push   esi
        call   dword ptr [edx + 30h]
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        jmp    SHORT loc_3_1CAC
loc_3_1CA5:
        mov    dword ptr [ebp - 10h], 37h
loc_3_1CAC:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_1D1F
        mov    esi, dword ptr [ebp - 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CBB:
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, 0eah
        jne    SHORT loc_3_1CD5
        cmp    dword ptr [ebp - 28h], 0
        je     SHORT loc_3_1CD5
        mov    dword ptr [ebp - 10h], 0
loc_3_1CD5:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_1D1F
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_3_1D02
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [edx + 1ch]
        mov    ecx, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 94h
        push   eax
        push   ecx
        call   sub_3_3D20
        add    esp, 0ch
        jmp    SHORT loc_3_1D1C
loc_3_1D02:
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 4]
        test   eax, eax
        je     SHORT loc_3_1D1F
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 94h
        push   ecx
        push   edx
        call   eax
loc_3_1D19:
        add    esp, 8
loc_3_1D1C:
        mov    dword ptr [ebp - 10h], eax
loc_3_1D1F:
        mov    esi, dword ptr [ebp - 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1D28:
        add    esp, 4
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_3_1D4C
        cmp    dword ptr [ebp - 10h], 36h
        je     SHORT loc_3_1D43
        cmp    dword ptr [ebp - 10h], 37h
        je     SHORT loc_3_1D43
        cmp    dword ptr [ebp - 10h], 40h
        jne    SHORT loc_3_1D4C
loc_3_1D43:
        dec    byte ptr [ebp - 1]
        jne    NEAR PTR loc_3_187A
loc_3_1D4C:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_3_1D62
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [edx + 10h]
        test   byte ptr [eax + 1ah], 80h
        jne    SHORT loc_3_1D62
        or     byte ptr [eax + 1dh], 10h
loc_3_1D62:
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Eh, 00Eh
        db  00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 001h, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh
        db  00Eh, 002h, 00Eh, 003h, 004h, 00Eh, 005h, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 006h, 00Eh, 007h
        db  008h, 009h, 00Ah, 00Eh, 00Bh, 00Eh, 00Eh, 00Ch, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh
        db  00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh
        db  00Eh, 00Dh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 38h]
        mov    eax, dword ptr [esi + 2ch]
        mov    edi, ecx
        sub    edi, esi
        sub    edi, 3ch
        test   eax, eax
        je     SHORT loc_3_1E47
        add    edi, eax
        cmp    edi, 200h
        jle    SHORT loc_3_1E33
        mov    eax, 6fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1E33:
        push   eax
        push   0
        mov    eax, dword ptr [esi + 28h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1E40:
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_1EC3
loc_3_1E47:
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_3_1EBA
loc_3_1E5F:
        mov    eax, dword ptr [esi + 1ch]
        mov    ax, word ptr [eax + 6]
        cmp    ax, 0ffh
        je     SHORT loc_3_1E76
        movzx  cx, byte ptr [ebx + 5ah]
        cmp    cx, ax
        jne    SHORT loc_3_1EB3
loc_3_1E76:
        push   0
        lea    eax, [esi + 3ch]
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        or     byte ptr [esi + 2], 1
        mov    dword ptr [esi + 20h], 0
        push   edi
        push   eax
        push   ecx
        push   ebx
        push   esi
        call   dword ptr [ebx + 38h]
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_1EB0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1EA0:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_1EB0
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_3_1EB3
loc_3_1EB0:
        mov    dword ptr [ebp - 4], eax
loc_3_1EB3:
        mov    ebx, dword ptr [ebx + 18h]
        test   ebx, ebx
        jne    SHORT loc_3_1E5F
loc_3_1EBA:
        cmp    dword ptr [ebp - 8], 1
        sbb    eax, eax
        and    eax, dword ptr [ebp - 4]
loc_3_1EC3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1ED0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 38h]
        mov    al, byte ptr [esi + 40h]
        cmp    al, 25h
        jne    SHORT loc_3_1EEC
        mov    byte ptr [esi + 40h], 26h
        jmp    SHORT loc_3_1EF4
loc_3_1EEC:
        cmp    al, 32h
        jne    SHORT loc_3_1EF4
        mov    byte ptr [esi + 40h], 33h
loc_3_1EF4:
        cmp    byte ptr [esi + 40h], 26h
        lea    edi, [esi + 5ch]
        jne    SHORT loc_3_1F14
        lea    ecx, [edi + 11h]
        mov    byte ptr [edi], 8
        mov    word ptr [ecx], 1
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 34h
        jmp    SHORT loc_3_1F31
loc_3_1F14:
        mov    ax, word ptr [ebx + 22h]
        lea    ecx, [edi + 13h]
        mov    word ptr [edi + 11h], ax
        mov    byte ptr [edi], 9
        mov    word ptr [ecx], 3
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 38h
loc_3_1F31:
        cmp    word ptr [ebx + 18h], 0
        je     NEAR PTR loc_3_2004
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_3_1F4E
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1F4E:
        mov    eax, dword ptr [esi + 2ch]
        mov    ecx, dword ptr [esi + 28h]
        push   eax
        push   ecx
        call   sub_3_3CB0
        mov    ax, word ptr [ebx + 18h]
        add    esp, 8
        mov    ecx, dword ptr [esi + 10h]
        xor    edx, edx
        mov    dx, word ptr [ecx + 20h]
        movzx  ecx, ax
        sub    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        jle    SHORT loc_3_1F77
        mov    edx, ecx
loc_3_1F77:
        mov    word ptr [edi + 9], 0
        mov    word ptr [edi + 7], 0
        mov    word ptr [edi + 5], 0
        mov    cx, word ptr [ebx + 1ah]
        sub    ax, dx
        mov    word ptr [edi + 0fh], cx
        mov    cx, word ptr [ebp - 4]
        mov    dword ptr [esi + 2ch], edx
        mov    word ptr [edi + 0bh], dx
        mov    word ptr [edi + 0dh], cx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 18h], ax
        add    word ptr [ebx + 1ah], dx
        add    word ptr [ecx], dx
        test   ax, ax
        je     SHORT loc_3_1FC5
        mov    eax, dword ptr [esi + 10h]
        test   byte ptr [eax + 1ah], 80h
        je     SHORT loc_3_1FC5
        or     byte ptr [esi + 2], 2
        jmp    SHORT loc_3_1FCB
loc_3_1FC5:
        and    word ptr [esi + 2], 0fffdh
loc_3_1FCB:
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [edi + 76h]
        test   eax, eax
        je     SHORT loc_3_1FE7
        push   edx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   edi
        push   esi
        call   dword ptr [eax + 30h]
        add    esp, 10h
        mov    dword ptr [esi + 20h], eax
        jmp    SHORT loc_3_1FEE
loc_3_1FE7:
        mov    dword ptr [esi + 20h], 37h
loc_3_1FEE:
        cmp    dword ptr [esi + 20h], 0
        je     NEAR PTR loc_3_2079
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2004:
        mov    eax, dword ptr [esi + 34h]
        add    ax, word ptr [ebx + 20h]
        mov    word ptr [ebx + 20h], ax
        cmp    word ptr [ebx + 1eh], ax
        jbe    SHORT loc_3_2082
        cmp    dword ptr [esi + 20h], 37h
        je     SHORT loc_3_2082
        mov    ecx, dword ptr [esi + 10h]
        test   byte ptr [ecx + 1ah], 80h
        jne    SHORT loc_3_2079
        mov    word ptr [edi + 0fh], ax
        mov    ax, word ptr [ebx + 1ch]
        xor    ebx, ebx
        mov    word ptr [edi + 9], ax
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [edi + 7], bx
        mov    edx, dword ptr [esi + 10h]
        mov    word ptr [edi + 5], bx
        mov    eax, dword ptr [edx + 76h]
        mov    word ptr [edi + 0dh], bx
        test   eax, eax
        mov    word ptr [edi + 0bh], bx
        mov    word ptr [ecx], bx
        je     SHORT loc_3_2061
        push   ebx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        push   esi
        call   dword ptr [eax + 30h]
        add    esp, 10h
        jmp    SHORT loc_3_2066
loc_3_2061:
        mov    eax, 37h
loc_3_2066:
        test   eax, eax
        je     SHORT loc_3_2079
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    eax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2079:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2082:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_2090:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        cmp    byte ptr [edx], 0ah
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 38h]
        jb     SHORT loc_3_2122
        mov    ax, word ptr [edx + 1]
        cmp    word ptr [ecx + 1ch], ax
        jbe    SHORT loc_3_20B3
        mov    word ptr [ecx + 1ch], ax
loc_3_20B3:
        mov    ax, word ptr [edx + 3]
        cmp    word ptr [ecx + 1eh], ax
        jbe    SHORT loc_3_20C1
        mov    word ptr [ecx + 1eh], ax
loc_3_20C1:
        xor    ebx, ebx
        mov    bx, word ptr [edx + 7]
        test   ebx, ebx
        je     SHORT loc_3_20FB
        cmp    ebx, 428h
        ja     SHORT loc_3_20F4
        xor    ecx, ecx
        add    edi, 94h
        mov    cx, word ptr [edx + 9]
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, ecx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_20FB
loc_3_20F4:
        mov    dword ptr [edi + 20h], 6fh
loc_3_20FB:
        cmp    word ptr [edx + 0dh], 0
        je     SHORT loc_3_2122
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    cx, word ptr [edx + 0fh]
        pop    edi
        pop    esi
        mov    dword ptr [eax], ecx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 18h]
        mov    cx, word ptr [edx + 11h]
        pop    ebx
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret
loc_3_2122:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2130:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax], 0
        mov    ebp, esp
        je     SHORT loc_3_217F
loc_3_213D:
        mov    cx, word ptr [eax]
        cmp    cx, 5ch
        je     SHORT loc_3_2183
        cmp    cx, 2fh
        je     SHORT loc_3_2183
        cmp    cx, 3ah
        je     SHORT loc_3_2183
        cmp    cx, 2ah
        je     SHORT loc_3_2183
        cmp    cx, 3fh
        je     SHORT loc_3_2183
        cmp    cx, 22h
        je     SHORT loc_3_2183
        cmp    cx, 3ch
        je     SHORT loc_3_2183
        cmp    cx, 3eh
        je     SHORT loc_3_2183
        cmp    cx, 7ch
        je     SHORT loc_3_2183
        add    eax, 2
        cmp    word ptr [eax], 0
        jne    SHORT loc_3_213D
loc_3_217F:
        mov    al, 1
        pop    ebp
        ret
loc_3_2183:
        xor    al, al
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2190:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 14h]
        lea    edi, [esi + 2ch]
        push   edi
        call   sub_3_2130
        add    esp, 4
        test   al, al
        je     SHORT loc_3_221D
        add    esi, 234h
        push   esi
        call   sub_3_2130
        add    esp, 4
        test   al, al
        je     SHORT loc_3_221D
        mov    eax, dword ptr [ebp + 10h]
        test   byte ptr [eax + 1bh], 4
        jne    SHORT loc_3_21D3
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_21D3:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 234h]
        test   eax, eax
        je     SHORT loc_3_2213
        push   5
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_21E9:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_2213
        cmp    word ptr [esi], 0
        je     SHORT loc_3_220C
        push   5
        mov    eax, dword ptr [ebx + 234h]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2205:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_2213
loc_3_220C:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_2213:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_221D:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2230:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 1ch]
        test   byte ptr [esi + 1ch], 80h
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_3_2271
        push   4ffh
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2257:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_2268
        mov    edx, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], dx
loc_3_2268:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_226E:
        add    esp, 4
loc_3_2271:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 1ah]
        test   ax, ax
        je     SHORT loc_3_2287
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_3_2287:
        call   sub_3_2D10
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_22A7
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 8
        mov    word ptr [ecx + 1ah], 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_22A7:
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 1ah]
        test   ah, 1
        jne    NEAR PTR loc_3_233E
        test   al, 8
        je     SHORT loc_3_22C1
        cmp    byte ptr [esi + 16h], 0
        jne    SHORT loc_3_233E
loc_3_22C1:
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 30h]
        mov    ecx, 6
        rep movsd
        movsw
        mov    word ptr [ebx + 232h], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 20eh], 1
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebx + 230h], ax
        test   eax, 80000000h
        je     SHORT loc_3_232A
        mov    eax, dword ptr [ecx + 0ch]
        lea    edi, [ebx + 216h]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        lea    edx, [ecx + eax]
        mov    ecx, 5
        lea    eax, [edx + 2]
        mov    esi, eax
        rep movsd
        movsw
        mov    word ptr [edx], 1ah
        lea    esi, [ebp - 30h]
        mov    edi, eax
        mov    ecx, 6
        rep movsd
        movsw
loc_3_232A:
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        push   ecx
        call   sub_3_2B50
        add    esp, 0ch
        jmp    NEAR PTR loc_3_2541
loc_3_233E:
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2345:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_236C
        push   ebx
        call   sub_3_2D70
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_236C:
        mov    ax, word ptr [ebx + 238h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [esi + 1eh], ax
        mov    edx, dword ptr [ebx + 23eh]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 10h], edx
        mov    word ptr [esi + 1ch], 0ah
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 1ah]
        test   ch, 1
        je     NEAR PTR loc_3_241F
        test   ch, 4
        je     SHORT loc_3_23B0
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_23AD:
        add    esp, 8
loc_3_23B0:
        lea    edi, [ebx + 232h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 1ah]
        and    ax, 200h
        lea    ecx, [ebx + 214h]
        mov    word ptr [edi], 0
        mov    dword ptr [ebp - 1ah], ecx
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0ffffff00h
        sub    edx, edx
        add    eax, 200h
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 238h]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 60h
        div    ecx
        mov    edx, dword ptr [ebp - 1ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 20eh], 3
        mov    word ptr [edx], ax
        lea    edx, [ebx + 230h]
        mov    eax, dword ptr [ecx]
        mov    word ptr [edx], ax
        lea    eax, [ebx + 22eh]
        mov    word ptr [eax], 104h
        jmp    SHORT loc_3_2465
loc_3_241F:
        sub    dx, dx
        lea    edi, [ebx + 232h]
        mov    word ptr [edi], 4
        lea    ecx, [ebx + 214h]
        mov    word ptr [ebp - 4], 11ch
        div    word ptr [ebp - 4]
        mov    word ptr [ecx], ax
        lea    edx, [ebx + 230h]
        mov    dword ptr [ebp - 1ah], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 20eh], 2
        mov    eax, dword ptr [ecx]
        mov    word ptr [edx], ax
        lea    eax, [ebx + 22eh]
        mov    word ptr [eax], 1
loc_3_2465:
        test   byte ptr [edx], 8
        je     SHORT loc_3_24A3
        mov    ax, word ptr [eax]
        mov    edi, 1
        sub    ax, 104h
        mov    ecx, dword ptr [ebx + 23ah]
        cmp    ax, di
        mov    edx, dword ptr [ebp + 8]
        sbb    eax, eax
        neg    eax
        push   eax
        push   0
        push   ecx
        push   edx
        call   sub_3_2F60
        mov    word ptr [ebp - 10h], di
        add    esp, 10h
        mov    word ptr [ebp - 14h], 0
        mov    word ptr [ebp - 12h], di
        jmp    SHORT loc_3_24F5
loc_3_24A3:
        mov    dword ptr [ebp - 4], 0
        mov    word ptr [ebp - 12h], 0
        mov    ax, word ptr [eax]
        lea    ecx, [ebp - 14h]
        mov    word ptr [ebp - 4], ax
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0ch]
        push   ecx
        push   eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 1ah]
        mov    ax, word ptr [edi]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ecx]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx]
        push   eax
        push   1
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        push   ecx
        call   sub_3_1820
        add    esp, 34h
loc_3_24F5:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1ah], 0
        jne    SHORT loc_3_250C
        cmp    word ptr [ebp - 12h], 0
        jne    SHORT loc_3_250C
        mov    word ptr [ecx + 1ah], 12h
loc_3_250C:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1ah], 10bh
        jne    SHORT loc_3_251D
        mov    word ptr [ecx + 1ah], 3
loc_3_251D:
        mov    ax, word ptr [ebp - 14h]
        push   esi
        mov    word ptr [ebx + 210h], 0
        mov    word ptr [ebx], ax
        mov    cx, word ptr [ebp - 12h]
        mov    word ptr [ebx + 212h], cx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_253E:
        add    esp, 4
loc_3_2541:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1ah], 0
        jne    SHORT loc_3_258F
        mov    eax, dword ptr [ecx + 10h]
        push   ebx
        push   ecx
        mov    dword ptr [ecx + 20h], ebx
        db  0C7h, 040h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 008h, 000h, 000h, 000h, 000h
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        call   sub_3_2750
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   ebx
        push   edx
        call   sub_3_2190
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_2598
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_3_25B0
        jmp    SHORT loc_3_2595
loc_3_258F:
        push   ebx
        call   sub_3_2D70
loc_3_2595:
        add    esp, 4
loc_3_2598:
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        movsx  eax, word ptr [ecx + 1ah]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_25B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], ecx
loc_3_25C8:
        test   byte ptr [edi + 230h], 8
        jne    NEAR PTR loc_3_2720
        mov    ax, word ptr [edi + 212h]
        cmp    word ptr [edi + 210h], ax
        jb     NEAR PTR loc_3_26EF
        cmp    word ptr [edi + 20eh], 1
        je     NEAR PTR loc_3_26BB
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_25FE:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_3_2732
        mov    ax, word ptr [edi + 238h]
        mov    ecx, dword ptr [edi + 23eh]
        mov    word ptr [ebx + 1eh], ax
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [edi + 6]
        push   eax
        mov    dword ptr [ebx], esi
        xor    eax, eax
        mov    dword ptr [ebx + 10h], ecx
        mov    word ptr [ebx + 1ch], 8
        mov    word ptr [ebp - 10h], 0
        mov    ax, word ptr [edi + 232h]
        mov    ecx, dword ptr [edi + 2]
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 22eh]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi + 214h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi]
        push   eax
        push   2
        push   0
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ebx
        push   esi
        call   sub_3_1820
        add    esp, 38h
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_3_2688
        cmp    word ptr [ebp - 10h], 0
        jne    SHORT loc_3_2688
        mov    word ptr [esi + 1ah], 12h
loc_3_2688:
        mov    word ptr [edi + 210h], 0
        mov    ax, word ptr [ebp - 10h]
        cmp    word ptr [esi + 1ah], 12h
        mov    word ptr [edi + 212h], ax
        jne    SHORT loc_3_26B0
        cmp    word ptr [ebp - 10h], 0
        jbe    SHORT loc_3_26B0
        mov    word ptr [esi + 1ah], 0
loc_3_26B0:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_26B6:
        add    esp, 4
        jmp    SHORT loc_3_26C7
loc_3_26BB:
        push   1
        push   edi
        push   esi
        call   sub_3_2B50
        add    esp, 0ch
loc_3_26C7:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_3_2715
        push   edi
        push   esi
        call   sub_3_2750
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_3_2190
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_2715
        jmp    NEAR PTR loc_3_25C8
loc_3_26EF:
        push   edi
        push   esi
        call   sub_3_2750
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_3_2190
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_25C8
        mov    word ptr [esi + 1ah], 0
loc_3_2715:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2720:
        mov    eax, 12h
        pop    edi
        mov    word ptr [esi + 1ah], 12h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2732:
        mov    eax, 8
        pop    edi
        mov    word ptr [esi + 1ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2750:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 14h]
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 20eh]
        cmp    eax, 1
        je     SHORT loc_3_278F
        cmp    eax, 2
        je     NEAR PTR loc_3_2880
        cmp    eax, 3
        je     NEAR PTR loc_3_299E
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_278F:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ecx + 210h]
        add    ecx, 216h
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 8], ecx
        inc    dx
        mov    esi, dword ptr [ecx + 24h]
        mov    word ptr [ecx - 6], dx
        cmp    word ptr [ecx], 0
        je     SHORT loc_3_280F
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    word ptr [ecx + 212h], dx
        jbe    SHORT loc_3_280F
loc_3_27C1:
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        xor    edx, edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dx, word ptr [eax + 210h]
        mov    eax, edx
        lea    ecx, [edx + edx*4]
        lea    edx, [eax + ecx*4]
        lea    edx, [eax + edx*2]
        add    edx, esi
        add    edx, 12h
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_27EC:
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_280F
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 210h]
        inc    ax
        mov    word ptr [ecx + 210h], ax
        cmp    word ptr [ecx + 212h], ax
        ja     SHORT loc_3_27C1
loc_3_280F:
        mov    eax, dword ptr [edi + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 2]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    edx, [eax + eax*4]
        lea    eax, [ecx + edx*4]
        lea    eax, [ecx + eax*2]
        add    eax, esi
        add    ebx, 44h
        add    eax, 12h
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2837:
        add    esp, 10h
        mov    eax, 0ch
loc_3_283F:
        cmp    word ptr [ebx], 20h
        jne    SHORT loc_3_2850
        mov    word ptr [ebx], 0
        sub    ebx, 2
        dec    eax
        jns    SHORT loc_3_283F
loc_3_2850:
        mov    edi, dword ptr [ebp + 0ch]
        xor    edx, edx
        add    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ecx + 210h]
        mov    eax, edx
        lea    ecx, [edx + edx*4]
        lea    edx, [eax + ecx*4]
        mov    ecx, 5
        lea    eax, [eax + edx*2]
        lea    esi, [eax + esi - 19h]
        rep movsd
        movsb
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2880:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 210h]
        mov    esi, dword ptr [ecx + 23ah]
        movzx  ecx, ax
        inc    ecx
        mov    dword ptr [ebp - 4], esi
loc_3_2899:
        mov    esi, dword ptr [ebp - 4]
        xor    eax, eax
        add    esi, edx
        mov    al, byte ptr [esi + 1ah]
        add    eax, 1ch
        add    edx, eax
        dec    ecx
        jne    SHORT loc_3_2899
        mov    ecx, dword ptr [ebp + 0ch]
        push   1
        add    ecx, 6
        xor    eax, eax
        mov    al, byte ptr [esi + 1ah]
        lea    edx, [esi + 1bh]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_28C3:
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        cdq
        sub    eax, edx
        push   1
        sar    eax, 1
        mov    word ptr [ecx + eax*2 + 6], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 2], eax
        mov    dword ptr [edi + 24h], eax
        xor    eax, eax
        lea    ecx, [esi + 1bh]
        mov    al, byte ptr [esi + 1ah]
        lea    edx, [ebx + 2ch]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_28F3:
        cdq
        add    esp, 10h
        sub    eax, edx
        sar    eax, 1
        mov    ecx, dword ptr [esi + 10h]
        mov    word ptr [ebx + eax*2 + 2ch], 0
        xor    eax, eax
        mov    dword ptr [ebx + 20h], ecx
        mov    dword ptr [ebx + 1ch], 0
        mov    word ptr [ebx + 234h], 0
        mov    ax, word ptr [esi + 18h]
        mov    cx, word ptr [esi + 6]
        mov    dword ptr [ebx], eax
        mov    dx, word ptr [esi + 4]
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 2], dx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_293A:
        add    esp, 4
        lea    ecx, [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [esi + 8]
        mov    dword ptr [ecx + 4], edx
        mov    dx, word ptr [esi + 0ah]
        mov    word ptr [ebp - 4], dx
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp - 4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_295E:
        add    esp, 4
        lea    ecx, [ebx + 0ch]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [esi + 0ch]
        mov    dword ptr [ecx + 4], edx
        mov    dx, word ptr [esi + 0eh]
        add    ebx, 14h
        mov    word ptr [ebp - 4], dx
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp - 4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2985:
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        inc    word ptr [ecx + 210h]
        mov    dword ptr [ebx], eax
        pop    edi
        mov    dword ptr [ebx + 4], edx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_299E:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 210h]
        mov    esi, dword ptr [ecx + 23ah]
        movzx  eax, ax
        inc    eax
loc_3_29B4:
        lea    ecx, [esi + edx]
        mov    dword ptr [ebp - 4], ecx
        add    edx, dword ptr [ecx]
        dec    eax
        jne    SHORT loc_3_29B4
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 2], eax
        mov    dword ptr [edi + 24h], eax
        mov    eax, dword ptr [ecx + 10h]
        test   byte ptr [eax + 1bh], 2
        je     NEAR PTR loc_3_2A6D
        lea    edi, [edx + 6]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 3ch]
        add    edx, 5eh
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, edx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 4]
        lea    edi, [ebx + 2ch]
        mov    eax, dword ptr [ecx + 3ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, eax
        and    esi, 0fffffffeh
        mov    word ptr [esi + ecx + 6], 0
        mov    esi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp - 4]
        lea    edi, [ebx + 234h]
        mov    eax, dword ptr [edx + 3ch]
        lea    esi, [edx + 46h]
        and    eax, 0fffffffeh
        mov    word ptr [eax + ebx + 2ch], 0
        xor    eax, eax
        mov    ax, word ptr [edx + 44h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [edx + 44h]
        and    eax, 0fffffffeh
        mov    word ptr [eax + ebx + 234h], 0
        jmp    NEAR PTR loc_3_2AED
loc_3_2A6D:
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        add    esi, 6
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 3ch]
        push   eax
        lea    eax, [edx + 5eh]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2A86:
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        cdq
        lea    edi, [ebx + 2ch]
        mov    dword ptr [ebp - 8], eax
        sub    eax, edx
        sar    eax, 1
        mov    word ptr [ecx + eax*2 + 6], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 44h]
        test   cx, cx
        je     SHORT loc_3_2ADD
        movzx  eax, cx
        push   1
        lea    ecx, [ebx + 234h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 46h
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2ADA:
        add    esp, 10h
loc_3_2ADD:
        cdq
        sub    eax, edx
        sar    eax, 1
        mov    word ptr [ebx + eax*2 + 234h], 0
loc_3_2AED:
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebx + 4]
        mov    edx, dword ptr [eax + 8]
        mov    esi, dword ptr [eax + 0ch]
        add    eax, 8
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], esi
        mov    edx, dword ptr [eax + 10h]
        mov    esi, dword ptr [eax + 14h]
        lea    ecx, [ebx + 0ch]
        add    eax, 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        add    eax, 18h
        mov    dword ptr [ecx + 4], esi
        lea    ecx, [ebx + 14h]
        mov    edx, dword ptr [eax]
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], esi
        mov    eax, dword ptr [edx + 2ch]
        mov    ecx, dword ptr [edx + 28h]
        pop    edi
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [edx + 38h]
        pop    esi
        mov    dword ptr [ebx + 20h], ecx
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [ebp + 0ch]
        pop    ebx
        inc    word ptr [ecx + 210h]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_3_2B50:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    edi, dword ptr [ebp + 0ch]
        mov    al, byte ptr [esi + 4]
        mov    cx, word ptr [edi + 238h]
        mov    edx, dword ptr [esi + 14h]
        sub    cx, 12h
        mov    ebx, dword ptr [edi + 23ah]
        mov    byte ptr [ebp - 1], al
        mov    dword ptr [ebp - 10h], edx
        mov    ax, cx
        mov    dword ptr [esi + 48h], ebx
        mov    word ptr [ebp - 0ch], 2bh
        movzx  ecx, cx
        sub    dx, dx
        push   ecx
        div    word ptr [ebp - 0ch]
        mov    word ptr [edi + 214h], ax
        lea    eax, [ebx + 12h]
        push   eax
        call   sub_3_3C80
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    word ptr [edi + 210h], 0
        mov    al, byte ptr [ebp + 10h]
        mov    byte ptr [esi + 4], al
        test   al, al
        je     SHORT loc_3_2BD0
        mov    dword ptr [ebp - 8], 800h
        lea    eax, [edi + 6]
        mov    dword ptr [esi + 14h], eax
loc_3_2BD0:
        mov    eax, dword ptr [ebp - 8]
        or     eax, 4081h
        mov    dword ptr [ebp - 14h], eax
loc_3_2BDB:
        mov    al, byte ptr [esi + 4]
        test   al, al
        je     SHORT loc_3_2BEB
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_3_2BF1
loc_3_2BEB:
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], ecx
loc_3_2BF1:
        test   al, al
        mov    eax, 0
        jne    SHORT loc_3_2C03
        xor    eax, eax
        mov    ax, word ptr [edi + 230h]
loc_3_2C03:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dx, word ptr [edi + 214h]
        push   ecx
        lea    ecx, [edx + edx*4]
        lea    ecx, [edx + ecx*4]
        lea    ecx, [edx + ecx*2]
        push   ecx
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 14h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        call   sub_3_3DA0
        movzx  ax, byte ptr [ebx + 3]
        mov    word ptr [edi + 212h], ax
        add    esp, 20h
        cmp    word ptr [esi + 1ah], 0
        jne    NEAR PTR loc_3_2CEA
        cmp    word ptr [edi + 216h], 0
        je     NEAR PTR loc_3_2CEA
        test   ax, ax
        mov    edx, dword ptr [ebp - 10h]
        mov    word ptr [edi + 210h], 0
        mov    dword ptr [esi + 14h], edx
        je     SHORT loc_3_2CB6
loc_3_2C6B:
        mov    eax, dword ptr [esi + 14h]
        lea    ecx, [edi + 216h]
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 210h]
        mov    ecx, eax
        lea    edx, [eax + eax*4]
        lea    eax, [ecx + edx*4]
        lea    eax, [ecx + eax*2]
        add    eax, ebx
        add    eax, 12h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2C96:
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_2CEA
        mov    ax, word ptr [edi + 210h]
        inc    ax
        mov    word ptr [edi + 210h], ax
        cmp    word ptr [edi + 212h], ax
        ja     SHORT loc_3_2C6B
loc_3_2CB6:
        xor    ecx, ecx
        mov    cx, word ptr [edi + 210h]
        mov    eax, ecx
        lea    edx, [ecx + ecx*4]
        mov    byte ptr [esi + 4], 1
        lea    ecx, [eax + edx*4]
        mov    word ptr [edi + 210h], 0
        lea    ecx, [eax + ecx*2]
        add    ecx, ebx
        sub    ecx, 19h
        cmp    word ptr [esi + 1ah], 0
        mov    dword ptr [esi + 14h], ecx
        je     NEAR PTR loc_3_2BDB
loc_3_2CEA:
        mov    al, byte ptr [ebp - 1]
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [esi + 4], al
        push   ecx
        mov    dword ptr [esi + 14h], edx
        call   sub_3_3CD0
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_3_2D10:
        push   esi
        push   242h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2D1B:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_2D28
        xor    eax, eax
        pop    esi
        ret
loc_3_2D28:
        push   980h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2D32:
        add    esp, 4
        mov    dword ptr [esi + 23ah], eax
        test   eax, eax
        jne    SHORT loc_3_2D4C
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2D45:
        add    esp, 4
        xor    eax, eax
        pop    esi
        ret
loc_3_2D4C:
        mov    word ptr [esi + 238h], 980h
        push   980h
        push   eax
        call   sub_3_3C80
        add    esp, 8
        mov    dword ptr [esi + 23eh], eax
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_3_2D70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 23eh]
        push   eax
        call   sub_3_3CD0
        add    esp, 4
        mov    eax, dword ptr [esi + 23ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2D92:
        add    esp, 4
        mov    eax, dword ptr [esi + 234h]
        test   eax, eax
        je     SHORT loc_3_2DA8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2DA5:
        add    esp, 4
loc_3_2DA8:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2DAE:
        add    esp, 4
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2DC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        cmp    word ptr [edi + 20eh], 1
        je     SHORT loc_3_2DF7
        test   byte ptr [edi + 230h], 8
        jne    SHORT loc_3_2DF7
        xor    eax, eax
        mov    ax, word ptr [edi]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4034h
        push   esi
        call   sub_3_3DA0
        add    esp, 10h
loc_3_2DF7:
        push   edi
        call   sub_3_2D70
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_2E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   41eh
        mov    esi, dword ptr [ebx + 1ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2E29:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_2E45
        mov    eax, 8
        pop    edi
        mov    word ptr [ebx + 1ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2E45:
        mov    edi, dword ptr [ebp - 4]
        push   26h
        add    edi, 216h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2E55:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_3_2E7D
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2E68:
        add    esp, 4
        mov    eax, 8
        mov    word ptr [ebx + 1ah], 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2E7D:
        mov    word ptr [edi], 4
        push   0
        mov    word ptr [edi + 2], 4
        mov    word ptr [edi + 4], 0
        lea    eax, [esi + 7bh]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2E9A:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        push   216h
        push   eax
        call   sub_3_3C80
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 1eh], 216h
        push   edi
        mov    dword ptr [eax], ebx
        push   105h
        mov    dword ptr [eax + 10h], ecx
        mov    word ptr [eax + 1ch], 0
        push   3
        push   0
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        push   ebx
        call   sub_3_1820
        add    esp, 24h
        cmp    word ptr [ebx + 1ah], 0
        jne    SHORT loc_3_2F29
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0eh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2EFB:
        add    esp, 4
        mov    edi, eax
        mov    dword ptr [esi + 18h], eax
        test   edi, edi
        jne    SHORT loc_3_2F0F
        mov    word ptr [ebx + 1ah], 8
        jmp    SHORT loc_3_2F29
loc_3_2F0F:
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0eh
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_2F29:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        call   sub_3_3CD0
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2F3E:
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2F4A:
        add    esp, 4
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_2F60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        cmp    dword ptr [ebp + 14h], 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 1ch]
        sbb    edi, edi
        and    edi, 0fffffeebh
        add    edi, 21ah
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2F88:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_2FA6
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2FA6:
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2FAD:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_2FD5
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2FBD:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2FD5:
        push   edi
        push   ebx
        call   sub_3_3C80
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 1eh], di
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp + 14h], 1
        mov    word ptr [ecx + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        sbb    eax, eax
        and    eax, 0ffffff00h
        add    eax, 102h
        push   eax
        push   3
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ecx
        push   edx
        call   sub_3_1820
        add    esp, 20h
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1ah], 0
        jne    NEAR PTR loc_3_30E2
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_3_30CC
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_3_30A1
        mov    eax, dword ptr [esi + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_306B
        mov    edx, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        lea    esi, [ebx + 12h]
        mov    ecx, edx
        shr    ecx, 2
        lea    edi, [eax + 5eh]
        mov    dword ptr [eax + 3ch], edx
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_3095
loc_3_306B:
        push   1
        mov    eax, dword ptr [ebx + 0ch]
        push   104h
        lea    ecx, [ebx + 12h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5eh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3086:
        add    esp, 14h
        mov    ecx, dword ptr [ebx + 0ch]
        shr    ecx, 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 3ch], ecx
loc_3_3095:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 38h], 8
        jmp    SHORT loc_3_30CC
loc_3_30A1:
        xor    eax, eax
        lea    esi, [ebx + 5]
        mov    al, byte ptr [ebx + 4]
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 1bh
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    cl, byte ptr [ebx + 4]
        mov    edx, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 18h], 8
        mov    byte ptr [edx + 1ah], cl
loc_3_30CC:
        mov    ecx, dword ptr [ebp + 10h]
        test   ecx, ecx
        je     SHORT loc_3_30E2
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_3_30DE
        mov    eax, dword ptr [ebx + 8]
        jmp    SHORT loc_3_30E0
loc_3_30DE:
        mov    eax, dword ptr [ebx]
loc_3_30E0:
        mov    dword ptr [ecx], eax
loc_3_30E2:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_3_3CD0
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_30F4:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3100:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3120:
        push   ebp
        mov    ecx, 5
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        repe cmpsb
        jne    SHORT loc_3_3141
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_3141:
        xor    eax, eax
        db  0BAh, 000h, 000h, 000h, 000h
loc_3_3148:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx]
loc_3_314D:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_3_316D
        or     bl, bl
        je     SHORT loc_3_3169
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_3_316D
        add    ecx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_3_314D
loc_3_3169:
        xor    ecx, ecx
        jmp    SHORT loc_3_3172
loc_3_316D:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_3172:
        test   ecx, ecx
        je     SHORT loc_3_3187
        add    edx, 10h
        inc    eax
        db  081h, 0FAh, 020h, 000h, 000h, 000h
        jbe    SHORT loc_3_3148
        mov    eax, 0ffh
loc_3_3187:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_3190:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 18h], 0
        mov    ebx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 2ch]
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 18h]
        test   eax, eax
        je     SHORT loc_3_31D2
        test   byte ptr [ebx + 1ch], 80h
        je     SHORT loc_3_31D2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_31C8:
        mov    dword ptr [ebx + 18h], 0
        add    esp, 4
loc_3_31D2:
        cmp    dword ptr [ebx + 18h], 0
        jne    NEAR PTR loc_3_32C7
        test   byte ptr [ebx + 1ch], 80h
        je     SHORT loc_3_320A
        push   4ffh
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_31F0:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_320A
        mov    ecx, dword ptr [eax + 20h]
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 1ah], cx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3207:
        add    esp, 4
loc_3_320A:
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 1ah]
        test   ax, ax
        jne    NEAR PTR loc_3_33BD
        test   byte ptr [edi + 1bh], 1
        je     SHORT loc_3_3241
        push   esi
        call   sub_3_2E10
        add    esp, 4
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 1ah], 0
        je     NEAR PTR loc_3_32C7
        mov    byte ptr [ebx + 16h], 0
        mov    word ptr [esi + 1ah], 0
loc_3_3241:
        cmp    byte ptr [ebx + 16h], 0ffh
        jne    SHORT loc_3_324B
        mov    byte ptr [ebx + 16h], 0
loc_3_324B:
        xor    eax, eax
        mov    ecx, 0ffffffffh
        mov    al, byte ptr [ebx + 16h]
        shl    eax, 4
        db  08Bh, 0B8h, 000h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        lea    ecx, [ecx*2 + 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3274:
        add    esp, 4
        mov    esi, eax
        mov    dword ptr [ebx + 18h], eax
        test   esi, esi
        jne    SHORT loc_3_328C
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_328C:
        xor    eax, eax
        mov    al, byte ptr [ebx + 16h]
        shl    eax, 4
        push   1
        push   edi
        db  08Bh, 088h, 00Ch, 000h, 000h, 000h
        mov    dword ptr [esi], ecx
        db  08Bh, 090h, 004h, 000h, 000h, 000h
        mov    dword ptr [esi + 4], edx
        lea    edx, [esi + 0ch]
        db  08Bh, 088h, 008h, 000h, 000h, 000h
        db  08Bh, 080h, 000h, 000h, 000h, 000h
        push   eax
        mov    dword ptr [ebp - 8], ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_32C1:
        add    esp, 10h
        mov    dword ptr [esi + 8], eax
loc_3_32C7:
        mov    eax, dword ptr [ebx + 18h]
        push   1
        push   208h
        add    eax, 0ch
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax - 4]
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 2ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_32E5:
        add    esp, 14h
        mov    edx, dword ptr [ebx + 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 8]
        shr    eax, 1
        mov    edi, ecx
        mov    byte ptr [eax + ecx], 0
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, dword ptr [ebp + 8]
        cmp    ecx, dword ptr [esi]
        jae    SHORT loc_3_332F
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        mov    edi, dword ptr [esi + 14h]
        shr    ecx, 2
        mov    esi, dword ptr [ebp - 4]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_332F:
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 8], 0
        mov    word ptr [esi + 18h], ax
        jne    SHORT loc_3_3354
        cmp    dword ptr [edx + 4], 0feh
        mov    eax, 104h
        jge    SHORT loc_3_3351
        mov    eax, 50h
loc_3_3351:
        mov    dword ptr [ebp - 8], eax
loc_3_3354:
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 10h
        mov    esi, dword ptr [ebp + 8]
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    dword ptr [esi], ecx
        call   sub_3_3120
        add    esp, 4
        mov    cl, al
        mov    byte ptr [ebx + 16h], al
        cmp    cl, 0ffh
        je     SHORT loc_3_338E
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        mov    al, cl
        shl    eax, 4
        db  08Bh, 090h, 00Ch, 000h, 000h, 000h
        or     word ptr [esi + 18h], dx
loc_3_338E:
        test   cl, cl
        jne    SHORT loc_3_33AF
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [eax + 4], 0ffh
        jl     SHORT loc_3_33AF
        mov    ecx, dword ptr [eax]
        mov    esi, dword ptr [ebp + 8]
        or     ecx, 4000h
        mov    word ptr [esi + 18h], cx
        mov    dword ptr [eax], ecx
loc_3_33AF:
        mov    esi, dword ptr [ebp + 8]
        pop    edi
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_33BD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_33D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 1ch]
        push   268h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_33EA:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_3408
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3408:
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_340F:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_3437
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_341F:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3437:
        push   268h
        push   ebx
        call   sub_3_3C80
        add    esp, 8
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 9
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        rep stosd
        stosw
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 1eh], 268h
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    word ptr [ecx + 1ch], 0ah
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        lea    eax, [ebp - 14h]
        push   eax
        push   edx
        push   104h
        push   1
        push   1
        push   16h
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ecx
        push   eax
        call   sub_3_1820
        add    esp, 34h
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1ah], 0
        jne    NEAR PTR loc_3_3573
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        jne    SHORT loc_3_34C2
        cmp    word ptr [ebx + 44h], 0
        jne    SHORT loc_3_34BE
        mov    eax, 1
loc_3_34BE:
        test   eax, eax
        je     SHORT loc_3_34CC
loc_3_34C2:
        cmp    eax, 1
        je     SHORT loc_3_3520
        jmp    NEAR PTR loc_3_3573
loc_3_34CC:
        mov    eax, dword ptr [esi + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_34F6
        lea    esi, [ebx + 46h]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, 6
        rep movsd
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ebx + 44h]
        and    eax, 0fffffffeh
        mov    word ptr [eax + ecx], 0
        jmp    SHORT loc_3_3573
loc_3_34F6:
        push   1
        xor    eax, eax
        mov    ax, word ptr [ebx + 44h]
        lea    ecx, [ebx + 46h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_350C:
        add    esp, 10h
        xor    eax, eax
        mov    ax, word ptr [ebx + 44h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + eax*2], 0
        jmp    SHORT loc_3_3573
loc_3_3520:
        mov    eax, dword ptr [esi + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_3551
        mov    eax, dword ptr [ebx + 3ch]
        lea    esi, [ebx + 5eh]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 3ch]
        mov    ecx, dword ptr [ebp + 10h]
        and    eax, 0fffffffeh
        mov    word ptr [eax + ecx], 0
        jmp    SHORT loc_3_3573
loc_3_3551:
        push   1
        mov    eax, dword ptr [ebx + 3ch]
        push   eax
        lea    ecx, [ebx + 5eh]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3564:
        add    esp, 10h
        mov    edx, dword ptr [ebx + 3ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + edx*2], 0
loc_3_3573:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_3_3CD0
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3585:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3591:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_35B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        mov    ecx, dword ptr [eax + 10h]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0ch], ecx
        push   edi
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        cmp    dword ptr [ebp + 0ch], 0
        mov    ecx, dword ptr [eax + 10h]
        je     SHORT loc_3_35ED
        cmp    byte ptr [eax + 16h], 1
        jne    SHORT loc_3_35ED
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 32h
        mov    eax, 32h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_35ED:
        test   byte ptr [ecx + 1bh], 1
        jne    SHORT loc_3_361A
        xor    eax, eax
        mov    edi, dword ptr [ebp - 0ch]
        mov    ax, word ptr [esi]
        add    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        movsx  eax, word ptr [ecx + 1ah]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_361A:
        push   410h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3624:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_3643
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3643:
        mov    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        xor    edx, edx
        mov    word ptr [ecx + 2], ax
        mov    ebx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 4], dx
        add    ecx, 208h
        mov    word ptr [ebx], ax
        mov    dword ptr [ebp - 10h], ecx
        mov    word ptr [ebx + 2], ax
        lea    eax, [esi + 4]
        mov    word ptr [ebx + 4], dx
        mov    dword ptr [ebp - 8], eax
        cmp    word ptr [eax], dx
        je     NEAR PTR loc_3_371E
loc_3_367E:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx]
        xor    edi, edi
        lea    esi, [ecx + 2]
        push   0
        lea    ebx, [eax + ecx]
        push   esi
        mov    di, word ptr [ebx]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ebx], 0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_36A2:
        mov    word ptr [ebx], di
        add    esp, 0ch
        test   di, di
        jne    SHORT loc_3_36D8
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_3_36D8
        mov    edi, dword ptr [ebp - 10h]
        xor    eax, eax
        mov    ecx, 82h
        rep stosd
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 10h]
        mov    ax, word ptr [ecx]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_36F9
loc_3_36D8:
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0ch], 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_3_33D0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_371E
loc_3_36F9:
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 0ch], 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_370E:
        add    esp, 0ch
        mov    dword ptr [ebp - 8], ebx
        cmp    word ptr [ebx], 0
        jne    NEAR PTR loc_3_367E
loc_3_371E:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3727:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_3740:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_374D:
        add    esp, 4
        mov    eax, 1
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3760:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 76h]
        test   eax, eax
        jne    SHORT loc_3_377F
        mov    eax, 37h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_377F:
        mov    ecx, dword ptr [esi + 72h]
        push   0
        push   eax
        mov    dword ptr [ebp - 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_378D:
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [edx + 4eh], ax
        test   ax, ax
        jne    SHORT loc_3_3814
        mov    eax, dword ptr [ebp + 10h]
        db  0B9h, 000h, 000h, 000h, 000h
        lea    ebx, [edx + eax + 3ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 4], eax
        add    ebx, 10h
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebx - 8], eax
        mov    eax, 20202020h
        mov    ecx, dword ptr [ecx + 0ch]
        add    esi, 2ch
        mov    edi, esi
        mov    dword ptr [ebx - 4], ecx
        mov    ecx, 0ffffffffh
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0ch], eax
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, ebx
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2], ax
        add    dword ptr [ebp + 10h], 20h
        mov    word ptr [edx + 50h], ax
        jmp    SHORT loc_3_3829
loc_3_3814:
        test   byte ptr [edx + 3], 40h
        jne    SHORT loc_3_3829
        mov    ecx, dword ptr [ebp - 4]
        and    byte ptr [esi + 1dh], 0efh
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [edx + 50h], ax
loc_3_3829:
        mov    ax, word ptr [edx + 2]
        test   al, 2
        je     SHORT loc_3_383D
        or     al, 20h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [edx + 2], ax
        mov    dword ptr [ecx + 40h], edx
loc_3_383D:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 14h], eax
        xor    eax, eax
        push   ebx
        or     byte ptr [edx + 2], 10h
        mov    word ptr [edx + 0ah], ax
        push   edx
        mov    dword ptr [edx + 18h], ecx
        mov    byte ptr [edx + 6], al
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_3_3870
        add    esp, 0ch
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_3870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        xor    edi, edi
        cmp    byte ptr [esi + 40h], 1eh
        mov    dword ptr [ebp - 4], edi
        jne    SHORT loc_3_38F8
        mov    ax, word ptr [esi + 71h]
        lea    edx, [esi + 5ch]
        add    ax, word ptr [edx + 17h]
        xor    ecx, ecx
        sub    ax, 3bh
        mov    cx, word ptr [edx + 3]
        mov    word ptr [edx + 19h], ax
        mov    eax, dword ptr [esi + 2ch]
        sub    ecx, eax
        cmp    dword ptr [esi + 18h], eax
        mov    dword ptr [ebp - 4], ecx
        jae    SHORT loc_3_38E1
        cmp    dword ptr [edx + 11h], edi
        je     SHORT loc_3_38E1
        mov    word ptr [esi + 50h], di
        mov    ecx, dword ptr [ebp + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edi, 5
        inc    eax
        mov    dword ptr [ecx + 40h], esi
        db  0A3h, 000h, 000h, 000h, 000h
        inc    byte ptr [esi + 7]
        cdq
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        idiv   edi
        db  089h, 015h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_3_38FB
loc_3_38E1:
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 40h], 0
        mov    ax, word ptr [edi + 2]
        mov    word ptr [esi + 50h], ax
        jmp    SHORT loc_3_38FE
loc_3_38F8:
        mov    ecx, dword ptr [ebp + 8]
loc_3_38FB:
        mov    edi, dword ptr [ebp + 10h]
loc_3_38FE:
        mov    byte ptr [ecx + 49h], 4
        lea    eax, [ecx + 44h]
        lea    edx, [ecx + 1ch]
        mov    word ptr [ecx + 0ah], 5205h
        mov    word ptr [ecx + 28h], 1eh
        mov    dword ptr [ecx + 24h], eax
        lea    eax, [edi + 0eh]
        mov    word ptr [ecx + 22h], 1
        mov    ebx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        mov    dword ptr [edx], ebx
        mov    word ptr [edx + 4], ax
        lea    eax, [edi + 14h]
        lea    edx, [ecx + 4ah]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [edx], ebx
        mov    ebx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [edx + 4], ebx
        lea    ebx, [esi + 3ch]
        mov    dword ptr [edx + 8], eax
        mov    edx, dword ptr [esi + 14h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 22h]
        mov    word ptr [ecx + 2eh], dx
        lea    edx, [eax + eax*2]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ecx + edx*2 + 24h], ebx
        mov    dx, word ptr [ecx + 22h]
        inc    dx
        test   eax, eax
        mov    word ptr [ecx + 22h], dx
        je     SHORT loc_3_3985
        mov    word ptr [ecx + 34h], ax
        mov    eax, dword ptr [esi + 28h]
        mov    ebx, dword ptr [eax + 4]
        movzx  eax, dx
        add    ebx, dword ptr [ebp - 4]
        lea    edx, [eax + eax*2]
        mov    dword ptr [ecx + edx*2 + 24h], ebx
        inc    word ptr [ecx + 22h]
loc_3_3985:
        mov    dword ptr [edi + 6], 0
        db  0C7h, 041h, 004h, 000h, 000h, 000h, 000h
        push   ecx
        push   3
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_399B:
        add    esp, 8
        cmp    al, 4
        jne    SHORT loc_3_39D3
        mov    byte ptr [esi + 6], 2
        mov    ax, word ptr [esi + 8]
        test   ax, ax
        jne    SHORT loc_3_39C2
        mov    word ptr [esi + 0ah], 64h
        mov    word ptr [esi + 4], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_39C2:
        mov    word ptr [esi + 0ah], ax
        pop    edi
        mov    word ptr [esi + 4], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_39D3:
        mov    al, byte ptr [esi + 6]
        test   al, al
        je     SHORT loc_3_39F3
        movzx  ax, al
        imul   ax, word ptr [edi]
        and    word ptr [esi + 2], 0fff7h
        mov    word ptr [esi + 4], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_39F3:
        cmp    word ptr [esi + 0ah], 0
        mov    ax, word ptr [edi]
        je     SHORT loc_3_3A12
        add    ax, 12h
        pop    edi
        and    word ptr [esi + 2], 0fff7h
        mov    word ptr [esi + 4], ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3A12:
        mov    word ptr [esi + 4], ax
        pop    edi
        and    word ptr [esi + 2], 0fff7h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3A30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 0ch]
        and    bl, 20h
        je     SHORT loc_3_3A4E
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        and    al, 80h
        lea    edi, [esi + 58h]
        mov    byte ptr [esi + 70h], al
        jmp    SHORT loc_3_3A7E
loc_3_3A4E:
        push   1ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3A55:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_3A71
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 36h
        mov    word ptr [esi + 1ah], 36h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_3A71:
        mov    al, byte ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        and    al, 82h
        or     al, 2
        mov    byte ptr [edi + 18h], al
loc_3_3A7E:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], ecx
        mov    byte ptr [edi + 19h], 0
        test   byte ptr [ebp + 0ch], 40h
        mov    dword ptr [esi + 50h], edi
        je     SHORT loc_3_3AC4
        cmp    dword ptr [esi], 0
        je     SHORT loc_3_3AC4
        push   esi
        call   sub_3_3AD0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3AC4
        test   bl, bl
        jne    SHORT loc_3_3AB4
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3AB1:
        add    esp, 4
loc_3_3AB4:
        mov    eax, 36h
        pop    edi
        mov    word ptr [esi + 1ah], 36h
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_3AC4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_3AD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 0ch], 0ffffh
        mov    dword ptr [ebp - 8], 0fff0h
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 50h]
        mov    eax, dword ptr [eax + 1ch]
        test   eax, eax
        je     SHORT loc_3_3B28
        mov    ecx, 80h
        test   word ptr [eax + 1ch], cx
        jne    SHORT loc_3_3B28
        mov    eax, dword ptr [eax + 10h]
        test   byte ptr [esi + 18h], cl
        mov    edx, dword ptr [eax + 76h]
        mov    bl, byte ptr [edx + 3]
        mov    byte ptr [ebp - 1], bl
        je     SHORT loc_3_3B28
        test   bl, 2
        je     SHORT loc_3_3B28
        mov    dword ptr [ebp - 0ch], 1400h
        mov    dword ptr [ebp - 8], 1000h
loc_3_3B28:
        cmp    byte ptr [esi + 19h], 2
        jne    SHORT loc_3_3B79
        mov    ecx, dword ptr [esi]
        cmp    ecx, 200h
        jb     SHORT loc_3_3B55
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_3_3B55
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0ch], ecx
        jb     NEAR PTR loc_3_3C74
        pop    edi
        mov    eax, ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3B55:
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3B5E:
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        sub    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [esi + 19h], 0
        sub    eax, ecx
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esi + 10h]
        add    eax, ecx
        mov    dword ptr [esi + 4], eax
loc_3_3B79:
        cmp    byte ptr [esi + 19h], 0
        jne    NEAR PTR loc_3_3C68
        mov    ecx, dword ptr [esi]
        test   ecx, ecx
        je     NEAR PTR loc_3_3C68
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 0ch], ecx
        mov    edi, eax
        mov    dword ptr [esi + 10h], eax
        shr    edi, 0ch
        lea    ebx, [ecx + eax]
        add    ebx, 0fffh
        mov    al, byte ptr [esi + 18h]
        shr    ebx, 0ch
        sub    ebx, edi
        test   al, 80h
        je     SHORT loc_3_3C13
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_3_3C13
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3BBB:
        lea    ecx, [ebx + 2]
        cmp    ecx, eax
        jbe    SHORT loc_3_3BDC
        cmp    eax, 2
        jb     SHORT loc_3_3C0A
        mov    ebx, 1
        jbe    SHORT loc_3_3BD1
        lea    ebx, [eax - 2]
loc_3_3BD1:
        lea    eax, [ebx + edi]
        shl    eax, 0ch
        sub    eax, dword ptr [esi + 10h]
        mov    dword ptr [esi], eax
loc_3_3BDC:
        lea    eax, [esi + 14h]
        mov    ecx, dword ptr [esi]
        push   eax
        mov    edx, dword ptr [esi + 10h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3BEC:
        add    esp, 0ch
        mov    dword ptr [esi + 4], eax
        test   eax, eax
        jne    SHORT loc_3_3BFF
        shr    ebx, 1
        test   ebx, ebx
        je     SHORT loc_3_3C0A
        jmp    SHORT loc_3_3BD1
loc_3_3BFF:
        mov    byte ptr [esi + 19h], 2
        mov    eax, dword ptr [esi]
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_3_3C68
loc_3_3C0A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3C13:
        test   al, 1
        jne    SHORT loc_3_3C68
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], edi
        push   eax
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3C27:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_3C44
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    byte ptr [esi + 19h], 0
        mov    word ptr [eax + 1ah], 5
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3C44:
        mov    byte ptr [esi + 19h], 1
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], ebx
        je     SHORT loc_3_3C68
        mov    ecx, dword ptr [esi + 4]
        and    ecx, 0fffh
        add    ecx, eax
        shr    eax, 0ch
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 14h], eax
loc_3_3C68:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0ch], ecx
        jb     SHORT loc_3_3C74
        mov    eax, ecx
loc_3_3C74:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_3C80:
        push   ebp
        mov    ebp, esp
        push   1ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3C8A:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_3CA4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edx
        mov    byte ptr [eax + 18h], 3
        mov    byte ptr [eax + 19h], 0
loc_3_3CA4:
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3CB0:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        sub    dword ptr [eax], ecx
        add    dword ptr [eax + 4], ecx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3CD0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 19h]
        cmp    eax, 1
        je     SHORT loc_3_3CE8
        cmp    eax, 2
        je     SHORT loc_3_3D00
        jmp    SHORT loc_3_3D0C
loc_3_3CE8:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 8]
        push   eax
        mov    edx, dword ptr [esi + 14h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3CFB:
        add    esp, 0ch
        jmp    SHORT loc_3_3D0C
loc_3_3D00:
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3D09:
        add    esp, 4
loc_3_3D0C:
        test   byte ptr [esi + 18h], 2
        je     SHORT loc_3_3D1B
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3D18:
        add    esp, 4
loc_3_3D1B:
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_3D20:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        cmp    ecx, -1
        push   esi
        push   edi
        jne    SHORT loc_3_3D3C
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_3_3D3C:
        mov    eax, ecx
        mov    edi, dword ptr [ebp + 8]
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 0ch]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3D60:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        cmp    ecx, -1
        push   esi
        push   edi
        jne    SHORT loc_3_3D7C
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_3_3D7C:
        mov    eax, ecx
        mov    edi, dword ptr [ebp + 8]
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 0ch]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3DA0:
        push   ebp
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        and    eax, 800h
        mov    edi, dword ptr [ecx + 1ch]
        cmp    eax, 1
        mov    al, 2
        mov    dword ptr [ebp - 18h], edi
        adc    al, 0ffh
        mov    byte ptr [ebp - 4], al
loc_3_3DCA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3DDF:
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        je     NEAR PTR loc_3_435E
        mov    esi, dword ptr [eax + 20h]
        test   esi, esi
        jne    NEAR PTR loc_3_431F
        mov    eax, dword ptr [eax + 38h]
        mov    edi, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [edi + 10h]
        test   byte ptr [ecx + 1bh], 2
        je     SHORT loc_3_3E11
        mov    ecx, dword ptr [ebp - 10h]
        or     byte ptr [ecx + 47h], 80h
loc_3_3E11:
        mov    edi, dword ptr [ebp - 18h]
        test   byte ptr [edi + 16h], 1
        je     SHORT loc_3_3E21
        mov    ecx, dword ptr [ebp - 10h]
        or     byte ptr [ecx + 46h], 1
loc_3_3E21:
        lea    eax, [ebp + 14h]
        mov    ebx, dword ptr [ebp - 14h]
        inc    ebx
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 1], 0
        mov    word ptr [ebp - 6], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 1ch], eax
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_3_3FDD
loc_3_3E44:
        mov    eax, dword ptr [ebp - 1ch]
        movsx  eax, byte ptr [eax]
        sub    eax, 2dh
        cmp    eax, 47h
        ja     NEAR PTR loc_3_3FCE
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_3_3E65:
        mov    dword ptr [ebx], 0ffffffffh
        add    ebx, 4
        jmp    NEAR PTR loc_3_3FCE
loc_3_3E73:
        mov    eax, ebx
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ebp - 1], 1
        sub    eax, dword ptr [ebp - 14h]
        sar    eax, 1
        mov    dword ptr [ebp - 14h], ebx
        add    ebx, 2
        mov    byte ptr [ecx], al
        jmp    NEAR PTR loc_3_3FCE
loc_3_3E8F:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_3_3EA8
        mov    eax, ebx
        mov    ecx, dword ptr [ebp - 14h]
        sub    eax, dword ptr [ebp - 14h]
        shr    al, 1
        mov    dword ptr [ebp - 14h], ebx
        add    ebx, 2
        mov    byte ptr [ecx], al
loc_3_3EA8:
        mov    eax, ebx
        mov    ecx, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 0ch], 4
        sub    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 0ch]
        sub    ax, 2
        mov    dword ptr [ebp - 14h], ebx
        mov    word ptr [ecx], ax
        mov    cl, byte ptr [edx - 4]
        mov    eax, dword ptr [ebp - 20h]
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [eax], cl
        mov    ecx, ebx
        sub    ecx, dword ptr [ebp - 10h]
        inc    ebx
        sub    cx, 3ch
        mov    word ptr [eax + 2], cx
        jmp    NEAR PTR loc_3_3FCE
loc_3_3EE0:
        add    dword ptr [ebp - 0ch], 4
        xor    esi, esi
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 4
        mov    cx, word ptr [eax - 4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 6], cx
        mov    edi, dword ptr [eax - 4]
        mov    edx, dword ptr [ebp - 6]
        mov    ecx, dword ptr [ebp - 10h]
        and    edx, 0ffffh
        mov    eax, dword ptr [ebp - 1ch]
        inc    eax
        mov    dword ptr [ecx + 2ch], edx
        mov    dword ptr [ecx + 28h], edi
        cmp    byte ptr [eax], 2bh
        jne    SHORT loc_3_3F23
        add    dword ptr [ebp - 0ch], 4
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax - 4]
loc_3_3F23:
        mov    eax, ebx
        mov    ecx, dword ptr [ebp - 14h]
        sub    eax, dword ptr [ebp - 10h]
        mov    edi, dword ptr [ebp - 18h]
        sub    eax, 33h
        mov    byte ptr [ecx], 0ch
        and    eax, 0fffffffch
        xor    ecx, ecx
        mov    cx, word ptr [edi + 22h]
        sub    ecx, eax
        cmp    ecx, edx
        jae    SHORT loc_3_3F47
        mov    word ptr [ebp - 6], cx
loc_3_3F47:
        mov    word ptr [ebx + 2], ax
        mov    edx, dword ptr [ebp - 6]
        and    edx, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    ebx, 4
        mov    dword ptr [ecx + 40h], edx
        mov    dx, word ptr [ebp - 6]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ebx - 4], dx
        mov    dword ptr [ebp - 14h], ebx
        lea    ebx, [eax + ecx + 3ch]
        test   esi, esi
        je     SHORT loc_3_3F7E
        mov    word ptr [ebx], si
        add    ebx, 2
        sub    word ptr [ebp - 6], 2
loc_3_3F7E:
        mov    byte ptr [ebp - 1], 1
        jmp    SHORT loc_3_3FCE
loc_3_3F84:
        mov    byte ptr [ebx], 4
        inc    ebx
loc_3_3F88:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [eax - 4]
        mov    eax, dword ptr [edi + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_3_3FA8
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 10h]
        and    eax, 1
        add    ebx, eax
loc_3_3FA8:
        push   0
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   208h
        push   ebx
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3FBE:
        add    esp, 18h
        mov    esi, eax
        cmp    esi, -1
        je     NEAR PTR loc_3_42EC
        add    ebx, esi
loc_3_3FCE:
        inc    dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 1ch]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_3_3E44
loc_3_3FDD:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_3_3FF6
        mov    eax, ebx
        mov    ecx, dword ptr [ebp - 14h]
        sub    eax, dword ptr [ebp - 14h]
        shr    al, 1
        mov    dword ptr [ebp - 14h], ebx
        add    ebx, 2
        mov    byte ptr [ecx], al
loc_3_3FF6:
        mov    eax, ebx
        mov    ecx, dword ptr [ebp - 14h]
        sub    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 10h]
        add    ax, word ptr [ebp - 6]
        sub    ax, 2
        mov    word ptr [ecx], ax
        mov    ax, word ptr [edx + 2]
        or     al, 1
        mov    edi, dword ptr [ebp - 18h]
        mov    cx, ax
        mov    word ptr [edx + 2], ax
        and    cl, 80h
        and    ax, 0ff7fh
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [edx + 2], ax
        mov    eax, dword ptr [edi + 10h]
        mov    edx, dword ptr [eax + 76h]
        test   edx, edx
        je     NEAR PTR loc_3_42F7
        mov    ecx, dword ptr [ebp - 6]
        sub    ebx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        sub    ebx, 3ch
        push   ecx
        push   ebx
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   dword ptr [edx + 30h]
        add    esp, 10h
        mov    esi, eax
        jmp    NEAR PTR loc_3_42FC
loc_3_405B:
        mov    byte ptr [ebx], 1
        inc    ebx
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        add    ebx, 2
        add    dword ptr [ebp - 0ch], 4
        mov    cx, word ptr [eax - 4]
        mov    edx, dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebx - 2], cx
        mov    eax, dword ptr [ebp - 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [edx - 4]
        mov    dword ptr [ecx + 28h], eax
        jmp    NEAR PTR loc_3_3FCE
loc_3_4095:
        dec    ebx
        jmp    NEAR PTR loc_3_3F88
loc_3_409B:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        add    ebx, 4
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [ebx - 4], ecx
        jmp    NEAR PTR loc_3_3FCE
loc_3_40B0:
        push   ebx
        mov    ecx, dword ptr [ebp - 10h]
        add    dword ptr [ebp - 0ch], 4
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [eax - 4]
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, dword ptr [ecx + 38h]
        jmp    NEAR PTR loc_3_3FCE
loc_3_40D1:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [eax - 4]
        mov    eax, dword ptr [ebp - 1ch]
        inc    eax
        mov    dword ptr [ecx + 24h], edx
        cmp    byte ptr [eax], 21h
        jne    SHORT loc_3_40F2
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_3_3FCE
loc_3_40F2:
        cmp    edx, 1
        sbb    al, al
        inc    al
        mov    byte ptr [ebp - 3], al
        jmp    NEAR PTR loc_3_3FCE
loc_3_4101:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [eax - 4]
        mov    word ptr [ecx + 8], dx
        jmp    NEAR PTR loc_3_3FCE
loc_3_4117:
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebx], 5
        inc    ebx
        cmp    byte ptr [ecx + 4], 0
        jne    SHORT loc_3_4131
        mov    word ptr [ebx], 0
        add    ebx, 2
        jmp    NEAR PTR loc_3_3FCE
loc_3_4131:
        mov    word ptr [ebx], 15h
        add    ebx, 2
        mov    edi, ebx
        mov    ecx, dword ptr [ebp + 8]
        add    ebx, 15h
        mov    esi, dword ptr [ecx + 14h]
        mov    ecx, 5
        rep movsd
        movsb
        jmp    NEAR PTR loc_3_3FCE
loc_3_4151:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 20h]
        cmp    dword ptr [eax + 28h], 0
        je     NEAR PTR loc_3_3FCE
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_416F:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ecx + 38h]
        mov    dword ptr [ebp - 14h], edx
        lea    ebx, [edx + 1]
        jmp    NEAR PTR loc_3_3FCE
loc_3_4183:
        mov    byte ptr [ebp - 2], 1
        jmp    NEAR PTR loc_3_3FCE
loc_3_418C:
        mov    edi, dword ptr [ebp - 18h]
        xor    eax, eax
        add    dword ptr [ebp - 0ch], 4
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [edi + 10h]
        add    dword ptr [ebp - 0ch], 4
        mov    ax, word ptr [ecx - 4]
        mov    esi, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [edx + 76h]
        mov    eax, dword ptr [ecx - 4]
        push   eax
        push   edx
        push   esi
        call   dword ptr [edi + 34h]
        add    esp, 10h
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_3_431F
        mov    word ptr [ebp - 6], 0
        jmp    NEAR PTR loc_3_3FCE
loc_3_41CE:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        add    dword ptr [ebp - 0ch], 4
        mov    edx, dword ptr [eax - 4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 34h], edx
        mov    edx, dword ptr [eax - 4]
        mov    dword ptr [ecx + 30h], edx
        jmp    NEAR PTR loc_3_3FCE
loc_3_41F0:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 4
        mov    cx, word ptr [eax - 4]
        mov    edx, dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 6], cx
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 6]
        and    eax, 0ffffh
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [edx - 4]
        mov    dword ptr [ecx + 28h], eax
        jmp    NEAR PTR loc_3_3FCE
loc_3_421F:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [eax - 4]
        mov    word ptr [ecx + 58h], dx
        jmp    NEAR PTR loc_3_3FCE
loc_3_4235:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        add    ebx, 2
        mov    ecx, dword ptr [eax - 4]
        mov    word ptr [ebx - 2], cx
        jmp    NEAR PTR loc_3_3FCE
loc_3_424B:
        mov    dword ptr [ebp - 20h], ebx
        mov    byte ptr [ebx], 0ffh
        mov    byte ptr [ebx + 1], 0
        inc    ebx
        inc    ebx
loc_3_4257:
        mov    word ptr [ebx], 0
        add    ebx, 2
        jmp    NEAR PTR loc_3_3FCE
loc_3_4264:
        mov    dword ptr [ebx], 0
        add    ebx, 4
        jmp    NEAR PTR loc_3_3FCE
loc_3_4272:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    esi, dword ptr [eax - 4]
        sub    eax, eax
        mov    edi, esi
        repne scasb
        not    ecx
        mov    eax, ecx
        mov    edi, ebx
        shr    ecx, 2
        add    ebx, eax
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    NEAR PTR loc_3_3FCE
loc_3_42A4:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax - 4]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        mov    dword ptr [ebx], ecx
        push   ebx
        call   sub_3_14D0
        add    esp, 8
        add    ebx, 4
        jmp    NEAR PTR loc_3_3FCE
loc_3_42C8:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax - 4]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        mov    dword ptr [ebx], ecx
        push   ebx
        call   sub_3_1570
        add    esp, 8
        add    ebx, 4
        jmp    NEAR PTR loc_3_3FCE
loc_3_42EC:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 8
        jmp    SHORT loc_3_431F
loc_3_42F7:
        mov    esi, 37h
loc_3_42FC:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_3_4307
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4307:
        test   esi, esi
        jne    SHORT loc_3_431F
        cmp    byte ptr [ebp - 3], 0
        jne    SHORT loc_3_436C
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_431A:
        add    esp, 4
        mov    esi, eax
loc_3_431F:
        cmp    byte ptr [ebp - 2], 0
        jne    SHORT loc_3_4331
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_432E:
        add    esp, 4
loc_3_4331:
        test   esi, esi
        je     SHORT loc_3_434D
        cmp    esi, 36h
        je     SHORT loc_3_4344
        cmp    esi, 37h
        je     SHORT loc_3_4344
        cmp    esi, 40h
        jne    SHORT loc_3_434D
loc_3_4344:
        dec    byte ptr [ebp - 4]
        jne    NEAR PTR loc_3_3DCA
loc_3_434D:
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        movsx  eax, si
        mov    word ptr [ecx + 1ah], si
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_435E:
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        movsx  eax, word ptr [ecx + 1ah]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_436C:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  01Ah, 01Ah, 001h, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 002h, 003h, 01Ah, 01Ah
        db  01Ah, 004h, 01Ah, 005h, 006h, 007h, 008h, 01Ah, 009h, 01Ah, 00Ah, 00Bh, 01Ah, 00Ch, 00Dh, 01Ah
        db  01Ah, 00Eh, 01Ah, 00Fh, 010h, 011h, 012h, 013h, 01Ah, 014h, 015h, 01Ah, 016h, 01Ah, 01Ah, 01Ah
        db  01Ah, 01Ah, 01Ah, 017h, 01Ah, 01Ah, 018h, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah
        db  01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 019h, 08Dh, 064h, 024h, 000h
_PTEXT ends
        end
