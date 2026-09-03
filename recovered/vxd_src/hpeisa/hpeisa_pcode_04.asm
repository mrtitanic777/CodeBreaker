; hpeisa_pcode_04.asm -- HPEISA object 4 segment _PTEXT
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
        call   sub_4_27D0
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
sub_4_0170:
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0200:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 4bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0210:
        dec    eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0220:
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
        call   sub_4_3810
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
        call   sub_4_1960
        test   al, al
        je     SHORT loc_4_0300
        mov    dword ptr [esi + 0b3h], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3630
        push   esi
        call   sub_4_1990
loc_4_0300:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0310:
        push   ebp
        mov    eax, 15h
        mov    ebp, esp
        sub    esp, 16ch
        mov    word ptr [ebp - 2ch], ax
        push   ebx
        mov    word ptr [ebp - 2ah], ax
        push   esi
        db  0C7h, 045h, 0D8h, 000h, 000h, 000h, 000h
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 1ch], ecx
        push   ebx
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 10h], 0
        lea    eax, [ebp - 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0360:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_0374
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0374:
        lea    eax, [ebp - 16ch]
        lea    ecx, [ebp - 10h]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0389:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_0430
        cmp    dword ptr [ebp - 16ch], 4018f022h
        jne    NEAR PTR loc_4_0430
        mov    bl, byte ptr [ebp - 0bah]
        and    ebx, 0fh
        test   byte ptr [ebp - 0bah], 20h
        jne    SHORT loc_4_03BC
        mov    dword ptr [ebp - 0ch], 1
loc_4_03BC:
        mov    al, byte ptr [ebp - 0ach]
        push   0
        and    eax, 7
        lea    ecx, [ebp - 2ch]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 4]
        shl    eax, 0ch
        add    eax, 0c00h
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 24h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03ED:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_03F9
        mov    eax, dword ptr [ebp - 24h]
        mov    esi, dword ptr [eax + 4]
loc_4_03F9:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0402:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [edi + 4], ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [edi + 8], edx
        mov    dword ptr [edi + 0ch], ebx
        mov    dword ptr [edi + 10h], eax
        xor    eax, eax
        mov    dword ptr [edi + 14h], ecx
        mov    dword ptr [edi + 18h], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0430:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0439:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0450:
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
        je     SHORT loc_4_0495
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0495:
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
        jne    SHORT loc_4_04CB
        mov    dword ptr [ecx + 20h], 1
        jmp    SHORT loc_4_04D8
loc_4_04CB:
        cmp    dword ptr [ebp - 0ch], 1
        jne    SHORT loc_4_04D8
        mov    dword ptr [ecx + 20h], 2
loc_4_04D8:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 209h], eax
        mov    dword ptr [ecx + 20dh], edx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0500:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     SHORT loc_4_0521
        mov    eax, 0c0010002h
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0521:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 10h]
        test   ecx, ecx
        jne    SHORT loc_4_053B
        push   edx
        push   esi
        push   edi
        call   sub_4_0950
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_053B:
        cmp    ecx, 1
        jne    SHORT loc_4_0573
        mov    ecx, dword ptr [edi + 47h]
        test   ecx, ecx
        je     SHORT loc_4_0563
        test   cl, 2
        je     SHORT loc_4_0551
        mov    eax, 0c0000001h
loc_4_0551:
        test   cl, 1
        je     SHORT loc_4_0578
        mov    eax, 0c001000dh
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0563:
        push   edx
        push   esi
        push   edi
        call   sub_4_09A0
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0573:
        mov    eax, 10001h
loc_4_0578:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_0580:
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
        ja     SHORT loc_4_05BF
        je     NEAR PTR loc_4_07A5
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_4_05E0
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_05BF:
        cmp    eax, 1010102h
        je     NEAR PTR loc_4_07C4
        cmp    eax, 1010103h
        je     NEAR PTR loc_4_07E3
        cmp    eax, 1010104h
        je     NEAR PTR loc_4_0893
loc_4_05E0:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_4_08A9
loc_4_05EC:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_4_0601
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_4_08AF
loc_4_0601:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_4_08AF
loc_4_0610:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 47h]
        test   al, 2
        je     SHORT loc_4_0623
        mov    dword ptr [ebp - 18h], 4
        jmp    SHORT loc_4_0635
loc_4_0623:
        mov    dword ptr [ebp - 18h], 2
        test   al, 1
        jne    SHORT loc_4_0635
        mov    dword ptr [ebp - 18h], 0
loc_4_0635:
        lea    eax, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_4_08AF
loc_4_0642:
        lea    eax, [ebp - 1ch]
        mov    edi, 4
        jmp    NEAR PTR loc_4_08AF
loc_4_064F:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_4_08AF
loc_4_0663:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_4_08AF
loc_4_0677:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_4_08AF
loc_4_068B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a3h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_08AF
loc_4_06A4:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a7h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_08AF
loc_4_06BD:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_08AF
loc_4_06D1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0abh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_08AF
loc_4_06EA:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0701:
        mov    byte ptr [ebp - 21h], 0
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_08AF
loc_4_070D:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 21h
        jmp    NEAR PTR loc_4_08AF
loc_4_071C:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_072D
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7bh]
        mov    ecx, dword ptr [eax + 0ch]
        jmp    SHORT loc_4_0736
loc_4_072D:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [eax + 8]
loc_4_0736:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_4_08AF
loc_4_0746:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_0754
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7fh]
        jmp    SHORT loc_4_075A
loc_4_0754:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 21h]
loc_4_075A:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_08AF
loc_4_076A:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_4_08AF
loc_4_077D:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_08AF
loc_4_0791:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_4_08AF
loc_4_07A5:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07BC:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_08AF
loc_4_07C4:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 67h
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07DB:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_08AF
loc_4_07E3:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_0839
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 7bh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_0810
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_08A9
loc_4_0810:
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
loc_4_0826:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ebp - 0ch], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_08A9
loc_4_0839:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [edi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_084C:
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_0867
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_08A9
loc_4_0867:
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
loc_4_0884:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_08A9
loc_4_0893:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 20dh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    SHORT loc_4_08AF
loc_4_08A9:
        mov    eax, dword ptr [ebp - 14h]
        mov    edi, dword ptr [ebp - 14h]
loc_4_08AF:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_08EA
        cmp    esi, 1010103h
        je     SHORT loc_4_08EA
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_4_08DA
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
loc_4_08DA:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08E5:
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_4_08EA:
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
sub_4_0950:
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
        call   sub_4_0580
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_09A0:
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
        ja     SHORT loc_4_09DD
        je     NEAR PTR loc_4_0AAF
        cmp    eax, 1010eh
        je     SHORT loc_4_09EF
        cmp    eax, 1010fh
        je     SHORT loc_4_0A29
        cmp    eax, 10112h
        je     NEAR PTR loc_4_0A9B
loc_4_09DD:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0AEA
loc_4_09EF:
        cmp    edi, 4
        je     SHORT loc_4_0A06
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0AEA
loc_4_0A06:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A12:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0B10
        jmp    NEAR PTR loc_4_0AE8
loc_4_0A29:
        cmp    edi, 4
        je     SHORT loc_4_0A40
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0AEA
loc_4_0A40:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A4C:
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_4_0A94
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 7fh]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_4_0A6E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 7fh], eax
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0AEA
loc_4_0A6E:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx + 21h]
        jne    SHORT loc_4_0A8C
        mov    eax, dword ptr [ecx + 21h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_4_0A8C
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 21h], eax
        call   sub_4_0D60
        jmp    SHORT loc_4_0AEA
loc_4_0A8C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0AEA
loc_4_0A94:
        mov    ebx, 0c0000001h
        jmp    SHORT loc_4_0AEA
loc_4_0A9B:
        cmp    edi, 4
        je     SHORT loc_4_0AEA
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0AEA
loc_4_0AAF:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_4_0ACD
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0AEA
loc_4_0ACD:
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
        call   sub_4_0B70
loc_4_0AE8:
        mov    ebx, eax
loc_4_0AEA:
        test   ebx, ebx
        jne    SHORT loc_4_0AF8
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_4_0AF8:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B10:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0B52
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0B48
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
loc_4_0B41:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    10h
loc_4_0B48:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_4_0B52:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B70:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0BB6
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0BAC
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
loc_4_0BA5:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    14h
loc_4_0BAC:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_4_0BB6:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    14h
        db  08Dh, 049h, 000h
sub_4_0BD0:
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
        call   sub_4_0580
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_4_0CA5
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_4_0C35
        je     SHORT loc_4_0C74
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_4_0C43
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0C35:
        cmp    eax, 1020102h
        je     SHORT loc_4_0C7C
        cmp    eax, 1020103h
        je     SHORT loc_4_0C84
loc_4_0C43:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    SHORT loc_4_0C8D
loc_4_0C4C:
        mov    eax, dword ptr [ebx + 83h]
        jmp    SHORT loc_4_0C8A
loc_4_0C54:
        mov    eax, dword ptr [ebx + 87h]
        jmp    SHORT loc_4_0C8A
loc_4_0C5C:
        mov    eax, dword ptr [ebx + 8bh]
        jmp    SHORT loc_4_0C8A
loc_4_0C64:
        mov    eax, dword ptr [ebx + 9bh]
        jmp    SHORT loc_4_0C8A
loc_4_0C6C:
        mov    eax, dword ptr [ebx + 9fh]
        jmp    SHORT loc_4_0C8A
loc_4_0C74:
        mov    eax, dword ptr [ebx + 97h]
        jmp    SHORT loc_4_0C8A
loc_4_0C7C:
        mov    eax, dword ptr [ebx + 8fh]
        jmp    SHORT loc_4_0C8A
loc_4_0C84:
        mov    eax, dword ptr [ebx + 93h]
loc_4_0C8A:
        mov    dword ptr [ebp - 4], eax
loc_4_0C8D:
        cmp    edi, 4
        jb     SHORT loc_4_0CA5
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CA1:
        add    dword ptr [ebp - 0ch], 4
loc_4_0CA5:
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
loc_4_0CE0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [ecx + 14h]
        cmp    edx, 10113h
        ja     SHORT loc_4_0D08
        je     SHORT loc_4_0D3F
        cmp    edx, 10101h
        jb     SHORT loc_4_0D3A
        cmp    edx, 10111h
        jbe    SHORT loc_4_0D3F
        jmp    SHORT loc_4_0D3A
loc_4_0D08:
        cmp    edx, 1010104h
        ja     SHORT loc_4_0D2A
        cmp    edx, 1010101h
        jae    SHORT loc_4_0D3F
        cmp    edx, 20101h
        jb     SHORT loc_4_0D3A
        cmp    edx, 20105h
        jbe    SHORT loc_4_0D3F
        jmp    SHORT loc_4_0D3A
loc_4_0D2A:
        cmp    edx, 1020101h
        jb     SHORT loc_4_0D3A
        cmp    edx, 1020103h
        jbe    SHORT loc_4_0D3F
loc_4_0D3A:
        mov    eax, 0c0010017h
loc_4_0D3F:
        test   eax, eax
        jne    SHORT loc_4_0D4D
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0BD0
loc_4_0D4D:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D60:
        push   ebp
        mov    ecx, 0eeh
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 1c5h]
        test   edx, edx
        je     SHORT loc_4_0D86
loc_4_0D76:
        mov    esi, dword ptr [edx + 21h]
        cmp    ecx, esi
        jae    SHORT loc_4_0D7F
        mov    ecx, esi
loc_4_0D7F:
        mov    edx, dword ptr [edx + 11h]
        test   edx, edx
        jne    SHORT loc_4_0D76
loc_4_0D86:
        pop    esi
        mov    dword ptr [eax + 7fh], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0D90:
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
loc_4_0DB7:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     SHORT loc_4_0DCE
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0DCE:
        mov    eax, dword ptr [esi + 47h]
        test   eax, eax
        je     SHORT loc_4_0DF9
        test   al, 0ah
        je     SHORT loc_4_0DE7
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0DE7:
        test   al, 1
        je     SHORT loc_4_0DF9
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0DF9:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_0E20
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FB0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E20:
        push   esi
        call   sub_4_1960
        test   al, al
        jne    SHORT loc_4_0E4A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FB0
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 46h], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E4A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E5B:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2C80
        test   eax, eax
        je     SHORT loc_4_0EA9
        jle    SHORT loc_4_0E97
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FB0
        push   esi
        call   sub_4_1990
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E97:
        test   eax, eax
        jge    SHORT loc_4_0EA9
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0EA9:
        xor    edi, edi
        xor    eax, eax
loc_4_0EAD:
        cmp    eax, dword ptr [ebp - 8]
        jae    SHORT loc_4_0EC7
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0b6h], cl
        jb     SHORT loc_4_0EAD
loc_4_0EC7:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        je     SHORT loc_4_0F2B
loc_4_0ECF:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EDC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EED:
        xor    ecx, ecx
        cmp    edi, 6
        jae    SHORT loc_4_0F0E
loc_4_0EF4:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_0F0E
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0b6h], al
        jb     SHORT loc_4_0EF4
loc_4_0F0E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2D40
        test   eax, eax
        jl     SHORT loc_4_0F45
        inc    ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, ebx
        ja     SHORT loc_4_0ECF
loc_4_0F2B:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        test   eax, eax
        jge    SHORT loc_4_0F53
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F45:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F53:
        lea    eax, [esi + 0b7h]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F63:
        test   al, al
        je     SHORT loc_4_0F89
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FF0
        push   esi
        call   sub_4_1990
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F89:
        push   esi
        inc    dword ptr [esi + 83h]
        call   sub_4_1990
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_16E0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0FB0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_0FDC
        pop    ebp
        mov    dword ptr [eax + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
loc_4_0FDC:
        mov    ecx, dword ptr [eax + 28h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_0FF0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 38h], 0
        jne    SHORT loc_4_1019
        pop    ebp
        mov    dword ptr [eax + 38h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
loc_4_1019:
        mov    ecx, dword ptr [eax + 34h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1030:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 24h], 0
        mov    eax, dword ptr [ecx + 24h]
        test   eax, eax
        jne    SHORT loc_4_1055
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [ecx + 28h], edx
        inc    dword ptr [ecx + 2ch]
        ret    8
loc_4_1055:
        pop    ebp
        mov    dword ptr [edx + 24h], eax
        mov    dword ptr [ecx + 24h], edx
        inc    dword ptr [ecx + 2ch]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1070:
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
        je     SHORT loc_4_10C8
        test   al, 1
        je     SHORT loc_4_1094
        inc    dword ptr [esi + 30h]
loc_4_1094:
        test   al, 2
        je     SHORT loc_4_10A1
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 93h]
loc_4_10A1:
        test   al, 4
        je     SHORT loc_4_10AE
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_10AE:
        test   al, 8
        je     SHORT loc_4_10BB
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_10BB:
        test   al, 10h
        je     SHORT loc_4_10C8
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_10C8:
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_4_10DC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_10DC:
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
loc_4_10FC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_110D:
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2C80
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_1157
        push   edi
        push   esi
        call   sub_4_1030
        test   ebx, ebx
        jge    SHORT loc_4_1149
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
loc_4_1149:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1157:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_115B:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_1175
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0b6h], al
        jb     SHORT loc_4_115B
loc_4_1175:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 14h]
        dec    eax
        je     SHORT loc_4_11E2
loc_4_1182:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_118F:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_11A0:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    SHORT loc_4_11C1
loc_4_11A7:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_11C1
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0b6h], al
        jb     SHORT loc_4_11A7
loc_4_11C1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2D40
        cmp    eax, -2
        je     SHORT loc_4_1204
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_4_1182
loc_4_11E2:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        cmp    eax, -2
        jne    SHORT loc_4_1219
        push   edi
        push   esi
        call   sub_4_1030
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1204:
        push   edi
        push   esi
        call   sub_4_1030
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1219:
        lea    eax, [esi + 0b7h]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1229:
        test   al, al
        je     SHORT loc_4_1249
        push   edi
        mov    eax, dword ptr [edi + 28h]
        push   eax
        push   esi
        call   sub_4_0FF0
        push   esi
        call   sub_4_3A70
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1249:
        push   0
        inc    dword ptr [esi + 83h]
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_125E:
        push   esi
        call   sub_4_16E0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_4_1270:
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
loc_4_1295:
        test   edx, edx
        je     SHORT loc_4_12A4
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_4_1295
        mov    dword ptr [ecx], edx
loc_4_12A4:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_4_12B7
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_12B7:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   25h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12CA:
        test   eax, eax
        je     SHORT loc_4_12DC
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_12DC:
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
loc_4_12F7:
        test   al, al
        jne    SHORT loc_4_1316
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   25h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1308:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_1316:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_1338
        mov    eax, dword ptr [esi + 1cdh]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 103h
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1cdh], ecx
        jmp    SHORT loc_4_134F
loc_4_1338:
        mov    eax, dword ptr [esi + 1c5h]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1c5h], ecx
loc_4_134F:
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
        jne    SHORT loc_4_13A2
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
loc_4_13A2:
        mov    eax, ebx
        pop    edi
        inc    dword ptr [esi + 1c9h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_13C0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        xor    eax, eax
loc_4_13CF:
        mov    dl, byte ptr [esi + eax + 67h]
        mov    ecx, dword ptr [esi + 73h]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     SHORT loc_4_13CF
        mov    edi, dword ptr [esi + 73h]
        add    edi, 6
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_1407
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 10h]
loc_4_13F2:
        xor    ecx, ecx
loc_4_13F4:
        mov    dl, byte ptr [eax + ecx]
        inc    edi
        inc    ecx
        mov    byte ptr [edi - 1], dl
        cmp    ecx, 6
        jb     SHORT loc_4_13F4
        add    eax, 6
        dec    ebx
        jne    SHORT loc_4_13F2
loc_4_1407:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 77h], eax
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_1420
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 44h], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_1420:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_142F:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_29A0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2A80
        test   eax, eax
        jge    SHORT loc_4_146E
        mov    eax, 0c0000001h
        pop    edi
        or     dword ptr [esi + 47h], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_146E:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  090h
loc_4_1480:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax + 0dh]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 0ffffffd0h
        je     SHORT loc_4_14A0
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_14A0:
        test   al, 1
        je     SHORT loc_4_14A9
        mov    ecx, 1
loc_4_14A9:
        test   al, 2
        je     SHORT loc_4_14B0
        or     ecx, 4
loc_4_14B0:
        test   al, 4
        je     SHORT loc_4_14B7
        or     ecx, 8
loc_4_14B7:
        test   al, 8
        je     SHORT loc_4_14BE
        or     ecx, 2
loc_4_14BE:
        test   al, 20h
        je     SHORT loc_4_14C5
        or     ecx, 10h
loc_4_14C5:
        test   dword ptr [esi + 1bdh], ecx
        je     SHORT loc_4_14D7
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_14D7:
        mov    dword ptr [esi + 1b9h], ecx
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_14EE
        mov    byte ptr [esi + 43h], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_4_14EE:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14FD:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 1b9h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2A80
        test   eax, eax
        jge    SHORT loc_4_1526
        or     dword ptr [esi + 47h], 2
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_4_1526:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1540:
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
loc_4_1561:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1570:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 1eeh]
        test   ecx, ecx
        je     SHORT loc_4_15A0
loc_4_1581:
        mov    eax, dword ptr [esi + 1eeh]
        push   ecx
        mov    edx, dword ptr [eax + 15h]
        mov    dword ptr [esi + 1eeh], edx
        call   sub_4_16E0
        mov    ecx, dword ptr [esi + 1eeh]
        test   ecx, ecx
        jne    SHORT loc_4_1581
loc_4_15A0:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 47h], 8
        jne    SHORT loc_4_15E6
        push   esi
        call   sub_4_1960
        test   al, al
        je     SHORT loc_4_15D5
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3420
        push   esi
        call   sub_4_1990
loc_4_15D5:
        push   3e8h
        add    esi, 1ddh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_15E6:
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_4_15F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 4], 0
        mov    esi, dword ptr [edi + 0dh]
        je     SHORT loc_4_160E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_160E:
        inc    dword ptr [edi]
        mov    eax, dword ptr [esi + 1c5h]
        cmp    edi, eax
        jne    SHORT loc_4_1625
        mov    eax, dword ptr [edi + 11h]
        mov    dword ptr [esi + 1c5h], eax
        jmp    SHORT loc_4_1638
loc_4_1625:
        cmp    dword ptr [eax + 11h], edi
        je     SHORT loc_4_1632
loc_4_162A:
        mov    eax, dword ptr [eax + 11h]
        cmp    dword ptr [eax + 11h], edi
        jne    SHORT loc_4_162A
loc_4_1632:
        mov    ecx, dword ptr [edi + 11h]
        mov    dword ptr [eax + 11h], ecx
loc_4_1638:
        push   0
        mov    eax, dword ptr [edi + 1dh]
        push   eax
        mov    ecx, dword ptr [esi + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1647:
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_165F
        cmp    ebx, 103h
        je     SHORT loc_4_1673
        cmp    ebx, 0c001000eh
        je     SHORT loc_4_167C
        jmp    SHORT loc_4_1685
loc_4_165F:
        mov    eax, dword ptr [edi]
        sub    eax, 2
        mov    dword ptr [edi], eax
        je     SHORT loc_4_1687
        mov    byte ptr [edi + 4], 1
        mov    ebx, 103h
        jmp    SHORT loc_4_1687
loc_4_1673:
        mov    byte ptr [edi + 4], 1
        sub    dword ptr [edi], 2
        jmp    SHORT loc_4_1687
loc_4_167C:
        mov    byte ptr [edi + 4], 1
        mov    ebx, 103h
loc_4_1685:
        dec    dword ptr [edi]
loc_4_1687:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_4_16CB
        mov    eax, dword ptr [edi + 21h]
        cmp    dword ptr [esi + 7fh], eax
        jne    SHORT loc_4_169A
        push   esi
        call   sub_4_0D60
loc_4_169A:
        cmp    dword ptr [esi + 1c5h], 0
        jne    SHORT loc_4_16B2
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16B2:
        push   0
        dec    dword ptr [esi + 1c9h]
        push   25h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16C2:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_16CB:
        mov    ebx, 103h
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_16E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    SHORT loc_4_1733
        mov    eax, dword ptr [esi + 21h]
        cmp    dword ptr [edi + 7fh], eax
        jne    SHORT loc_4_1700
        push   edi
        call   sub_4_0D60
loc_4_1700:
        cmp    dword ptr [edi + 1c5h], 0
        jne    SHORT loc_4_1718
        push   edi
        lea    eax, [edi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1718:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1723:
        push   0
        push   25h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_172D:
        dec    dword ptr [edi + 1c9h]
loc_4_1733:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1740:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        cmp    byte ptr [eax + 4], 0
        je     SHORT loc_4_175D
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_175D:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_176E
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_176E:
        push   40010004h
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 1eah], eax
        call   sub_4_1900
        or     dword ptr [esi + 47h], 1
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        je     SHORT loc_4_17B7
loc_4_178C:
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
loc_4_17AA:
        push   edi
        call   sub_4_16E0
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        jne    SHORT loc_4_178C
loc_4_17B7:
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_17E3
loc_4_17BE:
        mov    eax, dword ptr [ecx + 24h]
        push   0c001000ch
        push   ecx
        mov    dword ptr [esi + 38h], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17D6:
        push   edi
        call   sub_4_16E0
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        jne    SHORT loc_4_17BE
loc_4_17E3:
        mov    byte ptr [esi + 1ffh], 1
        push   esi
        call   sub_4_1960
        test   al, al
        je     SHORT loc_4_180B
        push   esi
        call   sub_4_1820
        push   esi
        call   sub_4_1990
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_180B:
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 1ffh], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_1820:
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
        call   sub_4_27D0
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_29A0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2A80
        test   eax, eax
        jge    SHORT loc_4_1870
        or     dword ptr [esi + 47h], 2
loc_4_1870:
        push   40010005h
        push   esi
        call   sub_4_1900
        mov    eax, dword ptr [esi + 47h]
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 47h], eax
        test   al, 2
        je     SHORT loc_4_188F
        push   80010004h
        jmp    SHORT loc_4_1891
loc_4_188F:
        push   0
loc_4_1891:
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_189A:
        push   edi
        call   sub_4_16E0
        push   esi
        call   sub_4_18B0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_18B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 1cdh]
        test   ecx, ecx
        je     SHORT loc_4_18F6
loc_4_18C1:
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
loc_4_18EC:
        mov    ecx, dword ptr [esi + 1cdh]
        test   ecx, ecx
        jne    SHORT loc_4_18C1
loc_4_18F6:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1900:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 1c5h]
        test   edi, edi
        je     SHORT loc_4_194B
        mov    esi, dword ptr [ebp + 0ch]
loc_4_191A:
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
loc_4_1936:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_193F:
        push   edi
        call   sub_4_16E0
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_4_191A
loc_4_194B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1960:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_4_1973
        xor    al, al
        pop    ebp
        ret    4
loc_4_1973:
        mov    byte ptr [eax + 40h], 1
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1986:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1990:
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
        je     NEAR PTR loc_4_1ABE
loc_4_19CD:
        cmp    byte ptr [esi + 1ffh], 0
        je     SHORT loc_4_19DE
        mov    bl, 1
        push   esi
        call   sub_4_1820
loc_4_19DE:
        cmp    byte ptr [esi + 44h], 0
        je     SHORT loc_4_1A21
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_29A0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 1b9h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2A80
        test   eax, eax
        jge    SHORT loc_4_1A1F
        or     dword ptr [esi + 47h], 2
loc_4_1A1F:
        mov    bl, 1
loc_4_1A21:
        cmp    byte ptr [esi + 43h], 0
        je     SHORT loc_4_1A4C
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 1b9h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2A80
        test   eax, eax
        jge    SHORT loc_4_1A4A
        or     dword ptr [esi + 47h], 2
loc_4_1A4A:
        mov    bl, 1
loc_4_1A4C:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_1A6A
        mov    byte ptr [esi + 46h], 0
loc_4_1A56:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_1A68
        push   0
        push   esi
        call   sub_4_1070
        test   eax, eax
        je     SHORT loc_4_1A56
loc_4_1A68:
        mov    bl, 1
loc_4_1A6A:
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_4_1A78
        push   esi
        mov    bl, 1
        call   sub_4_3A70
loc_4_1A78:
        cmp    byte ptr [esi + 41h], 0
        je     SHORT loc_4_1A8B
        mov    byte ptr [esi + 41h], 0
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A8B:
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
        jne    NEAR PTR loc_4_19CD
loc_4_1ABE:
        mov    byte ptr [esi + 40h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1AE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3810
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 031h, 01Dh, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1B20:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 18h
        mov    word ptr [ebp - 0ch], ax
        push   ebx
        mov    word ptr [ebp - 0ah], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 16h], bx
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 18h]
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], 4018f022h
        mov    word ptr [ebp - 18h], 1
        push   ecx
        push   eax
        db  068h, 00Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B61:
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        db  089h, 015h, 046h, 000h, 000h, 000h
        db  089h, 00Dh, 04Ah, 000h, 000h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 01Dh, 052h, 000h, 000h, 000h
        call   sub_4_2330
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
loc_4_1C34:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_1C61
        db  039h, 01Dh, 004h, 000h, 000h, 000h
        jbe    SHORT loc_4_1C4A
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1C4A:
        push   0
        db  0A1h, 00Ch, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C57:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1C61:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_1C70:
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
loc_4_1C8E:
        test   eax, eax
        je     SHORT loc_4_1C9F
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1C9F:
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_0450
        test   eax, eax
        je     SHORT loc_4_1CD1
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   225h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CC4:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1CD1:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_1D10
        test   eax, eax
        jne    SHORT loc_4_1CEA
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1CEA:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   225h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CFA:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_21C0
        push   esi
        call   sub_4_20B0
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_4_2018
        lea    edi, [ebp - 48h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 48h], 0
        mov    eax, dword ptr [esi + 209h]
        cmp    eax, -1
        je     SHORT loc_4_1D56
        mov    dword ptr [ebp - 48h], eax
loc_4_1D56:
        mov    edi, 2
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
        mov    dword ptr [ebp - 2ah], 0ffh
        push   esi
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1DA1:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_4_2012
        mov    eax, dword ptr [esi + 209h]
        cmp    eax, -1
        je     SHORT loc_4_1E1E
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
        mov    dword ptr [ebp - 1dh], edi
        mov    dword ptr [ebp - 15h], eax
        mov    eax, dword ptr [esi + 201h]
        lea    ecx, [ebp - 4]
        mov    byte ptr [ebp - 20h], 1
        mov    dword ptr [ebp - 19h], 3
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1DF7:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_1E1E
        mov    eax, dword ptr [esi + 209h]
        mov    ecx, dword ptr [esi + 201h]
        push   eax
        push   1
        push   0aa000001h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E16:
        add    esp, 10h
        jmp    NEAR PTR loc_4_2006
loc_4_1E1E:
        mov    ecx, dword ptr [esi + 205h]
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [esi + 209h]
        push   edx
        mov    ebx, dword ptr [esi + 21dh]
        push   ebx
        lea    eax, [ecx + 0ffh]
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
        call   sub_4_2490
        test   eax, eax
        jge    SHORT loc_4_1E87
        mov    eax, dword ptr [esi + 205h]
        mov    ecx, dword ptr [esi + 211h]
        push   eax
        mov    edx, dword ptr [esi + 201h]
        push   ecx
        push   2
        push   0aa000004h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E78:
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 14h
        jmp    NEAR PTR loc_4_1FF4
loc_4_1E87:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 20h], eax
loc_4_1E98:
        lea    eax, [esi + ecx]
        mov    edx, dword ptr [esi + 73h]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 6dh], dl
        mov    byte ptr [eax + 67h], dl
        jb     SHORT loc_4_1E98
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
loc_4_1ED5:
        test   al, al
        jne    SHORT loc_4_1EE5
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_4_1FF4
loc_4_1EE5:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_29A0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2A50
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_33E0
        mov    edi, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F14:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_1F27
        mov    dword ptr [ebp - 4], 0c000009ah
        jmp    NEAR PTR loc_4_1FEC
loc_4_1F27:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3400
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
loc_4_1F59:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_37F0
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 1bdh], eax
        mov    dword ptr [esi + 1b9h], 0
        call   sub_4_2A80
        test   eax, eax
        jge    SHORT loc_4_1F89
        or     dword ptr [esi + 47h], 2
        jmp    SHORT loc_4_1FEC
loc_4_1F89:
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
loc_4_1FC1:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_2024
        mov    eax, dword ptr [esi + 211h]
        mov    ecx, dword ptr [esi + 201h]
        push   eax
        push   1
        push   0aa000005h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FE1:
        add    esp, 10h
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28B0
loc_4_1FEC:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FF4:
        cmp    dword ptr [esi + 209h], -1
        je     SHORT loc_4_2006
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2006:
        mov    eax, dword ptr [esi + 201h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2012:
        push   esi
        call   sub_4_2180
loc_4_2018:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_2024:
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
loc_4_2090:
        mov    ecx, dword ptr [esi + 1c1h]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_20B0:
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
loc_4_20D1:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_20E6
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_20E6:
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
loc_4_2104:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_2155
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
loc_4_2138:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_2173
        push   0
        mov    eax, dword ptr [esi + 20dh]
        mov    ecx, dword ptr [ebx]
        lea    eax, [eax + eax*2 + 3]
        add    eax, eax
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2155:
        push   3
        db  0A1h, 04Eh, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2165:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2173:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_2180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2190:
        push   3
        db  0A1h, 04Eh, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21A0:
        push   0
        mov    ecx, dword ptr [esi + 20dh]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 73h]
        add    eax, eax
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21B8:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_21C0:
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
loc_4_2230:
        push   ebp
        db  0A1h, 008h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2245:
        push   0
        db  08Bh, 00Dh, 00Ch, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2253:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2260:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   4c4b40h
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 47h], 8
        lea    edi, [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2279:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2285:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_228B:
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_22A0
        mov    eax, dword ptr [esi + 221h]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_22CE
loc_4_22A0:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 221h], esi
        je     SHORT loc_4_22BC
loc_4_22AE:
        mov    ecx, dword ptr [ecx + 221h]
        cmp    dword ptr [ecx + 221h], esi
        jne    SHORT loc_4_22AE
loc_4_22BC:
        mov    eax, dword ptr [ecx + 221h]
        mov    edx, dword ptr [eax + 221h]
        mov    dword ptr [ecx + 221h], edx
loc_4_22CE:
        cmp    dword ptr [esi + 209h], -1
        je     SHORT loc_4_22E0
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22E0:
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22E9:
        mov    eax, dword ptr [esi + 201h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22F5:
        push   esi
        call   sub_4_2180
        push   0
        push   225h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2308:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_2310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_27D0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_2330:
        mov    eax, 110h
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2340:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 4]
        add    eax, edi
        push   eax
        push   ecx
        call   sub_4_0290
        push   edi
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0270
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  090h
sub_4_2370:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bx, word ptr [esi + 2]
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        jne    SHORT loc_4_238F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_238F:
        mov    di, word ptr [ebp + 10h]
        xor    ecx, ecx
        and    di, 1
        mov    cl, byte ptr [esi + 0bh]
        push   edi
        mov    edx, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0170
        test   eax, eax
        je     SHORT loc_4_23BC
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_23BC:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ebx
        push   ecx
        xor    ebx, ebx
        call   sub_4_0290
loc_4_23CC:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0bh]
        push   eax
        push   ecx
        call   sub_4_0200
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        je     SHORT loc_4_23F0
        inc    ebx
        cmp    ebx, 61a8h
        jb     SHORT loc_4_23CC
loc_4_23F0:
        cmp    ebx, 61a8h
        jae    SHORT loc_4_2436
        test   di, di
        jne    SHORT loc_4_241E
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   61h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
loc_4_241E:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0bh]
        push   eax
        push   ecx
        call   sub_4_0220
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_2436:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0bh]
        push   eax
        push   ecx
        call   sub_4_0220
        push   esi
        call   sub_4_27D0
        push   esi
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 11h]
        push   ecx
        push   esi
        call   sub_4_2A80
        test   eax, eax
        je     SHORT loc_4_2475
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
loc_4_2475:
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2490:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    word ptr [ebx], 0
        mov    edi, dword ptr [ebp + 18h]
        mov    word ptr [ebx + 2], di
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, edi
        mov    dword ptr [ebx + 17h], esi
        and    eax, 0ffff0fffh
        cmp    eax, 0c00h
        jne    NEAR PTR loc_4_27B3
        mov    eax, dword ptr [ebp + 1ch]
        sub    eax, edi
        cmp    eax, 0ffh
        jb     NEAR PTR loc_4_27B3
        mov    eax, dword ptr [ebp + 2ch]
        cmp    dword ptr [ebp + 14h], 0dh
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_24F3
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_24F3:
        mov    ecx, dword ptr [ebp + 14h]
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_250D
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_250D:
        cmp    dword ptr [ebp + 28h], 7
        mov    cl, byte ptr [ebp + 14h]
        mov    byte ptr [ebx + 0ch], cl
        ja     NEAR PTR loc_4_279C
        mov    eax, dword ptr [ebp + 28h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 4], al
        test   al, al
        je     NEAR PTR loc_4_279C
        push   edi
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_4_0260
        and    al, 81h
        mov    ecx, dword ptr [ebp + 14h]
        db  00Ah, 081h, 000h, 000h, 000h, 000h
        or     al, byte ptr [ebp - 4]
        or     al, 1
        mov    byte ptr [ebx + 9], al
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   0
        lea    ecx, [edi + 80h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        cmp    al, 22h
        jne    NEAR PTR loc_4_26B2
        lea    eax, [edi + 81h]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 0f0h
        jne    NEAR PTR loc_4_26B2
        lea    eax, [edi + 82h]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 18h
        jne    NEAR PTR loc_4_26B2
        lea    eax, [edi + 83h]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    byte ptr [ebp - 4], al
        and    al, 0f0h
        cmp    al, 40h
        jne    NEAR PTR loc_4_26B2
        push   0
        lea    eax, [edi + 20h]
        push   eax
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   sub_4_0280
        lea    ecx, [edi + 80h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        lea    ecx, [edi + 80h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        lea    ecx, [edi + 80h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        lea    ecx, [edi + 80h]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    cl, byte ptr [ebx + 9]
        lea    edx, [edi + 20h]
        push   ecx
        mov    eax, dword ptr [ebx + 4]
        push   edx
        push   eax
        call   sub_4_0280
        mov    byte ptr [ebx + 10fh], 0
        mov    al, byte ptr [ebx + 9]
        lea    ecx, [edi + 20h]
        and    al, 0f1h
        mov    edx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0280
        push   1fh
        push   0beh
        push   edi
        push   ebx
        call   sub_4_2340
        mov    dword ptr [ebp - 0ch], 0
        mov    word ptr [ebp - 6], 0
        mov    dword ptr [ebp - 10h], esi
loc_4_2654:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebx + 4]
        add    eax, edi
        add    eax, 85h
        push   eax
        push   ecx
        call   sub_4_0260
        mov    ecx, dword ptr [ebp - 10h]
        inc    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        and    ax, 0ffh
        add    word ptr [ebp - 6], ax
        cmp    dword ptr [ebp - 0ch], 6
        jb     SHORT loc_4_2654
        lea    eax, [edi + 8bh]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        add    edi, 8ch
        push   ecx
        call   sub_4_0260
        and    ax, 0ffh
        push   edi
        add    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   sub_4_0260
        and    byte ptr [ebp - 4], 0fh
        mov    byte ptr [ebx + 8], al
        jmp    SHORT loc_4_26B6
loc_4_26B2:
        mov    byte ptr [ebp - 4], 0ffh
loc_4_26B6:
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_4_0010
        cmp    byte ptr [ebp - 4], 0ffh
        jne    SHORT loc_4_26DC
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        pop    esi
        pop    ebx
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_26DC:
        cmp    byte ptr [ebp - 6], 0ffh
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebx + 0ah], al
        je     SHORT loc_4_270D
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 3
        repe cmpsb
        je     SHORT loc_4_270D
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        pop    esi
        pop    ebx
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_270D:
        mov    al, byte ptr [ebp + 28h]
        push   0c1ch
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    byte ptr [ebx + 0bh], al
        call   sub_4_0020
        test   eax, eax
        jne    SHORT loc_4_273E
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        pop    esi
        pop    ebx
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_273E:
        add    eax, 3
        and    eax, 0fffffffch
        mov    edx, eax
        mov    dword ptr [ebx + 0dh], eax
        lea    ecx, [eax + 5f4h]
        and    edx, 0fffff000h
        and    ecx, 0fffff000h
        cmp    edx, ecx
        je     SHORT loc_4_2762
        mov    dword ptr [ebx + 0dh], ecx
loc_4_2762:
        mov    dword ptr [ebx + 0ebh], 100h
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        mov    dword ptr [ebx + 0e3h], 2800h
        mov    dword ptr [ebx + 0e7h], 0d800h
        pop    esi
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [ebx], 2
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_279C:
        mov    eax, dword ptr [ebp + 2ch]
        pop    edi
        pop    esi
        pop    ebx
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_27B3:
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
sub_4_27D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 2]
        mov    dword ptr [esi + 37h], 0
        mov    word ptr [esi], 2
        cmp    byte ptr [esi + 10fh], 0
        je     SHORT loc_4_2801
        mov    al, byte ptr [esi + 9]
        and    eax, 0feh
        jmp    SHORT loc_4_280C
loc_4_2801:
        lea    eax, [esi + 9]
        mov    al, byte ptr [esi + 9]
        and    eax, 0f0h
loc_4_280C:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        lea    eax, [edi + 20h]
        lea    ebx, [edi + 80h]
        push   eax
        push   ecx
        call   sub_4_0280
        push   ebx
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        cmp    byte ptr [esi + 10fh], 0
        mov    al, byte ptr [esi + 9]
        jne    SHORT loc_4_2856
        and    al, 0f1h
loc_4_2856:
        or     al, 1
        mov    ecx, dword ptr [esi + 4]
        and    eax, 0ffh
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        push   ecx
        call   sub_4_0280
        push   edi
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0270
        push   21h
        push   0a0h
        push   edi
        push   esi
        call   sub_4_2340
        lea    ecx, [edi + 0a0h]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0270
        push   esi
        call   sub_4_3810
        push   1fh
        push   0beh
        push   edi
        push   esi
        call   sub_4_2340
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_28B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 2]
        mov    dword ptr [esi + 37h], 0
        lea    eax, [edi + 0a0h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    al, 4
        push   21h
        push   0a0h
        mov    byte ptr [ebp - 1], al
        push   edi
        mov    ebx, 63fh
        push   esi
        call   sub_4_2340
        mov    word ptr [esi], 4
        push   esi
        call   sub_4_3810
loc_4_28FC:
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0270
        dec    ebx
        jne    SHORT loc_4_28FC
        xor    ebx, ebx
        push   ebx
        push   0b4h
        push   edi
        push   esi
        call   sub_4_2340
        push   ebx
        push   0b6h
        push   edi
        push   esi
        call   sub_4_2340
        push   2
        push   0bah
        push   edi
        push   esi
        call   sub_4_2340
        lea    eax, [edi + 0aeh]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        test   al, 0ah
        je     SHORT loc_4_2985
        push   0ah
        push   0aeh
        push   edi
        push   esi
        call   sub_4_2340
        xor    eax, eax
        mov    dword ptr [esi + 33h], ebx
        mov    al, byte ptr [esi + 40h]
        mov    cl, byte ptr [esi + eax*4 + 44h]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 40h], cl
        mov    byte ptr [eax + 44h], bl
        mov    word ptr [eax + 45h], bx
        mov    al, byte ptr [esi + 3fh]
        test   al, al
        je     SHORT loc_4_297A
        dec    al
        mov    byte ptr [esi + 3fh], al
loc_4_297A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2985:
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_29A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    word ptr [esi + 15h], di
        mov    ebx, dword ptr [ebp + 0ch]
        lea    eax, [esi + 1bh]
        cmp    ebx, edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], edi
        je     SHORT loc_4_29D3
loc_4_29BE:
        mov    eax, dword ptr [esi + 17h]
        add    eax, edi
        add    edi, 6
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_29E0
        dec    ebx
        jne    SHORT loc_4_29BE
loc_4_29D3:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_29E0:
        push   ebp
        mov    ecx, 0ffffffffh
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
loc_4_29F8:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    eax, 8
loc_4_2A00:
        mov    edi, ecx
        mov    bl, dl
        shr    dl, 1
        and    edi, 80000000h
        shr    edi, 1fh
        and    ebx, 1
        xor    edi, ebx
        add    ecx, ecx
        test   edi, edi
        je     SHORT loc_4_2A23
        xor    ecx, 4c11db6h
        or     ecx, edi
loc_4_2A23:
        dec    eax
        jne    SHORT loc_4_2A00
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_4_29F8
        shr    ecx, 1ah
        mov    bl, 1
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 8]
        shr    edx, 3
        and    cl, 7
        shl    bl, cl
        pop    edi
        or     byte ptr [edx + eax + 1bh], bl
        inc    word ptr [eax + 15h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_2A50:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [edi + 33h], eax
        mov    dword ptr [edi + 37h], eax
        mov    byte ptr [edi + 3fh], al
        mov    byte ptr [edi + 40h], al
        mov    byte ptr [edi + 41h], al
        mov    byte ptr [edi + 42h], al
        add    edi, 43h
        mov    ecx, 28h
        rep stosd
        pop    edi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   al, 2
        mov    dword ptr [esi + 37h], ebx
        mov    dword ptr [esi + 11h], eax
        je     SHORT loc_4_2A9D
        mov    bl, 4
loc_4_2A9D:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 4
        mov    dword ptr [ebp - 8], eax
        jne    SHORT loc_4_2AAE
        test   byte ptr [ebp + 0ch], 8
        je     SHORT loc_4_2AB1
loc_4_2AAE:
        or     bl, 8
loc_4_2AB1:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 10h
        mov    dword ptr [ebp - 0ch], eax
        je     SHORT loc_4_2ABF
        or     bl, 1ch
loc_4_2ABF:
        test   byte ptr [ebp + 0ch], 20h
        je     SHORT loc_4_2AC8
        or     bl, 3
loc_4_2AC8:
        xor    edi, edi
        push   21h
        mov    di, word ptr [esi + 2]
        push   0a0h
        push   edi
        push   esi
        call   sub_4_2340
        push   esi
        call   sub_4_3810
        push   49h
        push   0bch
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0b4h
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0b6h
        push   edi
        push   esi
        call   sub_4_2340
        mov    al, bl
        and    eax, 0ffff00ffh
        push   eax
        push   0b8h
        push   edi
        push   esi
        call   sub_4_2340
        push   2
        push   0bah
        push   edi
        push   esi
        call   sub_4_2340
        push   28h
        push   0a2h
        push   edi
        push   esi
        call   sub_4_2340
        push   28h
        push   0a6h
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0a4h
        push   edi
        push   esi
        call   sub_4_2340
        push   0ffh
        push   0aeh
        push   edi
        push   esi
        call   sub_4_2340
        push   1fh
        push   0beh
        push   edi
        push   esi
        call   sub_4_2340
        push   61h
        push   0a0h
        push   edi
        xor    ebx, ebx
        push   esi
        call   sub_4_2340
        mov    eax, dword ptr [esi + 17h]
        mov    dword ptr [ebp - 4], eax
loc_4_2B8D:
        lea    ecx, [ebx + 0a2h]
        mov    eax, dword ptr [ebp - 4]
        add    ebx, 2
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_4_2340
        inc    dword ptr [ebp - 4]
        cmp    ebx, 0ch
        jl     SHORT loc_4_2B8D
        test   byte ptr [ebp + 0ch], 8
        jne    SHORT loc_4_2BF1
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_2BF1
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_2C0E
        lea    eax, [esi + 1bh]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], eax
loc_4_2BCB:
        lea    ecx, [ebx + 0b0h]
        mov    eax, dword ptr [ebp - 4]
        add    ebx, 2
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_4_2340
        inc    dword ptr [ebp - 4]
        cmp    ebx, 10h
        jl     SHORT loc_4_2BCB
        jmp    SHORT loc_4_2C0E
loc_4_2BF1:
        xor    ebx, ebx
loc_4_2BF3:
        lea    eax, [ebx + 0b0h]
        push   0ffffh
        push   eax
        add    ebx, 2
        push   edi
        push   esi
        call   sub_4_2340
        cmp    ebx, 10h
        jl     SHORT loc_4_2BF3
loc_4_2C0E:
        push   29h
        push   0aeh
        push   edi
        push   esi
        call   sub_4_2340
        mov    dword ptr [esi + 23h], 29h
        push   2
        push   0a0h
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0bah
        push   edi
        push   esi
        add    edi, 0a0h
        call   sub_4_2340
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0270
        and    al, 0c2h
        cmp    al, 2
        je     SHORT loc_4_2C63
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2C63:
        mov    word ptr [esi], 8
        cmp    byte ptr [esi + 3fh], 0
        je     SHORT loc_4_2C74
        push   esi
        call   sub_4_2E40
loc_4_2C74:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_4_2C80:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [edx], 8
        push   esi
        push   edi
        jne    SHORT loc_4_2C9B
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_2C9B:
        mov    esi, dword ptr [ebp + 14h]
        xor    eax, eax
        add    esi, 0ffh
        mov    al, byte ptr [edx + 41h]
        shr    esi, 8
        xor    ebx, ebx
        cmp    byte ptr [edx + 3fh], 0
        mov    bl, byte ptr [edx + eax*4 + 44h]
        je     SHORT loc_4_2D04
        xor    eax, eax
        inc    dword ptr [edx + 107h]
        mov    al, byte ptr [edx + 40h]
        cmp    eax, ebx
        jae    SHORT loc_4_2CEC
        mov    ecx, 28h
        sub    ecx, ebx
        cmp    ecx, esi
        jae    SHORT loc_4_2D0C
        cmp    esi, eax
        jbe    SHORT loc_4_2CE8
        mov    eax, 1
        pop    edi
        pop    esi
        inc    dword ptr [edx + 103h]
        pop    ebx
        pop    ebp
        ret    10h
loc_4_2CE8:
        xor    ebx, ebx
        jmp    SHORT loc_4_2D0C
loc_4_2CEC:
        sub    eax, ebx
        cmp    eax, esi
        jae    SHORT loc_4_2D0C
        mov    eax, 1
        pop    edi
        pop    esi
        inc    dword ptr [edx + 103h]
        pop    ebx
        pop    ebp
        ret    10h
loc_4_2D04:
        xor    ebx, ebx
        mov    byte ptr [edx + 41h], bl
        mov    byte ptr [edx + 40h], bl
loc_4_2D0C:
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, eax
        mov    byte ptr [edx + 42h], bl
        shr    ecx, 2
        mov    edi, dword ptr [edx + 0dh]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        mov    dword ptr [edx + 3bh], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2D40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [eax + 0dh]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, dword ptr [eax + 3bh]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        pop    edi
        add    dword ptr [eax + 3bh], edx
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_4_2D70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 3bh]
        cmp    ebx, 3ch
        jae    SHORT loc_4_2DA2
        mov    edx, 3ch
        mov    edi, dword ptr [esi + 0dh]
        sub    edx, ebx
        add    edi, ebx
        xor    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        mov    ebx, 3ch
        and    ecx, 3
        rep stosb
loc_4_2DA2:
        lea    eax, [ebx + 3]
        mov    ecx, dword ptr [esi + 0dh]
        and    eax, 0fffffffch
        push   eax
        mov    al, byte ptr [esi + 42h]
        and    eax, 0ffff00ffh
        shl    ax, 8
        push   eax
        push   ecx
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_2DD0
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2DD0:
        mov    cl, byte ptr [esi + 42h]
        lea    eax, [ebx + 0ffh]
        shr    eax, 8
        add    al, cl
        cmp    al, 28h
        jb     SHORT loc_4_2DE4
        xor    al, al
loc_4_2DE4:
        and    ecx, 0ffh
        mov    byte ptr [esi + ecx*4 + 44h], al
        xor    eax, eax
        mov    al, byte ptr [esi + 42h]
        mov    word ptr [esi + eax*4 + 45h], bx
        mov    dword ptr [esi + 3bh], 0
        mov    al, byte ptr [esi + 3fh]
        test   al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [esi + 3fh], cl
        jne    SHORT loc_4_2E1A
        mov    al, byte ptr [esi + 42h]
        push   esi
        mov    byte ptr [esi + 40h], al
        call   sub_4_2E40
        jmp    SHORT loc_4_2E26
loc_4_2E1A:
        mov    cl, byte ptr [esi + 42h]
        xor    eax, eax
        mov    al, byte ptr [esi + 41h]
        mov    byte ptr [esi + eax*4 + 44h], cl
loc_4_2E26:
        mov    al, byte ptr [esi + 42h]
        pop    edi
        mov    byte ptr [esi + 41h], al
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 2]
        mov    al, byte ptr [esi + 40h]
        and    eax, 0ffff00ffh
        push   eax
        push   0a8h
        push   edi
        push   esi
        call   sub_4_2340
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [esi + 40h]
        mov    cx, word ptr [esi + eax*4 + 45h]
        mov    dl, cl
        mov    dword ptr [ebp - 4], ecx
        and    edx, 0ffff00ffh
        push   edx
        push   0aah
        push   edi
        push   esi
        call   sub_4_2340
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0ffff00ffh
        push   ecx
        push   0ach
        push   edi
        push   esi
        call   sub_4_2340
        push   26h
        push   0a0h
        push   edi
        push   esi
        call   sub_4_2340
        xor    eax, eax
        pop    edi
        mov    dword ptr [esi + 37h], 2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_2EC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   62h
        and    edi, 0ffffh
        push   0a0h
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_2340
        lea    eax, [edi + 0aeh]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    bl, al
        push   22h
        push   0a0h
        push   edi
        push   esi
        call   sub_4_2340
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2F10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        add    esi, 7
        and    esi, 0fffffffch
        cmp    esi, 0f8h
        ja     SHORT loc_4_2F2F
        mov    esi, 0f8h
loc_4_2F2F:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, 10000h
        mov    dword ptr [ebx + 2fh], esi
        mov    eax, dword ptr [ebx + 0dh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 23h]
        shl    ax, 8
        mov    ecx, eax
        and    ecx, 0ffffh
        sub    edi, ecx
        cmp    edi, esi
        jae    SHORT loc_4_2F7E
        push   edi
        or     al, 1
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   ebx
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_2F75
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_2F75:
        mov    ax, 2800h
        sub    esi, edi
        add    dword ptr [ebp - 4], edi
loc_4_2F7E:
        push   esi
        or     al, 1
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   ebx
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_2F9E
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_2F9E:
        mov    eax, dword ptr [ebx + 0dh]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 27h], ecx
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebx + 0dh]
        add    esi, 4
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    cl, byte ptr [ebx + 27h]
        test   cl, 48h
        je     NEAR PTR loc_4_306C
        xor    esi, esi
        push   0ffh
        push   0b4h
        inc    dword ptr [ebx + 0fbh]
        mov    si, word ptr [ebx + 2]
        push   esi
        push   ebx
        call   sub_4_2340
        push   0ffh
        push   0b6h
        push   esi
        push   ebx
        call   sub_4_2340
        push   28h
        push   0a6h
        push   esi
        push   ebx
        call   sub_4_2340
        push   61h
        push   0a0h
        push   esi
        push   ebx
        call   sub_4_2340
        push   29h
        push   0aeh
        push   esi
        push   ebx
        call   sub_4_2340
        mov    dword ptr [ebx + 23h], 29h
        push   22h
        push   0a0h
        push   esi
        push   ebx
        call   sub_4_2340
        push   5
        push   0aeh
        push   esi
        push   ebx
        add    esi, 0a0h
        call   sub_4_2340
        push   esi
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_4_0270
        and    al, 0c2h
        pop    edi
        sub    al, 2
        pop    esi
        pop    ebx
        cmp    al, 1
        sbb    eax, eax
        mov    esp, ebp
        and    eax, 0fffffffeh
        pop    ebp
        dec    eax
        ret    10h
loc_4_306C:
        xor    eax, eax
        mov    al, byte ptr [ebx + 28h]
        sub    eax, dword ptr [ebx + 23h]
        lea    edx, [eax - 1]
        cmp    edx, 0d8h
        jbe    SHORT loc_4_3085
        lea    edx, [eax + 0d7h]
loc_4_3085:
        mov    al, byte ptr [ebx + 29h]
        cmp    al, 0fch
        jbe    SHORT loc_4_308D
        dec    edx
loc_4_308D:
        shl    edx, 8
        and    eax, 0ffh
        add    eax, edx
        mov    dword ptr [ebx + 2bh], eax
        sub    eax, 4
        test   cl, 1
        jne    SHORT loc_4_30F6
        mov    dl, cl
        and    dl, 4
        jne    SHORT loc_4_30B9
        test   cl, 2
        jne    SHORT loc_4_30B9
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 201h
        jmp    SHORT loc_4_30E1
loc_4_30B9:
        and    cl, 6
        cmp    cl, 6
        jne    SHORT loc_4_30CC
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 206h
        jmp    SHORT loc_4_30E1
loc_4_30CC:
        mov    ecx, dword ptr [ebp + 14h]
        test   dl, dl
        je     SHORT loc_4_30DB
        mov    dword ptr [ecx], 204h
        jmp    SHORT loc_4_30E1
loc_4_30DB:
        mov    dword ptr [ecx], 202h
loc_4_30E1:
        test   byte ptr [ebx + 11h], 20h
        jne    NEAR PTR loc_4_324A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_30F6:
        cmp    eax, 3ch
        jb     NEAR PTR loc_4_3239
        cmp    eax, 5eah
        ja     NEAR PTR loc_4_3239
        mov    edx, dword ptr [ebx + 17h]
        mov    ecx, dword ptr [ebp + 0ch]
        test   byte ptr [ecx], 1
        jne    SHORT loc_4_3182
        mov    ecx, 6
        mov    esi, edx
        mov    edi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_315F
        mov    ecx, dword ptr [ebx + 11h]
        test   cl, 1
        je     SHORT loc_4_313D
        mov    ecx, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_313D:
        test   cl, 10h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 100h
        je     SHORT loc_4_3154
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3154:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_315F:
        mov    ecx, dword ptr [ebp + 14h]
        test   byte ptr [ebx + 11h], 10h
        mov    dword ptr [ecx], 104h
        je     SHORT loc_4_3177
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3177:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3182:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 6
        mov    edi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_31CF
        mov    ecx, dword ptr [ebx + 11h]
        test   cl, 2
        je     SHORT loc_4_31AD
        mov    ecx, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 1
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_31AD:
        test   cl, 10h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 101h
        je     SHORT loc_4_31C4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_31C4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_31CF:
        mov    dword ptr [ebp - 4], 0
        cmp    word ptr [ebx + 15h], 0
        jbe    SHORT loc_4_3202
loc_4_31DD:
        add    edx, 6
        mov    ecx, 6
        mov    esi, edx
        mov    edi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_31F4
        test   byte ptr [ebx + 11h], 4
        jne    SHORT loc_4_321E
loc_4_31F4:
        xor    ecx, ecx
        inc    dword ptr [ebp - 4]
        mov    cx, word ptr [ebx + 15h]
        cmp    ecx, dword ptr [ebp - 4]
        ja     SHORT loc_4_31DD
loc_4_3202:
        mov    ecx, dword ptr [ebx + 11h]
        test   cl, 18h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 102h
        jne    SHORT loc_4_3230
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_321E:
        mov    ecx, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 2
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3230:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3239:
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 208h
        test   byte ptr [ebx + 11h], 20h
        jne    SHORT loc_4_324A
        xor    eax, eax
loc_4_324A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3260:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [ebp + 14h]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    eax, ebx
        mov    ecx, dword ptr [esi + 2fh]
        add    eax, 4
        cmp    eax, ecx
        ja     SHORT loc_4_32A4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 0dh]
        mov    edi, dword ptr [ebp + 10h]
        lea    esi, [eax + ecx + 4]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_32A4:
        cmp    byte ptr [ebp + 18h], 0
        jne    NEAR PTR loc_4_3370
        mov    eax, dword ptr [esi + 0dh]
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 23h]
        shl    eax, 8
        lea    edx, [eax + ecx]
        cmp    edx, 10000h
        jb     SHORT loc_4_32CD
        sub    edx, 0d800h
loc_4_32CD:
        mov    eax, 10000h
        mov    edi, dword ptr [esi + 2bh]
        sub    eax, edx
        cmp    edi, 5f2h
        mov    dword ptr [ebp - 4], eax
        jbe    SHORT loc_4_32E7
        mov    edi, 5f2h
loc_4_32E7:
        sub    edi, ecx
        add    edi, 3
        and    edi, 0fffffffch
        add    ecx, edi
        cmp    edi, dword ptr [ebp - 4]
        mov    dword ptr [esi + 2fh], ecx
        jbe    SHORT loc_4_3329
        mov    eax, dword ptr [ebp - 4]
        or     dl, 1
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_331D
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_331D:
        mov    dx, 2800h
        mov    eax, dword ptr [ebp - 4]
        sub    edi, eax
        add    dword ptr [ebp - 8], eax
loc_4_3329:
        push   edi
        or     dl, 1
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_334A
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_334A:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 0dh]
        mov    edi, dword ptr [ebp + 10h]
        lea    esi, [eax + ecx + 4]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_4_3365:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_3370:
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [esi + 23h]
        shl    ax, 8
        mov    ecx, dword ptr [ebp + 0ch]
        lea    ecx, [eax + ecx + 4]
        mov    eax, 10000h
        mov    edx, ecx
        and    edx, 0ffffh
        sub    eax, edx
        mov    dword ptr [ebp - 4], eax
        cmp    ebx, eax
        jbe    SHORT loc_4_33BF
        push   eax
        or     cl, 1
        push   ecx
        push   edi
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_33B5
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_33B5:
        mov    cx, 2800h
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
loc_4_33BF:
        push   ebx
        or     cl, 1
        push   ecx
        push   edi
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     SHORT loc_4_3365
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_4_33E0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 8
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0efh
        rep stosd
        mov    eax, 2ch
        pop    edi
        pop    ebp
        ret    4
sub_4_3400:
        push   ebp
        mov    ecx, 0bh
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0e3h
        mov    edi, dword ptr [ebp + 0ch]
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_3420:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     NEAR PTR loc_4_34C8
        mov    eax, dword ptr [esi + 37h]
        test   eax, eax
        jle    NEAR PTR loc_4_34C8
        dec    eax
        mov    dword ptr [esi + 37h], eax
        test   eax, eax
        jg     NEAR PTR loc_4_34C8
        push   esi
        call   sub_4_27D0
        xor    eax, eax
        inc    dword ptr [esi + 10bh]
        mov    dword ptr [esi + 33h], 0
        mov    al, byte ptr [esi + 40h]
        mov    cl, byte ptr [esi + eax*4 + 44h]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 40h], cl
        mov    byte ptr [eax + 44h], 0
        mov    word ptr [eax + 45h], 0
        mov    al, byte ptr [esi + 3fh]
        test   al, al
        je     SHORT loc_4_347F
        dec    al
        mov    byte ptr [esi + 3fh], al
loc_4_347F:
        mov    eax, dword ptr [esi + 11h]
        push   eax
        push   esi
        call   sub_4_2A80
        test   eax, eax
        je     SHORT loc_4_34A8
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebp
        ret    4
loc_4_34A8:
        push   10h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_1070
        cmp    eax, -1
        jge    SHORT loc_4_34C8
loc_4_34B8:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_1070
        cmp    eax, -1
        jl     SHORT loc_4_34B8
loc_4_34C8:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_34D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   esi
        inc    dword ptr [esi + 0f7h]
        mov    di, word ptr [esi + 2]
        call   sub_4_28B0
        push   2
        push   0bah
        push   edi
        push   esi
        call   sub_4_2340
        push   22h
        push   0a0h
        push   edi
        push   esi
        call   sub_4_2340
        lea    eax, [edi + 0a0h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    al, 0c2h
        cmp    al, 2
        je     SHORT loc_4_353A
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   0ch
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_4_353A:
        mov    word ptr [esi], 8
        xor    ebx, ebx
loc_4_3541:
        push   edi
        push   esi
        call   sub_4_2EC0
        and    eax, 0ffh
        cmp    eax, dword ptr [esi + 23h]
        je     NEAR PTR loc_4_35E7
        cmp    dword ptr [esi + 11h], 0
        mov    eax, 2
        je     SHORT loc_4_356A
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_4_3870
loc_4_356A:
        cmp    eax, 1
        jne    SHORT loc_4_3594
        mov    al, byte ptr [esi + 28h]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 23h], ecx
        cmp    al, 28h
        jae    SHORT loc_4_3581
        mov    al, 0ffh
loc_4_3581:
        and    ax, 0ffh
        push   eax
        push   0a6h
        push   edi
        push   esi
        call   sub_4_2340
        jmp    SHORT loc_4_35DA
loc_4_3594:
        cmp    eax, 2
        jne    SHORT loc_4_35D6
        push   edi
        push   esi
        call   sub_4_2EC0
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 23h], ecx
        cmp    al, 28h
        jae    SHORT loc_4_35AF
        mov    al, 0ffh
loc_4_35AF:
        and    ax, 0ffh
        push   eax
        push   0a6h
        push   edi
        push   esi
        call   sub_4_2340
        push   5
        push   0aeh
        push   edi
        push   esi
        call   sub_4_2340
        inc    dword ptr [esi + 0ffh]
        jmp    SHORT loc_4_35DA
loc_4_35D6:
        test   eax, eax
        jl     SHORT loc_4_3626
loc_4_35DA:
        inc    ebx
        cmp    ebx, 0d9h
        jb     NEAR PTR loc_4_3541
loc_4_35E7:
        push   10h
        push   0aeh
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0bah
        push   edi
        push   esi
        call   sub_4_2340
        cmp    byte ptr [esi + 3fh], 0
        je     SHORT loc_4_3616
        push   esi
        call   sub_4_2E40
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_4_3616:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_1070
        cmp    eax, -1
        jl     SHORT loc_4_3616
loc_4_3626:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_3630:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   0
        mov    bx, word ptr [edi + 2]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
loc_4_3652:
        lea    eax, [ebx + 0aeh]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        test   al, 10h
        jne    NEAR PTR loc_4_37C6
        push   5
        push   0aeh
        push   ebx
        push   edi
        call   sub_4_2340
        push   ebx
        push   edi
        call   sub_4_2EC0
        and    eax, 0ffh
        cmp    eax, dword ptr [edi + 23h]
        je     NEAR PTR loc_4_371B
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 1
        cmp    dword ptr [edi + 11h], 0
        mov    eax, 2
        je     SHORT loc_4_36A6
        push   esi
        call   sub_4_3870
loc_4_36A6:
        cmp    eax, 1
        jne    SHORT loc_4_36D0
        mov    al, byte ptr [edi + 28h]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [edi + 23h], ecx
        cmp    al, 28h
        jae    SHORT loc_4_36BD
        mov    al, 0ffh
loc_4_36BD:
        and    ax, 0ffh
        push   eax
        push   0a6h
        push   ebx
        push   edi
        call   sub_4_2340
        jmp    SHORT loc_4_371F
loc_4_36D0:
        cmp    eax, 2
        jne    SHORT loc_4_3712
        push   ebx
        push   edi
        call   sub_4_2EC0
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [edi + 23h], ecx
        cmp    al, 28h
        jae    SHORT loc_4_36EB
        mov    al, 0ffh
loc_4_36EB:
        and    ax, 0ffh
        push   eax
        push   0a6h
        push   ebx
        push   edi
        call   sub_4_2340
        push   5
        push   0aeh
        push   ebx
        push   edi
        call   sub_4_2340
        inc    dword ptr [edi + 0ffh]
        jmp    SHORT loc_4_371B
loc_4_3712:
        test   eax, eax
        jge    SHORT loc_4_371F
        jmp    NEAR PTR loc_4_37D1
loc_4_371B:
        mov    byte ptr [ebp - 1], 1
loc_4_371F:
        lea    eax, [ebx + 0aeh]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        test   al, 0ah
        je     NEAR PTR loc_4_37B9
        mov    dword ptr [edi + 37h], 0
        mov    dword ptr [edi + 33h], 0
        push   0ah
        push   0aeh
        push   ebx
        push   edi
        call   sub_4_2340
        lea    eax, [ebx + 0a8h]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    al, 0fh
        xor    edx, edx
        mov    dl, byte ptr [edi + 40h]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], 1
        mov    al, byte ptr [edi + edx*4 + 44h]
        lea    ecx, [edi + edx*4]
        mov    byte ptr [edi + 40h], al
        mov    byte ptr [ecx + 44h], 0
        mov    word ptr [ecx + 45h], 0
        mov    al, byte ptr [edi + 3fh]
        test   al, al
        je     SHORT loc_4_3799
        dec    al
        mov    byte ptr [edi + 3fh], al
        je     SHORT loc_4_3799
        push   edi
        call   sub_4_2E40
loc_4_3799:
        mov    al, byte ptr [ebp - 3]
        push   eax
        push   esi
        call   sub_4_1070
        cmp    eax, -1
        jge    SHORT loc_4_37B5
loc_4_37A8:
        push   0
        push   esi
        call   sub_4_1070
        cmp    eax, -1
        jl     SHORT loc_4_37A8
loc_4_37B5:
        xor    al, al
        jmp    SHORT loc_4_37BB
loc_4_37B9:
        mov    al, 1
loc_4_37BB:
        test   byte ptr [ebp - 1], al
        je     NEAR PTR loc_4_3652
        jmp    SHORT loc_4_37D1
loc_4_37C6:
        push   esi
        push   edi
        call   sub_4_34D0
        mov    byte ptr [ebp - 2], 1
loc_4_37D1:
        mov    eax, dword ptr [edi + 4]
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
sub_4_37F0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_3803
        mov    eax, 3fh
        pop    ebp
        ret    8
loc_4_3803:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 11h]
        ret    8
        db  08Dh, 049h, 000h
sub_4_3810:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 2]
        mov    byte ptr [eax + 10fh], 0
        mov    dl, byte ptr [eax + 9]
        add    ecx, 20h
        and    dl, 0f1h
        mov    eax, dword ptr [eax + 4]
        push   edx
        push   ecx
        push   eax
        call   sub_4_0280
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_3840:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 2]
        mov    byte ptr [eax + 10fh], 1
        mov    dl, byte ptr [eax + 9]
        add    ecx, 20h
        push   edx
        mov    eax, dword ptr [eax + 4]
        push   ecx
        push   eax
        call   sub_4_0280
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3870:
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
        jbe    SHORT loc_4_38AD
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_38AD
        push   0
        push   esi
        call   sub_4_1070
        cmp    eax, -1
        jne    SHORT loc_4_38AD
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38AD:
        lea    edi, [esi + 0bdh]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0fch
        push   edi
        push   ecx
        call   sub_4_2F10
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     SHORT loc_4_3913
        add    eax, 3
        cmp    eax, 3
        ja     SHORT loc_4_3913
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_38DB:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38E5:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38EF:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38FC:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_3C20
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3913:
        sub    dword ptr [ebp - 4], 0eh
        mov    ecx, dword ptr [esi + 7fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     SHORT loc_4_3923
        mov    ecx, eax
loc_4_3923:
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
loc_4_3957:
        cmp    dword ptr [ebp - 18h], 0
        sbb    eax, eax
        pop    edi
        inc    eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_3980:
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
        je     SHORT loc_4_39CB
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
loc_4_39C0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_4_39CB:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [ebp + 10h]
        lea    edi, [ecx + edx]
        cmp    edi, eax
        jbe    SHORT loc_4_39E2
        sub    eax, edx
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_4_39E5
loc_4_39E2:
        mov    dword ptr [ebp - 18h], ecx
loc_4_39E5:
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
loc_4_3A01:
        test   edi, edi
        je     SHORT loc_4_3A54
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A18:
        mov    eax, dword ptr [ebp - 10h]
        cmp    ebx, eax
        jb     SHORT loc_4_3A21
        mov    ebx, eax
loc_4_3A21:
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_4_3260
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     SHORT loc_4_3A54
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A4E:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_3A01
loc_4_3A54:
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
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_3A92
loc_4_3A7E:
        mov    ecx, dword ptr [eax + 24h]
        push   eax
        push   esi
        mov    dword ptr [esi + 38h], ecx
        call   sub_4_3AA0
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        jne    SHORT loc_4_3A7E
loc_4_3A92:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3AA0:
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
loc_4_3ABC:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 7fh]
        add    ebx, 0eh
        cmp    ebx, eax
        jb     SHORT loc_4_3ACE
        mov    ebx, eax
loc_4_3ACE:
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
loc_4_3AE8:
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
        call   sub_4_3B70
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
loc_4_3B37:
        push   0
        inc    dword ptr [esi + 83h]
        mov    byte ptr [esi + 41h], 1
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3B50:
        push   esi
        call   sub_4_16E0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3B70:
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
loc_4_3B96:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 14h]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_3C14
loc_4_3BA3:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax]
        lea    edx, [ecx + ebx]
        cmp    edx, dword ptr [ebp + 0ch]
        jae    SHORT loc_4_3BEE
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3BC0:
        mov    ecx, dword ptr [ebp + 18h]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3BCE:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3BE4:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_3BA3
loc_4_3BEE:
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_3C14
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
loc_4_3C0D:
        mov    ecx, dword ptr [ebp + 18h]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_3C14:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_4_3C20:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        test   eax, 208h
        jne    SHORT loc_4_3C6C
        test   eax, 206h
        je     SHORT loc_4_3C48
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        inc    dword ptr [eax + 9bh]
        ret    8
loc_4_3C48:
        test   eax, 204h
        je     SHORT loc_4_3C5C
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        ret    8
loc_4_3C5C:
        test   eax, 202h
        je     SHORT loc_4_3C6C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 9bh]
loc_4_3C6C:
        pop    ebp
        ret    8
_PTEXT ends
        end
