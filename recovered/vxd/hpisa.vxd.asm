; HPISA -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 8149 instructions recovered by recursive descent

Declare_Virtual_Device HPISA, 3, 10, HPISA_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1236  R-X
        db  052h, 00Fh, 0B7h, 054h, 024h, 008h, 033h, 0C0h, 0ECh, 05Ah, 0C3h, 090h, 052h, 00Fh, 0B7h, 054h
        db  024h, 008h, 08Ah, 044h, 024h, 00Ch, 0EEh, 0E4h, 061h, 05Ah, 0C3h, 090h
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
        je     loc_1_0071
        xor    dl, 8
        movzx  eax, byte ptr [edi + 0x3b]
        xchg   al, ah
        out    dx, ax
        xor    dl, 4
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        xor    eax, eax
        jmp    loc_1_00C4
loc_1_0071:
        xor    dl, 0x1a
        mov    al, 4
        out    dx, al
        xor    dl, 2
        out    dx, al
        xor    dl, 8
        mov    al, 0xa
        out    dx, al
        push   ecx
        mov    ecx, 0x40
        xor    dl, 8
loc_1_008A:
        in     al, dx
        cmp    al, 4
        loope  loc_1_008A
        pop    ecx
        xor    eax, eax
        xor    dl, 0
        out    dx, al
        movzx  eax, byte ptr [edi + 0x3b]
        xor    dl, 1
        out    dx, al
        mov    ax, cx
        and    al, 0xfe
        xor    dl, 3
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0xb
        mov    al, 0x12
        out    dx, al
        xor    dl, 0x1c
        cld    
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        call   sub_1_068B
        jne    loc_1_00CA
loc_1_00C4:
        pop    edi
        pop    ecx
        pop    edx
        pop    esi
        leave  
        ret    
loc_1_00CA:
        mov    word ptr [edi + 0x34], 0
        mov    word ptr [edi + 0x36], 0
        jmp    loc_1_00C4
sub_1_00D8:
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
        je     loc_1_01D7
        movzx  ebx, word ptr [edi + 0x34]
        or     ebx, ebx
        je     loc_1_0101
        dec    ecx
        inc    eax
loc_1_0101:
        and    ax, 0xfffe
        je     loc_1_01E0
        add    word ptr [edi + 0x36], ax
        test   byte ptr [edi + 8], 2
        je     loc_1_0194
        test   byte ptr [edi + 8], 4
        je     loc_1_0164
        push   edi
        mov    edi, dword ptr [edi + 0x20]
        xor    dl, 4
        in     ax, dx
        and    ax, 0xefff
        out    dx, ax
        push   eax
        or     ebx, ebx
        je     loc_1_013B
        mov    bh, byte ptr [esi]
        inc    esi
        mov    word ptr [edi], bx
        xor    ebx, ebx
loc_1_013B:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_0146
        cld    
        nop    
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_0146:
        test   al, 2
        je     loc_1_014C
        movsw  word ptr es:[edi], word ptr [esi]
loc_1_014C:
        test   al, 1
        je     loc_1_0154
        mov    bh, 0xff
        mov    bl, byte ptr [esi]
loc_1_0154:
        pop    eax
        or     ax, 0x1000
        out    dx, ax
        pop    edi
        mov    word ptr [edi + 0x34], bx
        xor    eax, eax
        jmp    loc_1_01D7
loc_1_0164:
        xor    dl, 0xc
        or     ebx, ebx
        je     loc_1_0174
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_0174:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_017E
        cld    
        rep outsd dx, dword ptr [esi]
loc_1_017E:
        test   al, 2
        je     loc_1_0184
        outsw  dx, word ptr [esi]
loc_1_0184:
        test   al, 1
        je     loc_1_018C
        mov    bh, 0xff
        mov    bl, byte ptr [esi]
loc_1_018C:
        mov    word ptr [edi + 0x34], bx
        xor    eax, eax
        jmp    loc_1_01D7
loc_1_0194:
        xor    dl, 0x1a
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0xb
        mov    al, 0x12
        out    dx, al
        xor    dl, 0x1c
        or     ebx, ebx
        je     loc_1_01B4
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_01B4:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_01BE
        cld    
        rep outsd dx, dword ptr [esi]
loc_1_01BE:
        test   al, 2
        je     loc_1_01C4
        outsw  dx, word ptr [esi]
loc_1_01C4:
        test   al, 1
        je     loc_1_01CC
        mov    bh, 0xff
        mov    bl, byte ptr [esi]
loc_1_01CC:
        mov    word ptr [edi + 0x34], bx
        call   sub_1_068B
        jne    loc_1_01EC
loc_1_01D7:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave  
        ret    
        db  08Bh, 0C0h
loc_1_01E0:
        mov    ah, 0xff
        mov    al, byte ptr [esi]
        mov    word ptr [edi + 0x34], ax
        xor    eax, eax
        jmp    loc_1_01D7
loc_1_01EC:
        mov    word ptr [edi + 0x34], 0
        mov    word ptr [edi + 0x36], 0
        jmp    loc_1_01D7
        db  08Bh, 0C0h
sub_1_01FC:
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
        je     loc_1_028F
        xchg   word ptr [edi + 0x34], bx
        or     ebx, ebx
        je     loc_1_021C
        inc    ecx
        inc    eax
loc_1_021C:
        cmp    ecx, 0x3c
        jge    loc_1_022B
        sub    ecx, 0x3c
        sub    eax, ecx
        mov    ecx, 0x3c
loc_1_022B:
        or     eax, eax
        je     loc_1_028F
        inc    eax
        and    al, 0xfe
        mov    word ptr [edi + 0x36], cx
        movzx  ecx, ax
        movzx  edx, word ptr [edi + 2]
        test   byte ptr [edi + 8], 2
        je     loc_1_025D
        xor    dl, 0xc
        shr    ecx, 1
        or     ebx, ebx
        je     loc_1_0255
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_0255
        jmp    loc_1_028F
loc_1_0255:
        xor    eax, eax
loc_1_0257:
        out    dx, ax
        loop   loc_1_0257
        jmp    loc_1_028F
loc_1_025D:
        xor    dl, 0x1a
        movzx  eax, cx
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0xb
        mov    al, 0x12
        out    dx, al
        xor    dl, 0x1c
        shr    ecx, 1
        or     ebx, ebx
        je     loc_1_0282
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_0282
        jmp    loc_1_0288
loc_1_0282:
        xor    eax, eax
loc_1_0284:
        out    dx, ax
        loop   loc_1_0284
loc_1_0288:
        call   sub_1_068B
        jne    loc_1_0291
loc_1_028F:
        xor    eax, eax
loc_1_0291:
        pop    ebx
        pop    ecx
        pop    edx
        pop    edi
        leave  
        ret    
        db  090h
sub_1_0298:
        push   edx
        movzx  edx, word ptr [esp + 8]
        xor    dl, 0x10
        mov    al, 0x62
        out    dx, al
        xor    dl, 7
        in     al, dx
        xchg   ah, al
        xor    dl, 7
        mov    al, 0x22
        out    dx, al
        movzx  eax, ah
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_02B8:
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
        je     loc_1_02F7
        xor    dl, 0xa
        out    dx, ax
        xor    dl, 6
        in     eax, dx
        mov    dword ptr [esi + 0x2c], eax
        shr    cx, 1
        rep insw word ptr es:[edi], dx
        jae    loc_1_033B
        in     ax, dx
        mov    byte ptr [edi], al
        jmp    loc_1_033B
loc_1_02F7:
        xor    dl, 0x18
        out    dx, al
        xchg   ah, al
        xor    dl, 1
        out    dx, al
        xor    dl, 3
        mov    ax, cx
        add    ax, 4
        inc    ax
        and    al, 0xfe
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0xb
        mov    al, 0xa
        out    dx, al
        xor    dl, 0x1c
        in     eax, dx
        mov    dword ptr [esi + 0x2c], eax
        cld    
        shr    cx, 1
        rep insw word ptr es:[edi], dx
        jae    loc_1_0330
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_0330:
        call   sub_1_068B
        jne    loc_1_04F2
loc_1_033B:
        movzx  eax, word ptr [esi + 0x2c]
        test   al, 0x48
        jne    loc_1_0523
        test   al, 1
        je     loc_1_0406
loc_1_034F:
        shr    ax, 8
        mov    ecx, dword ptr [esi + 0x24]
        sub    ax, cx
        dec    ax
        jns    loc_1_036A
        mov    al, byte ptr [esi + 0xf]
        sub    al, cl
        add    al, byte ptr [esi + 0x2d]
        sub    al, byte ptr [esi + 0xe]
        dec    al
loc_1_036A:
        shl    ax, 8
        mov    al, byte ptr [esi + 0x2e]
        cmp    al, 0xfc
        jbe    loc_1_0377
        dec    ah
loc_1_0377:
        sub    ax, 4
        mov    word ptr [esi + 0x2e], ax
        or     ebx, ebx
        jne    loc_1_03B0
        cmp    ax, 0x5ea
        ja     loc_1_044A
        cmp    ax, 0x3c
        jb     loc_1_044A
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi]
        movzx  edx, word ptr [edi + 4]
        mov    edi, dword ptr [esi + 0x10]
        test   eax, 1
        jne    loc_1_0469
        jmp    loc_1_03C0
loc_1_03B0:
        movzx  eax, word ptr [esi + 0x2e]
loc_1_03B4:
        mov    edi, dword ptr [ebp + 0x14]
        mov    dword ptr [edi], ebx
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave  
        ret    
loc_1_03C0:
        cmp    eax, dword ptr [edi]
        jne    loc_1_03EE
        cmp    dx, word ptr [edi + 4]
        jne    loc_1_03EE
        test   dword ptr [esi + 0xc2], 1
        jne    loc_1_03B0
        mov    ebx, 0x100
        test   dword ptr [esi + 0xc2], 0x10
        jne    loc_1_03B0
        mov    eax, 0
        jmp    loc_1_03B4
loc_1_03EE:
        mov    ebx, 0x104
        test   dword ptr [esi + 0xc2], 0x10
        jne    loc_1_03B0
        mov    eax, 0
        jmp    loc_1_03B4
loc_1_0406:
        mov    ebx, 0x206
        and    eax, 6
        cmp    eax, 6
        je     loc_1_0430
        mov    ebx, 0x202
        test   eax, 2
        jne    loc_1_0430
        mov    ebx, 0x204
        test   eax, 4
        jne    loc_1_0430
        mov    ebx, 0x201
loc_1_0430:
        test   dword ptr [esi + 0xc2], 0x20
        jne    loc_1_034F
        mov    eax, 0
        jmp    loc_1_03B4
loc_1_044A:
        mov    ebx, 0x208
        test   dword ptr [esi + 0xc2], 0x20
        jne    loc_1_03B0
        mov    eax, 0
        jmp    loc_1_03B4
loc_1_0469:
        cmp    eax, -1
        jne    loc_1_04A8
        cmp    dx, -1
        jne    loc_1_04A8
        mov    ebx, 1
        test   dword ptr [esi + 0xc2], 2
        jne    loc_1_03B0
        mov    ebx, 0x101
        test   dword ptr [esi + 0xc2], 0x10
        jne    loc_1_03B0
        mov    eax, 0
        jmp    loc_1_03B4
loc_1_04A8:
        mov    ecx, dword ptr [esi + 0x14]
        jecxz  loc_1_04CF
loc_1_04AD:
        add    edi, 6
        cmp    eax, dword ptr [edi]
        jne    loc_1_04EE
        cmp    dx, word ptr [edi + 4]
        jne    loc_1_04EE
        mov    ebx, 2
        test   dword ptr [esi + 0xc2], 4
        jne    loc_1_03B0
loc_1_04CF:
        mov    ebx, 0x102
        test   dword ptr [esi + 0xc2], 0x18
        jne    loc_1_03B0
        mov    eax, 0
        jmp    loc_1_03B4
loc_1_04EE:
        loop   loc_1_04AD
        jmp    loc_1_04CF
loc_1_04F2:
        inc    dword ptr [esi + 0x1e2]
        and    dl, 0xe0
        push   edx
        call   sub_1_0298
        add    esp, 4
        movzx  eax, al
        mov    dword ptr [esi + 0x24], eax
        dec    eax
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_1_0515
        movzx  eax, byte ptr [esi + 0xf]
        dec    eax
loc_1_0515:
        xor    dl, 0x13
        out    dx, al
        mov    eax, 0xfffffffe
        jmp    loc_1_03B4
loc_1_0523:
        inc    dword ptr [esi + 0x1de]
        and    dl, 0xe0
        xor    dl, 0x1a
        mov    al, 0xff
        out    dx, al
        xor    dl, 1
        mov    al, 0xff
        out    dx, al
        mov    al, byte ptr [esi + 0xe]
        xor    dl, 8
        out    dx, al
        xor    dl, 3
        mov    al, 0x61
        out    dx, al
        movzx  eax, byte ptr [esi + 0xe]
        inc    al
        mov    dword ptr [esi + 0x24], eax
        xor    dl, 7
        out    dx, al
        xor    dl, 7
        mov    al, 0x22
        out    dx, al
        xor    dl, 7
        mov    al, 5
        out    dx, al
        xor    dl, 7
        in     al, dx
        and    al, 0xc2
        mov    eax, 0xfffffffd
        cmp    al, 2
        je     loc_1_03B4
        mov    eax, 0xffffffff
        jmp    loc_1_03B4
        db  090h
sub_1_057C:
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
        jl     loc_1_05B4
        shl    edx, 8
        sub    ecx, edx
        movzx  eax, byte ptr [esi + 0xe]
        shl    eax, 8
        add    ecx, eax
loc_1_05B4:
        mov    eax, ecx
        movzx  edx, word ptr [esi + 2]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        test   byte ptr [esi + 8], 2
        je     loc_1_05CD
        xor    dl, 0xa
        out    dx, ax
        jmp    loc_1_0600
loc_1_05CD:
        xor    dl, 0x18
        out    dx, al
        xchg   ah, al
        xor    dl, 1
        out    dx, al
        mov    eax, ecx
        test   bl, 1
        je     loc_1_05E9
        inc    ax
        test   cl, 1
        jne    loc_1_05F0
        inc    ax
        jmp    loc_1_05F0
loc_1_05E9:
        test   cl, 1
        je     loc_1_05F0
        inc    ax
loc_1_05F0:
        xor    dl, 3
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0xb
        mov    al, 0xa
        out    dx, al
loc_1_0600:
        and    dl, 0xe0
        test   ebx, 1
        je     loc_1_0618
        dec    cx
        xor    dl, 0xc
        in     ax, dx
        mov    byte ptr [edi], ah
        inc    edi
        and    dl, 0xe0
loc_1_0618:
        push   ecx
        test   byte ptr [esi + 8], 2
        je     loc_1_0654
        test   byte ptr [esi + 8], 4
        je     loc_1_0646
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
        jmp    loc_1_065E
loc_1_0646:
        xor    dl, 0xc
        cld    
        shr    ecx, 2
        mov    eax, eax
        rep insd dword ptr es:[edi], dx
        jmp    loc_1_065E
loc_1_0654:
        xor    dl, 0xc
        cld    
        shr    ecx, 2
        nop    
        rep insd dword ptr es:[edi], dx
loc_1_065E:
        pop    ecx
        test   cx, 2
        je     loc_1_0668
        insw   word ptr es:[edi], dx
loc_1_0668:
        test   cx, 1
        je     loc_1_0673
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_0673:
        test   byte ptr [esi + 8], 2
        jne    loc_1_0682
        push   edx
        call   sub_1_068B
        pop    edx
        jne    loc_1_0684
loc_1_0682:
        xor    eax, eax
loc_1_0684:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave  
        ret    
sub_1_068B:
        push   ecx
        xor    dl, 0x1b
        mov    ecx, 0x1a
loc_1_0694:
        in     al, dx
        and    al, 0x40
        loope  loc_1_0694
        je     loc_1_06A2
        mov    al, 0x40
        out    dx, al
        pop    ecx
        xor    eax, eax
        ret    
loc_1_06A2:
        push   edx
        xor    dl, 7
        mov    al, 0x22
        out    dx, al
        pop    edx
        mov    al, 0x40
        out    dx, al
        pop    ecx
        mov    eax, 0xfffffffe
        or     eax, eax
        ret    
        db  000h, 000h
loc_1_06B8:
        mov    al, byte ptr [0xd18]          ; -> obj1:0x0D18
        cmp    al, 0
        jne    loc_1_06DD
        inc    byte ptr [0xd18]              ; -> obj1:0x0D18
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_06DF
        push   0
        push   0
        call   sub_4_1BB0
        cmp    eax, 0
        jne    loc_1_06DF
loc_1_06DD:
        clc    
        ret    
loc_1_06DF:
        stc    
        ret    
HPISA_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_06F0
        jmp    dword ptr [eax*4 + 0x6f2]     ; -> obj1:0x06F2
loc_1_06F0:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_06B8
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06F0
        dd  loc_1_06B8
        db  000h, 000h
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
        call   sub_2_07C0
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
        call   sub_1_0B8C
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
        call   sub_1_0B8C
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
sub_1_0B06:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0B12:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0B18:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_0B1E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_0B24:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_0B62:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_0B6E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_0B74:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_0B7A:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_0B80:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_0B86:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_0B8C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0BB0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_0BB6:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_0BBC:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_0BC2:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_0BCE:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0BD4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0BF8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_0BFE:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0C04:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_0C0A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0C10:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
sub_1_0C34:
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
sub_1_0C3A:
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
sub_1_0C40:
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
loc_1_0C46:
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
sub_1_0C4C:
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
sub_1_0C52:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_0C58:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_0C6C:
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
sub_1_0C8C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_0CAC
        or     ecx, ecx
        je     loc_1_0CC2
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_0CC0
loc_1_0CAC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_0CC0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_0CC0:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0CC2:
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
        db  'HPISA   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  HPISA_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Dh, 061h, 078h, 04Dh, 075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0D30:
        dec    ebp
        popal  
        js     0xd81
        jne    loc_1_0DA2
        je     0xda1
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_0D3D
loc_1_0D3D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], ch
        popal  
        jb     0xdac
        jae    0xdbc
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], ch
        popal  
        jb     0xdb8
        jae    loc_1_0DC8
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], ch
        je     loc_1_0DC1
        jb     0xdd0
        jne    0xdd0
        je     loc_1_0D62
loc_1_0D62:
        add    byte ptr [eax], al
loc_1_0D64:
        imul   ebp, dword ptr [esi + 0x74], 0x75727265
        jo     loc_1_0DE1
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    0xdc2
        jne    loc_1_0DE3
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        db  000h
loc_1_0D7C:
        inc    edx
        jne    loc_1_0DF2
        dec    esi
        jne    loc_1_0DEF
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0E07
        dec    ebp
        popal  
        jo     0xe02
        inc    edx
        popal  
        jae    loc_1_0DFD
        inc    ecx
        jb     0xe02
        jae    loc_1_0E12
        add    byte ptr [ebp + 0x65], cl
loc_1_0DA2:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0E1F
        dec    ebp
        popal  
        jo     0xe1a
        inc    edx
        popal  
        jae    0xe15
        inc    ecx
        jb     0xe1a
        jae    loc_1_0E2A
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
loc_1_0DC1:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
        add    dword ptr [eax], eax
loc_1_0DC8:
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
loc_1_0DE1:
        add    dword ptr [ecx], eax
loc_1_0DE3:
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
loc_1_0DEF:
        add    byte ptr [ecx + eax], cl
loc_1_0DF2:
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
loc_1_0DFD:
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
loc_1_0E07:
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
loc_1_0E12:
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
loc_1_0E1F:
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
loc_1_0E2A:
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_0E34:
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
loc_1_0E90:
        dec    eax
        push   eax
        and    byte ptr [eax + 0x43], dl
        and    byte ptr [ecx + eax*2 + 0x4e], cl
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     loc_1_0F13
        jb     0xed0
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 049h, 053h, 041h, 000h, 000h, 000h
loc_1_0EB0:
        dec    eax
        push   eax
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x50], cl
        das    
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x50], cl
        das    
        xor    dword ptr [esi], esi
        and    byte ptr [eax + edx*2], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x50], cl
        das    
        xor    dword ptr [esi], esi
        and    byte ptr [eax + 0x6c], dl
        jne    0xf49
        add    byte ptr [eax], al
loc_1_0ED8:
        dec    eax
        push   eax
        das    
        xor    dword ptr [esi], esi
        and    byte ptr [eax + 0x6c], dl
        jne    loc_1_0F55
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0xf62
        cmp    eax, 0x3f3f7830
        aas    
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [esi + 0x4f], cl
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
        popal  
        jo     loc_1_0F75
        jb     loc_1_0F24
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_1_0F28
        jo     loc_1_0F7C
        jae    0xf72
        outsb  dx, byte ptr [esi]
        je     loc_1_0F10
loc_1_0F10:
        inc    ebp
        push   edx
        push   edx
loc_1_0F13:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    loc_1_0F7C
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x49], 0x61204f2f
loc_1_0F24:
        jb     0xf8d
loc_1_0F28:
        jae    0xf9d
        sub    byte ptr [ebp + 0x73], ah
        sub    dword ptr [eax], eax
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    loc_1_0F9C
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x69], 0x7265746e
        jb     loc_1_0FBB
        jo     0xfbc
        and    byte ptr [esi + 0x65], dh
        arpl   word ptr [edi + ebp*2 + 0x72], si
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
loc_1_0F55:
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0xfc0
        jb     loc_1_0FCF
        jne    loc_1_0FCF
        je     0xf81
        jbe    0xfc8
        arpl   word ptr [edi + ebp*2 + 0x72], si
        and    byte ptr [ebp + 0x69], ch
        jae    loc_1_0FD9
        popal  
        je     0xfd2
        push   0
loc_1_0F74:
        inc    ebp
loc_1_0F75:
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
loc_1_0F7C:
        popal  
        jo     0xff4
        jb     0xfa3
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xfa7
        jae    0xffe
        jo     loc_1_0FFB
        outsd  dx, dword ptr [esi]
        jb     loc_1_1002
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_0F94:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_0F9C:
        je     loc_1_100D
        jb     loc_1_1005
        and    byte ptr fs:[ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     0x1016
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x6f], ah
        jb     loc_1_1026
        jne    loc_1_1026
        je     0x101d
        add    byte ptr fs:[eax], al
loc_1_0FBB:
        add    byte ptr [eax], al            ; -> obj1:0x0EB8
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0EC0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0ECC
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0ED8
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0EF4
        add    byte ptr [eax], al
loc_1_0FCF:
        add    byte ptr [eax], al            ; -> loc_1_0F10
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0F30
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0F50
loc_1_0FD9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0F74
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0F94
        add    byte ptr [eax], al
        db  000h
loc_1_0FE4:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        popal  
        bound  esp, qword ptr [ebx + 0x64]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        push   ebp
        jae    0x1064
loc_1_0FFB:
        outsb  dx, byte ptr [esi]
        and    byte ptr [di + 0x78], ah
        je     0x1067
loc_1_1002:
        jb     0x1072
        popal  
loc_1_1005:
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    0x1070
loc_1_100D:
        imul   esi, dword ptr gs:[esi + 0x65], 0x55000072
        jae    0x1080
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x6e], ch
        je     0x1083
        jb     0x108e
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + esi*2 + 0x61], dh
loc_1_1026:
        outsb  dx, byte ptr [esi]
        jae    0x108c
        db  065h, 069h, 076h, 065h, 072h, 000h, 000h
loc_1_1030:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_1_10A8
        and    byte ptr [eax + 0x65], dh
        jb     0x10a4
        outsd  dx, dword ptr [esi]
        jb     0x10ae
        and    byte ptr [ecx + eax*2 + 0x4e], cl
        and    byte ptr [eax + 0x6f], dh
        jb     0x10be
        and    byte ptr [ecx + 0x75], ah
        je     0x10be
        sub    eax, 0x736e6573
        add    byte ptr gs:[eax], al
        add    byte ptr [ecx + 0x75], al
        je     0x10cb
        sub    eax, 0x736e6573
        and    byte ptr gs:[ecx + eax*2 + 0x4e], cl
        and    byte ptr [eax + 0x6f], dh
        jb     0x10df
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x6556202e
        jb     loc_1_10E1
        jns    0x109b
        arpl   word ptr [ecx + 0x62], sp
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x67], 0x654d002e
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x1103
        and    byte ptr [ebp + 0x61], ch
        jo     loc_1_10AF
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x10b3
        arpl   word ptr [edi + 0x6e], bp
        imul   sp, word ptr [edi + 0x75], 0x6572
        cmp    esp, dword ptr fs:[eax]
        jne    loc_1_1114
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
loc_1_10A8:
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    0x1115
loc_1_10AF:
        jb     0x1125
        add    byte ptr [eax], al
loc_1_10B4:
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_1133
        and    byte ptr [ebp + 0x61], ch
        jo     0x10df
        popal  
        jb     loc_1_1129
        jae    loc_1_1139
        and    byte ptr [edi + ebp*2 + 0x65], ah
        jae    loc_1_10EC
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x10f0
        insd   dword ptr es:[edi], dx
        popal  
        je     loc_1_1137
        push   0x61646120
        jo     loc_1_114F
        jb     0x1119
        and    byte ptr [ebp + 0x73], dh
loc_1_10E1:
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
loc_1_10EC:
        outsb  dx, byte ptr [esi]
        jae    0x1155
        jb     loc_1_1165
        add    byte ptr [eax], al
loc_1_10F4:
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x1173
        and    byte ptr [ebp + 0x61], ch
        jo     0x111f
        jb     loc_1_1162
        outsb  dx, byte ptr [esi]
        and    byte ptr gs:[di + 0x75], ch
        jae    0x117d
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [edx], dh
        xor    byte ptr [eax + edi], dh
        and    byte ptr [edx + 0x79], ah
loc_1_1114:
        je     loc_1_117B
        jae    0x1153
        and    byte ptr [ebp + 0x73], dh
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    0x118f
loc_1_1129:
        jb     0x119f
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1130:
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
loc_1_1133:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x1157
loc_1_1137:
        insd   dword ptr es:[edi], dx
        popal  
loc_1_1139:
        jo     0x115b
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_11BA
        and    byte ptr [ecx + 0x6e], ch
        je     0x11b5
        and    byte ptr [ebx + 0x65], ch
        jb     0x11b9
        insb   byte ptr es:[edi], dx
        cmp    esp, dword ptr [eax]
loc_1_114F:
        jne    loc_1_11C4
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    loc_1_11C5
        jb     loc_1_11D5
loc_1_1162:
        add    byte ptr [eax], al
loc_1_1164:
        dec    ebp
loc_1_1165:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_11E3
        and    byte ptr [ebp + 0x61], ch
        jo     0x118f
        arpl   word ptr [edi + 0x6e], bp
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x7375203b
loc_1_117B:
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    loc_1_11EF
        jb     loc_1_11FF
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1190:
        or     byte ptr [eax], al
        or     dword ptr [eax], eax
loc_1_1194:
        inc    esi
        inc    ecx
        push   esp
        inc    ecx
        dec    esp
        cmp    ah, byte ptr [eax]
        inc    edx
        popal  
        and    byte ptr fs:[eax + 0x2f], cl
        push   edi
        and    byte ptr [ecx + 0x6e], ch
        imul   esi, dword ptr [eax + 0x63], 0x65646f
        add    byte ptr [eax], al
        db  000h
loc_1_11B0:
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
loc_1_11BA:
        jb     loc_1_1221
        jae    loc_1_1232
        popal  
        jb     0x1235
        add    byte ptr cs:[eax], al
loc_1_11C4:
        inc    esi
loc_1_11C5:
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jb     0x1235
        jae    0x1246
        popal  
        jb     0x1249
loc_1_11D5:
        add    byte ptr cs:[eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11E3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11ED:
        add    byte ptr [eax], al
loc_1_11EF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11FF:
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
loc_1_1221:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_1232:
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
        mov    ebx, dword ptr [0x11d8]       ; -> obj1:0x11D8
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_0B86
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
        call   sub_1_0B86
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
        call   sub_1_0B86
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
        call   sub_1_0B8C
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_0B8C
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_0B8C
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
        mov    eax, dword ptr [0x11d8]       ; -> obj1:0x11D8
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0B86
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
        call   sub_1_0B18
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_0B8C
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
        call   sub_1_0C8C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_0C8C
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
        call   sub_1_0C8C
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
        call   sub_1_0C8C
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
        call   sub_1_0C8C
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
        call   sub_1_0C8C
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
        call   sub_1_0C8C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0C8C
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
        call   sub_1_0C8C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0C8C
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
        call   sub_1_0C8C
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
        call   sub_1_0C8C
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x4000  R-X
        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0020:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        test   ecx, ecx
        jne    loc_4_004C
        mov    eax, dword ptr [0]            ; -> obj1:0x0EA4
        mov    ecx, dword ptr [ebp + 0x10]
        push   eax
        lea    edx, [ebp - 4]
        push   3
        push   ecx
        push   edx
        call   sub_1_0B86
loc_4_0043:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_004C:
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        mov    ecx, dword ptr [edx + 0x201]
        lea    eax, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   eax
        push   edx
        call   sub_1_0C52
loc_4_0068:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
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
        call   sub_1_0C58
loc_4_0111:
        mov    di, word ptr [ebp + 0xc]
        add    esp, 0x54
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax], di
        je     loc_4_012E
        push   eax
        call   sub_4_2C70
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
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0140:
        push   0x3e8
        call   sub_1_0B80
loc_4_014A:
        inc    dword ptr [0x52]              ; -> obj1:0x122E
        mov    eax, dword ptr [0x52]         ; -> obj1:0x122E
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh
        db  0ECh, 05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h
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
        call   sub_1_0BC2
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
        call   sub_1_0C3A
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
        dd  loc_1_0C46               ; fixup
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
        call   sub_1_0C40
loc_4_024D:
        mov    ecx, dword ptr [esi + 0x53]
        push   ecx
        call   sub_1_0BCE
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
        call   sub_4_3B40
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
        call   sub_4_19F0
        test   al, al
        je     loc_4_0300
        mov    dword ptr [esi + 0xb3], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3960
        push   esi
        call   sub_4_1A20
loc_4_0300:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0310:
        push   ebp
        mov    eax, 0xd
        mov    ebp, esp
        sub    esp, 0x48
        mov    word ptr [ebp - 0x30], ax
        push   ebx
        mov    word ptr [ebp - 0x2e], ax
        push   esi
        push   edi
        mov    eax, 0xa
        mov    dword ptr [ebp - 0x2c], 0     ; -> loc_1_0D30
        mov    dword ptr [ebp - 0x3c], 0     ; -> obj1:0x0D4C
        mov    dword ptr [ebp - 0x34], 0     ; -> loc_1_0D64
        mov    word ptr [ebp - 0x40], ax
        mov    word ptr [ebp - 0x3e], ax
        mov    dword ptr [ebp - 0x44], 0     ; -> loc_1_0D7C
        mov    word ptr [ebp - 0x38], ax
        mov    dword ptr [ebp - 0x24], 0     ; -> obj1:0x0DA0
        mov    word ptr [ebp - 0x36], ax
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x48], ax
        mov    esi, dword ptr [edi + 4]
        mov    word ptr [ebp - 0x46], ax
        mov    ebx, dword ptr [edi + 8]
        mov    eax, 0x18
        mov    ecx, dword ptr [edi + 0x14]
        mov    word ptr [ebp - 0x28], ax
        mov    edx, dword ptr [edi + 0x18]
        mov    word ptr [ebp - 0x26], ax
        mov    dword ptr [ebp - 0x14], ecx
        mov    eax, dword ptr [edi + 0x10]
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x10], edx
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 4]
        push   eax
        push   ecx
        push   edx
        call   sub_1_0C04
loc_4_039F:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_03B3
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_03B3:
        push   1
        lea    eax, [ebp - 0x48]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0C0A
loc_4_03CA:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_03D6
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax + 4]
loc_4_03D6:
        push   1
        lea    eax, [ebp - 0x40]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0C0A
loc_4_03ED:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_04AA
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    edx, [ebp - 0x38]
        mov    ecx, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 0x20], ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_0C0A
loc_4_0417:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_04AA
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    edx, [ebp - 0x30]
        mov    ecx, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 0x1c], ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_0C0A
loc_4_0441:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0450
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], ecx
loc_4_0450:
        push   1
        lea    eax, [ebp - 0x28]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0C0A
loc_4_0467:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0473
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 4]
loc_4_0473:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0C10
loc_4_047C:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [ebp - 0x18]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], ebx
        mov    dword ptr [edi + 0xc], ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [edi + 0x10], edx
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], ecx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_04AA:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0C10
loc_4_04B3:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_04D0:
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
        je     loc_4_0515
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0515:
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
        jne    loc_4_054B
        mov    dword ptr [ecx + 0x20], 1
        jmp    loc_4_0558
loc_4_054B:
        cmp    dword ptr [ebp - 0xc], 1
        jne    loc_4_0558
        mov    dword ptr [ecx + 0x20], 2
loc_4_0558:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x209], eax
        mov    dword ptr [ecx + 0x20d], edx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0580:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     loc_4_05A1
        mov    eax, 0xc0010002
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05A1:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x10]
        test   ecx, ecx
        jne    loc_4_05BB
        push   edx
        push   esi
        push   edi
        call   sub_4_09D0
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05BB:
        cmp    ecx, 1
        jne    loc_4_05F3
        mov    ecx, dword ptr [edi + 0x47]
        test   ecx, ecx
        je     loc_4_05E3
        test   cl, 2
        je     loc_4_05D1
        mov    eax, 0xc0000001
loc_4_05D1:
        test   cl, 1
        je     loc_4_05F8
        mov    eax, 0xc001000d
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05E3:
        push   edx
        push   esi
        push   edi
        call   sub_4_0A20
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_05F3:
        mov    eax, 0x10001
loc_4_05F8:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_0600:
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
        ja     loc_4_063F
        je     loc_4_0825
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_4_0660
        jmp    dword ptr [eax*4]             ; -> obj4:0x0978
loc_4_063F:
        cmp    eax, 0x1010102
        je     loc_4_0844
        cmp    eax, 0x1010103
        je     loc_4_0863
        cmp    eax, 0x1010104
        je     loc_4_0913
loc_4_0660:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_4_0929
loc_4_066C:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_4_0681
        mov    eax, 0                        ; -> loc_1_0E34
        mov    edi, 0x5c
        jmp    loc_4_092F
loc_4_0681:
        mov    eax, 0                        ; -> obj1:0x0DB8
        mov    edi, 0x7c
        jmp    loc_4_092F
loc_4_0690:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x47]
        test   al, 2
        je     loc_4_06A3
        mov    dword ptr [ebp - 0x18], 4
        jmp    loc_4_06B5
loc_4_06A3:
        mov    dword ptr [ebp - 0x18], 2
        test   al, 1
        jne    loc_4_06B5
        mov    dword ptr [ebp - 0x18], 0
loc_4_06B5:
        lea    eax, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_4_092F
loc_4_06C2:
        lea    eax, [ebp - 0x1c]
        mov    edi, 4
        jmp    loc_4_092F
loc_4_06CF:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_4_092F
loc_4_06E3:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_4_092F
loc_4_06F7:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_4_092F
loc_4_070B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa3]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_092F
loc_4_0724:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa7]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_092F
loc_4_073D:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_092F
loc_4_0751:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xab]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_092F
loc_4_076A:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 4
        call   sub_1_0C6C
loc_4_0781:
        mov    byte ptr [ebp - 0x21], 0
        lea    eax, [ebp - 0x24]
        jmp    loc_4_092F
loc_4_078D:
        mov    eax, 0                        ; -> loc_1_0E90
        mov    edi, 0x14
        jmp    loc_4_092F
loc_4_079C:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_07AD
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7b]
        mov    ecx, dword ptr [eax + 0xc]
        jmp    loc_4_07B6
loc_4_07AD:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [eax + 8]
loc_4_07B6:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_4_092F
loc_4_07C6:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_07D4
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7f]
        jmp    loc_4_07DA
loc_4_07D4:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x21]
loc_4_07DA:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_092F
loc_4_07EA:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_4_092F
loc_4_07FD:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_092F
loc_4_0811:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    loc_4_092F
loc_4_0825:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_0C6C
loc_4_083C:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_092F
loc_4_0844:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x67
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_0C6C
loc_4_085B:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_092F
loc_4_0863:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_08B9
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 0x7b]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_0890
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_0929
loc_4_0890:
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
loc_4_08A6:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0xc], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_0929
loc_4_08B9:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [edi + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0660
loc_4_08CC:
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_08E7
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_0929
loc_4_08E7:
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
loc_4_0904:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_0929
loc_4_0913:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x20d]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_092F
loc_4_0929:
        mov    eax, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [ebp - 0x14]
loc_4_092F:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_096A
        cmp    esi, 0x1010103
        je     loc_4_096A
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_4_095A
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
loc_4_095A:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0C6C
loc_4_0965:
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_4_096A:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
        ; ---- dispatch table (19 entries) ----
        dd  loc_4_066C
        dd  loc_4_0690
        dd  loc_4_06C2
        dd  loc_4_06C2
        dd  loc_4_06CF
        dd  loc_4_06E3
        dd  loc_4_06F7
        dd  loc_4_070B
        dd  loc_4_0724
        dd  loc_4_073D
        dd  loc_4_0751
        dd  loc_4_076A
        dd  loc_4_078D
        dd  loc_4_079C
        dd  loc_4_07C6
        dd  loc_4_07EA
        dd  loc_4_07FD
        dd  loc_4_0660
        dd  loc_4_0811
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_09D0:
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
        call   sub_4_0600
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0A20:
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
        ja     loc_4_0A5D
        je     loc_4_0B2F
        cmp    eax, 0x1010e
        je     loc_4_0A6F
        cmp    eax, 0x1010f
        je     loc_4_0AA9
        cmp    eax, 0x10112
        je     loc_4_0B1B
loc_4_0A5D:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B6A
loc_4_0A6F:
        cmp    edi, 4
        je     loc_4_0A86
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B6A
loc_4_0A86:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_0C6C
loc_4_0A92:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0B90
        jmp    loc_4_0B68
loc_4_0AA9:
        cmp    edi, 4
        je     loc_4_0AC0
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B6A
loc_4_0AC0:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_0C6C
loc_4_0ACC:
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_4_0B14
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x7f]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_4_0AEE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x7f], eax
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0B6A
loc_4_0AEE:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx + 0x21]
        jne    loc_4_0B0C
        mov    eax, dword ptr [ecx + 0x21]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_4_0B0C
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x21], eax
        call   sub_4_0DE0
        jmp    loc_4_0B6A
loc_4_0B0C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0B6A
loc_4_0B14:
        mov    ebx, 0xc0000001
        jmp    loc_4_0B6A
loc_4_0B1B:
        cmp    edi, 4
        je     loc_4_0B6A
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B6A
loc_4_0B2F:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_4_0B4D
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0B6A
loc_4_0B4D:
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
        call   sub_4_0BF0
loc_4_0B68:
        mov    ebx, eax
loc_4_0B6A:
        test   ebx, ebx
        jne    loc_4_0B78
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_4_0B78:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B90:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0BD2
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0BC8
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
loc_4_0BC1:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0BC8:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0BD2:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0BF0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0C36
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0C2C
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
loc_4_0C25:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0C2C:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0C36:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_0C50:
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
        call   sub_4_0600
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_4_0D25
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_4_0CB5
        je     loc_4_0CF4
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_4_0CC3
        jmp    dword ptr [eax*4]             ; -> obj4:0x0D40
loc_4_0CB5:
        cmp    eax, 0x1020102
        je     loc_4_0CFC
        cmp    eax, 0x1020103
        je     loc_4_0D04
loc_4_0CC3:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_4_0D0D
loc_4_0CCC:
        mov    eax, dword ptr [ebx + 0x83]
        jmp    loc_4_0D0A
loc_4_0CD4:
        mov    eax, dword ptr [ebx + 0x87]
        jmp    loc_4_0D0A
loc_4_0CDC:
        mov    eax, dword ptr [ebx + 0x8b]
        jmp    loc_4_0D0A
loc_4_0CE4:
        mov    eax, dword ptr [ebx + 0x9b]
        jmp    loc_4_0D0A
loc_4_0CEC:
        mov    eax, dword ptr [ebx + 0x9f]
        jmp    loc_4_0D0A
loc_4_0CF4:
        mov    eax, dword ptr [ebx + 0x97]
        jmp    loc_4_0D0A
loc_4_0CFC:
        mov    eax, dword ptr [ebx + 0x8f]
        jmp    loc_4_0D0A
loc_4_0D04:
        mov    eax, dword ptr [ebx + 0x93]
loc_4_0D0A:
        mov    dword ptr [ebp - 4], eax
loc_4_0D0D:
        cmp    edi, 4
        jb     loc_4_0D25
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_0C6C
loc_4_0D21:
        add    dword ptr [ebp - 0xc], 4
loc_4_0D25:
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
        dd  loc_4_0CCC
        dd  loc_4_0CD4
        dd  loc_4_0CDC
        dd  loc_4_0CE4
        dd  loc_4_0CEC
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0D60:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [ecx + 0x14]
        cmp    edx, 0x10113
        ja     loc_4_0D88
        je     loc_4_0DBF
        cmp    edx, 0x10101
        jb     loc_4_0DBA
        cmp    edx, 0x10111
        jbe    loc_4_0DBF
        jmp    loc_4_0DBA
loc_4_0D88:
        cmp    edx, 0x1010104
        ja     loc_4_0DAA
        cmp    edx, 0x1010101
        jae    loc_4_0DBF
        cmp    edx, 0x20101
        jb     loc_4_0DBA
        cmp    edx, 0x20105
        jbe    loc_4_0DBF
        jmp    loc_4_0DBA
loc_4_0DAA:
        cmp    edx, 0x1020101
        jb     loc_4_0DBA
        cmp    edx, 0x1020103
        jbe    loc_4_0DBF
loc_4_0DBA:
        mov    eax, 0xc0010017
loc_4_0DBF:
        test   eax, eax
        jne    loc_4_0DCD
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0C50
loc_4_0DCD:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0DE0:
        push   ebp
        mov    ecx, 0xee
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x1c5]
        test   edx, edx
        je     loc_4_0E06
loc_4_0DF6:
        mov    esi, dword ptr [edx + 0x21]
        cmp    ecx, esi
        jae    loc_4_0DFF
        mov    ecx, esi
loc_4_0DFF:
        mov    edx, dword ptr [edx + 0x11]
        test   edx, edx
        jne    loc_4_0DF6
loc_4_0E06:
        pop    esi
        mov    dword ptr [eax + 0x7f], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0E10:
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
        call   sub_1_0BB0
loc_4_0E37:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     loc_4_0E4E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E4E:
        mov    eax, dword ptr [esi + 0x47]
        test   eax, eax
        je     loc_4_0E79
        test   al, 0xa
        je     loc_4_0E67
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E67:
        test   al, 1
        je     loc_4_0E79
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E79:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_0EA0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1030
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0EA0:
        push   esi
        call   sub_4_19F0
        test   al, al
        jne    loc_4_0ECA
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1030
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x46], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0ECA:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_0BD4
loc_4_0EDB:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_34E0
        test   eax, eax
        je     loc_4_0F29
        jle    loc_4_0F17
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1030
        push   esi
        call   sub_4_1A20
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F17:
        test   eax, eax
        jge    loc_4_0F29
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F29:
        xor    edi, edi
        xor    eax, eax
loc_4_0F2D:
        cmp    eax, dword ptr [ebp - 8]
        jae    loc_4_0F47
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0xb6], cl
        jb     loc_4_0F2D
loc_4_0F47:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        je     loc_4_0FAE
loc_4_0F4F:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0BF8
loc_4_0F5C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0BD4
loc_4_0F6D:
        xor    ecx, ecx
        cmp    edi, 6
        jae    loc_4_0F8E
loc_4_0F74:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_0F8E
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0xb6], al
        jb     loc_4_0F74
loc_4_0F8E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_00D8
loc_4_0F9E:
        add    esp, 0xc
        test   eax, eax
        jl     loc_4_0FC8
        inc    ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, ebx
        ja     loc_4_0F4F
loc_4_0FAE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3600
        test   eax, eax
        jge    loc_4_0FD6
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FC8:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FD6:
        lea    eax, [esi + 0xb7]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_09E0
loc_4_0FE6:
        test   al, al
        je     loc_4_100C
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1070
        push   esi
        call   sub_4_1A20
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_100C:
        push   esi
        inc    dword ptr [esi + 0x83]
        call   sub_4_1A20
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_1770
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_1030:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_4_105C
        pop    ebp
        mov    dword ptr [eax + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
loc_4_105C:
        mov    ecx, dword ptr [eax + 0x28]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_1070:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x38], 0
        jne    loc_4_1099
        pop    ebp
        mov    dword ptr [eax + 0x38], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
loc_4_1099:
        mov    ecx, dword ptr [eax + 0x34]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10B0:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x24], 0
        mov    eax, dword ptr [ecx + 0x24]
        test   eax, eax
        jne    loc_4_10D5
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [ecx + 0x28], edx
        inc    dword ptr [ecx + 0x2c]
        ret    8
loc_4_10D5:
        pop    ebp
        mov    dword ptr [edx + 0x24], eax
        mov    dword ptr [ecx + 0x24], edx
        inc    dword ptr [ecx + 0x2c]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_10F0:
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
        je     loc_4_1148
        test   al, 1
        je     loc_4_1114
        inc    dword ptr [esi + 0x30]
loc_4_1114:
        test   al, 2
        je     loc_4_1121
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x93]
loc_4_1121:
        test   al, 4
        je     loc_4_112E
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_112E:
        test   al, 8
        je     loc_4_113B
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_113B:
        test   al, 0x10
        je     loc_4_1148
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_1148:
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_4_115C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_115C:
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
        call   sub_1_0BB0
loc_4_117C:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0BD4
loc_4_118D:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_34E0
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_11D7
        push   edi
        push   esi
        call   sub_4_10B0
        test   ebx, ebx
        jge    loc_4_11C9
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
loc_4_11C9:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_11D7:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_11DB:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_11F5
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0xb6], al
        jb     loc_4_11DB
loc_4_11F5:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 0x14]
        dec    eax
        je     loc_4_1265
loc_4_1202:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0BF8
loc_4_120F:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0BD4
loc_4_1220:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    loc_4_1241
loc_4_1227:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_1241
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0xb6], al
        jb     loc_4_1227
loc_4_1241:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_00D8
loc_4_1251:
        add    esp, 0xc
        cmp    eax, -2
        je     loc_4_1287
        mov    eax, dword ptr [ebp - 0x14]
        inc    dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, dword ptr [ebp - 0x10]
        ja     loc_4_1202
loc_4_1265:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3600
        cmp    eax, -2
        jne    loc_4_129C
        push   edi
        push   esi
        call   sub_4_10B0
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1287:
        push   edi
        push   esi
        call   sub_4_10B0
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_129C:
        lea    eax, [esi + 0xb7]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_09E0
loc_4_12AC:
        test   al, al
        je     loc_4_12CC
        push   edi
        mov    eax, dword ptr [edi + 0x28]
        push   eax
        push   esi
        call   sub_4_1070
        push   esi
        call   sub_4_3E00
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_12CC:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0AFA
loc_4_12E1:
        push   esi
        call   sub_4_1770
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1300:
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
loc_4_1325:
        test   edx, edx
        je     loc_4_1334
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_4_1325
        mov    dword ptr [ecx], edx
loc_4_1334:
        cmp    dword ptr [ecx], -1
        jne    loc_4_1347
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_1347:
        mov    eax, dword ptr [0]            ; -> obj1:0x0EA4
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x25
        push   ecx
        call   sub_1_0B86
loc_4_135A:
        test   eax, eax
        je     loc_4_136C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_136C:
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
loc_4_1387:
        test   al, al
        jne    loc_4_13A6
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x25
        push   eax
        call   sub_1_0B8C
loc_4_1398:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_13A6:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_13C8
        mov    eax, dword ptr [esi + 0x1cd]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 0x103
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1cd], ecx
        jmp    loc_4_13DF
loc_4_13C8:
        mov    eax, dword ptr [esi + 0x1c5]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1c5], ecx
loc_4_13DF:
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
        jne    loc_4_1432
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
loc_4_1432:
        mov    eax, ebx
        pop    edi
        inc    dword ptr [esi + 0x1c9]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1450:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        xor    eax, eax
loc_4_145F:
        mov    dl, byte ptr [esi + eax + 0x67]
        mov    ecx, dword ptr [esi + 0x73]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     loc_4_145F
        mov    edi, dword ptr [esi + 0x73]
        add    edi, 6
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_4_1497
        mov    eax, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x10]
loc_4_1482:
        xor    ecx, ecx
loc_4_1484:
        mov    dl, byte ptr [eax + ecx]
        inc    edi
        inc    ecx
        mov    byte ptr [edi - 1], dl
        cmp    ecx, 6
        jb     loc_4_1484
        add    eax, 6
        dec    ebx
        jne    loc_4_1482
loc_4_1497:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x77], eax
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_14B0
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x44], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_14B0:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1B70
        push   eax
        call   sub_1_0B7A
loc_4_14BF:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E70
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2F20
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3250
        test   eax, eax
        jge    loc_4_14FE
        mov    eax, 0xc0000001
        pop    edi
        or     dword ptr [esi + 0x47], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_14FE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  090h
loc_4_1510:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [eax + 0xd]
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0xffffffd0
        je     loc_4_1530
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_1530:
        test   al, 1
        je     loc_4_1539
        mov    ecx, 1
loc_4_1539:
        test   al, 2
        je     loc_4_1540
        or     ecx, 4
loc_4_1540:
        test   al, 4
        je     loc_4_1547
        or     ecx, 8
loc_4_1547:
        test   al, 8
        je     loc_4_154E
        or     ecx, 2
loc_4_154E:
        test   al, 0x20
        je     loc_4_1555
        or     ecx, 0x10
loc_4_1555:
        test   dword ptr [esi + 0x1bd], ecx
        je     loc_4_1567
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_1567:
        mov    dword ptr [esi + 0x1b9], ecx
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_157E
        mov    byte ptr [esi + 0x43], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_4_157E:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1B70
        push   eax
        call   sub_1_0B7A
loc_4_158D:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 0x1b9]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3250
        test   eax, eax
        jge    loc_4_15B6
        or     dword ptr [esi + 0x47], 2
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_4_15B6:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15D0:
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
        call   sub_1_0B62
loc_4_15F1:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1600:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x1ee]
        test   ecx, ecx
        je     loc_4_1630
loc_4_1611:
        mov    eax, dword ptr [esi + 0x1ee]
        push   ecx
        mov    edx, dword ptr [eax + 0x15]
        mov    dword ptr [esi + 0x1ee], edx
        call   sub_4_1770
        mov    ecx, dword ptr [esi + 0x1ee]
        test   ecx, ecx
        jne    loc_4_1611
loc_4_1630:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1640:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 8
        jne    loc_4_1676
        push   esi
        call   sub_4_19F0
        test   al, al
        je     loc_4_1665
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3690
        push   esi
        call   sub_4_1A20
loc_4_1665:
        push   0x3e8
        add    esi, 0x1dd
        push   esi
        call   sub_1_0B62
loc_4_1676:
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_4_1680:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 4], 0
        mov    esi, dword ptr [edi + 0xd]
        je     loc_4_169E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_169E:
        inc    dword ptr [edi]
        mov    eax, dword ptr [esi + 0x1c5]
        cmp    edi, eax
        jne    loc_4_16B5
        mov    eax, dword ptr [edi + 0x11]
        mov    dword ptr [esi + 0x1c5], eax
        jmp    loc_4_16C8
loc_4_16B5:
        cmp    dword ptr [eax + 0x11], edi
        je     loc_4_16C2
loc_4_16BA:
        mov    eax, dword ptr [eax + 0x11]
        cmp    dword ptr [eax + 0x11], edi
        jne    loc_4_16BA
loc_4_16C2:
        mov    ecx, dword ptr [edi + 0x11]
        mov    dword ptr [eax + 0x11], ecx
loc_4_16C8:
        push   0
        mov    eax, dword ptr [edi + 0x1d]
        push   eax
        mov    ecx, dword ptr [esi + 0x7b]
        push   ecx
        call   sub_2_0240
loc_4_16D7:
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_16EF
        cmp    ebx, 0x103
        je     loc_4_1703
        cmp    ebx, 0xc001000e
        je     loc_4_170C
        jmp    loc_4_1715
loc_4_16EF:
        mov    eax, dword ptr [edi]
        sub    eax, 2
        mov    dword ptr [edi], eax
        je     loc_4_1717
        mov    byte ptr [edi + 4], 1
        mov    ebx, 0x103
        jmp    loc_4_1717
loc_4_1703:
        mov    byte ptr [edi + 4], 1
        sub    dword ptr [edi], 2
        jmp    loc_4_1717
loc_4_170C:
        mov    byte ptr [edi + 4], 1
        mov    ebx, 0x103
loc_4_1715:
        dec    dword ptr [edi]
loc_4_1717:
        cmp    dword ptr [edi], 0
        jne    loc_4_175B
        mov    eax, dword ptr [edi + 0x21]
        cmp    dword ptr [esi + 0x7f], eax
        jne    loc_4_172A
        push   esi
        call   sub_4_0DE0
loc_4_172A:
        cmp    dword ptr [esi + 0x1c5], 0
        jne    loc_4_1742
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1B70
        push   eax
        call   sub_1_0B7A
loc_4_1742:
        push   0
        dec    dword ptr [esi + 0x1c9]
        push   0x25
        push   edi
        call   sub_1_0B8C
loc_4_1752:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_175B:
        mov    ebx, 0x103
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1770:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    loc_4_17C3
        mov    eax, dword ptr [esi + 0x21]
        cmp    dword ptr [edi + 0x7f], eax
        jne    loc_4_1790
        push   edi
        call   sub_4_0DE0
loc_4_1790:
        cmp    dword ptr [edi + 0x1c5], 0
        jne    loc_4_17A8
        push   edi
        lea    eax, [edi + 8]
        push   0                             ; -> loc_4_1B70
        push   eax
        call   sub_1_0B7A
loc_4_17A8:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0AF4
loc_4_17B3:
        push   0
        push   0x25
        push   esi
        call   sub_1_0B8C
loc_4_17BD:
        dec    dword ptr [edi + 0x1c9]
loc_4_17C3:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_17D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        cmp    byte ptr [eax + 4], 0
        je     loc_4_17ED
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17ED:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_17FE
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_17FE:
        push   0x40010004
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 0x1ea], eax
        call   sub_4_1990
        or     dword ptr [esi + 0x47], 1
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        je     loc_4_1847
loc_4_181C:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x24], eax
        dec    dword ptr [esi + 0x2c]
        inc    dword ptr [esi + 0x30]
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0AFA
loc_4_183A:
        push   edi
        call   sub_4_1770
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        jne    loc_4_181C
loc_4_1847:
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        je     loc_4_1873
loc_4_184E:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x38], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0AFA
loc_4_1866:
        push   edi
        call   sub_4_1770
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        jne    loc_4_184E
loc_4_1873:
        mov    byte ptr [esi + 0x1ff], 1
        push   esi
        call   sub_4_19F0
        test   al, al
        je     loc_4_189B
        push   esi
        call   sub_4_18B0
        push   esi
        call   sub_4_1A20
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_189B:
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x1ff], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_18B0:
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
        call   sub_4_2C70
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E70
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2F20
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3250
        test   eax, eax
        jge    loc_4_1900
        or     dword ptr [esi + 0x47], 2
loc_4_1900:
        push   0x40010005
        push   esi
        call   sub_4_1990
        mov    eax, dword ptr [esi + 0x47]
        and    eax, 0xfffffffe
        mov    dword ptr [esi + 0x47], eax
        test   al, 2
        je     loc_4_191F
        push   0x80010004
        jmp    loc_4_1921
loc_4_191F:
        push   0
loc_4_1921:
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0B06
loc_4_192A:
        push   edi
        call   sub_4_1770
        push   esi
        call   sub_4_1940
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1940:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x1cd]
        test   ecx, ecx
        je     loc_4_1986
loc_4_1951:
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
        call   sub_1_0AEE
loc_4_197C:
        mov    ecx, dword ptr [esi + 0x1cd]
        test   ecx, ecx
        jne    loc_4_1951
loc_4_1986:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1990:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x1c5]
        test   edi, edi
        je     loc_4_19DB
        mov    esi, dword ptr [ebp + 0xc]
loc_4_19AA:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi]
        mov    ebx, dword ptr [edi + 0x11]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_0B1E
loc_4_19C6:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_0B24
loc_4_19CF:
        push   edi
        call   sub_4_1770
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_4_19AA
loc_4_19DB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0x40], 0
        je     loc_4_1A03
        xor    al, al
        pop    ebp
        ret    4
loc_4_1A03:
        mov    byte ptr [eax + 0x40], 1
        push   eax
        push   0                             ; -> loc_4_1B70
        add    eax, 8
        push   eax
        call   sub_1_0B7A
loc_4_1A16:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1A20:
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
        je     loc_4_1B4E
loc_4_1A5D:
        cmp    byte ptr [esi + 0x1ff], 0
        je     loc_4_1A6E
        mov    bl, 1
        push   esi
        call   sub_4_18B0
loc_4_1A6E:
        cmp    byte ptr [esi + 0x44], 0
        je     loc_4_1AB1
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2E70
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2F20
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3250
        test   eax, eax
        jge    loc_4_1AAF
        or     dword ptr [esi + 0x47], 2
loc_4_1AAF:
        mov    bl, 1
loc_4_1AB1:
        cmp    byte ptr [esi + 0x43], 0
        je     loc_4_1ADC
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        mov    eax, dword ptr [esi + 0x1b9]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3250
        test   eax, eax
        jge    loc_4_1ADA
        or     dword ptr [esi + 0x47], 2
loc_4_1ADA:
        mov    bl, 1
loc_4_1ADC:
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_1AFA
        mov    byte ptr [esi + 0x46], 0
loc_4_1AE6:
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_1AF8
        push   0
        push   esi
        call   sub_4_10F0
        test   eax, eax
        je     loc_4_1AE6
loc_4_1AF8:
        mov    bl, 1
loc_4_1AFA:
        cmp    dword ptr [esi + 0x38], 0
        je     loc_4_1B08
        push   esi
        mov    bl, 1
        call   sub_4_3E00
loc_4_1B08:
        cmp    byte ptr [esi + 0x41], 0
        je     loc_4_1B1B
        mov    byte ptr [esi + 0x41], 0
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_1_0940
loc_4_1B1B:
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
        jne    loc_4_1A5D
loc_4_1B4E:
        mov    byte ptr [esi + 0x40], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3B90
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1B70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3B40
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 0F1h, 01Fh, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1BB0:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 0xc], ax
        push   ebx
        mov    word ptr [ebp - 0xa], ax
        xor    ebx, ebx
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_0EB0
        push   eax
        push   0xc                           ; -> obj1:0x11E8
        call   sub_1_0ACA
loc_4_1BE0:
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [0x46], edx         ; -> obj1:0x1222
        mov    dword ptr [0x4a], ecx         ; -> obj1:0x1226
        mov    dword ptr [4], ebx            ; -> obj1:0x11E0
        mov    dword ptr [0x52], ebx         ; -> obj1:0x122E
        call   sub_4_23B0
        mov    dword ptr [0x4e], eax         ; -> obj1:0x122A
        mov    byte ptr [0x10], 3            ; -> obj1:0x11EC
        mov    byte ptr [0x11], bl           ; -> loc_1_11ED
        mov    dword ptr [0x12], ebx         ; -> obj1:0x11EE
        mov    dword ptr [0x16], 0           ; -> obj1:0x11F2
        mov    dword ptr [0x1a], 0           ; -> obj1:0x11F6
        mov    dword ptr [0x1e], 0           ; -> obj1:0x11FA
        mov    dword ptr [0x22], 0           ; -> obj1:0x11FE
        mov    dword ptr [0x26], 0           ; -> obj1:0x1202
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0x2a], 0           ; -> obj1:0x1206
        mov    dword ptr [0x2e], 0           ; -> obj1:0x120A
        mov    dword ptr [0x32], 0           ; -> obj1:0x120E
        mov    dword ptr [0x36], 0           ; -> obj1:0x1212
        mov    dword ptr [0x3a], 0           ; -> obj1:0x1216
        push   0x36
        mov    dword ptr [0x3e], edx         ; -> obj1:0x121A
        push   0x10                          ; -> obj1:0x11EC
        lea    edx, [ebp - 4]
        push   0                             ; -> obj1:0x11DC
        mov    dword ptr [0x42], ecx         ; -> obj1:0x121E
        mov    eax, dword ptr [0xc]          ; -> obj1:0x11E8
        push   eax
        push   8                             ; -> obj1:0x11E4
        push   edx
        call   sub_1_0AD6
loc_4_1CB3:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1CE0
        cmp    dword ptr [4], ebx            ; -> obj1:0x11E0
        jbe    loc_4_1CC9
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1CC9:
        push   0
        mov    eax, dword ptr [0xc]          ; -> obj1:0x11E8
        push   eax
        call   sub_1_0AD0
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
        mov    eax, dword ptr [0]            ; -> obj1:0x0EA4
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   0x225
        push   ecx
        call   sub_1_0B86
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
        call   sub_4_04D0
        test   eax, eax
        je     loc_4_1D51
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x225
        push   eax
        call   sub_1_0B8C
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
        call   sub_1_0B8C
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
        jne    loc_4_209E
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
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0x205]
        mov    dword ptr [ebp - 0x3a], ebx
        mov    dword ptr [ebp - 0x2e], eax
        mov    dword ptr [ebp - 0x3e], 1
        lea    ecx, [ebp - 0x48]
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [8]            ; -> obj1:0x11E4
        push   eax
        lea    edx, [esi + 0x201]
        mov    dword ptr [ebp - 0x36], 0x600
        mov    dword ptr [ebp - 0x32], 1
        mov    dword ptr [ebp - 0x2a], 0x20
        push   esi
        push   ecx
        push   edx
        call   sub_1_0AE2
loc_4_1E20:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_4_2098
        mov    eax, dword ptr [esi + 0x209]
        cmp    eax, -1
        je     loc_4_1EA1
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
        mov    byte ptr [ebp - 0x20], 1
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x15], eax
        mov    dword ptr [ebp - 0x1d], 2
        mov    dword ptr [ebp - 0x19], 3
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0C34
loc_4_1E7A:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1EA1
        mov    eax, dword ptr [esi + 0x209]
        mov    ecx, dword ptr [esi + 0x201]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_0C58
loc_4_1E99:
        add    esp, 0x10
        jmp    loc_4_208C
loc_4_1EA1:
        mov    edx, dword ptr [esi + 0x21d]
        mov    ecx, dword ptr [esi + 0x205]
        lea    eax, [ebp - 0xc]
        mov    ebx, dword ptr [esi + 0x209]
        push   eax
        lea    edi, [edx + 0x800]
        push   ebx
        lea    eax, [ecx + 0x20]
        push   edi
        push   edx
        push   eax
        mov    edx, dword ptr [esi + 0x73]
        push   ecx
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 0x211]
        push   ecx
        push   edx
        push   esi
        push   eax
        call   sub_4_2B80
        test   eax, eax
        jge    loc_4_1F0D
        mov    eax, dword ptr [esi + 0x205]
        mov    ecx, dword ptr [esi + 0x211]
        push   eax
        mov    edx, dword ptr [esi + 0x201]
        push   ecx
        push   2
        push   0xaa000004
        push   edx
        call   sub_1_0C58
loc_4_1EFE:
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0x14
        jmp    loc_4_207A
loc_4_1F0D:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x20], eax
loc_4_1F1E:
        lea    eax, [esi + ecx]
        mov    edx, dword ptr [esi + 0x73]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 0x6d], dl
        mov    byte ptr [eax + 0x67], dl
        jb     loc_4_1F1E
        lea    ebx, [esi + 0x7b]
        lea    eax, [esi + 4]
        push   ebx
        lea    ecx, [esi + 0x67]
        push   eax
        mov    edx, dword ptr [esi + 0x20d]
        push   ecx
        push   0                             ; -> loc_4_15D0
        push   0                             ; -> loc_4_1510
        push   0                             ; -> loc_4_1450
        push   edx
        call   sub_2_0000
loc_4_1F5B:
        test   al, al
        jne    loc_4_1F6B
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_4_207A
loc_4_1F6B:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2E70
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2F20
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3770
        mov    edi, eax
        mov    ecx, dword ptr [0]            ; -> obj1:0x0EA4
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        call   sub_1_0B86
loc_4_1F9A:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_1FAD
        mov    dword ptr [ebp - 4], 0xc000009a
        jmp    loc_4_2072
loc_4_1FAD:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3790
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
        call   sub_1_0B8C
loc_4_1FDF:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3BE0
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 0x1bd], eax
        mov    dword ptr [esi + 0x1b9], 0
        call   sub_4_3250
        test   eax, eax
        jge    loc_4_200F
        or     dword ptr [esi + 0x47], 2
        jmp    loc_4_2072
loc_4_200F:
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
        call   sub_1_0B6E
loc_4_2047:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_20AA
        mov    eax, dword ptr [esi + 0x211]
        mov    ecx, dword ptr [esi + 0x201]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_0C58
loc_4_2067:
        add    esp, 0x10
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2D70
loc_4_2072:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_4_207A:
        cmp    dword ptr [esi + 0x209], -1
        je     loc_4_208C
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_0C4C
loc_4_208C:
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0AE8
loc_4_2098:
        push   esi
        call   sub_4_2200
loc_4_209E:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_20AA:
        mov    dword ptr [esi + 0x1c1], 0    ; -> obj1:0x11DC
        mov    byte ptr [esi + 0x45], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x1c9], ecx
        mov    dword ptr [esi + 0x1c5], ecx
        mov    dword ptr [esi + 0x1cd], ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x11DC
        mov    dword ptr [esi + 0x221], eax
        lea    eax, [esi + 0x1dd]
        mov    dword ptr [0], esi            ; -> obj1:0x11DC
        mov    dword ptr [esi + 0x1d1], ecx
        mov    dword ptr [esi + 0x1d9], esi
        push   0x3e8
        mov    dword ptr [eax], ecx
        push   eax
        mov    dword ptr [esi + 0x1e5], esi
        mov    dword ptr [esi + 0x1d5], 0    ; -> loc_4_1600
        mov    dword ptr [esi + 0x1e1], 0    ; -> loc_4_1640
        call   sub_1_0B62
loc_4_2116:
        mov    ecx, dword ptr [esi + 0x1c1]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2130:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0EA4
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [0x4e]         ; -> obj1:0x122A
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   3
        push   ecx
        push   esi
        call   sub_1_0B86
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
        mov    eax, dword ptr [0]            ; -> obj1:0x0EA4
        push   eax
        mov    ecx, dword ptr [esi + 0x20d]
        push   0
        lea    eax, [ecx + ecx*2 + 3]
        add    eax, eax
        push   eax
        push   ebx
        call   sub_1_0B86
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
        call   sub_1_0BB6
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
        call   sub_1_0B8C
loc_4_21D5:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> obj1:0x122A
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_0B8C
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
        call   sub_1_0BBC
loc_4_2210:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> obj1:0x122A
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_0B8C
loc_4_2220:
        push   0
        mov    ecx, dword ptr [esi + 0x20d]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 0x73]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_0B8C
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
        mov    eax, dword ptr [8]            ; -> obj1:0x11E4
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_0ADC
loc_4_22C5:
        push   0
        mov    ecx, dword ptr [0xc]          ; -> obj1:0x11E8
        push   ecx
        call   sub_1_0AD0
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
        call   sub_1_0B80
loc_4_22F9:
        push   esi
        push   0                             ; -> loc_4_2390
        push   edi
        call   sub_1_0B7A
loc_4_2305:
        push   edi
        call   sub_1_0B74
loc_4_230B:
        cmp    esi, dword ptr [0]            ; -> obj1:0x11DC
        jne    loc_4_2320
        mov    eax, dword ptr [esi + 0x221]
        mov    dword ptr [0], eax            ; -> obj1:0x11DC
        jmp    loc_4_234E
loc_4_2320:
        mov    ecx, dword ptr [0]            ; -> obj1:0x11DC
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
        call   sub_1_0C4C
loc_4_2360:
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_2_0120
loc_4_2369:
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0AE8
loc_4_2375:
        push   esi
        call   sub_4_2200
        push   0
        push   0x225
        push   esi
        call   sub_1_0B8C
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
        call   sub_4_2D70
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2C70
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
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x40]
        mov    word ptr [ebp - 2], di
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        cmp    al, 0x50
        jne    loc_4_24A5
        mov    eax, dword ptr [esi + 0x40]
        mov    ecx, dword ptr [esi + 4]
        inc    eax
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 0x48
        jne    loc_4_24A5
        mov    eax, dword ptr [esi + 0x40]
        mov    ecx, dword ptr [esi + 4]
        add    eax, 2
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0xf0
        jne    loc_4_24A5
        mov    eax, dword ptr [esi + 0x40]
        mov    ecx, dword ptr [esi + 4]
        add    eax, 3
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 0x53
        jne    loc_4_24A5
        push   1
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ebx, dword ptr [ebp + 0xc]
loc_4_2457:
        mov    eax, dword ptr [esi + 0x60]
        mov    ecx, dword ptr [esi + 4]
        add    eax, edi
        inc    ebx
        push   eax
        inc    edi
        push   ecx
        call   sub_4_0260
        mov    cl, al
        mov    byte ptr [ebx - 1], al
        and    ecx, 0xffff00ff
        add    word ptr [ebp - 2], cx
        cmp    edi, 6
        jl     loc_4_2457
        mov    eax, dword ptr [esi + 0x78]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    cl, al
        pop    edi
        mov    ax, word ptr [ebp - 2]
        and    ecx, 0xffff00ff
        add    ax, cx
        pop    esi
        and    ax, 0xff
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_24A5:
        mov    ax, 0xff00
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_24C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        jne    loc_4_24DE
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_24DE:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    word ptr [ebp - 2], ax
        test   al, 0x40
        jne    loc_4_2524
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 2]
        push   eax
        push   ebx
        xor    edx, edx
        mov    dx, word ptr [esi]
        push   ecx
        push   0                             ; -> obj1:0x1084
        mov    eax, dword ptr [esi + 4]
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_2524:
        mov    ecx, dword ptr [esi + 0x50]
        mov    edx, dword ptr [esi + 4]
        and    word ptr [ebp - 2], 0xffe3
        or     word ptr [ebp - 2], 0x1023
        mov    eax, dword ptr [ebp - 2]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0290
        push   2
        mov    ecx, dword ptr [esi + 0x48]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 0x68]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0270
        and    eax, 0xff
        mov    ecx, dword ptr [esi + 4]
        mov    edi, eax
        mov    eax, dword ptr [esi + 0x60]
        shl    edi, 0x10
        push   eax
        push   ecx
        call   sub_4_0270
        and    eax, 0xff00
        or     edi, eax
        cmp    ebx, edi
        je     loc_4_25AA
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        push   edi
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        push   0                             ; -> loc_1_10B4
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_25AA:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, eax
        sub    ecx, ebx
        cmp    ecx, 0x7ff
        jae    loc_4_25E4
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        push   ebx
        xor    eax, eax
        mov    edx, dword ptr [esi + 4]
        mov    ax, word ptr [esi + 2]
        push   eax
        push   0                             ; -> loc_1_10F4
        push   ecx
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_25E4:
        push   0x800
        mov    eax, dword ptr [esi + 4]
        push   edi
        push   eax
        call   sub_4_0020
        mov    dword ptr [esi + 0x20], eax
        test   eax, eax
        jne    loc_4_2625
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        push   edi
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        push   0                             ; -> loc_1_1130
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_2625:
        push   0
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        push   ecx
        call   sub_4_0290
        xor    bx, bx
        push   0
        mov    ecx, dword ptr [esi + 0x60]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_2649:
        push   ebx
        mov    eax, dword ptr [esi + 0x70]
        inc    bx
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        cmp    bx, 0x400
        jb     loc_4_2649
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    edx, dword ptr [esi + 0x50]
        mov    eax, dword ptr [esi + 4]
        and    word ptr [ebp - 2], 0xefff
        xor    bx, bx
        mov    ecx, dword ptr [ebp - 2]
        push   ecx
        push   edx
        push   eax
        call   sub_4_0290
loc_4_2689:
        mov    cx, bx
        mov    eax, dword ptr [ebp - 8]
        and    ecx, 1
        cmp    word ptr [eax + ecx*2], bx
        jne    loc_4_26A1
        inc    bx
        cmp    bx, 0x400
        jb     loc_4_2689
loc_4_26A1:
        mov    ecx, dword ptr [esi + 0x50]
        mov    edx, dword ptr [esi + 4]
        or     byte ptr [ebp - 1], 0x10
        mov    eax, dword ptr [ebp - 2]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0290
        cmp    bx, 0x400
        jae    loc_4_26F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, ebx
        push   eax
        and    ecx, 0xffff
        push   ecx
        xor    edx, edx
        mov    dx, word ptr [esi + 2]
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   edx
        push   0                             ; -> loc_1_1164
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_26F3:
        mov    ax, 3
        mov    edx, dword ptr [0xc]          ; -> obj1:0x0FC8
        shr    edi, 8
        add    edx, 0x16
loc_4_2703:
        shr    edi, 4
        dec    edx
        mov    ecx, edi
        and    ecx, 0xf
        dec    ax
        mov    cl, byte ptr [ecx]            ; -> loc_1_0FE4
        mov    byte ptr [edx + 1], cl
        jne    loc_4_2703
        mov    eax, 3
        pop    edi
        or     byte ptr [esi + 8], 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_4_2730:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        test   al, 0x1f
        push   ebx
        push   esi
        push   edi
        jne    loc_4_2930
        cmp    eax, 0x100
        jb     loc_4_2930
        cmp    eax, 0x3c0
        ja     loc_4_2930
        mov    ecx, dword ptr [ebp + 0x18]
        sub    ecx, eax
        cmp    ecx, 0x1f
        jb     loc_4_2930
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   edi
        push   esi
        call   sub_4_23E0
        cmp    ax, 0xff
        je     loc_4_278F
        mov    cx, 0xff
        pop    edi
        cmp    cx, ax
        pop    esi
        sbb    eax, eax
        pop    ebx
        and    eax, 0xfffffffb
        pop    ebp
        add    eax, 9
        ret    0x1c
loc_4_278F:
        push   6
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 0x74]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    byte ptr [esi + 0xa], al
        test   al, 0xf1
        je     loc_4_27BE
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_27BE:
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    ax, 0xfff0
        mov    ecx, dword ptr [esi + 0x50]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        push   3
        mov    ecx, dword ptr [esi + 0x48]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 0x60]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   1
        mov    ecx, dword ptr [esi + 0x60]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        mov    ebx, 0x4e20
        call   sub_4_0290
loc_4_280F:
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    loc_4_280F
        push   edi
        push   esi
        call   sub_4_23E0
        cmp    ax, 0xff
        je     loc_4_2836
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_2836:
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    ax, 0xffe3
        mov    ecx, dword ptr [esi + 0x50]
        or     ax, 0x1023
        mov    edx, dword ptr [esi + 4]
        push   eax
        mov    edi, 0x320
        push   ecx
        push   edx
        call   sub_4_0290
loc_4_285E:
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    loc_4_285E
        mov    eax, dword ptr [ebp + 0x20]
        mov    ecx, dword ptr [ebp + 0x1c]
        push   eax
        push   ecx
        push   esi
        call   sub_4_24C0
        mov    edi, eax
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    ebx, 3
        jb     loc_4_2924
        cmp    ebx, 8
        je     loc_4_2924
        cmp    ebx, 0xd
        je     loc_4_2924
        cmp    ebx, 0xe
        je     loc_4_2924
        cmp    ebx, 0xf
        ja     loc_4_2924
        push   2
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 0x74]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    eax, 0xf
        cmp    eax, ebx
        je     loc_4_28D7
        mov    eax, 7
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_28D7:
        push   0
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    byte ptr [esi + 0xe], 0x20
        mov    dword ptr [esi + 0x1ce], 0x100
        mov    dword ptr [esi + 0x1c6], 0x2000
        mov    dword ptr [esi + 0x1ca], 0x6000
        mov    eax, 0x80
        or     byte ptr [esi + 8], 2
        mov    word ptr [esi + 0xc0], ax
        mov    byte ptr [esi + 0xf], al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_2924:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_2930:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h
sub_4_2940:
        push   ebp
        mov    eax, 0                        ; -> obj6:0x0010
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    ecx, dword ptr [ebp + 0x14]
        push   esi
        xor    bl, bl
        push   edi
loc_4_2953:
        cmp    dword ptr [eax], ecx
        je     loc_4_2963
        add    eax, 4
        cmp    eax, 0x1c                     ; -> obj6:0x002C
        jb     loc_4_2953
        jmp    loc_4_2965
loc_4_2963:
        mov    bl, 1
loc_4_2965:
        xor    edx, edx
        test   bl, bl
        je     loc_4_2B6B
        mov    eax, dword ptr [ebp + 0x18]
        sub    eax, ecx
        cmp    eax, 0x1f
        jb     loc_4_2B6B
        mov    word ptr [ebp - 2], dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, esi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], edx
loc_4_298C:
        mov    eax, dword ptr [ebx + 0x40]
        mov    ecx, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        inc    edi
        push   eax
        push   ecx
        call   sub_4_0260
        mov    cl, al
        mov    byte ptr [edi - 1], al
        and    ecx, 0xffff00ff
        inc    dword ptr [ebp - 8]
        add    word ptr [ebp - 2], cx
        cmp    dword ptr [ebp - 8], 6
        jb     loc_4_298C
        mov    eax, dword ptr [ebx + 0x58]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        and    ax, 0xff
        mov    ecx, dword ptr [ebx + 4]
        add    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebx + 0x40]
        add    eax, 0xa
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 1
        jne    loc_4_2A00
        mov    eax, dword ptr [ebx + 0x40]
        mov    ecx, dword ptr [ebx + 4]
        add    eax, 0xb
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 4
        jne    loc_4_2A00
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2A00:
        cmp    byte ptr [ebp - 2], 0xff
        je     loc_4_2A22
        mov    edi, 0                        ; -> loc_1_1190
        mov    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_4_2A22
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2A22:
        mov    eax, dword ptr [ebx + 0x5c]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    byte ptr [ebx + 0xa], al
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 3
        jb     loc_4_2B5D
        cmp    eax, 0xf
        ja     loc_4_2B5D
        mov    cl, byte ptr [eax]            ; -> obj6:0x0000
        mov    byte ptr [ebx + 0xb], cl
        test   cl, cl
        je     loc_4_2B5D
        cmp    eax, 0xa
        je     loc_4_2A62
        cmp    eax, 0xb
        jne    loc_4_2A75
loc_4_2A62:
        mov    al, byte ptr [ebx + 0xa]
        test   al, 0x80
        je     loc_4_2B4F
        test   al, 7
        je     loc_4_2B4F
loc_4_2A75:
        cmp    byte ptr [ebx + 0xa], 0
        jne    loc_4_2A89
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2A89:
        push   0
        mov    eax, dword ptr [ebx + 0x60]
        push   eax
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [ebx + 0x60]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    ecx, dword ptr [ebx + 0x60]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        push   1
        mov    ecx, dword ptr [ebx + 0x60]
        push   ecx
        mov    edx, dword ptr [ebx + 4]
        push   edx
        call   sub_4_0280
        mov    al, byte ptr [ebx + 0xa]
        and    al, 0x80
        cmp    al, 1
        sbb    eax, eax
        inc    ax
        mov    word ptr [ebx + 8], ax
        test   al, 1
        je     loc_4_2B13
        mov    byte ptr [ebx + 0xe], 0x40
        mov    dword ptr [ebx + 0x1ce], 0x100
        mov    eax, 0xff
        pop    edi
        mov    word ptr [ebx + 0xc0], ax
        pop    esi
        mov    dword ptr [ebx + 0x1c6], 0x4000
        mov    dword ptr [ebx + 0x1ca], 0xc000
        mov    byte ptr [ebx + 0xf], al
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2B13:
        mov    byte ptr [ebx + 0xe], 0x20
        mov    dword ptr [ebx + 0x1ce], 0x100
        mov    eax, 0x80
        pop    edi
        mov    word ptr [ebx + 0xc0], ax
        pop    esi
        mov    dword ptr [ebx + 0x1c6], 0x2000
        mov    dword ptr [ebx + 0x1ca], 0x6000
        mov    byte ptr [ebx + 0xf], al
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2B4F:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2B5D:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_2B6B:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2B80:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 0x7c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        xor    ecx, ecx
        mov    edi, dword ptr [ebp + 0x18]
        mov    word ptr [esi], cx
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 2], di
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    eax, edx
        mov    dword ptr [esi + 0x10], edx
        mov    dword ptr [eax], ecx
        mov    word ptr [eax + 4], cx
loc_4_2BB6:
        lea    eax, [ecx + edi]
        inc    ecx
        cmp    ecx, 0x20
        mov    dword ptr [esi + ecx*4 + 0x3c], eax
        jl     loc_4_2BB6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 0x24]
        mov    ecx, dword ptr [ebp + 0x20]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   edx
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        push   eax
        push   ecx
        push   esi
        call   sub_4_2730
        cmp    eax, 4
        mov    ebx, eax
        jne    loc_4_2C0D
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edi
        push   ecx
        push   edx
        push   esi
        call   sub_4_2940
        cmp    eax, 5
        mov    ebx, eax
        jne    loc_4_2C0D
        mov    ebx, 4
loc_4_2C0D:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
        cmp    ebx, 4
        jge    loc_4_2C39
        test   ebx, ebx
        jl     loc_4_2C39
        mov    word ptr [esi], 2
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    eax, dword ptr [ebx*4]        ; -> obj1:0x0FBC
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    0x28
loc_4_2C39:
        cmp    ebx, 0xa
        jae    loc_4_2C50
        test   ebx, ebx
        jl     loc_4_2C50
        mov    eax, dword ptr [ebx*4]        ; -> obj1:0x0FBC
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dword ptr [ecx], eax
        jmp    loc_4_2C59
loc_4_2C50:
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dword ptr [ecx], 0            ; -> loc_1_1194
loc_4_2C59:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x28
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x32], 0
        mov    word ptr [esi + 0x34], 0
        mov    word ptr [esi], 2
        lea    edi, [esi + 4]
        test   byte ptr [esi + 8], 2
        je     loc_4_2CEB
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [edi]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    bx, ax
        mov    ecx, dword ptr [edi]
        and    bx, 0xfff0
        mov    eax, dword ptr [esi + 0x50]
        or     bh, 0x10
        push   ebx
        push   eax
        push   ecx
        call   sub_4_0290
        push   0x61
        mov    ecx, dword ptr [edi]
        push   ecx
        or     bl, 3
        call   sub_4_0260
        push   0x61
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_4_0260
        push   ebx
        mov    ecx, dword ptr [esi + 0x50]
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 0x48]
        push   ecx
        mov    edx, dword ptr [edi]
        push   edx
        call   sub_4_0280
        jmp    loc_4_2D2B
loc_4_2CEB:
        push   0
        mov    eax, dword ptr [esi + 0x60]
        push   eax
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x60]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    ecx, dword ptr [esi + 0x60]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
        push   1
        mov    ecx, dword ptr [esi + 0x60]
        push   ecx
        mov    edx, dword ptr [edi]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x60]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
loc_4_2D2B:
        mov    ebx, 0x3e8
loc_4_2D30:
        push   0x61
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    loc_4_2D30
        push   0x21
        mov    eax, dword ptr [esi + 0x80]
        push   eax
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x80]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_4_0260
        push   esi
        call   sub_4_3B40
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2D70:
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
        mov    edx, dword ptr [esi + 4]
        push   ecx
        mov    byte ptr [ebp - 1], al
        push   edx
        mov    edi, 0xc7f
        call   sub_4_0280
        mov    word ptr [esi], 4
        push   esi
        call   sub_4_3B40
loc_4_2DBD:
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        dec    edi
        jne    loc_4_2DBD
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0xa8]
        push   ebx
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0280
        push   ebx
        mov    ecx, dword ptr [esi + 0xac]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   2
        mov    ecx, dword ptr [esi + 0xb4]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x9c]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        test   al, 0xa
        je     loc_4_2E61
        push   0xa
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        mov    word ptr [esi + 0x30], bx
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x39]
        mov    dl, byte ptr [esi + ecx*4 + 0xc7]
        lea    eax, [esi + ecx*4]
        mov    byte ptr [esi + 0x39], dl
        mov    byte ptr [eax + 0xc7], bl
        mov    word ptr [eax + 0xc8], bx
        mov    al, byte ptr [esi + 0x38]
        test   al, al
        je     loc_4_2E56
        dec    al
        mov    byte ptr [esi + 0x38], al
loc_4_2E56:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2E61:
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_2E70:
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
        je     loc_4_2EA2
loc_4_2E8D:
        mov    eax, dword ptr [edi + 0x10]
        add    eax, esi
        add    esi, 6
        add    eax, 6
        push   eax
        push   edi
        call   sub_4_2EB0
        dec    ebx
        jne    loc_4_2E8D
loc_4_2EA2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2EB0:
        push   ebp
        mov    ecx, 0xffffffff
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
loc_4_2EC8:
        mov    dl, byte ptr [edi]
        inc    edi
        mov    eax, 8
loc_4_2ED0:
        mov    esi, ecx
        mov    bl, dl
        shr    dl, 1
        and    esi, 0x80000000
        shr    esi, 0x1f
        and    ebx, 1
        xor    esi, ebx
        add    ecx, ecx
        test   esi, esi
        je     loc_4_2EF3
        xor    ecx, 0x4c11db6
        or     ecx, esi
loc_4_2EF3:
        dec    eax
        jne    loc_4_2ED0
        dec    dword ptr [ebp - 4]
        jne    loc_4_2EC8
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
sub_4_2F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    word ptr [esi + 0x30], bx
        mov    word ptr [esi + 0x32], bx
        mov    byte ptr [esi + 0x38], bl
        mov    byte ptr [esi + 0x39], bl
        mov    byte ptr [esi + 0x3a], bl
        mov    byte ptr [esi + 0x3b], bl
        test   byte ptr [esi + 8], 2
        je     loc_4_3226
        push   6
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 0x70]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    byte ptr [ebp - 2], al
        test   al, 0x80
        je     loc_4_3226
        push   esi
        call   sub_4_2C70
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    word ptr [ebp - 4], ax
        push   ebx
        mov    ecx, dword ptr [esi + 0x48]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        push   ebx
        mov    ecx, dword ptr [esi + 0x60]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        mov    edi, 3
        call   sub_4_0290
        mov    ebx, dword ptr [esi + 0x10]
loc_4_2FAE:
        mov    ax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 0x70]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        add    ebx, 2
        push   edx
        call   sub_4_0290
        dec    edi
        jne    loc_4_2FAE
        mov    ebx, dword ptr [esi + 0x10]
        mov    edi, 3
loc_4_2FCD:
        mov    ax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 0x70]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        add    ebx, 2
        push   edx
        call   sub_4_0290
        dec    edi
        jne    loc_4_2FCD
        push   0x2e00
        mov    eax, dword ptr [esi + 0x70]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        push   0
        mov    ecx, dword ptr [esi + 0x70]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   0xf3
        mov    ecx, dword ptr [esi + 0x70]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        push   1
        push   esi
        call   sub_4_3250
        test   eax, eax
        je     loc_4_3042
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        push   0                             ; -> loc_1_1030
        push   1
        push   eax
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3042:
        mov    word ptr [esi], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [esi + 0x50]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0290
        mov    al, byte ptr [ebp - 2]
        push   4
        and    al, 0x40
        mov    ecx, dword ptr [esi + 0x48]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xffffffc1
        add    ax, 0x40
        mov    word ptr [ebp - 6], ax
        call   sub_4_0290
        mov    ecx, dword ptr [esi + 0x60]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0270
        mov    word ptr [ebp - 2], ax
        or     ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 2], ax
loc_4_3094:
        mov    eax, dword ptr [ebp - 2]
        mov    ecx, dword ptr [esi + 0x60]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        call   sub_4_0140
        mov    ecx, eax
        sub    edx, edx
        mov    eax, 0x12c
        div    dword ptr [0]                 ; -> obj7:0x0000
        lea    edi, [ecx + eax + 1]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
loc_4_30C6:
        call   sub_4_0140
        cmp    eax, edi
        jl     loc_4_30C6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        mov    dword ptr [ebp - 0xc], 0
loc_4_30DF:
        push   0xff
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   0x3c
        mov    ecx, dword ptr [esi + 0x94]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0x98]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0x90]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0x26
        mov    ecx, dword ptr [esi + 0x80]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        xor    bl, bl
        call   sub_4_0280
        call   sub_4_0140
        mov    ecx, eax
        sub    edx, edx
        mov    eax, 0x64
        div    dword ptr [0]                 ; -> obj7:0x0000
        lea    edi, [ecx + eax + 1]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0010
loc_4_315F:
        call   sub_4_0140
        cmp    eax, edi
        jge    loc_4_3180
        mov    eax, dword ptr [esi + 0x9c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        and    al, 0xa
        mov    bl, al
        test   bl, bl
        je     loc_4_315F
loc_4_3180:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0
        test   bl, bl
        je     loc_4_31BF
        mov    eax, dword ptr [esi + 0x90]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0x38
        jne    loc_4_31E7
        push   esi
        call   sub_4_2C70
        push   0
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0290
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_31BF:
        push   esi
        call   sub_4_2C70
        push   1
        push   esi
        call   sub_4_3250
        test   eax, eax
        jne    loc_4_322F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x50]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0290
        mov    word ptr [esi], 4
loc_4_31E7:
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 6
        jl     loc_4_30DF
        mov    ax, word ptr [ebp - 2]
        xor    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 2], ax
        test   word ptr [ebp - 6], ax
        je     loc_4_3094
        push   esi
        call   sub_4_2C70
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        push   0                             ; -> obj1:0x1058
        push   1
        push   eax
        call   sub_4_0080
loc_4_3226:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_322F:
        push   1
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        push   0                             ; -> loc_1_1030
        push   1
        push   eax
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_3250:
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
        je     loc_4_3275
        mov    bl, 4
loc_4_3275:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 4
        mov    dword ptr [ebp - 4], eax
        jne    loc_4_3286
        test   byte ptr [ebp + 0xc], 8
        je     loc_4_3289
loc_4_3286:
        or     bl, 8
loc_4_3289:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x10
        mov    dword ptr [ebp - 8], eax
        je     loc_4_3297
        or     bl, 0x1c
loc_4_3297:
        test   byte ptr [ebp + 0xc], 0x20
        je     loc_4_32A0
        or     bl, 3
loc_4_32A0:
        push   0x21
        mov    eax, dword ptr [esi + 0x80]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   esi
        call   sub_4_3B40
        mov    ax, word ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0xb8]
        and    ax, 1
        mov    edx, dword ptr [esi + 4]
        cmp    ax, 1
        sbb    al, al
        add    al, 0x49
        push   eax
        push   ecx
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0xa8]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0xac]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   ebx
        mov    ecx, dword ptr [esi + 0xb0]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        push   2
        mov    ecx, dword ptr [esi + 0xb4]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    cl, byte ptr [esi + 0xe]
        mov    edx, dword ptr [esi + 0x84]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        push   eax
        call   sub_4_0280
        mov    cl, byte ptr [esi + 0xe]
        mov    edx, dword ptr [esi + 0x8c]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        push   eax
        call   sub_4_0280
        mov    cl, byte ptr [esi + 0xf]
        mov    edx, dword ptr [esi + 0x88]
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0280
        test   byte ptr [esi + 8], 2
        je     loc_4_33A8
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
loc_4_33A8:
        push   0xff
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   0x61
        mov    ecx, dword ptr [esi + 0x80]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        xor    ebx, ebx
        call   sub_4_0280
        mov    edi, dword ptr [esi + 0x10]
loc_4_33D4:
        mov    al, byte ptr [edi]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        inc    edi
        mov    eax, dword ptr [esi + 0x84]
        add    eax, ebx
        inc    ebx
        push   eax
        push   ecx
        call   sub_4_0280
        cmp    ebx, 6
        jl     loc_4_33D4
        test   byte ptr [ebp + 0xc], 8
        jne    loc_4_3425
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_3425
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_3444
        lea    edi, [esi + 0x18]
        xor    ebx, ebx
loc_4_3407:
        mov    al, byte ptr [edi]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        inc    edi
        mov    eax, dword ptr [esi + 0xa0]
        add    eax, ebx
        inc    ebx
        push   eax
        push   ecx
        call   sub_4_0280
        cmp    ebx, 8
        jl     loc_4_3407
        jmp    loc_4_3444
loc_4_3425:
        xor    edi, edi
loc_4_3427:
        push   0xff
        mov    eax, dword ptr [esi + 0xa0]
        add    eax, edi
        mov    ecx, dword ptr [esi + 4]
        push   eax
        inc    edi
        push   ecx
        call   sub_4_0280
        cmp    edi, 8
        jl     loc_4_3427
loc_4_3444:
        mov    al, byte ptr [esi + 0xe]
        mov    ecx, dword ptr [esi + 0x9c]
        inc    al
        mov    edx, dword ptr [esi + 4]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0280
        xor    eax, eax
        push   2
        mov    al, byte ptr [esi + 0xe]
        inc    eax
        mov    dword ptr [esi + 0x24], eax
        mov    ecx, dword ptr [esi + 0x80]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0xb4]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x80]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 0xc2
        cmp    al, 2
        je     loc_4_34AB
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_34AB:
        mov    word ptr [esi], 8
        test   byte ptr [esi + 8], 2
        jne    loc_4_34C7
        mov    al, byte ptr [esi + 0xb]
        mov    ecx, dword ptr [esi + 0x60]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0280
loc_4_34C7:
        cmp    byte ptr [esi + 0x38], 0
        je     loc_4_34D3
        push   esi
        call   sub_4_3590
loc_4_34D3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_34E0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [edx], 8
        push   esi
        jne    loc_4_34F9
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_34F9:
        mov    esi, dword ptr [ebp + 0x14]
        xor    eax, eax
        add    esi, 0xff
        mov    al, byte ptr [edx + 0x3a]
        shr    esi, 8
        xor    ebx, ebx
        cmp    byte ptr [edx + 0x38], 0
        mov    bl, byte ptr [edx + eax*4 + 0xc7]
        je     loc_4_3563
        xor    eax, eax
        mov    al, byte ptr [edx + 0x39]
        cmp    ebx, eax
        jbe    loc_4_354C
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0xe]
        sub    ecx, ebx
        cmp    ecx, esi
        jae    loc_4_3544
        cmp    esi, eax
        jbe    loc_4_3542
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 0x1e6]
        pop    ebp
        ret    0x10
loc_4_3542:
        xor    ebx, ebx
loc_4_3544:
        inc    dword ptr [edx + 0x1ea]
        jmp    loc_4_356B
loc_4_354C:
        sub    eax, ebx
        cmp    eax, esi
        jae    loc_4_3544
        mov    eax, 1
        pop    esi
        pop    ebx
        inc    dword ptr [edx + 0x1e6]
        pop    ebp
        ret    0x10
loc_4_3563:
        xor    ebx, ebx
        mov    byte ptr [edx + 0x3a], bl
        mov    byte ptr [edx + 0x39], bl
loc_4_356B:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    byte ptr [edx + 0x3b], bl
        push   ecx
        push   edx
        call   sub_1_001C
loc_4_357C:
        add    esp, 0xc
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3590:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x39]
        mov    ecx, dword ptr [esi + 0x90]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        xor    edi, edi
        push   edx
        call   sub_4_0280
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 0x94]
        mov    cl, byte ptr [esi + 0x39]
        mov    di, word ptr [esi + ecx*4 + 0xc8]
        mov    ecx, dword ptr [esi + 4]
        push   edi
        push   eax
        shr    edi, 8
        push   ecx
        call   sub_4_0280
        push   edi
        mov    eax, dword ptr [esi + 0x98]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   0x26
        mov    ecx, dword ptr [esi + 0x80]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    word ptr [esi + 0x32], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_3600:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_01FC
loc_4_360D:
        add    esp, 4
        test   eax, eax
        jne    loc_4_3681
        xor    ecx, ecx
        mov    dl, byte ptr [esi + 0x3b]
        mov    cx, word ptr [esi + 0x36]
        lea    eax, [ecx + 0xff]
        shr    eax, 8
        add    al, dl
        cmp    byte ptr [esi + 0xe], al
        ja     loc_4_362F
        xor    al, al
loc_4_362F:
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
        jne    loc_4_366A
        mov    al, byte ptr [esi + 0x3b]
        push   esi
        mov    byte ptr [esi + 0x39], al
        call   sub_4_3590
        jmp    loc_4_3679
loc_4_366A:
        mov    cl, byte ptr [esi + 0x3b]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x3a]
        mov    byte ptr [esi + eax*4 + 0xc7], cl
loc_4_3679:
        mov    al, byte ptr [esi + 0x3b]
        mov    byte ptr [esi + 0x3a], al
        xor    eax, eax
loc_4_3681:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     loc_4_375C
        mov    ax, word ptr [esi + 0x32]
        test   ax, ax
        jle    loc_4_375C
        dec    ax
        mov    word ptr [esi + 0x32], ax
        test   ax, ax
        jg     loc_4_375C
        push   esi
        call   sub_4_2C70
        inc    dword ptr [esi + 0x1ee]
        mov    ax, word ptr [esi + 0x30]
        inc    ax
        mov    word ptr [esi + 0x30], ax
        cmp    ax, 2
        jbe    loc_4_370C
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
        je     loc_4_370A
        dec    al
        mov    byte ptr [esi + 0x38], al
loc_4_370A:
        mov    bl, 0x10
loc_4_370C:
        mov    eax, dword ptr [esi + 0xc2]
        push   eax
        push   esi
        call   sub_4_3250
        test   eax, eax
        je     loc_4_3739
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0xa
        push   0                             ; -> loc_1_11B0
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_3739:
        test   bl, bl
        je     loc_4_375C
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10F0
        cmp    eax, -1
        jge    loc_4_375C
loc_4_374C:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_10F0
        cmp    eax, -1
        jl     loc_4_374C
loc_4_375C:
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3770:
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
sub_4_3790:
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
sub_4_37B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        inc    dword ptr [esi + 0x1da]
        call   sub_4_2D70
        push   2
        mov    eax, dword ptr [esi + 0xb4]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   0x22
        mov    ecx, dword ptr [esi + 0x80]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x80]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        and    al, 0xc2
        cmp    al, 2
        je     loc_4_3821
        push   0
        mov    edi, dword ptr [ebp + 0xc]
        push   0
        push   0xc
        push   0                             ; -> loc_1_11C4
        push   1
        push   edi
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3821:
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
        je     loc_4_38FE
loc_4_3843:
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_0298
loc_4_384D:
        add    esp, 4
        and    eax, 0xff
        cmp    eax, dword ptr [esi + 0x24]
        je     loc_4_38FE
        cmp    dword ptr [esi + 0xc2], 0
        mov    eax, 2
        je     loc_4_3872
        push   edi
        call   sub_4_3C00
loc_4_3872:
        cmp    eax, 1
        jne    loc_4_38A0
        mov    al, byte ptr [esi + 0x2d]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x24], ecx
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_4_388D
        mov    al, byte ptr [esi + 0xf]
        dec    al
loc_4_388D:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        push   ecx
        call   sub_4_0280
        jmp    loc_4_38F4
loc_4_38A0:
        cmp    eax, 2
        jne    loc_4_38F0
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_0298
loc_4_38AF:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x24], ecx
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_4_38C5
        mov    al, byte ptr [esi + 0xf]
        dec    al
loc_4_38C5:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        push   ecx
        call   sub_4_0280
        push   5
        mov    ecx, dword ptr [esi + 0x9c]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        inc    dword ptr [esi + 0x1e2]
        jmp    loc_4_38F4
loc_4_38F0:
        test   eax, eax
        jl     loc_4_3956
loc_4_38F4:
        inc    ebx
        cmp    dword ptr [ebp - 4], ebx
        ja     loc_4_3843
loc_4_38FE:
        push   0x10
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   0
        mov    ecx, dword ptr [esi + 0xb4]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        cmp    byte ptr [esi + 0x38], 0
        je     loc_4_3949
        push   0xa
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        push   esi
        call   sub_4_3590
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3949:
        push   0
        push   edi
        call   sub_4_10F0
        cmp    eax, -1
        jl     loc_4_3949
loc_4_3956:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_4_3960:
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
loc_4_397C:
        mov    eax, dword ptr [esi + 0x9c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0x10
        jne    loc_4_3B16
        push   5
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        mov    cx, word ptr [esi + 2]
        push   ecx
        call   sub_1_0298
loc_4_39B0:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        cmp    ecx, dword ptr [esi + 0x24]
        je     loc_4_3A63
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 1
        cmp    dword ptr [esi + 0xc2], 0
        mov    eax, 2
        je     loc_4_39DC
        push   edi
        call   sub_4_3C00
loc_4_39DC:
        cmp    eax, 1
        jne    loc_4_3A0A
        mov    cl, byte ptr [esi + 0x2d]
        xor    eax, eax
        mov    al, cl
        dec    cl
        mov    dword ptr [esi + 0x24], eax
        cmp    byte ptr [esi + 0xe], cl
        jbe    loc_4_39F7
        mov    cl, byte ptr [esi + 0xf]
        dec    cl
loc_4_39F7:
        push   ecx
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        jmp    loc_4_3A67
loc_4_3A0A:
        cmp    eax, 2
        jne    loc_4_3A5A
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_0298
loc_4_3A19:
        add    esp, 4
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x24], ecx
        cmp    byte ptr [esi + 0xe], al
        jbe    loc_4_3A2F
        mov    al, byte ptr [esi + 0xf]
        dec    al
loc_4_3A2F:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 0x8c]
        push   eax
        push   ecx
        call   sub_4_0280
        push   5
        mov    ecx, dword ptr [esi + 0x9c]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_4_0280
        inc    dword ptr [esi + 0x1e2]
        jmp    loc_4_3A63
loc_4_3A5A:
        test   eax, eax
        jge    loc_4_3A67
        jmp    loc_4_3B21
loc_4_3A63:
        mov    byte ptr [ebp - 1], 1
loc_4_3A67:
        mov    eax, dword ptr [esi + 0x9c]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        test   al, 0xa
        je     loc_4_3B09
        mov    word ptr [esi + 0x32], 0
        mov    word ptr [esi + 0x30], 0
        push   0xa
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        mov    ecx, dword ptr [esi + 0x90]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
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
        je     loc_4_3AEC
        dec    al
        mov    byte ptr [esi + 0x38], al
        je     loc_4_3AEC
        push   esi
        call   sub_4_3590
loc_4_3AEC:
        push   ebx
        push   edi
        call   sub_4_10F0
        cmp    eax, -1
        jge    loc_4_3B05
loc_4_3AF8:
        push   0
        push   edi
        call   sub_4_10F0
        cmp    eax, -1
        jl     loc_4_3AF8
loc_4_3B05:
        xor    al, al
        jmp    loc_4_3B0B
loc_4_3B09:
        mov    al, 1
loc_4_3B0B:
        test   byte ptr [ebp - 1], al
        je     loc_4_397C
        jmp    loc_4_3B21
loc_4_3B16:
        push   edi
        push   esi
        call   sub_4_37B0
        mov    byte ptr [ebp - 2], 1
loc_4_3B21:
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
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_3B40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8], 2
        je     loc_4_3B6C
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
loc_4_3B6C:
        push   0
        mov    eax, dword ptr [esi + 0xbc]
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0280
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3B90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x1f
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xbc]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0280
        test   byte ptr [esi + 8], 2
        je     loc_4_3BCC
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
loc_4_3BCC:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3BE0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_4_3BF3
        mov    eax, 0x3f
        pop    ebp
        ret    8
loc_4_3BF3:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xc2]
        ret    8
sub_4_3C00:
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
        jbe    loc_4_3C3D
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_3C3D
        push   0
        push   esi
        call   sub_4_10F0
        cmp    eax, -1
        jne    loc_4_3C3D
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C3D:
        lea    edi, [esi + 0xbd]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0xfc
        push   edi
        push   ecx
        call   sub_1_02B8
loc_4_3C55:
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     loc_4_3CA6
        add    eax, 3
        cmp    eax, 3
        ja     loc_4_3CA6
        jmp    dword ptr [eax*4]             ; -> obj4:0x3CFC
loc_4_3C6E:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C78:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C82:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3C8F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_3FB0
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3CA6:
        sub    dword ptr [ebp - 4], 0xe
        mov    ecx, dword ptr [esi + 0x7f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     loc_4_3CB6
        mov    ecx, eax
loc_4_3CB6:
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
        call   sub_1_0760
loc_4_3CEA:
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
        dd  loc_4_3C6E
        dd  loc_4_3C78
        dd  loc_4_3C82
        dd  loc_4_3C8F
        db  08Dh, 064h, 024h, 000h
loc_4_3D10:
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
        je     loc_4_3D5B
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
        call   sub_1_0BFE
loc_4_3D50:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_4_3D5B:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [ebp + 0x10]
        lea    edi, [ecx + edx]
        cmp    edi, eax
        jbe    loc_4_3D72
        sub    eax, edx
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_4_3D75
loc_4_3D72:
        mov    dword ptr [ebp - 0x18], ecx
loc_4_3D75:
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
        call   sub_1_0BB0
loc_4_3D91:
        test   edi, edi
        je     loc_4_3DE7
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        call   sub_1_0BD4
loc_4_3DA8:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    ebx, eax
        jb     loc_4_3DB1
        mov    ebx, eax
loc_4_3DB1:
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_057C
loc_4_3DC7:
        add    esp, 0x14
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     loc_4_3DE7
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_0BF8
loc_4_3DE1:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_3D91
loc_4_3DE7:
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
sub_4_3E00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        je     loc_4_3E22
loc_4_3E0E:
        mov    ecx, dword ptr [eax + 0x24]
        push   eax
        push   esi
        mov    dword ptr [esi + 0x38], ecx
        call   sub_4_3E30
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        jne    loc_4_3E0E
loc_4_3E22:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3E30:
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
        call   sub_1_0BB0
loc_4_3E4C:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 0x7f]
        add    ebx, 0xe
        cmp    ebx, eax
        jb     loc_4_3E5E
        mov    ebx, eax
loc_4_3E5E:
        lea    eax, [ebx - 0xe]
        push   0
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 4], 0xe
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_0BB0
loc_4_3E78:
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
        call   sub_4_3F00
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
        call   sub_1_0760
loc_4_3EC7:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    byte ptr [esi + 0x41], 1
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0AFA
loc_4_3EE0:
        push   esi
        call   sub_4_1770
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3F00:
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
        call   sub_1_0BD4
loc_4_3F26:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x14]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     loc_4_3FA4
loc_4_3F33:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [eax]
        lea    edx, [ecx + ebx]
        cmp    edx, dword ptr [ebp + 0xc]
        jae    loc_4_3F7E
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_0C6C
loc_4_3F50:
        mov    ecx, dword ptr [ebp + 0x18]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_0BF8
loc_4_3F5E:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0BD4
loc_4_3F74:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    loc_4_3F33
loc_4_3F7E:
        cmp    dword ptr [esi], 0
        je     loc_4_3FA4
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
        call   sub_1_0C6C
loc_4_3F9D:
        mov    ecx, dword ptr [ebp + 0x18]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_3FA4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_3FB0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        test   eax, 0x208
        jne    loc_4_3FFC
        test   eax, 0x206
        je     loc_4_3FD8
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        inc    dword ptr [eax + 0x9b]
        ret    8
loc_4_3FD8:
        test   eax, 0x204
        je     loc_4_3FEC
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        ret    8
loc_4_3FEC:
        test   eax, 0x202
        je     loc_4_3FFC
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x9b]
loc_4_3FFC:
        pop    ebp
        ret    8
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x57  RW-
        db  02Dh, 03Fh, 063h, 06Fh, 06Dh, 06Dh, 065h, 06Eh, 074h, 03Ah, 022h, 05Fh, 048h, 050h, 05Fh, 020h
        db  068h, 070h, 069h, 06Dh, 02Eh, 063h, 028h, 054h, 068h, 075h, 020h, 053h, 065h, 070h, 020h, 032h
        db  033h, 020h, 031h, 038h, 03Ah, 030h, 031h, 03Ah, 035h, 034h, 020h, 031h, 039h, 039h, 033h, 029h
        db  020h, 044h, 02Eh, 030h, 02Eh, 034h, 020h, 063h, 06Fh, 06Dh, 070h, 06Ch, 069h, 065h, 064h, 028h
        db  053h, 065h, 070h, 020h, 032h, 037h, 020h, 031h, 039h, 039h, 034h, 020h, 031h, 030h, 03Ah, 034h
        db  038h, 03Ah, 034h, 037h, 029h, 022h, 020h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x2c  RW-
        db  000h, 000h, 000h, 017h, 019h, 01Bh, 000h, 01Fh, 000h, 015h, 013h, 01Dh, 000h, 000h, 000h, 000h
        db  000h, 002h, 000h, 000h, 040h, 002h, 000h, 000h, 080h, 002h, 000h, 000h, 0C0h, 002h, 000h, 000h
        db  000h, 003h, 000h, 000h, 020h, 003h, 000h, 000h, 040h, 003h, 000h, 000h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0x4  RW-
        db  04Ch, 001h, 005h, 031h
VxD__LDATA ENDS
