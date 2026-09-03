; spendis_lcode_01.asm -- SPENDIS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        cmp    byte ptr [edi + 153h], 0
        jne    SHORT loc_1_0055
        cmp    byte ptr [ebx + 18h], 0
        jne    SHORT loc_1_0047
        mov    byte ptr [edi + 152h], 1
        push   edi
        call   sub_1_0120
        push   edi
        call   sub_1_0080
        push   ebx
        push   edi
        call   sub_1_0070
        push   esi
        push   edi
        call   sub_1_0180
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0047:
        mov    esi, 0c0010002h
        pop    edi
        mov    eax, esi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0055:
        mov    esi, 0c001000dh
        pop    edi
        mov    eax, esi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0070:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0080:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 142h], 0
        je     SHORT loc_1_00CC
        xor    edi, edi
loc_1_0094:
        mov    edx, dword ptr [esi + 142h]
        lea    eax, [edx + 24h]
        cmp    dword ptr [eax], edi
        jne    SHORT loc_1_00A7
        mov    dword ptr [esi + 146h], edi
loc_1_00A7:
        mov    ecx, dword ptr [eax]
        push   0c001000ch
        push   edx
        mov    dword ptr [esi + 142h], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_2BDA
        dec    dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 142h], edi
        jne    SHORT loc_1_0094
loc_1_00CC:
        cmp    dword ptr [esi + 13ah], 0
        je     SHORT loc_1_010F
        xor    edi, edi
loc_1_00D7:
        mov    edx, dword ptr [esi + 13ah]
        lea    eax, [edx + 24h]
        cmp    dword ptr [eax], edi
        jne    SHORT loc_1_00EA
        mov    dword ptr [esi + 13eh], edi
loc_1_00EA:
        mov    ecx, dword ptr [eax]
        push   0c001000ch
        push   edx
        mov    dword ptr [esi + 13ah], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_2BDA
        dec    dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 13ah], edi
        jne    SHORT loc_1_00D7
loc_1_010F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0120:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 1a1h]
        sub    eax, edi
        cmp    eax, 0fffffe5fh
        je     SHORT loc_1_0167
loc_1_0139:
        push   0
        inc    dword ptr [edi + 14h]
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   40010004h
        push   eax
        call   sub_1_2BFE
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_2C04
        mov    eax, esi
        dec    dword ptr [edi + 14h]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0fffffe5fh
        jne    SHORT loc_1_0139
loc_1_0167:
        mov    byte ptr [esi + 153h], 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0180:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 1a1h]
        sub    eax, edi
        cmp    eax, 0fffffe5fh
        je     SHORT loc_1_01D8
        xor    ebx, ebx
loc_1_019C:
        inc    dword ptr [edi + 14h]
        cmp    dword ptr [ebp + 0ch], ebx
        je     SHORT loc_1_01B4
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   ebx
        push   40010007h
        push   eax
        call   sub_1_2BFE
loc_1_01B4:
        push   4
        lea    eax, [ebp + 0ch]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        push   40010005h
        push   ecx
        call   sub_1_2BFE
        mov    eax, esi
        dec    dword ptr [edi + 14h]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0fffffe5fh
        jne    SHORT loc_1_019C
loc_1_01D8:
        mov    byte ptr [esi + 153h], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_01F0:
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
        lea    esi, [edi + 1ddh]
        add    edx, 3
        out    dx, al
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_2C78
        push   edi
        call   sub_1_07E0
        xor    ecx, ecx
        mov    word ptr [edi + 1eh], cx
        mov    word ptr [edi + 1ch], cx
        mov    word ptr [edi + 14h], 1
        mov    word ptr [edi + 1ah], cx
        xor    edx, edx
        mov    dx, word ptr [edi]
        xor    eax, eax
        mov    word ptr [edi + 12h], cx
        add    edx, 0ah
        out    dx, ax
        mov    word ptr [edi + 16h], cx
        cmp    dword ptr [edi + 142h], ecx
        mov    word ptr [edi + 18h], cx
        je     SHORT loc_1_0260
loc_1_0251:
        push   edi
        call   sub_1_0C70
        cmp    dword ptr [edi + 142h], 0
        jne    SHORT loc_1_0251
loc_1_0260:
        push   edi
        call   sub_1_26E0
        push   0
        push   1eh
        push   edi
        call   sub_1_0740
        test   al, al
        jne    SHORT loc_1_029C
        push   0
        mov    eax, dword ptr [edi + 1c1h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_2D68
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_029C:
        push   edi
        call   sub_1_0460
        mov    bl, al
        push   1388h
        push   esi
        call   sub_1_2C72
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_02C0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        xor    al, al
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        out    dx, al
        push   edx
        mov    eax, dword ptr [esi + 1c1h]
        push   eax
        call   sub_1_0820
        test   al, al
        jne    SHORT loc_1_030B
        push   0
        mov    eax, dword ptr [esi + 1c1h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000004h
        push   eax
        call   sub_1_2D68
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_030B:
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [esi]
        push   0
        add    edx, 3
        push   1eh
        out    dx, al
        push   esi
        call   sub_1_0740
        test   al, al
        jne    SHORT loc_1_034B
        push   0
        mov    eax, dword ptr [esi + 1c1h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_2D68
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_034B:
        mov    dx, word ptr [esi]
        mov    al, 0bh
        out    dx, al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_1_09B0
        push   esi
        call   sub_1_08E0
        cmp    ax, 0ffffh
        jne    SHORT loc_1_038E
        push   0
        mov    eax, dword ptr [esi + 1c1h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_2D68
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_038E:
        test   ax, ax
        je     SHORT loc_1_03A5
        mov    byte ptr [esi + 0dh], 1
        mov    word ptr [esi + 0eh], 1800h
        mov    word ptr [esi + 10h], 6800h
        jmp    SHORT loc_1_03B5
loc_1_03A5:
        mov    byte ptr [esi + 0dh], 0
        mov    word ptr [esi + 0eh], 3000h
        mov    word ptr [esi + 10h], 0d000h
loc_1_03B5:
        lea    edi, [esi + 1c5h]
        push   1
        push   0
        mov    eax, dword ptr [esi + 1eah]
        push   eax
        mov    ecx, dword ptr [esi + 1c1h]
        push   eax
        lea    edx, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edi
        push   edx
        call   sub_1_2C7E
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_042A
        push   esi
        call   sub_1_0460
        test   al, al
        jne    SHORT loc_1_0420
        push   0
        mov    eax, dword ptr [esi + 1c1h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_2D68
        add    esp, 14h
        push   edi
        call   sub_1_2C84
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0420:
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_042A:
        mov    eax, dword ptr [esi + 1eah]
        mov    ecx, dword ptr [esi + 1c1h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000005h
        push   ecx
        call   sub_1_2D68
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0460:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    al, 40h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        push   esi
        out    dx, al
        inc    edx
        xor    edi, edi
        in     al, dx
        or     al, 80h
        out    dx, al
        call   sub_1_0890
        mov    di, word ptr [esi]
        mov    bx, word ptr [esi + 0eh]
        lea    edx, [edi + 8]
        mov    al, bh
        mov    cl, bh
        out    dx, al
        lea    edx, [edi + 9]
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
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
        mov    al, 14h
        mov    ebx, ecx
        out    dx, al
        lea    edx, [edi + 3]
        and    ebx, 0ffffh
        in     al, dx
        cmp    ebx, -1
        je     SHORT loc_1_04FA
        or     al, 10h
        and    al, 3fh
        cmp    cx, 1
        jne    SHORT loc_1_04E4
        and    al, 0fbh
loc_1_04E4:
        cmp    cx, 2
        jne    SHORT loc_1_04EE
        or     al, 4
        or     al, 20h
loc_1_04EE:
        cmp    cx, 3
        jne    SHORT loc_1_04FC
        or     al, 4
        and    al, 0dfh
        jmp    SHORT loc_1_04FC
loc_1_04FA:
        and    al, 2fh
loc_1_04FC:
        out    dx, al
        mov    edx, edi
        xor    al, al
        push   12ch
        xor    edi, edi
        out    dx, al
        mov    al, 1eh
        out    dx, al
        call   sub_1_2C90
        mov    di, word ptr [esi]
        mov    cx, word ptr [esi + 0eh]
        lea    edx, [edi + 4]
        mov    ax, cx
        out    dx, ax
        mov    ax, word ptr [esi + 10h]
        lea    edx, [edi + 6]
        add    ax, cx
        push   esi
        dec    ax
        out    dx, ax
        lea    edx, [edi + 0ah]
        xor    eax, eax
        out    dx, ax
        call   sub_1_0680
        mov    word ptr [esi + 14h], 1
        xor    eax, eax
        mov    word ptr [esi + 1eh], ax
        mov    word ptr [esi + 1ch], ax
        mov    cx, word ptr [esi + 0eh]
        mov    word ptr [esi + 1ah], ax
        push   7
        mov    word ptr [esi + 12h], ax
        push   7
        mov    word ptr [esi + 16h], ax
        push   esi
        mov    word ptr [esi + 18h], ax
        mov    word ptr [esi + 20h], cx
        call   sub_1_0740
        test   al, al
        jne    SHORT loc_1_057A
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_057A:
        push   esi
        call   sub_1_0700
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0590:
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
        call   sub_1_07E0
        test   al, al
        je     NEAR PTR loc_1_0661
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
        je     SHORT loc_1_0623
        mov    edi, dword ptr [ebp + 10h]
loc_1_05F6:
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
        call   sub_1_2B44
        mov    ecx, ebx
        and    ecx, 0ffffh
        cmp    ecx, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_05F6
loc_1_0623:
        push   3
        push   3
        push   esi
        call   sub_1_0740
        test   al, al
        je     SHORT loc_1_0661
        mov    ecx, 1
        xor    eax, eax
        mov    word ptr [esi + 1eh], ax
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [esi + 1ch], ax
        mov    word ptr [esi + 1ah], ax
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    word ptr [esi + 12h], ax
        add    edx, 0ah
        out    dx, ax
        mov    word ptr [esi + 16h], ax
        mov    word ptr [esi + 18h], ax
        mov    word ptr [esi + 14h], cx
loc_1_0661:
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
sub_1_0680:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        xor    eax, eax
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi]
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
        push   3e8h
        out    dx, ax
        mov    eax, 0b6h
        out    dx, ax
        mov    edx, ecx
        mov    al, 2
        out    dx, al
        call   sub_1_2C90
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 0ah]
        pop    esi
        out    dx, ax
        lea    edx, [ecx + 0ch]
        pop    ebp
        out    dx, ax
        lea    edx, [ecx + 1]
        mov    al, 8
        out    dx, al
        ret    4
        db  08Dh, 049h, 000h
sub_1_0700:
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
loc_1_0714:
        xor    eax, eax
        mov    al, cl
        inc    cl
        cmp    cl, 6
        lea    edx, [edi + eax + 4]
        mov    al, byte ptr [eax + esi + 2dh]
        out    dx, al
        jb     SHORT loc_1_0714
        mov    edx, edi
        xor    al, al
        out    dx, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 0ch]
        cmp    bl, 1eh
        jne    SHORT loc_1_076A
        push   0c8h
        call   sub_1_2C90
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_076A:
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    al, 8
        inc    edx
        push   3e8h
        out    dx, al
        xor    di, di
        call   sub_1_2C90
        mov    dx, word ptr [esi]
        mov    al, bl
        out    dx, al
        push   3e8h
        call   sub_1_2C90
loc_1_0793:
        xor    ebx, ebx
        mov    bx, word ptr [esi]
        lea    ecx, [ebx + 1]
        mov    edx, ecx
        in     al, dx
        test   al, 8
        je     SHORT loc_1_07B3
        mov    edx, ebx
        in     al, dx
        and    al, 0fh
        cmp    al, byte ptr [ebp + 10h]
        je     SHORT loc_1_07CE
        mov    edx, ecx
        mov    al, 8
        out    dx, al
        jmp    SHORT loc_1_07BA
loc_1_07B3:
        push   64h
        call   sub_1_2C90
loc_1_07BA:
        inc    di
        cmp    di, 64h
        jb     SHORT loc_1_0793
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_07CE:
        mov    byte ptr [ebp - 1], 1
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_07E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    di, di
loc_1_07EB:
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_0810
        push   64h
        call   sub_1_2C90
        mov    ax, di
        inc    di
        cmp    ax, 3e8h
        jne    SHORT loc_1_07EB
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0810:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0820:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    edx, dword ptr [ebp + 0ch]
        push   esi
        add    edx, 2
        mov    ebx, 0ech
loc_1_0835:
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], bl
        cmp    byte ptr [ebp - 1], 24h
        je     SHORT loc_1_0849
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_1_0835
loc_1_0849:
        mov    bl, 40h
        xor    cl, cl
loc_1_084D:
        in     al, dx
        mov    esi, ebx
        mov    byte ptr [ebp - 1], al
        and    esi, 0ffh
        and    eax, 0ech
        sub    esi, eax
        cmp    esi, -24h
        jne    SHORT loc_1_0879
        add    bl, 40h
        inc    cl
        cmp    cl, 3
        jb     SHORT loc_1_084D
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0879:
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0890:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        xor    ecx, ecx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        mov    cx, word ptr [esi + 4]
        inc    edx
        in     al, dx
        cmp    ecx, -1
        jne    SHORT loc_1_08AF
        mov    word ptr [esi + 4], 2
loc_1_08AF:
        mov    cx, word ptr [esi + 4]
        cmp    cx, 1
        jne    SHORT loc_1_08BC
        or     al, 42h
        out    dx, al
loc_1_08BC:
        cmp    cx, 3
        jne    SHORT loc_1_08C8
        mov    word ptr [esi + 4], 2
loc_1_08C8:
        cmp    word ptr [esi + 4], 2
        jne    SHORT loc_1_08D4
        and    al, 0bfh
        or     al, 2
        out    dx, al
loc_1_08D4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08E0:
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
loc_1_0908:
        mov    edx, esi
        out    dx, ax
        add    ax, 3
        dec    cx
        jne    SHORT loc_1_0908
        mov    cx, 3
        mov    edx, edi
        xor    bx, bx
        xor    eax, eax
        out    dx, ax
loc_1_0921:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    SHORT loc_1_0999
        add    cx, 3
        inc    bx
        cmp    bx, 10h
        jb     SHORT loc_1_0921
        mov    cx, 10h
        mov    edx, edi
        mov    eax, 8000h
        out    dx, ax
        mov    ax, 9
loc_1_0947:
        mov    edx, esi
        out    dx, ax
        add    ax, 3
        dec    cx
        jne    SHORT loc_1_0947
        mov    cx, 10h
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
loc_1_095D:
        mov    edx, esi
        xor    eax, eax
        out    dx, ax
        dec    cx
        jne    SHORT loc_1_095D
        mov    cx, 9
        mov    edx, edi
        xor    bx, bx
        mov    eax, 8000h
        out    dx, ax
loc_1_0977:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    SHORT loc_1_09A4
        add    cx, 3
        inc    bx
        cmp    bx, 10h
        jb     SHORT loc_1_0977
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0999:
        mov    ax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_09A4:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_1_09B0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        push   6
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    edi, [esi + 2dh]
        add    esi, 27h
        push   edi
        call   sub_1_2D7C
        push   6
        push   edi
        push   esi
        call   sub_1_2D7C
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_09E0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [eax]
        pop    ebp
        add    edx, 3
        mov    al, 0fh
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 153h], 0
        jne    NEAR PTR loc_1_0ABE
        inc    dword ptr [esi + 136h]
        mov    word ptr [esi + 24h], 3
loc_1_0A21:
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        in     al, dx
        mov    bl, al
        and    bl, 7
        jne    SHORT loc_1_0A3F
        cmp    dword ptr [esi + 142h], 0
        jne    SHORT loc_1_0A3F
        cmp    word ptr [esi + 24h], 0
        jne    SHORT loc_1_0A91
loc_1_0A3F:
        mov    al, bl
        and    al, 3
        jne    SHORT loc_1_0A4C
        cmp    word ptr [esi + 24h], 0
        jne    SHORT loc_1_0A57
loc_1_0A4C:
        out    dx, al
        push   esi
        call   sub_1_0AD0
        test   al, al
        je     SHORT loc_1_0ABE
loc_1_0A57:
        mov    al, bl
        and    al, 8
        je     SHORT loc_1_0A64
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
loc_1_0A64:
        mov    al, bl
        and    al, 4
        je     SHORT loc_1_0A77
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
        call   sub_1_1CE0
loc_1_0A77:
        cmp    dword ptr [esi + 142h], 0
        je     SHORT loc_1_0A21
loc_1_0A80:
        push   esi
        call   sub_1_0C70
        cmp    dword ptr [esi + 142h], 0
        jne    SHORT loc_1_0A80
        jmp    SHORT loc_1_0A21
loc_1_0A91:
        xor    edx, edx
        mov    al, 8
        dec    dword ptr [esi + 136h]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
        cmp    byte ptr [esi + 133h], 0
        je     SHORT loc_1_0ABE
        mov    byte ptr [esi + 133h], 0
        mov    eax, dword ptr [esi + 1eeh]
        push   eax
        call   sub_1_2A20
loc_1_0ABE:
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 24h], 3
loc_1_0AE2:
        dec    word ptr [esi + 24h]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        lea    edx, [ecx + 0ch]
        out    dx, ax
        lea    edx, [ecx + 0eh]
        in     ax, dx
        mov    di, ax
        in     ax, dx
        test   ah, 20h
        je     NEAR PTR loc_1_0C0F
        test   di, 8
        je     NEAR PTR loc_1_0C0F
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        inc    dword ptr [esi + 161h]
        in     ax, dx
        cmp    ax, 5eah
        mov    di, ax
        ja     NEAR PTR loc_1_0BE4
        mov    bx, word ptr [esi + 1f2h]
        cmp    di, bx
        ja     SHORT loc_1_0B3D
        mov    ebx, edi
loc_1_0B3D:
        mov    ax, bx
        add    ecx, 0eh
        shr    ax, 1
        mov    edx, eax
        lea    eax, [esi + 33h]
        and    edx, 0ffffh
        push   edx
        push   eax
        push   ecx
        call   sub_1_2B81
        test   bl, 1
        je     SHORT loc_1_0B74
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0ffffh
        add    edx, 0eh
        in     al, dx
        mov    byte ptr [ecx + esi + 32h], al
loc_1_0B74:
        mov    cx, word ptr [esi + 20h]
        add    cx, 8
        cmp    byte ptr [esi + 0dh], 0
        mov    word ptr [esi + 22h], di
        je     SHORT loc_1_0B94
        cmp    cx, 8000h
        jb     SHORT loc_1_0BA0
        sub    cx, 6800h
        jmp    SHORT loc_1_0BA0
loc_1_0B94:
        cmp    cx, 3000h
        jae    SHORT loc_1_0BA0
        add    cx, 3000h
loc_1_0BA0:
        mov    eax, edi
        lea    edx, [esi + 41h]
        and    eax, 0ffffh
        and    ecx, 0ffffh
        sub    eax, 0eh
        push   eax
        mov    eax, ebx
        and    eax, 0ffffh
        lea    ebx, [esi + 33h]
        sub    eax, 0eh
        push   eax
        push   edx
        push   0eh
        mov    edx, dword ptr [esi + 1eeh]
        push   ebx
        push   ebx
        push   ecx
        push   edx
        call   sub_1_2840
        cmp    byte ptr [esi + 153h], 0
        jne    SHORT loc_1_0C1A
        mov    byte ptr [esi + 133h], 1
loc_1_0BE4:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 20h], ax
        sub    ax, 2
        cmp    word ptr [esi + 0eh], ax
        jbe    SHORT loc_1_0BFA
        add    ax, word ptr [esi + 10h]
loc_1_0BFA:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 6
        out    dx, ax
        cmp    word ptr [esi + 24h], 0
        jne    NEAR PTR loc_1_0AE2
loc_1_0C0F:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0C1A:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    word ptr [esi + 1eh], 0
        je     SHORT loc_1_0C56
        mov    ax, word ptr [esi + 1f4h]
        dec    ax
        mov    word ptr [esi + 1f4h], ax
        jne    SHORT loc_1_0C56
        push   esi
        call   sub_1_1CE0
loc_1_0C56:
        push   7d0h
        add    esi, 1ddh
        push   esi
        call   sub_1_2C72
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_1_0C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 142h]
        mov    dword ptr [ebx + 14ah], edi
        lea    esi, [edi + 24h]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_0C9A
        mov    dword ptr [ebx + 146h], 0
loc_1_0C9A:
        mov    eax, dword ptr [esi]
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        mov    dword ptr [ebx + 142h], eax
        push   0
        push   0
        push   edi
        call   sub_1_2CC0
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 10h]
        push   edx
        push   eax
        call   sub_1_2CE4
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 1f2h]
        cmp    ecx, dword ptr [ebp - 8]
        jbe    SHORT loc_1_0CFE
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        je     SHORT loc_1_0CFE
        cmp    ecx, eax
        jb     SHORT loc_1_0CE1
        mov    ecx, eax
loc_1_0CE1:
        lea    eax, [ebp - 8]
        lea    edx, [ebx + 33h]
        push   eax
        and    ecx, 0ffffh
        push   edx
        push   ecx
        push   0
        push   edi
        call   sub_1_0D50
        lea    ecx, [ebx + 33h]
        mov    dword ptr [ebp - 4], ecx
loc_1_0CFE:
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + 133h], 1
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0eh
        mov    ecx, dword ptr [ebx + 1eeh]
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   0
        push   ecx
        call   sub_1_2840
        push   0
        mov    ecx, dword ptr [esi + 10h]
        push   edi
        push   ecx
        call   sub_1_2BDA
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0D50:
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
        je     NEAR PTR loc_1_0E19
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_2CC0
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_0E19
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_2CE4
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_0E14
        mov    edi, dword ptr [ebp + 0ch]
loc_1_0D9F:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0DCB
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_2D08
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_0E14
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_2CE4
        jmp    SHORT loc_1_0E0F
loc_1_0DCB:
        test   edi, edi
        je     SHORT loc_1_0DE8
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_0DE0
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_0E0F
loc_1_0DE0:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_0DE8:
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_1_0DF6
        mov    ebx, eax
loc_1_0DF6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        add    esi, ebx
        call   sub_1_2D7C
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_0E0F:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_1_0D9F
loc_1_0E14:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_0E19:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    eax, dword ptr [ebp + 1ch]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    byte ptr [ebp - 1], bl
        mov    dword ptr [eax], ebx
        cmp    dword ptr [ebp + 10h], ebx
        je     NEAR PTR loc_1_0F8D
        push   ebx
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 14h]
        push   ebx
        push   edx
        call   sub_1_2CC0
        cmp    dword ptr [ebp - 14h], ebx
        je     NEAR PTR loc_1_0F8D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        call   sub_1_2CE4
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 22h]
        mov    ecx, dword ptr [ebp + 18h]
        sub    eax, ecx
        cmp    eax, dword ptr [ebp + 10h]
        jae    SHORT loc_1_0E8F
        mov    dword ptr [ebp + 10h], eax
loc_1_0E8F:
        test   cl, 1
        je     SHORT loc_1_0E99
        mov    byte ptr [ebp - 1], 1
        dec    ecx
loc_1_0E99:
        mov    ax, word ptr [ebp + 0ch]
        add    ax, cx
        cmp    byte ptr [esi + 0dh], 0
        je     SHORT loc_1_0EB7
        cmp    ax, 8000h
        jb     SHORT loc_1_0EC8
        sub    ax, 6800h
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    SHORT loc_1_0ECD
loc_1_0EB7:
        cmp    ax, 3000h
        jae    SHORT loc_1_0EC8
        add    ax, 3000h
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    SHORT loc_1_0ECD
loc_1_0EC8:
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_0ECD:
        add    edx, 0ch
        cmp    dword ptr [ebp + 10h], 0
        out    dx, ax
        je     NEAR PTR loc_1_0F88
loc_1_0EDC:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_0F0C
        lea    eax, [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        push   ecx
        call   sub_1_2D08
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_0F88
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        call   sub_1_2CE4
        jmp    SHORT loc_1_0F7F
loc_1_0F0C:
        mov    edi, dword ptr [ebp + 10h]
        sub    edi, ebx
        cmp    edi, dword ptr [ebp - 8]
        jbe    SHORT loc_1_0F19
        mov    edi, dword ptr [ebp - 8]
loc_1_0F19:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_0F35
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dx, word ptr [esi]
        dec    edi
        add    edx, 0fh
        inc    ebx
        in     al, dx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        dec    dword ptr [ebp - 8]
loc_1_0F35:
        test   edi, 0fffffffeh
        je     SHORT loc_1_0F55
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 0ch]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0eh
        push   eax
        call   sub_1_2B81
loc_1_0F55:
        test   edi, 1
        je     SHORT loc_1_0F73
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ebp - 1], 1
        mov    dx, word ptr [esi]
        add    edx, 0eh
        in     al, dx
        mov    byte ptr [ecx + edi - 1], al
        jmp    SHORT loc_1_0F77
loc_1_0F73:
        mov    byte ptr [ebp - 1], 0
loc_1_0F77:
        add    ebx, edi
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 8], edi
loc_1_0F7F:
        cmp    ebx, dword ptr [ebp + 10h]
        jb     NEAR PTR loc_1_0EDC
loc_1_0F88:
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], ebx
loc_1_0F8D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FA0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ecx + 8]
        cmp    byte ptr [edx + 153h], 0
        jne    SHORT loc_1_101D
        cmp    byte ptr [ecx + 18h], 0
        jne    SHORT loc_1_1013
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_1_0FD3
        cmp    eax, 1
        je     SHORT loc_1_0FF3
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    8
loc_1_0FD3:
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
        push   ecx
        push   edx
        call   sub_1_1230
        pop    esi
        pop    ebp
        ret    8
loc_1_0FF3:
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
        push   ecx
        push   edx
        call   sub_1_1070
        pop    esi
        pop    ebp
        ret    8
loc_1_1013:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    8
loc_1_101D:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1030:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    dword ptr [ecx + 10h], 2
        je     SHORT loc_1_1046
        mov    eax, 0c00000bbh
        pop    ebp
        ret    8
loc_1_1046:
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
        call   sub_1_1230
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1070:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        cmp    eax, 1010103h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        mov    dword ptr [esi], 0
        ja     SHORT loc_1_10AF
        je     NEAR PTR loc_1_11D0
        cmp    eax, 1010eh
        je     SHORT loc_1_10BE
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_113C
        cmp    eax, 10112h
        je     NEAR PTR loc_1_11C4
loc_1_10AF:
        mov    esi, 0c0010017h
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_10BE:
        mov    edi, dword ptr [ebp + 18h]
        cmp    edi, 4
        je     SHORT loc_1_10D9
        mov    eax, 0c0010015h
        pop    edi
        mov    dword ptr [esi], 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_10D9:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_2D7C
        test   dword ptr [ebp - 8], 0f054h
        je     SHORT loc_1_110D
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], 4
        mov    eax, 0c00000bbh
        mov    dword ptr [esi], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_110D:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0ch]
        mov    eax, dword ptr [esi + 1eeh]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_112B:
        mov    esi, eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_113C:
        xor    esi, esi
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_115C
        mov    esi, 0c0010014h
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_115C:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2D7C
        cmp    dword ptr [ebp - 4], 100h
        ja     SHORT loc_1_11B5
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 1f2h]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_11A1
        mov    ax, word ptr [ebp - 4]
        pop    edi
        mov    word ptr [ecx + 1f2h], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 41h], ax
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11A1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 41h], ax
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11B5:
        mov    esi, 0c0000001h
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11C4:
        xor    esi, esi
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11D0:
        mov    ecx, 6
        mov    edi, dword ptr [ebp + 18h]
        mov    eax, edi
        cdq
        idiv   ecx
        test   edx, edx
        je     SHORT loc_1_11F0
        mov    eax, 0c0010015h
        pop    edi
        mov    dword ptr [esi], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11F0:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, edi
        cdq
        idiv   ecx
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [ecx + 1eeh]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1219:
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, esi
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1230:
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
        ja     SHORT loc_1_127E
        je     NEAR PTR loc_1_15AB
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_129F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_127E:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_15D0
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_15F5
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_1684
loc_1_129F:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_12B1
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_168B
loc_1_12B1:
        cmp    edx, 2020dh
        ja     SHORT loc_1_12D1
        je     NEAR PTR loc_1_13B6
        sub    edx, 20101h
        cmp    edx, 4
        ja     SHORT loc_1_12EB
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_12D1:
        cmp    edx, 1020101h
        ja     SHORT loc_1_12F7
        je     NEAR PTR loc_1_13D8
        cmp    edx, 2020eh
        je     NEAR PTR loc_1_13C7
loc_1_12EB:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_168B
loc_1_12F7:
        cmp    edx, 1020201h
        ja     SHORT loc_1_131F
        je     NEAR PTR loc_1_140B
        cmp    edx, 1020102h
        je     NEAR PTR loc_1_13E9
        cmp    edx, 1020103h
        je     NEAR PTR loc_1_13FA
        jmp    SHORT loc_1_12EB
loc_1_131F:
        sub    edx, 1020202h
        cmp    edx, 4
        ja     SHORT loc_1_12EB
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_1331:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 15dh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1342:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 161h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1353:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 169h]
        add    eax, dword ptr [edi + 16dh]
        add    eax, dword ptr [edi + 171h]
        add    eax, dword ptr [edi + 175h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1376:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 19dh]
        add    eax, dword ptr [edi + 191h]
        add    eax, dword ptr [edi + 195h]
        add    eax, dword ptr [edi + 185h]
        add    eax, dword ptr [edi + 199h]
        add    eax, dword ptr [edi + 189h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_13A5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 199h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_13B6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 191h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_13C7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 165h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_13D8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 195h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_13E9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17dh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_13FA:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 181h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_140B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 179h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_141C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 169h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_142D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 19dh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_143E:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 171h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_144F:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 175h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1460:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 16dh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1471:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_1486
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 0a0h
        jmp    NEAR PTR loc_1_168B
loc_1_1486:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 5ch
        jmp    NEAR PTR loc_1_168B
loc_1_1495:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_168B
loc_1_14A1:
        lea    esi, [ebp - 1ch]
        mov    ecx, 4
        jmp    NEAR PTR loc_1_168B
loc_1_14AE:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_168B
loc_1_14BA:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_168B
loc_1_14C6:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_168B
loc_1_14D2:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_14E3:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 10h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_14F4:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_168B
loc_1_1500:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 27h
        lea    eax, [ebp - 14h]
        push   edi
        lea    esi, [ebp - 14h]
        push   eax
        call   sub_1_2D7C
        mov    byte ptr [ebp - 11h], 0
        mov    ecx, 4
        jmp    NEAR PTR loc_1_168B
loc_1_1523:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 1eh
        jmp    NEAR PTR loc_1_168B
loc_1_1532:
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        je     SHORT loc_1_1547
        mov    eax, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_168B
loc_1_1547:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 1eeh]
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_168B
loc_1_155B:
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        jne    SHORT loc_1_1576
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 1f2h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1576:
        xor    eax, eax
        mov    ax, word ptr [ebx + 41h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_168B
loc_1_1584:
        lea    esi, [ebp - 2]
        mov    ecx, 2
        mov    word ptr [ebp - 2], 100h
        jmp    NEAR PTR loc_1_168B
loc_1_1597:
        lea    esi, [ebp - 8]
        mov    ecx, 4
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_168B
loc_1_15AB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 27h]
        add    edi, 27h
        lea    esi, [ebp - 24h]
        mov    dword ptr [ebp - 24h], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 20h], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 1fh], dl
        jmp    NEAR PTR loc_1_168B
loc_1_15D0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 2dh]
        add    edi, 2dh
        lea    esi, [ebp - 24h]
        mov    dword ptr [ebp - 24h], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 20h], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 1fh], dl
        jmp    NEAR PTR loc_1_168B
loc_1_15F5:
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        jne    SHORT loc_1_162D
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 18h]
        push   ecx
        mov    edi, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 18h]
        mov    eax, dword ptr [edi + 1eeh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_161B:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        lea    ecx, [eax*2]
        jmp    SHORT loc_1_168B
loc_1_162D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    ecx, dword ptr [edi + 1eeh]
        push   ecx
        lea    edx, [ebp - 18h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1650:
        cmp    dword ptr [ebp - 18h], 0
        mov    esi, dword ptr [ebp + 14h]
        jne    SHORT loc_1_1668
        mov    eax, dword ptr [ebp - 10h]
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    SHORT loc_1_168B
loc_1_1668:
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [edi + 1eeh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1678:
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    SHORT loc_1_168B
loc_1_1684:
        mov    dword ptr [ebp - 8], 10h
loc_1_168B:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_16C2
        cmp    ecx, dword ptr [ebp + 18h]
        jbe    SHORT loc_1_16AE
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
loc_1_16AE:
        mov    eax, dword ptr [ebp + 1ch]
        test   ecx, ecx
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_16C2
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        push   eax
        call   sub_1_2D7C
loc_1_16C2:
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
loc_1_1750:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    edi, edi
        cmp    byte ptr [esi + 153h], 0
        je     SHORT loc_1_1776
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_1776:
        xor    ebx, ebx
        mov    al, 0bh
        mov    bx, word ptr [esi]
        mov    edx, ebx
        out    dx, al
        mov    al, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        out    dx, al
        lea    edx, [ebx + 2]
        in     al, dx
        and    al, 0feh
        or     al, 2
        test   cl, 24h
        je     SHORT loc_1_1795
        or     al, 1
loc_1_1795:
        test   cl, 8
        je     SHORT loc_1_179C
        and    al, 0fdh
loc_1_179C:
        out    dx, al
        lea    edx, [ebx + 3]
        push   esi
        in     al, dx
        out    dx, al
        mov    edx, ebx
        xor    al, al
        out    dx, al
        call   sub_1_07E0
        test   al, al
        je     SHORT loc_1_180C
        mov    dx, word ptr [esi]
        mov    al, 1eh
        out    dx, al
        push   3
        call   sub_1_2C90
        mov    bx, word ptr [esi + 0eh]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    ax, bx
        lea    edx, [ecx + 4]
        out    dx, ax
        mov    ax, word ptr [esi + 10h]
        lea    edx, [ecx + 6]
        add    ax, bx
        dec    ax
        out    dx, ax
        lea    edx, [ecx + 0ah]
        xor    eax, eax
        out    dx, ax
        mov    word ptr [esi + 1eh], ax
        mov    word ptr [esi + 1ch], ax
        mov    word ptr [esi + 1ah], ax
        mov    word ptr [esi + 12h], ax
        mov    word ptr [esi + 16h], ax
        mov    word ptr [esi + 18h], ax
        mov    word ptr [esi + 14h], 1
        mov    bx, word ptr [esi + 0eh]
        mov    word ptr [esi + 20h], bx
        jmp    SHORT loc_1_1811
loc_1_180C:
        mov    edi, 0c0000001h
loc_1_1811:
        mov    dx, word ptr [esi]
        mov    al, 8
        out    dx, al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_1820:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 153h], 0
        je     SHORT loc_1_183C
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_1_183C:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 10h
        jbe    SHORT loc_1_184D
        mov    eax, 0c000009ah
        pop    ebp
        ret    1ch
loc_1_184D:
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        push   eax
        push   edx
        call   sub_1_0590
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1870:
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
        call   sub_1_2600
        push   0
        push   0
        push   edi
        call   sub_1_2C9C
        push   0
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_2BD4
        cmp    dword ptr [esi + 18dh], 0
        jne    SHORT loc_1_18BC
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
loc_1_18BC:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [edi + 153h], 0
        jne    NEAR PTR loc_1_19A6
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [edi + 12h]
        mov    cx, word ptr [edi + 0eh]
        add    eax, 5f2h
        cmp    eax, ecx
        jge    SHORT loc_1_1965
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_1959
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        push   edi
        call   sub_1_19C0
        cmp    eax, 103h
        jne    SHORT loc_1_1949
        mov    dword ptr [ebx + 2ch], esi
        lea    ecx, [ebx + 24h]
        inc    dword ptr [esi + 14h]
        cmp    dword ptr [edi + 142h], 0
        jne    SHORT loc_1_1931
        mov    dword ptr [edi + 142h], ebx
        jmp    SHORT loc_1_193A
loc_1_1931:
        mov    edx, dword ptr [edi + 146h]
        mov    dword ptr [edx + 24h], ebx
loc_1_193A:
        mov    dword ptr [ecx], 0
        mov    dword ptr [edi + 146h], ebx
        dec    dword ptr [esi + 14h]
loc_1_1949:
        mov    word ptr [edi + 1f4h], 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_1959:
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_1965:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 2ch], esi
        lea    eax, [ebx + 24h]
        inc    dword ptr [esi + 14h]
        cmp    dword ptr [edi + 13ah], 0
        jne    SHORT loc_1_1982
        mov    dword ptr [edi + 13ah], ebx
        jmp    SHORT loc_1_198B
loc_1_1982:
        mov    ecx, dword ptr [edi + 13eh]
        mov    dword ptr [ecx + 24h], ebx
loc_1_198B:
        mov    dword ptr [eax], 0
        mov    eax, 103h
        mov    dword ptr [edi + 13eh], ebx
        pop    edi
        inc    dword ptr [esi + 14h]
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_19A6:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_19C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        lea    eax, [ebp - 2ch]
        push   esi
        lea    ecx, [ebp - 10h]
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 4], si
        push   eax
        push   ecx
        lea    edx, [ebp - 30h]
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [ebp - 20h], esi
        push   eax
        call   sub_1_2CC0
        cmp    dword ptr [ebp - 2ch], esi
        je     NEAR PTR loc_1_1CC4
        cmp    dword ptr [ebp - 2ch], 5eah
        ja     NEAR PTR loc_1_1CC4
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        call   sub_1_2CE4
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 18h]
        mov    di, word ptr [esi + 0eh]
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    edx, edi
        add    ecx, 8
        and    edx, 0ffffh
        cmp    ecx, edx
        jge    SHORT loc_1_1A38
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    SHORT loc_1_1A40
loc_1_1A38:
        sub    ax, di
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_1A40:
        add    ax, 8
        add    edx, 0ch
        out    dx, ax
        push   3
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0eh
        push   eax
        call   sub_1_2B44
        push   6
        mov    ecx, dword ptr [ebp - 0ch]
        mov    di, 6
        push   ecx
        lea    edx, [ebp - 38h]
        push   edx
        call   sub_1_2D7C
        add    dword ptr [ebp - 0ch], 6
        mov    bx, word ptr [ebp - 30h]
        sub    dword ptr [ebp - 8], 6
        test   bx, bx
        je     NEAR PTR loc_1_1B21
loc_1_1A85:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1AF5
        cmp    word ptr [ebp - 4], 0
        je     SHORT loc_1_1AA8
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 0ch]
        add    edx, 0fh
        mov    al, byte ptr [eax]
        dec    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        out    dx, al
loc_1_1AA8:
        test   dword ptr [ebp - 8], 0fffffffeh
        je     SHORT loc_1_1ACA
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0eh
        push   eax
        call   sub_1_2B44
loc_1_1ACA:
        test   byte ptr [ebp - 8], 1
        je     SHORT loc_1_1AEB
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 4], 1
        add    edx, 0eh
        mov    al, byte ptr [ecx + eax - 1]
        out    dx, al
        jmp    SHORT loc_1_1AF1
loc_1_1AEB:
        mov    word ptr [ebp - 4], 0
loc_1_1AF1:
        add    di, word ptr [ebp - 8]
loc_1_1AF5:
        cmp    bx, 1
        jbe    SHORT loc_1_1B19
        lea    eax, [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        push   ecx
        call   sub_1_2D08
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 10h]
        push   edx
        push   eax
        call   sub_1_2CE4
loc_1_1B19:
        dec    bx
        jne    NEAR PTR loc_1_1A85
loc_1_1B21:
        cmp    word ptr [ebp - 4], 0
        je     SHORT loc_1_1B35
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        xor    al, al
        add    edx, 0fh
        out    dx, al
loc_1_1B35:
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    dword ptr [ebp - 14h], eax
        lea    ecx, [eax + 0eh]
        mov    edx, ecx
        xor    eax, eax
        out    dx, ax
        cmp    di, 3ch
        ja     SHORT loc_1_1B76
        mov    ebx, 3ch
        mov    eax, edi
        and    eax, 0ffffh
        sub    ebx, eax
        sar    ebx, 1
        test   bx, bx
        je     SHORT loc_1_1B6C
loc_1_1B62:
        mov    edx, ecx
        xor    eax, eax
        out    dx, ax
        dec    bx
        jne    SHORT loc_1_1B62
loc_1_1B6C:
        mov    word ptr [ebp - 4], 0
        mov    di, 3ch
loc_1_1B76:
        mov    bx, word ptr [esi + 18h]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0ch
        mov    edx, ebx
        and    edx, 0ffffh
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 1ch], edx
        mov    edx, eax
        mov    eax, dword ptr [ebp - 1ch]
        out    dx, ax
        mov    edx, ecx
        mov    eax, 4
        out    dx, ax
        xor    eax, eax
        out    dx, ax
        mov    edx, edi
        mov    eax, dword ptr [ebp - 1ch]
        and    edx, 0ffffh
        lea    edx, [edx + eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0eh]
        mov    dword ptr [ebp - 24h], edx
        mov    dword ptr [ebp - 18h], eax
        cmp    edx, eax
        jge    SHORT loc_1_1BC6
        lea    eax, [ebx + edi]
        jmp    SHORT loc_1_1BD0
loc_1_1BC6:
        mov    ax, di
        sub    ax, word ptr [esi + 0eh]
        add    ax, bx
loc_1_1BD0:
        add    ax, 8
        mov    edx, ecx
        out    dx, ax
        mov    ax, di
        mov    edx, ecx
        sub    ax, word ptr [ebp - 4]
        out    dx, ax
        cmp    word ptr [esi + 14h], 0
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_1_1BFC
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        out    dx, al
        mov    word ptr [esi + 14h], 0
        jmp    SHORT loc_1_1C22
loc_1_1BFC:
        mov    ax, word ptr [esi + 1ah]
        mov    edx, dword ptr [ebp - 28h]
        out    dx, ax
        mov    ax, word ptr [esi + 1ch]
        mov    edx, ecx
        or     ah, 80h
        and    eax, 0ffffh
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        inc    edx
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 1ch
        out    dx, al
loc_1_1C22:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 1ch], ax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 6
        cmp    eax, dword ptr [ebp - 18h]
        jge    SHORT loc_1_1C3A
        lea    eax, [ebx + 6]
        jmp    SHORT loc_1_1C45
loc_1_1C3A:
        mov    ax, bx
        sub    ax, word ptr [esi + 0eh]
        add    ax, 6
loc_1_1C45:
        mov    word ptr [esi + 1ah], ax
        mov    ecx, dword ptr [ebp - 18h]
        lea    eax, [edi + 8]
        add    word ptr [esi + 12h], ax
        cmp    dword ptr [ebp - 24h], ecx
        jge    SHORT loc_1_1C65
        lea    eax, [ebx + edi]
        add    ax, 8
        mov    word ptr [esi + 18h], ax
        jmp    SHORT loc_1_1C74
loc_1_1C65:
        sub    di, word ptr [esi + 0eh]
        add    di, bx
        add    di, 8
        mov    word ptr [esi + 18h], di
loc_1_1C74:
        inc    word ptr [esi + 1eh]
        lea    eax, [ebp - 38h]
        push   eax
        mov    ecx, dword ptr [esi + 1eeh]
        push   ecx
        inc    dword ptr [esi + 15dh]
        call   sub_1_2AC0
        test   al, al
        je     SHORT loc_1_1CB2
        mov    dword ptr [ebp - 20h], 103h
        cmp    word ptr [ebp + 10h], 0
        jne    SHORT loc_1_1CB8
        push   esi
        call   sub_1_1CE0
        mov    eax, dword ptr [ebp - 20h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1CB2:
        push   esi
        call   sub_1_1CE0
loc_1_1CB8:
        mov    eax, dword ptr [ebp - 20h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1CC4:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1CE0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 1], cl
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 1eh], cx
        je     NEAR PTR loc_1_1DD0
        xor    edi, edi
        mov    di, word ptr [esi]
        lea    eax, [edi + 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_1D06:
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 16h]
        out    dx, ax
        lea    edx, [edi + 0eh]
        in     ax, dx
        test   al, 80h
        je     NEAR PTR loc_1_1DD0
        mov    byte ptr [ebp - 1], 1
        in     ax, dx
        test   ah, 20h
        jne    SHORT loc_1_1D51
        mov    cl, 1
        test   al, 20h
        je     SHORT loc_1_1D37
        inc    dword ptr [esi + 169h]
        jmp    SHORT loc_1_1D77
loc_1_1D37:
        test   ah, 4
        je     SHORT loc_1_1D44
        inc    dword ptr [esi + 16dh]
        jmp    SHORT loc_1_1D77
loc_1_1D44:
        test   ah, 1
        je     SHORT loc_1_1D77
        inc    dword ptr [esi + 171h]
        jmp    SHORT loc_1_1D77
loc_1_1D51:
        test   al, 80h
        je     SHORT loc_1_1D5D
        inc    dword ptr [esi + 179h]
        jmp    SHORT loc_1_1D77
loc_1_1D5D:
        and    ax, 0fh
        cmp    ax, 1
        jne    SHORT loc_1_1D6F
        inc    dword ptr [esi + 17dh]
        jmp    SHORT loc_1_1D77
loc_1_1D6F:
        jbe    SHORT loc_1_1D77
        inc    dword ptr [esi + 181h]
loc_1_1D77:
        lea    edx, [edi + 0eh]
        in     ax, dx
        mov    bx, ax
        mov    dx, word ptr [esi + 0eh]
        sub    bx, word ptr [esi + 16h]
        cmp    bx, dx
        jbe    SHORT loc_1_1D8F
        add    bx, dx
loc_1_1D8F:
        mov    word ptr [esi + 1f4h], 3
        sub    word ptr [esi + 12h], bx
        mov    bx, word ptr [esi + 1eh]
        dec    bx
        test   cl, cl
        mov    word ptr [esi + 16h], ax
        mov    word ptr [esi + 1eh], bx
        je     SHORT loc_1_1DC7
        test   bx, bx
        lea    edx, [edi + 0ah]
        out    dx, ax
        je     SHORT loc_1_1DBF
        mov    edx, edi
        mov    al, 4
        out    dx, al
        jmp    SHORT loc_1_1DC5
loc_1_1DBF:
        mov    word ptr [esi + 14h], 1
loc_1_1DC5:
        xor    cl, cl
loc_1_1DC7:
        test   bx, bx
        jne    NEAR PTR loc_1_1D06
loc_1_1DD0:
        cmp    dword ptr [esi + 13ah], 0
        je     NEAR PTR loc_1_1E7D
loc_1_1DDD:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 12h]
        mov    ax, word ptr [esi + 0eh]
        add    ecx, 5f2h
        cmp    ecx, eax
        jge    NEAR PTR loc_1_1E7D
        mov    ebx, dword ptr [esi + 13ah]
        lea    edi, [ebx + 24h]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_1E0F
        mov    dword ptr [esi + 13eh], 0
loc_1_1E0F:
        mov    eax, dword ptr [edi]
        push   1
        push   ebx
        mov    dword ptr [esi + 13ah], eax
        push   esi
        call   sub_1_19C0
        cmp    eax, 103h
        jne    SHORT loc_1_1E55
        mov    eax, dword ptr [edi + 8]
        inc    dword ptr [eax + 14h]
        cmp    dword ptr [esi + 142h], 0
        jne    SHORT loc_1_1E3E
        mov    dword ptr [esi + 142h], ebx
        jmp    SHORT loc_1_1E47
loc_1_1E3E:
        mov    eax, dword ptr [esi + 146h]
        mov    dword ptr [eax + 24h], ebx
loc_1_1E47:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 146h], ebx
        jmp    SHORT loc_1_1E67
loc_1_1E55:
        mov    edi, dword ptr [edi + 8]
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_2BDA
        dec    dword ptr [edi + 14h]
loc_1_1E67:
        cmp    dword ptr [esi + 13ah], 0
        mov    word ptr [esi + 1f4h], 3
        jne    NEAR PTR loc_1_1DDD
loc_1_1E7D:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E90:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 40h
        push   esi
        lea    ecx, [ebp - 4]
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_1_2BAA
        mov    eax, dword ptr [ebp - 4]
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 40h], 3
        mov    byte ptr [ebp - 3fh], 0
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0EAh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        push   36h
        mov    dword ptr [ebp - 12h], edx
        lea    edx, [ebp - 40h]
        push   edx
        mov    dword ptr [ebp - 0eh], ecx
        push   0
        lea    ecx, [ebp - 8]
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_2BB6
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1F5E
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2BB0
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F5E:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1F70:
        push   ebp
        mov    ecx, 0ah
        mov    ebp, esp
        sub    esp, 38h
        mov    word ptr [ebp - 38h], cx
        push   esi
        mov    word ptr [ebp - 36h], cx
        push   edi
        mov    word ptr [ebp - 30h], cx
        mov    eax, 0ch
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2eh], cx
        mov    word ptr [ebp - 28h], ax
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 26h], ax
        mov    eax, 0fh
        mov    word ptr [ebp - 20h], ax
        mov    word ptr [ebp - 1eh], ax
        lea    edx, [ebp - 4]
        mov    word ptr [ebp - 18h], cx
        mov    word ptr [ebp - 16h], cx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        push   256h
        push   edx
        call   sub_1_2C96
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_1FFD
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1FFD:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 95h
        rep stosd
        stosw
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1bdh], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 154h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 26h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 1a1h
        mov    dword ptr [ecx + 1a5h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 1a5h]
        mov    dword ptr [eax + 1a1h], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1a9h
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 1adh], eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        lea    ecx, [ebp - 10h]
        mov    edx, dword ptr [eax + 1adh]
        push   ecx
        mov    dword ptr [eax + 1a9h], edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D14
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_209B
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2C9C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_209B:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        mov    ecx, 3
        jne    SHORT loc_1_20C3
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax + 4]
loc_1_20C3:
        mov    dl, 1fh
        mov    eax, dword ptr [ebp - 4]
        sub    dl, cl
        push   1
        mov    byte ptr [eax + 1e9h], dl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1eah], ecx
        lea    ecx, [ebp - 30h]
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_2104
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx], dx
        jmp    SHORT loc_1_210C
loc_1_2104:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 300h
loc_1_210C:
        push   1
        lea    eax, [ebp - 28h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_2138
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 2], dx
        jmp    SHORT loc_1_2141
loc_1_2138:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 0ffffh
loc_1_2141:
        push   1
        lea    eax, [ebp - 20h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_216D
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 4], dx
        jmp    SHORT loc_1_2176
loc_1_216D:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0ffffh
loc_1_2176:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_219F
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 6], 0
        jmp    SHORT loc_1_21AB
loc_1_219F:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 6], edx
loc_1_21AB:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_2D20
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    edx, dword ptr [ecx + 1eah]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_21CA:
        cmp    ax, 0ffffh
        je     SHORT loc_1_2214
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_21D6:
        test   eax, eax
        je     SHORT loc_1_2214
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        push   ecx
        call   sub_1_22A0
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_220A
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2C9C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_220A:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2214:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2C9C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_2230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        mov    byte ptr [esi + 154h], 1
        lea    ecx, [esi + 1ddh]
        push   ecx
        call   sub_1_2C78
        lea    ecx, [esi + 1c5h]
        push   ecx
        call   sub_1_2C84
        mov    ecx, dword ptr [esi + 1eeh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2269:
        mov    ecx, dword ptr [esi + 1c1h]
        push   ecx
        call   sub_1_2BC8
        mov    ecx, dword ptr [esi + 1b5h]
        lea    edx, [esi + 1b1h]
        push   0
        push   0
        mov    eax, dword ptr [edx]
        push   esi
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    edx, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
        call   sub_1_2C9C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_22A0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 9
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        lea    edi, [ebp - 28h]
        rep stosd
        stosw
        mov    eax, 1
        mov    dword ptr [ebp - 28h], ebx
        mov    byte ptr [ebp - 24h], bl
        mov    dword ptr [ebp - 1eh], eax
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    dword ptr [ebp - 1ah], ebx
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 16h], ebx
        mov    dword ptr [ebp - 12h], eax
        mov    dword ptr [ebp - 0eh], ecx
        lea    eax, [ebp - 28h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ah], 10h
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [esi + 1bdh]
        push   esi
        lea    edx, [esi + 1c1h]
        push   eax
        push   edx
        call   sub_1_2BC2
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_232C
        push   ebx
        mov    eax, dword ptr [esi + 1c1h]
        push   2
        push   2
        push   0aa000002h
        push   eax
        call   sub_1_2D68
        add    esp, 14h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_232C:
        push   esi
        call   sub_1_2640
        push   esi
        call   sub_1_26E0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   esi
        call   sub_1_02C0
        test   al, al
        jne    SHORT loc_1_2360
        mov    eax, dword ptr [esi + 1c1h]
        push   eax
        call   sub_1_2BC8
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_2360:
        lea    eax, [esi + 1eeh]
        lea    ecx, [esi + 1b9h]
        push   eax
        lea    edx, [esi + 2dh]
        push   ecx
        mov    dword ptr [esi + 1e5h], esi
        mov    dword ptr [esi + 1ddh], 0
        db  0C7h, 086h, 0E1h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_23A2:
        test   al, al
        jne    SHORT loc_1_23C0
        mov    eax, dword ptr [esi + 1c1h]
        push   eax
        call   sub_1_2BC8
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_23C0:
        lea    ecx, [esi + 1b1h]
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        pop    edi
        db  0A1h, 004h, 000h, 000h, 000h
        mov    dword ptr [esi + 1b5h], eax
        db  0A1h, 004h, 000h, 000h, 000h
        pop    esi
        pop    ebx
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    esp, ebp
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        pop    ebp
        ret    10h
        db  090h
loc_1_23F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 154h], 0
        je     SHORT loc_1_240F
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_240F:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, edx
        je     SHORT loc_1_242C
        mov    eax, dword ptr [ebp + 14h]
loc_1_241B:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_2428
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_1_241B
loc_1_2428:
        cmp    ecx, edx
        jne    SHORT loc_1_2438
loc_1_242C:
        mov    eax, 0c0010019h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2438:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], edx
        lea    edx, [ebp - 4]
        inc    dword ptr [esi + 136h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        push   43h
        push   edx
        call   sub_1_2C96
        test   eax, eax
        je     SHORT loc_1_2467
        mov    eax, 0c000009ah
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_2467:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 100h
        mov    byte ptr [ecx + 18h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 41h], ax
        mov    word ptr [esi + 1f2h], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        push   eax
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 1eeh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_24CB:
        test   al, al
        jne    SHORT loc_1_24EA
        mov    esi, 0c0000001h
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2C9C
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_24EA:
        lea    eax, [esi + 1a1h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 1a5h]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [esi + 1a5h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1a5h], eax
        mov    eax, dword ptr [esi + 18dh]
        inc    eax
        mov    dword ptr [esi + 18dh], eax
        cmp    eax, 1
        jne    SHORT loc_1_2543
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi]
        push   0ea60h
        add    edx, 3
        add    esi, 1ddh
        out    dx, al
        push   esi
        call   sub_1_2C72
loc_1_2543:
        xor    esi, esi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_1_2550:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 18h], al
        jne    SHORT loc_1_25C7
        mov    byte ptr [esi + 18h], 1
        push   eax
        dec    dword ptr [edi + 18dh]
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 1eeh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_257D:
        cmp    eax, 0c001000eh
        jne    SHORT loc_1_258F
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_258F:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], edx
        dec    dword ptr [esi + 14h]
        call   sub_1_2600
        push   0
        push   0
        push   esi
        call   sub_1_2C9C
        cmp    dword ptr [edi + 18dh], 0
        jne    SHORT loc_1_25C5
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [edi]
        add    edx, 3
        out    dx, al
loc_1_25C5:
        xor    eax, eax
loc_1_25C7:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_25D0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2BBC
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_2BB0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2600:
        xor    dx, dx
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 1a1h]
        sub    eax, esi
        cmp    eax, 0fffffe5fh
        je     SHORT loc_1_2634
loc_1_261B:
        mov    ax, word ptr [esi + 41h]
        cmp    ax, dx
        jbe    SHORT loc_1_2627
        mov    dx, ax
loc_1_2627:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0fffffe5fh
        jne    SHORT loc_1_261B
loc_1_2634:
        mov    word ptr [ecx + 1f2h], dx
        pop    esi
        pop    ebp
        ret    4
sub_1_2640:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18dh], ecx
        mov    dword ptr [eax + 136h], edx
        mov    byte ptr [eax + 153h], cl
        mov    dword ptr [eax + 155h], ecx
        mov    byte ptr [eax + 135h], dl
        mov    byte ptr [eax + 134h], dl
        mov    dword ptr [eax + 15dh], ecx
        mov    dword ptr [eax + 161h], ecx
        mov    dword ptr [eax + 169h], ecx
        mov    dword ptr [eax + 16dh], ecx
        mov    dword ptr [eax + 171h], ecx
        mov    dword ptr [eax + 175h], ecx
        mov    dword ptr [eax + 179h], ecx
        mov    dword ptr [eax + 17dh], ecx
        mov    dword ptr [eax + 181h], ecx
        mov    dword ptr [eax + 185h], ecx
        mov    dword ptr [eax + 189h], ecx
        mov    dword ptr [eax + 191h], ecx
        mov    word ptr [eax + 22h], cx
        mov    dword ptr [eax + 195h], ecx
        pop    ebp
        mov    dword ptr [eax + 199h], ecx
        mov    dword ptr [eax + 159h], 12345678h
        mov    dword ptr [eax + 19dh], ecx
        ret    4
        db  08Dh, 049h, 000h
sub_1_26E0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 1eh], cx
        mov    dword ptr [eax + 142h], ecx
        mov    word ptr [eax + 22h], cx
        pop    ebp
        mov    dword ptr [eax + 146h], ecx
        mov    dword ptr [eax + 14ah], ecx
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2710:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 8]
        add    edi, 0eh
        cmp    byte ptr [ecx + 153h], 0
        jne    SHORT loc_1_2785
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_1_277A
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        inc    dword ptr [esi + 14h]
        push   edi
        test   edx, edx
        jne    SHORT loc_1_2760
        mov    ecx, dword ptr [ecx + 14ah]
        mov    edx, dword ptr [ebp + 14h]
        push   ecx
        mov    eax, dword ptr [ebp + 18h]
        push   edx
        push   0
        push   eax
        call   sub_1_2D0E
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebp
        ret    18h
loc_1_2760:
        mov    edi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        push   edi
        push   eax
        push   edx
        push   ecx
        call   sub_1_0E30
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebp
        ret    18h
loc_1_277A:
        mov    eax, 0c001000ch
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_2785:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_2790:
        db  0A0h, 0DCh, 02Fh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_27B5
        db  0FEh, 005h, 0DCh, 02Fh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_27B7
        push   0
        push   0
        call   sub_1_1E90
        cmp    eax, 0
        jne    SHORT loc_1_27B7
loc_1_27B5:
        clc
        ret
loc_1_27B7:
        stc
        ret
SPENDIS_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_27C8
        db  0FFh, 024h, 085h, 0CAh, 027h, 000h, 000h
loc_1_27C8:
        clc
        ret
        db  090h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h
        db  0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h
        db  0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h
        db  0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h
        db  0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h
        db  0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h
        db  0C8h, 027h, 000h, 000h, 0C8h, 027h, 000h, 000h, 090h, 027h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_2840:
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
        call   sub_1_2870
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_2870:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_2910
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_28B0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_28A5
        cmp    al, 0ffh
        jne    SHORT loc_1_28A5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_28A5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_2917
loc_1_28A5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_2917
        db  08Bh, 0FFh
loc_1_28B0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_2900
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_28D6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_28DD
loc_1_28D6:
        mov    dword ptr [ebp - 10h], 1
loc_1_28DD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_28F0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_291A
        db  08Dh, 064h, 024h, 000h
loc_1_28F0:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_291A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2900:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_291A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2910:
        mov    dword ptr [ebp - 4], 20h
loc_1_2917:
        mov    esi, dword ptr [ebp + 8]
loc_1_291A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_2A11
loc_1_2925:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    SHORT loc_1_297D
        test   al, 2
        je     SHORT loc_1_2969
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 096h, 0E0h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_2969
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ecx + eax*4], edx
        jne    SHORT loc_1_297D
loc_1_2969:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_2974
        test   bl, 4
        jne    SHORT loc_1_297D
loc_1_2974:
        test   bl, 20h
        je     NEAR PTR loc_1_2A06
loc_1_297D:
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
        call   sub_1_2BF2
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_2A06
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_29BF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_29BF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_29D0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_29D6
        db  08Bh, 0FFh
loc_1_29D0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_29D6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_29E5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2BF8
loc_1_29E5:
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
        call   sub_1_2C9C
        mov    edi, ebx
        jmp    SHORT loc_1_2A09
loc_1_2A06:
        mov    edi, dword ptr [edi + 10h]
loc_1_2A09:
        or     edi, edi
        jne    NEAR PTR loc_1_2925
loc_1_2A11:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2A20:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A30
        pop    ebp
        ret    4
sub_1_2A30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_2AAE
loc_1_2A40:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_2AA7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2BF8
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_2AA7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_2A6C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_2A6C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_2A80
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_2A86
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A80:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_2A86:
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
        call   sub_1_2C9C
        mov    edi, ebx
        jmp    SHORT loc_1_2AAA
loc_1_2AA7:
        mov    edi, dword ptr [edi + 10h]
loc_1_2AAA:
        or     edi, edi
        jne    SHORT loc_1_2A40
loc_1_2AAE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_2AD5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2AD5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_2B11
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_2B02
        cmp    bl, 0ffh
        jne    SHORT loc_1_2B02
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_2B02
        test   al, 8
        je     SHORT loc_1_2AFB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2AFB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_2B02:
        test   al, 4
        jne    SHORT loc_1_2B0A
        test   al, 2
        je     SHORT loc_1_2B29
loc_1_2B0A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2B11:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_2B29
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_2B29
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2B29:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_2B44:
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
sub_1_2B81:
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
sub_1_2BAA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_2BB0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_2BB6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_2BBC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_2BC2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_2BC8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_2BD4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_2BDA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_2BF2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_2BF8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_2BFE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_2C04:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
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
sub_1_2C72:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_2C78:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_2C7E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_2C84:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_2C90:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_2C96:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_2C9C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_2CC0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_2CE4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_2D08:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_2D0E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_2D14:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_2D1A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_2D20:
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
sub_1_2D68:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_2D7C:
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
sub_1_2D9C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_2DBC
        or     ecx, ecx
        je     SHORT loc_1_2DD2
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_2DD0
loc_1_2DBC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_2DD0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_2DD0:
        rep movsb
loc_1_2DD2:
        popfd
        pop    edi
        pop    esi
        ret    0ch
loc_1_2DD8:
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
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
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
        db  001h
loc_1_2E78:
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
loc_1_2ED4:
        push   eax
        inc    ebx
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        and    byte ptr [ecx], dh
        bound  ebp, qword ptr ss:[ecx + 74h]
        and    byte ptr [ebp + 74h], al
        push   656e7265h
        db  074h, 020h
        inc    ecx
        popad
        jo     SHORT loc_1_2F63
        jb     NEAR PTR loc_1_2EF2
loc_1_2EF2:
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax - 1], dl
loc_1_2EFC:
        push   ebx
        push   eax
        inc    ebp
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F04:
        db  005h, 000h, 005h, 000h, 000h
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
        add    byte ptr [edx + esi*2 + 61h], dl
        outsb
        db  073h, 063h
        imul   esi, dword ptr gs:[esi + 65h], 72540072h
        popad
        outsb
        db  073h, 063h
        imul   esi, dword ptr gs:[esi + 65h], 6f490072h
        inc    ebx
        push   656e6e61h
        insb
        push   edx
        popad
        jns    NEAR PTR loc_1_2F63
loc_1_2F63:
        add    byte ptr [ecx + 6fh], cl
        inc    ebx
        push   656e6e61h
        insb
        push   edx
        popad
        jns    NEAR PTR loc_1_2F73
loc_1_2F73:
        add    byte ptr [edx + 75h], al
        db  073h, 04Eh
        jne    SHORT loc_1_2FE7
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        db  073h, 04Eh
        jne    SHORT loc_1_2FF3
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 050h, 045h, 04Eh, 044h
        db  049h, 053h, 020h, 000h, 040h, 000h, 054h, 0B9h, 027h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h
        db  000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h
loc_1_2FE7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FF3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
