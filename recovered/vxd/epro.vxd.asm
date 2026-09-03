; EPRO -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5817 instructions recovered by recursive descent

Declare_Virtual_Device EPRO, 3, 10, EPRO_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x298c  R-X
sub_1_0000:
        xor    dx, dx
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 0x1a6]
        sub    eax, esi
        cmp    eax, 0xfffffe5a
        je     loc_1_0034
loc_1_001B:
        mov    ax, word ptr [esi + 0x41]
        cmp    dx, ax
        jae    loc_1_0027
        mov    dx, ax
loc_1_0027:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0xfffffe5a
        jne    loc_1_001B
loc_1_0034:
        mov    word ptr [ecx + 0x1f7], dx
        pop    esi
        pop    ebp
        ret    4
sub_1_0040:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x192], ecx
        mov    dword ptr [eax + 0x13b], edx
        mov    byte ptr [eax + 0x158], cl
        mov    dword ptr [eax + 0x15a], ecx
        mov    byte ptr [eax + 0x13a], dl
        mov    byte ptr [eax + 0x139], dl
        mov    dword ptr [eax + 0x162], ecx
        mov    dword ptr [eax + 0x166], ecx
        mov    dword ptr [eax + 0x16e], ecx
        mov    dword ptr [eax + 0x172], ecx
        mov    dword ptr [eax + 0x176], ecx
        mov    dword ptr [eax + 0x17a], ecx
        mov    dword ptr [eax + 0x17e], ecx
        mov    dword ptr [eax + 0x182], ecx
        mov    dword ptr [eax + 0x186], ecx
        mov    dword ptr [eax + 0x18a], ecx
        mov    dword ptr [eax + 0x18e], ecx
        mov    dword ptr [eax + 0x196], ecx
        mov    word ptr [eax + 0x25], cx
        mov    dword ptr [eax + 0x19a], ecx
        pop    ebp
        mov    dword ptr [eax + 0x19e], ecx
        mov    dword ptr [eax + 0x1a2], ecx
        mov    byte ptr [eax + 0xf], cl
        mov    dword ptr [eax + 0x15e], 0x12345678
        ret    4
sub_1_00E0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x21], cx
        mov    dword ptr [eax + 0x147], ecx
        mov    word ptr [eax + 0x25], cx
        pop    ebp
        mov    word ptr [eax + 0x29], cx
        mov    dword ptr [eax + 0x14b], ecx
        mov    dword ptr [eax + 0x14f], ecx
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
        mov    esi, dword ptr [ebp + 0xc]
        mov    dx, word ptr [esi]
        add    edx, 3
        in     al, dx
        and    al, 0xf
        cmp    al, 0xf
        je     loc_1_01F4
        cmp    byte ptr [esi + 0x158], 0
        mov    al, 0xf
        out    dx, al
        jne    loc_1_01F4
        inc    dword ptr [esi + 0x13b]
        mov    word ptr [esi + 0x27], 3
loc_1_0157:
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        in     al, dx
        mov    bl, al
        and    bl, 7
        jne    loc_1_0175
        cmp    dword ptr [esi + 0x147], 0
        jne    loc_1_0175
        cmp    word ptr [esi + 0x27], 0
        jne    loc_1_01C7
loc_1_0175:
        mov    al, bl
        and    al, 3
        jne    loc_1_0182
        cmp    word ptr [esi + 0x27], 0
        jne    loc_1_018D
loc_1_0182:
        out    dx, al
        push   esi
        call   sub_1_0200
        test   al, al
        je     loc_1_01F4
loc_1_018D:
        mov    al, bl
        and    al, 8
        je     loc_1_019A
        xor    edx, edx
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
loc_1_019A:
        mov    al, bl
        and    al, 4
        je     loc_1_01AD
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi]
        inc    edx
        out    dx, al
        call   sub_1_1EC0
loc_1_01AD:
        cmp    dword ptr [esi + 0x147], 0
        je     loc_1_0157
loc_1_01B6:
        push   esi
        call   sub_1_03F0
        cmp    dword ptr [esi + 0x147], 0
        jne    loc_1_01B6
        jmp    loc_1_0157
loc_1_01C7:
        xor    edx, edx
        mov    al, 8
        dec    dword ptr [esi + 0x13b]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
        cmp    byte ptr [esi + 0x138], 0
        je     loc_1_01F4
        mov    byte ptr [esi + 0x138], 0
        mov    eax, dword ptr [esi + 0x1f3]
        push   eax
        call   sub_1_2640
loc_1_01F4:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0200:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x27], 3
loc_1_0212:
        dec    word ptr [esi + 0x27]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x23]
        lea    edx, [ecx + 0xc]
        out    dx, ax
        lea    edx, [ecx + 0xe]
        in     ax, dx
        mov    di, ax
        in     ax, dx
        test   ah, 0x20
        je     loc_1_03B3
        test   di, 8
        je     loc_1_03B3
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        inc    dword ptr [esi + 0x166]
        in     ax, dx
        cmp    ax, 0x5ea
        mov    di, ax
        ja     loc_1_0388
        mov    bx, word ptr [esi + 0x1f7]
        cmp    di, bx
        ja     loc_1_026D
        mov    ebx, edi
loc_1_026D:
        cmp    byte ptr [esi + 0xf], 0
        je     loc_1_02DE
        lea    edx, [ecx + 3]
        add    ecx, 0xc
        in     al, dx
        mov    byte ptr [ebp - 4], al
        or     al, 0x10
        out    dx, al
        mov    ax, bx
        shr    ax, 2
        mov    edx, eax
        lea    eax, [esi + 0x38]
        and    edx, 0xffff
        push   edx
        push   eax
        push   ecx
        call   sub_1_221E
        test   bl, 2
        je     loc_1_02B6
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0xfffc
        add    edx, 0xc
        in     ax, dx
        mov    word ptr [ecx + esi + 0x38], ax
loc_1_02B6:
        test   bl, 1
        je     loc_1_02D0
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0xffff
        add    edx, 0xc
        in     al, dx
        mov    byte ptr [ecx + esi + 0x37], al
loc_1_02D0:
        xor    edx, edx
        mov    al, byte ptr [ebp - 4]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
        jmp    loc_1_0318
loc_1_02DE:
        lea    eax, [esi + 0x38]
        add    ecx, 0xe
        mov    ax, bx
        shr    ax, 1
        mov    edx, eax
        lea    eax, [esi + 0x38]
        and    edx, 0xffff
        push   edx
        push   eax
        push   ecx
        call   sub_1_2209
        test   bl, 1
        je     loc_1_0318
        xor    edx, edx
        mov    ecx, ebx
        mov    dx, word ptr [esi]
        and    ecx, 0xffff
        add    edx, 0xe
        in     al, dx
        mov    byte ptr [ecx + esi + 0x37], al
loc_1_0318:
        mov    cx, word ptr [esi + 0x23]
        add    cx, 8
        cmp    byte ptr [esi + 0x10], 0
        mov    word ptr [esi + 0x25], di
        je     loc_1_0338
        cmp    cx, 0x8000
        jb     loc_1_0344
        sub    cx, 0x6000
        jmp    loc_1_0344
loc_1_0338:
        cmp    cx, 0x3000
        jae    loc_1_0344
        add    cx, 0x3000
loc_1_0344:
        mov    eax, edi
        lea    edx, [esi + 0x46]
        and    eax, 0xffff
        and    ecx, 0xffff
        sub    eax, 0xe
        push   eax
        mov    eax, ebx
        and    eax, 0xffff
        lea    ebx, [esi + 0x38]
        sub    eax, 0xe
        push   eax
        push   edx
        push   0xe
        mov    edx, dword ptr [esi + 0x1f3]
        push   ebx
        push   ebx
        push   ecx
        push   edx
        call   sub_1_2460
        cmp    byte ptr [esi + 0x158], 0
        jne    loc_1_03BE
        mov    byte ptr [esi + 0x138], 1
loc_1_0388:
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [esi + 0x23], ax
        sub    ax, 2
        cmp    word ptr [esi + 0x11], ax
        jbe    loc_1_039E
        add    ax, word ptr [esi + 0x13]
loc_1_039E:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 6
        out    dx, ax
        cmp    word ptr [esi + 0x27], 0
        jne    loc_1_0212
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
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        add    eax, 0x1e2
        push   0x7d0
        push   eax
        call   sub_1_22FA
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_1_03F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 0x147]
        mov    dword ptr [ebx + 0x14f], edi
        lea    esi, [edi + 0x24]
        cmp    dword ptr [esi], 0
        jne    loc_1_041A
        mov    dword ptr [ebx + 0x14b], 0
loc_1_041A:
        mov    eax, dword ptr [esi]
        lea    ecx, [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        mov    dword ptr [ebx + 0x147], eax
        push   0
        push   0
        push   edi
        call   sub_1_2348
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0x10]
        push   edx
        push   eax
        call   sub_1_236C
        xor    edx, edx
        mov    dx, word ptr [ebx + 0x1f7]
        cmp    dword ptr [ebp - 8], edx
        jae    loc_1_0480
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        je     loc_1_0480
        cmp    eax, edx
        mov    ecx, eax
        jb     loc_1_0463
        mov    ecx, edx
loc_1_0463:
        lea    eax, [ebp - 8]
        lea    edx, [ebx + 0x38]
        push   eax
        and    ecx, 0xffff
        push   edx
        push   ecx
        push   0
        push   edi
        call   sub_1_04D0
        lea    ecx, [ebx + 0x38]
        mov    dword ptr [ebp - 4], ecx
loc_1_0480:
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [ebx + 0x138], 1
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe
        mov    ecx, dword ptr [ebx + 0x1f3]
        push   eax
        push   0xe
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   0
        push   ecx
        call   sub_1_2460
        push   0
        mov    ecx, dword ptr [esi + 0x10]
        push   edi
        push   ecx
        call   sub_1_2262
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_04D0:
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
        je     loc_1_0599
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_2348
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_0599
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_236C
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_0594
        mov    edi, dword ptr [ebp + 0xc]
loc_1_051F:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_054B
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_2390
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0594
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_236C
        jmp    loc_1_058F
loc_1_054B:
        test   edi, edi
        je     loc_1_0568
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_0560
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_058F
loc_1_0560:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_0568:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_1_0576
        mov    ebx, eax
loc_1_0576:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        add    esi, ebx
        call   sub_1_2404
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_058F:
        cmp    dword ptr [ebp + 0x10], esi
        ja     loc_1_051F
loc_1_0594:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_0599:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    eax, dword ptr [ebp + 0x1c]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [eax], ebx
        cmp    dword ptr [ebp + 0x10], ebx
        je     loc_1_07A7
        push   ebx
        lea    eax, [ebp - 0x14]
        push   eax
        lea    ecx, [ebp - 0x18]
        push   ecx
        mov    edx, dword ptr [ebp + 0x14]
        push   ebx
        push   edx
        call   sub_1_2348
        cmp    dword ptr [ebp - 0x18], ebx
        je     loc_1_07A7
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_236C
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x25]
        mov    ecx, dword ptr [ebp + 0x18]
        sub    eax, ecx
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_1_060F
        mov    dword ptr [ebp + 0x10], eax
loc_1_060F:
        test   cl, 1
        je     loc_1_0617
        dec    ecx
        mov    bl, 1
loc_1_0617:
        mov    ax, word ptr [ebp + 0xc]
        add    ax, cx
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_0635
        cmp    ax, 0x8000
        jb     loc_1_0646
        sub    ax, 0x6000
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    loc_1_064B
loc_1_0635:
        cmp    ax, 0x3000
        jae    loc_1_0646
        add    ax, 0x3000
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    loc_1_064B
loc_1_0646:
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_064B:
        add    edx, 0xc
        cmp    byte ptr [esi + 0xf], 0
        out    dx, ax
        je     loc_1_0665
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x10
        out    dx, al
loc_1_0665:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_078D
loc_1_066F:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_06A2
        lea    eax, [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_2390
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_078D
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_236C
        jmp    loc_1_0781
loc_1_06A2:
        mov    edi, dword ptr [ebp + 0x10]
        sub    edi, dword ptr [ebp - 0x10]
        cmp    edi, dword ptr [ebp - 0xc]
        jbe    loc_1_06B0
        mov    edi, dword ptr [ebp - 0xc]
loc_1_06B0:
        cmp    byte ptr [esi + 0xf], 0
        je     loc_1_0720
        test   bl, bl
        je     loc_1_06D2
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        dec    edi
        add    edx, 0xd
        inc    dword ptr [ebp - 0x10]
        in     al, dx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        dec    dword ptr [ebp - 0xc]
loc_1_06D2:
        mov    ebx, edi
        and    ebx, 0xfffffffc
        je     loc_1_06F1
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 8]
        shr    eax, 2
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xc
        push   eax
        call   sub_1_221E
loc_1_06F1:
        test   edi, 2
        je     loc_1_070A
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        add    edx, 0xc
        in     ax, dx
        mov    word ptr [ebx + ecx], ax
loc_1_070A:
        test   edi, 1
        je     loc_1_071C
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0xc
        jmp    loc_1_076E
loc_1_071C:
        xor    bl, bl
        jmp    loc_1_0778
loc_1_0720:
        test   bl, bl
        je     loc_1_073C
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        mov    dx, word ptr [esi]
        dec    edi
        add    edx, 0xf
        inc    dword ptr [ebp - 0x10]
        in     al, dx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        dec    dword ptr [ebp - 0xc]
loc_1_073C:
        test   edi, 0xfffffffe
        je     loc_1_075C
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 8]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xe
        push   eax
        call   sub_1_2209
loc_1_075C:
        test   edi, 1
        mov    bl, 0
        je     loc_1_0778
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0xe
loc_1_076E:
        mov    ecx, dword ptr [ebp - 8]
        mov    bl, 1
        in     al, dx
        mov    byte ptr [ecx + edi - 1], al
loc_1_0778:
        add    dword ptr [ebp - 8], edi
        add    dword ptr [ebp - 0x10], edi
        sub    dword ptr [ebp - 0xc], edi
loc_1_0781:
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x10], ecx
        ja     loc_1_066F
loc_1_078D:
        cmp    byte ptr [esi + 0xf], 0
        je     loc_1_079F
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
loc_1_079F:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], ecx
loc_1_07A7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_07B0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        cmp    byte ptr [ecx + 0x158], 0
        jne    loc_1_082D
        cmp    byte ptr [edx + 0x18], 0
        jne    loc_1_0823
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_1_07E3
        cmp    eax, 1
        je     loc_1_0803
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    8
loc_1_07E3:
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
        push   edx
        push   ecx
        call   sub_1_0A70
        pop    esi
        pop    ebp
        ret    8
loc_1_0803:
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
        push   edx
        push   ecx
        call   sub_1_0880
        pop    esi
        pop    ebp
        ret    8
loc_1_0823:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    8
loc_1_082D:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0840:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    dword ptr [ecx + 0x10], 2
        je     loc_1_0856
        mov    eax, 0xc00000bb
        pop    ebp
        ret    8
loc_1_0856:
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
        call   sub_1_0A70
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0880:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        cmp    eax, 0x1010103
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x20]
        mov    dword ptr [edi], 0
        ja     loc_1_08BF
        je     loc_1_0A10
        cmp    eax, 0x1010e
        je     loc_1_08CE
        cmp    eax, 0x1010f
        je     loc_1_094C
        cmp    eax, 0x10112
        je     loc_1_09D4
loc_1_08BF:
        mov    edi, 0xc0010017
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_08CE:
        mov    esi, dword ptr [ebp + 0x18]
        cmp    esi, 4
        je     loc_1_08E9
        mov    eax, 0xc0010015
        mov    dword ptr [edi], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_08E9:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_2404
        test   dword ptr [ebp - 8], 0xf054
        je     loc_1_091D
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 4
        mov    eax, 0xc00000bb
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_091D:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xc]
        mov    eax, dword ptr [edi + 0x1f3]
        push   edx
        push   eax
        call   sub_2_1610
        mov    edi, eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_094C:
        xor    edi, edi
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_096C
        mov    edi, 0xc0010014
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_096C:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2404
        cmp    dword ptr [ebp - 4], 0x100
        ja     loc_1_09C5
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x1f7]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_09B1
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 0x1f7], ax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x41], ax
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_09B1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x41], ax
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_09C5:
        mov    edi, 0xc0000001
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_09D4:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_09F2
        mov    edi, 0xc0010014
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_09F2:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x43
        xor    edi, edi
        push   ecx
        call   sub_1_2404
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_0A10:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, esi
        cdq    
        idiv   ecx
        test   edx, edx
        je     loc_1_0A30
        mov    eax, 0xc0010015
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_0A30:
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq    
        idiv   ecx
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        mov    edx, dword ptr [ecx + 0x1f3]
        push   eax
        push   edx
        call   sub_2_1330
        mov    edi, eax
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, edi
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0A70:
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
        ja     loc_1_0ABE
        je     loc_1_0DEB
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_0ADF
        jmp    dword ptr [eax*4]             ; -> obj1:0x0F10
loc_1_0ABE:
        cmp    eax, 0x1010102
        je     loc_1_0E10
        cmp    eax, 0x1010103
        je     loc_1_0E35
        cmp    eax, 0x1010104
        je     loc_1_0EC4
loc_1_0ADF:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_0AF1
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_0ECB
loc_1_0AF1:
        cmp    edx, 0x2020d
        ja     loc_1_0B11
        je     loc_1_0BF6
        sub    edx, 0x20101
        cmp    edx, 4
        ja     loc_1_0B2B
        jmp    dword ptr [edx*4]             ; -> obj1:0x0F5C
loc_1_0B11:
        cmp    edx, 0x1020101
        ja     loc_1_0B37
        je     loc_1_0C18
        cmp    edx, 0x2020e
        je     loc_1_0C07
loc_1_0B2B:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_0ECB
loc_1_0B37:
        cmp    edx, 0x1020201
        ja     loc_1_0B5F
        je     loc_1_0C4B
        cmp    edx, 0x1020102
        je     loc_1_0C29
        cmp    edx, 0x1020103
        je     loc_1_0C3A
        jmp    loc_1_0B2B
loc_1_0B5F:
        sub    edx, 0x1020202
        cmp    edx, 4
        ja     loc_1_0B2B
        jmp    dword ptr [edx*4]             ; -> obj1:0x0F70
loc_1_0B71:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x162]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0B82:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x166]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0B93:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17a]
        add    eax, dword ptr [edi + 0x16e]
        add    eax, dword ptr [edi + 0x172]
        add    eax, dword ptr [edi + 0x176]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0BB6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x19e]
        add    eax, dword ptr [edi + 0x18e]
        add    eax, dword ptr [edi + 0x1a2]
        add    eax, dword ptr [edi + 0x196]
        add    eax, dword ptr [edi + 0x18a]
        add    eax, dword ptr [edi + 0x19a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0BE5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x19e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0BF6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x196]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C07:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C18:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x19a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C29:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x182]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C3A:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x186]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C4B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C5C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C6D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x1a2]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C7E:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x176]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0C8F:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0CA0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x172]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0CB1:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_0CC6
        mov    esi, 0                        ; -> loc_1_2858
        mov    ecx, 0xa0
        jmp    loc_1_0ECB
loc_1_0CC6:
        mov    esi, 0                        ; -> obj1:0x28F8
        mov    ecx, 0x5c
        jmp    loc_1_0ECB
loc_1_0CD5:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_0ECB
loc_1_0CE1:
        lea    esi, [ebp - 0x1c]
        mov    ecx, 4
        jmp    loc_1_0ECB
loc_1_0CEE:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_0ECB
loc_1_0CFA:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_0ECB
loc_1_0D06:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_0ECB
loc_1_0D12:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x11]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0D23:
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x13]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0D34:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_0ECB
loc_1_0D40:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x2c
        lea    eax, [ebp - 0x14]
        push   edi
        lea    esi, [ebp - 0x14]
        push   eax
        call   sub_1_2404
        mov    byte ptr [ebp - 0x11], 0
        mov    ecx, 4
        jmp    loc_1_0ECB
loc_1_0D63:
        mov    esi, 0                        ; -> loc_1_2958
        mov    ecx, 0x1f
        jmp    loc_1_0ECB
loc_1_0D72:
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        je     loc_1_0D87
        mov    eax, dword ptr [ebx + 0xc]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_0ECB
loc_1_0D87:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x1f3]
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_0ECB
loc_1_0D9B:
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        jne    loc_1_0DB6
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x1f7]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0DB6:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x41]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0ECB
loc_1_0DC4:
        lea    esi, [ebp - 2]
        mov    ecx, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_0ECB
loc_1_0DD7:
        lea    esi, [ebp - 8]
        mov    ecx, 4
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_1_0ECB
loc_1_0DEB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x2c]
        add    edi, 0x2c
        lea    esi, [ebp - 0x24]
        mov    dword ptr [ebp - 0x24], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x20], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 0x1f], dl
        jmp    loc_1_0ECB
loc_1_0E10:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x32]
        add    edi, 0x32
        lea    esi, [ebp - 0x24]
        mov    dword ptr [ebp - 0x24], eax
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x20], cl
        mov    ecx, 6
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [ebp - 0x1f], dl
        jmp    loc_1_0ECB
loc_1_0E35:
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        jne    loc_1_0E6D
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x18]
        push   ecx
        mov    edi, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 0x18]
        mov    eax, dword ptr [edi + 0x1f3]
        push   eax
        push   ecx
        call   sub_2_1790
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        lea    ecx, [eax*2]
        jmp    loc_1_0ECB
loc_1_0E6D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    ecx, dword ptr [edi + 0x1f3]
        push   ecx
        lea    edx, [ebp - 0x18]
        push   edx
        call   sub_2_16D0
        cmp    dword ptr [ebp - 0x18], 0
        mov    esi, dword ptr [ebp + 0x14]
        jne    loc_1_0EA8
        mov    eax, dword ptr [ebp - 0x10]
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    loc_1_0ECB
loc_1_0EA8:
        mov    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [edi + 0x1f3]
        push   eax
        push   ecx
        call   sub_2_1690
        lea    edx, [eax + eax*2]
        lea    ecx, [edx*2]
        jmp    loc_1_0ECB
loc_1_0EC4:
        mov    dword ptr [ebp - 8], 0x10
loc_1_0ECB:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_0F02
        cmp    ecx, dword ptr [ebp + 0x18]
        jbe    loc_1_0EEE
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
loc_1_0EEE:
        mov    eax, dword ptr [ebp + 0x1c]
        test   ecx, ecx
        mov    dword ptr [eax], ecx
        je     loc_1_0F02
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   esi
        push   eax
        call   sub_1_2404
loc_1_0F02:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
        ; ---- dispatch table (29 entries) ----
        dd  loc_1_0CB1
        dd  loc_1_0CD5
        dd  loc_1_0CE1
        dd  loc_1_0CE1
        dd  loc_1_0CEE
        dd  loc_1_0CFA
        dd  loc_1_0D06
        dd  loc_1_0D12
        dd  loc_1_0D23
        dd  loc_1_0D34
        dd  loc_1_0D34
        dd  loc_1_0D40
        dd  loc_1_0D63
        dd  loc_1_0D72
        dd  loc_1_0D9B
        dd  loc_1_0DC4
        dd  loc_1_0D34
        dd  loc_1_0ADF
        dd  loc_1_0DD7
        dd  loc_1_0B71
        dd  loc_1_0B82
        dd  loc_1_0B93
        dd  loc_1_0BB6
        dd  loc_1_0BE5
        dd  loc_1_0C5C
        dd  loc_1_0C6D
        dd  loc_1_0C7E
        dd  loc_1_0C8F
        dd  loc_1_0CA0
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F90:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    edi, edi
        cmp    byte ptr [esi + 0x158], 0
        je     loc_1_0FB6
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_0FB6:
        xor    ecx, ecx
        mov    al, 0xb
        mov    cx, word ptr [esi]
        mov    edx, ecx
        out    dx, al
        mov    al, 0x80
        mov    ebx, dword ptr [ebp + 0xc]
        out    dx, al
        lea    edx, [ecx + 2]
        in     al, dx
        and    al, 0xfe
        or     al, 2
        test   bl, 0x24
        je     loc_1_0FD5
        or     al, 1
loc_1_0FD5:
        test   bl, 8
        je     loc_1_0FDC
        and    al, 0xfd
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
        je     loc_1_104C
        mov    dx, word ptr [esi]
        mov    al, 0x1e
        out    dx, al
        push   3
        call   sub_1_2318
        mov    bx, word ptr [esi + 0x11]
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    ax, bx
        lea    edx, [ecx + 4]
        out    dx, ax
        mov    ax, word ptr [esi + 0x13]
        lea    edx, [ecx + 6]
        add    ax, bx
        dec    ax
        out    dx, ax
        lea    edx, [ecx + 0xa]
        xor    eax, eax
        out    dx, ax
        mov    word ptr [esi + 0x21], ax
        mov    word ptr [esi + 0x1f], ax
        mov    word ptr [esi + 0x1d], ax
        mov    word ptr [esi + 0x15], ax
        mov    word ptr [esi + 0x19], ax
        mov    word ptr [esi + 0x1b], ax
        mov    word ptr [esi + 0x17], 1
        mov    bx, word ptr [esi + 0x11]
        mov    word ptr [esi + 0x23], bx
        jmp    loc_1_1051
loc_1_104C:
        mov    edi, 0xc0000001
loc_1_1051:
        mov    dx, word ptr [esi]
        mov    al, 8
        out    dx, al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_1060:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x158], 0
        je     loc_1_107C
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_107C:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x10
        jbe    loc_1_108D
        mov    eax, 0xc000009a
        pop    ebp
        ret    0x1c
loc_1_108D:
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        push   eax
        push   edx
        call   sub_1_1510
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        ret    0x1c
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
        dec    dword ptr [edi + 0x14]
        call   0
        push   0
        push   0
        push   edi
        call   sub_1_2324
        push   0
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_225C
        cmp    dword ptr [esi + 0x192], 0
        jne    loc_1_10FC
        xor    edx, edx
        mov    al, 0xf
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
        cmp    byte ptr [ebx + 0x158], 0
        jne    loc_1_1165
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_1157
        mov    byte ptr [ebx + 0x157], 1
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
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1165:
        mov    edi, 0xc001000d
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
        cmp    dword ptr [esi + 0x147], 0
        je     loc_1_11DC
        xor    edi, edi
loc_1_11A4:
        mov    edx, dword ptr [esi + 0x147]
        lea    eax, [edx + 0x24]
        cmp    dword ptr [eax], edi
        jne    loc_1_11B7
        mov    dword ptr [esi + 0x14b], edi
loc_1_11B7:
        mov    ecx, dword ptr [eax]
        push   0xc001000c
        push   edx
        mov    dword ptr [esi + 0x147], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_2262
        dec    dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x147], edi
        jne    loc_1_11A4
loc_1_11DC:
        cmp    dword ptr [esi + 0x13f], 0
        je     loc_1_1223
        xor    edi, edi
loc_1_11E7:
        mov    edx, dword ptr [esi + 0x13f]
        lea    eax, [edx + 0x24]
        cmp    dword ptr [eax], edi
        jne    loc_1_11FA
        mov    dword ptr [esi + 0x143], edi
loc_1_11FA:
        mov    ecx, dword ptr [eax]
        push   0xc001000c
        dec    word ptr [esi + 0x29]
        push   edx
        mov    dword ptr [esi + 0x13f], ecx
        mov    ebx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_2262
        dec    dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x13f], edi
        jne    loc_1_11E7
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
        mov    edi, dword ptr [esi + 0x1a6]
        sub    eax, edi
        cmp    eax, 0xfffffe5a
        je     loc_1_1277
loc_1_1249:
        push   0
        inc    dword ptr [edi + 0x14]
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   0x40010004
        push   eax
        call   sub_1_2286
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_228C
        mov    eax, esi
        dec    dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0xfffffe5a
        jne    loc_1_1249
loc_1_1277:
        mov    byte ptr [esi + 0x158], 1
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
        mov    edi, dword ptr [esi + 0x1a6]
        sub    eax, edi
        cmp    eax, 0xfffffe5a
        je     loc_1_12E8
        xor    ebx, ebx
loc_1_12AC:
        inc    dword ptr [edi + 0x14]
        cmp    dword ptr [ebp + 0xc], ebx
        je     loc_1_12C4
        push   ebx
        mov    eax, dword ptr [edi + 0x10]
        push   ebx
        push   0x40010007
        push   eax
        call   sub_1_2286
loc_1_12C4:
        push   4
        lea    eax, [ebp + 0xc]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        push   0x40010005
        push   ecx
        call   sub_1_2286
        mov    eax, esi
        dec    dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi]
        sub    eax, edi
        cmp    eax, 0xfffffe5a
        jne    loc_1_12AC
loc_1_12E8:
        mov    byte ptr [esi + 0x158], 0
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
        mov    al, 0xf
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        lea    esi, [edi + 0x1e2]
        add    edx, 3
        out    dx, al
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_2300
        push   edi
        call   sub_1_1770
        xor    ecx, ecx
        mov    word ptr [edi + 0x21], cx
        mov    word ptr [edi + 0x1f], cx
        mov    word ptr [edi + 0x17], 1
        mov    word ptr [edi + 0x1d], cx
        xor    edx, edx
        mov    dx, word ptr [edi]
        xor    eax, eax
        mov    word ptr [edi + 0x15], cx
        add    edx, 0xa
        out    dx, ax
        mov    word ptr [edi + 0x19], cx
        cmp    dword ptr [edi + 0x147], ecx
        mov    word ptr [edi + 0x1b], cx
        je     loc_1_1370
loc_1_1361:
        push   edi
        call   sub_1_03F0
        cmp    dword ptr [edi + 0x147], 0
        jne    loc_1_1361
loc_1_1370:
        push   edi
        call   sub_1_00E0
        cmp    byte ptr [edi + 0xd], 0
        jne    loc_1_13B2
        push   0xe
        push   0xe
        push   edi
        call   sub_1_16D0
        test   al, al
        jne    loc_1_13B2
        push   0
        mov    eax, dword ptr [edi + 0x1c6]
        push   0                             ; -> loc_1_1300
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
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
        push   0x1388
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
        mov    al, 0x40
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    cl, byte ptr [esi + 0xc]
        mov    di, word ptr [esi]
        mov    edx, edi
        out    dx, al
        lea    edx, [edi + 2]
        in     al, dx
        and    al, 0xf8
        or     cl, al
        push   esi
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 1]
        xor    edi, edi
        in     al, dx
        or     al, 0x80
        out    dx, al
        call   sub_1_1870
        mov    di, word ptr [esi]
        mov    bx, word ptr [esi + 0x11]
        lea    edx, [edi + 8]
        mov    al, bh
        mov    cl, bh
        out    dx, al
        lea    edx, [edi + 9]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x13]
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
        test   cx, cx
        mov    al, 0x14
        out    dx, al
        lea    edx, [edi + 3]
        in     al, dx
        je     loc_1_147C
        cmp    cx, 4
        jae    loc_1_147C
        or     al, 0x10
        and    al, 0x3f
        cmp    cx, 1
        jne    loc_1_1466
        and    al, 0xfb
        or     al, 0x20
loc_1_1466:
        cmp    cx, 2
        jne    loc_1_1470
        or     al, 4
        or     al, 0x20
loc_1_1470:
        cmp    cx, 3
        jne    loc_1_147E
        or     al, 4
        and    al, 0xdf
        jmp    loc_1_147E
loc_1_147C:
        and    al, 0x2f
loc_1_147E:
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    di, 3
        mov    al, 0x1e
        out    dx, al
loc_1_148B:
        push   0x64
        call   sub_1_2318
        dec    di
        jne    loc_1_148B
        mov    cx, word ptr [esi + 0x11]
        xor    edi, edi
        mov    di, word ptr [esi]
        mov    ax, cx
        lea    edx, [edi + 4]
        out    dx, ax
        mov    ax, word ptr [esi + 0x13]
        lea    edx, [edi + 6]
        add    ax, cx
        push   esi
        dec    ax
        out    dx, ax
        lea    edx, [edi + 0xa]
        xor    eax, eax
        out    dx, ax
        call   sub_1_1600
        mov    word ptr [esi + 0x17], 1
        xor    eax, eax
        mov    word ptr [esi + 0x21], ax
        mov    word ptr [esi + 0x1f], ax
        mov    cx, word ptr [esi + 0x11]
        mov    word ptr [esi + 0x1d], ax
        push   7
        mov    word ptr [esi + 0x15], ax
        push   7
        mov    word ptr [esi + 0x19], ax
        push   esi
        mov    word ptr [esi + 0x1b], ax
        mov    word ptr [esi + 0x23], cx
        call   sub_1_16D0
        test   al, al
        jne    loc_1_1501
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
        mov    al, 0xb
        out    dx, al
        add    edx, 3
        push   esi
        in     al, dx
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 2], al
        mov    al, 0xf
        out    dx, al
        call   sub_1_1770
        test   al, al
        je     loc_1_15E1
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
        je     loc_1_15A3
        mov    edi, dword ptr [ebp + 0x10]
loc_1_1576:
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
        call   sub_1_21CC
        mov    ecx, ebx
        and    ecx, 0xffff
        cmp    ecx, dword ptr [ebp + 0xc]
        jb     loc_1_1576
loc_1_15A3:
        push   3
        push   3
        push   esi
        call   sub_1_16D0
        test   al, al
        je     loc_1_15E1
        mov    ecx, 1
        xor    eax, eax
        mov    word ptr [esi + 0x21], ax
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [esi + 0x1f], ax
        mov    word ptr [esi + 0x1d], ax
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    word ptr [esi + 0x15], ax
        add    edx, 0xa
        out    dx, ax
        mov    word ptr [esi + 0x19], ax
        mov    word ptr [esi + 0x1b], ax
        mov    word ptr [esi + 0x17], cx
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
        ret    0xc
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
        out    dx, ax
        mov    si, 0x64
        mov    eax, 0xb6
        out    dx, ax
        mov    edx, ecx
        mov    al, 2
        out    dx, al
loc_1_165C:
        push   0x64
        call   sub_1_2318
        dec    si
        jne    loc_1_165C
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [edi]
        lea    edx, [ecx + 0xa]
        pop    edi
        out    dx, ax
        lea    edx, [ecx + 0xc]
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
        mov    al, 0x80
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
        mov    al, byte ptr [eax + esi + 0x32]
        out    dx, al
        jb     loc_1_16A4
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
        mov    di, 0x3e8
        inc    edx
        out    dx, al
loc_1_16ED:
        push   0x64
        call   sub_1_2318
        dec    di
        jne    loc_1_16ED
        mov    dx, word ptr [esi]
        mov    bl, byte ptr [ebp + 0xc]
        mov    di, 0x3e8
        mov    al, bl
        out    dx, al
loc_1_1705:
        push   0x64
        call   sub_1_2318
        dec    di
        jne    loc_1_1705
        cmp    bl, 0xe
        jne    loc_1_171B
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
        je     loc_1_173E
        mov    edx, ebx
        in     al, dx
        and    al, 0xf
        cmp    al, byte ptr [ebp + 0x10]
        je     loc_1_1759
        mov    edx, ecx
        mov    al, 8
        out    dx, al
        jmp    loc_1_1745
loc_1_173E:
        push   0x64
        call   sub_1_2318
loc_1_1745:
        inc    di
        cmp    di, 0x64
        jb     loc_1_171E
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1759:
        mov    byte ptr [ebp - 1], 1
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
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
        test   al, 0x20
        je     loc_1_17A0
        push   0x64
        call   sub_1_2318
        mov    ax, si
loc_1_1790:
        inc    si
        cmp    ax, 0x3e8
        jne    loc_1_177B
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
        sub    esp, 0x20
        mov    byte ptr [ebp - 0x20], 0x6a
        mov    byte ptr [ebp - 0x1f], 0xb5
        mov    byte ptr [ebp - 0x1e], 0xda
        mov    byte ptr [ebp - 0x1d], 0xed
        mov    byte ptr [ebp - 0x1c], 0xf6
        mov    byte ptr [ebp - 0x1b], 0xfb
        mov    byte ptr [ebp - 0x1a], 0x7d
        mov    byte ptr [ebp - 0x19], 0xbe
        mov    byte ptr [ebp - 0x18], 0xdf
        mov    byte ptr [ebp - 0x17], 0x6f
        mov    byte ptr [ebp - 0x16], 0x37
        mov    byte ptr [ebp - 0x15], 0x1b
        mov    byte ptr [ebp - 0x14], 0xd
        mov    byte ptr [ebp - 0x13], 0x86
        mov    byte ptr [ebp - 0x12], 0xc3
        mov    byte ptr [ebp - 0x11], 0x61
        mov    byte ptr [ebp - 0x10], 0xb0
        mov    byte ptr [ebp - 0xf], 0x58
        mov    byte ptr [ebp - 0xe], 0x2c
        mov    byte ptr [ebp - 0xd], 0x16
        mov    byte ptr [ebp - 0xc], 0x8b
        mov    byte ptr [ebp - 0xb], 0x45
        mov    byte ptr [ebp - 0xa], 0xa2
        mov    byte ptr [ebp - 9], 0xd1
        mov    byte ptr [ebp - 8], 0xe8
        mov    byte ptr [ebp - 7], 0x74
        mov    byte ptr [ebp - 6], 0x3a
        mov    byte ptr [ebp - 5], 0x9d
        mov    byte ptr [ebp - 4], 0xce
        mov    byte ptr [ebp - 3], 0xe7
        mov    byte ptr [ebp - 2], 0x73
        mov    byte ptr [ebp - 1], 0x43
        pushf  
        cli    
        xor    cx, cx
        mov    edx, 0x279
        xor    al, al
        out    dx, al
        out    dx, al
loc_1_1845:
        mov    eax, ecx
        mov    edx, 0x279
        inc    cx
        and    eax, 0xffff
        cmp    cx, 0x20
        mov    al, byte ptr [ebp + eax - 0x20]
        out    dx, al
        jb     loc_1_1845
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
        and    esi, 0xffff
        cmp    esi, -1
        je     loc_1_18B1
        cmp    cx, 1
        jne    loc_1_1898
        or     al, 0x42
        out    dx, al
loc_1_1898:
        cmp    cx, 3
        jne    loc_1_18A1
        and    al, 0xbd
        out    dx, al
loc_1_18A1:
        cmp    cx, 2
        jne    loc_1_18B6
        and    al, 0xbf
        pop    esi
        or     al, 2
        pop    ebp
        out    dx, al
        ret    4
loc_1_18B1:
        and    al, 0xbf
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
        lea    edi, [esi + 0xc]
        mov    cx, 0x10
        add    esi, 0xe
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
        jne    loc_1_18E8
        mov    cx, 3
        mov    edx, edi
        xor    bx, bx
        xor    eax, eax
        out    dx, ax
loc_1_1901:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    loc_1_1979
        add    cx, 3
        inc    bx
        cmp    bx, 0x10
        jb     loc_1_1901
        mov    cx, 0x10
        mov    edx, edi
        mov    eax, 0x8000
        out    dx, ax
        mov    ax, 9
loc_1_1927:
        mov    edx, esi
        out    dx, ax
        add    ax, 3
        dec    cx
        jne    loc_1_1927
        mov    cx, 0x10
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
loc_1_193D:
        mov    edx, esi
        xor    eax, eax
        out    dx, ax
        dec    cx
        jne    loc_1_193D
        mov    cx, 9
        mov    edx, edi
        xor    bx, bx
        mov    eax, 0x8000
        out    dx, ax
loc_1_1957:
        mov    edx, esi
        in     ax, dx
        cmp    ax, cx
        jne    loc_1_1984
        add    cx, 3
        inc    bx
        cmp    bx, 0x10
        jb     loc_1_1957
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1979:
        mov    ax, 0xffff
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
        cmp    byte ptr [edi + 0x158], 0
        mov    byte ptr [ebp - 1], al
        mov    al, 0xf
        out    dx, al
        jne    loc_1_1A79
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [edi + 0x15]
        mov    cx, word ptr [edi + 0x11]
        add    eax, 0x5f2
        cmp    eax, ecx
        jge    loc_1_1A39
        cmp    dword ptr [edi + 0x13f], 0
        jne    loc_1_1A39
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_1A32
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        push   esi
        push   edi
        call   sub_1_1AA0
        cmp    eax, 0x103
        mov    ecx, eax
        jne    loc_1_1A27
        mov    dword ptr [ebx + 0x2c], esi
        lea    eax, [ebx + 0x24]
        inc    dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x147], 0
        jne    loc_1_1A0F
        mov    dword ptr [edi + 0x147], ebx
        jmp    loc_1_1A18
loc_1_1A0F:
        mov    edx, dword ptr [edi + 0x14b]
        mov    dword ptr [edx + 0x24], ebx
loc_1_1A18:
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 0x14b], ebx
        dec    dword ptr [esi + 0x14]
loc_1_1A27:
        mov    word ptr [edi + 0x1f9], 3
        jmp    loc_1_1A7E
loc_1_1A32:
        mov    ecx, 0xc0010002
        jmp    loc_1_1A7E
loc_1_1A39:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 0x2c], esi
        lea    eax, [ebx + 0x24]
        inc    dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x13f], 0
        jne    loc_1_1A56
        mov    dword ptr [edi + 0x13f], ebx
        jmp    loc_1_1A5F
loc_1_1A56:
        mov    ecx, dword ptr [edi + 0x143]
        mov    dword ptr [ecx + 0x24], ebx
loc_1_1A5F:
        mov    ecx, 0x103
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 0x143], ebx
        inc    dword ptr [esi + 0x14]
        inc    word ptr [edi + 0x29]
        jmp    loc_1_1A7E
loc_1_1A79:
        mov    ecx, 0xc001000d
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
        sub    esp, 0x38
        push   ebx
        lea    eax, [ebp - 0x30]
        push   esi
        lea    ecx, [ebp - 0x14]
        push   edi
        lea    edx, [ebp - 0x2c]
        xor    edi, edi
        push   eax
        mov    word ptr [ebp - 8], di
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 0x10]
        push   edi
        mov    dword ptr [ebp - 0x20], edi
        push   eax
        call   sub_1_2348
        cmp    dword ptr [ebp - 0x30], edi
        je     loc_1_1EAD
        cmp    dword ptr [ebp - 0x30], 0x5ea
        ja     loc_1_1EAD
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_236C
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x1b]
        mov    bx, word ptr [esi + 0x11]
        mov    ecx, eax
        and    ecx, 0xffff
        mov    edx, ebx
        add    ecx, 8
        and    edx, 0xffff
        cmp    ecx, edx
        jge    loc_1_1B18
        xor    edx, edx
        mov    dx, word ptr [esi]
        jmp    loc_1_1B20
loc_1_1B18:
        sub    ax, bx
        xor    edx, edx
        mov    dx, word ptr [esi]
loc_1_1B20:
        add    ax, 8
        add    edx, 0xc
        cmp    byte ptr [esi + 0xf], 0
        out    dx, ax
        je     loc_1_1C20
        xor    edx, edx
        push   6
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp - 0x10]
        add    edx, 3
        push   ecx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        or     al, 0x10
        out    dx, al
        lea    edx, [ebp - 0x38]
        push   edx
        call   sub_1_2404
        mov    bx, word ptr [ebp - 0x2c]
        test   bx, bx
        je     loc_1_1CF8
loc_1_1B5E:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_1BEF
        cmp    word ptr [ebp - 8], 0
        je     loc_1_1B85
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 0x10]
        add    edx, 0xd
        mov    al, byte ptr [eax]
        dec    dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 0x10]
        out    dx, al
loc_1_1B85:
        test   dword ptr [ebp - 0xc], 0xfffffffc
        je     loc_1_1BA7
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        shr    eax, 2
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xc
        push   eax
        call   sub_1_21E1
loc_1_1BA7:
        test   byte ptr [ebp - 0xc], 2
        je     loc_1_1BC4
        xor    edx, edx
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [esi]
        and    eax, 0xfffffffc
        mov    ecx, dword ptr [ebp - 0x10]
        add    edx, 0xc
        mov    ax, word ptr [eax + ecx]
        out    dx, ax
loc_1_1BC4:
        test   byte ptr [ebp - 0xc], 1
        je     loc_1_1BE5
        xor    edx, edx
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ebp - 8], 1
        add    edx, 0xc
        mov    al, byte ptr [eax + ecx - 1]
        out    dx, al
        jmp    loc_1_1BEB
loc_1_1BE5:
        mov    word ptr [ebp - 8], 0
loc_1_1BEB:
        add    di, word ptr [ebp - 0xc]
loc_1_1BEF:
        cmp    bx, 1
        jbe    loc_1_1C13
        lea    eax, [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_2390
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [ebp - 0x14]
        push   edx
        push   eax
        call   sub_1_236C
loc_1_1C13:
        dec    bx
        jne    loc_1_1B5E
        jmp    loc_1_1CF8
loc_1_1C20:
        push   3
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xe
        push   eax
        call   sub_1_21CC
        push   6
        mov    ecx, dword ptr [ebp - 0x10]
        mov    di, 6
        push   ecx
        lea    edx, [ebp - 0x38]
        push   edx
        call   sub_1_2404
        add    dword ptr [ebp - 0x10], 6
        mov    bx, word ptr [ebp - 0x2c]
        sub    dword ptr [ebp - 0xc], 6
        test   bx, bx
        je     loc_1_1CF8
loc_1_1C5C:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_1CCC
        cmp    word ptr [ebp - 8], 0
        je     loc_1_1C7F
        inc    di
        xor    edx, edx
        mov    dx, word ptr [esi]
        mov    eax, dword ptr [ebp - 0x10]
        add    edx, 0xf
        mov    al, byte ptr [eax]
        dec    dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 0x10]
        out    dx, al
loc_1_1C7F:
        test   dword ptr [ebp - 0xc], 0xfffffffe
        je     loc_1_1CA1
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        shr    eax, 1
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 0xe
        push   eax
        call   sub_1_21CC
loc_1_1CA1:
        test   byte ptr [ebp - 0xc], 1
        je     loc_1_1CC2
        xor    edx, edx
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ebp - 8], 1
        add    edx, 0xe
        mov    al, byte ptr [eax + ecx - 1]
        out    dx, al
        jmp    loc_1_1CC8
loc_1_1CC2:
        mov    word ptr [ebp - 8], 0
loc_1_1CC8:
        add    di, word ptr [ebp - 0xc]
loc_1_1CCC:
        cmp    bx, 1
        jbe    loc_1_1CF0
        lea    eax, [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_2390
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [ebp - 0x14]
        push   edx
        push   eax
        call   sub_1_236C
loc_1_1CF0:
        dec    bx
        jne    loc_1_1C5C
loc_1_1CF8:
        cmp    word ptr [ebp - 8], 0
        je     loc_1_1D1C
        cmp    byte ptr [esi + 0xf], 0
        je     loc_1_1D0F
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0xd
        jmp    loc_1_1D17
loc_1_1D0F:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0xf
loc_1_1D17:
        inc    di
        xor    al, al
        out    dx, al
loc_1_1D1C:
        cmp    byte ptr [esi + 0xf], 0
        je     loc_1_1D2E
        xor    edx, edx
        mov    al, byte ptr [ebp - 4]
        mov    dx, word ptr [esi]
        add    edx, 3
        out    dx, al
loc_1_1D2E:
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    dword ptr [ebp - 4], eax
        lea    ebx, [eax + 0xe]
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        cmp    di, 0x3c
        ja     loc_1_1D6F
        mov    ecx, 0x3c
        mov    eax, edi
        and    eax, 0xffff
        sub    ecx, eax
        sar    ecx, 1
        test   cx, cx
        je     loc_1_1D65
loc_1_1D5B:
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        dec    cx
        jne    loc_1_1D5B
loc_1_1D65:
        mov    word ptr [ebp - 8], 0
        mov    di, 0x3c
loc_1_1D6F:
        mov    cx, word ptr [esi + 0x1b]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        mov    edx, ecx
        and    edx, 0xffff
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x1c], edx
        mov    edx, eax
        mov    eax, dword ptr [ebp - 0x1c]
        out    dx, ax
        mov    edx, ebx
        mov    eax, 4
        out    dx, ax
        xor    eax, eax
        out    dx, ax
        mov    edx, edi
        mov    eax, dword ptr [ebp - 0x1c]
        and    edx, 0xffff
        lea    edx, [edx + eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x11]
        mov    dword ptr [ebp - 0x24], edx
        mov    dword ptr [ebp - 0x18], eax
        cmp    edx, eax
        jge    loc_1_1DBF
        lea    eax, [ecx + edi]
        jmp    loc_1_1DC9
loc_1_1DBF:
        mov    ax, di
        sub    ax, word ptr [esi + 0x11]
        add    ax, cx
loc_1_1DC9:
        add    ax, 8
        mov    edx, ebx
        out    dx, ax
        mov    ax, di
        mov    edx, ebx
        sub    ax, word ptr [ebp - 8]
        out    dx, ax
        cmp    word ptr [esi + 0x17], 0
        mov    word ptr [ebp - 8], ax
        je     loc_1_1DF5
        mov    edx, dword ptr [ebp - 4]
        mov    al, 4
        out    dx, al
        mov    word ptr [esi + 0x17], 0
        jmp    loc_1_1E14
loc_1_1DF5:
        mov    ax, word ptr [esi + 0x1d]
        mov    edx, dword ptr [ebp - 0x28]
        out    dx, ax
        mov    ax, word ptr [esi + 0x1f]
        mov    edx, ebx
        or     ah, 0x80
        and    eax, 0xffff
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x1c
        out    dx, al
loc_1_1E14:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 0x1f], ax
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 6
        cmp    eax, dword ptr [ebp - 0x18]
        jge    loc_1_1E2C
        lea    eax, [ecx + 6]
        jmp    loc_1_1E37
loc_1_1E2C:
        mov    ax, cx
        sub    ax, word ptr [esi + 0x11]
        add    ax, 6
loc_1_1E37:
        mov    word ptr [esi + 0x1d], ax
        mov    edx, dword ptr [ebp - 0x18]
        lea    eax, [edi + 8]
        add    word ptr [esi + 0x15], ax
        cmp    dword ptr [ebp - 0x24], edx
        jge    loc_1_1E57
        lea    eax, [ecx + edi]
        add    ax, 8
        mov    word ptr [esi + 0x1b], ax
        jmp    loc_1_1E66
loc_1_1E57:
        sub    di, word ptr [esi + 0x11]
        add    di, cx
        add    di, 8
        mov    word ptr [esi + 0x1b], di
loc_1_1E66:
        inc    word ptr [esi + 0x21]
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x162]
        test   byte ptr [eax + 0x43], 2
        jne    loc_1_1E94
        lea    eax, [ebp - 0x38]
        mov    ecx, dword ptr [esi + 0x1f3]
        push   eax
        push   ecx
        call   sub_1_26E0
        test   al, al
        je     loc_1_1E94
        mov    dword ptr [ebp - 0x20], 0x103
loc_1_1E94:
        cmp    word ptr [ebp + 0x14], 0
        jne    loc_1_1EA1
        push   esi
        call   sub_1_1EC0
loc_1_1EA1:
        mov    eax, dword ptr [ebp - 0x20]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1EAD:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
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
        cmp    word ptr [esi + 0x21], cx
        je     loc_1_1FB4
        xor    edi, edi
        mov    di, word ptr [esi]
        lea    eax, [edi + 0xc]
        mov    dword ptr [ebp - 8], eax
loc_1_1EE6:
        mov    bx, word ptr [esi + 0x19]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, ebx
        and    eax, 0xffff
        out    dx, ax
        lea    edx, [edi + 0xe]
        in     ax, dx
        test   al, 0x80
        je     loc_1_1FB4
        mov    byte ptr [ebp - 1], 1
        in     ax, dx
        test   ah, 0x20
        jne    loc_1_1F36
        mov    cl, 1
        test   al, 0x20
        je     loc_1_1F1C
        inc    dword ptr [esi + 0x16e]
        jmp    loc_1_1F5C
loc_1_1F1C:
        test   ah, 4
        je     loc_1_1F29
        inc    dword ptr [esi + 0x172]
        jmp    loc_1_1F5C
loc_1_1F29:
        test   ah, 1
        je     loc_1_1F5C
        inc    dword ptr [esi + 0x176]
        jmp    loc_1_1F5C
loc_1_1F36:
        test   al, 0x80
        je     loc_1_1F42
        inc    dword ptr [esi + 0x17e]
        jmp    loc_1_1F5C
loc_1_1F42:
        and    ax, 0xf
        cmp    ax, 1
        jne    loc_1_1F54
        inc    dword ptr [esi + 0x182]
        jmp    loc_1_1F5C
loc_1_1F54:
        jbe    loc_1_1F5C
        inc    dword ptr [esi + 0x186]
loc_1_1F5C:
        lea    edx, [edi + 0xe]
        in     ax, dx
        mov    dx, ax
        sub    dx, bx
        mov    bx, word ptr [esi + 0x11]
        cmp    bx, dx
        jae    loc_1_1F73
        add    dx, bx
loc_1_1F73:
        mov    word ptr [esi + 0x1f9], 3
        mov    bx, word ptr [esi + 0x21]
        dec    bx
        mov    word ptr [esi + 0x19], ax
        sub    word ptr [esi + 0x15], dx
        test   cl, cl
        mov    word ptr [esi + 0x21], bx
        je     loc_1_1FAB
        test   bx, bx
        lea    edx, [edi + 0xa]
        out    dx, ax
        je     loc_1_1FA3
        mov    edx, edi
        mov    al, 4
        out    dx, al
        jmp    loc_1_1FA9
loc_1_1FA3:
        mov    word ptr [esi + 0x17], 1
loc_1_1FA9:
        xor    cl, cl
loc_1_1FAB:
        test   bx, bx
        jne    loc_1_1EE6
loc_1_1FB4:
        cmp    dword ptr [esi + 0x13f], 0
        je     loc_1_206F
loc_1_1FC1:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 0x15]
        mov    ax, word ptr [esi + 0x11]
        add    ecx, 0x5f2
        cmp    ecx, eax
        jge    loc_1_206F
        mov    ebx, dword ptr [esi + 0x13f]
        lea    edi, [ebx + 0x24]
        cmp    dword ptr [edi], 0
        jne    loc_1_1FF3
        mov    dword ptr [esi + 0x143], 0
loc_1_1FF3:
        mov    eax, dword ptr [edi]
        lea    ecx, [edi + 8]
        dec    word ptr [esi + 0x29]
        push   1
        push   ebx
        mov    dword ptr [esi + 0x13f], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], ecx
        push   eax
        push   esi
        call   sub_1_1AA0
        cmp    eax, 0x103
        mov    eax, dword ptr [ebp - 8]
        jne    loc_1_2048
        mov    ecx, dword ptr [eax]
        inc    dword ptr [ecx + 0x14]
        cmp    dword ptr [esi + 0x147], 0
        jne    loc_1_2031
        mov    dword ptr [esi + 0x147], ebx
        jmp    loc_1_203A
loc_1_2031:
        mov    eax, dword ptr [esi + 0x14b]
        mov    dword ptr [eax + 0x24], ebx
loc_1_203A:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x14b], ebx
        jmp    loc_1_2059
loc_1_2048:
        mov    edi, dword ptr [eax]
        push   0
        push   ebx
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_2262
        dec    dword ptr [edi + 0x14]
loc_1_2059:
        cmp    dword ptr [esi + 0x13f], 0
        mov    word ptr [esi + 0x1f9], 3
        jne    loc_1_1FC1
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
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        add    eax, 0xe
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 8]
        cmp    byte ptr [edx + 0x158], 0
        jne    loc_1_20F6
        cmp    byte ptr [esi + 0x18], 0
        jne    loc_1_20EB
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x1c]
        push   edi
        inc    dword ptr [esi + 0x14]
        push   eax
        test   ecx, ecx
        jne    loc_1_20D1
        mov    eax, dword ptr [edx + 0x14f]
        mov    ecx, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x18]
        push   ecx
        push   0
        push   edx
        call   sub_1_2396
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebp
        ret    0x18
loc_1_20D1:
        mov    eax, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x14]
        push   eax
        push   edi
        push   ecx
        push   edx
        call   sub_1_05B0
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebp
        ret    0x18
loc_1_20EB:
        mov    eax, 0xc001000c
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_20F6:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2110:
        mov    al, byte ptr [0]              ; -> obj1:0x27A0
        cmp    al, 0
        jne    loc_1_2135
        inc    byte ptr [0]                  ; -> obj1:0x27A0
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_2137
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_2137
loc_1_2135:
        clc    
        ret    
loc_1_2137:
        stc    
        ret    
EPRO_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_2148
        jmp    dword ptr [eax*4]             ; -> obj1:0x214A
loc_1_2148:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_2110
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2148
        dd  loc_1_2110
        db  0CCh, 0CCh, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 06Eh, 05Eh, 0C2h, 00Ch, 000h
sub_1_21CC:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
sub_1_21E1:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        cld    
        rep outsd dx, dword ptr [esi]
        pop    esi
        ret    0xc
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_2209:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_221E:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        cld    
        rep insd dword ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_2232:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_2238:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_223E:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_2244:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_224A:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_2250:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_225C:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_2262:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_227A:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_2280:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_2286:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_228C:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_22A4:
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
        int3   
        int3   
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_22FA:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_2300:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_2306:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_230C:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_2318:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_231E:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_2324:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_2348:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_236C:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_2390:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_2396:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_239C:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_23A2:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_23A8:
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
sub_1_23F0:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        int3   
        int3   
sub_1_2404:
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
sub_1_2424:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_2444
        or     ecx, ecx
        je     loc_1_245A
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_2458
loc_1_2444:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_2458
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_2458:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_245A:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
sub_1_2460:
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
        call   sub_1_2490
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_2490:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_2530
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_24D0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_24C5
        cmp    al, 0xff
        jne    loc_1_24C5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_24C5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_2537
loc_1_24C5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_2537
        db  08Bh, 0FFh
loc_1_24D0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_2520
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_24F6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_24FD
loc_1_24F6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_24FD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_2510
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_253A
        db  08Dh, 064h, 024h, 000h
loc_1_2510:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_253A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2520:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_253A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2530:
        mov    dword ptr [ebp - 4], 0x20
loc_1_2537:
        mov    esi, dword ptr [ebp + 8]
loc_1_253A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_2631
loc_1_2545:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_259D
        test   al, 2
        je     loc_1_2589
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_17F0
        or     al, al
        je     loc_1_2589
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_259D
loc_1_2589:
        test   byte ptr [ebp - 4], 2
        je     loc_1_2594
        test   bl, 4
        jne    loc_1_259D
loc_1_2594:
        test   bl, 0x20
        je     loc_1_2626
loc_1_259D:
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
        call   sub_1_227A
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_2626
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_25DF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_25DF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_25F0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_25F6
        db  08Bh, 0FFh
loc_1_25F0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_25F6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_2605
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2280
loc_1_2605:
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
        call   sub_1_2324
        mov    edi, ebx
        jmp    loc_1_2629
loc_1_2626:
        mov    edi, dword ptr [edi + 0x10]
loc_1_2629:
        or     edi, edi
        jne    loc_1_2545
loc_1_2631:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
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
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_26CE
loc_1_2660:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_26C7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2280
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_26C7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_268C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_268C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_26A0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_26A6
        db  005h, 000h, 000h, 000h, 000h
loc_1_26A0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_26A6:
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
        call   sub_1_2324
        mov    edi, ebx
        jmp    loc_1_26CA
loc_1_26C7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_26CA:
        or     edi, edi
        jne    loc_1_2660
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
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_26F5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_26F5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_2731
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_2722
        cmp    bl, 0xff
        jne    loc_1_2722
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_2722
        test   al, 8
        je     loc_1_271B
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
        jne    loc_1_272A
        test   al, 2
        je     loc_1_2749
loc_1_272A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2731:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_2749
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_2749
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2749:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'EPRO    '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  EPRO_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 002h, 000h, 000h, 090h, 0FFh, 0FFh, 0FFh, 0FFh
        db  045h, 050h, 052h, 04Fh, 000h, 000h, 000h, 000h
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
        add    byte ptr [edx + edx*2 + 0x41], dl
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [edx + edx*2 + 0x41], dl
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ecx + 0x4f], cl
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
        add    byte ptr [edx + 0x75], al
        jae    0x2872
        jne    0x2893
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    0x287e
        jne    loc_1_289F
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], dl
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_288D
        jne    0x28ae
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2848:
        push   ebx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_289D
        jne    0x28be
        bound  esp, qword ptr [ebp + 0x72]
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
loc_1_288D:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
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
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_2958:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x29c1
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x74], al
        push   0x78457265
        jo     0x29d9
        jae    0x29dd
        and    byte ptr [eax + 0x52], dl
        dec    edi
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x29e8
        jb     loc_1_2977
loc_1_2977:
        add    byte ptr [esi], dh
        db  030h, 00Dh, 00Ah
loc_1_297C:
        or     dword ptr [eax + 0x61], edx
        arpl   word ptr [ebx + 0x65], bp
        je     0x29a4
loc_1_2984:
        push   esp
        jb     0x29e8
        outsb  dx, byte ptr [esi]
loc_1_2988:
        jae    0x29f0
        db  065h, 072h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x18b0  R-X
sub_2_0000:
        push   ebp
        mov    eax, 5
        mov    ebp, esp
        sub    esp, 0x48
        mov    word ptr [ebp - 0x10], ax
        push   esi
        mov    word ptr [ebp - 0xe], ax
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   ecx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_27C0
        push   esi
        push   eax
        call   sub_1_2232
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], esi            ; -> loc_1_2988
        mov    byte ptr [ebp - 0x48], 3
        mov    byte ptr [ebp - 0x47], 0
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_2_0620
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_2_0790
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_07B0
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_1_1990
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_1_2080
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_1_1110
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_1_0840
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_297C
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x1a], edx
        mov    eax, 0                        ; -> obj1:0x2980
        mov    dword ptr [ebp - 0x16], ecx
        mov    dword ptr [4], eax            ; -> loc_1_2984
        mov    dword ptr [0], eax            ; -> obj1:0x2980
        push   0x36
        lea    edx, [ebp - 0x48]
        push   edx
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_2_0810
        mov    dword ptr [ebp - 0x22], 0     ; -> loc_2_00F0
        mov    dword ptr [ebp - 0x1e], 0     ; -> loc_2_03E0
        push   eax
        push   0                             ; -> obj1:0x2978
        push   ecx
        call   sub_1_223E
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_00DB
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2238
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_00DB:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
loc_2_00E0:
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_00F0:
        push   ebp
        mov    ecx, 0xa
        mov    ebp, esp
        sub    esp, 0x48
        mov    word ptr [ebp - 0x48], cx
        push   esi
        mov    word ptr [ebp - 0x46], cx
        push   edi
        mov    word ptr [ebp - 0x40], cx
        mov    eax, 0xc
        mov    dword ptr [ebp - 0x44], 0     ; -> obj1:0x27D4
        mov    dword ptr [ebp - 0x3c], 0     ; -> obj1:0x27EC
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x2804
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x2818
        mov    word ptr [ebp - 0x3e], cx
        mov    word ptr [ebp - 0x38], ax
        mov    dword ptr [ebp - 0x24], 0     ; -> obj1:0x282C
        mov    word ptr [ebp - 0x36], ax
        mov    eax, 8
        mov    word ptr [ebp - 0x30], ax
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_2848
        mov    word ptr [ebp - 0x2e], ax
        mov    eax, 0xd
        mov    word ptr [ebp - 0x28], cx
        mov    word ptr [ebp - 0x26], cx
        mov    ecx, dword ptr [0]            ; -> obj1:0x27B4
        mov    word ptr [ebp - 0x20], ax
        push   ecx
        mov    word ptr [ebp - 0x1e], ax
        push   0
        push   0x25b
        lea    edx, [ebp - 4]
        push   edx
        call   sub_1_231E
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_2_0191
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0191:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x96
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [0]            ; -> obj1:0x2978
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c2], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x159], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x2b], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x1a6
        mov    dword ptr [ecx + 0x1aa], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x1aa]
        mov    dword ptr [eax + 0x1a6], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1ae
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x1b2], eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        lea    ecx, [ebp - 0x10]
        mov    edx, dword ptr [eax + 0x1b2]
        push   ecx
        mov    dword ptr [eax + 0x1ae], edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_239C
        cmp    dword ptr [ebp - 8], 0
        push   0
        je     loc_2_0230
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2324
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0230:
        lea    eax, [ebp - 0x48]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        call   sub_1_23A2
        cmp    dword ptr [ebp - 8], 0
        mov    ecx, 5
        jne    loc_2_0256
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax + 4]
loc_2_0256:
        mov    dl, 0x1f
        mov    eax, dword ptr [ebp - 4]
        sub    dl, cl
        push   1
        mov    byte ptr [eax + 0x1ee], dl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1ef], ecx
        lea    ecx, [ebp - 0x40]
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_23A2
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0297
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [edx], cx
        jmp    loc_2_029F
loc_2_0297:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 0x300
loc_2_029F:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_23A2
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_02CB
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 2], dx
        jmp    loc_2_02D4
loc_2_02CB:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 0xffff
loc_2_02D4:
        push   1
        lea    eax, [ebp - 0x30]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_23A2
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0300
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 4], dx
        jmp    loc_2_0309
loc_2_0300:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0xffff
loc_2_0309:
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_23A2
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0332
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 6], 0
        jmp    loc_2_033E
loc_2_0332:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 6], edx
loc_2_033E:
        push   0
        lea    eax, [ebp - 0x20]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_23A2
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0364
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xe], 0
        jmp    loc_2_0370
loc_2_0364:
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [edx + 0xe], cl
loc_2_0370:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        call   sub_1_22A4
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_03AA
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_03AA
        mov    eax, dword ptr [ebp - 4]
        push   6
        mov    byte ptr [eax + 0x2b], 1
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        add    eax, 0x32
        push   eax
        call   sub_1_2404
loc_2_03AA:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_23A8
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   esi
        push   ecx
        call   sub_2_0480
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_2_03D5
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2324
loc_2_03D5:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_03E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0xa
        mov    esi, dword ptr [ebp + 8]
        push   0xa
        push   esi
        mov    byte ptr [esi + 0x159], 1
        call   sub_1_16D0
        xor    edx, edx
        mov    al, 0xf
        mov    dx, word ptr [esi]
        push   0xe
        inc    edx
        push   0xe
        out    dx, al
        push   esi
        call   sub_1_16D0
        push   0x64
        call   sub_1_2318
        xor    edx, edx
        mov    al, 0xf
        mov    dx, word ptr [esi]
        lea    ecx, [esi + 0x1e2]
        inc    edx
        out    dx, al
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_2300
        lea    ecx, [esi + 0x1ca]
        push   ecx
        call   sub_1_230C
        mov    ecx, dword ptr [esi + 0x1f3]
        push   ecx
        call   sub_2_1150
        mov    ecx, dword ptr [esi + 0x1c6]
        push   ecx
        call   sub_1_2250
        mov    ecx, dword ptr [esi + 0x1ba]
        lea    edx, [esi + 0x1b6]
        push   0
        push   0
        mov    eax, dword ptr [edx]
        push   esi
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    edx, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
        call   sub_1_2324
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0480:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x27B4
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        push   0x26
        push   ecx
        call   sub_1_231E
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, 1
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dword ptr [eax + 0x12], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x16], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x1a], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x1e], 0x10
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 0x1c2]
        push   esi
        lea    ecx, [esi + 0x1c6]
        push   eax
        push   ecx
        call   sub_1_224A
        mov    edi, eax
        test   edi, edi
        je     loc_2_0540
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   eax
        call   sub_1_2324
        push   ebx
        mov    eax, dword ptr [esi + 0x1c6]
        push   2
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0540:
        push   esi
        call   sub_1_0040
        push   esi
        call   sub_1_00E0
        push   esi
        call   sub_2_0840
        test   al, al
        jne    loc_2_057D
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        call   sub_1_2324
        mov    eax, dword ptr [esi + 0x1c6]
        push   eax
        call   sub_1_2250
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_057D:
        lea    eax, [esi + 0x1f3]
        lea    ecx, [esi + 0x1be]
        push   eax
        lea    edx, [esi + 0x32]
        push   ecx
        mov    dword ptr [esi + 0x1ea], esi
        mov    dword ptr [esi + 0x1e2], 0
        mov    dword ptr [esi + 0x1e6], 0    ; -> loc_1_03D0
        push   edx
        push   0                             ; -> loc_1_10B0
        push   0                             ; -> loc_1_0F90
        push   0                             ; -> loc_1_1060
        push   0x10
        call   sub_2_1030
        push   0
        test   al, al
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jne    loc_2_05EA
        call   sub_1_2324
        mov    eax, dword ptr [esi + 0x1c6]
        push   eax
        call   sub_1_2250
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_05EA:
        call   sub_1_2324
        lea    ecx, [esi + 0x1b6]
        mov    dword ptr [ecx], 0            ; -> obj1:0x2980
        pop    edi
        mov    eax, dword ptr [4]            ; -> loc_1_2984
        mov    dword ptr [esi + 0x1ba], eax
        mov    eax, dword ptr [4]            ; -> loc_1_2984
        pop    esi
        pop    ebx
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    esp, ebp
        mov    dword ptr [4], ecx            ; -> loc_1_2984
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_0620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0x159], 0
        je     loc_2_063F
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_063F:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 0x18]
        cmp    eax, edx
        je     loc_2_065C
        mov    ecx, dword ptr [ebp + 0x14]
loc_2_064B:
        cmp    dword ptr [ecx], 0
        je     loc_2_0658
        add    ecx, 4
        inc    edx
        cmp    eax, edx
        ja     loc_2_064B
loc_2_0658:
        cmp    eax, edx
        jne    loc_2_0668
loc_2_065C:
        mov    eax, 0xc0010019
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0668:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], edx
        lea    edx, [ebp - 4]
        inc    dword ptr [esi + 0x13b]
        mov    ecx, dword ptr [0]            ; -> obj1:0x27B4
        push   ecx
        push   0
        push   0x47
        push   edx
        call   sub_1_231E
        test   eax, eax
        je     loc_2_0697
        mov    eax, 0xc000009a
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0697:
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
        mov    dword ptr [ecx + 0x43], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x41], ax
        mov    word ptr [esi + 0x1f7], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        push   eax
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x1f3]
        push   edx
        call   sub_2_11A0
        test   al, al
        jne    loc_2_0724
        mov    esi, 0xc0000001
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2324
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0724:
        lea    eax, [esi + 0x1a6]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x1aa]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [esi + 0x1aa]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1aa], eax
        mov    eax, dword ptr [esi + 0x192]
        inc    eax
        mov    dword ptr [esi + 0x192], eax
        cmp    eax, 1
        jne    loc_2_077D
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi]
        push   0xea60
        add    edx, 3
        add    esi, 0x1e2
        out    dx, al
        push   esi
        call   sub_1_22FA
loc_2_077D:
        xor    esi, esi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0790:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x18], al
        jne    loc_2_0807
        mov    byte ptr [esi + 0x18], 1
        push   eax
        dec    dword ptr [edi + 0x192]
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 0x1f3]
        push   eax
        push   ecx
        call   sub_2_1270
        cmp    eax, 0xc001000e
        jne    loc_2_07CF
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_07CF:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], edx
        dec    dword ptr [esi + 0x14]
        call   sub_1_0000
        push   0
        push   0
        push   esi
        call   sub_1_2324
        cmp    dword ptr [edi + 0x192], 0
        jne    loc_2_0805
        xor    edx, edx
        mov    al, 0xf
        mov    dx, word ptr [edi]
        add    edx, 3
        out    dx, al
loc_2_0805:
        xor    eax, eax
loc_2_0807:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0810:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x2978
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_2244
        push   0
        mov    ecx, dword ptr [0]            ; -> loc_1_297C
        push   ecx
        call   sub_1_2238
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0840:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    al, al
        push   ebx
        push   esi
        mov    bl, 0xff
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        push   esi
        out    dx, al
        call   sub_1_17B0
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x1c6]
        mov    ax, word ptr [esi]
        push   eax
        push   ecx
        push   esi
        call   sub_2_0AA0
        test   al, al
        jne    loc_2_0899
        push   0
        mov    eax, dword ptr [esi + 0x1c6]
        push   0                             ; -> sub_2_0840
        push   2
        push   0xaa000004
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0899:
        push   esi
        call   sub_2_0B40
        test   al, al
        jne    loc_2_08AE
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_08AE:
        cmp    byte ptr [esi + 0xd], 0
        jne    loc_2_093C
        mov    dx, word ptr [esi]
        mov    al, 0x80
        out    dx, al
        push   7
        push   esi
        call   sub_2_0CD0
        mov    word ptr [ebp - 2], ax
        mov    cx, ax
        xor    al, al
        mov    dx, word ptr [esi]
        out    dx, al
        xor    dx, dx
        mov    eax, dword ptr [esi + 0x1ef]
loc_2_08DC:
        test   cl, 1
        je     loc_2_08E3
        inc    bl
loc_2_08E3:
        mov    edi, edx
        and    edi, 0xffff
        cmp    edi, eax
        je     loc_2_090C
        cmp    dx, 2
        jne    loc_2_0900
        cmp    eax, 9
        jne    loc_2_0900
        test   byte ptr [ebp - 2], 4
        jne    loc_2_090C
loc_2_0900:
        shr    cx, 1
        inc    dx
        cmp    dx, 0x10
        jb     loc_2_08DC
loc_2_090C:
        test   bl, bl
        jl     loc_2_0915
        cmp    bl, 7
        jle    loc_2_093C
loc_2_0915:
        push   eax
        push   0                             ; -> sub_2_0840
        push   2
        mov    eax, dword ptr [esi + 0x1c6]
        push   0xaa000005
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_093C:
        xor    edx, edx
        mov    al, 0xf
        mov    dx, word ptr [esi]
        mov    byte ptr [esi + 0xc], bl
        add    edx, 3
        out    dx, al
        cmp    byte ptr [esi + 0xd], 0
        jne    loc_2_0986
        push   0xe
        push   0xe
        push   esi
        call   sub_1_16D0
        test   al, al
        jne    loc_2_0986
        push   0
        mov    eax, dword ptr [esi + 0x1c6]
        push   0                             ; -> sub_2_0840
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0986:
        mov    dx, word ptr [esi]
        mov    al, 0xb
        out    dx, al
        push   esi
        call   sub_2_0BD0
        push   esi
        call   sub_1_18C0
        cmp    ax, 0xffff
        jne    loc_2_09C6
        push   0
        mov    eax, dword ptr [esi + 0x1c6]
        push   0                             ; -> sub_2_0840
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_09C6:
        test   ax, ax
        je     loc_2_09DD
        mov    byte ptr [esi + 0x10], 1
        mov    word ptr [esi + 0x11], 0x2000
        mov    word ptr [esi + 0x13], 0x6000
        jmp    loc_2_09ED
loc_2_09DD:
        mov    byte ptr [esi + 0x10], 0
        mov    word ptr [esi + 0x11], 0x3000
        mov    word ptr [esi + 0x13], 0xd000
loc_2_09ED:
        lea    edi, [esi + 0x1ca]
        push   1
        push   0
        mov    eax, dword ptr [esi + 0x1ef]
        push   eax
        mov    ecx, dword ptr [esi + 0x1c6]
        push   eax
        lea    edx, [ebp - 8]
        push   0                             ; -> loc_1_0120
        push   esi
        push   0                             ; -> loc_1_0110
        push   ecx
        push   edi
        push   edx
        call   sub_1_2306
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0A64
        push   esi
        call   sub_1_13D0
        test   al, al
        jne    loc_2_0A59
        push   0
        mov    eax, dword ptr [esi + 0x1c6]
        push   0                             ; -> sub_2_0840
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        push   edi
        call   sub_1_230C
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A59:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A64:
        mov    eax, dword ptr [esi + 0x1ef]
        mov    ecx, dword ptr [esi + 0x1c6]
        push   eax
        push   0                             ; -> sub_2_0840
        push   2
        push   0xaa000005
        push   ecx
        call   sub_1_23F0
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0AA0:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    ebx, 0xec
        lea    edx, [esi + 2]
loc_2_0AB6:
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], bl
        cmp    byte ptr [ebp - 1], 0x24
        je     loc_2_0ACA
        inc    cl
        cmp    cl, 4
        jb     loc_2_0AB6
loc_2_0ACA:
        mov    bl, 0x40
        xor    cl, cl
loc_2_0ACE:
        in     al, dx
        mov    edi, ebx
        mov    byte ptr [ebp - 1], al
        and    edi, 0xff
        and    eax, 0xec
        sub    edi, eax
        cmp    edi, -0x24
        jne    loc_2_0B2F
        add    bl, 0x40
        inc    cl
        cmp    cl, 3
        jb     loc_2_0ACE
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [edi]
        mov    al, 0x80
        mov    edx, ecx
        out    dx, al
        mov    byte ptr [edi + 0xd], 0
        lea    edx, [esi + 0xa]
        in     al, dx
        mov    dl, al
        mov    byte ptr [ebp - 1], al
        shr    dl, 5
        mov    byte ptr [edi + 0xf], dl
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 0x10
        je     loc_2_0B1F
        mov    byte ptr [edi + 0xd], 1
loc_2_0B1F:
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0B2F:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0B40:
        push   ebp
        mov    al, 0x80
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        push   edi
        out    dx, al
        call   sub_2_0CD0
        and    ax, 0xfc3f
        shr    ax, 6
        mov    si, word ptr [edi]
        cmp    ax, si
        je     loc_2_0BC1
        shl    si, 6
        push   0
        push   edi
        call   sub_2_0CD0
        and    ax, 0x3ff
        or     ax, si
        push   eax
        push   0
        push   edi
        call   sub_2_0ED0
        push   edi
        call   sub_2_0C90
        mov    dx, word ptr [edi]
        xor    al, al
        out    dx, al
        push   0xe
        push   0xe
        push   edi
        call   sub_1_16D0
        test   al, al
        jne    loc_2_0BC1
        push   0
        mov    eax, dword ptr [edi + 0x1c6]
        push   0                             ; -> loc_1_1300
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_23F0
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_0BC1:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0BD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x2b], 0
        jne    loc_2_0C6C
        mov    al, byte ptr [esi + 0xd]
        test   al, al
        jne    loc_2_0C50
        mov    dx, word ptr [esi]
        mov    al, 0x80
        out    dx, al
        xor    di, di
loc_2_0BF5:
        mov    ax, di
        add    ax, 2
        push   eax
        push   esi
        call   sub_2_0CD0
        mov    ch, al
        mov    edx, edi
        inc    di
        mov    cl, ah
        lea    eax, [ebp - 4]
        and    edx, 0xffff
        add    edx, edx
        sub    eax, edx
        mov    word ptr [eax], cx
        cmp    di, 3
        jb     loc_2_0BF5
        mov    al, byte ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 6]
        mov    byte ptr [esi + 0x32], al
        mov    byte ptr [esi + 0x34], cl
        mov    ax, word ptr [ebp - 8]
        mov    cx, word ptr [ebp - 6]
        mov    byte ptr [esi + 0x33], ah
        mov    dx, word ptr [esi]
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 0x35], ch
        mov    byte ptr [esi + 0x36], al
        mov    ax, word ptr [ebp - 4]
        mov    byte ptr [esi + 0x37], ah
        xor    al, al
        out    dx, al
        jmp    loc_2_0C6C
loc_2_0C50:
        cmp    al, 1
        jne    loc_2_0C6C
        mov    byte ptr [esi + 0x33], 0xaa
        mov    byte ptr [esi + 0x35], 0x12
        mov    byte ptr [esi + 0x36], 0x34
        mov    byte ptr [esi + 0x37], 0x56
        xor    eax, eax
        mov    byte ptr [esi + 0x32], al
        mov    byte ptr [esi + 0x34], al
loc_2_0C6C:
        lea    eax, [esi + 0x32]
        push   6
        push   eax
        add    esi, 0x2c
        push   esi
        call   sub_1_2404
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C90:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    bx, bx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
loc_2_0C9E:
        push   ebx
        push   esi
        inc    bx
        call   sub_2_0CD0
        add    di, ax
        cmp    bx, 0x3f
        jb     loc_2_0C9E
        mov    ax, 0xbaba
        sub    ax, di
        push   eax
        push   0x3f
        push   esi
        call   sub_2_0ED0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0CD0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    edx, 0xa
        push   3
        in     al, dx
        push   6
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xfff2
        or     byte ptr [ebp - 2], 2
        mov    al, byte ptr [ebp - 2]
        push   edi
        out    dx, al
        call   sub_2_0D30
        mov    cx, word ptr [ebp + 0xc]
        push   6
        push   ecx
        push   edi
        call   sub_2_0D30
        push   edi
        call   sub_2_0E20
        mov    si, ax
        push   edi
        call   sub_2_0E90
        mov    ax, si
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0D30:
        push   ebp
        mov    cx, word ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        dec    cl
        xor    edx, edx
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ebx]
        push   edi
        mov    di, 1
        add    edx, 0xa
        in     al, dx
        mov    si, word ptr [ebp + 0xc]
        mov    byte ptr [ebp - 2], al
        shl    di, cl
        and    word ptr [ebp - 2], 0xfff3
loc_2_0D60:
        and    word ptr [ebp - 2], 0xfffb
        test   si, di
        je     loc_2_0D6F
        or     byte ptr [ebp - 2], 4
loc_2_0D6F:
        shr    di, 1
        xor    edx, edx
        mov    dx, word ptr [ebx]
        mov    al, byte ptr [ebp - 2]
        add    edx, 0xa
        push   0x64
        out    dx, al
        call   sub_1_2318
        lea    eax, [ebp - 2]
        push   eax
        push   ebx
        call   sub_2_0DC0
        lea    eax, [ebp - 2]
        push   eax
        push   ebx
        call   sub_2_0DF0
        test   di, di
        jne    loc_2_0D60
        xor    edx, edx
        pop    edi
        mov    dx, word ptr [ebx]
        pop    esi
        add    edx, 0xa
        pop    ebx
        and    word ptr [ebp - 2], 0xfffb
        mov    al, byte ptr [ebp - 2]
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0DC0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ax, word ptr [ecx]
        push   0x64
        or     al, 1
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx]
        add    edx, 0xa
        out    dx, al
        call   sub_1_2318
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DF0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ax, word ptr [ecx]
        push   0x64
        and    ax, 0xfffe
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx]
        add    edx, 0xa
        out    dx, al
        call   sub_1_2318
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0E20:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        xor    bx, bx
        add    edx, 0xa
        in     al, dx
        mov    di, 0x10
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xfff3
loc_2_0E45:
        add    bx, bx
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0DC0
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0xa
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xfffb
        test   byte ptr [ebp - 2], 8
        je     loc_2_0E6D
        or     bl, 1
loc_2_0E6D:
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0DF0
        dec    di
        jne    loc_2_0E45
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0E90:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    edx, 0xa
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xfff9
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0DC0
        lea    ecx, [ebp - 2]
        push   ecx
        push   esi
        call   sub_2_0DF0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0ED0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    edx, 0xa
        push   5
        in     al, dx
        push   0x13
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xfff2
        or     byte ptr [ebp - 2], 2
        mov    al, byte ptr [ebp - 2]
        push   esi
        out    dx, al
        call   sub_2_0D30
        mov    di, word ptr [ebp + 0xc]
        push   4
        push   edi
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0FF0
        push   3
        push   7
        push   esi
        call   sub_2_0D30
        push   6
        push   edi
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0F90
        test   ax, ax
        je     loc_2_0F80
        push   esi
        call   sub_2_0FF0
        push   3
        push   5
        push   esi
        call   sub_2_0D30
        push   6
        push   edi
        push   esi
        call   sub_2_0D30
        mov    ax, word ptr [ebp + 0x10]
        push   0x10
        push   eax
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0F90
        test   ax, ax
        je     loc_2_0F80
        push   esi
        call   sub_2_0FF0
        push   5
        push   0x10
        push   esi
        call   sub_2_0D30
        push   4
        push   edi
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0E90
loc_2_0F80:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    di, di
        push   esi
        mov    ebx, 8
        call   sub_2_0FF0
loc_2_0FAA:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0xa
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], bx
        jne    loc_2_0FD8
        inc    di
        push   0x64
        call   sub_1_2318
        cmp    di, 0xc8
        jb     loc_2_0FAA
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0FD8:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0FF0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        push   4
        add    edx, 0xa
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0xfffc
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        call   sub_1_2318
        or     byte ptr [ebp - 2], 2
        xor    edx, edx
        mov    al, byte ptr [ebp - 2]
        mov    dx, word ptr [esi]
        pop    esi
        add    edx, 0xa
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_1030:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj1:0x2954
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_231E
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_1064
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_1064:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_107E
        mov    esi, 2
loc_2_107E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_231E
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_10B2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1150
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_10B2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_231E
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_10E1
        push   eax
        call   sub_2_1150
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_10E1:
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
sub_2_1150:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_1170
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_2324
loc_2_1170:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_1186
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_2324
loc_2_1186:
        push   0
        push   0x32
        push   esi
        call   sub_1_2324
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_11A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_11B9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_11B9:
        mov    eax, dword ptr [0]            ; -> obj1:0x2954
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_231E
        or     eax, eax
        je     loc_2_11D9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_11D9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_11E0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_11F3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_11E0
        jmp    loc_2_1202
loc_2_11F3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_1202:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_1218
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1218:
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
sub_2_1270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_1610
        mov    edi, eax
        or     edi, edi
        je     loc_2_1299
        cmp    edi, 0x103
        jne    loc_2_12B0
loc_2_1299:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_1330
        or     eax, eax
        je     loc_2_12B0
        mov    edi, eax
loc_2_12B0:
        or     edi, edi
        je     loc_2_12BC
        cmp    edi, 0x103
        jne    loc_2_131B
loc_2_12BC:
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        jne    loc_2_1316
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_12D2
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_12D2:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_12E1
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_12E7
loc_2_12E1:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
loc_2_12E7:
        cmp    byte ptr [esi + 0x18], 0
        je     loc_2_12F6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2280
loc_2_12F6:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x19]
        shl    eax, cl
        or     dword ptr [ebx + 0x28], eax
        push   0
        push   0x1a
        push   esi
        call   sub_1_2324
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_1316:
        mov    edi, 0xc001000e
loc_2_131B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1330:
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
        call   sub_1_2424
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_2424
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_13B4
        sub    eax, eax
        nop    
loc_2_1390:
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
        ja     loc_2_1390
loc_2_13B4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_141F
loc_2_13C1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_1414
        lea    ecx, [ecx + ecx*2]
loc_2_13D0:
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
        call   sub_1_2424
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
        call   sub_1_2424
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_1417
loc_2_1414:
        inc    dword ptr [ebp - 4]
loc_2_1417:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_13C1
loc_2_141F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_150A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_1433:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_17F0
        or     al, al
        je     loc_2_146A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_14F8
loc_2_146A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_1544
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
        call   sub_1_2424
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
        call   sub_1_2424
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
loc_2_14F8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_1433
loc_2_150A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0xc], edx
        je     loc_2_1595
        mov    edi, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_1520:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_159B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_1582
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_1582
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_1589
loc_2_1544:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2424
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_2424
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_1582:
        mov    dword ptr [ebp - 8], 1
loc_2_1589:
        add    edi, 6
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0xc], edx
        ja     loc_2_1520
loc_2_1595:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_1600
loc_2_159B:
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
        je     loc_2_1602
        cmp    edi, 0x103
        je     loc_2_1602
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2424
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2424
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_1600:
        sub    edi, edi
loc_2_1602:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_1610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        mov    dword ptr [edi + 0xc], 0
        je     loc_2_164D
        lea    esp, [esp]
loc_2_1640:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1640
loc_2_164D:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    esi, ecx
        je     loc_2_1681
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_2_1683
        cmp    eax, 0x103
        je     loc_2_1683
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0xc], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1681:
        sub    eax, eax
loc_2_1683:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_1690:
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
        je     loc_2_16C2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_16B7:
        test   dword ptr [esi], edx
        je     loc_2_16BC
        inc    eax
loc_2_16BC:
        add    esi, 4
        dec    edi
        jne    loc_2_16B7
loc_2_16C2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_16D0:
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
        jbe    loc_2_174C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_1710:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_173A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_1766
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_2424
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_173A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_1710
loc_2_174C:
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
loc_2_1766:
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
sub_2_1790:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_17C0
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
loc_2_17B0:
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_17C0:
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
        call   sub_1_2424
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_17F0:
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
        je     loc_2_1889
        dec    esi
        cmp    esi, edx
        jb     loc_2_1889
        nop    
loc_2_1810:
        cmp    edx, ecx
        jb     loc_2_1889
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_1833
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1867
loc_2_1833:
        jbe    loc_2_1840
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_1867
        db  08Bh, 0FFh
loc_2_1840:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_1857
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1867
loc_2_1857:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_1867
        mov    dword ptr [ebp - 4], 0
loc_2_1867:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1899
        jle    loc_2_1878
        or     edx, edx
        je     loc_2_1889
        lea    esi, [edx - 1]
        jmp    loc_2_187B
loc_2_1878:
        lea    ecx, [edx + 1]
loc_2_187B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_1810
loc_2_1889:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1899:
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
