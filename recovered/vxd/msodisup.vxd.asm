; MSODISUP -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 4639 instructions recovered by recursive descent

Declare_Virtual_Device MSODISUP, 1, 0, MSODISUP_Control, 00497h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h
;   service table: obj1:0x37dc, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x39bc  R-X
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        or     ax, ax
        jne    loc_1_0010
        sub    eax, eax
        pop    ebp
        ret    
loc_1_0010:
        and    ax, 0x7fff
        cmp    ax, 0xb
        jbe    loc_1_0020
        sub    eax, eax
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0020:
        and    eax, 0xffff
        mov    eax, dword ptr [eax*4 + 0x38f4] ; -> obj1:0x38F4
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_1_0030:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x18]
        dec    eax
        mov    dword ptr [ecx + 0x18], eax
        jne    loc_1_015A
        cmp    dword ptr [ecx + 0x14], 5
        jne    loc_1_00D9
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx]
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_0070
        mov    dword ptr [ecx + 0x14], 0
        mov    dword ptr [esi + 0x34], 1
        mov    eax, 0x103
        jmp    loc_1_00C4
loc_1_0070:
        cmp    dword ptr [esi + 0x30], 1
        jne    loc_1_007D
        mov    dword ptr [esi + 0x20], 0
loc_1_007D:
        mov    edx, dword ptr [esi + 0x2c]
        or     edx, edx
        je     loc_1_00AF
        cmp    edx, ecx
        jne    loc_1_0090
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esi + 0x2c], eax
        jmp    loc_1_00AF
loc_1_0090:
        mov    eax, dword ptr [edx + 0x10]
        or     eax, eax
        je     loc_1_00AF
loc_1_0097:
        cmp    eax, ecx
        je     loc_1_00A6
        mov    edx, eax
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_0097
        jmp    loc_1_00AF
loc_1_00A6:
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [edx + 0x10], eax
        dec    dword ptr [esi + 0x30]
loc_1_00AF:
        mov    eax, dword ptr [ecx + 0x1c]
        or     dword ptr [esi + 0xec], eax
        push   0
        push   0x28
        push   ecx
        call   sub_1_36FC
        sub    eax, eax
loc_1_00C4:
        or     eax, eax
        jne    loc_1_015A
        push   0
        push   edi
        call   sub_1_3634
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_00D9:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x20], 2
        jne    loc_1_015A
        cmp    dword ptr [esi + 0x38], 0
        jne    loc_1_015A
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2c]
        cmp    eax, ecx
        je     loc_1_00FB
loc_1_00F1:
        add    ecx, dword ptr [eax + 0x18]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_00F1
loc_1_00FB:
        or     ecx, ecx
        jne    loc_1_015A
        push   esi
        call   sub_1_323D
        add    esp, 4
        mov    dword ptr [esi + 0x38], 1
        mov    dword ptr [esi + 0x20], 1
        inc    dword ptr [esi + 0x18]
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_014A
        mov    ebx, 1
loc_1_0125:
        cmp    dword ptr [edi + 0x14], ebx
        jne    loc_1_0143
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
loc_1_0143:
        mov    edi, dword ptr [edi + 0x10]
        or     edi, edi
        jne    loc_1_0125
loc_1_014A:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        mov    dword ptr [esi + 0x20], 1
loc_1_015A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  090h
sub_1_0160:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 6
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    ax, word ptr [eax + ecx + 0x32]
        and    eax, 0xffff
        sub    eax, 4
        and    eax, dword ptr [0x386c]       ; -> obj1:0x386C
        mov    ecx, dword ptr [0x3874]       ; -> obj1:0x3874
        mov    word ptr [eax + ecx + 2], 0x7569
        cmp    dword ptr [0x3864], eax       ; -> obj1:0x3864
        jne    loc_1_01EF
        cmp    dword ptr [0x38d8], 0         ; -> obj1:0x38D8
        je     loc_1_01EF
        mov    edx, 0x7569
        sub    esi, esi
loc_1_01A8:
        mov    eax, dword ptr [0x3874]       ; -> obj1:0x3874
        add    eax, dword ptr [0x3864]       ; -> obj1:0x3864
        cmp    word ptr [eax + 2], dx
        jne    loc_1_01EF
        dec    dword ptr [0x38d8]            ; -> obj1:0x38D8
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [0x3864]       ; -> obj1:0x3864
        add    ecx, eax
        add    ecx, 4
        and    ecx, dword ptr [0x386c]       ; -> obj1:0x386C
        mov    dword ptr [0x3864], ecx       ; -> obj1:0x3864
        add    eax, 4
        add    dword ptr [0x3860], eax       ; -> obj1:0x3860
        cmp    dword ptr [0x38d8], esi       ; -> obj1:0x38D8
        jne    loc_1_01A8
loc_1_01EF:
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0200:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 6
        mov    ecx, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    ax, word ptr [eax + ecx + 0x2a]
        and    eax, 0xffff
        sub    eax, 4
        and    eax, dword ptr [0x3888]       ; -> obj1:0x3888
        mov    ecx, dword ptr [0x3890]       ; -> obj1:0x3890
        mov    word ptr [eax + ecx + 2], 0x7569
        cmp    dword ptr [0x3880], eax       ; -> obj1:0x3880
        jne    loc_1_028F
        cmp    dword ptr [0x38dc], 0         ; -> obj1:0x38DC
        je     loc_1_028F
        mov    edx, 0x7569
        sub    esi, esi
loc_1_0248:
        mov    eax, dword ptr [0x3880]       ; -> obj1:0x3880
        add    eax, dword ptr [0x3890]       ; -> obj1:0x3890
        cmp    word ptr [eax + 2], dx
        jne    loc_1_028F
        dec    dword ptr [0x38dc]            ; -> obj1:0x38DC
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [0x3880]       ; -> obj1:0x3880
        add    ecx, eax
        add    ecx, 4
        and    ecx, dword ptr [0x3888]       ; -> obj1:0x3888
        mov    dword ptr [0x3880], ecx       ; -> obj1:0x3880
        add    eax, 4
        add    dword ptr [0x387c], eax       ; -> obj1:0x387C
        cmp    dword ptr [0x38dc], esi       ; -> obj1:0x38DC
        jne    loc_1_0248
loc_1_028F:
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 3
        and    eax, 0xfffffffc
        mov    dword ptr [ebp - 0x18], eax
        or     eax, 0x55490000
        mov    ecx, dword ptr [0x3874]       ; -> obj1:0x3874
        mov    edx, dword ptr [0x3868]       ; -> obj1:0x3868
        mov    dword ptr [ecx + edx], eax
        mov    eax, dword ptr [0x3868]       ; -> obj1:0x3868
        add    eax, 4
        and    eax, dword ptr [0x386c]       ; -> obj1:0x386C
        mov    dword ptr [0x3868], eax       ; -> obj1:0x3868
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        sub    dword ptr [0x3860], eax       ; -> obj1:0x3860
        mov    esi, dword ptr [ebp + 0x10]
        shl    esi, 6
        mov    eax, dword ptr [0x3868]       ; -> obj1:0x3868
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + ecx + 0x32], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0x10]
        push   0
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3720
        mov    dword ptr [ebp - 4], 0
        mov    edi, dword ptr [0x3874]       ; -> obj1:0x3874
        add    edi, dword ptr [0x3868]       ; -> obj1:0x3868
        mov    eax, dword ptr [0x3864]       ; -> obj1:0x3864
        cmp    dword ptr [0x3868], eax       ; -> obj1:0x3868
        jb     loc_1_0340
        mov    ebx, dword ptr [0x3870]       ; -> obj1:0x3870
        jmp    loc_1_0346
        db  090h
loc_1_0340:
        mov    ebx, dword ptr [0x3864]       ; -> obj1:0x3864
loc_1_0346:
        sub    ebx, dword ptr [0x3868]       ; -> obj1:0x3868
        mov    cx, word ptr [ebp + 0xc]
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x2e], cx
        cmp    dword ptr [ebp + 0xc], ebx
        ja     loc_1_0380
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x30], 1
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x36], cx
        jmp    loc_1_03A6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0380:
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x30], 2
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x36], bx
        sub    cx, bx
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x3c], cx
        mov    dword ptr [ebp - 0x14], ebx
loc_1_03A6:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_03F0
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3744
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ecx + esi + 0x21], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ecx + esi + 0x19], al
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x12], 0
        jmp    loc_1_03FC
        db  005h, 000h, 000h, 000h, 000h
loc_1_03F0:
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [esi + eax + 0x12], 0xffff
loc_1_03FC:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_047D
loc_1_0402:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0426
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3744
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3768
loc_1_0426:
        cmp    ebx, dword ptr [ebp - 4]
        jl     loc_1_0454
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        call   sub_1_3508
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0477
loc_1_0454:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        call   sub_1_3508
        add    esp, 0xc
        sub    dword ptr [ebp - 8], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
        mov    ebx, dword ptr [ebp + 0xc]
        sub    ebx, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [0x3874]       ; -> obj1:0x3874
loc_1_0477:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0402
loc_1_047D:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_04A3
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        sub    word ptr [esi + eax + 0x36], 3
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        add    dword ptr [esi + eax + 0x32], 3
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        sub    word ptr [esi + eax + 0x2e], 3
loc_1_04A3:
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, dword ptr [0x3868]       ; -> obj1:0x3868
        and    eax, dword ptr [0x386c]       ; -> obj1:0x386C
        mov    dword ptr [0x3868], eax       ; -> obj1:0x3868
        inc    dword ptr [0x38d8]            ; -> obj1:0x38D8
        mov    eax, dword ptr [0x3864]       ; -> obj1:0x3864
        cmp    dword ptr [0x3868], eax       ; -> obj1:0x3868
        jbe    loc_1_0508
        mov    eax, dword ptr [0x3870]       ; -> obj1:0x3870
        sub    eax, dword ptr [0x3868]       ; -> obj1:0x3868
        cmp    eax, 0x34
        jge    loc_1_0508
        lea    edx, [eax - 4]
        or     edx, 0x75690000
        mov    esi, dword ptr [0x3874]       ; -> obj1:0x3874
        mov    ecx, dword ptr [0x3868]       ; -> obj1:0x3868
        mov    dword ptr [esi + ecx], edx
        sub    dword ptr [0x3860], eax       ; -> obj1:0x3860
        mov    dword ptr [0x3868], 0         ; -> obj1:0x3868
        inc    dword ptr [0x38d8]            ; -> obj1:0x38D8
loc_1_0508:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
sub_1_0510:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0x3884]       ; -> obj1:0x3884
        lea    ecx, [edx + 3]
        and    ecx, 0xfffffffc
        mov    eax, ecx
        or     eax, 0x55490000
        mov    edi, dword ptr [0x3890]       ; -> obj1:0x3890
        mov    dword ptr [esi + edi], eax
        mov    eax, dword ptr [0x3884]       ; -> obj1:0x3884
        add    eax, 4
        and    eax, dword ptr [0x3888]       ; -> obj1:0x3888
        mov    dword ptr [0x3884], eax       ; -> obj1:0x3884
        lea    eax, [ecx + 4]
        sub    dword ptr [0x387c], eax       ; -> obj1:0x387C
        mov    esi, dword ptr [ebp + 0xc]
        shl    esi, 6
        mov    eax, dword ptr [0x3884]       ; -> obj1:0x3884
        mov    edi, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    word ptr [esi + edi + 0x32], ax
        mov    eax, dword ptr [0x3880]       ; -> obj1:0x3880
        cmp    dword ptr [0x3884], eax       ; -> obj1:0x3884
        jb     loc_1_0580
        mov    edi, dword ptr [0x388c]       ; -> obj1:0x388C
        jmp    loc_1_0586
        db  005h, 000h, 000h, 000h, 000h
loc_1_0580:
        mov    edi, dword ptr [0x3880]       ; -> obj1:0x3880
loc_1_0586:
        sub    edi, dword ptr [0x3884]       ; -> obj1:0x3884
        cmp    edx, edi
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        jg     loc_1_05A8
        mov    word ptr [esi + eax + 0x30], 1
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    word ptr [esi + eax + 0x36], dx
        jmp    loc_1_05C6
loc_1_05A8:
        mov    word ptr [esi + eax + 0x30], 2
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    word ptr [esi + eax + 0x36], di
        sub    dx, di
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    word ptr [esi + eax + 0x3c], dx
loc_1_05C6:
        mov    eax, dword ptr [0x3884]       ; -> obj1:0x3884
        add    eax, ecx
        and    eax, dword ptr [0x3888]       ; -> obj1:0x3888
        mov    dword ptr [0x3884], eax       ; -> obj1:0x3884
        inc    dword ptr [0x38dc]            ; -> obj1:0x38DC
        mov    eax, dword ptr [0x3880]       ; -> obj1:0x3880
        cmp    dword ptr [0x3884], eax       ; -> obj1:0x3884
        jbe    loc_1_062B
        mov    eax, dword ptr [0x388c]       ; -> obj1:0x388C
        sub    eax, dword ptr [0x3884]       ; -> obj1:0x3884
        cmp    eax, 0x104
        jge    loc_1_062B
        lea    ecx, [eax - 4]
        or     ecx, 0x75690000
        mov    edx, dword ptr [0x3884]       ; -> obj1:0x3884
        mov    esi, dword ptr [0x3890]       ; -> obj1:0x3890
        mov    dword ptr [edx + esi], ecx
        sub    dword ptr [0x387c], eax       ; -> obj1:0x387C
        mov    dword ptr [0x3884], 0         ; -> obj1:0x3884
        inc    dword ptr [0x38dc]            ; -> obj1:0x38DC
loc_1_062B:
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  090h
sub_1_0630:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x14], 0
        mov    ecx, dword ptr [0x38e0]       ; -> obj1:0x38E0
        movsx  eax, word ptr [eax + 0x10]
        mov    esi, dword ptr [ecx + eax*4]
        inc    dword ptr [0x3924]            ; -> obj1:0x3924
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        sub    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x20], eax
        mov    edi, dword ptr [esi + 0xf4]
        cmp    edi, 1
        jne    loc_1_06A6
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 0xe
        test   byte ptr [eax + 8], 0x80
        je     loc_1_0691
        mov    dl, byte ptr [eax + 0xe]
        and    edx, 0x1f
        add    edx, 0xe
loc_1_0691:
        cmp    edi, 7
        mov    dword ptr [esi + 0xf8], 3
        je     loc_1_06A6
        mov    dword ptr [esi + 0xf8], edx
loc_1_06A6:
        cmp    dword ptr [esi + 0x106], 0
        jne    loc_1_0712
        mov    eax, dword ptr [esi + 0xf4]
        or     eax, eax
        jne    loc_1_06E1
        mov    eax, dword ptr [ebp + 0xc]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0xc]
        shl    ecx, 8
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0x5dc
        jle    loc_1_071C
        mov    eax, dword ptr [esi + 8]
        movsx  eax, word ptr [eax + 0x28]
        jmp    loc_1_0719
loc_1_06E1:
        cmp    eax, 7
        jne    loc_1_0709
        mov    al, byte ptr [ecx - 3]
        or     al, al
        jne    loc_1_06F6
        mov    dword ptr [ebp - 8], 0x200
        jmp    loc_1_071C
loc_1_06F6:
        and    eax, 0xff
        dec    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        add    eax, 2
        shl    eax, 9
        jmp    loc_1_0719
loc_1_0709:
        mov    eax, dword ptr [esi + 8]
        movsx  eax, word ptr [eax + 0x28]
        jmp    loc_1_0719
loc_1_0712:
        mov    eax, dword ptr [ebp + 0x10]
        movsx  eax, word ptr [eax + 0x12]
loc_1_0719:
        mov    dword ptr [ebp - 8], eax
loc_1_071C:
        mov    ecx, dword ptr [ebp - 0x20]
        sub    ecx, dword ptr [esi + 0xf8]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        add    eax, ecx
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [esi + 0xf4], 7
        je     loc_1_0742
        mov    eax, dword ptr [ebp - 8]
        add    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 8], eax
loc_1_0742:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [0x387c]       ; -> obj1:0x387C
        jge    loc_1_0A51
        cmp    dword ptr [0x38ec], -1        ; -> obj1:0x38EC
        je     loc_1_0A51
        inc    dword ptr [esi + 0x18]
        mov    edx, dword ptr [esi + 0xfc]
        add    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], edx
        mov    ecx, edx
        add    ecx, 6
        mov    edi, 0x20
        cmp    dword ptr [esi + 0xf4], 7
        jne    loc_1_07A6
        mov    al, byte ptr [edx]
        or     al, al
        jne    loc_1_0790
        mov    edi, 0x28
        jmp    loc_1_0837
loc_1_0790:
        cmp    byte ptr [esi + 0x105], al
        jne    loc_1_0837
        mov    edi, 0x21
        jmp    loc_1_0837
loc_1_07A6:
        mov    eax, dword ptr [ebp - 0x10]
        lea    edx, [esi + 0x100]
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    loc_1_07C6
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_07C6
        mov    edi, 0x21
        jmp    loc_1_0837
loc_1_07C6:
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, 0x39b4                   ; -> obj1:0x39B4
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    loc_1_07E5
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_07E5
        mov    edi, 0x28
        jmp    loc_1_0837
loc_1_07E5:
        cmp    dword ptr [esi + 0xf4], 1
        mov    edx, dword ptr [ebp - 0x10]
        je     loc_1_081B
        test   byte ptr [edx], 1
        je     loc_1_0837
        mov    edi, 0x24
        push   edx
        mov    eax, dword ptr [esi + 0xa0]
        push   eax
        lea    eax, [esi + 0x80]
        push   eax
        lea    eax, [esi + 0x3c]
        push   eax
        call   sub_1_3539
        add    esp, 0x10
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_0837
loc_1_081B:
        mov    al, byte ptr [edx]
        test   al, 0x80
        je     loc_1_082F
        mov    edi, 0x1020
        test   al, 0x40
        je     loc_1_082F
        mov    edi, 0x3020
loc_1_082F:
        test   byte ptr [ecx], 0x80
        je     loc_1_0837
        or     edi, 0x10
loc_1_0837:
        mov    ebx, dword ptr [esi + 0x2c]
        or     ebx, ebx
        je     loc_1_08F4
loc_1_0842:
        cmp    dword ptr [ebx + 0x14], 1
        jne    loc_1_08E9
        test   dword ptr [ebx + 0x20], edi
        jne    loc_1_0886
        mov    ecx, dword ptr [esi + 0xf4]
        cmp    ecx, 1
        je     loc_1_087B
        cmp    ecx, 7
        je     loc_1_0876
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_0876
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [esi + eax*4 + 0xa0]
        test   dword ptr [ebx + 0x1c], eax
        jne    loc_1_0886
loc_1_0876:
        cmp    ecx, 1
        jne    loc_1_08E9
loc_1_087B:
        mov    edx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebx + 0x24]
        test   dword ptr [edx + 2], eax
        je     loc_1_08E9
loc_1_0886:
        mov    eax, dword ptr [ebp + 0x10]
        movsx  eax, word ptr [eax + 8]
        cmp    eax, dword ptr [ebp - 0xc]
        jb     loc_1_08E0
        cmp    dword ptr [0x38f0], 0         ; -> obj1:0x38F0
        je     loc_1_08B0
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_08B0
        mov    dword ptr [ebp - 0x1c], 1
        jmp    loc_1_08E0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_08B0:
        mov    ecx, dword ptr [esi + 0xf8]
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, ecx
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_1_3652
        push   ebx
        call   sub_1_2D1B
        add    esp, 4
        jmp    loc_1_08E9
        db  090h
loc_1_08E0:
        mov    eax, dword ptr [ebp - 4]
        or     eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [ebp - 4], eax
loc_1_08E9:
        mov    ebx, dword ptr [ebx + 0x10]
        or     ebx, ebx
        jne    loc_1_0842
loc_1_08F4:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0A42
        mov    ebx, dword ptr [0x38ec]       ; -> obj1:0x38EC
        mov    edi, ebx
        shl    edi, 6
        mov    ecx, dword ptr [0x3894]       ; -> obj1:0x3894
        add    ecx, edi
        inc    dword ptr [0x38f0]            ; -> obj1:0x38F0
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [0x38ec], eax       ; -> obj1:0x38EC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0510
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        add    eax, edi
        add    esp, 8
        cmp    dword ptr [ebp - 0x1c], 0
        mov    cx, word ptr [eax + 0x32]
        mov    word ptr [eax + 0x2a], cx
        mov    eax, dword ptr [ebp + 0x10]
        mov    cx, word ptr [eax + 0x10]
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    word ptr [edi + eax + 0x1a], cx
        je     loc_1_0970
        cmp    dword ptr [ebp - 0xc], 0x80
        jbe    loc_1_09A0
        mov    dword ptr [ebp - 0x1c], 0
loc_1_0970:
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        xor    ecx, ecx
        mov    cx, word ptr [edi + eax + 0x32]
        add    ecx, dword ptr [0x3890]       ; -> obj1:0x3890
        cmp    dword ptr [esi + 0xf4], 7
        je     loc_1_09F0
        mov    edx, dword ptr [ebp - 0x20]
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        call   sub_1_3508
        add    esp, 0xc
        jmp    loc_1_0A09
        db  08Bh, 0FFh
loc_1_09A0:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        shl    bx, 8
        mov    ax, word ptr [edi + eax + 0x32]
        and    eax, 0xffff
        add    eax, dword ptr [0x3890]       ; -> obj1:0x3890
        push   eax
        call   sub_1_3508
        add    esp, 0xc
        add    bx, 0x51
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    ax, word ptr [edi + eax + 0x32]
        and    eax, 0xffff
        mov    ecx, dword ptr [0x3890]       ; -> obj1:0x3890
        mov    word ptr [eax + ecx - 2], bx
        jmp    loc_1_0A09
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_09F0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x20], 3
        mov    ax, word ptr [eax]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0xf]
        mov    byte ptr [ecx + 2], al
loc_1_0A09:
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    edx, dword ptr [ebp - 0x20]
        add    dword ptr [edi + eax + 0x32], edx
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        sub    word ptr [edi + eax + 0x36], dx
        dec    dword ptr [0x3924]            ; -> obj1:0x3924
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_0A39
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        add    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0A39:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0A42:
        dec    dword ptr [0x3924]            ; -> obj1:0x3924
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0A51:
        dec    dword ptr [0x3924]            ; -> obj1:0x3924
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_0A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    esp, [esp]
loc_1_0A70:
        mov    ecx, dword ptr [0x38e0]       ; -> obj1:0x38E0
        movsx  eax, word ptr [esi + 0x1a]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [esi]
        movsx  ecx, word ptr [esi + 0x2a]
        mov    eax, dword ptr [0x3890]       ; -> obj1:0x3890
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        movsx  eax, word ptr [esi + 0x3e]
        mov    dword ptr [ebp - 0x14], eax
        sub    ecx, dword ptr [0x3880]       ; -> obj1:0x3880
        cmp    ecx, 4
        jne    loc_1_0C28
        cmp    word ptr [esi + 0xc], 0
        je     loc_1_0AD8
        push   eax
        call   sub_1_0200
        add    esp, 4
        mov    ecx, dword ptr [0x38ec]       ; -> obj1:0x38EC
        mov    edx, dword ptr [ebp - 0x14]
        shl    edx, 6
        mov    eax, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    dword ptr [edx + eax + 4], ecx
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [0x38ec], eax       ; -> obj1:0x38EC
        jmp    loc_1_0BF6
loc_1_0AD8:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xf4]
        cmp    eax, 1
        jne    loc_1_0B17
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, 0xe
        test   byte ptr [edx + 8], 0x80
        je     loc_1_0AFD
        mov    cl, byte ptr [edx + 0xe]
        and    ecx, 0x1f
        add    ecx, 0xe
loc_1_0AFD:
        cmp    eax, 7
        mov    eax, dword ptr [ebp - 8]
        jne    loc_1_0B11
        mov    dword ptr [eax + 0xf8], 3
        jmp    loc_1_0B17
loc_1_0B11:
        mov    dword ptr [eax + 0xf8], ecx
loc_1_0B17:
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    eax, dword ptr [eax + 0xf8]
        mov    dword ptr [ebp - 0xc], eax
        movsx  eax, word ptr [esi + 0x2a]
        mov    cx, word ptr [esi + 0x32]
        sub    ecx, eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x1c], eax
        movsx  eax, word ptr [esi + 0x36]
        sub    ecx, dword ptr [ebp - 0xc]
        add    ecx, eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    dword ptr [ebp - 0x18], ecx
        cmp    word ptr [esi + 0x30], 2
        jne    loc_1_0B57
        movsx  eax, word ptr [esi + 0x3c]
        add    eax, ecx
        mov    dword ptr [ebp - 0x18], eax
loc_1_0B57:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x18]
        mov    ebx, dword ptr [eax + 0x2c]
        or     ebx, ebx
        je     loc_1_0B97
loc_1_0B64:
        cmp    dword ptr [ebx + 0x14], 1
        jne    loc_1_0B90
        test   dword ptr [ebx + 0x1c], edi
        je     loc_1_0B90
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   edx
        push   esi
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_3652
loc_1_0B90:
        mov    ebx, dword ptr [ebx + 0x10]
        or     ebx, ebx
        jne    loc_1_0B64
loc_1_0B97:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0200
        add    esp, 4
        mov    eax, dword ptr [0x38ec]       ; -> obj1:0x38EC
        mov    ecx, dword ptr [ebp - 0x14]
        shl    ecx, 6
        mov    edx, dword ptr [0x3894]       ; -> obj1:0x3894
        mov    dword ptr [ecx + edx + 4], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [0x38ec], eax       ; -> obj1:0x38EC
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 0x2c]
        or     esi, esi
        je     loc_1_0BEA
        mov    ebx, 1
        nop    
loc_1_0BD0:
        cmp    dword ptr [esi + 0x14], ebx
        jne    loc_1_0BE3
        test   dword ptr [esi + 0x1c], edi
        je     loc_1_0BE3
        push   esi
        call   sub_1_2D1B
        add    esp, 4
loc_1_0BE3:
        mov    esi, dword ptr [esi + 0x10]
        or     esi, esi
        jne    loc_1_0BD0
loc_1_0BEA:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0EF0
        add    esp, 4
loc_1_0BF6:
        dec    dword ptr [0x38f0]            ; -> obj1:0x38F0
        cmp    dword ptr [0x38dc], 0         ; -> obj1:0x38DC
        je     loc_1_0C3A
        mov    eax, dword ptr [0x3880]       ; -> obj1:0x3880
        add    eax, dword ptr [0x3890]       ; -> obj1:0x3890
        cmp    byte ptr [eax + 2], 0x51
        jne    loc_1_0C3A
        movzx  esi, byte ptr [eax + 3]
        shl    esi, 6
        add    esi, dword ptr [0x3894]       ; -> obj1:0x3894
        jmp    loc_1_0A70
loc_1_0C28:
        mov    eax, dword ptr [ebp - 0x14]
        shl    ax, 8
        mov    edx, dword ptr [ebp - 4]
        add    ax, 0x51
        mov    word ptr [edx - 2], ax
loc_1_0C3A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0x38e0]       ; -> obj1:0x38E0
        movsx  eax, word ptr [edx + 0x1a]
        mov    edi, dword ptr [ecx + eax*4]
        movsx  eax, word ptr [edx + 0x3e]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edx]
        push   0
        mov    esi, dword ptr [eax + 0x24]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_363A
        mov    ecx, dword ptr [edi + 0xf4]
        cmp    ecx, 7
        mov    ebx, dword ptr [ebp - 4]
        jne    loc_1_0CE2
        shl    ebx, 6
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [ebx + eax + 0x12], 0xffff
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        sub    dword ptr [ebx + eax + 0x32], 3
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        add    word ptr [ebx + eax + 0x36], 3
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        add    word ptr [ebx + eax + 0x2e], 3
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        lea    ecx, [ebx + eax]
        mov    al, byte ptr [ecx + 0x21]
        or     al, al
        je     loc_1_0CDF
        xor    edx, edx
        mov    dl, byte ptr [edi + 0x105]
        movsx  eax, al
        cmp    edx, eax
        jne    loc_1_0D5D
loc_1_0CDF:
        push   ecx
        jmp    loc_1_0D54
loc_1_0CE2:
        shl    ebx, 6
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    eax, dword ptr [ebx + eax + 0x32]
        mov    edx, eax
        and    edx, 0xffff0fff
        shr    edx, 0xc
        add    edx, dword ptr [edi + 0xfc]
        and    eax, 0xffff
        add    eax, edx
        test   byte ptr [edi + 0xe4], 0x20
        jne    loc_1_0D4C
        cmp    ecx, 1
        je     loc_1_0D31
        test   byte ptr [eax], 1
        jne    loc_1_0D4C
        lea    ecx, [edi + 0x100]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_1_0D2D
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_0D2D:
        je     loc_1_0D4C
        jmp    loc_1_0D5D
loc_1_0D31:
        test   byte ptr [eax], 0x80
        jne    loc_1_0D4C
        lea    ecx, [edi + 0x100]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_1_0D5D
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    loc_1_0D5D
loc_1_0D4C:
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        add    eax, ebx
        push   eax
loc_1_0D54:
        push   edi
        call   sub_1_1350
        add    esp, 8
loc_1_0D5D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0160
        add    esp, 4
        mov    eax, dword ptr [0x38e8]       ; -> obj1:0x38E8
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    dword ptr [ebx + ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x38e8], eax       ; -> obj1:0x38E8
        push   esi
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_1_0030
        add    esp, 8
        call   sub_1_11C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [edx], 0xffffffff
        mov    eax, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x18]
        lea    eax, [eax + edi*4]
        mov    esi, dword ptr [ebp + 0x20]
loc_1_0DC6:
        or     edi, edi
        je     loc_1_0DDA
        sub    eax, 4
        dec    edi
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [esi + 0xf4], ecx
        jne    loc_1_0DC6
        mov    dword ptr [edx], edi
loc_1_0DDA:
        cmp    dword ptr [edx], -1
        jne    loc_1_0DF0
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h
loc_1_0DF0:
        cmp    dword ptr [esi + 0x20], 2
        jne    loc_1_0E03
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0E03:
        mov    ecx, 1
        mov    eax, dword ptr [esi + 0xec]
        mov    edi, edi
loc_1_0E10:
        test   ecx, eax
        jne    loc_1_0E1C
        add    ecx, ecx
        or     ecx, ecx
        jne    loc_1_0E10
        jmp    loc_1_0E2B
loc_1_0E1C:
        mov    edx, ecx
        not    edx
        and    edx, eax
        mov    dword ptr [esi + 0xec], edx
        mov    dword ptr [ebp - 8], ecx
loc_1_0E2B:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0E35
        sub    ecx, ecx
        jmp    loc_1_0EAB
loc_1_0E35:
        push   -1
        push   0
        push   0x28
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_36F6
        or     eax, eax
        je     loc_1_0E55
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [esi + 0xec], eax
        sub    ecx, ecx
        jmp    loc_1_0EAB
loc_1_0E55:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x24], 0
        mov    ecx, dword ptr [ebp - 4]
loc_1_0EAB:
        or     ecx, ecx
        jne    loc_1_0EC0
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h
loc_1_0EC0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
        inc    dword ptr [esi + 0x30]
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [esi + 0x2c], ecx
        cmp    dword ptr [esi + 0x20], 0
        jne    loc_1_0EDE
        mov    dword ptr [esi + 0x20], 1
loc_1_0EDE:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0EF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x18]
        dec    eax
        mov    dword ptr [esi + 0x18], eax
        cmp    dword ptr [esi + 0x34], edi
        je     loc_1_0F9E
        cmp    eax, edi
        jne    loc_1_0F9E
        mov    ecx, dword ptr [esi + 0x2c]
        cmp    ecx, edi
        je     loc_1_0F6D
loc_1_0F1A:
        mov    edx, dword ptr [ecx + 0x10]
        cmp    dword ptr [ecx + 0x14], 0
        jne    loc_1_0F67
        cmp    dword ptr [esi + 0x30], 1
        jne    loc_1_0F30
        mov    dword ptr [esi + 0x20], 0
loc_1_0F30:
        mov    eax, dword ptr [esi + 0x2c]
        or     eax, eax
        je     loc_1_0F62
        cmp    ecx, eax
        jne    loc_1_0F43
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esi + 0x2c], eax
        jmp    loc_1_0F62
loc_1_0F43:
        mov    ebx, dword ptr [eax + 0x10]
        or     ebx, ebx
        je     loc_1_0F62
loc_1_0F4A:
        cmp    ebx, ecx
        je     loc_1_0F59
        mov    eax, ebx
        mov    ebx, dword ptr [ebx + 0x10]
        or     ebx, ebx
        jne    loc_1_0F4A
        jmp    loc_1_0F62
loc_1_0F59:
        mov    ebx, dword ptr [ebx + 0x10]
        mov    dword ptr [eax + 0x10], ebx
        dec    dword ptr [esi + 0x30]
loc_1_0F62:
        mov    dword ptr [ecx + 0x10], edi
        mov    edi, ecx
loc_1_0F67:
        mov    ecx, edx
        or     edx, edx
        jne    loc_1_0F1A
loc_1_0F6D:
        mov    dword ptr [esi + 0x34], 0
        or     edi, edi
        je     loc_1_0F9E
loc_1_0F78:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3634
        mov    ebx, dword ptr [edi + 0x10]
        mov    eax, dword ptr [edi + 0x1c]
        or     dword ptr [esi + 0xec], eax
        push   0
        push   0x28
        push   edi
        call   sub_1_36FC
        mov    edi, ebx
        or     ebx, ebx
        jne    loc_1_0F78
loc_1_0F9E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xc]
        cmp    dword ptr [edi + 0x20], 2
        jne    loc_1_0FD1
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FD1:
        cmp    dword ptr [esi + 0x14], 1
        je     loc_1_0FE4
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FE4:
        mov    dword ptr [esi + 0x14], 2
        push   -1
        push   0
        push   0x2c
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_36F6
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1014
        mov    dword ptr [esi + 0x14], 1
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1014:
        mov    dword ptr [esi + 0x20], 0
        mov    eax, dword ptr [esi + 0xc]
        sub    ecx, ecx
        mov    eax, dword ptr [eax + 0x2c]
        cmp    eax, ecx
        je     loc_1_1031
loc_1_1027:
        or     ecx, dword ptr [eax + 0x20]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_1027
loc_1_1031:
        mov    dword ptr [edi + 0xe4], ecx
        or     ecx, ecx
        je     loc_1_1046
        mov    eax, dword ptr [edi + 4]
        or     cl, 1
        mov    byte ptr [eax + 3], cl
        jmp    loc_1_104D
loc_1_1046:
        mov    eax, dword ptr [edi + 4]
        mov    byte ptr [eax + 3], 0
loc_1_104D:
        push   0
        push   esi
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1070
        add    esp, 0x10
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1070:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xf4], 1
        je     loc_1_10A8
        push   0
        push   0x2c
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_36FC
        mov    esi, dword ptr [ebp + 0x10]
        push   0
        mov    dword ptr [esi + 0x14], 4
        push   esi
        call   sub_1_1F50
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_10E2
loc_1_10A8:
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x14], 3
        mov    dword ptr [edi + 0x14], 0x2010103
        lea    eax, [edi + 0x28]
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [eax], 0
        push   edi
        push   esi
        call   sub_1_2000
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_10E2
        push   0
        push   0x2c
        push   edi
        call   sub_1_36FC
loc_1_10E2:
        cmp    ebx, 0x103
        jne    loc_1_10F4
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_10F4:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_1110
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x14], 5
        cmp    dword ptr [edx + 0x18], 0
        je     loc_1_1130
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_1130:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx]
        cmp    dword ptr [ecx + 0x18], 0
        je     loc_1_1150
        mov    dword ptr [edx + 0x14], 0
        mov    dword ptr [ecx + 0x34], 1
        mov    edi, 0x103
        jmp    loc_1_11A4
loc_1_1150:
        cmp    dword ptr [ecx + 0x30], 1
        jne    loc_1_115D
        mov    dword ptr [ecx + 0x20], 0
loc_1_115D:
        mov    edi, dword ptr [ecx + 0x2c]
        or     edi, edi
        je     loc_1_118F
        cmp    edi, edx
        jne    loc_1_1170
        mov    eax, dword ptr [edx + 0x10]
        mov    dword ptr [ecx + 0x2c], eax
        jmp    loc_1_118F
loc_1_1170:
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_118F
loc_1_1177:
        cmp    eax, edx
        je     loc_1_1186
        mov    edi, eax
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_1177
        jmp    loc_1_118F
loc_1_1186:
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [edi + 0x10], eax
        dec    dword ptr [ecx + 0x30]
loc_1_118F:
        mov    eax, dword ptr [edx + 0x1c]
        or     dword ptr [ecx + 0xec], eax
        push   0
        push   0x28
        sub    edi, edi
        push   edx
        call   sub_1_36FC
loc_1_11A4:
        cmp    edi, 0x103
        je     loc_1_11B9
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_11B9
        push   edi
        push   esi
        call   sub_1_3634
loc_1_11B9:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  090h
sub_1_11C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0x3928], 0         ; -> obj1:0x3928
        cmp    dword ptr [0x38d0], 0         ; -> obj1:0x38D0
        je     loc_1_1343
        cmp    dword ptr [0x38a0], 0         ; -> obj1:0x38A0
        jne    loc_1_1343
        mov    dword ptr [0x38a0], 1         ; -> obj1:0x38A0
        cmp    dword ptr [0x38d0], 0         ; -> obj1:0x38D0
        je     loc_1_1339
loc_1_1204:
        mov    ecx, dword ptr [0x38d0]       ; -> obj1:0x38D0
        mov    dword ptr [ebp - 4], ecx
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_3720
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [0x3860], eax       ; -> obj1:0x3860
        jle    loc_1_1339
        cmp    dword ptr [0x38e8], -1        ; -> obj1:0x38E8
        je     loc_1_1339
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [edi + 0x24]
        add    edi, 0x24
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [0x38d0], eax       ; -> obj1:0x38D0
        or     eax, eax
        jne    loc_1_125E
        mov    dword ptr [0x38d4], 0         ; -> obj1:0x38D4
loc_1_125E:
        mov    eax, dword ptr [0x38e8]       ; -> obj1:0x38E8
        mov    dword ptr [ebp - 0xc], eax
        mov    ebx, eax
        shl    ebx, 6
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    eax, dword ptr [ebx + eax + 4]
        mov    dword ptr [0x38e8], eax       ; -> obj1:0x38E8
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xf4]
        sub    eax, 7
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_02A0
        add    esp, 0x10
        mov    dword ptr [edi], esi
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + ebx], ecx
        cmp    dword ptr [esi + 0x14], 1
        jne    loc_1_12E2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    eax, dword ptr [eax + 0x10]
        mov    word ptr [ecx + ebx + 0x1a], ax
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    edi, 0x103
        mov    eax, dword ptr [eax + ebx + 0x26]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2E2C
        add    esp, 8
        jmp    loc_1_12E7
loc_1_12E2:
        mov    edi, 0x40010007
loc_1_12E7:
        cmp    edi, 0x103
        je     loc_1_132C
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0160
        add    esp, 4
        mov    eax, dword ptr [0x38e8]       ; -> obj1:0x38E8
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    dword ptr [ebx + ecx + 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0x38e8], eax       ; -> obj1:0x38E8
        push   edi
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_363A
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0030
        add    esp, 8
loc_1_132C:
        cmp    dword ptr [0x38d0], 0         ; -> obj1:0x38D0
        jne    loc_1_1204
loc_1_1339:
        mov    dword ptr [0x38a0], 0         ; -> obj1:0x38A0
loc_1_1343:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1350:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], 0
        mov    word ptr [eax + 0x2a], 0xffff
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    ecx, eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0xf4]
        cmp    eax, 1
        jne    loc_1_13B8
        mov    edx, 0xe
        test   byte ptr [ecx + 8], 0x80
        je     loc_1_13A3
        mov    dl, byte ptr [ecx + 0xe]
        and    edx, 0x1f
        add    edx, 0xe
loc_1_13A3:
        cmp    eax, 7
        mov    dword ptr [esi + 0xf8], 3
        je     loc_1_13B8
        mov    dword ptr [esi + 0xf8], edx
loc_1_13B8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [esi + 0xfc]
        movsx  eax, word ptr [eax + 0x2e]
        sub    eax, dword ptr [esi + 0xf8]
        mov    edi, 0x20
        mov    dword ptr [ebp - 0x10], eax
        add    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], edx
        mov    ecx, edx
        add    ecx, 6
        cmp    dword ptr [esi + 0xf4], 7
        jne    loc_1_1410
        mov    al, byte ptr [edx]
        or     al, al
        jne    loc_1_13F7
        mov    edi, 0x28
        jmp    loc_1_14A2
loc_1_13F7:
        cmp    byte ptr [esi + 0x105], al
        jne    loc_1_14A2
        mov    edi, 0x21
        jmp    loc_1_14A2
        db  08Dh, 049h, 000h
loc_1_1410:
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [esi + 0x100]
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    loc_1_1430
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_1430
        mov    edi, 0x21
        jmp    loc_1_14A2
loc_1_1430:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, 0x39b4                   ; -> obj1:0x39B4
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    loc_1_1450
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_1450
        mov    edi, 0x28
        jmp    loc_1_14A2
        db  090h
loc_1_1450:
        cmp    dword ptr [esi + 0xf4], 1
        mov    edx, dword ptr [ebp - 8]
        je     loc_1_1486
        test   byte ptr [edx], 1
        je     loc_1_14A2
        mov    edi, 0x24
        push   edx
        mov    eax, dword ptr [esi + 0xa0]
        push   eax
        lea    eax, [esi + 0x80]
        push   eax
        lea    eax, [esi + 0x3c]
        push   eax
        call   sub_1_3539
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_14A2
loc_1_1486:
        mov    al, byte ptr [edx]
        test   al, 0x80
        je     loc_1_149A
        mov    edi, 0x1020
        test   al, 0x40
        je     loc_1_149A
        mov    edi, 0x3020
loc_1_149A:
        test   byte ptr [ecx], 0x80
        je     loc_1_14A2
        or     edi, 0x10
loc_1_14A2:
        inc    dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0x2c]
        or     ebx, ebx
        je     loc_1_152F
loc_1_14B0:
        cmp    dword ptr [ebx + 0x14], 1
        jne    loc_1_1528
        test   dword ptr [ebx + 0x20], edi
        jne    loc_1_14F0
        mov    ecx, dword ptr [esi + 0xf4]
        cmp    ecx, 1
        je     loc_1_14E5
        cmp    ecx, 7
        je     loc_1_14E0
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_14E0
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + eax*4 + 0xa0]
        test   dword ptr [ebx + 0x1c], eax
        jne    loc_1_14F0
loc_1_14E0:
        cmp    ecx, 1
        jne    loc_1_1528
loc_1_14E5:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebx + 0x24]
        test   dword ptr [edx + 2], eax
        je     loc_1_1528
loc_1_14F0:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0xf8]
        movsx  eax, word ptr [eax + 0x36]
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_3652
        push   ebx
        call   sub_1_2D1B
        add    esp, 4
loc_1_1528:
        mov    ebx, dword ptr [ebx + 0x10]
        or     ebx, ebx
        jne    loc_1_14B0
loc_1_152F:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
loc_1_1540:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [esi + 0x14], 5
        jne    loc_1_1566
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1566:
        cmp    dword ptr [0x38a0], 0         ; -> obj1:0x38A0
        jne    loc_1_1670
        cmp    dword ptr [0x3924], 0         ; -> obj1:0x3924
        jne    loc_1_1670
        call   sub_1_2D57
        or     eax, eax
        jne    loc_1_1670
        mov    dword ptr [0x38a0], 1         ; -> obj1:0x38A0
        cmp    dword ptr [0x38d0], 0         ; -> obj1:0x38D0
        jne    loc_1_1660
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        call   sub_1_3720
        mov    eax, dword ptr [0x3860]       ; -> obj1:0x3860
        cmp    dword ptr [ebp - 8], eax
        jge    loc_1_1663
        cmp    dword ptr [0x38e8], -1        ; -> obj1:0x38E8
        je     loc_1_1663
        mov    ecx, dword ptr [0x38e8]       ; -> obj1:0x38E8
        mov    ebx, ecx
        shl    ebx, 6
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    eax, dword ptr [ebx + eax + 4]
        mov    dword ptr [0x38e8], eax       ; -> obj1:0x38E8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf4]
        sub    eax, 7
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_02A0
        add    esp, 0x10
        mov    dword ptr [edi + 0x24], esi
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    dword ptr [ebx + eax], edi
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [esi + 0x18]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    word ptr [ebx + ecx + 0x1a], ax
        mov    eax, dword ptr [0x3878]       ; -> obj1:0x3878
        mov    eax, dword ptr [ebx + eax + 0x26]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2E2C
        add    esp, 8
        mov    dword ptr [0x38a0], 0         ; -> obj1:0x38A0
        call   sub_1_11C0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_1660:
        mov    edi, dword ptr [ebp + 0xc]
loc_1_1663:
        mov    dword ptr [0x38a0], 0         ; -> obj1:0x38A0
        jmp    loc_1_1673
        db  090h
loc_1_1670:
        mov    edi, dword ptr [ebp + 0xc]
loc_1_1673:
        inc    dword ptr [esi + 0x18]
        mov    dword ptr [edi + 0x28], 0
        mov    dword ptr [edi + 0x24], esi
        lea    eax, [edi + 0x24]
        cmp    dword ptr [0x38d4], 0         ; -> obj1:0x38D4
        je     loc_1_16A0
        mov    eax, dword ptr [0x38d4]       ; -> obj1:0x38D4
        mov    dword ptr [eax + 0x28], edi
        mov    dword ptr [0x38d4], edi       ; -> obj1:0x38D4
        jmp    loc_1_16AC
        db  08Dh, 064h, 024h, 000h
loc_1_16A0:
        mov    dword ptr [0x38d4], edi       ; -> obj1:0x38D4
        mov    dword ptr [0x38d0], edi       ; -> obj1:0x38D0
loc_1_16AC:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_16C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_16F4
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_16F4:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + 0x2a]
        cmp    ax, 0xffff
        je     loc_1_1793
        add    ecx, 0x32
        mov    dword ptr [ebp - 0x18], ecx
        movsx  esi, ax
        xor    edx, edx
        mov    dx, word ptr [ecx]
        mov    eax, dword ptr [ebp - 0x14]
        sub    edx, esi
        mov    eax, dword ptr [eax + 0xf4]
        cmp    eax, 1
        jne    loc_1_1766
        add    esi, dword ptr [0x3890]       ; -> obj1:0x3890
        mov    ecx, 0xe
        cmp    eax, 1
        jne    loc_1_174A
        test   byte ptr [esi + 8], 0x80
        je     loc_1_174A
        mov    cl, byte ptr [esi + 0xe]
        and    ecx, 0x1f
        add    ecx, 0xe
loc_1_174A:
        cmp    eax, 7
        mov    eax, dword ptr [ebp - 0x14]
        jne    loc_1_1760
        mov    dword ptr [eax + 0xf8], 3
        jmp    loc_1_1766
        db  08Bh, 0FFh
loc_1_1760:
        mov    dword ptr [eax + 0xf8], ecx
loc_1_1766:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xf8]
        sub    ax, dx
        add    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x36
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], ecx
        sub    dx, word ptr [eax + 0xf8]
        add    word ptr [ecx], dx
        jmp    loc_1_181D
loc_1_1793:
        mov    eax, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [eax + 0xf4]
        cmp    edx, 1
        jne    loc_1_17EC
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x32]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    eax, ecx
        mov    ecx, 0xe
        cmp    edx, 1
        jne    loc_1_17D2
        test   byte ptr [eax + 8], 0x80
        je     loc_1_17D2
        mov    cl, byte ptr [eax + 0xe]
        and    ecx, 0x1f
        add    ecx, 0xe
loc_1_17D2:
        cmp    edx, 7
        mov    eax, dword ptr [ebp - 0x14]
        jne    loc_1_17E6
        mov    dword ptr [eax + 0xf8], 3
        jmp    loc_1_17EC
loc_1_17E6:
        mov    dword ptr [eax + 0xf8], ecx
loc_1_17EC:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x32
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0xf8
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x18], ecx
        mov    eax, dword ptr [eax + 0xf8]
        add    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x36
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    eax, dword ptr [eax + 0xf8]
        sub    word ptr [ecx], ax
loc_1_181D:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_195D
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_3720
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 0x18]
        movsx  ecx, word ptr [ecx + 0x30]
        mov    esi, dword ptr [ebp + 0x10]
loc_1_1847:
        cmp    dword ptr [ebp - 8], ecx
        jae    loc_1_1872
        mov    eax, dword ptr [edx]
        mov    edi, eax
        and    edi, 0xffff0fff
        shr    edi, 0xc
        and    eax, 0xffff
        add    edi, eax
        movsx  ebx, word ptr [edx + 4]
        add    edx, 6
        inc    dword ptr [ebp - 8]
        cmp    ebx, esi
        jae    loc_1_1880
        sub    esi, ebx
        jmp    loc_1_1847
loc_1_1872:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        jmp    loc_1_1998
loc_1_1880:
        add    edi, esi
        sub    ebx, esi
        cmp    ebx, dword ptr [ebp + 0x14]
        jbe    loc_1_188C
        mov    ebx, dword ptr [ebp + 0x14]
loc_1_188C:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_195D
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [eax + eax*2]
        lea    esi, [ecx + eax*2 + 0x32]
loc_1_18A3:
        or     ebx, ebx
        jne    loc_1_18DE
        mov    ecx, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [ecx + 0x30]
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_1990
        mov    eax, dword ptr [esi]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        lea    edi, [ecx + eax]
        movsx  ebx, word ptr [esi + 4]
        add    esi, 6
        inc    dword ptr [ebp - 8]
        cmp    ebx, dword ptr [ebp + 0x14]
        jbe    loc_1_18DE
        mov    ebx, dword ptr [ebp + 0x14]
loc_1_18DE:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_190C
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_19D0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3744
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3768
loc_1_190C:
        cmp    ebx, dword ptr [ebp - 4]
        jae    loc_1_192C
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3508
        add    esp, 0xc
        sub    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    ebx, ebx
        jmp    loc_1_1953
loc_1_192C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3508
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 0x14]
        sub    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp + 0x14], ecx
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
loc_1_1953:
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_18A3
loc_1_195D:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    word ptr [ecx + 0x2a], -1
        jne    loc_1_1984
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xf8]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 0xf8]
        add    word ptr [ecx], ax
loc_1_1984:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  090h
loc_1_1990:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x14]
        sub    dword ptr [eax], ecx
loc_1_1998:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    word ptr [ecx + 0x2a], -1
        jne    loc_1_19BF
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xf8]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 0xf8]
        add    word ptr [ecx], ax
loc_1_19BF:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
loc_1_19D0:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x14]
        sub    dword ptr [eax], ecx
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0xc]
        cmp    dword ptr [esi + 0x20], 2
        jne    loc_1_1A10
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1A10:
        mov    dword ptr [esi + 0x20], 2
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2c]
        cmp    eax, ecx
        je     loc_1_1A2A
loc_1_1A20:
        add    ecx, dword ptr [eax + 0x18]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_1A20
loc_1_1A2A:
        or     ecx, ecx
        jne    loc_1_1AC0
        mov    dword ptr [esi + 0x38], 1
        push   esi
        call   sub_1_323D
        add    esp, 4
        inc    dword ptr [esi + 0x18]
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_1A76
        mov    ebx, 1
loc_1_1A51:
        cmp    dword ptr [edi + 0x14], ebx
        jne    loc_1_1A72
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
        mov    edi, dword ptr [edi + 0x10]
loc_1_1A72:
        or     edi, edi
        jne    loc_1_1A51
loc_1_1A76:
        mov    dword ptr [esi + 0x20], 1
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_1AAE
        mov    ebx, 1
loc_1_1A89:
        cmp    dword ptr [edi + 0x14], ebx
        jne    loc_1_1AAA
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
        mov    edi, dword ptr [edi + 0x10]
loc_1_1AAA:
        or     edi, edi
        jne    loc_1_1A89
loc_1_1AAE:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1AC0:
        mov    dword ptr [esi + 0x38], 0
        inc    dword ptr [esi + 0x18]
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_1AFB
        mov    ebx, 1
loc_1_1AD6:
        cmp    dword ptr [edi + 0x14], ebx
        jne    loc_1_1AF7
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
        mov    edi, dword ptr [edi + 0x10]
loc_1_1AF7:
        or     edi, edi
        jne    loc_1_1AD6
loc_1_1AFB:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_1B10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x20]
        cmp    eax, 2
        jne    loc_1_1B25
        mov    eax, 0xc001000d
        pop    ebp
        ret    
loc_1_1B25:
        or     eax, eax
        jne    loc_1_1B30
        mov    eax, 0xc0000001
        pop    ebp
        ret    
loc_1_1B30:
        sub    eax, eax
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B40:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x38b0], 0         ; -> obj1:0x38B0
        jne    loc_1_1B60
        mov    dword ptr [0x38ac], 0         ; -> obj1:0x38AC
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1B60:
        mov    ebx, dword ptr [0x38b0]       ; -> obj1:0x38B0
        mov    esi, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [esi + 0xc]
        or     eax, eax
        mov    dword ptr [0x38b0], eax       ; -> obj1:0x38B0
        jne    loc_1_1B81
        mov    dword ptr [0x38b4], 0         ; -> obj1:0x38B4
loc_1_1B81:
        cmp    dword ptr [edi + 0x20], 1
        jne    loc_1_1C03
        cmp    dword ptr [ebx + 0x10], 1
        je     loc_1_1B94
        mov    eax, 0xc00000bb
        jmp    loc_1_1C11
loc_1_1B94:
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1010103
        je     loc_1_1BAC
        cmp    eax, 0x2010103
        je     loc_1_1BB8
        mov    eax, 0xc00000bb
        jmp    loc_1_1C11
loc_1_1BAC:
        push   ebx
        push   esi
        call   sub_1_1EA0
        add    esp, 8
        jmp    loc_1_1C11
loc_1_1BB8:
        mov    eax, dword ptr [ebx + 0x18]
        sub    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [edi + 0x2c]
        cmp    eax, ecx
        je     loc_1_1BD3
loc_1_1BC9:
        or     ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_1BC9
loc_1_1BD3:
        mov    dword ptr [edi + 0xe8], ecx
        mov    eax, dword ptr [0x385c]       ; -> obj1:0x385C
        push   edi
        mov    word ptr [eax], 0xc0
        mov    ecx, dword ptr [0x385c]       ; -> obj1:0x385C
        mov    eax, dword ptr [edi + 0xe8]
        mov    dword ptr [ecx + 2], eax
        call   sub_1_2F77
        add    esp, 4
        push   eax
        call   0
        jmp    loc_1_1C0E
loc_1_1C03:
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_1B10
loc_1_1C0E:
        add    esp, 4
loc_1_1C11:
        mov    ecx, dword ptr [ebx + 0x14]
        cmp    ecx, 0x1010103
        je     loc_1_1C26
        cmp    ecx, 0x2010103
        je     loc_1_1C47
        jmp    loc_1_1C76
loc_1_1C26:
        mov    ecx, dword ptr [esi + 0x14]
        cmp    ecx, 1
        jne    loc_1_1C40
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_364C
        jmp    loc_1_1C76
        db  08Dh, 049h, 000h
loc_1_1C40:
        cmp    ecx, 4
        jne    loc_1_1C76
        jmp    loc_1_1C60
loc_1_1C47:
        mov    ecx, dword ptr [esi + 0x14]
        cmp    ecx, 1
        jne    loc_1_1C5B
        push   eax
        push   ebx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_364C
        jmp    loc_1_1C76
loc_1_1C5B:
        cmp    ecx, 3
        jne    loc_1_1C76
loc_1_1C60:
        push   1
        push   esi
        push   edi
        call   sub_1_1110
        add    esp, 0xc
        push   0
        push   0x2c
        push   ebx
        call   sub_1_36FC
loc_1_1C76:
        cmp    dword ptr [0x38b0], 0         ; -> obj1:0x38B0
        jne    loc_1_1C89
        mov    dword ptr [0x38ac], 0         ; -> obj1:0x38AC
loc_1_1C89:
        push   esi
        push   edi
        call   sub_1_0030
        add    esp, 8
        cmp    dword ptr [0x38b0], 0         ; -> obj1:0x38B0
        jne    loc_1_1B60
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x14], 0
        push   -1
        push   0
        push   0xe8
        push   eax
        call   sub_1_36F6
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1CE7
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1CE7:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x18], ecx
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [ecx + 8], 0
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x10], 1
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x14], 0x1010103
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [eax + 0x20], 0
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        add    esi, 0x28
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x18], esi
        cmp    dword ptr [edi + 0xa0], 0
        jbe    loc_1_1DAA
        lea    ebx, [edi + 0x80]
        lea    edx, [edi + 0xa4]
        lea    esp, [esp]
loc_1_1D50:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x1c]
        not    eax
        and    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        jne    loc_1_1D96
        mov    eax, dword ptr [edx - 0x68]
        mov    dword ptr [esi], eax
        inc    word ptr [ebp - 2]
        mov    ax, word ptr [ebx]
        mov    word ptr [esi + 4], ax
        add    esi, 6
        mov    eax, dword ptr [edi + 0xa0]
        mov    ecx, dword ptr [edi + eax*4 + 0x38]
        mov    dword ptr [edx - 0x68], ecx
        mov    cx, word ptr [edi + eax*2 + 0x7e]
        mov    word ptr [ebx], cx
        dec    eax
        mov    eax, dword ptr [edi + eax*4 + 0xa4]
        mov    dword ptr [edx], eax
        dec    dword ptr [edi + 0xa0]
loc_1_1D96:
        add    ebx, 2
        add    edx, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [edi + 0xa0], eax
        ja     loc_1_1D50
loc_1_1DAA:
        mov    ecx, dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp + 0x10], 0
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ecx + 0xa], ax
        je     loc_1_1E67
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, 6
        mov    eax, dword ptr [eax + 0x1c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [eax + 0x18]
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_1E67
loc_1_1DE8:
        push   ebx
        mov    eax, dword ptr [edi + 0xa0]
        push   eax
        lea    eax, [edi + 0x80]
        push   eax
        lea    eax, [edi + 0x3c]
        push   eax
        call   sub_1_3539
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1E16
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x1c]
        or     dword ptr [edi + eax*4 + 0xa0], ecx
        jmp    loc_1_1E59
loc_1_1E16:
        mov    eax, dword ptr [edi + 0xa0]
        cmp    eax, 0x10
        jae    loc_1_1E85
        lea    edx, [edi + eax*4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [edx + 0x3c], ecx
        lea    eax, [edi + eax*2 + 0x80]
        mov    cx, word ptr [ebx + 4]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x1c]
        add    esi, 6
        mov    dword ptr [edx + 0xa4], ecx
        inc    dword ptr [edi + 0xa0]
        mov    ecx, dword ptr [edx + 0x3c]
        mov    dword ptr [esi - 6], ecx
        mov    ax, word ptr [eax]
        mov    word ptr [esi - 2], ax
loc_1_1E59:
        add    ebx, 6
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0x14], eax
        ja     loc_1_1DE8
loc_1_1E67:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        add    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1E85:
        push   0
        push   0xe8
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_36FC
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_1_1EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [eax + 0xc]
        mov    edi, edi
loc_1_1EB0:
        cmp    word ptr [esi + 8], 0
        jne    loc_1_1F10
        mov    eax, dword ptr [esi + 0x18]
        add    eax, dword ptr [esi + 0x20]
        mov    edx, dword ptr [0x385c]       ; -> obj1:0x385C
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ax, word ptr [esi + 0xa]
        or     ax, ax
        je     loc_1_1F05
        dec    ax
        mov    word ptr [esi + 0xa], ax
        mov    ecx, dword ptr [esi + 0x18]
        add    ecx, dword ptr [esi + 0x20]
        mov    edx, dword ptr [0x385c]       ; -> obj1:0x385C
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [edx + 4], ax
        add    dword ptr [esi + 0x20], 6
        push   edi
        call   sub_1_30D4
        add    esp, 4
        jmp    loc_1_1EB0
loc_1_1F05:
        mov    word ptr [esi + 8], 1
        jmp    loc_1_1EB0
        db  08Dh, 049h, 000h
loc_1_1F10:
        mov    eax, dword ptr [esi + 0x20]
        cmp    dword ptr [esi + 0x1c], eax
        jbe    loc_1_1F41
        mov    ecx, dword ptr [esi + 0x18]
        add    ecx, eax
        mov    edx, dword ptr [0x385c]       ; -> obj1:0x385C
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [edx + 4], ax
        add    dword ptr [esi + 0x20], 6
        push   edi
        call   sub_1_2F77
        add    esp, 4
        jmp    loc_1_1EB0
loc_1_1F41:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_1_1CB0
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_1_1F78
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_1F78:
        cmp    dword ptr [0x38ac], 0         ; -> obj1:0x38AC
        jne    loc_1_1FB0
        mov    dword ptr [0x38ac], 1         ; -> obj1:0x38AC
        push   edi
        push   esi
        call   sub_1_1EA0
        add    esp, 8
        mov    esi, eax
        push   0
        push   0x2c
        push   edi
        call   sub_1_36FC
        call   sub_1_1B40
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1FB0:
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi], esi
        inc    dword ptr [esi + 0x18]
        cmp    dword ptr [0x38b4], 0         ; -> obj1:0x38B4
        je     loc_1_1FE0
        mov    eax, dword ptr [0x38b4]       ; -> obj1:0x38B4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [0x38b4], edi       ; -> obj1:0x38B4
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1FE0:
        mov    dword ptr [0x38b4], edi       ; -> obj1:0x38B4
        mov    dword ptr [0x38b0], edi       ; -> obj1:0x38B0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2000:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [edx], esi
        cmp    dword ptr [0x38ac], 0         ; -> obj1:0x38AC
        jne    loc_1_2080
        mov    dword ptr [0x38ac], 1         ; -> obj1:0x38AC
        mov    eax, dword ptr [edx + 0x18]
        sub    edx, edx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [ecx + 0x2c]
        cmp    eax, edx
        je     loc_1_203D
loc_1_2033:
        or     edx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_2033
loc_1_203D:
        mov    dword ptr [ecx + 0xe8], edx
        mov    eax, dword ptr [0x385c]       ; -> obj1:0x385C
        push   ecx
        mov    word ptr [eax], 0xc0
        mov    edx, dword ptr [0x385c]       ; -> obj1:0x385C
        mov    eax, dword ptr [ecx + 0xe8]
        mov    dword ptr [edx + 2], eax
        call   sub_1_2F77
        add    esp, 4
        push   eax
        call   0
        add    esp, 4
        mov    esi, eax
        call   sub_1_1B40
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2080:
        mov    dword ptr [edx + 4], 0
        mov    dword ptr [edx], esi
        inc    dword ptr [esi + 0x18]
        cmp    dword ptr [0x38b4], 0         ; -> obj1:0x38B4
        je     loc_1_20B0
        mov    eax, dword ptr [0x38b4]       ; -> obj1:0x38B4
        pop    esi
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [0x38b4], edx       ; -> obj1:0x38B4
        mov    eax, 0x103
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_20B0:
        mov    dword ptr [0x38b4], edx       ; -> obj1:0x38B4
        mov    dword ptr [0x38b0], edx       ; -> obj1:0x38B0
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_20D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [edi + 0x18]
        mov    ebx, dword ptr [edi + 0x14]
        mov    ecx, dword ptr [eax]
        sub    eax, eax
        cmp    ebx, 0x1010e
        je     loc_1_2120
        cmp    ebx, 0x1010f
        je     loc_1_2162
        cmp    ebx, 0x1010103
        je     loc_1_2189
        cmp    ebx, 0x2010103
        je     loc_1_2198
        mov    eax, 0xc00000bb
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_2120:
        mov    dword ptr [esi + 0x20], ecx
        mov    eax, dword ptr [esi + 0xc]
        sub    ecx, ecx
        mov    eax, dword ptr [eax + 0x2c]
        cmp    eax, ecx
        je     loc_1_213A
        nop    
loc_1_2130:
        or     ecx, dword ptr [eax + 0x20]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_2130
loc_1_213A:
        mov    dword ptr [edx + 0xe4], ecx
        or     ecx, ecx
        je     loc_1_2154
        mov    eax, dword ptr [edx + 4]
        or     cl, 1
        mov    byte ptr [eax + 3], cl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2154:
        mov    eax, dword ptr [edx + 4]
        pop    edi
        mov    byte ptr [eax + 3], 0
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2162:
        cmp    ecx, 0xd0
        jbe    loc_1_2174
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2174:
        cmp    dword ptr [edx + 0xf0], ecx
        jae    loc_1_21A2
        mov    dword ptr [edx + 0xf0], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2189:
        push   edi
        push   esi
        call   sub_1_1F50
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2198:
        push   edi
        push   esi
        call   sub_1_2000
        add    esp, 8
loc_1_21A2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_21B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        sub    edx, edx
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 0x14], edx
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    ebx, 2
        jne    loc_1_21E1
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        jmp    loc_1_21E9
loc_1_21E1:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    ecx, dword ptr [ecx + 0xc]
loc_1_21E9:
        mov    esi, dword ptr [ebp + 0x10]
        cmp    esi, 0x20101
        ja     loc_1_2212
        je     loc_1_24A0
        sub    esi, 0x10101
        cmp    esi, 0x10
        ja     loc_1_22B2
        shl    esi, 2
        jmp    loc_1_2694
loc_1_2212:
        cmp    esi, 0x1010102
        ja     loc_1_2264
        cmp    esi, 0x1010101
        jae    loc_1_2550
        cmp    esi, 0x20102
        je     loc_1_24C0
        cmp    esi, 0x20103
        je     loc_1_24E5
        cmp    esi, 0x20104
        je     loc_1_2501
        cmp    esi, 0x20105
        je     loc_1_2523
        cmp    esi, 0x2020d
        je     loc_1_2539
        jmp    loc_1_22B2
loc_1_2264:
        cmp    esi, 0x2010102
        ja     loc_1_2292
        cmp    esi, 0x2010101
        jae    loc_1_2550
        cmp    esi, 0x1010103
        je     loc_1_2560
        cmp    esi, 0x1010104
        je     loc_1_2610
        jmp    loc_1_22B2
loc_1_2292:
        cmp    esi, 0x2010103
        je     loc_1_2621
        cmp    esi, 0x6010101
        jb     loc_1_22B2
        cmp    esi, 0x6010102
        jbe    loc_1_2640
loc_1_22B2:
        mov    dword ptr [ebp - 0x10], 0x10001
        mov    esi, dword ptr [ebp - 0x1c]
        jmp    loc_1_264B
loc_1_22C1:
        mov    esi, 0x392c                   ; -> loc_1_392C
        mov    edx, 0x7c
        jmp    loc_1_264B
loc_1_22D0:
        mov    dword ptr [ebp - 0x14], 0
        lea    esi, [ebp - 0x14]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_22E4:
        mov    eax, dword ptr [ecx + 0xf4]
        mov    dword ptr [ebp - 0x18], eax
        lea    esi, [ebp - 0x18]
        mov    edx, 4
        jmp    loc_1_264B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2300:
        mov    dword ptr [ebp - 8], 0xd0
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2314:
        mov    eax, dword ptr [0x3870]       ; -> obj1:0x3870
        sub    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 8]
        movsx  edx, word ptr [eax + 0x28]
        and    ecx, 0xffff
        cmp    ecx, edx
        jle    loc_1_2340
        movsx  eax, word ptr [eax + 0x2a]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2349
        db  08Dh, 064h, 024h, 000h
loc_1_2340:
        movsx  eax, word ptr [eax + 0x2c]
        sub    eax, edx
        add    dword ptr [ebp - 8], eax
loc_1_2349:
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2356:
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 0x46]
        test   ah, 0x80
        je     loc_1_2374
        and    ax, 0x7fff
        mov    cx, 0xa
        cwd    
        idiv   cx
        movsx  eax, ax
        jmp    loc_1_2386
loc_1_2374:
        movsx  eax, ax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 4
loc_1_2386:
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2396:
        mov    eax, dword ptr [0x3870]       ; -> obj1:0x3870
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
        db  005h, 000h, 000h, 000h, 000h
loc_1_23B0:
        mov    eax, dword ptr [0x388c]       ; -> obj1:0x388C
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_23C5:
        mov    dword ptr [ebp - 8], 4
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_23E0:
        mov    dword ptr [ebp - 8], 4
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_23F4:
        mov    dword ptr [ebp - 8], 0x2a4f5741
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2408:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x2e]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        lea    esi, [ecx + eax]
        call   sub_1_3583
        lea    esi, [eax + esi + 1]
        xor    edx, edx
        mov    dl, byte ptr [esi - 1]
        jmp    loc_1_264B
loc_1_2434:
        cmp    ebx, 2
        jne    loc_1_2441
        mov    eax, dword ptr [ecx + 0xe4]
        jmp    loc_1_2444
loc_1_2441:
        mov    eax, dword ptr [eax + 0x20]
loc_1_2444:
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2454:
        mov    eax, dword ptr [ecx + 0xf0]
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2470:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edx, 2
        jmp    loc_1_264B
loc_1_2483:
        mov    eax, dword ptr [ecx + 8]
        lea    esi, [ebp - 2]
        mov    ax, word ptr [eax + 0x28]
        mov    word ptr [ebp - 2], ax
        mov    edx, 2
        jmp    loc_1_264B
        db  005h, 000h, 000h, 000h, 000h
loc_1_24A0:
        mov    eax, dword ptr [ecx + 0xc]
        lea    esi, [ebp - 8]
        mov    ecx, dword ptr [eax + 8]
        sub    ecx, dword ptr [eax + 0x14]
        sub    ecx, dword ptr [eax + 0x28]
        sub    ecx, dword ptr [eax + 0x18]
        mov    edx, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_264B
        db  090h
loc_1_24C0:
        mov    eax, dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        sub    ecx, dword ptr [eax + 0x24]
        sub    ecx, dword ptr [eax + 0x20]
        lea    esi, [ebp - 8]
        sub    ecx, dword ptr [eax + 0x34]
        sub    ecx, dword ptr [eax + 0x38]
        sub    ecx, dword ptr [eax + 0x2c]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 4
        jmp    loc_1_264B
loc_1_24E5:
        mov    eax, dword ptr [ecx + 0xc]
        lea    esi, [ebp - 8]
        mov    ecx, dword ptr [eax + 0x14]
        add    ecx, dword ptr [eax + 0x28]
        add    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2501:
        mov    eax, dword ptr [ecx + 0xc]
        lea    esi, [ebp - 8]
        mov    ecx, dword ptr [eax + 0x24]
        add    ecx, dword ptr [eax + 0x2c]
        add    ecx, dword ptr [eax + 0x20]
        add    ecx, dword ptr [eax + 0x34]
        mov    edx, 4
        add    ecx, dword ptr [eax + 0x38]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_264B
loc_1_2523:
        mov    eax, dword ptr [ecx + 0xc]
        lea    esi, [ebp - 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2539:
        mov    eax, dword ptr [ecx + 0xc]
        lea    esi, [ebp - 8]
        mov    eax, dword ptr [eax + 0x34]
        mov    dword ptr [ebp - 8], eax
        mov    edx, 4
        jmp    loc_1_264B
        db  090h
loc_1_2550:
        mov    esi, dword ptr [ecx + 8]
        add    esi, 0x1c
        mov    edx, 6
        jmp    loc_1_264B
loc_1_2560:
        sub    edi, edi
        cmp    ebx, 2
        jne    loc_1_2570
        mov    dword ptr [ebp - 0xc], 0xffffffff
        jmp    loc_1_2576
loc_1_2570:
        mov    esi, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0xc], esi
loc_1_2576:
        mov    edx, dword ptr [ecx + 0xa0]
        or     edx, edx
        je     loc_1_2594
        lea    eax, [ecx + 0xa4]
loc_1_2586:
        mov    esi, dword ptr [ebp - 0xc]
        test   dword ptr [eax], esi
        je     loc_1_258E
        inc    edi
loc_1_258E:
        add    eax, 4
        dec    edx
        jne    loc_1_2586
loc_1_2594:
        lea    eax, [edi + edi*2]
        lea    edi, [eax*2]
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [eax]
        cmp    edi, edx
        jbe    loc_1_25BA
        mov    eax, dword ptr [ebp + 0x1c]
        sub    edi, edx
        mov    dword ptr [eax], edi
        mov    eax, 0xc0010014
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_25BA:
        sub    edx, edi
        mov    esi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edx
        add    dword ptr [esi], edi
        sub    eax, eax
        cmp    dword ptr [ecx + 0xa0], eax
        jbe    loc_1_2605
        lea    ebx, [ecx + 0x80]
        mov    edi, dword ptr [ebp + 0x14]
        lea    edx, [ecx + 0xa4]
        lea    esp, [esp]
loc_1_25E0:
        mov    esi, dword ptr [ebp - 0xc]
        test   dword ptr [edx], esi
        je     loc_1_25F6
        mov    esi, dword ptr [edx - 0x68]
        mov    dword ptr [edi], esi
        mov    si, word ptr [ebx]
        mov    word ptr [edi + 4], si
        add    edi, 6
loc_1_25F6:
        add    ebx, 2
        add    edx, 4
        inc    eax
        cmp    dword ptr [ecx + 0xa0], eax
        ja     loc_1_25E0
loc_1_2605:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_2610:
        mov    dword ptr [ebp - 8], 0x10
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2621:
        cmp    ebx, 2
        jne    loc_1_2630
        mov    eax, dword ptr [ecx + 0xe8]
        jmp    loc_1_2633
        db  08Bh, 0FFh
loc_1_2630:
        mov    eax, dword ptr [eax + 0x24]
loc_1_2633:
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    loc_1_264B
loc_1_2640:
        mov    esi, dword ptr [ecx + 8]
        add    esi, 0x21
        mov    edx, 1
loc_1_264B:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_268A
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [eax]
        cmp    ecx, edx
        jae    loc_1_2672
        mov    eax, dword ptr [ebp + 0x1c]
        sub    edx, ecx
        mov    dword ptr [eax], edx
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_2672:
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [ebp + 0x20]
        add    dword ptr [esi], edx
        sub    dword ptr [eax], edx
loc_1_268A:
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_2694:
        shl    dword ptr [edx], 0
        add    al, dl                        ; -> loc_1_22D0
        and    al, byte ptr [eax]
        add    ah, ah                        ; -> loc_1_22E4
        and    al, byte ptr [eax]
        add    ah, ah                        ; -> loc_1_22E4
        and    al, byte ptr [eax]
        add    byte ptr [eax], al            ; -> loc_1_2300
        and    eax, dword ptr [eax]
        add    byte ptr [ebx], dl            ; -> loc_1_2314
        add    byte ptr [eax], al
        push   esi
        and    eax, dword ptr [eax]
        add    byte ptr [esi - 0x4fffffdd], dl ; -> loc_1_2396
        and    eax, dword ptr [eax]
        add    ch, al                        ; -> loc_1_23C5
        and    eax, dword ptr [eax]
        add    al, ah                        ; -> loc_1_23E0
        and    eax, dword ptr [eax]
        add    ah, dh                        ; -> loc_1_23F4
        and    eax, dword ptr [eax]
        add    byte ptr [eax], cl            ; -> loc_1_2408
        and    al, 0
        add    byte ptr [esp], dh            ; -> loc_1_2434
        add    byte ptr [eax], al
        push   esp
        and    al, 0
        add    byte ptr [eax + 0x24], dh     ; -> loc_1_2470
        add    byte ptr [eax], al
        and    dword ptr [eax + eax], 0
        lea    esp, [esp]
        lea    esp, [esp]
sub_1_26E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ecx + 0x20]
        push   eax
        lea    eax, [ecx + 0x24]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ecx + 0x18]
        push   eax
        mov    eax, dword ptr [ecx + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21B0
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2720:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_1_2750
        cmp    ecx, 2
        je     loc_1_2750
        cmp    ecx, 1
        jne    loc_1_2747
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_20D0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2747:
        mov    eax, 0x10001
        pop    ebp
        ret    8
loc_1_2750:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_26E0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2760:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x38bc]       ; -> obj1:0x38BC
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_361C
        push   0
        mov    eax, dword ptr [0x38a4]       ; -> obj1:0x38A4
        push   eax
        call   sub_1_3610
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        push   0
        movsx  eax, word ptr [eax]
        shl    eax, 4
        mov    ecx, dword ptr [0x38e4]       ; -> obj1:0x38E4
        movsx  eax, byte ptr [eax + ecx + 3]
        lea    eax, [eax*4 + 4]
        push   eax
        mov    eax, dword ptr [0x38e0]       ; -> obj1:0x38E0
        push   eax
        call   sub_1_36FC
        push   0
        push   0x3f
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        push   eax
        call   sub_1_36FC
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27D0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ecx, ecx
        mov    esi, dword ptr [0x38c4]       ; -> obj1:0x38C4
        cmp    esi, ecx
        je     loc_1_27F0
        mov    eax, dword ptr [ebp + 8]
loc_1_27E3:
        cmp    esi, eax
        je     loc_1_27F0
        mov    ecx, esi
        mov    esi, dword ptr [esi + 0x28]
        or     esi, esi
        jne    loc_1_27E3
loc_1_27F0:
        or     esi, esi
        je     loc_1_2818
        or     ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        je     loc_1_2800
        mov    dword ptr [ecx + 0x28], eax
        jmp    loc_1_2805
loc_1_2800:
        mov    dword ptr [0x38c4], eax       ; -> obj1:0x38C4
loc_1_2805:
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_3628
        push   0
        push   0
        push   esi
        call   sub_1_36FC
loc_1_2818:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2820:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_2B30
        add    esp, 4
        mov    esi, eax
        cmp    esi, -1
        jne    loc_1_2846
        mov    eax, 0xc0010006
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2846:
        push   -1
        push   0
        push   0x10a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_36F6
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2B20
        shl    esi, 4
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + eax + 6]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        mov    eax, dword ptr [esi + eax + 0xa]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        mov    eax, dword ptr [esi + eax + 0xe]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xc], eax
        add    esi, dword ptr [0x38e4]       ; -> obj1:0x38E4
        mov    eax, dword ptr [ebp - 4]
        add    esi, 2
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        movsx  eax, byte ptr [eax]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        movsx  eax, byte ptr [eax + 1]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 0x10]
        cmp    dword ptr [eax + 0x14], esi
        jb     loc_1_28FB
        lea    edx, [esi*4]
loc_1_28E3:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0x38e0]       ; -> obj1:0x38E0
        mov    dword ptr [edx + ecx], eax
        add    edx, 4
        mov    eax, dword ptr [ebp - 4]
        inc    esi
        cmp    dword ptr [eax + 0x14], esi
        jae    loc_1_28E3
loc_1_28FB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x38], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        movsx  eax, word ptr [eax + 0x44]
        mov    dword ptr [ecx + 0xf0], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa0], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x2c], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x30], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x34], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xec], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        test   byte ptr [eax + 0x22], 0x80
        mov    eax, dword ptr [ebp - 4]
        je     loc_1_29A0
        mov    dword ptr [eax + 0x106], 1
        jmp    loc_1_29AA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_29A0:
        mov    dword ptr [eax + 0x106], 0
loc_1_29AA:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0x1c
        add    edx, 0x100
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf8], 0xe
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xfc], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 0x3c]
        cmp    ax, 4
        je     loc_1_2A70
        cmp    ax, 0xb
        je     loc_1_2A70
        cmp    ax, 0x14
        je     loc_1_2A40
        cmp    ax, 0x17
        je     loc_1_2A40
        cmp    ax, 0xe
        jne    loc_1_2A8A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf8], 3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xfc], 1
        jmp    loc_1_2A8A
loc_1_2A40:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf8], 0xd
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xfc], 1
        jmp    loc_1_2A8A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2A70:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xfc], 2
loc_1_2A8A:
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 0xfc]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x32]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        inc    eax
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_2BF0
        add    esp, 8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2B10
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0x38bc]       ; -> obj1:0x38BC
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x24
        push   eax
        call   sub_1_3622
        or     eax, eax
        jne    loc_1_2B10
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [0x38c4]       ; -> obj1:0x38C4
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x38c4], eax       ; -> obj1:0x38C4
        push   0x80
        push   eax
        call   sub_1_3383
        add    esp, 8
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2B10:
        push   0
        push   0x10a
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_36FC
loc_1_2B20:
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_2B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    edi, edi
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], 0
loc_1_2B48:
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        mov    eax, dword ptr [edi + eax + 0xa]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    eax, dword ptr [eax + 0x32]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        inc    eax
loc_1_2B67:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x61
        jb     loc_1_2B7C
        cmp    cl, 0x7a
        ja     loc_1_2B7C
        xor    ebx, ebx
        mov    bl, cl
        sub    ebx, 0x20
        jmp    loc_1_2B80
loc_1_2B7C:
        xor    ebx, ebx
        mov    bl, cl
loc_1_2B80:
        mov    dl, byte ptr [esi]
        cmp    dl, 0x61
        jb     loc_1_2B97
        cmp    dl, 0x7a
        ja     loc_1_2B97
        and    edx, 0xff
        sub    edx, 0x20
        jmp    loc_1_2B9D
loc_1_2B97:
        and    edx, 0xff
loc_1_2B9D:
        cmp    ebx, edx
        jne    loc_1_2BA9
        or     cl, cl
        je     loc_1_2BA9
        inc    eax
        inc    esi
        jmp    loc_1_2B67
loc_1_2BA9:
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi], al
        je     loc_1_2BC2
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        inc    dword ptr [ebp - 4]
        movsx  eax, word ptr [eax]
        add    edi, 0x10
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_2B48
loc_1_2BC2:
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        movsx  eax, word ptr [eax]
        cmp    eax, dword ptr [ebp - 4]
        jb     loc_1_2BD9
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_2BD9:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BF0:
        push   ebp
        mov    ebp, esp
        sub    dx, dx
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_2BF9:
        mov    ax, dx
        inc    dx
        and    eax, 0xffff
        cmp    byte ptr [ecx + eax], 0
        jne    loc_1_2BF9
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 2], dx
        mov    word ptr [eax], dx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2C20:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2B30
        add    esp, 4
        cmp    eax, -1
        jne    loc_1_2C38
        sub    al, al
        pop    ebp
        ret    
loc_1_2C38:
        mov    al, 1
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
MSODISUP_Control:
        cmp    eax, 7
        jae    loc_1_2C4C
        jmp    dword ptr [eax*4 + 0x2c4e]    ; -> obj1:0x2C4E
loc_1_2C4C:
        clc    
        ret    
        ; ---- dispatch table (7 entries) ----
        dd  loc_2_0282
        dd  loc_2_0289
        dd  loc_2_031E
        dd  loc_1_2C4C
        dd  loc_1_2C4C
        dd  loc_1_2C4C
        dd  loc_1_34D1
MSODISUP_Service_00:
        mov    eax, 0x30a
        clc    
        ret    
MSODISUP_Service_01:
        jmp    loc_1_2C20
sub_1_2C76:
        mov    edx, eax
        shl    edx, 0xc
        shr    dx, 0xc
        ret    
sub_1_2C80:
        call   sub_1_3583
        mov    ecx, dword ptr [esp + 4]
        movzx  edx, cx
        sub    ecx, edx
        shr    ecx, 0xc
        add    ecx, edx
        add    eax, ecx
        ret    
sub_1_2C96:
        push   edx
        movzx  edx, cx
        sub    ecx, edx
        shr    ecx, 0xc
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
        pop    edx
        ret    
loc_1_2CA6:
        mov    cx, word ptr [ebp + 0x3c]
        rol    ecx, 0x10
        mov    cx, word ptr [ebp]
        call   sub_1_2C96
        push   ecx
        mov    eax, ecx
        mov    ecx, dword ptr [eax]
        call   sub_1_2C96
        push   ecx
        mov    ecx, dword ptr [eax + 4]
        call   sub_1_2C96
        push   ecx
        cld    
        call   sub_1_0630
        add    esp, 0xc
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        or     eax, eax
        je     loc_1_2CF8
        mov    eax, dword ptr [eax + 0x26]
        mov    word ptr [ebp + 4], ax
        ror    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x1c], 0
        or     word ptr [ebp + 0x2c], 0x40
        ret    
loc_1_2CF8:
        mov    word ptr [ebp + 0x1c], 0x8001
        and    word ptr [ebp + 0x2c], 0xffbf
        ret    
loc_1_2D05:
        push   edx
        call   sub_1_11C0
        pop    edx
        push   dword ptr [edx]
        xor    eax, eax
        mov    dword ptr [edx + 4], eax
        cld    
        VxDCall  VPOWERD.Service_003Eh           ; cd203e002800
        ret    
sub_1_2D1B:
        xor    ecx, ecx
        mov    edx, dword ptr [esp + 4]
        inc    ecx
        xchg   dword ptr [edx + 4], ecx
        jecxz  loc_1_2D28
        ret    
loc_1_2D28:
        push   esi
        mov    esi, 0x2d05                   ; -> loc_1_2D05
        VMMCall  VMM.Call_When_VM_Ints_Enabled   ; cd2019000100
        pop    esi
        ret    
loc_1_2D36:
        movzx  ecx, word ptr [ebp + 0x38]
        rol    ecx, 4
        movzx  eax, word ptr [ebp + 4]
        add    ecx, eax
        sub    ecx, 4
        push   ecx
        cld    
        call   sub_1_0A60
        add    esp, 4
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
sub_1_2D57:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        test   word ptr [ebx + 0x2c], 0x200
        je     loc_1_2D6D
        pop    ebx
        ret    
loc_1_2D6D:
        inc    eax
        cmp    dword ptr [0x3928], eax       ; -> obj1:0x3928
        je     loc_1_2D88
        mov    dword ptr [0x3928], eax       ; -> obj1:0x3928
        push   esi
        mov    esi, 0x11c0                   ; -> sub_1_11C0
        VMMCall  VMM.Call_When_VM_Ints_Enabled   ; cd2019000100
        pop    esi
loc_1_2D88:
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2D96:
        mov    ecx, dword ptr [0x2d8a]       ; -> obj1:0x2D8A
        jecxz  loc_1_2DBC
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [0x2d8a], eax       ; -> obj1:0x2D8A
        cmp    eax, 0
        jne    loc_1_2DB0
        mov    dword ptr [0x2d8e], eax       ; -> obj1:0x2D8E
loc_1_2DB0:
        push   ecx
        cld    
        call   sub_1_0C50
        add    esp, 4
        jmp    loc_1_2D96
loc_1_2DBC:
        mov    dword ptr [0x2d92], 0         ; -> obj1:0x2D92
        ret    
loc_1_2DC7:
        xor    edx, edx
        mov    dx, word ptr [ebp + 0x38]
        rol    edx, 4
        movzx  eax, word ptr [ebp + 4]
        add    edx, eax
        sub    edx, 4
        cmp    dword ptr [0x38a0], 0         ; -> obj1:0x38A0
        jne    loc_1_2E13
        mov    ecx, edx
        xchg   dword ptr [0x2d8e], ecx       ; -> obj1:0x2D8E
        jecxz  loc_1_2E24
        mov    dword ptr [ecx + 4], edx
loc_1_2DEF:
        mov    dword ptr [edx + 4], 0
        mov    eax, 1
        xchg   dword ptr [0x2d92], eax       ; -> obj1:0x2D92
        cmp    eax, 0
        jne    loc_1_2E1D
        mov    esi, 0x2d96                   ; -> loc_1_2D96
        VMMCall  VMM.Call_When_VM_Ints_Enabled   ; cd2019000100
        jmp    loc_1_2E1D
loc_1_2E13:
        push   edx
        cld    
        call   sub_1_0C50
        add    esp, 4
loc_1_2E1D:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
loc_1_2E24:
        mov    dword ptr [0x2d8a], edx       ; -> obj1:0x2D8A
        jmp    loc_1_2DEF
sub_1_2E2C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        jne    loc_1_2E60
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    loc_1_2ED0
loc_1_2E60:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x2e96                   ; -> loc_1_2E96
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   dword ptr [edx], 2
        je     loc_1_2E8F
        push   0x25
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_1_2E8F:
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_2E96:
        test   dword ptr [edx], 1
        jne    loc_1_2ED0
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        je     loc_1_2EC1
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x2e96                   ; -> loc_1_2E96
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
loc_1_2EC1:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
loc_1_2ED0:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        and    word ptr [ebx + 0x2c], 0xfbff
        mov    eax, dword ptr [0x3898]       ; -> obj1:0x3898
        mov    word ptr [ebx + 0x30], ax
        shr    eax, 0x10
        mov    word ptr [ebx + 0x34], ax
        mov    edi, edx
        mov    edx, dword ptr [edi + 0x10]
        mov    edx, dword ptr [edx + 0x10]
        mov    word ptr [ebx + 0x10], 0xc
        mov    ecx, dword ptr [edi + 0x14]
        mov    word ptr [ebx + 4], cx
        ror    ecx, 0x10
        mov    word ptr [ebx + 0x38], cx
        movzx  edx, word ptr [0x384c]        ; -> obj1:0x384C
        movzx  ecx, word ptr [0x384e]        ; -> obj1:0x384E
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edx, edi
        btr    dword ptr [edx], 0
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        test   dword ptr [edx], 2
        je     loc_1_2E8F
        btr    dword ptr [edx], 1
        push   edx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        ret    
sub_1_2F77:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        jne    loc_1_2FAB
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    loc_1_3024
loc_1_2FAB:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x2fea                   ; -> loc_1_2FEA
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   dword ptr [edx], 2
        je     loc_1_2FDA
        push   0x25
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_1_2FDA:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_2FEA:
        test   dword ptr [edx], 1
        jne    loc_1_3024
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        je     loc_1_3015
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x2fea                   ; -> loc_1_2FEA
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
loc_1_3015:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
loc_1_3024:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 0x2c], 0xfbff
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [edi + 0x10]
loc_1_3050:
        mov    word ptr [eax + 0x1c], si
        mov    word ptr [eax + 0x10], 2
        mov    edx, dword ptr [0x3858]       ; -> obj1:0x3858
        mov    word ptr [eax + 4], dx
        ror    edx, 0x10
        mov    word ptr [eax + 0x38], dx
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        inc    esi
        cmp    dword ptr [edi + 0x14], esi
        jns    loc_1_3050
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        btr    dword ptr [edx], 0
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        test   dword ptr [edx], 2
        je     loc_1_2FDA
        btr    dword ptr [edx], 1
        push   edx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        ret    
sub_1_30D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        jne    loc_1_3108
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    loc_1_3181
loc_1_3108:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x3147                   ; -> loc_1_3147
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   dword ptr [edx], 2
        je     loc_1_3137
        push   0x25
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_1_3137:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_3147:
        test   dword ptr [edx], 1
        jne    loc_1_3181
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        je     loc_1_3172
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x3147                   ; -> loc_1_3147
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
loc_1_3172:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
loc_1_3181:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 0x2c], 0xfbff
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [edi + 0x10]
loc_1_31AD:
        mov    word ptr [eax + 0x1c], si
        mov    word ptr [eax + 0x10], 3
        mov    edx, dword ptr [0x3858]       ; -> obj1:0x3858
        mov    word ptr [eax + 4], dx
        ror    edx, 0x10
        mov    word ptr [eax + 0x38], dx
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        inc    esi
        cmp    dword ptr [edi + 0x14], esi
        jns    loc_1_31AD
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        btr    dword ptr [edx], 0
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        test   dword ptr [edx], 2
        je     loc_1_3137
        btr    dword ptr [edx], 1
        push   edx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        ret    
        db  0CDh, 020h, 01Ah, 080h, 001h, 000h, 0CDh, 020h, 01Bh, 080h, 001h, 000h
sub_1_323D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        jne    loc_1_3271
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    loc_1_32EA
loc_1_3271:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x32b0                   ; -> loc_1_32B0
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   dword ptr [edx], 2
        je     loc_1_32A0
        push   0x25
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_1_32A0:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_32B0:
        test   dword ptr [edx], 1
        jne    loc_1_32EA
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        je     loc_1_32DB
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x32b0                   ; -> loc_1_32B0
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
loc_1_32DB:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
loc_1_32EA:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 0x2c], 0xfbff
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [edi + 0x10]
        mov    word ptr [eax + 0x1c], si
        mov    word ptr [eax + 0x10], 6
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        btr    dword ptr [edx], 0
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        test   dword ptr [edx], 2
        je     loc_1_32A0
        btr    dword ptr [edx], 1
        push   edx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        ret    
sub_1_3383:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        jne    loc_1_33B7
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    loc_1_3430
loc_1_33B7:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x33f6                   ; -> loc_1_33F6
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   dword ptr [edx], 2
        je     loc_1_33E6
        push   0x25
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_1_33E6:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_33F6:
        test   dword ptr [edx], 1
        jne    loc_1_3430
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        cmp    eax, edi
        je     loc_1_3421
        mov    ebx, edi
        mov    eax, 0x10
        mov    ecx, 0x101
        mov    esi, 0x33f6                   ; -> loc_1_33F6
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
loc_1_3421:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
loc_1_3430:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 0x2c], 0xfbff
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [edi + 0x10]
        mov    word ptr [eax + 0x1c], si
        mov    word ptr [eax + 0x10], 9
        mov    cx, word ptr [ebx + 0x14]
        mov    word ptr [eax + 0x18], cx
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        btr    dword ptr [edx], 0
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        test   dword ptr [edx], 2
        je     loc_1_33E6
        btr    dword ptr [edx], 1
        push   edx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        ret    
loc_1_34D1:
        pushfd 
        cli    
        mov    eax, dword ptr [0x3838]       ; -> obj1:0x3838
        mov    ecx, dword ptr [eax + 0x28]
        call   sub_1_2C96
        mov    eax, ecx
        movzx  ecx, word ptr [ecx]
        inc    ecx
        add    eax, 2
loc_1_34E9:
        mov    byte ptr [eax + 3], 0
        mov    byte ptr [eax + 2], 0
        add    eax, 0x10
        loop   loc_1_34E9
        mov    eax, dword ptr [0x383c]       ; -> obj1:0x383C
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [0x3840]       ; -> obj1:0x3840
        mov    dword ptr [ecx + 0x14], eax
        popfd  
        ret    
sub_1_3508:
        cld    
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ecx, 0xc
        jb     loc_1_3534
        mov    eax, edi
        neg    eax
        and    eax, 3
        xchg   ecx, eax
        sub    eax, ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
loc_1_3534:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    
sub_1_3539:
        push   ebx
        push   esi
        push   edi
        cld    
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [edi + edx*4], eax
        xor    ecx, ecx
        dec    ecx
loc_1_3551:
        repne scasd eax, dword ptr es:[edi]
        mov    ebx, ecx
        inc    ebx
        neg    ebx
        cmp    ebx, edx
        ja     loc_1_3579
        mov    edx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x1c]
        mov    si, word ptr [esi + 4]
        cmp    si, word ptr [edx + ebx*2 - 2]
        mov    edx, dword ptr [esp + 0x18]
        jne    loc_1_3551
        mov    eax, ebx
loc_1_3575:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3579:
        xor    eax, eax
        jmp    loc_1_3575
        db  0CDh, 020h, 0A8h, 080h, 001h, 000h
sub_1_3583:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 4]
        pop    ebx
        ret    
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h
        db  024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh
        db  074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h
        db  00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch
        db  08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_360A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_3610:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_3616:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_361C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_3622:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_3628:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_3634:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_363A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_364C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_3652:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_365E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_3664:
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
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_36F6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_36FC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_3720:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_3744:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_3768:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
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
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
        dd  MSODISUP_Service_00      ; fixup
        dd  MSODISUP_Service_01      ; fixup
        db  000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00497h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'MSODISUP'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  MSODISUP_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x37DC              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ah, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h
        db  001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h
        db  001h, 000h, 000h, 0C0h, 0BBh, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h
        db  001h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_392C:
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
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [esi], eax
        add    al, byte ptr [ecx]
        add    dword ptr [esi], eax
loc_1_39A8:
        dec    ebp
        push   ebx
        dec    edi
        inc    esp
        dec    ecx
        push   ebx
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x4d8  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x3838]       ; -> obj1:0x3838
        mov    eax, dword ptr [eax + 0x28]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    dword ptr [0x38e4], eax       ; -> obj1:0x38E4
        push   0
        push   0
        push   0
        push   0x38a4                        ; -> obj1:0x38A4
        call   sub_1_360A
        push   -1
        push   0
        push   0x3f
        push   0x38b8                        ; -> obj1:0x38B8
        call   sub_1_36F6
        cmp    dword ptr [0x38b8], 0         ; -> obj1:0x38B8
        jne    loc_2_0050
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_2_0050:
        push   -1
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        push   0
        movsx  ecx, word ptr [eax]
        shl    ecx, 4
        movsx  eax, byte ptr [eax + ecx + 3]
        lea    eax, [eax*4 + 4]
        push   eax
        push   0x38e0                        ; -> obj1:0x38E0
        call   sub_1_36F6
        cmp    dword ptr [0x38e0], 0         ; -> obj1:0x38E0
        je     loc_2_016B
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 6], 0xda0    ; -> loc_1_0DA0
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0xa], 0xfb0  ; -> loc_1_0FB0
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0xe], 0x1540 ; -> loc_1_1540
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0x12], 0x16c0 ; -> loc_1_16C0
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0x16], 0x19f0 ; -> loc_1_19F0
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0x1a], 0x2720 ; -> loc_1_2720
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0x1e], 0x2720 ; -> loc_1_2720
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        push   0x39a8                        ; -> loc_1_39A8
        mov    dword ptr [eax + 0x22], 0x2760 ; -> loc_1_2760
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0x26], 0x2820 ; -> loc_1_2820
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        mov    dword ptr [eax + 0x2a], 0x27d0 ; -> loc_1_27D0
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        add    eax, 0x2e
        push   eax
        call   sub_1_2BF0
        add    esp, 8
        push   0x3f
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        push   eax
        push   0
        mov    eax, dword ptr [0x38a4]       ; -> obj1:0x38A4
        push   eax
        lea    eax, [ebp - 4]
        push   0x38bc                        ; -> obj1:0x38BC
        push   eax
        call   sub_1_3616
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0183
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        push   0
        movsx  ecx, word ptr [eax]
        shl    ecx, 4
        movsx  eax, byte ptr [eax + ecx + 3]
        lea    eax, [eax*4 + 4]
        push   eax
        mov    eax, dword ptr [0x38e0]       ; -> obj1:0x38E0
        push   eax
        call   sub_1_36FC
loc_2_016B:
        push   0
        push   0x3f
        mov    eax, dword ptr [0x38b8]       ; -> obj1:0x38B8
        push   eax
        call   sub_1_36FC
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0183:
        mov    dword ptr [0x38c4], 0         ; -> obj1:0x38C4
        mov    dword ptr [0x38d8], 0         ; -> obj1:0x38D8
        mov    dword ptr [0x38dc], 0         ; -> obj1:0x38DC
        mov    dword ptr [0x38d4], 0         ; -> obj1:0x38D4
        mov    dword ptr [0x38d0], 0         ; -> obj1:0x38D0
        mov    dword ptr [0x38ac], 0         ; -> obj1:0x38AC
        mov    dword ptr [0x38b4], 0         ; -> obj1:0x38B4
        mov    dword ptr [0x38b0], 0         ; -> obj1:0x38B0
        call   sub_2_01E0
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_2_01E0:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        sub    edi, edi
loc_2_01E7:
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        mov    eax, dword ptr [edi + eax + 0xa]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        movsx  ecx, word ptr [eax + 0x28]
        mov    edx, dword ptr [0x3870]       ; -> obj1:0x3870
        sub    edx, 4
        cmp    ecx, edx
        jbe    loc_2_023B
        mov    edx, 0x8000
        nop    
loc_2_0210:
        cmp    dword ptr [0x3870], edx       ; -> obj1:0x3870
        jae    loc_2_023B
        shl    dword ptr [0x3870], 1         ; -> obj1:0x3870
        mov    ecx, dword ptr [0x3870]       ; -> obj1:0x3870
        mov    dword ptr [0x388c], ecx       ; -> obj1:0x388C
        movsx  ecx, word ptr [eax + 0x28]
        mov    ebx, dword ptr [0x3870]       ; -> obj1:0x3870
        sub    ebx, 4
        cmp    ecx, ebx
        ja     loc_2_0210
loc_2_023B:
        cmp    dword ptr [0x388c], 0x4000    ; -> obj1:0x388C
        jae    loc_2_0251
        mov    dword ptr [0x388c], 0x4000    ; -> obj1:0x388C
loc_2_0251:
        add    edi, 0x10
        mov    eax, dword ptr [0x38e4]       ; -> obj1:0x38E4
        inc    esi
        movsx  eax, word ptr [eax]
        cmp    eax, esi
        jae    loc_2_01E7
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0270:
        sub    eax, eax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0280:
        stc    
        ret    
loc_2_0282:
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        ret    
loc_2_0289:
        mov    ecx, edx
        call   sub_1_2C96
        mov    dword ptr [0x3838], ecx       ; -> obj1:0x3838
        mov    edx, ecx
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [0x383c], eax       ; -> obj1:0x383C
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [0x3840], eax       ; -> obj1:0x3840
        mov    esi, 0x2d36                   ; -> loc_1_2D36
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [0x3848], eax       ; -> obj1:0x3848
        mov    esi, 0x2dc7                   ; -> loc_1_2DC7
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [0x3844], eax       ; -> obj1:0x3844
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [0x384c], eax       ; -> obj1:0x384C
        mov    ecx, dword ptr [edx + 0x24]
        call   sub_1_2C96
        mov    dword ptr [0x3854], ecx       ; -> obj1:0x3854
        cld    
        call   0
        cmp    eax, 0xc0000001
        je     loc_2_0280
        mov    ecx, dword ptr [0x3838]       ; -> obj1:0x3838
        call   sub_2_0340
        or     eax, eax
        je     loc_2_0280
        mov    ecx, dword ptr [0x3838]       ; -> obj1:0x3838
        mov    esi, 0x2ca6                   ; -> loc_1_2CA6
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [ecx + 0x1c], eax
        clc    
        ret    
loc_2_031E:
        push   ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        jb     loc_2_032D
        mov    byte ptr [0x37e4], bl         ; -> obj1:0x37E4
loc_2_032D:
        pop    ebx
        call   sub_2_0270
        cmp    eax, 0xc0000001
        je     loc_2_0280
        clc    
        ret    
sub_2_0340:
        push   esi
        push   edi
        push   ebx
        push   edx
        mov    eax, dword ptr [0x3870]       ; -> obj1:0x3870
        dec    eax
        mov    dword ptr [0x386c], eax       ; -> obj1:0x386C
        inc    eax
        mov    dword ptr [0x3860], eax       ; -> obj1:0x3860
        sub    dword ptr [0x3860], 4         ; -> obj1:0x3860
        push   0x208
        push   eax
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_04D3
        mov    dword ptr [0x3874], eax       ; -> obj1:0x3874
        mov    eax, dword ptr [0x388c]       ; -> obj1:0x388C
        dec    eax
        mov    dword ptr [0x3888], eax       ; -> obj1:0x3888
        inc    eax
        mov    dword ptr [0x387c], eax       ; -> obj1:0x387C
        sub    dword ptr [0x387c], 4         ; -> obj1:0x387C
        push   0x208
        push   eax
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_04D3
        mov    dword ptr [0x3890], eax       ; -> obj1:0x3890
        mov    eax, 0x1000
        push   0x208
        push   eax
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_04D3
        mov    dword ptr [0x3878], eax       ; -> obj1:0x3878
        xor    esi, esi
        mov    ecx, 0x10
loc_2_03D4:
        mov    ebx, eax
        mov    word ptr [ebx + 0x3e], si
        mov    edx, dword ptr [0x3844]       ; -> obj1:0x3844
        mov    dword ptr [ebx + 0xe], edx
        xor    edx, edx
        dec    edx
        mov    word ptr [ebx + 0x12], dx
        add    eax, 4
        call   sub_1_2C76
        mov    dword ptr [ebx + 0x26], edx
        mov    edx, dword ptr [0x3874]       ; -> obj1:0x3874
        shr    edx, 4
        mov    word ptr [ebx + 0x34], dx
        mov    word ptr [ebx + 0x3a], dx
        add    eax, 0x3c
        inc    esi
        mov    dword ptr [ebx + 4], esi
        loop   loc_2_03D4
        mov    dword ptr [eax - 0x3c], 0xffffffff
        mov    dword ptr [0x3894], eax       ; -> obj1:0x3894
        xor    esi, esi
        mov    ecx, 0x10
loc_2_0422:
        mov    ebx, eax
        mov    word ptr [ebx + 0x3e], si
        mov    edx, dword ptr [0x3848]       ; -> obj1:0x3848
        mov    dword ptr [ebx + 0xe], edx
        add    eax, 4
        call   sub_1_2C76
        mov    dword ptr [ebx + 0x26], edx
        mov    edx, dword ptr [0x3890]       ; -> obj1:0x3890
        shr    edx, 4
        mov    word ptr [ebx + 0x34], dx
        mov    word ptr [ebx + 0x3a], dx
        add    eax, 0x3c
        inc    esi
        mov    dword ptr [ebx + 4], esi
        loop   loc_2_0422
        mov    dword ptr [eax - 0x3c], 0xffffffff
        xor    esi, esi
        mov    ecx, 0x10
        mov    edx, eax
        shl    edx, 0xc
        shr    dx, 0xc
        mov    ebx, dword ptr [0x3838]       ; -> obj1:0x3838
        mov    dword ptr [ebx + 0x3e], edx
        mov    ebx, dword ptr [ebx + 0x2e]
        add    edx, 0x38
loc_2_047C:
        mov    word ptr [eax + 0x3e], si
        mov    dword ptr [eax + 0xe], ebx
        mov    word ptr [eax + 0x30], 1
        mov    dword ptr [eax + 0x32], edx
        mov    word ptr [eax + 0x36], 4
        inc    esi
        mov    dword ptr [eax + 4], esi
        add    eax, 0x40
        add    edx, 0x40
        loop   loc_2_047C
        mov    dword ptr [eax - 0x3c], 0xffffffff
        mov    edi, eax
        add    eax, 0x100
        shl    edi, 0xc
        mov    di, 0x100
        mov    dword ptr [0x3898], edi       ; -> obj1:0x3898
        mov    dword ptr [0x385c], eax       ; -> obj1:0x385C
        call   sub_1_2C76
        mov    dword ptr [0x3858], edx       ; -> obj1:0x3858
        mov    eax, 1
loc_2_04CE:
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_2_04D3:
        xor    eax, eax
        jmp    loc_2_04CE
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x122  R-X
        db  04Fh, 044h, 049h, 024h, 048h, 04Ch, 050h, 024h, 000h, 000h, 000h, 000h, 000h, 045h, 052h, 052h
        db  04Fh, 052h, 03Ah, 020h, 054h, 072h, 069h, 065h, 064h, 020h, 074h, 06Fh, 020h, 06Ch, 06Fh, 061h
        db  064h, 020h, 04Dh, 053h, 04Fh, 044h, 049h, 053h, 055h, 050h, 020h, 074h, 077h, 069h, 063h, 065h
        db  02Eh, 00Dh, 00Ah, 024h, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 04Dh, 053h, 04Fh, 044h, 049h
        db  053h, 055h, 050h, 020h, 066h, 061h, 069h, 06Ch, 065h, 064h, 020h, 074h, 06Fh, 020h, 06Ch, 06Fh
        db  061h, 064h, 02Eh, 00Dh, 00Ah, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 04Fh, 044h, 049h, 048h
        db  04Ch, 050h, 02Eh, 045h, 058h, 045h, 020h, 06Dh, 075h, 073h, 074h, 020h, 062h, 065h, 020h, 06Ch
        db  06Fh, 061h, 064h, 065h, 064h, 020h, 062h, 065h, 066h, 06Fh, 072h, 065h, 020h, 073h, 074h, 061h
        db  072h, 074h, 069h, 06Eh, 067h, 020h, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h, 02Eh, 00Dh, 00Ah
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 04Fh, 044h, 049h, 048h, 04Ch, 050h, 02Eh, 045h, 058h
        db  045h, 020h, 073h, 068h, 06Fh, 075h, 06Ch, 064h, 020h, 062h, 065h, 020h, 06Ch, 06Fh, 061h, 064h
        db  065h, 064h, 020h, 069h, 06Eh, 020h, 041h, 055h, 054h, 04Fh, 045h, 058h, 045h, 043h, 02Eh, 042h
        db  041h, 054h, 020h, 061h, 066h, 074h, 065h, 072h, 020h, 04Ch, 053h, 04Ch, 02Eh, 043h, 04Fh, 04Dh
        db  00Dh, 00Ah, 024h, 0F7h, 0C3h, 003h, 000h, 075h, 03Ch, 02Bh, 0C0h, 08Eh, 0C0h, 026h, 08Bh, 00Eh
        db  0BCh, 000h, 026h, 08Bh, 016h, 0BEh, 000h, 08Bh, 0C1h, 00Bh, 0C2h, 074h, 02Ah, 0B8h, 000h, 051h
        db  0CDh, 02Fh, 03Ch, 0FFh, 08Bh, 0FEh, 0BEh, 000h, 000h, 0B9h, 004h, 000h, 0F3h, 0A7h, 075h, 017h
        db  089h, 01Eh, 009h, 000h, 089h, 016h, 00Bh, 000h, 066h, 08Bh, 016h, 009h, 000h, 033h, 0DBh, 033h
        db  0F6h, 0B8h, 000h, 000h, 0C3h, 0EBh, 000h, 0B8h, 001h, 080h, 033h, 0DBh, 033h, 0F6h, 066h, 033h
        db  0D2h, 0C3h
VxD__LTEXT ENDS
