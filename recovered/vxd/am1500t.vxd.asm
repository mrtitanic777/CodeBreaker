; AM1500T -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5155 instructions recovered by recursive descent

Declare_Virtual_Device AM1500T, 3, 10, AM1500T_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2934  R-X
sub_1_0000:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0050
        push   esi
        call   sub_1_03C0
        cmp    al, 1
        jne    loc_1_0024
        push   esi
        call   sub_1_0030
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_0024:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0030:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 0x42
        out    dx, ax
        pop    ebp
        ret    4
        db  090h
sub_1_0050:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 4
        out    dx, ax
        pop    ebp
        ret    4
        db  090h
loc_1_0070:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x49]
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_00B0
        lea    eax, [edi + 0x18]
        mov    dword ptr [edi + 0x28], 4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        push   eax
        push   esi
        call   sub_1_16B0
        dec    dword ptr [edi + 0x14]
        mov    edi, 0x103
        jmp    loc_1_00B5
        db  005h, 000h, 000h, 000h, 000h
loc_1_00B0:
        mov    edi, 0xc0010002
loc_1_00B5:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_0100
        cmp    dword ptr [esi + 0x49], 2
        jne    loc_1_00CA
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_00E6
loc_1_00CA:
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_00E6
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_00E6
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_0100
loc_1_00E6:
        mov    byte ptr [esi + 0x11], 1
        push   0
        add    esi, 0x12
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0100:
        dec    dword ptr [esi + 0x49]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0110:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x2930]       ; -> obj1:0x2930
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0ADC
        push   0
        mov    eax, dword ptr [0x292c]       ; -> obj1:0x292C
        push   eax
        call   sub_1_0AD0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 2], 1
        call   sub_1_0050
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0C94
        cmp    byte ptr [esi], 1
        jne    loc_1_0179
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x1e]
        push   eax
        call   sub_1_0B98
        push   0x2625a0
        call   sub_1_0BB0
loc_1_0179:
        lea    eax, [esi + 0x2b]
        push   eax
        call   sub_1_0BA4
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        call   sub_2_0D80
        push   esi
        call   sub_1_0650
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0AE8
        push   0
        push   0x248
        push   esi
        call   sub_1_0BBC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [edi + 8]
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_01E0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_01E0:
        inc    dword ptr [ebx + 0x49]
        cmp    byte ptr [ebx + 0xe8], 0
        jne    loc_1_02E0
        cmp    byte ptr [ebx + 0xe9], 0
        jne    loc_1_02E0
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_02D0
        cmp    byte ptr [ebx + 0xea], 0
        jne    loc_1_02E7
        mov    byte ptr [ebx + 0xe9], 1
        inc    dword ptr [edi + 0x14]
        mov    esi, dword ptr [ebx + 0x4d]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -0x4d
        je     loc_1_0256
loc_1_022A:
        inc    dword ptr [esi + 0x14]
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0B1E
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0B24
        dec    dword ptr [esi + 0x14]
        mov    esi, dword ptr [esi]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -0x4d
        jne    loc_1_022A
loc_1_0256:
        mov    byte ptr [ebx + 0xe9], 0
        push   edi
        push   ebx
        call   sub_1_05B0
        push   ebx
        call   sub_1_0300
        push   ebx
        call   sub_1_0030
        mov    byte ptr [ebx + 0xe8], 0
        mov    byte ptr [ebx + 0xea], 1
        cmp    dword ptr [ebx + 0x79], 0
        je     loc_1_028A
        push   ebx
        call   sub_1_16F0
loc_1_028A:
        mov    esi, dword ptr [ebx + 0x4d]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -0x4d
        je     loc_1_02C2
loc_1_0296:
        inc    dword ptr [esi + 0x14]
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0B1E
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0B24
        dec    dword ptr [esi + 0x14]
        mov    esi, dword ptr [esi]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -0x4d
        jne    loc_1_0296
loc_1_02C2:
        mov    byte ptr [ebx + 0xea], 0
        dec    dword ptr [edi + 0x14]
        jmp    loc_1_02E7
        db  08Bh, 0FFh
loc_1_02D0:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_02E7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_02E0:
        mov    dword ptr [ebp - 4], 0xc001000d
loc_1_02E7:
        dec    dword ptr [ebx + 0x49]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0050
        cmp    dword ptr [esi + 0xd0], 0
        mov    word ptr [esi + 0x43], 0
        je     loc_1_0350
loc_1_0320:
        mov    edx, dword ptr [esi + 0xd0]
        push   0xc001000c
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0xd0], ecx
        mov    edi, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0AFA
        dec    dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_0320
loc_1_0350:
        mov    dword ptr [esi + 0xd0], 0
        mov    dword ptr [esi + 0xd4], 0
        cmp    dword ptr [esi + 0xdc], 0
        je     loc_1_03A0
        lea    ecx, [ecx]
loc_1_0370:
        mov    edx, dword ptr [esi + 0xdc]
        push   0xc001000c
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0xdc], ecx
        mov    edi, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0AFA
        dec    dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_0370
loc_1_03A0:
        mov    dword ptr [esi + 0xdc], 0
        mov    dword ptr [esi + 0xe0], 0
        push   esi
        call   sub_1_03C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
sub_1_03C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    dl, dl
        mov    edi, dword ptr [esi + 0x99]
        mov    eax, dword ptr [esi + 0x95]
        cmp    dword ptr [esi + 0x8d], 0
        jbe    loc_1_043F
loc_1_03E3:
        mov    cl, dl
        and    ecx, 0xffff00ff
        mov    bx, cx
        imul   bx, bx, 0x5ea
        add    bx, ax
        mov    word ptr [edi], bx
        and    ecx, 0xffff
        mov    ebx, ecx
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ebx + ecx*4]
        mov    word ptr [edi + 4], 0xf000
        lea    ecx, [ecx + ecx*8]
        lea    ecx, [ebx + ecx*4]
        lea    ecx, [eax + ecx*2]
        shr    ecx, 0x10
        and    cx, 0xff
        mov    word ptr [edi + 2], cx
        or     word ptr [edi + 2], 0x300
        mov    word ptr [edi + 6], 0
        inc    dl
        add    edi, 8
        xor    ecx, ecx
        mov    cl, dl
        cmp    ecx, dword ptr [esi + 0x8d]
        jb     loc_1_03E3
loc_1_043F:
        sub    cl, cl
        cmp    dword ptr [esi + 0x8d], 0
        jbe    loc_1_0469
        mov    edx, 1
        nop    
loc_1_0450:
        xor    eax, eax
        mov    al, cl
        mov    byte ptr [esi + eax + 0xa2], dl
        inc    cl
        xor    eax, eax
        mov    al, cl
        cmp    eax, dword ptr [esi + 0x8d]
        jb     loc_1_0450
loc_1_0469:
        mov    byte ptr [esi + 0xa1], 0
        mov    eax, dword ptr [esi + 0xbe]
        mov    edi, dword ptr [esi + 0xba]
        sub    dl, dl
        cmp    dword ptr [esi + 0xb2], 0
        jbe    loc_1_04E5
loc_1_0487:
        mov    cl, dl
        and    ecx, 0xffff00ff
        mov    bx, cx
        imul   bx, bx, 0x5ee
        add    bx, di
        mov    word ptr [eax], bx
        and    ecx, 0xffff
        mov    ebx, ecx
        shl    ecx, 6
        sub    ecx, ebx
        lea    ecx, [ebx + ecx*4]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [edi + ecx*2]
        shr    ecx, 0x10
        and    cx, 0xff
        mov    word ptr [eax + 2], cx
        or     cx, 0x8000
        mov    word ptr [eax + 2], cx
        mov    word ptr [eax + 4], 0xfa12
        mov    word ptr [eax + 6], 0
        inc    dl
        add    eax, 8
        xor    ecx, ecx
        mov    cl, dl
        cmp    ecx, dword ptr [esi + 0xb2]
        jb     loc_1_0487
loc_1_04E5:
        mov    cx, word ptr [esi + 0x45]
        mov    word ptr [esi + 0xc6], 0
        lea    edi, [ecx + 0x12]
        mov    dx, di
        mov    ax, 3
        out    dx, ax
        add    cx, 0x10
        mov    dx, cx
        mov    ax, 0x200
        out    dx, ax
        mov    dx, di
        mov    ax, 2
        out    dx, ax
        mov    ebx, dword ptr [esi + 0xcc]
        mov    eax, ebx
        shr    eax, 0x10
        mov    dx, cx
        and    ax, 0xff
        out    dx, ax
        mov    dx, di
        mov    ax, 1
        out    dx, ax
        mov    dx, cx
        mov    ax, bx
        out    dx, ax
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        mov    dx, cx
        mov    ax, 1
        out    dx, ax
        mov    edi, 0xc350
        mov    ebx, 0x100
loc_1_0554:
        mov    cx, word ptr [esi + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        in     ax, dx
        test   bx, ax
        jne    loc_1_0576
        push   0xa
        call   sub_1_0BB0
        dec    edi
        jne    loc_1_0554
        jmp    loc_1_058B
loc_1_0576:
        mov    cx, word ptr [esi + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 0xff00
        out    dx, ax
loc_1_058B:
        or     edi, edi
        jne    loc_1_059A
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_059A:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0050
        mov    byte ptr [esi + 0xe8], 1
        mov    ecx, dword ptr [esi + 0x79]
        lea    edi, [esi + 0x79]
        or     ecx, ecx
        je     loc_1_061F
        cmp    dword ptr [ecx + 0x10], 4
        jne    loc_1_05E2
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ecx + 4]
        dec    dword ptr [eax + 0x14]
        mov    ecx, dword ptr [edi]
loc_1_05E2:
        or     ecx, ecx
        je     loc_1_0618
loc_1_05E6:
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 4
        je     loc_1_0610
        cmp    eax, 3
        je     loc_1_0610
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [ecx + 4]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0B0C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_0612
        db  08Bh, 0FFh
loc_1_0610:
        mov    edi, ecx
loc_1_0612:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        jne    loc_1_05E6
loc_1_0618:
        mov    byte ptr [esi + 0xeb], 0
loc_1_061F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0630:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    byte ptr [eax + 2], 1
        call   sub_1_0050
        push   0x3d090
        call   sub_1_0BB0
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0650:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0xb2]
        mov    eax, esi
        shl    esi, 7
        sub    esi, eax
        lea    esi, [eax + esi*4]
        lea    esi, [esi + esi*2]
        sub    esi, eax
        mov    eax, dword ptr [edx + 0x8d]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        lea    eax, [esi + eax + 0x1f]
        mov    ecx, dword ptr [edx + 0x89]
        or     ecx, ecx
        je     loc_1_06A4
        mov    esi, dword ptr [edx + 0x85]
        push   esi
        push   ecx
        push   0
        push   eax
        mov    eax, dword ptr [edx + 0x71]
        push   eax
        call   sub_1_0B48
loc_1_06A4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06B0:
        mov    al, byte ptr [0xd48]          ; -> obj1:0x0D48
        cmp    al, 0
        jne    loc_1_06D5
        inc    byte ptr [0xd48]              ; -> obj1:0x0D48
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_06D7
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_06D7
loc_1_06D5:
        clc    
        ret    
loc_1_06D7:
        stc    
        ret    
AM1500T_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_06E8
        jmp    dword ptr [eax*4 + 0x6ea]     ; -> obj1:0x06EA
loc_1_06E8:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_06B0
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06E8
        dd  loc_1_06B0
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0760:
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
        call   sub_1_0790
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_0830
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_07D0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_07C5
        cmp    al, 0xff
        jne    loc_1_07C5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_07C5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_0837
loc_1_07C5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0837
        db  08Bh, 0FFh
loc_1_07D0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_0820
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_07F6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_07FD
loc_1_07F6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_07FD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0810
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_083A
        db  08Dh, 064h, 024h, 000h
loc_1_0810:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_083A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0820:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_083A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0830:
        mov    dword ptr [ebp - 4], 0x20
loc_1_0837:
        mov    esi, dword ptr [ebp + 8]
loc_1_083A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0931
loc_1_0845:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_089D
        test   al, 2
        je     loc_1_0889
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_1420
        or     al, al
        je     loc_1_0889
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_089D
loc_1_0889:
        test   byte ptr [ebp - 4], 2
        je     loc_1_0894
        test   bl, 4
        jne    loc_1_089D
loc_1_0894:
        test   bl, 0x20
        je     loc_1_0926
loc_1_089D:
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
        call   sub_1_0B12
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0926
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_08DF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_08DF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_08F0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_08F6
        db  08Bh, 0FFh
loc_1_08F0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_08F6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0905
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
loc_1_0905:
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
        call   sub_1_0BBC
        mov    edi, ebx
        jmp    loc_1_0929
loc_1_0926:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0929:
        or     edi, edi
        jne    loc_1_0845
loc_1_0931:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0950
        pop    ebp
        ret    4
sub_1_0950:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_09CE
loc_1_0960:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_09C7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_09C7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_098C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_098C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_09A0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_09A6
        db  005h, 000h, 000h, 000h, 000h
loc_1_09A0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_09A6:
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
        call   sub_1_0BBC
        mov    edi, ebx
        jmp    loc_1_09CA
loc_1_09C7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_09CA:
        or     edi, edi
        jne    loc_1_0960
loc_1_09CE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_09F5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_09F5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_0A31
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_0A22
        cmp    bl, 0xff
        jne    loc_1_0A22
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_0A22
        test   al, 8
        je     loc_1_0A1B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A1B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A22:
        test   al, 4
        jne    loc_1_0A2A
        test   al, 2
        je     loc_1_0A49
loc_1_0A2A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A31:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0A49
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_0A49
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A49:
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
sub_1_0ACA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_0AD0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_0AD6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_0ADC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_0AE2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_0AE8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_0AEE:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0AF4:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_0AFA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_0B0C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0B12:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0B18:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_0B1E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_0B24:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_0B2A:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_0B3C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_0B42:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_0B48:
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
sub_1_0B92:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_0B98:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_0B9E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_0BA4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_0BB0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_0BB6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_0BBC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0BE0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0C04:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0C28:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_0C2E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0C34:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_0C3A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0C40:
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
sub_1_0C88:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
sub_1_0C8E:
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_0C94:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_0C9C:
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
sub_1_0CBC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_0CDC
        or     ecx, ecx
        je     loc_1_0CF2
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_0CF0
loc_1_0CDC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_0CF0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_0CF0:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0CF2:
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
        db  'AM1500T '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  AM1500T_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd0]
        mov    eax, dword ptr [edi + 0x28]
        lea    ecx, [edi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ecx], 0
        jne    loc_1_0D7A
        mov    dword ptr [esi + 0xd4], 0
loc_1_0D7A:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0xd0], eax
        mov    dword ptr [esi + 0xd8], edi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0BE0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0C04
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_0DC3
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_0E18
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    loc_1_0E08
loc_1_0DC3:
        cmp    dword ptr [ebp - 4], 0xde
        jae    loc_1_0DF0
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_0DF0
        lea    ebx, [esi + 0x178]
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   0xd0
        push   0xe
        push   edi
        call   sub_1_1370
        jmp    loc_1_0DFA
        db  08Bh, 0FFh
loc_1_0DF0:
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0xe
        sub    dword ptr [ebp - 4], 0xe
loc_1_0DFA:
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        push   0xe
loc_1_0E08:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        call   sub_1_0760
loc_1_0E18:
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0AFA
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_0E39
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        call   sub_1_0940
loc_1_0E39:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        dec    dword ptr [eax + 0x14]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0E50:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 0x28]
        lea    esi, [edx + 0x24]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xd0], 0
        jne    loc_1_0E74
        mov    dword ptr [eax + 0xd0], edx
        jmp    loc_1_0E7D
loc_1_0E74:
        mov    ecx, dword ptr [eax + 0xd4]
        mov    dword ptr [ecx + 0x24], edx
loc_1_0E7D:
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 0xd4], edx
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    ax, ax
        mov    cx, word ptr [edi + 0x45]
        lea    esi, [ecx + 0x12]
        mov    dx, si
        out    dx, ax
        add    cx, 0x10
        mov    dx, cx
        out    dx, ax
        mov    dx, si
        out    dx, ax
        mov    dx, cx
        in     ax, dx
        mov    si, word ptr [edi + 0x45]
        mov    word ptr [edi + 0x43], ax
        lea    ecx, [esi + 0x12]
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        add    si, 0x10
        mov    ax, word ptr [edi + 0x43]
        mov    dx, si
        out    dx, ax
        test   ah, 0x84
        je     loc_1_0EE7
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0EE7:
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        mov    ax, 0x40
        out    dx, ax
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0F00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 2], 0
        jne    loc_1_100A
        cmp    byte ptr [esi + 0xf], 0
        jne    loc_1_100A
        mov    byte ptr [esi + 0xf], 1
        mov    byte ptr [esi + 0x2a], 0
        test   byte ptr [esi + 0x44], 4
        je     loc_1_0F3E
        inc    dword ptr [esi + 0x49]
        push   esi
        call   sub_1_1110
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        call   sub_1_0940
        dec    dword ptr [esi + 0x49]
loc_1_0F3E:
        test   byte ptr [esi + 0x44], 0x80
        je     loc_1_0FAE
        inc    dword ptr [esi + 0x49]
        test   byte ptr [esi + 0x44], 0x20
        je     loc_1_0F53
        inc    dword ptr [esi + 0x16c]
loc_1_0F53:
        test   byte ptr [esi + 0x44], 0x10
        je     loc_1_0F80
        inc    dword ptr [esi + 0xfc]
        push   esi
        call   sub_1_0050
        push   esi
        call   sub_1_03C0
        mov    cx, word ptr [esi + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 2
        out    dx, ax
loc_1_0F80:
        mov    ax, word ptr [esi + 0x43]
        and    al, 0x33
        cmp    al, 0x33
        je     loc_1_0FAB
        push   esi
        call   sub_1_0050
        push   esi
        call   sub_1_03C0
        mov    cx, word ptr [esi + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 2
        out    dx, ax
loc_1_0FAB:
        dec    dword ptr [esi + 0x49]
loc_1_0FAE:
        mov    byte ptr [esi + 0xf], 0
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_0FF5
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_0FE3
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_0FE3
        cmp    byte ptr [esi + 0xe8], 0
        je     loc_1_0FD9
        cmp    dword ptr [esi + 0x49], 1
        je     loc_1_0FE3
loc_1_0FD9:
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_0FF5
loc_1_0FE3:
        mov    byte ptr [esi + 0x11], 1
        inc    dword ptr [esi + 0x49]
        push   0
        lea    eax, [esi + 0x12]
        push   eax
        call   sub_1_0B92
loc_1_0FF5:
        mov    cx, word ptr [esi + 0x45]
        sub    ax, ax
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 0x40
        out    dx, ax
loc_1_100A:
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
loc_1_1010:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        sub    ebx, ebx
        add    eax, 0
loc_1_1020:
        cmp    dword ptr [esi + 0xd0], ebx
        jne    loc_1_1048
        cmp    dword ptr [esi + 0xdc], ebx
        jne    loc_1_1048
        cmp    byte ptr [esi + 0xe8], bl
        je     loc_1_103E
        cmp    dword ptr [esi + 0x49], 2
        je     loc_1_1048
loc_1_103E:
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_10C5
loc_1_1048:
        cmp    byte ptr [esi + 0xe8], bl
        je     loc_1_1056
        cmp    dword ptr [esi + 0x49], 2
        je     loc_1_10B1
loc_1_1056:
        cmp    dword ptr [esi + 0xdc], ebx
        je     loc_1_1068
        push   esi
        call   sub_1_2650
        cmp    al, bl
        je     loc_1_10C5
loc_1_1068:
        cmp    dword ptr [esi + 0xd0], ebx
        je     loc_1_1076
        push   esi
        call   sub_1_0D50
loc_1_1076:
        mov    ecx, dword ptr [esi + 0x59]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_1020
        cmp    dword ptr [ecx + 0x14], ebx
        jne    loc_1_1020
        mov    edi, dword ptr [ecx + 0x10]
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        push   ebx
        push   0x45
        push   ecx
        call   sub_1_0BBC
        dec    dword ptr [esi + 0x55]
        push   ebx
        push   edi
        call   sub_1_0AF4
        jmp    loc_1_1020
loc_1_10B1:
        mov    byte ptr [esi + 0x11], 0
        dec    dword ptr [esi + 0x49]
        push   esi
        call   sub_1_0300
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_10C5:
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_10F0
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_10F0
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        jne    loc_1_10F0
        mov    byte ptr [esi + 0x11], 0
        dec    dword ptr [esi + 0x49]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  090h
loc_1_10F0:
        push   0
        lea    eax, [esi + 0x12]
        push   eax
        call   sub_1_0B92
        dec    dword ptr [esi + 0x49]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0xc6]
        mov    eax, ecx
        and    eax, 0xffff
        shl    eax, 3
        add    eax, dword ptr [esi + 0xbe]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        mov    bx, word ptr [edx + 2]
        and    bx, 0xff00
        test   bh, 0x80
        jne    loc_1_1329
        mov    edi, dword ptr [ebp - 0x14]
loc_1_1150:
        mov    ax, word ptr [esi + 0xc6]
        inc    ax
        mov    word ptr [esi + 0xc6], ax
        and    eax, 0xffff
        cmp    eax, dword ptr [esi + 0xb2]
        jb     loc_1_1176
        mov    word ptr [esi + 0xc6], 0
loc_1_1176:
        mov    ax, bx
        and    ax, 0x4000
        mov    word ptr [ebp - 0xc], ax
        or     ax, ax
        jne    loc_1_123E
        test   bh, 1
        je     loc_1_123E
        test   bh, 2
        je     loc_1_123E
        mov    edx, dword ptr [ebp - 8]
        xor    edi, edi
        mov    di, word ptr [edx + 6]
        sub    edi, 4
        cmp    edi, 0x5ea
        mov    edx, edi
        jb     loc_1_11B7
        mov    edx, 0x5ea
loc_1_11B7:
        and    ecx, 0xffff
        mov    eax, ecx
        shl    ecx, 6
        sub    ecx, eax
        lea    ecx, [eax + ecx*4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 0xb6]
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_1329
        mov    al, byte ptr [ecx]
        test   al, 1
        je     loc_1_1208
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_1200
        cmp    al, 0xff
        jne    loc_1_1200
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_1200
        mov    word ptr [ebp - 2], 4
        jmp    loc_1_120E
        db  08Dh, 049h, 000h
loc_1_1200:
        mov    word ptr [ebp - 2], 2
        jmp    loc_1_120E
loc_1_1208:
        mov    word ptr [ebp - 2], 1
loc_1_120E:
        cmp    edi, 0xe
        jae    loc_1_1224
        cmp    edi, 6
        jb     loc_1_123E
        lea    eax, [ecx + 0xe]
        push   0
        push   0
        push   0
        push   edi
        jmp    loc_1_1232
loc_1_1224:
        lea    eax, [edi - 0xe]
        push   eax
        sub    edx, 0xe
        push   edx
        lea    eax, [ecx + 0xe]
        push   eax
        push   0xe
loc_1_1232:
        push   ecx
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        call   sub_1_0760
loc_1_123E:
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [edx + 2]
        and    ax, 0xff
        mov    word ptr [edx + 2], ax
        or     ax, 0x8000
        cmp    word ptr [ebp - 0xc], 0
        mov    word ptr [edx + 2], ax
        jne    loc_1_12D0
        inc    dword ptr [esi + 0xf0]
        cmp    word ptr [ebp - 2], 4
        jne    loc_1_1288
        inc    dword ptr [esi + 0x144]
        mov    eax, dword ptr [esi + 0x128]
        lea    ecx, [esi + 0x128]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    loc_1_12FB
        inc    dword ptr [ecx + 4]
        jmp    loc_1_12FB
loc_1_1288:
        cmp    word ptr [ebp - 2], 2
        jne    loc_1_12B0
        inc    dword ptr [esi + 0x140]
        mov    eax, dword ptr [esi + 0x120]
        lea    ecx, [esi + 0x120]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    loc_1_12FB
        inc    dword ptr [ecx + 4]
        jmp    loc_1_12FB
        db  08Bh, 0FFh
loc_1_12B0:
        inc    dword ptr [esi + 0x13c]
        mov    eax, dword ptr [esi + 0x118]
        lea    ecx, [esi + 0x118]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    loc_1_12FB
        inc    dword ptr [ecx + 4]
        jmp    loc_1_12FB
        db  090h
loc_1_12D0:
        inc    dword ptr [esi + 0xf8]
        test   bh, 8
        je     loc_1_12E3
        inc    dword ptr [esi + 0x178]
        jmp    loc_1_12FB
loc_1_12E3:
        test   bh, 0x20
        je     loc_1_12F0
        inc    dword ptr [esi + 0x150]
        jmp    loc_1_12FB
loc_1_12F0:
        test   bh, 4
        je     loc_1_12FB
        inc    dword ptr [esi + 0x164]
loc_1_12FB:
        mov    cx, word ptr [esi + 0xc6]
        mov    eax, ecx
        and    eax, 0xffff
        shl    eax, 3
        add    eax, dword ptr [esi + 0xbe]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        mov    bx, word ptr [edx + 2]
        and    bx, 0xff00
        test   bh, 0x80
        je     loc_1_1150
loc_1_1329:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1340:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x2a], 0
        je     loc_1_1353
        push   esi
        call   0
loc_1_1353:
        mov    byte ptr [esi + 0x2a], 1
        push   0x1388
        add    esi, 0x1e
        push   esi
        call   sub_1_0B92
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1370:
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
        je     loc_1_143A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_143A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_1435
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_1_13C0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_13EC
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C28
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1435
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        jmp    loc_1_1430
loc_1_13EC:
        or     edi, edi
        je     loc_1_1409
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_1401
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1430
loc_1_1401:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1409:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_1417
        mov    ebx, eax
loc_1_1417:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        add    esi, ebx
        push   eax
        call   sub_1_0C9C
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1430:
        cmp    esi, dword ptr [ebp + 0x10]
        jb     loc_1_13C0
loc_1_1435:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_143A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1450:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_151B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 0x14], esi
        je     loc_1_151B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_1516
        mov    edi, dword ptr [ebp + 0x14]
loc_1_14A4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_14D0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C28
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1516
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        jmp    loc_1_1511
loc_1_14D0:
        or     edi, edi
        je     loc_1_14ED
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_1_14E5
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1511
loc_1_14E5:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_14ED:
        mov    ebx, dword ptr [ebp + 0xc]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_14FB
        mov    ebx, eax
loc_1_14FB:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    esi, ebx
        push   eax
        call   sub_1_0C9C
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1511:
        cmp    dword ptr [ebp + 0xc], esi
        ja     loc_1_14A4
loc_1_1516:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_151B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1530:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x49]
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_15A7
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_15A0
        inc    dword ptr [edi + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        or     ecx, ecx
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        jne    loc_1_1580
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0xe
        push   eax
        mov    eax, dword ptr [esi + 0xd8]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0C2E
        dec    dword ptr [edi + 0x14]
        sub    edi, edi
        jmp    loc_1_15AC
        db  090h
loc_1_1580:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, ecx
        push   eax
        call   sub_1_1450
        dec    dword ptr [edi + 0x14]
        sub    edi, edi
        jmp    loc_1_15AC
        db  08Dh, 064h, 024h, 000h
loc_1_15A0:
        mov    edi, 0xc001000c
        jmp    loc_1_15AC
loc_1_15A7:
        mov    edi, 0xc001000d
loc_1_15AC:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_15F4
        cmp    dword ptr [esi + 0x49], 2
        jne    loc_1_15C1
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_15DD
loc_1_15C1:
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_15DD
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_15DD
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_15F4
loc_1_15DD:
        mov    byte ptr [esi + 0x11], 1
        push   0
        add    esi, 0x12
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_15F4:
        dec    dword ptr [esi + 0x49]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  090h
loc_1_1600:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 0x49]
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_1657
        cmp    byte ptr [eax + 0x40], 0
        jne    loc_1_1650
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x10]
        or     ecx, ecx
        jl     loc_1_162C
        cmp    ecx, 1
        jle    loc_1_1633
loc_1_162C:
        mov    edi, 0xc00000bb
        jmp    loc_1_165C
loc_1_1633:
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx], 0
        push   edx
        mov    edi, 0x103
        push   esi
        call   sub_1_16B0
        jmp    loc_1_165C
        db  08Dh, 049h, 000h
loc_1_1650:
        mov    edi, 0xc0010002
        jmp    loc_1_165C
loc_1_1657:
        mov    edi, 0xc001000d
loc_1_165C:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_16A4
        cmp    dword ptr [esi + 0x49], 2
        jne    loc_1_1671
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_168D
loc_1_1671:
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_168D
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_168D
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_16A4
loc_1_168D:
        mov    byte ptr [esi + 0x11], 1
        push   0
        add    esi, 0x12
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_16A4:
        dec    dword ptr [esi + 0x49]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_16B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x79], 0
        je     loc_1_16D0
        mov    edx, dword ptr [eax + 0x7d]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 0x7d], ecx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_16D0:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x79], ecx
        mov    dword ptr [eax + 0x7d], ecx
        cmp    byte ptr [eax + 0xeb], 0
        jne    loc_1_16E8
        push   eax
        call   sub_1_16F0
loc_1_16E8:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_16F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_16FC:
        mov    edi, dword ptr [esi + 0x79]
        or     edi, edi
        je     loc_1_18F0
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_1900
        mov    byte ptr [esi + 0xeb], 1
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_1743
        cmp    eax, 1
        je     loc_1_176B
        cmp    eax, 2
        je     loc_1_1791
        cmp    eax, 3
        je     loc_1_17B9
        cmp    eax, 4
        je     loc_1_1837
        jmp    loc_1_1897
loc_1_1743:
        push   0
        lea    eax, [edi + 0x24]
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_19B0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1897
loc_1_176B:
        lea    eax, [edi + 0x24]
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_1DD0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1897
loc_1_1791:
        push   1
        lea    eax, [edi + 0x24]
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_19B0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1897
loc_1_17B9:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax + 0x75]
        push   eax
        call   sub_2_0DD0
        or     al, al
        jne    loc_1_1808
        push   0
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0AEE
        push   8
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x14
        push   0
        push   0x45
        push   ebx
        call   sub_1_0BBC
        jmp    loc_1_182E
loc_1_1808:
        lea    eax, [esi + 0x4d]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0x51]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x51]
        push   0
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x51], ebx
        inc    dword ptr [esi + 0x55]
        inc    dword ptr [ebx + 0x14]
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0AEE
loc_1_182E:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1897
loc_1_1837:
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        call   sub_2_0EA0
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_1855
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_186E
loc_1_1855:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_186E
        cmp    dword ptr [ebp - 4], 0xc001000e
        jne    loc_1_186E
        mov    dword ptr [ebp - 4], 0x103
loc_1_186E:
        mov    byte ptr [ebx + 0x40], 1
        dec    dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0x59]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0x5d]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x5d]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x5d], ebx
loc_1_1897:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_1907
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x79], eax
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_18D0
        cmp    eax, 1
        je     loc_1_18D0
        cmp    eax, 2
        jne    loc_1_16FC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0B2A
        dec    dword ptr [esi + 0x49]
        jmp    loc_1_16FC
loc_1_18D0:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0B0C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_16FC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18F0:
        mov    byte ptr [esi + 0xeb], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1900:
        mov    byte ptr [esi + 0xeb], 0
loc_1_1907:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1910:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x49]
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_1952
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 2
        je     loc_1_1934
        mov    edi, 0xc00000bb
        jmp    loc_1_1957
loc_1_1934:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        inc    dword ptr [esi + 0x49]
        push   eax
        mov    edi, 0x103
        push   esi
        call   sub_1_16B0
        jmp    loc_1_1957
loc_1_1952:
        mov    edi, 0xc001000d
loc_1_1957:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_19A0
        cmp    dword ptr [esi + 0x49], 2
        jne    loc_1_196C
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_1988
loc_1_196C:
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_1988
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_1988
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_19A0
loc_1_1988:
        mov    byte ptr [esi + 0x11], 1
        push   0
        add    esi, 0x12
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_1_19A0:
        dec    dword ptr [esi + 0x49]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_19B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    cl, byte ptr [ebp + 0x24]
        or     cl, cl
        je     loc_1_19D0
        mov    dword ptr [ebp - 0x10], 0x27d8 ; -> loc_1_27D8
        mov    edi, 0x34
        jmp    loc_1_19DC
        db  08Bh, 0FFh
loc_1_19D0:
        mov    dword ptr [ebp - 0x10], 0x28a8 ; -> obj1:0x28A8
        mov    edi, 0x18
loc_1_19DC:
        sub    esi, esi
        cmp    edi, esi
        jle    loc_1_19F6
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 0x10]
loc_1_19E8:
        cmp    dword ptr [eax], edx
        je     loc_1_19F9
        add    eax, 4
        inc    esi
        cmp    edi, esi
        jg     loc_1_19E8
        jmp    loc_1_19F9
loc_1_19F6:
        mov    edx, dword ptr [ebp + 0x10]
loc_1_19F9:
        cmp    edi, esi
        jne    loc_1_1A14
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_1A14:
        lea    ebx, [ebp - 8]
        mov    esi, 4
        mov    eax, edx
        and    eax, 0xffff0000
        cmp    eax, 0x10000
        je     loc_1_1A50
        cmp    eax, 0x20000
        je     loc_1_1BA0
        cmp    eax, 0x1010000
        je     loc_1_1C26
        cmp    eax, 0x1020000
        je     loc_1_1D0B
        jmp    loc_1_1D4D
loc_1_1A50:
        sub    edx, 0x10101
        cmp    edx, 0x12
        ja     loc_1_1D4D
        jmp    dword ptr [edx*4 + 0x1d80]    ; -> obj1:0x1D80
loc_1_1A66:
        mov    ebx, dword ptr [ebp - 0x10]
        lea    esi, [edi*4]
        jmp    loc_1_1D4D
loc_1_1A75:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_1D4D
loc_1_1A81:
        mov    dword ptr [ebp - 8], 0
        or     cl, cl
        je     loc_1_1D4D
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x55], 0
        jne    loc_1_1D4D
        sub    esi, esi
        jmp    loc_1_1D4D
loc_1_1AA4:
        mov    dword ptr [ebp - 8], 0xd0
        jmp    loc_1_1D4D
loc_1_1AB0:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AC0:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AD0:
        mov    dword ptr [ebp - 8], 0x5ea0
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AE0:
        mov    dword ptr [ebp - 8], 0xbdc0
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AF0:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1B00:
        mov    dword ptr [ebp - 8], 0x5ee
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1B10:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 3
        push   edi
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_0C9C
        mov    byte ptr [ebp - 0x15], 0
        lea    ebx, [ebp - 0x18]
        mov    esi, 4
        jmp    loc_1_1D4D
loc_1_1B33:
        mov    ebx, 0x2908                   ; -> loc_1_2908
        mov    esi, 0x1d
        jmp    loc_1_1D4D
loc_1_1B42:
        or     cl, cl
        je     loc_1_1B57
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x81]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1D4D
loc_1_1B57:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1D4D
loc_1_1B68:
        mov    dword ptr [ebp - 8], 0xd0
        jmp    loc_1_1D4D
loc_1_1B74:
        mov    word ptr [ebp - 2], 0x300
        lea    ebx, [ebp - 2]
        mov    esi, 2
        jmp    loc_1_1D4D
loc_1_1B87:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_1D4D
loc_1_1B93:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_1D4D
        db  090h
loc_1_1BA0:
        or     cl, cl
        je     loc_1_1D4D
        mov    ecx, edx
        and    ecx, 0xff
        dec    ecx
        and    edx, 0xff00
        cmp    edx, 0x100
        je     loc_1_1BD0
        cmp    edx, 0x200
        je     loc_1_1BE2
        jmp    loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1BD0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + ecx*4 + 0xec]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1D4D
loc_1_1BE2:
        mov    eax, ecx
        and    eax, 0xfffffffe
        cmp    eax, 0xc
        jae    loc_1_1C14
        test   cl, 1
        mov    edi, dword ptr [ebp + 8]
        je     loc_1_1C03
        mov    eax, dword ptr [edi + eax*2 + 0x130]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1D4D
loc_1_1C03:
        lea    ebx, [edi + eax*4 + 0x100]
        mov    esi, 8
        jmp    loc_1_1D4D
loc_1_1C14:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + ecx*4 + 0x118]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1D4D
loc_1_1C26:
        cmp    edx, 0x1010101
        je     loc_1_1C50
        cmp    edx, 0x1010102
        je     loc_1_1C60
        cmp    edx, 0x1010103
        je     loc_1_1C70
        cmp    edx, 0x1010104
        je     loc_1_1D02
        jmp    loc_1_1D4D
        db  090h
loc_1_1C50:
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 3
        mov    esi, 6
        jmp    loc_1_1D4D
loc_1_1C60:
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 9
        mov    esi, 6
        jmp    loc_1_1D4D
loc_1_1C70:
        or     cl, cl
        je     loc_1_1CA5
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 0x75]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_13C0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp + 0x14]
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2]
        jmp    loc_1_1D4D
loc_1_1CA5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x75]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_1300
        cmp    dword ptr [ebp - 0x14], 0
        mov    ebx, dword ptr [ebp + 0x14]
        jne    loc_1_1CE3
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2]
        jmp    loc_1_1D4D
loc_1_1CE3:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x75]
        push   eax
        call   sub_2_12C0
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2]
        jmp    loc_1_1D4D
loc_1_1D02:
        mov    dword ptr [ebp - 8], 0x10
        jmp    loc_1_1D4D
loc_1_1D0B:
        or     cl, cl
        je     loc_1_1D4D
        mov    eax, edx
        and    eax, 0xff
        dec    eax
        and    edx, 0xff00
        cmp    edx, 0x100
        je     loc_1_1D30
        cmp    edx, 0x200
        je     loc_1_1D40
        jmp    loc_1_1D4D
        db  090h
loc_1_1D30:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + eax*4 + 0x150]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1D4D
        db  090h
loc_1_1D40:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + eax*4 + 0x15c]
        mov    dword ptr [ebp - 8], eax
loc_1_1D4D:
        cmp    dword ptr [ebp + 0x18], esi
        jae    loc_1_1D65
        mov    eax, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        mov    eax, 0xc0010016
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_1D65:
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_0C9C
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_1A66
        dd  loc_1_1A75
        dd  loc_1_1A81
        dd  loc_1_1A81
        dd  loc_1_1AA4
        dd  loc_1_1AB0
        dd  loc_1_1AC0
        dd  loc_1_1AD0
        dd  loc_1_1AE0
        dd  loc_1_1AF0
        dd  loc_1_1B00
        dd  loc_1_1B10
        dd  loc_1_1B33
        dd  loc_1_1B42
        dd  loc_1_1B68
        dd  loc_1_1B74
        dd  loc_1_1B87
        dd  loc_1_1D4D
        dd  loc_1_1B93
        db  08Dh, 064h, 024h, 000h
sub_1_1DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010e
        je     loc_1_1E10
        cmp    eax, 0x1010f
        je     loc_1_1E81
        cmp    eax, 0x10112
        je     loc_1_1E93
        cmp    eax, 0x1010103
        je     loc_1_1ED4
        mov    eax, 0xc0010017
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_1E10:
        mov    esi, dword ptr [ebp + 0x18]
        cmp    esi, 4
        je     loc_1_1E24
        mov    eax, 0xc0010015
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1E24:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0C9C
        test   dword ptr [ebp - 4], 0xf050
        je     loc_1_1E5A
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], 4
        mov    dword ptr [eax], 0
        mov    eax, 0xc00000bb
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1E5A:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x75]
        push   eax
        call   sub_2_1240
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1E81:
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1E93:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_1EB0
        mov    eax, dword ptr [ebp + 0x20]
        pop    esi
        mov    dword ptr [eax], 4
        mov    eax, 0xc0010014
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_1EB0:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x41
        push   eax
        call   sub_1_0C9C
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1ED4:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, esi
        cdq    
        idiv   ecx
        or     edx, edx
        je     loc_1_1EF1
        mov    eax, 0xc0010015
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1EF1:
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq    
        idiv   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x75]
        push   eax
        call   sub_2_0F60
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1F30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [ecx + 0xe8], 0
        je     loc_1_1F4B
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_1F4B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_1F60
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_1_1F60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 8]
        test   edi, 0x20
        mov    eax, dword ptr [esi + 0xc8]
        je     loc_1_1F84
        mov    word ptr [eax], 0x8000
        jmp    loc_1_1FED
loc_1_1F84:
        mov    word ptr [eax], 0
        test   edi, 4
        je     loc_1_1FC0
        sub    ax, ax
        mov    ecx, 0xff
        lea    esp, [esp]
loc_1_1FA0:
        mov    edx, eax
        and    edx, 0xffff
        mov    ebx, dword ptr [esi + 0xc8]
        mov    byte ptr [edx + ebx + 8], cl
        inc    ax
        cmp    ax, 8
        jb     loc_1_1FA0
        jmp    loc_1_1FED
        db  08Dh, 064h, 024h, 000h
loc_1_1FC0:
        test   edi, 2
        je     loc_1_1FED
        lea    eax, [ebp - 0x68]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x75]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_13C0
        lea    eax, [ebp - 0x68]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_2060
loc_1_1FED:
        push   esi
        call   0
        mov    dword ptr [esi + 0x81], edi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2010:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 0xe8], 0
        je     loc_1_2030
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x1c
        db  08Dh, 049h, 000h
loc_1_2030:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x10
        jbe    loc_1_2042
        mov    eax, 0xc0010009
        pop    esi
        pop    ebp
        ret    0x1c
loc_1_2042:
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        push   eax
        push   esi
        call   sub_1_2060
        push   esi
        call   0
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        sub    edx, edx
        lea    ecx, [ecx]
loc_1_2070:
        mov    ecx, dword ptr [ebp + 8]
        inc    eax
        mov    ecx, dword ptr [ecx + 0xc8]
        mov    byte ptr [eax + ecx + 7], dl
        cmp    eax, 8
        jb     loc_1_2070
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        je     loc_1_20D3
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
loc_1_2090:
        sub    bl, bl
        push   esi
        push   6
        call   sub_1_20E0
        sub    ecx, ecx
loc_1_209C:
        mov    dl, al
        shr    dl, cl
        and    dl, 1
        inc    ecx
        lea    ebx, [edx + ebx*2]
        cmp    ecx, 6
        jb     loc_1_209C
        mov    al, bl
        shr    al, 3
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, al
        mov    edi, dword ptr [ecx + 0xc8]
        mov    al, 1
        mov    cl, bl
        and    cl, 7
        shl    al, cl
        or     byte ptr [edx + edi + 8], al
        add    esi, 6
        dec    dword ptr [ebp - 4]
        jne    loc_1_2090
loc_1_20D3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_20E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_2122
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_20F4:
        mov    dl, byte ptr [ecx]
        inc    ecx
        mov    esi, 8
loc_1_2100:
        mov    edi, eax
        shr    edi, 0x1f
        add    eax, eax
        mov    bl, dl
        and    ebx, 1
        cmp    ebx, edi
        je     loc_1_2118
        xor    eax, 0x4c11db6
        or     eax, 1
loc_1_2118:
        shr    dl, 1
        dec    esi
        jne    loc_1_2100
        dec    dword ptr [ebp + 8]
        jne    loc_1_20F4
loc_1_2122:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2130:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x14]
        ret    4
        db  08Dh, 049h, 000h
loc_1_2140:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0x103
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x49]
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_2220
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_2210
        inc    dword ptr [edi + 0x14]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2200
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_2200
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebx + 0x2c], ax
        mov    dword ptr [ebx + 0x28], edi
        mov    eax, dword ptr [ebp + 0xc]
        add    ebx, 0x24
        push   eax
        push   edi
        push   edi
        call   sub_1_2290
        cmp    byte ptr [ebx + 0xa], 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        jne    loc_1_21F0
        call   sub_1_0E50
        inc    dword ptr [esi + 0xec]
        lea    edx, [esi + 0x100]
        xor    ecx, ecx
        inc    dword ptr [esi + 0x130]
        mov    cx, word ptr [ebx + 8]
        add    ecx, dword ptr [edx]
        mov    dword ptr [edx], ecx
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        cmp    eax, ecx
        jbe    loc_1_220A
        inc    dword ptr [edx + 4]
        dec    dword ptr [edi + 0x14]
        jmp    loc_1_2227
        db  08Bh, 0FFh
loc_1_21F0:
        call   sub_1_2700
        dec    dword ptr [edi + 0x14]
        jmp    loc_1_2227
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2200:
        mov    dword ptr [ebp - 4], 0xc001000f
        dec    dword ptr [edi + 0x14]
loc_1_220A:
        dec    dword ptr [edi + 0x14]
        jmp    loc_1_2227
        db  090h
loc_1_2210:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_2227
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2220:
        mov    dword ptr [ebp - 4], 0xc001000d
loc_1_2227:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_1_2273
        cmp    dword ptr [esi + 0x49], 2
        jne    loc_1_223C
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_1_2258
loc_1_223C:
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_2258
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_1_2258
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_1_2273
loc_1_2258:
        mov    byte ptr [esi + 0x11], 1
        push   0
        add    esi, 0x12
        push   esi
        call   sub_1_0B92
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2273:
        dec    dword ptr [esi + 0x49]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2290:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [edi + 0x24]
        mov    ebx, dword ptr [eax + 8]
        mov    byte ptr [esi + 0xa], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   6
        push   0
        push   edi
        call   sub_1_1370
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 0x41], 2
        jne    loc_1_22D9
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 0x75]
        push   eax
        call   sub_1_09E0
        or     al, al
        je     loc_1_22D9
        or     byte ptr [esi + 0xa], 8
loc_1_22D9:
        test   byte ptr [ebp - 0x10], 1
        je     loc_1_2310
        cmp    dword ptr [ebp - 0xe], -1
        jne    loc_1_2300
        cmp    byte ptr [ebp - 0x10], 0xff
        jne    loc_1_2300
        cmp    byte ptr [ebp - 0xf], 0xff
        jne    loc_1_2300
        or     byte ptr [esi + 0xa], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_2300:
        or     byte ptr [esi + 0xa], 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_2310:
        or     byte ptr [esi + 0xa], 1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        push   6
        push   6
        push   edi
        call   sub_1_1370
        mov    eax, dword ptr [ebp - 0x16]
        cmp    dword ptr [ebp - 0xe], eax
        jae    loc_1_2337
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_2378
loc_1_2337:
        mov    eax, dword ptr [ebp - 0x16]
        cmp    dword ptr [ebp - 0xe], eax
        jbe    loc_1_2348
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_1_2378
loc_1_2348:
        mov    ax, word ptr [ebp - 0x18]
        cmp    word ptr [ebp - 0x10], ax
        jae    loc_1_2360
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_2378
        db  005h, 000h, 000h, 000h, 000h
loc_1_2360:
        mov    ax, word ptr [ebp - 0x18]
        cmp    word ptr [ebp - 0x10], ax
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_1_2378
        mov    dword ptr [ebp - 4], 0
loc_1_2378:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2382
        mov    byte ptr [esi + 0xa], 8
loc_1_2382:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_1_2390:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x24
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0xa1]
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 0x99]
        mov    dword ptr [ebp - 0x14], ecx
        lea    ecx, [eax + ecx*8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [esi + 0x91]
        lea    edi, [ecx + ecx*4]
        lea    edi, [ecx + edi*4]
        lea    edi, [edi + edi*8]
        lea    edi, [ecx + edi*4]
        mov    ecx, dword ptr [ebp - 0x18]
        lea    edi, [eax + edi*2]
        mov    ax, word ptr [ecx + 2]
        and    ax, 0xff00
        mov    cx, word ptr [ecx + 6]
        mov    word ptr [ebp - 2], cx
        mov    cl, byte ptr [esi + 0xa1]
        inc    cl
        mov    byte ptr [esi + 0xa1], cl
        and    ecx, 0xff
        cmp    dword ptr [esi + 0x8d], ecx
        ja     loc_1_2408
        mov    byte ptr [esi + 0xa1], bl
loc_1_2408:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    byte ptr [esi + ecx + 0xa2], 0
        mov    byte ptr [esi + 0x10], 1
        test   ah, 0x44
        je     loc_1_2490
        inc    dword ptr [esi + 0xf4]
        test   ah, 4
        je     loc_1_242D
        inc    dword ptr [esi + 0x15c]
loc_1_242D:
        test   ah, 0x40
        je     loc_1_255F
        mov    ax, word ptr [ebp - 2]
        and    ax, 0x4000
        je     loc_1_2446
        inc    dword ptr [esi + 0x168]
loc_1_2446:
        test   byte ptr [ebp - 1], 8
        je     loc_1_2452
        inc    dword ptr [esi + 0x170]
loc_1_2452:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_245E
        inc    dword ptr [esi + 0x174]
loc_1_245E:
        test   byte ptr [ebp - 1], 4
        je     loc_1_246A
        inc    dword ptr [esi + 0x160]
loc_1_246A:
        or     ax, ax
        jne    loc_1_2479
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_255F
loc_1_2479:
        push   esi
        call   0
        mov    byte ptr [esi + 0x10], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_2490:
        inc    dword ptr [esi + 0xec]
        test   ah, 8
        je     loc_1_24A1
        inc    dword ptr [esi + 0x154]
loc_1_24A1:
        test   ah, 0x10
        je     loc_1_24AC
        inc    dword ptr [esi + 0x158]
loc_1_24AC:
        mov    al, byte ptr [edx + 0xa]
        and    eax, 0xf7
        cmp    eax, 1
        je     loc_1_24C8
        cmp    eax, 2
        je     loc_1_2500
        cmp    eax, 4
        je     loc_1_2530
        jmp    loc_1_255F
loc_1_24C8:
        xor    eax, eax
        inc    dword ptr [esi + 0x130]
        mov    ax, word ptr [edx + 8]
        add    eax, dword ptr [esi + 0x100]
        mov    dword ptr [esi + 0x100], eax
        lea    ecx, [esi + 0x100]
        mov    dx, word ptr [edx + 8]
        and    edx, 0xffff
        cmp    edx, eax
        jbe    loc_1_255F
        inc    dword ptr [ecx + 4]
        jmp    loc_1_255F
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2500:
        xor    ecx, ecx
        inc    dword ptr [esi + 0x134]
        mov    cx, word ptr [edx + 8]
        add    ecx, dword ptr [esi + 0x108]
        mov    dword ptr [esi + 0x108], ecx
        lea    eax, [esi + 0x108]
        mov    dx, word ptr [edx + 8]
        and    edx, 0xffff
        cmp    edx, ecx
        jbe    loc_1_255F
        jmp    loc_1_255C
        db  08Bh, 0FFh
loc_1_2530:
        xor    ecx, ecx
        inc    dword ptr [esi + 0x138]
        mov    cx, word ptr [edx + 8]
        add    ecx, dword ptr [esi + 0x110]
        mov    dword ptr [esi + 0x110], ecx
        lea    eax, [esi + 0x110]
        mov    dx, word ptr [edx + 8]
        and    edx, 0xffff
        cmp    edx, ecx
        jbe    loc_1_255F
loc_1_255C:
        inc    dword ptr [eax + 4]
loc_1_255F:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_25B0
loc_1_2578:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   edi
        call   sub_1_0C9C
        add    edi, dword ptr [ebp - 0xc]
        add    ebx, dword ptr [ebp - 0xc]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C28
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2578
loc_1_25B0:
        cmp    ebx, 0x3c
        jge    loc_1_25D7
        mov    edx, 0x3c
        sub    edx, ebx
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    ecx, dword ptr [ebp - 0x18]
        mov    word ptr [ecx + 4], 0xffc4
        jmp    loc_1_25E1
loc_1_25D7:
        mov    ecx, dword ptr [ebp - 0x18]
        neg    bx
        mov    word ptr [ecx + 4], bx
loc_1_25E1:
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    byte ptr [esi + ecx + 0xa2], 0
        je     loc_1_2600
        mov    byte ptr [esi + 0x10], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_2600:
        mov    ecx, dword ptr [ebp - 0x18]
        and    word ptr [ecx + 2], 0xa3ff
        mov    word ptr [ecx + 6], 0
        or     word ptr [ecx + 2], 0x8000
        sub    ax, ax
        mov    ecx, dword ptr [ebp - 0x14]
        mov    byte ptr [esi + ecx + 0xa2], 1
        mov    cx, word ptr [esi + 0x45]
        mov    byte ptr [esi + 0x10], 0
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        mov    ax, 0x48
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2650:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_2670
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2670:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x99]
        mov    al, byte ptr [esi + 0xa1]
        test   byte ptr [ecx + eax*8 + 3], 0x80
        jne    loc_1_26EB
        cmp    byte ptr [eax + esi + 0xa2], 0
        je     loc_1_26EB
        mov    edi, dword ptr [esi + 0xdc]
        lea    ebx, [edi + 0x24]
        cmp    dword ptr [ebx], 0
        jne    loc_1_26A7
        mov    dword ptr [esi + 0xe0], 0
loc_1_26A7:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0xdc], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 4], eax
        push   edi
        push   esi
        call   sub_1_2390
        test   byte ptr [ebx + 0xa], 8
        je     loc_1_26CB
        push   edi
        push   esi
        call   sub_1_0E50
        jmp    loc_1_26DA
loc_1_26CB:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   edi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0AFA
loc_1_26DA:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [eax + 0x14]
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_26EB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2700:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 0x28]
        lea    esi, [edx + 0x24]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xdc], 0
        jne    loc_1_2724
        mov    dword ptr [eax + 0xdc], edx
        jmp    loc_1_272D
loc_1_2724:
        mov    ecx, dword ptr [eax + 0xe0]
        mov    dword ptr [ecx + 0x24], edx
loc_1_272D:
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 0xe0], edx
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_2740:
        inc    ecx
        insd   dword ptr es:[edi], dx
        xor    dword ptr [0x543030], esi
loc_1_2748:
        or     byte ptr [eax], al
        or     byte ptr [eax], al
loc_1_274C:
        inc    eax
        daa    
        add    byte ptr [eax], al
        pop    esp
        inc    esp
        jbe    loc_1_27BE
        arpl   word ptr [ebp + 0x5c], sp
        inc    ecx
        insd   dword ptr es:[edi], dx
        xor    dword ptr [0x30543030], esi
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2764:
        pop    esp
        inc    esp
        jbe    0x27d2
        arpl   word ptr [ebp + 0x5c], sp
        inc    ecx
        insd   dword ptr es:[edi], dx
        xor    dword ptr [0x30543030], esi
        xor    dword ptr [eax], eax
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
        dec    ecx
        dec    edi
        inc    ecx
        jb     0x27fc
        jae    0x280c
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        jb     0x2808
        jae    0x2818
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     0x2811
        jb     0x2820
        jne    0x2820
        je     loc_1_27B2
loc_1_27B2:
        add    byte ptr [eax], al
loc_1_27B4:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     loc_1_281D
        jb     0x282c
        jne    0x282c
        je     loc_1_27BE
loc_1_27BE:
        add    byte ptr [eax], al
        inc    esp
        insd   dword ptr es:[edi], dx
        popal  
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_27CC:
        inc    esp
        insd   dword ptr es:[edi], dx
        popal  
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_27D8:
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
loc_1_281D:
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
loc_1_2908:
        inc    ecx
        dec    ebp
        inc    esp
        and    byte ptr [ecx + 0x6d], al
        xor    dh, byte ptr [ecx]
        xor    byte ptr [eax], dh
        das    
        push   eax
        inc    ebx
        dec    esi
        inc    ebp
        push   esp
        sub    eax, 0x20415349
        inc    ecx
        popal  
        jo     0x2996
        jb     loc_1_2925
loc_1_2925:
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x14e0  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x292c                        ; -> obj1:0x292C
        call   sub_1_0ACA
        push   -1
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0BB6
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_0040
        mov    eax, 0xc000009a
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_2_0040:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [0x292c]       ; -> obj1:0x292C
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x40], 3
        mov    byte ptr [ebp - 0x3f], 0
        mov    dword ptr [ebp - 0x3e], 0
        mov    dword ptr [ebp - 0x3a], 0x900 ; -> loc_2_0900
        mov    dword ptr [ebp - 0x36], 0x70  ; -> loc_1_0070
        mov    dword ptr [ebp - 0x32], 0x2140 ; -> loc_1_2140
        mov    dword ptr [ebp - 0x2e], 0x1530 ; -> loc_1_1530
        mov    dword ptr [ebp - 0x2a], 0x1b0 ; -> loc_1_01B0
        mov    dword ptr [ebp - 0x26], 0x1600 ; -> loc_1_1600
        mov    dword ptr [ebp - 0x22], 0x1910 ; -> loc_1_1910
        mov    dword ptr [ebp - 0x1e], 0x110 ; -> loc_1_0110
        mov    dword ptr [ebp - 0x1a], 0x100 ; -> loc_2_0100
        mov    dword ptr [ebp - 0x16], 0x140 ; -> loc_1_0140
        mov    eax, dword ptr [0x2748]       ; -> loc_1_2748
        mov    ecx, dword ptr [0x274c]       ; -> loc_1_274C
        mov    dword ptr [ebp - 0x12], eax
        lea    eax, [ebp - 0x40]
        mov    dword ptr [ebp - 0xe], ecx
        push   0x36
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [0x292c]       ; -> obj1:0x292C
        push   eax
        lea    eax, [ebp - 4]
        push   0x2930                        ; -> obj1:0x2930
        push   eax
        call   sub_1_0AD6
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [0x2930]       ; -> obj1:0x2930
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_00E7
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_00E7:
        push   0
        mov    eax, dword ptr [0x292c]       ; -> obj1:0x292C
        push   eax
        call   sub_1_0AD0
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_0100:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 0x38], 0x12
        mov    word ptr [ebp - 0x36], 0x12
        mov    dword ptr [ebp - 0x34], 0x2764 ; -> loc_1_2764
        mov    word ptr [ebp - 0x20], 0xa
        mov    word ptr [ebp - 0x1e], 0xa
        mov    dword ptr [ebp - 0x1c], 0x279c ; -> obj1:0x279C
        mov    word ptr [ebp - 0x30], 0xa
        mov    word ptr [ebp - 0x2e], 0xa
        mov    dword ptr [ebp - 0x2c], 0x27b4 ; -> loc_1_27B4
        mov    word ptr [ebp - 0x28], 0xb
        mov    word ptr [ebp - 0x26], 0xb
        mov    dword ptr [ebp - 0x24], 0x27cc ; -> loc_1_27CC
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        jne    loc_2_0190
        push   0x20
        push   5
        push   3
        push   0x300
        lea    eax, [ebp - 0x38]
        push   0
        push   eax
        push   esi
        mov    eax, dword ptr [0x2930]       ; -> obj1:0x2930
        push   eax
        call   sub_2_02A0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_2_0190:
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0C34
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_01B0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_01B0:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0C3A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_01D8
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 1], al
        jmp    loc_2_01DC
loc_2_01D8:
        mov    byte ptr [ebp - 1], 3
loc_2_01DC:
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0C3A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0201
        mov    eax, dword ptr [ebp - 0x14]
        mov    bl, byte ptr [eax + 4]
        jmp    loc_2_0203
loc_2_0201:
        mov    bl, 5
loc_2_0203:
        push   1
        lea    eax, [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0C3A
        cmp    dword ptr [ebp - 8], 0
        mov    edi, 0x300
        jne    loc_2_022B
        mov    eax, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [eax + 4]
loc_2_022B:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0B3C
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0265
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_0265
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 2]
        or     al, byte ptr [ecx + 3]
        or     al, byte ptr [ecx + 4]
        or     al, byte ptr [ecx + 5]
        or     al, byte ptr [ecx + 1]
        or     al, byte ptr [ecx]
        jne    loc_2_026C
loc_2_0265:
        mov    dword ptr [ebp - 0xc], 0
loc_2_026C:
        push   0x20
        push   ebx
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [0x2930]       ; -> obj1:0x2930
        push   eax
        call   sub_2_02A0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0C40
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_2_02A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   esi
        push   edi
        push   -1
        push   0
        lea    eax, [ebp - 8]
        push   0x248
        push   eax
        call   sub_1_0BB6
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        jne    loc_2_08C5
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0x92
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x6d], eax
        lea    edi, [ebp - 0x34]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x20]
        mov    byte ptr [ebp - 0x30], 1
        mov    dword ptr [ebp - 0x34], eax
        mov    dword ptr [ebp - 0x26], 1
        lea    eax, [ebp - 0x34]
        mov    dword ptr [ebp - 0x22], 0x600
        mov    dword ptr [ebp - 0x1e], 1
        mov    esi, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp - 0x1a], esi
        mov    dword ptr [ebp - 0x16], 0x20
        mov    dword ptr [ebp - 0x2a], 1
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [eax + 0x6d]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x71
        push   eax
        call   sub_1_0AE2
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        jne    loc_2_0890
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x8d], 0x10
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xb2], 0x20
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x45], esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0B50
        or     al, al
        je     loc_2_0842
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0A60
        or     al, al
        jne    loc_2_03BB
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0650
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0AE8
        mov    dword ptr [ebp - 0xc], eax
        push   0
        push   0x248
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0BBC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_03BB:
        mov    eax, dword ptr [ebp + 0x14]
        or     eax, eax
        jne    loc_2_03C8
        mov    eax, dword ptr [ebp - 8]
        add    eax, 3
loc_2_03C8:
        push   6
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9
        push   eax
        call   sub_1_0C9C
        mov    eax, dword ptr [ebp - 8]
        sub    cl, cl
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0
loc_2_03E7:
        mov    esi, ecx
        and    esi, 0xff
        mov    eax, dword ptr [ebp - 8]
        mov    dl, byte ptr [esi + eax + 9]
        mov    eax, dword ptr [eax + 0xc8]
        mov    byte ptr [esi + eax + 2], dl
        inc    cl
        cmp    cl, 6
        jb     loc_2_03E7
        sub    cl, cl
        sub    edx, edx
loc_2_040B:
        mov    esi, ecx
        mov    eax, dword ptr [ebp - 8]
        and    esi, 0xff
        mov    eax, dword ptr [eax + 0xc8]
        mov    byte ptr [esi + eax + 8], dl
        inc    cl
        cmp    cl, 8
        jb     loc_2_040B
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0xc2]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 0x10], cx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0xb2]
        shr    ecx, 1
        or     ecx, ecx
        je     loc_2_0463
        mov    edx, 0x2000
        mov    edi, edi
loc_2_0450:
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 1
        mov    eax, dword ptr [eax + 0xc8]
        add    word ptr [eax + 0x12], dx
        or     ecx, ecx
        jne    loc_2_0450
loc_2_0463:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0xc8]
        mov    eax, dword ptr [eax + 0xc2]
        shr    eax, 0x10
        and    ax, 0xff
        or     word ptr [ecx + 0x12], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x9d]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 0x14], cx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x8d]
        shr    ecx, 1
        or     ecx, ecx
        je     loc_2_04B7
        mov    edx, 0x2000
loc_2_04A4:
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 1
        mov    eax, dword ptr [eax + 0xc8]
        add    word ptr [eax + 0x16], dx
        or     ecx, ecx
        jne    loc_2_04A4
loc_2_04B7:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0xc8]
        mov    eax, dword ptr [eax + 0x9d]
        shr    eax, 0x10
        and    ax, 0xff
        or     word ptr [ecx + 0x16], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0x4d
        mov    dword ptr [ecx + 0x51], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x51]
        mov    dword ptr [ecx + 0x4d], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0x59
        mov    dword ptr [ecx + 0x5d], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ecx + 0x59], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x79], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x7d], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x81], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x55], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xf], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x11], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xd0], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xd4], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xd8], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xdc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xe0], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x49], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xe8], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xe9], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xea], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xe4], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xeb], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xec
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    ecx, 0xc
        mov    dword ptr [eax + 0x10], 0
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x100
        sub    eax, eax
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x130
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x148
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x150
        mov    ecx, 7
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x15c
        sub    eax, eax
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x75
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x61
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9
        push   eax
        push   0x2130                        ; -> loc_1_2130
        push   0x1f30                        ; -> loc_1_1F30
        push   0x2010                        ; -> loc_1_2010
        push   0x10
        call   sub_2_0C60
        or     al, al
        jne    loc_2_06D2
        push   2
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0650
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0AE8
        push   0
        push   0x248
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0BBC
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_06D2:
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x2b
        sub    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        movsx  eax, byte ptr [ebp + 0x1c]
        push   1
        push   1
        push   eax
        push   eax
        push   0xf00                         ; -> loc_1_0F00
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xe90                         ; -> loc_1_0E90
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x2b
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0B9E
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0770
        push   1
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000005
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x75]
        push   eax
        call   sub_2_0D80
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0650
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0AE8
        push   0
        push   0x248
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0BBC
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0770:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x12], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x16], 0x1010 ; -> loc_1_1010
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [ecx + 0x1a], eax
        mov    eax, dword ptr [ebp - 8]
        push   0x1388
        mov    dword ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x22], 0x1340 ; -> loc_1_1340
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [ecx + 0x26], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x1e
        push   eax
        call   sub_1_0B92
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x2a], 0
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x45]
        add    cx, 0x14
        mov    dx, cx
        in     ax, dx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0000
        or     al, al
        jne    loc_2_0826
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x2b
        push   eax
        call   sub_1_0BA4
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x75]
        push   eax
        call   sub_2_0D80
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0650
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0AE8
        push   0
        push   0x248
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0BBC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_0826:
        push   0x630                         ; -> loc_1_0630
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0C8E
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_0842:
        push   3
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0650
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0AE8
        push   0
        push   0x248
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0BBC
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
loc_2_0890:
        push   4
        push   0
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x14
        push   0
        push   0x248
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0BBC
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_08C5:
        push   0x248
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   6
        push   0
        push   4
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x1c
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0900:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 2], 0
        je     loc_2_0920
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_2_0920:
        sub    edx, edx
        mov    eax, dword ptr [ebp + 0x18]
        cmp    eax, edx
        je     loc_2_0941
        mov    ecx, dword ptr [ebp + 0x14]
        lea    esp, [esp]
loc_2_0930:
        cmp    dword ptr [ecx], 0
        je     loc_2_093D
        add    ecx, 4
        inc    edx
        cmp    eax, edx
        ja     loc_2_0930
loc_2_093D:
        cmp    eax, edx
        jne    loc_2_0950
loc_2_0941:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_2_0950:
        mov    eax, dword ptr [ebp + 0x10]
        push   -1
        mov    dword ptr [eax], edx
        inc    dword ptr [esi + 0x49]
        lea    eax, [ebp - 4]
        push   0
        push   0x45
        push   eax
        call   sub_1_0BB6
        or     eax, eax
        jne    loc_2_09E0
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
        mov    byte ptr [eax + 0x40], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x41], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x28], 3
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x18
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x18
        push   eax
        push   esi
        call   sub_1_16B0
        mov    edi, 0x103
        jmp    loc_2_09FC
loc_2_09E0:
        push   9
        push   1
        push   2
        push   0xaa000002
        mov    edi, 0xc000009a
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x14
loc_2_09FC:
        cmp    byte ptr [esi + 0x11], 0
        jne    loc_2_0A46
        cmp    dword ptr [esi + 0x49], 2
        jne    loc_2_0A11
        cmp    byte ptr [esi + 0xe8], 0
        jne    loc_2_0A2D
loc_2_0A11:
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_2_0A2D
        cmp    dword ptr [esi + 0xd0], 0
        jne    loc_2_0A2D
        mov    eax, dword ptr [esi + 0x59]
        sub    eax, esi
        cmp    eax, 0x59
        je     loc_2_0A46
loc_2_0A2D:
        mov    byte ptr [esi + 0x11], 1
        push   0
        add    esi, 0x12
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0A46:
        dec    dword ptr [esi + 0x49]
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        sub    cx, cx
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_2_0A70:
        mov    edx, dword ptr [esi + 0x45]
        add    dx, di
        in     al, dx
        and    ax, 0xff
        add    cx, ax
        inc    edi
        cmp    edi, 0xc
        jb     loc_2_0A70
        mov    edi, 0xe
loc_2_0A89:
        mov    edx, dword ptr [esi + 0x45]
        add    dx, di
        in     al, dx
        and    ax, 0xff
        add    cx, ax
        inc    edi
        cmp    edi, 0x10
        jb     loc_2_0A89
        mov    edx, dword ptr [esi + 0x45]
        add    dx, 0xc
        in     ax, dx
        cmp    ax, cx
        je     loc_2_0AD1
        push   0xc0000001
        push   0xb
        push   2
        push   3
        push   0xaa000004
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0C88
        add    esp, 0x18
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0AD1:
        sub    ecx, ecx
loc_2_0AD3:
        mov    edx, dword ptr [esi + 0x45]
        add    dx, cx
        in     al, dx
        mov    byte ptr [esi + ecx + 3], al
        inc    ecx
        cmp    ecx, 6
        jb     loc_2_0AD3
        push   esi
        call   sub_1_0050
        mov    cx, word ptr [esi + 0x45]
        mov    ax, 0x59
        lea    edx, [ecx + 0x12]
        out    dx, ax
        lea    edx, [ecx + 0x10]
        in     ax, dx
        mov    di, word ptr [esi + 0x45]
        mov    ecx, eax
        and    ecx, 0xffff
        shl    ecx, 0x10
        lea    edx, [edi + 0x12]
        mov    ax, 0x58
        out    dx, ax
        lea    edx, [edi + 0x10]
        in     ax, dx
        and    eax, 0xffff
        add    ecx, eax
        mov    eax, ecx
        and    eax, 0xfffffff
        cmp    eax, 0x3003
        jne    loc_2_0B41
        mov    byte ptr [esi], 2
        shr    ecx, 0x1c
        mov    byte ptr [esi + 1], cl
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0B41:
        mov    byte ptr [esi], 1
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_0B50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0xb2]
        mov    eax, edx
        shl    edx, 7
        sub    edx, eax
        lea    edx, [eax + edx*4]
        lea    edx, [edx + edx*2]
        sub    edx, eax
        mov    eax, dword ptr [esi + 0x8d]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        lea    ebx, [edx + eax + 0x1f]
        lea    eax, [esi + 0x85]
        push   eax
        lea    eax, [esi + 0x89]
        push   eax
        push   0
        push   ebx
        mov    eax, dword ptr [esi + 0x71]
        push   eax
        call   sub_1_0B42
        mov    edi, dword ptr [esi + 0x89]
        or     edi, edi
        jne    loc_2_0BB5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_0BB5:
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edx, dword ptr [esi + 0x89]
        add    edx, 7
        and    edx, 0xfffffff8
        mov    ecx, dword ptr [esi + 0x85]
        add    ecx, 7
        and    ecx, 0xfffffff8
        mov    dword ptr [esi + 0xc8], edx
        mov    dword ptr [esi + 0xcc], ecx
        add    edx, 0x18
        add    ecx, 0x18
        mov    dword ptr [esi + 0x99], edx
        mov    dword ptr [esi + 0x9d], ecx
        mov    eax, dword ptr [esi + 0x8d]
        shl    eax, 3
        add    edx, eax
        add    ecx, eax
        mov    dword ptr [esi + 0xbe], edx
        mov    dword ptr [esi + 0xc2], ecx
        mov    eax, dword ptr [esi + 0xb2]
        shl    eax, 3
        add    edx, eax
        add    ecx, eax
        mov    dword ptr [esi + 0x91], edx
        mov    eax, dword ptr [esi + 0x8d]
        mov    edi, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edi + eax*4]
        mov    dword ptr [esi + 0x95], ecx
        lea    eax, [eax + eax*8]
        lea    eax, [edi + eax*4]
        add    eax, eax
        add    edx, eax
        add    ecx, eax
        mov    dword ptr [esi + 0xb6], edx
        mov    dword ptr [esi + 0xba], ecx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_0C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x2928]       ; -> obj1:0x2928
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_0BB6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0C94
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0C94:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_0CAE
        mov    esi, 2
loc_2_0CAE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0BB6
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_0CE2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0D80
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0CE2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_0BB6
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_0D11
        push   eax
        call   sub_2_0D80
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0D11:
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
sub_2_0D80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0DA0
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_0BBC
loc_2_0DA0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0DB6
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_0BBC
loc_2_0DB6:
        push   0
        push   0x32
        push   esi
        call   sub_1_0BBC
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_0DE9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0DE9:
        mov    eax, dword ptr [0x2928]       ; -> obj1:0x2928
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0BB6
        or     eax, eax
        je     loc_2_0E09
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0E09:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_0E10:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_0E23
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_0E10
        jmp    loc_2_0E32
loc_2_0E23:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_0E32:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_0E48
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0E48:
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
sub_2_0EA0:
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
        call   sub_2_1240
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0EC9
        cmp    ebx, 0x103
        jne    loc_2_0EE0
loc_2_0EC9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0F60
        or     eax, eax
        je     loc_2_0EE0
        mov    ebx, eax
loc_2_0EE0:
        or     ebx, ebx
        je     loc_2_0EEC
        cmp    ebx, 0x103
        jne    loc_2_0F4B
loc_2_0EEC:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_0F46
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_0F02
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0F02:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_0F11
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_0F17
loc_2_0F11:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_0F17:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_0F26
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
loc_2_0F26:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_0BBC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_0F46:
        mov    ebx, 0xc001000e
loc_2_0F4B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0F60:
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
        call   sub_1_0CBC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_0CBC
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_0FE4
        sub    eax, eax
        nop    
loc_2_0FC0:
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
        ja     loc_2_0FC0
loc_2_0FE4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_104F
loc_2_0FF1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_1044
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
        call   sub_1_0CBC
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
        call   sub_1_0CBC
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_1047
loc_2_1044:
        inc    dword ptr [ebp - 4]
loc_2_1047:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_0FF1
loc_2_104F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_113A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_1063:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_1420
        or     al, al
        je     loc_2_109A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_1128
loc_2_109A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_1174
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
        call   sub_1_0CBC
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
        call   sub_1_0CBC
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
loc_2_1128:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_1063
loc_2_113A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_11C5
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_1150:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_11CB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_11B2
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_11B2
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_11B9
loc_2_1174:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0CBC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0CBC
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_11B2:
        mov    dword ptr [ebp - 8], 1
loc_2_11B9:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_1150
loc_2_11C5:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_1230
loc_2_11CB:
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
        je     loc_2_1232
        cmp    edi, 0x103
        je     loc_2_1232
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0CBC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0CBC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_1230:
        sub    edi, edi
loc_2_1232:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_1240:
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
        je     loc_2_127D
        lea    esp, [esp]
loc_2_1270:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1270
loc_2_127D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_12B1
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
        je     loc_2_12B3
        cmp    eax, 0x103
        je     loc_2_12B3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_12B1:
        sub    eax, eax
loc_2_12B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_12C0:
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
        je     loc_2_12F2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_12E7:
        test   dword ptr [esi], edx
        je     loc_2_12EC
        inc    eax
loc_2_12EC:
        add    esi, 4
        dec    edi
        jne    loc_2_12E7
loc_2_12F2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1300:
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
        jbe    loc_2_137C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_1340:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_136A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_1396
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_0CBC
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_136A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_1340
loc_2_137C:
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
loc_2_1396:
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
sub_2_13C0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_13F0
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_13F0:
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
        call   sub_1_0CBC
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1420:
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
        je     loc_2_14B9
        dec    esi
        cmp    esi, edx
        jb     loc_2_14B9
        nop    
loc_2_1440:
        cmp    edx, ecx
        jb     loc_2_14B9
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_1463
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1497
loc_2_1463:
        jbe    loc_2_1470
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_1497
        db  08Bh, 0FFh
loc_2_1470:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_1487
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1497
loc_2_1487:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_1497
        mov    dword ptr [ebp - 4], 0
loc_2_1497:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_14C9
        jle    loc_2_14A8
        or     edx, edx
        je     loc_2_14B9
        lea    esi, [edx - 1]
        jmp    loc_2_14AB
loc_2_14A8:
        lea    ecx, [edx + 1]
loc_2_14AB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_1440
loc_2_14B9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_14C9:
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
