; ELNKII -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 6805 instructions recovered by recursive descent

Declare_Virtual_Device ELNKII, 3, 10, ELNKII_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2124  R-X
sub_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        lea    eax, [esi + 8]
        push   0x4d0                         ; -> sub_1_04D0
        push   eax
        call   sub_1_1CCA
        mov    ecx, dword ptr [esi + 0x50]
        sub    edi, edi
        lea    eax, [ecx + 0xb]
        mov    dx, ax
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0xa]
        out    dx, al
        mov    ebx, 0x80
loc_1_0032:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        in     al, dx
        test   bl, al
        jne    loc_1_004D
        push   0x3e8
        call   sub_1_1CD0
        inc    edi
        cmp    edi, 4
        jb     loc_1_0032
loc_1_004D:
        mov    ecx, dword ptr [esi + 0x50]
        mov    al, 2
        lea    edx, [ecx + 0xd]
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0070:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 0xc]
        cmp    byte ptr [esi + 0x4a], 0
        push   0
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        je     loc_1_00F7
        call   sub_1_1D00
        movsx  edi, word ptr [ebp + 0x10]
        shl    edi, 9
        lea    edi, [edi + edi*2]
        add    edi, dword ptr [esi + 0x86]
        sub    esi, esi
        cmp    dword ptr [ebp - 0xc], esi
        je     loc_1_00E7
        sub    ebx, ebx
        mov    edi, edi
loc_1_00B0:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_1DBC
        add    edi, dword ptr [ebp - 0x10]
        add    esi, dword ptr [ebp - 0x10]
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_00B0
loc_1_00E7:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_00F7:
        call   sub_1_1D00
        movsx  eax, word ptr [ebp + 0x10]
        shl    eax, 9
        mov    dword ptr [ebp - 0x18], 0
        lea    eax, [eax + eax*2 + 0x2000]
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_0234
        mov    edi, edi
loc_1_0120:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1D24
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0140
        jmp    loc_1_021D
        db  08Dh, 064h, 024h, 000h
loc_1_0140:
        mov    edi, dword ptr [esi + 0x54]
        mov    ecx, dword ptr [ebp - 0x14]
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 0x95]
        or     al, 0xc0
        out    dx, al
        mov    edi, dword ptr [ebp - 0x10]
        cmp    edi, 0x10
        jb     loc_1_01BB
        mov    ebx, 0x60
loc_1_016B:
        test   byte ptr [ebp - 8], 1
        je     loc_1_0185
        push   0x10
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1B70
        jmp    loc_1_0197
loc_1_0185:
        push   8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1B84
loc_1_0197:
        sub    edi, 0x10
        add    dword ptr [ebp - 8], 0x10
        mov    ecx, 0x80
loc_1_01A3:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   bl, al
        jne    loc_1_0247
        test   cl, al
        je     loc_1_01A3
        cmp    edi, 0x10
        jae    loc_1_016B
loc_1_01BB:
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1B70
        mov    ecx, 0x60
        mov    ebx, 0x80
loc_1_01D6:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    loc_1_0269
        test   bl, al
        je     loc_1_01D6
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 6
        mov    al, byte ptr [esi + 0x95]
        out    dx, al
        mov    ecx, 8
        add    eax, 0
loc_1_0200:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    loc_1_0200
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    ecx, dword ptr [ebp - 0x18]
        add    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x18], ecx
loc_1_021D:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_0120
loc_1_0234:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0247:
        push   2
        push   2
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0269:
        push   2
        push   2
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movsx  eax, word ptr [ebp + 0x10]
        cmp    byte ptr [esi + 0x4a], 0
        je     loc_1_02D0
        shl    eax, 9
        mov    edi, dword ptr [ebp + 0x18]
        lea    eax, [eax + eax*2]
        add    eax, dword ptr [esi + 0x86]
        add    eax, dword ptr [ebp + 0x14]
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   eax
        call   sub_1_1DBC
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_02D0:
        shl    eax, 9
        mov    edi, dword ptr [esi + 0x54]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0x14]
        lea    ecx, [eax + ecx + 0x2000]
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 0x95]
        or     al, 0xc0
        out    dx, al
        mov    edi, dword ptr [ebp + 0x18]
        or     edi, edi
        je     loc_1_0343
loc_1_0302:
        mov    ebx, edi
        cmp    ebx, 0x10
        jb     loc_1_030E
        mov    ebx, 0x10
loc_1_030E:
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1B70
        sub    edi, ebx
        add    dword ptr [ebp + 0xc], ebx
        mov    ecx, 0x60
        mov    ebx, 0x80
        mov    edi, edi
loc_1_0330:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    loc_1_036B
        test   bl, al
        je     loc_1_0330
        or     edi, edi
        jne    loc_1_0302
loc_1_0343:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 6
        mov    al, byte ptr [esi + 0x95]
        out    dx, al
        mov    ecx, 8
loc_1_0355:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    loc_1_0355
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_036B:
        push   2
        push   3
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        jne    loc_1_03B0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_1_03B0:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x4a], 0
        je     loc_1_03D2
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_1DBC
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_03D2:
        mov    eax, 0x2000
        sub    eax, dword ptr [esi + 0x86]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [esi + 0x54]
        add    ecx, eax
        lea    edx, [ebx + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [ebx + 0xa]
        mov    al, cl
        out    dx, al
        lea    edx, [ebx + 6]
        mov    al, byte ptr [esi + 0x95]
        or     al, 0x80
        out    dx, al
        cmp    edi, 0x10
        mov    ebx, dword ptr [ebp + 0xc]
        jb     loc_1_0468
loc_1_0405:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   al, 0x60
        jne    loc_1_0446
        test   al, 0x80
        je     loc_1_0405
        test   bl, 1
        je     loc_1_042A
        push   0x10
        push   ebx
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1BAD
        jmp    loc_1_0439
loc_1_042A:
        push   8
        push   ebx
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1BC1
loc_1_0439:
        sub    edi, 0x10
        add    ebx, 0x10
        cmp    edi, 0x10
        jae    loc_1_0405
        jmp    loc_1_0468
loc_1_0446:
        push   2
        push   4
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0468:
        mov    ecx, 0x60
        lea    ecx, [ecx]
loc_1_0470:
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    loc_1_04A8
        test   al, 0x80
        je     loc_1_0470
        push   edi
        push   ebx
        mov    eax, dword ptr [esi + 0x54]
        add    eax, 0xe
        push   eax
        call   sub_1_1BAD
        mov    ecx, dword ptr [esi + 0x54]
        add    ecx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x95]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_04A8:
        push   2
        push   4
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_04D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 0x50]
        mov    al, 0x21
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], 6
        mov    ax, word ptr [esi + 0x6a]
        movsx  ecx, ax
        mov    edi, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + ecx*4 + 0x7e]
        lea    edx, [edi + 4]
        mov    bl, byte ptr [esi + 0x92]
        imul   byte ptr [ebp - 4]
        add    bl, al
        mov    al, bl
        out    dx, al
        cmp    ecx, 0x3c
        jae    loc_1_0529
        mov    ecx, 0x3c
loc_1_0529:
        lea    edx, [edi + 6]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 5]
        mov    al, cl
        out    dx, al
        mov    dx, di
        mov    al, 0x26
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0550:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx + 0x50]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ecx + 0x98], al
        sub    al, al
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0570:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 0x62
        mov    edx, dword ptr [ecx + 0x50]
        out    dx, al
        add    edx, 7
        in     al, dx
        mov    byte ptr [ecx + 0x97], al
        mov    dx, word ptr [ecx + 0x50]
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x96]
        cmp    byte ptr [ecx + 0x93], al
        mov    edx, dword ptr [ecx + 0x50]
        jne    loc_1_05C7
        add    edx, 3
        mov    al, byte ptr [ecx + 0x94]
        dec    al
        out    dx, al
        pop    ebp
        ret    4
loc_1_05C7:
        add    edx, 3
        dec    al
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        in     al, dx
        test   al, 4
        je     loc_1_05FB
        push   esi
        call   sub_1_0610
loc_1_05FB:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        mov    al, 0x10
        out    dx, al
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_0610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xd
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [esi + 0x50]
        add    dword ptr [esi + 0x1bf], ecx
        add    edx, 0xe
        in     al, dx
        xor    ecx, ecx
        mov    cl, al
        mov    edx, dword ptr [esi + 0x50]
        add    dword ptr [esi + 0x1c3], ecx
        add    edx, 0xf
        in     al, dx
        and    eax, 0xff
        add    dword ptr [esi + 0x1c7], eax
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_04D0
        push   0x7d0
        call   sub_1_1CD0
        mov    ecx, dword ptr [esi + 0x50]
        sub    al, al
        lea    esi, [ecx + 0xb]
        mov    dx, si
        out    dx, al
        lea    edx, [ecx + 0xa]
        out    dx, al
        lea    edx, [ecx + 0xd]
        mov    al, 2
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x4b], 0
        je     loc_1_06B2
        sub    al, al
        pop    ebp
        ret    4
loc_1_06B2:
        mov    edx, dword ptr [eax + 0x50]
        add    edx, 0xf
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x30]
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, al
        je     loc_1_0773
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        out    dx, al
        test   al, 0x20
        je     loc_1_0702
        mov    eax, 8
        jmp    loc_1_0778
loc_1_0702:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_0715
        push   esi
        call   sub_1_0610
        mov    eax, 4
        jmp    loc_1_0778
loc_1_0715:
        test   byte ptr [ebp - 1], 1
        je     loc_1_0747
        cmp    dword ptr [esi + 0x34], 0xa
        jae    loc_1_0728
        mov    eax, 1
        jmp    loc_1_0778
loc_1_0728:
        mov    dword ptr [esi + 0x34], 0
        test   byte ptr [ebp - 1], 0xa
        je     loc_1_0740
        mov    eax, 2
        jmp    loc_1_0778
        db  08Dh, 064h, 024h, 000h
loc_1_0740:
        mov    eax, 1
        jmp    loc_1_0778
loc_1_0747:
        mov    dword ptr [esi + 0x34], 0
        test   byte ptr [ebp - 1], 0xa
        je     loc_1_0760
        mov    eax, 2
        jmp    loc_1_0778
        db  005h, 000h, 000h, 000h, 000h
loc_1_0760:
        test   byte ptr [ebp - 1], 4
        je     loc_1_0773
        push   esi
        call   sub_1_0610
        mov    eax, 1
        jmp    loc_1_0778
loc_1_0773:
        mov    eax, 0x10
loc_1_0778:
        sub    ebx, ebx
loc_1_077A:
        cmp    eax, 0x10
        jne    loc_1_07A3
        cmp    dword ptr [esi + 0x1ea], ebx
        je     loc_1_08DA
        cmp    byte ptr [esi + 0x1aa], bl
        jne    loc_1_08DA
        cmp    byte ptr [esi + 0x1cb], bl
        jne    loc_1_08DA
loc_1_07A3:
        cmp    eax, 1
        je     loc_1_07FD
        cmp    eax, 2
        je     loc_1_07B9
        cmp    eax, 4
        je     loc_1_07E0
        cmp    eax, 8
        je     loc_1_0811
        jmp    loc_1_0824
loc_1_07B9:
        push   esi
        call   sub_1_0550
        mov    byte ptr [esi + 0x22e], bl
        cmp    byte ptr [esi + 0x5c], bl
        jne    loc_1_07D3
        mov    byte ptr [esi + 0x6c], bl
        push   esi
        call   sub_1_0B70
loc_1_07D3:
        and    byte ptr [ebp - 1], 0xf5
        jmp    loc_1_0824
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07E0:
        mov    byte ptr [esi + 0x1a9], 1
        cmp    byte ptr [esi + 0x6c], bl
        je     loc_1_07F9
        test   byte ptr [ebp - 1], 0xa
        jne    loc_1_07F9
        or     byte ptr [ebp - 1], 2
        mov    byte ptr [esi + 0x6d], bl
loc_1_07F9:
        and    byte ptr [ebp - 1], 0xef
loc_1_07FD:
        cmp    byte ptr [esi + 0x1aa], bl
        jne    loc_1_0820
        push   esi
        call   sub_1_09D0
        cmp    al, bl
        je     loc_1_0824
        jmp    loc_1_0820
loc_1_0811:
        push   esi
        call   sub_1_0610
        and    byte ptr [ebp - 1], 0xdf
        jmp    loc_1_0824
        db  08Dh, 049h, 000h
loc_1_0820:
        and    byte ptr [ebp - 1], 0xfa
loc_1_0824:
        cmp    dword ptr [esi + 0x1ea], ebx
        je     loc_1_0842
        cmp    byte ptr [esi + 0x1aa], bl
        jne    loc_1_0842
        cmp    byte ptr [esi + 0x1cb], bl
        jne    loc_1_0842
        push   esi
        call   sub_1_09D0
loc_1_0842:
        test   byte ptr [ebp - 1], 0x20
        je     loc_1_0852
        mov    eax, 8
        jmp    loc_1_077A
loc_1_0852:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_0868
        push   esi
        call   sub_1_0610
        mov    eax, 4
        jmp    loc_1_077A
loc_1_0868:
        test   byte ptr [ebp - 1], 1
        je     loc_1_08A0
        cmp    dword ptr [esi + 0x34], 0xa
        jae    loc_1_0880
        mov    eax, 1
        jmp    loc_1_077A
        db  08Bh, 0FFh
loc_1_0880:
        mov    dword ptr [esi + 0x34], ebx
        test   byte ptr [ebp - 1], 0xa
        je     loc_1_0893
        mov    eax, 2
        jmp    loc_1_077A
loc_1_0893:
        mov    eax, 1
        jmp    loc_1_077A
        db  08Dh, 049h, 000h
loc_1_08A0:
        mov    dword ptr [esi + 0x34], ebx
        test   byte ptr [ebp - 1], 0xa
        je     loc_1_08B3
        mov    eax, 2
        jmp    loc_1_077A
loc_1_08B3:
        test   byte ptr [ebp - 1], 4
        je     loc_1_08D0
        push   esi
        call   sub_1_0610
        mov    eax, 1
        jmp    loc_1_077A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08D0:
        mov    eax, 0x10
        jmp    loc_1_077A
loc_1_08DA:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        je     loc_1_08EF
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        out    dx, al
loc_1_08EF:
        test   byte ptr [ebp - 1], 0x20
        je     loc_1_0900
        mov    eax, 8
        jmp    loc_1_0957
        db  08Dh, 064h, 024h, 000h
loc_1_0900:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_0913
        push   esi
        call   sub_1_0610
        mov    eax, 4
        jmp    loc_1_0957
loc_1_0913:
        test   byte ptr [ebp - 1], 1
        je     loc_1_0931
        cmp    dword ptr [esi + 0x34], 0xa
        jae    loc_1_0921
        jmp    loc_1_0952
loc_1_0921:
        mov    dword ptr [esi + 0x34], ebx
        test   byte ptr [ebp - 1], 0xa
        je     loc_1_0952
        mov    eax, 2
        jmp    loc_1_0957
loc_1_0931:
        mov    dword ptr [esi + 0x34], ebx
        test   byte ptr [ebp - 1], 0xa
        je     loc_1_0941
        mov    eax, 2
        jmp    loc_1_0957
loc_1_0941:
        test   byte ptr [ebp - 1], 4
        mov    eax, 0x10
        je     loc_1_0957
        push   esi
        call   sub_1_0610
loc_1_0952:
        mov    eax, 1
loc_1_0957:
        cmp    eax, 0x10
        jne    loc_1_07A3
        mov    byte ptr [esi + 0x1e5], 0x1b
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xf
        mov    al, 0x1b
        out    dx, al
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_09A7
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_0991
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_1_0991
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_09A7
loc_1_0991:
        push   1
        add    esi, 0x202
        push   esi
        call   sub_1_1CB2
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_09A7:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_1_09BE
        movsx  eax, word ptr [esi + 0x66]
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_1_09BE
        push   esi
        call   sub_1_1400
loc_1_09BE:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        mov    byte ptr [esi + 0x1aa], 1
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_0A08
        cmp    byte ptr [esi + 0x1a9], 0
        je     loc_1_0A08
        push   esi
        push   0x660                         ; -> loc_1_0660
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_1CCA
loc_1_0A08:
        push   esi
        call   sub_1_0570
        mov    edi, edi
loc_1_0A10:
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_0A6A
        mov    al, byte ptr [esi + 0x97]
        cmp    byte ptr [esi + 0x96], al
        je     loc_1_0B26
        mov    dword ptr [esi + 0x1f2], 0
        inc    dword ptr [esi + 0x34]
        push   esi
        call   sub_1_0F30
        cmp    eax, 3
        je     loc_1_0B43
        mov    al, byte ptr [esi + 0x9a]
        mov    byte ptr [esi + 0x96], al
        push   esi
        call   sub_1_05A0
        cmp    dword ptr [esi + 0x34], 0xa
        jbe    loc_1_0A10
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [esi + 0x34], 0
loc_1_0A6A:
        cmp    byte ptr [esi + 0x1a9], 0
        je     loc_1_0AB4
        mov    byte ptr [esi + 0x1a9], 0
        push   esi
        push   0x5e0                         ; -> loc_1_05E0
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_1CCA
        mov    eax, dword ptr [esi + 0x50]
        add    eax, 0xd
        mov    dx, ax
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x6d], al
        je     loc_1_0AB4
        cmp    word ptr [esi + 0x6a], -1
        je     loc_1_0AB4
        mov    byte ptr [esi + 0x6d], al
        mov    byte ptr [esi + 0x22e], al
        mov    byte ptr [esi + 0x6c], 1
        push   esi
        call   sub_1_04F0
loc_1_0AB4:
        cmp    dword ptr [esi + 0x1ea], 0
        je     loc_1_0B0A
        lea    ecx, [ecx]
loc_1_0AC0:
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_0B0A
        mov    ebx, dword ptr [esi + 0x1ea]
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x1ea], eax
        mov    dword ptr [esi + 0x1f2], ebx
        mov    eax, ebx
        inc    dword ptr [esi + 0x1ab]
        mov    edi, dword ptr [ebx + 0x28]
        push   eax
        push   esi
        call   sub_1_0D90
        push   0
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x1ea], 0
        jne    loc_1_0AC0
loc_1_0B0A:
        mov    byte ptr [esi + 0x1aa], 0
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_1_0B55
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0B26:
        push   esi
        call   sub_1_0570
        mov    al, byte ptr [esi + 0x97]
        cmp    byte ptr [esi + 0x96], al
        jne    loc_1_0A10
        jmp    loc_1_0A6A
loc_1_0B43:
        mov    byte ptr [esi + 0x1e5], 0x1a
        push   esi
        call   sub_2_2160
        jmp    loc_1_0A6A
loc_1_0B55:
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_1_1A60
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_0B70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x22e], 0
        mov    byte ptr [esi + 0x5c], 1
        mov    ax, word ptr [esi + 0x6a]
        cmp    ax, 0xffff
        jne    loc_1_0B99
        mov    byte ptr [esi + 0x5c], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0B99:
        movsx  eax, ax
        mov    edx, dword ptr [esi + eax*4 + 0x76]
        cmp    byte ptr [edx + 0x2c], 0
        lea    eax, [edx + 0x24]
        jne    loc_1_0BF0
        test   byte ptr [esi + 0x98], 1
        je     loc_1_0BC0
        inc    dword ptr [esi + 0x1ab]
        jmp    loc_1_0BC6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0BC0:
        inc    dword ptr [esi + 0x1b3]
loc_1_0BC6:
        mov    edi, dword ptr [eax + 4]
        mov    al, byte ptr [esi + 0x98]
        and    al, 1
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [edi + 0x1c]
        jmp    loc_1_0C16
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BF0:
        cmp    dword ptr [esi + 0x1ea], 0
        jne    loc_1_0C01
        mov    dword ptr [esi + 0x1ea], edx
        jmp    loc_1_0C0A
loc_1_0C01:
        mov    ecx, dword ptr [esi + 0x1ee]
        mov    dword ptr [ecx + 0x24], edx
loc_1_0C0A:
        mov    dword ptr [esi + 0x1ee], edx
        mov    dword ptr [eax], 0
loc_1_0C16:
        movsx  eax, word ptr [esi + 0x6a]
        mov    dword ptr [esi + eax*4 + 0x76], 0
        movsx  eax, word ptr [esi + 0x6a]
        mov    dword ptr [esi + eax*4 + 0x6e], 0
        sub    edx, edx
        movsx  eax, word ptr [esi + 0x6a]
        inc    eax
        div    dword ptr [esi + 0x38]
        mov    di, dx
        movsx  eax, di
        lea    ebx, [esi + eax*4]
        mov    ecx, dword ptr [ebx + 0x6e]
        or     ecx, ecx
        je     loc_1_0C65
        cmp    ecx, 1
        je     loc_1_0D16
        cmp    ecx, 2
        je     loc_1_0D31
        mov    byte ptr [esi + 0x5c], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0C65:
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_1_0C87
        mov    word ptr [esi + 0x6a], 0xffff
        mov    byte ptr [esi + 0x5c], 0
        push   2
        push   esi
        call   sub_2_1890
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0C87:
        mov    ecx, dword ptr [esi + 0x5e]
        or     ecx, ecx
        je     loc_1_0D01
        mov    edx, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x5e], edx
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 0x38]
        mov    word ptr [esi + 0x66], dx
        mov    dword ptr [ebx + 0x6e], 2
        mov    dword ptr [ebx + 0x76], ecx
        add    ebx, 0x7e
        mov    word ptr [esi + 0x6a], di
        push   ebx
        push   edi
        push   ecx
        push   esi
        call   sub_1_0070
        mov    eax, dword ptr [ebx]
        cmp    eax, 0x3c
        jae    loc_1_0CD4
        mov    ecx, 0x3c
        sub    ecx, eax
        push   ecx
        push   eax
        push   edi
        push   0x208c                        ; -> loc_1_208C
        push   esi
        call   sub_1_0290
loc_1_0CD4:
        mov    byte ptr [esi + 0x5c], 0
        cmp    byte ptr [esi + 0x1a9], 0
        je     loc_1_0CF0
        mov    byte ptr [esi + 0x6d], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0CF0:
        mov    byte ptr [esi + 0x6c], 1
        push   esi
        call   sub_1_04F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D01:
        mov    word ptr [esi + 0x6a], 0xffff
        mov    word ptr [esi + 0x68], di
        mov    byte ptr [esi + 0x5c], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D16:
        mov    word ptr [esi + 0x6a], 0xffff
        mov    word ptr [esi + 0x68], di
        mov    byte ptr [esi + 0x5c], 0
        push   esi
        call   sub_1_1400
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D31:
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_1_0D60
        mov    word ptr [esi + 0x6a], 0xffff
        mov    dword ptr [ebx + 0x6e], 0
        mov    byte ptr [esi + 0x5c], 0
        push   2
        push   esi
        call   sub_2_1890
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0D60:
        mov    word ptr [esi + 0x6a], di
        mov    byte ptr [esi + 0x5c], 0
        cmp    byte ptr [esi + 0x1a9], 0
        je     loc_1_0D77
        mov    byte ptr [esi + 0x6d], 1
        jmp    loc_1_0D81
loc_1_0D77:
        mov    byte ptr [esi + 0x6c], 1
        push   esi
        call   sub_1_04F0
loc_1_0D81:
        push   esi
        call   sub_1_1400
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_1D00
        cmp    dword ptr [ebp - 4], 6
        jae    loc_1_0DC0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DC0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 0x1e6]
        cmp    edi, eax
        jb     loc_1_0DD2
        mov    edi, eax
loc_1_0DD2:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0x9d]
        push   eax
        call   sub_1_0E50
        cmp    edi, 0xe
        jae    loc_1_0E13
        cmp    edi, 5
        jbe    loc_1_0E3C
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0x9d]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_1_1880
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0E13:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        sub    edi, 0xe
        push   edi
        lea    eax, [esi + 0xab]
        push   eax
        push   0xe
        lea    eax, [esi + 0x9d]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_1_1880
loc_1_0E3C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        sub    edi, edi
        push   eax
        call   sub_1_1D00
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 0x10]
        je     loc_1_0EA4
loc_1_0E75:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_0EA4
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0E75
loc_1_0EA4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0EB5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0EB5:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0xc], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_0EC4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_0ED3
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_0ED6
loc_1_0ED3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_0ED6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_1DBC
        add    esi, ebx
        cmp    esi, edi
        je     loc_1_0F12
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0F1D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        jmp    loc_1_0EC4
loc_1_0F12:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0F1D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0F30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x93]
        mov    al, byte ptr [esi + 0x96]
        sub    eax, ecx
        shl    eax, 8
        add    eax, dword ptr [esi + 0x8a]
        mov    dword ptr [ebp - 8], eax
        push   4
        push   eax
        lea    eax, [esi + 0x99]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        jne    loc_1_0F80
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0F80:
        mov    bl, byte ptr [esi + 0x9c]
        mov    al, byte ptr [esi + 0x96]
        lea    eax, [ebx + eax + 1]
        mov    dl, byte ptr [esi + 0x94]
        cmp    dl, al
        ja     loc_1_0FA2
        sub    al, dl
        add    al, byte ptr [esi + 0x93]
loc_1_0FA2:
        mov    cl, byte ptr [esi + 0x9a]
        cmp    cl, al
        je     loc_1_0FE4
        inc    al
        cmp    dl, al
        jne    loc_1_0FB8
        mov    al, byte ptr [esi + 0x93]
loc_1_0FB8:
        cmp    cl, al
        je     loc_1_0FE4
        cmp    byte ptr [esi + 0x93], cl
        ja     loc_1_0FD6
        cmp    dl, cl
        jbe    loc_1_0FD6
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FD6:
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FE4:
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x9b]
        xor    eax, eax
        mov    al, bl
        shl    eax, 8
        lea    edi, [ecx + eax - 4]
        mov    dword ptr [esi + 0x199], edi
        cmp    byte ptr [esi + 0x4a], 0
        jne    loc_1_1058
        mov    ebx, dword ptr [esi + 0x1e6]
        cmp    ebx, edi
        jb     loc_1_100F
        mov    ebx, edi
loc_1_100F:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [esi + 0x9d]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        jne    loc_1_1050
        push   1
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x10
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1050:
        lea    eax, [esi + 0x9d]
        jmp    loc_1_106D
loc_1_1058:
        mov    ebx, edi
        cmp    ebx, 0xfc
        jb     loc_1_1067
        mov    ebx, 0xfc
loc_1_1067:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
loc_1_106D:
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [esi + 0x1af]
        cmp    ebx, 0xe
        jae    loc_1_10A0
        push   0
        push   0
        push   0
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_1_1880
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_10A0:
        sub    edi, 0xe
        push   edi
        sub    ebx, 0xe
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe
        push   eax
        push   0xe
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_1_1880
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x1f2], 0
        je     loc_1_1158
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebx, edi
        push   eax
        call   sub_1_1D00
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1146
loc_1_1106:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x1f2]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_0E50
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_1146
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1106
loc_1_1146:
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    ebx, edi
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1158:
        mov    eax, dword ptr [esi + 0x199]
        mov    ecx, dword ptr [ebp + 0x14]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    loc_1_1170
        sub    eax, edi
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_1_1173
        db  090h
loc_1_1170:
        mov    dword ptr [ebp - 0x1c], ecx
loc_1_1173:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [esi + 0x8a]
        xor    eax, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x93]
        mov    al, byte ptr [esi + 0x96]
        sub    eax, edx
        shl    eax, 8
        add    eax, ecx
        lea    edi, [edi + eax + 4]
        mov    eax, dword ptr [esi + 0x8e]
        cmp    edi, eax
        jbe    loc_1_11A8
        sub    edi, eax
        add    edi, ecx
loc_1_11A8:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_1D00
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    dword ptr [ebp - 0xc], 0
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_1267
loc_1_11E0:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0x10], ebx
        jae    loc_1_11EE
        mov    ebx, dword ptr [ebp - 0x10]
loc_1_11EE:
        mov    eax, dword ptr [esi + 0x8e]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    loc_1_11FF
        mov    ebx, eax
        sub    ebx, edi
loc_1_11FF:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_1_1280
        add    edi, ebx
        sub    dword ptr [ebp - 0x10], ebx
        je     loc_1_1267
        cmp    dword ptr [esi + 0x8e], edi
        jne    loc_1_1227
        mov    edi, dword ptr [esi + 0x8a]
loc_1_1227:
        add    dword ptr [ebp - 0xc], ebx
        mov    eax, dword ptr [ebp - 0xc]
        cmp    eax, dword ptr [ebp - 8]
        jne    loc_1_125D
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1267
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    dword ptr [ebp - 0xc], 0
loc_1_125D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_11E0
loc_1_1267:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
loc_1_1280:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], eax
        push   2
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x10
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_1_12B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [eax + 8]
        lea    ebx, [edi + 0x24]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_12E0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12E0:
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_1_12F7
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12F7:
        mov    eax, dword ptr [ebp + 8]
        push   0
        inc    dword ptr [eax + 0x1c]
        inc    dword ptr [esi + 0x30]
        mov    dword ptr [ebx + 4], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebx], 0
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_1D00
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x71], 2
        je     loc_1_1339
        mov    byte ptr [ebx + 8], 0
        jmp    loc_1_134C
loc_1_1339:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_1_1B00
        mov    byte ptr [ebx + 8], al
loc_1_134C:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_1_135A
        mov    eax, dword ptr [esi + 0x62]
        mov    dword ptr [eax + 0x24], edi
        jmp    loc_1_135D
loc_1_135A:
        mov    dword ptr [esi + 0x5e], edi
loc_1_135D:
        mov    dword ptr [esi + 0x62], edi
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_1397
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_1387
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_1_1387
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_1397
loc_1_1387:
        push   1
        add    esi, 0x202
        push   esi
        call   sub_1_1CB2
        jmp    loc_1_13AE
loc_1_1397:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_1_13AE
        movsx  eax, word ptr [esi + 0x66]
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_1_13AE
        push   esi
        call   sub_1_1400
loc_1_13AE:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    esi, esi
        mov    edx, dword ptr [ebp + 0x10]
        cmp    edx, esi
        je     loc_1_13E1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_13D4:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    loc_1_13E9
        inc    esi
        cmp    esi, edx
        jb     loc_1_13D4
loc_1_13E1:
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_13E9:
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1400:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x66]
        mov    word ptr [ebp - 2], ax
        movsx  eax, ax
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_1_150E
loc_1_1422:
        mov    edi, dword ptr [esi + 0x5e]
        or     edi, edi
        je     loc_1_150E
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x5e], eax
        movsx  ebx, word ptr [ebp - 2]
        mov    dword ptr [esi + ebx*4 + 0x6e], 1
        lea    eax, [ebx + 1]
        mov    dword ptr [esi + ebx*4 + 0x76], edi
        sub    edx, edx
        div    dword ptr [esi + 0x38]
        mov    word ptr [esi + 0x66], dx
        lea    eax, [esi + ebx*4 + 0x7e]
        push   eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   edi
        push   esi
        call   sub_1_0070
        mov    eax, dword ptr [esi + ebx*4 + 0x7e]
        cmp    eax, 0x3c
        jae    loc_1_1482
        mov    ecx, 0x3c
        sub    ecx, eax
        push   ecx
        push   eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   0x208c                        ; -> loc_1_208C
        push   esi
        call   sub_1_0290
loc_1_1482:
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_14E0
        mov    dword ptr [esi + ebx*4 + 0x6e], 2
        cmp    word ptr [esi + 0x6a], -1
        jne    loc_1_14C1
        mov    ax, word ptr [esi + 0x68]
        cmp    ax, word ptr [ebp - 2]
        jne    loc_1_14C1
        mov    word ptr [esi + 0x6a], ax
        cmp    byte ptr [esi + 0x1a9], 0
        je     loc_1_14B7
        mov    byte ptr [esi + 0x6d], 1
        jmp    loc_1_14C1
loc_1_14B7:
        mov    byte ptr [esi + 0x6c], 1
        push   esi
        call   sub_1_04F0
loc_1_14C1:
        mov    ax, word ptr [esi + 0x66]
        mov    word ptr [ebp - 2], ax
        movsx  eax, ax
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        je     loc_1_1422
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_14E0:
        mov    eax, dword ptr [edi + 0x28]
        mov    dword ptr [ebp - 8], eax
        movsx  eax, word ptr [ebp - 2]
        mov    dword ptr [esi + eax*4 + 0x6e], 0
        push   0
        mov    eax, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1C1A
        push   3
        push   esi
        call   sub_2_1890
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x1c]
loc_1_150E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1520:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x22e], 0
        je     loc_1_1620
        cmp    word ptr [esi + 0x6a], -1
        je     loc_1_162E
        mov    byte ptr [esi + 0x6c], 0
        mov    byte ptr [esi + 0x22e], 0
        mov    al, byte ptr [esi + 0x5d]
        cmp    al, 0xa
        jae    loc_1_156A
        push   3
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x10
        jmp    loc_1_156F
loc_1_156A:
        inc    al
        mov    byte ptr [esi + 0x5d], al
loc_1_156F:
        push   esi
        call   0
        movsx  eax, word ptr [esi + 0x6a]
        lea    eax, [esi + eax*4 + 0x76]
        sub    edx, edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax], 0
        movsx  eax, word ptr [esi + 0x6a]
        mov    dword ptr [esi + eax*4 + 0x6e], 0
        movsx  eax, word ptr [esi + 0x6a]
        inc    eax
        div    dword ptr [esi + 0x38]
        mov    di, dx
        or     ecx, ecx
        je     loc_1_15DD
loc_1_15A2:
        mov    ebx, dword ptr [ecx + 0x28]
        push   0
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C1A
        movsx  eax, di
        lea    edx, [esi + eax*4]
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [edx + 0x6e], 2
        jne    loc_1_15DD
        mov    ecx, dword ptr [edx + 0x76]
        mov    dword ptr [edx + 0x76], 0
        mov    dword ptr [edx + 0x6e], 0
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 0x38]
        mov    di, dx
        or     ecx, ecx
        jne    loc_1_15A2
loc_1_15DD:
        mov    word ptr [esi + 0x6a], 0xffff
        mov    word ptr [esi + 0x68], di
        mov    ecx, dword ptr [esi + 0x5e]
        mov    dword ptr [esi + 0x5e], 0
        or     ecx, ecx
        je     loc_1_1612
loc_1_15F5:
        mov    edi, dword ptr [ecx + 0x24]
        mov    ebx, dword ptr [ecx + 0x28]
        lea    eax, [ecx + 0x24]
        push   0
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [ebx + 0x1c]
        mov    ecx, edi
        or     edi, edi
        jne    loc_1_15F5
loc_1_1612:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xd
        sub    al, al
        out    dx, al
        jmp    loc_1_162E
        db  08Dh, 049h, 000h
loc_1_1620:
        cmp    word ptr [esi + 0x6a], -1
        je     loc_1_162E
        mov    byte ptr [esi + 0x22e], 1
loc_1_162E:
        push   0x7d0
        add    esi, 0x222
        push   esi
        call   sub_1_1CB2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1650:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_1706
        cmp    dword ptr [esi + 0x1fe], 0
        jne    loc_1_1706
loc_1_1676:
        mov    edi, dword ptr [esi + 0x1f6]
        or     edi, edi
        je     loc_1_16E1
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x1f6], eax
        cmp    dword ptr [esi + 0x1fa], edi
        jne    loc_1_169A
        mov    dword ptr [esi + 0x1fa], 0
loc_1_169A:
        mov    dword ptr [esi + 0x1fe], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_16B5
        cmp    eax, 0xa
        je     loc_1_16B5
        push   esi
        call   sub_2_23D0
        jmp    loc_1_16BB
loc_1_16B5:
        push   esi
        call   sub_2_2460
loc_1_16BB:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_16D5
        cmp    ecx, 4
        je     loc_1_16D5
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C2C
loc_1_16D5:
        dec    dword ptr [ebx + 0x1c]
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_1_1676
loc_1_16E1:
        mov    dword ptr [esi + 0x1fe], 0
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_1_1706
        dec    dword ptr [esi + 0x30]
        push   1
        push   esi
        call   sub_2_1890
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_1706:
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_1771
        sub    ebx, ebx
        nop    
loc_1_1710:
        cmp    dword ptr [edi + 0x1c], 0
        jbe    loc_1_1720
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_175B
        db  08Dh, 049h, 000h
loc_1_1720:
        mov    eax, dword ptr [esi + 0x1e6]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_1731
        push   esi
        call   sub_2_09E0
loc_1_1731:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C14
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0xc]
        lea    ecx, [edi + 0xc]
        je     loc_1_174A
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_174D
loc_1_174A:
        mov    dword ptr [esi + 0x2c], eax
loc_1_174D:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   0x75
        push   eax
        call   sub_1_1CDC
loc_1_175B:
        or     edi, edi
        jne    loc_1_1710
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_1_1771
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_1771
        push   esi
        call   0
loc_1_1771:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_17A7
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_1_1792
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_1_1792
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_17A7
loc_1_1792:
        push   1
        add    esi, 0x202
        push   esi
        call   sub_1_1CB2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_17A7:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_1_17BE
        movsx  eax, word ptr [esi + 0x66]
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_1_17BE
        push   esi
        call   sub_1_1400
loc_1_17BE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17D0:
        mov    al, byte ptr [0x2118]         ; -> obj1:0x2118
        cmp    al, 0
        jne    loc_1_17F5
        inc    byte ptr [0x2118]             ; -> obj1:0x2118
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_17F7
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_17F7
loc_1_17F5:
        clc    
        ret    
loc_1_17F7:
        stc    
        ret    
ELNKII_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_1808
        jmp    dword ptr [eax*4 + 0x180a]    ; -> obj1:0x180A
loc_1_1808:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_17D0
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_1808
        dd  loc_1_17D0
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1880:
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
        call   sub_1_18B0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_18B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_1950
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_18F0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_18E5
        cmp    al, 0xff
        jne    loc_1_18E5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_18E5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_1957
loc_1_18E5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_1957
        db  08Bh, 0FFh
loc_1_18F0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_1940
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1916
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_191D
loc_1_1916:
        mov    dword ptr [ebp - 0x10], 1
loc_1_191D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1930
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_195A
        db  08Dh, 064h, 024h, 000h
loc_1_1930:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_195A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1940:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_195A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1950:
        mov    dword ptr [ebp - 4], 0x20
loc_1_1957:
        mov    esi, dword ptr [ebp + 8]
loc_1_195A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1A51
loc_1_1965:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_19BD
        test   al, 2
        je     loc_1_19A9
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2C50
        or     al, al
        je     loc_1_19A9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_19BD
loc_1_19A9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_19B4
        test   bl, 4
        jne    loc_1_19BD
loc_1_19B4:
        test   bl, 0x20
        je     loc_1_1A46
loc_1_19BD:
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
        call   sub_1_1C32
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1A46
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_19FF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_19FF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1A10
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1A16
        db  08Bh, 0FFh
loc_1_1A10:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1A16:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1A25
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1C38
loc_1_1A25:
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
        call   sub_1_1CDC
        mov    edi, ebx
        jmp    loc_1_1A49
loc_1_1A46:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1A49:
        or     edi, edi
        jne    loc_1_1965
loc_1_1A51:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1A60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A70
        pop    ebp
        ret    4
sub_1_1A70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1AEE
loc_1_1A80:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1AE7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1C38
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1AE7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1AAC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1AAC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1AC0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1AC6
        db  005h, 000h, 000h, 000h, 000h
loc_1_1AC0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1AC6:
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
        call   sub_1_1CDC
        mov    edi, ebx
        jmp    loc_1_1AEA
loc_1_1AE7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1AEA:
        or     edi, edi
        jne    loc_1_1A80
loc_1_1AEE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B00:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_1B15
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B15:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_1B51
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_1B42
        cmp    bl, 0xff
        jne    loc_1_1B42
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_1B42
        test   al, 8
        je     loc_1_1B3B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B3B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B42:
        test   al, 4
        jne    loc_1_1B4A
        test   al, 2
        je     loc_1_1B69
loc_1_1B4A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B51:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1B69
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_1B69
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B69:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_1B70:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
sub_1_1B84:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_1BAD:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_1BC1:
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
sub_1_1BEA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_1BF0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_1BF6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_1BFC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_1C02:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_1C08:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_1C14:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_1C1A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_1C26:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_1C2C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_1C32:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_1C38:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_1C3E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_1C44:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_1C5C:
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
sub_1_1CB2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_1CB8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_1CBE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_1CC4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_1CCA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_1CD0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_1CD6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_1CDC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_1D00:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_1D24:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_1D48:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_1D54:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_1D5A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_1D60:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_1D66:
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
sub_1_1DA2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_1DA8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_1DBC:
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
sub_1_1DDC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_1DFC
        or     ecx, ecx
        je     loc_1_1E12
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_1E10
loc_1_1DFC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_1E10
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_1E10:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_1E12:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1E6C:
        loopne loc_1_1E70
        add    byte ptr [eax], al
loc_1_1E70:
        mov    al, byte ptr [0x80000002]
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], dl
        add    byte ptr [eax], al
        push   eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dh
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 0x6b6e6c45]
        imul   ebp, dword ptr [ecx], 0x6e6c4500
        imul   ebp, dword ptr [ecx + 0x69], 0
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
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        dec    esp
        dec    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        dec    esp
        dec    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
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
        dec    esi
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
        dec    ebp
        inc    ebp
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        dec    ebp
        inc    ecx
        push   eax
        push   eax
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ebp
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        dec    ebp
        inc    ecx
        push   eax
        push   eax
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        db  000h
loc_1_1F3C:
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebp + 0x58], al
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1F54:
        inc    ebp
        pop    eax
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1F60:
        inc    ebp
        je     loc_1_1FCB
        jb     0x1fd2
        imul   ebp, dword ptr [esi + 0x6b], 0x20494920
        inc    ecx
        popal  
        jo     loc_1_1FE6
        jb     loc_1_1FA3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1FA3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1FCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_1FD8:
        loopne loc_1_1FDC
        add    byte ptr [eax], al
loc_1_1FDC:
        mov    al, byte ptr [0x80000002]
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], dl
loc_1_1FE6:
        add    byte ptr [eax], al
        push   eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dh
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [eax + eax], 0
        ror    byte ptr [eax + eax], 0
        or     byte ptr [0xdc00000], 0
loc_1_2008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2014:
        add    al, byte ptr [esi + eax]
        db  000h
loc_1_2018:
        inc    eax
        inc    eax
        add    byte ptr [eax], al
loc_1_201C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2020:
        inc    eax
        add    byte ptr [eax], al
        xchg   edx, eax
loc_1_2024:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2028:
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
loc_1_202C:
        add    dword ptr [edx], eax
        add    dword ptr [eax], eax
loc_1_2030:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2036:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_203C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_206C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2074:
        add    byte ptr [eax], cl
        add    al, 0xc
        add    cl, byte ptr [edx]
        push   es
        push   cs
        add    dword ptr [ecx], ecx
        add    eax, 0x70b030d
        ud0    
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_208C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ELNKII  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  ELNKII_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2d10  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    word ptr [ebp - 0x10], 7
        mov    word ptr [ebp - 0xe], 7
        mov    dword ptr [ebp - 0xc], 0x1e98 ; -> obj1:0x1E98
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_1_1BEA
        mov    dword ptr [0x1e62], esi       ; -> obj1:0x1E62
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x1e20], eax       ; -> obj1:0x1E20
        mov    byte ptr [0x1e66], 0          ; -> obj1:0x1E66
        mov    dword ptr [0x1e5a], 0         ; -> obj1:0x1E5A
        mov    byte ptr [0x1e24], 3          ; -> obj1:0x1E24
        mov    byte ptr [0x1e25], 0          ; -> obj1:0x1E25
        mov    dword ptr [0x1e2a], 0x820     ; -> obj1:0x1E2A
        mov    dword ptr [0x1e2e], 0xa20     ; -> obj1:0x1E2E
        mov    dword ptr [0x1e32], 0x12b0    ; -> obj1:0x1E32
        mov    dword ptr [0x1e36], 0x10d0    ; -> obj1:0x1E36
        mov    dword ptr [0x1e3a], 0x1580    ; -> obj1:0x1E3A
        mov    dword ptr [0x1e3e], 0xb80     ; -> obj1:0x1E3E
        mov    dword ptr [0x1e42], 0x13f0    ; -> obj1:0x1E42
        mov    dword ptr [0x1e46], 0x14c0    ; -> obj1:0x1E46
        mov    dword ptr [0x1e4a], 0x120     ; -> obj1:0x1E4A
        mov    dword ptr [0x1e4e], 0x14f0    ; -> obj1:0x1E4E
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0x1e52], eax       ; -> obj1:0x1E52
        mov    dword ptr [0x1e56], ecx       ; -> obj1:0x1E56
        push   0x36
        push   0x1e24                        ; -> obj1:0x1E24
        push   0x1e1c                        ; -> obj1:0x1E1C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   0x1e1c                        ; -> obj1:0x1E1C
        push   eax
        call   sub_1_1BF6
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0108
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1BF0
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0108:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x58
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x40], 0xa
        mov    word ptr [ebp - 0x3e], 0xa
        mov    dword ptr [ebp - 0x3c], 0x1eac ; -> obj1:0x1EAC
        mov    word ptr [ebp - 0x38], 0xa
        mov    word ptr [ebp - 0x36], 0xa
        mov    dword ptr [ebp - 0x34], 0x1ec4 ; -> obj1:0x1EC4
        mov    word ptr [ebp - 0x30], 0xc
        mov    word ptr [ebp - 0x2e], 0xc
        mov    esi, 0x300
        mov    dword ptr [ebp - 0x2c], 0x1f3c ; -> loc_1_1F3C
        mov    word ptr [ebp - 0x28], 9
        mov    word ptr [ebp - 0x26], 9
        mov    dword ptr [ebp - 0x24], 0x1f54 ; -> loc_1_1F54
        mov    bl, 3
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [ebp - 0x18], 0
        mov    byte ptr [ebp - 2], 0
        mov    eax, dword ptr [0x1e18]       ; -> obj1:0x1E18
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x230
        push   eax
        call   sub_1_1CD6
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_2_01B1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_01B1:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0x8c
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1D54
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_01F2
        push   0
        push   0x230
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1CDC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_01F2:
        push   1
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1D5A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0215
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [eax + 4]
loc_2_0215:
        sub    al, al
loc_2_0217:
        xor    ecx, ecx
        mov    cl, al
        cmp    dword ptr [ecx*4 + 0x1e6c], esi ; -> loc_1_1E6C
        je     loc_2_022A
        inc    al
        cmp    al, 8
        jb     loc_2_0217
loc_2_022A:
        cmp    al, 8
        jne    loc_2_023A
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 0x18], esi
        jmp    loc_2_02F9
loc_2_023A:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1D5A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_025D
        mov    eax, dword ptr [ebp - 0x14]
        mov    bl, byte ptr [eax + 4]
loc_2_025D:
        sub    al, al
        nop    
loc_2_0260:
        xor    ecx, ecx
        mov    cl, al
        cmp    byte ptr [ecx + 0x1e8c], bl   ; -> obj1:0x1E8C
        je     loc_2_0272
        inc    al
        cmp    al, 4
        jb     loc_2_0260
loc_2_0272:
        cmp    al, 4
        jne    loc_2_0283
        xor    eax, eax
        mov    al, bl
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_2_02F9
loc_2_0283:
        push   2
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1D5A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_02BA
        push   1
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 4
        push   eax
        call   sub_1_1D66
        or     al, al
        je     loc_2_02BA
        mov    byte ptr [ebp - 2], 1
loc_2_02BA:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1C5C
        cmp    dword ptr [ebp - 0x20], 6
        jne    loc_2_02F9
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_02F9
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, edx
        add    ecx, 0x19d
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_2_02F9:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1D60
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x38], 2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x3c], esi
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax + 0x49], cl
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x48], bl
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x4a], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x44], 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x4c], 0x10
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0380
        or     eax, eax
        je     loc_2_0374
        push   0
        push   0x230
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1CDC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0374:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
sub_2_0380:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x38], 2
        jbe    loc_2_03A5
        mov    dword ptr [ebp - 8], 0xc000009a
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_03A5:
        mov    dword ptr [ebx + 0x28], 0
        mov    dword ptr [ebx + 0x2c], 0
        mov    eax, dword ptr [0x1e18]       ; -> obj1:0x1E18
        push   eax
        lea    eax, [ebp - 0xc]
        push   0
        push   0x32
        push   eax
        call   sub_1_1CD6
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_2_03D6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_03D6:
        mov    edi, dword ptr [ebp - 0xc]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0xa], 1
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x16], 2
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebx + 0x3c]
        mov    dword ptr [eax + 0x1a], ecx
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x1e], 0x10
        mov    ecx, dword ptr [ebp - 0xc]
        lea    eax, [ebx + 0x50]
        mov    dword ptr [ecx + 0x22], eax
        mov    eax, dword ptr [ebx + 0x3c]
        mov    ecx, dword ptr [ebp - 0xc]
        add    eax, 0x400
        mov    dword ptr [ecx + 0x26], eax
        mov    eax, dword ptr [ebp - 0xc]
        lea    edi, [ebx + 4]
        mov    dword ptr [eax + 0x2a], 0x10
        mov    ecx, dword ptr [ebp - 0xc]
        lea    eax, [ebx + 0x54]
        mov    dword ptr [ecx + 0x2e], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ebx
        mov    eax, dword ptr [0x1e1c]       ; -> obj1:0x1E1C
        push   eax
        push   edi
        call   sub_1_1C02
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        push   0
        push   0x32
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        je     loc_2_0471
        call   sub_1_1CDC
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0471:
        call   sub_1_1CDC
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_04A0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        push   1
        push   0xaa00000a
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x10
        jmp    loc_2_0796
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_04A0:
        mov    dword ptr [ebx + 0x1f6], 0
        mov    dword ptr [ebx + 0x1fa], 0
        mov    dword ptr [ebx + 0x1fe], 0
        mov    dword ptr [ebx + 0x20e], 0x1650 ; -> loc_1_1650
        mov    dword ptr [ebx + 0x30], 0
        mov    dword ptr [ebx + 0x202], 0
        mov    eax, dword ptr [ebx + 0x20e]
        mov    dword ptr [ebx + 0x206], eax
        lea    eax, [ebp - 1]
        mov    dword ptr [ebx + 0x20a], ebx
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   ebx
        call   sub_2_1A90
        mov    dword ptr [ebx + 0x40], eax
        cmp    byte ptr [ebp - 2], 0
        jne    loc_2_0520
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    dword ptr [ebp - 8], 0xc0010006
        jmp    loc_2_0796
loc_2_0520:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_2_0544
        push   0
        push   0xaa00000d
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    dword ptr [ebp - 8], 0xc0010006
        jmp    loc_2_0796
loc_2_0544:
        cmp    byte ptr [ebx + 0x4a], 0
        je     loc_2_0593
        cmp    dword ptr [ebx + 0x40], 0
        jne    loc_2_0554
        mov    byte ptr [ebx + 0x4a], 0
loc_2_0554:
        cmp    byte ptr [ebx + 0x4a], 0
        je     loc_2_0593
        push   0x2000
        mov    eax, dword ptr [ebx + 0x40]
        push   eax
        mov    eax, dword ptr [edi]
        lea    esi, [ebx + 0x86]
        push   eax
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1DA2
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_059F
        push   0
        push   0xaa000001
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        jmp    loc_2_0796
loc_2_0593:
        lea    esi, [ebx + 0x86]
        mov    dword ptr [esi], 0x2000
loc_2_059F:
        mov    eax, dword ptr [ebx + 0x38]
        shl    eax, 9
        mov    byte ptr [ebx + 0x92], 0x20
        lea    eax, [eax + eax*2]
        add    eax, dword ptr [esi]
        mov    dword ptr [ebx + 0x8a], eax
        mov    cl, 6
        mov    al, byte ptr [ebx + 0x38]
        imul   cl
        mov    cl, byte ptr [ebx + 0x92]
        add    cl, al
        mov    byte ptr [ebx + 0x93], cl
        mov    eax, dword ptr [esi]
        add    eax, 0x2000
        mov    dword ptr [ebx + 0x8e], eax
        mov    al, byte ptr [ebx + 0x92]
        add    al, 0x20
        mov    byte ptr [ebx + 0x94], al
        mov    byte ptr [ebx + 0x1e4], 0
        mov    byte ptr [ebx + 0x1aa], 0
        mov    word ptr [ebx + 0x6a], 0xffff
        sub    ecx, ecx
        mov    byte ptr [ebx + 0x6c], 0
        mov    byte ptr [ebx + 0x1a9], 0
        mov    byte ptr [ebx + 0x6d], 0
        cmp    dword ptr [ebx + 0x38], ecx
        jbe    loc_2_0623
        lea    eax, [ebx + 0x6e]
loc_2_0614:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [ebx + 0x38], ecx
        ja     loc_2_0614
loc_2_0623:
        mov    byte ptr [ebx + 0x1cb], 0
        mov    byte ptr [ebx + 0x6c], 0
        mov    byte ptr [ebx + 0x22e], 0
        mov    byte ptr [ebx + 0x5c], 0
        push   ebx
        call   sub_2_1B60
        lea    eax, [ebx + 0x1d4]
        push   eax
        push   ebx
        lea    eax, [ebx + 0x19d]
        push   eax
        push   0x1a80                        ; -> loc_2_1A80
        push   0x1a00                        ; -> loc_2_1A00
        push   0x1980                        ; -> loc_2_1980
        mov    eax, dword ptr [ebx + 0x4c]
        push   eax
        call   sub_2_2490
        or     al, al
        jne    loc_2_0688
        push   0
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_2_078D
loc_2_0688:
        mov    byte ptr [ebx + 0x1e5], 0x1b
        mov    dword ptr [ebx + 0x20], 0x1e1c ; -> obj1:0x1E1C
        mov    eax, dword ptr [0x1e5a]       ; -> obj1:0x1E5A
        mov    dword ptr [ebx + 0x24], eax
        lea    esi, [ebx + 8]
        mov    dword ptr [0x1e5a], ebx       ; -> obj1:0x1E5A
        push   ebx
        call   sub_1_04D0
        mov    byte ptr [ebx + 0x4b], 1
        movsx  eax, byte ptr [ebx + 0x48]
        push   1
        push   0
        push   eax
        push   eax
        push   0x6d0                         ; -> loc_1_06D0
        push   ebx
        push   0x6a0                         ; -> loc_1_06A0
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_1_1CBE
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_06F0
        push   0
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        jmp    loc_2_074F
        db  08Dh, 049h, 000h
loc_2_06F0:
        push   ebx
        call   sub_2_1C20
        or     al, al
        jne    loc_2_0715
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    dword ptr [ebp - 8], 0xc0010006
        jmp    loc_2_0749
loc_2_0715:
        push   ebx
        call   sub_2_1DA0
        or     al, al
        jne    loc_2_07B0
        cmp    byte ptr [0x1e68], 0          ; -> obj1:0x1E68
        je     loc_2_07B0
        push   0
        push   0xaa000003
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    dword ptr [ebp - 8], 0xc0010008
loc_2_0749:
        push   esi
        call   sub_1_1CC4
loc_2_074F:
        cmp    dword ptr [0x1e5a], ebx       ; -> obj1:0x1E5A
        jne    loc_2_0761
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [0x1e5a], eax       ; -> obj1:0x1E5A
        jmp    loc_2_0781
loc_2_0761:
        mov    ecx, dword ptr [0x1e5a]       ; -> obj1:0x1E5A
        cmp    dword ptr [ecx + 0x24], ebx
        je     loc_2_0778
        lea    esp, [esp]
loc_2_0770:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], ebx
        jne    loc_2_0770
loc_2_0778:
        mov    eax, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], eax
loc_2_0781:
        mov    eax, dword ptr [ebx + 0x1d4]
        push   eax
        call   sub_2_25B0
loc_2_078D:
        mov    edx, dword ptr [ebx + 0x54]
        add    edx, 8
        sub    al, al
        out    dx, al
loc_2_0796:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C08
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_07B0:
        mov    byte ptr [ebx + 0x4b], 0
        mov    dword ptr [ebx + 0x212], 0
        mov    dword ptr [ebx + 0x216], 0x6d0 ; -> loc_1_06D0
        mov    dword ptr [ebx + 0x21a], ebx
        lea    eax, [ebx + 0x222]
        mov    dword ptr [ebx + 0x21e], 0x1520 ; -> loc_1_1520
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [ebx + 0x21e]
        mov    dword ptr [ebx + 0x226], ecx
        mov    dword ptr [ebx + 0x22a], ebx
        push   0x7d0
        push   eax
        call   sub_1_1CB2
        mov    byte ptr [ebx + 0x22f], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0820:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0xffffffff
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x18]
        lea    eax, [eax + edx*4]
loc_2_0843:
        or     edx, edx
        je     loc_2_0852
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_0843
        mov    dword ptr [ecx], edx
loc_2_0852:
        cmp    dword ptr [ecx], -1
        jne    loc_2_0864
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0864:
        mov    eax, dword ptr [0x1e18]       ; -> obj1:0x1E18
        push   eax
        push   0
        push   0x75
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1CD6
        or     eax, eax
        je     loc_2_08A0
        push   0
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_2_08A0:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x30]
        mov    ecx, dword ptr [esi + 0x28]
        or     ecx, ecx
        jne    loc_2_08BC
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_2_08BC
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xd
        sub    al, al
        out    dx, al
loc_2_08BC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x28], eax
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_2_09A1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        mov    edi, dword ptr [ebp + 0x1c]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_2_2600
        or     al, al
        je     loc_2_09A1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0xee
        mov    dword ptr [esi + 0x1e6], 0xee
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_0980
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_2_0963
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_2_0963
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_0980
loc_2_0963:
        push   1
        add    esi, 0x202
        push   esi
        call   sub_1_1CB2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  005h, 000h, 000h, 000h, 000h
loc_2_0980:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_2_0997
        movsx  eax, word ptr [esi + 0x66]
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_2_0997
        push   esi
        call   sub_1_1400
loc_2_0997:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_09A1:
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x28], eax
        push   0x75
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1CDC
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_09E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [eax + 0x28]
        cmp    esi, edx
        je     loc_2_0A00
loc_2_09F0:
        mov    ecx, dword ptr [esi + 0x18]
        cmp    edx, ecx
        jae    loc_2_09F9
        mov    edx, ecx
loc_2_09F9:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_2_09F0
loc_2_0A00:
        or     edx, edx
        jne    loc_2_0A09
        mov    edx, 0xee
loc_2_0A09:
        mov    dword ptr [eax + 0x1e6], edx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_2_0A43
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A43:
        inc    dword ptr [edi + 0x30]
        inc    dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [edi + 0x28]
        cmp    ecx, ebx
        jne    loc_2_0A58
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0x28], eax
        jmp    loc_2_0A71
loc_2_0A58:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_2_0A68
loc_2_0A60:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_2_0A60
loc_2_0A68:
        lea    eax, [ebx + 0xc]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_2_0A71:
        push   0
        mov    eax, dword ptr [ebx + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 0x1d4]
        push   eax
        call   sub_2_26D0
        mov    esi, eax
        or     esi, esi
        jne    loc_2_0AA8
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_2_0AA0
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    esi, 0x103
        jmp    loc_2_0AD4
loc_2_0AA0:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_2_0AD4
loc_2_0AA8:
        cmp    esi, 0x103
        jne    loc_2_0AC0
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_2_0AD4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0AC0:
        cmp    esi, 0xc001000e
        jne    loc_2_0AD1
        mov    byte ptr [ebx + 0x20], 1
        mov    esi, 0x103
loc_2_0AD1:
        dec    dword ptr [ebx + 0x1c]
loc_2_0AD4:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_2_0B09
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [edi + 0x1e6], eax
        jne    loc_2_0AEB
        push   edi
        call   sub_2_09E0
loc_2_0AEB:
        push   0
        push   0x75
        push   ebx
        call   sub_1_1CDC
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_2_0B17
        cmp    dword ptr [edi + 0x2c], 0
        jne    loc_2_0B17
        push   edi
        call   sub_1_0000
        jmp    loc_2_0B17
loc_2_0B09:
        mov    eax, dword ptr [edi + 0x2c]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [edi + 0x2c], ebx
        mov    esi, 0x103
loc_2_0B17:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_0B51
        cmp    byte ptr [edi + 0x1cb], 0
        jne    loc_2_0B38
        cmp    dword ptr [edi + 0x1f6], 0
        jne    loc_2_0B38
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_0B51
loc_2_0B38:
        push   1
        add    edi, 0x202
        push   edi
        call   sub_1_1CB2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0B51:
        cmp    dword ptr [edi + 0x5e], 0
        je     loc_2_0B68
        movsx  eax, word ptr [edi + 0x66]
        cmp    dword ptr [edi + eax*4 + 0x6e], 0
        jne    loc_2_0B68
        push   edi
        call   sub_1_1400
loc_2_0B68:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0B80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [edi + 0x30]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_2_0BA0
        mov    esi, 0xc0010002
        jmp    loc_2_0BDC
        db  090h
loc_2_0BA0:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        or     eax, eax
        jne    loc_2_0BB6
        push   ecx
        push   ebx
        push   edi
        call   sub_2_0FF0
        mov    esi, eax
        jmp    loc_2_0BDC
loc_2_0BB6:
        cmp    eax, 1
        jne    loc_2_0BD7
        cmp    byte ptr [edi + 0x1cb], 0
        je     loc_2_0BCB
        mov    esi, 0xc001000d
        jmp    loc_2_0BDC
loc_2_0BCB:
        push   ecx
        push   ebx
        push   edi
        call   sub_2_1040
        mov    esi, eax
        jmp    loc_2_0BDC
loc_2_0BD7:
        mov    esi, 0x10001
loc_2_0BDC:
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_0C17
        cmp    byte ptr [edi + 0x1cb], 0
        jne    loc_2_0C00
        cmp    dword ptr [edi + 0x1f6], 0
        jne    loc_2_0C00
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_0C17
loc_2_0C00:
        push   1
        add    edi, 0x202
        push   edi
        call   sub_1_1CB2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_0C17:
        cmp    dword ptr [edi + 0x5e], 0
        je     loc_2_0C2E
        movsx  eax, word ptr [edi + 0x66]
        cmp    dword ptr [edi + eax*4 + 0x6e], 0
        jne    loc_2_0C2E
        push   edi
        call   sub_1_1400
loc_2_0C2E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], 0
        lea    esi, [ebp - 8]
        mov    dword ptr [ebp - 0xc], 0
        mov    edi, 4
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010101
        ja     loc_2_0C90
        je     loc_2_0E43
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_2_0CB1
        shl    eax, 2
        jmp    loc_2_0FA4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C90:
        cmp    eax, 0x1010102
        je     loc_2_0E65
        cmp    eax, 0x1010103
        je     loc_2_0E87
        cmp    eax, 0x1010104
        je     loc_2_0F52
loc_2_0CB1:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_2_0F5B
        db  08Dh, 049h, 000h
loc_2_0CC0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_2_0CD5
        mov    esi, 0x78                     ; -> loc_3_0078
        mov    edi, 0x58
        jmp    loc_2_0F5B
loc_2_0CD5:
        mov    esi, 0                        ; -> loc_3_0000
        mov    edi, 0x78
        jmp    loc_2_0F5B
loc_2_0CE4:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x1cb], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_2_0CFE
        mov    dword ptr [ebp - 0x14], 0
loc_2_0CFE:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_2_0F5B
        db  005h, 000h, 000h, 000h, 000h
loc_2_0D10:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_2_0F5B
        db  08Dh, 049h, 000h
loc_2_0D20:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_2_0F5B
        db  08Dh, 064h, 024h, 000h
loc_2_0D30:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_2_0F5B
        db  08Dh, 064h, 024h, 000h
loc_2_0D40:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_2_0F5B
        db  08Dh, 064h, 024h, 000h
loc_2_0D50:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x38]
        shl    eax, 9
        lea    eax, [eax + eax*2]
        jmp    loc_2_0F58
loc_2_0D61:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0x2000
        mov    eax, dword ptr [ebx + 0x38]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 8
        neg    eax
        add    eax, 0x2000
        mov    dword ptr [ebp - 8], eax
        shr    eax, 8
        shl    eax, 2
        sub    dword ptr [ebp - 8], eax
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_2_0F5B
loc_2_0D97:
        mov    dword ptr [ebp - 8], 0x600
        jmp    loc_2_0F5B
loc_2_0DA3:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_2_0F5B
        db  090h
loc_2_0DB0:
        mov    esi, 0x1f60                   ; -> loc_1_1F60
        mov    edi, 0x16
        jmp    loc_2_0F5B
        db  090h
loc_2_0DC0:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0DD7
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1d4]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_2_0F58
loc_2_0DD7:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_2_0F58
loc_2_0DE5:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0DF9
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1e6]
        jmp    loc_2_0F58
loc_2_0DF9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        jmp    loc_2_0F58
loc_2_0E04:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_2_0F5B
loc_2_0E17:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_2_0F5B
loc_2_0E23:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0xe
        cmp    byte ptr [ebx + 0x4a], 0
        jne    loc_2_0F5B
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_2_0F5B
loc_2_0E43:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x1a3
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_1DBC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_0F5B
loc_2_0E65:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x19d
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_1DBC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_0F5B
loc_2_0E87:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_2_0EEA
        mov    eax, dword ptr [ebx + 0x1d4]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_0EBC
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_0F5B
loc_2_0EBC:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebx + 0x1d4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2BF0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_2_0F5B
loc_2_0EEA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x1d4]
        push   eax
        call   sub_2_2AF0
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_0F20
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_0F5B
        db  08Dh, 049h, 000h
loc_2_0F20:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x1d4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2B30
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_2_0F5B
loc_2_0F52:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x4c]
loc_2_0F58:
        mov    dword ptr [ebp - 8], eax
loc_2_0F5B:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0F97
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_2_0F97
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_2_0F87
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edi
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_0F87:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_1DBC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_2_0F97:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  090h
loc_2_0FA4:
        ror    byte ptr [eax + eax], 0
        in     al, 0xc
        add    byte ptr [eax], al
        adc    byte ptr [0xd100000], cl      ; -> loc_2_0D10
        add    byte ptr [eax], al
        and    byte ptr [0xd300000], cl      ; -> loc_2_0D30
        add    byte ptr [eax], al
        inc    eax
        or     eax, 0xd500000                ; -> loc_2_0D50
        add    byte ptr [eax], al
        popal  
        or     eax, 0xd970000                ; -> loc_2_0D97
        add    byte ptr [eax], al
        mov    dword ptr [0xb100000d], eax   ; -> loc_2_0CB1
        or     al, 0
        add    byte ptr [eax - 0x3ffffff3], dh ; -> loc_2_0DB0
        or     eax, 0xde50000                ; -> loc_2_0DE5
        add    byte ptr [eax], al
        add    al, 0xe
        add    byte ptr [eax], al
        pop    ss
        push   cs
        add    byte ptr [eax], al
        mov    cl, 0xc
        add    byte ptr [eax], al
        and    ecx, dword ptr [esi]
        add    byte ptr [eax], al
sub_2_0FF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C40
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_1040:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010e
        je     loc_2_1097
        cmp    eax, 0x1010f
        je     loc_2_10F8
        cmp    eax, 0x10112
        je     loc_2_1180
        cmp    eax, 0x1010103
        je     loc_2_11B0
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_11F1
loc_2_1097:
        cmp    edi, 4
        je     loc_2_10B4
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_11F1
loc_2_10B4:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1DBC
        test   dword ptr [ebp - 8], 0xf050
        je     loc_2_10E1
        mov    ebx, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_11F1
loc_2_10E1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1210
        jmp    loc_2_11EF
loc_2_10F8:
        cmp    edi, 4
        je     loc_2_1115
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_11F1
loc_2_1115:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1DBC
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_2_1179
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x1e6]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_2_1150
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x1e6], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_11F1
        db  090h
loc_2_1150:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_2_1171
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_2_1171
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        push   edx
        call   sub_2_09E0
        jmp    loc_2_11F1
loc_2_1171:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_11F1
loc_2_1179:
        mov    ebx, 0xc0010014
        jmp    loc_2_11F1
loc_2_1180:
        cmp    edi, 4
        je     loc_2_119A
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_11F1
loc_2_119A:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_1DBC
        sub    ebx, ebx
        jmp    loc_2_11F1
        db  08Dh, 049h, 000h
loc_2_11B0:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_2_11D4
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_11F1
loc_2_11D4:
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1270
loc_2_11EF:
        mov    ebx, eax
loc_2_11F1:
        or     ebx, ebx
        jne    loc_2_11FF
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_2_11FF:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1210:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x1cb], 0
        jne    loc_2_125A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1250
        inc    dword ptr [esi + 0x1c]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x1d4]
        push   eax
        call   sub_2_2A70
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_2_1250:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_2_125A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1270:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x1cb], 0
        jne    loc_2_12C0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_12B2
        inc    dword ptr [esi + 0x1c]
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x1d4]
        push   eax
        call   sub_2_2790
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_2_12B2:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_2_12C0:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_12D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], 0
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_0C40
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_2_13C9
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_1350
        je     loc_2_1390
        cmp    eax, 0x20101
        je     loc_2_1370
        cmp    eax, 0x20102
        je     loc_2_1378
        cmp    eax, 0x20103
        je     loc_2_1380
        cmp    eax, 0x20104
        je     loc_2_1388
        jmp    loc_2_1365
loc_2_1350:
        cmp    eax, 0x1020101
        je     loc_2_1398
        cmp    eax, 0x1020102
        je     loc_2_13A0
        cmp    eax, 0x1020103
        je     loc_2_13A8
loc_2_1365:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_2_13B1
        db  08Bh, 0FFh
loc_2_1370:
        mov    eax, dword ptr [ebx + 0x1ab]
        jmp    loc_2_13AE
loc_2_1378:
        mov    eax, dword ptr [ebx + 0x1af]
        jmp    loc_2_13AE
loc_2_1380:
        mov    eax, dword ptr [ebx + 0x1b3]
        jmp    loc_2_13AE
loc_2_1388:
        mov    eax, dword ptr [ebx + 0x1c3]
        jmp    loc_2_13AE
loc_2_1390:
        mov    eax, dword ptr [ebx + 0x1c7]
        jmp    loc_2_13AE
loc_2_1398:
        mov    eax, dword ptr [ebx + 0x1bf]
        jmp    loc_2_13AE
loc_2_13A0:
        mov    eax, dword ptr [ebx + 0x1b7]
        jmp    loc_2_13AE
loc_2_13A8:
        mov    eax, dword ptr [ebx + 0x1bb]
loc_2_13AE:
        mov    dword ptr [ebp - 4], eax
loc_2_13B1:
        cmp    edi, 4
        jb     loc_2_13C9
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_1DBC
        add    dword ptr [ebp - 0xc], 4
loc_2_13C9:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_13F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_1429
        cmp    eax, 0x20101
        jae    loc_2_144A
        cmp    eax, 0x10101
        jb     loc_2_1445
        cmp    eax, 0x1010b
        jbe    loc_2_144A
        cmp    eax, 0x1010d
        jb     loc_2_1445
        cmp    eax, 0x10113
        jbe    loc_2_144A
        jmp    loc_2_1445
loc_2_1429:
        cmp    eax, 0x1010101
        jb     loc_2_1445
        cmp    eax, 0x1010104
        jbe    loc_2_144A
        cmp    eax, 0x1020101
        jb     loc_2_1445
        cmp    eax, 0x1020103
        jbe    loc_2_144A
loc_2_1445:
        mov    esi, 0xc0010017
loc_2_144A:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x30]
        or     esi, esi
        jne    loc_2_145D
        push   ecx
        push   edi
        call   sub_2_12D0
        mov    esi, eax
loc_2_145D:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_1494
        cmp    byte ptr [edi + 0x1cb], 0
        jne    loc_2_147E
        cmp    dword ptr [edi + 0x1f6], 0
        jne    loc_2_147E
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_1494
loc_2_147E:
        push   1
        add    edi, 0x202
        push   edi
        call   sub_1_1CB2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_2_1494:
        cmp    dword ptr [edi + 0x5e], 0
        je     loc_2_14AB
        movsx  eax, word ptr [edi + 0x66]
        cmp    dword ptr [edi + eax*4 + 0x6e], 0
        jne    loc_2_14AB
        push   edi
        call   sub_1_1400
loc_2_14AB:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_14C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x1e1c]       ; -> obj1:0x1E1C
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1BFC
        push   0
        mov    eax, dword ptr [0x1e20]       ; -> obj1:0x1E20
        push   eax
        call   sub_1_1BF0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_14F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x22f], 1
        cmp    dword ptr [0x1e5a], esi       ; -> obj1:0x1E5A
        jne    loc_2_1513
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x1e5a], eax       ; -> obj1:0x1E5A
        jmp    loc_2_1531
loc_2_1513:
        mov    ecx, dword ptr [0x1e5a]       ; -> obj1:0x1E5A
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_1528
        mov    edi, edi
loc_2_1520:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_1520
loc_2_1528:
        mov    eax, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], eax
loc_2_1531:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x222]
        push   eax
        call   sub_1_1CB8
        push   0x2625a0
        call   sub_1_1CD0
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        call   sub_2_25B0
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_1CC4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1C08
        push   0
        push   0x230
        push   esi
        call   sub_1_1CDC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1580:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_2_15A0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_15A0:
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x1cb], 0
        je     loc_2_15C1
        dec    dword ptr [ebx + 0x1c]
        dec    dword ptr [esi + 0x30]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_15C1:
        mov    edi, dword ptr [esi + 0x28]
        or     edi, edi
        je     loc_2_15E8
loc_2_15C8:
        inc    dword ptr [edi + 0x1c]
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C3E
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        or     eax, eax
        jne    loc_2_15C8
loc_2_15E8:
        mov    byte ptr [esi + 0x1cb], 1
        mov    dword ptr [esi + 0x1cc], 0
        mov    dword ptr [esi + 0x1d0], ebx
        push   esi
        call   sub_2_1670
        mov    edi, eax
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_1642
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_2_162B
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_2_162B
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_1642
loc_2_162B:
        push   1
        add    esi, 0x202
        push   esi
        call   sub_1_1CB2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_1642:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_2_1659
        movsx  eax, word ptr [esi + 0x66]
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_2_1659
        push   esi
        call   sub_1_1400
loc_2_1659:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1670:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x1f6], 0
        je     loc_2_16BC
loc_2_1684:
        mov    ecx, dword ptr [esi + 0x1f6]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x1f6], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_2_16B0
        cmp    eax, 8
        je     loc_2_16B0
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C2C
loc_2_16B0:
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_2_1684
loc_2_16BC:
        cmp    word ptr [esi + 0x6a], -1
        je     loc_2_16DB
        mov    dword ptr [esi + 0x1cc], 2
        dec    dword ptr [esi + 0x30]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_16DB:
        dec    dword ptr [esi + 0x30]
        push   esi
        call   sub_2_16F0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_16F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    word ptr [eax + 0x66], 0
        inc    dword ptr [eax + 0x30]
        mov    word ptr [eax + 0x68], 0
        mov    edx, dword ptr [eax + 0x38]
        cmp    edx, ecx
        je     loc_2_171F
        lea    esi, [eax + 0x6e]
loc_2_1712:
        cmp    dword ptr [esi], 0
        jne    loc_2_1730
        add    esi, 4
        inc    ecx
        cmp    ecx, edx
        jb     loc_2_1712
loc_2_171F:
        dec    dword ptr [eax + 0x30]
        push   eax
        call   sub_2_1750
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1730:
        mov    dword ptr [eax + 0x1cc], 3
        dec    dword ptr [eax + 0x30]
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1750:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x38], 0
        jbe    loc_2_179E
        lea    edi, [esi + 0x76]
        nop    
loc_2_1770:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_2_1790
        mov    ebx, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [ebx + 0x1c]
        mov    dword ptr [edi], 0
loc_2_1790:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0x38], eax
        ja     loc_2_1770
loc_2_179E:
        mov    edx, dword ptr [esi + 0x5e]
        or     edx, edx
        je     loc_2_17C9
loc_2_17A5:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x5e], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x5e]
        or     edx, edx
        jne    loc_2_17A5
loc_2_17C9:
        mov    edx, dword ptr [esi + 0x1ea]
        or     edx, edx
        je     loc_2_17FD
loc_2_17D3:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x1ea], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x1ea]
        or     edx, edx
        jne    loc_2_17D3
loc_2_17FD:
        cmp    byte ptr [esi + 0x1aa], 0
        je     loc_2_1819
loc_2_1806:
        push   0x2710
        call   sub_1_1CD0
        cmp    byte ptr [esi + 0x1aa], 0
        jne    loc_2_1806
loc_2_1819:
        mov    byte ptr [esi + 0x1e5], 0x1b
        push   esi
        call   sub_2_2160
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x1cb], 0
        mov    edi, dword ptr [esi + 0x28]
        or     edi, edi
        je     loc_2_1880
        sub    ebx, ebx
loc_2_1842:
        inc    dword ptr [edi + 0x1c]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_2_1859
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C3E
loc_2_1859:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C3E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C44
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        cmp    eax, ebx
        jne    loc_2_1842
loc_2_1880:
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_1890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x1cc], eax
        jne    loc_2_1977
        cmp    eax, 1
        je     loc_2_1912
        cmp    eax, 2
        je     loc_2_191A
        cmp    eax, 3
        je     loc_2_1922
        mov    eax, dword ptr [ebp - 4]
loc_2_18BC:
        cmp    eax, 0x103
        je     loc_2_18DB
        push   eax
        mov    eax, dword ptr [esi + 0x1d0]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1C26
        mov    eax, dword ptr [esi + 0x1d0]
        dec    dword ptr [eax + 0x1c]
loc_2_18DB:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_1960
        cmp    byte ptr [esi + 0x1cb], 0
        jne    loc_2_18FC
        cmp    dword ptr [esi + 0x1f6], 0
        jne    loc_2_18FC
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_1960
loc_2_18FC:
        push   1
        add    esi, 0x202
        push   esi
        call   sub_1_1CB2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_1912:
        push   esi
        call   sub_2_1670
        jmp    loc_2_18BC
loc_2_191A:
        push   esi
        call   sub_2_16F0
        jmp    loc_2_18BC
loc_2_1922:
        mov    ecx, dword ptr [esi + 0x30]
        inc    ecx
        mov    dword ptr [esi + 0x30], ecx
        sub    edx, edx
        mov    eax, dword ptr [esi + 0x38]
        cmp    eax, edx
        je     loc_2_1942
        lea    edi, [esi + 0x6e]
loc_2_1935:
        cmp    dword ptr [edi], 0
        jne    loc_2_1951
        add    edi, 4
        inc    edx
        cmp    edx, eax
        jb     loc_2_1935
loc_2_1942:
        dec    ecx
        mov    dword ptr [esi + 0x30], ecx
        push   esi
        call   sub_2_1750
        jmp    loc_2_18BC
loc_2_1951:
        dec    ecx
        mov    dword ptr [esi + 0x30], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_1960:
        cmp    dword ptr [esi + 0x5e], 0
        je     loc_2_1977
        movsx  eax, word ptr [esi + 0x66]
        cmp    dword ptr [esi + eax*4 + 0x6e], 0
        jne    loc_2_1977
        push   esi
        call   sub_1_1400
loc_2_1977:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_1980:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_2_1993
        lea    edx, [eax + 0x49]
loc_2_1993:
        cmp    byte ptr [ecx + 0x1cb], 0
        je     loc_2_19A5
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_2_19A5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x1f6], 0
        jne    loc_2_19E0
        mov    dword ptr [ecx + 0x1fa], edx
        mov    dword ptr [ecx + 0x1f6], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_2_19E0:
        mov    eax, dword ptr [ecx + 0x1fa]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x1fa], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1A00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_2_1A13
        lea    edx, [eax + 0x21]
loc_2_1A13:
        cmp    byte ptr [ecx + 0x1cb], 0
        je     loc_2_1A25
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_2_1A25:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x1f6], 0
        jne    loc_2_1A60
        mov    dword ptr [ecx + 0x1fa], edx
        mov    dword ptr [ecx + 0x1f6], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_2_1A60:
        mov    eax, dword ptr [ecx + 0x1fa]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x1fa], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1A80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x1c]
        ret    4
        db  08Dh, 049h, 000h
sub_2_1A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x54]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, al
        je     loc_2_1B46
        and    eax, 0xff
        lea    edx, [eax - 1]
        test   eax, edx
        jne    loc_2_1B46
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 1
        mov    byte ptr [eax], 1
        sub    al, al
loc_2_1AC8:
        test   byte ptr [ebp - 1], dl
        jne    loc_2_1AE0
        shr    byte ptr [ebp - 1], 1
        inc    al
        cmp    al, 8
        jne    loc_2_1AC8
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_1AE0:
        xor    edx, edx
        mov    dl, al
        mov    eax, dword ptr [ecx + 0x3c]
        cmp    dword ptr [edx*4 + 0x1fd8], eax ; -> loc_1_1FD8
        mov    eax, dword ptr [ebp + 0x10]
        je     loc_2_1B00
        mov    byte ptr [eax], 0
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_1B00:
        mov    byte ptr [eax], 1
        mov    edx, dword ptr [ecx + 0x54]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 1], al
        sub    al, al
        mov    edx, 0x10
loc_2_1B14:
        test   byte ptr [ebp - 1], dl
        jne    loc_2_1B30
        shr    byte ptr [ebp - 1], 1
        inc    al
        cmp    al, 4
        jne    loc_2_1B14
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1B30:
        mov    byte ptr [ecx + 0x4a], 1
        and    eax, 0xff
        mov    eax, dword ptr [eax*4 + 0x1ff8] ; -> obj1:0x1FF8
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_1B46:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 0
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 6
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 6
        out    dx, al
        sub    ecx, ecx
loc_2_1B72:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, ecx
        in     al, dx
        mov    byte ptr [esi + ecx + 0x1a3], al
        inc    ecx
        cmp    ecx, 6
        jb     loc_2_1B72
        mov    edx, dword ptr [esi + 0x54]
        add    edx, 6
        mov    al, 2
        out    dx, al
        cmp    byte ptr [esi + 0x19d], 0
        jne    loc_2_1C0C
        cmp    byte ptr [esi + 0x19e], 0
        jne    loc_2_1C0C
        cmp    byte ptr [esi + 0x19f], 0
        jne    loc_2_1C0C
        cmp    byte ptr [esi + 0x1a0], 0
        jne    loc_2_1C0C
        cmp    byte ptr [esi + 0x1a1], 0
        jne    loc_2_1C0C
        cmp    byte ptr [esi + 0x1a2], 0
        jne    loc_2_1C0C
        mov    al, byte ptr [esi + 0x1a3]
        mov    byte ptr [esi + 0x19d], al
        mov    al, byte ptr [esi + 0x1a4]
        mov    byte ptr [esi + 0x19e], al
        mov    al, byte ptr [esi + 0x1a5]
        mov    byte ptr [esi + 0x19f], al
        mov    al, byte ptr [esi + 0x1a6]
        mov    byte ptr [esi + 0x1a0], al
        mov    al, byte ptr [esi + 0x1a7]
        mov    byte ptr [esi + 0x1a1], al
        mov    al, byte ptr [esi + 0x1a8]
        mov    byte ptr [esi + 0x1a2], al
loc_2_1C0C:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 3
        mov    edx, dword ptr [esi + 0x54]
        add    dx, 6
        out    dx, al
        sub    al, al
        out    dx, al
        mov    al, 2
        out    dx, al
        cmp    byte ptr [esi + 0x49], 1
        sbb    al, al
        and    al, 2
        mov    byte ptr [esi + 0x95], al
        or     al, 0x20
        mov    edi, dword ptr [esi + 0x54]
        mov    byte ptr [esi + 0x95], al
        lea    edx, [edi + 6]
        out    dx, al
        mov    dx, di
        mov    al, byte ptr [esi + 0x93]
        out    dx, al
        lea    edx, [edi + 1]
        mov    al, byte ptr [esi + 0x94]
        out    dx, al
        mov    al, 4
        mov    cl, byte ptr [esi + 0x48]
        shl    al, cl
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 2]
        mov    al, 0x10
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, 0x20
        out    dx, al
        lea    edx, [edi + 0xa]
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x4a], al
        lea    edx, [edi + 5]
        je     loc_2_1C96
        mov    al, 0x49
        jmp    loc_2_1C98
loc_2_1C96:
        mov    al, 0x40
loc_2_1C98:
        out    dx, al
        mov    dx, word ptr [esi + 0x50]
        mov    al, 0x21
        out    dx, al
        in     al, dx
        cmp    al, 0x21
        je     loc_2_1CB0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1CB0:
        mov    edi, dword ptr [esi + 0x50]
        mov    al, 0x48
        lea    edx, [edi + 0xe]
        out    dx, al
        lea    edx, [edi + 0xb]
        sub    al, al
        out    dx, al
        lea    edx, [edi + 0xa]
        out    dx, al
        lea    edx, [edi + 0xc]
        mov    al, byte ptr [esi + 0x1e4]
        out    dx, al
        lea    edx, [edi + 0xd]
        mov    al, 2
        out    dx, al
        mov    cl, byte ptr [esi + 0x93]
        lea    edx, [edi + 3]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 1]
        out    dx, al
        lea    edx, [edi + 2]
        mov    al, byte ptr [esi + 0x94]
        out    dx, al
        inc    cl
        mov    byte ptr [esi + 0x97], cl
        mov    al, byte ptr [esi + 0x93]
        inc    al
        mov    byte ptr [esi + 0x96], al
        mov    ecx, edi
        mov    byte ptr [esi + 0x1a9], 0
        lea    eax, [ecx + 7]
        mov    word ptr [ebp - 8], ax
        mov    dx, ax
        mov    al, 0xff
        out    dx, al
        lea    edx, [ecx + 0xf]
        mov    al, byte ptr [esi + 0x1e5]
        out    dx, al
        mov    ax, cx
        mov    dx, ax
        mov    al, 0x61
        out    dx, al
        sub    edi, edi
        mov    edi, edi
loc_2_1D30:
        lea    edx, [ecx + edi]
        inc    edx
        mov    al, byte ptr [esi + edi + 0x19d]
        out    dx, al
        inc    edi
        cmp    edi, 6
        jb     loc_2_1D30
        mov    eax, dword ptr [esi + 0x1d4]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 0xc]
        and    edi, 4
loc_2_1D50:
        or     edi, edi
        mov    eax, 0xff
        jne    loc_2_1D62
        xor    eax, eax
        mov    al, byte ptr [esi + ebx + 0x1d8]
loc_2_1D62:
        lea    edx, [ecx + ebx]
        add    edx, 8
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_2_1D50
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [esi + 0x97]
        out    dx, al
        mov    ax, cx
        mov    dx, ax
        mov    al, 0x21
        out    dx, al
        mov    ax, cx
        mov    dx, ax
        mov    al, 0x22
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20c
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 5], 1
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    eax, [esi + 0x19d]
        push   eax
        push   0x2030                        ; -> loc_1_2030
        call   sub_1_1DBC
        push   6
        lea    eax, [esi + 0x19d]
        push   eax
        push   0x2036                        ; -> loc_1_2036
        call   sub_1_1DBC
        mov    byte ptr [0x203c], 0          ; -> loc_1_203C
        mov    byte ptr [0x203d], 0          ; -> obj1:0x203D
        mov    byte ptr [0x206b], 0x92       ; -> obj1:0x206B
        mov    ecx, dword ptr [esi + 0x50]
        mov    al, 0x40
        lea    edi, [ecx + 0xe]
        mov    dx, di
        out    dx, al
        lea    edx, [ecx + 0xc]
        sub    al, al
        out    dx, al
        sub    edi, edi
loc_2_1E02:
        mov    ecx, dword ptr [esi + 0x50]
        mov    dx, cx
        mov    al, 0x21
        out    dx, al
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        mov    al, byte ptr [edi + 0x2014]   ; -> loc_1_2014
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        push   0x3c
        push   0
        push   0
        push   0x2030                        ; -> loc_1_2030
        out    dx, al
        push   esi
        call   sub_1_0290
        mov    ecx, dword ptr [esi + 0x50]
        lea    eax, [ecx + 4]
        mov    dx, ax
        mov    al, byte ptr [esi + 0x92]
        out    dx, al
        lea    edx, [ecx + 6]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 0x3c
        out    dx, al
        mov    dx, cx
        mov    al, 0x26
        out    dx, al
        sub    ebx, ebx
        lea    edx, [ecx + 7]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 0xa
        jne    loc_2_1E85
        mov    edi, edi
loc_2_1E60:
        inc    ebx
        cmp    ebx, 0x64
        ja     loc_2_2100
        push   0x2af8
        call   sub_1_1CD0
        mov    ecx, dword ptr [esi + 0x50]
        add    ecx, 7
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 0xa
        je     loc_2_1E60
loc_2_1E85:
        sub    ebx, ebx
loc_2_1E87:
        mov    dx, word ptr [esi + 0x50]
        mov    al, 0x21
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0x21
        je     loc_2_1EA9
        push   0x3e8
        call   sub_1_1CD0
        inc    ebx
        cmp    ebx, 0x7d0
        jb     loc_2_1E87
loc_2_1EA9:
        lea    eax, [ebp - 4]
        push   eax
        push   0x2008                        ; -> loc_1_2008
        call   sub_1_1CB8
        mov    ecx, dword ptr [esi + 0x50]
        mov    al, 0xa
        lea    ebx, [ecx + 7]
        mov    dx, bx
        out    dx, al
        lea    edx, [ecx + 4]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    al, 0x10
        cmp    byte ptr [edi + 0x201c], al   ; -> loc_1_201C
        jne    loc_2_2100
        mov    al, byte ptr [ebp - 3]
        and    al, 0x40
        cmp    byte ptr [edi + 0x2018], al   ; -> loc_1_2018
        je     loc_2_1EF2
        cmp    byte ptr [edi + 0x2014], 6    ; -> loc_1_2014
        jne    loc_2_2100
loc_2_1EF2:
        cmp    byte ptr [edi + 0x2014], 6    ; -> loc_1_2014
        je     loc_2_1F3E
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xc
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   al, 2
        je     loc_2_2100
        sub    ecx, ecx
        nop    
loc_2_1F10:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 6
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [ecx + 0x2020], al   ; -> loc_1_2020
        jne    loc_2_2100
        inc    ecx
        cmp    ecx, 4
        jb     loc_2_1F10
        mov    ecx, 4
loc_2_1F31:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 6
        in     al, dx
        mov    byte ptr [ebp - 1], al
        dec    ecx
        jne    loc_2_1F31
loc_2_1F3E:
        inc    edi
        cmp    edi, 2
        jb     loc_2_1E02
        mov    ecx, dword ptr [esi + 0x50]
        mov    dx, cx
        mov    al, 0x21
        out    dx, al
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        mov    al, 3
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        out    dx, al
        sub    edi, edi
        mov    ebx, 0xa
loc_2_1F67:
        cmp    byte ptr [edi + 0x2028], 0    ; -> loc_1_2028
        push   6
        je     loc_2_1F80
        lea    eax, [esi + 0x19d]
        push   eax
        jmp    loc_2_1F85
        db  005h, 000h, 000h, 000h, 000h
loc_2_1F80:
        push   0x206c                        ; -> loc_1_206C
loc_2_1F85:
        push   0x2030                        ; -> loc_1_2030
        call   sub_1_1DBC
        push   0x3c
        push   0
        push   0
        push   0x2030                        ; -> loc_1_2030
        push   esi
        call   sub_1_0290
        lea    eax, [ebp - 0xc]
        push   eax
        push   0x3c
        push   0x2030                        ; -> loc_1_2030
        call   sub_2_2210
        cmp    byte ptr [edi + 0x2024], 0    ; -> loc_1_2024
        jne    loc_2_1FC1
        mov    al, byte ptr [ebp - 0xc]
        not    al
        mov    byte ptr [ebp - 0xc], al
loc_2_1FC1:
        push   4
        push   0x3c
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_0290
        mov    ecx, dword ptr [esi + 0x50]
        lea    eax, [ecx + 4]
        mov    dx, ax
        mov    al, byte ptr [esi + 0x92]
        out    dx, al
        lea    edx, [ecx + 6]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 0x40
        out    dx, al
        mov    dx, cx
        mov    al, 0x26
        out    dx, al
        push   0x3e8
        push   0x2008                        ; -> loc_1_2008
        call   sub_1_1CB2
        mov    ecx, dword ptr [esi + 0x50]
        add    ecx, 7
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   bl, al
        jne    loc_2_2021
loc_2_2013:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   bl, al
        je     loc_2_2013
loc_2_2021:
        lea    eax, [ebp - 4]
        push   eax
        push   0x2008                        ; -> loc_1_2008
        call   sub_1_1CB8
        mov    ecx, dword ptr [esi + 0x50]
        add    ecx, 7
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xc
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    al, 3
        cmp    byte ptr [edi + 0x202c], al   ; -> loc_1_202C
        jne    loc_2_2100
        inc    edi
        cmp    edi, 3
        jb     loc_2_1F67
        sub    eax, eax
        nop    
loc_2_2060:
        mov    ecx, eax
        neg    cl
        mov    byte ptr [ebp + eax - 0x20c], cl
        inc    eax
        cmp    eax, 0x100
        jb     loc_2_2060
        sub    ebx, ebx
loc_2_2075:
        push   0x100
        mov    ecx, 0x600
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        push   edx
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        push   eax
        lea    eax, [ebp - 0x20c]
        push   eax
        push   esi
        call   sub_1_0290
        lea    edi, [ebp - 0x10c]
        mov    eax, 0x4d4d4d4d
        mov    ecx, 0x40
        rep stosd dword ptr es:[edi], eax
        push   0x100
        mov    eax, dword ptr [esi + 0x86]
        add    eax, ebx
        push   eax
        lea    eax, [ebp - 0x10c]
        push   eax
        push   esi
        call   sub_1_0390
        sub    eax, eax
        lea    esp, [esp]
loc_2_20D0:
        mov    cl, byte ptr [ebp + eax - 0x20c]
        cmp    byte ptr [ebp + eax - 0x10c], cl
        jne    loc_2_2100
        inc    eax
        cmp    eax, 0x100
        jb     loc_2_20D0
        add    ebx, 0x100
        cmp    ebx, 0x2000
        jb     loc_2_2075
        jmp    loc_2_2104
        db  08Dh, 064h, 024h, 000h
loc_2_2100:
        mov    byte ptr [ebp - 5], 0
loc_2_2104:
        mov    ecx, dword ptr [esi + 0x50]
        mov    dx, cx
        mov    al, 0x21
        out    dx, al
        lea    edx, [ecx + 0xe]
        mov    al, 0x48
        out    dx, al
        lea    edx, [ecx + 0xb]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0xa]
        out    dx, al
        lea    edx, [ecx + 0xc]
        mov    al, byte ptr [esi + 0x1e4]
        out    dx, al
        lea    edx, [ecx + 0xd]
        mov    al, 2
        out    dx, al
        push   0xc350
        call   sub_1_1CD0
        mov    ecx, dword ptr [esi + 0x50]
        mov    al, 0xff
        lea    esi, [ecx + 7]
        mov    dx, si
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        out    dx, al
        mov    al, byte ptr [ebp - 5]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2160:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0000
        push   esi
        call   sub_2_1C20
        or     al, al
        jne    loc_2_2195
        push   1
        push   1
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 0x14
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_2_2195:
        mov    edx, dword ptr [esi + 0x50]
        add    edx, 0xd
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_21B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_2_21FD
loc_2_21C2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_2_21D0:
        mov    ecx, eax
        and    ecx, 0x80000000
        cmp    ecx, 1
        sbb    edi, edi
        inc    edi
        mov    cl, bl
        and    ecx, 1
        xor    edi, ecx
        add    eax, eax
        shr    bl, 1
        or     edi, edi
        je     loc_2_21F4
        xor    eax, 0x4c11db6
        or     eax, edi
loc_2_21F4:
        dec    esi
        jne    loc_2_21D0
        inc    edx
        cmp    edx, dword ptr [ebp + 0xc]
        jb     loc_2_21C2
loc_2_21FD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2210:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        sub    esi, esi
        push   eax
        call   sub_2_21B0
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp + 0x10]
        lea    esp, [esp]
loc_2_2230:
        lea    eax, [ebp - 1]
        sub    eax, esi
        mov    al, byte ptr [eax]
        mov    dl, al
        and    edx, 0xf
        shr    al, 4
        mov    dl, byte ptr [edx + 0x2074]   ; -> loc_1_2074
        and    eax, 0xff
        shl    dl, 4
        add    dl, byte ptr [eax + 0x2074]   ; -> loc_1_2074
        not    dl
        mov    byte ptr [ecx + esi], dl
        inc    esi
        cmp    esi, 4
        jb     loc_2_2230
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_21B0
        shr    eax, 0x1a
        mov    ebx, eax
        mov    edx, dword ptr [ebp + 0xc]
        shr    ebx, 3
        mov    byte ptr [edx], bl
        mov    dl, 1
        and    al, 7
        mov    cl, al
        mov    eax, dword ptr [ebp + 0x10]
        shl    dl, cl
        mov    byte ptr [eax], dl
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_22A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
        lea    esp, [esp]
loc_2_22C0:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x1d8], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_2_22C0
        push   0x1f78                        ; -> obj1:0x1F78
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x1d4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2BF0
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_2_2327
        sub    edi, edi
loc_2_22F5:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x1f78]         ; -> obj1:0x1F78
        push   eax
        call   sub_2_2270
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x1d8], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_2_22F5
loc_2_2327:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_2330:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 0x50]
        add    edx, 0xc
        mov    al, byte ptr [eax + 0x1e4]
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2350:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edi, dword ptr [eax + 0x50]
        mov    si, di
        mov    dx, si
        mov    al, 0x62
        out    dx, al
loc_2_2366:
        lea    edx, [ecx + edi]
        add    edx, 8
        mov    al, 0xff
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     loc_2_2366
        mov    dx, si
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2390:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 0x62
        mov    ebx, dword ptr [ecx + 0x50]
        mov    si, bx
        mov    dx, si
        out    dx, al
        sub    edi, edi
loc_2_23A7:
        lea    edx, [ebx + edi]
        add    edx, 8
        mov    al, byte ptr [ecx + edi + 0x1d8]
        out    dx, al
        inc    edi
        cmp    edi, 8
        jb     loc_2_23A7
        mov    dx, si
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_23D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x1d4]
        mov    edi, dword ptr [eax + 0xc]
        test   edi, 0x24
        je     loc_2_2400
        push   0x2350                        ; -> loc_2_2350
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_1CCA
        jmp    loc_2_2405
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_2400:
        call   sub_2_2460
loc_2_2405:
        test   edi, 0x26
        lea    eax, [esi + 0x1e4]
        je     loc_2_2418
        or     byte ptr [eax], 8
        jmp    loc_2_241B
loc_2_2418:
        and    byte ptr [eax], 0xf7
loc_2_241B:
        test   edi, 0x20
        je     loc_2_2428
        or     byte ptr [eax], 0x10
        jmp    loc_2_242B
loc_2_2428:
        and    byte ptr [eax], 0xef
loc_2_242B:
        test   edi, 0x28
        je     loc_2_2438
        or     byte ptr [eax], 4
        jmp    loc_2_243B
loc_2_2438:
        and    byte ptr [eax], 0xfb
loc_2_243B:
        push   esi
        push   0x2330                        ; -> loc_2_2330
        add    esi, 8
        push   esi
        call   sub_1_1CCA
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_22A0
        push   esi
        push   0x2390                        ; -> loc_2_2390
        add    esi, 8
        push   esi
        call   sub_1_1CCA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2490:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x2120]       ; -> obj1:0x2120
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_1CD6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_24C4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_24C4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_24DE
        mov    esi, 2
loc_2_24DE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1CD6
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_2512
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_25B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2512:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_1CD6
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_2541
        push   eax
        call   sub_2_25B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2541:
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
sub_2_25B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_25D0
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_1CDC
loc_2_25D0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_25E6
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_1CDC
loc_2_25E6:
        push   0
        push   0x32
        push   esi
        call   sub_1_1CDC
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2600:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_2619
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2619:
        mov    eax, dword ptr [0x2120]       ; -> obj1:0x2120
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1CD6
        or     eax, eax
        je     loc_2_2639
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2639:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_2640:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_2653
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_2640
        jmp    loc_2_2662
loc_2_2653:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_2662:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_2678
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2678:
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
sub_2_26D0:
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
        call   sub_2_2A70
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_26F9
        cmp    ebx, 0x103
        jne    loc_2_2710
loc_2_26F9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_2790
        or     eax, eax
        je     loc_2_2710
        mov    ebx, eax
loc_2_2710:
        or     ebx, ebx
        je     loc_2_271C
        cmp    ebx, 0x103
        jne    loc_2_277B
loc_2_271C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_2776
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_2732
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2732:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_2741
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_2747
loc_2_2741:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_2747:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_2756
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1C38
loc_2_2756:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_1CDC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_2776:
        mov    ebx, 0xc001000e
loc_2_277B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2790:
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
        call   sub_1_1DDC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_1DDC
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_2814
        sub    eax, eax
        nop    
loc_2_27F0:
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
        ja     loc_2_27F0
loc_2_2814:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_287F
loc_2_2821:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_2874
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
        call   sub_1_1DDC
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
        call   sub_1_1DDC
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_2877
loc_2_2874:
        inc    dword ptr [ebp - 4]
loc_2_2877:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_2821
loc_2_287F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_296A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_2893:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2C50
        or     al, al
        je     loc_2_28CA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_2958
loc_2_28CA:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_29A4
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
        call   sub_1_1DDC
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
        call   sub_1_1DDC
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
loc_2_2958:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_2893
loc_2_296A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_29F5
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_2980:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_29FB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_29E2
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_29E2
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_29E9
loc_2_29A4:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DDC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_1DDC
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_29E2:
        mov    dword ptr [ebp - 8], 1
loc_2_29E9:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_2980
loc_2_29F5:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_2A60
loc_2_29FB:
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
        je     loc_2_2A62
        cmp    edi, 0x103
        je     loc_2_2A62
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DDC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DDC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_2A60:
        sub    edi, edi
loc_2_2A62:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_2A70:
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
        je     loc_2_2AAD
        lea    esp, [esp]
loc_2_2AA0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_2AA0
loc_2_2AAD:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_2AE1
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
        je     loc_2_2AE3
        cmp    eax, 0x103
        je     loc_2_2AE3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2AE1:
        sub    eax, eax
loc_2_2AE3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_2AF0:
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
        je     loc_2_2B22
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2B17:
        test   dword ptr [esi], edx
        je     loc_2_2B1C
        inc    eax
loc_2_2B1C:
        add    esi, 4
        dec    edi
        jne    loc_2_2B17
loc_2_2B22:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2B30:
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
        jbe    loc_2_2BAC
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_2B70:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_2B9A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_2BC6
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_1DDC
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_2B9A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_2B70
loc_2_2BAC:
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
loc_2_2BC6:
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
sub_2_2BF0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_2C20
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2C20:
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
        call   sub_1_1DDC
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2C50:
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
        je     loc_2_2CE9
        dec    esi
        cmp    esi, edx
        jb     loc_2_2CE9
        nop    
loc_2_2C70:
        cmp    edx, ecx
        jb     loc_2_2CE9
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_2C93
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2CC7
loc_2_2C93:
        jbe    loc_2_2CA0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_2CC7
        db  08Bh, 0FFh
loc_2_2CA0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_2CB7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2CC7
loc_2_2CB7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_2CC7
        mov    dword ptr [ebp - 4], 0
loc_2_2CC7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2CF9
        jle    loc_2_2CD8
        or     edx, edx
        je     loc_2_2CE9
        lea    esi, [edx - 1]
        jmp    loc_2_2CDB
loc_2_2CD8:
        lea    ecx, [edx + 1]
loc_2_2CDB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_2C70
loc_2_2CE9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2CF9:
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0xd0  R-X
loc_3_0000:
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
loc_3_0078:
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
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS
