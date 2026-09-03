; ELNK3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7979 instructions recovered by recursive descent

Declare_Virtual_Device ELNK3, 3, 10, ELNK3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x58e4  R-X
loc_1_0000:
        mov    al, byte ptr [0x5568]         ; -> obj1:0x5568
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x5568]             ; -> obj1:0x5568
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_1DB8
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
ELNK3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0038
        jmp    dword ptr [eax*4 + 0x3a]      ; -> obj1:0x003A
loc_1_0038:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0000
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0000
        db  000h, 000h
sub_1_00A8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 0x160]
        in     al, dx
        test   al, 1
        je     loc_1_00D4
        mov    edx, dword ptr [ecx + 0x15c]
        mov    eax, 0x7000
        out    dx, ax
        mov    eax, 0x6801
        pop    ebp
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_00D4:
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_00E8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0xbb], 1
        mov    edx, dword ptr [esi + 0x168]
        in     al, dx
        mov    di, ax
        and    edi, 0xffff00ff
loc_1_0107:
        mov    edx, dword ptr [esi + 0x160]
        in     ax, dx
        and    eax, 0xfe
        je     loc_1_0123
        push   esi
        and    eax, 0xfffffffe
        call   dword ptr [esi + eax*2 + 0x234]
        jmp    loc_1_0107
loc_1_0123:
        mov    dx, word ptr [esi + 0x18a]
        mov    ecx, edi
        add    dx, di
        and    ecx, 0xffff
        imul   ecx, ecx
        mov    ax, word ptr [esi + 0x188]
        add    ecx, dword ptr [esi + 0x18c]
        inc    ax
        mov    dword ptr [esi + 0x18c], ecx
        mov    word ptr [esi + 0x18a], dx
        mov    word ptr [esi + 0x188], ax
        cmp    ax, 0x100
        jb     loc_1_017E
        mov    word ptr [esi + 0x188], 0x80
        shr    ecx, 1
        shr    dx, 1
        mov    dword ptr [esi + 0x18c], ecx
        mov    word ptr [esi + 0x18a], dx
loc_1_017E:
        mov    byte ptr [esi + 0xbb], 0
        cmp    byte ptr [esi + 0xb1], 0
        je     loc_1_01B1
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x70fe
        mov    byte ptr [esi + 0xb1], 0
        out    dx, ax
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_5160
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_01B1:
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x70fe
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_01C8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xb0], 0
        je     loc_1_01E5
        mov    byte ptr [esi + 0xb0], 0
        push   esi
        call   sub_1_1A78
loc_1_01E5:
        lea    ecx, [esi + 0x234]
        xor    eax, eax
loc_1_01ED:
        test   al, 2
        je     loc_1_01F9
        mov    dword ptr [ecx], 0            ; -> loc_1_0458
        jmp    loc_1_0245
loc_1_01F9:
        test   al, 0x80
        je     loc_1_0205
        mov    dword ptr [ecx], 0            ; -> sub_1_0268
        jmp    loc_1_0245
loc_1_0205:
        test   al, 0x20
        je     loc_1_0211
        mov    dword ptr [ecx], 0            ; -> loc_1_1358
        jmp    loc_1_0245
loc_1_0211:
        test   al, 0x10
        je     loc_1_021D
        mov    dword ptr [ecx], 0            ; -> loc_1_1668
        jmp    loc_1_0245
loc_1_021D:
        test   al, 4
        je     loc_1_0229
        mov    dword ptr [ecx], 0            ; -> loc_1_0FC8
        jmp    loc_1_0245
loc_1_0229:
        test   al, 8
        je     loc_1_0235
        mov    dword ptr [ecx], 0            ; -> loc_1_1118
        jmp    loc_1_0245
loc_1_0235:
        test   al, 0x40
        mov    dword ptr [ecx], 0            ; -> loc_1_03F8
        jne    loc_1_0245
        mov    dword ptr [ecx], 0            ; -> loc_1_0528
loc_1_0245:
        add    ecx, 4
        add    eax, 2
        cmp    eax, 0x100
        jl     loc_1_01ED
        mov    dword ptr [esi + 0x210], 0    ; -> loc_1_17C8
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0268:
        push   ebp
        mov    eax, 0x806
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edi, dword ptr [ecx + 0x158]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        lea    edx, [edi + 1]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1d0], edx
        lea    edx, [edi + 2]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1d8], edx
        lea    edx, [edi + 3]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1d4], edx
        lea    edx, [edi + 5]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x200], edx
        lea    edx, [edi + 7]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1bc], edx
        lea    edx, [edi + 0xc]
        in     ax, dx
        mov    edx, eax
        and    edx, 0xffff
        add    dword ptr [ecx + 0x1c4], edx
        lea    edx, [edi + 0xa]
        in     ax, dx
        mov    edx, eax
        and    edx, 0xffff
        add    dword ptr [ecx + 0x1c8], edx
        lea    edx, [edi + 6]
        in     al, dx
        and    eax, 0xff
        add    dword ptr [ecx + 0x1b8], eax
        cmp    byte ptr [ecx + 0xb2], 0
        je     loc_1_0316
        add    dword ptr [ecx + 0x1d0], eax
loc_1_0316:
        mov    edx, esi
        mov    eax, 0x801
        cmp    dword ptr [ecx + 0xc0], 1
        out    dx, ax
        jne    loc_1_0344
        mov    edx, dword ptr [ecx + 0x164]
        in     al, dx
        test   al, 0x80
        je     loc_1_0344
        mov    eax, dword ptr [ecx + 0xc4]
        mov    edx, esi
        and    eax, 0xff7f
        out    dx, ax
        jmp    loc_1_0377
loc_1_0344:
        mov    edx, esi
        mov    eax, 0x806
        out    dx, ax
        mov    edx, edi
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1e0], edx
        lea    edx, [edi + 4]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1e4], edx
        lea    edx, [edi + 8]
        in     al, dx
        and    eax, 0xff
        add    dword ptr [ecx + 0x1e8], eax
loc_1_0377:
        mov    edx, esi
        mov    eax, 0xa800
        out    dx, ax
        mov    eax, 0x801
        pop    edi
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0398:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0xc0], 1
        ja     loc_1_03E9
        mov    esi, dword ptr [edi + 0x15c]
        mov    eax, 0x806
        mov    edx, esi
        mov    ecx, dword ptr [edi + 0x158]
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, ecx
        in     al, dx
        lea    edx, [ecx + 4]
        in     al, dx
        lea    edx, [ecx + 8]
        in     al, dx
        mov    edx, esi
        mov    eax, 0xa800
        out    dx, ax
        mov    eax, 0x801
        out    dx, ax
        mov    ax, word ptr [edi + 0xc4]
        out    dx, ax
loc_1_03E9:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03F8:
        push   ebp
        mov    eax, 0x6840
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0xb8], 1
        mov    edx, dword ptr [esi + 0x15c]
        cmp    byte ptr [esi + 0xb9], 0
        out    dx, ax
        je     loc_1_0445
        push   esi
        call   sub_1_1B58
        mov    byte ptr [esi + 0xb0], 0
        mov    byte ptr [esi + 0xb9], 0
        push   esi
        call   sub_1_3FC8
        mov    ax, word ptr [esi + 0xc4]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
loc_1_0445:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0458:
        push   ebp
        mov    eax, 0x804
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x15c]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [esi + 0x158]
        add    edx, 4
        in     ax, dx
        mov    cx, ax
        mov    edx, edi
        test   ch, 4
        mov    eax, 0x801
        out    dx, ax
        je     loc_1_04C8
        push   esi
        call   sub_1_12B8
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        cmp    dword ptr [esi + 0xf8], 0
        out    dx, ax
        je     loc_1_051C
        mov    eax, dword ptr [esi + 0xfc]
        pop    edi
        mov    byte ptr [esi + 0x110], 0
        mov    dword ptr [esi + 0x100], eax
        pop    esi
        sub    ax, 0x7000
        pop    ebp
        out    dx, ax
        ret    4
loc_1_04C8:
        test   ch, 0x20
        je     loc_1_04F2
        cmp    dword ptr [esi + 0x128], 2
        je     loc_1_04F2
        cmp    dword ptr [esi + 0xc0], 1
        jne    loc_1_04F2
        push   esi
        call   sub_1_1B58
        mov    byte ptr [esi + 0xb0], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_04F2:
        mov    edx, edi
        mov    eax, 0x2800
        out    dx, ax
        mov    ecx, dword ptr [esi + 0x160]
        mov    edx, ecx
loc_1_0503:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_0503
        mov    edx, edi
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_0515:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_0515
loc_1_051C:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0528:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0538:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esi]
        push   0
        push   0
        push   ebx
        call   sub_1_5400
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x28], esi
        mov    word ptr [ebx + 0x2c], cx
        cmp    ecx, 0xe
        mov    byte ptr [ebx + 0x2e], 0
        jb     loc_1_06B6
        cmp    dword ptr [edi + 0x13c], ecx
        jb     loc_1_06B6
        cmp    byte ptr [esi + 0x20], 0
        je     loc_1_0590
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0590:
        cmp    byte ptr [edi + 0x83], 0
        je     loc_1_05A7
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_05A7:
        inc    dword ptr [esi + 0x1c]
        test   byte ptr [edi + 0x21], 0xa
        mov    dword ptr [ebx + 0x24], 0
        je     loc_1_05D9
        cmp    dword ptr [edi + 0x15], 0
        jne    loc_1_05C2
        mov    dword ptr [edi + 0x15], ebx
        jmp    loc_1_05C8
loc_1_05C2:
        mov    eax, dword ptr [edi + 0x11]
        mov    dword ptr [eax + 0x24], ebx
loc_1_05C8:
        mov    eax, 0x103
        mov    dword ptr [edi + 0x11], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_05D9:
        mov    dword ptr [edi + 0x15], ebx
        mov    dword ptr [edi + 0x11], ebx
        push   ebx
        or     byte ptr [edi + 0x21], 8
        mov    eax, dword ptr [edi + 0xc4]
        push   edi
        and    eax, 0xfffffffb
        mov    dword ptr [edi + 0xc4], eax
        mov    edx, dword ptr [edi + 0x15c]
        out    dx, ax
        call   sub_1_0728
        test   eax, eax
        je     loc_1_062A
        mov    eax, dword ptr [edi + 0xc4]
        or     eax, 4
        mov    dword ptr [edi + 0xc4], eax
        mov    edx, dword ptr [edi + 0x15c]
        pop    edi
        out    dx, ax
        mov    eax, 0x103
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_062A:
        mov    eax, dword ptr [edi + 0x15]
        mov    ebx, dword ptr [eax + 0x24]
        mov    dword ptr [edi + 0x15], ebx
        test   ebx, ebx
        je     loc_1_0650
loc_1_0637:
        push   ebx
        push   edi
        call   sub_1_0728
        test   eax, eax
        jne    loc_1_0685
        push   ebx
        push   edi
        call   sub_1_06C8
        mov    ebx, dword ptr [edi + 0x15]
        test   ebx, ebx
        jne    loc_1_0637
loc_1_0650:
        and    byte ptr [edi + 0x21], 0xf7
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_0663
        push   esi
        call   sub_1_3F18
loc_1_0663:
        mov    eax, dword ptr [edi + 0xc4]
        or     eax, 4
        mov    dword ptr [edi + 0xc4], eax
        mov    edx, dword ptr [edi + 0x15c]
        pop    edi
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0685:
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_0694
        push   esi
        call   sub_1_3F18
loc_1_0694:
        mov    eax, dword ptr [edi + 0xc4]
        or     eax, 4
        mov    dword ptr [edi + 0xc4], eax
        mov    edx, dword ptr [edi + 0x15c]
        pop    edi
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_06B6:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_06C8:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, byte ptr [edx + 0x2e]
        test   al, 2
        jne    loc_1_06DC
        xor    edi, edi
        jmp    loc_1_06EE
loc_1_06DC:
        and    al, 8
        cmp    al, 1
        sbb    edi, edi
        and    edi, 0xfffefff5
        sub    edi, 0x3ffefff4
loc_1_06EE:
        mov    ecx, dword ptr [ebp + 8]
        push   edi
        push   edx
        mov    eax, dword ptr [ecx + 0x15]
        mov    esi, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x15], esi
        mov    esi, dword ptr [edx + 0x28]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_531A
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_0717
        push   esi
        call   sub_1_3F18
loc_1_0717:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0728:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [eax + 0x17c]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [ebp - 0x18], ecx
        xor    edi, edi
        mov    eax, dword ptr [edx + 8]
        mov    di, word ptr [edx + 0x2c]
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        push   eax
        call   sub_1_5424
        cmp    dword ptr [ebp - 8], 6
        jl     loc_1_07B3
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x82], 0
        jne    loc_1_0787
        mov    eax, dword ptr [ebp - 0xc]
        test   byte ptr [eax], 1
        jne    loc_1_0787
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x40]
        mov    eax, 0
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_0799
loc_1_0787:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_40F8
        and    eax, 0xff
loc_1_0799:
        test   eax, eax
        je     loc_1_07B3
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        inc    dword ptr [eax + 0x1c0]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1888
loc_1_07B3:
        mov    eax, dword ptr [ebp + 8]
        and    dword ptr [eax + 0xc4], 0xfffffffb
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        and    eax, 0xffff
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, 8
        jge    loc_1_084B
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        or     dword ptr [ecx + 0xc4], 4
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
loc_1_084B:
        cli    
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, edi
        out    dx, eax
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 0x10], 4
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jl     loc_1_086A
loc_1_0868:
        mov    eax, ecx
loc_1_086A:
        mov    dword ptr [ebp - 4], eax
loc_1_086D:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     loc_1_08B9
        sub    ebx, eax
        sub    dword ptr [ebp - 4], eax
        push   esi
        mov    esi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    eax, [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_0918
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_5424
        jmp    loc_1_086D
loc_1_08B9:
        push   esi
        mov    esi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0xc], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        cli    
        cmp    dword ptr [ebp - 0x10], 8
        jl     loc_1_09EF
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0xcc]
        cmp    eax, ecx
        jl     loc_1_086A
        jmp    loc_1_0868
loc_1_0918:
        neg    edi
        and    edi, 3
        mov    dword ptr [ebp - 0x10], edi
        je     loc_1_0931
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x10]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_0931:
        sti    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    edx, dword ptr [esi + 0x15c]
        add    esi, 0x15c
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x178]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, 0x801
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0xf], 0x20
        out    dx, ax
        je     loc_1_09C8
        inc    dword ptr [ecx + 0x1cc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x164]
        mov    edx, ecx
loc_1_097B:
        in     al, dx
        test   al, 0x80
        je     loc_1_097B
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0398
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1198
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_1_0A88
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x19c]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
loc_1_09C8:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        or     dword ptr [eax + 0xc4], 4
        mov    eax, dword ptr [ebp + 8]
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        mov    esp, ebp
        out    dx, ax
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_09EF:
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0x1aa]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x104], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x108], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10c], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebx - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        or     dword ptr [ecx + 0xc4], 4
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_0A88:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [eax + 0x17c]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [ebp - 0x18], ecx
        xor    edi, edi
        mov    di, word ptr [edx + 0x2c]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebp - 0xc], ecx
        cmp    ecx, 8
        jge    loc_1_0B06
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 0x7000]
        pop    edi
        pop    esi
        mov    edx, dword ptr [ecx + 0x15c]
        pop    ebx
        out    dx, ax
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0B06:
        cli    
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, edi
        out    dx, eax
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 0xc], 4
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jl     loc_1_0B25
        mov    eax, ecx
loc_1_0B25:
        mov    ecx, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 8]
        push   edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 8]
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [ebp - 0x10], eax
        push   eax
loc_1_0B3A:
        call   sub_1_5424
loc_1_0B3F:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     loc_1_0B86
        sub    ebx, eax
        sub    dword ptr [ebp - 4], eax
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    eax, [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_0BE6
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edx
        jmp    loc_1_0B3A
loc_1_0B86:
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0x14], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0xc], eax
        cli    
        cmp    dword ptr [ebp - 0xc], 8
        mov    eax, dword ptr [ebp + 8]
        jl     loc_1_0CA1
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jl     loc_1_0BDE
        mov    eax, ecx
loc_1_0BDE:
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0B3F
loc_1_0BE6:
        neg    edi
        and    edi, 3
        mov    dword ptr [ebp - 0xc], edi
        je     loc_1_0BFF
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_0BFF:
        sti    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    edx, dword ptr [esi + 0x15c]
        add    esi, 0x15c
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x178]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, 0x801
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0xb], 0x20
        out    dx, ax
        je     loc_1_0C96
        inc    dword ptr [ecx + 0x1cc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x164]
        mov    edx, ecx
loc_1_0C49:
        in     al, dx
        test   al, 0x80
        je     loc_1_0C49
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0398
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1198
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_1_0A88
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x19c]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
loc_1_0C96:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0CA1:
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0x1aa]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x104], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x108], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10c], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebx - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D28:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [eax + 0x17c]
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], ecx
        mov    ecx, dword ptr [eax + 0xf8]
        cli    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        mov    edx, eax
        mov    eax, dword ptr [ebp + 8]
        and    edx, 0xffff
        mov    dword ptr [ebp - 0x10], edx
        cmp    byte ptr [eax + 0x110], 0
        jne    loc_1_0DB4
        cmp    edx, 8
        jge    loc_1_0D8D
        mov    edx, dword ptr [eax + 0x15c]
        mov    ecx, eax
        mov    eax, dword ptr [eax + 0xfc]
        sub    ax, 0x7000
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0D8D:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xfc]
        out    dx, eax
        lea    edx, [ebp - 8]
        lea    eax, [ebp - 0x14]
        sub    dword ptr [ebp - 0x10], 4
        mov    ecx, dword ptr [ecx + 8]
        push   edx
        push   eax
        mov    dword ptr [ebp - 0xc], ecx
        push   ecx
        call   sub_1_5424
        jmp    loc_1_0DD2
loc_1_0DB4:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x104]
        mov    dword ptr [ebp - 0xc], ecx
        mov    edx, dword ptr [eax + 0x108]
        mov    dword ptr [ebp - 0x14], edx
        mov    ecx, dword ptr [eax + 0x10c]
        mov    dword ptr [ebp - 8], ecx
loc_1_0DD2:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edi, dword ptr [eax + 0x100]
        mov    eax, dword ptr [eax + 0xcc]
        cmp    eax, ecx
        jl     loc_1_0DEA
loc_1_0DE8:
        mov    eax, ecx
loc_1_0DEA:
        mov    dword ptr [ebp - 4], eax
loc_1_0DED:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     loc_1_0E3C
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 0xc]
        sub    edi, eax
        push   ecx
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_5448
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_0E9B
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_5424
        jmp    loc_1_0DED
loc_1_0E3C:
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0x14], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        cli    
        cmp    dword ptr [ebp - 0x10], 8
        mov    eax, dword ptr [ebp + 8]
        jl     loc_1_0F61
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jl     loc_1_0DEA
        jmp    loc_1_0DE8
loc_1_0E9B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfc]
        neg    eax
        and    eax, 3
        mov    dword ptr [ebp - 0x10], eax
        je     loc_1_0EBD
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x10]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_0EBD:
        sti    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    edx, dword ptr [esi + 0x15c]
        add    esi, 0x15c
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x178]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, 0x801
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0xf], 0x20
        out    dx, ax
        je     loc_1_0F57
        inc    dword ptr [ecx + 0x1cc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x164]
        mov    edx, ecx
loc_1_0F07:
        in     al, dx
        test   al, 0x80
        je     loc_1_0F07
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0398
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1198
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xf8]
        push   edx
        push   ecx
        call   sub_1_0A88
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x19c]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
loc_1_0F57:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0F61:
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0x1aa]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x104], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x108], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10c], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0FC8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x164]
        in     al, dx
        out    dx, al
        test   al, 0x10
        je     loc_1_1048
        push   esi
        inc    dword ptr [esi + 0x1cc]
        call   sub_1_12B8
        push   esi
        call   sub_1_0398
        cmp    dword ptr [esi + 0xf8], 0
        je     loc_1_102A
        lea    eax, [esi + 0x1aa]
        push   eax
        push   esi
        call   sub_1_1198
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        push   esi
        call   sub_1_0A88
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        push   esi
        call   sub_1_06C8
        mov    dword ptr [esi + 0xf8], 0
        jmp    loc_1_1037
loc_1_102A:
        lea    eax, [esi + 0x19c]
        push   eax
        push   esi
        call   sub_1_1198
loc_1_1037:
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        jmp    loc_1_1099
loc_1_1048:
        test   al, 0x20
        je     loc_1_10D0
        push   esi
        inc    dword ptr [esi + 0x1ec]
        call   sub_1_12B8
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        push   esi
        call   sub_1_0398
        mov    eax, dword ptr [esi + 0xf8]
        test   eax, eax
        je     loc_1_1099
        push   eax
        push   esi
        call   sub_1_0A88
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        push   esi
        call   sub_1_06C8
        mov    dword ptr [esi + 0xf8], 0
loc_1_1099:
        mov    edi, dword ptr [esi + 0x15]
        test   edi, edi
        jne    loc_1_10A8
        and    byte ptr [esi + 0x21], 0xf7
        test   edi, edi
        je     loc_1_110C
loc_1_10A8:
        push   edi
        push   esi
        call   sub_1_0A88
        cmp    eax, 1
        je     loc_1_110C
        push   edi
        push   esi
        call   sub_1_06C8
        mov    edi, dword ptr [esi + 0x15]
        test   edi, edi
        jne    loc_1_10A8
        and    byte ptr [esi + 0x21], 0xf7
        test   edi, edi
        jne    loc_1_10A8
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_10D0:
        test   al, 8
        je     loc_1_10F3
        push   esi
        inc    dword ptr [esi + 0x1dc]
        call   sub_1_0398
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x4800
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_10F3:
        push   esi
        inc    dword ptr [esi + 0x1f0]
        call   sub_1_0398
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x4800
        out    dx, ax
loc_1_110C:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1118:
        push   ebp
        mov    eax, 0x6808
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    edx, dword ptr [edi + 0x15c]
        out    dx, ax
        mov    ax, word ptr [edi + 0x19c]
        out    dx, ax
        call   sub_1_0D28
        cmp    eax, 1
        je     loc_1_1189
        mov    eax, dword ptr [edi + 0xf8]
        push   eax
        push   edi
        call   sub_1_06C8
        mov    dword ptr [edi + 0xf8], 0
        mov    esi, dword ptr [edi + 0x15]
        test   esi, esi
        jne    loc_1_1167
        and    byte ptr [edi + 0x21], 0xf7
        test   esi, esi
        je     loc_1_1189
loc_1_1167:
        push   esi
        push   edi
        call   sub_1_0A88
        cmp    eax, 1
        je     loc_1_1189
        push   esi
        push   edi
        call   sub_1_06C8
        mov    esi, dword ptr [edi + 0x15]
        test   esi, esi
        jne    loc_1_1167
        and    byte ptr [edi + 0x21], 0xf7
        test   esi, esi
        jne    loc_1_1167
loc_1_1189:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1198:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1268
        mov    dx, ax
        mov    ecx, dword ptr [ebp + 0xc]
        sub    dx, word ptr [ecx + 8]
        mov    word ptr [ecx + 8], ax
        mov    edi, edx
        mov    eax, dword ptr [esi + 0x198]
        and    edi, 0xffff
        cmp    edi, eax
        jbe    loc_1_11CA
        mov    dx, ax
loc_1_11CA:
        mov    di, word ptr [ecx + 0xa]
        inc    di
        mov    ax, word ptr [ecx + 0xc]
        add    ax, dx
        mov    word ptr [ecx + 0xa], di
        mov    word ptr [ecx + 0xc], ax
        cmp    di, 4
        jb     loc_1_11F5
        shr    di, 1
        shr    ax, 1
        mov    word ptr [ecx + 0xa], di
        mov    word ptr [ecx + 0xc], ax
loc_1_11F5:
        mov    ax, word ptr [ecx + 0xc]
        mov    edi, dword ptr [ecx]
        sub    dx, dx
        and    edi, 0x7ff
        div    word ptr [ecx + 0xa]
        mov    esi, dword ptr [esi + 0x194]
        and    eax, 0xffff
        mov    ebx, esi
        sub    ebx, eax
        imul   ebx, edi
        mov    eax, ebx
        cdq    
        idiv   esi
        mov    esi, eax
        test   esi, esi
        jl     loc_1_123C
        cmp    esi, edi
        jle    loc_1_122B
        mov    esi, edi
loc_1_122B:
        add    edi, esi
        cmp    edi, 0x5ea
        jle    loc_1_1257
        mov    edi, 0x5ea
        jmp    loc_1_1257
loc_1_123C:
        neg    esi
        mov    eax, edi
        cdq    
        sub    eax, edx
        sar    eax, 1
        cmp    esi, eax
        jle    loc_1_124C
        mov    esi, eax
loc_1_124C:
        sub    edi, esi
        mov    eax, dword ptr [ecx + 4]
        cmp    edi, eax
        jge    loc_1_1257
        mov    edi, eax
loc_1_1257:
        add    edi, 0x9800
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_1268:
        push   ebp
        mov    eax, 0x806
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x158]
        add    edx, 6
        in     al, dx
        and    eax, 0xff
        mov    edx, esi
        pop    esi
        add    dword ptr [ecx + 0x1b8], eax
        mov    eax, 0xa800
        pop    ebp
        out    dx, ax
        mov    eax, 0x801
        out    dx, ax
        mov    eax, dword ptr [ecx + 0x1b8]
        ret    4
        db  08Dh, 049h, 000h
sub_1_12B8:
        push   ebp
        mov    eax, 0x5800
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x160]
loc_1_12D4:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_12D4
        mov    edx, esi
        mov    eax, 0x4800
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12F8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [eax]
        mov    esi, dword ptr [ebp + 0x18]
        push   esi
        mov    dword ptr [esi + 0x28], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [esi + 0x2e], ax
        mov    eax, dword ptr [ebp + 0x10]
        add    ax, 0xe
        mov    word ptr [esi + 0x2c], ax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        call   dword ptr [eax + 0xc]
        test   eax, eax
        je     loc_1_1336
        cmp    eax, 1
        je     loc_1_1345
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x18
loc_1_1336:
        mov    ecx, dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp + 0x1c]
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        xor    eax, eax
        ret    0x18
loc_1_1345:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1358:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x17c]
        mov    ecx, dword ptr [esi + 0x15c]
        mov    edx, ecx
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, 0x6820
        mov    dword ptr [ebp - 0x2c], ecx
        cmp    byte ptr [esi + 0xb0], 0
        out    dx, ax
        je     loc_1_148B
        mov    edx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0xd4]
        mov    ecx, dword ptr [esi + 0x208]
        mov    byte ptr [esi + 0xb0], 0
        out    dx, ax
        mov    edx, dword ptr [esi + 0x170]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [esi + 0x22c]
        mov    dword ptr [ebp - 0x24], edx
        in     ax, dx
        add    ecx, dword ptr [ebp - 8]
        and    eax, 0x7fc
        mov    ebx, eax
        mov    dword ptr [ebp - 4], ecx
loc_1_13C7:
        add    dword ptr [ebp - 8], ebx
        mov    dword ptr [ebp - 0x1c], ebx
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x1c]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    edx, dword ptr [ebp - 0x24]
        add    dword ptr [ebp - 4], ebx
        in     ax, dx
        mov    ebx, eax
        and    ebx, 0xffff
        test   bh, 0x80
        je     loc_1_13FC
        and    ebx, 0x7fc
        jmp    loc_1_13C7
loc_1_13FC:
        test   bh, 0x40
        je     loc_1_141D
        and    ebx, 0x7ff
        mov    esi, dword ptr [ebp + 8]
        add    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_19B8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_141D:
        and    ebx, 0x7ff
        lea    eax, [ebx + 3]
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x34]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    esi, dword ptr [ebp + 8]
        add    dword ptr [ebp - 8], ebx
        mov    edi, dword ptr [ebp - 8]
        lea    edx, [esi + 0x204]
        mov    dword ptr [esi + 0x20c], edi
        mov    dword ptr [esi + 0x208], edi
        lea    eax, [edi - 0xe]
        mov    ecx, dword ptr [edx]
        push   eax
        sub    edi, 0xe
        push   edi
        lea    ebx, [ecx + 0xe]
        push   ebx
        mov    eax, dword ptr [esi + 0x5e]
        mov    byte ptr [esi + 0xb1], 1
        push   0xe
        push   ecx
        push   ecx
        push   edx
        push   eax
        call   sub_1_4F80
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1A78
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_148B:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xbc]
        mov    ecx, dword ptr [esi + 0x22c]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [esi + 0x170]
        mov    dword ptr [ebp - 0x28], ecx
        mov    edx, eax
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 0x20], eax
        in     ax, dx
        and    eax, 0x7fc
        cmp    dword ptr [ebp - 8], 0
        mov    ebx, eax
        je     loc_1_1508
loc_1_14C0:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, ebx
        jl     loc_1_14C9
        mov    eax, ebx
loc_1_14C9:
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x20]
        add    dword ptr [ebp - 4], eax
        sub    dword ptr [ebp - 0x10], eax
        in     ax, dx
        mov    ebx, eax
        and    ebx, 0xffff
        test   bh, 0x80
        je     loc_1_1543
        and    ebx, 0x7fc
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_14C0
loc_1_1508:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x28]
        mov    dword ptr [esi + 0x208], eax
        mov    cx, word ptr [edx + 0xc]
        xor    eax, eax
        mov    al, cl
        shl    eax, 8
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        add    eax, edx
        cmp    eax, 0x600
        jge    loc_1_15DE
        add    eax, 0xe
        jmp    loc_1_1611
loc_1_1543:
        test   bh, 0x40
        je     loc_1_1567
        and    ebx, 0x7ff
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, dword ptr [ebp - 0x10]
        add    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_19B8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1567:
        and    ebx, 0x7ff
        lea    eax, [ebx + 3]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x30]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    esi, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 8], eax
        lea    edx, [esi + 0x204]
        mov    byte ptr [esi + 0xb1], 1
        add    dword ptr [ebp - 8], ebx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x208], ecx
        mov    dword ptr [esi + 0x20c], ecx
        lea    eax, [ecx - 0xe]
        mov    esi, dword ptr [edx]
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    ebx, [esi + 0xe]
        push   ebx
        push   0xe
        push   esi
        push   esi
        push   edx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_4F80
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1A78
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15DE:
        je     loc_1_15F3
        cmp    eax, 0x800
        je     loc_1_15F3
        cmp    eax, 0x8137
        mov    eax, 0x5ea
        jne    loc_1_1611
loc_1_15F3:
        mov    eax, dword ptr [ebp - 0x28]
        mov    cx, word ptr [eax + 0x10]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        shl    eax, 8
        lea    eax, [eax + edx + 0xe]
loc_1_1611:
        cmp    eax, 0x5ea
        jg     loc_1_161D
        cmp    dword ptr [ebp - 8], eax
        jle    loc_1_1622
loc_1_161D:
        mov    eax, 0x5ea
loc_1_1622:
        mov    ecx, eax
        mov    esi, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 8]
        sar    ecx, 1
        imul   ecx, dword ptr [esi + 0xd0]
        sar    ecx, 5
        sub    eax, ecx
        sub    eax, dword ptr [esi + 0x190]
        jns    loc_1_1643
        xor    eax, eax
loc_1_1643:
        sub    ax, 0x7800
        mov    esi, dword ptr [ebp + 8]
        and    eax, 0xffff
        mov    edx, dword ptr [ebp - 0x2c]
        mov    byte ptr [esi + 0xb0], 1
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1668:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x17c]
        mov    edx, dword ptr [ebx + 0x170]
        mov    dword ptr [ebp - 0x10], eax
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   ch, 0x40
        mov    dword ptr [ebp - 4], ecx
        je     loc_1_16D7
        and    dword ptr [ebp - 4], 0x7ff
        cmp    byte ptr [ebx + 0xb0], 0
        je     loc_1_16AE
        mov    eax, dword ptr [ebx + 0x208]
        add    dword ptr [ebp - 4], eax
loc_1_16AE:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_19B8
        mov    ax, word ptr [ebx + 0xd4]
        mov    edx, dword ptr [ebx + 0x15c]
        mov    byte ptr [ebx + 0xb0], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_16D7:
        and    dword ptr [ebp - 4], 0x7ff
        cmp    byte ptr [ebx + 0xb0], 0
        je     loc_1_175A
        mov    ax, word ptr [ebx + 0xd4]
        mov    edx, dword ptr [ebx + 0x15c]
        mov    byte ptr [ebx + 0xb0], 0
        out    dx, ax
        mov    eax, dword ptr [ebx + 0x208]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 0x22c]
        mov    byte ptr [ebx + 0xb1], 1
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [ecx + 3]
        mov    dword ptr [ebx + 0x20c], edx
        mov    dword ptr [ebx + 0x208], edx
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x18]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        lea    edx, [ebx + 0x204]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0xe
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_17A3
loc_1_175A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 0x22c]
        mov    dword ptr [ebx + 0x20c], eax
        mov    dword ptr [ebx + 0x208], eax
        add    eax, 3
        mov    dword ptr [ebp - 0xc], ecx
        mov    byte ptr [ebx + 0xb1], 1
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x14]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        lea    edx, [ebx + 0x204]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebp - 4]
loc_1_17A3:
        sub    eax, 0xe
        push   eax
        lea    eax, [ecx + 0xe]
        push   eax
        push   0xe
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebx + 0x5e]
        push   ecx
        call   sub_1_4F80
        push   ebx
        call   sub_1_1A78
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17C8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [eax + 0x22c]
        mov    cx, word ptr [esi + 0x2c]
        mov    eax, dword ptr [ebp + 0xc]
        add    edi, ecx
        mov    ebx, dword ptr [eax + 8]
        sub    ebx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0x2e]
        cmp    ebx, eax
        jbe    loc_1_17F9
        mov    ebx, eax
loc_1_17F9:
        mov    eax, dword ptr [esi + 8]
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        mov    dword ptr [esi + 0x30], ebx
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_1_5424
        test   ebx, ebx
        je     loc_1_1879
loc_1_1814:
        cmp    ebx, dword ptr [ebp - 4]
        jbe    loc_1_1860
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        push   ecx
        call   sub_1_54BC
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1876
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5424
        test   ebx, ebx
        jne    loc_1_1814
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1860:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        push   eax
        call   sub_1_54BC
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1876:
        sub    dword ptr [esi + 0x30], ebx
loc_1_1879:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_1888:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [eax + 0x2c]
        mov    eax, dword ptr [esi]
        add    eax, 0xf
        mov    dword ptr [esi + 0x220], edi
        and    eax, 0xfffffff0
        cmp    eax, edi
        jae    loc_1_18B2
        mov    edi, eax
loc_1_18B2:
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 4]
        push   edx
        mov    dword ptr [esi + 0x21c], edi
        mov    ecx, dword ptr [eax + 8]
        lea    eax, [ebp - 0xc]
        push   eax
        mov    dword ptr [ebp - 8], ecx
        push   ecx
        call   sub_1_5424
        mov    ebx, dword ptr [esi + 0x230]
loc_1_18D5:
        cmp    edi, dword ptr [ebp - 4]
        jb     loc_1_1914
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_54BC
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_191F
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5424
        jmp    loc_1_18D5
loc_1_1914:
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   ebx
        call   sub_1_54BC
loc_1_191F:
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [esi + 0x218]
        mov    dword ptr [esi + 0x228], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 0x220]
        lea    ebx, [ecx + 0xe]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [esi + 0x21c]
        sub    eax, 0xe
        push   eax
        push   ebx
        push   0xe
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 0x5e]
        push   ecx
        call   sub_1_4F80
        cmp    byte ptr [esi + 0xbb], 0
        je     loc_1_196F
        mov    byte ptr [esi + 0xb1], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_196F:
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_5160
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1988:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0x10]
        mov    dx, word ptr [ecx + 0x2c]
        lea    eax, [ecx + 0x30]
        push   eax
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x10]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x2e]
        push   edx
        push   eax
        push   0
        push   ecx
        call   sub_1_544E
        xor    eax, eax
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_19B8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 0x170]
        in     ax, dx
        and    eax, 0x3800
        cmp    eax, 0x1800
        ja     loc_1_19DE
        je     loc_1_19F6
        cmp    eax, 0x800
        je     loc_1_19EE
        jmp    loc_1_1A0C
loc_1_19DE:
        cmp    eax, 0x2000
        je     loc_1_19FE
        cmp    eax, 0x2800
        je     loc_1_1A06
        jmp    loc_1_1A0C
loc_1_19EE:
        inc    dword ptr [ecx + 0x1fc]
        jmp    loc_1_1A0C
loc_1_19F6:
        inc    dword ptr [ecx + 0xec]
        jmp    loc_1_1A0C
loc_1_19FE:
        inc    dword ptr [ecx + 0x1f4]
        jmp    loc_1_1A0C
loc_1_1A06:
        inc    dword ptr [ecx + 0x1f8]
loc_1_1A0C:
        test   eax, eax
        je     loc_1_1A26
        cmp    eax, 0x1800
        je     loc_1_1A26
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [ecx + 0x1bc]
        sub    dword ptr [ecx + 0x1c8], eax
loc_1_1A26:
        push   ecx
        call   sub_1_1A78
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1A38:
        push   ebp
        mov    cl, 0xf5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0xbc]
        sub    cl, byte ptr [edx + 0x190]
        shr    esi, 1
        mov    eax, dword ptr [edx + 0xd8]
        imul   esi, dword ptr [edx + 0xd0]
        shr    esi, cl
        cmp    esi, eax
        jge    loc_1_1A65
        mov    esi, eax
loc_1_1A65:
        add    esi, 0x8800
        mov    dword ptr [edx + 0xd4], esi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1A78:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 0x4000
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [edi + 0x160]
loc_1_1A96:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_1A96
        cmp    dword ptr [edi + 0xc0], 1
        ja     loc_1_1B43
        mov    edx, dword ptr [edi + 0x170]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   ch, 0x80
        je     loc_1_1B43
        mov    edx, esi
        mov    eax, 0x803
        out    dx, ax
        mov    edx, dword ptr [edi + 0x174]
        and    ecx, 0x7ff
        in     ax, dx
        mov    ebx, eax
        mov    edx, esi
        and    ebx, 0xffff
        mov    eax, 0x801
        out    dx, ax
        add    ecx, ebx
        add    ecx, 0x64
        cmp    ecx, dword ptr [edi + 0xdc]
        jae    loc_1_1B43
        mov    edx, dword ptr [edi + 0x170]
        in     ax, dx
        mov    ecx, eax
        mov    edx, esi
        and    ecx, 0xffff
        mov    eax, 0x803
        out    dx, ax
        mov    edx, dword ptr [edi + 0x174]
        and    ecx, 0x7ff
        in     ax, dx
        mov    ebx, eax
        mov    edx, esi
        and    ebx, 0xffff
        mov    eax, 0x801
        out    dx, ax
        add    ecx, ebx
        add    ecx, 0x64
        cmp    ecx, dword ptr [edi + 0xdc]
        jae    loc_1_1B43
        push   edi
        call   sub_1_1B58
loc_1_1B43:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B58:
        push   ebp
        mov    eax, 0x2800
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x160]
loc_1_1B74:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_1B74
        mov    ax, word ptr [ecx + 0xf0]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0x2000
        pop    esi
        out    dx, ax
        mov    ax, word ptr [ecx + 0xd4]
        pop    ebp
        out    dx, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0xe4]
        mov    dword ptr [esi + 0xe4], eax
        cmp    eax, 0x3e8
        jbe    loc_1_1BDA
        mov    dword ptr [esi + 0xe4], 0
        push   esi
        call   sub_1_0268
loc_1_1BDA:
        cmp    byte ptr [esi + 0xb2], 0
        jne    loc_1_1C2C
        mov    eax, dword ptr [esi + 0x1d0]
        cmp    eax, 0x3e8
        jbe    loc_1_1C2C
        shl    eax, 2
        cmp    dword ptr [esi + 0x1b8], eax
        jae    loc_1_1C2C
        mov    ecx, dword ptr [esi + 0x15c]
        mov    eax, 0x804
        mov    byte ptr [esi + 0xb2], 1
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x158]
        add    edx, 0xa
        in     ax, dx
        and    eax, 0xfff7
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x801
        out    dx, ax
loc_1_1C2C:
        mov    cx, word ptr [esi + 0x188]
        mov    ax, word ptr [esi + 0x18a]
        mov    ebx, ecx
        sub    dx, dx
        and    ebx, 0xffff
        div    cx
        mov    word ptr [ebp - 4], ax
        sub    edx, edx
        mov    cx, word ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x18c]
        div    ebx
        imul   cx, cx
        sub    ax, cx
        mov    edx, dword ptr [esi + 0x4a]
        mov    cx, ax
        shr    ax, 0xc
        mov    edi, eax
        and    edi, 0xffff
        mov    bl, byte ptr [edx + edi + 0x144]
        and    ebx, 0xffff00ff
        test   bx, bx
        jne    loc_1_1C90
        mov    bx, cx
        shr    bx, 5
        add    bx, 2
loc_1_1C90:
        mov    ax, cx
        sub    dx, dx
        div    bx
        mov    edi, eax
        mov    eax, ebx
        sub    dx, dx
        and    edi, 0xffff
        and    eax, 0xffff
        add    edi, eax
        sar    edi, 1
        mov    ax, cx
        div    di
        mov    ecx, eax
        mov    eax, edi
        and    ecx, 0xffff
        and    eax, 0xffff
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        sar    ecx, 1
        and    eax, 0xffff
        mov    edx, ecx
        and    edx, 0xffff
        lea    ecx, [eax + edx*2]
        mov    eax, dword ptr [esi + 4]
        shl    ecx, 2
        add    eax, dword ptr [esi + 0xe8]
        mov    dword ptr [esi + 0x190], ecx
        mov    dword ptr [esi + 0xe8], eax
        cmp    eax, 0x3e8
        jbe    loc_1_1D20
        mov    dword ptr [esi + 0xe8], 0
        cmp    dword ptr [esi + 0xec], 0x64
        jb     loc_1_1D16
        add    dword ptr [esi + 0xd8], 8
loc_1_1D16:
        mov    dword ptr [esi + 0xec], 0
loc_1_1D20:
        push   esi
        call   sub_1_1A38
        cmp    byte ptr [esi + 0xb0], 0
        jne    loc_1_1D3E
        mov    ax, word ptr [esi + 0xd4]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
loc_1_1D3E:
        push   esi
        xor    edi, edi
        call   sub_1_1268
        and    eax, 0xffff
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_1D51:
        test   edi, edi
        je     loc_1_1D5C
        cmp    edi, 1
        je     loc_1_1D64
        jmp    loc_1_1D6A
loc_1_1D5C:
        lea    ebx, [esi + 0x19c]
        jmp    loc_1_1D6A
loc_1_1D64:
        lea    ebx, [esi + 0x1aa]
loc_1_1D6A:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 8]
        sub    eax, ecx
        cmp    eax, dword ptr [esi + 0x198]
        jl     loc_1_1D84
        push   ebx
        push   esi
        call   sub_1_1198
loc_1_1D84:
        inc    edi
        cmp    edi, 2
        jl     loc_1_1D51
        cmp    dword ptr [esi + 0xf8], 0
        jne    loc_1_1DA2
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
loc_1_1DA2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1DB8:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 0xc], ax
        push   ebx
        mov    word ptr [ebp - 0xa], ax
        push   edi
        mov    edi, 0                        ; -> obj1:0x5790
        xor    eax, eax
        mov    ecx, 0x55
        xor    ebx, ebx
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_5574
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [0], 8               ; -> obj1:0x5764
        mov    dword ptr [4], ebx            ; -> obj1:0x5768
        mov    dword ptr [8], ebx            ; -> obj1:0x576C
        mov    dword ptr [0xc], ebx          ; -> obj1:0x5770
        mov    dword ptr [0x10], ebx         ; -> obj1:0x5774
        mov    dword ptr [0x14], ebx         ; -> obj1:0x5778
        mov    dword ptr [0x18], ebx         ; -> obj1:0x577C
        push   ebx
        mov    dword ptr [0x1c], ebx         ; -> obj1:0x5780
        push   ebx
        mov    dword ptr [0x20], ebx         ; -> obj1:0x5784
        mov    word ptr [2], 4               ; -> obj1:0x5766
        mov    word ptr [0x24], 0x62f7       ; -> obj1:0x5788
        push   0                             ; -> obj1:0x5764
        push   4                             ; -> obj1:0x5794
        call   sub_1_52EA
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0x3e], edx         ; -> loc_1_57CE
        mov    dword ptr [0x42], ecx         ; -> loc_1_57D2
        mov    dword ptr [0x46], ebx         ; -> loc_1_57D6
        mov    byte ptr [8], 3               ; -> obj1:0x5798
        mov    byte ptr [9], bl              ; -> loc_1_5799
        mov    dword ptr [0xa], ebx          ; -> obj1:0x579A
        mov    dword ptr [0xe], 0            ; -> obj1:0x579E
        mov    dword ptr [0x12], 0           ; -> obj1:0x57A2
        mov    dword ptr [0x16], 0           ; -> obj1:0x57A6
        mov    dword ptr [0x1a], 0           ; -> obj1:0x57AA
        mov    dword ptr [0x1e], 0           ; -> obj1:0x57AE
        mov    dword ptr [0x22], 0           ; -> obj1:0x57B2
        mov    dword ptr [0x26], 0           ; -> obj1:0x57B6
        mov    dword ptr [0x2a], 0           ; -> obj1:0x57BA
        mov    dword ptr [0x2e], 0           ; -> obj1:0x57BE
        mov    dword ptr [0x32], 0           ; -> obj1:0x57C2
        mov    eax, dword ptr [ebp + 8]
        push   0x36
        push   8                             ; -> obj1:0x5798
        mov    dword ptr [0x36], edx         ; -> obj1:0x57C6
        mov    edx, dword ptr [ebp + 0xc]
        push   0                             ; -> obj1:0x5790
        mov    dword ptr [0x3a], ecx         ; -> loc_1_57CA
        mov    dword ptr [0x52], eax         ; -> loc_1_57E2
        mov    dword ptr [0x56], edx         ; -> loc_1_57E6
        mov    ecx, dword ptr [4]            ; -> obj1:0x5794
        push   ecx
        push   0                             ; -> obj1:0x5790
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_52F6
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_1F3C
        cmp    dword ptr [0x46], ebx         ; -> loc_1_57D6
        jbe    loc_1_1F24
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F24:
        push   0
        mov    eax, dword ptr [4]            ; -> obj1:0x5794
        push   eax
        call   sub_1_52F0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F3C:
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_1F48:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x5644
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   0x462
        push   ecx
        call   sub_1_53D6
        test   eax, eax
        je     loc_1_1F77
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F77:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x118
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2438
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2648
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_1_2048
        test   eax, eax
        je     loc_1_1FC9
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x462
        push   eax
        call   sub_1_53DC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1FC9:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3338
        test   eax, eax
        je     loc_1_2009
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x3a]
        push   ecx
        call   sub_1_54A8
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        push   0
        push   0x462
        push   ecx
        call   sub_1_53DC
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2009:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_2188
        mov    esi, eax
        test   esi, esi
        je     loc_1_2034
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x462
        push   eax
        call   sub_1_53DC
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2034:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2048:
        push   ebp
        mov    eax, 0xf
        mov    ebp, esp
        sub    esp, 0x24
        mov    word ptr [ebp - 0x24], ax
        push   esi
        mov    word ptr [ebp - 0x22], ax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, 8
        push   ecx
        mov    word ptr [ebp - 0x1c], ax
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 0x1a], ax
        push   edx
        mov    dword ptr [ebp - 0x20], 0     ; -> loc_1_55BC
        mov    dword ptr [ebp - 0x18], 0     ; -> obj1:0x55EC
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_5454
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_209A
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_209A:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        mov    dword ptr [esi + 8], eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_535C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_20D1
        lea    edx, [esi + 0x3e]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_20D1:
        push   1
        lea    eax, [ebp - 0x24]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_545A
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 8
        jne    loc_1_20F9
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
loc_1_20F9:
        cmp    eax, 0xff
        ja     loc_1_210D
        and    ax, 0xff
        mov    word ptr [esi + 0x460], ax
        jmp    loc_1_2116
loc_1_210D:
        mov    word ptr [esi + 0x460], 8
loc_1_2116:
        push   1
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_545A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_215A
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        cmp    al, 1
        jne    loc_1_2141
        xor    eax, eax
        jmp    loc_1_215F
loc_1_2141:
        cmp    al, 2
        jne    loc_1_214C
        mov    eax, 1
        jmp    loc_1_215F
loc_1_214C:
        sub    al, 3
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xfffffffe
        add    eax, 4
        jmp    loc_1_215F
loc_1_215A:
        mov    eax, 4
loc_1_215F:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   esi
        call   sub_1_31AE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5460
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2188:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x5644
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    ecx, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0
        push   0x32
        push   ecx
        call   sub_1_53D6
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_21B9
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_21B9:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x128]
        test   eax, eax
        je     loc_1_21F6
        cmp    eax, 1
        jne    loc_1_21E5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 2
        jmp    loc_1_2200
loc_1_21E5:
        cmp    eax, 2
        jne    loc_1_21F6
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 3
        jmp    loc_1_2200
loc_1_21F6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 1
loc_1_2200:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        lea    edx, [esi + 0x158]
        mov    edi, 0x10
        mov    dword ptr [eax + 0x16], 1
loc_1_2217:
        mov    eax, dword ptr [esi + 0x154]
        mov    ebx, dword ptr [ebp - 4]
        add    ecx, 0xc
        cmp    ecx, 0xc
        mov    dword ptr [ebx + ecx + 0xe], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 0x12], edi
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 0x16], edx
        jb     loc_1_2217
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x5790
        push   edx
        lea    ecx, [esi + 0x3a]
        push   esi
        push   eax
        push   ecx
        call   sub_1_5302
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_22AF
        push   esi
        call   sub_1_3AB8
        test   eax, eax
        je     loc_1_2286
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000002
        push   eax
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0xc000009a
        add    esp, 0xc
        jmp    loc_1_23DB
loc_1_2286:
        push   esi
        call   sub_1_27A8
        test   eax, eax
        je     loc_1_230C
        cmp    eax, 2
        jne    loc_1_22C8
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000007
        push   eax
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0xc0010004
        add    esp, 0xc
loc_1_22AF:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x10
        push   eax
        call   sub_1_53DC
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_22C8:
        cmp    eax, 3
        jne    loc_1_22F0
        mov    eax, dword ptr [esi + 0x154]
        mov    ecx, dword ptr [esi + 0x3a]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0xc001001e
        add    esp, 0x10
        jmp    loc_1_22AF
loc_1_22F0:
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000006
        push   eax
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0xc0000001
        add    esp, 0xc
        jmp    loc_1_22AF
loc_1_230C:
        lea    ebx, [esi + 0x22]
        xor    eax, eax
        mov    edi, ebx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, 2
        mov    ecx, dword ptr [esi + 0x130]
        cmp    dword ptr [esi + 0x12c], 1
        adc    eax, -1
        push   eax
        mov    al, byte ptr [esi + 0x124]
        push   eax
        push   ecx
        push   ecx
        mov    eax, dword ptr [esi + 0x3a]
        push   0                             ; -> loc_1_4F58
        lea    edx, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_4F38
        push   eax
        push   ebx
        push   edx
        call   sub_1_53BE
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2374
        mov    eax, dword ptr [esi + 0x130]
        mov    ecx, dword ptr [esi + 0x3a]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_54A8
        add    esp, 0x10
        jmp    loc_1_23DB
loc_1_2374:
        push   esi
        call   sub_1_3068
        test   eax, eax
        je     loc_1_23A8
        mov    eax, dword ptr [esi + 0x130]
        mov    ecx, dword ptr [esi + 0x154]
        push   eax
        mov    edx, dword ptr [esi + 0x3a]
        push   ecx
        push   2
        push   0xaa000001
        push   edx
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0xc001001e
        add    esp, 0x14
        jmp    loc_1_23D5
loc_1_23A8:
        lea    eax, [esi + 0x5e]
        lea    ecx, [esi + 0xd]
        push   eax
        lea    edx, [esi + 0x3e]
        push   ecx
        push   edx
        push   0                             ; -> loc_1_3ED8
        push   0                             ; -> loc_1_3C88
        push   0                             ; -> loc_1_3C58
        push   0x10
        call   sub_2_0000
loc_1_23CA:
        test   al, al
        jne    loc_1_23EF
        mov    dword ptr [ebp - 8], 0xc0000001
loc_1_23D5:
        push   ebx
        call   sub_1_53C4
loc_1_23DB:
        push   esi
        call   sub_1_3B58
        mov    eax, dword ptr [esi + 0x3a]
        push   eax
        call   sub_1_5308
        jmp    loc_1_22AF
loc_1_23EF:
        xor    ecx, ecx
        mov    dword ptr [esi + 0x56], ecx
        mov    dword ptr [esi + 0x52], ecx
        mov    dword ptr [esi + 0x5a], ecx
        mov    eax, dword ptr [0x4a]         ; -> loc_1_57DA
        mov    dword ptr [esi + 0x4e], eax
        push   ecx
        push   0x32
        mov    dword ptr [0x4a], esi         ; -> loc_1_57DA
        mov    edx, dword ptr [esi + 0x4a]
        mov    dword ptr [esi + 0x62], ecx
        mov    dword ptr [esi + 0x66], 0     ; -> loc_1_3C38
        mov    dword ptr [esi + 0x6a], esi
        inc    dword ptr [edx + 0x46]
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_53DC
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2438:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x19], ecx
        mov    dword ptr [eax + 0x1d], ecx
        mov    dword ptr [eax + 0x11], ecx
        mov    dword ptr [eax + 0x15], ecx
        mov    dword ptr [eax + 0x72], ecx
        mov    dword ptr [eax + 0x76], ecx
        mov    dword ptr [eax + 0x7a], ecx
        mov    dword ptr [eax + 0x7e], ecx
        pop    ebp
        mov    dword ptr [eax], 0x4a
        mov    dword ptr [eax + 0x4a], 0     ; -> obj1:0x5790
        mov    dword ptr [eax + 4], 0x64
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 050h, 080h
        db  048h, 021h, 040h, 0E8h, 0DFh, 01Ch, 000h, 000h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2498:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x5790
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_52FC
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x5794
        push   ecx
        call   sub_1_52F0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_24C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [0x4a], esi         ; -> loc_1_57DA
        jne    loc_1_24E6
        mov    eax, dword ptr [esi + 0x4e]
        mov    dword ptr [0x4a], eax         ; -> loc_1_57DA
        jmp    loc_1_2502
loc_1_24E6:
        mov    ecx, dword ptr [0x4a]         ; -> loc_1_57DA
        cmp    dword ptr [ecx + 0x4e], esi
        je     loc_1_24F9
loc_1_24F1:
        mov    ecx, dword ptr [ecx + 0x4e]
        cmp    dword ptr [ecx + 0x4e], esi
        jne    loc_1_24F1
loc_1_24F9:
        mov    eax, dword ptr [ecx + 0x4e]
        mov    edx, dword ptr [eax + 0x4e]
        mov    dword ptr [ecx + 0x4e], edx
loc_1_2502:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x62]
        push   eax
        push   ecx
        mov    edi, 1
        call   sub_1_53B8
        lea    ecx, [esi + 0x22]
        push   ecx
        call   sub_1_53C4
        mov    ebx, dword ptr [esi + 0x130]
        mov    edx, ebx
        and    dl, 7
        mov    cl, dl
        shl    edi, cl
        mov    ecx, 7
        cmp    ecx, ebx
        sbb    edx, edx
        xor    ecx, ecx
        and    edx, 0x80
        add    edx, 0x21
        in     al, dx
        mov    cl, al
        jmp    loc_1_2545
loc_1_2545:
        jmp    loc_1_2547
loc_1_2547:
        mov    eax, edi
        or     al, cl
        mov    ecx, dword ptr [esi + 0x5e]
        out    dx, al
        push   ecx
        call   sub_2_0120
loc_1_2555:
        push   esi
        call   sub_1_3B58
        push   esi
        call   sub_1_3BA8
        mov    ecx, dword ptr [esi + 0x3a]
        push   ecx
        call   sub_1_54B4
        mov    ecx, dword ptr [esi + 0x3a]
        push   ecx
        call   sub_1_5308
        push   0
        push   0x462
        push   esi
        call   sub_1_53DC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  033h, 0D2h, 08Bh, 0ECh, 08Bh, 04Ch, 024h, 008h, 08Dh, 081h, 02Bh, 001h, 000h, 000h, 088h, 051h
        db  05Ah, 083h, 0C1h, 05Ah, 089h, 010h, 089h, 050h, 004h, 089h, 050h, 008h, 089h, 050h, 00Ch, 08Dh
        db  081h, 0E1h, 000h, 000h, 000h, 089h, 010h, 089h, 050h, 004h, 088h, 050h, 008h, 088h, 091h, 0EAh
        db  000h, 000h, 000h, 0C6h, 081h, 0EBh, 000h, 000h, 000h, 040h, 0C6h, 081h, 0ECh, 000h, 000h, 000h
        db  05Bh, 0C6h, 081h, 0EDh, 000h, 000h, 000h, 06Fh, 0C6h, 081h, 0EEh, 000h, 000h, 000h, 080h, 0C6h
        db  081h, 0EFh, 000h, 000h, 000h, 08Fh, 0C6h, 081h, 0F0h, 000h, 000h, 000h, 09Dh, 0C6h, 081h, 0F1h
        db  000h, 000h, 000h, 0A9h, 0C6h, 081h, 0F2h, 000h, 000h, 000h, 0B5h, 0C6h, 081h, 0F3h, 000h, 000h
        db  000h, 0C0h, 0C6h, 081h, 0F4h, 000h, 000h, 000h, 0CAh, 033h, 0C0h, 05Dh, 0C6h, 081h, 0F5h, 000h
        db  000h, 000h, 0D4h, 0C6h, 081h, 0F6h, 000h, 000h, 000h, 0DEh, 0C6h, 081h, 0F7h, 000h, 000h, 000h
        db  0E7h, 0C6h, 081h, 0F8h, 000h, 000h, 000h, 0EFh, 0C6h, 081h, 0F9h, 000h, 000h, 000h, 0F8h, 0C2h
        db  004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2648:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0xb0]
        mov    byte ptr [esi], dl
        mov    byte ptr [esi + 1], dl
        mov    byte ptr [esi + 2], dl
        mov    byte ptr [esi + 3], dl
        mov    byte ptr [esi + 4], dl
        mov    dword ptr [esi + 0xc], 0x3c
        mov    dword ptr [esi + 0x14], 0x78fe
        mov    dword ptr [esi + 0x28], edx
        mov    dword ptr [esi + 0x24], 0x8800
        mov    dword ptr [esi + 0x34], edx
        mov    dword ptr [esi + 0x38], edx
        mov    dword ptr [esi + 0x3c], edx
        mov    eax, 0x5ea
        mov    word ptr [esi + 0x40], 0x8000
        mov    dword ptr [esi + 0x68], edx
        mov    dword ptr [esi + 0x6c], edx
        mov    dword ptr [esi + 0xa4], edx
        mov    dword ptr [esi + 0x70], edx
        mov    dword ptr [esi + 0x84], edx
        mov    dword ptr [esi + 0x88], eax
        mov    ecx, 0x20
        mov    dword ptr [esi + 0x8c], eax
        mov    eax, 4
        mov    dword ptr [esi + 0x90], 0x989680
        mov    dword ptr [esi + 0x9c], eax
        mov    dword ptr [esi + 0xa0], eax
        mov    word ptr [esi + 0xd8], 1
        mov    word ptr [esi + 0xda], cx
        mov    dword ptr [esi + 0xdc], 0x400
        mov    dword ptr [esi + 0xe0], ecx
        mov    dword ptr [esi + 0xe4], 0x1388
        mov    dword ptr [esi + 0xe8], 0x1d4c
        mov    dword ptr [esi + 0xec], 0x9810
        mov    dword ptr [esi + 0xf0], eax
        mov    word ptr [esi + 0xf4], dx
        mov    word ptr [esi + 0xf6], dx
        mov    word ptr [esi + 0xf8], dx
        mov    dword ptr [esi + 0xfa], 0x9900
        mov    dword ptr [esi + 0xfe], 0x100
        mov    word ptr [esi + 0x102], dx
        mov    word ptr [esi + 0x104], dx
        mov    word ptr [esi + 0x106], dx
        mov    dword ptr [esi + 0x160], 0    ; -> loc_1_17C8
        mov    dword ptr [esi + 0x164], edx
        push   edi
        mov    dword ptr [esi + 0x158], edx
        mov    dword ptr [esi + 0x15c], edx
        mov    dword ptr [esi + 0x174], 0    ; -> loc_1_1988
        mov    dword ptr [esi + 0x178], edx
        mov    dword ptr [esi + 0x16c], edx
        mov    dword ptr [esi + 0x170], edx
        call   sub_1_01C8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_27A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        add    ebx, 0xb0
        mov    eax, dword ptr [ecx + 0x22c]
        mov    esi, dword ptr [ecx + 0x184]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xe
        mov    dword ptr [ebx + 0xac], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xe
        mov    dword ptr [ebx + 0xb0], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xb
        mov    dword ptr [ebx + 0xb4], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xa
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xc
        mov    dword ptr [ebx + 0xbc], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 8
        mov    dword ptr [ebx + 0xc0], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xa
        mov    dword ptr [ebx + 0xc4], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 8
        mov    dword ptr [ebx + 0xc8], eax
        mov    eax, dword ptr [ebx + 0xa8]
        mov    dword ptr [ebx + 0xcc], eax
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebx + 0x78]
        test   eax, eax
        je     loc_1_285F
        cmp    eax, 4
        jne    loc_1_288C
loc_1_285F:
        cmp    byte ptr [ebx + 0xa], 0
        jne    loc_1_288C
        cli    
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3A38
        mov    ecx, dword ptr [ebx + 0x68]
        mov    edx, esi
        sub    cl, 0x28
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [ebx + 0xa4]
        sub    eax, 0x200
        shr    eax, 4
        sub    al, 0x20
        out    dx, al
        sti    
loc_1_288C:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 0x800
        mov    edx, dword ptr [ecx + 0x15c]
        mov    ecx, dword ptr [ecx + 0x158]
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_28BA
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28BA:
        lea    edx, [ecx + 0xc]
        mov    eax, 0x5555
        out    dx, ax
        in     ax, dx
        cmp    ax, 0x5555
        je     loc_1_28DA
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28DA:
        mov    eax, 0xaaaa
        out    dx, ax
        in     ax, dx
        cmp    ax, 0xaaaa
        je     loc_1_28F7
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28F7:
        cmp    dword ptr [ebx + 0x80], 0
        jne    loc_1_2917
        lea    edx, [ecx + 8]
        in     ax, dx
        shr    ax, 0xc
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebx + 0x80], ecx
loc_1_2917:
        cmp    dword ptr [ebx + 0x78], 2
        jne    loc_1_292A
        mov    dword ptr [ebx + 0x7c], 0
        mov    byte ptr [ebx + 0x74], 1
        jmp    loc_1_2935
loc_1_292A:
        mov    dword ptr [ebx + 0x7c], 1
        mov    byte ptr [ebx + 0x74], 0
loc_1_2935:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x158]
        add    edx, 4
        in     ax, dx
        test   ah, 2
        je     loc_1_294C
        mov    byte ptr [ebx + 5], 1
loc_1_294C:
        test   ah, 0x10
        je     loc_1_2955
        mov    byte ptr [ebx + 6], 1
loc_1_2955:
        test   ah, 0x20
        je     loc_1_295E
        mov    byte ptr [ebx + 7], 1
loc_1_295E:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x158]
        add    edx, 6
        cmp    dword ptr [ebx + 0x44], 0
        in     ax, dx
        jne    loc_1_29B0
        mov    cx, ax
        and    ecx, 0xc000
        je     loc_1_298F
        cmp    ecx, 0x4000
        je     loc_1_2998
        cmp    ecx, 0xc000
        je     loc_1_29A1
        jmp    loc_1_29A8
loc_1_298F:
        mov    dword ptr [ebx + 0x44], 1
        jmp    loc_1_29A8
loc_1_2998:
        mov    dword ptr [ebx + 0x44], 3
        jmp    loc_1_29A8
loc_1_29A1:
        mov    dword ptr [ebx + 0x44], 2
loc_1_29A8:
        test   al, 0x80
        je     loc_1_29B0
        mov    byte ptr [ebx + 4], 1
loc_1_29B0:
        push   0xd
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3BF8
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_1_29DF
        mov    cx, ax
        and    cx, 0x3f00
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        lea    esi, [edx + edx*4 + 5]
        lea    ecx, [esi + esi*4]
        mov    dword ptr [ebx + 0x18], ecx
loc_1_29DF:
        test   ah, 0x40
        je     loc_1_29E8
        mov    byte ptr [ebx + 3], 1
loc_1_29E8:
        push   0xe
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3BF8
        and    eax, 0xffff
        test   eax, 0xffffff00
        jle    loc_1_2A0D
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A0D:
        push   0x14
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3BF8
        and    eax, 0xf
        push   0xa
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        mov    dword ptr [ebx + 0x10], eax
        call   sub_1_3BF8
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    esi, 0x44
        push   0xb
        push   ecx
        mov    byte ptr [ecx + 0x45], al
        mov    byte ptr [esi], ah
        call   sub_1_3BF8
        mov    ecx, dword ptr [ebp + 8]
        push   0xc
        push   ecx
        mov    byte ptr [ecx + 0x46], ah
        mov    byte ptr [ecx + 0x47], al
        call   sub_1_3BF8
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [ecx + 0x3e]
        mov    byte ptr [ecx + 0x48], ah
        mov    byte ptr [ecx + 0x49], al
        mov    al, byte ptr [ecx + 0x3f]
        or     al, byte ptr [ecx + 0x40]
        or     al, byte ptr [ecx + 0x41]
        or     al, byte ptr [ecx + 0x43]
        or     al, byte ptr [ecx + 0x42]
        or     al, byte ptr [edx]
        jne    loc_1_2A7C
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    cx, word ptr [esi + 4]
        mov    word ptr [edx + 4], cx
loc_1_2A7C:
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + 0x158]
        mov    esi, dword ptr [ecx + 0x130]
        shl    si, 0xc
        lea    eax, [edi + 8]
        mov    edx, eax
        in     ax, dx
        and    ax, 0xfff
        lea    edx, [edi + 8]
        or     si, ax
        mov    ax, si
        mov    esi, dword ptr [ecx + 0x15c]
        out    dx, ax
        lea    edx, [edi + 4]
        mov    eax, 1
        out    dx, ax
        mov    edx, esi
        mov    eax, 0x5800
        out    dx, ax
        mov    eax, dword ptr [ecx + 0x160]
        mov    edx, eax
        mov    dword ptr [ebp - 0x14], eax
loc_1_2AC9:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2AC9
        mov    edx, esi
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
loc_1_2ADC:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2ADC
        mov    edx, esi
        mov    eax, 0x68ff
        out    dx, ax
        mov    eax, 0x806
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, edi
        in     al, dx
        lea    edx, [edi + 4]
        in     al, dx
        lea    edx, [edi + 8]
        in     al, dx
        lea    edx, [edi + 1]
        in     al, dx
        lea    edx, [edi + 2]
        in     al, dx
        lea    edx, [edi + 3]
        in     al, dx
        lea    edx, [edi + 5]
        in     al, dx
        lea    edx, [edi + 6]
        in     al, dx
        lea    edx, [edi + 7]
        in     al, dx
        lea    edx, [edi + 0xc]
        in     ax, dx
        lea    edx, [edi + 0xa]
        in     ax, dx
        mov    edx, esi
        mov    eax, 0x801
        mov    esi, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x16c]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    ax, 0xbd4
        mov    dword ptr [ebx + 0x30], ecx
        ja     loc_1_2B50
        mov    ax, 0xbd4
loc_1_2B50:
        and    eax, 0xffff
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x94], eax
        mov    eax, 0x803
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x174]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    ax, 0xbd4
        mov    dword ptr [ebx + 0x2c], ecx
        ja     loc_1_2B88
        mov    ax, 0xbd4
loc_1_2B88:
        and    eax, 0xffff
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x98], eax
        mov    ax, word ptr [ebx + 0x14]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        mov    eax, 0x70fe
        mov    edi, dword ptr [ebx + 0x18]
        out    dx, ax
        lea    edi, [edi + edi*4]
        add    edi, edi
        shr    edi, 5
        cmp    edi, 0xff
        jb     loc_1_2BC1
        mov    edi, 0xff
loc_1_2BC1:
        mov    eax, 0x801
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        push   esi
        mov    esi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 0x200
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        shl    edi, 9
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x168]
        xor    ecx, ecx
        in     al, dx
        mov    cl, al
        sub    edx, edx
        mov    eax, edi
        div    ecx
        and    eax, 0xfffffffc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, 0x5800
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x160]
loc_1_2C1F:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2C1F
        mov    edx, esi
        mov    eax, 0x68ff
        mov    dword ptr [ebp - 0x18], 0x200
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x18]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x168]
        in     al, dx
        and    eax, 0xff
        shr    eax, 1
        mov    dword ptr [ebx + 0x20], eax
        mov    eax, 0x2800
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ecx + 0x160]
loc_1_2C7A:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2C7A
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0x68ff
        xor    edi, edi
        xor    esi, esi
        out    dx, ax
        mov    eax, 0x802
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    eax, dword ptr [ecx + 0x158]
        mov    dword ptr [ebp - 4], eax
loc_1_2CA2:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, edi
        inc    di
        add    edx, esi
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        inc    esi
        mov    al, byte ptr [eax + ecx + 0x3e]
        out    dx, al
        cmp    di, 6
        jb     loc_1_2CA2
        cmp    byte ptr [ebx + 4], 0
        je     loc_1_2D24
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0xb000
        cmp    byte ptr [ebx + 5], 0
        out    dx, ax
        je     loc_1_2CE8
        push   ecx
        call   sub_1_2D98
        test   al, al
        je     loc_1_2CE8
        mov    dword ptr [ebx + 0x44], 1
        jmp    loc_1_2D31
loc_1_2CE8:
        cmp    byte ptr [ebx + 6], 0
        je     loc_1_2D06
        push   2
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_2E28
        test   al, al
        je     loc_1_2D06
        mov    dword ptr [ebx + 0x44], 2
        jmp    loc_1_2D31
loc_1_2D06:
        cmp    byte ptr [ebx + 7], 0
        je     loc_1_2D24
        push   3
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_2E28
        test   al, al
        je     loc_1_2D24
        mov    dword ptr [ebx + 0x44], 3
        jmp    loc_1_2D31
loc_1_2D24:
        mov    eax, dword ptr [ebx + 0x44]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_2FC8
loc_1_2D31:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 0xa800
        mov    esi, dword ptr [ecx + 0x15c]
        mov    ecx, dword ptr [ecx + 0x160]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_2D52:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2D52
        mov    edx, esi
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_2D64:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2D64
        mov    ax, word ptr [ebx + 0x24]
        mov    edx, esi
        out    dx, ax
        mov    ax, word ptr [ebx + 0xec]
        pop    edi
        out    dx, ax
        mov    eax, 0x801
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D98:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_2FC8
        mov    edx, dword ptr [edi + 0x15c]
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x2000
        push   0x30d40
        out    dx, ax
        call   sub_1_53D0
        mov    esi, dword ptr [edi + 0x15c]
        mov    eax, 0x804
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        add    edx, 0xa
        in     ax, dx
        and    ax, 0x800
        mov    edx, esi
        cmp    ax, 1
        mov    ecx, dword ptr [edi + 0x160]
        sbb    ebx, ebx
        mov    eax, 0x5800
        out    dx, ax
        inc    ebx
        mov    edx, ecx
loc_1_2DFD:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2DFD
        mov    edx, esi
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_2E0F:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2E0F
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E28:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   esi
        call   sub_1_2FC8
loc_1_2E3D:
        push   esi
        call   sub_1_2E68
        test   al, al
        jne    loc_1_2E55
        inc    edi
        cmp    edi, 3
        jb     loc_1_2E3D
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_2E55:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E68:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17c]
        mov    ecx, dword ptr [edi + 0x15c]
        mov    edx, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0x804
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        add    edx, 6
        in     ax, dx
        and    ax, 0xfff
        or     ah, 0x80
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, 0x8001
        lea    ecx, [edi + 0x3e]
        out    dx, ax
        mov    eax, 0x801
        mov    dword ptr [ebp - 8], ecx
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 0x800e
        out    dx, eax
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        out    dx, eax
        mov    esi, 0x3e8
loc_1_2F10:
        push   esi
        call   sub_1_53D0
        mov    edx, dword ptr [edi + 0x164]
        in     al, dx
        test   al, 0x80
        jne    loc_1_2F2F
        add    esi, 0x3e8
        cmp    esi, 0x59d8
        jb     loc_1_2F10
loc_1_2F2F:
        mov    edx, dword ptr [edi + 0x15c]
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, dword ptr [edi + 0x160]
loc_1_2F42:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2F42
        xor    esi, esi
loc_1_2F4B:
        mov    edx, dword ptr [edi + 0x170]
        in     ax, dx
        test   ah, 0x80
        je     loc_1_2F65
        push   0x64
        inc    esi
        call   sub_1_53D0
        cmp    esi, 0xf
        jb     loc_1_2F4B
loc_1_2F65:
        mov    edx, dword ptr [edi + 0x170]
        in     ax, dx
        test   ah, 0xc0
        jne    loc_1_2F74
        mov    bl, 1
loc_1_2F74:
        mov    ecx, dword ptr [edi + 0x15c]
        mov    eax, 0x2800
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 0x160]
loc_1_2F89:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2F89
        mov    edx, ecx
        mov    eax, 0x68ff
        out    dx, ax
        mov    eax, 0x804
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        pop    edi
        add    edx, 6
        pop    esi
        in     ax, dx
        and    eax, 0xfff
        out    dx, ax
        mov    al, bl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2FC8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, 0x800
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ecx + 0x15c]
        mov    edx, ebx
        mov    edi, dword ptr [ecx + 0x158]
        out    dx, ax
        lea    edx, [edi + 6]
        mov    esi, dword ptr [ebp + 0xc]
        in     ax, dx
        and    ax, 0x3fff
        cmp    esi, 1
        je     loc_1_300A
        cmp    esi, 2
        jne    loc_1_3002
        or     ah, 0xc0
        jmp    loc_1_300A
loc_1_3002:
        cmp    esi, 3
        jne    loc_1_300A
        or     ah, 0x40
loc_1_300A:
        out    dx, ax
        mov    edx, ebx
        mov    eax, 0x804
        out    dx, ax
        lea    edx, [edi + 0xa]
        in     ax, dx
        and    ax, 0xff37
        cmp    esi, 3
        jne    loc_1_3025
        or     al, 8
loc_1_3025:
        cmp    esi, 1
        jne    loc_1_3037
        or     al, 0x40
        cmp    byte ptr [ecx + 0xb3], 0
        jne    loc_1_3037
        or     al, 0x80
loc_1_3037:
        out    dx, ax
        mov    edx, ebx
        cmp    esi, 2
        jne    loc_1_3047
        mov    eax, 0x1000
        jmp    loc_1_304C
loc_1_3047:
        mov    eax, 0xb800
loc_1_304C:
        out    dx, ax
        push   0xbb8
        call   sub_1_53D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3068:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x15c]
        mov    eax, 0x6000
        out    dx, ax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_309A
loc_1_3097:
        inc    dword ptr [ebp - 8]
loc_1_309A:
        cmp    dword ptr [ebp - 8], 0x1388
        jae    loc_1_30ED
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_30C2
        jmp    loc_1_30ED
loc_1_30C2:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x168]
        in     al, dx
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_1_30E1
        jmp    loc_1_30ED
loc_1_30E1:
        push   1
        call   sub_1_53D0
        jmp    loc_1_3097
loc_1_30ED:
        cmp    dword ptr [ebp - 8], 0x320
        jae    loc_1_3101
        mov    dword ptr [ebp - 8], 0x320
loc_1_3101:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_3110
loc_1_310D:
        inc    dword ptr [ebp - 4]
loc_1_3110:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_3180
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3143
        jmp    loc_1_3180
loc_1_3143:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x160]
        in     ax, dx
        and    eax, 0xffff
        test   al, 1
        jne    loc_1_316A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb8], 1
        jmp    loc_1_3180
loc_1_316A:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x168]
        in     al, dx
        push   1
        call   sub_1_53D0
        jmp    loc_1_310D
loc_1_3180:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_31A0
        mov    eax, 1
        jmp    loc_1_31A7
loc_1_31A0:
        xor    eax, eax
        jmp    loc_1_31A7
loc_1_31A7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_31AE:
        push   ebp
        mov    ecx, 0xa
        mov    ebp, esp
        sub    esp, 0x28
        mov    word ptr [ebp - 0x28], cx
        mov    eax, 5
        mov    word ptr [ebp - 0x26], cx
        push   esi
        mov    word ptr [ebp - 0x18], ax
        push   edi
        mov    dword ptr [ebp - 0x24], 0     ; -> loc_1_5600
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_1_5614
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_5624
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_5638
        mov    word ptr [ebp - 0x16], ax
        mov    eax, 7
        mov    word ptr [ebp - 0x10], ax
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0xe], ax
        push   1
        mov    word ptr [ebp - 0x20], cx
        lea    eax, [ebp - 0x28]
        mov    word ptr [ebp - 0x1e], cx
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        lea    eax, [ebp - 8]
        mov    dword ptr [esi + 0x128], ecx
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_545A
        mov    dword ptr [esi + 0x154], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3252
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0x200
        jb     loc_1_3252
        cmp    eax, 0x3e0
        ja     loc_1_3252
        test   al, 0xf
        jne    loc_1_3252
        mov    dword ptr [esi + 0x154], eax
loc_1_3252:
        push   1
        lea    eax, [ebp - 0x20]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_545A
        mov    dword ptr [esi + 0x130], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_32AA
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 3
        je     loc_1_32A4
        cmp    eax, 5
        je     loc_1_32A4
        cmp    eax, 7
        je     loc_1_32A4
        cmp    eax, 9
        je     loc_1_32A4
        cmp    eax, 0xa
        je     loc_1_32A4
        cmp    eax, 0xb
        je     loc_1_32A4
        cmp    eax, 0xc
        je     loc_1_32A4
        cmp    eax, 0xf
        jne    loc_1_32AA
loc_1_32A4:
        mov    dword ptr [esi + 0x130], eax
loc_1_32AA:
        push   1
        lea    eax, [ebp - 0x18]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_545A
        mov    dword ptr [esi + 0x120], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_32E4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 1
        jb     loc_1_32E4
        cmp    eax, 0xf
        ja     loc_1_32E4
        mov    dword ptr [esi + 0x120], eax
loc_1_32E4:
        push   1
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_545A
        mov    dword ptr [esi + 0x180], 0x110
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3326
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0x100
        jb     loc_1_3326
        cmp    eax, 0x1f0
        ja     loc_1_3326
        test   al, 0xf
        jne    loc_1_3326
        mov    dword ptr [esi + 0x180], eax
loc_1_3326:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3338:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x128]
        cmp    eax, 1
        jne    loc_1_3388
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3558
        test   eax, eax
        jne    loc_1_335F
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_335F:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3628
        test   eax, eax
        jne    loc_1_337E
        mov    dword ptr [esi + 0x128], 0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_337E:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_3388:
        cmp    eax, 2
        jne    loc_1_339C
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3408
        pop    esi
        pop    ebp
        ret    4
loc_1_339C:
        test   eax, eax
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        jne    loc_1_33CA
        call   sub_1_3628
        test   eax, eax
        jne    loc_1_33B5
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_33B5:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3558
        pop    esi
        cmp    eax, 1
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
loc_1_33CA:
        call   sub_1_3628
        test   eax, eax
        jne    loc_1_33DA
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_33DA:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3558
        test   eax, eax
        jne    loc_1_33EF
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_33EF:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3408
        pop    esi
        cmp    eax, 1
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3408:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x120]
        test   ebx, ebx
        je     loc_1_344A
        cmp    byte ptr [edi + ebx + 0x13b], 0
        jne    loc_1_344A
        push   ebx
        push   esi
        call   sub_1_3498
        test   al, al
        je     loc_1_344A
        xor    eax, eax
        mov    dword ptr [esi + 0x11c], ebx
        mov    byte ptr [edi + ebx + 0x13b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_344A:
        mov    ebx, 1
loc_1_344F:
        cmp    byte ptr [edi + ebx + 0x13b], 0
        jne    loc_1_3464
        push   ebx
        push   esi
        call   sub_1_3498
        test   al, al
        jne    loc_1_3476
loc_1_3464:
        inc    ebx
        cmp    ebx, 8
        jbe    loc_1_344F
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3476:
        xor    eax, eax
        mov    dword ptr [esi + 0x11c], ebx
        mov    byte ptr [edi + ebx + 0x13b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3498:
        push   ebp
        mov    al, 0xff
        mov    ebp, esp
        xor    edx, edx
        out    0x94, al
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, edx
        je     loc_1_34B3
loc_1_34A8:
        mov    eax, edx
        inc    edx
        or     al, 8
        cmp    edx, ecx
        out    0x96, al
        jb     loc_1_34A8
loc_1_34B3:
        mov    edx, 0x100
        in     al, dx
        mov    cl, al
        mov    edx, 0x101
        in     al, dx
        and    ax, 0xff
        and    ecx, 0xffff00ff
        shl    cx, 8
        add    cx, ax
        mov    al, cl
        shr    cx, 8
        and    eax, 0xffff00ff
        shl    ax, 8
        add    ax, cx
        cmp    ax, 0x627c
        je     loc_1_350A
        cmp    ax, 0x627d
        je     loc_1_350A
        cmp    ax, 0x61db
        je     loc_1_350A
        cmp    ax, 0x62f6
        je     loc_1_350A
        cmp    ax, 0x62f7
        je     loc_1_350A
        xor    al, al
        pop    ebp
        out    0x96, al
        ret    8
loc_1_350A:
        mov    edx, 0x102
        in     al, dx
        test   al, 1
        jne    loc_1_351C
        xor    al, al
        pop    ebp
        out    0x96, al
        ret    8
loc_1_351C:
        mov    edx, 0x104
        in     al, dx
        and    ax, 0xfc
        mov    edx, dword ptr [ebp + 8]
        add    ax, 2
        shl    ax, 8
        pop    ebp
        mov    ecx, eax
        xor    al, al
        out    0x96, al
        and    ecx, 0xffff
        mov    al, 1
        mov    dword ptr [edx + 0x154], ecx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3558:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x120]
        test   ebx, ebx
        je     loc_1_35A5
        cmp    byte ptr [edi + ebx + 0x12b], 0
        jne    loc_1_35A5
        push   ebx
        push   esi
        call   sub_1_35F8
        test   al, al
        je     loc_1_35A5
        mov    eax, ebx
        mov    dword ptr [esi + 0x11c], ebx
        shl    eax, 0xc
        mov    dword ptr [esi + 0x154], eax
        xor    eax, eax
        mov    byte ptr [edi + ebx + 0x12b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_35A5:
        mov    ebx, 1
loc_1_35AA:
        cmp    byte ptr [edi + ebx + 0x12b], 0
        jne    loc_1_35BF
        push   ebx
        push   esi
        call   sub_1_35F8
        test   al, al
        jne    loc_1_35D1
loc_1_35BF:
        inc    ebx
        cmp    ebx, 0xf
        jbe    loc_1_35AA
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_35D1:
        mov    eax, ebx
        mov    dword ptr [esi + 0x11c], ebx
        shl    eax, 0xc
        mov    dword ptr [esi + 0x154], eax
        xor    eax, eax
        mov    byte ptr [edi + ebx + 0x12b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_35F8:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        shl    edx, 0xc
        mov    ebp, esp
        add    edx, 0xc80
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_3616
        xor    al, al
        pop    ebp
        ret    8
loc_1_3616:
        add    edx, 2
        pop    ebp
        in     ax, dx
        mov    cl, al
        not    cl
        and    cl, 1
        mov    al, cl
        ret    8
sub_1_3628:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x180]
        mov    dword ptr [edi + 0x184], eax
        cmp    byte ptr [esi + 0x5a], 0
        jne    loc_1_3651
        mov    byte ptr [esi + 0x5a], 1
        push   esi
        push   edi
        call   sub_1_3718
loc_1_3651:
        mov    ebx, dword ptr [edi + 0x154]
        test   ebx, ebx
        je     loc_1_36E6
        push   ebx
        push   esi
        push   edi
        call   sub_1_3838
        cmp    eax, 2
        jne    loc_1_370F
        push   0
        push   ebx
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     loc_1_3687
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3687:
        push   1
        push   ebx
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     loc_1_369E
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_369E:
        push   0
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     loc_1_36BC
        xor    eax, eax
        mov    dword ptr [edi + 0x154], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36BC:
        push   1
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     loc_1_36DA
        xor    eax, eax
        mov    dword ptr [edi + 0x154], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36DA:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36E6:
        push   0
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     loc_1_36FE
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36FE:
        push   1
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_1_370F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_3718:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x184]
        mov    dword ptr [ebp - 8], eax
        cli    
        push   edi
        mov    ebx, 1
        call   sub_1_3A38
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0xd0
        out    dx, al
        mov    esi, dword ptr [ebp + 0xc]
        add    esi, 0x69
loc_1_3749:
        push   7
        push   edi
        call   sub_1_3A78
        cmp    ax, 0x6d50
        jne    loc_1_3814
        push   0xa
        push   edi
        call   sub_1_3A78
        push   0xb
        mov    byte ptr [esi + 6], ah
        push   edi
        mov    byte ptr [esi + 7], al
        call   sub_1_3A78
        push   0xc
        mov    byte ptr [esi + 8], ah
        push   edi
        mov    byte ptr [esi + 9], al
        call   sub_1_3A78
        push   3
        mov    byte ptr [esi + 0xa], ah
        push   edi
        mov    byte ptr [esi + 0xb], al
        mov    byte ptr [esi - 1], 1
        mov    byte ptr [esi], 0
        call   sub_1_3A78
        test   al, 1
        je     loc_1_379D
        mov    byte ptr [esi], 1
        jmp    loc_1_37F7
loc_1_379D:
        push   8
        push   edi
        call   sub_1_3A78
        and    ax, 0x1f
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, 0x20
        shl    ecx, 4
        cmp    ax, 0x1f
        mov    dword ptr [esi + 2], ecx
        jne    loc_1_37C5
        mov    byte ptr [esi], 1
        jmp    loc_1_37F7
loc_1_37C5:
        push   0x10
        push   edi
        call   sub_1_3A78
        test   al, 1
        je     loc_1_37F7
        push   0x13
        push   edi
        call   sub_1_3A78
        test   al, 8
        jne    loc_1_37E1
        test   al, 0xc
        jne    loc_1_37F7
loc_1_37E1:
        mov    byte ptr [esi + 1], 1
        push   0x14
        push   edi
        call   sub_1_3A78
        and    al, 0xf
        cmp    al, 2
        jae    loc_1_37F7
        mov    byte ptr [ebp - 1], 1
loc_1_37F7:
        lea    eax, [ebx - 0x30]
        mov    edx, dword ptr [ebp - 8]
        push   edi
        add    esi, 0xd
        out    dx, al
        xor    al, al
        inc    ebx
        out    dx, al
        call   sub_1_3A38
        cmp    ebx, 8
        jb     loc_1_3749
loc_1_3814:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        sti    
        cmp    byte ptr [ebp - 1], al
        je     loc_1_382E
        mov    edx, 0x279
        mov    al, 2
        out    dx, al
        mov    edx, 0xa79
        out    dx, al
loc_1_382E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3838:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 0x800
        lea    edx, [esi + 0xe]
        mov    dword ptr [edi + 0x154], esi
        out    dx, ax
        mov    edx, esi
        mov    dword ptr [edi + 0x158], esi
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_3875
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3875:
        lea    edx, [esi + 2]
        in     ax, dx
        test   al, 1
        je     loc_1_388C
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_388C:
        lea    edx, [esi + 0xc]
        mov    eax, 0x5555
        out    dx, ax
        in     ax, dx
        cmp    ax, 0x5555
        je     loc_1_38AC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_38AC:
        mov    eax, 0xaaaa
        out    dx, ax
        in     ax, dx
        cmp    ax, 0xaaaa
        je     loc_1_38C9
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_38C9:
        mov    ebx, 1
        push   0xa
        push   edi
        mov    byte ptr [edi + 0xba], bl
        call   sub_1_3BF8
        push   0xb
        mov    byte ptr [ebp - 8], ah
        push   edi
        mov    byte ptr [ebp - 7], al
        call   sub_1_3BF8
        push   0xc
        mov    byte ptr [ebp - 6], ah
        push   edi
        mov    byte ptr [ebp - 5], al
        call   sub_1_3BF8
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [ebp - 4], ah
        lea    edx, [edi + 0x68]
loc_1_3901:
        cmp    byte ptr [edx], 0
        je     loc_1_393C
        mov    cl, byte ptr [ebp - 8]
        cmp    byte ptr [edx + 7], cl
        jne    loc_1_3933
        mov    cl, byte ptr [ebp - 7]
        cmp    byte ptr [edx + 8], cl
        jne    loc_1_3933
        mov    cl, byte ptr [ebp - 6]
        cmp    byte ptr [edx + 9], cl
        jne    loc_1_3933
        mov    cl, byte ptr [ebp - 5]
        cmp    byte ptr [edx + 0xa], cl
        jne    loc_1_3933
        mov    cl, byte ptr [ebp - 4]
        cmp    byte ptr [edx + 0xb], cl
        jne    loc_1_3933
        cmp    byte ptr [edx + 0xc], al
        je     loc_1_3980
loc_1_3933:
        add    edx, 0xd
        inc    ebx
        cmp    ebx, 0x10
        jb     loc_1_3901
loc_1_393C:
        lea    ecx, [ebx + ebx*2]
        lea    ebx, [ebx + ecx*4]
        mov    ecx, 1
        mov    byte ptr [ebx + edi + 0x5b], cl
        lea    edx, [ebx + edi]
        mov    bl, byte ptr [ebp - 8]
        mov    byte ptr [edx + 0x5c], cl
        mov    cl, byte ptr [ebp - 7]
        mov    dword ptr [edx + 0x5e], esi
        mov    byte ptr [edx + 0x62], bl
        mov    bl, byte ptr [ebp - 6]
        mov    byte ptr [edx + 0x63], cl
        mov    cl, byte ptr [ebp - 5]
        pop    edi
        mov    byte ptr [edx + 0x64], bl
        mov    bl, byte ptr [ebp - 4]
        pop    esi
        mov    byte ptr [edx + 0x65], cl
        mov    byte ptr [edx + 0x66], bl
        pop    ebx
        mov    byte ptr [edx + 0x67], al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3980:
        lea    eax, [ebx + ebx*2]
        lea    eax, [ebx + eax*4]
        add    eax, edi
        cmp    byte ptr [eax + 0x5c], 0
        je     loc_1_399C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_399C:
        pop    edi
        mov    dword ptr [eax + 0x5e], esi
        mov    byte ptr [eax + 0x5c], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_39B8:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        mov    cl, byte ptr [ebp + 0x14]
        lea    edi, [esi + 0x68]
loc_1_39D1:
        cmp    byte ptr [edi], 0
        je     loc_1_39F7
        cmp    byte ptr [edi + 1], 0
        jne    loc_1_39EA
        cmp    byte ptr [edi + 2], cl
        jne    loc_1_39EA
        test   eax, eax
        je     loc_1_39F5
        cmp    dword ptr [edi + 3], eax
        je     loc_1_39F5
loc_1_39EA:
        add    edi, 0xd
        inc    edx
        cmp    edx, 0x10
        jl     loc_1_39D1
        jmp    loc_1_39F7
loc_1_39F5:
        mov    bl, 1
loc_1_39F7:
        cmp    bl, 1
        jne    loc_1_3A23
        lea    eax, [edx + edx*2]
        lea    eax, [edx + eax*4]
        add    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 0x5c], 1
        mov    ecx, dword ptr [eax + 0x5e]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x154], ecx
        mov    dword ptr [eax + 0x118], edx
        mov    al, 1
        ret    0x10
loc_1_3A23:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A38:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [eax + 0x184]
        mov    esi, 0xfe
        xor    al, al
        out    dx, al
        out    dx, al
        mov    ax, 0xff
loc_1_3A53:
        out    dx, al
        add    ax, ax
        test   ah, 1
        je     loc_1_3A5E
        xor    al, 0xcf
loc_1_3A5E:
        mov    ecx, esi
        dec    esi
        test   ecx, ecx
        jne    loc_1_3A53
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A78:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        push   0x190
        xor    si, si
        mov    edi, dword ptr [eax + 0x184]
        mov    al, byte ptr [ebp + 0xc]
        mov    edx, edi
        add    al, 0x80
        out    dx, al
        call   sub_1_53D0
        mov    ecx, 0x10
loc_1_3AA1:
        mov    edx, edi
        in     al, dx
        and    ax, 1
        dec    ecx
        lea    esi, [eax + esi*2]
        jne    loc_1_3AA1
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_3AB8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x5644
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        add    esi, 0xb0
        push   0
        push   0x800
        lea    ecx, [esi + 0x17c]
        push   ecx
        call   sub_1_53D6
        test   eax, eax
        je     loc_1_3AF0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_3AF0:
        mov    edi, dword ptr [esi + 0x17c]
        xor    eax, eax
        mov    ecx, 0x200
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x5644
        lea    ebx, [esi + 0x180]
        push   eax
        push   0
        push   0x5ec
        push   ebx
        call   sub_1_53D6
        test   eax, eax
        je     loc_1_3B28
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_3B28:
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        mov    ecx, 0x17b
        rep stosd dword ptr es:[edi], eax
        pop    edi
        mov    eax, dword ptr [esi + 0x17c]
        mov    dword ptr [esi + 0x154], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esi + 0x168], ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3B58:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x22c]
        add    esi, 0xb0
        test   eax, eax
        je     loc_1_3B7C
        push   0
        push   0x800
        push   eax
        call   sub_1_53DC
loc_1_3B7C:
        mov    eax, dword ptr [esi + 0x180]
        test   eax, eax
        je     loc_1_3B93
        push   0
        push   0x5ec
        push   eax
        call   sub_1_53DC
loc_1_3B93:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3BA8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, 1
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [eax + 0x4a]
        lea    ecx, [esi + 0x69]
loc_1_3BBF:
        cmp    byte ptr [ecx], 0
        je     loc_1_3BCF
        mov    edi, dword ptr [eax + 0x154]
        cmp    dword ptr [ecx + 2], edi
        je     loc_1_3BDA
loc_1_3BCF:
        add    ecx, 0xd
        inc    edx
        cmp    edx, 0x10
        jl     loc_1_3BBF
        jmp    loc_1_3BDC
loc_1_3BDA:
        mov    bl, 1
loc_1_3BDC:
        cmp    bl, 1
        jne    loc_1_3BEC
        lea    eax, [edx + edx*2]
        lea    ecx, [edx + eax*4]
        mov    byte ptr [ecx + esi + 0x5c], 0
loc_1_3BEC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_3BF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x158]
        mov    eax, 0x800
        and    esi, 0xffff00ff
        add    si, 0x80
        lea    edx, [ecx + 0xe]
        out    dx, ax
        mov    ax, si
        lea    edx, [ecx + 0xa]
        out    dx, ax
loc_1_3C27:
        in     ax, dx
        test   ah, 0x80
        jne    loc_1_3C27
        lea    edx, [ecx + 0xc]
        pop    esi
        in     ax, dx
        pop    ebp
        ret    8
loc_1_3C38:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_1BA8
        mov    eax, dword ptr [esi + 4]
        add    esi, 0x62
        push   eax
        push   esi
        call   sub_1_53B2
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
loc_1_3C58:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        mov    eax, 0
        pop    ebp
        cmp    byte ptr [ecx + 0x83], 1
        adc    eax, -1
        and    eax, 0xc001000d
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3C88:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    edx, dword ptr [eax]
        xor    eax, eax
        cmp    byte ptr [edx + 0x83], al
        je     loc_1_3CA4
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_3CA4:
        mov    ecx, dword ptr [ebp + 0xc]
        test   ecx, 0xf010
        je     loc_1_3CB8
        mov    eax, 0xc0000001
        pop    ebp
        ret    0x14
loc_1_3CB8:
        test   cl, 1
        je     loc_1_3CC2
        mov    eax, 1
loc_1_3CC2:
        test   cl, 2
        je     loc_1_3CCA
        or     eax, 2
loc_1_3CCA:
        test   cl, 4
        je     loc_1_3CD2
        or     eax, 2
loc_1_3CD2:
        test   cl, 8
        je     loc_1_3CDA
        or     eax, 4
loc_1_3CDA:
        mov    byte ptr [edx + 0x82], 0
        test   cl, 0x20
        je     loc_1_3CF0
        mov    byte ptr [edx + 0x82], 1
        or     eax, 8
loc_1_3CF0:
        add    ax, 0x8000
        pop    ebp
        mov    word ptr [edx + 0xf0], ax
        mov    edx, dword ptr [edx + 0x15c]
        out    dx, ax
        xor    eax, eax
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3D18:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_1_3D3D:
        test   edx, edx
        je     loc_1_3D4C
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_3D3D
        mov    dword ptr [ecx], edx
loc_1_3D4C:
        cmp    dword ptr [ecx], -1
        jne    loc_1_3D5F
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_3D5F:
        mov    eax, dword ptr [0]            ; -> obj1:0x5644
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x49
        push   ecx
        call   sub_1_53D6
        test   eax, eax
        je     loc_1_3D84
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_3D84:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x14
        mov    ebx, dword ptr [ebp + 0x20]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 0x5e]
        push   eax
        push   ecx
        call   sub_2_0170
loc_1_3D9F:
        test   al, al
        jne    loc_1_3DBE
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x49
        push   eax
        call   sub_1_53DC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_3DBE:
        cmp    byte ptr [ebx + 0x83], 0
        je     loc_1_3DDD
        mov    eax, dword ptr [ebx + 0x5a]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, 0x103
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x5a], ecx
        jmp    loc_1_3DEE
loc_1_3DDD:
        mov    eax, dword ptr [ebx + 0x52]
        mov    ecx, dword ptr [ebp - 4]
        xor    esi, esi
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x52], ecx
loc_1_3DEE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 0x4a]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x2a], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edx + 0x18], eax
        cmp    dword ptr [ebx + 0x56], ecx
        jne    loc_1_3E42
        mov    eax, dword ptr [ebx + 4]
        lea    ecx, [ebx + 0x62]
        push   eax
        push   ecx
        call   sub_1_53B2
loc_1_3E42:
        push   ebx
        inc    dword ptr [ebx + 0x56]
        call   sub_1_4118
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_1_3E58:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x20], 0
        mov    ecx, dword ptr [esi]
        je     loc_1_3E71
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    4
loc_1_3E71:
        mov    byte ptr [esi + 0x20], 1
        inc    dword ptr [esi + 0x1c]
        mov    eax, dword ptr [ecx + 0x52]
        cmp    esi, eax
        jne    loc_1_3E87
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx + 0x52], eax
        jmp    loc_1_3E9A
loc_1_3E87:
        cmp    dword ptr [eax + 0xc], esi
        je     loc_1_3E94
loc_1_3E8C:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], esi
        jne    loc_1_3E8C
loc_1_3E94:
        mov    edx, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], edx
loc_1_3E9A:
        lea    eax, [esi + 0x21]
        mov    edx, dword ptr [esi + 0x14]
        push   eax
        mov    ecx, dword ptr [ecx + 0x5e]
        push   edx
        push   ecx
        call   sub_2_0240
loc_1_3EAB:
        test   eax, eax
        je     loc_1_3EB6
        cmp    eax, 0x103
        jne    loc_1_3EBC
loc_1_3EB6:
        push   esi
        call   sub_1_3EF8
loc_1_3EBC:
        push   esi
        call   sub_1_3EF8
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3ED8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_3EF8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x1c]
        dec    ecx
        mov    dword ptr [eax + 0x1c], ecx
        jne    loc_1_3F0E
        push   eax
        call   sub_1_3F18
loc_1_3F0E:
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3F18:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_1_4E38
        mov    eax, dword ptr [edi + 0x56]
        mov    ecx, 1
        dec    eax
        push   edi
        mov    dword ptr [edi + 0x56], eax
        cmp    ecx, eax
        mov    al, cl
        adc    al, 0xff
        mov    byte ptr [edi + 0xc], al
        call   sub_1_4148
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_1_5314
        push   0
        push   0x49
        push   esi
        call   sub_1_53DC
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F68:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax + 0x20], 0
        mov    esi, dword ptr [eax]
        je     loc_1_3F82
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    4
loc_1_3F82:
        mov    cl, byte ptr [esi + 0x21]
        test   cl, 2
        je     loc_1_3F94
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    4
loc_1_3F94:
        or     cl, 2
        push   0x40010004
        push   esi
        mov    byte ptr [esi + 0x21], cl
        inc    dword ptr [eax + 0x1c]
        mov    dword ptr [esi + 0x6e], eax
        call   sub_1_4098
        mov    byte ptr [esi + 0x83], 1
        push   esi
        call   sub_1_41E8
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3FC8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x11]
loc_1_3FD6:
        push   edi
        call   sub_1_4078
        test   eax, eax
        je     loc_1_3FFF
        or     byte ptr [eax + 0x2e], 0xa
        mov    dword ptr [eax + 0x24], ebx
        cmp    dword ptr [esi + 0x1d], ebx
        jne    loc_1_3FF4
        mov    dword ptr [esi + 0x1d], eax
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_3FD6
loc_1_3FF4:
        mov    ecx, dword ptr [esi + 0x19]
        mov    dword ptr [ecx + 0x24], eax
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_3FD6
loc_1_3FFF:
        lea    edi, [esi + 0x19]
loc_1_4002:
        push   edi
        call   sub_1_4078
        test   eax, eax
        je     loc_1_4015
        push   eax
        push   esi
        call   sub_1_06C8
        jmp    loc_1_4002
loc_1_4015:
        mov    edi, dword ptr [esi + 0x6e]
        push   0x40010005
        and    byte ptr [esi + 0x21], 0xfd
        mov    byte ptr [esi + 0x83], 0
        push   esi
        call   sub_1_4098
        push   0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5326
        push   edi
        call   sub_1_3EF8
        mov    ecx, dword ptr [esi + 0x5a]
        test   ecx, ecx
        je     loc_1_406C
loc_1_4046:
        mov    eax, dword ptr [esi + 0x5a]
        mov    ebx, dword ptr [esi + 0x52]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x5a], edx
        mov    dword ptr [ecx + 0xc], ebx
        mov    dword ptr [esi + 0x52], ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_530E
        mov    ecx, dword ptr [esi + 0x5a]
        test   ecx, ecx
        jne    loc_1_4046
loc_1_406C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4078:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     loc_1_408C
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 4], edx
loc_1_408C:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4098:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x52]
        test   edi, edi
        je     loc_1_40E1
        mov    esi, dword ptr [ebp + 0xc]
loc_1_40AF:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 0x1c]
        mov    ebx, dword ptr [edi + 0xc]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_533E
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_5344
        push   edi
        call   sub_1_3EF8
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_1_40AF
loc_1_40E1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40F8:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x5e]
        push   edx
        call   sub_1_5200
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4118:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x56], 1
        jne    loc_1_4139
        mov    edx, dword ptr [eax + 0x15c]
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, 0x4800
        out    dx, ax
loc_1_4139:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4148:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x56], 0
        jne    loc_1_4162
        mov    edx, dword ptr [eax + 0x15c]
        mov    eax, 0x1800
        out    dx, ax
loc_1_4162:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 055h, 0B8h, 000h, 058h, 000h, 000h, 08Bh, 0ECh, 056h, 08Bh, 04Dh, 008h, 08Bh, 0B1h
        db  05Ch, 001h, 000h, 000h, 08Bh, 089h, 060h, 001h, 000h, 000h, 08Bh, 0D6h, 066h, 0EFh, 08Bh, 0D1h
        db  066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 028h, 000h, 000h, 066h, 0EFh
        db  08Bh, 0D1h, 066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 078h, 000h, 000h
        db  066h, 0EFh, 0B8h, 0FFh, 068h, 000h, 000h, 05Eh, 066h, 0EFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_41B8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_0268
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_41D8:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_41E8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0xb9], 1
        call   sub_1_12B8
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        mov    dword ptr [esi + 0xf8], 0
        out    dx, ax
        mov    eax, 0x1800
        pop    esi
        out    dx, ax
        mov    eax, 0x7840
        pop    ebp
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4238:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        inc    dword ptr [esi + 0x1c]
        cmp    byte ptr [esi + 0x20], 0
        je     loc_1_4252
        mov    edi, 0xc0010002
        jmp    loc_1_428E
loc_1_4252:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        jne    loc_1_4268
        push   eax
        push   esi
        push   edx
        call   sub_1_47F8
        mov    edi, eax
        jmp    loc_1_428E
loc_1_4268:
        cmp    ecx, 1
        jne    loc_1_4289
        cmp    byte ptr [edx + 0x83], 0
        je     loc_1_427D
        mov    edi, 0xc001000d
        jmp    loc_1_428E
loc_1_427D:
        push   eax
        push   esi
        push   edx
        call   sub_1_4848
        mov    edi, eax
        jmp    loc_1_428E
loc_1_4289:
        mov    edi, 0x10001
loc_1_428E:
        push   esi
        call   sub_1_3EF8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_42A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 0x134]
        mov    word ptr [ebp - 6], bx
        mov    dword ptr [ebp - 0x20], ecx
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], ebx
        cmp    edx, 0x1010101
        mov    eax, edx
        mov    byte ptr [ebp - 4], bl
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 0x1c], ebx
        ja     loc_1_42F3
        je     loc_1_4639
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_4314
        jmp    dword ptr [eax*4]             ; -> obj1:0x477C
loc_1_42F3:
        cmp    eax, 0x1010102
        je     loc_1_4655
        cmp    eax, 0x1010103
        je     loc_1_4671
        cmp    eax, 0x1010104
        je     loc_1_471F
loc_1_4314:
        mov    eax, edx
        and    eax, 0xff000000
        cmp    eax, 0xff000000
        je     loc_1_432E
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_4730
loc_1_432E:
        add    edx, 0xfefeff
        cmp    edx, 8
        ja     loc_1_4340
        jmp    dword ptr [edx*4]             ; -> obj1:0x47C8
loc_1_4340:
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_4730
loc_1_434C:
        mov    al, byte ptr [esi + 0x456]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    loc_1_4736
loc_1_4362:
        mov    ax, word ptr [esi + 0x457]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_4736
loc_1_437A:
        mov    ax, word ptr [esi + 0x459]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_4736
loc_1_4392:
        mov    ax, word ptr [esi + 0x45b]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_4736
loc_1_43AA:
        mov    al, byte ptr [esi + 0x45d]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    loc_1_4736
loc_1_43C0:
        mov    ax, word ptr [esi + 0x460]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_4736
loc_1_43D8:
        mov    ax, word ptr [esi + 0x45e]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_4736
loc_1_43F0:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x460]
        lea    ecx, [eax + eax*4]
        lea    edi, [ecx*2]
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_4419
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], edi
        jmp    loc_1_4730
loc_1_4419:
        push   edi
        mov    eax, dword ptr [esi + 0x452]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    bl, 1
        call   sub_1_54BC
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx], edi
        jmp    loc_1_4730
loc_1_4436:
        cmp    dword ptr [ebp + 0x1c], 0x1e
        jae    loc_1_4451
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], 0x1e
        jmp    loc_1_4730
loc_1_4451:
        push   0x1e
        mov    eax, dword ptr [esi + 0x452]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    bl, 1
        call   sub_1_54BC
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx], 0x1e
        jmp    loc_1_4730
loc_1_4473:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_4488
        mov    eax, 0                        ; -> loc_1_56E4
        mov    esi, 0x5c
        jmp    loc_1_4736
loc_1_4488:
        mov    eax, 0                        ; -> loc_1_5648
        mov    esi, 0x9c
        jmp    loc_1_4736
loc_1_4497:
        cmp    byte ptr [esi + 0x83], 0
        mov    dword ptr [ebp - 0x1c], 2
        jne    loc_1_44AE
        mov    dword ptr [ebp - 0x1c], 0
loc_1_44AE:
        lea    eax, [ebp - 0x1c]
        mov    esi, 4
        jmp    loc_1_4736
loc_1_44BB:
        lea    eax, [ebp - 0x20]
        mov    esi, 4
        jmp    loc_1_4736
loc_1_44C8:
        mov    eax, dword ptr [esi + 0x138]
        mov    esi, 4
        sub    eax, 0xe
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_44E1:
        mov    eax, dword ptr [esi + 0x13c]
        mov    esi, 4
        sub    eax, 0xe
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_44FA:
        mov    ecx, 0x64
        mov    eax, dword ptr [esi + 0x140]
        sub    edx, edx
        mov    esi, 4
        div    ecx
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_4519:
        mov    eax, dword ptr [esi + 0x144]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_452F:
        mov    eax, dword ptr [esi + 0x148]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_4545:
        mov    eax, dword ptr [esi + 0x14c]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_455B:
        mov    eax, dword ptr [esi + 0x150]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_4571:
        push   3
        add    esi, 0x44
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        mov    esi, 4
        call   sub_1_54BC
        and    dword ptr [ebp - 0xc], 0xffffff00
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_4594:
        mov    eax, 0                        ; -> loc_1_5740
        mov    esi, 0x18
        jmp    loc_1_4736
loc_1_45A3:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_45BA
        test   ecx, ecx
        jne    loc_1_45B5
        mov    eax, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_45C9
loc_1_45B5:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_45C9
loc_1_45BA:
        xor    eax, eax
        test   ecx, ecx
        jne    loc_1_45C9
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x14]
        mov    eax, dword ptr [eax + 8]
loc_1_45C9:
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_45D9:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_45E6
        mov    eax, dword ptr [esi]
        sub    eax, 0xe
        jmp    loc_1_45EC
loc_1_45E6:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x18]
loc_1_45EC:
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_45FC:
        lea    eax, [ebp - 6]
        mov    esi, 2
        mov    word ptr [ebp - 6], 0x300
        jmp    loc_1_4736
loc_1_460F:
        mov    eax, dword ptr [esi + 0x13c]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_4736
loc_1_4625:
        lea    eax, [ebp - 0xc]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], 7
        jmp    loc_1_4736
loc_1_4639:
        push   6
        add    esi, 0x44
        push   esi
        lea    eax, [ebp - 0x28]
        push   eax
        mov    esi, 6
        call   sub_1_54BC
        lea    eax, [ebp - 0x28]
        jmp    loc_1_4736
loc_1_4655:
        push   6
        add    esi, 0x3e
        push   esi
        lea    eax, [ebp - 0x28]
        push   eax
        mov    esi, 6
        call   sub_1_54BC
        lea    eax, [ebp - 0x28]
        jmp    loc_1_4736
loc_1_4671:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_46C6
        mov    ecx, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_469B
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_1_4730
loc_1_469B:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x18]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0x10]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_0760
loc_1_46B1:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0x10], 0
        lea    eax, [eax + eax*2]
        mov    bl, 1
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_4730
loc_1_46C6:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        push   ecx
        call   sub_2_0660
loc_1_46D6:
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_46F1
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_1_4730
loc_1_46F1:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [edi + 0x14]
        push   edx
        mov    ecx, dword ptr [esi + 0x5e]
        push   eax
        lea    edx, [ebp - 0x10]
        push   ecx
        push   edx
        mov    bl, 1
        call   sub_2_06A0
loc_1_4710:
        mov    ecx, dword ptr [ebp - 0x18]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_4730
loc_1_471F:
        lea    eax, [ebp - 0xc]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], 0x10
        jmp    loc_1_4736
loc_1_4730:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0x14]
loc_1_4736:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_476D
        test   bl, bl
        jne    loc_1_476D
        cmp    dword ptr [ebp + 0x1c], esi
        jae    loc_1_475D
        mov    ecx, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_475D:
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_54BC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], esi
loc_1_476D:
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (28 entries) ----
        dd  loc_1_4473
        dd  loc_1_4497
        dd  loc_1_44BB
        dd  loc_1_44BB
        dd  loc_1_44C8
        dd  loc_1_44E1
        dd  loc_1_44FA
        dd  loc_1_4519
        dd  loc_1_452F
        dd  loc_1_4545
        dd  loc_1_455B
        dd  loc_1_4571
        dd  loc_1_4594
        dd  loc_1_45A3
        dd  loc_1_45D9
        dd  loc_1_45FC
        dd  loc_1_460F
        dd  loc_1_4314
        dd  loc_1_4625
        dd  loc_1_434C
        dd  loc_1_4362
        dd  loc_1_437A
        dd  loc_1_4392
        dd  loc_1_43AA
        dd  loc_1_43C0
        dd  loc_1_43D8
        dd  loc_1_43F0
        dd  loc_1_4436
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_47F8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x18]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_42A8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4848:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_1_4892
        je     loc_1_4A67
        cmp    eax, 0x1010e
        je     loc_1_49D5
        cmp    eax, 0x1010f
        je     loc_1_4A11
        cmp    eax, 0x10112
        je     loc_1_4A5E
loc_1_4892:
        mov    edx, eax
        and    edx, 0xff000000
        cmp    edx, 0xff000000
        je     loc_1_48B6
        mov    dword ptr [ebp - 4], 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_48B6:
        add    eax, 0xfefefe
        cmp    eax, 3
        ja     loc_1_48C7
        jmp    dword ptr [eax*4]             ; -> obj1:0x4AC4
loc_1_48C7:
        mov    dword ptr [ebp - 4], 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_48DB:
        cmp    ebx, 2
        je     loc_1_48F4
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_48F4:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x457
        push   edi
        call   sub_1_54BC
        jmp    loc_1_4AA5
loc_1_490B:
        cmp    ebx, 2
        je     loc_1_4924
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_4924:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x457
        push   edi
        call   sub_1_54BC
        jmp    loc_1_4AA5
loc_1_493B:
        cmp    ebx, 2
        je     loc_1_4954
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_4954:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x457
        push   edi
        call   sub_1_54BC
        jmp    loc_1_4AA5
loc_1_496B:
        cmp    ebx, 1
        je     loc_1_4984
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_4984:
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 0x45d], al
        je     loc_1_4AA5
        mov    byte ptr [edi + 0x45d], al
        test   al, al
        jne    loc_1_4AA5
        xor    edx, edx
        xor    eax, eax
        mov    word ptr [edi + 0x45e], 0
        mov    dx, word ptr [edi + 0x460]
        mov    edi, dword ptr [edi + 0x452]
        lea    edx, [edx + edx*4]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        jmp    loc_1_4AA5
loc_1_49D5:
        cmp    ebx, 4
        je     loc_1_49EE
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_49EE:
        push   4
        lea    eax, [ebp - 0xc]
        push   ecx
        push   eax
        call   sub_1_54BC
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edi
        call   sub_1_4DC8
        jmp    loc_1_4AA2
loc_1_4A11:
        cmp    ebx, 4
        je     loc_1_4A27
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_4A27:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_54BC
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x138]
        sub    eax, 0xe
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_4A55
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [eax + 0x18], ecx
        call   sub_1_4E38
        jmp    loc_1_4AA5
loc_1_4A55:
        mov    dword ptr [ebp - 4], 0xc0010014
        jmp    loc_1_4AA5
loc_1_4A5E:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_4AA5
loc_1_4A67:
        mov    edi, 6
        mov    eax, ebx
        sub    edx, edx
        div    edi
        test   edx, edx
        je     loc_1_4A87
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_4AA5
loc_1_4A87:
        push   ecx
        mov    eax, ebx
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_1_4D58
loc_1_4AA2:
        mov    dword ptr [ebp - 4], eax
loc_1_4AA5:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_4AB5
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], ebx
loc_1_4AB5:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_48DB
        dd  loc_1_490B
        dd  loc_1_493B
        dd  loc_1_496B
        db  08Dh, 064h, 024h, 000h
sub_1_4AD8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [esi + 0x14]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_42A8
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_4C7E
        xor    edi, edi
        push   ebx
        call   sub_1_41B8
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2020d
        ja     loc_1_4B5C
        je     loc_1_4BF7
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_4B8A
        jmp    dword ptr [eax*4]             ; -> obj1:0x4C98
loc_1_4B5C:
        cmp    eax, 0x1020201
        ja     loc_1_4B94
        je     loc_1_4C17
        cmp    eax, 0x1020101
        je     loc_1_4BFF
        cmp    eax, 0x1020102
        je     loc_1_4C07
        cmp    eax, 0x1020103
        je     loc_1_4C0F
loc_1_4B8A:
        mov    edi, 0xc0010017
        jmp    loc_1_4C50
loc_1_4B94:
        sub    eax, 0x1020202
        cmp    eax, 5
        ja     loc_1_4B8A
        jmp    dword ptr [eax*4]             ; -> obj1:0x4CAC
loc_1_4BA5:
        mov    eax, dword ptr [ebx + 0x1b8]
        jmp    loc_1_4C4D
loc_1_4BB0:
        mov    eax, dword ptr [ebx + 0x1bc]
        jmp    loc_1_4C4D
loc_1_4BBB:
        mov    eax, dword ptr [ebx + 0x1ec]
        add    eax, dword ptr [ebx + 0x1dc]
        add    eax, dword ptr [ebx + 0x1cc]
        add    eax, dword ptr [ebx + 0x1f0]
        jmp    loc_1_4C4D
loc_1_4BD5:
        mov    eax, dword ptr [ebx + 0x1f4]
        add    eax, dword ptr [ebx + 0x1f8]
        add    eax, dword ptr [ebx + 0x1fc]
        add    eax, dword ptr [ebx + 0x200]
        jmp    loc_1_4C4D
loc_1_4BEF:
        mov    eax, dword ptr [ebx + 0x200]
        jmp    loc_1_4C4D
loc_1_4BF7:
        mov    eax, dword ptr [ebx + 0x1f8]
        jmp    loc_1_4C4D
loc_1_4BFF:
        mov    eax, dword ptr [ebx + 0x1f4]
        jmp    loc_1_4C4D
loc_1_4C07:
        mov    eax, dword ptr [ebx + 0x1d4]
        jmp    loc_1_4C4D
loc_1_4C0F:
        mov    eax, dword ptr [ebx + 0x1d8]
        jmp    loc_1_4C4D
loc_1_4C17:
        mov    eax, dword ptr [ebx + 0x1e8]
        jmp    loc_1_4C4D
loc_1_4C1F:
        mov    eax, dword ptr [ebx + 0x1dc]
        jmp    loc_1_4C4D
loc_1_4C27:
        mov    eax, dword ptr [ebx + 0x200]
        jmp    loc_1_4C4D
loc_1_4C2F:
        mov    eax, dword ptr [ebx + 0x1cc]
        jmp    loc_1_4C4D
loc_1_4C37:
        mov    eax, dword ptr [ebx + 0x1d0]
        jmp    loc_1_4C4D
loc_1_4C3F:
        mov    eax, dword ptr [ebx + 0x1e0]
        jmp    loc_1_4C4D
loc_1_4C47:
        mov    eax, dword ptr [ebx + 0x1e4]
loc_1_4C4D:
        mov    dword ptr [ebp - 4], eax
loc_1_4C50:
        push   ebx
        call   sub_1_41D8
        test   edi, edi
        jne    loc_1_4C7E
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_4C75
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_54BC
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_4C7E
loc_1_4C75:
        add    dword ptr [ebp - 8], 4
        mov    edi, 0xc0010014
loc_1_4C7E:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_4BA5
        dd  loc_1_4BB0
        dd  loc_1_4BBB
        dd  loc_1_4BD5
        dd  loc_1_4BEF
        dd  loc_1_4C1F
        dd  loc_1_4C27
        dd  loc_1_4C2F
        dd  loc_1_4C37
        dd  loc_1_4C3F
        dd  loc_1_4C47
        db  08Dh, 064h, 024h, 000h
loc_1_4CC8:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x14]
        cmp    ecx, 0x20105
        ja     loc_1_4CFB
        cmp    ecx, 0x20101
        jae    loc_1_4D45
        cmp    ecx, 0x10101
        jb     loc_1_4D3C
        cmp    ecx, 0x10111
        jbe    loc_1_4D45
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_4CFB:
        cmp    ecx, 0x1010104
        ja     loc_1_4D1C
        cmp    ecx, 0x1010101
        jae    loc_1_4D45
        cmp    ecx, 0x2020d
        je     loc_1_4D45
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_4D1C:
        cmp    ecx, 0x1020207
        ja     loc_1_4D3C
        cmp    ecx, 0x1020201
        jae    loc_1_4D45
        cmp    ecx, 0x1020101
        jb     loc_1_4D3C
        cmp    ecx, 0x1020103
        jbe    loc_1_4D45
loc_1_4D3C:
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_4D45:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4AD8
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_4D58:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x83], 0
        push   edi
        jne    loc_1_4DAE
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_4DA1
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ecx + 0x5e]
        push   ecx
        call   sub_2_0300
loc_1_4D91:
        mov    edi, eax
        push   esi
        call   sub_1_3EF8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_4DA1:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_4DAE:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4DC8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        cmp    byte ptr [ecx + 0x83], 0
        jne    loc_1_4E24
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_4E17
        inc    dword ptr [esi + 0x1c]
        cmp    dword ptr [ecx + 0x134], edi
        jne    loc_1_4E09
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    edi, dword ptr [esi + 0x14]
        push   edi
        mov    eax, dword ptr [ecx + 0x5e]
        push   eax
        call   sub_2_05E0
loc_1_4E07:
        mov    edi, eax
loc_1_4E09:
        push   esi
        call   sub_1_3EF8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_4E17:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_4E24:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E38:
        push   ebp
        mov    ecx, 0x3c
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x52]
        test   edx, edx
        je     loc_1_4E5B
loc_1_4E4B:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_1_4E54
        mov    ecx, esi
loc_1_4E54:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_4E4B
loc_1_4E5B:
        add    ecx, 0xe
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    4
        db  08Dh, 049h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        add    eax, 0xac
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        push   esi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5406
        test   esi, esi
        je     loc_1_4EB5
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0xc]
loc_1_4E91:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        add    edi, 4
        mov    eax, dword ptr [edx + 0xac]
        lea    ecx, [ebp - 4]
        push   eax
        push   ebx
        push   ecx
        add    ebx, 4
        call   sub_1_5412
        dec    esi
        jne    loc_1_4E91
loc_1_4EB5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 0B9h, 000h, 000h, 000h, 000h
        db  08Bh, 0ECh, 056h, 08Bh, 075h, 00Ch, 08Bh, 046h, 00Ch, 085h, 0C0h, 074h, 010h, 08Bh, 056h, 004h
        db  08Dh, 00Ch, 010h, 0C1h, 0EAh, 00Ch, 049h, 0C1h, 0E9h, 00Ch, 02Bh, 0CAh, 041h, 08Bh, 045h, 018h
        db  039h, 008h, 072h, 010h, 050h, 08Bh, 045h, 014h, 050h, 056h, 0E8h, 02Dh, 005h, 000h, 000h, 05Eh
        db  05Dh, 0C2h, 014h, 000h, 05Eh, 089h, 008h, 05Dh, 0C2h, 014h, 000h, 08Dh, 064h, 024h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 00Ch, 08Bh
        db  0ECh, 050h, 0E8h, 0F9h, 004h, 000h, 000h, 08Bh, 045h, 008h, 08Bh, 088h, 0ACh, 000h, 000h, 000h
        db  051h, 0E8h, 0D8h, 004h, 000h, 000h, 05Dh, 0C2h, 008h, 000h
loc_1_4F38:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_00A8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4F58:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        call   sub_1_00E8
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_4F80:
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
        call   sub_1_4FB0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_4FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_5050
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_4FF0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_4FE5
        cmp    al, 0xff
        jne    loc_1_4FE5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_4FE5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_5057
loc_1_4FE5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_5057
        db  08Bh, 0FFh
loc_1_4FF0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_5040
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_5016
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_501D
loc_1_5016:
        mov    dword ptr [ebp - 0x10], 1
loc_1_501D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_5030
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_505A
        db  08Dh, 064h, 024h, 000h
loc_1_5030:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_505A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5040:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_505A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5050:
        mov    dword ptr [ebp - 4], 0x20
loc_1_5057:
        mov    esi, dword ptr [ebp + 8]
loc_1_505A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_5151
loc_1_5065:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    loc_1_50BD
        test   al, 2
        je     loc_1_50A9
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
        je     loc_1_50A9
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
        jne    loc_1_50BD
loc_1_50A9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_50B4
        test   bl, 4
        jne    loc_1_50BD
loc_1_50B4:
        test   bl, 0x20
        je     loc_1_5146
loc_1_50BD:
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
        call   sub_1_5332
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_5146
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_50FF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_50FF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_5110
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_5116
        db  08Bh, 0FFh
loc_1_5110:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_5116:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_5125
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5338
loc_1_5125:
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
        call   sub_1_53DC
        mov    edi, ebx
        jmp    loc_1_5149
loc_1_5146:
        mov    edi, dword ptr [edi + 0x10]
loc_1_5149:
        or     edi, edi
        jne    loc_1_5065
loc_1_5151:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_5160:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5170
        pop    ebp
        ret    4
sub_1_5170:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_51EE
loc_1_5180:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_51E7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5338
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_51E7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_51AC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_51AC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_51C0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_51C6
        db  005h, 000h, 000h, 000h, 000h
loc_1_51C0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_51C6:
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
        call   sub_1_53DC
        mov    edi, ebx
        jmp    loc_1_51EA
loc_1_51E7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_51EA:
        or     edi, edi
        jne    loc_1_5180
loc_1_51EE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5200:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_5215
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5215:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_5251
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_5242
        cmp    bl, 0xff
        jne    loc_1_5242
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_5242
        test   al, 8
        je     loc_1_523B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_523B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_5242:
        test   al, 4
        jne    loc_1_524A
        test   al, 2
        je     loc_1_5269
loc_1_524A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5251:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_5269
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_5269
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5269:
        sub    al, al
        pop    ebx
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
sub_1_52EA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_52F0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_52F6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_52FC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_5302:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_5308:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_530E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_5314:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_531A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_5326:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_5332:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_5338:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_533E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_5344:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_535C:
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
sub_1_53B2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_53B8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_53BE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_53C4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_53D0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_53D6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_53DC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_5400:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_5406:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_5412:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_5424:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_5448:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_544E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_5454:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_545A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_5460:
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
sub_1_54A8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_54B4:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_54BC:
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
sub_1_54DC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_54FC
        or     ecx, ecx
        je     loc_1_5512
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_5510
loc_1_54FC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_5510
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_5510:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5512:
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
        db  'ELNK3   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  ELNK3_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 045h, 04Ch, 04Eh, 04Bh
        db  033h, 000h, 000h, 000h
loc_1_5574:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     0x55e9
        insd   dword ptr es:[edi], dx
        jne    loc_1_55F0
        dec    ebp
        jne    loc_1_55F2
        je     0x55f1
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_55D9
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    ebp
        popal  
        js     loc_1_5601
        insd   dword ptr es:[edi], dx
        jne    loc_1_5608
        dec    ebp
        jne    0x560a
        je     loc_1_5609
        arpl   word ptr [ecx + 0x73], sp
        je     0x55f1
        imul   esi, dword ptr [ebx + 0x74], 0
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_5625
        arpl   word ptr [ebp + 0x43], sp
        outsd  dx, dword ptr [esi]
        jne    loc_1_5627
        je     0x55bb
        db  000h
loc_1_55BC:
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_5635
        arpl   word ptr [ebp + 0x43], sp
        outsd  dx, dword ptr [esi]
        jne    0x5637
        je     loc_1_55CB
loc_1_55CB:
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
loc_1_55D9:
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
        inc    edx
        jne    0x565a
        push   esp
        jns    0x565a
        add    byte ptr gs:[edx + 0x75], al
        jae    0x5644
loc_1_55F0:
        jns    0x5662
loc_1_55F2:
        add    byte ptr gs:[ecx + 0x4f], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        db  000h
loc_1_5600:
        dec    ecx
loc_1_5601:
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
loc_1_5608:
        push   ebx
loc_1_5609:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4c], dl
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5614:
        push   ebx
        dec    esp
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        inc    esp
        push   eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
loc_1_5624:
        dec    ecx
loc_1_5625:
        inc    esp
        push   eax
loc_1_5627:
        dec    edi
        push   edx
        push   esp
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
loc_1_5635:
        add    byte ptr [eax], al
        db  000h
loc_1_5638:
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
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_5648:
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
        add    eax, 0x6010202
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx + eax], eax
        add    al, byte ptr [ecx]
loc_1_56E4:
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
loc_1_5740:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    esp, dword ptr [eax]
        inc    ebp
        je     loc_1_57B1
        jb     0x57ba
        je     loc_1_576F
        inc    ecx
        popal  
        jo     loc_1_57C8
        jb     loc_1_5757
loc_1_5757:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_576F:
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
loc_1_5799:
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
loc_1_57B1:
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
        db  000h
loc_1_57C8:
        add    byte ptr [eax], al
loc_1_57CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57CE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57E6:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x5760]       ; -> obj1:0x5760
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_53D6
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
        call   sub_1_53D6
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
        call   sub_1_53D6
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
        call   sub_1_53DC
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_53DC
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_53DC
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
        mov    eax, dword ptr [0x5760]       ; -> obj1:0x5760
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_53D6
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
        call   sub_1_5338
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_53DC
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
        call   sub_1_54DC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_54DC
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
        call   sub_1_54DC
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
        call   sub_1_54DC
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
        call   sub_1_54DC
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
        call   sub_1_54DC
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
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_2_0565
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_0552
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
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
        call   sub_1_54DC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_54DC
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
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_2_04F0
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
        call   sub_1_54DC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_54DC
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
        cmp    ecx, ebx
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
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 0x24], 0
        jbe    loc_2_071C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_54DC
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
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
        call   sub_1_54DC
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
        cmp    edx, ecx
        jb     loc_2_0859
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
