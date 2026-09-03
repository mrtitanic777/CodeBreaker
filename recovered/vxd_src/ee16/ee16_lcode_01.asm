; ee16_lcode_01.asm -- EE16 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0fh
        mov    cx, word ptr [esi]
        in     al, dx
        lea    edx, [ecx + 4]
        mov    ax, word ptr [ebp + 10h]
        out    dx, ax
        mov    bx, word ptr [ebp + 14h]
        mov    ax, bx
        shr    ax, 1
        and    eax, 0ffffh
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        call   sub_1_0FA1
        test   bl, 1
        je     SHORT loc_1_0056
        mov    dx, word ptr [esi]
        in     al, dx
        mov    ecx, ebx
        and    ecx, 0ffffh
        mov    byte ptr [edi + ecx - 1], al
loc_1_0056:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_1_0060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0fh
        mov    cx, word ptr [esi]
        in     al, dx
        lea    edx, [ecx + 2]
        mov    ax, word ptr [ebp + 10h]
        out    dx, ax
        mov    bx, word ptr [ebp + 14h]
        mov    ax, bx
        shr    ax, 1
        and    eax, 0ffffh
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        call   sub_1_0F64
        test   bl, 1
        je     SHORT loc_1_00B5
        mov    eax, ebx
        and    eax, 0ffffh
        mov    dx, word ptr [esi]
        mov    al, byte ptr [eax + edi - 1]
        out    dx, al
loc_1_00B5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 143h]
        mov    eax, ebx
        add    eax, 24h
        mov    dword ptr [esi + 14bh], ebx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_00EC
        mov    dword ptr [esi + 147h], 0
loc_1_00EC:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 143h], eax
        lea    edi, [ebx + 24h]
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        push   ebx
        call   sub_1_10E0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1104
        xor    edx, edx
        mov    dx, word ptr [esi + 1edh]
        cmp    dword ptr [ebp - 8], edx
        jae    SHORT loc_1_0156
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        je     SHORT loc_1_0156
        mov    ecx, eax
        cmp    ecx, edx
        jb     SHORT loc_1_0138
        mov    ecx, edx
loc_1_0138:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [esi + 34h]
        push   eax
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        push   0
        push   ebx
        call   sub_1_01B0
        lea    eax, [esi + 34h]
        mov    dword ptr [ebp - 4], eax
loc_1_0156:
        mov    edi, dword ptr [edi + 8]
        mov    byte ptr [esi + 134h], 1
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0eh
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 1e9h]
        push   eax
        call   sub_1_0C60
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0FFA
        dec    dword ptr [edi + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_027A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10E0
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_027A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_0275
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_0200:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_022C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1128
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_0275
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        jmp    SHORT loc_1_0270
loc_1_022C:
        or     edi, edi
        je     SHORT loc_1_0249
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_0241
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_0270
loc_1_0241:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_0249:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_0257
        mov    ebx, eax
loc_1_0257:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    esi, ebx
        push   eax
        call   sub_1_119C
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_0270:
        cmp    esi, dword ptr [ebp + 10h]
        jb     SHORT loc_1_0200
loc_1_0275:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_027A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_03B9
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_10E0
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_03B9
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 25h]
        mov    cx, word ptr [esi]
        add    cx, 0fh
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 14h], al
        mov    dx, word ptr [esi]
        add    dx, 4
        mov    eax, dword ptr [ebp + 18h]
        add    ax, word ptr [ebp + 0ch]
        out    dx, ax
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_03B1
loc_1_0314:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0344
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1128
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_03B1
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        jmp    SHORT loc_1_03A5
loc_1_0344:
        or     edi, edi
        je     SHORT loc_1_03B1
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_0353
        cmp    dword ptr [ebp + 18h], edi
        ja     SHORT loc_1_03B1
loc_1_0353:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        cmp    edi, dword ptr [ebp - 4]
        mov    ebx, edi
        jbe    SHORT loc_1_0366
        mov    ebx, dword ptr [ebp - 4]
loc_1_0366:
        cmp    dword ptr [ebp - 14h], ebx
        jae    SHORT loc_1_036E
        mov    ebx, dword ptr [ebp - 14h]
loc_1_036E:
        test   ebx, 0fffffffeh
        je     SHORT loc_1_038A
        mov    eax, ebx
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0FA1
loc_1_038A:
        test   bl, 1
        je     SHORT loc_1_039A
        mov    dx, word ptr [esi]
        in     al, dx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + ecx - 1], al
loc_1_039A:
        add    dword ptr [ebp - 0ch], ebx
        add    dword ptr [ebp - 10h], ebx
        sub    edi, ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_03A5:
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 10h], ecx
        ja     NEAR PTR loc_1_0314
loc_1_03B1:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [eax], ecx
loc_1_03B9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax]
        add    dx, 7
        mov    al, byte ptr [eax + 8]
        out    dx, al
        mov    al, 1
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        cmp    byte ptr [edi + 154h], 0
        jne    NEAR PTR loc_1_04F1
        inc    dword ptr [edi + 137h]
        sub    ebx, ebx
loc_1_0411:
        mov    dx, word ptr [edi]
        sub    dx, 3ff8h
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    word ptr [ebp - 2], 0f000h
        jne    SHORT loc_1_0433
        cmp    dword ptr [edi + 143h], ebx
        je     NEAR PTR loc_1_04C2
loc_1_0433:
        mov    si, word ptr [ebp - 2]
        and    si, 5000h
        cmp    si, bx
        je     SHORT loc_1_0473
        push   edi
        call   sub_1_0710
        cmp    al, bl
        je     SHORT loc_1_0473
        mov    cx, word ptr [edi]
        mov    ax, si
        lea    edx, [ecx - 3ff6h]
        and    word ptr [ebp - 2], 0afffh
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   edi
        call   sub_1_0500
        cmp    al, bl
        je     NEAR PTR loc_1_04F1
loc_1_0473:
        mov    si, word ptr [ebp - 2]
        and    si, 0a000h
        cmp    si, bx
        je     SHORT loc_1_04AB
        push   edi
        call   sub_1_0710
        cmp    al, bl
        je     SHORT loc_1_04AB
        mov    cx, word ptr [edi]
        mov    ax, si
        lea    edx, [ecx - 3ff6h]
        and    word ptr [ebp - 2], 5fffh
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   edi
        call   sub_1_0A00
loc_1_04AB:
        cmp    dword ptr [edi + 143h], ebx
        je     NEAR PTR loc_1_0411
        push   edi
        call   sub_1_00C0
        jmp    NEAR PTR loc_1_0411
loc_1_04C2:
        mov    dx, word ptr [edi]
        dec    dword ptr [edi + 137h]
        add    dx, 7
        mov    al, byte ptr [edi + 8]
        or     al, 8
        out    dx, al
        cmp    byte ptr [edi + 134h], 0
        je     SHORT loc_1_04F1
        mov    byte ptr [edi + 134h], 0
        mov    eax, dword ptr [edi + 1e9h]
        push   eax
        call   sub_1_0E40
loc_1_04F1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0500:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0a000h
loc_1_0511:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 1dh]
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 400ah]
        in     ax, dx
        test   di, ax
        je     NEAR PTR loc_1_0660
        mov    cx, word ptr [esi]
        sub    ax, ax
        lea    edx, [ecx + 400ah]
        out    dx, ax
        lea    edx, [ecx + 400ch]
        mov    ax, 8000h
        out    dx, ax
        lea    edx, [ecx + 4000h]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    ax, 3fffh
        add    ax, 0eh
        mov    word ptr [ebp - 2], ax
        inc    dword ptr [esi + 162h]
        cmp    word ptr [ebp - 2], 3ch
        jb     NEAR PTR loc_1_0622
        cmp    word ptr [ebp - 2], 5eah
        ja     NEAR PTR loc_1_0622
        mov    bx, word ptr [esi + 1edh]
        cmp    word ptr [ebp - 2], bx
        ja     SHORT loc_1_058E
        mov    bx, word ptr [ebp - 2]
loc_1_058E:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 1dh]
        lea    edx, [ecx + 4]
        add    ax, 12h
        out    dx, ax
        mov    ax, bx
        shr    ax, 1
        and    eax, 0ffffh
        push   eax
        lea    eax, [esi + 34h]
        push   eax
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        call   sub_1_0FA1
        test   bl, 1
        je     SHORT loc_1_05D0
        mov    dx, word ptr [esi]
        in     al, dx
        mov    ecx, ebx
        and    ecx, 0ffffh
        mov    byte ptr [esi + ecx + 33h], al
loc_1_05D0:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 25h], ax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        sub    eax, 0eh
        push   eax
        mov    eax, ebx
        and    eax, 0ffffh
        sub    eax, 0eh
        push   eax
        lea    eax, [esi + 42h]
        push   eax
        push   0eh
        lea    eax, [esi + 34h]
        push   eax
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 1dh]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [esi + 1e9h]
        push   eax
        call   sub_1_0C60
        cmp    byte ptr [esi + 154h], 0
        jne    NEAR PTR loc_1_06AC
        mov    byte ptr [esi + 134h], 1
loc_1_0622:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 1fh]
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 400ch]
        sub    ax, ax
        out    dx, ax
        mov    ax, word ptr [esi + 1dh]
        mov    word ptr [esi + 1fh], ax
        add    ax, 600h
        cmp    word ptr [esi + 23h], ax
        mov    word ptr [esi + 1dh], ax
        jae    NEAR PTR loc_1_0511
        mov    ax, word ptr [esi + 21h]
        mov    word ptr [esi + 1dh], ax
        jmp    NEAR PTR loc_1_0511
loc_1_0660:
        mov    dx, word ptr [esi]
        sub    dx, 3ff8h
        in     ax, dx
        test   al, 40h
        jne    SHORT loc_1_06A1
        mov    dx, word ptr [esi]
        sub    dx, 3ff2h
        mov    ax, word ptr [esi + 1dh]
        add    ax, 0ah
        out    dx, ax
        push   esi
        call   sub_1_0710
        mov    cx, word ptr [esi]
        mov    ax, 10h
        lea    edx, [ecx - 3ff6h]
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   esi
        call   sub_1_0710
loc_1_06A1:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_06AC:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    word ptr [esi + 1bh], 0
        je     SHORT loc_1_06EC
        mov    ax, word ptr [esi + 1efh]
        dec    ax
        mov    word ptr [esi + 1efh], ax
        jne    SHORT loc_1_06EC
        push   esi
        db  0E8h, 05Ah, 007h, 0FFh, 0FFh
        db  0FFh, 005h, 0DCh, 01Ch, 000h, 000h
loc_1_06EC:
        push   7d0h
        add    esi, 1ceh
        push   esi
        call   sub_1_1092
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0710:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
loc_1_0721:
        mov    dx, word ptr [esi]
        sub    dx, 3ff6h
        in     ax, dx
        cmp    ax, bx
        je     SHORT loc_1_074D
        push   14h
        call   sub_1_10B0
        mov    ax, di
        inc    di
        cmp    ax, 1388h
        jne    SHORT loc_1_0721
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_074D:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0760:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [edi + 154h], 0
        jne    NEAR PTR loc_1_0834
        mov    ax, word ptr [edi + 0fh]
        cmp    word ptr [edi + 1bh], ax
        jae    SHORT loc_1_07F0
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_07E0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        push   esi
        push   edi
        call   sub_1_0840
        cmp    eax, 103h
        jne    SHORT loc_1_07CD
        mov    dword ptr [ebx + 2ch], esi
        lea    ecx, [ebx + 24h]
        inc    dword ptr [esi + 14h]
        cmp    dword ptr [edi + 143h], 0
        jne    SHORT loc_1_07B5
        mov    dword ptr [edi + 143h], ebx
        jmp    SHORT loc_1_07BE
loc_1_07B5:
        mov    edx, dword ptr [edi + 147h]
        mov    dword ptr [edx + 24h], ebx
loc_1_07BE:
        mov    dword ptr [ecx], 0
        mov    dword ptr [edi + 147h], ebx
        dec    dword ptr [esi + 14h]
loc_1_07CD:
        mov    word ptr [edi + 1efh], 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_07E0:
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_07F0:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 2ch], esi
        lea    eax, [ebx + 24h]
        inc    dword ptr [esi + 14h]
        cmp    dword ptr [edi + 13bh], 0
        jne    SHORT loc_1_0810
        mov    dword ptr [edi + 13bh], ebx
        jmp    SHORT loc_1_0819
        db  08Dh, 049h, 000h
loc_1_0810:
        mov    ecx, dword ptr [edi + 13fh]
        mov    dword ptr [ecx + 24h], ebx
loc_1_0819:
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 13fh], ebx
        inc    dword ptr [esi + 14h]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_0834:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
sub_1_0840:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_10E0
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_09E8
        cmp    dword ptr [ebp - 10h], 5eah
        ja     NEAR PTR loc_1_09E8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1104
        mov    esi, dword ptr [ebp + 8]
        push   3
        mov    di, word ptr [esi]
        mov    cx, word ptr [esi + 15h]
        lea    edx, [edi + 2]
        add    cx, 16h
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        call   sub_1_0F64
        push   6
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_119C
        mov    di, 0ch
        sub    dword ptr [ebp - 4], 0ch
        mov    bx, word ptr [ebp - 14h]
        add    dword ptr [ebp - 8], 0ch
        or     bx, bx
        je     SHORT loc_1_0939
loc_1_08D5:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0911
        test   dword ptr [ebp - 4], 0fffffffeh
        je     SHORT loc_1_08F9
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0F64
loc_1_08F9:
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_1_090D
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        mov    al, byte ptr [eax + ecx - 1]
        out    dx, al
loc_1_090D:
        add    di, word ptr [ebp - 4]
loc_1_0911:
        cmp    bx, 1
        jbe    SHORT loc_1_0935
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1128
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1104
loc_1_0935:
        dec    bx
        jne    SHORT loc_1_08D5
loc_1_0939:
        cmp    di, 3ch
        jae    SHORT loc_1_0956
        mov    cx, 3ch
        sub    cx, di
        je     SHORT loc_1_0952
        mov    dx, word ptr [esi]
loc_1_094B:
        sub    al, al
        out    dx, al
        dec    cx
        jne    SHORT loc_1_094B
loc_1_0952:
        mov    di, 3ch
loc_1_0956:
        mov    cx, word ptr [esi + 15h]
        lea    eax, [ecx + 600h]
        cmp    word ptr [esi + 19h], ax
        mov    word ptr [esi + 15h], ax
        jae    SHORT loc_1_0972
        mov    ax, word ptr [esi + 17h]
        mov    word ptr [esi + 15h], ax
loc_1_0972:
        mov    bx, word ptr [esi]
        mov    ax, word ptr [esi + 15h]
        lea    edx, [ebx + 8]
        out    dx, ax
        lea    edx, [ebx + 4004h]
        out    dx, ax
        lea    edx, [ebx + 8]
        mov    ax, cx
        out    dx, ax
        lea    edx, [ebx + 4006h]
        lea    eax, [edi - 0eh]
        inc    word ptr [esi + 1bh]
        or     ax, 8000h
        inc    dword ptr [esi + 15eh]
        out    dx, ax
        lea    eax, [ecx + 0eh]
        lea    edx, [ebx + 4004h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 43h], 2
        jne    SHORT loc_1_09DD
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [esi + 1e9h]
        push   eax
        call   sub_1_0EE0
        or     al, al
        je     SHORT loc_1_09DD
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_09DD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_09E8:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    cl, cl
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 5
        cmp    word ptr [esi + 1bh], 0
        je     NEAR PTR loc_1_0B1E
        nop
loc_1_0A20:
        mov    ax, word ptr [ebp - 2]
        dec    word ptr [ebp - 2]
        or     ax, ax
        je     NEAR PTR loc_1_0B1E
        mov    di, word ptr [esi]
        mov    ax, word ptr [esi + 13h]
        lea    edx, [edi + 8]
        out    dx, ax
        lea    edx, [edi + 400eh]
        in     ax, dx
        test   ah, 0a0h
        je     NEAR PTR loc_1_0B13
        mov    dx, word ptr [esi]
        dec    word ptr [esi + 1bh]
        add    dx, 400eh
        sub    ax, ax
        out    dx, ax
        mov    word ptr [esi + 1efh], 3
        mov    ax, word ptr [esi + 13h]
        add    ax, 600h
        cmp    word ptr [esi + 19h], ax
        mov    word ptr [esi + 13h], ax
        jae    SHORT loc_1_0A82
        mov    ax, word ptr [esi + 17h]
        mov    word ptr [esi + 13h], ax
loc_1_0A82:
        mov    ebx, dword ptr [esi + 13bh]
        or     ebx, ebx
        je     NEAR PTR loc_1_0B11
        lea    edi, [ebx + 24h]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_0AA2
        mov    dword ptr [esi + 13fh], 0
loc_1_0AA2:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 13bh], eax
        lea    eax, [edi + 8]
        mov    dword ptr [ebp - 8], eax
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        push   esi
        call   sub_1_0840
        cmp    eax, 103h
        mov    eax, dword ptr [ebp - 8]
        jne    SHORT loc_1_0AF7
        mov    eax, dword ptr [eax]
        inc    dword ptr [eax + 14h]
        cmp    dword ptr [esi + 143h], 0
        jne    SHORT loc_1_0AE0
        mov    dword ptr [esi + 143h], ebx
        jmp    SHORT loc_1_0AE9
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0AE0:
        mov    eax, dword ptr [esi + 147h]
        mov    dword ptr [eax + 24h], ebx
loc_1_0AE9:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 147h], ebx
        jmp    SHORT loc_1_0B08
loc_1_0AF7:
        mov    edi, dword ptr [eax]
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0FFA
        dec    dword ptr [edi + 14h]
loc_1_0B08:
        mov    word ptr [esi + 1efh], 3
loc_1_0B11:
        mov    cl, 1
loc_1_0B13:
        cmp    word ptr [esi + 1bh], 0
        jne    NEAR PTR loc_1_0A20
loc_1_0B1E:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 8]
        add    edi, 0eh
        cmp    byte ptr [ecx + 154h], 0
        jne    SHORT loc_1_0BA5
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_0B9A
        inc    dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 0ch]
        or     edx, edx
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        push   edi
        jne    SHORT loc_1_0B80
        mov    eax, dword ptr [ecx + 14bh]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_112E
        dec    dword ptr [esi + 14h]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_0B80:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0290
        dec    dword ptr [esi + 14h]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_0B9A:
        mov    eax, 0c001000ch
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_0BA5:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_0BB0:
        db  0A0h, 048h, 012h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0BD5
        db  0FEh, 005h, 048h, 012h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0BD7
        push   0
        push   0
        db  0E8h, 030h, 0F4h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0BD7
loc_1_0BD5:
        clc
        ret
loc_1_0BD7:
        stc
        ret
EE16_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_0BE8
        db  0FFh, 024h, 085h, 0EAh, 00Bh, 000h, 000h
loc_1_0BE8:
        clc
        ret
        db  0B0h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 00Bh, 000h, 000h, 0E8h, 00Bh, 000h, 000h, 0B0h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_0C60:
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
        call   sub_1_0C90
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_0D30
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_0CD0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_0CC5
        cmp    al, 0ffh
        jne    SHORT loc_1_0CC5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_0CC5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_0D37
loc_1_0CC5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_0D37
        db  08Bh, 0FFh
loc_1_0CD0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_0D20
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0CF6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_0CFD
loc_1_0CF6:
        mov    dword ptr [ebp - 10h], 1
loc_1_0CFD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0D10
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_0D3A
        db  08Dh, 064h, 024h, 000h
loc_1_0D10:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_0D3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D20:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_0D3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D30:
        mov    dword ptr [ebp - 4], 20h
loc_1_0D37:
        mov    esi, dword ptr [ebp + 8]
loc_1_0D3A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_0E31
loc_1_0D45:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_0D9D
        test   al, 2
        je     SHORT loc_1_0D89
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0A6h, 01Ah, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_0D89
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_0D9D
loc_1_0D89:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_0D94
        test   bl, 4
        jne    SHORT loc_1_0D9D
loc_1_0D94:
        test   bl, 20h
        je     NEAR PTR loc_1_0E26
loc_1_0D9D:
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
        call   sub_1_1012
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0E26
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_0DDF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_0DDF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0DF0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0DF6
        db  08Bh, 0FFh
loc_1_0DF0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0DF6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0E05
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1018
loc_1_0E05:
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
        call   sub_1_10BC
        mov    edi, ebx
        jmp    SHORT loc_1_0E29
loc_1_0E26:
        mov    edi, dword ptr [edi + 10h]
loc_1_0E29:
        or     edi, edi
        jne    NEAR PTR loc_1_0D45
loc_1_0E31:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0E40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0E50
        pop    ebp
        ret    4
sub_1_0E50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_0ECE
loc_1_0E60:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0EC7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1018
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0EC7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_0E8C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_0E8C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0EA0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0EA6
        db  005h, 000h, 000h, 000h, 000h
loc_1_0EA0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0EA6:
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
        call   sub_1_10BC
        mov    edi, ebx
        jmp    SHORT loc_1_0ECA
loc_1_0EC7:
        mov    edi, dword ptr [edi + 10h]
loc_1_0ECA:
        or     edi, edi
        jne    SHORT loc_1_0E60
loc_1_0ECE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0EE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_0EF5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0EF5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_0F31
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_0F22
        cmp    bl, 0ffh
        jne    SHORT loc_1_0F22
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_0F22
        test   al, 8
        je     SHORT loc_1_0F1B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0F1B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0F22:
        test   al, 4
        jne    SHORT loc_1_0F2A
        test   al, 2
        je     SHORT loc_1_0F49
loc_1_0F2A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0F31:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0F49
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_0F49
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0F49:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_0F64:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsw
        pop    esi
        ret    0ch
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h
sub_1_0FA1:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insw
        pop    edi
        ret    0ch
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_0FCA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0FD0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0FD6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_0FDC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0FE2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0FE8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0FF4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_0FFA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_1012:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_1018:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_101E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_1024:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_1030:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_103C:
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
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_1092:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_1098:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_109E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_10A4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_10B0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_10B6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_10BC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_10E0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_1104:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_1128:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_112E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_1134:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_113A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_1140:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
sub_1_1146:
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
sub_1_1188:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_119C:
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
sub_1_11BC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_11DC
        or     ecx, ecx
        je     SHORT loc_1_11F2
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_11F0
loc_1_11DC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_11F0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_11F0:
        rep movsb
loc_1_11F2:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 045h, 045h, 031h, 036h
        db  020h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 0D9h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1250:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 8]
        cmp    byte ptr [edx + 154h], 0
        jne    SHORT loc_1_12D0
        cmp    byte ptr [ecx + 18h], 0
        jne    SHORT loc_1_12C2
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_1282
        cmp    eax, 1
        je     SHORT loc_1_12A2
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    8
loc_1_1282:
        lea    eax, [esi + 24h]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   ecx
        push   edx
        call   sub_1_14F0
        pop    esi
        pop    ebp
        ret    8
loc_1_12A2:
        lea    eax, [esi + 24h]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   ecx
        push   edx
        call   sub_1_1320
        pop    esi
        pop    ebp
        ret    8
loc_1_12C2:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_12D0:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_12E0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 10h], 2
        je     SHORT loc_1_12F5
        mov    eax, 0c00000bbh
        pop    ebp
        ret    8
loc_1_12F5:
        lea    eax, [ecx + 24h]
        push   eax
        lea    eax, [ecx + 20h]
        push   eax
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        mov    eax, dword ptr [ecx + 18h]
        push   eax
        mov    eax, dword ptr [ecx + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14F0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1320:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010eh
        mov    dword ptr [esi], 0
        je     SHORT loc_1_1367
        cmp    eax, 1010fh
        je     SHORT loc_1_13C0
        cmp    eax, 10112h
        je     NEAR PTR loc_1_1447
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_1492
        mov    esi, 0c0010017h
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1367:
        mov    edi, dword ptr [ebp + 18h]
        cmp    edi, 4
        je     SHORT loc_1_1382
        mov    dword ptr [esi], 4
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1382:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_119C
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 1e9h]
        push   eax
        db  0E8h, 081h, 012h, 0FFh, 0FFh
        mov    esi, eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_13C0:
        sub    esi, esi
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_13E0
        mov    eax, dword ptr [ebp + 1ch]
        mov    esi, 0c0010014h
        mov    dword ptr [eax], 0
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_13E0:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_119C
        cmp    dword ptr [ebp - 4], 100h
        ja     SHORT loc_1_1438
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 1edh]
        cmp    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_1427
        mov    word ptr [ecx + 1edh], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ecx + 41h], ax
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1427:
        mov    ecx, dword ptr [ebp + 0ch]
        pop    edi
        mov    word ptr [ecx + 41h], ax
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1438:
        mov    esi, 0c0000001h
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1447:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_146B
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [esi], 4
        mov    dword ptr [eax], 0
        mov    esi, 0c0010014h
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_146B:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 43h
        push   ecx
        call   sub_1_119C
        mov    eax, dword ptr [ebp + 1ch]
        sub    esi, esi
        mov    dword ptr [eax], 4
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1492:
        mov    ecx, 6
        mov    edi, dword ptr [ebp + 18h]
        mov    eax, edi
        cdq
        idiv   ecx
        or     edx, edx
        je     SHORT loc_1_14B2
        mov    dword ptr [esi], ecx
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_14B2:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, edi
        cdq
        idiv   ecx
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 1e9h]
        push   eax
        db  0E8h, 075h, 00Eh, 0FFh, 0FFh
        mov    esi, eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h
sub_1_14F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 8], 0
        lea    esi, [ebp - 8]
        mov    word ptr [ebp - 2], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    ecx, 4
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    eax, edx
        cmp    eax, 1010101h
        ja     SHORT loc_1_1553
        je     NEAR PTR loc_1_18F9
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_1574
        shl    eax, 2
        db  0FFh, 0A0h, 024h, 01Ah, 000h, 000h
loc_1_1553:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_1920
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_1945
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_19D7
loc_1_1574:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_1586
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_19DE
loc_1_1586:
        cmp    edx, 1020102h
        ja     SHORT loc_1_1600
        je     NEAR PTR loc_1_1723
        cmp    edx, 20105h
        ja     SHORT loc_1_15D7
        je     NEAR PTR loc_1_16E4
        cmp    edx, 20101h
        je     NEAR PTR loc_1_1664
        cmp    edx, 20102h
        je     NEAR PTR loc_1_1675
        cmp    edx, 20103h
        je     NEAR PTR loc_1_1686
        cmp    edx, 20104h
        je     NEAR PTR loc_1_16A9
        jmp    NEAR PTR loc_1_1658
loc_1_15D7:
        cmp    edx, 2020dh
        je     NEAR PTR loc_1_16F4
        cmp    edx, 2020eh
        je     NEAR PTR loc_1_1704
        cmp    edx, 1020101h
        je     NEAR PTR loc_1_1715
        jmp    SHORT loc_1_1658
        db  08Dh, 049h, 000h
loc_1_1600:
        cmp    edx, 1020203h
        ja     SHORT loc_1_1634
        je     NEAR PTR loc_1_1767
        cmp    edx, 1020103h
        je     NEAR PTR loc_1_1734
        cmp    edx, 1020201h
        je     NEAR PTR loc_1_1745
        cmp    edx, 1020202h
        je     NEAR PTR loc_1_1756
        jmp    SHORT loc_1_1658
loc_1_1634:
        cmp    edx, 1020204h
        je     NEAR PTR loc_1_1775
        cmp    edx, 1020205h
        je     NEAR PTR loc_1_1786
        cmp    edx, 1020206h
        je     NEAR PTR loc_1_1797
loc_1_1658:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_19DE
loc_1_1664:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 15eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1675:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 162h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1686:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16ah]
        add    eax, dword ptr [edi + 16eh]
        add    eax, dword ptr [edi + 172h]
        add    eax, dword ptr [edi + 176h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_16A9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 2ch]
        mov    edx, dword ptr [edi + 186h]
        add    edx, dword ptr [edi + 18ah]
        and    eax, 0ffffh
        add    edx, eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2ah]
        add    edx, eax
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        add    edx, eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 26h]
        add    edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_19DE
loc_1_16E4:
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_16F4:
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1704:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 166h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1715:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1723:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1734:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 182h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1745:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1756:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1767:
        xor    eax, eax
        mov    ax, word ptr [ebp - 26h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1775:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 172h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1786:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 176h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1797:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_17A8:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_17C0
        db  0BEh, 0E0h, 01Ch, 000h, 000h
        mov    ecx, 9ch
        jmp    NEAR PTR loc_1_19DE
        db  08Dh, 049h, 000h
loc_1_17C0:
        db  0BEh, 07Ch, 01Dh, 000h, 000h
        mov    ecx, 58h
        jmp    NEAR PTR loc_1_19DE
        db  090h
loc_1_17D0:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_17E0:
        lea    esi, [ebp - 18h]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_19DE
        db  08Dh, 049h, 000h
loc_1_17F0:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_1800:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_1810:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_1820:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0fh]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1841:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 11h]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1862:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_19DE
        db  08Bh, 0FFh
loc_1_1870:
        db  0BEh, 0D4h, 01Dh, 000h, 000h
        mov    ecx, 16h
        jmp    NEAR PTR loc_1_19DE
        db  090h
loc_1_1880:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_1895
        mov    eax, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_1895:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 1e9h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_18A9:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_18C4
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 1edh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_18C4:
        xor    eax, eax
        mov    ax, word ptr [ebx + 41h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19DE
loc_1_18D2:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    ecx, 2
        jmp    NEAR PTR loc_1_19DE
loc_1_18E5:
        mov    dword ptr [ebp - 8], 0eh
        lea    esi, [ebp - 8]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_19DE
loc_1_18F9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 28h]
        add    edi, 28h
        mov    dword ptr [ebp - 24h], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 20h], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 24h]
        mov    byte ptr [ebp - 1fh], al
        mov    ecx, 6
        jmp    NEAR PTR loc_1_19DE
        db  08Bh, 0FFh
loc_1_1920:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 2eh]
        add    edi, 2eh
        mov    dword ptr [ebp - 24h], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 20h], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 24h]
        mov    byte ptr [ebp - 1fh], al
        mov    ecx, 6
        jmp    NEAR PTR loc_1_19DE
loc_1_1945:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_1980
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 1e9h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 045h, 00Eh, 0FFh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    esi, dword ptr [ebp + 14h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    NEAR PTR loc_1_19DE
loc_1_1980:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 1e9h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 04Dh, 00Dh, 0FFh, 0FFh
        cmp    dword ptr [ebp - 14h], 0
        mov    esi, dword ptr [ebp + 14h]
        jne    SHORT loc_1_19BB
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_19DE
loc_1_19BB:
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 1e9h]
        push   eax
        db  0E8h, 0E5h, 00Ch, 0FFh, 0FFh
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_19DE
loc_1_19D7:
        mov    dword ptr [ebp - 8], 10h
loc_1_19DE:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1A15
        cmp    dword ptr [ebp + 18h], ecx
        jae    SHORT loc_1_1A01
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010016h
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1A01:
        mov    eax, dword ptr [ebp + 1ch]
        or     ecx, ecx
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_1A15
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_119C
loc_1_1A15:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 049h, 000h
loc_1_1A24:
        test   al, 17h
        add    byte ptr [eax], al
        rcl    byte ptr [edi]
        add    byte ptr [eax], al
        loopne loc_1_1A45
        add    byte ptr [eax], al
        loopne loc_1_1A49
        add    byte ptr [eax], al
        db  0F0h, 017h, 000h, 000h, 000h, 018h, 000h, 000h, 010h, 018h, 000h, 000h, 020h, 018h, 000h, 000h
        db  041h
loc_1_1A45:
        sbb    byte ptr [eax], al
        db  000h, 062h
loc_1_1A49:
        sbb    byte ptr [eax], al
        db  000h, 062h, 018h
        add    byte ptr [eax], al
        db  074h, 015h
        add    byte ptr [eax], al
        jo     SHORT loc_1_1A6E
        add    byte ptr [eax], al
        sbb    byte ptr [eax], 0
        db  000h, 0A9h, 018h, 000h, 000h, 0D2h
        sbb    byte ptr [eax], al
        db  000h, 062h, 018h
        add    byte ptr [eax], al
        db  074h, 015h
        add    byte ptr [eax], al
        in     eax, 18h
loc_1_1A6E:
        add    byte ptr [eax], al
loc_1_1A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 8]
        sub    esi, esi
        cmp    byte ptr [ecx + 154h], 0
        je     SHORT loc_1_1A91
        mov    esi, 0c001000dh
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    14h
loc_1_1A91:
        mov    dl, byte ptr [ecx + 1f9h]
        mov    eax, dword ptr [ebp + 0ch]
        test   al, 24h
        je     SHORT loc_1_1AAA
        mov    al, dl
        or     al, 1
        mov    byte ptr [ecx + 1f9h], al
        jmp    SHORT loc_1_1ABF
loc_1_1AAA:
        test   al, 8
        je     SHORT loc_1_1AB8
        mov    al, dl
        and    al, 0fdh
        mov    byte ptr [ecx + 1f9h], al
loc_1_1AB8:
        and    byte ptr [ecx + 1f9h], 0feh
loc_1_1ABF:
        cmp    byte ptr [ecx + 1f9h], dl
        je     SHORT loc_1_1AD3
        push   ecx
        db  0E8h, 0F3h, 0F8h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_1_1AD3
        sub    esi, esi
loc_1_1AD3:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1AE0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 154h], 0
        je     SHORT loc_1_1B00
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_1B00:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 10h
        jbe    SHORT loc_1_1B11
        mov    eax, 0c000009ah
        pop    ebp
        ret    1ch
loc_1_1B11:
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        push   eax
        push   edx
        db  0E8h, 044h, 0F9h, 0FEh, 0FFh
        sub    eax, eax
        pop    ebp
        ret    1ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [edi + 14h]
        push   esi
        db  0E8h, 05Dh, 0EFh, 0FEh, 0FFh
        push   0
        push   0
        push   edi
        call   sub_1_10BC
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0FF4
        cmp    dword ptr [esi + 18eh], 0
        jne    SHORT loc_1_1B7C
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, byte ptr [esi + 8]
        out    dx, al
loc_1_1B7C:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 002h, 000h
        db  000h, 090h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_1B9C:
        db  070h, 002h
        pushad
        add    dl, byte ptr [eax + 2]
        inc    eax
        add    dh, byte ptr [eax]
        add    ah, byte ptr [eax]
        add    dl, byte ptr [eax]
        add    al, byte ptr [eax]
        add    dh, byte ptr [eax + 3]
        pushad
        add    edx, dword ptr [eax + 3]
        inc    eax
        add    esi, dword ptr [eax]
        add    esp, dword ptr [eax]
        add    edx, dword ptr [eax]
        add    eax, dword ptr [eax]
        db  003h
loc_1_1BBC:
        or     al, 0
        or     dword ptr [eax], eax
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0b000a00h
        add    byte ptr [edi], cl
        add    byte ptr [ebp + 65h], ah
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1BD4:
        xor    dword ptr gs:[esi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        db  073h, 054h
        db  079h, 070h
        add    byte ptr gs:[ecx + 52h], cl
        push   ecx
        add    byte ptr [ecx + 52h], cl
        push   ecx
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
        add    byte ptr [edx + esi*2 + 61h], dl
        outsb
        db  073h, 063h
        imul   esi, dword ptr gs:[esi + 65h], 72540072h
        popad
        outsb
        jae    SHORT loc_1_1C81
        imul   esi, dword ptr gs:[esi + 65h], 4f490072h
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
        add    byte ptr [edi + esi*2 + 69h], dl
        jae    SHORT loc_1_1CAD
        sub    eax, 72696150h
        and    byte ptr [eax], ch
        push   esp
        push   eax
        inc    ebp
        sub    dword ptr [eax], eax
        add    byte ptr [edi + esi*2 + 69h], dl
        db  073h, 074h
        sub    eax, 72696150h
        and    byte ptr [eax], ch
        push   esp
        push   eax
        inc    ebp
        sub    dword ptr [eax], eax
        add    byte ptr [eax + ebp*2 + 69h], dl
        arpl   word ptr [ebx + 20h], bp
        dec    esi
        db  065h, 074h, 020h
        sub    byte ptr [ecx + 55h], al
        dec    ecx
        das
        inc    esp
        dec    ecx
        pop    eax
        sub    dword ptr [eax], eax
loc_1_1C70:
        push   esp
        push   206b6369h
        dec    esi
        db  065h, 074h, 020h
        sub    byte ptr [ecx + 55h], al
        dec    ecx
        das
        inc    esp
        dec    ecx
loc_1_1C81:
        pop    eax
        sub    dword ptr [eax], eax
        push   esp
        push   4e206e69h
        je     NEAR PTR loc_1_1CAD
        sub    byte ptr [edx + 4eh], al
        inc    ebx
        das
        inc    ebx
        dec    edi
        inc    ecx
        pop    eax
        sub    dword ptr [eax], eax
loc_1_1C98:
        push   esp
        push   4e206e69h
        db  065h, 074h, 020h
        sub    byte ptr [edx + 4eh], al
        inc    ebx
        das
        inc    ebx
        dec    edi
        inc    ecx
        pop    eax
        sub    dword ptr [eax], eax
        inc    ebp
loc_1_1CAD:
        popad
        db  072h, 06Ch
        jns    SHORT loc_1_1CB2
loc_1_1CB2:
        add    byte ptr [eax], al
loc_1_1CB4:
        inc    ebp
        popad
        db  072h, 06Ch
        jns    SHORT loc_1_1CBA
loc_1_1CBA:
        add    byte ptr [eax], al
        dec    esp
        popad
        je     SHORT loc_1_1D25
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1CC4:
        dec    esp
        popad
        je     SHORT loc_1_1D2D
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        db  065h, 076h, 065h
        jb     SHORT loc_1_1CD2
loc_1_1CD2:
        add    byte ptr [eax], al
loc_1_1CD4:
        dec    esi
        jbe    NEAR PTR loc_1_1D3D
        jb     SHORT loc_1_1CDA
loc_1_1CDA:
        add    byte ptr [eax], al
loc_1_1CDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1CE0:
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
        add    dword ptr [ecx], eax
        add    byte ptr [10000101h], cl
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
loc_1_1D25:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
loc_1_1D2D:
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0d000201h
loc_1_1D3D:
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
        add    byte ptr [10000101h], cl
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
loc_1_1DD4:
        dec    ecx
        outsb
        db  074h, 065h
        insb
        and    byte ptr [ebp + 74h], al
        push   78457265h
        db  070h, 072h
        db  065h, 073h, 073h
        and    byte ptr [ecx], dh
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    edx, dword ptr [eax]
        add    esp, dword ptr [eax]
        add    esi, dword ptr [eax]
        add    eax, dword ptr [eax + 3]
        push   eax
        add    esp, dword ptr [eax + 3]
        db  070h, 003h
        add    byte ptr [edx], al
        adc    byte ptr [edx], al
        and    byte ptr [edx], al
        xor    byte ptr [edx], al
        inc    eax
        add    dl, byte ptr [eax + 2]
        pushad
        add    dh, byte ptr [eax + 2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
