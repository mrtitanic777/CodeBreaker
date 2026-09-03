; EE16 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5934 instructions recovered by recursive descent

Declare_Virtual_Device EE16, 3, 10, EE16_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1e28  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0xf
        mov    cx, word ptr [esi]
        in     al, dx
        lea    edx, [ecx + 4]
        mov    ax, word ptr [ebp + 0x10]
        out    dx, ax
        mov    bx, word ptr [ebp + 0x14]
        mov    ax, bx
        shr    ax, 1
        and    eax, 0xffff
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        call   sub_1_0FA1
        test   bl, 1
        je     loc_1_0056
        mov    dx, word ptr [esi]
        in     al, dx
        mov    ecx, ebx
        and    ecx, 0xffff
        mov    byte ptr [edi + ecx - 1], al
loc_1_0056:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
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
        add    dx, 0xf
        mov    cx, word ptr [esi]
        in     al, dx
        lea    edx, [ecx + 2]
        mov    ax, word ptr [ebp + 0x10]
        out    dx, ax
        mov    bx, word ptr [ebp + 0x14]
        mov    ax, bx
        shr    ax, 1
        and    eax, 0xffff
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        call   sub_1_0F64
        test   bl, 1
        je     loc_1_00B5
        mov    eax, ebx
        and    eax, 0xffff
        mov    dx, word ptr [esi]
        mov    al, byte ptr [eax + edi - 1]
        out    dx, al
loc_1_00B5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0x143]
        mov    eax, ebx
        add    eax, 0x24
        mov    dword ptr [esi + 0x14b], ebx
        cmp    dword ptr [eax], 0
        jne    loc_1_00EC
        mov    dword ptr [esi + 0x147], 0
loc_1_00EC:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x143], eax
        lea    edi, [ebx + 0x24]
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        push   ebx
        call   sub_1_10E0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1104
        xor    edx, edx
        mov    dx, word ptr [esi + 0x1ed]
        cmp    dword ptr [ebp - 8], edx
        jae    loc_1_0156
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        je     loc_1_0156
        mov    ecx, eax
        cmp    ecx, edx
        jb     loc_1_0138
        mov    ecx, edx
loc_1_0138:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [esi + 0x34]
        push   eax
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        push   0
        push   ebx
        call   sub_1_01B0
        lea    eax, [esi + 0x34]
        mov    dword ptr [ebp - 4], eax
loc_1_0156:
        mov    edi, dword ptr [edi + 8]
        mov    byte ptr [esi + 0x134], 1
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe
        push   eax
        push   0xe
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x1e9]
        push   eax
        call   sub_1_0C60
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0FFA
        dec    dword ptr [edi + 0x14]
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
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_027A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10E0
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_027A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_0275
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_1_0200:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_022C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1128
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0275
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        jmp    loc_1_0270
loc_1_022C:
        or     edi, edi
        je     loc_1_0249
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_0241
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0270
loc_1_0241:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_0249:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_0257
        mov    ebx, eax
loc_1_0257:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        add    esi, ebx
        push   eax
        call   sub_1_119C
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_0270:
        cmp    esi, dword ptr [ebp + 0x10]
        jb     loc_1_0200
loc_1_0275:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_027A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_03B9
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_10E0
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_03B9
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 0x25]
        mov    cx, word ptr [esi]
        add    cx, 0xf
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 0x14], al
        mov    dx, word ptr [esi]
        add    dx, 4
        mov    eax, dword ptr [ebp + 0x18]
        add    ax, word ptr [ebp + 0xc]
        out    dx, ax
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_03B1
loc_1_0314:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0344
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1128
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_03B1
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1104
        jmp    loc_1_03A5
loc_1_0344:
        or     edi, edi
        je     loc_1_03B1
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_1_0353
        cmp    dword ptr [ebp + 0x18], edi
        ja     loc_1_03B1
loc_1_0353:
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        cmp    edi, dword ptr [ebp - 4]
        mov    ebx, edi
        jbe    loc_1_0366
        mov    ebx, dword ptr [ebp - 4]
loc_1_0366:
        cmp    dword ptr [ebp - 0x14], ebx
        jae    loc_1_036E
        mov    ebx, dword ptr [ebp - 0x14]
loc_1_036E:
        test   ebx, 0xfffffffe
        je     loc_1_038A
        mov    eax, ebx
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0FA1
loc_1_038A:
        test   bl, 1
        je     loc_1_039A
        mov    dx, word ptr [esi]
        in     al, dx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ebx + ecx - 1], al
loc_1_039A:
        add    dword ptr [ebp - 0xc], ebx
        add    dword ptr [ebp - 0x10], ebx
        sub    edi, ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_03A5:
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x10], ecx
        ja     loc_1_0314
loc_1_03B1:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], ecx
loc_1_03B9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
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
        mov    edi, dword ptr [ebp + 0xc]
        cmp    byte ptr [edi + 0x154], 0
        jne    loc_1_04F1
        inc    dword ptr [edi + 0x137]
        sub    ebx, ebx
loc_1_0411:
        mov    dx, word ptr [edi]
        sub    dx, 0x3ff8
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    word ptr [ebp - 2], 0xf000
        jne    loc_1_0433
        cmp    dword ptr [edi + 0x143], ebx
        je     loc_1_04C2
loc_1_0433:
        mov    si, word ptr [ebp - 2]
        and    si, 0x5000
        cmp    si, bx
        je     loc_1_0473
        push   edi
        call   sub_1_0710
        cmp    al, bl
        je     loc_1_0473
        mov    cx, word ptr [edi]
        mov    ax, si
        lea    edx, [ecx - 0x3ff6]
        and    word ptr [ebp - 2], 0xafff
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   edi
        call   sub_1_0500
        cmp    al, bl
        je     loc_1_04F1
loc_1_0473:
        mov    si, word ptr [ebp - 2]
        and    si, 0xa000
        cmp    si, bx
        je     loc_1_04AB
        push   edi
        call   sub_1_0710
        cmp    al, bl
        je     loc_1_04AB
        mov    cx, word ptr [edi]
        mov    ax, si
        lea    edx, [ecx - 0x3ff6]
        and    word ptr [ebp - 2], 0x5fff
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   edi
        call   sub_1_0A00
loc_1_04AB:
        cmp    dword ptr [edi + 0x143], ebx
        je     loc_1_0411
        push   edi
        call   sub_1_00C0
        jmp    loc_1_0411
loc_1_04C2:
        mov    dx, word ptr [edi]
        dec    dword ptr [edi + 0x137]
        add    dx, 7
        mov    al, byte ptr [edi + 8]
        or     al, 8
        out    dx, al
        cmp    byte ptr [edi + 0x134], 0
        je     loc_1_04F1
        mov    byte ptr [edi + 0x134], 0
        mov    eax, dword ptr [edi + 0x1e9]
        push   eax
        call   sub_1_0E40
loc_1_04F1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0500:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0xa000
loc_1_0511:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 0x1d]
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 0x400a]
        in     ax, dx
        test   di, ax
        je     loc_1_0660
        mov    cx, word ptr [esi]
        sub    ax, ax
        lea    edx, [ecx + 0x400a]
        out    dx, ax
        lea    edx, [ecx + 0x400c]
        mov    ax, 0x8000
        out    dx, ax
        lea    edx, [ecx + 0x4000]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    ax, 0x3fff
        add    ax, 0xe
        mov    word ptr [ebp - 2], ax
        inc    dword ptr [esi + 0x162]
        cmp    word ptr [ebp - 2], 0x3c
        jb     loc_1_0622
        cmp    word ptr [ebp - 2], 0x5ea
        ja     loc_1_0622
        mov    bx, word ptr [esi + 0x1ed]
        cmp    word ptr [ebp - 2], bx
        ja     loc_1_058E
        mov    bx, word ptr [ebp - 2]
loc_1_058E:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 0x1d]
        lea    edx, [ecx + 4]
        add    ax, 0x12
        out    dx, ax
        mov    ax, bx
        shr    ax, 1
        and    eax, 0xffff
        push   eax
        lea    eax, [esi + 0x34]
        push   eax
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        call   sub_1_0FA1
        test   bl, 1
        je     loc_1_05D0
        mov    dx, word ptr [esi]
        in     al, dx
        mov    ecx, ebx
        and    ecx, 0xffff
        mov    byte ptr [esi + ecx + 0x33], al
loc_1_05D0:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0x25], ax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        sub    eax, 0xe
        push   eax
        mov    eax, ebx
        and    eax, 0xffff
        sub    eax, 0xe
        push   eax
        lea    eax, [esi + 0x42]
        push   eax
        push   0xe
        lea    eax, [esi + 0x34]
        push   eax
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x1d]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [esi + 0x1e9]
        push   eax
        call   sub_1_0C60
        cmp    byte ptr [esi + 0x154], 0
        jne    loc_1_06AC
        mov    byte ptr [esi + 0x134], 1
loc_1_0622:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 0x1f]
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 0x400c]
        sub    ax, ax
        out    dx, ax
        mov    ax, word ptr [esi + 0x1d]
        mov    word ptr [esi + 0x1f], ax
        add    ax, 0x600
        cmp    word ptr [esi + 0x23], ax
        mov    word ptr [esi + 0x1d], ax
        jae    loc_1_0511
        mov    ax, word ptr [esi + 0x21]
        mov    word ptr [esi + 0x1d], ax
        jmp    loc_1_0511
loc_1_0660:
        mov    dx, word ptr [esi]
        sub    dx, 0x3ff8
        in     ax, dx
        test   al, 0x40
        jne    loc_1_06A1
        mov    dx, word ptr [esi]
        sub    dx, 0x3ff2
        mov    ax, word ptr [esi + 0x1d]
        add    ax, 0xa
        out    dx, ax
        push   esi
        call   sub_1_0710
        mov    cx, word ptr [esi]
        mov    ax, 0x10
        lea    edx, [ecx - 0x3ff6]
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
        mov    esi, dword ptr [ebp + 0xc]
        cmp    word ptr [esi + 0x1b], 0
        je     loc_1_06EC
        mov    ax, word ptr [esi + 0x1ef]
        dec    ax
        mov    word ptr [esi + 0x1ef], ax
        jne    loc_1_06EC
        push   esi
        call   sub_2_0E40
        inc    dword ptr [0x1cdc]            ; -> loc_1_1CDC
loc_1_06EC:
        push   0x7d0
        add    esi, 0x1ce
        push   esi
        call   sub_1_1092
        pop    esi
        pop    ebp
        ret    0x10
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
        sub    dx, 0x3ff6
        in     ax, dx
        cmp    ax, bx
        je     loc_1_074D
        push   0x14
        call   sub_1_10B0
        mov    ax, di
        inc    di
        cmp    ax, 0x1388
        jne    loc_1_0721
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
        cmp    byte ptr [edi + 0x154], 0
        jne    loc_1_0834
        mov    ax, word ptr [edi + 0xf]
        cmp    word ptr [edi + 0x1b], ax
        jae    loc_1_07F0
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_07E0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        push   esi
        push   edi
        call   sub_1_0840
        cmp    eax, 0x103
        jne    loc_1_07CD
        mov    dword ptr [ebx + 0x2c], esi
        lea    ecx, [ebx + 0x24]
        inc    dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x143], 0
        jne    loc_1_07B5
        mov    dword ptr [edi + 0x143], ebx
        jmp    loc_1_07BE
loc_1_07B5:
        mov    edx, dword ptr [edi + 0x147]
        mov    dword ptr [edx + 0x24], ebx
loc_1_07BE:
        mov    dword ptr [ecx], 0
        mov    dword ptr [edi + 0x147], ebx
        dec    dword ptr [esi + 0x14]
loc_1_07CD:
        mov    word ptr [edi + 0x1ef], 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_07E0:
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_07F0:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 0x2c], esi
        lea    eax, [ebx + 0x24]
        inc    dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x13b], 0
        jne    loc_1_0810
        mov    dword ptr [edi + 0x13b], ebx
        jmp    loc_1_0819
        db  08Dh, 049h, 000h
loc_1_0810:
        mov    ecx, dword ptr [edi + 0x13f]
        mov    dword ptr [ecx + 0x24], ebx
loc_1_0819:
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 0x13f], ebx
        inc    dword ptr [esi + 0x14]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_0834:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
sub_1_0840:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_10E0
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_09E8
        cmp    dword ptr [ebp - 0x10], 0x5ea
        ja     loc_1_09E8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1104
        mov    esi, dword ptr [ebp + 8]
        push   3
        mov    di, word ptr [esi]
        mov    cx, word ptr [esi + 0x15]
        lea    edx, [edi + 2]
        add    cx, 0x16
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        call   sub_1_0F64
        push   6
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_119C
        mov    di, 0xc
        sub    dword ptr [ebp - 4], 0xc
        mov    bx, word ptr [ebp - 0x14]
        add    dword ptr [ebp - 8], 0xc
        or     bx, bx
        je     loc_1_0939
loc_1_08D5:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0911
        test   dword ptr [ebp - 4], 0xfffffffe
        je     loc_1_08F9
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
        je     loc_1_090D
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        mov    al, byte ptr [eax + ecx - 1]
        out    dx, al
loc_1_090D:
        add    di, word ptr [ebp - 4]
loc_1_0911:
        cmp    bx, 1
        jbe    loc_1_0935
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1128
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1104
loc_1_0935:
        dec    bx
        jne    loc_1_08D5
loc_1_0939:
        cmp    di, 0x3c
        jae    loc_1_0956
        mov    cx, 0x3c
        sub    cx, di
        je     loc_1_0952
        mov    dx, word ptr [esi]
loc_1_094B:
        sub    al, al
        out    dx, al
        dec    cx
        jne    loc_1_094B
loc_1_0952:
        mov    di, 0x3c
loc_1_0956:
        mov    cx, word ptr [esi + 0x15]
        lea    eax, [ecx + 0x600]
        cmp    word ptr [esi + 0x19], ax
        mov    word ptr [esi + 0x15], ax
        jae    loc_1_0972
        mov    ax, word ptr [esi + 0x17]
        mov    word ptr [esi + 0x15], ax
loc_1_0972:
        mov    bx, word ptr [esi]
        mov    ax, word ptr [esi + 0x15]
        lea    edx, [ebx + 8]
        out    dx, ax
        lea    edx, [ebx + 0x4004]
        out    dx, ax
        lea    edx, [ebx + 8]
        mov    ax, cx
        out    dx, ax
        lea    edx, [ebx + 0x4006]
        lea    eax, [edi - 0xe]
        inc    word ptr [esi + 0x1b]
        or     ax, 0x8000
        inc    dword ptr [esi + 0x15e]
        out    dx, ax
        lea    eax, [ecx + 0xe]
        lea    edx, [ebx + 0x4004]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 0x43], 2
        jne    loc_1_09DD
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x1e9]
        push   eax
        call   sub_1_0EE0
        or     al, al
        je     loc_1_09DD
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_09DD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_09E8:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
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
        cmp    word ptr [esi + 0x1b], 0
        je     loc_1_0B1E
        nop    
loc_1_0A20:
        mov    ax, word ptr [ebp - 2]
        dec    word ptr [ebp - 2]
        or     ax, ax
        je     loc_1_0B1E
        mov    di, word ptr [esi]
        mov    ax, word ptr [esi + 0x13]
        lea    edx, [edi + 8]
        out    dx, ax
        lea    edx, [edi + 0x400e]
        in     ax, dx
        test   ah, 0xa0
        je     loc_1_0B13
        mov    dx, word ptr [esi]
        dec    word ptr [esi + 0x1b]
        add    dx, 0x400e
        sub    ax, ax
        out    dx, ax
        mov    word ptr [esi + 0x1ef], 3
        mov    ax, word ptr [esi + 0x13]
        add    ax, 0x600
        cmp    word ptr [esi + 0x19], ax
        mov    word ptr [esi + 0x13], ax
        jae    loc_1_0A82
        mov    ax, word ptr [esi + 0x17]
        mov    word ptr [esi + 0x13], ax
loc_1_0A82:
        mov    ebx, dword ptr [esi + 0x13b]
        or     ebx, ebx
        je     loc_1_0B11
        lea    edi, [ebx + 0x24]
        cmp    dword ptr [edi], 0
        jne    loc_1_0AA2
        mov    dword ptr [esi + 0x13f], 0
loc_1_0AA2:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x13b], eax
        lea    eax, [edi + 8]
        mov    dword ptr [ebp - 8], eax
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        push   esi
        call   sub_1_0840
        cmp    eax, 0x103
        mov    eax, dword ptr [ebp - 8]
        jne    loc_1_0AF7
        mov    eax, dword ptr [eax]
        inc    dword ptr [eax + 0x14]
        cmp    dword ptr [esi + 0x143], 0
        jne    loc_1_0AE0
        mov    dword ptr [esi + 0x143], ebx
        jmp    loc_1_0AE9
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0AE0:
        mov    eax, dword ptr [esi + 0x147]
        mov    dword ptr [eax + 0x24], ebx
loc_1_0AE9:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x147], ebx
        jmp    loc_1_0B08
loc_1_0AF7:
        mov    edi, dword ptr [eax]
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0FFA
        dec    dword ptr [edi + 0x14]
loc_1_0B08:
        mov    word ptr [esi + 0x1ef], 3
loc_1_0B11:
        mov    cl, 1
loc_1_0B13:
        cmp    word ptr [esi + 0x1b], 0
        jne    loc_1_0A20
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
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 8]
        add    edi, 0xe
        cmp    byte ptr [ecx + 0x154], 0
        jne    loc_1_0BA5
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_0B9A
        inc    dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 0xc]
        or     edx, edx
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        push   edi
        jne    loc_1_0B80
        mov    eax, dword ptr [ecx + 0x14b]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_112E
        dec    dword ptr [esi + 0x14]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_0B80:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0290
        dec    dword ptr [esi + 0x14]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_0B9A:
        mov    eax, 0xc001000c
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_0BA5:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_0BB0:
        mov    al, byte ptr [0x1248]         ; -> obj1:0x1248
        cmp    al, 0
        jne    loc_1_0BD5
        inc    byte ptr [0x1248]             ; -> obj1:0x1248
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0BD7
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_0BD7
loc_1_0BD5:
        clc    
        ret    
loc_1_0BD7:
        stc    
        ret    
EE16_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0BE8
        jmp    dword ptr [eax*4 + 0xbea]     ; -> obj1:0x0BEA
loc_1_0BE8:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0BB0
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BE8
        dd  loc_1_0BB0
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0C60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0C90
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_0D30
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_0CD0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_0CC5
        cmp    al, 0xff
        jne    loc_1_0CC5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_0CC5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_0D37
loc_1_0CC5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0D37
        db  08Bh, 0FFh
loc_1_0CD0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_0D20
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0CF6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_0CFD
loc_1_0CF6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_0CFD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0D10
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_0D3A
        db  08Dh, 064h, 024h, 000h
loc_1_0D10:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_0D3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D20:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_0D3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D30:
        mov    dword ptr [ebp - 4], 0x20
loc_1_0D37:
        mov    esi, dword ptr [ebp + 8]
loc_1_0D3A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0E31
loc_1_0D45:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_0D9D
        test   al, 2
        je     loc_1_0D89
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2810
        or     al, al
        je     loc_1_0D89
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_0D9D
loc_1_0D89:
        test   byte ptr [ebp - 4], 2
        je     loc_1_0D94
        test   bl, 4
        jne    loc_1_0D9D
loc_1_0D94:
        test   bl, 0x20
        je     loc_1_0E26
loc_1_0D9D:
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_1012
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0E26
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_0DDF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_0DDF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0DF0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0DF6
        db  08Bh, 0FFh
loc_1_0DF0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_0DF6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0E05
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1018
loc_1_0E05:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_10BC
        mov    edi, ebx
        jmp    loc_1_0E29
loc_1_0E26:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0E29:
        or     edi, edi
        jne    loc_1_0D45
loc_1_0E31:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
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
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0ECE
loc_1_0E60:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0EC7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1018
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0EC7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_0E8C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_0E8C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0EA0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0EA6
        db  005h, 000h, 000h, 000h, 000h
loc_1_0EA0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_0EA6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_10BC
        mov    edi, ebx
        jmp    loc_1_0ECA
loc_1_0EC7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0ECA:
        or     edi, edi
        jne    loc_1_0E60
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
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_0EF5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0EF5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_0F31
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_0F22
        cmp    bl, 0xff
        jne    loc_1_0F22
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_0F22
        test   al, 8
        je     loc_1_0F1B
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
        jne    loc_1_0F2A
        test   al, 2
        je     loc_1_0F49
loc_1_0F2A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0F31:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0F49
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_0F49
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
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h
sub_1_0FA1:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_0FCA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_0FD0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_0FD6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_0FDC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_0FE2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_0FE8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0FF4:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_0FFA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_1012:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_1018:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_101E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_1024:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_1030:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_103C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_1092:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_1098:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_109E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_10A4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_10B0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_10B6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_10BC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_10E0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_1104:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_1128:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_112E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_1134:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_113A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_1140:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_1146:
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_1188:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_119C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
sub_1_11BC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_11DC
        or     ecx, ecx
        je     loc_1_11F2
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_11F0
loc_1_11DC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_11F0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_11F0:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_11F2:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'EE16    '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  EE16_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1250:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 8]
        cmp    byte ptr [edx + 0x154], 0
        jne    loc_1_12D0
        cmp    byte ptr [ecx + 0x18], 0
        jne    loc_1_12C2
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_1282
        cmp    eax, 1
        je     loc_1_12A2
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    8
loc_1_1282:
        lea    eax, [esi + 0x24]
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   ecx
        push   edx
        call   sub_1_14F0
        pop    esi
        pop    ebp
        ret    8
loc_1_12A2:
        lea    eax, [esi + 0x24]
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   ecx
        push   edx
        call   sub_1_1320
        pop    esi
        pop    ebp
        ret    8
loc_1_12C2:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_12D0:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_12E0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x10], 2
        je     loc_1_12F5
        mov    eax, 0xc00000bb
        pop    ebp
        ret    8
loc_1_12F5:
        lea    eax, [ecx + 0x24]
        push   eax
        lea    eax, [ecx + 0x20]
        push   eax
        mov    eax, dword ptr [ecx + 0x1c]
        push   eax
        mov    eax, dword ptr [ecx + 0x18]
        push   eax
        mov    eax, dword ptr [ecx + 0x14]
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
        mov    esi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010e
        mov    dword ptr [esi], 0
        je     loc_1_1367
        cmp    eax, 0x1010f
        je     loc_1_13C0
        cmp    eax, 0x10112
        je     loc_1_1447
        cmp    eax, 0x1010103
        je     loc_1_1492
        mov    esi, 0xc0010017
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1367:
        mov    edi, dword ptr [ebp + 0x18]
        cmp    edi, 4
        je     loc_1_1382
        mov    dword ptr [esi], 4
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1382:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_119C
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0x1e9]
        push   eax
        call   sub_2_2630
        mov    esi, eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_13C0:
        sub    esi, esi
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_13E0
        mov    eax, dword ptr [ebp + 0x1c]
        mov    esi, 0xc0010014
        mov    dword ptr [eax], 0
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_13E0:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_119C
        cmp    dword ptr [ebp - 4], 0x100
        ja     loc_1_1438
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x1ed]
        cmp    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        jae    loc_1_1427
        mov    word ptr [ecx + 0x1ed], ax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x41], ax
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1427:
        mov    ecx, dword ptr [ebp + 0xc]
        pop    edi
        mov    word ptr [ecx + 0x41], ax
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1438:
        mov    esi, 0xc0000001
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1447:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_146B
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [esi], 4
        mov    dword ptr [eax], 0
        mov    esi, 0xc0010014
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_146B:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x43
        push   ecx
        call   sub_1_119C
        mov    eax, dword ptr [ebp + 0x1c]
        sub    esi, esi
        mov    dword ptr [eax], 4
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1492:
        mov    ecx, 6
        mov    edi, dword ptr [ebp + 0x18]
        mov    eax, edi
        cdq    
        idiv   ecx
        or     edx, edx
        je     loc_1_14B2
        mov    dword ptr [esi], ecx
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_14B2:
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, 6
        mov    eax, edi
        cdq    
        idiv   ecx
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0x1e9]
        push   eax
        call   sub_2_2350
        mov    esi, eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h
sub_1_14F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 8], 0
        lea    esi, [ebp - 8]
        mov    word ptr [ebp - 2], 0
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0xc], 0
        mov    ecx, 4
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        mov    eax, edx
        cmp    eax, 0x1010101
        ja     loc_1_1553
        je     loc_1_18F9
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_1574
        shl    eax, 2
        jmp    loc_1_1A24
loc_1_1553:
        cmp    eax, 0x1010102
        je     loc_1_1920
        cmp    eax, 0x1010103
        je     loc_1_1945
        cmp    eax, 0x1010104
        je     loc_1_19D7
loc_1_1574:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_1586
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_19DE
loc_1_1586:
        cmp    edx, 0x1020102
        ja     loc_1_1600
        je     loc_1_1723
        cmp    edx, 0x20105
        ja     loc_1_15D7
        je     loc_1_16E4
        cmp    edx, 0x20101
        je     loc_1_1664
        cmp    edx, 0x20102
        je     loc_1_1675
        cmp    edx, 0x20103
        je     loc_1_1686
        cmp    edx, 0x20104
        je     loc_1_16A9
        jmp    loc_1_1658
loc_1_15D7:
        cmp    edx, 0x2020d
        je     loc_1_16F4
        cmp    edx, 0x2020e
        je     loc_1_1704
        cmp    edx, 0x1020101
        je     loc_1_1715
        jmp    loc_1_1658
        db  08Dh, 049h, 000h
loc_1_1600:
        cmp    edx, 0x1020203
        ja     loc_1_1634
        je     loc_1_1767
        cmp    edx, 0x1020103
        je     loc_1_1734
        cmp    edx, 0x1020201
        je     loc_1_1745
        cmp    edx, 0x1020202
        je     loc_1_1756
        jmp    loc_1_1658
loc_1_1634:
        cmp    edx, 0x1020204
        je     loc_1_1775
        cmp    edx, 0x1020205
        je     loc_1_1786
        cmp    edx, 0x1020206
        je     loc_1_1797
loc_1_1658:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_19DE
loc_1_1664:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x15e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1675:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x162]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1686:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16a]
        add    eax, dword ptr [edi + 0x16e]
        add    eax, dword ptr [edi + 0x172]
        add    eax, dword ptr [edi + 0x176]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_16A9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0x2c]
        mov    edx, dword ptr [edi + 0x186]
        add    edx, dword ptr [edi + 0x18a]
        and    eax, 0xffff
        add    edx, eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x2a]
        add    edx, eax
        mov    eax, dword ptr [ebp - 0x28]
        and    eax, 0xffff
        add    edx, eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x26]
        add    edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_19DE
loc_1_16E4:
        mov    eax, dword ptr [ebp - 0x28]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_16F4:
        mov    eax, dword ptr [ebp - 0x2c]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1704:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x166]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1715:
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x2a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1723:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1734:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x182]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1745:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1756:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1767:
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x26]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1775:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x172]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1786:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x176]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1797:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_17A8:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_17C0
        mov    esi, 0x1ce0                   ; -> loc_1_1CE0
        mov    ecx, 0x9c
        jmp    loc_1_19DE
        db  08Dh, 049h, 000h
loc_1_17C0:
        mov    esi, 0x1d7c                   ; -> obj1:0x1D7C
        mov    ecx, 0x58
        jmp    loc_1_19DE
        db  090h
loc_1_17D0:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_17E0:
        lea    esi, [ebp - 0x18]
        mov    ecx, 4
        jmp    loc_1_19DE
        db  08Dh, 049h, 000h
loc_1_17F0:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_1800:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_1810:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_19DE
        db  08Dh, 064h, 024h, 000h
loc_1_1820:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0xf]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1841:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x11]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1862:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_19DE
        db  08Bh, 0FFh
loc_1_1870:
        mov    esi, 0x1dd4                   ; -> loc_1_1DD4
        mov    ecx, 0x16
        jmp    loc_1_19DE
        db  090h
loc_1_1880:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        je     loc_1_1895
        mov    eax, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_1895:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x1e9]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_18A9:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        jne    loc_1_18C4
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x1ed]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_18C4:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x41]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_19DE
loc_1_18D2:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    ecx, 2
        jmp    loc_1_19DE
loc_1_18E5:
        mov    dword ptr [ebp - 8], 0xe
        lea    esi, [ebp - 8]
        mov    ecx, 4
        jmp    loc_1_19DE
loc_1_18F9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x28]
        add    edi, 0x28
        mov    dword ptr [ebp - 0x24], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x20], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 0x24]
        mov    byte ptr [ebp - 0x1f], al
        mov    ecx, 6
        jmp    loc_1_19DE
        db  08Bh, 0FFh
loc_1_1920:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x2e]
        add    edi, 0x2e
        mov    dword ptr [ebp - 0x24], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x20], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 0x24]
        mov    byte ptr [ebp - 0x1f], al
        mov    ecx, 6
        jmp    loc_1_19DE
loc_1_1945:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        jne    loc_1_1980
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 0x1e9]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_2_27B0
        mov    eax, dword ptr [ebp - 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_19DE
loc_1_1980:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x1e9]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_26F0
        cmp    dword ptr [ebp - 0x14], 0
        mov    esi, dword ptr [ebp + 0x14]
        jne    loc_1_19BB
        mov    eax, dword ptr [ebp - 0x10]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_19DE
loc_1_19BB:
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x1e9]
        push   eax
        call   sub_2_26B0
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_19DE
loc_1_19D7:
        mov    dword ptr [ebp - 8], 0x10
loc_1_19DE:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1A15
        cmp    dword ptr [ebp + 0x18], ecx
        jae    loc_1_1A01
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010016
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1A01:
        mov    eax, dword ptr [ebp + 0x1c]
        or     ecx, ecx
        mov    dword ptr [eax], ecx
        je     loc_1_1A15
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_119C
loc_1_1A15:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 049h, 000h
loc_1_1A24:
        test   al, 0x17
        add    byte ptr [eax], al
        rcl    byte ptr [edi]
        add    byte ptr [eax], al
        loopne loc_1_1A45
        add    byte ptr [eax], al
        loopne loc_1_1A49
        add    byte ptr [eax], al
        dd  loc_1_17F0               ; fixup
        dd  loc_1_1800               ; fixup
        dd  loc_1_1810               ; fixup
        dd  loc_1_1820               ; fixup
        dd  loc_1_1841               ; fixup
loc_1_1A45:
        sbb    byte ptr [eax], al
        db  000h
        dd  loc_1_1862               ; fixup
loc_1_1A49:
        sbb    byte ptr [eax], al
        add    byte ptr [edx + 0x18], ah     ; -> loc_1_1862
        add    byte ptr [eax], al
        je     0x1a67
        add    byte ptr [eax], al
        jo     loc_1_1A6E
        add    byte ptr [eax], al
        sbb    byte ptr [eax], 0
        add    byte ptr [ecx - 0x2dffffe8], ch ; -> loc_1_18A9
        sbb    byte ptr [eax], al
        add    byte ptr [edx + 0x18], ah     ; -> loc_1_1862
        add    byte ptr [eax], al
        je     0x1a7f
        add    byte ptr [eax], al
        in     eax, 0x18
loc_1_1A6E:
        add    byte ptr [eax], al
loc_1_1A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax + 8]
        sub    esi, esi
        cmp    byte ptr [ecx + 0x154], 0
        je     loc_1_1A91
        mov    esi, 0xc001000d
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_1A91:
        mov    dl, byte ptr [ecx + 0x1f9]
        mov    eax, dword ptr [ebp + 0xc]
        test   al, 0x24
        je     loc_1_1AAA
        mov    al, dl
        or     al, 1
        mov    byte ptr [ecx + 0x1f9], al
        jmp    loc_1_1ABF
loc_1_1AAA:
        test   al, 8
        je     loc_1_1AB8
        mov    al, dl
        and    al, 0xfd
        mov    byte ptr [ecx + 0x1f9], al
loc_1_1AB8:
        and    byte ptr [ecx + 0x1f9], 0xfe
loc_1_1ABF:
        cmp    byte ptr [ecx + 0x1f9], dl
        je     loc_1_1AD3
        push   ecx
        call   sub_2_13C0
        or     eax, eax
        je     loc_1_1AD3
        sub    esi, esi
loc_1_1AD3:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1AE0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x154], 0
        je     loc_1_1B00
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
        db  005h, 000h, 000h, 000h, 000h
loc_1_1B00:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x10
        jbe    loc_1_1B11
        mov    eax, 0xc000009a
        pop    ebp
        ret    0x1c
loc_1_1B11:
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        push   eax
        push   edx
        call   sub_2_1460
        sub    eax, eax
        pop    ebp
        ret    0x1c
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
        dec    dword ptr [edi + 0x14]
        push   esi
        call   sub_2_0AB0
        push   0
        push   0
        push   edi
        call   sub_1_10BC
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0FF4
        cmp    dword ptr [esi + 0x18e], 0
        jne    loc_1_1B7C
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
        jo     0x1ba0
        pushal 
        add    dl, byte ptr [eax + 2]
        inc    eax
        add    dh, byte ptr [eax]
        add    ah, byte ptr [eax]
        add    dl, byte ptr [eax]
        add    al, byte ptr [eax]
        add    dh, byte ptr [eax + 3]
        pushal 
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
        add    eax, 0xb000a00
        add    byte ptr [edi], cl
        add    byte ptr [ebp + 0x65], ah
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1BD4:
        xor    dword ptr gs:[esi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        jne    0x1c52
        push   esp
        jns    0x1c52
        add    byte ptr gs:[edx + 0x75], al
        jae    0x1c3c
        jns    0x1c5a
        add    byte ptr gs:[ecx + 0x52], cl
        push   ecx
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 0x61], dl
        outsb  dx, byte ptr [esi]
        jae    0x1c75
        imul   esi, dword ptr gs:[esi + 0x65], 0x72540072
        popal  
        outsb  dx, byte ptr [esi]
        jae    loc_1_1C81
        imul   esi, dword ptr gs:[esi + 0x65], 0x4f490072
        inc    ebx
        dec    eax
        push   edx
        inc    esp
        pop    ecx
        add    byte ptr [ecx + 0x4f], cl
        inc    ebx
        dec    eax
        push   edx
        inc    esp
        pop    ecx
        add    byte ptr [edi + esi*2 + 0x69], dl
        jae    loc_1_1CAD
        sub    eax, 0x72696150
        and    byte ptr [eax], ch
        push   esp
        push   eax
        inc    ebp
        sub    dword ptr [eax], eax
        add    byte ptr [edi + esi*2 + 0x69], dl
        jae    0x1cc1
        sub    eax, 0x72696150
        and    byte ptr [eax], ch
        push   esp
        push   eax
        inc    ebp
        sub    dword ptr [eax], eax
        add    byte ptr [eax + ebp*2 + 0x69], dl
        arpl   word ptr [ebx + 0x20], bp
        dec    esi
        je     0x1c86
        sub    byte ptr [ecx + 0x55], al
        dec    ecx
        das    
        inc    esp
        dec    ecx
        pop    eax
        sub    dword ptr [eax], eax
loc_1_1C70:
        push   esp
        push   0x206b6369
        dec    esi
        je     0x1c9a
        sub    byte ptr [ecx + 0x55], al
        dec    ecx
        das    
        inc    esp
        dec    ecx
loc_1_1C81:
        pop    eax
        sub    dword ptr [eax], eax
        push   esp
        push   0x4e206e69
        je     loc_1_1CAD
        sub    byte ptr [edx + 0x4e], al
        inc    ebx
        das    
        inc    ebx
        dec    edi
        inc    ecx
        pop    eax
        sub    dword ptr [eax], eax
loc_1_1C98:
        push   esp
        push   0x4e206e69
        je     0x1cc1
        sub    byte ptr [edx + 0x4e], al
        inc    ebx
        das    
        inc    ebx
        dec    edi
        inc    ecx
        pop    eax
        sub    dword ptr [eax], eax
        inc    ebp
loc_1_1CAD:
        popal  
        jb     0x1d1c
        jns    loc_1_1CB2
loc_1_1CB2:
        add    byte ptr [eax], al
loc_1_1CB4:
        inc    ebp
        popal  
        jb     0x1d24
        jns    loc_1_1CBA
loc_1_1CBA:
        add    byte ptr [eax], al
        dec    esp
        popal  
        je     loc_1_1D25
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1CC4:
        dec    esp
        popal  
        je     loc_1_1D2D
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        jbe    0x1d35
        jb     loc_1_1CD2
loc_1_1CD2:
        add    byte ptr [eax], al
loc_1_1CD4:
        dec    esi
        jbe    loc_1_1D3D
        jb     loc_1_1CDA
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
        add    eax, 0x6000101
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
        add    byte ptr [0x10000101], cl
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
        add    eax, 0xd000201
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
        add    eax, 0x6010202
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
        add    eax, 0x6000101
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
        add    byte ptr [0x10000101], cl
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
        outsb  dx, byte ptr [esi]
        je     0x1e3d
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x74], al
        push   0x78457265
        jo     0x1e55
        jae    0x1e59
        and    byte ptr [ecx], dh
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    edx, dword ptr [eax]
        add    esp, dword ptr [eax]
        add    esi, dword ptr [eax]
        add    eax, dword ptr [eax + 3]
        push   eax
        add    esp, dword ptr [eax + 3]
        jo     0x1dff
        add    byte ptr [edx], al
        adc    byte ptr [edx], al
        and    byte ptr [edx], al
        xor    byte ptr [edx], al
        inc    eax
        add    dl, byte ptr [eax + 2]
        pushal 
        add    dh, byte ptr [eax + 2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x28d0  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        mov    word ptr [ebp - 0x18], 5
        mov    word ptr [ebp - 0x16], 5
        mov    dword ptr [ebp - 0x14], 0x1bd4 ; -> loc_1_1BD4
        mov    word ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 0xe], 1
        mov    word ptr [ebp - 0xc], 0x628b
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0FCA
        lea    eax, [ebp - 0x10]
        mov    dword ptr [0x1b98], eax       ; -> obj1:0x1B98
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x1e18], eax       ; -> obj1:0x1E18
        mov    byte ptr [ebp - 0x50], 3
        mov    byte ptr [ebp - 0x4f], 0
        mov    dword ptr [ebp - 0x4a], 0x870 ; -> loc_2_0870
        mov    dword ptr [ebp - 0x46], 0x9f0 ; -> loc_2_09F0
        mov    dword ptr [ebp - 0x36], 0x1250 ; -> loc_1_1250
        mov    dword ptr [ebp - 0x42], 0x760 ; -> loc_1_0760
        mov    dword ptr [ebp - 0x3e], 0xb30 ; -> loc_1_0B30
        mov    dword ptr [ebp - 0x3a], 0xc50 ; -> loc_2_0C50
        mov    dword ptr [ebp - 0x32], 0x12e0 ; -> loc_1_12E0
        mov    dword ptr [ebp - 0x2e], 0xa80 ; -> loc_2_0A80
        mov    dword ptr [ebp - 0x2a], 0x100 ; -> loc_2_0100
        mov    dword ptr [ebp - 0x26], 0x590 ; -> loc_2_0590
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x22], eax
        lea    eax, [ebp - 0x50]
        mov    dword ptr [ebp - 0x1e], ecx
        mov    dword ptr [0x1e20], 0x1e1c    ; -> obj1:0x1E20
        mov    dword ptr [0x1e1c], 0x1e1c    ; -> obj1:0x1E1C
        push   0x36
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   0x1e14                        ; -> obj1:0x1E14
        push   eax
        call   sub_1_0FD6
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_00F4
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0FD0
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_00F4:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_2_0100:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x7c
        push   esi
        push   edi
        mov    word ptr [ebp - 0x7c], 8
        mov    word ptr [ebp - 0x7a], 8
        mov    dword ptr [ebp - 0x78], 0x1be4 ; -> obj1:0x1BE4
        mov    word ptr [ebp - 0x6c], 4
        mov    word ptr [ebp - 0x6a], 4
        mov    dword ptr [ebp - 0x68], 0x1bf0 ; -> obj1:0x1BF0
        mov    word ptr [ebp - 0x74], 0xa
        mov    word ptr [ebp - 0x72], 0xa
        mov    dword ptr [ebp - 0x70], 0x1c00 ; -> obj1:0x1C00
        mov    word ptr [ebp - 0x64], 0xc
        mov    word ptr [ebp - 0x62], 0xc
        mov    dword ptr [ebp - 0x60], 0x1c18 ; -> obj1:0x1C18
        mov    word ptr [ebp - 0x44], 8
        mov    word ptr [ebp - 0x42], 8
        mov    dword ptr [ebp - 0x40], 0x1c2c ; -> obj1:0x1C2C
        mov    word ptr [ebp - 0x54], 0x13
        mov    word ptr [ebp - 0x52], 0x13
        mov    dword ptr [ebp - 0x50], 0x1c48 ; -> obj1:0x1C48
        mov    word ptr [ebp - 0x5c], 0x14
        mov    word ptr [ebp - 0x5a], 0x14
        mov    dword ptr [ebp - 0x58], 0x1c70 ; -> loc_1_1C70
        mov    word ptr [ebp - 0x4c], 0x14
        mov    word ptr [ebp - 0x4a], 0x14
        mov    dword ptr [ebp - 0x48], 0x1c98 ; -> loc_1_1C98
        mov    word ptr [ebp - 0x3c], 6
        mov    word ptr [ebp - 0x3a], 6
        mov    dword ptr [ebp - 0x38], 0x1cb4 ; -> loc_1_1CB4
        mov    word ptr [ebp - 0x34], 5
        mov    word ptr [ebp - 0x32], 5
        mov    dword ptr [ebp - 0x30], 0x1cc4 ; -> loc_1_1CC4
        mov    word ptr [ebp - 0x2c], 6
        mov    word ptr [ebp - 0x2a], 6
        mov    dword ptr [ebp - 0x28], 0x1cd4 ; -> loc_1_1CD4
        mov    eax, dword ptr [0x1b94]       ; -> obj1:0x1B94
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x25d
        push   eax
        call   sub_1_10B6
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_2_0203
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0203:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0x97
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [0x1e14]       ; -> obj1:0x1E14
        mov    dword ptr [ecx + 0x1ae], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x155], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x27], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x192
        mov    dword ptr [ecx + 0x196], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x196]
        mov    dword ptr [ecx + 0x192], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x19a
        mov    dword ptr [ecx + 0x19e], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x19e]
        mov    dword ptr [ecx + 0x19a], eax
        lea    eax, [ebp - 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1134
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_02A6
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10BC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_02A6:
        push   1
        lea    eax, [ebp - 0x7c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_113A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_02D1
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 2], eax
        jmp    loc_2_02DB
loc_2_02D1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2], 2
loc_2_02DB:
        push   1
        lea    eax, [ebp - 0x74]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_113A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0306
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ecx], ax
        jmp    loc_2_030E
loc_2_0306:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 0x310
loc_2_030E:
        push   0
        lea    eax, [ebp - 0x6c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_113A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0340
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ecx + 0x1e7], ax
        jmp    loc_2_034C
        db  08Dh, 049h, 000h
loc_2_0340:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1e7], 5
loc_2_034C:
        mov    eax, dword ptr [ebp - 4]
        mov    cl, 0x1f
        sub    cl, byte ptr [eax + 0x1e7]
        mov    byte ptr [eax + 0x1e6], cl
        push   2
        lea    eax, [ebp - 0x64]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_113A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_03E7
        push   1
        lea    eax, [ebp - 0x5c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_1146
        or     al, al
        je     loc_2_039B
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 9], 1
        jmp    loc_2_03E7
loc_2_039B:
        push   1
        lea    eax, [ebp - 0x54]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_1146
        or     al, al
        je     loc_2_03C0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 9], 3
        jmp    loc_2_03E7
        db  08Dh, 064h, 024h, 000h
loc_2_03C0:
        push   1
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_1146
        or     al, al
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_03E1
        mov    word ptr [eax + 9], 2
        jmp    loc_2_03E7
loc_2_03E1:
        mov    word ptr [eax + 9], 0xffff
loc_2_03E7:
        push   2
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_113A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0465
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_1146
        or     al, al
        je     loc_2_0425
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0xb], 1
        jmp    loc_2_0465
loc_2_0425:
        push   1
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_1146
        or     al, al
        je     loc_2_0446
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0xb], 2
        jmp    loc_2_0465
loc_2_0446:
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_1146
        or     al, al
        je     loc_2_0465
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0xb], 3
loc_2_0465:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_103C
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_04A1
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_04A1
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, ecx
        add    edx, 0x2e
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_2_04A1:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1140
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 2], 3
        jne    loc_2_055D
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1030
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_055D
        cmp    word ptr [ebp - 0x24], 0x628b
        jne    loc_2_055D
        mov    al, byte ptr [ebp - 0x21]
        and    eax, 0xf
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax*2 + 0x1b9c] ; -> loc_1_1B9C
        mov    word ptr [ecx], ax
        mov    al, byte ptr [ebp - 0x21]
        and    al, 0x70
        shr    al, 3
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x1bbc]   ; -> loc_1_1BBC
        mov    word ptr [ecx + 0x1e7], ax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, 0x1f
        sub    cl, byte ptr [eax + 0x1e7]
        mov    byte ptr [eax + 0x1e6], cl
        mov    al, byte ptr [ebp - 0x22]
        and    al, 4
        shr    al, 2
        mov    ecx, dword ptr [ebp - 4]
        and    ax, 0xff
        mov    word ptr [ecx + 9], ax
        test   byte ptr [ebp - 0x22], 4
        jne    loc_2_0546
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 9], 1
        jmp    loc_2_055D
loc_2_0546:
        test   byte ptr [ebp - 0x21], 0x80
        mov    eax, dword ptr [ebp - 4]
        jne    loc_2_0557
        mov    word ptr [eax + 9], 2
        jmp    loc_2_055D
loc_2_0557:
        mov    word ptr [eax + 9], 3
loc_2_055D:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0610
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_2_057F
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10BC
loc_2_057F:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0590:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        mov    byte ptr [esi + 0x155], 1
        push   eax
        lea    eax, [esi + 0x1ce]
        push   eax
        call   sub_1_1098
        lea    eax, [esi + 0x1b6]
        push   eax
        call   sub_1_10A4
        mov    eax, dword ptr [esi + 0x1e9]
        push   eax
        call   sub_2_2170
        mov    eax, dword ptr [esi + 0x1b2]
        push   eax
        call   sub_1_0FE8
        mov    eax, dword ptr [esi + 0x1a2]
        mov    ecx, dword ptr [esi + 0x1a6]
        lea    edx, [esi + 0x1a2]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0
        push   esi
        call   sub_1_10BC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x1b94]       ; -> obj1:0x1B94
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x56
        push   eax
        call   sub_1_10B6
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 0
        cmp    dword ptr [esi + 2], 3
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_0660
        mov    dword ptr [eax + 0xa], 1
        jmp    loc_2_0667
        db  090h
loc_2_0660:
        mov    dword ptr [eax + 0xa], 3
loc_2_0667:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x12], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x16], 5
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [esi]
        mov    dword ptr [ecx + 0x1a], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1e], 0x10
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x3000
        mov    dword ptr [ecx + 0x26], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x2a], 0x10
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x2e], 0
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x4000
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x36], 0x10
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x3a], 0
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x8000
        mov    dword ptr [ecx + 0x3e], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x42], 0x10
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x46], 0
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0xc000
        mov    dword ptr [ecx + 0x4a], eax
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [esi + 0x1b2]
        mov    dword ptr [eax + 0x4e], 0x10
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x52], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x1ae]
        push   eax
        push   ebx
        call   sub_1_0FE2
        mov    edi, eax
        or     edi, edi
        je     loc_2_0768
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10BC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0768:
        push   esi
        call   sub_2_0B00
        push   esi
        call   sub_2_0C10
        push   esi
        call   sub_2_0EF0
        or     al, al
        jne    loc_2_07C0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10BC
        push   0
        push   1
        push   2
        push   0xaa000003
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1188
        add    esp, 0x14
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0FE8
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_07C0:
        mov    dword ptr [esi + 0x1ce], 0
        mov    dword ptr [esi + 0x1d2], 0x6c0 ; -> loc_1_06C0
        mov    dword ptr [esi + 0x1d6], esi
        lea    eax, [esi + 0x1e9]
        push   eax
        lea    eax, [esi + 0x1aa]
        push   eax
        lea    eax, [esi + 0x2e]
        push   eax
        push   0x1b30                        ; -> loc_1_1B30
        push   0x1a70                        ; -> loc_1_1A70
        push   0x1ae0                        ; -> loc_1_1AE0
        push   0x10
        call   sub_2_2050
        or     al, al
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jne    loc_2_0830
        call   sub_1_10BC
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0FE8
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0830:
        call   sub_1_10BC
        mov    dword ptr [esi + 0x1a2], 0x1e1c ; -> obj1:0x1E1C
        lea    ecx, [esi + 0x1a2]
        mov    eax, dword ptr [0x1e20]       ; -> obj1:0x1E20
        mov    dword ptr [esi + 0x1a6], eax
        mov    eax, dword ptr [0x1e20]       ; -> obj1:0x1E20
        pop    edi
        mov    dword ptr [eax], ecx
        sub    eax, eax
        mov    dword ptr [0x1e20], ecx       ; -> obj1:0x1E20
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0x155], 0
        je     loc_2_0890
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
loc_2_0890:
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, edx
        je     loc_2_08B1
        mov    eax, dword ptr [ebp + 0x14]
        lea    esp, [esp]
loc_2_08A0:
        cmp    dword ptr [eax], 0
        je     loc_2_08AD
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     loc_2_08A0
loc_2_08AD:
        cmp    ecx, edx
        jne    loc_2_08C0
loc_2_08B1:
        mov    eax, 0xc0010019
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
loc_2_08C0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], edx
        inc    dword ptr [esi + 0x137]
        mov    eax, dword ptr [0x1b94]       ; -> obj1:0x1B94
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x47
        push   eax
        call   sub_1_10B6
        or     eax, eax
        jne    loc_2_09E9
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x41], 0x100
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x43], 0
        mov    word ptr [esi + 0x1ed], 0x100
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x1e9]
        push   eax
        call   sub_2_21C0
        or     al, al
        jne    loc_2_0980
        mov    esi, 0xc0000001
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10BC
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0980:
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [esi + 0x192]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x196]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 0x196]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x196], eax
        inc    dword ptr [esi + 0x18e]
        cmp    byte ptr [esi + 0x136], 0
        je     loc_2_09E5
        mov    byte ptr [esi + 0x136], 0
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, byte ptr [esi + 8]
        or     al, 8
        out    dx, al
        push   esi
        call   sub_2_1530
        push   0xea60
        add    esi, 0x1ce
        push   esi
        call   sub_1_1092
loc_2_09E5:
        sub    esi, esi
        mov    eax, esi
loc_2_09E9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_09F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x18], al
        jne    loc_2_0A6F
        mov    byte ptr [esi + 0x18], 1
        dec    dword ptr [edi + 0x18e]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x1e9]
        push   eax
        call   sub_2_2290
        cmp    eax, 0xc001000e
        jne    loc_2_0A30
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_0A30:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [esi + 0x14]
        push   edi
        call   sub_2_0AB0
        push   0
        push   0
        push   esi
        call   sub_1_10BC
        cmp    dword ptr [edi + 0x18e], 0
        jne    loc_2_0A6D
        mov    dx, word ptr [edi]
        add    dx, 7
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    byte ptr [edi + 0x136], 1
loc_2_0A6D:
        sub    eax, eax
loc_2_0A6F:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x1e14]       ; -> obj1:0x1E14
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0FDC
        push   0
        mov    eax, dword ptr [0x1e18]       ; -> obj1:0x1E18
        push   eax
        call   sub_1_0FD0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        sub    si, si
        mov    ecx, dword ptr [edx + 0x192]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0xfffffe6e
        je     loc_2_0AE9
        add    eax, 0
loc_2_0AD0:
        mov    ax, word ptr [ecx + 0x41]
        cmp    si, ax
        jae    loc_2_0ADC
        mov    si, ax
loc_2_0ADC:
        mov    ecx, dword ptr [ecx]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0xfffffe6e
        jne    loc_2_0AD0
loc_2_0AE9:
        mov    word ptr [edx + 0x1ed], si
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0B00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x18e], 0
        mov    dword ptr [eax + 0x137], 1
        mov    byte ptr [eax + 0x154], 0
        mov    dword ptr [eax + 0x156], 0
        mov    byte ptr [eax + 0x136], 1
        mov    byte ptr [eax + 0x135], 1
        mov    dword ptr [eax + 0x15a], 0x12345678
        mov    dword ptr [eax + 0x15e], 0
        mov    dword ptr [eax + 0x162], 0
        mov    dword ptr [eax + 0x16a], 0
        mov    dword ptr [eax + 0x16e], 0
        mov    dword ptr [eax + 0x172], 0
        mov    dword ptr [eax + 0x176], 0
        mov    dword ptr [eax + 0x17a], 0
        mov    dword ptr [eax + 0x17e], 0
        mov    dword ptr [eax + 0x182], 0
        mov    dword ptr [eax + 0x186], 0
        mov    dword ptr [eax + 0x18a], 0
        mov    word ptr [eax + 0x25], 0
        mov    byte ptr [eax + 0x1f1], 0xc
        mov    byte ptr [eax + 0x1f2], 0xe
        mov    byte ptr [eax + 0x1f3], 0x40
        mov    byte ptr [eax + 0x1f4], 0x26
        mov    byte ptr [eax + 0x1f5], 0
        mov    byte ptr [eax + 0x1f6], 0x60
        mov    byte ptr [eax + 0x1f7], 0
        mov    byte ptr [eax + 0x1f8], 0xf2
        mov    byte ptr [eax + 0x1f9], 0
        mov    byte ptr [eax + 0x1fa], 0
        mov    byte ptr [eax + 0x1fb], 0x40
        mov    byte ptr [eax + 0x1fc], 0
        pop    ebp
        ret    4
        db  090h
sub_2_0C10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 0x1b], 0
        mov    word ptr [eax + 0x25], 0
        mov    dword ptr [eax + 0x143], 0
        mov    dword ptr [eax + 0x147], 0
        mov    dword ptr [eax + 0x14b], 0
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    ebx, dword ptr [esi + 8]
        cmp    byte ptr [ebx + 0x154], 0
        jne    loc_2_0CB0
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_2_0CA0
        mov    byte ptr [ebx + 0x153], 1
        push   ebx
        call   sub_2_0D70
        push   ebx
        call   sub_2_0CD0
        push   ebx
        call   sub_2_0E40
        push   esi
        push   ebx
        call   sub_2_0CC0
        push   edi
        push   ebx
        call   sub_2_0DD0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0CA0:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_0CB0:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_0CC0:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0CD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x143], 0
        je     loc_2_0D1D
        sub    edi, edi
loc_2_0CE4:
        mov    edx, dword ptr [esi + 0x143]
        cmp    dword ptr [edx + 0x24], edi
        lea    eax, [edx + 0x24]
        jne    loc_2_0CF8
        mov    dword ptr [esi + 0x147], edi
loc_2_0CF8:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x143], ecx
        mov    ebx, dword ptr [eax + 8]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0FFA
        dec    dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x143], edi
        jne    loc_2_0CE4
loc_2_0D1D:
        cmp    dword ptr [esi + 0x13b], 0
        je     loc_2_0D61
        sub    edi, edi
loc_2_0D28:
        mov    edx, dword ptr [esi + 0x13b]
        cmp    dword ptr [edx + 0x24], edi
        lea    eax, [edx + 0x24]
        jne    loc_2_0D3C
        mov    dword ptr [esi + 0x13f], edi
loc_2_0D3C:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x13b], ecx
        mov    ebx, dword ptr [eax + 8]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0FFA
        dec    dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x13b], edi
        jne    loc_2_0D28
loc_2_0D61:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0D70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, edi
        mov    esi, dword ptr [edi + 0x192]
        sub    eax, esi
        cmp    eax, 0xfffffe6e
        je     loc_2_0DB7
loc_2_0D89:
        inc    dword ptr [esi + 0x14]
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_101E
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_1024
        dec    dword ptr [esi + 0x14]
        mov    esi, dword ptr [esi]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0xfffffe6e
        jne    loc_2_0D89
loc_2_0DB7:
        mov    byte ptr [edi + 0x154], 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 0x192]
        sub    eax, edi
        cmp    eax, 0xfffffe6e
        je     loc_2_0E2C
        sub    ebx, ebx
        lea    esp, [esp]
loc_2_0DF0:
        inc    dword ptr [edi + 0x14]
        cmp    dword ptr [ebp + 0xc], ebx
        je     loc_2_0E08
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_101E
loc_2_0E08:
        push   4
        lea    eax, [ebp + 0xc]
        push   eax
        push   0x40010005
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_101E
        dec    dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xfffffe6e
        jne    loc_2_0DF0
loc_2_0E2C:
        mov    byte ptr [esi + 0x154], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, 0xa
        mov    dx, word ptr [edi]
        add    dx, 7
        mov    al, byte ptr [edi + 8]
        out    dx, al
        lea    esi, [edi + 0x1ce]
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_1098
loc_2_0E70:
        cmp    word ptr [edi + 0x1b], 0
        je     loc_2_0E84
        push   edi
        call   sub_1_0A00
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     loc_2_0E70
loc_2_0E84:
        push   edi
        call   sub_2_1580
        cmp    dword ptr [edi + 0x143], 0
        je     loc_2_0EA2
loc_2_0E93:
        push   edi
        call   sub_1_00C0
        cmp    dword ptr [edi + 0x143], 0
        jne    loc_2_0E93
loc_2_0EA2:
        push   edi
        call   sub_2_0C10
        mov    cx, word ptr [edi]
        add    cx, 0xe
        mov    dx, cx
        mov    al, 0x80
        out    dx, al
        push   edi
        call   sub_2_1180
        mov    bl, al
        mov    cx, word ptr [edi]
        add    cx, 7
        mov    al, byte ptr [edi + 8]
        or     al, 8
        mov    dx, cx
        out    dx, al
        push   edi
        call   sub_2_1530
        push   0x1388
        push   esi
        call   sub_1_1092
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0EF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], 0
        mov    ax, word ptr [esi]
        push   eax
        mov    eax, dword ptr [esi + 0x1b2]
        push   eax
        call   sub_2_1810
        mov    word ptr [esi + 0xd], ax
        sub    eax, eax
        mov    cx, word ptr [esi]
        cmp    word ptr [0x1b9c], cx         ; -> loc_1_1B9C
        je     loc_2_0F39
        mov    edx, 0x1b9c                   ; -> loc_1_1B9C
        nop    
loc_2_0F30:
        add    edx, 2
        inc    eax
        cmp    word ptr [edx], cx
        jne    loc_2_0F30
loc_2_0F39:
        mov    word ptr [ebp - 2], ax
        cmp    dword ptr [esi + 2], 3
        mov    ax, word ptr [esi]
        mov    word ptr [ebp - 6], ax
        je     loc_2_0FB9
        cmp    word ptr [esi + 0xd], 0
        jne    loc_2_0F9D
        mov    di, word ptr [0x1dec]         ; -> obj1:0x1DEC
        or     di, di
        je     loc_2_0F9D
        mov    ebx, 0x1dec                   ; -> obj1:0x1DEC
loc_2_0F62:
        mov    ax, word ptr [ebx]
        push   eax
        mov    eax, dword ptr [esi + 0x1b2]
        push   eax
        call   sub_2_1810
        mov    word ptr [esi + 0xd], ax
        or     ax, ax
        jne    loc_2_0F92
        mov    di, word ptr [ebx + 2]
        add    ebx, 2
        or     di, di
        jne    loc_2_0F62
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0F92:
        mov    word ptr [esi], di
        sub    di, di
        or     di, di
        jne    loc_2_0F62
loc_2_0F9D:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x1e7]
        sub    eax, 2
        cmp    eax, 0xd
        ja     loc_2_1135
        jmp    dword ptr [eax*4 + 0x1140]    ; -> obj2:0x1140
loc_2_0FB9:
        cmp    word ptr [esi + 0xd], 0
        jne    loc_2_0F9D
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0FCB:
        mov    al, 1
        jmp    loc_2_1006
        db  090h
loc_2_0FD0:
        mov    al, 2
        jmp    loc_2_1006
loc_2_0FD4:
        mov    al, 3
        jmp    loc_2_1006
loc_2_0FD8:
        mov    al, 4
        jmp    loc_2_1006
loc_2_0FDC:
        mov    al, 1
        jmp    loc_2_1006
loc_2_0FE0:
        mov    al, 5
        jmp    loc_2_1006
loc_2_0FE4:
        mov    al, 6
        jmp    loc_2_1006
loc_2_0FE8:
        cmp    word ptr [esi + 0xd], 0xbabc
        jne    loc_2_1135
        sub    al, al
        jmp    loc_2_1006
loc_2_0FF8:
        cmp    word ptr [esi + 0xd], 0xbabc
        jne    loc_2_1135
        mov    al, 7
loc_2_1006:
        mov    byte ptr [esi + 8], al
        and    ax, 0xff
        shl    ax, 0xd
        or     word ptr [ebp - 2], ax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1AD0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1B30
        mov    cx, word ptr [esi]
        add    cx, 7
        mov    dx, cx
        mov    al, byte ptr [esi + 8]
        out    dx, al
        push   esi
        call   sub_2_1580
        mov    ax, word ptr [esi]
        add    ax, 0xe
        mov    dx, ax
        mov    al, 0x80
        out    dx, al
        mov    ax, word ptr [ebp - 6]
        push   eax
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   esi
        call   sub_2_19F0
        push   esi
        call   sub_2_1C10
        push   esi
        call   sub_2_1950
        push   esi
        call   sub_2_1870
        cmp    dword ptr [esi + 2], 3
        je     loc_2_1086
        mov    eax, 1
        jmp    loc_2_108B
loc_2_1086:
        mov    eax, 2
loc_2_108B:
        sub    cl, cl
        xor    edx, edx
        mov    dx, word ptr [esi + 0x1e7]
        push   eax
        push   ecx
        push   edx
        push   edx
        lea    edi, [esi + 0x1b6]
        push   0x3f0                         ; -> loc_1_03F0
        push   esi
        push   0x3d0                         ; -> loc_1_03D0
        mov    eax, dword ptr [esi + 0x1b2]
        push   eax
        lea    eax, [ebp - 0xc]
        push   edi
        push   eax
        call   sub_1_109E
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_1105
        push   esi
        call   sub_2_1180
        or     al, al
        jne    loc_2_10FA
        push   0
        push   0xef0                         ; -> sub_2_0EF0
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x1b2]
        push   eax
        call   sub_1_1188
        add    esp, 0x14
        push   edi
        call   sub_1_10A4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_10FA:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1105:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x1e7]
        push   eax
        push   0xef0                         ; -> sub_2_0EF0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [esi + 0x1b2]
        push   eax
        call   sub_1_1188
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1135:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        ; ---- dispatch table (14 entries) ----
        dd  loc_2_0FCB
        dd  loc_2_0FD0
        dd  loc_2_0FD4
        dd  loc_2_0FD8
        dd  loc_2_1135
        dd  loc_2_1135
        dd  loc_2_1135
        dd  loc_2_0FDC
        dd  loc_2_0FE0
        dd  loc_2_0FE4
        dd  loc_2_0FE8
        dd  loc_2_1135
        dd  loc_2_1135
        dd  loc_2_0FF8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, 0xfff6
        mov    di, word ptr [esi]
        lea    ecx, [edi + 2]
        mov    dx, cx
        out    dx, ax
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        mov    dx, cx
        out    dx, ax
        mov    dx, di
        mov    ax, 1
        out    dx, ax
        mov    ax, 8
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        mov    dx, cx
        mov    ax, 8
        out    dx, ax
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        mov    word ptr [esi + 0x15], 0xe0
        mov    word ptr [esi + 0x13], 0xe0
        mov    word ptr [esi + 0x17], 0xe0
        mov    di, 0xe0
        mov    dx, word ptr [esi]
        sub    dx, 0x3ff4
        mov    ax, word ptr [esi + 0x13]
        out    dx, ax
        sub    cx, cx
        cmp    word ptr [esi + 0xf], cx
        jbe    loc_2_1284
loc_2_1212:
        mov    bx, word ptr [esi]
        mov    ax, di
        lea    edx, [ebx + 2]
        out    dx, ax
        mov    dx, bx
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        mov    ax, di
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        mov    ax, 0xffff
        out    dx, ax
        lea    eax, [edi + 0x1e]
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        out    dx, ax
        mov    ax, 0x2004
        out    dx, ax
        mov    ax, word ptr [esi + 0xf]
        sub    ax, cx
        cmp    ax, 1
        jne    loc_2_1260
        mov    ax, 0xe0
        jmp    loc_2_1269
        db  08Dh, 064h, 024h, 000h
loc_2_1260:
        lea    eax, [edi + 0x600]
        mov    dx, bx
loc_2_1269:
        out    dx, ax
        lea    eax, [edi + 6]
        mov    dx, bx
        out    dx, ax
        mov    word ptr [esi + 0x19], di
        add    di, 0x600
        inc    cx
        cmp    word ptr [esi + 0xf], cx
        ja     loc_2_1212
loc_2_1284:
        mov    word ptr [esi + 0x1d], di
        mov    word ptr [esi + 0x21], di
        mov    dx, word ptr [esi]
        sub    dx, 0x3ff2
        mov    ax, di
        add    ax, 0xa
        out    dx, ax
        sub    cx, cx
        cmp    word ptr [esi + 0x11], cx
        jbe    loc_2_131E
loc_2_12A6:
        mov    bx, word ptr [esi]
        mov    ax, di
        lea    edx, [ebx + 2]
        out    dx, ax
        mov    dx, bx
        sub    ax, ax
        out    dx, ax
        mov    ax, 0xffff
        out    dx, ax
        lea    eax, [edi + 0x20]
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        mov    ax, 0x85dc
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        mov    ax, word ptr [esi + 0x11]
        sub    ax, cx
        cmp    ax, 1
        jne    loc_2_12F1
        mov    ax, 0x8000
        out    dx, ax
        mov    ax, word ptr [esi + 0x21]
        add    ax, 0xa
        jmp    loc_2_12FF
loc_2_12F1:
        mov    dx, bx
        sub    ax, ax
        out    dx, ax
        lea    eax, [edi + 0x60a]
loc_2_12FF:
        out    dx, ax
        mov    dx, bx
        mov    ax, di
        out    dx, ax
        mov    word ptr [esi + 0x23], di
        mov    word ptr [esi + 0x1f], di
        add    di, 0x600
        inc    cx
        cmp    word ptr [esi + 0x11], cx
        ja     loc_2_12A6
loc_2_131E:
        mov    dx, word ptr [esi]
        add    dx, 0xd
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 0x82
        mov    cx, word ptr [esi]
        mov    al, byte ptr [ebp - 1]
        lea    edx, [ecx + 0xd]
        out    dx, al
        lea    edx, [ecx + 0xe]
        sub    al, al
        out    dx, al
        push   esi
        call   sub_2_15C0
        or     al, al
        jne    loc_2_1352
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1352:
        push   esi
        call   sub_2_15E0
        or     al, al
        jne    loc_2_1367
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1367:
        push   esi
        call   sub_2_1620
        or     al, al
        jne    loc_2_1380
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_1380:
        push   esi
        call   sub_2_1700
        or     al, al
        jne    loc_2_1395
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1395:
        mov    dx, word ptr [esi]
        add    dx, 0xd
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xfd
        or     byte ptr [ebp - 1], 0x80
        mov    dx, word ptr [esi]
        add    dx, 0xd
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_13C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x15]
        lea    eax, [edi + 0x600]
        cmp    word ptr [esi + 0x19], ax
        mov    word ptr [esi + 0x15], ax
        jae    loc_2_13E5
        mov    ax, word ptr [esi + 0x17]
        mov    word ptr [esi + 0x15], ax
loc_2_13E5:
        mov    cx, word ptr [esi]
        mov    ax, 0x20
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        mov    ax, 0x2002
        out    dx, ax
        mov    ax, word ptr [esi + 0x15]
        out    dx, ax
        push   6
        lea    eax, [esi + 0x1f1]
        push   eax
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        call   sub_1_0F64
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 0x15]
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 0x4004]
        out    dx, ax
        lea    edx, [ecx + 8]
        mov    ax, di
        out    dx, ax
        lea    edx, [ecx + 0x4004]
        inc    word ptr [esi + 0x1b]
        mov    ax, 0x20
        out    dx, ax
        lea    edx, [ecx + 0x400e]
        mov    ax, 0xa000
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_1460:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x15]
        mov    word ptr [ebp - 2], ax
        add    ax, 0x600
        cmp    word ptr [esi + 0x19], ax
        mov    word ptr [esi + 0x15], ax
        jae    loc_2_148A
        mov    ax, word ptr [esi + 0x17]
        mov    word ptr [esi + 0x15], ax
loc_2_148A:
        mov    cx, word ptr [esi]
        mov    ax, 0x40
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        mov    ax, 0x2003
        out    dx, ax
        mov    ax, word ptr [esi + 0x15]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0xc]
        imul   ax, ax, 6
        out    dx, ax
        sub    bx, bx
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_2_14E9
        mov    edi, dword ptr [ebp + 0x10]
        nop    
loc_2_14C0:
        mov    eax, ebx
        and    eax, 0xffff
        push   3
        lea    eax, [eax + eax*2]
        lea    eax, [edi + eax*2]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        inc    bx
        call   sub_1_0F64
        mov    eax, ebx
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp + 0xc]
        jb     loc_2_14C0
loc_2_14E9:
        mov    di, word ptr [esi]
        mov    ax, word ptr [esi + 0x15]
        lea    ecx, [edi + 8]
        mov    dx, cx
        out    dx, ax
        lea    ebx, [edi + 0x4004]
        mov    dx, bx
        out    dx, ax
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        inc    word ptr [esi + 0x1b]
        mov    dx, bx
        mov    ax, 0x40
        out    dx, ax
        lea    edx, [edi + 0x400e]
        mov    ax, 0xa000
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_2_1530:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0710
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 0x13]
        lea    edx, [ecx - 0x3ff4]
        out    dx, ax
        lea    edx, [ecx - 0x3ff2]
        mov    ax, word ptr [esi + 0x1d]
        add    ax, 0xa
        out    dx, ax
        lea    edx, [ecx - 0x3ff6]
        mov    ax, 0x110
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1580:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, byte ptr [esi + 8]
        out    dx, al
        call   sub_1_0710
        mov    cx, word ptr [esi]
        mov    ax, 0x440
        lea    edx, [ecx - 0x3ff6]
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        push   esi
        call   sub_1_0710
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_15C0:
        push   ebp
        mov    ebp, esp
        push   0xa000
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1780
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_15E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x8002
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1740
        push   0xc
        push   0x26
        lea    eax, [esi + 0x1f1]
        push   eax
        push   esi
        call   sub_1_0060
        push   0x2000
        push   0x100
        push   esi
        call   sub_2_1780
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0x8007
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1740
        push   0x2000
        push   0x100
        push   esi
        call   sub_2_1780
        or     al, al
        jne    loc_2_1652
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1652:
        mov    cx, word ptr [esi]
        mov    ax, 0x20
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 0x4000]
        in     ax, dx
        test   ah, 8
        je     loc_2_1674
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1674:
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x8003
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1740
        mov    cx, word ptr [edi]
        mov    ax, 0x26
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dx, cx
        imul   ax, ax, 6
        out    dx, ax
        sub    bx, bx
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_2_16E2
        mov    esi, dword ptr [ebp + 0x10]
loc_2_16B8:
        mov    eax, ebx
        and    eax, 0xffff
        push   3
        lea    eax, [eax + eax*2]
        movsx  eax, byte ptr [esi + eax*2]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi]
        push   eax
        inc    bx
        call   sub_1_0F64
        mov    eax, ebx
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp + 0xc]
        jb     loc_2_16B8
loc_2_16E2:
        push   0x2000
        push   0x100
        push   edi
        call   sub_2_1780
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1700:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x8001
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1740
        push   6
        push   0x26
        lea    eax, [esi + 0x2e]
        push   eax
        push   esi
        call   sub_1_0060
        push   0x2000
        push   0x100
        push   esi
        call   sub_2_1780
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1740:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax]
        mov    ax, 0x20
        lea    edx, [ecx + 8]
        out    dx, ax
        lea    edx, [ecx + 0x4002]
        mov    ax, word ptr [ebp + 0xc]
        out    dx, ax
        lea    edx, [ecx + 0x4004]
        mov    ax, 0x20
        out    dx, ax
        lea    edx, [ecx - 0x3ff4]
        out    dx, ax
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    bx, bx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0710
        cmp    al, bl
        je     loc_2_17DE
        mov    cx, word ptr [esi]
        mov    ax, word ptr [ebp + 0xc]
        lea    edx, [ecx - 0x3ff6]
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        mov    di, word ptr [ebp + 0x10]
loc_2_17B2:
        mov    dx, word ptr [esi]
        sub    dx, 0x3ff8
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    ax, 0xf000
        cmp    ax, di
        je     loc_2_17E9
        push   0x3e8
        call   sub_1_10B0
        mov    ax, bx
        inc    bx
        cmp    ax, 0x64
        jne    loc_2_17B2
loc_2_17DE:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_17E9:
        mov    cx, word ptr [esi]
        mov    ax, word ptr [ebp - 2]
        lea    edx, [ecx - 0x3ff6]
        and    ax, 0xf000
        out    dx, ax
        lea    edx, [ecx + 6]
        mov    al, 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_2_1810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        sub    esi, esi
        mov    dx, word ptr [ebp + 0xc]
        add    dx, 0x300f
        mov    bl, 4
loc_2_1825:
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 3
        lea    ecx, [eax*4]
        shr    byte ptr [ebp - 1], 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        shl    eax, cl
        or     esi, eax
        dec    bl
        jne    loc_2_1825
        mov    eax, esi
        and    eax, 0xfff0
        cmp    eax, 0xbab0
        je     loc_2_185C
        sub    ax, ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_185C:
        mov    ax, si
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [esi]
        add    dx, 2
        sub    ax, ax
        out    dx, ax
        mov    word ptr [ebp - 2], ax
        mov    ecx, 0x10
        lea    ebx, [ebx]
loc_2_18A0:
        mov    dx, word ptr [esi]
        inc    word ptr [ebp - 2]
        sub    ax, ax
        cmp    word ptr [ebp - 2], cx
        out    dx, ax
        jb     loc_2_18A0
        mov    dx, word ptr [esi]
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        sub    ax, ax
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        mov    al, 0x55
        out    dx, al
        lea    edx, [ecx + 0xf]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        mov    ax, 0x8000
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        mov    al, 0x55
        out    dx, al
        lea    edx, [ecx + 0xf]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        sub    ax, ax
        lea    edx, [ecx + 2]
        out    dx, ax
        mov    dx, cx
        mov    al, 0xaa
        out    dx, al
        lea    edx, [ecx + 0xf]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi]
        mov    ax, 0x8000
        lea    edx, [ecx + 4]
        out    dx, ax
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    word ptr [ebp - 2], 0xaa
        jne    loc_2_1934
        mov    word ptr [esi + 0xf], 8
        mov    word ptr [esi + 0x11], 0xd
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1934:
        cmp    word ptr [ebp - 2], 0x55
        jne    loc_2_1947
        mov    word ptr [esi + 0xf], 0xc
        mov    word ptr [esi + 0x11], 0x1e
loc_2_1947:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_1950:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    edi, edi
loc_2_1960:
        mov    ax, di
        add    ax, 2
        push   eax
        push   esi
        call   sub_2_1CB0
        mov    ch, al
        mov    cl, ah
        lea    eax, [ebp - 4]
        mov    edx, edi
        and    edx, 0xffff
        add    edx, edx
        sub    eax, edx
        inc    di
        mov    word ptr [eax], cx
        cmp    di, 3
        jb     loc_2_1960
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [esi + 0x28], al
        mov    ax, word ptr [ebp - 8]
        mov    byte ptr [esi + 0x29], ah
        lea    ecx, [esi + 0x28]
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [esi + 0x2a], al
        mov    ax, word ptr [ebp - 6]
        mov    byte ptr [esi + 0x2b], ah
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 0x2c], al
        mov    ax, word ptr [ebp - 4]
        mov    byte ptr [esi + 0x2d], ah
        lea    eax, [esi + 0x2e]
        cmp    byte ptr [eax], 0
        jne    loc_2_19E5
        cmp    byte ptr [esi + 0x2f], 0
        jne    loc_2_19E5
        cmp    byte ptr [esi + 0x30], 0
        jne    loc_2_19E5
        cmp    byte ptr [esi + 0x31], 0
        jne    loc_2_19E5
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_2_19E5
        cmp    byte ptr [esi + 0x33], 0
        jne    loc_2_19E5
        push   6
        push   ecx
        push   eax
        call   sub_1_119C
loc_2_19E5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_2_19F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        sub    si, si
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1CB0
        mov    cx, word ptr [ebp + 0xc]
        and    cx, 0xfc0f
        mov    dx, ax
        and    dx, 0xfc0f
        cmp    dx, cx
        je     loc_2_1A2E
        mov    bl, 1
        and    ax, 0x3f0
        or     ax, cx
        push   eax
        push   0
        push   edi
        call   sub_2_1EE0
loc_2_1A2E:
        push   5
        push   edi
        call   sub_2_1CB0
        mov    dx, ax
        and    dx, 1
        mov    cx, word ptr [ebp + 0x10]
        cmp    dx, cx
        je     loc_2_1A58
        mov    bl, 1
        and    ax, 0xfffe
        or     ax, cx
        push   eax
        push   5
        push   edi
        call   sub_2_1EE0
loc_2_1A58:
        or     bl, bl
        je     loc_2_1ABA
        push   edi
        call   sub_2_1C70
        mov    cx, word ptr [edi]
        add    cx, 0xe
        mov    dx, cx
        mov    al, 0x40
        out    dx, al
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi], ax
        push   0x3e8
        call   sub_1_10B0
loc_2_1A80:
        mov    ax, word ptr [edi]
        push   eax
        mov    eax, dword ptr [edi + 0x1b2]
        push   eax
        call   sub_2_1810
        or     ax, ax
        jne    loc_2_1AAA
        push   0x3e8
        call   sub_1_10B0
        mov    ax, si
        inc    si
        cmp    ax, 0x64
        jne    loc_2_1A80
loc_2_1AAA:
        mov    cx, word ptr [edi]
        mov    al, byte ptr [edi + 8]
        lea    edx, [ecx + 7]
        out    dx, al
        lea    edx, [ecx + 0xe]
        mov    al, 0x80
        out    dx, al
loc_2_1ABA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1AD0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 9]
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    ecx, -1
        je     loc_2_1B23
        cmp    ax, 1
        mov    eax, dword ptr [ebp + 0xc]
        jne    loc_2_1AF5
        and    word ptr [eax], 0xefff
loc_2_1AF5:
        cmp    word ptr [edx + 9], 2
        jne    loc_2_1B10
        mov    ecx, dword ptr [ebp + 0x10]
        or     word ptr [eax], 0x1000
        and    word ptr [ecx], 0xfffe
        jmp    loc_2_1B13
        db  005h, 000h, 000h, 000h, 000h
loc_2_1B10:
        mov    ecx, dword ptr [ebp + 0x10]
loc_2_1B13:
        cmp    word ptr [edx + 9], 3
        jne    loc_2_1B23
        or     word ptr [eax], 0x1000
        or     word ptr [ecx], 1
loc_2_1B23:
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0xb]
        mov    edx, eax
        and    edx, 0xffff
        cmp    edx, -1
        je     loc_2_1BF2
        cmp    ax, 1
        mov    esi, dword ptr [ebp + 0xc]
        jne    loc_2_1B6A
        mov    ax, word ptr [esi]
        or     ax, 0x400
        mov    word ptr [esi], ax
        and    ax, 0xf7ff
        mov    word ptr [esi], ax
loc_2_1B6A:
        cmp    word ptr [ecx + 0xb], 3
        jne    loc_2_1B82
        mov    ax, word ptr [esi]
        and    ax, 0xfbff
        mov    word ptr [esi], ax
        or     ax, 0x800
        mov    word ptr [esi], ax
loc_2_1B82:
        cmp    word ptr [ecx + 0xb], 2
        jne    loc_2_1BFA
        mov    ax, word ptr [esi]
        or     ax, 0x400
        mov    word ptr [esi], ax
        or     ax, 0x800
        mov    word ptr [esi], ax
        mov    dx, word ptr [ecx]
        add    dx, 0xd
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 0x30
        mov    di, word ptr [ecx]
        mov    al, byte ptr [ebp - 1]
        lea    edx, [edi + 0xd]
        out    dx, al
        mov    dx, di
        in     ax, dx
        mov    dx, word ptr [ecx]
        add    dx, 0xd
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xdf
        mov    dx, word ptr [ecx]
        add    dx, 0xd
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        test   al, 0x40
        je     loc_2_1BFA
        and    word ptr [esi], 0xfbff
        and    byte ptr [ebp - 1], 0xf7
        mov    dx, word ptr [ecx]
        add    dx, 0xd
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_1BF2:
        mov    esi, dword ptr [ebp + 0xc]
        or     word ptr [esi], 0xc00
loc_2_1BFA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 2], 3
        je     loc_2_1C68
        cmp    word ptr [ecx + 0xd], 0xbaba
        je     loc_2_1C68
        mov    dx, word ptr [ecx]
        add    dx, 0x300e
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    word ptr [ecx + 9], 1
        jne    loc_2_1C3E
        and    byte ptr [ebp - 1], 0x7f
loc_2_1C3E:
        cmp    word ptr [ecx + 9], 2
        jne    loc_2_1C4D
        or     byte ptr [ebp - 1], 0x80
        and    byte ptr [ebp - 1], 0xfd
loc_2_1C4D:
        cmp    word ptr [ecx + 9], 3
        jne    loc_2_1C5C
        or     byte ptr [ebp - 1], 0x80
        or     byte ptr [ebp - 1], 2
loc_2_1C5C:
        mov    dx, word ptr [ecx]
        add    dx, 0x300e
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_2_1C68:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_1C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    di, di
        sub    bx, bx
        mov    esi, dword ptr [ebp + 8]
        nop    
loc_2_1C80:
        push   ebx
        push   esi
        call   sub_2_1CB0
        add    di, ax
        inc    bx
        cmp    bx, 0x3f
        jb     loc_2_1C80
        mov    ax, 0xbaba
        sub    ax, di
        push   eax
        push   0x3f
        push   esi
        call   sub_2_1EE0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        or     word ptr [ebp - 2], 0x80
        and    word ptr [ebp - 2], 0xffbf
        mov    dx, word ptr [edi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffb2
        or     word ptr [ebp - 2], 2
        mov    dx, word ptr [edi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   3
        push   6
        push   edi
        call   sub_2_1D30
        push   6
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_2_1D30
        push   edi
        call   sub_2_1E20
        mov    si, ax
        push   edi
        call   sub_2_1E90
        mov    ax, si
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bx, 1
        mov    cx, word ptr [ebp + 0x10]
        dec    cl
        mov    esi, dword ptr [ebp + 8]
        shl    bx, cl
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffb3
        mov    di, word ptr [ebp + 0xc]
        mov    edi, edi
loc_2_1D60:
        and    word ptr [ebp - 2], 0xfffb
        test   di, bx
        je     loc_2_1D70
        or     word ptr [ebp - 2], 4
loc_2_1D70:
        mov    dx, word ptr [esi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   0x3e8
        call   sub_1_10B0
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DC0
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DF0
        shr    bx, 1
        or     bx, bx
        jne    loc_2_1D60
        and    word ptr [ebp - 2], 0xfffb
        mov    dx, word ptr [esi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_2_1DC0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx]
        or     ax, 1
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        push   0x3e8
        mov    dx, word ptr [ecx]
        add    dx, 0xe
        out    dx, al
        call   sub_1_10B0
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1DF0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx]
        and    ax, 0xfffe
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        push   0x3e8
        mov    dx, word ptr [ecx]
        add    dx, 0xe
        out    dx, al
        call   sub_1_10B0
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    di, di
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffb3
        mov    bx, 0x10
loc_2_1E44:
        add    di, di
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DC0
        mov    cx, word ptr [esi]
        add    cx, 0xe
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffbb
        test   byte ptr [ebp - 2], 8
        je     loc_2_1E6F
        or     di, 1
loc_2_1E6F:
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DF0
        dec    bx
        jne    loc_2_1E44
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffbf
        and    word ptr [ebp - 2], 0xfff9
        mov    dx, word ptr [esi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DC0
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_1DF0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1EE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffb2
        or     word ptr [ebp - 2], 2
        mov    dx, word ptr [esi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   5
        push   0x13
        push   esi
        call   sub_2_1D30
        push   4
        mov    di, word ptr [ebp + 0xc]
        push   edi
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_2000
        push   3
        push   7
        push   esi
        call   sub_2_1D30
        push   6
        push   edi
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_1FA0
        or     ax, ax
        je     loc_2_1F97
        push   esi
        call   sub_2_2000
        push   3
        push   5
        push   esi
        call   sub_2_1D30
        push   6
        push   edi
        push   esi
        call   sub_2_1D30
        push   0x10
        mov    ax, word ptr [ebp + 0x10]
        push   eax
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_1FA0
        or     ax, ax
        je     loc_2_1F97
        push   esi
        call   sub_2_2000
        push   5
        push   0x10
        push   esi
        call   sub_2_1D30
        push   4
        push   edi
        push   esi
        call   sub_2_1D30
        push   esi
        call   sub_2_1E90
loc_2_1F97:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
sub_2_1FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    di, di
        push   esi
        call   sub_2_2000
        mov    ebx, 8
        lea    ebx, [ebx]
loc_2_1FC0:
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], bx
        jne    loc_2_1FF0
        push   0x3e8
        call   sub_1_10B0
        inc    di
        cmp    di, 0x14
        jb     loc_2_1FC0
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_1FF0:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_2_2000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xffbc
        mov    dx, word ptr [esi]
        add    dx, 0xe
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        push   0x3e8
        call   sub_1_10B0
        or     word ptr [ebp - 2], 2
        mov    cx, word ptr [esi]
        add    cx, 0xe
        mov    dx, cx
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_2050:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x1e10]       ; -> obj1:0x1E10
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_10B6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_2084
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2084:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_209E
        mov    esi, 2
loc_2_209E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_10B6
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_20D2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2170
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_20D2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_10B6
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_2101
        push   eax
        call   sub_2_2170
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2101:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 0x2c], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0x30], dl
        add    eax, 0x2c
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_2170:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_2190
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_10BC
loc_2_2190:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_21A6
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_10BC
loc_2_21A6:
        push   0
        push   0x32
        push   esi
        call   sub_1_10BC
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_21C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_21D9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_21D9:
        mov    eax, dword ptr [0x1e10]       ; -> obj1:0x1E10
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_10B6
        or     eax, eax
        je     loc_2_21F9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_21F9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_2200:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_2213
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_2200
        jmp    loc_2_2222
loc_2_2213:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_2222:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_2238
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2238:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_2290:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2630
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_22B9
        cmp    ebx, 0x103
        jne    loc_2_22D0
loc_2_22B9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_2350
        or     eax, eax
        je     loc_2_22D0
        mov    ebx, eax
loc_2_22D0:
        or     ebx, ebx
        je     loc_2_22DC
        cmp    ebx, 0x103
        jne    loc_2_233B
loc_2_22DC:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_2336
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_22F2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_22F2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_2301
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_2307
loc_2_2301:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_2307:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_2316
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1018
loc_2_2316:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_10BC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_2336:
        mov    ebx, 0xc001000e
loc_2_233B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2350:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x20]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [esi + 0x24]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_11BC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_11BC
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_23D4
        sub    eax, eax
        nop    
loc_2_23B0:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 0x1c]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 0x24], edi
        ja     loc_2_23B0
loc_2_23D4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_243F
loc_2_23E1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_2434
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        call   sub_1_11BC
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        call   sub_1_11BC
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_2437
loc_2_2434:
        inc    dword ptr [ebp - 4]
loc_2_2437:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_23E1
loc_2_243F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_252A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_2453:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2810
        or     al, al
        je     loc_2_248A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_2518
loc_2_248A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_2564
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
        call   sub_1_11BC
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
        mov    ecx, dword ptr [esi + 0x24]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_11BC
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x14], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 0x19]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 0x24]
loc_2_2518:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_2453
loc_2_252A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_25B5
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_2540:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_25BB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_25A2
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_25A2
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_25A9
loc_2_2564:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_11BC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_11BC
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_25A2:
        mov    dword ptr [ebp - 8], 1
loc_2_25A9:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_2540
loc_2_25B5:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_2620
loc_2_25BB:
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_2622
        cmp    edi, 0x103
        je     loc_2_2622
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_11BC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_11BC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_2620:
        sub    edi, edi
loc_2_2622:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_2630:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        mov    dword ptr [esi + 0xc], 0
        je     loc_2_266D
        lea    esp, [esp]
loc_2_2660:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_2660
loc_2_266D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_26A1
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_2_26A3
        cmp    eax, 0x103
        je     loc_2_26A3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_26A1:
        sub    eax, eax
loc_2_26A3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_26B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x19]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xff
        mov    edi, dword ptr [edx + 0x24]
        cmp    edi, eax
        je     loc_2_26E2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_26D7:
        test   dword ptr [esi], edx
        je     loc_2_26DC
        inc    eax
loc_2_26DC:
        add    esi, 4
        dec    edi
        jne    loc_2_26D7
loc_2_26E2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_26F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x19]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 0x24], 0
        jbe    loc_2_276C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_2730:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_275A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_2786
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_11BC
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_275A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_2730
loc_2_276C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_2786:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x18]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_27B0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_27E0
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_27E0:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_11BC
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2810:
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
        je     loc_2_28A9
        dec    esi
        cmp    esi, edx
        jb     loc_2_28A9
        nop    
loc_2_2830:
        cmp    edx, ecx
        jb     loc_2_28A9
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_2853
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2887
loc_2_2853:
        jbe    loc_2_2860
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_2887
        db  08Bh, 0FFh
loc_2_2860:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_2877
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2887
loc_2_2877:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_2887
        mov    dword ptr [ebp - 4], 0
loc_2_2887:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_28B9
        jle    loc_2_2898
        or     edx, edx
        je     loc_2_28A9
        lea    esi, [edx - 1]
        jmp    loc_2_289B
loc_2_2898:
        lea    ecx, [edx + 1]
loc_2_289B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_2830
loc_2_28A9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_28B9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS
