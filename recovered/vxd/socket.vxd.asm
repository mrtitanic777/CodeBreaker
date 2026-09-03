; SOCKET -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7241 instructions recovered by recursive descent

Declare_Virtual_Device SOCKET, 3, 10, SOCKET_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2944  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], 0
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_2550
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_1_0036
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0036:
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 0x10]
        mov    ebx, dword ptr [esi + 0x53]
        shl    ebx, 8
        add    ebx, dword ptr [esi + 0xf1]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_0088
        sub    edi, edi
        mov    edi, edi
loc_1_0060:
        cmp    dword ptr [ebp - 0x10], edi
        jne    loc_1_0088
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2598
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0xc], edi
        jne    loc_1_0060
loc_1_0088:
        mov    eax, ebx
        and    eax, 1
        mov    dword ptr [ebp - 0x28], eax
        lea    ecx, [eax + ebx]
        mov    di, word ptr [esi + 0x103]
        mov    dx, di
        sub    al, al
        out    dx, al
        lea    edx, [edi + 8]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0xb]
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x120], 0
        mov    dx, di
        mov    al, 0xa
        out    dx, al
        lea    edx, [edi + 0x10]
        je     loc_1_00E0
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     al, dx
        mov    byte ptr [ebp - 2], al
        jmp    loc_1_00E6
        db  08Bh, 0FFh
loc_1_00E0:
        in     ax, dx
        mov    word ptr [ebp - 6], ax
loc_1_00E6:
        mov    word ptr [ebp - 6], 0xff
        lea    esp, [esp]
loc_1_00F0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    di, ax
        and    edi, 0xffff00ff
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    ax, 0xff
        shl    ax, 8
        or     di, ax
        cmp    di, cx
        jne    loc_1_0151
        dec    word ptr [ebp - 6]
        jne    loc_1_00F0
        cmp    di, cx
        jne    loc_1_0151
        push   ebx
        push   2
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0151:
        mov    cx, word ptr [esi + 0x103]
        cmp    byte ptr [esi + 0x120], 0
        mov    dx, cx
        je     loc_1_0177
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, bh
        jmp    loc_1_0236
loc_1_0177:
        sub    al, al
        out    dx, al
        cmp    dword ptr [ebp - 0x28], 0
        je     loc_1_0224
        lea    edx, [ecx + 8]
        lea    eax, [ebx - 1]
        out    dx, al
        mov    byte ptr [ebp - 0x24], ah
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, 1
        out    dx, al
        lea    edx, [ecx + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        lea    edx, [ecx + 0x10]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    cx, word ptr [esi + 0x103]
        lea    eax, [ebx - 1]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 0x24]
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, 2
        out    dx, al
        lea    edx, [ecx + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0x12
        out    dx, al
        mov    eax, dword ptr [ebp - 0x14]
        lea    edx, [ecx + 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0xffff00ff
        or     ax, cx
        out    dx, ax
        mov    di, 0xff
loc_1_01F7:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   al, 0x40
        jne    loc_1_021A
        dec    di
        push   0x3e8
        call   sub_1_2520
        or     di, di
        jne    loc_1_01F7
loc_1_021A:
        inc    dword ptr [ebp - 0x14]
        inc    ebx
        dec    dword ptr [ebp - 0x20]
        dec    dword ptr [ebp - 0x10]
loc_1_0224:
        mov    cx, word ptr [esi + 0x103]
        mov    eax, ebx
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
loc_1_0236:
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, byte ptr [ebp - 0x20]
        out    dx, al
        lea    edx, [ecx + 0xb]
        mov    al, byte ptr [ebp - 0x1f]
        out    dx, al
        mov    dx, cx
        mov    al, 0x12
        out    dx, al
        sub    ebx, ebx
loc_1_024D:
        cmp    dword ptr [ebp - 0x18], ebx
        je     loc_1_0283
        mov    edx, dword ptr [esi + 0x103]
        mov    eax, dword ptr [ebp - 0x14]
        add    dx, 0x10
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp - 0x1c]
        shl    ax, 8
        mov    cl, byte ptr [ecx]
        and    ecx, 0xffff00ff
        inc    dword ptr [ebp - 0x14]
        or     ax, cx
        dec    dword ptr [ebp - 0x10]
        out    dx, ax
        mov    dword ptr [ebp - 0x18], ebx
loc_1_0283:
        cmp    byte ptr [esi + 0x120], bl
        mov    eax, dword ptr [ebp - 0x10]
        je     loc_1_02A4
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x103]
        add    eax, 0x10
        push   eax
        call   sub_1_23C0
        jmp    loc_1_02D4
loc_1_02A4:
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x103]
        add    eax, 0x10
        push   eax
        call   sub_1_23D4
        test   byte ptr [ebp - 0x10], 1
        je     loc_1_02D4
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x14]
        lea    ecx, [ecx + eax - 1]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 0x18], 1
loc_1_02D4:
        mov    edi, 0xff
        lea    esp, [esp]
loc_1_02E0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0x40
        jne    loc_1_02FE
        dec    edi
        push   0x3e8
        call   sub_1_2520
        cmp    edi, ebx
        jne    loc_1_02E0
loc_1_02FE:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_0353
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_035C
loc_1_0326:
        cmp    dword ptr [ebp - 0x10], ebx
        jne    loc_1_0353
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_0353
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_0326
loc_1_0353:
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_024D
loc_1_035C:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_0372
        mov    edx, dword ptr [esi + 0x103]
        mov    ecx, dword ptr [ebp - 0x1c]
        add    dx, 0x10
        mov    al, byte ptr [ecx]
        out    dx, al
loc_1_0372:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_03AA
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_03AA:
        mov    ebx, dword ptr [ebp + 0xc]
        and    ebx, 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [ebx + eax]
        mov    di, word ptr [esi + 0x103]
        mov    dx, di
        sub    al, al
        out    dx, al
        lea    edx, [edi + 8]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0xb]
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x120], 0
        mov    dx, di
        mov    al, 0xa
        out    dx, al
        lea    edx, [edi + 0x10]
        je     loc_1_0404
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     al, dx
        mov    byte ptr [ebp - 1], al
        jmp    loc_1_040A
loc_1_0404:
        in     ax, dx
        mov    word ptr [ebp - 4], ax
loc_1_040A:
        mov    word ptr [ebp - 4], 0xff
loc_1_0410:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    di, ax
        and    edi, 0xffff00ff
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    ax, 0xff
        shl    ax, 8
        or     di, ax
        cmp    cx, di
        jne    loc_1_0460
        dec    word ptr [ebp - 4]
        jne    loc_1_0410
        cmp    cx, di
        jne    loc_1_0460
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_1_0460:
        cmp    byte ptr [esi + 0x120], 0
        je     loc_1_04B0
        mov    ecx, dword ptr [esi + 0x103]
        mov    dx, cx
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp + 0xd]
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
        lea    edx, [ecx + 0xb]
        mov    al, byte ptr [ebp + 0x15]
        out    dx, al
        mov    dx, cx
        mov    al, 0x12
        out    dx, al
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        add    ecx, 0x10
        push   ecx
        call   sub_1_23C0
        jmp    loc_1_05C2
        db  08Dh, 049h, 000h
loc_1_04B0:
        mov    di, word ptr [esi + 0x103]
        mov    dx, di
        sub    al, al
        out    dx, al
        or     ebx, ebx
        je     loc_1_0570
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [edi + 8]
        lea    ecx, [eax - 1]
        mov    al, cl
        out    dx, al
        mov    bl, ch
        lea    edx, [edi + 9]
        mov    al, bl
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, 1
        out    dx, al
        lea    edx, [edi + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, di
        mov    al, 0xa
        out    dx, al
        lea    edx, [edi + 0x10]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    di, word ptr [esi + 0x103]
        mov    al, cl
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, bl
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, di
        mov    al, 0x12
        out    dx, al
        lea    edx, [edi + 0x10]
        mov    edi, dword ptr [ebp + 0x10]
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [edi]
        and    eax, 0xffff00ff
        shl    ax, 8
        and    ecx, 0xffff00ff
        or     ax, cx
        out    dx, ax
        mov    bx, 0xff
        lea    ebx, [ebx]
loc_1_0540:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   al, 0x40
        jne    loc_1_0563
        dec    bx
        push   0x3e8
        call   sub_1_2520
        or     bx, bx
        jne    loc_1_0540
loc_1_0563:
        inc    edi
        inc    dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0x14]
        jmp    loc_1_0573
        db  08Dh, 064h, 024h, 000h
loc_1_0570:
        mov    edi, dword ptr [ebp + 0x10]
loc_1_0573:
        mov    ecx, dword ptr [esi + 0x103]
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
        lea    edx, [ecx + 0xb]
        mov    al, byte ptr [ebp + 0x15]
        out    dx, al
        mov    dx, cx
        mov    al, 0x12
        out    dx, al
        mov    eax, dword ptr [ebp + 0x14]
        shr    eax, 1
        push   eax
        push   edi
        add    ecx, 0x10
        push   ecx
        call   sub_1_23D4
        test   byte ptr [ebp + 0x14], 1
        je     loc_1_05C2
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        mov    eax, dword ptr [ebp + 0x14]
        mov    al, byte ptr [edi + eax - 1]
        out    dx, al
loc_1_05C2:
        mov    di, 0xff
        mov    ebx, 0x40
        add    eax, 0
loc_1_05D0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   bl, al
        jne    loc_1_05F3
        dec    di
        push   0x3e8
        call   sub_1_2520
        or     di, di
        jne    loc_1_05D0
loc_1_05F3:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_0600:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_0620
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0620:
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 0x103]
        in     al, dx
        cmp    byte ptr [esi + 0x120], 0
        je     loc_1_0677
        mov    ebx, dword ptr [esi + 0x103]
        mov    dx, bx
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 0x10]
        lea    edx, [ebx + 8]
        out    dx, al
        lea    edx, [ebx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ebx + 0xa]
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
        lea    edx, [ebx + 0xb]
        mov    al, byte ptr [ebp + 0x15]
        out    dx, al
        mov    dx, bx
        mov    al, 0xa
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        add    ebx, 0x10
        push   ebx
        call   sub_1_23FD
        jmp    loc_1_0732
loc_1_0677:
        mov    cx, word ptr [esi + 0x103]
        mov    dx, cx
        sub    al, al
        out    dx, al
        test   byte ptr [ebp + 0x10], 1
        je     loc_1_06E0
        mov    eax, dword ptr [ebp + 0x10]
        dec    eax
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, 2
        out    dx, al
        lea    edx, [ecx + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        lea    edx, [ecx + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        in     ax, dx
        mov    byte ptr [edi], ah
        mov    bl, 0xff
loc_1_06B6:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0x40
        jne    loc_1_06D5
        dec    bl
        push   0x3e8
        call   sub_1_2520
        or     bl, bl
        jne    loc_1_06B6
loc_1_06D5:
        inc    dword ptr [ebp + 0x10]
        inc    edi
        dec    dword ptr [ebp + 0x14]
        jmp    loc_1_06E3
        db  08Bh, 0FFh
loc_1_06E0:
        mov    edi, dword ptr [ebp + 0xc]
loc_1_06E3:
        mov    ecx, dword ptr [esi + 0x103]
        mov    eax, dword ptr [ebp + 0x10]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
        lea    edx, [ecx + 0xb]
        mov    al, byte ptr [ebp + 0x15]
        out    dx, al
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        mov    eax, dword ptr [ebp + 0x14]
        shr    eax, 1
        push   eax
        push   edi
        add    ecx, 0x10
        push   ecx
        call   sub_1_2411
        test   byte ptr [ebp + 0x14], 1
        je     loc_1_0732
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     al, dx
        mov    ecx, dword ptr [ebp + 0x14]
        mov    byte ptr [edi + ecx - 1], al
loc_1_0732:
        mov    bl, 0xff
loc_1_0734:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0x40
        jne    loc_1_0753
        dec    bl
        push   0x3e8
        call   sub_1_2520
        or     bl, bl
        jne    loc_1_0734
loc_1_0753:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_0760:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 0x103]
        mov    al, 0x21
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 0x5b]
        mov    si, word ptr [edi + 0x103]
        mov    ebx, dword ptr [edi + ecx*4 + 0xc1]
        lea    edx, [esi + 4]
        mov    al, byte ptr [edi + 0xfd]
        add    al, cl
        out    dx, al
        cmp    ebx, 0x3c
        jae    loc_1_07B3
        mov    ebx, 0x3c
loc_1_07B3:
        lea    edx, [esi + 6]
        mov    al, bh
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, bl
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [edi + 0x103]
        mov    al, 0x26
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07E0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 0x62
        mov    dx, word ptr [ecx + 0x103]
        out    dx, al
        add    dx, 7
        in     al, dx
        mov    byte ptr [ecx + 0x101], al
        mov    dx, word ptr [ecx + 0x103]
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0810:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx + 0x103]
        add    dx, 4
        in     al, dx
        mov    byte ptr [ecx + 0x102], al
        sub    al, al
        ret    4
        db  08Dh, 049h, 000h
sub_1_0830:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x100]
        cmp    byte ptr [ecx + 0xfe], al
        mov    edx, dword ptr [ecx + 0x103]
        jne    loc_1_0860
        add    dx, 3
        mov    al, byte ptr [ecx + 0xff]
        dec    al
        out    dx, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0860:
        add    dx, 3
        dec    al
        out    dx, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0870:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 0x103]
        add    dx, 0xc
        mov    al, byte ptr [eax + 0x15f]
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 049h, 000h
loc_1_0890:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x107], 4
        je     loc_1_08A5
        push   eax
        call   sub_1_08B0
loc_1_08A5:
        sub    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_08B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xd
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [esi + 0x103]
        add    dword ptr [esi + 0x136], ecx
        add    dx, 0xe
        in     al, dx
        xor    ecx, ecx
        mov    cl, al
        mov    edx, dword ptr [esi + 0x103]
        add    dword ptr [esi + 0x13a], ecx
        add    dx, 0xf
        in     al, dx
        and    eax, 0xff
        add    dword ptr [esi + 0x13e], eax
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0910:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0760
        push   0x7d0
        call   sub_1_2520
        test   byte ptr [esi + 0x107], 0xa
        jne    loc_1_094F
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0xa
        jne    loc_1_094F
        mov    al, byte ptr [esi + 0x5f]
        mov    byte ptr [esi + 0x60], al
        mov    byte ptr [esi + 0x1b5], 0
loc_1_094F:
        mov    byte ptr [esi + 0x5f], 0
        mov    cx, word ptr [esi + 0x103]
        mov    dword ptr [esi + 0x14b], 0
        lea    edx, [ecx + 0xb]
        sub    al, al
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
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0990:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 0x103]
        add    dx, 0xf
        sub    al, al
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_09B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_1_0CE0
        mov    byte ptr [esi + 0x121], 1
        mov    edx, dword ptr [esi + 0x103]
        inc    dword ptr [esi + 0x30]
        add    dx, 7
        in     al, dx
        mov    byte ptr [esi + 0x107], al
        cmp    al, 0xff
        jne    loc_1_0A2D
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        sub    al, al
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, 0xff
        jne    loc_1_0A20
        mov    byte ptr [esi + 0x107], 0
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 0x1a9]
        push   eax
        call   sub_1_2508
        mov    dword ptr [esi + 0x2bf], 1
        jmp    loc_1_0A2D
        db  090h
loc_1_0A20:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        mov    al, 0xff
        out    dx, al
loc_1_0A2D:
        mov    al, byte ptr [esi + 0x107]
        or     al, al
        je     loc_1_0A42
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        out    dx, al
loc_1_0A42:
        test   al, 0x20
        je     loc_1_0A50
        mov    eax, 8
        jmp    loc_1_0A86
        db  08Dh, 049h, 000h
loc_1_0A50:
        test   al, 0x10
        je     loc_1_0A61
        push   esi
        call   sub_1_08B0
        mov    eax, 4
        jmp    loc_1_0A86
loc_1_0A61:
        test   al, 0xa
        je     loc_1_0A6C
        mov    eax, 2
        jmp    loc_1_0A86
loc_1_0A6C:
        test   al, 1
        je     loc_1_0A72
        jmp    loc_1_0A81
loc_1_0A72:
        test   al, 4
        mov    eax, 0x10
        je     loc_1_0A86
        push   esi
        call   sub_1_08B0
loc_1_0A81:
        mov    eax, 1
loc_1_0A86:
        sub    ebx, ebx
loc_1_0A88:
        cmp    eax, 0x10
        jne    loc_1_0AA5
        cmp    dword ptr [esi + 0x165], ebx
        je     loc_1_0BB0
        cmp    byte ptr [esi + 0x142], bl
        jne    loc_1_0BB0
loc_1_0AA5:
        cmp    eax, 1
        je     loc_1_0B12
        cmp    eax, 2
        je     loc_1_0AC0
        cmp    eax, 4
        je     loc_1_0B04
        cmp    eax, 8
        je     loc_1_0B53
        jmp    loc_1_0B29
        db  090h
loc_1_0AC0:
        push   esi
        call   sub_1_0810
        mov    byte ptr [esi + 0x1b5], bl
        cmp    byte ptr [esi + 0x11f], bl
        jne    loc_1_0AFB
        mov    byte ptr [esi + 0x5f], bl
        mov    dword ptr [esi + 0x14b], ebx
        test   byte ptr [esi + 0x107], 8
        je     loc_1_0AEC
        push   esi
        call   sub_2_08F0
loc_1_0AEC:
        test   byte ptr [esi + 0x107], 2
        je     loc_1_0AFB
        push   esi
        call   sub_1_0F40
loc_1_0AFB:
        and    byte ptr [esi + 0x107], 0xf5
        jmp    loc_1_0B29
loc_1_0B04:
        mov    byte ptr [esi + 0x11c], 1
        and    byte ptr [esi + 0x107], 0xef
loc_1_0B12:
        push   esi
        call   sub_1_0CF0
        cmp    al, bl
        je     loc_1_0B23
        and    byte ptr [esi + 0x107], 0xfa
loc_1_0B23:
        push   esi
        call   sub_1_1B40
loc_1_0B29:
        cmp    dword ptr [esi + 0x165], ebx
        je     loc_1_0B3F
        cmp    byte ptr [esi + 0x142], bl
        jne    loc_1_0B3F
        push   esi
        call   sub_1_0CF0
loc_1_0B3F:
        mov    al, byte ptr [esi + 0x107]
        test   al, 0x20
        je     loc_1_0B62
        mov    eax, 8
        jmp    loc_1_0A88
loc_1_0B53:
        push   esi
        call   sub_1_08B0
        and    byte ptr [esi + 0x107], 0xdf
        jmp    loc_1_0B29
loc_1_0B62:
        test   al, 0x10
        je     loc_1_0B76
        push   esi
        call   sub_1_08B0
        mov    eax, 4
        jmp    loc_1_0A88
loc_1_0B76:
        test   al, 0xa
        je     loc_1_0B84
        mov    eax, 2
        jmp    loc_1_0A88
loc_1_0B84:
        test   al, 1
        je     loc_1_0B92
        mov    eax, 1
        jmp    loc_1_0A88
loc_1_0B92:
        test   al, 4
        je     loc_1_0BA6
        push   esi
        call   sub_1_08B0
        mov    eax, 1
        jmp    loc_1_0A88
loc_1_0BA6:
        mov    eax, 0x10
        jmp    loc_1_0A88
loc_1_0BB0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        mov    byte ptr [esi + 0x107], al
        cmp    al, 0xff
        jne    loc_1_0C0D
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        mov    al, bl
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0xff
        jne    loc_1_0C00
        mov    byte ptr [esi + 0x107], bl
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [esi + 0x1a9]
        push   eax
        call   sub_1_2508
        mov    dword ptr [esi + 0x2bf], 1
        jmp    loc_1_0C0D
        db  08Dh, 064h, 024h, 000h
loc_1_0C00:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        mov    al, 0xff
        out    dx, al
loc_1_0C0D:
        mov    al, byte ptr [esi + 0x107]
        cmp    al, bl
        je     loc_1_0C22
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        out    dx, al
loc_1_0C22:
        test   al, 0x20
        je     loc_1_0C30
        mov    eax, 8
        jmp    loc_1_0C66
        db  08Dh, 049h, 000h
loc_1_0C30:
        test   al, 0x10
        je     loc_1_0C41
        push   esi
        call   sub_1_08B0
        mov    eax, 4
        jmp    loc_1_0C66
loc_1_0C41:
        test   al, 0xa
        je     loc_1_0C4C
        mov    eax, 2
        jmp    loc_1_0C66
loc_1_0C4C:
        test   al, 1
        je     loc_1_0C52
        jmp    loc_1_0C61
loc_1_0C52:
        test   al, 4
        mov    eax, 0x10
        je     loc_1_0C66
        push   esi
        call   sub_1_08B0
loc_1_0C61:
        mov    eax, 1
loc_1_0C66:
        cmp    eax, 0x10
        jne    loc_1_0AA5
        mov    byte ptr [esi + 0x121], 0
        mov    byte ptr [esi + 0x160], 0x1b
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xf
        mov    al, 0x1b
        out    dx, al
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_0CC1
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_0CAB
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_1_0CAB
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_0CC1
loc_1_0CAB:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0CC1:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_1_0CE0
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_0CE0
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_1_0CE0
        push   esi
        call   sub_1_1B40
loc_1_0CE0:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        mov    byte ptr [esi + 0x11e], 0
        push   esi
        call   sub_1_07E0
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_0D2E
        cmp    byte ptr [esi + 0x11c], 0
        je     loc_1_0D2E
        push   esi
        push   0x910                         ; -> loc_1_0910
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_251A
loc_1_0D2E:
        mov    ebx, 4
loc_1_0D33:
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_0E29
        test   byte ptr [esi + 0x107], bl
        jne    loc_1_0EE0
        mov    al, byte ptr [esi + 0x100]
        cmp    byte ptr [esi + 0x101], al
        jne    loc_1_0D72
        push   esi
        call   sub_1_07E0
        mov    al, byte ptr [esi + 0x100]
        cmp    byte ptr [esi + 0x101], al
        je     loc_1_0E29
loc_1_0D72:
        inc    dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0x16d], 0
        push   esi
        call   sub_1_1440
        or     al, al
        je     loc_1_0DE1
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x1ba]
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 0x1b9]
        lea    eax, [ecx + eax - 4]
        mov    ecx, dword ptr [esi + 0x161]
        cmp    ecx, eax
        jb     loc_1_0DAC
        mov    ecx, eax
loc_1_0DAC:
        add    ecx, 0xe
        push   ecx
        mov    eax, dword ptr [esi + 0x1bb]
        push   eax
        lea    eax, [esi + 0x1bf]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        jne    loc_1_0DCA
        jmp    loc_1_0DE1
loc_1_0DCA:
        push   esi
        call   sub_1_1600
        cmp    eax, 3
        je     loc_1_0F00
        inc    dword ptr [esi + 0x126]
        jmp    loc_1_0DE6
loc_1_0DE1:
        mov    eax, 1
loc_1_0DE6:
        cmp    eax, 3
        je     loc_1_0F00
        cmp    eax, 1
        jne    loc_1_0E02
        push   esi
        call   sub_1_07E0
        mov    al, byte ptr [esi + 0x101]
        jmp    loc_1_0E08
loc_1_0E02:
        mov    al, byte ptr [esi + 0x1b8]
loc_1_0E08:
        mov    byte ptr [esi + 0x100], al
        push   esi
        call   sub_1_0830
        cmp    dword ptr [esi + 0x34], 0xa
        jbe    loc_1_0D33
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [esi + 0x34], 0
loc_1_0E29:
        cmp    byte ptr [esi + 0x11c], 0
        je     loc_1_0E76
        mov    byte ptr [esi + 0x11c], 0
        push   esi
        push   0x890                         ; -> loc_1_0890
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_251A
        mov    eax, dword ptr [esi + 0x103]
        add    ax, 0xd
        mov    dx, ax
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x60], al
        je     loc_1_0E76
        cmp    dword ptr [esi + 0x5b], -1
        je     loc_1_0E76
        mov    byte ptr [esi + 0x60], al
        mov    byte ptr [esi + 0x1b5], al
        mov    byte ptr [esi + 0x5f], 1
        push   esi
        call   sub_1_0780
loc_1_0E76:
        cmp    dword ptr [esi + 0x165], 0
        je     loc_1_0ECA
        nop    
loc_1_0E80:
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_0ED3
        mov    ebx, dword ptr [esi + 0x165]
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x165], eax
        mov    dword ptr [esi + 0x16d], ebx
        mov    eax, ebx
        inc    dword ptr [esi + 0x122]
        mov    edi, dword ptr [ebx + 0x28]
        push   eax
        push   esi
        call   sub_1_12B0
        push   0
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x165], 0
        jne    loc_1_0E80
loc_1_0ECA:
        cmp    byte ptr [esi + 0x142], 0
        je     loc_1_0F12
loc_1_0ED3:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0EE0:
        push   esi
        call   sub_1_07E0
        mov    al, byte ptr [esi + 0x101]
        mov    byte ptr [esi + 0x100], al
        push   esi
        call   sub_1_0830
        jmp    loc_1_0E29
        db  08Dh, 049h, 000h
loc_1_0F00:
        mov    byte ptr [esi + 0x160], 0x1a
        push   esi
        call   sub_2_06F0
        jmp    loc_1_0E29
loc_1_0F12:
        cmp    byte ptr [esi + 0x11e], 0
        je     loc_1_0F2E
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_1_22B0
        mov    byte ptr [esi + 0x11e], 0
loc_1_0F2E:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x11f], 1
        cmp    dword ptr [esi + 0x5b], -1
        jne    loc_1_0F80
        mov    byte ptr [esi + 0x11f], 0
        push   4
        push   1
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0F80:
        push   esi
        call   sub_1_0810
        test   byte ptr [esi + 0x102], 1
        je     loc_1_0F97
        inc    dword ptr [esi + 0x122]
        jmp    loc_1_0F9D
loc_1_0F97:
        inc    dword ptr [esi + 0x12a]
loc_1_0F9D:
        mov    eax, dword ptr [esi + 0x5b]
        mov    eax, dword ptr [esi + eax*4 + 0xc1]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [esi + 0x5b]
        add    eax, edx
        cmp    eax, edx
        jbe    loc_1_0FD4
        lea    eax, [esi + edx*4 + 0x61]
        nop    
loc_1_0FC0:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 0x5b]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     loc_1_0FC0
loc_1_0FD4:
        mov    eax, dword ptr [esi + 0x57]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x57], eax
        cmp    eax, 0xc
        jne    loc_1_0FE9
        mov    dword ptr [esi + 0x57], 0
loc_1_0FE9:
        mov    eax, dword ptr [esi + 0x57]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_0FFF
        cmp    dword ptr [esi + 0x53], eax
        je     loc_1_0FFF
        mov    dword ptr [esi + 0x57], 0
loc_1_0FFF:
        mov    eax, dword ptr [esi + 0x5b]
        mov    dword ptr [esi + eax*4 + 0x91], 0
        mov    eax, dword ptr [esi + 0x57]
        mov    ecx, dword ptr [esi + eax*4 + 0x61]
        or     ecx, ecx
        je     loc_1_1040
        cmp    ecx, 1
        je     loc_1_1230
        cmp    ecx, 2
        je     loc_1_1250
        mov    byte ptr [esi + 0x11f], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1040:
        cmp    byte ptr [esi + 0x142], 0
        je     loc_1_1068
        mov    dword ptr [esi + 0x5b], 0xffffffff
        mov    byte ptr [esi + 0x11f], 0
        push   2
        push   esi
        call   sub_2_2160
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1068:
        mov    ebx, dword ptr [esi + 0x4b]
        or     ebx, ebx
        je     loc_1_1215
        mov    eax, dword ptr [ebx + 0x24]
        lea    edi, [ebx + 0x24]
        mov    dword ptr [esi + 0x4b], eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        push   ebx
        call   sub_1_2550
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 0x53]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_10AC
        mov    dword ptr [esi + 0x53], 0
loc_1_10AC:
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [esi + 0x57], eax
        mov    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        cmp    eax, ecx
        jbe    loc_1_10D5
        lea    eax, [esi + ecx*4 + 0x61]
loc_1_10C1:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [esi + 0x53]
        add    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        ja     loc_1_10C1
loc_1_10D5:
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [esi + eax*4 + 0x91], ebx
        mov    eax, dword ptr [esi + 0x57]
        mov    dword ptr [esi + 0x5b], eax
        mov    eax, dword ptr [esi + 0x53]
        lea    eax, [esi + eax*4]
        add    eax, 0xc1
        push   eax
        push   ebx
        push   esi
        call   0
        or     al, al
        jne    loc_1_1151
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        mov    edx, dword ptr [esi + 0x53]
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        cmp    eax, edx
        jbe    loc_1_1134
        lea    eax, [esi + edx*4 + 0x61]
loc_1_1120:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 0x53]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     loc_1_1120
loc_1_1134:
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [esi + eax*4 + 0x91], 0
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1151:
        mov    ecx, dword ptr [esi + 0x53]
        mov    eax, dword ptr [esi + ecx*4 + 0xc1]
        cmp    eax, 0x3c
        jae    loc_1_1181
        mov    edx, 0x3c
        sub    edx, eax
        push   edx
        push   0x2728                        ; -> loc_1_2728
        mov    edx, dword ptr [esi + 0xf1]
        shl    ecx, 8
        add    edx, ecx
        add    edx, eax
        push   edx
        push   esi
        call   sub_1_0390
loc_1_1181:
        mov    eax, dword ptr [esi + 0x53]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x53], eax
        cmp    eax, 0xc
        jne    loc_1_1196
        mov    dword ptr [esi + 0x53], 0
loc_1_1196:
        mov    byte ptr [esi + 0x11f], 0
        cmp    byte ptr [esi + 0x11c], 0
        je     loc_1_11B0
        mov    byte ptr [esi + 0x60], 1
        jmp    loc_1_11BA
        db  08Dh, 064h, 024h, 000h
loc_1_11B0:
        mov    byte ptr [esi + 0x5f], 1
        push   esi
        call   sub_1_0780
loc_1_11BA:
        cmp    byte ptr [edi + 8], 0
        jne    loc_1_11E0
        mov    edi, dword ptr [edi + 4]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_246A
        dec    dword ptr [edi + 0x1c]
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11E0:
        cmp    dword ptr [esi + 0x165], 0
        jne    loc_1_11F1
        mov    dword ptr [esi + 0x165], ebx
        jmp    loc_1_11FA
loc_1_11F1:
        mov    eax, dword ptr [esi + 0x169]
        mov    dword ptr [eax + 0x24], ebx
loc_1_11FA:
        mov    dword ptr [esi + 0x169], ebx
        mov    dword ptr [edi], 0
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1215:
        mov    dword ptr [esi + 0x5b], 0xffffffff
        mov    byte ptr [esi + 0x11f], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1230:
        mov    dword ptr [esi + 0x5b], 0xffffffff
        mov    byte ptr [esi + 0x11f], 0
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1250:
        cmp    byte ptr [esi + 0x142], 0
        je     loc_1_1283
        mov    dword ptr [esi + 0x5b], 0xffffffff
        mov    eax, dword ptr [esi + 0x57]
        mov    dword ptr [esi + eax*4 + 0x61], 0
        mov    byte ptr [esi + 0x11f], 0
        push   2
        push   esi
        call   sub_2_2160
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1283:
        mov    dword ptr [esi + 0x5b], eax
        mov    byte ptr [esi + 0x11f], 0
        mov    byte ptr [esi + 0x5f], 1
        push   esi
        call   sub_1_0780
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12B0:
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
        call   sub_1_2550
        cmp    dword ptr [ebp - 4], 6
        jae    loc_1_12E0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12E0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 0x161]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_12F5
        mov    edi, eax
loc_1_12F5:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0x1bf]
        push   eax
        call   sub_1_1360
        cmp    edi, 0xe
        jae    loc_1_1318
        cmp    edi, 6
        jb     loc_1_1341
        push   0
        push   0
        push   0
        push   edi
        jmp    loc_1_132C
loc_1_1318:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        sub    edi, 0xe
        push   edi
        lea    eax, [esi + 0x1cd]
        push   eax
        push   0xe
loc_1_132C:
        lea    eax, [esi + 0x1bf]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_1_20D0
loc_1_1341:
        mov    byte ptr [esi + 0x11e], 1
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1360:
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
        call   sub_1_2550
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 0x10]
        je     loc_1_13B4
loc_1_1385:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2574
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_13B4
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1385
loc_1_13B4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_13C5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_13C5:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0xc], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_13D4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_13E3
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_13E6
loc_1_13E3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_13E6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_260C
        add    esi, ebx
        cmp    esi, edi
        je     loc_1_1422
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_142D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2574
        jmp    loc_1_13D4
loc_1_1422:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_142D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1440:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0xfe]
        mov    al, byte ptr [esi + 0x100]
        sub    eax, ecx
        shl    eax, 8
        add    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [ebp - 0xc], eax
        push   6
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        jne    loc_1_1486
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1486:
        add    dword ptr [ebp - 0xc], 4
        sub    ecx, ecx
        lea    edx, [ebp - 0x1c]
        nop    
loc_1_1490:
        lea    edi, [edx + 1]
        mov    dword ptr [ebp - 8], edi
        lea    edi, [edx + 2]
        mov    dword ptr [ebp - 0x10], edi
        mov    bl, byte ptr [edx + 3]
        lea    eax, [edx + 3]
        mov    byte ptr [ebp - 2], bl
        lea    edi, [edx + 4]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [edx], 0x5e
        je     loc_1_14B3
        jmp    loc_1_1527
loc_1_14B3:
        test   byte ptr [edx], 0x20
        je     loc_1_14C3
        or     ecx, ecx
        jne    loc_1_14C3
        test   byte ptr [edi], 1
        jne    loc_1_14C3
        jmp    loc_1_1527
loc_1_14C3:
        mov    edi, dword ptr [ebp - 0x10]
        cmp    byte ptr [edi], bl
        jne    loc_1_14E5
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], bl
        mov    bl, byte ptr [edi]
        sub    bl, byte ptr [esi + 0x100]
        dec    bl
        mov    edi, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], bl
        cmp    byte ptr [edi], 0xfc
        jbe    loc_1_14E5
        inc    eax
loc_1_14E5:
        xor    ebx, ebx
        mov    bl, byte ptr [eax]
        mov    edi, dword ptr [ebp - 0x10]
        shl    ebx, 8
        movzx  edi, byte ptr [edi]
        lea    edi, [ebx + edi - 4]
        cmp    edi, 0x5ea
        ja     loc_1_1503
        cmp    edi, 0x3c
        jae    loc_1_150C
loc_1_1503:
        mov    bl, byte ptr [ebp - 2]
        mov    byte ptr [eax], bl
        mov    byte ptr [ebp - 1], 1
loc_1_150C:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1531
        mov    edi, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi + 0xfe], al
        ja     loc_1_1527
        cmp    byte ptr [esi + 0xff], al
        jae    loc_1_152B
loc_1_1527:
        mov    byte ptr [ebp - 1], 1
loc_1_152B:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1567
loc_1_1531:
        or     ecx, ecx
        je     loc_1_1540
        cmp    ecx, 1
        je     loc_1_1555
        jmp    loc_1_1566
        db  08Dh, 064h, 024h, 000h
loc_1_1540:
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0xc]
        cmp    byte ptr [esi + 0x120], 0
        jne    loc_1_1566
        inc    edx
        inc    dword ptr [ebp - 0xc]
        jmp    loc_1_1566
loc_1_1555:
        dec    edx
        dec    dword ptr [ebp - 0xc]
        cmp    byte ptr [esi + 0x120], 0
        jne    loc_1_1566
        dec    edx
        dec    dword ptr [ebp - 0xc]
loc_1_1566:
        inc    ecx
loc_1_1567:
        cmp    ecx, 2
        jae    loc_1_1576
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1490
loc_1_1576:
        mov    al, byte ptr [edx]
        mov    byte ptr [esi + 0x1b7], al
        mov    al, byte ptr [edx + 1]
        inc    edx
        mov    byte ptr [esi + 0x1b8], al
        mov    al, byte ptr [edx + 1]
        inc    edx
        mov    byte ptr [esi + 0x1b9], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esi + 0x1ba], al
        mov    eax, dword ptr [ebp - 0xc]
        xor    ecx, ecx
        mov    dword ptr [esi + 0x1bb], eax
        xor    eax, eax
        mov    al, byte ptr [esi + 0x1ba]
        shl    eax, 8
        mov    cl, byte ptr [esi + 0x1b9]
        lea    eax, [eax + ecx - 4]
        cmp    eax, 0x5ea
        ja     loc_1_15D2
        cmp    eax, 0x3c
        jb     loc_1_15D2
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15D2:
        mov    al, byte ptr [esi + 0x1b8]
        cmp    byte ptr [esi + 0xfe], al
        ja     loc_1_15F3
        cmp    byte ptr [esi + 0xff], al
        jb     loc_1_15F3
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15F3:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1600:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0x1ba]
        mov    al, byte ptr [esi + 0x100]
        lea    ecx, [ebx + eax + 1]
        mov    dl, byte ptr [esi + 0xff]
        cmp    cl, dl
        jb     loc_1_162A
        sub    cl, dl
        add    cl, byte ptr [esi + 0xfe]
loc_1_162A:
        mov    al, byte ptr [esi + 0x1b8]
        cmp    cl, al
        je     loc_1_1650
        inc    cl
        cmp    cl, dl
        jne    loc_1_1640
        mov    cl, byte ptr [esi + 0xfe]
loc_1_1640:
        cmp    cl, al
        je     loc_1_1650
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_1650:
        cmp    byte ptr [esi + 0xfe], al
        ja     loc_1_16E8
        cmp    al, dl
        ja     loc_1_16E8
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x1b9]
        xor    eax, eax
        mov    al, bl
        shl    eax, 8
        lea    eax, [ecx + eax - 4]
        cmp    eax, 0x5ea
        jbe    loc_1_1690
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1690:
        mov    ecx, dword ptr [esi + 0x161]
        add    ecx, 0xe
        cmp    ecx, eax
        jb     loc_1_169F
        mov    ecx, eax
loc_1_169F:
        mov    dword ptr [esi + 0x2bb], eax
        cmp    ecx, 0xe
        jae    loc_1_16B3
        push   0
        push   0
        push   0
        push   ecx
        jmp    loc_1_16C4
loc_1_16B3:
        sub    eax, 0xe
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    eax, [esi + 0x1cd]
        push   eax
        push   0xe
loc_1_16C4:
        lea    eax, [esi + 0x1bf]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_1_20D0
        mov    byte ptr [esi + 0x11e], 1
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_16E8:
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1700:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x16d], 0
        je     loc_1_17A7
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebx, edi
        push   eax
        call   sub_1_2550
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_177A
loc_1_173A:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x16d]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_1360
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 0xc]
        jb     loc_1_177A
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_173A
loc_1_177A:
        cmp    edi, ebx
        jbe    loc_1_1795
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1795:
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
loc_1_17A7:
        mov    eax, dword ptr [esi + 0x2bb]
        mov    ecx, dword ptr [ebp + 0x14]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    loc_1_17D9
        cmp    edi, eax
        jbe    loc_1_17D2
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_17D2:
        sub    eax, edi
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_17DC
loc_1_17D9:
        mov    dword ptr [ebp - 0x20], ecx
loc_1_17DC:
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x14], eax
        add    edi, dword ptr [esi + 0x1bb]
        mov    eax, dword ptr [esi + 0xf9]
        cmp    edi, eax
        jbe    loc_1_17FA
        sub    edi, eax
        add    edi, dword ptr [esi + 0xf5]
loc_1_17FA:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_2550
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_1_18BB
        nop    
loc_1_1830:
        mov    ebx, dword ptr [ebp - 0xc]
        sub    ebx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x14], ebx
        jae    loc_1_183E
        mov    ebx, dword ptr [ebp - 0x14]
loc_1_183E:
        mov    eax, dword ptr [esi + 0xf9]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    loc_1_184F
        mov    ebx, eax
        sub    ebx, edi
loc_1_184F:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, dword ptr [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        je     loc_1_1931
        add    edi, ebx
        sub    dword ptr [ebp - 0x14], ebx
        je     loc_1_18BB
        cmp    dword ptr [esi + 0xf9], edi
        jne    loc_1_187B
        mov    edi, dword ptr [esi + 0xf5]
loc_1_187B:
        add    dword ptr [ebp - 0x10], ebx
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp - 0xc]
        jne    loc_1_18B1
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_18BB
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        mov    dword ptr [ebp - 0x10], 0
loc_1_18B1:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_1830
loc_1_18BB:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [esi + 0x11c], 0
        jne    loc_1_1A07
        cmp    dword ptr [esi + 0x5b], -1
        je     loc_1_1A07
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 8
        je     loc_1_18F5
        push   esi
        call   sub_2_08F0
loc_1_18F5:
        test   byte ptr [ebp - 1], 2
        je     loc_1_1A07
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        mov    al, 2
        out    dx, al
        push   esi
        call   sub_1_0810
        mov    byte ptr [esi + 0x5f], 0
        mov    dword ptr [esi + 0x14b], 0
        test   byte ptr [esi + 0x102], 1
        je     loc_1_1960
        inc    dword ptr [esi + 0x122]
        jmp    loc_1_1966
loc_1_1931:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx], eax
        push   2
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0x10
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  090h
loc_1_1960:
        inc    dword ptr [esi + 0x12a]
loc_1_1966:
        mov    edi, dword ptr [esi + 0x5b]
        lea    ecx, [esi + edi*4]
        mov    edx, dword ptr [ecx + 0xc1]
        add    edx, 0xff
        shr    edx, 8
        lea    eax, [edi + edx]
        cmp    eax, edi
        jbe    loc_1_1998
        add    ecx, 0x61
loc_1_1985:
        mov    dword ptr [ecx], 0
        add    ecx, 4
        inc    edi
        mov    eax, dword ptr [esi + 0x5b]
        add    eax, edx
        cmp    eax, edi
        ja     loc_1_1985
loc_1_1998:
        mov    eax, dword ptr [esi + 0x57]
        add    eax, edx
        mov    dword ptr [esi + 0x57], eax
        cmp    eax, 0xc
        jne    loc_1_19AC
        mov    dword ptr [esi + 0x57], 0
loc_1_19AC:
        mov    eax, dword ptr [esi + 0x57]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_19C2
        cmp    dword ptr [esi + 0x53], eax
        je     loc_1_19C2
        mov    dword ptr [esi + 0x57], 0
loc_1_19C2:
        mov    eax, dword ptr [esi + 0x5b]
        mov    dword ptr [esi + eax*4 + 0x91], 0
        mov    byte ptr [esi + 0x1b5], 0
        mov    eax, dword ptr [esi + 0x57]
        cmp    dword ptr [esi + eax*4 + 0x61], 2
        jne    loc_1_1A00
        mov    dword ptr [esi + 0x5b], eax
        mov    byte ptr [esi + 0x5f], 1
        push   esi
        call   sub_1_0780
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A00:
        mov    dword ptr [esi + 0x5b], 0xffffffff
loc_1_1A07:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        add    edi, 0x24
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_1A50
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A50:
        cmp    byte ptr [esi + 0x142], 0
        je     loc_1_1A67
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A67:
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [esi + 0x30]
        mov    dword ptr [edi + 4], ebx
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        call   sub_1_2550
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        test   byte ptr [ebx + 0x71], 2
        je     loc_1_1AA0
        mov    byte ptr [edi + 8], 0
        jmp    loc_1_1AB3
loc_1_1AA0:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_1_2350
        mov    byte ptr [edi + 8], al
loc_1_1AB3:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_1_1AC4
        mov    eax, dword ptr [esi + 0x4f]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_1_1ACA
loc_1_1AC4:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x4b], ecx
loc_1_1ACA:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x4f], ecx
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_1B07
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_1AF7
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_1_1AF7
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_1B07
loc_1_1AF7:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        jmp    loc_1_1B26
loc_1_1B07:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_1_1B26
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_1B26
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_1_1B26
        push   esi
        call   sub_1_1B40
loc_1_1B26:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_1_1DFF
loc_1_1B56:
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_1DFF
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_2550
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 0x5b]
        cmp    eax, -1
        jne    loc_1_1BE0
        mov    ecx, dword ptr [esi + 0x57]
        cmp    dword ptr [esi + ecx*4 + 0x61], 0
        mov    eax, dword ptr [esi + 0x53]
        jne    loc_1_1BAB
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_1C12
        mov    dword ptr [esi + 0x53], 0
        jmp    loc_1_1C12
loc_1_1BAB:
        cmp    ecx, eax
        jbe    loc_1_1BC0
        add    eax, dword ptr [ebp - 4]
        cmp    eax, ecx
        jbe    loc_1_1C12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1BC0:
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_1C12
        mov    dword ptr [esi + 0x53], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x57], eax
        jae    loc_1_1C12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BE0:
        mov    ecx, dword ptr [esi + 0x53]
        cmp    eax, ecx
        jbe    loc_1_1BF7
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, eax
        jbe    loc_1_1C12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BF7:
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, 0xc
        jbe    loc_1_1C12
        mov    dword ptr [esi + 0x53], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x5b], eax
        jb     loc_1_1DFF
loc_1_1C12:
        mov    edi, dword ptr [esi + 0x4b]
        mov    ebx, dword ptr [esi + 0x53]
        mov    eax, dword ptr [edi + 0x24]
        mov    ecx, ebx
        mov    dword ptr [esi + 0x4b], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    loc_1_1C43
        lea    eax, [esi + ebx*4 + 0x61]
loc_1_1C30:
        mov    dword ptr [eax], 1
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     loc_1_1C30
loc_1_1C43:
        mov    dword ptr [esi + ebx*4 + 0x91], edi
        lea    eax, [esi + ebx*4 + 0xc1]
        push   eax
        push   edi
        push   esi
        call   0
        or     al, al
        jne    loc_1_1CB0
        push   0xc0000001
        mov    eax, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        mov    ecx, ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    loc_1_1C93
        lea    eax, [esi + ebx*4 + 0x61]
loc_1_1C80:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     loc_1_1C80
loc_1_1C93:
        mov    eax, dword ptr [edi + 0x28]
        dec    dword ptr [eax + 0x1c]
        mov    dword ptr [esi + ebx*4 + 0x91], 0
        jmp    loc_1_1DA7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CB0:
        mov    ecx, dword ptr [esi + ebx*4 + 0xc1]
        cmp    ecx, 0x3c
        jae    loc_1_1CDF
        mov    eax, 0x3c
        sub    eax, ecx
        push   eax
        push   0x2728                        ; -> loc_1_2728
        mov    eax, dword ptr [esi + 0xf1]
        mov    edx, ebx
        shl    edx, 8
        add    eax, edx
        add    eax, ecx
        push   eax
        push   esi
        call   sub_1_0390
loc_1_1CDF:
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_1DBA
        mov    ecx, ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    loc_1_1D13
        lea    eax, [esi + ebx*4 + 0x61]
        add    eax, 0
loc_1_1D00:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     loc_1_1D00
loc_1_1D13:
        mov    eax, dword ptr [esi + 0x53]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x53], eax
        cmp    eax, 0xc
        jne    loc_1_1D28
        mov    dword ptr [esi + 0x53], 0
loc_1_1D28:
        cmp    dword ptr [esi + 0x5b], -1
        jne    loc_1_1D63
        mov    eax, dword ptr [esi + 0x57]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_1D44
        cmp    dword ptr [esi + 0x53], eax
        je     loc_1_1D44
        mov    dword ptr [esi + 0x57], 0
loc_1_1D44:
        mov    eax, dword ptr [esi + 0x57]
        mov    dword ptr [esi + 0x5b], eax
        cmp    byte ptr [esi + 0x11c], 0
        je     loc_1_1D59
        mov    byte ptr [esi + 0x60], 1
        jmp    loc_1_1D63
loc_1_1D59:
        mov    byte ptr [esi + 0x5f], 1
        push   esi
        call   sub_1_0780
loc_1_1D63:
        cmp    byte ptr [edi + 0x2c], 0
        jne    loc_1_1D80
        mov    ebx, dword ptr [edi + 0x28]
        push   0
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_246A
        dec    dword ptr [ebx + 0x1c]
        jmp    loc_1_1DA7
        db  08Dh, 064h, 024h, 000h
loc_1_1D80:
        cmp    dword ptr [esi + 0x165], 0
        jne    loc_1_1D91
        mov    dword ptr [esi + 0x165], edi
        jmp    loc_1_1D9A
loc_1_1D91:
        mov    eax, dword ptr [esi + 0x169]
        mov    dword ptr [eax + 0x24], edi
loc_1_1D9A:
        mov    dword ptr [esi + 0x169], edi
        mov    dword ptr [edi + 0x24], 0
loc_1_1DA7:
        cmp    dword ptr [esi + 0x4b], 0
        jne    loc_1_1B56
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DBA:
        mov    eax, dword ptr [edi + 0x28]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    loc_1_1DE3
        lea    eax, [esi + ebx*4 + 0x61]
        nop    
loc_1_1DD0:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     loc_1_1DD0
loc_1_1DE3:
        push   0
        mov    eax, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        push   3
        push   esi
        call   sub_2_2160
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x1c]
loc_1_1DFF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1E10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1b5], 0
        je     loc_1_1E70
        cmp    dword ptr [esi + 0x5b], -1
        je     loc_1_1E7D
        mov    byte ptr [esi + 0x1b5], 0
        push   3
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0x10
        push   esi
        call   sub_2_0680
        cmp    dword ptr [esi + 0x14b], 4
        jbe    loc_1_1E61
        push   esi
        call   sub_2_06F0
        mov    dword ptr [esi + 0x14b], 0
loc_1_1E61:
        push   esi
        call   sub_2_08F0
        inc    dword ptr [esi + 0x14b]
        jmp    loc_1_1E7D
        db  090h
loc_1_1E70:
        cmp    dword ptr [esi + 0x5b], -1
        je     loc_1_1E7D
        mov    byte ptr [esi + 0x1b5], 1
loc_1_1E7D:
        push   0x7d0
        add    esi, 0x1a9
        push   esi
        call   sub_1_2502
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1EA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_1F56
        cmp    dword ptr [esi + 0x179], 0
        jne    loc_1_1F56
loc_1_1EC6:
        mov    edi, dword ptr [esi + 0x171]
        or     edi, edi
        je     loc_1_1F31
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x171], eax
        cmp    dword ptr [esi + 0x175], edi
        jne    loc_1_1EEA
        mov    dword ptr [esi + 0x175], 0
loc_1_1EEA:
        mov    dword ptr [esi + 0x179], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_1F05
        cmp    eax, 0xa
        je     loc_1_1F05
        push   esi
        call   sub_2_2370
        jmp    loc_1_1F0B
loc_1_1F05:
        push   esi
        call   sub_2_2400
loc_1_1F0B:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_1F25
        cmp    ecx, 4
        je     loc_1_1F25
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_247C
loc_1_1F25:
        dec    dword ptr [ebx + 0x1c]
        cmp    byte ptr [esi + 0x142], 0
        je     loc_1_1EC6
loc_1_1F31:
        mov    dword ptr [esi + 0x179], 0
        cmp    byte ptr [esi + 0x142], 0
        je     loc_1_1F56
        dec    dword ptr [esi + 0x30]
        push   1
        push   esi
        call   sub_2_2160
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_1F56:
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_1FC1
        sub    ebx, ebx
        nop    
loc_1_1F60:
        cmp    dword ptr [edi + 0x1c], 0
        jbe    loc_1_1F70
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_1FAB
        db  08Dh, 049h, 000h
loc_1_1F70:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x161], eax
        jne    loc_1_1F81
        push   esi
        call   sub_2_1230
loc_1_1F81:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_2464
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0xc]
        lea    ecx, [edi + 0xc]
        je     loc_1_1F9A
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_1F9D
loc_1_1F9A:
        mov    dword ptr [esi + 0x2c], eax
loc_1_1F9D:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   0x75
        push   eax
        call   sub_1_252C
loc_1_1FAB:
        or     edi, edi
        jne    loc_1_1F60
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_1_1FC1
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_1FC1
        push   esi
        call   sub_2_0680
loc_1_1FC1:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_1FF7
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_1_1FE2
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_1_1FE2
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_1FF7
loc_1_1FE2:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_1FF7:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_1_2016
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_1_2016
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_1_2016
        push   esi
        call   sub_1_1B40
loc_1_2016:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
loc_1_2020:
        mov    al, byte ptr [0x26b8]         ; -> obj1:0x26B8
        cmp    al, 0
        jne    loc_1_2045
        inc    byte ptr [0x26b8]             ; -> obj1:0x26B8
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_2047
        push   0
        push   0
        call   sub_2_0950
        cmp    eax, 0
        jne    loc_1_2047
loc_1_2045:
        clc    
        ret    
loc_1_2047:
        stc    
        ret    
SOCKET_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_2058
        jmp    dword ptr [eax*4 + 0x205a]    ; -> obj1:0x205A
loc_1_2058:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_2020
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2058
        dd  loc_1_2020
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_20D0:
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
        call   sub_1_2100
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_2100:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_21A0
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_2140
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_2135
        cmp    al, 0xff
        jne    loc_1_2135
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_2135
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_21A7
loc_1_2135:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_21A7
        db  08Bh, 0FFh
loc_1_2140:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_2190
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_2166
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_216D
loc_1_2166:
        mov    dword ptr [ebp - 0x10], 1
loc_1_216D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_2180
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_21AA
        db  08Dh, 064h, 024h, 000h
loc_1_2180:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_21AA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2190:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_21AA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_21A0:
        mov    dword ptr [ebp - 4], 0x20
loc_1_21A7:
        mov    esi, dword ptr [ebp + 8]
loc_1_21AA:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_22A1
loc_1_21B5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_220D
        test   al, 2
        je     loc_1_21F9
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2BF0
        or     al, al
        je     loc_1_21F9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_220D
loc_1_21F9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_2204
        test   bl, 4
        jne    loc_1_220D
loc_1_2204:
        test   bl, 0x20
        je     loc_1_2296
loc_1_220D:
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
        call   sub_1_2482
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_2296
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_224F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_224F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_2260
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_2266
        db  08Bh, 0FFh
loc_1_2260:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_2266:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_2275
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2488
loc_1_2275:
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
        call   sub_1_252C
        mov    edi, ebx
        jmp    loc_1_2299
loc_1_2296:
        mov    edi, dword ptr [edi + 0x10]
loc_1_2299:
        or     edi, edi
        jne    loc_1_21B5
loc_1_22A1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_22B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22C0
        pop    ebp
        ret    4
sub_1_22C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_233E
loc_1_22D0:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_2337
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2488
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_2337
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_22FC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_22FC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_2310
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_2316
        db  005h, 000h, 000h, 000h, 000h
loc_1_2310:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_2316:
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
        call   sub_1_252C
        mov    edi, ebx
        jmp    loc_1_233A
loc_1_2337:
        mov    edi, dword ptr [edi + 0x10]
loc_1_233A:
        or     edi, edi
        jne    loc_1_22D0
loc_1_233E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2350:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_2365
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2365:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_23A1
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_2392
        cmp    bl, 0xff
        jne    loc_1_2392
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_2392
        test   al, 8
        je     loc_1_238B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_238B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_2392:
        test   al, 4
        jne    loc_1_239A
        test   al, 2
        je     loc_1_23B9
loc_1_239A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_23A1:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_23B9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_23B9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_23B9:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_23C0:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
sub_1_23D4:
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
sub_1_23FD:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_2411:
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
sub_1_243A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_2440:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_2446:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_244C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_2452:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_2458:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_2464:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_246A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_2476:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_247C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_2482:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_2488:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_248E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_2494:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_24AC:
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
sub_1_2502:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_2508:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_250E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_2514:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_251A:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_2520:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_2526:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_252C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_2550:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_2574:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_2598:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_25A4:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_25AA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_25B0:
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
sub_1_25F8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_260C:
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
sub_1_262C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_264C
        or     ecx, ecx
        je     loc_1_2662
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_2660
loc_1_264C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_2660
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_2660:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_2662:
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
        db  'SOCKET  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SOCKET_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_2728:
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
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_27B4:
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
loc_1_282C:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        add    byte ptr [eax], al
loc_1_2890:
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_28A0:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_28D0:
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
        inc    edx
        jne    0x296e
        push   esp
        jns    0x296e
        add    byte ptr gs:[edx + 0x75], al
        jae    0x2958
        jns    0x2976
        add    byte ptr gs:[ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        push   esp
        jns    0x2984
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        push   esp
        jns    0x2994
        add    byte ptr gs:[eax], al
        add    byte ptr [esi + 0x6f], cl
        jbe    0x2991
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], ah
        inc    ecx
        popal  
        jo     0x29ad
        jb     0x296a
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2cb0  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        sub    al, al
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xf
        out    dx, al
        push   esi
        call   sub_1_0760
        push   0x7d0
        call   sub_1_2520
        mov    dx, word ptr [esi + 0x103]
        in     al, dx
        cmp    al, 0x21
        je     loc_2_0040
        cmp    al, 0x23
        je     loc_2_0040
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0040:
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0050:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x1f
        mov    cx, word ptr [esi + 0x103]
        in     al, dx
        lea    edx, [ecx + 0x1f]
        mov    al, 0xff
        out    dx, al
        mov    dx, cx
        mov    al, 0x21
        out    dx, al
        push   0x7d0
        call   sub_1_2520
        mov    dx, word ptr [esi + 0x103]
        in     al, dx
        cmp    al, 0x21
        je     loc_2_00A0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_00A0:
        mov    di, word ptr [esi + 0x103]
        mov    al, 0x48
        lea    edx, [edi + 0xe]
        out    dx, al
        lea    edx, [edi + 0xf]
        sub    al, al
        out    dx, al
        lea    edx, [edi + 7]
        mov    al, 0xff
        out    dx, al
        lea    edx, [edi + 8]
        sub    al, al
        out    dx, al
        lea    edx, [edi + 9]
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, 0x20
        out    dx, al
        lea    edx, [edi + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, di
        mov    al, 0xa
        out    dx, al
        sub    cl, cl
        lea    edx, [edi + 0x10]
loc_2_00DA:
        in     al, dx
        xor    ebx, ebx
        mov    bl, cl
        mov    byte ptr [ebp + ebx - 0x24], al
        inc    cl
        cmp    cl, 0x20
        jb     loc_2_00DA
        lea    edx, [edi + 0x1f]
        in     al, dx
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x1f
        mov    al, 0xff
        out    dx, al
        mov    cl, 6
        mov    edx, 0x42
        mov    ebx, 0x57
loc_2_0107:
        mov    edi, ecx
        and    edi, 0xff
        mov    al, byte ptr [ebp + edi - 0x24]
        cmp    al, dl
        jne    loc_2_011D
        cmp    byte ptr [ebp + edi - 0x23], dl
        je     loc_2_0139
loc_2_011D:
        cmp    al, bl
        jne    loc_2_0127
        cmp    byte ptr [ebp + edi - 0x23], bl
        je     loc_2_0139
loc_2_0127:
        inc    cl
        cmp    cl, 0x1f
        jb     loc_2_0107
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0139:
        xor    eax, eax
        mov    al, cl
        cmp    byte ptr [ebp + eax - 0x24], 0x42
        jne    loc_2_0156
        mov    byte ptr [esi + 0x120], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0156:
        mov    byte ptr [esi + 0x120], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0xaa
        mov    edi, 0x400
        mov    byte ptr [ebp - 3], 0x55
        mov    byte ptr [ebp - 2], 0xff
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ebp + 8]
loc_2_0191:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_2_01D3
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        je     loc_2_01D3
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    loc_2_01D3
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    loc_2_01D3
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    loc_2_01D3
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        je     loc_2_01E6
loc_2_01D3:
        add    edi, 0x400
        cmp    edi, 0x10000
        jb     loc_2_0191
        mov    ebx, dword ptr [ebp - 0x10]
        jmp    loc_2_0240
loc_2_01E6:
        mov    ebx, edi
        test   edi, 0xffff0000
        jne    loc_2_0240
loc_2_01F0:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_2_0240
        push   4
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        je     loc_2_0240
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    loc_2_0240
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    loc_2_0240
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    loc_2_0240
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    loc_2_0240
        add    ebx, 0x400
        test   ebx, 0xffff0000
        je     loc_2_01F0
loc_2_0240:
        cmp    edi, 0x10000
        jae    loc_2_02F1
        cmp    ebx, edi
        je     loc_2_02F1
        test   ebx, 0xffff0000
        je     loc_2_0262
        sub    ebx, 0x100
loc_2_0262:
        mov    dword ptr [ebp - 0xc], edi
        cmp    ebx, edi
        jbe    loc_2_02B7
loc_2_0269:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_2_02D0
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        je     loc_2_02DB
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    loc_2_02E6
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    loc_2_02E6
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    loc_2_02E6
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    loc_2_02E6
        add    dword ptr [ebp - 0xc], 4
        cmp    ebx, dword ptr [ebp - 0xc]
        ja     loc_2_0269
loc_2_02B7:
        mov    dword ptr [esi + 0x114], edi
        sub    ebx, edi
        mov    dword ptr [esi + 0x118], ebx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02D0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02DB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02E6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02F1:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_0300:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0760
        mov    cx, word ptr [esi + 0x103]
        mov    al, 0x50
        lea    edx, [ecx + 0xe]
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, 0xa0
        out    dx, al
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0xc]
        mov    al, 0x20
        out    dx, al
        lea    edx, [ecx + 1]
        mov    al, 4
        out    dx, al
        lea    edx, [ecx + 2]
        mov    al, 0xff
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 4
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 0x3c
        out    dx, al
        lea    edx, [ecx + 6]
        sub    al, al
        out    dx, al
        push   0x7d0
        call   sub_1_2520
        mov    di, word ptr [esi + 0x103]
        mov    al, 0xff
        lea    ecx, [edi + 7]
        mov    dx, cx
        out    dx, al
        mov    dx, di
        mov    al, 0x41
        out    dx, al
        mov    dx, cx
        mov    al, 4
        out    dx, al
        mov    dx, di
        mov    al, 1
        out    dx, al
        push   0x7d0
        call   sub_1_2520
        mov    dx, word ptr [esi + 0x103]
        in     al, dx
        test   al, 1
        jne    loc_2_03A0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_03A0:
        mov    cx, word ptr [esi + 0x103]
        mov    al, 0x37
        lea    edx, [ecx + 0xa]
        out    dx, al
        cmp    byte ptr [esi + 0x45], 3
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        jne    loc_2_03E0
        mov    byte ptr [esi + 0x120], 0
loc_2_03C0:
        mov    cx, word ptr [esi + 0x103]
        sub    al, al
        lea    edx, [ecx + 0xf]
        out    dx, al
        cmp    byte ptr [esi + 0x120], al
        mov    dx, cx
        out    dx, al
        lea    edx, [ecx + 0xe]
        je     loc_2_03FB
        mov    al, 0x48
        jmp    loc_2_03FD
loc_2_03E0:
        push   esi
        call   sub_2_0050
        or     al, al
        jne    loc_2_03C0
        push   esi
        call   sub_1_0760
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_03FB:
        mov    al, 0x49
loc_2_03FD:
        out    dx, al
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0xc]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0xff
        out    dx, al
        mov    dx, cx
        mov    al, 0x21
        out    dx, al
        lea    edx, [ecx + 0xa]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0xb]
        out    dx, al
        mov    cl, 0xff
        mov    ebx, 0x80
loc_2_0425:
        dec    cl
        je     loc_2_0438
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   bl, al
        je     loc_2_0425
loc_2_0438:
        mov    cx, word ptr [esi + 0x103]
        mov    al, 2
        lea    edx, [ecx + 0xd]
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        out    dx, al
        push   esi
        call   sub_2_0170
        or     al, al
        push   esi
        jne    loc_2_0466
        call   sub_1_0760
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0466:
        call   sub_1_0760
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0480:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0xc
        mov    cx, word ptr [esi + 0x103]
        lea    edx, [ecx + 0xa]
        out    dx, al
        lea    edx, [ecx + 0xb]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        out    dx, al
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        sub    ecx, ecx
        lea    ebx, [ebx]
loc_2_04B0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     al, dx
        mov    byte ptr [esi + ecx + 0x10e], al
        inc    ecx
        cmp    ecx, 6
        jb     loc_2_04B0
        cmp    byte ptr [esi + 0x108], 0
        jne    loc_2_0546
        cmp    byte ptr [esi + 0x109], 0
        jne    loc_2_0546
        cmp    byte ptr [esi + 0x10a], 0
        jne    loc_2_0546
        cmp    byte ptr [esi + 0x10b], 0
        jne    loc_2_0546
        cmp    byte ptr [esi + 0x10c], 0
        jne    loc_2_0546
        cmp    byte ptr [esi + 0x10d], 0
        jne    loc_2_0546
        mov    al, byte ptr [esi + 0x10e]
        mov    byte ptr [esi + 0x108], al
        mov    al, byte ptr [esi + 0x10f]
        mov    byte ptr [esi + 0x109], al
        mov    al, byte ptr [esi + 0x110]
        mov    byte ptr [esi + 0x10a], al
        mov    al, byte ptr [esi + 0x111]
        mov    byte ptr [esi + 0x10b], al
        mov    al, byte ptr [esi + 0x112]
        mov    byte ptr [esi + 0x10c], al
        mov    al, byte ptr [esi + 0x113]
        mov    byte ptr [esi + 0x10d], al
loc_2_0546:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0550:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 0x21
        mov    dx, word ptr [ecx + 0x103]
        out    dx, al
        in     al, dx
        and    al, 0x21
        cmp    al, 0x21
        je     loc_2_0578
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0578:
        cmp    byte ptr [ecx + 0x120], 0
        mov    edx, dword ptr [ecx + 0x103]
        je     loc_2_0590
        add    dx, 0xe
        mov    al, 0x48
        jmp    loc_2_0596
        db  090h
loc_2_0590:
        add    dx, 0xe
        mov    al, 0x49
loc_2_0596:
        mov    si, word ptr [ecx + 0x103]
        out    dx, al
        lea    edx, [esi + 0xb]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 0xa]
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    al, byte ptr [ecx + 0x15f]
        out    dx, al
        lea    edx, [esi + 0xd]
        mov    al, 2
        out    dx, al
        mov    bl, byte ptr [ecx + 0xfe]
        lea    edx, [esi + 3]
        mov    al, bl
        out    dx, al
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, byte ptr [ecx + 0xff]
        out    dx, al
        inc    bl
        mov    byte ptr [ecx + 0x101], bl
        mov    al, byte ptr [ecx + 0xfe]
        inc    al
        mov    byte ptr [ecx + 0x100], al
        mov    byte ptr [ecx + 0x11c], 0
        lea    eax, [esi + 7]
        mov    dx, ax
        mov    al, 0xff
        out    dx, al
        lea    edx, [esi + 0xf]
        mov    al, byte ptr [ecx + 0x160]
        out    dx, al
        mov    dx, si
        mov    al, 0x61
        out    dx, al
        sub    edi, edi
loc_2_060A:
        lea    edx, [esi + edi]
        inc    dx
        mov    al, byte ptr [ecx + edi + 0x108]
        out    dx, al
        inc    edi
        cmp    edi, 6
        jb     loc_2_060A
        mov    eax, dword ptr [ecx + 0x14f]
        sub    edi, edi
        mov    ebx, dword ptr [eax + 0xc]
        and    ebx, 4
        add    eax, 0
loc_2_0630:
        or     ebx, ebx
        mov    eax, 0xff
        jne    loc_2_0642
        xor    eax, eax
        mov    al, byte ptr [ecx + edi + 0x153]
loc_2_0642:
        lea    edx, [esi + edi]
        add    dx, 8
        out    dx, al
        inc    edi
        cmp    edi, 8
        jb     loc_2_0630
        lea    eax, [esi + 7]
        mov    dx, ax
        mov    al, byte ptr [ecx + 0x101]
        out    dx, al
        mov    dx, si
        mov    al, 0x21
        out    dx, al
        mov    al, 0x22
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0680:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        lea    eax, [esi + 8]
        push   0x760                         ; -> sub_1_0760
        push   eax
        call   sub_1_251A
        mov    cx, word ptr [esi + 0x103]
        sub    al, al
        lea    edx, [ecx + 0xb]
        out    dx, al
        lea    edx, [ecx + 0xa]
        out    dx, al
        sub    edi, edi
        mov    ebx, 0x80
loc_2_06B3:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   bl, al
        jne    loc_2_06D2
        push   0x3e8
        call   sub_1_2520
        inc    edi
        cmp    edi, 4
        jb     loc_2_06B3
loc_2_06D2:
        mov    cx, word ptr [esi + 0x103]
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
        db  08Bh, 0FFh
sub_2_06F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0680
        push   0x7d0
        call   sub_1_2520
        push   esi
        call   sub_2_0550
        or     al, al
        jne    loc_2_0730
        push   1
        push   1
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0x14
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_0730:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xd
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0750:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_2_079D
loc_2_0762:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_2_0770:
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
        je     loc_2_0794
        xor    eax, 0x4c11db6
        or     eax, edi
loc_2_0794:
        dec    esi
        jne    loc_2_0770
        inc    edx
        cmp    edx, dword ptr [ebp + 0xc]
        jb     loc_2_0762
loc_2_079D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_07B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0750
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
sub_2_07E0:
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
loc_2_0800:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x153], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_2_0800
        push   0x26c0                        ; -> obj1:0x26C0
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2B90
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_2_0867
        sub    edi, edi
loc_2_0835:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x26c0]         ; -> obj1:0x26C0
        push   eax
        call   sub_2_07B0
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x153], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_2_0835
loc_2_0867:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0870:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    si, word ptr [eax + 0x103]
        mov    dx, si
        mov    al, 0x62
        out    dx, al
loc_2_0886:
        lea    edx, [esi + ecx]
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     loc_2_0886
        mov    dx, si
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_08B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 0x62
        mov    di, word ptr [ecx + 0x103]
        mov    dx, di
        out    dx, al
        sub    esi, esi
loc_2_08C7:
        lea    edx, [edi + esi]
        add    dx, 8
        mov    al, byte ptr [ecx + esi + 0x153]
        out    dx, al
        inc    esi
        cmp    esi, 8
        jb     loc_2_08C7
        mov    dx, di
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 8
        mov    edx, dword ptr [edi + 0x103]
        add    dx, 7
        out    dx, al
        push   edi
        call   sub_1_0760
        push   0x7d0
        call   sub_1_2520
        mov    si, word ptr [edi + 0x103]
        mov    al, 2
        lea    ecx, [esi + 0xd]
        mov    dx, cx
        out    dx, al
        mov    dx, si
        mov    al, 0x22
        out    dx, al
        mov    dx, cx
        sub    al, al
        out    dx, al
        cmp    dword ptr [edi + 0x5b], -1
        je     loc_2_0941
        mov    byte ptr [edi + 0x5f], 1
        push   edi
        call   sub_1_0780
loc_2_0941:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0950:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        mov    word ptr [ebp - 0x10], 7
        mov    word ptr [ebp - 0xe], 7
        mov    dword ptr [ebp - 0xc], 0x2890 ; -> loc_1_2890
        mov    word ptr [ebp - 0x1c], 0
        mov    word ptr [ebp - 0x1a], 3
        mov    word ptr [ebp - 0x18], 0x67b0
        mov    word ptr [ebp - 0x16], 0x611f
        mov    word ptr [ebp - 0x14], 0x7154
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_243A
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [0x27ae], eax       ; -> obj1:0x27AE
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x276c], eax       ; -> obj1:0x276C
        mov    byte ptr [0x27b2], 0          ; -> obj1:0x27B2
        mov    dword ptr [0x27a6], 0         ; -> obj1:0x27A6
        mov    byte ptr [0x2770], 3          ; -> obj1:0x2770
        mov    byte ptr [0x2771], 0          ; -> obj1:0x2771
        mov    dword ptr [0x2772], 0         ; -> obj1:0x2772
        mov    dword ptr [0x2776], 0x1060    ; -> obj1:0x2776
        mov    dword ptr [0x277a], 0x1270    ; -> obj1:0x277A
        mov    dword ptr [0x277e], 0x1a20    ; -> obj1:0x277E
        mov    dword ptr [0x2782], 0x1700    ; -> obj1:0x2782
        mov    dword ptr [0x2786], 0x1e30    ; -> obj1:0x2786
        mov    dword ptr [0x278a], 0x1440    ; -> obj1:0x278A
        mov    dword ptr [0x278e], 0x1c90    ; -> obj1:0x278E
        mov    dword ptr [0x2792], 0x1d70    ; -> obj1:0x2792
        mov    dword ptr [0x2796], 0xa90     ; -> obj1:0x2796
        mov    dword ptr [0x279a], 0x1da0    ; -> obj1:0x279A
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0x279e], eax       ; -> obj1:0x279E
        mov    dword ptr [0x27a2], ecx       ; -> obj1:0x27A2
        push   0x36
        push   0x2770                        ; -> obj1:0x2770
        push   0x2768                        ; -> obj1:0x2768
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   0x2768                        ; -> obj1:0x2768
        push   eax
        call   sub_1_2446
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0A80
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2440
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0A80:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x58
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x48], 7
        mov    word ptr [ebp - 0x46], 7
        mov    dword ptr [ebp - 0x44], 0x28a0 ; -> loc_1_28A0
        mov    word ptr [ebp - 0x40], 0xa
        mov    word ptr [ebp - 0x3e], 0xa
        mov    dword ptr [ebp - 0x3c], 0x28b4 ; -> obj1:0x28B4
        mov    word ptr [ebp - 0x30], 0xd
        mov    word ptr [ebp - 0x2e], 0xd
        mov    dword ptr [ebp - 0x2c], 0x28d0 ; -> loc_1_28D0
        mov    esi, 0x300
        mov    word ptr [ebp - 0x38], 0xe
        mov    word ptr [ebp - 0x36], 0xe
        mov    dword ptr [ebp - 0x34], 0x2918 ; -> obj1:0x2918
        mov    byte ptr [ebp - 1], 0
        mov    bl, 3
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x20], 0
        mov    dword ptr [ebp - 0x1c], 0x10
        mov    eax, dword ptr [0x2764]       ; -> obj1:0x2764
        push   eax
        lea    eax, [ebp - 0xc]
        push   0
        push   0x2c3
        push   eax
        call   sub_1_2526
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_2_0B2B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0B2B:
        mov    edi, dword ptr [ebp - 0xc]
        sub    eax, eax
        mov    ecx, 0xb0
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_25A4
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0B70
        push   0
        push   0x2c3
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_252C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_2_0B70:
        push   1
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0B93
        mov    eax, dword ptr [ebp - 0x10]
        mov    esi, dword ptr [eax + 4]
loc_2_0B93:
        cmp    esi, 0x120
        jb     loc_2_0CC4
        cmp    esi, 0xc3d0
        ja     loc_2_0CC4
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0BCE
        mov    eax, dword ptr [ebp - 0x10]
        mov    bl, byte ptr [eax + 4]
loc_2_0BCE:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0BF6
        mov    eax, dword ptr [ebp - 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        mov    dword ptr [ebp - 0x20], ecx
loc_2_0BF6:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0C1C
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x1c], ecx
loc_2_0C1C:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_24AC
        cmp    dword ptr [ebp - 0x28], 6
        jne    loc_2_0C5B
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0C5B
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, edx
        add    ecx, 0x108
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_2_0C5B:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_25B0
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x38], 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x3c], esi
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x40], bl
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [eax + 0x47], ecx
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x41], ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_0CE0
        or     eax, eax
        je     loc_2_0CCD
        push   0
        push   0x2c3
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_252C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0CC4:
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 0x18], esi
        jmp    loc_2_0C5B
loc_2_0CCD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0CE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x38], 0xc
        jbe    loc_2_0D05
        mov    dword ptr [ebp - 4], 0xc000009a
loc_2_0CF9:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D05:
        lea    edi, [ebp - 0x2c]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x22], 1
        mov    dword ptr [ebp - 0x16], 1
        mov    eax, dword ptr [esi + 0x3c]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 0x12], eax
        mov    dword ptr [ebp - 0xe], 0x20
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [0x2768]       ; -> obj1:0x2768
        push   eax
        push   edi
        call   sub_1_2452
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_2_0CF9
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0D72
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        push   1
        push   0xaa00000a
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0x10
        jmp    loc_2_0FC3
loc_2_0D72:
        mov    dword ptr [esi + 0x189], 0x1ea0 ; -> loc_1_1EA0
        mov    dword ptr [esi + 0x17d], 0
        mov    eax, dword ptr [esi + 0x189]
        mov    dword ptr [esi + 0x181], eax
        mov    dword ptr [esi + 0x185], esi
        mov    eax, dword ptr [esi + 0x3c]
        mov    dword ptr [esi + 0x103], eax
        push   esi
        call   0
        or     al, al
        jne    loc_2_0DC9
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_2_0FC3
loc_2_0DC9:
        push   esi
        call   sub_2_0300
        or     al, al
        jne    loc_2_0DF1
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_2_0FC3
loc_2_0DF1:
        mov    eax, dword ptr [esi + 0x114]
        mov    dword ptr [esi + 0xf1], eax
        mov    byte ptr [esi + 0xfd], ah
        mov    eax, dword ptr [esi + 0x38]
        shl    eax, 8
        add    eax, dword ptr [esi + 0xf1]
        mov    dword ptr [esi + 0xf5], eax
        mov    eax, dword ptr [esi + 0x38]
        add    al, byte ptr [esi + 0xfd]
        mov    byte ptr [esi + 0xfe], al
        mov    eax, dword ptr [esi + 0xf1]
        add    eax, dword ptr [esi + 0x118]
        mov    dword ptr [esi + 0xf9], eax
        mov    eax, dword ptr [esi + 0x118]
        shr    eax, 8
        add    al, byte ptr [esi + 0xfd]
        mov    byte ptr [esi + 0xff], al
        mov    byte ptr [esi + 0x15f], 0
        mov    byte ptr [esi + 0x121], 0
        sub    ecx, ecx
        mov    dword ptr [esi + 0x5b], 0xffffffff
        cmp    dword ptr [esi + 0x38], ecx
        jbe    loc_2_0E7F
        lea    eax, [esi + 0x61]
        lea    ebx, [ebx]
loc_2_0E70:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 0x38], ecx
        ja     loc_2_0E70
loc_2_0E7F:
        mov    byte ptr [esi + 0x1b5], 0
        mov    byte ptr [esi + 0x11f], 0
        push   esi
        call   sub_2_0480
        lea    ebx, [esi + 0x14f]
        push   ebx
        push   esi
        lea    eax, [esi + 0x108]
        push   eax
        push   0x2360                        ; -> loc_2_2360
        push   0x22e0                        ; -> loc_2_22E0
        push   0x2260                        ; -> loc_2_2260
        mov    eax, dword ptr [esi + 0x41]
        push   eax
        call   sub_2_2430
        or     al, al
        jne    loc_2_0EE0
        push   0
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_2_0FC3
        db  08Dh, 064h, 024h, 000h
loc_2_0EE0:
        mov    byte ptr [esi + 0x160], 0x1b
        mov    dword ptr [esi + 0x20], 0x2768 ; -> obj1:0x2768
        mov    eax, dword ptr [0x27a6]       ; -> obj1:0x27A6
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [0x27a6], esi       ; -> obj1:0x27A6
        push   esi
        call   sub_2_0550
        or     al, al
        jne    loc_2_0F21
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_2_0F7D
loc_2_0F21:
        cmp    dword ptr [esi + 0x47], 0
        mov    edx, dword ptr [esi + 0x103]
        je     loc_2_0F35
        add    dx, 0x18
        mov    al, 1
        jmp    loc_2_0F3B
loc_2_0F35:
        add    dx, 0x18
        sub    al, al
loc_2_0F3B:
        out    dx, al
        movsx  eax, byte ptr [esi + 0x40]
        push   1
        push   0
        push   eax
        push   eax
        push   0x9b0                         ; -> loc_1_09B0
        push   esi
        push   0x990                         ; -> loc_1_0990
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_250E
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0FE0
        push   0
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
loc_2_0F7D:
        cmp    dword ptr [0x27a6], esi       ; -> obj1:0x27A6
        jne    loc_2_0F90
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x27a6], eax       ; -> obj1:0x27A6
        jmp    loc_2_0FB1
        db  090h
loc_2_0F90:
        mov    ecx, dword ptr [0x27a6]       ; -> obj1:0x27A6
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_0FA8
        add    eax, 0
loc_2_0FA0:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_0FA0
loc_2_0FA8:
        mov    eax, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], eax
loc_2_0FB1:
        mov    dx, word ptr [esi + 0x103]
        mov    al, 1
        out    dx, al
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_2550
loc_2_0FC3:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_2458
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0FE0:
        mov    dword ptr [esi + 0x18d], 0
        mov    dword ptr [esi + 0x191], 0x9b0 ; -> loc_1_09B0
        mov    dword ptr [esi + 0x195], esi
        mov    dword ptr [esi + 0x199], 0
        lea    eax, [esi + 0x1a9]
        mov    dword ptr [esi + 0x19d], 0x9b0 ; -> loc_1_09B0
        mov    dword ptr [esi + 0x1a1], esi
        mov    dword ptr [esi + 0x1a5], 0x1e10 ; -> loc_1_1E10
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 0x1a5]
        mov    dword ptr [esi + 0x1ad], ecx
        mov    dword ptr [esi + 0x1b1], esi
        push   0x7d0
        push   eax
        call   sub_1_2502
        mov    byte ptr [esi + 0x1b6], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1060:
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
loc_2_1083:
        or     edx, edx
        je     loc_2_1092
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_1083
        mov    dword ptr [ecx], edx
loc_2_1092:
        cmp    dword ptr [ecx], -1
        jne    loc_2_10A4
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_10A4:
        mov    eax, dword ptr [0x2764]       ; -> obj1:0x2764
        push   eax
        push   0
        push   0x75
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2526
        or     eax, eax
        je     loc_2_10E0
        push   0
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_2_10E0:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x30]
        mov    ecx, dword ptr [esi + 0x28]
        or     ecx, ecx
        jne    loc_2_1100
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_2_1100
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xd
        sub    al, al
        out    dx, al
loc_2_1100:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x28], eax
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_11F2
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        mov    edi, dword ptr [ebp + 0x1c]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_2_25A0
        or     al, al
        je     loc_2_11F2
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
        mov    dword ptr [eax + 0x71], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0xee
        mov    dword ptr [esi + 0x161], 0xee
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_11C9
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_11B1
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_11B1
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_11C9
loc_2_11B1:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_11C9:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_11E8
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_11E8
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_11E8
        push   esi
        call   sub_1_1B40
loc_2_11E8:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_11F2:
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x28], eax
        push   0x75
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_252C
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  005h, 000h, 000h, 000h, 000h
sub_2_1230:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0x28]
        cmp    esi, edx
        je     loc_2_1250
loc_2_1240:
        mov    eax, dword ptr [esi + 0x18]
        cmp    edx, eax
        jae    loc_2_1249
        mov    edx, eax
loc_2_1249:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_2_1240
loc_2_1250:
        or     edx, edx
        jne    loc_2_1259
        mov    edx, 0xee
loc_2_1259:
        mov    dword ptr [ecx + 0x161], edx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1270:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_2_1293
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1293:
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [edi + 0x1c]
        mov    ecx, dword ptr [esi + 0x28]
        cmp    edi, ecx
        jne    loc_2_12A8
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0x28], eax
        jmp    loc_2_12C1
loc_2_12A8:
        cmp    dword ptr [ecx + 0xc], edi
        je     loc_2_12B8
loc_2_12B0:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], edi
        jne    loc_2_12B0
loc_2_12B8:
        lea    eax, [edi + 0xc]
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_2_12C1:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        cmp    al, 0xff
        jne    loc_2_132D
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        sub    al, al
        out    dx, al
        in     al, dx
        cmp    al, 0xff
        jne    loc_2_1320
        mov    eax, dword ptr [esi + 0x14f]
        mov    dword ptr [esi + 0x2bf], 1
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [esi + 0x14f]
        push   0
        mov    dword ptr [eax + 0x10], 0
        push   0x75
        push   edi
        call   sub_1_252C
        dec    dword ptr [esi + 0x30]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_1320:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        mov    al, 0xff
        out    dx, al
loc_2_132D:
        push   0
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_2_2670
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_1368
        mov    eax, dword ptr [edi + 0x1c]
        cmp    eax, 2
        je     loc_2_1360
        mov    byte ptr [edi + 0x20], 1
        sub    dword ptr [edi + 0x1c], 2
        mov    ebx, 0x103
        jmp    loc_2_1394
        db  08Dh, 064h, 024h, 000h
loc_2_1360:
        sub    eax, 2
        mov    dword ptr [edi + 0x1c], eax
        jmp    loc_2_1394
loc_2_1368:
        cmp    ebx, 0x103
        jne    loc_2_1380
        mov    byte ptr [edi + 0x20], 1
        sub    dword ptr [edi + 0x1c], 2
        jmp    loc_2_1394
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1380:
        cmp    ebx, 0xc001000e
        jne    loc_2_1391
        mov    byte ptr [edi + 0x20], 1
        mov    ebx, 0x103
loc_2_1391:
        dec    dword ptr [edi + 0x1c]
loc_2_1394:
        cmp    dword ptr [edi + 0x1c], 0
        jne    loc_2_13C9
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x161], eax
        jne    loc_2_13AB
        push   esi
        call   sub_2_1230
loc_2_13AB:
        push   0
        push   0x75
        push   edi
        call   sub_1_252C
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_13D7
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_2_13D7
        push   esi
        call   sub_2_0680
        jmp    loc_2_13D7
loc_2_13C9:
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [esi + 0x2c], edi
        mov    ebx, 0x103
loc_2_13D7:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_1411
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_13F8
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_13F8
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_1411
loc_2_13F8:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1411:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_1430
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_1430
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_1430
        push   esi
        call   sub_1_1B40
loc_2_1430:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_1440:
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
        je     loc_2_1460
        mov    esi, 0xc0010002
        jmp    loc_2_149C
        db  090h
loc_2_1460:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        jne    loc_2_1476
        push   eax
        push   ebx
        push   edi
        call   sub_2_1890
        mov    esi, eax
        jmp    loc_2_149C
loc_2_1476:
        cmp    ecx, 1
        jne    loc_2_1497
        cmp    byte ptr [edi + 0x142], 0
        je     loc_2_148B
        mov    esi, 0xc001000d
        jmp    loc_2_149C
loc_2_148B:
        push   eax
        push   ebx
        push   edi
        call   sub_2_18E0
        mov    esi, eax
        jmp    loc_2_149C
loc_2_1497:
        mov    esi, 0x10001
loc_2_149C:
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_14D7
        cmp    byte ptr [edi + 0x142], 0
        jne    loc_2_14C0
        cmp    dword ptr [edi + 0x171], 0
        jne    loc_2_14C0
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_14D7
loc_2_14C0:
        push   1
        add    edi, 0x17d
        push   edi
        call   sub_1_2502
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_14D7:
        cmp    dword ptr [edi + 0x4b], 0
        je     loc_2_14F6
        mov    eax, dword ptr [edi + 0x53]
        cmp    dword ptr [edi + eax*4 + 0x61], 0
        jne    loc_2_14F6
        cmp    byte ptr [edi + 0x121], 0
        jne    loc_2_14F6
        push   edi
        call   sub_1_1B40
loc_2_14F6:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_2_1500:
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
        ja     loc_2_1550
        je     loc_2_16D0
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_2_1571
        shl    eax, 2
        jmp    loc_2_1838
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1550:
        cmp    eax, 0x1010102
        je     loc_2_16F2
        cmp    eax, 0x1010103
        je     loc_2_1714
        cmp    eax, 0x1010104
        je     loc_2_17E6
loc_2_1571:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_2_17EF
        db  08Dh, 049h, 000h
loc_2_1580:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_2_1595
        mov    esi, 0x282c                   ; -> loc_1_282C
        mov    edi, 0x58
        jmp    loc_2_17EF
loc_2_1595:
        mov    esi, 0x27b4                   ; -> loc_1_27B4
        mov    edi, 0x78
        jmp    loc_2_17EF
loc_2_15A4:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x142], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_2_15BE
        mov    dword ptr [ebp - 0x14], 0
loc_2_15BE:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_2_17EF
        db  005h, 000h, 000h, 000h, 000h
loc_2_15D0:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_2_17EF
        db  08Dh, 049h, 000h
loc_2_15E0:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_15F0:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_1600:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_1610:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_2_17EC
        db  08Bh, 0FFh
loc_2_1620:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 0x20
        sub    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_2_17EC
loc_2_1633:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_2_17EF
        db  090h
loc_2_1640:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_1650:
        mov    esi, 0x2928                   ; -> obj1:0x2928
        mov    edi, 0x15
        jmp    loc_2_17EF
        db  090h
loc_2_1660:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_1677
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x14f]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_2_17EC
loc_2_1677:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_2_17EC
loc_2_1685:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_1699
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x161]
        jmp    loc_2_17EC
loc_2_1699:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        jmp    loc_2_17EC
loc_2_16A4:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_2_17EF
loc_2_16B7:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_2_17EF
loc_2_16C3:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_2_17EF
        db  090h
loc_2_16D0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x10e
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_260C
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_17EF
loc_2_16F2:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x108
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_260C
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_17EF
loc_2_1714:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_2_1781
        mov    eax, dword ptr [ebx + 0x14f]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_1749
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_17EF
loc_2_1749:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebx + 0x14f]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2B90
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_17EF
        jmp    loc_2_17EF
loc_2_1781:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x14f]
        push   eax
        call   sub_2_2A90
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_17B4
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_17EF
loc_2_17B4:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x14f]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2AD0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_2_17EF
loc_2_17E6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x41]
loc_2_17EC:
        mov    dword ptr [ebp - 8], eax
loc_2_17EF:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_182B
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_2_182B
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_2_181B
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
loc_2_181B:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_260C
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_2_182B:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  090h
loc_2_1838:
        adc    byte ptr [0x15a40000], 0      ; -> loc_2_15A4
        add    al, dl                        ; -> loc_2_15D0
        adc    eax, 0x15d00000               ; -> loc_2_15D0
        add    byte ptr [eax], al
        loopne loc_2_185F
        add    byte ptr [eax], al
        dd  loc_2_15F0               ; fixup
        dd  loc_2_1600               ; fixup
        dd  loc_2_1610               ; fixup
        dd  loc_2_1620               ; fixup
        dd  loc_2_1633               ; fixup
loc_2_185F:
        add    byte ptr [eax + 0x16], al     ; -> loc_2_1640
        add    byte ptr [eax], al
        jno    0x187b
        add    byte ptr [eax], al
        push   eax
        push   ss
        add    byte ptr [eax], al
        pushal 
        push   ss
        add    byte ptr [eax], al
        test   dword ptr [esi], edx
        add    byte ptr [eax], al
        movsb  byte ptr es:[edi], byte ptr [esi]
        push   ss
        add    byte ptr [eax], al
        mov    bh, 0x16
        add    byte ptr [eax], al
        jno    loc_2_1893
        add    byte ptr [eax], al
        ret    
        db  016h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1890:
        push   ebp
        mov    ebp, esp
loc_2_1893:
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
        call   sub_2_1500
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_18E0:
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
        je     loc_2_1937
        cmp    eax, 0x1010f
        je     loc_2_1998
        cmp    eax, 0x10112
        je     loc_2_1A20
        cmp    eax, 0x1010103
        je     loc_2_1A50
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_1A91
loc_2_1937:
        cmp    edi, 4
        je     loc_2_1954
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_1A91
loc_2_1954:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_260C
        test   dword ptr [ebp - 8], 0xf050
        je     loc_2_1981
        mov    ebx, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_1A91
loc_2_1981:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1AB0
        jmp    loc_2_1A8F
loc_2_1998:
        cmp    edi, 4
        je     loc_2_19B5
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_1A91
loc_2_19B5:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_260C
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_2_1A19
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x161]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_2_19F0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x161], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_1A91
        db  090h
loc_2_19F0:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_2_1A11
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_2_1A11
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        push   edx
        call   sub_2_1230
        jmp    loc_2_1A91
loc_2_1A11:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_1A91
loc_2_1A19:
        mov    ebx, 0xc0010014
        jmp    loc_2_1A91
loc_2_1A20:
        cmp    edi, 4
        je     loc_2_1A3A
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_1A91
loc_2_1A3A:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_260C
        sub    ebx, ebx
        jmp    loc_2_1A91
        db  08Dh, 049h, 000h
loc_2_1A50:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_2_1A74
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_1A91
loc_2_1A74:
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
        call   sub_2_1B10
loc_2_1A8F:
        mov    ebx, eax
loc_2_1A91:
        or     ebx, ebx
        jne    loc_2_1A9F
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_2_1A9F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x142], 0
        jne    loc_2_1AFA
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1AF0
        inc    dword ptr [esi + 0x1c]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x14f]
        push   eax
        call   sub_2_2A10
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_2_1AF0:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_2_1AFA:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x142], 0
        jne    loc_2_1B60
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1B52
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
        mov    eax, dword ptr [ecx + 0x14f]
        push   eax
        call   sub_2_2730
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_2_1B52:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_2_1B60:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_1500
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_2_1C76
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_1BF6
        je     loc_2_1C38
        cmp    eax, 0x20101
        je     loc_2_1C12
        cmp    eax, 0x20102
        je     loc_2_1C20
        cmp    eax, 0x20103
        je     loc_2_1C28
        cmp    eax, 0x20104
        je     loc_2_1C30
        mov    edi, 0xc0010017
        jmp    loc_2_1C59
loc_2_1BF6:
        cmp    eax, 0x1020101
        je     loc_2_1C40
        cmp    eax, 0x1020102
        je     loc_2_1C48
        cmp    eax, 0x1020103
        je     loc_2_1C50
        mov    edi, 0xc0010017
        jmp    loc_2_1C59
loc_2_1C12:
        mov    eax, dword ptr [ebx + 0x122]
        jmp    loc_2_1C56
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1C20:
        mov    eax, dword ptr [ebx + 0x126]
        jmp    loc_2_1C56
loc_2_1C28:
        mov    eax, dword ptr [ebx + 0x12a]
        jmp    loc_2_1C56
loc_2_1C30:
        mov    eax, dword ptr [ebx + 0x13a]
        jmp    loc_2_1C56
loc_2_1C38:
        mov    eax, dword ptr [ebx + 0x13e]
        jmp    loc_2_1C56
loc_2_1C40:
        mov    eax, dword ptr [ebx + 0x136]
        jmp    loc_2_1C56
loc_2_1C48:
        mov    eax, dword ptr [ebx + 0x12e]
        jmp    loc_2_1C56
loc_2_1C50:
        mov    eax, dword ptr [ebx + 0x132]
loc_2_1C56:
        mov    dword ptr [ebp - 4], eax
loc_2_1C59:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_2_1C76
        or     edi, edi
        jne    loc_2_1C76
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_260C
        add    dword ptr [ebp - 8], 4
loc_2_1C76:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_1C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_1CD0
        cmp    eax, 0x20101
        jae    loc_2_1CF1
        cmp    eax, 0x10101
        jb     loc_2_1CEC
        cmp    eax, 0x1010b
        jbe    loc_2_1CF1
        cmp    eax, 0x1010d
        jb     loc_2_1CEC
        cmp    eax, 0x10111
        jbe    loc_2_1CF1
        cmp    eax, 0x10113
        je     loc_2_1CF1
        jmp    loc_2_1CEC
loc_2_1CD0:
        cmp    eax, 0x1010101
        jb     loc_2_1CEC
        cmp    eax, 0x1010104
        jbe    loc_2_1CF1
        cmp    eax, 0x1020101
        jb     loc_2_1CEC
        cmp    eax, 0x1020103
        jbe    loc_2_1CF1
loc_2_1CEC:
        mov    esi, 0xc0010017
loc_2_1CF1:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x30]
        or     esi, esi
        jne    loc_2_1D04
        push   ecx
        push   edi
        call   sub_2_1B70
        mov    esi, eax
loc_2_1D04:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_1D40
        cmp    byte ptr [edi + 0x142], 0
        jne    loc_2_1D25
        cmp    dword ptr [edi + 0x171], 0
        jne    loc_2_1D25
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_1D40
loc_2_1D25:
        push   1
        add    edi, 0x17d
        push   edi
        call   sub_1_2502
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_2_1D40:
        cmp    dword ptr [edi + 0x4b], 0
        je     loc_2_1D5F
        mov    eax, dword ptr [edi + 0x53]
        cmp    dword ptr [edi + eax*4 + 0x61], 0
        jne    loc_2_1D5F
        cmp    byte ptr [edi + 0x121], 0
        jne    loc_2_1D5F
        push   edi
        call   sub_1_1B40
loc_2_1D5F:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x2768]       ; -> obj1:0x2768
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_244C
        push   0
        mov    eax, dword ptr [0x276c]       ; -> obj1:0x276C
        push   eax
        call   sub_1_2440
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_1DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x1b6], 1
        cmp    esi, dword ptr [0x27a6]       ; -> obj1:0x27A6
        jne    loc_2_1DC3
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x27a6], eax       ; -> obj1:0x27A6
        jmp    loc_2_1DE1
loc_2_1DC3:
        mov    ecx, dword ptr [0x27a6]       ; -> obj1:0x27A6
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_1DD8
        mov    edi, edi
loc_2_1DD0:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_1DD0
loc_2_1DD8:
        mov    eax, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], eax
loc_2_1DE1:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x1a9]
        push   eax
        call   sub_1_2508
        push   0x2625a0
        call   sub_1_2520
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_2_2550
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_2514
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2458
        push   0
        push   0x2c3
        push   esi
        call   sub_1_252C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_2_1E53
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E53:
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x142], 0
        je     loc_2_1E76
        dec    dword ptr [edi + 0x1c]
        dec    dword ptr [esi + 0x30]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E76:
        mov    ebx, dword ptr [esi + 0x28]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     loc_2_1EA6
loc_2_1E84:
        inc    dword ptr [ebx + 0x1c]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010004
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_248E
        mov    eax, dword ptr [ebx + 0xc]
        dec    dword ptr [ebx + 0x1c]
        mov    ebx, eax
        or     eax, eax
        jne    loc_2_1E84
loc_2_1EA6:
        mov    byte ptr [esi + 0x142], 1
        mov    dword ptr [esi + 0x143], 0
        mov    dword ptr [esi + 0x147], edi
        push   esi
        call   sub_2_1F30
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [edi + 0x1c]
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_1F04
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_1EEA
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_1EEA
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_1F04
loc_2_1EEA:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1F04:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_1F23
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_1F23
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_1F23
        push   esi
        call   sub_1_1B40
loc_2_1F23:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_1F30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x171], 0
        je     loc_2_1F7C
loc_2_1F44:
        mov    ecx, dword ptr [esi + 0x171]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x171], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_2_1F70
        cmp    eax, 8
        je     loc_2_1F70
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_247C
loc_2_1F70:
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_1F44
loc_2_1F7C:
        cmp    dword ptr [esi + 0x5b], -1
        je     loc_2_1F9A
        mov    dword ptr [esi + 0x143], 2
        dec    dword ptr [esi + 0x30]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1F9A:
        dec    dword ptr [esi + 0x30]
        push   esi
        call   sub_2_1FB0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1FB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        inc    dword ptr [eax + 0x30]
        mov    dword ptr [eax + 0x53], 0
        mov    dword ptr [eax + 0x57], 0
        mov    edx, dword ptr [eax + 0x38]
        cmp    edx, ecx
        je     loc_2_1FE1
        lea    esi, [eax + 0x61]
loc_2_1FD4:
        cmp    dword ptr [esi], 0
        jne    loc_2_1FF0
        add    esi, 4
        inc    ecx
        cmp    ecx, edx
        jb     loc_2_1FD4
loc_2_1FE1:
        dec    dword ptr [eax + 0x30]
        push   eax
        call   sub_2_2010
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_1FF0:
        mov    dword ptr [eax + 0x143], 3
        dec    dword ptr [eax + 0x30]
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2010:
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
        jbe    loc_2_2060
        lea    edi, [esi + 0x91]
loc_2_2032:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_2_2052
        mov    ebx, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_246A
        dec    dword ptr [ebx + 0x1c]
        mov    dword ptr [edi], 0
loc_2_2052:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0x38], eax
        ja     loc_2_2032
loc_2_2060:
        mov    edx, dword ptr [esi + 0x4b]
        or     edx, edx
        je     loc_2_208B
loc_2_2067:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x4b], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_246A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x4b]
        or     edx, edx
        jne    loc_2_2067
loc_2_208B:
        mov    edx, dword ptr [esi + 0x165]
        or     edx, edx
        je     loc_2_20BF
loc_2_2095:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x165], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_246A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x165]
        or     edx, edx
        jne    loc_2_2095
loc_2_20BF:
        cmp    byte ptr [esi + 0x121], 0
        je     loc_2_20DB
loc_2_20C8:
        push   0x2710
        call   sub_1_2520
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_20C8
loc_2_20DB:
        mov    byte ptr [esi + 0x160], 0x1b
        push   esi
        call   sub_2_06F0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x142], 0
        mov    edi, dword ptr [esi + 0x28]
        or     edi, edi
        je     loc_2_2142
        sub    ebx, ebx
loc_2_2104:
        inc    dword ptr [edi + 0x1c]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_2_211B
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_248E
loc_2_211B:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_248E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_2494
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        cmp    eax, ebx
        jne    loc_2_2104
loc_2_2142:
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x143], eax
        jne    loc_2_224F
        cmp    eax, 1
        je     loc_2_21E2
        cmp    eax, 2
        je     loc_2_21EA
        cmp    eax, 3
        je     loc_2_21F2
        mov    eax, dword ptr [ebp - 4]
loc_2_218C:
        cmp    eax, 0x103
        je     loc_2_21AB
        push   eax
        mov    eax, dword ptr [esi + 0x147]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_2476
        mov    eax, dword ptr [esi + 0x147]
        dec    dword ptr [eax + 0x1c]
loc_2_21AB:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_2230
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_21CC
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_21CC
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_2230
loc_2_21CC:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_21E2:
        push   esi
        call   sub_2_1F30
        jmp    loc_2_218C
loc_2_21EA:
        push   esi
        call   sub_2_1FB0
        jmp    loc_2_218C
loc_2_21F2:
        mov    ecx, dword ptr [esi + 0x30]
        inc    ecx
        mov    dword ptr [esi + 0x30], ecx
        sub    edx, edx
        mov    eax, dword ptr [esi + 0x38]
        cmp    eax, edx
        je     loc_2_2212
        lea    edi, [esi + 0x61]
loc_2_2205:
        cmp    dword ptr [edi], 0
        jne    loc_2_2221
        add    edi, 4
        inc    edx
        cmp    edx, eax
        jb     loc_2_2205
loc_2_2212:
        dec    ecx
        mov    dword ptr [esi + 0x30], ecx
        push   esi
        call   sub_2_2010
        jmp    loc_2_218C
loc_2_2221:
        dec    ecx
        mov    dword ptr [esi + 0x30], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_2230:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_224F
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_224F
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_224F
        push   esi
        call   sub_1_1B40
loc_2_224F:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2260:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_2_2273
        lea    edx, [eax + 0x49]
loc_2_2273:
        cmp    byte ptr [ecx + 0x142], 0
        je     loc_2_2285
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_2_2285:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x171], 0
        jne    loc_2_22C0
        mov    dword ptr [ecx + 0x175], edx
        mov    dword ptr [ecx + 0x171], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_2_22C0:
        mov    eax, dword ptr [ecx + 0x175]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x175], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_22E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_2_22F3
        lea    edx, [eax + 0x21]
loc_2_22F3:
        cmp    byte ptr [ecx + 0x142], 0
        je     loc_2_2305
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_2_2305:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x171], 0
        jne    loc_2_2340
        mov    dword ptr [ecx + 0x175], edx
        mov    dword ptr [ecx + 0x171], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_2_2340:
        mov    eax, dword ptr [ecx + 0x175]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x175], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2360:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x1c]
        ret    4
        db  08Dh, 049h, 000h
sub_2_2370:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x14f]
        mov    edi, dword ptr [eax + 0xc]
        test   edi, 0x24
        je     loc_2_23A0
        push   0x870                         ; -> loc_2_0870
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_251A
        jmp    loc_2_23A5
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_23A0:
        call   sub_2_2400
loc_2_23A5:
        test   edi, 0x26
        lea    eax, [esi + 0x15f]
        je     loc_2_23B8
        or     byte ptr [eax], 8
        jmp    loc_2_23BB
loc_2_23B8:
        and    byte ptr [eax], 0xf7
loc_2_23BB:
        test   edi, 0x20
        je     loc_2_23C8
        or     byte ptr [eax], 0x10
        jmp    loc_2_23CB
loc_2_23C8:
        and    byte ptr [eax], 0xef
loc_2_23CB:
        test   edi, 0x28
        je     loc_2_23D8
        or     byte ptr [eax], 4
        jmp    loc_2_23DB
loc_2_23D8:
        and    byte ptr [eax], 0xfb
loc_2_23DB:
        push   esi
        push   0x870                         ; -> loc_1_0870
        add    esi, 8
        push   esi
        call   sub_1_251A
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2400:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_07E0
        push   esi
        push   0x8b0                         ; -> loc_2_08B0
        add    esi, 8
        push   esi
        call   sub_1_251A
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2430:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x2940]       ; -> obj1:0x2940
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_2526
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_2464
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2464:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_247E
        mov    esi, 2
loc_2_247E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_2526
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_24B2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2550
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_24B2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_2526
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_24E1
        push   eax
        call   sub_2_2550
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_24E1:
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
sub_2_2550:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_2570
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_252C
loc_2_2570:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_2586
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_252C
loc_2_2586:
        push   0
        push   0x32
        push   esi
        call   sub_1_252C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_25A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_25B9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_25B9:
        mov    eax, dword ptr [0x2940]       ; -> obj1:0x2940
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2526
        or     eax, eax
        je     loc_2_25D9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_25D9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_25E0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_25F3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_25E0
        jmp    loc_2_2602
loc_2_25F3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_2602:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_2618
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2618:
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
sub_2_2670:
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
        call   sub_2_2A10
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_2699
        cmp    ebx, 0x103
        jne    loc_2_26B0
loc_2_2699:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_2730
        or     eax, eax
        je     loc_2_26B0
        mov    ebx, eax
loc_2_26B0:
        or     ebx, ebx
        je     loc_2_26BC
        cmp    ebx, 0x103
        jne    loc_2_271B
loc_2_26BC:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_2716
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_26D2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_26D2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_26E1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_26E7
loc_2_26E1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_26E7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_26F6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2488
loc_2_26F6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_252C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_2716:
        mov    ebx, 0xc001000e
loc_2_271B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2730:
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
        call   sub_1_262C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_262C
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_27B4
        sub    eax, eax
        nop    
loc_2_2790:
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
        ja     loc_2_2790
loc_2_27B4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_281F
loc_2_27C1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_2814
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
        call   sub_1_262C
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
        call   sub_1_262C
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_2817
loc_2_2814:
        inc    dword ptr [ebp - 4]
loc_2_2817:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_27C1
loc_2_281F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_290A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_2833:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2BF0
        or     al, al
        je     loc_2_286A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_28F8
loc_2_286A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_2944
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
        call   sub_1_262C
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
        call   sub_1_262C
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
loc_2_28F8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_2833
loc_2_290A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_2995
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_2920:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_299B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_2982
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_2982
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_2989
loc_2_2944:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_262C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_262C
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_2982:
        mov    dword ptr [ebp - 8], 1
loc_2_2989:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_2920
loc_2_2995:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_2A00
loc_2_299B:
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
        je     loc_2_2A02
        cmp    edi, 0x103
        je     loc_2_2A02
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_262C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_262C
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_2A00:
        sub    edi, edi
loc_2_2A02:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_2A10:
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
        je     loc_2_2A4D
        lea    esp, [esp]
loc_2_2A40:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_2A40
loc_2_2A4D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_2A81
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
        je     loc_2_2A83
        cmp    eax, 0x103
        je     loc_2_2A83
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2A81:
        sub    eax, eax
loc_2_2A83:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_2A90:
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
        je     loc_2_2AC2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2AB7:
        test   dword ptr [esi], edx
        je     loc_2_2ABC
        inc    eax
loc_2_2ABC:
        add    esi, 4
        dec    edi
        jne    loc_2_2AB7
loc_2_2AC2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2AD0:
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
        jbe    loc_2_2B4C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_2B10:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_2B3A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_2B66
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_262C
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_2B3A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_2B10
loc_2_2B4C:
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
loc_2_2B66:
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
sub_2_2B90:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_2BC0
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2BC0:
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
        call   sub_1_262C
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2BF0:
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
        je     loc_2_2C89
        dec    esi
        cmp    esi, edx
        jb     loc_2_2C89
        nop    
loc_2_2C10:
        cmp    edx, ecx
        jb     loc_2_2C89
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_2C33
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2C67
loc_2_2C33:
        jbe    loc_2_2C40
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_2C67
        db  08Bh, 0FFh
loc_2_2C40:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_2C57
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2C67
loc_2_2C57:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_2C67
        mov    dword ptr [ebp - 4], 0
loc_2_2C67:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2C99
        jle    loc_2_2C78
        or     edx, edx
        je     loc_2_2C89
        lea    esi, [edx - 1]
        jmp    loc_2_2C7B
loc_2_2C78:
        lea    ecx, [edx + 1]
loc_2_2C7B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_2C10
loc_2_2C89:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2C99:
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
