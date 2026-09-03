; PCNTN3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 7513 instructions recovered by recursive descent

Declare_Virtual_Device PCNTN3, 3, 10, PCNTN3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x53dc  R-X
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0xef]
        mov    eax, edx
        shl    edx, 7
        sub    edx, eax
        lea    ecx, [eax + edx*4]
        lea    edx, [ecx + ecx*2]
        sub    edx, eax
        mov    eax, dword ptr [esi + 0xca]
        mov    ecx, eax
        lea    ebx, [eax + eax*4]
        lea    eax, [ecx + ebx*4]
        lea    edi, [ecx + eax*8]
        lea    eax, [edi + edi*8]
        add    eax, ecx
        lea    ecx, [esi + 0xc2]
        push   ecx
        lea    ebx, [edx + eax + 0x38]
        lea    eax, [esi + 0xc6]
        mov    edx, dword ptr [esi + 0xae]
        push   eax
        push   0
        push   ebx
        push   edx
        call   sub_1_4BA2
        mov    edi, dword ptr [esi + 0xc6]
        test   edi, edi
        jne    loc_1_0068
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0068:
        xor    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    ecx, dword ptr [esi + 0xc6]
        mov    edx, dword ptr [esi + 0xc2]
        add    ecx, 0xf
        add    edx, 0xf
        and    ecx, 0xfffffff0
        and    edx, 0xfffffff0
        mov    edi, dword ptr [esi + 0xca]
        mov    dword ptr [esi + 0x105], ecx
        add    ecx, 0x20
        mov    dword ptr [esi + 0x109], edx
        add    edx, 0x20
        lea    eax, [edi*8]
        mov    dword ptr [esi + 0xd6], ecx
        mov    dword ptr [esi + 0xda], edx
        add    ecx, eax
        add    edx, eax
        mov    eax, dword ptr [esi + 0xef]
        mov    dword ptr [esi + 0xfb], ecx
        shl    eax, 3
        mov    dword ptr [esi + 0xff], edx
        add    ecx, eax
        add    edx, eax
        lea    eax, [edi + edi*4]
        mov    dword ptr [esi + 0xce], ecx
        mov    dword ptr [esi + 0xd2], edx
        lea    ebx, [edi + eax*4]
        lea    eax, [ebx + ebx*8]
        lea    ebx, [edi + eax*4]
        pop    edi
        lea    eax, [ebx*2]
        add    ecx, eax
        add    edx, eax
        mov    al, 1
        mov    dword ptr [esi + 0xf3], ecx
        mov    dword ptr [esi + 0xf7], edx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_0110:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [edx + 0xef]
        mov    eax, esi
        shl    esi, 7
        sub    esi, eax
        lea    ecx, [eax + esi*4]
        lea    esi, [ecx + ecx*2]
        sub    esi, eax
        mov    eax, dword ptr [edx + 0xca]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [edx + 0xc6]
        test   ecx, ecx
        lea    eax, [esi + eax + 0x1f]
        je     loc_1_0168
        mov    esi, dword ptr [edx + 0xc2]
        push   esi
        push   ecx
        push   0
        push   eax
        mov    eax, dword ptr [edx + 0xae]
        push   eax
        call   sub_1_4BA8
loc_1_0168:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0170:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x48]
        lea    ebx, [ecx + 0x12]
        add    ecx, 0x10
        mov    edx, ebx
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        test   ax, 4
        mov    di, ax
        je     loc_1_01D6
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        and    di, 0xffbf
        mov    edx, ecx
        out    dx, ax
        mov    ecx, 1
        mov    byte ptr [esi + 0x125], cl
        mov    ebx, dword ptr [esi + 0x48]
        mov    dword ptr [esi + 0x82], ecx
        lea    edx, [ebx + 0x12]
        out    dx, ax
        mov    word ptr [esi + 0x46], di
        lea    edx, [ebx + 0x10]
        mov    eax, edi
        pop    edi
        and    eax, 0xffff
        pop    esi
        out    dx, ax
        mov    al, cl
        pop    ebx
        pop    ebp
        ret    4
loc_1_01D6:
        test   di, 0x80
        je     loc_1_021B
        test   di, 0x40
        je     loc_1_021B
        and    di, 0xffbf
        mov    edx, ebx
        mov    byte ptr [esi + 0x125], 1
        xor    eax, eax
        out    dx, ax
        mov    edx, ecx
        mov    ecx, dword ptr [esi + 0x48]
        out    dx, ax
        mov    word ptr [esi + 0x46], di
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, edi
        and    eax, 0xffff
        pop    edi
        out    dx, ax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_021B:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 2], 0
        jne    loc_1_048E
        cmp    byte ptr [esi + 0xf], 0
        jne    loc_1_048E
        mov    di, word ptr [esi + 0x46]
        mov    byte ptr [esi + 0xf], 1
        mov    byte ptr [esi + 0x2d], 0
        cmp    dword ptr [esi + 0x82], 0
        je     loc_1_02FD
        mov    eax, dword ptr [esi + 0x86]
        inc    eax
        mov    dword ptr [esi + 0x86], eax
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_02F6
        cmp    byte ptr [esi + 0x10], 0
        jne    loc_1_02F6
        mov    byte ptr [esi + 0x126], 1
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 2
        out    dx, ax
        lea    eax, [ebp - 1]
        lea    edx, [esi + 0x15]
        push   eax
        push   edx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_02C2
        mov    byte ptr [esi + 0x11], 0
loc_1_02C2:
        mov    byte ptr [esi + 0x126], 0
        xor    eax, eax
        dec    dword ptr [esi + 0x86]
        mov    byte ptr [esi + 0xf], 0
        mov    byte ptr [esi + 0x125], 0
        mov    ecx, dword ptr [esi + 0x48]
        pop    edi
        pop    esi
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 0x40
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    0x10
loc_1_02F6:
        dec    eax
        mov    dword ptr [esi + 0x86], eax
loc_1_02FD:
        test   di, 0x400
        je     loc_1_032B
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_032B
        push   esi
        inc    dword ptr [esi + 0x86]
        call   sub_1_05B0
        mov    eax, dword ptr [esi + 0xb2]
        push   eax
        call   sub_1_49A0
        dec    dword ptr [esi + 0x86]
loc_1_032B:
        test   di, 0x8000
        je     loc_1_03AB
        inc    dword ptr [esi + 0x86]
        test   di, 0x2000
        je     loc_1_0345
        inc    dword ptr [esi + 0x1aa]
loc_1_0345:
        test   di, 0x1000
        je     loc_1_03A5
        inc    dword ptr [esi + 0x13a]
        cmp    byte ptr [esi], 1
        jne    loc_1_03A5
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_03A5
        mov    byte ptr [esi + 0x126], 1
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 2
        out    dx, ax
        lea    eax, [ebp - 1]
        lea    edx, [esi + 0x15]
        push   eax
        push   edx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_039E
        mov    byte ptr [esi + 0x11], 0
loc_1_039E:
        mov    byte ptr [esi + 0x126], 0
loc_1_03A5:
        dec    dword ptr [esi + 0x86]
loc_1_03AB:
        xor    eax, eax
        inc    dword ptr [esi + 0x86]
        mov    ecx, dword ptr [esi + 0x48]
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        in     ax, dx
        and    al, 0x33
        cmp    al, 0x33
        je     loc_1_041A
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_041A
        cmp    byte ptr [esi + 0x10], 0
        jne    loc_1_041A
        mov    byte ptr [esi + 0x126], 1
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 2
        out    dx, ax
        lea    eax, [ebp - 1]
        lea    edx, [esi + 0x15]
        push   eax
        push   edx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0413
        mov    byte ptr [esi + 0x11], 0
loc_1_0413:
        mov    byte ptr [esi + 0x126], 0
loc_1_041A:
        dec    dword ptr [esi + 0x86]
        mov    byte ptr [esi + 0xf], 0
        mov    byte ptr [esi + 0x125], 0
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_047A
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_0464
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_0464
        cmp    byte ptr [esi + 0x126], 0
        je     loc_1_0455
        cmp    dword ptr [esi + 0x86], 1
        je     loc_1_0464
loc_1_0455:
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_047A
loc_1_0464:
        mov    byte ptr [esi + 0x11], 1
        push   0
        inc    dword ptr [esi + 0x86]
        push   0
        push   esi
        push   0
        call   sub_1_04A0
loc_1_047A:
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 0x40
        out    dx, ax
loc_1_048E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x126], 0
        je     loc_1_04D2
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_04D2
        mov    byte ptr [esi + 0x11], 0
        push   esi
        dec    dword ptr [esi + 0x86]
        call   sub_1_3850
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_04D2:
        xor    ebx, ebx
loc_1_04D4:
        cmp    dword ptr [esi + 0x119], ebx
        je     loc_1_04F3
        cmp    byte ptr [esi + 0x126], bl
        jne    loc_1_04F3
        cmp    byte ptr [esi + 0x13], bl
        jne    loc_1_04F3
        push   esi
        call   sub_1_1C00
        test   al, al
        jne    loc_1_04D4
loc_1_04F3:
        xor    ebx, ebx
loc_1_04F5:
        cmp    dword ptr [esi + 0x10d], ebx
        je     loc_1_050C
        cmp    byte ptr [esi + 0x14], bl
        jne    loc_1_050C
        push   esi
        call   sub_1_45B0
        test   al, al
        jne    loc_1_04F5
loc_1_050C:
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_055F
        xor    edi, edi
loc_1_051D:
        mov    ecx, dword ptr [esi + 0x96]
        cmp    dword ptr [ecx + 0x14], edi
        jne    loc_1_0550
        mov    ebx, dword ptr [ecx + 0x10]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        push   edi
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        push   0x45
        push   ecx
        mov    dword ptr [edx + 4], eax
        call   sub_1_4C1C
        push   edi
        dec    dword ptr [esi + 0x92]
        push   ebx
        call   sub_1_4B54
loc_1_0550:
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        jne    loc_1_051D
loc_1_055F:
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_0591
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_0591
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        jne    loc_1_0591
        pop    edi
        dec    dword ptr [esi + 0x86]
        mov    byte ptr [esi + 0x11], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_0591:
        push   0xa
        add    esi, 0x15
        push   esi
        call   sub_1_4BF2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0x103]
        mov    eax, ecx
        and    eax, 0xffff
        shl    eax, 3
        add    eax, dword ptr [esi + 0xfb]
        mov    bx, word ptr [eax + 2]
        mov    dword ptr [ebp - 8], eax
        and    bx, 0xff00
        test   bh, 0x80
        jne    loc_1_07CB
        mov    edi, dword ptr [ebp - 4]
loc_1_05EB:
        mov    ax, word ptr [esi + 0x103]
        inc    ax
        mov    word ptr [esi + 0x103], ax
        mov    edx, eax
        and    edx, 0xffff
        cmp    edx, dword ptr [esi + 0xef]
        jb     loc_1_0614
        mov    word ptr [esi + 0x103], 0
loc_1_0614:
        mov    ax, bx
        and    ax, 0x4000
        mov    word ptr [ebp - 0xc], ax
        jne    loc_1_06EC
        test   bh, 1
        je     loc_1_06EC
        test   bh, 2
        je     loc_1_06EC
        mov    edx, dword ptr [ebp - 8]
        xor    edi, edi
        mov    di, word ptr [edx + 6]
        sub    edi, 4
        cmp    edi, 0x5ea
        ja     loc_1_06EC
        mov    edx, edi
        jb     loc_1_0658
        mov    edx, 0x5ea
loc_1_0658:
        and    ecx, 0xffff
        mov    eax, ecx
        shl    ecx, 6
        sub    ecx, eax
        lea    eax, [eax + ecx*4]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 0xf3]
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_07CB
        mov    al, byte ptr [ecx]
        test   al, 1
        je     loc_1_06A6
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_069E
        cmp    al, 0xff
        jne    loc_1_069E
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_069E
        mov    word ptr [ebp - 4], 4
        jmp    loc_1_06AC
loc_1_069E:
        mov    word ptr [ebp - 4], 2
        jmp    loc_1_06AC
loc_1_06A6:
        mov    word ptr [ebp - 4], 1
loc_1_06AC:
        cmp    edi, 0xe
        jae    loc_1_06CC
        cmp    edi, 6
        jb     loc_1_06EC
        lea    eax, [ecx + 0xe]
        push   0
        push   0
        push   0
        push   edi
        push   ecx
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 0xb2]
        push   ecx
        jmp    loc_1_06E7
loc_1_06CC:
        lea    eax, [edi - 0xe]
        sub    edx, 0xe
        push   eax
        push   edx
        lea    eax, [ecx + 0xe]
        mov    edx, dword ptr [esi + 0xb2]
        push   eax
        push   0xe
        push   ecx
        push   ecx
        add    ecx, 0xe
        push   ecx
        push   edx
loc_1_06E7:
        call   sub_1_47C0
loc_1_06EC:
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + 4], 0xfa12
        mov    ax, word ptr [edx + 2]
        and    ax, 0xff
        mov    word ptr [edx + 2], ax
        or     ah, 0x80
        cmp    word ptr [ebp - 0xc], 0
        mov    word ptr [edx + 2], ax
        jne    loc_1_0774
        inc    dword ptr [esi + 0x12e]
        cmp    word ptr [ebp - 4], 4
        jne    loc_1_0737
        lea    ecx, [esi + 0x166]
        inc    dword ptr [esi + 0x182]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    eax, edi
        jae    loc_1_079F
        inc    dword ptr [ecx + 4]
        jmp    loc_1_079F
loc_1_0737:
        cmp    word ptr [ebp - 4], 2
        jne    loc_1_0759
        lea    ecx, [esi + 0x15e]
        inc    dword ptr [esi + 0x17e]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    eax, edi
        jae    loc_1_079F
        inc    dword ptr [ecx + 4]
        jmp    loc_1_079F
loc_1_0759:
        lea    ecx, [esi + 0x156]
        inc    dword ptr [esi + 0x17a]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    eax, edi
        jae    loc_1_079F
        inc    dword ptr [ecx + 4]
        jmp    loc_1_079F
loc_1_0774:
        inc    dword ptr [esi + 0x136]
        test   bh, 8
        je     loc_1_0787
        inc    dword ptr [esi + 0x186]
        jmp    loc_1_079F
loc_1_0787:
        test   bh, 0x20
        je     loc_1_0794
        inc    dword ptr [esi + 0x18e]
        jmp    loc_1_079F
loc_1_0794:
        test   bh, 4
        je     loc_1_079F
        inc    dword ptr [esi + 0x1a2]
loc_1_079F:
        mov    cx, word ptr [esi + 0x103]
        mov    eax, ecx
        and    eax, 0xffff
        shl    eax, 3
        add    eax, dword ptr [esi + 0xfb]
        mov    bx, word ptr [eax + 2]
        mov    dword ptr [ebp - 8], eax
        and    bx, 0xff00
        test   bh, 0x80
        je     loc_1_05EB
loc_1_07CB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07E0:
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
        je     loc_1_08A9
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_08A9
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_08A4
        mov    edi, dword ptr [ebp + 0xc]
loc_1_082F:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_085B
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4C88
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_08A4
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        jmp    loc_1_089F
loc_1_085B:
        test   edi, edi
        je     loc_1_0878
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_1_0870
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_089F
loc_1_0870:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_0878:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_1_0886
        mov    ebx, eax
loc_1_0886:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        add    esi, ebx
        call   sub_1_4CFC
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_089F:
        cmp    dword ptr [ebp + 0x10], esi
        ja     loc_1_082F
loc_1_08A4:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_08A9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08C0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_098C
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   esi
        push   edx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 0x14], esi
        je     loc_1_098C
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], esi
        mov    dword ptr [ebp - 0x10], ecx
        je     loc_1_0987
        mov    edi, dword ptr [ebp + 0x14]
loc_1_0915:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0941
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4C88
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0987
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        jmp    loc_1_0982
loc_1_0941:
        test   edi, edi
        je     loc_1_095E
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_0956
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0982
loc_1_0956:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_095E:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_1_096C
        mov    ebx, eax
loc_1_096C:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        add    esi, ebx
        call   sub_1_4CFC
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_0982:
        cmp    esi, dword ptr [ebp + 0xc]
        jb     loc_1_0915
loc_1_0987:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_098C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_09A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 0x86]
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_09F8
        cmp    byte ptr [eax + 0x40], 0
        jne    loc_1_09F1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x10]
        test   edx, edx
        jl     loc_1_09D0
        cmp    edx, 1
        jle    loc_1_09D7
loc_1_09D0:
        mov    edi, 0xc00000bb
        jmp    loc_1_09FD
loc_1_09D7:
        push   ecx
        mov    edi, 0x103
        push   esi
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0
        call   sub_1_0A60
        jmp    loc_1_09FD
loc_1_09F1:
        mov    edi, 0xc0010002
        jmp    loc_1_09FD
loc_1_09F8:
        mov    edi, 0xc001000d
loc_1_09FD:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_0A4D
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_0A15
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_0A36
loc_1_0A15:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_0A36
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_0A36
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_0A4D
loc_1_0A36:
        push   0xa
        add    esi, 0x15
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_0A4D:
        mov    eax, edi
        dec    dword ptr [esi + 0x86]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_0A60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0xb6], 0
        je     loc_1_0A85
        mov    edx, dword ptr [eax + 0xba]
        mov    ecx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 0xba], ecx
        ret    8
loc_1_0A85:
        cmp    byte ptr [eax + 0x129], 0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xb6], ecx
        mov    dword ptr [eax + 0xba], ecx
        jne    loc_1_0AA3
        push   eax
        call   sub_1_0AB0
loc_1_0AA3:
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_0ABC:
        mov    edi, dword ptr [esi + 0xb6]
        test   edi, edi
        je     loc_1_0CA7
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_0CB7
        mov    byte ptr [esi + 0x129], 1
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 4
        ja     loc_1_0C4C
        jmp    dword ptr [eax*4]             ; -> obj1:0x0CC8
loc_1_0AF1:
        push   0
        lea    eax, [edi + 0x24]
        push   eax
        lea    ecx, [edi + 0x20]
        push   ecx
        mov    edx, dword ptr [edi + 0x1c]
        push   edx
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    ecx, dword ptr [edi + 0x14]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_1_0D90
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0C4C
loc_1_0B19:
        lea    eax, [edi + 0x24]
        lea    ecx, [edi + 0x20]
        push   eax
        mov    edx, dword ptr [edi + 0x1c]
        push   ecx
        mov    eax, dword ptr [edi + 0x18]
        push   edx
        mov    ecx, dword ptr [edi + 0x14]
        push   eax
        mov    edx, dword ptr [edi + 4]
        push   ecx
        push   edx
        push   esi
        call   sub_1_11A0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0C4C
loc_1_0B3F:
        push   1
        lea    eax, [edi + 0x24]
        push   eax
        lea    ecx, [edi + 0x20]
        push   ecx
        mov    edx, dword ptr [edi + 0x1c]
        push   edx
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    ecx, dword ptr [edi + 0x14]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_1_0D90
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0C4C
loc_1_0B67:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0xc]
        mov    ecx, dword ptr [ebx + 0x10]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        push   ecx
        push   ebx
        mov    eax, dword ptr [edx + 0xb2]
        push   eax
        call   sub_2_0170
loc_1_0B82:
        test   al, al
        jne    loc_1_0BA2
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   0xc0000001
        push   eax
        call   sub_1_4B4E
        push   0
        push   0x45
        push   ebx
        call   sub_1_4C1C
        jmp    loc_1_0BD7
loc_1_0BA2:
        lea    eax, [esi + 0x8a]
        push   0
        push   0
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esi + 0x8e]
        mov    dword ptr [ebx + 4], ecx
        mov    ecx, dword ptr [esi + 0x8e]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 0x8e], ebx
        inc    dword ptr [esi + 0x92]
        inc    dword ptr [ebx + 0x14]
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_4B4E
loc_1_0BD7:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0C4C
loc_1_0BE0:
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    ecx, dword ptr [esi + 0xb2]
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        push   ecx
        call   sub_2_0240
loc_1_0BF5:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_1_0C01
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_0C17
loc_1_0C01:
        mov    eax, 0x103
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_0C17
        cmp    dword ptr [ebp - 4], 0xc001000e
        jne    loc_1_0C17
        mov    dword ptr [ebp - 4], eax
loc_1_0C17:
        mov    byte ptr [ebx + 0x40], 1
        dec    dword ptr [ebx + 0x14]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 0x96]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 0x9a]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 0x9a]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 0x9a], ebx
loc_1_0C4C:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_0CBE
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0xb6], eax
        mov    eax, dword ptr [edi + 0x10]
        test   eax, eax
        je     loc_1_0C8E
        cmp    eax, 1
        je     loc_1_0C8E
        cmp    eax, 2
        jne    loc_1_0ABC
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0xae]
        push   eax
        push   edi
        push   ecx
        call   sub_1_4B8A
        dec    dword ptr [esi + 0x86]
        jmp    loc_1_0ABC
loc_1_0C8E:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_4B6C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_0ABC
loc_1_0CA7:
        mov    byte ptr [esi + 0x129], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0CB7:
        mov    byte ptr [esi + 0x129], 0
loc_1_0CBE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_0AF1
        dd  loc_1_0B19
        dd  loc_1_0B3F
        dd  loc_1_0B67
        dd  loc_1_0BE0
        db  08Dh, 064h, 024h, 000h
loc_1_0CE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x86]
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_0D28
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 2
        je     loc_1_0D07
        mov    edi, 0xc00000bb
        jmp    loc_1_0D2D
loc_1_0D07:
        push   eax
        mov    edi, 0x103
        mov    dword ptr [eax + 4], 0
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [esi + 0x86]
        call   sub_1_0A60
        jmp    loc_1_0D2D
loc_1_0D28:
        mov    edi, 0xc001000d
loc_1_0D2D:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_0D7D
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_0D45
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_0D66
loc_1_0D45:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_0D66
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_0D66
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_0D7D
loc_1_0D66:
        push   0xa
        add    esi, 0x15
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_0D7D:
        mov    eax, edi
        dec    dword ptr [esi + 0x86]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_0D90:
        push   ebp
        mov    cl, byte ptr [esp + 0x24]
        mov    ebp, esp
        sub    esp, 0x18
        test   cl, cl
        push   ebx
        push   esi
        push   edi
        je     loc_1_0DAF
        mov    dword ptr [ebp - 0x10], 0     ; -> loc_1_4DB0
        mov    edx, 0x34
        jmp    loc_1_0DBB
loc_1_0DAF:
        mov    dword ptr [ebp - 0x10], 0     ; -> obj1:0x4E80
        mov    edx, 0x18
loc_1_0DBB:
        xor    eax, eax
        test   edx, edx
        jle    loc_1_0DD5
        mov    esi, dword ptr [ebp - 0x10]
        mov    ebx, dword ptr [ebp + 0x10]
loc_1_0DC7:
        cmp    dword ptr [esi], ebx
        je     loc_1_0DD8
        add    esi, 4
        inc    eax
        cmp    edx, eax
        jg     loc_1_0DC7
        jmp    loc_1_0DD8
loc_1_0DD5:
        mov    ebx, dword ptr [ebp + 0x10]
loc_1_0DD8:
        cmp    edx, eax
        jne    loc_1_0DF3
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0xc00000bb
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_0DF3:
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    eax, ebx
        and    eax, 0xffff0000
        cmp    eax, 0x20000
        ja     loc_1_0E1B
        je     loc_1_0F6D
        cmp    eax, 0x10000
        je     loc_1_0E36
        jmp    loc_1_1103
loc_1_0E1B:
        cmp    eax, 0x1010000
        je     loc_1_0FEF
        cmp    eax, 0x1020000
        je     loc_1_10C3
        jmp    loc_1_1103
loc_1_0E36:
        sub    ebx, 0x10101
        cmp    ebx, 0x12
        ja     loc_1_1103
        jmp    dword ptr [ebx*4]             ; -> obj1:0x1138
loc_1_0E4C:
        mov    esi, dword ptr [ebp - 0x10]
        lea    edi, [edx*4]
        jmp    loc_1_1103
loc_1_0E5B:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_1103
loc_1_0E67:
        xor    eax, eax
        test   cl, cl
        mov    dword ptr [ebp - 8], eax
        je     loc_1_1103
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x92], eax
        jne    loc_1_1103
        xor    edi, edi
        jmp    loc_1_1103
loc_1_0E8A:
        mov    dword ptr [ebp - 8], 0xd0
        jmp    loc_1_1103
loc_1_0E96:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_1103
loc_1_0EA2:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_1103
loc_1_0EAE:
        mov    dword ptr [ebp - 8], 0x5ea0
        jmp    loc_1_1103
loc_1_0EBA:
        mov    dword ptr [ebp - 8], 0xbdc0
        jmp    loc_1_1103
loc_1_0EC6:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_1103
loc_1_0ED2:
        mov    dword ptr [ebp - 8], 0x5ee
        jmp    loc_1_1103
loc_1_0EDE:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 3
        lea    eax, [ebp - 0x18]
        push   ebx
        lea    esi, [ebp - 0x18]
        push   eax
        mov    edi, 4
        call   sub_1_4CFC
        mov    byte ptr [ebp - 0x15], 0
        jmp    loc_1_1103
loc_1_0F01:
        mov    esi, 0                        ; -> obj4:0x0000
        mov    edi, 0x1d
        jmp    loc_1_1103
loc_1_0F10:
        test   cl, cl
        je     loc_1_0F25
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xbe]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1103
loc_1_0F25:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_1103
loc_1_0F36:
        mov    dword ptr [ebp - 8], 0xd0
        jmp    loc_1_1103
loc_1_0F42:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_1103
loc_1_0F55:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_1103
loc_1_0F61:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_1103
loc_1_0F6D:
        test   cl, cl
        je     loc_1_1103
        mov    ecx, ebx
        and    ebx, 0xff00
        and    ecx, 0xff
        dec    ecx
        cmp    ebx, 0x100
        je     loc_1_0F99
        cmp    ebx, 0x200
        je     loc_1_0FAB
        jmp    loc_1_1103
loc_1_0F99:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + ecx*4 + 0x12a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1103
loc_1_0FAB:
        mov    eax, ecx
        and    eax, 0xfffffffe
        cmp    eax, 0xc
        jae    loc_1_0FDD
        test   cl, 1
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_0FCC
        mov    eax, dword ptr [ebx + eax*2 + 0x16e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1103
loc_1_0FCC:
        lea    esi, [ebx + eax*4 + 0x13e]
        mov    edi, 8
        jmp    loc_1_1103
loc_1_0FDD:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + ecx*4 + 0x156]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1103
loc_1_0FEF:
        sub    ebx, 0x1010101
        cmp    ebx, 3
        ja     loc_1_1103
        jmp    dword ptr [ebx*4]             ; -> obj1:0x1184
loc_1_1005:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 6
        add    esi, 3
        jmp    loc_1_1103
loc_1_1015:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 6
        add    esi, 9
        jmp    loc_1_1103
loc_1_1025:
        test   cl, cl
        je     loc_1_105D
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x18]
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 0x14]
        mov    eax, dword ptr [ebx + 0xb2]
        push   eax
        push   ecx
        call   sub_2_0760
loc_1_1048:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 0xc]
        lea    eax, [ecx + ecx*2]
        lea    edi, [eax*2]
        jmp    loc_1_1103
loc_1_105D:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0xb2]
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        lea    eax, [ebp - 0x14]
        push   edx
        push   eax
        call   sub_2_06A0
loc_1_1083:
        cmp    dword ptr [ebp - 0x14], 0
        mov    esi, dword ptr [ebp + 0x14]
        jne    loc_1_109B
        mov    eax, dword ptr [ebp - 0xc]
        lea    ecx, [eax + eax*2]
        lea    edi, [ecx*2]
        jmp    loc_1_1103
loc_1_109B:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0xb2]
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        push   edx
        call   sub_2_0660
loc_1_10AE:
        lea    ecx, [eax + eax*2]
        lea    edi, [ecx*2]
        jmp    loc_1_1103
loc_1_10BA:
        mov    dword ptr [ebp - 8], 0x10
        jmp    loc_1_1103
loc_1_10C3:
        test   cl, cl
        je     loc_1_1103
        mov    eax, ebx
        and    ebx, 0xff00
        and    eax, 0xff
        dec    eax
        cmp    ebx, 0x100
        je     loc_1_10E7
        cmp    ebx, 0x200
        je     loc_1_10F6
        jmp    loc_1_1103
loc_1_10E7:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 0x18e]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1103
loc_1_10F6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 0x19a]
        mov    dword ptr [ebp - 8], eax
loc_1_1103:
        cmp    dword ptr [ebp + 0x18], edi
        jae    loc_1_111B
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edi
        mov    eax, 0xc0010016
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_111B:
        push   edi
        mov    eax, dword ptr [ebp + 0x14]
        push   esi
        push   eax
        call   sub_1_4CFC
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
        ; ---- dispatch table (23 entries) ----
        dd  loc_1_0E4C
        dd  loc_1_0E5B
        dd  loc_1_0E67
        dd  loc_1_0E67
        dd  loc_1_0E8A
        dd  loc_1_0E96
        dd  loc_1_0EA2
        dd  loc_1_0EAE
        dd  loc_1_0EBA
        dd  loc_1_0EC6
        dd  loc_1_0ED2
        dd  loc_1_0EDE
        dd  loc_1_0F01
        dd  loc_1_0F10
        dd  loc_1_0F36
        dd  loc_1_0F42
        dd  loc_1_0F55
        dd  loc_1_1103
        dd  loc_1_0F61
        dd  loc_1_1005
        dd  loc_1_1015
        dd  loc_1_1025
        dd  loc_1_10BA
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11A0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 0x1010103
        push   esi
        ja     loc_1_11D5
        je     loc_1_12A6
        cmp    eax, 0x1010e
        je     loc_1_11E1
        cmp    eax, 0x1010f
        je     loc_1_1255
        cmp    eax, 0x10112
        je     loc_1_1267
loc_1_11D5:
        mov    eax, 0xc0010017
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11E1:
        mov    esi, dword ptr [ebp + 0x18]
        cmp    esi, 4
        je     loc_1_11F5
        mov    eax, 0xc0010015
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_11F5:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4CFC
        test   dword ptr [ebp - 4], 0xf050
        je     loc_1_122B
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x20]
        pop    esi
        mov    dword ptr [ecx], 4
        mov    dword ptr [eax], 0
        mov    eax, 0xc00000bb
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_122B:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [eax + 0xb2]
        push   edx
        push   ecx
        call   sub_2_05E0
loc_1_1249:
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1255:
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1267:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_1282
        mov    eax, dword ptr [ebp + 0x20]
        pop    esi
        mov    dword ptr [eax], 4
        mov    eax, 0xc0010014
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1282:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x41
        push   eax
        call   sub_1_4CFC
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_12A6:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, esi
        cdq    
        idiv   ecx
        test   edx, edx
        je     loc_1_12C3
        mov    eax, 0xc0010015
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_12C3:
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
        mov    edx, dword ptr [ecx + 0xb2]
        push   eax
        push   edx
        call   sub_2_0300
loc_1_12EC:
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1300:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [ecx + 0x126], 0
        jne    loc_1_1331
        cmp    byte ptr [ecx + 0xf], 0
        jne    loc_1_1331
        cmp    byte ptr [ecx + 0x10], 0
        jne    loc_1_1331
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   edx
        push   ecx
        call   sub_1_1340
        pop    ebp
        ret    0x14
loc_1_1331:
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1340:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [esi + 0x105]
        test   edi, 0x20
        je     loc_1_1375
        mov    word ptr [eax], 0x8000
        cmp    dword ptr [esi + 0x52], 0
        je     loc_1_13E7
        mov    eax, dword ptr [esi + 0x105]
        or     word ptr [eax], 0x1080
        jmp    loc_1_13E7
loc_1_1375:
        mov    word ptr [eax], 0
        cmp    dword ptr [esi + 0x52], 0
        je     loc_1_138B
        mov    eax, dword ptr [esi + 0x105]
        or     word ptr [eax], 0x1080
loc_1_138B:
        test   edi, 4
        je     loc_1_13B7
        xor    ax, ax
        mov    ecx, 0xff
loc_1_139B:
        mov    edx, eax
        mov    ebx, dword ptr [esi + 0x105]
        inc    ax
        and    edx, 0xffff
        cmp    ax, 8
        mov    byte ptr [edx + ebx + 8], cl
        jb     loc_1_139B
        jmp    loc_1_13E7
loc_1_13B7:
        test   edi, 2
        je     loc_1_13E7
        lea    eax, [ebp - 0x68]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi + 0xb2]
        push   ecx
        lea    eax, [ebp - 8]
        push   0x60
        push   edx
        push   eax
        call   sub_2_0760
loc_1_13D9:
        lea    ecx, [ebp - 0x68]
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        push   esi
        call   sub_1_1480
loc_1_13E7:
        mov    byte ptr [esi + 0x126], 1
        push   esi
        call   sub_1_3110
        xor    eax, eax
        mov    dword ptr [esi + 0xbe], edi
        mov    byte ptr [esi + 0x126], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_1410:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_1468
        cmp    byte ptr [esi + 0xf], 0
        jne    loc_1_1468
        cmp    byte ptr [esi + 0x10], 0
        jne    loc_1_1468
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x10
        jbe    loc_1_1442
        mov    eax, 0xc0010009
        pop    esi
        pop    ebp
        ret    0x1c
loc_1_1442:
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        push   eax
        push   esi
        call   sub_1_1480
        mov    byte ptr [esi + 0x126], 1
        push   esi
        call   sub_1_3110
        mov    byte ptr [esi + 0x126], 0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x1c
loc_1_1468:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x1c
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1480:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        xor    eax, eax
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_148D:
        mov    ecx, dword ptr [ebp + 8]
        inc    eax
        cmp    eax, 8
        mov    ebx, dword ptr [ecx + 0x105]
        mov    byte ptr [ebx + eax + 7], dl
        jb     loc_1_148D
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_1_14F0
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
loc_1_14AD:
        xor    ebx, ebx
        push   esi
        push   6
        call   sub_1_1500
        xor    ecx, ecx
loc_1_14B9:
        mov    dl, al
        shr    dl, cl
        and    dl, 1
        inc    ecx
        cmp    ecx, 6
        lea    ebx, [edx + ebx*2]
        jb     loc_1_14B9
        mov    al, bl
        xor    edx, edx
        shr    al, 3
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, al
        mov    edi, dword ptr [ecx + 0x105]
        mov    al, 1
        mov    cl, bl
        and    cl, 7
        add    esi, 6
        shl    al, cl
        or     byte ptr [edx + edi + 8], al
        dec    dword ptr [ebp - 4]
        jne    loc_1_14AD
loc_1_14F0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1500:
        push   ebp
        mov    eax, 0xffffffff
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_153F
        mov    esi, dword ptr [ebp + 0xc]
loc_1_1514:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    ecx, 8
loc_1_151C:
        mov    edi, eax
        mov    bl, dl
        shr    edi, 0x1f
        add    eax, eax
        and    ebx, 1
        cmp    ebx, edi
        je     loc_1_1534
        xor    eax, 0x4c11db6
        or     eax, 1
loc_1_1534:
        shr    dl, 1
        dec    ecx
        jne    loc_1_151C
        dec    dword ptr [ebp + 8]
        jne    loc_1_1514
loc_1_153F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1550:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x14]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x86]
        mov    ecx, dword ptr [esi + 0x48]
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        in     ax, dx
        and    al, 0x33
        cmp    al, 0x33
        je     loc_1_15DD
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_177C
        cmp    byte ptr [esi + 0xf], 0
        jne    loc_1_15DD
        cmp    byte ptr [esi + 0x10], 0
        jne    loc_1_15DD
        mov    byte ptr [esi + 0x126], 1
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        push   esi
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        out    dx, ax
        call   sub_1_3110
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x15]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_15D6
        mov    byte ptr [esi + 0x11], 0
loc_1_15D6:
        mov    byte ptr [esi + 0x126], 0
loc_1_15DD:
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_177C
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_1775
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        inc    dword ptr [edi + 0x14]
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1763
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_1763
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 0x2c], cx
        add    ebx, 0x24
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 4], edi
        push   eax
        push   edi
        push   edi
        call   sub_1_17F0
        cmp    byte ptr [ebx + 0xa], 8
        jne    loc_1_16E9
        mov    byte ptr [esi + 0x14], 1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_1_46D0
        lea    edx, [esi + 0x13e]
        xor    ecx, ecx
        mov    byte ptr [esi + 0x14], 0
        xor    eax, eax
        inc    dword ptr [esi + 0x12a]
        inc    dword ptr [esi + 0x16e]
        mov    cx, word ptr [ebx + 8]
        add    ecx, dword ptr [edx]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [ebx + 8]
        cmp    eax, ecx
        jbe    loc_1_1681
        inc    dword ptr [edx + 4]
loc_1_1681:
        dec    dword ptr [edi + 0x14]
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_16D5
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_169C
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_16BD
loc_1_169C:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_16BD
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_16BD
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_16D5
loc_1_16BD:
        mov    byte ptr [esi + 0x11], 1
        push   esi
        call   sub_1_1D70
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16D5:
        mov    eax, 0x103
        pop    edi
        dec    dword ptr [esi + 0x86]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16E9:
        mov    byte ptr [esi + 0x13], 1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_1_1D30
        mov    byte ptr [esi + 0x13], 0
        dec    dword ptr [edi + 0x14]
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_174F
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_1716
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_1737
loc_1_1716:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_1737
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_1737
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_174F
loc_1_1737:
        mov    byte ptr [esi + 0x11], 1
        push   esi
        call   sub_1_1D70
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_174F:
        mov    eax, 0x103
        pop    edi
        dec    dword ptr [esi + 0x86]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1763:
        mov    ebx, 0xc001000f
        mov    eax, dword ptr [edi + 0x14]
        dec    eax
        mov    dword ptr [edi + 0x14], eax
        dec    eax
        mov    dword ptr [edi + 0x14], eax
        jmp    loc_1_1781
loc_1_1775:
        mov    ebx, 0xc0010002
        jmp    loc_1_1781
loc_1_177C:
        mov    ebx, 0xc001000d
loc_1_1781:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_17D4
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_1799
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_17BA
loc_1_1799:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_17BA
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_17BA
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_17D4
loc_1_17BA:
        push   0xa
        add    esi, 0x15
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_17D4:
        mov    eax, ebx
        pop    edi
        dec    dword ptr [esi + 0x86]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        mov    ebx, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 0x10]
        push   ecx
        lea    eax, [ebp - 0xc]
        lea    esi, [edi + 0x24]
        push   eax
        push   6
        mov    byte ptr [esi + 0xa], 0
        push   0
        push   edi
        call   sub_1_07E0
        mov    ecx, dword ptr [ebp + 0xc]
        test   byte ptr [ecx + 0x41], 2
        jne    loc_1_183C
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebx + 0xb2]
        push   eax
        push   ecx
        call   sub_1_4A40
        test   al, al
        je     loc_1_183C
        or     byte ptr [esi + 0xa], 8
loc_1_183C:
        test   byte ptr [ebp - 0xc], 1
        je     loc_1_1871
        cmp    dword ptr [ebp - 0xa], -1
        jne    loc_1_1864
        mov    eax, 0xff
        cmp    byte ptr [ebp - 0xc], al
        jne    loc_1_1864
        cmp    byte ptr [ebp - 0xb], al
        jne    loc_1_1864
        or     byte ptr [esi + 0xa], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1864:
        or     byte ptr [esi + 0xa], 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1871:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x14]
        or     byte ptr [esi + 0xa], 1
        push   eax
        push   ecx
        push   6
        push   6
        push   edi
        call   sub_1_07E0
        mov    ecx, dword ptr [ebp - 0x12]
        cmp    dword ptr [ebp - 0xa], ecx
        jae    loc_1_1896
        mov    eax, 1
        jmp    loc_1_18C0
loc_1_1896:
        mov    eax, dword ptr [ebp - 0x12]
        cmp    dword ptr [ebp - 0xa], eax
        jbe    loc_1_18A5
        mov    eax, 0xffffffff
        jmp    loc_1_18C0
loc_1_18A5:
        mov    ax, word ptr [ebp - 0x14]
        cmp    word ptr [ebp - 0xc], ax
        jae    loc_1_18B6
        mov    eax, 1
        jmp    loc_1_18C0
loc_1_18B6:
        mov    ax, word ptr [ebp - 0xc]
        cmp    word ptr [ebp - 0x14], ax
        sbb    eax, eax
loc_1_18C0:
        test   eax, eax
        jne    loc_1_18C8
        mov    byte ptr [esi + 0xa], 8
loc_1_18C8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18E0:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        add    ecx, 0x24
        sub    esp, 0x1c
        xor    edx, edx
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + 0xde]
        mov    eax, dword ptr [esi + 0xd6]
        mov    dword ptr [ebp - 0x14], edx
        lea    edi, [eax + edx*8]
        mov    eax, dword ptr [esi + 0xce]
        mov    dword ptr [ebp - 0x18], edi
        lea    edi, [edx + edx*4]
        lea    edi, [edx + edi*4]
        lea    edi, [edi + edi*8]
        lea    edi, [edx + edi*4]
        mov    edx, dword ptr [ebp - 0x18]
        lea    edi, [eax + edi*2]
        mov    ax, word ptr [edx + 2]
        and    ax, 0xff00
        mov    dx, word ptr [edx + 6]
        mov    word ptr [ebp - 4], dx
        mov    dl, byte ptr [esi + 0xde]
        inc    dl
        mov    byte ptr [esi + 0xde], dl
        and    edx, 0xff
        cmp    edx, dword ptr [esi + 0xca]
        jb     loc_1_1956
        mov    byte ptr [esi + 0xde], bl
loc_1_1956:
        mov    edx, dword ptr [ebp - 0x14]
        test   ah, 0x44
        mov    byte ptr [edx + esi + 0xdf], 0
        je     loc_1_1A24
        inc    dword ptr [esi + 0x132]
        test   ah, 4
        je     loc_1_197B
        inc    dword ptr [esi + 0x19a]
loc_1_197B:
        test   ah, 0x40
        je     loc_1_1ACF
        mov    ax, word ptr [ebp - 4]
        and    ax, 0x4000
        je     loc_1_1994
        inc    dword ptr [esi + 0x1a6]
loc_1_1994:
        test   byte ptr [ebp - 3], 8
        je     loc_1_19A0
        inc    dword ptr [esi + 0x1ae]
loc_1_19A0:
        test   byte ptr [ebp - 3], 0x10
        je     loc_1_19AC
        inc    dword ptr [esi + 0x1b2]
loc_1_19AC:
        test   byte ptr [ebp - 3], 4
        je     loc_1_19B8
        inc    dword ptr [esi + 0x19e]
loc_1_19B8:
        test   ax, ax
        jne    loc_1_19C7
        test   byte ptr [ebp - 3], 0x80
        je     loc_1_1ACF
loc_1_19C7:
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_1A10
        cmp    byte ptr [esi + 0xf], 0
        jne    loc_1_1A10
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        mov    byte ptr [esi + 0x126], 1
        lea    edx, [ecx + 0x12]
        push   esi
        out    dx, ax
        lea    edx, [ecx + 0x10]
        out    dx, ax
        call   sub_1_3110
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x15]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1A09
        mov    byte ptr [esi + 0x11], 0
loc_1_1A09:
        mov    byte ptr [esi + 0x126], 0
loc_1_1A10:
        mov    edx, dword ptr [ebp - 0x14]
        pop    edi
        mov    byte ptr [edx + esi + 0xdf], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A24:
        inc    dword ptr [esi + 0x12a]
        test   ah, 8
        je     loc_1_1A35
        inc    dword ptr [esi + 0x192]
loc_1_1A35:
        test   ah, 0x10
        je     loc_1_1A40
        inc    dword ptr [esi + 0x196]
loc_1_1A40:
        mov    al, byte ptr [ecx + 0xa]
        and    eax, 0xf7
        cmp    eax, 1
        je     loc_1_1A59
        cmp    eax, 2
        je     loc_1_1A7F
        cmp    eax, 4
        je     loc_1_1AA8
        jmp    loc_1_1ACF
loc_1_1A59:
        lea    eax, [esi + 0x13e]
        xor    edx, edx
        inc    dword ptr [esi + 0x16e]
        mov    dx, word ptr [ecx + 8]
        add    edx, dword ptr [eax]
        mov    dword ptr [eax], edx
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0xffff
        cmp    ecx, edx
        jbe    loc_1_1ACF
        jmp    loc_1_1ACC
loc_1_1A7F:
        lea    edx, [esi + 0x146]
        xor    eax, eax
        inc    dword ptr [esi + 0x172]
        mov    ax, word ptr [ecx + 8]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0xffff
        cmp    ecx, eax
        jbe    loc_1_1ACF
        inc    dword ptr [edx + 4]
        jmp    loc_1_1ACF
loc_1_1AA8:
        lea    eax, [esi + 0x14e]
        xor    edx, edx
        inc    dword ptr [esi + 0x176]
        mov    dx, word ptr [ecx + 8]
        add    edx, dword ptr [eax]
        mov    dword ptr [eax], edx
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0xffff
        cmp    ecx, edx
        jbe    loc_1_1ACF
loc_1_1ACC:
        inc    dword ptr [eax + 4]
loc_1_1ACF:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   ecx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1B20
loc_1_1AE8:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edx
        push   edi
        call   sub_1_4CFC
        add    edi, dword ptr [ebp - 0xc]
        add    ebx, dword ptr [ebp - 0xc]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4C88
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1AE8
loc_1_1B20:
        cmp    ebx, 0x3c
        jge    loc_1_1B47
        mov    edx, 0x3c
        xor    eax, eax
        sub    edx, ebx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [ebp - 0x18]
        mov    word ptr [eax + 4], 0xffc4
        jmp    loc_1_1B51
loc_1_1B47:
        neg    bx
        mov    edx, dword ptr [ebp - 0x18]
        mov    word ptr [edx + 4], bx
loc_1_1B51:
        mov    edx, dword ptr [ebp - 0x14]
        cmp    byte ptr [edx + esi + 0xdf], 0
        jne    loc_1_1BAA
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 0x14]
        and    word ptr [edx + 2], 0xa3ff
        mov    word ptr [edx + 6], 0
        or     byte ptr [edx + 3], 0x80
        mov    byte ptr [eax + esi + 0xdf], 1
        pushfd 
        cli    
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edi, [ecx + 0x12]
        add    ecx, 0x10
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    si, ax
        mov    edx, edi
        and    si, 0x40
        xor    eax, eax
        out    dx, ax
        or     si, 8
        mov    edx, ecx
        mov    ax, si
        out    dx, ax
        popfd  
loc_1_1BAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 056h, 057h, 08Bh, 070h, 048h, 033h, 0C0h, 090h, 08Dh, 07Eh, 012h, 083h
        db  0C6h, 010h, 08Bh, 0D7h, 066h, 0EFh, 08Bh, 0D6h, 066h, 0EDh, 066h, 08Bh, 0C8h, 08Bh, 0D7h, 066h
        db  083h, 0E1h, 040h, 033h, 0C0h, 066h, 0EFh, 080h, 0C9h, 008h, 08Bh, 0D6h, 08Bh, 0C1h, 05Fh, 025h
        db  0FFh, 0FFh, 000h, 000h, 05Eh, 066h, 0EFh, 0B0h, 001h, 05Dh, 0C2h, 004h, 000h
sub_1_1C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_1C1D
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1C1D:
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        mov    byte ptr [esi + 0x10], 1
        lea    edi, [ecx + 0x12]
        add    ecx, 0x10
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        and    al, 0x33
        cmp    al, 0x33
        je     loc_1_1C86
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_1CE1
        cmp    byte ptr [esi + 0xf], 0
        jne    loc_1_1CE1
        mov    edx, edi
        xor    eax, eax
        mov    byte ptr [esi + 0x126], 1
        out    dx, ax
        mov    edx, ecx
        push   esi
        out    dx, ax
        call   sub_1_3110
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x15]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1C7F
        mov    byte ptr [esi + 0x11], 0
loc_1_1C7F:
        mov    byte ptr [esi + 0x126], 0
loc_1_1C86:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0xd6]
        mov    al, byte ptr [esi + 0xde]
        test   byte ptr [ecx + eax*8 + 3], 0x80
        jne    loc_1_1D14
        cmp    byte ptr [eax + esi + 0xdf], 0
        je     loc_1_1D14
        mov    edi, dword ptr [esi + 0x119]
        lea    ebx, [edi + 0x24]
        cmp    dword ptr [ebx], 0
        jne    loc_1_1CBD
        mov    dword ptr [esi + 0x11d], 0
loc_1_1CBD:
        mov    eax, dword ptr [ebx]
        push   edi
        push   esi
        mov    dword ptr [esi + 0x119], eax
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 8], ecx
        call   sub_1_18E0
        test   byte ptr [ebx + 0xa], 8
        je     loc_1_1CF0
        push   edi
        push   esi
        call   sub_1_46D0
        jmp    loc_1_1CFF
loc_1_1CE1:
        xor    al, al
        pop    edi
        mov    byte ptr [esi + 0x10], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1CF0:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   edi
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        call   sub_1_4B5A
loc_1_1CFF:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        dec    dword ptr [eax + 0x14]
        mov    al, 1
        mov    byte ptr [esi + 0x10], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D14:
        xor    al, al
        pop    edi
        mov    byte ptr [esi + 0x10], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D30:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [edx + 0x28]
        lea    esi, [edx + 0x24]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x119], 0
        jne    loc_1_1D55
        mov    dword ptr [eax + 0x119], edx
        jmp    loc_1_1D5E
loc_1_1D55:
        mov    ecx, dword ptr [eax + 0x11d]
        mov    dword ptr [ecx + 0x24], edx
loc_1_1D5E:
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebp
        mov    dword ptr [eax + 0x11d], edx
        ret    8
        db  090h
sub_1_1D70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x126], 0
        je     loc_1_1DA5
        mov    eax, dword ptr [esi + 0x86]
        cmp    eax, 2
        jne    loc_1_1DA5
        dec    eax
        push   esi
        mov    byte ptr [esi + 0x11], 0
        mov    dword ptr [esi + 0x86], eax
        call   sub_1_3850
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1DA5:
        xor    ebx, ebx
loc_1_1DA7:
        cmp    dword ptr [esi + 0x119], ebx
        je     loc_1_1DC6
        cmp    byte ptr [esi + 0x126], bl
        jne    loc_1_1DC6
        cmp    byte ptr [esi + 0x13], bl
        jne    loc_1_1DC6
        push   esi
        call   sub_1_1C00
        test   al, al
        jne    loc_1_1DA7
loc_1_1DC6:
        xor    ebx, ebx
loc_1_1DC8:
        cmp    dword ptr [esi + 0x10d], ebx
        je     loc_1_1DDF
        cmp    byte ptr [esi + 0x14], bl
        jne    loc_1_1DDF
        push   esi
        call   sub_1_45B0
        test   al, al
        jne    loc_1_1DC8
loc_1_1DDF:
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_1E32
        xor    edi, edi
loc_1_1DF0:
        mov    ecx, dword ptr [esi + 0x96]
        cmp    dword ptr [ecx + 0x14], edi
        jne    loc_1_1E23
        mov    ebx, dword ptr [ecx + 0x10]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        push   edi
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        push   0x45
        push   ecx
        mov    dword ptr [edx + 4], eax
        call   sub_1_4C1C
        push   edi
        dec    dword ptr [esi + 0x92]
        push   ebx
        call   sub_1_4B54
loc_1_1E23:
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        jne    loc_1_1DF0
loc_1_1E32:
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_1E64
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_1E64
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        jne    loc_1_1E64
        pop    edi
        dec    dword ptr [esi + 0x86]
        mov    byte ptr [esi + 0x11], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1E64:
        push   0xa
        add    esi, 0x15
        push   esi
        call   sub_1_4BF2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E80:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x40
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   0                             ; -> obj1:0x53D0
        call   sub_1_4B2A
        push   -1
        lea    ecx, [ebp - 8]
        push   0
        push   8
        push   ecx
        call   sub_1_4C16
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_1_1EBC
        mov    eax, 0xc000009a
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1EBC:
        mov    eax, dword ptr [0]            ; -> obj1:0x53D0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x40], 3
        mov    byte ptr [ebp - 0x3f], 0
        mov    dword ptr [ebp - 0x3e], 0
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_1_3400
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_1_3540
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_1_1560
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_44D0
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_1_3700
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_1_09A0
        mov    dword ptr [ebp - 0x22], 0     ; -> loc_1_0CE0
        mov    edx, dword ptr [0]            ; -> obj1:0x4F04
        mov    eax, dword ptr [4]            ; -> obj1:0x4F08
        push   0x36
        mov    dword ptr [ebp - 0x12], edx
        lea    ecx, [ebp - 0x40]
        mov    dword ptr [ebp - 0xe], eax
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   edx
        mov    eax, dword ptr [0]            ; -> obj1:0x53D0
        push   eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x1e], 0     ; -> loc_1_35F0
        mov    dword ptr [ebp - 0x1a], 0     ; -> loc_1_1F80
        mov    dword ptr [ebp - 0x16], 0     ; -> loc_1_3620
        push   0                             ; -> obj1:0x53D8
        push   ecx
        call   sub_1_4B36
        mov    edx, dword ptr [0]            ; -> obj1:0x53D8
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], edx
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1F64
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F64:
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x53D0
        push   eax
        call   sub_1_4B30
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_1F80:
        push   ebp
        mov    eax, 0xa
        mov    ebp, esp
        sub    esp, 0xa4
        mov    word ptr [ebp - 0x94], ax
        push   esi
        mov    word ptr [ebp - 0x92], ax
        push   edi
        mov    word ptr [ebp - 0xa4], ax
        xor    edi, edi
        mov    word ptr [ebp - 0xa2], ax
        mov    dword ptr [ebp - 4], edi
        mov    dword ptr [ebp - 0x90], 0     ; -> obj1:0x52AC
        mov    dword ptr [ebp - 0xa0], 0     ; -> loc_1_52C4
        mov    dword ptr [ebp - 0x98], 0     ; -> loc_1_52DC
        mov    dword ptr [ebp - 0x88], 0     ; -> obj1:0x5304
        mov    eax, 0xb
        mov    ecx, 5
        mov    word ptr [ebp - 0x9c], ax
        mov    dword ptr [ebp - 0x50], 0     ; -> loc_1_5318
        mov    word ptr [ebp - 0x9a], ax
        mov    eax, 0xc
        mov    word ptr [ebp - 0x8c], ax
        mov    dword ptr [ebp - 0x20], 0     ; -> loc_1_5324
        mov    word ptr [ebp - 0x8a], ax
        mov    eax, 3
        mov    word ptr [ebp - 0x54], cx
        mov    word ptr [ebp - 0x52], cx
        mov    dword ptr [ebp - 0x40], 0     ; -> obj1:0x5330
        mov    word ptr [ebp - 0x24], ax
        mov    word ptr [ebp - 0x22], ax
        mov    dword ptr [ebp - 0x38], 0     ; -> obj1:0x5340
        mov    word ptr [ebp - 0x44], cx
        mov    word ptr [ebp - 0x42], cx
        mov    dword ptr [ebp - 0x30], 0     ; -> obj1:0x5350
        mov    word ptr [ebp - 0x3c], cx
        mov    word ptr [ebp - 0x3a], cx
        mov    dword ptr [ebp - 0x28], 0     ; -> obj1:0x5360
        mov    word ptr [ebp - 0x34], cx
        mov    eax, 9
        mov    word ptr [ebp - 0x32], cx
        mov    dword ptr [ebp - 0x48], 0     ; -> loc_1_5374
        mov    word ptr [ebp - 0x2c], cx
        mov    dword ptr [ebp - 0x80], 0     ; -> obj1:0x5384
        mov    word ptr [ebp - 0x2a], cx
        mov    word ptr [ebp - 0x4c], ax
        mov    dword ptr [ebp - 0x78], 0     ; -> obj1:0x5390
        mov    word ptr [ebp - 0x4a], ax
        mov    eax, 4
        mov    word ptr [ebp - 0x84], ax
        mov    word ptr [ebp - 0x82], ax
        mov    dword ptr [ebp - 0x70], 0     ; -> obj1:0x53A0
        mov    word ptr [ebp - 0x7c], cx
        mov    word ptr [ebp - 0x7a], cx
        mov    dword ptr [ebp - 0x68], 0     ; -> obj1:0x53AC
        mov    word ptr [ebp - 0x74], cx
        mov    word ptr [ebp - 0x72], cx
        mov    word ptr [ebp - 0x6c], ax
        mov    word ptr [ebp - 0x6a], ax
        mov    word ptr [ebp - 0x64], cx
        mov    word ptr [ebp - 0x62], cx
        mov    word ptr [ebp - 0x5c], ax
        mov    esi, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 0x5a], ax
        cmp    esi, edi
        mov    dword ptr [ebp - 0x60], 0     ; -> loc_1_53B8
        mov    dword ptr [ebp - 0x58], 0     ; -> obj1:0x53C4
        je     loc_1_253E
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4C94
        cmp    dword ptr [ebp - 4], edi
        je     loc_1_2105
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2105:
        push   0
        lea    eax, [ebp - 0xa4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2130
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> loc_1_5244
loc_1_2130:
        push   0
        lea    eax, [ebp - 0x9c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_215B
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> loc_1_5248
loc_1_215B:
        push   1
        lea    eax, [ebp - 0x94]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2187
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0], ecx            ; -> loc_1_5240
loc_1_2187:
        mov    edx, 0xc80
        in     eax, dx
        cmp    eax, 0x107110e
        jne    loc_1_2199
        mov    edi, 0x8800
loc_1_2199:
        push   edi
        call   sub_1_4380
        test   al, al
        jne    loc_1_21A5
        xor    edi, edi
loc_1_21A5:
        test   edi, edi
        je     loc_1_21AF
        mov    dword ptr [0], edi            ; -> loc_1_5240
loc_1_21AF:
        push   2
        lea    eax, [ebp - 0x8c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_22A8
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 0x84]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     loc_1_21FB
        mov    dword ptr [0], 1              ; -> loc_1_524C
        jmp    loc_1_22B2
loc_1_21FB:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 0x7c]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     loc_1_2220
        mov    dword ptr [0], 5              ; -> loc_1_524C
        jmp    loc_1_22B2
loc_1_2220:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 0x74]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     loc_1_2242
        mov    dword ptr [0], 6              ; -> loc_1_524C
        jmp    loc_1_22B2
loc_1_2242:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 0x6c]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     loc_1_2264
        mov    dword ptr [0], 2              ; -> loc_1_524C
        jmp    loc_1_22B2
loc_1_2264:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 0x64]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     loc_1_2286
        mov    dword ptr [0], 3              ; -> loc_1_524C
        jmp    loc_1_22B2
loc_1_2286:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 0x5c]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     loc_1_22A8
        mov    dword ptr [0], 4              ; -> loc_1_524C
        jmp    loc_1_22B2
loc_1_22A8:
        mov    dword ptr [0], 0              ; -> loc_1_524C
loc_1_22B2:
        push   1
        lea    eax, [ebp - 0x54]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2320
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [0], cx              ; -> obj1:0x5250
        test   cx, cx
        je     loc_1_2320
        cmp    word ptr [0], 1               ; -> obj1:0x5250
        jne    loc_1_22F6
        mov    word ptr [0], 0               ; -> obj1:0x5250
        jmp    loc_1_2329
loc_1_22F6:
        cmp    word ptr [0], 2               ; -> obj1:0x5250
        jne    loc_1_230B
        mov    word ptr [0], 3               ; -> obj1:0x5250
        jmp    loc_1_2329
loc_1_230B:
        cmp    word ptr [0], 3               ; -> obj1:0x5250
        jne    loc_1_2320
        mov    word ptr [0], 1               ; -> obj1:0x5250
        jmp    loc_1_2329
loc_1_2320:
        mov    word ptr [0], 0xffff          ; -> obj1:0x5250
loc_1_2329:
        push   0
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_23BA
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        movsx  eax, cx
        mov    word ptr [0], cx              ; -> loc_1_5254
        cmp    eax, 0xd
        ja     loc_1_23C3
        jmp    dword ptr [eax*4]             ; -> obj1:0x257C
loc_1_2362:
        mov    word ptr [0], 0x32            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_236D:
        mov    word ptr [0], 0x46            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_2378:
        mov    word ptr [0], 0x50            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_2383:
        mov    word ptr [0], 0x5a            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_238E:
        mov    word ptr [0], 0x64            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_2399:
        mov    word ptr [0], 0x6e            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_23A4:
        mov    word ptr [0], 0x78            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_23AF:
        mov    word ptr [0], 0x82            ; -> loc_1_5254
        jmp    loc_1_23C3
loc_1_23BA:
        mov    word ptr [0], 0x3c            ; -> loc_1_5254
loc_1_23C3:
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_23F4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0], ecx            ; -> obj1:0x525C
        cmp    ecx, 0x10000
        jne    loc_1_23FE
loc_1_23F4:
        mov    dword ptr [0], 0xffffffff     ; -> obj1:0x525C
loc_1_23FE:
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_242F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0], ecx            ; -> obj1:0x5260
        cmp    ecx, 0x10000
        jne    loc_1_2439
loc_1_242F:
        mov    dword ptr [0], 0xffffffff     ; -> obj1:0x5260
loc_1_2439:
        push   1
        lea    eax, [ebp - 0x34]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_246A
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0], ecx            ; -> obj1:0x5264
        cmp    ecx, 0x10000
        jne    loc_1_2474
loc_1_246A:
        mov    dword ptr [0], 0xffffffff     ; -> obj1:0x5264
loc_1_2474:
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_24A5
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0], ecx            ; -> obj1:0x5268
        cmp    ecx, 0x10000
        jne    loc_1_24AF
loc_1_24A5:
        mov    dword ptr [0], 0xffffffff     ; -> obj1:0x5268
loc_1_24AF:
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_24D8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0], ecx            ; -> loc_1_5258
loc_1_24D8:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x14]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_4B9C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_252E
        cmp    dword ptr [ebp - 0x14], 6
        jne    loc_1_252E
        mov    eax, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [eax + 2]
        or     cl, byte ptr [eax + 3]
        or     cl, byte ptr [eax + 4]
        or     cl, byte ptr [eax + 5]
        or     cl, byte ptr [eax + 1]
        or     cl, byte ptr [eax]
        je     loc_1_252E
        xor    dl, dl
loc_1_2511:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, dl
        inc    dl
        cmp    dl, 6
        mov    cl, byte ptr [ecx + eax]
        mov    byte ptr [ebp + eax - 0x1c], cl
        jb     loc_1_2511
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_2535
loc_1_252E:
        mov    dword ptr [ebp - 0xc], 0
loc_1_2535:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_4CA0
loc_1_253E:
        push   0x20
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [0]            ; -> obj1:0x53D8
        push   esi
        push   edx
        call   sub_1_29F0
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        pop    edi
        mov    dword ptr [0], eax            ; -> loc_1_523C
        pop    esi
        mov    dword ptr [0], eax            ; -> loc_1_5240
        mov    byte ptr [0], al              ; -> loc_1_5244
        mov    byte ptr [0], al              ; -> loc_1_5248
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
        ; ---- dispatch table (14 entries) ----
        dd  loc_1_23BA
        dd  loc_1_23C3
        dd  loc_1_23C3
        dd  loc_1_23C3
        dd  loc_1_23C3
        dd  loc_1_2362
        dd  loc_1_23BA
        dd  loc_1_236D
        dd  loc_1_2378
        dd  loc_1_2383
        dd  loc_1_238E
        dd  loc_1_2399
        dd  loc_1_23A4
        dd  loc_1_23AF
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_25C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   eax
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    loc_1_2794
        cmp    dword ptr [ebp - 4], 0x20001022
        jne    loc_1_2794
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    loc_1_2794
        test   byte ptr [ebp - 4], 1
        je     loc_1_2794
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BCC
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   0x10
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 2
        jne    loc_1_2794
        mov    edi, dword ptr [ebp - 4]
        push   4
        and    edi, 0xfffe
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BCC
        test   edi, edi
        je     loc_1_2794
        push   edi
        call   sub_1_4380
        test   al, al
        je     loc_1_2794
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [0], edi            ; -> loc_1_5240
        mov    dword ptr [0], 0x20001022     ; -> obj1:0x53CC
        mov    dword ptr [0], 1              ; -> obj1:0x53D4
        push   0x3c
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    loc_1_26BA
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [0], bl              ; -> loc_1_5248
        mov    byte ptr [0], al              ; -> loc_1_5244
loc_1_26BA:
        push   4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   0
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    loc_1_26F1
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, 0xffff0000
        jmp    loc_1_26F4
loc_1_26F1:
        mov    ebx, dword ptr [ebp - 4]
loc_1_26F4:
        xor    edi, edi
        push   4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], edi
        push   eax
        push   edi
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   4
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   edi
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   edi
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    loc_1_2747
        mov    ebx, dword ptr [ebp - 4]
        and    ebx, 0xfffb
loc_1_2747:
        xor    edi, edi
        push   4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], edi
        push   eax
        push   edi
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        mov    dword ptr [ebp - 4], ebx
        lea    ebx, [ebp - 4]
        push   ebx
        push   4
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        mov    dword ptr [ebp - 4], edi
        push   ebx
        push   edi
        push   edi
        push   esi
        mov    esi, 0x27100
        call   sub_1_4BCC
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 0x14
        in     ax, dx
loc_1_278A:
        push   1
        call   sub_1_4C10
        dec    esi
        jne    loc_1_278A
loc_1_2794:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_27A0:
        cmp    dword ptr [0], 0              ; -> obj1:0x53D4
        push   esi
        push   edi
        jne    loc_1_2895
        mov    esi, 0x200
loc_1_27B4:
        push   esi
        call   sub_1_4440
        test   al, al
        je     loc_1_27D0
        lea    edx, [esi + 9]
        in     al, dx
        cmp    al, 1
        jne    loc_1_27D0
        push   esi
        call   sub_1_4380
        test   al, al
        jne    loc_1_27E2
loc_1_27D0:
        add    esi, 0x20
        cmp    esi, 0x400
        jb     loc_1_27B4
        mov    eax, dword ptr [0]            ; -> obj1:0x53D4
        jmp    loc_1_27F1
loc_1_27E2:
        mov    dword ptr [0], 1              ; -> obj1:0x53CC
        mov    eax, 2
loc_1_27F1:
        mov    dword ptr [0], eax            ; -> obj1:0x53D4
        test   eax, eax
        je     loc_1_2895
        lea    edi, [esi + 0x12]
        mov    eax, 8
        mov    edx, edi
        mov    dword ptr [0], esi            ; -> loc_1_5240
        add    esi, 0x16
        mov    cl, byte ptr [0]              ; -> loc_1_5244
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        shr    ax, 4
        and    ax, 0xf
        test   cl, cl
        je     loc_1_2847
        and    ecx, 0xff
        mov    edx, eax
        and    edx, 0xffff
        cmp    ecx, edx
        je     loc_1_2847
        mov    dword ptr [0], 8              ; -> loc_1_523C
        jmp    loc_1_284C
loc_1_2847:
        mov    byte ptr [0], al              ; -> loc_1_5244
loc_1_284C:
        mov    edx, edi
        mov    eax, 8
        out    dx, ax
        mov    edx, esi
        mov    cl, byte ptr [0]              ; -> loc_1_5248
        in     ax, dx
        and    ax, 7
        test   cl, cl
        je     loc_1_2890
        and    ecx, 0xff
        mov    edx, eax
        and    edx, 0xffff
        cmp    ecx, edx
        je     loc_1_2890
        mov    dword ptr [0], 0xb            ; -> loc_1_523C
        mov    dword ptr [0], 0              ; -> obj1:0x53D4
        pop    edi
        pop    esi
        ret    
loc_1_2890:
        mov    byte ptr [0], al              ; -> loc_1_5248
loc_1_2895:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_28A0:
        cmp    dword ptr [0], 0              ; -> obj1:0x53D4
        push   esi
        push   edi
        jne    loc_1_294F
        mov    si, 0x200
loc_1_28B3:
        mov    edi, esi
        and    edi, 0xffff
        push   edi
        call   sub_1_4440
        test   al, al
        je     loc_1_28D7
        lea    edx, [edi + 9]
        in     al, dx
        cmp    al, 0x10
        jne    loc_1_28D7
        push   edi
        call   sub_1_4380
        test   al, al
        jne    loc_1_28E9
loc_1_28D7:
        add    si, 0x20
        cmp    si, 0x400
        jb     loc_1_28B3
        mov    eax, dword ptr [0]            ; -> obj1:0x53D4
        jmp    loc_1_28F8
loc_1_28E9:
        mov    dword ptr [0], 0x10           ; -> obj1:0x53CC
        mov    eax, 3
loc_1_28F8:
        mov    dword ptr [0], eax            ; -> obj1:0x53D4
        test   eax, eax
        je     loc_1_294F
        mov    edx, esi
        xor    al, al
        and    edx, 0xffff
        mov    byte ptr [0], al              ; -> loc_1_5248
        mov    dword ptr [0], edx            ; -> loc_1_5240
        add    edx, 8
        in     al, dx
        mov    dl, al
        mov    al, byte ptr [0]              ; -> loc_1_5244
        and    edx, 0xffff00ff
        test   al, al
        je     loc_1_2949
        and    eax, 0xff
        mov    ecx, edx
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_2949
        mov    dword ptr [0], 8              ; -> loc_1_523C
        pop    edi
        pop    esi
        ret    
loc_1_2949:
        mov    byte ptr [0], dl              ; -> loc_1_5244
loc_1_294F:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2960:
        cmp    dword ptr [0], 0              ; -> obj1:0x53D4
        push   esi
        push   edi
        jne    loc_1_29EA
        mov    si, 0x300
loc_1_296F:
        mov    edi, esi
        and    edi, 0xffff
        push   edi
        call   sub_1_4440
        test   al, al
        je     loc_1_299E
        push   edi
        call   sub_1_4380
        test   al, al
        je     loc_1_299E
        lea    edx, [edi + 9]
        in     al, dx
        and    ax, 0xff
        test   ax, ax
        je     loc_1_29B0
        cmp    ax, 0xff
        je     loc_1_29B0
loc_1_299E:
        add    si, 0x20
        cmp    si, 0x360
        jbe    loc_1_296F
        mov    eax, dword ptr [0]            ; -> obj1:0x53D4
        jmp    loc_1_29B5
loc_1_29B0:
        mov    eax, 4
loc_1_29B5:
        mov    dword ptr [0], eax            ; -> obj1:0x53D4
        test   eax, eax
        je     loc_1_29EA
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        jne    loc_1_29CE
        mov    byte ptr [0], 3               ; -> loc_1_5244
loc_1_29CE:
        cmp    byte ptr [0], 0               ; -> loc_1_5248
        jne    loc_1_29DE
        mov    byte ptr [0], 5               ; -> loc_1_5248
loc_1_29DE:
        mov    eax, esi
        and    eax, 0xffff
        mov    dword ptr [0], eax            ; -> loc_1_5240
loc_1_29EA:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_29F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   -1
        push   0
        push   0x286
        push   eax
        call   sub_1_4C16
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_300F
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0xa1
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        lea    edi, [ebp - 0x30]
        xor    edx, edx
        mov    dword ptr [ecx + 0xaa], eax
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dl, byte ptr [0]              ; -> loc_1_5248
        mov    eax, 1
        mov    dword ptr [ebp - 0x30], edx
        mov    byte ptr [ebp - 0x2c], al
        mov    dword ptr [ebp - 0x22], eax
        mov    dword ptr [ebp - 0x26], eax
        mov    dword ptr [ebp - 0x1e], 0x600
        cmp    dword ptr [0], eax            ; -> loc_1_524C
        jne    loc_1_2A6D
        mov    dword ptr [ebp - 0x26], 5
loc_1_2A6D:
        cmp    dword ptr [0], 3              ; -> loc_1_524C
        jne    loc_1_2A7D
        mov    dword ptr [ebp - 0x26], 2
loc_1_2A7D:
        cmp    dword ptr [0], 0              ; -> loc_1_523C
        jne    loc_1_2A97
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_3D50
        mov    esi, eax
        jmp    loc_1_2AA0
loc_1_2A97:
        mov    esi, dword ptr [0]            ; -> loc_1_523C
        mov    edi, dword ptr [ebp + 0xc]
loc_1_2AA0:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x6a
        cmp    dword ptr [eax], 1
        jne    loc_1_2AB2
        mov    dword ptr [ebp - 0x26], 5
loc_1_2AB2:
        cmp    dword ptr [eax], 3
        jne    loc_1_2ABE
        mov    dword ptr [ebp - 0x26], 2
loc_1_2ABE:
        cmp    esi, 9
        jb     loc_1_2ACC
        mov    dword ptr [ebp - 0x1a], 0
        jmp    loc_1_2AED
loc_1_2ACC:
        xor    eax, eax
        mov    ecx, dword ptr [0]            ; -> loc_1_5240
        mov    al, byte ptr [0]              ; -> loc_1_5248
        mov    dword ptr [ebp - 0x16], ecx
        mov    dword ptr [ebp - 0x1a], 1
        mov    dword ptr [ebp - 0x12], 0x20
        mov    dword ptr [ebp - 0x30], eax
loc_1_2AED:
        lea    eax, [ebp - 0x30]
        mov    ecx, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edi
        push   edx
        mov    eax, dword ptr [edx + 0xaa]
        push   eax
        lea    eax, [edx + 0xae]
        push   eax
        call   sub_1_4B42
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_2B30
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x286
        push   eax
        call   sub_1_4C1C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2B30:
        mov    eax, dword ptr [ebp - 4]
        cmp    esi, 9
        jb     loc_1_2B62
        mov    ecx, dword ptr [eax + 0xae]
        push   ecx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x286
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2B62:
        cmp    dword ptr [eax + 0x6a], 1
        je     loc_1_2B70
        mov    edx, dword ptr [eax + 0x48]
        add    edx, 0x14
        in     ax, dx
loc_1_2B70:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xca], 0x10
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xef], 0x20
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        test   al, al
        je     loc_1_2FD9
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_1_2BA8
        mov    eax, dword ptr [ebp - 4]
        add    eax, 3
loc_1_2BA8:
        push   6
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9
        push   eax
        call   sub_1_4CFC
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    dword ptr [edx + 0x82], ecx
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 0x105]
        mov    word ptr [eax], cx
loc_1_2BCE:
        mov    esi, ecx
        mov    eax, dword ptr [ebp - 4]
        and    esi, 0xff
        inc    cl
        mov    ebx, dword ptr [eax + 0x105]
        cmp    cl, 6
        mov    dl, byte ptr [esi + eax + 9]
        mov    byte ptr [ebx + esi + 2], dl
        jb     loc_1_2BCE
        xor    ecx, ecx
        xor    edx, edx
loc_1_2BF2:
        mov    esi, ecx
        mov    eax, dword ptr [ebp - 4]
        and    esi, 0xff
        inc    cl
        mov    ebx, dword ptr [eax + 0x105]
        cmp    cl, 8
        mov    byte ptr [esi + ebx + 8], dl
        jb     loc_1_2BF2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xff]
        mov    edx, dword ptr [eax + 0x105]
        mov    word ptr [edx + 0x10], cx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xef]
        shr    ecx, 1
        test   ecx, ecx
        je     loc_1_2C4A
        mov    edx, 0x2000
loc_1_2C36:
        shr    ecx, 1
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 0x105]
        add    word ptr [ebx + 0x12], dx
        test   ecx, ecx
        jne    loc_1_2C36
loc_1_2C4A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x105]
        mov    eax, dword ptr [eax + 0xff]
        shr    eax, 0x10
        and    ax, 0xff
        or     word ptr [ecx + 0x12], ax
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx + 0xda]
        mov    eax, dword ptr [edx + 0x105]
        mov    word ptr [eax + 0x14], cx
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx + 0xca]
        shr    ecx, 1
        test   ecx, ecx
        je     loc_1_2CA0
        mov    edx, 0x2000
loc_1_2C8C:
        shr    ecx, 1
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 0x105]
        add    word ptr [ebx + 0x16], dx
        test   ecx, ecx
        jne    loc_1_2C8C
loc_1_2CA0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x105]
        mov    eax, dword ptr [eax + 0xda]
        shr    eax, 0x10
        and    ax, 0xff
        or     word ptr [ecx + 0x16], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8a
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x8e], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x8e]
        mov    dword ptr [eax + 0x8a], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x96
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0x9a], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x9a]
        mov    dword ptr [eax + 0x96], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xb6], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xba], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xbe], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x92], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xf], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x12], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x11], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x13], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x14], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10d], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x111], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x115], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x119], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x11d], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x125], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x86], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x126], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x127], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x128], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x121], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x129], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x12a], ebx
        mov    dword ptr [eax + 0x12e], ebx
        mov    dword ptr [eax + 0x132], ebx
        mov    dword ptr [eax + 0x136], ebx
        mov    dword ptr [eax + 0x13a], ebx
        add    eax, 0x12a
        mov    ecx, 0xc
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        add    edi, 0x13e
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 6
        add    edi, 0x16e
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x186], ebx
        mov    dword ptr [eax + 0x18a], ebx
        add    eax, 0x186
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18e], ebx
        mov    dword ptr [eax + 0x192], ebx
        mov    dword ptr [eax + 0x196], ebx
        add    eax, 0x18e
        mov    ecx, 7
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        add    edi, 0x19a
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xb2
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x9e
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9
        push   eax
        push   0                             ; -> loc_1_1550
        push   0                             ; -> loc_1_1300
        push   0                             ; -> loc_1_1410
        push   0x10
        call   sub_2_0000
loc_1_2E71:
        test   al, al
        jne    loc_1_2EAA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0110
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xae]
        push   ecx
        call   sub_1_4B48
        push   ebx
        mov    ecx, dword ptr [ebp - 4]
        push   0x286
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2EAA:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        add    edi, 0x2e
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        cmp    dword ptr [ebp - 0x26], 1
        jne    loc_1_2ECC
        xor    eax, eax
        push   1
        mov    al, byte ptr [0]              ; -> loc_1_5244
        push   0
        jmp    loc_1_2ED7
loc_1_2ECC:
        xor    eax, eax
        push   2
        mov    al, byte ptr [0]              ; -> loc_1_5244
        push   1
loc_1_2ED7:
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> loc_1_0230
        push   ecx
        mov    eax, dword ptr [ecx + 0xae]
        push   0                             ; -> loc_1_0170
        push   eax
        lea    eax, [ecx + 0x2e]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4BFE
        cmp    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        je     loc_1_2F44
        mov    ecx, dword ptr [eax + 0xb2]
        push   ecx
        call   sub_2_0120
loc_1_2F10:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_0110
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0xae]
        push   edx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x286
        push   ecx
        call   sub_1_4C1C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2F44:
        mov    dword ptr [eax + 0x15], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x19], 0     ; -> sub_1_04A0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1d], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3020
        test   al, al
        jne    loc_1_2FB9
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2e
        push   eax
        call   sub_1_4C04
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0xb2]
        push   edx
        call   sub_2_0120
loc_1_2F83:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_0110
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0xae]
        push   edx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x286
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2FB9:
        push   0                             ; -> loc_1_44B0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [eax + 0xae]
        push   ecx
        call   sub_1_4CEE
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2FD9:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0110
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xae]
        push   ecx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x286
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_300F:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
sub_1_3020:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x6a], 1
        je     loc_1_30B3
        push   esi
        call   sub_1_3280
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 0x59
        lea    ebx, [ecx + 0x12]
        add    ecx, 0x10
        mov    edx, ebx
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    edi, eax
        mov    edx, ebx
        and    edi, 0xffff
        mov    eax, 0x58
        out    dx, ax
        shl    edi, 0x10
        mov    edx, ecx
        in     ax, dx
        and    eax, 0xffff
        lea    ecx, [eax + edi]
        mov    eax, ecx
        and    eax, 0xfffffff
        cmp    eax, 0x3003
        jne    loc_1_3088
        shr    ecx, 0x1c
        mov    byte ptr [esi], 1
        mov    byte ptr [esi + 1], cl
        jmp    loc_1_30B3
loc_1_3088:
        cmp    eax, 0x2260
        jne    loc_1_309A
        shr    ecx, 0x1c
        mov    byte ptr [esi], 2
        mov    byte ptr [esi + 1], cl
        jmp    loc_1_30B3
loc_1_309A:
        cmp    eax, 0x2261
        jne    loc_1_30AC
        shr    ecx, 0x1c
        mov    byte ptr [esi], 3
        mov    byte ptr [esi + 1], cl
        jmp    loc_1_30B3
loc_1_30AC:
        test   eax, eax
        jne    loc_1_30B3
        mov    byte ptr [esi], 0
loc_1_30B3:
        push   esi
        call   sub_1_3910
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3140
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_30D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x48]
        cmp    byte ptr [eax + 0x125], 0
        lea    edx, [ecx + 0x12]
        je     loc_1_30FA
        xor    eax, eax
        pop    ebp
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 3
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_30FA:
        xor    eax, eax
        pop    ebp
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 0x43
        out    dx, ax
        mov    al, 1
        ret    4
        db  08Bh, 0FFh
sub_1_3110:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        pushfd 
        cli    
        push   esi
        call   sub_1_30D0
        popfd  
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3140:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x48]
        xor    eax, eax
        pop    ebp
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 0x43
        out    dx, ax
        ret    4
        db  090h
sub_1_3160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   0
        mov    ecx, dword ptr [esi + 0xae]
        push   ecx
        call   sub_1_4BD2
        cmp    eax, 4
        jne    loc_1_31DF
        mov    ebx, dword ptr [ebp - 4]
        push   4
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 0xae]
        push   eax
        and    ebx, 0xfffb
        mov    dword ptr [ebp - 4], 0
        push   0
        push   0
        push   ecx
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0xae]
        mov    dword ptr [ebp - 4], ebx
        lea    ebx, [ebp - 4]
        push   ebx
        push   4
        push   0
        push   eax
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0xae]
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   0
        push   0
        push   eax
        call   sub_1_4BD8
loc_1_31DF:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   0
        mov    ecx, dword ptr [esi + 0xae]
        push   ecx
        call   sub_1_4BD2
        cmp    eax, 4
        jne    loc_1_3272
        mov    ebx, dword ptr [ebp - 4]
        push   4
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 0xae]
        push   eax
        or     ebx, 4
        push   0
        and    ebx, 0xffff
        mov    dword ptr [ebp - 4], 0
        push   0
        push   ecx
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0xae]
        mov    dword ptr [ebp - 4], ebx
        lea    ebx, [ebp - 4]
        push   ebx
        push   4
        push   0
        push   eax
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0xae]
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   0
        push   0
        push   eax
        call   sub_1_4BD8
loc_1_3272:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3280:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        cmp    dword ptr [0], 0              ; -> obj1:0x5284
        push   ebx
        push   esi
        push   edi
        jne    loc_1_33EC
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [0], 1              ; -> obj1:0x5284
        cmp    dword ptr [esi + 0x6a], 1
        jne    loc_1_33BF
        mov    dword ptr [ebp - 0xc], 0
        mov    edi, 0x100
loc_1_32B9:
        push   esi
        call   sub_1_3160
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    ebx, [ecx + 0x12]
        add    ecx, 0x10
        mov    edx, ebx
        out    dx, ax
        mov    edx, ecx
        mov    eax, 4
        out    dx, ax
        mov    edx, ebx
        mov    eax, 3
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x300
        out    dx, ax
        mov    edx, ebx
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 0x109]
        mov    dword ptr [ebp - 8], edx
        mov    edx, ecx
        shr    dword ptr [ebp - 8], 0x10
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xffff00ff
        out    dx, ax
        mov    edx, ebx
        mov    eax, 1
        out    dx, ax
        mov    ax, word ptr [esi + 0x109]
        mov    edx, ecx
        out    dx, ax
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0xff00
        out    dx, ax
        mov    edx, ebx
        xor    eax, eax
        xor    bx, bx
        push   esi
        out    dx, ax
        mov    edx, ecx
        mov    eax, 1
        out    dx, ax
        call   sub_1_31F0
loc_1_3346:
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   dword ptr [ebp - 4], edi
        jne    loc_1_3370
        inc    bx
        push   1
        call   sub_1_4C10
        cmp    bx, 0x3e8
        jb     loc_1_3346
        jmp    loc_1_3384
loc_1_3370:
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 0xff00
        out    dx, ax
loc_1_3384:
        test   dword ptr [ebp - 4], edi
        jne    loc_1_3398
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 9
        jb     loc_1_32B9
        jmp    loc_1_33E2
loc_1_3398:
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        mov    dword ptr [0], 0              ; -> obj1:0x5284
        lea    edx, [ecx + 0x12]
        pop    edi
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_33BF:
        mov    ecx, dword ptr [esi + 0x48]
        xor    eax, eax
        mov    si, 5
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    eax, 4
        out    dx, ax
loc_1_33D7:
        push   1
        call   sub_1_4C10
        dec    si
        jne    loc_1_33D7
loc_1_33E2:
        mov    dword ptr [0], 0              ; -> obj1:0x5284
loc_1_33EC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3400:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 2], 0
        je     loc_1_341E
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_341E:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 0x18]
        cmp    edx, eax
        je     loc_1_343B
        mov    ecx, dword ptr [ebp + 0x14]
loc_1_342A:
        cmp    dword ptr [ecx], 0
        je     loc_1_3437
        add    ecx, 4
        inc    edx
        cmp    edx, eax
        jb     loc_1_342A
loc_1_3437:
        cmp    edx, eax
        jne    loc_1_3448
loc_1_343B:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_3448:
        mov    eax, dword ptr [ebp + 0x10]
        push   -1
        push   0
        lea    ecx, [ebp - 4]
        push   0x45
        mov    dword ptr [eax], edx
        push   ecx
        inc    dword ptr [esi + 0x86]
        call   sub_1_4C16
        test   eax, eax
        jne    loc_1_34D4
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
        mov    byte ptr [ecx + 0x40], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x41], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x28], 3
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x18], 0
        add    eax, 0x18
        mov    edi, 0x103
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x18
        push   eax
        push   esi
        call   sub_1_0A60
        jmp    loc_1_34D9
loc_1_34D4:
        mov    edi, 0xc000009a
loc_1_34D9:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_352B
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_34F1
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_3512
loc_1_34F1:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_3512
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_3512
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_352B
loc_1_3512:
        push   0xa
        add    esi, 0x15
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_352B:
        mov    eax, edi
        dec    dword ptr [esi + 0x86]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  005h, 000h, 000h, 000h, 000h
loc_1_3540:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x86]
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_357E
        mov    dword ptr [edi + 0x28], 4
        mov    dword ptr [edi + 0x1c], edi
        lea    eax, [edi + 0x18]
        push   eax
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        call   sub_1_0A60
        dec    dword ptr [edi + 0x14]
        mov    edi, 0x103
        jmp    loc_1_3583
loc_1_357E:
        mov    edi, 0xc0010002
loc_1_3583:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_35D3
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_359B
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_35BC
loc_1_359B:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_35BC
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_35BC
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_35D3
loc_1_35BC:
        push   0xa
        add    esi, 0x15
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_35D3:
        mov    eax, edi
        dec    dword ptr [esi + 0x86]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35F0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x53D8
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4B3C
        push   0
        mov    ecx, dword ptr [0]            ; -> obj1:0x53D0
        push   ecx
        call   sub_1_4B30
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x27100
        push   esi
        mov    byte ptr [esi + 2], 1
        call   sub_1_3280
        mov    edx, dword ptr [esi + 0x48]
        add    edx, 0x14
        in     ax, dx
loc_1_3642:
        push   1
        call   sub_1_4C10
        dec    edi
        jne    loc_1_3642
        mov    eax, dword ptr [esi + 0xae]
        push   eax
        call   sub_1_4CF4
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x15]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_366F
        mov    byte ptr [esi + 0x11], 0
loc_1_366F:
        lea    eax, [esi + 0x2e]
        push   eax
        call   sub_1_4C04
        mov    eax, dword ptr [esi + 0xb2]
        push   eax
        call   sub_2_0120
loc_1_3684:
        push   esi
        call   sub_1_0110
        mov    eax, dword ptr [esi + 0xae]
        push   eax
        call   sub_1_4B48
        xor    edx, edx
        mov    ecx, 0                        ; -> obj1:0x5270
        mov    eax, dword ptr [esi + 0x48]
loc_1_36A0:
        cmp    dword ptr [ecx], eax
        je     loc_1_36B2
        add    ecx, 4
        inc    edx
        cmp    ecx, 8                        ; -> obj1:0x5278
        jbe    loc_1_36A0
        jmp    loc_1_36E1
loc_1_36B2:
        mov    dword ptr [edx*4], 0          ; -> obj1:0x5270
        mov    al, byte ptr [edx]            ; -> obj1:0x527C
        cmp    byte ptr [esi + 0x4c], al
        jne    loc_1_36CF
        mov    byte ptr [edx], 0             ; -> obj1:0x527C
loc_1_36CF:
        mov    al, byte ptr [edx]            ; -> obj1:0x5280
        cmp    byte ptr [esi + 0x4d], al
        jne    loc_1_36E1
        mov    byte ptr [edx], 0             ; -> obj1:0x5280
loc_1_36E1:
        push   0
        push   0x286
        push   esi
        call   sub_1_4C1C
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3700:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [edi + 2], 0
        je     loc_1_372A
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_372A:
        inc    dword ptr [edi + 0x86]
        cmp    byte ptr [edi + 0x126], 0
        jne    loc_1_3835
        cmp    byte ptr [edi + 0x127], 0
        jne    loc_1_3835
        cmp    byte ptr [esi + 0x40], 0
        jne    loc_1_382C
        cmp    byte ptr [edi + 0x128], 0
        jne    loc_1_383C
        mov    byte ptr [edi + 0x127], 1
        mov    eax, edi
        inc    dword ptr [esi + 0x14]
        mov    ebx, dword ptr [edi + 0x8a]
        sub    eax, ebx
        cmp    eax, 0xffffff76
        je     loc_1_37AA
loc_1_377C:
        push   0
        inc    dword ptr [ebx + 0x14]
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   0x40010004
        push   eax
        call   sub_1_4B7E
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_4B84
        mov    eax, edi
        dec    dword ptr [ebx + 0x14]
        mov    ebx, dword ptr [ebx]
        sub    eax, ebx
        cmp    eax, 0xffffff76
        jne    loc_1_377C
loc_1_37AA:
        mov    byte ptr [edi + 0x127], 0
        push   esi
        push   edi
        call   sub_1_3CD0
        push   edi
        call   sub_1_3850
        push   edi
        call   sub_1_3140
        mov    byte ptr [edi + 0x126], 0
        mov    byte ptr [edi + 0x128], 1
        cmp    dword ptr [edi + 0xb6], 0
        je     loc_1_37E1
        push   edi
        call   sub_1_0AB0
loc_1_37E1:
        mov    ebx, dword ptr [edi + 0x8a]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, 0xffffff76
        je     loc_1_3820
loc_1_37F2:
        push   0
        inc    dword ptr [ebx + 0x14]
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   0x40010005
        push   eax
        call   sub_1_4B7E
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_4B84
        mov    eax, edi
        dec    dword ptr [ebx + 0x14]
        mov    ebx, dword ptr [ebx]
        sub    eax, ebx
        cmp    eax, 0xffffff76
        jne    loc_1_37F2
loc_1_3820:
        mov    byte ptr [edi + 0x128], 0
        dec    dword ptr [esi + 0x14]
        jmp    loc_1_383C
loc_1_382C:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_383C
loc_1_3835:
        mov    dword ptr [ebp - 4], 0xc001000d
loc_1_383C:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [edi + 0x86]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3850:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3280
        cmp    dword ptr [esi + 0x10d], 0
        mov    word ptr [esi + 0x46], 0
        je     loc_1_389C
loc_1_386D:
        mov    edx, dword ptr [esi + 0x10d]
        push   0xc001000c
        push   edx
        lea    eax, [edx + 0x24]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x10d], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_4B5A
        dec    dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_386D
loc_1_389C:
        mov    dword ptr [esi + 0x10d], 0
        mov    dword ptr [esi + 0x111], 0
        cmp    dword ptr [esi + 0x119], 0
        je     loc_1_38E8
loc_1_38B9:
        mov    edx, dword ptr [esi + 0x119]
        push   0xc001000c
        push   edx
        lea    eax, [edx + 0x24]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x119], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_4B5A
        dec    dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_38B9
loc_1_38E8:
        mov    dword ptr [esi + 0x119], 0
        mov    dword ptr [esi + 0x11d], 0
        push   esi
        call   sub_1_3910
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3910:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd6]
        mov    ecx, dword ptr [esi + 0xd2]
        cmp    dword ptr [esi + 0x6a], 1
        jne    loc_1_3942
        mov    ebx, dword ptr [esi + 0x48]
        mov    eax, 0x3a
        lea    edx, [ebx + 0x12]
        out    dx, ax
        lea    edx, [ebx + 0x10]
        mov    eax, 0x200
        out    dx, ax
loc_1_3942:
        xor    dl, dl
        cmp    dword ptr [esi + 0xca], 0
        jbe    loc_1_39A5
loc_1_394D:
        mov    al, dl
        add    edi, 8
        and    eax, 0xffff00ff
        inc    dl
        mov    bx, ax
        and    eax, 0xffff
        imul   bx, bx, 0x5ea
        add    bx, cx
        mov    word ptr [edi - 8], bx
        mov    ebx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ebx + eax*4]
        lea    eax, [eax + eax*8]
        lea    ebx, [ebx + eax*4]
        lea    eax, [ecx + ebx*2]
        shr    eax, 0x10
        and    ax, 0xff
        mov    word ptr [edi - 6], ax
        xor    eax, eax
        mov    word ptr [edi - 4], 0xf000
        mov    word ptr [edi - 2], 0
        or     byte ptr [edi - 5], 3
        mov    al, dl
        cmp    eax, dword ptr [esi + 0xca]
        jb     loc_1_394D
loc_1_39A5:
        xor    ecx, ecx
        mov    dword ptr [esi + 0x82], ecx
        cmp    dword ptr [esi + 0xca], ecx
        jbe    loc_1_39D3
        mov    edx, 1
loc_1_39BA:
        xor    eax, eax
        xor    ebx, ebx
        mov    al, cl
        inc    cl
        mov    bl, cl
        mov    byte ptr [eax + esi + 0xdf], dl
        cmp    ebx, dword ptr [esi + 0xca]
        jb     loc_1_39BA
loc_1_39D3:
        mov    eax, dword ptr [esi + 0xfb]
        mov    edi, dword ptr [esi + 0xf7]
        mov    byte ptr [esi + 0xde], 0
        xor    dl, dl
        cmp    dword ptr [esi + 0xef], 0
        jbe    loc_1_3A4E
loc_1_39F1:
        mov    cl, dl
        add    eax, 8
        and    ecx, 0xffff00ff
        inc    dl
        mov    bx, cx
        and    ecx, 0xffff
        imul   bx, bx, 0x5ee
        add    bx, di
        mov    word ptr [eax - 8], bx
        mov    ebx, ecx
        shl    ecx, 6
        sub    ecx, ebx
        lea    ebx, [ebx + ecx*4]
        lea    ecx, [ebx + ebx*2]
        lea    ecx, [edi + ecx*2]
        shr    ecx, 0x10
        and    cx, 0xff
        mov    word ptr [eax - 6], cx
        or     ch, 0x80
        mov    word ptr [eax - 6], cx
        xor    ecx, ecx
        mov    word ptr [eax - 4], 0xfa12
        mov    word ptr [eax - 2], 0
        mov    cl, dl
        cmp    ecx, dword ptr [esi + 0xef]
        jb     loc_1_39F1
loc_1_3A4E:
        mov    word ptr [esi + 0x103], 0
        mov    edi, dword ptr [esi + 0x6a]
        cmp    edi, 1
        je     loc_1_3AA1
        cmp    edi, 3
        je     loc_1_3AA1
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 4
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        in     ax, dx
        mov    bx, ax
        lea    edx, [ecx + 0x12]
        or     bh, 0x20
        mov    eax, 4
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 0x10]
        out    dx, ax
        lea    edx, [ecx + 0x12]
        mov    eax, 0x52
        out    dx, ax
        mov    ax, word ptr [esi + 0x68]
        lea    edx, [ecx + 0x10]
        out    dx, ax
loc_1_3AA1:
        cmp    dword ptr [esi + 0x56], -1
        jne    loc_1_3AB8
        cmp    edi, 2
        je     loc_1_3AB8
        cmp    edi, 4
        je     loc_1_3AB8
        mov    dword ptr [esi + 0x56], 0xc0
loc_1_3AB8:
        mov    ebx, dword ptr [esi + 0x56]
        cmp    ebx, -1
        je     loc_1_3ADF
        cmp    edi, 2
        je     loc_1_3ADF
        cmp    edi, 4
        je     loc_1_3ADF
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 4
        lea    edx, [ecx + 0x12]
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 0x16]
        out    dx, ax
loc_1_3ADF:
        cmp    dword ptr [esi + 0x5a], -1
        jne    loc_1_3AF6
        cmp    edi, 2
        je     loc_1_3AF6
        cmp    edi, 4
        je     loc_1_3AF6
        mov    dword ptr [esi + 0x5a], 0xb0
loc_1_3AF6:
        mov    ebx, dword ptr [esi + 0x5a]
        cmp    ebx, -1
        je     loc_1_3B13
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 5
        lea    edx, [ecx + 0x12]
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 0x16]
        out    dx, ax
loc_1_3B13:
        cmp    dword ptr [esi + 0x5e], -1
        jne    loc_1_3B2A
        cmp    edi, 2
        je     loc_1_3B2A
        cmp    edi, 4
        je     loc_1_3B2A
        mov    dword ptr [esi + 0x5e], 0x4088
loc_1_3B2A:
        mov    ebx, dword ptr [esi + 0x5e]
        cmp    ebx, -1
        je     loc_1_3B47
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 6
        lea    edx, [ecx + 0x12]
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 0x16]
        out    dx, ax
loc_1_3B47:
        cmp    dword ptr [esi + 0x62], -1
        jne    loc_1_3B5E
        cmp    edi, 2
        je     loc_1_3B5E
        cmp    edi, 4
        je     loc_1_3B5E
        mov    dword ptr [esi + 0x62], 0x81
loc_1_3B5E:
        mov    ecx, dword ptr [esi + 0x62]
        cmp    ecx, -1
        je     loc_1_3B7B
        mov    edi, dword ptr [esi + 0x48]
        mov    eax, 7
        lea    edx, [edi + 0x12]
        out    dx, ax
        mov    ax, cx
        lea    edx, [edi + 0x16]
        out    dx, ax
loc_1_3B7B:
        mov    cl, byte ptr [esi]
        cmp    cl, 3
        jne    loc_1_3BCD
        mov    di, word ptr [esi + 0x66]
        cmp    di, -1
        je     loc_1_3BA7
        mov    ebx, dword ptr [esi + 0x48]
        mov    eax, 9
        cmp    di, -1
        lea    edx, [ebx + 0x12]
        out    dx, ax
        mov    ax, di
        lea    edx, [ebx + 0x16]
        out    dx, ax
        jne    loc_1_3BBD
loc_1_3BA7:
        mov    edi, dword ptr [esi + 0x48]
        mov    eax, 9
        lea    edx, [edi + 0x12]
        out    dx, ax
        lea    edx, [edi + 0x16]
        in     ax, dx
        mov    word ptr [esi + 0x66], ax
loc_1_3BBD:
        cmp    word ptr [esi + 0x66], 3
        jne    loc_1_3BEC
        mov    dword ptr [esi + 0x52], 0
        jmp    loc_1_3BEC
loc_1_3BCD:
        mov    bx, word ptr [esi + 0x66]
        cmp    bx, -1
        je     loc_1_3BEC
        mov    edi, dword ptr [esi + 0x48]
        mov    eax, 9
        lea    edx, [edi + 0x12]
        out    dx, ax
        mov    ax, bx
        lea    edx, [edi + 0x16]
        out    dx, ax
loc_1_3BEC:
        cmp    cl, 1
        jne    loc_1_3C22
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 2
        lea    edi, [ecx + 0x12]
        add    ecx, 0x16
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    bx, ax
        mov    edx, edi
        and    bx, 0xfffe
        mov    eax, 2
        out    dx, ax
        or     bx, ax
        mov    edx, ecx
        mov    ax, bx
        out    dx, ax
loc_1_3C22:
        cmp    dword ptr [esi + 0x52], 0
        je     loc_1_3C61
        mov    eax, dword ptr [esi + 0x105]
        or     word ptr [eax], 0x1080
        mov    eax, 2
        mov    ecx, dword ptr [esi + 0x48]
        lea    edi, [ecx + 0x12]
        add    ecx, 0x16
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    bx, ax
        mov    edx, edi
        mov    eax, 2
        out    dx, ax
        mov    ax, bx
        mov    edx, ecx
        and    eax, 0xfffc
        out    dx, ax
loc_1_3C61:
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, 3
        lea    edi, [ecx + 0x12]
        add    ecx, 0x10
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x300
        out    dx, ax
        mov    edx, edi
        mov    eax, 2
        out    dx, ax
        mov    esi, dword ptr [esi + 0x109]
        mov    edx, ecx
        mov    ebx, esi
        shr    ebx, 0x10
        mov    al, bl
        and    eax, 0xffff00ff
        out    dx, ax
        mov    edx, edi
        mov    eax, 1
        out    dx, ax
        mov    ax, si
        mov    edx, ecx
        out    dx, ax
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0xff00
        out    dx, ax
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3CD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3280
        lea    edi, [esi + 0xb6]
        mov    byte ptr [esi + 0x126], 1
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     loc_1_3D3F
        cmp    dword ptr [ecx + 0x10], 4
        jne    loc_1_3D04
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [ecx + 4]
        dec    dword ptr [ecx + 0x14]
        mov    ecx, dword ptr [edi]
loc_1_3D04:
        test   ecx, ecx
        je     loc_1_3D38
loc_1_3D08:
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 4
        je     loc_1_3D30
        cmp    eax, 3
        je     loc_1_3D30
        mov    eax, dword ptr [ecx]
        push   0xc001000c
        push   ecx
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_4B6C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_3D32
loc_1_3D30:
        mov    edi, ecx
loc_1_3D32:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        jne    loc_1_3D08
loc_1_3D38:
        mov    byte ptr [esi + 0x129], 0
loc_1_3D3F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D50:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_524C
        mov    dword ptr [0], 0              ; -> obj1:0x53D4
        cmp    dword ptr [0], 0              ; -> loc_1_5240
        mov    ebp, esp
        push   esi
        push   edi
        je     loc_1_41C9
        dec    eax
        cmp    eax, 5
        ja     loc_1_3D7E
        jmp    dword ptr [eax*4]             ; -> obj1:0x434C
loc_1_3D7E:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4440
        test   al, al
        jne    loc_1_3DAA
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_25C0
        cmp    dword ptr [0], 1              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x12
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3DAA:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4380
        test   al, al
        jne    loc_1_3DC4
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3DC4:
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 9
        in     al, dx
        and    eax, 0xff
        mov    dword ptr [0], eax            ; -> obj1:0x53CC
        cmp    eax, 0x10
        ja     loc_1_3E18
        je     loc_1_3EF6
        test   eax, eax
        je     loc_1_3E28
        cmp    eax, 1
        je     loc_1_3E54
loc_1_3DEC:
        mov    al, byte ptr [0]              ; -> loc_1_5248
        mov    cl, byte ptr [0]              ; -> loc_1_5244
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        push   ecx
        push   edx
        call   sub_1_43B0
        test   al, al
        jne    loc_1_42B2
        mov    eax, 0xd
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3E18:
        cmp    eax, 0x11
        je     loc_1_3F4F
        cmp    eax, 0xff
        jne    loc_1_3DEC
loc_1_3E28:
        mov    dword ptr [0], 4              ; -> obj1:0x53D4
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        jne    loc_1_3E42
        mov    byte ptr [0], 3               ; -> loc_1_5244
loc_1_3E42:
        cmp    byte ptr [0], 0               ; -> loc_1_5248
        jne    loc_1_3DEC
        mov    byte ptr [0], 5               ; -> loc_1_5248
        jmp    loc_1_3DEC
loc_1_3E54:
        mov    esi, dword ptr [0]            ; -> loc_1_5240
        mov    eax, 8
        add    esi, 0x12
        mov    edi, dword ptr [0]            ; -> loc_1_5240
        mov    edx, esi
        add    edi, 0x16
        mov    dword ptr [0], 2              ; -> obj1:0x53D4
        out    dx, ax
        mov    edx, edi
        in     ax, dx
        shr    ax, 4
        and    ax, 0xf
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        je     loc_1_3EAE
        xor    ecx, ecx
        mov    edx, eax
        mov    cl, byte ptr [0]              ; -> loc_1_5244
        and    edx, 0xffff
        cmp    ecx, edx
        je     loc_1_3EAE
        mov    dword ptr [0], 8              ; -> loc_1_523C
        jmp    loc_1_3EB3
loc_1_3EAE:
        mov    byte ptr [0], al              ; -> loc_1_5244
loc_1_3EB3:
        mov    edx, esi
        mov    eax, 8
        out    dx, ax
        mov    edx, edi
        in     ax, dx
        and    ax, 7
        cmp    byte ptr [0], 0               ; -> loc_1_5248
        je     loc_1_3EEC
        xor    ecx, ecx
        mov    edx, eax
        mov    cl, byte ptr [0]              ; -> loc_1_5248
        and    edx, 0xffff
        cmp    ecx, edx
        je     loc_1_3EEC
        mov    eax, 0xb
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3EEC:
        mov    byte ptr [0], al              ; -> loc_1_5248
        jmp    loc_1_3DEC
loc_1_3EF6:
        mov    dword ptr [0], 3              ; -> obj1:0x53D4
        mov    byte ptr [0], 0               ; -> loc_1_5248
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 8
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        je     loc_1_3F44
        xor    eax, eax
        mov    ecx, edx
        mov    al, byte ptr [0]              ; -> loc_1_5244
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_3F44
        mov    dword ptr [0], 8              ; -> loc_1_523C
        jmp    loc_1_3DEC
loc_1_3F44:
        mov    byte ptr [0], dl              ; -> loc_1_5244
        jmp    loc_1_3DEC
loc_1_3F4F:
        cmp    dword ptr [0], 1              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_25C0
        cmp    dword ptr [0], 1              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x12
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3F7D:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_25C0
        cmp    dword ptr [0], 1              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x12
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3F9E:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4440
        test   al, al
        jne    loc_1_3FB8
        mov    eax, 0x14
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3FB8:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4380
        test   al, al
        jne    loc_1_3FD2
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3FD2:
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 9
        in     al, dx
        and    eax, 0xff
        mov    dword ptr [0], eax            ; -> obj1:0x53CC
        cmp    eax, 1
        jne    loc_1_3DEC
        mov    edi, dword ptr [0]            ; -> loc_1_5240
        mov    eax, 8
        add    edi, 0x12
        mov    esi, dword ptr [0]            ; -> loc_1_5240
        mov    edx, edi
        add    esi, 0x16
        mov    dword ptr [0], 2              ; -> obj1:0x53D4
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        shr    ax, 4
        and    ax, 0xf
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        je     loc_1_4049
        xor    ecx, ecx
        mov    edx, eax
        mov    cl, byte ptr [0]              ; -> loc_1_5244
        and    edx, 0xffff
        cmp    ecx, edx
        je     loc_1_4049
        mov    dword ptr [0], 8              ; -> loc_1_523C
        jmp    loc_1_404E
loc_1_4049:
        mov    byte ptr [0], al              ; -> loc_1_5244
loc_1_404E:
        mov    edx, edi
        mov    eax, 8
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        and    ax, 7
        cmp    byte ptr [0], 0               ; -> loc_1_5248
        je     loc_1_4087
        xor    ecx, ecx
        mov    edx, eax
        mov    cl, byte ptr [0]              ; -> loc_1_5248
        and    edx, 0xffff
        cmp    ecx, edx
        je     loc_1_4087
        mov    eax, 0xb
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4087:
        mov    byte ptr [0], al              ; -> loc_1_5248
        jmp    loc_1_3DEC
loc_1_4091:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4440
        test   al, al
        jne    loc_1_40AB
        mov    eax, 0x14
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_40AB:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4380
        test   al, al
        jne    loc_1_40C5
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_40C5:
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 9
        in     al, dx
        and    eax, 0xff
        mov    dword ptr [0], eax            ; -> obj1:0x53CC
        cmp    eax, 0x10
        jne    loc_1_3DEC
        mov    dword ptr [0], 3              ; -> obj1:0x53D4
        mov    byte ptr [0], 0               ; -> loc_1_5248
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 8
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        je     loc_1_4130
        xor    eax, eax
        mov    ecx, edx
        mov    al, byte ptr [0]              ; -> loc_1_5244
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_4130
        mov    dword ptr [0], 8              ; -> loc_1_523C
        jmp    loc_1_3DEC
loc_1_4130:
        mov    byte ptr [0], dl              ; -> loc_1_5244
        jmp    loc_1_3DEC
loc_1_413B:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4440
        test   al, al
        jne    loc_1_4155
        mov    eax, 0x14
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4155:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        push   eax
        call   sub_1_4380
        test   al, al
        jne    loc_1_416F
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_416F:
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, 9
        in     al, dx
        and    eax, 0xff
        mov    dword ptr [0], eax            ; -> obj1:0x53CC
        test   eax, eax
        jne    loc_1_3DEC
        cmp    eax, 0xff
        jne    loc_1_3DEC
        mov    dword ptr [0], 4              ; -> obj1:0x53D4
        cmp    byte ptr [0], 0               ; -> loc_1_5244
        jne    loc_1_41B0
        mov    byte ptr [0], 3               ; -> loc_1_5244
loc_1_41B0:
        cmp    byte ptr [0], 0               ; -> loc_1_5248
        jne    loc_1_3DEC
        mov    byte ptr [0], 5               ; -> loc_1_5248
        jmp    loc_1_3DEC
loc_1_41C9:
        dec    eax
        cmp    eax, 5
        ja     loc_1_41D6
        jmp    dword ptr [eax*4]             ; -> obj1:0x4364
loc_1_41D6:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_25C0
        cmp    dword ptr [0], 1              ; -> obj1:0x53D4
        je     loc_1_3DEC
        call   sub_1_27A0
        cmp    dword ptr [0], 2              ; -> obj1:0x53D4
        je     loc_1_3DEC
        call   sub_1_28A0
        cmp    dword ptr [0], 3              ; -> obj1:0x53D4
        je     loc_1_3DEC
        call   sub_1_2960
        cmp    dword ptr [0], 4              ; -> obj1:0x53D4
        je     loc_1_3DEC
        cmp    dword ptr [0], 0              ; -> obj1:0x53D4
        jne    loc_1_3DEC
        mov    eax, 0x15
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_423A:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_25C0
        cmp    dword ptr [0], 1              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x12
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_425B:
        call   sub_1_27A0
        cmp    dword ptr [0], 2              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x16
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4278:
        call   sub_1_28A0
        cmp    dword ptr [0], 3              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4295:
        call   sub_1_2960
        cmp    dword ptr [0], 4              ; -> obj1:0x53D4
        je     loc_1_3DEC
        mov    eax, 0x18
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_42B2:
        mov    eax, dword ptr [0]            ; -> loc_1_5240
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x48], eax
        mov    dl, byte ptr [0]              ; -> loc_1_5244
        mov    byte ptr [ecx + 0x4c], dl
        mov    al, byte ptr [0]              ; -> loc_1_5248
        mov    byte ptr [ecx + 0x4d], al
        mov    esi, dword ptr [0]            ; -> loc_1_524C
        mov    dword ptr [ecx + 0x4e], esi
        mov    edx, dword ptr [0]            ; -> loc_1_5258
        mov    dword ptr [ecx + 0x52], edx
        mov    eax, dword ptr [0]            ; -> obj1:0x525C
        mov    dword ptr [ecx + 0x56], eax
        mov    esi, dword ptr [0]            ; -> obj1:0x5260
        mov    dword ptr [ecx + 0x5a], esi
        mov    edx, dword ptr [0]            ; -> obj1:0x5264
        mov    dword ptr [ecx + 0x5e], edx
        mov    eax, dword ptr [0]            ; -> obj1:0x5268
        mov    dword ptr [ecx + 0x62], eax
        mov    esi, dword ptr [0]            ; -> obj1:0x53D4
        mov    dword ptr [ecx + 0x6a], esi
        mov    edx, dword ptr [0]            ; -> obj1:0x53CC
        mov    dword ptr [ecx + 0x6e], edx
        mov    ax, word ptr [0]              ; -> obj1:0x5250
        mov    word ptr [ecx + 0x66], ax
        mov    si, word ptr [0]              ; -> loc_1_5254
        mov    word ptr [ecx + 0x68], si
        xor    esi, esi
loc_1_432B:
        mov    edx, dword ptr [0]            ; -> loc_1_5240
        add    edx, esi
        inc    esi
        in     al, dx
        mov    byte ptr [ecx + esi + 2], al
        cmp    esi, 6
        jb     loc_1_432B
        mov    eax, dword ptr [0]            ; -> loc_1_523C
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (12 entries) ----
        dd  loc_1_3F7D
        dd  loc_1_3F9E
        dd  loc_1_4091
        dd  loc_1_413B
        dd  loc_1_3F7D
        dd  loc_1_3F7D
        dd  loc_1_423A
        dd  loc_1_425B
        dd  loc_1_4278
        dd  loc_1_4295
        dd  loc_1_423A
        dd  loc_1_423A
        db  08Dh, 064h, 024h, 000h
sub_1_4380:
        push   ebp
        mov    ecx, 0                        ; -> obj1:0x5270
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
loc_1_438C:
        cmp    dword ptr [ecx], eax
        je     loc_1_43A1
        add    ecx, 4
        cmp    ecx, 8                        ; -> obj1:0x5278
        jbe    loc_1_438C
        mov    al, 1
        pop    ebp
        ret    4
loc_1_43A1:
        xor    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43B0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_524C
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [0]            ; -> obj1:0x53D4
        mov    cl, byte ptr [ebp + 0xc]
        mov    dl, byte ptr [ebp + 0x10]
loc_1_43CB:
        cmp    eax, 4
        je     loc_1_43DF
        cmp    eax, 2
        je     loc_1_43DF
        cmp    edi, 4
        je     loc_1_43DF
        cmp    edi, 2
        jne    loc_1_43E7
loc_1_43DF:
        cmp    byte ptr [esi], cl            ; -> obj1:0x527C
        je     loc_1_440B
loc_1_43E7:
        test   dl, dl
        je     loc_1_43F3
        cmp    byte ptr [esi], dl            ; -> obj1:0x5280
        je     loc_1_4414
loc_1_43F3:
        cmp    dword ptr [esi*4], ebx        ; -> obj1:0x5270
        je     loc_1_441D
        inc    esi
        cmp    esi, 3
        jb     loc_1_43CB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_440B:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_4414:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_441D:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [esi*4], eax        ; -> obj1:0x5270
        mov    byte ptr [esi], cl            ; -> obj1:0x527C
        mov    al, 1
        mov    byte ptr [esi], dl            ; -> obj1:0x5280
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_4440:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
loc_1_444D:
        lea    edx, [ecx + edi]
        inc    edi
        in     al, dx
        and    ax, 0xff
        add    si, ax
        cmp    edi, 0xc
        jb     loc_1_444D
        cmp    ecx, 0x8800
        jne    loc_1_4471
        lea    edx, [ecx + 8]
        in     al, dx
        and    ax, 0xff
        sub    si, ax
loc_1_4471:
        mov    edi, 0xe
loc_1_4476:
        lea    edx, [ecx + edi]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        add    si, dx
        cmp    al, 0x57
        jne    loc_1_44A7
        inc    edi
        cmp    edi, 0x10
        jb     loc_1_4476
        lea    edx, [ecx + 0xc]
        pop    edi
        in     ax, dx
        sub    si, ax
        cmp    si, 1
        pop    esi
        sbb    cl, cl
        pop    ebp
        neg    cl
        mov    al, cl
        ret    4
loc_1_44A7:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_1_44B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        mov    byte ptr [eax + 2], 1
        call   sub_1_3280
        push   0x3d090
        call   sub_1_4C10
        pop    ebp
        ret    4
        db  090h
loc_1_44D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x86]
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_4545
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_453E
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        inc    dword ptr [edi + 0x14]
        test   ecx, ecx
        jne    loc_1_4522
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 0x115]
        add    eax, 0xe
        mov    edx, dword ptr [ebp + 0x14]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_4C8E
        dec    dword ptr [edi + 0x14]
        xor    edi, edi
        jmp    loc_1_454A
loc_1_4522:
        push   0
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, ecx
        push   eax
        call   sub_1_08C0
        dec    dword ptr [edi + 0x14]
        xor    edi, edi
        jmp    loc_1_454A
loc_1_453E:
        mov    edi, 0xc001000c
        jmp    loc_1_454A
loc_1_4545:
        mov    edi, 0xc001000d
loc_1_454A:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_459A
        cmp    dword ptr [esi + 0x86], 2
        jne    loc_1_4562
        cmp    byte ptr [esi + 0x126], 0
        jne    loc_1_4583
loc_1_4562:
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_4583
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_4583
        mov    eax, dword ptr [esi + 0x96]
        sub    eax, esi
        cmp    eax, 0x96
        je     loc_1_459A
loc_1_4583:
        push   0xa
        add    esi, 0x15
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_459A:
        mov    eax, edi
        dec    dword ptr [esi + 0x86]
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_45B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x12], 0
        je     loc_1_45CD
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_45CD:
        mov    edi, dword ptr [esi + 0x10d]
        mov    byte ptr [esi + 0x12], 1
        mov    ecx, dword ptr [edi + 0x28]
        lea    eax, [edi + 0x24]
        mov    dword ptr [ebp - 0xc], ecx
        cmp    dword ptr [eax], 0
        jne    loc_1_45EF
        mov    dword ptr [esi + 0x111], 0
loc_1_45EF:
        mov    eax, dword ptr [eax]
        lea    ecx, [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        mov    dword ptr [esi + 0x10d], eax
        push   0
        mov    dword ptr [esi + 0x115], edi
        push   0
        push   edi
        call   sub_1_4C40
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 0x14]
        push   edx
        push   eax
        call   sub_1_4C64
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_463D
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_4693
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   eax
        push   ecx
        push   ecx
        jmp    loc_1_4685
loc_1_463D:
        cmp    dword ptr [ebp - 4], 0xde
        jae    loc_1_4668
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_4668
        lea    ebx, [esi + 0x1b6]
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   0xd0
        push   0xe
        push   edi
        call   sub_1_07E0
        jmp    loc_1_4672
loc_1_4668:
        sub    dword ptr [ebp - 4], 0xe
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0xe
loc_1_4672:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, 0xe
        mov    edx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        push   ebx
        push   0xe
        push   edx
        push   edx
loc_1_4685:
        push   0
        mov    eax, dword ptr [esi + 0xb2]
        push   eax
        call   sub_1_47C0
loc_1_4693:
        push   0
        mov    ecx, dword ptr [ebp - 0xc]
        push   edi
        mov    eax, dword ptr [ecx + 0x10]
        push   eax
        call   sub_1_4B5A
        cmp    dword ptr [esi + 0x10d], 0
        jne    loc_1_46B7
        mov    eax, dword ptr [esi + 0xb2]
        push   eax
        call   sub_1_49A0
loc_1_46B7:
        mov    ecx, dword ptr [ebp - 0xc]
        pop    edi
        dec    dword ptr [ecx + 0x14]
        mov    byte ptr [esi + 0x12], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_46D0:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [edx + 0x28]
        lea    esi, [edx + 0x24]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10d], 0
        jne    loc_1_46F5
        mov    dword ptr [eax + 0x10d], edx
        jmp    loc_1_46FE
loc_1_46F5:
        mov    ecx, dword ptr [eax + 0x111]
        mov    dword ptr [ecx + 0x24], edx
loc_1_46FE:
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebp
        mov    dword ptr [eax + 0x111], edx
        ret    8
        db  090h
loc_1_4710:
        mov    al, byte ptr [0x4da8]         ; -> obj1:0x4DA8
        cmp    al, 0
        jne    loc_1_4735
        inc    byte ptr [0x4da8]             ; -> obj1:0x4DA8
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_4737
        push   0
        push   0
        call   sub_1_1E80
        cmp    eax, 0
        jne    loc_1_4737
loc_1_4735:
        clc    
        ret    
loc_1_4737:
        stc    
        ret    
PCNTN3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_4748
        jmp    dword ptr [eax*4 + 0x474a]    ; -> obj1:0x474A
loc_1_4748:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_4710
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4748
        dd  loc_1_4710
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_47C0:
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
        call   sub_1_47F0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_47F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_4890
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_4830
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_4825
        cmp    al, 0xff
        jne    loc_1_4825
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_4825
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_4897
loc_1_4825:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_4897
        db  08Bh, 0FFh
loc_1_4830:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_4880
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_4856
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_485D
loc_1_4856:
        mov    dword ptr [ebp - 0x10], 1
loc_1_485D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_4870
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_489A
        db  08Dh, 064h, 024h, 000h
loc_1_4870:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_489A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4880:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_489A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4890:
        mov    dword ptr [ebp - 4], 0x20
loc_1_4897:
        mov    esi, dword ptr [ebp + 8]
loc_1_489A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_4991
loc_1_48A5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_48FD
        test   al, 2
        je     loc_1_48E9
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_1_48E9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_48FD
loc_1_48E9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_48F4
        test   bl, 4
        jne    loc_1_48FD
loc_1_48F4:
        test   bl, 0x20
        je     loc_1_4986
loc_1_48FD:
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
        call   sub_1_4B72
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_4986
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_493F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_493F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_4950
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_4956
        db  08Bh, 0FFh
loc_1_4950:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4956:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_4965
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_4B78
loc_1_4965:
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
        call   sub_1_4C1C
        mov    edi, ebx
        jmp    loc_1_4989
loc_1_4986:
        mov    edi, dword ptr [edi + 0x10]
loc_1_4989:
        or     edi, edi
        jne    loc_1_48A5
loc_1_4991:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_49A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_49B0
        pop    ebp
        ret    4
sub_1_49B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_4A2E
loc_1_49C0:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_4A27
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_4B78
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_4A27
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_49EC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_49EC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_4A00
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_4A06
        db  005h, 000h, 000h, 000h, 000h
loc_1_4A00:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4A06:
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
        call   sub_1_4C1C
        mov    edi, ebx
        jmp    loc_1_4A2A
loc_1_4A27:
        mov    edi, dword ptr [edi + 0x10]
loc_1_4A2A:
        or     edi, edi
        jne    loc_1_49C0
loc_1_4A2E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4A40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        test   al, 0x20
        je     loc_1_4A55
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4A55:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dl, byte ptr [ecx]
        test   dl, 1
        je     loc_1_4A91
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_4A82
        cmp    dl, 0xff
        jne    loc_1_4A82
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_4A82
        test   al, 8
        je     loc_1_4A7B
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4A7B:
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
loc_1_4A82:
        test   al, 4
        jne    loc_1_4A8A
        test   al, 2
        je     loc_1_4AA9
loc_1_4A8A:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4A91:
        mov    eax, dword ptr [esi + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_4AA9
        mov    ax, word ptr [ecx]
        cmp    word ptr [esi + 0x2c], ax
        jne    loc_1_4AA9
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4AA9:
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_4B2A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_4B30:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_4B36:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_4B3C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_4B42:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_4B48:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_4B4E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_4B54:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_4B5A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_4B6C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_4B72:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_4B78:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_4B7E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_4B84:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_4B8A:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_4B9C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_4BA2:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_4BA8:
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
sub_1_4BC6:
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
sub_1_4BCC:
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
sub_1_4BD2:
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
sub_1_4BD8:
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_4BF2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_4BF8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_4BFE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_4C04:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_4C10:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_4C16:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_4C1C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_4C40:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_4C64:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_4C88:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_4C8E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_4C94:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_4C9A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_4CA0:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_4CA6:
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
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
sub_1_4CEE:
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_4CF4:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_4CFC:
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
sub_1_4D1C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_4D3C
        or     ecx, ecx
        je     loc_1_4D52
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_4D50
loc_1_4D3C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_4D50
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_4D50:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_4D52:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PCNTN3  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  PCNTN3_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4DB0:
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
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    al, byte ptr [edx]
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    byte ptr [ebx], al
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x6000202
        add    al, byte ptr [edx]
        add    byte ptr [edi], al
        add    al, byte ptr [edx]
        add    byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx], cl
        add    al, byte ptr [edx]
        add    byte ptr [ebx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], cl
        add    al, byte ptr [eax]
        or     eax, 0xe000202
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
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
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
loc_1_4EE0:
        pop    esp
        inc    esp
        jbe    0x4f4e
        arpl   word ptr [ebp + 0x5c], sp
        push   eax
        inc    ebx
        dec    esi
        push   esp
        dec    esi
        xor    esi, dword ptr [eax]
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al            ; -> loc_1_4EE0
        add    byte ptr [eax], al
        db  000h
loc_1_4EFC:
        push   eax
        inc    ebx
        dec    esi
        push   esp
        dec    esi
        xor    eax, dword ptr [eax]
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al            ; -> loc_1_4EFC
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x64], cl
        imul   esi, dword ptr [ebx + 0x52], 0x73696765
        je     loc_1_4F7C
        jb     0x4f66
        popal  
        arpl   word ptr [eax], sp
        jae    loc_1_4F93
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_1_4F96
        jne    0x4f92
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4F2C:
        inc    edx
        jne    0x4fa2
        push   ebx
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [0x20], bh           ; -> loc_1_4F2C
        add    byte ptr [eax], al
        db  000h
loc_1_4F3C:
        insb   byte ptr es:[edi], dx
        xor    byte ptr fs:[eax], ah
        cmp    eax, 0x20                     ; -> loc_1_4F3C
        add    byte ptr [eax], al
loc_1_4F48:
        insb   byte ptr es:[edi], dx
        xor    dword ptr fs:[eax], esp
        cmp    eax, 0x20                     ; -> loc_1_4F48
        add    byte ptr [eax], al
loc_1_4F54:
        insb   byte ptr es:[edi], dx
        xor    ah, byte ptr fs:[eax]
        cmp    eax, 0x20                     ; -> loc_1_4F54
        add    byte ptr [eax], al
loc_1_4F60:
        insb   byte ptr es:[edi], dx
        xor    esp, dword ptr fs:[eax]
        cmp    eax, 0x20                     ; -> loc_1_4F60
        add    byte ptr [eax], al
loc_1_4F6C:
        je     0x4fde
        and    byte ptr [0x20], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4F78:
        dec    esi
        ja     0x4f9c
loc_1_4F7C:
        inc    ecx
        jb     0x4fe6
        jae    0x4ff6
        and    byte ptr [0x20], bh           ; -> loc_1_4F78
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        and    byte ptr [edx + 0x61], ah
        jae    loc_1_4FF8
loc_1_4F93:
        and    byte ptr [ecx + 0x64], ah
loc_1_4F96:
        jb     loc_1_4FFE
        jae    loc_1_500E
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6c], ah
        jb     0x5008
        popal  
        jns    0x4fc7
        imul   ebp, dword ptr [esi + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_1_501E
        jb     loc_1_4FD9
        jbe    0x5026
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4FC4:
        pop    ecx
        outsd  dx, dword ptr [esi]
        jne    0x4fe8
        arpl   word ptr [ecx + 0x6e], sp
        daa    
        je     0x4fee
        jae    0x5040
        arpl   word ptr gs:[ecx + 0x66], bp
        jns    0x4ff6
        dec    ecx
        dec    edi
        inc    edx
loc_1_4FD9:
        popal  
        jae    loc_1_5041
        and    byte ptr [ecx + 0x64], ah
        jb     0x5047
        jae    loc_1_5057
        and    byte ptr [esi + 0x6f], ah
        jb     0x5009
        push   eax
        inc    ebx
        dec    ecx
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0 ; -> loc_1_4FC4
        db  000h
loc_1_4FF8:
        dec    ecx
        push   edx
        push   ecx
        and    byte ptr [esi + 0x75], ch
loc_1_4FFE:
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [esi + 0x6f], ah
        jne    0x5075
        and    byte ptr fs:[edi + ebp*2 + 0x65], ah
        jae    0x507c
loc_1_500E:
        daa    
        je     0x5031
        insd   dword ptr es:[edi], dx
        popal  
        je     0x5078
        push   0x4f525020
        push   esp
        dec    edi
        inc    ebx
        dec    edi
loc_1_501E:
        dec    esp
        dec    ecx
        dec    esi
        dec    ecx
        add    byte ptr [eax], al            ; -> loc_1_4FF8
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x41], al
        and    byte ptr [esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [esi + 0x6f], ah
        jne    0x50a5
        and    byte ptr fs:[edi + ebp*2 + 0x65], ah
        jae    0x50ac
        daa    
        je     0x5061
loc_1_5041:
        insd   dword ptr es:[edi], dx
        popal  
        je     0x50a8
        push   0x6f727020
        je     0x50bb
        arpl   word ptr [edi + 0x6c], bp
        imul   ebp, dword ptr cs:[esi + 0x69], 0x20
loc_1_5057:
        add    byte ptr [eax], al            ; -> obj1:0x5028
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        and    byte ptr [ecx + 0x6e], ah
        das    
        outsd  dx, dword ptr [esi]
        jb     loc_1_5087
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6c], ah
        jb     0x50de
        popal  
        jns    loc_1_509D
        imul   ebp, dword ptr [esi + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
loc_1_5087:
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x50f4
        jb     0x50af
        jbe    loc_1_50FC
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_509C:
        dec    ecx
loc_1_509D:
        dec    edi
        and    byte ptr [edx + 0x61], ah
        jae    0x5108
        and    byte ptr [ecx + 0x64], ah
        jb     loc_1_510E
        jae    0x511e
        and    byte ptr [0x20], bh           ; -> loc_1_509C
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     0x511d
        jb     0x512c
        jne    0x512c
        je     0x50de
        outsb  dx, byte ptr [esi]
        jne    0x512e
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [0], bh              ; -> obj1:0x50B4
        add    byte ptr [eax], al
loc_1_50CC:
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [ebx + 0x68], ah
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [0], bh              ; -> loc_1_50CC
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        and    byte ptr [ebx + 0x63], dh
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x4350202c
        dec    ecx
        and    byte ptr [edx + 0x75], ah
        jae    0x511c
loc_1_50FC:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x5120
        outsw  dx, word ptr [esi]
        jne    0x5172
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al            ; -> obj1:0x50E0
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
loc_1_510E:
        dec    ecx
        and    byte ptr [ebx + 0x63], dh
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x6564202c
        jbe    0x518d
        arpl   word ptr [ebp + 0x20], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_1_514B
        outsw  dx, word ptr [esi]
        jne    loc_1_519D
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5138:
        dec    esp
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x4f], sp
        jo     0x51a5
        outsb  dx, byte ptr [esi]
        inc    ecx
        popal  
        jo     loc_1_51BA
        jb     0x5169
        popaw  
loc_1_514B:
        imul   ebp, dword ptr [ebp + 0x64], 0 ; -> loc_1_5138
        add    byte ptr [eax], ah
        inc    esp
        jbe    0x51c2
        arpl   word ptr [ebp + 0x20], sp
        popal  
        je     loc_1_517F
        jae    0x51d1
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x204f4920
        bound  esp, qword ptr [ecx + 0x73]
        and    byte ptr gs:[ecx + 0x64], ah
        jb     loc_1_51DB
        jae    loc_1_51EB
        and    byte ptr [esi + 0x6f], ch
        je     loc_1_519D
        outsw  dx, word ptr [esi]
loc_1_517F:
        jne    0x51ef
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5188:
        and    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x746f6e20
        and    byte ptr [esi + 0x6f], ah
        jne    loc_1_5206
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al            ; -> loc_1_5188
loc_1_519D:
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        xor    bh, byte ptr [edx]
        and    byte ptr [eax + 0x6e], dl
        push   eax
        and    byte ptr [ebx + 0x63], dh
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x6564202c
loc_1_51BA:
        jbe    loc_1_5225
        arpl   word ptr [ebp + 0x20], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x51e3
        outsw  dx, word ptr [esi]
        jne    0x5235
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_51D0:
        xor    dh, byte ptr [ebx]
        cmp    ah, byte ptr [eax]
        push   esi
        inc    ebp
        push   ebx
        inc    ecx
        and    byte ptr [ebx + 0x63], dh
loc_1_51DB:
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x6564202c
loc_1_51EB:
        jbe    loc_1_5256
        arpl   word ptr [ebp + 0x20], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x5214
        outsw  dx, word ptr [esi]
        jne    0x5266
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al            ; -> loc_1_51D0
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        xor    al, 0x3a
        and    byte ptr [ecx + 0x53], cl
loc_1_5206:
        inc    ecx
        and    byte ptr [ebx + 0x63], dh
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x6564202c
        jbe    loc_1_5285
        arpl   word ptr [ebp + 0x20], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x5243
        outsw  dx, word ptr [esi]
loc_1_5225:
        jne    loc_1_5295
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_523C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5240:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5244:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5248:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_524C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h
loc_1_5254:
        cmp    al, 0
loc_1_5256:
        add    byte ptr [eax], al
loc_1_5258:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5285:
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x45], cl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
loc_1_5295:
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    ecx
        jb     loc_1_530C
        jae    loc_1_531C
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        jb     loc_1_5318
        jae    loc_1_5328
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     loc_1_5321
        jb     0x5330
        jne    0x5330
        je     loc_1_52C2
loc_1_52C2:
        add    byte ptr [eax], al
loc_1_52C4:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     loc_1_532D
        jb     0x533c
        jne    0x533c
        je     loc_1_52CE
loc_1_52CE:
        add    byte ptr [eax], al
        inc    esp
        insd   dword ptr es:[edi], dx
        popal  
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_52DC:
        inc    esp
        insd   dword ptr es:[edi], dx
        popal  
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        inc    edx
        jne    0x535e
        push   esp
        jns    0x535e
        add    byte ptr gs:[edx + 0x75], al
        jae    0x5348
        jns    0x5366
        add    byte ptr gs:[edx + 0x55], al
        push   ebx
        pop    edi
        push   esp
        dec    edi
        pop    edi
        push   ebx
        inc    ebx
        inc    ecx
        dec    esi
        add    byte ptr [edx + 0x55], al
        push   ebx
        pop    edi
        push   esp
        dec    edi
        pop    edi
        push   ebx
loc_1_530C:
        inc    ebx
        inc    ecx
        dec    esi
        add    byte ptr [esi + 0x44], al
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5318:
        inc    esi
        inc    esp
        push   ebp
        push   eax
loc_1_531C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esp
loc_1_5321:
        push   eax
        add    byte ptr [eax], al
loc_1_5324:
        push   esp
        push   eax
        add    byte ptr [eax], al
loc_1_5328:
        dec    esp
        inc    ebp
        inc    esp
        xor    byte ptr [eax], al
loc_1_532D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x44], cl
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], al
        push   ebx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5374:
        inc    edx
        push   ebp
        push   ebx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    ebx
        dec    ecx
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x4e], dl
        push   eax
        add    byte ptr [eax + 0x4e], dl
        push   eax
        add    byte ptr [esi + 0x45], dl
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_53B8:
        push   esi
        inc    ebp
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [ecx + 0x53], cl
        inc    ecx
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x53c8]       ; -> obj1:0x53C8
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_4C16
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_004E
        mov    esi, 2
loc_2_004E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_4C16
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_0082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_4C16
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_00B1
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_00B1:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x18]
        add    ecx, 0x2c
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dl, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], dl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ecx + 5], al
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
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0140
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_4C1C
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_4C1C
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_4C1C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_0189
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0189:
        mov    eax, dword ptr [0x53c8]       ; -> obj1:0x53C8
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C16
        or     eax, eax
        je     loc_2_01A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_01A9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_01B0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_01C3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_01B0
        jmp    loc_2_01D2
loc_2_01C3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_01D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_01E8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_01E8:
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
sub_2_0240:
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
        call   sub_2_05E0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0269
        cmp    ebx, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    ebx, eax
loc_2_0280:
        or     ebx, ebx
        je     loc_2_028C
        cmp    ebx, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_4B78
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_4C1C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    ebx, 0xc001000e
loc_2_02EB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0300:
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
        call   sub_1_4D1C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_4D1C
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_0384
        sub    eax, eax
        nop    
loc_2_0360:
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
        ja     loc_2_0360
loc_2_0384:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_03EF
loc_2_0391:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_03E4
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
        call   sub_1_4D1C
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
        call   sub_1_4D1C
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_03E7
loc_2_03E4:
        inc    dword ptr [ebp - 4]
loc_2_03E7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_0391
loc_2_03EF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_04DA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_0403:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_2_043A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_04C8
loc_2_043A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_0514
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
        call   sub_1_4D1C
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
        call   sub_1_4D1C
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
loc_2_04C8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_0565
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_0552
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_0552
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0559
loc_2_0514:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4D1C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_4D1C
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0552:
        mov    dword ptr [ebp - 8], 1
loc_2_0559:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_04F0
loc_2_0565:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_05D0
loc_2_056B:
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
        je     loc_2_05D2
        cmp    edi, 0x103
        je     loc_2_05D2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4D1C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4D1C
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_05D0:
        sub    edi, edi
loc_2_05D2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_05E0:
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
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_0651
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
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0651:
        sub    eax, eax
loc_2_0653:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_0660:
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
        je     loc_2_0692
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_0687:
        test   dword ptr [esi], edx
        je     loc_2_068C
        inc    eax
loc_2_068C:
        add    esi, 4
        dec    edi
        jne    loc_2_0687
loc_2_0692:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_06A0:
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
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_071C
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    edi, edi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + edi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        call   sub_1_4D1C
        add    ebx, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    edi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_06E0
loc_2_071C:
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
loc_2_0736:
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
sub_2_0760:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_0790
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0790:
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
        call   sub_1_4D1C
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_07C0:
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
        je     loc_2_0859
        dec    esi
        cmp    esi, edx
        jb     loc_2_0859
        nop    
loc_2_07E0:
        cmp    ecx, edx
        ja     loc_2_0859
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_0803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0803:
        jbe    loc_2_0810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0837
        db  08Bh, 0FFh
loc_2_0810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_0827
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0827:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_0837
        mov    dword ptr [ebp - 4], 0
loc_2_0837:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0869
        jle    loc_2_0848
        or     edx, edx
        je     loc_2_0859
        lea    esi, [edx - 1]
        jmp    loc_2_084B
loc_2_0848:
        lea    ecx, [edx + 1]
loc_2_084B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_07E0
loc_2_0859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0869:
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

VxD__LDATA SEGMENT   ; object 4  vsize=0x3d  RW-
        db  041h, 04Dh, 044h, 020h, 041h, 06Dh, 032h, 031h, 030h, 030h, 02Fh, 050h, 043h, 04Eh, 045h, 054h
        db  02Dh, 049h, 053h, 041h, 020h, 061h, 064h, 061h, 070h, 074h, 065h, 072h, 000h, 000h, 000h, 000h
        db  041h, 04Dh, 044h, 020h, 041h, 06Dh, 032h, 031h, 030h, 030h, 02Fh, 050h, 043h, 04Eh, 045h, 054h
        db  02Dh, 049h, 053h, 041h, 020h, 061h, 064h, 061h, 070h, 074h, 065h, 072h, 038h
VxD__LDATA ENDS
