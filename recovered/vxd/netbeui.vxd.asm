; NETBEUI -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 10794 instructions recovered by recursive descent

Declare_Virtual_Device NETBEUI, 3, 0, NETBEUI_Control, 00031h, Undefined_Init_Order
;   DDK version 0400h   init_order 054006000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x7e2c  R-X
NETBEUI_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
loc_1_0014:
        mov    ax, 0x100
        clc    
        ret    
        db  000h, 000h
sub_1_001C:
        enter  0x10, 0
        mov    dword ptr [ebp - 8], ebx
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_1_6CDC
        mov    eax, dword ptr [ebp - 4]
        test   eax, eax
        je     loc_1_0071
        lea    ecx, [ebp - 0xc]
        push   ecx
        lea    ecx, [ebp - 0x10]
        push   ecx
        push   eax
        call   sub_1_6CC4
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [edi + 0x3c], 0
        je     loc_1_0059
        mov    eax, dword ptr [ebx + 0x124]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebx + 0x124], ecx
loc_1_0059:
        push   dword ptr [ebp - 4]
        call   sub_1_6CBE
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_1_6CDC
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0059
loc_1_0071:
        push   edi
        call   sub_1_6C9A
        mov    ebx, dword ptr [ebp - 8]
        leave  
        ret    
sub_1_007C:
        push   edi
        push   ebx
        push   ecx
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_1_0097
        or     dword ptr [esi + 4], 2
        mov    dword ptr [esi + 0x28], 0
        mov    ebx, 0x4010102
        jmp    loc_1_00A6
loc_1_0097:
        and    dword ptr [esi + 4], 0xfffffffd
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_00DD
        mov    ebx, 0x1010102
loc_1_00A6:
        mov    dword ptr [esi + 0x44], 0x72c ; -> loc_1_072C
        mov    ecx, 6
        lea    edi, [esi + 0x31]
        call   sub_1_02F2
        jb     loc_1_020D
        lea    edi, [0x6e69]                 ; -> obj1:0x6E69
        mov    ecx, 6
        mov    ebx, 0x1010103
        call   sub_1_037F
        jb     loc_1_020D
        jmp    loc_1_014B
loc_1_00DD:
        cmp    dword ptr [esi + 0x28], 1
        je     loc_1_00E9
        stc    
        jmp    loc_1_020D
loc_1_00E9:
        mov    dword ptr [esi + 0x44], 0x702 ; -> loc_1_0702
        mov    ebx, 0x2010101
        cmp    byte ptr [0x6f35], 0          ; -> loc_1_6F35
        je     loc_1_0103
        mov    ebx, 0x2010102
loc_1_0103:
        mov    ecx, 6
        lea    edi, [esi + 0x31]
        call   sub_1_02F2
        jb     loc_1_020D
        xor    ecx, ecx
        mov    ebx, 0x2010103
        call   sub_1_02F2
        jb     loc_1_020D
        mov    dword ptr [0x6e4c], ecx       ; -> loc_1_6E4C
        mov    eax, dword ptr [0x6e6f]       ; -> obj1:0x6E6F
        or     eax, dword ptr [0x6e4c]       ; -> loc_1_6E4C
        sub    ecx, ecx
        mov    ebx, 0x2010103
        call   sub_1_037F
        jb     loc_1_020D
loc_1_014B:
        test   dword ptr [esi + 4], 2
        je     loc_1_0193
        and    dword ptr [esi + 4], 0xfffffffe
        mov    dword ptr [esi + 8], 1
        lea    edi, [0x6e73]                 ; -> obj1:0x6E73
        mov    ecx, 6
        mov    ebx, 0x4010104
        call   sub_1_037F
        jb     loc_1_020D
        lea    edi, [0x6e79]                 ; -> loc_1_6E79
        mov    ecx, 4
        mov    ebx, 0x4010106
        call   sub_1_037F
        jb     loc_1_020D
        jmp    loc_1_01B5
loc_1_0193:
        movzx  ecx, byte ptr [0x6f2c]        ; -> obj1:0x6F2C
        mov    dword ptr [esi + 8], ecx
        mov    ebx, 0x10107
        sub    ecx, ecx
        call   sub_1_02F2
        jb     loc_1_020D
        imul   ecx, ecx, 0x64
        mov    dword ptr [esi + 0x40], ecx
        or     dword ptr [esi + 4], 1
loc_1_01B5:
        sub    ecx, ecx
        mov    ebx, 0x10106
        call   sub_1_02F2
        jb     loc_1_020D
        mov    word ptr [esi + 0x92], cx
        mov    ebx, 0x10105
        sub    ecx, ecx
        call   sub_1_02F2
        mov    eax, 0x80
        cmp    ecx, eax
        jb     loc_1_020D
        sub    ecx, ecx
        mov    ebx, 0x1010f
        call   sub_1_037F
        sub    ecx, ecx
        jb     loc_1_020D
        sub    ecx, ecx
        mov    ebx, 0x1010e
        mov    eax, 3
        cmp    dword ptr [esi + 0x28], 0
        je     loc_1_0206
        mov    eax, 0x4001
loc_1_0206:
        call   sub_1_037F
        jb     loc_1_020D
loc_1_020D:
        pop    ecx
        pop    ebx
        pop    edi
        ret    
sub_1_0211:
        enter  0x1c, 0
        push   edi
        and    dword ptr [esi], 0xfffffffd
        lea    edi, [esi + 0x14]
        mov    dword ptr [ebp - 0x18], edi
        push   esi
        mov    esi, edi
        cld    
loc_1_0223:
        lodsb  al, byte ptr [esi]
        or     al, al
        jne    loc_1_0223
        sub    esi, edi
        mov    word ptr [ebp - 0x1c], si
        mov    word ptr [ebp - 0x1a], si
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x10], 1
        mov    dword ptr [ebp - 0xc], 3
        pop    esi
        xor    eax, eax
        push   eax
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        push   esi
        push   dword ptr [0x6db4]            ; -> obj1:0x6DB4
        push   3
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [esi + 0x28]
        push   eax
        lea    eax, [esi + 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C10
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0x103
        je     loc_1_028D
        cmp    eax, 0
        jne    loc_1_028A
        and    dword ptr [esi], 0xfffffdff
loc_1_0287:
        pop    edi
        leave  
        ret    
loc_1_028A:
        stc    
        jmp    loc_1_0287
loc_1_028D:
        mov    eax, dword ptr [esi + 0x130]
        push   ecx
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        pop    ecx
        test   dword ptr [esi], 0x200
        je     loc_1_0287
        stc    
        mov    eax, 0xc0000001
        jmp    loc_1_0287
sub_1_02B0:
        test   dword ptr [esi], 0x200
        jne    loc_1_02F1
        push   dword ptr [esi + 0x10]
        lea    eax, [esi + 0x2c]
        push   eax
        call   sub_1_6C16
        cmp    dword ptr [esi + 0x2c], 0x103
        je     loc_1_02D5
        or     dword ptr [esi], 0x200
        jmp    loc_1_02F1
loc_1_02D5:
        test   dword ptr [esi], 0x200
        jne    loc_1_02F1
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        jmp    loc_1_02D5
loc_1_02F1:
        ret    
sub_1_02F2:
        enter  0x30, 0
        test   ecx, ecx
        mov    dword ptr [ebp - 0xc], 4
        lea    eax, [ebp - 0x2c]
        je     loc_1_0309
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, edi
loc_1_0309:
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x28], 0
        mov    dword ptr [ebp - 0x14], ebx
        mov    dword ptr [esi + 0x2c], 0xffffffff
        push   ebx
        push   ecx
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_6C2E
        pop    ecx
        pop    ebx
        cmp    dword ptr [ebp - 0x30], 0
        jne    loc_1_034E
loc_1_033F:
        test   ecx, ecx
        jne    loc_1_0348
        mov    ecx, dword ptr [ebp - 0x2c]
        jmp    loc_1_034B
loc_1_0348:
        mov    ecx, dword ptr [ebp - 8]
loc_1_034B:
        clc    
loc_1_034C:
        leave  
        ret    
loc_1_034E:
        cmp    dword ptr [ebp - 0x30], 0x103
        jne    loc_1_0379
loc_1_0357:
        cmp    dword ptr [esi + 0x2c], -1
        jne    loc_1_0373
        push   ecx
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        pop    ecx
        jmp    loc_1_0357
loc_1_0373:
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_033F
loc_1_0379:
        mov    ecx, dword ptr [ebp - 4]
        stc    
        jmp    loc_1_034C
sub_1_037F:
        enter  0x30, 0
        push   eax
        push   ebx
        mov    dword ptr [ebp - 0xc], 4
        mov    dword ptr [ebp - 0x2c], eax
        lea    eax, [ebp - 0x2c]
        test   ecx, ecx
        je     loc_1_039B
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, edi
loc_1_039B:
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x18], 1
        mov    dword ptr [ebp - 0x28], 0
        mov    dword ptr [ebp - 0x14], ebx
        mov    dword ptr [esi + 0x2c], 0xffffffff
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_6C2E
        cmp    dword ptr [ebp - 0x30], 0
        jne    loc_1_03D5
loc_1_03CD:
        mov    ecx, dword ptr [ebp - 8]
        clc    
loc_1_03D1:
        pop    ebx
        pop    eax
        leave  
        ret    
loc_1_03D5:
        cmp    dword ptr [ebp - 0x30], 0x103
        jne    loc_1_0443
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        mov    ax, 0xfdff
        or     ax, word ptr [ebx + 0x2c]
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_040A:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        cmp    dword ptr [esi + 0x2c], -1
        je     loc_1_040A
        cmp    ax, 0xffff
        jne    loc_1_0428
        VMMCall  VMM.Disable_VM_Ints             ; cd201b000100
loc_1_0428:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_03CD
loc_1_0443:
        mov    ecx, dword ptr [ebp - 4]
        stc    
        jmp    loc_1_03D1
sub_1_0449:
        test   word ptr [esi + 0x10], 2
        jne    loc_1_04A8
        sub    eax, eax
        clc    
        pushfd 
        push   edi
        cli    
        lea    edi, [0x6f78]                 ; -> obj1:0x6F78
        call   sub_1_2903
        sti    
        je     loc_1_04BD
        push   edi
        push   esi
        mov    ecx, 0x30
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        push   esi
        movzx  ecx, word ptr [esi + 0x1c]
        mov    esi, dword ptr [esi + 0x18]
        push   ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        lea    ecx, [edi + 0x30]
        mov    dword ptr [edi + 0x18], ecx
loc_1_048C:
        cli    
        push   esi
        lea    esi, [0x6fb0]                 ; -> obj1:0x6FB0
        call   sub_1_287D
        pop    esi
        sti    
        or     dword ptr [0x6eec], 0x200     ; -> obj1:0x6EEC
        pop    edi
        popfd  
        ret    
loc_1_04A8:
        mov    eax, 2
        test   dword ptr [ebx], 0x40
        je     loc_1_04CC
        and    word ptr [esi + 0x10], 0xfbff
        stc    
        ret    
loc_1_04BD:
        or     dword ptr [ebx], 0x80
        pop    edi
        pop    ecx
        mov    eax, 6
        clc    
        ret    
loc_1_04CC:
        stc    
        pushfd 
        push   edi
        mov    edi, esi
        jmp    loc_1_048C
loc_1_04D3:
        call   sub_1_0449
        mov    eax, 0
        jb     loc_1_0527
        inc    byte ptr [ebx + 0x30]
        jmp    loc_1_062D
loc_1_04E7:
        mov    eax, 8
        jmp    loc_1_04F3
loc_1_04EE:
        mov    eax, 6
loc_1_04F3:
        push   edi
        call   sub_1_6C9A
        jmp    loc_1_062D
loc_1_04FE:
        call   sub_1_001C
        mov    eax, 6
        jmp    loc_1_062D
loc_1_050D:
        mov    eax, 6
        jmp    loc_1_062D
sub_1_0517:
        enter  4, 0
        push   edi
        mov    ebx, dword ptr [esi + 0x14]
        test   word ptr [esi + 0x10], 0x400
        jne    loc_1_04D3
loc_1_0527:
        push   ebx
        sub    esp, 8
        mov    eax, esp
        push   dword ptr [ebx + 0x128]
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6C8E
        pop    eax
        pop    edx
        cmp    edx, 0
        mov    edi, eax
        pop    eax
        jne    loc_1_050D
        xor    eax, eax
        test   word ptr [esi + 0x10], 2
        je     loc_1_0553
        mov    eax, esi
loc_1_0553:
        mov    dword ptr [edi + 0x38], eax
        movzx  ecx, word ptr [esi + 0x1c]
        mov    dword ptr [edi + 0x3c], ecx
        test   ecx, ecx
        je     loc_1_05BE
        cmp    ecx, 0x40
        ja     loc_1_04E7
        mov    edx, dword ptr [ebx + 0x124]
        test   edx, edx
        je     loc_1_04EE
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx + 0x124], eax
        push   edx
        push   ecx
        sub    esp, 8
        push   ecx
        push   edx
        push   dword ptr [ebx + 0x12c]
        mov    eax, esp
        add    eax, 0xc
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6CB2
        pop    eax
        pop    edx
        cmp    edx, 0
        pop    ecx
        pop    edx
        jne    loc_1_04FE
        xchg   edi, edx
        add    ecx, 3
        shr    ecx, 2
        push   esi
        mov    esi, dword ptr [esi + 0x18]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    edi, edx
        push   eax
        push   edi
        call   sub_1_6CD6
loc_1_05BE:
        movzx  ecx, word ptr [esi + 0x1e]
        mov    edx, esi
        add    edx, 0x20
loc_1_05C7:
        test   ecx, ecx
        je     loc_1_0608
        push   edx
        push   ecx
        mov    eax, dword ptr [edx + 4]
        movzx  ecx, word ptr [edx + 2]
        sub    esp, 8
        push   ecx
        push   eax
        push   dword ptr [ebx + 0x12c]
        mov    eax, esp
        add    eax, 0xc
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6CB2
        pop    eax
        pop    edx
        cmp    edx, 0
        jne    loc_1_04FE
        push   eax
        push   edi
        call   sub_1_6CD6
        pop    ecx
        pop    edx
        add    edx, 8
        dec    ecx
        jmp    loc_1_05C7
loc_1_0608:
        mov    ebx, dword ptr [esi + 0x14]
        push   edi
        push   dword ptr [ebx + 0x10]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C1C
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0x103
        jne    loc_1_0632
        mov    eax, 2
        inc    dword ptr [ebx + 0xe8]
loc_1_062D:
        mov    edx, esi
        pop    edi
        leave  
        ret    
loc_1_0632:
        mov    ebx, dword ptr [esi + 0x14]
        inc    byte ptr [ebx + 0x30]
        call   sub_1_001C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0676
        push   edi
        test   word ptr [esi + 0x10], 0x402
        jne    loc_1_0668
        mov    edi, dword ptr [esi + 0xc]
        or     edi, edi
        je     loc_1_0668
        mov    edx, 0
        xchg   dword ptr [edi + 0xc4], edx
        or     edx, edx
        je     loc_1_0668
        mov    edx, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], edx
loc_1_0668:
        pop    edi
        inc    dword ptr [ebx + 0xe8]
        mov    eax, 0
        jmp    loc_1_062D
loc_1_0676:
        or     dword ptr [ebx], 0x10
        test   word ptr [esi + 0x10], 0x400
        je     loc_1_0693
        push   esi
        push   edi
        lea    esi, [esi]
        cli    
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        sti    
        pop    edi
        pop    esi
loc_1_0693:
        mov    eax, 6
        jmp    loc_1_062D
        db  000h, 000h, 001h
loc_1_069D:
        or     dword ptr [0x7f6f1303], eax
        inc    ebx
        push   ebx
        arpl   word ptr [ebx + 0xf], si
        pop    ds
        xchg   dword ptr [edi - 0xc1c4051], edx
        nop    
sub_1_06B0:
        push   ss
        xor    byte ptr [eax], al
        add    byte ptr [ebx - 0x1bffffd0], dl ; -> loc_1_3093
        xor    byte ptr [eax], al
        add    byte ptr [ecx + esi], dl      ; -> loc_1_3114
        add    byte ptr [eax], al
        adc    al, 0x31
        add    byte ptr [eax], al
        pop    edx
        xor    dword ptr [eax], eax
        add    byte ptr [edx + 0x31], bl     ; -> loc_1_315A
        add    byte ptr [eax], al
        pushal 
        xor    al, byte ptr [eax]
        add    byte ptr [eax + 0x32], ah     ; -> loc_1_3260
        add    byte ptr [eax], al
        mov    bl, 0x32
        add    byte ptr [eax], al
        mov    bl, 0x32
        add    byte ptr [eax], al
        and    esi, dword ptr [ebx]
        add    byte ptr [eax], al
        and    esi, dword ptr [ebx]
        add    byte ptr [eax], al
        inc    ecx
        xor    eax, dword ptr [eax]
        add    byte ptr [ecx + 0x33], al     ; -> loc_1_3341
        add    byte ptr [eax], al
        dd  sub_1_338C               ; fixup
        dd  sub_1_338C               ; fixup
        dd  sub_1_3428               ; fixup
        dd  sub_1_3428               ; fixup
        db  0B8h, 001h, 000h, 001h, 000h, 0C3h
loc_1_0702:
        mov    edx, esi
        mov    al, byte ptr [ebx + 1]
        and    al, 0xc0
        cmp    al, 0x40
        jne    loc_1_0726
        inc    dword ptr [esi + 0xf0]
        lea    esi, [ebx + 0xe]
        test   byte ptr [ebx + 8], 0x80
        je     loc_1_0737
        mov    al, byte ptr [ebx + 0xe]
        and    eax, 0x1f
        add    esi, eax
        jmp    loc_1_0737
loc_1_0726:
        mov    eax, 0x10001
        ret    
loc_1_072C:
        inc    dword ptr [esi + 0xf0]
        mov    edx, esi
        lea    esi, [ebx + 0xe]
loc_1_0737:
        mov    ax, word ptr [esi]
        and    ax, 0x1fe
        cmp    al, 0xf0
        jne    loc_1_077F
        mov    al, byte ptr [esi + 2]
        movzx  ecx, ah
        test   al, 1
        jne    loc_1_0753
        mov    eax, edx
        jmp    loc_1_2E80
loc_1_0753:
        mov    edi, 0x69c                    ; -> obj1:0x069C
        push   ecx
        mov    ecx, 0x13
        cld    
        repne scasb al, byte ptr es:[edi]
        pop    ecx
        mov    eax, edx
        jne    loc_1_0773
        sub    edi, 0x69d                    ; -> loc_1_069D
        call   sub_1_06B0
loc_1_0773:
        mov    eax, 0
        ret    
loc_1_0779:
        mov    eax, 0x10001
        ret    
loc_1_077F:
        test   al, al
        je     loc_1_0787
        cmp    al, 0xfe
        jne    loc_1_0779
loc_1_0787:
        mov    al, byte ptr [esi + 2]
        mov    cl, al
        and    al, 3
        cmp    al, 3
        jne    loc_1_0779
        mov    al, cl
        movzx  ecx, byte ptr [esi + 1]
        and    ecx, 1
        and    al, 0xef
        cmp    al, 0xaf
        jne    loc_1_07AA
        mov    eax, edx
        call   sub_1_338C
        jmp    loc_1_0773
loc_1_07AA:
        cmp    al, 0xe3
        jne    loc_1_0779
        mov    eax, edx
        call   sub_1_3428
        jmp    loc_1_0773
loc_1_07B7:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_07C9
        and    dword ptr [ecx], 0xfffffdff
loc_1_07C9:
        mov    eax, dword ptr [ecx + 0x130]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        leave  
        ret    0xc
loc_1_07D9:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        or     dword ptr [ebx], 0x200
        pop    ebx
        leave  
        ret    8
loc_1_07EB:
        ret    8
loc_1_07EE:
        push   ebp
        mov    ebp, esp
        pushfd 
        push   esi
        push   edi
        push   ebx
        test   dword ptr [0x6eec], 0x40      ; -> obj1:0x6EEC
        je     loc_1_0806
        call   sub_1_646D
loc_1_0806:
        mov    ebx, dword ptr [ebp + 8]
        cli    
        inc    byte ptr [ebx + 0x30]
        test   dword ptr [ebx], 0x10
        je     loc_1_0831
        mov    al, byte ptr [0x6f31]         ; -> loc_1_6F31
        cmp    al, byte ptr [ebx + 0x30]
        ja     loc_1_0831
        and    dword ptr [ebx], 0xffffffef
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        sti    
        call   sub_1_67F7
        jmp    loc_1_0832
loc_1_0831:
        sti    
loc_1_0832:
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0x38]
        test   esi, esi
        je     loc_1_0893
        test   word ptr [esi + 0x10], 2
        je     loc_1_0893
        test   word ptr [esi + 0x10], 8
        je     loc_1_0863
        mov    eax, dword ptr [ebp + 0x10]
        xor    eax, 0
        je     loc_1_0856
        mov    al, 0x21
loc_1_0856:
        push   esi
        mov    esi, dword ptr [esi + 0x30]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        pop    esi
loc_1_0863:
        test   word ptr [esi + 0x10], 0x1000
        je     loc_1_0875
        and    dword ptr [0x6eec], 0xffffefff ; -> obj1:0x6EEC
loc_1_0875:
        cli    
        or     word ptr [esi + 0x10], 0x40
        test   word ptr [esi + 0x10], 0x400
        sti    
        jne    loc_1_0893
        cli    
        push   edi
        lea    edi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_2868
        pop    edi
        sti    
loc_1_0893:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        call   sub_1_001C
        pop    ebx
        pop    edi
        pop    esi
        popfd  
        leave  
        ret    0xc
loc_1_08A6:
        push   ebp
        mov    ebp, esp
        pushfd 
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edi + 0x2c], eax
        cmp    eax, 0
        jne    loc_1_08C2
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x3c], eax
loc_1_08C2:
        mov    eax, dword ptr [edi + 0x130]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        pop    edi
        popfd  
        leave  
        ret    0x10
loc_1_08D4:
        pushfd 
        push   edi
        push   esi
        push   ebx
        test   dword ptr [0x6eec], 0x40      ; -> obj1:0x6EEC
        je     loc_1_08E9
        call   sub_1_646D
loc_1_08E9:
        test   dword ptr [0x6eec], 0x6b8     ; -> obj1:0x6EEC
        je     loc_1_08FA
        call   sub_1_67F7
loc_1_08FA:
        pop    ebx
        pop    esi
        pop    edi
        popfd  
        ret    4
loc_1_0901:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd 
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        je     loc_1_0912
        jmp    loc_1_0975
loc_1_0912:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, 0x6f40                   ; -> obj1:0x6F40
        mov    ecx, 8
        repne scasd eax, dword ptr es:[edi]
        jne    loc_1_0975
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0x6e80], eax       ; -> obj1:0x6E80
        mov    eax, dword ptr [ebp + 0x20]
        add    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [esi + 0x28], 1
        je     loc_1_0950
        movzx  eax, word ptr [ebx + 0xc]
        xchg   al, ah
        cmp    ax, 0x600
        jae    loc_1_0983
        cmp    eax, dword ptr [ebp + 0x20]
        ja     loc_1_0983
        add    eax, 0xe
loc_1_0950:
        mov    word ptr [esi + 0x76], ax
        mov    eax, dword ptr [ebp + 0x1c]
        add    eax, dword ptr [ebp + 0x14]
        mov    word ptr [esi + 0x74], ax
        mov    dword ptr [esi + 0x70], ebx
        or     dword ptr [esi], 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x80], eax
        push   esi
        call   dword ptr [esi + 0x44]
        pop    esi
        and    dword ptr [esi], 0xfffffffb
loc_1_0975:
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        popfd  
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    0x1c
loc_1_0983:
        mov    eax, 0x10001
        jmp    loc_1_0975
loc_1_098A:
        push   ebp
        mov    ebp, esp
        pushfd 
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x2c], eax
        pop    ebx
        popfd  
        leave  
        ret    0xc
loc_1_099E:
        push   ebp
        mov    ebp, esp
        pushfd 
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, 0x6f40                   ; -> obj1:0x6F40
        mov    ecx, 8
        repne scasd eax, dword ptr es:[edi]
        jne    loc_1_0ABA
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0x40010007
        jne    loc_1_09D7
        push   0
        push   esi
        lea    eax, [ebp + 0xc]
        push   eax
        call   sub_3_011B
        jmp    loc_1_0ABA
loc_1_09D7:
        cmp    eax, 0x40010004
        je     loc_1_09F1
        cmp    eax, 0x40010005
        je     loc_1_0A00
        cmp    eax, 0x40010003
        jne    loc_1_0A0F
        jmp    loc_1_0ABA
loc_1_09F1:
        or     dword ptr [0x6eec], 0x100     ; -> obj1:0x6EEC
        jmp    loc_1_0ABA
loc_1_0A00:
        and    dword ptr [0x6eec], 0xfffffeff ; -> obj1:0x6EEC
        jmp    loc_1_0ABA
loc_1_0A0F:
        cmp    eax, 0x40010008
        jne    loc_1_0A53
        or     dword ptr [esi + 4], 1
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        jne    loc_1_0A28
        mov    ecx, 0x60
loc_1_0A28:
        imul   ecx, ecx, 0x64
        mov    dword ptr [esi + 0x40], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    word ptr [esi + 0x92], cx
        movzx  ecx, word ptr [edi + 0xc]
        or     ecx, ecx
        jne    loc_1_0A45
        mov    ecx, 2
loc_1_0A45:
        mov    dword ptr [esi + 8], ecx
        mov    byte ptr [esi + 0x30], cl
        mov    byte ptr [esi + 0x134], cl
        jmp    loc_1_0ABA
loc_1_0A53:
        cmp    eax, 0x40010009
        jne    loc_1_0A92
        and    dword ptr [esi + 4], 0xfffffffe
        mov    dword ptr [esi + 8], 1
        mov    edi, dword ptr [ebp + 0x10]
        lea    edi, [edi]
        sub    edi, 6
        mov    eax, esi
        call   sub_1_236B
        jb     loc_1_0ABA
        cli    
        mov    byte ptr [ebx + 0x38], 2
        mov    dword ptr [ebx + 0x3c], 2
        sti    
        mov    byte ptr [ebx + 0xb9], 1
        push   ebx
        call   sub_1_6435
        jmp    loc_1_0ABA
loc_1_0A92:
        cmp    eax, 0x4001000a
        jne    loc_1_0ABA
        mov    edi, dword ptr [ebp + 0x10]
        lea    edi, [edi]
        sub    edi, 6
        mov    eax, esi
        call   sub_1_236B
        jb     loc_1_0ABA
        cli    
        or     dword ptr [ebx + 0x34], 0x14
        or     dword ptr [ebx + 0x3c], 0x20
        sti    
        push   ebx
        call   sub_1_4E6C
loc_1_0ABA:
        pop    ebx
        pop    edi
        pop    esi
        popfd  
        leave  
        ret    0x10
loc_1_0AC2:
        ret    4
        db  000h, 000h, 000h, 000h, 002h, 001h, 002h, 002h, 003h, 000h, 000h, 001h, 000h, 005h
loc_1_0AD3:
        cmp    byte ptr [ebx + 0x25], 1
        jne    loc_1_0B14
        dec    byte ptr [ebx + 0x90]
        js     loc_1_0AFB
        mov    esi, dword ptr [ebx + 0x94]
        mov    eax, dword ptr [esi + 4]
loc_1_0AEA:
        call   sub_1_39D9
        mov    ecx, eax
        mov    eax, 0xad3                    ; -> loc_1_0AD3
        jmp    sub_1_34D0
loc_1_0AFB:
        test   word ptr [ebx + 0x42], 0x40
        jne    loc_1_0B0D
        mov    al, 0x12
        test   word ptr [ebx + 0x40], 8
        jne    loc_1_0B0F
loc_1_0B0D:
        mov    al, 0x14
loc_1_0B0F:
        call   sub_1_0BAA
loc_1_0B14:
        ret    
loc_1_0B15:
        cmp    byte ptr [ebx + 0x25], 1
        jne    loc_1_0B3D
        mov    esi, dword ptr [ebx + 0x94]
        mov    eax, dword ptr [esi + 4]
        dec    byte ptr [ebx + 0x90]
        js     loc_1_0B3E
        call   sub_1_3927
        mov    ecx, eax
        mov    eax, 0xb15                    ; -> loc_1_0B15
        call   sub_1_34D0
loc_1_0B3D:
        ret    
loc_1_0B3E:
        test   dword ptr [eax + 4], 1
        jne    loc_1_0B4B
        mov    cl, 1
        jmp    loc_1_0B62
loc_1_0B4B:
        test   dword ptr [eax + 4], 2
        je     loc_1_0B5C
        mov    cl, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_0B62
loc_1_0B5C:
        mov    cl, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_0B62:
        mov    byte ptr [ebx + 0x90], cl
        jmp    loc_1_0AEA
loc_1_0B6A:
        mov    al, 5
        jmp    sub_1_0BAA
loc_1_0B6E:
        cli    
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_0BA1
        test   word ptr [ebx + 0x42], 0x4000
        je     loc_1_0BA1
        or     word ptr [ebx + 0x40], 0x40
        mov    esi, dword ptr [ebx + 0x88]
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        mov    byte ptr [esi + 1], 5
        call   sub_1_6BE9
        call   sub_1_3CD2
        jb     loc_1_0BA3
loc_1_0BA1:
        sti    
        ret    
loc_1_0BA3:
        push   ebx
        call   sub_1_5BF3
        ret    
sub_1_0BAA:
        cli    
        cmp    byte ptr [ebx + 0x25], 1
        je     loc_1_0BBF
        cmp    byte ptr [ebx + 0x25], 3
        je     loc_1_0BBF
        cmp    byte ptr [ebx + 0x25], 4
        je     loc_1_0BBF
        sti    
        ret    
loc_1_0BBF:
        mov    byte ptr [ebx + 0x25], 0
        test   word ptr [ebx + 0x40], 0x80
        pushfd 
        jne    loc_1_0BCE
        jmp    loc_1_0BE5
loc_1_0BCE:
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        mov    esi, dword ptr [ebx + 0x88]
        test   word ptr [ebx + 0x42], 8
        je     loc_1_0BE5
        mov    al, 0xb
loc_1_0BE5:
        or     word ptr [ebx + 0x42], 0x4000
        or     word ptr [ebx + 0x6a], 2
        push   esi
        mov    esi, dword ptr [ebx + 0x94]
        dec    byte ptr [esi + 3]
        cmp    byte ptr [esi + 1], 3
        jne    loc_1_0C05
        call   sub_1_250A
loc_1_0C05:
        pop    esi
        test   word ptr [ebx + 0x42], 2
        jne    loc_1_0C14
        push   ebx
        call   sub_1_5BF3
loc_1_0C14:
        popfd  
        je     loc_1_0C1F
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_0C1F:
        ret    
sub_1_0C20:
        cli    
        cmp    byte ptr [ebx + 0x25], 2
        je     loc_1_0C2F
        cmp    byte ptr [ebx + 0x25], 8
        je     loc_1_0BBF
        sti    
        ret    
loc_1_0C2F:
        test   byte ptr [ebx + 0x24], 2
        je     loc_1_0C49
        and    byte ptr [ebx + 0x24], 0xfd
        push   esi
        push   edi
        lea    esi, [ebx]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_0C49:
        jmp    loc_1_0BBF
sub_1_0C4E:
        mov    al, byte ptr [esi + 0x18]
        add    byte ptr [edx + 2], al
        mov    al, byte ptr [esi + 0x19]
        add    byte ptr [edx + 3], al
        sub    edi, edi
        push   edi
        cld    
loc_1_0C5E:
        mov    edi, dword ptr [esp]
        mov    edi, dword ptr [edi*4 + 0x77c0] ; -> obj1:0x77C0
        test   edi, edi
        je     loc_1_0C82
        cmp    dword ptr [edi + 0x94], esi
        jne    loc_1_0C82
        movzx  eax, byte ptr [edi + 0x25]
        mov    al, byte ptr [eax + 0xac8]    ; -> obj1:0x0AC8
        or     al, al
        jne    loc_1_0C99
loc_1_0C82:
        inc    dword ptr [esp]
        cmp    dword ptr [esp], 0x100
        jb     loc_1_0C5E
        pop    edi
        sub    eax, eax
        ret    
loc_1_0C92:
        pop    edi
        mov    eax, 6
        ret    
loc_1_0C99:
        cmp    ecx, 0x24
        jb     loc_1_0C92
        inc    byte ptr [edx + 1]
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [edi + 0x68]
        mov    byte ptr [ebx], al
        mov    al, byte ptr [edi + 0x8c]
        mov    byte ptr [ebx + 0x22], al
        mov    al, byte ptr [edi + 0x8d]
        mov    byte ptr [ebx + 0x23], al
        push   ecx
        push   esi
        lea    esi, [edi + 0x44]
        lea    edi, [ebx + 2]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    ecx
        sub    ecx, 0x24
        add    ebx, 0x24
        jmp    loc_1_0C82
loc_1_0CD4:
        mov    edi, ebx
        mov    eax, esi
        lea    esi, [ebx + 0x1a]
        call   sub_1_2480
        mov    esi, ebx
        mov    al, 0x15
        jb     loc_1_0CFE
        cmp    byte ptr [ebx + 1], 2
        jne    loc_1_0CFE
        mov    al, 0x19
        test   byte ptr [ebx], 2
        jne    loc_1_0CFE
        call   sub_1_229D
        mov    al, 0x11
        jb     loc_1_0CFE
        jmp    loc_1_0D0B
loc_1_0CFE:
        mov    byte ptr [edi + 1], al
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0xff
        ret    
loc_1_0D0B:
        mov    byte ptr [ebx + 0x25], 1
        mov    al, byte ptr [ebx + 0x68]
        mov    byte ptr [edi + 2], al
        cli    
        mov    dword ptr [ebx + 0x88], edi
        or     word ptr [ebx + 0x40], 0x80
        mov    eax, 0xb15                    ; -> loc_1_0B15
        mov    ecx, dword ptr [esi + 4]
        call   sub_1_34D0
        sti    
        mov    dword ptr [ebx + 0x94], esi
        inc    byte ptr [esi + 3]
        push   dword ptr [esi + 4]
        mov    edx, edi
        mov    ecx, 4
        lea    esi, [edi + 0x1a]
        lea    edi, [ebx + 0x44]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, edx
        mov    ecx, 4
        lea    esi, [edi + 0xa]
        lea    edi, [ebx + 0x54]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, edx
        mov    al, byte ptr [edi + 0x2a]
        mov    byte ptr [ebx + 0x8e], al
        mov    al, byte ptr [edi + 0x2b]
        mov    byte ptr [ebx + 0x8f], al
        mov    eax, dword ptr [esp]
        test   dword ptr [eax + 4], 1
        jne    loc_1_0D7F
        mov    al, 1
        jmp    loc_1_0D94
loc_1_0D7F:
        test   dword ptr [eax + 4], 2
        je     loc_1_0D8F
        mov    al, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_0D94
loc_1_0D8F:
        mov    al, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_0D94:
        mov    byte ptr [ebx + 0x90], al
        pop    eax
        call   sub_1_3927
        mov    al, 0xff
        ret    
loc_1_0DA3:
        mov    edi, ebx
        mov    eax, esi
        lea    esi, [edi + 0x1a]
        call   sub_1_2480
        mov    al, 0x15
        jb     loc_1_0DC0
        cmp    byte ptr [ebx + 1], 2
        jne    loc_1_0DC0
        mov    al, 0x19
        test   byte ptr [ebx], 2
        je     loc_1_0DCD
loc_1_0DC0:
        mov    byte ptr [edi + 1], al
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0xff
        ret    
loc_1_0DCD:
        mov    esi, ebx
        call   sub_1_229D
        mov    al, 0x11
        jb     loc_1_0DC0
        mov    dword ptr [ebx + 0x94], esi
        inc    byte ptr [esi + 3]
        mov    byte ptr [ebx + 0x25], 2
        mov    al, byte ptr [ebx + 0x68]
        mov    byte ptr [edi + 2], al
        mov    dword ptr [ebx + 0x88], edi
        or     word ptr [ebx + 0x40], 0x80
        mov    eax, esi
        mov    edx, edi
        mov    ecx, 4
        lea    esi, [edi + 0x1a]
        lea    edi, [ebx + 0x44]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, edx
        mov    ecx, 4
        lea    esi, [edi + 0xa]
        lea    edi, [ebx + 0x54]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, edx
        mov    esi, eax
        mov    al, byte ptr [edi + 0x2a]
        mov    byte ptr [ebx + 0x8e], al
        mov    al, byte ptr [edi + 0x2b]
        mov    byte ptr [ebx + 0x8f], al
        cli    
        or     byte ptr [ebx + 0x24], 2
        cmp    byte ptr [ebx + 0x54], 0x2a
        je     loc_1_0E45
        push   esi
        lea    esi, [esi + 0x1c]
        call   sub_1_2892
        pop    esi
        jmp    loc_1_0E4F
loc_1_0E45:
        push   esi
        lea    esi, [esi + 0x24]
        call   sub_1_2892
        pop    esi
loc_1_0E4F:
        sti    
        mov    al, 0xff
        ret    
loc_1_0E53:
        call   sub_1_5D8D
        jb     loc_1_0E91
        mov    dword ptr [ebx + 0x88], edi
        or     word ptr [ebx + 0x40], 0xa0
        and    word ptr [ebx + 0x40], 0xffbf
        or     word ptr [ebx + 0x42], 0x4000
        mov    eax, dword ptr [0x6f17]       ; -> loc_1_6F17
        mov    dword ptr [edi + 0x32], eax
        mov    dword ptr [edi + 0x3a], 0xb6e ; -> loc_1_0B6E
        lea    eax, [ebx + 0x30]
        cmp    eax, dword ptr [ebx + 0x30]
        sti    
        jne    loc_1_0E8E
        call   sub_1_3CD2
loc_1_0E8E:
        mov    al, 0xff
        ret    
loc_1_0E91:
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0xff
        ret    
loc_1_0E9B:
        mov    edi, ebx
        movzx  ecx, word ptr [edi + 8]
        cmp    ecx, 4
        jae    loc_1_0EB5
        mov    al, 1
loc_1_0EA8:
        mov    byte ptr [edi + 1], al
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0xff
        ret    
loc_1_0EB5:
        mov    eax, esi
        mov    edx, eax
        sub    ebx, ebx
        cmp    byte ptr [edi + 0x1a], 0x2a
        je     loc_1_0EDC
        lea    esi, [edi + 0x1a]
        push   ecx
        call   sub_1_2480
        pop    ecx
        mov    al, 0x15
        jb     loc_1_0EA8
        cmp    byte ptr [ebx + 1], 2
        jne    loc_1_0EA8
        mov    al, 0x19
        test   byte ptr [ebx], 2
        jne    loc_1_0EA8
loc_1_0EDC:
        mov    esi, ebx
        mov    ebx, dword ptr [edi + 4]
        xor    al, al
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 2], al
        mov    byte ptr [ebx + 3], al
        mov    eax, edx
        mov    edx, ebx
        push   edi
        test   esi, esi
        jne    loc_1_0F25
        mov    byte ptr [ebx], 0xff
        add    ebx, 4
        mov    esi, eax
        mov    edi, esi
        mov    esi, dword ptr [edi + 0x84]
loc_1_0F05:
        push   esi
        push   edi
        cmp    byte ptr [esi + 2], 0
        je     loc_1_0F12
        call   sub_1_0C4E
loc_1_0F12:
        pop    edi
        pop    esi
        cmp    al, 6
        je     loc_1_0F32
        add    esi, 0x44
        cmp    dword ptr [edi + 0x88], esi
        ja     loc_1_0F05
        jmp    loc_1_0F32
loc_1_0F25:
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [ebx], al
        add    ebx, 4
        call   sub_1_0C4E
loc_1_0F32:
        pop    edi
        movzx  edx, word ptr [edi + 8]
        sub    edx, ecx
        mov    word ptr [edi + 8], dx
        ret    
        db  000h, 000h
loc_1_0F40:
        cli    
        cmp    byte ptr [ebx + 0x25], 6
        jne    loc_1_0F94
        mov    esi, dword ptr [ebx + 0x94]
        dec    byte ptr [ebx + 0x90]
        js     loc_1_0F6B
        mov    eax, 0xf40                    ; -> loc_1_0F40
        mov    ecx, dword ptr [esi + 4]
        call   sub_1_34D0
        mov    eax, dword ptr [esi + 4]
        sti    
        jmp    sub_1_39EC
loc_1_0F6B:
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 2
        mov    cl, byte ptr [esi + 2]
        mov    esi, dword ptr [ebx + 0x88]
        call   sub_1_235E
        mov    byte ptr [esi + 3], cl
        mov    byte ptr [esi + 1], 0
        call   sub_1_6BE9
loc_1_0F94:
        sti    
        ret    
sub_1_0F96:
        cli    
        mov    esi, dword ptr [ebx + 0x84]
        add    esi, 0x44
loc_1_0FA0:
        cmp    dword ptr [ebx + 0x88], esi
        jb     loc_1_0FB3
        cmp    byte ptr [esi + 2], 0
        je     loc_1_0FB6
        add    esi, 0x44
        jmp    loc_1_0FA0
loc_1_0FB3:
        sti    
        stc    
        ret    
loc_1_0FB6:
        mov    byte ptr [esi + 2], 0xff
        sti    
        mov    edx, esi
        mov    ecx, ebx
        mov    esi, ebx
        mov    bl, byte ptr [esi + 0x90]
        inc    bl
loc_1_0FC9:
        cmp    bl, 0xfe
        jb     loc_1_0FD0
        mov    bl, 2
loc_1_0FD0:
        push   ebx
        call   sub_1_24CC
        pop    ebx
        jb     loc_1_0FDD
        inc    bl
        jmp    loc_1_0FC9
loc_1_0FDD:
        mov    esi, edx
        xchg   ebx, ecx
        mov    byte ptr [esi + 2], cl
        mov    byte ptr [ebx + 0x90], cl
        mov    byte ptr [esi + 1], 0
        mov    dword ptr [esi + 4], ebx
        sub    eax, eax
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 3], al
        mov    byte ptr [esi + 0x18], al
        mov    byte ptr [esi + 0x19], al
        lea    eax, [esi + 0x1c]
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x20], eax
        lea    eax, [esi + 0x24]
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [esi + 0x28], eax
        lea    eax, [esi + 0x2c]
        mov    dword ptr [esi + 0x2c], eax
        mov    dword ptr [esi + 0x30], eax
        lea    eax, [esi + 0x34]
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0x38], eax
        lea    eax, [esi + 0x3c]
        mov    dword ptr [esi + 0x3c], eax
        mov    dword ptr [esi + 0x40], eax
        clc    
        ret    
loc_1_102D:
        mov    edi, ebx
        mov    ebx, esi
        call   sub_1_0F96
        jb     loc_1_1067
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [edi + 3], al
        mov    al, byte ptr [edi]
        and    al, 0x7f
        cmp    al, 0x36
        jne    loc_1_1049
        or     byte ptr [esi], 1
loc_1_1049:
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 2
        inc    byte ptr [ebx + 0x91]
        xchg   edi, esi
        add    esi, 0x1a
        add    edi, 8
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    eax, eax
        ret    
loc_1_1067:
        mov    al, 0xe
        ret    
loc_1_106A:
        mov    al, byte ptr [ebx + 0x1a]
        or     al, al
        je     loc_1_108E
        cmp    al, 0x2a
        je     loc_1_108E
        mov    edi, ebx
        push   esi
        mov    eax, esi
        lea    esi, [edi + 0x1a]
        call   sub_1_2480
        jb     loc_1_1091
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [edi + 3], al
        pop    esi
        mov    al, 0xd
        ret    
loc_1_108E:
        mov    al, 0x15
        ret    
loc_1_1091:
        pop    ebx
        call   sub_1_0F96
        jb     loc_1_10E6
        test   dword ptr [ebx], 2
        jne    loc_1_10D0
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [edi + 3], al
        mov    al, byte ptr [edi]
        and    al, 0x7f
        cmp    al, 0x36
        jne    loc_1_10B2
        or     byte ptr [esi], 1
loc_1_10B2:
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 2
        inc    byte ptr [ebx + 0x91]
        xchg   edi, esi
        add    esi, 0x1a
        add    edi, 8
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    eax, eax
        ret    
loc_1_10D0:
        call   sub_1_229D
        jae    loc_1_10E9
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi + 0x18], 0
        mov    al, 0x21
        ret    
loc_1_10E6:
        mov    al, 0xe
        ret    
loc_1_10E9:
        mov    dword ptr [ebx + 0x94], esi
        mov    byte ptr [esi + 1], 1
        mov    byte ptr [esi + 3], 1
        mov    byte ptr [ebx + 0x25], 6
        mov    al, byte ptr [edi]
        and    al, 0x7f
        cmp    al, 0x36
        jne    loc_1_110B
        or     word ptr [ebx + 0x40], 0x10
        or     byte ptr [esi], 1
loc_1_110B:
        cli    
        mov    dword ptr [ebx + 0x88], edi
        or     word ptr [ebx + 0x40], 0x80
        mov    eax, 0xf40                    ; -> loc_1_0F40
        mov    ecx, dword ptr [esi + 4]
        call   sub_1_34D0
        sti    
        mov    eax, esi
        mov    edx, edi
        lea    esi, [edi + 0x1a]
        lea    edi, [ebx + 0x44]
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, eax
        mov    edi, edx
        mov    ecx, esi
        lea    esi, [edi + 0x1a]
        lea    edi, [ecx + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, eax
        mov    edi, edx
        mov    eax, dword ptr [esi + 4]
        inc    byte ptr [eax + 0x91]
        test   dword ptr [eax + 4], 1
        jne    loc_1_115F
        mov    al, 1
        jmp    loc_1_1174
loc_1_115F:
        test   dword ptr [eax + 4], 2
        je     loc_1_116F
        mov    al, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_1174
loc_1_116F:
        mov    al, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_1174:
        mov    byte ptr [ebx + 0x90], al
        mov    eax, dword ptr [esi + 4]
        call   sub_1_39EC
        mov    al, 0xff
        ret    
loc_1_1185:
        mov    eax, esi
        lea    edi, [ebx + 0x1a]
        cmp    byte ptr [edi], 0
        je     loc_1_119C
        cmp    byte ptr [edi], 0x2a
        je     loc_1_119C
        call   sub_1_2527
        jb     loc_1_119C
        ret    
loc_1_119C:
        mov    al, 0x15
        ret    
        db  000h
loc_1_11A0:
        test   byte ptr [ebx + 0x3f], 8
        jne    loc_1_1372
        jmp    loc_1_12ED
loc_1_11AF:
        cli    
        mov    al, 0x24
        movzx  edi, byte ptr [ebx + 2]
        dec    edi
        mov    edi, dword ptr [edi*4 + 0x77c0] ; -> obj1:0x77C0
        test   edi, edi
        je     loc_1_11CF
        cmp    byte ptr [edi + 0x25], 5
        je     loc_1_11CF
        or     word ptr [edi + 0x42], 8
        sub    al, al
loc_1_11CF:
        sti    
        ret    
loc_1_11D1:
        cli    
        mov    al, 0x24
        movzx  edi, byte ptr [ebx + 2]
        dec    edi
        mov    edi, dword ptr [edi*4 + 0x77c0] ; -> obj1:0x77C0
        test   edi, edi
        je     loc_1_1201
        cmp    byte ptr [edi + 0x25], 5
        je     loc_1_1201
        or     word ptr [edi + 0x42], 8
        cmp    byte ptr [edi + 0x25], 2
        jne    loc_1_11FF
        push   ebx
        mov    ebx, edi
        cli    
        call   sub_1_0C20
        pop    ebx
loc_1_11FF:
        sub    al, al
loc_1_1201:
        sti    
        ret    
loc_1_1203:
        cli    
        mov    edi, ebx
        test   edi, edi
        je     loc_1_1230
        push   esi
        push   edi
        lea    esi, [edi + 0x32]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        sti    
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    esi, edi
        mov    byte ptr [esi + 1], 0xb
        call   sub_1_6BE9
        sub    al, al
        ret    
loc_1_1230:
        sti    
        mov    al, 0x24
        ret    
loc_1_1234:
        sti    
        stc    
        ret    
sub_1_1237:
        cli    
        cmp    byte ptr [ebx + 0x31], 0xff
        jne    loc_1_1234
        test   byte ptr [ebx + 0x3f], 4
        jne    loc_1_12AB
        push   esi
        push   edi
        lea    esi, [ebx + 0x32]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        mov    dl, byte ptr [ebx]
        mov    edi, ebx
        call   sub_1_2913
        sti    
        and    dl, 0x7f
        cmp    dl, 0x15
        je     loc_1_1290
        cmp    byte ptr [ebx + 3], 0xff
        je     loc_1_128A
        mov    al, byte ptr [ebx + 0x30]
        call   sub_1_2550
        mov    bl, byte ptr [ebx + 3]
        call   sub_1_24CC
        cmp    dl, 0x48
        je     loc_1_1287
        cmp    dl, 0x16
        jne    loc_1_128C
loc_1_1287:
        dec    byte ptr [ebx + 0x19]
loc_1_128A:
        clc    
        ret    
loc_1_128C:
        dec    byte ptr [ebx + 0x18]
        ret    
loc_1_1290:
        movzx  ebx, byte ptr [ebx + 2]
        dec    ebx
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        dec    byte ptr [ebx + 0x8c]
        ret    
loc_1_12A3:
        call   sub_1_5744
        pop    ebx
        clc    
        ret    
loc_1_12AB:
        push   ebx
        movzx  ebx, byte ptr [ebx + 2]
        dec    ebx
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_12E9
        cmp    word ptr [ebx + 0x84], 0
        je     loc_1_12A3
        or     word ptr [ebx + 0x40], 0xc000
        call   sub_1_5744
        call   sub_1_5CEC
        jb     loc_1_12DB
loc_1_12D8:
        pop    ebx
        clc    
        ret    
loc_1_12DB:
        xor    eax, eax
        call   sub_1_3DA6
        call   sub_1_5C40
        jmp    loc_1_12D8
loc_1_12E9:
        sti    
        pop    ebx
        stc    
        ret    
loc_1_12ED:
        mov    al, 0xb
        call   sub_1_1237
        jb     loc_1_12FE
        call   sub_1_646D
        sub    al, al
        ret    
loc_1_12FE:
        mov    al, 0x24
        ret    
loc_1_1301:
        jmp    loc_1_12ED
loc_1_1303:
        jmp    loc_1_12ED
loc_1_1305:
        jmp    loc_1_12ED
loc_1_1307:
        cli    
        mov    al, 0x24
        movzx  edi, byte ptr [ebx + 2]
        dec    edi
        mov    edi, dword ptr [edi*4 + 0x77c0] ; -> obj1:0x77C0
        test   edi, edi
        je     loc_1_1327
        cmp    byte ptr [edi + 0x25], 9
        jne    loc_1_1327
        or     word ptr [edi + 0x42], 8
        sub    al, al
loc_1_1327:
        sti    
        ret    
sub_1_1329:
        cli    
        cmp    byte ptr [ebx + 0x31], 0xff
        jne    loc_1_136F
        mov    esi, ebx
        movzx  ebx, byte ptr [esi + 2]
        dec    ebx
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_136F
        push   esi
        push   edi
        lea    esi, [esi + 0x32]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        or     word ptr [ebx + 0x40], 0x60
        sti    
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    edi, esi
        cli    
        call   sub_1_2913
        sti    
        call   sub_1_3CD2
        clc    
        ret    
loc_1_136F:
        sti    
        stc    
        ret    
loc_1_1372:
        mov    al, 0xb
        call   sub_1_1329
        jb     loc_1_137E
        sub    al, al
        ret    
loc_1_137E:
        mov    al, 0x24
        ret    
sub_1_1381:
        push   eax
        push   ecx
        push   edi
        mov    eax, dword ptr [esi + 0x31]
        mov    dword ptr [ebx], eax
        mov    ax, word ptr [esi + 0x35]
        mov    word ptr [ebx + 4], ax
        mov    byte ptr [ebx + 6], 2
        mov    byte ptr [ebx + 8], 0xff
        mov    byte ptr [ebx + 9], 0x22
        mov    eax, dword ptr [esi + 0xcc]
        shr    eax, 6
        mov    word ptr [ebx + 0xa], ax
        mov    eax, dword ptr [esi + 0xd0]
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [esi + 0xe0]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [esi + 0xe8]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [esi + 0x108]
        mov    dword ptr [ebx + 0x20], eax
        sub    ah, ah
        mov    al, byte ptr [0x6f05]         ; -> loc_1_6F05
        dec    al
        mov    word ptr [ebx + 0x28], ax
        mov    word ptr [ebx + 0x2a], 0xff
        mov    word ptr [ebx + 0x2c], 0xff
        mov    al, byte ptr [0x6ef0]         ; -> obj1:0x6EF0
        mov    word ptr [ebx + 0x34], ax
        mov    word ptr [ebx + 0x36], ax
        mov    al, byte ptr [esi + 0x91]
        dec    al
        mov    word ptr [ebx + 0x3a], ax
        mov    ax, 0x1000
        mov    word ptr [ebx + 0x38], ax
        mov    word ptr [ebx + 0x30], 0x400
        mov    ax, word ptr [0x6f03]         ; -> loc_1_6F03
        mov    word ptr [ebx + 0x2e], ax
        mov    edi, dword ptr [esi + 0x84]
        xor    ecx, ecx
        xor    eax, eax
loc_1_1434:
        cmp    byte ptr [edi + 2], 0
        je     loc_1_143F
        mov    al, byte ptr [edi + 3]
        add    ecx, eax
loc_1_143F:
        add    edi, 0x44
        cmp    edi, dword ptr [esi + 0x88]
        jbe    loc_1_1434
        mov    word ptr [ebx + 0x32], cx
        pop    edi
        pop    ecx
        pop    eax
        ret    
sub_1_1452:
        push   ecx
        mov    ecx, eax
        jecxz  loc_1_146A
loc_1_1457:
        add    ebx, 0x44
        cmp    ebx, dword ptr [esi + 0x88]
        ja     loc_1_146A
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_1457
        loop   loc_1_1457
loc_1_146A:
        pop    ecx
        ret    
sub_1_146C:
        push   ebx
        push   esi
        push   edi
        push   ecx
        mov    edx, ecx
        sub    edx, 0x3c
        lea    edi, [ebx + 0x3c]
        cli    
        mov    ebx, dword ptr [esi + 0x84]
        add    ebx, 0x44
        or     eax, eax
        jne    loc_1_14D5
loc_1_1486:
        cmp    dword ptr [esi + 0x88], ebx
        jb     loc_1_14EE
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_14D0
        sub    edx, 0x12
        js     loc_1_14E4
        push   esi
        lea    esi, [ebx + 8]
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    esi
        inc    eax
        movzx  ecx, byte ptr [ebx + 2]
        test   byte ptr [ebx], 1
        je     loc_1_14B0
        or     ch, 0x80
loc_1_14B0:
        test   byte ptr [ebx], 2
        je     loc_1_14B8
        or     ch, 2
loc_1_14B8:
        cmp    byte ptr [ebx + 1], 2
        jne    loc_1_14C3
        or     ch, 4
        jmp    loc_1_14CC
loc_1_14C3:
        cmp    byte ptr [ebx + 1], 3
        jne    loc_1_14CC
        or     ch, 5
loc_1_14CC:
        xchg   ecx, eax
        stosw  word ptr es:[edi], ax
        xchg   ecx, eax
loc_1_14D0:
        add    ebx, 0x44
        jmp    loc_1_1486
loc_1_14D5:
        call   sub_1_1452
        xor    eax, eax
        add    edx, 0x3c
        sub    edi, 0x3c
        jmp    loc_1_1486
loc_1_14E4:
        sti    
        add    edx, 0x12
        mov    ebx, eax
        mov    al, 6
        jmp    loc_1_14F3
loc_1_14EE:
        sti    
        mov    ebx, eax
        sub    al, al
loc_1_14F3:
        pop    ecx
        sub    ecx, edx
        mov    edx, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_14FC:
        pushal 
        cli    
        mov    esi, dword ptr [ebx + 4]
        mov    bl, byte ptr [ebx + 2]
        lea    edx, [esi + 0xa4]
        push   edx
        mov    edi, dword ptr [edx]
loc_1_150D:
        cmp    edi, dword ptr [esp]
        je     loc_1_153A
        lea    edx, [edi - 0x32]
        cmp    byte ptr [edx + 3], bl
        je     loc_1_151E
loc_1_151A:
        mov    edi, dword ptr [edi]
        jmp    loc_1_150D
loc_1_151E:
        push   esi
        push   edi
        lea    esi, [edx + 0x32]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        jecxz  loc_1_151A
        mov    edi, edx
        call   sub_1_2913
        jmp    loc_1_151A
loc_1_153A:
        pop    edx
        sti    
        call   sub_1_646D
        popal  
        ret    
sub_1_1543:
        push   edi
        push   esi
loc_1_1545:
        mov    esi, dword ptr [esp]
        cmp    dword ptr [esi + 0x88], ebx
        jb     loc_1_155B
        cmp    byte ptr [ebx + 2], 0
        jne    loc_1_155E
        jmp    loc_1_161B
loc_1_155B:
        pop    esi
        pop    edi
        ret    
loc_1_155E:
        call   sub_1_14FC
loc_1_1563:
        cli    
        lea    edi, [ebx + 0x1c]
        call   sub_1_2903
        sti    
        je     loc_1_1598
        and    word ptr [edi + 0x40], 0xff7f
        jecxz  loc_1_1585
        mov    esi, dword ptr [edi + 0x88]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_1585:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    word ptr [edi + 0x40], 0x2000
        call   sub_1_5999
        jmp    loc_1_1563
loc_1_1598:
        cli    
        lea    edi, [ebx + 0x24]
        call   sub_1_2903
        sti    
        je     loc_1_15CD
        and    word ptr [edi + 0x40], 0xff7f
        jecxz  loc_1_15BA
        mov    esi, dword ptr [edi + 0x88]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_15BA:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    word ptr [edi + 0x40], 0x2000
        call   sub_1_5999
        jmp    loc_1_1598
loc_1_15CD:
        cli    
        lea    esi, [ebx + 0x2c]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_15EE
        jecxz  loc_1_15E6
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_15E6:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_15CD
loc_1_15EE:
        cli    
        lea    esi, [ebx + 0x34]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_160F
        jecxz  loc_1_1607
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_1607:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_15EE
loc_1_160F:
        mov    word ptr [ebx + 2], 0
        mov    word ptr [ebx + 0x18], 0
loc_1_161B:
        push   eax
        lea    eax, [ebx + 0x3c]
        mov    dword ptr [ebx + 0x3c], eax
        mov    dword ptr [ebx + 0x40], eax
        pop    eax
        add    ebx, 0x44
        jmp    loc_1_1545
loc_1_162E:
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        cli    
        push   esi
        lea    esi, [esi + 0xb4]
        push   ebx
        lea    ebx, [ebx + 0x32]
        call   sub_1_2892
        pop    ebx
        pop    esi
        sti    
        mov    al, 0xff
        ret    
loc_1_164B:
        cmp    dword ptr [ebx + 4], 0
        je     loc_1_166E
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        cli    
        push   esi
        lea    esi, [esi + 0xac]
        push   ebx
        lea    ebx, [ebx + 0x32]
        call   sub_1_2892
        pop    ebx
        pop    esi
        sti    
        mov    al, 0xff
        ret    
loc_1_166E:
        mov    al, 1
        ret    
sub_1_1671:
        and    dword ptr [0x6eec], 0xfffffffa ; -> obj1:0x6EEC
        cli    
        mov    byte ptr [esi + 0x90], 1
        mov    ebx, dword ptr [esi + 0x84]
        mov    al, 0x17
        call   sub_1_1543
        mov    byte ptr [esi + 0x91], 1
        mov    word ptr [esi + 0x118], 0
        mov    ebx, dword ptr [esi + 0x84]
        mov    dword ptr [esi + 0x8c], ebx
        sti    
        mov    byte ptr [ebx], 4
        mov    byte ptr [ebx + 2], 1
        mov    dword ptr [ebx + 4], esi
        mov    byte ptr [ebx + 1], 2
        lea    eax, [ebx + 0x1c]
        mov    dword ptr [ebx + 0x1c], eax
        mov    dword ptr [ebx + 0x20], eax
        lea    eax, [ebx + 0x24]
        mov    dword ptr [ebx + 0x24], eax
        mov    dword ptr [ebx + 0x28], eax
        lea    eax, [ebx + 0x2c]
        mov    dword ptr [ebx + 0x2c], eax
        mov    dword ptr [ebx + 0x30], eax
        lea    eax, [ebx + 0x34]
        mov    dword ptr [ebx + 0x34], eax
        mov    dword ptr [ebx + 0x38], eax
        lea    eax, [ebx + 0x3c]
        mov    dword ptr [ebx + 0x3c], eax
        mov    dword ptr [ebx + 0x40], eax
        sub    eax, eax
        mov    dword ptr [esi + 0xcc], eax
        mov    dword ptr [esi + 0xe8], eax
        mov    dword ptr [esi + 0xf0], eax
        mov    byte ptr [ebx + 0x18], al
        mov    byte ptr [ebx + 0x19], al
        mov    byte ptr [ebx + 3], al
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0xc], eax
        mov    word ptr [ebx + 0x10], ax
        mov    eax, dword ptr [esi + 0x31]
        mov    dword ptr [ebx + 0x12], eax
        mov    dx, word ptr [esi + 0x35]
        mov    word ptr [ebx + 0x16], ax
        mov    edi, esi
loc_1_171C:
        cli    
        lea    esi, [edi + 0x94]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_1741
        jecxz  loc_1_1739
        mov    byte ptr [esi + 1], 0xb
        call   sub_1_6BE9
loc_1_1739:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_171C
loc_1_1741:
        cli    
        lea    esi, [edi + 0xa4]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_1766
        jecxz  loc_1_175E
        mov    byte ptr [esi + 1], 0xb
        call   sub_1_6BE9
loc_1_175E:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_1741
loc_1_1766:
        cli    
        lea    esi, [edi + 0x9c]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_178B
        jecxz  loc_1_1783
        mov    byte ptr [esi + 1], 0xb
        call   sub_1_6BE9
loc_1_1783:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_1766
loc_1_178B:
        cli    
        lea    esi, [edi + 0xac]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_17B0
        jecxz  loc_1_17A8
        mov    byte ptr [esi + 1], 0xb
        call   sub_1_6BE9
loc_1_17A8:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_178B
loc_1_17B0:
        cli    
        lea    esi, [edi + 0xb4]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_17D5
        jecxz  loc_1_17CD
        mov    byte ptr [esi + 1], 0xb
        call   sub_1_6BE9
loc_1_17CD:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        jmp    loc_1_17B0
loc_1_17D5:
        mov    esi, edi
        test   dword ptr [esi], 2
        jne    loc_1_17F0
        or     dword ptr [0x6eec], 4         ; -> obj1:0x6EEC
        call   sub_1_007C
        jae    loc_1_17F0
        mov    al, 0xf7
        ret    
loc_1_17F0:
        push   ecx
        mov    ecx, 0x100
        mov    eax, esi
        mov    esi, 0x77c0                   ; -> obj1:0x77C0
loc_1_17FD:
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     loc_1_1837
        cmp    byte ptr [edi + 0x25], 0xa
        jne    loc_1_180E
        mov    ebx, dword ptr [edi + 0x3c]
        jmp    loc_1_1818
loc_1_180E:
        mov    ebx, dword ptr [edi + 0x38]
        test   ebx, ebx
        je     loc_1_1837
        mov    ebx, dword ptr [ebx + 0x40]
loc_1_1818:
        cmp    ebx, eax
        jne    loc_1_1837
        or     word ptr [edi + 0x40], 0x40
        cmp    dword ptr [esp], 0
        jne    loc_1_182C
        or     word ptr [edi + 0x6a], 2
loc_1_182C:
        or     word ptr [edi + 0x42], 0x4000
        call   sub_1_5999
loc_1_1837:
        add    esi, 4
        loop   loc_1_17FD
        pop    ecx
        mov    ecx, 0x100
loc_1_1842:
        mov    ebx, ecx
        dec    ebx
        shl    ebx, 3
        lea    ebx, [ebx + 0x6fc0]           ; -> obj1:0x6FC0
        mov    esi, dword ptr [ebx]
loc_1_1850:
        cmp    esi, ebx
        je     loc_1_1890
        sub    esi, 0x1c
        cmp    dword ptr [esi + 0x40], eax
        jne    loc_1_187D
        mov    byte ptr [esi + 0x38], 2
        mov    dword ptr [esi + 0x3c], 2
        push   eax
        push   ecx
        push   ebx
        mov    edi, esi
        mov    byte ptr [edi + 0xb9], 0
        call   sub_1_5292
        jb     loc_1_1882
        pop    ebx
        pop    ecx
        pop    eax
loc_1_187D:
        mov    esi, dword ptr [esi + 0x1c]
        jmp    loc_1_1850
loc_1_1882:
        push   dword ptr [esi + 0x1c]
        call   sub_1_6338
        pop    esi
        pop    ebx
        pop    ecx
        pop    eax
        jmp    loc_1_1850
loc_1_1890:
        loop   loc_1_1842
        mov    esi, eax
        or     dword ptr [0x6eec], 5         ; -> obj1:0x6EEC
        sub    al, al
        ret    
loc_1_189E:
        sub    ecx, ecx
        test   byte ptr [ebx], 0x80
        je     sub_1_1671
        mov    al, 3
        ret    
loc_1_18AC:
        test   byte ptr [ebx], 0x80
        je     loc_1_18B4
        mov    al, 3
        ret    
loc_1_18B4:
        mov    ebx, dword ptr [ebx + 4]
        mov    al, byte ptr [ebx]
        and    al, 0x7f
        mov    esi, 0x6e84                   ; -> obj1:0x6E84
loc_1_18C0:
        cmp    byte ptr [esi], al
        je     loc_1_18D3
        add    esi, 5
        cmp    esi, 0x6ec0                   ; -> obj1:0x6EC0
        jb     loc_1_18C0
        mov    al, 0x26
        jmp    loc_1_18D8
loc_1_18D3:
        mov    esi, dword ptr [esi + 1]
        call   esi
loc_1_18D8:
        ret    
sub_1_18D9:
        cli    
        cmp    byte ptr [ebx + 0x25], 0
        jne    loc_1_18E1
        ret    
loc_1_18E1:
        cmp    byte ptr [ebx + 0x25], 9
        je     loc_1_18E8
        ret    
loc_1_18E8:
        test   byte ptr [ebx + 0x24], 2
        je     loc_1_1902
        and    byte ptr [ebx + 0x24], 0xfd
        push   esi
        push   edi
        lea    esi, [ebx]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_1902:
        push   esi
        test   word ptr [ebx + 0x40], 0x80
        jne    loc_1_1912
        call   sub_1_235E
        jmp    loc_1_1936
loc_1_1912:
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        mov    esi, dword ptr [ebx + 0x88]
        test   word ptr [ebx + 0x42], 8
        je     loc_1_1929
        mov    al, 0xb
loc_1_1929:
        mov    byte ptr [esi + 1], al
        call   sub_1_235E
        call   sub_1_6BE9
loc_1_1936:
        pop    esi
        ret    
loc_1_1938:
        cli    
        cmp    byte ptr [ebx + 0x25], 9
        jne    loc_1_195B
        dec    byte ptr [ebx + 0x90]
        je     loc_1_195D
        mov    eax, 0x1938                   ; -> loc_1_1938
        mov    ecx, dword ptr [ebx + 0x3c]
        call   sub_1_34D0
        sti    
        call   sub_1_3ED6
        ret    
loc_1_195B:
        sti    
        ret    
loc_1_195D:
        sti    
        mov    al, 5
        call   sub_1_18D9
        ret    
loc_1_1966:
        mov    edi, ebx
        movzx  ecx, word ptr [edi + 8]
        cmp    ecx, 0x3c
        jae    loc_1_1974
        mov    al, 1
        ret    
loc_1_1974:
        mov    ebx, edi
        cmp    byte ptr [ebx + 0xa], 0x2a
        jne    loc_1_19A5
        cmp    byte ptr [ebx + 0x1a], 0x2a
        jne    loc_1_198E
        mov    eax, dword ptr [esi + 0x40]
        mov    dword ptr [ebx + 0x1a], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 0x1e], eax
loc_1_198E:
        mov    ebx, dword ptr [edi + 4]
        sub    eax, eax
        call   sub_1_1381
        call   sub_1_146C
        mov    word ptr [edi + 8], cx
        ret    
loc_1_19A2:
        mov    al, 0x21
        ret    
loc_1_19A5:
        call   sub_1_229D
        jb     loc_1_19A2
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 0x80], edx
        mov    edx, esi
        mov    eax, edi
        lea    esi, [edi + 0xa]
        lea    edi, [ebx + 0x54]
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, eax
        mov    al, byte ptr [ebx + 0x68]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [ebx + 0x25], 9
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebx + 0x86], cx
        mov    word ptr [ebx + 0x84], 0
        mov    word ptr [ebx + 0x66], 1
        mov    dword ptr [ebx + 0x3c], edx
        test   dword ptr [edx + 4], 1
        jne    loc_1_19FD
        mov    al, 1
        jmp    loc_1_1A12
loc_1_19FD:
        test   dword ptr [edx + 4], 2
        je     loc_1_1A0D
        mov    al, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_1A12
loc_1_1A0D:
        mov    al, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_1A12:
        mov    byte ptr [ebx + 0x90], al
        cli    
        mov    dword ptr [ebx + 0x88], edi
        or     word ptr [ebx + 0x40], 0x80
        mov    eax, 0x1938                   ; -> loc_1_1938
        mov    ecx, edx
        call   sub_1_34D0
        sti    
        call   sub_1_3ED6
        mov    al, 0xff
        ret    
loc_1_1A3A:
        sub    al, al
        mov    byte ptr [ebx + 1], al
        ret    
sub_1_1A40:
        mov    al, 5
        jmp    sub_1_1329
loc_1_1A47:
        mov    cl, 1
        jmp    loc_1_1A8B
loc_1_1A4B:
        mov    cx, 3
        mov    dx, word ptr [ebx + 0xa]
        mov    ax, word ptr [ebx + 8]
        add    ax, dx
        jb     loc_1_1A5E
        jmp    loc_1_1A8D
loc_1_1A5E:
        mov    al, 1
        ret    
loc_1_1A61:
        mov    cl, 8
        jmp    loc_1_1A8B
loc_1_1A65:
        mov    cx, 2
        mov    dx, word ptr [ebx + 0xa]
        mov    ax, word ptr [ebx + 8]
        add    ax, dx
        jae    loc_1_1A8D
        mov    al, 1
        ret    
loc_1_1A79:
        ret    
loc_1_1A7A:
        and    cl, 0xfe
        jmp    loc_1_1AA2
loc_1_1A7F:
        mov    edi, eax
loc_1_1A81:
        call   sub_1_5E2C
        mov    al, 0xff
        ret    
loc_1_1A89:
        sub    ecx, ecx
loc_1_1A8B:
        xor    edx, edx
loc_1_1A8D:
        call   sub_1_5D8D
        jb     loc_1_1A79
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        test   word ptr [ebx + 0x40], 1
        je     loc_1_1A7A
loc_1_1AA2:
        mov    byte ptr [edi + 0x3f], cl
        cli    
        lea    eax, [ebx + 0x30]
        lea    esi, [edi + 0x32]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ebx + 0x34], esi
        sti    
        inc    byte ptr [ebx + 0x8d]
        mov    al, byte ptr [ebx + 0x8f]
        mov    byte ptr [edi + 0x3e], al
        test   word ptr [ebx + 0x40], 0x402
loc_1_1ACF:
        jne    loc_1_1A81
        add    dx, word ptr [edi + 8]
        cmp    dx, word ptr [ebx + 0x6e]
        ja     loc_1_1ACF
        mov    eax, edi
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_1A7F
        or     word ptr [ebx + 0x40], 0x400
        add    word ptr [esi + 0x3c], dx
        mov    word ptr [esi + 0x1e], 1
        mov    dx, word ptr [eax + 8]
        mov    word ptr [esi + 0x22], dx
        mov    dword ptr [ebx + 0x74], edx
        mov    dword ptr [esi + 0x24], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0x74], edx
        mov    dword ptr [esi + 0x24], edx
        test   cl, 2
        je     loc_1_1B25
        inc    word ptr [esi + 0x1e]
        mov    dx, word ptr [eax + 0xa]
        mov    word ptr [esi + 0x2a], dx
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x2c], edx
loc_1_1B25:
        xor    edx, edx
        mov    word ptr [esi + 0x36], dx
        mov    word ptr [esi + 0x3a], cs
        mov    word ptr [ebx + 0x72], dx
        shl    ecx, 9
        and    ch, 2
        or     byte ptr [esi + 0x11], ch
        or     byte ptr [ebx + 0x43], ch
        or     ch, 4
        cli    
        test   word ptr [ebx + 0x6a], 4
        je     loc_1_1B5D
        or     ch, 8
        dec    dword ptr [0x6eff]            ; -> loc_1_6EFF
        and    word ptr [ebx + 0x6a], 0xfff3
        mov    dx, word ptr [ebx + 0x64]
loc_1_1B5D:
        sti    
        mov    word ptr [esi + 0x38], dx
        mov    byte ptr [esi + 0x35], ch
        test   word ptr [ebx + 0x42], 1
        jne    loc_1_1BC2
loc_1_1B6D:
        mov    byte ptr [esi + 0x34], 0x16
        or     word ptr [ebx + 0x40], 0x200
        cli    
        lea    eax, [edi + 8]
        cmp    eax, dword ptr [edi + 8]
        jne    loc_1_1BCD
        call   sub_1_4F96
        sti    
        jb     loc_1_1BB8
        mov    dword ptr [esi + 0xc], edi
        call   sub_1_0517
        mov    cl, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [edi + 0xb9], cl
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     loc_1_1BAA
loc_1_1BAA:
        push   eax
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], eax
        pop    eax
        cmp    ax, 2
        ja     loc_1_1BBB
loc_1_1BB8:
        mov    al, 0xff
        ret    
loc_1_1BBB:
        and    dword ptr [edi + 0x34], 0xfffffff7
        mov    al, 0xff
        ret    
loc_1_1BC2:
        mov    byte ptr [esi + 0x36], 1
        and    word ptr [ebx + 0x42], 0xfffe
        jmp    loc_1_1B6D
loc_1_1BCD:
        push   esi
        push   eax
        lea    eax, [edi + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 0xc], eax
        pop    eax
        pop    esi
        sti    
        mov    al, 0xff
        ret    
        db  000h, 000h
sub_1_1BE8:
        mov    al, 5
        call   sub_1_1237
        jb     loc_1_1BF7
        call   sub_1_646D
        ret    
loc_1_1BF7:
        ret    
loc_1_1BF8:
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0xff
        ret    
loc_1_1C02:
        mov    edi, ebx
        call   sub_1_5D8D
        jb     loc_1_1BF8
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    byte ptr [edi + 0x3f], 0
        mov    dl, byte ptr [ebx + 0x8e]
        mov    byte ptr [edi + 0x3e], dl
        cli    
        push   esi
        lea    esi, [ebx + 0x28]
        push   edi
        lea    edi, [edi + 0x32]
        call   sub_1_287D
        pop    edi
        pop    esi
        inc    byte ptr [ebx + 0x8c]
        or     word ptr [ebx + 0x42], 0x8000
        test   byte ptr [ebx + 0x24], 0x20
        je     loc_1_1C5A
        push   esi
        push   edi
        lea    esi, [ebx + 0x18]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [ebx + 0x24], 0xdf
        call   sub_1_5CEC
loc_1_1C5A:
        sti    
        mov    al, 0xff
        ret    
        db  0C3h
loc_1_1C5F:
        mov    bl, al
        xor    bh, bh
        call   sub_1_24CC
        jae    loc_1_1C90
loc_1_1C6A:
        mov    al, 0x13
        jmp    loc_1_1D2A
loc_1_1C71:
        mov    al, 0x19
        jmp    loc_1_1D2A
loc_1_1C78:
        mov    edi, ebx
        mov    al, byte ptr [edi + 3]
        cmp    al, 0xff
        je     loc_1_1D2B
        mov    ebx, dword ptr [esi + 0x8c]
        cmp    byte ptr [ebx + 2], al
        jne    loc_1_1C5F
loc_1_1C90:
        cmp    byte ptr [ebx + 1], 2
        jne    loc_1_1C6A
        test   byte ptr [ebx], 2
        jne    loc_1_1C71
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    byte ptr [edi + 0x3f], 0
        mov    edx, edi
        mov    edi, ebx
        cli    
        add    edi, 0x3c
        cmp    edi, dword ptr [edi]
        jne    loc_1_1CCB
        lea    edx, [edx + 0x32]
        sub    edi, 8
        mov    dword ptr [edx], edi
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [ebx], edx
        mov    dword ptr [edi + 4], edx
        sti    
        inc    byte ptr [edi - 0x1b]
        mov    al, 0xff
        ret    
loc_1_1CCB:
        mov    ebx, dword ptr [edi]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        sub    ebx, 0x18
        and    byte ptr [ebx + 0x24], 0xdf
        mov    edi, edx
loc_1_1CDD:
        mov    al, byte ptr [ebx + 0x68]
        mov    byte ptr [edi + 2], al
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_1D0C
        push   esi
        lea    esi, [ebx + 0x28]
        push   edi
        lea    edi, [edi + 0x32]
        call   sub_1_287D
        pop    edi
        pop    esi
        or     word ptr [ebx + 0x42], 0x8000
        inc    byte ptr [ebx + 0x8c]
        sti    
        call   sub_1_5CEC
        jmp    loc_1_1D26
loc_1_1D0C:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        sti    
        mov    ebx, edi
        call   sub_1_5D8D
        sti    
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0xff
        jmp    loc_1_1D29
loc_1_1D26:
        sti    
        mov    al, 0xff
loc_1_1D29:
        ret    
loc_1_1D2A:
        ret    
loc_1_1D2B:
        mov    ebx, esi
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    byte ptr [edi + 0x3f], 0
        cli    
        test   dword ptr [ebx], 0x400
        jne    loc_1_1D56
loc_1_1D40:
        push   esi
        lea    esi, [ebx + 0x9c]
        push   edi
        lea    edi, [edi + 0x32]
        call   sub_1_287D
        pop    edi
        pop    esi
        sti    
        mov    al, 0xff
        ret    
loc_1_1D56:
        mov    edx, edi
        mov    esi, dword ptr [ebx + 0x84]
loc_1_1D5E:
        cmp    esi, dword ptr [ebx + 0x88]
        jae    loc_1_1D91
        cmp    byte ptr [esi + 2], 0
        je     loc_1_1D79
        lea    edi, [esi + 0x3c]
        call   sub_1_2903
        lea    edi, [edi - 0x18]
        jne    loc_1_1D7E
loc_1_1D79:
        add    esi, 0x44
        jmp    loc_1_1D5E
loc_1_1D7E:
        mov    ebx, edi
        mov    edi, edx
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 3], cl
        and    byte ptr [ebx + 0x24], 0xdf
        jmp    loc_1_1CDD
loc_1_1D91:
        and    dword ptr [ebx], 0xfffffbff
        mov    edi, edx
        jmp    loc_1_1D40
        db  000h
sub_1_1D9C:
        inc    word ptr [ebx + 0x4a]
        or     dword ptr [ebx + 0x3c], 0x80
        push   ebx
        call   sub_1_4E6C
        ret    
sub_1_1DAE:
        or     dword ptr [0x6eec], 0x78      ; -> obj1:0x6EEC
        sub    eax, eax
loc_1_1DB7:
        cmp    eax, 8
        je     loc_1_1DD0
        mov    esi, dword ptr [eax*4 + 0x6f40] ; -> obj1:0x6F40
        test   esi, esi
        je     loc_1_1DCD
        inc    dword ptr [esi + 0xcc]
loc_1_1DCD:
        inc    eax
        jmp    loc_1_1DB7
loc_1_1DD0:
        ret    
sub_1_1DD1:
        test   dword ptr [ebx + 0x34], 0x40
        je     loc_1_1DED
        cmp    byte ptr [ebx + 0xba], 0xa
        jb     loc_1_1DED
        cli    
        call   sub_1_1EE8
        sti    
        inc    dword ptr [ebx + 0x50]
loc_1_1DED:
        test   dword ptr [ebx + 0x34], 0x80
        je     loc_1_1E02
        call   sub_1_1FA8
        and    dword ptr [ebx + 0x34], 0xffff7fff
loc_1_1E02:
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [ebx + 0x64], eax
        ret    
sub_1_1E09:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_1E1F
        mov    edi, dword ptr [ebx + 0x40]
        inc    dword ptr [edi + 0x110]
        mov    edi, ebx
        call   sub_1_3F66
loc_1_1E1F:
        mov    eax, dword ptr [ebx + 0x6c]
        mov    dword ptr [ebx + 0x58], eax
        ret    
sub_1_1E26:
        cmp    byte ptr [ebx + 0x38], 1
        jne    loc_1_1E66
        mov    edi, dword ptr [ebx + 0x40]
        inc    dword ptr [edi + 0x108]
        dec    byte ptr [ebx + 0xb9]
        js     loc_1_1E48
        or     dword ptr [ebx + 0x3c], 8
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_1E48:
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [ebx + 0xb9], al
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x70], eax
        mov    dword ptr [ebx + 0x3c], 2
        push   ebx
        call   sub_1_4E6C
loc_1_1E66:
        ret    
sub_1_1E67:
        cmp    byte ptr [ebx + 0x38], 2
        jne    loc_1_1E96
        mov    edi, dword ptr [ebx + 0x40]
        or     edi, edi
        je     loc_1_1E90
        inc    dword ptr [edi + 0x108]
        dec    byte ptr [ebx + 0xb9]
        js     loc_1_1E90
        mov    dword ptr [ebx + 0x3c], 2
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_1E90:
        push   ebx
        call   sub_1_6435
loc_1_1E96:
        ret    
sub_1_1E97:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_1EE0
        mov    edi, dword ptr [ebx + 0x40]
        inc    dword ptr [edi + 0x108]
        dec    byte ptr [ebx + 0xb9]
        js     loc_1_1EC2
        call   sub_1_2038
        or     dword ptr [ebx + 0x3c], 1
        and    dword ptr [ebx + 0x34], 0xfffffff7
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_1EC2:
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [ebx + 0xb9], al
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x70], eax
        mov    dword ptr [ebx + 0x3c], 2
        push   ebx
        call   sub_1_4E6C
loc_1_1EE0:
        ret    
loc_1_1EE1:
        mov    al, 0x18
        jmp    sub_1_0C20
sub_1_1EE8:
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        mov    dl, byte ptr [0x6f2d]         ; -> loc_1_6F2D
        mov    al, byte ptr [ebx + 0x48]
        movzx  ecx, byte ptr [ebx + 0xbb]
        cmp    cl, dl
        ja     loc_1_1F37
        cmp    dword ptr [ebx + 0x68], 0x3c
        jb     loc_1_1F14
        push   ecx
        mov    ecx, dword ptr [ebx + 0x40]
        cmp    al, byte ptr [ecx + 0x134]
        pop    ecx
        jae    loc_1_1F68
loc_1_1F14:
        test   dword ptr [ebx + 0x34], 0x2000
        je     loc_1_1F68
        mov    byte ptr [ebx + 0xbc], al
        cmp    al, 0x7f
        je     loc_1_1F68
        cmp    byte ptr [ebx + 0xbd], 0xff
        je     loc_1_1F32
        jmp    loc_1_1F68
loc_1_1F32:
        inc    byte ptr [ebx + 0x48]
        jmp    loc_1_1F68
loc_1_1F37:
        and    dword ptr [ebx + 0x34], 0xffffbfff
        cmp    byte ptr [ebx + 0x48], 1
        je     loc_1_1F68
        mov    byte ptr [ebx + 0xbd], al
        cmp    al, byte ptr [ebx + 0xbc]
        jne    loc_1_1F5F
        dec    al
        mov    byte ptr [ebx + 0x48], al
        mov    byte ptr [ebx + 0xbc], al
        jmp    loc_1_1F68
loc_1_1F5F:
        mov    al, byte ptr [ebx + 0xbc]
        mov    byte ptr [ebx + 0x48], al
loc_1_1F68:
        and    dword ptr [ebx + 0x34], 0xffffdfff
        sub    ecx, ecx
        mov    byte ptr [ebx + 0xba], cl
        mov    byte ptr [ebx + 0xbb], cl
        test   dword ptr [ebx + 0x34], 0x4000
        jne    loc_1_1FA4
        inc    byte ptr [ebx + 0xc1]
        cmp    word ptr [ebx + 0xc1], 0x78
        jne    loc_1_1FA4
        mov    byte ptr [ebx + 0xc1], cl
        not    cl
        mov    byte ptr [ebx + 0xbd], cl
loc_1_1FA4:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_1FA8:
        sub    eax, eax
        mov    al, byte ptr [ebx + 0x49]
        cli    
        movzx  ecx, byte ptr [ebx + 0x56]
        cmp    word ptr [ebx + 0x4a], 0
        jne    loc_1_1FFD
        jecxz  loc_1_1FBE
        dec    byte ptr [ebx + 0x56]
loc_1_1FBE:
        cmp    byte ptr [ebx + 0x54], 0
        jne    loc_1_1FF8
        mov    byte ptr [ebx + 0xc0], al
        cmp    dword ptr [ebx + 0x68], 0x3c
        jb     loc_1_1FDD
        push   ecx
        mov    ecx, dword ptr [ebx + 0x40]
        cmp    al, byte ptr [ecx + 0x134]
        pop    ecx
        ja     loc_1_1FF0
loc_1_1FDD:
        test   dword ptr [ebx + 0x34], 0x8000
        je     loc_1_1FF0
        mov    al, 0x7f
        cmp    byte ptr [ebx + 0x49], al
        jae    loc_1_1FF0
        inc    byte ptr [ebx + 0x49]
loc_1_1FF0:
        mov    word ptr [ebx + 0x4a], 0
        sti    
        ret    
loc_1_1FF8:
        dec    byte ptr [ebx + 0x54]
        sti    
        ret    
loc_1_1FFD:
        add    ecx, 2
        cmp    ecx, 0xa
        jl     loc_1_2017
        mov    cl, byte ptr [ebx + 0x55]
        mov    byte ptr [ebx + 0x54], cl
        add    byte ptr [ebx + 0x55], 0x3c
        jae    loc_1_2015
        mov    byte ptr [ebx + 0x55], 0xff
loc_1_2015:
        xor    ecx, ecx
loc_1_2017:
        mov    byte ptr [ebx + 0x56], cl
        cmp    al, 1
        je     loc_1_1FF0
        cmp    al, byte ptr [ebx + 0xc0]
        jne    loc_1_202D
        dec    word ptr [ebx + 0xc0]
loc_1_202D:
        mov    al, byte ptr [ebx + 0xc0]
        mov    byte ptr [ebx + 0x49], al
        jmp    loc_1_1FF0
sub_1_2038:
        mov    ecx, dword ptr [ebx + 0x40]
        test   dword ptr [ecx + 4], 2
        jne    loc_1_205F
        mov    ecx, dword ptr [ebx + 0x68]
        shl    ecx, 4
        cmp    ecx, 0x3c0
        jb     loc_1_2055
        shr    ecx, 3
loc_1_2055:
        mov    eax, dword ptr [ebx + 0x70]
        cmp    eax, ecx
        jae    loc_1_205F
        add    dword ptr [ebx + 0x70], eax
loc_1_205F:
        ret    
loc_1_2060:
        mov    cl, 1
        jmp    loc_1_2066
loc_1_2064:
        sub    ecx, ecx
loc_1_2066:
        mov    edi, ebx
        movzx  edx, word ptr [edi + 8]
        cmp    edx, 0x400
        ja     loc_1_209B
        add    edx, 0x2c
        mov    bl, byte ptr [edi + 3]
        call   sub_1_24CC
        jb     loc_1_2098
        cmp    byte ptr [ebx + 1], 2
        jne    loc_1_2098
        test   byte ptr [ebx], 2
        jne    loc_1_209E
        mov    eax, esi
        call   sub_1_37A5
        jae    loc_1_20A1
        mov    al, 9
        ret    
loc_1_2098:
        mov    al, 0x13
        ret    
loc_1_209B:
        mov    al, 1
        ret    
loc_1_209E:
        mov    al, 0x19
        ret    
loc_1_20A1:
        mov    dword ptr [esi + 0x14], eax
        movzx  eax, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 0x24]
        add    edi, eax
        lea    edi, [edi + 0x1c]
        lea    esi, [ebx + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        mov    ebx, esi
        mov    edx, edi
        test   ecx, ecx
        jne    loc_1_20D7
        lea    esi, [edi + 0xa]
        mov    edi, dword ptr [ebx + 0x24]
        add    edi, eax
        lea    edi, [edi + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_20D7:
        mov    esi, ebx
        mov    edi, edx
        mov    bx, word ptr [edi + 8]
        mov    word ptr [esi + 0x2a], bx
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x2c], ebx
        mov    dword ptr [esi + 0x30], edi
        or     word ptr [esi + 0x10], 8
        inc    word ptr [esi + 0x1e]
        mov    ebx, dword ptr [esi + 0x24]
        add    ebx, eax
        mov    word ptr [ebx], 0x2c
        mov    word ptr [ebx + 2], 0xefff
        mov    byte ptr [ebx + 4], 8
        jecxz  loc_1_210F
        mov    byte ptr [ebx + 4], 9
loc_1_210F:
        sub    eax, eax
        mov    byte ptr [ebx + 5], al
        mov    word ptr [ebx + 6], ax
        mov    word ptr [ebx + 8], ax
        mov    word ptr [ebx + 0xa], ax
        mov    ebx, dword ptr [esi + 0x14]
        cli    
        push   edi
        lea    edi, [ebx + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [ebx], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        mov    al, 0xff
        ret    
loc_1_2142:
        mov    edi, ebx
        mov    bl, byte ptr [edi + 3]
        cmp    bl, 0xff
        mov    al, 1
        jne    loc_1_2159
        mov    al, 2
        jmp    loc_1_2177
loc_1_2152:
        mov    edi, ebx
        mov    bl, byte ptr [edi + 3]
        sub    al, al
loc_1_2159:
        call   sub_1_24CC
        jb     loc_1_2176
        mov    esi, ebx
        cli    
        cmp    byte ptr [esi + 1], 2
        jne    loc_1_2173
        test   byte ptr [esi], 2
        je     loc_1_2177
        sti    
        sti    
        mov    al, 0x19
        ret    
loc_1_2173:
        sti    
        mov    al, 0x13
loc_1_2176:
        ret    
loc_1_2177:
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    byte ptr [edi + 0x3f], 0
        or     al, al
        je     loc_1_21B6
        cmp    al, 1
        jne    loc_1_219F
        cli    
        push   esi
        lea    esi, [esi + 0x2c]
        push   edi
        lea    edi, [edi + 0x32]
        call   sub_1_287D
        pop    edi
        pop    esi
        sti    
        inc    byte ptr [esi + 0x18]
        jmp    loc_1_21B3
loc_1_219F:
        cli    
        push   esi
        lea    esi, [esi + 0x94]
        push   edi
        lea    edi, [edi + 0x32]
        call   sub_1_287D
        pop    edi
        pop    esi
        sti    
loc_1_21B3:
        mov    al, 0xff
        ret    
loc_1_21B6:
        cli    
        mov    ax, word ptr [edi + 8]
        inc    byte ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 4]
        lea    ecx, [ebx + 0xa4]
        cmp    ecx, dword ptr [ebx + 0xa4]
        je     loc_1_21E1
        mov    esi, dword ptr [ebx + 0xa4]
loc_1_21D5:
        cmp    ax, word ptr [esi - 0x2a]
        ja     loc_1_21F7
        mov    esi, dword ptr [esi]
        cmp    esi, ecx
        jne    loc_1_21D5
loc_1_21E1:
        push   esi
        lea    esi, [ebx + 0xa4]
        push   edi
        lea    edi, [edi + 0x32]
        call   sub_1_287D
        pop    edi
        pop    esi
loc_1_21F3:
        sti    
        mov    al, 0xff
        ret    
loc_1_21F7:
        lea    edi, [edi + 0x32]
        mov    dword ptr [edi], esi
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax], edi
        jmp    loc_1_21F3
        db  000h, 000h, 000h
sub_1_220C:
        push   eax
        push   ecx
        mov    eax, dword ptr [0x6f60]       ; -> obj1:0x6F60
        movzx  ecx, byte ptr [0x6f23]        ; -> loc_1_6F23
loc_1_221A:
        cmp    dword ptr [eax + 0x30], edi
        je     loc_1_2228
loc_1_221F:
        add    eax, 0x3e
        loop   loc_1_221A
        clc    
loc_1_2225:
        pop    ecx
        pop    eax
        ret    
loc_1_2228:
        test   word ptr [eax + 0x10], 0x8000
        jne    loc_1_221F
        stc    
        jmp    loc_1_2225
sub_1_2233:
        cmp    byte ptr [0x6ec0], 0          ; -> obj1:0x6EC0
        je     loc_1_226D
        cld    
        push   ecx
        push   edi
        push   eax
        push   0
        push   eax
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        add    esp, 8
        or     eax, eax
        je     loc_1_2267
        cld    
        pop    ecx
        push   eax
        mov    edi, eax
        xor    eax, eax
        push   ecx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        pop    ecx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        pop    eax
        pop    edi
        pop    ecx
        ret    
loc_1_2267:
        add    esp, 4
        pop    edi
        pop    ecx
        ret    
loc_1_226D:
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        ret    
sub_1_227A:
        cmp    byte ptr [0x6ec0], 0          ; -> obj1:0x6EC0
        je     loc_1_2290
        push   0
        push   eax
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        add    esp, 8
        ret    
loc_1_2290:
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        ret    
sub_1_229D:
        push   edx
        mov    ecx, 0xfe
        movzx  edx, byte ptr [0x6f34]        ; -> obj1:0x6F34
        sub    eax, eax
        cli    
loc_1_22AD:
        cmp    edx, 0xfe
        jae    loc_1_22C5
loc_1_22B5:
        cmp    eax, dword ptr [edx*4 + 0x77c0] ; -> obj1:0x77C0
        je     loc_1_22C9
        inc    edx
        loop   loc_1_22AD
loc_1_22C1:
        sti    
        pop    edx
        stc    
        ret    
loc_1_22C5:
        sub    edx, edx
        jmp    loc_1_22B5
loc_1_22C9:
        push   edx
        mov    eax, 0xd0
        call   sub_1_2233
        pop    edx
        test   eax, eax
        je     loc_1_22C1
        mov    ebx, eax
        sub    eax, eax
        mov    dword ptr [edx*4 + 0x77c0], ebx ; -> obj1:0x77C0
        mov    byte ptr [ebx + 0x25], 0
        sti    
        mov    word ptr [ebx + 0x40], ax
        mov    word ptr [ebx + 0x42], ax
        mov    word ptr [ebx + 0x6a], ax
        mov    dword ptr [ebx + 0x3c], eax
        mov    dword ptr [ebx + 0x38], eax
        mov    byte ptr [ebx + 0x24], al
        mov    word ptr [ebx + 0x64], ax
        mov    word ptr [ebx + 0x66], ax
        mov    byte ptr [ebx + 0x69], al
        mov    dword ptr [ebx + 0x88], eax
        mov    byte ptr [ebx + 0x8d], al
        mov    byte ptr [ebx + 0x8c], al
        mov    byte ptr [ebx + 0x6d], al
        mov    ax, 0x1000
        mov    word ptr [ebx + 0x6e], ax
        mov    al, byte ptr [0x6f2e]         ; -> obj1:0x6F2E
        mov    byte ptr [ebx + 0x6c], al
        cli    
        mov    eax, dword ptr [0x6fbc]       ; -> obj1:0x6FBC
        mov    dword ptr [ebx + 0x20], eax
        mov    dword ptr [0x6fbc], ebx       ; -> obj1:0x6FBC
        sti    
        inc    dl
        mov    byte ptr [0x6f34], dl         ; -> obj1:0x6F34
        mov    byte ptr [ebx + 0x68], dl
        lea    eax, [ebx + 0x28]
        mov    dword ptr [ebx + 0x28], eax
        mov    dword ptr [ebx + 0x2c], eax
        lea    eax, [ebx + 0x30]
        mov    dword ptr [ebx + 0x30], eax
        mov    dword ptr [ebx + 0x34], eax
        clc    
        pop    edx
        ret    
sub_1_235E:
        mov    word ptr [ebx + 0x40], 0x2000
        push   ebx
        call   sub_1_5BF3
        ret    
sub_1_236B:
        push   ecx
        push   edx
        cli    
        pushfd 
        cmp    dword ptr [eax + 0x28], 1
        je     loc_1_23AC
        movzx  ebx, word ptr [edi + 0xa]
        xor    bl, bh
        sub    bh, bh
        lea    edx, [ebx*8 + 0x6fc0]         ; -> obj1:0x6FC0
        mov    ebx, dword ptr [edx]
loc_1_2386:
        cmp    edx, ebx
        je     loc_1_23DF
        cmp    eax, dword ptr [ebx + 0x24]
        jne    loc_1_23A8
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, dword ptr [ebx + 0x72]
        jne    loc_1_23A8
        mov    cx, word ptr [edi + 6]
        cmp    cx, word ptr [ebx + 0x70]
        jne    loc_1_23A8
loc_1_23A1:
        popfd  
        pop    edx
        pop    ecx
        sub    ebx, 0x1c
        ret    
loc_1_23A8:
        mov    ebx, dword ptr [ebx]
        jmp    loc_1_2386
loc_1_23AC:
        movzx  ebx, word ptr [edi + 0xc]
        xor    bl, bh
        sub    bh, bh
        lea    edx, [ebx*8 + 0x6fc0]         ; -> obj1:0x6FC0
        mov    ebx, dword ptr [edx]
loc_1_23BD:
        cmp    edx, ebx
        je     loc_1_23DF
        cmp    eax, dword ptr [ebx + 0x24]
        jne    loc_1_23DB
        mov    ecx, dword ptr [edi + 0xa]
        cmp    ecx, dword ptr [ebx + 0x74]
        jne    loc_1_23DB
        mov    cx, word ptr [edi + 8]
        and    cl, 0x7f
        cmp    cx, word ptr [ebx + 0x72]
        je     loc_1_23A1
loc_1_23DB:
        mov    ebx, dword ptr [ebx]
        jmp    loc_1_23BD
loc_1_23DF:
        mov    ebx, edx
        popfd  
        pop    edx
        pop    ecx
        stc    
        ret    
sub_1_23E6:
        push   ebx
        push   esi
        push   edi
        mov    edi, esi
        mov    ebx, edi
        push   ecx
        mov    ecx, esp
        call   sub_1_14FC
        pop    ecx
loc_1_23F6:
        cli    
        lea    ebx, [edi + 0x1c]
        call   sub_1_28F3
        sti    
        je     loc_1_241A
        dec    byte ptr [edi + 3]
        mov    esi, dword ptr [ebx + 0x88]
        call   sub_1_235E
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        jmp    loc_1_23F6
loc_1_241A:
        cli    
        lea    ebx, [edi + 0x24]
        call   sub_1_28F3
        sti    
        je     loc_1_243E
        dec    byte ptr [edi + 3]
        mov    esi, dword ptr [ebx + 0x88]
        call   sub_1_235E
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        jmp    loc_1_241A
loc_1_243E:
        cli    
        lea    esi, [edi + 0x2c]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_245D
        mov    byte ptr [esi + 1], al
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        call   sub_1_6BE9
        jmp    loc_1_243E
loc_1_245D:
        cli    
        lea    esi, [edi + 0x34]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_247C
        mov    byte ptr [esi + 1], al
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        call   sub_1_6BE9
        jmp    loc_1_245D
loc_1_247C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2480:
        push   edi
        cli    
        cld    
        mov    ebx, dword ptr [eax + 0x8c]
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_249D
        mov    ecx, 4
        push   esi
        lea    edi, [ebx + 8]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    esi
        je     loc_1_24C4
loc_1_249D:
        mov    ebx, dword ptr [eax + 0x84]
loc_1_24A3:
        cmp    ebx, dword ptr [eax + 0x88]
        ja     loc_1_24C8
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_24BF
        mov    ecx, 4
        push   esi
        lea    edi, [ebx + 8]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    esi
        je     loc_1_24C4
loc_1_24BF:
        add    ebx, 0x44
        jmp    loc_1_24A3
loc_1_24C4:
        sti    
        pop    edi
        clc    
        ret    
loc_1_24C8:
        sti    
        pop    edi
        stc    
        ret    
sub_1_24CC:
        test   bl, bl
        je     loc_1_2506
        cmp    bl, 0xff
        je     loc_1_2506
        push   edi
        cli    
        mov    edi, dword ptr [esi + 0x8c]
        cmp    byte ptr [edi + 2], bl
        jne    loc_1_24EE
loc_1_24E2:
        sti    
        mov    ebx, edi
        mov    dword ptr [esi + 0x8c], ebx
        pop    edi
        clc    
        ret    
loc_1_24EE:
        mov    edi, dword ptr [esi + 0x84]
loc_1_24F4:
        cmp    byte ptr [edi + 2], bl
        je     loc_1_24E2
        add    edi, 0x44
        cmp    edi, dword ptr [esi + 0x88]
        jbe    loc_1_24F4
        sti    
        pop    edi
loc_1_2506:
        mov    al, 0x13
        stc    
        ret    
sub_1_250A:
        cmp    byte ptr [esi + 3], 0
        jne    loc_1_2521
        push   ebx
        mov    ebx, dword ptr [esi + 4]
        dec    byte ptr [ebx + 0x91]
        mov    byte ptr [esi + 2], 0
        pop    ebx
        clc    
        ret    
loc_1_2521:
        mov    byte ptr [esi + 1], 3
        stc    
        ret    
sub_1_2527:
        push   esi
        push   ebx
        mov    esi, edi
        call   sub_1_2480
        jb     loc_1_254D
        mov    esi, ebx
        cmp    byte ptr [esi + 2], 1
        je     loc_1_254C
        mov    al, 0x17
        call   sub_1_23E6
        call   sub_1_250A
        mov    al, 0xf
        jb     loc_1_254C
        sub    al, al
loc_1_254C:
        clc    
loc_1_254D:
        pop    ebx
        pop    esi
        ret    
sub_1_2550:
        cmp    al, 8
        jae    loc_1_2563
        movzx  eax, al
        mov    esi, dword ptr [eax*4 + 0x6f40] ; -> obj1:0x6F40
        test   esi, esi
        je     loc_1_2563
        ret    
loc_1_2563:
        mov    al, 0x23
        stc    
        ret    
loc_1_2567:
        adc    al, byte ptr [edx]
        ljmp   0xd11:0x86081205
        db  020h
sub_1_2571:
        cmp    dword ptr [eax + 0x28], 1
        je     loc_1_25A2
        mov    edx, dword ptr [edi + 6]
        mov    dword ptr [esi], edx
        mov    dx, word ptr [edi + 0xa]
        mov    word ptr [esi + 4], dx
        mov    edx, dword ptr [eax + 0x31]
        mov    dword ptr [esi + 6], edx
        mov    dx, word ptr [eax + 0x35]
        mov    word ptr [esi + 0xa], dx
        mov    dx, word ptr [edi + 0xc]
        mov    word ptr [esi + 0xc], dx
        sub    edx, edx
        mov    ecx, 0xe
        ret    
loc_1_25A2:
        mov    dh, byte ptr [edi + 1]
        mov    dl, 0x10
        mov    word ptr [esi], dx
        mov    edx, dword ptr [edi + 8]
        mov    ecx, edx
        and    dl, 0x7f
        and    cl, 0x80
        mov    dword ptr [esi + 2], edx
        mov    dx, word ptr [edi + 0xc]
        mov    word ptr [esi + 6], dx
        mov    edx, dword ptr [eax + 0x31]
        or     dl, cl
        mov    dword ptr [esi + 8], edx
        mov    dx, word ptr [eax + 0x35]
        mov    word ptr [esi + 0xc], dx
        xor    ecx, ecx
        xor    edx, edx
        test   byte ptr [edi + 8], 0x80
        je     loc_1_261F
        mov    cl, byte ptr [edi + 0xe]
        and    ecx, 0x1f
        cmp    ecx, 2
        jbe    loc_1_2623
        push   ebx
        mov    bl, byte ptr [edi + 0xf]
        and    ebx, 0x70
        cmp    bl, 0x50
        jae    loc_1_25FD
        shr    ebx, 3
        movzx  edx, word ptr [ebx + 0x2567]  ; -> loc_1_2567
        add    edx, ecx
loc_1_25FD:
        push   eax
        push   ecx
        push   esi
        push   edi
        xchg   edi, esi
        shr    ecx, 1
        lea    esi, [esi + 0xe]
        lea    edi, [edi + 0xe]
        cld    
        lodsw  ax, word ptr [esi]
        and    al, 0x1f
        stosw  word ptr es:[edi], ax
        dec    ecx
        rep movsw word ptr es:[edi], word ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        xor    byte ptr [esi + 0xf], 0x80
        pop    ebx
loc_1_261F:
        add    ecx, 0xe
        ret    
loc_1_2623:
        sub    ecx, ecx
        and    byte ptr [esi + 8], 0x7f
        jmp    loc_1_261F
sub_1_262B:
        push   esi
        xor    eax, eax
        lea    edi, [edi + 0x31]
        mov    ecx, 3
        push   ecx
        cld    
        repe cmpsw word ptr [esi], word ptr es:[edi]
        pop    ecx
        je     loc_1_2645
        pop    edi
        repe scasw ax, word ptr es:[edi]
        je     loc_1_2652
        ret    
loc_1_2645:
        mov    edi, esi
        rep stosw word ptr es:[edi], ax
        mov    byte ptr [ebx + 0xb6], 0xe
        pop    eax
loc_1_2652:
        or     dword ptr [ebx + 0x34], 0x400
        ret    
sub_1_265A:
        cmp    eax, dword ptr [0x6de8]       ; -> obj1:0x6DE8
        jae    loc_1_2666
        sub    eax, eax
        jmp    loc_1_2670
loc_1_2666:
        push   edx
        sub    edx, edx
        idiv   dword ptr [0x6de8]            ; -> obj1:0x6DE8
        pop    edx
loc_1_2670:
        ret    
sub_1_2671:
        test   dword ptr [ebx + 4], 1
        jne    loc_1_267F
        xor    eax, eax
        mov    ecx, eax
        ret    
loc_1_267F:
        push   edx
        mov    ecx, dword ptr [ebx + 0x40]
        movzx  edx, word ptr [ebx + 0x92]
        imul   eax, edx, 0x1f40
        sub    edx, edx
        idiv   ecx
        test   edx, edx
        je     loc_1_2699
        inc    eax
loc_1_2699:
        movzx  ecx, byte ptr [ebx + 0x134]
        imul   ecx, eax
        pop    edx
        ret    
sub_1_26A5:
        push   eax
        push   edi
        sub    eax, eax
        mov    dword ptr [esi + 0x50], eax
        mov    dword ptr [esi + 0x3c], eax
        mov    byte ptr [esi + 0x44], al
        mov    byte ptr [esi + 0x81], al
        mov    byte ptr [esi + 0x45], al
        mov    byte ptr [esi + 0x46], al
        mov    byte ptr [esi + 0x47], al
        mov    word ptr [esi + 0x78], 0xf0f0
        mov    word ptr [esi + 0x4a], ax
        mov    byte ptr [esi + 0x39], al
        push   ebx
        mov    ebx, dword ptr [esi + 0x40]
        call   sub_1_2671
        test   dword ptr [ebx + 4], 2
        jne    loc_1_26E2
        shl    eax, 1
loc_1_26E2:
        call   sub_1_265A
        xchg   ecx, eax
        call   sub_1_265A
        xchg   ecx, eax
        add    eax, dword ptr [0x6f0b]       ; -> loc_1_6F0B
        add    eax, ecx
        pop    ebx
        mov    dword ptr [esi + 0x68], eax
        mov    dword ptr [esi + 0x70], eax
        mov    eax, dword ptr [0x6f07]       ; -> loc_1_6F07
        mov    dword ptr [esi + 0x6c], eax
        mov    eax, dword ptr [0x6f0f]       ; -> loc_1_6F0F
        mov    dword ptr [esi + 0x74], eax
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [esi + 0xb9], al
        mov    dword ptr [esi + 0x58], 0xffffffff
        mov    dword ptr [esi + 0x5c], 0xffffffff
        mov    dword ptr [esi + 0x60], 0xffffffff
        mov    dword ptr [esi + 0x64], 0xffffffff
        mov    al, byte ptr [0x6f2b]         ; -> loc_1_6F2B
        mov    byte ptr [esi + 0x49], al
        push   ebx
        mov    ebx, dword ptr [esi + 0x40]
        mov    eax, dword ptr [ebx + 8]
        mov    byte ptr [esi + 0x48], al
        pop    ebx
        push   ecx
        mov    al, 0xff
        xor    ecx, ecx
        mov    byte ptr [esi + 0xbc], al
        mov    byte ptr [esi + 0xbd], al
        mov    byte ptr [esi + 0xba], cl
        mov    byte ptr [esi + 0xbb], cl
        mov    byte ptr [esi + 0xc1], cl
        mov    byte ptr [esi + 0xc0], cl
        mov    word ptr [esi + 0x4a], cx
        mov    byte ptr [esi + 0x56], cl
        mov    byte ptr [esi + 0x54], cl
        mov    byte ptr [esi + 0x55], 0x3c
        pop    ecx
        mov    eax, dword ptr [0x6f13]       ; -> loc_1_6F13
        mov    dword ptr [esi + 0x4c], eax
        or     eax, eax
        je     loc_1_27AD
        push   ebx
        mov    ebx, dword ptr [esi + 0x40]
        test   dword ptr [ebx + 4], 2
        jne    loc_1_27A1
        or     dword ptr [esi + 0x34], 0x40c0
        jmp    loc_1_27AC
loc_1_27A1:
        or     dword ptr [esi + 0x34], 0x4000
        mov    byte ptr [esi + 0x49], 1
loc_1_27AC:
        pop    ebx
loc_1_27AD:
        pop    edi
        pop    eax
        ret    
sub_1_27B0:
        push   edi
        push   eax
        push   ebx
        mov    dword ptr [esi + 0x40], eax
        mov    ebx, eax
        cli    
        lea    eax, [esi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        lea    eax, [esi + 0x10]
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [esi + 0x14], eax
        lea    eax, [esi + 8]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0xc], eax
        xor    eax, eax
        mov    dword ptr [esi + 0x34], eax
        mov    byte ptr [esi + 0xb8], al
        mov    byte ptr [esi + 0x38], 0
        call   sub_1_26A5
        pop    ebx
        push   edi
        lea    edi, [ebx]
        push   esi
        lea    esi, [esi + 0x1c]
        call   sub_1_2868
        pop    esi
        pop    edi
        or     byte ptr [esi + 0xb8], 2
        mov    eax, dword ptr [0x6fb8]       ; -> obj1:0x6FB8
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [0x6fb8], esi       ; -> obj1:0x6FB8
        mov    ebx, esi
        pop    eax
        push   esi
        push   ecx
        lea    esi, [ebx + 0x8c]
        call   sub_1_2571
        mov    edi, eax
        push   eax
        movzx  eax, word ptr [edi + 0x92]
        or     edx, edx
        je     loc_1_2830
        sub    eax, edx
        sbb    esi, esi
        and    eax, esi
        add    eax, edx
loc_1_2830:
        mov    word ptr [ebx + 0x7a], ax
        mov    byte ptr [ebx + 0xb6], cl
        mov    eax, 0xc
        lea    esi, [ebx + 0x8c]
        cmp    dword ptr [edi + 0x28], 0
        je     loc_1_2856
        mov    eax, 0x32
        lea    esi, [ebx + 0x8e]
loc_1_2856:
        mov    byte ptr [ebx + 0xb7], al
        call   sub_1_262B
        pop    eax
        pop    ecx
        pop    esi
        sti    
        pop    edi
        clc    
        ret    
sub_1_2868:
        push   esi
        push   eax
        lea    eax, [edi]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 4], eax
        pop    eax
        pop    esi
        ret    
sub_1_287D:
        push   edi
        push   eax
        lea    eax, [esi]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 4], eax
        pop    eax
        pop    edi
        ret    
sub_1_2892:
        push   ebx
        push   eax
        lea    eax, [esi]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi + 4], eax
        pop    eax
        pop    ebx
        ret    
sub_1_28A7:
        push   eax
        push   esi
        lea    eax, [edi]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        xchg   esi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 4], eax
        pop    esi
        pop    eax
        ret    
sub_1_28BB:
        push   eax
        push   edi
        lea    eax, [esi]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        xchg   edi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 4], eax
        pop    edi
        pop    eax
        ret    
sub_1_28CF:
        push   eax
        push   ebx
        lea    eax, [esi]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx], eax
        xchg   ebx, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [ebx + 4], eax
        pop    ebx
        pop    eax
        ret    
sub_1_28E3:
        push   edi
        mov    edi, dword ptr [esi]
        push   edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    esi
        cmp    esi, edi
        pop    edi
        ret    
sub_1_28F3:
        push   edi
        mov    edi, dword ptr [ebx]
        push   edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [ebx], edi
        mov    dword ptr [edi + 4], ebx
        pop    ebx
        cmp    ebx, edi
        pop    edi
        ret    
sub_1_2903:
        push   esi
        mov    esi, dword ptr [edi]
        push   esi
        mov    esi, dword ptr [esi]
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 4], edi
        pop    edi
        cmp    edi, esi
        pop    esi
        ret    
sub_1_2913:
        mov    byte ptr [edi + 1], al
        push   edi
        push   eax
        lea    eax, [0x6fa8]                 ; -> obj1:0x6FA8
        lea    edi, [edi + 0x32]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [0x6fac]       ; -> obj1:0x6FAC
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [0x6fac], eax       ; -> obj1:0x6FAC
        pop    eax
        pop    edi
        or     dword ptr [0x6eec], 0x40      ; -> obj1:0x6EEC
        ret    
        db  0E8h, 02Fh, 0FDh, 0FFh, 0FFh, 0D1h, 0E0h, 0E8h, 011h, 0FDh, 0FFh, 0FFh, 091h, 0E8h, 00Bh, 0FDh
        db  0FFh, 0FFh, 091h, 003h, 005h
        dd  loc_1_6F0B               ; fixup
        db  003h, 0C1h, 0C3h
sub_1_2959:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    edi, eax
        movzx  ecx, word ptr [esi + 0xca]
        test   ecx, ecx
        je     loc_1_29AB
        mov    eax, 1
        shl    eax, cl
        mov    ecx, eax
        test   ecx, ecx
        jne    loc_1_2979
        dec    ecx
loc_1_2979:
        movzx  edx, word ptr [edi + 0x92]
        imul   eax, edx, 0x1f40
        sub    edx, edx
        idiv   ecx
        test   edx, edx
        je     loc_1_298F
        inc    eax
loc_1_298F:
        shl    eax, 1
        call   sub_1_265A
        mov    ecx, eax
        movzx  eax, byte ptr [edi + 0x134]
        imul   ecx, eax
        add    ecx, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x68], ecx
        mov    dword ptr [ebx + 0x70], ecx
loc_1_29AB:
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_1_29B1:
        push   ebx
        mov    bl, 0x1f
loc_1_29B4:
        clc    
        shl    eax, 1
        jb     loc_1_29BD
        dec    bl
        jne    loc_1_29B4
loc_1_29BD:
        xor    eax, eax
        mov    al, bl
        pop    ebx
        ret    
        db  000h
sub_1_29C4:
        cmp    al, byte ptr [eax + eax]
        pop    ebp
        inc    esp
        add    byte ptr [eax], al
        int    0x44
        add    byte ptr [eax], al
        sub    dword ptr [edx], eax
        add    byte ptr [ecx + 0x31], bl     ; -> loc_1_3159
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [ecx + 0x31], bl     ; -> loc_1_3159
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        add    bl, cl                        ; -> loc_1_4CCB
        dec    esp
        add    byte ptr [eax], al
        or     dword ptr [ebp], 0x455400     ; -> loc_1_4554
        add    byte ptr [ecx + 0x31], bl     ; -> loc_1_3159
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [ebx + 0x44], bh     ; -> loc_1_447B
        add    byte ptr [eax], al
        mov    dword ptr [edi], 0x434900     ; -> loc_1_4349
        add    byte ptr [ecx + 0x31], bl     ; -> loc_1_3159
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [ecx + 0x31], bl     ; -> loc_1_3159
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [ebp + 0x49], cl     ; -> loc_1_494D
        add    byte ptr [eax], al
        pop    ebx
        dec    ecx
        add    byte ptr [eax], al
        push   ebx
        dec    edx
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        inc    eax
        add    byte ptr [eax], al
        push   ebp
        inc    ecx
        add    byte ptr [eax], al
        mov    word ptr [ecx], es
        add    byte ptr [eax], dh            ; -> loc_1_4C30
        dec    esp
        add    byte ptr [eax], al
        or     byte ptr [eax + eax], 0x64    ; -> loc_1_4C64
        dec    esp
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [ecx + 0x31], bl     ; -> loc_1_3159
        add    byte ptr [eax], al
        cdq    
        inc    eax
        add    byte ptr [eax], al
sub_1_2A44:
        add    edi, esi
        sub    edi, ebx
        cmp    word ptr [eax + 0x76], di
        ret    
loc_1_2A4D:
        ret    
sub_1_2A4E:
        mov    dl, 0xf
        test   dword ptr [eax], 2
        je     loc_1_2A4D
        test   byte ptr [esi + 2], 3
        jne    loc_1_2A6F
        test   byte ptr [esi + 1], 1
        jne    loc_1_2A4D
        test   byte ptr [esi + 2], 1
        je     loc_1_2A4D
        or     dl, 0x10
        jmp    loc_1_2A78
loc_1_2A6F:
        test   byte ptr [esi + 2], 0x10
        je     loc_1_2A78
        or     dl, 0x10
loc_1_2A78:
        cli    
        lea    ebx, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_28F3
        sti    
        je     loc_1_2ADF
        mov    dword ptr [ebx + 0x14], eax
        lea    esi, [ebx + 0x34]
        mov    dword ptr [ebx + 0x24], esi
        mov    word ptr [ebx + 0x1e], 1
        mov    word ptr [ebx + 0x10], 2
        push   edx
        call   sub_1_2571
        pop    edx
        add    esi, ecx
        mov    byte ptr [esi], 0xf0
        mov    byte ptr [esi + 1], 0xf1
        mov    byte ptr [esi + 2], dl
        add    ecx, 3
        mov    word ptr [ebx + 0x22], cx
        cmp    dword ptr [eax + 0x28], 1
        je     loc_1_2AC2
        mov    word ptr [esi - 2], 0x300
loc_1_2AC2:
        cli    
        push   esi
        lea    esi, [eax + 0xbc]
        call   sub_1_2892
        pop    esi
        sti    
        or     dword ptr [eax], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        ret    
loc_1_2ADF:
        ret    
sub_1_2AE0:
        push   eax
        movzx  eax, word ptr [ebx + 0x44]
        shl    eax, 1
        test   byte ptr [ebx + 0x80], 0x40
        jne    loc_1_2B10
        test   byte ptr [esi + 1], 1
        je     loc_1_2B01
        cmp    byte ptr [ebx + 0x38], 3
        je     loc_1_2B42
loc_1_2AFC:
        or     ah, 1
        jmp    loc_1_2B23
loc_1_2B01:
        test   byte ptr [esi + 3], 1
        je     loc_1_2B23
        or     byte ptr [ebx + 0x80], 0x80
        jmp    loc_1_2B23
loc_1_2B10:
        test   byte ptr [esi + 1], 1
        jne    loc_1_2AFC
        test   byte ptr [esi + 2], 0x10
        je     loc_1_2B23
        or     byte ptr [ebx + 0x80], 0x80
loc_1_2B23:
        cli    
        mov    dword ptr [ebx + 0x3c], 0x40
        mov    word ptr [ebx + 0x7e], ax
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebx + 0x7c], ax
        mov    byte ptr [ebx + 0x38], 3
        sti    
        push   ebx
        call   sub_1_4E6C
loc_1_2B42:
        pop    eax
        ret    
sub_1_2B44:
        mov    edi, edx
        call   sub_1_236B
        jb     loc_1_2B83
        mov    cl, byte ptr [esi + 2]
        shr    cl, 1
        cmp    cl, byte ptr [ebx + 0x45]
        jne    loc_1_2B8A
        and    dword ptr [ebx + 0x34], 0xfffffffb
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        jb     loc_1_2B88
        cli    
        inc    byte ptr [ebx + 0x45]
        and    byte ptr [ebx + 0x45], 0x7f
        mov    cl, byte ptr [ebx + 0x47]
        add    cl, byte ptr [ebx + 0x49]
        and    cl, 0x7f
        cmp    cl, byte ptr [ebx + 0x45]
        je     loc_1_2BAD
        sti    
loc_1_2B7C:
        mov    eax, dword ptr [ebx + 0x74]
        mov    dword ptr [ebx + 0x60], eax
        ret    
loc_1_2B83:
        call   sub_1_2A4E
loc_1_2B88:
        sti    
        ret    
loc_1_2B8A:
        test   dword ptr [ebx + 0x34], 4
        jne    loc_1_2B9B
        or     dword ptr [ebx + 0x3c], 0x20
        or     dword ptr [ebx + 0x34], 4
loc_1_2B9B:
        test   byte ptr [esi + 3], 1
        je     loc_1_2BA5
        or     dword ptr [ebx + 0x3c], 0x10
loc_1_2BA5:
        sti    
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_2BAD:
        or     dword ptr [ebx + 0x3c], 0x80
        sti    
        push   ebx
        call   sub_1_4E6C
        jmp    loc_1_2B7C
sub_1_2BBD:
        push   ecx
        cmp    bl, 3
        je     loc_1_2BE0
        cmp    dword ptr [eax + 0x28], 1
        je     loc_1_2BE3
        test   byte ptr [edx], 1
        je     loc_1_2BE0
        mov    ecx, dword ptr [eax + 0x31]
        cmp    ecx, dword ptr [edx + 6]
        jne    loc_1_2BE0
        mov    cx, word ptr [eax + 0x35]
        cmp    cx, word ptr [edx + 0xa]
        je     loc_1_2BFE
loc_1_2BE0:
        clc    
        pop    ecx
        ret    
loc_1_2BE3:
        test   byte ptr [edx + 2], 0x80
        je     loc_1_2BE0
        mov    ecx, dword ptr [edx + 8]
        and    cl, 0x7f
        cmp    ecx, dword ptr [eax + 0x31]
        jne    loc_1_2BE0
        mov    cx, word ptr [eax + 0x35]
        cmp    cx, word ptr [edx + 0xc]
        jne    loc_1_2BE0
loc_1_2BFE:
        stc    
        pop    ecx
        ret    
sub_1_2C01:
        mov    word ptr [esi + 0x22], cx
        push   ebx
        mov    ebx, dword ptr [esi + 0x14]
        cli    
        push   edi
        lea    edi, [ebx + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [ebx], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        pop    ebx
        ret    
sub_1_2C27:
        push   edx
        push   edi
        push   esi
        lea    eax, [ebx + 8]
        mov    esi, dword ptr [ebx + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    edx, esi
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 0x14]
        mov    dword ptr [edi], esi
        cmp    esi, eax
        je     loc_1_2C46
        mov    dword ptr [esi + 4], edi
        jmp    loc_1_2C49
loc_1_2C46:
        mov    dword ptr [ebx + 0xc], edi
loc_1_2C49:
        mov    dword ptr [ebx + 8], edx
        lea    eax, [ebx + 0x10]
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [ebx + 0x14], eax
        movzx  eax, byte ptr [ebx + 0x44]
        sub    al, byte ptr [ebx + 0x46]
        and    al, 0x7f
        mov    edi, dword ptr [ebx + 0x40]
        add    dword ptr [edi + 0xf8], eax
        or     al, al
        je     loc_1_2C84
        add    byte ptr [ebx + 0xbb], al
        inc    byte ptr [ebx + 0x39]
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        cmp    al, byte ptr [ebx + 0x39]
        jb     loc_1_2C88
        mov    al, byte ptr [ebx + 0x46]
        mov    byte ptr [ebx + 0x44], al
loc_1_2C84:
        pop    esi
        pop    edi
        pop    edx
        ret    
loc_1_2C88:
        or     dword ptr [ebx + 0x3c], 2
        mov    byte ptr [ebx + 0x38], 2
        jmp    loc_1_2C84
loc_1_2C92:
        cmp    byte ptr [ebx + 0x38], 3
        je     loc_1_2D7A
        test   dword ptr [ebx + 0x34], 0x1000
        je     loc_1_2CB7
        and    dword ptr [ebx + 0x34], 0xffffefff
        mov    byte ptr [ebx + 0x38], 4
        call   sub_1_6104
        jmp    loc_1_2CC5
loc_1_2CB7:
        push   ebx
        call   sub_1_4E6C
        stc    
        ret    
sub_1_2CBF:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_2C92
loc_1_2CC5:
        shr    al, 1
        pushfd 
        cmp    al, byte ptr [ebx + 0x46]
        je     loc_1_2CFD
        push   ecx
        cli    
        mov    cl, byte ptr [ebx + 0x81]
        mov    ah, byte ptr [ebx + 0x46]
        sub    cl, ah
        mov    ch, al
        sub    ch, ah
        and    cx, 0x7f7f
        cmp    ch, cl
        jbe    loc_1_2CF2
        cmp    ch, 0x7f
        jne    loc_1_2D88
        mov    al, ah
loc_1_2CF2:
        sti    
        pop    ecx
        mov    byte ptr [ebx + 0x39], 0
        call   sub_1_617A
loc_1_2CFD:
        popfd  
        jb     loc_1_2D9A
loc_1_2D04:
        test   dword ptr [ebx + 0x34], 0x100
        jne    loc_1_2E13
loc_1_2D11:
        mov    al, byte ptr [ebx + 0x46]
        cmp    al, byte ptr [ebx + 0x44]
        jne    loc_1_2D3C
        test   dword ptr [ebx + 0x34], 8
        jne    loc_1_2D3C
        mov    dword ptr [ebx + 0x5c], 0xffffffff
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [ebx + 0xb9], al
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x70], eax
        clc    
        ret    
loc_1_2D3C:
        cli    
        test   byte ptr [ebx + 0xb8], 1
        jne    loc_1_2D78
        or     byte ptr [ebx + 0xb8], 1
        mov    edx, dword ptr [ebx + 0x40]
        push   ebx
        push   eax
        lea    eax, [edx + 0xc4]
        lea    ebx, [ebx + 0x24]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 0xc8]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx + 0xc8], eax
        pop    eax
        pop    ebx
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
loc_1_2D78:
        clc    
        ret    
loc_1_2D7A:
        mov    byte ptr [ebx + 0x80], 1
        call   sub_1_2AE0
        stc    
        ret    
loc_1_2D88:
        pop    ecx
        add    esp, 4
        mov    byte ptr [ebx + 0x80], 8
        call   sub_1_2AE0
        stc    
        ret    
loc_1_2D9A:
        test   byte ptr [esi + 1], 1
        jne    loc_1_2DEE
        cli    
        or     dword ptr [ebx + 0x3c], 0x10
        test   byte ptr [ebx + 0xb8], 1
        jne    loc_1_2D11
        or     byte ptr [ebx + 0xb8], 1
        mov    edx, dword ptr [ebx + 0x40]
        push   ebx
        push   eax
        lea    eax, [edx + 0xc4]
        lea    ebx, [ebx + 0x24]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 0xc8]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx + 0xc8], eax
        pop    eax
        pop    ebx
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        jmp    loc_1_2D11
loc_1_2DE9:
        jmp    loc_1_2D04
loc_1_2DEE:
        test   dword ptr [ebx + 0x34], 8
        je     loc_1_2DE9
        and    dword ptr [ebx + 0x34], 0xffffffd7
        mov    dword ptr [ebx + 0x5c], 0xffffffff
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [ebx + 0xb9], al
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x70], eax
loc_1_2E13:
        cli    
        lea    eax, [ebx + 0x10]
        cmp    dword ptr [ebx + 0x10], eax
        je     loc_1_2E21
        call   sub_1_2C27
loc_1_2E21:
        test   byte ptr [ebx + 0xb8], 1
        jne    loc_1_2E5C
        or     byte ptr [ebx + 0xb8], 1
        mov    edx, dword ptr [ebx + 0x40]
        push   ebx
        push   eax
        lea    eax, [edx + 0xc4]
        lea    ebx, [ebx + 0x24]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 0xc8]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx + 0xc8], eax
        pop    eax
        pop    ebx
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
loc_1_2E5C:
        clc    
        ret    
        db  08Bh, 0FBh, 0E8h, 006h, 0F5h, 0FFh, 0FFh, 072h, 012h, 080h, 07Bh, 038h, 004h, 075h, 00Ch, 0C6h
        db  083h, 080h, 000h, 000h, 000h, 001h, 0E8h, 067h, 0FCh, 0FFh, 0FFh, 0C3h
loc_1_2E7A:
        mov    eax, 0x10001
        ret    
loc_1_2E80:
        mov    edi, esi
        sub    edi, ebx
        add    edi, 0x12
        cmp    word ptr [eax + 0x76], di
        jb     loc_1_2E7A
        mov    edx, ebx
        lea    edi, [esi + 4]
        movzx  ecx, byte ptr [edi + 0xc]
        dec    ecx
        js     loc_1_2F79
        mov    ecx, dword ptr [ecx*4 + 0x77c0] ; -> obj1:0x77C0
        test   ecx, ecx
        je     loc_1_2F79
        mov    ebx, dword ptr [ecx + 0x38]
        test   ebx, ebx
        je     loc_1_2FDE
loc_1_2EB7:
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        mov    eax, ecx
        jb     loc_1_2F25
        cli    
        mov    cl, byte ptr [esi + 2]
        shr    cl, 1
        cmp    cl, byte ptr [ebx + 0x45]
        jne    loc_1_2F81
        and    dword ptr [ebx + 0x34], 0xfffffffb
        inc    byte ptr [ebx + 0x45]
        and    byte ptr [ebx + 0x45], 0x7f
        test   dword ptr [ebx + 0x3c], 0x10
        jne    loc_1_2F29
        sti    
        mov    cl, byte ptr [ebx + 0x47]
        add    cl, byte ptr [ebx + 0x49]
        and    cl, 0x7f
        cmp    cl, byte ptr [ebx + 0x45]
        je     loc_1_2FFB
loc_1_2EF9:
        mov    ecx, dword ptr [ebx + 0x74]
        mov    dword ptr [ebx + 0x60], ecx
loc_1_2EFF:
        test   word ptr [eax + 0x40], 0x40
        jne    loc_1_2F25
        cmp    word ptr [edi + 2], 0xefff
        jne    loc_1_2F25
        movzx  ebx, byte ptr [edi + 4]
        cmp    bl, 0x20
        jae    loc_1_2F25
        call   sub_1_29C4
loc_1_2F1F:
        mov    eax, 0
        ret    
loc_1_2F25:
        sti    
        xor    eax, eax
        ret    
loc_1_2F29:
        push   eax
        cli    
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x100000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        pop    ebx
        jne    loc_1_2F57
        push   edi
        push   edx
        push   esi
        mov    edi, ebx
        call   sub_1_5121
        sbb    eax, eax
        mov    ebx, edi
        pop    esi
        pop    edx
        pop    edi
loc_1_2F57:
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   eax
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        pop    eax
        test   eax, eax
        pop    eax
        jne    loc_1_2EF9
        jmp    loc_1_2EFF
loc_1_2F79:
        call   sub_1_2B44
        xor    eax, eax
        ret    
loc_1_2F81:
        test   dword ptr [ebx + 0x34], 4
        jne    loc_1_2F1F
        cli    
        or     dword ptr [ebx + 0x34], 4
        or     dword ptr [ebx + 0x3c], 0x20
        mov    edi, ebx
        cli    
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x100000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        pop    ebx
        jne    loc_1_2FB8
        call   sub_1_535E
        jae    loc_1_2FBE
loc_1_2FB8:
        push   edi
        call   sub_1_4E6C
loc_1_2FBE:
        sti    
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        mov    eax, 0
        ret    
loc_1_2FDE:
        mov    ebx, ecx
        call   sub_1_62FA
        jb     loc_1_2FEE
        mov    ebx, eax
        jmp    loc_1_2EB7
loc_1_2FEE:
        mov    edi, edx
        call   sub_1_2A4E
        mov    eax, 0
        ret    
loc_1_2FFB:
        or     dword ptr [ebx + 0x3c], 0x80
        or     dword ptr [ebx + 0x34], 0x8000
        push   eax
        push   ebx
        call   sub_1_4E6C
        pop    eax
        jmp    loc_1_2EFF
loc_1_3016:
        mov    edi, 4
        call   sub_1_2A44
        jb     loc_1_304B
        push   eax
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_304C
        pop    edi
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        jb     loc_1_3045
        and    dword ptr [ebx + 0x34], 0xfffffffd
        cli    
        test   dword ptr [ebx + 0x3c], 0x10
        jne    loc_1_3053
loc_1_3045:
        push   ebx
        call   sub_1_4E6C
loc_1_304B:
        ret    
loc_1_304C:
        pop    eax
        call   sub_1_2A4E
        ret    
loc_1_3053:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x100000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        pop    ebx
        jne    loc_1_3077
        mov    edi, ebx
        call   sub_1_5121
        mov    ebx, edi
loc_1_3077:
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        sti    
        jmp    loc_1_3045
loc_1_3093:
        mov    edi, 4
        call   sub_1_2A44
        jb     loc_1_30DC
        push   eax
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_30DD
        pop    edi
        mov    al, byte ptr [esi + 3]
        cli    
        mov    cl, al
        shr    cl, 1
        add    cl, 0x81
        sub    cl, byte ptr [ebx + 0x46]
        and    cl, 0x7f
        cmp    byte ptr [ebx + 0x48], cl
        jbe    loc_1_30D6
        sti    
        or     dword ptr [ebx + 0x34], 0x100
        call   sub_1_2CBF
        jb     loc_1_30D6
        and    dword ptr [ebx + 0x34], 0xfffffefd
loc_1_30D6:
        push   ebx
        call   sub_1_4E6C
loc_1_30DC:
        ret    
loc_1_30DD:
        pop    eax
        call   sub_1_2A4E
        ret    
loc_1_30E4:
        mov    edi, 4
        call   sub_1_2A44
        jb     loc_1_3107
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_310E
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        jb     loc_1_3107
        or     dword ptr [ebx + 0x34], 2
loc_1_3107:
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_310E:
        call   sub_1_2A4E
        ret    
loc_1_3114:
        mov    edi, 0x2f
        call   sub_1_2A44
        jb     loc_1_3158
        test   ecx, ecx
        jne    loc_1_3158
        test   byte ptr [esi + 2], 0x10
        jne    loc_1_3158
        mov    edx, ebx
        lea    edi, [esi + 3]
        cmp    word ptr [edi + 2], 0xefff
        jne    loc_1_3158
        movzx  ebx, byte ptr [edi + 4]
        cmp    bl, 0x20
        jae    loc_1_3158
        cmp    bl, 7
        ja     loc_1_314C
        call   sub_1_2BBD
        jb     loc_1_3158
loc_1_314C:
        cmp    bl, 0x14
        jae    loc_1_3158
        call   sub_1_29C4
loc_1_3158:
        ret    
loc_1_3159:
        ret    
loc_1_315A:
        mov    edx, eax
        mov    edi, ebx
        call   sub_1_236B
        jae    loc_1_3225
        cli    
        push   eax
        mov    eax, 0xc8
        call   sub_1_2233
        mov    esi, eax
        pop    eax
        test   esi, esi
        je     loc_1_31B9
        mov    edx, eax
        mov    dword ptr [esi + 0x40], eax
        push   edx
        call   sub_1_27B0
        pop    edx
        jb     loc_1_31B9
        mov    dword ptr [ebx + 0x40], edx
        call   sub_1_236B
        jb     loc_1_31BF
        mov    dword ptr [ebx + 0x40], edx
loc_1_3197:
        mov    byte ptr [ebx + 0x38], 1
loc_1_319B:
        or     dword ptr [ebx + 0x3c], 4
        or     dword ptr [ebx + 0x34], 0x1000
        push   ebx
        call   sub_1_4E6C
        mov    eax, dword ptr [ebx + 0x6c]
        mov    dword ptr [ebx + 0x58], eax
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [ebx + 0x64], eax
        ret    
loc_1_31B9:
        call   sub_1_2A4E
        ret    
loc_1_31BF:
        call   sub_1_2A4E
        ret    
loc_1_31C5:
        cmp    byte ptr [ebx + 0x38], 1
        je     loc_1_319B
        cli    
        test   byte ptr [ebx + 0xb8], 4
        je     loc_1_31ED
        push   esi
        push   edi
        lea    esi, [ebx + 0x2c]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [ebx + 0xb8], 0xfb
loc_1_31ED:
        sti    
        call   sub_1_6255
loc_1_31F3:
        cli    
        lea    esi, [ebx]
        call   sub_1_28E3
        je     loc_1_320F
        and    byte ptr [esi + 0x24], 0xfd
        sti    
        or     word ptr [esi + 0x40], 0x40
        push   esi
        call   sub_1_5BF3
        jmp    loc_1_31F3
loc_1_320F:
        sti    
        mov    dword ptr [ebx + 0x34], 0
        xchg   ebx, esi
        call   sub_1_26A5
        xchg   ebx, esi
        jmp    loc_1_3197
loc_1_3225:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_31C5
        call   sub_1_6255
loc_1_3230:
        cli    
        lea    esi, [ebx]
        call   sub_1_28E3
        je     loc_1_324C
        and    byte ptr [esi + 0x24], 0xfd
        sti    
        or     word ptr [esi + 0x40], 0x40
        push   esi
        call   sub_1_5BF3
        jmp    loc_1_3230
loc_1_324C:
        sti    
        mov    dword ptr [ebx + 0x34], 0
        mov    esi, ebx
        call   sub_1_26A5
        jmp    loc_1_3197
loc_1_3260:
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_32AD
        mov    dword ptr [ebx + 0x58], 0xffffffff
        mov    dword ptr [ebx + 0x5c], 0xffffffff
        mov    dword ptr [ebx + 0x60], 0xffffffff
        mov    dword ptr [ebx + 0x64], 0xffffffff
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [ebx + 0xb9], al
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x70], eax
        mov    byte ptr [ebx + 0x38], 2
        call   sub_1_6255
        mov    dword ptr [ebx + 0x3c], 4
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_32AD:
        call   sub_1_2A4E
        ret    
loc_1_32B3:
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_32E8
        jecxz  loc_1_32E9
        mov    dword ptr [ebx + 0x5c], 0xffffffff
        mov    al, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [ebx + 0xb9], al
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [ebx + 0x70], eax
        cmp    byte ptr [ebx + 0x38], 1
        je     loc_1_32FC
        cmp    byte ptr [ebx + 0x38], 2
        jne    loc_1_32E9
        push   ebx
        call   sub_1_6435
loc_1_32E8:
        ret    
loc_1_32E9:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_32E8
        mov    byte ptr [ebx + 0x80], 0x41
        call   sub_1_2AE0
        ret    
loc_1_32FC:
        mov    dword ptr [ebx + 0x3c], 1
        test   dword ptr [ebx + 0x34], 0x1000
        jne    loc_1_3310
        or     dword ptr [ebx + 0x34], 0x20
loc_1_3310:
        push   ebx
        call   sub_1_4E6C
        mov    eax, dword ptr [ebx + 0x6c]
        mov    dword ptr [ebx + 0x58], eax
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [ebx + 0x64], eax
        ret    
loc_1_3323:
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_3340
        cmp    byte ptr [ebx + 0x38], 0
        je     loc_1_333F
        mov    dword ptr [ebx + 0x5c], 0xffffffff
        push   ebx
        call   sub_1_6435
loc_1_333F:
        ret    
loc_1_3340:
        ret    
loc_1_3341:
        mov    edi, 8
        call   sub_1_2A44
        jne    loc_1_338A
        inc    dword ptr [eax + 0xd0]
        mov    edi, ebx
        call   sub_1_236B
        jb     loc_1_338A
        jecxz  loc_1_3378
        cmp    byte ptr [ebx + 0x38], 4
        je     loc_1_336A
        cmp    byte ptr [ebx + 0x38], 3
        jne    loc_1_338A
loc_1_336A:
        mov    dword ptr [ebx + 0x3c], 2
        push   ebx
        call   sub_1_4E6C
        ret    
loc_1_3378:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_338A
        mov    byte ptr [ebx + 0x80], 0x41
        call   sub_1_2AE0
loc_1_338A:
        ret    
loc_1_338B:
        ret    
sub_1_338C:
        mov    edi, 6
        call   sub_1_2A44
        jne    loc_1_338B
        or     ecx, ecx
        jne    loc_1_338B
        mov    edi, ebx
        mov    dh, byte ptr [0x6f2b]         ; -> loc_1_6F2B
        mov    cx, word ptr [esi]
        xchg   cl, ch
        or     ch, ch
        jne    loc_1_3400
loc_1_33AD:
        mov    dl, byte ptr [esi + 2]
        cli    
        lea    ebx, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_28F3
        sti    
        je     loc_1_33FF
        mov    dword ptr [ebx + 0x14], eax
        lea    esi, [ebx + 0x34]
        mov    dword ptr [ebx + 0x24], esi
        mov    word ptr [ebx + 0x10], 2
        mov    word ptr [ebx + 0x1e], 1
        push   edx
        push   ecx
        call   sub_1_2571
        add    esi, ecx
        pop    edx
        mov    word ptr [esi], dx
        pop    edx
        mov    byte ptr [esi + 2], dl
        lea    esi, [esi + 3]
        mov    byte ptr [esi], 0x81
        mov    byte ptr [esi + 1], 3
        and    dh, 0x7f
        mov    byte ptr [esi + 2], dh
        add    ecx, 6
        mov    esi, ebx
        call   sub_1_2C01
loc_1_33FF:
        ret    
loc_1_3400:
        cmp    ch, 0xf0
        jne    loc_1_341F
        call   sub_1_236B
        jb     loc_1_33AD
        cmp    byte ptr [ebx + 0x38], 3
        je     loc_1_33FF
        mov    dh, byte ptr [esi + 5]
        shr    dh, 1
        mov    byte ptr [ebx + 0x48], dh
        mov    dh, byte ptr [ebx + 0x49]
loc_1_341D:
        jmp    loc_1_33AD
loc_1_341F:
        cmp    ch, 0xff
        jne    loc_1_33FF
        mov    ch, 0xf0
        jmp    loc_1_341D
sub_1_3428:
        jecxz  loc_1_342C
loc_1_342A:
        sti    
        ret    
loc_1_342C:
        mov    edi, ebx
        mov    bl, 0xe3
        test   byte ptr [esi + 2], 0x10
        je     loc_1_3439
        or     bl, 0x10
loc_1_3439:
        cli    
        test   dword ptr [0x6eec], 0x1000    ; -> obj1:0x6EEC
        jne    loc_1_342A
        lea    esi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_28E3
        je     loc_1_342A
        or     dword ptr [0x6eec], 0x1000    ; -> obj1:0x6EEC
        sti    
        mov    dword ptr [esi + 0x14], eax
        mov    word ptr [esi + 0x10], 0x1002
        mov    word ptr [esi + 0x1e], 1
        mov    dword ptr [esi + 0x24], 0x7bd4 ; -> obj1:0x7BD4
        push   esi
        mov    esi, dword ptr [esi + 0x24]
        call   sub_1_2571
        add    esi, ecx
        mov    byte ptr [esi], 0xf0
        mov    byte ptr [esi + 1], 0xf1
        mov    byte ptr [esi + 2], bl
        add    ecx, 3
        pop    edi
        push   esi
        push   edi
        lea    edx, [esi + 3]
        mov    ebx, ecx
        mov    edi, dword ptr [edi + 0x14]
        push   ecx
        neg    ecx
        add    ecx, 0x258
        call   sub_1_5F9D
        jae    loc_1_34A9
        sub    ecx, ecx
loc_1_34A9:
        mov    ebx, ecx
        pop    ecx
        add    ecx, ebx
        pop    esi
        or     ebx, ebx
        jne    loc_1_34C8
        mov    edi, dword ptr [esi + 0x14]
        cmp    dword ptr [edi + 0x28], 1
        je     loc_1_34C8
        pop    edi
        add    ebx, 3
        xchg   bl, bh
        mov    word ptr [edi - 2], bx
        jmp    loc_1_34C9
loc_1_34C8:
        pop    edi
loc_1_34C9:
        call   sub_1_2C01
        ret    
        db  000h
sub_1_34D0:
        pushfd 
        cli    
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_3529
        push   eax
        test   dword ptr [ecx + 4], 2
        je     loc_1_34FE
        mov    eax, 0x7d0
        test   dword ptr [ecx + 4], 1
        jne    loc_1_34F7
        mov    eax, 0x64
loc_1_34F7:
        call   sub_1_265A
        jmp    loc_1_351C
loc_1_34FE:
        push   ebx
        mov    ebx, ecx
        call   sub_1_2671
        shl    eax, 1
        pop    ebx
        call   sub_1_265A
        xchg   ecx, eax
        call   sub_1_265A
        add    eax, ecx
        add    eax, dword ptr [0x6f1b]       ; -> loc_1_6F1B
loc_1_351C:
        mov    ecx, dword ptr [ebx + 0x88]
        mov    dword ptr [ecx + 0x32], eax
        pop    eax
        mov    dword ptr [ecx + 0x3a], eax
loc_1_3529:
        popfd  
        ret    
sub_1_352B:
        mov    ebx, dword ptr [0x6fb8]       ; -> obj1:0x6FB8
loc_1_3531:
        test   ebx, ebx
        je     loc_1_3556
        cli    
        cmp    byte ptr [ebx + 0x38], 0
        je     loc_1_3550
        dec    dword ptr [ebx + 0x5c]
        je     loc_1_3557
loc_1_3541:
        dec    dword ptr [ebx + 0x60]
        je     loc_1_357E
loc_1_3546:
        dec    dword ptr [ebx + 0x58]
        je     loc_1_3585
loc_1_354B:
        dec    dword ptr [ebx + 0x64]
        je     loc_1_358C
loc_1_3550:
        sti    
        mov    ebx, dword ptr [ebx + 0x18]
        jmp    loc_1_3531
loc_1_3556:
        ret    
loc_1_3557:
        cmp    byte ptr [ebx + 0x38], 4
        jne    loc_1_3564
        call   sub_1_1E97
        jmp    loc_1_3541
loc_1_3564:
        cmp    byte ptr [ebx + 0x38], 1
        jne    loc_1_3571
        call   sub_1_1E26
        jmp    loc_1_3541
loc_1_3571:
        cmp    byte ptr [ebx + 0x38], 2
        jne    loc_1_3541
        call   sub_1_1E67
        jmp    loc_1_3541
loc_1_357E:
        call   sub_1_1D9C
        jmp    loc_1_3546
loc_1_3585:
        call   sub_1_1E09
        jmp    loc_1_354B
loc_1_358C:
        call   sub_1_1DD1
        jmp    loc_1_3550
sub_1_3593:
        dec    dword ptr [0x6ec4]            ; -> obj1:0x6EC4
        mov    eax, dword ptr [0x6ec4]       ; -> obj1:0x6EC4
        push   eax
        test   eax, eax
        jne    loc_1_35AD
        mov    eax, dword ptr [0x6ec8]       ; -> obj1:0x6EC8
        mov    dword ptr [0x6ec4], eax       ; -> obj1:0x6EC4
loc_1_35AD:
        mov    ebx, dword ptr [0x6fbc]       ; -> obj1:0x6FBC
loc_1_35B3:
        test   ebx, ebx
        je     loc_1_3652
        cli    
        test   byte ptr [ebx + 0x40], 0x80
        je     loc_1_35D8
        cmp    byte ptr [ebx + 0x25], 2
        je     loc_1_35D8
        mov    edi, dword ptr [ebx + 0x88]
        dec    dword ptr [edi + 0x32]
        jne    loc_1_35D8
        push   ebx
        call   dword ptr [edi + 0x3a]
        pop    ebx
loc_1_35D8:
        sti    
        cmp    dword ptr [esp], 0
        jne    loc_1_3649
        cli    
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_3649
        cmp    byte ptr [ebx + 0x8f], 0
        je     loc_1_3612
        lea    eax, [ebx + 0x30]
        mov    edx, dword ptr [eax]
loc_1_35F4:
        cmp    eax, edx
        je     loc_1_3612
        dec    byte ptr [edx + 0xc]
        jne    loc_1_360E
        mov    ecx, dword ptr [edx]
        pushal 
        lea    ebx, [edx - 0x32]
        call   sub_1_1A40
        popal  
        cli    
        mov    edx, ecx
        jmp    loc_1_35F4
loc_1_360E:
        mov    edx, dword ptr [edx]
        jmp    loc_1_35F4
loc_1_3612:
        cmp    byte ptr [ebx + 0x8e], 0
        je     loc_1_3649
        lea    eax, [ebx + 0x28]
        mov    edx, dword ptr [eax]
loc_1_3620:
        cmp    eax, edx
        je     loc_1_3649
        mov    cl, byte ptr [edx - 0x32]
        and    cl, 0x7f
        cmp    cl, 0x15
        jne    loc_1_3645
        dec    byte ptr [edx + 0xc]
        jne    loc_1_3645
        mov    ecx, dword ptr [edx]
        pushal 
        lea    ebx, [edx - 0x32]
        call   sub_1_1BE8
        popal  
        cli    
        mov    edx, ecx
        jmp    loc_1_3620
loc_1_3645:
        mov    edx, dword ptr [edx]
        jmp    loc_1_3620
loc_1_3649:
        sti    
        mov    ebx, dword ptr [ebx + 0x20]
        jmp    loc_1_35B3
loc_1_3652:
        pop    eax
        ret    
loc_1_3654:
        inc    dword ptr [0x6ed4]            ; -> obj1:0x6ED4
        bts    dword ptr [0x6eec], 1         ; -> obj1:0x6EEC
        jb     loc_1_3698
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        jb     loc_1_367E
        mov    eax, dword ptr [ebx + 8]
        test   byte ptr [eax + 0x2d], 2
        je     loc_1_367E
        pushfd 
        call   sub_1_36AF
        popfd  
        jmp    loc_1_3698
loc_1_367E:
        sub    eax, eax
        sub    ebx, ebx
        mov    esi, 0x36af                   ; -> sub_1_36AF
        mov    ecx, 0x801
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0x6f3c], esi       ; -> obj1:0x6F3C
loc_1_3698:
        mov    eax, dword ptr [0x6de8]       ; -> obj1:0x6DE8
        mov    esi, 0x3654                   ; -> loc_1_3654
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x6f38], esi       ; -> obj1:0x6F38
        ret    
sub_1_36AF:
        sub    eax, eax
        and    byte ptr [0x6eec], 0xfd       ; -> obj1:0x6EEC
        mov    dword ptr [0x6f3c], eax       ; -> obj1:0x6F3C
        cmp    dword ptr [0x6ed4], eax       ; -> obj1:0x6ED4
        je     loc_1_372E
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
loc_1_36CB:
        dec    dword ptr [0x6ed4]            ; -> obj1:0x6ED4
        sub    eax, eax
loc_1_36D3:
        cmp    eax, 8
        je     loc_1_36F3
        mov    esi, dword ptr [eax*4 + 0x6f40] ; -> obj1:0x6F40
        test   esi, esi
        je     loc_1_36F0
        btr    dword ptr [esi], 4
        jae    loc_1_36F0
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
loc_1_36F0:
        inc    eax
        jmp    loc_1_36D3
loc_1_36F3:
        call   sub_1_352B
        call   sub_1_3593
        call   sub_1_5638
        sti    
        dec    dword ptr [0x6ecc]            ; -> obj1:0x6ECC
        jne    loc_1_371A
        mov    eax, dword ptr [0x6ed0]       ; -> obj1:0x6ED0
        mov    dword ptr [0x6ecc], eax       ; -> obj1:0x6ECC
        call   sub_1_1DAE
loc_1_371A:
        cmp    dword ptr [0x6ed4], 0         ; -> obj1:0x6ED4
        jne    loc_1_36CB
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        call   sub_1_67F7
loc_1_372E:
        ret    
        db  000h
loc_1_3730:
        adc    byte ptr [eax - 0x40], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h
loc_1_3738:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
sub_1_373E:
        push   ebx
        cli    
        lea    ebx, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_28F3
        sti    
        stc    
        je     loc_1_37A3
        mov    word ptr [ebx + 0x10], 2
        mov    word ptr [ebx + 0x1e], 1
        push   edi
        mov    edi, edx
        lea    esi, [ebx + 0x34]
        mov    dword ptr [ebx + 0x24], esi
        call   sub_1_2571
        cmp    dword ptr [eax + 0x28], 0
        je     loc_1_3772
        add    esi, 2
loc_1_3772:
        mov    edx, dword ptr [eax + 0x31]
        cmp    edx, dword ptr [esi]
        jne    loc_1_3787
        mov    edx, dword ptr [eax + 0x33]
        cmp    edx, dword ptr [esi + 2]
        jne    loc_1_3787
        or     word ptr [ebx + 0x10], 0x400
loc_1_3787:
        pop    edi
        mov    esi, dword ptr [ebx + 0x24]
        mov    byte ptr [ecx + esi + 1], 0xf0
        mov    byte ptr [ecx + esi], 0xf0
        mov    byte ptr [ecx + esi + 2], 3
        add    ecx, 3
        mov    esi, ebx
        mov    word ptr [esi + 0x22], cx
        clc    
loc_1_37A3:
        pop    ebx
        ret    
sub_1_37A5:
        push   ebx
        cli    
        lea    esi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_28E3
        sti    
        stc    
        je     loc_1_381F
        mov    word ptr [esi + 0x10], 0x402
        mov    word ptr [esi + 0x1e], 1
        push   edi
        push   ecx
        lea    edi, [esi + 0x34]
        mov    dword ptr [esi + 0x24], edi
        push   esi
        mov    esi, 0x3730                   ; -> loc_1_3730
        mov    ecx, 8
        cmp    dword ptr [eax + 0x28], 1
        je     loc_1_37E5
        mov    esi, 0x3738                   ; -> loc_1_3738
        mov    ecx, 6
loc_1_37E5:
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [eax + 0x31]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        je     loc_1_37FA
        add    edx, 3
        xchg   dl, dh
        mov    word ptr [edi], dx
        jmp    loc_1_3803
loc_1_37FA:
        or     byte ptr [edi - 6], 0x80
        mov    word ptr [edi], 0x70c2
loc_1_3803:
        add    edi, 2
        pop    esi
        mov    byte ptr [edi], 0xf0
        mov    byte ptr [edi + 1], 0xf0
        mov    byte ptr [edi + 2], 3
        sub    edi, dword ptr [esi + 0x24]
        add    edi, 3
        mov    word ptr [esi + 0x22], di
        clc    
        pop    ecx
        pop    edi
loc_1_381F:
        pop    ebx
        ret    
sub_1_3821:
        cli    
        push   eax
        mov    esi, dword ptr [0x6f68]       ; -> obj1:0x6F68
        mov    esi, dword ptr [esi]
        lea    eax, [0x6f68]                 ; -> obj1:0x6F68
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [0x6f68], esi       ; -> obj1:0x6F68
        cmp    esi, eax
        pop    eax
        je     loc_1_3888
        sti    
        mov    word ptr [esi + 0x10], 1
        push   eax
        mov    eax, dword ptr [edi + 0x34]
        and    eax, 0x400
        or     word ptr [esi + 0x10], ax
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [esi + 0x14], eax
        lea    eax, [ebx + 0x98]
        mov    dword ptr [esi + 0x18], eax
        sub    eax, eax
        mov    byte ptr [esi + 0x20], al
        mov    byte ptr [esi + 0x28], al
        mov    word ptr [esi + 0x1e], ax
        movzx  eax, byte ptr [ebx + 0x91]
        mov    word ptr [esi + 0x1c], ax
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 0x30], ebx
        mov    word ptr [esi + 0x3c], 0x12
        pop    eax
        stc    
        ret    
loc_1_3888:
        test   byte ptr [ebx + 0x24], 8
        jne    loc_1_38A4
        push   esi
        lea    esi, [0x6fa0]                 ; -> obj1:0x6FA0
        push   ebx
        lea    ebx, [ebx + 0x10]
        call   sub_1_2892
        pop    ebx
        pop    esi
        or     byte ptr [ebx + 0x24], 8
loc_1_38A4:
        or     dword ptr [0x6eec], 0x800     ; -> obj1:0x6EEC
        sti    
        ret    
sub_1_38B0:
        mov    edx, 0x2c
        call   sub_1_37A5
        jae    loc_1_38BD
        ret    
loc_1_38BD:
        mov    edx, esi
        movzx  ecx, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        mov    esi, dword ptr [esi + 0x24]
        add    esi, ecx
        sub    ecx, ecx
        mov    byte ptr [esi + 5], cl
        mov    word ptr [esi + 6], cx
        mov    word ptr [esi + 8], cx
        mov    word ptr [esi + 0xa], cx
        mov    word ptr [esi], 0x2c
        mov    word ptr [esi + 2], 0xefff
        mov    byte ptr [esi + 4], 2
        lea    edi, [esi + 0xc]
        lea    esi, [ebx + 0x54]
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [eax + 0x84]
        lea    esi, [esi + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, edx
        mov    dword ptr [esi + 0x14], eax
        cli    
        push   edi
        lea    edi, [eax + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [eax], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        ret    
sub_1_3927:
        mov    edx, 0x2c
        call   sub_1_37A5
        jae    loc_1_3934
        ret    
loc_1_3934:
        cmp    dword ptr [eax + 0x28], 1
        jne    loc_1_3955
        cmp    byte ptr [0x6f36], 1          ; -> obj1:0x6F36
        je     loc_1_3955
        sub    word ptr [esi + 0x22], 2
        mov    edx, dword ptr [esi + 0x24]
        and    byte ptr [edx + 8], 0x7f
        mov    ecx, dword ptr [edx + 0x10]
        mov    dword ptr [edx + 0xe], ecx
loc_1_3955:
        mov    dword ptr [esi + 0x14], eax
        movzx  ecx, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        add    ecx, dword ptr [esi + 0x24]
        mov    word ptr [ecx], 0x2c
        mov    word ptr [ecx + 2], 0xefff
        mov    byte ptr [ecx + 4], 0xa
        sub    edx, edx
        mov    byte ptr [ecx + 5], dl
        mov    word ptr [ecx + 8], dx
        push   eax
        mov    eax, dword ptr [eax + 0x40]
        call   sub_1_29B1
        mov    byte ptr [ecx + 5], al
        pop    eax
        mov    dl, byte ptr [ebx + 0x68]
        mov    word ptr [ecx + 0xa], dx
        cmp    byte ptr [ebx + 0x25], 7
        jne    loc_1_3998
        xor    dl, dl
loc_1_3998:
        mov    word ptr [ecx + 6], dx
        push   esi
        push   ecx
        lea    edi, [ecx + 0xc]
        lea    esi, [ebx + 0x54]
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    ecx, 4
        lea    edi, [esi + 0x1c]
        lea    esi, [ebx + 0x44]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        cli    
        push   edi
        lea    edi, [eax + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [eax], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        clc    
        ret    
sub_1_39D9:
        mov    edx, 0x2c
        call   sub_1_37A5
        jb     loc_1_39EA
        jmp    loc_1_3955
loc_1_39EA:
        stc    
        ret    
sub_1_39EC:
        mov    edx, 0x2c
        call   sub_1_37A5
        jae    loc_1_39F9
        ret    
loc_1_39F9:
        mov    dword ptr [esi + 0x14], eax
        movzx  ecx, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        add    ecx, dword ptr [esi + 0x24]
        mov    word ptr [ecx], 0x2c
        mov    word ptr [ecx + 2], 0xefff
        mov    dl, 1
        test   word ptr [ebx + 0x40], 0x10
        je     loc_1_3A1F
        mov    dl, 0
loc_1_3A1F:
        mov    byte ptr [ecx + 4], dl
        xor    edx, edx
        mov    byte ptr [ecx + 5], dl
        mov    word ptr [ecx + 6], dx
        mov    word ptr [ecx + 8], dx
        mov    dl, byte ptr [ebx + 0x68]
        mov    word ptr [ecx + 0xa], dx
        push   esi
        lea    edi, [ecx + 0x1c]
        lea    esi, [ebx + 0x44]
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        cli    
        push   edi
        lea    edi, [eax + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [eax], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        clc    
        ret    
sub_1_3A64:
        call   sub_1_373E
        jae    loc_1_3A6C
        ret    
loc_1_3A6C:
        mov    dword ptr [esi + 0x14], eax
        movzx  ecx, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        add    ecx, dword ptr [esi + 0x24]
        mov    word ptr [ecx], 0x2c
        mov    word ptr [ecx + 2], 0xefff
        mov    byte ptr [ecx + 4], 0xd
        mov    byte ptr [ecx + 5], 0
        sub    edx, edx
        test   byte ptr [ebx], 1
        je     loc_1_3A96
        inc    edx
loc_1_3A96:
        mov    word ptr [ecx + 6], dx
        mov    dx, word ptr [edi + 0xa]
        mov    word ptr [ecx + 8], dx
        mov    word ptr [ecx + 0xa], 0
        mov    ebx, ecx
        mov    ecx, 4
        push   edi
loc_1_3AB0:
        mov    edx, dword ptr [edi + 0x1c]
        mov    dword ptr [ebx + 0x1c], edx
        mov    dword ptr [ebx + 0xc], edx
        add    edi, 4
        add    ebx, 4
        loop   loc_1_3AB0
        pop    edi
        cmp    dword ptr [eax + 0x28], 1
        je     loc_1_3AD6
        mov    ecx, dword ptr [esi + 0x24]
        mov    edx, 0x2f
        xchg   dl, dh
        mov    word ptr [ecx + 0xc], dx
loc_1_3AD6:
        cli    
        push   edi
        lea    edi, [eax + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [eax], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        clc    
        ret    
sub_1_3AF4:
        xchg   ebx, eax
        call   sub_1_373E
        xchg   ebx, eax
        jae    loc_1_3AFE
        ret    
loc_1_3AFE:
        cmp    dword ptr [ebx + 0x28], 1
        mov    edx, dword ptr [esi + 0x24]
        jne    loc_1_3B2A
        test   byte ptr [edx + 8], 0x80
        je     loc_1_3B38
        mov    cx, 0x70c2
        and    cl, 0xbf
        mov    word ptr [edx + 0xe], cx
        mov    word ptr [edx + 0x10], 0xf0f0
        mov    byte ptr [edx + 0x12], 3
        mov    word ptr [esi + 0x22], 0x13
        jmp    loc_1_3B38
loc_1_3B2A:
        mov    cx, word ptr [esi + 0x22]
        add    cx, 0x1e
        xchg   cl, ch
        mov    word ptr [edx + 0xc], cx
loc_1_3B38:
        mov    dword ptr [esi + 0x14], ebx
        movzx  ecx, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        push   esi
        mov    esi, dword ptr [esi + 0x24]
        add    esi, ecx
        mov    word ptr [esi], 0x2c
        mov    word ptr [esi + 2], 0xefff
        mov    byte ptr [esi + 4], 0xe
        push   eax
        mov    eax, dword ptr [ebx + 0x40]
        call   sub_1_29B1
        mov    byte ptr [esi + 5], al
        pop    eax
        mov    word ptr [esi + 6], ax
        xor    ah, ah
        mov    word ptr [esi + 0xa], ax
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [esi + 8], ax
        xchg   edi, esi
        push   esi
        push   edi
        mov    ecx, 4
        lea    esi, [esi + 0x1c]
        lea    edi, [edi + 0xc]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        mov    ecx, 4
        lea    esi, [esi + 0xc]
        lea    edi, [edi + 0x1c]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        cli    
        push   edi
        lea    edi, [ebx + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [ebx], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        clc    
        ret    
sub_1_3BB8:
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_3C39
        push   ecx
        push   edx
        mov    byte ptr [esi + 0x34], 0x19
        mov    al, 0xe
        mov    ecx, dword ptr [edi + 0x40]
        cmp    dword ptr [ecx + 0x28], 1
        jne    loc_1_3BE7
        test   byte ptr [edi + 0x94], 0x80
        je     loc_1_3BE7
        mov    al, byte ptr [edi + 0x9b]
        and    al, 0x70
        shr    al, 3
loc_1_3BE7:
        or     al, 0x81
        mov    byte ptr [esi + 0x35], al
        mov    eax, 0x1000
        movzx  ecx, byte ptr [edi + 0xb6]
        movzx  edx, word ptr [edi + 0x7a]
        sub    edx, ecx
        sub    edx, 0x12
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [esi + 0x36], ax
        mov    al, byte ptr [ebx + 0x68]
        sub    ah, ah
        mov    word ptr [esi + 0x3a], ax
        mov    ax, word ptr [ebx + 0x64]
        mov    word ptr [esi + 0x38], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc    
        pop    edx
        pop    ecx
        ret    
loc_1_3C39:
        or     word ptr [ebx + 0x6a], 0x400
        sti    
        stc    
        ret    
sub_1_3C42:
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_3CC9
        mov    byte ptr [esi + 0x34], 0x17
        mov    byte ptr [esi + 0x35], 0x81
        mov    eax, 0x1000
        movzx  ecx, byte ptr [edi + 0xb6]
        movzx  edx, word ptr [edi + 0x7a]
        sub    edx, ecx
        sub    edx, 0x12
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [esi + 0x36], ax
        mov    eax, dword ptr [0x6f07]       ; -> loc_1_6F07
        add    eax, 0x32
        mov    dword ptr [edi + 0x6c], eax
        mov    eax, dword ptr [edi + 0x6c]
        mov    dword ptr [edi + 0x58], eax
        mov    al, byte ptr [ebx + 0x68]
        xor    ah, ah
        mov    word ptr [esi + 0x3a], ax
        mov    ax, word ptr [ebx + 0x64]
        mov    word ptr [esi + 0x38], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        push   eax
        mov    eax, dword ptr [ebx + 0x38]
        cmp    dword ptr [eax + 0x68], 0x3c
        jbe    loc_1_3CC6
        mov    byte ptr [ebx + 0x8f], 0
        mov    byte ptr [ebx + 0x8e], 0
loc_1_3CC6:
        pop    eax
        clc    
        ret    
loc_1_3CC9:
        or     word ptr [ebx + 0x6a], 0x800
        sti    
        stc    
        ret    
sub_1_3CD2:
        push   esi
        push   edi
        test   word ptr [ebx + 0x42], 2
        jne    loc_1_3D4D
        cli    
        test   word ptr [ebx + 0x6a], 4
        je     loc_1_3CFB
        and    word ptr [ebx + 0x6a], 0xfff3
        dec    dword ptr [0x6eff]            ; -> loc_1_6EFF
        push   eax
        mov    ax, word ptr [ebx + 0x64]
        call   sub_1_3E4E
        pop    eax
loc_1_3CFB:
        sti    
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_3D4D
        or     word ptr [ebx + 0x42], 2
        or     word ptr [esi + 0x10], 0x100
        mov    byte ptr [esi + 0x34], 0x18
        sub    eax, eax
        mov    byte ptr [esi + 0x35], al
        mov    word ptr [esi + 0x38], ax
        mov    word ptr [esi + 0x3a], ax
        and    word ptr [ebx + 0x40], 0xf0ff
        test   word ptr [ebx + 0x40], 0x40
        je     loc_1_3D31
        inc    eax
loc_1_3D31:
        mov    word ptr [esi + 0x36], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        stc    
loc_1_3D4D:
        cmc    
        pop    edi
        pop    esi
        ret    
sub_1_3D51:
        push   esi
        push   edi
        cli    
        test   word ptr [ebx + 0x6a], 0x80
        jne    loc_1_3D97
        sti    
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_3D97
        mov    byte ptr [esi + 0x34], 0x1b
        mov    word ptr [esi + 0x36], ax
        sub    eax, eax
        mov    byte ptr [esi + 0x35], al
        mov    word ptr [esi + 0x38], ax
        mov    word ptr [esi + 0x3a], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc    
        pop    edi
        pop    esi
        ret    
loc_1_3D97:
        or     word ptr [ebx + 0x6a], 0x100
        mov    word ptr [ebx + 0x7e], ax
        stc    
        sti    
        pop    edi
        pop    esi
        ret    
sub_1_3DA6:
        push   esi
        push   edi
        push   ecx
        sbb    ecx, ecx
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_3DE7
        mov    byte ptr [esi + 0x34], 0x1a
        mov    word ptr [esi + 0x36], ax
        sub    eax, eax
        and    cl, 2
        mov    byte ptr [esi + 0x35], cl
        mov    word ptr [esi + 0x38], ax
        mov    word ptr [esi + 0x3a], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc    
        jmp    loc_1_3DFF
loc_1_3DE7:
        mov    word ptr [ebx + 0x7c], ax
        test   ecx, ecx
        jne    loc_1_3DF7
        or     word ptr [ebx + 0x6a], 0x80
        jmp    loc_1_3DFD
loc_1_3DF7:
        or     word ptr [ebx + 0x6a], 0x1000
loc_1_3DFD:
        sti    
        stc    
loc_1_3DFF:
        pop    ecx
        pop    edi
        pop    esi
        ret    
sub_1_3E03:
        push   esi
        push   edi
        mov    edi, dword ptr [ebx + 0x38]
        call   sub_1_3821
        jae    loc_1_3E3F
        mov    byte ptr [esi + 0x34], 0x1c
        mov    word ptr [esi + 0x38], ax
        sub    eax, eax
        mov    byte ptr [esi + 0x35], al
        mov    word ptr [esi + 0x36], ax
        mov    word ptr [esi + 0x3a], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc    
        pop    edi
        pop    esi
        ret    
loc_1_3E3F:
        or     word ptr [ebx + 0x6a], 0x200
        mov    word ptr [ebx + 0x66], ax
        sti    
        stc    
        pop    edi
        pop    esi
        ret    
sub_1_3E4E:
        push   esi
        push   edi
        mov    edi, dword ptr [ebx + 0x38]
        cli    
        lea    esi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_28E3
        stc    
        je     loc_1_3ED1
        mov    word ptr [esi + 0x10], 0x21
        mov    word ptr [esi + 0x38], ax
        mov    eax, dword ptr [edi + 0x34]
        and    eax, 0x400
        or     word ptr [esi + 0x10], ax
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [esi + 0x14], eax
        lea    eax, [ebx + 0x98]
        mov    dword ptr [esi + 0x18], eax
        xor    eax, eax
        mov    byte ptr [esi + 0x20], al
        mov    byte ptr [esi + 0x28], al
        mov    byte ptr [esi + 0x35], al
        mov    word ptr [esi + 0x36], ax
        mov    word ptr [esi + 0x3a], ax
        mov    word ptr [esi + 0x1e], ax
        movzx  eax, byte ptr [ebx + 0x91]
        mov    word ptr [esi + 0x1c], ax
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 0x30], ebx
        mov    word ptr [esi + 0x3c], 0x12
        mov    byte ptr [esi + 0x34], 0x14
        push   ebx
        mov    ebx, dword ptr [ebx + 0x38]
        cli    
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc    
loc_1_3ED1:
        pop    edi
        pop    esi
        ret    
loc_1_3ED4:
        stc    
        ret    
sub_1_3ED6:
        mov    eax, dword ptr [ebx + 0x3c]
        mov    edx, 0x2c
        call   sub_1_37A5
        jb     loc_1_3ED4
        mov    dword ptr [esi + 0x14], eax
        movzx  edx, word ptr [esi + 0x22]
        add    word ptr [esi + 0x22], 0x2c
        add    edx, dword ptr [esi + 0x24]
        mov    word ptr [edx], 0x2c
        mov    word ptr [edx + 2], 0xefff
        mov    byte ptr [edx + 4], 3
        sub    ecx, ecx
        mov    word ptr [edx + 8], cx
        mov    cl, byte ptr [ebx + 0x68]
        mov    word ptr [edx + 0xa], cx
        mov    cx, word ptr [ebx + 0x86]
        sub    cx, word ptr [ebx + 0x84]
        mov    word ptr [edx + 6], cx
        mov    cl, byte ptr [ebx + 0x66]
        mov    byte ptr [edx + 5], cl
        push   edi
        push   esi
        mov    ecx, 4
        lea    edi, [edx + 0xc]
        lea    esi, [ebx + 0x54]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xchg   ecx, eax
        lea    edi, [edx + 0x1c]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    esi, [ecx + 0x31]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        pop    esi
        pop    edi
        cli    
        push   edi
        lea    edi, [ecx + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [ecx], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        clc    
        ret    
sub_1_3F66:
        push   ebx
        cli    
        lea    ebx, [edi]
        cmp    ebx, dword ptr [ebx]
        clc    
        je     loc_1_3FA1
        mov    ebx, dword ptr [ebx]
        sti    
        call   sub_1_3821
        jae    loc_1_3FA1
        mov    byte ptr [esi + 0x34], 0x1f
        sub    eax, eax
        mov    byte ptr [esi + 0x35], al
        mov    word ptr [esi + 0x36], ax
        mov    word ptr [esi + 0x38], ax
        mov    word ptr [esi + 0x3a], ax
        cli    
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_2868
        pop    edi
        sti    
        push   edi
        call   sub_1_4E6C
        stc    
loc_1_3FA1:
        cmc    
        sti    
        pop    ebx
        ret    
        db  000h, 000h, 000h
sub_1_3FA8:
        push   eax
        push   edx
        cli    
        mov    edx, dword ptr [esi + 0x1c]
        mov    eax, edi
loc_1_3FB0:
        lea    ebx, [esi + 0x1c]
        call   sub_1_28F3
        je     loc_1_3FD9
        mov    ecx, 4
        push   esi
        lea    esi, [ebx + 0x54]
        mov    edi, eax
        cld    
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    esi
        je     loc_1_3FDC
        push   esi
        lea    esi, [esi + 0x1c]
        call   sub_1_2892
        pop    esi
        cmp    edx, ebx
        jne    loc_1_3FB0
loc_1_3FD9:
        stc    
        jmp    loc_1_3FE1
loc_1_3FDC:
        and    byte ptr [ebx + 0x24], 0xfd
        clc    
loc_1_3FE1:
        sti    
        mov    edi, eax
        pop    edx
        pop    eax
        ret    
sub_1_3FE7:
        push   eax
        push   ecx
        push   esi
        mov    al, byte ptr [edi + 6]
        sub    esi, esi
        mov    ecx, 0xfe
loc_1_3FF4:
        mov    ebx, dword ptr [esi + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_4022
        cmp    byte ptr [ebx + 0x69], al
        jne    loc_1_4022
        cmp    byte ptr [ebx + 0x25], 8
        jne    loc_1_4022
        push   esi
        push   edi
        push   ecx
        lea    esi, [ebx + 0x54]
        add    edi, 0x1c
        mov    ecx, 4
        cld    
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    ecx
        pop    edi
        pop    esi
        jne    loc_1_4022
        stc    
        jmp    loc_1_4028
loc_1_4022:
        add    esi, 4
        loop   loc_1_3FF4
        clc    
loc_1_4028:
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_1_402C:
        push   esi
        push   edi
        push   ecx
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x10]
        push   ecx
        shr    ecx, 2
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    edi
        pop    esi
        ret    8
sub_1_404A:
        cli    
        lea    ebx, [eax + 0xb4]
        call   sub_1_28F3
        lea    ebx, [ebx - 0x32]
        sti    
        je     loc_1_4098
        push   esi
        push   edi
        mov    esi, edi
        mov    edi, ebx
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 3], al
        mov    ax, word ptr [esi + 6]
        mov    word ptr [edi + 8], ax
        push   edi
        add    edi, 0x1a
        add    esi, 0xc
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    edi, 0x20
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        mov    byte ptr [edi + 1], 0
        mov    esi, edi
        call   sub_1_6BE9
        pop    edi
        pop    esi
loc_1_4098:
        ret    
loc_1_4099:
        ret    
loc_1_409A:
        cmp    byte ptr [ebx + 0x25], 0
        je     loc_1_40AB
        or     word ptr [ebx + 0x40], 0x40
        push   ebx
        call   sub_1_5BF3
loc_1_40AB:
        ret    
loc_1_40AC:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 4
        jne    loc_1_409A
        test   word ptr [ebx + 0x42], 0x40
        jne    loc_1_40D3
        test   word ptr [ebx + 0x42], 8
        je     loc_1_40D4
        or     word ptr [ebx + 0x40], 0x40
        call   sub_1_3CD2
        call   sub_1_0BAA
loc_1_40D3:
        ret    
loc_1_40D4:
        mov    edx, dword ptr [ebx + 0x38]
        movzx  ecx, byte ptr [edx + 0xb6]
        movzx  edx, word ptr [edx + 0x7a]
        sub    edx, ecx
        sub    edx, 0x12
        movzx  eax, word ptr [edi + 6]
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [ebx + 0x6e], ax
        and    word ptr [ebx + 0x40], 0xfffa
        mov    al, byte ptr [edi + 5]
        test   al, 0x80
        je     loc_1_4108
        or     word ptr [ebx + 0x40], 1
loc_1_4108:
        test   al, 1
        je     loc_1_4111
        or     word ptr [ebx + 0x40], 4
loc_1_4111:
        mov    byte ptr [ebx + 0x25], 5
        cli    
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_4154
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        mov    dl, byte ptr [ebx + 0x68]
        mov    esi, dword ptr [ebx + 0x88]
        mov    byte ptr [esi + 2], dl
        sub    al, al
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        push   eax
        mov    eax, dword ptr [ebx + 0x38]
        cmp    dword ptr [eax + 0x68], 0x3c
        jbe    loc_1_4153
        mov    byte ptr [ebx + 0x8f], 0
        mov    byte ptr [ebx + 0x8e], 0
loc_1_4153:
        pop    eax
loc_1_4154:
        ret    
loc_1_4155:
        test   word ptr [eax + 0x42], 0x4000
        jne    loc_1_418A
        cmp    byte ptr [eax + 0x25], 5
        jne    loc_1_418A
        or     word ptr [eax + 0x40], 0x20
        cmp    word ptr [edi + 6], 0
        je     loc_1_4174
        or     word ptr [eax + 0x40], 0x40
loc_1_4174:
        mov    edi, dword ptr [eax + 0x38]
        or     dword ptr [edi + 0x3c], 0x80
        push   edi
        call   sub_1_4E6C
        push   eax
        call   sub_1_5BF3
loc_1_418A:
        ret    
loc_1_418B:
        ret    
loc_1_418C:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 8
        jne    loc_1_418B
        test   word ptr [ebx + 0x42], 8
        je     loc_1_41A2
        call   sub_1_0C20
        ret    
loc_1_41A2:
        cli    
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_4211
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        mov    al, byte ptr [edi + 5]
        test   al, 0x80
        je     loc_1_41BE
        or     word ptr [ebx + 0x40], 1
loc_1_41BE:
        test   al, 1
        je     loc_1_41C7
        or     word ptr [ebx + 0x40], 4
loc_1_41C7:
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [ebx + 0x64], ax
        movzx  eax, word ptr [edi + 6]
        mov    edx, dword ptr [ebx + 0x38]
        movzx  ecx, byte ptr [edx + 0xb6]
        movzx  edx, word ptr [edx + 0x7a]
        sub    edx, ecx
        sub    edx, 0x12
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [ebx + 0x6e], ax
        mov    byte ptr [ebx + 0x25], 5
        call   sub_1_3C42
        mov    esi, dword ptr [ebx + 0x88]
        mov    dl, byte ptr [ebx + 0x68]
        mov    byte ptr [esi + 2], dl
        sub    al, al
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_4211:
        push   dword ptr [ebx + 0x38]
        call   sub_1_4E6C
        ret    
loc_1_421A:
        push   edi
        lea    edi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_2868
        pop    edi
        sti    
loc_1_4228:
        ret    
loc_1_4229:
        push   eax
        lea    esi, [edi + 0xc]
        call   sub_1_2480
        pop    eax
        jb     loc_1_4228
        call   sub_1_373E
        jb     loc_1_4228
        cli    
        test   dword ptr [0x6eec], 0x1000    ; -> obj1:0x6EEC
        jne    loc_1_421A
        or     dword ptr [0x6eec], 0x1000    ; -> obj1:0x6EEC
        sti    
        mov    dword ptr [esi + 0x14], eax
        or     word ptr [esi + 0x10], 0x1000
        movzx  edx, word ptr [edi + 6]
        and    dh, 0x3f
        mov    ebx, 0x7bd4                   ; -> obj1:0x7BD4
        mov    ecx, 0x258
        mov    dword ptr [esi + 0x2c], ebx
        mov    byte ptr [esi + 0x28], 0
        inc    word ptr [esi + 0x1e]
        push   edx
        push   ecx
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        push   esi
        mov    esi, dword ptr [esi + 0x14]
        sub    ah, ah
        mov    al, byte ptr [edi + 5]
        cmp    al, 1
        ja     loc_1_4297
        call   sub_1_1381
        sub    eax, eax
loc_1_4297:
        push   eax
        call   sub_1_146C
        pop    ebx
        add    edx, ebx
        pop    esi
        test   al, al
        jne    loc_1_431C
        add    esp, 8
loc_1_42A8:
        movzx  ebx, word ptr [esi + 0x22]
        add    ebx, dword ptr [esi + 0x24]
        mov    word ptr [ebx + 6], cx
        and    ch, 0x3f
        mov    word ptr [esi + 0x2a], cx
        add    word ptr [esi + 0x22], 0x2c
        mov    word ptr [ebx], 0x2c
        mov    word ptr [ebx + 2], 0xefff
        mov    byte ptr [ebx + 4], 0xf
        mov    byte ptr [ebx + 5], dl
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [ebx + 8], ax
        xor    eax, eax
        mov    word ptr [ebx + 0xa], ax
        push   esi
        lea    esi, [edi + 0xc]
        lea    edi, [ebx + 0x1c]
        mov    ecx, 4
        push   ecx
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    edi, [ebx + 0xc]
        pop    ecx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    ebx, dword ptr [esi + 0x14]
        cmp    dword ptr [ebx + 0x28], 1
        jne    loc_1_432C
loc_1_42FF:
        cli    
        push   edi
        lea    edi, [ebx + 0xbc]
        call   sub_1_2868
        pop    edi
        sti    
        or     dword ptr [ebx], 0x100
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        ret    
loc_1_431C:
        pop    ebx
        pop    eax
        cmp    eax, ebx
        jbe    loc_1_4327
        or     ch, 0x80
loc_1_4325:
        jmp    loc_1_42A8
loc_1_4327:
        or     ch, 0x40
        jmp    loc_1_4325
loc_1_432C:
        mov    edi, dword ptr [esi + 0x24]
        mov    ax, word ptr [esi + 0x22]
        add    ax, word ptr [esi + 0x2a]
        sub    ax, 0xe
        xchg   al, ah
        mov    word ptr [edi + 0xc], ax
        jmp    loc_1_42FF
loc_1_4343:
        mov    ecx, eax
        jmp    loc_1_4391
loc_1_4347:
        sti    
        ret    
loc_1_4349:
        movzx  ebx, word ptr [edi + 8]
        dec    ebx
        cmp    ebx, 0xfd
        ja     loc_1_4347
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_4347
        cmp    byte ptr [ebx + 0x25], 9
        jne    loc_1_4347
        cli    
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_4347
        mov    ecx, dword ptr [ebx + 0x88]
        mov    dword ptr [ecx + 0x3a], 0xffffffff
        sti    
        push   eax
        movzx  ecx, word ptr [edi + 6]
        and    ch, 0x3f
        movzx  eax, word ptr [ebx + 0x86]
        cmp    ecx, eax
        ja     loc_1_4343
loc_1_4391:
        pop    eax
        push   edi
        push   ebx
        mov    esi, ebx
        mov    ebx, edi
        add    ebx, 0x2c
        sub    ebx, edx
        mov    edx, dword ptr [esi + 0x80]
        mov    edi, eax
        call   sub_1_5F9D
        jb     loc_1_43D2
loc_1_43AC:
        pop    ebx
        mov    eax, edi
        pop    edi
        mov    dh, byte ptr [edi + 7]
        test   word ptr [ebx + 0x42], 8
        jne    loc_1_43C2
        test   dh, 0xc0
        jne    loc_1_43D4
        sub    al, al
loc_1_43C2:
        mov    esi, dword ptr [ebx + 0x88]
        mov    word ptr [esi + 8], cx
        call   sub_1_18D9
        ret    
loc_1_43D2:
        jmp    loc_1_43AC
loc_1_43D4:
        mov    esi, eax
        mov    al, 6
        test   dh, 0x40
        jne    loc_1_43C2
        cmp    byte ptr [edi + 5], 0
        je     loc_1_43C2
        add    word ptr [ebx + 0x84], cx
        add    dword ptr [ebx + 0x80], ecx
        sub    word ptr [ebx + 0x86], cx
        mov    al, byte ptr [edi + 5]
        xor    ah, ah
        mov    word ptr [ebx + 0x66], ax
        mov    eax, esi
        test   dword ptr [eax + 4], 1
        jne    loc_1_440F
        mov    al, 1
        jmp    loc_1_4424
loc_1_440F:
        test   dword ptr [eax + 4], 2
        je     loc_1_441F
        mov    al, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_4424
loc_1_441F:
        mov    al, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_4424:
        mov    byte ptr [ebx + 0x90], al
        call   sub_1_3ED6
        mov    eax, 0x1938                   ; -> loc_1_1938
        call   sub_1_34D0
        ret    
loc_1_443A:
        push   eax
        lea    esi, [edi + 0x1c]
        call   sub_1_2480
        jb     loc_1_4456
        test   byte ptr [ebx], 1
        jne    loc_1_4456
        mov    eax, dword ptr [ebx + 4]
        call   sub_1_3A64
        add    esp, 4
        ret    
loc_1_4456:
        pop    eax
        call   sub_1_404A
        ret    
loc_1_445D:
        push   eax
        lea    esi, [edi + 0x1c]
        call   sub_1_2480
        jb     loc_1_4474
        mov    eax, dword ptr [ebx + 4]
        call   sub_1_3A64
        add    esp, 4
        ret    
loc_1_4474:
        pop    eax
        call   sub_1_404A
        ret    
loc_1_447B:
        movzx  ebx, word ptr [edi + 8]
        dec    ebx
        cmp    ebx, 0xfd
        ja     loc_1_44CA
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_44CA
        cli    
        cmp    byte ptr [ebx + 0x25], 6
        jne    loc_1_44CA
        btr    word ptr [ebx + 0x40], 7
        jae    loc_1_44CA
        sti    
        mov    edi, dword ptr [ebx + 0x94]
        mov    byte ptr [edi + 3], 0
        lea    edi, [edi + 8]
        call   sub_1_2527
        mov    esi, dword ptr [ebx + 0x88]
        call   sub_1_235E
        mov    al, 0x16
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_44CA:
        sti    
        ret    
loc_1_44CC:
        ret    
loc_1_44CD:
        lea    esi, [edi + 0xc]
        call   sub_1_2480
        jb     loc_1_44CC
        or     byte ptr [ebx], 2
        mov    esi, ebx
        mov    al, 0x19
        call   sub_1_23E6
        mov    edi, 0x77c0                   ; -> obj1:0x77C0
        mov    ecx, 0x100
loc_1_44ED:
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        je     loc_1_454D
        cmp    dword ptr [ebx + 0x94], esi
        jne    loc_1_454D
        cli    
        cmp    byte ptr [ebx + 0x25], 5
        je     loc_1_454D
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_454D
        test   word ptr [ebx + 0x42], 0x40
        jne    loc_1_454D
        and    word ptr [ebx + 0x40], 0xff7f
        sti    
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebx + 0x88]
        mov    byte ptr [esi + 1], 0x19
        call   sub_1_6BE9
        pop    ebx
        pop    edi
        pop    esi
        cmp    byte ptr [ebx + 0x25], 1
        je     loc_1_4548
        cmp    byte ptr [ebx + 0x25], 7
        je     loc_1_4548
        or     word ptr [ebx + 0x42], 0x4000
        push   ebx
        call   sub_1_5BF3
        jmp    loc_1_454D
loc_1_4548:
        call   sub_1_235E
loc_1_454D:
        sti    
        add    edi, 4
        loop   loc_1_44ED
        ret    
loc_1_4554:
        lea    esi, [edi + 0xc]
        push   eax
        call   sub_1_2480
        jb     loc_1_457B
        add    esp, 4
        mov    al, byte ptr [edi + 6]
        or     al, al
        jne    loc_1_45A5
loc_1_4569:
        xor    ah, ah
        test   byte ptr [ebx], 1
        je     loc_1_4572
        inc    ah
loc_1_4572:
        mov    ebx, dword ptr [ebx + 4]
        call   sub_1_3AF4
        ret    
loc_1_457B:
        pop    eax
        call   sub_1_404A
        ret    
loc_1_4582:
        cli    
        lea    ebx, [esi + 0x24]
        call   sub_1_28F3
        je     loc_1_4594
        and    byte ptr [ebx + 0x24], 0xfd
        sti    
        jmp    loc_1_45BE
loc_1_4594:
        sti    
        push   eax
        mov    eax, dword ptr [esi + 4]
        call   sub_1_404A
        pop    eax
        mov    ebx, esi
        xor    al, al
        jmp    loc_1_4569
loc_1_45A5:
        mov    esi, ebx
        call   sub_1_3FE7
        jb     loc_1_46A9
        push   edi
        lea    edi, [edi + 0x1c]
        call   sub_1_3FA8
        pop    edi
        jb     loc_1_4582
loc_1_45BE:
        cli    
        mov    ecx, dword ptr [ebx + 0x88]
        mov    dword ptr [ecx + 0x3a], 0x1ee1 ; -> loc_1_1EE1
        push   ecx
        push   ebx
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_2671
        shl    eax, 1
        call   sub_1_265A
        xchg   ecx, eax
        call   sub_1_265A
        add    eax, ecx
        add    eax, dword ptr [0x6f1b]       ; -> loc_1_6F1B
        test   dword ptr [ebx + 4], 2
        je     loc_1_4616
        mov    eax, 0x7d0
        test   dword ptr [ebx + 4], 1
        jne    loc_1_4607
        mov    eax, 0x64
loc_1_4607:
        call   sub_1_265A
        movzx  ecx, byte ptr [0x6f21]        ; -> loc_1_6F21
        imul   eax, ecx
loc_1_4616:
        xor    ecx, ecx
        test   dword ptr [ebx + 4], 1
        jne    loc_1_4625
        mov    cl, 1
        jmp    loc_1_463C
loc_1_4625:
        test   dword ptr [ebx + 4], 2
        je     loc_1_4636
        mov    cl, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_463C
loc_1_4636:
        mov    cl, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_463C:
        pop    ebx
        pop    ecx
        mov    dword ptr [ecx + 0x32], eax
        mov    byte ptr [ebx + 0x25], 8
        sti    
        mov    dword ptr [ebx + 0x38], 0
        movzx  eax, byte ptr [edi + 5]
        mov    word ptr [ebx + 0xca], ax
        mov    ecx, 4
        push   esi
        push   edi
        lea    esi, [edi + 0x1c]
        lea    edi, [ebx + 0x54]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        mov    ecx, 4
        push   edi
        lea    esi, [edi + 0xc]
        lea    edi, [ebx + 0x44]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [ebx + 0x88]
        push   esi
        mov    ecx, 4
        lea    edi, [esi + 0xa]
        lea    esi, [ebx + 0x54]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    ecx, 4
        lea    edi, [esi + 0x1a]
        lea    esi, [ebx + 0x44]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [ebx + 0x64], ax
        mov    al, byte ptr [edi + 6]
        mov    byte ptr [ebx + 0x69], al
loc_1_46A9:
        mov    al, byte ptr [ebx + 0x68]
        xor    ah, ah
        test   byte ptr [esi], 1
        je     loc_1_46B5
        inc    ah
loc_1_46B5:
        push   ebx
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_3AF4
        cmp    dword ptr [ebx + 0x28], 1
        pop    ebx
        je     loc_1_46CB
        mov    word ptr [ebx + 0x6e], 0x5ca
loc_1_46CB:
        ret    
loc_1_46CC:
        dec    byte ptr [ebx + 0x90]
        js     loc_1_46EE
        mov    esi, dword ptr [ebx + 0x94]
        mov    eax, dword ptr [esi + 4]
        call   sub_1_38B0
        mov    ecx, eax
        mov    eax, 0x46cc                   ; -> loc_1_46CC
        jmp    sub_1_34D0
loc_1_46EE:
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_46F5
        ret    
loc_1_46F5:
        cmp    byte ptr [ebx + 0x25], 7
        jne    loc_1_4701
        call   sub_1_235E
        ret    
loc_1_4701:
        or     word ptr [ebx + 0x42], 0x4000
        push   ebx
        call   sub_1_5BF3
        ret    
sub_1_470E:
        test   word ptr [ebx + 0x40], 0x40
        je     loc_1_4718
        stc    
        ret    
loc_1_4718:
        push   eax
        push   esi
        push   edi
        push   ebx
        test   word ptr [ebx + 0x42], 0x20
        je     loc_1_472C
        cmp    byte ptr [edi + 7], 1
        je     loc_1_4759
        jmp    loc_1_475E
loc_1_472C:
        mov    esi, eax
        mov    edi, edx
        cmp    dword ptr [esi + 0x28], 0
        je     loc_1_4743
        mov    eax, dword ptr [edi + 8]
        and    al, 0x7f
        add    edi, 2
        add    ebx, 2
        jmp    loc_1_4746
loc_1_4743:
        mov    eax, dword ptr [edi + 6]
loc_1_4746:
        cmp    eax, dword ptr [ebx + 0x98]
        jne    loc_1_475E
        mov    eax, dword ptr [edi + 8]
        cmp    eax, dword ptr [ebx + 0x9a]
        jne    loc_1_475E
loc_1_4759:
        pop    ebx
        pop    edi
        pop    esi
        pop    eax
        ret    
loc_1_475E:
        pop    ebx
        pop    edi
        pop    esi
        pop    eax
        or     word ptr [ebx + 0x42], 0x40
        call   sub_1_38B0
        mov    ecx, eax
        test   dword ptr [eax + 4], 1
        jne    loc_1_477B
        mov    al, 1
        jmp    loc_1_4790
loc_1_477B:
        test   dword ptr [eax + 4], 2
        je     loc_1_478B
        mov    al, byte ptr [0x6f21]         ; -> loc_1_6F21
        jmp    loc_1_4790
loc_1_478B:
        mov    al, byte ptr [0x6f1f]         ; -> loc_1_6F1F
loc_1_4790:
        mov    byte ptr [ebx + 0x90], al
        mov    eax, 0x46cc                   ; -> loc_1_46CC
        call   sub_1_34D0
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_47A8
        stc    
        ret    
loc_1_47A8:
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_47C5
        mov    esi, dword ptr [ebx + 0x88]
        and    word ptr [ebx + 0x40], 0xff7f
        mov    byte ptr [esi + 1], 0x19
        call   sub_1_6BE9
loc_1_47C5:
        stc    
        ret    
loc_1_47C7:
        movzx  ebx, byte ptr [edi + 8]
        dec    ebx
        cmp    ebx, 0xfd
        ja     loc_1_4827
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_4827
        cmp    byte ptr [ebx + 0x25], 0
        je     loc_1_4827
        test   word ptr [ebx + 0x40], 8
        je     loc_1_47F5
        call   sub_1_470E
        jae    loc_1_47F5
        ret    
loc_1_47F5:
        cmp    byte ptr [edi + 7], 0
        je     loc_1_4800
        or     word ptr [ebx + 0x42], 0x20
loc_1_4800:
        push   eax
        movzx  eax, byte ptr [edi + 5]
        mov    word ptr [ebx + 0xca], ax
        pop    eax
        or     word ptr [ebx + 0x40], 8
        cli    
        test   word ptr [ebx + 0x40], 0x80
        je     loc_1_4827
        cmp    byte ptr [ebx + 0x25], 1
        je     loc_1_4857
        cmp    byte ptr [ebx + 0x25], 7
        je     loc_1_4828
loc_1_4827:
        ret    
loc_1_4828:
        ret    
loc_1_4829:
        sti    
        pop    eax
        mov    esi, eax
        mov    edi, edx
        cmp    dword ptr [esi + 0x28], 0
        je     loc_1_484C
        mov    eax, dword ptr [edi + 8]
        and    al, 0x7f
loc_1_483A:
        mov    dword ptr [ebx + 0x9a], eax
        mov    ax, word ptr [edi + 0xc]
        mov    word ptr [ebx + 0x9e], ax
        ret    
loc_1_484C:
        mov    eax, dword ptr [edi + 6]
        sub    edi, 2
        sub    ebx, 2
        jmp    loc_1_483A
loc_1_4857:
        push   eax
        mov    al, byte ptr [edi + 6]
        or     al, al
        je     loc_1_4829
        mov    ecx, dword ptr [ebx + 0x88]
        mov    esi, dword ptr [0x6f17]       ; -> loc_1_6F17
        mov    dword ptr [ecx + 0x32], esi
        mov    dword ptr [ecx + 0x3a], 0xb6a ; -> loc_1_0B6A
        sti    
        mov    byte ptr [ebx + 0x69], al
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [ebx + 0x64], ax
        pop    eax
        push   ebx
        push   edx
        push   edi
        mov    edi, edx
        cli    
        call   sub_1_236B
        jae    loc_1_4901
        cli    
        push   eax
        mov    eax, 0xc8
        call   sub_1_2233
        mov    esi, eax
        pop    eax
        test   esi, esi
        je     loc_1_48F6
        mov    dword ptr [esi + 0x40], eax
        call   sub_1_27B0
        sti    
        jb     loc_1_48F6
        mov    esi, ebx
        pop    edi
        pop    edx
        pop    ebx
        mov    eax, esi
        call   sub_1_6298
        mov    dword ptr [ebx + 0x38], esi
        mov    byte ptr [ebx + 0x25], 3
        mov    edi, dword ptr [ebx + 0x94]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x40], eax
        cli    
        or     byte ptr [ebx + 0x24], 2
        push   esi
        lea    esi, [esi]
        call   sub_1_2892
        pop    esi
        sti    
        mov    byte ptr [esi + 0x38], 1
        mov    dword ptr [esi + 0x3c], 8
        xchg   ebx, esi
        call   sub_1_2959
        xchg   ebx, esi
        push   esi
        call   sub_1_4E6C
        ret    
loc_1_48F6:
        pop    edi
        pop    edx
        pop    ebx
        mov    al, 9
        call   sub_1_0BAA
        ret    
loc_1_4901:
        mov    esi, ebx
        pop    edi
        pop    edx
        pop    ebx
        mov    eax, esi
        call   sub_1_6298
        or     byte ptr [ebx + 0x24], 2
        push   esi
        lea    esi, [esi]
        call   sub_1_2892
        pop    esi
        mov    dword ptr [ebx + 0x38], esi
        cmp    byte ptr [esi + 0x38], 4
        jne    loc_1_493A
        sti    
        mov    byte ptr [ebx + 0x25], 4
        mov    eax, dword ptr [esi + 0x40]
        xchg   ebx, esi
        call   sub_1_2959
        xchg   ebx, esi
        call   sub_1_3BB8
        ret    
loc_1_493A:
        mov    byte ptr [ebx + 0x25], 3
        cmp    byte ptr [esi + 0x38], 1
        je     loc_1_494B
        or     dword ptr [esi + 0x34], 0x800
loc_1_494B:
        sti    
        ret    
loc_1_494D:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_495A
        call   sub_1_569A
loc_1_495A:
        ret    
loc_1_495B:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_49A2
        test   word ptr [ebx + 0x40], 0x20
        jne    loc_1_49A2
        test   byte ptr [edi + 5], 8
        je     loc_1_4976
        call   sub_1_569A
loc_1_4976:
        test   word ptr [ebx + 0x40], 0x4000
        jne    loc_1_49AD
loc_1_497E:
        and    word ptr [ebx + 0x40], 0xbfff
        call   sub_1_5CEC
        jb     loc_1_49B5
        call   sub_1_608F
        jb     loc_1_49DC
        cmp    word ptr [ebx + 0x86], 0
        je     loc_1_49DC
        test   byte ptr [edi + 5], 1
        jne    loc_1_49A3
loc_1_49A2:
        ret    
loc_1_49A3:
        mov    ax, word ptr [edi + 0xa]
        call   sub_1_3E03
        ret    
loc_1_49AD:
        cmp    word ptr [edi + 6], 1
        je     loc_1_497E
        ret    
loc_1_49B5:
        or     word ptr [ebx + 0x40], 0xc000
        mov    eax, dword ptr [ebx + 0x38]
        mov    eax, dword ptr [eax + 0x40]
        inc    dword ptr [eax + 0x100]
        call   sub_1_5C40
        test   byte ptr [edi + 5], 2
        jne    loc_1_49D3
        ret    
loc_1_49D3:
        xor    eax, eax
        stc    
        call   sub_1_3DA6
        ret    
loc_1_49DC:
        or     word ptr [ebx + 0x40], 0x4000
        mov    eax, dword ptr [ebx + 0x38]
        mov    eax, dword ptr [eax + 0x40]
        inc    dword ptr [eax + 0x100]
        movzx  eax, word ptr [ebx + 0x84]
        push   eax
        test   byte ptr [edi + 5], 2
        jne    loc_1_4A23
        mov    al, 6
        call   sub_1_5744
        call   sub_1_5CEC
        pop    eax
        jb     loc_1_4A11
        call   sub_1_3D51
        ret    
loc_1_4A11:
        clc    
        call   sub_1_3DA6
loc_1_4A17:
        or     word ptr [ebx + 0x40], 0x8000
        call   sub_1_5C40
        ret    
loc_1_4A23:
        pop    eax
        stc    
        call   sub_1_3DA6
        mov    al, 6
        call   sub_1_5744
        call   sub_1_5CEC
        jb     loc_1_4A17
        xor    eax, eax
        call   sub_1_3D51
        ret    
loc_1_4A40:
        cmp    word ptr [edi + 6], 1
        je     loc_1_4A6F
        test   byte ptr [edi + 5], 8
        je     loc_1_4A52
        call   sub_1_569A
loc_1_4A52:
        ret    
loc_1_4A53:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_4ADF
        test   word ptr [ebx + 0x40], 0x20
        jne    loc_1_4AE0
        test   word ptr [ebx + 0x40], 0x4000
        jne    loc_1_4A40
loc_1_4A6F:
        and    word ptr [ebx + 0x40], 0xbfff
        call   sub_1_5CEC
        jb     loc_1_4AEE
        call   sub_1_608F
        jb     loc_1_4B1F
        mov    cl, byte ptr [edi + 5]
        mov    ch, 0xff
        test   cl, 2
        jne    loc_1_4ABA
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [ebx + 0x64], ax
        test   cl, 4
        je     loc_1_4ABC
        test   word ptr [ebx + 0x6a], 0x20
        jne    loc_1_4ABC
        cmp    byte ptr [0x6f32], 0          ; -> obj1:0x6F32
        je     loc_1_4ABC
        or     word ptr [ebx + 0x6a], 4
        inc    dword ptr [0x6eff]            ; -> loc_1_6EFF
loc_1_4ABA:
        xor    ch, ch
loc_1_4ABC:
        sub    al, al
        call   sub_1_5744
        test   cl, 8
        je     loc_1_4ACD
        call   sub_1_569A
loc_1_4ACD:
        or     ch, ch
        jne    loc_1_4AD2
        ret    
loc_1_4AD2:
        mov    ax, word ptr [ebx + 0x64]
        call   sub_1_3E4E
        jb     loc_1_4AE1
        sti    
        ret    
loc_1_4ADF:
        ret    
loc_1_4AE0:
        ret    
loc_1_4AE1:
        or     word ptr [ebx + 0x6a], 4
        inc    dword ptr [0x6eff]            ; -> loc_1_6EFF
        sti    
        ret    
loc_1_4AEE:
        or     word ptr [ebx + 0x40], 0xc000
        mov    eax, dword ptr [ebx + 0x38]
        mov    eax, dword ptr [eax + 0x40]
        inc    dword ptr [eax + 0x100]
        call   sub_1_5C40
        test   byte ptr [edi + 5], 8
        je     loc_1_4B10
        call   sub_1_569A
loc_1_4B10:
        test   byte ptr [edi + 5], 2
        jne    loc_1_4B17
        ret    
loc_1_4B17:
        xor    eax, eax
        stc    
        jmp    sub_1_3DA6
loc_1_4B1F:
        or     word ptr [ebx + 0x40], 0x4000
        mov    eax, dword ptr [ebx + 0x38]
        mov    eax, dword ptr [eax + 0x40]
        inc    dword ptr [eax + 0x100]
        movzx  eax, word ptr [ebx + 0x84]
        push   eax
        mov    al, 6
        call   sub_1_5744
        test   byte ptr [edi + 5], 8
        je     loc_1_4B4B
        call   sub_1_569A
loc_1_4B4B:
        test   byte ptr [edi + 5], 2
        jne    loc_1_4B5F
        call   sub_1_5CEC
        pop    eax
        jb     loc_1_4B74
loc_1_4B59:
        call   sub_1_3D51
        ret    
loc_1_4B5F:
        pop    eax
        stc    
        call   sub_1_3DA6
        call   sub_1_5CEC
        mov    eax, 0
        jae    loc_1_4B59
        jmp    loc_1_4B7A
loc_1_4B74:
        clc    
        call   sub_1_3DA6
loc_1_4B7A:
        or     word ptr [ebx + 0x40], 0x8000
        call   sub_1_5C40
        ret    
sub_1_4B86:
        mov    edi, dword ptr [ebx + 0x94]
        lea    esi, [ebx + 0x28]
        cmp    esi, dword ptr [esi]
        je     loc_1_4BF3
        cmp    word ptr [ebx + 0x84], 0
        jne    loc_1_4BB1
        mov    al, 7
        call   sub_1_5744
        cmp    esi, dword ptr [esi]
        je     loc_1_4BAF
        push   ecx
        call   sub_1_5CEC
        pop    ecx
loc_1_4BAF:
        clc    
        ret    
loc_1_4BB1:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        mov    esi, dword ptr [esi]
        cmp    eax, esi
        je     loc_1_4BF3
        push   esi
        push   edi
        lea    esi, [esi]
        cli    
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        sti    
        pop    edi
        pop    esi
        dec    byte ptr [ebx + 0x8c]
        mov    al, byte ptr [ebx + 0x68]
        mov    ah, byte ptr [edi + 2]
        lea    edi, [esi - 0x32]
        mov    word ptr [edi + 2], ax
loc_1_4BE0:
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    esi, edi
        mov    byte ptr [esi + 1], 7
        call   sub_1_6BE9
        clc    
        ret    
loc_1_4BF3:
        cli    
        lea    esi, [edi + 0x34]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_4C0F
        dec    byte ptr [edi + 0x19]
        mov    edi, esi
loc_1_4C07:
        mov    al, byte ptr [ebx + 0x68]
        mov    byte ptr [edi + 2], al
        jmp    loc_1_4BE0
loc_1_4C0F:
        mov    al, byte ptr [edi + 2]
        mov    edi, dword ptr [edi + 4]
        cli    
        lea    esi, [edi + 0x9c]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_4C2E
        mov    edi, esi
        mov    byte ptr [edi + 3], al
        jmp    loc_1_4C07
loc_1_4C2E:
        stc    
        ret    
loc_1_4C30:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_4C4E
        or     word ptr [ebx + 0x40], 2
        test   byte ptr [edi + 5], 2
        jne    loc_1_4C4F
        mov    cx, word ptr [edi + 6]
loc_1_4C47:
        mov    word ptr [ebx + 0xcc], cx
loc_1_4C4E:
        ret    
loc_1_4C4F:
        xor    ecx, ecx
        call   sub_1_4B86
        jae    loc_1_4C47
        or     word ptr [ebx + 0x42], 4
        call   sub_1_5C40
        jmp    loc_1_4C47
loc_1_4C64:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_4C7F
        and    word ptr [ebx + 0x40], 0xfffd
        push   ebx
        call   sub_1_57BB
        push   dword ptr [ebx + 0x38]
        call   sub_1_4E6C
loc_1_4C7F:
        ret    
loc_1_4C80:
        mov    ebx, eax
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_4CCA
        movzx  ecx, word ptr [edi + 6]
        test   word ptr [ebx + 0x40], 2
        je     loc_1_4C9B
        movzx  ecx, word ptr [ebx + 0xcc]
loc_1_4C9B:
        test   word ptr [ebx + 0x40], 0x400
        je     loc_1_4CB2
        test   word ptr [ebx + 0x42], 0x200
        jne    loc_1_4CB2
        call   sub_1_5E8F
        jb     loc_1_4CB7
loc_1_4CB2:
        or     word ptr [ebx + 0x42], 1
loc_1_4CB7:
        and    word ptr [ebx + 0x40], 0xfffd
        push   ebx
        call   sub_1_57BB
        push   dword ptr [ebx + 0x38]
        call   sub_1_4E6C
loc_1_4CCA:
        ret    
loc_1_4CCB:
        lea    esi, [edi + 0xc]
        push   eax
        call   sub_1_2480
        jae    loc_1_4D06
        pop    eax
        cli    
        lea    esi, [eax + 0xac]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_4D05
        push   esi
        push   edi
        mov    edi, esi
        add    edi, 0x1a
        pop    esi
        add    esi, 0xc
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, esi
        sub    edi, 0x1c
        pop    esi
        sub    cl, cl
        mov    ebx, eax
        jmp    loc_1_4D3C
loc_1_4D05:
        ret    
loc_1_4D06:
        add    esp, 4
        mov    cl, byte ptr [ebx + 2]
        cli    
        lea    esi, [ebx + 0x2c]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        jne    loc_1_4D31
        mov    ebx, dword ptr [ebx + 4]
        cli    
        lea    esi, [ebx + 0x94]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        jne    loc_1_4D3C
        ret    
loc_1_4D31:
        dec    byte ptr [ebx + 0x18]
        test   byte ptr [ebx], 2
        jne    loc_1_4D70
        mov    ebx, dword ptr [ebx + 4]
loc_1_4D3C:
        push   edi
        mov    eax, edi
        sub    eax, edx
        add    eax, 0x2c
        mov    edi, ebx
        mov    ebx, eax
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    byte ptr [esi + 3], cl
        call   sub_1_60E7
        mov    edx, dword ptr [esi + 4]
        movzx  ecx, word ptr [esi + 8]
        push   esi
        call   sub_1_60D4
        pop    esi
        mov    byte ptr [esi + 1], al
        mov    word ptr [esi + 8], cx
        jmp    sub_1_6BE9
loc_1_4D70:
        mov    edi, esi
        mov    al, 0x19
        cli    
        call   sub_1_2913
        sti    
        ret    
loc_1_4D7C:
        add    esp, 0x14
        pop    ebp
        ret    
loc_1_4D81:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    ebx, eax
        mov    dword ptr [ebp - 0x12], edi
        cli    
        lea    esi, [ebx + 0xa4]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_4D7C
        mov    ecx, edi
        sub    ecx, edx
        add    ecx, 0x2c
        push   ecx
        mov    dword ptr [ebp - 4], esi
        mov    eax, esi
        mov    esi, ebx
        mov    bl, byte ptr [eax + 3]
        call   sub_1_24CC
        dec    byte ptr [ebx + 0x18]
        mov    ebx, esi
        mov    esi, eax
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        push   dword ptr [ebp - 0x12]
        call   sub_1_60E7
        mov    edi, ebx
        pop    ebx
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 8], edx
        movzx  ecx, word ptr [esi + 8]
        push   ebp
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        pop    ebx
        call   sub_1_60D4
        pop    ebp
        mov    byte ptr [esi + 1], al
        mov    word ptr [esi + 8], cx
        mov    byte ptr [ebp - 0xe], al
        mov    dword ptr [ebp - 0xc], ecx
loc_1_4DF6:
        cli    
        lea    esi, [edi + 0xa4]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_4E5C
        mov    eax, esi
        mov    esi, edi
        mov    edi, eax
        push   ebx
        mov    bl, byte ptr [edi + 3]
        call   sub_1_24CC
        dec    byte ptr [ebx + 0x18]
        pop    ebx
        xchg   esi, edi
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        push   dword ptr [ebp - 0x12]
        call   sub_1_60E7
        movzx  ecx, word ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0xc]
        sub    ecx, eax
        jae    loc_1_4E3A
        mov    byte ptr [ebp - 0xe], 6
loc_1_4E3A:
        sbb    edx, edx
        and    ecx, edx
        add    ecx, eax
        mov    word ptr [esi + 8], cx
        push   dword ptr [esi + 4]
        push   dword ptr [ebp - 8]
        call   sub_1_402C
        mov    al, byte ptr [ebp - 0xe]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        jmp    loc_1_4DF6
loc_1_4E5C:
        mov    esi, dword ptr [ebp - 4]
        call   sub_1_6BE9
        add    esp, 0x14
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_1_4E6C:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cli    
        test   byte ptr [edi + 0xb8], 1
        jne    loc_1_4EA8
        or     byte ptr [edi + 0xb8], 1
        push   ebx
        mov    ebx, dword ptr [edi + 0x40]
        push   edi
        push   eax
        lea    eax, [ebx + 0xc4]
        lea    edi, [edi + 0x24]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebx + 0xc8]
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [ebx + 0xc8], eax
        pop    eax
        pop    edi
        pop    ebx
loc_1_4EA8:
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        sti    
        pop    edi
        ret    4
sub_1_4EB4:
        push   esi
        mov    esi, dword ptr [edi + 0x40]
        cli    
        dec    byte ptr [esi + 0x30]
        js     loc_1_4F1F
        sti    
        movzx  ebx, byte ptr [edi + 0xb6]
        mov    word ptr [ebx + edi + 0x8c], dx
        mov    word ptr [ebx + edi + 0x8e], ax
        lea    eax, [edi + 0x8c]
        mov    dword ptr [esi + 0x60], eax
        mov    word ptr [esi + 0x64], bx
        add    word ptr [esi + 0x64], cx
        movzx  ebx, byte ptr [edi + 0xb7]
        xchg   cl, ch
        mov    word ptr [ebx + edi + 0x8c], cx
        mov    eax, dword ptr [edi + 0x34]
        and    eax, 0x400
        or     word ptr [esi + 0x58], ax
        push   esi
        lea    esi, [esi + 0x48]
        mov    dword ptr [esi + 0xc], edi
        call   sub_1_0517
        pop    esi
        and    word ptr [esi + 0x58], 0xfbff
        cmp    eax, 2
        ja     loc_1_4F25
        pop    esi
        clc    
        ret    
loc_1_4F1F:
        inc    byte ptr [esi + 0x30]
        or     dword ptr [esi], 0x10
loc_1_4F25:
        cli    
        test   byte ptr [edi + 0xb8], 1
        jne    loc_1_4F48
        or     byte ptr [edi + 0xb8], 1
        push   esi
        lea    esi, [esi + 0xc4]
        push   edi
        lea    edi, [edi + 0x24]
        call   sub_1_28BB
        pop    edi
        pop    esi
loc_1_4F48:
        sti    
        pop    esi
        stc    
        ret    
loc_1_4F4C:
        or     dword ptr [ecx], 0x10
        test   byte ptr [edi + 0xb8], 1
        jne    loc_1_4F71
        or     byte ptr [edi + 0xb8], 1
        push   esi
        lea    esi, [ecx + 0xc4]
        push   edi
        lea    edi, [edi + 0x24]
        call   sub_1_28BB
        pop    edi
        pop    esi
loc_1_4F71:
        inc    byte ptr [ecx + 0x30]
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_28A7
        pop    edi
loc_1_4F7E:
        sti    
        stc    
        ret    
sub_1_4F81:
        cli    
        push   eax
        mov    esi, dword ptr [edi + 8]
        mov    esi, dword ptr [esi]
        lea    eax, [edi + 8]
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [edi + 8], esi
        cmp    esi, eax
        pop    eax
        je     loc_1_4F7E
sub_1_4F96:
        mov    ecx, dword ptr [edi + 0x40]
        dec    byte ptr [ecx + 0x30]
        js     loc_1_4F4C
        test   dword ptr [edi + 0x34], 0xa
        jne    loc_1_4F71
        mov    ecx, dword ptr [esi + 0x18]
        movzx  edx, byte ptr [edi + 0xb6]
        add    ecx, edx
        mov    dx, word ptr [edi + 0x44]
        add    dx, dx
        mov    word ptr [ecx + 2], dx
        and    byte ptr [ecx + 1], 0xfe
        mov    al, 0x81
        add    al, byte ptr [edi + 0x44]
        sub    al, byte ptr [edi + 0x46]
        and    al, 0x7f
        cmp    byte ptr [edi + 0x48], al
        jbe    loc_1_5082
loc_1_4FD5:
        push   esi
        lea    esi, [esi + 0x34]
        cld    
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ecx + 8], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ecx + 0xc], eax
        pop    esi
        mov    ecx, dword ptr [esi + 0x18]
        movzx  eax, byte ptr [edi + 0xb7]
        add    ecx, eax
        mov    ax, word ptr [esi + 0x3c]
        xchg   al, ah
        mov    word ptr [ecx], ax
        mov    al, byte ptr [edi + 0x44]
        inc    al
        and    al, 0x7f
        mov    byte ptr [edi + 0x44], al
        mov    ah, byte ptr [edi + 0x46]
        mov    cl, al
        mov    ch, byte ptr [edi + 0x81]
        sub    cl, ah
        sub    ch, ah
        and    cx, 0x7f7f
        cmp    ch, cl
        ja     loc_1_5020
        mov    byte ptr [edi + 0x81], al
loc_1_5020:
        inc    byte ptr [edi + 0xba]
        test   dword ptr [edi + 0x34], 0x4000
        jne    loc_1_506D
loc_1_502F:
        mov    al, byte ptr [edi + 0x45]
        mov    byte ptr [edi + 0x47], al
        lea    eax, [edi + 0x10]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0x14], esi
        and    dword ptr [edi + 0x3c], 0xffffff7f
        mov    dword ptr [edi + 0x60], 0xffffffff
        btr    word ptr [esi + 0x10], 2
        jae    loc_1_506B
        mov    ecx, dword ptr [esi + 0x30]
        inc    byte ptr [ecx + 0x6c]
        mov    al, byte ptr [0x6f2f]         ; -> loc_1_6F2F
        cmp    byte ptr [ecx + 0x6c], al
        je     loc_1_50B7
loc_1_506B:
        clc    
        ret    
loc_1_506D:
        cmp    byte ptr [edi + 0xba], 0x64
        jb     loc_1_502F
        push   ebx
        mov    ebx, edi
        cli    
        call   sub_1_1EE8
        pop    ebx
        jmp    loc_1_502F
loc_1_5082:
        cmp    byte ptr [esi + 0x34], 0x16
        je     loc_1_4FD5
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        lea    eax, [edi + 0xc4]
        cmp    eax, dword ptr [edi + 0xc4]
        pop    edi
        jne    loc_1_4FD5
        or     byte ptr [ecx + 3], 1
        or     dword ptr [edi + 0x34], 0x2008
        and    dword ptr [edi + 0x3c], 0xfffffffe
        jmp    loc_1_4FD5
loc_1_50B7:
        or     dword ptr [0x6eec], 0x20      ; -> obj1:0x6EEC
        test   byte ptr [ecx + 0x24], 8
        jne    loc_1_506B
        push   ecx
        push   eax
        lea    eax, [0x6fa0]                 ; -> obj1:0x6FA0
        lea    ecx, [ecx + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0x6fa4]       ; -> obj1:0x6FA4
        mov    dword ptr [ecx + 4], eax
        xchg   ecx, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x6fa4], eax       ; -> obj1:0x6FA4
        pop    eax
        pop    ecx
        or     byte ptr [ecx + 0x24], 8
        jmp    loc_1_506B
sub_1_50E9:
        mov    dword ptr [esi + 0xc], edi
        sti    
        call   sub_1_0517
        mov    cl, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [edi + 0xb9], cl
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     loc_1_510C
loc_1_510C:
        push   eax
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], eax
        pop    eax
        cmp    eax, 2
        ja     loc_1_511B
        clc    
        ret    
loc_1_511B:
        and    dword ptr [edi + 0x34], 0xfffffff7
        stc    
        ret    
sub_1_5121:
        test   dword ptr [edi + 0x3c], 0x10
        je     loc_1_51A2
        mov    al, 5
        mov    ah, byte ptr [edi + 0x45]
        add    ah, ah
        or     ah, 1
        test   dword ptr [edi + 0x34], 1
        jne    loc_1_516E
        mov    al, 1
        mov    ebx, eax
        call   sub_1_4F81
        mov    eax, ebx
        jb     loc_1_516E
        test   dword ptr [edi + 0x34], 8
        jne    loc_1_5199
        mov    ebx, dword ptr [esi + 0x18]
        movzx  ecx, byte ptr [edi + 0xb6]
        add    ebx, ecx
        or     byte ptr [ebx + 3], 1
        or     byte ptr [ebx + 1], 1
        call   sub_1_50E9
        jae    loc_1_5181
loc_1_516E:
        mov    ecx, 4
        mov    dx, word ptr [edi + 0x78]
        or     dh, 1
        call   sub_1_4EB4
        jb     loc_1_5198
loc_1_5181:
        cli    
        and    dword ptr [edi + 0x3c], 0xffffff6f
loc_1_5189:
        mov    al, byte ptr [edi + 0x45]
        mov    byte ptr [edi + 0x47], al
        sti    
        mov    dword ptr [edi + 0x60], 0xffffffff
        clc    
loc_1_5198:
        ret    
loc_1_5199:
        call   sub_1_50E9
        cli    
        jae    loc_1_5189
        sti    
loc_1_51A2:
        stc    
        ret    
sub_1_51A4:
        test   dword ptr [edi + 0x3c], 0x80
        je     loc_1_5208
        mov    al, 5
        mov    ah, byte ptr [edi + 0x45]
        add    ah, ah
        test   dword ptr [edi + 0x34], 1
        jne    loc_1_51DD
        mov    al, 1
        mov    ebx, eax
        call   sub_1_4F81
        mov    eax, ebx
        jb     loc_1_51DD
        mov    ebx, dword ptr [esi + 0x18]
        movzx  ecx, byte ptr [edi + 0xb6]
        add    ebx, ecx
        call   sub_1_50E9
        jae    loc_1_51F0
loc_1_51DD:
        mov    ecx, 4
        mov    dx, word ptr [edi + 0x78]
        or     dh, 1
        call   sub_1_4EB4
        jb     loc_1_5207
loc_1_51F0:
        cli    
        and    dword ptr [edi + 0x3c], 0xffffff7f
        mov    al, byte ptr [edi + 0x45]
        mov    byte ptr [edi + 0x47], al
        sti    
        mov    dword ptr [edi + 0x60], 0xffffffff
        clc    
loc_1_5207:
        ret    
loc_1_5208:
        stc    
        ret    
sub_1_520A:
        test   dword ptr [edi + 0x3c], 1
        je     loc_1_5220
        test   dword ptr [edi + 0x34], 8
        je     loc_1_5222
        and    dword ptr [edi + 0x3c], 0xfffffffe
loc_1_5220:
        stc    
loc_1_5221:
        ret    
loc_1_5222:
        mov    al, 5
        mov    ah, byte ptr [edi + 0x45]
        add    ah, ah
        or     ah, 1
        test   dword ptr [edi + 0x34], 1
        jne    loc_1_5237
        mov    al, 1
loc_1_5237:
        mov    ecx, 4
        mov    dx, word ptr [edi + 0x78]
        call   sub_1_4EB4
        jb     loc_1_5221
        cli    
        and    dword ptr [edi + 0x3c], 0xffffff7e
        or     dword ptr [edi + 0x34], 8
        mov    al, byte ptr [edi + 0x45]
        mov    byte ptr [edi + 0x47], al
        mov    dword ptr [edi + 0x60], 0xffffffff
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     loc_1_526E
loc_1_526E:
        push   eax
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], eax
        pop    eax
        test   dword ptr [edi + 0x34], 0x20
        sti    
        jne    loc_1_5281
        ret    
loc_1_5281:
        mov    ebx, edi
        and    dword ptr [edi + 0x34], 0xffffffdf
        mov    byte ptr [edi + 0x38], 4
        call   sub_1_6104
        clc    
        ret    
sub_1_5292:
        test   dword ptr [edi + 0x3c], 2
        je     loc_1_52D8
        mov    byte ptr [edi + 0x38], 2
        or     dword ptr [edi + 0x34], 0x10
        mov    ebx, edi
        call   sub_1_6255
        mov    al, 0x53
        mov    ecx, 3
        mov    dx, word ptr [edi + 0x78]
        call   sub_1_4EB4
        jb     loc_1_52D7
        and    dword ptr [edi + 0x3c], 0xfffffffd
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     loc_1_52CE
loc_1_52CE:
        push   eax
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], eax
        pop    eax
        clc    
loc_1_52D7:
        ret    
loc_1_52D8:
        stc    
        ret    
sub_1_52DA:
        test   dword ptr [edi + 0x3c], 4
        je     loc_1_5319
        mov    al, 0x73
        mov    ecx, 3
        mov    dx, word ptr [edi + 0x78]
        or     dh, 1
        call   sub_1_4EB4
        jb     loc_1_5303
        and    dword ptr [edi + 0x3c], 0xfffffffb
        cmp    byte ptr [edi + 0x38], 2
        je     loc_1_5304
        clc    
loc_1_5303:
        ret    
loc_1_5304:
        test   dword ptr [edi + 0x34], 0x10
        jne    loc_1_5303
        mov    byte ptr [edi + 0x38], 0
        push   edi
        call   sub_1_6435
        clc    
        ret    
loc_1_5319:
        stc    
        ret    
sub_1_531B:
        test   dword ptr [edi + 0x3c], 8
        je     loc_1_535C
        cmp    byte ptr [edi + 0x38], 4
        je     loc_1_5356
        mov    al, 0x7f
        mov    ecx, 3
        mov    dx, word ptr [edi + 0x78]
        call   sub_1_4EB4
        jb     loc_1_535B
        mov    byte ptr [edi + 0x38], 1
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     loc_1_534E
loc_1_534E:
        push   eax
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], eax
        pop    eax
loc_1_5356:
        and    dword ptr [edi + 0x3c], 0xfffffff7
        clc    
loc_1_535B:
        ret    
loc_1_535C:
        stc    
        ret    
sub_1_535E:
        test   dword ptr [edi + 0x3c], 0x20
        je     loc_1_53AC
        test   dword ptr [edi + 0x34], 4
        je     loc_1_53A8
        mov    al, 9
        mov    ah, byte ptr [edi + 0x45]
        add    ah, ah
        test   dword ptr [edi + 0x3c], 0x10
        je     loc_1_5383
        or     ah, 1
loc_1_5383:
        mov    ecx, 4
        mov    dx, word ptr [edi + 0x78]
        or     dh, 1
        call   sub_1_4EB4
        jb     loc_1_53A7
        and    dword ptr [edi + 0x3c], 0xffffff4f
        mov    ebx, dword ptr [edi + 0x40]
        inc    dword ptr [ebx + 0xe0]
        clc    
loc_1_53A7:
        ret    
loc_1_53A8:
        and    dword ptr [edi + 0x3c], 0xffffffdf
loc_1_53AC:
        stc    
        ret    
sub_1_53AE:
        test   dword ptr [edi + 0x3c], 0x40
        jne    loc_1_53B9
        stc    
        ret    
loc_1_53B9:
        push   esi
        lea    esi, [edi + 0x8c]
        movzx  eax, byte ptr [edi + 0xb6]
        add    esi, eax
        mov    al, 0x87
        test   byte ptr [edi + 0x80], 0x80
        je     loc_1_53D6
        or     al, 0x10
loc_1_53D6:
        mov    bx, word ptr [edi + 0x7c]
        test   byte ptr [edi + 0x80], 0x40
        je     loc_1_53E5
        xor    bh, bh
loc_1_53E5:
        mov    word ptr [esi + 3], bx
        mov    ah, bl
        mov    dx, word ptr [edi + 0x78]
        or     dh, 1
        mov    bx, word ptr [edi + 0x7e]
        mov    word ptr [esi + 5], bx
        mov    bl, byte ptr [edi + 0x80]
        and    bl, 0x1f
        mov    byte ptr [esi + 7], bl
        mov    ecx, 8
        call   sub_1_4EB4
        jb     loc_1_5426
        mov    byte ptr [edi + 0x38], 3
        and    dword ptr [edi + 0x3c], 0xffffffbf
        mov    ebx, dword ptr [edi + 0x40]
        inc    dword ptr [ebx + 0xd8]
        pop    esi
        clc    
        ret    
loc_1_5426:
        pop    esi
        stc    
        ret    
sub_1_5429:
        push   esi
        push   edi
        mov    esi, dword ptr [edi + 0x40]
loc_1_542E:
        test   dword ptr [esi], 0x90
        jne    loc_1_5478
        call   sub_1_535E
        jae    loc_1_546C
        call   sub_1_5121
        jae    loc_1_546C
        call   sub_1_520A
        jae    loc_1_546C
        call   sub_1_51A4
        jae    loc_1_546C
        call   sub_1_5292
        jae    loc_1_546C
        call   sub_1_52DA
        jae    loc_1_546C
        call   sub_1_531B
        jae    loc_1_546C
        call   sub_1_53AE
loc_1_546C:
        mov    esi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 0x3c], 0x7fff
        jne    loc_1_542E
loc_1_5478:
        pop    edi
        pop    esi
        ret    
loc_1_547B:
        and    dword ptr [edi], 0xfffffeff
        pop    esi
        clc    
        ret    
sub_1_5484:
        mov    edi, esi
        push   esi
loc_1_5487:
        cli    
        lea    esi, [edi + 0xbc]
        call   sub_1_28E3
        sti    
        je     loc_1_547B
        cli    
        dec    byte ptr [edi + 0x30]
        js     loc_1_54F6
        sti    
        call   sub_1_0517
        cmp    eax, 2
        ja     loc_1_54FD
        je     loc_1_5487
        cli    
        or     word ptr [esi + 0x10], 0x40
        test   word ptr [esi + 0x10], 0x400
        sti    
        jne    loc_1_5487
        test   word ptr [esi + 0x10], 0x1000
        je     loc_1_54CA
        and    dword ptr [0x6eec], 0xffffefff ; -> obj1:0x6EEC
loc_1_54CA:
        test   word ptr [esi + 0x10], 8
        je     loc_1_54E5
        test   eax, eax
        je     loc_1_54D8
        mov    al, 0x21
loc_1_54D8:
        push   esi
        mov    esi, dword ptr [esi + 0x30]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        pop    esi
loc_1_54E5:
        cli    
        push   edi
        lea    edi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_2868
        pop    edi
        sti    
        jmp    loc_1_5487
loc_1_54F6:
        inc    byte ptr [edi + 0x30]
        or     dword ptr [edi], 0x10
        sti    
loc_1_54FD:
        cli    
        push   edi
        lea    edi, [edi + 0xbc]
        call   sub_1_28A7
        pop    edi
        sti    
        pop    esi
        stc    
        ret    
sub_1_550F:
        test   dword ptr [esi], 0x100
        je     loc_1_551C
        call   sub_1_5484
loc_1_551C:
        push   esi
loc_1_551D:
        pop    esi
        test   dword ptr [esi], 0x90
        jne    loc_1_5600
loc_1_552A:
        cli    
        push   eax
        mov    edi, dword ptr [esi + 0xc4]
        mov    edi, dword ptr [edi]
        lea    eax, [esi + 0xc4]
        mov    dword ptr [edi + 4], eax
        xchg   dword ptr [esi + 0xc4], edi
        cmp    edi, eax
        lea    edi, [edi - 0x24]
        pop    eax
        je     loc_1_5600
        and    byte ptr [edi + 0xb8], 0xfe
        push   esi
        sti    
        test   dword ptr [edi + 0x3c], 0x7fff
        je     loc_1_5566
        call   sub_1_5429
loc_1_5566:
        call   sub_1_4F81
        jb     loc_1_551D
        mov    dword ptr [esi + 0xc], edi
        sti    
        call   sub_1_0517
        pop    esi
        cli    
        test   byte ptr [edi + 0xb8], 1
        jne    loc_1_55A9
        or     byte ptr [edi + 0xb8], 1
        push   edi
        push   eax
        lea    eax, [esi + 0xc4]
        lea    edi, [edi + 0x24]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0xc8]
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 0xc8], eax
        pop    eax
        pop    edi
loc_1_55A9:
        sti    
        mov    cl, byte ptr [0x6f20]         ; -> obj1:0x6F20
        mov    byte ptr [edi + 0xb9], cl
        push   edi
        mov    edi, dword ptr [edi + 0x40]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     loc_1_55C4
loc_1_55C4:
        push   eax
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x5c], eax
        pop    eax
        cmp    eax, 2
        jbe    loc_1_552A
        cli    
        cmp    dword ptr [edi + 0x14], edx
        jne    loc_1_55FC
        mov    eax, dword ptr [edx + 4]
        lea    ecx, [edi + 0x10]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    ecx, [edi + 8]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        dec    byte ptr [edi + 0x44]
        and    byte ptr [edi + 0x44], 0x7f
loc_1_55FC:
        and    dword ptr [edi + 0x34], 0xfffffff7
loc_1_5600:
        sti    
        ret    
sub_1_5602:
        push   0
loc_1_5604:
        mov    eax, dword ptr [esp]
        cmp    al, 8
        jae    loc_1_5620
        mov    esi, dword ptr [eax*4 + 0x6f40] ; -> obj1:0x6F40
        test   esi, esi
        je     loc_1_561B
        call   sub_1_550F
loc_1_561B:
        inc    byte ptr [esp]
        jmp    loc_1_5604
loc_1_5620:
        pop    eax
        ret    
        db  000h, 000h
sub_1_5624:
        push   ebx
        mov    ebx, edi
        cmp    byte ptr [ebx + 0x25], 0
        je     loc_1_5636
        mov    byte ptr [ebx + 0x25], 4
        call   sub_1_3BB8
loc_1_5636:
        pop    ebx
        ret    
sub_1_5638:
        cmp    dword ptr [0x6eff], 0         ; -> loc_1_6EFF
        jne    loc_1_5642
        ret    
loc_1_5642:
        mov    ecx, 0xffffffff
        cli    
loc_1_5648:
        inc    ecx
        cmp    ecx, 0xfd
        ja     loc_1_568B
        mov    ebx, dword ptr [ecx*4 + 0x77c0] ; -> obj1:0x77C0
        test   ebx, ebx
        je     loc_1_5648
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_5648
        test   word ptr [ebx + 0x6a], 4
        je     loc_1_5648
        bts    word ptr [ebx + 0x6a], 3
        jae    loc_1_5648
        and    word ptr [ebx + 0x6a], 0xfff3
        mov    ax, word ptr [ebx + 0x64]
        call   sub_1_3E4E
        cli    
        jb     loc_1_568D
        dec    dword ptr [0x6eff]            ; -> loc_1_6EFF
        jne    loc_1_5648
loc_1_568B:
        sti    
        ret    
loc_1_568D:
        or     word ptr [ebx + 0x6a], 0xc
        jmp    loc_1_568B
loc_1_5694:
        sti    
        jmp    loc_1_572F
sub_1_569A:
        push   esi
        push   edi
        cli    
        push   eax
        mov    edi, dword ptr [ebx + 0x30]
        mov    edi, dword ptr [edi]
        lea    eax, [ebx + 0x30]
        mov    dword ptr [edi + 4], eax
        xchg   dword ptr [ebx + 0x30], edi
        cmp    edi, eax
        lea    edi, [edi - 0x32]
        pop    eax
        je     loc_1_5694
        dec    byte ptr [ebx + 0x8d]
        btr    word ptr [edi + 0x3f], 3
        jae    loc_1_5704
        mov    dword ptr [edi + 0x3a], 0x1c78 ; -> loc_1_1C78
        push   edi
        push   eax
        lea    eax, [0x6f80]                 ; -> obj1:0x6F80
        lea    edi, [edi + 0x32]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [0x6f84]       ; -> obj1:0x6F84
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [0x6f84], eax       ; -> obj1:0x6F84
        pop    eax
        pop    edi
        or     dword ptr [0x6eec], 0x400     ; -> obj1:0x6EEC
        mov    ax, word ptr [edi + 0xa]
        mov    word ptr [edi + 8], ax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [edi + 4], eax
        jmp    loc_1_570C
        db  08Bh, 0C0h
loc_1_5704:
        xor    eax, eax
        call   sub_1_2913
        nop    
loc_1_570C:
        and    word ptr [ebx + 0x40], 0xf1ff
        lea    esi, [ebx + 0x30]
        cmp    esi, dword ptr [esi]
        jne    loc_1_5732
        test   word ptr [ebx + 0x42], 0x4000
        sti    
        je     loc_1_572F
        test   word ptr [ebx + 0x40], 0x40
        jne    loc_1_572F
        call   sub_1_3CD2
loc_1_572F:
        pop    edi
        pop    esi
        ret    
loc_1_5732:
        mov    edi, dword ptr [esi]
        sub    edi, 0x32
        or     word ptr [ebx + 0x40], 0x100
        call   sub_1_5E2C
        jmp    loc_1_572F
sub_1_5744:
        push   esi
        push   edi
        cli    
        and    word ptr [ebx + 0x40], 0xefff
        push   eax
        mov    esi, dword ptr [ebx + 0x28]
        mov    esi, dword ptr [esi]
        lea    eax, [ebx + 0x28]
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [ebx + 0x28], esi
        cmp    esi, eax
        lea    esi, [esi - 0x32]
        pop    eax
        je     loc_1_57B7
        dec    byte ptr [ebx + 0x8c]
        and    word ptr [ebx + 0x6a], 0xff9f
        mov    byte ptr [esi + 1], al
        mov    ax, word ptr [ebx + 0x84]
        mov    word ptr [esi + 8], ax
        and    byte ptr [esi + 0x3f], 0xfb
        push   esi
        push   eax
        lea    eax, [0x6fa8]                 ; -> obj1:0x6FA8
        lea    esi, [esi + 0x32]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [0x6fac]       ; -> obj1:0x6FAC
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [0x6fac], eax       ; -> obj1:0x6FAC
        pop    eax
        pop    esi
        mov    edi, dword ptr [ebx + 0x94]
        mov    al, byte ptr [ebx + 0x68]
        mov    ah, byte ptr [edi + 2]
        mov    word ptr [esi + 2], ax
        or     dword ptr [0x6eec], 0x40      ; -> obj1:0x6EEC
loc_1_57B7:
        sti    
        pop    edi
        pop    esi
        ret    
sub_1_57BB:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        cli    
        test   byte ptr [ebx + 0x24], 8
        jne    loc_1_57DD
        push   esi
        lea    esi, [0x6fa0]                 ; -> obj1:0x6FA0
        push   ebx
        lea    ebx, [ebx + 0x10]
        call   sub_1_2892
        pop    ebx
        pop    esi
        or     byte ptr [ebx + 0x24], 8
loc_1_57DD:
        or     dword ptr [0x6eec], 0x20      ; -> obj1:0x6EEC
        sti    
        pop    ebx
        ret    4
sub_1_57E9:
        sub    ecx, ecx
        mov    dword ptr [esi + 0x30], ebx
        or     word ptr [esi + 0x10], 4
        mov    byte ptr [esi + 0x34], 0x15
        mov    word ptr [esi + 0x1e], 1
        mov    ax, word ptr [ebx + 0x70]
        cmp    ax, word ptr [ebx + 0x6e]
        jb     loc_1_580B
        mov    ax, word ptr [ebx + 0x6e]
loc_1_580B:
        sub    word ptr [ebx + 0x70], ax
        mov    edx, dword ptr [ebx + 0x74]
        mov    dword ptr [esi + 0x24], edx
        mov    byte ptr [esi + 0x20], 0
        movzx  eax, ax
        add    word ptr [esi + 0x3c], ax
        mov    word ptr [esi + 0x22], ax
        add    dword ptr [ebx + 0x74], eax
        cmp    word ptr [ebx + 0x70], 0
        je     loc_1_584B
loc_1_582E:
        cmp    word ptr [ebx + 0x70], 0
        je     loc_1_5836
        ret    
loc_1_5836:
        mov    byte ptr [esi + 0x34], 0x16
        inc    ecx
        test   word ptr [ebx + 0x42], 0x200
        jne    loc_1_5844
        ret    
loc_1_5844:
        or     word ptr [esi + 0x10], 0x200
        ret    
loc_1_584B:
        test   word ptr [ebx + 0x40], 0x800
        je     loc_1_582E
        push   esi
        mov    esi, dword ptr [ebx + 0x30]
        sub    esi, 0x32
        mov    dx, word ptr [esi + 0xa]
        mov    word ptr [ebx + 0x70], dx
        mov    edx, dword ptr [esi + 0xc]
        mov    dword ptr [ebx + 0x74], edx
        pop    esi
        and    word ptr [ebx + 0x40], 0xf7ff
        mov    dx, word ptr [ebx + 0x6e]
        cmp    word ptr [esi + 0x22], dx
        je     loc_1_582E
        sub    dx, word ptr [esi + 0x22]
        movzx  edx, dx
        movzx  eax, word ptr [ebx + 0x70]
        cmp    eax, edx
        jb     loc_1_588A
        mov    eax, edx
loc_1_588A:
        sub    word ptr [ebx + 0x70], ax
        mov    word ptr [esi + 0x1e], 2
        mov    edx, dword ptr [ebx + 0x74]
        mov    dword ptr [esi + 0x2c], edx
        mov    byte ptr [esi + 0x28], 0
        add    word ptr [esi + 0x3c], ax
        mov    word ptr [esi + 0x2a], ax
        add    dword ptr [ebx + 0x74], eax
        jmp    loc_1_582E
loc_1_58AB:
        cli    
        test   byte ptr [ebx + 0x24], 8
        jne    loc_1_58C8
        push   esi
        lea    esi, [0x6fa0]                 ; -> obj1:0x6FA0
        push   ebx
        lea    ebx, [ebx + 0x10]
        call   sub_1_28CF
        pop    ebx
        pop    esi
        or     byte ptr [ebx + 0x24], 8
loc_1_58C8:
        sti    
loc_1_58C9:
        inc    byte ptr [ebx + 0x6c]
        push   dword ptr [ebx + 0x38]
        call   sub_1_4E6C
loc_1_58D4:
        ret    
sub_1_58D5:
        test   word ptr [ebx + 0x40], 0x100
        je     loc_1_58D4
        test   word ptr [ebx + 0x40], 0x200
        jne    loc_1_58D4
        test   word ptr [ebx + 0x40], 2
        jne    loc_1_58D4
        mov    edi, dword ptr [ebx + 0x38]
loc_1_58F0:
        dec    byte ptr [ebx + 0x6c]
        js     loc_1_58C9
        call   sub_1_3821
        jae    loc_1_58AB
        sub    eax, eax
        mov    word ptr [esi + 0x36], ax
        mov    word ptr [esi + 0x38], ax
        mov    word ptr [esi + 0x3a], cs
        cmp    byte ptr [0x6f32], 0          ; -> obj1:0x6F32
        je     loc_1_591D
        test   word ptr [ebx + 0x6a], 0x40
        jne    loc_1_591D
        or     al, 4
loc_1_591D:
        test   word ptr [ebx + 0x42], 0x200
        je     loc_1_5929
        or     al, 2
        and    al, 0xfb
loc_1_5929:
        cli    
        test   word ptr [ebx + 0x6a], 4
        je     loc_1_594D
        or     al, 8
        dec    dword ptr [0x6eff]            ; -> loc_1_6EFF
        and    word ptr [ebx + 0x6a], 0xfff3
        shl    eax, 0x10
        mov    ax, word ptr [ebx + 0x64]
        mov    word ptr [esi + 0x38], ax
        shr    eax, 0x10
loc_1_594D:
        sti    
        mov    byte ptr [esi + 0x35], al
        test   word ptr [ebx + 0x42], 1
        je     loc_1_5964
        and    word ptr [ebx + 0x42], 0xfffe
        mov    word ptr [esi + 0x36], 1
loc_1_5964:
        call   sub_1_57E9
        cli    
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_2868
        pop    edi
        sti    
        or     ecx, ecx
        je     loc_1_58F0
        cli    
        or     word ptr [ebx + 0x40], 0x200
        and    word ptr [ebx + 0x40], 0xfeff
        or     word ptr [ebx + 0x6a], 0x40
        sti    
        push   dword ptr [ebx + 0x38]
        call   sub_1_4E6C
        ret    
sub_1_5999:
        push   ecx
        push   eax
        push   ebx
        push   esi
        cli    
        mov    byte ptr [edi + 0x25], 0
        test   byte ptr [edi + 0x24], 0x84
        je     loc_1_59BD
        push   esi
        push   edi
        lea    esi, [edi + 8]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [edi + 0x24], 0x7b
loc_1_59BD:
        test   word ptr [edi + 0x40], 0x2000
        jne    loc_1_5B81
        xor    ecx, ecx
        test   word ptr [edi + 0x6a], 4
        je     loc_1_59DE
        and    word ptr [edi + 0x6a], 0xfff3
        dec    dword ptr [0x6eff]            ; -> loc_1_6EFF
loc_1_59DE:
        test   byte ptr [edi + 0x24], 2
        je     loc_1_5A1F
        and    byte ptr [edi + 0x24], 0xfd
        push   esi
        push   edi
        lea    esi, [edi]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        test   word ptr [edi + 0x42], 0x4002
        je     loc_1_5A1F
        test   word ptr [edi + 0x42], 0x4000
        je     loc_1_5A09
        inc    ecx
loc_1_5A09:
        mov    ebx, dword ptr [edi + 0x38]
        lea    edx, [ebx]
        cmp    edx, dword ptr [ebx]
        jne    loc_1_5A1F
        mov    dword ptr [ebx + 0x3c], 2
        push   ebx
        call   sub_1_4E6C
loc_1_5A1F:
        and    word ptr [edi + 0x42], 0xfffd
        sti    
        mov    dl, 0x34
        test   dword ptr [0x6eec], 1         ; -> obj1:0x6EEC
        je     loc_1_5A3F
        mov    dl, 0xa
        test   word ptr [edi + 0x40], 0x40
        je     loc_1_5A3F
        mov    dl, 0x18
loc_1_5A3F:
        cli    
        test   byte ptr [edi + 0x24], 8
        je     loc_1_5A5B
        and    byte ptr [edi + 0x24], 0xf7
        push   esi
        push   edi
        lea    esi, [edi + 0x10]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_5A5B:
        test   byte ptr [edi + 0x24], 0x20
        je     loc_1_5A77
        and    byte ptr [edi + 0x24], 0xdf
        push   esi
        push   edi
        lea    esi, [edi + 0x18]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        sti    
loc_1_5A77:
        mov    esi, dword ptr [edi + 0x94]
        mov    ah, byte ptr [esi + 2]
        mov    al, byte ptr [edi + 0x68]
loc_1_5A83:
        cli    
        lea    esi, [edi + 0x28]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_5AAF
        inc    ecx
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    word ptr [esi + 2], ax
        test   word ptr [edi + 0x6a], 2
        jne    loc_1_5A83
        mov    byte ptr [esi + 1], dl
        call   sub_1_6BE9
        jmp    loc_1_5A83
loc_1_5AAF:
        test   word ptr [edi + 0x6a], 2
        jne    loc_1_5B02
        mov    esi, dword ptr [edi + 0x94]
        cli    
        lea    ebx, [esi + 0x34]
        call   sub_1_28F3
        lea    ebx, [ebx - 0x32]
        je     loc_1_5AE8
        dec    byte ptr [esi + 0x19]
loc_1_5ACE:
        sti    
        mov    al, byte ptr [edi + 0x68]
        mov    esi, ebx
        mov    byte ptr [esi + 2], al
        mov    byte ptr [esi + 1], dl
        inc    ecx
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        call   sub_1_6BE9
        jmp    loc_1_5B02
loc_1_5AE8:
        mov    esi, dword ptr [edi + 0x94]
        mov    esi, dword ptr [esi + 4]
        lea    ebx, [esi + 0x9c]
        call   sub_1_28F3
        lea    ebx, [ebx - 0x32]
        jne    loc_1_5ACE
        sti    
loc_1_5B02:
        cli    
        lea    esi, [edi + 0x30]
        call   sub_1_28E3
        lea    esi, [esi - 0x32]
        sti    
        je     loc_1_5B2A
        inc    ecx
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        test   word ptr [edi + 0x6a], 2
        jne    loc_1_5B02
        mov    byte ptr [esi + 1], dl
        call   sub_1_6BE9
        jmp    loc_1_5B02
loc_1_5B2A:
        cli    
        test   word ptr [edi + 0x40], 0x80
        je     loc_1_5B5F
        and    word ptr [edi + 0x40], 0xff7f
        test   word ptr [edi + 0x6a], 2
        jne    loc_1_5B5F
        mov    esi, dword ptr [edi + 0x88]
        cmp    dl, 0xa
        jne    loc_1_5B56
        mov    al, byte ptr [esi]
        and    al, 0x7f
        cmp    al, 0x12
        jne    loc_1_5B56
        sub    dl, dl
loc_1_5B56:
        mov    byte ptr [esi + 1], dl
        inc    ecx
        call   sub_1_6BE9
loc_1_5B5F:
        mov    byte ptr [edi + 0x24], 0
        test   word ptr [edi + 0x6a], 2
        jne    loc_1_5B81
        mov    esi, dword ptr [edi + 0x94]
        dec    byte ptr [esi + 3]
        cmp    byte ptr [esi + 1], 3
        jne    loc_1_5B7F
        call   sub_1_250A
loc_1_5B7F:
        jecxz  loc_1_5BD0
loc_1_5B81:
        cli    
        xor    eax, eax
        mov    word ptr [edi + 0x40], ax
        mov    word ptr [edi + 0x42], ax
        mov    word ptr [edi + 0x6a], ax
        movzx  ebx, byte ptr [edi + 0x68]
        dec    ebx
        mov    dword ptr [ebx*4 + 0x77c0], 0 ; -> obj1:0x77C0
        mov    eax, 0x6fbc                   ; -> obj1:0x6FBC
        sub    eax, 0x20
loc_1_5BA8:
        cmp    dword ptr [eax + 0x20], 0
        je     loc_1_5BC0
        cmp    dword ptr [eax + 0x20], edi
        jne    loc_1_5BBB
        mov    ecx, dword ptr [edi + 0x20]
        mov    dword ptr [eax + 0x20], ecx
        jmp    loc_1_5BC0
loc_1_5BBB:
        mov    eax, dword ptr [eax + 0x20]
        jmp    loc_1_5BA8
loc_1_5BC0:
        push   eax
        mov    eax, edi
        call   sub_1_227A
        pop    eax
        sti    
        clc    
loc_1_5BCB:
        pop    esi
        pop    ebx
        pop    eax
        pop    ecx
        ret    
loc_1_5BD0:
        test   word ptr [edi + 0x42], 0x4000
        jne    loc_1_5B81
        mov    byte ptr [edi + 0x25], 0xa
        mov    ebx, dword ptr [esi + 4]
        mov    dword ptr [edi + 0x3c], ebx
        cmp    byte ptr [esi + 1], 3
        stc    
        je     loc_1_5BCB
        mov    ebx, edi
        call   sub_1_5C40
        stc    
        jmp    loc_1_5BCB
sub_1_5BF3:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cli    
        test   byte ptr [edi + 0x24], 0x84
        jne    loc_1_5C1C
        call   sub_1_220C
        jb     loc_1_5C28
        or     byte ptr [edi + 0x24], 4
        push   esi
        lea    esi, [0x6f90]                 ; -> obj1:0x6F90
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_287D
        pop    edi
        pop    esi
loc_1_5C1C:
        or     dword ptr [0x6eec], 0x10      ; -> obj1:0x6EEC
        sti    
        pop    edi
        ret    4
loc_1_5C28:
        or     byte ptr [edi + 0x24], 0x80
        push   esi
        lea    esi, [0x6f98]                 ; -> obj1:0x6F98
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_287D
        pop    edi
        pop    esi
        jmp    loc_1_5C1C
sub_1_5C40:
        cli    
        test   byte ptr [ebx + 0x24], 0x20
        jne    loc_1_5C6B
        push   edi
        mov    edi, dword ptr [ebx + 0x94]
        or     byte ptr [ebx + 0x24], 0x20
        push   esi
        lea    esi, [edi + 0x3c]
        push   ebx
        lea    ebx, [ebx + 0x18]
        call   sub_1_2892
        pop    ebx
        pop    esi
        mov    edi, dword ptr [edi + 4]
        or     dword ptr [edi], 0x400
        pop    edi
loc_1_5C6B:
        sti    
        ret    
loc_1_5C6D:
        mov    esi, dword ptr [ebx + 0x94]
        mov    al, byte ptr [ebx + 0x68]
        mov    ah, byte ptr [esi + 2]
        mov    esi, edi
        mov    word ptr [esi + 2], ax
        and    word ptr [ebx + 0x42], 0xfffb
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    byte ptr [esi + 1], 7
        call   sub_1_6BE9
        test   word ptr [ebx + 0x40], 0x8000
        je     loc_1_5CDB
        jmp    loc_1_5CF6
loc_1_5C9D:
        clc    
        ret    
loc_1_5C9F:
        cli    
        lea    edi, [ebx + 0x28]
        call   sub_1_2903
        lea    edi, [edi - 0x32]
        jne    loc_1_5CB6
        and    word ptr [ebx + 0x42], 0x7fff
        sti    
        jmp    loc_1_5CFE
loc_1_5CB6:
        or     word ptr [ebx + 0x6a], 0x20
        sti    
        dec    byte ptr [ebx + 0x8c]
        jmp    loc_1_5D1C
loc_1_5CC4:
        push   esi
        mov    esi, dword ptr [esi + 4]
        cli    
        lea    edi, [esi + 0x9c]
        call   sub_1_2903
        lea    edi, [edi - 0x32]
        sti    
        pop    esi
        jne    loc_1_5D1C
loc_1_5CDB:
        test   word ptr [ebx + 0x40], 0x8000
        jne    loc_1_5CE8
        call   sub_1_5C40
loc_1_5CE8:
        pop    edi
        pop    esi
        stc    
        ret    
sub_1_5CEC:
        test   word ptr [ebx + 0x40], 0x1000
        jne    loc_1_5C9D
        push   esi
        push   edi
loc_1_5CF6:
        test   word ptr [ebx + 0x42], 0x8000
        jne    loc_1_5C9F
loc_1_5CFE:
        mov    esi, dword ptr [ebx + 0x94]
        cli    
        lea    eax, [esi + 0x34]
        mov    edi, dword ptr [eax]
        cmp    edi, eax
        je     loc_1_5CC4
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        sub    edi, 0x32
        sti    
        dec    byte ptr [esi + 0x19]
loc_1_5D1C:
        test   word ptr [ebx + 0x42], 4
        jne    loc_1_5C6D
        cli    
        lea    eax, [ebx + 0x28]
        lea    ecx, [edi + 0x32]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebx + 0x28], ecx
        mov    dword ptr [eax + 4], ecx
        sti    
        inc    byte ptr [ebx + 0x8c]
        mov    edx, dword ptr [edi + 4]
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebx + 0x86], cx
        mov    word ptr [ebx + 0x84], 0
        mov    dword ptr [ebx + 0x80], edx
        or     byte ptr [edi + 0x3f], 4
        or     word ptr [ebx + 0x40], 0x1000
        cli    
        test   word ptr [ebx + 0x40], 0x8000
        jne    loc_1_5D78
loc_1_5D73:
        sti    
        pop    edi
        pop    esi
        clc    
        ret    
loc_1_5D78:
        and    word ptr [ebx + 0x40], 0x7fff
        sti    
        mov    ax, word ptr [ebx + 0x84]
        call   sub_1_3D51
        jmp    loc_1_5D73
sub_1_5D8D:
        cli    
        mov    edi, ebx
        movzx  ebx, byte ptr [edi + 2]
        dec    ebx
        cmp    ebx, 0xfd
        ja     loc_1_5DB8
        mov    ebx, dword ptr [ebx*4 + 0x77c0] ; -> obj1:0x77C0
        or     ebx, ebx
        je     loc_1_5DB8
        test   word ptr [ebx + 0x40], 0x60
        jne    loc_1_5DC0
        cmp    byte ptr [ebx + 0x25], 5
        jne    loc_1_5DB8
        clc    
        ret    
loc_1_5DB8:
        mov    al, 8
loc_1_5DBA:
        sti    
        mov    byte ptr [edi + 1], al
        stc    
        ret    
loc_1_5DC0:
        mov    al, 0x18
        test   word ptr [ebx + 0x40], 0x40
        jne    loc_1_5DCC
        mov    al, 0xa
loc_1_5DCC:
        test   byte ptr [ebx + 0x24], 0x80
        jne    loc_1_5DBA
        push   eax
        test   byte ptr [ebx + 0x24], 4
        je     loc_1_5DFB
        and    byte ptr [ebx + 0x24], 0xfb
        push   esi
        push   edi
        lea    esi, [ebx + 8]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        push   edi
        mov    edi, ebx
        sti    
        call   sub_1_5999
        cli    
        pop    edi
        jae    loc_1_5E29
loc_1_5DFB:
        test   byte ptr [ebx + 0x24], 0x20
        je     loc_1_5E16
        push   esi
        push   edi
        lea    esi, [ebx + 0x18]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [ebx + 0x24], 0xdf
loc_1_5E16:
        or     word ptr [ebx + 0x42], 0x4000
        test   word ptr [ebx + 0x42], 2
        jne    loc_1_5E29
        call   sub_1_235E
loc_1_5E29:
        pop    eax
        jmp    loc_1_5DBA
sub_1_5E2C:
        cli    
        or     word ptr [ebx + 0x40], 0x100
        movzx  eax, word ptr [ebx + 0x40]
        test   eax, 0x400
        je     loc_1_5E40
        jmp    loc_1_5E80
loc_1_5E40:
        and    eax, 0xfffff5ff
        or     eax, 0x400
        test   byte ptr [edi + 0x3f], 2
        je     loc_1_5E55
        or     eax, 0x800
loc_1_5E55:
        mov    word ptr [ebx + 0x40], ax
        and    word ptr [ebx + 0x42], 0xfdff
        test   byte ptr [edi + 0x3f], 1
        je     loc_1_5E6B
        or     word ptr [ebx + 0x42], 0x200
loc_1_5E6B:
        mov    ax, word ptr [edi + 8]
        mov    word ptr [ebx + 0x70], ax
        mov    word ptr [ebx + 0x72], 0
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 0x74], eax
        sti    
loc_1_5E80:
        push   ebx
        call   sub_1_57BB
        push   dword ptr [ebx + 0x38]
        call   sub_1_4E6C
        ret    
sub_1_5E8F:
        push   esi
        push   edi
        push   ecx
        cli    
        cli    
        lea    edi, [ebx + 0x30]
        cmp    edi, dword ptr [edi]
        sti    
        je     loc_1_5EAD
        mov    edi, dword ptr [edi]
        sub    edi, 0x32
        or     word ptr [ebx + 0x40], 0x100
        test   word ptr [ebx + 0x40], 0x400
loc_1_5EAD:
        je     loc_1_5EED
        and    word ptr [ebx + 0x40], 0xf5ff
        or     word ptr [ebx + 0x40], 0x400
        add    cx, word ptr [ebx + 0x72]
        mov    word ptr [ebx + 0x72], cx
        cmp    cx, word ptr [edi + 8]
        jae    loc_1_5EF2
        mov    edx, dword ptr [edi + 4]
        add    edx, ecx
        mov    dword ptr [ebx + 0x74], edx
        mov    ax, word ptr [edi + 8]
        sub    ax, cx
        mov    word ptr [ebx + 0x70], ax
        test   byte ptr [edi + 0x3f], 2
        je     loc_1_5EE8
        or     word ptr [ebx + 0x40], 0x800
loc_1_5EE8:
        or     word ptr [ebx + 0x42], 1
loc_1_5EED:
        sti    
        pop    ecx
        pop    edi
        pop    esi
        ret    
loc_1_5EF2:
        test   byte ptr [edi + 0x3f], 2
        jne    loc_1_5F08
        sti    
        jecxz  loc_1_5EE8
        or     word ptr [ebx + 0x40], 0x60
        call   sub_1_3CD2
        stc    
        jmp    loc_1_5EED
loc_1_5F08:
        sub    cx, word ptr [edi + 8]
        mov    edx, dword ptr [edi + 0xc]
        add    edx, ecx
        mov    dword ptr [ebx + 0x74], edx
        mov    ax, word ptr [edi + 0xa]
        sub    ax, cx
        mov    word ptr [ebx + 0x70], ax
        and    word ptr [ebx + 0x40], 0xf7ff
        or     word ptr [ebx + 0x42], 1
        sti    
        pop    ecx
        pop    edi
        pop    esi
        ret    
loc_1_5F2F:
        movzx  eax, word ptr [edi + 0x76]
        sub    eax, ebx
        cmp    ecx, eax
        jae    loc_1_5F3B
        mov    eax, ecx
loc_1_5F3B:
        mov    ecx, eax
        pushfd 
        push   ecx
        push   edi
        push   ecx
        mov    edi, dword ptr [edi + 0x68]
        lea    edi, [edi + 2]
        push   edi
        mov    edi, edx
        cld    
loc_1_5F4B:
        pop    esi
        pop    eax
        or     eax, eax
        je     loc_1_5F98
        movzx  ecx, word ptr [esi + 4]
        sub    ecx, ebx
        cmp    ecx, eax
        jb     loc_1_5F5D
        mov    ecx, eax
loc_1_5F5D:
        sub    eax, ecx
        push   eax
        mov    eax, 6
        add    eax, esi
        push   eax
        mov    eax, dword ptr [esp + 8]
        test   dword ptr [eax], 1
        jne    loc_1_5F82
        movzx  eax, word ptr [esi]
        movzx  esi, word ptr [esi + 2]
        shl    esi, 4
        add    esi, eax
        jmp    loc_1_5F84
loc_1_5F82:
        mov    esi, dword ptr [esi]
loc_1_5F84:
        add    esi, ebx
        sub    ebx, ebx
        mov    eax, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_5F4B
loc_1_5F98:
        pop    eax
        pop    ecx
        popfd  
        jmp    loc_1_5FD9
sub_1_5F9D:
        enter  8, 0
        push   edi
        test   dword ptr [edi], 4
        je     loc_1_5F2F
        movzx  eax, word ptr [edi + 0x76]
        or     eax, eax
        je     loc_1_5FDC
        cmp    ax, word ptr [edi + 0x74]
        ja     loc_1_5FE2
loc_1_5FB8:
        sub    eax, ebx
        cmp    ecx, eax
        jb     loc_1_5FC0
        mov    ecx, eax
loc_1_5FC0:
        push   ecx
        mov    esi, dword ptr [edi + 0x70]
        mov    edi, edx
        add    esi, ebx
        cld    
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        cmp    ecx, eax
loc_1_5FD9:
        pop    edi
        leave  
        ret    
loc_1_5FDC:
        movzx  eax, word ptr [edi + 0x74]
        jmp    loc_1_5FB8
loc_1_5FE2:
        sub    eax, ebx
        cmp    ecx, eax
        jb     loc_1_5FEA
        mov    ecx, eax
loc_1_5FEA:
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [edi + 0x78]
        push   ecx
        sub    ebx, dword ptr [0x6e80]       ; -> obj1:0x6E80
        push   ebx
        push   dword ptr [edi + 0x80]
        push   dword ptr [edi + 0x10]
        lea    eax, [ebp - 4]
        push   eax
        sub    esp, 8
        push   ecx
        push   edx
        push   dword ptr [edi + 0x12c]
        mov    eax, esp
        add    eax, 0xc
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6CB2
        pop    eax
        pop    edx
        cmp    edx, 0
        jne    loc_1_6067
        push   eax
        mov    dword ptr [edi + 0x7c], eax
        mov    eax, dword ptr [edi + 0x78]
        mov    dword ptr [eax + 0x3c], 0
        push   eax
        push   eax
        call   sub_1_6C94
        call   sub_1_6CD6
        call   sub_1_6C22
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_606D
loc_1_6050:
        push   dword ptr [edi + 0x7c]
        call   sub_1_6CBE
        pop    eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_6067
        mov    ecx, dword ptr [ebp - 8]
        cmp    ecx, eax
        pop    edi
        leave  
        ret    
loc_1_6067:
        sub    ecx, ecx
        stc    
        pop    edi
        leave  
        ret    
loc_1_606D:
        mov    eax, dword ptr [edi + 0x130]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [edi + 0x2c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi + 0x78]
        mov    eax, dword ptr [eax + 0x3c]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_6050
sub_1_608F:
        push   ecx
        push   esi
        push   edi
        mov    esi, dword ptr [ebx + 0x38]
        movzx  edx, byte ptr [esi + 0xb6]
        add    edx, 0x12
        mov    edi, dword ptr [esi + 0x40]
        mov    eax, dword ptr [ebx + 0x80]
        movzx  ecx, word ptr [ebx + 0x86]
        push   ebx
        mov    ebx, edx
        mov    edx, eax
        call   sub_1_5F9D
        pop    ebx
        pushfd 
        sub    word ptr [ebx + 0x86], cx
        add    word ptr [ebx + 0x84], cx
        add    dword ptr [ebx + 0x80], ecx
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        ret    
sub_1_60D4:
        push   esi
        push   edx
        push   eax
        call   sub_1_5F9D
        pop    eax
        pop    edx
        pop    esi
        jb     loc_1_60E4
        sub    al, al
        ret    
loc_1_60E4:
        mov    al, 6
        ret    
sub_1_60E7:
        push   esi
        push   edi
        push   ecx
        mov    ecx, 4
        lea    edi, [esi + 0xa]
        mov    esi, dword ptr [esp + 0x10]
        lea    esi, [esi + 0x1c]
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        pop    edi
        pop    esi
        ret    4
        db  000h, 000h
sub_1_6104:
        push   ecx
        push   edi
        push   esi
        push   eax
        cli    
        lea    edi, [ebx]
        call   sub_1_2903
        je     loc_1_613D
        mov    ecx, edi
loc_1_6114:
        cli    
        push   esi
        lea    esi, [ebx]
        call   sub_1_287D
        pop    esi
        sti    
        push   ecx
        call   sub_1_5624
        pop    ecx
        cli    
        lea    edi, [ebx]
        call   sub_1_2903
        je     loc_1_613D
        cmp    ecx, edi
        jne    loc_1_6114
        push   esi
        lea    esi, [ebx]
        call   sub_1_28BB
        pop    esi
loc_1_613D:
        sti    
        pop    eax
        pop    esi
        pop    edi
        pop    ecx
        ret    
sub_1_6143:
        push   esi
        movzx  ecx, al
        shr    cl, 1
loc_1_6149:
        lea    esi, [ebx + 8]
        call   sub_1_28E3
        inc    byte ptr [ebx + 0x44]
        and    byte ptr [ebx + 0x44], 0x7f
        or     word ptr [esi + 0x10], 0x8000
        push   edi
        lea    edi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_2868
        pop    edi
        loop   loc_1_6149
        pop    esi
        ret    
loc_1_616F:
        sub    al, ah
        je     loc_1_61E3
        call   sub_1_6143
        jmp    loc_1_61E3
sub_1_617A:
        push   eax
        push   ecx
        push   edx
        push   edi
        push   esi
        cli    
        mov    ah, byte ptr [ebx + 0x46]
        mov    byte ptr [ebx + 0x46], al
        shl    eax, 1
        sub    edx, edx
        lea    ecx, [ebx + 0x10]
        mov    esi, dword ptr [ebx + 0x10]
        cmp    esi, ecx
        je     loc_1_616F
loc_1_6194:
        cmp    ah, al
        je     loc_1_61E3
        add    ah, 2
        push   eax
        mov    esi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [esi]
        lea    eax, [ebx + 0x10]
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [ebx + 0x10], esi
        cmp    esi, eax
        pop    eax
        je     loc_1_616F
        test   word ptr [esi + 0x10], 0x200
        jne    loc_1_620D
loc_1_61B7:
        or     word ptr [esi + 0x10], 0x8000
        test   word ptr [esi + 0x10], 0x100
        jne    loc_1_6224
loc_1_61C5:
        push   esi
        push   eax
        lea    eax, [0x6f68]                 ; -> obj1:0x6F68
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [0x6f6c]       ; -> obj1:0x6F6C
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [0x6f6c], eax       ; -> obj1:0x6F6C
        pop    eax
        pop    esi
        jmp    loc_1_6194
loc_1_61E3:
        sti    
        or     edx, edx
        jne    loc_1_622F
loc_1_61E8:
        test   dword ptr [0x6eec], 0x800     ; -> obj1:0x6EEC
        jne    loc_1_61FA
loc_1_61F4:
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        ret    
loc_1_61FA:
        or     dword ptr [0x6eec], 0x20      ; -> obj1:0x6EEC
        and    dword ptr [0x6eec], 0xfffff7ff ; -> obj1:0x6EEC
        jmp    loc_1_61F4
loc_1_620D:
        mov    edi, dword ptr [esi + 0x30]
        test   byte ptr [edi + 0x24], 0x40
        jne    loc_1_621F
        mov    dword ptr [edi + 0x78], edx
        mov    edx, edi
        or     byte ptr [edi + 0x24], 0x40
loc_1_621F:
        inc    byte ptr [edi + 0x6d]
        jmp    loc_1_61B7
loc_1_6224:
        mov    edi, dword ptr [esi + 0x30]
        push   edi
        call   sub_1_5BF3
        jmp    loc_1_61C5
loc_1_622F:
        or     edx, edx
        je     loc_1_61E8
        push   ebx
        mov    ebx, edx
        and    word ptr [ebx + 0x42], 0xfdff
        movzx  ecx, byte ptr [ebx + 0x6d]
loc_1_6240:
        call   sub_1_569A
        loop   loc_1_6240
        mov    byte ptr [ebx + 0x6d], 0
        and    byte ptr [ebx + 0x24], 0xbf
        mov    edx, dword ptr [ebx + 0x78]
        pop    ebx
        jmp    loc_1_622F
sub_1_6255:
        push   esi
        cli    
loc_1_6257:
        lea    esi, [ebx + 0x10]
        call   sub_1_28E3
        je     loc_1_6276
        or     word ptr [esi + 0x10], 0x8000
        push   edi
        lea    edi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_2868
        pop    edi
        jmp    loc_1_6257
loc_1_6276:
        lea    esi, [ebx + 8]
        call   sub_1_28E3
        je     loc_1_6295
        or     word ptr [esi + 0x10], 0x8000
        push   edi
        lea    edi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_2868
        pop    edi
        jmp    loc_1_6276
loc_1_6295:
        sti    
        pop    esi
        ret    
sub_1_6298:
        push   esi
        push   edi
        push   eax
        push   ecx
        mov    esi, eax
        movzx  ecx, byte ptr [esi + 0xb6]
        mov    byte ptr [ebx + 0x91], cl
        push   esi
        lea    esi, [esi + 0x8c]
        lea    edi, [ebx + 0x98]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        mov    ax, 0xf0f0
        stosw  word ptr es:[edi], ax
        stosw  word ptr es:[edi], ax
        mov    word ptr [edi], 0xe
        mov    word ptr [edi + 2], 0xefff
        sub    eax, eax
        mov    byte ptr [edi + 5], al
        mov    word ptr [edi + 6], ax
        mov    word ptr [edi + 8], ax
        mov    word ptr [edi + 0xa], ax
        mov    ax, word ptr [ebx + 0x68]
        xchg   al, ah
        mov    word ptr [edi + 0xc], ax
        add    byte ptr [ebx + 0x91], 0x12
        pop    ecx
        pop    eax
        pop    edi
        pop    esi
        clc    
        ret    
loc_1_62F7:
        pop    eax
        stc    
        ret    
sub_1_62FA:
        mov    dword ptr [0x6ed8], eax       ; -> obj1:0x6ED8
        push   eax
        push   ebx
        push   edi
        mov    edi, edx
        call   sub_1_236B
        pop    edi
        pop    eax
        jb     loc_1_62F7
        add    esp, 4
        xchg   esi, eax
        cli    
        or     byte ptr [esi + 0x24], 2
        push   edi
        lea    edi, [ebx]
        call   sub_1_2868
        pop    edi
        sti    
        push   eax
        mov    eax, dword ptr [0x6ed8]       ; -> obj1:0x6ED8
        call   sub_1_2959
        pop    eax
        mov    dword ptr [esi + 0x38], ebx
        xchg   esi, eax
        xchg   ebx, eax
        call   sub_1_6298
        clc    
        ret    
sub_1_6338:
        cli    
        mov    dword ptr [edi + 0x3c], 0
        test   byte ptr [edi + 0xb8], 4
        je     loc_1_6361
        push   esi
        push   edi
        lea    esi, [edi + 0x2c]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [edi + 0xb8], 0xfb
loc_1_6361:
        mov    eax, 0x6fb8                   ; -> obj1:0x6FB8
        sub    eax, 0x18
loc_1_6369:
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_6381
        cmp    dword ptr [eax + 0x18], edi
        jne    loc_1_637C
        mov    ecx, dword ptr [edi + 0x18]
        mov    dword ptr [eax + 0x18], ecx
        jmp    loc_1_6381
loc_1_637C:
        mov    eax, dword ptr [eax + 0x18]
        jmp    loc_1_6369
loc_1_6381:
        mov    dword ptr [edi + 0x34], 0
        mov    dword ptr [edi + 0x40], 0
        test   byte ptr [edi + 0xb8], 1
        je     loc_1_63A9
        push   esi
        push   edi
        lea    esi, [edi + 0x24]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_63A9:
        test   byte ptr [edi + 0xb8], 2
        je     loc_1_63C3
        push   esi
        push   edi
        lea    esi, [edi + 0x1c]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_63C3:
        sti    
        cli    
        lea    esi, [edi + 8]
        call   sub_1_28E3
        sti    
        je     loc_1_63E7
        or     word ptr [esi + 0x10], 0x8000
        cli    
        push   edi
        lea    edi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_2868
        pop    edi
        sti    
        jmp    loc_1_63C3
loc_1_63E7:
        cli    
        lea    esi, [edi + 0x10]
        call   sub_1_28E3
        sti    
        je     loc_1_640A
        or     word ptr [esi + 0x10], 0x8000
        cli    
        push   edi
        lea    edi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_2868
        pop    edi
        sti    
        jmp    loc_1_63E7
loc_1_640A:
        lea    ebx, [edi]
        call   sub_1_28F3
        je     loc_1_6424
        and    byte ptr [ebx + 0x24], 0xfd
        or     word ptr [ebx + 0x40], 0x40
        push   ebx
        call   sub_1_5BF3
        jmp    loc_1_640A
loc_1_6424:
        mov    byte ptr [edi + 0xb8], 0
        push   eax
        mov    eax, edi
        call   sub_1_227A
        pop    eax
        ret    
sub_1_6435:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cli    
        test   byte ptr [edi + 0xb8], 4
        jne    loc_1_645D
        or     byte ptr [edi + 0xb8], 4
        push   esi
        lea    esi, [0x6f88]                 ; -> obj1:0x6F88
        push   edi
        lea    edi, [edi + 0x2c]
        call   sub_1_287D
        pop    edi
        pop    esi
loc_1_645D:
        or     dword ptr [0x6eec], 0x10      ; -> obj1:0x6EEC
        sti    
        pop    edi
        ret    4
        db  000h, 000h, 000h
loc_1_646C:
        ret    
sub_1_646D:
        mov    al, 1
        xchg   byte ptr [0x6f06], al         ; -> obj1:0x6F06
        or     al, al
        jne    loc_1_646C
        cli    
        mov    esi, 0x6fa8                   ; -> obj1:0x6FA8
        mov    ebx, dword ptr [esi]
        cmp    esi, ebx
        je     loc_1_64B1
loc_1_6485:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
        mov    dword ptr [eax + 4], esi
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        sti    
        sub    ebx, 0x32
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx + 0x31], al
        test   byte ptr [ebx], 0x80
        je     loc_1_64AA
        mov    ecx, dword ptr [ebx + 0x2c]
        jecxz  loc_1_64AA
        pushal 
        call   ecx
        popal  
loc_1_64AA:
        cli    
        mov    ebx, dword ptr [esi]
        cmp    esi, ebx
        jne    loc_1_6485
loc_1_64B1:
        and    dword ptr [0x6eec], 0xffffffbf ; -> obj1:0x6EEC
        sti    
        mov    byte ptr [0x6f06], 0          ; -> obj1:0x6F06
        ret    
sub_1_64C1:
        cli    
        lea    edi, [0x6f88]                 ; -> obj1:0x6F88
        call   sub_1_2903
        lea    edi, [edi - 0x2c]
        je     loc_1_6513
        and    byte ptr [edi + 0xb8], 0xfb
        test   dword ptr [edi + 0x34], 0x800
        jne    loc_1_6578
        test   byte ptr [edi + 0xb8], 2
        je     loc_1_6507
        push   esi
        push   edi
        lea    esi, [edi + 0x1c]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [edi + 0xb8], 0xfd
loc_1_6507:
        mov    byte ptr [edi + 0x38], 0
        sti    
        call   sub_1_6338
loc_1_6511:
        jmp    sub_1_64C1
loc_1_6513:
        cli    
        lea    edi, [0x6f90]                 ; -> obj1:0x6F90
        call   sub_1_2903
        lea    edi, [edi - 8]
        je     loc_1_6530
        and    byte ptr [edi + 0x24], 0xfb
        sti    
        call   sub_1_5999
        jmp    loc_1_6513
loc_1_6530:
        sub    ebx, ebx
loc_1_6532:
        lea    edi, [0x6f98]                 ; -> obj1:0x6F98
        call   sub_1_2903
        lea    edi, [edi - 8]
        je     loc_1_655B
        call   sub_1_220C
        jb     loc_1_6554
        and    byte ptr [edi + 0x24], 0x7f
        call   sub_1_5999
        jmp    loc_1_6532
loc_1_6554:
        mov    dword ptr [edi + 8], ebx
        mov    ebx, edi
        jmp    loc_1_6532
loc_1_655B:
        or     ebx, ebx
        je     loc_1_659C
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 8]
        push   esi
        lea    esi, [0x6f98]                 ; -> obj1:0x6F98
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_287D
        pop    edi
        pop    esi
        jmp    loc_1_655B
loc_1_6578:
        mov    byte ptr [edi + 0x38], 1
        sti    
        push   esi
        mov    esi, edi
        call   sub_1_26A5
        pop    esi
        or     dword ptr [edi + 0x3c], 8
        and    dword ptr [edi + 0x34], 0xfffff7ff
        push   edi
        call   sub_1_4E6C
        jmp    loc_1_6511
loc_1_659C:
        sti    
        ret    
sub_1_659E:
        test   dword ptr [0x6eec], 0x800     ; -> obj1:0x6EEC
        jne    loc_1_65D0
        cli    
        lea    ebx, [0x6fa0]                 ; -> obj1:0x6FA0
        call   sub_1_28F3
        lea    ebx, [ebx - 0x10]
        je     loc_1_65CF
        and    byte ptr [ebx + 0x24], 0xf7
        sti    
        test   word ptr [ebx + 0x6a], 0x1f80
        jne    loc_1_65D1
loc_1_65C8:
        call   sub_1_58D5
        jmp    sub_1_659E
loc_1_65CF:
        sti    
loc_1_65D0:
        ret    
loc_1_65D1:
        test   word ptr [ebx + 0x6a], 0x400
        je     loc_1_65E6
        call   sub_1_3BB8
        jb     loc_1_65D0
        and    word ptr [ebx + 0x6a], 0xfbff
loc_1_65E6:
        test   word ptr [ebx + 0x6a], 0x800
        je     loc_1_65FB
        call   sub_1_3C42
        jb     loc_1_65D0
        and    word ptr [ebx + 0x6a], 0xf7ff
loc_1_65FB:
        test   word ptr [ebx + 0x6a], 0x80
        je     loc_1_6614
        mov    ax, word ptr [ebx + 0x7c]
        call   sub_1_3DA6
        jb     loc_1_65D0
        and    word ptr [ebx + 0x6a], 0xff7f
loc_1_6614:
        test   word ptr [ebx + 0x6a], 0x1000
        je     loc_1_662E
        stc    
        mov    ax, word ptr [ebx + 0x7c]
        call   sub_1_3DA6
loc_1_6626:
        jb     loc_1_65D0
        and    word ptr [ebx + 0x6a], 0xefff
loc_1_662E:
        test   word ptr [ebx + 0x6a], 0x100
        je     loc_1_6647
        mov    ax, word ptr [ebx + 0x7e]
        call   sub_1_3D51
        jb     loc_1_6626
        and    word ptr [ebx + 0x6a], 0xfeff
loc_1_6647:
        test   word ptr [ebx + 0x6a], 0x200
        je     loc_1_6660
        mov    ax, word ptr [ebx + 0x66]
        call   sub_1_3E03
        jb     loc_1_6626
        and    word ptr [ebx + 0x6a], 0xfdff
loc_1_6660:
        jmp    loc_1_65C8
sub_1_6665:
        cli    
        test   word ptr [edi + 0x10], 2
        jne    loc_1_66BC
        push   esi
        lea    esi, [0x6f78]                 ; -> obj1:0x6F78
        call   sub_1_287D
        pop    esi
        push   esi
        push   edi
        mov    edi, 0x6f40                   ; -> obj1:0x6F40
        mov    ecx, 8
loc_1_6687:
        mov    esi, dword ptr [edi]
        add    edi, 4
        test   esi, esi
        je     loc_1_66A5
        test   dword ptr [esi], 0x80
        je     loc_1_66A5
        and    dword ptr [esi], 0xffffff7f
        or     dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
loc_1_66A5:
        loop   loc_1_6687
        pop    edi
        pop    esi
loc_1_66A9:
        sti    
        test   dword ptr [0x6eec], 0x40      ; -> obj1:0x6EEC
        je     loc_1_66BB
        call   sub_1_646D
loc_1_66BB:
        ret    
loc_1_66BC:
        cli    
        and    word ptr [edi + 0x10], 0xfbff
        test   word ptr [edi + 0x10], 0x40
        sti    
        je     loc_1_66A9
        test   word ptr [edi + 0x10], 0x1000
        je     loc_1_66DE
        and    dword ptr [0x6eec], 0xffffefff ; -> obj1:0x6EEC
loc_1_66DE:
        test   word ptr [edi + 0x10], 8
        je     loc_1_66F3
        sub    eax, eax
        mov    esi, dword ptr [edi + 0x30]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_66F3:
        push   esi
        lea    esi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_287D
        pop    esi
        jmp    loc_1_66A9
loc_1_6702:
        ret    
sub_1_6703:
        cli    
        lea    edi, [0x6fb0]                 ; -> obj1:0x6FB0
        call   sub_1_2903
        sti    
        je     loc_1_6702
        mov    esi, dword ptr [edi + 0x14]
        movzx  ecx, word ptr [edi + 0x1e]
        mov    word ptr [0x7bc0], cx         ; -> obj1:0x7BC0
        mov    dx, word ptr [edi + 0x1c]
        mov    ebx, 0x7bc2                   ; -> obj1:0x7BC2
        or     dx, dx
        je     loc_1_6741
        inc    word ptr [0x7bc0]             ; -> obj1:0x7BC0
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebx], eax
        mov    word ptr [ebx + 4], dx
        add    ebx, 6
loc_1_6741:
        jecxz  loc_1_6767
        push   edi
        lea    esi, [edi + 0x20]
        mov    edi, ebx
loc_1_6749:
        push   ecx
        mov    eax, dword ptr [esi + 4]
        mov    cx, word ptr [esi + 2]
        add    dx, cx
        mov    word ptr [edi + 4], cx
        mov    dword ptr [edi], eax
        add    edi, 6
        add    esi, 8
        pop    ecx
        loop   loc_1_6749
        pop    edi
        mov    esi, dword ptr [edi + 0x14]
loc_1_6767:
        mov    ebx, dword ptr [0x7bc2]       ; -> obj1:0x7BC2
        mov    word ptr [esi + 0x76], dx
        mov    eax, 0x7bc0                   ; -> obj1:0x7BC0
        mov    dword ptr [esi + 0x68], eax
        push   esi
        push   edi
        and    dword ptr [esi], 0xfffffffb
        or     dword ptr [esi], 1
        call   dword ptr [esi + 0x44]
        pop    edi
        pop    esi
        call   sub_1_6665
        jmp    sub_1_6703
sub_1_6790:
        cli    
        lea    ebx, [0x6f80]                 ; -> obj1:0x6F80
        call   sub_1_28F3
        lea    ebx, [ebx - 0x32]
        sti    
        je     loc_1_67DC
        mov    edi, dword ptr [ebx + 0x3a]
        inc    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    al, byte ptr [ebx + 0x30]
        call   sub_1_2550
        jb     loc_1_67C5
        test   dword ptr [esi], 2
        je     loc_1_67D3
loc_1_67BD:
        push   ebx
        call   edi
        pop    ebx
        cmp    al, 0xff
        je     sub_1_6790
loc_1_67C5:
        mov    byte ptr [ebx + 1], al
        xor    ah, ah
        mov    esi, ebx
        call   sub_1_6BE9
        jmp    sub_1_6790
loc_1_67D3:
        cmp    byte ptr [ebx], 0x32
        je     loc_1_67BD
        mov    al, 0x34
        jmp    loc_1_67C5
loc_1_67DC:
        ret    
loc_1_67DD:
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        ret    
sub_1_67F7:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x100000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        pop    ebx
        jne    loc_1_67DD
sub_1_6812:
        cli    
        test   dword ptr [0x6eec], 0x400     ; -> obj1:0x6EEC
        je     loc_1_6830
        and    dword ptr [0x6eec], 0xfffffbff ; -> obj1:0x6EEC
        sti    
        call   sub_1_6790
        cli    
loc_1_6830:
        test   dword ptr [0x6eec], 0x20      ; -> obj1:0x6EEC
        je     loc_1_684A
        and    dword ptr [0x6eec], 0xffffffdf ; -> obj1:0x6EEC
        sti    
        call   sub_1_659E
        cli    
loc_1_684A:
        test   dword ptr [0x6eec], 8         ; -> obj1:0x6EEC
        je     loc_1_6864
        and    dword ptr [0x6eec], 0xfffffff7 ; -> obj1:0x6EEC
        sti    
        call   sub_1_5602
        cli    
loc_1_6864:
        test   dword ptr [0x6eec], 0x200     ; -> obj1:0x6EEC
        je     loc_1_6881
        and    dword ptr [0x6eec], 0xfffffdff ; -> obj1:0x6EEC
        sti    
        call   sub_1_6703
        cli    
loc_1_6881:
        test   dword ptr [0x6eec], 0x10      ; -> obj1:0x6EEC
        je     loc_1_689B
        and    dword ptr [0x6eec], 0xffffffef ; -> obj1:0x6EEC
        sti    
        call   sub_1_64C1
        cli    
loc_1_689B:
        test   dword ptr [0x6eec], 0x6b8     ; -> obj1:0x6EEC
        jne    sub_1_6812
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        sti    
        ret    
        db  000h, 000h
sub_1_68C8:
        aam    0xc
        add    byte ptr [eax], al
        mov    dword ptr [0x5300000d], eax   ; -> loc_1_0E53
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        mov    dword ptr [edx], ebx
        add    byte ptr [eax], al
        add    bl, byte ptr [eax + eax]
        add    byte ptr [eax + 0x1c], bh     ; -> loc_1_1C78
        add    byte ptr [eax], al
        sbb    al, byte ptr gs:[eax]
        add    byte ptr [eax], al            ; -> loc_1_6C00
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        and    byte ptr fs:[eax], al
        add    byte ptr [edx + 0x21], al     ; -> loc_1_2142
        add    byte ptr [eax], al
        pushal 
        and    byte ptr [eax], al
        add    byte ptr [edx + 0x21], dl     ; -> loc_1_2152
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        push   0x10
        add    byte ptr [eax], al
        test   dword ptr [ecx], edx
        add    byte ptr [eax], al
        sahf   
        sbb    byte ptr [eax], al
        add    byte ptr [esi + 0x19], ah     ; -> loc_1_1966
        add    byte ptr [eax], al
        wait   
        push   cs
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        sbb    byte ptr [eax], al
        add    byte ptr [edx + 0x10], ch     ; -> loc_1_106A
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        popal  
        sbb    al, byte ptr [eax]
        add    byte ptr [eax], al            ; -> loc_1_6C00
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        cmp    bl, byte ptr [edx]
        add    byte ptr [eax], al
        inc    edi
        sbb    al, byte ptr [eax]
        add    byte ptr [ebx + 0x1a], cl     ; -> loc_1_1A4B
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        dec    ebx
        push   ss
        add    byte ptr [eax], al
        sub    eax, 0x2e000010               ; -> loc_1_162E
        push   ss
        add    byte ptr [eax], al
loc_1_6A64:
        mov    al, 3
        jmp    loc_1_6BE1
loc_1_6A6B:
        movzx  ecx, byte ptr [ebx]
        push   ecx
        mov    al, byte ptr [ebx + 0x30]
        call   sub_1_2550
        and    cl, 0x7f
        sub    cl, 0x10
        jb     loc_1_6A64
        cmp    cl, 0x66
        ja     loc_1_6A64
        mov    byte ptr [ebx + 1], 0xff
        mov    byte ptr [ebx + 0x31], 0xff
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x100000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        inc    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        pop    ebx
        jne    loc_1_6B13
        test   dword ptr [esi], 2
        je     loc_1_6BB7
loc_1_6AB3:
        push   ebx
        call   sub_1_68C8
        pop    ebx
        sub    ah, ah
        cmp    al, 0xff
        jne    loc_1_6B00
        xor    al, al
loc_1_6AC4:
        test   dword ptr [0x6eec], 0x6b8     ; -> obj1:0x6EEC
        jne    loc_1_6AF5
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   eax
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        pop    eax
loc_1_6AEB:
        test   byte ptr [esp], 0x80
        je     loc_1_6B5E
loc_1_6AF1:
        add    esp, 4
        ret    
loc_1_6AF5:
        push   eax
        push   ebx
        call   sub_1_6812
        pop    ebx
        pop    eax
        jmp    loc_1_6AEB
loc_1_6B00:
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 0x31], al
        test   al, al
        jne    loc_1_6AC4
        mov    esi, ebx
        call   sub_1_6BE9
        jmp    loc_1_6AC4
loc_1_6B13:
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        dec    byte ptr [0x6f05]             ; -> loc_1_6F05
        mov    eax, dword ptr [ecx*4 + 0x68c8] ; -> sub_1_68C8
        mov    dword ptr [ebx + 0x3a], eax
        cli    
        push   esi
        lea    esi, [0x6f80]                 ; -> obj1:0x6F80
        push   ebx
        lea    ebx, [ebx + 0x32]
        call   sub_1_2892
        pop    ebx
        pop    esi
        sti    
        or     dword ptr [0x6eec], 0x400     ; -> obj1:0x6EEC
        sub    eax, eax
        jmp    loc_1_6AEB
loc_1_6B5E:
        sub    edx, edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_6B7B:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    byte ptr [ebx + 0x31], 0xff
        jne    loc_1_6B9A
        push   eax
        sub    eax, eax
        cmp    edx, 0x40
        jbe    loc_1_6B90
        inc    eax
loc_1_6B90:
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        pop    eax
        inc    edx
        jmp    loc_1_6B7B
loc_1_6B9A:
        mov    al, byte ptr [ebx + 0x31]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_6BB2:
        jmp    loc_1_6AF1
loc_1_6BB7:
        cmp    byte ptr [ebx], 0x32
        je     loc_1_6AB3
        mov    al, 0x34
        dec    byte ptr [0x6ef6]             ; -> obj1:0x6EF6
        push   eax
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        pop    ebx
        pop    eax
        jmp    loc_1_6BE1
        db  0B0h, 022h
loc_1_6BE1:
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 0x31], al
        jmp    loc_1_6BB2
sub_1_6BE9:
        pushal 
        mov    ebx, esi
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx + 0x31], al
        test   byte ptr [ebx], 0x80
        je     loc_1_6BFE
        mov    ecx, dword ptr [ebx + 0x2c]
        jecxz  loc_1_6BFE
        call   ecx
loc_1_6BFE:
        popal  
        ret    
loc_1_6C00:
        mov    al, 3
        ret    
        db  000h
sub_1_6C04:
        VxDCall  VPOWERD.Service_8029h           ; cd2029802800
sub_1_6C0A:
        VxDCall  VPOWERD.Service_802Ah           ; cd202a802800
sub_1_6C10:
        VxDCall  VPOWERD.Service_802Bh           ; cd202b802800
sub_1_6C16:
        VxDCall  VPOWERD.Service_802Ch           ; cd202c802800
sub_1_6C1C:
        VxDCall  VPOWERD.Service_802Dh           ; cd202d802800
sub_1_6C22:
        VxDCall  VPOWERD.Service_802Eh           ; cd202e802800
        VxDCall  VPOWERD.Service_802Fh           ; cd202f802800
sub_1_6C2E:
        VxDCall  VPOWERD.Service_8030h           ; cd2030802800
        VxDCall  VPOWERD.Service_8050h           ; cd2050802800
sub_1_6C3A:
        VxDCall  VPOWERD.Service_8051h           ; cd2051802800
        VxDCall  VPOWERD.Service_8052h           ; cd2052802800
        VxDCall  VPOWERD.Service_8053h           ; cd2053802800
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_6C76:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_6C7C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
sub_1_6C82:
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
sub_1_6C88:
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
sub_1_6C8E:
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
sub_1_6C94:
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
sub_1_6C9A:
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_6CA6:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_6CAC:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_6CB2:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_6CBE:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_6CC4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
sub_1_6CD6:
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
sub_1_6CDC:
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_6CFA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_6D00:
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
        adc    al, 0
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00031h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NETBEUI '                  ; DDB_Name
        dd  054006000h                  ; DDB_Init_Order
        dd  NETBEUI_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 000h
loc_1_6DE0:
        dec    esi
        inc    ebp
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_6DEC:
        add    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    bh, 7
        add    byte ptr [eax], al
        fld    dword ptr [edi]
        add    byte ptr [eax], al
        out    dx, al
        pop    es
        add    byte ptr [eax], al
        cmpsb  byte ptr [esi], byte ptr es:[edi]
        or     byte ptr [eax], al
        add    bl, ch                        ; -> loc_1_07EB
        pop    es
        add    byte ptr [eax], al
        mov    cl, byte ptr [ecx]
        add    byte ptr [eax], al
        add    dword ptr [ecx], ecx
        add    byte ptr [eax], al
        aam    8
        add    byte ptr [eax], al
        sahf   
        or     dword ptr [eax], eax
        add    dl, al                        ; -> loc_1_0AC2
        or     al, byte ptr [eax]
        add    byte ptr [eax], al            ; -> loc_3_0000
        add    byte ptr [eax], al
        add    byte ptr [ebx], bl            ; -> sub_3_011B
        add    dword ptr [eax], eax
        add    ah, al                        ; -> loc_3_01C4
        add    dword ptr [eax], eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], cl
        add    byte ptr [esi], ch            ; -> loc_1_6E2E
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_6E2E:
        dec    esi
        inc    ebp
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        xor    ebp, dword ptr [esi]
        xor    eax, 0x4944
        add    byte ptr [eax], al
loc_1_6E48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax - 0x80000000], 0xd5
loc_1_6E79:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        scasd  eax, dword ptr es:[edi]
        adc    dword ptr [eax], eax
        add    byte ptr [ecx], dl
        rcl    dword ptr [ecx]
        add    byte ptr [eax], al
        adc    eax, 0x12ed                   ; -> loc_1_12ED
        push   ss
        add    dword ptr [ebx], edx
        add    byte ptr [eax], al
        and    eax, dword ptr [ebx]          ; -> loc_1_1303
        adc    eax, dword ptr [eax]
        add    byte ptr [ecx], ah
        add    eax, 0x33000013
        pop    es
        adc    eax, dword ptr [eax]
        add    byte ptr [edx + esi*2], dl    ; -> loc_1_1372
        adc    eax, dword ptr [eax]
        add    byte ptr [edi], dl
        jb     0x6ec2
        add    byte ptr [eax], al
        dec    eax
        mov    al, byte ptr [0x76000011]
        add    edx, dword ptr [edx]
        add    byte ptr [eax], al
        je     loc_1_1203
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6EF1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6EFF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F03:
        add    byte ptr [eax], al
loc_1_6F05:
        inc    dword ptr [eax]
loc_1_6F07:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F0B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F0F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F13:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F17:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F1B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F1F:
        add    byte ptr [eax], al
loc_1_6F21:
        add    byte ptr [eax], al
loc_1_6F23:
        add    byte ptr [eax], al
loc_1_6F25:
        add    byte ptr [eax], al
loc_1_6F27:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F2B:
        add    byte ptr [eax], al
loc_1_6F2D:
        add    byte ptr [eax], al
loc_1_6F2F:
        add    byte ptr [eax], al
loc_1_6F31:
        add    byte ptr [eax], al
loc_1_6F33:
        add    byte ptr [eax], al
loc_1_6F35:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x6db0], ah
        test   al, al
        je     loc_1_700B
        clc    
        ret    
loc_1_700B:
        inc    byte ptr [0x6db0]
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_7056
        VxDCall  VNETBIOS.VNETBIOS_Get_Version   ; cd2000001400
        jb     loc_1_7056
        cmp    eax, 0x30a
        jb     loc_1_7056
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_1_7037
        mov    byte ptr [0x6ec0], 1
loc_1_7037:
        clc    
        pushf  
        cld    
        push   ecx
        push   edi
        push   eax
        xor    eax, eax
        mov    edi, 0x77c0
        mov    ecx, 0x100
        rep stosd dword ptr es:[edi], eax
        pop    eax
        pop    edi
        pop    ecx
        popf   
        call   sub_1_743B
loc_1_7056:
        ret    
        db  000h
sub_1_7058:
        xor    ecx, ecx
        movzx  eax, byte ptr [0x6f22]
        imul   eax, eax, 0x84
        add    ecx, eax
        movzx  eax, byte ptr [0x6f23]
        imul   eax, eax, 0x3e
        add    ecx, eax
        movzx  eax, byte ptr [0x6f24]
        imul   eax, eax, 0x70
        add    ecx, eax
        add    ecx, 3
        and    cl, 0xfc
        push   1
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_1_709D
        mov    dword ptr [0x6e3a], eax
        ret    
loc_1_709D:
        stc    
        ret    
sub_1_709F:
        mov    dword ptr [0x6eec], 1
        lea    eax, [0x6f68]
        mov    dword ptr [0x6f68], eax
        mov    dword ptr [0x6f6c], eax
        lea    eax, [0x6f70]
        mov    dword ptr [0x6f70], eax
        mov    dword ptr [0x6f74], eax
        lea    eax, [0x6f78]
        mov    dword ptr [0x6f78], eax
        mov    dword ptr [0x6f7c], eax
        lea    eax, [0x6f88]
        mov    dword ptr [0x6f88], eax
        mov    dword ptr [0x6f8c], eax
        lea    eax, [0x6f90]
        mov    dword ptr [0x6f90], eax
        mov    dword ptr [0x6f94], eax
        lea    eax, [0x6f98]
        mov    dword ptr [0x6f98], eax
        mov    dword ptr [0x6f9c], eax
        lea    eax, [0x6fa0]
        mov    dword ptr [0x6fa0], eax
        mov    dword ptr [0x6fa4], eax
        lea    eax, [0x6fa8]
        mov    dword ptr [0x6fa8], eax
        mov    dword ptr [0x6fac], eax
        lea    eax, [0x6fb0]
        mov    dword ptr [0x6fb0], eax
        mov    dword ptr [0x6fb4], eax
        lea    eax, [0x6f80]
        mov    dword ptr [0x6f80], eax
        mov    dword ptr [0x6f84], eax
        mov    edi, 0x6fc0
        mov    ecx, 0x100
loc_1_7153:
        lea    eax, [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        add    edi, 8
        loop   loc_1_7153
        mov    edi, dword ptr [0x6e3a]
        mov    dword ptr [0x6f64], edi
        movzx  ecx, byte ptr [0x6f22]
loc_1_7172:
        lea    eax, [edi + 0x34]
        mov    dword ptr [edi + 0x24], eax
        mov    word ptr [edi + 0x1e], 1
        mov    word ptr [edi + 0x10], 2
        cli    
        push   esi
        lea    esi, [0x6f70]
        call   0xfffe987d
        pop    esi
        sti    
        add    edi, 0x84
        loop   loc_1_7172
        mov    dword ptr [0x6f60], edi
        movzx  ecx, byte ptr [0x6f23]
loc_1_71A8:
        mov    word ptr [edi + 0x10], 0x8001
        cli    
        push   esi
        lea    esi, [0x6f68]
        call   0xfffe987d
        pop    esi
        sti    
        add    edi, 0x3e
        loop   loc_1_71A8
        movzx  ecx, byte ptr [0x6f24]
        jecxz  loc_1_71DF
loc_1_71CB:
        cli    
        push   esi
        lea    esi, [0x6f78]
        call   0xfffe987d
        pop    esi
        sti    
        add    edi, 0x70
        loop   loc_1_71CB
loc_1_71DF:
        VxDCall  VTD.VTD_Get_Interrupt_Period    ; cd2002000500
        mov    dword ptr [0x6de8], eax
        mov    ecx, eax
        mov    eax, dword ptr [0x6f07]
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f07], eax
        mov    eax, dword ptr [0x6f0b]
        sub    edx, edx
        idiv   ecx
        mov    dword ptr [0x6f0b], eax
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    eax, dword ptr [0x6f0f]
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f0f], eax
        mov    eax, dword ptr [0x6f0b]
        movzx  edx, byte ptr [0x6f26]
        mov    byte ptr [0x6f2d], dl
        test   edx, edx
        je     loc_1_7243
        mul    edx
loc_1_7243:
        mov    edx, eax
        shr    eax, 1
        or     al, 1
        add    eax, edx
        mov    edx, dword ptr [0x6f27]
        test   edx, edx
        je     loc_1_7259
        cmp    eax, edx
        ja     loc_1_725B
loc_1_7259:
        mov    eax, edx
loc_1_725B:
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f13], eax
        mov    al, byte ptr [0x6f2e]
        shr    al, 1
        or     al, 1
        mov    byte ptr [0x6f2f], al
        mov    eax, dword ptr [0x6f1b]
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f1b], eax
        mov    eax, 0x7530
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f17], eax
        mov    eax, 0x1f4
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6ec8], eax
        mov    dword ptr [0x6ec4], eax
        mov    eax, 0x3e8
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6ed0], eax
        mov    dword ptr [0x6ecc], eax
        mov    al, byte ptr [0x6f31]
        cmp    al, byte ptr [0x6f30]
        jbe    loc_1_72F0
        mov    al, byte ptr [0x6f30]
        shr    al, 1
        mov    byte ptr [0x6f31], al
loc_1_72F0:
        clc    
        ret    
sub_1_72F2:
        mov    eax, dword ptr [0x6de8]
        mov    esi, 0x3654
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x6f38], esi
        cmp    esi, 1
        ret    
sub_1_730C:
        enter  0x18, 0
        push   edi
        push   esi
        lea    edi, [0x6e48]
        lea    esi, [0x6de0]
        mov    word ptr [ebp - 8], 8
        mov    word ptr [ebp - 6], 8
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        call   0xfffedc3a
        cmp    dword ptr [edi], 0
        je     loc_1_7346
loc_1_7340:
        stc    
        jmp    loc_1_7413
loc_1_7346:
        cld    
        lea    ebx, [ebp - 0xc]
        mov    edx, 0x43e
loc_1_734F:
        add    edx, 0x3e
        cmp    edx, 0xa8a
        jae    loc_1_740A
        mov    ax, word ptr [edx + 0x20]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], ax
        lea    eax, [edx]
        mov    dword ptr [ebp - 4], eax
        push   edx
        push   dword ptr [edx + 0x22]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0x10]
        push   ebx
        push   0x6e48
        call   0xfffedcfa
        pop    edx
        cmp    dword ptr [0x6e48], 0
        jne    loc_1_73B9
        cmp    dword ptr [edx + 0x22], 3
        je     loc_1_73BE
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        lea    esi, [edx + 0x2e]
        cmp    eax, dword ptr [esi]
        jae    loc_1_73AF
        lea    esi, [edx + 0x2a]
        cmp    eax, dword ptr [esi]
        jb     loc_1_73AF
        mov    esi, dword ptr [ebp - 0xc]
        lea    esi, [esi + 4]
loc_1_73AF:
        mov    edi, dword ptr [edx + 0x3a]
        mov    ecx, dword ptr [edx + 0x36]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_734F
loc_1_73B9:
        lea    esi, [edx + 0x32]
        jmp    loc_1_73AF
loc_1_73BE:
        mov    esi, dword ptr [ebp - 0xc]
        movzx  ecx, word ptr [esi + 4]
        push   edx
        mov    eax, ecx
        call   0xfffe9233
        pop    edx
        movzx  ecx, word ptr [esi + 4]
        mov    esi, dword ptr [esi + 8]
        mov    edi, eax
        test   eax, eax
        je     loc_1_7340
        push   ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        mov    edi, eax
        mov    esi, dword ptr [edx + 0x3a]
        mov    eax, dword ptr [edx + 0x36]
loc_1_73EB:
        cmp    byte ptr [edi], 0
        je     loc_1_734F
        mov    dword ptr [esi], edi
        add    esi, 4
        sub    eax, 4
        je     loc_1_734F
        push   eax
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        pop    eax
        jmp    loc_1_73EB
loc_1_740A:
        push   dword ptr [ebp - 0x10]
        call   0xfffedd00
        clc    
loc_1_7413:
        pop    esi
        pop    edi
        leave  
        ret    
sub_1_7417:
        enter  4, 0
        push   0x42
        push   0x6dec
        push   0x6db4
        lea    eax, [ebp - 4]
        push   eax
        call   0xfffedc04
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0
        je     loc_1_7439
        stc    
loc_1_7439:
        leave  
        ret    
sub_1_743B:
        call   sub_1_7417
        jb     loc_1_7471
        call   sub_1_730C
        jb     loc_1_7473
        call   sub_1_7058
        jb     loc_1_7475
        call   sub_1_709F
        jb     loc_1_7477
        call   sub_1_72F2
        jb     loc_1_7479
        ret    
loc_1_745F:
        push   dword ptr [0x6db4]
        push   0x6e48
        call   0xfffedc0a
        stc    
        ret    
loc_1_7471:
        stc    
        ret    
loc_1_7473:
        jmp    loc_1_745F
loc_1_7475:
        jmp    loc_1_745F
loc_1_7477:
        jmp    loc_1_745F
loc_1_7479:
        jmp    loc_1_745F
        db  000h, 050h, 049h, 047h, 047h, 059h, 042h, 041h, 043h, 04Bh, 041h, 043h, 04Bh, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 032h, 06Fh, 000h, 000h, 042h
        db  049h, 04Eh, 044h, 049h, 04Eh, 047h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 0B8h, 06Dh, 000h, 000h, 053h, 045h, 053h
        db  053h, 049h, 04Fh, 04Eh, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 006h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0F0h, 06Eh, 000h, 000h, 04Eh, 041h, 04Dh, 045h, 043h
        db  041h, 043h, 048h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 025h, 06Fh, 000h, 000h, 04Eh, 041h, 04Dh, 045h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 0F1h, 06Eh, 000h, 000h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 032h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 023h, 06Fh, 000h, 000h, 04Ch, 04Fh, 04Fh, 050h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 003h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 024h
        db  06Fh, 000h, 000h, 044h, 041h, 054h, 041h, 047h, 052h, 041h, 04Dh, 050h, 041h, 043h, 04Bh, 045h
        db  054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 022h, 06Fh, 000h
        db  000h, 054h, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 030h, 075h, 000h, 000h, 004h, 000h, 000h, 000h, 007h, 06Fh, 000h, 000h, 054h
        db  031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h, 000h, 00Bh, 06Fh, 000h, 000h, 054h, 032h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0C8h
        db  000h, 000h, 000h, 004h, 000h, 000h, 000h, 00Fh, 06Fh, 000h, 000h, 041h, 044h, 041h, 050h, 054h
        db  052h, 041h, 054h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0E8h, 003h, 000h
        db  000h, 004h, 000h, 000h, 000h, 027h, 06Fh, 000h, 000h, 057h, 049h, 04Eh, 044h, 04Fh, 057h, 045h
        db  052h, 052h, 04Fh, 052h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 001h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 026h, 06Fh, 000h, 000h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 054h, 049h
        db  04Dh, 045h, 04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h
        db  000h, 01Bh, 06Fh, 000h, 000h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 052h, 045h, 054h, 052h
        db  049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 01Fh
        db  06Fh, 000h, 000h, 057h, 041h, 04Eh, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 021h, 06Fh, 000h
        db  000h, 044h, 04Ch, 043h, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 020h, 06Fh, 000h, 000h, 04Dh
        db  041h, 058h, 049h, 04Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Bh, 06Fh, 000h, 000h, 04Dh, 041h, 058h
        db  04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Ch, 06Fh, 000h, 000h, 050h, 049h, 050h, 045h, 04Ch
        db  049h, 04Eh, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 02Eh, 06Fh, 000h, 000h, 04Dh, 041h, 058h, 054h, 052h, 041h, 04Eh
        db  053h, 04Dh, 049h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 030h, 06Fh, 000h, 000h, 04Dh, 049h, 04Eh, 054h, 052h, 041h, 04Eh, 053h, 04Dh
        db  049h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 031h, 06Fh, 000h, 000h, 04Ch, 041h, 04Eh, 041h, 042h, 041h, 053h, 045h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 033h
        db  06Fh, 000h, 000h, 055h, 053h, 045h, 043h, 053h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 035h, 06Fh, 000h
        db  000h, 051h, 075h, 065h, 072h, 079h, 041h, 06Ch, 077h, 061h, 079h, 073h, 055h, 073h, 065h, 073h
        db  053h, 06Fh, 075h, 072h, 063h, 065h, 052h, 06Fh, 075h, 074h, 069h, 06Eh, 067h, 000h, 000h, 000h
        db  000h, 01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 036h, 06Fh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xa8c  R-X
loc_2_0000:
        mov    al, byte ptr [0x6db0]         ; -> obj1:0x6DB0
        test   al, al
        je     loc_2_000B
        clc    
        ret    
loc_2_000B:
        inc    byte ptr [0x6db0]             ; -> obj1:0x6DB0
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_2_0056
        VxDCall  VNETBIOS.VNETBIOS_Get_Version   ; cd2000001400
        jb     loc_2_0056
        cmp    eax, 0x30a
        jb     loc_2_0056
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_0037
        mov    byte ptr [0x6ec0], 1          ; -> obj1:0x6EC0
loc_2_0037:
        clc    
        pushf  
        cld    
        push   ecx
        push   edi
        push   eax
        xor    eax, eax
        mov    edi, 0x77c0                   ; -> obj1:0x77C0
        mov    ecx, 0x100
        rep stosd dword ptr es:[edi], eax
        pop    eax
        pop    edi
        pop    ecx
        popf   
        call   sub_2_043B
loc_2_0056:
        ret    
        db  000h
sub_2_0058:
        xor    ecx, ecx
        movzx  eax, byte ptr [0x6f22]        ; -> obj1:0x6F22
        imul   eax, eax, 0x84
        add    ecx, eax
        movzx  eax, byte ptr [0x6f23]        ; -> loc_1_6F23
        imul   eax, eax, 0x3e
        add    ecx, eax
        movzx  eax, byte ptr [0x6f24]        ; -> obj1:0x6F24
        imul   eax, eax, 0x70
        add    ecx, eax
        add    ecx, 3
        and    cl, 0xfc
        push   1
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_2_009D
        mov    dword ptr [0x6e3a], eax       ; -> obj1:0x6E3A
        ret    
loc_2_009D:
        stc    
        ret    
sub_2_009F:
        mov    dword ptr [0x6eec], 1         ; -> obj1:0x6EEC
        lea    eax, [0x6f68]                 ; -> obj1:0x6F68
        mov    dword ptr [0x6f68], eax       ; -> obj1:0x6F68
        mov    dword ptr [0x6f6c], eax       ; -> obj1:0x6F6C
        lea    eax, [0x6f70]                 ; -> obj1:0x6F70
        mov    dword ptr [0x6f70], eax       ; -> obj1:0x6F70
        mov    dword ptr [0x6f74], eax       ; -> obj1:0x6F74
        lea    eax, [0x6f78]                 ; -> obj1:0x6F78
        mov    dword ptr [0x6f78], eax       ; -> obj1:0x6F78
        mov    dword ptr [0x6f7c], eax       ; -> obj1:0x6F7C
        lea    eax, [0x6f88]                 ; -> obj1:0x6F88
        mov    dword ptr [0x6f88], eax       ; -> obj1:0x6F88
        mov    dword ptr [0x6f8c], eax       ; -> obj1:0x6F8C
        lea    eax, [0x6f90]                 ; -> obj1:0x6F90
        mov    dword ptr [0x6f90], eax       ; -> obj1:0x6F90
        mov    dword ptr [0x6f94], eax       ; -> obj1:0x6F94
        lea    eax, [0x6f98]                 ; -> obj1:0x6F98
        mov    dword ptr [0x6f98], eax       ; -> obj1:0x6F98
        mov    dword ptr [0x6f9c], eax       ; -> obj1:0x6F9C
        lea    eax, [0x6fa0]                 ; -> obj1:0x6FA0
        mov    dword ptr [0x6fa0], eax       ; -> obj1:0x6FA0
        mov    dword ptr [0x6fa4], eax       ; -> obj1:0x6FA4
        lea    eax, [0x6fa8]                 ; -> obj1:0x6FA8
        mov    dword ptr [0x6fa8], eax       ; -> obj1:0x6FA8
        mov    dword ptr [0x6fac], eax       ; -> obj1:0x6FAC
        lea    eax, [0x6fb0]                 ; -> obj1:0x6FB0
        mov    dword ptr [0x6fb0], eax       ; -> obj1:0x6FB0
        mov    dword ptr [0x6fb4], eax       ; -> obj1:0x6FB4
        lea    eax, [0x6f80]                 ; -> obj1:0x6F80
        mov    dword ptr [0x6f80], eax       ; -> obj1:0x6F80
        mov    dword ptr [0x6f84], eax       ; -> obj1:0x6F84
        mov    edi, 0x6fc0                   ; -> obj1:0x6FC0
        mov    ecx, 0x100
loc_2_0153:
        lea    eax, [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        add    edi, 8
        loop   loc_2_0153
        mov    edi, dword ptr [0x6e3a]       ; -> obj1:0x6E3A
        mov    dword ptr [0x6f64], edi       ; -> obj1:0x6F64
        movzx  ecx, byte ptr [0x6f22]        ; -> obj1:0x6F22
loc_2_0172:
        lea    eax, [edi + 0x34]
        mov    dword ptr [edi + 0x24], eax
        mov    word ptr [edi + 0x1e], 1
        mov    word ptr [edi + 0x10], 2
        cli    
        push   esi
        lea    esi, [0x6f70]                 ; -> obj1:0x6F70
        call   sub_1_287D
        pop    esi
        sti    
        add    edi, 0x84
        loop   loc_2_0172
        mov    dword ptr [0x6f60], edi       ; -> obj1:0x6F60
        movzx  ecx, byte ptr [0x6f23]        ; -> loc_1_6F23
loc_2_01A8:
        mov    word ptr [edi + 0x10], 0x8001
        cli    
        push   esi
        lea    esi, [0x6f68]                 ; -> obj1:0x6F68
        call   sub_1_287D
        pop    esi
        sti    
        add    edi, 0x3e
        loop   loc_2_01A8
        movzx  ecx, byte ptr [0x6f24]        ; -> obj1:0x6F24
        jecxz  loc_2_01DF
loc_2_01CB:
        cli    
        push   esi
        lea    esi, [0x6f78]                 ; -> obj1:0x6F78
        call   sub_1_287D
        pop    esi
        sti    
        add    edi, 0x70
        loop   loc_2_01CB
loc_2_01DF:
        VxDCall  VTD.VTD_Get_Interrupt_Period    ; cd2002000500
        mov    dword ptr [0x6de8], eax       ; -> obj1:0x6DE8
        mov    ecx, eax
        mov    eax, dword ptr [0x6f07]       ; -> loc_1_6F07
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f07], eax       ; -> loc_1_6F07
        mov    eax, dword ptr [0x6f0b]       ; -> loc_1_6F0B
        sub    edx, edx
        idiv   ecx
        mov    dword ptr [0x6f0b], eax       ; -> loc_1_6F0B
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    eax, dword ptr [0x6f0f]       ; -> loc_1_6F0F
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f0f], eax       ; -> loc_1_6F0F
        mov    eax, dword ptr [0x6f0b]       ; -> loc_1_6F0B
        movzx  edx, byte ptr [0x6f26]        ; -> obj1:0x6F26
        mov    byte ptr [0x6f2d], dl         ; -> loc_1_6F2D
        test   edx, edx
        je     loc_2_0243
        mul    edx
loc_2_0243:
        mov    edx, eax
        shr    eax, 1
        or     al, 1
        add    eax, edx
        mov    edx, dword ptr [0x6f27]       ; -> loc_1_6F27
        test   edx, edx
        je     loc_2_0259
        cmp    eax, edx
        ja     loc_2_025B
loc_2_0259:
        mov    eax, edx
loc_2_025B:
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f13], eax       ; -> loc_1_6F13
        mov    al, byte ptr [0x6f2e]         ; -> obj1:0x6F2E
        shr    al, 1
        or     al, 1
        mov    byte ptr [0x6f2f], al         ; -> loc_1_6F2F
        mov    eax, dword ptr [0x6f1b]       ; -> loc_1_6F1B
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f1b], eax       ; -> loc_1_6F1B
        mov    eax, 0x7530
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6f17], eax       ; -> loc_1_6F17
        mov    eax, 0x1f4
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6ec8], eax       ; -> obj1:0x6EC8
        mov    dword ptr [0x6ec4], eax       ; -> obj1:0x6EC4
        mov    eax, 0x3e8
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc    
        adc    eax, 0
        mov    dword ptr [0x6ed0], eax       ; -> obj1:0x6ED0
        mov    dword ptr [0x6ecc], eax       ; -> obj1:0x6ECC
        mov    al, byte ptr [0x6f31]         ; -> loc_1_6F31
        cmp    al, byte ptr [0x6f30]         ; -> obj1:0x6F30
        jbe    loc_2_02F0
        mov    al, byte ptr [0x6f30]         ; -> obj1:0x6F30
        shr    al, 1
        mov    byte ptr [0x6f31], al         ; -> loc_1_6F31
loc_2_02F0:
        clc    
        ret    
sub_2_02F2:
        mov    eax, dword ptr [0x6de8]       ; -> obj1:0x6DE8
        mov    esi, 0x3654                   ; -> loc_1_3654
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x6f38], esi       ; -> obj1:0x6F38
        cmp    esi, 1
        ret    
sub_2_030C:
        enter  0x18, 0
        push   edi
        push   esi
        lea    edi, [0x6e48]                 ; -> loc_1_6E48
        lea    esi, [0x6de0]                 ; -> loc_1_6DE0
        mov    word ptr [ebp - 8], 8
        mov    word ptr [ebp - 6], 8
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        call   sub_1_6C3A
        cmp    dword ptr [edi], 0
        je     loc_2_0346
loc_2_0340:
        stc    
        jmp    loc_2_0413
loc_2_0346:
        cld    
        lea    ebx, [ebp - 0xc]
        mov    edx, 0x43e                    ; -> obj2:0x043E
loc_2_034F:
        add    edx, 0x3e
        cmp    edx, 0xa8a                    ; -> obj2:0x0A8A
        jae    loc_2_040A
        mov    ax, word ptr [edx + 0x20]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], ax
        lea    eax, [edx]
        mov    dword ptr [ebp - 4], eax
        push   edx
        push   dword ptr [edx + 0x22]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0x10]
        push   ebx
        push   0x6e48                        ; -> loc_1_6E48
        call   sub_1_6CFA
        pop    edx
        cmp    dword ptr [0x6e48], 0         ; -> loc_1_6E48
        jne    loc_2_03B9
        cmp    dword ptr [edx + 0x22], 3
        je     loc_2_03BE
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        lea    esi, [edx + 0x2e]
        cmp    eax, dword ptr [esi]
        jae    loc_2_03AF
        lea    esi, [edx + 0x2a]
        cmp    eax, dword ptr [esi]
        jb     loc_2_03AF
        mov    esi, dword ptr [ebp - 0xc]
        lea    esi, [esi + 4]
loc_2_03AF:
        mov    edi, dword ptr [edx + 0x3a]
        mov    ecx, dword ptr [edx + 0x36]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_2_034F
loc_2_03B9:
        lea    esi, [edx + 0x32]
        jmp    loc_2_03AF
loc_2_03BE:
        mov    esi, dword ptr [ebp - 0xc]
        movzx  ecx, word ptr [esi + 4]
        push   edx
        mov    eax, ecx
        call   sub_1_2233
        pop    edx
        movzx  ecx, word ptr [esi + 4]
        mov    esi, dword ptr [esi + 8]
        mov    edi, eax
        test   eax, eax
        je     loc_2_0340
        push   ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        mov    edi, eax
        mov    esi, dword ptr [edx + 0x3a]
        mov    eax, dword ptr [edx + 0x36]
loc_2_03EB:
        cmp    byte ptr [edi], 0
        je     loc_2_034F
        mov    dword ptr [esi], edi
        add    esi, 4
        sub    eax, 4
        je     loc_2_034F
        push   eax
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        pop    eax
        jmp    loc_2_03EB
loc_2_040A:
        push   dword ptr [ebp - 0x10]
        call   sub_1_6D00
        clc    
loc_2_0413:
        pop    esi
        pop    edi
        leave  
        ret    
sub_2_0417:
        enter  4, 0
        push   0x42
        push   0x6dec                        ; -> loc_1_6DEC
        push   0x6db4                        ; -> obj1:0x6DB4
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C04
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0
        je     loc_2_0439
        stc    
loc_2_0439:
        leave  
        ret    
sub_2_043B:
        call   sub_2_0417
        jb     loc_2_0471
        call   sub_2_030C
        jb     loc_2_0473
        call   sub_2_0058
        jb     loc_2_0475
        call   sub_2_009F
        jb     loc_2_0477
        call   sub_2_02F2
        jb     loc_2_0479
        ret    
loc_2_045F:
        push   dword ptr [0x6db4]            ; -> obj1:0x6DB4
        push   0x6e48                        ; -> loc_1_6E48
        call   sub_1_6C0A
        stc    
        ret    
loc_2_0471:
        stc    
        ret    
loc_2_0473:
        jmp    loc_2_045F
loc_2_0475:
        jmp    loc_2_045F
loc_2_0477:
        jmp    loc_2_045F
loc_2_0479:
        jmp    loc_2_045F
        db  000h, 050h, 049h, 047h, 047h, 059h, 042h, 041h, 043h, 04Bh, 041h, 043h, 04Bh, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F32              ; fixup
        db  042h, 049h, 04Eh, 044h, 049h, 04Eh, 047h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  009h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        dd  obj1:0x6DB8              ; fixup
        db  053h, 045h, 053h, 053h, 049h, 04Fh, 04Eh, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h
        db  000h, 000h, 006h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6EF0              ; fixup
        db  04Eh, 041h, 04Dh, 045h, 043h, 041h, 043h, 048h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F25               ; fixup
        db  04Eh, 041h, 04Dh, 045h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h
        db  000h, 000h, 011h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6EF1               ; fixup
        db  050h, 041h, 043h, 04Bh, 045h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 032h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F23               ; fixup
        db  04Ch, 04Fh, 04Fh, 050h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 003h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F24              ; fixup
        db  044h, 041h, 054h, 041h, 047h, 052h, 041h, 04Dh, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 014h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F22              ; fixup
        db  054h, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 030h, 075h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_1_6F07               ; fixup
        db  054h, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_1_6F0B               ; fixup
        db  054h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0C8h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_1_6F0F               ; fixup
        db  041h, 044h, 041h, 050h, 054h, 052h, 041h, 054h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0E8h, 003h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_1_6F27               ; fixup
        db  057h, 049h, 04Eh, 044h, 04Fh, 057h, 045h, 052h, 052h, 04Fh, 052h, 053h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F26              ; fixup
        db  04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 054h, 049h, 04Dh, 045h, 04Fh, 055h, 054h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_1_6F1B               ; fixup
        db  04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F1F               ; fixup
        db  057h, 041h, 04Eh, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F21               ; fixup
        db  044h, 04Ch, 043h, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F20              ; fixup
        db  04Dh, 041h, 058h, 049h, 04Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F2B               ; fixup
        db  04Dh, 041h, 058h, 04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F2C              ; fixup
        db  050h, 049h, 050h, 045h, 04Ch, 049h, 04Eh, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 014h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F2E              ; fixup
        db  04Dh, 041h, 058h, 054h, 052h, 041h, 04Eh, 053h, 04Dh, 049h, 054h, 053h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F30              ; fixup
        db  04Dh, 049h, 04Eh, 054h, 052h, 041h, 04Eh, 053h, 04Dh, 049h, 054h, 053h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F31               ; fixup
        db  04Ch, 041h, 04Eh, 041h, 042h, 041h, 053h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 01Fh, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F33               ; fixup
        db  055h, 053h, 045h, 043h, 053h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_6F35               ; fixup
        db  051h, 075h, 065h, 072h, 079h, 041h, 06Ch, 077h, 061h, 079h, 073h, 055h, 073h, 065h, 073h, 053h
        db  06Fh, 075h, 072h, 063h, 065h, 052h, 06Fh, 075h, 074h, 069h, 06Eh, 067h, 000h, 000h, 000h, 000h
        db  01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj1:0x6F36              ; fixup
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x484  R-X
loc_3_0000:
        enter  0xc, 0
        push   esi
        push   edi
        push   ebx
        call   sub_3_0208
        mov    esi, eax
        test   esi, esi
        jne    loc_3_001C
        mov    eax, 0xc000009a
        jmp    loc_3_0104
loc_3_001C:
        push   esi
        lea    edi, [esi + 0x14]
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [esi]
        mov    esi, dword ptr [esi + 4]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        or     dword ptr [esi], 0x200
        call   sub_1_0211
        jae    loc_3_003E
        jmp    loc_3_0104
loc_3_003E:
        call   sub_1_007C
        jae    loc_3_004F
        mov    eax, 0xc000009a
        jmp    loc_3_0104
loc_3_004F:
        mov    edi, dword ptr [esi + 0x84]
        mov    edx, dword ptr [esi + 0x31]
        mov    dword ptr [edi + 0x12], edx
        mov    dx, word ptr [esi + 0x35]
        mov    word ptr [edi + 0x16], dx
        push   esi
        mov    esi, dword ptr [ebp + 0x14]
        or     esi, esi
        je     loc_3_00BB
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C3A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_00BB
        push   0
        push   0x10                          ; -> obj4:0x0010
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6CFA
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 0xffffffff
        jne    loc_3_00A8
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
loc_3_00A8:
        push   dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        call   sub_1_6D00
        mov    ebx, dword ptr [ebp - 8]
        cmp    ebx, 8
        jb     loc_3_00BD
loc_3_00BB:
        mov    bl, 0xff
loc_3_00BD:
        mov    al, bl
        mov    byte ptr [ebp - 0xc], al
        mov    edx, 0x6a6b                   ; -> loc_1_6A6B
        mov    ecx, dword ptr [ebp + 0x18]
        VxDCall  VNETBIOS.VNETBIOS_Register2     ; cd2005001400
        pop    esi
        jae    loc_3_00E7
        cmp    byte ptr [ebp - 0xc], 0xff
        push   esi
        jne    loc_3_00BB
        pop    esi
loc_3_00DB:
        call   sub_1_02B0
        mov    eax, 0xc000009a
        jmp    loc_3_0104
loc_3_00E7:
        cmp    al, 8
        jb     loc_3_00ED
        jmp    loc_3_00DB
loc_3_00ED:
        movzx  eax, al
        lea    ebx, [eax*4 + 0x6f40]         ; -> obj1:0x6F40
        mov    dword ptr [ebx], esi
        mov    dword ptr [esi], 0x840
        or     dword ptr [esi], 2
        xor    eax, eax
loc_3_0104:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        test   eax, eax
        je     loc_3_0114
        push   esi
        call   sub_3_041F
        pop    ecx
loc_3_0114:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    0x14
sub_3_011B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 8
        mov    ebx, 0x6f40                   ; -> obj1:0x6F40
loc_3_012E:
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        jne    loc_3_0148
        mov    dword ptr [ebx], eax
        mov    eax, ebx
        sub    eax, 0x6f40                   ; -> obj1:0x6F40
        shr    eax, 2
        VxDCall  VNETBIOS.VNETBIOS_Deregister    ; cd2004001400
        jmp    loc_3_014D
loc_3_0148:
        add    ebx, 4
        loop   loc_3_012E
loc_3_014D:
        test   dword ptr [esi], 0x200
        jne    loc_3_0163
        mov    ecx, esp
        push   esi
        call   sub_1_1671
        pop    esi
        call   sub_1_02B0
loc_3_0163:
        call   sub_3_017F
        push   esi
        call   sub_3_041F
        add    esp, 4
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [ebx], eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    0xc
sub_3_017F:
        push   ecx
        push   eax
loc_3_0181:
        push   eax
        mov    ecx, dword ptr [esi + 0xc4]
        mov    ecx, dword ptr [ecx]
        lea    eax, [esi + 0xc4]
        mov    dword ptr [ecx + 4], eax
        xchg   dword ptr [esi + 0xc4], ecx
        cmp    ecx, eax
        lea    ecx, [ecx - 0x24]
        pop    eax
        je     loc_3_01AA
        and    byte ptr [ecx + 0xb8], 0xfe
        jmp    loc_3_0181
loc_3_01AA:
        xor    eax, eax
        mov    ecx, dword ptr [0x6fb8]       ; -> obj1:0x6FB8
loc_3_01B2:
        jecxz  loc_3_01C1
        cmp    dword ptr [ecx + 0x40], esi
        jne    loc_3_01BC
        mov    dword ptr [ecx + 0x40], eax
loc_3_01BC:
        mov    ecx, dword ptr [ecx + 0x18]
        jmp    loc_3_01B2
loc_3_01C1:
        pop    eax
        pop    ecx
        ret    
loc_3_01C4:
        enter  4, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [0x6f38]       ; -> obj1:0x6F38
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    esi, dword ptr [0x6f3c]       ; -> obj1:0x6F3C
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        push   0
        push   dword ptr [0x6e3a]            ; -> obj1:0x6E3A
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        push   dword ptr [0x6db4]            ; -> obj1:0x6DB4
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C0A
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0208:
        enter  4, 0
        movzx  eax, byte ptr [0x6ef1]        ; -> loc_1_6EF1
        imul   eax, eax, 0x44
        add    eax, 0x2bb
        and    al, 0xfc
        push   eax
        push   -1
        push   2
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C76
        pop    ecx
        xor    eax, eax
        mov    edi, dword ptr [ebp - 4]
        test   edi, edi
        jne    loc_3_023B
        jmp    loc_3_041D
loc_3_023B:
        shr    ecx, 2
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [ebp - 4]
        mov    dl, byte ptr [0x6f30]         ; -> obj1:0x6F30
        mov    byte ptr [edi + 0x30], dl
        cmp    dword ptr [0x6f27], 0         ; -> loc_1_6F27
        jne    loc_3_0264
        cmp    dl, byte ptr [0x6f2c]         ; -> obj1:0x6F2C
        jbe    loc_3_0264
        mov    dl, byte ptr [0x6f2c]         ; -> obj1:0x6F2C
loc_3_0264:
        mov    byte ptr [edi + 0x134], dl
        mov    byte ptr [edi + 0x91], 1
        mov    byte ptr [edi + 0x90], 1
        mov    dword ptr [edi + 0x5c], edi
        lea    eax, [edi + 0x94]
        mov    dword ptr [edi + 0x94], eax
        mov    dword ptr [edi + 0x98], eax
        lea    eax, [edi + 0x9c]
        mov    dword ptr [edi + 0x9c], eax
        mov    dword ptr [edi + 0xa0], eax
        lea    eax, [edi + 0xa4]
        mov    dword ptr [edi + 0xa4], eax
        mov    dword ptr [edi + 0xa8], eax
        lea    eax, [edi + 0xbc]
        mov    dword ptr [edi + 0xbc], eax
        mov    dword ptr [edi + 0xc0], eax
        lea    eax, [edi + 0xc4]
        mov    dword ptr [edi + 0xc4], eax
        mov    dword ptr [edi + 0xc8], eax
        lea    eax, [edi + 0xac]
        mov    dword ptr [edi + 0xac], eax
        mov    dword ptr [edi + 0xb0], eax
        lea    eax, [edi + 0xb4]
        mov    dword ptr [edi + 0xb4], eax
        mov    dword ptr [edi + 0xb8], eax
        mov    esi, edi
        add    edi, 0x138
        mov    dword ptr [esi + 0x84], edi
        mov    dword ptr [esi + 0x8c], edi
        mov    dword ptr [esi + 0x88], edi
        movzx  ecx, byte ptr [0x6ef1]        ; -> loc_1_6EF1
        jecxz  loc_3_0330
loc_3_031C:
        mov    dword ptr [edi + 4], esi
        add    edi, 0x44
        loop   loc_3_031C
        sub    edi, 0x44
        mov    dword ptr [esi + 0x88], edi
        add    edi, 0x44
loc_3_0330:
        mov    edx, dword ptr [esi + 0x84]
        mov    byte ptr [edx], 4
        mov    byte ptr [edx + 1], 2
        mov    byte ptr [edx + 2], 1
        lea    eax, [edx + 0x1c]
        mov    dword ptr [edx + 0x1c], eax
        mov    dword ptr [edx + 0x20], eax
        lea    eax, [edx + 0x24]
        mov    dword ptr [edx + 0x24], eax
        mov    dword ptr [edx + 0x28], eax
        lea    eax, [edx + 0x2c]
        mov    dword ptr [edx + 0x2c], eax
        mov    dword ptr [edx + 0x30], eax
        lea    eax, [edx + 0x34]
        mov    dword ptr [edx + 0x34], eax
        mov    dword ptr [edx + 0x38], eax
        lea    eax, [edx + 0x3c]
        mov    dword ptr [edx + 0x3c], eax
        mov    dword ptr [edx + 0x40], eax
        xor    eax, eax
        mov    dword ptr [esi + 0x128], eax
        mov    dword ptr [esi + 0x12c], eax
        mov    dword ptr [esi + 0x78], eax
        push   8
        movzx  eax, byte ptr [0x6f22]        ; -> obj1:0x6F22
        push   eax
        lea    eax, [esi + 0x128]
        push   eax
        lea    eax, [esi + 0x2c]
        push   eax
        call   sub_1_6C82
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_3_03BD
        movzx  eax, byte ptr [0x6f22]        ; -> obj1:0x6F22
        push   eax
        lea    eax, [esi + 0x12c]
        push   eax
        lea    eax, [esi + 0x2c]
        push   eax
        call   sub_1_6CA6
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_3_03C8
loc_3_03BD:
        push   esi
        call   sub_3_041F
        pop    ecx
        xor    eax, eax
        jmp    loc_3_041D
loc_3_03C8:
        xor    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_03D4
        jmp    loc_3_03BD
loc_3_03D4:
        mov    dword ptr [esi + 0x130], eax
        mov    dword ptr [esi + 0x124], 0
        mov    ecx, 6
loc_3_03E9:
        mov    eax, dword ptr [esi + 0x124]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 0x124], edi
        add    edi, 0x40
        loop   loc_3_03E9
        sub    esp, 8
        mov    eax, esp
        push   dword ptr [esi + 0x128]
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6C8E
        pop    eax
        pop    edx
        cmp    edx, 0
        jne    loc_3_03BD
        mov    dword ptr [esi + 0x78], eax
        mov    eax, esi
loc_3_041D:
        leave  
        ret    
sub_3_041F:
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        je     loc_3_0480
        mov    eax, dword ptr [esi + 0x78]
        test   eax, eax
        je     loc_3_0436
        push   eax
        call   sub_1_6C9A
loc_3_0436:
        mov    eax, dword ptr [esi + 0x12c]
        test   eax, eax
        je     loc_3_0446
        push   eax
        call   sub_1_6CAC
loc_3_0446:
        mov    eax, dword ptr [esi + 0x128]
        test   eax, eax
        je     loc_3_0456
        push   eax
        call   sub_1_6C88
loc_3_0456:
        mov    eax, dword ptr [esi + 0x130]
        test   eax, eax
        je     loc_3_0466
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_3_0466:
        movzx  eax, byte ptr [0x6ef1]        ; -> loc_1_6EF1
        imul   eax, eax, 0x44
        add    eax, 0x2bb
        and    al, 0xfc
        push   2
        push   eax
        push   esi
        call   sub_1_6C7C
loc_3_0480:
        leave  
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x18  RW-
        db  04Eh, 065h, 074h, 042h, 045h, 055h, 049h, 05Fh, 06Ch, 061h, 06Eh, 061h, 06Eh, 075h, 06Dh, 000h
        db  008h, 000h, 008h, 000h
        dd  obj4:0x0008              ; fixup
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x80  R-X
        db  04Eh, 044h, 049h, 053h, 048h, 04Ch, 050h, 024h, 000h, 06Fh, 0E9h, 033h, 037h, 000h, 000h, 000h
        db  000h, 0F7h, 0C3h, 003h, 000h, 075h, 063h, 0B8h, 040h, 03Dh, 0BAh, 000h, 000h, 0CDh, 021h, 072h
        db  049h, 08Bh, 0D8h, 08Bh, 0FBh, 08Dh, 036h, 009h, 000h, 08Bh, 0D6h, 0B9h, 008h, 000h, 0B8h, 002h
        db  044h, 0CDh, 021h, 072h, 03Fh, 081h, 03Ch, 033h, 037h, 075h, 039h, 081h, 07Ch, 002h, 06Fh, 0EFh
        db  075h, 032h, 08Bh, 0DFh, 0B4h, 03Eh, 0CDh, 021h, 0B8h, 006h, 000h, 050h, 0FFh, 05Ch, 004h, 083h
        db  0C4h, 002h, 00Bh, 0C0h, 074h, 014h, 00Bh, 0D2h, 074h, 020h, 00Bh, 0DBh, 074h, 01Ch, 08Eh, 044h
        db  006h, 08Bh, 0F8h, 026h, 080h, 07Dh, 02Fh, 000h, 075h, 010h, 033h, 0C0h, 066h, 033h, 0D2h, 033h
        db  0F6h, 033h, 0DBh, 0C3h, 08Bh, 0DFh, 0B4h, 03Eh, 0CDh, 021h, 0B8h, 001h, 080h, 0EBh, 0EDh, 033h
VxD__LTEXT ENDS
