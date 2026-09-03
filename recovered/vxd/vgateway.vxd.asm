; VGATEWAY -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 12308 instructions recovered by recursive descent

Declare_Virtual_Device VGATEWAY, 3, 10, VGATEWAY_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xa5c  R-X
sub_1_0000:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        pop    ecx
        test   eax, eax
        je     loc_1_0015
loc_1_0012:
        cld    
        leave  
        ret    
loc_1_0015:
        mov    ecx, dword ptr [0x6f4]        ; -> obj1:0x06F4
        cmp    word ptr [ecx], 0
        jne    loc_1_0012
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011004000
        push   dword ptr [ebp + 0xc]
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        pop    ecx
        cld    
        leave  
        ret    
sub_1_0034:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        pop    eax
        cld    
        xor    eax, eax
        leave  
        ret    
sub_1_0047:
        push   esi
        mov    eax, 1
        cmp    byte ptr [ebx + 4], 0
        jne    loc_1_0069
        xor    esi, esi
        xchg   dword ptr [ebx + 0x2d0], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        push   ebx
        call   sub_1_0034
        pop    edx
        dec    eax
loc_1_0069:
        pop    esi
        ret    
loc_1_006B:
        VxDCall  VNETBIOS.VNETBIOS_Get_Version   ; cd2000001400
        jb     loc_1_007B
        cmp    eax, 0x30a
        jb     loc_1_007B
        clc    
loc_1_007B:
        ret    
loc_1_007C:
        xor    esi, esi
        xchg   dword ptr [0x6ec], esi        ; -> obj1:0x06EC
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        cmp    dword ptr [0x6e8], 0          ; -> obj1:0x06E8
        je     loc_1_009B
        sub    eax, eax
        VxDCall  Dev_0498.Service_0003h          ; cd2003009804
loc_1_009B:
        clc    
        ret    
VGATEWAY_Control:
        cmp    eax, 1
        je     loc_1_006B
        cmp    eax, 0x1b
        je     loc_1_006B
        cmp    eax, 0x1c
        je     loc_1_007C
        cmp    eax, 0x23
        je     loc_1_00B3
        clc    
        ret    
loc_1_00B3:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0
        jne    loc_1_00C2
        xor    eax, eax
        jmp    loc_1_00E8
loc_1_00C2:
        cmp    ecx, -1
        jne    loc_1_00CB
        xor    eax, eax
        jmp    loc_1_00E8
loc_1_00CB:
        sub    ecx, 1
        cmp    ecx, 5
        jae    loc_1_00ED
        mov    edi, dword ptr [esi + 0x10]
        call   dword ptr [ecx*4 + 0x710]     ; -> obj1:0x0710
        cmp    ax, 1
        jne    loc_1_00ED
        xor    eax, eax
loc_1_00E8:
        clc    
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_00ED:
        mov    eax, 1
        jmp    loc_1_00E8
VGATEWAY_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 6
        jae    loc_1_0127
        mov    word ptr [ebp + 0x1c], 1
        mov    cx, word ptr [ebp + 0x38]
        or     cx, word ptr [ebp + 0x38]
        je     loc_1_011B
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
loc_1_011B:
        call   sub_1_06F8
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_1_0127:
        mov    word ptr [ebp + 0x1c], 0
        ret    
loc_1_012E:
        mov    eax, 0x8287
        ret    
loc_1_0134:
        VxDCall  VNETBIOS.VNETBIOS_Enum          ; cd2003001400
        or     edx, eax
        xor    ecx, ecx
        xor    eax, eax
        mov    dword ptr [0xa28], eax        ; -> loc_1_0A28
        mov    edi, 0xa2c                    ; -> loc_1_0A2C
loc_1_014A:
        bt     edx, ecx
        jae    loc_1_0169
        pushal 
        push   ecx
        call   sub_2_87EB
        pop    ecx
        or     eax, eax
        popal  
        je     loc_1_0165
        bts    dword ptr [0xa28], ecx        ; -> loc_1_0A28
        jmp    loc_1_0169
loc_1_0165:
        mov    dword ptr [eax + edi], ecx
        inc    eax
loc_1_0169:
        inc    ecx
        cmp    ecx, 0x10
        jne    loc_1_014A
        mov    dword ptr [0xa54], eax        ; -> loc_1_0A54
        or     eax, eax
        jne    loc_1_017E
        mov    eax, 0
        ret    
loc_1_017E:
        cmp    dword ptr [0x6e8], 1          ; -> obj1:0x06E8
        je     loc_1_021B
        VxDCall  Dev_0498.Service_0000h          ; cd2000009804
        jb     loc_1_019F
        mov    dword ptr [0x6e8], 1          ; -> obj1:0x06E8
        jmp    loc_1_021B
loc_1_019F:
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        cld    
        mov    esi, edx
        mov    edi, 0x73e                    ; -> obj1:0x073E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [edi], 0
        push   0x724                         ; -> loc_1_0724
        push   0x73e                         ; -> obj1:0x073E
        call   sub_1_0578
        add    esp, 8
        mov    edx, eax
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        jae    loc_1_0207
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        cld    
        mov    esi, edx
        mov    edi, 0x73e                    ; -> obj1:0x073E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [edi], 0
        push   0x731                         ; -> obj1:0x0731
        push   0x73e                         ; -> obj1:0x073E
        call   sub_1_0578
        add    esp, 8
        mov    edx, eax
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_1_021B
loc_1_0207:
        VxDCall  Dev_0498.Service_0000h          ; cd2000009804
        jb     loc_1_021B
        mov    dword ptr [0x6e8], 1          ; -> obj1:0x06E8
        jmp    loc_1_021B
loc_1_021B:
        VxDCall  DOSMGR.DOSMGR_Get_IndosPtr      ; cd2004001500
        mov    dword ptr [0x6f4], eax        ; -> obj1:0x06F4
        call   sub_2_805C
        cmp    dword ptr [0x6e8], 0          ; -> obj1:0x06E8
        je     loc_1_023F
        mov    eax, 0x458                    ; -> loc_1_0458
        VxDCall  Dev_0498.Service_0003h          ; cd2003009804
loc_1_023F:
        call   sub_2_61B0
        mov    eax, 1
        ret    
loc_1_024A:
        push   0x304
        push   0
        call   0
        add    esp, 8
        or     eax, eax
        je     loc_1_02C0
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [edi + 4]
        test   ebx, 1
        je     loc_1_027D
        mov    ebx, 0x5ed                    ; -> loc_1_05ED
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [edi + 8]
        mov    dword ptr [eax + 0x2c4], ebx
        jmp    loc_1_028E
loc_1_027D:
        mov    ebx, 0x5b0                    ; -> loc_1_05B0
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [edi + 8]
        mov    dword ptr [eax + 0x2c4], ebx
loc_1_028E:
        mov    ebx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ebx
        mov    byte ptr [eax + 4], 0
        sub    ebx, ebx
        mov    dword ptr [eax + 0x2d0], ebx
        mov    dword ptr [eax + 0x2c8], ebx
        mov    dword ptr [eax + 0x2cc], ebx
        push   edi
        push   dword ptr [edi + 0x10]
        push   eax
        call   sub_2_7EC8
        add    esp, 8
        pop    edi
        mov    eax, 1
        ret    
loc_1_02C0:
        mov    eax, 0
        ret    
loc_1_02C6:
        xor    eax, eax
        mov    ebx, dword ptr [edi]
        push   word ptr [ebp + 0x10]
        push   word ptr [ebp + 0x38]
        mov    eax, dword ptr [edi + 4]
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        push   edi
        push   ebx
        call   sub_2_8980
        pop    ebx
        pop    edi
        call   sub_1_0047
        pop    word ptr [ebp + 0x38]
        pop    word ptr [ebp + 0x10]
        ret    
loc_1_0303:
        push   edi
        mov    eax, dword ptr [edi + 4]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_2_8980
        add    esp, 8
        pop    edi
        mov    ebx, dword ptr [edi]
        call   sub_1_0047
        ret    
loc_1_031C:
        xor    eax, eax
        mov    ebx, dword ptr [edi]
        mov    ebx, dword ptr [edi + 4]
        cmp    ebx, 0x10
        jb     loc_1_0352
        xor    ecx, ecx
        mov    ecx, dword ptr [0xa28]        ; -> loc_1_0A28
loc_1_0330:
        bsf    ebx, ecx
        jne    loc_1_0339
        sub    eax, eax
        jmp    loc_1_0384
loc_1_0339:
        pushal 
        push   ebx
        call   sub_2_88AB
        pop    ebx
        or     eax, eax
        popal  
        jne    loc_1_034B
        btr    ecx, ebx
        jmp    loc_1_0330
loc_1_034B:
        btr    dword ptr [0xa28], ebx        ; -> loc_1_0A28
loc_1_0352:
        push   ebx
        push   dword ptr [edi]
        call   sub_2_8A6F
        add    esp, 8
        mov    ebx, dword ptr [edi]
        call   sub_1_0047
        or     eax, eax
        je     loc_1_0384
        mov    eax, 0x3e8
        mov    esi, 0x39c                    ; -> loc_1_039C
        mov    edx, dword ptr [edi]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edx + 0x2c8], esi
        mov    ax, 1
loc_1_0384:
        ret    
loc_1_0385:
        xor    eax, eax
        mov    ebx, dword ptr [edi]
        push   ebx
        push   ebx
        call   sub_2_8AA0
        add    esp, 4
        pop    ebx
        push   eax
        call   sub_1_0047
        pop    eax
        ret    
loc_1_039C:
        mov    edi, edx
        sub    esi, esi
        mov    dword ptr [edx + 0x2c8], esi
        cmp    byte ptr [edi + 4], 0
        je     loc_1_03D6
        mov    esi, dword ptr [edi + 0x2cc]
        or     esi, esi
        jne    loc_1_03D6
        push   edx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x3ed                    ; -> loc_1_03ED
        mov    ecx, 0x4b
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    edx
        mov    dword ptr [edx + 0x2cc], esi
loc_1_03D6:
        mov    eax, 0x3e8
        mov    esi, 0x39c                    ; -> loc_1_039C
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edx + 0x2c8], esi
        ret    
loc_1_03ED:
        push   edx
        xor    esi, esi
        mov    dword ptr [edx + 0x2cc], esi
        call   sub_2_0147
        pop    edx
        cmp    byte ptr [edx + 4], 0
        jne    loc_1_0418
        xor    esi, esi
        xchg   dword ptr [edx + 0x2d0], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        push   edx
        call   sub_1_0034
        pop    edx
        ret    
loc_1_0418:
        ret    
sub_1_0419:
        pushal 
        lea    ecx, [ebx - 0x34]
        mov    eax, dword ptr [ecx + 0x24]
        mov    edi, dword ptr [ecx + 0x28]
        bts    dword ptr [edi + 0x2d4], eax
        mov    esi, dword ptr [edi + 0x2d0]
        or     esi, esi
        jne    loc_1_0456
        mov    edx, edi
        push   edi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x490                    ; -> loc_1_0490
        mov    ecx, 0x4b
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    edi
        mov    dword ptr [edi + 0x2d0], esi
loc_1_0456:
        popal  
        ret    
loc_1_0458:
        pushal 
        lea    ecx, [ebx - 0x1c]
        mov    eax, dword ptr [ecx + 0x18]
        bts    dword ptr [0x6f0], eax        ; -> obj1:0x06F0
        mov    esi, dword ptr [0x6ec]        ; -> obj1:0x06EC
        or     esi, esi
        jne    loc_1_048E
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x4d8                    ; -> loc_1_04D8
        mov    ecx, 0x4b
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0x6ec], esi        ; -> obj1:0x06EC
loc_1_048E:
        popal  
        ret    
loc_1_0490:
        pushal 
        cld    
loc_1_0492:
        pushfd 
        cli    
        sub    ebx, ebx
        xchg   dword ptr [edx + 0x2d4], ebx
        or     ebx, ebx
        jne    loc_1_04AA
        mov    dword ptr [edx + 0x2d0], ebx
        popfd  
loc_1_04A7:
        cld    
        popal  
        ret    
loc_1_04AA:
        popfd  
        mov    ecx, 9
loc_1_04B0:
        dec    ecx
        btr    ebx, ecx
        jae    loc_1_04D3
        cld    
        pushal 
        push   ebx
        push   ecx
        push   edx
        call   sub_2_0000
        pop    edx
        pop    ecx
        pop    ebx
        popal  
        cmp    byte ptr [edx + 4], 0
        jne    loc_1_04D3
        push   edx
        call   sub_1_0034
        pop    edx
        jmp    loc_1_04A7
loc_1_04D3:
        inc    ecx
        loop   loc_1_04B0
        jmp    loc_1_0492
loc_1_04D8:
        pushal 
        cld    
        pushfd 
        cli    
        mov    ebx, dword ptr [0x6f0]        ; -> obj1:0x06F0
        xor    esi, esi
        mov    dword ptr [0x6f0], esi        ; -> obj1:0x06F0
        mov    dword ptr [0x6ec], esi        ; -> obj1:0x06EC
        popfd  
        btr    ebx, 9
        jae    loc_1_0502
        pushal 
        cld    
        push   0
        call   sub_2_6F40
        pop    eax
        popal  
loc_1_0502:
        popal  
        cld    
        ret    
sub_1_0505:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x2c8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        xor    esi, esi
        xchg   dword ptr [edi + 0x2cc], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_052B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_054A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, edi
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_0561:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        rep stosb byte ptr es:[edi], al
        mov    eax, edi
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_0578:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        mov    edi, esi
        mov    esi, dword ptr [ebp + 0xc]
        not    ecx
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        adc    ecx, ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_05B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x2c4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        mov    ebx, dword ptr [ebp + 0xc]
        and    ebx, 0xffff
        push   0
        push   0
        push   edx
        push   ecx
        push   ebx
        push   eax
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        cld    
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_05ED:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx + 0xc]
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x2c4]
        call   eax
        add    esp, 0x10
        cld    
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0614:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ecx, dword ptr [0x6f4]        ; -> obj1:0x06F4
        cmp    word ptr [ecx], 0
        jne    loc_1_062C
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011004000
loc_1_062C:
        mov    ebx, dword ptr [ebp + 8]
        cld    
        VxDCall  VNETBIOS.VNETBIOS_Submit        ; cd2002001400
        cld    
        and    eax, 0xff
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_0641:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x6e8], 0          ; -> obj1:0x06E8
        je     loc_1_0665
        cld    
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  Dev_0498.Service_0002h          ; cd2002009804
        cld    
        and    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0665:
        push   dword ptr [ebp + 8]
        call   sub_1_0614
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0672:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [0x6e8], 0          ; -> obj1:0x06E8
        je     loc_1_068C
        cld    
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  Dev_0498.Service_0001h          ; cd2001009804
        cld    
loc_1_068C:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VGATEWAY'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VGATEWAY_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VGATEWAY_PM_API          ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06F8:
        add    dword ptr cs:[eax], eax
        add    byte ptr [ecx + eax], dh      ; -> loc_1_0134
        add    byte ptr [eax], al
        dec    edx
        add    al, byte ptr [eax]
        add    byte ptr [ebx + eax], bl      ; -> loc_1_031C
        add    byte ptr [eax], al
        test   dword ptr [ebx], eax
        add    byte ptr [eax], al
        mov    byte ptr [edx], 0
        add    byte ptr [ecx + eax], dh      ; -> loc_1_0134
        add    byte ptr [eax], al
        dec    edx
        add    al, byte ptr [eax]
        add    byte ptr [ebx + eax], bl      ; -> loc_1_031C
        add    byte ptr [eax], al
        test   dword ptr [ebx], eax
        add    byte ptr [eax], al
        add    eax, dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_0724:
        jae    0x796
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x78762e72
        add    byte ptr fs:[ebx + 0x70], dh
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x38332e72
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0793:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp - 0x75], dl
        in     al, dx
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_07F3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h
loc_1_0840:
        inc    esp
        dec    ecx
        inc    ecx
        dec    esp
        dec    ecx
        dec    esi
        pop    edi
        inc    edi
        inc    ecx
        push   esp
        inc    ebp
        push   edi
        inc    ecx
        pop    ecx
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0854:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0860:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0864:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0868:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_086C:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0870:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0874:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0880:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_088C:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0898:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08A4:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08B0:
        push   ebx
        jns    loc_1_0926
        je     0x91a
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x92c
        jb     loc_1_0921
        outsb  dx, byte ptr [esi]
        je     loc_1_0902
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x935
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x925
        push   ebx
        jb     loc_1_0943
        imul   esp, dword ptr [ebx + 0x65], 0x65525c73
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     loc_1_093D
        inc    ecx
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_1_0951
        pop    esp
        push   eax
        popal  
        jb     0x944
        insd   dword ptr es:[edi], dx
        je     loc_1_094C
        jb     0x95c
        pop    esp
        dec    esi
        je     0x950
        imul   ebp, dword ptr [edi + 0x73], 0x65746147
        ja     0x958
        jns    loc_1_08F9
loc_1_08F9:
        add    byte ptr [eax], al
        db  000h
loc_1_08FC:
        dec    ebp
        popal  
        js     loc_1_094E
        popal  
        insd   dword ptr es:[edi], dx
loc_1_0902:
        jae    loc_1_0905
loc_1_0905:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x61], cl
        js     0x95f
        jae    loc_1_0982
        imul   ebp, dword ptr [edi + 0x6e], 0x69530073
        jp     0x96f
        outsd  dx, dword ptr [esi]
        jb     0x986
        bound  esi, qword ptr [ebp + 0x66]
        add    byte ptr [eax], al
loc_1_0920:
        dec    ebp
loc_1_0921:
        popal  
        js     0x968
        jns    loc_1_0994
loc_1_0926:
        dec    ebp
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x65], dl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     loc_1_0997
        dec    esp
        imul   esi, dword ptr [ebx + 0x74], 0x6e65
        add    byte ptr [eax], al
loc_1_093C:
        inc    ebp
loc_1_093D:
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x42]
loc_1_0943:
        jb     0x9b4
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     loc_1_094C
loc_1_094C:
        dec    esi
        popal  
loc_1_094E:
        insd   dword ptr es:[edi], dx
        push   ebp
loc_1_0951:
        jo     0x9b7
        popal  
        je     0x9bb
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x69440000
        jae    loc_1_09C1
        bound  ebp, qword ptr [ebp + 0x4d]
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_09AF
        ja     0x9cf
        push   edi
        push   0x65536e65
        jae    0x9e6
        imul   ebp, dword ptr [edi + 0x6e], 0x66617254
        imul   sp, word ptr [ebx], 0
loc_1_0980:
        dec    ebp
        popal  
loc_1_0982:
        js     loc_1_09C6
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_09CD
        inc    edx
        jne    0x9f3
        jb     loc_1_09F6
        add    byte ptr fs:[eax], al
loc_1_0994:
        dec    esi
        jne    loc_1_0A04
loc_1_0997:
        push   edx
        arpl   word ptr gs:[esi + 0x51], si
        jne    0xa03
        jb     0xa19
        dec    ecx
        outsb  dx, byte ptr [esi]
        imul   esp, dword ptr fs:[ebx + 0x61], 0x6e6f6974
        jae    loc_1_09AC
loc_1_09AC:
        inc    ecx
        jne    0xa23
loc_1_09AF:
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr fs:[ebx + 0x63], 0x656e6e6f
        arpl   word ptr [eax + eax], si
loc_1_09BC:
        dec    ebp
        jne    0xa2b
        je     loc_1_0A2A
loc_1_09C1:
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_0A0C
loc_1_09C6:
        outsd  dx, dword ptr [esi]
        jb     loc_1_0A40
        popal  
        jb     loc_1_0A30
        push   edx
loc_1_09CD:
        popal  
        je     0xa35
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09D4:
        dec    ebp
        popal  
        js     loc_1_0A1C
        inc    edx
        jne    loc_1_0A42
        jb     0xa45
        push   eax
        jb     loc_1_0A2C
        jb     loc_1_0A56
        jne    0xa59
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [edx + 0x63], dl
        jbe    loc_1_0A38
        push   ebx
loc_1_09F6:
        jne    loc_1_0A5A
        insd   dword ptr es:[edi], dx
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x72655064
        inc    edi
        jb     0xa73
loc_1_0A04:
        jne    0xa76
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_1_0A0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A28:
        add    byte ptr [eax], al
loc_1_0A2A:
        add    byte ptr [eax], al
loc_1_0A2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A38:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A40:
        add    byte ptr [eax], al
loc_1_0A42:
        add    byte ptr [eax], al
loc_1_0A44:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A54:
        add    byte ptr [eax], al
loc_1_0A56:
        add    byte ptr [eax], al
loc_1_0A58:
        add    byte ptr [eax], al
loc_1_0A5A:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x8ac5  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x260
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], eax
loc_2_003D:
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 0x14], eax
        je     loc_2_00E4
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 0x65]
        and    eax, 0xff
        cmp    eax, 0xff
        je     loc_2_00D7
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0x7f
        mov    dword ptr [ebp - 0x28], eax
        jmp    loc_2_00B9
loc_2_008A:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_00D2
loc_2_009F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_00D2
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_00B9:
        cmp    dword ptr [ebp - 0x28], 0x10
        jl     loc_2_009F
        cmp    dword ptr [ebp - 0x28], 0x11
        jle    loc_2_008A
        jmp    loc_2_009F
loc_2_00D2:
        jmp    loc_2_00DF
loc_2_00D7:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], eax
loc_2_00DF:
        jmp    loc_2_003D
loc_2_00E4:
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0113
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0x20]
        add    esp, 8
        jmp    loc_2_00E4
loc_2_0113:
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0142
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0x20]
        add    esp, 8
        jmp    loc_2_0113
loc_2_0142:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0147:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_01D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3EAC
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_2_0194
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_01AC
loc_2_0194:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4C41
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1AAA
        add    esp, 4
loc_2_01AC:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_01D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4C78
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_39E6
        add    esp, 4
loc_2_01D8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_01DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 3
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_01FC
loc_2_01F9:
        inc    dword ptr [ebp - 8]
loc_2_01FC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x300], ecx
        jbe    loc_2_021F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7567
        add    esp, 4
        jmp    loc_2_01F9
loc_2_021F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_77DB
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0255
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
        jmp    loc_2_027A
loc_2_0255:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7DD3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_027A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
loc_2_027A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2f0], 0
        je     loc_2_0296
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0700
        add    esp, 4
loc_2_0296:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2ec]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 6], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_02AB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_02C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_02E6
        jmp    loc_2_031E
loc_2_02E6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 4
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2c0], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3
        jne    loc_2_0312
loc_2_0312:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0D00
        add    esp, 4
loc_2_031E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0323:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2c0]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_2_037C
loc_2_0343:
        mov    word ptr [ebp - 0x20], 0x469
        jmp    loc_2_0395
loc_2_034E:
        mov    word ptr [ebp - 0x20], 0x467
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp - 0x1a]
        add    esi, 0x134
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_2_0395
loc_2_036C:
        mov    word ptr [ebp - 0x20], 0x468
        jmp    loc_2_0395
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_037C:
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_2_0343
        cmp    dword ptr [ebp - 0x24], 1
        je     loc_2_034E
        jmp    loc_2_036C
loc_2_0395:
        mov    eax, dword ptr [ebp - 0x1a]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 8]
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0505
        add    esp, 4
        mov    eax, 1
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [edx + 0x2b4]
        shl    eax, cl
        or     dword ptr [0xa28], eax        ; -> loc_1_0A28
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_03DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_0405
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 1
        jmp    loc_2_040A
loc_2_0405:
        jmp    loc_2_0418
loc_2_040A:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
loc_2_0418:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_2_0420:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x168
        push   eax
        call   sub_2_7A70
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_043C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_063F
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_052A
        call   sub_2_06A8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0488
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 0xe
        sub    eax, eax
        jmp    loc_2_0570
        db  0E9h, 0A2h, 000h, 000h, 000h
loc_2_0488:
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xc
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x1c
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0xc
        mov    byte ptr [ebp - 4], al
        jmp    loc_2_04CE
loc_2_04CE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_051B
loc_2_04DB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x168
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_052A
loc_2_04F5:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_06E3
        add    esp, 4
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 0x14]
        mov    word ptr [ecx], ax
        sub    eax, eax
        jmp    loc_2_0570
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_051B:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_04DB
        jmp    loc_2_04F5
loc_2_052A:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x1e], ax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_0560
        mov    eax, dword ptr [ebp - 8]
        inc    word ptr [eax + 0x20]
loc_2_0560:
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_0570
loc_2_0570:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0575:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_05DE
loc_2_058E:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xfffd
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x1e], ax
        jmp    loc_2_05F7
loc_2_05A6:
        mov    eax, dword ptr [ebp + 0xc]
        dec    word ptr [eax + 0x20]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_05D4
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xfffe
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x1e], ax
loc_2_05D4:
        jmp    loc_2_05F7
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_05DE:
        cmp    dword ptr [ebp - 4], 1
        je     loc_2_05A6
        cmp    dword ptr [ebp - 4], 2
        je     loc_2_058E
        jmp    loc_2_05F7
loc_2_05F7:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_063A
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_06E3
        add    esp, 4
loc_2_063A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_063F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x168]
        mov    dword ptr [ebp - 4], eax
loc_2_0654:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x168
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_069C
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        je     loc_2_068F
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_0697
loc_2_068F:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_06A3
loc_2_0697:
        jmp    loc_2_0654
loc_2_069C:
        sub    eax, eax
        jmp    loc_2_06A3
loc_2_06A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_06A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x24
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_06D6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
loc_2_06D6:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_06DE
loc_2_06DE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_06E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0700:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x75        ; -> obj1:0x0790
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        push   0x10
        push   0x840                         ; -> loc_1_0840
        mov    eax, 0x790                    ; -> obj1:0x0790
        add    eax, 0x1a
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        or     eax, eax
        je     loc_2_0761
        jmp    loc_2_07CD
loc_2_0761:
        mov    al, byte ptr [0x793]          ; -> loc_1_0793
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x250], al
        push   0
        push   0x5ad
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0796
        jmp    loc_2_07CD
loc_2_0796:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x7d2 ; -> loc_2_07D2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x250]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B5A
        add    esp, 4
loc_2_07CD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_07D2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_07E0
loc_2_07E0:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_0809
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_0937
loc_2_0809:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_091E
loc_2_081C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_2_084F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x250]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0xc
        jmp    loc_2_0937
loc_2_084F:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_08D7
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_2_08D7
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_0892
loc_2_088E:
        inc    word ptr [ebp - 4]
loc_2_0892:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_08D7
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8704
        add    esp, 0x10
        jmp    loc_2_088E
loc_2_08D7:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x250]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0xc
        jmp    loc_2_0937
loc_2_08F6:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_0937
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_091E:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_081C
        cmp    dword ptr [ebp - 0xc], 6
        je     loc_2_08D7
        jmp    loc_2_08F6
loc_2_0937:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_093C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0x14], 0
        jmp    loc_2_095F
loc_2_095F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_0988
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_0B55
loc_2_0988:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_09B3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x248], 0
        jne    loc_2_09B3
        mov    dword ptr [ebp - 0x10], 1
loc_2_09B3:
        cmp    dword ptr [0xa24], 0          ; -> loc_1_0A24
        je     loc_2_09EA
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x26c
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_09E5
        mov    dword ptr [ebp - 4], 1
loc_2_09E5:
        jmp    loc_2_09F1
loc_2_09EA:
        mov    dword ptr [ebp - 4], 1
loc_2_09F1:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        or     eax, eax
        jne    loc_2_0A13
        mov    dword ptr [ebp - 0x14], 1
loc_2_0A13:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x37]
        mov    byte ptr [ebp - 0x18], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        mov    byte ptr [ebp - 0xc], al
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_0AC5
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0AC5
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_0AC5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_0AC5
        push   0
        push   0x5ad
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0A83
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_0AC0
loc_2_0A83:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0xa2
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [0xa24]        ; -> loc_1_0A24
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x248], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1420
        add    esp, 8
loc_2_0AC0:
        jmp    loc_2_0B1D
loc_2_0AC5:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_2_0B04
loc_2_0AD8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_0B1D
loc_2_0AE3:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0B1D
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_0B04:
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_2_0AD8
        cmp    dword ptr [ebp - 0x1c], 6
        je     loc_2_0AD8
        jmp    loc_2_0AE3
loc_2_0B1D:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0B55
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 1
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x93c ; -> loc_2_093C
        mov    al, byte ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0xc
loc_2_0B55:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0B5A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0B81
        jmp    loc_2_0C11
loc_2_0B81:
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_0B90
loc_2_0B8C:
        inc    word ptr [ebp - 4]
loc_2_0B90:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_0C11
        push   0
        push   0x5ad
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0BCB
        jmp    loc_2_0C11
loc_2_0BCB:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 1
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x93c ; -> loc_2_093C
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0xc
        jmp    loc_2_0B8C
loc_2_0C11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2b5], 0
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_0C42
loc_2_0C3F:
        inc    dword ptr [ebp - 4]
loc_2_0C42:
        cmp    dword ptr [ebp - 4], 9
        jge    loc_2_0C5F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x2b6], 0
        jmp    loc_2_0C3F
loc_2_0C5F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0C64:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_0C7C
loc_2_0C79:
        inc    dword ptr [ebp - 4]
loc_2_0C7C:
        cmp    dword ptr [ebp - 4], 9
        jge    loc_2_0CAE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x2b6]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_0CA9
        mov    ax, 2
        jmp    loc_2_0CB7
loc_2_0CA9:
        jmp    loc_2_0C79
loc_2_0CAE:
        mov    ax, 1
        jmp    loc_2_0CB7
loc_2_0CB7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0CBC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_0CEB
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E8F
        add    esp, 8
        jmp    loc_2_0CFB
loc_2_0CEB:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
loc_2_0CFB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_0D0E
loc_2_0D0E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_0D45
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2b6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2b5], 1
        jmp    loc_2_0D4F
loc_2_0D45:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2b6], 1
loc_2_0D4F:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_0D5E
loc_2_0D5B:
        inc    dword ptr [ebp - 4]
loc_2_0D5E:
        cmp    dword ptr [ebp - 4], 2
        jg     loc_2_0DBD
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x260]
        mov    dword ptr [ebp - 0xc], eax
loc_2_0D7B:
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x260
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_2_0DB8
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_34D2
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_0D7B
loc_2_0DB8:
        jmp    loc_2_0D5B
loc_2_0DBD:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1f4], 1
        sbb    eax, eax
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2b7], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1f0], 1
        sbb    eax, eax
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2bd], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7A3B
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2b9], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_778F
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2ba], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1A5E
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2b8], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_21E8
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2bb], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_355A
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2bc], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_678E
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2be], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b6]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_0E90
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0F54
        add    esp, 4
        jmp    loc_2_0EC5
loc_2_0E90:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C64
        add    esp, 4
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_2_0EB9
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2b5], 2
        jmp    loc_2_0EC5
loc_2_0EB9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0323
        add    esp, 4
loc_2_0EC5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0ECA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_0ED5
loc_2_0ED5:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x2b6], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b5]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_0F1A
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b6]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_0F1A
        jmp    loc_2_0F4F
loc_2_0F1A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C64
        add    esp, 4
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_2_0F43
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2b5], 2
        jmp    loc_2_0F4F
loc_2_0F43:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0323
        add    esp, 4
loc_2_0F4F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0F54:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 8], eax
loc_2_0F69:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_1022
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_0F8D
loc_2_0F8D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_0FFA
        db  0E9h, 078h, 000h, 000h, 000h
loc_2_0FA5:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0CBC
        add    esp, 8
        jmp    loc_2_101D
loc_2_0FBA:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1e4
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3E42
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        jmp    loc_2_101D
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_0FFA:
        cmp    dword ptr [ebp - 0xc], 1
        je     loc_2_101D
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_2_0FA5
        cmp    dword ptr [ebp - 0xc], 3
        je     loc_2_0FBA
        jmp    loc_2_101D
loc_2_101D:
        jmp    loc_2_0F69
loc_2_1022:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_104E
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_104E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1053:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1085
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_1085:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_2_1090:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x238], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x23a], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x23c], 0
        mov    eax, dword ptr [0xa24]        ; -> loc_1_0A24
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x248], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x24c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x240], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1104:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_111C
loc_2_1118:
        inc    word ptr [ebp - 4]
loc_2_111C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_119B
        push   0
        push   0x5ad
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_1157
        jmp    loc_2_119B
loc_2_1157:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x11a0 ; -> loc_2_11A0
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 0x10
        jmp    loc_2_1118
loc_2_119B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_11A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_2_11BC
loc_2_11BC:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x37]
        mov    byte ptr [ebp - 0x1c], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_2_1228
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_120B
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_1223
loc_2_120B:
        mov    al, byte ptr [ebp - 0xc]
        push   eax
        mov    al, byte ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 0x10
loc_2_1223:
        jmp    loc_2_141B
loc_2_1228:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        cmp    eax, 0x40
        jl     loc_2_125F
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_141B
loc_2_125F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 8], eax
loc_2_127B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_12EF
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 8], eax
        je     loc_2_12A5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_12EA
loc_2_12A5:
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_2_12EA
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0x18]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x1c]
        cmp    eax, ecx
        jne    loc_2_12EA
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_12EF
loc_2_12EA:
        jmp    loc_2_127B
loc_2_12EF:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_130E
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_141B
loc_2_130E:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_2_1402
loc_2_1321:
        push   0
        push   0x5ad
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 0x10
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_1395
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0xa0
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1420
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_139B
loc_2_1395:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], eax
loc_2_139B:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x20], 0x11a0 ; -> loc_2_11A0
        mov    al, byte ptr [ebp - 0xc]
        push   eax
        mov    al, byte ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 0x10
        jmp    loc_2_141B
loc_2_13CB:
        mov    al, byte ptr [ebp - 0xc]
        push   eax
        mov    al, byte ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 0x10
        jmp    loc_2_141B
loc_2_13E8:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_141B
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1402:
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_2_1321
        cmp    dword ptr [ebp - 0x20], 6
        je     loc_2_13CB
        jmp    loc_2_13E8
loc_2_141B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1420:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_14B4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_14B4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x22c]
        mov    dword ptr [ebp - 4], eax
loc_2_146F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_14B4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_2_14AF
        mov    dword ptr [ebp - 0xc], 1
        jmp    loc_2_14B4
loc_2_14AF:
        jmp    loc_2_146F
loc_2_14B4:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_14D8
        jmp    loc_2_14C3
loc_2_14C3:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7B57
        add    esp, 8
        jmp    loc_2_14ED
loc_2_14D8:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7AAA
        add    esp, 8
loc_2_14ED:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x244]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1510
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1515
        add    esp, 4
loc_2_1510:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1515:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_2_151E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_156C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x22c]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_2_156C
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x24c], 0
        je     loc_2_156C
        jmp    loc_2_169C
loc_2_156C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_158F
        jmp    loc_2_169C
loc_2_158F:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 1
        lea    eax, [ebp - 4]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_043C
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x240], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 0x40
        jl     loc_2_1609
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_169C
loc_2_1609:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_167B
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x16a1 ; -> loc_2_16A1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x240]
        mov    al, byte ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x37], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x24], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        jmp    loc_2_169C
        db  0E9h, 01Ch, 000h, 000h, 000h
loc_2_167B:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_151E
loc_2_169C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_16A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        push   2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x240]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0575
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x240], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_1719
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        jmp    loc_2_1770
loc_2_1719:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x40
        jl     loc_2_1746
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_1770
loc_2_1746:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_1770
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1515
        add    esp, 4
loc_2_1770:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1775:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_2_177E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x22c
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_17B1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_177E
loc_2_17B1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_17B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_1821
loc_2_17CF:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [0xa20]        ; -> loc_1_0A20
        cmp    dword ptr [eax + 0x5b], ecx
        jb     loc_2_17E8
        sub    eax, eax
        jmp    loc_2_1928
loc_2_17E8:
        jmp    loc_2_183A
loc_2_17ED:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x23c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x2fc]
        jb     loc_2_1812
        sub    eax, eax
        jmp    loc_2_1928
loc_2_1812:
        jmp    loc_2_183A
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_1821:
        cmp    dword ptr [ebp - 8], 2
        je     loc_2_17CF
        cmp    dword ptr [ebp - 8], 4
        je     loc_2_17ED
        jmp    loc_2_183A
loc_2_183A:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        add    eax, 0x75
        push   eax
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1864
        sub    eax, eax
        jmp    loc_2_1928
loc_2_1864:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x18], ax
        push   8
        push   0x854                         ; -> loc_1_0854
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2c
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x34
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x74
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_18FD
loc_2_18C0:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x238]
        jmp    loc_2_1920
loc_2_18CF:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x23a]
        mov    eax, dword ptr [ebp + 0x14]
        inc    dword ptr [eax + 0x5b]
        jmp    loc_2_1920
loc_2_18E4:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x23c]
        jmp    loc_2_1920
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_18FD:
        cmp    dword ptr [ebp - 0xc], 1
        je     loc_2_18C0
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_2_18CF
        cmp    dword ptr [ebp - 0xc], 4
        je     loc_2_18E4
        jmp    loc_2_1920
loc_2_1920:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_1928
loc_2_1928:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_192D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x18]
        mov    word ptr [ebp - 0xc], ax
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_2_19CF
loc_2_1975:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x238]
        jmp    loc_2_19F2
loc_2_1984:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x23a]
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6850
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_19B1
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x5b]
loc_2_19B1:
        jmp    loc_2_19F2
loc_2_19B6:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x23c]
        jmp    loc_2_19F2
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_19CF:
        cmp    dword ptr [ebp - 0x20], 1
        je     loc_2_1975
        cmp    dword ptr [ebp - 0x20], 2
        je     loc_2_1984
        cmp    dword ptr [ebp - 0x20], 4
        je     loc_2_19B6
        jmp    loc_2_19F2
loc_2_19F2:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_1A59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x238]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1A59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x23a]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1A59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x23c]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1A59
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_1A59:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1A5E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x238]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1A92
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x23a]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_1A9E
loc_2_1A92:
        sub    al, al
        jmp    loc_2_1AA5
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_1A9E:
        mov    al, 1
        jmp    loc_2_1AA5
loc_2_1AA5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1AAA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_1AD5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        jmp    loc_2_1B3A
loc_2_1AD5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x248], 0
        je     loc_2_1AEE
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x248]
loc_2_1AEE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x24c], 0
        je     loc_2_1B3A
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x24c]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x24c], 0
        jne    loc_2_1B3A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x244]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_1B3A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1515
        add    esp, 4
loc_2_1B3A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1B3F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2f8], 0
        je     loc_2_1B62
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x24c], 0xa
loc_2_1B62:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_1B88
loc_2_1B84:
        inc    word ptr [ebp - 4]
loc_2_1B88:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_1BF5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2165
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_1BBA
        jmp    loc_2_1BF5
loc_2_1BBA:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 1
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x1bfa ; -> loc_2_1BFA
        push   0x860                         ; -> loc_1_0860
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 0x10
        jmp    loc_2_1B84
loc_2_1BF5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_1BFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_1C08
loc_2_1C08:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_1C65
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_1C50
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 0x10
loc_2_1C50:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        jmp    loc_2_1FEF
loc_2_1C65:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        cmp    eax, 0x40
        jl     loc_2_1C9C
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_1FEF
loc_2_1C9C:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_2_1CD1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        jmp    loc_2_1FEF
loc_2_1CD1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_2_1FD6
loc_2_1CE4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_2_1D3A
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 0x10
        push   0x864                         ; -> loc_1_0864
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 0x10
        jmp    loc_2_1FEF
loc_2_1D3A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5DE6
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_1D95
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 0x10
        push   0x868                         ; -> loc_1_0868
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 0x10
        jmp    loc_2_1FEF
loc_2_1D95:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xc], 2
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x1d
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xd
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x40], 0
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x44], eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E13
        add    esp, 8
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_1E13
loc_2_1E0F:
        inc    word ptr [ebp - 4]
loc_2_1E13:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_1E3C
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + ecx + 0x48], 0
        jmp    loc_2_1E0F
loc_2_1E3C:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x58
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xd9], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xd8], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xda], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xdb], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xdc], 3
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0xf4], 0
        push   0x86c                         ; -> loc_1_086C
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 0x10
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xd
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_043C
        add    esp, 0x10
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_2_1F21
        db  0E9h, 056h, 000h, 000h, 000h
loc_2_1EE4:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_1FEF
loc_2_1EF9:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_1FEF
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1F21:
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_2_1F3A
        cmp    dword ptr [ebp - 0x1c], 0xe
        je     loc_2_1EE4
        jmp    loc_2_1EF9
loc_2_1F3A:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x64
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x20], 0x1ff4 ; -> loc_2_1FF4
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xd9], 1
        jmp    loc_2_1F6C
loc_2_1F6C:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x1d
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xd
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8232
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
        jmp    loc_2_1FEF
loc_2_1F9B:
        push   0x870                         ; -> loc_1_0870
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 0x10
        jmp    loc_2_1FEF
loc_2_1FBC:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        jmp    loc_2_1FEF
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1FD6:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_1CE4
        cmp    dword ptr [ebp - 0x18], 0x18
        je     loc_2_1F9B
        jmp    loc_2_1FBC
loc_2_1FEF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_1FF4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_2002
loc_2_2002:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        cmp    eax, 0x40
        jl     loc_2_2042
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_2160
loc_2_2042:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_208D
loc_2_2055:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0xd8], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xd9], 3
        jmp    loc_2_209C
loc_2_2073:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_2160
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_208D:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_2055
        jmp    loc_2_2073
loc_2_209C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x44]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_20C8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_2160
loc_2_20C8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xdc]
        and    eax, 0xff
        cmp    eax, 5
        jne    loc_2_20F4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_2160
loc_2_20F4:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xc], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3FE8
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xda]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4088
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xf4], 0
        je     loc_2_2160
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4211
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 0
loc_2_2160:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2165:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_2_7BC5
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2189
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x1e0]
loc_2_2189:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_2191
loc_2_2191:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2196:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x1e0]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_21E3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1e0], 0
        jne    loc_2_21E3
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_21E3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_21E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1e0], 0
        je     loc_2_220A
        sub    al, al
        jmp    loc_2_2211
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_220A:
        mov    al, 1
        jmp    loc_2_2211
loc_2_2211:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2220:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x10], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x12], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x134], 0
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_225C
loc_2_2258:
        inc    word ptr [ebp - 4]
loc_2_225C:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_2_22C3
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ebp + 0xc]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    ecx, ecx
        lea    ecx, [ecx + ecx*8]
        add    ecx, dword ptr [ebp + 8]
        add    ecx, 0x14
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], edx
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ecx + 0x10], ax
        jmp    loc_2_2258
loc_2_22C3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x15c
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2d8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x14c], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_22FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_2367
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_2333
loc_2_232F:
        inc    word ptr [ebp - 4]
loc_2_2333:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_2_2362
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_26FA
        add    esp, 8
        jmp    loc_2_232F
loc_2_2362:
        jmp    loc_2_237E
loc_2_2367:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x10]
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0xc
loc_2_237E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2383:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F09
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_23AE
        mov    ax, 1
        jmp    loc_2_2576
loc_2_23AE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F81
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_23E0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        mov    ax, 1
        jmp    loc_2_2576
loc_2_23E0:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0xc], 1
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0xd], 0
        mov    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0x10], ax
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x12
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xe], 0
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x1e4
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x1f2], eax
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_2450
loc_2_244C:
        inc    word ptr [ebp - 4]
loc_2_2450:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_24CB
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + eax*4 + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp - 0x10]
        add    eax, 0x28
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx + 0x10], ax
        jmp    loc_2_244C
loc_2_24CB:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_24EF
loc_2_24EB:
        inc    word ptr [ebp - 4]
loc_2_24EF:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_256E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3040
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 1
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_254B
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x27d5 ; -> loc_2_27D5
        jmp    loc_2_2555
loc_2_254B:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x2b4e ; -> loc_2_2B4E
loc_2_2555:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_80E0
        add    esp, 0xc
        jmp    loc_2_24EB
loc_2_256E:
        sub    ax, ax
        jmp    loc_2_2576
loc_2_2576:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_257B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_259C
loc_2_2598:
        inc    word ptr [ebp - 4]
loc_2_259C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_2646
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + eax*4 + 0x24]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_2641
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp + 0xc]
        add    eax, 0x28
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_2_2622
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 4], 0
        je     loc_2_2622
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
loc_2_2622:
        mov    ax, word ptr [ebp - 4]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 0x10
        mov    byte ptr [ebp - 0xc], al
        jmp    loc_2_2641
loc_2_2641:
        jmp    loc_2_2598
loc_2_2646:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2667:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xc], 2
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1104
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_26DC
loc_2_2698:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
        jmp    loc_2_26F5
loc_2_26AD:
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, byte ptr [eax + 0x21]
        cmp    eax, 3
        jne    loc_2_26CD
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
loc_2_26CD:
        jmp    loc_2_26F5
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_26DC:
        cmp    dword ptr [ebp - 4], 1
        je     loc_2_26AD
        cmp    dword ptr [ebp - 4], 2
        je     loc_2_2698
        jmp    loc_2_26F5
loc_2_26F5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_26FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x14
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0x24]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_27C1
loc_2_273B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_322C
        add    esp, 8
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_276F
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0xc
        jmp    loc_2_27A0
loc_2_276F:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2383
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_27A0
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0xc
loc_2_27A0:
        jmp    loc_2_27D0
loc_2_27A5:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_61F2
        add    esp, 0xc
        jmp    loc_2_27D0
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_27C1:
        cmp    dword ptr [ebp - 8], 1
        je     loc_2_273B
        jmp    loc_2_27A5
loc_2_27D0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_27D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_27F5
loc_2_27F5:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x14]
        dec    word ptr [eax + 0x10]
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_2831
        jmp    loc_2_28F2
loc_2_2831:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_2866
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
        jmp    loc_2_28F2
loc_2_2866:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_2_3172
        add    esp, 4
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_28D0
        mov    ax, word ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_28CB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
loc_2_28CB:
        jmp    loc_2_28F2
loc_2_28D0:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2667
        add    esp, 8
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0xc
loc_2_28F2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_28F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_2_2919
        jmp    loc_2_2A6F
loc_2_2919:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_29FB
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x134], 1
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x28], eax
        jmp    loc_2_29BF
loc_2_295D:
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, byte ptr [eax + 0xf]
        cmp    eax, 3
        jne    loc_2_2984
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x138], 4
        jmp    loc_2_297F
loc_2_297F:
        jmp    loc_2_2991
loc_2_2984:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x138], 0x80000001
loc_2_2991:
        jmp    loc_2_29D8
loc_2_2996:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x138], 0x80000002
        jmp    loc_2_29D8
loc_2_29A8:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x138], 0x80000006
        jmp    loc_2_29D8
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_29BF:
        cmp    dword ptr [ebp - 0x28], 1
        je     loc_2_295D
        cmp    dword ptr [ebp - 0x28], 2
        je     loc_2_2996
        jmp    loc_2_29A8
loc_2_29D8:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x13b], 0x80
        je     loc_2_29FB
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
        jmp    loc_2_2A6F
loc_2_29FB:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x10]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_2A6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2e8], 1
        jne    loc_2_2A54
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7D31
        add    esp, 4
        mov    dword ptr [ebp - 0x24], eax
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_2_2A54
        jmp    loc_2_2A44
loc_2_2A44:
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
loc_2_2A54:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 2
        push   0
        push   0x467
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 8]
        add    esp, 0xc
loc_2_2A6F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2A74:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_2A82
loc_2_2A82:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_322C
        add    esp, 8
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_2AA4
        jmp    loc_2_2B49
loc_2_2AA4:
        push   2
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2383
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_2B49
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0xc
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     loc_2_2AEC
        jmp    loc_2_2B49
loc_2_2AEC:
        push   0x26
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_2B49
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1e], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x10
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   0x2de3                        ; -> loc_2_2DE3
        push   4
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 0x10
loc_2_2B49:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_2B4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_2B6E
loc_2_2B6E:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x14]
        dec    word ptr [eax + 0x10]
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_2BAA
        jmp    loc_2_2D9B
loc_2_2BAA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_2BDF
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
        jmp    loc_2_2D9B
loc_2_2BDF:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_2_3172
        add    esp, 4
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_2_2CDB
        db  0E9h, 001h, 001h, 000h, 000h
loc_2_2BFD:
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_2C0C
loc_2_2C08:
        inc    word ptr [ebp - 4]
loc_2_2C0C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_2C3C
        mov    ax, word ptr [ebp - 4]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 0x10
        jmp    loc_2_2C08
loc_2_2C3C:
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0xc
        mov    byte ptr [ebp - 0xc], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        or     eax, eax
        je     loc_2_2C86
        jmp    loc_2_2C65
loc_2_2C65:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        jmp    loc_2_2D9B
loc_2_2C86:
        mov    eax, dword ptr [ebp - 0x14]
        mov    byte ptr [eax + 0xc], 3
        push   0x2da0                        ; -> loc_2_2DA0
        push   4
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x1e4
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 0x10
        jmp    loc_2_2CAE
loc_2_2CAE:
        jmp    loc_2_2D9B
loc_2_2CB3:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_2D9B
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_2CDB:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_2CFE
        cmp    dword ptr [ebp - 0x18], 1
        jl     loc_2_2CB3
        cmp    dword ptr [ebp - 0x18], 2
        jle    loc_2_2BFD
        jmp    loc_2_2CB3
loc_2_2CFE:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2667
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2e8], 1
        jne    loc_2_2D4C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7D31
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_2D4C
        jmp    loc_2_2D3C
loc_2_2D3C:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
loc_2_2D4C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        or     eax, eax
        jne    loc_2_2D8D
        jmp    loc_2_2D65
loc_2_2D65:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2f0], 0
        je     loc_2_2D8D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B5A
        add    esp, 4
loc_2_2D8D:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_2D9B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_2DA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_2DAB
loc_2_2DAB:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_2DE3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_2DEE
loc_2_2DEE:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2E13:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        inc    word ptr [eax + 0xe]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2E25:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        dec    word ptr [eax + 0xe]
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_2E8A
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_2E8A
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_2E8A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
loc_2_2E8A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2E8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xc], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x190]
        mov    dword ptr [ebp - 4], eax
loc_2_2EAB:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_2F04
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_2EF7
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x44], ecx
        jne    loc_2_2EF7
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0xc
loc_2_2EF7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_2EAB
loc_2_2F04:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2F09:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x14c], 0
        je     loc_2_2F4E
        push   0x1fc
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2F4E
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x14c]
loc_2_2F4E:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_2F56
loc_2_2F56:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2F5B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x14c]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_2F81:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_2F9D
loc_2_2F95:
        inc    word ptr [ebp - 4]
        add    dword ptr [ebp - 0xc], 0x74
loc_2_2F9D:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_3033
        call   sub_2_7BC5
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_3019
        mov    word ptr [ebp - 8], 0
        jmp    loc_2_2FD2
loc_2_2FCE:
        inc    word ptr [ebp - 8]
loc_2_2FD2:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_2_3010
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x15c
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        jmp    loc_2_2FCE
loc_2_3010:
        mov    ax, 1
        jmp    loc_2_303B
loc_2_3019:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x15c
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_2F95
loc_2_3033:
        sub    ax, ax
        jmp    loc_2_303B
loc_2_303B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3040:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x15c
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_3065
loc_2_3065:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_306A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp - 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_3121
loc_2_30B9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x37]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, ecx
        shl    ecx, 3
        sub    ecx, edx
        mov    edx, dword ptr [ebp - 0xc]
        mov    byte ptr [edx + ecx*4 + 0x26], al
        jmp    loc_2_316D
loc_2_30DB:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        or     eax, 4
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
loc_2_30EF:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        or     eax, 1
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        jmp    loc_2_316D
loc_2_3108:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        or     eax, 2
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        jmp    loc_2_316D
loc_2_3121:
        cmp    dword ptr [ebp - 0x10], 0xe
        jg     loc_2_314A
        je     loc_2_30DB
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_30B9
        cmp    dword ptr [ebp - 0x10], 0xd
        je     loc_2_30EF
        jmp    loc_2_3108
loc_2_314A:
        cmp    dword ptr [ebp - 0x10], 0x15
        jl     loc_2_3108
        cmp    dword ptr [ebp - 0x10], 0x16
        jle    loc_2_30EF
        cmp    dword ptr [ebp - 0x10], 0x19
        je     loc_2_30EF
        jmp    loc_2_3108
loc_2_316D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3172:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_3190
loc_2_318C:
        inc    word ptr [ebp - 4]
loc_2_3190:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_31D4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*4 + 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 8], ax
        jmp    loc_2_318C
loc_2_31D4:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 2
        je     loc_2_31ED
        mov    ax, 3
        jmp    loc_2_3227
loc_2_31ED:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 4
        je     loc_2_3206
        mov    ax, 2
        jmp    loc_2_3227
loc_2_3206:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 1
        je     loc_2_321F
        mov    ax, 1
        jmp    loc_2_3227
loc_2_321F:
        sub    ax, ax
        jmp    loc_2_3227
loc_2_3227:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_322C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_3241:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_3289
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x12
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        je     loc_2_327C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_3284
loc_2_327C:
        sub    ax, ax
        jmp    loc_2_3292
loc_2_3284:
        jmp    loc_2_3241
loc_2_3289:
        mov    ax, 1
        jmp    loc_2_3292
loc_2_3292:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_32A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_32B8
loc_2_32B4:
        inc    word ptr [ebp - 4]
loc_2_32B8:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 6
        jge    loc_2_32ED
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        lea    eax, [eax + eax*2]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x260
        push   eax
        call   sub_2_7A70
        add    esp, 4
        jmp    loc_2_32B4
loc_2_32ED:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_32F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0x7f
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_33AA
loc_2_330C:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_332F
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x278
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_333A
loc_2_332F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x29c
        mov    dword ptr [ebp - 4], eax
loc_2_333A:
        jmp    loc_2_3415
loc_2_333F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_3362
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x26c
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_336D
loc_2_3362:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x290
        mov    dword ptr [ebp - 4], eax
loc_2_336D:
        jmp    loc_2_3415
loc_2_3372:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_3395
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x260
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_33A0
loc_2_3395:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x284
        mov    dword ptr [ebp - 4], eax
loc_2_33A0:
        jmp    loc_2_3415
        db  0E9h, 06Bh, 000h, 000h, 000h
loc_2_33AA:
        sub    dword ptr [ebp - 8], 0x10
        cmp    dword ptr [ebp - 8], 0x13
        ja     loc_2_3372
        shl    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_33C5
loc_2_33C5:
        or     al, 0x33
        add    byte ptr [eax], al
        or     al, 0x33
        add    byte ptr [eax], al
        jb     0x3402
        add    byte ptr [eax], al
        jb     loc_2_3406
        add    byte ptr [eax], al
        aas    
        xor    eax, dword ptr [eax]
        add    byte ptr [ebx + esi], cl      ; -> loc_2_330C
        add    byte ptr [eax], al
        or     al, 0x33
        add    byte ptr [eax], al
        jb     0x3416
        add    byte ptr [eax], al
        jb     0x341a
        add    byte ptr [eax], al
        jb     0x341e
        add    byte ptr [eax], al
        jb     loc_2_3422
        add    byte ptr [eax], al
        jb     loc_2_3426
        add    byte ptr [eax], al
        jb     sub_2_342A
        add    byte ptr [eax], al
        jb     0x342e
        add    byte ptr [eax], al
        jb     loc_2_3432
        add    byte ptr [eax], al
        jb     loc_2_3436
        add    byte ptr [eax], al
        aas    
loc_2_3406:
        xor    eax, dword ptr [eax]
        add    byte ptr [ebx + esi], cl      ; -> loc_2_330C
        add    byte ptr [eax], al
        aas    
        xor    eax, dword ptr [eax]
        add    byte ptr [ebx + esi], cl      ; -> loc_2_330C
        add    byte ptr [eax], al
loc_2_3415:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AAA
loc_2_3422:
        add    esp, 8
        pop    edi
loc_2_3426:
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_342A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
loc_2_3432:
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
loc_2_3436:
        add    eax, 0x34
        mov    dword ptr [ebp - 4], eax
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        call   sub_1_0419
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_344B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x60], 0x419 ; -> sub_1_0419
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x34
        push   eax
        call   sub_1_0614
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     loc_2_34BD
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x35], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x65], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_32F2
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_342A
        add    esp, 8
        jmp    loc_2_34CD
loc_2_34BD:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_32F2
        add    esp, 8
loc_2_34CD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_34D2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x65]
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_2_3523
        push   0x40
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [ebp - 0x40], 0x35
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x34
        mov    dword ptr [ebp - 0x3c], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x64]
        mov    byte ptr [ebp - 0x10], al
        lea    eax, [ebp - 0x40]
        push   eax
        call   sub_1_0614
        add    esp, 4
loc_2_3523:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3530:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x252], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x254], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_355A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x252]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_3583
        mov    al, 1
        jmp    loc_2_358A
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_3583:
        sub    al, al
        jmp    loc_2_358A
loc_2_358A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_358F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x252]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_35B4
        jmp    loc_2_3702
loc_2_35B4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_35CD
        jmp    loc_2_3702
loc_2_35CD:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x26c]
        mov    dword ptr [ebp - 0xc], eax
loc_2_35D9:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x26c
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_2_361D
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        cmp    eax, 0x94
        jne    loc_2_3618
        jmp    loc_2_3613
loc_2_3613:
        jmp    loc_2_3702
loc_2_3618:
        jmp    loc_2_35D9
loc_2_361D:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_3655
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_3650
        jmp    loc_2_364B
loc_2_364B:
        jmp    loc_2_3702
loc_2_3650:
        jmp    loc_2_3678
loc_2_3655:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C73
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_3678
        jmp    loc_2_3673
loc_2_3673:
        jmp    loc_2_3702
loc_2_3678:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3B93
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_3696
        jmp    loc_2_3702
loc_2_3696:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x254], ax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], 0x3707 ; -> loc_2_3707
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x34], 0xb3
        push   0x10
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x3e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
loc_2_3702:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_3707:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_3742
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        jmp    loc_2_39E1
loc_2_3742:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x252]
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_2_3774
        jmp    loc_2_375F
loc_2_375F:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        jmp    loc_2_39E1
loc_2_3774:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x3a]
        and    eax, 0xffff
        cmp    eax, 0xff
        jle    loc_2_37A0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        jmp    loc_2_39E1
loc_2_37A0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_3844
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        cmp    eax, 5
        jne    loc_2_382F
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_382F
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_2_3CD8
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C73
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_382A
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_358F
        add    esp, 8
loc_2_382A:
        jmp    loc_2_383F
loc_2_382F:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
loc_2_383F:
        jmp    loc_2_39E1
loc_2_3844:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 0x10], eax
loc_2_3850:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 0x10]
        je     loc_2_38B6
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_2_38B1
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x38]
        push   eax
        call   sub_2_3ACC
        add    esp, 8
        or     eax, eax
        je     loc_2_38B1
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0CBC
        add    esp, 8
loc_2_38B1:
        jmp    loc_2_3850
loc_2_38B6:
        jmp    loc_2_38BB
loc_2_38BB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3CF6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_2_394B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_390A
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
        jmp    loc_2_392F
loc_2_390A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7DD3
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_392F
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
loc_2_392F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2f0], 0
        je     loc_2_394B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B5A
        add    esp, 4
loc_2_394B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x174]
        mov    dword ptr [ebp - 0x10], eax
loc_2_3957:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        cmp    eax, dword ptr [ebp - 0x10]
        je     loc_2_39D1
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x20]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_2_39CC
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x21]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_39CC
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x38]
        push   eax
        call   sub_2_3ACC
        add    esp, 8
        or     eax, eax
        je     loc_2_39CC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_63A5
        add    esp, 8
loc_2_39CC:
        jmp    loc_2_3957
loc_2_39D1:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
loc_2_39E1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_39E6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x254]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x254]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_3A2E
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_358F
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x254], ax
loc_2_3A2E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3A33:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_3AAE
loc_2_3A4C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x252]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_3A7B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x254], ax
        jmp    loc_2_3A87
loc_2_3A7B:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x252], 2
loc_2_3A87:
        jmp    loc_2_3AC7
loc_2_3A8C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x254], ax
        jmp    loc_2_3AC7
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_3AAE:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_3A4C
        cmp    dword ptr [ebp - 4], 1
        je     loc_2_3A8C
        jmp    loc_2_3AC7
loc_2_3AC7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3ACC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 8], 0
        jmp    loc_2_3AEA
loc_2_3AE6:
        inc    word ptr [ebp - 8]
loc_2_3AEA:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x3a]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_2_3B84
        push   0x10
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ebp - 4]
        add    eax, 0x3c
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_3B7F
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 0x4d]
        and    eax, 7
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_3B66
loc_2_3B50:
        sub    eax, eax
        jmp    loc_2_3B8E
loc_2_3B57:
        mov    eax, 1
        jmp    loc_2_3B8E
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_3B66:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_3B50
        cmp    dword ptr [ebp - 0xc], 4
        je     loc_2_3B50
        jmp    loc_2_3B57
loc_2_3B7F:
        jmp    loc_2_3AE6
loc_2_3B84:
        mov    eax, 1
        jmp    loc_2_3B8E
loc_2_3B8E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3B93:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0x129e
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_3BBF
        sub    eax, eax
        jmp    loc_2_3C23
loc_2_3BBF:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x252], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x34
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        push   0x874                         ; -> loc_1_0874
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x2c
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x74
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x3c], 0x122a
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_3C23
loc_2_3C23:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3C28:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x252], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_3C6E
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_3C6E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3C73:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_3C88:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_3CCC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        test   al, 2
        je     loc_2_3CBF
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_3CC7
loc_2_3CBF:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_3CD3
loc_2_3CC7:
        jmp    loc_2_3C88
loc_2_3CCC:
        sub    eax, eax
        jmp    loc_2_3CD3
loc_2_3CD3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3CD8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xd], al
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3CF6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_3D0B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_3D40
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xfd
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0xd], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_3D0B
loc_2_3D40:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2a8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_3D72
loc_2_3D6A:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_2_3D72:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_3DC0
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_2_3DC0
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0xc]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_2_3D6A
loc_2_3DC0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 8], eax
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_3E34
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xc]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xc], ax
loc_2_3E34:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x12], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3E42:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x2a8
        cmp    dword ptr [eax + 8], ecx
        je     loc_2_3E62
        jmp    loc_2_3EA7
loc_2_3E62:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx]
        je     loc_2_3E9B
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xc]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx]
        mov    word ptr [ecx + 0xc], ax
loc_2_3E9B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
loc_2_3EA7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3EAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_3ED8
        jmp    loc_2_3F62
loc_2_3ED8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2a8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        dec    word ptr [eax + 0xc]
loc_2_3EF1:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_3F62
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_3F62
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xe]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0x12]
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2a8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_3EF1
loc_2_3F62:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3F70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1f4], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1f8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x20c], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x210
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x21c
        push   eax
        call   sub_2_7A70
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_3FE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1f8]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_400C
        jmp    loc_2_4083
loc_2_400C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_403A
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_4083
loc_2_403A:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1f8], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1f8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x438e ; -> sub_2_438E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e0]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x3c], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84A8
        add    esp, 8
loc_2_4083:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4088:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx + eax*4 + 0x28]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_40BC
        sub    ax, ax
        jmp    loc_2_4174
loc_2_40BC:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_40E0
        mov    ax, 1
        jmp    loc_2_4174
loc_2_40E0:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + eax*4 + 0x28], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 1
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp + 0xc]
        add    eax, 0x28
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x438e ; -> sub_2_438E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e0]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x3c], ax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84EA
        add    esp, 0xc
        sub    ax, ax
        jmp    loc_2_4174
loc_2_4174:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4179:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x20], 0x438e ; -> sub_2_438E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e0]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x3c], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_41D3
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84A8
        add    esp, 8
        jmp    loc_2_420C
loc_2_41D3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84EA
        add    esp, 0xc
loc_2_420C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4211:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_421F
loc_2_421F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0xf4], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_438E
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4250:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_425B
loc_2_425B:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x20], 0x46b9 ; -> loc_2_46B9
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8536
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_427A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_42E0
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x10], 1
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0xda]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0xdb]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x36], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x20], 0x48b8 ; -> loc_2_48B8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_85AB
        add    esp, 8
        jmp    loc_2_4389
loc_2_42E0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0xd8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x36], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x210
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_2_432D
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20c]
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_2_4342
loc_2_432D:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4AC2
        add    esp, 8
        jmp    loc_2_4389
loc_2_4342:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x20], 0x48b8 ; -> loc_2_48B8
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x20c]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_85AB
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_4389:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_438E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_43C9
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_46B4
loc_2_43C9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 0x14], al
        jmp    loc_2_43D7
loc_2_43D7:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5095
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_4409
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_46B4
loc_2_4409:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    al, byte ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4FD6
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_4448
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_46B4
loc_2_4448:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_2_467D
loc_2_445B:
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4CD7
        add    esp, 0xc
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_2_4635
loc_2_447C:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    loc_2_4658
loc_2_4491:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0xf4], eax
        jmp    loc_2_4658
loc_2_44B1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_2_4602
loc_2_44C4:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_4518
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_4F9B
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_427A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    loc_2_452E
loc_2_4518:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A1D
        add    esp, 8
loc_2_452E:
        jmp    loc_2_461B
loc_2_4533:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_45B9
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_4F9B
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x38]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e0]
        mov    ecx, dword ptr [ebp - 0xc]
        add    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 0x3c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x2e0]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx + 0x3c], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 2
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4250
        add    esp, 8
        jmp    loc_2_45CF
loc_2_45B9:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A1D
        add    esp, 8
loc_2_45CF:
        jmp    loc_2_461B
loc_2_45D4:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    loc_2_461B
loc_2_4602:
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_2_44C4
        cmp    dword ptr [ebp - 0x20], 6
        je     loc_2_4533
        jmp    loc_2_45D4
loc_2_461B:
        jmp    loc_2_4658
loc_2_4620:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    loc_2_4658
loc_2_4635:
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_2_447C
        cmp    dword ptr [ebp - 0x1c], 1
        je     loc_2_4491
        cmp    dword ptr [ebp - 0x1c], 2
        je     loc_2_44B1
        jmp    loc_2_4620
loc_2_4658:
        jmp    loc_2_46B4
loc_2_465D:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0
        jmp    loc_2_46B4
        db  0E9h, 037h, 000h, 000h, 000h
loc_2_467D:
        cmp    dword ptr [ebp - 0x18], 0
        jl     loc_2_465D
        cmp    dword ptr [ebp - 0x18], 1
        jle    loc_2_445B
        cmp    dword ptr [ebp - 0x18], 6
        je     loc_2_445B
        cmp    dword ptr [ebp - 0x18], 0xa
        je     loc_2_445B
        cmp    dword ptr [ebp - 0x18], 0x18
        je     loc_2_445B
        jmp    loc_2_465D
loc_2_46B4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_46B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5095
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_472A
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4725
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
loc_2_4725:
        jmp    loc_2_48B3
loc_2_472A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 0x10], al
        jmp    loc_2_4738
loc_2_4738:
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    al, byte ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4FD6
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_4791
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_478C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
loc_2_478C:
        jmp    loc_2_48B3
loc_2_4791:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_47B0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_48B3
loc_2_47B0:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4CD7
        add    esp, 0xc
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_2_4886
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_4858
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0x3c]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0x3c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0x3c]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x3c], ax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_427A
        add    esp, 0xc
        jmp    loc_2_4881
loc_2_4858:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
loc_2_4881:
        jmp    loc_2_4896
loc_2_4886:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
loc_2_4896:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
loc_2_48B3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_48B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jne    loc_2_48E1
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x20c]
loc_2_48E1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_490A
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_4A18
loc_2_490A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 0xc], al
        jmp    loc_2_4918
loc_2_4918:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    al, byte ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4FD6
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_494E
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_4A18
loc_2_494E:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_49C4
        db  0E9h, 06Dh, 000h, 000h, 000h
loc_2_4966:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4CD7
        add    esp, 0xc
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_2_499A
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0xc
loc_2_499A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jne    loc_2_49BA
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4B64
        add    esp, 8
loc_2_49BA:
        jmp    loc_2_49D3
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_49C4:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_49D3
        jmp    loc_2_4966
loc_2_49D3:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2ec]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 6], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jne    loc_2_4A18
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4AE7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A42
        add    esp, 4
loc_2_4A18:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4A1D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_4A28
loc_2_4A28:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x21c
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4A42:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_7A70
        add    esp, 4
loc_2_4A57:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x21c
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_4A8A
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_4A57
loc_2_4A8A:
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_4ABD
        jmp    loc_2_4AA8
loc_2_4AA8:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_438E
        add    esp, 8
        jmp    loc_2_4A8A
loc_2_4ABD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4AC2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_4ACD
loc_2_4ACD:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x210
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4AE7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x210
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_4B13
        jmp    loc_2_4B5F
loc_2_4B13:
        jmp    loc_2_4B18
loc_2_4B18:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x48b8 ; -> loc_2_48B8
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x20c]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_85AB
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_4B5F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4B64:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x210]
        mov    dword ptr [ebp - 4], eax
loc_2_4B79:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x210
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_4BF6
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x36]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_2_4BE9
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_4BE9
        jmp    loc_2_4BBA
loc_2_4BBA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_4BF1
loc_2_4BE9:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_2_4BF1:
        jmp    loc_2_4B79
loc_2_4BF6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4BFB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_2_4C04:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x210
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_4C3C
        jmp    loc_2_4C27
loc_2_4C27:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    loc_2_4C04
loc_2_4C3C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4C41:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A42
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_4C73
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4BFB
        add    esp, 4
loc_2_4C73:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4C78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_4CD2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_4CD2
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 6]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_4CD2
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
loc_2_4CD2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4CD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x190]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_4D5D
loc_2_4D06:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_4D58
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [ecx + 0xd8]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_2_4D4B
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_4D58
loc_2_4D4B:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_4D06
loc_2_4D58:
        jmp    loc_2_4DD1
loc_2_4D5D:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_4DD1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [ecx + 0xda]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_2_4DC4
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [ecx + 0xdb]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_2_4DC4
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_4DD1
loc_2_4DC4:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_4D5D
loc_2_4DD1:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_4DE3
        sub    ax, ax
        jmp    loc_2_4E41
loc_2_4DE3:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_4E1E
loc_2_4DFE:
        mov    ax, 1
        jmp    loc_2_4E41
loc_2_4E07:
        mov    ax, 2
        jmp    loc_2_4E41
loc_2_4E10:
        mov    ax, 3
        jmp    loc_2_4E41
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_4E1E:
        cmp    dword ptr [ebp - 0x10], 1
        jl     loc_2_4E10
        cmp    dword ptr [ebp - 0x10], 2
        jle    loc_2_4DFE
        cmp    dword ptr [ebp - 0x10], 3
        je     loc_2_4E07
        jmp    loc_2_4E10
loc_2_4E41:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4E46:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        cmp    eax, 1
        jne    loc_2_4E6E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e0]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_4E75
loc_2_4E6E:
        mov    dword ptr [ebp - 0xc], 0xffff
loc_2_4E75:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x78
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f4]
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x2e4]
        jbe    loc_2_4EA0
        sub    eax, eax
        jmp    loc_2_4F2F
loc_2_4EA0:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_4EC2
        sub    eax, eax
        jmp    loc_2_4F2F
loc_2_4EC2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x1f4], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        push   0x880                         ; -> loc_1_0880
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x2c
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x14], eax
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x34
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x74
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x3c], ax
        jmp    loc_2_4F27
loc_2_4F27:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_4F2F
loc_2_4F2F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4F34:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, dword ptr [ecx + 0x14]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 0x1f4], eax
        jmp    loc_2_4F55
loc_2_4F55:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_4F96
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1f4], 0
        jne    loc_2_4F96
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_4F96:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4F9B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x10], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x11]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x36]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x36], al
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_4FD6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        cmp    eax, 0x40
        jl     loc_2_5025
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        cmp    eax, 1
        jne    loc_2_500E
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_501C
loc_2_500E:
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
loc_2_501C:
        mov    ax, 1
        jmp    loc_2_5090
loc_2_5025:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_506D
loc_2_5032:
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        mov    ax, 1
        jmp    loc_2_5090
loc_2_5049:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        mov    ax, 1
        jmp    loc_2_5090
loc_2_5060:
        sub    ax, ax
        jmp    loc_2_5090
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_506D:
        cmp    dword ptr [ebp - 4], 0x36
        je     loc_2_5049
        cmp    dword ptr [ebp - 4], 0x38
        je     loc_2_5049
        cmp    dword ptr [ebp - 4], 0x3c
        je     loc_2_5032
        jmp    loc_2_5060
loc_2_5090:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5095:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_50C2
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_5132
loc_2_50C2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 8], eax
loc_2_50D7:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_5126
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp - 0x10]
        add    eax, 0x28
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_5119
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_5121
loc_2_5119:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_5132
loc_2_5121:
        jmp    loc_2_50D7
loc_2_5126:
        jmp    loc_2_512B
loc_2_512B:
        sub    eax, eax
        jmp    loc_2_5132
loc_2_5132:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_5140:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x180
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1e4
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x18c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1e0], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1f0], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_51AA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_51B8
loc_2_51B8:
        mov    eax, dword ptr [ebp + 0x10]
        movsx  eax, byte ptr [eax]
        or     eax, eax
        jne    loc_2_51ED
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_51E8
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x12
        mov    dword ptr [ebp - 0xc], eax
loc_2_51E8:
        jmp    loc_2_51F3
loc_2_51ED:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
loc_2_51F3:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6089
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_5219
        jmp    loc_2_5455
loc_2_5219:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_2_523B
        jmp    loc_2_5455
loc_2_523B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_322C
        add    esp, 8
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_5262
        jmp    loc_2_525D
loc_2_525D:
        jmp    loc_2_5455
loc_2_5262:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5DE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_5280
        jmp    loc_2_5455
loc_2_5280:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5EE5
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_52B3
        jmp    loc_2_529E
loc_2_529E:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5EBF
        add    esp, 8
        jmp    loc_2_5455
loc_2_52B3:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xc], 1
        push   0x10
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1d
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2d
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xd
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x40], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x44], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E13
        add    esp, 8
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_2_5340
loc_2_533C:
        inc    word ptr [ebp - 0x10]
loc_2_5340:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_5369
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 0x48], 0
        jmp    loc_2_533C
loc_2_5369:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x58
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xd9], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xd8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xdc], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xdb], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 0
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_2_53BC
loc_2_53B8:
        inc    word ptr [ebp - 0x10]
loc_2_53BC:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_5449
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6007
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x58
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1c], 1
        mov    al, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x1d], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x2c], 0x545a ; -> loc_2_545A
        jmp    loc_2_5423
loc_2_5423:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xd
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1d
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8232
        add    esp, 0x10
        jmp    loc_2_53B8
loc_2_5449:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
loc_2_5455:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_545A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 8], al
        jmp    loc_2_548F
loc_2_548F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0x40
        jl     loc_2_54B5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + ecx + 0x48], 1
        jmp    loc_2_5570
loc_2_54B5:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_2_5561
loc_2_54C8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xda], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xdb], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xdc], 3
loc_2_54F0:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x58
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_5548
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1e4
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x2c], 0x576d ; -> loc_2_576D
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_34D2
        add    esp, 8
        jmp    loc_2_5543
loc_2_5543:
        jmp    loc_2_54F0
loc_2_5548:
        jmp    loc_2_554D
loc_2_554D:
        jmp    loc_2_5570
        db  0E9h, 000h, 000h, 000h, 000h, 0E9h, 014h, 000h, 000h, 000h, 0E9h, 00Fh, 000h, 000h, 000h
loc_2_5561:
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_2_54C8
        jmp    loc_2_5570
loc_2_5570:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6031
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x58
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_55A1
        jmp    loc_2_5768
loc_2_55A1:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x44]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_55CD
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5768
loc_2_55CD:
        mov    word ptr [ebp - 0x1c], 0
        jmp    loc_2_55DC
loc_2_55D8:
        inc    word ptr [ebp - 0x1c]
loc_2_55DC:
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_5635
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx + 0x48]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_5630
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_5768
loc_2_5630:
        jmp    loc_2_55D8
loc_2_5635:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xdc]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_2_5661
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5768
loc_2_5661:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xc], 2
        lea    eax, [ebp - 0x20]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xd
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_043C
        add    esp, 0x10
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x28], eax
        jmp    loc_2_56D9
        db  0E9h, 060h, 000h, 000h, 000h
loc_2_569C:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5768
loc_2_56B1:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_5768
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_56D9:
        cmp    dword ptr [ebp - 0x28], 0
        je     loc_2_56FC
        cmp    dword ptr [ebp - 0x28], 0xd
        jl     loc_2_56B1
        cmp    dword ptr [ebp - 0x28], 0xe
        jle    loc_2_569C
        jmp    loc_2_56B1
loc_2_56FC:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x64
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x20], 0x58ac ; -> loc_2_58AC
        jmp    loc_2_5724
loc_2_5724:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x2d
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xd
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 0x10
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xd9], 2
        push   0x5880                        ; -> loc_2_5880
        push   0x1e
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xdd
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 0x10
loc_2_5768:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_576D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x34]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x7f
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_5846
loc_2_579F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_2_57DC
        db  0E9h, 000h, 000h, 000h, 000h
loc_2_57B1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0xc
        jmp    loc_2_587B
        db  0E9h, 000h, 000h, 000h, 000h, 0E9h, 014h, 000h, 000h, 000h, 0E9h, 00Fh, 000h, 000h, 000h
loc_2_57DC:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_57B1
        jmp    loc_2_57EB
loc_2_57EB:
        jmp    loc_2_585F
        db  0E9h, 000h, 000h, 000h, 000h
loc_2_57F5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x21
        je     loc_2_5811
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x22
        jne    loc_2_583C
loc_2_5811:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 0x10
loc_2_583C:
        jmp    loc_2_585F
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_5846:
        cmp    dword ptr [ebp - 0x10], 0x10
        je     loc_2_579F
        cmp    dword ptr [ebp - 0x10], 0x12
        je     loc_2_57F5
        jmp    loc_2_585F
loc_2_585F:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6031
        add    esp, 8
loc_2_587B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_5880:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_588E
loc_2_588E:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x64
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_34D2
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_58AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xdd
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3E42
        add    esp, 8
        jmp    loc_2_58D8
loc_2_58D8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        cmp    eax, 0x40
        jl     loc_2_5914
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_5A54
        db  0E9h, 05Ah, 000h, 000h, 000h
loc_2_5914:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_595F
loc_2_5927:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0xd8], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xd9], 3
        jmp    loc_2_596E
loc_2_5945:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5A54
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_595F:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_5927
        jmp    loc_2_5945
loc_2_596E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x44]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_599A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5A54
loc_2_599A:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xdc]
        and    eax, 0xff
        cmp    eax, 5
        jne    loc_2_59C6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5A54
loc_2_59C6:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xc], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3FE8
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xda]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4088
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_5A1D
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5A54
loc_2_5A1D:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xf4], 0
        je     loc_2_5A54
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4211
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xf4], 0
loc_2_5A54:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5A59:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_5AF4
loc_2_5A75:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xdc], 5
        jmp    loc_2_5B17
loc_2_5A84:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_5ABC
loc_2_5A94:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xdc], 5
        jmp    loc_2_5AD5
loc_2_5AA3:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xd9], 5
        jmp    loc_2_5AD5
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_5ABC:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_5AA3
        cmp    dword ptr [ebp - 8], 1
        je     loc_2_5A94
        jmp    loc_2_5AD5
loc_2_5AD5:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    loc_2_5B17
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_5AF4:
        cmp    dword ptr [ebp - 4], 1
        jl     loc_2_5B17
        cmp    dword ptr [ebp - 4], 2
        jle    loc_2_5A75
        cmp    dword ptr [ebp - 4], 3
        je     loc_2_5A84
        jmp    loc_2_5B17
loc_2_5B17:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5B1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x64
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xc], 4
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xf4], 0
        je     loc_2_5B6C
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xf4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4211
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf4], 0
loc_2_5B6C:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xdc]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_5BCB
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xda]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x5c30 ; -> loc_2_5C30
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xdb]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0xc
        jmp    loc_2_5C2B
loc_2_5BCB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xd9]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_5C1B
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x5d03 ; -> loc_2_5D03
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xd8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0xc
        jmp    loc_2_5C2B
loc_2_5C1B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5D81
        add    esp, 8
loc_2_5C2B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_5C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc], 5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 4], al
        jmp    loc_2_5C57
loc_2_5C57:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x21
        je     loc_2_5C73
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x22
        jne    loc_2_5C9E
loc_2_5C73:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 0x10
loc_2_5C9E:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xd9]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_5CEE
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x20], 0x5d03 ; -> loc_2_5D03
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xd8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0xc
        jmp    loc_2_5CFE
loc_2_5CEE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5D81
        add    esp, 8
loc_2_5CFE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_5D03:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc], 6
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x21
        je     loc_2_5D41
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x22
        jne    loc_2_5D6C
loc_2_5D41:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x36]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xffff00ff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 0x10
loc_2_5D6C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5D81
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5D81:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x40], 0
        je     loc_2_5DB5
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x40]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0575
        add    esp, 0xc
loc_2_5DB5:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5EBF
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E25
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5DE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x18c], 0
        je     loc_2_5E30
        push   0xf8
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_5E30
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x18c]
        jmp    loc_2_5E37
loc_2_5E30:
        sub    eax, eax
        jmp    loc_2_5EBA
loc_2_5E37:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x58
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xdd
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xeb], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x64
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        push   0x88c                         ; -> loc_1_088C
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x90
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x98
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_5EBA
loc_2_5EBA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5EBF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x18c]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_5EE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_2_5F04
loc_2_5EF9:
        inc    word ptr [ebp - 0xc]
        add    dword ptr [ebp - 4], 0x80
loc_2_5F04:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_5FFA
        push   0x80
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_5F93
        mov    word ptr [ebp - 8], 0
        jmp    loc_2_5F43
loc_2_5F3F:
        inc    word ptr [ebp - 8]
loc_2_5F43:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_2_5F85
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x180
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6031
        add    esp, 8
        jmp    loc_2_5F3F
loc_2_5F85:
        mov    ax, 1
        jmp    loc_2_6002
        db  0E9h, 062h, 000h, 000h, 000h
loc_2_5F93:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        push   0x898                         ; -> loc_1_0898
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x38
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x40
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x180
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x1f0]
        jmp    loc_2_5EF9
loc_2_5FFA:
        sub    ax, ax
        jmp    loc_2_6002
loc_2_6002:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6007:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x180
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_602C
loc_2_602C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6031:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x1f0]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_6084
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1f0], 0
        jne    loc_2_6084
        push   7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_6084:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6089:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x190]
        mov    dword ptr [ebp - 4], eax
loc_2_609E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x190
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_611A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x1d
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_610D
        push   0x10
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xd
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_610D
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx + 0xc]
        and    ecx, 0xff
        cmp    ecx, 3
        je     loc_2_610D
        mov    ax, 1
        jmp    loc_2_6122
loc_2_610D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_609E
loc_2_611A:
        sub    ax, ax
        jmp    loc_2_6122
loc_2_6122:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6127:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_613C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_61A4
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x12
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        je     loc_2_6177
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_619F
loc_2_6177:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        cmp    eax, 2
        je     loc_2_6197
        sub    eax, eax
        jmp    loc_2_61AB
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_6197:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_61AB
loc_2_619F:
        jmp    loc_2_613C
loc_2_61A4:
        sub    eax, eax
        jmp    loc_2_61AB
loc_2_61AB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_61B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0xa10], 0          ; -> loc_1_0A10
        push   0xa14                         ; -> loc_1_0A14
        call   sub_2_7A70
        add    esp, 4
        push   0xa44                         ; -> loc_1_0A44
        call   sub_2_7A70
        add    esp, 4
        mov    dword ptr [0xa58], 0          ; -> loc_1_0A58
        mov    ax, 1
        jmp    loc_2_61ED
loc_2_61ED:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_61F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6850
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_6220
        sub    ax, ax
        jmp    loc_2_63A0
loc_2_6220:
        mov    ax, word ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_697F
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_624B
        mov    ax, 1
        jmp    loc_2_63A0
loc_2_624B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_68BF
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_633C
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_6A82
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_6296
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        mov    ax, 1
        jmp    loc_2_63A0
loc_2_6296:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x2a]
        push   eax
        mov    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_6B3C
        add    esp, 0xc
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_62E1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_6B13
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        mov    ax, 1
        jmp    loc_2_63A0
loc_2_62E1:
        mov    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x2e], ax
        mov    byte ptr [ebp - 0x18], 0
        jmp    loc_2_62F9
loc_2_62F6:
        inc    byte ptr [ebp - 0x18]
loc_2_62F9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_633C
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x11], 0x705d ; -> loc_2_705D
        mov    al, byte ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_2_6D87
        add    esp, 0xc
        jmp    loc_2_62F6
loc_2_633C:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_6372
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x21], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6479
        add    esp, 4
        jmp    loc_2_6379
loc_2_6372:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 0
loc_2_6379:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x3b
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xc
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0xc], ecx
        sub    ax, ax
        jmp    loc_2_63A0
loc_2_63A0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_63A5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_2_6407
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3b
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_6407
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_6D38
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_6B13
        add    esp, 4
loc_2_6407:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x20]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_642E
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x25
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3E42
        add    esp, 8
loc_2_642E:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4f
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_645F
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_642E
loc_2_645F:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        jmp    loc_2_6474
loc_2_6474:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6479:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x174]
        mov    dword ptr [ebp - 0x20], eax
loc_2_648E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        cmp    eax, dword ptr [ebp - 0x20]
        je     loc_2_65E2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_64B8
        jmp    loc_2_65E2
loc_2_64B8:
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x20]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_65DD
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x23]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_6554
        push   0x10
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x21]
        mov    word ptr [ebp - 0x18], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x21]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_653A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
loc_2_653A:
        mov    ax, word ptr [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0xc
        jmp    loc_2_65DD
loc_2_6554:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x21]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_65CF
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0xc
        mov    byte ptr [ebp - 0x1c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        or     eax, eax
        je     loc_2_65A4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        jmp    loc_2_65CA
loc_2_65A4:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 3
        push   0x6813                        ; -> loc_2_6813
        push   4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x25
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 0x10
        jmp    loc_2_65CA
loc_2_65CA:
        jmp    loc_2_65DD
loc_2_65CF:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_65DD:
        jmp    loc_2_648E
loc_2_65E2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_65E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x20], 0
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x28], 0
        mov    dword ptr [ebp - 0x14], 0
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x174]
        mov    dword ptr [ebp - 0x24], eax
loc_2_6624:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        cmp    eax, dword ptr [ebp - 0x24]
        je     loc_2_6743
        mov    eax, dword ptr [ebp - 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x24], eax
loc_2_6643:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x4f
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x1c], eax
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_2_673E
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_2_668A
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x248], 0
        jne    loc_2_668A
        mov    dword ptr [ebp - 0x20], 1
loc_2_668A:
        cmp    dword ptr [0xa24], 0          ; -> loc_1_0A24
        je     loc_2_66C1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x26c
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_66BC
        mov    dword ptr [ebp - 0x18], 1
loc_2_66BC:
        jmp    loc_2_66C8
loc_2_66C1:
        mov    dword ptr [ebp - 0x18], 1
loc_2_66C8:
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        or     eax, eax
        jne    loc_2_66EA
        mov    dword ptr [ebp - 0x28], 1
loc_2_66EA:
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_2_6729
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_6729
        cmp    dword ptr [ebp - 0x28], 0
        je     loc_2_6729
        mov    dword ptr [ebp - 0x14], 1
        jmp    loc_2_6714
loc_2_6714:
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1420
        add    esp, 8
        jmp    loc_2_6739
loc_2_6729:
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
loc_2_6739:
        jmp    loc_2_6643
loc_2_673E:
        jmp    loc_2_6624
loc_2_6743:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_2_675B
        mov    eax, dword ptr [0xa24]        ; -> loc_1_0A24
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x248], eax
loc_2_675B:
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x1c], eax
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_2_6789
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    loc_2_675B
loc_2_6789:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_678E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_2_6797:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_67D6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x174]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_63A5
        add    esp, 8
        jmp    loc_2_6797
loc_2_67D6:
        push   0xa44                         ; -> loc_1_0A44
        call   sub_2_7B96
        add    esp, 4
        mov    ecx, eax
        and    ecx, 0xffff
        or     ecx, ecx
        jne    loc_2_67FF
        mov    al, 1
        jmp    loc_2_680E
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_67FF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0xa10], eax        ; -> loc_1_0A10
        sub    al, al
        jmp    loc_2_680E
loc_2_680E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_6813:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_681E
loc_2_681E:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_2_6850:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x174]
        mov    dword ptr [ebp - 8], eax
loc_2_6865:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_68B3
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        je     loc_2_68A6
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_68AE
loc_2_68A6:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_68BA
loc_2_68AE:
        jmp    loc_2_6865
loc_2_68B3:
        sub    eax, eax
        jmp    loc_2_68BA
loc_2_68BA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_68BF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0xa14]        ; -> loc_1_0A14
        mov    dword ptr [ebp - 8], eax
loc_2_68D0:
        cmp    dword ptr [ebp - 8], 0xa14    ; -> loc_1_0A14
        je     loc_2_691A
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        push   0x10
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1a
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        je     loc_2_690D
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_6915
loc_2_690D:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_6921
loc_2_6915:
        jmp    loc_2_68D0
loc_2_691A:
        sub    eax, eax
        jmp    loc_2_6921
loc_2_6921:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6926:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0xa14]        ; -> loc_1_0A14
        mov    dword ptr [ebp - 8], eax
loc_2_6937:
        cmp    dword ptr [ebp - 8], 0xa14    ; -> loc_1_0A14
        je     loc_2_6973
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2a], ecx
        je     loc_2_6966
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_696E
loc_2_6966:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_697A
loc_2_696E:
        jmp    loc_2_6937
loc_2_6973:
        sub    eax, eax
        jmp    loc_2_697A
loc_2_697A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_697F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x5f
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_69A8
        sub    eax, eax
        jmp    loc_2_6A54
loc_2_69A8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 0
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 0
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x23], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x25
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x33], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x3b
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x47], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x4b], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x4f
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x5b], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_6A54
loc_2_6A54:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6A59:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x50
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_6AAB
        sub    eax, eax
        jmp    loc_2_6B0E
loc_2_6AAB:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x18], 0
        push   0x10
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1a
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [0xa58]        ; -> loc_1_0A58
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x2a], eax
        inc    dword ptr [0xa58]             ; -> loc_1_0A58
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0xa14                         ; -> loc_1_0A14
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_6B0E
loc_2_6B0E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6B13:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6B3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_6B60
loc_2_6B5D:
        inc    dword ptr [ebp - 4]
loc_2_6B60:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_2_6C04
        push   0x59
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_6BC7
loc_2_6B85:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_6BBE
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_6C11
        add    esp, 4
        jmp    loc_2_6B85
loc_2_6BBE:
        mov    ax, 1
        jmp    loc_2_6C0C
loc_2_6BC7:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xd], eax
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x19
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_6B5D
loc_2_6C04:
        sub    ax, ax
        jmp    loc_2_6C0C
loc_2_6C0C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6C11:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6C2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_6C52
loc_2_6C4F:
        inc    dword ptr [ebp - 4]
loc_2_6C52:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_2_6D0E
        push   0x606
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_6CBC
loc_2_6C7A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_6CB3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
        jmp    loc_2_6C7A
loc_2_6CB3:
        mov    ax, 1
        jmp    loc_2_6D16
loc_2_6CBC:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xd], eax
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x19
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x59
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1d], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x21], 0x5ad
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_6C4F
loc_2_6D0E:
        sub    ax, ax
        jmp    loc_2_6D16
loc_2_6D16:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6D1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6D38:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_6D46
loc_2_6D46:
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_6D55
loc_2_6D51:
        inc    word ptr [ebp - 4]
loc_2_6D55:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_6D82
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1a
        push   eax
        call   sub_2_6EDF
        add    esp, 8
        jmp    loc_2_6D51
loc_2_6D82:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6D87:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x19], 0xb6
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x33
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x15], 9
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x49], al
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0xa44                         ; -> loc_1_0A44
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x45], 0x458 ; -> loc_1_0458
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19
        push   eax
        call   sub_1_0614
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     loc_2_6E26
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x1a], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x4a], al
        push   0
        call   sub_2_6F40
        add    esp, 4
loc_2_6E26:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6E2B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x19], 0xa1
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x1c], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x59
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x21], 0x5ad
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x15], 9
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x4a], 0xff
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x49], al
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0xa44                         ; -> loc_1_0A44
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x45], 0x458 ; -> loc_1_0458
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19
        push   eax
        call   sub_1_0614
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     loc_2_6EDA
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x1a], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x4a], al
        push   0
        call   sub_2_6F40
        add    esp, 4
loc_2_6EDA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_6EDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x31        ; -> obj1:0x0790
        push   0x10
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, 0x790                    ; -> obj1:0x0790
        add    eax, 0x1a
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0641
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h
sub_2_6F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [0xa44]        ; -> loc_1_0A44
        mov    dword ptr [ebp - 8], eax
loc_2_6F5D:
        cmp    dword ptr [ebp - 8], 0xa44    ; -> loc_1_0A44
        je     loc_2_6FC2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0x4a]
        and    eax, 0xff
        cmp    eax, 0xff
        je     loc_2_6FB5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    loc_2_6FBD
loc_2_6FB5:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_2_6FBD:
        jmp    loc_2_6F5D
loc_2_6FC2:
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_6FED
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        call   dword ptr [eax + 0x11]
        add    esp, 4
        jmp    loc_2_6FC2
loc_2_6FED:
        cmp    dword ptr [0xa10], 0          ; -> loc_1_0A10
        je     loc_2_7051
        push   0xa44                         ; -> loc_1_0A44
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_7051
        push   8
        mov    eax, dword ptr [0xa10]        ; -> loc_1_0A10
        push   eax
        call   sub_2_0ECA
        add    esp, 8
        mov    eax, dword ptr [0xa10]        ; -> loc_1_0A10
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_7047
        mov    eax, dword ptr [0xa10]        ; -> loc_1_0A10
        push   eax
        call   sub_1_0034
        add    esp, 4
loc_2_7047:
        mov    dword ptr [0xa10], 0          ; -> loc_1_0A10
loc_2_7051:
        sub    eax, eax
        jmp    loc_2_7058
loc_2_7058:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_705D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd]
        push   eax
        call   sub_2_6926
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_7093
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6C11
        add    esp, 4
        jmp    loc_2_72D3
loc_2_7093:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [ebp - 8], al
        jmp    loc_2_70A1
loc_2_70A1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x2c], eax
        jmp    loc_2_70FE
loc_2_70B4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1c]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    byte ptr [edx + ecx*2 + 0x30], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + eax*2 + 0x31], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19
        push   eax
        call   sub_1_0672
        add    esp, 4
        jmp    loc_2_7117
loc_2_70E7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + eax*2 + 0x31], 1
        jmp    loc_2_7117
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_70FE:
        cmp    dword ptr [ebp - 0x2c], 0
        je     loc_2_70B4
        cmp    dword ptr [ebp - 0x2c], 0xd
        je     loc_2_70B4
        jmp    loc_2_70E7
loc_2_7117:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6C11
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        dec    word ptr [eax + 0x2e]
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 0x2e]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_7143
        jmp    loc_2_72D3
loc_2_7143:
        mov    word ptr [ebp - 0x18], 0
        mov    word ptr [ebp - 0x14], 0
        jmp    loc_2_7158
loc_2_7154:
        inc    word ptr [ebp - 0x18]
loc_2_7158:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_7198
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ecx + eax*2 + 0x31]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_7193
        mov    word ptr [ebp - 0x14], 1
        jmp    loc_2_7198
loc_2_7193:
        jmp    loc_2_7154
loc_2_7198:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x2a]
        push   eax
        mov    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        imul   eax, dword ptr [0xa50]        ; -> loc_1_0A50
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_2_6C2E
        add    esp, 0xc
        mov    word ptr [ebp - 0x1c], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_71DC
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_722F
loc_2_71DC:
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_7206
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
        jmp    loc_2_71DC
loc_2_7206:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_6D38
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_74E7
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_6B13
        add    esp, 4
        jmp    loc_2_72D3
loc_2_722F:
        mov    word ptr [ebp - 0x18], 0
        jmp    loc_2_723E
loc_2_723A:
        inc    word ptr [ebp - 0x18]
loc_2_723E:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_72B9
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_7261
loc_2_725D:
        inc    word ptr [ebp - 4]
loc_2_7261:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa50]        ; -> loc_1_0A50
        jae    loc_2_72B4
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x11], 0x72d8 ; -> loc_2_72D8
        mov    ax, word ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ecx + eax*2 + 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_6E2B
        add    esp, 0xc
        jmp    loc_2_725D
loc_2_72B4:
        jmp    loc_2_723A
loc_2_72B9:
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_7486
        add    esp, 4
        jmp    loc_2_72D3
loc_2_72D3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_72D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd]
        push   eax
        call   sub_2_6926
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_730E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
        jmp    loc_2_7481
loc_2_730E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_736A
        jmp    loc_2_7326
loc_2_7326:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a]
        and    eax, 0xff
        cmp    eax, 6
        jne    loc_2_7359
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6E2B
        add    esp, 0xc
        jmp    loc_2_7365
loc_2_7359:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
loc_2_7365:
        jmp    loc_2_7481
loc_2_736A:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0xc], eax
loc_2_7373:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xc
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_2_7462
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        cmp    dword ptr [eax + 0x248], 0
        jne    loc_2_745D
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x21]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_2_17B6
        add    esp, 0x10
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_745D
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x34], 0xa0
        push   0x10
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x23
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x4e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x3e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x21]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0x3c], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x21]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1d]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x38]
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_2_65E7
        add    esp, 4
loc_2_745D:
        jmp    loc_2_7373
loc_2_7462:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6E2B
        add    esp, 0xc
        jmp    loc_2_7481
loc_2_7481:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7486:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0xc], eax
loc_2_7498:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_2_74E2
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x21], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_2_6479
        add    esp, 4
        jmp    loc_2_7498
loc_2_74E2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_74E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_2_74F0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_7543
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x21], 1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_2_6479
        add    esp, 4
        jmp    loc_2_74F0
loc_2_7543:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_7550:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x228], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7567:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_7575
loc_2_7575:
        call   sub_2_7BC5
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_759A
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_75FC
loc_2_759A:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x228]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x7601 ; -> loc_2_7601
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x34], 0xf6
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x3c], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x37], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x36], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
loc_2_75FC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_7601:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_7615
loc_2_7615:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x228]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_7655
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x228]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_7650
        mov    word ptr [ebp - 4], 1
loc_2_7650:
        jmp    loc_2_7760
loc_2_7655:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_7747
loc_2_7668:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7567
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x37]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_76F4
loc_2_7687:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2A74
        add    esp, 8
        jmp    loc_2_7717
loc_2_769F:
        push   2
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_61F2
        add    esp, 0xc
        jmp    loc_2_7717
loc_2_76B9:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    byte ptr [eax + 0x3c], 0
        je     loc_2_76E5
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_51AA
        add    esp, 0xc
        jmp    loc_2_76E5
loc_2_76E5:
        jmp    loc_2_7717
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_76F4:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_769F
        cmp    dword ptr [ebp - 0x10], 1
        je     loc_2_7687
        cmp    dword ptr [ebp - 0x10], 0xa
        je     loc_2_76B9
        jmp    loc_2_7717
loc_2_7717:
        jmp    loc_2_7760
loc_2_771C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_7760
loc_2_772F:
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_7760
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_7747:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_7668
        cmp    dword ptr [ebp - 0xc], 1
        je     loc_2_771C
        jmp    loc_2_772F
loc_2_7760:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_778A
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_778A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_778F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x228]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_77B8
        sub    al, al
        jmp    loc_2_77BF
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_77B8:
        mov    al, 1
        jmp    loc_2_77BF
loc_2_77BF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_77C4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x22a], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_77DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_77E9
loc_2_77E9:
        call   sub_2_7BC5
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_780E
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_78B4
loc_2_780E:
        push   0x5ad
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_7849
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_78B4
loc_2_7849:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x22a]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0x78b9 ; -> loc_2_78B9
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x34], 0xf4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x3c], 0x5ad
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x37], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x36], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
loc_2_78B4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_78B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_2_78CD
loc_2_78CD:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x22a]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_2_790D
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22a]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_7908
        mov    word ptr [ebp - 4], 1
loc_2_7908:
        jmp    loc_2_79FA
loc_2_790D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        or     eax, eax
        jne    loc_2_79C5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_77DB
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_79C0
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        or     eax, eax
        je     loc_2_796C
        jmp    loc_2_79C0
loc_2_796C:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_2_797B
loc_2_7977:
        inc    word ptr [ebp - 0xc]
loc_2_797B:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, dword ptr [0xa54]        ; -> loc_1_0A54
        jae    loc_2_79C0
        mov    ax, word ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8620
        add    esp, 0x10
        jmp    loc_2_7977
loc_2_79C0:
        jmp    loc_2_79FA
loc_2_79C5:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_79EC
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    loc_2_79FA
loc_2_79EC:
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
loc_2_79FA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x38]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_7A36
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_7A36:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7A3B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22a]
        and    eax, 0xffff
        or     eax, eax
        je     loc_2_7A64
        sub    al, al
        jmp    loc_2_7A6B
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_7A64:
        mov    al, 1
        jmp    loc_2_7A6B
loc_2_7A6B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7A70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7A95:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7AAA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7AE6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7B16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    loc_2_7B34
        sub    eax, eax
        jmp    loc_2_7B52
loc_2_7B34:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_7B52
loc_2_7B52:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7B57:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7B96:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    loc_2_7BB7
        sub    ax, ax
        jmp    loc_2_7BC0
        db  0E9h, 009h, 000h, 000h, 000h
loc_2_7BB7:
        mov    ax, 1
        jmp    loc_2_7BC0
loc_2_7BC0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7BC5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x74
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_7BEE
        sub    eax, eax
        jmp    loc_2_7C2B
loc_2_7BEE:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        push   0x8a4                         ; -> loc_1_08A4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2c
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x40
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x34
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_7C2B
loc_2_7C2B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0034
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7C4D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xd], al
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7C6B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_7C80:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_7CC4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        test   al, 1
        je     loc_2_7CB7
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_7CCB
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_7CB7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_7C80
loc_2_7CC4:
        sub    eax, eax
        jmp    loc_2_7CCB
loc_2_7CCB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_7CE5:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_7D25
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax + 0x21]
        or     eax, eax
        jne    loc_2_7D18
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_7D2C
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_7D18:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_7CE5
loc_2_7D25:
        sub    eax, eax
        jmp    loc_2_7D2C
loc_2_7D2C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7D31:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_7D5A
        sub    eax, eax
        jmp    loc_2_7DCE
loc_2_7D5A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        mov    dword ptr [ebp - 4], eax
loc_2_7D66:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_7DC7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        push   0xf
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x12
        push   eax
        call   sub_1_052B
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_7DBA
        mov    eax, dword ptr [ebp - 0xc]
        movsx  eax, byte ptr [eax + 0x21]
        cmp    eax, 0x20
        jne    loc_2_7DBA
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_2_7DCE
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_7DBA:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_7D66
loc_2_7DC7:
        sub    eax, eax
        jmp    loc_2_7DCE
loc_2_7DCE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7DD3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x150
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0xffff
        or     eax, eax
        jne    loc_2_7E03
        sub    eax, eax
        jmp    loc_2_7E11
        db  0E9h, 00Eh, 000h, 000h, 000h
loc_2_7E03:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x150]
        jmp    loc_2_7E11
loc_2_7E11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_7E20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        push   0x8b0                         ; -> loc_1_08B0
        push   0x80000002
        call   0xfffe07d0                    ; -> obj1:0x07D0
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_7E95
        mov    dword ptr [ebp - 0xc], 4
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0810
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_7E7E
        cmp    dword ptr [ebp - 4], 3
        je     loc_2_7E84
loc_2_7E7E:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
loc_2_7E84:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_07F3
        add    esp, 4
        jmp    loc_2_7E9B
loc_2_7E95:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
loc_2_7E9B:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    eax, ecx
        ja     loc_2_7EAB
        mov    eax, ecx
loc_2_7EAB:
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    eax, ecx
        jb     loc_2_7EB8
        mov    eax, ecx
loc_2_7EB8:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_7EC3
loc_2_7EC3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_7EC8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0xff
        push   1
        push   0xff
        push   0x8fc                         ; -> loc_1_08FC
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2d8], eax
        push   0xff
        push   1
        push   0xff
        push   0x908                         ; -> obj1:0x0908
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2dc], eax
        push   0x10000
        push   0x400
        push   0x1194
        push   0x914                         ; -> obj1:0x0914
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2e0], eax
        push   0x800
        push   0x80
        push   0x200
        push   0x920                         ; -> loc_1_0920
        call   sub_2_7E20
        add    esp, 0x10
        shl    eax, 0xa
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2e4], eax
        push   2
        push   0
        push   0
        push   0x92c                         ; -> obj1:0x092C
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2e8], eax
        push   1
        push   0
        push   0
        push   0x93c                         ; -> loc_1_093C
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2f0], eax
        push   0xe10
        push   0xa
        push   0x3c
        push   0x94c                         ; -> loc_1_094C
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2f4], eax
        push   1
        push   0
        push   1
        push   0x95c                         ; -> obj1:0x095C
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2f8], eax
        push   0xff
        push   0x10
        push   0x20
        push   0x980                         ; -> loc_1_0980
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2fc], eax
        push   0x20
        push   1
        push   3
        push   0x994                         ; -> loc_1_0994
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x300], eax
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_2_8038
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0xea60
        jb     loc_2_802A
        mov    eax, 0xea60
loc_2_802A:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2ec], eax
        jmp    loc_2_8057
loc_2_8038:
        push   0xea60
        push   0
        push   0
        push   0x9ac                         ; -> loc_1_09AC
        call   sub_2_7E20
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2ec], eax
loc_2_8057:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_805C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   -1
        push   0
        push   5
        push   0x9bc                         ; -> loc_1_09BC
        call   sub_2_7E20
        add    esp, 0x10
        mov    dword ptr [0xa24], eax        ; -> loc_1_0A24
        cmp    dword ptr [0xa24], 0          ; -> loc_1_0A24
        je     loc_2_80A0
        mov    dword ptr [0xa20], 1          ; -> loc_1_0A20
        mov    dword ptr [0xa50], 1          ; -> loc_1_0A50
        jmp    loc_2_80D3
loc_2_80A0:
        push   0xff
        push   1
        push   0xa
        push   0x9d4                         ; -> loc_1_09D4
        call   sub_2_7E20
        add    esp, 0x10
        mov    dword ptr [0xa20], eax        ; -> loc_1_0A20
        push   0x20
        push   1
        push   3
        push   0x9f0                         ; -> obj1:0x09F0
        call   sub_2_7E20
        add    esp, 0x10
        mov    dword ptr [0xa50], eax        ; -> loc_1_0A50
loc_2_80D3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_80E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0xb0
        push   0x10
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 3
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8138:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x75        ; -> obj1:0x0790
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, 0x790                    ; -> obj1:0x0790
        add    eax, 0x1a
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        mov    cl, byte ptr [0x793]          ; -> loc_1_0793
        mov    edx, dword ptr [ebp + 0x10]
        mov    byte ptr [edx], cl
        mov    al, byte ptr [ebp - 4]
        jmp    loc_2_81A1
loc_2_81A1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_81A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x31        ; -> obj1:0x0790
        push   0x10
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, 0x790                    ; -> obj1:0x0790
        add    eax, 0x1a
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_8207
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0xffff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        jmp    loc_2_8215
loc_2_8207:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
loc_2_8215:
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        jmp    loc_2_822D
loc_2_822D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8232:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x90
        push   0x10
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_82B1
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5e], 0x5a
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5f], 0x5a
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        jmp    loc_2_82D8
loc_2_82B1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5e], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5f], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
loc_2_82D8:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_82ED:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x91
        push   0x10
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x4e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        push   0x10
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x3e
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_836C
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5e], 0x5a
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5f], 0x5a
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        jmp    loc_2_8393
loc_2_836C:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5e], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x5f], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
loc_2_8393:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_83A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x12        ; -> obj1:0x0790
        mov    al, byte ptr [ebp + 0x14]
        mov    byte ptr [0x792], al          ; -> obj1:0x0792
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_2_83F7
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        jmp    loc_2_8405
loc_2_83F7:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
loc_2_8405:
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8417:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x34
        push   eax
        call   sub_1_0561
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x92
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x36], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_847A
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 3
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        jmp    loc_2_8493
loc_2_847A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
loc_2_8493:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_84A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x96
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x37], 0xff
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_84EA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x96
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x37], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8536:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x95
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_857D
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        jmp    loc_2_8596
loc_2_857D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
loc_2_8596:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_85AB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0x94
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_85F2
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        jmp    loc_2_860B
loc_2_85F2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
loc_2_860B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x20        ; -> obj1:0x0790
        mov    eax, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax + 0x3c]
        mov    word ptr [0x798], ax          ; -> obj1:0x0798
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [0x794], eax        ; -> obj1:0x0794
        push   0x10
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0x4e
        push   eax
        mov    eax, 0x790                    ; -> obj1:0x0790
        add    eax, 0xa
        push   eax
        call   sub_1_054A
        add    esp, 0xc
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0x793], al          ; -> loc_1_0793
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        jmp    loc_2_86A0
loc_2_86A0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_86A5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x10], 1
        mov    al, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x11], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0xa1
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x3c], 0x5ad
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x37], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 5
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8704:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x22        ; -> obj1:0x0790
        mov    eax, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax + 0x3c]
        mov    word ptr [0x798], ax          ; -> obj1:0x0798
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [0x794], eax        ; -> obj1:0x0794
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0x793], al          ; -> loc_1_0793
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8764:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_2_87A4
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 5
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    al, byte ptr [eax + 0xa2c]    ; -> loc_1_0A2C
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
        jmp    loc_2_87BD
loc_2_87A4:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2b4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x64], al
loc_2_87BD:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x34], 0xa3
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x3c], 0x5ad
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x37], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_87EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x3c
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_8814
        sub    eax, eax
        jmp    loc_2_88A6
loc_2_8814:
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x33        ; -> obj1:0x0790
        mov    word ptr [0x798], 0x3c        ; -> obj1:0x0798
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x794], eax        ; -> obj1:0x0794
        mov    al, byte ptr [ebp + 8]
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        mov    byte ptr [0x79a], 0x2a        ; -> obj1:0x079A
        mov    byte ptr [0x7aa], 0x2a        ; -> obj1:0x07AA
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0034
        add    esp, 4
        movsx  eax, byte ptr [0x7aa]         ; -> obj1:0x07AA
        cmp    eax, 0x2a
        jne    loc_2_8883
        sub    eax, eax
        jmp    loc_2_88A6
loc_2_8883:
        test   byte ptr [0x7ae], 2           ; -> obj1:0x07AE
        je     loc_2_889F
        mov    eax, 1
        jmp    loc_2_88A6
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_889F:
        sub    eax, eax
        jmp    loc_2_88A6
loc_2_88A6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_88AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x3c
        push   0
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_88D7
        mov    eax, 1
        jmp    loc_2_896C
loc_2_88D7:
        push   0x40
        push   0
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0561
        add    esp, 0xc
        mov    byte ptr [0x790], 0x33        ; -> obj1:0x0790
        mov    word ptr [0x798], 0x3c        ; -> obj1:0x0798
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x794], eax        ; -> obj1:0x0794
        mov    al, byte ptr [ebp + 8]
        mov    byte ptr [0x7c0], al          ; -> obj1:0x07C0
        mov    byte ptr [0x79a], 0x2a        ; -> obj1:0x079A
        mov    byte ptr [0x7aa], 0x2a        ; -> obj1:0x07AA
        push   0x790                         ; -> obj1:0x0790
        call   sub_1_0614
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0034
        add    esp, 4
        movsx  eax, byte ptr [0x7aa]         ; -> obj1:0x07AA
        cmp    eax, 0x2a
        jne    loc_2_8949
        mov    eax, 1
        jmp    loc_2_896C
loc_2_8949:
        test   byte ptr [0x7ae], 1           ; -> obj1:0x07AE
        je     loc_2_8965
        mov    eax, 1
        jmp    loc_2_896C
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_8965:
        sub    eax, eax
        jmp    loc_2_896C
loc_2_896C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_8980:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_898E
loc_2_898E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        or     eax, eax
        je     loc_2_89A9
        sub    ax, ax
        jmp    loc_2_8A6A
loc_2_89A9:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2220
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0420
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x174
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5140
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3F70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1090
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3530
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_32A0
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a8
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C20
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7550
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_77C4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_22FC
        add    esp, 4
        mov    ax, 1
        jmp    loc_2_8A6A
loc_2_8A6A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8A6F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_8A7A
loc_2_8A7A:
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x2b4], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_01DD
        add    esp, 4
        mov    ax, 1
        jmp    loc_2_8A9B
loc_2_8A9B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_8AA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_8AAB
loc_2_8AAB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02AB
        add    esp, 4
        mov    ax, 1
        jmp    loc_2_8AC0
loc_2_8AC0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
VxD__LTEXT ENDS
