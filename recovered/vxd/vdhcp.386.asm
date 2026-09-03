; VDHCP -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 6636 instructions recovered by recursive descent

Declare_Virtual_Device VDHCP, 3, 0, VDHCP_Control, 0049Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 05400B500h   flags 00000h
;   service table: obj1:0x298c, 4 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3064  R-X
sub_1_0000:
        VxDCall  Dev_0488.Service_8001h          ; cd2001808804
sub_1_0006:
        VxDCall  Dev_0488.Service_8002h          ; cd2002808804
sub_1_000C:
        VMMCall  VMM.Get_System_Time             ; cd203f800100
        VxDCall  Dev_0488.Service_8003h          ; cd2003808804
        VxDCall  Dev_0488.Service_8004h          ; cd2004808804
sub_1_001E:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_0024:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
sub_1_002A:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        VxDCall  Dev_0488.Service_8005h          ; cd2005808804
sub_1_0036:
        VxDCall  Dev_0488.Service_800Dh          ; cd200d808804
sub_1_003C:
        VxDCall  Dev_0488.Service_8006h          ; cd2006808804
        mov    eax, dword ptr [0x28c8]       ; -> obj1:0x28C8
        VxDCall  Dev_0488.Service_8009h          ; cd2009808804
loc_1_004D:
        mov    eax, dword ptr [0x28c8]       ; -> obj1:0x28C8
        VxDCall  Dev_0488.Service_800Ch          ; cd200c808804
loc_1_0058:
        mov    eax, dword ptr [0x28c8]       ; -> obj1:0x28C8
        VxDCall  Dev_0488.Service_800Bh          ; cd200b808804
        add    byte ptr [ebx + 0x56], dl
        push   edi
        push   ebp
        mov    bx, word ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        jne    loc_1_0080
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0080:
        mov    edx, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_0105
loc_1_008E:
        cmp    dword ptr [edx + 0x11], ecx
        je     loc_1_0098
        cmp    ecx, -1
        jne    loc_1_00FB
loc_1_0098:
        mov    edi, dword ptr [edx + 0x51]
        mov    esi, dword ptr [edi + 0x2a]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, -0x2a
        je     loc_1_00F6
        movzx  eax, bx
loc_1_00AA:
        movzx  ebp, byte ptr [esi + 8]
        cmp    ebp, eax
        je     loc_1_00C0
        mov    esi, dword ptr [esi]
        mov    ebp, edi
        sub    ebp, esi
        cmp    ebp, -0x2a
        jne    loc_1_00AA
        jmp    loc_1_00F6
        db  090h
loc_1_00C0:
        cmp    bx, 0x2b
        jne    loc_1_0110
        mov    ebx, dword ptr [esp + 0x18]
        shr    ebx, 8
        lea    edi, [esi + 0xa]
        movzx  esi, byte ptr [esi + 9]
        or     esi, esi
        je     loc_1_00F6
        movzx  ebp, bx
        nop    
loc_1_00DC:
        movzx  eax, byte ptr [edi]
        cmp    eax, ebp
        je     loc_1_00F2
        movzx  eax, byte ptr [edi + 1]
        sub    esi, eax
        lea    eax, [eax + eax*2]
        add    edi, eax
        or     esi, esi
        jne    loc_1_00DC
loc_1_00F2:
        or     esi, esi
        jne    loc_1_0134
loc_1_00F6:
        cmp    ecx, -1
        jne    loc_1_0158
loc_1_00FB:
        mov    edx, dword ptr [edx]
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_008E
loc_1_0105:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0110:
        add    esi, 0xa
        mov    ecx, dword ptr [esp + 0x20]
        push   ecx
        movzx  eax, byte ptr [esi - 1]
        push   eax
        push   esi
        push   dword ptr [ecx]
        push   dword ptr [esp + 0x2c]
        call   sub_4_0B3C
        add    esp, 0x14
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_0134:
        add    edi, 2
        mov    ecx, dword ptr [esp + 0x20]
        push   ecx
        movzx  eax, byte ptr [edi - 1]
        push   eax
        push   edi
        push   dword ptr [ecx]
        push   dword ptr [esp + 0x2c]
        call   sub_4_0B3C
        add    esp, 0x14
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_0158:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_0164:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, 1
        je     loc_1_0180
        cmp    eax, 2
        je     loc_1_0214
        mov    eax, 0x1a
        pop    edi
        pop    esi
        ret    
loc_1_0180:
        cmp    dword ptr [esp + 0x18], 8
        jne    loc_1_020C
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [esi], 0
        je     loc_1_020C
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        je     loc_1_01C4
        mov    edi, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    edi, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_01B9
loc_1_01AA:
        cmp    dword ptr [edi + 0x11], ecx
        je     loc_1_01C6
        mov    edi, dword ptr [edi]
        cmp    edi, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_01AA
loc_1_01B9:
        pop    edi
        mov    eax, 3
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_01C4:
        sub    edi, edi
loc_1_01C6:
        push   0x14
        call   sub_4_0CD0
        add    esp, 4
        or     eax, eax
        jne    loc_1_01DC
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
loc_1_01DC:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [eax + 0x10], edi
        mov    dword ptr [eax], 0x2b70       ; -> loc_1_2B70
        mov    ecx, dword ptr [0x2b74]       ; -> loc_1_2B74
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [0x2b74]       ; -> loc_1_2B74
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x2b74], eax       ; -> loc_1_2B74
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_020C:
        pop    edi
        mov    eax, 0x1a
        pop    esi
        ret    
loc_1_0214:
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        push   0x2b84                        ; -> loc_1_2B84
        call   sub_1_0364
        add    esp, 8
        or     eax, eax
        jne    loc_1_0234
        mov    eax, 0x1a
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0234:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x11], ecx
        push   dword ptr [esi + 0x18]
        lea    ecx, [esi + 0x1c]
        push   ecx
        push   dword ptr [esi + 0x14]
        push   eax
        call   sub_1_042C
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0258
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_0258:
        cmp    eax, 8
        jne    loc_1_0268
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_0268:
        pop    edi
        mov    eax, 0x1a
        pop    esi
        ret    
sub_1_0270:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [0x2b70]       ; -> loc_1_2B70
        cmp    ebp, 0x2b70                   ; -> loc_1_2B70
        je     loc_1_02B3
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_028E:
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_0299
        cmp    esi, eax
        jne    loc_1_02A8
loc_1_0299:
        push   dword ptr [esp + 0x20]
        push   ebx
        push   edi
        push   dword ptr [ebp + 0xc]
        call   dword ptr [ebp + 8]
        add    esp, 0x10
loc_1_02A8:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, 0x2b70                   ; -> loc_1_2B70
        jne    loc_1_028E
loc_1_02B3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_02B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0x51]
        mov    dword ptr [ebp - 0x14], 0x301
        mov    ecx, dword ptr [esi + 0xa]
        mov    dword ptr [ebp - 0x10], ecx
        mov    dword ptr [ebp - 0xc], 0x200
        mov    dword ptr [ebp - 8], 0x100
        cmp    dword ptr [ebp + 0xc], 0x101
        jne    loc_1_035C
        mov    dword ptr [ebp - 4], 0x101
        push   3
        push   eax
        call   sub_1_0814
        add    esp, 8
        or     eax, eax
        je     loc_1_035C
        mov    cl, byte ptr [eax + 9]
        shr    cl, 2
        movzx  ebx, cl
        lea    edi, [eax + 0xa]
        or     ebx, ebx
        jle    loc_1_035C
loc_1_0311:
        mov    dword ptr [ebp - 0x44], 0
        mov    dword ptr [ebp - 0x1c], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 0x2c], eax
        mov    dword ptr [ebp - 0x3c], 1
        mov    dword ptr [ebp - 0x28], 3
        mov    dword ptr [ebp - 0x24], 2
        mov    eax, dword ptr [esi + 6]
        mov    dword ptr [ebp - 0x40], eax
        push   0x30
        lea    eax, [ebp - 0x44]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [0x3054]       ; -> obj1:0x3054
        call   dword ptr [eax + 0x4c]
        add    esp, 0x10
        add    edi, 4
        dec    ebx
        jne    loc_1_0311
loc_1_035C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_0364:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [ebx]
        cmp    eax, ebx
        je     loc_1_039C
        mov    ebp, dword ptr [esp + 0x18]
loc_1_0376:
        mov    edx, dword ptr [eax + 0xd]
        cmp    dword ptr [ebp], edx
        jne    loc_1_0396
        lea    esi, [ebp + 4]
        mov    ecx, edx
        shr    ecx, 2
        mov    edi, dword ptr [eax + 9]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0394
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_0394:
        je     loc_1_039E
loc_1_0396:
        mov    eax, dword ptr [eax]
        cmp    eax, ebx
        jne    loc_1_0376
loc_1_039C:
        sub    eax, eax
loc_1_039E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_03A4:
        push   esi
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    edx, dword ptr [0x304c]       ; -> loc_1_304C
        cmp    edx, 0x304c                   ; -> loc_1_304C
        je     loc_1_03CC
loc_1_03BD:
        cmp    dword ptr [edx + 0x31], eax
        jl     loc_1_03DC
        mov    edx, dword ptr [edx]
        cmp    edx, 0x304c                   ; -> loc_1_304C
        jne    loc_1_03BD
loc_1_03CC:
        pop    esi
        mov    eax, dword ptr [0x2b80]       ; -> loc_1_2B80
        inc    dword ptr [0x2b80]            ; -> loc_1_2B80
        ret    
        db  08Dh, 049h, 000h
loc_1_03DC:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 0x51]
        mov    esi, dword ptr [eax + 0xe]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_4_0CC0
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_03FC:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        push   7
        push   0x28e0                        ; -> obj1:0x28E0
        push   0x37
        push   dword ptr [esp + 0x18]
        call   sub_1_12A8
        add    esp, 0x14
        push   esi
        push   4
        push   0x28e8                        ; -> obj1:0x28E8
        push   0x2b
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        pop    esi
        ret    
sub_1_042C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [eax + 0x51]
        push   dword ptr [esp + 0x18]
        push   eax
        call   sub_1_0814
        add    esp, 8
        or     eax, eax
        je     loc_1_048C
        movzx  ecx, byte ptr [eax + 9]
        mov    ebx, dword ptr [esp + 0x20]
        cmp    ecx, ebx
        jne    loc_1_0474
        lea    esi, [eax + 0xa]
        mov    ecx, ebx
        shr    ecx, 2
        mov    edi, dword ptr [esp + 0x1c]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0474
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_0474
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0474:
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_0024
        add    esp, 4
        jmp    loc_1_0490
        db  08Dh, 049h, 000h
loc_1_048C:
        mov    ebx, dword ptr [esp + 0x20]
loc_1_0490:
        mov    ax, bx
        add    ax, 0xa
        push   eax
        call   sub_4_06D0
        add    esp, 4
        or     eax, eax
        jne    loc_1_04B0
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_04B0:
        mov    byte ptr [eax + 9], bl
        mov    cl, byte ptr [esp + 0x18]
        mov    byte ptr [eax + 8], cl
        lea    edi, [eax + 0xa]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebp, 0x2a
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_04E8:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x51]
        mov    eax, dword ptr [esi + 0x2a]
        sub    eax, esi
        cmp    eax, 0x2a
        je     loc_1_051A
loc_1_04FA:
        mov    eax, dword ptr [esi + 0x2a]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 0x2a]
        sub    eax, esi
        cmp    eax, 0x2a
        jne    loc_1_04FA
loc_1_051A:
        pop    esi
        ret    
sub_1_051C:
        sub    eax, eax
        ret    
        db  090h
sub_1_0520:
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 8]
loc_1_0526:
        movzx  edx, byte ptr [ecx + 0x28e0]  ; -> obj1:0x28E0
        cmp    edx, eax
        je     loc_1_0540
        inc    ecx
        cmp    ecx, 7
        jb     loc_1_0526
        mov    eax, 0x57
        ret    
        db  08Dh, 049h, 000h
loc_1_0540:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x10]
        call   sub_1_042C
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh, 02Bh, 0C0h, 0C3h, 090h
sub_1_055C:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x51]
        cmp    dword ptr [ecx], -1
        je     loc_1_056C
        sub    eax, eax
        ret    
        db  090h
loc_1_056C:
        cmp    dword ptr [eax + 0x35], 0
        je     loc_1_0580
        mov    eax, dword ptr [eax + 0x11]
        push   eax
        push   ecx
        call   sub_1_27A4
        add    esp, 8
        ret    
loc_1_0580:
        sub    eax, eax
        push   eax
        push   ecx
        call   sub_1_27A4
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_0590:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x51]
        mov    eax, dword ptr [esi]
        cmp    eax, -1
        je     loc_1_05B0
        push   eax
        call   sub_1_08A0
        mov    dword ptr [esi], 0xffffffff
        call   sub_1_1178
loc_1_05B0:
        sub    eax, eax
        pop    esi
        ret    
sub_1_05B4:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x51]
        cmp    dword ptr [edi + 0x35], 0
        jne    loc_1_05EB
        push   dword ptr [edi + 0x15]
        push   dword ptr [edi + 0x11]
        movzx  eax, word ptr [esi + 4]
        push   eax
        call   sub_1_0F03
        add    esp, 0xc
        push   0x101
        push   edi
        call   sub_1_02B8
        add    esp, 8
        mov    dword ptr [edi + 0x35], 1
loc_1_05EB:
        cmp    dword ptr [esi], -1
        je     loc_1_05F8
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_05F8:
        push   dword ptr [edi + 0x11]
        push   esi
        call   sub_1_27A4
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_061A
        push   0
        push   0x3e8
        push   edi
        call   sub_1_0844
        add    esp, 0xc
loc_1_061A:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_0620:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x51]
        mov    eax, dword ptr [esi]
        cmp    eax, -1
        je     loc_1_063C
        push   eax
        call   sub_1_08A0
        mov    dword ptr [esi], 0xffffffff
loc_1_063C:
        cmp    dword ptr [edi + 0x35], 0
        je     loc_1_065A
        push   0
        push   0
        movzx  eax, word ptr [esi + 4]
        push   eax
        call   sub_1_0F03
        add    esp, 0xc
        mov    dword ptr [edi + 0x35], 0
loc_1_065A:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_0660:
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, -1
        mov    edx, dword ptr [esp + 4]
        jne    loc_1_067C
        jmp    loc_1_0685
        db  08Dh, 049h, 000h
loc_1_067C:
        add    ecx, eax
        mov    dword ptr [edx + 0x41], ecx
        cmp    ecx, eax
        jge    loc_1_068C
loc_1_0685:
        mov    dword ptr [edx + 0x41], 0x7fffffff
loc_1_068C:
        lea    ecx, [edx + 0x39]
        mov    dword ptr [ecx], 0x2b60       ; -> loc_1_2B60
        mov    eax, dword ptr [0x2b64]       ; -> loc_1_2B64
        mov    dword ptr [edx + 0x3d], eax
        mov    eax, dword ptr [0x2b64]       ; -> loc_1_2B64
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x2b64], ecx       ; -> loc_1_2B64
        push   0
        push   0
        call   sub_4_058C
        add    esp, 8
        ret    
        db  090h
sub_1_06B8:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esi + 0x11]
        mov    dword ptr [esp + 0x14], eax
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_1_071C
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_06DC
        mov    eax, dword ptr [esp + 0x24]
loc_1_06DC:
        mov    dword ptr [esi + 0x1d], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esi + 0x11], eax
        mov    edi, dword ptr [esp + 0x20]
        or     edi, edi
        je     loc_1_06FC
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     loc_1_06FC
        mov    eax, dword ptr [eax]
        jmp    loc_1_0708
        db  08Dh, 049h, 000h
loc_1_06FC:
        push   dword ptr [esp + 0x24]
        call   sub_1_1390
        add    esp, 4
loc_1_0708:
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x15], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_0720
loc_1_071C:
        mov    edi, dword ptr [esp + 0x20]
loc_1_0720:
        or     edi, edi
        je     loc_1_0738
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     loc_1_0738
        push   dword ptr [eax]
        call   sub_1_0E7C
        mov    ebp, eax
        jmp    loc_1_073D
        db  08Bh, 0FFh
loc_1_0738:
        mov    ebp, 0xe10
loc_1_073D:
        mov    dword ptr [esi + 0x21], ebp
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    ebx, eax
        mov    dword ptr [esi + 0x25], ebx
        or     edi, edi
        je     loc_1_0764
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_0764
        push   dword ptr [eax]
        call   sub_1_0E7C
        jmp    loc_1_0768
        db  090h
loc_1_0764:
        mov    eax, ebp
        shr    eax, 1
loc_1_0768:
        add    eax, ebx
        cmp    eax, ebx
        jge    loc_1_0773
        mov    eax, 0x7fffffff
loc_1_0773:
        or     edi, edi
        mov    dword ptr [esi + 0x29], eax
        je     loc_1_078C
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_078C
        push   dword ptr [eax]
        call   sub_1_0E7C
        jmp    loc_1_079C
        db  08Bh, 0FFh
loc_1_078C:
        sub    edx, edx
        lea    eax, [ebp*4]
        mov    ecx, 5
        div    ecx
loc_1_079C:
        add    eax, ebx
        cmp    eax, ebx
        jge    loc_1_07A7
        mov    eax, 0x7fffffff
loc_1_07A7:
        mov    dword ptr [esi + 0x2d], eax
        lea    eax, [ebx + ebp]
        cmp    eax, ebx
        jge    loc_1_07B6
        mov    eax, 0x7fffffff
loc_1_07B6:
        mov    dword ptr [esi + 0x31], eax
        push   0
        push   esi
        call   sub_4_10D4
        add    esp, 8
        mov    edi, eax
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_1_0809
        push   esi
        call   sub_1_0620
        add    esp, 4
        or     eax, eax
        jne    loc_1_080B
        push   esi
        call   sub_1_05B4
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_07F4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_07F4:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_0270
        add    esp, 0x10
loc_1_0809:
        mov    eax, edi
loc_1_080B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
sub_1_0814:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x51]
        mov    eax, dword ptr [esi + 0x2a]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, -0x2a
        je     loc_1_083F
        mov    edx, dword ptr [esp + 0xc]
loc_1_082C:
        movzx  ecx, byte ptr [eax + 8]
        cmp    ecx, edx
        je     loc_1_0841
        mov    eax, dword ptr [eax]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, -0x2a
        jne    loc_1_082C
loc_1_083F:
        sub    eax, eax
loc_1_0841:
        pop    esi
        ret    
        db  090h
sub_1_0844:
        ret    
        db  08Dh, 049h, 000h
sub_1_0848:
        push   edi
        cmp    dword ptr [0x3054], 0         ; -> obj1:0x3054
        jne    loc_1_0868
        mov    dword ptr [0x2b7c], 0x2742    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        pop    edi
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_0868:
        push   0x7a
        call   sub_4_0CD0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_1_088C
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x1e
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, edx
        pop    edi
        ret    0xc
        db  090h
loc_1_088C:
        pop    edi
        mov    dword ptr [0x2b7c], 0x2747    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        ret    0xc
        db  090h
sub_1_08A0:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 0x2e]
        or     eax, eax
        je     loc_1_08C8
        mov    ecx, dword ptr [0x3054]       ; -> obj1:0x3054
        or     ecx, ecx
        je     loc_1_08C8
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        call   dword ptr [ecx + 4]
        add    esp, 4
loc_1_08C8:
        push   esi
        call   sub_4_0CC0
        add    esp, 4
        pop    esi
        add    esp, 0x10
        ret    4
sub_1_08D8:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [0x3054]       ; -> obj1:0x3054
        or     edi, edi
        jne    loc_1_0904
        mov    dword ptr [0x2b7c], 0x2742    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    0xc
        db  08Bh, 0FFh
loc_1_0904:
        lea    edx, [esi + 0x18]
        mov    dword ptr [edx], 1
        mov    word ptr [esi + 0x1c], 0xe
        mov    word ptr [esi + 0x1e], 2
        mov    eax, dword ptr [esp + 0x20]
        mov    cx, word ptr [eax + 2]
        mov    word ptr [esi + 0x20], cx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x22], eax
        push   0x2980                        ; -> loc_1_2980
        push   0x11
        push   edx
        lea    eax, [esp + 0x14]
        push   eax
        call   dword ptr [edi]
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0958
        mov    dword ptr [0x2b7c], 0x2747    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    0xc
loc_1_0958:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x2e], eax
        push   0xc14                         ; -> loc_1_0C14
        push   4
        push   eax
        mov    eax, dword ptr [0x3054]       ; -> obj1:0x3054
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_0980:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0xc], 0
        mov    esi, dword ptr [esp + 0x24]
        mov    edx, dword ptr [0x3054]       ; -> obj1:0x3054
        or     edx, edx
        jne    loc_1_09B4
        mov    dword ptr [0x2b7c], 0x2742    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    0x18
loc_1_09B4:
        lea    edi, [esi + 0x32]
        mov    dword ptr [esi + 0x42], 0x4655424e
        mov    dword ptr [edi], 0
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [esi + 0x3e], ecx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 0x36], eax
        mov    dword ptr [esi + 0x3a], 0
        mov    dword ptr [esi + 0x18], 1
        mov    word ptr [esi + 0x1c], 0xe
        mov    word ptr [esi + 0x1e], 2
        mov    eax, dword ptr [esp + 0x34]
        mov    bx, word ptr [eax + 2]
        mov    word ptr [esi + 0x20], bx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x22], eax
        mov    dword ptr [esi + 0x10], 0x16
        lea    eax, [esi + 0x18]
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esp + 0x14], 0xe1c ; -> loc_1_0E1C
        mov    dword ptr [esp + 0x18], 0
        mov    eax, dword ptr [esi + 0x2e]
        mov    dword ptr [esp + 0x10], eax
        push   edi
        lea    eax, [esp + 0x10]
        push   eax
        push   ecx
        push   esi
        lea    eax, [esp + 0x20]
        push   eax
        call   dword ptr [edx + 0x30]
        add    esp, 0x14
        or     eax, eax
        je     loc_1_0A48
        cmp    eax, 1
        je     loc_1_0A58
        cmp    eax, 0xff
        je     loc_1_0A48
        jmp    loc_1_0A58
        db  090h
loc_1_0A48:
        mov    eax, dword ptr [esp + 0xc]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    0x18
        db  08Dh, 049h, 000h
loc_1_0A58:
        pop    edi
        mov    dword ptr [0x2b7c], 0x2747    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    0x18
sub_1_0A70:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [0x2dd8], 0         ; -> loc_1_2DD8
        je     loc_1_0A88
        mov    edx, 0x2dd8                   ; -> loc_1_2DD8
        jmp    loc_1_0A98
        db  090h
loc_1_0A88:
        mov    edx, 0x2b8c                   ; -> loc_1_2B8C
        cmp    dword ptr [0x2b8c], 0         ; -> loc_1_2B8C
        jne    loc_1_0A98
        sub    edx, edx
loc_1_0A98:
        or     edx, edx
        je     loc_1_0AD0
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, 0x240
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0x240
        lea    esi, [edx + 8]
        mov    edi, dword ptr [esp + 0x14]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [edx], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    0x18
        db  090h
loc_1_0AD0:
        lea    edi, [esi + 0x6e]
        mov    dword ptr [edi], 1
        mov    dword ptr [esi + 0x72], 0
        mov    dword ptr [esi + 0x76], 0
        mov    dword ptr [esi + 0x5a], 0
        mov    dword ptr [esi + 0x62], 0
        mov    eax, dword ptr [0x28c8]       ; -> obj1:0x28C8
        mov    dword ptr [esi + 0x5e], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x66], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x6a], eax
        and    dword ptr [esi + 0x4a], 0xfffffffd
        lea    ebx, [esi + 0x4e]
        push   esi
        push   0xe20                         ; -> loc_1_0E20
        push   dword ptr [0x2b30]            ; -> loc_1_2B30
        push   ebx
        call   0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0B40
        mov    dword ptr [0x2b7c], 0x2747    ; -> loc_1_2B7C
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        ret    0x18
loc_1_0B40:
        push   edi
        mov    dword ptr [esi + 0x46], edi
        mov    dword ptr [0x2b34], edi       ; -> loc_1_2B34
        mov    dword ptr [0x2b68], ebx       ; -> loc_1_2B68
        call   sub_1_0036
        add    esp, 4
        mov    dword ptr [0x2b68], 0         ; -> loc_1_2B68
        mov    dword ptr [0x2b34], 0         ; -> loc_1_2B34
        mov    eax, dword ptr [edi]
        or     eax, eax
        jne    loc_1_0B81
        mov    dword ptr [0x2b7c], 0x79      ; -> loc_1_2B7C
        mov    eax, 0xffffffff
loc_1_0B81:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x18
        db  090h
sub_1_0B88:
        cmp    dword ptr [0x28cc], 0         ; -> loc_1_28CC
        je     loc_1_0BB4
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        shl    ecx, 3
        add    ecx, dword ptr [eax + 4]
        mov    dword ptr [0x2b30], ecx       ; -> loc_1_2B30
        mov    eax, 1
        ret    0x14
loc_1_0BB4:
        mov    eax, 1
        mov    dword ptr [0x2b30], 0x3e8     ; -> loc_1_2B30
        ret    0x14
        db  08Bh, 0FFh
sub_1_0BC8:
        mov    eax, dword ptr [0x2b7c]       ; -> loc_1_2B7C
        ret    
        db  08Bh, 0FFh
sub_1_0BD0:
        sub    eax, eax
        ret    0x14
        db  08Dh, 049h, 000h
sub_1_0BD8:
        mov    eax, dword ptr [0x2b68]       ; -> loc_1_2B68
        or     eax, eax
        je     loc_1_0BF4
        push   eax
        call   6
        add    esp, 4
        mov    dword ptr [0x2b68], 0         ; -> loc_1_2B68
loc_1_0BF4:
        mov    eax, dword ptr [0x2b34]       ; -> loc_1_2B34
        or     eax, eax
        je     loc_1_0C12
        push   0
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    dword ptr [0x2b34], 0         ; -> loc_1_2B34
loc_1_0C12:
        ret    
        db  090h
loc_1_0C14:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x38]
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], 0
        cmp    dword ptr [esp + 0x14], 0x16
        jb     loc_1_0CF0
        push   0x43
        call   sub_1_0E70
        mov    ecx, dword ptr [esp + 0x18]
        cmp    ax, word ptr [ecx + 8]
        jne    loc_1_0CF0
        mov    eax, dword ptr [esp + 0x2c]
        cmp    eax, 0xef
        jb     loc_1_0CF0
        cmp    dword ptr [0x2b8c], 0         ; -> loc_1_2B8C
        je     loc_1_0C7E
        mov    esi, 0x2b8c                   ; -> loc_1_2B8C
        mov    edi, 0x2dd8                   ; -> loc_1_2DD8
        mov    ecx, 0x92
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x2b8c], 0         ; -> loc_1_2B8C
loc_1_0C7E:
        mov    dword ptr [0x2b5c], 0x4655424e ; -> loc_1_2B5C
        mov    dword ptr [0x2b4c], 0         ; -> loc_1_2B4C
        mov    dword ptr [0x2b58], 0x240     ; -> loc_1_2B58
        mov    dword ptr [0x2b50], 0x2b94    ; -> loc_1_2B50
        mov    dword ptr [0x2b54], 0         ; -> loc_1_2B54
        mov    dword ptr [0x3028], 0xcfc     ; -> loc_1_3028
        mov    dword ptr [0x3024], eax       ; -> loc_1_3024
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x302c], eax       ; -> obj1:0x302C
        mov    dword ptr [0x3020], 0x2b4c    ; -> obj1:0x3020
        mov    dword ptr [0x3030], 0         ; -> obj1:0x3030
        mov    dword ptr [ebx], 0x3020       ; -> obj1:0x3020
        or     dword ptr [eax + 0x4a], 1
        mov    eax, 0x18
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0CF0:
        pop    edi
        mov    eax, 0x17
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_0CFC:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x2c]
        and    dword ptr [ebx + 0x4a], 0xfffffffe
        mov    eax, dword ptr [esp + 0x30]
        or     eax, eax
        je     loc_1_0D40
        mov    dword ptr [0x2b7c], eax       ; -> loc_1_2B7C
        mov    eax, dword ptr [ebx + 0x46]
        or     eax, eax
        je     loc_1_0E12
        push   0
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    dword ptr [ebx + 0x46], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_0D40:
        mov    esi, 0x2bb0                   ; -> loc_1_2BB0
        movzx  eax, byte ptr [0x2b96]        ; -> loc_1_2B96
        mov    dword ptr [esp + 0x14], eax
        movzx  eax, byte ptr [0x2b96]        ; -> loc_1_2B96
        lea    edi, [esp + 0x18]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x14]
        push   eax
        push   0x2b84                        ; -> loc_1_2B84
        call   sub_1_0364
        add    esp, 8
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_0DDC
        mov    dword ptr [0x2b8c], 1         ; -> loc_1_2B8C
        mov    ebp, dword ptr [esp + 0x34]
        mov    dword ptr [0x2b90], ebp       ; -> loc_1_2B90
        cmp    dword ptr [ebx + 0x46], 0
        je     loc_1_0DE0
        cmp    dword ptr [0x2dd8], 0         ; -> loc_1_2DD8
        mov    eax, 0x2dd8                   ; -> loc_1_2DD8
        jne    loc_1_0DB0
        mov    eax, 0x2b8c                   ; -> loc_1_2B8C
loc_1_0DB0:
        mov    ebp, dword ptr [eax + 4]
        lea    esi, [eax + 8]
        mov    edi, dword ptr [ebx + 0x66]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [eax], 0
        lea    eax, [ebx + 0x4e]
        push   eax
        call   6
        add    esp, 4
        jmp    loc_1_0DE0
        db  090h
loc_1_0DDC:
        mov    ebp, dword ptr [esp + 0x34]
loc_1_0DE0:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_0DEF
        test   byte ptr [ebx + 0x4a], 2
        je     loc_1_0DEF
        sub    ebp, ebp
loc_1_0DEF:
        mov    eax, dword ptr [ebx + 0x46]
        or     eax, eax
        je     loc_1_0E12
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_0E01
        or     ebp, ebp
        jne    loc_1_0E12
loc_1_0E01:
        push   ebp
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    dword ptr [ebx + 0x46], 0
loc_1_0E12:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_0E1C:
        ret    
        db  08Dh, 049h, 000h
loc_1_0E20:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 0x4a]
        test   cl, 1
        je     loc_1_0E34
        or     ecx, 2
        mov    dword ptr [eax + 0x4a], ecx
        ret    
        db  090h
loc_1_0E34:
        push   0
        push   0x15
        push   eax
        call   sub_1_0CFC
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
sub_1_0E44:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        and    eax, 0xff00
        mov    ecx, edx
        shl    ecx, 0x10
        or     eax, ecx
        shl    eax, 8
        mov    ecx, edx
        and    ecx, 0xff0000
        shr    edx, 0x10
        or     ecx, edx
        shr    ecx, 8
        or     eax, ecx
        ret    4
        db  08Bh, 0FFh
sub_1_0E70:
        mov    cx, word ptr [esp + 4]
        mov    ah, cl
        mov    al, ch
        ret    4
sub_1_0E7C:
        push   dword ptr [esp + 4]
        call   sub_1_0E44
        ret    4
        db  0FFh, 074h, 024h, 004h, 0E8h, 0DFh, 0FFh, 0FFh, 0FFh, 0C2h, 004h, 000h
VDHCP_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  edi, word ptr [ebp + 0x1c]
        movzx  eax, word ptr [ebp + 0x10]
        push   eax
        movzx  eax, word ptr [ebp + 0x38]
        push   eax
        push   ebx
        call   sub_1_1188
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0EF9
        or     eax, eax
        je     loc_1_0ECD
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   eax
        call   sub_1_11D0
        add    esp, 8
        or     eax, eax
        cmp    eax, -1
        je     loc_1_0EF9
loc_1_0ECD:
        mov    esi, eax
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   esi
        push   edi
        call   sub_1_16B8
        add    esp, 0xc
        mov    word ptr [ebp + 0x1c], ax
        or     esi, esi
        je     loc_1_0EF4
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   esi
        call   sub_1_1217
        add    esp, 8
loc_1_0EF4:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0EF9:
        cmp    eax, -1
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_1_0EF4
loc_1_0F02:
        ret    
sub_1_0F03:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDCall  Dev_0489.Service_0004h          ; cd2004008904
        add    esp, 0xc
        pop    ebp
        ret    
sub_1_0F1D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [0x2b78], 0         ; -> loc_1_2B78
        push   0
        push   dword ptr [ebp + 8]
        push   0xf45                         ; -> loc_1_0F45
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
loc_1_0F45:
        push   ebp
        mov    ebp, esp
        pushfd 
        pushal 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x10001034
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [0x297c]       ; -> loc_1_297C
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        call   sub_1_0F6D
        popal  
        popfd  
        pop    ebp
        ret    
sub_1_0F6D:
        cmp    eax, 7
        je     loc_1_0F7C
        mov    dword ptr [0x2b78], 1         ; -> loc_1_2B78
loc_1_0F7C:
        call   sub_4_1100
        ret    
VDHCP_Service_00:
        mov    ax, 0x100
        clc    
        ret    
VDHCP_Service_01:
        jmp    0x64
VDHCP_Service_02:
        jmp    loc_1_0164
VDHCP_Service_03:
        jmp    loc_4_06FC
VDHCP_Control:
        cmp    eax, 1
        je     loc_2_04F4
        cmp    eax, 0x1b
        je     loc_2_04F4
        cmp    eax, 0x23
        je     loc_1_100D
        cmp    eax, 4
        je     loc_1_0F02
        clc    
        ret    
        db  0CDh, 020h, 011h, 001h, 001h, 000h, 03Dh, 000h, 000h, 000h, 020h, 0B8h, 000h, 000h, 000h, 000h
        db  076h, 01Ch, 053h, 0CDh, 020h, 004h, 000h, 015h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 0F7h
        db  003h, 020h, 000h, 000h, 000h, 075h, 003h, 003h, 043h, 004h, 00Fh, 0B7h, 000h, 05Bh, 0C3h
sub_1_0FE8:
        push   edi
        push   esi
        mov    eax, 0
        mov    ebx, 0
        mov    ecx, 0x40
        mov    edx, 0
        mov    esi, dword ptr [esp + 0xc]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    eax, esi
        pop    esi
        pop    edi
        ret    
loc_1_100D:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 1
        jb     loc_1_105A
        cmp    ecx, 3
        ja     loc_1_105A
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_1034
        push   dword ptr [esi + 0x14]
        push   eax
        call   sub_1_11D0
        add    esp, 8
        or     eax, eax
        je     loc_1_1078
loc_1_1034:
        mov    dword ptr [esi + 0x10], eax
        mov    ecx, dword ptr [esi + 0x14]
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        call   sub_1_16B8
        add    esp, 0xc
        push   eax
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_1_1217
        add    esp, 8
        pop    eax
        jmp    loc_1_107F
loc_1_105A:
        cmp    ecx, 0
        je     loc_1_106B
        cmp    ecx, -1
        je     loc_1_1074
        mov    eax, 1
        jmp    loc_1_107F
loc_1_106B:
        xor    eax, eax
        mov    ecx, 0
        jmp    loc_1_107F
loc_1_1074:
        xor    eax, eax
        jmp    loc_1_107F
loc_1_1078:
        mov    eax, 0x6f8
        jmp    loc_1_107F
loc_1_107F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1083:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        cmp    ebx, 0x44c
        jne    loc_1_10A2
        movzx  eax, word ptr [0xc]           ; -> loc_3_000C
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_10A2:
        cmp    ebx, 0x44d
        jne    loc_1_10B9
        movzx  eax, word ptr [0xe]           ; -> loc_3_000E
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_10B9:
        cmp    ebx, 0x44e
        jne    loc_1_10D0
        movzx  eax, word ptr [0x10]          ; -> obj3:0x0010
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_10D0:
        cmp    ebx, 0x44f
        jne    loc_1_10E7
        movzx  eax, word ptr [0x12]          ; -> loc_3_0012
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_10E7:
        cmp    ebx, 0x450
        jne    loc_1_10FE
        movzx  eax, word ptr [0x14]          ; -> loc_3_0014
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_10FE:
        cmp    ebx, 0x451
        jne    loc_1_1115
        movzx  eax, word ptr [0x16]          ; -> obj3:0x0016
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_1115:
        cmp    ebx, 0x452
        jne    loc_1_112C
        movzx  eax, word ptr [0x18]          ; -> loc_3_0018
        add    eax, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_112E
loc_1_112C:
        xor    eax, eax
loc_1_112E:
        pop    ebx
        ret    
sub_1_1130:
        mov    eax, 0x29f0                   ; -> loc_1_29F0
        push   eax
        VxDCall  Dev_0488.Service_0008h          ; cd2008008804
        add    esp, 4
        cmp    eax, 0
        jne    loc_1_114A
        mov    eax, 1
        ret    
loc_1_114A:
        mov    dword ptr [0x3054], eax       ; -> obj1:0x3054
        mov    eax, 0
        ret    
loc_1_1155:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        VxDCall  Dev_0489.Service_0007h          ; cd2007008904
        add    esp, 8
        ret    
sub_1_1169:
        mov    eax, dword ptr [esp + 4]
        push   eax
        VxDCall  Dev_0489.Service_0008h          ; cd2008008904
        add    esp, 4
        ret    
sub_1_1178:
        mov    eax, 0xffffffff
        push   eax
        VxDCall  Dev_0489.Service_0008h          ; cd2008008904
        add    esp, 4
        ret    
sub_1_1188:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 0x14]
        movzx  eax, word ptr [esp + 0x18]
        movzx  esi, word ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_11C4
        test   dword ptr [ebx], 0x20
        je     loc_1_11C9
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_11C4
        add    eax, esi
        cmp    eax, 0x110000
        jae    loc_1_11C4
loc_1_11C1:
        add    eax, dword ptr [ebx + 4]
loc_1_11C4:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_11C9:
        shl    eax, 4
        add    eax, esi
        jmp    loc_1_11C1
sub_1_11D0:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_120F
        or     ebx, ebx
        je     loc_1_120F
        movzx  ecx, ax
        and    ch, 0xf
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1213
        add    eax, esi
loc_1_120F:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_1213:
        xor    eax, eax
        jmp    loc_1_120F
sub_1_1217:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_1252
        or     ebx, ebx
        je     loc_1_1252
        movzx  ecx, ax
        and    ch, 0xf
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_125B
loc_1_1252:
        mov    eax, 1
loc_1_1257:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_125B:
        xor    eax, eax
        jmp    loc_1_1257
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_12A8:
        push   ebx
        push   esi
        push   edi
        mov    cl, byte ptr [esp + 0x14]
        cmp    cl, 0xff
        jne    loc_1_12C0
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0xff
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_12C0:
        or     cl, cl
        jne    loc_1_12E0
        mov    eax, dword ptr [esp + 0x10]
        lea    ecx, [eax + 1]
        mov    edi, dword ptr [esp + 0x20]
        dec    edi
        cmp    edi, ecx
        jbe    loc_1_131C
        mov    byte ptr [eax], 0
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_12E0:
        mov    bl, byte ptr [esp + 0x1c]
        movzx  edx, bl
        mov    eax, dword ptr [esp + 0x10]
        lea    esi, [edx + eax]
        add    esi, 2
        mov    edi, dword ptr [esp + 0x20]
        dec    edi
        cmp    esi, edi
        jae    loc_1_131C
        mov    byte ptr [eax], cl
        mov    byte ptr [eax + 1], bl
        lea    edi, [eax + 2]
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  ecx, byte ptr [eax + 1]
        lea    eax, [ecx + eax + 2]
loc_1_131C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1320:
        sub    esp, 0x14
        push   esi
        push   edi
        mov    al, byte ptr [esp + 0x24]
        mov    byte ptr [esp + 8], al
        mov    dl, byte ptr [esp + 0x2c]
        movzx  eax, dl
        lea    edi, [esp + 9]
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [esp + 0x30]
        inc    dl
        push   edx
        lea    eax, [esp + 0x10]
        push   eax
        push   0x3d
        push   dword ptr [esp + 0x30]
        call   sub_1_12A8
        add    esp, 0x14
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
sub_1_136C:
        mov    eax, dword ptr [esp + 4]
        lea    edx, [eax + 4]
        mov    ecx, dword ptr [esp + 8]
        dec    ecx
        cmp    edx, ecx
        jae    loc_1_138C
        mov    byte ptr [eax], 0x63
        inc    eax
        mov    byte ptr [eax], 0x82
        inc    eax
        mov    byte ptr [eax], 0x53
        inc    eax
        mov    byte ptr [eax], 0x63
        inc    eax
loc_1_138C:
        ret    
        db  08Dh, 049h, 000h
sub_1_1390:
        mov    eax, dword ptr [esp + 4]
        test   eax, 0x80000000
        jne    loc_1_13A4
        mov    eax, 0xff000000
        ret    
        db  08Dh, 049h, 000h
loc_1_13A4:
        mov    ecx, eax
        and    ecx, 0xc0000000
        cmp    ecx, 0x80000000
        jne    loc_1_13BC
        mov    eax, 0xffff0000
        ret    
        db  08Bh, 0FFh
loc_1_13BC:
        and    eax, 0xe0000000
        cmp    eax, 0xc0000000
        jne    loc_1_13D0
        mov    eax, 0xffffff00
        ret    
        db  08Bh, 0FFh
loc_1_13D0:
        mov    eax, 0xffffffff
        ret    
        db  08Bh, 0FFh
sub_1_13D8:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        cmp    dword ptr [esp + 0x28], esi
        jne    loc_1_13F4
        mov    eax, 0x6f
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_13F4:
        mov    edi, 4
        mov    ebx, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    ebx, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_1465
        nop    
loc_1_1408:
        inc    esi
        mov    eax, dword ptr [ebx + 0xd]
        add    eax, 0x2c
        add    edi, eax
        mov    dword ptr [esp + 0x18], 0
        lea    eax, [esp + 0x18]
        push   eax
        push   0
        push   6
        push   dword ptr [ebx + 0x11]
        call   0x64
        add    esp, 0x10
        cmp    eax, 0x21
        jne    loc_1_1436
        add    edi, dword ptr [esp + 0x18]
loc_1_1436:
        lea    eax, [esp + 0x18]
        mov    dword ptr [esp + 0x18], 0
        push   eax
        push   0
        push   0xf
        push   dword ptr [ebx + 0x11]
        call   0x64
        add    esp, 0x10
        cmp    eax, 0x21
        jne    loc_1_145B
        add    edi, dword ptr [esp + 0x18]
loc_1_145B:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_1408
loc_1_1465:
        cmp    edi, dword ptr [esp + 0x28]
        jbe    loc_1_1488
        cmp    dword ptr [esp + 0x28], 4
        jb     loc_1_1478
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax], edi
loc_1_1478:
        pop    ebp
        mov    eax, 0x6f
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_1488:
        lea    ebx, [esi + esi*4]
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax], esi
        add    eax, 4
        mov    dword ptr [esp + 0x1c], eax
        lea    ebx, [ebx + ebx*8]
        sub    ebx, esi
        add    ebx, 4
        mov    eax, dword ptr [0x2b84]       ; -> loc_1_2B84
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_15C7
loc_1_14B4:
        mov    ebp, dword ptr [esp + 0x1c]
        add    dword ptr [esp + 0x1c], 0x2c
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [eax + 0xd]
        mov    dword ptr [ebp + 4], edx
        mov    eax, dword ptr [eax + 0x11]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x21]
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x25]
        mov    dword ptr [ebp + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x31]
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x19]
        mov    dword ptr [ebp + 0x18], eax
        mov    dword ptr [ebp], ebx
        mov    edi, dword ptr [esp + 0x24]
        add    edi, ebx
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 9]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        add    ebx, dword ptr [eax + 0xd]
        mov    eax, dword ptr [esp + 0x28]
        sub    eax, ebx
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        add    eax, ebx
        push   eax
        push   6
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax + 0x11]
        call   0x64
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_1558
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp + 0x1c], eax
        mov    dword ptr [ebp + 0x20], ebx
        add    ebx, dword ptr [esp + 0x14]
        jmp    loc_1_1566
        db  08Dh, 049h, 000h
loc_1_1558:
        mov    dword ptr [ebp + 0x1c], 0
        mov    dword ptr [ebp + 0x20], 0
loc_1_1566:
        mov    eax, dword ptr [esp + 0x28]
        sub    eax, ebx
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        add    eax, ebx
        push   eax
        push   0xf
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax + 0x11]
        call   0x64
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_15A4
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp + 0x24], eax
        mov    dword ptr [ebp + 0x28], ebx
        add    ebx, dword ptr [esp + 0x14]
        jmp    loc_1_15B2
        db  08Dh, 049h, 000h
loc_1_15A4:
        mov    dword ptr [ebp + 0x24], 0
        mov    dword ptr [ebp + 0x28], 0
loc_1_15B2:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_14B4
loc_1_15C7:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_15D4:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_1614
        mov    ebx, dword ptr [esp + 0x10]
loc_1_15E9:
        mov    eax, dword ptr [ebx + 4]
        cmp    dword ptr [edx + 0xd], eax
        jne    loc_1_160A
        mov    edi, dword ptr [ebx]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [edx + 9]
        add    edi, ebx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_1608
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_1608:
        je     loc_1_1620
loc_1_160A:
        mov    edx, dword ptr [edx]
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_15E9
loc_1_1614:
        pop    edi
        mov    eax, 3
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_1620:
        mov    ecx, dword ptr [edx + 0x39]
        mov    eax, dword ptr [edx + 0x3d]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [edx + 0x11], 0
        push   0
        push   edx
        jne    loc_1_1640
        call   sub_1_21B8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1640:
        call   sub_1_22A4
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_164C:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    ebx, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_169D
        mov    edx, dword ptr [esp + 0x10]
loc_1_1661:
        mov    eax, dword ptr [edx + 4]
        cmp    dword ptr [ebx + 0xd], eax
        jne    loc_1_1693
        mov    edi, dword ptr [edx]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebx + 9]
        add    edi, edx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_1693
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_1693
        mov    ecx, dword ptr [ebx + 0x39]
        mov    eax, dword ptr [ebx + 0x3d]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [ebx + 0x11], 0
        jne    loc_1_16A8
loc_1_1693:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_1661
loc_1_169D:
        pop    edi
        mov    eax, 3
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_16A8:
        push   ebx
        call   sub_1_2180
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_16B8:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        je     loc_1_16D4
        cmp    eax, 2
        je     loc_1_16E8
        cmp    eax, 3
        je     loc_1_16FC
        mov    eax, 0x57
        ret    
        db  08Dh, 049h, 000h
loc_1_16D4:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_13D8
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_16E8:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_15D4
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_16FC:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_164C
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_1710:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x50000000
        jne    loc_1_1775
        call   sub_1_1780
        or     esi, esi
        je     loc_1_1775
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 4], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_1753
        call   sub_1_17AA
loc_1_174C:
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_1_1753:
        push   esi
        mov    edx, esi
        mov    eax, 0x1000
        mov    ecx, 0
        mov    esi, 0x1798                   ; -> loc_1_1798
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        test   esi, esi
        pop    esi
        jne    loc_1_174C
        call   sub_1_1791
loc_1_1775:
        push   dword ptr [ebp + 0xc]
        call   dword ptr [ebp + 8]
        add    esp, 4
        jmp    loc_1_174C
sub_1_1780:
        mov    esi, dword ptr [0x2b20]       ; -> loc_1_2B20
        mov    dword ptr [0x2b20], 0         ; -> loc_1_2B20
        ret    
sub_1_1791:
        mov    dword ptr [0x2b20], esi       ; -> loc_1_2B20
        ret    
loc_1_1798:
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    esi, edx
        call   sub_1_17AA
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
sub_1_17AA:
        push   esi
        mov    ecx, 0x1000
        mov    edx, esi
        mov    ebx, 0x17c1                   ; -> loc_1_17C1
        mov    esi, ebx
        VxDCall  NDIS.Service_0013h              ; cd2013002a00
        pop    esi
        ret    
loc_1_17C1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [esi]
        add    esp, 4
        call   sub_1_1791
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_17E0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax + eax*4]
        lea    esi, [esi + esi*4]
        lea    esi, [esi + esi*4]
        shl    esi, 5
        call   sub_4_0BD4
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 4
        mov    ecx, 0x7fff
        sub    edx, edx
        div    ecx
        lea    eax, [eax + esi - 1]
        push   eax
        call   sub_4_0C28
        add    esp, 4
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_181C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x10], esi
        mov    ebx, dword ptr [esp + 0x20]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        push   dword ptr [esp + 0x18]
        call   sub_1_051C
        add    esp, 4
        mov    ebp, dword ptr [esp + 0x24]
        or     ebp, ebp
        je     loc_1_193D
        cmp    byte ptr [esi], 0x63
        jne    loc_1_193D
        cmp    byte ptr [esi + 1], 0x82
        jne    loc_1_193D
        cmp    byte ptr [esi + 2], 0x53
        jne    loc_1_193D
        cmp    byte ptr [esi + 3], 0x63
        jne    loc_1_193D
        add    esi, 4
        cmp    byte ptr [esi], 0xff
        je     loc_1_193D
loc_1_1885:
        mov    al, byte ptr [esi]
        or     al, al
        je     loc_1_189C
        cmp    al, 0xff
        je     loc_1_189C
        lea    edx, [esi + 1]
        movzx  ecx, byte ptr [edx]
        lea    edi, [ecx + esi + 2]
        jmp    loc_1_18A1
        db  090h
loc_1_189C:
        lea    edx, [esi + 1]
        mov    edi, edx
loc_1_18A1:
        mov    ecx, edi
        sub    ecx, dword ptr [esp + 0x10]
        cmp    ecx, ebp
        jg     loc_1_193D
        movzx  ecx, al
        cmp    ecx, 0x36
        jg     loc_1_18CC
        je     loc_1_191C
        cmp    ecx, 1
        je     loc_1_1904
        cmp    ecx, 0x33
        je     loc_1_190C
        cmp    ecx, 0x35
        je     loc_1_1914
        jmp    loc_1_18D6
        db  08Bh, 0FFh
loc_1_18CC:
        cmp    ecx, 0x3a
        je     loc_1_1924
        cmp    ecx, 0x3b
        je     loc_1_192C
loc_1_18D6:
        movzx  eax, byte ptr [edx]
        push   eax
        lea    eax, [esi + 2]
        push   eax
        push   ecx
        push   dword ptr [esp + 0x24]
        call   sub_1_0520
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1932
        push   esi
        push   0x3ea
        push   0x2b24                        ; -> loc_1_2B24
        call   sub_4_0CE0
        add    esp, 0xc
        jmp    loc_1_1932
loc_1_1904:
        add    esi, 2
        mov    dword ptr [ebx + 4], esi
        jmp    loc_1_1932
loc_1_190C:
        add    esi, 2
        mov    dword ptr [ebx + 8], esi
        jmp    loc_1_1932
loc_1_1914:
        add    esi, 2
        mov    dword ptr [ebx], esi
        jmp    loc_1_1932
        db  090h
loc_1_191C:
        add    esi, 2
        mov    dword ptr [ebx + 0xc], esi
        jmp    loc_1_1932
loc_1_1924:
        add    esi, 2
        mov    dword ptr [ebx + 0x10], esi
        jmp    loc_1_1932
loc_1_192C:
        add    esi, 2
        mov    dword ptr [ebx + 0x14], esi
loc_1_1932:
        mov    esi, edi
        cmp    byte ptr [edi], 0xff
        jne    loc_1_1885
loc_1_193D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_1948:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [edx + 0x4d]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0x89
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [ebx], 1
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [edx + 0x45]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [edx + 0xd]
        lea    edi, [ebx + 0x1c]
        mov    esi, dword ptr [edx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, byte ptr [edx + 0xd]
        mov    byte ptr [ebx + 2], al
        lea    eax, [ebx + 0xec]
        add    ebx, 0x224
        push   ebx
        push   eax
        call   sub_1_136C
        add    esp, 8
        mov    byte ptr [esp + 0xf], 1
        push   ebx
        push   1
        lea    ecx, [esp + 0x17]
        push   ecx
        push   0x35
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_19C4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_1948
        add    esp, 4
        mov    ebx, dword ptr [esi + 0x4d]
        add    ebx, 0x224
        push   ebx
        push   dword ptr [esi + 0xd]
        push   dword ptr [esi + 9]
        mov    cl, byte ptr [esi + 8]
        push   ecx
        push   eax
        call   sub_1_1320
        add    esp, 0x14
        mov    edx, eax
        lea    eax, [esi + 0x1d]
        cmp    dword ptr [eax], 0
        je     loc_1_1A0D
        push   ebx
        push   4
        push   eax
        push   0x32
        push   edx
        call   sub_1_12A8
        add    esp, 0x14
        mov    edx, eax
loc_1_1A0D:
        mov    ebp, dword ptr [0x3058]       ; -> obj1:0x3058
        or     ebp, ebp
        je     loc_1_1A34
        push   ebx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   ebp
        push   0xc
        push   edx
        call   sub_1_12A8
        add    esp, 0x14
        mov    edx, eax
loc_1_1A34:
        push   ebx
        push   0
        push   0
        push   0xff
        push   edx
        call   sub_1_12A8
        add    esp, 0x14
        sub    eax, dword ptr [esi + 0x4d]
        push   dword ptr [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_24B8
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_1A60:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [edx + 0x4d]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0x89
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [ebx], 1
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [edx + 0x45]
        cmp    dword ptr [esp + 0x18], 0
        mov    word ptr [ebx + 8], ax
        je     loc_1_1A95
        mov    eax, dword ptr [edx + 0x1d]
        mov    dword ptr [ebx + 0xc], eax
loc_1_1A95:
        mov    eax, dword ptr [edx + 0xd]
        lea    edi, [ebx + 0x1c]
        mov    esi, dword ptr [edx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, byte ptr [edx + 0xd]
        mov    byte ptr [ebx + 2], al
        lea    eax, [ebx + 0xec]
        add    ebx, 0x224
        push   ebx
        push   eax
        call   sub_1_136C
        add    esp, 8
        mov    byte ptr [esp + 0xf], 3
        push   ebx
        push   1
        lea    ecx, [esp + 0x17]
        push   ecx
        push   0x35
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_1AE8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        push   edi
        mov    esi, dword ptr [esp + 0x18]
        push   esi
        call   sub_1_1A60
        add    esp, 8
        mov    ebx, dword ptr [esi + 0x4d]
        add    ebx, 0x224
        push   ebx
        push   dword ptr [esi + 0xd]
        push   dword ptr [esi + 9]
        mov    cl, byte ptr [esi + 8]
        push   ecx
        push   eax
        call   sub_1_1320
        add    esp, 0x14
        mov    edx, eax
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_1B3D
        or     edi, edi
        jne    loc_1_1B3D
        push   ebx
        push   4
        lea    eax, [esp + 0x24]
        push   eax
        push   0x32
        push   edx
        call   sub_1_12A8
        add    esp, 0x14
        mov    edx, eax
loc_1_1B3D:
        cmp    dword ptr [esp + 0x20], -1
        je     loc_1_1B59
        push   ebx
        push   4
        lea    eax, [esp + 0x28]
        push   eax
        push   0x36
        push   edx
        call   sub_1_12A8
        add    esp, 0x14
        mov    edx, eax
loc_1_1B59:
        mov    ebp, dword ptr [0x3058]       ; -> obj1:0x3058
        or     ebp, ebp
        je     loc_1_1B80
        push   ebx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   ebp
        push   0xc
        push   edx
        call   sub_1_12A8
        add    esp, 0x14
        mov    edx, eax
loc_1_1B80:
        push   ebx
        push   edx
        call   sub_1_03FC
        add    esp, 8
        push   ebx
        push   0
        push   0
        push   0xff
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        push   dword ptr [esp + 0x18]
        sub    eax, dword ptr [esi + 0x4d]
        push   eax
        push   esi
        call   sub_1_24B8
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_1BB4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [ebx + 0x4d]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 0x89
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [ebp], 1
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 1], al
        mov    eax, dword ptr [ebx + 0x45]
        mov    word ptr [ebp + 8], ax
        push   0x8000
        call   sub_1_0E70
        mov    word ptr [ebp + 0xa], ax
        mov    eax, dword ptr [ebx + 0x11]
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [ebx + 0xd]
        lea    edi, [ebp + 0x1c]
        mov    esi, dword ptr [ebx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, byte ptr [ebx + 0xd]
        mov    byte ptr [ebp + 2], al
        lea    eax, [ebp + 0xec]
        lea    esi, [ebp + 0x224]
        push   esi
        push   eax
        call   sub_1_136C
        add    esp, 8
        mov    byte ptr [esp + 0x13], 7
        push   esi
        push   1
        lea    ecx, [esp + 0x1b]
        push   ecx
        push   0x35
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        push   esi
        push   4
        lea    ecx, [ebx + 0x19]
        push   ecx
        push   0x36
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        push   esi
        push   dword ptr [ebx + 0xd]
        push   dword ptr [ebx + 9]
        mov    cl, byte ptr [ebx + 8]
        push   ecx
        push   eax
        call   sub_1_1320
        add    esp, 0x14
        push   esi
        push   0
        push   0
        push   0xff
        push   eax
        call   sub_1_12A8
        add    esp, 0x14
        sub    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_1C80:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], 0
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_1BB4
        add    esp, 4
        lea    ecx, [esp + 4]
        push   ecx
        push   eax
        push   esi
        call   sub_1_24B8
        add    esp, 0xc
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_1CB0:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x18], 0xffffffff
        mov    dword ptr [esp + 0x20], 0xffffffff
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x28], 0x40
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x2c], eax
        mov    esi, dword ptr [esp + 0x34]
        mov    dword ptr [esi + 0x45], 0
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 0x38]
loc_1_1CF6:
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_19C4
        add    esp, 8
        or     eax, eax
        jne    loc_1_1FA4
        mov    ecx, 4
        sub    ecx, ebx
        mov    eax, dword ptr [esp + 0x28]
        sub    edx, edx
        div    ecx
        mov    ebp, eax
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x14], eax
        or     ebp, ebp
        je     loc_1_1DFA
        nop    
loc_1_1D34:
        mov    eax, ebp
        mov    dword ptr [esp + 0x10], 0x240
        sub    eax, 5
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 5
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_2588
        add    esp, 0x10
        cmp    eax, 0x79
        je     loc_1_1DFA
        or     eax, eax
        jne    loc_1_1FA4
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, 0xec
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x4d]
        add    eax, 0xec
        push   eax
        push   esi
        call   sub_1_181C
        add    esp, 0x10
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_1_1DE0
        cmp    byte ptr [eax], 2
        jne    loc_1_1DE0
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_1_1DA1
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x24], eax
loc_1_1DA1:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        mov    ecx, 0
        je     loc_1_1DAF
        mov    ecx, dword ptr [eax]
loc_1_1DAF:
        mov    eax, dword ptr [esi + 0x4d]
        mov    edx, dword ptr [eax + 0x10]
        cmp    dword ptr [esp + 0x18], -1
        jne    loc_1_1DC8
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x20], edx
loc_1_1DC8:
        mov    eax, dword ptr [esi + 0x1d]
        or     eax, eax
        je     loc_1_1E44
        cmp    eax, edx
        je     loc_1_1E44
        cmp    ebx, 2
        jae    loc_1_1E44
        and    eax, ecx
        and    ecx, edx
        cmp    eax, ecx
        jne    loc_1_1E44
loc_1_1DE0:
        push   0
        call   sub_4_0BE4
        add    esp, 4
        sub    ebp, eax
        add    ebp, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x14], eax
        jne    loc_1_1D34
loc_1_1DFA:
        inc    ebx
        push   ebx
        call   sub_1_17E0
        add    esp, 4
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    ecx, dword ptr [esp + 0x2c]
        sub    dword ptr [esp + 0x28], eax
        add    dword ptr [esp + 0x28], ecx
        mov    edx, eax
        sub    edx, ecx
        add    dword ptr [esi + 0x45], edx
        mov    dword ptr [esp + 0x2c], eax
        cmp    ebx, 4
        jb     loc_1_1CF6
        cmp    dword ptr [esp + 0x20], -1
        jne    loc_1_1E50
        mov    eax, 0x79
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
loc_1_1E44:
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x20], edx
loc_1_1E50:
        push   0
        mov    dword ptr [esp + 0x20], 0
        mov    dword ptr [esp + 0x2c], 0x40
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x2c], eax
        sub    ebp, ebp
loc_1_1E70:
        push   0
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x28]
        push   eax
        push   esi
        call   sub_1_1AE8
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_1FA4
        mov    ecx, 4
        sub    ecx, ebp
        mov    eax, dword ptr [esp + 0x28]
        sub    edx, edx
        div    ecx
        mov    ebx, eax
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x14], eax
        or     ebx, ebx
        je     loc_1_1F6E
        nop    
loc_1_1EB8:
        mov    eax, ebx
        mov    dword ptr [esp + 0x10], 0x240
        sub    eax, 5
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 5
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_2588
        add    esp, 0x10
        cmp    eax, 0x79
        je     loc_1_1F6E
        or     eax, eax
        jne    loc_1_1FA4
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, 0xec
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x4d]
        add    eax, 0xec
        push   eax
        push   esi
        call   sub_1_181C
        add    esp, 0x10
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_1_1F54
        mov    al, byte ptr [eax]
        cmp    al, 5
        jne    loc_1_1F50
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_1_1F24
        mov    ecx, dword ptr [eax]
        jmp    loc_1_1F28
loc_1_1F24:
        mov    ecx, dword ptr [esp + 0x18]
loc_1_1F28:
        mov    edx, dword ptr [esi + 0x4d]
        mov    eax, dword ptr [esp + 0x20]
        cmp    dword ptr [edx + 0x10], eax
        jne    loc_1_1F54
        cmp    ecx, dword ptr [esp + 0x18]
        jne    loc_1_1F54
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     loc_1_1FAC
        push   dword ptr [eax]
        call   sub_1_0E7C
        mov    dword ptr [esi + 0x21], eax
        jmp    loc_1_1FB3
        db  08Dh, 049h, 000h
loc_1_1F50:
        cmp    al, 6
        je     loc_1_1FCC
loc_1_1F54:
        push   0
        call   sub_4_0BE4
        add    esp, 4
        sub    ebx, eax
        add    ebx, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x14], eax
        jne    loc_1_1EB8
loc_1_1F6E:
        inc    ebp
        push   ebp
        call   sub_1_17E0
        add    esp, 4
        push   0
        call   sub_4_0BE4
        add    esp, 4
        sub    dword ptr [esp + 0x28], eax
        mov    ecx, dword ptr [esp + 0x28]
        add    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x28], ecx
        mov    dword ptr [esp + 0x2c], eax
        cmp    ebp, 4
        jb     loc_1_1E70
        mov    eax, 0x79
loc_1_1FA4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_1FAC:
        mov    dword ptr [esi + 0x21], 0xe10
loc_1_1FB3:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x19], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x11], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_1FCC:
        pop    ebp
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
sub_1_1FDC:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x20], 0x18
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x24], eax
        mov    esi, dword ptr [esp + 0x2c]
        mov    dword ptr [esi + 0x45], 0
        mov    dword ptr [esp + 0x1c], 0
        mov    edi, dword ptr [esp + 0x30]
loc_1_2018:
        push   dword ptr [esi + 0x35]
        push   -1
        push   dword ptr [esi + 0x1d]
        lea    eax, [esp + 0x24]
        push   eax
        push   esi
        call   sub_1_1AE8
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_214C
        mov    ecx, 2
        sub    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x20]
        sub    edx, edx
        div    ecx
        mov    ebp, eax
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x14], eax
        or     ebp, ebp
        je     loc_1_210B
        nop    
loc_1_2060:
        mov    eax, ebp
        mov    dword ptr [esp + 0x10], 0x240
        sub    eax, 5
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 5
        push   eax
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_2588
        add    esp, 0x10
        cmp    eax, 0x79
        je     loc_1_210B
        or     eax, eax
        jne    loc_1_214C
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, 0xec
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x4d]
        add    eax, 0xec
        push   eax
        push   esi
        call   sub_1_181C
        add    esp, 0x10
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_1_20F1
        cmp    byte ptr [eax], 5
        jne    loc_1_20E8
        mov    ebx, 0xffffffff
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_1_20CA
        mov    ebx, dword ptr [eax]
loc_1_20CA:
        mov    eax, dword ptr [esi + 0x4d]
        mov    ecx, dword ptr [esi + 0x11]
        cmp    dword ptr [eax + 0x10], ecx
        jne    loc_1_20F1
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     loc_1_2154
        push   dword ptr [eax]
        call   sub_1_0E7C
        mov    dword ptr [esi + 0x21], eax
        jmp    loc_1_215B
loc_1_20E8:
        or     eax, eax
        je     loc_1_20F1
        cmp    byte ptr [eax], 6
        je     loc_1_2170
loc_1_20F1:
        push   0
        call   sub_4_0BE4
        add    esp, 4
        sub    ebp, eax
        add    ebp, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x14], eax
        jne    loc_1_2060
loc_1_210B:
        mov    ebx, dword ptr [esp + 0x1c]
        inc    ebx
        push   ebx
        call   sub_1_17E0
        add    esp, 4
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    edx, eax
        mov    ecx, dword ptr [esp + 0x24]
        sub    dword ptr [esp + 0x20], edx
        add    dword ptr [esp + 0x20], ecx
        sub    edx, ecx
        add    dword ptr [esi + 0x45], edx
        mov    dword ptr [esp + 0x24], eax
        mov    dword ptr [esp + 0x1c], ebx
        cmp    ebx, 2
        jb     loc_1_2018
        mov    eax, 0x79
loc_1_214C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_2154:
        mov    dword ptr [esi + 0x21], 0xe10
loc_1_215B:
        cmp    ebx, -1
        je     loc_1_2163
        mov    dword ptr [esi + 0x19], ebx
loc_1_2163:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_2170:
        pop    ebp
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
sub_1_2180:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_055C
        add    esp, 4
        push   esi
        call   sub_1_1C80
        add    esp, 4
        or     eax, eax
        jne    loc_1_21B5
        push   1
        push   -1
        push   0
        push   0
        push   esi
        call   sub_1_06B8
        add    esp, 0x14
        push   esi
        call   sub_1_0590
        add    esp, 4
loc_1_21B5:
        pop    esi
        ret    
        db  090h
sub_1_21B8:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x28]
        push   esi
        call   sub_1_04E8
        add    esp, 4
        push   esi
        call   sub_1_055C
        add    esp, 4
        lea    eax, [esp + 0xc]
        push   eax
        push   esi
        call   sub_1_1CB0
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    loc_1_2238
        push   1
        push   dword ptr [esi + 0x19]
        push   dword ptr [esi + 0x11]
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_06B8
        add    esp, 0x14
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        mov    dword ptr [esi + 0x49], 0x22a4 ; -> sub_1_22A4
        cmp    dword ptr [0x2dd4], 0         ; -> loc_1_2DD4
        je     loc_1_227C
        push   dword ptr [esi + 0x31]
        push   dword ptr [esi + 0x11]
        push   0x450
        call   sub_4_0AC4
        add    esp, 0xc
        mov    dword ptr [0x2dd4], 0         ; -> loc_1_2DD4
        jmp    loc_1_227C
loc_1_2238:
        push   0
        mov    dword ptr [0x2dd4], 1         ; -> loc_1_2DD4
        push   -1
        push   0x44e
        call   sub_4_0AC4
        add    esp, 0xc
        cmp    eax, 0x12c
        mov    ebx, 0
        ja     loc_1_2266
        mov    ebx, 0x12c
        sub    ebx, eax
loc_1_2266:
        push   edi
        push   0x3eb
        push   esi
        call   sub_1_0844
        add    esp, 0xc
        mov    dword ptr [esi + 0x49], 0x21b8 ; -> sub_1_21B8
loc_1_227C:
        push   ebx
        push   esi
        call   sub_1_0660
        add    esp, 8
        push   esi
        call   sub_1_0590
        add    esp, 4
        mov    eax, dword ptr [esp + 0x2c]
        or     eax, eax
        je     loc_1_2299
        mov    dword ptr [eax], ebx
loc_1_2299:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
sub_1_22A4:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        push   esi
        call   sub_1_055C
        add    esp, 4
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_1FDC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    loc_1_232C
        cmp    dword ptr [esi + 0x35], 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [esi + 0x19]
        push   dword ptr [esi + 0x11]
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_06B8
        add    esp, 0x14
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        mov    dword ptr [esi + 0x49], 0x22a4 ; -> sub_1_22A4
        cmp    dword ptr [0x2dd4], 0         ; -> loc_1_2DD4
        je     loc_1_2490
        push   dword ptr [esi + 0x31]
        push   dword ptr [esi + 0x11]
        push   0x451
        call   sub_4_0AC4
        add    esp, 0xc
        mov    dword ptr [0x2dd4], 0         ; -> loc_1_2DD4
        jmp    loc_1_2490
        db  08Bh, 0FFh
loc_1_232C:
        push   edi
        cmp    edi, 5
        jne    loc_1_23DC
        push   0x3ec
        push   esi
        call   sub_1_0844
        add    esp, 0xc
        push   1
        push   -1
        push   0
        push   0
        push   esi
        call   sub_1_06B8
        add    esp, 0x14
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_1CB0
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_1_23B4
        mov    dword ptr [0x2dd4], 1         ; -> loc_1_2DD4
        push   0
        push   -1
        push   0x44e
        call   sub_4_0AC4
        add    esp, 0xc
        cmp    eax, 0x12c
        mov    ebx, 0
        ja     loc_1_2397
        mov    ebx, 0x12c
        sub    ebx, eax
loc_1_2397:
        push   edi
        push   0x3eb
        push   esi
        call   sub_1_0844
        add    esp, 0xc
        mov    dword ptr [esi + 0x49], 0x21b8 ; -> sub_1_21B8
        jmp    loc_1_2490
        db  08Bh, 0FFh
loc_1_23B4:
        push   1
        push   dword ptr [esi + 0x19]
        push   dword ptr [esi + 0x11]
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_06B8
        add    esp, 0x14
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        jmp    loc_1_2489
        db  08Bh, 0FFh
loc_1_23DC:
        push   0x3ed
        push   esi
        call   sub_1_0844
        add    esp, 0xc
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    ebp, eax
        cmp    dword ptr [esi + 0x31], ebp
        jge    loc_1_2458
        mov    ebx, dword ptr [esi + 0x11]
        push   0
        push   0x3e9
        push   esi
        call   sub_1_0844
        add    esp, 0xc
        push   0
        push   ebx
        push   0x44d
        call   sub_4_0AC4
        add    esp, 0xc
        cmp    eax, 0x12c
        mov    ebx, 0
        ja     loc_1_2431
        mov    ebx, 0x12c
        sub    ebx, eax
loc_1_2431:
        push   1
        push   -1
        push   0
        push   0
        push   esi
        call   sub_1_06B8
        add    esp, 0x14
        mov    dword ptr [0x2dd4], 1         ; -> loc_1_2DD4
        mov    dword ptr [esi + 0x49], 0x21b8 ; -> sub_1_21B8
        jmp    loc_1_2490
        db  08Dh, 049h, 000h
loc_1_2458:
        cmp    dword ptr [esi + 0x35], 0
        jne    loc_1_2469
        push   esi
        call   sub_1_05B4
        add    esp, 4
        mov    edi, eax
loc_1_2469:
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        cmp    dword ptr [esi + 0x2d], ebp
        jge    loc_1_2483
        mov    dword ptr [0x2dd4], 1         ; -> loc_1_2DD4
loc_1_2483:
        or     ebx, ebx
        jne    loc_1_2489
        sub    ebx, ebx
loc_1_2489:
        mov    dword ptr [esi + 0x49], 0x22a4 ; -> sub_1_22A4
loc_1_2490:
        push   ebx
        push   esi
        call   sub_1_0660
        add    esp, 8
        push   esi
        call   sub_1_0590
        add    esp, 4
        mov    eax, dword ptr [esp + 0x30]
        or     eax, eax
        je     loc_1_24AD
        mov    dword ptr [eax], ebx
loc_1_24AD:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  090h
sub_1_24B8:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x24]
        cmp    dword ptr [edi], 0
        jne    loc_1_24D9
        call   sub_4_0BD4
        mov    esi, eax
        shl    esi, 0x10
        call   sub_4_0BD4
        add    esi, eax
        mov    dword ptr [edi], esi
loc_1_24D9:
        mov    eax, dword ptr [edi]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esi + 0x4d]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [esp + 8], 2
        push   0x43
        call   sub_1_0E70
        mov    word ptr [esp + 0xa], ax
        cmp    dword ptr [esi + 0x35], 0
        je     loc_1_251C
        push   0
        call   sub_4_0BE4
        add    esp, 4
        cmp    dword ptr [esi + 0x2d], eax
        jge    loc_1_2510
        jmp    loc_1_251C
        db  090h
loc_1_2510:
        mov    eax, dword ptr [esi + 0x19]
        mov    dword ptr [esp + 0xc], eax
        jmp    loc_1_2524
        db  08Dh, 049h, 000h
loc_1_251C:
        mov    dword ptr [esp + 0xc], 0xffffffff
loc_1_2524:
        sub    eax, eax
        lea    edi, [esp + 0x10]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        cmp    dword ptr [esp + 0xc], -1
        jne    loc_1_2543
        mov    eax, dword ptr [esi + 0x51]
        movzx  eax, word ptr [eax + 4]
        push   eax
        call   sub_1_1169
        add    esp, 4
loc_1_2543:
        mov    ecx, dword ptr [esp + 0x20]
        sub    ecx, 0x12c
        sbb    eax, eax
        not    eax
        and    ecx, eax
        add    ecx, 0x12c
        push   0x10
        lea    eax, [esp + 0xc]
        push   eax
        push   0
        push   ecx
        push   dword ptr [esi + 0x4d]
        mov    eax, dword ptr [esi + 0x51]
        push   dword ptr [eax]
        call   sub_1_0980
        cmp    eax, -1
        jne    loc_1_2580
        call   sub_1_0BC8
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
loc_1_2580:
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_2588:
        sub    esp, 0x124
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0x10
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    dword ptr [esp + 0x10], eax
        mov    ebx, dword ptr [esp + 0x144]
        mov    ecx, ebx
        mov    edi, dword ptr [esp + 0x138]
        mov    eax, dword ptr [edi + 0x51]
        mov    ebp, dword ptr [eax]
        mov    dword ptr [esp + 0x30], 0
        mov    dword ptr [esp + 0x34], ebp
        inc    dword ptr [esp + 0x30]
        mov    esi, dword ptr [esp + 0x13c]
loc_1_25D4:
        mov    dword ptr [esp + 0x18], ecx
        mov    dword ptr [esp + 0x1c], 0
        lea    eax, [esp + 0x18]
        push   eax
        push   0
        push   0
        lea    eax, [esp + 0x3c]
        push   eax
        push   0
        call   sub_1_0B88
        or     eax, eax
        je     loc_1_2648
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   0
        push   dword ptr [esi]
        push   dword ptr [edi + 0x4d]
        push   ebp
        call   sub_1_0A70
        cmp    eax, -1
        je     loc_1_2658
        mov    edx, dword ptr [edi + 0x4d]
        mov    ecx, dword ptr [esp + 0x140]
        cmp    dword ptr [edx + 4], ecx
        je     loc_1_2668
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    ecx, dword ptr [esp + 0x10]
        sub    ecx, eax
        add    ecx, ebx
        jns    loc_1_25D4
        mov    eax, 0x79
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x124
        ret    
loc_1_2648:
        pop    ebp
        mov    eax, 0x79
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x124
        ret    
loc_1_2658:
        call   sub_1_0BC8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x124
        ret    
loc_1_2668:
        mov    dword ptr [esi], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x124
        ret    
        db  090h
loc_1_2678:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, 0xffffffff
        mov    dword ptr [0x2dd4], 0         ; -> loc_1_2DD4
        call   sub_2_01A0
        or     eax, eax
        je     loc_1_2698
        jmp    loc_1_26FE
        db  090h
loc_1_2698:
        mov    edi, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    edi, 0x2b84                   ; -> loc_1_2B84
        je     loc_1_26FC
loc_1_26A6:
        cmp    dword ptr [edi + 0x11], 0
        lea    eax, [esp + 8]
        push   eax
        push   edi
        jne    loc_1_26D0
        call   sub_1_21B8
        add    esp, 8
        or     eax, eax
        je     loc_1_26E6
        cmp    eax, 0x79
        je     loc_1_26E6
        cmp    eax, 5
        je     loc_1_26E6
        mov    esi, 0xffffffff
        jmp    loc_1_26FE
        db  090h
loc_1_26D0:
        call   sub_1_22A4
        add    esp, 8
        or     eax, eax
        je     loc_1_26E6
        cmp    eax, 0x79
        je     loc_1_26E6
        cmp    eax, 5
        jne    loc_1_2710
loc_1_26E6:
        mov    edi, dword ptr [edi]
        mov    ecx, dword ptr [esp + 8]
        sub    ecx, esi
        sbb    eax, eax
        and    ecx, eax
        add    esi, ecx
        cmp    edi, 0x2b84                   ; -> loc_1_2B84
        jne    loc_1_26A6
loc_1_26FC:
        sub    eax, eax
loc_1_26FE:
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        je     loc_1_2708
        mov    dword ptr [ecx], esi
loc_1_2708:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_2710:
        mov    esi, 0xffffffff
        jmp    loc_1_26FE
        db  090h
sub_1_2718:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x11], 0
        jne    loc_1_272C
        mov    eax, 0x12c
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_272C:
        cmp    dword ptr [esi + 0x21], -1
        jne    loc_1_273C
        mov    eax, 0x7fffffff
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_273C:
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x29]
        cmp    eax, ecx
        jge    loc_1_2754
        sub    ecx, eax
        mov    eax, ecx
        pop    esi
        ret    
        db  090h
loc_1_2754:
        mov    ecx, dword ptr [esi + 0x2d]
        cmp    eax, ecx
        jge    loc_1_2774
        sub    ecx, eax
        mov    eax, ecx
        cdq    
        sub    eax, edx
        sar    eax, 1
        cmp    eax, 0x12c
        jg     loc_1_27A1
        mov    eax, 0x12c
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_2774:
        mov    ecx, dword ptr [esi + 0x31]
        cmp    eax, ecx
        jge    loc_1_279C
        mov    dword ptr [esi + 0x19], 0xffffffff
        sub    ecx, eax
        mov    eax, ecx
        cdq    
        sub    eax, edx
        sar    eax, 1
        cmp    eax, 0x12c
        jg     loc_1_27A1
        mov    eax, 0x12c
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_279C:
        mov    eax, 0x12c
loc_1_27A1:
        pop    esi
        ret    
        db  090h
sub_1_27A4:
        sub    esp, 0x14
        push   esi
        push   edi
        push   0x11
        push   2
        push   2
        call   sub_1_0848
        mov    esi, eax
        cmp    esi, -1
        jne    loc_1_27C8
        call   sub_1_0BC8
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_27C8:
        push   4
        mov    dword ptr [esp + 0xc], 1
        lea    eax, [esp + 0xc]
        push   eax
        push   4
        push   0xffff
        push   esi
        call   sub_1_0BD0
        or     eax, eax
        je     loc_1_2800
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_2800:
        push   4
        lea    eax, [esp + 0xc]
        push   eax
        push   0x20
        push   0xffff
        push   esi
        call   sub_1_0BD0
        or     eax, eax
        je     loc_1_2830
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_2830:
        mov    edi, dword ptr [esp + 0x24]
        or     edi, edi
        jne    loc_1_2870
        mov    dword ptr [esp + 8], 0x4d2
        push   4
        lea    eax, [esp + 0xc]
        push   eax
        push   0x8000
        push   0xffff
        push   esi
        call   sub_1_0BD0
        or     eax, eax
        je     loc_1_2870
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
loc_1_2870:
        push   0x44
        mov    word ptr [esp + 0x10], 2
        call   sub_1_0E70
        mov    word ptr [esp + 0xe], ax
        mov    dword ptr [esp + 0x10], edi
        lea    edi, [esp + 0x14]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        push   0x10
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_08D8
        or     eax, eax
        je     loc_1_28B8
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_28B8:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h
loc_1_28CC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_28D0:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        db  000h
loc_1_28D8:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    ecx, dword ptr [edi]
        push   es
        sub    al, 0x2e
        das    
        add    byte ptr [edi], dh
        add    al, byte ptr [eax]
        add    byte ptr [ebx + 0x79], dl
        jae    0x2964
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x2968
        jb     0x295d
        outsb  dx, byte ptr [esi]
        je     loc_1_293E
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x2971
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_1_2961
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_1_297C
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0x2987
        je     0x2979
        jb     0x2964
        popal  
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0x2994
        je     loc_1_2986
        jb     0x2971
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_1_2928:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0x29a2
        je     0x2994
        jb     0x297f
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_2938:
        inc    esp
        inc    ebp
        push   ebx
        push   esp
        add    byte ptr [eax], al
loc_1_293E:
        add    byte ptr [eax], al
loc_1_2940:
        dec    esp
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 4], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
loc_1_2961:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2978:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_297C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2980:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2984:
        add    byte ptr [eax], al
loc_1_2986:
        add    byte ptr [eax], al
loc_1_2988:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  VDHCP_Service_00         ; fixup
        dd  VDHCP_Service_01         ; fixup
        dd  VDHCP_Service_02         ; fixup
        dd  VDHCP_Service_03         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0049Ah                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VDHCP   '                  ; DDB_Name
        dd  05400B500h                  ; DDB_Init_Order
        dd  VDHCP_Control            ; DDB_Control_Proc
        dd  VDHCP_PM_API             ; DDB_V86_API_Proc
        dd  VDHCP_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x298C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_29F0:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    loc_1_2A7C
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x2a80
        jb     loc_1_2A75
        outsb  dx, byte ptr [esi]
        je     0x2a56
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_1_2A89
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x2a79
        push   ebx
        jb     loc_1_2A97
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        inc    esp
        dec    eax
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_2A30:
        push   esi
        jb     0x2aa7
        db  069h, 06Fh, 06Eh, 000h
loc_1_2A38:
        push   ebx
        jns    loc_1_2AAE
        je     loc_1_2AA2
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_1_2AB4
        jb     0x2aa9
        outsb  dx, byte ptr [esi]
        je     0x2a8a
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x2abd
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_1_2AAD
        push   ebx
        jb     0x2acb
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        inc    esp
        dec    eax
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_2A64:
        push   eax
        outsd  dx, dword ptr [esi]
        jo     loc_1_2ADD
        jo     loc_1_2AB0
        insb   byte ptr es:[edi], dx
        popal  
        add    byte ptr [bx + si], al
        add    byte ptr [eax + ebp*2 + 0x63], al
        jo     loc_1_2ABE
loc_1_2A75:
        outsb  dx, byte ptr [esi]
        outsw  dx, word ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A7C:
        dec    edi
        jo     0x2af3
        imul   ebp, dword ptr [edi + 0x6e], 0x6f666e49
        add    byte ptr [eax], al
loc_1_2A88:
        push   ebx
loc_1_2A89:
        jns    loc_1_2AFE
        je     0x2af2
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x2b04
        jb     0x2af9
        outsb  dx, byte ptr [esi]
        je     loc_1_2ADA
loc_1_2A97:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_1_2B0D
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x2afd
        push   ebx
loc_1_2AA2:
        jb     0x2b1b
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
loc_1_2AAD:
        pop    esp
loc_1_2AAE:
        inc    esp
        dec    eax
loc_1_2AB0:
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_2AB4:
        push   ebx
        jns    loc_1_2B2A
        je     loc_1_2B1E
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_1_2B30
loc_1_2ABE:
        jb     loc_1_2B25
        outsb  dx, byte ptr [esi]
        je     loc_1_2B06
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x2b39
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x2b29
        push   ebx
        jb     0x2b47
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
loc_1_2ADA:
        inc    esp
        dec    eax
        inc    ebx
loc_1_2ADD:
        push   eax
        add    byte ptr [eax], al
loc_1_2AE0:
        push   esi
        jb     0x2b57
        imul   ebp, dword ptr [edi + 0x6e], 0x706f5000
        jne    0x2b5d
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        add    byte ptr [bx + si], al
        db  000h
loc_1_2AF4:
        inc    esp
        push   0x6e497063
        outsw  dx, word ptr [esi]
        add    byte ptr [eax], al
loc_1_2AFE:
        add    byte ptr [eax], al
loc_1_2B00:
        inc    esp
        push   0x6e497063
loc_1_2B06:
        outsw  dx, word ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B0C:
        dec    edi
loc_1_2B0D:
        jo     0x2b83
        imul   ebp, dword ptr [edi + 0x6e], 0x6f666e49
        add    byte ptr [eax], al
loc_1_2B18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B1E:
        add    byte ptr [eax], al
loc_1_2B20:
        sbb    byte ptr [ebx], ch
        add    byte ptr [eax], al
loc_1_2B24:
        inc    esp
loc_1_2B25:
        push   0x7063
loc_1_2B2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B34:
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
loc_1_2B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B58:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B5C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B94:
        add    byte ptr [eax], al
loc_1_2B96:
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
loc_1_2BB0:
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
loc_1_2DD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2DD8:
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
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        add    byte ptr [ebx + 1], dl
        int3   
        add    dword ptr [edx], ebp
        add    al, byte ptr [esi - 0xffd1bfe]
        add    dl, byte ptr [eax + ebp*2 + 0x69]
        jae    0x3041
        inc    esp
        dec    eax
        inc    ebx
loc_1_3024:
        push   eax
        and    byte ptr [ebx + 0x6c], ah
loc_1_3028:
        imul   esp, dword ptr [ebp + 0x6e], 0x61772074
        jae    0x3051
        jne    0x30a1
        popal  
loc_1_3034:
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x30a9
        and    byte ptr [edi + 0x62], ch
        je     0x30a0
        imul   ebp, dword ptr [esi + 0x20], 0x49206e61
        push   eax
        and    byte ptr [esi + 0x65], ch
        je     0x30c3
loc_1_304C:
        outsd  dx, dword ptr [esi]
        jb     0x30ba
        and    byte ptr [ecx + 0x64], ah
        jb     0x30ba
        jae    0x30ca
        and    byte ptr [esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x20], ah
        inc    esp
loc_1_3060:
        dec    eax
        inc    ebx
        push   eax
        db  020h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xc1c  R-X
        db  007h, 000h, 000h, 000h
        dd  loc_3_001C               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 0ACh, 000h, 053h, 001h, 0CCh, 001h, 02Ah, 002h, 086h, 002h
        db  0E4h, 002h, 0F0h, 002h, 054h, 068h, 069h, 073h, 020h, 044h, 048h, 043h, 050h, 020h, 063h, 06Ch
        db  069h, 065h, 06Eh, 074h, 020h, 077h, 061h, 073h, 020h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h
        db  074h, 06Fh, 020h, 06Fh, 062h, 074h, 061h, 069h, 06Eh, 020h, 061h, 06Eh, 020h, 049h, 050h, 020h
        db  06Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 020h, 061h, 064h, 064h, 072h, 065h, 073h, 073h, 020h
        db  066h, 072h, 06Fh, 06Dh, 020h, 061h, 020h, 044h, 048h, 043h, 050h, 020h, 073h, 065h, 072h, 076h
        db  065h, 072h, 02Eh, 020h, 053h, 06Fh, 06Dh, 065h, 020h, 06Fh, 066h, 020h, 079h, 06Fh, 075h, 072h
        db  020h, 06Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 020h, 073h, 065h, 072h, 076h, 069h, 063h, 065h
        db  020h, 06Dh, 061h, 079h, 020h, 06Eh, 06Fh, 074h, 020h, 062h, 065h, 020h, 061h, 076h, 061h, 069h
        db  06Ch, 061h, 062h, 06Ch, 065h, 02Eh, 020h, 044h, 06Fh, 020h, 079h, 06Fh, 075h, 020h, 077h, 061h
        db  06Eh, 074h, 020h, 074h, 06Fh, 020h, 073h, 065h, 065h, 020h, 066h, 075h, 074h, 075h, 072h, 065h
        db  020h, 044h, 048h, 043h, 050h, 020h, 06Dh, 065h, 073h, 073h, 061h, 067h, 065h, 073h, 03Fh, 000h
        db  054h, 068h, 069h, 073h, 020h, 044h, 048h, 043h, 050h, 020h, 063h, 06Ch, 069h, 065h, 06Eh, 074h
        db  020h, 068h, 061h, 073h, 020h, 06Ch, 06Fh, 073h, 074h, 020h, 074h, 068h, 065h, 020h, 06Ch, 065h
        db  061h, 073h, 065h, 020h, 074h, 06Fh, 020h, 069h, 074h, 073h, 020h, 049h, 050h, 020h, 06Eh, 065h
        db  074h, 077h, 06Fh, 072h, 06Bh, 020h, 061h, 064h, 064h, 072h, 065h, 073h, 073h, 02Eh, 020h, 04Eh
        db  065h, 074h, 077h, 06Fh, 072h, 06Bh, 020h, 074h, 072h, 061h, 06Eh, 073h, 061h, 063h, 074h, 069h
        db  06Fh, 06Eh, 073h, 020h, 063h, 061h, 06Eh, 020h, 06Eh, 06Fh, 020h, 06Ch, 06Fh, 06Eh, 067h, 065h
        db  072h, 020h, 062h, 065h, 020h, 070h, 065h, 072h, 066h, 06Fh, 072h, 06Dh, 065h, 064h, 020h, 06Fh
        db  06Eh, 020h, 074h, 068h, 065h, 020h, 064h, 065h, 076h, 069h, 063h, 065h, 02Eh, 020h, 044h, 06Fh
        db  020h, 079h, 06Fh, 075h, 020h, 077h, 061h, 06Eh, 074h, 020h, 074h, 06Fh, 020h, 073h, 065h, 065h
        db  020h, 066h, 075h, 074h, 075h, 072h, 065h, 020h, 044h, 048h, 043h, 050h, 020h, 06Dh, 065h, 073h
        db  073h, 061h, 067h, 065h, 073h, 03Fh, 000h, 054h, 068h, 069h, 073h, 020h, 044h, 048h, 043h, 050h
        db  020h, 063h, 06Ch, 069h, 065h, 06Eh, 074h, 020h, 077h, 061h, 073h, 020h, 075h, 06Eh, 061h, 062h
        db  06Ch, 065h, 020h, 074h, 06Fh, 020h, 06Fh, 062h, 074h, 061h, 069h, 06Eh, 020h, 061h, 06Eh, 020h
        db  049h, 050h, 020h, 06Eh, 065h, 074h, 077h, 06Fh
sub_2_01A0:
        jb     loc_2_020D
        and    byte ptr [ecx + 0x64], ah
        jb     loc_2_020D
        jae    0x21d
        and    byte ptr [esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x20], ah
        inc    esp
        dec    eax
        inc    ebx
        push   eax
        and    byte ptr [ebx + 0x65], dh
        jb     0x231
        jb     0x1ec
        and    byte ptr [edi + ebp*2 + 0x20], al
        jns    0x233
        jne    0x1e6
        ja     loc_2_0229
        outsb  dx, byte ptr [esi]
        je     loc_2_01EB
        je     0x23c
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[esi + 0x75], ah
        je     0x24b
        jb     0x23d
        and    byte ptr [eax + ecx*2 + 0x43], al
        push   eax
        and    byte ptr [ebp + 0x65], ch
        jae    0x255
        popal  
        jae    loc_2_0226
        add    byte ptr [eax + ebp*2 + 0x69], dl
loc_2_01EB:
        jae    loc_2_020D
        inc    esp
        dec    eax
        inc    ebx
        push   eax
        and    byte ptr [ebx + 0x6c], ah
        imul   esp, dword ptr [ebp + 0x6e], 0x6f632074
        jne    0x269
        outsb  dx, byte ptr fs:[esi]
        daa    
        je     loc_2_0222
        jb     0x269
        outsb  dx, byte ptr [esi]
        ja     0x228
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x50], cl
loc_2_020D:
        and    byte ptr [ecx + 0x64], ah
        jb     0x278
        jae    0x288
        and    byte ptr [ebp + 0x61], ch
        jae    loc_2_0280
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
        jns    0x291
loc_2_0222:
        jne    0x244
        ja     loc_2_0287
loc_2_0226:
        outsb  dx, byte ptr [esi]
        je     loc_2_0249
loc_2_0229:
        je     loc_2_029A
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[esi + 0x75], ah
        je     loc_2_02A9
        jb     0x29b
        and    byte ptr [eax + ecx*2 + 0x43], al
        push   eax
        and    byte ptr [ebp + 0x65], ch
        jae    0x2b3
        popal  
        jae    0x284
        add    byte ptr [eax + ebp*2 + 0x65], dl
loc_2_0249:
        and    byte ptr [eax + ecx*2 + 0x43], al
        push   eax
        and    byte ptr [ebx + 0x6c], ah
        imul   esp, dword ptr [ebp + 0x6e], 0x75732074
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_2_02D0
        jne    0x2cc
        insb   byte ptr es:[edi], dx
        jns    loc_2_0283
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_2_02CD
        and    byte ptr fs:[ecx + 0x6e], ah
        and    byte ptr [ecx + 0x50], cl
        and    byte ptr [ecx + 0x64], ah
        jb     loc_2_02DA
        jae    0x2ea
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
        jns    0x2ed
        jne    0x2a0
loc_2_0280:
        ja     loc_2_02E3
        outsb  dx, byte ptr [esi]
loc_2_0283:
        je     loc_2_02A5
        je     0x2f6
loc_2_0287:
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[esi + 0x75], ah
        je     0x305
        jb     0x2f7
        and    byte ptr [eax + ecx*2 + 0x43], al
        push   eax
        and    byte ptr [ebp + 0x65], ch
loc_2_029A:
        jae    0x30f
        popal  
        jae    loc_2_02E0
        add    byte ptr [eax + ebp*2 + 0x65], dl
loc_2_02A5:
        and    byte ptr [eax + ecx*2 + 0x43], al
loc_2_02A9:
        push   eax
        and    byte ptr [ebx + 0x6c], ah
        imul   esp, dword ptr [ebp + 0x6e], 0x75732074
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_2_032C
        jne    loc_2_0328
        insb   byte ptr es:[edi], dx
        jns    0x2df
        jb     loc_2_0326
        outsb  dx, byte ptr [esi]
        ja     loc_2_032A
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ecx + 0x50], cl
loc_2_02CD:
        and    byte ptr [ecx + 0x64], ah
loc_2_02D0:
        jb     loc_2_0338
        jae    loc_2_0348
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
loc_2_02DA:
        jns    0x34b
        jne    0x2fe
        ja     0x341
loc_2_02E0:
        outsb  dx, byte ptr [esi]
        je     loc_2_0303
loc_2_02E3:
        je     loc_2_0354
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[esi + 0x75], ah
        je     0x363
        jb     0x355
        and    byte ptr [eax + ecx*2 + 0x43], al
        push   eax
        and    byte ptr [ebp + 0x65], ch
        jae    0x36d
        popal  
        jae    loc_2_033E
        add    byte ptr [eax + ecx*2 + 0x43], al
loc_2_0303:
        push   eax
        and    byte ptr [ebx + 0x6c], al
        imul   esp, dword ptr [ebp + 0x6e], 0x74
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
loc_2_0326:
        add    byte ptr [eax], al
loc_2_0328:
        add    byte ptr [eax], al
loc_2_032A:
        add    byte ptr [eax], al
loc_2_032C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0338:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_033E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0348:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0354:
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
sub_2_0374:
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
loc_2_04E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_04F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_04F4:
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0x30c  R-X
        db  0CDh, 020h, 000h, 000h
loc_3_0004:
        mov    byte ptr [edx + esi*2], al
        adc    al, 0xcd
        and    byte ptr [eax], al
        db  000h
loc_3_000C:
        inc    eax
        db  000h
loc_3_000E:
        jb     loc_3_001C
        db  0CDh, 020h
loc_3_0012:
        or     al, byte ptr [eax]
loc_3_0014:
        mov    byte ptr [ebx], al
        dd  obj1:0x28C8              ; fixup
loc_3_0018:
        sub    byte ptr [eax], al
        add    al, bh
loc_3_001C:
        cmc    
        sbb    eax, eax
        ret    
        db  0E8h, 0DBh, 0FFh, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 084h, 0FFh, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x3040              ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x3048              ; fixup
        db  000h, 000h, 000h, 000h, 0A1h
        dd  obj1:0x28C8              ; fixup
        db  0A3h
        dd  obj1:0x3044              ; fixup
        db  0E8h
        dd  sub_1_002A               ; fixup
        db  0E8h, 07Bh, 006h, 000h, 000h, 00Bh, 0C0h, 075h, 003h, 02Bh, 0C0h, 0C3h, 0E8h, 037h, 002h, 000h
        db  000h, 00Bh, 0C0h, 075h, 003h, 02Bh, 0C0h, 0C3h, 0C7h, 005h
        dd  loc_1_2B78               ; fixup
        db  001h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_2B64               ; fixup
        dd  loc_1_2B60               ; fixup
        db  0C7h, 005h
        dd  loc_1_2B60               ; fixup
        dd  loc_1_2B60               ; fixup
        db  0C7h, 005h
        dd  loc_1_2B88               ; fixup
        dd  loc_1_2B84               ; fixup
        db  0C7h, 005h
        dd  loc_1_2B84               ; fixup
        dd  loc_1_2B84               ; fixup
        db  0C7h, 005h
        dd  obj1:0x3050              ; fixup
        dd  loc_1_304C               ; fixup
        db  0C7h, 005h
        dd  loc_1_304C               ; fixup
        dd  loc_1_304C               ; fixup
        db  0C7h, 005h
        dd  loc_1_2B74               ; fixup
        dd  loc_1_2B70               ; fixup
        db  0C7h, 005h
        dd  loc_1_2B70               ; fixup
        dd  loc_1_2B70               ; fixup
        db  0E8h, 011h, 006h, 000h, 000h, 00Bh, 0C0h, 074h, 005h, 02Bh, 0C0h, 0C3h, 08Bh, 0FFh, 06Ah, 000h
        db  0E8h
        dd  loc_1_2678               ; fixup
        db  083h, 0C4h, 004h, 06Ah, 001h, 068h
        dd  loc_4_07D8               ; fixup
        db  0E8h
        dd  loc_1_1155               ; fixup
        db  083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 003h, 02Bh, 0C0h, 0C3h, 068h
        dd  loc_4_070C               ; fixup
        db  0E8h
        dd  loc_1_0058               ; fixup
        db  083h, 0C4h, 004h, 068h
        dd  loc_4_07B4               ; fixup
        db  0E8h
        dd  loc_1_004D               ; fixup
        db  083h, 0C4h, 004h, 06Ah, 000h, 06Ah, 000h, 0E8h
        dd  sub_4_058C               ; fixup
        db  083h, 0C4h, 008h, 0E8h
        dd  sub_1_002A               ; fixup
        db  0C7h, 005h
        dd  loc_1_28CC               ; fixup
        db  000h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 0C3h, 090h, 02Bh, 0C0h, 0C3h, 090h, 056h
        db  057h, 06Ah, 000h, 0FFh, 074h, 024h, 010h, 0E8h, 057h, 004h, 000h, 000h, 083h, 0C4h, 008h, 08Bh
        db  0F0h, 00Bh, 0F6h, 074h, 052h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh
        db  0F7h, 0D1h, 051h, 0E8h
        dd  sub_4_06D0               ; fixup
        db  083h, 0C4h, 004h, 08Bh, 04Ch, 024h, 010h, 089h, 001h, 08Bh, 0D0h, 00Bh, 0D2h, 074h, 028h, 08Bh
        db  0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h
        db  08Bh, 0F7h, 08Bh, 0FAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h
        db  02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 08Bh, 0FFh, 05Fh, 0B8h, 09Ah, 000h, 000h, 0C0h, 05Eh, 0C3h, 05Fh
        db  0B8h, 001h, 000h, 000h, 0C0h, 05Eh, 0C3h
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x30], eax
        mov    dword ptr [ebp - 0x2c], 0x28ec ; -> obj1:0x28EC
        mov    dword ptr [ebp - 0x28], 0x80000002
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        jne    loc_3_0290
        mov    dword ptr [ebp - 0x24], 0x10
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0x20], eax
        lea    eax, [ebp - 0x44]
        mov    dword ptr [ebp - 0x1c], eax
        lea    eax, [ebp - 0x34]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x10], 0x2928 ; -> loc_1_2928
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    esi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        or     esi, esi
        jne    loc_3_0290
        lea    edi, [ebp - 0x44]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_4_06D0
        add    esp, 4
        mov    dword ptr [0x3058], eax       ; -> obj1:0x3058
        or     eax, eax
        je     loc_3_0290
        lea    edi, [ebp - 0x44]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [0x3058]       ; -> obj1:0x3058
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_0290:
        sub    eax, eax
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Bh, 0FFh, 053h, 056h, 057h, 02Bh, 0DBh, 039h, 01Dh
        dd  loc_1_2940               ; fixup
        db  074h, 03Dh, 0BEh
        dd  obj1:0x2944              ; fixup
        db  0FFh, 076h, 0FCh, 0E8h
        dd  loc_1_1083               ; fixup
        db  083h, 0C4h, 004h, 089h, 006h, 08Bh, 0F8h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh
        db  0F7h, 0D1h, 049h, 03Bh, 0CBh, 072h, 010h, 08Bh, 03Eh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h
        db  0F2h, 0AEh, 0F7h, 0D1h, 08Dh, 059h, 0FFh, 083h, 0C6h, 008h, 083h, 07Eh, 0FCh, 000h, 075h, 0C8h
        db  083h, 0C3h, 04Eh, 053h, 0E8h
        dd  sub_4_06D0               ; fixup
        db  083h, 0C4h, 004h, 0A3h
        dd  loc_1_2978               ; fixup
        db  00Bh, 0C0h, 075h, 009h, 02Bh, 0C0h, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 049h, 000h, 068h, 052h, 004h
        db  000h, 000h, 0E8h
        dd  loc_1_1083               ; fixup
        db  083h, 0C4h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1114  R-X
        db  081h, 0ECh, 0C8h, 001h, 000h, 000h, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 054h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 058h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 05Ch, 000h, 001h
        db  000h, 000h, 0C7h, 044h, 024h, 060h, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 064h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 01Ch, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh, 042h
        db  055h, 046h, 0C7h, 044h, 024h, 040h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 000h, 001h
        db  000h, 000h, 08Dh, 084h, 024h, 0D8h, 000h, 000h, 000h, 089h, 044h, 024h, 044h, 0C7h, 044h, 024h
        db  048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h, 030h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh
        db  044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 020h, 050h, 08Dh, 044h, 024h, 048h, 050h, 08Dh, 044h
        db  024h, 060h, 050h, 06Ah, 000h, 0A1h
        dd  obj1:0x3054              ; fixup
        db  0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 074h, 010h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh
        db  081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 08Bh, 044h, 024h, 01Ch, 0C1h, 0E8h
        db  003h, 089h, 044h, 024h, 020h, 0C7h, 044h, 024h, 028h, 000h, 000h, 000h, 000h, 00Bh, 0C0h, 00Fh
        db  084h, 071h, 002h, 000h, 000h, 08Dh, 09Ch, 024h, 0D8h, 000h, 000h, 000h, 08Bh, 0B4h, 024h, 0DCh
        db  001h, 000h, 000h, 08Bh, 003h, 03Dh, 001h, 003h, 000h, 000h, 00Fh, 085h, 041h, 002h, 000h, 000h
        db  089h, 044h, 024h, 054h, 08Bh, 043h, 004h, 089h, 044h, 024h, 058h, 0C7h, 044h, 024h, 05Ch, 000h
        db  001h, 000h, 000h, 0C7h, 044h, 024h, 060h, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 064h, 001h
        db  000h, 000h, 000h, 0C7h, 044h, 024h, 01Ch, 004h, 000h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh
        db  042h, 055h, 046h, 0C7h, 044h, 024h, 040h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 004h
        db  000h, 000h, 000h, 08Dh, 044h, 024h, 02Ch, 089h, 044h, 024h, 044h, 0C7h, 044h, 024h, 048h, 000h
        db  000h, 000h, 000h, 08Dh, 07Ch, 024h, 030h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h
        db  030h, 050h, 08Dh, 044h, 024h, 020h, 050h, 08Dh, 044h, 024h, 048h, 050h, 08Dh, 044h, 024h, 060h
        db  050h, 06Ah, 000h, 0A1h
        dd  obj1:0x3054              ; fixup
        db  0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 085h, 0BBh, 001h, 000h, 000h, 081h, 07Ch
        db  024h, 02Ch, 003h, 003h, 000h, 000h, 00Fh, 085h, 0ADh, 001h, 000h, 000h, 0C7h, 044h, 024h, 05Ch
        db  000h, 002h, 000h, 000h, 0C7h, 044h, 024h, 064h, 001h, 000h, 000h, 000h, 0C7h, 044h, 024h, 01Ch
        db  05Ch, 000h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh, 042h, 055h, 046h, 0C7h, 044h, 024h, 040h
        db  000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 05Ch, 000h, 000h, 000h, 08Dh, 044h, 024h, 07Ch
        db  089h, 044h, 024h, 044h, 0C7h, 044h, 024h, 048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h, 030h
        db  02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 020h, 050h
        db  08Dh, 044h, 024h, 048h, 050h, 08Dh, 044h, 024h, 060h, 050h, 06Ah, 000h, 0A1h
        dd  obj1:0x3054              ; fixup
        db  0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 085h, 03Ah, 001h, 000h, 000h, 083h, 0BCh
        db  024h, 0D0h, 000h, 000h, 000h, 001h, 00Fh, 082h, 02Ch, 001h, 000h, 000h, 08Bh, 084h, 024h, 0D0h
        db  000h, 000h, 000h, 08Dh, 004h, 040h, 0C1h, 0E0h, 003h, 089h, 044h, 024h, 01Ch, 050h, 0E8h, 0CAh
        db  004h, 000h, 000h, 083h, 0C4h, 004h, 089h, 044h, 024h, 024h, 00Bh, 0C0h, 08Bh, 00Dh
        dd  obj1:0x3054              ; fixup
        db  00Fh, 084h, 025h, 001h, 000h, 000h, 0C7h, 044h, 024h, 064h, 002h, 001h, 000h, 000h, 0C7h, 044h
        db  024h, 050h, 04Eh, 042h, 055h, 046h, 0C7h, 044h, 024h, 040h, 000h, 000h, 000h, 000h, 08Bh, 044h
        db  024h, 01Ch, 089h, 044h, 024h, 04Ch, 08Bh, 044h, 024h, 024h, 089h, 044h, 024h, 044h, 0C7h, 044h
        db  024h, 048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h, 030h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh
        db  08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 020h, 050h, 08Dh, 044h, 024h, 048h, 050h, 08Dh
        db  044h, 024h, 060h, 050h, 06Ah, 000h, 0FFh, 051h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 074h, 007h
        db  0E9h, 095h, 000h, 000h, 000h, 08Bh, 0FFh, 02Bh, 0FFh, 039h, 0BCh, 024h, 0D0h, 000h, 000h, 000h
        db  00Fh, 086h, 084h, 000h, 000h, 000h, 08Bh, 06Ch, 024h, 024h, 083h, 0C5h, 014h, 00Bh, 0F6h, 074h
        db  00Eh, 066h, 08Bh, 084h, 024h, 0E0h, 001h, 000h, 000h, 066h, 039h, 045h, 000h, 074h, 006h, 083h
        db  07Dh, 0ECh, 000h, 075h, 058h, 08Dh, 044h, 024h, 014h, 050h, 08Dh, 044h, 024h, 01Ch, 050h, 08Dh
        db  044h, 024h, 01Bh, 050h, 08Dh, 044h, 024h, 074h, 050h, 08Dh, 045h, 0ECh, 050h, 0FFh, 074h, 024h
        db  034h, 08Dh, 084h, 024h, 0F0h, 000h, 000h, 000h, 050h, 0E8h, 0ADh, 000h, 000h, 000h, 083h, 0C4h
        db  01Ch, 00Bh, 0C0h, 075h, 076h, 083h, 07Ch, 024h, 018h, 000h, 074h, 021h, 0FFh, 074h, 024h, 013h
        db  08Dh, 044h, 024h, 06Ch, 050h, 0FFh, 073h, 004h, 0FFh, 074h, 024h, 020h, 066h, 08Bh, 045h, 000h
        db  050h, 0E8h, 065h, 006h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0F6h, 075h, 06Ah, 047h, 083h, 0C5h
        db  018h, 039h, 0BCh, 024h, 0D0h, 000h, 000h, 000h, 077h, 083h, 0FFh, 074h, 024h, 024h, 0E8h
        dd  sub_1_0024               ; fixup
        db  083h, 0C4h, 004h, 083h, 0C3h, 008h, 0FFh, 044h, 024h, 028h, 08Bh, 044h, 024h, 020h, 039h, 044h
        db  024h, 028h, 00Fh, 082h, 09Dh, 0FDh, 0FFh, 0FFh, 05Dh, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh
        db  05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h
        db  0C8h, 001h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 0FFh, 074h, 024h, 024h, 0E8h
        dd  sub_1_0024               ; fixup
        db  083h, 0C4h, 004h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h
        db  08Dh, 049h, 000h, 05Dh, 083h, 0F8h, 001h, 01Bh, 0C0h, 0F7h, 0D8h, 05Fh, 05Eh, 05Bh, 081h, 0C4h
        db  0C8h, 001h, 000h, 000h, 0C3h, 08Bh, 0FFh, 081h, 0ECh, 0A0h, 000h, 000h, 000h, 053h, 056h, 057h
        db  055h, 08Bh, 084h, 024h, 0C8h, 000h, 000h, 000h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 044h
        db  024h, 03Ch, 002h, 002h, 000h, 000h, 0C7h, 044h, 024h, 048h, 000h, 001h, 000h, 000h, 02Bh, 0EDh
        db  039h, 0ACh, 024h, 0B8h, 000h, 000h, 000h, 00Fh, 084h, 02Dh, 001h, 000h, 000h, 08Bh, 09Ch, 024h
        db  0B4h, 000h, 000h, 000h, 08Bh, 0B4h, 024h, 0BCh, 000h, 000h, 000h, 08Bh, 003h, 03Dh, 000h, 002h
        db  000h, 000h, 00Fh, 085h, 001h, 001h, 000h, 000h, 089h, 044h, 024h, 03Ch, 08Bh, 043h, 004h, 089h
        db  044h, 024h, 040h, 0C7h, 044h, 024h, 044h, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 001h
        db  000h, 000h, 000h, 0C7h, 044h, 024h, 010h, 004h, 000h, 000h, 000h, 0C7h, 044h, 024h, 038h, 04Eh
        db  042h, 055h, 046h, 0C7h, 044h, 024h, 028h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 034h, 004h
        db  000h, 000h, 000h, 08Dh, 044h, 024h, 014h, 089h, 044h, 024h, 02Ch, 0C7h, 044h, 024h, 030h, 000h
        db  000h, 000h, 000h, 08Dh, 07Ch, 024h, 018h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h
        db  018h, 050h, 08Dh, 044h, 024h, 014h, 050h, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 048h
        db  050h, 06Ah, 000h, 0A1h
        dd  obj1:0x3054              ; fixup
        db  0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 085h, 096h, 000h, 000h, 000h, 081h, 07Ch
        db  024h, 014h, 002h, 002h, 000h, 000h, 075h, 079h, 0C7h, 044h, 024h, 044h, 000h, 002h, 000h, 000h
        db  0C7h, 044h, 024h, 04Ch, 001h, 000h, 000h, 000h, 0C7h, 044h, 024h, 010h, 05Dh, 000h, 000h, 000h
        db  08Dh, 07Ch, 024h, 018h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 0C7h, 044h, 024h, 038h, 04Eh, 042h
        db  055h, 046h, 089h, 044h, 024h, 028h, 0C7h, 044h, 024h, 034h, 05Dh, 000h, 000h, 000h, 08Dh, 044h
        db  024h, 050h, 089h, 044h, 024h, 02Ch, 0C7h, 044h, 024h, 030h, 000h, 000h, 000h, 000h, 08Dh, 044h
        db  024h, 018h, 050h, 08Dh, 044h, 024h, 014h, 050h, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h
        db  048h, 050h, 06Ah, 000h, 0A1h
        dd  obj1:0x3054              ; fixup
        db  0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 074h, 005h, 083h, 0F8h, 009h, 075h, 01Ch, 08Bh
        db  044h, 024h, 050h, 039h, 046h, 004h, 074h, 01Eh, 045h, 083h, 0C3h, 008h, 03Bh, 0ACh, 024h, 0B8h
        db  000h, 000h, 000h, 00Fh, 082h, 0E1h, 0FEh, 0FFh, 0FFh, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h
        db  0C4h, 0A0h, 000h, 000h, 000h, 0C3h, 08Bh, 054h, 024h, 060h, 083h, 0EAh, 010h, 01Bh, 0C0h, 023h
        db  0D0h, 083h, 0C2h, 010h, 08Bh, 0BCh, 024h, 0C0h, 000h, 000h, 000h, 089h, 017h, 08Dh, 074h, 024h
        db  064h, 083h, 0C7h, 004h, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h, 003h
        db  0F3h, 0A4h, 08Bh, 044h, 024h, 054h, 083h, 0F8h, 001h, 074h, 01Bh, 083h, 0F8h, 006h, 074h, 022h
        db  083h, 0F8h, 009h, 074h, 029h, 083h, 0F8h, 00Fh, 074h, 024h, 08Bh, 084h, 024h, 0C4h, 000h, 000h
        db  000h, 0C6h, 000h, 007h, 0EBh, 022h, 08Bh, 084h, 024h, 0C4h, 000h, 000h, 000h, 0C6h, 000h, 007h
        db  0EBh, 016h, 08Bh, 084h, 024h, 0C4h, 000h, 000h, 000h, 0C6h, 000h, 001h, 0EBh, 00Ah, 08Bh, 084h
        db  024h, 0C4h, 000h, 000h, 000h, 0C6h, 000h, 006h, 08Bh, 04Ch, 024h, 050h, 08Bh, 084h, 024h, 0CCh
        db  000h, 000h, 000h, 089h, 008h, 08Bh, 084h, 024h, 0C8h, 000h, 000h, 000h, 0C7h, 000h, 001h, 000h
        db  000h, 000h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0A0h, 000h, 000h, 000h, 0C3h, 08Dh
        db  049h, 000h
sub_4_058C:
        cmp    dword ptr [0x28cc], 0         ; -> loc_1_28CC
        je     loc_4_05A8
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_4_05E8
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_4_05A8:
        cmp    dword ptr [0x2b6c], 0         ; -> loc_1_2B6C
        jne    loc_4_05CC
        mov    dword ptr [0x2b6c], 1         ; -> loc_1_2B6C
        push   dword ptr [esp + 8]
        push   0x5d0                         ; -> loc_4_05D0
        call   sub_1_1710
        add    esp, 8
loc_4_05CC:
        ret    
        db  08Dh, 049h, 000h
loc_4_05D0:
        cmp    dword ptr [0x3054], 0         ; -> obj1:0x3054
        je     loc_4_05E7
        push   dword ptr [esp + 4]
        push   0
        call   sub_4_05E8
        add    esp, 8
loc_4_05E7:
        ret    
sub_4_05E8:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], 0xffffffff
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [0x2b60]       ; -> loc_1_2B60
        cmp    ecx, 0x2b60                   ; -> loc_1_2B60
        je     loc_4_064E
loc_4_060E:
        lea    edx, [ecx - 0x39]
        mov    eax, dword ptr [edx + 0x41]
        cmp    esi, eax
        jl     loc_4_0638
        mov    ecx, dword ptr [edx + 0x39]
        mov    eax, dword ptr [edx + 0x3d]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esp + 4]
        push   eax
        push   edx
        call   dword ptr [edx + 0x49]
        add    esp, 8
        mov    ecx, dword ptr [0x2b60]       ; -> loc_1_2B60
        jmp    loc_4_0644
        db  090h
loc_4_0638:
        sub    eax, esi
        cmp    dword ptr [esp + 4], eax
        jbe    loc_4_0644
        mov    dword ptr [esp + 4], eax
loc_4_0644:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x2b60                   ; -> loc_1_2B60
        jne    loc_4_060E
loc_4_064E:
        push   0x3034                        ; -> loc_1_3034
        cmp    dword ptr [esp + 8], -1
        jne    loc_4_0664
        call   sub_1_0006
        add    esp, 4
        jmp    loc_4_06C1
loc_4_0664:
        call   sub_1_0006
        add    esp, 4
        mov    dword ptr [0x3040], 0         ; -> obj1:0x3040
        mov    dword ptr [0x3048], 0         ; -> obj1:0x3048
        mov    eax, dword ptr [0x28c8]       ; -> obj1:0x28C8
        mov    dword ptr [0x3044], eax       ; -> obj1:0x3044
        cmp    dword ptr [esp + 4], 0x15180
        jbe    loc_4_069C
        mov    dword ptr [esp + 4], 0x15180
loc_4_069C:
        push   0
        push   0x58c                         ; -> sub_4_058C
        mov    eax, dword ptr [esp + 0xc]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        push   eax
        push   0x3034                        ; -> loc_1_3034
        call   sub_1_0000
        add    esp, 0x10
loc_4_06C1:
        pop    esi
        mov    dword ptr [0x2b6c], 0         ; -> loc_1_2B6C
        add    esp, 4
        ret    
sub_4_06D0:
        push   esi
        movzx  esi, word ptr [esp + 8]
        push   esi
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_4_06FA
        cmp    dword ptr [0x28cc], 0         ; -> loc_1_28CC
        je     loc_4_06FA
        call   sub_1_002A
        push   esi
        call   sub_1_001E
        add    esp, 4
loc_4_06FA:
        pop    esi
        ret    
loc_4_06FC:
        push   dword ptr [esp + 4]
        push   1
        call   0
        add    esp, 8
        ret    
        db  090h
loc_4_070C:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     loc_4_07B0
        mov    edx, 0x28d0                   ; -> loc_1_28D0
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    loc_4_07B0
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_4_07B0
        push   0x3034                        ; -> loc_1_3034
        call   sub_1_0006
        add    esp, 4
        call   sub_1_0BD8
        mov    dword ptr [0x3054], 0         ; -> obj1:0x3054
        mov    edx, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        je     loc_4_078E
loc_4_075C:
        mov    esi, dword ptr [edx]
        mov    ecx, esi
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x304c]       ; -> loc_1_304C
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], 0x304c   ; -> loc_1_304C
        mov    eax, dword ptr [0x304c]       ; -> loc_1_304C
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [0x304c], edx       ; -> loc_1_304C
        mov    edx, esi
        cmp    esi, 0x2b84                   ; -> loc_1_2B84
        jne    loc_4_075C
loc_4_078E:
        mov    eax, dword ptr [0x2b60]       ; -> loc_1_2B60
        cmp    eax, 0x2b60                   ; -> loc_1_2B60
        je     loc_4_07B0
loc_4_079A:
        mov    edx, dword ptr [eax]
        mov    ecx, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    eax, ecx
        cmp    ecx, 0x2b60                   ; -> loc_1_2B60
        jne    loc_4_079A
loc_4_07B0:
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_4_07B4:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_4_07D6
        mov    edx, 0x28d8                   ; -> loc_1_28D8
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    loc_4_07D6
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_4_07D6
        call   sub_1_1130
loc_4_07D6:
        ret    
        db  090h
loc_4_07D8:
        push   esi
        cmp    dword ptr [esp + 0xc], 0
        je     loc_4_07E4
        sub    eax, eax
        pop    esi
        ret    
loc_4_07E4:
        mov    eax, dword ptr [0x2b60]       ; -> loc_1_2B60
        cmp    eax, 0x2b60                   ; -> loc_1_2B60
        mov    si, word ptr [esp + 8]
        je     loc_4_0817
loc_4_07F5:
        lea    edx, [eax - 0x39]
        mov    ecx, dword ptr [edx + 0x51]
        cmp    word ptr [ecx + 4], si
        je     loc_4_080C
        mov    eax, dword ptr [eax]
        cmp    eax, 0x2b60                   ; -> loc_1_2B60
        jne    loc_4_07F5
        jmp    loc_4_0817
loc_4_080C:
        mov    ecx, dword ptr [edx + 0x39]
        mov    eax, dword ptr [edx + 0x3d]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_4_0817:
        mov    edx, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        je     loc_4_0861
loc_4_0825:
        mov    eax, dword ptr [edx + 0x51]
        cmp    word ptr [eax + 4], si
        je     loc_4_083C
        mov    edx, dword ptr [edx]
        cmp    edx, 0x2b84                   ; -> loc_1_2B84
        jne    loc_4_0825
        sub    eax, eax
        pop    esi
        ret    
loc_4_083C:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx], 0x304c       ; -> loc_1_304C
        mov    eax, dword ptr [0x3050]       ; -> obj1:0x3050
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x3050]       ; -> obj1:0x3050
        mov    dword ptr [eax], edx
        mov    dword ptr [0x3050], edx       ; -> obj1:0x3050
loc_4_0861:
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_4_0868:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x2c]
        lea    esi, [ebx + 0x2c7]
        push   esi
        call   sub_4_0CD0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_4_0890
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_4_0890:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, esi
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    edi, [edx + 0x55]
        mov    dword ptr [edx + 0x51], edi
        add    edi, 0x32
        mov    dword ptr [edx + 0x4d], edi
        add    edi, 0x240
        mov    dword ptr [edx + 9], edi
        mov    al, byte ptr [esp + 0x24]
        mov    byte ptr [edx + 8], al
        mov    dword ptr [edx + 0xd], ebx
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x11], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 0x15], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edx + 0x19], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edx + 0x1d], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [edx + 0x21], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [edx + 0x25], eax
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [edx + 0x29], eax
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [edx + 0x2d], eax
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [edx + 0x31], eax
        mov    dword ptr [edx + 0x35], 0
        mov    ecx, dword ptr [edx + 0x51]
        mov    dword ptr [ecx], 0xffffffff
        mov    ax, word ptr [esp + 0x44]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [ecx + 0xa], eax
        mov    dword ptr [ecx + 0x12], 0
        lea    eax, [ecx + 0x2a]
        mov    dword ptr [ecx + 0x2e], eax
        mov    dword ptr [eax], eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h, 083h, 0ECh, 00Ch, 053h, 056h, 057h, 055h, 08Bh, 07Ch, 024h, 02Ch, 057h, 068h
        dd  loc_1_304C               ; fixup
        db  0E8h
        dd  sub_1_0364               ; fixup
        db  083h, 0C4h, 008h, 08Bh, 0F0h, 00Bh, 0F6h, 074h, 059h, 08Bh, 00Eh, 08Bh, 046h, 004h, 089h, 008h
        db  089h, 041h, 004h, 0C7h, 006h
        dd  loc_1_2B84               ; fixup
        db  0A1h
        dd  loc_1_2B88               ; fixup
        db  089h, 046h, 004h, 0A1h
        dd  loc_1_2B88               ; fixup
        db  089h, 030h, 089h, 035h
        dd  loc_1_2B88               ; fixup
        db  0C7h, 046h, 035h, 000h, 000h, 000h, 000h, 08Bh, 04Eh, 051h, 0C7h, 001h, 0FFh, 0FFh, 0FFh, 0FFh
        db  066h, 08Bh, 044h, 024h, 020h, 066h, 089h, 041h, 004h, 08Bh, 044h, 024h, 024h, 089h, 041h, 006h
        db  08Bh, 044h, 024h, 028h, 089h, 041h, 00Ah, 0C7h, 041h, 012h, 000h, 000h, 000h, 000h, 08Bh, 05Ch
        db  024h, 010h, 0EBh, 05Bh, 0FFh, 074h, 024h, 028h, 0FFh, 074h, 024h, 028h, 0FFh, 074h, 024h, 028h
        db  06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 0FFh, 037h, 08Dh, 047h, 004h, 050h
        db  0FFh, 074h, 024h, 058h, 06Ah, 000h, 06Ah, 0FFh, 06Ah, 000h, 06Ah, 000h, 068h
        dd  loc_1_2B84               ; fixup
        db  0E8h, 05Eh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 040h, 08Bh, 0D8h, 00Bh, 0DBh, 074h, 00Dh, 08Bh, 0C3h
        db  05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 08Dh, 049h, 000h, 08Bh, 035h
        dd  loc_1_2B88               ; fixup
        db  08Bh, 0C6h, 08Bh, 068h, 051h, 0E8h
        dd  loc_1_03A4               ; fixup
        db  089h, 045h, 00Eh, 083h, 03Fh, 006h, 075h, 024h, 08Bh, 047h, 004h, 03Bh, 005h
        dd  loc_1_2938               ; fixup
        db  075h, 019h, 0C7h, 046h, 011h, 0FFh, 0FFh, 0FFh, 0FFh, 0C7h, 046h, 021h, 0FFh, 0FFh, 0FFh, 0FFh
        db  08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 090h, 083h, 07Eh, 011h, 000h, 075h
        db  01Ah, 08Dh, 044h, 024h, 014h, 050h, 056h, 0E8h
        dd  sub_1_21B8               ; fixup
        db  083h, 0C4h, 008h, 08Bh, 0D8h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 08Dh
        db  044h, 024h, 018h, 050h, 056h, 0E8h
        dd  sub_1_22A4               ; fixup
        db  083h, 0C4h, 008h, 08Bh, 0D8h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 08Bh
        db  0FFh
sub_4_0A98:
        sub    ecx, ecx
        cmp    dword ptr [0x2940], ecx       ; -> loc_1_2940
        je     loc_4_0AB9
        mov    edx, 0x2940                   ; -> loc_1_2940
        inc    eax
        mov    eax, dword ptr [esp + 4]
loc_4_0AAC:
        cmp    dword ptr [edx], eax
        je     loc_4_0AB9
        add    edx, 8
        inc    ecx
        cmp    dword ptr [edx], 0
        jne    loc_4_0AAC
loc_4_0AB9:
        mov    eax, dword ptr [ecx*8 + 0x2944] ; -> obj1:0x2944
        ret    
        db  08Dh, 049h, 000h
sub_4_0AC4:
        push   esi
        push   edi
        cmp    dword ptr [0x2b78], 0         ; -> loc_1_2B78
        jne    loc_4_0AD4
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_4_0AD4:
        push   dword ptr [esp + 0xc]
        call   sub_4_0A98
        add    esp, 4
        or     eax, eax
        jne    loc_4_0AEC
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_4_0AEC:
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [0x2978]       ; -> loc_1_2978
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    esi, eax
        push   dword ptr [0x2978]            ; -> loc_1_2978
        call   sub_1_0F1D
        add    esp, 4
        push   0
        call   sub_4_0BE4
        add    esp, 4
        sub    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_4_0B3C:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x20]
        or     eax, eax
        mov    ebx, dword ptr [esp + 0x1c]
        je     loc_4_0B4D
        mov    dword ptr [eax], ebx
loc_4_0B4D:
        or     ebx, ebx
        mov    edx, dword ptr [esp + 0x14]
        jne    loc_4_0B70
        mov    edi, dword ptr [esp + 0x10]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_4_0B70:
        or     edx, edx
        jne    loc_4_0B80
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_4_0B80:
        cmp    ebx, edx
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0x18]
        jg     loc_4_0BBC
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    ebx, edx
        jge    loc_4_0BB6
        sub    edx, ebx
        mov    edi, dword ptr [esp + 0x10]
        add    edi, ebx
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_4_0BB6:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_4_0BBC:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_4_0BD4:
        call   sub_1_000C
        shr    eax, 3
        ret    
        db  08Dh, 049h, 000h, 0C3h, 08Dh, 049h, 000h
sub_4_0BE4:
        cmp    dword ptr [0x2984], 0         ; -> loc_1_2984
        jne    loc_4_0C01
        call   sub_2_0374
        mov    dword ptr [0x2984], eax       ; -> loc_1_2984
        call   sub_1_000C
        mov    dword ptr [0x2988], eax       ; -> loc_1_2988
loc_4_0C01:
        call   sub_1_000C
        sub    eax, dword ptr [0x2988]       ; -> loc_1_2988
        mov    ecx, 0x3e8
        sub    edx, edx
        div    ecx
        add    eax, dword ptr [0x2984]       ; -> loc_1_2984
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     loc_4_0C25
        mov    dword ptr [ecx], eax
loc_4_0C25:
        ret    
        db  08Bh, 0FFh
sub_4_0C28:
        sub    esp, 0x24
        push   edi
        cmp    dword ptr [0x28cc], 0         ; -> loc_1_28CC
        jne    loc_4_0C47
        cmp    dword ptr [esp + 0x2c], 0x3e8
        jbe    loc_4_0C47
        mov    dword ptr [esp + 0x2c], 0x3e8
loc_4_0C47:
        sub    eax, eax
        lea    edi, [esp + 4]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    dword ptr [esp + 4], 1
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0xc], eax
        lea    edi, [esp + 0x10]
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [0x28c8]       ; -> obj1:0x28C8
        mov    dword ptr [esp + 0x20], eax
        lea    eax, [esp + 4]
        push   eax
        push   0xcb0                         ; -> loc_4_0CB0
        push   dword ptr [esp + 0x34]
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_0000
        add    esp, 0x10
        or     eax, eax
        je     loc_4_0CA8
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_0036
        add    esp, 4
loc_4_0CA8:
        pop    edi
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
loc_4_0CB0:
        push   0
        push   dword ptr [esp + 0xc]
        call   sub_1_003C
        add    esp, 8
        ret    
        db  090h
sub_4_0CC0:
        push   dword ptr [esp + 4]
        call   sub_1_0024
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_4_0CD0:
        movzx  eax, word ptr [esp + 4]
        push   eax
        call   sub_1_001E
        add    esp, 4
        ret    
        db  090h
sub_4_0CE0:
        sub    eax, eax
        ret    
        db  090h
sub_4_0CE4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x51c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0xffffffff
        mov    dword ptr [ebp - 0xc], 0xffffffff
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x51]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x9c], eax
        mov    dword ptr [ebp - 0x98], 0x2a88 ; -> loc_1_2A88
        mov    dword ptr [ebp - 0x94], 0x80000002
        push   dword ptr [ebp - 0x9c]
        push   dword ptr [ebp - 0x98]
        push   dword ptr [ebp - 0x94]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0DFC
        cmp    eax, 2
        jne    loc_4_1090
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x90], eax
        mov    dword ptr [ebp - 0x8c], 0x2ab4 ; -> loc_1_2AB4
        mov    dword ptr [ebp - 0x88], 0x80000002
        push   dword ptr [ebp - 0x90]
        push   dword ptr [ebp - 0x8c]
        push   dword ptr [ebp - 0x88]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_1090
        mov    dword ptr [ebp - 0x84], 1
        mov    dword ptr [ebp - 0x80], 4
        lea    eax, [ebp - 0x84]
        mov    dword ptr [ebp - 0x7c], eax
        mov    dword ptr [ebp - 0x78], 3
        mov    dword ptr [ebp - 0x74], 0
        mov    dword ptr [ebp - 0x70], 0x2ae0 ; -> loc_1_2AE0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x6c], eax
        push   dword ptr [ebp - 0x80]
        push   dword ptr [ebp - 0x7c]
        push   dword ptr [ebp - 0x78]
        push   dword ptr [ebp - 0x74]
        push   dword ptr [ebp - 0x70]
        push   dword ptr [ebp - 0x6c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_4_1052
loc_4_0DFC:
        mov    dword ptr [ebp - 0x68], 4
        mov    dword ptr [ebp - 0x64], 0x2b78 ; -> loc_1_2B78
        mov    dword ptr [ebp - 0x60], 3
        mov    dword ptr [ebp - 0x5c], 0
        mov    dword ptr [ebp - 0x58], 0x2ae8 ; -> obj1:0x2AE8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x54], eax
        push   dword ptr [ebp - 0x68]
        push   dword ptr [ebp - 0x64]
        push   dword ptr [ebp - 0x60]
        push   dword ptr [ebp - 0x5c]
        push   dword ptr [ebp - 0x58]
        push   dword ptr [ebp - 0x54]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_4_1052
        mov    esi, 0x2af4                   ; -> loc_1_2AF4
        lea    edi, [ebp - 0x11c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xe]
        cmp    eax, 0x63
        jle    loc_4_0E74
        mov    esi, 0x57
        jmp    loc_4_1052
        db  090h
loc_4_0E74:
        mov    ecx, 0xa
        cdq    
        idiv   ecx
        add    al, 0x30
        mov    byte ptr [ebp - 0x114], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xe]
        cdq    
        idiv   ecx
        add    dl, 0x30
        mov    byte ptr [ebp - 0x113], dl
        mov    byte ptr [ebp - 0x112], 0
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x50], eax
        lea    eax, [ebp - 0x11c]
        mov    dword ptr [ebp - 0x4c], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x48], eax
        push   dword ptr [ebp - 0x50]
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        jne    loc_4_1052
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xe]
        mov    dword ptr [ebp - 0xdc], eax
        mov    eax, dword ptr [ebx + 0x11]
        mov    dword ptr [ebp - 0xd8], eax
        mov    eax, dword ptr [ebx + 0x15]
        mov    dword ptr [ebp - 0xd4], eax
        mov    eax, dword ptr [ebx + 0x19]
        mov    dword ptr [ebp - 0xd0], eax
        mov    eax, dword ptr [ebx + 0x1d]
        mov    dword ptr [ebp - 0xcc], eax
        mov    eax, dword ptr [ebx + 0x21]
        mov    dword ptr [ebp - 0xc8], eax
        mov    eax, dword ptr [ebx + 0x25]
        mov    dword ptr [ebp - 0xc4], eax
        mov    eax, dword ptr [ebx + 0x29]
        mov    dword ptr [ebp - 0xc0], eax
        mov    eax, dword ptr [ebx + 0x2d]
        mov    dword ptr [ebp - 0xbc], eax
        mov    eax, dword ptr [ebx + 0x31]
        mov    dword ptr [ebp - 0xb8], eax
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp - 0xb0], al
        mov    eax, dword ptr [ebx + 0xd]
        mov    dword ptr [ebp - 0xb4], eax
        mov    eax, dword ptr [ebx + 0xd]
        lea    edi, [ebp - 0xaf]
        mov    esi, dword ptr [ebx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [ebp - 0x44], 0x3d
        lea    eax, [ebp - 0xdc]
        mov    dword ptr [ebp - 0x40], eax
        mov    dword ptr [ebp - 0x3c], 3
        mov    dword ptr [ebp - 0x38], 0
        mov    dword ptr [ebp - 0x34], 0x2b00 ; -> loc_1_2B00
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        push   dword ptr [ebp - 0x38]
        push   dword ptr [ebp - 0x34]
        push   dword ptr [ebp - 0x30]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_4_1052
        lea    edx, [ebp - 0x51c]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 0x2a]
        sub    eax, ebx
        cmp    eax, -0x2a
        je     loc_4_0FEE
loc_4_0FC6:
        mov    edi, edx
        movzx  eax, byte ptr [ebx + 9]
        add    eax, 2
        lea    esi, [ebx + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, eax
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, ebx
        cmp    eax, -0x2a
        jne    loc_4_0FC6
loc_4_0FEE:
        mov    byte ptr [edx], 0xff
        mov    byte ptr [edx + 1], 0
        add    edx, 2
        lea    eax, [ebp - 0x51c]
        sub    edx, eax
        mov    dword ptr [ebp - 0x2c], edx
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x24], 3
        mov    dword ptr [ebp - 0x20], 0
        mov    dword ptr [ebp - 0x1c], 0x2b0c ; -> loc_1_2B0C
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x18], eax
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_4_1052
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x12], 0
loc_4_1052:
        cmp    dword ptr [ebp - 8], -1
        je     loc_4_1070
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_4_1070:
        cmp    dword ptr [ebp - 0xc], -1
        je     loc_4_108E
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_4_108E:
        mov    eax, esi
loc_4_1090:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_4_1098:
        push   esi
        mov    esi, dword ptr [0x2b84]       ; -> loc_1_2B84
        cmp    esi, 0x2b84                   ; -> loc_1_2B84
        je     loc_4_10C8
        nop    
loc_4_10A8:
        mov    eax, esi
        mov    ecx, dword ptr [esi + 0x51]
        mov    esi, dword ptr [esi]
        cmp    dword ptr [ecx + 0x12], 0
        je     loc_4_10C0
        push   0
        push   eax
        call   sub_4_0CE4
        add    esp, 8
loc_4_10C0:
        cmp    esi, 0x2b84                   ; -> loc_1_2B84
        jne    loc_4_10A8
loc_4_10C8:
        pop    esi
        mov    dword ptr [0x29fc], 0         ; -> obj1:0x29FC
        ret    
sub_4_10D4:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x51]
        mov    dword ptr [eax + 0x12], 1
        cmp    dword ptr [0x29fc], 0         ; -> obj1:0x29FC
        jne    loc_4_10FD
        push   0x1098                        ; -> loc_4_1098
        call   sub_1_0FE8
        add    esp, 4
        mov    dword ptr [0x2a00], eax       ; -> obj1:0x2A00
loc_4_10FD:
        sub    eax, eax
        ret    
sub_4_1100:
        push   0
        push   dword ptr [0x2b84]            ; -> loc_1_2B84
        call   sub_4_10D4
        add    esp, 8
        ret    
        db  08Dh, 049h, 032h
VxD__LTEXT ENDS
