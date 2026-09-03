; LMOUSE -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 17803 instructions recovered by recursive descent

Declare_Virtual_Device LMOUSE, 1, 0, LMOUSE_Control, 0318Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 03A000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1604  R-X
LMOUSE_Control:
        cmp    eax, 1
        je     loc_2_0423
        cmp    eax, 0x1b
        je     loc_2_040F
        cmp    eax, 0x1c
        je     loc_2_03EC
        cmp    eax, 0xf
        jne    loc_1_002E
        push   edx
        push   ebx
        call   sub_1_1254
        add    esp, 8
        cmp    eax, 1
        ret    
loc_1_002E:
        cmp    eax, 0
        jne    loc_1_0041
        push   edx
        push   ebx
        call   sub_2_0382
        add    esp, 8
        cmp    eax, 1
        ret    
loc_1_0041:
        cmp    eax, 4
        jne    loc_1_0053
        push   ebx
        call   sub_2_0392
        add    esp, 4
        cmp    eax, 1
        ret    
loc_1_0053:
        cmp    eax, 2
        jne    loc_1_0065
        push   ebx
        call   sub_2_03A2
        add    esp, 4
        cmp    eax, 1
        ret    
loc_1_0065:
        cmp    eax, 0x23
        jne    loc_1_007A
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_2_AB34
        add    esp, 0x10
        cmp    eax, 1
        ret    
loc_1_007A:
        clc    
        ret    
sub_1_007C:
        xor    ecx, ecx
        test   eax, 8
        je     loc_1_0087
        not    ecx
loc_1_0087:
        mov    dword ptr [0x1390], ecx       ; -> obj1:0x1390
        and    eax, 0xfffffff7
        VxDCall  VMD.Service_0003h               ; cd2003000c00
        ret    
        db  090h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_1394               ; fixup
loc_1_00A0:
        test   dword ptr [0x1390], 1         ; -> obj1:0x1390
        je     loc_1_00AF
        or     bh, 4
loc_1_00AF:
        jmp    loc_1_1394
        db  000h, 000h, 000h
loc_1_00B8:
        mov    edi, 0x1398                   ; -> loc_1_1398
        jmp    loc_1_00E8
        db  090h
loc_1_00C0:
        mov    edi, 0x13a4                   ; -> obj1:0x13A4
        jmp    loc_1_00E8
        db  090h
loc_1_00C8:
        mov    edi, 0x13b0                   ; -> obj1:0x13B0
        jmp    loc_1_00E8
        db  090h
loc_1_00D0:
        mov    edi, 0x13bc                   ; -> obj1:0x13BC
        jmp    loc_1_00E8
        db  090h
loc_1_00D8:
        mov    edi, 0x13c8                   ; -> obj1:0x13C8
        jmp    loc_1_00E8
        db  090h
loc_1_00E0:
        mov    edi, 0x13d4                   ; -> obj1:0x13D4
        jmp    loc_1_00E8
        db  090h
loc_1_00E8:
        push   ebp
        mov    ebp, esp
        push   ecx
        mov    ecx, edi
        mov    edi, dword ptr [edi + 8]
        push   edi
        call   dword ptr [ecx + 4]
        pop    edi
        cmp    ecx, 1
        je     loc_1_0110
        cmp    ecx, 2
        je     loc_1_010C
        mov    edi, dword ptr [edi]
        push   esi
        push   edi
        call   sub_1_0113
        add    esp, 8
loc_1_010C:
        clc    
loc_1_010D:
        pop    ecx
        pop    ebp
        ret    
loc_1_0110:
        stc    
        jmp    loc_1_010D
sub_1_0113:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_0133
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0715
        add    esp, 8
        or     eax, eax
        je     loc_1_015C
loc_1_0133:
        mov    eax, dword ptr [ebp + 8]
        lea    eax, [eax + 0xaf]
        cmp    dword ptr [eax + 8], 0
        jne    loc_1_015C
        mov    dword ptr [eax + 8], 0xffffffff
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0x15f                    ; -> loc_1_015F
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [eax + 0xc], esi
        mov    eax, esi
loc_1_015C:
        pop    esi
        leave  
        ret    
loc_1_015F:
        lea    edi, [edx + 0xaf]
        mov    dword ptr [edi + 8], 0
        mov    eax, 0xffffffff
        xchg   dword ptr [edi + 4], eax
        or     eax, eax
        jne    loc_1_018E
        mov    esi, edx
loc_1_017A:
        xor    eax, eax
        push   eax
        push   eax
        push   esi
        call   sub_1_08A0
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_017A
        mov    dword ptr [edi + 4], eax
loc_1_018E:
        ret    
        db  000h
loc_1_0190:
        lea    esi, [edi + 0x10]
        mov    edi, dword ptr [edi + 0x28]
        lea    edi, [edi]
        push   esi
        call   dword ptr [edi]
        pop    esi
        cmp    cl, 0x10
        je     loc_1_0252
        test   ecx, 1
        je     loc_1_01B2
        call   sub_1_0280
loc_1_01B2:
        test   cl, 0x20
        je     loc_1_01C7
        and    byte ptr [edi + 0x12], 0xfb
        mov    dword ptr [edi + 4], 0xffffff9c
        jmp    loc_1_024C
loc_1_01C7:
        test   ecx, 2
        je     loc_1_024C
        mov    ecx, 2
        shl    al, cl
        jae    loc_1_01F6
        test   byte ptr [edi + 0x12], 4
        je     loc_1_01E8
        cmp    dword ptr [edi + 4], 4
        je     loc_1_01E8
        and    byte ptr [edi + 0x12], 0xfb
loc_1_01E8:
        mov    dword ptr [edi + 4], 1
        shr    al, cl
        mov    byte ptr [edi + 0xf], al
        jmp    loc_1_024C
loc_1_01F6:
        inc    dword ptr [edi + 4]
        js     loc_1_024C
        cmp    dword ptr [edi + 4], 3
        je     loc_1_0211
        jg     loc_1_0258
        mov    ah, byte ptr [edi + 0xf]
        shr    ax, cl
        xchg   ah, al
        mov    word ptr [edi + 0xf], ax
        jmp    loc_1_024C
loc_1_0211:
        mov    ah, byte ptr [edi + 0xf]
        shr    ax, cl
        movsx  ebx, al
        mov    dword ptr [esi + 4], ebx
        movsx  ebx, byte ptr [edi + 0x10]
        mov    dword ptr [esi], ebx
        mov    al, ah
        and    eax, 3
        mov    al, byte ptr [eax + 0x13e0]   ; -> obj1:0x13E0
        mov    ah, byte ptr [edi + 0x12]
        and    ah, 0xfc
        or     al, ah
        mov    byte ptr [edi + 0x12], al
        and    eax, 7
        mov    dword ptr [esi + 8], eax
loc_1_023F:
        mov    dword ptr [esi + 0xc], 2
        mov    ecx, 3
        ret    
loc_1_024C:
        mov    ecx, 2
        ret    
loc_1_0252:
        mov    ecx, 1
        ret    
loc_1_0258:
        cmp    dword ptr [edi + 4], 4
        ja     loc_1_024C
        shr    al, 5
        and    al, 4
        mov    ah, byte ptr [edi + 0x12]
        and    ah, 0xfb
        or     al, ah
        mov    byte ptr [edi + 0x12], al
        and    eax, 7
        mov    dword ptr [esi + 8], eax
        xor    eax, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        jmp    loc_1_023F
        db  02Eh, 08Bh, 0C0h
sub_1_0280:
        push   eax
        push   ecx
        push   esi
        test   dword ptr [edi + 0x13], 1
        je     loc_1_02A3
        mov    cl, ch
        and    ecx, 0x20
        xor    ecx, dword ptr [edi + 0x27]
        jne    loc_1_02A3
        mov    edx, edi
        mov    esi, 0x2a8                    ; -> loc_1_02A8
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_1_02A3:
        pop    esi
        pop    ecx
        pop    eax
        ret    
        db  090h
loc_1_02A8:
        mov    edi, edx
        mov    dx, word ptr [edi + 0xc]
        add    dx, 6
        in     al, dx
        and    eax, 0x20
        xor    eax, dword ptr [edi + 0x27]
        jne    loc_1_02C7
        mov    edx, dword ptr [edi + 0x17]
        push   edx
        call   sub_2_2925
        add    esp, 4
loc_1_02C7:
        ret    
loc_1_02C8:
        lea    esi, [edi + 0x10]
        mov    edi, dword ptr [edi + 0x28]
        lea    edi, [edi]
        push   esi
        call   dword ptr [edi]
        pop    esi
        cmp    cl, 0x10
        je     loc_1_0364
        test   ecx, 0x20
        je     loc_1_02EE
        mov    dword ptr [edi + 4], 0xffffff9c
        jmp    loc_1_035E
loc_1_02EE:
        test   ecx, 2
        je     loc_1_035E
        test   al, 0x80
        jne    loc_1_0310
        inc    dword ptr [edi + 4]
        js     loc_1_035E
        cmp    dword ptr [edi + 4], 3
        je     loc_1_0326
        jb     loc_1_0321
        mov    dword ptr [edi + 4], 0xffffff9c
        jmp    loc_1_035E
loc_1_0310:
        and    al, 0x7f
        shl    al, 3
        mov    dword ptr [edi + 4], 1
        mov    byte ptr [edi + 0xf], al
        jmp    loc_1_035E
loc_1_0321:
        mov    byte ptr [edi + 0x10], al
        jmp    loc_1_035E
loc_1_0326:
        movsx  ebx, byte ptr [edi + 0x10]
        mov    ah, byte ptr [edi + 0xf]
        shl    ah, 1
        jb     loc_1_0333
        neg    ebx
loc_1_0333:
        mov    dword ptr [esi], ebx
        shl    ah, 1
        jae    loc_1_033B
        neg    al
loc_1_033B:
        movsx  ebx, al
        mov    dword ptr [esi + 4], ebx
        rol    ax, 3
        and    eax, 7
        mov    al, byte ptr [eax + 0x13e4]   ; -> obj1:0x13E4
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0xc], 2
        mov    ecx, 3
        ret    
loc_1_035E:
        mov    ecx, 2
        ret    
loc_1_0364:
        mov    ecx, 1
        ret    
        db  000h, 000h
loc_1_036C:
        push   ebx
        push   eax
        mov    dx, word ptr [edi + 0xc]
        add    dx, 2
        in     al, dx
        mov    ebx, eax
        test   al, 1
        pop    eax
        mov    cl, 0x10
        jne    loc_1_0405
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        xor    ecx, ecx
        and    ebx, 6
        jne    loc_1_03AE
        mov    dx, word ptr [edi + 0xc]
        in     al, dx
        jmp    loc_1_0398
loc_1_0398:
        jmp    loc_1_039A
loc_1_039A:
        jmp    loc_1_039C
loc_1_039C:
        jmp    loc_1_039E
loc_1_039E:
        mov    dx, word ptr [edi + 0xc]
        add    dx, 6
        in     al, dx
        mov    ch, al
        or     ecx, 1
        jmp    loc_1_0405
loc_1_03AE:
        mov    dx, word ptr [edi + 0xc]
        add    dx, 5
        in     al, dx
        mov    bh, al
        jmp    loc_1_03BB
loc_1_03BB:
        jmp    loc_1_03BD
loc_1_03BD:
        jmp    loc_1_03BF
loc_1_03BF:
        jmp    loc_1_03C1
loc_1_03C1:
        mov    dx, word ptr [edi + 0xc]
        add    edx, 6
        in     al, dx
        mov    bl, al
        jmp    loc_1_03CD
loc_1_03CD:
        jmp    loc_1_03CF
loc_1_03CF:
        jmp    loc_1_03D1
loc_1_03D1:
        jmp    loc_1_03D3
loc_1_03D3:
        mov    dx, word ptr [edi + 0xc]
        in     al, dx
        or     ecx, 2
        test   bh, 0x1e
        je     loc_1_03F0
        jmp    loc_1_03E2
loc_1_03E2:
        jmp    loc_1_03E4
loc_1_03E4:
        jmp    loc_1_03E6
loc_1_03E6:
        jmp    loc_1_03E8
loc_1_03E8:
        mov    dx, word ptr [edi + 0xc]
        in     al, dx
        or     ecx, 0x20
loc_1_03F0:
        test   bl, 8
        je     loc_1_0405
        jmp    loc_1_03F7
loc_1_03F7:
        jmp    loc_1_03F9
loc_1_03F9:
        jmp    loc_1_03FB
loc_1_03FB:
        jmp    loc_1_03FD
loc_1_03FD:
        mov    dx, word ptr [edi + 0xc]
        in     al, dx
        or     ecx, 0x20
loc_1_0405:
        pop    ebx
        ret    
        db  000h
loc_1_0408:
        lea    esi, [edi + 0x10]
        mov    edi, dword ptr [edi + 0x28]
        lea    edi, [edi]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    bx, word ptr [edi + 8]
        mov    dx, bx
        add    dx, 2
        mov    ecx, 4
        mov    al, 0x80
        out    dx, al
        jmp    loc_1_042B
loc_1_042B:
        jmp    loc_1_042D
loc_1_042D:
        jmp    loc_1_042F
loc_1_042F:
        jmp    loc_1_0431
loc_1_0431:
        jmp    loc_1_0433
loc_1_0433:
        xchg   bx, dx
        in     al, dx
        jmp    loc_1_0439
loc_1_0439:
        jmp    loc_1_043B
loc_1_043B:
        jmp    loc_1_043D
loc_1_043D:
        jmp    loc_1_043F
loc_1_043F:
        jmp    loc_1_0441
loc_1_0441:
        ror    ax, cl
        xchg   bx, dx
        mov    al, 0xa0
        out    dx, al
        jmp    loc_1_044C
loc_1_044C:
        jmp    loc_1_044E
loc_1_044E:
        jmp    loc_1_0450
loc_1_0450:
        jmp    loc_1_0452
loc_1_0452:
        jmp    loc_1_0454
loc_1_0454:
        xchg   bx, dx
        in     al, dx
        jmp    loc_1_045A
loc_1_045A:
        jmp    loc_1_045C
loc_1_045C:
        jmp    loc_1_045E
loc_1_045E:
        jmp    loc_1_0460
loc_1_0460:
        jmp    loc_1_0462
loc_1_0462:
        rol    ax, cl
        cbw    
        cwde   
        mov    dword ptr [esi], eax
        xchg   bx, dx
        mov    al, 0xc0
        out    dx, al
        jmp    loc_1_0472
loc_1_0472:
        jmp    loc_1_0474
loc_1_0474:
        jmp    loc_1_0476
loc_1_0476:
        jmp    loc_1_0478
loc_1_0478:
        jmp    loc_1_047A
loc_1_047A:
        xchg   bx, dx
        in     al, dx
        jmp    loc_1_0480
loc_1_0480:
        jmp    loc_1_0482
loc_1_0482:
        jmp    loc_1_0484
loc_1_0484:
        jmp    loc_1_0486
loc_1_0486:
        jmp    loc_1_0488
loc_1_0488:
        ror    ax, cl
        xchg   bx, dx
        mov    al, 0xe0
        out    dx, al
        jmp    loc_1_0493
loc_1_0493:
        jmp    loc_1_0495
loc_1_0495:
        jmp    loc_1_0497
loc_1_0497:
        jmp    loc_1_0499
loc_1_0499:
        jmp    loc_1_049B
loc_1_049B:
        xchg   bx, dx
        in     al, dx
        jmp    loc_1_04A1
loc_1_04A1:
        jmp    loc_1_04A3
loc_1_04A3:
        jmp    loc_1_04A5
loc_1_04A5:
        jmp    loc_1_04A7
loc_1_04A7:
        jmp    loc_1_04A9
loc_1_04A9:
        xchg   bx, dx
        rol    ax, cl
        push   eax
        cbw    
        cwde   
        mov    dword ptr [esi + 4], eax
        pop    eax
        ror    ax, 9
        not    eax
        and    eax, 7
        mov    al, byte ptr [eax + 0x13ec]   ; -> obj1:0x13EC
        mov    dword ptr [esi + 8], eax
        push   eax
        xor    al, al
        out    dx, al
        pop    eax
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edi], eax
        xor    ecx, eax
        or     ecx, dword ptr [esi]
        or     ecx, dword ptr [esi + 4]
        jecxz  loc_1_04E8
        mov    dword ptr [esi + 0xc], 2
        mov    ecx, 3
        ret    
loc_1_04E8:
        mov    ecx, 2
        ret    
        db  08Bh, 0C0h
loc_1_04F0:
        mov    eax, 0xffffffff
        ret    
        db  000h, 000h
loc_1_04F8:
        lea    esi, [edi + 0x10]
        mov    edi, dword ptr [edi + 0x28]
        lea    edi, [edi]
        push   eax
        cmp    dword ptr [0x13f8], 0         ; -> obj1:0x13F8
        jne    loc_1_05C5
        cmp    dword ptr [0x13f4], 0         ; -> obj1:0x13F4
        je     loc_1_0539
        mov    dx, 0x311
        in     al, dx
        test   al, 2
        je     loc_1_0539
        mov    ah, al
        mov    dx, 0x310
        in     al, dx
        test   ah, 0x20
        je     loc_1_0549
        cmp    al, 0xaa
        je     loc_1_0549
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_0582
loc_1_0539:
        in     al, 0x64
        test   al, 1
        je     loc_1_05C5
        test   al, 0x20
        je     loc_1_05C5
        in     al, 0x60
loc_1_0549:
        mov    edx, dword ptr [edi + 4]
        shl    edx, 8
        mov    dl, al
        mov    dword ptr [edi + 4], edx
        inc    dword ptr [edi]
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ebx, eax
        xchg   dword ptr [edi + 8], eax
        sub    ebx, eax
        cmp    ebx, 0x5a
        jb     loc_1_0575
        mov    dword ptr [edi], 1
loc_1_0575:
        cmp    dword ptr [edi], 3
        je     loc_1_0584
        jb     loc_1_0582
        mov    dword ptr [edi], 0
loc_1_0582:
        jmp    loc_1_05BF
loc_1_0584:
        mov    eax, edx
        shr    eax, 0x10
        mov    dword ptr [edi], 0
        mov    cl, dh
        bt     ax, 4
        sbb    ch, ch
        movsx  ecx, cx
        mov    dword ptr [esi], ecx
        bt     ax, 5
        sbb    dh, dh
        movsx  edx, dx
        neg    edx
        mov    dword ptr [esi + 4], edx
        and    eax, 7
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0xc], 2
        mov    ecx, 3
        ret    
loc_1_05BF:
        mov    ecx, 2
        ret    
loc_1_05C5:
        pop    eax
        mov    ecx, 1
        ret    
loc_1_05CC:
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ah, 0xc2
        jne    loc_1_05DD
        cmp    al, 7
        jbe    loc_2_39DC
loc_1_05DD:
        stc    
        ret    
        db  0F8h, 0C3h, 000h, 000h, 000h
sub_1_05E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [ebx + 8]
        mov    ecx, 8
        push   ecx
        push   edi
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    esi
        mov    eax, esi
        mov    edi, dword ptr [ebp + 0x10]
        pop    ecx
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [eax + 0x2c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0645:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [ebx + 8]
        mov    ebx, edi
        mov    ecx, 8
        push   ecx
        push   edi
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [ebp + 0x14]
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 0x3c], ax
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebx + 0x38], ax
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebx + 0x3c]
        mov    word ptr [esi], ax
        mov    ax, word ptr [ebx + 0x38]
        mov    word ptr [esi + 2], ax
        pop    esi
        mov    eax, esi
        mov    edi, dword ptr [ebp + 0x10]
        pop    ecx
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [eax + 0x2c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_1_06CC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, esi
        imul   edi, dword ptr [ebp + 0xc]
        push   1
        push   edi
        call   sub_1_12F4
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 8
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ecx + 8], eax
        add    eax, edi
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 0x10], esi
        cmp    dword ptr [ecx], 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_1_0702:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   dword ptr [eax]
        call   sub_1_12FC
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_0715:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [edx + 8]
        mov    ebx, dword ptr [edx + 0x10]
        add    ebx, edi
        cmp    dword ptr [edx + 4], ebx
        jne    loc_1_0732
        mov    ebx, dword ptr [edx]
loc_1_0732:
        cmp    dword ptr [edx + 0xc], ebx
        je     loc_1_0755
        mov    eax, 1
        mov    ecx, dword ptr [edx + 0x10]
        mov    dword ptr [ebp - 4], ecx
        mov    esi, dword ptr [ebp + 0xc]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [edx + 8], ebx
loc_1_0755:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_075C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    esi, dword ptr [edx + 0xc]
        cmp    dword ptr [edx + 8], esi
        je     loc_1_079B
        mov    eax, 1
        mov    ebx, dword ptr [edx + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [edx + 0xc]
        add    ecx, dword ptr [edx + 0x10]
        mov    dword ptr [edx + 0xc], ecx
        cmp    dword ptr [edx + 4], ecx
        jne    loc_1_079B
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edx + 0xc], ecx
loc_1_079B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_07A0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    esi, esi
        push   1
        push   0x14
        call   sub_1_12F4
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 8
        mov    dword ptr [ecx], eax
        mov    ecx, eax
        cmp    ecx, esi
        je     loc_1_07CD
        push   0x10
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_1_06CC
        add    esp, 0xc
        mov    esi, eax
loc_1_07CD:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
sub_1_07D2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0702
        add    esp, 4
        push   0
        push   esi
        call   sub_1_12FC
        add    esp, 8
        pop    esi
        pop    ebp
        ret    
sub_1_07F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + eax*4]
        pop    ebp
        ret    
sub_1_0801:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [eax], 0
        je     loc_1_0830
        mov    edi, 0                        ; -> loc_1_1404
loc_1_0817:
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi]
        cmp    dword ptr [eax], ecx
        je     loc_1_0825
        add    edi, 0x20
        jmp    loc_1_0817
loc_1_0825:
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [eax], 0
        jmp    loc_1_085B
loc_1_0830:
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        mov    edi, 0                        ; -> loc_1_1404
        jne    loc_1_0852
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0x10]
        call   sub_1_075C
        add    esp, 8
        or     eax, eax
        jne    loc_1_085B
        sub    eax, eax
        jmp    loc_1_089B
loc_1_0852:
        mov    ecx, edi
        add    edi, 0x20
        cmp    dword ptr [ecx], eax
        jne    loc_1_0852
loc_1_085B:
        mov    ebx, dword ptr [ebp + 0x14]
        cmp    dword ptr [edi], ebx
        je     loc_1_0896
loc_1_0862:
        cmp    edi, 0x180                    ; -> loc_1_1584
        jae    loc_1_0896
        test   byte ptr [edi + 4], 4
        jne    loc_1_088F
        push   dword ptr [ebp + 0xc]
        mov    eax, edi
        sub    eax, 0                        ; -> loc_1_1404
        and    eax, 0xffffffe7
        sar    eax, 3
        mov    ecx, dword ptr [esi + 0x24]
        push   dword ptr [eax + ecx]
        call   dword ptr [edi]
        add    esp, 8
        or     eax, eax
        je     loc_1_0896
loc_1_088F:
        add    edi, 0x20
        cmp    dword ptr [edi], ebx
        jne    loc_1_0862
loc_1_0896:
        mov    eax, 1
loc_1_089B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_08A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xc]
        cmp    dword ptr [eax + 4], 0
        mov    eax, 0
        jne    loc_1_08C6
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ecx + 8]
        push   ecx
        call   sub_1_0801
        add    esp, 0x10
loc_1_08C6:
        pop    ebp
        ret    
sub_1_08C8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret    
sub_1_08D8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        or     edi, edi
        jne    loc_1_08F4
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0113
        add    esp, 8
        jmp    loc_1_08F7
loc_1_08F4:
        mov    esi, dword ptr [ebp + 8]
loc_1_08F7:
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 4], edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
loc_1_0904:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        sub    esi, esi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_0913:
        mov    eax, dword ptr [ecx + 8]
        or     eax, eax
        je     loc_1_092E
        test   al, 1
        je     loc_1_0920
        or     edi, dword ptr [edx]
loc_1_0920:
        shr    eax, 1
        mov    dword ptr [ecx + 8], eax
        add    edx, 4
        inc    esi
        cmp    esi, 0x20
        jb     loc_1_0913
loc_1_092E:
        mov    dword ptr [ecx + 8], edi
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h
loc_1_093C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi], 0
        je     loc_1_09EB
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0xc], 1
        jne    loc_1_09EB
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0x28]
        imul   ecx
        mov    ebx, eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x20]
        imul   ecx
        sub    ebx, eax
        add    ebx, dword ptr [edi + 0xc]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x18]
        imul   ecx
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        imul   ecx
        mov    ecx, dword ptr [edi + 0x10]
        add    ecx, eax
        add    ecx, dword ptr [ebp - 0xc]
        mov    eax, ebx
        and    eax, 0xffff
        mov    dword ptr [edi + 0xc], eax
        mov    eax, ecx
        and    eax, 0xffff
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [ebp - 8], ebx
        movzx  eax, word ptr [ebp - 6]
        movsx  eax, ax
        mov    dword ptr [esi], eax
        mov    dword ptr [ebp - 4], ecx
        movzx  eax, word ptr [ebp - 2]
        movsx  eax, ax
        mov    dword ptr [esi + 4], eax
loc_1_09EB:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
loc_1_09F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        je     loc_1_0A71
        push   1
        push   dword ptr [esi]
        call   sub_1_07F0
        add    esp, 8
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_0A71
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esi + 4]
        mov    edx, dword ptr [ecx + 8]
        xor    edi, edx
        mov    dword ptr [esi + 4], edx
        mov    eax, dword ptr [esi + 0xc]
        or     eax, dword ptr [esi + 8]
        mov    ebx, edx
        not    ebx
        and    eax, ebx
        test   edi, eax
        je     loc_1_0A4F
        mov    eax, dword ptr [esi + 0x14]
        and    eax, edi
        test   dword ptr [esi + 8], eax
        jne    loc_1_0A4F
        mov    dword ptr [esi + 0x10], 0
        mov    dword ptr [esi + 0x14], 0
loc_1_0A4F:
        mov    eax, dword ptr [esi + 8]
        and    eax, edx
        and    eax, edi
        je     loc_1_0A61
        xor    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [esi + 0x10], eax
loc_1_0A61:
        mov    eax, dword ptr [esi + 8]
        not    eax
        or     eax, dword ptr [esi + 0xc]
        and    eax, edx
        or     eax, dword ptr [esi + 0x10]
        mov    dword ptr [ecx + 8], eax
loc_1_0A71:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  000h
loc_1_0A7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        test   byte ptr [edi + 0xc], 1
        jne    loc_1_0B0A
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ebp - 0x2c]
        imul   ecx
        lea    ecx, [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 0x20], edi
        mov    dword ptr [ebp - 0x24], 0x320
        mov    ecx, dword ptr [ecx]
        add    ecx, eax
        mov    dword ptr [ebp - 0x28], ecx
        mov    eax, dword ptr [ebp - 0x28]
        cdq    
        idiv   dword ptr [ebp - 0x24]
        mov    ebx, dword ptr [ebp - 0x20]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebx], edx
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 0x14], eax
        add    edi, 4
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        imul   ecx
        add    esi, 0x10
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 0xc], 0x320
        mov    ecx, dword ptr [esi]
        add    ecx, eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        cdq    
        idiv   dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx], edx
loc_1_0B0A:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h
loc_1_0B18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 1
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [esi + 8], 0
        je     loc_1_0B89
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_0B89
        push   1
        push   ebx
        call   sub_1_07F0
        add    esp, 8
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_0B89
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_0B93
        cmp    eax, 1
        je     loc_1_0C00
        cmp    eax, 2
        je     loc_1_0D11
        cmp    eax, 3
        je     loc_1_0D3E
        cmp    eax, 4
        je     loc_1_0D59
        mov    edi, dword ptr [ebp + 0xc]
loc_1_0B78:
        test   byte ptr [edi + 0xc], 1
        je     loc_1_0B89
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x1c], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x20], eax
loc_1_0B89:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0B93:
        mov    dword ptr [esi + 0x10], 0
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, ecx
        mov    edx, dword ptr [edi + 8]
        and    eax, edx
        cmp    eax, ecx
        jne    loc_1_0BC3
        not    ecx
        and    ecx, edx
        mov    dword ptr [edi + 8], ecx
        mov    eax, dword ptr [esi + 0xc]
        or     eax, ecx
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 0x14], 2
        jmp    loc_1_0B78
loc_1_0BC3:
        or     eax, eax
        je     loc_1_0B78
        and    edx, ecx
        mov    dword ptr [esi + 0x10], edx
        mov    dword ptr [esi + 0x14], 1
        push   1
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        push   0                             ; -> loc_1_0B18
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0B78
loc_1_0C00:
        push   0                             ; -> loc_1_0B18
        push   0
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   ebx
        call   sub_1_0801
        add    esp, 0x10
        mov    dword ptr [esi + 0x24], eax
        or     eax, eax
        je     loc_1_0CA5
        mov    eax, dword ptr [esi + 0x10]
        cmp    eax, dword ptr [edi + 8]
        jne    loc_1_0C72
        test   byte ptr [edi + 0xc], 1
        jne    loc_1_0C4C
        mov    eax, dword ptr [edi]
        cdq    
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jge    loc_1_0C46
        mov    eax, dword ptr [edi + 4]
        cdq    
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jl     loc_1_0C6B
loc_1_0C46:
        test   byte ptr [edi + 0xc], 1
        je     loc_1_0C72
loc_1_0C4C:
        mov    eax, dword ptr [esi + 0x1c]
        sub    eax, dword ptr [edi]
        cdq    
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jge    loc_1_0C72
        mov    eax, dword ptr [esi + 0x20]
        sub    eax, dword ptr [edi + 4]
        cdq    
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jge    loc_1_0C72
loc_1_0C6B:
        mov    dword ptr [esi + 0x24], 0
loc_1_0C72:
        mov    dword ptr [ebp - 4], 1
        cmp    dword ptr [esi + 0x24], 0
        je     loc_1_0CFA
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [edi + 8]
        mov    edx, eax
        and    edx, ecx
        cmp    edx, eax
        jne    loc_1_0CCE
        not    eax
        and    eax, ecx
        mov    dword ptr [edi + 8], eax
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x14], 2
        jmp    loc_1_0D01
loc_1_0CA5:
        mov    eax, dword ptr [esi + 0x28]
        or     eax, eax
        lea    ecx, [eax - 1]
        mov    dword ptr [esi + 0x28], ecx
        je     loc_1_0C72
        push   0                             ; -> loc_1_0B18
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0B78
loc_1_0CCE:
        mov    eax, dword ptr [esi + 0x10]
        mov    edx, ecx
        not    edx
        test   eax, edx
        je     loc_1_0CFA
        xor    ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x14], 4
        push   0                             ; -> loc_1_0B18
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0xc
        jmp    loc_1_0B78
loc_1_0CFA:
        mov    dword ptr [esi + 0x14], 3
loc_1_0D01:
        push   0
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        jmp    loc_1_0B78
loc_1_0D11:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 8]
        test   eax, ecx
        jne    loc_1_0D2A
        mov    dword ptr [esi + 0x14], 0
        jmp    loc_1_0B78
loc_1_0D2A:
        not    ecx
        and    ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    eax, dword ptr [esi + 0xc]
        or     eax, ecx
        mov    dword ptr [edi + 8], eax
        jmp    loc_1_0B78
loc_1_0D3E:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 8]
        test   dword ptr [esi + 0x10], eax
        jne    loc_1_0B78
        mov    dword ptr [esi + 0x14], 0
        jmp    loc_1_0B78
loc_1_0D59:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        xor    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 0x14], 0
        push   0
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        jmp    loc_1_0B78
        db  000h, 000h, 000h
loc_1_0D7C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [esi + 8], 0
        je     loc_1_0EAE
        push   1
        push   ebx
        call   sub_1_07F0
        add    esp, 8
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_0EAE
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_0DD2
        cmp    eax, 1
        je     loc_1_0E2B
        cmp    eax, 2
        je     loc_1_0E4D
        cmp    eax, 3
        je     loc_1_0E6C
        cmp    eax, 4
        je     loc_1_0E89
        jmp    loc_1_0EAE
loc_1_0DD2:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 8]
        test   dword ptr [edi + 8], eax
        je     loc_1_0EAE
        push   1
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        inc    dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0xc]
        or     eax, dword ptr [esi + 8]
        not    eax
        and    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 4], eax
        test   byte ptr [edi + 0xc], 1
        jne    loc_1_0E10
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], 0
loc_1_0E10:
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 8], eax
        push   0                             ; -> loc_1_0D7C
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0xc
        jmp    loc_1_0EAE
loc_1_0E2B:
        inc    eax
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [ebp + 0xc]
        or     eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 8], eax
        push   0                             ; -> loc_1_0D7C
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0xc
        jmp    loc_1_0EAE
loc_1_0E4D:
        inc    eax
        mov    dword ptr [esi + 0x10], eax
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 8], eax
        push   0                             ; -> loc_1_0D7C
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0xc
        jmp    loc_1_0EAE
loc_1_0E6C:
        inc    eax
        mov    dword ptr [esi + 0x10], eax
        push   0
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [ebp + 0xc]
        or     eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 8], eax
        jmp    loc_1_0EAE
loc_1_0E89:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 8]
        test   eax, ecx
        je     loc_1_0EA7
        not    ecx
        and    ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    eax, dword ptr [esi + 0xc]
        or     eax, ecx
        mov    dword ptr [edi + 8], eax
        jmp    loc_1_0EAE
loc_1_0EA7:
        mov    dword ptr [esi + 0x10], 0
loc_1_0EAE:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0EB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        cmp    dword ptr [eax], 0
        je     loc_1_0F32
        call   sub_1_1297
        mov    eax, dword ptr [eax + 4]
        movzx  esi, word ptr [eax + 0x417]
        test   esi, 0x30f
        je     loc_1_0F32
        sub    edi, edi
        sub    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, 0                        ; -> loc_1_1584
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0EF8:
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [ebp - 4], eax
        je     loc_1_0F18
        movzx  ebx, word ptr [edx]
        and    ebx, esi
        movzx  eax, word ptr [edx + 2]
        xor    ebx, eax
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        or     edi, eax
loc_1_0F18:
        add    edx, 4
        inc    ecx
        cmp    edx, 0x18                     ; -> loc_1_159C
        jb     loc_1_0EF8
        or     edi, edi
        je     loc_1_0F32
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 1
loc_1_0F32:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h
sub_1_0F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        cdq    
        xor    eax, edx
        sub    eax, edx
        mov    ecx, dword ptr [esi]
        imul   ecx, ecx, 0x41
        movzx  edx, byte ptr [esi + ecx + 4]
        dec    edx
        add    ecx, esi
        lea    ecx, [edx + ecx + 5]
        mov    edi, edx
        movzx  ebx, byte ptr [ecx]
        cmp    ebx, eax
        jbe    loc_1_0F7E
loc_1_0F70:
        or     edi, edi
        je     loc_1_0F7E
        dec    edi
        movzx  ebx, byte ptr [ecx - 1]
        dec    ecx
        cmp    ebx, eax
        ja     loc_1_0F70
loc_1_0F7E:
        cmp    edi, edx
        jb     loc_1_0F87
        mov    dword ptr [ebp - 4], edi
        jmp    loc_1_0F8D
loc_1_0F87:
        lea    ecx, [edi + 1]
        mov    dword ptr [ebp - 4], ecx
loc_1_0F8D:
        mov    dword ptr [esi + 0x108], 0
        mov    ebx, dword ptr [esi]
        imul   ebx, ebx, 0x41
        lea    ecx, [esi + edi]
        lea    edx, [ebx + ecx]
        add    ebx, esi
        mov    ecx, dword ptr [ebp - 4]
        add    ebx, ecx
        mov    cl, byte ptr [edx + 5]
        cmp    byte ptr [ebx + 5], cl
        je     loc_1_0FF2
        movzx  ebx, byte ptr [ebx + 0x25]
        movzx  ecx, byte ptr [edx + 0x25]
        sub    ebx, ecx
        movzx  ecx, byte ptr [edx + 5]
        sub    eax, ecx
        imul   ebx, eax
        shl    ebx, 4
        mov    eax, ebx
        mov    dword ptr [esi + 0x108], eax
        mov    edx, dword ptr [esi]
        imul   edx, edx, 0x41
        lea    ebx, [esi + edx]
        mov    ecx, dword ptr [ebp - 4]
        movzx  ebx, byte ptr [ebx + ecx + 5]
        lea    ecx, [esi + edi]
        movzx  ecx, byte ptr [edx + ecx + 5]
        sub    ebx, ecx
        cdq    
        idiv   ebx
        mov    dword ptr [esi + 0x108], eax
loc_1_0FF2:
        mov    dword ptr [ebp - 0x20], 0x10
        mov    eax, dword ptr [esi]
        imul   eax, eax, 0x41
        add    eax, edi
        movzx  eax, byte ptr [eax + esi + 0x25]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x20]
        imul   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ebp - 0x18], ecx
        mov    ecx, dword ptr [esi + 0x108]
        add    ecx, eax
        mov    dword ptr [ebp - 0x1c], ecx
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x18]
        imul   ecx
        add    esi, 0x10c
        mov    dword ptr [ebp - 8], esi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 0x10], 0x10
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        cdq    
        idiv   dword ptr [ebp - 0x10]
        mov    ebx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_105C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        test   byte ptr [edi + 0xc], 1
        jne    loc_1_1084
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0F40
        add    esp, 8
        add    edi, 4
        push   edi
        push   esi
        call   sub_1_0F40
        add    esp, 8
loc_1_1084:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
loc_1_1090:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        test   byte ptr [ecx + 0xc], 1
        jne    loc_1_1119
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    loc_1_1119
        add    esi, 4
        mov    dword ptr [ebp - 0x14], esi
        mov    dword ptr [ebp - 0x18], ecx
        mov    dword ptr [ebp - 0x1c], 2
        mov    edx, dword ptr [ecx]
        mov    eax, edx
        and    eax, 0x8000
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        dec    eax
        add    eax, edx
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        cdq    
        idiv   dword ptr [ebp - 0x1c]
        mov    ebx, dword ptr [ebp - 0x18]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 0x14]
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebp - 4], esi
        add    ecx, 4
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0xc], 2
        mov    ecx, dword ptr [ecx]
        mov    eax, ecx
        and    eax, 0x8000
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        dec    eax
        add    eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        cdq    
        idiv   dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx], edx
loc_1_1119:
        mov    eax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1124:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret    
        db  000h, 000h, 000h
sub_1_113C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_1149:
        mov    edx, eax
        and    dl, 1
        add    dl, byte ptr [esi]            ; -> loc_1_159C
        mov    byte ptr [esi], dl            ; -> loc_1_159C
        je     loc_1_1169
        mov    ebx, ecx
        and    bl, 1
        sub    dl, bl
        mov    byte ptr [esi], dl            ; -> loc_1_159C
loc_1_1169:
        shr    eax, 1
        shr    ecx, 1
        inc    esi
        cmp    esi, 3
        jb     loc_1_1149
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_1177:
        sub    eax, eax
        mov    ecx, 2
        sub    edx, edx
loc_1_1180:
        add    eax, eax
        cmp    byte ptr [ecx], dl            ; -> loc_1_159C
        je     loc_1_118D
        or     eax, 1
loc_1_118D:
        dec    ecx
        jns    loc_1_1180
        ret    
loc_1_1191:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 8]
        and    edx, 7
        mov    dword ptr [edi + 8], edx
        mov    ecx, dword ptr [esi]
        mov    eax, edx
        xor    eax, ecx
        je     loc_1_11C0
        and    ecx, eax
        push   ecx
        and    edx, eax
        push   edx
        call   sub_1_113C
        add    esp, 8
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi], eax
loc_1_11C0:
        call   sub_1_1177
        mov    dword ptr [edi + 8], eax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_1_11D4:
        mov    eax, dword ptr [0x15f8]       ; -> loc_1_15F8
        ret    
loc_1_11DA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [0x15f8], edx       ; -> loc_1_15F8
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        and    eax, 7
        mov    al, byte ptr [eax + 0x15f0]   ; -> loc_1_15F0
        call   sub_1_007C
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h
loc_1_120C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   eax
        call   sub_1_0801
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1229:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_08C8
        add    esp, 8
        push   esi
        push   dword ptr [ebp + 0xc]
        push   0                             ; -> loc_1_120C
        call   sub_1_1310
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h
sub_1_1254:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, 0xc
        je     loc_1_1263
        or     ecx, ecx
        jne    loc_1_126B
loc_1_1263:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_15FC
loc_1_126B:
        cmp    ecx, 0xd
        je     loc_1_1274
        or     ecx, ecx
        jne    loc_1_127C
loc_1_1274:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [4], eax            ; -> loc_1_1600
loc_1_127C:
        mov    eax, 1
        pop    ebp
        ret    
        db  0A1h
        dd  loc_1_15FC               ; fixup
        db  00Bh, 0C0h, 075h, 00Ah, 0E8h, 073h, 000h, 000h, 000h, 0A3h
        dd  loc_1_15FC               ; fixup
        db  0C3h
sub_1_1297:
        mov    eax, dword ptr [4]            ; -> loc_1_1600
        or     eax, eax
        jne    loc_1_12AA
        call   sub_1_1304
        mov    dword ptr [4], eax            ; -> loc_1_1600
loc_1_12AA:
        ret    
        db  000h
sub_1_12AC:
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e801700
        add    byte ptr [eax], al
sub_1_12B4:
        VxDCall  SHELL._SHELL_CallDll            ; cd2018801700
        add    byte ptr [eax], al
sub_1_12BC:
        VxDCall  SHELL._SHELL_HookSystemBroadcast ; cd2011801700
        add    byte ptr [eax], al
loc_1_12C4:
        VxDCall  SHELL._SHELL_UnhookSystemBroadcast ; cd2012801700
        add    byte ptr [eax], al
sub_1_12CC:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_1_12D4:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_1_12DC:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_1_12E4:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_1_12EC:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_1_12F4:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_12FC:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_1304:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_1_1310:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_1_132C:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
        add    byte ptr [eax], al
sub_1_1334:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0318Ah                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'LMOUSE  '                  ; DDB_Name
        dd  03A000000h                  ; DDB_Init_Order
        dd  LMOUSE_Control           ; DDB_Control_Proc
        dd  LMOUSE_PM_API            ; DDB_V86_API_Proc
        dd  LMOUSE_PM_API            ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_1394:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1398:
        mov    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al                        ; -> loc_1_00C0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, cl                        ; -> loc_1_00C8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, dl                        ; -> loc_1_00D0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, bl                        ; -> loc_1_00D8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_00E0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [ecx]
        add    eax, dword ptr [eax]
        add    al, byte ptr [esi + eax]
        add    dword ptr [ebx], eax
        add    eax, 0x4020007
        push   es
        add    dword ptr [ebx], eax
        add    eax, 7
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    al, 3
        add    eax, dword ptr [esp + eax]
        add    al, 0
loc_1_1404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_140C:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1414:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_1418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_141C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_1420:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1448:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [eax], al            ; -> loc_2_98FB
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_9954
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_98BC
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_98CD
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_98E5
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0B18
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1508:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1528:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1584:
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        or     byte ptr [edx], al
        or     byte ptr [eax], al
        or     byte ptr [edx], al
        or     byte ptr [edx], al
        add    al, 1
        add    al, 0
        add    al, 1
        add    al, 1
loc_1_159C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        jne    0x1617
        je     0x1614
        outsb  dx, byte ptr [esi]
        jae    0x15c8
        outsd  dx, dword ptr fs:[esi]
        ja     0x161a
        das    
        jne    0x161f
        and    byte ptr [ebp + 0x6c], ah
        je     0x1616
        cmp    ah, byte ptr [eax]
        and    eax, 0x25202c78
        js     0x15c8
        add    byte ptr [eax], al
        jb     0x1629
        bound  eax, qword ptr [edx + 0x6c]
        outsb  dx, byte ptr gs:[esi]
        inc    edx
        jne    0x163f
        je     0x163c
        outsb  dx, byte ptr [esi]
        pop    ebx
        and    eax, 0x203a5d64
        and    eax, 0x52000a64
        jae    0x1651
        insb   byte ptr es:[edi], dx
        je     0x1648
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + si + 0x75], ah
        je     0x165a
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    0x1624
        and    byte ptr [0xa78], ah
loc_1_15F0:
        add    byte ptr [eax], ah
        adc    byte ptr [eax], dh
        or     byte ptr [eax], ch
        sbb    byte ptr [eax], bh
loc_1_15F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xad66  R-X
        db  0E8h, 0A5h, 041h, 000h, 000h, 0E8h, 01Dh, 047h, 000h, 000h, 0E8h, 005h, 058h, 000h, 000h, 0E8h
        db  000h, 089h, 000h, 000h, 0FFh, 035h
        dd  obj3:0x000C              ; fixup
        db  0E8h
        dd  loc_1_12C4               ; fixup
        db  083h, 0C4h, 004h, 0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_2_0028:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], 1
        call   sub_2_45E2
        or     eax, eax
        je     loc_2_0080
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        je     loc_2_0066
        push   dword ptr [esi + 8]
        call   sub_2_4619
        add    esp, 4
        or     eax, eax
        je     loc_2_0072
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 8]
        call   sub_2_4399
        add    esp, 8
        jmp    loc_2_0072
loc_2_0066:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_43B7
        add    esp, 4
loc_2_0072:
        call   sub_2_45E2
        or     eax, eax
        jne    loc_2_0080
        call   0
loc_2_0080:
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0088:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        or     edx, edx
        je     loc_2_00C4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        or     word ptr [edx], 4
        mov    eax, dword ptr [ecx + 0x28]
        mov    word ptr [edx + 6], ax
        mov    dword ptr [edx + 0x1c], 0     ; -> loc_2_03F9
        mov    dword ptr [edx + 0x18], 0
        mov    eax, dword ptr [ecx + 0x44]
        mov    word ptr [edx + 2], ax
        mov    al, byte ptr [ecx + 0x5c]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 0x38]
        mov    byte ptr [edx + 5], al
loc_2_00C4:
        mov    eax, 1
        pop    ebp
        ret    
loc_2_00CB:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, 2
        je     loc_2_00E7
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x28]
        sub    eax, ecx
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_00E7:
        pop    ebp
        ret    
sub_2_00E9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, 0x10                     ; -> obj3:0x0010
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, dword ptr [0x10]         ; -> obj3:0x0010
        je     loc_2_010D
loc_2_0101:
        cmp    dword ptr [esi], 0
        je     loc_2_0185
        add    esi, 8
        cmp    dword ptr [esi], eax
        jne    loc_2_0101
loc_2_010D:
        cmp    dword ptr [esi], 0
        je     loc_2_0185
        push   eax
        push   0                             ; -> loc_2_00CB
        call   sub_2_4594
        add    esp, 8
        or     eax, eax
        jne    loc_2_0185
        call   sub_2_43E7
        mov    edi, eax
        or     edi, edi
        je     loc_2_0185
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [edi], eax
        push   edi
        call   dword ptr [esi + 4]
        add    esp, 8
        or     eax, eax
        jne    loc_2_0150
        push   edi
        call   sub_2_44A3
        add    esp, 4
        sub    edi, edi
        jmp    loc_2_0185
loc_2_0150:
        push   dword ptr [edi + 0x38]
        push   dword ptr [edi + 0x5c]
        push   dword ptr [edi + 0x4c]
        push   dword ptr [edi + 0x58]
        call   sub_2_05C9
        add    esp, 0x10
        test   byte ptr [ebx + 8], 0x10
        je     loc_2_0185
        push   dword ptr [edi + 0x44]
        push   dword ptr [edi + 0x5c]
        mov    eax, dword ptr [ebx + 0x14]
        push   dword ptr [edi + 0x38]
        push   dword ptr [eax + 0xc]
        call   sub_2_5A0F
        add    esp, 0x10
        or     dword ptr [ebx + 8], 2
loc_2_0185:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_018C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    edi, dword ptr [esi]
        test   byte ptr [esi + 8], 1
        je     loc_2_01C6
        push   esi
        push   dword ptr [esi + 0xc]
        call   sub_2_00E9
        add    esp, 8
        cmp    eax, ebx
        je     loc_2_01C2
        push   dword ptr [esi + 0x14]
        push   eax
        call   sub_2_0088
        add    esp, 8
        mov    eax, 1
        jmp    loc_2_01FF
loc_2_01C2:
        sub    eax, eax
        jmp    loc_2_01FF
loc_2_01C6:
        cmp    dword ptr [edi], 0
        jne    loc_2_01CF
        sub    eax, eax
        jmp    loc_2_01FF
loc_2_01CF:
        push   esi
        push   dword ptr [edi]
        call   sub_2_00E9
        add    esp, 8
        or     eax, eax
        je     loc_2_01E1
        inc    ebx
        jmp    loc_2_01E4
loc_2_01E1:
        add    edi, 4
loc_2_01E4:
        cmp    dword ptr [edi], 0
        je     loc_2_01FD
        call   sub_2_45E2
        cmp    eax, 6
        jae    loc_2_01FD
        test   byte ptr [esi + 8], 2
        je     loc_2_01CF
        or     ebx, ebx
        je     loc_2_01CF
loc_2_01FD:
        mov    eax, ebx
loc_2_01FF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_0204:
        push   ebp
        mov    ebp, esp
        mov    dword ptr [4], 1              ; -> obj3:0x0004
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   1
        call   sub_2_02FE
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0222:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   0
        call   sub_2_02FE
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0236:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0x54
        jne    loc_2_0259
        push   1
        call   sub_2_92D2
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_2_9885
        add    esp, 4
loc_2_0259:
        mov    eax, 1
        pop    ebp
        ret    
sub_2_0260:
        push   esi
        sub    esi, esi
        call   sub_2_0791
        cmp    ax, 0x400
        jb     loc_2_02FA
        call   sub_2_8750
        call   sub_2_82CF
        cmp    eax, 0x62a
        jne    loc_2_028F
        mov    dword ptr [8], 4              ; -> obj3:0x0008
        jmp    loc_2_02AE
loc_2_028F:
        or     eax, eax
        jne    loc_2_02A4
        mov    dword ptr [8], 1              ; -> obj3:0x0008
        call   sub_2_9040
        jmp    loc_2_02AE
loc_2_02A4:
        mov    dword ptr [8], 2              ; -> obj3:0x0008
loc_2_02AE:
        push   0x62a
        call   sub_2_821C
        add    esp, 4
        call   sub_2_4754
        call   sub_2_4180
        push   0
        push   0
        push   0                             ; -> loc_2_0236
        call   sub_1_12BC
loc_2_02D3:
        add    esp, 0xc
        mov    dword ptr [0xc], eax          ; -> obj3:0x000C
        push   0
        call   sub_2_92D2
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_2_9885
        add    esp, 4
        mov    esi, 1
loc_2_02FA:
        mov    eax, esi
        pop    esi
        ret    
sub_2_02FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    edi, 1
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, edi
        jne    loc_2_0323
        cmp    dword ptr [0], 0              ; -> obj3:0x0000
        je     loc_2_0371
loc_2_0323:
        call   sub_2_45E2
        or     eax, eax
        jne    loc_2_0333
        call   sub_2_0260
        mov    edi, eax
loc_2_0333:
        call   sub_2_5F08
        mov    ebx, eax
        or     edi, edi
        je     loc_2_0371
        push   dword ptr [ebp + 0xc]
        push   esi
        push   ebx
        call   sub_2_5E31
        add    esp, 0xc
        or     eax, eax
        je     loc_2_0363
        push   ebx
        call   sub_2_018C
        add    esp, 4
        or     eax, eax
        je     loc_2_0363
        mov    dword ptr [ebp - 4], 1
loc_2_0363:
        call   sub_2_45E2
        or     eax, eax
        jne    loc_2_0371
        call   0
loc_2_0371:
        cmp    esi, 1
        mov    eax, edi
        je     loc_2_037B
        mov    eax, dword ptr [ebp - 4]
loc_2_037B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0382:
        mov    dword ptr [0], 1              ; -> obj3:0x0000
        mov    eax, 1
        ret    
sub_2_0392:
        push   0
        call   sub_2_0028
        add    esp, 4
        mov    eax, 1
        ret    
sub_2_03A2:
        call   sub_2_4787
        push   0
        call   sub_2_92D2
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_2_9885
        add    esp, 4
        mov    dword ptr [4], 0              ; -> obj3:0x0004
        mov    eax, 1
        ret    
sub_2_03D1:
        call   sub_1_1304
loc_2_03D6:
        test   byte ptr [eax], 0x40
        jne    loc_2_03E9
        cmp    dword ptr [4], 0              ; -> obj3:0x0004
        mov    eax, 1
        jne    loc_2_03EB
loc_2_03E9:
        sub    eax, eax
loc_2_03EB:
        ret    
loc_2_03EC:
        push   esi
        call   sub_2_0028
        add    esp, 4
        cmp    eax, 1
        ret    
loc_2_03F9:
        push   esi
        call   sub_2_0028
        add    esp, 4
        cmp    eax, 1
        ret    
LMOUSE_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        jmp    loc_2_5F54
loc_2_040F:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   ebx
        push   ecx
        call   sub_2_0222
        add    esp, 8
        cmp    eax, 1
        ret    
loc_2_0423:
        xor    ecx, ecx
        push   ebx
        push   ecx
        call   sub_2_0204
        add    esp, 8
        cmp    eax, 1
        ret    
        db  000h
sub_2_0434:
        enter  0, 0
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0
        je     loc_2_044F
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        jmp    loc_2_0455
loc_2_044F:
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_2_0455:
        pop    ebx
        leave  
        ret    
sub_2_0458:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        leave  
        ret    
sub_2_0467:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        leave  
        ret    
sub_2_0476:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    ebx, ebx
        VxDCall  VPICD.VPICD_Force_Default_Owner ; cd2010000300
        sbb    eax, eax
        leave  
        ret    
sub_2_0488:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        leave  
        ret    
        db  0C8h, 000h, 000h, 000h, 08Bh, 045h, 008h, 06Ah, 000h, 050h, 0CDh, 020h, 015h, 001h, 001h, 000h
        db  083h, 0C4h, 008h, 0C9h, 0C3h, 0CDh, 020h, 000h, 000h, 00Dh, 000h, 0C3h
sub_2_04B3:
        push   eax
        push   ecx
        mov    ecx, 0x10000
loc_2_04BA:
        in     al, 0x64
        test   al, 2
        loopne loc_2_04BA
        pop    ecx
        pop    eax
        ret    
sub_2_04C3:
        call   sub_2_04B3
        out    0x64, al
        ret    
sub_2_04CB:
        push   eax
        mov    al, 0xae
        call   sub_2_04C3
        pop    eax
        ret    
sub_2_04D5:
        push   eax
        mov    al, 0xad
        call   sub_2_04C3
        pop    eax
        ret    
sub_2_04DF:
        mov    edx, 0x60
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x64
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        call   sub_2_04D5
        ret    
sub_2_04FB:
        mov    edx, 0x60
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x64
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        call   sub_2_04CB
        ret    
sub_2_0517:
        pushal 
        pushfd 
        VxDCall  VKD.VKD_Get_Version             ; cd2000000d00
        cmp    ax, 0x20a
        jb     loc_2_052D
        VxDCall  VKD.VKD_Disable_Keyboard        ; cd2010000d00
        jmp    loc_2_0532
loc_2_052D:
        call   sub_2_04DF
loc_2_0532:
        popfd  
        popal  
        ret    
sub_2_0535:
        pushal 
        pushfd 
        VxDCall  VKD.VKD_Get_Version             ; cd2000000d00
        cmp    ax, 0x20a
        jb     loc_2_054B
        VxDCall  VKD.VKD_Enable_Keyboard         ; cd200f000d00
        jmp    loc_2_0550
loc_2_054B:
        call   sub_2_04FB
loc_2_0550:
        popfd  
        popal  
        ret    
sub_2_0553:
        push   edi
        push   1
        push   0x6c
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_2_0573
        mov    edi, eax
        mov    dword ptr [0x30], eax         ; -> obj3:0x0030
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
loc_2_0573:
        ret    
sub_2_0574:
        push   esi
        mov    eax, dword ptr [0x30]         ; -> obj3:0x0030
        or     eax, eax
        je     loc_2_059D
        mov    esi, eax
        mov    dword ptr [0x30], 0           ; -> obj3:0x0030
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    esi
loc_2_059D:
        ret    
sub_2_059E:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        ret    
sub_2_05A5:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        ret    
sub_2_05AC:
        enter  0, 0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        xor    eax, eax
        jb     loc_2_05C6
        mov    eax, 1
loc_2_05C6:
        pop    esi
        leave  
        ret    
sub_2_05C9:
        enter  0, 0
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ah, cl
        mov    ebx, 3
        mov    edx, dword ptr [ebp + 0x14]
        mov    dh, bl
        xor    ebx, ebx
        xor    ecx, ecx
        VxDCall  VMD.Service_0006h               ; cd2006000c00
        pop    ebx
        leave  
        ret    
sub_2_05ED:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        cmp    al, 2
        jne    loc_2_05FA
        mov    al, 0x82
loc_2_05FA:
        leave  
        ret    
        db  0C8h, 000h, 000h, 000h, 053h, 033h, 0DBh, 08Bh, 045h, 008h, 083h, 0F8h, 002h, 074h, 008h, 0CDh
        db  020h, 001h, 000h, 001h, 000h, 0EBh, 006h, 0CDh, 020h, 003h, 000h, 001h, 000h, 08Bh, 0C3h, 05Bh
        db  0C9h, 0C3h, 0C8h, 000h, 000h, 000h, 08Bh, 045h, 008h, 06Ah, 000h, 050h, 0CDh, 020h, 0A7h, 000h
        db  001h, 000h, 083h, 0C4h, 008h, 0C9h, 0C3h
sub_2_0633:
        enter  0, 0
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    eax, 1
        jae    loc_2_0650
        mov    eax, 0
loc_2_0650:
        pop    esi
        leave  
        ret    
sub_2_0653:
        enter  0, 0
        push   esi
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        sbb    eax, eax
        not    eax
        pop    esi
        leave  
        ret    
sub_2_0668:
        enter  0, 0
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0
        je     loc_2_0683
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        jmp    loc_2_0689
loc_2_0683:
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_2_0689:
        pop    ebx
        leave  
        ret    
sub_2_068C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], eax
        sbb    eax, eax
        not    eax
        pop    esi
        leave  
        ret    
sub_2_06A8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        xor    eax, eax
        cmp    dword ptr [esi], 0
        jne    loc_2_06CB
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], eax
        sbb    eax, eax
        not    eax
loc_2_06CB:
        pop    esi
        leave  
        ret    
sub_2_06CE:
        enter  0, 0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        xor    eax, eax
        jb     loc_2_06E8
        mov    eax, 1
loc_2_06E8:
        pop    esi
        leave  
        ret    
sub_2_06EB:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    eax, dword ptr [ebp + 0xc]
        cmp    word ptr [eax], 0
        jne    loc_2_0707
        mov    word ptr [eax], cx
        mov    dword ptr [eax + 2], edx
loc_2_0707:
        leave  
        ret    
sub_2_0709:
        enter  0, 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [eax]
        mov    edx, dword ptr [eax + 2]
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        leave  
        ret    
sub_2_0721:
        enter  0, 0
        push   edi
        push   1
        push   0x20
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_2_078E
        mov    edi, eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 4], eax
        mov    al, byte ptr [ebp + 8]
        xor    ah, ah
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 2
        cmp    word ptr [ebp + 0x10], 0
        je     loc_2_0774
        mov    word ptr [edi + 2], 1
        cmp    word ptr [ebp + 0x10], 1
        je     loc_2_0774
        mov    word ptr [edi + 2], 2
        cmp    word ptr [ebp + 0x10], 2
        je     loc_2_0774
        mov    word ptr [edi + 2], 4
loc_2_0774:
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pushfd 
        push   eax
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    eax
        popfd  
        jae    loc_2_078E
        xor    eax, eax
loc_2_078E:
        pop    edi
        leave  
        ret    
sub_2_0791:
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        ret    
sub_2_0798:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        leave  
        ret    
sub_2_07AB:
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
        ret    
        db  000h, 000h
sub_2_07B4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    eax, [eax + 0xaf]
        mov    esi, 0xffffffff
        xchg   dword ptr [eax + 8], esi
        or     esi, esi
        je     loc_2_07D8
        xor    esi, esi
        xchg   dword ptr [eax + 0xc], esi
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
loc_2_07D8:
        pop    esi
        leave  
        ret    
sub_2_07DB:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    eax, [eax + 0xaf]
        xor    esi, esi
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0xc], esi
        pop    esi
        leave  
        ret    
sub_2_07F3:
        mov    edi, 0x1398                   ; -> loc_1_1398
loc_2_07F8:
        cmp    dword ptr [edi + 8], eax
        je     loc_2_080B
        add    edi, 0xc
        cmp    edi, 0x13e0                   ; -> obj1:0x13E0
        jb     loc_2_07F8
        stc    
        jmp    loc_2_080C
loc_2_080B:
        clc    
loc_2_080C:
        ret    
sub_2_080D:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    eax, dword ptr [ebp + 8]
        call   sub_2_07F3
        mov    eax, 0
        jb     loc_2_082C
        mov    dword ptr [edi + 8], 0
        mov    eax, 1
loc_2_082C:
        pop    edi
        leave  
        ret    
sub_2_082F:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    eax, dword ptr [ebp + 8]
        call   sub_2_07F3
        jb     loc_2_0844
        mov    eax, 0
        jmp    loc_2_0863
loc_2_0844:
        mov    eax, 0
        call   sub_2_07F3
        mov    eax, 0
        jb     loc_2_0863
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [edi]
loc_2_0863:
        pop    edi
        leave  
        ret    
        db  000h, 000h
loc_2_0868:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 8]
        call   sub_2_0488
        add    esp, 4
        push   esi
        call   sub_2_07B4
        add    esp, 4
        push   dword ptr [esi + 0x38]
        call   sub_2_585C
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_1E74
        add    esp, 8
        lea    eax, [esi + 0xaf]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
loc_2_08E1:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 0xc], 0
        mov    eax, 0
        jne    loc_2_08FD
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_2_0A9E
        add    esp, 8
loc_2_08FD:
        pop    ebp
        ret    
loc_2_08FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x28]
        push   edi
        call   sub_2_3F1C
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_0927
        sub    eax, eax
        jmp    loc_2_09CE
loc_2_0927:
        push   edi
        call   sub_2_3F69
        add    esp, 4
        or     eax, eax
        jne    loc_2_093E
        mov    eax, 1
        jmp    loc_2_09CE
loc_2_093E:
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        mov    ax, word ptr [esi + 0x3c]
        mov    word ptr [ebp - 2], ax
        push   0x4b0
        push   esi
        call   sub_2_0A9E
        add    esp, 8
        cmp    ebx, 1
        mov    eax, dword ptr [esi + 0xd7]
        jne    loc_2_0983
        add    eax, 0x2b
        push   eax
        push   edi
        lea    eax, [ebp - 0x84]
        push   eax
        call   sub_2_1258
        jmp    loc_2_0994
loc_2_0983:
        add    eax, 0x2b
        push   eax
        push   edi
        lea    eax, [ebp - 0x84]
        push   eax
        call   sub_2_1298
loc_2_0994:
        add    esp, 0xc
        mov    edi, eax
        push   esi
        call   sub_2_07DB
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 0x13]
        push   dword ptr [esi + 0xcf]
        lea    ecx, [eax + 0x2b]
        push   ecx
        push   eax
        call   sub_2_1FE0
        add    esp, 0x10
        movzx  eax, word ptr [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0A9E
        add    esp, 8
        mov    eax, edi
loc_2_09CE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_09D5:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    word ptr [edi], 0
        add    edi, 2
        mov    ecx, 0x20
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    ebp
        ret    
loc_2_09F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xd7]
        lea    ebx, [eax + 0x2b]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        lea    eax, [ebp - 0x84]
        push   eax
        call   sub_2_09D5
        add    esp, 4
        push   0x4b0
        push   ebx
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     loc_2_0A48
        push   8
        push   ebx
        call   sub_2_1F5D
        add    esp, 8
        or     eax, eax
        mov    edi, 1
        jne    loc_2_0A4A
loc_2_0A48:
        sub    edi, edi
loc_2_0A4A:
        or     edi, edi
        je     loc_2_0A95
        lea    eax, [ebp - 0x84]
        push   eax
        push   ebx
        call   sub_2_0C99
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_2_0A95
        push   dword ptr [esi + 0x3c]
        push   esi
        call   sub_2_0A9E
        add    esp, 8
        push   esi
        call   sub_2_07DB
        add    esp, 4
        mov    ecx, dword ptr [esi + 0xd7]
        push   dword ptr [ecx + 0x13]
        push   dword ptr [esi + 0xcf]
        lea    eax, [ecx + 0x2b]
        push   eax
        push   ecx
        call   sub_2_1FE0
        add    esp, 0x10
loc_2_0A95:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0A9E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0xd7]
        movzx  eax, word ptr [eax + 0x51]
        sub    eax, edi
        cmp    eax, 1
        sbb    ebx, ebx
        neg    ebx
        or     ebx, ebx
        jne    loc_2_0BA1
        cmp    edi, 0x4b0
        je     loc_2_0ADF
        cmp    edi, 0x2580
        je     loc_2_0ADF
        sub    ebx, ebx
        jmp    loc_2_0BA1
loc_2_0ADF:
        test   byte ptr [esi + 0xa3], 2
        jne    loc_2_0AEF
        sub    ebx, ebx
        jmp    loc_2_0BA1
loc_2_0AEF:
        push   edi
        call   sub_2_1F3C
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 1
        sbb    ebx, ebx
        inc    ebx
        je     loc_2_0BA1
        mov    ecx, dword ptr [esi + 0xd7]
        cmp    word ptr [ecx + 0x51], 0x4b0
        jne    loc_2_0B4E
        push   dword ptr [eax + 4]
        add    ecx, 0x2b
        push   ecx
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        je     loc_2_0B8D
        push   0xa
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x43
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   edi
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_1F06
        jmp    loc_2_0B88
loc_2_0B4E:
        push   edi
        add    ecx, 0x2b
        push   ecx
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     loc_2_0B8D
        push   0xa
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x43
        push   eax
        call   sub_2_2554
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_2137
loc_2_0B88:
        add    esp, 8
        mov    ebx, eax
loc_2_0B8D:
        or     ebx, ebx
        je     loc_2_0BA1
        movzx  eax, di
        mov    dword ptr [esi + 0x3c], eax
        mov    eax, dword ptr [esi + 0xd7]
        mov    word ptr [eax + 0x51], di
loc_2_0BA1:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0BAA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [eax + 4]
        lea    edi, [ecx + 0x28]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [ecx], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_0BCD:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx]
        cmp    ax, 0x80
        ja     loc_2_0BE9
        movzx  eax, ax
        mov    dl, byte ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 2], dl
        inc    word ptr [ecx]
loc_2_0BE9:
        pop    ebp
        ret    
sub_2_0BEB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x18
        mov    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [edi]
        sub    ebx, ebx
        push   eax
        call   sub_2_2570
        add    esp, 8
        mov    esi, dword ptr [ebp + 0xc]
loc_2_0C17:
        lea    eax, [ebp - 1]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_20FD
        add    esp, 8
        or     al, al
        je     loc_2_0C6D
        push   esi
        push   dword ptr [ebp - 1]
        call   dword ptr [edi + 8]
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 3
        je     loc_2_0C5B
        cmp    ebx, 1
        je     loc_2_0C4C
        push   esi
        push   dword ptr [ebp - 1]
        call   sub_2_0BCD
        add    esp, 8
loc_2_0C4C:
        push   dword ptr [edi]
        push   dword ptr [ebp - 8]
        call   sub_2_2570
        add    esp, 8
        jmp    loc_2_0C7B
loc_2_0C5B:
        cmp    word ptr [esi], 0
        je     loc_2_0C6D
        mov    word ptr [esi], 0
        mov    ebx, 1
        jmp    loc_2_0C7B
loc_2_0C6D:
        push   dword ptr [ebp - 8]
        call   sub_2_2582
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
loc_2_0C7B:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0C8B
        cmp    ebx, 2
        je     loc_2_0C8B
        cmp    ebx, 1
        jne    loc_2_0C17
loc_2_0C8B:
        cmp    word ptr [esi], 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0C99:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_2122
        add    esp, 8
        lea    esi, [edi + 0x18]
        push   0x96
        push   esi
        call   sub_2_2554
        add    esp, 8
        mov    word ptr [edi + 0x26], 0x4b0
        push   1
        push   edi
        call   sub_2_2122
        add    esp, 8
        push   0x96
        push   esi
        call   sub_2_2554
        add    esp, 8
        push   edi
        call   sub_2_2096
        add    esp, 4
        push   3
        push   edi
        call   sub_2_2122
        add    esp, 8
        push   dword ptr [0x44]              ; -> obj3:0x0078
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_2_0BEB
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_0D06:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0BAA
        add    esp, 8
        push   edi
        push   esi
        call   sub_2_1EA0
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0D7F
        push   dword ptr [ebp + 0x10]
        call   sub_2_09D5
        add    esp, 4
        push   0x4b0
        push   esi
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     loc_2_0D5D
        push   8
        push   esi
        call   sub_2_1F5D
        add    esp, 8
        or     eax, eax
        mov    ebx, 1
        jne    loc_2_0D5F
loc_2_0D5D:
        sub    ebx, ebx
loc_2_0D5F:
        or     ebx, ebx
        je     loc_2_0D7F
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_0C99
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_0D7F
        push   edi
        push   esi
        call   sub_2_1E74
        add    esp, 8
loc_2_0D7F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_0D86:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [eax + 2]
        sub    al, 0x42
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        ret    
sub_2_0D99:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        push   0x14
        lea    ebx, [edi + 0x18]
        push   ebx
        call   sub_2_2554
        mov    esi, dword ptr [ebp + 0x10]
        add    esp, 8
        push   dword ptr [esi + 4]
        push   edi
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     loc_2_0DE9
        push   0x14
        push   ebx
        call   sub_2_2554
        add    esp, 8
        push   edi
        call   sub_2_2096
        add    esp, 4
        push   esi
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_2_0BEB
        add    esp, 0xc
loc_2_0DE9:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_0DEE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [eax]
        movzx  eax, cx
        or     eax, eax
        je     loc_2_0E1A
        test   byte ptr [ebp + 8], 0xc0
        je     loc_2_0E06
        jmp    loc_2_0E28
loc_2_0E06:
        cmp    cx, 3
        jne    loc_2_0E13
        mov    eax, 2
        jmp    loc_2_0E2D
loc_2_0E13:
        mov    eax, 4
        jmp    loc_2_0E2D
loc_2_0E1A:
        mov    al, byte ptr [ebp + 8]
        and    al, 0xc0
        cmp    al, 0x40
        mov    eax, 4
        je     loc_2_0E2D
loc_2_0E28:
        mov    eax, 3
loc_2_0E2D:
        pop    ebp
        ret    
sub_2_0E2F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi], 4
        jne    loc_2_0E8E
        mov    al, byte ptr [esi + 2]
        mov    edx, dword ptr [ebp + 0xc]
        and    ax, 0x3f
        mov    word ptr [edx + 0x50], ax
        mov    al, byte ptr [esi + 3]
        and    ax, 7
        mov    word ptr [edx + 0x52], ax
        mov    cl, byte ptr [esi + 3]
        shr    cl, 3
        mov    eax, dword ptr [edx + 0x7b]
        movzx  ecx, cl
        xor    ecx, eax
        and    ecx, 1
        xor    ecx, eax
        mov    dword ptr [edx + 0x7b], ecx
        mov    al, byte ptr [esi + 3]
        shr    al, 4
        mov    dword ptr [edx + 0x4c], 3
        movzx  eax, al
        add    eax, eax
        xor    eax, ecx
        and    eax, 2
        xor    eax, ecx
        mov    dword ptr [edx + 0x7b], eax
        mov    eax, 1
        jmp    loc_2_0E90
loc_2_0E8E:
        sub    eax, eax
loc_2_0E90:
        pop    esi
        pop    ebp
        ret    
loc_2_0E93:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [eax]
        movzx  eax, cx
        or     eax, eax
        je     loc_2_0EBF
        test   byte ptr [ebp + 8], 0xc0
        je     loc_2_0EAB
        jmp    loc_2_0ECD
loc_2_0EAB:
        cmp    cx, 6
        jne    loc_2_0EB8
        mov    eax, 2
        jmp    loc_2_0ED2
loc_2_0EB8:
        mov    eax, 4
        jmp    loc_2_0ED2
loc_2_0EBF:
        mov    al, byte ptr [ebp + 8]
        and    al, 0xc0
        cmp    al, 0x40
        mov    eax, 4
        je     loc_2_0ED2
loc_2_0ECD:
        mov    eax, 3
loc_2_0ED2:
        pop    ebp
        ret    
sub_2_0ED4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax], 7
        jne    loc_2_0F1D
        mov    cl, byte ptr [eax + 2]
        and    cl, 0x3f
        cmp    cl, 1
        jne    loc_2_0F1D
        mov    dl, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp + 0xc]
        and    dx, 3
        mov    word ptr [ecx + 0x65], dx
        mov    dl, byte ptr [eax + 4]
        and    dx, 7
        mov    word ptr [ecx + 0x67], dx
        mov    al, byte ptr [eax + 3]
        and    al, 0xc
        shr    al, 2
        push   ecx
        push   eax
        call   sub_2_3FD6
        add    esp, 8
        mov    eax, 1
        jmp    loc_2_0F1F
loc_2_0F1D:
        sub    eax, eax
loc_2_0F1F:
        pop    ebp
        ret    
loc_2_0F21:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [eax]
        or     eax, eax
        je     loc_2_0F42
        cmp    eax, 1
        je     loc_2_0F62
        test   byte ptr [ebp + 8], 0xc0
        je     loc_2_0F3B
        jmp    loc_2_0F72
loc_2_0F3B:
        mov    eax, 4
        jmp    loc_2_0F77
loc_2_0F42:
        mov    al, byte ptr [ebp + 8]
        cmp    al, 0x4d
        jne    loc_2_0F50
        mov    eax, 4
        jmp    loc_2_0F77
loc_2_0F50:
        cmp    al, 0x42
        jne    loc_2_0F5B
        mov    eax, 2
        jmp    loc_2_0F77
loc_2_0F5B:
        mov    eax, 3
        jmp    loc_2_0F77
loc_2_0F62:
        mov    al, byte ptr [ebp + 8]
        cmp    al, 0x31
        jb     loc_2_0F72
        cmp    al, 0x34
        mov    eax, 4
        jbe    loc_2_0F77
loc_2_0F72:
        mov    eax, 1
loc_2_0F77:
        pop    ebp
        ret    
sub_2_0F79:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
loc_2_0F7F:
        mov    cl, byte ptr [eax]
        inc    eax
        test   cl, 0x10
        jne    loc_2_0F7F
        pop    ebp
        ret    
sub_2_0F89:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    ecx, ecx
        sub    ax, ax
        mov    edx, dword ptr [ebp + 8]
loc_2_0F96:
        movzx  si, byte ptr [edx]
        mov    di, si
        and    di, 0xf
        shl    di, cl
        or     ax, di
        add    ecx, 4
        inc    edx
        test   si, 0x10
        jne    loc_2_0F96
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_0FB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, 0
        test   byte ptr [edi], 0x20
        je     loc_2_10B4
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   0x87
        call   sub_1_12F4
loc_2_0FDC:
        add    esp, 8
        mov    dword ptr [esi + 0x83], eax
        mov    ebx, eax
        push   ebx
        call   sub_2_409F
        add    esp, 4
        mov    al, byte ptr [edi]
        and    al, 0xf
        mov    byte ptr [ebx + 0x5a], al
        inc    edi
        push   edi
        call   sub_2_0F89
        add    esp, 4
        mov    word ptr [ebx + 0x52], ax
        push   edi
        call   sub_2_0F79
        add    esp, 4
        mov    edi, eax
        movzx  eax, word ptr [ebx + 0x52]
        push   eax
        push   ebx
        call   sub_2_4026
        add    esp, 8
        mov    al, byte ptr [edi]
        and    al, 2
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    word ptr [ebx + 0x69], ax
        mov    al, byte ptr [edi]
        and    al, 2
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    word ptr [esi + 0x69], ax
        mov    al, byte ptr [edi]
        and    al, 4
        sub    al, 4
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebx + 0x73], eax
        mov    al, byte ptr [edi]
        and    al, 4
        sub    al, 4
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi + 0x73], eax
        mov    al, byte ptr [edi]
        and    al, 8
        sub    al, 8
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebx + 0x6f], eax
        mov    al, byte ptr [edi]
        and    al, 8
        sub    al, 8
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi + 0x6f], eax
        mov    al, byte ptr [edi]
        and    al, 0x10
        sub    al, 0x10
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebx + 0x6b], eax
        mov    al, byte ptr [edi]
        and    al, 0x10
        sub    al, 0x10
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi + 0x6b], eax
        mov    al, byte ptr [edi + 1]
        inc    edi
        and    ax, 7
        mov    word ptr [ebx + 0x67], ax
        mov    al, byte ptr [edi]
        and    ax, 7
        mov    word ptr [esi + 0x67], ax
        mov    eax, ebx
loc_2_10B4:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_10B9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, 2
        movzx  esi, word ptr [eax]
        lea    edx, [esi - 1]
        cmp    edx, 2
        jle    loc_2_10E8
        mov    ebx, 0x20
loc_2_10D6:
        movzx  edi, cx
        test   byte ptr [eax + edi + 2], bl
        je     loc_2_10F3
        inc    cx
        movzx  edi, cx
        cmp    edi, edx
        jl     loc_2_10D6
loc_2_10E8:
        test   byte ptr [esi + eax + 1], 0x30
        je     loc_2_10F7
        sub    eax, eax
        jmp    loc_2_10FC
loc_2_10F3:
        sub    eax, eax
        jmp    loc_2_10FC
loc_2_10F7:
        mov    eax, 1
loc_2_10FC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_1101:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_10B9
        add    esp, 4
        or     eax, eax
        jne    loc_2_111B
        sub    eax, eax
        jmp    loc_2_1190
loc_2_111B:
        lea    ebx, [esi + 4]
        mov    edi, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_0F89
        add    esp, 4
        mov    word ptr [edi + 0x52], ax
        mov    dword ptr [edi + 0x4c], 4
        movzx  eax, ax
        push   eax
        push   edi
        call   sub_2_4026
        add    esp, 8
        push   ebx
        call   sub_2_0F79
        add    esp, 4
        mov    ebx, eax
        push   ebx
        call   sub_2_0F89
        add    esp, 4
        mov    word ptr [edi + 0x50], ax
        push   ebx
        call   sub_2_0F79
        add    esp, 4
        mov    ebx, eax
        movzx  eax, word ptr [esi]
        add    eax, esi
        inc    eax
        cmp    eax, ebx
        jbe    loc_2_118B
        push   edi
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        jne    loc_2_118B
        push   ebx
        push   edi
        call   sub_2_0FB6
        add    esp, 8
        or     dword ptr [edi + 0x7b], 2
loc_2_118B:
        mov    eax, 1
loc_2_1190:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_1195:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x10], 0
        mov    esi, dword ptr [ebp + 8]
        je     loc_2_11B8
        mov    ax, word ptr [esi]
        cmp    ax, 4
        jb     loc_2_11B4
        jne    loc_2_11B8
        test   byte ptr [esi + 5], 0x20
        je     loc_2_11B8
loc_2_11B4:
        sub    eax, eax
        jmp    loc_2_1214
loc_2_11B8:
        lea    eax, [esi + 2]
        cmp    byte ptr [eax], 0x4d
        je     loc_2_11C4
        sub    eax, eax
        jmp    loc_2_1214
loc_2_11C4:
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x4c], 1
        mov    cl, byte ptr [esi + 3]
        or     cl, cl
        jne    loc_2_11D9
        sub    eax, eax
        jmp    loc_2_1214
loc_2_11D9:
        and    cl, 0xf
        mov    byte ptr [edi + 0x5a], cl
        mov    dword ptr [edi + 0x4c], 2
        movzx  ecx, word ptr [esi]
        push   ecx
        push   eax
        lea    eax, [edi + 0x7f]
        push   eax
        call   sub_2_2804
        add    esp, 0xc
        or     eax, eax
        push   edi
        je     loc_2_120B
        call   sub_2_28BC
        add    esp, 4
        mov    eax, 1
        jmp    loc_2_1214
loc_2_120B:
        push   esi
        call   sub_2_1101
        add    esp, 8
loc_2_1214:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1218:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_09D5
        add    esp, 4
        push   dword ptr [0x3c]              ; -> obj3:0x0070
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_0D99
        add    esp, 0xc
        or     eax, eax
        je     loc_2_1253
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_0E2F
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    loc_2_1255
loc_2_1253:
        sub    eax, eax
loc_2_1255:
        pop    esi
        pop    ebp
        ret    
sub_2_1258:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_09D5
        add    esp, 4
        push   dword ptr [0x40]              ; -> obj3:0x0074
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_0D99
        add    esp, 0xc
        or     eax, eax
        je     loc_2_1293
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_0ED4
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    loc_2_1295
loc_2_1293:
        sub    eax, eax
loc_2_1295:
        pop    esi
        pop    ebp
        ret    
sub_2_1298:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_09D5
        add    esp, 4
        push   dword ptr [0x44]              ; -> obj3:0x0078
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_0D99
        add    esp, 0xc
        or     eax, eax
        je     loc_2_12D5
        push   1
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_1195
        add    esp, 0xc
        or     eax, eax
        mov    eax, 1
        jne    loc_2_12D7
loc_2_12D5:
        sub    eax, eax
loc_2_12D7:
        pop    esi
        pop    ebp
        ret    
sub_2_12DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        call   sub_2_409F
        add    esp, 4
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_0D86
        add    esp, 4
        or     eax, eax
        je     loc_2_1300
        sub    eax, eax
        jmp    loc_2_1375
loc_2_1300:
        lea    eax, [edi + 0x54]
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ebx + 0x30]
        push   dword ptr [ebx + 0x28]
        push   2
        call   sub_2_3DEC
        add    esp, 0x10
        mov    dword ptr [edi + 0x56], eax
        push   0
        push   edi
        push   esi
        call   sub_2_1195
        add    esp, 0xc
        or     eax, eax
        je     loc_2_1341
        push   edi
        call   sub_2_3F4B
        add    esp, 4
        or     eax, eax
        je     loc_2_1370
        push   ebx
        push   edi
        push   esi
        call   sub_2_1298
        jmp    loc_2_136D
loc_2_1341:
        push   ebx
        push   edi
        push   esi
        call   sub_2_1298
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_1370
        push   ebx
        push   edi
        push   esi
        call   sub_2_1218
        add    esp, 0xc
        or     eax, eax
        je     loc_2_1370
        test   byte ptr [edi + 0x7b], 1
        je     loc_2_1370
        push   ebx
        push   edi
        push   esi
        call   sub_2_1258
loc_2_136D:
        add    esp, 0xc
loc_2_1370:
        mov    eax, 1
loc_2_1375:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_137A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        lea    eax, [edx + 0x2b]
        mov    edi, eax
        mov    ecx, 0xd
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ax, word ptr [eax]
        mov    word ptr [edx + 0xc], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_13A1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [edi + 0xd7]
        call   sub_2_2050
        add    esp, 4
        lea    esi, [edi + 0xaf]
        push   dword ptr [0]                 ; -> obj3:0x007C
        push   esi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     loc_2_13EA
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 0xcf], eax
        mov    eax, dword ptr [0]            ; -> obj3:0x007C
        mov    dword ptr [edi + 0xd3], eax
        mov    eax, 1
        jmp    loc_2_13EC
loc_2_13EA:
        sub    eax, eax
loc_2_13EC:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_13F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   2
        mov    eax, dword ptr [esi + 0xd7]
        mov    eax, dword ptr [eax + 0x5b]
        mov    dword ptr [esi + 0x5c], eax
        mov    eax, dword ptr [esi + 0xd7]
        mov    eax, dword ptr [eax + 0x53]
        mov    dword ptr [esi + 0x38], eax
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 0x58], eax
        mov    dword ptr [esi + 0x28], 2
        mov    dword ptr [esi + 0x68], 0x64
        mov    dword ptr [esi + 0x6c], 8
        mov    eax, dword ptr [esi + 0xd7]
        mov    dword ptr [esi + 0x3c], 0x4b0
        movzx  eax, word ptr [eax + 0xc]
        mov    dword ptr [esi + 0x44], eax
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_1450:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   0x5f
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     loc_2_14C6
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0xd7]
        call   sub_2_137A
        add    esp, 8
        push   esi
        call   sub_2_13A1
        add    esp, 4
        cmp    eax, edi
        je     loc_2_14C6
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [esi + 0xcf]
        push   eax
        add    eax, 0x2b
        push   eax
        call   sub_2_1F89
        add    esp, 0xc
        cmp    eax, edi
        je     loc_2_14B7
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_13F0
        add    esp, 8
        mov    edi, 1
        jmp    loc_2_14C6
loc_2_14B7:
        add    esi, 0xaf
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_14C6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_14CC:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xd7]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x13], eax
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x20
        mov    edx, dword ptr [ecx + 0xd7]
        mov    dword ptr [edx + 0x27], eax
        mov    eax, dword ptr [ecx + 0xd7]
        mov    dword ptr [eax + 0x1b], 0
        mov    eax, dword ptr [ecx + 0xd7]
        mov    dword ptr [eax + 0x17], ecx
        mov    edx, dword ptr [ecx + 0xd7]
        movzx  eax, word ptr [ecx + 0x7a]
        mov    dword ptr [edx + 0x1f], eax
        mov    eax, dword ptr [ecx + 0xd7]
        mov    edx, dword ptr [ecx + 0x38]
        mov    dword ptr [eax + 0x23], edx
        mov    eax, 1
        pop    ebp
        ret    
sub_2_1528:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x14], 0     ; -> loc_2_09F0
        mov    dword ptr [eax + 0x18], 0     ; -> loc_2_0868
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_2_08E1
        mov    dword ptr [eax + 0x20], 0     ; -> loc_2_08FF
        mov    eax, 1
        ret    
sub_2_1551:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xb8
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        lea    eax, [ebp - 0xb8]
        push   eax
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_2_0D06
        add    esp, 0xc
        cmp    eax, esi
        je     loc_2_1638
        mov    ebx, dword ptr [ebp + 8]
        lea    eax, [ebp - 0xb8]
        lea    edi, [ebx + 0x28]
        push   edi
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_2_12DA
        add    esp, 0xc
        cmp    eax, esi
        je     loc_2_1625
        push   edi
        lea    eax, [ebp - 0x34]
        push   eax
        push   ebx
        call   sub_2_1450
        add    esp, 0xc
        cmp    eax, esi
        je     loc_2_1625
        push   ebx
        call   sub_2_1528
        add    esp, 4
        push   ebx
        call   sub_2_4333
        add    esp, 4
        push   ebx
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [ebx + 0x2c]
        push   ebx
        call   sub_2_9371
        add    esp, 8
        push   edi
        call   sub_2_3F4B
        add    esp, 4
        cmp    eax, esi
        je     loc_2_15F3
        call   sub_2_80A2
        cmp    eax, esi
        je     loc_2_15F3
        mov    esi, 1
loc_2_15F3:
        push   esi
        push   ebx
        call   sub_2_14CC
        add    esp, 8
        push   ebx
        call   sub_2_07DB
        add    esp, 4
        mov    ecx, dword ptr [ebx + 0xd7]
        push   esi
        push   dword ptr [ebx + 0xcf]
        lea    eax, [ecx + 0x2b]
        push   eax
        push   ecx
        call   sub_2_1FE0
        add    esp, 0x10
        mov    esi, 1
loc_2_1625:
        or     esi, esi
        jne    loc_2_1638
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_2_1E74
        add    esp, 8
loc_2_1638:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h
loc_2_1644:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 8]
        call   sub_2_0488
        add    esp, 4
        push   esi
        call   sub_2_07B4
        add    esp, 4
        push   dword ptr [esi + 0x38]
        call   sub_2_585C
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_1E74
        add    esp, 8
        lea    eax, [esi + 0xaf]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
loc_2_16BD:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 0xc], 0
        mov    eax, 0
        jne    loc_2_16D9
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_2_16DE
        add    esp, 8
loc_2_16D9:
        pop    ebp
        ret    
loc_2_16DB:
        sub    eax, eax
        ret    
sub_2_16DE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0xd7]
        movzx  eax, word ptr [eax + 0x51]
        cmp    eax, edi
        jne    loc_2_16FE
        mov    eax, 1
        jmp    loc_2_177C
loc_2_16FE:
        push   edi
        call   sub_2_1F3C
        add    esp, 4
        or     eax, eax
        je     loc_2_177A
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        je     loc_2_177A
        push   0xa
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x43
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   edi
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     loc_2_177A
        movzx  eax, di
        mov    dword ptr [esi + 0x3c], eax
        mov    eax, dword ptr [esi + 0xd7]
        push   0xa
        mov    word ptr [eax + 0x51], di
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x43
        push   eax
        call   sub_2_2554
        add    esp, 8
        mov    eax, 1
        jmp    loc_2_177C
loc_2_177A:
        sub    eax, eax
loc_2_177C:
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_1780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [eax + 0xd7]
        add    esi, 0x2b
        push   0
        push   esi
        call   sub_2_1F5D
        mov    eax, dword ptr [ebp + 8]
        add    esp, 8
        mov    eax, dword ptr [eax + 0xd7]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        push   0
        push   esi
        call   sub_2_2122
        add    esp, 8
        lea    edi, [esi + 0x18]
        push   0x1f4
        push   edi
        call   sub_2_2554
        add    esp, 8
        push   3
        push   esi
        call   sub_2_2122
        add    esp, 8
        push   0x1f4
        push   edi
        call   sub_2_2554
        add    esp, 8
        mov    ebx, 0x50                     ; -> obj3:0x00D0
loc_2_17F5:
        mov    edi, dword ptr [ebx]
        add    ebx, 4
        push   edi
        push   esi
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     loc_2_1818
        mov    word ptr [esi + 0x26], di
        push   esi
        call   sub_2_18D8
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_2_1818:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_182C
        cmp    edi, 0x2580
        jne    loc_2_17F5
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1867
loc_2_182C:
        push   dword ptr [ebp + 8]
        call   sub_2_1C90
        add    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_2_07DB
        mov    eax, dword ptr [ebp + 8]
        add    esp, 4
        mov    ecx, dword ptr [eax + 0xd7]
        push   0
        push   dword ptr [eax + 0xcf]
        lea    eax, [ecx + 0x2b]
        push   eax
        push   ecx
        call   sub_2_1FE0
        add    esp, 0x10
        mov    eax, 1
        jmp    loc_2_1869
loc_2_1867:
        sub    eax, eax
loc_2_1869:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1870:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [eax + 4]
        lea    edi, [ecx + 0x28]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [ecx], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1893:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [0x48]              ; -> obj3:0x00C8
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2137
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_2_18BE
        push   2
        add    esi, 0x18
        push   esi
        call   sub_2_2554
        add    esp, 8
loc_2_18BE:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_18C4:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        sub    al, 0x4f
        cmp    al, 1
        pop    ebp
        sbb    eax, eax
        and    eax, 1
        add    eax, 3
        ret    
sub_2_18D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1893
        add    esp, 4
        cmp    eax, esi
        je     loc_2_1971
        push   edi
        call   sub_2_2096
        add    esp, 4
        push   0x64
        lea    eax, [edi + 0x18]
        push   eax
        call   sub_2_2570
        add    esp, 8
        push   dword ptr [0x4c]              ; -> obj3:0x00CC
        push   edi
        call   sub_2_2137
        add    esp, 8
        cmp    eax, esi
        je     loc_2_1973
        push   0x64
        lea    eax, [edi + 0x18]
        push   eax
        call   sub_2_2570
        add    esp, 8
        mov    ebx, dword ptr [ebp - 8]
loc_2_192E:
        lea    eax, [ebp - 1]
        push   eax
        push   edi
        call   sub_2_20FD
        add    esp, 8
        or     al, al
        je     loc_2_194E
        push   dword ptr [ebp - 1]
        call   sub_2_18C4
        add    esp, 4
        mov    ebx, eax
        jmp    loc_2_195C
loc_2_194E:
        lea    eax, [edi + 0x18]
        push   eax
        call   sub_2_2582
        add    esp, 4
        mov    esi, eax
loc_2_195C:
        or     esi, esi
        jne    loc_2_1965
        cmp    ebx, 4
        jne    loc_2_192E
loc_2_1965:
        sub    ebx, 4
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_2_1973
loc_2_1971:
        sub    eax, eax
loc_2_1973:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_197A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        push   dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1870
        add    esp, 8
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_2_1EA0
        add    esp, 8
        or     eax, eax
        je     loc_2_1A3C
        push   0
        push   edi
        call   sub_2_1F5D
        add    esp, 8
        push   0
        push   edi
        call   sub_2_2122
        add    esp, 8
        lea    esi, [edi + 0x18]
        push   0x1f4
        push   esi
        call   sub_2_2554
        add    esp, 8
        push   3
        push   edi
        call   sub_2_2122
        add    esp, 8
        push   0x1f4
        push   esi
        call   sub_2_2554
        add    esp, 8
        mov    esi, 0x50                     ; -> obj3:0x00D0
loc_2_19F2:
        mov    ebx, dword ptr [esi]
        add    esi, 4
        push   ebx
        push   edi
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     loc_2_1A15
        mov    word ptr [edi + 0x26], bx
        push   edi
        call   sub_2_18D8
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_2_1A15:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1A29
        cmp    ebx, 0x2580
        jne    loc_2_19F2
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1A30
loc_2_1A29:
        mov    eax, 1
        jmp    loc_2_1A3E
loc_2_1A30:
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_2_1E74
        add    esp, 8
loc_2_1A3C:
        sub    eax, eax
loc_2_1A3E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1A45:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        lea    eax, [edx + 0x2b]
        mov    edi, eax
        mov    ecx, 0xd
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ax, word ptr [eax]
        mov    word ptr [edx + 0xc], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1A6C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [edi + 0xd7]
        call   sub_2_2050
        add    esp, 4
        lea    esi, [edi + 0xaf]
        push   0                             ; -> loc_1_02C8
        push   esi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     loc_2_1AB3
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 0xcf], eax
        mov    dword ptr [edi + 0xd3], 0     ; -> loc_1_02C8
        mov    eax, 1
        jmp    loc_2_1AB5
loc_2_1AB3:
        sub    eax, eax
loc_2_1AB5:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1AB9:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   2
        mov    eax, dword ptr [esi + 0xd7]
        mov    byte ptr [esi + 0x82], 3
        mov    eax, dword ptr [eax + 0x5b]
        mov    dword ptr [esi + 0x5c], eax
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 0x58], eax
        mov    eax, dword ptr [esi + 0xd7]
        mov    eax, dword ptr [eax + 0x53]
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [esi + 0x28], 2
        mov    dword ptr [esi + 0x2c], 4
        mov    dword ptr [esi + 0x74], 7
        mov    dword ptr [esi + 0x6c], 5
        mov    dword ptr [esi + 0x3c], 0x960
        mov    eax, dword ptr [esi + 0xd7]
        mov    dword ptr [esi + 0x68], 0x96
        movzx  eax, word ptr [eax + 0xc]
        mov    dword ptr [esi + 0x44], eax
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_1B2E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   0x5f
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     loc_2_1BA4
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0xd7]
        call   sub_2_1A45
        add    esp, 8
        push   esi
        call   sub_2_1A6C
        add    esp, 4
        cmp    eax, edi
        je     loc_2_1BA4
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [esi + 0xcf]
        push   eax
        add    eax, 0x2b
        push   eax
        call   sub_2_1F89
        add    esp, 0xc
        cmp    eax, edi
        je     loc_2_1B95
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_1AB9
        add    esp, 8
        mov    edi, 1
        jmp    loc_2_1BA4
loc_2_1B95:
        add    esi, 0xaf
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_1BA4:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1BAA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        call   sub_2_1F1E
        add    esp, 4
        mov    ecx, eax
        cmp    ecx, 1
        sbb    eax, eax
        inc    eax
        je     loc_2_1C12
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ecx + 4]
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        je     loc_2_1C12
        push   0xa
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x43
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   edi
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_1F5D
        add    esp, 8
        or     eax, eax
        je     loc_2_1C12
        movzx  ecx, di
        mov    dword ptr [esi + 0x6c], ecx
loc_2_1C12:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1C16:
        push   ebp
        mov    ebp, esp
        mov    eax, 0x10                     ; -> obj3:0x0090
        mov    edx, dword ptr [ebp + 8]
loc_2_1C21:
        mov    ecx, dword ptr [eax + 8]
        add    eax, 8
        cmp    ecx, edx
        je     loc_2_1C35
        cmp    ecx, 0x96
        jne    loc_2_1C21
        sub    eax, eax
loc_2_1C35:
        pop    ebp
        ret    
sub_2_1C37:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_1C16
        add    esp, 4
        cmp    eax, 1
        sbb    edi, edi
        inc    edi
        je     loc_2_1C89
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x2b
        push   eax
        call   sub_2_2137
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_2_1C89
        movzx  eax, bx
        mov    dword ptr [esi + 0x68], eax
        push   0xa
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x43
        push   eax
        call   sub_2_2554
        add    esp, 8
loc_2_1C89:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_1C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x960
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_16DE
        add    esp, 8
        push   5
        push   esi
        call   sub_2_1BAA
        add    esp, 8
        push   0x96
        push   esi
        call   sub_2_1C37
        add    esp, 8
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_1CC6:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x14], 0     ; -> loc_2_1780
        mov    dword ptr [eax + 0x18], 0     ; -> loc_2_1644
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_2_16BD
        mov    dword ptr [eax + 0x20], 0     ; -> loc_2_16DB
        mov    eax, 1
        ret    
sub_2_1CEF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        lea    ebx, [esi + 0x28]
        push   ebx
        call   sub_2_409F
        add    esp, 4
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_2_197A
        add    esp, 8
        cmp    eax, edi
        je     loc_2_1DA4
        push   ebx
        lea    eax, [ebp - 0x34]
        push   eax
        push   esi
        call   sub_2_1B2E
        add    esp, 0xc
        cmp    eax, edi
        je     loc_2_1D95
        push   esi
        call   sub_2_1C90
        add    esp, 4
        push   esi
        call   sub_2_1CC6
        add    esp, 4
        push   esi
        call   sub_2_4333
        add    esp, 4
        push   esi
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [esi + 0x2c]
        push   esi
        call   sub_2_9371
        add    esp, 8
        mov    eax, dword ptr [esi + 0xd7]
        push   esi
        mov    dword ptr [eax + 0x13], edi
        call   sub_2_07DB
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        push   edi
        push   dword ptr [esi + 0xcf]
        lea    ecx, [eax + 0x2b]
        push   ecx
        push   eax
        call   sub_2_1FE0
        add    esp, 0x10
        mov    edi, 1
        jmp    loc_2_1DA4
loc_2_1D95:
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_2_1E74
        add    esp, 8
loc_2_1DA4:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h
loc_2_1DB0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        cmp    dword ptr [ecx + 0x28], 2
        jne    loc_2_1DCB
        mov    ecx, dword ptr [ecx + 0x38]
        sub    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
loc_2_1DCB:
        pop    ebp
        ret    
sub_2_1DCD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        sub    esi, esi
        push   dword ptr [ebx + 4]
        push   0                             ; -> loc_2_1DB0
        call   sub_2_4594
        add    esp, 8
        cmp    eax, esi
        jne    loc_2_1E0B
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1551
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    loc_2_1E0B
        push   ebx
        push   edi
        call   sub_2_1CEF
        add    esp, 8
        mov    esi, eax
loc_2_1E0B:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_1E12:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        test   byte ptr [ecx + 8], 1
        je     loc_2_1E52
        mov    edx, dword ptr [ecx + 4]
        cmp    edx, eax
        je     loc_2_1E40
        mov    ecx, dword ptr [ecx + 0x10]
loc_2_1E2E:
        cmp    dword ptr [edx + 4], ecx
        je     loc_2_1E3B
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_2_1E2E
        jmp    loc_2_1E40
loc_2_1E3B:
        mov    esi, 1
loc_2_1E40:
        or     esi, esi
        je     loc_2_1E70
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_2_1DCD
        add    esp, 8
        jmp    loc_2_1E70
loc_2_1E52:
        mov    edi, dword ptr [ecx + 4]
        mov    esi, dword ptr [ebp + 8]
loc_2_1E58:
        or     edi, edi
        je     loc_2_1E70
        push   edi
        push   esi
        call   sub_2_1DCD
        add    esp, 8
        or     eax, eax
        jne    loc_2_1E70
        mov    edi, dword ptr [edi]
        or     eax, eax
        je     loc_2_1E58
loc_2_1E70:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1E74:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2030
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 0x22]
        or     eax, eax
        je     loc_2_1E9A
        push   dword ptr [esi + 0x28]
        push   eax
        call   sub_2_21C6
        add    esp, 8
loc_2_1E9A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        call   sub_2_2066
        cmp    eax, 0x30b
        jb     loc_2_1F00
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0x2c]
        call   sub_2_21A7
        add    esp, 4
        cmp    eax, edi
        je     loc_2_1ED7
        push   dword ptr [esi + 0x28]
        call   sub_2_21D8
        add    esp, 4
        mov    dword ptr [esi + 0x22], eax
        cmp    eax, edi
        je     loc_2_1F00
loc_2_1ED7:
        push   esi
        call   sub_2_2071
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_2_1F00
        mov    eax, dword ptr [esi + 0x22]
        or     eax, eax
        je     loc_2_1F00
        push   dword ptr [esi + 0x28]
        push   eax
        call   sub_2_21C6
        add    esp, 8
        mov    dword ptr [esi + 0x22], 0
loc_2_1F00:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_1F06:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_20AA
        add    esp, 8
        mov    eax, 1
        pop    ebp
        ret    
sub_2_1F1E:
        push   ebp
        mov    ebp, esp
        mov    eax, 0xf                      ; -> obj3:0x00EF
        mov    ecx, dword ptr [ebp + 8]
loc_2_1F29:
        mov    edx, dword ptr [eax + 9]
        add    eax, 9
        cmp    ecx, edx
        je     loc_2_1F3A
        cmp    edx, 8
        jne    loc_2_1F29
        sub    eax, eax
loc_2_1F3A:
        pop    ebp
        ret    
sub_2_1F3C:
        push   ebp
        mov    ebp, esp
        mov    eax, 0x48                     ; -> obj3:0x0128
        mov    ecx, dword ptr [ebp + 8]
loc_2_1F47:
        mov    edx, dword ptr [eax + 8]
        add    eax, 8
        cmp    ecx, edx
        je     loc_2_1F5B
        cmp    edx, 0x2580
        jne    loc_2_1F47
        sub    eax, eax
loc_2_1F5B:
        pop    ebp
        ret    
sub_2_1F5D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0xc]
        call   sub_2_1F1E
        add    esp, 4
        cmp    eax, 1
        sbb    esi, esi
        inc    esi
        je     loc_2_1F84
        movzx  eax, byte ptr [eax + 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_20C2
        add    esp, 8
loc_2_1F84:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
sub_2_1F89:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [eax + 0x30]
        call   sub_2_0721
        add    esp, 0xc
        mov    dword ptr [esi + 8], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        pop    ebp
        ret    
sub_2_1FAF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0xb
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2122
        add    esp, 8
        push   dword ptr [ebp + 0x14]
        push   esi
        call   sub_2_20D9
        add    esp, 8
        push   dword ptr [ebp + 0xc]
        call   sub_2_0467
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_1FE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 4], 0xffffff9c
        mov    byte ptr [ecx + 0x12], 0
        mov    byte ptr [ecx + 0xe], 0
        mov    byte ptr [ecx + 0xf], 0
        mov    byte ptr [ecx + 0x10], 0
        mov    byte ptr [ecx + 0x11], 0
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ecx + 8]
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_1FAF
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        je     loc_2_2027
        push   esi
        call   sub_2_2096
        add    esp, 4
loc_2_2027:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_2030:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_2298
        mov    eax, 1
        je     loc_2_204B
        mov    eax, 0
loc_2_204B:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_2050:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0x36c        ; -> loc_1_036C
        leave  
        ret    
sub_2_205F:
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        ret    
sub_2_2066:
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        jae    loc_2_2070
        xor    eax, eax
loc_2_2070:
        ret    
sub_2_2071:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [edi]
        lea    edi, [edi]
        call   sub_2_24F2
        mov    eax, 1
        je     loc_2_2091
        mov    eax, 0
loc_2_2091:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_2096:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_247F
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_20AA:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0xc]
        call   sub_2_221F
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_20C2:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        call   sub_2_21F6
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_20D9:
        enter  0, 0
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, 1
        cmp    ebx, 0
        je     loc_2_20F4
        mov    eax, 9
loc_2_20F4:
        call   sub_2_2258
        pop    ebx
        pop    edi
        leave  
        ret    
sub_2_20FD:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_247B
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [edi], al
        mov    eax, 1
        je     loc_2_211D
        mov    eax, 0
loc_2_211D:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_2122:
        enter  0, 0
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_2_2250
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_2137:
        enter  0, 0
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_2_2459
        mov    eax, 1
        je     loc_2_2154
        mov    eax, 0
loc_2_2154:
        pop    ebx
        pop    edi
        leave  
        ret    
sub_2_2158:
        pushal 
        mov    ebp, esp
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        cmp    eax, 0x400
        jb     loc_2_21A5
        mov    eax, ecx
        test   edx, edx
        je     loc_2_2180
        push   eax
        mov    eax, edx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VCD.VCD_Free_Port_Windows_Style ; cd2007000e00
        pop    eax
        jmp    loc_2_219F
loc_2_2180:
        mov    dword ptr [ebp + 0x1c], edx
        push   eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   4
        pop    ecx
        xor    esi, esi
        xor    edi, edi
        VxDCall  VCD.VCD_Acquire_Port_Windows_Style ; cd2006000e00
        mov    ebx, eax
        pop    eax
        jb     loc_2_21A5
        mov    dword ptr [ebp + 0x1c], ebx
loc_2_219F:
        VxDCall  VCD.VCD_Set_Port_Global_Special ; cd200a000e00
loc_2_21A5:
        popal  
        ret    
sub_2_21A7:
        push   ebp
        mov    ebp, esp
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        cmp    eax, 0x400
        jb     loc_2_21C2
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VCD.VCD_Find_COM_Index          ; cd2009000e00
        jae    loc_2_21C4
loc_2_21C2:
        xor    eax, eax
loc_2_21C4:
        leave  
        ret    
sub_2_21C6:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_2158
        sbb    eax, eax
        leave  
        ret    
sub_2_21D8:
        push   ebp
        mov    ebp, esp
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_2158
        leave  
        ret    
        db  000h
sub_2_21E8:
        xor    dh, dh
        add    dx, word ptr [edi]
        in     al, dx
        ret    
sub_2_21EF:
        xor    dh, dh
        add    dx, word ptr [edi]
        out    dx, al
        ret    
sub_2_21F6:
        mov    dl, 3
        call   sub_2_21EF
        ret    
sub_2_21FE:
        mov    dl, 3
        call   sub_2_21E8
        ret    
sub_2_2206:
        or     al, al
        je     loc_2_220C
        mov    al, 0xff
loc_2_220C:
        and    al, 0x80
        push   eax
        call   sub_2_21FE
        and    al, 0x7f
        pop    ecx
        or     al, cl
        call   sub_2_21F6
        ret    
sub_2_221F:
        mov    cx, ax
        mov    dx, 1
        mov    ax, 0xc200
        div    cx
        push   eax
        mov    al, 0xff
        call   sub_2_2206
        pop    eax
        push   eax
        mov    dl, 0
        call   sub_2_21EF
        pop    eax
        xchg   ah, al
        mov    dl, 1
        call   sub_2_21EF
        xor    al, al
        call   sub_2_2206
        ret    
sub_2_2250:
        mov    dl, 4
        call   sub_2_21EF
        ret    
sub_2_2258:
        mov    dl, 1
        call   sub_2_21EF
        ret    
sub_2_2260:
        mov    dl, 2
        call   sub_2_21EF
        ret    
sub_2_2268:
        mov    al, byte ptr [ebx + 6]
        call   sub_2_2258
        mov    al, byte ptr [ebx + 4]
        call   sub_2_21F6
        mov    al, byte ptr [ebx + 5]
        call   sub_2_2250
        mov    eax, dword ptr [ebx]
        call   sub_2_221F
        mov    cl, byte ptr [ebx + 7]
        mov    al, 1
        or     cl, cl
        jne    loc_2_2292
        mov    al, 0
loc_2_2292:
        call   sub_2_2260
        ret    
sub_2_2298:
        push   edi
        push   ebx
        lea    ebx, [edi + 8]
        call   sub_2_2268
        xor    ebx, ebx
        pop    ebx
        pop    edi
        ret    
sub_2_22A7:
        push   ebx
        xor    bl, bl
        test   al, 0x1e
        je     loc_2_22D0
        inc    bl
        test   al, 2
        je     loc_2_22B8
        inc    word ptr [edi + 0x10]
loc_2_22B8:
        test   al, 4
        je     loc_2_22C0
        inc    word ptr [edi + 0x14]
loc_2_22C0:
        test   al, 8
        je     loc_2_22C8
        inc    word ptr [edi + 0x16]
loc_2_22C8:
        test   al, 0x10
        je     loc_2_22D0
        inc    word ptr [edi + 0x12]
loc_2_22D0:
        or     bl, bl
        pop    ebx
        ret    
sub_2_22D4:
        xor    ax, ax
        mov    word ptr [edi + 0x10], ax
        mov    word ptr [edi + 0x12], ax
        mov    word ptr [edi + 0x16], ax
        mov    word ptr [edi + 0x14], ax
        ret    
        db  066h, 08Bh, 047h, 010h, 066h, 089h, 003h, 066h, 08Bh, 047h, 012h, 066h, 089h, 043h, 002h, 066h
        db  08Bh, 047h, 016h, 066h, 089h, 043h, 006h, 066h, 08Bh, 047h, 014h, 066h, 089h, 043h, 004h, 0C3h
sub_2_2308:
        mov    dl, 5
        call   sub_2_21E8
        push   eax
        call   sub_2_22A7
        pop    eax
        ret    
sub_2_2317:
        call   sub_2_2308
        not    al
        and    al, 1
        ret    
sub_2_2321:
        call   sub_2_2308
        not    al
        and    al, 0x20
        ret    
sub_2_232B:
        call   sub_2_2308
        not    al
        and    al, 0x40
        ret    
sub_2_2335:
        call   sub_2_2321
        jne    loc_2_2341
        call   sub_2_232B
loc_2_2341:
        ret    
sub_2_2342:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     loc_2_2352
        call   sub_2_2375
        not    al
        and    al, 0x10
loc_2_2352:
        ret    
sub_2_2353:
        or     eax, eax
        je     loc_2_235C
        mov    eax, 0xffffffff
loc_2_235C:
        mov    dword ptr [edi + 4], eax
        ret    
sub_2_2360:
        call   sub_2_2335
        jne    loc_2_236C
        call   sub_2_2342
loc_2_236C:
        ret    
sub_2_236D:
        mov    dl, 4
        call   sub_2_21E8
        ret    
sub_2_2375:
        mov    dl, 6
        call   sub_2_21E8
        ret    
sub_2_237D:
        mov    dl, 1
        call   sub_2_21E8
        ret    
sub_2_2385:
        mov    dl, 2
        call   sub_2_21E8
        ret    
sub_2_238D:
        mov    al, 0xff
        call   sub_2_2206
        mov    dl, 1
        call   sub_2_21E8
        mov    ch, al
        mov    dl, 0
        call   sub_2_21E8
        mov    cl, al
        call   sub_2_23C6
        jne    loc_2_23B9
        mov    dx, 1
        mov    ax, 0xc200
        div    cx
        cwde   
loc_2_23B9:
        push   eax
        xor    al, al
        call   sub_2_2206
        pop    eax
        xor    cx, cx
        ret    
sub_2_23C6:
        cmp    cx, 2
        ja     loc_2_23D0
        mov    cx, 0x60
loc_2_23D0:
        xor    ax, ax
        ret    
sub_2_23D4:
        mov    dl, 2
        call   sub_2_21E8
        not    al
        test   al, 0xc0
        mov    al, 0
        jne    loc_2_23E5
        dec    al
loc_2_23E5:
        ret    
        db  0B0h, 001h, 0E8h, 073h, 0FEh, 0FFh, 0FFh, 0E8h, 0E2h, 0FFh, 0FFh, 0FFh, 066h, 09Ch, 0B0h, 000h
        db  0E8h, 065h, 0FEh, 0FFh, 0FFh, 066h, 09Dh, 0C3h, 066h, 08Bh, 007h, 0C3h
sub_2_2402:
        mov    word ptr [edi], ax
        ret    
sub_2_2406:
        mov    dl, 0
        call   sub_2_21E8
        ret    
sub_2_240E:
        mov    word ptr [edi + 2], ax
        ret    
sub_2_2413:
        mov    ax, word ptr [edi + 2]
        ret    
sub_2_2418:
        mov    dl, 0
        call   sub_2_21EF
        ret    
sub_2_2420:
        push   eax
        call   sub_2_2413
        push   edi
        lea    edi, [edi + 0x18]
        push   eax
        push   edi
        call   sub_2_2570
        add    esp, 8
        pop    edi
loc_2_2435:
        call   sub_2_2360
        je     loc_2_244E
        push   edi
        lea    edi, [edi + 0x18]
        push   edi
        call   sub_2_2582
        add    esp, 4
        pop    edi
        or     eax, eax
        je     loc_2_2435
loc_2_244E:
        pop    eax
        jne    loc_2_2458
        call   sub_2_2418
        xor    eax, eax
loc_2_2458:
        ret    
sub_2_2459:
        mov    al, byte ptr [ebx]
        or     al, al
        je     loc_2_246B
        push   ebx
        call   sub_2_2420
        pop    ebx
        jne    loc_2_246B
        inc    ebx
        jmp    sub_2_2459
loc_2_246B:
        ret    
loc_2_246C:
        call   sub_2_2317
        jne    loc_2_247A
        call   sub_2_2406
        xor    ah, ah
loc_2_247A:
        ret    
sub_2_247B:
        call   dword ptr [edi + 0x1e]
        ret    
sub_2_247F:
        call   sub_2_247B
        je     sub_2_247F
        call   sub_2_2375
        ret    
        db  057h, 08Dh, 07Fh, 018h, 050h, 057h, 0E8h, 0D9h, 000h, 000h, 000h, 083h, 0C4h, 008h, 05Fh, 0E8h
        db  0DBh, 0FFh, 0FFh, 0FFh, 074h, 012h, 057h, 08Dh, 07Fh, 018h, 057h, 0E8h, 0D6h, 000h, 000h, 000h
        db  083h, 0C4h, 004h, 05Fh, 00Bh, 0C0h, 074h, 0E7h, 0C3h
sub_2_24B5:
        call   sub_2_21FE
        mov    byte ptr [ebx + 4], al
        call   sub_2_236D
        mov    byte ptr [ebx + 5], al
        call   sub_2_238D
        or     eax, eax
        je     loc_2_24E5
        mov    dword ptr [ebx], eax
        call   sub_2_23D4
        mov    byte ptr [ebx + 7], al
        call   sub_2_237D
        mov    byte ptr [ebx + 6], al
        mov    eax, 1
loc_2_24E5:
        ret    
sub_2_24E6:
        mov    dword ptr [edi + 0x1e], ebx
        ret    
sub_2_24EA:
        call   sub_2_2385
        test   al, 0x30
        ret    
sub_2_24F2:
        push   edi
        push   ebx
        call   sub_2_2402
        call   sub_2_24EA
        jne    loc_2_2531
        lea    ebx, [edi + 8]
        call   sub_2_24B5
        or     eax, eax
        jne    loc_2_250F
        inc    eax
        jmp    loc_2_2531
loc_2_250F:
        mov    ebx, 0x246c                   ; -> loc_2_246C
        call   sub_2_24E6
        xor    eax, eax
        call   sub_2_2353
        mov    ax, 0x3e8
        call   sub_2_240E
        call   sub_2_22D4
        xor    ax, ax
loc_2_2531:
        pop    ebx
        pop    edi
        ret    
sub_2_2534:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        cmp    eax, dword ptr [edi]
        mov    eax, 1
        ja     loc_2_2548
        mov    eax, 0
loc_2_2548:
        ret    
sub_2_2549:
        mov    dword ptr [edi], eax
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        add    dword ptr [edi], eax
        ret    
sub_2_2554:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_2_2549
loc_2_2563:
        call   sub_2_2534
        cmp    eax, 0
        je     loc_2_2563
        pop    edi
        leave  
        ret    
sub_2_2570:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_2_2549
        pop    edi
        leave  
        ret    
sub_2_2582:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_2534
        pop    edi
        leave  
        ret    
sub_2_2591:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        ret    
sub_2_2598:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 1
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [ebp + 0x10]
loc_2_25AD:
        inc    esi
        mov    eax, dword ptr [edx]
        inc    eax
        mov    dword ptr [edx], eax
        cmp    ecx, esi
        jb     loc_2_25C5
        mov    al, byte ptr [eax]
        cmp    al, 8
        je     loc_2_25C1
        cmp    al, 0x28
        jne    loc_2_25AD
loc_2_25C1:
        cmp    ecx, esi
        jae    loc_2_25C9
loc_2_25C5:
        sub    eax, eax
        jmp    loc_2_260D
loc_2_25C9:
        mov    dword ptr [edx + 8], 4        ; -> obj3:0x0160
        mov    eax, dword ptr [edx]
        mov    byte ptr [edx + 0xc], 0x20
        cmp    byte ptr [eax], 0x28
        jne    loc_2_25E6
        mov    dword ptr [edx + 8], 0        ; -> obj3:0x015C
        mov    byte ptr [edx + 0xc], 0
loc_2_25E6:
        mov    eax, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
loc_2_25EB:
        inc    esi
        mov    eax, dword ptr [edx + 4]
        inc    eax
        mov    dword ptr [edx + 4], eax
        cmp    ecx, esi
        jb     loc_2_2604
        mov    edi, dword ptr [edx + 8]
        mov    al, byte ptr [eax]
        cmp    byte ptr [edi], al
        jne    loc_2_25EB
        cmp    ecx, esi
        jae    loc_2_2608
loc_2_2604:
        sub    eax, eax
        jmp    loc_2_260D
loc_2_2608:
        mov    eax, 1
loc_2_260D:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_2611:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx]
        mov    al, byte ptr [ecx + 0xc]
        add    al, byte ptr [edx]
        inc    edx
        mov    dword ptr [ecx], edx
        ret    
sub_2_2623:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        mov    cl, byte ptr [eax]
        and    ecx, 0x3f
        inc    eax
        mov    dword ptr [esi], eax
        mov    al, byte ptr [eax]
        and    eax, 0x3f
        shl    ecx, 6
        or     eax, ecx
        mov    ecx, 0x64
        cdq    
        idiv   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 4], ax
        inc    dword ptr [esi]
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_2657:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
loc_2_2665:
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 6], al
        inc    ebx
        cmp    ebx, 3
        jb     loc_2_2665
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_2682:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
loc_2_2690:
        push   edi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [esi + ebx + 9], al
        inc    esi
        cmp    esi, 4
        jb     loc_2_2690
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_26AD:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        sub    ecx, 2
        cmp    ecx, edx
        jbe    loc_2_26D3
        mov    eax, dword ptr [eax + 8]
        mov    cl, byte ptr [edx]
        cmp    byte ptr [eax + 1], cl
        je     loc_2_26D3
        cmp    byte ptr [eax + 2], cl
        mov    eax, 0
        jne    loc_2_26D8
loc_2_26D3:
        mov    eax, 1
loc_2_26D8:
        pop    ebp
        ret    
sub_2_26DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
loc_2_26E8:
        push   esi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    loc_2_2708
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 0xd], al
        inc    ebx
        cmp    ebx, 8
        jb     loc_2_26E8
loc_2_2708:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_2712:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
loc_2_2720:
        push   esi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    loc_2_2740
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 0x15], al
        inc    ebx
        cmp    ebx, 9
        jb     loc_2_2720
loc_2_2740:
        mov    byte ptr [edi + ebx + 0x16], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_274F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
loc_2_2764:
        push   edi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    loc_2_27B1
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 8]
        mov    cl, byte ptr [eax]
        cmp    byte ptr [edx + 3], cl
        jne    loc_2_2780
        inc    eax
        mov    dword ptr [edi], eax
loc_2_2780:
        sub    ebx, ebx
loc_2_2782:
        push   edi
        call   sub_2_2611
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, ebx
        mov    byte ptr [esi + ecx + 0x1f], al
        inc    ebx
        cmp    ebx, 7
        jb     loc_2_2782
        add    esi, 7
        inc    dword ptr [ebp - 4]
        cmp    esi, 0x23
        jb     loc_2_2764
loc_2_27A5:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_27B1:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_2_27A5
        mov    ecx, dword ptr [ebp - 4]
        imul   ecx, ecx, 7
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + eax + 0x1f], 0
        jmp    loc_2_27A5
sub_2_27C7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
loc_2_27D5:
        push   esi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    loc_2_27F5
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 0x42], al
        inc    ebx
        cmp    ebx, 0x28
        jb     loc_2_27D5
loc_2_27F5:
        mov    byte ptr [edi + ebx + 0x42], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_2804:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_2598
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_2828
        sub    eax, eax
        jmp    loc_2_28B7
loc_2_2828:
        push   1
        push   0x71
        call   sub_1_12F4
loc_2_2831:
        mov    esi, eax
        mov    eax, dword ptr [ebp + 8]
        add    esp, 8
        mov    dword ptr [eax], esi
        inc    dword ptr [ebp - 0x10]
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_2623
        add    esp, 8
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_2657
        add    esp, 8
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_2682
        add    esp, 8
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [eax + 1], cl
        jne    loc_2_28B2
        inc    dword ptr [ebp - 0x10]
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_26DA
        add    esp, 8
        inc    dword ptr [ebp - 0x10]
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_2712
        add    esp, 8
        inc    dword ptr [ebp - 0x10]
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_274F
        add    esp, 8
        inc    dword ptr [ebp - 0x10]
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_27C7
        add    esp, 8
loc_2_28B2:
        mov    eax, 1
loc_2_28B7:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_28BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, 8                        ; -> obj3:0x0164
        lea    edi, [ebp - 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [ebp + 8]
        push   3
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x7f]
        add    eax, 6
        push   eax
        call   sub_2_6FA8
        add    esp, 0xc
        cmp    eax, ebx
        jne    loc_2_2913
        mov    edi, dword ptr [esi + 0x7f]
        push   0x10
        lea    eax, [edi + 9]
        push   eax
        call   sub_2_71AC
        add    esp, 8
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x7f]
        mov    ebx, 1
        mov    eax, dword ptr [eax]
        and    eax, 0x7fff
        mov    dword ptr [esi + 8], eax
        jmp    loc_2_291C
loc_2_2913:
        mov    eax, dword ptr [esi + 0x7f]
        mov    dword ptr [eax], 0xffffff
loc_2_291C:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2925:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_4399
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2948:
        push   ebp
        mov    ebp, esp
        push   0x10
        push   2
        push   dword ptr [ebp + 8]
        call   sub_2_2EBC
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_295B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [ebp + 8]
        mov    edi, 1
        movzx  eax, si
        mov    dword ptr [ebp - 4], eax
        add    eax, 4
        cmp    eax, dword ptr [ebp - 4]
        jle    loc_2_29B2
loc_2_297B:
        movzx  ebx, si
        push   0
        push   0
        push   ebx
        call   sub_2_0434
        add    esp, 0xc
        push   ebx
        call   sub_2_0653
        add    esp, 4
        or     eax, eax
        je     loc_2_29A1
        or     edi, edi
        mov    edi, 1
        jne    loc_2_29A3
loc_2_29A1:
        sub    edi, edi
loc_2_29A3:
        inc    si
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, si
        add    eax, 4
        cmp    ecx, eax
        jl     loc_2_297B
loc_2_29B2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_29BB:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 4]
        call   sub_2_0458
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 4]
        call   sub_2_0488
        add    esp, 4
        push   esi
        call   sub_2_07B4
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        mov    ax, word ptr [eax + 0xa]
        push   eax
        call   sub_2_295B
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0xa
        push   eax
        call   sub_2_2948
        add    esp, 4
        lea    eax, [esi + 0xaf]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
loc_2_2A3B:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0x91
        push   3
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0xa
        push   eax
        call   sub_2_2EBC
        add    esp, 0xc
        push   esi
        call   sub_2_07DB
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0xa
        push   eax
        call   sub_2_2B2A
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_2A7E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   2
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, 0xde
        je     loc_2_2AA6
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, 0xde
        je     loc_2_2AA6
        jmp    loc_2_2AD6
loc_2_2AA6:
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        mov    bl, al
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, bl
        jne    loc_2_2AD6
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, bl
        mov    eax, 1
        je     loc_2_2AD8
loc_2_2AD6:
        sub    eax, eax
loc_2_2AD8:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_2ADC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x91
        push   3
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2EBC
        add    esp, 0xc
        push   0xa5
        push   1
        push   esi
        call   sub_2_2EBC
        add    esp, 0xc
        push   0x1e
        lea    eax, [esi + 2]
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   1
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, 0xa5
        mov    eax, 1
        je     loc_2_2B27
        sub    eax, eax
loc_2_2B27:
        pop    esi
        pop    ebp
        ret    
sub_2_2B2A:
        push   ebp
        mov    ebp, esp
        push   0
        push   2
        push   dword ptr [ebp + 8]
        call   sub_2_2EBC
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2B3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        lea    esi, [edi + 2]
        push   0xfa
        push   esi
        call   sub_2_2570
        add    esp, 8
        push   2
        push   edi
        call   sub_2_2ED2
        add    esp, 8
        and    al, byte ptr [ebp + 0xc]
        mov    bl, al
loc_2_2B6E:
        push   2
        push   edi
        call   sub_2_2ED2
        add    esp, 8
        and    al, byte ptr [ebp + 0xc]
        xor    bl, al
        or     byte ptr [ebp - 1], bl
        mov    bl, al
        push   esi
        call   sub_2_2582
        add    esp, 4
        or     eax, eax
        je     loc_2_2B6E
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2B9A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2A7E
        add    esp, 4
        or     eax, eax
        jne    loc_2_2C0E
        push   esi
        call   sub_2_2ADC
        add    esp, 4
        or     eax, eax
        je     loc_2_2C0E
        mov    edi, 5
loc_2_2BC2:
        push   esi
        call   sub_2_2948
        add    esp, 4
        push   0xff
        push   esi
        call   sub_2_2B3D
        add    esp, 8
        mov    bl, al
        push   esi
        call   sub_2_2B2A
        add    esp, 4
        not    bl
        push   ebx
        push   esi
        call   sub_2_2B3D
        add    esp, 8
        mov    bl, al
        push   esi
        call   sub_2_2948
        add    esp, 4
        push   ebx
        push   esi
        call   sub_2_2EE5
        add    esp, 8
        or     eax, eax
        jne    loc_2_2C10
        dec    edi
        jne    loc_2_2BC2
        jmp    loc_2_2C10
loc_2_2C0E:
        sub    eax, eax
loc_2_2C10:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_2C15:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0                             ; -> loc_1_0408
        lea    edi, [esi + 0xaf]
        push   edi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     loc_2_2C4E
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 0xcf], eax
        mov    dword ptr [esi + 0xd3], 0     ; -> loc_1_0408
        mov    eax, 1
        jmp    loc_2_2C50
loc_2_2C4E:
        sub    eax, eax
loc_2_2C50:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_2C54:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 8], 2
        jne    loc_2_2C64
        mov    byte ptr [eax + 8], 9
loc_2_2C64:
        pop    ebp
        ret    
sub_2_2C66:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, eax
        mov    edi, dword ptr [ecx + 0xd7]
        add    edi, 0xa
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [ecx + 0xd7]
        mov    ax, word ptr [eax]
        mov    word ptr [ecx + 8], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_2C95:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0x28]
        push   esi
        call   sub_2_409F
        add    esp, 4
        mov    eax, dword ptr [edi + 0xd7]
        push   1
        movzx  eax, byte ptr [eax + 0x12]
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0x10], 1
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 0x30], eax
        mov    byte ptr [esi + 0x5a], 0
        mov    word ptr [esi + 0x52], 0
        mov    dword ptr [esi], 1
        mov    eax, 1
        mov    dword ptr [esi + 4], 1
        mov    dword ptr [esi + 0x40], 0x3c
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_2CF4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd7]
        movzx  esi, word ptr [eax + 8]
        lea    edi, [esi + 4]
        cmp    esi, edi
        jae    loc_2_2D31
loc_2_2D0D:
        push   0                             ; -> loc_1_04F0
        push   esi
        call   sub_2_0633
        add    esp, 8
        or     eax, eax
        je     loc_2_2D2C
        push   0
        push   0
        push   esi
        call   sub_2_0668
        add    esp, 0xc
loc_2_2D2C:
        inc    esi
        cmp    esi, edi
        jb     loc_2_2D0D
loc_2_2D31:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_2D3A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_2C54
        add    esp, 4
        push   0x13
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     loc_2_2DC6
        push   esi
        push   ebx
        call   sub_2_2C66
        add    esp, 8
        push   esi
        call   sub_2_2C15
        add    esp, 4
        cmp    eax, edi
        je     loc_2_2DC6
        mov    ebx, dword ptr [esi + 0xd7]
        push   edi
        push   dword ptr [esi + 0xcf]
        mov    al, byte ptr [ebx + 0x12]
        push   eax
        call   sub_2_0721
        add    esp, 0xc
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0xd7]
        cmp    dword ptr [eax + 4], edi
        je     loc_2_2DB7
        push   esi
        call   sub_2_2C95
        add    esp, 4
        push   esi
        call   sub_2_2CF4
        add    esp, 4
        mov    edi, 1
        jmp    loc_2_2DC6
loc_2_2DB7:
        add    esi, 0xaf
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_2DC6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_2DCD:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 4]
        call   sub_2_0467
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0xa
        push   eax
        call   sub_2_2B2A
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
loc_2_2DFF:
        sub    eax, eax
        ret    
loc_2_2E02:
        sub    eax, eax
        ret    
sub_2_2E05:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x14], 0     ; -> loc_2_2A3B
        mov    dword ptr [eax + 0x18], 0     ; -> loc_2_29BB
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_2_2E02
        mov    dword ptr [eax + 0x20], 0     ; -> loc_2_2DFF
        mov    eax, 1
        ret    
loc_2_2E2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        sub    esi, esi
        mov    word ptr [ebp - 0xc], 0x23c
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2B9A
        add    esp, 4
        cmp    eax, esi
        je     loc_2_2EB3
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_2D3A
        add    esp, 0xc
        cmp    eax, esi
        je     loc_2_2EA3
        push   edi
        call   sub_2_2E05
        add    esp, 4
        push   edi
        call   sub_2_4333
        add    esp, 4
        push   edi
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [edi + 0x2c]
        push   edi
        call   sub_2_9371
        add    esp, 8
        push   edi
        call   sub_2_07DB
        add    esp, 4
        push   edi
        call   sub_2_2DCD
        add    esp, 4
        mov    esi, 1
loc_2_2EA3:
        or     esi, esi
        jne    loc_2_2EB3
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2948
        add    esp, 4
loc_2_2EB3:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_2EBC:
        enter  0, 0
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    dx, word ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        pop    edi
        leave  
        ret    
sub_2_2ED2:
        enter  0, 0
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    dx, word ptr [ebp + 0xc]
        in     al, dx
        pop    edi
        leave  
        ret    
sub_2_2EE5:
        enter  0, 0
        push   edi
        push   ebx
        xor    eax, eax
        movzx  ebx, byte ptr [ebp + 0xc]
        cmp    ebx, 8
        ja     loc_2_2F06
        mov    al, byte ptr [ebx + 0x168]    ; -> obj3:0x0168
        or     al, al
        je     loc_2_2F06
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 8], al
loc_2_2F06:
        pop    ebx
        pop    edi
        leave  
        ret    
        db  000h, 000h
loc_2_2F0C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        call   sub_1_1304
loc_2_2F16:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, eax
        mov    eax, dword ptr [esi + 0xd7]
        push   dword ptr [eax + 0xc]
        call   sub_2_0488
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x10
        push   eax
        push   0                             ; -> loc_4_0000
        push   0                             ; -> loc_1_05CC
        call   sub_2_39A7
        add    esp, 0xc
        call   sub_2_374C
        push   edi
        call   sub_2_3868
        add    esp, 4
        call   sub_2_3840
        push   esi
        call   sub_2_07B4
        add    esp, 4
        lea    eax, [esi + 0xaf]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_2F8E:
        push   ebp
        mov    ebp, esp
        call   sub_1_1304
loc_2_2F96:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_3208
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2FA3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        mov    al, cl
        and    al, 0x10
        sub    al, 0x10
        cmp    al, 1
        mov    edx, dword ptr [ebp + 8]
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    word ptr [edx + 0x69], ax
        mov    al, cl
        and    al, 0x20
        sub    al, 0x20
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 0x73], eax
        mov    al, cl
        and    al, 0x80
        sub    al, 0x80
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 0x6b], eax
        and    cl, 0x40
        sub    cl, 0x40
        cmp    cl, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 0x6f], eax
        mov    eax, 1
        pop    ebp
        ret    
sub_2_2FF5:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx]
        and    al, 0x30
        shr    al, 4
        movzx  dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    word ptr [eax + 0x65], dx
        mov    cl, byte ptr [ecx]
        shr    cl, 6
        push   ecx
        call   sub_2_3FD6
        add    esp, 8
        mov    eax, 1
        pop    ebp
        ret    
sub_2_3023:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    al, byte ptr [edi]
        cmp    byte ptr [edi + 1], 1
        jne    loc_2_3064
        mov    esi, dword ptr [ebp + 8]
        and    ax, 7
        mov    word ptr [esi + 0x67], ax
        push   esi
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        push   edi
        push   esi
        jne    loc_2_3055
        call   sub_2_2FA3
        jmp    loc_2_305A
loc_2_3055:
        call   sub_2_2FF5
loc_2_305A:
        add    esp, 8
        mov    eax, 1
        jmp    loc_2_3066
loc_2_3064:
        sub    eax, eax
loc_2_3066:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_306A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    al, byte ptr [esi + 1]
        or     al, al
        je     loc_2_30C0
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 0x5a], al
        mov    cl, byte ptr [esi]
        mov    al, cl
        and    ax, 0xf
        shl    ax, 3
        and    cl, 0x70
        shr    cl, 4
        movzx  cx, cl
        or     ax, cx
        mov    word ptr [edx + 0x52], ax
        movzx  ax, byte ptr [esi + 2]
        mov    word ptr [edx + 0x50], ax
        mov    cl, byte ptr [esi]
        shr    cl, 7
        mov    eax, dword ptr [edx + 0x7b]
        movzx  ecx, cl
        xor    ecx, eax
        and    ecx, 1
        xor    ecx, eax
        mov    dword ptr [edx + 0x7b], ecx
        mov    eax, 1
        jmp    loc_2_30C2
loc_2_30C0:
        sub    eax, eax
loc_2_30C2:
        pop    esi
        pop    ebp
        ret    
loc_2_30C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_1334
loc_2_30D3:
        mov    edi, dword ptr [ebp + 8]
        mov    esi, eax
        lea    ebx, [edi + 0x28]
        push   ebx
        call   sub_2_3F1C
        add    esp, 4
        or     eax, eax
        jne    loc_2_30EF
        sub    eax, eax
        jmp    loc_2_3201
loc_2_30EF:
        push   ebx
        call   sub_2_3F69
        add    esp, 4
        or     eax, eax
        jne    loc_2_3101
        jmp    loc_2_31FC
loc_2_3101:
        call   sub_2_374C
        push   esi
        push   0
        call   sub_2_38A3
        add    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_37A4
        add    esp, 8
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        call   sub_2_3023
        add    esp, 8
        push   ebx
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        jne    loc_2_31CF
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_306A
        add    esp, 8
        movzx  eax, word ptr [ebx + 0x52]
        push   eax
        push   ebx
        call   sub_2_4026
        add    esp, 8
        cmp    word ptr [ebx + 0x69], 2
        jne    loc_2_31CF
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        cmp    dword ptr [ebx + 0x83], 0
        jne    loc_2_31A4
        push   1
        push   0x87
        call   sub_1_12F4
loc_2_318E:
        add    esp, 8
        mov    dword ptr [ebx + 0x83], eax
        or     eax, eax
        je     loc_2_31A4
        push   eax
        call   sub_2_409F
        add    esp, 4
loc_2_31A4:
        mov    eax, dword ptr [ebx + 0x83]
        or     eax, eax
        je     loc_2_31CF
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x83]
        movzx  ecx, word ptr [eax + 0x52]
        push   ecx
        push   eax
        call   sub_2_4026
        add    esp, 8
loc_2_31CF:
        push   esi
        push   3
        call   sub_2_38A3
        add    esp, 8
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        push   edi
        call   sub_2_07DB
        add    esp, 4
        push   edi
        call   sub_2_3448
        add    esp, 4
        call   sub_2_3840
loc_2_31FC:
        mov    eax, 1
loc_2_3201:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_3208:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        call   sub_2_374C
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   3
        call   sub_2_32DC
        add    esp, 8
        cmp    eax, edi
        je     loc_2_3251
        push   esi
        call   sub_2_35EC
        add    esp, 4
        cmp    eax, edi
        je     loc_2_3251
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_07DB
        add    esp, 4
        push   esi
        call   sub_2_35CE
        add    esp, 4
        cmp    eax, edi
        je     loc_2_3251
        mov    edi, 1
loc_2_3251:
        call   sub_2_3840
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_325C:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    esi, esi
        mov    dword ptr [0], esi            ; -> obj1:0x13F4
        test   byte ptr [ebp + 8], 8
        jne    loc_2_3282
        push   dword ptr [ebp + 0xc]
        call   sub_2_3E9C
        add    esp, 4
        cmp    eax, esi
        je     loc_2_3282
        mov    esi, 1
loc_2_3282:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
sub_2_3287:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_325C
        add    esp, 8
        or     eax, eax
        je     loc_2_32B1
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 1
        mov    eax, 1
        jmp    loc_2_32D7
loc_2_32B1:
        test   byte ptr [ebp + 8], 4
        je     loc_2_32C1
        mov    eax, dword ptr [esi + 4]
        or     byte ptr [eax + 0x410], 4
loc_2_32C1:
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        push   0x11
        call   sub_1_05E4
loc_2_32CD:
        add    esp, 0xc
        mov    ax, word ptr [ebp - 4]
        and    eax, 4
loc_2_32D7:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_32DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_38CB
        add    esp, 8
        or     eax, eax
        je     loc_2_3318
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_387B
        add    esp, 8
        or     eax, eax
        je     loc_2_3318
        push   0xc
        call   sub_2_0476
        add    esp, 4
        mov    eax, 1
        jmp    loc_2_331A
loc_2_3318:
        sub    eax, eax
loc_2_331A:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_331F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   0
        call   sub_2_38A3
        mov    esi, dword ptr [ebp + 8]
        add    esp, 8
        lea    eax, [esi + 0x54]
        push   eax
        push   0
        push   0
        push   4
        call   sub_2_3DEC
        add    esp, 0x10
        mov    dword ptr [esi + 0x56], eax
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        or     eax, eax
        je     loc_2_3418
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_2_306A
        add    esp, 8
        movzx  eax, word ptr [esi + 0x52]
        push   eax
        push   esi
        call   sub_2_4026
        add    esp, 8
        test   byte ptr [esi + 0x7b], 1
        je     loc_2_3418
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_37A4
        add    esp, 8
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_2_3023
        add    esp, 8
        push   esi
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        jne    loc_2_3418
        cmp    word ptr [esi + 0x69], 2
        jne    loc_2_3418
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        push   1
        push   0x87
        call   sub_1_12F4
loc_2_33DC:
        add    esp, 8
        mov    dword ptr [esi + 0x83], eax
        or     eax, eax
        je     loc_2_3418
        push   eax
        call   sub_2_409F
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 0x83]
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [esi + 0x83]
        movzx  ecx, word ptr [eax + 0x52]
        push   ecx
        push   eax
        call   sub_2_4026
        add    esp, 8
loc_2_3418:
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_2_3429:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_409F
        add    esp, 4
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_331F
        add    esp, 8
        pop    esi
        pop    ebp
        ret    
sub_2_3448:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 0xd7]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [eax + 0xd7]
        mov    dword ptr [eax + 4], 0
        ret    
sub_2_3469:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_3448
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        mov    byte ptr [eax + 0x14], 0xc
        mov    eax, dword ptr [esi + 0xd7]
        and    dword ptr [eax + 0x10], 0xfffffffe
        mov    eax, dword ptr [esi + 0xd7]
        pop    esi
        and    dword ptr [eax + 0x10], 0xfffffffd
        mov    eax, 1
        pop    ebp
        ret    
sub_2_349F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0                             ; -> loc_1_04F8
        lea    edi, [esi + 0xaf]
        push   edi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     loc_2_34D8
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 0xcf], eax
        mov    dword ptr [esi + 0xd3], 0     ; -> loc_1_04F8
        mov    eax, 1
        jmp    loc_2_34DA
loc_2_34D8:
        sub    eax, eax
loc_2_34DA:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_34DE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        push   4
        mov    eax, dword ptr [ecx + 0xd7]
        lea    esi, [ecx + 0x28]
        movzx  eax, byte ptr [eax + 0x14]
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [ecx + 0x38], 1
        mov    dword ptr [esi], 4
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 0x30], eax
        mov    dword ptr [esi + 0x40], 0x28
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_3521:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        push   0x15
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     loc_2_35C7
        push   esi
        call   sub_2_3469
        add    esp, 4
        push   esi
        call   sub_2_349F
        add    esp, 4
        cmp    eax, edi
        je     loc_2_35C7
        push   dword ptr [ebp + 0xc]
        call   sub_2_3855
        add    esp, 4
        mov    ebx, dword ptr [esi + 0xd7]
        push   edi
        push   dword ptr [esi + 0xcf]
        mov    al, byte ptr [ebx + 0x14]
        push   eax
        call   sub_2_0721
        add    esp, 0xc
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [esi + 0xd7]
        cmp    dword ptr [eax + 0xc], edi
        je     loc_2_35B8
        call   sub_2_3840
        push   esi
        call   sub_2_34DE
        add    esp, 4
        mov    eax, dword ptr [esi + 0xd7]
        add    eax, 0x10
        push   eax
        push   0                             ; -> loc_4_0000
        push   0                             ; -> loc_1_05CC
        call   sub_2_3925
        add    esp, 0xc
        mov    edi, 1
        jmp    loc_2_35C7
loc_2_35B8:
        add    esi, 0xaf
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_35C7:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_35CE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd7]
        push   dword ptr [eax + 0xc]
        call   sub_2_0467
        add    esp, 4
        mov    eax, 1
        pop    ebp
        ret    
sub_2_35EC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   3
        call   sub_2_38A3
        add    esp, 8
        or     eax, eax
        je     loc_2_3627
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_3627
        push   esi
        push   3
        push   2
        call   sub_2_388F
        add    esp, 0xc
        or     eax, eax
        mov    eax, 1
        jne    loc_2_3629
loc_2_3627:
        sub    eax, eax
loc_2_3629:
        pop    esi
        pop    ebp
        ret    
sub_2_362C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_35EC
        add    esp, 4
        or     eax, eax
        je     loc_2_3658
        push   esi
        push   0                             ; -> loc_2_3A20
        call   sub_2_38DF
        add    esp, 8
        cmp    eax, 2
        mov    eax, 1
        je     loc_2_365A
loc_2_3658:
        sub    eax, eax
loc_2_365A:
        pop    esi
        pop    ebp
        ret    
loc_2_365D:
        sub    eax, eax
        ret    
sub_2_3660:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x14], 0     ; -> loc_2_2F8E
        mov    dword ptr [eax + 0x18], 0     ; -> loc_2_2F0C
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_2_365D
        mov    dword ptr [eax + 0x20], 0     ; -> loc_2_30C5
        mov    eax, 1
        ret    
loc_2_3689:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
        call   sub_1_1304
loc_2_36A0:
        mov    esi, eax
        call   sub_2_374C
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        push   dword ptr [eax + 8]
        call   sub_2_3287
        add    esp, 0xc
        cmp    eax, edi
        je     loc_2_3739
        push   esi
        push   3
        call   sub_2_32DC
        add    esp, 8
        cmp    eax, edi
        je     loc_2_3739
        mov    ebx, dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        lea    eax, [ebx + 0x28]
        push   esi
        push   eax
        call   sub_2_3429
        add    esp, 0xc
        push   esi
        call   sub_2_362C
        add    esp, 4
        cmp    eax, edi
        je     loc_2_3739
        push   esi
        push   ebx
        call   sub_2_3521
        add    esp, 8
        cmp    eax, edi
        je     loc_2_3739
        push   ebx
        call   sub_2_3660
        add    esp, 4
        push   ebx
        call   sub_2_4333
        add    esp, 4
        push   ebx
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [ebx + 0x2c]
        push   ebx
        call   sub_2_9371
        add    esp, 8
        push   ebx
        call   sub_2_07DB
        add    esp, 4
        push   ebx
        call   sub_2_35CE
        add    esp, 4
        mov    edi, 1
loc_2_3739:
        or     edi, edi
        jne    loc_2_3742
        call   sub_2_3840
loc_2_3742:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_374C:
        push   esi
        call   sub_2_0517
        call   sub_2_3A27
        call   sub_2_0553
        mov    esi, eax
        or     esi, esi
        je     loc_2_3769
        call   sub_2_059E
        jmp    loc_2_376E
loc_2_3769:
        call   sub_2_0535
loc_2_376E:
        mov    eax, esi
        pop    esi
        ret    
sub_2_3772:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     loc_2_378C
        push   dword ptr [ebp + 0xc]
        push   eax
        push   6
        call   sub_2_3A71
        add    esp, 0xc
        jmp    loc_2_378E
loc_2_378C:
        sub    eax, eax
loc_2_378E:
        pop    ebp
        ret    
sub_2_3790:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   6
        call   sub_2_3A32
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_37A4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   2
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_37ED
        push   esi
        push   2
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_37ED
        push   esi
        push   2
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_37ED
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_3790
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    loc_2_37EF
loc_2_37ED:
        sub    eax, eax
loc_2_37EF:
        pop    esi
        pop    ebp
        ret    
sub_2_37F2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_383B
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_383B
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     loc_2_383B
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_3790
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    loc_2_383D
loc_2_383B:
        sub    eax, eax
loc_2_383D:
        pop    esi
        pop    ebp
        ret    
sub_2_3840:
        call   sub_2_05A5
        call   sub_2_0574
        call   sub_2_0535
        mov    eax, 1
        ret    
sub_2_3855:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   1
        push   0
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_3868:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0
        push   0
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_387B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   1
        call   sub_2_3A32
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_388F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        push   2
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_38A3:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   3
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   4
        call   sub_2_3A32
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_38CB:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   5
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_38DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_068C
        add    esp, 0xc
        or     eax, eax
        je     loc_2_391C
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 4]
        push   7
        call   sub_2_3A71
        add    esp, 0xc
        or     eax, eax
        je     loc_2_3915
        mov    eax, 2
        jmp    loc_2_391E
loc_2_3915:
        mov    eax, 1
        jmp    loc_2_391E
loc_2_391C:
        sub    eax, eax
loc_2_391E:
        mov    esp, ebp
        pop    ebp
        ret    
        db  02Bh, 0C0h, 0C3h
sub_2_3925:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   dword ptr [ebp + 8]
        push   0x15
        call   sub_2_06CE
        add    esp, 8
        or     eax, eax
        je     loc_2_39A0
        mov    esi, dword ptr [ebp + 0x10]
        push   0                             ; -> loc_4_0034
        or     dword ptr [esi], 1
        push   0
        push   dword ptr [ebp + 0xc]
        call   sub_2_06A8
        add    esp, 0xc
        or     eax, eax
        je     loc_2_398F
        push   0                             ; -> loc_4_0038
        push   0x15
        call   sub_2_06EB
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> loc_4_0034
        shr    eax, 0x10
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [0]             ; -> loc_4_0034
        mov    dword ptr [ebp - 6], eax
        lea    eax, [ebp - 8]
        push   eax
        push   0x15
        call   sub_2_0709
        add    esp, 8
        or     dword ptr [esi], 2
loc_2_398F:
        mov    dword ptr [0], 1              ; -> loc_4_003E
        mov    eax, 1
        jmp    loc_2_39A2
loc_2_39A0:
        sub    eax, eax
loc_2_39A2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_39A7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        test   byte ptr [esi], 1
        je     loc_2_39C7
        push   dword ptr [ebp + 8]
        push   0x15
        call   sub_2_05AC
        add    esp, 8
        or     eax, eax
        je     loc_2_39C7
        and    dword ptr [esi], 0xfffffffe
loc_2_39C7:
        mov    dword ptr [0], 0              ; -> loc_4_003E
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
loc_2_39DC:
        cmp    dword ptr [0x13f8], 0         ; -> obj1:0x13F8
        jne    loc_1_05DD
        movzx  eax, al
        cmp    eax, 7
        ja     loc_2_39F3
        jmp    loc_2_3A09
loc_2_39F3:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 1
        clc    
        ret    
loc_2_39FE:
        and    word ptr [ebp + 0x2c], 0xfffe
        mov    byte ptr [ebp + 0x1d], 0
        clc    
        ret    
loc_2_3A09:
        cmp    eax, 7
        je     loc_2_39FE
        mov    al, byte ptr [eax + 0x13fc]   ; -> obj1:0x13FC
        mov    byte ptr [ebp + 0x1d], al
        or     word ptr [ebp + 0x2c], 1
        clc    
        ret    
        db  08Bh, 0C0h
loc_2_3A20:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
sub_2_3A27:
        mov    dword ptr [0x1ac], 0x14       ; -> obj3:0x01AC
        ret    
sub_2_3A32:
        enter  0, 0
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        mov    ebx, 0
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [esi + 8]
        call   sub_2_3A98
        mov    eax, 0
        jb     loc_2_3A6C
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [edi], al
        mov    al, byte ptr [esi + 0x18]
        mov    byte ptr [edi + 1], al
        mov    al, byte ptr [esi + 0x14]
        mov    byte ptr [edi + 2], al
        mov    eax, 1
loc_2_3A6C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3A71:
        enter  0, 0
        push   esi
        push   ebx
        mov    al, byte ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [esi + 8]
        call   sub_2_3A98
        mov    eax, 1
        jae    loc_2_3A94
        mov    eax, 0
loc_2_3A94:
        pop    ebx
        pop    esi
        leave  
        ret    
sub_2_3A98:
        inc    dword ptr [0x13f8]            ; -> obj1:0x13F8
loc_2_3A9E:
        mov    byte ptr [esi + 0x1c], al
        mov    byte ptr [esi + 0x1d], 0xc2
        cmp    al, 7
        jne    loc_2_3AB6
        mov    word ptr [esi + 0x10], bx
        shr    ebx, 0x10
        mov    word ptr [esi + 0x38], bx
        jmp    loc_2_3AB9
loc_2_3AB6:
        mov    byte ptr [esi + 0x11], bl
loc_2_3AB9:
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     dword ptr [esi + 0x2c], 0
        jae    loc_2_3ADA
        dec    dword ptr [0x1ac]             ; -> obj3:0x01AC
        jle    loc_2_3AD9
        cmp    byte ptr [esi + 0x1d], 4
        je     loc_2_3A9E
loc_2_3AD9:
        stc    
loc_2_3ADA:
        dec    dword ptr [0x13f8]            ; -> obj1:0x13F8
        ret    
        db  000h, 000h, 000h
sub_2_3AE4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        sub    esi, esi
        call   sub_2_3B43
        cmp    eax, esi
        je     loc_2_3B3A
        mov    word ptr [ebp - 4], 0xe800
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        push   0x15
        call   sub_1_05E4
loc_2_3B08:
        add    esp, 0xc
        test   al, 1
        jne    loc_2_3B3A
        cmp    word ptr [0], si              ; -> obj3:0x01B0
        je     loc_2_3B30
        mov    eax, 0                        ; -> obj3:0x01B0
loc_2_3B1D:
        mov    cx, word ptr [ebp - 0x10]
        cmp    word ptr [eax], cx
        je     loc_2_3B30
        add    eax, 4
        inc    esi
        cmp    word ptr [eax], 0
        jne    loc_2_3B1D
loc_2_3B30:
        mov    ax, word ptr [esi*4 + 2]      ; -> obj3:0x01B2
        jmp    loc_2_3B3E
loc_2_3B3A:
        mov    ax, 0x101
loc_2_3B3E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_3B43:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, 0x44                     ; -> obj3:0x01F4
        lea    edi, [ebp - 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        call   sub_1_1304
loc_2_3B5C:
        mov    edx, eax
        lea    edi, [ebp - 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 0xfffea
        push   eax
        call   sub_2_6FA8
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_3B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        push   edi
        call   sub_2_3B43
        or     eax, eax
        je     loc_2_3BAC
        cmp    dword ptr [ebp + 8], 1
        mov    edi, 1
        je     loc_2_3BAE
loc_2_3BAC:
        sub    edi, edi
loc_2_3BAE:
        or     edi, edi
        je     loc_2_3C60
        mov    word ptr [ebp - 4], 0xe802
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        push   0x15
        call   sub_1_05E4
loc_2_3BC8:
        add    esp, 0xc
        and    eax, 1
        cmp    eax, 1
        sbb    edi, edi
        neg    edi
        or     edi, edi
        je     loc_2_3C60
        mov    di, word ptr [ebp - 0x10]
        and    edi, 1
        je     loc_2_3C60
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 4], 1
        mov    dword ptr [esi + 8], 1
        call   sub_2_3AE4
        movzx  eax, ax
        mov    dword ptr [esi + 0x10], eax
        mov    ax, word ptr [ebp - 0x10]
        and    ax, 4
        shr    ax, 2
        movzx  eax, ax
        mov    dword ptr [esi + 0xc], eax
        test   byte ptr [ebp - 0x10], 2
        je     loc_2_3C22
        mov    dword ptr [esi], 4
        jmp    loc_2_3C60
loc_2_3C22:
        mov    dword ptr [esi], 2
        mov    ax, word ptr [ebp - 0x10]
        and    ax, 0xf0
        shr    ax, 4
        movzx  eax, ax
        mov    dword ptr [esi + 0x14], eax
        mov    ax, word ptr [ebp - 0x10]
        and    ax, 0x300
        shr    ax, 8
        movzx  eax, ax
        inc    eax
        mov    dword ptr [esi + 0x18], eax
        cmp    eax, 2
        jbe    loc_2_3C60
        cmp    word ptr [ebp - 0xc], 0
        je     loc_2_3C60
        movzx  eax, word ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x1c], eax
loc_2_3C60:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_3C68:
        call   sub_2_3B43
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_2_3C74:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, 0                        ; -> obj3:0x01FC
        lea    edi, [ebp - 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        call   sub_1_1304
loc_2_3C8B:
        mov    edx, eax
        lea    edi, [ebp - 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 0xfe010
        push   eax
        call   sub_2_6FA8
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, 8                        ; -> obj3:0x0204
        lea    edi, [ebp - 4]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        call   sub_2_3C74
        or     eax, eax
        je     loc_2_3D0E
        call   sub_1_1304
loc_2_3CE0:
        mov    edx, eax
        lea    edi, [ebp - 4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 0xfe000
        push   eax
        call   sub_2_6FA8
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_3D0E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_3D14:
        push   ebp
        mov    ebp, esp
        call   sub_2_3C74
        or     eax, eax
        je     loc_2_3D3A
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   0x15
        call   sub_1_05E4
loc_2_3D2D:
        add    esp, 0xc
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_3D3A:
        pop    ebp
        ret    
loc_2_3D3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        sub    esi, esi
        mov    word ptr [ebp - 4], 0xc280
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        call   sub_2_3D14
        add    esp, 8
        cmp    eax, esi
        je     loc_2_3D60
        movzx  esi, word ptr [ebp - 0x10]
loc_2_3D60:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_3D67:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        mov    word ptr [ebp - 4], 0xc281
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x10], ax
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        call   sub_2_3D14
        add    esp, 8
        or     eax, eax
        je     loc_2_3DE5
        mov    cl, byte ptr [ebp - 4]
        and    cl, 0x38
        shr    cl, 3
        mov    edx, dword ptr [ebp + 0xc]
        movzx  ecx, cl
        mov    ecx, dword ptr [ecx*4]        ; -> obj3:0x0208
        mov    dword ptr [edx], ecx
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 1
        mov    dword ptr [edx + 4], ecx
        mov    cl, byte ptr [ebp - 4]
        and    cl, 4
        shr    cl, 2
        movzx  ecx, cl
        mov    dword ptr [edx + 0xc], ecx
        mov    cl, byte ptr [ebp - 4]
        and    cl, 2
        shr    cl, 1
        movzx  ecx, cl
        mov    dword ptr [edx + 8], ecx
        movzx  ecx, word ptr [ebp - 0x10]
        mov    dword ptr [edx + 0x10], ecx
        movzx  ecx, byte ptr [ebp - 8]
        mov    dword ptr [edx + 0x14], ecx
        movzx  ecx, byte ptr [ebp - 7]
        mov    dword ptr [edx + 0x18], ecx
        movzx  ecx, word ptr [ebp - 0xc]
        mov    dword ptr [edx + 0x1c], ecx
loc_2_3DE5:
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_3DEC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x14]
        sub    ebx, ebx
        mov    word ptr [eax], bx
        cmp    dword ptr [0], ebx            ; -> obj3:0x0210
        je     loc_2_3E92
        mov    dword ptr [ebp - 4], 0        ; -> obj3:0x0210
        mov    esi, dword ptr [ebp + 8]
loc_2_3E13:
        or     ebx, ebx
        jne    loc_2_3E92
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax]
        mov    edi, eax
        or     edi, edi
        je     loc_2_3E86
loc_2_3E22:
        or     ebx, ebx
        jne    loc_2_3E86
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   edi
        call   dword ptr [eax + 4]
        add    esp, 8
        or     eax, eax
        je     loc_2_3E83
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_2_3E54
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_2_3E54
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_3E54
        cmp    dword ptr [ebp - 0x24], esi
        mov    ebx, 1
        je     loc_2_3E56
loc_2_3E54:
        sub    ebx, ebx
loc_2_3E56:
        cmp    esi, 2
        jne    loc_2_3E76
        or     ebx, ebx
        je     loc_2_3E74
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_2_3E74
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 0x10], eax
        mov    ebx, 1
        je     loc_2_3E76
loc_2_3E74:
        sub    ebx, ebx
loc_2_3E76:
        or     ebx, ebx
        je     loc_2_3E83
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [eax], cx
loc_2_3E83:
        dec    edi
        jne    loc_2_3E22
loc_2_3E86:
        add    dword ptr [ebp - 4], 8
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    loc_2_3E13
loc_2_3E92:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_3E9C:
        enter  0, 0
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    esi, dword ptr [esi + 8]
        mov    word ptr [esi + 0x1c], 0xf963
        mov    word ptr [esi + 0x10], 0x4954
        mov    word ptr [esi + 0x14], 0
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     dword ptr [esi + 0x2c], 0
        jb     loc_2_3F14
        mov    word ptr [esi + 0x1c], 0xf95f
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     dword ptr [esi + 0x2c], 0
        jb     loc_2_3F03
        xor    ebx, ebx
        xor    eax, eax
        mov    bx, word ptr [esi + 0x38]
        shl    ebx, 4
        mov    ax, word ptr [esi + 0x10]
        add    ebx, eax
        cmp    word ptr [ebx], 0xa
        jl     loc_2_3F14
        test   word ptr [ebx + 0x14], 0x40
        je     loc_2_3F14
loc_2_3F03:
        mov    dword ptr [0x13f4], 1         ; -> obj1:0x13F4
        mov    eax, 1
        jmp    loc_2_3F16
loc_2_3F14:
        xor    eax, eax
loc_2_3F16:
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h
sub_2_3F1C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 0x22
        je     loc_2_3F44
        cmp    eax, 0x23
        je     loc_2_3F44
        cmp    eax, 0x18
        je     loc_2_3F3D
        cmp    eax, 0x19
        je     loc_2_3F3D
        sub    eax, eax
        jmp    loc_2_3F49
loc_2_3F3D:
        mov    eax, 1
        jmp    loc_2_3F49
loc_2_3F44:
        mov    eax, 2
loc_2_3F49:
        pop    ebp
        ret    
sub_2_3F4B:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x7f]
        or     eax, eax
        je     loc_2_3F65
        cmp    dword ptr [eax], 0xffffff
        mov    eax, 1
        jne    loc_2_3F67
loc_2_3F65:
        sub    eax, eax
loc_2_3F67:
        pop    ebp
        ret    
sub_2_3F69:
        push   ebp
        mov    ebp, esp
        call   sub_2_2591
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        sub    edx, dword ptr [ecx + 0x48]
        cmp    edx, 0x2ee
        ja     loc_2_3F85
        sub    eax, eax
        jmp    loc_2_3F8D
loc_2_3F85:
        mov    dword ptr [ecx + 0x48], eax
        mov    eax, 1
loc_2_3F8D:
        pop    ebp
        ret    
sub_2_3F8F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 0x63], 0x18
        mov    dword ptr [eax + 8], 0x18
        mov    dword ptr [eax + 4], 0x18
        mov    eax, 1
        ret    
sub_2_3FB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        sub    word ptr [eax + 0x67], 2
        mov    word ptr [eax + 0x63], 0x19
        mov    dword ptr [eax + 8], 0x19
        mov    dword ptr [eax + 4], 0x19
        mov    eax, 1
        ret    
sub_2_3FD6:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    al, byte ptr [ebp + 8]
        cmp    al, 1
        jne    loc_2_3FF6
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_3F8F
        add    esp, 4
        mov    dword ptr [esi + 0x77], 1
        jmp    loc_2_401E
loc_2_3FF6:
        cmp    al, 3
        mov    esi, dword ptr [ebp + 0xc]
        jne    loc_2_400F
        push   esi
        call   sub_2_3FB0
        add    esp, 4
        mov    dword ptr [esi + 0x77], 1
        jmp    loc_2_401E
loc_2_400F:
        cmp    dword ptr [esi + 0x77], 0
        jne    loc_2_401E
        push   esi
        call   sub_2_3FB0
        add    esp, 4
loc_2_401E:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_4026:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, 0x10
        je     loc_2_4050
        cmp    ecx, 0x11
        je     loc_2_4050
        cmp    ecx, 0x12
        jne    loc_2_4066
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0x23
        mov    dword ptr [eax + 4], 0x23
        jmp    loc_2_4061
loc_2_4050:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0x22
        mov    dword ptr [eax + 4], 0x22
loc_2_4061:
        mov    eax, 1
loc_2_4066:
        pop    ebp
        ret    
sub_2_4068:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        sub    esi, esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0xc
        cmp    eax, esi
        je     loc_2_4098
        mov    eax, dword ptr [ebp - 4]
        mov    esi, 1
        mov    ecx, dword ptr [eax + 0x30]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
loc_2_4098:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_409F:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    dword ptr [ecx + 0x10], 0
        mov    dword ptr [ecx + 0x14], 0
        mov    dword ptr [ecx + 0x1c], 0
        mov    dword ptr [ecx + 0x24], 2
        mov    dword ptr [ecx + 0x28], 0
        mov    dword ptr [ecx + 0x2c], 0
        mov    word ptr [ecx + 0x50], 0
        mov    word ptr [ecx + 0x52], 0
        mov    dword ptr [ecx + 0x4c], 0
        mov    dword ptr [ecx + 0x56], 0
        mov    byte ptr [ecx + 0x5a], 0
        mov    dword ptr [ecx + 0x30], 0
        mov    dword ptr [ecx + 0x34], 0
        mov    dword ptr [ecx + 0x3c], 0
        mov    dword ptr [ecx + 0x40], 0
        mov    dword ptr [ecx + 0x44], 0
        mov    eax, dword ptr [ecx + 0x7b]
        and    eax, 0xfffffffe
        mov    dword ptr [ecx + 0x7b], eax
        and    eax, 0xfffffffd
        mov    dword ptr [ecx + 0x7b], eax
        mov    word ptr [ecx + 0x69], 0
        mov    word ptr [ecx + 0x65], 0
        mov    word ptr [ecx + 0x67], 0
        mov    dword ptr [ecx + 0x73], 0
        mov    eax, 1
        mov    dword ptr [ecx + 0x6b], 0
        mov    dword ptr [ecx + 0x6f], 0
        mov    dword ptr [ecx + 0x77], 0
        mov    dword ptr [ecx + 0x48], 0
        mov    dword ptr [ecx + 0x83], 0
        mov    dword ptr [ecx + 0x7f], 0
        pop    ebp
        ret    
sub_2_4180:
        enter  0, 0
        push   esi
        mov    eax, 0xc0009
        mov    esi, 0xa0                     ; -> loc_1_00A0
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    eax, 0
        jb     loc_2_41A7
        mov    dword ptr [0x1394], esi       ; -> loc_1_1394
        mov    eax, 1
loc_2_41A7:
        pop    esi
        leave  
        ret    
        db  0C8h, 000h, 000h, 000h, 056h, 0B8h, 009h, 000h, 00Ch, 000h, 0BEh
        dd  loc_1_00A0               ; fixup
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h, 0B8h, 001h, 000h, 000h, 000h, 073h, 005h, 0B8h, 000h, 000h
        db  000h, 000h, 05Eh, 0C9h, 0C3h, 000h, 000h
sub_2_41D0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    esi, esi
        call   sub_1_132C
loc_2_41DB:
        mov    ecx, eax
        sub    ecx, dword ptr [0]            ; -> obj3:0x022C
        cmp    ecx, 0x3e8
        jbe    loc_2_41FC
        mov    dword ptr [0], eax            ; -> obj3:0x022C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 0x20]
        add    esp, 4
        mov    esi, eax
loc_2_41FC:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
sub_2_4201:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        push   dword ptr [esi + 0x30]
        call   sub_2_965A
        add    esp, 4
        cmp    eax, edi
        je     loc_2_4228
        mov    edi, 1
        push   esi
        call   sub_2_41D0
        add    esp, 4
loc_2_4228:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_422E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    esi, esi
        push   dword ptr [edi + 0x30]
        call   sub_2_966F
        add    esp, 4
        cmp    eax, esi
        je     loc_2_4255
        mov    esi, 1
        push   edi
        call   sub_2_41D0
        add    esp, 4
loc_2_4255:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_425B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        push   dword ptr [esi + 0x30]
        call   sub_2_9699
        add    esp, 4
        cmp    eax, edi
        je     loc_2_4282
        mov    edi, 1
        push   esi
        call   sub_2_41D0
        add    esp, 4
loc_2_4282:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_4288:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_425B
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_4297:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_425B
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h
sub_2_42A8:
        mov    eax, 0                        ; -> obj3:0x0230
        ret    
sub_2_42AE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        cmp    eax, 0x18                     ; -> obj3:0x0248
        jb     loc_2_42C0
        sub    eax, eax
loc_2_42C0:
        pop    ebp
        ret    
sub_2_42C2:
        call   sub_2_42A8
        or     eax, eax
        je     loc_2_42E1
loc_2_42CB:
        cmp    dword ptr [eax], 0
        jne    loc_2_42DD
        push   eax
        call   sub_2_42AE
        add    esp, 4
        or     eax, eax
        jne    loc_2_42CB
loc_2_42DD:
        or     eax, eax
        jne    loc_2_42E5
loc_2_42E1:
        sub    eax, eax
        jmp    loc_2_42E7
loc_2_42E5:
        mov    eax, dword ptr [eax]
loc_2_42E7:
        ret    
sub_2_42E8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x18], eax         ; -> obj3:0x0248
        pop    ebp
        ret    
sub_2_42F5:
        mov    eax, dword ptr [0x18]         ; -> obj3:0x0248
        or     eax, eax
        je     loc_2_4312
        push   eax
        call   sub_2_4619
        add    esp, 4
        or     eax, eax
        je     loc_2_4312
        mov    eax, dword ptr [0x18]         ; -> obj3:0x0248
        jmp    loc_2_4317
loc_2_4312:
        call   sub_2_42C2
loc_2_4317:
        ret    
sub_2_4318:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 6
        jb     loc_2_432A
        call   sub_2_42F5
        jmp    loc_2_4331
loc_2_432A:
        mov    eax, dword ptr [eax*4]        ; -> obj3:0x0230
loc_2_4331:
        pop    ebp
        ret    
sub_2_4333:
        push   ebp
        mov    ebp, esp
        call   sub_2_42A8
        or     eax, eax
        je     loc_2_435A
loc_2_433F:
        cmp    dword ptr [eax], 0
        je     loc_2_4351
        push   eax
        call   sub_2_42AE
        add    esp, 4
        or     eax, eax
        jne    loc_2_433F
loc_2_4351:
        or     eax, eax
        je     loc_2_435A
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
loc_2_435A:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        ret    
sub_2_4362:
        push   ebp
        mov    ebp, esp
        push   esi
        call   sub_2_42A8
        or     eax, eax
        je     loc_2_4394
        mov    esi, dword ptr [ebp + 8]
loc_2_4372:
        cmp    dword ptr [eax], esi
        je     loc_2_4383
        push   eax
        call   sub_2_42AE
        add    esp, 4
        or     eax, eax
        jne    loc_2_4372
loc_2_4383:
        or     eax, eax
        je     loc_2_4394
        mov    dword ptr [eax], 0
        mov    eax, 1
        jmp    loc_2_4396
loc_2_4394:
        sub    eax, eax
loc_2_4396:
        pop    esi
        pop    ebp
        ret    
sub_2_4399:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 0x18]
        add    esp, 4
        or     eax, eax
        jne    loc_2_43B3
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 0
loc_2_43B3:
        sub    eax, eax
        pop    ebp
        ret    
sub_2_43B7:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0                             ; -> sub_2_4399
        call   sub_2_4594
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        ret    
sub_2_43D3:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 0x1c]
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_43E7:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   1
        push   0xdb
        call   sub_1_12F4
loc_2_43F8:
        add    esp, 8
        mov    edi, eax
        cmp    edi, esi
        je     loc_2_4497
        push   1
        push   0x10
        call   sub_1_12F4
loc_2_440E:
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        push   1
        push   0x10
        call   sub_1_12F4
loc_2_441D:
        add    esp, 8
        mov    dword ptr [edi + 8], eax
        push   0xa
        lea    eax, [edi + 0x10]
        push   eax
        call   sub_1_07A0
loc_2_442E:
        add    esp, 8
        push   edi
        call   sub_2_9725
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, esi
        je     loc_2_4458
        cmp    dword ptr [edi + 4], esi
        je     loc_2_4458
        cmp    dword ptr [edi + 8], esi
        je     loc_2_4458
        cmp    ebx, esi
        je     loc_2_4458
        mov    esi, 1
        jmp    loc_2_4488
loc_2_4458:
        push   eax
        call   sub_1_07D2
loc_2_445E:
        add    esp, 4
        push   0
        push   dword ptr [edi + 8]
        call   sub_1_12FC
loc_2_446B:
        add    esp, 8
        push   0
        push   dword ptr [edi + 4]
        call   sub_1_12FC
loc_2_4478:
        add    esp, 8
        or     ebx, ebx
        je     loc_2_4488
        push   edi
        call   sub_2_97BB
        add    esp, 4
loc_2_4488:
        or     esi, esi
        jne    loc_2_449D
        push   0
        push   edi
        call   sub_1_12FC
loc_2_4494:
        add    esp, 8
loc_2_4497:
        or     esi, esi
        jne    loc_2_449D
        sub    edi, edi
loc_2_449D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_44A3:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_97BB
        add    esp, 4
        mov    eax, dword ptr [esi + 0xab]
        or     eax, eax
        je     loc_2_44C8
        push   0
        push   eax
        call   sub_1_12FC
loc_2_44C5:
        add    esp, 8
loc_2_44C8:
        mov    eax, dword ptr [esi + 0xa7]
        or     eax, eax
        je     loc_2_44DD
        push   0
        push   eax
        call   sub_1_12FC
loc_2_44DA:
        add    esp, 8
loc_2_44DD:
        push   dword ptr [esi + 0x10]
        call   sub_1_07D2
loc_2_44E5:
        add    esp, 4
        push   0
        push   dword ptr [esi + 8]
        call   sub_1_12FC
loc_2_44F2:
        add    esp, 8
        push   0
        push   dword ptr [esi + 4]
        call   sub_1_12FC
loc_2_44FF:
        add    esp, 8
        mov    eax, dword ptr [esi + 0xd7]
        or     eax, eax
        je     loc_2_4517
        push   0
        push   eax
        call   sub_1_12FC
loc_2_4514:
        add    esp, 8
loc_2_4517:
        push   0
        push   esi
        call   sub_1_12FC
loc_2_451F:
        add    esp, 8
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_452A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_1_12F4
loc_2_453B:
        add    esp, 8
        mov    dword ptr [esi + 0xd7], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_4550:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_4318
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_455F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, 1
        push   dword ptr [ebp + 8]
        call   sub_2_4550
        mov    ecx, dword ptr [ebp + 0xc]
        add    esp, 4
        mov    dword ptr [ecx], eax
        or     eax, eax
        mov    eax, dword ptr [ebp + 0x10]
        jne    loc_2_4589
        mov    dword ptr [eax], 0xffffffff
        sub    esi, esi
        jmp    loc_2_458F
loc_2_4589:
        mov    dword ptr [eax], 0
loc_2_458F:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
sub_2_4594:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_2_42A8
        mov    esi, eax
        or     esi, esi
        je     loc_2_45CB
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
loc_2_45AB:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_2_45BC
        push   ebx
        push   eax
        call   edi
        add    esp, 8
        or     eax, eax
        jne    loc_2_45CB
loc_2_45BC:
        push   esi
        call   sub_2_42AE
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_2_45AB
loc_2_45CB:
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_45D6:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        inc    dword ptr [eax]
        sub    eax, eax
        ret    
sub_2_45E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> loc_2_45D6
        call   sub_2_4594
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_4607:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        sub    ecx, dword ptr [ebp + 8]
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        ret    
sub_2_4619:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0                             ; -> loc_2_4607
        call   sub_2_4594
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_4630:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_43D3
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_4644:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x2c]
        push   edi
        call   sub_2_966F
        add    esp, 4
        or     eax, eax
        je     loc_2_4676
        push   esi
        call   sub_2_41D0
        add    esp, 4
        cmp    dword ptr [esi + 0x2c], edi
        je     loc_2_4676
        push   dword ptr [esi + 0x2c]
        push   esi
        call   sub_2_9371
        add    esp, 8
loc_2_4676:
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h
loc_2_467C:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_4644
        add    esp, 4
        sub    eax, eax
        pop    ebp
        ret    
sub_2_468E:
        push   0
        push   0                             ; -> loc_2_467C
        call   sub_2_4594
        add    esp, 8
        ret    
        db  000h, 000h
loc_2_46A0:
        push   ebp
        mov    esi, ebp
        cmp    dword ptr [0x274], 0          ; -> obj3:0x0274
        je     loc_2_4721
        movzx  ecx, word ptr [esi + 0x1c]
        cmp    ecx, 0x34
        ja     loc_2_46CA
        mov    cx, word ptr [ecx*2 + 0x5218] ; -> obj2:0x5218
        movsx  ecx, cx
        add    ecx, 0x5282                   ; -> loc_2_5282
        call   ecx
        jmp    loc_2_4725
loc_2_46CA:
        cmp    cl, 0x6c
        jne    loc_2_46F1
        shr    ecx, 8
        sub    ecx, 0x10
        js     loc_2_4721
        cmp    ecx, 0x1d
        ja     loc_2_4721
        mov    cx, word ptr [ecx*2 + 0x388]  ; -> obj3:0x0388
        movsx  ecx, cx
        add    ecx, 0x48b0                   ; -> loc_2_48B0
        call   ecx
        jmp    loc_2_4725
loc_2_46F1:
        cmp    ecx, 0x4d
        jne    loc_2_470A
        mov    ecx, 0x64
        mov    word ptr [esi], cx
        mov    ecx, dword ptr [0x260]        ; -> obj3:0x0260
        mov    word ptr [esi + 0x38], cx
        jmp    loc_2_4724
loc_2_470A:
        cmp    ecx, 0x6d
        jne    loc_2_4721
        mov    ecx, dword ptr [0x258]        ; -> obj3:0x0258
        mov    word ptr [esi], cx
        shr    ecx, 0x10
        mov    word ptr [esi + 0x38], cx
        jmp    loc_2_4724
loc_2_4721:
        stc    
        jmp    loc_2_4725
loc_2_4724:
        clc    
loc_2_4725:
        pop    ebp
        ret    
        db  0C8h, 000h, 000h, 000h, 056h, 083h, 03Dh
        dd  obj3:0x0274              ; fixup
        db  001h, 075h, 017h, 0B8h, 033h, 000h, 000h, 000h, 0BEh
        dd  loc_2_46A0               ; fixup
        db  0CDh, 020h, 018h, 001h, 001h, 000h, 0B8h, 001h, 000h, 000h, 000h, 073h, 005h, 0B8h, 000h, 000h
        db  000h, 000h, 05Eh, 0C9h, 0C3h
sub_2_4754:
        enter  0, 0
        push   esi
        mov    eax, 0x33
        mov    esi, 0x46a0                   ; -> loc_2_46A0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0
        jb     loc_2_477F
        call   sub_2_51E9
        call   sub_2_55A4
        mov    eax, 1
loc_2_477F:
        mov    dword ptr [0x274], eax        ; -> obj3:0x0274
        pop    esi
        leave  
        ret    
sub_2_4787:
        push   ebp
        mov    ebp, esp
        add    esp, -8
        mov    dword ptr [0x24c], 0          ; -> obj3:0x024C
        push   0x204
        push   0x179
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_48AB
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0x33
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        mov    word ptr [0x278], dx          ; -> obj3:0x0278
        mov    word ptr [0x27a], cx          ; -> obj3:0x027A
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        jb     loc_2_48AB
        and    ecx, 0xffff
        mov    dword ptr [0x260], ecx        ; -> obj3:0x0260
        mov    dword ptr [ebp - 8], edx
        mov    edi, dword ptr [ebp - 4]
        mov    esi, 0x278                    ; -> obj3:0x0278
        mov    ecx, 0xf9
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xf9
        push   eax
        push   0x73
        push   eax
        call   sub_2_8933
        add    esp, 8
        or     eax, eax
        pop    eax
        jne    loc_2_4834
        mov    edi, eax
        xor    eax, eax
        cld    
        mov    ecx, 0x73
        repne scasb al, byte ptr es:[edi]
        jne    loc_2_4834
        dec    edi
        mov    esi, 0x267                    ; -> obj3:0x0267
        mov    ecx, 0xd
        jmp    loc_2_4847
loc_2_4834:
        mov    ecx, 0x10
        mov    esi, 0x264                    ; -> obj3:0x0264
        mov    edi, dword ptr [ebp - 4]
        add    edi, 0xf9
loc_2_4847:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [0x260]        ; -> obj3:0x0260
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        add    edx, 0x80
        mov    eax, 0x33
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
        pop    eax
        mov    word ptr [0x250], 0xf9        ; -> obj3:0x0250
        mov    word ptr [0x252], ax          ; -> obj3:0x0252
        mov    word ptr [0x254], 0xb9        ; -> obj3:0x0254
        mov    word ptr [0x256], ax          ; -> obj3:0x0256
        mov    word ptr [0x258], 0x90        ; -> obj3:0x0258
        mov    word ptr [0x25c], 0xa5        ; -> obj3:0x025C
        mov    word ptr [0x25a], ax          ; -> obj3:0x025A
        mov    word ptr [0x25e], ax          ; -> obj3:0x025E
        mov    dword ptr [0x24c], 1          ; -> obj3:0x024C
loc_2_48AB:
        leave  
        ret    
        db  000h, 000h, 000h
loc_2_48B0:
        xor    eax, eax
        mov    word ptr [esi + 0x1c], 0xffff
        mov    word ptr [esi + 0x10], 0x10
        mov    word ptr [esi + 0x18], ax
        mov    word ptr [esi], ax
        mov    word ptr [esi + 0x14], cs
        mov    word ptr [esi + 4], ds
        clc    
        ret    
        db  066h, 0C7h, 046h, 01Ch, 0FFh, 0FFh, 066h, 0C7h, 046h, 010h, 02Ah, 006h, 066h, 0C7h, 046h, 018h
        db  000h, 000h, 066h, 0C7h, 046h, 014h, 000h, 000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_676C
        add    esp, 8
        or     eax, eax
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 0xffffffff
        jge    loc_2_490C
        xor    eax, eax
        xor    edx, edx
loc_2_490C:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        db  083h, 0F9h, 064h, 077h, 013h, 051h, 053h, 0E8h, 0BBh, 023h, 000h, 000h, 083h, 0C4h, 008h, 00Bh
        db  0C0h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 07Dh, 002h, 033h, 0C0h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6CBE
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_4950
        xor    eax, eax
        xor    edx, edx
loc_2_4950:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6C89
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_4979
        xor    eax, eax
        xor    edx, edx
loc_2_4979:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        movzx  eax, word ptr [esi + 0x18]
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        push   ebx
        call   sub_2_6C44
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_49A7
        xor    eax, eax
        xor    edx, edx
loc_2_49A7:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6C14
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_49D0
        xor    eax, eax
        xor    edx, edx
loc_2_49D0:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6BE4
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_49F9
        xor    eax, eax
        xor    edx, edx
loc_2_49F9:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6BB3
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_4A22
        xor    eax, eax
        xor    edx, edx
loc_2_4A22:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        call   sub_2_691D
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0xffffffff
        mov    ecx, dword ptr [ebp - 4]
        dec    ecx
        mov    edx, dword ptr [ebp - 8]
        dec    edx
        jge    loc_2_4A56
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
loc_2_4A56:
        mov    word ptr [esi + 0x18], cx
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h
sub_2_4A74:
        cmp    edx, 4
        jbe    loc_2_4A7B
        xor    edx, edx
loc_2_4A7B:
        mov    edx, dword ptr [edx*4 + 0x4a60] ; -> obj2:0x4A60
        ret    
sub_2_4A83:
        push   ebx
        xor    ebx, ebx
loc_2_4A86:
        inc    ebx
        shr    eax, 1
        or     eax, eax
        jne    loc_2_4A86
        mov    eax, ebx
        pop    ebx
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        mov    eax, ecx
        call   sub_2_4A83
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        push   ebx
        call   sub_2_69ED
        add    esp, 0xc
        or     eax, eax
        jl     loc_2_4ABF
        mov    edx, dword ptr [ebp - 4]
        call   sub_2_4A74
        mov    eax, 0xffffffff
        jmp    loc_2_4AC3
loc_2_4ABF:
        xor    eax, eax
        xor    edx, edx
loc_2_4AC3:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        inc    ecx
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   ebx
        call   sub_2_69AE
        add    esp, 0xc
        or     eax, eax
        jl     loc_2_4AF1
        mov    edx, dword ptr [ebp - 4]
        call   sub_2_4A74
        mov    eax, 0xffffffff
        jmp    loc_2_4AF5
loc_2_4AF1:
        xor    eax, eax
        xor    edx, edx
loc_2_4AF5:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6DDA
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        jge    loc_2_4B1E
        xor    eax, eax
        xor    edx, edx
loc_2_4B1E:
        mov    word ptr [esi + 0x14], dx
        leave  
        ret    
        db  083h, 0F9h, 020h, 077h, 013h, 051h, 053h, 0E8h, 0C5h, 022h, 000h, 000h, 083h, 0C4h, 008h, 00Bh
        db  0C0h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 07Dh, 002h, 033h, 0C0h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -8
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   ebx
        call   sub_2_6D10
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4B65
        or     dword ptr [ebp - 8], 1
loc_2_4B65:
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   ebx
        call   sub_2_6D10
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4B7E
        or     dword ptr [ebp - 8], 2
loc_2_4B7E:
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   ebx
        call   sub_2_6D10
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4B97
        or     dword ptr [ebp - 8], 4
loc_2_4B97:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [esi + 0x18], cx
        mov    word ptr [esi + 0x14], 1
        mov    eax, 0xffffffff
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        xor    eax, eax
        or     edx, edx
        jne    loc_2_4BB9
        xor    ecx, ecx
loc_2_4BB9:
        push   edx
        and    edx, 0xfffffffe
        pop    edx
        jne    loc_2_4C20
        cmp    ecx, 7
        ja     loc_2_4C20
        mov    dword ptr [ebp - 4], ecx
        mov    eax, 0
        test   ecx, 1
        je     loc_2_4BD7
        not    eax
loc_2_4BD7:
        push   eax
        push   1
        push   ebx
        call   sub_2_6D62
        add    esp, 0xc
        mov    eax, 0
        test   dword ptr [ebp - 4], 2
        je     loc_2_4BF3
        not    eax
loc_2_4BF3:
        push   eax
        push   2
        push   ebx
        call   sub_2_6D62
        add    esp, 0xc
        mov    eax, 0
        test   dword ptr [ebp - 4], 4
        je     loc_2_4C0F
        not    eax
loc_2_4C0F:
        push   eax
        push   3
        push   ebx
        call   sub_2_6D62
        add    esp, 0xc
        mov    eax, 0xffffffff
loc_2_4C20:
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -8
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   ebx
        call   sub_2_67A2
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4C48
        or     dword ptr [ebp - 8], 1
loc_2_4C48:
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   ebx
        call   sub_2_67A2
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4C61
        or     dword ptr [ebp - 8], 2
loc_2_4C61:
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   ebx
        call   sub_2_67A2
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4C7A
        or     dword ptr [ebp - 8], 4
loc_2_4C7A:
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   ebx
        call   sub_2_67A2
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4C93
        or     dword ptr [ebp - 8], 8
loc_2_4C93:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [esi + 0x18], cx
        mov    edx, 1
        mov    word ptr [esi + 0x14], dx
        mov    eax, 0xffffffff
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        xor    eax, eax
        or     edx, edx
        jne    loc_2_4CB8
        xor    ecx, ecx
loc_2_4CB8:
        push   edx
        and    edx, 0xfffffffe
        pop    edx
        jne    loc_2_4D3B
        cmp    ecx, 0xf
        ja     loc_2_4D3B
        mov    dword ptr [ebp - 4], ecx
        mov    eax, 0
        test   ecx, 1
        je     loc_2_4CD6
        not    eax
loc_2_4CD6:
        push   eax
        push   1
        push   ebx
        call   sub_2_67F4
        add    esp, 0xc
        mov    eax, 0
        test   dword ptr [ebp - 4], 2
        je     loc_2_4CF2
        not    eax
loc_2_4CF2:
        push   eax
        push   2
        push   ebx
        call   sub_2_67F4
        add    esp, 0xc
        mov    eax, 0
        test   dword ptr [ebp - 4], 4
        je     loc_2_4D0E
        not    eax
loc_2_4D0E:
        push   eax
        push   3
        push   ebx
        call   sub_2_67F4
        add    esp, 0xc
        mov    eax, 0
        test   dword ptr [ebp - 4], 8
        je     loc_2_4D2A
        not    eax
loc_2_4D2A:
        push   eax
        push   4
        push   ebx
        call   sub_2_67F4
        add    esp, 0xc
        mov    eax, 0xffffffff
loc_2_4D3B:
        leave  
        ret    
loc_2_4D3D:
        push   ebp
        mov    ebp, esp
        add    esp, -4
        cmp    ecx, 4
        mov    eax, 0
        jae    loc_2_4D77
        inc    ecx
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   ebx
        call   sub_2_694F
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0
        mov    edx, eax
        jl     loc_2_4D73
        mov    eax, 0xffffffff
        mov    ecx, dword ptr [ebp - 4]
        dec    ecx
        inc    edx
        shl    edx, cl
loc_2_4D73:
        mov    word ptr [esi + 0x14], dx
loc_2_4D77:
        leave  
        ret    
        db  083h, 0F9h, 004h, 073h, 021h, 041h, 083h, 0FAh, 008h, 077h, 01Bh, 08Bh, 0C2h, 0E8h, 0F8h, 0FCh
        db  0FFh, 0FFh, 050h, 051h, 053h, 0E8h, 0EEh, 01Bh, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 0B8h
        db  0FFh, 0FFh, 0FFh, 0FFh, 07Dh, 002h, 033h, 0C0h, 0C3h, 00Bh, 0DBh, 074h, 001h, 04Bh, 053h, 0E8h
        db  0A3h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h
sub_2_4DB1:
        push   eax
        push   ecx
        push   6
        call   sub_2_4550
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        pop    ecx
        pop    eax
        ret    
        db  033h, 0DBh, 08Ah, 05Eh, 011h, 0E8h, 0D4h, 0FFh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 02Dh, 08Bh, 0D8h
        db  00Fh, 0B6h, 046h, 010h, 03Dh, 013h, 000h, 000h, 000h, 077h, 020h, 00Fh, 0B7h, 04Eh, 018h, 00Fh
        db  0B7h, 056h, 014h, 066h, 08Bh, 004h, 045h
        dd  obj3:0x03C4              ; fixup
        db  00Fh, 0BFh, 0C0h, 005h
        dd  loc_2_4D3D               ; fixup
        db  0FFh, 0D0h, 066h, 089h, 046h, 01Ch, 0EBh, 006h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h
        db  066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        mov    eax, 0
        movzx  ebx, byte ptr [esi + 0x10]
        or     ebx, ebx
        je     loc_2_4E51
        lea    eax, [ebp - 4]
        push   eax
        push   6
        call   sub_2_4068
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     loc_2_4E51
        xor    ebx, ebx
        cmp    dword ptr [ebp - 4], 0x14
        je     loc_2_4E47
        cmp    dword ptr [ebp - 4], 0x15
        jne    loc_2_4E49
loc_2_4E47:
        not    bl
loc_2_4E49:
        mov    byte ptr [esi + 0x11], bl
        mov    eax, 0xffffffff
loc_2_4E51:
        mov    word ptr [esi + 0x1c], ax
        clc    
        leave  
        ret    
        db  083h, 03Dh
        dd  obj3:0x024C              ; fixup
        db  001h, 075h, 018h, 0A1h
        dd  obj3:0x0258              ; fixup
        db  066h, 089h, 006h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 0A1h
        dd  obj3:0x025C              ; fixup
        db  066h, 089h, 046h, 004h, 0F8h, 0C3h, 080h, 07Eh, 010h, 003h, 075h, 013h, 056h, 06Ah, 006h, 0E8h
        db  089h, 008h, 000h, 000h, 083h, 0C4h, 008h, 066h, 0C7h, 046h, 01Ch, 0FFh, 0FFh, 0EBh, 006h, 066h
        db  0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h, 00Fh, 0B7h, 05Eh, 010h, 00Fh, 0B7h, 04Eh, 018h, 00Ah
        db  0DBh, 075h, 050h, 066h, 08Bh, 046h, 038h, 0C1h, 0E0h, 010h, 066h, 08Bh, 006h, 00Ah, 0C9h, 075h
        db  021h, 00Ah, 0FFh, 074h, 00Ch, 0C7h, 005h
        dd  obj3:0x0380              ; fixup
        db  000h, 000h, 000h, 000h, 0EBh, 06Eh, 0C7h, 005h
        dd  obj3:0x0380              ; fixup
        db  001h, 000h, 000h, 000h, 0A3h
        dd  obj3:0x0384              ; fixup
        db  0EBh, 05Dh, 00Ah, 0FFh, 074h, 00Ch, 0C7h, 005h
        dd  obj3:0x0378              ; fixup
        db  000h, 000h, 000h, 000h, 0EBh, 04Dh, 0C7h, 005h
        dd  obj3:0x0378              ; fixup
        db  001h, 000h, 000h, 000h, 0A3h
        dd  obj3:0x037C              ; fixup
        db  0EBh, 03Ch, 00Ah, 0C9h, 075h, 01Ah, 083h, 03Dh
        dd  obj3:0x0380              ; fixup
        db  001h, 075h, 037h, 0A1h
        dd  obj3:0x0384              ; fixup
        db  066h, 089h, 006h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 0EBh, 01Eh, 083h, 03Dh
        dd  obj3:0x0378              ; fixup
        db  001h, 075h, 01Dh, 0A1h
        dd  obj3:0x037C              ; fixup
        db  066h, 089h, 006h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 066h, 0C7h, 046h, 004h, 000h, 000h
        db  066h, 0C7h, 046h, 01Ch, 0FFh, 0FFh, 0EBh, 006h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h
        db  066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        mov    word ptr [esi + 0x1c], 0x1a6c
        xor    ebx, ebx
        mov    word ptr [esi + 0x18], bx
        lea    eax, [ebp - 4]
        call   sub_2_4DB1
        je     loc_2_4FAE
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   edx
        call   sub_2_67A2
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4F87
        inc    ebx
        inc    ebx
loc_2_4F87:
        call   sub_2_4DB1
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   edx
        call   sub_2_67A2
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4FA4
        or     ebx, 4
loc_2_4FA4:
        mov    word ptr [esi + 0x1c], 0xffff
        mov    word ptr [esi + 0x10], bx
loc_2_4FAE:
        clc    
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  002h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 006h, 000h, 000h, 000h, 0B8h, 06Ch, 01Bh, 000h
        db  000h, 00Fh, 0B7h, 05Eh, 010h, 083h, 0FBh, 006h, 077h, 02Bh, 08Bh, 00Ch, 09Dh
        dd  obj2:0x4FB1              ; fixup
        db  083h, 0F9h, 0FFh, 074h, 01Fh, 0E8h, 0C5h, 0FDh, 0FFh, 0FFh, 075h, 007h, 0B8h, 06Ch, 01Bh, 000h
        db  000h, 0EBh, 011h, 08Bh, 0DAh, 0BAh, 001h, 000h, 000h, 000h, 0E8h, 0A9h, 0FCh, 0FFh, 0FFh, 0B8h
        db  0FFh, 0FFh, 0FFh, 0FFh, 066h, 089h, 046h, 01Ch, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch, 000h, 000h
        db  0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        call   sub_2_4DB1
        mov    eax, 0
        je     loc_2_5051
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6B61
        add    esp, 8
        xor    edx, edx
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, 0x5a
        div    bx
        mov    ebx, 3
        or     eax, eax
        je     loc_2_5051
        cmp    eax, ebx
        je     loc_2_5051
        and    eax, ebx
        xor    eax, ebx
loc_2_5051:
        mov    byte ptr [esi + 0x10], al
        clc    
        leave  
        ret    
        db  00Fh, 0BEh, 046h, 010h, 0BBh, 003h, 000h, 000h, 000h, 03Bh, 0C3h, 077h, 020h, 074h, 006h, 00Bh
        db  0C0h, 074h, 009h, 033h, 0C3h, 0BBh, 05Ah, 000h, 000h, 000h, 0F7h, 0E3h, 0E8h, 039h, 0FDh, 0FFh
        db  0FFh, 074h, 00Ah, 050h, 052h, 0E8h, 0FBh, 01Ah, 000h, 000h, 083h, 0C4h, 008h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        call   sub_2_4DB1
        mov    ebx, 0
        mov    ecx, ebx
        je     loc_2_50C3
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_66A4
        add    esp, 8
        xor    ebx, ebx
        mov    ecx, dword ptr [ebp - 4]
        or     ecx, ecx
        jne    loc_2_50B8
        mov    ecx, dword ptr [0x374]        ; -> obj3:0x0374
        jmp    loc_2_50C3
loc_2_50B8:
        inc    ebx
        cmp    ecx, 1
        je     loc_2_50C3
        mov    ecx, 2
loc_2_50C3:
        mov    word ptr [esi + 0x10], bx
        mov    word ptr [esi + 0x18], cx
        clc    
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        cmp    word ptr [esi + 0x10], 0x55
        je     loc_2_511C
        call   sub_2_4DB1
        je     loc_2_511C
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6860
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        jecxz  loc_2_5102
        cmp    word ptr [esi + 0x10], 0
        jne    loc_2_511C
        mov    ecx, 0
        jmp    loc_2_510D
loc_2_5102:
        cmp    word ptr [esi + 0x10], cx
        je     loc_2_511C
        mov    ecx, 1
loc_2_510D:
        call   sub_2_4DB1
        push   ecx
        push   edx
        call   sub_2_68C6
        add    esp, 8
loc_2_511C:
        clc    
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -4
        call   sub_2_4DB1
        mov    ecx, 0
        je     loc_2_5148
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6860
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        jecxz  loc_2_5148
        mov    ecx, 0xffffffff
loc_2_5148:
        mov    byte ptr [esi + 0x10], cl
        clc    
        leave  
        ret    
        db  066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h, 00Fh, 0B7h, 05Eh, 010h, 00Fh, 0B7h, 04Eh, 018h
        db  0E3h, 010h, 083h, 0F9h, 002h, 076h, 005h, 0B9h, 002h, 000h, 000h, 000h, 089h, 00Dh
        dd  obj3:0x0374              ; fixup
        db  08Bh, 00Dh
        dd  obj3:0x0374              ; fixup
        db  00Bh, 0DBh, 075h, 002h, 033h, 0C9h, 0E8h, 030h, 0FCh, 0FFh, 0FFh, 074h, 00Ah, 051h, 052h, 0E8h
        db  035h, 015h, 000h, 000h, 083h, 0C4h, 008h, 0F8h, 0C3h, 097h, 066h, 0B8h, 014h, 038h, 0CDh, 020h
        db  01Ch, 000h, 001h, 000h, 097h, 0B8h, 000h, 000h, 000h, 000h, 083h, 0FFh, 0FFh, 074h, 016h, 057h
        db  0B9h, 010h, 000h, 000h, 000h, 033h, 0C0h, 0F3h, 0AAh, 05Fh, 057h, 06Ah, 006h, 0E8h, 0F0h, 003h
        db  000h, 000h, 083h, 0C4h, 008h, 066h, 0C7h, 046h, 01Ch, 0FFh, 0FFh, 0F8h, 0C3h, 066h, 0C7h, 046h
        db  01Ch, 000h, 000h, 0F8h, 0C3h, 066h, 0C7h, 046h, 010h, 053h, 053h, 0B4h, 036h, 0B0h, 034h, 066h
        db  089h, 046h, 018h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h
        db  0C3h, 0F8h, 0C3h
sub_2_51E9:
        call   sub_2_51F4
        call   sub_2_51FF
        ret    
sub_2_51F4:
        mov    dword ptr [0x374], 1          ; -> obj3:0x0374
        ret    
sub_2_51FF:
        xor    eax, eax
        mov    dword ptr [0x378], eax        ; -> obj3:0x0378
        mov    dword ptr [0x37c], eax        ; -> obj3:0x037C
        mov    dword ptr [0x380], eax        ; -> obj3:0x0380
        mov    dword ptr [0x384], eax        ; -> obj3:0x0384
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 095h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0B7h, 000h, 0B9h, 000h, 0BBh, 000h, 000h, 000h
        db  000h, 000h, 07Bh, 001h, 000h, 000h, 0A6h, 002h, 0AEh, 002h, 001h, 003h
loc_2_5282:
        stc    
        ret    
        db  0F8h, 0C3h, 0C3h, 00Fh, 0B7h, 046h, 014h, 083h, 0F8h, 064h, 076h, 005h, 0B8h, 064h, 000h, 000h
        db  000h, 0A3h
        dd  obj3:0x03EC              ; fixup
        db  00Fh, 0B7h, 046h, 010h, 0E8h, 01Fh, 000h, 000h, 000h, 08Bh, 0D8h, 00Fh, 0B7h, 046h, 018h, 0E8h
        db  014h, 000h, 000h, 000h, 0E8h, 0FEh, 0FAh, 0FFh, 0FFh, 074h, 00Bh, 050h, 053h, 052h, 0E8h, 072h
        db  014h, 000h, 000h, 083h, 0C4h, 00Ch, 0F8h, 0C3h, 083h, 0F8h, 001h, 07Dh, 005h, 0B8h, 001h, 000h
        db  000h, 000h, 083h, 0F8h, 064h, 076h, 005h, 0B8h, 064h, 000h, 000h, 000h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -8
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], eax
        call   sub_2_4DB1
        je     loc_2_52FD
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_66F6
        add    esp, 0xc
loc_2_52FD:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x10], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [esi + 0x18], ax
        mov    eax, dword ptr [0x3ec]        ; -> obj3:0x03EC
        mov    word ptr [esi + 0x14], ax
        clc    
        leave  
        ret    
        db  066h, 0C7h, 046h, 010h, 020h, 008h, 033h, 0C0h, 033h, 0C9h, 0E8h, 08Bh, 0FAh, 0FFh, 0FFh, 074h
        db  006h, 08Bh, 042h, 05Ch, 08Bh, 04Ah, 028h, 0C1h, 0E1h, 008h, 00Bh, 0C1h, 066h, 089h, 046h, 018h
        db  0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        movsx  eax, word ptr [esi + 0x10]
        inc    eax
        je     loc_2_536F
        dec    eax
        mov    ebx, 0xfffffffe
        cmp    eax, 4
        ja     loc_2_53BC
        jne    loc_2_5359
        xor    eax, eax
loc_2_5359:
        call   sub_2_4DB1
        mov    ebx, 0
        je     loc_2_53BC
        push   eax
        push   edx
        call   sub_2_66BF
        add    esp, 8
loc_2_536F:
        call   sub_2_4DB1
        mov    ebx, 0
        je     loc_2_53BC
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_66A4
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        or     eax, eax
        jne    loc_2_5394
        mov    eax, 4
loc_2_5394:
        mov    word ptr [esi + 0x10], ax
        cmp    dword ptr [0x24c], 1          ; -> obj3:0x024C
        jne    loc_2_53BA
        dec    eax
        shl    eax, 4
        add    ax, word ptr [0x254]          ; -> obj3:0x0254
        mov    word ptr [esi + 4], ax
        mov    ax, word ptr [0x256]          ; -> obj3:0x0256
        mov    word ptr [esi + 0x38], ax
loc_2_53BA:
        xor    ebx, ebx
loc_2_53BC:
        mov    word ptr [esi + 0x1c], bx
        clc    
        leave  
        ret    
        db  000h, 020h, 010h, 030h, 008h, 028h, 018h, 038h, 004h, 024h, 014h, 034h, 00Ch, 02Ch, 01Ch, 03Ch
sub_2_53D3:
        and    eax, 0xf
        movzx  eax, byte ptr [eax + 0x53c3]  ; -> obj2:0x53C3
        ret    
sub_2_53DE:
        or     eax, eax
        je     loc_2_53FC
        shl    eax, 0x1a
        xor    ebx, ebx
        mov    ecx, 5
loc_2_53EC:
        inc    ebx
        shl    eax, 1
        jb     loc_2_53F3
        loop   loc_2_53EC
loc_2_53F3:
        mov    eax, ebx
        cmp    eax, 4
        jbe    loc_2_53FC
        xor    eax, eax
loc_2_53FC:
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -0xc
        movzx  ecx, word ptr [esi + 0x18]
        jecxz  loc_2_546F
        movzx  eax, word ptr [esi + 0x18]
        shr    eax, 8
        call   sub_2_53DE
        or     eax, eax
        je     loc_2_542C
        call   sub_2_4DB1
        je     loc_2_546F
        push   eax
        push   1
        push   edx
        call   sub_2_6981
        add    esp, 0xc
loc_2_542C:
        movzx  eax, word ptr [esi + 0x18]
        xor    ah, ah
        call   sub_2_53DE
        or     eax, eax
        je     loc_2_544C
        call   sub_2_4DB1
        push   eax
        push   2
        push   edx
        call   sub_2_6981
        add    esp, 0xc
loc_2_544C:
        movsx  eax, word ptr [esi + 0x10]
        cdq    
        mov    ecx, 0x168
        idiv   ecx
        or     edx, edx
        jns    loc_2_545E
        add    edx, ecx
loc_2_545E:
        mov    eax, edx
        call   sub_2_4DB1
        push   eax
        push   edx
        call   sub_2_6B7C
        add    esp, 8
loc_2_546F:
        call   sub_2_4DB1
        mov    eax, 0
        je     loc_2_5521
        lea    eax, [ebp - 8]
        push   eax
        push   1
        push   edx
        call   sub_2_694F
        add    esp, 0xc
        push   dword ptr [ebp - 8]
        call   sub_2_A035
        add    esp, 4
        call   sub_2_53D3
        xor    ebx, ebx
        mov    bh, al
        call   sub_2_4DB1
        lea    eax, [ebp - 8]
        push   eax
        push   2
        push   edx
        call   sub_2_694F
        add    esp, 0xc
        push   dword ptr [ebp - 8]
        call   sub_2_A035
        add    esp, 4
        call   sub_2_53D3
        mov    bl, al
        mov    word ptr [esi + 0x18], bx
        call   sub_2_4DB1
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6B61
        add    esp, 8
        lea    eax, [ebp - 0xc]
        push   eax
        push   6
        call   sub_2_5780
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        call   sub_2_53D3
        mov    ebx, eax
        push   6
        call   sub_2_57D0
        add    esp, 4
        or     eax, eax
        je     loc_2_5511
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x10], ax
        mov    eax, ebx
        mov    ah, 0x1a
        jmp    loc_2_5521
loc_2_5511:
        mov    word ptr [esi + 0x10], bx
        mov    ebx, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x14], bx
        mov    eax, 0xffffffff
loc_2_5521:
        mov    word ptr [esi + 0x1c], ax
        clc    
        leave  
        ret    
        db  066h, 0C7h, 046h, 01Ch, 097h, 0F4h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        movzx  eax, word ptr [esi + 0x18]
        or     eax, eax
        jne    loc_2_5546
        mov    word ptr [esi + 0x18], 0x154
        jmp    loc_2_557A
loc_2_5546:
        xchg   edi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_2_5580
        push   edi
        movzx  ecx, word ptr [esi + 0x18]
        xor    eax, eax
        rep stosb byte ptr es:[edi], al
        pop    edi
        call   sub_2_4DB1
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6B61
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0xb], ax
loc_2_557A:
        xor    eax, eax
        mov    word ptr [esi + 0x1c], ax
loc_2_5580:
        clc    
        leave  
        ret    
        db  083h, 03Dh
        dd  obj3:0x024C              ; fixup
        db  001h, 075h, 016h, 0A1h
        dd  obj3:0x0250              ; fixup
        db  066h, 089h, 046h, 014h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 033h, 0C0h, 066h, 089h, 046h
        db  01Ch, 0F8h, 0C3h
sub_2_55A4:
        ret    
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0xc
        or     eax, eax
        je     loc_2_55D6
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        call   sub_2_55DD
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_55D6:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_55DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        lea    ecx, [edi + 0x28]
        mov    ax, word ptr [esi + 8]
        mov    edx, dword ptr [ecx]
        shl    dx, 8
        xor    dx, ax
        and    dx, 0x700
        xor    dx, ax
        mov    word ptr [esi + 8], dx
        mov    eax, dword ptr [edi + 0x74]
        mov    eax, dword ptr [eax*4]        ; -> obj3:0x03F0
        xor    ax, dx
        and    ax, 7
        xor    ax, dx
        mov    word ptr [esi + 8], ax
        movzx  ebx, word ptr [edi + 0x7a]
        cmp    ebx, 8
        jne    loc_2_5630
        mov    ebx, 3
        jmp    loc_2_5634
loc_2_5630:
        jbe    loc_2_5634
        sub    ebx, ebx
loc_2_5634:
        mov    ax, word ptr [esi + 8]
        shl    bx, 3
        xor    bx, ax
        and    bx, 0x38
        xor    bx, ax
        mov    word ptr [esi + 8], bx
        push   ecx
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        shl    ax, 6
        xor    ax, bx
        and    ax, 0x40
        xor    ax, bx
        mov    word ptr [esi + 8], ax
        and    word ptr [esi + 8], 0x7fff
        cmp    dword ptr [edi + 0x30], 0x1b
        jne    loc_2_568F
        mov    ax, word ptr [esi + 8]
        or     ax, 0x8000
        mov    word ptr [esi + 8], ax
        and    ax, 0x8fff
        or     ax, 0x800
        mov    word ptr [esi + 8], ax
loc_2_568F:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_676C
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0xc], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0xe], ax
        mov    eax, dword ptr [edi + 0x5c]
        mov    word ptr [esi + 0xa], ax
        mov    eax, dword ptr [edi + 0x68]
        mov    word ptr [esi + 4], ax
        mov    ax, word ptr [esi + 8]
        and    ah, 7
        cmp    ah, 2
        jne    loc_2_56F8
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_6A59
        add    esp, 8
        mov    ecx, 0x64
        mov    eax, dword ptr [ebp - 8]
        sub    edx, edx
        div    ecx
        mov    word ptr [esi], ax
        mov    eax, dword ptr [edi + 0x6c]
        mov    word ptr [esi + 2], ax
        mov    eax, dword ptr [edi + 0x38]
        mov    word ptr [esi + 0xa], ax
        mov    ax, word ptr [edi + 0x78]
        mov    word ptr [esi + 6], ax
        jmp    loc_2_5709
loc_2_56F8:
        mov    word ptr [esi], 0
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi + 6], 0
loc_2_5709:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0xc
        or     eax, eax
        je     loc_2_5740
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        call   sub_2_5747
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_5740:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5747:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [edx + 0x5c]
        mov    byte ptr [ecx + 0x10], al
        mov    al, byte ptr [edx + 0x28]
        cmp    al, 2
        mov    byte ptr [ecx + 0x11], al
        jne    loc_2_5768
        mov    al, byte ptr [edx + 0x38]
        mov    byte ptr [ecx + 0x18], al
        jmp    loc_2_576C
loc_2_5768:
        mov    byte ptr [ecx + 0x18], 0
loc_2_576C:
        mov    al, byte ptr [edx + 0x30]
        mov    byte ptr [ecx + 0x19], al
        mov    eax, dword ptr [edx + 0x44]
        mov    word ptr [ecx + 0x14], ax
        sub    eax, eax
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_5780:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0xc
        or     eax, eax
        je     loc_2_57AE
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        call   sub_2_57B5
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_57AE:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_57B5:
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret    
sub_2_57D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0xc
        or     eax, eax
        je     loc_2_57FA
        push   dword ptr [ebp - 4]
        call   sub_2_5801
        add    esp, 4
        jmp    loc_2_57FD
loc_2_57FA:
        mov    eax, dword ptr [ebp - 0xc]
loc_2_57FD:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5801:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x30]
        call   sub_2_96AE
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 056h, 0A1h
        dd  obj3:0x044C              ; fixup
        db  00Bh, 0C0h, 074h, 013h, 08Bh, 030h, 06Ah, 000h, 050h, 0E8h, 000h, 055h, 000h, 000h, 083h, 0C4h
        db  008h, 08Bh, 0C6h, 00Bh, 0F6h, 075h, 0EDh, 0C7h, 005h
        dd  obj3:0x044C              ; fixup
        db  000h, 000h, 000h, 000h, 06Ah, 000h, 0FFh, 035h
        dd  obj3:0x0448              ; fixup
        db  0E8h, 0E0h, 054h, 000h, 000h, 083h, 0C4h, 008h, 0C7h, 005h
        dd  obj3:0x0448              ; fixup
        db  000h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 05Eh, 0C3h
sub_2_585C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [0x3c]         ; -> obj3:0x044C
        or     eax, eax
        je     loc_2_5876
        mov    ecx, dword ptr [ebp + 8]
loc_2_586B:
        cmp    dword ptr [eax + 4], ecx
        je     loc_2_5878
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_2_586B
loc_2_5876:
        sub    eax, eax
loc_2_5878:
        pop    ebp
        ret    
sub_2_587A:
        push   ebp
        mov    ebp, esp
        mov    eax, 0x50                     ; -> obj3:0x0460
        mov    ecx, dword ptr [ebp + 8]
loc_2_5885:
        cmp    dword ptr [eax], ecx
        je     loc_2_5898
        add    eax, 0xc
        cmp    eax, 0x80                     ; -> obj3:0x0490
        jb     loc_2_5885
        mov    eax, 0x80                     ; -> obj3:0x0490
loc_2_5898:
        pop    ebp
        ret    
sub_2_589A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, 0x98                     ; -> obj3:0x04A8
        lea    edi, [ebp - 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        push   1
        push   0x28
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    esi, eax
        push   0x28
        push   eax
        call   sub_2_80EC
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        jne    loc_2_58E3
        push   0
        push   eax
        call   sub_2_AD28
        add    esp, 8
        sub    eax, eax
        jmp    loc_2_59BA
loc_2_58E3:
        call   sub_2_205F
        mov    dword ptr [ebp - 0xc], eax
        push   1
        push   0x10
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     loc_2_59AA
loc_2_5908:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4], eax
        push   dword ptr [esi]
        call   sub_2_587A
        add    esp, 4
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 8], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_5981
        mov    eax, dword ptr [esi]
        cmp    eax, 9
        jg     loc_2_5981
        add    al, 0x30
        mov    byte ptr [ebp - 0x15], al
        push   -1
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_2_AD08
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, -1
        je     loc_2_5981
        cmp    ebx, -2
        je     loc_2_5981
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0xa
        push   ebx
        call   sub_2_AD18
        add    esp, 0x10
        or     eax, eax
        je     loc_2_5978
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 0x10
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    dword ptr [edi + 8], eax
loc_2_5978:
        push   ebx
        call   sub_2_AD10
        add    esp, 4
loc_2_5981:
        add    esi, 4
        cmp    dword ptr [esi], 0
        je     loc_2_599B
        push   1
        push   0x10
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [edi], eax
        mov    edi, eax
        jmp    loc_2_59A1
loc_2_599B:
        mov    dword ptr [edi], 0
loc_2_59A1:
        cmp    dword ptr [esi], 0
        jne    loc_2_5908
loc_2_59AA:
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_AD28
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x10]
loc_2_59BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_59C1:
        push   esi
        push   1
        push   0x28
        call   sub_2_AD20
        add    esp, 8
        mov    esi, eax
        push   0x28
        push   esi
        call   sub_2_80BC
        add    esp, 8
        mov    eax, esi
        pop    esi
        ret    
sub_2_59DF:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 4
        push   eax
        push   dword ptr [ebp + 0xc]
        push   3
        push   0x90                          ; -> obj3:0x04A0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_AD58
        add    esp, 0x1c
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        ret    
sub_2_5A0F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 0xc]
        shl    ecx, 0x18
        mov    eax, dword ptr [ebp + 0x10]
        shl    eax, 0x10
        or     ecx, eax
        or     ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 4], ecx
        push   0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   0x90                          ; -> obj3:0x04A0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_AD60
        add    esp, 0x1c
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, 0xa0                     ; -> obj3:0x04B0
        lea    edi, [ebp - 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 0x1d
        push   eax
        lea    eax, [ebp - 0xc]
        push   dword ptr [ebp + 0xc]
        push   1
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_AD58
        add    esp, 0x1c
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5A94:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   esi
        push   edi
        sub    esi, esi
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_5A50
        add    esp, 0x10
        cmp    eax, esi
        jne    loc_2_5ACE
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   edi
        call   sub_2_5A50
        add    esp, 0x10
        cmp    eax, esi
        je     loc_2_5B0F
loc_2_5ACE:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_5AE8
        push   0x1e
        push   dword ptr [0x18]              ; -> obj3:0x0428
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_2_6E78
        add    esp, 0xc
loc_2_5AE8:
        mov    edi, 0x18                     ; -> obj3:0x0428
loc_2_5AED:
        push   dword ptr [edi]
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        je     loc_2_5B0C
        add    edi, 8
        cmp    edi, 0x38                     ; -> obj3:0x0448
        jb     loc_2_5AED
        jmp    loc_2_5B0F
loc_2_5B0C:
        mov    esi, dword ptr [edi + 4]
loc_2_5B0F:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5B17:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        sub    esi, esi
        push   2
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_AD38
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_5B80
        push   esi
        push   esi
        push   2
        push   dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_AD40
        add    esp, 0x14
        cmp    eax, esi
        jne    loc_2_5B80
        mov    eax, dword ptr [ebp - 4]
        push   esi
        movzx  ecx, word ptr [eax + 4]
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 4]
        push   4
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_AD40
        add    esp, 0x14
        cmp    eax, esi
        jne    loc_2_5B80
        mov    eax, dword ptr [ebp - 4]
        mov    esi, 1
        movzx  ecx, word ptr [eax + 2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
loc_2_5B80:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5B87:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], ebx
        mov    dword ptr [esi], ebx
        lea    eax, [ebp - 0x14]
        push   ebx
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        call   sub_2_59DF
        add    esp, 0x10
        cmp    eax, ebx
        jne    loc_2_5BCA
        push   1
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_59DF
        add    esp, 0x10
        cmp    eax, ebx
        je     loc_2_5BFF
loc_2_5BCA:
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 0x18
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xf0000
        shr    eax, 0x10
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    dword ptr [esi + 4], eax
        push   eax
        call   sub_2_21A7
        add    esp, 4
        or     eax, eax
        je     loc_2_5BFA
        mov    dword ptr [esi], eax
loc_2_5BFA:
        mov    ebx, 1
loc_2_5BFF:
        or     ebx, ebx
        jne    loc_2_5CCF
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        push   edi
        call   sub_2_5B17
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_5CD3
        push   0
        push   0
        push   edi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_AD48
        add    esp, 0x10
        or     eax, eax
        jne    loc_2_5CBC
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_5CBC
        push   1
        push   dword ptr [ebp - 0xc]
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_5CBC
        push   0
        push   dword ptr [ebp - 0xc]
        push   eax
        push   0
        push   edi
        call   sub_2_AD50
        add    esp, 0x14
        or     eax, eax
        jne    loc_2_5CBC
        push   0x5c
        push   dword ptr [ebp - 4]
        call   sub_2_7030
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_2_5CBC
        inc    edi
        mov    eax, edi
loc_2_5C80:
        push   0x26
        push   eax
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     loc_2_5C94
        mov    byte ptr [eax], 0x5c
        jmp    loc_2_5C80
loc_2_5C94:
        push   0
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_AD30
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_5CBC
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        push   dword ptr [ebp - 0x10]
        call   sub_2_5B17
        add    esp, 0xc
        mov    ebx, eax
loc_2_5CBC:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_5CCF
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_AD28
        add    esp, 8
loc_2_5CCF:
        or     ebx, ebx
        je     loc_2_5CEF
loc_2_5CD3:
        cmp    dword ptr [esi], 0
        jne    loc_2_5CEF
        push   dword ptr [esi + 4]
        call   sub_2_21A7
        add    esp, 4
        mov    dword ptr [esi], eax
        or     eax, eax
        jne    loc_2_5CEF
        mov    dword ptr [esi], 4
loc_2_5CEF:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5CF8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi]
        push   eax
        call   sub_2_585C
        add    esp, 4
        or     eax, eax
        je     loc_2_5D5E
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [eax + 0xc], ecx
        jne    loc_2_5D2E
        mov    edx, dword ptr [esi + 4]
        cmp    dword ptr [eax + 8], edx
        jne    loc_2_5D2E
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    loc_2_5D60
loc_2_5D2E:
        or     ecx, ecx
        je     loc_2_5D4F
        cmp    dword ptr [esi + 4], 0
        je     loc_2_5D4F
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    loc_2_5D60
loc_2_5D4F:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    loc_2_5D60
loc_2_5D5E:
        sub    eax, eax
loc_2_5D60:
        pop    esi
        pop    ebp
        ret    
sub_2_5D63:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [0x3c]         ; -> obj3:0x044C
        cmp    dword ptr [esi], 0
        je     loc_2_5D79
loc_2_5D72:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    loc_2_5D72
loc_2_5D79:
        push   1
        push   0x10
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
sub_2_5DB2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_5B87
        add    esp, 8
        or     eax, eax
        je     loc_2_5DFC
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_5DFC
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_5CF8
        add    esp, 8
        or     eax, eax
        je     loc_2_5DED
        mov    eax, 1
        jmp    loc_2_5DFE
loc_2_5DED:
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_5D63
        add    esp, 8
        jmp    loc_2_5DFE
loc_2_5DFC:
        sub    eax, eax
loc_2_5DFE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5E03:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xc], 2
        jne    loc_2_5E2A
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   dword ptr [eax + 0xc]
        call   sub_2_5DB2
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     loc_2_5E2F
loc_2_5E2A:
        mov    eax, 1
loc_2_5E2F:
        pop    ebp
        ret    
sub_2_5E31:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x14], edi
        and    dword ptr [esi + 8], 0xffffffef
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    ebx, 1
        je     loc_2_5ECE
        or     ebx, ebx
        jne    loc_2_5E5E
        or     edi, edi
        je     loc_2_5ECE
loc_2_5E5E:
        push   dword ptr [edi + 0xc]
        call   sub_2_5A94
        add    esp, 4
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        je     loc_2_5EC5
        mov    eax, dword ptr [esi + 8]
        or     eax, 1
        mov    dword ptr [esi + 8], eax
        or     eax, 2
        mov    dword ptr [esi + 8], eax
        push   edi
        push   esi
        call   sub_2_5E03
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_2_5EFE
        cmp    dword ptr [esi + 0xc], 2
        jne    loc_2_5EFE
        mov    eax, dword ptr [esi + 8]
        and    eax, 0xfffffffe
        mov    dword ptr [esi + 8], eax
        and    eax, 0xfffffffd
        mov    dword ptr [esi + 8], eax
        or     eax, 0x10
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], 1
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0
        jmp    loc_2_5EFE
loc_2_5EC5:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_5EFE
loc_2_5ECE:
        call   sub_2_8134
        mov    dword ptr [esi + 0xc], eax
        call   sub_2_8181
        mov    dword ptr [esi + 0x10], eax
        mov    ecx, dword ptr [esi + 8]
        and    ecx, 0xfffffffe
        mov    dword ptr [esi + 8], ecx
        dec    ebx
        cmp    ebx, 1
        sbb    eax, eax
        lea    eax, [eax*2 + 2]
        xor    eax, ecx
        and    eax, 2
        xor    eax, ecx
        mov    dword ptr [esi + 8], eax
loc_2_5EFE:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5F08:
        cmp    dword ptr [0x38], 0           ; -> obj3:0x0448
        jne    loc_2_5F1B
        call   sub_2_59C1
        mov    dword ptr [0x38], eax         ; -> obj3:0x0448
loc_2_5F1B:
        cmp    dword ptr [0x3c], 0           ; -> obj3:0x044C
        jne    loc_2_5F2E
        call   sub_2_589A
        mov    dword ptr [0x3c], eax         ; -> obj3:0x044C
loc_2_5F2E:
        call   sub_2_7FD0
        shl    eax, 2
        xor    eax, dword ptr [0x40]         ; -> obj3:0x0450
        and    eax, 4
        xor    dword ptr [0x40], eax         ; -> obj3:0x0450
        and    dword ptr [0x40], 0xfffffff7  ; -> obj3:0x0450
        mov    eax, 0x38                     ; -> obj3:0x0448
        ret    
        db  000h, 000h
loc_2_5F54:
        and    word ptr [ebp + 0x2c], 0xfffe
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 1
        ja     loc_2_5F76
        je     loc_2_5F6B
        mov    word ptr [ebp + 0x1c], 0x62a
        ret    
loc_2_5F6B:
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0x549], ax          ; -> obj3:0x0549
        ret    
loc_2_5F76:
        cmp    ah, 0x4c
        jne    loc_2_5FAC
        movzx  eax, al
        cmp    al, 0xb
        jae    loc_2_5F94
        mov    ax, word ptr [eax*2 + 0x4da]  ; -> obj3:0x04DA
        movsx  eax, ax
        add    eax, 0x6124                   ; -> loc_2_6124
        jmp    eax
loc_2_5F94:
        sub    al, 0x50
        cmp    al, 0x28
        jae    loc_2_5FBF
        mov    ax, word ptr [eax*2 + 0x4f0]  ; -> obj3:0x04F0
        movsx  eax, ax
        add    eax, 0x6124                   ; -> loc_2_6124
        jmp    eax
loc_2_5FAC:
        cmp    ah, 0x4f
        jne    loc_2_5FBF
        cmp    al, 2
        jae    loc_2_5FBF
        movzx  eax, al
        jmp    dword ptr [eax*4 + 0x540]     ; -> obj3:0x0540
loc_2_5FBF:
        or     word ptr [ebp + 0x2c], 1
        ret    
sub_2_5FC5:
        movzx  eax, al
        movzx  ecx, cl
        mov    edx, esp
        mov    esi, dword ptr [esp]
        sub    esp, eax
        mov    dword ptr [esp], esi
        mov    esi, edx
        sub    edx, ecx
        mov    dword ptr [edx + 4], edi
        cmp    word ptr [0x549], 0           ; -> obj3:0x0549
        je     loc_2_5FEC
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
loc_2_5FEC:
        xchg   edi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_2_6061
        mov    dword ptr [edx + 8], edi
        ret    
sub_2_6001:
        mov    eax, dword ptr [edi]
        add    edi, 2
        movzx  eax, ax
        mov    dword ptr [edx], eax
        sub    edx, 4
        ret    
        db  08Bh, 007h, 083h, 0C7h, 002h, 00Fh, 0BFh, 0C0h, 089h, 002h, 083h, 0EAh, 004h, 0C3h, 08Bh, 007h
        db  083h, 0C7h, 004h, 089h, 002h, 083h, 0EAh, 004h, 0C3h, 089h, 032h, 083h, 0EAh, 004h, 083h, 0EEh
        db  004h, 08Bh, 007h, 083h, 0C7h, 004h, 0E8h, 035h, 000h, 000h, 000h, 089h, 006h, 083h, 0EEh, 004h
        db  0C3h, 08Bh, 007h, 083h, 0C7h, 004h, 0E8h, 025h, 000h, 000h, 000h, 089h, 002h, 083h, 0EAh, 004h
        db  0C3h, 083h, 0C6h, 008h, 08Bh, 07Eh, 0FCh, 08Bh, 006h, 066h, 089h, 007h, 0C3h
sub_2_605C:
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_2_6061:
        pushal 
        mov    esi, 0x4bc                    ; -> obj3:0x04BC
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
        push   esi
        mov    esi, eax
        shr    eax, 0x10
        test   dword ptr [ebx], 0x20
        jne    loc_2_608A
        shl    eax, 4
        movzx  esi, si
        add    eax, esi
loc_2_6085:
        add    eax, dword ptr [ebx + 4]
        pop    esi
        ret    
loc_2_608A:
        test   dword ptr [ebx], 0x180
        jne    loc_2_6097
        movzx  esi, si
        jmp    loc_2_609B
loc_2_6097:
        mov    ax, word ptr [ebp + 0x3c]
loc_2_609B:
        push   edx
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        cmp    eax, -1
        je     loc_2_6061
        add    eax, esi
        cmp    eax, 0x110000
        jb     loc_2_6085
        pop    esi
        ret    
sub_2_60BA:
        sub    edi, dword ptr [esi]
        add    dword ptr [ebp + 0x30], edi
        inc    byte ptr [0x548]              ; -> obj3:0x0548
        cmp    byte ptr [0x548], 1           ; -> obj3:0x0548
        mov    eax, 0xffffffec
        ret    
sub_2_60D2:
        dec    byte ptr [0x548]              ; -> obj3:0x0548
        call   sub_2_605C
        ret    
        push   ebp
        mov    ebp, esp
        add    esp, -8
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0xc
        or     eax, eax
        mov    eax, dword ptr [ebp - 8]
        je     loc_2_6106
        lea    eax, [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
loc_2_6106:
        leave  
        ret    
loc_2_6108:
        mov    al, 8
        mov    cl, 8
        call   sub_2_5FC5
        call   sub_2_60BA
        jne    loc_2_611B
        call   dword ptr [esi - 4]
loc_2_611B:
        call   sub_2_60D2
        add    esp, 8
        ret    
loc_2_6124:
        mov    edi, 0x6570                   ; -> loc_2_6570
        jmp    loc_2_6108
        db  0BFh
        dd  loc_2_659B               ; fixup
        db  0EBh, 0D6h, 0B0h, 014h, 0B1h, 010h, 0E8h, 08Ah, 0FEh, 0FFh, 0FFh, 0E8h, 0E8h, 0FEh, 0FFh, 0FFh
        db  0E8h, 075h, 0FFh, 0FFh, 0FFh, 075h, 003h, 0FFh, 056h, 0FCh, 0E8h, 083h, 0FFh, 0FFh, 0FFh, 0E8h
        db  0FCh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 014h, 0C3h, 0BFh
        dd  sub_2_65A9               ; fixup
        db  0EBh, 0D3h, 0B0h, 010h, 0B1h, 008h, 0E8h, 05Dh, 0FEh, 0FFh, 0FFh, 0E8h, 094h, 0FEh, 0FFh, 0FFh
        db  0E8h, 0CEh, 0FEh, 0FFh, 0FFh, 0E8h, 043h, 0FFh, 0FFh, 0FFh, 075h, 003h, 0FFh, 056h, 0FCh, 0E8h
        db  051h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h, 0BFh
        dd  sub_2_65B9               ; fixup
        db  0EBh, 0D3h, 0BFh
        dd  sub_2_65E2               ; fixup
        db  0EBh, 09Fh, 0BFh
        dd  loc_2_65F2               ; fixup
        db  0EBh, 098h, 0BFh
        dd  loc_2_6602               ; fixup
        db  0EBh, 0BEh, 0BFh
        dd  loc_2_6614               ; fixup
        db  0EBh, 08Ah, 0BFh
        dd  loc_2_6623               ; fixup
        db  0EBh, 083h, 0BFh
        dd  loc_2_6633               ; fixup
        db  0EBh, 0A9h, 0BFh
        dd  loc_2_6645               ; fixup
        db  0E9h, 072h, 0FFh, 0FFh, 0FFh, 0B0h, 010h, 0B1h, 008h, 0E8h, 0FCh, 0FDh, 0FFh, 0FFh, 0E8h, 072h
        db  0FEh, 0FFh, 0FFh, 0E8h, 02Eh, 0FEh, 0FFh, 0FFh, 0E8h, 0E2h, 0FEh, 0FFh, 0FFh, 075h, 00Ah, 0E8h
        db  0FFh, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0E9h, 0FEh, 0FFh, 0FFh, 083h, 0C4h
        db  010h, 0C3h, 0BFh
        dd  sub_2_6658               ; fixup
        db  0EBh, 0CCh, 0B0h, 018h, 0B1h, 010h, 0E8h, 0C8h, 0FDh, 0FFh, 0FFh, 0E8h, 026h, 0FEh, 0FFh, 0FFh
        db  0E8h, 0FAh, 0FDh, 0FFh, 0FFh, 0E8h, 0AEh, 0FEh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 0CBh, 0FEh, 0FFh
        db  0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0B5h, 0FEh, 0FFh, 0FFh, 0E8h, 02Eh, 0FEh, 0FFh, 0FFh
        db  083h, 0C4h, 018h, 0C3h, 0BFh
        dd  sub_2_6692               ; fixup
        db  0EBh, 0C7h, 0BFh
        dd  sub_2_66A4               ; fixup
        db  0EBh, 0C0h, 0B0h, 010h, 0B1h, 008h, 0E8h, 088h, 0FDh, 0FFh, 0FFh, 0E8h, 0BFh, 0FDh, 0FFh, 0FFh
        db  0E8h, 0BAh, 0FDh, 0FFh, 0FFh, 0E8h, 06Eh, 0FEh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 08Bh, 0FEh, 0FFh
        db  0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 075h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h, 0BFh
        dd  sub_2_66BF               ; fixup
        db  0EBh, 0CCh, 0B0h, 024h, 0B1h, 018h, 0E8h, 054h, 0FDh, 0FFh, 0FFh, 0E8h, 0B2h, 0FDh, 0FFh, 0FFh
        db  0E8h, 0ADh, 0FDh, 0FFh, 0FFh, 0E8h, 081h, 0FDh, 0FFh, 0FFh, 0E8h, 035h, 0FEh, 0FFh, 0FFh, 075h
        db  00Ah, 0E8h, 052h, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 03Ch, 0FEh, 0FFh, 0FFh
        db  0E8h, 0B5h, 0FDh, 0FFh, 0FFh, 0E8h, 0B0h, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 024h, 0C3h, 0BFh
        dd  sub_2_66F6               ; fixup
        db  0EBh, 0BDh, 0B0h, 014h, 0B1h, 008h, 0E8h, 011h, 0FDh, 0FFh, 0FFh, 0E8h, 056h, 0FDh, 0FFh, 0FFh
        db  0E8h, 051h, 0FDh, 0FFh, 0FFh, 0E8h, 03Eh, 0FDh, 0FFh, 0FFh, 0E8h, 0F2h, 0FDh, 0FFh, 0FFh, 075h
        db  00Ah, 0E8h, 00Fh, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0F9h, 0FDh, 0FFh, 0FFh
        db  083h, 0C4h, 014h, 0C3h, 0BFh
        dd  sub_2_672F               ; fixup
        db  0EBh, 0C7h, 0BFh
        dd  sub_2_676C               ; fixup
        db  0E9h, 006h, 0FFh, 0FFh, 0FFh, 0BFh
        dd  loc_2_677E               ; fixup
        db  0E9h, 0FCh, 0FEh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6790               ; fixup
        db  0E9h, 032h, 0FFh, 0FFh, 0FFh, 0B0h, 01Ch, 0B1h, 010h, 0E8h, 0BAh, 0FCh, 0FFh, 0FFh, 0E8h, 018h
        db  0FDh, 0FFh, 0FFh, 0E8h, 0ECh, 0FCh, 0FFh, 0FFh, 0E8h, 0E7h, 0FCh, 0FFh, 0FFh, 0E8h, 09Bh, 0FDh
        db  0FFh, 0FFh, 075h, 00Ah, 0E8h, 0B8h, 0FDh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0A2h
        db  0FDh, 0FFh, 0FFh, 0E8h, 01Bh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 01Ch, 0C3h, 0BFh
        dd  sub_2_67A2               ; fixup
        db  0EBh, 0C2h, 0B0h, 014h, 0B1h, 008h, 0E8h, 07Ch, 0FCh, 0FFh, 0FFh, 0E8h, 0C1h, 0FCh, 0FFh, 0FFh
        db  0E8h, 0AEh, 0FCh, 0FFh, 0FFh, 0E8h, 0A9h, 0FCh, 0FFh, 0FFh, 0E8h, 05Dh, 0FDh, 0FFh, 0FFh, 075h
        db  00Ah, 0E8h, 07Ah, 0FDh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 064h, 0FDh, 0FFh, 0FFh
        db  083h, 0C4h, 014h, 0C3h, 0BFh
        dd  sub_2_67F4               ; fixup
        db  0EBh, 0C7h, 0B0h, 018h, 0B1h, 010h, 0E8h, 043h, 0FCh, 0FFh, 0FFh, 0E8h, 0A1h, 0FCh, 0FFh, 0FFh
        db  0E8h, 075h, 0FCh, 0FFh, 0FFh, 0E8h, 029h, 0FDh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 046h, 0FDh, 0FFh
        db  0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 030h, 0FDh, 0FFh, 0FFh, 0E8h, 0A9h, 0FCh, 0FFh, 0FFh
        db  083h, 0C4h, 018h, 0C3h, 0BFh
        dd  sub_2_6860               ; fixup
        db  0EBh, 0C7h, 0B0h, 010h, 0B1h, 008h, 0E8h, 00Ah, 0FCh, 0FFh, 0FFh, 0E8h, 04Fh, 0FCh, 0FFh, 0FFh
        db  0E8h, 03Ch, 0FCh, 0FFh, 0FFh, 0E8h, 0F0h, 0FCh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 00Dh, 0FDh, 0FFh
        db  0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0F7h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h, 0BFh
        dd  sub_2_68C6               ; fixup
        db  0EBh, 0CCh, 0B0h, 024h, 0B1h, 018h, 0E8h, 0D6h, 0FBh, 0FFh, 0FFh, 0E8h, 034h, 0FCh, 0FFh, 0FFh
        db  0E8h, 02Fh, 0FCh, 0FFh, 0FFh, 0E8h, 003h, 0FCh, 0FFh, 0FFh, 0E8h, 0B7h, 0FCh, 0FFh, 0FFh, 075h
        db  00Ah, 0E8h, 0D4h, 0FCh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0BEh, 0FCh, 0FFh, 0FFh
        db  0E8h, 037h, 0FCh, 0FFh, 0FFh, 0E8h, 032h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 024h, 0C3h, 0BFh
        dd  sub_2_691D               ; fixup
        db  0EBh, 0BDh, 0BFh
        dd  sub_2_694F               ; fixup
        db  0E9h, 0CFh, 0FEh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6981               ; fixup
        db  0E9h, 003h, 0FFh, 0FFh, 0FFh, 0BFh
        dd  sub_2_69AE               ; fixup
        db  0E9h, 0BBh, 0FEh, 0FFh, 0FFh, 0BFh
        dd  sub_2_69ED               ; fixup
        db  0E9h, 0B1h, 0FEh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6A59               ; fixup
        db  0E9h, 099h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6A78               ; fixup
        db  0E9h, 0CFh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6B61               ; fixup
        db  0E9h, 085h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6B7C               ; fixup
        db  0E9h, 0BBh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6BB3               ; fixup
        db  0E9h, 071h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6BE4               ; fixup
        db  0E9h, 067h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6C14               ; fixup
        db  0E9h, 05Dh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6C44               ; fixup
        db  0E9h, 061h, 0FEh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6C89               ; fixup
        db  0E9h, 0CEh, 0FEh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6CB8               ; fixup
        db  0E9h, 03Fh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6E64               ; fixup
        db  0E9h, 035h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6E6A               ; fixup
        db  0E9h, 02Bh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6E70               ; fixup
        db  0E9h, 021h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6CBE               ; fixup
        db  0E9h, 017h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6CD9               ; fixup
        db  0E9h, 04Dh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6D10               ; fixup
        db  0E9h, 011h, 0FEh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6D62               ; fixup
        db  0E9h, 045h, 0FEh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6DCE               ; fixup
        db  0E9h, 0FDh, 0FDh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6DD4               ; fixup
        db  0E9h, 031h, 0FEh, 0FFh, 0FFh, 0BFh
        dd  sub_2_6DDA               ; fixup
        db  0E9h, 0DBh, 0FCh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6DF5               ; fixup
        db  0E9h, 011h, 0FDh, 0FFh, 0FFh, 0BFh
        dd  loc_2_6E2C               ; fixup
        db  0E9h, 007h, 0FDh, 0FFh, 0FFh
loc_2_652D:
        mov    al, 0xc
        mov    cl, 8
        call   sub_2_5FC5
        call   sub_2_6001
        call   sub_2_60BA
        jne    loc_2_6545
        call   dword ptr [esi - 4]
loc_2_6545:
        call   sub_2_60D2
        add    esp, 0xc
        ret    
loc_2_654E:
        mov    edi, 0xa7eb                   ; -> loc_2_A7EB
        jmp    loc_2_652D
loc_2_6555:
        mov    edi, 0xa6d7                   ; -> loc_2_A6D7
        jmp    loc_2_652D
sub_2_655C:
        call   sub_2_468E
        call   sub_1_11D4
loc_2_6566:
        push   eax
        call   sub_2_42E8
        add    esp, 4
        ret    
loc_2_6570:
        cmp    dword ptr [4], 0              ; -> obj3:0x0550
        jne    loc_2_6588
        call   sub_2_95DD
        push   1
        call   sub_2_9885
        add    esp, 4
loc_2_6588:
        call   sub_2_655C
        mov    eax, dword ptr [4]            ; -> obj3:0x0550
        inc    eax
        mov    dword ptr [4], eax            ; -> obj3:0x0550
        sub    eax, eax
        ret    
loc_2_659B:
        mov    eax, dword ptr [4]            ; -> obj3:0x0550
        dec    eax
        mov    dword ptr [4], eax            ; -> obj3:0x0550
        sub    eax, eax
        ret    
sub_2_65A9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 0x62a
        sub    eax, eax
        ret    
sub_2_65B9:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   0                             ; -> obj3:0x054C
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_6E78
        add    esp, 0xc
        or     edi, edi
        je     loc_2_65DC
        mov    byte ptr [esi + edi - 1], 0
loc_2_65DC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_65E2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 0
        sub    eax, eax
        ret    
loc_2_65F2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 0x62a
        sub    eax, eax
        ret    
loc_2_6602:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_65B9
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6614:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_65E2
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6623:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 6
        sub    eax, eax
        ret    
loc_2_6633:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_8933
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6645:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj3:0x0008
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    ebp
        ret    
        db  000h
sub_2_6658:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x28]
        mov    word ptr [edx], ax
        mov    eax, dword ptr [ecx + 0x44]
        mov    word ptr [edx + 2], ax
        mov    word ptr [edx + 4], 0
        mov    eax, dword ptr [ecx + 0x5c]
        mov    word ptr [edx + 6], ax
        cmp    dword ptr [ecx + 0x28], 2
        jne    loc_2_668E
        mov    eax, dword ptr [ecx + 0x38]
        mov    word ptr [edx + 8], ax
        mov    word ptr [edx + 0xa], 0
loc_2_668E:
        sub    eax, eax
        pop    ebp
        ret    
sub_2_6692:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x30]
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret    
sub_2_66A4:
        push   ebp
        mov    ebp, esp
        push   0
        push   8
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret    
sub_2_66BF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   8
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        push   edi
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x34]
        call   sub_2_7AFD
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_66F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   9
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_672F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        push   eax
        push   9
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_9825
        add    esp, 0xc
        push   esi
        push   dword ptr [edi + 0x2c]
        push   dword ptr [edi + 0x34]
        call   sub_2_7F7A
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_676C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x4c]
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret    
loc_2_677E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_AB2D
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6790:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_AB30
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_67A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, 0xfffffffd
        lea    ecx, [esi - 1]
        cmp    ecx, 0x20
        jae    loc_2_67EF
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   4
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        add    esp, 0xc
        push   esi
        call   sub_2_A035
        add    esp, 4
        and    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        sbb    ecx, ecx
        inc    ecx
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        sub    eax, eax
loc_2_67EF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_67F4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, 0xfffffffd
        lea    ecx, [esi - 1]
        cmp    ecx, 0x20
        jae    loc_2_685A
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   4
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_9847
        add    esp, 0xc
        cmp    dword ptr [ebp + 0x10], 0
        push   esi
        je     loc_2_683E
        call   sub_2_A035
        add    esp, 4
        or     dword ptr [ebp - 4], eax
        jmp    loc_2_684B
loc_2_683E:
        call   sub_2_A035
        add    esp, 4
        not    eax
        and    dword ptr [ebp - 4], eax
loc_2_684B:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_2_A531
        add    esp, 0xc
loc_2_685A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6860:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_691D
        add    esp, 0xc
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_694F
        add    esp, 0xc
        lea    eax, [ebp - 0xc]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_694F
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0x10], eax
        jne    loc_2_68B6
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_2_68B6
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 1
        jmp    loc_2_68BF
loc_2_68B6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 0
loc_2_68BF:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_68C6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_691D
        add    esp, 0xc
        mov    edi, dword ptr [ebp + 0xc]
        or     edi, edi
        je     loc_2_68EE
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_68F1
loc_2_68EE:
        mov    eax, dword ptr [ebp - 8]
loc_2_68F1:
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_6981
        add    esp, 0xc
        or     edi, edi
        je     loc_2_6907
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_690A
loc_2_6907:
        mov    eax, dword ptr [ebp - 4]
loc_2_690A:
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_6981
        add    esp, 0xc
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_691D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x30]
        call   sub_2_95E8
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_694F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_A46C
        add    esp, 0xc
        mov    esi, eax
        push   dword ptr [ebp - 4]
        call   sub_2_A020
        mov    ecx, dword ptr [ebp + 0x10]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6981:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, 0xfffffffd
        lea    edx, [ecx - 1]
        cmp    edx, 0x20
        jae    loc_2_69AC
        push   ecx
        call   sub_2_A035
        add    esp, 4
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_A494
        add    esp, 0xc
loc_2_69AC:
        pop    ebp
        ret    
sub_2_69AE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x4c], ecx
        jae    loc_2_69CE
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], esi
        mov    esi, 0xfffffffd
        jmp    loc_2_69E6
loc_2_69CE:
        lea    edx, [ebp - 4]
        push   edx
        push   ecx
        push   dword ptr [eax + 0x30]
        call   sub_2_9612
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
loc_2_69E6:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_69ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, 0xfffffffd
        lea    ecx, [edi - 1]
        cmp    ecx, 0x20
        jae    loc_2_6A52
        mov    ebx, 1
        mov    esi, dword ptr [ebp + 8]
loc_2_6A0E:
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   esi
        call   sub_2_694F
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], edi
        je     loc_2_6A27
        inc    ebx
        cmp    ebx, 0x20
        jbe    loc_2_6A0E
loc_2_6A27:
        cmp    ebx, 4
        ja     loc_2_6A44
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   esi
        call   sub_2_69AE
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], edx
        jmp    loc_2_6A52
loc_2_6A44:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, 0xfffffff6
        mov    dword ptr [ecx], 0
loc_2_6A52:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6A59:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        cmp    dword ptr [ecx + 0x28], 2
        jne    loc_2_6A71
        mov    edx, dword ptr [ecx + 0x3c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], edx
        jmp    loc_2_6A76
loc_2_6A71:
        mov    eax, 0xfffffff5
loc_2_6A76:
        pop    ebp
        ret    
sub_2_6A78:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x4b0
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_2_6B39
        mov    eax, dword ptr [esi + 0x74]
        cmp    eax, 1
        jb     loc_2_6AA7
        cmp    eax, 2
        jbe    loc_2_6AF9
        cmp    eax, 6
        je     loc_2_6AF9
        cmp    eax, 7
        je     loc_2_6B18
loc_2_6AA7:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0x4b0
        je     loc_2_6AC6
        cmp    eax, 0x2580
        je     loc_2_6AC6
        mov    dword ptr [esi + 0x3c], 0x4b0
        mov    ebx, 0xffffffee
        jmp    loc_2_6B3E
loc_2_6AC6:
        cmp    dword ptr [esi + 0x30], 0x17
        je     loc_2_6AEB
        cmp    dword ptr [esi + 0x30], 0x13
        jne    loc_2_6AD8
        cmp    dword ptr [esi + 0x74], 4
        je     loc_2_6AEB
loc_2_6AD8:
        cmp    dword ptr [esi + 0x30], 0xb
        je     loc_2_6AEB
        cmp    dword ptr [esi + 0x30], 0x29
        je     loc_2_6AEB
        mov    dword ptr [esi + 0x3c], eax
        sub    ebx, ebx
        jmp    loc_2_6B3E
loc_2_6AEB:
        mov    dword ptr [esi + 0x3c], 0x4b0
        mov    ebx, 0xffffffee
        jmp    loc_2_6B3E
loc_2_6AF9:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0x4b0
        jne    loc_2_6B0A
        mov    dword ptr [esi + 0x3c], eax
        sub    ebx, ebx
        jmp    loc_2_6B3E
loc_2_6B0A:
        mov    dword ptr [esi + 0x3c], 0x4b0
        mov    ebx, 0xffffffee
        jmp    loc_2_6B3E
loc_2_6B18:
        mov    edi, 0x960
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, edi
        jne    loc_2_6B2B
        mov    dword ptr [esi + 0x3c], eax
        sub    ebx, ebx
        jmp    loc_2_6B3E
loc_2_6B2B:
        mov    dword ptr [esi + 0x3c], 0x960
        mov    ebx, 0xffffffee
        jmp    loc_2_6B3E
loc_2_6B39:
        mov    ebx, 0xfffffff5
loc_2_6B3E:
        or     ebx, ebx
        jne    loc_2_6B5A
        push   dword ptr [esi + 0x3c]
        push   esi
        call   sub_2_4630
        add    esp, 8
        or     eax, eax
        jne    loc_2_6B5A
        mov    dword ptr [esi + 0x3c], edi
        mov    ebx, 0xffffffee
loc_2_6B5A:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_6B61:
        push   ebp
        mov    ebp, esp
        push   0
        push   6
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret    
sub_2_6B7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   6
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        push   edi
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x34]
        call   sub_2_7F19
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6BB3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_422E
        add    esp, 4
        cmp    eax, esi
        je     loc_2_6BD9
        movzx  ecx, word ptr [edi + 0x8f]
        mov    eax, dword ptr [ebp + 0xc]
        inc    ecx
        mov    dword ptr [eax], ecx
        jmp    loc_2_6BDE
loc_2_6BD9:
        mov    esi, 0xfffffff4
loc_2_6BDE:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_6BE4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_4201
        add    esp, 4
        cmp    eax, esi
        je     loc_2_6C09
        mov    eax, dword ptr [ebp + 0xc]
        movzx  ecx, word ptr [edi + 0x8d]
        mov    dword ptr [eax], ecx
        jmp    loc_2_6C0E
loc_2_6C09:
        mov    esi, 0xfffffff3
loc_2_6C0E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_6C14:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_425B
        add    esp, 4
        cmp    eax, esi
        je     loc_2_6C39
        mov    eax, dword ptr [ebp + 0xc]
        movzx  ecx, word ptr [edi + 0x91]
        mov    dword ptr [eax], ecx
        jmp    loc_2_6C3E
loc_2_6C39:
        mov    esi, 0xfffffff1
loc_2_6C3E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_6C44:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_4288
        add    esp, 4
        cmp    eax, esi
        je     loc_2_6C7E
        movzx  ecx, word ptr [edi + 0x91]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    ecx, eax
        jb     loc_2_6C77
        mov    ecx, dword ptr [edi + eax*4 + 0x8f]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        jmp    loc_2_6C83
loc_2_6C77:
        mov    esi, 0xfffffff0
        jmp    loc_2_6C83
loc_2_6C7E:
        mov    esi, 0xfffffff1
loc_2_6C83:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_6C89:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_4297
        add    esp, 4
        cmp    eax, esi
        je     loc_2_6CAD
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edi + 0x9b]
        mov    dword ptr [eax], ecx
        jmp    loc_2_6CB2
loc_2_6CAD:
        mov    esi, 0xfffffff1
loc_2_6CB2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_6CB8:
        mov    eax, 0xfffffffe
        ret    
sub_2_6CBE:
        push   ebp
        mov    ebp, esp
        push   0
        push   3
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret    
sub_2_6CD9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   3
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        push   edi
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x34]
        call   sub_2_7CBD
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, 0xfffffffd
        lea    ecx, [esi - 1]
        cmp    ecx, 0x20
        jae    loc_2_6D5D
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   5
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        add    esp, 0xc
        push   esi
        call   sub_2_A035
        add    esp, 4
        and    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        sbb    ecx, ecx
        inc    ecx
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        sub    eax, eax
loc_2_6D5D:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6D62:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, 0xfffffffd
        lea    ecx, [esi - 1]
        cmp    ecx, 0x20
        jae    loc_2_6DC8
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   5
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_9847
        add    esp, 0xc
        cmp    dword ptr [ebp + 0x10], 0
        push   esi
        je     loc_2_6DAC
        call   sub_2_A035
        add    esp, 4
        or     dword ptr [ebp - 4], eax
        jmp    loc_2_6DB9
loc_2_6DAC:
        call   sub_2_A035
        add    esp, 4
        not    eax
        and    dword ptr [ebp - 4], eax
loc_2_6DB9:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_2_A4E4
        add    esp, 0xc
loc_2_6DC8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6DCE:
        mov    eax, 0xfffffffe
        ret    
loc_2_6DD4:
        mov    eax, 0xfffffffe
        ret    
sub_2_6DDA:
        push   ebp
        mov    ebp, esp
        push   0
        push   1
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret    
loc_2_6DF5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        push   edi
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x34]
        call   sub_2_7E1D
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6E2C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, 0xfffffffd
        lea    ecx, [edi - 1]
        cmp    ecx, 0x20
        jae    loc_2_6E60
        push   0
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_6D62
        add    esp, 0xc
        or     eax, eax
        jl     loc_2_6E60
        push   0
        push   edi
        push   esi
        call   sub_2_67F4
        add    esp, 0xc
loc_2_6E60:
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_6E64:
        mov    eax, 0xfffffffe
        ret    
loc_2_6E6A:
        mov    eax, 0xfffffffe
        ret    
loc_2_6E70:
        mov    eax, 0xfffffffe
        ret    
        db  000h, 000h
sub_2_6E78:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, edi
        mov    ecx, dword ptr [ebp + 0x10]
        jcxz   loc_2_6E98
loc_2_6E8C:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_2_6E94
        stosb  byte ptr es:[edi], al
        loop   loc_2_6E8C
loc_2_6E94:
        xor    al, al
        rep stosb byte ptr es:[edi], al
loc_2_6E98:
        mov    eax, ebx
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h
sub_2_6EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 8]
        mov    al, 0xff
loc_2_6EAD:
        or     al, al
        je     loc_2_6EDD
        lodsb  al, byte ptr [esi]
        mov    ah, byte ptr [ebx]
        inc    ebx
        cmp    ah, al
        je     loc_2_6EAD
        sub    al, 0x41
        cmp    al, 0x1a
        sbb    cl, cl
        and    cl, 0x20
        add    al, cl
        add    al, 0x41
        xchg   al, ah
        sub    al, 0x41
        cmp    al, 0x1a
        sbb    cl, cl
        and    cl, 0x20
        add    al, cl
        add    al, 0x41
        cmp    al, ah
        je     loc_2_6EAD
        sbb    al, al
        sbb    al, 0xff
loc_2_6EDD:
        movsx  eax, al
        pop    ebx
        pop    esi
        leave  
        ret    
sub_2_6EE4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    bl, 1
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        cmp    cl, 0xa
        jne    loc_2_6EFA
        cwde   
loc_2_6EFA:
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        cld    
        xchg   ebx, eax
        or     al, al
        je     loc_2_6F17
        cmp    cl, 0xa
        jne    loc_2_6F17
        or     edx, edx
        jns    loc_2_6F17
        mov    al, 0x2d
        stosb  byte ptr es:[edi], al
        neg    ebx
        adc    edx, 0
        neg    edx
loc_2_6F17:
        mov    esi, edi
loc_2_6F19:
        xchg   edx, eax
        xor    edx, edx
        or     eax, eax
        je     loc_2_6F22
        div    ecx
loc_2_6F22:
        xchg   ebx, eax
        div    ecx
        xchg   edx, eax
        xchg   ebx, edx
        add    al, 0x30
        cmp    al, 0x39
        jbe    loc_2_6F30
        add    al, 0x27
loc_2_6F30:
        stosb  byte ptr es:[edi], al
        mov    eax, edx
        or     eax, ebx
        jne    loc_2_6F19
        mov    byte ptr [edi], al
loc_2_6F39:
        dec    edi
        lodsb  al, byte ptr [esi]
        xchg   byte ptr [edi], al
        mov    byte ptr [esi - 1], al
        lea    eax, [esi + 1]
        cmp    eax, edi
        jb     loc_2_6F39
        pop    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        cwde   
        xor    ebx, ebx
loc_2_6F5E:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_2_6F5E
        cmp    al, 9
        je     loc_2_6F5E
        push   eax
        cmp    al, 0x2d
        je     loc_2_6F70
        cmp    al, 0x2b
        jne    loc_2_6F71
loc_2_6F70:
        lodsb  al, byte ptr [esi]
loc_2_6F71:
        cmp    al, 0x39
        ja     loc_2_6F94
        sub    al, 0x30
        jb     loc_2_6F94
        shl    ebx, 1
        rcl    edx, 1
        mov    ecx, ebx
        mov    edi, edx
        shl    ebx, 1
        rcl    edx, 1
        shl    ebx, 1
        rcl    edx, 1
        add    ebx, ecx
        adc    edx, edi
        add    ebx, eax
        adc    edx, 0
        jmp    loc_2_6F70
loc_2_6F94:
        pop    eax
        cmp    al, 0x2d
        xchg   ebx, eax
        jne    loc_2_6FA1
        neg    eax
        adc    edx, 0
        neg    edx
loc_2_6FA1:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  000h, 000h
sub_2_6FA8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        jcxz   loc_2_6FF8
        mov    bh, 0x41
        mov    bl, 0x5a
        mov    dh, 0x20
loc_2_6FC0:
        mov    ah, byte ptr [esi]
        mov    al, byte ptr [edi]
        or     ah, ah
        je     loc_2_6FE8
        or     al, al
        je     loc_2_6FE8
        inc    esi
        inc    edi
        cmp    ah, bh
        jb     loc_2_6FD8
        cmp    ah, bl
        ja     loc_2_6FD8
        add    ah, dh
loc_2_6FD8:
        cmp    al, bh
        jb     loc_2_6FE2
        cmp    al, bl
        ja     loc_2_6FE2
        add    al, dh
loc_2_6FE2:
        cmp    ah, al
        jne    loc_2_6FEE
        loop   loc_2_6FC0
loc_2_6FE8:
        xor    ecx, ecx
        cmp    ah, al
        je     loc_2_6FF8
loc_2_6FEE:
        mov    cx, 0
        jb     loc_2_6FF6
        dec    ecx
        dec    ecx
loc_2_6FF6:
        not    ecx
loc_2_6FF8:
        mov    eax, ecx
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  000h
sub_2_7000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, edi
        xor    eax, eax
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    al, byte ptr [ebp + 0xc]
        mov    edi, ebx
        repne scasb al, byte ptr es:[edi]
        je     loc_2_7029
        or     al, al
        jne    loc_2_7025
        inc    edi
        jmp    loc_2_7029
loc_2_7025:
        xor    eax, eax
        jmp    loc_2_702C
loc_2_7029:
        lea    eax, [edi - 1]
loc_2_702C:
        pop    ebx
        pop    edi
        leave  
        ret    
sub_2_7030:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    edi
        mov    al, byte ptr [ebp + 0xc]
        std    
        repne scasb al, byte ptr es:[edi]
        je     loc_2_7051
        xor    eax, eax
        mov    edx, eax
        jmp    loc_2_7054
loc_2_7051:
        lea    eax, [edi + 1]
loc_2_7054:
        cld    
        pop    edi
        leave  
        ret    
sub_2_7058:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, ebx
        jmp    loc_2_706F
        db  090h
loc_2_7064:
        sub    al, 0x61
        cmp    al, 0x1a
        jae    loc_2_706E
        add    al, 0x41
        mov    byte ptr [ebx], al
loc_2_706E:
        inc    ebx
loc_2_706F:
        mov    al, byte ptr [ebx]
        or     al, al
        jne    loc_2_7064
        xchg   edx, eax
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   esi
        push   edi
        push   ebx
        mov    dl, 1
        mov    byte ptr [ebp - 1], dl
        mov    esi, dword ptr [ebp + 8]
loc_2_708D:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_2_708D
        cmp    al, 0xd
        ja     loc_2_709C
        cmp    al, 9
        jb     loc_2_709C
        jmp    loc_2_708D
loc_2_709C:
        cmp    al, 0x2b
        je     loc_2_70A8
        cmp    al, 0x2d
        jne    loc_2_70A9
        or     byte ptr [ebp - 1], 2
loc_2_70A8:
        lodsb  al, byte ptr [esi]
loc_2_70A9:
        mov    ecx, dword ptr [ebp + 0x10]
        jecxz  loc_2_70BD
        cmp    ecx, 1
        je     loc_2_70B8
        cmp    ecx, 0x24
        jbe    loc_2_70DC
loc_2_70B8:
        jmp    loc_2_7150
loc_2_70BD:
        mov    ecx, 0xa
        cmp    al, 0x30
        jne    loc_2_70DC
        cmp    byte ptr [esi], 0x78
        je     loc_2_70D7
        cmp    byte ptr [esi], 0x58
        je     loc_2_70D7
        mov    ecx, 8
        jmp    loc_2_70DC
loc_2_70D7:
        mov    ecx, 0x10
loc_2_70DC:
        mov    dword ptr [ebp + 0x10], ecx
        cmp    ecx, 0x10
        jne    loc_2_70F4
        cmp    al, 0x30
        jne    loc_2_70F4
        cmp    byte ptr [esi], 0x78
        je     loc_2_70F2
        cmp    byte ptr [esi], 0x58
        jne    loc_2_70F4
loc_2_70F2:
        inc    esi
        lodsb  al, byte ptr [esi]
loc_2_70F4:
        xor    edx, edx
        xor    ebx, ebx
loc_2_70F8:
        call   sub_2_718D
        jb     loc_2_7159
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    eax, ecx
        jae    loc_2_7159
        or     byte ptr [ebp - 1], 8
        mov    edi, eax
        mov    eax, edx
        mul    ecx
        jae    loc_2_7116
        or     byte ptr [ebp - 1], 4
loc_2_7116:
        push   eax
        mov    eax, ebx
        mul    ecx
        pop    ecx
        add    edx, ecx
        jae    loc_2_7124
        or     byte ptr [ebp - 1], 4
loc_2_7124:
        add    eax, edi
        adc    edx, 0
        jae    loc_2_712F
        or     byte ptr [ebp - 1], 4
loc_2_712F:
        mov    ebx, eax
        lodsb  al, byte ptr [esi]
        jmp    loc_2_70F8
loc_2_7134:
        mov    edx, 0x7fffffff
        mov    ebx, 0xffffffff
        test   byte ptr [ebp - 1], 1
        je     loc_2_7170
        or     edx, 0x80000000
        and    byte ptr [ebp - 1], 0xfd
        jmp    loc_2_7170
loc_2_7150:
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        xor    edx, edx
        jmp    loc_2_7170
loc_2_7159:
        dec    esi
        test   byte ptr [ebp - 1], 8
        je     loc_2_7150
        test   byte ptr [ebp - 1], 4
        jne    loc_2_7134
        test   byte ptr [ebp - 1], 1
        jne    loc_2_7170
        or     edx, edx
        js     loc_2_7134
loc_2_7170:
        mov    edi, dword ptr [ebp + 0xc]
        or     edi, edi
        je     loc_2_7179
        mov    dword ptr [edi], esi
loc_2_7179:
        mov    eax, ebx
        test   byte ptr [ebp - 1], 2
        je     loc_2_7188
        neg    eax
        adc    edx, 0
        neg    edx
loc_2_7188:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_2_718D:
        cmp    al, 0x61
        jb     loc_2_7193
        and    al, 0xdf
loc_2_7193:
        sub    al, 0x30
        jb     loc_2_71A4
        cmp    al, 0xa
        jb     loc_2_71A3
        cmp    al, 0x11
        jb     loc_2_71A4
        sub    al, 7
        cmp    al, 0x24
loc_2_71A3:
        cmc    
loc_2_71A4:
        movsx  eax, al
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_2_71AC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    ebx, ebx
        xor    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
loc_2_71B8:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_2_71B8
        cmp    al, 9
        je     loc_2_71B8
        cmp    al, 0x2b
        je     loc_2_71CA
        cmp    al, 0x2d
        jne    loc_2_71CB
        dec    ecx
loc_2_71CA:
        lodsb  al, byte ptr [esi]
loc_2_71CB:
        cmp    al, 0x30
        jb     loc_2_71EF
        cmp    al, 0x39
        jbe    loc_2_71DF
        or     al, 0x20
        cmp    al, 0x66
        ja     loc_2_71EF
        cmp    al, 0x61
        jb     loc_2_71EF
        sub    al, 0x57
loc_2_71DF:
        and    eax, 0xf
        cmp    eax, dword ptr [ebp + 0xc]
        jae    loc_2_71EF
        xchg   ebx, eax
        mul    dword ptr [ebp + 0xc]
        add    ebx, eax
        jmp    loc_2_71CA
loc_2_71EF:
        mov    eax, ebx
        jcxz   loc_2_71F6
        neg    eax
loc_2_71F6:
        pop    ebx
        pop    esi
        leave  
        ret    
        db  000h, 000h
sub_2_71FC:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 9
        jb     loc_2_7212
        cmp    al, 0xd
        ja     loc_2_7212
        cmp    al, 0x20
        jne    loc_2_7212
        mov    al, 1
        jmp    loc_2_7214
loc_2_7212:
        xor    eax, eax
loc_2_7214:
        leave  
        ret    
        db  000h, 000h
sub_2_7218:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   0x7c                          ; -> obj3:0x05D0
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_863A
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_7263
        push   dword ptr [ebp + 0x14]
        push   ebx
        push   edi
        push   esi
        call   sub_2_867A
        add    esp, 0x10
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   ebx
        push   edi
        push   esi
        call   sub_2_863A
        add    esp, 0x18
loc_2_7263:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_7268:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        jne    loc_2_728A
        push   0
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_867A
        add    esp, 0x10
        jmp    loc_2_72FF
loc_2_728A:
        push   1
        push   0x80
        call   sub_1_12F4
loc_2_7296:
        add    esp, 8
        mov    dword ptr [0x3c], eax         ; -> obj3:0x0590
        or     eax, eax
        je     loc_2_72FD
        push   0x80
        push   eax
        push   0x80                          ; -> obj3:0x05D4
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_863A
        add    esp, 0x18
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        push   edi
        call   sub_2_6EA0
        add    esp, 8
        mov    ebx, eax
        push   0
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_1_12FC
loc_2_72DD:
        add    esp, 8
        or     ebx, ebx
        je     loc_2_72F6
        push   edi
        push   esi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_867A
        add    esp, 0x10
        jmp    loc_2_72FF
loc_2_72F6:
        mov    eax, 1
        jmp    loc_2_72FF
loc_2_72FD:
        sub    eax, eax
loc_2_72FF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 0x30
        jl     loc_2_7317
        cmp    al, 0x39
        mov    eax, 1
        jle    loc_2_7319
loc_2_7317:
        sub    eax, eax
loc_2_7319:
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    bl, byte ptr [ebp + 8]
        movsx  eax, bl
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    loc_2_733C
        cmp    bl, 0x2c
        mov    eax, 0
        jne    loc_2_7341
loc_2_733C:
        mov    eax, 1
loc_2_7341:
        pop    ebx
        pop    ebp
        ret    
sub_2_7344:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + ecx*4]
        pop    ebp
        ret    
sub_2_7352:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        sub    ebx, ebx
        cmp    dword ptr [ebp + 0xc], ebx
        je     loc_2_7391
        mov    esi, dword ptr [ebp + 0x14]
loc_2_7364:
        or     edi, edi
        jne    loc_2_7391
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_2_7344
        add    esp, 8
        push   dword ptr [ebp + 0x10]
        push   eax
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        jne    loc_2_738B
        mov    dword ptr [esi], ebx
        mov    edi, 1
loc_2_738B:
        inc    ebx
        cmp    dword ptr [ebp + 0xc], ebx
        ja     loc_2_7364
loc_2_7391:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_7398:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax], 0
        je     loc_2_73AE
loc_2_73A3:
        cmp    byte ptr [eax], 0x20
        jne    loc_2_73AE
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_2_73A3
loc_2_73AE:
        pop    ebp
        ret    
sub_2_73B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0xc]
        push   0x84                          ; -> obj3:0x05D8
        push   dword ptr [ebp + 8]
        call   sub_2_6EE4
        add    esp, 0xc
        push   0x84                          ; -> obj3:0x05D8
        call   sub_2_7398
        add    esp, 4
        mov    ebx, eax
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, ecx
        push   1
        push   edi
        call   sub_1_12F4
loc_2_73EF:
        add    esp, 8
        mov    esi, eax
        push   edi
        push   ebx
        push   esi
        call   sub_2_6E78
        add    esp, 0xc
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_7406:
        push   ebp
        mov    ebp, esp
        mov    edx, 1
        mov    cl, byte ptr [ebp + 0xc]
        shl    edx, cl
        and    edx, dword ptr [ebp + 8]
        cmp    edx, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        ret    
sub_2_741E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, 0x1f
loc_2_742C:
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_2_7406
        add    esp, 8
        or     eax, eax
        jne    loc_2_7442
        dec    edi
        cmp    edi, 3
        jae    loc_2_742C
loc_2_7442:
        inc    edi
        push   1
        lea    eax, [edi + 1]
        push   eax
        call   sub_1_12F4
loc_2_744E:
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    esi, eax
        sub    ebx, ebx
        cmp    edi, ebx
        je     loc_2_7477
loc_2_745C:
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_2_7406
        add    esp, 8
        cmp    eax, 1
        sbb    al, al
        add    al, 0x31
        mov    byte ptr [esi], al
        inc    esi
        inc    ebx
        cmp    edi, ebx
        ja     loc_2_745C
loc_2_7477:
        mov    byte ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7484:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    eax, eax
        sub    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
loc_2_7490:
        mov    bl, byte ptr [edx]
        or     bl, bl
        je     loc_2_74AB
        inc    edx
        cmp    bl, 0x31
        jne    loc_2_74A5
        mov    esi, 1
        shl    esi, cl
        or     eax, esi
loc_2_74A5:
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_7490
loc_2_74AB:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_74AF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    edx, edx
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, eax
        je     loc_2_752C
        cmp    byte ptr [ecx], 1
        sbb    esi, esi
        neg    esi
        cmp    esi, eax
        jne    loc_2_74E2
loc_2_74CA:
        mov    al, byte ptr [ecx + edx]
        cmp    al, 0x2c
        jne    loc_2_74E2
        cmp    al, 0x20
        jne    loc_2_74E2
        inc    edx
        cmp    byte ptr [ecx + edx], 1
        sbb    esi, esi
        neg    esi
        or     esi, esi
        je     loc_2_74CA
loc_2_74E2:
        or     esi, esi
        je     loc_2_74EF
        sub    eax, eax
        mov    dword ptr [0x70], eax         ; -> obj3:0x05C4
        jmp    loc_2_752C
loc_2_74EF:
        lea    eax, [ecx + edx]
        or     esi, esi
        jne    loc_2_7514
loc_2_74F6:
        mov    bl, byte ptr [ecx + edx]
        cmp    bl, 0x2c
        je     loc_2_7510
        cmp    bl, 0x20
        je     loc_2_7510
        inc    edx
        cmp    byte ptr [ecx + edx], 1
        sbb    esi, esi
        neg    esi
        or     esi, esi
        je     loc_2_74F6
loc_2_7510:
        or     esi, esi
        je     loc_2_7520
loc_2_7514:
        mov    dword ptr [0x70], 0           ; -> obj3:0x05C4
        jmp    loc_2_752C
loc_2_7520:
        add    ecx, edx
        mov    byte ptr [ecx], 0
        inc    ecx
        mov    dword ptr [0x70], ecx         ; -> obj3:0x05C4
loc_2_752C:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_7530:
        push   dword ptr [0x70]              ; -> obj3:0x05C4
        call   sub_2_74AF
        add    esp, 4
        ret    
sub_2_753F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, edx
        mov    esi, dword ptr [edx]
        cmp    esi, eax
        je     loc_2_755A
loc_2_7551:
        add    ecx, 4
        inc    eax
        cmp    dword ptr [ecx], 0
        jne    loc_2_7551
loc_2_755A:
        mov    ecx, dword ptr [ebp + 0x10]
        shl    ecx, 2
        lea    edi, [eax + 1]
        cmp    ecx, edi
        jae    loc_2_756B
        sub    eax, eax
        jmp    loc_2_7587
loc_2_756B:
        or     esi, esi
        mov    ecx, dword ptr [ebp + 0xc]
        je     loc_2_7581
loc_2_7572:
        mov    esi, dword ptr [edx]
        mov    dword ptr [ecx], esi
        add    edx, 4
        add    ecx, 4
        cmp    dword ptr [edx], 0
        jne    loc_2_7572
loc_2_7581:
        mov    dword ptr [ecx], 0
loc_2_7587:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_758B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0x78], 0x74        ; -> obj3:0x05CC
        push   1
        push   0x80
        call   sub_1_12F4
loc_2_75A7:
        add    esp, 8
        mov    dword ptr [0x3c], eax         ; -> obj3:0x0590
        or     eax, eax
        je     loc_2_760F
        push   0x80
        push   eax
        mov    esi, dword ptr [ebp + 0x14]
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 0x18
        push   dword ptr [0x78]              ; -> obj3:0x05CC
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        mov    ebx, dword ptr [ebp + 0x1c]
        push   ebx
        mov    edi, dword ptr [ebp + 0x18]
        push   edi
        call   sub_2_7352
        add    esp, 0x10
        or     eax, eax
        jne    loc_2_75FF
        push   dword ptr [0x78]              ; -> obj3:0x05CC
        push   esi
        push   ebx
        push   edi
        call   sub_2_7352
        add    esp, 0x10
loc_2_75FF:
        push   0
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_1_12FC
loc_2_760C:
        add    esp, 8
loc_2_760F:
        mov    eax, dword ptr [0x78]         ; -> obj3:0x05CC
        pop    edi
        mov    eax, dword ptr [eax]
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_761B:
        push   ebp
        mov    ebp, esp
        push   7
        push   0x20                          ; -> obj3:0x0574
        push   dword ptr [ebp + 0x14]
        push   0x20                          ; -> obj3:0x0574
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_758B
        add    esp, 0x18
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        ret    
sub_2_764F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 0x18]
        push   ebx
        push   dword ptr [ebp + 0x14]
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   1
        push   0x80
        call   sub_1_12F4
loc_2_7674:
        add    esp, 8
        mov    dword ptr [0x3c], eax         ; -> obj3:0x0590
        cmp    eax, edi
        je     loc_2_76B9
        push   0x80
        push   eax
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 0x18
        push   ebx
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_2_71AC
        add    esp, 8
        mov    edi, eax
        push   0
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_1_12FC
loc_2_76B6:
        add    esp, 8
loc_2_76B9:
        push   0
        push   esi
        call   sub_1_12FC
loc_2_76C1:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_76CB:
        push   ebp
        mov    ebp, esp
        push   0xa
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_764F
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   0x10
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_764F
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_76FF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 0x14]
        call   sub_2_741E
        add    esp, 4
        mov    esi, eax
        push   1
        push   0x80
        call   sub_1_12F4
loc_2_771F:
        add    esp, 8
        mov    dword ptr [0x3c], eax         ; -> obj3:0x0590
        cmp    eax, edi
        je     loc_2_7763
        push   0x80
        push   eax
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 0x18
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_2_7484
        add    esp, 4
        mov    edi, eax
        push   0
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_1_12FC
loc_2_7760:
        add    esp, 8
loc_2_7763:
        push   0
        push   esi
        call   sub_1_12FC
loc_2_776B:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_7774:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    dword ptr [ebp + 0x28], esi
        je     loc_2_783C
        push   1
        push   0x80
        call   sub_1_12F4
loc_2_7791:
        add    esp, 8
        mov    dword ptr [0x3c], eax         ; -> obj3:0x0590
        cmp    eax, esi
        je     loc_2_780B
        push   0x80
        push   eax
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 0x18
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_2_74AF
        add    esp, 4
        cmp    eax, esi
        je     loc_2_77FB
        mov    edi, 0x48                     ; -> obj3:0x059C
        mov    ebx, dword ptr [ebp + 0x1c]
loc_2_77D1:
        push   0x74                          ; -> obj3:0x05C8
        push   eax
        push   dword ptr [ebp + 0x20]
        push   ebx
        call   sub_2_7352
        add    esp, 0x10
        or     eax, eax
        je     loc_2_77F2
        mov    eax, dword ptr [0x74]         ; -> obj3:0x05C8
        mov    dword ptr [edi], eax
        add    edi, 4
        inc    esi
loc_2_77F2:
        call   sub_2_7530
        or     eax, eax
        jne    loc_2_77D1
loc_2_77FB:
        push   0
        push   dword ptr [0x3c]              ; -> obj3:0x0590
        call   sub_1_12FC
loc_2_7808:
        add    esp, 8
loc_2_780B:
        mov    dword ptr [esi*4 + 0x48], 0   ; -> obj3:0x059C
        or     esi, esi
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        jne    loc_2_782D
        push   dword ptr [ebp + 0x18]
        call   sub_2_753F
        add    esp, 0xc
        jmp    loc_2_783E
loc_2_782D:
        push   0x48                          ; -> obj3:0x059C
        call   sub_2_753F
        add    esp, 0xc
        jmp    loc_2_783E
loc_2_783C:
        sub    eax, eax
loc_2_783E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_7843:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x14]
        push   0x20                          ; -> obj3:0x0574
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7881:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0xa
        push   dword ptr [ebp + 0x14]
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        add    esp, 0x10
        mov    edi, eax
        push   0
        push   esi
        call   sub_1_12FC
loc_2_78B1:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x10
        push   dword ptr [ebp + 0x14]
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        add    esp, 0x10
        mov    edi, eax
        push   0
        push   esi
        call   sub_1_12FC
loc_2_78EA:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_78F3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [ebp + 0x14]
        call   sub_2_741E
        add    esp, 4
        mov    esi, eax
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        add    esp, 0x10
        mov    edi, eax
        push   0
        push   esi
        call   sub_1_12FC
loc_2_7921:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_792A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        push   0xa
        push   dword ptr [ebp + 0xc]
        call   sub_2_73B0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        je     loc_2_79DF
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   1
        lea    edx, [ecx - 1]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [edx + ecx - 1]
        inc    edx
        push   edx
        call   sub_1_12F4
loc_2_797A:
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_79D2
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_79D2:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_1_12FC
loc_2_79DC:
        add    esp, 8
loc_2_79DF:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_79E8:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_12FC
loc_2_79F5:
        add    esp, 8
        mov    eax, 1
        pop    ebp
        ret    
        db  000h
sub_2_7A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x10
        mov    edi, 0x6e0                    ; -> obj3:0x0CC4
        push   0x6e0                         ; -> obj3:0x0CC4
        push   dword ptr [ebp + 0x10]
        call   sub_2_6EE4
        add    esp, 0xc
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ecx - 1]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [eax + ecx*4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, 0x6e0                    ; -> obj3:0x0CC4
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0x700                    ; -> obj3:0x0CE4
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, 0x6e0                    ; -> obj3:0x0CC4
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0x6e0                    ; -> obj3:0x0CC4
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        sub    ecx, edx
        push   0x10
        lea    eax, [ecx + 0x6e0]            ; -> obj3:0x0CC4
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_2_6EE4
        add    esp, 0xc
        mov    eax, 0x6e0                    ; -> obj3:0x0CC4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_7ABA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   4
        push   0x670                         ; -> obj3:0x0C54
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_839E
        add    esp, 4
        push   dword ptr [eax*4 + 0x670]     ; -> obj3:0x0C54
        push   dword ptr [0x118]             ; -> obj3:0x06FC
        push   dword ptr [ebp + 8]
        push   esi
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_758B
        add    esp, 0x18
        pop    esi
        pop    ebp
        ret    
sub_2_7AFD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   0x670                         ; -> obj3:0x0C54
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [0x118]             ; -> obj3:0x06FC
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   4
        push   0x690                         ; -> obj3:0x0C74
        push   dword ptr [0x69c]             ; -> obj3:0x0C80
        push   dword ptr [0x11c]             ; -> obj3:0x0700
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_758B
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [0x11c]             ; -> obj3:0x0700
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_7268
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7B95:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x148]             ; -> obj3:0x072C
        call   sub_2_792A
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     loc_2_7BDF
        push   dword ptr [ebp + 0x14]
        push   esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   edi
        call   sub_2_76FF
        add    esp, 0x10
        mov    edi, eax
        push   esi
        call   sub_2_79E8
        add    esp, 4
loc_2_7BDF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_7BE5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x148]             ; -> obj3:0x072C
        call   sub_2_792A
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     loc_2_7C2F
        push   dword ptr [ebp + 0x14]
        push   esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   edi
        call   sub_2_78F3
        add    esp, 0x10
        mov    edi, eax
        push   esi
        call   sub_2_79E8
        add    esp, 4
loc_2_7C2F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_7C35:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x140]             ; -> obj3:0x0724
        call   sub_2_792A
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     loc_2_7C8D
        push   6
        push   0x6c8                         ; -> obj3:0x0CAC
        mov    eax, dword ptr [ebp + 0x14]
        push   dword ptr [eax*4 + 0x6c8]     ; -> obj3:0x0CAC
        push   esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   edi
        call   sub_2_758B
        add    esp, 0x18
        mov    edi, eax
        push   esi
        call   sub_2_79E8
        add    esp, 4
loc_2_7C8D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_7C93:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [0x14c]             ; -> obj3:0x0730
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7CBD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x14c]             ; -> obj3:0x0730
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7CE8:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [0x158]             ; -> obj3:0x073C
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_76FF
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7D12:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x158]             ; -> obj3:0x073C
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_78F3
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7D3D:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [0x150]             ; -> obj3:0x0734
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_76FF
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7D67:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x150]             ; -> obj3:0x0734
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_78F3
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7D92:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_83CC
        add    esp, 4
        push   eax
        push   dword ptr [0x124]             ; -> obj3:0x0708
        push   dword ptr [ebp + 8]
        push   esi
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   1
        call   sub_2_76CB
        add    esp, 0x10
        pop    esi
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x124]             ; -> obj3:0x0708
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   1
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7DF3:
        push   ebp
        mov    ebp, esp
        push   0x30
        push   dword ptr [0x154]             ; -> obj3:0x0738
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7E1D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x154]             ; -> obj3:0x0738
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7E48:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x3d
        push   0x528                         ; -> obj3:0x0B0C
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_8400
        add    esp, 4
        push   dword ptr [eax*4 + 0x528]     ; -> obj3:0x0B0C
        push   dword ptr [0x130]             ; -> obj3:0x0714
        push   dword ptr [ebp + 8]
        push   esi
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   1
        call   sub_2_758B
        add    esp, 0x18
        pop    esi
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [0x130]             ; -> obj3:0x0714
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7EC4:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x134]             ; -> obj3:0x0718
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   1
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7EEF:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [0x138]             ; -> obj3:0x071C
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7F19:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x138]             ; -> obj3:0x071C
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7F44:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_8370
        add    esp, 4
        push   eax
        push   dword ptr [0x13c]             ; -> obj3:0x0720
        push   dword ptr [ebp + 8]
        push   esi
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_76CB
        add    esp, 0x10
        pop    esi
        pop    ebp
        ret    
sub_2_7F7A:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [0x13c]             ; -> obj3:0x0720
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7FA5:
        push   ebp
        mov    ebp, esp
        push   1
        push   dword ptr [0x144]             ; -> obj3:0x0728
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   0x528                         ; -> obj3:0x0B0C
        call   sub_2_7A00
        add    esp, 0xc
        push   eax
        push   0
        call   sub_2_761B
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_7FD0:
        push   0
        push   dword ptr [0xa8]              ; -> obj3:0x0D94
        push   0                             ; -> obj3:0x0CEC
        push   1
        call   sub_2_761B
        add    esp, 0x10
        ret    
        db  06Ah, 001h, 0FFh, 035h
        dd  obj3:0x0DA0              ; fixup
        db  068h
        dd  obj3:0x0CEC              ; fixup
        db  06Ah, 001h, 0E8h, 01Fh, 0F6h, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h
sub_2_8000:
        push   0
        push   dword ptr [0xb8]              ; -> obj3:0x0DA4
        push   0                             ; -> obj3:0x0CEC
        push   1
        call   sub_2_761B
        add    esp, 0x10
        ret    
        push   ebp
        mov    ebp, esp
        call   sub_2_8000
        or     eax, eax
        je     loc_2_8040
        movzx  eax, word ptr [ebp + 8]
        push   eax
        push   dword ptr [0xbc]              ; -> obj3:0x0DA8
        push   0                             ; -> obj3:0x0CEC
        push   1
        call   sub_2_76CB
        add    esp, 0x10
        jmp    loc_2_8044
loc_2_8040:
        mov    ax, word ptr [ebp + 8]
loc_2_8044:
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        call   sub_2_8000
        or     eax, eax
        je     loc_2_806E
        movzx  eax, word ptr [ebp + 8]
        push   eax
        push   dword ptr [0xc0]              ; -> obj3:0x0DAC
        push   0                             ; -> obj3:0x0CEC
        push   1
        call   sub_2_76CB
        add    esp, 0x10
        jmp    loc_2_8072
loc_2_806E:
        mov    ax, word ptr [ebp + 8]
loc_2_8072:
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        call   sub_2_8000
        or     eax, eax
        je     loc_2_809C
        movzx  eax, word ptr [ebp + 8]
        push   eax
        push   dword ptr [0xc4]              ; -> obj3:0x0DB0
        push   0                             ; -> obj3:0x0CEC
        push   1
        call   sub_2_76CB
        add    esp, 0x10
        jmp    loc_2_80A0
loc_2_809C:
        mov    ax, word ptr [ebp + 8]
loc_2_80A0:
        pop    ebp
        ret    
sub_2_80A2:
        push   0
        push   dword ptr [0xc8]              ; -> obj3:0x0DB4
        push   0                             ; -> obj3:0x0CEC
        push   1
        call   sub_2_761B
        add    esp, 0x10
        ret    
        db  000h, 000h
sub_2_80BC:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   9
        push   0x160                         ; -> obj3:0x0F44
        push   0x88                          ; -> obj3:0x0E6C
        push   0xa0                          ; -> obj3:0x0E84
        push   dword ptr [0x7c]              ; -> obj3:0x0E60
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_7774
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_80EC:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   5
        push   0xe0                          ; -> obj3:0x0EC4
        push   0xf8                          ; -> obj3:0x0EDC
        push   0x108                         ; -> obj3:0x0EEC
        push   dword ptr [0x80]              ; -> obj3:0x0E64
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_7774
        mov    esp, ebp
        pop    ebp
        ret    
        db  06Ah, 001h, 0FFh, 035h
        dd  obj3:0x0E5C              ; fixup
        db  068h
        dd  obj3:0x0DE4              ; fixup
        db  06Ah, 001h, 0E8h, 0EBh, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h
sub_2_8134:
        push   9
        push   0x160                         ; -> obj3:0x0F44
        push   dword ptr [0x160]             ; -> obj3:0x0F44
        push   dword ptr [0x70]              ; -> obj3:0x0E54
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_758B
        add    esp, 0x18
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0x160                         ; -> obj3:0x0F44
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [0x70]              ; -> obj3:0x0E54
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8181:
        push   5
        push   0xe0                          ; -> obj3:0x0EC4
        push   dword ptr [0x160]             ; -> obj3:0x0F44
        push   dword ptr [0x74]              ; -> obj3:0x0E58
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_758B
        add    esp, 0x18
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0xe0                          ; -> obj3:0x0EC4
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [0x74]              ; -> obj3:0x0E58
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret    
        db  06Ah, 03Dh, 068h
        dd  obj3:0x0B0C              ; fixup
        db  0FFh, 035h
        dd  obj3:0x0B0C              ; fixup
        db  0FFh, 035h
        dd  obj3:0x0E68              ; fixup
        db  068h
        dd  obj3:0x0DE4              ; fixup
        db  06Ah, 001h, 0E8h, 09Eh, 0F3h, 0FFh, 0FFh, 083h, 0C4h, 018h, 0C3h
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0                             ; -> obj3:0x0B0C
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [0x84]              ; -> obj3:0x0E68
        push   0                             ; -> obj3:0x0DE4
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_821C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0xa
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        shr    eax, 8
        push   eax
        call   sub_2_73B0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0xa
        and    esi, 0xff
        push   esi
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   1
        push   5
        call   sub_1_12F4
loc_2_8257:
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_8285
        push   1
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_2_6E78
        add    esp, 0xc
        lea    edi, [ebx + 1]
        push   2
        mov    byte ptr [edi], 0x2e
        inc    edi
        push   esi
        push   edi
        call   sub_2_6E78
        add    esp, 0xc
        inc    edi
        inc    edi
        jmp    loc_2_8288
loc_2_8285:
        mov    edi, dword ptr [ebp - 8]
loc_2_8288:
        mov    byte ptr [edi], 0
        push   ebx
        push   dword ptr [0x38]              ; -> obj3:0x0FA0
        push   0                             ; -> obj3:0x0F68
        push   1
        call   sub_2_7268
        add    esp, 0x10
        mov    edi, eax
        push   0
        push   ebx
        call   sub_1_12FC
loc_2_82AB:
        add    esp, 8
        push   0
        push   dword ptr [ebp - 4]
        call   sub_1_12FC
loc_2_82B8:
        add    esp, 8
        push   0
        push   esi
        call   sub_1_12FC
loc_2_82C3:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_82CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        sub    bl, bl
        mov    byte ptr [ebp - 1], bl
        push   1
        push   0xa
        call   sub_1_12F4
loc_2_82ED:
        add    esp, 8
        mov    edi, eax
        mov    esi, edi
        or     edi, edi
        je     loc_2_8366
        push   0xa
        push   edi
        push   0x44                          ; -> obj3:0x0FAC
        push   dword ptr [0x38]              ; -> obj3:0x0FA0
        push   0                             ; -> obj3:0x0F68
        push   1
        call   sub_2_7218
        add    esp, 0x18
        cmp    byte ptr [edi], 0x2e
        je     loc_2_8334
loc_2_831A:
        mov    dl, byte ptr [esi]
        or     dl, dl
        je     loc_2_8334
        mov    cl, 0xa
        mov    al, byte ptr [ebp - 1]
        imul   cl
        lea    eax, [edx + eax - 0x30]
        inc    esi
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi], 0x2e
        jne    loc_2_831A
loc_2_8334:
        inc    esi
        cmp    byte ptr [esi], 0
        je     loc_2_834C
loc_2_833A:
        mov    cl, 0xa
        mov    al, bl
        imul   cl
        mov    cl, byte ptr [esi]
        inc    esi
        lea    ebx, [eax + ecx - 0x30]
        cmp    byte ptr [esi], 0
        jne    loc_2_833A
loc_2_834C:
        movzx  ecx, byte ptr [ebp - 1]
        shl    ecx, 8
        movzx  eax, bl
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        push   0
        push   edi
        call   sub_1_12FC
loc_2_8363:
        add    esp, 8
loc_2_8366:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8370:
        push   ebp
        mov    ebp, esp
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, dword ptr [0]            ; -> obj3:0x0FB4
        je     loc_2_8392
        mov    eax, 0                        ; -> obj3:0x0FB4
loc_2_8385:
        cmp    dword ptr [eax], 0
        je     loc_2_8392
        add    eax, 6
        inc    edx
        cmp    dword ptr [eax], ecx
        jne    loc_2_8385
loc_2_8392:
        imul   edx, edx, 6
        pop    ebp
        movzx  eax, byte ptr [edx + 4]       ; -> obj3:0x0FB8
        ret    
sub_2_839E:
        push   ebp
        mov    ebp, esp
        sub    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, dword ptr [0]            ; -> obj3:0x0FB4
        je     loc_2_83C0
        mov    edx, 0                        ; -> obj3:0x0FB4
loc_2_83B3:
        cmp    dword ptr [edx], 0
        je     loc_2_83C0
        add    edx, 6
        inc    ecx
        cmp    dword ptr [edx], eax
        jne    loc_2_83B3
loc_2_83C0:
        imul   ecx, ecx, 6
        pop    ebp
        movzx  eax, byte ptr [ecx + 5]       ; -> obj3:0x0FB9
        ret    
sub_2_83CC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 4
        je     loc_2_83F2
        cmp    eax, 0x18
        jl     loc_2_83EB
        cmp    eax, 0x19
        jle    loc_2_83F9
        cmp    eax, 0x22
        jl     loc_2_83EB
        cmp    eax, 0x23
        jle    loc_2_83F9
loc_2_83EB:
        mov    eax, 0x4b0
        jmp    loc_2_83FE
loc_2_83F2:
        mov    eax, 0x960
        jmp    loc_2_83FE
loc_2_83F9:
        mov    eax, 0x2580
loc_2_83FE:
        pop    ebp
        ret    
sub_2_8400:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        jne    loc_2_8410
        mov    eax, 0x13
loc_2_8410:
        pop    ebp
        ret    
        db  000h, 000h
sub_2_8414:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_12DC
loc_2_8425:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8429:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_12CC
loc_2_843A:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_843E:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   edx
        push   1
        push   0
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_12EC
loc_2_8463:
        add    esp, 0x18
        pop    edi
        pop    ebp
        ret    
sub_2_8469:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_12E4
loc_2_848D:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8491:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        sub    esi, esi
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], esi
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj3:0x1050
        push   dword ptr [ebp + 0x14]
        call   sub_2_8414
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_84E8
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_2_8414
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_84E8
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        call   sub_2_843E
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_84E8
        mov    esi, 1
loc_2_84E8:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_84F9
        push   dword ptr [ebp - 8]
        call   sub_1_12D4
loc_2_84F6:
        add    esp, 4
loc_2_84F9:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_850A
        push   dword ptr [ebp - 4]
        call   sub_1_12D4
loc_2_8507:
        add    esp, 4
loc_2_850A:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8511:
        push   ebp
        mov    ebp, esp
        push   0x80000002
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_8491
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_852B:
        push   ebp
        mov    ebp, esp
        push   0x80000001
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_8491
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8545:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        sub    edi, edi
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 0xc], edi
        mov    esi, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj3:0x1050
        push   dword ptr [ebp + 0x1c]
        call   sub_2_8429
        add    esp, 0xc
        cmp    eax, edi
        jne    loc_2_85AA
        lea    eax, [ebp - 0xc]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        call   sub_2_8429
        add    esp, 0xc
        cmp    eax, edi
        jne    loc_2_85AA
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0xc]
        call   sub_2_8469
        add    esp, 0x10
        cmp    eax, edi
        jne    loc_2_85AA
        mov    esi, dword ptr [ebp - 4]
        mov    edi, 1
loc_2_85AA:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_85BB
        push   dword ptr [ebp - 0xc]
        call   sub_1_12D4
loc_2_85B8:
        add    esp, 4
loc_2_85BB:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_85CC
        push   dword ptr [ebp - 8]
        call   sub_1_12D4
loc_2_85C9:
        add    esp, 4
loc_2_85CC:
        or     edi, edi
        jne    loc_2_85F1
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x14]
        call   sub_2_6E78
        add    esp, 0xc
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 4], ecx
loc_2_85F1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_85FA:
        push   ebp
        mov    ebp, esp
        push   0x80000001
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_8545
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_861A:
        push   ebp
        mov    ebp, esp
        push   0x80000002
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_8545
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_863A:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        jne    loc_2_8662
        cmp    dword ptr [0x2c], 0           ; -> obj3:0x107C
        je     loc_2_8662
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_2_85FA
        jmp    loc_2_8676
loc_2_8662:
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_2_861A
loc_2_8676:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_867A:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        jne    loc_2_869C
        cmp    dword ptr [0x2c], 0           ; -> obj3:0x107C
        je     loc_2_869C
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_2_852B
        jmp    loc_2_86AA
loc_2_869C:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_2_8511
loc_2_86AA:
        mov    esp, ebp
        pop    ebp
        ret    
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h, 0A1h
        dd  obj3:0x107C              ; fixup
        db  0C3h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x2c], eax         ; -> obj3:0x107C
        pop    ebp
        ret    
        db  000h
sub_2_86C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_2_0798
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_2_873F
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_2_6E78
        add    esp, 0xc
        mov    word ptr [ebp - 8], 0x4300
        mov    eax, esi
        and    ax, 0xf
        mov    word ptr [ebp - 0x10], ax
        shr    esi, 4
        mov    word ptr [ebp - 4], si
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   eax
        push   0x21
        call   sub_1_0645
loc_2_8735:
        add    esp, 0x10
        mov    ebx, eax
        call   sub_2_07AB
loc_2_873F:
        and    ebx, 1
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        cmp    dword ptr [8], 0              ; -> obj3:0x1088
        jne    loc_2_890E
        call   sub_2_03D1
        or     eax, eax
        je     loc_2_890E
        mov    esi, 0                        ; -> obj3:0x1080
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        jb     loc_2_890E
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        je     loc_2_87AF
loc_2_8799:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    loc_2_8799
loc_2_87AF:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [ecx - 1]
        or     esi, esi
        je     loc_2_890E
        push   1
        lea    eax, [esi + 2]
        push   eax
        call   sub_1_12F4
loc_2_87D3:
        add    esp, 8
        mov    dword ptr [8], eax            ; -> obj3:0x1088
        or     eax, eax
        je     loc_2_890E
        inc    esi
        push   esi
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_6E78
        add    esp, 0xc
        mov    edi, dword ptr [8]            ; -> obj3:0x1088
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [8]            ; -> obj3:0x1088
        cmp    byte ptr [ecx + eax - 2], 0x3b
        jne    loc_2_8825
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [8]            ; -> obj3:0x1088
        mov    byte ptr [ecx + eax - 2], 0
loc_2_8825:
        mov    edi, dword ptr [8]            ; -> obj3:0x1088
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [8]            ; -> obj3:0x1088
        cmp    byte ptr [ecx + eax - 2], 0x5c
        jne    loc_2_8859
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [8]            ; -> obj3:0x1088
        mov    byte ptr [ecx + eax - 2], 0
loc_2_8859:
        push   dword ptr [8]                 ; -> obj3:0x1088
        call   sub_2_86C8
        add    esp, 4
        or     eax, eax
        je     loc_2_889D
        mov    edi, dword ptr [8]            ; -> obj3:0x1088
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [8]            ; -> obj3:0x1088
        cmp    byte ptr [ecx + eax - 2], 0x5c
        je     loc_2_88F7
        mov    esi, 0xc                      ; -> obj3:0x108C
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_2_88F7
loc_2_889D:
        push   0
        push   dword ptr [8]                 ; -> obj3:0x1088
        call   sub_1_12FC
loc_2_88AA:
        add    esp, 8
        mov    dword ptr [8], 0              ; -> obj3:0x1088
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    dword ptr [ebp - 4], edx
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [ecx - 1]
        or     esi, esi
        je     loc_2_88F7
        inc    esi
        push   1
        push   esi
        call   sub_1_12F4
loc_2_88DE:
        add    esp, 8
        mov    dword ptr [8], eax            ; -> obj3:0x1088
        or     eax, eax
        je     loc_2_88F7
        push   esi
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_6E78
        add    esp, 0xc
loc_2_88F7:
        cmp    dword ptr [8], 0              ; -> obj3:0x1088
        je     loc_2_890E
        push   dword ptr [8]                 ; -> obj3:0x1088
        call   sub_2_7058
        add    esp, 4
loc_2_890E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0A1h
        dd  obj3:0x1088              ; fixup
        db  00Bh, 0C0h, 074h, 015h, 06Ah, 000h, 050h, 0E8h
        dd  sub_1_12FC               ; fixup
        db  083h, 0C4h, 008h, 0C7h, 005h
        dd  obj3:0x1088              ; fixup
        db  000h, 000h, 000h, 000h, 0C3h
sub_2_8933:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [8]            ; -> obj3:0x1088
        or     edx, edx
        je     loc_2_897E
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        je     loc_2_897E
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    eax, dword ptr [ebp + 0xc]
        cmp    ecx, eax
        jae    loc_2_8977
        push   eax
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_2_6E78
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_2_8983
loc_2_8977:
        mov    eax, 0xfffffff8
        jmp    loc_2_8983
loc_2_897E:
        mov    eax, 0xffffffeb
loc_2_8983:
        pop    edi
        pop    ebp
        ret    
        db  000h, 000h
sub_2_8988:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        push   dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_6E78
        add    esp, 0xc
        mov    byte ptr [esi + edi - 1], 0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_89AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_2_0798
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_8A2F
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_2_8988
        add    esp, 0xc
        mov    byte ptr [ebp - 7], 0x3d
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [ebp - 8], al
        mov    eax, ebx
        and    ax, 0xf
        mov    word ptr [ebp - 0x10], ax
        shr    ebx, 4
        mov    word ptr [ebp - 4], bx
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   eax
        push   0x21
        call   sub_1_0645
loc_2_8A1C:
        add    esp, 0x10
        mov    esi, eax
        mov    eax, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        call   sub_2_07AB
loc_2_8A2F:
        test   esi, 1
        mov    eax, 0
        je     loc_2_8A40
        movzx  eax, word ptr [ebp - 8]
loc_2_8A40:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8A47:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        mov    byte ptr [ebp - 3], 0x3e
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x10], ax
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        push   0x21
        call   sub_1_05E4
loc_2_8A64:
        add    esp, 0xc
        test   al, 1
        mov    eax, 0
        je     loc_2_8A74
        movzx  eax, word ptr [ebp - 4]
loc_2_8A74:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8A78:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_0798
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_8AEA
        mov    byte ptr [ebp - 7], 0x3f
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x14], ax
        mov    word ptr [ebp - 0xc], si
        mov    eax, ebx
        and    ax, 0xf
        mov    word ptr [ebp - 0x10], ax
        mov    eax, ebx
        shr    eax, 4
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   eax
        push   0x21
        call   sub_1_0645
loc_2_8ACA:
        add    esp, 0x10
        mov    edi, eax
        mov    eax, dword ptr [ebp + 0x14]
        movzx  ecx, word ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        push   esi
        push   ebx
        push   dword ptr [ebp + 0xc]
        call   sub_2_8988
        add    esp, 0xc
        call   sub_2_07AB
loc_2_8AEA:
        test   edi, 1
        mov    eax, 0
        je     loc_2_8AFB
        movzx  eax, word ptr [ebp - 8]
loc_2_8AFB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8B02:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        mov    byte ptr [ebp - 3], 0x42
        mov    al, byte ptr [ebp + 0x10]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, eax
        shr    ecx, 0x10
        mov    word ptr [ebp - 8], cx
        mov    word ptr [ebp - 0xc], ax
        lea    eax, [ebp - 0x20]
        push   eax
        push   eax
        push   0x21
        call   sub_1_05E4
loc_2_8B35:
        add    esp, 0xc
        test   al, 1
        mov    eax, 0xffffffff
        jne    loc_2_8B4E
        movzx  eax, word ptr [ebp - 0xc]
        shl    eax, 0x10
        movzx  ecx, word ptr [ebp - 4]
        or     eax, ecx
loc_2_8B4E:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8B52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        push   1
        push   ebx
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        call   sub_2_8B02
        add    esp, 0xc
        cmp    eax, ebx
        jne    loc_2_8B76
        mov    dword ptr [0x8c], ebx         ; -> obj3:0x1130
loc_2_8B76:
        push   0
        push   dword ptr [0x8c]              ; -> obj3:0x1130
        push   edi
        call   sub_2_8B02
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_8BFE
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   edi
        call   sub_2_8A78
        add    esp, 0x10
        or     eax, eax
        jne    loc_2_8BFE
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_8BFE
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [esi + eax], 0
        push   0xa
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     loc_2_8BC5
        mov    byte ptr [eax], 0
loc_2_8BC5:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    dword ptr [0x8c], ecx         ; -> obj3:0x1130
        push   0xd
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     loc_2_8BEA
        mov    byte ptr [eax], 0
loc_2_8BEA:
        push   0x1a
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     loc_2_8BFC
        mov    byte ptr [eax], 0
loc_2_8BFC:
        mov    ebx, esi
loc_2_8BFE:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_8C07:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, esi
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        je     loc_2_8C34
loc_2_8C22:
        movsx  eax, byte ptr [ebx + 1]
        inc    ebx
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    loc_2_8C22
loc_2_8C34:
        cmp    byte ptr [esi], 0
        je     loc_2_8C55
        cmp    ebx, esi
        je     loc_2_8C55
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   ebx
        push   esi
        call   sub_2_8988
        add    esp, 0xc
loc_2_8C55:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        je     loc_2_8C9C
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edi, [esi + ecx - 2]
        movsx  eax, byte ptr [edi]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        je     loc_2_8C98
loc_2_8C86:
        movsx  eax, byte ptr [edi - 1]
        dec    edi
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    loc_2_8C86
loc_2_8C98:
        mov    byte ptr [edi + 1], 0
loc_2_8C9C:
        movsx  eax, byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_8CA4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_8C07
        add    esp, 4
        push   0x3b
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     loc_2_8CC6
        mov    byte ptr [eax], 0
loc_2_8CC6:
        push   esi
        call   sub_2_8C07
        add    esp, 4
        movsx  eax, byte ptr [esi]
        pop    esi
        pop    ebp
        ret    
sub_2_8CD5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_2_8CDF:
        push   esi
        push   0x80
        push   dword ptr [0]                 ; -> obj3:0x10A4
        call   sub_2_8B52
        add    esp, 0xc
        or     eax, eax
        mov    eax, dword ptr [0]            ; -> obj3:0x10A4
        je     loc_2_8D0E
        push   eax
        call   sub_2_8CA4
        add    esp, 4
        or     eax, eax
        je     loc_2_8CDF
        mov    edi, 1
loc_2_8D0E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_8D14:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x10A4
        sub    esi, esi
        cmp    byte ptr [eax], 0x5b
        jne    loc_2_8D6E
        push   0x5d
        push   eax
        call   sub_2_7000
        add    esp, 8
        cmp    eax, esi
        je     loc_2_8D6E
        mov    esi, 1
        push   0x28
        mov    eax, dword ptr [0]            ; -> obj3:0x10A4
        add    eax, esi
        push   eax
        push   dword ptr [4]                 ; -> obj3:0x10A8
        call   sub_2_8988
        add    esp, 0xc
        push   0x5d
        push   dword ptr [4]                 ; -> obj3:0x10A8
        call   sub_2_7000
        add    esp, 8
        mov    byte ptr [eax], 0
        push   dword ptr [4]                 ; -> obj3:0x10A8
        call   sub_2_8C07
        add    esp, 4
loc_2_8D6E:
        mov    eax, esi
        pop    esi
        ret    
sub_2_8D72:
        push   esi
        push   edi
        sub    esi, esi
        push   0x3d
        push   dword ptr [0]                 ; -> obj3:0x10A4
        call   sub_2_7000
        add    esp, 8
        cmp    eax, esi
        je     loc_2_8E24
        push   0x28
        push   0x3d
        push   dword ptr [0]                 ; -> obj3:0x10A4
        call   sub_2_7000
        add    esp, 8
        inc    eax
        push   eax
        push   dword ptr [0xc]               ; -> obj3:0x10B0
        call   sub_2_8988
        add    esp, 0xc
        push   dword ptr [0xc]               ; -> obj3:0x10B0
        call   sub_2_8C07
        add    esp, 4
        push   0x3d
        push   dword ptr [0]                 ; -> obj3:0x10A4
        call   sub_2_7000
        add    esp, 8
        mov    byte ptr [eax], 0
        push   0x28
        push   dword ptr [0]                 ; -> obj3:0x10A4
        push   dword ptr [8]                 ; -> obj3:0x10AC
        call   sub_2_8988
        add    esp, 0xc
        push   dword ptr [8]                 ; -> obj3:0x10AC
        call   sub_2_8C07
        add    esp, 4
        mov    edi, dword ptr [8]            ; -> obj3:0x10AC
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        je     loc_2_8E22
        mov    edi, dword ptr [0xc]          ; -> obj3:0x10B0
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    esi, 1
        jne    loc_2_8E24
loc_2_8E22:
        sub    esi, esi
loc_2_8E24:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
sub_2_8E29:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    ebx, 0x28                     ; -> obj3:0x10CC
        mov    esi, dword ptr [ebp + 8]
loc_2_8E39:
        push   dword ptr [ebx]
        push   esi
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        je     loc_2_8E55
        add    ebx, 4
        cmp    ebx, 0x68                     ; -> obj3:0x110C
        jb     loc_2_8E39
        jmp    loc_2_8E5A
loc_2_8E55:
        mov    edi, 1
loc_2_8E5A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_8E61:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    ebx, 0x80                     ; -> obj3:0x1124
        mov    esi, dword ptr [ebp + 8]
loc_2_8E74:
        push   dword ptr [ebx]
        push   esi
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        je     loc_2_8E90
        add    ebx, 4
        cmp    ebx, 0x8c                     ; -> obj3:0x1130
        jb     loc_2_8E74
        jmp    loc_2_8E92
loc_2_8E90:
        sub    edi, edi
loc_2_8E92:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_8E99:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x5c
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        jne    loc_2_8EE1
        mov    edi, 0x10                     ; -> obj3:0x10B4
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_8EE1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_8EE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp - 8]
loc_2_8EF7:
        or     ebx, ebx
        je     loc_2_8F1E
        cmp    ebx, 1
        je     loc_2_8F5D
        cmp    ebx, 2
        je     loc_2_8FB1
        push   dword ptr [ebp - 4]
        call   sub_2_8A47
        add    esp, 4
        mov    ebx, 4
        jmp    loc_2_9027
loc_2_8F1E:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_2_89AB
        add    esp, 0xc
        or     eax, eax
        je     loc_2_8F40
        mov    edi, 2
        mov    ebx, 4
        jmp    loc_2_9027
loc_2_8F40:
        push   dword ptr [ebp - 4]
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        je     loc_2_9022
        mov    ebx, 1
        jmp    loc_2_9027
loc_2_8F5D:
        call   sub_2_8D14
        or     eax, eax
        jne    loc_2_8F7A
        push   dword ptr [ebp - 4]
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        jne    loc_2_8F5D
        mov    ebx, 3
loc_2_8F7A:
        push   dword ptr [4]                 ; -> obj3:0x10A8
        call   sub_2_8E29
        add    esp, 4
        or     eax, eax
        push   dword ptr [ebp - 4]
        je     loc_2_8F9C
        call   sub_2_8CD5
        add    esp, 4
        jmp    loc_2_9027
loc_2_8F9C:
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        jne    loc_2_8FAA
        jmp    loc_2_9022
loc_2_8FAA:
        mov    ebx, 2
        jmp    loc_2_9027
loc_2_8FB1:
        call   sub_2_8D14
        or     eax, eax
        jne    loc_2_9039
        call   sub_2_8D72
        or     eax, eax
        mov    eax, dword ptr [8]            ; -> obj3:0x10AC
        je     loc_2_9013
        push   eax
        call   sub_2_8E29
        add    esp, 4
        or     eax, eax
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
        je     loc_2_8FDC
        jmp    loc_2_9013
loc_2_8FDC:
        push   eax
        call   sub_2_8E61
        add    esp, 4
        or     eax, eax
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
        je     loc_2_8FFC
        push   eax
        call   sub_2_8E99
        add    esp, 4
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
loc_2_8FFC:
        push   dword ptr [0xc]               ; -> obj3:0x10B0
        push   dword ptr [8]                 ; -> obj3:0x10AC
        push   eax
        push   1
        call   sub_2_867A
        add    esp, 0x10
loc_2_9013:
        push   dword ptr [ebp - 4]
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        jne    loc_2_8FB1
loc_2_9022:
        mov    ebx, 3
loc_2_9027:
        cmp    ebx, 4
        jne    loc_2_8EF7
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9039:
        mov    ebx, 1
        jmp    loc_2_9027
sub_2_9040:
        push   ebx
        push   esi
        push   edi
        call   sub_2_03D1
        or     eax, eax
        je     loc_2_9172
        push   1
        push   0x80
        call   sub_1_12F4
loc_2_905C:
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj3:0x10A4
        push   1
        push   0x28
        call   sub_1_12F4
loc_2_906D:
        add    esp, 8
        mov    dword ptr [4], eax            ; -> obj3:0x10A8
        push   1
        push   0x28
        call   sub_1_12F4
loc_2_907E:
        add    esp, 8
        mov    dword ptr [8], eax            ; -> obj3:0x10AC
        push   1
        push   0x28
        call   sub_1_12F4
loc_2_908F:
        add    esp, 8
        mov    dword ptr [0xc], eax          ; -> obj3:0x10B0
        mov    edx, dword ptr [0]            ; -> obj3:0x10A4
        or     edx, edx
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
        je     loc_2_911D
        or     eax, eax
        je     loc_2_911D
        cmp    dword ptr [8], 0              ; -> obj3:0x10AC
        je     loc_2_911D
        cmp    dword ptr [0xc], 0            ; -> obj3:0x10B0
        je     loc_2_911D
        push   0x80
        push   edx
        call   sub_2_8933
        add    esp, 8
        or     eax, eax
        mov    edx, dword ptr [0]            ; -> obj3:0x10A4
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
        jl     loc_2_911D
        mov    edi, 0x18                     ; -> obj3:0x10BC
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   edx
        call   sub_2_8EE6
        add    esp, 4
        mov    edx, dword ptr [0]            ; -> obj3:0x10A4
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
loc_2_911D:
        or     edx, edx
        je     loc_2_9131
        push   0
        push   edx
        call   sub_1_12FC
loc_2_9129:
        add    esp, 8
        mov    eax, dword ptr [4]            ; -> obj3:0x10A8
loc_2_9131:
        or     eax, eax
        je     loc_2_9140
        push   0
        push   eax
        call   sub_1_12FC
loc_2_913D:
        add    esp, 8
loc_2_9140:
        cmp    dword ptr [8], 0              ; -> obj3:0x10AC
        je     loc_2_9159
        push   0
        push   dword ptr [8]                 ; -> obj3:0x10AC
        call   sub_1_12FC
loc_2_9156:
        add    esp, 8
loc_2_9159:
        cmp    dword ptr [0xc], 0            ; -> obj3:0x10B0
        je     loc_2_9172
        push   0
        push   dword ptr [0xc]               ; -> obj3:0x10B0
        call   sub_1_12FC
loc_2_916F:
        add    esp, 8
loc_2_9172:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h
sub_2_9178:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_12CC
loc_2_9189:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_918D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_12E4
loc_2_91B1:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_91B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    dword ptr [ebp - 8], ebx
        mov    esi, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x1c]
        call   sub_2_9178
        add    esp, 0xc
        cmp    eax, ebx
        jne    loc_2_9203
        lea    eax, [ebp - 4]
        push   eax
        mov    edi, dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        call   sub_2_918D
        add    esp, 0x10
        cmp    eax, ebx
        jne    loc_2_9206
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, 1
        jmp    loc_2_9206
loc_2_9203:
        mov    edi, dword ptr [ebp + 0x14]
loc_2_9206:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_9217
        push   dword ptr [ebp - 8]
        call   sub_1_12D4
loc_2_9214:
        add    esp, 4
loc_2_9217:
        or     ebx, ebx
        jne    loc_2_9237
        push   esi
        push   dword ptr [ebp + 0x10]
        push   edi
        call   sub_2_6E78
        add    esp, 0xc
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 4], ecx
loc_2_9237:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9241:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        jne    loc_2_9262
        push   0x80000003
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   0                             ; -> obj3:0x1134
        jmp    loc_2_9279
loc_2_9262:
        push   0x80000001
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [0x20]              ; -> obj3:0x1154
loc_2_9279:
        call   sub_2_91B5
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9282:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, 0xffffffff
        push   1
        push   0x20
        call   sub_1_12F4
loc_2_9295:
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_2_92CC
        push   0x20
        push   esi
        push   0x24                          ; -> obj3:0x1158
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_9241
        add    esp, 0x14
        push   0xa
        push   esi
        call   sub_2_71AC
        add    esp, 8
        mov    edi, eax
        push   0
        push   esi
        call   sub_1_12FC
loc_2_92C9:
        add    esp, 8
loc_2_92CC:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_92D2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x28                          ; -> obj3:0x115C
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9282
        add    esp, 8
        or     eax, eax
        jne    loc_2_9302
        push   0x34                          ; -> obj3:0x1168
        push   esi
        call   sub_2_9282
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     loc_2_9307
loc_2_9302:
        mov    eax, 1
loc_2_9307:
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h
loc_2_930C:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, edx
        je     loc_2_932E
        mov    ecx, dword ptr [eax + 0x2c]
        cmp    dword ptr [edx + 0x2c], ecx
        jne    loc_2_932E
        mov    eax, dword ptr [eax + 0x34]
        cmp    dword ptr [edx + 0x34], eax
        mov    eax, 1
        je     loc_2_9330
loc_2_932E:
        sub    eax, eax
loc_2_9330:
        pop    ebp
        ret    
sub_2_9332:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [esi + 0x34], 0
        push   0                             ; -> loc_2_930C
        call   sub_2_4594
        add    esp, 8
        or     eax, eax
        je     loc_2_936B
loc_2_9352:
        mov    eax, dword ptr [esi + 0x34]
        inc    eax
        mov    dword ptr [esi + 0x34], eax
        push   esi
        push   0                             ; -> loc_2_930C
        call   sub_2_4594
        add    esp, 8
        or     eax, eax
        jne    loc_2_9352
loc_2_936B:
        mov    eax, dword ptr [esi + 0x34]
        pop    esi
        pop    ebp
        ret    
sub_2_9371:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9332
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   eax
        call   sub_2_7E48
        add    esp, 8
        mov    dword ptr [ebp - 0x10], eax
        push   eax
        push   esi
        call   sub_2_A58E
        add    esp, 8
        push   edi
        push   esi
        call   sub_2_95DC
        add    esp, 8
        push   edi
        push   esi
        call   sub_2_95E4
        add    esp, 8
        mov    dword ptr [ebp - 4], 0
        mov    ebx, 4
loc_2_93C3:
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0x10]
        call   sub_2_96D5
        add    esp, 8
        movzx  eax, ax
        imul   eax, eax, 0x41
        add    eax, 0                        ; -> obj3:0x1560
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_9E2E
        add    esp, 0xc
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], ebx
        jl     loc_2_93C3
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7ABA
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_66BF
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7EC4
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        lea    eax, [esi + 0x74]
        push   eax
        call   sub_2_A0AC
        add    esp, 4
        or     eax, eax
        jne    loc_2_9442
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_A57E
        add    esp, 8
loc_2_9442:
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    ebx, 1
        cmp    dword ptr [ebp - 0xc], ebx
        jb     loc_2_9484
loc_2_9459:
        push   dword ptr [ebp - 0xc]
        push   ebx
        call   sub_2_A035
        add    esp, 4
        push   eax
        push   ebx
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7B95
        add    esp, 0x14
        push   eax
        push   ebx
        push   esi
        call   sub_2_A494
        add    esp, 0xc
        inc    ebx
        cmp    ebx, dword ptr [ebp - 0xc]
        jbe    loc_2_9459
loc_2_9484:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_2_94A0
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7D92
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_6A78
        add    esp, 8
loc_2_94A0:
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7EEF
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_6B7C
        add    esp, 8
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7F44
        add    esp, 8
        push   eax
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7F44
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_672F
        add    esp, 0xc
        push   dword ptr [ebp - 0x10]
        call   sub_2_9645
        add    esp, 4
        or     eax, eax
        je     loc_2_9532
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    ebx, 1
        cmp    dword ptr [ebp - 0xc], ebx
        jb     loc_2_9532
loc_2_94FF:
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   dword ptr [ebp - 0x10]
        call   sub_2_9612
        add    esp, 0xc
        push   dword ptr [ebp - 4]
        push   ebx
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7C35
        add    esp, 0x10
        push   eax
        push   ebx
        push   dword ptr [ebp - 0x10]
        call   sub_2_962D
        add    esp, 0xc
        inc    ebx
        cmp    ebx, dword ptr [ebp - 0xc]
        jbe    loc_2_94FF
loc_2_9532:
        push   dword ptr [ebp - 0x10]
        call   sub_2_9684
        add    esp, 4
        or     eax, eax
        je     loc_2_9557
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7FA5
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_A59E
        add    esp, 8
loc_2_9557:
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        cmp    dword ptr [ebp - 0xc], 2
        jne    loc_2_9580
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7C93
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_6CD9
        add    esp, 8
loc_2_9580:
        push   1
        push   dword ptr [ebp - 0xc]
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7D3D
        add    esp, 0xc
        push   eax
        push   esi
        call   sub_2_A4E4
        add    esp, 0xc
        push   1
        push   dword ptr [ebp - 0xc]
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7CE8
        add    esp, 0xc
        push   eax
        push   esi
        call   sub_2_A531
        add    esp, 0xc
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7DF3
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   1
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_95DC:
        ret    
sub_2_95DD:
        jmp    loc_2_AB20
        db  000h, 000h
sub_2_95E4:
        ret    
        db  000h, 000h, 000h
sub_2_95E8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        lea    ecx, [eax + 0x10]             ; -> obj3:0x118C
        mov    eax, dword ptr [ebp + 0xc]
        mov    dl, byte ptr [ecx]
        and    edx, 3
        mov    dword ptr [eax], edx
        mov    al, byte ptr [ecx]
        and    al, 0xc
        shr    al, 2
        movzx  ecx, al
        mov    eax, dword ptr [ebp + 0x10]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    
sub_2_9612:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        imul   ecx, ecx, 9
        mov    eax, dword ptr [ebp + 0xc]
        movzx  ecx, byte ptr [ecx + eax + 0x10] ; -> obj3:0x118C
        mov    eax, dword ptr [ebp + 0x10]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    
sub_2_962D:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        imul   ecx, ecx, 9
        mov    dl, byte ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + eax + 0x10], dl ; -> obj3:0x118C
        pop    ebp
        ret    
sub_2_9645:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        mov    ax, word ptr [eax + 0x17]     ; -> obj3:0x1193
        and    eax, 8
        ret    
sub_2_965A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        mov    ax, word ptr [eax + 0x17]     ; -> obj3:0x1193
        and    eax, 1
        ret    
sub_2_966F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        mov    ax, word ptr [eax + 0x17]     ; -> obj3:0x1193
        and    eax, 2
        ret    
sub_2_9684:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        mov    ax, word ptr [eax + 0x17]     ; -> obj3:0x1193
        and    eax, 0x20
        ret    
sub_2_9699:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        mov    ax, word ptr [eax + 0x17]     ; -> obj3:0x1193
        and    eax, 0x10
        ret    
sub_2_96AE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        mov    ax, word ptr [eax + 0x17]     ; -> obj3:0x1193
        and    eax, 0x40
        ret    
sub_2_96C3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        movzx  eax, byte ptr [eax + 0x16]    ; -> obj3:0x1192
        ret    
sub_2_96D5:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        movzx  ecx, byte ptr [eax + 0x15]    ; -> obj3:0x1191
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        movzx  ax, byte ptr [eax + ecx*4]    ; -> obj3:0x117C
        ret    
        db  000h
sub_2_96F4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x4c], 0
        ret    
sub_2_9703:
        push   ebp
        mov    ebp, esp
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_1_12F4
loc_2_9710:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9714:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 0xc]
        call   sub_1_12FC
loc_2_9721:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9725:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_96F4
        add    esp, 4
        push   8
        push   1
        call   sub_2_9703
        add    esp, 8
        mov    dword ptr [esi + 0xc], eax
        push   0x30
        push   1
        call   sub_2_9703
        add    esp, 8
        or     eax, eax
        je     loc_2_97B1
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [esi + 0x24], eax
        mov    ebx, 8                        ; -> loc_1_140C
        sub    edi, edi
loc_2_9771:
        mov    ecx, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 4], ecx
        push   dword ptr [ebx]
        push   dword ptr [ebx - 4]
        call   sub_2_9703
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + ecx], eax
        mov    eax, dword ptr [esi + 0x24]
        cmp    dword ptr [edi + eax], 0
        je     loc_2_97AA
        push   dword ptr [eax + edi]
        push   esi
        call   dword ptr [ebx + 4]
        add    esp, 8
        add    ebx, 0x20
        add    edi, 4
        cmp    edi, 0x30
        jb     loc_2_9771
        jmp    loc_2_97B1
loc_2_97AA:
        mov    dword ptr [ebp - 8], 0
loc_2_97B1:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_97BB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0x10                     ; -> loc_1_1414
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_2_97CB:
        mov    eax, dword ptr [esi + 0x24]
        push   dword ptr [edi + eax]
        push   esi
        call   dword ptr [ebx]
        add    esp, 8
        mov    eax, dword ptr [esi + 0x24]
        push   dword ptr [edi + eax]
        push   dword ptr [ebx - 0xc]
        call   sub_2_9714
        add    esp, 8
        add    ebx, 0x20
        add    edi, 4
        cmp    edi, 0x30
        jb     loc_2_97CB
        push   dword ptr [esi + 0x24]
        push   1
        call   sub_2_9714
        add    esp, 8
        mov    dword ptr [esi + 0x24], 0
        push   dword ptr [esi + 0xc]
        push   1
        call   sub_2_9714
        add    esp, 8
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_9825:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        push   dword ptr [ecx + eax*4]
        shl    eax, 5
        call   sub_1_141C
        add    esp, 8
        sub    eax, eax
        pop    ebp
        ret    
sub_2_9847:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        push   dword ptr [ecx + eax*4]
        shl    eax, 5
        call   sub_1_1418
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp + 0xc]
        push   dword ptr [eax + ecx*4]
        shl    ecx, 5
        push   dword ptr [ebp + 0x10]
        call   sub_1_1420
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9885:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        je     loc_2_98A5
        and    dword ptr [0x44], 0xfffffffb  ; -> loc_1_1448
        and    dword ptr [0x124], 0xfffffffb ; -> loc_1_1528
        and    dword ptr [0x104], 0xfffffffb ; -> loc_1_1508
        jmp    loc_2_98BA
loc_2_98A5:
        or     dword ptr [0x44], 4           ; -> loc_1_1448
        or     dword ptr [0x124], 4          ; -> loc_1_1528
        or     dword ptr [0x104], 4          ; -> loc_1_1508
loc_2_98BA:
        pop    ebp
        ret    
loc_2_98BC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + ecx*4 - 4]
        ret    
loc_2_98CD:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx]
        lea    ecx, [ecx + eax*4 - 4]
        mov    edx, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ecx], edx
        pop    ebp
        ret    
loc_2_98E5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 0x20
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_98FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [ebp + 8]
loc_2_990B:
        cmp    edi, 3
        ja     loc_2_9924
        push   edi
        call   sub_2_A035
        add    esp, 4
        mov    dword ptr [ebp - 0x10], edi
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        jmp    loc_2_9937
loc_2_9924:
        push   1
        call   sub_2_A035
        add    esp, 4
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
loc_2_9937:
        push   eax
        push   2
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        inc    edi
        cmp    edi, 0x20
        jbe    loc_2_990B
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9954:
        ret    
        db  000h, 000h, 000h
sub_2_9958:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        cmp    edx, 0xb4
        jle    loc_2_996F
        mov    eax, 0x168
        sub    eax, edx
        mov    edx, eax
loc_2_996F:
        cmp    edx, 0x5a
        jle    loc_2_9989
        mov    eax, 0x168                    ; -> obj3:0x151C
        lea    ecx, [edx*2]
        sub    eax, ecx
        movzx  eax, word ptr [eax]
        neg    eax
        jmp    loc_2_9991
loc_2_9989:
        movzx  eax, word ptr [edx*2]         ; -> obj3:0x13B4
loc_2_9991:
        or     edx, edx
        jne    loc_2_9998
        inc    eax
        jmp    loc_2_99A1
loc_2_9998:
        cmp    edx, 0xb4
        jne    loc_2_99A1
        dec    eax
loc_2_99A1:
        pop    ebp
        ret    
loc_2_99A3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret    
loc_2_99AD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 0x168
        mov    eax, dword ptr [eax]
        cdq    
        idiv   ecx
        mov    edi, edx
        or     edi, edi
        jge    loc_2_99C7
        add    edi, ecx
loc_2_99C7:
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi], edi
        cli    
        push   edi
        call   sub_2_9958
        add    esp, 4
        mov    dword ptr [esi + 4], eax
        sub    edi, 0x5a
        jns    loc_2_99E4
        add    edi, 0x168
loc_2_99E4:
        push   edi
        call   sub_2_9958
        add    esp, 4
        mov    dword ptr [esi + 8], eax
        sti    
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_99F7:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_9A0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   6
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9A31:
        ret    
        db  000h, 000h
loc_2_9A34:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [esi]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    
loc_2_9A52:
        push   ebp
        mov    ebp, esp
        cli    
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 0xc], eax
        sti    
        sub    eax, eax
        pop    ebp
        ret    
loc_2_9A6C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_9A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 1
        push   eax
        push   4
        push   ecx
        call   sub_2_9825
        add    esp, 0xc
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9AB3:
        ret    
sub_2_9AB4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 0x18]
        mov    esi, dword ptr [ebp + 0x14]
        sub    edx, esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, ecx
        imul   edx, eax
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 8]
        xor    edx, edx
        div    dword ptr [ebp - 4]
        add    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9AE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x30]
        call   sub_2_96C3
        add    esp, 4
        mov    eax, dword ptr [eax*4]        ; -> obj3:0x146C
        mov    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp - 2]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        push   eax
        push   0x64
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_2_9AB4
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_9B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x30]
        call   sub_2_96C3
        add    esp, 4
        mov    eax, dword ptr [eax*4]        ; -> obj3:0x146C
        mov    dword ptr [ebp - 4], eax
        push   0x64
        push   1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0xc]
        call   sub_2_9AB4
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9B58:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_2_9B20
        mov    edi, dword ptr [ebp + 0xc]
        add    esp, 8
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx], eax
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi]
        call   sub_2_9B20
        add    esp, 8
        mov    ecx, dword ptr [edi + 4]
        pop    edi
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    
loc_2_9B8C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli    
        mov    edi, dword ptr [ebp + 0xc]
        push   dword ptr [edi]
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi]
        call   sub_2_9AE8
        add    esp, 8
        mov    dword ptr [esi + 4], eax
        push   dword ptr [edi + 4]
        push   dword ptr [esi]
        call   sub_2_9AE8
        add    esp, 8
        mov    dword ptr [esi + 0xc], eax
        sti    
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_9BBE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_9BD4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x32
        mov    dword ptr [ebp - 4], 0x32
        push   eax
        push   9
        push   ecx
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9C08:
        ret    
        db  000h, 000h, 000h
loc_2_9C0C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, 0x64
        jbe    loc_2_9C1E
        mov    eax, 0x64
loc_2_9C1E:
        mov    ecx, 5
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x18], eax
        sub    eax, eax
        ret    
loc_2_9C31:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0x18]
        imul   eax, eax, 5
        ret    
loc_2_9C3F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 0xb
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_9C55:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 3
        mov    dword ptr [eax + 0xc], 4
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        push   eax
        push   3
        push   ecx
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9C90:
        ret    
        db  000h, 000h, 000h
loc_2_9C94:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [esi]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    
loc_2_9CB2:
        push   ebp
        mov    ebp, esp
        cli    
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [edx + 0x10], 0
        sti    
        sub    eax, eax
        pop    ebp
        ret    
loc_2_9CD3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_9CE9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 1
        push   eax
        push   5
        push   ecx
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9D1D:
        ret    
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 4]
        ret    
        push   ebp
        mov    ebp, esp
        cli    
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], ecx
        sti    
        sub    eax, eax
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
        db  0C3h
loc_2_9D7C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret    
loc_2_9D86:
        push   ebp
        mov    ebp, esp
        cli    
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        sti    
        sub    eax, eax
        pop    ebp
        ret    
loc_2_9D99:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret    
loc_2_9DAE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9DD5:
        ret    
        db  000h, 000h
loc_2_9DD8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret    
loc_2_9DE2:
        push   ebp
        mov    ebp, esp
        cli    
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        sti    
        sub    eax, eax
        pop    ebp
        ret    
loc_2_9DF5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 0x44
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_9E0B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli    
        mov    ecx, dword ptr [ebp + 0xc]
        imul   ecx, ecx, 0x41
        mov    eax, dword ptr [ebp + 8]
        lea    edi, [ecx + eax + 4]
        mov    esi, dword ptr [ebp + 0x10]
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        sti    
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_9E2E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   8
        push   dword ptr [ebp + 8]
        call   sub_1_07F0
loc_2_9E41:
        add    esp, 8
        push   eax
        call   sub_2_9E0B
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9E4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, 0                        ; -> obj3:0x1560
        mov    edi, dword ptr [ebp + 0xc]
loc_2_9E61:
        push   esi
        push   ebx
        push   edi
        call   sub_2_9E0B
        add    esp, 0xc
        add    esi, 0x41
        inc    ebx
        cmp    esi, 0x104                    ; -> obj3:0x1664
        jb     loc_2_9E61
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 4]
        push   eax
        push   8
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9E9C:
        ret    
        db  000h, 000h, 000h
loc_2_9EA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret    
loc_2_9EAA:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret    
loc_2_9EBB:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret    
loc_2_9ED0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 4]
        push   eax
        push   7
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9EF7:
        ret    
loc_2_9EF8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret    
loc_2_9F02:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret    
loc_2_9F16:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    
loc_2_9F25:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9F4C:
        ret    
        db  000h, 000h, 000h
loc_2_9F50:
        jmp    sub_1_1177
loc_2_9F55:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_1_113C
loc_2_9F65:
        add    esp, 8
        sub    eax, eax
        pop    ebp
        ret    
loc_2_9F6C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    
loc_2_9F7B:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, 0                        ; -> loc_1_159C
        mov    dword ptr [eax], 0
        sub    eax, eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, 1
        pop    edi
        pop    ebp
        ret    
loc_2_9F9A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0xffffffff
        lea    eax, [ebp - 8]
        push   eax
        push   0xa
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_9FC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret    
loc_2_9FCA:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret    
loc_2_9FD9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret    
loc_2_9FEE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0xb
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        mov    eax, dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [eax + 4], esi
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_A01E:
        ret    
        db  000h
sub_2_A020:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, eax
        je     loc_2_A033
loc_2_A02C:
        inc    eax
        shr    ecx, 1
        or     ecx, ecx
        jne    loc_2_A02C
loc_2_A033:
        pop    ebp
        ret    
sub_2_A035:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, eax
        je     loc_2_A04A
        mov    eax, 1
        dec    cl
        shl    eax, cl
loc_2_A04A:
        pop    ebp
        ret    
sub_2_A04C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2c], 0x18
        je     loc_2_A072
        cmp    dword ptr [esi + 0x2c], 0x19
        je     loc_2_A072
        lea    eax, [esi + 0x74]
        push   eax
        push   dword ptr [esi + 0x28]
        call   sub_2_A44C
        add    esp, 8
        mov    dword ptr [esi + 0x2c], eax
loc_2_A072:
        lea    edi, [esi + 0x74]
        cmp    dword ptr [edi], 4
        je     loc_2_A07F
        cmp    dword ptr [edi], 5
        jne    loc_2_A098
loc_2_A07F:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_2_A098
        lea    eax, [esi + 0x89]
        push   eax
        push   edi
        push   dword ptr [esi + 0x2c]
        call   sub_2_A107
        add    esp, 0xc
loc_2_A098:
        push   edi
        push   dword ptr [esi + 0x2c]
        call   sub_2_A0BB
        add    esp, 8
        mov    dword ptr [esi + 0x4c], eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h
sub_2_A0AC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        cmp    byte ptr [eax + 0xe], 1
        sbb    eax, eax
        inc    eax
        ret    
sub_2_A0BB:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 7
        jg     loc_2_A0D9
        je     loc_2_A100
        cmp    eax, 1
        je     loc_2_A0F9
        cmp    eax, 2
        je     loc_2_A100
        cmp    eax, 4
        je     loc_2_A0F9
        jmp    loc_2_A0E8
loc_2_A0D9:
        cmp    eax, 0x1b
        je     loc_2_A100
        cmp    eax, 0x1e
        je     loc_2_A0F9
        cmp    eax, 0x26
        je     loc_2_A100
loc_2_A0E8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xe]
        or     al, al
        jne    loc_2_A0F4
        jmp    loc_2_A100
loc_2_A0F4:
        movzx  eax, al
        jmp    loc_2_A105
loc_2_A0F9:
        mov    eax, 3
        jmp    loc_2_A105
loc_2_A100:
        mov    eax, 2
loc_2_A105:
        pop    ebp
        ret    
sub_2_A107:
        ret    
sub_2_A108:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        cmp    eax, 7
        ja     loc_2_A11B
        jmp    dword ptr [eax*4 + 0x104]     ; -> obj2:0xA1B0
loc_2_A11B:
        sub    eax, eax
        jmp    loc_2_A1AE
loc_2_A122:
        mov    eax, 1
        jmp    loc_2_A1AE
loc_2_A12C:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx]
        cmp    eax, 6
        je     loc_2_A14F
        cmp    eax, 7
        je     loc_2_A156
        cmp    byte ptr [ecx + 0xe], 3
        jne    loc_2_A148
        mov    eax, 8
        jmp    loc_2_A1AE
loc_2_A148:
        mov    eax, 7
        jmp    loc_2_A1AE
loc_2_A14F:
        mov    eax, 0x1e
        jmp    loc_2_A1AE
loc_2_A156:
        mov    eax, 4
        jmp    loc_2_A1AE
loc_2_A15D:
        mov    eax, 2
        jmp    loc_2_A1AE
loc_2_A164:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx], 6
        jne    loc_2_A173
        mov    eax, 0x1e
        jmp    loc_2_A1AE
loc_2_A173:
        cmp    byte ptr [ecx + 0xe], 3
        jne    loc_2_A190
        mov    ax, word ptr [ecx + 4]
        cmp    ax, 0xa
        je     loc_2_A189
        cmp    ax, 0x14
        jne    loc_2_A190
loc_2_A189:
        mov    eax, 4
        jmp    loc_2_A1AE
loc_2_A190:
        mov    eax, 3
        jmp    loc_2_A1AE
loc_2_A197:
        sub    eax, eax
        jmp    loc_2_A1AE
loc_2_A19B:
        mov    eax, 0x12
        jmp    loc_2_A1AE
loc_2_A1A2:
        mov    eax, 0x21
        jmp    loc_2_A1AE
loc_2_A1A9:
        mov    eax, 0x28
loc_2_A1AE:
        pop    ebp
        ret    
        ; ---- dispatch table (8 entries) ----
        dd  loc_2_A122
        dd  loc_2_A12C
        dd  loc_2_A15D
        dd  loc_2_A164
        dd  loc_2_A197
        dd  loc_2_A19B
        dd  loc_2_A1A2
        dd  loc_2_A1A9
sub_2_A1D0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 6]
        cmp    eax, 0x20
        jg     loc_2_A1F3
        je     loc_2_A2D6
        cmp    eax, 0x17
        ja     loc_2_A21B
        shl    eax, 2
        jmp    loc_2_A2F9
loc_2_A1F3:
        cmp    eax, 0x40
        je     loc_2_A2DD
        cmp    eax, 0x60
        je     loc_2_A2E4
        cmp    eax, 0x80
        je     loc_2_A2EB
        cmp    eax, 0x81
        je     loc_2_A2F2
loc_2_A21B:
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    loc_2_A2F7
loc_2_A22C:
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    loc_2_A2F7
loc_2_A23D:
        cmp    byte ptr [ecx + 0xe], 3
        mov    eax, dword ptr [ebp + 8]
        jne    loc_2_A268
        cmp    eax, 2
        jne    loc_2_A252
        cmp    word ptr [ecx + 4], 2
        je     loc_2_A25E
loc_2_A252:
        cmp    eax, 4
        jne    loc_2_A268
        cmp    word ptr [ecx + 4], 0x50
        jne    loc_2_A268
loc_2_A25E:
        mov    eax, 0x13
        jmp    loc_2_A2F7
loc_2_A268:
        push   ecx
        push   eax
        call   sub_2_A108
        add    esp, 8
        jmp    loc_2_A2F7
loc_2_A277:
        mov    eax, 0xa
        jmp    loc_2_A2F7
loc_2_A27E:
        mov    eax, 0x16
        jmp    loc_2_A2F7
loc_2_A285:
        mov    eax, 0x1c
        jmp    loc_2_A2F7
loc_2_A28C:
        mov    eax, 0x13
        jmp    loc_2_A2F7
loc_2_A293:
        mov    eax, 0x15
        jmp    loc_2_A2F7
loc_2_A29A:
        mov    eax, 0xb
        jmp    loc_2_A2F7
loc_2_A2A1:
        mov    eax, 0x17
        jmp    loc_2_A2F7
loc_2_A2A8:
        mov    eax, 0x29
        jmp    loc_2_A2F7
loc_2_A2AF:
        mov    eax, 0x2a
        jmp    loc_2_A2F7
loc_2_A2B6:
        mov    eax, 0x2b
        jmp    loc_2_A2F7
loc_2_A2BD:
        sub    eax, eax
        jmp    loc_2_A2F7
loc_2_A2C1:
        mov    eax, 0x22
        jmp    loc_2_A2F7
loc_2_A2C8:
        mov    eax, 0x23
        jmp    loc_2_A2F7
loc_2_A2CF:
        mov    eax, 0x22
        jmp    loc_2_A2F7
loc_2_A2D6:
        mov    eax, 0x1f
        jmp    loc_2_A2F7
loc_2_A2DD:
        mov    eax, 0x2f
        jmp    loc_2_A2F7
loc_2_A2E4:
        mov    eax, 0x33
        jmp    loc_2_A2F7
loc_2_A2EB:
        mov    eax, 0x20
        jmp    loc_2_A2F7
loc_2_A2F2:
        mov    eax, 0x27
loc_2_A2F7:
        pop    ebp
        ret    
loc_2_A2F9:
        add    byte ptr [ecx], 0
        add    byte ptr [ecx - 0x34ffffff], dl ; -> loc_2_A23D
        add    dword ptr [eax], eax
        add    dl, dl                        ; -> loc_2_A27E
        add    dword ptr [eax], eax
        add    cl, bl                        ; -> loc_2_A285
        add    dword ptr [eax], eax
        add    al, ah                        ; -> loc_2_A28C
        add    dword ptr [eax], eax
        add    bh, ah                        ; -> loc_2_A293
        add    dword ptr [eax], eax
        add    dh, ch                        ; -> loc_2_A29A
        add    dword ptr [eax], eax
        add    ch, dh                        ; -> loc_2_A2A1
        add    dword ptr [eax], eax
        add    ah, bh                        ; -> loc_2_A2A8
        add    dword ptr [eax], eax
        add    byte ptr [ebx], al            ; -> loc_2_A2AF
        add    al, byte ptr [eax]
        add    byte ptr [edx], cl            ; -> loc_2_A2B6
        add    al, byte ptr [eax]
        add    byte ptr [edi + 1], ch        ; -> loc_2_A21B
        add    byte ptr [eax], al
        outsd  dx, dword ptr [esi]
        add    dword ptr [eax], eax
        add    byte ptr [edi + 1], ch        ; -> loc_2_A21B
        add    byte ptr [eax], al
        outsd  dx, dword ptr [esi]
        add    dword ptr [eax], eax
        add    byte ptr [ecx], dl            ; -> loc_2_A2BD
        add    al, byte ptr [eax]
        add    byte ptr [0x1c000002], dl     ; -> loc_2_A2C1
        add    al, byte ptr [eax]
        add    byte ptr [ebx], ah            ; -> loc_2_A2CF
        add    al, byte ptr [eax]
        add    byte ptr [ebx], ah            ; -> loc_2_A2CF
        add    al, byte ptr [eax]
        add    byte ptr [ebx], ah            ; -> loc_2_A2CF
        add    al, byte ptr [eax]
        add    byte ptr [ebx], ah            ; -> loc_2_A2CF
        add    al, byte ptr [eax]
        add    byte ptr [ebx], ah            ; -> loc_2_A2CF
        add    al, byte ptr [eax]
        db  000h
sub_2_A359:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        cmp    word ptr [eax + 6], 0
        je     loc_2_A374
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A1D0
        add    esp, 8
        jmp    loc_2_A3AE
loc_2_A374:
        movzx  ecx, byte ptr [eax + 9]
        or     ecx, ecx
        je     loc_2_A394
        cmp    ecx, 1
        je     loc_2_A3A2
        cmp    ecx, 2
        je     loc_2_A3A9
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    loc_2_A3AE
loc_2_A394:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    loc_2_A3AE
loc_2_A3A2:
        mov    eax, 0x1f
        jmp    loc_2_A3AE
loc_2_A3A9:
        mov    eax, 0x2f
loc_2_A3AE:
        pop    ebp
        ret    
sub_2_A3B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [eax + 8]
        or     cl, cl
        jne    loc_2_A3CC
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A359
        add    esp, 8
        jmp    loc_2_A44A
loc_2_A3CC:
        movzx  ecx, cx
        cmp    ecx, 0x104
        jg     loc_2_A3F3
        je     loc_2_A42E
        cmp    ecx, 0x101
        je     loc_2_A419
        cmp    ecx, 0x102
        je     loc_2_A420
        cmp    ecx, 0x103
        je     loc_2_A427
        jmp    loc_2_A40B
loc_2_A3F3:
        cmp    ecx, 0x201
        jl     loc_2_A40B
        cmp    ecx, 0x203
        jle    loc_2_A43E
        cmp    ecx, 0x204
        je     loc_2_A445
loc_2_A40B:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A359
        add    esp, 8
        jmp    loc_2_A44A
loc_2_A419:
        mov    eax, 0x1b
        jmp    loc_2_A44A
loc_2_A420:
        mov    eax, 0x39
        jmp    loc_2_A44A
loc_2_A427:
        mov    eax, 0x3a
        jmp    loc_2_A44A
loc_2_A42E:
        cmp    word ptr [eax + 6], 0x40
        jne    loc_2_A437
        jmp    loc_2_A445
loc_2_A437:
        mov    eax, 0x3b
        jmp    loc_2_A44A
loc_2_A43E:
        mov    eax, 0x26
        jmp    loc_2_A44A
loc_2_A445:
        mov    eax, 0x3c
loc_2_A44A:
        pop    ebp
        ret    
sub_2_A44C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        cmp    dword ptr [eax + 0xa], 0
        push   dword ptr [ebp + 8]
        je     loc_2_A463
        call   sub_2_A3B0
        jmp    loc_2_A468
loc_2_A463:
        call   sub_2_A1D0
loc_2_A468:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_A46C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0x10]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_A494:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0xc], esi
        mov    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], edi
        push   eax
        push   2
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_9825
        add    esp, 0xc
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_676C
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   edi
        push   esi
        push   dword ptr [ebx + 0x2c]
        push   dword ptr [ebx + 0x34]
        call   sub_2_7BE5
        add    esp, 0x14
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_A4E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   5
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x34]
        call   sub_2_7D67
        add    esp, 0x10
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_A531:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   4
        push   esi
        call   sub_2_9825
        add    esp, 0xc
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x34]
        call   sub_2_7D12
        add    esp, 0x10
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_A57E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x4c], ecx
        sub    eax, eax
        pop    ebp
        ret    
sub_2_A58E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x30], ecx
        sub    eax, eax
        pop    ebp
        ret    
sub_2_A59E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        push   7
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0xc
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h, 0C3h, 000h, 000h, 000h
sub_2_A5C8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        cmp    eax, 0x28
        ja     loc_2_A5DB
        jmp    dword ptr [eax*4 + 0x6b]      ; -> obj2:0xA633
loc_2_A5DB:
        mov    eax, 0xc
        jmp    loc_2_A631
loc_2_A5E2:
        sub    eax, eax
        jmp    loc_2_A631
loc_2_A5E6:
        mov    eax, 3
        jmp    loc_2_A631
loc_2_A5ED:
        mov    eax, 4
        jmp    loc_2_A631
loc_2_A5F4:
        mov    eax, 1
        jmp    loc_2_A631
loc_2_A5FB:
        mov    eax, 6
        jmp    loc_2_A631
loc_2_A602:
        mov    eax, 2
        jmp    loc_2_A631
loc_2_A609:
        mov    eax, 0xb
        jmp    loc_2_A631
loc_2_A610:
        mov    eax, 7
        jmp    loc_2_A631
loc_2_A617:
        mov    eax, 8
        jmp    loc_2_A631
loc_2_A61E:
        mov    eax, 0xa
        jmp    loc_2_A631
loc_2_A625:
        mov    eax, 9
        jmp    loc_2_A631
loc_2_A62C:
        mov    eax, 5
loc_2_A631:
        pop    ebp
        ret    
        ; ---- dispatch table (41 entries) ----
        dd  loc_2_A5E2
        dd  loc_2_A5E6
        dd  loc_2_A5ED
        dd  loc_2_A5F4
        dd  loc_2_A5F4
        dd  loc_2_A5F4
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5F4
        dd  loc_2_A5FB
        dd  loc_2_A5FB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A602
        dd  loc_2_A609
        dd  loc_2_A609
        dd  loc_2_A610
        dd  loc_2_A610
        dd  loc_2_A617
        dd  loc_2_A61E
        dd  loc_2_A602
        dd  loc_2_A625
        dd  loc_2_A5DB
        dd  loc_2_A62C
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A61E
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A5DB
        dd  loc_2_A610
loc_2_A6D7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, 0xfffffffe
        push   6
        call   sub_2_4550
        add    esp, 4
        or     eax, eax
        je     loc_2_A7B9
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 0x51
        ja     loc_2_A70F
        cmp    ecx, 0x50
        jae    loc_2_A7B2
        cmp    ecx, 8
        ja     loc_2_A70F
        jmp    dword ptr [ecx*4 + 0x1ff]     ; -> obj2:0xA7C7
loc_2_A70F:
        mov    esi, 0xfffffffe
        jmp    loc_2_A7B9
loc_2_A719:
        push   0
        push   3
        push   eax
        call   sub_2_67F4
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A72D:
        push   0
        push   3
        push   eax
        call   sub_2_67F4
        add    esp, 0xc
        mov    esi, 0xfffffffe
        jmp    loc_2_A7B9
loc_2_A741:
        push   1
        push   3
        push   eax
        call   sub_2_67F4
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A752:
        push   1
        push   eax
        call   sub_2_68C6
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A761:
        push   0
        push   eax
        call   sub_2_68C6
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A770:
        push   0
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A77F:
        push   0xb4
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A791:
        push   0x5a
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A7A0:
        push   0x10e
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_A7B9
loc_2_A7B2:
        call   sub_2_AB13
        sub    esi, esi
loc_2_A7B9:
        or     esi, esi
        mov    eax, 0
        setge  al
        dec    eax
        pop    esi
        pop    ebp
        ret    
        ; ---- dispatch table (9 entries) ----
        dd  loc_2_A719
        dd  loc_2_A72D
        dd  loc_2_A741
        dd  loc_2_A752
        dd  loc_2_A761
        dd  loc_2_A770
        dd  loc_2_A77F
        dd  loc_2_A791
        dd  loc_2_A7A0
loc_2_A7EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, 0xfffffffe
        mov    dword ptr [ebp - 4], 0
        push   6
        call   sub_2_4550
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_2_AA5B
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0xc
        ja     loc_2_A823
        jmp    dword ptr [eax*4 + 0x4a6]     ; -> obj2:0xAA6E
loc_2_A823:
        mov    esi, 0xfffffffe
        jmp    loc_2_AA5B
loc_2_A82D:
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_6692
        add    esp, 8
        mov    esi, eax
        push   dword ptr [ebp - 8]
        call   sub_2_A5C8
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_AA5B
loc_2_A84F:
        lea    eax, [ebp - 0x20]
        push   eax
        push   edi
        call   sub_2_6658
        add    esp, 8
        mov    esi, eax
        cmp    word ptr [ebp - 0x20], 2
        jne    loc_2_A872
        movzx  eax, word ptr [ebp - 0x18]
        dec    eax
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_AA5B
loc_2_A872:
        mov    dword ptr [ebp - 4], 4
        jmp    loc_2_AA5B
loc_2_A87E:
        lea    eax, [ebp - 0x14]
        push   eax
        push   edi
        call   sub_2_6BE4
        add    esp, 8
        mov    esi, eax
        mov    eax, dword ptr [ebp - 0x14]
        or     eax, eax
        je     loc_2_A8AA
        cmp    eax, 1
        je     loc_2_A8B6
        cmp    eax, 2
        je     loc_2_A8C2
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_AA5B
loc_2_A8AA:
        mov    dword ptr [ebp - 4], 3
        jmp    loc_2_AA5B
loc_2_A8B6:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_AA5B
loc_2_A8C2:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_2_AA5B
loc_2_A8CE:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6BB3
        add    esp, 8
        mov    esi, eax
        dec    dword ptr [ebp - 4]
        jmp    loc_2_AA5B
loc_2_A8E5:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_65A9
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xff
        mov    ebx, dword ptr [ebp - 4]
        and    ebx, 0xffffff00
        imul   ebx, ebx, 0xa
        mov    dword ptr [ebp - 0x10], 0xa
        mov    eax, ecx
        cdq    
        idiv   dword ptr [ebp - 0x10]
        shl    eax, 8
        add    ebx, eax
        mov    dword ptr [ebp - 0xc], 0xa
        mov    eax, ecx
        cdq    
        idiv   dword ptr [ebp - 0xc]
        or     ebx, edx
        mov    dword ptr [ebp - 4], ebx
        call   sub_2_655C
        push   0
        push   edi
        call   sub_2_66BF
        add    esp, 8
        push   0x32
        push   0x32
        push   edi
        call   sub_2_672F
        add    esp, 0xc
        jmp    loc_2_AA5B
loc_2_A94E:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_676C
        add    esp, 8
        mov    esi, eax
        mov    eax, 0
        cmp    dword ptr [ebp - 4], 2
        setg   al
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_AA5B
loc_2_A971:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6B61
        add    esp, 8
        mov    esi, eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, eax
        je     loc_2_A9A6
        cmp    eax, 0x5a
        je     loc_2_A9B2
        cmp    eax, 0xb4
        je     loc_2_A9BE
        cmp    eax, 0x10e
        je     loc_2_A9CA
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_AA5B
loc_2_A9A6:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_AA5B
loc_2_A9B2:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_2_AA5B
loc_2_A9BE:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_AA5B
loc_2_A9CA:
        mov    dword ptr [ebp - 4], 3
        jmp    loc_2_AA5B
loc_2_A9D6:
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   edi
        call   sub_2_67A2
        add    esp, 0xc
        mov    esi, eax
        cmp    dword ptr [ebp - 4], 1
        cmc    
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_AA5B
loc_2_A9F6:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6860
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_AA5B
loc_2_AA11:
        mov    esi, 0xfffffffe
        jmp    loc_2_AA5B
loc_2_AA18:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6A59
        add    esp, 8
        mov    esi, eax
        jmp    loc_2_AA5B
loc_2_AA29:
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_6692
        add    esp, 8
        mov    esi, eax
        push   dword ptr [ebp - 8]
        call   sub_2_A5C8
        add    esp, 4
        sub    eax, 0xb
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_AA5B
loc_2_AA52:
        mov    dword ptr [ebp - 4], 0x20
        sub    esi, esi
loc_2_AA5B:
        or     esi, esi
        mov    eax, 0xffffffff
        jl     loc_2_AA67
        mov    eax, dword ptr [ebp - 4]
loc_2_AA67:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (13 entries) ----
        dd  loc_2_A82D
        dd  loc_2_A84F
        dd  loc_2_A87E
        dd  loc_2_A8CE
        dd  loc_2_A8E5
        dd  loc_2_A94E
        dd  loc_2_A971
        dd  loc_2_A9D6
        dd  loc_2_A9F6
        dd  loc_2_AA11
        dd  loc_2_AA18
        dd  loc_2_AA29
        dd  loc_2_AA52
        db  000h, 000h
loc_2_AAA4:
        push   ebp
        mov    ebp, esp
        mov    dword ptr [0], 0              ; -> obj3:0x1980
        push   dword ptr [ebp + 8]
        push   6
        push   0xc                           ; -> obj3:0x198C
        push   0x14                          ; -> obj3:0x1994
        call   sub_1_12B4
loc_2_AAC5:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_AAC9:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        cmp    dword ptr [0], eax            ; -> obj3:0x1980
        jne    loc_2_AB01
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [8], ax              ; -> obj3:0x1988
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [4], eax            ; -> obj3:0x1984
        push   1
        push   4                             ; -> obj3:0x1984
        push   0                             ; -> loc_2_AAA4
        call   sub_1_12AC
loc_2_AAF9:
        add    esp, 0xc
        mov    dword ptr [0], eax            ; -> obj3:0x1980
loc_2_AB01:
        pop    ebp
        ret    
        db  06Ah, 000h, 06Ah, 000h, 0E8h, 0BDh, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 066h, 02Bh, 0C0h, 0C3h
sub_2_AB13:
        push   0
        push   1
        call   sub_2_AAC9
        add    esp, 8
        ret    
loc_2_AB20:
        push   0
        push   2
        call   sub_2_AAC9
        add    esp, 8
        ret    
sub_2_AB2D:
        sub    eax, eax
        ret    
sub_2_AB30:
        sub    eax, eax
        ret    
        db  000h
sub_2_AB34:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    loc_2_AB47
        sub    eax, eax
        jmp    loc_2_AC0E
loc_2_AB47:
        cmp    esi, -1
        jne    loc_2_AB56
        mov    eax, 1
        jmp    loc_2_AC0E
loc_2_AB56:
        inc    byte ptr [0]                  ; -> obj3:0x0548
        cmp    byte ptr [0], 1               ; -> obj3:0x0548
        jne    loc_2_ABFA
        cmp    esi, 0x100
        jb     loc_2_AB9D
        cmp    esi, 0x10b
        jae    loc_2_AB9D
        imul   esi, esi, 5
        mov    edi, dword ptr [ebp + 0x14]
        push   dword ptr [esi - 0x4ff]       ; -> obj3:0x14A1
        push   dword ptr [edi + 0x18]
        movzx  eax, byte ptr [esi - 0x500]   ; -> obj3:0x14A0
        call   sub_3_1AA0
        add    esp, 8
        sub    eax, eax
        jmp    loc_2_AC08
loc_2_AB9D:
        cmp    esi, 0x200
        jb     loc_2_ABE7
        cmp    esi, 0x228
        jae    loc_2_ABE7
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edi + 0x18]
        push   ecx
        lea    eax, [ecx + 4]
        push   eax
        push   dword ptr [eax]
        call   sub_2_455F
        add    esp, 0xc
        or     eax, eax
        je     loc_2_ABE3
        imul   esi, esi, 5
        push   dword ptr [esi - 0x9c7]       ; -> obj3:0x0FD9
        push   dword ptr [edi + 0x18]
        movzx  eax, byte ptr [esi - 0x9c8]   ; -> obj3:0x0FD8
        call   sub_3_1AB0
        add    esp, 8
loc_2_ABE3:
        sub    eax, eax
        jmp    loc_2_AC08
loc_2_ABE7:
        mov    edi, dword ptr [ebp + 0x14]
        mov    eax, 0x50
        mov    ecx, dword ptr [edi + 0x18]
        mov    dword ptr [ecx], 0xffffffea
        jmp    loc_2_AC08
loc_2_ABFA:
        mov    edi, dword ptr [ebp + 0x14]
        sub    eax, eax
        mov    ecx, dword ptr [edi + 0x18]
        mov    dword ptr [ecx], 0xffffffec
loc_2_AC08:
        dec    byte ptr [0]                  ; -> obj3:0x0548
loc_2_AC0E:
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_AC12:
        push   ebp
        mov    ebp, esp
        call   dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx], eax
        ret    
loc_2_AC1F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 4]
        push   eax
        call   dword ptr [ebp + 0xc]
        add    esp, 4
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_AC35:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 4]
        lea    eax, [esi + 8]
        push   eax
        call   dword ptr [ebp + 0xc]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_AC4E:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_AC67:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_AC80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_AC98:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_ACB5:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_ACD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0xc]
        push   eax
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 0xc
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
loc_2_ACEC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_AD08:
        VxDCall  BIOS.Service_8004h              ; cd2004802b00
        add    byte ptr [eax], al
sub_2_AD10:
        VxDCall  BIOS.Service_8009h              ; cd2009802b00
        add    byte ptr [eax], al
sub_2_AD18:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        add    byte ptr [eax], al
sub_2_AD20:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_2_AD28:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_2_AD30:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        add    byte ptr [eax], al
sub_2_AD38:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_2_AD40:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        add    byte ptr [eax], al
sub_2_AD48:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key_Size ; cd203c803300
        add    byte ptr [eax], al
sub_2_AD50:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        add    byte ptr [eax], al
sub_2_AD58:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_2_AD60:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x1acc  RW-
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h
        dd  loc_2_3689               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_2_1E12               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_2_2E2E               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 03Fh, 000h, 000h
        db  02Ah, 021h, 000h, 000h, 02Ah, 023h, 000h, 000h, 000h, 000h, 000h, 000h, 08Ch, 000h, 000h, 000h
        dd  obj3:0x0034              ; fixup
        dd  loc_2_0DEE               ; fixup
        db  000h, 000h, 000h, 000h, 08Ch, 000h, 000h, 000h
        dd  obj3:0x0038              ; fixup
        dd  loc_2_0E93               ; fixup
        db  000h, 000h, 000h, 000h, 08Ch, 000h, 000h, 000h
        dd  obj3:0x003C              ; fixup
        dd  loc_2_0F21               ; fixup
        dd  obj3:0x0044              ; fixup
        dd  obj3:0x0054              ; fixup
        dd  obj3:0x0064              ; fixup
        dd  loc_1_0190               ; fixup
        db  04Ch, 000h, 000h, 000h, 052h, 000h, 000h, 000h, 04Dh, 000h, 000h, 000h, 051h, 000h, 000h, 000h
        db  04Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 023h, 000h, 000h, 000h
        dd  obj3:0x0080              ; fixup
        db  01Eh, 000h, 000h, 000h
        dd  obj3:0x0084              ; fixup
        db  046h, 000h, 000h, 000h
        dd  obj3:0x0088              ; fixup
        db  064h, 000h, 000h, 000h
        dd  obj3:0x008C              ; fixup
        db  096h, 000h, 000h, 000h
        dd  obj3:0x0090              ; fixup
        db  044h, 000h, 000h, 000h, 073h, 000h, 000h, 000h
        dd  obj3:0x00C0              ; fixup
        dd  obj3:0x00C4              ; fixup
        db  060h, 009h, 000h, 000h, 0B0h, 004h, 000h, 000h, 0C0h, 012h, 000h, 000h, 080h, 025h, 000h, 000h
        db  055h, 000h, 000h, 000h, 054h, 000h, 000h, 000h, 053h, 000h, 000h, 000h, 056h, 000h, 000h, 000h
        db  058h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x00E0              ; fixup
        db  007h, 005h, 000h, 000h, 000h
        dd  obj3:0x00E8              ; fixup
        db  00Fh, 007h, 000h, 000h, 000h
        dd  obj3:0x00EC              ; fixup
        db  002h, 008h, 000h, 000h, 000h
        dd  obj3:0x00F0              ; fixup
        db  002h, 02Ah, 06Eh, 000h, 000h, 02Ah, 06Fh, 000h, 000h, 02Ah, 070h, 000h, 000h, 02Ah, 071h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0B0h, 004h, 000h, 000h
        dd  obj3:0x011C              ; fixup
        db  060h, 009h, 000h, 000h
        dd  obj3:0x0120              ; fixup
        db  0C0h, 012h, 000h, 000h
        dd  obj3:0x0124              ; fixup
        db  080h, 025h, 000h, 000h
        dd  obj3:0x0128              ; fixup
        db  053h, 065h, 072h, 069h, 061h, 06Ch, 04Dh, 06Fh, 075h, 073h, 065h, 000h, 029h, 05Ch, 03Bh, 02Ch
        db  009h, 03Ch, 01Bh, 00Ch, 04Ch, 047h, 049h, 000h, 000h, 005h, 004h, 000h, 003h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  014h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  03Ch, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 050h, 000h, 000h, 000h, 005h, 000h, 000h, 000h
        db  064h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 0C8h, 000h, 000h, 000h, 014h, 000h, 000h, 000h
        db  0E8h, 000h, 001h, 001h, 0E9h, 000h, 001h, 001h, 0EAh, 000h, 001h, 001h, 0ECh, 000h, 001h, 001h
        db  0FCh, 000h, 001h, 001h, 0F4h, 000h, 001h, 001h, 004h, 002h, 001h, 001h, 008h, 002h, 001h, 001h
        db  010h, 002h, 002h, 001h, 014h, 002h, 002h, 001h, 018h, 002h, 002h, 001h, 040h, 002h, 003h, 001h
        db  044h, 002h, 003h, 001h, 048h, 002h, 003h, 001h, 00Ch, 002h, 004h, 001h, 00Dh, 002h, 004h, 001h
        db  000h, 000h, 001h, 001h, 043h, 04Fh, 04Dh, 050h, 041h, 051h, 000h, 000h, 054h, 04Fh, 053h, 048h
        db  049h, 042h, 041h, 000h, 054h, 034h, 000h, 000h, 002h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_2_3D3C               ; fixup
        dd  loc_2_3D67               ; fixup
        dd  loc_2_3C68               ; fixup
        dd  loc_2_3B90               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 043h, 03Ah, 05Ch, 04Dh, 04Fh, 055h, 053h, 045h, 044h, 052h, 056h, 02Eh
        db  049h, 04Eh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 054h, 068h, 069h, 073h
        db  020h, 069h, 073h, 020h, 061h, 020h, 04Ch, 04Fh, 047h, 049h, 054h, 045h, 043h, 048h, 020h, 06Dh
        db  06Fh, 075h, 073h, 065h, 020h, 064h, 072h, 069h, 076h, 065h, 072h, 02Ch, 020h, 062h, 075h, 074h
        db  020h, 073h, 06Fh, 06Dh, 065h, 020h, 073h, 06Fh, 066h, 074h, 077h, 061h, 072h, 065h, 020h, 065h
        db  078h, 070h, 065h, 063h, 074h, 020h, 068h, 065h, 072h, 065h, 020h, 074h, 068h, 065h, 020h, 066h
        db  06Fh, 06Ch, 06Ch, 06Fh, 077h, 069h, 06Eh, 067h, 020h, 073h, 074h, 072h, 069h, 06Eh, 067h, 03Ah
        db  02Ah, 02Ah, 02Ah, 020h, 054h, 068h, 069h, 073h, 020h, 069h, 073h, 020h, 043h, 06Fh, 070h, 079h
        db  072h, 069h, 067h, 068h, 074h, 020h, 031h, 039h, 038h, 033h, 020h, 04Dh, 069h, 063h, 072h, 06Fh
        db  073h, 06Fh, 066h, 074h, 020h, 02Ah, 02Ah, 02Ah, 0EBh, 032h, 0E9h, 02Fh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Ch, 04Fh, 047h, 049h, 054h, 045h, 043h, 048h
        db  020h, 04Dh, 04Fh, 055h, 053h, 045h, 020h, 044h, 052h, 049h, 056h, 045h, 052h, 020h, 056h, 036h
        db  02Eh, 034h, 032h, 00Ah, 00Dh, 020h, 043h, 053h, 053h, 000h, 000h, 000h, 02Eh, 0FFh, 02Eh, 000h
        db  000h, 053h, 06Ch, 06Fh, 077h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 04Dh, 06Fh, 064h, 065h, 072h, 061h, 074h, 065h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 046h, 061h, 073h, 074h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 055h, 06Eh, 061h, 063h, 063h, 065h, 06Ch, 065h, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 020h
        db  020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 01Fh, 000h, 014h, 005h, 057h, 005h
        db  05Fh, 005h, 0A8h, 005h, 0CBh, 005h, 0ECh, 005h, 094h, 006h, 09Ch, 006h, 0A4h, 006h, 01Dh, 007h
        db  05Ch, 007h, 064h, 007h, 0A7h, 007h, 0D6h, 007h, 01Eh, 008h, 06Fh, 008h, 09Eh, 008h, 0A6h, 008h
        db  0DFh, 008h, 013h, 009h, 01Bh, 009h, 02Bh, 009h, 02Dh, 009h, 02Fh, 009h, 031h, 009h, 033h, 009h
        db  035h, 009h, 037h, 009h, 000h, 000h, 03Ch, 000h, 0E5h, 0FEh, 06Dh, 0FFh, 002h, 0FEh, 06Eh, 0FEh
        db  0BEh, 0FDh, 0E7h, 0FDh, 08Ch, 0FDh, 054h, 0FDh, 0EBh, 0FCh, 0C2h, 0FCh, 099h, 0FCh, 070h, 0FCh
        db  042h, 0FCh, 019h, 0FCh, 0F0h, 0FBh, 0D5h, 0FBh, 0ACh, 0FBh, 000h, 000h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  005h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  04Eh, 06Fh, 06Eh, 065h, 000h, 000h, 000h, 000h, 062h, 075h, 073h, 000h, 073h, 065h, 072h, 069h
        db  061h, 06Ch, 000h, 000h, 070h, 073h, 032h, 000h
        dd  obj3:0x0410              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj3:0x0418              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj3:0x041C              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj3:0x0424              ; fixup
        db  004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0F8h, 003h, 000h, 000h, 004h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 0F8h, 002h, 000h, 000h
        db  003h, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 0E8h, 003h, 000h, 000h, 004h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h, 0E8h, 002h, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 049h
        db  04Eh, 046h, 04Fh, 000h, 043h, 04Fh, 04Dh, 020h, 000h, 000h, 000h, 000h, 04Dh, 06Fh, 075h, 073h
        db  065h, 054h, 079h, 070h, 065h, 000h, 000h, 000h, 04Ch, 04Dh, 04Fh, 055h, 053h, 045h, 03Ah, 020h
        db  041h, 050h, 049h, 020h, 054h, 072h, 061h, 06Eh, 073h, 06Ch, 061h, 074h, 069h, 06Fh, 06Eh, 020h
        db  065h, 072h, 072h, 06Fh, 072h, 02Eh, 000h, 000h, 007h, 000h, 034h, 000h, 061h, 000h, 068h, 000h
        db  06Fh, 000h, 076h, 000h, 07Dh, 000h, 084h, 000h, 08Bh, 000h, 092h, 000h, 0C9h, 000h, 002h, 001h
        db  009h, 001h, 03Dh, 001h, 080h, 001h, 0B9h, 001h, 0C0h, 001h, 0CAh, 001h, 0D4h, 001h, 015h, 002h
        db  04Eh, 002h, 087h, 002h, 0BBh, 002h, 0FEh, 002h, 005h, 003h, 00Fh, 003h, 019h, 003h, 023h, 003h
        db  02Dh, 003h, 037h, 003h, 041h, 003h, 04Bh, 003h, 055h, 003h, 05Fh, 003h, 069h, 003h, 073h, 003h
        db  07Dh, 003h, 087h, 003h, 091h, 003h, 09Bh, 003h, 0A5h, 003h, 0AFh, 003h, 0B9h, 003h, 0C3h, 003h
        db  0CDh, 003h, 0D7h, 003h, 0E1h, 003h, 0EBh, 003h, 0F5h, 003h, 0FFh, 003h
        ; ---- dispatch table (2 entries) ----
        dd  loc_2_6555
        dd  loc_2_654E
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Fh, 066h, 066h, 000h
        db  04Fh, 06Eh, 000h, 000h, 059h, 065h, 073h, 000h, 054h, 072h, 075h, 065h, 000h, 000h, 000h, 000h
        db  04Fh, 04Bh, 000h, 000h, 031h, 000h, 000h, 000h, 059h, 000h, 000h, 000h
        dd  obj3:0x0554              ; fixup
        dd  obj3:0x0558              ; fixup
        dd  obj3:0x055C              ; fixup
        dd  obj3:0x0560              ; fixup
        dd  obj3:0x0568              ; fixup
        dd  obj3:0x056C              ; fixup
        dd  obj3:0x0570              ; fixup
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 000h, 000h, 041h, 063h, 063h, 065h, 06Ch, 065h, 072h, 061h, 074h, 069h, 06Fh, 06Eh
        db  000h, 000h, 000h, 000h, 042h, 075h, 074h, 074h, 06Fh, 06Eh, 04Dh, 061h, 070h, 070h, 069h, 06Eh
        db  067h, 000h, 000h, 000h, 042h, 075h, 074h, 074h, 06Fh, 06Eh, 053h, 077h, 061h, 070h, 000h, 000h
        db  000h, 000h, 000h, 000h, 042h, 061h, 075h, 064h, 052h, 061h, 074h, 065h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 043h, 068h, 061h, 06Eh, 06Eh, 065h, 06Ch, 04Eh, 075h, 06Dh, 062h, 065h
        db  072h, 000h, 000h, 000h, 044h, 04Fh, 053h, 044h, 072h, 061h, 067h, 04Ch, 06Fh, 063h, 06Bh, 000h
        db  000h, 000h, 000h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 04Dh, 06Fh, 064h, 065h, 06Ch, 000h
        db  000h, 000h, 000h, 000h, 04Eh, 075h, 06Dh, 062h, 065h, 072h, 04Fh, 066h, 042h, 075h, 074h, 074h
        db  06Fh, 06Eh, 073h, 000h, 04Fh, 072h, 069h, 065h, 06Eh, 074h, 061h, 074h, 069h, 06Fh, 06Eh, 000h
        db  000h, 000h, 000h, 000h, 04Dh, 06Fh, 075h, 073h, 065h, 053h, 070h, 065h, 065h, 064h, 000h, 000h
        db  000h, 000h, 000h, 000h, 050h, 06Fh, 073h, 069h, 074h, 069h, 06Fh, 06Eh, 042h, 075h, 074h, 074h
        db  06Fh, 06Eh, 000h, 000h, 045h, 072h, 067h, 06Fh, 06Eh, 06Fh, 06Dh, 069h, 063h, 052h, 065h, 073h
        db  06Fh, 06Ch, 075h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h, 000h, 000h, 04Dh, 061h, 070h, 070h
        db  069h, 06Eh, 067h, 042h, 075h, 074h, 074h, 06Fh, 06Eh, 000h, 000h, 000h, 043h, 068h, 06Fh, 072h
        db  064h, 044h, 065h, 06Ch, 061h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 06Fh, 075h, 062h
        db  06Ch, 065h, 043h, 06Ch, 069h, 063h, 06Bh, 000h, 000h, 000h, 000h, 000h, 044h, 069h, 073h, 061h
        db  062h, 06Ch, 065h, 072h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 072h, 061h, 067h
        db  04Ch, 06Fh, 063h, 06Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x05E4              ; fixup
        dd  obj3:0x05F4              ; fixup
        dd  obj3:0x0604              ; fixup
        dd  obj3:0x0614              ; fixup
        dd  obj3:0x0624              ; fixup
        dd  obj3:0x0634              ; fixup
        dd  obj3:0x0644              ; fixup
        dd  obj3:0x0654              ; fixup
        dd  obj3:0x0664              ; fixup
        dd  obj3:0x0674              ; fixup
        dd  obj3:0x0684              ; fixup
        dd  obj3:0x0694              ; fixup
        dd  obj3:0x06AC              ; fixup
        dd  obj3:0x06BC              ; fixup
        dd  obj3:0x06CC              ; fixup
        dd  obj3:0x06DC              ; fixup
        dd  obj3:0x06EC              ; fixup
        db  000h, 000h, 000h, 000h, 055h, 06Eh, 06Bh, 06Eh, 06Fh, 077h, 06Eh, 000h, 042h, 075h, 073h, 000h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 070h, 06Fh, 072h, 074h, 000h, 000h, 050h, 053h, 032h, 000h
        db  000h, 000h, 000h, 000h, 053h, 065h, 072h, 069h, 061h, 06Ch, 043h, 000h, 043h, 037h, 000h, 000h
        db  043h, 039h, 000h, 000h, 053h, 065h, 072h, 069h, 061h, 06Ch, 04Dh, 000h, 053h, 065h, 072h, 069h
        db  061h, 06Ch, 056h, 000h, 054h, 072h, 061h, 063h, 06Bh, 04Dh, 061h, 06Eh, 031h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 054h, 072h, 061h, 063h, 06Bh, 04Dh, 061h, 06Eh, 032h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 054h, 072h, 061h, 063h, 06Bh, 04Dh, 061h, 06Eh, 033h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 04Bh, 069h, 064h, 07Ah, 000h, 000h, 000h, 000h, 04Dh, 069h, 063h, 072h
        db  06Fh, 073h, 06Fh, 066h, 074h, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 069h, 063h, 072h
        db  06Fh, 073h, 06Fh, 066h, 074h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 069h, 063h, 072h
        db  06Fh, 073h, 06Fh, 066h, 074h, 033h, 000h, 000h, 000h, 000h, 000h, 000h, 046h, 069h, 072h, 073h
        db  074h, 04Dh, 06Fh, 075h, 073h, 065h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 069h, 06Ch, 06Fh
        db  074h, 04Dh, 06Fh, 075h, 073h, 065h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 06Fh, 06Dh, 070h
        db  061h, 071h, 050h, 065h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 06Fh, 075h, 073h
        db  065h, 04Dh, 061h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 06Fh, 075h, 073h
        db  065h, 04Dh, 061h, 06Eh, 04Ch, 065h, 066h, 074h, 031h, 000h, 000h, 000h, 04Dh, 06Fh, 075h, 073h
        db  065h, 04Dh, 061h, 06Eh, 04Ch, 065h, 066h, 074h, 032h, 000h, 000h, 000h, 054h, 072h, 061h, 063h
        db  06Bh, 04Dh, 061h, 06Eh, 050h, 06Fh, 072h, 074h, 061h, 062h, 06Ch, 065h, 031h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 054h, 072h, 061h, 063h, 06Bh, 04Dh, 061h, 06Eh, 050h, 06Fh, 072h, 074h
        db  061h, 062h, 06Ch, 065h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 06Fh, 072h, 064h
        db  06Ch, 065h, 073h, 073h, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 06Fh, 072h, 064h
        db  06Ch, 065h, 073h, 073h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 06Fh, 075h, 073h
        db  065h, 04Dh, 061h, 06Eh, 044h, 065h, 06Ch, 075h, 078h, 065h, 000h, 000h, 045h, 061h, 073h, 079h
        db  050h, 06Fh, 069h, 06Eh, 074h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 033h, 044h, 04Dh, 06Fh
        db  075h, 073h, 065h, 000h, 04Fh, 053h, 032h, 04Dh, 06Fh, 075h, 073h, 065h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 042h, 061h, 06Ch, 06Ch, 050h, 06Fh, 069h, 06Eh, 074h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h, 067h, 072h, 061h, 074h, 065h, 064h, 054h, 072h
        db  061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 031h, 000h, 000h, 000h, 000h, 043h, 079h, 062h, 065h
        db  072h, 04Dh, 061h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 041h, 063h, 063h, 065h
        db  073h, 073h, 042h, 075h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 06Fh, 072h, 064h
        db  06Ch, 065h, 073h, 073h, 033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 074h, 061h, 072h
        db  054h, 072h, 061h, 063h, 06Bh, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 046h, 069h, 072h, 073h
        db  074h, 054h, 072h, 061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 000h, 000h, 050h, 069h, 06Ch, 06Fh
        db  074h, 054h, 072h, 061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 000h, 000h, 041h, 063h, 063h, 075h
        db  050h, 06Fh, 069h, 06Eh, 074h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 070h, 061h, 063h
        db  065h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 04Ah, 06Fh, 079h, 053h
        db  074h, 069h, 063h, 06Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 054h, 072h, 061h, 063h
        db  06Bh, 04Dh, 061h, 06Eh, 056h, 06Fh, 079h, 061h, 067h, 065h, 072h, 000h, 050h, 061h, 06Ch, 06Dh
        db  054h, 072h, 061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 031h, 000h, 000h, 04Dh, 06Fh, 075h, 073h
        db  065h, 04Dh, 061h, 06Eh, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h
        db  067h, 072h, 061h, 074h, 065h, 064h, 054h, 072h, 061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 032h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h, 067h, 072h, 061h, 074h, 065h, 064h, 054h, 072h
        db  061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 033h, 000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h
        db  067h, 072h, 061h, 074h, 065h, 064h, 054h, 072h, 061h, 063h, 06Bh, 042h, 061h, 06Ch, 06Ch, 034h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h, 067h, 072h, 061h, 074h, 065h, 064h, 04Ch, 06Fh
        db  067h, 069h, 050h, 06Fh, 069h, 06Eh, 074h, 031h, 000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h
        db  067h, 072h, 061h, 074h, 065h, 064h, 04Ch, 06Fh, 067h, 069h, 050h, 06Fh, 069h, 06Eh, 074h, 032h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h, 067h, 072h, 061h, 074h, 065h, 064h, 04Ch, 06Fh
        db  067h, 069h, 050h, 06Fh, 069h, 06Eh, 074h, 033h, 000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h
        db  067h, 072h, 061h, 074h, 065h, 064h, 04Ch, 06Fh, 067h, 069h, 050h, 06Fh, 069h, 06Eh, 074h, 034h
        db  000h, 000h, 000h, 000h, 054h, 06Fh, 075h, 063h, 068h, 050h, 061h, 064h, 031h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 054h, 06Fh, 075h, 063h, 068h, 050h, 061h, 064h, 032h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 054h, 06Fh, 075h, 063h, 068h, 050h, 061h, 064h, 033h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 054h, 06Fh, 075h, 063h, 068h, 050h, 061h, 064h, 034h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h, 067h, 072h, 061h, 074h, 065h, 064h, 031h, 000h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 074h, 065h, 067h, 072h, 061h, 074h, 065h, 064h, 032h, 000h
        db  000h, 000h, 000h, 000h, 045h, 061h, 073h, 079h, 050h, 06Fh, 069h, 06Eh, 074h, 045h, 06Ch, 069h
        db  074h, 065h, 000h, 000h, 045h, 061h, 073h, 079h, 050h, 06Fh, 069h, 06Eh, 074h, 043h, 06Fh, 06Eh
        db  074h, 075h, 072h, 061h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 061h, 073h, 079h
        db  050h, 06Fh, 069h, 06Eh, 074h, 041h, 065h, 072h, 06Fh, 000h, 000h, 000h, 04Ch, 06Fh, 067h, 069h
        db  050h, 06Fh, 069h, 06Eh, 074h, 041h, 065h, 072h, 06Fh, 000h, 000h, 000h
        dd  obj3:0x0744              ; fixup
        dd  obj3:0x074C              ; fixup
        dd  obj3:0x0754              ; fixup
        dd  obj3:0x075C              ; fixup
        dd  obj3:0x0764              ; fixup
        dd  obj3:0x076C              ; fixup
        dd  obj3:0x0770              ; fixup
        dd  obj3:0x0774              ; fixup
        dd  obj3:0x077C              ; fixup
        dd  obj3:0x0784              ; fixup
        dd  obj3:0x0794              ; fixup
        dd  obj3:0x07A4              ; fixup
        dd  obj3:0x07B4              ; fixup
        dd  obj3:0x07BC              ; fixup
        dd  obj3:0x07CC              ; fixup
        dd  obj3:0x07DC              ; fixup
        dd  obj3:0x07EC              ; fixup
        dd  obj3:0x07FC              ; fixup
        dd  obj3:0x080C              ; fixup
        dd  obj3:0x081C              ; fixup
        dd  obj3:0x082C              ; fixup
        dd  obj3:0x083C              ; fixup
        dd  obj3:0x084C              ; fixup
        dd  obj3:0x0864              ; fixup
        dd  obj3:0x087C              ; fixup
        dd  obj3:0x088C              ; fixup
        dd  obj3:0x089C              ; fixup
        dd  obj3:0x08AC              ; fixup
        dd  obj3:0x08BC              ; fixup
        dd  obj3:0x08C4              ; fixup
        dd  obj3:0x08D4              ; fixup
        dd  obj3:0x08E4              ; fixup
        dd  obj3:0x08FC              ; fixup
        dd  obj3:0x090C              ; fixup
        dd  obj3:0x091C              ; fixup
        dd  obj3:0x092C              ; fixup
        dd  obj3:0x093C              ; fixup
        dd  obj3:0x094C              ; fixup
        dd  obj3:0x095C              ; fixup
        dd  obj3:0x096C              ; fixup
        dd  obj3:0x097C              ; fixup
        dd  obj3:0x098C              ; fixup
        dd  obj3:0x099C              ; fixup
        dd  obj3:0x09AC              ; fixup
        dd  obj3:0x09BC              ; fixup
        dd  obj3:0x09D4              ; fixup
        dd  obj3:0x09EC              ; fixup
        dd  obj3:0x0A04              ; fixup
        dd  obj3:0x0A1C              ; fixup
        dd  obj3:0x0A34              ; fixup
        dd  obj3:0x0A4C              ; fixup
        dd  obj3:0x0A64              ; fixup
        dd  obj3:0x0A74              ; fixup
        dd  obj3:0x0A84              ; fixup
        dd  obj3:0x0A94              ; fixup
        dd  obj3:0x0AA4              ; fixup
        dd  obj3:0x0AB4              ; fixup
        dd  obj3:0x0AC4              ; fixup
        dd  obj3:0x0AD4              ; fixup
        dd  obj3:0x0AEC              ; fixup
        dd  obj3:0x0AFC              ; fixup
        db  000h, 000h, 000h, 000h, 04Eh, 06Fh, 074h, 050h, 072h, 065h, 073h, 065h, 06Eh, 074h, 000h, 000h
        db  04Fh, 066h, 066h, 000h, 04Dh, 069h, 064h, 064h, 06Ch, 065h, 000h, 000h, 052h, 069h, 067h, 068h
        db  074h, 000h, 000h, 000h, 042h, 06Fh, 074h, 068h, 000h, 000h, 000h, 000h
        dd  obj3:0x0C10              ; fixup
        dd  obj3:0x0C14              ; fixup
        dd  obj3:0x0C1C              ; fixup
        dd  obj3:0x0C24              ; fixup
        dd  obj3:0x0C04              ; fixup
        db  04Ch, 06Fh, 077h, 000h, 04Dh, 065h, 064h, 069h, 075h, 06Dh, 000h, 000h, 048h, 069h, 067h, 068h
        db  000h, 000h, 000h, 000h
        dd  obj3:0x0C10              ; fixup
        dd  obj3:0x0C40              ; fixup
        dd  obj3:0x0C44              ; fixup
        dd  obj3:0x0C4C              ; fixup
        db  032h, 04Bh, 065h, 079h, 000h, 000h, 000h, 000h, 033h, 04Bh, 065h, 079h, 000h, 000h, 000h, 000h
        dd  obj3:0x07B4              ; fixup
        dd  obj3:0x0C64              ; fixup
        dd  obj3:0x0C6C              ; fixup
        dd  obj3:0x0C04              ; fixup
        db  04Ch, 065h, 066h, 074h, 000h, 000h, 000h, 000h, 052h, 069h, 067h, 068h, 074h, 000h, 000h, 000h
        db  054h, 06Fh, 070h, 000h, 000h, 000h, 000h, 000h, 042h, 06Fh, 074h, 074h, 06Fh, 06Dh, 000h, 000h
        db  04Eh, 06Fh, 06Eh, 065h, 000h, 000h, 000h, 000h
        dd  obj3:0x0C84              ; fixup
        dd  obj3:0x0C14              ; fixup
        dd  obj3:0x0C8C              ; fixup
        dd  obj3:0x0C94              ; fixup
        dd  obj3:0x0C9C              ; fixup
        dd  obj3:0x0CA4              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  05Ch, 030h, 030h, 030h, 030h, 000h, 000h, 000h, 054h, 065h, 063h, 068h, 06Eh, 069h, 063h, 061h
        db  06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 046h, 06Fh, 072h, 063h, 065h, 050h, 053h, 032h
        db  045h, 071h, 075h, 069h, 070h, 06Dh, 065h, 06Eh, 074h, 046h, 06Ch, 061h, 067h, 000h, 000h, 000h
        db  054h, 069h, 06Dh, 069h, 06Eh, 067h, 000h, 000h, 044h, 065h, 074h, 065h, 063h, 074h, 049h, 050h
        db  044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 050h, 053h, 032h, 053h, 079h, 06Eh, 063h, 068h
        db  072h, 06Fh, 06Eh, 069h, 07Ah, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  044h, 065h, 062h, 075h, 067h, 054h, 069h, 06Dh, 069h, 06Eh, 067h, 000h, 000h, 000h, 000h, 000h
        db  052h, 054h, 053h, 044h, 054h, 052h, 04Ch, 06Fh, 077h, 054h, 069h, 06Dh, 065h, 000h, 000h, 000h
        db  044h, 054h, 052h, 048h, 069h, 067h, 068h, 054h, 069h, 06Dh, 065h, 000h, 000h, 000h, 000h, 000h
        db  054h, 069h, 06Dh, 069h, 06Eh, 067h, 046h, 061h, 063h, 074h, 06Fh, 072h, 000h, 000h, 000h, 000h
        db  04Dh, 06Fh, 06Eh, 069h, 074h, 06Fh, 072h, 055h, 06Eh, 070h, 06Ch, 075h, 067h, 000h, 000h, 000h
        dd  obj3:0x0CFC              ; fixup
        dd  obj3:0x0D14              ; fixup
        dd  obj3:0x0D1C              ; fixup
        dd  obj3:0x0D2C              ; fixup
        dd  obj3:0x0D44              ; fixup
        dd  obj3:0x0D54              ; fixup
        dd  obj3:0x0D64              ; fixup
        dd  obj3:0x0D74              ; fixup
        dd  obj3:0x0D84              ; fixup
        db  000h, 000h, 000h, 000h, 048h, 061h, 072h, 064h, 077h, 061h, 072h, 065h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 053h, 06Fh, 066h, 074h, 077h, 061h, 072h, 065h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
        dd  obj3:0x0DBC              ; fixup
        dd  obj3:0x0DCC              ; fixup
        db  047h, 06Ch, 06Fh, 062h, 061h, 06Ch, 000h, 000h, 043h, 06Fh, 06Eh, 06Eh, 065h, 063h, 074h, 069h
        db  06Fh, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 06Fh, 06Eh, 06Eh, 065h, 063h, 074h, 069h
        db  06Fh, 06Eh, 050h, 06Fh, 072h, 074h, 000h, 000h, 046h, 06Fh, 072h, 063h, 065h, 064h, 043h, 06Fh
        db  06Eh, 06Eh, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 065h, 061h, 072h, 063h, 068h, 04Fh, 072h, 064h, 065h, 072h, 000h, 000h, 000h, 000h, 000h
        db  050h, 06Fh, 072h, 074h, 053h, 065h, 061h, 072h, 063h, 068h, 04Fh, 072h, 064h, 065h, 072h, 000h
        db  041h, 063h, 074h, 069h, 076h, 065h, 04Dh, 06Fh, 075h, 073h, 065h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x0DEC              ; fixup
        dd  obj3:0x0DFC              ; fixup
        dd  obj3:0x0E0C              ; fixup
        dd  obj3:0x0E24              ; fixup
        dd  obj3:0x0E34              ; fixup
        dd  obj3:0x0E44              ; fixup
        db  004h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 053h, 032h, 02Ch, 020h, 053h, 065h, 072h
        db  069h, 061h, 06Ch, 02Ch, 020h, 042h, 075h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Eh, 06Fh, 06Eh, 065h, 000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 031h, 000h, 000h, 000h, 000h
        db  043h, 04Fh, 04Dh, 032h, 000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 033h, 000h, 000h, 000h, 000h
        db  043h, 04Fh, 04Dh, 034h, 000h, 000h, 000h, 000h
        dd  obj3:0x0E9C              ; fixup
        dd  obj3:0x0EA4              ; fixup
        dd  obj3:0x0EAC              ; fixup
        dd  obj3:0x0EB4              ; fixup
        dd  obj3:0x0EBC              ; fixup
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 031h, 02Ch, 020h, 043h, 04Fh, 04Dh, 032h, 000h, 000h
        db  042h, 075h, 073h, 000h, 053h, 065h, 072h, 069h, 061h, 06Ch, 000h, 000h, 049h, 06Eh, 070h, 06Fh
        db  072h, 074h, 000h, 000h, 050h, 053h, 032h, 000h, 048h, 050h, 000h, 000h, 043h, 06Fh, 06Dh, 070h
        db  061h, 071h, 050h, 065h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 041h, 063h, 063h, 065h
        db  073h, 073h, 042h, 075h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Ah, 06Fh, 079h, 053h
        db  074h, 069h, 063h, 06Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x0E9C              ; fixup
        dd  obj3:0x0EF8              ; fixup
        dd  obj3:0x0EFC              ; fixup
        dd  obj3:0x0F04              ; fixup
        dd  obj3:0x0F0C              ; fixup
        dd  obj3:0x0F10              ; fixup
        dd  obj3:0x0F14              ; fixup
        dd  obj3:0x0F24              ; fixup
        dd  obj3:0x0F34              ; fixup
        db  057h, 069h, 06Eh, 047h, 06Ch, 06Fh, 062h, 061h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  044h, 072h, 069h, 076h, 065h, 072h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 000h, 000h, 000h
        db  057h, 069h, 06Eh, 043h, 043h, 000h, 000h, 000h, 055h, 073h, 065h, 044h, 04Fh, 053h, 044h, 072h
        db  069h, 076h, 065h, 072h, 000h, 000h, 000h, 000h
        dd  obj3:0x0F78              ; fixup
        dd  obj3:0x0F88              ; fixup
        dd  obj3:0x0F90              ; fixup
        db  030h, 02Eh, 030h, 030h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 028h, 001h, 00Ah, 000h
        db  000h, 000h, 032h, 002h, 00Bh, 000h, 000h, 000h, 032h, 002h, 016h, 000h, 000h, 000h, 03Ch, 002h
        db  017h, 000h, 000h, 000h, 03Ch, 002h, 01Eh, 000h, 000h, 000h, 03Ch, 002h, 019h, 000h, 000h, 000h
        db  01Eh, 001h, 022h, 000h, 000h, 000h, 019h, 001h, 01Bh, 000h, 000h, 000h, 014h, 001h, 026h, 000h
        db  000h, 000h, 01Eh, 000h, 01Fh, 000h, 000h, 000h, 032h, 002h, 02Ch, 000h, 000h, 000h, 032h, 002h
        db  02Dh, 000h, 000h, 000h, 032h, 002h, 02Eh, 000h, 000h, 000h, 032h, 002h, 023h, 000h, 000h, 000h
        db  032h, 002h, 029h, 000h, 000h, 000h, 032h, 002h, 02Ah, 000h, 000h, 000h, 032h, 002h, 02Fh, 000h
        db  000h, 000h, 01Eh, 000h, 030h, 000h, 000h, 000h, 01Eh, 000h, 031h, 000h, 000h, 000h, 01Eh, 000h
        db  032h, 000h, 000h, 000h, 01Eh, 000h, 039h, 000h, 000h, 000h, 014h, 001h, 03Ah, 000h, 000h, 000h
        db  023h, 001h, 03Bh, 000h, 000h, 000h, 032h, 001h, 03Ch, 000h, 000h, 000h, 01Eh, 000h, 000h, 000h
        db  000h, 000h, 032h, 001h, 053h, 06Fh, 066h, 074h, 077h, 061h, 072h, 065h, 05Ch, 04Ch, 06Fh, 067h
        db  069h, 074h, 065h, 063h, 068h, 05Ch, 04Dh, 06Fh, 075h, 073h, 065h, 057h, 061h, 072h, 065h, 05Ch
        db  043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 000h, 000h
        db  000h, 000h, 000h, 000h, 04Ch, 04Dh, 04Fh, 055h, 053h, 045h, 000h, 000h, 000h, 000h, 000h, 000h
        db  05Ch, 000h, 000h, 000h, 044h, 06Fh, 073h, 047h, 06Ch, 06Fh, 062h, 061h, 06Ch, 000h, 000h, 000h
        db  044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Ch, 030h, 030h, 030h, 030h, 000h, 000h, 000h
        db  04Dh, 06Fh, 075h, 073h, 065h, 044h, 072h, 076h, 02Eh, 049h, 06Eh, 069h, 000h, 000h, 000h, 000h
        dd  obj3:0x0E0C              ; fixup
        dd  obj3:0x0DFC              ; fixup
        dd  obj3:0x0E44              ; fixup
        dd  obj3:0x0DEC              ; fixup
        dd  obj3:0x109C              ; fixup
        dd  obj3:0x1090              ; fixup
        dd  obj3:0x0D1C              ; fixup
        dd  obj3:0x0D14              ; fixup
        dd  obj3:0x0F68              ; fixup
        dd  obj3:0x0EA4              ; fixup
        dd  obj3:0x0EAC              ; fixup
        dd  obj3:0x0EB4              ; fixup
        dd  obj3:0x0EBC              ; fixup
        dd  obj3:0x05F4              ; fixup
        dd  obj3:0x0604              ; fixup
        dd  obj3:0x0634              ; fixup
        db  054h, 065h, 063h, 068h, 06Eh, 069h, 063h, 061h, 06Ch, 000h, 000h, 000h, 057h, 069h, 06Eh, 047h
        db  06Ch, 06Fh, 062h, 061h, 06Ch, 000h, 000h, 000h
        dd  obj3:0x0DE4              ; fixup
        dd  obj3:0x110C              ; fixup
        dd  obj3:0x1118              ; fixup
        db  000h, 000h, 000h, 000h, 02Eh, 044h, 065h, 066h, 061h, 075h, 06Ch, 074h, 05Ch, 043h, 06Fh, 06Eh
        db  074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 04Dh, 06Fh, 075h, 073h, 065h
        db  000h, 000h, 000h, 000h
        dd  obj3:0x113D              ; fixup
        db  02Dh, 031h, 000h, 000h, 04Dh, 06Fh, 075h, 073h, 065h, 053h, 070h, 065h, 065h, 064h, 000h, 000h
        db  053h, 077h, 061h, 070h, 04Dh, 06Fh, 075h, 073h, 065h, 042h, 075h, 074h, 074h, 06Fh, 06Eh, 073h
        db  000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
        db  00Ch, 00Dh, 00Eh, 00Fh, 009h, 000h, 002h, 001h, 005h, 000h, 000h, 008h, 000h, 009h, 000h, 002h
        db  001h, 005h, 000h, 000h, 000h, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 000h, 000h, 000h, 009h
        db  000h, 002h, 001h, 005h, 000h, 000h, 008h, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 001h, 000h
        db  000h, 009h, 000h, 002h, 001h, 005h, 000h, 001h, 000h, 000h, 009h, 000h, 002h, 001h, 005h, 000h
        db  001h, 000h, 000h, 009h, 000h, 002h, 005h, 005h, 000h, 001h, 008h, 000h, 009h, 000h, 002h, 001h
        db  005h, 000h, 001h, 008h, 000h, 009h, 000h, 002h, 001h, 005h, 001h, 003h, 000h, 000h, 009h, 000h
        db  002h, 001h, 005h, 001h, 003h, 004h, 000h, 009h, 000h, 002h, 001h, 005h, 001h, 003h, 004h, 000h
        db  009h, 000h, 002h, 001h, 005h, 000h, 000h, 000h, 000h, 009h, 000h, 002h, 005h, 005h, 000h, 001h
        db  000h, 000h, 009h, 000h, 002h, 005h, 005h, 000h, 001h, 000h, 000h, 009h, 000h, 002h, 005h, 005h
        db  000h, 001h, 000h, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 001h, 000h, 000h, 009h, 000h, 002h
        db  001h, 005h, 000h, 001h, 000h, 000h, 009h, 004h, 003h, 001h, 005h, 000h, 000h, 00Ch, 000h, 009h
        db  000h, 002h, 001h, 005h, 000h, 001h, 000h, 000h, 009h, 002h, 000h, 001h, 005h, 000h, 001h, 000h
        db  000h, 009h, 002h, 000h, 001h, 005h, 000h, 001h, 004h, 000h, 00Eh, 003h, 004h, 001h, 005h, 001h
        db  002h, 044h, 000h, 00Eh, 003h, 004h, 001h, 005h, 001h, 002h, 044h, 000h, 009h, 000h, 002h, 001h
        db  005h, 000h, 001h, 007h, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 000h, 027h, 000h, 009h, 000h
        db  002h, 001h, 005h, 000h, 001h, 004h, 000h, 009h, 003h, 004h, 005h, 005h, 000h, 002h, 00Ch, 000h
        db  009h, 000h, 002h, 001h, 005h, 000h, 000h, 00Ch, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 000h
        db  000h, 000h, 00Dh, 000h, 002h, 004h, 004h, 001h, 001h, 04Ch, 000h, 009h, 003h, 004h, 001h, 005h
        db  001h, 002h, 048h, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 000h, 004h, 000h, 009h, 000h, 002h
        db  001h, 005h, 000h, 000h, 004h, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 000h, 036h, 000h, 009h
        db  002h, 001h, 000h, 005h, 001h, 002h, 03Eh, 000h, 009h, 000h, 002h, 001h, 005h, 001h, 003h, 000h
        db  000h, 009h, 000h, 002h, 001h, 005h, 001h, 003h, 000h, 000h, 009h, 003h, 004h, 005h, 005h, 000h
        db  002h, 00Ch, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 001h, 00Ch, 000h, 009h, 003h, 004h, 001h
        db  005h, 000h, 002h, 00Ch, 000h, 009h, 003h, 002h, 000h, 005h, 001h, 002h, 04Ch, 000h, 00Dh, 000h
        db  002h, 001h, 005h, 001h, 003h, 04Ch, 000h, 009h, 000h, 002h, 001h, 005h, 000h, 001h, 004h, 000h
        db  009h, 000h, 002h, 001h, 005h, 001h, 002h, 008h, 000h, 009h, 003h, 004h, 001h, 005h, 001h, 002h
        db  008h, 000h, 009h, 000h, 002h, 001h, 005h, 001h, 002h, 008h, 000h, 009h, 003h, 004h, 001h, 005h
        db  002h, 002h, 008h, 000h, 009h, 000h, 002h, 001h, 005h, 002h, 003h, 008h, 000h, 009h, 003h, 004h
        db  001h, 005h, 002h, 002h, 008h, 000h, 009h, 000h, 002h, 001h, 005h, 002h, 002h, 008h, 000h, 009h
        db  003h, 004h, 001h, 005h, 003h, 002h, 008h, 000h, 009h, 000h, 002h, 001h, 005h, 003h, 002h, 008h
        db  000h, 009h, 003h, 004h, 001h, 005h, 003h, 002h, 008h, 000h, 009h, 000h, 002h, 001h, 005h, 003h
        db  002h, 008h, 000h, 009h, 003h, 004h, 001h, 005h, 001h, 002h, 008h, 000h, 009h, 000h, 002h, 001h
        db  005h, 001h, 002h, 008h, 000h, 009h, 003h, 004h, 005h, 005h, 001h, 002h, 00Ch, 000h, 009h, 003h
        db  004h, 005h, 005h, 001h, 002h, 00Ch, 000h, 009h, 004h, 003h, 005h, 005h, 001h, 002h, 00Ch, 000h
        db  009h, 004h, 003h, 005h, 005h, 002h, 003h, 00Ch, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0F6h, 0FFh
        db  0D8h, 0FFh, 0A6h, 0FFh, 060h, 0FFh, 007h, 0FFh, 099h, 0FEh, 018h, 0FEh, 082h, 0FDh, 0D9h, 0FCh
        db  01Ch, 0FCh, 04Ch, 0FBh, 068h, 0FAh, 070h, 0F9h, 065h, 0F8h, 047h, 0F7h, 015h, 0F6h, 0D0h, 0F4h
        db  078h, 0F3h, 00Eh, 0F2h, 090h, 0F0h, 0FFh, 0EEh, 05Ch, 0EDh, 0A6h, 0EBh, 0DEh, 0E9h, 004h, 0E8h
        db  017h, 0E6h, 019h, 0E4h, 009h, 0E2h, 0E7h, 0DFh, 0B4h, 0DDh, 06Fh, 0DBh, 01Ah, 0D9h, 0B3h, 0D6h
        db  03Ch, 0D4h, 0B4h, 0D1h, 01Ch, 0CFh, 073h, 0CCh, 0BBh, 0C9h, 0F3h, 0C6h, 01Bh, 0C4h, 035h, 0C1h
        db  03Fh, 0BEh, 03Ah, 0BBh, 027h, 0B8h, 005h, 0B5h, 0D5h, 0B1h, 097h, 0AEh, 04Ch, 0ABh, 0F3h, 0A7h
        db  08Eh, 0A4h, 01Bh, 0A1h, 09Ch, 09Dh, 011h, 09Ah, 079h, 096h, 0D6h, 092h, 027h, 08Fh, 06Dh, 08Bh
        db  0A9h, 087h, 0DAh, 083h, 000h, 080h, 01Ch, 07Ch, 02Fh, 078h, 039h, 074h, 039h, 070h, 031h, 06Ch
        db  020h, 068h, 007h, 064h, 0E6h, 05Fh, 0BEh, 05Bh, 08Fh, 057h, 058h, 053h, 01Ch, 04Fh, 0D9h, 04Ah
        db  090h, 046h, 042h, 042h, 0EFh, 03Dh, 096h, 039h, 03Ah, 035h, 0D9h, 030h, 074h, 02Ch, 00Ch, 028h
        db  0A1h, 023h, 033h, 01Fh, 0C2h, 01Ah, 050h, 016h, 0DCh, 011h, 066h, 00Dh, 0EFh, 008h, 078h, 004h
        db  000h, 000h, 000h, 000h, 005h, 000h, 096h, 000h, 001h, 000h, 064h, 000h, 00Ah, 000h, 0FAh, 000h
        db  005h, 000h, 0C8h, 000h, 053h, 074h, 061h, 074h, 065h, 030h, 00Ah, 000h, 053h, 074h, 061h, 074h
        db  065h, 031h, 00Ah, 000h, 053h, 06Dh, 061h, 06Ch, 06Ch, 020h, 06Dh, 06Fh, 076h, 065h, 06Dh, 065h
        db  06Eh, 074h, 03Ah, 020h, 054h, 072h, 061h, 073h, 068h, 021h, 021h, 021h, 020h, 058h, 03Ah, 025h
        db  064h, 020h, 059h, 03Ah, 025h, 064h, 00Ah, 000h, 04Eh, 065h, 077h, 020h, 072h, 065h, 070h, 06Fh
        db  072h, 074h, 00Ah, 000h, 053h, 074h, 061h, 074h, 065h, 032h, 00Ah, 000h, 053h, 074h, 061h, 074h
        db  065h, 033h, 00Ah, 000h, 053h, 074h, 061h, 074h, 065h, 034h, 00Ah, 000h, 043h, 068h, 06Fh, 072h
        db  064h, 020h, 066h, 061h, 069h, 06Ch, 075h, 072h, 065h, 03Ah, 020h, 069h, 06Eh, 063h, 06Fh, 072h
        db  072h, 065h, 063h, 074h, 020h, 073h, 074h, 061h, 074h, 065h, 020h, 076h, 061h, 06Ch, 075h, 065h
        db  02Eh, 00Ah, 000h, 000h, 044h, 062h, 06Ch, 043h, 06Ch, 06Bh, 03Ah, 020h, 053h, 074h, 061h, 074h
        db  065h, 020h, 030h, 00Ah, 000h, 000h, 000h, 000h, 044h, 062h, 06Ch, 043h, 06Ch, 06Bh, 03Ah, 020h
        db  053h, 074h, 061h, 074h, 065h, 020h, 031h, 00Ah, 000h, 000h, 000h, 000h, 044h, 062h, 06Ch, 043h
        db  06Ch, 06Bh, 03Ah, 020h, 053h, 074h, 061h, 074h, 065h, 020h, 032h, 00Ah, 000h, 000h, 000h, 000h
        db  044h, 062h, 06Ch, 043h, 06Ch, 06Bh, 03Ah, 020h, 053h, 074h, 061h, 074h, 065h, 020h, 033h, 00Ah
        db  000h, 000h, 000h, 000h, 044h, 062h, 06Ch, 043h, 06Ch, 06Bh, 03Ah, 020h, 053h, 074h, 061h, 074h
        db  065h, 020h, 034h, 00Ah, 000h, 000h, 000h, 000h, 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 001h, 005h, 007h, 009h, 00Bh, 025h
        db  027h, 029h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 014h, 018h, 01Ch, 020h, 024h
        db  028h, 02Ch, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 001h, 009h, 00Ch, 00Fh, 012h
        db  015h, 018h, 01Bh, 01Eh, 021h, 025h, 029h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 014h, 018h, 01Ch, 020h
        db  024h, 028h, 02Ch, 030h, 034h, 038h, 03Ch, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 015h, 001h, 002h, 003h, 004h
        db  005h, 007h, 009h, 00Bh, 00Dh, 010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h, 027h, 02Bh, 030h, 035h
        db  03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 014h, 018h, 01Ch
        db  020h, 024h, 028h, 02Ch, 030h, 034h, 038h, 03Ch, 040h, 044h, 048h, 04Ch, 050h, 054h, 058h, 05Ch
        db  060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 001h, 005h
        db  007h, 009h, 00Bh, 025h, 027h, 029h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 00Ah
        db  00Ch, 00Eh, 010h, 020h, 024h, 028h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 001h
        db  009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh, 01Eh, 021h, 025h, 029h, 02Eh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah
        db  00Ch, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch, 030h, 034h, 038h, 03Ch, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 015h
        db  001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh, 00Dh, 010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h
        db  027h, 02Bh, 030h, 035h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ah, 00Ch, 00Eh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch, 030h, 034h, 038h, 03Ch, 042h
        db  046h, 04Ah, 04Eh, 052h, 056h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 009h, 001h, 005h, 007h, 009h, 00Bh, 025h, 027h, 029h, 02Bh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 001h, 002h, 002h, 002h, 003h, 003h, 003h, 004h, 004h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 00Dh, 001h, 009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh, 01Eh, 021h, 025h, 029h, 02Eh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 002h, 003h, 003h, 003h, 003h, 003h, 003h, 004h, 004h, 004h, 005h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 015h, 001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh, 00Dh, 010h, 013h, 016h
        db  019h, 01Ch, 01Fh, 023h, 027h, 02Bh, 030h, 035h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 001h, 001h, 002h, 002h, 002h, 003h, 003h, 003h, 003h, 004h, 004h
        db  004h, 005h, 005h, 005h, 006h, 006h, 006h, 007h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 009h, 001h, 005h, 007h, 009h, 00Bh, 025h, 027h, 029h, 02Bh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch, 030h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 001h, 009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh, 01Eh
        db  021h, 025h, 029h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch, 030h
        db  034h, 038h, 03Ch, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 015h, 001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh
        db  00Dh, 010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h, 027h, 02Bh, 030h, 035h, 03Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 034h, 038h, 03Ch, 040h, 044h, 048h, 04Ch, 050h, 054h, 058h, 05Ch, 060h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 06Fh, 064h, 053h, 063h, 068h, 043h, 061h
        db  06Ch, 06Ch, 062h, 061h, 063h, 06Bh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 053h, 054h, 055h, 042h, 048h, 04Ch, 050h, 000h, 06Ch, 06Dh, 06Fh, 075h
        db  073h, 065h, 02Eh, 064h, 072h, 076h, 000h, 000h, 000h
        dd  loc_2_6570               ; fixup
        db  000h
        dd  loc_2_659B               ; fixup
        db  001h
        dd  sub_2_65A9               ; fixup
        db  002h
        dd  sub_2_65B9               ; fixup
        db  001h
        dd  sub_2_65E2               ; fixup
        db  001h
        dd  loc_2_65F2               ; fixup
        db  002h
        dd  loc_2_6602               ; fixup
        db  001h
        dd  loc_2_6614               ; fixup
        db  001h
        dd  loc_2_6623               ; fixup
        db  002h
        dd  loc_2_6633               ; fixup
        db  001h
        dd  loc_2_6645               ; fixup
        db  000h, 000h
        dd  sub_2_6658               ; fixup
        db  001h
        dd  sub_2_6692               ; fixup
        db  001h
        dd  sub_2_66A4               ; fixup
        db  002h
        dd  sub_2_66BF               ; fixup
        db  003h
        dd  sub_2_66F6               ; fixup
        db  004h
        dd  sub_2_672F               ; fixup
        db  001h
        dd  sub_2_676C               ; fixup
        db  001h
        dd  loc_2_677E               ; fixup
        db  002h
        dd  loc_2_6790               ; fixup
        db  005h
        dd  sub_2_67A2               ; fixup
        db  004h
        dd  sub_2_67F4               ; fixup
        db  001h
        dd  sub_2_6860               ; fixup
        db  002h
        dd  sub_2_68C6               ; fixup
        db  003h
        dd  sub_2_691D               ; fixup
        db  005h
        dd  sub_2_694F               ; fixup
        db  004h
        dd  sub_2_6981               ; fixup
        db  005h
        dd  sub_2_69AE               ; fixup
        db  005h
        dd  sub_2_69ED               ; fixup
        db  001h
        dd  sub_2_6A59               ; fixup
        db  002h
        dd  sub_2_6A78               ; fixup
        db  001h
        dd  sub_2_6B61               ; fixup
        db  002h
        dd  sub_2_6B7C               ; fixup
        db  001h
        dd  sub_2_6BB3               ; fixup
        db  001h
        dd  sub_2_6BE4               ; fixup
        db  001h
        dd  sub_2_6C14               ; fixup
        db  005h
        dd  sub_2_6C44               ; fixup
        db  001h
        dd  sub_2_6C89               ; fixup
        db  001h
        dd  loc_2_6CB8               ; fixup
        db  001h
        dd  loc_2_6E64               ; fixup
        db  001h
        dd  loc_2_6E6A               ; fixup
        db  006h
        dd  loc_2_6E70               ; fixup
        db  001h
        dd  sub_2_6CBE               ; fixup
        db  002h
        dd  sub_2_6CD9               ; fixup
        db  005h
        dd  sub_2_6D10               ; fixup
        db  004h
        dd  sub_2_6D62               ; fixup
        db  005h
        dd  loc_2_6DCE               ; fixup
        db  004h
        dd  loc_2_6DD4               ; fixup
        db  001h
        dd  sub_2_6DDA               ; fixup
        db  002h
        dd  loc_2_6DF5               ; fixup
        db  002h
        dd  loc_2_6E2C               ; fixup
sub_3_1AA0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_1AB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x44  R-X
loc_4_0000:
        cmp    dword ptr [0x3e], 0           ; -> loc_4_003E
        je     loc_4_0020
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0xc2
        jne    loc_4_0020
        cmp    al, 7
        ja     loc_4_0020
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_2_39DC
loc_4_0020:
        mov    edx, dword ptr [0x3a]         ; -> loc_4_003A
        movzx  ecx, word ptr [0x38]          ; -> loc_4_0038
        VxDCall  Dev_1701.Service_8049h          ; cd2049800117
        dec    esp
loc_4_0034:
        dec    ebp
        dec    edi
        push   ebp
        push   ebx
loc_4_0038:
        inc    ebp
        db  02Eh
loc_4_003A:
        push   esi
        pop    eax
        inc    esp
        db  020h
loc_4_003E:
        push   esi
        jb     0xb5
        db  069h, 06Fh
VxD__LTEXT ENDS
