; SPENDIS -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 5040 instructions recovered by recursive descent

Declare_Virtual_Device SPENDIS, 3, 10, SPENDIS_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2ffc  R-X
loc_1_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        cmp    byte ptr [edi + 0x153], 0
        jne    loc_1_0055
        cmp    byte ptr [ebx + 0x18], 0
        jne    loc_1_0047
        mov    byte ptr [edi + 0x152], 1
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
        mov    esi, 0xc0010002
        pop    edi
        mov    eax, esi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0055:
        mov    esi, 0xc001000d
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
        cmp    dword ptr [esi + 0x142], 0
        je     loc_1_00CC
        xor    edi, edi
loc_1_0094:
        mov    edx, dword ptr [esi + 0x142]
        lea    eax, [edx + 0x24]
        cmp    dword ptr [eax], edi
        jne    loc_1_00A7
        mov    dword ptr [esi + 0x146], edi
loc_1_00A7:
        mov    ecx, dword ptr [eax]
        push   0xc001000c
        push   edx
        mov    dword ptr [esi + 0x142], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_2BDA
        dec    dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x142], edi
        jne    loc_1_0094
loc_1_00CC:
        cmp    dword ptr [esi + 0x13a], 0
        je     loc_1_010F
        xor    edi, edi
loc_1_00D7:
        mov    edx, dword ptr [esi + 0x13a]
        lea    eax, [edx + 0x24]
        cmp    dword ptr [eax], edi
        jne    loc_1_00EA
        mov    dword ptr [esi + 0x13e], edi
loc_1_00EA:
        mov    ecx, dword ptr [eax]
        push   0xc001000c
        push   edx
        mov    dword ptr [esi + 0x13a], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_2BDA
        dec    dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x13a], edi
        jne    loc_1_00D7
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
        mov    edi, dword ptr [esi + 0x1a1]
        sub    eax, edi
        cmp    eax, 0xfffffe5f
        je     loc_1_0167
loc_1_0139:
        push   0
        inc    dword ptr [edi + 0x14]
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   0x40010004
        push   eax
        call   sub_1_2BFE
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_2C04
        mov    eax, esi
        dec    dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0xfffffe5f
        jne    loc_1_0139
loc_1_0167:
        mov    byte ptr [esi + 0x153], 1
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
        mov    edi, dword ptr [esi + 0x1a1]
        sub    eax, edi
        cmp    eax, 0xfffffe5f
        je     loc_1_01D8
        xor    ebx, ebx
loc_1_019C:
        inc    dword ptr [edi + 0x14]
        cmp    dword ptr [ebp + 0xc], ebx
        je     loc_1_01B4
        push   ebx
        mov    eax, dword ptr [edi + 0x10]
        push   ebx
        push   0x40010007
        push   eax
        call   sub_1_2BFE
loc_1_01B4:
        push   4
        lea    eax, [ebp + 0xc]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        push   0x40010005
        push   ecx
        call   sub_1_2BFE
        mov    eax, esi
        dec    dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0xfffffe5f
        jne    loc_1_019C
loc_1_01D8:
        mov    byte ptr [esi + 0x153], 0
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
        mov    al, 0xf
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        lea    esi, [edi + 0x1dd]
        add    edx, 3
        out    dx, al
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_2C78
        push   edi
        call   sub_1_07E0
        xor    ecx, ecx
        mov    word ptr [edi + 0x1e], cx
        mov    word ptr [edi + 0x1c], cx
        mov    word ptr [edi + 0x14], 1
        mov    word ptr [edi + 0x1a], cx
        xor    edx, edx
        mov    dx, word ptr [edi]
        xor    eax, eax
        mov    word ptr [edi + 0x12], cx
        add    edx, 0xa
        out    dx, ax
        mov    word ptr [edi + 0x16], cx
        cmp    dword ptr [edi + 0x142], ecx
        mov    word ptr [edi + 0x18], cx
        je     loc_1_0260
loc_1_0251:
        push   edi
        call   sub_1_0C70
        cmp    dword ptr [edi + 0x142], 0
        jne    loc_1_0251
loc_1_0260:
        push   edi
        call   sub_1_26E0
        push   0
        push   0x1e
        push   edi
        call   sub_1_0740
        test   al, al
        jne    loc_1_029C
        push   0
        mov    eax, dword ptr [edi + 0x1c1]
        push   0                             ; -> loc_1_01F0
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_2D68
        add    esp, 0x14
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
        push   0x1388
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
        mov    eax, dword ptr [esi + 0x1c1]
        push   eax
        call   sub_1_0820
        test   al, al
        jne    loc_1_030B
        push   0
        mov    eax, dword ptr [esi + 0x1c1]
        push   0                             ; -> sub_1_02C0
        push   2
        push   0xaa000004
        push   eax
        call   sub_1_2D68
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_030B:
        xor    edx, edx
        mov    al, 0xf
        mov    dx, word ptr [esi]
        push   0
        add    edx, 3
        push   0x1e
        out    dx, al
        push   esi
        call   sub_1_0740
        test   al, al
        jne    loc_1_034B
        push   0
        mov    eax, dword ptr [esi + 0x1c1]
        push   0                             ; -> sub_1_02C0
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_2D68
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_034B:
        mov    dx, word ptr [esi]
        mov    al, 0xb
        out    dx, al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_1_09B0
        push   esi
        call   sub_1_08E0
        cmp    ax, 0xffff
        jne    loc_1_038E
        push   0
        mov    eax, dword ptr [esi + 0x1c1]
        push   0                             ; -> sub_1_02C0
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_2D68
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_038E:
        test   ax, ax
        je     loc_1_03A5
        mov    byte ptr [esi + 0xd], 1
        mov    word ptr [esi + 0xe], 0x1800
        mov    word ptr [esi + 0x10], 0x6800
        jmp    loc_1_03B5
loc_1_03A5:
        mov    byte ptr [esi + 0xd], 0
        mov    word ptr [esi + 0xe], 0x3000
        mov    word ptr [esi + 0x10], 0xd000
loc_1_03B5:
        lea    edi, [esi + 0x1c5]
        push   1
        push   0
        mov    eax, dword ptr [esi + 0x1ea]
        push   eax
        mov    ecx, dword ptr [esi + 0x1c1]
        push   eax
        lea    edx, [ebp - 4]
        push   0                             ; -> loc_1_0A00
        push   esi
        push   0                             ; -> loc_1_09E0
        push   ecx
        push   edi
        push   edx
        call   sub_1_2C7E
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_042A
        push   esi
        call   sub_1_0460
        test   al, al
        jne    loc_1_0420
        push   0
        mov    eax, dword ptr [esi + 0x1c1]
        push   0                             ; -> sub_1_02C0
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_2D68
        add    esp, 0x14
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
        mov    eax, dword ptr [esi + 0x1ea]
        mov    ecx, dword ptr [esi + 0x1c1]
        push   eax
        push   0                             ; -> sub_1_02C0
        push   2
        push   0xaa000005
        push   ecx
        call   sub_1_2D68
        add    esp, 0x14
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
        mov    al, 0x40
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
        or     al, 0x80
        out    dx, al
        call   sub_1_0890
        mov    di, word ptr [esi]
        mov    bx, word ptr [esi + 0xe]
        lea    edx, [edi + 8]
        mov    al, bh
        mov    cl, bh
        out    dx, al
        lea    edx, [edi + 9]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x10]
        and    ebx, 0xffff
        add    eax, ebx
        sar    eax, 8
        dec    al
        out    dx, al
        lea    edx, [edi + 0xa]
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0xb]
        lea    eax, [ecx - 1]
        out    dx, al
        mov    edx, edi
        mov    al, 0x80
        out    dx, al
        lea    edx, [edi + 1]
        in     al, dx
        or     al, 0xa0
        out    dx, al
        mov    cx, word ptr [esi + 2]
        lea    edx, [edi + 2]
        mov    al, 0x14
        mov    ebx, ecx
        out    dx, al
        lea    edx, [edi + 3]
        and    ebx, 0xffff
        in     al, dx
        cmp    ebx, -1
        je     loc_1_04FA
        or     al, 0x10
        and    al, 0x3f
        cmp    cx, 1
        jne    loc_1_04E4
        and    al, 0xfb
loc_1_04E4:
        cmp    cx, 2
        jne    loc_1_04EE
        or     al, 4
        or     al, 0x20
loc_1_04EE:
        cmp    cx, 3
        jne    loc_1_04FC
        or     al, 4
        and    al, 0xdf
        jmp    loc_1_04FC
loc_1_04FA:
        and    al, 0x2f
loc_1_04FC:
        out    dx, al
        mov    edx, edi
        xor    al, al
        push   0x12c
        xor    edi, edi
        out    dx, al
        mov    al, 0x1e
        out    dx, al
        call   sub_1_2C90
        mov    di, word ptr [esi]
        mov    cx, word ptr [esi + 0xe]
        lea    edx, [edi + 4]
        mov    ax, cx
        out    dx, ax
        mov    ax, word ptr [esi + 0x10]
        lea    edx, [edi + 6]
        add    ax, cx
        push   esi
        dec    ax
        out    dx, ax
        lea    edx, [edi + 0xa]
        xor    eax, eax
        out    dx, ax
        call   sub_1_0680
        mov    word ptr [esi + 0x14], 1
        xor    eax, eax
        mov    word ptr [esi + 0x1e], ax
        mov    word ptr [esi + 0x1c], ax
        mov    cx, word ptr [esi + 0xe]
        mov    word ptr [esi + 0x1a], ax
        push   7
        mov    word ptr [esi + 0x12], ax
        push   7
        mov    word ptr [esi + 0x16], ax
        push   esi
        mov    word ptr [esi + 0x18], ax
        mov    word ptr [esi + 0x20], cx
        call   sub_1_0740
        test   al, al
        jne    loc_1_057A
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
        mov    al, 0xb
        out    dx, al
        add    edx, 3
        push   esi
        in     al, dx
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 2], al
        mov    al, 0xf
        out    dx, al
        call   sub_1_07E0
        test   al, al
        je     loc_1_0661
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 0xa]
        out    dx, ax
        lea    edx, [ecx + 0xc]
        out    dx, ax
        lea    edx, [ecx + 0xe]
        mov    eax, 3
        out    dx, ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0xc]
        out    dx, ax
        imul   cx, cx, 6
        out    dx, ax
        mov    ax, cx
        cmp    dword ptr [ebp + 0xc], ebx
        out    dx, ax
        je     loc_1_0623
        mov    edi, dword ptr [ebp + 0x10]
loc_1_05F6:
        push   3
        mov    eax, ebx
        inc    bx
        and    eax, 0xffff
        lea    ecx, [eax + eax*2]
        xor    eax, eax
        mov    ax, word ptr [esi]
        lea    edx, [edi + ecx*2]
        push   edx
        add    eax, 0xe
        push   eax
        call   sub_1_2B44
        mov    ecx, ebx
        and    ecx, 0xffff
        cmp    ecx, dword ptr [ebp + 0xc]
        jb     loc_1_05F6
loc_1_0623:
        push   3
        push   3
        push   esi
        call   sub_1_0740
        test   al, al
        je     loc_1_0661
        mov    ecx, 1
        xor    eax, eax
        mov    word ptr [esi + 0x1e], ax
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [esi + 0x1c], ax
        mov    word ptr [esi + 0x1a], ax
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    word ptr [esi + 0x12], ax
        add    edx, 0xa
        out    dx, ax
        mov    word ptr [esi + 0x16], ax
        mov    word ptr [esi + 0x18], ax
        mov    word ptr [esi + 0x14], cx
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
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_0680:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        xor    eax, eax
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 0xa]
        out    dx, ax
        lea    edx, [ecx + 0xc]
        out    dx, ax
        lea    edx, [ecx + 0xe]
        mov    eax, 2
        out    dx, ax
        xor    eax, eax
        out    dx, ax
        out    dx, ax
        mov    eax, 0xa
        out    dx, ax
        mov    eax, 0x2e
        out    dx, ax
        mov    eax, 0x60
        out    dx, ax
        mov    eax, 0x8f2
        out    dx, ax
        mov    eax, 0x4000
        push   0x3e8
        out    dx, ax
        mov    eax, 0xb6
        out    dx, ax
        mov    edx, ecx
        mov    al, 2
        out    dx, al
        call   sub_1_2C90
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 0xa]
        pop    esi
        out    dx, ax
        lea    edx, [ecx + 0xc]
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
        mov    al, 0x80
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
        mov    al, byte ptr [eax + esi + 0x2d]
        out    dx, al
        jb     loc_1_0714
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
        mov    bl, byte ptr [ebp + 0xc]
        cmp    bl, 0x1e
        jne    loc_1_076A
        push   0xc8
        call   sub_1_2C90
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_076A:
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    al, 8
        inc    edx
        push   0x3e8
        out    dx, al
        xor    di, di
        call   sub_1_2C90
        mov    dx, word ptr [esi]
        mov    al, bl
        out    dx, al
        push   0x3e8
        call   sub_1_2C90
loc_1_0793:
        xor    ebx, ebx
        mov    bx, word ptr [esi]
        lea    ecx, [ebx + 1]
        mov    edx, ecx
        in     al, dx
        test   al, 8
        je     loc_1_07B3
        mov    edx, ebx
        in     al, dx
        and    al, 0xf
        cmp    al, byte ptr [ebp + 0x10]
        je     loc_1_07CE
        mov    edx, ecx
        mov    al, 8
        out    dx, al
        jmp    loc_1_07BA
loc_1_07B3:
        push   0x64
        call   sub_1_2C90
loc_1_07BA:
        inc    di
        cmp    di, 0x64
        jb     loc_1_0793
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_07CE:
        mov    byte ptr [ebp - 1], 1
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
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
        test   al, 0x20
        je     loc_1_0810
        push   0x64
        call   sub_1_2C90
        mov    ax, di
        inc    di
        cmp    ax, 0x3e8
        jne    loc_1_07EB
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
        mov    edx, dword ptr [ebp + 0xc]
        push   esi
        add    edx, 2
        mov    ebx, 0xec
loc_1_0835:
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], bl
        cmp    byte ptr [ebp - 1], 0x24
        je     loc_1_0849
        inc    cl
        cmp    cl, 4
        jb     loc_1_0835
loc_1_0849:
        mov    bl, 0x40
        xor    cl, cl
loc_1_084D:
        in     al, dx
        mov    esi, ebx
        mov    byte ptr [ebp - 1], al
        and    esi, 0xff
        and    eax, 0xec
        sub    esi, eax
        cmp    esi, -0x24
        jne    loc_1_0879
        add    bl, 0x40
        inc    cl
        cmp    cl, 3
        jb     loc_1_084D
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
        jne    loc_1_08AF
        mov    word ptr [esi + 4], 2
loc_1_08AF:
        mov    cx, word ptr [esi + 4]
        cmp    cx, 1
        jne    loc_1_08BC
        or     al, 0x42
        out    dx, al
loc_1_08BC:
        cmp    cx, 3
        jne    loc_1_08C8
        mov    word ptr [esi + 4], 2
loc_1_08C8:
        cmp    word ptr [esi + 4], 2
        jne    loc_1_08D4
        and    al, 0xbf
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
        lea    edi, [esi + 0xc]
        mov    cx, 0x10
        add    esi, 0xe
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
        jne    loc_1_0908
        mov    cx, 3
        mov    edx, edi
        xor    bx, bx
        xor    eax, eax
        out    dx, ax
loc_1_0921:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    loc_1_0999
        add    cx, 3
        inc    bx
        cmp    bx, 0x10
        jb     loc_1_0921
        mov    cx, 0x10
        mov    edx, edi
        mov    eax, 0x8000
        out    dx, ax
        mov    ax, 9
loc_1_0947:
        mov    edx, esi
        out    dx, ax
        add    ax, 3
        dec    cx
        jne    loc_1_0947
        mov    cx, 0x10
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
loc_1_095D:
        mov    edx, esi
        xor    eax, eax
        out    dx, ax
        dec    cx
        jne    loc_1_095D
        mov    cx, 9
        mov    edx, edi
        xor    bx, bx
        mov    eax, 0x8000
        out    dx, ax
loc_1_0977:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    loc_1_09A4
        add    cx, 3
        inc    bx
        cmp    bx, 0x10
        jb     loc_1_0977
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0999:
        mov    ax, 0xffff
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
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        push   6
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    edi, [esi + 0x2d]
        add    esi, 0x27
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
        mov    al, 0xf
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x153], 0
        jne    loc_1_0ABE
        inc    dword ptr [esi + 0x136]
        mov    word ptr [esi + 0x24], 3
loc_1_0A21:
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        in     al, dx
        mov    bl, al
        and    bl, 7
        jne    loc_1_0A3F
        cmp    dword ptr [esi + 0x142], 0
        jne    loc_1_0A3F
        cmp    word ptr [esi + 0x24], 0
        jne    loc_1_0A91
loc_1_0A3F:
        mov    al, bl
        and    al, 3
        jne    loc_1_0A4C
        cmp    word ptr [esi + 0x24], 0
        jne    loc_1_0A57
loc_1_0A4C:
        out    dx, al
        push   esi
        call   sub_1_0AD0
        test   al, al
        je     loc_1_0ABE
loc_1_0A57:
        mov    al, bl
        and    al, 8
        je     loc_1_0A64
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
loc_1_0A64:
        mov    al, bl
        and    al, 4
        je     loc_1_0A77
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
        call   sub_1_1CE0
loc_1_0A77:
        cmp    dword ptr [esi + 0x142], 0
        je     loc_1_0A21
loc_1_0A80:
        push   esi
        call   sub_1_0C70
        cmp    dword ptr [esi + 0x142], 0
        jne    loc_1_0A80
        jmp    loc_1_0A21
loc_1_0A91:
        xor    edx, edx
        mov    al, 8
        dec    dword ptr [esi + 0x136]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
        cmp    byte ptr [esi + 0x133], 0
        je     loc_1_0ABE
        mov    byte ptr [esi + 0x133], 0
        mov    eax, dword ptr [esi + 0x1ee]
        push   eax
        call   sub_1_2A20
loc_1_0ABE:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x24], 3
loc_1_0AE2:
        dec    word ptr [esi + 0x24]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x20]
        lea    edx, [ecx + 0xc]
        out    dx, ax
        lea    edx, [ecx + 0xe]
        in     ax, dx
        mov    di, ax
        in     ax, dx
        test   ah, 0x20
        je     loc_1_0C0F
        test   di, 8
        je     loc_1_0C0F
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        inc    dword ptr [esi + 0x161]
        in     ax, dx
        cmp    ax, 0x5ea
        mov    di, ax
        ja     loc_1_0BE4
        mov    bx, word ptr [esi + 0x1f2]
        cmp    di, bx
        ja     loc_1_0B3D
        mov    ebx, edi
loc_1_0B3D:
        mov    ax, bx
        add    ecx, 0xe
        shr    ax, 1
        mov    edx, eax
        lea    eax, [esi + 0x33]
        and    edx, 0xffff
        push   edx
        push   eax
        push   ecx
        call   sub_1_2B81
        test   bl, 1
        je     loc_1_0B74
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0xffff
        add    edx, 0xe
        in     al, dx
        mov    byte ptr [ecx + esi + 0x32], al
loc_1_0B74:
        mov    cx, word ptr [esi + 0x20]
        add    cx, 8
        cmp    byte ptr [esi + 0xd], 0
        mov    word ptr [esi + 0x22], di
        je     loc_1_0B94
        cmp    cx, 0x8000
        jb     loc_1_0BA0
        sub    cx, 0x6800
        jmp    loc_1_0BA0
loc_1_0B94:
        cmp    cx, 0x3000
        jae    loc_1_0BA0
        add    cx, 0x3000
loc_1_0BA0:
        mov    eax, edi
        lea    edx, [esi + 0x41]
        and    eax, 0xffff
        and    ecx, 0xffff
        sub    eax, 0xe
        push   eax
        mov    eax, ebx
        and    eax, 0xffff
        lea    ebx, [esi + 0x33]
        sub    eax, 0xe
        push   eax
        push   edx
        push   0xe
        mov    edx, dword ptr [esi + 0x1ee]
        push   ebx
        push   ebx
        push   ecx
        push   edx
        call   sub_1_2840
        cmp    byte ptr [esi + 0x153], 0
        jne    loc_1_0C1A
        mov    byte ptr [esi + 0x133], 1
loc_1_0BE4:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0x20], ax
        sub    ax, 2
        cmp    word ptr [esi + 0xe], ax
        jbe    loc_1_0BFA
        add    ax, word ptr [esi + 0x10]
loc_1_0BFA:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 6
        out    dx, ax
        cmp    word ptr [esi + 0x24], 0
        jne    loc_1_0AE2
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
        mov    esi, dword ptr [ebp + 0xc]
        cmp    word ptr [esi + 0x1e], 0
        je     loc_1_0C56
        mov    ax, word ptr [esi + 0x1f4]
        dec    ax
        mov    word ptr [esi + 0x1f4], ax
        jne    loc_1_0C56
        push   esi
        call   sub_1_1CE0
loc_1_0C56:
        push   0x7d0
        add    esi, 0x1dd
        push   esi
        call   sub_1_2C72
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h
sub_1_0C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 0x142]
        mov    dword ptr [ebx + 0x14a], edi
        lea    esi, [edi + 0x24]
        cmp    dword ptr [esi], 0
        jne    loc_1_0C9A
        mov    dword ptr [ebx + 0x146], 0
loc_1_0C9A:
        mov    eax, dword ptr [esi]
        lea    ecx, [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        mov    dword ptr [ebx + 0x142], eax
        push   0
        push   0
        push   edi
        call   sub_1_2CC0
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0x10]
        push   edx
        push   eax
        call   sub_1_2CE4
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x1f2]
        cmp    ecx, dword ptr [ebp - 8]
        jbe    loc_1_0CFE
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        je     loc_1_0CFE
        cmp    ecx, eax
        jb     loc_1_0CE1
        mov    ecx, eax
loc_1_0CE1:
        lea    eax, [ebp - 8]
        lea    edx, [ebx + 0x33]
        push   eax
        and    ecx, 0xffff
        push   edx
        push   ecx
        push   0
        push   edi
        call   sub_1_0D50
        lea    ecx, [ebx + 0x33]
        mov    dword ptr [ebp - 4], ecx
loc_1_0CFE:
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [ebx + 0x133], 1
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe
        mov    ecx, dword ptr [ebx + 0x1ee]
        push   eax
        push   0xe
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   0
        push   ecx
        call   sub_1_2840
        push   0
        mov    ecx, dword ptr [esi + 0x10]
        push   edi
        push   ecx
        call   sub_1_2BDA
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0D50:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_0E19
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_2CC0
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_0E19
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_2CE4
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_0E14
        mov    edi, dword ptr [ebp + 0xc]
loc_1_0D9F:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0DCB
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_2D08
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0E14
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_2CE4
        jmp    loc_1_0E0F
loc_1_0DCB:
        test   edi, edi
        je     loc_1_0DE8
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_1_0DE0
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0E0F
loc_1_0DE0:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_0DE8:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_1_0DF6
        mov    ebx, eax
loc_1_0DF6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        add    esi, ebx
        call   sub_1_2D7C
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_0E0F:
        cmp    dword ptr [ebp + 0x10], esi
        ja     loc_1_0D9F
loc_1_0E14:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_0E19:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    eax, dword ptr [ebp + 0x1c]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    byte ptr [ebp - 1], bl
        mov    dword ptr [eax], ebx
        cmp    dword ptr [ebp + 0x10], ebx
        je     loc_1_0F8D
        push   ebx
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x14]
        push   ebx
        push   edx
        call   sub_1_2CC0
        cmp    dword ptr [ebp - 0x14], ebx
        je     loc_1_0F8D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edx
        call   sub_1_2CE4
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x22]
        mov    ecx, dword ptr [ebp + 0x18]
        sub    eax, ecx
        cmp    eax, dword ptr [ebp + 0x10]
        jae    loc_1_0E8F
        mov    dword ptr [ebp + 0x10], eax
loc_1_0E8F:
        test   cl, 1
        je     loc_1_0E99
        mov    byte ptr [ebp - 1], 1
        dec    ecx
loc_1_0E99:
        mov    ax, word ptr [ebp + 0xc]
        add    ax, cx
        cmp    byte ptr [esi + 0xd], 0
        je     loc_1_0EB7
        cmp    ax, 0x8000
        jb     loc_1_0EC8
        sub    ax, 0x6800
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    loc_1_0ECD
loc_1_0EB7:
        cmp    ax, 0x3000
        jae    loc_1_0EC8
        add    ax, 0x3000
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    loc_1_0ECD
loc_1_0EC8:
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_0ECD:
        add    edx, 0xc
        cmp    dword ptr [ebp + 0x10], 0
        out    dx, ax
        je     loc_1_0F88
loc_1_0EDC:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0F0C
        lea    eax, [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        call   sub_1_2D08
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_0F88
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edx
        call   sub_1_2CE4
        jmp    loc_1_0F7F
loc_1_0F0C:
        mov    edi, dword ptr [ebp + 0x10]
        sub    edi, ebx
        cmp    edi, dword ptr [ebp - 8]
        jbe    loc_1_0F19
        mov    edi, dword ptr [ebp - 8]
loc_1_0F19:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0F35
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dx, word ptr [esi]
        dec    edi
        add    edx, 0xf
        inc    ebx
        in     al, dx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0xc]
        dec    dword ptr [ebp - 8]
loc_1_0F35:
        test   edi, 0xfffffffe
        je     loc_1_0F55
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 0xc]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xe
        push   eax
        call   sub_1_2B81
loc_1_0F55:
        test   edi, 1
        je     loc_1_0F73
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ebp - 1], 1
        mov    dx, word ptr [esi]
        add    edx, 0xe
        in     al, dx
        mov    byte ptr [ecx + edi - 1], al
        jmp    loc_1_0F77
loc_1_0F73:
        mov    byte ptr [ebp - 1], 0
loc_1_0F77:
        add    ebx, edi
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 8], edi
loc_1_0F7F:
        cmp    ebx, dword ptr [ebp + 0x10]
        jb     loc_1_0EDC
loc_1_0F88:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], ebx
loc_1_0F8D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FA0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ecx + 8]
        cmp    byte ptr [edx + 0x153], 0
        jne    loc_1_101D
        cmp    byte ptr [ecx + 0x18], 0
        jne    loc_1_1013
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_1_0FD3
        cmp    eax, 1
        je     loc_1_0FF3
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    8
loc_1_0FD3:
        lea    eax, [esi + 0x24]
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    esi, dword ptr [esi + 0x14]
        push   esi
        push   ecx
        push   edx
        call   sub_1_1230
        pop    esi
        pop    ebp
        ret    8
loc_1_0FF3:
        lea    eax, [esi + 0x24]
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    esi, dword ptr [esi + 0x14]
        push   esi
        push   ecx
        push   edx
        call   sub_1_1070
        pop    esi
        pop    ebp
        ret    8
loc_1_1013:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    8
loc_1_101D:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1030:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    dword ptr [ecx + 0x10], 2
        je     loc_1_1046
        mov    eax, 0xc00000bb
        pop    ebp
        ret    8
loc_1_1046:
        lea    eax, [ecx + 0x24]
        lea    edx, [ecx + 0x20]
        push   eax
        push   edx
        mov    eax, dword ptr [ecx + 0x1c]
        mov    edx, dword ptr [ecx + 0x18]
        push   eax
        mov    ecx, dword ptr [ecx + 0x14]
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
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        cmp    eax, 0x1010103
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        mov    dword ptr [esi], 0
        ja     loc_1_10AF
        je     loc_1_11D0
        cmp    eax, 0x1010e
        je     loc_1_10BE
        cmp    eax, 0x1010f
        je     loc_1_113C
        cmp    eax, 0x10112
        je     loc_1_11C4
loc_1_10AF:
        mov    esi, 0xc0010017
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_10BE:
        mov    edi, dword ptr [ebp + 0x18]
        cmp    edi, 4
        je     loc_1_10D9
        mov    eax, 0xc0010015
        pop    edi
        mov    dword ptr [esi], 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_10D9:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_2D7C
        test   dword ptr [ebp - 8], 0xf054
        je     loc_1_110D
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], 4
        mov    eax, 0xc00000bb
        mov    dword ptr [esi], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_110D:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xc]
        mov    eax, dword ptr [esi + 0x1ee]
        push   edx
        push   eax
        call   sub_2_0800
loc_1_112B:
        mov    esi, eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_113C:
        xor    esi, esi
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_115C
        mov    esi, 0xc0010014
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_115C:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2D7C
        cmp    dword ptr [ebp - 4], 0x100
        ja     loc_1_11B5
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x1f2]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_11A1
        mov    ax, word ptr [ebp - 4]
        pop    edi
        mov    word ptr [ecx + 0x1f2], ax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x41], ax
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11A1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x41], ax
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11B5:
        mov    esi, 0xc0000001
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11C4:
        xor    esi, esi
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11D0:
        mov    ecx, 6
        mov    edi, dword ptr [ebp + 0x18]
        mov    eax, edi
        cdq    
        idiv   ecx
        test   edx, edx
        je     loc_1_11F0
        mov    eax, 0xc0010015
        pop    edi
        mov    dword ptr [esi], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11F0:
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, 6
        mov    eax, edi
        cdq    
        idiv   ecx
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        mov    edx, dword ptr [ecx + 0x1ee]
        push   eax
        push   edx
        call   sub_2_0520
loc_1_1219:
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, esi
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1230:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0x24
        mov    word ptr [ebp - 2], cx
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 0x1c]
        push   edi
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0x20]
        lea    esi, [ebp - 8]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        mov    ecx, 4
        mov    edx, dword ptr [ebp + 0x10]
        cmp    edx, 0x1010101
        mov    eax, edx
        ja     loc_1_127E
        je     loc_1_15AB
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_129F
        jmp    dword ptr [eax*4]             ; -> obj1:0x16D0
loc_1_127E:
        cmp    eax, 0x1010102
        je     loc_1_15D0
        cmp    eax, 0x1010103
        je     loc_1_15F5
        cmp    eax, 0x1010104
        je     loc_1_1684
loc_1_129F:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_12B1
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_168B
loc_1_12B1:
        cmp    edx, 0x2020d
        ja     loc_1_12D1
        je     loc_1_13B6
        sub    edx, 0x20101
        cmp    edx, 4
        ja     loc_1_12EB
        jmp    dword ptr [edx*4]             ; -> obj1:0x171C
loc_1_12D1:
        cmp    edx, 0x1020101
        ja     loc_1_12F7
        je     loc_1_13D8
        cmp    edx, 0x2020e
        je     loc_1_13C7
loc_1_12EB:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_168B
loc_1_12F7:
        cmp    edx, 0x1020201
        ja     loc_1_131F
        je     loc_1_140B
        cmp    edx, 0x1020102
        je     loc_1_13E9
        cmp    edx, 0x1020103
        je     loc_1_13FA
        jmp    loc_1_12EB
loc_1_131F:
        sub    edx, 0x1020202
        cmp    edx, 4
        ja     loc_1_12EB
        jmp    dword ptr [edx*4]             ; -> obj1:0x1730
loc_1_1331:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x15d]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1342:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x161]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1353:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x169]
        add    eax, dword ptr [edi + 0x16d]
        add    eax, dword ptr [edi + 0x171]
        add    eax, dword ptr [edi + 0x175]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1376:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x19d]
        add    eax, dword ptr [edi + 0x191]
        add    eax, dword ptr [edi + 0x195]
        add    eax, dword ptr [edi + 0x185]
        add    eax, dword ptr [edi + 0x199]
        add    eax, dword ptr [edi + 0x189]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_13A5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x199]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_13B6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x191]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_13C7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x165]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_13D8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x195]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_13E9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17d]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_13FA:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x181]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_140B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x179]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_141C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x169]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_142D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x19d]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_143E:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x171]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_144F:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x175]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1460:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16d]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1471:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_1486
        mov    esi, 0                        ; -> loc_1_2DD8
        mov    ecx, 0xa0
        jmp    loc_1_168B
loc_1_1486:
        mov    esi, 0                        ; -> loc_1_2E78
        mov    ecx, 0x5c
        jmp    loc_1_168B
loc_1_1495:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_168B
loc_1_14A1:
        lea    esi, [ebp - 0x1c]
        mov    ecx, 4
        jmp    loc_1_168B
loc_1_14AE:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_168B
loc_1_14BA:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_168B
loc_1_14C6:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_168B
loc_1_14D2:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0xe]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_14E3:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x10]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_14F4:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_168B
loc_1_1500:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x27
        lea    eax, [ebp - 0x14]
        push   edi
        lea    esi, [ebp - 0x14]
        push   eax
        call   sub_1_2D7C
        mov    byte ptr [ebp - 0x11], 0
        mov    ecx, 4
        jmp    loc_1_168B
loc_1_1523:
        mov    esi, 0                        ; -> loc_1_2ED4
        mov    ecx, 0x1e
        jmp    loc_1_168B
loc_1_1532:
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        je     loc_1_1547
        mov    eax, dword ptr [ebx + 0xc]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_168B
loc_1_1547:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x1ee]
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_168B
loc_1_155B:
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        jne    loc_1_1576
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x1f2]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1576:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x41]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_168B
loc_1_1584:
        lea    esi, [ebp - 2]
        mov    ecx, 2
        mov    word ptr [ebp - 2], 0x100
        jmp    loc_1_168B
loc_1_1597:
        lea    esi, [ebp - 8]
        mov    ecx, 4
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_168B
loc_1_15AB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x27]
        add    edi, 0x27
        lea    esi, [ebp - 0x24]
        mov    dword ptr [ebp - 0x24], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x20], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 0x1f], dl
        jmp    loc_1_168B
loc_1_15D0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x2d]
        add    edi, 0x2d
        lea    esi, [ebp - 0x24]
        mov    dword ptr [ebp - 0x24], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x20], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 0x1f], dl
        jmp    loc_1_168B
loc_1_15F5:
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        jne    loc_1_162D
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x18]
        push   ecx
        mov    edi, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 0x18]
        mov    eax, dword ptr [edi + 0x1ee]
        push   eax
        push   ecx
        call   sub_2_0980
loc_1_161B:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        lea    ecx, [eax*2]
        jmp    loc_1_168B
loc_1_162D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    ecx, dword ptr [edi + 0x1ee]
        push   ecx
        lea    edx, [ebp - 0x18]
        push   edx
        call   sub_2_08C0
loc_1_1650:
        cmp    dword ptr [ebp - 0x18], 0
        mov    esi, dword ptr [ebp + 0x14]
        jne    loc_1_1668
        mov    eax, dword ptr [ebp - 0x10]
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    loc_1_168B
loc_1_1668:
        mov    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [edi + 0x1ee]
        push   eax
        push   ecx
        call   sub_2_0880
loc_1_1678:
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    loc_1_168B
loc_1_1684:
        mov    dword ptr [ebp - 8], 0x10
loc_1_168B:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_16C2
        cmp    ecx, dword ptr [ebp + 0x18]
        jbe    loc_1_16AE
        mov    eax, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [ebp - 0xc], 0xc0010016
        pop    esi
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 0xc]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_16AE:
        mov    eax, dword ptr [ebp + 0x1c]
        test   ecx, ecx
        mov    dword ptr [eax], ecx
        je     loc_1_16C2
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   esi
        push   eax
        call   sub_1_2D7C
loc_1_16C2:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
        ; ---- dispatch table (29 entries) ----
        dd  loc_1_1471
        dd  loc_1_1495
        dd  loc_1_14A1
        dd  loc_1_14A1
        dd  loc_1_14AE
        dd  loc_1_14BA
        dd  loc_1_14C6
        dd  loc_1_14D2
        dd  loc_1_14E3
        dd  loc_1_14F4
        dd  loc_1_14F4
        dd  loc_1_1500
        dd  loc_1_1523
        dd  loc_1_1532
        dd  loc_1_155B
        dd  loc_1_1584
        dd  loc_1_14F4
        dd  loc_1_129F
        dd  loc_1_1597
        dd  loc_1_1331
        dd  loc_1_1342
        dd  loc_1_1353
        dd  loc_1_1376
        dd  loc_1_13A5
        dd  loc_1_141C
        dd  loc_1_142D
        dd  loc_1_143E
        dd  loc_1_144F
        dd  loc_1_1460
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1750:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    edi, edi
        cmp    byte ptr [esi + 0x153], 0
        je     loc_1_1776
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_1776:
        xor    ebx, ebx
        mov    al, 0xb
        mov    bx, word ptr [esi]
        mov    edx, ebx
        out    dx, al
        mov    al, 0x80
        mov    ecx, dword ptr [ebp + 0xc]
        out    dx, al
        lea    edx, [ebx + 2]
        in     al, dx
        and    al, 0xfe
        or     al, 2
        test   cl, 0x24
        je     loc_1_1795
        or     al, 1
loc_1_1795:
        test   cl, 8
        je     loc_1_179C
        and    al, 0xfd
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
        je     loc_1_180C
        mov    dx, word ptr [esi]
        mov    al, 0x1e
        out    dx, al
        push   3
        call   sub_1_2C90
        mov    bx, word ptr [esi + 0xe]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    ax, bx
        lea    edx, [ecx + 4]
        out    dx, ax
        mov    ax, word ptr [esi + 0x10]
        lea    edx, [ecx + 6]
        add    ax, bx
        dec    ax
        out    dx, ax
        lea    edx, [ecx + 0xa]
        xor    eax, eax
        out    dx, ax
        mov    word ptr [esi + 0x1e], ax
        mov    word ptr [esi + 0x1c], ax
        mov    word ptr [esi + 0x1a], ax
        mov    word ptr [esi + 0x12], ax
        mov    word ptr [esi + 0x16], ax
        mov    word ptr [esi + 0x18], ax
        mov    word ptr [esi + 0x14], 1
        mov    bx, word ptr [esi + 0xe]
        mov    word ptr [esi + 0x20], bx
        jmp    loc_1_1811
loc_1_180C:
        mov    edi, 0xc0000001
loc_1_1811:
        mov    dx, word ptr [esi]
        mov    al, 8
        out    dx, al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_1820:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x153], 0
        je     loc_1_183C
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_183C:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x10
        jbe    loc_1_184D
        mov    eax, 0xc000009a
        pop    ebp
        ret    0x1c
loc_1_184D:
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        push   eax
        push   edx
        call   sub_1_0590
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        ret    0x1c
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
        dec    dword ptr [edi + 0x14]
        call   sub_1_2600
        push   0
        push   0
        push   edi
        call   sub_1_2C9C
        push   0
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_2BD4
        cmp    dword ptr [esi + 0x18d], 0
        jne    loc_1_18BC
        xor    edx, edx
        mov    al, 0xf
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
        cmp    byte ptr [edi + 0x153], 0
        jne    loc_1_19A6
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [edi + 0x12]
        mov    cx, word ptr [edi + 0xe]
        add    eax, 0x5f2
        cmp    eax, ecx
        jge    loc_1_1965
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_1959
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        push   edi
        call   sub_1_19C0
        cmp    eax, 0x103
        jne    loc_1_1949
        mov    dword ptr [ebx + 0x2c], esi
        lea    ecx, [ebx + 0x24]
        inc    dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x142], 0
        jne    loc_1_1931
        mov    dword ptr [edi + 0x142], ebx
        jmp    loc_1_193A
loc_1_1931:
        mov    edx, dword ptr [edi + 0x146]
        mov    dword ptr [edx + 0x24], ebx
loc_1_193A:
        mov    dword ptr [ecx], 0
        mov    dword ptr [edi + 0x146], ebx
        dec    dword ptr [esi + 0x14]
loc_1_1949:
        mov    word ptr [edi + 0x1f4], 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_1959:
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_1965:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 0x2c], esi
        lea    eax, [ebx + 0x24]
        inc    dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x13a], 0
        jne    loc_1_1982
        mov    dword ptr [edi + 0x13a], ebx
        jmp    loc_1_198B
loc_1_1982:
        mov    ecx, dword ptr [edi + 0x13e]
        mov    dword ptr [ecx + 0x24], ebx
loc_1_198B:
        mov    dword ptr [eax], 0
        mov    eax, 0x103
        mov    dword ptr [edi + 0x13e], ebx
        pop    edi
        inc    dword ptr [esi + 0x14]
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_19A6:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_19C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        lea    eax, [ebp - 0x2c]
        push   esi
        lea    ecx, [ebp - 0x10]
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 4], si
        push   eax
        push   ecx
        lea    edx, [ebp - 0x30]
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    dword ptr [ebp - 0x20], esi
        push   eax
        call   sub_1_2CC0
        cmp    dword ptr [ebp - 0x2c], esi
        je     loc_1_1CC4
        cmp    dword ptr [ebp - 0x2c], 0x5ea
        ja     loc_1_1CC4
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edx
        call   sub_1_2CE4
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x18]
        mov    di, word ptr [esi + 0xe]
        mov    ecx, eax
        and    ecx, 0xffff
        mov    edx, edi
        add    ecx, 8
        and    edx, 0xffff
        cmp    ecx, edx
        jge    loc_1_1A38
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    loc_1_1A40
loc_1_1A38:
        sub    ax, di
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_1A40:
        add    ax, 8
        add    edx, 0xc
        out    dx, ax
        push   3
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xe
        push   eax
        call   sub_1_2B44
        push   6
        mov    ecx, dword ptr [ebp - 0xc]
        mov    di, 6
        push   ecx
        lea    edx, [ebp - 0x38]
        push   edx
        call   sub_1_2D7C
        add    dword ptr [ebp - 0xc], 6
        mov    bx, word ptr [ebp - 0x30]
        sub    dword ptr [ebp - 8], 6
        test   bx, bx
        je     loc_1_1B21
loc_1_1A85:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1AF5
        cmp    word ptr [ebp - 4], 0
        je     loc_1_1AA8
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 0xc]
        add    edx, 0xf
        mov    al, byte ptr [eax]
        dec    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0xc]
        out    dx, al
loc_1_1AA8:
        test   dword ptr [ebp - 8], 0xfffffffe
        je     loc_1_1ACA
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xe
        push   eax
        call   sub_1_2B44
loc_1_1ACA:
        test   byte ptr [ebp - 8], 1
        je     loc_1_1AEB
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [ebp - 4], 1
        add    edx, 0xe
        mov    al, byte ptr [ecx + eax - 1]
        out    dx, al
        jmp    loc_1_1AF1
loc_1_1AEB:
        mov    word ptr [ebp - 4], 0
loc_1_1AF1:
        add    di, word ptr [ebp - 8]
loc_1_1AF5:
        cmp    bx, 1
        jbe    loc_1_1B19
        lea    eax, [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        call   sub_1_2D08
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 0x10]
        push   edx
        push   eax
        call   sub_1_2CE4
loc_1_1B19:
        dec    bx
        jne    loc_1_1A85
loc_1_1B21:
        cmp    word ptr [ebp - 4], 0
        je     loc_1_1B35
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        xor    al, al
        add    edx, 0xf
        out    dx, al
loc_1_1B35:
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    dword ptr [ebp - 0x14], eax
        lea    ecx, [eax + 0xe]
        mov    edx, ecx
        xor    eax, eax
        out    dx, ax
        cmp    di, 0x3c
        ja     loc_1_1B76
        mov    ebx, 0x3c
        mov    eax, edi
        and    eax, 0xffff
        sub    ebx, eax
        sar    ebx, 1
        test   bx, bx
        je     loc_1_1B6C
loc_1_1B62:
        mov    edx, ecx
        xor    eax, eax
        out    dx, ax
        dec    bx
        jne    loc_1_1B62
loc_1_1B6C:
        mov    word ptr [ebp - 4], 0
        mov    di, 0x3c
loc_1_1B76:
        mov    bx, word ptr [esi + 0x18]
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0xc
        mov    edx, ebx
        and    edx, 0xffff
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x1c], edx
        mov    edx, eax
        mov    eax, dword ptr [ebp - 0x1c]
        out    dx, ax
        mov    edx, ecx
        mov    eax, 4
        out    dx, ax
        xor    eax, eax
        out    dx, ax
        mov    edx, edi
        mov    eax, dword ptr [ebp - 0x1c]
        and    edx, 0xffff
        lea    edx, [edx + eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0xe]
        mov    dword ptr [ebp - 0x24], edx
        mov    dword ptr [ebp - 0x18], eax
        cmp    edx, eax
        jge    loc_1_1BC6
        lea    eax, [ebx + edi]
        jmp    loc_1_1BD0
loc_1_1BC6:
        mov    ax, di
        sub    ax, word ptr [esi + 0xe]
        add    ax, bx
loc_1_1BD0:
        add    ax, 8
        mov    edx, ecx
        out    dx, ax
        mov    ax, di
        mov    edx, ecx
        sub    ax, word ptr [ebp - 4]
        out    dx, ax
        cmp    word ptr [esi + 0x14], 0
        mov    word ptr [ebp - 4], ax
        je     loc_1_1BFC
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        out    dx, al
        mov    word ptr [esi + 0x14], 0
        jmp    loc_1_1C22
loc_1_1BFC:
        mov    ax, word ptr [esi + 0x1a]
        mov    edx, dword ptr [ebp - 0x28]
        out    dx, ax
        mov    ax, word ptr [esi + 0x1c]
        mov    edx, ecx
        or     ah, 0x80
        and    eax, 0xffff
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        inc    edx
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x1c
        out    dx, al
loc_1_1C22:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 0x1c], ax
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 6
        cmp    eax, dword ptr [ebp - 0x18]
        jge    loc_1_1C3A
        lea    eax, [ebx + 6]
        jmp    loc_1_1C45
loc_1_1C3A:
        mov    ax, bx
        sub    ax, word ptr [esi + 0xe]
        add    ax, 6
loc_1_1C45:
        mov    word ptr [esi + 0x1a], ax
        mov    ecx, dword ptr [ebp - 0x18]
        lea    eax, [edi + 8]
        add    word ptr [esi + 0x12], ax
        cmp    dword ptr [ebp - 0x24], ecx
        jge    loc_1_1C65
        lea    eax, [ebx + edi]
        add    ax, 8
        mov    word ptr [esi + 0x18], ax
        jmp    loc_1_1C74
loc_1_1C65:
        sub    di, word ptr [esi + 0xe]
        add    di, bx
        add    di, 8
        mov    word ptr [esi + 0x18], di
loc_1_1C74:
        inc    word ptr [esi + 0x1e]
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [esi + 0x1ee]
        push   ecx
        inc    dword ptr [esi + 0x15d]
        call   sub_1_2AC0
        test   al, al
        je     loc_1_1CB2
        mov    dword ptr [ebp - 0x20], 0x103
        cmp    word ptr [ebp + 0x10], 0
        jne    loc_1_1CB8
        push   esi
        call   sub_1_1CE0
        mov    eax, dword ptr [ebp - 0x20]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1CB2:
        push   esi
        call   sub_1_1CE0
loc_1_1CB8:
        mov    eax, dword ptr [ebp - 0x20]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1CC4:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
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
        cmp    word ptr [esi + 0x1e], cx
        je     loc_1_1DD0
        xor    edi, edi
        mov    di, word ptr [esi]
        lea    eax, [edi + 0xc]
        mov    dword ptr [ebp - 8], eax
loc_1_1D06:
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x16]
        out    dx, ax
        lea    edx, [edi + 0xe]
        in     ax, dx
        test   al, 0x80
        je     loc_1_1DD0
        mov    byte ptr [ebp - 1], 1
        in     ax, dx
        test   ah, 0x20
        jne    loc_1_1D51
        mov    cl, 1
        test   al, 0x20
        je     loc_1_1D37
        inc    dword ptr [esi + 0x169]
        jmp    loc_1_1D77
loc_1_1D37:
        test   ah, 4
        je     loc_1_1D44
        inc    dword ptr [esi + 0x16d]
        jmp    loc_1_1D77
loc_1_1D44:
        test   ah, 1
        je     loc_1_1D77
        inc    dword ptr [esi + 0x171]
        jmp    loc_1_1D77
loc_1_1D51:
        test   al, 0x80
        je     loc_1_1D5D
        inc    dword ptr [esi + 0x179]
        jmp    loc_1_1D77
loc_1_1D5D:
        and    ax, 0xf
        cmp    ax, 1
        jne    loc_1_1D6F
        inc    dword ptr [esi + 0x17d]
        jmp    loc_1_1D77
loc_1_1D6F:
        jbe    loc_1_1D77
        inc    dword ptr [esi + 0x181]
loc_1_1D77:
        lea    edx, [edi + 0xe]
        in     ax, dx
        mov    bx, ax
        mov    dx, word ptr [esi + 0xe]
        sub    bx, word ptr [esi + 0x16]
        cmp    bx, dx
        jbe    loc_1_1D8F
        add    bx, dx
loc_1_1D8F:
        mov    word ptr [esi + 0x1f4], 3
        sub    word ptr [esi + 0x12], bx
        mov    bx, word ptr [esi + 0x1e]
        dec    bx
        test   cl, cl
        mov    word ptr [esi + 0x16], ax
        mov    word ptr [esi + 0x1e], bx
        je     loc_1_1DC7
        test   bx, bx
        lea    edx, [edi + 0xa]
        out    dx, ax
        je     loc_1_1DBF
        mov    edx, edi
        mov    al, 4
        out    dx, al
        jmp    loc_1_1DC5
loc_1_1DBF:
        mov    word ptr [esi + 0x14], 1
loc_1_1DC5:
        xor    cl, cl
loc_1_1DC7:
        test   bx, bx
        jne    loc_1_1D06
loc_1_1DD0:
        cmp    dword ptr [esi + 0x13a], 0
        je     loc_1_1E7D
loc_1_1DDD:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 0x12]
        mov    ax, word ptr [esi + 0xe]
        add    ecx, 0x5f2
        cmp    ecx, eax
        jge    loc_1_1E7D
        mov    ebx, dword ptr [esi + 0x13a]
        lea    edi, [ebx + 0x24]
        cmp    dword ptr [edi], 0
        jne    loc_1_1E0F
        mov    dword ptr [esi + 0x13e], 0
loc_1_1E0F:
        mov    eax, dword ptr [edi]
        push   1
        push   ebx
        mov    dword ptr [esi + 0x13a], eax
        push   esi
        call   sub_1_19C0
        cmp    eax, 0x103
        jne    loc_1_1E55
        mov    eax, dword ptr [edi + 8]
        inc    dword ptr [eax + 0x14]
        cmp    dword ptr [esi + 0x142], 0
        jne    loc_1_1E3E
        mov    dword ptr [esi + 0x142], ebx
        jmp    loc_1_1E47
loc_1_1E3E:
        mov    eax, dword ptr [esi + 0x146]
        mov    dword ptr [eax + 0x24], ebx
loc_1_1E47:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x146], ebx
        jmp    loc_1_1E67
loc_1_1E55:
        mov    edi, dword ptr [edi + 8]
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_2BDA
        dec    dword ptr [edi + 0x14]
loc_1_1E67:
        cmp    dword ptr [esi + 0x13a], 0
        mov    word ptr [esi + 0x1f4], 3
        jne    loc_1_1DDD
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
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x40
        push   esi
        lea    ecx, [ebp - 4]
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_1_2BAA
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], esi            ; -> obj1:0x2FE8
        mov    dword ptr [0], eax            ; -> obj1:0x2FF8
        mov    byte ptr [ebp - 0x40], 3
        mov    byte ptr [ebp - 0x3f], 0
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_1_23F0
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_1_2550
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_1_0FA0
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_1_18D0
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_2710
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_1_0000
        mov    dword ptr [ebp - 0x22], 0     ; -> loc_1_1030
        mov    dword ptr [ebp - 0x1e], 0     ; -> loc_1_25D0
        mov    dword ptr [ebp - 0x1a], 0     ; -> loc_1_1F70
        mov    dword ptr [ebp - 0x16], 0     ; -> loc_1_2230
        mov    edx, dword ptr [0]            ; -> loc_1_2F04
        mov    ecx, dword ptr [4]            ; -> obj1:0x2F08
        mov    eax, 0                        ; -> obj1:0x2FF0
        push   0x36
        mov    dword ptr [ebp - 0x12], edx
        lea    edx, [ebp - 0x40]
        push   edx
        mov    dword ptr [ebp - 0xe], ecx
        push   0
        lea    ecx, [ebp - 8]
        mov    dword ptr [4], eax            ; -> obj1:0x2FF4
        mov    dword ptr [0], eax            ; -> obj1:0x2FF0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> obj1:0x2FE4
        push   ecx
        call   sub_1_2BB6
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1F5E
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2BB0
        mov    eax, 0xc0000001
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
        mov    ecx, 0xa
        mov    ebp, esp
        sub    esp, 0x38
        mov    word ptr [ebp - 0x38], cx
        push   esi
        mov    word ptr [ebp - 0x36], cx
        push   edi
        mov    word ptr [ebp - 0x30], cx
        mov    eax, 0xc
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x2F18
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x2F30
        mov    dword ptr [ebp - 0x24], 0     ; -> obj1:0x2F48
        mov    dword ptr [ebp - 0x1c], 0     ; -> obj1:0x2F64
        mov    word ptr [ebp - 0x2e], cx
        mov    word ptr [ebp - 0x28], ax
        mov    dword ptr [ebp - 0x14], 0     ; -> obj1:0x2F80
        mov    word ptr [ebp - 0x26], ax
        mov    eax, 0xf
        mov    word ptr [ebp - 0x20], ax
        mov    word ptr [ebp - 0x1e], ax
        lea    edx, [ebp - 4]
        mov    word ptr [ebp - 0x18], cx
        mov    word ptr [ebp - 0x16], cx
        mov    ecx, dword ptr [0]            ; -> obj1:0x2EF8
        push   ecx
        push   0
        push   0x256
        push   edx
        call   sub_1_2C96
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_1FFD
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1FFD:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x95
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [0]            ; -> obj1:0x2FE4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1bd], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x154], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x26], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x1a1
        mov    dword ptr [ecx + 0x1a5], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x1a5]
        mov    dword ptr [eax + 0x1a1], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1a9
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x1ad], eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        lea    ecx, [ebp - 0x10]
        mov    edx, dword ptr [eax + 0x1ad]
        push   ecx
        mov    dword ptr [eax + 0x1a9], edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D14
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_209B
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2C9C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_209B:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        mov    ecx, 3
        jne    loc_1_20C3
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax + 4]
loc_1_20C3:
        mov    dl, 0x1f
        mov    eax, dword ptr [ebp - 4]
        sub    dl, cl
        push   1
        mov    byte ptr [eax + 0x1e9], dl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1ea], ecx
        lea    ecx, [ebp - 0x30]
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2104
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx], dx
        jmp    loc_1_210C
loc_1_2104:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 0x300
loc_1_210C:
        push   1
        lea    eax, [ebp - 0x28]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2138
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 2], dx
        jmp    loc_1_2141
loc_1_2138:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 0xffff
loc_1_2141:
        push   1
        lea    eax, [ebp - 0x20]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_216D
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 4], dx
        jmp    loc_1_2176
loc_1_216D:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0xffff
loc_1_2176:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2D1A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_219F
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 6], 0
        jmp    loc_1_21AB
loc_1_219F:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 6], edx
loc_1_21AB:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_2D20
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    edx, dword ptr [ecx + 0x1ea]
        push   eax
        push   edx
        call   sub_2_0000
loc_1_21CA:
        cmp    ax, 0xffff
        je     loc_1_2214
        push   eax
        call   sub_2_0080
loc_1_21D6:
        test   eax, eax
        je     loc_1_2214
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ecx
        call   sub_1_22A0
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_220A
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2C9C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_220A:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2214:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2C9C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_2230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        mov    byte ptr [esi + 0x154], 1
        lea    ecx, [esi + 0x1dd]
        push   ecx
        call   sub_1_2C78
        lea    ecx, [esi + 0x1c5]
        push   ecx
        call   sub_1_2C84
        mov    ecx, dword ptr [esi + 0x1ee]
        push   ecx
        call   sub_2_0340
loc_1_2269:
        mov    ecx, dword ptr [esi + 0x1c1]
        push   ecx
        call   sub_1_2BC8
        mov    ecx, dword ptr [esi + 0x1b5]
        lea    edx, [esi + 0x1b1]
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
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        lea    edi, [ebp - 0x28]
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, 1
        mov    dword ptr [ebp - 0x28], ebx
        mov    byte ptr [ebp - 0x24], bl
        mov    dword ptr [ebp - 0x1e], eax
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    dword ptr [ebp - 0x1a], ebx
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x16], ebx
        mov    dword ptr [ebp - 0x12], eax
        mov    dword ptr [ebp - 0xe], ecx
        lea    eax, [ebp - 0x28]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xa], 0x10
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [esi + 0x1bd]
        push   esi
        lea    edx, [esi + 0x1c1]
        push   eax
        push   edx
        call   sub_1_2BC2
        mov    edi, eax
        test   edi, edi
        je     loc_1_232C
        push   ebx
        mov    eax, dword ptr [esi + 0x1c1]
        push   2
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_2D68
        add    esp, 0x14
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_232C:
        push   esi
        call   sub_1_2640
        push   esi
        call   sub_1_26E0
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        call   sub_1_02C0
        test   al, al
        jne    loc_1_2360
        mov    eax, dword ptr [esi + 0x1c1]
        push   eax
        call   sub_1_2BC8
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_2360:
        lea    eax, [esi + 0x1ee]
        lea    ecx, [esi + 0x1b9]
        push   eax
        lea    edx, [esi + 0x2d]
        push   ecx
        mov    dword ptr [esi + 0x1e5], esi
        mov    dword ptr [esi + 0x1dd], 0
        mov    dword ptr [esi + 0x1e1], 0    ; -> loc_1_0C30
        push   edx
        push   0                             ; -> loc_1_1870
        push   0                             ; -> loc_1_1750
        push   0                             ; -> loc_1_1820
        push   0x10
        call   sub_2_0220
loc_1_23A2:
        test   al, al
        jne    loc_1_23C0
        mov    eax, dword ptr [esi + 0x1c1]
        push   eax
        call   sub_1_2BC8
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_23C0:
        lea    ecx, [esi + 0x1b1]
        mov    dword ptr [ecx], 0            ; -> obj1:0x2FF0
        pop    edi
        mov    eax, dword ptr [4]            ; -> obj1:0x2FF4
        mov    dword ptr [esi + 0x1b5], eax
        mov    eax, dword ptr [4]            ; -> obj1:0x2FF4
        pop    esi
        pop    ebx
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    esp, ebp
        mov    dword ptr [4], ecx            ; -> obj1:0x2FF4
        pop    ebp
        ret    0x10
        db  090h
loc_1_23F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0x154], 0
        je     loc_1_240F
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_240F:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, edx
        je     loc_1_242C
        mov    eax, dword ptr [ebp + 0x14]
loc_1_241B:
        cmp    dword ptr [eax], 0
        je     loc_1_2428
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     loc_1_241B
loc_1_2428:
        cmp    ecx, edx
        jne    loc_1_2438
loc_1_242C:
        mov    eax, 0xc0010019
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2438:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], edx
        lea    edx, [ebp - 4]
        inc    dword ptr [esi + 0x136]
        mov    ecx, dword ptr [0]            ; -> obj1:0x2EF8
        push   ecx
        push   0
        push   0x43
        push   edx
        call   sub_1_2C96
        test   eax, eax
        je     loc_1_2467
        mov    eax, 0xc000009a
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2467:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0x100
        mov    byte ptr [ecx + 0x18], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x41], ax
        mov    word ptr [esi + 0x1f2], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        push   eax
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x1ee]
        push   edx
        call   sub_2_0390
loc_1_24CB:
        test   al, al
        jne    loc_1_24EA
        mov    esi, 0xc0000001
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2C9C
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_24EA:
        lea    eax, [esi + 0x1a1]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x1a5]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [esi + 0x1a5]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1a5], eax
        mov    eax, dword ptr [esi + 0x18d]
        inc    eax
        mov    dword ptr [esi + 0x18d], eax
        cmp    eax, 1
        jne    loc_1_2543
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi]
        push   0xea60
        add    edx, 3
        add    esi, 0x1dd
        out    dx, al
        push   esi
        call   sub_1_2C72
loc_1_2543:
        xor    esi, esi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_1_2550:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x18], al
        jne    loc_1_25C7
        mov    byte ptr [esi + 0x18], 1
        push   eax
        dec    dword ptr [edi + 0x18d]
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 0x1ee]
        push   eax
        push   ecx
        call   sub_2_0460
loc_1_257D:
        cmp    eax, 0xc001000e
        jne    loc_1_258F
        mov    eax, 0x103
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
        dec    dword ptr [esi + 0x14]
        call   sub_1_2600
        push   0
        push   0
        push   esi
        call   sub_1_2C9C
        cmp    dword ptr [edi + 0x18d], 0
        jne    loc_1_25C5
        xor    edx, edx
        mov    al, 0xf
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
        mov    eax, dword ptr [0]            ; -> obj1:0x2FE4
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2BBC
        push   0
        mov    ecx, dword ptr [0]            ; -> obj1:0x2FF8
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
        mov    esi, dword ptr [ecx + 0x1a1]
        sub    eax, esi
        cmp    eax, 0xfffffe5f
        je     loc_1_2634
loc_1_261B:
        mov    ax, word ptr [esi + 0x41]
        cmp    ax, dx
        jbe    loc_1_2627
        mov    dx, ax
loc_1_2627:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0xfffffe5f
        jne    loc_1_261B
loc_1_2634:
        mov    word ptr [ecx + 0x1f2], dx
        pop    esi
        pop    ebp
        ret    4
sub_1_2640:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x18d], ecx
        mov    dword ptr [eax + 0x136], edx
        mov    byte ptr [eax + 0x153], cl
        mov    dword ptr [eax + 0x155], ecx
        mov    byte ptr [eax + 0x135], dl
        mov    byte ptr [eax + 0x134], dl
        mov    dword ptr [eax + 0x15d], ecx
        mov    dword ptr [eax + 0x161], ecx
        mov    dword ptr [eax + 0x169], ecx
        mov    dword ptr [eax + 0x16d], ecx
        mov    dword ptr [eax + 0x171], ecx
        mov    dword ptr [eax + 0x175], ecx
        mov    dword ptr [eax + 0x179], ecx
        mov    dword ptr [eax + 0x17d], ecx
        mov    dword ptr [eax + 0x181], ecx
        mov    dword ptr [eax + 0x185], ecx
        mov    dword ptr [eax + 0x189], ecx
        mov    dword ptr [eax + 0x191], ecx
        mov    word ptr [eax + 0x22], cx
        mov    dword ptr [eax + 0x195], ecx
        pop    ebp
        mov    dword ptr [eax + 0x199], ecx
        mov    dword ptr [eax + 0x159], 0x12345678
        mov    dword ptr [eax + 0x19d], ecx
        ret    4
        db  08Dh, 049h, 000h
sub_1_26E0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x1e], cx
        mov    dword ptr [eax + 0x142], ecx
        mov    word ptr [eax + 0x22], cx
        pop    ebp
        mov    dword ptr [eax + 0x146], ecx
        mov    dword ptr [eax + 0x14a], ecx
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2710:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 8]
        add    edi, 0xe
        cmp    byte ptr [ecx + 0x153], 0
        jne    loc_1_2785
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_277A
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        inc    dword ptr [esi + 0x14]
        push   edi
        test   edx, edx
        jne    loc_1_2760
        mov    ecx, dword ptr [ecx + 0x14a]
        mov    edx, dword ptr [ebp + 0x14]
        push   ecx
        mov    eax, dword ptr [ebp + 0x18]
        push   edx
        push   0
        push   eax
        call   sub_1_2D0E
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebp
        ret    0x18
loc_1_2760:
        mov    edi, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        push   edi
        push   eax
        push   edx
        push   ecx
        call   sub_1_0E30
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebp
        ret    0x18
loc_1_277A:
        mov    eax, 0xc001000c
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_2785:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_2790:
        mov    al, byte ptr [0x2fdc]         ; -> obj1:0x2FDC
        cmp    al, 0
        jne    loc_1_27B5
        inc    byte ptr [0x2fdc]             ; -> obj1:0x2FDC
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_27B7
        push   0
        push   0
        call   sub_1_1E90
        cmp    eax, 0
        jne    loc_1_27B7
loc_1_27B5:
        clc    
        ret    
loc_1_27B7:
        stc    
        ret    
SPENDIS_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_27C8
        jmp    dword ptr [eax*4 + 0x27ca]    ; -> obj1:0x27CA
loc_1_27C8:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_2790
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_27C8
        dd  loc_1_2790
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2840:
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
        call   sub_1_2870
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_2870:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_2910
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_28B0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_28A5
        cmp    al, 0xff
        jne    loc_1_28A5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_28A5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_2917
loc_1_28A5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_2917
        db  08Bh, 0FFh
loc_1_28B0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_2900
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_28D6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_28DD
loc_1_28D6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_28DD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_28F0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_291A
        db  08Dh, 064h, 024h, 000h
loc_1_28F0:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_291A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2900:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_291A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2910:
        mov    dword ptr [ebp - 4], 0x20
loc_1_2917:
        mov    esi, dword ptr [ebp + 8]
loc_1_291A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_2A11
loc_1_2925:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    loc_1_297D
        test   al, 2
        je     loc_1_2969
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_09E0
        or     al, al
        je     loc_1_2969
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
        jne    loc_1_297D
loc_1_2969:
        test   byte ptr [ebp - 4], 2
        je     loc_1_2974
        test   bl, 4
        jne    loc_1_297D
loc_1_2974:
        test   bl, 0x20
        je     loc_1_2A06
loc_1_297D:
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
        call   sub_1_2BF2
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_2A06
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_29BF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_29BF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_29D0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_29D6
        db  08Bh, 0FFh
loc_1_29D0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_29D6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_29E5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2BF8
loc_1_29E5:
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
        call   sub_1_2C9C
        mov    edi, ebx
        jmp    loc_1_2A09
loc_1_2A06:
        mov    edi, dword ptr [edi + 0x10]
loc_1_2A09:
        or     edi, edi
        jne    loc_1_2925
loc_1_2A11:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
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
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_2AAE
loc_1_2A40:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_2AA7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2BF8
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_2AA7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_2A6C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_2A6C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_2A80
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_2A86
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A80:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_2A86:
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
        call   sub_1_2C9C
        mov    edi, ebx
        jmp    loc_1_2AAA
loc_1_2AA7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_2AAA:
        or     edi, edi
        jne    loc_1_2A40
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
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_2AD5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2AD5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_2B11
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_2B02
        cmp    bl, 0xff
        jne    loc_1_2B02
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_2B02
        test   al, 8
        je     loc_1_2AFB
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
        jne    loc_1_2B0A
        test   al, 2
        je     loc_1_2B29
loc_1_2B0A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2B11:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_2B29
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_2B29
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
sub_1_2B81:
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
sub_1_2BAA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_2BB0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_2BB6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_2BBC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_2BC2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_2BC8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_2BD4:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_2BDA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_2BF2:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_2BF8:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_2BFE:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_2C04:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
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
sub_1_2C72:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_2C78:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_2C7E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_2C84:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_2C90:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_2C96:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_2C9C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_2CC0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_2CE4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_2D08:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_2D0E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_2D14:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_2D1A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_2D20:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
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
sub_1_2D68:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_2D7C:
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
sub_1_2D9C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_2DBC
        or     ecx, ecx
        je     loc_1_2DD2
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_2DD0
loc_1_2DBC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_2DD0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_2DD0:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_2DD2:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
loc_1_2DD8:
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
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
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
        add    eax, 0xd000201
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
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
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
        bound  ebp, qword ptr ss:[ecx + 0x74]
        and    byte ptr [ebp + 0x74], al
        push   0x656e7265
        je     0x2f0a
        inc    ecx
        popal  
        jo     loc_1_2F63
        jb     loc_1_2EF2
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
        add    eax, 0x500                    ; -> loc_1_2EFC
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
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
        jae    0x2fa5
        imul   esi, dword ptr gs:[esi + 0x65], 0x72540072
        popal  
        outsb  dx, byte ptr [esi]
        jae    0x2fb1
        imul   esi, dword ptr gs:[esi + 0x65], 0x6f490072
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        push   edx
        popal  
        jns    loc_1_2F63
loc_1_2F63:
        add    byte ptr [ecx + 0x6f], cl
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        push   edx
        popal  
        jns    loc_1_2F73
loc_1_2F73:
        add    byte ptr [edx + 0x75], al
        jae    0x2fc6
        jne    loc_1_2FE7
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    0x2fd2
        jne    loc_1_2FF3
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 050h, 045h, 04Eh, 044h
        db  049h, 053h, 020h, 000h, 040h, 000h, 054h
        dd  SPENDIS_Control          ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h
        db  033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h
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
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xaa0  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_2_0160
        mov    word ptr [ebp - 2], ax
        test   ax, ax
        jne    loc_2_0024
        mov    ax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0024:
        xor    di, di
        cmp    word ptr [ebp - 2], di
        je     loc_2_005C
        mov    si, word ptr [ebp + 0xc]
        mov    bl, byte ptr [ebp + 8]
loc_2_0034:
        push   edi
        call   sub_2_01D0
        mov    byte ptr [0], al              ; -> obj3:0x0000
        test   al, al
        jne    loc_2_0054
        cmp    bl, byte ptr [0x19]           ; -> obj3:0x001D
        jne    loc_2_0054
        cmp    si, word ptr [0x1c]           ; -> obj3:0x0020
        je     loc_2_0069
loc_2_0054:
        inc    di
        cmp    di, word ptr [ebp - 2]
        jb     loc_2_0034
loc_2_005C:
        mov    ax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0069:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0x22
        push   eax
        call   sub_2_00C0
        mov    byte ptr [0], al              ; -> obj3:0x0000
        test   al, al
        je     loc_2_009E
        xor    eax, eax
        pop    ebp
        ret    4
loc_2_009E:
        mov    al, byte ptr [0x1a]           ; -> obj3:0x001E
        pop    ebp
        sub    al, 6
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0x1b                     ; -> obj3:0x001F
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_00C0:
        push   ebp
        mov    ecx, 0
        mov    ax, word ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    byte ptr [0], cl              ; -> obj3:0x0000
        mov    word ptr [0], ax              ; -> obj3:0x0004
        push   esi
        mov    word ptr [2], cx              ; -> obj3:0x0006
        push   edi
        mov    dl, byte ptr [ebp + 0xc]
        mov    byte ptr [4], dl              ; -> obj3:0x0008
        pushal 
        lea    ebx, [0]                      ; -> obj3:0x0004
        mov    ecx, 0x12
        mov    esi, 0
        mov    edx, 0
        mov    al, 7
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        jae    loc_2_010E
        jmp    loc_2_0144
loc_2_010E:
        mov    byte ptr [5], 0               ; -> obj3:0x0009
        mov    word ptr [0x10], 0x50         ; -> obj3:0x0014
        lea    ebx, [0]                      ; -> obj3:0x0004
        mov    ecx, 0x64
        mov    esi, 0
        mov    edx, 0
        mov    al, 0xd
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        jae    loc_2_0149
        mov    byte ptr [0], al              ; -> obj3:0x0000
        jmp    loc_2_0149
loc_2_0144:
        mov    byte ptr [0], al              ; -> obj3:0x0000
loc_2_0149:
        popal  
        mov    al, byte ptr [0]              ; -> obj3:0x0000
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0160:
        mov    byte ptr [2], 0               ; -> obj3:0x0006
        mov    byte ptr [0], 0x19            ; -> obj3:0x0000
        push   ebx
        push   esi
        push   edi
        pushal 
        lea    ebx, [0]                      ; -> obj3:0x0004
        mov    ecx, 0x64
        mov    esi, 0
        mov    edx, 0
        mov    al, 0xb
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        jb     loc_2_0196
        mov    byte ptr [0], al              ; -> obj3:0x0000
loc_2_0196:
        popal  
        cmp    byte ptr [2], 0x43            ; -> obj3:0x0006
        jne    loc_2_01C3
        cmp    byte ptr [3], 0x53            ; -> obj3:0x0007
        jne    loc_2_01C3
        cmp    byte ptr [0], 0               ; -> obj3:0x0000
        jne    loc_2_01BC
        mov    ax, word ptr [4]              ; -> obj3:0x0008
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_01BC:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_01C3:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_01D0:
        mov    byte ptr [0], 0               ; -> obj3:0x0000
        push   ebp
        mov    ax, word ptr [esp + 8]
        mov    ebp, esp
        mov    word ptr [0], ax              ; -> obj3:0x0004
        push   ebx
        push   esi
        push   edi
        pushal 
        lea    ebx, [0]                      ; -> obj3:0x0004
        mov    ecx, 0x25
        mov    esi, 0
        mov    edx, 0
        mov    al, 4
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        jae    loc_2_020D
        mov    byte ptr [0], al              ; -> obj3:0x0000
loc_2_020D:
        popal  
        mov    al, byte ptr [0]              ; -> obj3:0x0000
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0220:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x2fe0]       ; -> obj1:0x2FE0
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_2C96
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0254
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0254:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_026E
        mov    esi, 2
loc_2_026E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2C96
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_02A2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0340
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_02A2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_2C96
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_02D1
        push   eax
        call   sub_2_0340
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_02D1:
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
sub_2_0340:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0360
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_2C9C
loc_2_0360:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0376
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_2C9C
loc_2_0376:
        push   0
        push   0x32
        push   esi
        call   sub_1_2C9C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_03A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_03A9:
        mov    eax, dword ptr [0x2fe0]       ; -> obj1:0x2FE0
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2C96
        or     eax, eax
        je     loc_2_03C9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_03C9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_03D0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_03E3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_03D0
        jmp    loc_2_03F2
loc_2_03E3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_03F2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_0408
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0408:
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
sub_2_0460:
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
        call   sub_2_0800
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0489
        cmp    ebx, 0x103
        jne    loc_2_04A0
loc_2_0489:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0520
        or     eax, eax
        je     loc_2_04A0
        mov    ebx, eax
loc_2_04A0:
        or     ebx, ebx
        je     loc_2_04AC
        cmp    ebx, 0x103
        jne    loc_2_050B
loc_2_04AC:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_0506
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_04C2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_04C2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_04D1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_04D7
loc_2_04D1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_04D7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_04E6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2BF8
loc_2_04E6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_2C9C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_0506:
        mov    ebx, 0xc001000e
loc_2_050B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0520:
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
        call   sub_1_2D9C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_2D9C
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_05A4
        sub    eax, eax
        nop    
loc_2_0580:
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
        ja     loc_2_0580
loc_2_05A4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_060F
loc_2_05B1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_0604
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
        call   sub_1_2D9C
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
        call   sub_1_2D9C
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_0607
loc_2_0604:
        inc    dword ptr [ebp - 4]
loc_2_0607:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_05B1
loc_2_060F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_06FA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_0623:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_09E0
        or     al, al
        je     loc_2_065A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_06E8
loc_2_065A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_0734
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
        call   sub_1_2D9C
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
        call   sub_1_2D9C
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
loc_2_06E8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_0623
loc_2_06FA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_2_0785
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_2_0710:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_078B
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_0772
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_0772
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0779
loc_2_0734:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2D9C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_2D9C
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0772:
        mov    dword ptr [ebp - 8], 1
loc_2_0779:
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_2_0710
loc_2_0785:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_07F0
loc_2_078B:
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
        je     loc_2_07F2
        cmp    edi, 0x103
        je     loc_2_07F2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2D9C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2D9C
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_07F0:
        sub    edi, edi
loc_2_07F2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_0800:
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
        je     loc_2_083D
        lea    esp, [esp]
loc_2_0830:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0830
loc_2_083D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_2_0871
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
        je     loc_2_0873
        cmp    eax, 0x103
        je     loc_2_0873
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0871:
        sub    eax, eax
loc_2_0873:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_0880:
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
        je     loc_2_08B2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_08A7:
        test   dword ptr [esi], edx
        je     loc_2_08AC
        inc    eax
loc_2_08AC:
        add    esi, 4
        dec    edi
        jne    loc_2_08A7
loc_2_08B2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_08C0:
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
        jbe    loc_2_093C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_0900:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_092A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0956
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_2D9C
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_092A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_0900
loc_2_093C:
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
loc_2_0956:
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
sub_2_0980:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_09B0
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_09B0:
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
        call   sub_1_2D9C
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_09E0:
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
        je     loc_2_0A79
        dec    esi
        cmp    esi, edx
        jb     loc_2_0A79
        nop    
loc_2_0A00:
        cmp    edx, ecx
        jb     loc_2_0A79
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_0A23
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0A57
loc_2_0A23:
        jbe    loc_2_0A30
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0A57
        db  08Bh, 0FFh
loc_2_0A30:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_0A47
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0A57
loc_2_0A47:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_0A57
        mov    dword ptr [ebp - 4], 0
loc_2_0A57:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0A89
        jle    loc_2_0A68
        or     edx, edx
        je     loc_2_0A79
        lea    esi, [edx - 1]
        jmp    loc_2_0A6B
loc_2_0A68:
        lea    ecx, [edx + 1]
loc_2_0A6B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_0A00
loc_2_0A79:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0A89:
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

VxD__LDATA SEGMENT   ; object 3  vsize=0x68  RW-
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h, 024h, 04Eh, 044h, 049h, 053h, 033h, 020h, 050h, 043h, 04Dh
        db  043h, 049h, 041h, 020h, 045h, 074h, 068h, 065h, 072h, 06Eh, 065h, 074h, 020h, 064h, 072h, 069h
        db  076h, 065h, 072h, 020h, 066h, 06Fh, 072h, 020h, 057h, 039h, 035h, 000h, 000h, 00Bh, 053h, 050h
        db  045h, 04Eh, 044h, 049h, 053h, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS
