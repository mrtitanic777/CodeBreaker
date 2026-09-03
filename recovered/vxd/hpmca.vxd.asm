; HPMCA -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 6958 instructions recovered by recursive descent

Declare_Virtual_Device HPMCA, 3, 10, HPMCA_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x110a  R-X
        db  052h, 00Fh, 0B7h, 054h, 024h, 008h, 033h, 0C0h, 0ECh, 05Ah, 0C3h, 090h
sub_1_000C:
        push   edx
        movzx  edx, word ptr [esp + 8]
        mov    al, byte ptr [esp + 0xc]
        out    dx, al
        in     al, 0x61
        pop    edx
        ret    
        db  090h
sub_1_001C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edx
        push   ecx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        movzx  edx, word ptr [edi + 2]
        mov    word ptr [edi + 0x34], 0
        mov    ax, cx
        and    al, 0xfe
        mov    word ptr [edi + 0x36], ax
        test   cx, 1
        je     loc_1_0053
        add    esi, ecx
        mov    ah, 0xff
        mov    al, byte ptr [esi - 1]
        sub    esi, ecx
        mov    word ptr [edi + 0x34], ax
loc_1_0053:
        test   byte ptr [edi + 8], 2
        je     loc_1_0074
        xor    dl, 8
        movzx  eax, byte ptr [edi + 0x3b]
        xchg   al, ah
        out    dx, ax
        xor    dl, 4
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        xor    eax, eax
        jmp    loc_1_010D
loc_1_0074:
        xor    dl, 0x1a
        mov    al, 4
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 2
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 8
        push   0xa
        push   edx
        call   sub_1_000C
        add    esp, 8
        push   ecx
        mov    ecx, 0x40
        xor    dl, 8
loc_1_00A7:
        in     al, dx
        cmp    al, 4
        loope  loc_1_00A7
        pop    ecx
        xor    eax, eax
        xor    dl, 0
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        movzx  eax, byte ptr [edi + 0x3b]
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    ax, cx
        and    al, 0xfe
        xor    dl, 3
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0xb
        push   0x12
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0x1c
        cld    
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        call   sub_1_07C3
        jne    loc_1_0113
loc_1_010D:
        pop    edi
        pop    ecx
        pop    edx
        pop    esi
        leave  
        ret    
loc_1_0113:
        mov    word ptr [edi + 0x34], 0
        mov    word ptr [edi + 0x36], 0
        jmp    loc_1_010D
        db  02Eh, 08Bh, 0C0h
sub_1_0124:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [edi + 2]
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0x10]
        or     ecx, eax
        je     loc_1_0246
        movzx  ebx, word ptr [edi + 0x34]
        or     ebx, ebx
        je     loc_1_014D
        dec    ecx
        inc    eax
loc_1_014D:
        and    ax, 0xfffe
        je     loc_1_0250
        add    word ptr [edi + 0x36], ax
        test   byte ptr [edi + 8], 2
        je     loc_1_01E4
        test   byte ptr [edi + 8], 4
        je     loc_1_01B3
        push   edi
        mov    edi, dword ptr [edi + 0x20]
        xor    dl, 4
        in     ax, dx
        and    ax, 0xefff
        out    dx, ax
        push   eax
        or     ebx, ebx
        je     loc_1_0187
        mov    bh, byte ptr [esi]
        inc    esi
        mov    word ptr [edi], bx
        xor    ebx, ebx
loc_1_0187:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_0192
        cld    
        nop    
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_0192:
        test   al, 2
        je     loc_1_0198
        movsw  word ptr es:[edi], word ptr [esi]
loc_1_0198:
        test   al, 1
        je     loc_1_01A0
        mov    bh, 0xff
        mov    bl, byte ptr [esi]
loc_1_01A0:
        pop    eax
        or     ax, 0x1000
        out    dx, ax
        pop    edi
        mov    word ptr [edi + 0x34], bx
        xor    eax, eax
        jmp    loc_1_0246
loc_1_01B3:
        xor    dl, 0xc
        or     ebx, ebx
        je     loc_1_01C3
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_01C3:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_01CE
        cld    
        nop    
        rep outsd dx, dword ptr [esi]
loc_1_01CE:
        test   al, 2
        je     loc_1_01D4
        outsw  dx, word ptr [esi]
loc_1_01D4:
        test   al, 1
        je     loc_1_01DC
        mov    bh, 0xff
        mov    bl, byte ptr [esi]
loc_1_01DC:
        mov    word ptr [edi + 0x34], bx
        xor    eax, eax
        jmp    loc_1_0246
loc_1_01E4:
        xor    dl, 0x1a
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0xb
        push   0x12
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0x1c
        or     ebx, ebx
        je     loc_1_021E
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_021E:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_022D
        cld    
        mov    eax, eax
        shl    ecx, 1
        rep outsw dx, word ptr [esi]
loc_1_022D:
        test   al, 2
        je     loc_1_0233
        outsw  dx, word ptr [esi]
loc_1_0233:
        test   al, 1
        je     loc_1_023B
        mov    bh, 0xff
        mov    bl, byte ptr [esi]
loc_1_023B:
        mov    word ptr [edi + 0x34], bx
        call   sub_1_07C3
        jne    loc_1_025C
loc_1_0246:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave  
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0250:
        mov    ah, 0xff
        mov    al, byte ptr [esi]
        mov    word ptr [edi + 0x34], ax
        xor    eax, eax
        jmp    loc_1_0246
loc_1_025C:
        mov    word ptr [edi + 0x34], 0
        mov    word ptr [edi + 0x36], 0
        jmp    loc_1_0246
        db  08Bh, 0C0h
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ecx
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ebx, ebx
        movzx  ecx, word ptr [edi + 0x36]
        or     ecx, ecx
        je     loc_1_031D
        xchg   word ptr [edi + 0x34], bx
        or     ebx, ebx
        je     loc_1_0290
        inc    ecx
        inc    eax
loc_1_0290:
        cmp    ecx, 0x3c
        jge    loc_1_029F
        sub    ecx, 0x3c
        sub    eax, ecx
        mov    ecx, 0x3c
loc_1_029F:
        or     eax, eax
        je     loc_1_031D
        inc    eax
        and    al, 0xfe
        mov    word ptr [edi + 0x36], cx
        movzx  ecx, ax
        movzx  edx, word ptr [edi + 2]
        test   byte ptr [edi + 8], 2
        je     loc_1_02D1
        xor    dl, 0xc
        shr    ecx, 1
        or     ebx, ebx
        je     loc_1_02C9
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_02C9
        jmp    loc_1_031D
loc_1_02C9:
        xor    eax, eax
loc_1_02CB:
        out    dx, ax
        loop   loc_1_02CB
        jmp    loc_1_031D
loc_1_02D1:
        xor    dl, 0x1a
        movzx  eax, cx
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0xb
        push   0x12
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0x1c
        shr    ecx, 1
        or     ebx, ebx
        je     loc_1_0310
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_0310
        jmp    loc_1_0316
loc_1_0310:
        xor    eax, eax
loc_1_0312:
        out    dx, ax
        loop   loc_1_0312
loc_1_0316:
        call   sub_1_07C3
        jne    loc_1_031F
loc_1_031D:
        xor    eax, eax
loc_1_031F:
        pop    ebx
        pop    ecx
        pop    edx
        pop    edi
        leave  
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_0328:
        push   edx
        movzx  edx, word ptr [esp + 8]
        xor    dl, 0x10
        push   0x62
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        in     al, dx
        xchg   ah, al
        xor    dl, 7
        push   0x22
        push   edx
        call   sub_1_000C
        add    esp, 8
        movzx  eax, ah
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_0358:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 0x24]
        xchg   al, ah
        mov    ebx, 0
        test   byte ptr [esi + 8], 2
        je     loc_1_0397
        xor    dl, 0xa
        out    dx, ax
        xor    dl, 6
        in     eax, dx
        mov    dword ptr [esi + 0x2c], eax
        shr    cx, 1
        rep insw word ptr es:[edi], dx
        jae    loc_1_0407
        in     ax, dx
        mov    byte ptr [edi], al
        jmp    loc_1_0407
loc_1_0397:
        xor    dl, 0x18
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   ah, al
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 3
        mov    ax, cx
        add    ax, 4
        inc    ax
        and    al, 0xfe
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0xb
        push   0xa
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0x1c
        in     eax, dx
        mov    dword ptr [esi + 0x2c], eax
        cld    
        shr    cx, 1
        rep insw word ptr es:[edi], dx
        jae    loc_1_03FC
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_03FC:
        call   sub_1_07C3
        jne    loc_1_05BE
loc_1_0407:
        movzx  eax, word ptr [esi + 0x2c]
        test   al, 0x48
        jne    loc_1_05F8
        test   al, 1
        je     loc_1_04D2
loc_1_041B:
        shr    ax, 8
        mov    ecx, dword ptr [esi + 0x24]
        sub    ax, cx
        dec    ax
        jns    loc_1_0436
        mov    al, byte ptr [esi + 0xf]
        sub    al, cl
        add    al, byte ptr [esi + 0x2d]
        sub    al, byte ptr [esi + 0xe]
        dec    al
loc_1_0436:
        shl    ax, 8
        mov    al, byte ptr [esi + 0x2e]
        cmp    al, 0xfc
        jbe    loc_1_0443
        dec    ah
loc_1_0443:
        sub    ax, 4
        mov    word ptr [esi + 0x2e], ax
        or     ebx, ebx
        jne    loc_1_047C
        cmp    ax, 0x5ea
        ja     loc_1_0516
        cmp    ax, 0x3c
        jb     loc_1_0516
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi]
        movzx  edx, word ptr [edi + 4]
        mov    edi, dword ptr [esi + 0x10]
        test   eax, 1
        jne    loc_1_0535
        jmp    loc_1_048C
loc_1_047C:
        movzx  eax, word ptr [esi + 0x2e]
loc_1_0480:
        mov    edi, dword ptr [ebp + 0x14]
        mov    dword ptr [edi], ebx
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave  
        ret    
loc_1_048C:
        cmp    eax, dword ptr [edi]
        jne    loc_1_04BA
        cmp    dx, word ptr [edi + 4]
        jne    loc_1_04BA
        test   dword ptr [esi + 0xc2], 1
        jne    loc_1_047C
        mov    ebx, 0x100
        test   dword ptr [esi + 0xc2], 0x10
        jne    loc_1_047C
        mov    eax, 0
        jmp    loc_1_0480
loc_1_04BA:
        mov    ebx, 0x104
        test   dword ptr [esi + 0xc2], 0x10
        jne    loc_1_047C
        mov    eax, 0
        jmp    loc_1_0480
loc_1_04D2:
        mov    ebx, 0x206
        and    eax, 6
        cmp    eax, 6
        je     loc_1_04FC
        mov    ebx, 0x202
        test   eax, 2
        jne    loc_1_04FC
        mov    ebx, 0x204
        test   eax, 4
        jne    loc_1_04FC
        mov    ebx, 0x201
loc_1_04FC:
        test   dword ptr [esi + 0xc2], 0x20
        jne    loc_1_041B
        mov    eax, 0
        jmp    loc_1_0480
loc_1_0516:
        mov    ebx, 0x208
        test   dword ptr [esi + 0xc2], 0x20
        jne    loc_1_047C
        mov    eax, 0
        jmp    loc_1_0480
loc_1_0535:
        cmp    eax, -1
        jne    loc_1_0574
        cmp    dx, -1
        jne    loc_1_0574
        mov    ebx, 1
        test   dword ptr [esi + 0xc2], 2
        jne    loc_1_047C
        mov    ebx, 0x101
        test   dword ptr [esi + 0xc2], 0x10
        jne    loc_1_047C
        mov    eax, 0
        jmp    loc_1_0480
loc_1_0574:
        mov    ecx, dword ptr [esi + 0x14]
        jecxz  loc_1_059B
loc_1_0579:
        add    edi, 6
        cmp    eax, dword ptr [edi]
        jne    loc_1_05BA
        cmp    dx, word ptr [edi + 4]
        jne    loc_1_05BA
        mov    ebx, 2
        test   dword ptr [esi + 0xc2], 4
        jne    loc_1_047C
loc_1_059B:
        mov    ebx, 0x102
        test   dword ptr [esi + 0xc2], 0x18
        jne    loc_1_047C
        mov    eax, 0
        jmp    loc_1_0480
loc_1_05BA:
        loop   loc_1_0579
        jmp    loc_1_059B
loc_1_05BE:
        inc    dword ptr [esi + 0x1e2]
        and    dl, 0xe0
        push   edx
        call   sub_1_0328
        add    esp, 4
        movzx  eax, al
        mov    dword ptr [esi + 0x24], eax
        dec    eax
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_1_05E1
        movzx  eax, byte ptr [esi + 0xf]
        dec    eax
loc_1_05E1:
        xor    dl, 0x13
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    eax, 0xfffffffe
        jmp    loc_1_0480
loc_1_05F8:
        inc    dword ptr [esi + 0x1de]
        and    dl, 0xe0
        xor    dl, 0x1a
        push   0xff
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 1
        push   0xff
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    al, byte ptr [esi + 0xe]
        xor    dl, 8
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 3
        push   0x61
        push   edx
        call   sub_1_000C
        add    esp, 8
        movzx  eax, byte ptr [esi + 0xe]
        inc    al
        mov    dword ptr [esi + 0x24], eax
        xor    dl, 7
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        push   0x22
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        push   5
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        in     al, dx
        and    al, 0xc2
        mov    eax, 0xfffffffd
        cmp    al, 2
        je     loc_1_0480
        mov    eax, 0xffffffff
        jmp    loc_1_0480
sub_1_0690:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x24]
        xchg   al, ah
        add    eax, ebx
        add    eax, 4
        and    al, 0xfe
        mov    ecx, eax
        shr    eax, 8
        movzx  edx, word ptr [esi + 0xc0]
        cmp    eax, edx
        jl     loc_1_06C8
        shl    edx, 8
        sub    ecx, edx
        movzx  eax, byte ptr [esi + 0xe]
        shl    eax, 8
        add    ecx, eax
loc_1_06C8:
        mov    eax, ecx
        movzx  edx, word ptr [esi + 2]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        test   byte ptr [esi + 8], 2
        je     loc_1_06E1
        xor    dl, 0xa
        out    dx, ax
        jmp    loc_1_0740
loc_1_06E1:
        xor    dl, 0x18
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   ah, al
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    eax, ecx
        test   bl, 1
        je     loc_1_070F
        inc    ax
        test   cl, 1
        jne    loc_1_0716
        inc    ax
        jmp    loc_1_0716
loc_1_070F:
        test   cl, 1
        je     loc_1_0716
        inc    ax
loc_1_0716:
        xor    dl, 3
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0xb
        push   0xa
        push   edx
        call   sub_1_000C
        add    esp, 8
loc_1_0740:
        and    dl, 0xe0
        test   ebx, 1
        je     loc_1_0758
        dec    cx
        xor    dl, 0xc
        in     ax, dx
        mov    byte ptr [edi], ah
        inc    edi
        and    dl, 0xe0
loc_1_0758:
        push   ecx
        test   byte ptr [esi + 8], 2
        je     loc_1_0794
        test   byte ptr [esi + 8], 4
        je     loc_1_0786
        push   esi
        mov    esi, dword ptr [esi + 0x20]
        xor    dl, 4
        in     ax, dx
        and    ax, 0xefff
        out    dx, ax
        cld    
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        or     ax, 0x1000
        out    dx, ax
        pop    esi
        xor    dl, 8
        jmp    loc_1_079F
loc_1_0786:
        xor    dl, 0xc
        cld    
        shr    ecx, 2
        mov    eax, eax
        rep insd dword ptr es:[edi], dx
        jmp    loc_1_079F
loc_1_0794:
        xor    dl, 0xc
        cld    
        shr    ecx, 1
        mov    eax, eax
        rep insw word ptr es:[edi], dx
loc_1_079F:
        pop    ecx
        test   cx, 1
        je     loc_1_07AB
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_07AB:
        test   byte ptr [esi + 8], 2
        jne    loc_1_07BA
        push   edx
        call   sub_1_07C3
        pop    edx
        jne    loc_1_07BC
loc_1_07BA:
        xor    eax, eax
loc_1_07BC:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave  
        ret    
sub_1_07C3:
        push   ecx
        xor    dl, 0x1b
        mov    ecx, 0x1a
loc_1_07CC:
        in     al, dx
        and    al, 0x40
        loope  loc_1_07CC
        je     loc_1_07E2
        push   0x40
        push   edx
        call   sub_1_000C
        add    esp, 8
        pop    ecx
        xor    eax, eax
        ret    
loc_1_07E2:
        push   edx
        xor    dl, 7
        push   0x22
        push   edx
        call   sub_1_000C
        add    esp, 8
        pop    edx
        push   0x40
        push   edx
        call   sub_1_000C
        add    esp, 8
        pop    ecx
        mov    eax, 0xfffffffe
        or     eax, eax
        ret    
        db  000h, 000h
loc_1_0808:
        mov    al, byte ptr [0xe68]          ; -> obj1:0x0E68
        cmp    al, 0
        jne    loc_1_082D
        inc    byte ptr [0xe68]              ; -> obj1:0x0E68
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_082F
        push   0
        push   0
        call   sub_4_1BA0
        cmp    eax, 0
        jne    loc_1_082F
loc_1_082D:
        clc    
        ret    
loc_1_082F:
        stc    
        ret    
HPMCA_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0840
        jmp    dword ptr [eax*4 + 0x842]     ; -> obj1:0x0842
loc_1_0840:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0808
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0840
        dd  loc_1_0808
        db  000h, 000h
sub_1_08B0:
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
        call   sub_1_08E0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_08E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_0980
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_0920
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_0915
        cmp    al, 0xff
        jne    loc_1_0915
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_0915
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_0987
loc_1_0915:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0987
        db  08Bh, 0FFh
loc_1_0920:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_0970
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0946
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_094D
loc_1_0946:
        mov    dword ptr [ebp - 0x10], 1
loc_1_094D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0960
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_098A
        db  08Dh, 064h, 024h, 000h
loc_1_0960:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_098A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0970:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_098A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0980:
        mov    dword ptr [ebp - 4], 0x20
loc_1_0987:
        mov    esi, dword ptr [ebp + 8]
loc_1_098A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0A81
loc_1_0995:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_09ED
        test   al, 2
        je     loc_1_09D9
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
        je     loc_1_09D9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_09ED
loc_1_09D9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_09E4
        test   bl, 4
        jne    loc_1_09ED
loc_1_09E4:
        test   bl, 0x20
        je     loc_1_0A76
loc_1_09ED:
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
        call   sub_1_0C62
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0A76
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_0A2F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_0A2F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0A40
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0A46
        db  08Bh, 0FFh
loc_1_0A40:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_0A46:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0A55
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0C68
loc_1_0A55:
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
        call   sub_1_0CDC
        mov    edi, ebx
        jmp    loc_1_0A79
loc_1_0A76:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0A79:
        or     edi, edi
        jne    loc_1_0995
loc_1_0A81:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0A90:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0AA0
        pop    ebp
        ret    4
sub_1_0AA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0B1E
loc_1_0AB0:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0B17
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0C68
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0B17
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_0ADC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_0ADC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0AF0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0AF6
        db  005h, 000h, 000h, 000h, 000h
loc_1_0AF0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_0AF6:
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
        call   sub_1_0CDC
        mov    edi, ebx
        jmp    loc_1_0B1A
loc_1_0B17:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0B1A:
        or     edi, edi
        jne    loc_1_0AB0
loc_1_0B1E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B30:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_0B45
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B45:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_0B81
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_0B72
        cmp    bl, 0xff
        jne    loc_1_0B72
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_0B72
        test   al, 8
        je     loc_1_0B6B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B6B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B72:
        test   al, 4
        jne    loc_1_0B7A
        test   al, 2
        je     loc_1_0B99
loc_1_0B7A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B81:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0B99
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_0B99
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B99:
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
sub_1_0C1A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_0C20:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_0C26:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_0C2C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_0C32:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_0C38:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_0C3E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0C44:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_0C4A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_0C56:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0C62:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0C68:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_0C6E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_0C74:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_0C80:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_0CB2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_0CBE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_0CC4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_0CCA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_0CD0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_0CD6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_0CDC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0D00:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_0D06:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_0D0C:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_0D12:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_0D1E:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0D24:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0D48:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_0D4E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0D54:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_0D5A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0D60:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
sub_1_0D84:
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
sub_1_0D8A:
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
sub_1_0D90:
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
loc_1_0D96:
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
sub_1_0D9C:
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
loc_1_0DA2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_0DA8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_0DBC:
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
sub_1_0DDC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_0DFC
        or     ecx, ecx
        je     loc_1_0E12
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_0E10
loc_1_0DFC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_0E10
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_0E10:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0E12:
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
        db  'HPMCA   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  HPMCA_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh, 04Dh, 075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h
        db  04Ch, 069h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0E88:
        dec    ebp
        popal  
        js     0xef5
        insd   dword ptr es:[edi], dx
        jne    loc_1_0EFC
        dec    ebp
        jne    loc_1_0EFE
        je     0xefd
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_0EE5
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    esi
        je     loc_1_0F1B
        outsd  dx, dword ptr [esi]
        jb     0xf12
        inc    ecx
        jb     loc_1_0F11
        jae    loc_1_0F21
        add    byte ptr [eax], al
        dec    esi
        je     0xf2b
        outsd  dx, dword ptr [esi]
        jb     0xf22
        inc    ecx
        jb     loc_1_0F21
        jae    loc_1_0F31
        add    byte ptr [eax], al
        push   ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     loc_1_0F13
        jne    0xf34
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        push   ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     loc_1_0F1F
        jne    0xf40
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        inc    edx
        jne    0xf4e
        dec    esi
        jne    0xf4b
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        db  000h
loc_1_0EE4:
        inc    edx
loc_1_0EE5:
        jne    0xf5a
        dec    esi
        jne    loc_1_0F57
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        db  000h
loc_1_0EF0:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
loc_1_0EFC:
        add    al, 1
loc_1_0EFE:
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
loc_1_0F11:
        add    dword ptr [ecx], eax
loc_1_0F13:
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
loc_1_0F1B:
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
loc_1_0F1F:
        add    byte ptr [edx], cl
loc_1_0F21:
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
loc_1_0F31:
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
loc_1_0F57:
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
loc_1_0F6C:
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
loc_1_0FC8:
        dec    eax
        push   eax
        xor    dh, byte ptr [edi]
        xor    dh, byte ptr [esi + esi]
        inc    ecx
        and    byte ptr [ebp + 0x43], cl
        and    byte ptr [ecx + eax*2 + 0x4e], cl
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x1051
        jb     0x100f
        xor    dword ptr [esi], esi
        and    byte ptr [eax + edx*2], dl
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 04Dh, 043h, 041h, 000h, 000h, 000h
loc_1_0FF4:
        dec    eax
        push   eax
        dec    ebp
        inc    ebx
        inc    ecx
        add    byte ptr [eax], al
        db  000h
loc_1_0FFC:
        dec    esi
        dec    edi
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
        popal  
        jo     loc_1_107D
        jb     loc_1_102C
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_1_1030
        jo     0x1084
        jae    loc_1_107A
        outsb  dx, byte ptr [esi]
        je     loc_1_1018
loc_1_1018:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    0x1084
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x49], 0x61204f2f
loc_1_102C:
        jb     loc_1_1095
loc_1_1030:
        jae    0x10a5
        sub    byte ptr [ebp + 0x73], ah
        sub    dword ptr [eax], eax
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    loc_1_10A4
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x49], 0x7265746e
        jb     0x10c3
        jo     loc_1_10C4
        and    byte ptr [esi + 0x65], dl
        arpl   word ptr [edi + ebp*2 + 0x72], si
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        jb     0x10d1
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x6e], ch
        je     loc_1_10CE
        jb     0x10dd
        jne    0x10dd
        je     0x108f
        jbe    loc_1_10D6
        arpl   word ptr [edi + ebp*2 + 0x72], si
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x50], cl
loc_1_107A:
        das    
        xor    dword ptr [esi], esi
loc_1_107D:
        and    byte ptr [ebp + 0x43], cl
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x61], al
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jb     loc_1_10F5
        jae    loc_1_1106
        popal  
        jb     0x1109
loc_1_1095:
        add    byte ptr cs:[eax], al
loc_1_1098:
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jb     0x1109
loc_1_10A4:
        jae    0x111a
        popal  
        jb     0x111d
        add    byte ptr cs:[eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_10C4:
        add    byte ptr [eax], al
loc_1_10C6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10CE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10DE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10E6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10F2:
        add    byte ptr [eax], al
        db  000h
loc_1_10F5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_1106:
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
        mov    ebx, dword ptr [0x10ac]       ; -> obj1:0x10AC
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_0CD6
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
        call   sub_1_0CD6
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
        call   sub_1_0CD6
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
        call   sub_1_0CDC
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_0CDC
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_0CDC
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
        mov    eax, dword ptr [0x10ac]       ; -> obj1:0x10AC
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0CD6
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
        call   sub_1_0C68
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_0CDC
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
        call   sub_1_0DDC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_0DDC
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
        call   sub_1_0DDC
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
        call   sub_1_0DDC
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
        call   sub_1_0DDC
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
        call   sub_1_0DDC
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
        call   sub_1_0DDC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0DDC
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
        call   sub_1_0DDC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0DDC
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
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [edi + 0x24], 0
        jbe    loc_2_071C
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        call   sub_1_0DDC
        add    ebx, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [edi + 0x24], eax
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
        call   sub_1_0DDC
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x36a0  R-X
        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 04Ch
        db  024h, 00Ch, 08Bh, 0ECh, 083h, 0ECh, 008h, 085h, 0C9h, 075h, 01Eh, 0A1h
        dd  obj1:0x0FE8              ; fixup
        db  08Bh, 04Dh, 010h, 050h, 08Dh, 055h, 0FCh, 06Ah, 003h, 051h, 052h, 0E8h
        dd  sub_1_0CD6               ; fixup
        db  08Bh, 045h, 0FCh, 08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Bh, 045h, 010h, 08Bh, 055h, 008h, 050h
        db  051h, 08Bh, 08Ah, 001h, 002h, 000h, 000h, 08Dh, 045h, 0FCh, 051h, 08Dh, 055h, 0F8h, 050h, 052h
        db  0E8h
        dd  loc_1_0DA2               ; fixup
        db  08Bh, 045h, 0FCh, 08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0080:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 0xf
        sub    esp, 0x3c
        lea    edx, [ebp - 0x3c]
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 0x3c]
        rep stosd dword ptr es:[edi], eax
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0x10]
        cmp    byte ptr [eax], cl
        je     loc_4_00B3
loc_4_00A1:
        cmp    ecx, 0x3c
        jae    loc_4_00B3
        mov    bl, byte ptr [eax + ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [eax + ecx], 0
        jne    loc_4_00A1
loc_4_00B3:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 0x10]
        push   edx
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x18]
        push   ecx
        mov    eax, dword ptr [ebp - 0x1c]
        push   edx
        mov    ecx, dword ptr [ebp - 0x20]
        push   eax
        mov    edx, dword ptr [ebp - 0x24]
        push   ecx
        mov    eax, dword ptr [ebp - 0x28]
        push   edx
        mov    ecx, dword ptr [ebp - 0x2c]
        push   eax
        mov    edx, dword ptr [ebp - 0x30]
        push   ecx
        mov    eax, dword ptr [ebp - 0x34]
        push   edx
        mov    ecx, dword ptr [ebp - 0x38]
        push   eax
        mov    edx, dword ptr [ebp - 0x3c]
        push   ecx
        mov    esi, dword ptr [ebp + 8]
        push   edx
        push   0x12
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0DA8
loc_4_0111:
        mov    di, word ptr [ebp + 0xc]
        add    esp, 0x54
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax], di
        je     loc_4_012E
        push   eax
        call   sub_4_2660
        mov    eax, dword ptr [esi]
        mov    word ptr [eax], di
        or     dword ptr [esi + 0x47], 2
loc_4_012E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 068h, 0E8h, 003h, 000h, 000h, 0E8h
        dd  sub_1_0CD0               ; fixup
        db  0FFh, 005h
        dd  obj1:0x1102              ; fixup
        db  0A1h
        dd  obj1:0x1102              ; fixup
        db  0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        push   esi
        mov    ebx, dword ptr [ebp + 0x14]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x18]
        push   ebx
        mov    edi, dword ptr [esi + 0x4b]
        and    ecx, 0xffff
        push   eax
        lea    edx, [esi + 0x53]
        mov    dword ptr [esi + 0x63], ebx
        mov    dword ptr [esi + 0x5f], ecx
        lea    ecx, [ebp - 4]
        mov    eax, dword ptr [esi + 0x4f]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0D12
loc_4_01A7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_01BB
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_01BB:
        mov    ax, word ptr [ebp + 0x18]
        dec    ax
        mov    ecx, dword ptr [esi + 0x53]
        cmp    ax, 1
        sbb    al, al
        lea    edx, [ebp - 4]
        inc    al
        push   eax
        push   ebx
        push   0
        push   ecx
        push   edi
        push   edx
        call   sub_1_0D8A
loc_4_01DB:
        mov    eax, 0
        pop    edi
        cmp    dword ptr [ebp - 4], 1
        adc    eax, -1
        pop    esi
        and    eax, 0xfffffffe
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 048h, 04Bh, 051h, 0E8h
        dd  loc_1_0D96               ; fixup
        db  048h, 05Dh, 0C2h, 008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x5f]
        mov    ecx, dword ptr [esi + 0x63]
        dec    eax
        mov    edx, dword ptr [esi + 0x53]
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [esi + 0x4b]
        push   edx
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_0D90
loc_4_024D:
        mov    ecx, dword ptr [esi + 0x53]
        push   ecx
        call   sub_1_0D1E
loc_4_0256:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0260:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        in     al, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_0270:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        in     ax, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0280:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    al, byte ptr [esp + 0x10]
        out    dx, al
        pop    ebp
        ret    0xc
sub_4_0290:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ax, word ptr [esp + 0x10]
        mov    ebp, esp
        out    dx, ax
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_02B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_31F0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_02D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 0xb
        jne    loc_4_0300
        push   esi
        call   sub_4_19E0
        test   al, al
        je     loc_4_0300
        mov    dword ptr [esi + 0xb3], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3010
        push   esi
        call   sub_4_1A10
loc_4_0300:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0310:
        push   ebp
        mov    eax, 0x15
        mov    ebp, esp
        sub    esp, 0x38
        mov    word ptr [ebp - 0x30], ax
        push   ebx
        mov    word ptr [ebp - 0x2e], ax
        push   esi
        push   edi
        mov    eax, 0xa
        mov    word ptr [ebp - 0x38], ax
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x36], ax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0x14]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 0x14], eax
        mov    ecx, dword ptr [esi + 0x18]
        xor    ebx, ebx
        mov    dword ptr [ebp - 0x18], edx
        mov    dword ptr [ebp - 0x10], ecx
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 8]
        push   edi
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 0x20], ebx
        mov    dword ptr [ebp - 0x2c], 0     ; -> loc_1_0E88
        mov    dword ptr [ebp - 0x34], 0     ; -> loc_1_0EE4
        push   ecx
        call   sub_1_0D54
loc_4_0372:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_0385
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0385:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0D5A
loc_4_039C:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_03AB
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x14], ecx
loc_4_03AB:
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x20]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   edi
        push   edx
        call   sub_1_0C80
loc_4_03BD:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_047F
        cmp    word ptr [ebp - 0x28], 0x63ca
        jne    loc_4_047F
        mov    al, byte ptr [ebp - 0x26]
        and    al, 0xe
        shr    al, 1
        and    eax, 0xff
        cmp    eax, 7
        ja     loc_4_040A
        jmp    dword ptr [eax*4]             ; -> obj4:0x0498
loc_4_03EC:
        mov    bl, 3
        jmp    loc_4_040A
loc_4_03F0:
        mov    bl, 4
        jmp    loc_4_040A
loc_4_03F4:
        mov    bl, 5
        jmp    loc_4_040A
loc_4_03F8:
        mov    bl, 7
        jmp    loc_4_040A
loc_4_03FC:
        mov    bl, 9
        jmp    loc_4_040A
loc_4_0400:
        mov    bl, 0xa
        jmp    loc_4_040A
loc_4_0404:
        mov    bl, 0xb
        jmp    loc_4_040A
loc_4_0408:
        mov    bl, 0xc
loc_4_040A:
        mov    edi, ebx
        push   0
        and    edi, 0xff
        mov    bl, byte ptr [ebp - 0x26]
        and    ebx, 0xe0
        lea    eax, [ebp - 0x30]
        add    ebx, 8
        push   eax
        shl    ebx, 7
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0D5A
loc_4_0438:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0447
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], ecx
loc_4_0447:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0D60
loc_4_0450:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 4], eax
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0xc], edi
        pop    edi
        mov    dword ptr [esi + 0x14], edx
        mov    dword ptr [esi + 0x10], 1
        mov    dword ptr [esi + 0x18], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_047F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0D60
loc_4_0488:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
        ; ---- dispatch table (8 entries) ----
        dd  loc_4_03EC
        dd  loc_4_03F0
        dd  loc_4_03F4
        dd  loc_4_03F8
        dd  loc_4_03FC
        dd  loc_4_0400
        dd  loc_4_0404
        dd  loc_4_0408
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_04C0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x1c
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   ecx
        mov    dword ptr [ebp - 0xc], eax
        push   edx
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 8], 0xffffffff
        mov    dword ptr [ebp - 4], 0x14
        push   eax
        call   sub_4_0310
        test   eax, eax
        je     loc_4_0505
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0505:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x205], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x21d], edx
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x219], eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx + 0x211], edx
        jne    loc_4_053B
        mov    dword ptr [ecx + 0x20], 1
        jmp    loc_4_0548
loc_4_053B:
        cmp    dword ptr [ebp - 0xc], 1
        jne    loc_4_0548
        mov    dword ptr [ecx + 0x20], 2
loc_4_0548:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x209], eax
        mov    dword ptr [ecx + 0x20d], edx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0570:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     loc_4_0591
        mov    eax, 0xc0010002
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0591:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x10]
        test   ecx, ecx
        jne    loc_4_05AB
        push   edx
        push   esi
        push   edi
        call   sub_4_09C0
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05AB:
        cmp    ecx, 1
        jne    loc_4_05E3
        mov    ecx, dword ptr [edi + 0x47]
        test   ecx, ecx
        je     loc_4_05D3
        test   cl, 2
        je     loc_4_05C1
        mov    eax, 0xc0000001
loc_4_05C1:
        test   cl, 1
        je     loc_4_05E8
        mov    eax, 0xc001000d
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05D3:
        push   edx
        push   esi
        push   edi
        call   sub_4_0A10
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05E3:
        mov    eax, 0x10001
loc_4_05E8:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_05F0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x24
        mov    word ptr [ebp - 2], ax
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x1c], eax
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], eax
        cmp    esi, 0x1010101
        mov    eax, esi
        ja     loc_4_062F
        je     loc_4_0815
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_4_0650
        jmp    dword ptr [eax*4]             ; -> obj4:0x0968
loc_4_062F:
        cmp    eax, 0x1010102
        je     loc_4_0834
        cmp    eax, 0x1010103
        je     loc_4_0853
        cmp    eax, 0x1010104
        je     loc_4_0903
loc_4_0650:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_4_0919
loc_4_065C:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_4_0671
        mov    eax, 0                        ; -> loc_1_0F6C
        mov    edi, 0x5c
        jmp    loc_4_091F
loc_4_0671:
        mov    eax, 0                        ; -> loc_1_0EF0
        mov    edi, 0x7c
        jmp    loc_4_091F
loc_4_0680:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x47]
        test   al, 2
        je     loc_4_0693
        mov    dword ptr [ebp - 0x18], 4
        jmp    loc_4_06A5
loc_4_0693:
        mov    dword ptr [ebp - 0x18], 2
        test   al, 1
        jne    loc_4_06A5
        mov    dword ptr [ebp - 0x18], 0
loc_4_06A5:
        lea    eax, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_4_091F
loc_4_06B2:
        lea    eax, [ebp - 0x1c]
        mov    edi, 4
        jmp    loc_4_091F
loc_4_06BF:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_4_091F
loc_4_06D3:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_4_091F
loc_4_06E7:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_4_091F
loc_4_06FB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa3]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_091F
loc_4_0714:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa7]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_091F
loc_4_072D:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_091F
loc_4_0741:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xab]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_091F
loc_4_075A:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 4
        call   sub_1_0DBC
loc_4_0771:
        mov    byte ptr [ebp - 0x21], 0
        lea    eax, [ebp - 0x24]
        jmp    loc_4_091F
loc_4_077D:
        mov    eax, 0                        ; -> loc_1_0FC8
        mov    edi, 0x1f
        jmp    loc_4_091F
loc_4_078C:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_079D
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7b]
        mov    ecx, dword ptr [eax + 0xc]
        jmp    loc_4_07A6
loc_4_079D:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [eax + 8]
loc_4_07A6:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_4_091F
loc_4_07B6:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_07C4
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7f]
        jmp    loc_4_07CA
loc_4_07C4:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x21]
loc_4_07CA:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_091F
loc_4_07DA:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_4_091F
loc_4_07ED:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_091F
loc_4_0801:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    loc_4_091F
loc_4_0815:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_0DBC
loc_4_082C:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_091F
loc_4_0834:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x67
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_0DBC
loc_4_084B:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_091F
loc_4_0853:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_08A9
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 0x7b]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_0880
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_0919
loc_4_0880:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_0760
loc_4_0896:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0xc], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_0919
loc_4_08A9:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [edi + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0660
loc_4_08BC:
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_08D7
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_0919
loc_4_08D7:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebx + 0x1d]
        push   edx
        mov    ecx, dword ptr [edi + 0x7b]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_2_06A0
loc_4_08F4:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_0919
loc_4_0903:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x20d]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_091F
loc_4_0919:
        mov    eax, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [ebp - 0x14]
loc_4_091F:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_095A
        cmp    esi, 0x1010103
        je     loc_4_095A
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_4_094A
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    ecx, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_4_094A:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0DBC
loc_4_0955:
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_4_095A:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
        ; ---- dispatch table (19 entries) ----
        dd  loc_4_065C
        dd  loc_4_0680
        dd  loc_4_06B2
        dd  loc_4_06B2
        dd  loc_4_06BF
        dd  loc_4_06D3
        dd  loc_4_06E7
        dd  loc_4_06FB
        dd  loc_4_0714
        dd  loc_4_072D
        dd  loc_4_0741
        dd  loc_4_075A
        dd  loc_4_077D
        dd  loc_4_078C
        dd  loc_4_07B6
        dd  loc_4_07DA
        dd  loc_4_07ED
        dd  loc_4_0650
        dd  loc_4_0801
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_09C0:
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
        call   sub_4_05F0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_4_0A4D
        je     loc_4_0B1F
        cmp    eax, 0x1010e
        je     loc_4_0A5F
        cmp    eax, 0x1010f
        je     loc_4_0A99
        cmp    eax, 0x10112
        je     loc_4_0B0B
loc_4_0A4D:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B5A
loc_4_0A5F:
        cmp    edi, 4
        je     loc_4_0A76
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B5A
loc_4_0A76:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_0DBC
loc_4_0A82:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0B80
        jmp    loc_4_0B58
loc_4_0A99:
        cmp    edi, 4
        je     loc_4_0AB0
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B5A
loc_4_0AB0:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_0DBC
loc_4_0ABC:
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_4_0B04
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x7f]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_4_0ADE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x7f], eax
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0B5A
loc_4_0ADE:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx + 0x21]
        jne    loc_4_0AFC
        mov    eax, dword ptr [ecx + 0x21]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_4_0AFC
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x21], eax
        call   sub_4_0DD0
        jmp    loc_4_0B5A
loc_4_0AFC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0B5A
loc_4_0B04:
        mov    ebx, 0xc0000001
        jmp    loc_4_0B5A
loc_4_0B0B:
        cmp    edi, 4
        je     loc_4_0B5A
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B5A
loc_4_0B1F:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_4_0B3D
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B5A
loc_4_0B3D:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        call   sub_4_0BE0
loc_4_0B58:
        mov    ebx, eax
loc_4_0B5A:
        test   ebx, ebx
        jne    loc_4_0B68
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_4_0B68:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B80:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0BC2
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0BB8
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 0x1d]
        push   eax
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 0x7b]
        push   ecx
        call   sub_2_05E0
loc_4_0BB1:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0BB8:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0BC2:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0BE0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0C26
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0C1C
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [esi + 0x1d]
        push   edx
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 0x7b]
        push   ecx
        call   sub_2_0300
loc_4_0C15:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0C1C:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0C26:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_0C40:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    dword ptr [ebp - 0xc], ecx
        push   esi
        mov    dword ptr [ebp - 0x10], ecx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 0xc]
        lea    ebx, [ebp - 0x10]
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        push   ebx
        mov    edx, dword ptr [esi + 0x14]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        mov    dword ptr [ebp - 4], ecx
        push   1
        push   edx
        push   ecx
        push   ebx
        call   sub_4_05F0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_4_0D15
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_4_0CA5
        je     loc_4_0CE4
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_4_0CB3
        jmp    dword ptr [eax*4]             ; -> obj4:0x0D30
loc_4_0CA5:
        cmp    eax, 0x1020102
        je     loc_4_0CEC
        cmp    eax, 0x1020103
        je     loc_4_0CF4
loc_4_0CB3:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_4_0CFD
loc_4_0CBC:
        mov    eax, dword ptr [ebx + 0x83]
        jmp    loc_4_0CFA
loc_4_0CC4:
        mov    eax, dword ptr [ebx + 0x87]
        jmp    loc_4_0CFA
loc_4_0CCC:
        mov    eax, dword ptr [ebx + 0x8b]
        jmp    loc_4_0CFA
loc_4_0CD4:
        mov    eax, dword ptr [ebx + 0x9b]
        jmp    loc_4_0CFA
loc_4_0CDC:
        mov    eax, dword ptr [ebx + 0x9f]
        jmp    loc_4_0CFA
loc_4_0CE4:
        mov    eax, dword ptr [ebx + 0x97]
        jmp    loc_4_0CFA
loc_4_0CEC:
        mov    eax, dword ptr [ebx + 0x8f]
        jmp    loc_4_0CFA
loc_4_0CF4:
        mov    eax, dword ptr [ebx + 0x93]
loc_4_0CFA:
        mov    dword ptr [ebp - 4], eax
loc_4_0CFD:
        cmp    edi, 4
        jb     loc_4_0D15
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_0DBC
loc_4_0D11:
        add    dword ptr [ebp - 0xc], 4
loc_4_0D15:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        pop    edi
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_4_0CBC
        dd  loc_4_0CC4
        dd  loc_4_0CCC
        dd  loc_4_0CD4
        dd  loc_4_0CDC
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0D50:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [ecx + 0x14]
        cmp    edx, 0x10113
        ja     loc_4_0D78
        je     loc_4_0DAF
        cmp    edx, 0x10101
        jb     loc_4_0DAA
        cmp    edx, 0x10111
        jbe    loc_4_0DAF
        jmp    loc_4_0DAA
loc_4_0D78:
        cmp    edx, 0x1010104
        ja     loc_4_0D9A
        cmp    edx, 0x1010101
        jae    loc_4_0DAF
        cmp    edx, 0x20101
        jb     loc_4_0DAA
        cmp    edx, 0x20105
        jbe    loc_4_0DAF
        jmp    loc_4_0DAA
loc_4_0D9A:
        cmp    edx, 0x1020101
        jb     loc_4_0DAA
        cmp    edx, 0x1020103
        jbe    loc_4_0DAF
loc_4_0DAA:
        mov    eax, 0xc0010017
loc_4_0DAF:
        test   eax, eax
        jne    loc_4_0DBD
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0C40
loc_4_0DBD:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0DD0:
        push   ebp
        mov    ecx, 0xee
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x1c5]
        test   edx, edx
        je     loc_4_0DF6
loc_4_0DE6:
        mov    esi, dword ptr [edx + 0x21]
        cmp    ecx, esi
        jae    loc_4_0DEF
        mov    ecx, esi
loc_4_0DEF:
        mov    edx, dword ptr [edx + 0x11]
        test   edx, edx
        jne    loc_4_0DE6
loc_4_0DF6:
        pop    esi
        mov    dword ptr [eax + 0x7f], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0E00:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        lea    ecx, [ebp - 0x14]
        push   esi
        lea    edx, [ebp - 0x10]
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   edx
        push   0
        push   ecx
        call   sub_1_0D00
loc_4_0E27:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     loc_4_0E3E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E3E:
        mov    eax, dword ptr [esi + 0x47]
        test   eax, eax
        je     loc_4_0E69
        test   al, 0xa
        je     loc_4_0E57
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E57:
        test   al, 1
        je     loc_4_0E69
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E69:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_0E90
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1020
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E90:
        push   esi
        call   sub_4_19E0
        test   al, al
        jne    loc_4_0EBA
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1020
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x46], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0EBA:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_0D24
loc_4_0ECB:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2B90
        test   eax, eax
        je     loc_4_0F19
        jle    loc_4_0F07
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1020
        push   esi
        call   sub_4_1A10
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F07:
        test   eax, eax
        jge    loc_4_0F19
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F19:
        xor    edi, edi
        xor    eax, eax
loc_4_0F1D:
        cmp    eax, dword ptr [ebp - 8]
        jae    loc_4_0F37
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0xb6], cl
        jb     loc_4_0F1D
loc_4_0F37:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        je     loc_4_0F9E
loc_4_0F3F:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0D48
loc_4_0F4C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0D24
loc_4_0F5D:
        xor    ecx, ecx
        cmp    edi, 6
        jae    loc_4_0F7E
loc_4_0F64:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_0F7E
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0xb6], al
        jb     loc_4_0F64
loc_4_0F7E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_0124
loc_4_0F8E:
        add    esp, 0xc
        test   eax, eax
        jl     loc_4_0FB8
        inc    ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, ebx
        ja     loc_4_0F3F
loc_4_0F9E:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2CB0
        test   eax, eax
        jge    loc_4_0FC6
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FB8:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FC6:
        lea    eax, [esi + 0xb7]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_0B30
loc_4_0FD6:
        test   al, al
        je     loc_4_0FFC
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1060
        push   esi
        call   sub_4_1A10
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FFC:
        push   esi
        inc    dword ptr [esi + 0x83]
        call   sub_4_1A10
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_1760
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_1020:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_4_104C
        pop    ebp
        mov    dword ptr [eax + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
loc_4_104C:
        mov    ecx, dword ptr [eax + 0x28]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_1060:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x38], 0
        jne    loc_4_1089
        pop    ebp
        mov    dword ptr [eax + 0x38], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
loc_4_1089:
        mov    ecx, dword ptr [eax + 0x34]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10A0:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x24], 0
        mov    eax, dword ptr [ecx + 0x24]
        test   eax, eax
        jne    loc_4_10C5
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [ecx + 0x28], edx
        inc    dword ptr [ecx + 0x2c]
        ret    8
loc_4_10C5:
        pop    ebp
        mov    dword ptr [edx + 0x24], eax
        mov    dword ptr [ecx + 0x24], edx
        inc    dword ptr [ecx + 0x2c]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    al, byte ptr [ebp + 0xc]
        push   esi
        test   al, al
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xb3], 0
        je     loc_4_1138
        test   al, 1
        je     loc_4_1104
        inc    dword ptr [esi + 0x30]
loc_4_1104:
        test   al, 2
        je     loc_4_1111
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x93]
loc_4_1111:
        test   al, 4
        je     loc_4_111E
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_111E:
        test   al, 8
        je     loc_4_112B
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_112B:
        test   al, 0x10
        je     loc_4_1138
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_1138:
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_4_114C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_114C:
        mov    edi, dword ptr [esi + 0x24]
        lea    ecx, [ebp - 0x18]
        push   ecx
        lea    edx, [ebp - 0xc]
        mov    eax, dword ptr [edi + 0x24]
        push   edx
        mov    dword ptr [esi + 0x24], eax
        lea    eax, [ebp - 0x14]
        push   eax
        dec    dword ptr [esi + 0x2c]
        push   0
        push   edi
        call   sub_1_0D00
loc_4_116C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0D24
loc_4_117D:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2B90
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_11C7
        push   edi
        push   esi
        call   sub_4_10A0
        test   ebx, ebx
        jge    loc_4_11B9
        add    ebx, 2
        pop    edi
        pop    esi
        cmp    ebx, 1
        sbb    eax, eax
        pop    ebx
        and    eax, 0xfffffffc
        mov    esp, ebp
        add    eax, 2
        pop    ebp
        ret    8
loc_4_11B9:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_11C7:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_11CB:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_11E5
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0xb6], al
        jb     loc_4_11CB
loc_4_11E5:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 0x14]
        dec    eax
        je     loc_4_1255
loc_4_11F2:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0D48
loc_4_11FF:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0D24
loc_4_1210:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    loc_4_1231
loc_4_1217:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_1231
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0xb6], al
        jb     loc_4_1217
loc_4_1231:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_0124
loc_4_1241:
        add    esp, 0xc
        cmp    eax, -2
        je     loc_4_1277
        mov    eax, dword ptr [ebp - 0x14]
        inc    dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, dword ptr [ebp - 0x10]
        ja     loc_4_11F2
loc_4_1255:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2CB0
        cmp    eax, -2
        jne    loc_4_128C
        push   edi
        push   esi
        call   sub_4_10A0
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1277:
        push   edi
        push   esi
        call   sub_4_10A0
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_128C:
        lea    eax, [esi + 0xb7]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_0B30
loc_4_129C:
        test   al, al
        je     loc_4_12BC
        push   edi
        mov    eax, dword ptr [edi + 0x28]
        push   eax
        push   esi
        call   sub_4_1060
        push   esi
        call   sub_4_34A0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_12BC:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0C4A
loc_4_12D1:
        push   esi
        call   sub_4_1760
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_12F0:
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
loc_4_1315:
        test   edx, edx
        je     loc_4_1324
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_4_1315
        mov    dword ptr [ecx], edx
loc_4_1324:
        cmp    dword ptr [ecx], -1
        jne    loc_4_1337
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_1337:
        mov    eax, dword ptr [0]            ; -> obj1:0x0FE8
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x25
        push   ecx
        call   sub_1_0CD6
loc_4_134A:
        test   eax, eax
        je     loc_4_135C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_135C:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x1d
        mov    esi, dword ptr [ebp + 0x20]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0170
loc_4_1377:
        test   al, al
        jne    loc_4_1396
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x25
        push   eax
        call   sub_1_0CDC
loc_4_1388:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_1396:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_13B8
        mov    eax, dword ptr [esi + 0x1cd]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 0x103
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1cd], ecx
        jmp    loc_4_13CF
loc_4_13B8:
        mov    eax, dword ptr [esi + 0x1c5]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1c5], ecx
loc_4_13CF:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xd], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x1c1]
        mov    dword ptr [eax + 0x19], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax + 5], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 9], edx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x7f]
        mov    dword ptr [eax + 0x21], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [esi + 0x1c9], 0
        jne    loc_4_1422
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
loc_4_1422:
        mov    eax, ebx
        pop    edi
        inc    dword ptr [esi + 0x1c9]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1440:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        xor    eax, eax
loc_4_144F:
        mov    dl, byte ptr [esi + eax + 0x67]
        mov    ecx, dword ptr [esi + 0x73]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     loc_4_144F
        mov    edi, dword ptr [esi + 0x73]
        add    edi, 6
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_4_1487
        mov    eax, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x10]
loc_4_1472:
        xor    ecx, ecx
loc_4_1474:
        mov    dl, byte ptr [eax + ecx]
        inc    edi
        inc    ecx
        mov    byte ptr [edi - 1], dl
        cmp    ecx, 6
        jb     loc_4_1474
        add    eax, 6
        dec    ebx
        jne    loc_4_1472
loc_4_1487:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x77], eax
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_14A0
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x44], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_14A0:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1B60
        push   eax
        call   sub_1_0CCA
loc_4_14AF:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2840
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28F0
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2920
        test   eax, eax
        jge    loc_4_14EE
        mov    eax, 0xc0000001
        pop    edi
        or     dword ptr [esi + 0x47], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_14EE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  090h
loc_4_1500:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [eax + 0xd]
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0xffffffd0
        je     loc_4_1520
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_1520:
        test   al, 1
        je     loc_4_1529
        mov    ecx, 1
loc_4_1529:
        test   al, 2
        je     loc_4_1530
        or     ecx, 4
loc_4_1530:
        test   al, 4
        je     loc_4_1537
        or     ecx, 8
loc_4_1537:
        test   al, 8
        je     loc_4_153E
        or     ecx, 2
loc_4_153E:
        test   al, 0x20
        je     loc_4_1545
        or     ecx, 0x10
loc_4_1545:
        test   dword ptr [esi + 0x1bd], ecx
        je     loc_4_1557
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_1557:
        mov    dword ptr [esi + 0x1b9], ecx
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_156E
        mov    byte ptr [esi + 0x43], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_4_156E:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1B60
        push   eax
        call   sub_1_0CCA
loc_4_157D:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 0x1b9]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2920
        test   eax, eax
        jge    loc_4_15A6
        or     dword ptr [esi + 0x47], 2
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_4_15A6:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xd]
        push   1
        add    ecx, 0x1d1
        push   ecx
        mov    edx, dword ptr [ecx + 0x1d]
        mov    dword ptr [eax + 0x15], edx
        mov    dword ptr [ecx + 0x1d], eax
        call   sub_1_0CB2
loc_4_15E1:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x1ee]
        test   ecx, ecx
        je     loc_4_1620
loc_4_1601:
        mov    eax, dword ptr [esi + 0x1ee]
        push   ecx
        mov    edx, dword ptr [eax + 0x15]
        mov    dword ptr [esi + 0x1ee], edx
        call   sub_4_1760
        mov    ecx, dword ptr [esi + 0x1ee]
        test   ecx, ecx
        jne    loc_4_1601
loc_4_1620:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1630:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 8
        jne    loc_4_1666
        push   esi
        call   sub_4_19E0
        test   al, al
        je     loc_4_1655
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D40
        push   esi
        call   sub_4_1A10
loc_4_1655:
        push   0x3e8
        add    esi, 0x1dd
        push   esi
        call   sub_1_0CB2
loc_4_1666:
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_4_1670:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 4], 0
        mov    esi, dword ptr [edi + 0xd]
        je     loc_4_168E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_168E:
        inc    dword ptr [edi]
        mov    eax, dword ptr [esi + 0x1c5]
        cmp    edi, eax
        jne    loc_4_16A5
        mov    eax, dword ptr [edi + 0x11]
        mov    dword ptr [esi + 0x1c5], eax
        jmp    loc_4_16B8
loc_4_16A5:
        cmp    dword ptr [eax + 0x11], edi
        je     loc_4_16B2
loc_4_16AA:
        mov    eax, dword ptr [eax + 0x11]
        cmp    dword ptr [eax + 0x11], edi
        jne    loc_4_16AA
loc_4_16B2:
        mov    ecx, dword ptr [edi + 0x11]
        mov    dword ptr [eax + 0x11], ecx
loc_4_16B8:
        push   0
        mov    eax, dword ptr [edi + 0x1d]
        push   eax
        mov    ecx, dword ptr [esi + 0x7b]
        push   ecx
        call   sub_2_0240
loc_4_16C7:
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_16DF
        cmp    ebx, 0x103
        je     loc_4_16F3
        cmp    ebx, 0xc001000e
        je     loc_4_16FC
        jmp    loc_4_1705
loc_4_16DF:
        mov    eax, dword ptr [edi]
        sub    eax, 2
        mov    dword ptr [edi], eax
        je     loc_4_1707
        mov    byte ptr [edi + 4], 1
        mov    ebx, 0x103
        jmp    loc_4_1707
loc_4_16F3:
        mov    byte ptr [edi + 4], 1
        sub    dword ptr [edi], 2
        jmp    loc_4_1707
loc_4_16FC:
        mov    byte ptr [edi + 4], 1
        mov    ebx, 0x103
loc_4_1705:
        dec    dword ptr [edi]
loc_4_1707:
        cmp    dword ptr [edi], 0
        jne    loc_4_174B
        mov    eax, dword ptr [edi + 0x21]
        cmp    dword ptr [esi + 0x7f], eax
        jne    loc_4_171A
        push   esi
        call   sub_4_0DD0
loc_4_171A:
        cmp    dword ptr [esi + 0x1c5], 0
        jne    loc_4_1732
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1B60
        push   eax
        call   sub_1_0CCA
loc_4_1732:
        push   0
        dec    dword ptr [esi + 0x1c9]
        push   0x25
        push   edi
        call   sub_1_0CDC
loc_4_1742:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_174B:
        mov    ebx, 0x103
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1760:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    loc_4_17B3
        mov    eax, dword ptr [esi + 0x21]
        cmp    dword ptr [edi + 0x7f], eax
        jne    loc_4_1780
        push   edi
        call   sub_4_0DD0
loc_4_1780:
        cmp    dword ptr [edi + 0x1c5], 0
        jne    loc_4_1798
        push   edi
        lea    eax, [edi + 8]
        push   0                             ; -> loc_4_1B60
        push   eax
        call   sub_1_0CCA
loc_4_1798:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0C44
loc_4_17A3:
        push   0
        push   0x25
        push   esi
        call   sub_1_0CDC
loc_4_17AD:
        dec    dword ptr [edi + 0x1c9]
loc_4_17B3:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_17C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        cmp    byte ptr [eax + 4], 0
        je     loc_4_17DD
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17DD:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_17EE
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17EE:
        push   0x40010004
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 0x1ea], eax
        call   sub_4_1980
        or     dword ptr [esi + 0x47], 1
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        je     loc_4_1837
loc_4_180C:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x24], eax
        dec    dword ptr [esi + 0x2c]
        inc    dword ptr [esi + 0x30]
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0C4A
loc_4_182A:
        push   edi
        call   sub_4_1760
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        jne    loc_4_180C
loc_4_1837:
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        je     loc_4_1863
loc_4_183E:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x38], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0C4A
loc_4_1856:
        push   edi
        call   sub_4_1760
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        jne    loc_4_183E
loc_4_1863:
        mov    byte ptr [esi + 0x1ff], 1
        push   esi
        call   sub_4_19E0
        test   al, al
        je     loc_4_188B
        push   esi
        call   sub_4_18A0
        push   esi
        call   sub_4_1A10
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_188B:
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x1ff], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_18A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x1ea]
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 0x1ff], 0
        mov    byte ptr [esi + 0x42], 1
        and    dword ptr [esi + 0x47], 0xfffffffd
        push   eax
        call   sub_4_2660
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2840
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28F0
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2920
        test   eax, eax
        jge    loc_4_18F0
        or     dword ptr [esi + 0x47], 2
loc_4_18F0:
        push   0x40010005
        push   esi
        call   sub_4_1980
        mov    eax, dword ptr [esi + 0x47]
        and    eax, 0xfffffffe
        mov    dword ptr [esi + 0x47], eax
        test   al, 2
        je     loc_4_190F
        push   0x80010004
        jmp    loc_4_1911
loc_4_190F:
        push   0
loc_4_1911:
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0C56
loc_4_191A:
        push   edi
        call   sub_4_1760
        push   esi
        call   sub_4_1930
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1930:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x1cd]
        test   ecx, ecx
        je     loc_4_1976
loc_4_1941:
        mov    eax, dword ptr [esi + 0x1cd]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0x11]
        mov    eax, dword ptr [esi + 0x1c5]
        mov    dword ptr [esi + 0x1cd], edx
        mov    dword ptr [ecx + 0x11], eax
        mov    dword ptr [esi + 0x1c5], ecx
        mov    ecx, dword ptr [ecx + 5]
        push   ecx
        call   sub_1_0C3E
loc_4_196C:
        mov    ecx, dword ptr [esi + 0x1cd]
        test   ecx, ecx
        jne    loc_4_1941
loc_4_1976:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1980:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x1c5]
        test   edi, edi
        je     loc_4_19CB
        mov    esi, dword ptr [ebp + 0xc]
loc_4_199A:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi]
        mov    ebx, dword ptr [edi + 0x11]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_0C6E
loc_4_19B6:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_0C74
loc_4_19BF:
        push   edi
        call   sub_4_1760
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_4_199A
loc_4_19CB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0x40], 0
        je     loc_4_19F3
        xor    al, al
        pop    ebp
        ret    4
loc_4_19F3:
        mov    byte ptr [eax + 0x40], 1
        push   eax
        push   0                             ; -> loc_4_1B60
        add    eax, 8
        push   eax
        call   sub_1_0CCA
loc_4_1A06:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1A10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x38], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 0x46]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x41]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x1ff]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x43]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x44]
        or     ecx, eax
        je     loc_4_1B3E
loc_4_1A4D:
        cmp    byte ptr [esi + 0x1ff], 0
        je     loc_4_1A5E
        mov    bl, 1
        push   esi
        call   sub_4_18A0
loc_4_1A5E:
        cmp    byte ptr [esi + 0x44], 0
        je     loc_4_1AA1
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2840
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28F0
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2920
        test   eax, eax
        jge    loc_4_1A9F
        or     dword ptr [esi + 0x47], 2
loc_4_1A9F:
        mov    bl, 1
loc_4_1AA1:
        cmp    byte ptr [esi + 0x43], 0
        je     loc_4_1ACC
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi + 0x1b9]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2920
        test   eax, eax
        jge    loc_4_1ACA
        or     dword ptr [esi + 0x47], 2
loc_4_1ACA:
        mov    bl, 1
loc_4_1ACC:
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_1AEA
        mov    byte ptr [esi + 0x46], 0
loc_4_1AD6:
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_1AE8
        push   0
        push   esi
        call   sub_4_10E0
        test   eax, eax
        je     loc_4_1AD6
loc_4_1AE8:
        mov    bl, 1
loc_4_1AEA:
        cmp    dword ptr [esi + 0x38], 0
        je     loc_4_1AF8
        push   esi
        mov    bl, 1
        call   sub_4_34A0
loc_4_1AF8:
        cmp    byte ptr [esi + 0x41], 0
        je     loc_4_1B0B
        mov    byte ptr [esi + 0x41], 0
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_1_0A90
loc_4_1B0B:
        cmp    dword ptr [esi + 0x38], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 0x46]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x41]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x1ff]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x43]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x44]
        or     ecx, eax
        jne    loc_4_1A4D
loc_4_1B3E:
        mov    byte ptr [esi + 0x40], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3240
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1B60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_31F0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 0B1h, 016h, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1BA0:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0x14
        mov    word ptr [ebp - 0x14], ax
        push   ebx
        mov    word ptr [ebp - 0x12], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0xc], bx
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], 0     ; -> loc_1_0FF4
        mov    word ptr [ebp - 0xa], 1
        mov    word ptr [ebp - 8], 0x63ca
        push   ecx
        push   eax
        push   0xc                           ; -> obj1:0x10BC
        call   sub_1_0C1A
loc_4_1BE0:
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        push   1
        mov    dword ptr [0x46], edx         ; -> obj1:0x10F6
        mov    dword ptr [0x4a], ecx         ; -> obj1:0x10FA
        mov    dword ptr [4], ebx            ; -> obj1:0x10B4
        mov    dword ptr [0x52], ebx         ; -> obj1:0x1102
        call   sub_4_23B0
        mov    dword ptr [0x4e], eax         ; -> obj1:0x10FE
        mov    byte ptr [0x10], 3            ; -> obj1:0x10C0
        mov    byte ptr [0x11], bl           ; -> obj1:0x10C1
        mov    dword ptr [0x12], ebx         ; -> obj1:0x10C2
        mov    dword ptr [0x16], 0           ; -> loc_1_10C6
        mov    dword ptr [0x1a], 0           ; -> loc_1_10CA
        mov    dword ptr [0x1e], 0           ; -> loc_1_10CE
        mov    dword ptr [0x22], 0           ; -> loc_1_10D2
        mov    dword ptr [0x26], 0           ; -> loc_1_10D6
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [0x2a], 0           ; -> loc_1_10DA
        mov    dword ptr [0x2e], 0           ; -> loc_1_10DE
        mov    dword ptr [0x32], 0           ; -> loc_1_10E2
        mov    dword ptr [0x36], 0           ; -> loc_1_10E6
        mov    dword ptr [0x3a], 0           ; -> loc_1_10EA
        push   0x36
        mov    dword ptr [0x3e], edx         ; -> loc_1_10EE
        push   0x10                          ; -> obj1:0x10C0
        lea    edx, [ebp - 4]
        push   0                             ; -> obj1:0x10B0
        mov    dword ptr [0x42], ecx         ; -> loc_1_10F2
        mov    eax, dword ptr [0xc]          ; -> obj1:0x10BC
        push   eax
        push   8                             ; -> obj1:0x10B8
        push   edx
        call   sub_1_0C26
loc_4_1CB3:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1CE0
        cmp    dword ptr [4], ebx            ; -> obj1:0x10B4
        jbe    loc_4_1CC9
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1CC9:
        push   0
        mov    eax, dword ptr [0xc]          ; -> obj1:0x10BC
        push   eax
        call   sub_1_0C20
loc_4_1CD6:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1CE0:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1CF0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0FE8
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   0x225
        push   ecx
        call   sub_1_0CD6
loc_4_1D0E:
        test   eax, eax
        je     loc_4_1D1F
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1D1F:
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_04C0
        test   eax, eax
        je     loc_4_1D51
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x225
        push   eax
        call   sub_1_0CDC
loc_4_1D44:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1D51:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_1D90
        test   eax, eax
        jne    loc_4_1D6A
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1D6A:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x225
        push   eax
        call   sub_1_0CDC
loc_4_1D7A:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_2240
        push   esi
        call   sub_4_2130
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_2095
        lea    edi, [ebp - 0x48]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x48], 0
        mov    eax, dword ptr [esi + 0x209]
        cmp    eax, -1
        je     loc_4_1DD6
        mov    dword ptr [ebp - 0x48], eax
loc_4_1DD6:
        mov    edi, 3
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0x205]
        mov    dword ptr [ebp - 0x3e], edi
        mov    dword ptr [ebp - 0x3a], ebx
        mov    dword ptr [ebp - 0x2e], eax
        lea    ecx, [ebp - 0x48]
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [8]            ; -> obj1:0x10B8
        push   eax
        lea    edx, [esi + 0x201]
        mov    dword ptr [ebp - 0x36], 0x600
        mov    dword ptr [ebp - 0x32], 1
        mov    dword ptr [ebp - 0x2a], 0x1f
        push   esi
        push   ecx
        push   edx
        call   sub_1_0C32
loc_4_1E21:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_4_208F
        mov    eax, dword ptr [esi + 0x209]
        cmp    eax, -1
        je     loc_4_1E9E
        lea    ecx, [ebp - 0x20]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], ebx
        mov    dword ptr [ecx + 0xc], ebx
        push   0x600
        mov    word ptr [ecx + 0x10], bx
        push   ecx
        mov    byte ptr [ecx + 0x12], bl
        lea    edx, [esi + 0x4b]
        mov    dword ptr [ebp - 0x19], edi
        mov    dword ptr [ebp - 0x15], eax
        mov    eax, dword ptr [esi + 0x201]
        lea    ecx, [ebp - 4]
        mov    byte ptr [ebp - 0x20], 1
        mov    dword ptr [ebp - 0x1d], 2
        push   eax
        push   edx
        push   ecx
        call   sub_1_0D84
loc_4_1E77:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1E9E
        mov    eax, dword ptr [esi + 0x209]
        mov    ecx, dword ptr [esi + 0x201]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_0DA8
loc_4_1E96:
        add    esp, 0x10
        jmp    loc_4_2083
loc_4_1E9E:
        mov    ecx, dword ptr [esi + 0x205]
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [esi + 0x209]
        push   edx
        mov    ebx, dword ptr [esi + 0x21d]
        push   ebx
        lea    eax, [ecx + 0x1f]
        push   ebx
        mov    edx, dword ptr [esi + 0x73]
        push   eax
        push   ecx
        mov    ecx, dword ptr [esi + 0x211]
        mov    eax, dword ptr [esi]
        push   ecx
        push   edx
        push   esi
        push   eax
        call   sub_4_23E0
        test   eax, eax
        jge    loc_4_1F04
        mov    eax, dword ptr [esi + 0x205]
        mov    ecx, dword ptr [esi + 0x211]
        push   eax
        mov    edx, dword ptr [esi + 0x201]
        push   ecx
        push   2
        push   0xaa000004
        push   edx
        call   sub_1_0DA8
loc_4_1EF5:
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0x14
        jmp    loc_4_2071
loc_4_1F04:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x20], eax
loc_4_1F15:
        lea    eax, [esi + ecx]
        mov    edx, dword ptr [esi + 0x73]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 0x6d], dl
        mov    byte ptr [eax + 0x67], dl
        jb     loc_4_1F15
        lea    ebx, [esi + 0x7b]
        lea    eax, [esi + 4]
        push   ebx
        lea    ecx, [esi + 0x67]
        push   eax
        mov    edx, dword ptr [esi + 0x20d]
        push   ecx
        push   0                             ; -> loc_4_15C0
        push   0                             ; -> loc_4_1500
        push   0                             ; -> loc_4_1440
        push   edx
        call   sub_2_0000
loc_4_1F52:
        test   al, al
        jne    loc_4_1F62
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_4_2071
loc_4_1F62:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2840
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28F0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2E20
        mov    edi, eax
        mov    ecx, dword ptr [0]            ; -> obj1:0x0FE8
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        call   sub_1_0CD6
loc_4_1F91:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_1FA4
        mov    dword ptr [ebp - 4], 0xc000009a
        jmp    loc_4_2069
loc_4_1FA4:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E40
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   edi
        push   ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 0xa3], edx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 0xa7], eax
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0xab], edx
        call   sub_1_0CDC
loc_4_1FD6:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3280
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 0x1bd], eax
        mov    dword ptr [esi + 0x1b9], 0
        call   sub_4_2920
        test   eax, eax
        jge    loc_4_2006
        or     dword ptr [esi + 0x47], 2
        jmp    loc_4_2069
loc_4_2006:
        lea    edx, [esi + 8]
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [esi + 0x211]
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [esi + 0x201]
        push   ecx
        push   0                             ; -> loc_4_02D0
        push   esi
        push   0                             ; -> loc_4_02B0
        push   eax
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        call   sub_1_0CBE
loc_4_203E:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_20A1
        mov    eax, dword ptr [esi + 0x211]
        mov    ecx, dword ptr [esi + 0x201]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_0DA8
loc_4_205E:
        add    esp, 0x10
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2740
loc_4_2069:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_4_2071:
        cmp    dword ptr [esi + 0x209], -1
        je     loc_4_2083
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_0D9C
loc_4_2083:
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0C38
loc_4_208F:
        push   esi
        call   sub_4_2200
loc_4_2095:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_20A1:
        mov    dword ptr [esi + 0x1c1], 0    ; -> obj1:0x10B0
        mov    byte ptr [esi + 0x45], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x1c9], ecx
        mov    dword ptr [esi + 0x1c5], ecx
        mov    dword ptr [esi + 0x1cd], ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x10B0
        mov    dword ptr [esi + 0x221], eax
        lea    eax, [esi + 0x1dd]
        mov    dword ptr [0], esi            ; -> obj1:0x10B0
        mov    dword ptr [esi + 0x1d1], ecx
        mov    dword ptr [esi + 0x1d9], esi
        push   0x3e8
        mov    dword ptr [eax], ecx
        push   eax
        mov    dword ptr [esi + 0x1e5], esi
        mov    dword ptr [esi + 0x1d5], 0    ; -> loc_4_15F0
        mov    dword ptr [esi + 0x1e1], 0    ; -> loc_4_1630
        call   sub_1_0CB2
loc_4_210D:
        mov    ecx, dword ptr [esi + 0x1c1]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2130:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0FE8
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [0x4e]         ; -> obj1:0x10FE
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   3
        push   ecx
        push   esi
        call   sub_1_0CD6
loc_4_2151:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_2166
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2166:
        lea    ebx, [esi + 0x73]
        mov    eax, dword ptr [0]            ; -> obj1:0x0FE8
        push   eax
        mov    ecx, dword ptr [esi + 0x20d]
        push   0
        lea    eax, [ecx + ecx*2 + 3]
        add    eax, eax
        push   eax
        push   ebx
        call   sub_1_0CD6
loc_4_2184:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_21D5
        mov    edx, dword ptr [esi + 0x20d]
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        lea    edx, [edx + edx*2 + 3]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        push   2
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    ecx, [ebp - 4]
        lea    eax, [esi + 0x4f]
        push   eax
        push   ecx
        call   sub_1_0D06
loc_4_21B8:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_21F3
        push   0
        mov    eax, dword ptr [esi + 0x20d]
        mov    ecx, dword ptr [ebx]
        lea    eax, [eax + eax*2 + 3]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_0CDC
loc_4_21D5:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> obj1:0x10FE
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_0CDC
loc_4_21E5:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_21F3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_2200:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x4f]
        push   eax
        call   sub_1_0D0C
loc_4_2210:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> obj1:0x10FE
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_0CDC
loc_4_2220:
        push   0
        mov    ecx, dword ptr [esi + 0x20d]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 0x73]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_0CDC
loc_4_2238:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_2240:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x1c9], ecx
        mov    byte ptr [eax + 0x40], cl
        mov    byte ptr [eax + 0x44], cl
        mov    byte ptr [eax + 0x1fe], cl
        mov    byte ptr [eax + 0x45], cl
        mov    byte ptr [eax + 0x1e9], cl
        mov    dword ptr [eax + 0x47], ecx
        mov    dword ptr [eax + 0x1b9], ecx
        mov    dword ptr [eax + 0x77], ecx
        mov    byte ptr [eax + 0x43], cl
        mov    byte ptr [eax + 0x1ff], cl
        mov    byte ptr [eax + 0x200], cl
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x30], ecx
        mov    byte ptr [eax + 0x46], cl
        mov    dword ptr [eax + 0x34], ecx
        pop    ebp
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x7f], 0xee
        mov    dword ptr [eax + 0x3c], ecx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_22B0:
        push   ebp
        mov    eax, dword ptr [8]            ; -> obj1:0x10B8
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_0C2C
loc_4_22C5:
        push   0
        mov    ecx, dword ptr [0xc]          ; -> obj1:0x10BC
        push   ecx
        call   sub_1_0C20
loc_4_22D3:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_22E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x4c4b40
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0x47], 8
        lea    edi, [esi + 8]
        call   sub_1_0CD0
loc_4_22F9:
        push   esi
        push   0                             ; -> loc_4_2390
        push   edi
        call   sub_1_0CCA
loc_4_2305:
        push   edi
        call   sub_1_0CC4
loc_4_230B:
        cmp    esi, dword ptr [0]            ; -> obj1:0x10B0
        jne    loc_4_2320
        mov    eax, dword ptr [esi + 0x221]
        mov    dword ptr [0], eax            ; -> obj1:0x10B0
        jmp    loc_4_234E
loc_4_2320:
        mov    ecx, dword ptr [0]            ; -> obj1:0x10B0
        cmp    dword ptr [ecx + 0x221], esi
        je     loc_4_233C
loc_4_232E:
        mov    ecx, dword ptr [ecx + 0x221]
        cmp    dword ptr [ecx + 0x221], esi
        jne    loc_4_232E
loc_4_233C:
        mov    eax, dword ptr [ecx + 0x221]
        mov    edx, dword ptr [eax + 0x221]
        mov    dword ptr [ecx + 0x221], edx
loc_4_234E:
        cmp    dword ptr [esi + 0x209], -1
        je     loc_4_2360
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_0D9C
loc_4_2360:
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_2_0120
loc_4_2369:
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0C38
loc_4_2375:
        push   esi
        call   sub_4_2200
        push   0
        push   0x225
        push   esi
        call   sub_1_0CDC
loc_4_2388:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_2390:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2740
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2660
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_23B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    dword ptr [0], eax            ; -> obj7:0x0000
        test   eax, eax
        jne    loc_4_23CA
        mov    dword ptr [0], 1              ; -> obj7:0x0000
loc_4_23CA:
        mov    eax, 0x1f2
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_23E0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0xc
        mov    ax, word ptr [ebp + 0x18]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], cx
        mov    edi, dword ptr [ebp + 0x10]
        mov    word ptr [esi + 2], ax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 0x10], edi
        mov    dword ptr [edi], ecx
        mov    word ptr [edi + 4], cx
loc_4_240B:
        mov    eax, dword ptr [ebp + 0x18]
        add    eax, ecx
        inc    ecx
        cmp    ecx, 0x20
        mov    dword ptr [esi + ecx*4 + 0x3c], eax
        jb     loc_4_240B
        xor    ecx, ecx
        mov    eax, 0                        ; -> obj6:0x0008
loc_4_2421:
        xor    edx, edx
        mov    dx, word ptr [eax]
        cmp    edx, dword ptr [ebp + 0x18]
        je     loc_4_2436
        add    eax, 2
        inc    ecx
        cmp    eax, 0x10                     ; -> obj6:0x0018
        jb     loc_4_2421
loc_4_2436:
        cmp    ecx, 8
        jae    loc_4_2643
        mov    eax, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp + 0x18]
        cmp    eax, 0x1f
        jb     loc_4_2643
        xor    eax, eax
        mov    cl, byte ptr [ebp + 0x14]
loc_4_2453:
        cmp    byte ptr [eax], cl            ; -> obj6:0x0000
        je     loc_4_2461
        inc    eax
        cmp    eax, 8
        jb     loc_4_2453
loc_4_2461:
        cmp    eax, 8
        jb     loc_4_247D
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> obj1:0x1038
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_247D:
        mov    eax, dword ptr [esi + 4]
        xor    ebx, ebx
        push   eax
        call   0
        mov    dword ptr [ebp - 8], 1
loc_4_248F:
        mov    eax, ebx
        mov    ecx, dword ptr [esi + 4]
        or     al, 8
        push   eax
        push   0x96
        push   ecx
        call   sub_4_0280
        push   0xff
        mov    ecx, dword ptr [esi + 4]
        push   0x94
        push   ecx
        call   sub_4_0280
        push   0x100
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        cmp    al, 0xca
        jne    loc_4_2501
        push   0x101
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        cmp    al, 0x63
        jne    loc_4_2501
        push   0x102
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        xor    ecx, ecx
        xor    edx, edx
        mov    byte ptr [ebp - 1], al
        and    al, 0xef
        shr    al, 4
        mov    cl, al
        mov    dx, word ptr [ecx]            ; -> obj6:0x0008
        cmp    edx, dword ptr [ebp + 0x18]
        je     loc_4_2509
loc_4_2501:
        inc    ebx
        cmp    ebx, 8
        jb     loc_4_248F
        jmp    loc_4_2532
loc_4_2509:
        mov    al, byte ptr [ebp - 1]
        xor    ecx, ecx
        and    al, 0xe
        xor    edx, edx
        shr    al, 1
        mov    cl, al
        mov    dl, byte ptr [ecx]            ; -> obj6:0x0000
        cmp    edx, dword ptr [ebp + 0x14]
        je     loc_4_2532
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [ebp - 8], 0xffffffff
        mov    dword ptr [eax], 0            ; -> obj1:0x1058
loc_4_2532:
        cmp    ebx, 8
        jae    loc_4_260E
        cmp    dword ptr [ebp - 8], 1
        jne    loc_4_2609
        mov    al, byte ptr [ebp - 1]
        or     al, 1
        mov    byte ptr [ebp - 0xc], al
        mov    byte ptr [esi + 0xb], al
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0xc], bl
        and    al, 0xfe
        mov    ecx, dword ptr [esi + 4]
        push   eax
        mov    ebx, 7
        push   0x102
        push   ecx
        call   sub_4_0280
loc_4_256C:
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    loc_4_256C
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   0x102
        push   ecx
        xor    ebx, ebx
        call   sub_4_0280
        push   0x102
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
loc_4_259C:
        mov    eax, dword ptr [esi + 0x40]
        mov    ecx, dword ptr [esi + 4]
        add    eax, ebx
        inc    edi
        push   eax
        inc    ebx
        push   ecx
        call   sub_4_0260
        mov    byte ptr [edi - 1], al
        cmp    ebx, 6
        jb     loc_4_259C
        mov    eax, dword ptr [esi + 0x5c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    byte ptr [esi + 0xa], al
        mov    eax, 0x100
        mov    word ptr [esi + 8], 1
        mov    byte ptr [esi + 0xe], 0x40
        mov    byte ptr [esi + 0xf], 0xff
        mov    word ptr [esi + 0xc0], ax
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dword ptr [esi + 0x1ce], eax
        mov    dword ptr [esi + 0x1c6], 0x4000
        mov    dword ptr [esi + 0x1ca], 0xc000
        mov    word ptr [esi], 2
        mov    dword ptr [ecx], 0            ; -> obj1:0x1078
        jmp    loc_4_261E
loc_4_2609:
        cmp    ebx, 8
        jb     loc_4_261E
loc_4_260E:
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [ebp - 8], 0xffffffff
        mov    dword ptr [eax], 0            ; -> loc_1_0FFC
loc_4_261E:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0x96
        push   eax
        call   sub_4_0280
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_2643:
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> loc_1_1018
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2660:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x32], ax
        mov    word ptr [esi + 0x34], ax
        mov    word ptr [esi], 2
        mov    al, byte ptr [esi + 0xc]
        mov    ecx, dword ptr [esi + 4]
        or     al, 8
        push   eax
        push   0x96
        push   ecx
        call   sub_4_0280
        push   0xff
        mov    ecx, dword ptr [esi + 4]
        push   0x94
        push   ecx
        call   sub_4_0280
        push   0x102
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        mov    bl, al
        mov    ecx, dword ptr [esi + 4]
        and    al, 0xfe
        mov    edi, 7
        push   eax
        push   0x102
        push   ecx
        call   sub_4_0280
loc_4_26C5:
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    loc_4_26C5
        or     bl, 1
        mov    eax, dword ptr [esi + 4]
        push   ebx
        push   0x102
        push   eax
        call   sub_4_0280
        push   0x102
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0x96
        push   eax
        call   sub_4_0280
        push   0x21
        mov    eax, dword ptr [esi + 0x80]
        push   eax
        call   sub_1_000C
loc_4_2711:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x80]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        push   esi
        call   sub_4_31F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2740:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x32], ax
        mov    word ptr [esi + 0x34], ax
        mov    ecx, dword ptr [esi + 0x80]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 4
        push   0x21
        mov    ecx, dword ptr [esi + 0x80]
        mov    byte ptr [ebp - 1], al
        push   ecx
        mov    edi, 0x1f3f
        call   sub_1_000C
loc_4_277E:
        mov    word ptr [esi], 4
        add    esp, 8
        push   esi
        call   sub_4_31F0
loc_4_278C:
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    loc_4_278C
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0xa8]
        push   ebx
        push   eax
        call   sub_1_000C
loc_4_27A9:
        add    esp, 8
        mov    eax, dword ptr [esi + 0xac]
        push   ebx
        push   eax
        call   sub_1_000C
loc_4_27B9:
        add    esp, 8
        mov    eax, dword ptr [esi + 0xb4]
        push   2
        push   eax
        call   sub_1_000C
loc_4_27CA:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x9c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0xa
        je     loc_4_282C
        push   0xa
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_000C
loc_4_27EF:
        mov    word ptr [esi + 0x30], bx
        add    esp, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 0x39]
        mov    cl, byte ptr [esi + eax*4 + 0xc7]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 0x39], cl
        mov    byte ptr [eax + 0xc7], bl
        mov    word ptr [eax + 0xc8], bx
        mov    al, byte ptr [esi + 0x38]
        test   al, al
        je     loc_4_2821
        dec    al
        mov    byte ptr [esi + 0x38], al
loc_4_2821:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_282C:
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2840:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    esi, esi
        mov    ebx, dword ptr [ebp + 0xc]
        lea    eax, [edi + 0x18]
        mov    dword ptr [edi + 0x14], esi
        cmp    ebx, esi
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], esi
        je     loc_4_2872
loc_4_285D:
        mov    eax, dword ptr [edi + 0x10]
        add    eax, esi
        add    esi, 6
        add    eax, 6
        push   eax
        push   edi
        call   sub_4_2880
        dec    ebx
        jne    loc_4_285D
loc_4_2872:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2880:
        push   ebp
        mov    ecx, 0xffffffff
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
loc_4_2898:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    eax, 8
loc_4_28A0:
        mov    edi, ecx
        mov    bl, dl
        shr    dl, 1
        and    edi, 0x80000000
        shr    edi, 0x1f
        and    ebx, 1
        xor    edi, ebx
        add    ecx, ecx
        test   edi, edi
        je     loc_4_28C3
        xor    ecx, 0x4c11db6
        or     ecx, edi
loc_4_28C3:
        dec    eax
        jne    loc_4_28A0
        dec    dword ptr [ebp - 4]
        jne    loc_4_2898
        shr    ecx, 0x1a
        mov    bl, 1
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 8]
        shr    edx, 3
        and    cl, 7
        shl    bl, cl
        pop    edi
        or     byte ptr [edx + eax + 0x18], bl
        pop    esi
        inc    dword ptr [eax + 0x14]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_28F0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x30], cx
        pop    ebp
        mov    word ptr [eax + 0x32], cx
        mov    byte ptr [eax + 0x38], cl
        mov    byte ptr [eax + 0x39], cl
        mov    byte ptr [eax + 0x3a], cl
        mov    byte ptr [eax + 0x3b], cl
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2920:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 0x32], bx
        test   al, 2
        mov    word ptr [esi + 0x36], bx
        mov    dword ptr [esi + 0xc2], eax
        je     loc_4_2945
        mov    bl, 4
loc_4_2945:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 4
        mov    dword ptr [ebp - 4], eax
        jne    loc_4_2956
        test   byte ptr [ebp + 0xc], 8
        je     loc_4_2959
loc_4_2956:
        or     bl, 8
loc_4_2959:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x10
        mov    dword ptr [ebp - 8], eax
        je     loc_4_2967
        or     bl, 0x1c
loc_4_2967:
        test   byte ptr [ebp + 0xc], 0x20
        je     loc_4_2970
        or     bl, 3
loc_4_2970:
        push   0x21
        mov    eax, dword ptr [esi + 0x80]
        push   eax
        call   sub_1_000C
loc_4_297E:
        add    esp, 8
        push   esi
        call   sub_4_31F0
        mov    ax, word ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0xb8]
        and    ax, 1
        cmp    ax, 1
        sbb    al, al
        add    al, 0x49
        push   eax
        push   ecx
        call   sub_1_000C
loc_4_29A4:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0xa8]
        push   0
        push   ecx
        call   sub_1_000C
loc_4_29B5:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0xac]
        push   0
        push   ecx
        call   sub_1_000C
loc_4_29C6:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0xb0]
        push   ebx
        push   ecx
        call   sub_1_000C
loc_4_29D6:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0xb4]
        push   2
        push   ecx
        call   sub_1_000C
loc_4_29E7:
        add    esp, 8
        mov    cl, byte ptr [esi + 0xe]
        mov    edx, dword ptr [esi + 0x84]
        push   ecx
        push   edx
        call   sub_1_000C
loc_4_29FA:
        add    esp, 8
        mov    cl, byte ptr [esi + 0xe]
        mov    edx, dword ptr [esi + 0x8c]
        push   ecx
        push   edx
        call   sub_1_000C
loc_4_2A0D:
        add    esp, 8
        mov    cl, byte ptr [esi + 0xf]
        mov    edx, dword ptr [esi + 0x88]
        push   ecx
        push   edx
        call   sub_1_000C
loc_4_2A20:
        add    esp, 8
        test   byte ptr [esi + 8], 2
        je     loc_4_2A6F
        push   2
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    al, byte ptr [esi + 0xf]
        mov    cl, byte ptr [esi + 0xe]
        and    eax, 0xffff00ff
        and    ecx, 0xffff00ff
        dec    ax
        mov    edx, dword ptr [esi + 0x78]
        shl    ax, 8
        add    ax, cx
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   edx
        push   ecx
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 0x48]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
loc_4_2A6F:
        push   0xff
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_000C
loc_4_2A80:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x80]
        xor    ebx, ebx
        push   0x61
        push   eax
        call   sub_1_000C
loc_4_2A93:
        add    esp, 8
        mov    edi, dword ptr [esi + 0x10]
loc_4_2A99:
        mov    al, byte ptr [edi]
        inc    edi
        push   eax
        mov    eax, dword ptr [esi + 0x84]
        add    ax, bx
        inc    ebx
        push   eax
        call   sub_1_000C
loc_4_2AAD:
        add    esp, 8
        cmp    ebx, 6
        jl     loc_4_2A99
        test   byte ptr [ebp + 0xc], 8
        jne    loc_4_2AEA
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_2AEA
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_2B09
        lea    edi, [esi + 0x18]
        xor    ebx, ebx
loc_4_2ACC:
        mov    al, byte ptr [edi]
        inc    edi
        push   eax
        mov    eax, dword ptr [esi + 0xa0]
        add    ax, bx
        inc    ebx
        push   eax
        call   sub_1_000C
loc_4_2AE0:
        add    esp, 8
        cmp    ebx, 8
        jl     loc_4_2ACC
        jmp    loc_4_2B09
loc_4_2AEA:
        xor    edi, edi
loc_4_2AEC:
        push   0xff
        mov    eax, dword ptr [esi + 0xa0]
        add    ax, di
        inc    edi
        push   eax
        call   sub_1_000C
loc_4_2B01:
        add    esp, 8
        cmp    edi, 8
        jl     loc_4_2AEC
loc_4_2B09:
        mov    al, byte ptr [esi + 0xe]
        mov    ecx, dword ptr [esi + 0x9c]
        inc    al
        push   eax
        push   ecx
        call   sub_1_000C
loc_4_2B1B:
        add    esp, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 0xe]
        push   2
        inc    eax
        mov    dword ptr [esi + 0x24], eax
        mov    ecx, dword ptr [esi + 0x80]
        push   ecx
        call   sub_1_000C
loc_4_2B35:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0xb4]
        push   0
        push   ecx
        call   sub_1_000C
loc_4_2B46:
        add    esp, 8
        mov    ecx, dword ptr [esi + 0x80]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 0xc2
        cmp    al, 2
        je     loc_4_2B6D
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2B6D:
        mov    word ptr [esi], 8
        cmp    byte ptr [esi + 0x38], 0
        je     loc_4_2B7E
        push   esi
        call   sub_4_2C40
loc_4_2B7E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2B90:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [edx], 8
        push   esi
        jne    loc_4_2BA9
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_2BA9:
        mov    esi, dword ptr [ebp + 0x14]
        xor    eax, eax
        add    esi, 0xff
        mov    al, byte ptr [edx + 0x3a]
        shr    esi, 8
        xor    ebx, ebx
        cmp    byte ptr [edx + 0x38], 0
        mov    bl, byte ptr [edx + eax*4 + 0xc7]
        je     loc_4_2C13
        xor    eax, eax
        mov    al, byte ptr [edx + 0x39]
        cmp    ebx, eax
        jbe    loc_4_2BFC
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0xe]
        sub    ecx, ebx
        cmp    ecx, esi
        jae    loc_4_2BF4
        cmp    esi, eax
        jbe    loc_4_2BF2
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 0x1e6]
        pop    ebp
        ret    0x10
loc_4_2BF2:
        xor    ebx, ebx
loc_4_2BF4:
        inc    dword ptr [edx + 0x1ea]
        jmp    loc_4_2C1B
loc_4_2BFC:
        sub    eax, ebx
        cmp    eax, esi
        jae    loc_4_2BF4
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 0x1e6]
        pop    ebp
        ret    0x10
loc_4_2C13:
        xor    ebx, ebx
        mov    byte ptr [edx + 0x3a], bl
        mov    byte ptr [edx + 0x39], bl
loc_4_2C1B:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    byte ptr [edx + 0x3b], bl
        push   ecx
        push   edx
        call   sub_1_001C
loc_4_2C2C:
        add    esp, 0xc
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2C40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x39]
        mov    ecx, dword ptr [esi + 0x90]
        push   eax
        xor    edi, edi
        push   ecx
        call   sub_1_000C
loc_4_2C5A:
        add    esp, 8
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x39]
        mov    eax, dword ptr [esi + 0x94]
        mov    di, word ptr [esi + ecx*4 + 0xc8]
        push   edi
        push   eax
        shr    edi, 8
        call   sub_1_000C
loc_4_2C7A:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x98]
        push   edi
        push   eax
        call   sub_1_000C
loc_4_2C8A:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x80]
        push   0x26
        push   eax
        call   sub_1_000C
loc_4_2C9B:
        add    esp, 8
        mov    word ptr [esi + 0x32], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2CB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_026C
loc_4_2CBD:
        add    esp, 4
        test   eax, eax
        jne    loc_4_2D31
        xor    ecx, ecx
        mov    dl, byte ptr [esi + 0x3b]
        mov    cx, word ptr [esi + 0x36]
        lea    eax, [ecx + 0xff]
        shr    eax, 8
        add    al, dl
        cmp    byte ptr [esi + 0xe], al
        ja     loc_4_2CDF
        xor    al, al
loc_4_2CDF:
        and    edx, 0xff
        mov    byte ptr [esi + edx*4 + 0xc7], al
        xor    eax, eax
        mov    al, byte ptr [esi + 0x3b]
        mov    word ptr [esi + eax*4 + 0xc8], cx
        mov    word ptr [esi + 0x36], 0
        mov    al, byte ptr [esi + 0x38]
        test   al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [esi + 0x38], cl
        jne    loc_4_2D1A
        mov    al, byte ptr [esi + 0x3b]
        push   esi
        mov    byte ptr [esi + 0x39], al
        call   sub_4_2C40
        jmp    loc_4_2D29
loc_4_2D1A:
        mov    cl, byte ptr [esi + 0x3b]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x3a]
        mov    byte ptr [esi + eax*4 + 0xc7], cl
loc_4_2D29:
        mov    al, byte ptr [esi + 0x3b]
        mov    byte ptr [esi + 0x3a], al
        xor    eax, eax
loc_4_2D31:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2D40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     loc_4_2E0C
        mov    ax, word ptr [esi + 0x32]
        test   ax, ax
        jle    loc_4_2E0C
        dec    ax
        mov    word ptr [esi + 0x32], ax
        test   ax, ax
        jg     loc_4_2E0C
        push   esi
        call   sub_4_2660
        inc    dword ptr [esi + 0x1ee]
        mov    ax, word ptr [esi + 0x30]
        inc    ax
        mov    word ptr [esi + 0x30], ax
        cmp    ax, 2
        jbe    loc_4_2DBC
        mov    word ptr [esi + 0x30], bx
        xor    eax, eax
        mov    al, byte ptr [esi + 0x39]
        mov    cl, byte ptr [esi + eax*4 + 0xc7]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 0x39], cl
        mov    byte ptr [eax + 0xc7], bl
        mov    word ptr [eax + 0xc8], bx
        mov    al, byte ptr [esi + 0x38]
        test   al, al
        je     loc_4_2DBA
        dec    al
        mov    byte ptr [esi + 0x38], al
loc_4_2DBA:
        mov    bl, 0x10
loc_4_2DBC:
        mov    eax, dword ptr [esi + 0xc2]
        push   eax
        push   esi
        call   sub_4_2920
        test   eax, eax
        je     loc_4_2DE9
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0xa
        push   0                             ; -> obj1:0x1084
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2DE9:
        test   bl, bl
        je     loc_4_2E0C
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10E0
        cmp    eax, -1
        jge    loc_4_2E0C
loc_4_2DFC:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10E0
        cmp    eax, -1
        jl     loc_4_2DFC
loc_4_2E0C:
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2E20:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 8
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x1d2
        rep stosd dword ptr es:[edi], eax
        mov    eax, 0x2c
        pop    edi
        pop    ebp
        ret    4
sub_4_2E40:
        push   ebp
        mov    ecx, 0xb
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x1c6
        mov    edi, dword ptr [ebp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_2E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        inc    dword ptr [esi + 0x1da]
        call   sub_4_2740
        push   2
        mov    eax, dword ptr [esi + 0xb4]
        push   eax
        call   sub_1_000C
loc_4_2E86:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x80]
        push   0x22
        push   eax
        call   sub_1_000C
loc_4_2E97:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x80]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        and    al, 0xc2
        cmp    al, 2
        je     loc_4_2ECF
        push   0
        mov    edi, dword ptr [ebp + 0xc]
        push   0
        push   0xc
        push   0                             ; -> loc_1_1098
        push   1
        push   edi
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2ECF:
        mov    word ptr [esi], 8
        xor    eax, eax
        mov    al, byte ptr [esi + 0xf]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0xe]
        xor    ebx, ebx
        sub    eax, ecx
        mov    edi, dword ptr [ebp + 0xc]
        inc    eax
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        je     loc_4_2FA9
loc_4_2EF1:
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_0328
loc_4_2EFB:
        add    esp, 4
        and    eax, 0xff
        cmp    eax, dword ptr [esi + 0x24]
        je     loc_4_2FA9
        cmp    dword ptr [esi + 0xc2], 0
        mov    eax, 2
        je     loc_4_2F20
        push   edi
        call   sub_4_32A0
loc_4_2F20:
        cmp    eax, 1
        jne    loc_4_2F4D
        mov    al, byte ptr [esi + 0x2d]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x24], ecx
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_4_2F3B
        mov    al, byte ptr [esi + 0xf]
        dec    al
loc_4_2F3B:
        push   eax
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        call   sub_1_000C
loc_4_2F48:
        add    esp, 8
        jmp    loc_4_2F9F
loc_4_2F4D:
        cmp    eax, 2
        jne    loc_4_2F9B
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_0328
loc_4_2F5C:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x24], ecx
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_4_2F72
        mov    al, byte ptr [esi + 0xf]
        dec    al
loc_4_2F72:
        push   eax
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        call   sub_1_000C
loc_4_2F7F:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x9c]
        push   5
        push   eax
        call   sub_1_000C
loc_4_2F90:
        add    esp, 8
        inc    dword ptr [esi + 0x1e2]
        jmp    loc_4_2F9F
loc_4_2F9B:
        test   eax, eax
        jl     loc_4_2FFE
loc_4_2F9F:
        inc    ebx
        cmp    dword ptr [ebp - 4], ebx
        ja     loc_4_2EF1
loc_4_2FA9:
        push   0x10
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_000C
loc_4_2FB7:
        add    esp, 8
        mov    eax, dword ptr [esi + 0xb4]
        push   0
        push   eax
        call   sub_1_000C
loc_4_2FC8:
        add    esp, 8
        cmp    byte ptr [esi + 0x38], 0
        je     loc_4_2FF1
        push   0xa
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_000C
loc_4_2FDF:
        add    esp, 8
        push   esi
        call   sub_4_2C40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2FF1:
        push   0
        push   edi
        call   sub_4_10E0
        cmp    eax, -1
        jl     loc_4_2FF1
loc_4_2FFE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3010:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        mov    byte ptr [ebp - 2], 0
        mov    edi, dword ptr [ebp + 8]
loc_4_302C:
        mov    eax, dword ptr [esi + 0x9c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0x10
        jne    loc_4_31C2
        push   5
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_000C
loc_4_3052:
        mov    ax, word ptr [esi + 2]
        add    esp, 8
        push   eax
        call   sub_1_0328
loc_4_305F:
        add    esp, 4
        and    eax, 0xff
        cmp    eax, dword ptr [esi + 0x24]
        je     loc_4_3110
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 1
        cmp    dword ptr [esi + 0xc2], 0
        mov    eax, 2
        je     loc_4_308C
        push   edi
        call   sub_4_32A0
loc_4_308C:
        cmp    eax, 1
        jne    loc_4_30B9
        mov    cl, byte ptr [esi + 0x2d]
        xor    eax, eax
        mov    al, cl
        dec    cl
        mov    dword ptr [esi + 0x24], eax
        cmp    byte ptr [esi + 0xe], cl
        jbe    loc_4_30A7
        mov    cl, byte ptr [esi + 0xf]
        dec    cl
loc_4_30A7:
        push   ecx
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        call   sub_1_000C
loc_4_30B4:
        add    esp, 8
        jmp    loc_4_3114
loc_4_30B9:
        cmp    eax, 2
        jne    loc_4_3107
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_0328
loc_4_30C8:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x24], ecx
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_4_30DE
        mov    al, byte ptr [esi + 0xf]
        dec    al
loc_4_30DE:
        push   eax
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        call   sub_1_000C
loc_4_30EB:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x9c]
        push   5
        push   eax
        call   sub_1_000C
loc_4_30FC:
        add    esp, 8
        inc    dword ptr [esi + 0x1e2]
        jmp    loc_4_3110
loc_4_3107:
        test   eax, eax
        jge    loc_4_3114
        jmp    loc_4_31CD
loc_4_3110:
        mov    byte ptr [ebp - 1], 1
loc_4_3114:
        mov    eax, dword ptr [esi + 0x9c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0xa
        je     loc_4_31B5
        mov    word ptr [esi + 0x32], 0
        mov    word ptr [esi + 0x30], 0
        push   0xa
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_000C
loc_4_3146:
        add    esp, 8
        mov    eax, dword ptr [esi + 0x90]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    bl, al
        xor    eax, eax
        and    bl, 0xf
        mov    al, byte ptr [esi + 0x39]
        mov    byte ptr [ebp - 2], 1
        mov    dl, byte ptr [esi + eax*4 + 0xc7]
        lea    ecx, [esi + eax*4]
        mov    byte ptr [esi + 0x39], dl
        mov    byte ptr [ecx + 0xc7], 0
        mov    word ptr [ecx + 0xc8], 0
        mov    al, byte ptr [esi + 0x38]
        test   al, al
        je     loc_4_3198
        dec    al
        mov    byte ptr [esi + 0x38], al
        je     loc_4_3198
        push   esi
        call   sub_4_2C40
loc_4_3198:
        push   ebx
        push   edi
        call   sub_4_10E0
        cmp    eax, -1
        jge    loc_4_31B1
loc_4_31A4:
        push   0
        push   edi
        call   sub_4_10E0
        cmp    eax, -1
        jl     loc_4_31A4
loc_4_31B1:
        xor    al, al
        jmp    loc_4_31B7
loc_4_31B5:
        mov    al, 1
loc_4_31B7:
        test   byte ptr [ebp - 1], al
        je     loc_4_302C
        jmp    loc_4_31CD
loc_4_31C2:
        push   edi
        push   esi
        call   sub_4_2E60
        mov    byte ptr [ebp - 2], 1
loc_4_31CD:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 2]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_31F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8], 2
        je     loc_4_321C
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    ax, 0xfffb
        mov    ecx, dword ptr [esi + 0x50]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_321C:
        push   0
        mov    eax, dword ptr [esi + 0xbc]
        push   eax
        call   sub_1_000C
loc_4_322A:
        add    esp, 8
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3240:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x1f
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xbc]
        push   eax
        call   sub_1_000C
loc_4_3255:
        add    esp, 8
        test   byte ptr [esi + 8], 2
        je     loc_4_327B
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        or     al, 4
        mov    ecx, dword ptr [esi + 0x50]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_327B:
        pop    esi
        pop    ebp
        ret    4
sub_4_3280:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_4_3293
        mov    eax, 0x3f
        pop    ebp
        ret    8
loc_4_3293:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xc2]
        ret    8
sub_4_32A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xb3]
        inc    eax
        mov    dword ptr [esi + 0xb3], eax
        cmp    eax, 0xa
        jbe    loc_4_32DD
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_32DD
        push   0
        push   esi
        call   sub_4_10E0
        cmp    eax, -1
        jne    loc_4_32DD
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_32DD:
        lea    edi, [esi + 0xbd]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0xfc
        push   edi
        push   ecx
        call   sub_1_0358
loc_4_32F5:
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     loc_4_3346
        add    eax, 3
        cmp    eax, 3
        ja     loc_4_3346
        jmp    dword ptr [eax*4]             ; -> obj4:0x339C
loc_4_330E:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3318:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3322:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_332F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_3650
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3346:
        sub    dword ptr [ebp - 4], 0xe
        mov    ecx, dword ptr [esi + 0x7f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     loc_4_3356
        mov    ecx, eax
loc_4_3356:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x1c], esi
        push   eax
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0xc], 0
        push   ecx
        inc    dword ptr [esi + 0x87]
        lea    ecx, [esi + 0xcb]
        lea    edx, [ebp - 0x1c]
        mov    byte ptr [esi + 0x41], 1
        push   ecx
        mov    eax, dword ptr [esi + 0x7b]
        push   0xe
        push   edi
        push   edi
        push   edx
        push   eax
        call   sub_1_08B0
loc_4_338A:
        cmp    dword ptr [ebp - 0x18], 0
        sbb    eax, eax
        pop    edi
        inc    eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        ; ---- dispatch table (4 entries) ----
        dd  loc_4_330E
        dd  loc_4_3318
        dd  loc_4_3322
        dd  loc_4_332F
        db  08Dh, 064h, 024h, 000h
loc_4_33B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    ecx, dword ptr [ebp + 0x1c]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ecx], 0
        mov    dword ptr [esi + 4], 0
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_4_33FB
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        add    edx, 0xe
        mov    ecx, dword ptr [ebp + 0x14]
        push   edx
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_0D4E
loc_4_33F0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_4_33FB:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [ebp + 0x10]
        lea    edi, [ecx + edx]
        cmp    edi, eax
        jbe    loc_4_3412
        sub    eax, edx
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_4_3415
loc_4_3412:
        mov    dword ptr [ebp - 0x18], ecx
loc_4_3415:
        mov    edi, dword ptr [ebp - 0x18]
        add    edx, 0xe
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    dword ptr [ebp - 8], edx
        push   0
        push   ecx
        call   sub_1_0D00
loc_4_3431:
        test   edi, edi
        je     loc_4_3487
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        call   sub_1_0D24
loc_4_3448:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    ebx, eax
        jb     loc_4_3451
        mov    ebx, eax
loc_4_3451:
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_0690
loc_4_3467:
        add    esp, 0x14
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     loc_4_3487
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_0D48
loc_4_3481:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_3431
loc_4_3487:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, edi
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h
sub_4_34A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        je     loc_4_34C2
loc_4_34AE:
        mov    ecx, dword ptr [eax + 0x24]
        push   eax
        push   esi
        mov    dword ptr [esi + 0x38], ecx
        call   sub_4_34D0
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        jne    loc_4_34AE
loc_4_34C2:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_34D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_0D00
loc_4_34EC:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 0x7f]
        add    ebx, 0xe
        cmp    ebx, eax
        jb     loc_4_34FE
        mov    ebx, eax
loc_4_34FE:
        lea    eax, [ebx - 0xe]
        push   0
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 4], 0xe
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_0D00
loc_4_3518:
        lea    ecx, [ebp - 0x14]
        lea    edx, [ebp - 0x10]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        lea    ecx, [esi + 0xbd]
        mov    dword ptr [ebp - 0x10], 0
        push   eax
        push   ebx
        push   ecx
        call   sub_4_35A0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [esi + 0xcb]
        push   eax
        mov    dword ptr [ebp - 0x28], esi
        push   edx
        mov    dword ptr [ebp - 0x20], ecx
        push   0xe
        lea    ecx, [esi + 0xbd]
        push   ecx
        lea    edx, [ebp - 0x28]
        push   ecx
        mov    dword ptr [ebp - 0x18], edi
        push   edx
        mov    ecx, dword ptr [esi + 0x7b]
        push   ecx
        call   sub_1_08B0
loc_4_3567:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    byte ptr [esi + 0x41], 1
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0C4A
loc_4_3580:
        push   esi
        call   sub_4_1760
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_35A0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        lea    edx, [ebp - 4]
        push   ecx
        mov    esi, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0D24
loc_4_35C6:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x14]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     loc_4_3644
loc_4_35D3:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [eax]
        lea    edx, [ecx + ebx]
        cmp    edx, dword ptr [ebp + 0xc]
        jae    loc_4_361E
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_0DBC
loc_4_35F0:
        mov    ecx, dword ptr [ebp + 0x18]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_0D48
loc_4_35FE:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0D24
loc_4_3614:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    loc_4_35D3
loc_4_361E:
        cmp    dword ptr [esi], 0
        je     loc_4_3644
        mov    eax, dword ptr [ebp + 0x18]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [eax]
        sub    esi, eax
        add    ecx, dword ptr [ebp - 4]
        push   esi
        add    eax, dword ptr [ebp + 8]
        push   ecx
        push   eax
        call   sub_1_0DBC
loc_4_363D:
        mov    ecx, dword ptr [ebp + 0x18]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_3644:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_3650:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        test   eax, 0x208
        jne    loc_4_369C
        test   eax, 0x206
        je     loc_4_3678
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        inc    dword ptr [eax + 0x9b]
        ret    8
loc_4_3678:
        test   eax, 0x204
        je     loc_4_368C
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        ret    8
loc_4_368C:
        test   eax, 0x202
        je     loc_4_369C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x9b]
loc_4_369C:
        pop    ebp
        ret    8
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x57  RW-
        db  02Dh, 03Fh, 063h, 06Fh, 06Dh, 06Dh, 065h, 06Eh, 074h, 03Ah, 022h, 05Fh, 048h, 050h, 05Fh, 020h
        db  068h, 070h, 069h, 06Dh, 02Eh, 063h, 028h, 054h, 068h, 075h, 020h, 053h, 065h, 070h, 020h, 032h
        db  033h, 020h, 031h, 038h, 03Ah, 030h, 031h, 03Ah, 035h, 034h, 020h, 031h, 039h, 039h, 033h, 029h
        db  020h, 044h, 02Eh, 030h, 02Eh, 034h, 020h, 063h, 06Fh, 06Dh, 070h, 06Ch, 069h, 065h, 064h, 028h
        db  053h, 065h, 070h, 020h, 032h, 037h, 020h, 031h, 039h, 039h, 034h, 020h, 031h, 034h, 03Ah, 033h
        db  033h, 03Ah, 030h, 039h, 029h, 022h, 020h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x18  RW-
        db  003h, 004h, 005h, 007h, 009h, 00Ah, 00Bh, 00Ch, 000h, 004h, 000h, 014h, 000h, 024h, 000h, 034h
        db  000h, 044h, 000h, 054h, 000h, 064h, 000h, 074h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0x4  RW-
        db  04Ch, 001h, 005h, 031h
VxD__LDATA ENDS
