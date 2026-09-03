; epro_lcode_01.asm -- EPRO object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        xor    dx, dx
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 1a6h]
        sub    eax, esi
        cmp    eax, 0fffffe5ah
        je     SHORT loc_1_0034
loc_1_001B:
        mov    ax, word ptr [esi + 41h]
        cmp    dx, ax
        jae    SHORT loc_1_0027
        mov    dx, ax
loc_1_0027:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0fffffe5ah
        jne    SHORT loc_1_001B
loc_1_0034:
        mov    word ptr [ecx + 1f7h], dx
        pop    esi
        pop    ebp
        ret    4
sub_1_0040:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 192h], ecx
        mov    dword ptr [eax + 13bh], edx
        mov    byte ptr [eax + 158h], cl
        mov    dword ptr [eax + 15ah], ecx
        mov    byte ptr [eax + 13ah], dl
        mov    byte ptr [eax + 139h], dl
        mov    dword ptr [eax + 162h], ecx
        mov    dword ptr [eax + 166h], ecx
        mov    dword ptr [eax + 16eh], ecx
        mov    dword ptr [eax + 172h], ecx
        mov    dword ptr [eax + 176h], ecx
        mov    dword ptr [eax + 17ah], ecx
        mov    dword ptr [eax + 17eh], ecx
        mov    dword ptr [eax + 182h], ecx
        mov    dword ptr [eax + 186h], ecx
        mov    dword ptr [eax + 18ah], ecx
        mov    dword ptr [eax + 18eh], ecx
        mov    dword ptr [eax + 196h], ecx
        mov    word ptr [eax + 25h], cx
        mov    dword ptr [eax + 19ah], ecx
        pop    ebp
        mov    dword ptr [eax + 19eh], ecx
        mov    dword ptr [eax + 1a2h], ecx
        mov    byte ptr [eax + 0fh], cl
        mov    dword ptr [eax + 15eh], 12345678h
        ret    4
sub_1_00E0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 21h], cx
        mov    dword ptr [eax + 147h], ecx
        mov    word ptr [eax + 25h], cx
        pop    ebp
        mov    word ptr [eax + 29h], cx
        mov    dword ptr [eax + 14bh], ecx
        mov    dword ptr [eax + 14fh], ecx
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0110:
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0120:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dx, word ptr [esi]
        add    edx, 3
        in     al, dx
        and    al, 0fh
        cmp    al, 0fh
        je     NEAR PTR loc_1_01F4
        cmp    byte ptr [esi + 158h], 0
        mov    al, 0fh
        out    dx, al
        jne    NEAR PTR loc_1_01F4
        inc    dword ptr [esi + 13bh]
        mov    word ptr [esi + 27h], 3
loc_1_0157:
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        in     al, dx
        mov    bl, al
        and    bl, 7
        jne    SHORT loc_1_0175
        cmp    dword ptr [esi + 147h], 0
        jne    SHORT loc_1_0175
        cmp    word ptr [esi + 27h], 0
        jne    SHORT loc_1_01C7
loc_1_0175:
        mov    al, bl
        and    al, 3
        jne    SHORT loc_1_0182
        cmp    word ptr [esi + 27h], 0
        jne    SHORT loc_1_018D
loc_1_0182:
        out    dx, al
        push   esi
        call   sub_1_0200
        test   al, al
        je     SHORT loc_1_01F4
loc_1_018D:
        mov    al, bl
        and    al, 8
        je     SHORT loc_1_019A
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
loc_1_019A:
        mov    al, bl
        and    al, 4
        je     SHORT loc_1_01AD
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
        call   sub_1_1EC0
loc_1_01AD:
        cmp    dword ptr [esi + 147h], 0
        je     SHORT loc_1_0157
loc_1_01B6:
        push   esi
        call   sub_1_03F0
        cmp    dword ptr [esi + 147h], 0
        jne    SHORT loc_1_01B6
        jmp    SHORT loc_1_0157
loc_1_01C7:
        xor    edx, edx
        mov    al, 8
        dec    dword ptr [esi + 13bh]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
        cmp    byte ptr [esi + 138h], 0
        je     SHORT loc_1_01F4
        mov    byte ptr [esi + 138h], 0
        mov    eax, dword ptr [esi + 1f3h]
        push   eax
        call   sub_1_2640
loc_1_01F4:
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0200:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 27h], 3
loc_1_0212:
        dec    word ptr [esi + 27h]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        xor    eax, eax
        mov    ax, word ptr [esi + 23h]
        lea    edx, [ecx + 0ch]
        out    dx, ax
        lea    edx, [ecx + 0eh]
        in     ax, dx
        mov    di, ax
        in     ax, dx
        test   ah, 20h
        je     NEAR PTR loc_1_03B3
        test   di, 8
        je     NEAR PTR loc_1_03B3
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        inc    dword ptr [esi + 166h]
        in     ax, dx
        cmp    ax, 5eah
        mov    di, ax
        ja     NEAR PTR loc_1_0388
        mov    bx, word ptr [esi + 1f7h]
        cmp    di, bx
        ja     SHORT loc_1_026D
        mov    ebx, edi
loc_1_026D:
        cmp    byte ptr [esi + 0fh], 0
        je     SHORT loc_1_02DE
        lea    edx, [ecx + 3]
        add    ecx, 0ch
        in     al, dx
        mov    byte ptr [ebp - 4], al
        or     al, 10h
        out    dx, al
        mov    ax, bx
        shr    ax, 2
        mov    edx, eax
        lea    eax, [esi + 38h]
        and    edx, 0ffffh
        push   edx
        push   eax
        push   ecx
        call   sub_1_221E
        test   bl, 2
        je     SHORT loc_1_02B6
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0fffch
        add    edx, 0ch
        in     ax, dx
        mov    word ptr [ecx + esi + 38h], ax
loc_1_02B6:
        test   bl, 1
        je     SHORT loc_1_02D0
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0ffffh
        add    edx, 0ch
        in     al, dx
        mov    byte ptr [ecx + esi + 37h], al
loc_1_02D0:
        xor    edx, edx
        mov    al, byte ptr [ebp - 4]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
        jmp    SHORT loc_1_0318
loc_1_02DE:
        lea    eax, [esi + 38h]
        add    ecx, 0eh
        mov    ax, bx
        shr    ax, 1
        mov    edx, eax
        lea    eax, [esi + 38h]
        and    edx, 0ffffh
        push   edx
        push   eax
        push   ecx
        call   sub_1_2209
        test   bl, 1
        je     SHORT loc_1_0318
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0ffffh
        add    edx, 0eh
        in     al, dx
        mov    byte ptr [ecx + esi + 37h], al
loc_1_0318:
        mov    cx, word ptr [esi + 23h]
        add    cx, 8
        cmp    byte ptr [esi + 10h], 0
        mov    word ptr [esi + 25h], di
        je     SHORT loc_1_0338
        cmp    cx, 8000h
        jb     SHORT loc_1_0344
        sub    cx, 6000h
        jmp    SHORT loc_1_0344
loc_1_0338:
        cmp    cx, 3000h
        jae    SHORT loc_1_0344
        add    cx, 3000h
loc_1_0344:
        mov    eax, edi
        lea    edx, [esi + 46h]
        and    eax, 0ffffh
        and    ecx, 0ffffh
        sub    eax, 0eh
        push   eax
        mov    eax, ebx
        and    eax, 0ffffh
        lea    ebx, [esi + 38h]
        sub    eax, 0eh
        push   eax
        push   edx
        push   0eh
        mov    edx, dword ptr [esi + 1f3h]
        push   ebx
        push   ebx
        push   ecx
        push   edx
        call   sub_1_2460
        cmp    byte ptr [esi + 158h], 0
        jne    SHORT loc_1_03BE
        mov    byte ptr [esi + 138h], 1
loc_1_0388:
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [esi + 23h], ax
        sub    ax, 2
        cmp    word ptr [esi + 11h], ax
        jbe    SHORT loc_1_039E
        add    ax, word ptr [esi + 13h]
loc_1_039E:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 6
        out    dx, ax
        cmp    word ptr [esi + 27h], 0
        jne    NEAR PTR loc_1_0212
loc_1_03B3:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_03BE:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03D0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        add    eax, 1e2h
        push   7d0h
        push   eax
        call   sub_1_22FA
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_03F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 147h]
        mov    dword ptr [ebx + 14fh], edi
        lea    esi, [edi + 24h]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_041A
        mov    dword ptr [ebx + 14bh], 0
loc_1_041A:
        mov    eax, dword ptr [esi]
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        mov    dword ptr [ebx + 147h], eax
        push   0
        push   0
        push   edi
        call   sub_1_2348
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 10h]
        push   edx
        push   eax
        call   sub_1_236C
        xor    edx, edx
        mov    dx, word ptr [ebx + 1f7h]
        cmp    dword ptr [ebp - 8], edx
        jae    SHORT loc_1_0480
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        je     SHORT loc_1_0480
        cmp    eax, edx
        mov    ecx, eax
        jb     SHORT loc_1_0463
        mov    ecx, edx
loc_1_0463:
        lea    eax, [ebp - 8]
        lea    edx, [ebx + 38h]
        push   eax
        and    ecx, 0ffffh
        push   edx
        push   ecx
        push   0
        push   edi
        call   sub_1_04D0
        lea    ecx, [ebx + 38h]
        mov    dword ptr [ebp - 4], ecx
loc_1_0480:
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + 138h], 1
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0eh
        mov    ecx, dword ptr [ebx + 1f3h]
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   0
        push   ecx
        call   sub_1_2460
        push   0
        mov    ecx, dword ptr [esi + 10h]
        push   edi
        push   ecx
        call   sub_1_2262
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_04D0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_0599
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_2348
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_0599
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_236C
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_0594
        mov    edi, dword ptr [ebp + 0ch]
loc_1_051F:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_054B
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_2390
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_0594
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_236C
        jmp    SHORT loc_1_058F
loc_1_054B:
        test   edi, edi
        je     SHORT loc_1_0568
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_0560
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_058F
loc_1_0560:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_0568:
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_1_0576
        mov    ebx, eax
loc_1_0576:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        add    esi, ebx
        call   sub_1_2404
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_058F:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_1_051F
loc_1_0594:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_0599:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    eax, dword ptr [ebp + 1ch]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [eax], ebx
        cmp    dword ptr [ebp + 10h], ebx
        je     NEAR PTR loc_1_07A7
        push   ebx
        lea    eax, [ebp - 14h]
        push   eax
        lea    ecx, [ebp - 18h]
        push   ecx
        mov    edx, dword ptr [ebp + 14h]
        push   ebx
        push   edx
        call   sub_1_2348
        cmp    dword ptr [ebp - 18h], ebx
        je     NEAR PTR loc_1_07A7
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_236C
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 25h]
        mov    ecx, dword ptr [ebp + 18h]
        sub    eax, ecx
        cmp    dword ptr [ebp + 10h], eax
        jbe    SHORT loc_1_060F
        mov    dword ptr [ebp + 10h], eax
loc_1_060F:
        test   cl, 1
        je     SHORT loc_1_0617
        dec    ecx
        mov    bl, 1
loc_1_0617:
        mov    ax, word ptr [ebp + 0ch]
        add    ax, cx
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_1_0635
        cmp    ax, 8000h
        jb     SHORT loc_1_0646
        sub    ax, 6000h
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    SHORT loc_1_064B
loc_1_0635:
        cmp    ax, 3000h
        jae    SHORT loc_1_0646
        add    ax, 3000h
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    SHORT loc_1_064B
loc_1_0646:
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_064B:
        add    edx, 0ch
        cmp    byte ptr [esi + 0fh], 0
        out    dx, ax
        je     SHORT loc_1_0665
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 10h
        out    dx, al
loc_1_0665:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_078D
loc_1_066F:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_06A2
        lea    eax, [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_2390
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_078D
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_236C
        jmp    NEAR PTR loc_1_0781
loc_1_06A2:
        mov    edi, dword ptr [ebp + 10h]
        sub    edi, dword ptr [ebp - 10h]
        cmp    edi, dword ptr [ebp - 0ch]
        jbe    SHORT loc_1_06B0
        mov    edi, dword ptr [ebp - 0ch]
loc_1_06B0:
        cmp    byte ptr [esi + 0fh], 0
        je     SHORT loc_1_0720
        test   bl, bl
        je     SHORT loc_1_06D2
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        dec    edi
        add    edx, 0dh
        inc    dword ptr [ebp - 10h]
        in     al, dx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        dec    dword ptr [ebp - 0ch]
loc_1_06D2:
        mov    ebx, edi
        and    ebx, 0fffffffch
        je     SHORT loc_1_06F1
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 8]
        shr    eax, 2
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0ch
        push   eax
        call   sub_1_221E
loc_1_06F1:
        test   edi, 2
        je     SHORT loc_1_070A
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        add    edx, 0ch
        in     ax, dx
        mov    word ptr [ebx + ecx], ax
loc_1_070A:
        test   edi, 1
        je     SHORT loc_1_071C
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0ch
        jmp    SHORT loc_1_076E
loc_1_071C:
        xor    bl, bl
        jmp    SHORT loc_1_0778
loc_1_0720:
        test   bl, bl
        je     SHORT loc_1_073C
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        dec    edi
        add    edx, 0fh
        inc    dword ptr [ebp - 10h]
        in     al, dx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        dec    dword ptr [ebp - 0ch]
loc_1_073C:
        test   edi, 0fffffffeh
        je     SHORT loc_1_075C
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 8]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0eh
        push   eax
        call   sub_1_2209
loc_1_075C:
        test   edi, 1
        mov    bl, 0
        je     SHORT loc_1_0778
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0eh
loc_1_076E:
        mov    ecx, dword ptr [ebp - 8]
        mov    bl, 1
        in     al, dx
        mov    byte ptr [ecx + edi - 1], al
loc_1_0778:
        add    dword ptr [ebp - 8], edi
        add    dword ptr [ebp - 10h], edi
        sub    dword ptr [ebp - 0ch], edi
loc_1_0781:
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 10h], ecx
        ja     NEAR PTR loc_1_066F
loc_1_078D:
        cmp    byte ptr [esi + 0fh], 0
        je     SHORT loc_1_079F
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
loc_1_079F:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [eax], ecx
loc_1_07A7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_07B0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        cmp    byte ptr [ecx + 158h], 0
        jne    SHORT loc_1_082D
        cmp    byte ptr [edx + 18h], 0
        jne    SHORT loc_1_0823
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_1_07E3
        cmp    eax, 1
        je     SHORT loc_1_0803
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    8
loc_1_07E3:
        lea    eax, [esi + 24h]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    esi, dword ptr [esi + 14h]
        push   esi
        push   edx
        push   ecx
        call   sub_1_0A70
        pop    esi
        pop    ebp
        ret    8
loc_1_0803:
        lea    eax, [esi + 24h]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    esi, dword ptr [esi + 14h]
        push   esi
        push   edx
        push   ecx
        call   sub_1_0880
        pop    esi
        pop    ebp
        ret    8
loc_1_0823:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    8
loc_1_082D:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0840:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    dword ptr [ecx + 10h], 2
        je     SHORT loc_1_0856
        mov    eax, 0c00000bbh
        pop    ebp
        ret    8
loc_1_0856:
        lea    eax, [ecx + 24h]
        lea    edx, [ecx + 20h]
        push   eax
        push   edx
        mov    eax, dword ptr [ecx + 1ch]
        mov    edx, dword ptr [ecx + 18h]
        push   eax
        mov    ecx, dword ptr [ecx + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        push   0
        push   eax
        call   sub_1_0A70
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0880:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        cmp    eax, 1010103h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 20h]
        mov    dword ptr [edi], 0
        ja     SHORT loc_1_08BF
        je     NEAR PTR loc_1_0A10
        cmp    eax, 1010eh
        je     SHORT loc_1_08CE
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_094C
        cmp    eax, 10112h
        je     NEAR PTR loc_1_09D4
loc_1_08BF:
        mov    edi, 0c0010017h
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_08CE:
        mov    esi, dword ptr [ebp + 18h]
        cmp    esi, 4
        je     SHORT loc_1_08E9
        mov    eax, 0c0010015h
        mov    dword ptr [edi], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_08E9:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_2404
        test   dword ptr [ebp - 8], 0f054h
        je     SHORT loc_1_091D
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 4
        mov    eax, 0c00000bbh
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_091D:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0ch]
        mov    eax, dword ptr [edi + 1f3h]
        push   edx
        push   eax
        db  0E8h, 0D5h, 00Ch, 000h, 000h
        mov    edi, eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_094C:
        xor    edi, edi
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_096C
        mov    edi, 0c0010014h
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_096C:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2404
        cmp    dword ptr [ebp - 4], 100h
        ja     SHORT loc_1_09C5
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 1f7h]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_09B1
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 1f7h], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 41h], ax
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_09B1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 41h], ax
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_09C5:
        mov    edi, 0c0000001h
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_09D4:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_09F2
        mov    edi, 0c0010014h
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_09F2:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 43h
        xor    edi, edi
        push   ecx
        call   sub_1_2404
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_0A10:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, esi
        cdq
        idiv   ecx
        test   edx, edx
        je     SHORT loc_1_0A30
        mov    eax, 0c0010015h
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_0A30:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq
        idiv   ecx
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [ecx + 1f3h]
        push   eax
        push   edx
        db  0E8h, 0D7h, 008h, 000h, 000h
        mov    edi, eax
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, edi
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0A70:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 24h
        mov    word ptr [ebp - 2], cx
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 1ch]
        push   edi
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 20h]
        lea    esi, [ebp - 8]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        mov    ecx, 4
        mov    edx, dword ptr [ebp + 10h]
        cmp    edx, 1010101h
        mov    eax, edx
        ja     SHORT loc_1_0ABE
        je     NEAR PTR loc_1_0DEB
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_0ADF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0ABE:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_0E10
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_0E35
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_0EC4
loc_1_0ADF:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_0AF1
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_0ECB
loc_1_0AF1:
        cmp    edx, 2020dh
        ja     SHORT loc_1_0B11
        je     NEAR PTR loc_1_0BF6
        sub    edx, 20101h
        cmp    edx, 4
        ja     SHORT loc_1_0B2B
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_0B11:
        cmp    edx, 1020101h
        ja     SHORT loc_1_0B37
        je     NEAR PTR loc_1_0C18
        cmp    edx, 2020eh
        je     NEAR PTR loc_1_0C07
loc_1_0B2B:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_0ECB
loc_1_0B37:
        cmp    edx, 1020201h
        ja     SHORT loc_1_0B5F
        je     NEAR PTR loc_1_0C4B
        cmp    edx, 1020102h
        je     NEAR PTR loc_1_0C29
        cmp    edx, 1020103h
        je     NEAR PTR loc_1_0C3A
        jmp    SHORT loc_1_0B2B
loc_1_0B5F:
        sub    edx, 1020202h
        cmp    edx, 4
        ja     SHORT loc_1_0B2B
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_0B71:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 162h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0B82:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 166h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0B93:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17ah]
        add    eax, dword ptr [edi + 16eh]
        add    eax, dword ptr [edi + 172h]
        add    eax, dword ptr [edi + 176h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0BB6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 19eh]
        add    eax, dword ptr [edi + 18eh]
        add    eax, dword ptr [edi + 1a2h]
        add    eax, dword ptr [edi + 196h]
        add    eax, dword ptr [edi + 18ah]
        add    eax, dword ptr [edi + 19ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0BE5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 19eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0BF6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 196h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C07:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C18:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 19ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C29:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 182h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C3A:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 186h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C4B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C5C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C6D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 1a2h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C7E:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 176h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0C8F:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CA0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 172h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CB1:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_0CC6
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 0a0h
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CC6:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 5ch
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CD5:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CE1:
        lea    esi, [ebp - 1ch]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CEE:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_0ECB
loc_1_0CFA:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D06:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D12:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 11h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D23:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 13h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D34:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D40:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 2ch
        lea    eax, [ebp - 14h]
        push   edi
        lea    esi, [ebp - 14h]
        push   eax
        call   sub_1_2404
        mov    byte ptr [ebp - 11h], 0
        mov    ecx, 4
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D63:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 1fh
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D72:
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        je     SHORT loc_1_0D87
        mov    eax, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D87:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 1f3h]
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_0ECB
loc_1_0D9B:
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        jne    SHORT loc_1_0DB6
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 1f7h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0DB6:
        xor    eax, eax
        mov    ax, word ptr [ebx + 41h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0ECB
loc_1_0DC4:
        lea    esi, [ebp - 2]
        mov    ecx, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_0ECB
loc_1_0DD7:
        lea    esi, [ebp - 8]
        mov    ecx, 4
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_0ECB
loc_1_0DEB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 2ch]
        add    edi, 2ch
        lea    esi, [ebp - 24h]
        mov    dword ptr [ebp - 24h], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 20h], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 1fh], dl
        jmp    NEAR PTR loc_1_0ECB
loc_1_0E10:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 32h]
        add    edi, 32h
        lea    esi, [ebp - 24h]
        mov    dword ptr [ebp - 24h], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 20h], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 1fh], dl
        jmp    NEAR PTR loc_1_0ECB
loc_1_0E35:
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        jne    SHORT loc_1_0E6D
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 18h]
        push   ecx
        mov    edi, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 18h]
        mov    eax, dword ptr [edi + 1f3h]
        push   eax
        push   ecx
        db  0E8h, 035h, 009h, 000h, 000h
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_0ECB
loc_1_0E6D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    ecx, dword ptr [edi + 1f3h]
        push   ecx
        lea    edx, [ebp - 18h]
        push   edx
        db  0E8h, 040h, 008h, 000h, 000h
        cmp    dword ptr [ebp - 18h], 0
        mov    esi, dword ptr [ebp + 14h]
        jne    SHORT loc_1_0EA8
        mov    eax, dword ptr [ebp - 10h]
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    SHORT loc_1_0ECB
loc_1_0EA8:
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [edi + 1f3h]
        push   eax
        push   ecx
        db  0E8h, 0D8h, 007h, 000h, 000h
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    SHORT loc_1_0ECB
loc_1_0EC4:
        mov    dword ptr [ebp - 8], 10h
loc_1_0ECB:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_0F02
        cmp    ecx, dword ptr [ebp + 18h]
        jbe    SHORT loc_1_0EEE
        mov    eax, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [ebp - 0ch], 0c0010016h
        pop    esi
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 0ch]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_0EEE:
        mov    eax, dword ptr [ebp + 1ch]
        test   ecx, ecx
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_0F02
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        push   eax
        call   sub_1_2404
loc_1_0F02:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
loc_1_0F90:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    edi, edi
        cmp    byte ptr [esi + 158h], 0
        je     SHORT loc_1_0FB6
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_0FB6:
        xor    ecx, ecx
        mov    al, 0bh
        mov    cx, word ptr [esi]
        mov    edx, ecx
        out    dx, al
        mov    al, 80h
        mov    ebx, dword ptr [ebp + 0ch]
        out    dx, al
        lea    edx, [ecx + 2]
        in     al, dx
        and    al, 0feh
        or     al, 2
        test   bl, 24h
        je     SHORT loc_1_0FD5
        or     al, 1
loc_1_0FD5:
        test   bl, 8
        je     SHORT loc_1_0FDC
        and    al, 0fdh
loc_1_0FDC:
        out    dx, al
        lea    edx, [ecx + 3]
        push   esi
        in     al, dx
        out    dx, al
        mov    edx, ecx
        xor    al, al
        out    dx, al
        call   sub_1_1770
        test   al, al
        je     SHORT loc_1_104C
        mov    dx, word ptr [esi]
        mov    al, 1eh
        out    dx, al
        push   3
        call   sub_1_2318
        mov    bx, word ptr [esi + 11h]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    ax, bx
        lea    edx, [ecx + 4]
        out    dx, ax
        mov    ax, word ptr [esi + 13h]
        lea    edx, [ecx + 6]
        add    ax, bx
        dec    ax
        out    dx, ax
        lea    edx, [ecx + 0ah]
        xor    eax, eax
        out    dx, ax
        mov    word ptr [esi + 21h], ax
        mov    word ptr [esi + 1fh], ax
        mov    word ptr [esi + 1dh], ax
        mov    word ptr [esi + 15h], ax
        mov    word ptr [esi + 19h], ax
        mov    word ptr [esi + 1bh], ax
        mov    word ptr [esi + 17h], 1
        mov    bx, word ptr [esi + 11h]
        mov    word ptr [esi + 23h], bx
        jmp    SHORT loc_1_1051
loc_1_104C:
        mov    edi, 0c0000001h
loc_1_1051:
        mov    dx, word ptr [esi]
        mov    al, 8
        out    dx, al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_1060:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 158h], 0
        je     SHORT loc_1_107C
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_107C:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 10h
        jbe    SHORT loc_1_108D
        mov    eax, 0c000009ah
        pop    ebp
        ret    1ch
loc_1_108D:
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        push   eax
        push   edx
        call   sub_1_1510
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        push   esi
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        dec    dword ptr [edi + 14h]
        call   0
        push   0
        push   0
        push   edi
        call   sub_1_2324
        push   0
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_225C
        cmp    dword ptr [esi + 192h], 0
        jne    SHORT loc_1_10FC
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
loc_1_10FC:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 8]
        cmp    byte ptr [ebx + 158h], 0
        jne    SHORT loc_1_1165
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_1157
        mov    byte ptr [ebx + 157h], 1
        push   ebx
        call   sub_1_1230
        push   ebx
        call   sub_1_1190
        push   esi
        push   ebx
        call   sub_1_1180
        push   edi
        push   ebx
        call   sub_1_1290
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1157:
        mov    edi, 0c0010002h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1165:
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1180:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1190:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 147h], 0
        je     SHORT loc_1_11DC
        xor    edi, edi
loc_1_11A4:
        mov    edx, dword ptr [esi + 147h]
        lea    eax, [edx + 24h]
        cmp    dword ptr [eax], edi
        jne    SHORT loc_1_11B7
        mov    dword ptr [esi + 14bh], edi
loc_1_11B7:
        mov    ecx, dword ptr [eax]
        push   0c001000ch
        push   edx
        mov    dword ptr [esi + 147h], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_2262
        dec    dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 147h], edi
        jne    SHORT loc_1_11A4
loc_1_11DC:
        cmp    dword ptr [esi + 13fh], 0
        je     SHORT loc_1_1223
        xor    edi, edi
loc_1_11E7:
        mov    edx, dword ptr [esi + 13fh]
        lea    eax, [edx + 24h]
        cmp    dword ptr [eax], edi
        jne    SHORT loc_1_11FA
        mov    dword ptr [esi + 143h], edi
loc_1_11FA:
        mov    ecx, dword ptr [eax]
        push   0c001000ch
        dec    word ptr [esi + 29h]
        push   edx
        mov    dword ptr [esi + 13fh], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_2262
        dec    dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 13fh], edi
        jne    SHORT loc_1_11E7
loc_1_1223:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1230:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 1a6h]
        sub    eax, edi
        cmp    eax, 0fffffe5ah
        je     SHORT loc_1_1277
loc_1_1249:
        push   0
        inc    dword ptr [edi + 14h]
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   40010004h
        push   eax
        call   sub_1_2286
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_228C
        mov    eax, esi
        dec    dword ptr [edi + 14h]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0fffffe5ah
        jne    SHORT loc_1_1249
loc_1_1277:
        mov    byte ptr [esi + 158h], 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1290:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 1a6h]
        sub    eax, edi
        cmp    eax, 0fffffe5ah
        je     SHORT loc_1_12E8
        xor    ebx, ebx
loc_1_12AC:
        inc    dword ptr [edi + 14h]
        cmp    dword ptr [ebp + 0ch], ebx
        je     SHORT loc_1_12C4
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   ebx
        push   40010007h
        push   eax
        call   sub_1_2286
loc_1_12C4:
        push   4
        lea    eax, [ebp + 0ch]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        push   40010005h
        push   ecx
        call   sub_1_2286
        mov    eax, esi
        dec    dword ptr [edi + 14h]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0fffffe5ah
        jne    SHORT loc_1_12AC
loc_1_12E8:
        mov    byte ptr [esi + 158h], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1300:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        mov    al, 0fh
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        lea    esi, [edi + 1e2h]
        add    edx, 3
        out    dx, al
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_2300
        push   edi
        call   sub_1_1770
        xor    ecx, ecx
        mov    word ptr [edi + 21h], cx
        mov    word ptr [edi + 1fh], cx
        mov    word ptr [edi + 17h], 1
        mov    word ptr [edi + 1dh], cx
        xor    edx, edx
        mov    dx, word ptr [edi]
        xor    eax, eax
        mov    word ptr [edi + 15h], cx
        add    edx, 0ah
        out    dx, ax
        mov    word ptr [edi + 19h], cx
        cmp    dword ptr [edi + 147h], ecx
        mov    word ptr [edi + 1bh], cx
        je     SHORT loc_1_1370
loc_1_1361:
        push   edi
        call   sub_1_03F0
        cmp    dword ptr [edi + 147h], 0
        jne    SHORT loc_1_1361
loc_1_1370:
        push   edi
        call   sub_1_00E0
        cmp    byte ptr [edi + 0dh], 0
        jne    SHORT loc_1_13B2
        push   0eh
        push   0eh
        push   edi
        call   sub_1_16D0
        test   al, al
        jne    SHORT loc_1_13B2
        push   0
        mov    eax, dword ptr [edi + 1c6h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_23F0
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_13B2:
        push   edi
        call   sub_1_13D0
        mov    bl, al
        push   1388h
        push   esi
        call   sub_1_22FA
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_13D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    al, 40h
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    cl, byte ptr [esi + 0ch]
        mov    di, word ptr [esi]
        mov    edx, edi
        out    dx, al
        lea    edx, [edi + 2]
        in     al, dx
        and    al, 0f8h
        or     cl, al
        push   esi
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 1]
        xor    edi, edi
        in     al, dx
        or     al, 80h
        out    dx, al
        call   sub_1_1870
        mov    di, word ptr [esi]
        mov    bx, word ptr [esi + 11h]
        lea    edx, [edi + 8]
        mov    al, bh
        mov    cl, bh
        out    dx, al
        lea    edx, [edi + 9]
        xor    eax, eax
        mov    ax, word ptr [esi + 13h]
        and    ebx, 0ffffh
        add    eax, ebx
        sar    eax, 8
        dec    al
        out    dx, al
        lea    edx, [edi + 0ah]
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0bh]
        lea    eax, [ecx - 1]
        out    dx, al
        mov    edx, edi
        mov    al, 80h
        out    dx, al
        lea    edx, [edi + 1]
        in     al, dx
        or     al, 0a0h
        out    dx, al
        mov    cx, word ptr [esi + 2]
        lea    edx, [edi + 2]
        test   cx, cx
        mov    al, 14h
        out    dx, al
        lea    edx, [edi + 3]
        in     al, dx
        je     SHORT loc_1_147C
        cmp    cx, 4
        jae    SHORT loc_1_147C
        or     al, 10h
        and    al, 3fh
        cmp    cx, 1
        jne    SHORT loc_1_1466
        and    al, 0fbh
        or     al, 20h
loc_1_1466:
        cmp    cx, 2
        jne    SHORT loc_1_1470
        or     al, 4
        or     al, 20h
loc_1_1470:
        cmp    cx, 3
        jne    SHORT loc_1_147E
        or     al, 4
        and    al, 0dfh
        jmp    SHORT loc_1_147E
loc_1_147C:
        and    al, 2fh
loc_1_147E:
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    di, 3
        mov    al, 1eh
        out    dx, al
loc_1_148B:
        push   64h
        call   sub_1_2318
        dec    di
        jne    SHORT loc_1_148B
        mov    cx, word ptr [esi + 11h]
        xor    edi, edi
        mov    di, word ptr [esi]
        mov    ax, cx
        lea    edx, [edi + 4]
        out    dx, ax
        mov    ax, word ptr [esi + 13h]
        lea    edx, [edi + 6]
        add    ax, cx
        push   esi
        dec    ax
        out    dx, ax
        lea    edx, [edi + 0ah]
        xor    eax, eax
        out    dx, ax
        call   sub_1_1600
        mov    word ptr [esi + 17h], 1
        xor    eax, eax
        mov    word ptr [esi + 21h], ax
        mov    word ptr [esi + 1fh], ax
        mov    cx, word ptr [esi + 11h]
        mov    word ptr [esi + 1dh], ax
        push   7
        mov    word ptr [esi + 15h], ax
        push   7
        mov    word ptr [esi + 19h], ax
        push   esi
        mov    word ptr [esi + 1bh], ax
        mov    word ptr [esi + 23h], cx
        call   sub_1_16D0
        test   al, al
        jne    SHORT loc_1_1501
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1501:
        push   esi
        call   sub_1_1690
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_1510:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    edx, edx
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        mov    al, 0bh
        out    dx, al
        add    edx, 3
        push   esi
        in     al, dx
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 2], al
        mov    al, 0fh
        out    dx, al
        call   sub_1_1770
        test   al, al
        je     NEAR PTR loc_1_15E1
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 0ah]
        out    dx, ax
        lea    edx, [ecx + 0ch]
        out    dx, ax
        lea    edx, [ecx + 0eh]
        mov    eax, 3
        out    dx, ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        out    dx, ax
        imul   cx, cx, 6
        out    dx, ax
        mov    ax, cx
        cmp    dword ptr [ebp + 0ch], ebx
        out    dx, ax
        je     SHORT loc_1_15A3
        mov    edi, dword ptr [ebp + 10h]
loc_1_1576:
        push   3
        mov    eax, ebx
        inc    bx
        and    eax, 0ffffh
        lea    ecx, [eax + eax*2]
        xor    eax, eax
        mov    ax, word ptr [esi]
        lea    edx, [edi + ecx*2]
        push   edx
        add    eax, 0eh
        push   eax
        call   sub_1_21CC
        mov    ecx, ebx
        and    ecx, 0ffffh
        cmp    ecx, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_1576
loc_1_15A3:
        push   3
        push   3
        push   esi
        call   sub_1_16D0
        test   al, al
        je     SHORT loc_1_15E1
        mov    ecx, 1
        xor    eax, eax
        mov    word ptr [esi + 21h], ax
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [esi + 1fh], ax
        mov    word ptr [esi + 1dh], ax
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    word ptr [esi + 15h], ax
        add    edx, 0ah
        out    dx, ax
        mov    word ptr [esi + 19h], ax
        mov    word ptr [esi + 1bh], ax
        mov    word ptr [esi + 17h], cx
loc_1_15E1:
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi]
        pop    edi
        out    dx, al
        add    edx, 3
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_1600:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        xor    eax, eax
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    cx, word ptr [edi]
        lea    edx, [ecx + 0ah]
        out    dx, ax
        lea    edx, [ecx + 0ch]
        out    dx, ax
        lea    edx, [ecx + 0eh]
        mov    eax, 2
        out    dx, ax
        xor    eax, eax
        out    dx, ax
        out    dx, ax
        mov    eax, 0ah
        out    dx, ax
        mov    eax, 2eh
        out    dx, ax
        mov    eax, 60h
        out    dx, ax
        mov    eax, 8f2h
        out    dx, ax
        mov    eax, 4000h
        out    dx, ax
        mov    si, 64h
        mov    eax, 0b6h
        out    dx, ax
        mov    edx, ecx
        mov    al, 2
        out    dx, al
loc_1_165C:
        push   64h
        call   sub_1_2318
        dec    si
        jne    SHORT loc_1_165C
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [edi]
        lea    edx, [ecx + 0ah]
        pop    edi
        out    dx, ax
        lea    edx, [ecx + 0ch]
        pop    esi
        out    dx, ax
        lea    edx, [ecx + 1]
        mov    al, 8
        out    dx, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, 80h
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi]
        xor    cl, cl
        mov    edx, edi
        out    dx, al
loc_1_16A4:
        xor    eax, eax
        mov    al, cl
        inc    cl
        cmp    cl, 6
        lea    edx, [edi + eax + 4]
        mov    al, byte ptr [eax + esi + 32h]
        out    dx, al
        jb     SHORT loc_1_16A4
        mov    edx, edi
        xor    al, al
        out    dx, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    edx, edx
        push   ebx
        push   esi
        mov    al, 8
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        mov    dx, word ptr [esi]
        mov    di, 3e8h
        inc    edx
        out    dx, al
loc_1_16ED:
        push   64h
        call   sub_1_2318
        dec    di
        jne    SHORT loc_1_16ED
        mov    dx, word ptr [esi]
        mov    bl, byte ptr [ebp + 0ch]
        mov    di, 3e8h
        mov    al, bl
        out    dx, al
loc_1_1705:
        push   64h
        call   sub_1_2318
        dec    di
        jne    SHORT loc_1_1705
        cmp    bl, 0eh
        jne    SHORT loc_1_171B
        push   esi
        call   sub_1_17B0
loc_1_171B:
        xor    di, di
loc_1_171E:
        xor    ebx, ebx
        mov    bx, word ptr [esi]
        lea    ecx, [ebx + 1]
        mov    edx, ecx
        in     al, dx
        test   al, 8
        je     SHORT loc_1_173E
        mov    edx, ebx
        in     al, dx
        and    al, 0fh
        cmp    al, byte ptr [ebp + 10h]
        je     SHORT loc_1_1759
        mov    edx, ecx
        mov    al, 8
        out    dx, al
        jmp    SHORT loc_1_1745
loc_1_173E:
        push   64h
        call   sub_1_2318
loc_1_1745:
        inc    di
        cmp    di, 64h
        jb     SHORT loc_1_171E
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1759:
        mov    byte ptr [ebp - 1], 1
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1770:
        push   ebp
        mov    ebp, esp
        push   esi
        xor    si, si
        push   edi
        mov    edi, dword ptr [ebp + 8]
loc_1_177B:
        xor    edx, edx
        mov    dx, word ptr [edi]
        inc    edx
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_17A0
        push   64h
        call   sub_1_2318
        mov    ax, si
loc_1_1790:
        inc    si
        cmp    ax, 3e8h
        jne    SHORT loc_1_177B
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_17A0:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_17B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        mov    byte ptr [ebp - 20h], 6ah
        mov    byte ptr [ebp - 1fh], 0b5h
        mov    byte ptr [ebp - 1eh], 0dah
        mov    byte ptr [ebp - 1dh], 0edh
        mov    byte ptr [ebp - 1ch], 0f6h
        mov    byte ptr [ebp - 1bh], 0fbh
        mov    byte ptr [ebp - 1ah], 7dh
        mov    byte ptr [ebp - 19h], 0beh
        mov    byte ptr [ebp - 18h], 0dfh
        mov    byte ptr [ebp - 17h], 6fh
        mov    byte ptr [ebp - 16h], 37h
        mov    byte ptr [ebp - 15h], 1bh
        mov    byte ptr [ebp - 14h], 0dh
        mov    byte ptr [ebp - 13h], 86h
        mov    byte ptr [ebp - 12h], 0c3h
        mov    byte ptr [ebp - 11h], 61h
        mov    byte ptr [ebp - 10h], 0b0h
        mov    byte ptr [ebp - 0fh], 58h
        mov    byte ptr [ebp - 0eh], 2ch
        mov    byte ptr [ebp - 0dh], 16h
        mov    byte ptr [ebp - 0ch], 8bh
        mov    byte ptr [ebp - 0bh], 45h
        mov    byte ptr [ebp - 0ah], 0a2h
        mov    byte ptr [ebp - 9], 0d1h
        mov    byte ptr [ebp - 8], 0e8h
        mov    byte ptr [ebp - 7], 74h
        mov    byte ptr [ebp - 6], 3ah
        mov    byte ptr [ebp - 5], 9dh
        mov    byte ptr [ebp - 4], 0ceh
        mov    byte ptr [ebp - 3], 0e7h
        mov    byte ptr [ebp - 2], 73h
        mov    byte ptr [ebp - 1], 43h
        pushf
        cli
        xor    cx, cx
        mov    edx, 279h
        xor    al, al
        out    dx, al
        out    dx, al
loc_1_1845:
        mov    eax, ecx
        mov    edx, 279h
        inc    cx
        and    eax, 0ffffh
        cmp    cx, 20h
        mov    al, byte ptr [ebp + eax - 20h]
        out    dx, al
        jb     SHORT loc_1_1845
        popf
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1870:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx]
        mov    cx, word ptr [ecx + 4]
        inc    edx
        in     al, dx
        mov    esi, ecx
        and    esi, 0ffffh
        cmp    esi, -1
        je     SHORT loc_1_18B1
        cmp    cx, 1
        jne    SHORT loc_1_1898
        or     al, 42h
        out    dx, al
loc_1_1898:
        cmp    cx, 3
        jne    SHORT loc_1_18A1
        and    al, 0bdh
        out    dx, al
loc_1_18A1:
        cmp    cx, 2
        jne    SHORT loc_1_18B6
        and    al, 0bfh
        pop    esi
        or     al, 2
        pop    ebp
        out    dx, al
        ret    4
loc_1_18B1:
        and    al, 0bfh
        or     al, 2
        out    dx, al
loc_1_18B6:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_18C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    si, word ptr [eax]
        xor    al, al
        mov    edx, esi
        lea    edi, [esi + 0ch]
        mov    cx, 10h
        add    esi, 0eh
        out    dx, al
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        mov    ax, 3
loc_1_18E8:
        mov    edx, esi
        out    dx, ax
        add    ax, 3
        dec    cx
        jne    SHORT loc_1_18E8
        mov    cx, 3
        mov    edx, edi
        xor    bx, bx
        xor    eax, eax
        out    dx, ax
loc_1_1901:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    SHORT loc_1_1979
        add    cx, 3
        inc    bx
        cmp    bx, 10h
        jb     SHORT loc_1_1901
        mov    cx, 10h
        mov    edx, edi
        mov    eax, 8000h
        out    dx, ax
        mov    ax, 9
loc_1_1927:
        mov    edx, esi
        out    dx, ax
        add    ax, 3
        dec    cx
        jne    SHORT loc_1_1927
        mov    cx, 10h
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
loc_1_193D:
        mov    edx, esi
        xor    eax, eax
        out    dx, ax
        dec    cx
        jne    SHORT loc_1_193D
        mov    cx, 9
        mov    edx, edi
        xor    bx, bx
        mov    eax, 8000h
        out    dx, ax
loc_1_1957:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    SHORT loc_1_1984
        add    cx, 3
        inc    bx
        cmp    bx, 10h
        jb     SHORT loc_1_1957
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1979:
        mov    ax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1984:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_1990:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    edx, edx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        mov    dx, word ptr [edi]
        add    edx, 3
        in     al, dx
        cmp    byte ptr [edi + 158h], 0
        mov    byte ptr [ebp - 1], al
        mov    al, 0fh
        out    dx, al
        jne    NEAR PTR loc_1_1A79
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [edi + 15h]
        mov    cx, word ptr [edi + 11h]
        add    eax, 5f2h
        cmp    eax, ecx
        jge    SHORT loc_1_1A39
        cmp    dword ptr [edi + 13fh], 0
        jne    SHORT loc_1_1A39
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_1A32
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        push   esi
        push   edi
        call   sub_1_1AA0
        cmp    eax, 103h
        mov    ecx, eax
        jne    SHORT loc_1_1A27
        mov    dword ptr [ebx + 2ch], esi
        lea    eax, [ebx + 24h]
        inc    dword ptr [esi + 14h]
        cmp    dword ptr [edi + 147h], 0
        jne    SHORT loc_1_1A0F
        mov    dword ptr [edi + 147h], ebx
        jmp    SHORT loc_1_1A18
loc_1_1A0F:
        mov    edx, dword ptr [edi + 14bh]
        mov    dword ptr [edx + 24h], ebx
loc_1_1A18:
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 14bh], ebx
        dec    dword ptr [esi + 14h]
loc_1_1A27:
        mov    word ptr [edi + 1f9h], 3
        jmp    SHORT loc_1_1A7E
loc_1_1A32:
        mov    ecx, 0c0010002h
        jmp    SHORT loc_1_1A7E
loc_1_1A39:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 2ch], esi
        lea    eax, [ebx + 24h]
        inc    dword ptr [esi + 14h]
        cmp    dword ptr [edi + 13fh], 0
        jne    SHORT loc_1_1A56
        mov    dword ptr [edi + 13fh], ebx
        jmp    SHORT loc_1_1A5F
loc_1_1A56:
        mov    ecx, dword ptr [edi + 143h]
        mov    dword ptr [ecx + 24h], ebx
loc_1_1A5F:
        mov    ecx, 103h
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 143h], ebx
        inc    dword ptr [esi + 14h]
        inc    word ptr [edi + 29h]
        jmp    SHORT loc_1_1A7E
loc_1_1A79:
        mov    ecx, 0c001000dh
loc_1_1A7E:
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi]
        pop    edi
        add    edx, 3
        pop    esi
        out    dx, al
        mov    eax, ecx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        lea    eax, [ebp - 30h]
        push   esi
        lea    ecx, [ebp - 14h]
        push   edi
        lea    edx, [ebp - 2ch]
        xor    edi, edi
        push   eax
        mov    word ptr [ebp - 8], di
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        push   edi
        mov    dword ptr [ebp - 20h], edi
        push   eax
        call   sub_1_2348
        cmp    dword ptr [ebp - 30h], edi
        je     NEAR PTR loc_1_1EAD
        cmp    dword ptr [ebp - 30h], 5eah
        ja     NEAR PTR loc_1_1EAD
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_236C
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 1bh]
        mov    bx, word ptr [esi + 11h]
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    edx, ebx
        add    ecx, 8
        and    edx, 0ffffh
        cmp    ecx, edx
        jge    SHORT loc_1_1B18
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    SHORT loc_1_1B20
loc_1_1B18:
        sub    ax, bx
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_1B20:
        add    ax, 8
        add    edx, 0ch
        cmp    byte ptr [esi + 0fh], 0
        out    dx, ax
        je     NEAR PTR loc_1_1C20
        xor    edx, edx
        push   6
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp - 10h]
        add    edx, 3
        push   ecx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        or     al, 10h
        out    dx, al
        lea    edx, [ebp - 38h]
        push   edx
        call   sub_1_2404
        mov    bx, word ptr [ebp - 2ch]
        test   bx, bx
        je     NEAR PTR loc_1_1CF8
loc_1_1B5E:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1BEF
        cmp    word ptr [ebp - 8], 0
        je     SHORT loc_1_1B85
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 10h]
        add    edx, 0dh
        mov    al, byte ptr [eax]
        dec    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 10h]
        out    dx, al
loc_1_1B85:
        test   dword ptr [ebp - 0ch], 0fffffffch
        je     SHORT loc_1_1BA7
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        shr    eax, 2
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0ch
        push   eax
        call   sub_1_21E1
loc_1_1BA7:
        test   byte ptr [ebp - 0ch], 2
        je     SHORT loc_1_1BC4
        xor    edx, edx
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [esi]
        and    eax, 0fffffffch
        mov    ecx, dword ptr [ebp - 10h]
        add    edx, 0ch
        mov    ax, word ptr [eax + ecx]
        out    dx, ax
loc_1_1BC4:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_1BE5
        xor    edx, edx
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ebp - 8], 1
        add    edx, 0ch
        mov    al, byte ptr [eax + ecx - 1]
        out    dx, al
        jmp    SHORT loc_1_1BEB
loc_1_1BE5:
        mov    word ptr [ebp - 8], 0
loc_1_1BEB:
        add    di, word ptr [ebp - 0ch]
loc_1_1BEF:
        cmp    bx, 1
        jbe    SHORT loc_1_1C13
        lea    eax, [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_2390
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 14h]
        push   edx
        push   eax
        call   sub_1_236C
loc_1_1C13:
        dec    bx
        jne    NEAR PTR loc_1_1B5E
        jmp    NEAR PTR loc_1_1CF8
loc_1_1C20:
        push   3
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0eh
        push   eax
        call   sub_1_21CC
        push   6
        mov    ecx, dword ptr [ebp - 10h]
        mov    di, 6
        push   ecx
        lea    edx, [ebp - 38h]
        push   edx
        call   sub_1_2404
        add    dword ptr [ebp - 10h], 6
        mov    bx, word ptr [ebp - 2ch]
        sub    dword ptr [ebp - 0ch], 6
        test   bx, bx
        je     NEAR PTR loc_1_1CF8
loc_1_1C5C:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_1CCC
        cmp    word ptr [ebp - 8], 0
        je     SHORT loc_1_1C7F
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 10h]
        add    edx, 0fh
        mov    al, byte ptr [eax]
        dec    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 10h]
        out    dx, al
loc_1_1C7F:
        test   dword ptr [ebp - 0ch], 0fffffffeh
        je     SHORT loc_1_1CA1
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0eh
        push   eax
        call   sub_1_21CC
loc_1_1CA1:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_1CC2
        xor    edx, edx
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ebp - 8], 1
        add    edx, 0eh
        mov    al, byte ptr [eax + ecx - 1]
        out    dx, al
        jmp    SHORT loc_1_1CC8
loc_1_1CC2:
        mov    word ptr [ebp - 8], 0
loc_1_1CC8:
        add    di, word ptr [ebp - 0ch]
loc_1_1CCC:
        cmp    bx, 1
        jbe    SHORT loc_1_1CF0
        lea    eax, [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_2390
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 14h]
        push   edx
        push   eax
        call   sub_1_236C
loc_1_1CF0:
        dec    bx
        jne    NEAR PTR loc_1_1C5C
loc_1_1CF8:
        cmp    word ptr [ebp - 8], 0
        je     SHORT loc_1_1D1C
        cmp    byte ptr [esi + 0fh], 0
        je     SHORT loc_1_1D0F
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0dh
        jmp    SHORT loc_1_1D17
loc_1_1D0F:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0fh
loc_1_1D17:
        inc    di
        xor    al, al
        out    dx, al
loc_1_1D1C:
        cmp    byte ptr [esi + 0fh], 0
        je     SHORT loc_1_1D2E
        xor    edx, edx
        mov    al, byte ptr [ebp - 4]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
loc_1_1D2E:
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    dword ptr [ebp - 4], eax
        lea    ebx, [eax + 0eh]
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        cmp    di, 3ch
        ja     SHORT loc_1_1D6F
        mov    ecx, 3ch
        mov    eax, edi
        and    eax, 0ffffh
        sub    ecx, eax
        sar    ecx, 1
        test   cx, cx
        je     SHORT loc_1_1D65
loc_1_1D5B:
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        dec    cx
        jne    SHORT loc_1_1D5B
loc_1_1D65:
        mov    word ptr [ebp - 8], 0
        mov    di, 3ch
loc_1_1D6F:
        mov    cx, word ptr [esi + 1bh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        mov    edx, ecx
        and    edx, 0ffffh
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 1ch], edx
        mov    edx, eax
        mov    eax, dword ptr [ebp - 1ch]
        out    dx, ax
        mov    edx, ebx
        mov    eax, 4
        out    dx, ax
        xor    eax, eax
        out    dx, ax
        mov    edx, edi
        mov    eax, dword ptr [ebp - 1ch]
        and    edx, 0ffffh
        lea    edx, [edx + eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 11h]
        mov    dword ptr [ebp - 24h], edx
        mov    dword ptr [ebp - 18h], eax
        cmp    edx, eax
        jge    SHORT loc_1_1DBF
        lea    eax, [ecx + edi]
        jmp    SHORT loc_1_1DC9
loc_1_1DBF:
        mov    ax, di
        sub    ax, word ptr [esi + 11h]
        add    ax, cx
loc_1_1DC9:
        add    ax, 8
        mov    edx, ebx
        out    dx, ax
        mov    ax, di
        mov    edx, ebx
        sub    ax, word ptr [ebp - 8]
        out    dx, ax
        cmp    word ptr [esi + 17h], 0
        mov    word ptr [ebp - 8], ax
        je     SHORT loc_1_1DF5
        mov    edx, dword ptr [ebp - 4]
        mov    al, 4
        out    dx, al
        mov    word ptr [esi + 17h], 0
        jmp    SHORT loc_1_1E14
loc_1_1DF5:
        mov    ax, word ptr [esi + 1dh]
        mov    edx, dword ptr [ebp - 28h]
        out    dx, ax
        mov    ax, word ptr [esi + 1fh]
        mov    edx, ebx
        or     ah, 80h
        and    eax, 0ffffh
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    al, 1ch
        out    dx, al
loc_1_1E14:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 1fh], ax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 6
        cmp    eax, dword ptr [ebp - 18h]
        jge    SHORT loc_1_1E2C
        lea    eax, [ecx + 6]
        jmp    SHORT loc_1_1E37
loc_1_1E2C:
        mov    ax, cx
        sub    ax, word ptr [esi + 11h]
        add    ax, 6
loc_1_1E37:
        mov    word ptr [esi + 1dh], ax
        mov    edx, dword ptr [ebp - 18h]
        lea    eax, [edi + 8]
        add    word ptr [esi + 15h], ax
        cmp    dword ptr [ebp - 24h], edx
        jge    SHORT loc_1_1E57
        lea    eax, [ecx + edi]
        add    ax, 8
        mov    word ptr [esi + 1bh], ax
        jmp    SHORT loc_1_1E66
loc_1_1E57:
        sub    di, word ptr [esi + 11h]
        add    di, cx
        add    di, 8
        mov    word ptr [esi + 1bh], di
loc_1_1E66:
        inc    word ptr [esi + 21h]
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 162h]
        test   byte ptr [eax + 43h], 2
        jne    SHORT loc_1_1E94
        lea    eax, [ebp - 38h]
        mov    ecx, dword ptr [esi + 1f3h]
        push   eax
        push   ecx
        call   sub_1_26E0
        test   al, al
        je     SHORT loc_1_1E94
        mov    dword ptr [ebp - 20h], 103h
loc_1_1E94:
        cmp    word ptr [ebp + 14h], 0
        jne    SHORT loc_1_1EA1
        push   esi
        call   sub_1_1EC0
loc_1_1EA1:
        mov    eax, dword ptr [ebp - 20h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1EAD:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_1EC0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 1], cl
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 21h], cx
        je     NEAR PTR loc_1_1FB4
        xor    edi, edi
        mov    di, word ptr [esi]
        lea    eax, [edi + 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_1EE6:
        mov    bx, word ptr [esi + 19h]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, ebx
        and    eax, 0ffffh
        out    dx, ax
        lea    edx, [edi + 0eh]
        in     ax, dx
        test   al, 80h
        je     NEAR PTR loc_1_1FB4
        mov    byte ptr [ebp - 1], 1
        in     ax, dx
        test   ah, 20h
        jne    SHORT loc_1_1F36
        mov    cl, 1
        test   al, 20h
        je     SHORT loc_1_1F1C
        inc    dword ptr [esi + 16eh]
        jmp    SHORT loc_1_1F5C
loc_1_1F1C:
        test   ah, 4
        je     SHORT loc_1_1F29
        inc    dword ptr [esi + 172h]
        jmp    SHORT loc_1_1F5C
loc_1_1F29:
        test   ah, 1
        je     SHORT loc_1_1F5C
        inc    dword ptr [esi + 176h]
        jmp    SHORT loc_1_1F5C
loc_1_1F36:
        test   al, 80h
        je     SHORT loc_1_1F42
        inc    dword ptr [esi + 17eh]
        jmp    SHORT loc_1_1F5C
loc_1_1F42:
        and    ax, 0fh
        cmp    ax, 1
        jne    SHORT loc_1_1F54
        inc    dword ptr [esi + 182h]
        jmp    SHORT loc_1_1F5C
loc_1_1F54:
        jbe    SHORT loc_1_1F5C
        inc    dword ptr [esi + 186h]
loc_1_1F5C:
        lea    edx, [edi + 0eh]
        in     ax, dx
        mov    dx, ax
        sub    dx, bx
        mov    bx, word ptr [esi + 11h]
        cmp    bx, dx
        jae    SHORT loc_1_1F73
        add    dx, bx
loc_1_1F73:
        mov    word ptr [esi + 1f9h], 3
        mov    bx, word ptr [esi + 21h]
        dec    bx
        mov    word ptr [esi + 19h], ax
        sub    word ptr [esi + 15h], dx
        test   cl, cl
        mov    word ptr [esi + 21h], bx
        je     SHORT loc_1_1FAB
        test   bx, bx
        lea    edx, [edi + 0ah]
        out    dx, ax
        je     SHORT loc_1_1FA3
        mov    edx, edi
        mov    al, 4
        out    dx, al
        jmp    SHORT loc_1_1FA9
loc_1_1FA3:
        mov    word ptr [esi + 17h], 1
loc_1_1FA9:
        xor    cl, cl
loc_1_1FAB:
        test   bx, bx
        jne    NEAR PTR loc_1_1EE6
loc_1_1FB4:
        cmp    dword ptr [esi + 13fh], 0
        je     NEAR PTR loc_1_206F
loc_1_1FC1:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 15h]
        mov    ax, word ptr [esi + 11h]
        add    ecx, 5f2h
        cmp    ecx, eax
        jge    NEAR PTR loc_1_206F
        mov    ebx, dword ptr [esi + 13fh]
        lea    edi, [ebx + 24h]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_1FF3
        mov    dword ptr [esi + 143h], 0
loc_1_1FF3:
        mov    eax, dword ptr [edi]
        lea    ecx, [edi + 8]
        dec    word ptr [esi + 29h]
        push   1
        push   ebx
        mov    dword ptr [esi + 13fh], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], ecx
        push   eax
        push   esi
        call   sub_1_1AA0
        cmp    eax, 103h
        mov    eax, dword ptr [ebp - 8]
        jne    SHORT loc_1_2048
        mov    ecx, dword ptr [eax]
        inc    dword ptr [ecx + 14h]
        cmp    dword ptr [esi + 147h], 0
        jne    SHORT loc_1_2031
        mov    dword ptr [esi + 147h], ebx
        jmp    SHORT loc_1_203A
loc_1_2031:
        mov    eax, dword ptr [esi + 14bh]
        mov    dword ptr [eax + 24h], ebx
loc_1_203A:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 14bh], ebx
        jmp    SHORT loc_1_2059
loc_1_2048:
        mov    edi, dword ptr [eax]
        push   0
        push   ebx
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_2262
        dec    dword ptr [edi + 14h]
loc_1_2059:
        cmp    dword ptr [esi + 13fh], 0
        mov    word ptr [esi + 1f9h], 3
        jne    NEAR PTR loc_1_1FC1
loc_1_206F:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_2080:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        add    eax, 0eh
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 8]
        cmp    byte ptr [edx + 158h], 0
        jne    SHORT loc_1_20F6
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_20EB
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        inc    dword ptr [esi + 14h]
        push   eax
        test   ecx, ecx
        jne    SHORT loc_1_20D1
        mov    eax, dword ptr [edx + 14fh]
        mov    ecx, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 18h]
        push   ecx
        push   0
        push   edx
        call   sub_1_2396
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebp
        ret    18h
loc_1_20D1:
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 14h]
        push   eax
        push   edi
        push   ecx
        push   edx
        call   sub_1_05B0
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebp
        ret    18h
loc_1_20EB:
        mov    eax, 0c001000ch
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_20F6:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2110:
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_2135
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_2137
        push   0
        push   0
        db  0E8h, 0D0h, 0DEh, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_2137
loc_1_2135:
        clc
        ret
loc_1_2137:
        stc
        ret
EPRO_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_2148
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_2148:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 056h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h
        db  00Ch, 000h
sub_1_21CC:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsw
        pop    esi
        ret    0ch
sub_1_21E1:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        cld
        rep outsd
        pop    esi
        ret    0ch
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_2209:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insw
        pop    edi
        ret    0ch
sub_1_221E:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        cld
        rep insd
        pop    edi
        ret    0ch
sub_1_2232:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_2238:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_223E:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_2244:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_224A:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_2250:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_225C:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_2262:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_227A:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_2280:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_2286:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_228C:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_22A4:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        int3
        int3
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_22FA:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_2300:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_2306:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_230C:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_2318:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_231E:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_2324:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_2348:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_236C:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_2390:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_2396:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_239C:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_23A2:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_23A8:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_23F0:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        int3
        int3
sub_1_2404:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_2424:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_2444
        or     ecx, ecx
        je     SHORT loc_1_245A
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_2458
loc_1_2444:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_2458
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_2458:
        rep movsb
loc_1_245A:
        popfd
        pop    edi
        pop    esi
        ret    0ch
sub_1_2460:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2490
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_2490:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_2530
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_24D0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_24C5
        cmp    al, 0ffh
        jne    SHORT loc_1_24C5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_24C5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_2537
loc_1_24C5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_2537
        db  08Bh, 0FFh
loc_1_24D0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_2520
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_24F6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_24FD
loc_1_24F6:
        mov    dword ptr [ebp - 10h], 1
loc_1_24FD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_2510
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_253A
        db  08Dh, 064h, 024h, 000h
loc_1_2510:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_253A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2520:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_253A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2530:
        mov    dword ptr [ebp - 4], 20h
loc_1_2537:
        mov    esi, dword ptr [ebp + 8]
loc_1_253A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_2631
loc_1_2545:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_259D
        test   al, 2
        je     SHORT loc_1_2589
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 086h, 0F2h, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_2589
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_259D
loc_1_2589:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_2594
        test   bl, 4
        jne    SHORT loc_1_259D
loc_1_2594:
        test   bl, 20h
        je     NEAR PTR loc_1_2626
loc_1_259D:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_227A
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_2626
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_25DF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_25DF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_25F0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_25F6
        db  08Bh, 0FFh
loc_1_25F0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_25F6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_2605
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2280
loc_1_2605:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_2324
        mov    edi, ebx
        jmp    SHORT loc_1_2629
loc_1_2626:
        mov    edi, dword ptr [edi + 10h]
loc_1_2629:
        or     edi, edi
        jne    NEAR PTR loc_1_2545
loc_1_2631:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2640:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2650
        pop    ebp
        ret    4
sub_1_2650:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_26CE
loc_1_2660:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_26C7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2280
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_26C7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_268C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_268C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_26A0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_26A6
        db  005h, 000h, 000h, 000h, 000h
loc_1_26A0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_26A6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_2324
        mov    edi, ebx
        jmp    SHORT loc_1_26CA
loc_1_26C7:
        mov    edi, dword ptr [edi + 10h]
loc_1_26CA:
        or     edi, edi
        jne    SHORT loc_1_2660
loc_1_26CE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_26F5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_26F5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_2731
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_2722
        cmp    bl, 0ffh
        jne    SHORT loc_1_2722
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_2722
        test   al, 8
        je     SHORT loc_1_271B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_271B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_2722:
        test   al, 4
        jne    SHORT loc_1_272A
        test   al, 2
        je     SHORT loc_1_2749
loc_1_272A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2731:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_2749
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_2749
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2749:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 045h, 050h, 052h, 04Fh
        db  020h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  002h, 000h, 000h, 090h, 0FFh, 0FFh, 0FFh, 0FFh, 045h, 050h, 052h, 04Fh, 000h, 000h, 000h, 000h
loc_1_27C0:
        inc    ebp
        push   eax
        push   edx
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [edx + edx*2 + 41h], dl
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [edx + edx*2 + 41h], dl
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ecx + 4fh], cl
        inc    ebx
        dec    eax
        push   edx
        inc    esp
        pop    ecx
        add    byte ptr [ecx + 4fh], cl
        inc    ebx
        dec    eax
        push   edx
        inc    esp
        pop    ecx
        add    byte ptr [edx + 75h], al
        db  073h, 04Eh
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        db  073h, 04Eh
        jne    SHORT loc_1_289F
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], dl
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_288D
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2848:
        push   ebx
        outsd
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_289D
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2858:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
loc_1_288D:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
loc_1_289D:
        add    dword ptr [ecx], eax
loc_1_289F:
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0d000201h
        add    al, byte ptr [edx]
        add    byte ptr [esi], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    al, 2
        add    al, byte ptr [ecx]
        add    eax, 6010202h
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_2958:
        dec    ecx
        outsb
        db  074h, 065h
        insb
        and    byte ptr [ebp + 74h], al
        push   78457265h
        db  070h, 072h
        db  065h, 073h, 073h
        and    byte ptr [eax + 52h], dl
        dec    edi
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2977
loc_1_2977:
        add    byte ptr [esi], dh
        db  030h, 00Dh, 00Ah
loc_1_297C:
        or     dword ptr [eax + 61h], edx
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
loc_1_2984:
        push   esp
        db  072h, 061h
        outsb
loc_1_2988:
        db  073h, 066h
        db  065h, 072h
_LTEXT ends
        end
