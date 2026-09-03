; SNIP -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7175 instructions recovered by recursive descent

Declare_Virtual_Device SNIP, 3, 10, SNIP_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2934  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
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
        mov    dword ptr [ebp - 0x24], eax
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
        je     loc_1_00D8
        in     al, dx
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     al, dx
        jmp    loc_1_00DE
loc_1_00D8:
        in     ax, dx
        mov    word ptr [ebp - 6], ax
loc_1_00DE:
        mov    word ptr [ebp - 6], 0xff
loc_1_00E4:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 8
        in     al, dx
        mov    di, ax
        and    edi, 0xffff00ff
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 9
        in     al, dx
        and    ax, 0xff
        shl    ax, 8
        or     di, ax
        cmp    di, cx
        jne    loc_1_0140
        dec    word ptr [ebp - 6]
        jne    loc_1_00E4
        cmp    di, cx
        jne    loc_1_0140
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
        db  090h
loc_1_0140:
        cmp    byte ptr [esi + 0x120], 0
        je     loc_1_0166
        mov    cx, word ptr [esi + 0x103]
        mov    dx, cx
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, bh
        jmp    loc_1_0234
loc_1_0166:
        mov    di, word ptr [esi + 0x103]
        mov    dx, di
        sub    al, al
        out    dx, al
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_0222
        lea    ecx, [ebx - 1]
        lea    edx, [edi + 8]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 9]
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
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    di, word ptr [esi + 0x103]
        mov    al, cl
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 9]
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
        mov    eax, dword ptr [ebp - 0x14]
        lea    edx, [edi + 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0xffff00ff
        or     ax, cx
        out    dx, ax
        mov    di, 0xff
        add    eax, 0
loc_1_01F0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        and    ax, 0xff
        mov    word ptr [ebp - 4], ax
        test   al, 0x40
        jne    loc_1_0218
        dec    di
        push   0x3e8
        call   sub_1_2520
        or     di, di
        jne    loc_1_01F0
loc_1_0218:
        inc    dword ptr [ebp - 0x14]
        inc    ebx
        dec    dword ptr [ebp - 0x20]
        dec    dword ptr [ebp - 0x10]
loc_1_0222:
        mov    cx, word ptr [esi + 0x103]
        mov    eax, ebx
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
loc_1_0234:
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
loc_1_024B:
        cmp    dword ptr [ebp - 0x18], ebx
        je     loc_1_0281
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
loc_1_0281:
        cmp    byte ptr [esi + 0x120], bl
        mov    eax, dword ptr [ebp - 0x10]
        je     loc_1_02A2
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x103]
        add    eax, 0x10
        push   eax
        call   sub_1_23C0
        jmp    loc_1_02D2
loc_1_02A2:
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x103]
        add    eax, 0x10
        push   eax
        call   sub_1_23D4
        test   byte ptr [ebp - 0x10], 1
        je     loc_1_02D2
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ecx + eax - 1]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 0x18], 1
loc_1_02D2:
        mov    edi, 0xff
loc_1_02D7:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0x40
        jne    loc_1_02F5
        dec    edi
        push   0x3e8
        call   sub_1_2520
        cmp    edi, ebx
        jne    loc_1_02D7
loc_1_02F5:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_034D
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_0356
loc_1_0320:
        cmp    dword ptr [ebp - 0x10], ebx
        jne    loc_1_034D
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_034D
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_0320
loc_1_034D:
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_024B
loc_1_0356:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_0372
        mov    edx, dword ptr [esi + 0x103]
        mov    ecx, dword ptr [ebp - 0x1c]
        add    dx, 0x10
        mov    al, byte ptr [ecx]
        and    eax, 0xffff00ff
        out    dx, ax
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
        sub    esp, 8
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
        je     loc_1_0400
        in     al, dx
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     al, dx
        jmp    loc_1_0406
        db  08Bh, 0FFh
loc_1_0400:
        in     ax, dx
        mov    word ptr [ebp - 6], ax
loc_1_0406:
        mov    word ptr [ebp - 6], 0xff
        lea    esp, [esp]
loc_1_0410:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 8
        in     al, dx
        mov    di, ax
        and    edi, 0xffff00ff
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 9
        in     al, dx
        and    ax, 0xff
        shl    ax, 8
        or     di, ax
        cmp    di, cx
        jne    loc_1_0455
        dec    word ptr [ebp - 6]
        jne    loc_1_0410
        cmp    di, cx
        jne    loc_1_0455
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0455:
        cmp    byte ptr [esi + 0x120], 0
        je     loc_1_04A2
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
        jmp    loc_1_05B9
loc_1_04A2:
        mov    di, word ptr [esi + 0x103]
        mov    dx, di
        sub    al, al
        out    dx, al
        or     ebx, ebx
        je     loc_1_0561
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
        in     ax, dx
        mov    word ptr [ebp - 4], ax
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
        mov    cl, byte ptr [ebp - 4]
        mov    al, byte ptr [edi]
        and    eax, 0xffff00ff
        shl    ax, 8
        and    ecx, 0xffff00ff
        or     ax, cx
        out    dx, ax
        mov    bx, 0xff
        mov    edi, edi
loc_1_0530:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        and    ax, 0xff
        mov    word ptr [ebp - 4], ax
        test   al, 0x40
        jne    loc_1_0558
        dec    bx
        push   0x3e8
        call   sub_1_2520
        or     bx, bx
        jne    loc_1_0530
loc_1_0558:
        inc    edi
        inc    dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0x14]
        jmp    loc_1_0564
loc_1_0561:
        mov    edi, dword ptr [ebp + 0x10]
loc_1_0564:
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
        je     loc_1_05B9
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        mov    eax, dword ptr [ebp + 0x14]
        mov    al, byte ptr [edi + eax - 1]
        and    eax, 0xffff00ff
        out    dx, ax
loc_1_05B9:
        mov    di, 0xff
        mov    ebx, 0x40
loc_1_05C2:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   bl, al
        jne    loc_1_05E2
        dec    di
        push   0x3e8
        call   sub_1_2520
        or     di, di
        jne    loc_1_05C2
loc_1_05E2:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
sub_1_05F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_0610
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0610:
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 0x103]
        in     al, dx
        cmp    byte ptr [esi + 0x120], 0
        je     loc_1_0667
        mov    ecx, dword ptr [esi + 0x103]
        mov    dx, cx
        sub    al, al
        out    dx, al
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
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        add    ecx, 0x10
        push   ecx
        call   sub_1_23FD
        jmp    loc_1_0725
loc_1_0667:
        mov    cx, word ptr [esi + 0x103]
        mov    dx, cx
        sub    al, al
        out    dx, al
        test   byte ptr [ebp + 0x10], 1
        je     loc_1_06D0
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
loc_1_06A6:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0x40
        jne    loc_1_06C5
        dec    bl
        push   0x3e8
        call   sub_1_2520
        or     bl, bl
        jne    loc_1_06A6
loc_1_06C5:
        inc    dword ptr [ebp + 0x10]
        inc    edi
        dec    dword ptr [ebp + 0x14]
        jmp    loc_1_06D3
        db  08Bh, 0FFh
loc_1_06D0:
        mov    edi, dword ptr [ebp + 0xc]
loc_1_06D3:
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
        je     loc_1_0725
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x10
        in     ax, dx
        mov    cl, al
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [edi + eax - 1], cl
loc_1_0725:
        mov    bl, 0xff
loc_1_0727:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   al, 0x40
        jne    loc_1_0746
        dec    bl
        push   0x3e8
        call   sub_1_2520
        or     bl, bl
        jne    loc_1_0727
loc_1_0746:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
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
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0x5b]
        mov    di, word ptr [esi + 0x103]
        mov    ecx, dword ptr [esi + ebx*4 + 0xc1]
        lea    edx, [edi + 4]
        mov    al, byte ptr [esi + 0xfd]
        add    al, bl
        out    dx, al
        cmp    ecx, 0x3c
        jae    loc_1_07B3
        mov    ecx, 0x3c
loc_1_07B3:
        lea    edx, [edi + 6]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 5]
        mov    al, cl
        out    dx, al
        mov    dx, di
        in     al, dx
        mov    dx, word ptr [esi + 0x103]
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
        call   sub_2_0880
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
        call   sub_1_05F0
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
        call   sub_2_0680
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
        call   sub_2_20F0
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
        call   sub_2_20F0
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
        call   sub_1_05F0
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
        call   sub_1_05F0
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
        call   sub_2_0880
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
        call   sub_2_20F0
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
        call   sub_2_0610
        cmp    dword ptr [esi + 0x14b], 4
        jbe    loc_1_1E61
        push   esi
        call   sub_2_0680
        mov    dword ptr [esi + 0x14b], 0
loc_1_1E61:
        push   esi
        call   sub_2_0880
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
        call   sub_2_2300
        jmp    loc_1_1F0B
loc_1_1F05:
        push   esi
        call   sub_2_2390
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
        call   sub_2_20F0
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
        call   sub_2_11C0
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
        call   sub_2_0610
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
        call   sub_2_08E0
        cmp    eax, 0
        jne    loc_1_2047
loc_1_2045:
        clc    
        ret    
loc_1_2047:
        stc    
        ret    
SNIP_Control:
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
        call   sub_2_2B80
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
sub_1_24F4:
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
sub_1_25F2:
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
        db  'SNIP    '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SNIP_Control             ; DDB_Control_Proc
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
        dec    esi
        dec    ecx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2890:
        push   ebx
        dec    esi
        dec    ecx
        push   eax
        add    byte ptr [eax], al
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
        add    byte ptr gs:[esi + 0x61], cl
        je     0x2975
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x65], dl
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x6f], 0x6375646e
        je     0x298c
        jb     0x293f
        push   ebx
        dec    esi
        dec    ecx
        push   eax
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x299d
        jb     0x295a
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2c40  R-X
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
        sub    esp, 4
        push   esi
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
        push   0x4e20
        call   sub_1_2520
        mov    dx, word ptr [esi + 0x103]
        in     al, dx
        cmp    al, 0x21
        je     loc_2_0097
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0097:
        mov    cx, word ptr [esi + 0x103]
        mov    al, 0x48
        lea    edx, [ecx + 0xe]
        out    dx, al
        lea    edx, [ecx + 0xf]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0xff
        out    dx, al
        lea    edx, [ecx + 8]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 9]
        out    dx, al
        lea    edx, [ecx + 0xa]
        mov    al, 0x20
        out    dx, al
        lea    edx, [ecx + 0xb]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        lea    edx, [ecx + 0x1f]
        in     al, dx
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0x1f
        mov    al, 0xff
        out    dx, al
        mov    byte ptr [esi + 0x120], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_2_00F0:
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
loc_2_0111:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_2_0153
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_05F0
        or     al, al
        je     loc_2_0153
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    loc_2_0153
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    loc_2_0153
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    loc_2_0153
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        je     loc_2_0166
loc_2_0153:
        add    edi, 0x400
        cmp    edi, 0x10000
        jb     loc_2_0111
        mov    ebx, dword ptr [ebp - 0x10]
        jmp    loc_2_01C0
loc_2_0166:
        mov    ebx, edi
        test   edi, 0xffff0000
        jne    loc_2_01C0
loc_2_0170:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_2_01C0
        push   4
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_05F0
        or     al, al
        je     loc_2_01C0
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    loc_2_01C0
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    loc_2_01C0
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    loc_2_01C0
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    loc_2_01C0
        add    ebx, 0x400
        test   ebx, 0xffff0000
        je     loc_2_0170
loc_2_01C0:
        cmp    edi, 0x10000
        jae    loc_2_0271
        cmp    ebx, edi
        je     loc_2_0271
        test   ebx, 0xffff0000
        je     loc_2_01E2
        sub    ebx, 0x100
loc_2_01E2:
        mov    dword ptr [ebp - 0xc], edi
        cmp    ebx, edi
        jbe    loc_2_0237
loc_2_01E9:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        je     loc_2_0250
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_05F0
        or     al, al
        je     loc_2_025B
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    loc_2_0266
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    loc_2_0266
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    loc_2_0266
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    loc_2_0266
        add    dword ptr [ebp - 0xc], 4
        cmp    ebx, dword ptr [ebp - 0xc]
        ja     loc_2_01E9
loc_2_0237:
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
loc_2_0250:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_025B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0266:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0271:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_0280:
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
        jne    loc_2_0320
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_0320:
        mov    cx, word ptr [esi + 0x103]
        mov    al, 0x37
        lea    edx, [ecx + 0xa]
        out    dx, al
        cmp    byte ptr [esi + 0x45], 3
        mov    dx, cx
        mov    al, 0xa
        out    dx, al
        jne    loc_2_0360
        mov    byte ptr [esi + 0x120], 0
loc_2_0340:
        mov    cx, word ptr [esi + 0x103]
        sub    al, al
        lea    edx, [ecx + 0xf]
        out    dx, al
        cmp    byte ptr [esi + 0x120], al
        mov    dx, cx
        out    dx, al
        lea    edx, [ecx + 0xe]
        je     loc_2_037B
        mov    al, 0x48
        jmp    loc_2_037D
loc_2_0360:
        push   esi
        call   sub_2_0050
        or     al, al
        jne    loc_2_0340
        push   esi
        call   sub_1_0760
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_037B:
        mov    al, 0x49
loc_2_037D:
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
loc_2_03A5:
        dec    cl
        je     loc_2_03B8
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   bl, al
        je     loc_2_03A5
loc_2_03B8:
        mov    cx, word ptr [esi + 0x103]
        mov    al, 2
        lea    edx, [ecx + 0xd]
        out    dx, al
        mov    dx, cx
        mov    al, 0x22
        out    dx, al
        push   esi
        call   sub_2_00F0
        or     al, al
        push   esi
        jne    loc_2_03E6
        call   sub_1_0760
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_03E6:
        call   sub_1_0760
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0400:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x108], 0
        jne    loc_2_04CF
        cmp    byte ptr [esi + 0x109], 0
        jne    loc_2_04CF
        cmp    byte ptr [esi + 0x10a], 0
        jne    loc_2_04CF
        cmp    byte ptr [esi + 0x10b], 0
        jne    loc_2_04CF
        cmp    byte ptr [esi + 0x10c], 0
        jne    loc_2_04CF
        cmp    byte ptr [esi + 0x10d], 0
        jne    loc_2_04CF
        push   0x1000
        mov    eax, dword ptr [esi + 0x46]
        push   eax
        mov    eax, dword ptr [esi + 4]
        lea    edi, [esi + 0x46]
        push   eax
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_25F2
        sub    eax, eax
loc_2_0471:
        mov    ecx, dword ptr [edi]
        inc    eax
        mov    cl, byte ptr [ecx + eax*2 + 0xfee]
        mov    byte ptr [esi + eax + 0x10d], cl
        cmp    eax, 6
        jb     loc_2_0471
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
loc_2_04CF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_04E0:
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
        je     loc_2_0508
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0508:
        cmp    byte ptr [ecx + 0x120], 0
        mov    edx, dword ptr [ecx + 0x103]
        je     loc_2_0520
        add    dx, 0xe
        mov    al, 0x48
        jmp    loc_2_0526
        db  090h
loc_2_0520:
        add    dx, 0xe
        mov    al, 0x49
loc_2_0526:
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
loc_2_059A:
        lea    edx, [edi + esi]
        inc    dx
        mov    al, byte ptr [ecx + edi + 0x108]
        out    dx, al
        inc    edi
        cmp    edi, 6
        jb     loc_2_059A
        mov    eax, dword ptr [ecx + 0x14f]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 0xc]
        and    edi, 4
        add    eax, 0
loc_2_05C0:
        or     edi, edi
        mov    eax, 0xff
        jne    loc_2_05D2
        xor    eax, eax
        mov    al, byte ptr [ecx + ebx + 0x153]
loc_2_05D2:
        lea    edx, [ebx + esi]
        add    dx, 8
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_2_05C0
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
sub_2_0610:
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
loc_2_0643:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 7
        in     al, dx
        test   bl, al
        jne    loc_2_0662
        push   0x3e8
        call   sub_1_2520
        inc    edi
        cmp    edi, 4
        jb     loc_2_0643
loc_2_0662:
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
sub_2_0680:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0610
        push   0x7d0
        call   sub_1_2520
        push   esi
        call   sub_2_04E0
        or     al, al
        jne    loc_2_06C0
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
loc_2_06C0:
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xd
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_2_072D
loc_2_06F2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_2_0700:
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
        je     loc_2_0724
        xor    eax, 0x4c11db6
        or     eax, edi
loc_2_0724:
        dec    esi
        jne    loc_2_0700
        inc    edx
        cmp    edx, dword ptr [ebp + 0xc]
        jb     loc_2_06F2
loc_2_072D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0740:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_06E0
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
sub_2_0770:
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
loc_2_0790:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x153], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_2_0790
        push   0x26c0                        ; -> obj1:0x26C0
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2B20
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_2_07F7
        sub    edi, edi
loc_2_07C5:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x26c0]         ; -> obj1:0x26C0
        push   eax
        call   sub_2_0740
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x153], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_2_07C5
loc_2_07F7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0800:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    si, word ptr [eax + 0x103]
        mov    dx, si
        mov    al, 0x62
        out    dx, al
loc_2_0816:
        lea    edx, [esi + ecx]
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     loc_2_0816
        mov    dx, si
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0840:
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
loc_2_0857:
        lea    edx, [edi + esi]
        add    dx, 8
        mov    al, byte ptr [ecx + esi + 0x153]
        out    dx, al
        inc    esi
        cmp    esi, 8
        jb     loc_2_0857
        mov    dx, di
        mov    al, 0x22
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0880:
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
        je     loc_2_08D1
        mov    byte ptr [edi + 0x5f], 1
        push   edi
        call   sub_1_0780
loc_2_08D1:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        mov    word ptr [ebp - 0x10], 5
        mov    word ptr [ebp - 0xe], 5
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
        mov    byte ptr [0x2771], 0xa        ; -> obj1:0x2771
        mov    dword ptr [0x2772], 0         ; -> obj1:0x2772
        mov    dword ptr [0x2776], 0xff0     ; -> obj1:0x2776
        mov    dword ptr [0x277a], 0x1200    ; -> obj1:0x277A
        mov    dword ptr [0x277e], 0x1a20    ; -> obj1:0x277E
        mov    dword ptr [0x2782], 0x1700    ; -> obj1:0x2782
        mov    dword ptr [0x2786], 0x1dc0    ; -> obj1:0x2786
        mov    dword ptr [0x278a], 0x13d0    ; -> obj1:0x278A
        mov    dword ptr [0x278e], 0x1c20    ; -> obj1:0x278E
        mov    dword ptr [0x2792], 0x1d00    ; -> obj1:0x2792
        mov    dword ptr [0x2796], 0xa20     ; -> obj1:0x2796
        mov    dword ptr [0x279a], 0x1d30    ; -> obj1:0x279A
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
        je     loc_2_0A10
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2440
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0A10:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x3c], 7
        mov    word ptr [ebp - 0x3a], 7
        mov    dword ptr [ebp - 0x38], 0x28a0 ; -> loc_1_28A0
        mov    word ptr [ebp - 0x34], 0xa
        mov    word ptr [ebp - 0x32], 0xa
        mov    dword ptr [ebp - 0x30], 0x28b4 ; -> obj1:0x28B4
        mov    esi, 0x300
        mov    word ptr [ebp - 0x2c], 0xd
        mov    word ptr [ebp - 0x2a], 0xd
        mov    dword ptr [ebp - 0x28], 0x28d0 ; -> loc_1_28D0
        mov    word ptr [ebp - 0x44], 8
        mov    bl, 3
        mov    word ptr [ebp - 0x42], 8
        mov    dword ptr [ebp - 0x40], 0x2900 ; -> obj1:0x2900
        mov    dword ptr [ebp - 0x14], 0x10
        mov    eax, dword ptr [0x2764]       ; -> obj1:0x2764
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x2c3
        push   eax
        call   sub_1_2526
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_0AA9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0AA9:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0xb0
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_25A4
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0AF0
        push   0
        push   0x2c3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_252C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_2_0AF0:
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0B19
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ecx + 0x45], al
loc_2_0B19:
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0B3C
        mov    eax, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [eax + 4]
loc_2_0B3C:
        push   0
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0B5F
        mov    eax, dword ptr [ebp - 0xc]
        mov    bl, byte ptr [eax + 4]
loc_2_0B5F:
        lea    eax, [ebp - 0x5a]
        mov    word ptr [ebp - 0x5c], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0xc], 0
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   0x10
        lea    eax, [ebp - 0x5c]
        push   eax
        push   1
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_24F4
        add    esp, 0x1c
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0BB1
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0BB1:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [ecx + 0x46], eax
        lea    eax, [ebp - 0x2c]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_25AA
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0BE0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x14], ecx
loc_2_0BE0:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_24AC
        cmp    dword ptr [ebp - 0x1c], 6
        jne    loc_2_0C1F
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0C1F
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, edx
        add    ecx, 0x108
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_2_0C1F:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_25B0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x38], 0xc
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    dword ptr [eax + 0x3c], esi
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    byte ptr [eax + 0x40], bl
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x41], ecx
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0C90
        or     eax, eax
        je     loc_2_0C7B
        push   0
        push   0x2c3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_252C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0C7B:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x38], 0xc
        jbe    loc_2_0CB5
        mov    dword ptr [ebp - 4], 0xc000009a
loc_2_0CA9:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0CB5:
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
        jne    loc_2_0CA9
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0D22
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        push   1
        push   0xaa00000a
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0x10
        jmp    loc_2_0F53
loc_2_0D22:
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
        jne    loc_2_0D79
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_2_0F53
loc_2_0D79:
        push   esi
        call   sub_2_0280
        or     al, al
        jne    loc_2_0DA1
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_2_0F53
loc_2_0DA1:
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
        jbe    loc_2_0E2F
        lea    eax, [esi + 0x61]
        lea    ebx, [ebx]
loc_2_0E20:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 0x38], ecx
        ja     loc_2_0E20
loc_2_0E2F:
        mov    byte ptr [esi + 0x1b5], 0
        mov    byte ptr [esi + 0x11f], 0
        push   esi
        call   sub_2_0400
        lea    ebx, [esi + 0x14f]
        push   ebx
        push   esi
        lea    eax, [esi + 0x108]
        push   eax
        push   0x22f0                        ; -> loc_2_22F0
        push   0x2270                        ; -> loc_2_2270
        push   0x21f0                        ; -> loc_2_21F0
        mov    eax, dword ptr [esi + 0x41]
        push   eax
        call   sub_2_23C0
        or     al, al
        jne    loc_2_0E90
        push   0
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_2_0F53
        db  08Dh, 064h, 024h, 000h
loc_2_0E90:
        mov    byte ptr [esi + 0x160], 0x1b
        mov    dword ptr [esi + 0x20], 0x2768 ; -> obj1:0x2768
        mov    eax, dword ptr [0x27a6]       ; -> obj1:0x27A6
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [0x27a6], esi       ; -> obj1:0x27A6
        push   esi
        call   sub_2_04E0
        or     al, al
        jne    loc_2_0ED1
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_2_0F12
loc_2_0ED1:
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
        je     loc_2_0F70
        push   0
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_25F8
        add    esp, 0xc
loc_2_0F12:
        cmp    dword ptr [0x27a6], esi       ; -> obj1:0x27A6
        jne    loc_2_0F24
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x27a6], eax       ; -> obj1:0x27A6
        jmp    loc_2_0F41
loc_2_0F24:
        mov    ecx, dword ptr [0x27a6]       ; -> obj1:0x27A6
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_0F38
        nop    
loc_2_0F30:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_0F30
loc_2_0F38:
        mov    eax, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], eax
loc_2_0F41:
        mov    dx, word ptr [esi + 0x103]
        mov    al, 1
        out    dx, al
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_24E0
loc_2_0F53:
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
loc_2_0F70:
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
loc_2_0FF0:
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
loc_2_1013:
        or     edx, edx
        je     loc_2_1022
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_1013
        mov    dword ptr [ecx], edx
loc_2_1022:
        cmp    dword ptr [ecx], -1
        jne    loc_2_1034
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_1034:
        mov    eax, dword ptr [0x2764]       ; -> obj1:0x2764
        push   eax
        push   0
        push   0x75
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2526
        or     eax, eax
        je     loc_2_1070
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
loc_2_1070:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x30]
        mov    ecx, dword ptr [esi + 0x28]
        or     ecx, ecx
        jne    loc_2_1090
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_2_1090
        mov    edx, dword ptr [esi + 0x103]
        add    dx, 0xd
        sub    al, al
        out    dx, al
loc_2_1090:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x28], eax
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_1182
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        mov    edi, dword ptr [ebp + 0x1c]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_2_2530
        or     al, al
        je     loc_2_1182
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
        jne    loc_2_1159
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_1141
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_1141
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_1159
loc_2_1141:
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
loc_2_1159:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_1178
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_1178
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_1178
        push   esi
        call   sub_1_1B40
loc_2_1178:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_1182:
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
sub_2_11C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [eax + 0x28]
        cmp    esi, edx
        je     loc_2_11E0
loc_2_11D0:
        mov    ecx, dword ptr [esi + 0x18]
        cmp    edx, ecx
        jae    loc_2_11D9
        mov    edx, ecx
loc_2_11D9:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_2_11D0
loc_2_11E0:
        or     edx, edx
        jne    loc_2_11E9
        mov    edx, 0xee
loc_2_11E9:
        mov    dword ptr [eax + 0x161], edx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1200:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x20], 0
        je     loc_2_1223
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1223:
        inc    dword ptr [edi + 0x30]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [edi + 0x28]
        cmp    ecx, esi
        jne    loc_2_1238
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0x28], eax
        jmp    loc_2_1251
loc_2_1238:
        cmp    dword ptr [ecx + 0xc], esi
        je     loc_2_1248
loc_2_1240:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], esi
        jne    loc_2_1240
loc_2_1248:
        lea    eax, [esi + 0xc]
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_2_1251:
        mov    edx, dword ptr [edi + 0x103]
        add    dx, 7
        in     al, dx
        cmp    al, 0xff
        jne    loc_2_12BD
        mov    edx, dword ptr [edi + 0x103]
        add    dx, 7
        sub    al, al
        out    dx, al
        in     al, dx
        cmp    al, 0xff
        jne    loc_2_12B0
        mov    eax, dword ptr [edi + 0x14f]
        mov    dword ptr [edi + 0x2bf], 1
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [edi + 0x14f]
        push   0
        mov    dword ptr [eax + 0x10], 0
        push   0x75
        push   esi
        call   sub_1_252C
        dec    dword ptr [edi + 0x30]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_12B0:
        mov    edx, dword ptr [edi + 0x103]
        add    dx, 7
        mov    al, 0xff
        out    dx, al
loc_2_12BD:
        push   0
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 0x14f]
        push   eax
        call   sub_2_2600
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_12F8
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, 2
        je     loc_2_12F0
        mov    byte ptr [esi + 0x20], 1
        sub    dword ptr [esi + 0x1c], 2
        mov    ebx, 0x103
        jmp    loc_2_1324
        db  08Dh, 064h, 024h, 000h
loc_2_12F0:
        sub    eax, 2
        mov    dword ptr [esi + 0x1c], eax
        jmp    loc_2_1324
loc_2_12F8:
        cmp    ebx, 0x103
        jne    loc_2_1310
        mov    byte ptr [esi + 0x20], 1
        sub    dword ptr [esi + 0x1c], 2
        jmp    loc_2_1324
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1310:
        cmp    ebx, 0xc001000e
        jne    loc_2_1321
        mov    byte ptr [esi + 0x20], 1
        mov    ebx, 0x103
loc_2_1321:
        dec    dword ptr [esi + 0x1c]
loc_2_1324:
        cmp    dword ptr [esi + 0x1c], 0
        jne    loc_2_1359
        mov    eax, dword ptr [edi + 0x161]
        cmp    dword ptr [esi + 0x18], eax
        jne    loc_2_133B
        push   edi
        call   sub_2_11C0
loc_2_133B:
        push   0
        push   0x75
        push   esi
        call   sub_1_252C
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_2_1367
        cmp    dword ptr [edi + 0x2c], 0
        jne    loc_2_1367
        push   edi
        call   sub_2_0610
        jmp    loc_2_1367
loc_2_1359:
        mov    eax, dword ptr [edi + 0x2c]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [edi + 0x2c], esi
        mov    ebx, 0x103
loc_2_1367:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_13A1
        cmp    byte ptr [edi + 0x142], 0
        jne    loc_2_1388
        cmp    dword ptr [edi + 0x171], 0
        jne    loc_2_1388
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_13A1
loc_2_1388:
        push   1
        add    edi, 0x17d
        push   edi
        call   sub_1_2502
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_13A1:
        cmp    dword ptr [edi + 0x4b], 0
        je     loc_2_13C0
        mov    eax, dword ptr [edi + 0x53]
        cmp    dword ptr [edi + eax*4 + 0x61], 0
        jne    loc_2_13C0
        cmp    byte ptr [edi + 0x121], 0
        jne    loc_2_13C0
        push   edi
        call   sub_1_1B40
loc_2_13C0:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_13D0:
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
        je     loc_2_13F0
        mov    esi, 0xc0010002
        jmp    loc_2_142C
        db  090h
loc_2_13F0:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        or     eax, eax
        jne    loc_2_1406
        push   ecx
        push   ebx
        push   edi
        call   sub_2_1820
        mov    esi, eax
        jmp    loc_2_142C
loc_2_1406:
        cmp    eax, 1
        jne    loc_2_1427
        cmp    byte ptr [edi + 0x142], 0
        je     loc_2_141B
        mov    esi, 0xc001000d
        jmp    loc_2_142C
loc_2_141B:
        push   ecx
        push   ebx
        push   edi
        call   sub_2_1870
        mov    esi, eax
        jmp    loc_2_142C
loc_2_1427:
        mov    esi, 0x10001
loc_2_142C:
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_1467
        cmp    byte ptr [edi + 0x142], 0
        jne    loc_2_1450
        cmp    dword ptr [edi + 0x171], 0
        jne    loc_2_1450
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_1467
loc_2_1450:
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
loc_2_1467:
        cmp    dword ptr [edi + 0x4b], 0
        je     loc_2_1486
        mov    eax, dword ptr [edi + 0x53]
        cmp    dword ptr [edi + eax*4 + 0x61], 0
        jne    loc_2_1486
        cmp    byte ptr [edi + 0x121], 0
        jne    loc_2_1486
        push   edi
        call   sub_1_1B40
loc_2_1486:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_2_1490:
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
        ja     loc_2_14E0
        je     loc_2_1660
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_2_1501
        shl    eax, 2
        jmp    loc_2_17C8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_14E0:
        cmp    eax, 0x1010102
        je     loc_2_1682
        cmp    eax, 0x1010103
        je     loc_2_16A4
        cmp    eax, 0x1010104
        je     loc_2_1776
loc_2_1501:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_2_177F
        db  08Dh, 049h, 000h
loc_2_1510:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_2_1525
        mov    esi, 0x282c                   ; -> loc_1_282C
        mov    edi, 0x58
        jmp    loc_2_177F
loc_2_1525:
        mov    esi, 0x27b4                   ; -> loc_1_27B4
        mov    edi, 0x78
        jmp    loc_2_177F
loc_2_1534:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x142], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_2_154E
        mov    dword ptr [ebp - 0x14], 0
loc_2_154E:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_2_177F
        db  005h, 000h, 000h, 000h, 000h
loc_2_1560:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_2_177F
        db  08Dh, 049h, 000h
loc_2_1570:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_1580:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_1590:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_15A0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_2_177C
        db  08Bh, 0FFh
loc_2_15B0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 0x20
        sub    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_2_177C
loc_2_15C3:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_2_177F
        db  090h
loc_2_15D0:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_15E0:
        mov    esi, 0x2908                   ; -> obj1:0x2908
        mov    edi, 0x24
        jmp    loc_2_177F
        db  090h
loc_2_15F0:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_1607
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x14f]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_2_177C
loc_2_1607:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_2_177C
loc_2_1615:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_1629
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x161]
        jmp    loc_2_177C
loc_2_1629:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        jmp    loc_2_177C
loc_2_1634:
        mov    word ptr [ebp - 2], 0x30a
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_2_177F
loc_2_1647:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_2_177F
loc_2_1653:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_2_177F
        db  090h
loc_2_1660:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x10e
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_260C
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_177F
loc_2_1682:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x108
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_260C
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_177F
loc_2_16A4:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_2_1711
        mov    eax, dword ptr [ebx + 0x14f]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_16D9
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_177F
loc_2_16D9:
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
        call   sub_2_2B20
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_177F
        jmp    loc_2_177F
loc_2_1711:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x14f]
        push   eax
        call   sub_2_2A20
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_1744
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_177F
loc_2_1744:
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
        call   sub_2_2A60
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_2_177F
loc_2_1776:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x41]
loc_2_177C:
        mov    dword ptr [ebp - 8], eax
loc_2_177F:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_17BB
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_2_17BB
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_2_17AB
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
loc_2_17AB:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_260C
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_2_17BB:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  090h
loc_2_17C8:
        adc    byte ptr [0x15340000], dl     ; -> loc_2_1534
        add    byte ptr [eax], al
        pushal 
        adc    eax, 0x15600000               ; -> loc_2_1560
        add    byte ptr [eax], al
        jo     0x17ef
        add    byte ptr [eax], al
        adc    byte ptr [0x15900000], 0      ; -> loc_2_1590
        add    byte ptr [eax - 0x4fffffeb], ah ; -> loc_2_15A0
        adc    eax, 0x15c30000               ; -> loc_2_15C3
        add    byte ptr [eax], al
        rcl    byte ptr [0x15010000]         ; -> loc_2_1501
        add    byte ptr [eax], al
        loopne loc_2_180F
        add    byte ptr [eax], al
        dd  loc_2_15F0               ; fixup
        dd  loc_2_1615               ; fixup
        dd  loc_2_1634               ; fixup
        dd  loc_2_1647               ; fixup
        dd  loc_2_1501               ; fixup
loc_2_180F:
        add    byte ptr [ebx + 0x16], dl     ; -> loc_2_1653
        add    byte ptr [eax], al
        add    eax, 0
        lea    esp, [esp]
sub_2_1820:
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
        call   sub_2_1490
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        sub    ebx, ebx
        mov    esi, dword ptr [edi + 0x1c]
        mov    ecx, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x14]
        cmp    eax, 0x1010e
        je     loc_2_18C7
        cmp    eax, 0x1010f
        je     loc_2_1928
        cmp    eax, 0x10112
        je     loc_2_19B0
        cmp    eax, 0x1010103
        je     loc_2_19E0
        mov    ebx, 0xc0010017
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_2_1A21
loc_2_18C7:
        cmp    esi, 4
        je     loc_2_18E4
        mov    ebx, 0xc0010014
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_2_1A21
loc_2_18E4:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_260C
        test   dword ptr [ebp - 8], 0xf050
        je     loc_2_1911
        mov    ebx, 0xc00000bb
        mov    dword ptr [edi + 0x20], 4
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_2_1A21
loc_2_1911:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1A40
        jmp    loc_2_1A1F
loc_2_1928:
        cmp    esi, 4
        je     loc_2_1945
        mov    ebx, 0xc0010014
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_2_1A21
loc_2_1945:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_260C
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_2_19A9
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x161]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_2_1980
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x161], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_1A21
        db  090h
loc_2_1980:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_2_19A1
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_2_19A1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        push   edx
        call   sub_2_11C0
        jmp    loc_2_1A21
loc_2_19A1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_1A21
loc_2_19A9:
        mov    ebx, 0xc0010014
        jmp    loc_2_1A21
loc_2_19B0:
        cmp    esi, 4
        je     loc_2_19CA
        mov    ebx, 0xc0010014
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_2_1A21
loc_2_19CA:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_260C
        sub    ebx, ebx
        jmp    loc_2_1A21
        db  08Dh, 049h, 000h
loc_2_19E0:
        mov    ebx, 6
        mov    eax, esi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_2_1A04
        mov    ebx, 0xc0010014
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_2_1A21
loc_2_1A04:
        push   ecx
        mov    ecx, 6
        mov    eax, esi
        sub    edx, edx
        div    ecx
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1AA0
loc_2_1A1F:
        mov    ebx, eax
loc_2_1A21:
        or     ebx, ebx
        jne    loc_2_1A2F
        mov    dword ptr [edi + 0x20], esi
        mov    dword ptr [edi + 0x24], 0
loc_2_1A2F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1A40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x142], 0
        jne    loc_2_1A8A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1A80
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
        call   sub_2_29A0
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_2_1A80:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_2_1A8A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1AA0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x142], 0
        jne    loc_2_1AF0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1AE2
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
        call   sub_2_26C0
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_2_1AE2:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_2_1AF0:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1B00:
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
        call   sub_2_1490
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_2_1C06
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_1B86
        je     loc_2_1BC8
        cmp    eax, 0x20101
        je     loc_2_1BA2
        cmp    eax, 0x20102
        je     loc_2_1BB0
        cmp    eax, 0x20103
        je     loc_2_1BB8
        cmp    eax, 0x20104
        je     loc_2_1BC0
        mov    edi, 0xc0010017
        jmp    loc_2_1BE9
loc_2_1B86:
        cmp    eax, 0x1020101
        je     loc_2_1BD0
        cmp    eax, 0x1020102
        je     loc_2_1BD8
        cmp    eax, 0x1020103
        je     loc_2_1BE0
        mov    edi, 0xc0010017
        jmp    loc_2_1BE9
loc_2_1BA2:
        mov    eax, dword ptr [ebx + 0x122]
        jmp    loc_2_1BE6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1BB0:
        mov    eax, dword ptr [ebx + 0x126]
        jmp    loc_2_1BE6
loc_2_1BB8:
        mov    eax, dword ptr [ebx + 0x12a]
        jmp    loc_2_1BE6
loc_2_1BC0:
        mov    eax, dword ptr [ebx + 0x13a]
        jmp    loc_2_1BE6
loc_2_1BC8:
        mov    eax, dword ptr [ebx + 0x13e]
        jmp    loc_2_1BE6
loc_2_1BD0:
        mov    eax, dword ptr [ebx + 0x136]
        jmp    loc_2_1BE6
loc_2_1BD8:
        mov    eax, dword ptr [ebx + 0x12e]
        jmp    loc_2_1BE6
loc_2_1BE0:
        mov    eax, dword ptr [ebx + 0x132]
loc_2_1BE6:
        mov    dword ptr [ebp - 4], eax
loc_2_1BE9:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_2_1C06
        or     edi, edi
        jne    loc_2_1C06
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_260C
        add    dword ptr [ebp - 8], 4
loc_2_1C06:
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
loc_2_1C20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_1C60
        cmp    eax, 0x20101
        jae    loc_2_1C81
        cmp    eax, 0x10101
        jb     loc_2_1C7C
        cmp    eax, 0x1010b
        jbe    loc_2_1C81
        cmp    eax, 0x1010d
        jb     loc_2_1C7C
        cmp    eax, 0x10111
        jbe    loc_2_1C81
        cmp    eax, 0x10113
        je     loc_2_1C81
        jmp    loc_2_1C7C
loc_2_1C60:
        cmp    eax, 0x1010101
        jb     loc_2_1C7C
        cmp    eax, 0x1010104
        jbe    loc_2_1C81
        cmp    eax, 0x1020101
        jb     loc_2_1C7C
        cmp    eax, 0x1020103
        jbe    loc_2_1C81
loc_2_1C7C:
        mov    esi, 0xc0010017
loc_2_1C81:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x30]
        or     esi, esi
        jne    loc_2_1C94
        push   ecx
        push   edi
        call   sub_2_1B00
        mov    esi, eax
loc_2_1C94:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_2_1CD0
        cmp    byte ptr [edi + 0x142], 0
        jne    loc_2_1CB5
        cmp    dword ptr [edi + 0x171], 0
        jne    loc_2_1CB5
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_2_1CD0
loc_2_1CB5:
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
loc_2_1CD0:
        cmp    dword ptr [edi + 0x4b], 0
        je     loc_2_1CEF
        mov    eax, dword ptr [edi + 0x53]
        cmp    dword ptr [edi + eax*4 + 0x61], 0
        jne    loc_2_1CEF
        cmp    byte ptr [edi + 0x121], 0
        jne    loc_2_1CEF
        push   edi
        call   sub_1_1B40
loc_2_1CEF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1D00:
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
loc_2_1D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x1b6], 1
        cmp    esi, dword ptr [0x27a6]       ; -> obj1:0x27A6
        jne    loc_2_1D53
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x27a6], eax       ; -> obj1:0x27A6
        jmp    loc_2_1D71
loc_2_1D53:
        mov    ecx, dword ptr [0x27a6]       ; -> obj1:0x27A6
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_1D68
        mov    edi, edi
loc_2_1D60:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_1D60
loc_2_1D68:
        mov    eax, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], eax
loc_2_1D71:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x1a9]
        push   eax
        call   sub_1_2508
        push   0x2625a0
        call   sub_1_2520
        mov    eax, dword ptr [esi + 0x14f]
        push   eax
        call   sub_2_24E0
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
loc_2_1DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_2_1DE3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1DE3:
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x142], 0
        je     loc_2_1E06
        dec    dword ptr [edi + 0x1c]
        dec    dword ptr [esi + 0x30]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E06:
        mov    ebx, dword ptr [esi + 0x28]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     loc_2_1E36
loc_2_1E14:
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
        jne    loc_2_1E14
loc_2_1E36:
        mov    byte ptr [esi + 0x142], 1
        mov    dword ptr [esi + 0x143], 0
        mov    dword ptr [esi + 0x147], edi
        push   esi
        call   sub_2_1EC0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [edi + 0x1c]
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_1E94
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_1E7A
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_1E7A
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_1E94
loc_2_1E7A:
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
loc_2_1E94:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_1EB3
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_1EB3
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_1EB3
        push   esi
        call   sub_1_1B40
loc_2_1EB3:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_1EC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x171], 0
        je     loc_2_1F0C
loc_2_1ED4:
        mov    ecx, dword ptr [esi + 0x171]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x171], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_2_1F00
        cmp    eax, 8
        je     loc_2_1F00
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_247C
loc_2_1F00:
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_1ED4
loc_2_1F0C:
        cmp    dword ptr [esi + 0x5b], -1
        je     loc_2_1F2A
        mov    dword ptr [esi + 0x143], 2
        dec    dword ptr [esi + 0x30]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1F2A:
        dec    dword ptr [esi + 0x30]
        push   esi
        call   sub_2_1F40
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1F40:
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
        je     loc_2_1F71
        lea    esi, [eax + 0x61]
loc_2_1F64:
        cmp    dword ptr [esi], 0
        jne    loc_2_1F80
        add    esi, 4
        inc    ecx
        cmp    edx, ecx
        ja     loc_2_1F64
loc_2_1F71:
        dec    dword ptr [eax + 0x30]
        push   eax
        call   sub_2_1FA0
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_1F80:
        mov    dword ptr [eax + 0x143], 3
        dec    dword ptr [eax + 0x30]
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1FA0:
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
        jbe    loc_2_1FF0
        lea    edi, [esi + 0x91]
loc_2_1FC2:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_2_1FE2
        mov    ebx, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_246A
        dec    dword ptr [ebx + 0x1c]
        mov    dword ptr [edi], 0
loc_2_1FE2:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0x38], eax
        ja     loc_2_1FC2
loc_2_1FF0:
        mov    edx, dword ptr [esi + 0x4b]
        or     edx, edx
        je     loc_2_201B
loc_2_1FF7:
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
        jne    loc_2_1FF7
loc_2_201B:
        mov    edx, dword ptr [esi + 0x165]
        or     edx, edx
        je     loc_2_204F
loc_2_2025:
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
        jne    loc_2_2025
loc_2_204F:
        cmp    byte ptr [esi + 0x121], 0
        je     loc_2_206B
loc_2_2058:
        push   0x2710
        call   sub_1_2520
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_2058
loc_2_206B:
        mov    byte ptr [esi + 0x160], 0x1b
        push   esi
        call   sub_2_0680
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x142], 0
        mov    edi, dword ptr [esi + 0x28]
        or     edi, edi
        je     loc_2_20D2
        sub    ebx, ebx
loc_2_2094:
        inc    dword ptr [edi + 0x1c]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_2_20AB
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_248E
loc_2_20AB:
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
        jne    loc_2_2094
loc_2_20D2:
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_20F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x143], eax
        jne    loc_2_21DF
        cmp    eax, 1
        je     loc_2_2172
        cmp    eax, 2
        je     loc_2_217A
        cmp    eax, 3
        je     loc_2_2182
        mov    eax, dword ptr [ebp - 4]
loc_2_211C:
        cmp    eax, 0x103
        je     loc_2_213B
        push   eax
        mov    eax, dword ptr [esi + 0x147]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_2476
        mov    eax, dword ptr [esi + 0x147]
        dec    dword ptr [eax + 0x1c]
loc_2_213B:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_21C0
        cmp    byte ptr [esi + 0x142], 0
        jne    loc_2_215C
        cmp    dword ptr [esi + 0x171], 0
        jne    loc_2_215C
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_2_21C0
loc_2_215C:
        push   1
        add    esi, 0x17d
        push   esi
        call   sub_1_2502
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_2172:
        push   esi
        call   sub_2_1EC0
        jmp    loc_2_211C
loc_2_217A:
        push   esi
        call   sub_2_1F40
        jmp    loc_2_211C
loc_2_2182:
        mov    ecx, dword ptr [esi + 0x30]
        inc    ecx
        mov    dword ptr [esi + 0x30], ecx
        sub    edx, edx
        mov    eax, dword ptr [esi + 0x38]
        cmp    eax, edx
        je     loc_2_21A2
        lea    edi, [esi + 0x61]
loc_2_2195:
        cmp    dword ptr [edi], 0
        jne    loc_2_21B1
        add    edi, 4
        inc    edx
        cmp    eax, edx
        ja     loc_2_2195
loc_2_21A2:
        dec    ecx
        mov    dword ptr [esi + 0x30], ecx
        push   esi
        call   sub_2_1FA0
        jmp    loc_2_211C
loc_2_21B1:
        dec    ecx
        mov    dword ptr [esi + 0x30], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_21C0:
        cmp    dword ptr [esi + 0x4b], 0
        je     loc_2_21DF
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x61], 0
        jne    loc_2_21DF
        cmp    byte ptr [esi + 0x121], 0
        jne    loc_2_21DF
        push   esi
        call   sub_1_1B40
loc_2_21DF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_21F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_2_2203
        lea    edx, [eax + 0x49]
loc_2_2203:
        cmp    byte ptr [ecx + 0x142], 0
        je     loc_2_2215
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_2_2215:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x171], 0
        jne    loc_2_2250
        mov    dword ptr [ecx + 0x175], edx
        mov    dword ptr [ecx + 0x171], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_2_2250:
        mov    eax, dword ptr [ecx + 0x175]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x175], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2270:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_2_2283
        lea    edx, [eax + 0x21]
loc_2_2283:
        cmp    byte ptr [ecx + 0x142], 0
        je     loc_2_2295
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_2_2295:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x171], 0
        jne    loc_2_22D0
        mov    dword ptr [ecx + 0x175], edx
        mov    dword ptr [ecx + 0x171], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_2_22D0:
        mov    eax, dword ptr [ecx + 0x175]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x175], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_22F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x1c]
        ret    4
        db  08Dh, 049h, 000h
sub_2_2300:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x14f]
        mov    edi, dword ptr [eax + 0xc]
        test   edi, 0x24
        je     loc_2_2330
        push   0x800                         ; -> loc_2_0800
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_251A
        jmp    loc_2_2335
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_2330:
        call   sub_2_2390
loc_2_2335:
        test   edi, 0x26
        lea    eax, [esi + 0x15f]
        je     loc_2_2348
        or     byte ptr [eax], 8
        jmp    loc_2_234B
loc_2_2348:
        and    byte ptr [eax], 0xf7
loc_2_234B:
        test   edi, 0x20
        je     loc_2_2358
        or     byte ptr [eax], 0x10
        jmp    loc_2_235B
loc_2_2358:
        and    byte ptr [eax], 0xef
loc_2_235B:
        test   edi, 0x28
        je     loc_2_2368
        or     byte ptr [eax], 4
        jmp    loc_2_236B
loc_2_2368:
        and    byte ptr [eax], 0xfb
loc_2_236B:
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
sub_2_2390:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0770
        push   esi
        push   0x840                         ; -> loc_2_0840
        add    esi, 8
        push   esi
        call   sub_1_251A
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_23C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x2930]       ; -> obj1:0x2930
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
        je     loc_2_23F4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_23F4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_240E
        mov    esi, 2
loc_2_240E:
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
        je     loc_2_2442
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_24E0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2442:
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
        je     loc_2_2471
        push   eax
        call   sub_2_24E0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2471:
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
sub_2_24E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_2500
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_252C
loc_2_2500:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_2516
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_252C
loc_2_2516:
        push   0
        push   0x32
        push   esi
        call   sub_1_252C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2530:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_2549
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2549:
        mov    eax, dword ptr [0x2930]       ; -> obj1:0x2930
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2526
        or     eax, eax
        je     loc_2_2569
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2569:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_2570:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_2583
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_2570
        jmp    loc_2_2592
loc_2_2583:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_2592:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_25A8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_25A8:
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
sub_2_2600:
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
        call   sub_2_29A0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_2629
        cmp    ebx, 0x103
        jne    loc_2_2640
loc_2_2629:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_26C0
        or     eax, eax
        je     loc_2_2640
        mov    ebx, eax
loc_2_2640:
        or     ebx, ebx
        je     loc_2_264C
        cmp    ebx, 0x103
        jne    loc_2_26AB
loc_2_264C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_26A6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_2662
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2662:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_2671
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_2677
loc_2_2671:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_2677:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_2686
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2488
loc_2_2686:
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
loc_2_26A6:
        mov    ebx, 0xc001000e
loc_2_26AB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_26C0:
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
        je     loc_2_2744
        sub    eax, eax
        nop    
loc_2_2720:
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
        ja     loc_2_2720
loc_2_2744:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_27AF
loc_2_2751:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_27A4
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
        jmp    loc_2_27A7
loc_2_27A4:
        inc    dword ptr [ebp - 4]
loc_2_27A7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_2751
loc_2_27AF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_289A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_27C3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2B80
        or     al, al
        je     loc_2_27FA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_2888
loc_2_27FA:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_28D4
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
loc_2_2888:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_27C3
loc_2_289A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_2925
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_28B0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_292B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_2912
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_2912
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_2919
loc_2_28D4:
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
loc_2_2912:
        mov    dword ptr [ebp - 8], 1
loc_2_2919:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_28B0
loc_2_2925:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_2990
loc_2_292B:
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
        je     loc_2_2992
        cmp    edi, 0x103
        je     loc_2_2992
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
loc_2_2990:
        sub    edi, edi
loc_2_2992:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_29A0:
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
        je     loc_2_29DD
        lea    esp, [esp]
loc_2_29D0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_29D0
loc_2_29DD:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_2A11
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
        je     loc_2_2A13
        cmp    eax, 0x103
        je     loc_2_2A13
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2A11:
        sub    eax, eax
loc_2_2A13:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_2A20:
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
        je     loc_2_2A52
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2A47:
        test   dword ptr [esi], edx
        je     loc_2_2A4C
        inc    eax
loc_2_2A4C:
        add    esi, 4
        dec    edi
        jne    loc_2_2A47
loc_2_2A52:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2A60:
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
        jbe    loc_2_2ADC
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_2AA0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_2ACA
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_2AF6
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_262C
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_2ACA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_2AA0
loc_2_2ADC:
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
loc_2_2AF6:
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
sub_2_2B20:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_2B50
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2B50:
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
sub_2_2B80:
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
        je     loc_2_2C19
        dec    esi
        cmp    esi, edx
        jb     loc_2_2C19
        nop    
loc_2_2BA0:
        cmp    edx, ecx
        jb     loc_2_2C19
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_2BC3
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2BF7
loc_2_2BC3:
        jbe    loc_2_2BD0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_2BF7
        db  08Bh, 0FFh
loc_2_2BD0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_2BE7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2BF7
loc_2_2BE7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_2BF7
        mov    dword ptr [ebp - 4], 0
loc_2_2BF7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2C29
        jle    loc_2_2C08
        or     edx, edx
        je     loc_2_2C19
        lea    esi, [edx - 1]
        jmp    loc_2_2C0B
loc_2_2C08:
        lea    ecx, [edx + 1]
loc_2_2C0B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_2BA0
loc_2_2C19:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2C29:
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
