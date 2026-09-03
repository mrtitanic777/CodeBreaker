; VDD -- reconstructed 32-bit disassembly (VxD / LE format)
; 18 objects, 15645 instructions recovered by recursive descent

Declare_Virtual_Device VDD, 2, 0, VDD_Control, 0000Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h
;   service table: obj1:0x618, 23 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x11a8  R-X
VDD_Control:
        cmp    eax, 0x22
        jne    loc_1_0013
        push   edx
        push   ebx
        push   0x674                         ; -> obj1:0x0674
        call   sub_14_17B8
        stc    
        ret    
loc_1_0013:
        cmp    eax, 0x24
        jae    loc_1_001F
        jmp    dword ptr [eax*4 + 0x21]      ; -> obj1:0x0021
loc_1_001F:
        clc    
        ret    
        ; ---- dispatch table (36 entries) ----
        dd  loc_4_0000
        dd  sub_4_006F
        dd  loc_4_08AE
        dd  loc_10_0043
        dd  loc_1_001F
        dd  loc_9_0000
        dd  loc_9_0002
        dd  loc_10_0000
        dd  loc_10_005F
        dd  sub_10_0050
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_11_0000
        dd  loc_12_0000
        dd  loc_13_0000
        dd  loc_15_0000
        dd  loc_9_00E4
        dd  loc_9_010A
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_11_0036
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_001F
        dd  loc_1_00B1
loc_1_00B1:
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_00BA
        mov    eax, dword ptr [eax]
loc_1_00BA:
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0x18]
        push   eax
        push   dword ptr [esi + 0xc]
        call   sub_14_1831
        clc    
        ret    
        db  000h
VDD_Do_Physical_IO:
        test   ecx, 0x20
        jne    loc_1_011C
        cmp    ecx, 8
        jne    loc_1_00E0
        in     ax, dx
        jmp    loc_1_028B
loc_1_00E0:
        cmp    ecx, 0
        jne    loc_1_00EB
        in     al, dx
        jmp    loc_1_028B
loc_1_00EB:
        cmp    ecx, 0x10
        jne    loc_1_00F6
        in     eax, dx
        jmp    loc_1_028B
loc_1_00F6:
        cmp    ecx, 0xc
        jne    loc_1_0102
        out    dx, ax
        jmp    loc_1_028B
loc_1_0102:
        cmp    ecx, 4
        jne    loc_1_010D
        out    dx, al
        jmp    loc_1_028B
loc_1_010D:
        cmp    ecx, 0x14
        jne    loc_1_028B
        out    dx, eax
        jmp    loc_1_028B
loc_1_011C:
        pushfd 
        cld    
        test   ecx, 0x80
        je     loc_1_01E8
        push   edx
        push   ecx
        shr    ecx, 0x10
        push   0
        push   ecx
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    ecx
        mov    edx, dword ptr [ebp + 4]
        test   ecx, 4
        jne    loc_1_014B
        mov    edx, dword ptr [ebp]
loc_1_014B:
        add    eax, edx
        pop    edx
        mov    esi, eax
        mov    edi, 1
        test   ecx, 0x40
        je     loc_1_0167
        mov    edi, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp + 0x18], 0
loc_1_0167:
        test   ecx, 0x100
        je     loc_1_0172
        neg    edi
        std    
loc_1_0172:
        and    ecx, 0x1c
        cmp    ecx, 0x14
        je     loc_1_01CD
        cmp    ecx, 4
        je     loc_1_01DC
        cmp    ecx, 0xc
        je     loc_1_01BE
        cmp    ecx, 0x10
        je     loc_1_019F
        cmp    ecx, 0
        je     loc_1_01B0
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 1
        add    dword ptr [ebp], esi
        rep insw word ptr es:[edi], dx
        jmp    loc_1_028A
loc_1_019F:
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 2
        add    dword ptr [ebp], esi
        rep insd dword ptr es:[edi], dx
        jmp    loc_1_028A
loc_1_01B0:
        mov    ecx, edi
        xchg   esi, edi
        add    dword ptr [ebp], esi
        rep insb byte ptr es:[edi], dx
        jmp    loc_1_028A
loc_1_01BE:
        mov    ecx, edi
        shl    edi, 1
        add    dword ptr [ebp + 4], edi
        rep outsw dx, word ptr [esi]
        jmp    loc_1_028A
loc_1_01CD:
        mov    ecx, edi
        shl    edi, 2
        add    dword ptr [ebp + 4], edi
        rep outsd dx, dword ptr [esi]
        jmp    loc_1_028A
loc_1_01DC:
        mov    ecx, edi
        add    dword ptr [ebp + 4], edi
        rep outsb dx, byte ptr [esi]
        jmp    loc_1_028A
loc_1_01E8:
        mov    ax, 0x3c04
        test   ecx, 4
        jne    loc_1_01F8
        mov    ax, 0x3800
loc_1_01F8:
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    esi, eax
        mov    edi, 1
        test   ecx, 0x40
        je     loc_1_0217
        movzx  edi, word ptr [ebp + 0x18]
        mov    word ptr [ebp + 0x18], 0
loc_1_0217:
        test   ecx, 0x100
        je     loc_1_0222
        neg    edi
        std    
loc_1_0222:
        and    ecx, 0x1c
        cmp    ecx, 0x14
        je     loc_1_0275
        cmp    ecx, 4
        je     loc_1_0282
        cmp    ecx, 0xc
        je     loc_1_0268
        cmp    ecx, 0x10
        je     loc_1_024D
        cmp    ecx, 0
        je     loc_1_025C
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 1
        add    word ptr [ebp], si
        rep insw word ptr es:[edi], dx
        jmp    loc_1_028A
loc_1_024D:
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 2
        add    word ptr [ebp], si
        rep insd dword ptr es:[edi], dx
        jmp    loc_1_028A
loc_1_025C:
        mov    ecx, edi
        xchg   esi, edi
        add    word ptr [ebp], si
        rep insb byte ptr es:[edi], dx
        jmp    loc_1_028A
loc_1_0268:
        mov    ecx, edi
        shl    edi, 1
        add    word ptr [ebp + 4], di
        rep outsw dx, word ptr [esi]
        jmp    loc_1_028A
loc_1_0275:
        mov    ecx, edi
        shl    edi, 2
        add    word ptr [ebp + 4], di
        rep outsd dx, dword ptr [esi]
        jmp    loc_1_028A
loc_1_0282:
        mov    ecx, edi
        add    word ptr [ebp + 4], di
        rep outsb dx, byte ptr [esi]
loc_1_028A:
        popfd  
loc_1_028B:
        ret    
sub_1_028C:
        push   eax
        push   ebx
        test   byte ptr [edi + 1], 0x20
        jne    loc_1_02E1
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_1_02E4
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jae    loc_1_02B4
        mov    ebx, edi
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_1_02E4
loc_1_02B4:
        mov    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax + 0x449]
        and    al, 0x7f
        cmp    al, 0x13
        ja     loc_1_02E1
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x8c], 0
        je     loc_1_02DD
        popfd  
        call   dword ptr [edi + 0x8c]
        jmp    loc_1_02DE
loc_1_02DD:
        popfd  
loc_1_02DE:
        pop    edi
        jae    loc_1_02E4
loc_1_02E1:
        stc    
        jmp    loc_1_02E5
loc_1_02E4:
        clc    
loc_1_02E5:
        pop    ebx
        pop    eax
        ret    
loc_1_02E8:
        mov    ebx, edx
        mov    eax, 0xbb8
        xor    ecx, ecx
        VMMCall  VMM.Close_VM                    ; cd20ec000100
        ret    
loc_1_02F8:
        inc    word ptr [ebp + 0x24]
        mov    ax, word ptr [ebp + 0x1c]
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    byte ptr [edi + 0x638], 0
        jne    loc_1_035B
        mov    edx, dword ptr [ebp + 0x10]
        shl    edx, 0x10
        mov    dx, ax
        push   edx
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xa4], 0
        je     loc_1_0335
        popfd  
        call   dword ptr [edi + 0xa4]
        jmp    loc_1_0336
loc_1_0335:
        popfd  
loc_1_0336:
        pop    edi
        pop    edx
        jae    loc_1_0341
        call   sub_15_2611
        jmp    loc_1_0355
loc_1_0341:
        cmp    al, 3
        jne    loc_1_035B
        mov    eax, dword ptr [edi + 0x620]
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x1c], 0x4f
loc_1_0355:
        add    word ptr [ebp + 0x24], 9
loc_1_035B:
        ret    
loc_1_035C:
        inc    word ptr [ebp + 0x24]
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    edx, eax
        shl    edx, 0x10
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    dx, ax
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    byte ptr [edi + 0x638], 0
        jne    loc_1_038A
        call   sub_15_2611
loc_1_038A:
        ret    
        db  090h
loc_1_038C:
        call   sub_1_03AB
        jae    loc_1_03A5
        mov    ecx, dword ptr [0xebc]        ; -> obj1:0x0EBC
        mov    edx, dword ptr [0xec0]        ; -> obj1:0x0EC0
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_03A5:
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
sub_1_03AB:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0x40
        je     loc_1_03B5
        stc    
        ret    
loc_1_03B5:
        or     al, al
        je     loc_1_0408
        cmp    al, 3
        je     loc_1_050D
        cmp    al, 4
        je     loc_1_0531
        cmp    al, 7
        je     loc_1_03CF
        stc    
        ret    
loc_1_03CF:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        and    byte ptr [edi + 2], 0xbd
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0501
        xor    eax, eax
        xchg   dword ptr [edi + 0x5dc], eax
        or     eax, eax
        je     loc_1_0501
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_1_0501
loc_1_0408:
        dec    al
        mov    byte ptr [ebp + 0x1c], al
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
sub_1_0415:
        or     byte ptr [edi + 2], 2
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0485
        call   sub_9_00C7
        test   byte ptr [edi + 1], 1
        jne    loc_1_0485
        call   sub_15_29F4
        add    ecx, eax
        shl    ecx, 0xc
        sub    ecx, 0x10
        add    ecx, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    dx, 0x3ce
        in     al, dx
        mov    ah, al
        mov    al, 5
        jmp    loc_1_044A
loc_1_044A:
        out    dx, al
        inc    edx
        jmp    loc_1_044E
loc_1_044E:
        in     al, dx
        push   eax
        and    al, 0xf4
        jmp    loc_1_0454
loc_1_0454:
        out    dx, al
        mov    dx, 0x3c4
        jmp    loc_1_045B
loc_1_045B:
        in     al, dx
        mov    ah, al
        mov    al, 2
        jmp    loc_1_0462
loc_1_0462:
        out    dx, al
        inc    edx
        jmp    loc_1_0466
loc_1_0466:
        in     al, dx
        push   eax
        mov    al, 0xf
        jmp    loc_1_046C
loc_1_046C:
        out    dx, al
        pop    eax
        jmp    loc_1_0470
loc_1_0470:
        out    dx, al
        dec    edx
        mov    al, ah
        jmp    loc_1_0476
loc_1_0476:
        out    dx, al
        mov    dx, 0x3cf
        pop    eax
        jmp    loc_1_047E
loc_1_047E:
        out    dx, al
        dec    edx
        mov    al, ah
        jmp    loc_1_0484
loc_1_0484:
        out    dx, al
loc_1_0485:
        lea    eax, [edi + 0x4af]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_04AD
        mov    eax, dword ptr [edi + 0x5dc]
        or     eax, eax
        jne    loc_1_04B7
        push   0
        push   0x94
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
loc_1_04AD:
        mov    dword ptr [edi + 0x5dc], eax
        or     eax, eax
        je     loc_1_04DF
loc_1_04B7:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_1_04D0
        push   eax
        call   sub_15_1802
        call   sub_15_1969
        call   sub_15_17B4
        pop    eax
loc_1_04D0:
        lea    esi, [edi + 0xc]
        push   edi
        mov    edi, eax
        mov    ecx, 0x25
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
loc_1_04DF:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_04ED
        test   byte ptr [edi + 4], 1
        jmp    loc_1_04F1
loc_1_04ED:
        test   byte ptr [edi + 4], 2
loc_1_04F1:
        je     loc_1_0501
        or     byte ptr [edi + 2], 0x40
        test   byte ptr [ebx], 8
        je     loc_1_0501
        call   sub_15_498E
loc_1_0501:
        call   sub_15_394E
        call   sub_15_4BBC
loc_1_050B:
        clc    
        ret    
loc_1_050D:
        bts    dword ptr [0x6c4], 3          ; -> obj1:0x06C4
        jb     loc_1_0529
        mov    eax, 0x100000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        mov    byte ptr [0xec4], 0           ; -> obj1:0x0EC4
loc_1_0529:
        inc    byte ptr [0xec4]              ; -> obj1:0x0EC4
        jmp    loc_1_050B
loc_1_0531:
        dec    byte ptr [0xec4]              ; -> obj1:0x0EC4
        jne    loc_1_050B
        btr    dword ptr [0x6c4], 3          ; -> obj1:0x06C4
        jae    loc_1_050B
        mov    eax, 0xfff00000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        call   sub_15_0092
        clc    
        ret    
loc_1_0555:
        mov    eax, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    eax, ebx
        inc    word ptr [ebp + 0x24]
        test   byte ptr [eax + 0x5e2], 1
        je     loc_1_056F
        call   sub_1_05C0
        ret    
loc_1_056F:
        inc    word ptr [ebp + 0x24]
        ret    
loc_1_0574:
        movzx  eax, word ptr [ebp + 4]
        or     eax, eax
        je     loc_1_058D
        mov    ax, 0x400
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        jne    loc_1_058D
        xor    eax, eax
loc_1_058D:
        mov    edx, dword ptr [ebp + 0x10]
        cmp    dx, 1
        jne    loc_1_05A9
        movzx  edx, dx
        mov    dword ptr [0xefc], eax        ; -> loc_1_0EFC
        mov    cx, word ptr [ebp + 0x18]
        mov    word ptr [0xfb2], cx          ; -> loc_1_0FB2
loc_1_05A9:
        push   eax
        push   edx
        call   sub_14_014E
        pop    ecx
        pop    ecx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_05C0:
        cmp    dword ptr [0x10a4], -1        ; -> obj1:0x10A4
        jne    loc_1_05CD
        sub    eax, eax
        jmp    loc_1_05F7
loc_1_05CD:
        cmp    dword ptr [0x10a0], 0         ; -> obj1:0x10A0
        mov    eax, 1
        jne    loc_1_05F7
        push   0
        push   0
        push   0                             ; -> loc_5_0000
        call   sub_14_1848
        add    esp, 0xc
        mov    dword ptr [0x10a0], eax       ; -> obj1:0x10A0
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_05F7:
        ret    
sub_1_05F8:
        mov    eax, dword ptr [0x10a0]       ; -> obj1:0x10A0
        or     eax, eax
        je     loc_1_0614
        push   eax
        call   sub_14_1858
        add    esp, 4
        mov    dword ptr [0x10a0], 0         ; -> obj1:0x10A0
loc_1_0614:
        ret    
        db  000h, 000h, 000h
        dd  VDD_Get_Version          ; fixup
        dd  VDD_PIF_State            ; fixup
        dd  VDD_Get_GrabRtn          ; fixup
        dd  VDD_Hide_Cursor          ; fixup
        dd  VDD_Set_VMType           ; fixup
        dd  VDD_Get_ModTime          ; fixup
        dd  VDD_Set_HCurTrk          ; fixup
        dd  VDD_Msg_ClrScrn          ; fixup
        dd  VDD_Msg_ForColor         ; fixup
        dd  VDD_Msg_BakColor         ; fixup
        dd  VDD_Msg_TextOut          ; fixup
        dd  VDD_Msg_SetCursPos       ; fixup
        dd  VDD_Query_Access         ; fixup
        dd  VDD_Check_Update_Soon    ; fixup
        dd  VDD_Get_Mini_Dispatch_Table ; fixup
        dd  VDD_Register_Virtual_Port ; fixup
        dd  VDD_Get_VM_Info          ; fixup
        dd  VDD_Get_Special_VM_IDs   ; fixup
        dd  VDD_Register_Extra_Screen_Selector ; fixup
        dd  VDD_Takeover_VGA_Port    ; fixup
        dd  VDD_Get_DISPLAYINFO      ; fixup
        dd  VDD_Do_Physical_IO       ; fixup
        dd  VDD_Set_Sleep_Flag_Addr  ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Ah                      ; DDB_Req_Device_Number
        db  2, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VDD     '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  VDD_Control              ; DDB_Control_Proc
        dd  VDD_PM_API               ; DDB_V86_API_Proc
        dd  VDD_PM_API               ; DDB_PM_API_Proc
        dd  obj1:0x0618              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0798:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_079C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07A0:
        add    byte ptr [eax], al
loc_1_07A2:
        add    byte ptr [eax], al
loc_1_07A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_085C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E98:
        add    byte ptr [edx], al
loc_1_0E9A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E9E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EA4:
        mov    al, byte ptr [0xb010a020]
        or     byte ptr [eax + 8], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EC5:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0ED2:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0ED6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EDA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EEC:
        add    byte ptr [eax], al
loc_1_0EEE:
        add    byte ptr [eax], al
loc_1_0EF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F00:
        add    byte ptr [eax], al
loc_1_0F02:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FB0:
        add    byte ptr [eax], al
loc_1_0FB2:
        add    byte ptr [eax], al
loc_1_0FB4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  081h, 000h, 000h, 000h
sub_1_0FBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0FC0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0FC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FD4:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0FD8:
        add    byte ptr fs:[eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        db  00Fh, 00Fh, 01Fh, 003h, 01Bh, 00Eh, 00Fh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1014:
        ret    
        db  003h, 030h, 000h
        ; ---- dispatch table (33 entries) ----
        dd  loc_15_4D50
        dd  loc_15_4D74
        dd  loc_15_4DC0
        dd  loc_15_4D4C
        dd  loc_15_4E24
        dd  loc_15_4E54
        dd  loc_15_4F28
        dd  loc_15_4F74
        dd  loc_15_4FF8
        dd  loc_15_5088
        dd  loc_15_5140
        dd  loc_15_4D4C
        dd  loc_15_514C
        dd  loc_15_4D4C
        dd  loc_15_5160
        dd  loc_15_51B8
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_530C
        dd  loc_15_5320
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_53B8
        dd  loc_15_5410
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D4C
        dd  loc_15_4D1D
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xba  R-X
        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        pop    edi
        add    byte ptr [esi + 0x43000000], ah
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_003B
        jb     loc_2_0092
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x99
        and    byte ptr [edx + 0x65], ah
        arpl   word ptr [ecx + 0x75], sp
        jae    0x93
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [esi + 0x69], dh
        outsd  dx, dword ptr gs:[esi]
        and    byte ptr [ebp + 0x76], ah
loc_2_003B:
        imul   esp, dword ptr [ebx + 0x65], 0x6e6f6320
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x6f59202e
        jne    loc_2_006D
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0xcc
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x65], dl
        je     0xd6
        jo     loc_2_0083
        jo     0xd7
        outsd  dx, dword ptr [esi]
        jb     0xca
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x67], ah
loc_2_006D:
        popal  
        imul   ebp, dword ptr [esi + 0x2e], 0x69560000
        outsd  dx, dword ptr gs:[esi]
        and    byte ptr [ecx + 0x6e], ch
        imul   esi, dword ptr [ecx + ebp*2 + 0x61], 0x617a696c
loc_2_0083:
        je     0xee
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x6f59202e
loc_2_0092:
        jne    loc_2_00B4
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0x113
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x65], dl
        je     0x11d
        jo     0xca
        jo     0x11e
        outsd  dx, dword ptr [esi]
        jb     0x111
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x67], ah
loc_2_00B4:
        popal  
        db  069h, 06Eh, 02Eh, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2e5  R-X
        db  005h, 000h, 000h, 000h
loc_3_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
loc_3_000E:
        xchg   ecx, eax
        add    byte ptr [ebx + 1], ah
loc_3_0012:
        and    al, 2
loc_3_0014:
        nop    
        add    cl, ch
        add    dl, byte ptr [eax + ebp*2 + 0x65]
        jb     0x82
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [esi + 0x6f], ch
        je     0x45
        outsb  dx, byte ptr gs:[esi]
        outsd  dx, dword ptr [esi]
        jne    loc_3_0091
        push   0x61766120
        imul   ebp, dword ptr [ecx + 0x62], 0x6d20656c
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0xb5
        and    byte ptr [esi + 0x6f], ah
        jb     0x61
        je     0xab
        imul   esi, dword ptr [ebx + 0x20], 0x6c707061
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + ebp*2 + 0x20], dh
        arpl   word ptr [edi + 0x72], bp
        jb     0xbf
        arpl   word ptr [esp + ebp*2 + 0x79], si
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        jo     0xd0
        popal  
        jns    0x87
        imul   ebp, dword ptr [esi + 0x66], 0x616d726f
        je     0xd9
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[ebp + 0x61], cl
        imul   esp, dword ptr [ebp + 0x20], 0x73
        jne    loc_3_00EE
        and    byte ptr gs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ecx + 0x70], ah
        jo     loc_3_00F2
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        daa    
        jae    0xb0
        push   eax
loc_3_0091:
        jb     0x102
        jo     loc_3_00FA
        jb     loc_3_010B
        imul   esp, dword ptr [ebp + 0x73], 0x65726120
        and    byte ptr [ebx + 0x6f], ah
        jb     0x115
        arpl   word ptr gs:[esi + ebp], si
        add    byte ptr [eax + ebp*2 + 0x69], dl
        jae    0xce
        popal  
        jo     loc_3_0121
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x61], dh
        jae    loc_3_00DE
        jne    0x12e
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_3_0136
        and    byte ptr [ecx + 0x63], ah
        arpl   word ptr [ebp + 0x73], sp
        jae    0xef
        je     0x139
        and    byte ptr gs:[esi + 0x69], dh
        outsd  dx, dword ptr gs:[esi]
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        jo     0x14a
loc_3_00DE:
        popal  
        jns    0x10f
        and    byte ptr [ecx + 0x74], cl
        jae    0x106
        imul   esi, dword ptr fs:[ebx + 0x70], 0x2079616c
loc_3_00EE:
        ja     0x151
        jae    0x112
loc_3_00F2:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x116
        jne    loc_3_0168
        popal  
loc_3_00FA:
        je     loc_3_0161
        and    byte ptr cs:[ebx + 0x68], al
        arpl   word ptr gs:[ebx + 0x20], bp
        je     loc_3_016F
        and    byte ptr gs:[eax + 0x72], dl
loc_3_010B:
        outsd  dx, dword ptr [esi]
        jo     0x173
        jb     loc_3_0184
        imul   esp, dword ptr [ebp + 0x73], 0x206f7420
        outsb  dx, byte ptr gs:[esi]
        jae    0x190
        jb     loc_3_0182
        and    byte ptr [eax + ebp*2 + 0x65], dh
loc_3_0121:
        jns    0x143
        popal  
        jb     0x18b
        and    byte ptr [ebx + 0x6f], ah
        jb     0x19d
        arpl   word ptr gs:[esi + ebp + 0x20], si
        dec    ecx
        and    byte ptr [eax + ebp*2 + 0x65], dh
loc_3_0136:
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        jo     0x1a8
        popal  
        jns    loc_3_015F
        imul   esi, dword ptr [ebx + 0x20], 0x61727265
        je     loc_3_01B1
        arpl   word ptr [eax], bp
        jb     0x1b2
        jb     loc_3_01B1
        ja     0x172
        je     0x1bc
        and    byte ptr gs:[ecx + ebp*2 + 0x73], ah
        jo     0x1c7
        popal  
        jns    0x17e
        outsd  dx, dword ptr [esi]
loc_3_015F:
        jb     0x181
loc_3_0161:
        jb     loc_3_01C8
        jae    0x1d9
        popal  
        jb     loc_3_01DC
loc_3_0168:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ecx + 0x70], ah
loc_3_016F:
        jo     0x1dd
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[eax], al
        pop    ecx
        outsd  dx, dword ptr [esi]
        jne    0x19f
        arpl   word ptr [ecx + 0x6e], sp
loc_3_0182:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
loc_3_0184:
        je     loc_3_01A6
        jb     0x1fd
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0x1af
        jb     0x1f3
        jo     0x1fc
        imul   esp, dword ptr [ebx + 0x73], 0x70706120
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x69], dh
loc_3_01A6:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_3_0210
        and    byte ptr cs:[eax + ebp*2 + 0x65], dl
loc_3_01B1:
        and    byte ptr [ecx + 0x70], ah
        jo     loc_3_0222
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [ebx + 0x75], dh
loc_3_01C8:
        jae    0x23a
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr fs:[ebp + 0x6e], dh
        je     0x23d
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x6f], bh
        jne    0x1fa
        jae    0x253
loc_3_01DC:
        imul   esi, dword ptr [ebx + 0x68], 0x20746920
        je     0x255
        and    byte ptr [esi + 0x75], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x63], dh
        jb     0x255
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [eax + 0x72], dh
        jae    0x26e
        imul   ebp, dword ptr [esi + 0x67], 0x746c4120
        sub    eax, 0x65746e45
        jb     0x237
        and    byte ptr [ebx + 0x68], al
        arpl   word ptr gs:[ebx + 0x20], bp
loc_3_0210:
        je     0x27a
        and    byte ptr gs:[eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        jo     loc_3_027E
        jb     0x28f
        imul   esp, dword ptr [ebp + 0x73], 0x206f7420
loc_3_0222:
        outsb  dx, byte ptr gs:[esi]
        jae    0x29b
        jb     0x28d
        and    byte ptr [eax + ebp*2 + 0x65], dh
        jns    0x24e
        popal  
        jb     0x296
        and    byte ptr [ebx + 0x6f], ah
        jb     loc_3_02A8
        arpl   word ptr gs:[esi + ebp], si
        add    byte ptr [eax + ebp*2 + 0x69], dl
        jae    0x261
        popal  
        jo     0x2b4
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [ebx + 0x61], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x274
        bound  esp, qword ptr [ebp + 0x20]
        jb     loc_3_02CE
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ecx + 0x20], ah
        ja     0x2cb
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_3_0295
        and    byte ptr [eax + 0x72], dl
        jae    0x2e0
        and    byte ptr [ecx + 0x6e], ah
        jns    0x292
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x2e7
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        jbe    loc_3_02E3
loc_3_027E:
        jb     0x2f4
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0x2a6
        popal  
        jo     0x2f9
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + ebp*2 + 0x20], dh
loc_3_0295:
        jne    0x304
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x63], dh
        jb     0x303
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ebp + 0x6f], ch
        add    byte ptr cs:[eax], al
loc_3_02A8:
        push   esp
        push   0x61207369
        jo     0x320
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [ebx + 0x61], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x2e0
        bound  esp, qword ptr [ebp + 0x20]
        jb     0x32a
        jae    0x33b
        outsd  dx, dword ptr [esi]
        jb     0x32f
        and    byte ptr fs:[ecx + 0x6e], ah
loc_3_02CE:
        and    byte ptr fs:[edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [ebp + 0x72], dh
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x61], 0x2e646574
loc_3_02E3:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xed4  R-X
loc_4_0000:
        mov    dword ptr [0xef0], ebx        ; -> loc_1_0EF0
        mov    dword ptr [0xed2], 0x24       ; -> loc_1_0ED2
        mov    dword ptr [0x6c8], 0          ; -> obj1:0x06C8
        mov    dword ptr [0x6c4], 0          ; -> obj1:0x06C4
        push   0
        push   0x63c
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        test   eax, eax
        jne    loc_4_003E
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_003E:
        mov    dword ptr [0x6cc], eax        ; -> obj1:0x06CC
        mov    edi, ebx
        add    edi, eax
        mov    eax, dword ptr [0x6c8]        ; -> obj1:0x06C8
        mov    dword ptr [edi + 4], eax
        call   sub_4_012C
        call   sub_4_054C
        call   sub_4_0C20
        jb     loc_4_0112
        mov    dword ptr [0xed2], 0x10       ; -> loc_1_0ED2
        ret    
sub_4_006F:
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_4_0079
        ret    
loc_4_0079:
        pushal 
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    ch, 0
        mov    edx, 0xa
        call   sub_9_00B4
        call   sub_4_03CC
        jb     loc_4_0112
        call   sub_4_0628
        jb     loc_4_0112
        call   sub_4_0C22
        jb     loc_4_0112
        call   sub_4_0150
        mov    eax, 0x17001a
        mov    esi, 0x8b5                    ; -> loc_15_08B5
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_4_0112
        call   sub_4_0BC4
        push   0
        push   0xcd0                         ; -> obj4:0x0CD0
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        test   eax, eax
        je     loc_4_0112
        push   0
        push   0xce4                         ; -> obj4:0x0CE4
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        test   eax, eax
        je     loc_4_0112
        push   0
        push   0xcf8                         ; -> obj4:0x0CF8
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        test   eax, eax
        je     loc_4_0112
        push   0
        push   0xd0c                         ; -> obj4:0x0D0C
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        test   eax, eax
        je     loc_4_0112
        clc    
        popal  
        ret    
loc_4_0112:
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, ecx
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
        add    byte ptr [eax], al
sub_4_012C:
        mov    dword ptr [0x7a4], ebx        ; -> loc_1_07A4
        mov    dword ptr [0x7a8], ebx        ; -> loc_1_07A8
        mov    dword ptr [0x7ac], ebx        ; -> loc_1_07AC
        mov    eax, 0x7bc                    ; -> loc_1_07BC
        sub    eax, dword ptr [0x6cc]        ; -> obj1:0x06CC
        mov    dword ptr [0x7b8], eax        ; -> loc_1_07B8
        clc    
        ret    
sub_4_0150:
        call   sub_10_0070
        jb     loc_4_0175
        call   sub_15_1EAC
        call   sub_4_0176
        call   sub_4_01B6
        jb     loc_4_0175
        call   sub_4_0211
        jb     loc_4_0175
        call   sub_15_4BBC
        clc    
loc_4_0175:
        ret    
sub_4_0176:
        push   edi
        mov    edi, 0xd8c                    ; -> loc_4_0D8C
        mov    esi, 0xd98                    ; -> obj4:0x0D98
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        jb     loc_4_018B
        jne    loc_4_0193
loc_4_018B:
        movzx  eax, byte ptr [0x484]
        inc    eax
loc_4_0193:
        cmp    al, 0x32
        jae    loc_4_01A4
        cmp    al, 0x2b
        jb     loc_4_01AD
        mov    byte ptr [0xe98], 0x2b        ; -> loc_1_0E98
        jmp    loc_4_01B4
loc_4_01A4:
        mov    byte ptr [0xe98], 0x32        ; -> loc_1_0E98
        jmp    loc_4_01B4
loc_4_01AD:
        mov    byte ptr [0xe98], 0x19        ; -> loc_1_0E98
loc_4_01B4:
        pop    edi
        ret    
sub_4_01B6:
        mov    dh, 0xa0
        mov    dl, 0x10
loc_4_01BA:
        movzx  eax, dh
        call   sub_15_3E18
        inc    dh
        dec    dl
        jne    loc_4_01BA
        mov    eax, 0x10
        call   sub_4_031A
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xb8], 0
        je     loc_4_01E7
        call   dword ptr [edi + 0xb8]
loc_4_01E7:
        pop    edi
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [0xdfc], esi        ; -> loc_1_0DFC
        push   edi
        lea    esi, [edi + 0xc]
        mov    edi, 0xe04                    ; -> loc_1_0E04
        mov    ecx, 0x25
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        or     byte ptr [0x6c5], 1           ; -> obj1:0x06C5
        call   sub_15_3E44
        clc    
        ret    
sub_4_0211:
        call   sub_4_08C4
        jb     loc_4_0319
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 2
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0x1202
        mov    byte ptr [ebp + 0x10], 0x30
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 3
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 1
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        call   sub_15_16C3
        jb     loc_4_0319
        imul   eax, ecx
        shl    eax, 1
        add    eax, 0xfff
        shr    eax, 0xc
        cmp    eax, 2
        jae    loc_4_02B1
        mov    al, 2
loc_4_02B1:
        mov    byte ptr [0xe99], al          ; -> obj1:0x0E99
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xb4], 0
        je     loc_4_02CB
        call   dword ptr [edi + 0xb4]
loc_4_02CB:
        pop    edi
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [0x7c0], esi        ; -> loc_1_07C0
        push   edi
        lea    esi, [edi + 0xc]
        mov    edi, 0x7c8                    ; -> loc_1_07C8
        mov    ecx, 0x25
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, dword ptr [esp]
        push   ebx
        lea    esi, [edi + 0xa0]
        lea    edx, [edi + 0x50]
        mov    edi, 0x85c                    ; -> loc_1_085C
        mov    ecx, 0x10
        xor    bl, bl
loc_4_02FF:
        movzx  ebx, byte ptr [edx]
        mov    eax, dword ptr [esi + ebx*4]
        stosd  dword ptr es:[edi], eax
        inc    edx
        loop   loc_4_02FF
        pop    ebx
        pop    edi
        call   sub_4_0935
        jb     loc_4_0319
        or     byte ptr [0x6c4], 1           ; -> obj1:0x06C4
loc_4_0319:
        ret    
sub_4_031A:
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        push   eax
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 2
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    eax
        mov    word ptr [ebp + 0x1c], ax
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 1
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
VDD_Get_Special_VM_IDs:
        mov    esi, 0xdf8                    ; -> loc_1_0DF8
        mov    edi, dword ptr [0x7b8]        ; -> loc_1_07B8
        ret    
        db  000h
loc_4_0390:
        cmp    eax, 0x30740e07
        cmp    ah, 0x4f
        je     loc_4_03B9
        or     ah, ah
        je     loc_4_03B0
        cmp    ah, 5
        je     loc_4_03B0
        cmp    ah, 0x11
        jne    0x3c7
        cmp    al, 3
        je     0x3c7
        cmp    al, 0x30
        je     0x3c7
loc_4_03B0:
        nop    
        pushfd 
        lcall  0xcf90, 0
loc_4_03B9:
        nop    
        push   eax
        push   ebx
        pushfd 
        lcall  0x9090, 0
        iretd  
        db  090h, 0CFh, 0EAh, 000h, 000h, 000h, 000h
sub_4_03CC:
        mov    eax, 0x10
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        shl    ecx, 0x10
        mov    cx, dx
        mov    dword ptr [0xeac], ecx        ; -> obj1:0x0EAC
        push   ecx
        push   0
        push   0x3c
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        pop    edx
        or     eax, eax
        jne    loc_4_03FF
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_03FF:
        push   edi
        mov    edi, eax
        mov    esi, 0x390                    ; -> loc_4_0390
        mov    ecx, 0x3c
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, eax
        mov    dword ptr [edi + 0x23], edx
        mov    dword ptr [edi + 0x38], edx
        mov    dword ptr [edi + 0x2e], edx
        shr    eax, 4
        movzx  ecx, ax
        mov    edx, edi
        and    edx, 0xf
        mov    eax, 0x10
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
        mov    eax, ecx
        shl    eax, 0x10
        add    eax, edx
        mov    dword ptr [0xeb0], eax        ; -> obj1:0x0EB0
        add    eax, 0x20
        xor    edx, edx
        mov    esi, 0x2753                   ; -> loc_15_2753
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        add    eax, 7
        mov    esi, 0x2841                   ; -> loc_15_2841
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    eax, dword ptr [0xeb0]        ; -> obj1:0x0EB0
        add    eax, 0x29
        xor    edx, edx
        mov    esi, 0x2f8                    ; -> loc_1_02F8
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        add    eax, 9
        mov    esi, 0x35c                    ; -> loc_1_035C
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    eax, dword ptr [0xeb0]        ; -> obj1:0x0EB0
        add    eax, 0x35
        xor    edx, edx
        mov    esi, 0x555                    ; -> loc_1_0555
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        pop    edi
        mov    esi, 0x2174                   ; -> loc_15_2174
        mov    eax, 0x10
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    esi, 0x20f2                   ; -> loc_15_20F2
        mov    eax, 0x16
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    esi, 0x20d9                   ; -> loc_15_20D9
        mov    eax, 9
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x10
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0xeb4], ecx        ; -> obj1:0x0EB4
        mov    dword ptr [0xeb8], edx        ; -> obj1:0x0EB8
        mov    esi, 0x288f                   ; -> loc_15_288F
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x10
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 0x3ab                    ; -> sub_1_03AB
        mov    eax, 0x2f
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x2f
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0xebc], ecx        ; -> obj1:0x0EBC
        mov    dword ptr [0xec0], edx        ; -> obj1:0x0EC0
        mov    esi, 0x38c                    ; -> loc_1_038C
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x2f
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        clc    
        ret    
        db  000h, 000h, 000h
sub_4_054C:
        push   0
        push   0x20000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        jne    loc_4_0566
        jmp    loc_4_059F
loc_4_0566:
        dec    eax
        mov    dword ptr [0xec8], eax        ; -> obj1:0x0EC8
        mov    eax, dword ptr [0xed2]        ; -> loc_1_0ED2
        shl    eax, 3
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_4_05B7
        mov    dword ptr [0xed6], eax        ; -> loc_1_0ED6
        xor    eax, eax
        mov    ecx, 0x10
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_4_05B7
        mov    dword ptr [0xeda], esi        ; -> loc_1_0EDA
        ret    
loc_4_059F:
        movzx  ecx, word ptr [0xc]           ; -> loc_2_000C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, ecx
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_05B7:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
        add    byte ptr [eax], al
        db  000h
VDD_Get_Mini_Dispatch_Table:
        mov    ecx, 0x31
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        ret    
VDD_Register_Virtual_Port:
        cmp    edx, 0x3ce
        jne    loc_4_05DC
        or     dword ptr [0x109c], 1         ; -> obj1:0x109C
        jmp    loc_4_0608
loc_4_05DC:
        mov    esi, 0x6c2                    ; -> loc_15_06C2
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_0608
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        cmp    cl, 2
        jne    loc_4_0608
        inc    edx
        mov    esi, 0x6c2                    ; -> loc_15_06C2
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_0608
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_4_0608:
        ret    
VDD_Takeover_VGA_Port:
        cmp    edx, 0x3df
        ja     loc_4_0623
        sub    edx, 0x3c0
        js     loc_4_0623
        xchg   dword ptr [edx*4 + 0x1018], ecx ; -> obj1:0x1018
        clc    
        jmp    loc_4_0624
loc_4_0623:
        stc    
loc_4_0624:
        ret    
        db  000h, 000h, 000h
sub_4_0628:
        mov    eax, dword ptr [0xed2]        ; -> loc_1_0ED2
        call   sub_15_347C
        or     byte ptr [edi + 4], 1
        xor    eax, eax
        xor    esi, esi
        push   edi
        mov    edi, 0xd78                    ; -> loc_4_0D78
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        or     eax, eax
        je     loc_4_064F
        or     byte ptr [edi + 4], 0x80
loc_4_064F:
        or     eax, 0xffffffff
        xor    esi, esi
        push   edi
        mov    edi, 0xd65                    ; -> loc_4_0D65
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        or     eax, eax
        jne    loc_4_0669
        or     byte ptr [edi + 4], 0x40
loc_4_0669:
        or     eax, 0xffffffff
        xor    esi, esi
        push   edi
        mov    edi, 0xdce                    ; -> loc_4_0DCE
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        or     eax, eax
        jne    loc_4_0683
        or     byte ptr [edi + 4], 2
loc_4_0683:
        VxDCall  VDD2.VDD2_Get_Version           ; cd2000001c00
        jae    loc_4_0739
        xor    esi, esi
        push   edi
        mov    edi, 0xda6                    ; -> obj4:0x0DA6
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        jb     loc_4_06B0
        je     loc_4_06B0
        or     eax, eax
        je     loc_4_06FF
loc_4_06A6:
        and    byte ptr [edi + 4], 0xfb
        or     byte ptr [edi + 4], 0x20
        jmp    loc_4_0721
loc_4_06B0:
        mov    dx, 0x3ce
        in     al, dx
        mov    ah, al
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        push   eax
        or     al, 0xc
        out    dx, al
        mov    esi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        add    esi, 0x10000
        mov    ax, word ptr [esi]
        mov    ecx, eax
        xor    cx, 0x5555
        mov    word ptr [esi], cx
        xchg   word ptr [esi], ax
        cmp    eax, ecx
        jne    loc_4_06F5
        add    esi, 2
        mov    ax, word ptr [esi]
        mov    ecx, eax
        xor    cx, 0xaaaa
        mov    word ptr [esi], cx
        xchg   word ptr [esi], ax
        cmp    eax, ecx
loc_4_06F5:
        pop    eax
        pushfd 
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        popfd  
        je     loc_4_06A6
loc_4_06FF:
        xor    esi, esi
        push   edi
        or     eax, 0xffffffff
        mov    edi, 0xdb2                    ; -> loc_4_0DB2
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        or     byte ptr [edi + 4], 4
        or     eax, eax
        jne    loc_4_0721
        jmp    loc_4_0739
        db  0F6h, 047h, 004h, 004h, 074h, 01Ch
loc_4_0721:
        xor    eax, eax
        push   eax
        push   eax
        push   8
        push   0xb0
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_073D
loc_4_0739:
        and    byte ptr [edi + 4], 0xdb
loc_4_073D:
        xor    eax, eax
        push   eax
        push   0xd20                         ; -> obj4:0x0D20
        push   eax
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        mov    eax, dword ptr [0xd34]        ; -> obj4:0x0D34
        test   eax, 0xff00ffff
        je     loc_4_075D
        jmp    loc_4_078F
loc_4_075D:
        xor    eax, eax
        push   eax
        push   eax
        push   0x10
        push   0xa0
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0777
        jmp    loc_4_078F
loc_4_0777:
        xor    eax, eax
        push   eax
        push   eax
        push   8
        push   0xb8
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_07A9
loc_4_078F:
        movzx  ecx, word ptr [0xc]           ; -> loc_2_000C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, ecx
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
        stc    
        ret    
loc_4_07A9:
        push   edi
        xor    esi, esi
        mov    edi, 0xdbe                    ; -> loc_4_0DBE
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        jb     loc_4_07C8
        je     loc_4_07C8
        and    byte ptr [edi + 7], 0x7f
        or     eax, eax
        je     loc_4_07C8
        or     byte ptr [edi + 7], 0x80
loc_4_07C8:
        test   byte ptr [edi + 7], 0x80
        je     loc_4_0857
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x1130
        mov    byte ptr [ebp + 0x11], 2
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ecx, word ptr [ebp + 0x38]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    ch, 0xc0
        jb     loc_4_0857
        test   cx, 0xfff
        jne    loc_4_0857
        shr    ecx, 8
        mov    eax, ecx
        shr    eax, 5
        shl    eax, 2
        mov    eax, dword ptr [eax + 0xd20]  ; -> obj4:0x0D20
        test   al, 0xc0
        jne    loc_4_0853
        xor    eax, eax
        add    ecx, 6
        mov    dword ptr [0xfb4], ecx        ; -> loc_1_0FB4
        push   eax
        push   eax
        push   2
        push   ecx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0857
loc_4_0853:
        and    byte ptr [edi + 7], 0x7f
loc_4_0857:
        mov    esi, 0x3abb                   ; -> loc_15_3ABB
        mov    eax, 0xa0
        mov    ecx, 0x10
loc_4_0866:
        VMMCall  VMM.Hook_V86_Page               ; cd2071000100
        inc    eax
        loop   loc_4_0866
        mov    eax, 0xb8
        mov    ecx, 8
loc_4_0879:
        VMMCall  VMM.Hook_V86_Page               ; cd2071000100
        inc    eax
        loop   loc_4_0879
        test   byte ptr [edi + 4], 4
        je     loc_4_089B
        mov    eax, 0xb0
        mov    ecx, 8
loc_4_0892:
        VMMCall  VMM.Hook_V86_Page               ; cd2071000100
        inc    eax
        loop   loc_4_0892
loc_4_089B:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [0x6c8], eax        ; -> obj1:0x06C8
        call   sub_10_0238
        call   sub_15_3509
        ret    
loc_4_08AE:
        mov    dword ptr [0xfb8], 0x101      ; -> obj1:0x0FB8
        call   sub_4_0CC4
        jmp    loc_4_0B83
        db  000h, 000h
sub_4_08C4:
        mov    dx, 0xa010
loc_4_08C8:
        movzx  eax, dh
        call   sub_15_3E18
        inc    dh
        dec    dl
        jne    loc_4_08C8
        xor    eax, eax
        push   9
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   2
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_4_0911
        mov    dword ptr [0xd44], eax        ; -> obj4:0x0D44
        mov    dword ptr [0xd48], edx        ; -> obj4:0x0D48
        push   0
        push   0
        push   2
        push   0xa0
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
loc_4_0911:
        mov    dh, 0xb8
        mov    dl, 8
        test   byte ptr [edi + 4], 4
        je     loc_4_091F
        mov    dh, 0xb0
        mov    dl, 0x10
loc_4_091F:
        movzx  eax, dh
        call   sub_15_3E18
        inc    dh
        dec    dl
        jne    loc_4_091F
        clc    
        ret    
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
sub_4_0935:
        movzx  eax, byte ptr [0x484]
        inc    eax
        mov    dword ptr [0xee4], eax        ; -> loc_1_0EE4
        movzx  eax, byte ptr [0x44a]
        mov    dword ptr [0xee8], eax        ; -> loc_1_0EE8
        movzx  ecx, byte ptr [edi + 0x15]
        and    cl, 0x1f
        inc    ecx
        mov    byte ptr [0xfd0], cl          ; -> loc_1_0FD0
        shl    ecx, 8
        mov    dword ptr [0xd4c], ecx        ; -> obj4:0x0D4C
        cmp    dword ptr [0xd44], 0          ; -> obj4:0x0D44
        je     loc_4_0995
        push   edi
        xor    esi, esi
        mov    edi, 0xdf0                    ; -> loc_4_0DF0
        or     eax, 0xffffffff
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        jbe    loc_4_0988
        or     al, al
        je     loc_4_0988
        mov    al, 1
loc_4_0988:
        mov    byte ptr [0xd50], al          ; -> obj4:0x0D50
        or     eax, eax
        je     loc_4_0AE6
loc_4_0995:
        push   edi
        movzx  ecx, byte ptr [0xfd0]         ; -> loc_1_0FD0
        push   ecx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    al, 6
        mov    cl, 0x10
        cmp    byte ptr [0xfd0], cl          ; -> loc_1_0FD0
        jae    loc_4_09D0
        mov    cl, 8
        mov    al, 3
        cmp    byte ptr [0xfd0], 0xe         ; -> loc_1_0FD0
        jb     loc_4_09D0
        mov    al, 2
        mov    cl, 0xe
loc_4_09D0:
        xchg   byte ptr [0xfd0], cl          ; -> loc_1_0FD0
        cmp    byte ptr [0xfd0], cl          ; -> loc_1_0FD0
        jbe    loc_4_09EB
        cmp    dword ptr [0xd44], 0          ; -> obj4:0x0D44
        jne    loc_4_0AC9
loc_4_09EB:
        push   eax
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 2
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    eax
        mov    byte ptr [ebp + 0x11], al
        mov    word ptr [ebp + 0x1c], 0x1130
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  esi, word ptr [ebp + 0x38]
        movzx  ecx, word ptr [ebp + 8]
        shl    esi, 4
        add    esi, ecx
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 1
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    byte ptr [0xd50], 1           ; -> obj4:0x0D50
        je     loc_4_0A79
        xor    eax, eax
        push   eax
        push   0xd20                         ; -> obj4:0x0D20
        push   eax
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        mov    ecx, esi
        shr    ecx, 0xc
        bt     dword ptr [0xd20], ecx        ; -> obj4:0x0D20
        jae    loc_4_0A79
        pop    ecx
        jmp    loc_4_0A90
loc_4_0A79:
        mov    ecx, dword ptr [0xd4c]        ; -> obj4:0x0D4C
        push   0
        push   ecx
        push   esi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        pop    ecx
        inc    eax
        jne    loc_4_0AA5
loc_4_0A90:
        pop    edi
        mov    byte ptr [0xfd0], cl          ; -> loc_1_0FD0
        cmp    dword ptr [0xd44], 0          ; -> obj4:0x0D44
        jne    loc_4_0AE6
        jmp    loc_4_0B7D
loc_4_0AA5:
        dec    eax
        mov    dword ptr [0xfcc], eax        ; -> loc_1_0FCC
        mov    eax, dword ptr [0xd44]        ; -> obj4:0x0D44
        or     eax, eax
        je     loc_4_0B73
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        jmp    loc_4_0B73
loc_4_0AC9:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ecx
        mov    byte ptr [0xfd0], cl          ; -> loc_1_0FD0
        pop    edi
loc_4_0AE6:
        mov    ecx, dword ptr [0xd4c]        ; -> obj4:0x0D4C
        add    ecx, 0xfff
        shr    ecx, 0xc
        cmp    ecx, 2
        jb     loc_4_0B01
        mov    byte ptr [0xfd0], 0x20        ; -> loc_1_0FD0
loc_4_0B01:
        xor    eax, eax
        push   0x80
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_4_0B7D
        mov    dword ptr [0xfc8], eax        ; -> loc_1_0FC8
        mov    dword ptr [0xfcc], edx        ; -> loc_1_0FCC
        push   edi
        movzx  eax, byte ptr [0xfd0]         ; -> loc_1_0FD0
        mov    ecx, 0x100
        mov    esi, dword ptr [0xd48]        ; -> obj4:0x0D48
        mov    edi, edx
        cld    
loc_4_0B3E:
        push   ecx
        push   esi
        mov    ecx, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        add    esi, 0x20
        pop    ecx
        loop   loc_4_0B3E
        mov    edi, dword ptr [esp]
        mov    eax, 0xa0
        call   sub_15_3E18
        mov    eax, 0xa1
        call   sub_15_3E18
        push   0
        push   dword ptr [0xd44]             ; -> obj4:0x0D44
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_0B73:
        clc    
        pop    edi
        pushfd 
        call   sub_15_3E44
        popfd  
        ret    
loc_4_0B7D:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_0B83:
        pushal 
        mov    eax, dword ptr [0xfc8]        ; -> loc_1_0FC8
        or     eax, eax
        je     loc_4_0BAE
        mov    ecx, dword ptr [0xd4c]        ; -> obj4:0x0D4C
        add    ecx, 0xfff
        shr    ecx, 0xc
        push   0x100
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_4_0BAE:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    ecx, 4
        call   sub_12_0019
        popal  
        clc    
        ret    
        db  000h
sub_4_0BC4:
        push   edi
        mov    eax, 0x32
        mov    edi, 0xd54                    ; -> loc_4_0D54
        xor    esi, esi
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0xfd4], eax        ; -> loc_1_0FD4
        add    eax, eax
        mov    dword ptr [0xfd8], eax        ; -> loc_1_0FD8
        pop    edi
        push   edi
        mov    eax, 2
        mov    edi, 0xde0                    ; -> obj4:0x0DE0
        xor    esi, esi
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        or     eax, eax
        jne    loc_4_0C00
        mov    eax, 2
loc_4_0C00:
        mov    dword ptr [0xfdc], eax        ; -> obj1:0x0FDC
        pop    edi
        mov    esi, 0x4670                   ; -> loc_15_4670
        mov    eax, 0x10038
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0xfe0], esi        ; -> obj1:0x0FE0
        ret    
        db  000h, 000h, 000h
sub_4_0C20:
        clc    
        ret    
sub_4_0C22:
        pushal 
        push   edi
        or     byte ptr [edi + 0x5e2], 0xe
        bts    dword ptr [edi], 0x10
        mov    dword ptr [edi + 0x4a7], 0xffffffff
        mov    edi, 0xe10                    ; -> loc_4_0E10
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        jb     loc_4_0CC1
        mov    edi, dword ptr [esp]
        test   byte ptr [edi + 4], 0x20
        jne    loc_4_0C80
        VxDCall  VDD2.VDD2_Get_Version           ; cd2000001c00
        jae    loc_4_0C80
        mov    edi, 0xdfc                    ; -> obj4:0x0DFC
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        mov    edi, dword ptr [esp]
        jae    loc_4_0C8D
        btr    dword ptr [edi + 4], 2
        jae    loc_4_0C80
        and    byte ptr [0x6c8], 0xfb        ; -> obj1:0x06C8
        or     byte ptr [edi + 4], 0x20
        or     byte ptr [0x6c8], 0x20        ; -> obj1:0x06C8
loc_4_0C80:
        and    byte ptr [edi + 4], 0xf7
        and    byte ptr [0x6c8], 0xf7        ; -> obj1:0x06C8
        jmp    loc_4_0CC0
loc_4_0C8D:
        or     byte ptr [0x6c8], 8           ; -> obj1:0x06C8
        or     byte ptr [edi + 4], 8
        bts    dword ptr [edi], 0x17
        test   byte ptr [edi + 4], 4
        jne    loc_4_0CC0
        mov    edx, 0x3b4
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    dl, 0xba
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        btr    dword ptr [edi], 0x17
loc_4_0CC0:
        clc    
loc_4_0CC1:
        pop    edi
        popal  
        ret    
sub_4_0CC4:
        or     word ptr [0xb1b], 0x40        ; -> obj9:0x0B1B
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 004h, 000h, 000h, 01Eh
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 084h
        db  004h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0A8h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 004h, 000h, 000h, 002h, 000h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0D54:
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x7055776f
        popal  
        je     0xdc5
        push   esp
        db  069h, 06Dh, 065h, 000h
loc_4_0D65:
        push   esi
        imul   esp, dword ptr [ebp + 0x6f], 0x6b636142
        jb     0xde0
        jne    0xde1
        dec    ebp
        jae    0xdde
        db  000h
loc_4_0D78:
        push   esi
        imul   esp, dword ptr [ebp + 0x6f], 0x70737553
        outsb  dx, byte ptr gs:[esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x61], 0x656c62
loc_4_0D8C:
        push   ebx
        arpl   word ptr [edx + 0x65], si
        outsb  dx, byte ptr gs:[esi]
        dec    esp
        imul   ebp, dword ptr [esi + 0x65], 0x6f4e0073
        outsb  dx, byte ptr [esi]
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x4173776f
        jo     loc_4_0E15
        add    byte ptr [ebp + esi*2 + 0x61], al
        insb   byte ptr es:[edi], dx
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x79616c
loc_4_0DB2:
        push   esi
        inc    edi
        inc    ecx
        dec    ebp
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        push   esp
        js     0xe31
        db  000h
loc_4_0DBE:
        push   edx
        jae    loc_4_0E27
        jb     loc_4_0E3A
        push   esi
        imul   esp, dword ptr [ebp + 0x6f], 0x4d4f52
loc_4_0DCE:
        inc    ecx
        jne    0xe45
        outsd  dx, dword ptr [esi]
        push   edx
        jae    loc_4_0E4A
        outsd  dx, dword ptr [esi]
        jb     loc_4_0E3E
        push   ebx
        arpl   word ptr [edx + 0x65], si
        outsb  dx, byte ptr gs:[esi]
        add    byte ptr [ebx + 0x63], dl
        jb     loc_4_0E53
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        inc    esi
        jb     loc_4_0E4E
        jno    0xe60
        outsb  dx, byte ptr gs:[esi]
        arpl   word ptr [ecx], di
loc_4_0DF0:
        push   ebp
        jae    loc_4_0E58
        push   edx
        dec    edi
        dec    ebp
        outsw  dx, word ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_4_0DFB
loc_4_0DFB:
        add    byte ptr [ebx], al
        add    byte ptr [edx + 0x4cb403], bh ; -> loc_15_4CB4
        add    byte ptr [ebx + eax + 0x4cb4], dh ; -> loc_15_4CB4
        mov    ch, 3
        mov    ah, 0x4c
        add    byte ptr [eax], al
loc_4_0E10:
        and    byte ptr [eax], al
        rol    byte ptr [ebx], 0xb4          ; -> loc_15_4CB4
loc_4_0E15:
        dec    esp
        add    byte ptr [eax], al
        rol    dword ptr [ebx], 0xb4         ; -> loc_15_4CB4
        dec    esp
        add    byte ptr [eax], al
        ret    0xb403                        ; -> loc_15_4CB4
        db  04Ch, 000h, 000h, 0C3h, 003h
        dd  loc_15_4CB4              ; fixup
loc_4_0E27:
        dec    esp
        add    byte ptr [eax], al
        les    eax, ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        lds    eax, ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        mov    byte ptr [ebx], 0xb4          ; -> loc_15_4CB4
        dec    esp
loc_4_0E3A:
        add    byte ptr [eax], al
        db  0C7h, 003h
loc_4_0E3E:
        mov    ah, 0x4c
        add    byte ptr [eax], al
        enter  -0x4bfd, 0x4c                 ; -> loc_15_4CB4
        add    byte ptr [eax], al
        leave  
        db  003h
loc_4_0E4A:
        mov    ah, 0x4c
        add    byte ptr [eax], al
loc_4_0E4E:
        retf   0xb403                        ; -> loc_15_4CB4
        db  04Ch, 000h
loc_4_0E53:
        add    bl, cl
        db  003h
        dd  loc_15_4CB4              ; fixup
loc_4_0E58:
        add    byte ptr [eax], al
        int3   
        add    esi, dword ptr [esp + ecx*2 + 0x3cd0000] ; -> loc_15_4CB4
        mov    ah, 0x4c
        add    byte ptr [eax], al
        into   
        add    esi, dword ptr [esp + ecx*2 + 0x3cf0000] ; -> loc_15_4CB4
        mov    ah, 0x4c
        add    byte ptr [eax], al
        rol    byte ptr [ebx], 1
        mov    ah, 0x4c
        add    byte ptr [eax], al
        rol    dword ptr [ebx], 1
        mov    ah, 0x4c
        add    byte ptr [eax], al
        rol    byte ptr [ebx], cl
        mov    ah, 0x4c
        add    byte ptr [eax], al
        rol    dword ptr [ebx], cl
        mov    ah, 0x4c
        add    byte ptr [eax], al
        aam    3
        mov    ah, 0x4c
        add    byte ptr [eax], al
        aad    3
        mov    ah, 0x4c
        add    byte ptr [eax], al
        salc   
        add    esi, dword ptr [esp + ecx*2 + 0x3d70000] ; -> loc_15_4CB4
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fadd   dword ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fld    dword ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fiadd  dword ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fild   dword ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fadd   qword ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fld    qword ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fiadd  word ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        fild   word ptr [ebx]
        mov    ah, 0x4c
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x3fe  R-X
loc_5_0000:
        sub    esp, 4
        mov    word ptr [esp + 2], 0
        mov    eax, dword ptr [0x10a4]       ; -> obj1:0x10A4
        or     eax, eax
        jne    loc_5_0047
        push   0x150                         ; -> obj6:0x0150
        call   sub_14_1860
        add    esp, 4
        or     eax, eax
        je     loc_5_005E
        push   0x15c                         ; -> obj6:0x015C
        push   eax
        call   sub_14_1868
        add    esp, 8
        mov    dword ptr [0x10a4], eax       ; -> obj1:0x10A4
        or     eax, eax
        jne    loc_5_0047
        mov    dword ptr [0x10a4], 0xffffffff ; -> obj1:0x10A4
        jmp    loc_5_005E
loc_5_0047:
        mov    dword ptr [0x10a4], eax       ; -> obj1:0x10A4
        lea    ecx, [esp + 2]
        push   ecx
        push   2
        push   eax
        push   0
        call   sub_14_1850
        add    esp, 0x10
loc_5_005E:
        mov    dword ptr [0x10a0], 0         ; -> obj1:0x10A0
        add    esp, 4
        ret    
loc_5_006C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_14_00E0
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        or     eax, eax
        jne    loc_5_0091
        mov    eax, 0x57
        jmp    loc_5_0127
loc_5_0091:
        cmp    dword ptr [ebp + 0x14], 0x1c
        je     loc_5_00A1
        mov    eax, 0x57
        jmp    loc_5_0127
loc_5_00A1:
        push   dword ptr [ebp + 8]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        jne    loc_5_00B7
        mov    eax, 0x57
        jmp    loc_5_0127
loc_5_00B7:
        test   byte ptr [eax + 8], 1
        jne    loc_5_00C4
        mov    eax, 0x57
        jmp    loc_5_0127
loc_5_00C4:
        push   0xff
        push   dword ptr [ebp + 8]
        call   sub_14_0C2B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_5_00E3
        mov    eax, 0x57
        jmp    loc_5_0127
loc_5_00E3:
        pushal 
        mov    ebp, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp]
        mov    ebx, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x14]
        push   ebp
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], ebx
        mov    dword ptr [ebp + 8], ecx
        mov    dword ptr [ebp + 0xc], edx
        mov    dword ptr [ebp + 0x10], esi
        mov    dword ptr [ebp + 0x14], edi
        popal  
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_14_0C2B
        add    esp, 8
        sub    eax, eax
loc_5_0127:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  083h, 0ECh, 01Ch, 053h, 056h, 057h, 055h, 02Bh, 0DBh, 08Bh, 074h, 024h, 030h, 068h, 0FFh, 000h
        db  000h, 000h, 056h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 08Bh, 0F8h, 083h, 0FFh, 0FFh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh
        db  076h, 083h, 07Ch, 024h, 034h, 000h, 074h, 022h, 0C7h, 044h, 024h, 010h, 003h, 000h, 000h, 000h
        db  08Dh, 044h, 024h, 010h, 06Ah, 01Ch, 050h, 06Ah, 000h, 056h, 0E8h, 0F7h, 0FEh, 0FFh, 0FFh, 083h
        db  0C4h, 010h, 08Bh, 0D8h, 00Bh, 0DBh, 074h, 006h, 0EBh, 04Bh, 00Bh, 0DBh, 075h, 03Dh, 08Bh, 06Ch
        db  024h, 038h, 080h, 07Dh, 000h, 000h, 074h, 033h, 066h, 00Fh, 0BEh, 045h, 000h, 066h, 00Dh, 000h
        db  00Eh, 00Fh, 0BFh, 0C0h, 089h, 044h, 024h, 010h, 0C7h, 044h, 024h, 014h, 000h, 000h, 000h, 000h
        db  08Dh, 044h, 024h, 010h, 06Ah, 01Ch, 050h, 045h, 06Ah, 000h, 056h, 0E8h, 0B6h, 0FEh, 0FFh, 0FFh
        db  083h, 0C4h, 010h, 08Bh, 0D8h, 080h, 07Dh, 000h, 000h, 075h, 0CDh, 057h, 056h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 01Ch, 0C3h, 053h, 056h, 057h
        db  055h, 08Bh, 06Ch, 024h, 020h, 00Bh, 0EDh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 03Dh
        db  08Bh, 05Ch, 024h, 014h, 06Ah, 002h, 053h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 022h, 08Bh
        db  07Ch, 024h, 018h, 08Bh, 074h, 024h, 01Ch, 08Bh, 0CDh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CDh
        db  083h, 0E1h, 003h, 0F3h, 0A4h, 050h, 053h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 053h, 056h, 057h, 055h, 08Bh, 06Ch
        db  024h, 020h, 00Bh, 0EDh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 03Dh, 08Bh, 05Ch, 024h
        db  014h, 06Ah, 002h, 053h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 022h, 08Bh
        db  07Ch, 024h, 01Ch, 08Bh, 074h, 024h, 018h, 08Bh, 0CDh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CDh
        db  083h, 0E1h, 003h, 0F3h, 0A4h, 050h, 053h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 056h, 08Bh, 044h, 024h, 014h, 00Bh
        db  0C0h, 074h, 029h, 083h, 0F8h, 004h, 077h, 024h, 08Bh, 074h, 024h, 008h, 06Ah, 001h, 056h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 075h, 002h, 0EBh, 00Eh, 050h, 056h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 02Bh, 0C0h, 0EBh, 005h, 0B8h, 057h, 000h, 000h, 000h, 05Eh, 0C3h, 056h, 08Bh
        db  044h, 024h, 014h, 00Bh, 0C0h, 074h, 029h, 083h, 0F8h, 004h, 077h, 024h, 08Bh, 074h, 024h, 008h
        db  06Ah, 001h, 056h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 075h, 002h, 0EBh, 00Eh, 050h, 056h, 0E8h
        dd  sub_14_0C2B              ; fixup
        db  083h, 0C4h, 008h, 02Bh, 0C0h, 0EBh, 005h, 0B8h, 057h, 000h, 000h, 000h, 05Eh, 0C3h, 000h, 000h
        db  000h
sub_5_02F4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0x4011
        mov    edx, 0x12
        mov    eax, 0x6c00
        mov    ecx, 6
        push   ebp
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        jb     loc_5_035A
        mov    ebx, eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, 0x4000
        mov    edx, dword ptr [ebp + 0xc]
        push   ebp
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        jb     loc_5_033C
        cmp    eax, dword ptr [ebp + 0x10]
        je     loc_5_033C
        stc    
loc_5_033C:
        pushfd 
        push   ebp
        mov    eax, 0x3e00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    eax, 0xd00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        popfd  
loc_5_035A:
        cmc    
        pop    edi
        pop    esi
        pop    ebx
        sbb    eax, eax
        leave  
        ret    
sub_5_0362:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x4100
        push   ebp
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        mov    edx, 1
        jae    loc_5_0387
        cmp    al, 2
        je     loc_5_0387
        xor    edx, edx
loc_5_0387:
        push   edx
        push   ebp
        mov    eax, 0xd00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_5_039D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0x10
        mov    edx, 1
        mov    eax, 0x6c00
        mov    ecx, 6
        push   ebp
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        jb     loc_5_03F6
        mov    ebx, eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, 0x3f00
        mov    edx, dword ptr [ebp + 0xc]
        push   ebp
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        jb     loc_5_03E5
        cmp    eax, dword ptr [ebp + 0x10]
        je     loc_5_03E5
        stc    
loc_5_03E5:
        pushfd 
        push   ebp
        mov    eax, 0x3e00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        popfd  
loc_5_03F6:
        cmc    
        pop    edi
        pop    esi
        pop    ebx
        sbb    eax, eax
        leave  
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x34c  RW-
        db  0E0h, 0C7h, 0B1h, 04Ch, 065h, 02Dh, 0CFh, 011h, 08Eh, 030h, 008h, 000h, 02Bh, 036h, 038h, 0A6h
        db  020h, 0AAh, 018h, 0C3h, 079h, 02Dh, 0CFh, 011h, 08Eh, 030h, 008h, 000h, 02Bh, 036h, 038h, 0A6h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0D0h, 002h, 000h, 000h, 090h, 001h, 000h, 000h, 046h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
        db  0D0h, 002h, 000h, 000h, 090h, 001h, 000h, 000h, 058h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        db  080h, 002h, 000h, 000h, 0E0h, 001h, 000h, 000h, 03Ch, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        db  080h, 002h, 000h, 000h, 0E0h, 001h, 000h, 000h, 043h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        db  080h, 002h, 000h, 000h, 0E0h, 001h, 000h, 000h, 048h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        db  080h, 002h, 000h, 000h, 0E0h, 001h, 000h, 000h, 04Bh, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  020h, 003h, 000h, 000h, 058h, 002h, 000h, 000h, 038h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  020h, 003h, 000h, 000h, 058h, 002h, 000h, 000h, 03Ch, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  020h, 003h, 000h, 000h, 058h, 002h, 000h, 000h, 048h, 000h, 000h, 000h, 000h, 080h, 000h, 000h
        db  020h, 003h, 000h, 000h, 058h, 002h, 000h, 000h, 04Bh, 000h, 000h, 000h, 000h, 040h, 000h, 000h
        db  040h, 003h, 000h, 000h, 070h, 002h, 000h, 000h, 04Bh, 000h, 000h, 000h, 000h, 020h, 000h, 000h
        db  000h, 004h, 000h, 000h, 000h, 003h, 000h, 000h, 02Bh, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  000h, 004h, 000h, 000h, 000h, 003h, 000h, 000h, 03Ch, 000h, 000h, 000h, 000h, 008h, 000h, 000h
        db  000h, 004h, 000h, 000h, 000h, 003h, 000h, 000h, 046h, 000h, 000h, 000h, 000h, 004h, 000h, 000h
        db  000h, 004h, 000h, 000h, 000h, 003h, 000h, 000h, 04Bh, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        db  000h, 005h, 000h, 000h, 000h, 004h, 000h, 000h, 04Bh, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  080h, 004h, 000h, 000h, 066h, 003h, 000h, 000h, 04Bh, 000h, 000h, 000h, 000h, 000h, 080h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  055h, 053h, 045h, 052h, 02Eh, 045h, 058h, 045h, 000h, 000h, 000h, 000h, 04Dh, 045h, 053h, 053h
        db  041h, 047h, 045h, 042h, 045h, 045h, 050h, 000h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 05Ch
        db  053h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 000h, 000h, 000h, 000h, 044h, 069h, 073h, 070h
        db  06Ch, 061h, 079h, 05Ch, 053h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 05Ch, 025h, 030h, 034h
        db  058h, 000h, 000h, 000h, 025h, 064h, 02Ch, 025h, 064h, 000h, 000h, 000h, 025h, 064h, 000h, 000h
        db  042h, 069h, 074h, 073h, 050h, 065h, 072h, 050h, 069h, 078h, 065h, 06Ch, 000h, 000h, 000h, 000h
        db  052h, 065h, 073h, 06Fh, 06Ch, 075h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 052h, 065h, 066h, 072h
        db  065h, 073h, 068h, 052h, 061h, 074h, 065h, 000h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 046h
        db  06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 000h, 044h, 065h, 073h, 06Bh, 074h, 06Fh, 070h, 053h
        db  069h, 07Ah, 065h, 000h, 044h, 065h, 073h, 06Bh, 074h, 06Fh, 070h, 050h, 06Fh, 073h, 000h, 000h
        db  050h, 04Fh, 053h, 054h, 000h, 000h, 000h, 000h, 044h, 045h, 046h, 041h, 055h, 04Ch, 054h, 000h
        db  064h, 072h, 076h, 032h, 000h, 000h, 000h, 000h, 044h, 045h, 046h, 041h, 055h, 04Ch, 054h, 000h
        db  056h, 044h, 044h, 03Ah, 020h, 063h, 061h, 06Eh, 074h, 020h, 070h, 06Fh, 073h, 074h, 020h, 052h
        db  04Fh, 04Dh, 02Ch, 020h, 06Eh, 06Fh, 06Eh, 020h, 070h, 068h, 079h, 073h, 020h, 06Dh, 065h, 06Dh
        db  06Fh, 072h, 079h, 020h, 061h, 074h, 020h, 043h, 030h, 030h, 030h, 02Dh, 043h, 046h, 046h, 046h
        db  00Dh, 00Ah, 000h, 000h, 056h, 044h, 044h, 03Ah, 020h, 063h, 061h, 06Eh, 074h, 020h, 072h, 065h
        db  061h, 064h, 020h, 052h, 04Fh, 04Dh, 020h, 066h, 072h, 06Fh, 06Dh, 020h, 050h, 043h, 049h, 020h
        db  064h, 065h, 076h, 069h, 063h, 065h, 020h, 028h, 06Fh, 072h, 020h, 072h, 065h, 067h, 069h, 073h
        db  074h, 072h, 079h, 029h, 00Dh, 00Ah, 000h, 000h, 056h, 044h, 044h, 03Ah, 020h, 02Ah, 02Ah, 02Ah
        db  020h, 063h, 061h, 06Eh, 074h, 020h, 073h, 074h, 061h, 072h, 074h, 020h, 064h, 065h, 076h, 069h
        db  063h, 065h, 02Ch, 020h, 06Eh, 06Fh, 074h, 020h, 061h, 06Ch, 06Ch, 020h, 064h, 065h, 076h, 069h
        db  063h, 065h, 073h, 020h, 061h, 072h, 065h, 020h, 050h, 043h, 049h, 020h, 028h, 06Fh, 072h, 020h
        db  062h, 072h, 06Fh, 06Bh, 065h, 06Eh, 020h, 050h, 043h, 049h, 020h, 064h, 065h, 076h, 069h, 063h
        db  065h, 029h, 02Eh, 00Dh, 00Ah, 000h, 000h, 000h, 025h, 030h, 034h, 058h, 000h, 000h, 000h, 000h
        db  044h, 065h, 066h, 061h, 075h, 06Ch, 074h, 05Fh, 04Dh, 06Fh, 06Eh, 069h, 074h, 06Fh, 072h, 000h
        db  04Dh, 06Fh, 06Eh, 069h, 074h, 06Fh, 072h, 05Ch, 025h, 073h, 05Ch, 025h, 073h, 000h, 000h, 000h
        db  045h, 044h, 049h, 044h, 000h, 000h, 000h, 000h, 04Dh, 04Fh, 04Eh, 049h, 054h, 04Fh, 052h, 05Ch
        db  025h, 073h, 02Ch, 02Ah, 050h, 04Eh, 050h, 030h, 039h, 046h, 046h, 000h, 048h, 061h, 072h, 064h
        db  077h, 061h, 072h, 065h, 049h, 044h, 000h, 000h, 04Dh, 06Fh, 06Eh, 069h, 074h, 06Fh, 072h, 000h
        db  043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h, 025h, 064h, 02Ch, 025h, 064h, 000h, 000h, 000h
        db  04Dh, 061h, 078h, 052h, 065h, 073h, 06Fh, 06Ch, 075h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h
        db  031h, 000h, 000h, 000h, 044h, 050h, 04Dh, 053h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0xc  R-X
        db  066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x59c  R-X
loc_8_0000:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        stc    
        ret    
        db  000h
sub_8_000F:
        pushal 
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    edx, 0x21
        mov    ecx, 0
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        mov    byte ptr [0xe], al            ; -> obj8:0x000E
        or     al, 0xff
        mov    edx, 0x21
        mov    ecx, 4
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        mov    esi, 0                        ; -> loc_8_0000
        mov    eax, 0x1a
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        popal  
        ret    
sub_8_0054:
        pushal 
        mov    esi, 0                        ; -> loc_8_0000
        mov    eax, 0x1a
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        mov    al, byte ptr [0xe]            ; -> obj8:0x000E
        mov    edx, 0x21
        mov    ecx, 4
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        popal  
        ret    
sub_8_0082:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    edx, edx
        mov    eax, 0xf0000
loc_8_0095:
        cmp    byte ptr [edx + eax], 0xcf
        je     loc_8_009E
        inc    edx
        jmp    loc_8_0095
loc_8_009E:
        shl    eax, 0xc
        or     eax, edx
        mov    edx, 0x40
        mov    dword ptr [edx], eax
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    edi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 0xc]
        xchg   ah, al
        mov    dword ptr [edi + 0x1c], eax
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [edi + 0x18], 0
        mov    dword ptr [edi + 0x14], 0
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 8], 0
        mov    word ptr [edi + 0x40], 0
        mov    word ptr [edi + 0x44], 0
        mov    ecx, dword ptr [ebp + 8]
        shr    ecx, 4
        mov    edx, 3
        mov    word ptr [edi + 0x38], cx
        mov    word ptr [edi + 0x3c], cx
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
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
sub_8_0138:
        push   ebx
        push   esi
        push   edi
        test   word ptr [0xb1b], 0x40        ; -> obj9:0x0B1B
        je     loc_8_0169
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x38], 0
        je     loc_8_0163
        call   dword ptr [edi + 0x38]
loc_8_0163:
        pop    edi
        call   sub_15_05F4
loc_8_0169:
        or     byte ptr [0x7a0], 2           ; -> loc_1_07A0
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_8_0174:
        push   ebx
        push   esi
        push   edi
        test   word ptr [0xb1b], 0x40        ; -> obj9:0x0B1B
        je     loc_8_01A5
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x34], 0
        je     loc_8_019F
        call   dword ptr [edi + 0x34]
loc_8_019F:
        pop    edi
        call   sub_15_062A
loc_8_01A5:
        and    byte ptr [0x7a0], 0xfd        ; -> loc_1_07A0
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_8_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    loc_8_01C4
        mov    esi, dword ptr [0x10b4]       ; -> obj1:0x10B4
loc_8_01C4:
        mov    eax, dword ptr [0x10ac]       ; -> obj1:0x10AC
        cmp    eax, esi
        jne    loc_8_01D2
        jmp    loc_8_031D
loc_8_01D2:
        and    dword ptr [eax + 0xc], 0xffffff7f
        add    eax, 0x2c
        push   eax
        call   sub_14_0000
        add    esp, 4
        mov    dword ptr [0x10ac], esi       ; -> obj1:0x10AC
        or     dword ptr [esi + 0xc], 0x80
        lea    eax, [esi + 0x2c]
        push   eax
        call   sub_14_0069
        add    esp, 4
        mov    eax, dword ptr [0x10ac]       ; -> obj1:0x10AC
        cmp    eax, dword ptr [0x10b4]       ; -> obj1:0x10B4
        jne    loc_8_0290
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 4], 0x40a
        push   0
        jb     loc_8_0274
        push   dword ptr [0x2c]              ; -> obj6:0x002C
        push   0xa0
        call   sub_14_18A8
        push   eax
        mov    eax, dword ptr [0x28]         ; -> obj6:0x0028
        shr    eax, 0xc
        push   eax
        call   sub_14_1920
        add    esp, 0x14
        push   0x40000000
        push   dword ptr [0x2c]              ; -> obj6:0x002C
        mov    eax, dword ptr [0x28]         ; -> obj6:0x0028
        shr    eax, 0xc
        push   eax
        call   sub_14_18E0
        add    esp, 0xc
        mov    dword ptr [0x28], 0           ; -> obj6:0x0028
        jmp    loc_8_031D
loc_8_0274:
        push   0x30
        push   0xa0
        call   sub_14_18A8
        push   eax
        push   0xa0
        call   sub_14_1918
        jmp    loc_8_031A
loc_8_0290:
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x40a
        jb     loc_8_02D9
        mov    eax, dword ptr [esi + 0x24]
        add    eax, 0xfff
        shr    eax, 0xc
        add    eax, 0x20
        mov    dword ptr [0x2c], eax         ; -> obj6:0x002C
        push   0x40000000
        push   eax
        push   0xa0
        call   sub_14_18D8
        add    esp, 0xc
        mov    dword ptr [0x28], eax         ; -> obj6:0x0028
        or     eax, eax
        jne    loc_8_02D9
        sub    eax, eax
        jmp    loc_8_0322
loc_8_02D9:
        push   0
        push   0x20
        push   0xa0
        call   sub_14_18A8
        push   eax
        push   0xa0
        call   sub_14_1918
        add    esp, 0x14
        push   0
        mov    eax, dword ptr [esi + 0x24]
        add    eax, 0xfff
        shr    eax, 0xc
        push   eax
        push   0xc0
        call   sub_14_18A8
        push   eax
        mov    eax, dword ptr [esi + 0x28]
        shr    eax, 0xc
        push   eax
        call   sub_14_1920
loc_8_031A:
        add    esp, 0x14
loc_8_031D:
        mov    eax, 1
loc_8_0322:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_8_0327:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0x50
        lea    eax, [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        push   1
        push   0x1f0                         ; -> obj6:0x01F0
        push   0x1f8                         ; -> obj6:0x01F8
        push   dword ptr [esi + 4]
        call   sub_14_1990
        add    esp, 0x1c
        or     eax, eax
        jne    loc_8_036A
        cmp    byte ptr [ebp - 0x5c], 0x30
        jne    loc_8_036A
        jmp    loc_8_0590
loc_8_036A:
        mov    dword ptr [ebp - 8], 0x50
        mov    byte ptr [ebp - 0x5c], 0
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        push   1
        push   0x200                         ; -> obj6:0x0200
        push   0x208                         ; -> obj6:0x0208
        push   dword ptr [esi + 4]
        call   sub_14_1990
        add    esp, 0x1c
        or     eax, eax
        jne    loc_8_0590
        cmp    byte ptr [ebp - 0x5c], 0
        je     loc_8_0590
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x40a
        jae    loc_8_03E6
        push   0x10000
        push   0xc0000
        call   sub_14_0D79
        add    esp, 8
        or     eax, eax
        jne    loc_8_03E6
        push   0x210                         ; -> obj6:0x0210
        call   sub_14_06E6
        add    esp, 4
        jmp    loc_8_0590
loc_8_03E6:
        push   dword ptr [esi + 4]
        call   sub_14_0580
        add    esp, 4
        mov    dword ptr [esi + 0x24], eax
        or     eax, eax
        je     loc_8_042C
        cmp    eax, 0x100000
        jbe    loc_8_0406
        mov    dword ptr [esi + 0x24], 0x10000
loc_8_0406:
        cmp    dword ptr [esi + 0x24], 0x10000
        jbe    loc_8_0414
        jmp    loc_8_0590
loc_8_0414:
        mov    dword ptr [ebp - 8], 0
        push   4
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [esi + 4]
        call   sub_14_05AC
        add    esp, 0xc
loc_8_042C:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0xaa55
        jne    loc_8_0583
        push   1
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        mov    eax, dword ptr [esi + 0x24]
        shr    eax, 0xc
        push   eax
        call   sub_14_18BC
        add    esp, 0x20
        mov    dword ptr [esi + 0x28], eax
        or     eax, eax
        jne    loc_8_0468
        jmp    loc_8_0590
loc_8_0468:
        push   dword ptr [esi + 0x24]
        push   eax
        push   dword ptr [esi + 4]
        call   sub_14_05AC
        add    esp, 0xc
        push   1
        push   dword ptr [esi + 4]
        call   sub_16_00DB
        add    esp, 8
        or     eax, eax
        je     loc_8_0568
        mov    ebx, 0                        ; -> loc_8_0000
        shr    ebx, 0xc
        mov    edi, 0x59b                    ; -> obj8:0x059B
        shr    edi, 0xc
        sub    edi, ebx
        inc    edi
        push   0
        push   edi
        push   ebx
        call   sub_14_18D8
        add    esp, 0xc
        call   sub_8_000F
        push   1
        call   sub_14_040A
        add    esp, 4
        push   esi
        call   sub_14_0801
        add    esp, 4
        push   esi
        call   sub_8_01B0
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_8_04ED
        push   dword ptr [esi + 0x18]
        push   0xc0000
        call   sub_8_0082
        add    esp, 8
        push   0
        call   sub_8_01B0
        add    esp, 4
loc_8_04ED:
        push   0
        call   sub_14_0801
        add    esp, 4
        push   0
        call   sub_14_040A
        add    esp, 4
        call   sub_8_0054
        push   0
        push   edi
        push   ebx
        call   sub_14_18E0
        add    esp, 0xc
        push   1
        push   dword ptr [esi + 4]
        call   sub_16_01FE
        add    esp, 8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_8_0568
        mov    eax, dword ptr [esi + 0x28]
        movzx  ecx, byte ptr [eax + 2]
        shl    ecx, 9
        cmp    dword ptr [esi + 0x24], ecx
        je     loc_8_0558
        mov    dword ptr [esi + 0x24], ecx
        add    ecx, 0xfff
        and    ecx, 0xfffff000
        mov    dword ptr [esi + 0x24], ecx
        push   0
        shr    ecx, 0xc
        push   ecx
        push   eax
        call   sub_14_1928
        add    esp, 0xc
        mov    dword ptr [esi + 0x28], eax
loc_8_0558:
        cmp    dword ptr [esi + 0x24], 0x8000
        ja     loc_8_0568
        mov    eax, 1
        jmp    loc_8_0592
loc_8_0568:
        mov    eax, dword ptr [esi + 0x28]
        or     eax, eax
        je     loc_8_057A
        push   0
        push   eax
        call   sub_14_18C4
        add    esp, 8
loc_8_057A:
        mov    dword ptr [esi + 0x28], 0
        jmp    loc_8_0590
loc_8_0583:
        push   0x244                         ; -> obj6:0x0244
        call   sub_14_06E6
        add    esp, 4
loc_8_0590:
        sub    eax, eax
loc_8_0592:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x1dd8  R-X
loc_9_0000:
        clc    
        ret    
loc_9_0002:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_9_0094
        ret    
sub_9_0010:
        pushal 
        bts    dword ptr [0x6c4], 6          ; -> obj1:0x06C4
        jb     loc_9_0078
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        mov    ebp, dword ptr [ebx + 8]
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        jne    loc_9_003C
        mov    eax, 3
        call   sub_4_031A
        call   sub_4_006F
loc_9_003C:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi + 2], 0x40
        je     loc_9_0054
        test   byte ptr [edi], 0x80
        jne    loc_9_0054
        call   sub_15_498E
loc_9_0054:
        mov    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_12D5
        call   sub_9_0574
        call   sub_9_05A0
loc_9_0071:
        call   sub_15_119E
loc_9_0076:
        popal  
        ret    
loc_9_0078:
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_9_0076
        mov    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        jmp    loc_9_0071
        db  000h, 000h, 000h
sub_9_0094:
        mov    ecx, dword ptr [0xeac]        ; -> obj1:0x0EAC
        jecxz  loc_9_00AD
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x10
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
loc_9_00AD:
        call   sub_1_05F8
        ret    
        db  000h
sub_9_00B4:
        pushal 
        mov    dword ptr [0xecc], edx        ; -> obj1:0x0ECC
        movzx  ecx, ch
        mov    word ptr [0xed0], cx          ; -> obj1:0x0ED0
        popal  
        ret    
sub_9_00C7:
        ret    
sub_9_00C8:
        ret    
        db  000h, 000h, 000h
VDD_Get_Version:
        mov    ax, 0x400
        mov    esi, 0xd6                     ; -> loc_9_00D6
        ret    
loc_9_00D6:
        push   esi
        dec    ecx
        inc    esp
        inc    ebp
        dec    edi
        push   esi
        inc    edi
        inc    ecx
VDD_Set_Sleep_Flag_Addr:
        mov    dword ptr [0xef4], eax        ; -> loc_1_0EF4
        ret    
loc_9_00E4:
        push   0xff
        push   1
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
        mov    dword ptr [0xee0], ebx        ; -> loc_1_0EE0
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_9_0107
        call   dword ptr [edi + 0xc]
loc_9_0107:
        pop    edi
        clc    
        ret    
loc_9_010A:
        cmp    ebx, dword ptr [0xee0]        ; -> loc_1_0EE0
        jne    loc_9_0147
        or     byte ptr [0x6c4], 4           ; -> obj1:0x06C4
        je     loc_9_0123
        xor    eax, eax
        VxDCall  VDD.VDD_Msg_ClrScrn             ; cd2007000a00
loc_9_0123:
        and    byte ptr [0x6c4], 0xbf        ; -> obj1:0x06C4
        mov    dword ptr [0xee0], 0          ; -> loc_1_0EE0
        mov    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_0049
loc_9_0147:
        clc    
        ret    
VDD_Msg_ClrScrn:
        pushal 
        and    al, 7
        shl    eax, 4
        or     al, 7
        mov    byte ptr [0xeed], al          ; -> obj1:0x0EED
        mov    byte ptr [0xeec], al          ; -> loc_1_0EEC
        call   sub_15_1802
        call   sub_9_0010
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_9_0174
        call   dword ptr [edi + 0xc]
loc_9_0174:
        pop    edi
        mov    edi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        add    edi, 0x18000
        mov    al, 0x20
        mov    ah, byte ptr [0xeec]          ; -> loc_1_0EEC
        mov    ecx, dword ptr [0xee4]        ; -> loc_1_0EE4
        mov    edx, dword ptr [0xee8]        ; -> loc_1_0EE8
        imul   ecx, edx
        rep stosw word ptr es:[edi], ax
        call   sub_15_17B4
        popal  
        mov    eax, dword ptr [0xee8]        ; -> loc_1_0EE8
        mov    edx, dword ptr [0xee4]        ; -> loc_1_0EE4
        ret    
VDD_Msg_ForColor:
        push   eax
        and    al, 0xf
        mov    ah, byte ptr [0xeed]          ; -> obj1:0x0EED
        and    ah, 0xf0
        or     al, ah
        mov    byte ptr [0xeed], al          ; -> obj1:0x0EED
        pop    eax
        ret    
VDD_Msg_BakColor:
        push   eax
        and    al, 7
        shl    eax, 4
        mov    ah, byte ptr [0xeed]          ; -> obj1:0x0EED
        and    ah, 0xf
        or     al, ah
        mov    byte ptr [0xeed], al          ; -> obj1:0x0EED
        pop    eax
        ret    
VDD_Msg_TextOut:
        jecxz  loc_9_021C
        pushal 
        call   sub_9_0010
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_9_01F1
        call   dword ptr [edi + 0xc]
loc_9_01F1:
        pop    edi
        mov    edx, 0xa0
        mul    dl
        mov    edx, dword ptr [esp + 0x14]
        add    eax, edx
        add    eax, edx
        mov    edi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        add    edi, 0x18000
        add    edi, eax
        mov    ah, byte ptr [0xeed]          ; -> obj1:0x0EED
        cld    
loc_9_0216:
        lodsb  al, byte ptr [esi]
        stosw  word ptr es:[edi], ax
        loop   loc_9_0216
        popal  
loc_9_021C:
        ret    
VDD_Msg_SetCursPos:
        push   eax
        push   edx
        call   sub_9_0010
        cmp    eax, dword ptr [0xee4]        ; -> loc_1_0EE4
        jae    loc_9_024F
        push   edx
        mov    edx, dword ptr [0xee8]        ; -> loc_1_0EE8
        mul    dl
        pop    edx
        add    eax, edx
loc_9_0238:
        call   sub_15_146B
        push   eax
        mov    al, 0xf
        out    dx, al
        pop    eax
        inc    edx
        out    dx, al
        dec    edx
        mov    al, 0xe
        out    dx, al
        inc    edx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_9_024F:
        or     eax, 0xffffffff
        jmp    loc_9_0238
VDD_Set_HCurTrk:
        push   ebx
        add    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        or     byte ptr [ebx + 1], 2
        pop    ebx
        ret    
VDD_Get_GrabRtn:
        mov    esi, 0x267                    ; -> loc_9_0267
        ret    
loc_9_0267:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_1969
        call   sub_15_00A8
        jb     loc_9_0285
        call   sub_15_1E91
        call   sub_15_47E8
loc_9_0285:
        ret    
VDD_Register_Extra_Screen_Selector:
        push   eax
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._GetDescriptor              ; cd207b000100
        add    esp, 0xc
        pop    esi
        mov    edi, 0xf0c                    ; -> loc_1_0F0C
        mov    ecx, 8
loc_9_029F:
        cmp    esi, dword ptr [edi]
        je     loc_9_02FD
        add    edi, 0x14
        dec    ecx
        jne    loc_9_029F
        mov    edi, 0xf0c                    ; -> loc_1_0F0C
        mov    ecx, 8
loc_9_02B3:
        cmp    dword ptr [edi], 0
        je     loc_9_02C1
        add    edi, 0x14
        dec    ecx
        jne    loc_9_02B3
        stc    
        jmp    loc_9_02FE
loc_9_02C1:
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], edx
        xor    ax, ax
        and    edx, 0xfff0ffff
        and    edx, 0xff7fffff
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [edi + 0x10], edx
        cmp    dword ptr [0xfac], 0          ; -> loc_1_0FAC
        jne    loc_9_02FD
        mov    eax, 0xd
        mov    esi, 0x3078                   ; -> loc_15_3078
        VMMCall  VMM.Hook_PM_Fault               ; cd2080000100
        mov    dword ptr [0xfac], esi        ; -> loc_1_0FAC
loc_9_02FD:
        clc    
loc_9_02FE:
        ret    
loc_9_02FF:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 1
        jne    loc_9_03C9
        push   3
        push   eax
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
        and    byte ptr [0x7a0], 0xfb        ; -> loc_1_07A0
        push   ebx
        push   edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   ebx
loc_9_0325:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_4BBC
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_9_0325
        pop    ebx
        pop    edi
        pop    ebx
        mov    byte ptr [0xfb0], 0xff        ; -> loc_1_0FB0
        VxDCall  Dev_011F.Service_0000h          ; cd2000001f01
        jae    loc_9_0356
        mov    byte ptr [0xfb0], 0           ; -> loc_1_0FB0
loc_9_0356:
        mov    ax, word ptr [ebp]
        mov    word ptr [0xf00], ax          ; -> loc_1_0F00
        mov    ax, word ptr [ebp + 0x38]
        mov    word ptr [0xf02], ax          ; -> loc_1_0F02
        mov    word ptr [0xed0], 0           ; -> obj1:0x0ED0
        mov    dword ptr [0xecc], 0x20       ; -> obj1:0x0ECC
        mov    edx, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp + 0x14], -1
        jne    loc_9_0392
loc_9_0386:
        mov    dword ptr [0xed2], 0x20       ; -> loc_1_0ED2
        jmp    loc_9_03C1
loc_9_0392:
        mov    ecx, edx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 4], 0
        je     loc_9_03A3
        call   dword ptr [edi + 4]
loc_9_03A3:
        pop    edi
        cmp    edx, dword ptr [ebp + 0x18]
        je     loc_9_0386
        mov    eax, dword ptr [0xecc]        ; -> obj1:0x0ECC
        add    al, 2
        cmp    edx, 0x8000
        jbe    loc_9_03BA
        add    al, 2
loc_9_03BA:
        mov    dword ptr [0xed2], eax        ; -> loc_1_0ED2
        add    edx, ecx
loc_9_03C1:
        and    dword ptr [ebp + 0x2c], 0xfffffffe
        mov    dword ptr [ebp + 0x1c], edx
        clc    
loc_9_03C9:
        ret    
loc_9_03CA:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 1
        jne    loc_9_0432
        or     byte ptr [0x7a0], 4           ; -> loc_1_07A0
        call   sub_15_05F4
        push   ebx
        push   edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   ebx
loc_9_03E7:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    dword ptr [edi + 0x4a7], 0
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_9_03E7
        pop    ebx
        pop    edi
        pop    ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x68], 0
        je     loc_9_0416
        call   dword ptr [edi + 0x68]
loc_9_0416:
        pop    edi
        push   0xff
        push   1
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
        mov    eax, 3
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_9_0432:
        ret    
loc_9_0433:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 1
        jne    loc_9_0487
        push   3
        push   eax
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
        push   dword ptr [0x7ac]             ; -> loc_1_07AC
        push   dword ptr [0x7a8]             ; -> loc_1_07A8
        mov    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        mov    dword ptr [0x7ac], ebx        ; -> loc_1_07AC
        mov    dword ptr [0x7a8], ebx        ; -> loc_1_07A8
        mov    al, byte ptr [0x7a0]          ; -> loc_1_07A0
        push   eax
        mov    byte ptr [0x7a0], 0           ; -> loc_1_07A0
        call   sub_15_1EAC
        pop    eax
        mov    byte ptr [0x7a0], al          ; -> loc_1_07A0
        pop    dword ptr [0x7a8]             ; -> loc_1_07A8
        pop    dword ptr [0x7ac]             ; -> loc_1_07AC
loc_9_0487:
        ret    
loc_9_0488:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi], 0
        je     loc_9_0495
        call   dword ptr [edi]
loc_9_0495:
        pop    edi
        ret    
loc_9_0497:
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0xf08], eax        ; -> loc_1_0F08
        ret    
loc_9_04A7:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x70], 0
        je     loc_9_04B6
        call   dword ptr [edi + 0x70]
loc_9_04B6:
        pop    edi
        ret    
loc_9_04B8:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x74], 0
        je     loc_9_04C7
        call   dword ptr [edi + 0x74]
loc_9_04C7:
        pop    edi
        ret    
loc_9_04C9:
        mov    dword ptr [0xef8], 0          ; -> loc_1_0EF8
        mov    ax, 0x1418
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_9_04E9
        mov    dword ptr [0xef8], eax        ; -> loc_1_0EF8
        xor    eax, eax
loc_9_04E9:
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_9_04EE:
        xor    edi, edi
        xor    esi, esi
        mov    eax, 0xc0001030
        movzx  ecx, word ptr [0x12]          ; -> loc_3_0012
        add    ecx, dword ptr [4]            ; -> loc_3_0004
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003801700
sub_9_050A:
        movzx  ecx, word ptr [0xe]           ; -> loc_3_000E
        add    ecx, dword ptr [4]            ; -> loc_3_0004
        mov    dl, 0x16
        jmp    loc_9_0547
sub_9_051B:
        test   byte ptr [edi + 4], 0x40
        jne    loc_9_0537
        test   byte ptr [edi], 1
        je     loc_9_0537
        movzx  ecx, word ptr [0x10]          ; -> obj3:0x0010
        add    ecx, dword ptr [4]            ; -> loc_3_0004
        mov    dl, 0x12
        jmp    loc_9_0547
loc_9_0537:
        ret    
sub_9_0538:
        movzx  ecx, word ptr [0xc]           ; -> loc_3_000C
        add    ecx, dword ptr [4]            ; -> loc_3_0004
        mov    dl, 0x10
loc_9_0547:
        mov    eax, 0x80000030
        movzx  edx, dl
        bts    dword ptr [edi + 8], edx
        jb     loc_9_0570
        push   edi
        push   eax
        xor    edi, edi
        xor    esi, esi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        pop    eax
        jae    loc_9_056F
        or     eax, 0x1000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
loc_9_056F:
        pop    edi
loc_9_0570:
        ret    
        db  000h, 000h, 000h
sub_9_0574:
        mov    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_3A1F
        xor    edx, edx
        mov    esi, 0x3eda                   ; -> loc_15_3EDA
        movzx  ecx, byte ptr [0xe99]         ; -> obj1:0x0E99
loc_9_0595:
        mov    eax, edx
        call   sub_15_2B10
        inc    edx
        loop   loc_9_0595
        ret    
sub_9_05A0:
        call   sub_15_18BA
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_9_05B4
        call   dword ptr [edi + 0xc]
loc_9_05B4:
        pop    edi
        mov    dx, 0x402
        call   sub_15_4324
        push   edi
        mov    edi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        call   sub_9_05CD
        pop    edi
        clc    
        ret    
sub_9_05CD:
        push   edx
        mov    dl, byte ptr [0xfd0]          ; -> loc_1_0FD0
        mov    dh, 0x20
        sub    dh, dl
        mov    ecx, 0x100
        mov    esi, dword ptr [0xfcc]        ; -> loc_1_0FCC
        cld    
        xor    eax, eax
loc_9_05E6:
        push   ecx
        movzx  ecx, dl
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  ecx, dh
        rep stosb byte ptr es:[edi], al
        pop    ecx
        loop   loc_9_05E6
        pop    edx
        ret    
        db  000h, 000h
sub_9_05F8:
        test   byte ptr [edi + 4], 4
        jne    loc_9_060D
        test   byte ptr [edi + 0x96], 1
        jne    loc_9_063B
        test   byte ptr [edi + 4], 8
        je     loc_9_063B
loc_9_060D:
        bts    dword ptr [edi], 0x17
        jb     loc_9_063B
        push   edx
        xor    edx, edx
        xchg   dword ptr [edi + 0x4a7], edx
        push   edx
        mov    edx, 0x3b4
        call   sub_15_4C7C
        inc    edx
        call   sub_15_4C7C
        mov    dl, 0xba
        call   sub_15_4C7C
        pop    dword ptr [edi + 0x4a7]
        pop    edx
loc_9_063B:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_067C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0680:
        add    byte ptr [eax], al
loc_9_0682:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0690:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06B4:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06D8:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06F0:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0714:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0734:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0774:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0794:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0798:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0860:
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 064h, 069h, 073h, 070h, 06Ch
        db  061h, 079h, 05Ch, 073h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 000h
loc_9_0915:
        inc    edx
        imul   esi, dword ptr [ebx + esi*2 + 0x50], 0x69507265
        js     loc_9_0985
        insb   byte ptr es:[edi], dx
        db  000h
loc_9_0922:
        push   edx
        jae    0x995
        insb   byte ptr es:[edi], dx
        jne    loc_9_099D
        db  069h, 06Fh, 06Eh, 000h
loc_9_092D:
        inc    esp
        push   eax
        dec    ecx
        dec    esp
        outsd  dx, dword ptr [esi]
        db  067h, 069h, 063h, 061h, 06Ch, 058h, 000h
loc_9_0939:
        inc    ebp
        js     0x9b0
        dec    ebp
        outsd  dx, dword ptr [esi]
        push   ebx
        ja     loc_9_09AC
        je     0x9a8
        push   0x73654400
        imul   esi, dword ptr [edi + ebp*2 + 0x70], 0x50
        outsd  dx, dword ptr [esi]
        jae    loc_9_0952
loc_9_0952:
        inc    esp
        jae    0x9c1
        je     0x9c7
        jo     loc_9_09AD
        imul   edi, dword ptr [edx + 0x65], 0x73694400
        jo     0x9cf
        popal  
        jns    loc_9_09AC
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_9_096B
loc_9_096B:
        push   ebx
        arpl   word ptr [edx + 0x65], si
        outsb  dx, byte ptr gs:[esi]
        push   edx
        jae    loc_9_09E4
        insb   byte ptr es:[edi], dx
        jne    loc_9_09EC
        db  069h, 06Fh, 06Eh, 000h
loc_9_097C:
        inc    esp
        push   edx
        push   esi
        add    byte ptr [edx + edx*2 + 0x56], al
        xor    al, byte ptr [eax]
loc_9_0985:
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        push   esi
        inc    esp
        inc    esp
        add    byte ptr [ebx + 0x41], al
        push   edx
        inc    esp
        push   esi
        inc    esp
        inc    esp
        add    byte ptr [ebx + 0x48], al
        dec    ecx
        push   eax
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_9_099D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x4f], cl
        inc    esp
        inc    ebp
        push   ebx
        pop    esp
        add    byte ptr [ebp + eax*2 + 0x46], al
        inc    ecx
        push   ebp
loc_9_09AC:
        dec    esp
loc_9_09AD:
        push   esp
        add    byte ptr [0x31], ch
        add    byte ptr [eax], al
loc_9_09B6:
        dec    ebp
        dec    edi
        inc    esp
        inc    ebp
        add    byte ptr [ebp + 0x4f], cl
        inc    esp
        inc    ebp
        push   ebx
        add    byte ptr [eax], al
loc_9_09C2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_09C6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_09E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_09EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0AC6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_0B0E:
        dec    esi
        dec    edi
        dec    esi
        inc    ebp
        db  000h
loc_9_0B13:
        xor    al, 0x30
        sub    al, 0x34
        cmp    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0B1E:
        add    byte ptr [eax], al
loc_9_0B20:
        add    byte ptr [eax], al
loc_9_0B22:
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x79616c
loc_9_0B2A:
        dec    ecx
        outsb  dx, byte ptr [si]
        outsd  dx, dword ptr [esi]
        jb     0xb95
        push   edx
        db  065h, 067h, 069h, 073h, 074h, 072h, 079h, 000h
loc_9_0B39:
        push   edx
        jb     0xba3
        jae    0xba8
        push   edx
        popal  
        je     0xba9
        add    byte ptr [edx + 0x65], dl
        jb     0xbaf
        jae    0xbb4
        push   edx
        popal  
        je     0xbb5
        inc    esi
        jne    0xbb7
        add    byte ptr gs:[bx + si], al
        add    byte ptr [eax], al
        db  000h
sub_9_0B5A:
        push   ebp
        mov    ebp, esp
        lea    eax, [ebp + 8]
        push   0
        push   dword ptr [ebp + 8]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004003300
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0
        jne    loc_9_0B7B
        mov    eax, dword ptr [ebp + 8]
loc_9_0B7B:
        leave  
        ret    
sub_9_0B7D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cld    
        test   word ptr [0xb1b], 0x40        ; -> obj9:0x0B1B
        jne    loc_9_0D8D
        test   word ptr [0xb1b], 0x10        ; -> obj9:0x0B1B
        jne    loc_9_0D8D
        or     ebx, ebx
        je     loc_9_0D8D
        or     word ptr [0xb1b], 0xb0        ; -> obj9:0x0B1B
        mov    dword ptr [0x65c], ebx        ; -> obj9:0x065C
        xor    eax, eax
        mov    esi, 0xb22                    ; -> loc_9_0B22
        mov    edi, 0xb2a                    ; -> loc_9_0B2A
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0xb1d], al          ; -> obj9:0x0B1D
        and    word ptr [0xb1b], 0xfffb      ; -> obj9:0x0B1B
        and    word ptr [0xb1b], 0xfff7      ; -> obj9:0x0B1B
        or     word ptr [0xb1b], 4           ; -> obj9:0x0B1B
        cmp    byte ptr [0xb1d], 0           ; -> obj9:0x0B1D
        jne    loc_9_0D8D
        call   sub_9_0D97
        jb     loc_9_0D8D
        test   word ptr [0xb1b], 2           ; -> obj9:0x0B1B
        je     loc_9_0D4C
        mov    edi, 0x798                    ; -> loc_9_0798
        mov    esi, 0xb0e                    ; -> loc_9_0B0E
        call   sub_9_1CDB
        je     loc_9_0D4C
        mov    eax, 0x798                    ; -> loc_9_0798
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   2
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Load_DLVxDs  ; cd202f003300
        add    esp, 0x10
        and    word ptr [0xb1e], 0xfffd      ; -> loc_9_0B1E
        cmp    eax, 0
        je     loc_9_0C66
        or     word ptr [0xb1e], 2           ; -> loc_9_0B1E
        and    word ptr [0xb1b], 0xff7f      ; -> obj9:0x0B1B
        mov    eax, 0x24
        jmp    loc_9_0D92
loc_9_0C66:
        xor    eax, eax
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_9_0C7D
        call   dword ptr [edi + 0xa8]
loc_9_0C7D:
        pop    edi
        push   eax
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, 0x995                    ; -> obj9:0x0995
        mov    edx, 0x7e0                    ; -> loc_9_07E0
        mov    dword ptr [0x99c], 6          ; -> obj9:0x099C
        mov    esi, 0x99c                    ; -> obj9:0x099C
        push   1
        push   esi
        push   edx
        push   3
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        pop    ecx
        cmp    eax, 0
        jne    loc_9_0D07
        and    word ptr [0xb1e], 0xfffb      ; -> loc_9_0B1E
        cmp    ecx, dword ptr [0x7e0]        ; -> loc_9_07E0
        je     loc_9_0D4C
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, 0x7e0                    ; -> loc_9_07E0
        lea    ecx, [edx + 4]
        mov    dword ptr [ecx], 4
        push   0
        push   ecx
        push   ecx
        push   edx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044003300
        add    esp, 0x14
        pop    ecx
        cmp    dword ptr [0x7e0], 1          ; -> loc_9_07E0
        ja     loc_9_0D07
        and    word ptr [0xb1b], 0xffdf      ; -> obj9:0x0B1B
        or     word ptr [0xb1e], 4           ; -> loc_9_0B1E
loc_9_0D07:
        mov    dword ptr [0x7e0], ecx        ; -> loc_9_07E0
        mov    dword ptr [0x7e4], 0          ; -> loc_9_07E4
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, 0x995                    ; -> obj9:0x0995
        mov    edx, 0x7e0                    ; -> loc_9_07E0
        push   1
        push   6
        push   edx
        push   3
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        mov    eax, 0x24
        test   word ptr [0xb1b], 0x20        ; -> obj9:0x0B1B
        je     loc_9_0D92
loc_9_0D4C:
        test   word ptr [0xb1b], 0x400       ; -> obj9:0x0B1B
        je     loc_9_0D8D
        mov    edi, 0x7bc                    ; -> loc_9_07BC
        mov    esi, 0xb0e                    ; -> loc_9_0B0E
        call   sub_9_1CDB
        je     loc_9_0D8D
        mov    eax, 0x7bc                    ; -> loc_9_07BC
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   2
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Load_DLVxDs  ; cd202f003300
        add    esp, 0x10
        cmp    eax, 0
        jne    loc_9_0D8D
        or     word ptr [0xb1e], 0x400       ; -> loc_9_0B1E
loc_9_0D8D:
        mov    eax, 0
loc_9_0D92:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_9_0D97:
        cmp    dword ptr [0x65c], 0          ; -> obj9:0x065C
        je     loc_9_1444
        and    word ptr [0xb1b], 0xfffc      ; -> obj9:0x0B1B
        mov    eax, 0x668                    ; -> obj9:0x0668
        push   4
        push   eax
        push   0
        push   dword ptr [0x65c]             ; -> obj9:0x065C
        call   sub_14_0A30
        add    esp, 0x10
        cmp    eax, 0
        je     loc_9_0E1F
        mov    byte ptr [0x66c], 0x34        ; -> obj9:0x066C
        mov    byte ptr [0x66d], 0           ; -> obj9:0x066D
        mov    esi, 0xb13                    ; -> loc_9_0B13
        mov    edi, 0x694                    ; -> loc_9_0694
        mov    ecx, 8
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [0x680], 0x3639      ; -> loc_9_0680
        mov    byte ptr [0x682], 0           ; -> loc_9_0682
        mov    dword ptr [0x8f4], 0xffffffff ; -> obj9:0x08F4
        mov    eax, dword ptr [0x65c]        ; -> obj9:0x065C
        cmp    eax, dword ptr [0x63c]        ; -> obj9:0x063C
        je     loc_9_119C
        mov    byte ptr [0x66c], 0x38        ; -> obj9:0x066C
        jmp    loc_9_109D
loc_9_0E1F:
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x915                    ; -> loc_9_0915
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x66c                    ; -> obj9:0x066C
        mov    dword ptr [0x67c], 0x10       ; -> loc_9_067C
        mov    edi, 0x67c                    ; -> loc_9_067C
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        and    word ptr [0xb1e], 0xfff7      ; -> loc_9_0B1E
        cmp    eax, 0
        je     loc_9_0E67
        or     word ptr [0xb1e], 8           ; -> loc_9_0B1E
loc_9_0E67:
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x922                    ; -> loc_9_0922
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x694                    ; -> loc_9_0694
        mov    dword ptr [0x6b4], 0x20       ; -> loc_9_06B4
        mov    edi, 0x6b4                    ; -> loc_9_06B4
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        and    word ptr [0xb1e], 0xffef      ; -> loc_9_0B1E
        cmp    eax, 0
        je     loc_9_0EAF
        or     word ptr [0xb1e], 0x10        ; -> loc_9_0B1E
loc_9_0EAF:
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x92d                    ; -> loc_9_092D
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x680                    ; -> loc_9_0680
        mov    dword ptr [0x690], 0x10       ; -> loc_9_0690
        mov    edi, 0x690                    ; -> loc_9_0690
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        and    word ptr [0xb1e], 0xffdf      ; -> loc_9_0B1E
        cmp    eax, 0
        je     loc_9_0EF7
        or     word ptr [0xb1e], 0x20        ; -> loc_9_0B1E
loc_9_0EF7:
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x95e                    ; -> obj9:0x095E
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x6f4                    ; -> loc_9_06F4
        mov    dword ptr [0x664], 0x20       ; -> obj9:0x0664
        mov    edi, 0x664                    ; -> obj9:0x0664
        mov    byte ptr [0x6f4], 0           ; -> loc_9_06F4
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x952                    ; -> loc_9_0952
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x734                    ; -> loc_9_0734
        mov    dword ptr [0x664], 0x20       ; -> obj9:0x0664
        mov    edi, 0x664                    ; -> obj9:0x0664
        mov    byte ptr [0x734], 0           ; -> loc_9_0734
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x947                    ; -> obj9:0x0947
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x714                    ; -> loc_9_0714
        mov    dword ptr [0x664], 0x20       ; -> obj9:0x0664
        mov    edi, 0x664                    ; -> obj9:0x0664
        mov    byte ptr [0x714], 0           ; -> loc_9_0714
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0x96b                    ; -> loc_9_096B
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x754                    ; -> loc_9_0754
        mov    dword ptr [0x664], 0x20       ; -> obj9:0x0664
        mov    edi, 0x664                    ; -> obj9:0x0664
        mov    byte ptr [0x754], 0           ; -> loc_9_0754
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        mov    ebx, 0xb45                    ; -> obj9:0x0B45
        mov    ecx, 0x660                    ; -> obj9:0x0660
        mov    edx, 0x7e0                    ; -> loc_9_07E0
        mov    dword ptr [0x664], 0x80       ; -> obj9:0x0664
        mov    edi, 0x664                    ; -> obj9:0x0664
        mov    byte ptr [0x7e0], 0           ; -> loc_9_07E0
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    ecx, 0
        mov    esi, 0x7e0                    ; -> loc_9_07E0
        cmp    byte ptr [esi], 0
        je     loc_9_102D
        call   sub_9_1D21
loc_9_102D:
        mov    dword ptr [0xb56], ecx        ; -> obj9:0x0B56
        mov    eax, dword ptr [0x668]        ; -> obj9:0x0668
        push   eax
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0x939                    ; -> loc_9_0939
        mov    edx, 0x6e8                    ; -> loc_9_06E8
        mov    dword ptr [0x6f0], 8          ; -> loc_9_06F0
        mov    esi, 0x6f0                    ; -> loc_9_06F0
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 0x800       ; -> obj9:0x0B1B
        cmp    eax, 0
        jne    loc_9_109D
        mov    esi, 0x6e8                    ; -> loc_9_06E8
        call   sub_9_1D21
        or     ecx, ecx
        jne    loc_9_109D
        and    word ptr [0xb1b], 0xf7ff      ; -> obj9:0x0B1B
loc_9_109D:
        mov    dword ptr [0x8f4], 0          ; -> obj9:0x08F4
        cmp    word ptr [ebp + 0x1c], 0x8085
        je     loc_9_119C
        mov    eax, 0x7e0                    ; -> loc_9_07E0
        mov    esi, 0x9a0                    ; -> obj9:0x09A0
        mov    edi, 0x66c                    ; -> obj9:0x066C
        call   sub_9_1CB5
        mov    byte ptr [ecx + eax], 0x5c
        mov    byte ptr [ecx + eax + 1], 0
        mov    esi, eax
        mov    edi, 0x694                    ; -> loc_9_0694
        call   sub_9_1CB5
        mov    eax, 0x7e0                    ; -> loc_9_07E0
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0xb39                    ; -> loc_9_0B39
        mov    edx, 0x6b8                    ; -> loc_9_06B8
        mov    dword ptr [0x6d8], 0x20       ; -> loc_9_06D8
        mov    esi, 0x6d8                    ; -> loc_9_06D8
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        cmp    eax, 0
        je     loc_9_115C
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0xb39                    ; -> loc_9_0B39
        mov    edx, 0x6b8                    ; -> loc_9_06B8
        mov    dword ptr [0x6d8], 0x20       ; -> loc_9_06D8
        mov    esi, 0x6d8                    ; -> loc_9_06D8
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        mov    dword ptr [0x8f4], 0xffffffff ; -> obj9:0x08F4
        cmp    eax, 0
        jne    loc_9_119C
loc_9_115C:
        mov    dword ptr [0x8f4], 0          ; -> obj9:0x08F4
        mov    esi, 0x9af                    ; -> obj9:0x09AF
        mov    edi, 0x6b8                    ; -> loc_9_06B8
        call   sub_9_1CDB
        je     loc_9_119C
        mov    dword ptr [0x8f4], 0xffffffff ; -> obj9:0x08F4
        mov    esi, 0x6b8                    ; -> loc_9_06B8
        call   sub_9_1D08
        jb     loc_9_119C
        call   sub_9_1D21
        or     ecx, ecx
        je     loc_9_119C
        mov    dword ptr [0x8f4], ecx        ; -> obj9:0x08F4
loc_9_119C:
        mov    dword ptr [0x774], 0          ; -> loc_9_0774
        mov    eax, dword ptr [0x65c]        ; -> obj9:0x065C
        or     eax, eax
        je     loc_9_13EB
        cmp    eax, dword ptr [0x63c]        ; -> obj9:0x063C
        jne    loc_9_12D5
        test   word ptr [0xb1e], 0x18        ; -> loc_9_0B1E
        jne    loc_9_12D5
        mov    eax, 0x7e0                    ; -> loc_9_07E0
        mov    esi, 0x9a0                    ; -> obj9:0x09A0
        mov    edi, 0x66c                    ; -> obj9:0x066C
        call   sub_9_1CB5
        mov    byte ptr [ecx + eax], 0x5c
        mov    byte ptr [ecx + eax + 1], 0
        mov    esi, eax
        mov    edi, 0x694                    ; -> loc_9_0694
        call   sub_9_1CB5
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0x97c                    ; -> loc_9_097C
        mov    edx, 0x774                    ; -> loc_9_0774
        mov    dword ptr [0x794], 0x20       ; -> loc_9_0794
        mov    esi, 0x794                    ; -> loc_9_0794
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 1           ; -> obj9:0x0B1B
        cmp    eax, 0
        je     loc_9_123D
        and    word ptr [0xb1b], 0xfffe      ; -> obj9:0x0B1B
loc_9_123D:
        mov    eax, 0x7e0                    ; -> loc_9_07E0
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0x985                    ; -> loc_9_0985
        mov    edx, 0x798                    ; -> loc_9_0798
        mov    dword ptr [0x7b8], 0x20       ; -> loc_9_07B8
        mov    esi, 0x7b8                    ; -> loc_9_07B8
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 2           ; -> obj9:0x0B1B
        cmp    eax, 0
        je     loc_9_1288
        and    word ptr [0xb1b], 0xfffd      ; -> obj9:0x0B1B
loc_9_1288:
        mov    eax, 0x7e0                    ; -> loc_9_07E0
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0x98d                    ; -> obj9:0x098D
        mov    edx, 0x7bc                    ; -> loc_9_07BC
        mov    dword ptr [0x7dc], 0x20       ; -> loc_9_07DC
        mov    esi, 0x7dc                    ; -> loc_9_07DC
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 0x400       ; -> obj9:0x0B1B
        cmp    eax, 0
        je     loc_9_12D5
        and    word ptr [0xb1b], 0xfbff      ; -> obj9:0x0B1B
loc_9_12D5:
        test   word ptr [0xb1b], 1           ; -> obj9:0x0B1B
        jne    loc_9_133D
        mov    esi, 0x980                    ; -> obj9:0x0980
        mov    eax, dword ptr [0x65c]        ; -> obj9:0x065C
        cmp    eax, dword ptr [0x63c]        ; -> obj9:0x063C
        jne    loc_9_12F7
        mov    esi, 0x97c                    ; -> loc_9_097C
loc_9_12F7:
        mov    edx, 0x774                    ; -> loc_9_0774
        mov    dword ptr [0x794], 0x20       ; -> loc_9_0794
        mov    ecx, 0x794                    ; -> loc_9_0794
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        push   1
        push   ecx
        push   edx
        push   1
        push   esi
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 1           ; -> obj9:0x0B1B
        cmp    eax, 0
        je     loc_9_133D
        and    word ptr [0xb1b], 0xfffe      ; -> obj9:0x0B1B
loc_9_133D:
        test   word ptr [0xb1b], 2           ; -> obj9:0x0B1B
        jne    loc_9_1393
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0x985                    ; -> loc_9_0985
        mov    edx, 0x798                    ; -> loc_9_0798
        mov    dword ptr [0x7b8], 0x20       ; -> loc_9_07B8
        mov    esi, 0x7b8                    ; -> loc_9_07B8
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 2           ; -> obj9:0x0B1B
        cmp    eax, 0
        je     loc_9_1393
        and    word ptr [0xb1b], 0xfffd      ; -> obj9:0x0B1B
loc_9_1393:
        test   word ptr [0xb1b], 0x400       ; -> obj9:0x0B1B
        jne    loc_9_13EB
        mov    eax, 0x9a7                    ; -> obj9:0x09A7
        mov    ebx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    ecx, 0x98d                    ; -> obj9:0x098D
        mov    edx, 0x7bc                    ; -> loc_9_07BC
        mov    dword ptr [0x7dc], 0x20       ; -> loc_9_07DC
        mov    esi, 0x7dc                    ; -> loc_9_07DC
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     word ptr [0xb1b], 0x400       ; -> obj9:0x0B1B
        cmp    eax, 0
        je     loc_9_13EB
        and    word ptr [0xb1b], 0xfbff      ; -> obj9:0x0B1B
loc_9_13EB:
        or     word ptr [0xb1e], 0x180       ; -> loc_9_0B1E
        mov    eax, dword ptr [0x8f4]        ; -> obj9:0x08F4
        or     eax, eax
        js     loc_9_1441
        and    word ptr [0xb1e], 0xfeff      ; -> loc_9_0B1E
        or     word ptr [0xb1e], 0x200       ; -> loc_9_0B1E
        or     eax, eax
        jne    loc_9_1441
        and    word ptr [0xb1e], 0xfdff      ; -> loc_9_0B1E
        or     word ptr [0xb1e], 0x40        ; -> loc_9_0B1E
        push   dword ptr [0x65c]             ; -> obj9:0x065C
        call   sub_9_0B5A
        pop    ecx
        or     eax, eax
        je     loc_9_1441
        and    word ptr [0xb1e], 0xffbf      ; -> loc_9_0B1E
        call   sub_9_1446
loc_9_1441:
        clc    
        jmp    loc_9_1445
loc_9_1444:
        stc    
loc_9_1445:
        ret    
sub_9_1446:
        mov    byte ptr [0xb20], 0           ; -> loc_9_0B20
        mov    byte ptr [0xb21], 0           ; -> obj9:0x0B21
        mov    dword ptr [0x8e4], 0          ; -> obj9:0x08E4
        mov    dword ptr [0x8e8], 0          ; -> obj9:0x08E8
        mov    dword ptr [0x8ec], 0          ; -> obj9:0x08EC
        mov    dword ptr [0x8f0], 0          ; -> obj9:0x08F0
        mov    dword ptr [0x8f4], 0          ; -> obj9:0x08F4
        mov    dword ptr [0x8f8], 0xffffffff ; -> obj9:0x08F8
        mov    ebx, eax
        test   word ptr [0xb1e], 0x10        ; -> loc_9_0B1E
        jne    loc_9_17C2
        mov    esi, 0x694                    ; -> loc_9_0694
        call   sub_9_1D58
        mov    dword ptr [0x8fc], edx        ; -> obj9:0x08FC
        mov    dword ptr [0x900], ecx        ; -> obj9:0x0900
        mov    ecx, 0x9bb                    ; -> obj9:0x09BB
        mov    edx, 0x9c6                    ; -> loc_9_09C6
        mov    edi, 0x100
        push   1
        push   edi
        push   edx
        push   ecx
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_9_17C2
        mov    eax, 0x9c6                    ; -> loc_9_09C6
        mov    ecx, 0x9b2                    ; -> obj9:0x09B2
        push   ecx
        push   eax
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_9_1784
        xor    ecx, ecx
        mov    dword ptr [0xae6], 0xffffffff ; -> obj9:0x0AE6
        mov    edx, 0xac6                    ; -> loc_9_0AC6
loc_9_1512:
        push   ecx
        push   edx
        push   0x20
        push   edx
        push   ecx
        push   dword ptr [0x9b2]             ; -> obj9:0x09B2
        VMMCall  VMM._RegEnumKey                 ; cd204c010100
        add    esp, 0x10
        pop    edx
        pop    ecx
        cmp    eax, 0
        jne    loc_9_1572
        push   ecx
        mov    esi, 0xac6                    ; -> loc_9_0AC6
        mov    cl, 0x2c
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    eax, dword ptr [0x8fc]        ; -> obj9:0x08FC
        mov    esi, ecx
        pop    ecx
        or     esi, esi
        je     loc_9_1569
        sub    esi, eax
        jns    loc_9_1555
        neg    esi
loc_9_1555:
        cmp    esi, dword ptr [0xae6]        ; -> obj9:0x0AE6
        jae    loc_9_1569
        mov    dword ptr [0xae6], esi        ; -> obj9:0x0AE6
        mov    dword ptr [0xaea], ecx        ; -> obj9:0x0AEA
loc_9_1569:
        inc    ecx
        cmp    ecx, 0x100
        jb     loc_9_1512
loc_9_1572:
        cmp    dword ptr [0xae6], -1         ; -> obj9:0x0AE6
        je     loc_9_1597
        mov    ecx, dword ptr [0xaea]        ; -> obj9:0x0AEA
        push   0x20
        push   edx
        push   ecx
        push   dword ptr [0x9b2]             ; -> obj9:0x09B2
        VMMCall  VMM._RegEnumKey                 ; cd204c010100
        add    esp, 0x10
loc_9_1597:
        push   dword ptr [0x9b2]             ; -> obj9:0x09B2
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        cmp    dword ptr [0xae6], -1         ; -> obj9:0x0AE6
        je     loc_9_17C2
        mov    esi, 0x9a0                    ; -> obj9:0x09A0
        mov    edi, 0xac6                    ; -> loc_9_0AC6
        mov    eax, 0x9c6                    ; -> loc_9_09C6
        call   sub_9_1CB5
        mov    byte ptr [0x9c1], 1           ; -> obj9:0x09C1
loc_9_15CE:
        mov    esi, 0x9c1                    ; -> obj9:0x09C1
        mov    edi, 0x9c2                    ; -> loc_9_09C2
        mov    ecx, 1
        test   byte ptr [0x9c1], 0xf0        ; -> obj9:0x09C1
        je     loc_9_15E7
        inc    ecx
loc_9_15E7:
        call   sub_9_1D6E
        mov    esi, 0x9b6                    ; -> loc_9_09B6
        mov    edi, 0x9c2                    ; -> loc_9_09C2
        mov    eax, 0xaee                    ; -> obj9:0x0AEE
        call   sub_9_1CB5
        mov    dword ptr [0x7e0], 0          ; -> loc_9_07E0
        mov    ecx, 0x9c6                    ; -> loc_9_09C6
        mov    edx, 0x7e0                    ; -> loc_9_07E0
        mov    dword ptr [0x860], 0x80       ; -> loc_9_0860
        mov    esi, 0x860                    ; -> loc_9_0860
        push   1
        push   esi
        push   edx
        push   1
        push   eax
        push   ecx
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        cmp    eax, 0
        jne    loc_9_175D
        cmp    dword ptr [0x7e0], 0          ; -> loc_9_07E0
        je     loc_9_175D
        mov    esi, 0x7e0                    ; -> loc_9_07E0
        mov    cl, 0x2c
        call   sub_9_1DAB
        push   edi
        mov    cl, 0x2d
        call   sub_9_1DC2
        jb     loc_9_167A
        call   sub_9_1DAB
        push   edi
        mov    cl, 0x2e
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    dword ptr [0x8e4], ecx        ; -> obj9:0x08E4
        pop    esi
loc_9_167A:
        mov    cl, 0x2e
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    dword ptr [0x8e8], ecx        ; -> obj9:0x08E8
        pop    esi
        mov    cl, 0x2c
        call   sub_9_1DAB
        mov    al, byte ptr [edi]
        mov    byte ptr [0xb20], al          ; -> loc_9_0B20
        mov    al, byte ptr [edi + 2]
        mov    byte ptr [0xb21], al          ; -> obj9:0x0B21
        mov    cl, 0x2d
        call   sub_9_1DC2
        jb     loc_9_16C5
        call   sub_9_1DAB
        push   edi
        mov    cl, 0x2e
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    dword ptr [0x8ec], ecx        ; -> obj9:0x08EC
        pop    esi
loc_9_16C5:
        mov    cl, 0x2e
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    dword ptr [0x8f0], ecx        ; -> obj9:0x08F0
        mov    eax, dword ptr [0x900]        ; -> obj9:0x0900
        mov    ecx, dword ptr [0xb56]        ; -> obj9:0x0B56
        cmp    ecx, 0x64
        jae    loc_9_16F8
        mov    ecx, 0x69
        cmp    eax, 0x258
        jbe    loc_9_16F8
        mov    ecx, 0x6c
loc_9_16F8:
        mul    ecx
        mov    ecx, eax
        mov    eax, dword ptr [0x8e8]        ; -> obj9:0x08E8
        mov    edx, 0x186a0
        mul    edx
        div    ecx
        mov    edx, dword ptr [0x8f0]        ; -> obj9:0x08F0
        cmp    eax, edx
        jbe    loc_9_1716
        mov    eax, edx
loc_9_1716:
        cmp    eax, dword ptr [0x8f4]        ; -> obj9:0x08F4
        jbe    loc_9_1723
        mov    dword ptr [0x8f4], eax        ; -> obj9:0x08F4
loc_9_1723:
        mov    eax, dword ptr [0x8e4]        ; -> obj9:0x08E4
        mov    edx, 0x186a0
        mul    edx
        div    ecx
        mov    edx, dword ptr [0x8ec]        ; -> obj9:0x08EC
        cmp    eax, edx
        jae    loc_9_173D
        mov    eax, edx
loc_9_173D:
        cmp    eax, dword ptr [0x8f8]        ; -> obj9:0x08F8
        jae    loc_9_174A
        mov    dword ptr [0x8f8], eax        ; -> obj9:0x08F8
loc_9_174A:
        inc    byte ptr [0x9c1]              ; -> obj9:0x09C1
        cmp    byte ptr [0x9c1], 0xff        ; -> obj9:0x09C1
        jb     loc_9_15CE
loc_9_175D:
        cmp    dword ptr [0x8f8], -1         ; -> obj9:0x08F8
        je     loc_9_1784
        mov    eax, dword ptr [0x8f4]        ; -> obj9:0x08F4
        cmp    eax, dword ptr [0x8f8]        ; -> obj9:0x08F8
        jae    loc_9_1778
        mov    dword ptr [0x8f8], eax        ; -> obj9:0x08F8
loc_9_1778:
        and    word ptr [0xb1e], 0xff7f      ; -> loc_9_0B1E
        clc    
        jmp    loc_9_17CC
loc_9_1784:
        test   word ptr [0xb1b], 8           ; -> obj9:0x0B1B
        je     loc_9_17C2
        lea    eax, [0x8f8]                  ; -> obj9:0x08F8
        lea    edx, [0x8f4]                  ; -> obj9:0x08F4
        push   edx
        push   eax
        push   dword ptr [0x900]             ; -> obj9:0x0900
        push   dword ptr [0x8fc]             ; -> obj9:0x08FC
        push   ebx
        call   sub_14_137D
        add    esp, 0x14
        or     eax, eax
        je     loc_9_17C2
        or     word ptr [0xb1e], 0x280       ; -> loc_9_0B1E
        clc    
        jmp    loc_9_17CC
loc_9_17C2:
        or     word ptr [0xb1e], 0x80        ; -> loc_9_0B1E
        stc    
loc_9_17CC:
        ret    
loc_9_17CD:
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, 0x22
        jl     loc_9_182C
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_9_182C
        mov    ebx, dword ptr [ebp + 0x10]
        test   ebx, 0xffff0000
        jne    loc_9_17FC
        dec    ebx
        cmp    ebx, 8
        jae    loc_9_182C
        mov    ebx, dword ptr [ebx*4 + 0x63c] ; -> obj9:0x063C
loc_9_17FC:
        or     ebx, ebx
        je     loc_9_182C
        mov    dword ptr [0x65c], ebx        ; -> obj9:0x065C
        mov    ecx, dword ptr [ebp + 0x18]
        call   sub_9_184A
        jb     loc_9_182C
        test   word ptr [eax + 2], 6
        jne    loc_9_182C
        test   word ptr [0xb1b], 1           ; -> obj9:0x0B1B
        je     loc_9_182C
        mov    dword ptr [ebp + 0x1c], 0
        jmp    loc_9_1833
loc_9_182C:
        mov    dword ptr [ebp + 0x1c], 0xffffffff
loc_9_1833:
        ret    
VDD_Get_DISPLAYINFO:
        cmp    ecx, 0x22
        jl     loc_9_1844
        call   sub_9_184A
        jb     loc_9_1844
        xor    eax, eax
        jmp    loc_9_1849
loc_9_1844:
        mov    eax, 0xffffffff
loc_9_1849:
        ret    
sub_9_184A:
        cld    
        push   eax
        push   ecx
        or     word ptr [0xb1e], 1           ; -> loc_9_0B1E
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        pop    ecx
        pop    eax
        cmp    dword ptr [edi + 0x24], 0
        jne    loc_9_187D
        and    word ptr [0xb1e], 0xfffe      ; -> loc_9_0B1E
        push   eax
        push   ecx
        mov    word ptr [eax], cx
        call   sub_9_0D97
        pop    ecx
        pop    eax
        jb     loc_9_1A5A
loc_9_187D:
        mov    word ptr [eax], cx
        mov    cx, word ptr [0xb1e]          ; -> loc_9_0B1E
        mov    word ptr [eax + 2], cx
        mov    ecx, dword ptr [0x65c]        ; -> obj9:0x065C
        mov    dword ptr [eax + 4], ecx
        test   word ptr [0xb1b], 1           ; -> obj9:0x0B1B
        je     loc_9_18AE
        lea    edi, [eax + 8]
        mov    esi, 0x774                    ; -> loc_9_0774
        mov    ecx, 0x10
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_9_18AE:
        test   word ptr [eax + 2], 8
        jne    loc_9_18C3
        mov    esi, 0x66c                    ; -> obj9:0x066C
        call   sub_9_1D21
        mov    byte ptr [eax + 0x1f], cl
loc_9_18C3:
        mov    byte ptr [eax + 0x1e], 1
        test   word ptr [eax + 2], 0x10
        jne    loc_9_18E1
        mov    esi, 0x694                    ; -> loc_9_0694
        call   sub_9_1D58
        mov    word ptr [eax + 0x18], dx
        mov    word ptr [eax + 0x1a], cx
loc_9_18E1:
        mov    word ptr [eax + 0x1c], 0x60
        test   word ptr [eax + 2], 0x20
        jne    loc_9_18FD
        mov    esi, 0x680                    ; -> loc_9_0680
        call   sub_9_1D21
        mov    word ptr [eax + 0x1c], cx
loc_9_18FD:
        cmp    word ptr [eax], 0x4a
        jb     loc_9_1978
        mov    esi, 0x6f4                    ; -> loc_9_06F4
        call   sub_9_1D21
        mov    dword ptr [eax + 0x36], ecx
        mov    esi, 0x714                    ; -> loc_9_0714
        call   sub_9_1D58
        mov    dword ptr [eax + 0x3a], edx
        mov    dword ptr [eax + 0x3e], ecx
        movzx  edx, word ptr [eax + 0x18]
        movzx  ecx, word ptr [eax + 0x1a]
        mov    dword ptr [eax + 0x42], edx
        mov    dword ptr [eax + 0x46], ecx
        mov    esi, 0x734                    ; -> loc_9_0734
        cmp    byte ptr [esi], 0
        je     loc_9_194F
        call   sub_9_1D58
        cmp    word ptr [eax + 0x18], dx
        ja     loc_9_194F
        cmp    word ptr [eax + 0x1a], cx
        ja     loc_9_194F
        mov    dword ptr [eax + 0x42], edx
        mov    dword ptr [eax + 0x46], ecx
loc_9_194F:
        mov    esi, 0x754                    ; -> loc_9_0754
        cmp    byte ptr [esi], 0
        je     loc_9_1978
        call   sub_9_1D58
        cmp    word ptr [eax + 0x18], dx
        jb     loc_9_1978
        cmp    word ptr [eax + 0x1a], cx
        jb     loc_9_1978
        xchg   word ptr [eax + 0x18], dx
        xchg   word ptr [eax + 0x1a], cx
        mov    dword ptr [eax + 0x42], edx
        mov    dword ptr [eax + 0x46], ecx
loc_9_1978:
        cmp    word ptr [eax], 0x32
        jb     loc_9_1A2C
        mov    word ptr [eax + 0x20], 0
        mov    word ptr [eax + 0x22], 0
        mov    word ptr [eax + 0x24], 0
        mov    word ptr [eax + 0x26], 0
        mov    word ptr [eax + 0x28], 0
        mov    word ptr [eax + 0x2a], 0
        mov    byte ptr [eax + 0x30], 0
        mov    byte ptr [eax + 0x31], 0
        mov    ebx, eax
        push   dword ptr [0x65c]             ; -> obj9:0x065C
        call   sub_9_0B5A
        pop    ecx
        xchg   ebx, eax
        mov    dword ptr [eax + 0x2c], ebx
        test   word ptr [eax + 2], 0x100
        jne    loc_9_1A2C
        mov    cx, word ptr [0x8f4]          ; -> obj9:0x08F4
        mov    word ptr [eax + 0x20], cx
        test   word ptr [eax + 2], 0x200
        jne    loc_9_1A2C
        test   word ptr [eax + 2], 0x80
        jne    loc_9_1A2C
        mov    cx, word ptr [0x8f8]          ; -> obj9:0x08F8
        mov    word ptr [eax + 0x22], cx
        mov    cx, word ptr [0x8e4]          ; -> obj9:0x08E4
        mov    word ptr [eax + 0x24], cx
        mov    cx, word ptr [0x8e8]          ; -> obj9:0x08E8
        mov    word ptr [eax + 0x26], cx
        mov    cx, word ptr [0x8ec]          ; -> obj9:0x08EC
        mov    word ptr [eax + 0x28], cx
        mov    cx, word ptr [0x8f0]          ; -> obj9:0x08F0
        mov    word ptr [eax + 0x2a], cx
        mov    cl, byte ptr [0xb20]          ; -> loc_9_0B20
        mov    byte ptr [eax + 0x30], cl
        mov    cl, byte ptr [0xb21]          ; -> obj9:0x0B21
        mov    byte ptr [eax + 0x31], cl
loc_9_1A2C:
        test   word ptr [eax + 2], 1
        jne    loc_9_1A4F
        mov    edx, dword ptr [0x63c]        ; -> obj9:0x063C
        cmp    edx, dword ptr [0x65c]        ; -> obj9:0x065C
        je     loc_9_1A4F
        mov    dword ptr [0x65c], edx        ; -> obj9:0x065C
        push   eax
        call   sub_9_0D97
        pop    eax
loc_9_1A4F:
        test   word ptr [eax + 2], 0x18
        jne    loc_9_1A5A
        clc    
        jmp    loc_9_1A5B
loc_9_1A5A:
        stc    
loc_9_1A5B:
        ret    
sub_9_1A5C:
        enter  0x74, 0
        lea    edi, [ebp - 0x6c]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        mov    eax, 0x1388
        mov    edx, ebp
        mov    esi, 0x1aac                   ; -> loc_9_1AAC
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        VMMCall  VMM.Set_Thread_Time_Out         ; cd20fd000100
        mov    dword ptr [ebp - 0x70], esi
        xor    eax, eax
        mov    dword ptr [ebp - 0x74], eax
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        xor    esi, esi
        xchg   dword ptr [ebp - 0x70], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        xor    esi, esi
        xchg   dword ptr [ebp - 0x74], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        leave  
        ret    
loc_9_1AAC:
        mov    ebp, edx
        mov    ebx, edi
        xor    eax, eax
        mov    dword ptr [ebp - 0x70], eax
        mov    ecx, 0x120
        mov    esi, 0x1ac9                   ; -> loc_9_1AC9
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 0x74], esi
        ret    
loc_9_1AC9:
        mov    ebp, edx
        mov    dword ptr [ebp - 0x74], 0
        lea    esi, [ebp - 0x6c]
        mov    dword ptr [esi + 0x1c], 0xffffffff
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        ret    
sub_9_1AE3:
        push   ebx
        push   esi
        push   edi
        push   ebp
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp], 0
        mov    word ptr [ebp + 0x38], 0
        mov    word ptr [ebp + 0x1c], 0x4f15
        mov    byte ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x18], 0
        call   sub_9_1A5C
        mov    al, byte ptr [ebp + 0x10]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    ax, 0x4f
        jne    loc_9_1CA4
        test   eax, 0x30000
        je     loc_9_1CAB
        mov    ecx, 0x80
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_9_1B7E
        xor    eax, eax
        cmp    ecx, 0x80
        jae    loc_9_1BAA
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        jmp    loc_9_1CAB
loc_9_1B7E:
        test   word ptr [0xb1b], 0x40        ; -> obj9:0x0B1B
        jne    loc_9_1CAB
        push   0
        push   0x80
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        or     eax, eax
        je     loc_9_1CAB
        mov    edi, eax
        shl    edi, 0xc
loc_9_1BAA:
        push   eax
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp], di
        ror    edi, 0x10
        mov    word ptr [ebp + 0x38], di
        mov    word ptr [ebp + 0x1c], 0x4f15
        mov    byte ptr [ebp + 0x10], 1
        mov    word ptr [ebp + 0x18], 0
        mov    word ptr [ebp + 0x14], 0
        call   sub_9_1A5C
        mov    ax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        or     esi, esi
        je     loc_9_1C1A
        cld    
        mov    edi, dword ptr [esp + 0x14]
        push   eax
        mov    ecx, 0x20
        rep movsd dword ptr es:[edi], dword ptr [esi]
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
        pop    eax
        jmp    loc_9_1C32
loc_9_1C1A:
        mov    esi, dword ptr [esp + 0x14]
        push   eax
        mov    ecx, 0x80
        mov    ax, ds
        mov    fs, ax
        stc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        pop    eax
loc_9_1C32:
        cmp    ax, -1
        je     loc_9_1CA4
        cmp    ax, 0x4f
        jne    loc_9_1CAB
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi]
        cmp    eax, 0xffffff00
        jne    loc_9_1CAB
        mov    eax, dword ptr [esi + 4]
        cmp    eax, 0xffffff
        jne    loc_9_1CAB
        mov    ax, word ptr [esi + 8]
        mov    cl, al
        shr    cl, 2
        and    cl, 0x1f
        add    cl, 0x40
        mov    byte ptr [edi], cl
        mov    cx, ax
        xchg   ch, cl
        shr    cx, 5
        and    cl, 0x1f
        add    cl, 0x40
        mov    byte ptr [edi + 1], cl
        and    ah, 0x1f
        add    ah, 0x40
        mov    byte ptr [edi + 2], ah
        lea    esi, [esi + 0xa]
        add    edi, 3
        mov    ecx, 4
        call   sub_9_1D6E
        or     word ptr [0xb1b], 8           ; -> obj9:0x0B1B
        mov    eax, 0
        jmp    loc_9_1CB0
loc_9_1CA4:
        mov    eax, 0x20
        jmp    loc_9_1CB0
loc_9_1CAB:
        mov    eax, 0x13
loc_9_1CB0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_9_1CB5:
        cld    
        push   eax
        push   ebx
        push   esi
        push   edi
        xor    ecx, ecx
loc_9_1CBC:
        mov    bl, byte ptr [esi]
        or     bl, bl
        je     loc_9_1CC9
        mov    byte ptr [eax], bl
        inc    eax
        inc    esi
        inc    ecx
        jmp    loc_9_1CBC
loc_9_1CC9:
        mov    bl, byte ptr [edi]
        mov    byte ptr [eax], bl
        or     bl, bl
        je     loc_9_1CD6
        inc    eax
        inc    edi
        inc    ecx
        jmp    loc_9_1CC9
loc_9_1CD6:
        pop    edi
        pop    esi
        pop    ebx
        pop    eax
        ret    
sub_9_1CDB:
        cld    
        push   eax
        push   edi
        push   esi
loc_9_1CDF:
        mov    al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        inc    esi
        inc    edi
        cmp    al, 0x61
        jb     loc_9_1CEF
        cmp    al, 0x7a
        ja     loc_9_1CEF
        and    al, 0xdf
loc_9_1CEF:
        cmp    ah, 0x61
        jb     loc_9_1CFC
        cmp    ah, 0x7a
        ja     loc_9_1CFC
        and    ah, 0xdf
loc_9_1CFC:
        cmp    al, ah
        jne    loc_9_1D04
        or     al, al
        jne    loc_9_1CDF
loc_9_1D04:
        pop    esi
        pop    edi
        pop    eax
        ret    
sub_9_1D08:
        cld    
        push   eax
        push   esi
loc_9_1D0B:
        mov    al, byte ptr [esi]
        inc    esi
        or     al, al
        je     loc_9_1D1D
        cmp    al, 0x30
        jb     loc_9_1D1A
        cmp    al, 0x39
        jbe    loc_9_1D0B
loc_9_1D1A:
        stc    
        jmp    loc_9_1D1E
loc_9_1D1D:
        clc    
loc_9_1D1E:
        pop    esi
        pop    eax
        ret    
sub_9_1D21:
        push   eax
        push   ebx
        push   edx
        push   edi
        xor    ecx, ecx
        xor    bl, bl
loc_9_1D29:
        cmp    byte ptr [esi], 0
        je     loc_9_1D33
        inc    bl
        inc    esi
        jmp    loc_9_1D29
loc_9_1D33:
        or     bl, bl
        je     loc_9_1D53
        dec    esi
        mov    edi, 1
loc_9_1D3D:
        movzx  eax, byte ptr [esi]
        sub    eax, 0x30
        mul    edi
        add    ecx, eax
        dec    bl
        je     loc_9_1D53
        dec    esi
        lea    edi, [edi + edi*4]
        add    edi, edi
        jmp    loc_9_1D3D
loc_9_1D53:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_9_1D58:
        mov    cl, 0x2c
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    edx, ecx
        mov    esi, edi
        call   sub_9_1D21
        ret    
sub_9_1D6E:
        or     ecx, ecx
        je     loc_9_1DAA
        push   eax
        push   esi
        push   edi
        push   ecx
        mov    eax, ecx
        add    edi, eax
        mov    byte ptr [edi], 0
loc_9_1D7D:
        mov    al, byte ptr [esi]
        mov    ah, al
        shr    al, 4
        and    ah, 0xf
        add    ax, 0x3030
        cmp    al, 0x3a
        jb     loc_9_1D91
        add    al, 7
loc_9_1D91:
        cmp    ah, 0x3a
        jb     loc_9_1D99
        add    ah, 7
loc_9_1D99:
        dec    edi
        mov    byte ptr [edi], ah
        dec    ecx
        je     loc_9_1DA6
        dec    edi
        mov    byte ptr [edi], al
        inc    esi
        dec    ecx
        jne    loc_9_1D7D
loc_9_1DA6:
        pop    ecx
        pop    edi
        pop    esi
        pop    eax
loc_9_1DAA:
        ret    
sub_9_1DAB:
        push   esi
        mov    edi, esi
loc_9_1DAE:
        cmp    byte ptr [esi], cl
        je     loc_9_1DBA
        cmp    byte ptr [esi], 0
        je     loc_9_1DBA
        inc    esi
        jmp    loc_9_1DAE
loc_9_1DBA:
        mov    byte ptr [esi], 0
        mov    edi, esi
        inc    edi
        pop    esi
        ret    
sub_9_1DC2:
        push   esi
loc_9_1DC3:
        cmp    byte ptr [esi], cl
        je     loc_9_1DCF
        cmp    byte ptr [esi], 0
        je     loc_9_1DD2
        inc    esi
        jmp    loc_9_1DC3
loc_9_1DCF:
        clc    
        jmp    loc_9_1DD3
loc_9_1DD2:
        stc    
loc_9_1DD3:
        pop    esi
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 10  vsize=0x31c  R-X
loc_10_0000:
        inc    dword ptr [0x794]             ; -> obj1:0x0794
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x34], 0
        je     loc_10_0015
        call   dword ptr [edi + 0x34]
loc_10_0015:
        pop    edi
        or     dword ptr [0x109c], 0x80000000 ; -> obj1:0x109C
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    ax, word ptr [0xeee]          ; -> loc_1_0EEE
        mov    word ptr [edi + 0x5e2], ax
        call   sub_10_0238
        jb     loc_10_0042
        call   sub_10_0070
        clc    
loc_10_0042:
        ret    
loc_10_0043:
        call   sub_10_0050
        or     byte ptr [0x6c4], 4           ; -> obj1:0x06C4
        ret    
sub_10_0050:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_10_0113
        clc    
        ret    
loc_10_005F:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_3509
        clc    
        ret    
        db  000h, 000h
sub_10_0070:
        push   edi
        mov    eax, dword ptr [0x6c8]        ; -> obj1:0x06C8
        mov    dword ptr [edi + 4], eax
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        cld    
        je     loc_10_00A3
        mov    esi, 0x7c8                    ; -> loc_1_07C8
        push   esi
        lea    edi, [edi + 0xc]
        mov    ecx, 0x25
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    edi, dword ptr [esp]
        lea    edi, [edi + 0x4af]
        mov    ecx, 0x25
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_10_00A3:
        mov    edi, dword ptr [esp]
        lea    edi, [edi + 0xa0]
        push   edi
        mov    ecx, 0x100
        xor    eax, eax
loc_10_00B4:
        stosd  dword ptr es:[edi], eax
        inc    eax
        loop   loc_10_00B4
        pop    edi
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_10_00D5
        mov    esi, 0x85c                    ; -> loc_1_085C
        mov    ecx, 0x10
loc_10_00CC:
        lodsd  eax, dword ptr [esi]
        movzx  edx, al
        mov    dword ptr [edi + edx*4], eax
        loop   loc_10_00CC
loc_10_00D5:
        pop    edi
        lea    esi, [edi + 0xa0]
        mov    dword ptr [edi + 0x4a0], esi
        mov    byte ptr [edi + 0x4a4], 1
        mov    byte ptr [edi + 0x4a5], 0
        mov    byte ptr [edi + 0x5e0], 0xc
        mov    byte ptr [edi + 0x5e1], 0xff
        xor    eax, eax
        mov    dword ptr [edi + 0x5e4], eax
        mov    dword ptr [edi + 0x600], eax
        call   sub_15_4BBC
        clc    
        ret    
sub_10_0113:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_10_01D7
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x1202
        cmp    byte ptr [0xe98], 0x2b        ; -> loc_1_0E98
        jne    loc_10_0151
        mov    word ptr [ebp + 0x1c], 0x1201
loc_10_0151:
        mov    byte ptr [ebp + 0x10], 0x30
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    byte ptr [ebp + 0x1c], 0x12
        je     loc_10_016D
        mov    byte ptr [0xe98], 0x19        ; -> loc_1_0E98
loc_10_016D:
        mov    word ptr [ebp + 0x1c], 3
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    byte ptr [0xe98], 0x19        ; -> loc_1_0E98
        je     loc_10_01B3
        mov    word ptr [ebp + 0x1c], 0x1112
        mov    byte ptr [ebp + 0x10], 0
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0x100
        mov    word ptr [ebp + 0x18], 0x407
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_10_01B3:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        test   byte ptr [edi], 1
        je     loc_10_01D7
        mov    dword ptr [edi + 0x5f0], 0x28
loc_10_01D7:
        bt     dword ptr [edi + 8], 8
        ret    
        db  000h, 000h, 000h
VDD_PIF_State:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_10_0208
        push   edi
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    word ptr [edi + 0x5e2], ax
        je     loc_10_0206
        mov    word ptr [edi + 0x5e2], ax
        call   sub_15_4BBC
loc_10_0206:
        pop    edi
        ret    
loc_10_0208:
        mov    word ptr [0xeee], ax          ; -> loc_1_0EEE
        ret    
loc_10_020F:
        VxDCall  VDD.VDD_Get_Version             ; cd2000000a00
        mov    dword ptr [ebp + 0x1c], eax
        movzx  edi, word ptr [ebp + 0x38]
        push   edx
        push   0
        push   edi
        push   edx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        inc    eax
        je     loc_10_0237
        dec    eax
        mov    edi, eax
        add    edi, dword ptr [ebp]
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_10_0237:
        ret    
sub_10_0238:
        mov    eax, dword ptr [0xfdc]        ; -> obj1:0x0FDC
        mov    word ptr [edi + 0x5d0], ax
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_10_025C
        mov    eax, dword ptr [0xed2]        ; -> loc_1_0ED2
        call   sub_15_347C
        jb     loc_10_031A
loc_10_025C:
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   0x10
        push   0xa0
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   8
        push   0xb8
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        test   byte ptr [edi + 4], 4
        je     loc_10_02AD
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   8
        push   0xb0
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
loc_10_02AD:
        push   edi
        mov    eax, 0xa0a0a0a
        test   byte ptr [edi + 0x5e2], 0x40
        jne    loc_10_02D4
        mov    eax, 4
        test   byte ptr [edi + 0x5e2], 0x20
        jne    loc_10_02D4
        mov    eax, 8
        or     eax, 0x20000
loc_10_02D4:
        lea    edi, [edi + 0x543]
        mov    ecx, 4
        xor    esi, esi
loc_10_02E1:
        movzx  edx, al
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi + 8], dx
        mov    word ptr [edi + 0xa], si
        add    edi, 0xc
        shr    eax, 8
        loop   loc_10_02E1
        pop    edi
        lea    esi, [edi + 0x543]
        call   sub_15_37CD
        jb     loc_10_0314
        mov    byte ptr [edi + 0x4a6], 1
        or     byte ptr [edi + 3], 0x80
        clc    
        ret    
loc_10_0314:
        call   sub_15_36C5
        stc    
loc_10_031A:
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 11  vsize=0xe0  R-X
loc_11_0000:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_11_0054
        call   sub_11_0094
        dec    dword ptr [0x794]             ; -> obj1:0x0794
        jne    loc_11_0034
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x38], 0
        je     loc_11_0029
        call   dword ptr [edi + 0x38]
loc_11_0029:
        pop    edi
        and    dword ptr [0x109c], 0x7fffffff ; -> obj1:0x109C
loc_11_0034:
        clc    
        ret    
loc_11_0036:
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_11_0052
        mov    dword ptr [0xf00], 0          ; -> loc_1_0F00
        mov    dword ptr [0xf08], 0          ; -> loc_1_0F08
loc_11_0052:
        clc    
        ret    
sub_11_0054:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_11_006B
        call   sub_15_1802
        mov    dword ptr [0x7a8], 0          ; -> loc_1_07A8
loc_11_006B:
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_11_0093
        push   ebx
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        or     ebx, ebx
        je     loc_11_008D
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_119E
        jmp    loc_11_0092
loc_11_008D:
        call   sub_15_18BA
loc_11_0092:
        pop    ebx
loc_11_0093:
        ret    
sub_11_0094:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        and    byte ptr [edi + 0x5e2], 0x7f
        call   sub_15_4538
        call   sub_15_36C5
        call   sub_15_01B1
        mov    eax, dword ptr [edi + 0x5b8]
        or     eax, eax
        je     loc_11_00C8
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_11_00C8:
        mov    eax, dword ptr [edi + 0x5bc]
        or     eax, eax
        je     loc_11_00DE
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_11_00DE:
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 12  vsize=0x48  R-X
loc_12_0000:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_4877
        call   sub_12_0014
        clc    
        ret    
sub_12_0014:
        mov    ecx, 4
sub_12_0019:
        push   esi
        lea    esi, [edi + 0x543]
loc_12_0020:
        push   ecx
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_12_003D
        movzx  ecx, word ptr [esi + 0xa]
        push   0x100
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_12_003D:
        add    esi, 0xc
        pop    ecx
        loop   loc_12_0020
        pop    esi
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 13  vsize=0x60  R-X
loc_13_0000:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_13_0018
        jb     loc_13_0015
        call   sub_15_4849
        clc    
loc_13_0015:
        ret    
        db  000h, 000h
sub_13_0018:
        push   esi
        lea    esi, [edi + 0x543]
        mov    ecx, 4
loc_13_0024:
        push   ecx
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_13_0045
        movzx  ecx, word ptr [esi + 0xa]
        push   0x100
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_13_004E
loc_13_0045:
        add    esi, 0xc
        pop    ecx
        loop   loc_13_0024
        clc    
        pop    esi
        ret    
loc_13_004E:
        pop    ecx
        neg    ecx
        add    ecx, 4
        jecxz  loc_13_005B
        call   sub_12_0019
loc_13_005B:
        stc    
        pop    esi
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 14  vsize=0x19ce  R-X
sub_14_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 0x12345678
        stosd  dword ptr es:[edi], eax
        mov    esi, 0x449
        mov    ecx, 0x1e
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x484
        mov    ecx, 7
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x4a8
        mov    ecx, 4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x410
        mov    ecx, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x40
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x7c
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0xbc
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x10c
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x1b4
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_14_0069:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cld    
        mov    esi, dword ptr [ebp + 8]
        lodsd  eax, dword ptr [esi]
        cmp    eax, 0x12345678
        jne    loc_14_00CF
        mov    edi, 0x449
        mov    ecx, 0x1e
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0x484
        mov    ecx, 7
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0x4a8
        mov    ecx, 4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0x410
        mov    ecx, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0x40
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0x7c
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0xbc
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0x10c
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0x1b4
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_14_00CF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_14_00E0:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        jb     loc_14_00FA
        cmp    eax, dword ptr [0x20]         ; -> obj6:0x0020
        ja     loc_14_00FA
        mov    eax, dword ptr [eax*4 + 0x638] ; -> obj9:0x0638
        jmp    loc_14_0108
loc_14_00FA:
        test   eax, 0xffff0000
        je     loc_14_0106
        cmp    eax, -1
        jne    loc_14_0108
loc_14_0106:
        sub    eax, eax
loc_14_0108:
        ret    
sub_14_0109:
        sub    esp, 4
        mov    dword ptr [esp], 0
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 1
        jb     loc_14_0132
        cmp    dword ptr [0x20], ecx         ; -> obj6:0x0020
        jb     loc_14_0132
        mov    eax, dword ptr [ecx*4 + 0x10b0] ; -> obj1:0x10B0
        mov    dword ptr [esp], eax
        jmp    loc_14_0146
loc_14_0132:
        or     ecx, ecx
        je     loc_14_0146
        lea    eax, [esp]
        push   0
        push   eax
        push   ecx
        call   sub_14_19A8
        add    esp, 0xc
loc_14_0146:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
sub_14_014E:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    esi, -1
        jne    loc_14_015A
        sub    esi, esi
loc_14_015A:
        push   dword ptr [esp + 8]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     loc_14_0170
        mov    dword ptr [eax + 0x22c], esi
loc_14_0170:
        pop    esi
        ret    
sub_14_0172:
        sub    esp, 8
        push   3
        lea    eax, [esp + 8]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_14_1968
        add    esp, 0xc
        or     eax, eax
        je     loc_14_018E
        jmp    loc_14_01F9
loc_14_018E:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [esp], eax
loc_14_0196:
        push   0
        push   0
        push   2
        lea    eax, [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   eax
        call   sub_14_1970
        add    esp, 0x14
        or     eax, eax
        jne    loc_14_01F9
        mov    eax, dword ptr [esp]
        mov    cx, word ptr [eax + 4]
        cmp    cx, 0x3b0
        jb     loc_14_01C7
        cmp    cx, 0x3bb
        jbe    loc_14_01EB
loc_14_01C7:
        mov    ax, word ptr [eax + 6]
        cmp    ax, 0x3b0
        jb     loc_14_01D7
        cmp    ax, 0x3bb
        jbe    loc_14_01F2
loc_14_01D7:
        cmp    cx, 0x3b0
        jae    loc_14_0196
        cmp    ax, 0x3bb
        jbe    loc_14_0196
        mov    eax, 1
        jmp    loc_14_01FB
loc_14_01EB:
        mov    eax, 1
        jmp    loc_14_01FB
loc_14_01F2:
        mov    eax, 1
        jmp    loc_14_01FB
loc_14_01F9:
        sub    eax, eax
loc_14_01FB:
        add    esp, 8
        ret    
sub_14_01FF:
        sub    esp, 8
        push   ebx
        push   dword ptr [esp + 0x10]
        call   sub_14_00E0
        add    esp, 4
        or     eax, eax
        jne    loc_14_0215
        jmp    loc_14_0287
loc_14_0215:
        push   2
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_14_1968
        add    esp, 0xc
        or     eax, eax
        je     loc_14_022B
        jmp    loc_14_0287
loc_14_022B:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    ebx, 1
loc_14_0238:
        push   0
        push   0
        push   ebx
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_14_1970
        add    esp, 0x14
        or     eax, eax
        jne    loc_14_0287
        mov    edx, dword ptr [esp + 4]
        test   byte ptr [edx + 0xc], bl
        je     loc_14_0238
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        cmp    eax, 0x7ffff
        jb     loc_14_0238
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [esp + 0x18]
        or     ecx, ecx
        je     loc_14_027B
        mov    edx, dword ptr [edx + 8]
        sub    edx, eax
        inc    edx
        mov    dword ptr [ecx], edx
loc_14_027B:
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        je     loc_14_0289
        mov    dword ptr [ecx], eax
        jmp    loc_14_0289
loc_14_0287:
        sub    eax, eax
loc_14_0289:
        pop    ebx
        add    esp, 8
        ret    
loc_14_028E:
        push   0
        push   0
        push   dword ptr [esp + 0xc]
        call   sub_14_01FF
        add    esp, 0xc
        ret    
sub_14_029F:
        sub    esp, 4
        push   0
        lea    eax, [esp + 4]
        push   4
        push   eax
        push   dword ptr [esp + 0x18]
        push   0
        push   dword ptr [esp + 0x1c]
        call   sub_14_19C8
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0x18
        add    esp, 4
        ret    
sub_14_02C5:
        push   0
        lea    eax, [esp + 0x10]
        push   4
        push   eax
        push   dword ptr [esp + 0x14]
        push   1
        push   dword ptr [esp + 0x18]
        call   sub_14_19C8
        add    esp, 0x18
        ret    
sub_14_02E1:
        sub    esp, 4
        push   0
        lea    eax, [esp + 6]
        push   2
        push   eax
        push   dword ptr [esp + 0x18]
        push   0
        push   dword ptr [esp + 0x1c]
        call   sub_14_19C8
        mov    ax, word ptr [esp + 0x1a]
        add    esp, 0x18
        add    esp, 4
        ret    
sub_14_0308:
        push   0
        lea    eax, [esp + 0x10]
        push   2
        push   eax
        push   dword ptr [esp + 0x14]
        push   1
        push   dword ptr [esp + 0x18]
        call   sub_14_19C8
        add    esp, 0x18
        ret    
sub_14_0324:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   4
        push   esi
        call   sub_14_029F
        add    esp, 8
        mov    edi, eax
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, eax
        not    ecx
        and    ecx, edi
        mov    edx, dword ptr [esp + 0x10]
        and    edx, eax
        or     ecx, edx
        push   ecx
        push   4
        push   esi
        call   sub_14_02C5
        add    esp, 0xc
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_14_035A:
        push   7
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_14_0324
        add    esp, 0xc
        ret    
sub_14_036D:
        sub    esp, 4
        push   ebx
        push   esi
        push   0
        lea    eax, [esp + 0xc]
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_14_1938
        add    esp, 0xc
        or     eax, eax
        jne    loc_14_0404
loc_14_0389:
        push   0xa
        push   dword ptr [esp + 0xc]
        call   sub_14_02E1
        add    esp, 8
        movzx  ebx, ax
        cmp    ebx, 1
        mov    eax, dword ptr [0]            ; -> obj18:0x0000
        je     loc_14_03A9
        cmp    bh, 3
        jne    loc_14_03D9
loc_14_03A9:
        lea    esi, [eax*4]
        mov    eax, dword ptr [esp + 8]
        push   0
        mov    dword ptr [esi + 4], eax      ; -> obj18:0x0004
        push   dword ptr [esp + 0xc]
        call   sub_14_035A
        add    esp, 8
        mov    dword ptr [esi + 0x54], eax   ; -> obj18:0x0054
        mov    eax, dword ptr [0]            ; -> obj18:0x0000
        inc    eax
        mov    dword ptr [0], eax            ; -> obj18:0x0000
loc_14_03D9:
        cmp    ebx, 0x604
        jne    loc_14_03ED
        push   dword ptr [esp + 8]
        call   sub_14_036D
        add    esp, 4
loc_14_03ED:
        push   0
        lea    eax, [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   eax
        call   sub_14_1940
        add    esp, 0xc
        or     eax, eax
        je     loc_14_0389
loc_14_0404:
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_14_040A:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0xc], 0
        je     loc_14_042D
        mov    dword ptr [0], 0              ; -> obj18:0x0000
        push   dword ptr [0x11a0]            ; -> obj1:0x11A0
        call   sub_14_036D
        add    esp, 4
        jmp    loc_14_0459
loc_14_042D:
        sub    edi, edi
        cmp    dword ptr [0], edi            ; -> obj18:0x0000
        jle    loc_14_0459
        sub    esi, esi
loc_14_0439:
        push   dword ptr [esi + 0x54]        ; -> obj18:0x0054
        push   dword ptr [esi + 4]           ; -> obj18:0x0004
        call   sub_14_035A
        add    esp, 8
        add    esi, 4
        inc    edi
        cmp    edi, dword ptr [0]            ; -> obj18:0x0000
        jl     loc_14_0439
loc_14_0459:
        pop    edi
        pop    esi
        ret    
sub_14_045C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x18]
        push   ebx
        lea    eax, [esp + 0x14]
        push   esi
        sub    edi, edi
        push   eax
        call   sub_14_1938
        add    esp, 0xc
        cmp    eax, ebx
        jne    loc_14_04DB
        mov    ebp, dword ptr [esp + 0x1c]
loc_14_0482:
        push   0xa
        push   dword ptr [esp + 0x14]
        call   sub_14_02E1
        add    esp, 8
        movzx  eax, ax
        cmp    ebp, dword ptr [esp + 0x10]
        jne    loc_14_049E
        mov    edi, 1
loc_14_049E:
        cmp    eax, 0x604
        jne    loc_14_04B4
        push   ebp
        push   dword ptr [esp + 0x14]
        call   sub_14_045C
        add    esp, 8
        mov    edi, eax
loc_14_04B4:
        push   4
        push   dword ptr [esp + 0x14]
        call   sub_14_029F
        add    esp, 8
        push   0
        or     ebx, eax
        lea    eax, [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_14_1940
        add    esp, 0xc
        or     eax, eax
        je     loc_14_0482
loc_14_04DB:
        cmp    esi, dword ptr [0x11a0]       ; -> obj1:0x11A0
        je     loc_14_0532
        push   4
        push   esi
        call   sub_14_029F
        add    esp, 8
        or     ebx, eax
        or     edi, edi
        je     loc_14_04FC
        or     ebx, 2
        or     ebx, 1
        jmp    loc_14_04FF
loc_14_04FC:
        or     ebx, 2
loc_14_04FF:
        push   ebx
        push   esi
        call   sub_14_035A
        add    esp, 8
        push   0x3e
        push   esi
        call   sub_14_02E1
        add    esp, 8
        or     edi, edi
        je     loc_14_0522
        or     ax, 4
        or     ax, 8
        jmp    loc_14_0526
loc_14_0522:
        and    ax, 0xfff7
loc_14_0526:
        push   eax
        push   0x3e
        push   esi
        call   sub_14_0308
        add    esp, 0xc
loc_14_0532:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  056h, 08Bh, 074h, 024h, 008h, 00Bh, 0F6h, 074h, 01Bh, 06Ah, 001h, 0E8h, 0BEh, 0FEh, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 068h, 0FFh, 000h, 000h, 000h, 056h, 0E8h, 000h, 0FEh, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 056h, 0EBh, 010h, 06Ah, 000h, 0E8h, 0A3h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0FFh, 035h
        dd  obj9:0x063C              ; fixup
        db  0FFh, 035h
        dd  obj1:0x11A0              ; fixup
        db  0E8h, 0E1h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 008h, 05Eh, 0C3h
sub_14_0580:
        sub    esp, 4
        mov    dword ptr [esp], 0
        push   0
        lea    eax, [esp + 4]
        push   4
        push   eax
        push   0
        push   3
        push   dword ptr [esp + 0x1c]
        call   sub_14_19C8
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0x18
        add    esp, 4
        ret    
sub_14_05AC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        push   0
        mov    edi, dword ptr [esp + 0x1c]
        push   ebx
        push   edi
        push   0
        mov    esi, dword ptr [esp + 0x24]
        push   4
        push   esi
        call   sub_14_19C8
        add    esp, 0x18
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_14_0623
        mov    eax, dword ptr [edi]
        and    eax, 0xffff
        cmp    eax, 0xaa55
        je     loc_14_0623
        push   0
        push   esi
        call   sub_14_029F
        add    esp, 8
        cmp    eax, 0x41581002
        jne    loc_14_0623
        push   0
        push   ebx
        push   0xfffd0000
        call   sub_14_1890
        add    esp, 0xc
        mov    ecx, dword ptr [eax]
        and    ecx, 0xffff
        cmp    ecx, 0xaa55
        jne    loc_14_0623
        mov    esi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_14_0623:
        cmp    ebp, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_14_062F:
        sub    esp, 8
        push   0
        push   0x8000
        push   dword ptr [esp + 0x14]
        call   sub_14_1890
        add    esp, 0xc
        mov    dword ptr [esp + 4], eax
        mov    ecx, dword ptr [esp + 4]
        in     al, 0x21
        mov    dl, byte ptr [ecx + 2]
        in     al, 0x21
        mov    dh, byte ptr [ecx + 3]
        shl    edx, 0x10
        in     al, 0x21
        mov    dl, byte ptr [ecx]
        in     al, 0x21
        mov    dh, byte ptr [ecx + 1]
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        add    esp, 8
        ret    
sub_14_066F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_14_040A
        add    esp, 4
        mov    edx, 0x3d4
        in     al, 0x21
        in     al, dx
        mov    byte ptr [esp + 0xf], al
        push   0xa0000
        call   sub_14_062F
        add    esp, 4
        mov    edi, eax
        push   0xb0000
        call   sub_14_062F
        add    esp, 4
        mov    ebx, eax
        push   0xb8000
        call   sub_14_062F
        add    esp, 4
        mov    esi, eax
        push   0
        call   sub_14_040A
        add    esp, 4
        cmp    byte ptr [esp + 0xf], 0xff
        jne    loc_14_06DD
        cmp    edi, -1
        jne    loc_14_06DD
        cmp    ebx, -1
        jne    loc_14_06DD
        cmp    esi, -1
        mov    eax, 1
        je     loc_14_06DF
loc_14_06DD:
        sub    eax, eax
loc_14_06DF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_14_06E6:
        call   sub_14_18F4
        or     eax, eax
        jne    loc_14_06FF
        push   dword ptr [esp + 4]
        push   0
        call   sub_14_18FC
        call   sub_14_1908
loc_14_06FF:
        ret    
sub_14_0700:
        push   esi
        push   edi
        sub    edi, edi
        mov    ecx, dword ptr [0x20]         ; -> obj6:0x0020
        cmp    ecx, edi
        jle    loc_14_0734
        mov    esi, 0x10b4                   ; -> obj1:0x10B4
loc_14_0713:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_14_072C
        push   0
        push   dword ptr [eax + 4]
        call   sub_14_035A
        add    esp, 8
        mov    ecx, dword ptr [0x20]         ; -> obj6:0x0020
loc_14_072C:
        add    esi, 4
        inc    edi
        cmp    ecx, edi
        jg     loc_14_0713
loc_14_0734:
        pop    edi
        pop    esi
        ret    
sub_14_0737:
        push   esi
        push   edi
        sub    edi, edi
        mov    ecx, dword ptr [0x20]         ; -> obj6:0x0020
        cmp    ecx, edi
        jle    loc_14_076C
        mov    esi, 0x10b4                   ; -> obj1:0x10B4
loc_14_074A:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_14_0764
        push   dword ptr [eax + 0xc]
        push   dword ptr [eax + 4]
        call   sub_14_035A
        add    esp, 8
        mov    ecx, dword ptr [0x20]         ; -> obj6:0x0020
loc_14_0764:
        add    esi, 4
        inc    edi
        cmp    edi, ecx
        jl     loc_14_074A
loc_14_076C:
        pop    edi
        pop    esi
        ret    
sub_14_076F:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ebx, dword ptr [esi + 0x2c9]
        or     ebx, ebx
        je     loc_14_079D
        mov    dx, 0x3ce
        in     al, dx
        mov    cl, al
        mov    al, 6
        out    dx, al
        mov    dx, 0x3cf
        in     al, dx
        and    al, 0xf3
        and    bl, 0xc
        or     al, bl
        out    dx, al
        mov    dx, 0x3ce
        mov    al, cl
        out    dx, al
loc_14_079D:
        mov    eax, dword ptr [esi + 0x14]
        and    eax, 0xffff
        cmp    eax, 0x1013
        je     loc_14_07B8
        mov    dx, 0x3cc
        in     al, dx
        or     al, 2
        mov    dx, 0x3c2
        out    dx, al
loc_14_07B8:
        pop    esi
        pop    ebx
        ret    
sub_14_07BB:
        push   esi
        mov    dx, 0x3ce
        in     al, dx
        mov    cl, al
        mov    al, 6
        out    dx, al
        mov    dx, 0x3cf
        in     al, dx
        mov    esi, dword ptr [esp + 8]
        movzx  eax, al
        mov    dword ptr [esi + 0x2c9], eax
        and    al, 0xfb
        or     al, 8
        out    dx, al
        mov    dx, 0x3ce
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [esi + 0x14]
        and    eax, 0xffff
        cmp    eax, 0x1013
        je     loc_14_07FF
        mov    dx, 0x3cc
        in     al, dx
        and    al, 0xfd
        mov    dx, 0x3c2
        out    dx, al
loc_14_07FF:
        pop    esi
        ret    
sub_14_0801:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0x11a4]       ; -> obj1:0x11A4
        mov    esi, dword ptr [esp + 0x14]
        cmp    edi, esi
        jne    loc_14_0818
        jmp    loc_14_0953
loc_14_0818:
        or     edi, edi
        je     loc_14_0883
        and    dword ptr [edi + 0xc], 0xffffffcf
        cmp    edi, dword ptr [0x10b4]       ; -> obj1:0x10B4
        je     loc_14_083B
        call   sub_8_0174
        push   dword ptr [0x11a4]            ; -> obj1:0x11A4
        call   sub_14_07BB
        add    esp, 4
loc_14_083B:
        call   sub_14_0737
        mov    ebx, dword ptr [0x20]         ; -> obj6:0x0020
        or     ebx, ebx
        jle    loc_14_0881
        mov    ebp, 0x10b4                   ; -> obj1:0x10B4
        mov    edi, dword ptr [0x11a4]       ; -> obj1:0x11A4
loc_14_0855:
        mov    edx, dword ptr [ebp]
        or     edx, edx
        je     loc_14_087B
        mov    eax, dword ptr [edx + 0x22c]
        or     eax, eax
        je     loc_14_087B
        cmp    edx, edi
        je     loc_14_087B
        mov    cl, byte ptr [eax]
        and    cl, 0xef
        mov    byte ptr [eax], cl
        mov    dl, byte ptr [edx + 0x230]
        or     dl, cl
        mov    byte ptr [eax], dl
loc_14_087B:
        add    ebp, 4
        dec    ebx
        jne    loc_14_0855
loc_14_0881:
        sub    edi, edi
loc_14_0883:
        mov    dword ptr [0x11a4], edi       ; -> obj1:0x11A4
        or     esi, esi
        je     loc_14_090C
        mov    dword ptr [0x11a4], esi       ; -> obj1:0x11A4
        or     dword ptr [esi + 0xc], 0x30
        call   sub_14_0700
        mov    eax, dword ptr [0x11a4]       ; -> obj1:0x11A4
        push   0xff
        push   dword ptr [eax + 4]
        call   sub_14_035A
        add    esp, 8
        mov    edi, dword ptr [0x11a4]       ; -> obj1:0x11A4
        cmp    edi, dword ptr [0x10b4]       ; -> obj1:0x10B4
        je     loc_14_08CA
        call   sub_8_0138
        mov    edi, dword ptr [0x11a4]       ; -> obj1:0x11A4
loc_14_08CA:
        mov    eax, dword ptr [0x20]         ; -> obj6:0x0020
        or     eax, eax
        jle    loc_14_090C
        mov    ecx, 0x10b4                   ; -> obj1:0x10B4
loc_14_08D8:
        mov    esi, dword ptr [ecx]
        or     esi, esi
        je     loc_14_0906
        mov    ebp, dword ptr [esi + 0x22c]
        or     ebp, ebp
        je     loc_14_0906
        cmp    esi, edi
        je     loc_14_0906
        mov    dl, byte ptr [ebp]
        mov    byte ptr [esi + 0x230], dl
        mov    bl, dl
        and    bl, 0x10
        mov    byte ptr [esi + 0x230], bl
        or     dl, 0x10
        mov    byte ptr [ebp], dl
loc_14_0906:
        add    ecx, 4
        dec    eax
        jne    loc_14_08D8
loc_14_090C:
        mov    dword ptr [0x11a4], edi       ; -> obj1:0x11A4
        or     edi, edi
        je     loc_14_093F
        push   dword ptr [edi + 4]
        push   dword ptr [0x11a0]            ; -> obj1:0x11A0
        call   sub_14_045C
        add    esp, 8
        mov    eax, dword ptr [0x11a4]       ; -> obj1:0x11A4
        cmp    eax, dword ptr [0x10b4]       ; -> obj1:0x10B4
        je     loc_14_0953
        push   eax
        call   sub_14_076F
        add    esp, 4
        jmp    loc_14_0953
loc_14_093F:
        push   dword ptr [0x63c]             ; -> obj9:0x063C
        push   dword ptr [0x11a0]            ; -> obj1:0x11A0
        call   sub_14_045C
        add    esp, 8
loc_14_0953:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_14_095D:
        push   esi
        sub    esi, esi
        mov    ecx, dword ptr [esp + 0x10]
        cmp    ecx, -1
        je     loc_14_09F1
        cmp    ecx, esi
        jne    loc_14_0979
        mov    esi, dword ptr [0x63c]        ; -> obj9:0x063C
        jmp    loc_14_09F1
loc_14_0979:
        cmp    byte ptr [ecx], 0x5c
        jne    loc_14_09F1
        cmp    byte ptr [ecx + 1], 0x5c
        jne    loc_14_09F1
        cmp    byte ptr [ecx + 2], 0x2e
        jne    loc_14_09F1
        cmp    byte ptr [ecx + 3], 0x5c
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 4]
        or     al, 0x20
        cmp    al, 0x64
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 5]
        or     al, 0x20
        cmp    al, 0x69
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 6]
        or     al, 0x20
        cmp    al, 0x73
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 7]
        or     al, 0x20
        cmp    al, 0x70
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 8]
        or     al, 0x20
        cmp    al, 0x6c
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 9]
        or     al, 0x20
        cmp    al, 0x61
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 0xa]
        or     al, 0x20
        cmp    al, 0x79
        jne    loc_14_09F1
        mov    al, byte ptr [ecx + 0xb]
        cmp    al, 0x30
        jl     loc_14_09F1
        cmp    al, 0x39
        jg     loc_14_09F1
        cmp    byte ptr [ecx + 0xc], 0
        jne    loc_14_09F1
        movsx  eax, al
        sub    eax, 0x30
        push   eax
        call   sub_14_00E0
        add    esp, 4
        mov    esi, eax
loc_14_09F1:
        or     esi, esi
        je     loc_14_0A08
        push   esi
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     loc_14_0A08
        inc    dword ptr [eax + 0x231]
loc_14_0A08:
        mov    eax, esi
        pop    esi
        ret    
loc_14_0A0C:
        push   dword ptr [esp + 4]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     loc_14_0A2D
        mov    ecx, dword ptr [eax + 0x231]
        or     ecx, ecx
        jle    loc_14_0A2D
        dec    ecx
        mov    dword ptr [eax + 0x231], ecx
loc_14_0A2D:
        sub    eax, eax
        ret    
sub_14_0A30:
        sub    esp, 0x80
        push   ebx
        push   esi
        push   edi
        push   dword ptr [esp + 0x90]
        call   sub_14_00E0
        add    esp, 4
        or     eax, eax
        jne    loc_14_0A51
        jmp    loc_14_0ADA
loc_14_0A51:
        mov    ebx, dword ptr [esp + 0x98]
        or     ebx, ebx
        je     loc_14_0ADA
        cmp    ebx, -1
        je     loc_14_0ADA
        cmp    dword ptr [esp + 0x9c], 4
        je     loc_14_0A6D
        jmp    loc_14_0ADA
loc_14_0A6D:
        sub    ecx, ecx
        mov    esi, dword ptr [0x20]         ; -> obj6:0x0020
        cmp    esi, ecx
        jle    loc_14_0A8A
        mov    edx, 0x63c                    ; -> obj9:0x063C
loc_14_0A7E:
        cmp    dword ptr [edx], eax
        je     loc_14_0A8A
        add    edx, 4
        inc    ecx
        cmp    ecx, esi
        jl     loc_14_0A7E
loc_14_0A8A:
        or     ecx, ecx
        jne    loc_14_0A9E
        mov    esi, 0x168                    ; -> obj6:0x0168
        lea    edi, [esp + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        jmp    loc_14_0AB1
loc_14_0A9E:
        push   ecx
        lea    eax, [esp + 0x10]
        push   0x17c                         ; -> obj6:0x017C
        push   eax
        call   sub_14_18B4
        add    esp, 0xc
loc_14_0AB1:
        cmp    dword ptr [esp + 0x94], 0
        lea    eax, [esp + 0xc]
        push   ebx
        push   eax
        push   0x80000005
        je     loc_14_0AD0
        call   sub_14_1880
        add    esp, 0xc
        jmp    loc_14_0ADF
loc_14_0AD0:
        call   sub_14_1870
        add    esp, 0xc
        jmp    loc_14_0ADF
loc_14_0ADA:
        mov    eax, 0x57
loc_14_0ADF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
sub_14_0AE9:
        sub    esp, 0x28
        mov    eax, dword ptr [esp + 0x38]
        or     eax, eax
        je     loc_14_0B0D
        push   eax
        push   dword ptr [esp + 0x38]
        lea    eax, [esp + 8]
        push   0x194                         ; -> obj6:0x0194
        push   eax
        call   sub_14_18B4
        add    esp, 0x10
        jmp    loc_14_0B23
loc_14_0B0D:
        push   dword ptr [esp + 0x34]
        lea    eax, [esp + 4]
        push   0x19c                         ; -> obj6:0x019C
        push   eax
        call   sub_14_18B4
        add    esp, 0xc
loc_14_0B23:
        push   eax
        lea    eax, [esp + 4]
        push   eax
        push   1
        push   0
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        call   sub_14_1888
        add    esp, 0x40
        ret    
loc_14_0B3E:
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x1c]
        cmp    eax, 0x4a
        jae    loc_14_0B56
        mov    eax, 0x57
        jmp    loc_14_0C25
loc_14_0B56:
        mov    esi, dword ptr [esp + 0x18]
        movzx  ecx, word ptr [esi]
        cmp    ecx, eax
        je     loc_14_0B6B
        mov    eax, 0x57
        jmp    loc_14_0C25
loc_14_0B6B:
        lea    eax, [esp + 8]
        push   4
        push   eax
        push   1
        push   dword ptr [esp + 0x1c]
        call   sub_14_0A30
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        jne    loc_14_0C23
        push   0
        movzx  eax, byte ptr [esi + 0x1f]
        push   eax
        push   0x1a0                         ; -> obj6:0x01A0
        push   dword ptr [esp + 0x14]
        call   sub_14_0AE9
        add    esp, 0x10
        movzx  eax, word ptr [esi + 0x1a]
        push   eax
        movzx  eax, word ptr [esi + 0x18]
        push   eax
        push   0x1b0                         ; -> obj6:0x01B0
        push   dword ptr [esp + 0x14]
        call   sub_14_0AE9
        add    esp, 0x10
        push   0
        push   dword ptr [esi + 0x32]
        push   0x1bc                         ; -> obj6:0x01BC
        push   dword ptr [esp + 0x14]
        call   sub_14_0AE9
        add    esp, 0x10
        push   0
        push   dword ptr [esi + 0x36]
        push   0x1c8                         ; -> obj6:0x01C8
        push   dword ptr [esp + 0x14]
        call   sub_14_0AE9
        add    esp, 0x10
        push   dword ptr [esi + 0x42]
        push   dword ptr [esi + 0x42]
        push   0x1d8                         ; -> obj6:0x01D8
        push   dword ptr [esp + 0x14]
        call   sub_14_0AE9
        add    esp, 0x10
        push   dword ptr [esi + 0x3a]
        push   dword ptr [esi + 0x3a]
        push   0x1e4                         ; -> obj6:0x01E4
        push   dword ptr [esp + 0x14]
        call   sub_14_0AE9
        add    esp, 0x10
        push   dword ptr [esp + 8]
        call   sub_14_1878
        add    esp, 4
loc_14_0C23:
        mov    eax, edi
loc_14_0C25:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_14_0C2B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x24], 1           ; -> obj6:0x0024
        jg     loc_14_0C43
        mov    ebp, dword ptr [esp + 0x18]
        mov    eax, ebp
        jmp    loc_14_0D74
loc_14_0C43:
        push   dword ptr [esp + 0x14]
        call   sub_14_0109
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_14_0D6F
        cmp    dword ptr [edi], 0x42414611
        jne    loc_14_0D6F
        call   sub_14_18CC
        mov    esi, eax
        call   sub_14_18A8
        cmp    esi, eax
        je     loc_14_0C82
        cmp    dword ptr [0x10b4], edi       ; -> obj1:0x10B4
        je     loc_14_0C82
        jmp    loc_14_0D6F
loc_14_0C82:
        test   byte ptr [edi + 8], 1
        mov    ebp, dword ptr [esp + 0x18]
        jne    loc_14_0C93
        mov    eax, ebp
        jmp    loc_14_0D74
loc_14_0C93:
        test   ebp, 0xffffff00
        je     loc_14_0CA0
        jmp    loc_14_0D6F
loc_14_0CA0:
        test   ebp, 0x10
        je     loc_14_0CAB
        or     ebp, 1
loc_14_0CAB:
        test   ebp, 0x20
        je     loc_14_0CB6
        or     ebp, 2
loc_14_0CB6:
        cmp    dword ptr [0x10b4], edi       ; -> obj1:0x10B4
        jne    loc_14_0CC1
        or     ebp, 3
loc_14_0CC1:
        mov    ebx, dword ptr [edi + 0xc]
        cmp    ebx, ebp
        jne    loc_14_0CCF
        mov    eax, ebp
        jmp    loc_14_0D74
loc_14_0CCF:
        mov    eax, ebx
        xor    eax, ebp
        and    eax, 3
        xor    eax, ebx
        mov    dword ptr [edi + 0xc], eax
        mov    esi, ebp
        and    esi, 0x30
        je     loc_14_0D1C
        mov    eax, dword ptr [0x11a4]       ; -> obj1:0x11A4
        or     eax, eax
        je     loc_14_0D07
        cmp    eax, edi
        je     loc_14_0D07
        cmp    dword ptr [0x10a8], 0         ; -> obj1:0x10A8
        je     loc_14_0CFA
        jmp    loc_14_0D6F
loc_14_0CFA:
        cmp    dword ptr [0x10b4], edi       ; -> obj1:0x10B4
        jne    loc_14_0D12
loc_14_0D02:
        mov    dword ptr [0x10a8], eax       ; -> obj1:0x10A8
loc_14_0D07:
        push   edi
        call   sub_14_0801
        add    esp, 4
        jmp    loc_14_0D3C
loc_14_0D12:
        cmp    eax, dword ptr [0x10b4]       ; -> obj1:0x10B4
        jne    loc_14_0D6F
        jmp    loc_14_0D02
loc_14_0D1C:
        cmp    dword ptr [0x11a4], edi       ; -> obj1:0x11A4
        jne    loc_14_0D3C
        push   dword ptr [0x10a8]            ; -> obj1:0x10A8
        call   sub_14_0801
        add    esp, 4
        mov    dword ptr [0x10a8], 0         ; -> obj1:0x10A8
loc_14_0D3C:
        test   ebp, 0x80
        je     loc_14_0D47
        push   edi
        jmp    loc_14_0D51
loc_14_0D47:
        cmp    dword ptr [0x10ac], edi       ; -> obj1:0x10AC
        jne    loc_14_0D59
        push   0
loc_14_0D51:
        call   sub_8_01B0
        add    esp, 4
loc_14_0D59:
        or     esi, esi
        jne    loc_14_0D6B
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 4]
        call   sub_14_035A
        add    esp, 8
loc_14_0D6B:
        mov    eax, ebx
        jmp    loc_14_0D74
loc_14_0D6F:
        mov    eax, 0xffffffff
loc_14_0D74:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_14_0D79:
        sub    esp, 0x80
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x90]
        shr    esi, 0xc
        lea    eax, [esp + 8]
        push   0
        push   eax
        mov    edi, dword ptr [esp + 0x94]
        push   esi
        mov    eax, edi
        shr    eax, 0xc
        push   eax
        call   sub_14_1910
        add    esp, 0x10
        sub    ecx, ecx
        cmp    esi, ecx
        jle    loc_14_0DCB
loc_14_0DAE:
        mov    eax, dword ptr [esp + ecx*4 + 8]
        test   al, 1
        je     loc_14_0DCB
        and    eax, 0xfffff000
        mov    edx, ecx
        shl    edx, 0xc
        sub    eax, edx
        cmp    eax, edi
        jne    loc_14_0DCB
        inc    ecx
        cmp    ecx, esi
        jl     loc_14_0DAE
loc_14_0DCB:
        sub    ecx, esi
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        add    esp, 0x80
        ret    
sub_14_0DDD:
        sub    esp, 0x10
        push   esi
        mov    dword ptr [esp + 0xc], 4
        mov    eax, dword ptr [esp + 0x18]
        mov    esi, 4
loc_14_0DF2:
        push   0
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_14_1930
        add    esp, 0xc
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_14_19A0
        add    esp, 0x14
        cmp    dword ptr [esp + 8], esi
        mov    eax, dword ptr [esp + 4]
        jne    loc_14_0E2B
        jmp    loc_14_0DF2
loc_14_0E2B:
        pop    esi
        add    esp, 0x10
        ret    
sub_14_0E30:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    ebx, 0x13
        push   1
        push   0x2cd
        call   sub_14_1898
        add    esp, 8
        mov    esi, eax
        cmp    esi, ebp
        jne    loc_14_0E61
        mov    ebx, 2
        mov    edi, dword ptr [esp + 0x18]
        jmp    loc_14_1054
loc_14_0E61:
        push   0
        mov    edi, dword ptr [esp + 0x1c]
        push   esi
        push   edi
        call   sub_14_19B0
        add    esp, 0xc
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 0x10], 0
        mov    dword ptr [esp + 0x10], 4
        push   0
        lea    eax, [esi + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esi + 0x10]
        push   eax
        push   edi
        call   sub_14_19A0
        add    esp, 0x14
        cmp    dword ptr [esi + 0x10], 4
        jne    loc_14_0EDF
        cmp    dword ptr [0x11a0], 0         ; -> obj1:0x11A0
        jne    loc_14_0EB8
        push   edi
        call   sub_14_0DDD
        add    esp, 4
        mov    dword ptr [0x11a0], eax       ; -> obj1:0x11A0
loc_14_0EB8:
        push   0
        push   edi
        call   sub_14_029F
        add    esp, 8
        mov    dword ptr [esi + 0x14], eax
        push   0xa
        push   edi
        call   sub_14_02E1
        add    esp, 8
        movzx  ebp, ax
        push   4
        push   edi
        call   sub_14_02E1
        add    esp, 8
loc_14_0EDF:
        lea    eax, [esi + 0x20]
        push   eax
        lea    eax, [esi + 0x1c]
        push   eax
        push   edi
        call   sub_14_01FF
        add    esp, 0xc
        push   edi
        call   sub_14_0172
        add    esp, 4
        or     eax, eax
        je     loc_14_0F90
        cmp    dword ptr [0x63c], 0          ; -> obj9:0x063C
        jne    loc_14_1054
        test   byte ptr [0xb1b], 0x40        ; -> obj9:0x0B1B
        je     loc_14_0F21
        mov    ebx, 0x22
        jmp    loc_14_1054
loc_14_0F21:
        push   edi
        call   sub_9_0B7D
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_14_1054
        mov    dword ptr [esi + 0xc], 0x83
        mov    eax, dword ptr [esi + 8]
        or     eax, 1
        mov    dword ptr [esi + 8], eax
        or     eax, 2
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [0x63c], edi        ; -> obj9:0x063C
        mov    dword ptr [0x10b4], esi       ; -> obj1:0x10B4
        mov    dword ptr [0x10ac], esi       ; -> obj1:0x10AC
        mov    dword ptr [0x11a4], 0         ; -> obj1:0x11A4
        cmp    dword ptr [esi + 0x10], 4
        jne    loc_14_0F6C
loc_14_0F6C:
        inc    dword ptr [0x20]              ; -> obj6:0x0020
        test   byte ptr [esi + 8], 1
        je     loc_14_0F83
        mov    eax, dword ptr [0x24]         ; -> obj6:0x0024
        inc    eax
        mov    dword ptr [0x24], eax         ; -> obj6:0x0024
loc_14_0F83:
        mov    dword ptr [esi], 0x42414611
        sub    eax, eax
        jmp    loc_14_1095
loc_14_0F90:
        cmp    dword ptr [0x63c], 0          ; -> obj9:0x063C
        jne    loc_14_0F9E
        jmp    loc_14_104F
loc_14_0F9E:
        cmp    dword ptr [0x20], 8           ; -> obj6:0x0020
        jl     loc_14_0FAC
        jmp    loc_14_104F
loc_14_0FAC:
        mov    eax, dword ptr [0x20]         ; -> obj6:0x0020
        shl    eax, 2
        mov    dword ptr [eax + 0x63c], edi  ; -> obj9:0x063C
        mov    dword ptr [eax + 0x10b4], esi ; -> obj1:0x10B4
        cmp    dword ptr [esi + 0x10], 4
        jne    loc_14_1090
        cmp    ebp, 0x300
        je     loc_14_0FF5
        cmp    ebp, 0x301
        je     loc_14_0FF5
        cmp    ebp, 1
        je     loc_14_0FF5
        push   4
        push   edi
        call   sub_14_02E1
        add    esp, 8
        and    eax, 3
        mov    dword ptr [esi + 0xc], eax
        jmp    loc_14_0F6C
loc_14_0FF5:
        or     dword ptr [esi + 8], 1
        mov    ebx, dword ptr [esi + 4]
        push   0
        push   ebx
        call   sub_14_035A
        add    esp, 8
        push   0x20
        push   0
        push   dword ptr [0x63c]             ; -> obj9:0x063C
        call   sub_14_0324
        add    esp, 0xc
        push   0x20
        push   0
        push   ebx
        call   sub_14_0324
        add    esp, 0xc
        call   sub_14_066F
        or     eax, eax
        jne    loc_14_103E
        push   0x278                         ; -> obj6:0x0278
        call   sub_14_06E6
        add    esp, 4
        jmp    loc_14_104F
loc_14_103E:
        push   esi
        call   sub_8_0327
        add    esp, 4
        or     eax, eax
        jne    loc_14_0F6C
loc_14_104F:
        mov    ebx, 0x13
loc_14_1054:
        mov    eax, dword ptr [0x20]         ; -> obj6:0x0020
        shl    eax, 2
        or     esi, esi
        mov    dword ptr [eax + 0x10b4], 0   ; -> obj1:0x10B4
        mov    dword ptr [eax + 0x63c], 0    ; -> obj9:0x063C
        je     loc_14_108C
        push   0
        push   esi
        call   sub_14_18A0
        add    esp, 8
        push   0
        push   0
        push   edi
        call   sub_14_19B0
        add    esp, 0xc
loc_14_108C:
        mov    eax, ebx
        jmp    loc_14_1095
loc_14_1090:
        mov    eax, 0x13
loc_14_1095:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_14_109D:
        push   dword ptr [esp + 4]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     loc_14_10CF
        test   byte ptr [eax + 8], 2
        je     loc_14_10B5
        jmp    loc_14_10CF
loc_14_10B5:
        cmp    dword ptr [eax + 0x231], 0
        je     loc_14_10C0
        jmp    loc_14_10CF
loc_14_10C0:
        cmp    dword ptr [eax + 0xc], 0
        je     loc_14_10C8
        jmp    loc_14_10CF
loc_14_10C8:
        mov    eax, 1
        jmp    loc_14_10D1
loc_14_10CF:
        sub    eax, eax
loc_14_10D1:
        ret    
sub_14_10D2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_14_109D
        add    esp, 4
        or     eax, eax
        jne    loc_14_10F3
        mov    eax, 0x13
        jmp    loc_14_11A3
loc_14_10F3:
        mov    dword ptr [esp + 0x10], 0
        sub    ebx, ebx
        mov    ecx, dword ptr [0x20]         ; -> obj6:0x0020
        cmp    ecx, ebx
        jle    loc_14_117F
        sub    edi, edi
        sub    esi, esi
loc_14_110B:
        mov    eax, dword ptr [esi + 0x63c]  ; -> obj9:0x063C
        cmp    dword ptr [esp + 0x18], eax
        jne    loc_14_115E
        mov    ebp, dword ptr [esi + 0x10b4] ; -> obj1:0x10B4
        test   byte ptr [ebp + 8], 1
        je     loc_14_1129
        dec    dword ptr [0x24]              ; -> obj6:0x0024
loc_14_1129:
        mov    eax, dword ptr [ebp + 0x28]
        or     eax, eax
        je     loc_14_113B
        push   0
        push   eax
        call   sub_14_18C4
        add    esp, 8
loc_14_113B:
        push   0
        push   ebp
        call   sub_14_18A0
        add    esp, 8
        push   0
        push   0
        push   dword ptr [esp + 0x20]
        call   sub_14_19B0
        add    esp, 0xc
        mov    ecx, dword ptr [0x20]         ; -> obj6:0x0020
        jmp    loc_14_1177
loc_14_115E:
        mov    dword ptr [edi + 0x63c], eax  ; -> obj9:0x063C
        mov    eax, dword ptr [esi + 0x10b4] ; -> obj1:0x10B4
        mov    dword ptr [edi + 0x10b4], eax ; -> obj1:0x10B4
        add    edi, 4
        inc    dword ptr [esp + 0x10]
loc_14_1177:
        add    esi, 4
        inc    ebx
        cmp    ecx, ebx
        jg     loc_14_110B
loc_14_117F:
        mov    eax, dword ptr [esp + 0x10]
        shl    eax, 2
        dec    ecx
        mov    dword ptr [eax + 0x63c], 0    ; -> obj9:0x063C
        mov    dword ptr [eax + 0x10b4], 0   ; -> obj1:0x10B4
        mov    dword ptr [0x20], ecx         ; -> obj6:0x0020
        sub    eax, eax
loc_14_11A3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_14_11AB:
        sub    esp, 0x1a4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1b8]
        push   esi
        call   sub_14_0109
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_14_11D5
        mov    eax, 1
        jmp    loc_14_130B
loc_14_11D5:
        test   byte ptr [edi + 8], 1
        je     loc_14_1251
        mov    ebp, 0x13
        push   0
        push   esi
        call   sub_16_00DB
        add    esp, 8
        or     eax, eax
        je     loc_14_1231
        push   0xff
        push   esi
        call   sub_14_0C2B
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, -1
        je     loc_14_1226
        lea    eax, [edi + 0x2b5]
        push   eax
        lea    eax, [edi + 0x235]
        push   eax
        call   sub_9_1AE3
        add    esp, 8
        mov    ebp, eax
        push   ebx
        push   esi
        call   sub_14_0C2B
        add    esp, 8
loc_14_1226:
        push   0
        push   esi
        call   sub_16_01FE
        add    esp, 8
loc_14_1231:
        or     ebp, ebp
        jne    loc_14_123B
        or     dword ptr [edi + 8], 4
        jmp    loc_14_123F
loc_14_123B:
        and    dword ptr [edi + 8], 0xfffffffb
loc_14_123F:
        cmp    ebp, 0x20
        jne    loc_14_1251
        push   0x30
        push   0
        push   esi
        call   sub_16_0059
        add    esp, 0xc
loc_14_1251:
        mov    eax, dword ptr [edi + 8]
        test   al, 4
        jne    loc_14_1288
        test   al, 2
        je     loc_14_1288
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        push   -1
        call   sub_14_19C0
        add    esp, 0xc
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0xf0]
        push   0x2c8                         ; -> obj6:0x02C8
        push   eax
        call   sub_14_18B4
        add    esp, 0xc
        jmp    loc_14_12C6
loc_14_1288:
        push   0
        lea    eax, [esp + 0xf0]
        push   0xc8
        push   eax
        push   esi
        call   sub_14_1948
        add    esp, 0x10
        sub    edx, edx
        cmp    byte ptr [esp + 0xec], dl
        je     loc_14_12C6
        sub    ecx, ecx
loc_14_12AD:
        lea    eax, [esp + edx + 0xec]
        cmp    byte ptr [eax], 0x5c
        jne    loc_14_12BC
        mov    byte ptr [eax], 0x5f
loc_14_12BC:
        inc    edx
        cmp    byte ptr [esp + edx + 0xec], cl
        jne    loc_14_12AD
loc_14_12C6:
        test   byte ptr [edi + 8], 4
        je     loc_14_12D4
        add    edi, 0x2b5
        jmp    loc_14_12D9
loc_14_12D4:
        mov    edi, 0x2d0                    ; -> obj6:0x02D0
loc_14_12D9:
        lea    eax, [esp + 0xec]
        push   eax
        push   edi
        lea    eax, [esp + 0x74]
        push   0x2e0                         ; -> obj6:0x02E0
        push   eax
        call   sub_14_18B4
        add    esp, 0x10
        push   0
        lea    eax, [esp + 0x70]
        push   esi
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_14_1950
        add    esp, 0x10
        sub    eax, eax
loc_14_130B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1a4
        ret    
sub_14_1316:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        movzx  eax, word ptr [esi]
        mov    cl, byte ptr [esi + 4]
        mov    dl, cl
        and    edx, 0xf0
        and    ecx, 0xf
        add    edx, ecx
        shl    edx, 4
        movzx  ecx, byte ptr [esi + 2]
        add    edx, ecx
        movzx  ecx, byte ptr [esi + 3]
        lea    edi, [edx + ecx]
        mov    dl, byte ptr [esi + 7]
        mov    bl, dl
        and    ebx, 0xf0
        shl    ebx, 4
        movzx  ecx, byte ptr [esi + 5]
        add    ebx, ecx
        movzx  ecx, byte ptr [esi + 6]
        add    ebx, ecx
        and    edx, 0xf
        lea    ecx, [ebx + edx]
        or     edi, edi
        je     loc_14_1377
        or     ecx, ecx
        je     loc_14_1377
        imul   eax, eax, 0x2710
        imul   ecx, edi
        sub    edx, edx
        div    ecx
        jmp    loc_14_1379
loc_14_1377:
        sub    eax, eax
loc_14_1379:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_14_137D:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   eax
        call   sub_14_1930
        add    esp, 0xc
        push   dword ptr [esp + 0x28]
        call   sub_14_0109
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_14_13B0
        sub    eax, eax
        jmp    loc_14_15C1
loc_14_13B0:
        test   byte ptr [edi + 8], 4
        jne    loc_14_13BD
        sub    eax, eax
        jmp    loc_14_15C1
loc_14_13BD:
        mov    ebx, dword ptr [edi + 0x258]
        and    ebx, 0x80ffff
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x14], 0
        sub    esi, esi
        mov    dword ptr [esp + 0x10], 0x7fffffff
        cmp    dword ptr [0x30], esi         ; -> obj6:0x0030
        je     loc_14_1437
        mov    ebp, 0x30                     ; -> obj6:0x0030
loc_14_13F0:
        test   dword ptr [ebp + 0xc], ebx
        je     loc_14_142E
        mov    eax, dword ptr [ebp]
        mov    ecx, dword ptr [ebp + 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [esp + 0x34]
        jne    loc_14_141A
        cmp    ecx, dword ptr [esp + 0x38]
        jne    loc_14_141A
        cmp    esi, edx
        jge    loc_14_1410
        mov    esi, edx
loc_14_1410:
        cmp    dword ptr [esp + 0x10], edx
        jle    loc_14_141A
        mov    dword ptr [esp + 0x10], edx
loc_14_141A:
        cmp    eax, dword ptr [esp + 0x18]
        jle    loc_14_1424
        mov    dword ptr [esp + 0x18], eax
loc_14_1424:
        cmp    ecx, dword ptr [esp + 0x14]
        jle    loc_14_142E
        mov    dword ptr [esp + 0x14], ecx
loc_14_142E:
        add    ebp, 0x10
        cmp    dword ptr [ebp], 0
        jne    loc_14_13F0
loc_14_1437:
        lea    ecx, [edi + 0x25b]
        mov    dword ptr [esp + 0x24], 8
loc_14_1445:
        movzx  ebx, word ptr [ecx]
        or     ebx, ebx
        je     loc_14_14F5
        cmp    ebx, 0x101
        je     loc_14_14F5
        mov    eax, ebx
        and    eax, 0xff
        lea    ebp, [eax*8 + 0xf8]
        mov    eax, ebx
        and    eax, 0xc000
        cmp    eax, 0x4000
        jne    loc_14_1488
        mov    eax, ebp
        imul   eax, eax, 3
        cdq    
        and    edx, 3
        add    eax, edx
        sar    eax, 2
        jmp    loc_14_14B9
loc_14_1488:
        cmp    eax, 0x8000
        jne    loc_14_14A5
        lea    eax, [ebp*4]
        mov    dword ptr [esp + 0x1c], 5
        cdq    
        idiv   dword ptr [esp + 0x1c]
        jmp    loc_14_14B9
loc_14_14A5:
        or     eax, eax
        mov    eax, ebp
        je     loc_14_14B9
        mov    eax, ebp
        imul   eax, eax, 9
        cdq    
        and    edx, 0xf
        add    eax, edx
        sar    eax, 4
loc_14_14B9:
        and    ebx, 0x1f00
        shr    ebx, 8
        add    ebx, 0x3c
        cmp    ebp, dword ptr [esp + 0x34]
        jne    loc_14_14E1
        cmp    eax, dword ptr [esp + 0x38]
        jne    loc_14_14E1
        cmp    esi, ebx
        jge    loc_14_14D7
        mov    esi, ebx
loc_14_14D7:
        cmp    dword ptr [esp + 0x10], ebx
        jle    loc_14_14E1
        mov    dword ptr [esp + 0x10], ebx
loc_14_14E1:
        cmp    dword ptr [esp + 0x18], ebp
        jge    loc_14_14EB
        mov    dword ptr [esp + 0x18], ebp
loc_14_14EB:
        cmp    eax, dword ptr [esp + 0x14]
        jle    loc_14_14F5
        mov    dword ptr [esp + 0x14], eax
loc_14_14F5:
        add    ecx, 2
        dec    dword ptr [esp + 0x24]
        jne    loc_14_1445
        add    edi, 0x26b
        mov    dword ptr [esp + 0x20], 4
loc_14_1510:
        cmp    dword ptr [edi], 0
        je     loc_14_1573
        mov    cl, byte ptr [edi + 4]
        and    ecx, 0xf0
        shl    ecx, 4
        movzx  eax, byte ptr [edi + 2]
        lea    ebx, [ecx + eax]
        mov    al, byte ptr [edi + 7]
        and    eax, 0xf0
        shl    eax, 4
        movzx  ecx, byte ptr [edi + 5]
        lea    ebp, [eax + ecx]
        push   edi
        call   sub_14_1316
        add    esp, 4
        cmp    ebx, dword ptr [esp + 0x34]
        jne    loc_14_155F
        cmp    ebp, dword ptr [esp + 0x38]
        jne    loc_14_155F
        cmp    esi, eax
        jge    loc_14_1555
        mov    esi, eax
loc_14_1555:
        cmp    dword ptr [esp + 0x10], eax
        jle    loc_14_155F
        mov    dword ptr [esp + 0x10], eax
loc_14_155F:
        cmp    ebx, dword ptr [esp + 0x18]
        jle    loc_14_1569
        mov    dword ptr [esp + 0x18], ebx
loc_14_1569:
        cmp    ebp, dword ptr [esp + 0x14]
        jle    loc_14_1573
        mov    dword ptr [esp + 0x14], ebp
loc_14_1573:
        add    edi, 0x12
        dec    dword ptr [esp + 0x20]
        jne    loc_14_1510
        cmp    dword ptr [esp + 0x34], 0
        jne    loc_14_15A0
        cmp    dword ptr [esp + 0x38], 0
        jne    loc_14_15A0
        mov    ecx, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 0x40]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], eax
        jmp    loc_14_15BC
loc_14_15A0:
        or     esi, esi
        jne    loc_14_15AC
        mov    dword ptr [esp + 0x10], 0
loc_14_15AC:
        mov    ecx, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 0x40]
        mov    dword ptr [ecx], esi
loc_14_15BC:
        mov    eax, 1
loc_14_15C1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_14_15C9:
        sub    esp, 0x88
        push   esi
        push   edi
        push   dword ptr [esp + 0x94]
        call   sub_14_0109
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_14_15F0
        mov    eax, 1
        jmp    loc_14_16E6
loc_14_15F0:
        test   byte ptr [esi + 8], 4
        je     loc_14_16E4
        push   0
        push   0x80
        lea    eax, [esi + 0x235]
        push   eax
        push   3
        push   0x2f0                         ; -> obj6:0x02F0
        mov    edi, dword ptr [esp + 0xac]
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 0x1c
        lea    eax, [esi + 0x2b5]
        push   eax
        lea    eax, [esp + 0x14]
        push   0x2f8                         ; -> obj6:0x02F8
        push   eax
        call   sub_14_18B4
        add    esp, 0xc
        push   0
        inc    eax
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   1
        push   0x30c                         ; -> obj6:0x030C
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 0x1c
        push   0
        push   8
        push   0x318                         ; -> obj6:0x0318
        push   1
        push   0x320                         ; -> obj6:0x0320
        push   0
        push   edi
        call   sub_14_1998
        lea    eax, [esp + 0x28]
        add    esp, 0x1c
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_14_137D
        add    esp, 0x14
        or     eax, eax
        je     loc_14_16C0
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        lea    eax, [esp + 0x18]
        push   0x328                         ; -> obj6:0x0328
        push   eax
        call   sub_14_18B4
        add    esp, 0x10
        push   0
        inc    eax
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   1
        push   0x330                         ; -> obj6:0x0330
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 0x1c
loc_14_16C0:
        test   byte ptr [esi + 0x24d], 0xe0
        je     loc_14_16E4
        push   0
        push   2
        push   0x340                         ; -> obj6:0x0340
        push   1
        push   0x344                         ; -> obj6:0x0344
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 0x1c
loc_14_16E4:
        sub    eax, eax
loc_14_16E6:
        pop    edi
        pop    esi
        add    esp, 0x88
        ret    
loc_14_16EF:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, 5
        je     loc_14_1706
        cmp    eax, 6
        je     loc_14_172F
        mov    eax, 1
        jmp    loc_14_173F
loc_14_1706:
        mov    esi, dword ptr [esp + 0x14]
        push   0
        push   esi
        call   sub_14_1980
        add    esp, 8
        push   esi
        call   sub_14_11AB
        add    esp, 4
        mov    edi, eax
        push   0
        push   esi
        call   sub_14_1988
        add    esp, 8
        mov    eax, edi
        jmp    loc_14_173F
loc_14_172F:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_14_15C9
        add    esp, 8
loc_14_173F:
        pop    edi
        pop    esi
        ret    
loc_14_1742:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        je     loc_14_1761
        cmp    eax, 2
        je     loc_14_179C
        cmp    eax, 3
        je     loc_14_176C
        cmp    eax, 4
        je     loc_14_1790
        mov    eax, 1
        jmp    loc_14_17A8
loc_14_1761:
        push   dword ptr [esp + 0xc]
        call   sub_14_0E30
        jmp    loc_14_17A5
loc_14_176C:
        cmp    dword ptr [esp + 8], 1
        jbe    loc_14_177A
        mov    eax, 1
        jmp    loc_14_17A8
loc_14_177A:
        push   dword ptr [esp + 0xc]
        call   sub_14_109D
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
        jmp    loc_14_17A8
loc_14_1790:
        cmp    dword ptr [esp + 8], 1
        mov    eax, 0
        je     loc_14_17A8
loc_14_179C:
        push   dword ptr [esp + 0xc]
        call   sub_14_10D2
loc_14_17A5:
        add    esp, 4
loc_14_17A8:
        ret    
loc_14_17A9:
        push   0
        push   dword ptr [esp + 8]
        call   sub_14_1978
        add    esp, 8
        ret    
sub_14_17B8:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x14], 1
        je     loc_14_17C8
        mov    eax, 0x13
        jmp    loc_14_182C
loc_14_17C8:
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_14_0172
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_14_17F6
        push   0
        push   dword ptr [esp + 0x10]
        push   0x17a9                        ; -> loc_14_17A9
        call   sub_14_19B8
        add    esp, 0xc
loc_14_17EE:
        or     edi, edi
        je     loc_14_1806
        push   0
        jmp    loc_14_1808
loc_14_17F6:
        cmp    dword ptr [0x63c], 0          ; -> obj9:0x063C
        jne    loc_14_17EE
        mov    eax, 0x21
        jmp    loc_14_182C
loc_14_1806:
        push   3
loc_14_1808:
        push   0
        push   0x1742                        ; -> loc_14_1742
        push   esi
        call   sub_14_1958
        add    esp, 0x10
        or     eax, eax
        jne    loc_14_182C
        push   1
        push   0x16ef                        ; -> loc_14_16EF
        push   esi
        call   sub_14_1960
        add    esp, 0xc
loc_14_182C:
        pop    edi
        pop    esi
        ret    0xc
sub_14_1831:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_14_1843
        cmp    eax, -1
        mov    eax, 0x32
        jne    loc_14_1845
loc_14_1843:
        sub    eax, eax
loc_14_1845:
        ret    0x10
sub_14_1848:
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e801700
        add    byte ptr [eax], al
sub_14_1850:
        VxDCall  SHELL._SHELL_CallDll            ; cd2018801700
        add    byte ptr [eax], al
sub_14_1858:
        VxDCall  SHELL._SHELL_CancelAppyTimeEvent ; cd200f801700
        add    byte ptr [eax], al
sub_14_1860:
        VxDCall  SHELL._SHELL_LoadLibrary        ; cd2015801700
        add    byte ptr [eax], al
sub_14_1868:
        VxDCall  SHELL._SHELL_GetProcAddress     ; cd2017801700
        add    byte ptr [eax], al
sub_14_1870:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_14_1878:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_14_1880:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_14_1888:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_14_1890:
        VMMCall  VMM._MapPhysToLinear            ; cd206c800100
        add    byte ptr [eax], al
sub_14_1898:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_14_18A0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_14_18A8:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_14_18B4:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        add    byte ptr [eax], al
sub_14_18BC:
        VMMCall  VMM._PageAllocate               ; cd2053800100
        add    byte ptr [eax], al
sub_14_18C4:
        VMMCall  VMM._PageFree                   ; cd2055800100
        add    byte ptr [eax], al
sub_14_18CC:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_14_18D8:
        VMMCall  VMM._LinPageLock                ; cd2063800100
        add    byte ptr [eax], al
sub_14_18E0:
        VMMCall  VMM._LinPageUnLock              ; cd2064800100
        add    byte ptr [eax], al
sub_14_18E8:
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    eax, edx
        ret    
        db  000h, 000h, 000h
sub_14_18F4:
        VMMCall  VMM.Open_Boot_Log               ; cd2088810100
        add    byte ptr [eax], al
sub_14_18FC:
        pop    eax
        pop    ecx
        pop    edx
        VMMCall  VMM.Write_Boot_Log              ; cd2089010100
        jmp    eax
        db  000h
sub_14_1908:
        VMMCall  VMM.Close_Boot_Log              ; cd208a810100
        add    byte ptr [eax], al
sub_14_1910:
        VMMCall  VMM._CopyPageTable              ; cd2061800100
        add    byte ptr [eax], al
sub_14_1918:
        VMMCall  VMM._PhysIntoV86                ; cd205e800100
        add    byte ptr [eax], al
sub_14_1920:
        VMMCall  VMM._LinMapIntoV86              ; cd2062800100
        add    byte ptr [eax], al
sub_14_1928:
        VMMCall  VMM._PageReAllocate             ; cd2054800100
        add    byte ptr [eax], al
sub_14_1930:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        add    byte ptr [eax], al
sub_14_1938:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        add    byte ptr [eax], al
sub_14_1940:
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005803300
        add    byte ptr [eax], al
sub_14_1948:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        add    byte ptr [eax], al
sub_14_1950:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        add    byte ptr [eax], al
sub_14_1958:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        add    byte ptr [eax], al
sub_14_1960:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        add    byte ptr [eax], al
sub_14_1968:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_14_1970:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        add    byte ptr [eax], al
sub_14_1978:
        VxDCall  CONFIGMG._CONFIGMG_Register_DevLoader ; cd2032803300
        add    byte ptr [eax], al
sub_14_1980:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        add    byte ptr [eax], al
sub_14_1988:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        add    byte ptr [eax], al
sub_14_1990:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_14_1998:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        add    byte ptr [eax], al
sub_14_19A0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        add    byte ptr [eax], al
sub_14_19A8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_Driver_Private_DWord ; cd2048803300
        add    byte ptr [eax], al
sub_14_19B0:
        VxDCall  CONFIGMG._CONFIGMG_Set_Device_Driver_Private_DWord ; cd2049803300
        add    byte ptr [eax], al
sub_14_19B8:
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e803300
        add    byte ptr [eax], al
sub_14_19C0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Hardware_Profile_Info ; cd2052803300
        add    byte ptr [eax], al
sub_14_19C8:
        VxDCall  CONFIGMG._CONFIGMG_Call_Enumerator_Function ; cd2054803300
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 15  vsize=0x541c  R-X
loc_15_0000:
        test   edx, edx
        je     loc_15_000D
        cmp    edx, 0xa
        jne    loc_15_0090
loc_15_000D:
        cmp    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        je     loc_15_0090
loc_15_0015:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        pop    ebx
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    dword ptr [0x7a4], ebx        ; -> loc_1_07A4
        test   byte ptr [0x6c4], 0x40        ; -> obj1:0x06C4
        jne    loc_15_0090
        test   byte ptr [0x6c4], 8           ; -> obj1:0x06C4
        je     sub_15_0049
        or     byte ptr [0x6c4], 0x80        ; -> obj1:0x06C4
        clc    
        ret    
sub_15_0049:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_15_005F
        push   0xff
        push   1
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
loc_15_005F:
        call   sub_15_1802
        call   sub_15_12D5
        call   sub_15_35F9
        call   sub_15_119E
        call   sub_15_4B45
        call   sub_15_17B4
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_15_0090
        push   3
        push   1
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
loc_15_0090:
        clc    
        ret    
sub_15_0092:
        btr    dword ptr [0x6c4], 7          ; -> obj1:0x06C4
        jb     loc_15_009D
        ret    
loc_15_009D:
        mov    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        jmp    loc_15_0015
sub_15_00A8:
        cmp    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        jne    loc_15_010E
        test   byte ptr [edi + 1], 0x20
        jne    loc_15_0107
        call   sub_15_015A
        jb     loc_15_0107
        call   sub_15_1802
        test   byte ptr [edi + 2], 0x40
        jne    loc_15_0110
        call   sub_15_025E
        push   edi
        lea    esi, [edi + 0x543]
        lea    edi, [edi + 0x57b]
        mov    ecx, 4
loc_15_00DF:
        push   ecx
        movzx  ecx, word ptr [edi + 8]
        jecxz  loc_15_00F6
        push   esi
        push   edi
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [edi + 4]
        cld    
        shl    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
loc_15_00F6:
        add    esi, 0xc
        add    edi, 0xc
        pop    ecx
        loop   loc_15_00DF
        pop    edi
loc_15_0100:
        call   sub_15_17B4
        clc    
        ret    
loc_15_0107:
        call   sub_15_016E
        jmp    loc_15_010E
loc_15_010E:
        stc    
        ret    
loc_15_0110:
        push   edi
        lea    esi, [edi + 0x57b]
        lea    edi, [edi + 0x543]
        mov    ecx, 0xe
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        call   sub_15_29F4
        mov    esi, dword ptr [edi + 0x5b8]
        lea    esi, [esi + eax*4]
loc_15_0134:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_15_0141
        pushal 
        call   sub_15_4009
        popal  
loc_15_0141:
        add    esi, 4
        loop   loc_15_0134
        push   edi
        lea    edi, [edi + 0x543]
        xor    eax, eax
        mov    ecx, 0xe
        cld    
        rep stosd dword ptr es:[edi], eax
        pop    edi
        jmp    loc_15_0100
sub_15_015A:
        lea    esi, [edi + 0x57b]
        or     edx, 0xffffffff
        call   sub_15_36D3
        call   sub_15_37CD
        ret    
sub_15_016E:
        lea    esi, [edi + 0x57b]
        call   sub_15_3695
        ret    
sub_15_017A:
        call   sub_15_29F4
        lea    esi, [edi + 0x5f4]
        cmp    cx, word ptr [esi + 0xa]
        jne    loc_15_0191
        cmp    dword ptr [esi + 4], 0
        jne    loc_15_01A9
loc_15_0191:
        cmp    cx, word ptr [esi + 0xa]
        ja     loc_15_01A0
        test   byte ptr [edi + 0x5e2], 0x80
        jne    loc_15_01A9
loc_15_01A0:
        mov    word ptr [esi + 8], cx
        call   sub_15_376B
loc_15_01A9:
        mov    edx, dword ptr [esi + 4]
        movzx  ecx, word ptr [esi + 0xa]
        ret    
sub_15_01B1:
        test   byte ptr [edi + 0x5e2], 0x80
        jne    loc_15_01E6
        xor    eax, eax
        mov    dword ptr [edi + 0x5f8], eax
        mov    word ptr [edi + 0x5fc], ax
        mov    word ptr [edi + 0x5fe], ax
        xchg   dword ptr [edi + 0x5f4], eax
        or     eax, eax
        je     loc_15_01E6
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_15_01E6:
        ret    
sub_15_01E7:
        mov    eax, dword ptr [edi + 0x5c0]
        or     eax, eax
        stc    
        jne    loc_15_024A
        cmp    dword ptr [edi + 0x5c8], 0xffff0000
        stc    
        jne    loc_15_024A
        pushal 
        call   sub_15_29F4
        mov    edx, ecx
        call   sub_15_2B01
        mov    ecx, eax
        shl    ecx, 0xc
        add    ecx, dword ptr [ebx + 4]
        shr    ecx, 0xc
        shl    edx, 2
        sub    esp, edx
        mov    eax, esp
        shr    edx, 2
        push   edx
        push   0
        push   eax
        push   edx
        push   ecx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        pop    ecx
        mov    eax, esp
        lea    edx, [ecx*4]
        xor    ebx, ebx
loc_15_023C:
        or     bl, byte ptr [eax]
        add    eax, 4
        loop   loc_15_023C
        add    esp, edx
        bt     ebx, 6
        popal  
loc_15_024A:
        ret    
sub_15_024B:
        mov    ecx, 1
        push   edi
        lea    edi, [edi + 0x5c4]
        cld    
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        pop    edi
        ret    
sub_15_025E:
        push   esi
        mov    esi, dword ptr [edi + 0x5b8]
        xor    ecx, ecx
loc_15_0267:
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     loc_15_02D2
        call   sub_15_3F3C
        je     loc_15_02B9
        push   ecx
        push   esi
        call   sub_15_10CC
        call   sub_15_4009
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_02A6
loc_15_0287:
        push   ecx
        push   0
        push   -1
        push   0
        push   -1
        push   1
        push   ecx
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        pop    ecx
        shl    ecx, 0xc
        add    ecx, dword ptr [ebx + 4]
        mov    cl, byte ptr [ecx]
loc_15_02A6:
        pop    esi
        pop    ecx
        mov    eax, dword ptr [edi + 0x5bc]
        or     byte ptr [ecx + eax], 1
        mov    eax, ecx
        call   sub_15_456A
loc_15_02B9:
        inc    ecx
        cmp    ecx, dword ptr [edi + 0x5b4]
        jb     loc_15_0267
        pop    esi
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        je     loc_15_02D1
        call   sub_15_10EA
loc_15_02D1:
        ret    
loc_15_02D2:
        push   ecx
        push   esi
        mov    eax, ecx
        call   sub_15_3A9D
        jae    loc_15_02EE
        mov    esi, ebx
        push   eax
        call   sub_15_2D54
        pop    eax
        xchg   ebx, esi
        jb     loc_15_030A
        cmp    ebx, esi
        jne    loc_15_030A
loc_15_02EE:
        call   sub_15_2B01
        cmp    eax, 0xc0
        jae    loc_15_030A
        push   eax
        mov    ecx, eax
        call   sub_15_3F19
        test   cl, 0x40
        jne    loc_15_030E
        add    esp, 4
loc_15_030A:
        pop    esi
        pop    ecx
        jmp    loc_15_02B9
loc_15_030E:
        pop    ecx
        jmp    loc_15_0287
sub_15_0314:
        call   sub_15_025E
        call   sub_15_29F4
        or     byte ptr [esi], 0x10
        mov    word ptr [esi + 2], cx
        add    ecx, 7
        shr    ecx, 3
        push   ebx
        push   ecx
        push   esi
        push   edi
        xor    ch, ch
        lea    ebx, [edi + 0x5c0]
        lea    edi, [edi + 0x5c4]
        lea    esi, [esi + 4]
        xchg   esi, edi
        cld    
loc_15_0343:
        mov    al, byte ptr [ebx]
        mov    byte ptr [ebx], 0
        or     al, byte ptr [esi]
        mov    byte ptr [esi], al
        or     ch, al
        stosb  byte ptr es:[edi], al
        inc    ebx
        inc    esi
        dec    cl
        jne    loc_15_0343
        pop    edi
        pop    esi
        or     ch, ch
        pop    ecx
        jne    loc_15_0370
        cmp    dword ptr [edi + 0x5c8], 0xffff0000
        jne    loc_15_036E
        and    word ptr [esi], 0xffef
        xor    ecx, ecx
loc_15_036E:
        pop    ebx
        ret    
loc_15_0370:
        mov    dword ptr [edi + 0x5c8], 0xffffffff
        pop    ebx
        ret    
sub_15_037C:
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_047D
        cmp    dword ptr [0xf00], 0          ; -> loc_1_0F00
        jne    loc_15_03A6
        mov    eax, dword ptr [0xef8]        ; -> loc_1_0EF8
        or     eax, eax
        je     loc_15_05F2
        mov    byte ptr [eax], 0
        jmp    loc_15_05F2
loc_15_03A6:
        and    byte ptr [0x7a2], 0xfb        ; -> loc_1_07A2
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        cmp    eax, ebx
        je     loc_15_03D2
        or     byte ptr [0x7a2], 4           ; -> loc_1_07A2
        mov    esi, 0x37c                    ; -> sub_15_037C
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        jmp    loc_15_047B
loc_15_03D2:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    esi, edi
        or     byte ptr [0x7a0], 1           ; -> loc_1_07A0
        xor    ecx, ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        call   sub_15_05F4
        mov    byte ptr [0x449], 6
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x18], 0
        je     loc_15_0406
        call   dword ptr [edi + 0x18]
loc_15_0406:
        pop    edi
        push   ebx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        movzx  edx, word ptr [0xf00]         ; -> loc_1_0F00
        mov    cx, word ptr [0xf02]          ; -> loc_1_0F02
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x1c], 0
        je     loc_15_045C
        call   dword ptr [edi + 0x1c]
loc_15_045C:
        pop    edi
        call   sub_15_062A
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        and    byte ptr [0x7a0], 0xfe        ; -> loc_1_07A0
        mov    eax, dword ptr [0xef8]        ; -> loc_1_0EF8
        or     eax, eax
        je     loc_15_047B
        mov    byte ptr [eax], 0
loc_15_047B:
        stc    
        ret    
loc_15_047D:
        mov    eax, dword ptr [0xefc]        ; -> loc_1_0EFC
        or     eax, eax
        je     loc_15_0490
        mov    cx, word ptr [0xfb2]          ; -> loc_1_0FB2
        or     word ptr [eax], cx
loc_15_0490:
        mov    eax, dword ptr [0xef8]        ; -> loc_1_0EF8
        or     eax, eax
        je     loc_15_049C
        mov    byte ptr [eax], 1
loc_15_049C:
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_04D2
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi + 1], 0x20
        jne    loc_15_04B9
        call   sub_1_028C
        jae    loc_15_04D2
loc_15_04B9:
        mov    esi, 0xe15                    ; -> loc_15_0E15
        mov    eax, 0x1000
        mov    ecx, 3
        xor    edx, edx
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        stc    
        ret    
loc_15_04D2:
        mov    ecx, dword ptr [0x7a8]        ; -> loc_1_07A8
        cmp    ecx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_04F0
        add    ecx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        test   byte ptr [ecx + 1], 0x20
        je     loc_15_05F2
loc_15_04F0:
        xor    ecx, ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x10], 0
        je     loc_15_0507
        call   dword ptr [edi + 0x10]
loc_15_0507:
        pop    edi
        call   sub_15_05F4
        mov    eax, 0x400
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_051D
        add    eax, dword ptr [ebx + 4]
loc_15_051D:
        mov    dx, word ptr [eax + 0x60]
        shl    edx, 0x10
        mov    dl, byte ptr [eax + 0x62]
        mov    dh, byte ptr [eax + 0x49]
        push   edx
        mov    dl, byte ptr [eax + 0x4a]
        mov    dh, byte ptr [eax + 0x84]
        shl    edx, 0x10
        mov    dl, byte ptr [eax + 0x85]
        mov    dh, byte ptr [eax + 0x89]
        push   edx
        push   dword ptr [eax + 0x50]
        push   dword ptr [eax + 0x54]
        push   dword ptr [eax + 0x58]
        push   dword ptr [eax + 0x5c]
        push   dword ptr [eax + 0x87]
        sub    eax, 0x400
        push   dword ptr [eax + 0x10c]
        push   eax
        movzx  eax, byte ptr [eax + 0x449]
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        jne    loc_15_0573
        mov    al, 3
loc_15_0573:
        or     al, 0x80
        push   ebx
        or     byte ptr [0x7a0], 2           ; -> loc_1_07A0
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
        pop    eax
        pop    dword ptr [eax + 0x10c]
        add    eax, 0x400
        pop    dword ptr [eax + 0x87]
        pop    dword ptr [eax + 0x5c]
        pop    dword ptr [eax + 0x58]
        pop    dword ptr [eax + 0x54]
        pop    dword ptr [eax + 0x50]
        pop    edx
        mov    byte ptr [eax + 0x85], dl
        mov    byte ptr [eax + 0x89], dh
        shr    edx, 0x10
        mov    byte ptr [eax + 0x4a], dl
        mov    byte ptr [eax + 0x84], dh
        pop    edx
        mov    byte ptr [eax + 0x49], dh
        mov    byte ptr [eax + 0x62], dl
        shr    edx, 0x10
        mov    word ptr [eax + 0x60], dx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x14], 0
        je     loc_15_05DD
        call   dword ptr [edi + 0x14]
loc_15_05DD:
        pop    edi
        call   sub_15_062A
        and    byte ptr [0x7a0], 0xfd        ; -> loc_1_07A0
        mov    esi, edi
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_15_05F2:
        clc    
        ret    
sub_15_05F4:
        test   byte ptr [0x6c8], 8           ; -> obj1:0x06C8
        je     loc_15_0618
        mov    edx, 0x3b4
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    dl, 0xb5
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    dl, 0xba
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_15_0618:
        mov    edx, 0x3c0
loc_15_061D:
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        cmp    dl, 0xe0
        jb     loc_15_061D
        ret    
sub_15_062A:
        test   byte ptr [0x6c8], 8           ; -> obj1:0x06C8
        je     loc_15_064E
        mov    edx, 0x3b4
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    dl, 0xb5
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    dl, 0xba
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_15_064E:
        mov    edx, 0x3c0
loc_15_0653:
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    edx
        cmp    dl, 0xe0
        jb     loc_15_0653
        ret    
sub_15_0660:
        pushfd 
        pushal 
        cmp    byte ptr [0x7a1], 0           ; -> obj1:0x07A1
        je     loc_15_06BF
        call   sub_15_0865
        test   byte ptr [0x7a0], 3           ; -> loc_1_07A0
        jne    loc_15_06B8
        mov    cl, 1
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x28], 0
        je     loc_15_068A
        call   dword ptr [edi + 0x28]
loc_15_068A:
        pop    edi
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_119E
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x30], 0
        je     loc_15_06A7
        call   dword ptr [edi + 0x30]
loc_15_06A7:
        pop    edi
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_06B7
        call   dword ptr [edi + 0xc]
loc_15_06B7:
        pop    edi
loc_15_06B8:
        mov    byte ptr [0x7a1], 0           ; -> obj1:0x07A1
loc_15_06BF:
        popal  
        popfd  
        ret    
loc_15_06C2:
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_0779
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_0779
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_0732
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_06F4
        call   dword ptr [edi + 0xc]
loc_15_06F4:
        pop    edi
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x38], 0
        je     loc_15_0704
        call   dword ptr [edi + 0x38]
loc_15_0704:
        pop    edi
        xor    cl, cl
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x28], 0
        je     loc_15_0716
        call   dword ptr [edi + 0x28]
loc_15_0716:
        pop    edi
        push   esi
        mov    ecx, ebx
        mov    esi, ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x24], 0
        je     loc_15_072B
        call   dword ptr [edi + 0x24]
loc_15_072B:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        jmp    loc_15_0779
loc_15_0732:
        push   eax
        push   ecx
        push   edx
        push   ebp
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_0745
        call   dword ptr [edi + 0xc]
loc_15_0745:
        pop    edi
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x38], 0
        je     loc_15_0755
        call   dword ptr [edi + 0x38]
loc_15_0755:
        pop    edi
        xor    cl, cl
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x28], 0
        je     loc_15_0767
        call   dword ptr [edi + 0x28]
loc_15_0767:
        pop    edi
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_119E
        pop    ebp
        pop    edx
        pop    ecx
        pop    eax
loc_15_0779:
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xb0], 0
        je     loc_15_0793
        popfd  
        call   dword ptr [edi + 0xb0]
        jmp    loc_15_0794
loc_15_0793:
        popfd  
loc_15_0794:
        pop    edi
        jb     loc_15_079D
        VxDCall  VDD.VDD_Do_Physical_IO          ; cd2015000a00
loc_15_079D:
        ret    
sub_15_079E:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_083D
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_07FC
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_07C4
        call   dword ptr [edi + 0xc]
loc_15_07C4:
        pop    edi
        and    dword ptr [0x109c], 0x7fffffff ; -> obj1:0x109C
        xor    cl, cl
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x28], 0
        je     loc_15_07E0
        call   dword ptr [edi + 0x28]
loc_15_07E0:
        pop    edi
        push   esi
        mov    ecx, ebx
        mov    esi, ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x24], 0
        je     loc_15_07F5
        call   dword ptr [edi + 0x24]
loc_15_07F5:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        jmp    loc_15_083D
loc_15_07FC:
        push   eax
        push   ecx
        push   edx
        push   ebp
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_080F
        call   dword ptr [edi + 0xc]
loc_15_080F:
        pop    edi
        and    dword ptr [0x109c], 0x7fffffff ; -> obj1:0x109C
        xor    cl, cl
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x28], 0
        je     loc_15_082B
        call   dword ptr [edi + 0x28]
loc_15_082B:
        pop    edi
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_119E
        pop    ebp
        pop    edx
        pop    ecx
        pop    eax
loc_15_083D:
        ret    
sub_15_083E:
        push   ebx
        xor    eax, eax
        mov    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
loc_15_0847:
        push   ecx
        push   eax
        call   sub_15_2D54
        jb     loc_15_085D
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_085D
        call   sub_15_2D96
loc_15_085D:
        pop    eax
        pop    ecx
        inc    eax
        dec    ecx
        jne    loc_15_0847
        pop    ebx
        ret    
sub_15_0865:
        push   ebx
        push   edi
        mov    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        mov    eax, dword ptr [0xecc]        ; -> obj1:0x0ECC
        mov    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
        sub    ecx, eax
        jle    loc_15_08AA
loc_15_087C:
        push   ebx
        push   ecx
        push   eax
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_2D54
        jb     loc_15_08A3
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_08A3
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_3EDC
loc_15_08A3:
        pop    eax
        pop    ecx
        pop    ebx
        inc    eax
        dec    ecx
        jne    loc_15_087C
loc_15_08AA:
        pop    edi
        pop    ebx
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_0798               ; fixup
loc_15_08B5:
        pushal 
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_09F4
        mov    ebp, dword ptr [ebx + 8]
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        xor    eax, eax
        test   byte ptr [edi + 1], 0x20
        je     loc_15_08D6
        dec    eax
loc_15_08D6:
        mov    ecx, dword ptr [edi + 0x620]
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xac], 0
        je     loc_15_08F6
        popfd  
        call   dword ptr [edi + 0xac]
        jmp    loc_15_08F7
loc_15_08F6:
        popfd  
loc_15_08F7:
        pop    edi
        jb     loc_15_09FB
        test   byte ptr [edi + 1], 0x20
        jne    loc_15_0926
        cmp    dword ptr [edi + 0x620], 0x13
        jbe    loc_15_09F4
        cmp    dword ptr [0x77c], 0          ; -> obj1:0x077C
        je     loc_15_09FB
        or     eax, eax
        jne    loc_15_09FB
loc_15_0926:
        test   word ptr [0xb1b], 0x800       ; -> obj9:0x0B1B
        je     loc_15_09FB
        mov    ecx, dword ptr [edi + 0x61c]
        cmp    ecx, -1
        je     loc_15_09FB
        or     ecx, ecx
        je     loc_15_09FB
        push   ecx
        mov    eax, dword ptr [edi + 0x614]
        or     eax, eax
        je     loc_15_0981
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        mov    dword ptr [edi + 0x614], 0
        mov    dword ptr [edi + 0x618], 0
        mov    dword ptr [edi + 0x62c], 0
loc_15_0981:
        pop    ecx
        add    ecx, 0xfff
        shr    ecx, 0xc
        push   0
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    ecx, eax
        or     ecx, edx
        je     loc_15_09FB
        mov    dword ptr [edi + 0x614], eax
        mov    dword ptr [edi + 0x618], edx
        or     byte ptr [0x7a2], 1           ; -> loc_1_07A2
        mov    esi, 0xaec                    ; -> loc_15_0AEC
        mov    eax, 0x1000
        mov    ecx, 1
        xor    edx, edx
        xor    edi, edi
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
loc_15_09D4:
        test   byte ptr [0x7a2], 1           ; -> loc_1_07A2
        je     loc_15_09F4
        mov    eax, 0x79c                    ; -> loc_1_079C
        mov    dword ptr [0x79c], eax        ; -> loc_1_079C
        push   1
        push   eax
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
        jmp    loc_15_09D4
loc_15_09F4:
        popal  
        jmp    loc_1_0798
loc_15_09FB:
        popal  
        stc    
        ret    
sub_15_09FE:
        push   eax
        push   edi
        push   ebp
        mov    ecx, 0xffffffff
        cmp    byte ptr [0x7a3], 0           ; -> obj1:0x07A3
        jne    loc_15_0AE8
        mov    byte ptr [0x7a3], 0xff        ; -> obj1:0x07A3
        mov    ebp, dword ptr [ebx + 8]
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x90], 0
        je     loc_15_0A37
        popfd  
        call   dword ptr [edi + 0x90]
        jmp    loc_15_0A38
loc_15_0A37:
        popfd  
loc_15_0A38:
        pop    edi
        jb     loc_15_0ACC
        mov    ecx, 0x100
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_15_0ADC
        cmp    ecx, 0x100
        jb     loc_15_0AD5
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp], di
        ror    edi, 0x10
        mov    word ptr [ebp + 0x38], di
        mov    word ptr [ebp + 0x1c], 0x4f00
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    di, word ptr [ebp + 0x1c]
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    di, 0x4f
        jne    loc_15_0AD5
        movzx  eax, word ptr [eax + 0x12]
        shl    eax, 0x10
        or     eax, eax
        je     loc_15_0AD5
        mov    ecx, 0x100
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        mov    ecx, eax
loc_15_0ACC:
        mov    byte ptr [0x7a3], 0           ; -> obj1:0x07A3
        jmp    loc_15_0AE8
loc_15_0AD5:
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_15_0ADC:
        mov    byte ptr [0x7a3], 0           ; -> obj1:0x07A3
        mov    ecx, 0xffffffff
loc_15_0AE8:
        pop    ebp
        pop    edi
        pop    eax
        ret    
loc_15_0AEC:
        cld    
        pushal 
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    dword ptr [edi + 0x62c], 0
        xor    ecx, ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jb     loc_15_0B1D
        xor    eax, eax
        VxDCall  VMD.Service_000Ah               ; cd200a000c00
loc_15_0B1D:
        mov    edx, 0x21
        mov    ecx, 0
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        mov    byte ptr [edi + 0x613], al
        or     al, 1
        mov    edx, 0x21
        mov    ecx, 4
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x80], 0
        je     loc_15_0B5F
        popfd  
        call   dword ptr [edi + 0x80]
        jmp    loc_15_0B60
loc_15_0B5F:
        popfd  
loc_15_0B60:
        pop    edi
        jb     loc_15_0B77
        push   ebx
        mov    ax, 0x4f05
        mov    bx, 0x100
        xor    edx, edx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
loc_15_0B77:
        mov    word ptr [edi + 0x60f], dx
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x84], 0
        je     loc_15_0B98
        popfd  
        call   dword ptr [edi + 0x84]
        jmp    loc_15_0B99
loc_15_0B98:
        popfd  
loc_15_0B99:
        pop    edi
        jb     loc_15_0BB0
        push   ebx
        mov    ax, 0x4f05
        mov    bx, 0x101
        xor    edx, edx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
loc_15_0BB0:
        mov    word ptr [edi + 0x611], dx
        mov    edx, dword ptr [edi + 0x620]
        and    dh, 0x3f
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x94], 0
        je     loc_15_0BDA
        popfd  
        call   dword ptr [edi + 0x94]
        jmp    loc_15_0BDB
loc_15_0BDA:
        popfd  
loc_15_0BDB:
        pop    edi
        jae    loc_15_0BFF
        mov    ecx, eax
        mov    eax, dword ptr [0xec8]        ; -> obj1:0x0EC8
        or     ecx, ecx
        je     loc_15_0CA3
        push   0
        push   edx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        jmp    loc_15_0CA3
loc_15_0BFF:
        mov    ecx, 0x100
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_15_0DED
        cmp    ecx, 0x100
        jb     loc_15_0C84
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp], di
        ror    edi, 0x10
        mov    word ptr [ebp + 0x38], di
        mov    word ptr [ebp + 0x18], dx
        mov    word ptr [ebp + 0x1c], 0x4f01
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    dx, word ptr [ebp + 0x1c]
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    dx, 0x4f
        je     loc_15_0C90
        mov    ecx, 0x100
loc_15_0C84:
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        jmp    loc_15_0DED
loc_15_0C90:
        movzx  edx, word ptr [eax + 4]
        shl    edx, 0xa
        mov    eax, dword ptr [0xec8]        ; -> obj1:0x0EC8
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_15_0CA3:
        push   eax
        push   ebx
        push   edx
        push   ebp
        push   edi
        mov    eax, 0x4f07
        mov    ebx, 1
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        mov    word ptr [edi + 0x634], 0xffff
        mov    word ptr [edi + 0x636], 0xffff
        cmp    ax, 0x4f
        jne    loc_15_0CE1
        mov    word ptr [edi + 0x634], cx
        mov    word ptr [edi + 0x636], dx
loc_15_0CE1:
        pop    ebp
        pop    edx
        pop    ebx
        pop    eax
        push   ebx
        push   edx
        mov    dword ptr [edi + 0x62c], edx
        mov    dword ptr [edi + 0x630], eax
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jb     loc_15_0D0B
        mov    eax, 3
        VxDCall  VMD.Service_0005h               ; cd2005000c00
loc_15_0D0B:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x9c], 0
        je     loc_15_0D20
        call   dword ptr [edi + 0x9c]
loc_15_0D20:
        pop    edi
        xor    edx, edx
        xor    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x88], 0
        je     loc_15_0D3E
        popfd  
        call   dword ptr [edi + 0x88]
        jmp    loc_15_0D3F
loc_15_0D3E:
        popfd  
loc_15_0D3F:
        pop    edi
        jb     loc_15_0D63
        mov    ax, 0x4f05
        xor    ebx, ebx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ax, 0x4f05
        mov    ebx, 1
        xor    edx, edx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_15_0D63:
        pop    edx
        shr    edx, 2
        mov    ecx, dword ptr [edi + 0x61c]
        shr    ecx, 2
        mov    ebx, edx
        xor    edx, edx
        mov    esi, dword ptr [edi + 0x630]
        mov    edi, dword ptr [edi + 0x618]
loc_15_0D80:
        push   ecx
        push   esi
        mov    ecx, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    ecx
        sub    ecx, ebx
        je     loc_15_0DD6
        inc    edx
        clc    
        mov    eax, edx
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x88], 0
        je     loc_15_0DA9
        popfd  
        call   dword ptr [edi + 0x88]
        jmp    loc_15_0DAA
loc_15_0DA9:
        popfd  
loc_15_0DAA:
        pop    edi
        jb     loc_15_0D80
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x4f05
        xor    ebx, ebx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        push   edx
        mov    ax, 0x4f05
        mov    ebx, 1
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        pop    ecx
        pop    ebx
        jmp    loc_15_0D80
loc_15_0DD6:
        pop    ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xa0], 0
        je     loc_15_0DEC
        call   dword ptr [edi + 0xa0]
loc_15_0DEC:
        pop    edi
loc_15_0DED:
        and    byte ptr [0x7a2], 0xfe        ; -> loc_1_07A2
        push   dword ptr [0x79c]             ; -> loc_1_079C
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        mov    dword ptr [0x79c], 0          ; -> loc_1_079C
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        popal  
        ret    
loc_15_0E15:
        cld    
        pushal 
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        xor    ecx, ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        cmp    dword ptr [edi + 0x62c], 0
        jne    loc_15_0E56
        xor    edi, edi
        mov    esi, 0x2e8                    ; -> loc_1_02E8
        mov    edx, ebx
        mov    eax, 0x30
        movzx  ecx, word ptr [0x14]          ; -> loc_3_0014
        add    ecx, dword ptr [4]            ; -> loc_3_0004
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        jmp    loc_15_10C2
loc_15_0E56:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x10], 0
        je     loc_15_0E65
        call   dword ptr [edi + 0x10]
loc_15_0E65:
        pop    edi
        or     byte ptr [0x7a0], 2           ; -> loc_1_07A0
        call   sub_15_05F4
        mov    eax, dword ptr [edi + 0x620]
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x98], 0
        je     loc_15_0E92
        popfd  
        call   dword ptr [edi + 0x98]
        jmp    loc_15_0E93
loc_15_0E92:
        popfd  
loc_15_0E93:
        pop    edi
        jb     loc_15_0EC2
        push   ebx
        mov    ebx, eax
        mov    eax, 0x4f02
        or     bh, 0x80
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
        cmp    ax, 0x4f
        jne    loc_15_1030
        and    byte ptr [0x449], 0x7f
        and    byte ptr [0x487], 0x7f
loc_15_0EC2:
        push   ebx
        push   edi
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x9c], 0
        je     loc_15_0ED9
        call   dword ptr [edi + 0x9c]
loc_15_0ED9:
        pop    edi
        mov    edx, dword ptr [edi + 0x62c]
        or     edx, edx
        je     loc_15_0F9C
        push   edx
        xor    edx, edx
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x88], 0
        je     loc_15_0F04
        popfd  
        call   dword ptr [edi + 0x88]
        jmp    loc_15_0F05
loc_15_0F04:
        popfd  
loc_15_0F05:
        pop    edi
        jb     loc_15_0F29
        mov    ax, 0x4f05
        xor    ebx, ebx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ax, 0x4f05
        mov    ebx, 1
        xor    edx, edx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_15_0F29:
        pop    edx
        shr    edx, 2
        mov    ecx, dword ptr [edi + 0x61c]
        shr    ecx, 2
        mov    esi, dword ptr [edi + 0x618]
        mov    edi, dword ptr [edi + 0x630]
        mov    ebx, edx
        xor    edx, edx
loc_15_0F46:
        push   ecx
        push   edi
        mov    ecx, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    ecx
        sub    ecx, ebx
        je     loc_15_0F9C
        inc    edx
        mov    eax, edx
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x88], 0
        je     loc_15_0F6F
        popfd  
        call   dword ptr [edi + 0x88]
        jmp    loc_15_0F70
loc_15_0F6F:
        popfd  
loc_15_0F70:
        pop    edi
        jb     loc_15_0F46
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x4f05
        xor    ebx, ebx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        push   edx
        mov    ax, 0x4f05
        mov    ebx, 1
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        pop    ecx
        pop    ebx
        jmp    loc_15_0F46
loc_15_0F9C:
        pop    edi
        pop    ebx
        call   sub_15_1657
        mov    edx, 0x3d4
        call   sub_15_1639
        cmp    word ptr [edi + 0x634], -1
        je     loc_15_0FDA
        push   ebx
        push   ebp
        push   edi
        mov    eax, 0x4f07
        xor    ebx, ebx
        mov    cx, word ptr [edi + 0x634]
        mov    dx, word ptr [edi + 0x636]
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        pop    ebp
        pop    ebx
loc_15_0FDA:
        movzx  edx, word ptr [edi + 0x60f]
        movzx  eax, word ptr [edi + 0x611]
        clc    
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x88], 0
        je     loc_15_0FFE
        call   dword ptr [edi + 0x88]
loc_15_0FFE:
        pop    edi
        jb     loc_15_1030
        push   ebx
        mov    ax, 0x4f05
        xor    bx, bx
        movzx  edx, word ptr [edi + 0x60f]
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ax, 0x4f05
        mov    bx, 1
        movzx  edx, word ptr [edi + 0x611]
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
loc_15_1030:
        mov    eax, dword ptr [edi + 0x614]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        mov    dword ptr [edi + 0x614], 0
        mov    dword ptr [edi + 0x618], 0
        mov    dword ptr [edi + 0x62c], 0
        push   edi
        and    byte ptr [0x7a0], 0xfd        ; -> loc_1_07A0
        call   sub_15_4BBC
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xa0], 0
        je     loc_15_1082
        call   dword ptr [edi + 0xa0]
loc_15_1082:
        pop    edi
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x14], 0
        je     loc_15_1092
        call   dword ptr [edi + 0x14]
loc_15_1092:
        pop    edi
        pop    edi
        mov    al, byte ptr [edi + 0x613]
        mov    edx, 0x21
        mov    ecx, 4
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jb     loc_15_10C2
        mov    eax, 1
        VxDCall  VMD.Service_000Ah               ; cd200a000c00
loc_15_10C2:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        popal  
        ret    
        db  000h, 000h
sub_15_10CC:
        bts    dword ptr [0x6c4], 0xc        ; -> obj1:0x06C4
        jb     loc_15_10E9
        and    byte ptr [0x6c5], 0x9f        ; -> obj1:0x06C5
        push   eax
        mov    eax, dword ptr [0x7ac]        ; -> loc_1_07AC
        mov    dword ptr [0x7b4], eax        ; -> loc_1_07B4
        pop    eax
loc_15_10E9:
        ret    
sub_15_10EA:
        btr    dword ptr [0x6c4], 0xc        ; -> obj1:0x06C4
        jae    loc_15_1136
        push   eax
        mov    eax, dword ptr [0x7b4]        ; -> loc_1_07B4
        cmp    eax, ebx
        jne    loc_15_1100
        xor    eax, eax
loc_15_1100:
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        je     loc_15_1114
        btr    dword ptr [0x6c4], 0xd        ; -> obj1:0x06C4
        jae    loc_15_1130
        jmp    loc_15_1126
loc_15_1114:
        cmp    ebx, dword ptr [0x7b4]        ; -> loc_1_07B4
        btr    dword ptr [0x6c4], 0xd        ; -> obj1:0x06C4
        jne    loc_15_1126
        jae    loc_15_1135
loc_15_1126:
        mov    dword ptr [0x7b0], 0          ; -> loc_1_07B0
loc_15_1130:
        mov    dword ptr [0x7ac], eax        ; -> loc_1_07AC
loc_15_1135:
        pop    eax
loc_15_1136:
        call   sub_15_119E
        and    byte ptr [0x6c5], 0xbf        ; -> obj1:0x06C5
        ret    
sub_15_1143:
        push   eax
        mov    eax, ebx
        xchg   dword ptr [0x7b0], eax        ; -> loc_1_07B0
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        je     loc_15_117A
        or     eax, eax
        je     loc_15_1191
        bts    dword ptr [0x6c4], 0xd        ; -> obj1:0x06C4
        jb     loc_15_1191
        push   ebx
        push   edi
        push   esi
        mov    ebx, eax
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_1AA5
        pop    esi
        pop    edi
        pop    ebx
        jmp    loc_15_1191
loc_15_117A:
        or     ebx, ebx
        je     loc_15_1191
        cmp    ebx, eax
        je     loc_15_1191
        push   ebx
        mov    ebx, edi
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        call   sub_15_19BD
        pop    ebx
loc_15_1191:
        pop    eax
        ret    
sub_15_1193:
        mov    dword ptr [0x7ac], 0          ; -> loc_1_07AC
        ret    
sub_15_119E:
        pushal 
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        je     loc_15_12D3
        cmp    dword ptr [0x7ac], 0          ; -> loc_1_07AC
        je     loc_15_11C1
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_11C1
        call   sub_15_1969
loc_15_11C1:
        push   ebx
        push   edi
        push   dword ptr [0x7ac]             ; -> loc_1_07AC
        mov    dword ptr [0x7ac], ebx        ; -> loc_1_07AC
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_1240
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        jne    loc_15_11E5
        call   sub_15_4BBC
loc_15_11E5:
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_1240
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x34], 0
        je     loc_15_11FC
        call   dword ptr [edi + 0x34]
loc_15_11FC:
        pop    edi
        or     dword ptr [0x109c], 0x80000000 ; -> obj1:0x109C
        cmp    byte ptr [0x7a1], 0           ; -> obj1:0x07A1
        jne    loc_15_1240
        cmp    byte ptr [0xfb0], 0           ; -> loc_1_0FB0
        je     loc_15_1224
        mov    esi, 0x660                    ; -> sub_15_0660
        VxDCall  Dev_011F.Service_0001h          ; cd2001001f01
loc_15_1224:
        call   sub_15_3023
        mov    byte ptr [0x7a1], 0xff        ; -> obj1:0x07A1
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_15_123F
        call   dword ptr [edi + 0x2c]
loc_15_123F:
        pop    edi
loc_15_1240:
        pop    ebx
        or     ebx, ebx
        je     loc_15_1284
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_1284
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        jne    loc_15_1284
        xor    eax, eax
        xchg   dword ptr [0x7ac], eax        ; -> loc_1_07AC
        push   eax
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    ebx, dword ptr [0x7b0]        ; -> loc_1_07B0
        jne    loc_15_1274
        call   sub_15_1AA5
loc_15_1274:
        call   sub_15_3A50
        pop    dword ptr [0x7ac]             ; -> loc_1_07AC
        call   sub_15_4BBC
loc_15_1284:
        pop    edi
        pop    ebx
        mov    esi, edi
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_12A2
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        or     ebx, ebx
        je     loc_15_12A2
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
loc_15_12A2:
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_12C8
        xor    eax, eax
        xchg   dword ptr [0x7ac], eax        ; -> loc_1_07AC
        push   eax
        push   ebx
        mov    ebx, esi
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        call   sub_15_1143
        pop    ebx
        pop    dword ptr [0x7ac]             ; -> loc_1_07AC
loc_15_12C8:
        call   sub_15_147C
        stc    
        call   sub_15_1857
loc_15_12D3:
        popal  
        ret    
sub_15_12D5:
        pushal 
        mov    dword ptr [0xe9a], ebx        ; -> loc_1_0E9A
        mov    ecx, dword ptr [0x7a8]        ; -> loc_1_07A8
        cmp    ebx, ecx
        je     loc_15_1469
        push   edi
        push   ebx
        cmp    dword ptr [0xf00], 0          ; -> loc_1_0F00
        je     loc_15_138F
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [0xe9e], ebx        ; -> loc_1_0E9E
loc_15_1305:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [ebx], 2
        je     loc_15_137D
        test   byte ptr [edi], 0x80
        jne    loc_15_137D
        test   byte ptr [edi + 2], 2
        jne    loc_15_137D
        call   sub_15_3A9D
        jae    loc_15_137D
        mov    eax, ebx
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        xchg   ebx, eax
        cmp    eax, ebx
        je     loc_15_137D
        push   ebx
        mov    ebx, dword ptr [0xe9a]        ; -> loc_1_0E9A
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_1372
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_1372
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_1372
        or     ebx, ebx
        je     loc_15_1372
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jb     loc_15_1372
        mov    eax, 3
        VxDCall  VMD.Service_0005h               ; cd2005000c00
loc_15_1372:
        pop    ebx
        VMMCall  VMM.Suspend_VM                  ; cd202b000100
        or     byte ptr [edi + 2], 0x20
loc_15_137D:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [0xe9e]        ; -> loc_1_0E9E
        jne    loc_15_1305
loc_15_138F:
        call   sub_15_083E
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_13A3
        call   dword ptr [edi + 0xc]
loc_15_13A3:
        pop    edi
        pop    ebx
        pop    edi
        mov    eax, dword ptr [0x7a8]        ; -> loc_1_07A8
        cmp    eax, dword ptr [0x7ac]        ; -> loc_1_07AC
        je     loc_15_13C6
        push   ebx
        push   edi
        mov    ebx, eax
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_119E
        pop    edi
        pop    ebx
loc_15_13C6:
        call   sub_15_1969
        mov    esi, edi
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        je     loc_15_13EA
        cmp    dword ptr [0x7ac], 0          ; -> loc_1_07AC
        je     loc_15_13EA
        mov    esi, dword ptr [0x7ac]        ; -> loc_1_07AC
        add    esi, dword ptr [0x6cc]        ; -> obj1:0x06CC
loc_15_13EA:
        call   sub_15_147C
        cmp    dword ptr [0xf00], 0          ; -> loc_1_0F00
        je     loc_15_142C
        push   edi
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   ebx
loc_15_1401:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi + 2], 0x20
        je     loc_15_141E
        and    byte ptr [edi + 2], 0xdf
        test   byte ptr [edi], 0x80
        jne    loc_15_141E
        VMMCall  VMM.Resume_VM                   ; cd202c000100
loc_15_141E:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_15_1401
        pop    ebx
        pop    ebx
        pop    edi
loc_15_142C:
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        push   dword ptr [0x7a8]             ; -> loc_1_07A8
        mov    dword ptr [0x7a8], ebx        ; -> loc_1_07A8
        je     loc_15_1453
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        clc    
        call   sub_15_1857
        call   sub_15_4BBC
loc_15_1453:
        pop    ebx
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_1469
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_4BBC
loc_15_1469:
        popal  
        ret    
sub_15_146B:
        push   eax
        mov    edx, 0x3cc
        in     al, dx
        mov    dl, 0xd4
        test   al, 1
        pop    eax
        jne    loc_15_147B
        mov    dl, 0xb4
loc_15_147B:
        ret    
sub_15_147C:
        pushal 
        cld    
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_1637
        call   sub_15_146B
        push   eax
        push   ecx
        mov    eax, dword ptr [0x7a8]        ; -> loc_1_07A8
        mov    ecx, dword ptr [0x7ac]        ; -> loc_1_07AC
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x3c], 0
        je     loc_15_14AC
        call   dword ptr [edi + 0x3c]
loc_15_14AC:
        pop    edi
        pop    ecx
        pop    eax
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_155D
        call   sub_15_037C
        jb     loc_15_1637
        call   sub_15_146B
        push   edx
        mov    edx, 0x3c4
        mov    eax, 0x100
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [edi + 0x67]
        out    dx, ax
        mov    al, 3
        mov    ah, byte ptr [edi + 0x69]
        out    dx, ax
        mov    dl, 0xce
        mov    al, 6
        mov    ah, byte ptr [edi + 0x7c]
        out    dx, ax
        mov    dl, 0xc4
        xor    al, al
        mov    ah, byte ptr [edi + 0x66]
        out    dx, ax
        pop    edx
        push   esi
        call   sub_15_1639
        push   edx
        add    edx, 6
        in     al, dx
        mov    dl, 0xc0
        mov    al, 0x10
        out    dx, al
        mov    al, byte ptr [edi + 0x60]
        out    dx, al
        mov    al, 0x12
        out    dx, al
        mov    al, byte ptr [edi + 0x62]
        out    dx, al
        mov    al, 0x13
        out    dx, al
        mov    al, byte ptr [edi + 0x63]
        out    dx, al
        mov    al, 0x14
        out    dx, al
        mov    al, byte ptr [edi + 0x64]
        out    dx, al
        mov    dl, 0xc4
        mov    al, 1
        mov    ah, byte ptr [edi + 0x67]
        or     ah, 0x20
        out    dx, ax
        lea    esi, [edi + 0x50]
        mov    cl, 0x15
        xor    ch, ch
        mov    dl, 0xc0
loc_15_1537:
        mov    al, ch
        out    dx, al
        mov    al, byte ptr [esi]
        out    dx, al
        inc    esi
        inc    ch
        dec    cl
        jne    loc_15_1537
        call   sub_15_1657
        mov    dl, 0xc0
        mov    al, 0x11
        out    dx, al
        mov    al, byte ptr [edi + 0x61]
        out    dx, al
        mov    dl, 0xc4
        mov    al, 1
        mov    ah, byte ptr [edi + 0x67]
        out    dx, ax
        pop    edx
        pop    esi
loc_15_155D:
        push   esi
        push   edi
        push   edx
        lea    edi, [edi + 0xc]
        lea    esi, [esi + 0xc]
        mov    edx, 0x3c4
        mov    al, 2
        mov    ah, byte ptr [esi + 0x5c]
        out    dx, ax
        mov    al, byte ptr [esi + 0x5e]
        mov    ah, byte ptr [edi + 0x5e]
        and    ax, 0x10e
        or     ah, al
        mov    al, 4
        out    dx, ax
        mov    dl, 0xce
        mov    al, 0
        mov    ah, byte ptr [esi + 0x6a]
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [esi + 0x6b]
        out    dx, ax
        mov    al, 2
        mov    ah, byte ptr [esi + 0x6c]
        out    dx, ax
        mov    al, 3
        mov    ah, byte ptr [esi + 0x6d]
        out    dx, ax
        mov    al, 4
        mov    ah, byte ptr [esi + 0x6e]
        out    dx, ax
        mov    al, byte ptr [esi + 0x6f]
        mov    ah, byte ptr [edi + 0x6f]
        and    ax, 0x609f
        or     ah, al
        mov    al, 5
        out    dx, ax
        mov    al, byte ptr [esi + 0x70]
        mov    ah, byte ptr [edi + 0x70]
        and    ax, 0x1fe
        or     ah, al
        mov    al, 6
        out    dx, ax
        mov    al, 7
        mov    ah, byte ptr [esi + 0x71]
        out    dx, ax
        mov    al, 8
        mov    ah, byte ptr [esi + 0x72]
        out    dx, ax
        mov    dl, 0xc2
        mov    al, byte ptr [edi + 0x8a]
        mov    ah, byte ptr [esi + 0x8a]
        and    ax, 0x22dd
        or     ah, al
        mov    dl, 0xcc
        in     al, dx
        cmp    ah, al
        je     loc_15_15F5
        mov    al, ah
        mov    dl, 0xc2
        out    dx, al
loc_15_15F5:
        pop    edx
        pop    edi
        pop    esi
        test   dword ptr [esi], 0x4000
        je     loc_15_1611
        push   edx
        mov    edx, 0x3ce
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        or     al, 4
        out    dx, al
        pop    edx
loc_15_1611:
        cmp    esi, 0xdf8                    ; -> loc_1_0DF8
        je     loc_15_161F
        sub    esi, dword ptr [0x6cc]        ; -> obj1:0x06CC
loc_15_161F:
        mov    ecx, edi
        sub    ecx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x24], 0
        je     loc_15_1636
        call   dword ptr [edi + 0x24]
loc_15_1636:
        pop    edi
loc_15_1637:
        popal  
        ret    
sub_15_1639:
        mov    al, 0x11
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x7f
        out    dx, al
        dec    edx
        lea    esi, [edi + 0xc]
        mov    ecx, 0x19
        xor    al, al
loc_15_164C:
        mov    ah, byte ptr [esi]
        out    dx, ax
        inc    esi
        inc    al
        dec    ecx
        jne    loc_15_164C
        ret    
sub_15_1657:
        lea    esi, [edi + 0xa0]
        mov    ecx, 0x100
        test   byte ptr [0x6c4], 0x40        ; -> obj1:0x06C4
        je     loc_15_1670
        mov    ecx, 0x10
loc_15_1670:
        mov    edx, 0x3c8
loc_15_1675:
        mov    eax, dword ptr [esi]
        out    dx, al
        inc    dl
        shr    eax, 8
        out    dx, al
        shr    eax, 8
        out    dx, al
        shr    eax, 8
        out    dx, al
        add    esi, 4
        dec    dl
        dec    ecx
        jne    loc_15_1675
        ret    
sub_15_168F:
        movzx  eax, word ptr [edi + 0x18]
        xchg   al, ah
        test   byte ptr [edi + 0x20], 0x40
        je     loc_15_16A4
        lea    eax, [eax*4]
        jmp    loc_15_16AC
loc_15_16A4:
        test   byte ptr [edi + 0x23], 0x40
        jne    loc_15_16AC
        add    eax, eax
loc_15_16AC:
        ret    
sub_15_16AD:
        call   sub_1_028C
        jae    loc_15_16BC
        mov    byte ptr [edi + 0x5b3], 0x56
        ret    
loc_15_16BC:
        movzx  eax, byte ptr [edi + 0xd]
        inc    eax
        jmp    loc_15_16E8
sub_15_16C3:
        mov    byte ptr [edi + 0x5b3], 0xff
        call   sub_1_028C
        jae    loc_15_16D8
        mov    byte ptr [edi + 0x5b3], 0x56
loc_15_16D8:
        movzx  eax, byte ptr [edi + 0x1f]
        shl    eax, 1
        mov    cl, byte ptr [edi + 0x23]
        test   cl, 8
        je     loc_15_16E8
        shl    eax, 1
loc_15_16E8:
        push   edx
        push   eax
        movzx  eax, byte ptr [edi + 0x1e]
        mov    cl, byte ptr [edi + 0x13]
        shr    cl, 2
        adc    ah, 0
        shr    cl, 3
        and    cl, 2
        or     ah, cl
        inc    eax
        mov    cl, byte ptr [edi + 0x23]
        test   cl, 4
        je     loc_15_170A
        shl    eax, 1
loc_15_170A:
        mov    edx, eax
        mov    cl, byte ptr [edi + 0x15]
        test   cl, 0x80
        je     loc_15_1716
        shr    edx, 1
loc_15_1716:
        pop    eax
        test   byte ptr [edi + 0x7c], 1
        je     loc_15_1799
loc_15_171D:
        mov    cl, byte ptr [edi + 0x23]
        test   cl, 1
        jne    loc_15_1727
        shl    edx, 1
loc_15_1727:
        movzx  ecx, byte ptr [edi + 0x15]
        and    cl, 0x1f
        je     loc_15_173B
        inc    ecx
        push   eax
        mov    eax, edx
        xor    edx, edx
        div    ecx
        mov    edx, eax
        pop    eax
loc_15_173B:
        mov    cl, byte ptr [edi + 0x60]
        test   cl, 0x40
        je     loc_15_1785
        shr    eax, 1
        mov    byte ptr [edi + 0x5b3], 0x13
        test   byte ptr [edi + 0x6a], 8
        jne    loc_15_1785
        mov    byte ptr [edi + 0x5b3], 0x58
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_1785
        or     ebx, ebx
        je     loc_15_1785
        test   byte ptr [edi], 0x80
        jne    loc_15_1785
        pushal 
        VMMCall  VMM.Suspend_VM                  ; cd202b000100
        or     byte ptr [edi], 0x80
        or     byte ptr [edi + 9], 4
        or     byte ptr [0x6c4], 0x10        ; -> obj1:0x06C4
        call   sub_9_051B
        popal  
loc_15_1785:
        mov    cl, byte ptr [edi + 0x67]
        test   cl, 1
        mov    ecx, eax
        je     loc_15_1791
        xor    ecx, ecx
loc_15_1791:
        lea    ecx, [ecx + eax*8]
        mov    eax, edx
        stc    
        jmp    loc_15_17B2
loc_15_1799:
        movzx  ecx, byte ptr [edi + 0x15]
        and    cl, 0x1f
        cmp    cl, 2
        jbe    loc_15_171D
        inc    ecx
        xchg   edx, eax
        push   edx
        xor    edx, edx
        div    ecx
        pop    ecx
        clc    
loc_15_17B2:
        pop    edx
        ret    
sub_15_17B4:
        push   ebx
        push   edx
        push   edi
        mov    dx, 0x3da
        in     al, dx
        push   edx
        jmp    loc_15_17BF
loc_15_17BF:
        mov    dl, 0xc0
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        in     al, dx
        or     al, 0x20
        test   ebx, ebx
        je     loc_15_17F7
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_17F7
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        and    byte ptr [0x6c4], 0xfd        ; -> obj1:0x06C4
        push   edx
        call   sub_15_4BBC
        pop    edx
        and    byte ptr [edi + 1], 0x7f
        mov    al, byte ptr [edi + 0x4c]
        test   byte ptr [edi + 0x4c], 0x80
loc_15_17F7:
        out    dx, al
        jmp    loc_15_17FA
loc_15_17FA:
        pop    edx
        je     loc_15_17FE
        in     al, dx
loc_15_17FE:
        pop    edi
        pop    edx
        pop    ebx
        ret    
sub_15_1802:
        push   ebx
        push   edx
        push   edi
        mov    edx, 0x3da
        in     al, dx
        mov    dl, 0xc0
        in     al, dx
        mov    ah, al
        xor    al, al
        out    dx, al
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        test   ebx, ebx
        je     loc_15_1853
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_1853
        bts    dword ptr [0x6c4], 1          ; -> obj1:0x06C4
        jb     loc_15_1853
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        bts    dword ptr [edi], 0xf
        jb     loc_15_1847
        and    byte ptr [edi + 0x4c], 0x80
        and    ah, 0x7f
        or     byte ptr [edi + 0x4c], ah
loc_15_1847:
        call   sub_15_1969
        push   edx
        call   sub_15_4BBC
        pop    edx
loc_15_1853:
        pop    edi
        pop    edx
        pop    ebx
        ret    
sub_15_1857:
        push   edx
        mov    dx, 0x3ce
        mov    al, byte ptr [esi + 0x4f]
        out    dx, al
        jmp    loc_15_1862
loc_15_1862:
        mov    dl, 0xc4
        mov    al, byte ptr [esi + 0x4d]
        out    dx, al
        jmp    loc_15_186A
loc_15_186A:
        jb     loc_15_18A7
        call   sub_15_146B
        mov    al, byte ptr [edi + 0x4e]
        out    dx, al
        jmp    loc_15_1877
loc_15_1877:
        add    edx, 6
        in     al, dx
        jmp    loc_15_187D
loc_15_187D:
        mov    al, byte ptr [edi + 0x4c]
        and    al, 0xdf
        push   edx
        mov    dl, 0xc0
        test   byte ptr [edi + 0x4c], 0x20
        je     loc_15_1896
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_1896
        or     al, 0x20
loc_15_1896:
        out    dx, al
        jmp    loc_15_1899
loc_15_1899:
        pop    edx
        test   byte ptr [edi + 0x4c], 0x80
        je     loc_15_18A3
        in     al, dx
        jmp    loc_15_18A3
loc_15_18A3:
        and    byte ptr [edi + 1], 0x7f
loc_15_18A7:
        pop    edx
        ret    
sub_15_18A9:
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        jne    loc_15_18B9
        pushal 
        call   sub_15_4BBC
        popal  
loc_15_18B9:
        ret    
sub_15_18BA:
        call   sub_15_1969
        push   ebx
        push   edi
        xor    ebx, ebx
        xchg   dword ptr [0x7ac], ebx        ; -> loc_1_07AC
        or     ebx, ebx
        je     loc_15_1905
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_1905
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        jne    loc_15_1905
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    ebx, dword ptr [0x7b0]        ; -> loc_1_07B0
        jne    loc_15_18F3
        call   sub_15_1AA5
loc_15_18F3:
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        je     loc_15_1900
        call   sub_15_3A50
loc_15_1900:
        call   sub_15_4BBC
loc_15_1905:
        test   byte ptr [0x6c5], 1           ; -> obj1:0x06C5
        je     loc_15_1926
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    esi, 0xdf8                    ; -> loc_1_0DF8
        call   sub_15_147C
loc_15_1926:
        pop    edi
        pop    ebx
        bt     dword ptr [0x6c4], 8          ; -> obj1:0x06C4
        cmc    
        ret    
sub_15_1932:
        push   ebx
        push   edi
        xor    ebx, ebx
        mov    dword ptr [0x7ac], ebx        ; -> loc_1_07AC
        test   byte ptr [0x6c5], 1           ; -> obj1:0x06C5
        je     loc_15_195D
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    esi, 0xdf8                    ; -> loc_1_0DF8
        call   sub_15_147C
loc_15_195D:
        pop    edi
        pop    ebx
        bt     dword ptr [0x6c4], 8          ; -> obj1:0x06C4
        cmc    
        ret    
sub_15_1969:
        push   ebx
        push   edi
        mov    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        or     ebx, ebx
        je     loc_15_19BA
        cmp    ebx, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_19BA
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_19BA
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_199C
        test   byte ptr [edi + 2], 1
        jne    loc_15_19BA
loc_15_199C:
        test   byte ptr [0x6c5], 0x40        ; -> obj1:0x06C5
        jne    loc_15_19BA
        test   byte ptr [0x6c5], 0x10        ; -> obj1:0x06C5
        je     loc_15_19B5
        or     byte ptr [0x6c5], 0x40        ; -> obj1:0x06C5
loc_15_19B5:
        call   sub_15_1EAC
loc_15_19BA:
        pop    edi
        pop    ebx
        ret    
sub_15_19BD:
        test   byte ptr [esi], 2
        jne    loc_15_1AA4
        test   byte ptr [esi + 0x7c], 8
        jne    loc_15_1AA4
        cmp    dword ptr [0xf00], 0          ; -> loc_1_0F00
        je     loc_15_1A16
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_1AA4
        stc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x64], 0
        je     loc_15_19FA
        popfd  
        call   dword ptr [edi + 0x64]
        jmp    loc_15_19FB
loc_15_19FA:
        popfd  
loc_15_19FB:
        pop    edi
        jae    loc_15_1AA4
        mov    eax, esi
        sub    eax, dword ptr [0x6cc]        ; -> obj1:0x06CC
        cmp    eax, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_1AA4
loc_15_1A16:
        push   ebx
        mov    ebx, esi
        call   sub_15_1932
        call   sub_15_28C8
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 4
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        mov    cl, byte ptr [esi]
        inc    al
        out    dx, al
        mov    ch, byte ptr [esi]
        ror    ecx, 0x10
        inc    al
        out    dx, al
        mov    cl, byte ptr [esi]
        inc    al
        out    dx, al
        mov    ch, byte ptr [esi]
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    dl, 0xc4
        mov    al, 2
        out    dx, al
        inc    edx
        mov    al, 1
        out    dx, al
        mov    al, byte ptr [ebx + 0x4ab]
        mov    byte ptr [esi], al
        mov    al, 2
        out    dx, al
        mov    al, byte ptr [ebx + 0x4ac]
        mov    byte ptr [esi], al
        mov    al, 4
        out    dx, al
        mov    al, byte ptr [ebx + 0x4ad]
        mov    byte ptr [esi], al
        mov    al, 8
        out    dx, al
        mov    al, byte ptr [ebx + 0x4ae]
        mov    byte ptr [esi], al
        mov    al, byte ptr [esi]
        mov    byte ptr [esi], ch
        mov    al, 4
        out    dx, al
        mov    byte ptr [esi], cl
        ror    ecx, 0x10
        mov    al, 2
        out    dx, al
        mov    byte ptr [esi], ch
        mov    al, 1
        out    dx, al
        mov    byte ptr [esi], cl
        call   sub_15_28F6
        mov    esi, ebx
        pop    ebx
loc_15_1AA4:
        ret    
sub_15_1AA5:
        test   byte ptr [edi], 2
        jne    loc_15_1AB0
        test   byte ptr [edi + 0x7c], 8
        je     loc_15_1AB1
loc_15_1AB0:
        ret    
loc_15_1AB1:
        cmp    dword ptr [0xf00], 0          ; -> loc_1_0F00
        je     loc_15_1AEB
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_1AB0
        stc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x60], 0
        je     loc_15_1AD7
        popfd  
        call   dword ptr [edi + 0x60]
        jmp    loc_15_1AD8
loc_15_1AD7:
        popfd  
loc_15_1AD8:
        pop    edi
        jae    loc_15_1AB0
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_1AB0
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_1AF0
loc_15_1AEB:
        call   sub_15_1932
loc_15_1AF0:
        call   sub_15_28C8
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 5
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        ror    eax, 8
        and    al, 0x70
        or     al, 1
        out    dx, al
        dec    dl
        mov    byte ptr [esi], al
        mov    al, 4
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        xor    al, al
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 0x4ab], al
        mov    al, 1
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 0x4ac], al
        mov    al, 2
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 0x4ad], al
        mov    al, 3
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 0x4ae], al
        mov    al, ah
        out    dx, al
        dec    dl
        rol    eax, 8
        mov    ah, al
        mov    al, 5
        out    dx, ax
        rol    eax, 8
        out    dx, al
        call   sub_15_28F6
        ret    
sub_15_1B5E:
        pushal 
        call   sub_15_16AD
        jb     loc_15_1B94
        mov    byte ptr [edi + 0x5b3], 0
        cmp    cl, 0x28
        je     loc_15_1BC0
        mov    byte ptr [edi + 0x5b3], 3
        cmp    cl, 0x50
        jne    loc_15_1C32
        test   byte ptr [edi + 0x96], 1
        jne    loc_15_1BC0
        mov    byte ptr [edi + 0x5b3], 7
        jmp    loc_15_1BC0
loc_15_1B94:
        cmp    byte ptr [edi + 0x5b3], 0xff
        jne    loc_15_1BC0
        cmp    eax, 0xc8
        jne    loc_15_1BE7
        cmp    ecx, 0x140
        jne    loc_15_1BC9
        mov    byte ptr [edi + 0x5b3], 4
        test   byte ptr [edi + 0x6a], 4
        je     loc_15_1BC0
        mov    byte ptr [edi + 0x5b3], 0xd
loc_15_1BC0:
        popal  
        movzx  eax, byte ptr [edi + 0x5b3]
        ret    
loc_15_1BC9:
        cmp    ecx, 0x280
        jne    loc_15_1C32
        mov    byte ptr [edi + 0x5b3], 6
        test   byte ptr [edi + 0x23], 1
        je     loc_15_1BC0
        mov    byte ptr [edi + 0x5b3], 0xe
        jmp    loc_15_1BC0
loc_15_1BE7:
        cmp    ecx, 0x280
        jne    loc_15_1C32
        test   byte ptr [edi + 0x6a], 8
        jne    loc_15_1C32
        cmp    eax, 0x15e
        je     loc_15_1C19
        cmp    eax, 0x1e0
        jne    loc_15_1C32
        mov    byte ptr [edi + 0x5b3], 0x12
        test   byte ptr [edi + 0x23], 0x20
        jne    loc_15_1BC0
        mov    byte ptr [edi + 0x5b3], 0x11
        jmp    loc_15_1BC0
loc_15_1C19:
        mov    byte ptr [edi + 0x5b3], 0x10
        test   byte ptr [edi + 0x96], 1
        jne    loc_15_1BC0
        mov    byte ptr [edi + 0x5b3], 0xf
        jmp    loc_15_1BC0
loc_15_1C32:
        mov    byte ptr [edi + 0x5b3], 0xff
        jmp    loc_15_1BC0
sub_15_1C3B:
        push   ecx
        mov    eax, 0x10a0
        or     ebx, ebx
        je     loc_15_1C55
        movzx  ecx, byte ptr [edi + 0x7c]
        shr    ecx, 1
        and    cl, 6
        mov    ax, word ptr [ecx + 0xea4]    ; -> loc_1_0EA4
loc_15_1C55:
        pop    ecx
        ret    
sub_15_1C57:
        test   byte ptr [edi], 1
        je     loc_15_1C75
        test   byte ptr [edi + 3], 0x80
        je     loc_15_1C75
        test   byte ptr [edi], 4
        jne    loc_15_1C75
        call   sub_15_1D6D
        jne    loc_15_1C77
        call   sub_15_1CD3
        jb     loc_15_1C77
loc_15_1C75:
        clc    
        ret    
loc_15_1C77:
        stc    
        ret    
sub_15_1C79:
        and    byte ptr [edi + 0x5f0], 0xbf
        test   byte ptr [edi + 3], 0x80
        je     loc_15_1CA4
        test   byte ptr [edi], 4
        jne    loc_15_1CA4
        call   sub_15_1CD3
        jne    loc_15_1C9B
        pushfd 
        or     byte ptr [edi + 0x5f0], 0x40
        popfd  
loc_15_1C9B:
        jb     loc_15_1CAF
        call   sub_15_1D6D
        jne    loc_15_1CA8
loc_15_1CA4:
        xor    eax, eax
        jmp    loc_15_1CBE
loc_15_1CA8:
        mov    eax, 0x20
        jmp    loc_15_1CBE
loc_15_1CAF:
        mov    byte ptr [edi + 0x9d], al
        cmp    al, 0xb
        je     loc_15_1CA4
        mov    eax, 0x28
loc_15_1CBE:
        or     dword ptr [edi + 0x5f0], eax
        call   sub_15_1E91
        mov    eax, dword ptr [edi + 0x5f0]
        mov    word ptr [esi], ax
        ret    
sub_15_1CD3:
        call   sub_15_1B5E
        push   eax
        cmp    al, byte ptr [edi + 0x540]
        jne    loc_15_1D57
        test   byte ptr [edi + 0x4c], 0x20
        jne    loc_15_1CF2
        test   byte ptr [edi + 0x4ef], 0x20
        jne    loc_15_1D57
        jmp    loc_15_1CFB
loc_15_1CF2:
        test   byte ptr [edi + 0x4ef], 0x20
        je     loc_15_1D57
loc_15_1CFB:
        mov    al, byte ptr [edi + 0x15]
        cmp    al, byte ptr [edi + 0x4b8]
        jne    loc_15_1D57
        mov    al, byte ptr [edi + 0x1e]
        cmp    al, byte ptr [edi + 0x4c1]
        jne    loc_15_1D57
        mov    ax, word ptr [edi + 0x18]
        cmp    ax, word ptr [edi + 0x4bb]
        jne    loc_15_1D57
        btr    dword ptr [edi], 0x1c
        jb     loc_15_1D57
        mov    eax, dword ptr [edi + 0x50]
        cmp    eax, dword ptr [edi + 0x4f3]
        jne    loc_15_1D57
        mov    eax, dword ptr [edi + 0x54]
        cmp    eax, dword ptr [edi + 0x4f7]
        jne    loc_15_1D57
        mov    eax, dword ptr [edi + 0x58]
        cmp    eax, dword ptr [edi + 0x4fb]
        jne    loc_15_1D57
        mov    eax, dword ptr [edi + 0x5c]
        cmp    eax, dword ptr [edi + 0x4ff]
        jne    loc_15_1D57
        pop    eax
        test   byte ptr [edi + 0x4c], 0x20
        clc    
        ret    
loc_15_1D57:
        pop    eax
        test   byte ptr [edi + 0x4c], 0x20
        stc    
        ret    
sub_15_1D5E:
        mov    dword ptr [edi + 0x5f0], 0
        and    byte ptr [edi + 1], 0xfd
        ret    
sub_15_1D6D:
        mov    ax, word ptr [edi + 0x1a]
        cmp    ax, word ptr [edi + 0x4bd]
        jne    loc_15_1D85
        mov    ax, word ptr [edi + 0x16]
        cmp    ax, word ptr [edi + 0x4b9]
loc_15_1D85:
        ret    
loc_15_1D86:
        pushfd 
        lea    edx, [edi + 0x4af]
        jae    loc_15_1D92
        lea    edx, [edi + 0xc]
loc_15_1D92:
        call   sub_15_1B5E
        mov    byte ptr [esi], al
        call   sub_15_16AD
        jb     loc_15_1DAD
        mov    byte ptr [esi + 3], al
        mov    word ptr [esi + 0x1c], cx
        or     byte ptr [esi + 2], 8
        jmp    loc_15_1DB9
loc_15_1DAD:
        and    byte ptr [esi + 2], 0xf7
        mov    word ptr [esi + 0x1c], cx
        mov    word ptr [esi + 0x1e], ax
loc_15_1DB9:
        mov    al, byte ptr [edx + 0x55]
        mov    byte ptr [esi + 1], al
        popfd  
        jae    loc_15_1E1A
        call   sub_15_1E6D
        mov    word ptr [esi + 4], cx
        mov    word ptr [esi + 6], ax
        test   byte ptr [edi + 2], 8
        jne    loc_15_1E0C
        movzx  eax, byte ptr [edx + 0xa]
        and    al, 0x1f
        movzx  ecx, byte ptr [edx + 0xb]
        and    cl, 0x1f
        mov    ch, byte ptr [edx + 9]
        and    ch, 0xf
        cmp    al, ch
        ja     loc_15_1E0C
        cmp    al, cl
        jne    loc_15_1DF2
        inc    cl
loc_15_1DF2:
        or     cl, cl
        jne    loc_15_1DFA
        mov    cl, ch
        inc    cl
loc_15_1DFA:
        cmp    al, cl
        jb     loc_15_1E00
        xchg   cl, al
loc_15_1E00:
        cmp    al, ch
        jbe    loc_15_1E10
        mov    cl, ch
        inc    cl
        xor    al, al
        jmp    loc_15_1E10
loc_15_1E0C:
        xor    eax, eax
        mov    ecx, eax
loc_15_1E10:
        mov    word ptr [esi + 8], ax
        mov    ch, ah
        mov    word ptr [esi + 0xa], cx
loc_15_1E1A:
        mov    eax, dword ptr [edx + 0x44]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edx + 0x48]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edx + 0x4c]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [edx + 0x50]
        mov    dword ptr [esi + 0x18], eax
        and    byte ptr [esi + 2], 0xfb
        mov    eax, dword ptr [edi + 0xa0]
        or     eax, eax
        je     loc_15_1E44
        or     byte ptr [esi + 2], 4
loc_15_1E44:
        and    byte ptr [esi + 2], 0xfe
        test   byte ptr [edi + 3], 0x80
        jne    loc_15_1E54
        or     byte ptr [esi + 2], 1
        jmp    loc_15_1E5E
loc_15_1E54:
        test   byte ptr [edi + 9], 1
        je     loc_15_1E5E
        or     byte ptr [esi + 2], 1
loc_15_1E5E:
        and    byte ptr [esi + 2], 0xfd
        test   byte ptr [edi + 1], 2
        je     loc_15_1E6C
        or     byte ptr [esi + 2], 2
loc_15_1E6C:
        ret    
sub_15_1E6D:
        push   edx
        movzx  ecx, byte ptr [edx + 0xd]
        mov    ch, byte ptr [edx + 0xc]
        movzx  eax, byte ptr [edx + 0xf]
        mov    ah, byte ptr [edx + 0xe]
        sub    eax, ecx
        movzx  ecx, byte ptr [edx + 1]
        inc    cl
        xor    edx, edx
        or     ecx, ecx
        je     loc_15_1E8F
        div    cx
        mov    ecx, edx
loc_15_1E8F:
        pop    edx
        ret    
sub_15_1E91:
        pushfd 
        push   esi
        push   edi
        lea    esi, [edi + 0xc]
        lea    edi, [edi + 0x4af]
        mov    ecx, 0x25
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        popfd  
        ret    
        db  000h, 000h, 000h
sub_15_1EAC:
        pushal 
        mov    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_1EF8
        test   byte ptr [0x7a0], 3           ; -> loc_1_07A0
        jne    loc_15_1EF8
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_146B
        call   sub_15_1FA3
        call   sub_15_1FBC
        call   sub_15_1FED
        call   sub_15_2014
        call   sub_15_1EFA
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        je     loc_15_1EF8
        call   sub_15_203B
loc_15_1EF8:
        popal  
        ret    
sub_15_1EFA:
        push   edx
        add    edx, 6
        mov    si, 0x3c0
        in     al, dx
        xchg   esi, edx
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_1F1E
        test   byte ptr [edi + 1], 0x80
        jne    loc_15_1F1E
        in     al, dx
        and    byte ptr [edi + 0x4c], 0x80
        and    al, 0x7f
        or     byte ptr [edi + 0x4c], al
loc_15_1F1E:
        push   edi
        mov    ecx, 0x15
        xor    ah, ah
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_1F41
        test   byte ptr [edi + 0x4c], 0x20
        je     loc_15_1F38
        or     ah, 0x20
loc_15_1F38:
        add    edi, 0x10
        or     ah, 0x10
        sub    cl, 0x10
loc_15_1F41:
        lea    edi, [edi + 0x50]
        cld    
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_1F65
        test   ah, 0x10
        jne    loc_15_1F65
        push   ecx
        mov    ecx, 0x10000
        xchg   esi, edx
loc_15_1F5B:
        in     al, dx
        jmp    loc_15_1F5E
loc_15_1F5E:
        test   al, 8
        loope  loc_15_1F5B
        xchg   esi, edx
        pop    ecx
loc_15_1F65:
        xchg   esi, edx
        in     al, dx
        jmp    loc_15_1F6A
loc_15_1F6A:
        xchg   esi, edx
        mov    al, ah
        out    dx, al
        jmp    loc_15_1F71
loc_15_1F71:
        inc    edx
        in     al, dx
        jmp    loc_15_1F75
loc_15_1F75:
        dec    edx
        stosb  byte ptr es:[edi], al
        inc    ah
        loop   loc_15_1F65
        xchg   esi, edx
        in     al, dx
        jmp    loc_15_1F80
loc_15_1F80:
        xchg   esi, edx
        pop    edi
        mov    al, byte ptr [edi + 0x4c]
        and    al, 0x7f
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        je     loc_15_1F93
        and    al, 0xdf
loc_15_1F93:
        out    dx, al
        jmp    loc_15_1F96
loc_15_1F96:
        xchg   esi, edx
        test   byte ptr [edi + 0x4c], 0x80
        je     loc_15_1FA1
        in     al, dx
        jmp    loc_15_1FA1
loc_15_1FA1:
        pop    edx
        ret    
sub_15_1FA3:
        push   edx
        mov    dl, 0xcc
        in     al, dx
        jmp    loc_15_1FA9
loc_15_1FA9:
        mov    byte ptr [edi + 0x96], al
        mov    dl, 0xca
        in     al, dx
        jmp    loc_15_1FB4
loc_15_1FB4:
        mov    byte ptr [edi + 0x99], al
        pop    edx
        ret    
sub_15_1FBC:
        in     al, dx
        mov    byte ptr [edi + 0x4e], al
        push   edi
        mov    ecx, 0x19
        lea    edi, [edi + 0xc]
        xor    ah, ah
        cld    
loc_15_1FCC:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        stosb  byte ptr es:[edi], al
        inc    ah
        loop   loc_15_1FCC
        pop    edi
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x20], 0
        je     loc_15_1FE7
        call   dword ptr [edi + 0x20]
loc_15_1FE7:
        pop    edi
        mov    al, byte ptr [edi + 0x4e]
        out    dx, al
        ret    
sub_15_1FED:
        push   edx
        mov    dx, 0x3ce
        in     al, dx
        mov    byte ptr [edi + 0x4f], al
        mov    ecx, 9
        push   edi
        lea    edi, [edi + 0x76]
        xor    ah, ah
        cld    
loc_15_2002:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        stosb  byte ptr es:[edi], al
        inc    ah
        loop   loc_15_2002
        pop    edi
        mov    al, byte ptr [edi + 0x4f]
        out    dx, al
        pop    edx
        ret    
sub_15_2014:
        push   edx
        mov    dx, 0x3c4
        in     al, dx
        mov    byte ptr [edi + 0x4d], al
        push   edi
        mov    ecx, 5
        lea    edi, [edi + 0x66]
        xor    ah, ah
        cld    
loc_15_2029:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        stosb  byte ptr es:[edi], al
        inc    ah
        loop   loc_15_2029
        pop    edi
        mov    al, byte ptr [edi + 0x4d]
        out    dx, al
        pop    edx
        ret    
sub_15_203B:
        push   edx
        push   edi
        lea    edi, [edi + 0xa0]
        xor    ecx, ecx
        cld    
        mov    dx, 0x3c7
loc_15_204A:
        mov    al, cl
        out    dx, al
        add    edx, 2
        in     al, dx
        ror    eax, 8
        in     al, dx
        ror    eax, 8
        in     al, dx
        ror    eax, 8
        mov    al, cl
        mov    dword ptr [edi], eax
        add    edi, 4
        sub    edx, 2
        inc    cl
        jne    loc_15_204A
        pop    edi
        pop    edx
        ret    
        db  000h, 000h, 000h
sub_15_2070:
        test   byte ptr [edi + 0x5e2], 1
        je     loc_15_20D6
        mov    eax, dword ptr [0x40]
        cmp    eax, dword ptr [0xeb0]        ; -> obj1:0x0EB0
        jne    loc_15_20B2
        cmp    word ptr [0x44a], 0x50
        jne    loc_15_20B2
        mov    esi, 0xb8000
        cmp    byte ptr [0x449], 3
        je     loc_15_20B2
        test   byte ptr [edi + 4], 4
        je     loc_15_20D6
        cmp    byte ptr [0x449], 7
        jne    loc_15_20B2
        mov    esi, 0xb0000
loc_15_20B2:
        jne    loc_15_20D5
        add    esi, dword ptr [ebx + 4]
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_20D5
        cmp    byte ptr [0x462], 0
        jne    loc_15_20D3
        call   sub_15_4538
        mov    esi, dword ptr [edi + 0x547]
loc_15_20D3:
        cmp    esi, esi
loc_15_20D5:
        ret    
loc_15_20D6:
        or     al, 1
        ret    
loc_15_20D9:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_20F0
        xor    byte ptr [0xec5], 1           ; -> loc_1_0EC5
        jne    loc_15_20F0
        VxDCall  VDD.VDD_Check_Update_Soon       ; cd200d000a00
loc_15_20F0:
        stc    
        ret    
loc_15_20F2:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 1
        je     loc_15_2134
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 1
        je     loc_15_210C
        cmp    ah, 0x11
        jne    loc_15_2134
loc_15_210C:
        cmp    word ptr [edi + 0x5cc], 0
        je     loc_15_2134
        dec    word ptr [edi + 0x5ce]
        jne    loc_15_2134
        dec    word ptr [edi + 0x5cc]
        call   sub_15_4780
        or     byte ptr [edi + 3], 0x20
        call   sub_15_46DC
loc_15_2134:
        stc    
        ret    
sub_15_2136:
        mov    eax, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    eax, ebx
        mov    ecx, dword ptr [eax + 0x5c8]
        cmp    ecx, -1
        je     loc_15_2173
        xchg   ch, cl
        ror    ecx, 8
        cmp    dl, ch
        jae    loc_15_2153
        mov    ch, dl
loc_15_2153:
        cmp    dl, cl
        jbe    loc_15_2159
        mov    cl, dl
loc_15_2159:
        ror    ecx, 0x10
        cmp    dh, cl
        jae    loc_15_2162
        mov    cl, dh
loc_15_2162:
        cmp    dh, ch
        jbe    loc_15_2168
        mov    ch, dh
loc_15_2168:
        ror    ecx, 8
        xchg   cl, ch
        mov    dword ptr [eax + 0x5c8], ecx
loc_15_2173:
        ret    
loc_15_2174:
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_225B
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_21D8
        test   byte ptr [0x6c5], 2           ; -> obj1:0x06C5
        jne    loc_15_21D8
        bts    dword ptr [0x6c4], 0xa        ; -> obj1:0x06C4
        jb     loc_15_21D8
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 3
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_15_21D8:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 2
        je     loc_15_23DD
        cmp    ah, 3
        je     loc_15_23B6
        cmp    ah, 6
        je     loc_15_247B
        cmp    ah, 7
        je     loc_15_247B
        cmp    ah, 9
        je     loc_15_225D
        cmp    ah, 0xe
        je     loc_15_22B4
        or     ah, ah
        je     loc_15_2237
        cmp    ah, 0x4f
        je     loc_15_25A1
        cmp    ah, 0x11
        jne    loc_15_225B
        cmp    al, 3
        je     loc_15_225B
        cmp    al, 0x30
        je     loc_15_225B
        cmp    al, 0x20
        jb     loc_15_2237
        cmp    al, 0x24
        jbe    loc_15_225B
loc_15_2237:
        test   byte ptr [edi], 2
        jne    loc_15_225B
        call   sub_15_2769
        mov    eax, 0x1388
        mov    esi, 0x2845                   ; -> loc_15_2845
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        mov    dword ptr [edi + 0x5c8], 0xffffffff
loc_15_225B:
        stc    
        ret    
loc_15_225D:
        mov    ecx, eax
        cmp    word ptr [ebp + 0x18], 1
        jne    loc_15_225B
        call   sub_15_2070
        jne    loc_15_225B
        mov    edi, esi
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        movzx  eax, word ptr [0x44c]
        movzx  edx, dh
        mul    edx
        pop    edx
        add    esi, eax
        mov    ch, dl
        movzx  edx, byte ptr [0x462]
        movzx  edx, word ptr [edx*2 + 0x450]
        push   ebx
        mov    eax, 0xa0
        mul    dh
        add    eax, esi
        movzx  ebx, dl
        shl    ebx, 1
        add    eax, ebx
        mov    word ptr [eax], cx
        pop    ebx
        inc    dl
        call   sub_15_2136
        jmp    loc_15_2368
loc_15_22B4:
        mov    ecx, eax
        cmp    cl, 7
        je     loc_15_225B
        cmp    cl, 0xd
        jne    loc_15_22F3
        test   byte ptr [edi], 1
        je     loc_15_22F3
        dec    word ptr [edi + 0x5d0]
        jne    loc_15_22F3
        pushal 
        mov    eax, dword ptr [0xfdc]        ; -> obj1:0x0FDC
        mov    word ptr [edi + 0x5d0], ax
        mov    word ptr [edi + 0x5cc], 1
        call   sub_15_4780
        or     byte ptr [edi + 3], 0x20
        call   sub_15_46DC
        popal  
loc_15_22F3:
        call   sub_15_2070
        jne    loc_15_225B
        mov    edi, esi
        movzx  eax, word ptr [0x44e]
        add    esi, eax
        movzx  edx, byte ptr [0x462]
        movzx  edx, word ptr [edx*2 + 0x450]
        cmp    cl, 0xd
        je     loc_15_236A
        cmp    cl, 8
        je     loc_15_236E
        cmp    cl, 0xa
        je     loc_15_234C
        push   ebx
        mov    eax, 0xa0
        mul    dh
        add    eax, esi
        movzx  ebx, dl
        shl    ebx, 1
        add    eax, ebx
        mov    byte ptr [eax], cl
        pop    ebx
        inc    dl
        call   sub_15_2136
        cmp    dl, byte ptr [0x44a]
        jne    loc_15_235C
        xor    dl, dl
loc_15_234C:
        cmp    dh, byte ptr [0x484]
        je     loc_15_2376
        ja     loc_15_225B
        inc    dh
loc_15_235C:
        movzx  ecx, byte ptr [0x462]
        call   sub_15_2702
loc_15_2368:
        clc    
        ret    
loc_15_236A:
        xor    dl, dl
        jmp    loc_15_235C
loc_15_236E:
        or     dl, dl
        je     loc_15_235C
        dec    dl
        jmp    loc_15_235C
loc_15_2376:
        movzx  ecx, byte ptr [0x462]
        mov    eax, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    eax, ebx
        mov    dword ptr [eax + 0x5c8], 0xffffffff
        call   sub_15_2702
        mov    edi, esi
        mov    eax, 0xa0
        lea    esi, [eax + edi]
        mul    dh
        or     eax, eax
        je     loc_15_23A7
        mov    ecx, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_15_23A7:
        mov    eax, dword ptr [edi]
        mov    al, 0x20
        mov    ecx, 0x50
        cld    
        rep stosw word ptr es:[edi], ax
        jmp    loc_15_2368
loc_15_23B6:
        call   sub_15_2070
        jne    loc_15_225B
        movzx  ecx, byte ptr [ebp + 0x11]
        mov    ax, word ptr [0x460]
        mov    word ptr [ebp + 0x18], ax
        mov    ax, word ptr [ecx*2 + 0x450]
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_15_2368
loc_15_23DD:
        movzx  ecx, byte ptr [ebp + 0x11]
        cmp    cl, byte ptr [0x462]
        jne    loc_15_225B
        call   sub_15_2070
        jne    loc_15_225B
        mov    edi, esi
        movzx  eax, word ptr [0x44e]
        add    esi, eax
        mov    edx, dword ptr [ebp + 0x14]
        call   sub_15_2702
        or     dl, dl
        jne    loc_15_2476
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 1
        je     loc_15_2476
        xchg   word ptr [edi + 0x5d2], dx
        cmp    dx, word ptr [edi + 0x5d2]
        jb     loc_15_2476
        je     loc_15_244A
        cmp    dh, byte ptr [0x484]
        jne    loc_15_2476
        dec    dh
        cmp    dx, word ptr [edi + 0x5d2]
        jne    loc_15_2476
        inc    word ptr [edi + 0x5d0]
        jmp    loc_15_2476
loc_15_244A:
        dec    word ptr [edi + 0x5d0]
        jne    loc_15_2476
        mov    eax, dword ptr [0xfdc]        ; -> obj1:0x0FDC
        mov    word ptr [edi + 0x5d0], ax
        mov    word ptr [edi + 0x5cc], 1
        call   sub_15_4780
        or     byte ptr [edi + 3], 0x20
        call   sub_15_46DC
loc_15_2476:
        jmp    loc_15_2368
loc_15_247B:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_225B
        mov    dword ptr [edi + 0x5c8], 0xffffffff
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        mov    al, byte ptr [ebp + 0x19]
        test   byte ptr [0x487], 8
        jne    loc_15_2593
        cmp    al, byte ptr [0x484]
        jbe    loc_15_24BA
        mov    al, byte ptr [0x484]
        mov    byte ptr [ebp + 0x19], al
loc_15_24BA:
        mov    al, byte ptr [ebp + 0x15]
        cmp    al, byte ptr [0x484]
        jbe    loc_15_24CD
        mov    al, byte ptr [0x484]
        mov    byte ptr [ebp + 0x15], al
loc_15_24CD:
        sub    al, byte ptr [ebp + 0x19]
        jb     loc_15_2585
        inc    al
        cmp    byte ptr [ebp + 0x1c], al
        jbe    loc_15_24E0
        mov    byte ptr [ebp + 0x1c], al
loc_15_24E0:
        mov    esi, dword ptr [edi + 0x547]
        call   sub_15_168F
        add    esi, eax
        mov    edx, eax
        call   sub_15_16C3
        jb     loc_15_2593
        mov    eax, ecx
        mul    byte ptr [ebp + 0x15]
        xchg   edx, eax
        add    edx, eax
        add    edx, ecx
        add    edx, ecx
        dec    edx
        shr    eax, 0xc
        shr    edx, 0xc
loc_15_250D:
        call   sub_15_456A
        inc    eax
        cmp    eax, edx
        jb     loc_15_250D
        mov    eax, ecx
        dec    cl
        cmp    byte ptr [ebp + 0x18], cl
        jbe    loc_15_2523
        mov    byte ptr [ebp + 0x18], cl
loc_15_2523:
        cmp    byte ptr [ebp + 0x14], cl
        jbe    loc_15_252B
        mov    byte ptr [ebp + 0x14], cl
loc_15_252B:
        mov    cl, byte ptr [ebp + 0x18]
        cmp    cl, byte ptr [ebp + 0x14]
        jg     loc_15_2585
        shl    eax, 1
        push   eax
        mul    byte ptr [ebp + 0x19]
        add    esi, eax
        pop    eax
        push   edi
        movzx  edx, byte ptr [ebp + 0x14]
        movzx  edi, byte ptr [ebp + 0x18]
        add    esi, edi
        add    esi, edi
        sub    edx, edi
        inc    edx
        mov    edi, esi
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_15_2568
        cmp    byte ptr [ebp + 0x1d], 6
        je     loc_15_2561
        call   sub_15_26B2
        jmp    loc_15_257B
loc_15_2561:
        call   sub_15_2671
        jmp    loc_15_257B
loc_15_2568:
        xchg   edx, eax
        sub    edx, eax
        sub    edx, eax
        movzx  ecx, byte ptr [ebp + 0x15]
        sub    cl, byte ptr [ebp + 0x19]
        inc    cl
        call   sub_15_269E
loc_15_257B:
        pop    edi
        or     byte ptr [edi + 3], 0x20
        call   sub_15_46DC
loc_15_2585:
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x18]
        pop    dword ptr [ebp + 0x1c]
        jmp    loc_15_2368
loc_15_2593:
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x18]
        pop    dword ptr [ebp + 0x1c]
        jmp    loc_15_225B
loc_15_25A1:
        mov    edx, dword ptr [ebp + 0x10]
        shl    edx, 0x10
        mov    dx, ax
        push   edx
        clc    
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xa4], 0
        je     loc_15_25C5
        popfd  
        call   dword ptr [edi + 0xa4]
        jmp    loc_15_25C6
loc_15_25C5:
        popfd  
loc_15_25C6:
        pop    edi
        pop    edx
        jae    loc_15_25D4
        call   sub_15_2611
        jmp    loc_15_2368
loc_15_25D4:
        cmp    al, 3
        je     loc_15_25FC
        mov    dx, word ptr [ebp + 0x10]
        shl    edx, 0x10
        mov    dx, word ptr [ebp + 0x1c]
        xor    eax, eax
        mov    esi, 0x2611                   ; -> sub_15_2611
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        mov    byte ptr [edi + 0x638], 0xff
        jmp    loc_15_225B
loc_15_25FC:
        mov    eax, dword ptr [edi + 0x620]
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_15_2368
sub_15_2611:
        push   edi
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xbc], 0
        je     loc_15_262F
        call   dword ptr [edi + 0xbc]
loc_15_262F:
        pop    edi
        cmp    dx, 0x4f02
        jne    loc_15_2668
        cmp    word ptr [ebp + 0x1c], 0x4f
        jne    loc_15_2668
        shr    edx, 0x10
        and    dl, 0x7f
        and    dh, 0x7f
        mov    dword ptr [edi + 0x620], edx
        and    byte ptr [edi + 1], 0xdf
        cmp    edx, 0x100
        jb     loc_15_2668
        or     byte ptr [edi + 1], 0x20
        call   sub_15_09FE
        mov    dword ptr [edi + 0x61c], ecx
loc_15_2668:
        mov    byte ptr [edi + 0x638], 0
        pop    edi
        ret    
sub_15_2671:
        push   edx
        mov    edx, eax
        mul    byte ptr [ebp + 0x1c]
        add    esi, eax
        movzx  ecx, byte ptr [ebp + 0x15]
        sub    cl, byte ptr [ebp + 0x19]
        inc    cl
        sub    cl, byte ptr [ebp + 0x1c]
        pop    eax
        sub    edx, eax
        sub    edx, eax
        jecxz  loc_15_269A
        cld    
loc_15_268D:
        push   ecx
        mov    ecx, eax
        rep movsw word ptr es:[edi], word ptr [esi]
        add    esi, edx
        add    edi, edx
        pop    ecx
        loop   loc_15_268D
loc_15_269A:
        movzx  ecx, byte ptr [ebp + 0x1c]
sub_15_269E:
        mov    esi, eax
        mov    ah, byte ptr [ebp + 0x11]
        mov    al, 0x20
        cld    
loc_15_26A6:
        push   ecx
        mov    ecx, esi
        rep stosw word ptr es:[edi], ax
        add    edi, edx
        pop    ecx
        loop   loc_15_26A6
        ret    
sub_15_26B2:
        push   ebx
        mov    ebx, eax
        add    esi, edx
        add    esi, edx
        mov    edi, esi
        movzx  ecx, byte ptr [ebp + 0x15]
        sub    cl, byte ptr [ebp + 0x19]
        mul    cl
        add    edi, eax
        mov    eax, ebx
        sub    ebx, edx
        sub    ebx, edx
        sub    edi, 2
        sub    cl, byte ptr [ebp + 0x1c]
        jl     loc_15_26EB
        mul    cl
        add    esi, eax
        sub    esi, 2
        inc    cl
        std    
loc_15_26DE:
        push   ecx
        mov    ecx, edx
        rep movsw word ptr es:[edi], word ptr [esi]
        sub    esi, ebx
        sub    edi, ebx
        pop    ecx
        loop   loc_15_26DE
loc_15_26EB:
        mov    cl, byte ptr [ebp + 0x1c]
        mov    ah, byte ptr [ebp + 0x11]
        mov    al, 0x20
        std    
loc_15_26F4:
        push   ecx
        mov    ecx, edx
        rep stosw word ptr es:[edi], ax
        sub    edi, ebx
        pop    ecx
        loop   loc_15_26F4
        cld    
        pop    ebx
        ret    
sub_15_2702:
        mov    word ptr [ecx*2 + 0x450], dx
        mov    eax, 0xa0
        mul    dh
        add    eax, esi
        movzx  ecx, dl
        shl    ecx, 1
        add    eax, ecx
        sub    eax, edi
        shr    eax, 1
        xchg   al, ah
        mov    ecx, ebx
        add    ecx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        mov    word ptr [ecx + 0x1a], ax
        sub    ecx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        cmp    ecx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_2752
        push   edx
        push   eax
        mov    dx, word ptr [0x463]
        mov    al, 0xf
        out    dx, al
        inc    edx
        mov    al, ah
        out    dx, al
        dec    edx
        mov    al, 0xe
        out    dx, al
        inc    edx
        pop    eax
        out    dx, al
        pop    edx
loc_15_2752:
        ret    
loc_15_2753:
        inc    word ptr [ebp + 0x24]
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    byte ptr [ebp + 0x1d], 5
        je     loc_15_282C
sub_15_2769:
        test   byte ptr [0x6c4], 4           ; -> obj1:0x06C4
        je     loc_15_282B
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_282B
        movzx  eax, word ptr [ebp + 0x1c]
        or     ah, ah
        jne    loc_15_27C3
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc0], 0
        je     loc_15_27A0
        call   dword ptr [edi + 0xc0]
loc_15_27A0:
        pop    edi
        and    al, 0x7f
        mov    dword ptr [edi + 0x620], eax
        and    byte ptr [edi + 1], 0xdf
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_15_27C3
        push   0xff
        push   1
        call   sub_14_0C2B
        pop    ecx
        pop    ecx
loc_15_27C3:
        cmp    byte ptr [ebp + 0x1d], 0x11
        jne    loc_15_27CE
        or     byte ptr [edi], 0x20
        jmp    loc_15_27D5
loc_15_27CE:
        mov    byte ptr [edi + 0x4a6], 1
loc_15_27D5:
        call   sub_15_3A50
        bts    dword ptr [edi], 1
        jb     loc_15_282B
        call   sub_15_4BBC
        mov    eax, 0x1000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        test   byte ptr [edi], 1
        je     loc_15_27FD
        call   sub_15_4780
        or     byte ptr [edi], 4
loc_15_27FD:
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ah, 0x11
        je     loc_15_281B
        or     al, al
        js     loc_15_281B
        call   sub_15_4538
        mov    dword ptr [edi + 0x609], 0
        jmp    loc_15_282B
loc_15_281B:
        or     byte ptr [edi], 8
        call   sub_15_29F4
        add    eax, ecx
        mov    dword ptr [edi + 0x609], eax
loc_15_282B:
        ret    
loc_15_282C:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_282B
        bts    dword ptr [edi], 4
        jb     loc_15_282B
        call   sub_15_4BBC
        jmp    loc_15_282B
loc_15_2841:
        inc    word ptr [ebp + 0x24]
loc_15_2845:
        test   byte ptr [0x6c4], 4           ; -> obj1:0x06C4
        je     loc_15_288E
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_288E
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        btr    dword ptr [edi], 4
        jb     loc_15_2886
        call   sub_15_4740
        btr    dword ptr [edi], 1
        jae    loc_15_288E
        mov    eax, 0xfffff000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        call   sub_15_3864
loc_15_2886:
        call   sub_15_4BBC
        and    byte ptr [edi], 0xd7
loc_15_288E:
        ret    
loc_15_288F:
        bts    dword ptr [0x6c4], 9          ; -> obj1:0x06C4
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0x1002
        je     loc_15_28B2
loc_15_28A0:
        mov    ecx, dword ptr [0xeb4]        ; -> obj1:0x0EB4
        mov    edx, dword ptr [0xeb8]        ; -> obj1:0x0EB8
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_15_28B2:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi + 3], 1
        je     loc_15_28A0
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
        add    byte ptr [eax], al
sub_15_28C8:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x58], 0
        je     loc_15_28D7
        call   dword ptr [edi + 0x58]
loc_15_28D7:
        pop    edi
        mov    esi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        cmp    dword ptr [0xed2], 0x10       ; -> loc_1_0ED2
        ja     loc_15_28EF
        add    esi, 0x9fff
        jmp    loc_15_28F5
loc_15_28EF:
        add    esi, 0xfffc
loc_15_28F5:
        ret    
sub_15_28F6:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x5c], 0
        je     loc_15_2905
        call   dword ptr [edi + 0x5c]
loc_15_2905:
        pop    edi
        ret    
sub_15_2907:
        push   eax
        push   ecx
        test   byte ptr [edi + 0x23], 1
        je     loc_15_2922
        push   eax
        mov    al, byte ptr [edi + 0x7c]
        and    al, 3
        cmp    al, 3
        pop    eax
        jne    loc_15_2922
        and    eax, 0xf
        and    ecx, 0xf
        jmp    loc_15_2928
loc_15_2922:
        test   byte ptr [edi + 0x6a], 8
        je     loc_15_2928
loc_15_2928:
        cmp    eax, ecx
        pop    ecx
        pop    eax
        ret    
        db  0C1h, 0E8h, 002h, 0C1h, 0E9h, 002h, 0EBh, 0F3h
sub_15_2935:
        mov    esi, eax
        test   byte ptr [edi + 0x23], 1
        je     loc_15_294D
        push   eax
        mov    al, byte ptr [edi + 0x7c]
        and    al, 3
        cmp    al, 3
        pop    eax
        jne    loc_15_294D
        and    esi, 0xf
        jmp    loc_15_2953
loc_15_294D:
        test   byte ptr [edi + 0x6a], 8
        je     loc_15_2953
loc_15_2953:
        shl    esi, 3
        add    esi, dword ptr [0xed6]        ; -> loc_1_0ED6
        ret    
        db  0C1h, 0EEh, 002h, 0EBh, 0F1h
sub_15_2962:
        cmp    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        clc    
        jne    loc_15_2996
        push   eax
        push   ecx
        push   esi
        mov    esi, dword ptr [0xed6]        ; -> loc_1_0ED6
        mov    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
loc_15_297A:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_15_2993
        cmp    ebx, dword ptr [eax + 8]
        clc    
        jne    loc_15_2993
        cmp    dword ptr [esi], 0
        clc    
        jne    loc_15_2993
        add    esi, 8
        loop   loc_15_297A
        stc    
loc_15_2993:
        pop    esi
        pop    ecx
        pop    eax
loc_15_2996:
        ret    
sub_15_2997:
        push   edx
        mov    ecx, eax
        movzx  eax, word ptr [ecx]
        cmp    eax, dword ptr [0xecc]        ; -> obj1:0x0ECC
        jb     loc_15_29B7
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 8], 0
        je     loc_15_29B4
        call   dword ptr [edi + 8]
loc_15_29B4:
        pop    edi
        jmp    loc_15_29C7
loc_15_29B7:
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_29C6
        call   dword ptr [edi + 0xc]
loc_15_29C6:
        pop    edi
loc_15_29C7:
        and    eax, 0xf
        mov    edx, eax
        call   sub_15_1C3B
        movzx  eax, al
        mov    ah, al
        add    al, dl
        xor    ecx, ecx
        pop    edx
        ret    
sub_15_29DC:
        push   ecx
        push   esi
        call   sub_15_2935
        movzx  ecx, word ptr [ecx]
        shl    ecx, 3
        add    ecx, dword ptr [0xed6]        ; -> loc_1_0ED6
        cmp    ecx, esi
        pop    esi
        pop    ecx
        ret    
sub_15_29F4:
        mov    ecx, edi
        sub    ecx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        cmp    ecx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_2A17
        mov    ecx, dword ptr [0xecc]        ; -> obj1:0x0ECC
        inc    ecx
        jecxz  loc_15_2A17
        dec    ecx
        movzx  eax, word ptr [0xed0]         ; -> obj1:0x0ED0
        stc    
        ret    
loc_15_2A17:
        call   sub_1_028C
        jae    loc_15_2A53
        mov    ecx, dword ptr [edi + 0x61c]
        cmp    ecx, -1
        je     loc_15_2A2D
        or     ecx, ecx
        jne    loc_15_2A4A
loc_15_2A2D:
        push   ebx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jae    loc_15_2A3E
        mov    ebx, edi
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
loc_15_2A3E:
        call   sub_15_09FE
        mov    dword ptr [edi + 0x61c], ecx
        pop    ebx
loc_15_2A4A:
        mov    ecx, 0x10
        xor    eax, eax
        stc    
        ret    
loc_15_2A53:
        and    byte ptr [edi + 1], 0xdf
        push   edx
        call   sub_15_16C3
        jae    loc_15_2A7F
        imul   ecx, eax
        test   byte ptr [edi + 0x6a], 8
        jne    loc_15_2A87
        shr    ecx, 2
        cmp    byte ptr [edi + 0x5b3], 0x58
        je     loc_15_2A87
        test   byte ptr [edi + 0x7b], 0x20
        jne    loc_15_2A87
        shr    ecx, 1
        clc    
        jmp    loc_15_2A87
loc_15_2A7F:
        imul   eax, ecx
        shl    eax, 1
        mov    ecx, eax
        stc    
loc_15_2A87:
        cmc    
        pushfd 
        call   sub_15_168F
        push   eax
        and    eax, 0xfff
        add    ecx, eax
        add    ecx, 0xfff
        shr    ecx, 0xc
        pop    eax
        shr    eax, 0xc
        cmp    dword ptr [edi + 0x609], 0
        je     loc_15_2AC6
        cmp    edi, 0x7bc                    ; -> loc_1_07BC
        je     loc_15_2AC6
        add    ecx, eax
        xor    eax, eax
        cmp    ecx, dword ptr [edi + 0x609]
        jae    loc_15_2AC6
        mov    ecx, dword ptr [edi + 0x609]
loc_15_2AC6:
        popfd  
        pop    edx
        ret    
sub_15_2AC9:
        push   ecx
        push   edx
        mov    edx, eax
        call   sub_15_29F4
        xchg   edx, eax
        cmp    eax, edx
        jb     loc_15_2AE1
        add    edx, ecx
        cmp    eax, edx
        jae    loc_15_2AE1
        xor    dl, dl
        jmp    loc_15_2AE4
loc_15_2AE1:
        or     dl, 1
loc_15_2AE4:
        pop    edx
        pop    ecx
        ret    
sub_15_2AE7:
        push   ecx
        mov    ecx, eax
        call   sub_15_1C3B
        push   eax
        movzx  eax, al
        sub    ecx, eax
        pop    eax
        jb     loc_15_2AFF
        cmp    cl, ah
        cmc    
        jb     loc_15_2AFF
        xchg   ecx, eax
        clc    
loc_15_2AFF:
        pop    ecx
        ret    
sub_15_2B01:
        push   edx
        mov    edx, eax
        call   sub_15_1C3B
        movzx  eax, al
        add    eax, edx
        pop    edx
        ret    
sub_15_2B10:
        pushal 
        call   sub_15_2935
        mov    edx, dword ptr [esi + 4]
        or     edx, edx
        jne    loc_15_2B2B
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_2BEE
        jmp    loc_15_2B55
loc_15_2B2B:
        cmp    ebx, dword ptr [edx + 8]
        jne    loc_15_2B49
        movzx  eax, word ptr [edx + 4]
        mov    edx, esi
        call   sub_15_2935
        mov    eax, dword ptr [esp + 0x1c]
        cmp    esi, edx
        je     loc_15_2BEE
        mov    esi, edx
loc_15_2B49:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_2BE0
loc_15_2B55:
        mov    esi, dword ptr [0xed6]        ; -> loc_1_0ED6
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_2B89
        push   eax
        push   edx
        mov    eax, dword ptr [0xecc]        ; -> obj1:0x0ECC
        sub    ecx, eax
        jg     loc_15_2B7E
        pop    edx
        pop    eax
        jmp    loc_15_2BDB
loc_15_2B7E:
        mov    edx, 8
        mul    edx
        add    esi, eax
        pop    edx
        pop    eax
loc_15_2B89:
        xor    ebx, ebx
        xor    edi, edi
loc_15_2B8D:
        cmp    dword ptr [esi + 4], 0
        je     loc_15_2BCE
        cmp    dword ptr [esi], 0
        je     loc_15_2BA4
        mov    ebp, eax
        sub    ebp, dword ptr [esi]
        cmp    ebp, ebx
        jb     loc_15_2BA4
        mov    ebx, ebp
        mov    edi, esi
loc_15_2BA4:
        add    esi, 8
        loop   loc_15_2B8D
        mov    esi, edi
        mov    edi, dword ptr [esp]
        mov    ebx, dword ptr [esp + 0x10]
        or     esi, esi
        je     loc_15_2BDB
        mov    eax, dword ptr [esi + 4]
        call   sub_15_2D96
        mov    eax, dword ptr [esp + 0x1c]
        jae    loc_15_2BEE
        mov    eax, dword ptr [esi + 4]
        call   sub_15_2CA7
        jmp    loc_15_2B55
loc_15_2BCE:
        mov    edi, dword ptr [esp]
        mov    ebx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x1c]
        jmp    loc_15_2BEE
loc_15_2BDB:
        xor    eax, eax
        stc    
        jmp    loc_15_2C4E
loc_15_2BE0:
        mov    ecx, eax
        mov    eax, dword ptr [esi + 4]
        call   sub_15_2D96
        jb     loc_15_2BDB
        mov    eax, ecx
loc_15_2BEE:
        mov    edx, eax
        mov    ecx, esi
        mov    esi, dword ptr [0xeda]        ; -> loc_1_0EDA
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_15_2BDB
        mov    word ptr [eax + 4], dx
        mov    word ptr [eax + 6], 0
        mov    dword ptr [eax + 8], ebx
        mov    byte ptr [eax + 3], 0
        cmp    dword ptr [ecx + 4], 0
        je     loc_15_2C24
        push   ecx
        mov    ecx, dword ptr [ecx + 4]
        VMMCall  VMM.List_Insert                 ; cd20a0000100
        pop    ecx
        jmp    loc_15_2C2D
loc_15_2C24:
        mov    dword ptr [ecx + 4], eax
        VMMCall  VMM.List_Attach                 ; cd209e000100
loc_15_2C2D:
        sub    ecx, dword ptr [0xed6]        ; -> loc_1_0ED6
        shr    ecx, 3
        mov    word ptr [eax], cx
        mov    esi, eax
        call   sub_15_2CA7
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xc], ecx
        clc    
        inc    byte ptr [edi + 0x60e]
loc_15_2C4E:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_15_2C54:
        pushal 
        mov    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    esi, dword ptr [0xed6]        ; -> loc_1_0ED6
        mov    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
loc_15_2C6F:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_15_2CA0
        cmp    ebx, dword ptr [eax + 8]
        je     loc_15_2C8A
        cmp    dword ptr [esi], 0
        jne    loc_15_2CA0
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi], eax
        jmp    loc_15_2CA0
loc_15_2C8A:
        cmp    dword ptr [esi], 0
        je     loc_15_2CA0
        movzx  eax, word ptr [eax + 4]
        call   sub_15_2AC9
        jne    loc_15_2CA0
        mov    dword ptr [esi], 0
loc_15_2CA0:
        add    esi, 8
        loop   loc_15_2C6F
        popal  
        ret    
sub_15_2CA7:
        push   eax
        push   ecx
        push   esi
        push   0
        mov    eax, dword ptr [esi + 8]
        cmp    eax, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_2CF5
        movzx  eax, word ptr [esi]
        mov    ecx, eax
        shl    eax, 3
        add    eax, dword ptr [0xed6]        ; -> loc_1_0ED6
        mov    esi, dword ptr [eax + 4]
loc_15_2CC8:
        movzx  eax, word ptr [esi + 4]
        call   sub_15_2AC9
        je     loc_15_2CFE
        call   sub_15_3852
        jb     loc_15_2CFE
        mov    eax, esi
        mov    esi, dword ptr [0xeda]        ; -> loc_1_0EDA
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, eax
        je     loc_15_2CF1
        cmp    cx, word ptr [esi]
        je     loc_15_2CC8
loc_15_2CF1:
        mov    esi, dword ptr [esp + 4]
loc_15_2CF5:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esp], eax
loc_15_2CFE:
        movzx  eax, word ptr [esi]
        shl    eax, 3
        add    eax, dword ptr [0xed6]        ; -> loc_1_0ED6
        pop    dword ptr [eax]
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_15_2D10:
        pushal 
        mov    edx, eax
        movzx  eax, word ptr [eax + 4]
        push   esi
        call   sub_15_2935
        sub    esi, dword ptr [0xed6]        ; -> loc_1_0ED6
        shr    esi, 3
        cmp    si, word ptr [eax]
        pop    esi
        je     loc_15_2D4A
        or     esi, esi
        jne    loc_15_2D33
        mov    esi, dword ptr [edx + 0xc]
loc_15_2D33:
        mov    eax, ecx
        call   sub_15_2B10
        jb     loc_15_2D48
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, edx
        call   sub_15_2D96
        clc    
loc_15_2D48:
        popal  
        ret    
loc_15_2D4A:
        or     esi, esi
        je     loc_15_2D48
        mov    dword ptr [edx + 0xc], esi
        clc    
        jmp    loc_15_2D48
sub_15_2D54:
        push   ecx
        push   edx
        push   esi
        mov    ecx, eax
        cmp    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
        jb     loc_15_2D61
loc_15_2D61:
        call   sub_15_2935
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_15_2D91
        mov    dx, word ptr [eax]
loc_15_2D70:
        mov    ebx, eax
        cmp    cx, word ptr [eax + 4]
        je     loc_15_2D8B
        mov    esi, dword ptr [0xeda]        ; -> loc_1_0EDA
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_15_2D8B
        cmp    dx, word ptr [eax]
        je     loc_15_2D70
loc_15_2D8B:
        mov    eax, ebx
        mov    ebx, dword ptr [eax + 8]
        stc    
loc_15_2D91:
        cmc    
        pop    esi
        pop    edx
        pop    ecx
        ret    
sub_15_2D96:
        pushal 
        movzx  ebp, word ptr [eax]
        shl    ebp, 3
        add    ebp, dword ptr [0xed6]        ; -> loc_1_0ED6
        mov    eax, dword ptr [ebp + 4]
        mov    ebx, dword ptr [eax + 8]
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
loc_15_2DB1:
        mov    dword ptr [ebp + 4], eax
        xor    ecx, ecx
        push   eax
        push   ebp
        call   dword ptr [eax + 0xc]
        pop    ebp
        pop    eax
        jb     loc_15_2DF4
        dec    byte ptr [edi + 0x60e]
        xor    edx, edx
        mov    dword ptr [ebp + 4], edx
        mov    edx, eax
        mov    esi, dword ptr [0xeda]        ; -> loc_1_0EDA
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        xchg   edx, eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        movzx  ecx, word ptr [eax]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        or     edx, edx
        je     loc_15_2DF3
        mov    eax, edx
        cmp    cx, word ptr [eax]
        je     loc_15_2DB1
loc_15_2DF3:
        clc    
loc_15_2DF4:
        popal  
        ret    
sub_15_2DF6:
        push   eax
        push   edx
        mov    ah, byte ptr [edi + 0x7c]
        and    ah, 0xc
        jmp    loc_15_2E04
sub_15_2E00:
        push   eax
        push   edx
        mov    ah, 4
loc_15_2E04:
        mov    edx, 0x3ce
        in     al, dx
        push   eax
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        or     al, ah
        out    dx, al
        pop    eax
        dec    edx
        out    dx, al
        pop    edx
        pop    eax
        ret    
sub_15_2E1B:
        or     ebx, ebx
        je     loc_15_2E24
        call   sub_15_18A9
loc_15_2E24:
        ret    
sub_15_2E25:
        btr    dword ptr [edi], 0xe
        jae    loc_15_2E3D
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_2E38
        call   sub_15_1193
loc_15_2E38:
        call   sub_15_18A9
loc_15_2E3D:
        ret    
sub_15_2E3E:
        pushal 
        mov    esi, eax
        call   sub_15_2CA7
        mov    esi, eax
        call   sub_15_2997
        xchg   ecx, eax
        xor    edx, edx
        or     ebx, ebx
        je     loc_15_2E89
        cmp    ch, 0xb0
        jb     loc_15_2E76
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_2E76
        sub    cl, ch
        add    cl, 0xa0
        bts    dword ptr [edi], 0xe
        jb     loc_15_2E89
        or     dl, 1
        call   sub_15_2E00
        jmp    loc_15_2E84
loc_15_2E76:
        btr    dword ptr [edi], 0xe
        jae    loc_15_2E89
        or     dl, 1
        call   sub_15_2DF6
loc_15_2E84:
        call   sub_15_2E1B
loc_15_2E89:
        movzx  ecx, cl
        mov    dword ptr [esp + 0x18], ecx
        bt     edx, 0
        popal  
        ret    
        db  050h, 051h, 0E8h, 0FAh, 0FAh, 0FFh, 0FFh, 03Bh, 01Dh
        dd  loc_1_07A8               ; fixup
        db  075h, 005h, 080h, 0F9h, 0C0h, 072h, 005h, 080h, 0F9h, 0B8h, 0B1h, 000h, 0F5h, 080h, 0D1h, 000h
        db  00Fh, 0BAh, 027h, 00Eh, 080h, 0D9h, 000h, 0D0h, 0E9h, 059h, 058h, 0C3h
sub_15_2EBF:
        push   esi
        mov    esi, eax
        call   sub_15_2CA7
        pop    esi
        ret    
        db  000h, 000h, 000h
VDD_Get_VM_Info:
        mov    edi, dword ptr [0x7a8]        ; -> loc_1_07A8
        mov    esi, dword ptr [0x7ac]        ; -> loc_1_07AC
        ret    
VDD_Hide_Cursor:
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        and    byte ptr [edi + 2], 0xf7
        or     eax, eax
        je     loc_15_2EF7
        or     byte ptr [edi + 2], 8
        mov    word ptr [edi + 0x4bd], 0xffff
loc_15_2EF7:
        pop    edi
        ret    
VDD_Query_Access:
        push   eax
        push   edi
        test   byte ptr [0x7a0], 7           ; -> loc_1_07A0
        jne    loc_15_2F38
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_2F2A
        mov    eax, dword ptr [0xed2]        ; -> loc_1_0ED2
        cmp    eax, dword ptr [0xecc]        ; -> obj1:0x0ECC
        je     loc_15_2F38
        cmp    byte ptr [edi + 0x5b3], 0x58
        je     loc_15_2F38
loc_15_2F2A:
        test   byte ptr [edi], 0x80
        jne    loc_15_2F38
        test   byte ptr [edi + 2], 0x20
        jne    loc_15_2F38
        clc    
        jmp    loc_15_2F39
loc_15_2F38:
        stc    
loc_15_2F39:
        pop    edi
        pop    eax
        ret    
VDD_Set_VMType:
        pushal 
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        or     byte ptr [edi], 1
        mov    dword ptr [edi + 0x5c8], 0xffffffff
        or     eax, eax
        jne    loc_15_2F70
        and    byte ptr [edi], 0xfe
        call   sub_15_01B1
        call   sub_15_4780
        test   byte ptr [edi + 2], 2
        je     loc_15_2F6E
        call   sub_1_0415
loc_15_2F6E:
        popal  
        ret    
loc_15_2F70:
        btr    dword ptr [edi], 0x16
        jae    loc_15_2F7B
        call   sub_15_394E
loc_15_2F7B:
        mov    dword ptr [edi + 0x5f0], 0x28
        or     byte ptr [edi + 1], 2
        push   edi
        call   sub_15_46DC
        pop    edi
        test   dword ptr [edi], 0x200080
        je     loc_15_2FAC
        test   byte ptr [edi + 1], 0x20
        jne    loc_15_2FAC
        cmp    byte ptr [edi + 0x5b3], 0x56
        je     loc_15_2FAC
        call   sub_9_051B
loc_15_2FAC:
        popal  
        ret    
VDD_Check_Update_Soon:
        pushal 
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    word ptr [edi + 0x5cc], 1
        je     loc_15_2FF3
        test   byte ptr [edi], 1
        je     loc_15_2FF3
        call   sub_15_4780
        mov    eax, 5
        mov    edx, edi
        mov    esi, 0x46a4                   ; -> loc_15_46A4
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        mov    dword ptr [edi + 0x5e4], esi
        inc    word ptr [edi + 0x5cc]
        mov    word ptr [edi + 0x5ce], 4
loc_15_2FF3:
        popal  
        ret    
VDD_Get_ModTime:
        pushal 
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 4
        je     loc_15_300B
loc_15_3003:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        jmp    loc_15_301D
loc_15_300B:
        call   sub_15_1C57
        jb     loc_15_3003
        call   sub_15_44DC
        mov    eax, dword ptr [edi + 0x5e8]
loc_15_301D:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_15_3023:
        pushal 
        cmp    byte ptr [0xfb1], 0           ; -> obj1:0x0FB1
        jne    loc_15_3076
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_3076
        mov    ecx, 8
        mov    edi, 0xf0c                    ; -> loc_1_0F0C
loc_15_303F:
        push   ecx
        mov    edx, dword ptr [edi]
        or     edx, edx
        je     loc_15_3065
        mov    eax, dword ptr [edi + 0xc]
        mov    esi, dword ptr [edi + 0x10]
        mov    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        push   0
        push   eax
        push   esi
        push   ebx
        push   edx
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        or     eax, eax
        je     loc_15_3075
loc_15_3065:
        pop    ecx
        add    edi, 0x14
        dec    ecx
        jne    loc_15_303F
        mov    byte ptr [0xfb1], 0xff        ; -> obj1:0x0FB1
        jmp    loc_15_3076
loc_15_3075:
        pop    ecx
loc_15_3076:
        popal  
        ret    
loc_15_3078:
        pushal 
        pushfd 
        sti    
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_3134
        cmp    byte ptr [0xfb1], 0           ; -> obj1:0x0FB1
        je     loc_15_3134
        mov    ecx, 8
        mov    edi, 0xf0c                    ; -> loc_1_0F0C
loc_15_309E:
        mov    edx, dword ptr [edi]
        or     edx, edx
        je     loc_15_30C4
        movzx  eax, word ptr [ebp + 0x38]
        cmp    eax, edx
        je     loc_15_30CC
        movzx  eax, word ptr [ebp + 0x40]
        cmp    eax, edx
        je     loc_15_30CC
        movzx  eax, word ptr [ebp + 0x44]
        cmp    eax, edx
        je     loc_15_30CC
        movzx  eax, word ptr [ebp + 0x3c]
        cmp    eax, edx
        je     loc_15_30CC
loc_15_30C4:
        add    edi, 0x14
        dec    ecx
        jne    loc_15_309E
        jmp    loc_15_3134
loc_15_30CC:
        push   fs
        mov    fs, word ptr [ebp + 0x28]
        mov    edx, dword ptr [ebp + 0x24]
        mov    al, byte ptr fs:[edx]
        pop    fs
        cmp    al, 0x6c
        jb     loc_15_30F2
        cmp    al, 0x6f
        jbe    loc_15_3134
        cmp    al, 0xec
        jb     loc_15_30F2
        cmp    al, 0xef
        jbe    loc_15_3134
        cmp    al, 0xfa
        je     loc_15_3134
        cmp    al, 0xfb
        je     loc_15_3134
loc_15_30F2:
        mov    ecx, 8
        mov    edi, 0xf0c                    ; -> loc_1_0F0C
loc_15_30FC:
        push   ecx
        mov    edx, dword ptr [edi]
        or     edx, edx
        je     loc_15_311C
        mov    eax, dword ptr [edi + 4]
        mov    esi, dword ptr [edi + 8]
        push   0
        push   eax
        push   esi
        push   ebx
        push   edx
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        or     eax, eax
        je     loc_15_3131
loc_15_311C:
        pop    ecx
        add    edi, 0x14
        dec    ecx
        jne    loc_15_30FC
        call   sub_15_0660
        mov    byte ptr [0xfb1], 0           ; -> obj1:0x0FB1
        jmp    loc_15_313C
loc_15_3131:
        pop    ecx
        jmp    loc_15_313C
loc_15_3134:
        popfd  
        popal  
        jmp    loc_1_0FAC
loc_15_313C:
        popfd  
        popal  
        ret    
VDD_PM_API:
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_31BC
        movzx  ecx, byte ptr [ebp + 0x1c]
        cmp    ecx, 0x80
        jb     loc_15_3199
        sub    ecx, 0x80
        cmp    ecx, 0x11
        ja     loc_15_31BC
        cmp    ecx, 0xb
        jae    loc_15_316D
        jmp    dword ptr [ecx*4 + 0x31e5]    ; -> obj15:0x31E5
loc_15_316D:
        movzx  eax, word ptr [ebp + 0x38]
        or     eax, eax
        je     loc_15_317F
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
loc_15_317F:
        mov    ebx, esp
        push   dword ptr [ebp + 0x18]
        push   eax
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   dword ptr [ecx*4 + 0x31e5]    ; -> obj15:0x31E5
        mov    esp, ebx
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_15_31BC
loc_15_3199:
        cmp    cl, 9
        ja     loc_15_31BC
        mov    edx, ebx
        or     ecx, ecx
        je     loc_15_31B5
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edi, ebx
        add    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
loc_15_31B5:
        jmp    dword ptr [ecx*4 + 0x31bd]    ; -> obj15:0x31BD
loc_15_31BC:
        ret    
        ; ---- dispatch table (28 entries) ----
        dd  loc_10_020F
        dd  loc_15_32F0
        dd  loc_15_3232
        dd  loc_15_3260
        dd  loc_15_32E3
        dd  loc_15_3425
        dd  loc_15_332B
        dd  loc_15_3419
        dd  loc_15_322D
        dd  sub_15_3434
        dd  loc_9_02FF
        dd  loc_9_03CA
        dd  loc_9_0433
        dd  loc_9_0488
        dd  loc_9_0497
        dd  loc_9_17CD
        dd  loc_9_04A7
        dd  loc_9_04B8
        dd  loc_9_04C9
        dd  loc_1_0574
        dd  loc_15_31BC
        dd  sub_14_0C2B
        dd  loc_14_028E
        dd  loc_14_095D
        dd  loc_14_0A0C
        dd  sub_14_0A30
        dd  loc_14_0B3E
        dd  loc_5_006C
loc_15_322D:
        jb     loc_15_3258
        clc    
        jmp    loc_15_3235
loc_15_3232:
        jb     loc_15_3258
        stc    
loc_15_3235:
        pushfd 
        push   edx
        movzx  esi, word ptr [ebp + 0x38]
        push   0
        push   esi
        push   edx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        inc    eax
        je     loc_15_3259
        dec    eax
        add    eax, dword ptr [ebp]
        mov    esi, eax
        popfd  
        jmp    loc_15_1D86
loc_15_3258:
        pushfd 
loc_15_3259:
        xor    eax, eax
        mov    dword ptr [ebp + 0x18], eax
        popfd  
        ret    
loc_15_3260:
        jb     loc_15_32DC
        push   edx
        movzx  esi, word ptr [ebp + 0x38]
        push   0
        push   esi
        push   edx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        inc    eax
        je     loc_15_32DC
        dec    eax
        add    eax, dword ptr [ebp]
        mov    esi, eax
        call   sub_15_3F63
        jb     loc_15_32C5
        test   byte ptr [edi + 3], 0x80
        je     loc_15_32C1
        test   byte ptr [edi], 4
        jne    loc_15_32C1
        call   sub_15_1C79
        call   sub_15_0314
loc_15_329A:
        add    ecx, 4
        mov    word ptr [ebp + 0x18], cx
        mov    ecx, 0xffff0000
        xchg   dword ptr [edi + 0x5c8], ecx
        mov    word ptr [ebp + 0x10], cx
        shr    ecx, 0x10
        mov    word ptr [ebp + 0x1c], cx
        movzx  cx, byte ptr [edi + 0x60]
        mov    word ptr [ebp + 0x14], cx
        ret    
loc_15_32C1:
        xor    eax, eax
        jmp    loc_15_32D1
loc_15_32C5:
        push   esi
        call   sub_9_0538
        pop    esi
        mov    eax, 0x8000
loc_15_32D1:
        xor    ecx, ecx
        mov    word ptr [esi], ax
        mov    word ptr [esi + 2], cx
        jmp    loc_15_329A
loc_15_32DC:
        mov    word ptr [ebp + 0x18], 0
        ret    
loc_15_32E3:
        jb     loc_15_32EF
        call   sub_15_1D5E
        call   sub_15_024B
loc_15_32EF:
        ret    
loc_15_32F0:
        jb     loc_15_3324
        inc    dword ptr [edi + 0x600]
        cmp    byte ptr [edi + 0x9d], 0xd
        jb     loc_15_3311
        xchg   ebx, edx
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        mov    dword ptr [edi + 0x5ec], eax
        xchg   ebx, edx
loc_15_3311:
        lea    esi, [edi + 0x543]
        call   sub_15_333F
        jb     loc_15_3323
        call   sub_15_33F9
loc_15_3323:
        ret    
loc_15_3324:
        mov    word ptr [ebp + 0x18], 0
        ret    
loc_15_332B:
        jb     loc_15_3324
        lea    esi, [edi + 0x57b]
        cmp    dword ptr [esi + 4], 0
        jne    sub_15_333F
        lea    esi, [edi + 0x543]
sub_15_333F:
        movzx  eax, byte ptr [edi + 0x60]
        mov    word ptr [ebp + 0x14], ax
        push   edi
        mov    word ptr [ebp + 0x18], 0x28
        movzx  eax, word ptr [ebp + 0x38]
        push   edx
        push   0
        push   eax
        push   edx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        inc    eax
        je     loc_15_33C3
        dec    eax
        add    eax, dword ptr [ebp]
        lea    ecx, [edi + 0xa0]
        mov    dword ptr [eax + 0x24], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        movzx  ecx, word ptr [esi + 8]
        shl    ecx, 0xc
        mov    dword ptr [eax + 0x14], ecx
        test   byte ptr [edi + 0x7c], 1
        je     loc_15_33CC
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 4], ecx
        movzx  ecx, word ptr [esi + 0x14]
        shl    ecx, 0xc
        mov    dword ptr [eax + 0x18], ecx
        mov    ecx, dword ptr [esi + 0x1c]
        mov    dword ptr [eax + 8], ecx
        movzx  ecx, word ptr [esi + 0x20]
        shl    ecx, 0xc
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, dword ptr [esi + 0x28]
        mov    dword ptr [eax + 0xc], ecx
        movzx  ecx, word ptr [esi + 0x2c]
        shl    ecx, 0xc
        mov    dword ptr [eax + 0x20], ecx
loc_15_33B6:
        push   eax
        call   sub_15_168F
        pop    edi
        mov    dword ptr [edi + 0x10], eax
        pop    edi
        clc    
        ret    
loc_15_33C3:
        xor    eax, eax
        mov    word ptr [ebp + 0x18], ax
        pop    edi
        stc    
        ret    
loc_15_33CC:
        push   esi
        push   edx
        push   eax
        call   sub_15_017A
        pop    eax
        mov    dword ptr [eax + 4], edx
        shl    ecx, 0xc
        mov    dword ptr [eax + 0x18], ecx
        pop    edx
        pop    esi
        mov    ecx, dword ptr [esi + 0x1c]
        mov    dword ptr [eax + 8], ecx
        movzx  ecx, word ptr [esi + 0x20]
        shl    ecx, 0xc
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], 0
        jmp    loc_15_33B6
sub_15_33F9:
        push   ebx
        push   edi
        mov    ebx, edx
        mov    edi, ebx
        add    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        bts    dword ptr [edi], 0x12
        jb     loc_15_3416
        mov    eax, 0x10
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_15_3416:
        pop    edi
        pop    ebx
        ret    
loc_15_3419:
        jb     loc_15_3424
        test   byte ptr [edi], 1
        je     sub_15_016E
loc_15_3424:
        ret    
loc_15_3425:
        jb     loc_15_3433
        dec    dword ptr [edi + 0x600]
        clc    
        call   sub_15_3434
loc_15_3433:
        ret    
sub_15_3434:
        jb     loc_15_3479
        mov    eax, dword ptr [edi + 0x600]
        or     eax, eax
        jne    loc_15_3479
        push   edi
        mov    edi, edx
        add    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        btr    dword ptr [edi], 0x12
        pop    edi
        jae    loc_15_345D
        xchg   ebx, edx
        mov    eax, 0xfffffff0
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_15_345D:
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        sub    eax, dword ptr [edi + 0x5ec]
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
        xchg   ebx, edx
        neg    eax
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
loc_15_3479:
        ret    
        db  000h, 000h
sub_15_347C:
        mov    dword ptr [edi + 0x5b4], eax
        shl    eax, 2
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_15_34B8
        mov    dword ptr [edi + 0x5b8], eax
        mov    eax, dword ptr [edi + 0x5b4]
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_15_34B8
        mov    dword ptr [edi + 0x5bc], eax
        stc    
loc_15_34B8:
        cmc    
        ret    
sub_15_34BA:
        push   eax
        inc    eax
        shl    eax, 2
        push   1
        push   eax
        push   dword ptr [edi + 0x5b8]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        test   eax, eax
        je     loc_15_3506
        mov    dword ptr [edi + 0x5b8], eax
        mov    eax, dword ptr [esp]
        inc    eax
        push   1
        push   eax
        push   dword ptr [edi + 0x5bc]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        test   eax, eax
        je     loc_15_3506
        mov    dword ptr [edi + 0x5bc], eax
        mov    eax, dword ptr [esp]
        inc    eax
        mov    dword ptr [edi + 0x5b4], eax
        stc    
loc_15_3506:
        cmc    
        pop    eax
        ret    
sub_15_3509:
        test   byte ptr [edi + 7], 0x80
        je     loc_15_3525
        mov    edx, dword ptr [0xfb4]        ; -> loc_1_0FB4
        push   0
        push   2
        push   edx
        push   ebx
        push   edx
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
loc_15_3525:
        test   byte ptr [edi + 4], 0x20
        je     loc_15_3540
        mov    edx, 0xb0
        push   0
        push   8
        push   edx
        push   ebx
        push   edx
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
loc_15_3540:
        clc    
        ret    
sub_15_3542:
        push   edx
        push   ecx
        push   eax
        mov    ecx, dword ptr [edi + 0x5bc]
        test   byte ptr [eax + ecx], 2
        je     loc_15_355A
        mov    ecx, eax
        call   sub_15_3D8B
        mov    eax, ecx
loc_15_355A:
        push   ebx
        call   sub_15_2D54
        mov    ecx, ebx
        pop    ebx
        jb     loc_15_3586
        cmp    ebx, ecx
        jne    loc_15_357F
        movzx  ecx, word ptr [eax + 4]
        cmp    ecx, dword ptr [esp]
        je     loc_15_35CF
        xchg   dword ptr [esp], eax
        call   sub_15_2907
        xchg   dword ptr [esp], eax
        je     loc_15_35A5
loc_15_357F:
        call   sub_15_2D96
        jb     loc_15_35DA
loc_15_3586:
        mov    eax, dword ptr [esp]
        mov    esi, dword ptr [edi + 0x5b8]
        cmp    dword ptr [esi + eax*4], 0
        je     loc_15_35A5
        mov    ecx, eax
        mov    eax, dword ptr [esi + eax*4]
        xor    esi, esi
        call   sub_15_2D10
        jae    loc_15_35B6
        jmp    loc_15_35DA
loc_15_35A5:
        mov    eax, dword ptr [esp]
        mov    ecx, eax
        mov    esi, 0x3e7e                   ; -> loc_15_3E7E
        call   sub_15_2B10
        jb     loc_15_35DA
loc_15_35B6:
        mov    esi, dword ptr [edi + 0x5b8]
        mov    dword ptr [esi + ecx*4], eax
        mov    esi, dword ptr [edi + 0x5bc]
        add    esi, ecx
        and    byte ptr [esi], 1
        call   sub_15_4177
loc_15_35CF:
        mov    esi, eax
        call   sub_15_2CA7
loc_15_35D6:
        pop    eax
        pop    ecx
        pop    edx
        ret    
loc_15_35DA:
        call   sub_9_050A
        jmp    loc_15_35D6
sub_15_35E1:
        push   ecx
        lea    edx, [ecx + eax]
        cmp    dword ptr [edi + 0x5b4], edx
        jae    loc_15_35F7
        push   eax
        lea    eax, [edx - 1]
        call   sub_15_34BA
        pop    eax
loc_15_35F7:
        pop    ecx
        ret    
sub_15_35F9:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_49DE
        call   sub_15_3A1F
        call   sub_15_10CC
        call   sub_15_29F4
        pushfd 
        cmp    ecx, 0x20
        jbe    loc_15_3620
        mov    ecx, 0x20
loc_15_3620:
        call   sub_15_35E1
        jae    loc_15_3636
        pushal 
        call   sub_9_0538
        popal  
        mov    ecx, dword ptr [edi + 0x5b4]
        sub    ecx, eax
loc_15_3636:
        mov    byte ptr [edi + 0x606], al
        mov    word ptr [edi + 0x607], cx
loc_15_3643:
        call   sub_15_3542
        inc    eax
        loop   loc_15_3643
        popfd  
        jb     loc_15_366E
        movzx  ecx, byte ptr [edi + 0x4a6]
        xor    eax, eax
loc_15_3657:
        jecxz  loc_15_366E
        shr    ecx, 1
        jae    loc_15_3669
        call   sub_15_3542
        inc    eax
        call   sub_15_3542
        dec    eax
loc_15_3669:
        add    eax, 2
        jmp    loc_15_3657
loc_15_366E:
        call   sub_15_2C54
        call   sub_9_00C8
        call   sub_15_4A18
        call   sub_15_10EA
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_15_3694
        call   sub_15_119E
        call   sub_15_4B2C
loc_15_3694:
        ret    
sub_15_3695:
        push   esi
        lea    esi, [esi]
        mov    ecx, 4
loc_15_369D:
        push   ecx
        xor    eax, eax
        mov    dword ptr [esi + 4], eax
        mov    word ptr [esi + 8], ax
        mov    word ptr [esi + 0xa], ax
        xchg   dword ptr [esi], eax
        or     eax, eax
        je     loc_15_36BD
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_15_36BD:
        add    esi, 0xc
        pop    ecx
        loop   loc_15_369D
        pop    esi
        ret    
sub_15_36C5:
        push   esi
        lea    esi, [edi + 0x543]
        call   sub_15_3695
        pop    esi
        ret    
sub_15_36D3:
        pushal 
        call   sub_15_29F4
        add    eax, ecx
        cmp    byte ptr [edi + 0x5b3], 0x58
        je     loc_15_3720
        xor    ebx, ebx
        xor    ecx, ecx
        mov    dl, byte ptr [edi + 0x7c]
        test   byte ptr [edi + 0x23], 1
        je     loc_15_36FF
        and    dl, 3
        cmp    dl, 3
        je     loc_15_3727
        test   byte ptr [edi + 0x6a], 4
        jne    loc_15_3720
loc_15_36FF:
        test   dl, 1
        jne    loc_15_3729
        mov    ax, 8
        or     dh, dh
        jne    loc_15_3729
        movzx  ebx, byte ptr [edi + 0x4a6]
loc_15_3713:
        adc    ecx, 0
        shr    ebx, 1
        jb     loc_15_3713
        jne    loc_15_3713
        shl    ecx, 1
        jmp    loc_15_3729
loc_15_3720:
        mov    ebx, eax
        mov    ecx, eax
        push   eax
        jmp    loc_15_372B
loc_15_3727:
        mov    ecx, eax
loc_15_3729:
        push   0
loc_15_372B:
        lea    ebp, [esi + 8]
        call   sub_15_3752
        mov    eax, ebx
        lea    ebp, [esi + 0x14]
        call   sub_15_3752
        mov    eax, ecx
        lea    ebp, [esi + 0x20]
        call   sub_15_3752
        pop    eax
        lea    ebp, [esi + 0x2c]
        call   sub_15_3752
        popal  
        ret    
sub_15_3752:
        test   byte ptr [edi + 0x5e2], 0x80
        jne    loc_15_3760
        test   byte ptr [edi], 8
        je     loc_15_3766
loc_15_3760:
        cmp    ax, word ptr [ebp]
        jbe    loc_15_376A
loc_15_3766:
        mov    word ptr [ebp], ax
loc_15_376A:
        ret    
sub_15_376B:
        push   ecx
        movzx  eax, word ptr [esi + 8]
        mov    ecx, dword ptr [esi]
        or     eax, eax
        je     loc_15_37AB
        jecxz  loc_15_378B
        push   dword ptr [0xfb8]             ; -> obj1:0x0FB8
        push   eax
        push   ecx
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        jmp    loc_15_37A4
loc_15_378B:
        xor    ecx, ecx
        push   dword ptr [0xfb8]             ; -> obj1:0x0FB8
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   ebx
        push   7
        push   eax
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
loc_15_37A4:
        or     eax, eax
        jne    loc_15_37BD
        stc    
        jmp    loc_15_37CB
loc_15_37AB:
        jecxz  loc_15_37B9
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_15_37B9:
        xor    eax, eax
        xor    edx, edx
loc_15_37BD:
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
        movzx  edx, word ptr [esi + 8]
        mov    word ptr [esi + 0xa], dx
        clc    
loc_15_37CB:
        pop    ecx
        ret    
sub_15_37CD:
        pushal 
        test   byte ptr [edi + 4], 1
        je     loc_15_37DC
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_15_3849
loc_15_37DC:
        and    byte ptr [edi + 9], 0xfe
        lea    esi, [esi]
        mov    ecx, 4
loc_15_37E7:
        movzx  eax, word ptr [esi + 0xa]
        cmp    ax, word ptr [esi + 8]
        je     loc_15_3843
        jb     loc_15_37FC
        test   byte ptr [edi + 0x5e2], 0x80
        jne    loc_15_3843
loc_15_37FC:
        push   eax
        cmp    byte ptr [edi + 0x605], 0
        je     loc_15_380D
        push   ecx
        call   sub_15_3A50
        pop    ecx
loc_15_380D:
        call   sub_15_376B
        pop    eax
        jb     loc_15_384B
        cmp    cl, 4
        jne    loc_15_3843
        test   byte ptr [edi + 0x7c], 1
        jne    loc_15_3843
        cmp    ax, word ptr [esi + 0xa]
        jae    loc_15_3843
        push   ecx
        push   edi
        mov    edi, dword ptr [esi + 4]
        movzx  ecx, word ptr [esi + 0xa]
        sub    ecx, eax
        shl    ecx, 0xa
        shl    eax, 0xc
        add    edi, eax
        mov    eax, 0x7200720
        cld    
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    ecx
loc_15_3843:
        add    esi, 0xc
        loop   loc_15_37E7
        clc    
loc_15_3849:
        popal  
        ret    
loc_15_384B:
        or     byte ptr [edi + 9], 1
        stc    
        jmp    loc_15_3849
sub_15_3852:
        test   byte ptr [edi + 0x7c], 1
        jne    loc_15_3863
        push   eax
        shr    eax, 1
        bt     dword ptr [edi + 0x4a6], eax
        pop    eax
loc_15_3863:
        ret    
sub_15_3864:
        pushal 
        test   byte ptr [edi + 2], 0x40
        jne    loc_15_387D
        lea    esi, [edi + 0x543]
        xor    edx, edx
        call   sub_15_36D3
        call   sub_15_37CD
loc_15_387D:
        cmp    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        jne    loc_15_388F
        call   sub_9_00C8
        call   sub_15_4A18
loc_15_388F:
        xor    ebp, ebp
        call   sub_15_29F4
        jb     loc_15_389F
        movzx  ebp, byte ptr [edi + 0x4a6]
loc_15_389F:
        call   sub_15_35E1
        jae    loc_15_38AE
        mov    ecx, dword ptr [edi + 0x5b4]
        sub    ecx, eax
loc_15_38AE:
        lea    edx, [ecx + eax - 1]
        push   edi
        mov    esi, dword ptr [edi + 0x5b8]
        mov    ecx, dword ptr [edi + 0x5bc]
        push   dword ptr [edi + 0x5b4]
        xor    edi, edi
loc_15_38C7:
        push   eax
        mov    eax, edi
        shr    eax, 1
        bt     ebp, eax
        pop    eax
        jb     loc_15_38DA
        cmp    edi, eax
        jb     loc_15_38F3
        cmp    edx, edi
        jb     loc_15_38F3
loc_15_38DA:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_3933
        push   edi
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_3A9D
        pop    edi
        jb     loc_15_3933
loc_15_38F3:
        push   eax
        pushfd 
        mov    eax, dword ptr [esi + edi*4]
        or     eax, eax
        je     loc_15_3931
        popfd  
        pushfd 
        jae    loc_15_3927
        and    byte ptr [edi + ecx], 0xfe
        pushal 
        xor    ecx, ecx
        xchg   word ptr [eax + 6], cx
        jecxz  loc_15_3922
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   1
        push   ecx
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
loc_15_3922:
        popal  
        and    byte ptr [eax + 3], 0xbf
loc_15_3927:
        call   sub_15_10CC
        call   sub_15_2D96
loc_15_3931:
        popfd  
        pop    eax
loc_15_3933:
        inc    edi
        cmp    edi, dword ptr [esp]
        jb     loc_15_38C7
        add    esp, 4
        pop    edi
        call   sub_15_3E44
        call   sub_15_024B
        call   sub_15_10EA
        popal  
        ret    
sub_15_394E:
        test   byte ptr [edi + 2], 0x40
        jne    loc_15_3968
        lea    esi, [edi + 0x543]
        xor    edx, edx
        call   sub_15_36D3
        call   sub_15_37CD
        jmp    loc_15_396D
loc_15_3968:
        call   sub_15_36C5
loc_15_396D:
        ret    
sub_15_396E:
        push   eax
        push   ecx
        call   sub_15_29F4
        mov    byte ptr [edi + 0x606], al
        mov    word ptr [edi + 0x607], cx
        push   edx
        lea    edx, [ecx + eax]
        cmp    edx, dword ptr [edi + 0x5b4]
        pop    edx
        jbe    loc_15_3997
        mov    ecx, dword ptr [edi + 0x5b4]
        sub    ecx, eax
loc_15_3997:
        call   sub_15_3542
        inc    eax
        loop   loc_15_3997
        dec    eax
        call   sub_15_39AD
        call   sub_9_00C8
        pop    ecx
        pop    eax
        ret    
sub_15_39AD:
        pushal 
        test   byte ptr [edi + 2], 0x40
        jne    loc_15_3A1A
        mov    ecx, eax
        inc    ecx
        test   byte ptr [edi + 0x23], 1
        je     loc_15_39D5
        mov    dl, byte ptr [edi + 0x7c]
        and    dl, 3
        cmp    dl, 3
        je     loc_15_39E7
        mov    dl, byte ptr [edi + 0x6a]
        test   dl, 4
        je     loc_15_39D5
        test   dl, 8
        je     loc_15_39EB
loc_15_39D5:
        cmp    cx, word ptr [edi + 0x54b]
        jbe    loc_15_3A1C
        mov    word ptr [edi + 0x54b], cx
        jmp    loc_15_3A0F
loc_15_39E7:
        mov    dl, 5
        jmp    loc_15_39EB
loc_15_39EB:
        xor    dh, dh
        mov    eax, ecx
        lea    esi, [edi + 0x543]
        mov    ecx, 4
loc_15_39FA:
        cmp    ax, word ptr [esi + 8]
        jbe    loc_15_3A06
        mov    word ptr [esi + 8], ax
        inc    dh
loc_15_3A06:
        add    esi, 0xc
        loop   loc_15_39FA
        or     dh, dh
        je     loc_15_3A1C
loc_15_3A0F:
        lea    esi, [edi + 0x543]
        call   sub_15_37CD
loc_15_3A1A:
        popal  
        ret    
loc_15_3A1C:
        clc    
        jmp    loc_15_3A1A
sub_15_3A1F:
        push   eax
        mov    eax, dword ptr [0x7ac]        ; -> loc_1_07AC
        cmp    eax, ebx
        je     loc_15_3A48
        or     eax, eax
        je     loc_15_3A46
loc_15_3A2D:
        cmp    eax, dword ptr [0x7b8]        ; -> loc_1_07B8
        je     loc_15_3A46
        pushal 
        mov    ebx, eax
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        call   sub_15_3A50
        popal  
loc_15_3A46:
        pop    eax
        ret    
loc_15_3A48:
        test   byte ptr [edi + 1], 0x40
        je     loc_15_3A46
        jmp    loc_15_3A2D
sub_15_3A50:
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_3A9C
        or     ebx, ebx
        je     loc_15_3A9C
        push   ebp
        push   esi
        mov    esi, dword ptr [edi + 0x5b8]
        mov    ebp, dword ptr [edi + 0x5bc]
        call   sub_15_2E25
        xor    ecx, ecx
loc_15_3A72:
        test   byte ptr [ebp + ecx], 2
        je     loc_15_3A80
        call   sub_15_3D8B
        jmp    loc_15_3A8C
loc_15_3A80:
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     loc_15_3A8C
        call   sub_15_3EDC
loc_15_3A8C:
        inc    ecx
        cmp    ecx, dword ptr [edi + 0x5b4]
        jb     loc_15_3A72
        pop    esi
        pop    ebp
        jmp    sub_15_3E44
loc_15_3A9C:
        ret    
sub_15_3A9D:
        test   byte ptr [edi + 0x6a], 4
        je     loc_15_3AAF
        test   byte ptr [edi + 0x23], 1
        je     loc_15_3AAF
        test   byte ptr [edi + 0x6a], 8
        je     loc_15_3AB9
loc_15_3AAF:
        push   eax
        mov    al, byte ptr [edi + 0x7b]
        and    al, 3
        pop    eax
        jne    loc_15_3AB9
        ret    
loc_15_3AB9:
        stc    
        ret    
loc_15_3ABB:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    dword ptr [0xef4], 0          ; -> loc_1_0EF4
        je     loc_15_3AEE
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_3AEE
        mov    esi, dword ptr [0xef4]        ; -> loc_1_0EF4
loc_15_3ADA:
        cmp    byte ptr [esi], 0
        je     loc_15_3AEE
        push   eax
        mov    eax, 0x64
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        pop    eax
        jmp    loc_15_3ADA
loc_15_3AEE:
        mov    ecx, 6
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        push   eax
        or     byte ptr [edi + 3], 0xc
        test   byte ptr [0x7a0], 3           ; -> loc_1_07A0
        jne    loc_15_3C91
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_3B20
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_3B20
        call   sub_15_1969
loc_15_3B20:
        call   sub_15_2AE7
        jb     loc_15_3C91
        cmp    eax, dword ptr [edi + 0x5b4]
        jb     loc_15_3B3E
        call   sub_15_34BA
        jb     loc_15_3C91
loc_15_3B3E:
        test   byte ptr [edi], 0x20
        jne    loc_15_3B5E
        mov    cl, byte ptr [edi + 0x15]
        and    cl, 0x1f
        cmp    cl, 2
        jbe    loc_15_3B69
        mov    cl, byte ptr [edi + 0x7c]
        test   cl, 1
        jne    loc_15_3B5B
        test   cl, 2
        jne    loc_15_3B69
loc_15_3B5B:
        or     byte ptr [edi], 0x40
loc_15_3B5E:
        mov    ecx, eax
        shr    ecx, 1
        bts    dword ptr [edi + 0x4a6], ecx
loc_15_3B69:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_3B9F
        test   byte ptr [edi], 0x80
        jne    loc_15_3C91
        test   byte ptr [edi], 0x60
        jne    loc_15_3B86
        call   sub_15_3A9D
        jb     loc_15_3B9F
loc_15_3B86:
        mov    ecx, dword ptr [esp]
        call   sub_15_3CA8
        jb     loc_15_3B96
        pop    eax
        jmp    loc_15_3C9C
loc_15_3B96:
        cmp    eax, -1
        je     loc_15_3C91
loc_15_3B9F:
        mov    esi, dword ptr [edi + 0x5b8]
        lea    esi, [esi + eax*4]
        mov    ecx, dword ptr [esi]
        or     ecx, ecx
        jne    loc_15_3BBA
        test   byte ptr [edi], 2
        jne    loc_15_3BD6
        call   sub_15_39AD
        jmp    loc_15_3BD6
loc_15_3BBA:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_3C03
        call   sub_15_29DC
        je     loc_15_3C03
        xchg   ecx, eax
        call   sub_15_10CC
        call   sub_15_2D96
        mov    eax, ecx
loc_15_3BD6:
        call   sub_15_10CC
        push   eax
        push   esi
        mov    esi, 0x3e7e                   ; -> loc_15_3E7E
        call   sub_15_2B10
        pop    esi
        mov    ecx, eax
        pop    eax
        jb     loc_15_3C63
        mov    dword ptr [esi], ecx
        add    eax, dword ptr [edi + 0x5bc]
        and    byte ptr [eax], 1
        mov    esi, eax
        mov    eax, ecx
        call   sub_15_4177
        mov    ecx, eax
loc_15_3C03:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_3C2D
        test   byte ptr [edi], 2
        jne    loc_15_3C2D
        push   ecx
        call   sub_15_29F4
        cmp    cx, word ptr [edi + 0x607]
        pop    ecx
        jne    loc_15_3C28
        cmp    al, byte ptr [edi + 0x606]
        je     loc_15_3C2D
loc_15_3C28:
        call   sub_15_396E
loc_15_3C2D:
        mov    eax, ecx
        call   sub_15_10EA
        mov    edx, 1
        call   sub_15_2E3E
        mov    esi, eax
        cmp    word ptr [esi + 6], 0
        je     loc_15_3C4C
        call   sub_15_3EDC
loc_15_3C4C:
        pop    eax
        mov    word ptr [esi + 6], ax
        push   0
        push   1
        push   eax
        push   ebx
        push   ecx
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        jmp    loc_15_3C9C
loc_15_3C63:
        call   sub_15_10EA
        test   byte ptr [edi + 2], 2
        je     loc_15_3C7B
        test   byte ptr [edi + 4], 0x10
        je     loc_15_3C91
        call   sub_15_498E
        jmp    loc_15_3C91
loc_15_3C7B:
        test   byte ptr [edi], 2
        jne    loc_15_3C91
        test   byte ptr [edi + 4], 0x40
        jne    loc_15_3C91
        mov    esi, 0x4ee                    ; -> sub_9_04EE
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
loc_15_3C91:
        call   sub_15_10EA
        pop    eax
        call   sub_15_3E18
loc_15_3C9C:
        and    dword ptr [edi], 0xf7ffffbf
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
sub_15_3CA8:
        pushal 
        lea    esi, [edi + 0x543]
        test   byte ptr [edi], 0x60
        je     loc_15_3CC9
        add    esi, 0x18
        cmp    ax, word ptr [esi + 8]
        jb     loc_15_3D0C
        mov    ecx, eax
        or     cl, 1
        inc    ecx
        mov    word ptr [esi + 8], cx
        jmp    loc_15_3D12
loc_15_3CC9:
        cmp    ax, word ptr [esi + 8]
        jb     loc_15_3D0C
        test   byte ptr [edi], 2
        je     loc_15_3CE0
loc_15_3CD4:
        popal  
        mov    eax, 0xffffffff
        stc    
        jmp    loc_15_3D8A
loc_15_3CE0:
        push   ecx
        mov    ecx, dword ptr [0xed2]        ; -> loc_1_0ED2
        cmp    ecx, dword ptr [0xecc]        ; -> obj1:0x0ECC
        pop    ecx
        jb     loc_15_3CF7
loc_15_3CF0:
        stc    
        popal  
        jmp    loc_15_3D8A
loc_15_3CF7:
        pushal 
        inc    ax
        mov    word ptr [esi + 8], ax
        lea    esi, [edi + 0x543]
        call   sub_15_37CD
        popal  
        jb     loc_15_3CD4
loc_15_3D0C:
        cmp    ax, word ptr [esi + 0xa]
        jb     loc_15_3D27
loc_15_3D12:
        push   eax
        call   sub_15_376B
        pop    eax
        jae    loc_15_3D27
        test   byte ptr [edi], 0x60
        je     loc_15_3CD4
        call   sub_9_04EE
        jmp    loc_15_3CF0
loc_15_3D27:
        push   0
        push   eax
        push   1
        push   dword ptr [esi]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_15_3CD4
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x18]
        push   0
        push   eax
        push   1
        push   ecx
        push   ebx
        push   dword ptr [esi]
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        inc    byte ptr [edi + 0x605]
        mov    esi, dword ptr [edi + 0x5b8]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     loc_15_3D71
        call   sub_15_2D96
loc_15_3D71:
        mov    esi, dword ptr [edi + 0x5bc]
        mov    eax, dword ptr [esp + 0x1c]
        or     byte ptr [esi + eax], 2
        test   byte ptr [edi], 0x60
        je     loc_15_3D88
        or     byte ptr [esi + eax], 4
loc_15_3D88:
        clc    
        popal  
loc_15_3D8A:
        ret    
sub_15_3D8B:
        push   esi
        mov    eax, ecx
        push   eax
        call   sub_15_2B01
        push   eax
        mov    ecx, eax
        shl    ecx, 0xc
        add    ecx, dword ptr [ebx + 4]
        shr    ecx, 0xc
        sub    esp, 4
        mov    eax, esp
        push   0
        push   eax
        push   1
        push   ecx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        pop    ecx
        test   cl, 0x40
        pop    ecx
        mov    eax, dword ptr [esp]
        mov    esi, dword ptr [edi + 0x5bc]
        je     loc_15_3DCD
        or     byte ptr [esi + eax], 1
        call   sub_15_456A
loc_15_3DCD:
        and    byte ptr [esi + eax], 0xfd
        dec    byte ptr [edi + 0x605]
        push   0
        push   7
        push   0
        push   -8
        push   1
        push   ecx
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        pop    ecx
        btr    dword ptr [esi + ecx], 2
        lea    esi, [edi + 0x543]
        jae    loc_15_3E00
        lea    esi, [edi + 0x55b]
loc_15_3E00:
        cmp    cx, word ptr [esi + 0xa]
        jae    loc_15_3E16
        push   0
        push   ecx
        push   1
        push   dword ptr [esi]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_15_3E16:
        pop    esi
        ret    
sub_15_3E18:
        push   edx
        push   eax
        mov    edx, eax
        sub    edx, 0xa0
        bts    dword ptr [edi + 0x5d8], edx
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        pop    edx
        push   0
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        pop    edx
        ret    
sub_15_3E44:
        mov    eax, dword ptr [edi + 0x5d8]
        or     eax, eax
        je     loc_15_3E7D
        pushal 
        mov    ebp, eax
        mov    esi, 0xa0
loc_15_3E56:
        shr    ebp, 1
        jae    loc_15_3E71
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   1
        push   esi
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        or     al, 1
loc_15_3E71:
        je     loc_15_3E76
        inc    esi
        jmp    loc_15_3E56
loc_15_3E76:
        mov    dword ptr [edi + 0x5d8], ebp
        popal  
loc_15_3E7D:
        ret    
loc_15_3E7E:
        jecxz  loc_15_3E8B
        cmp    ecx, 2
        je     loc_15_3E89
        jb     loc_15_3ED5
        clc    
        ret    
loc_15_3E89:
        stc    
        ret    
loc_15_3E8B:
        xor    edx, edx
        call   sub_15_3F3C
        je     loc_15_3EA6
        call   sub_15_4009
        mov    dl, 1
        push   eax
        movzx  eax, word ptr [eax + 4]
        call   sub_15_456A
        pop    eax
loc_15_3EA6:
        movzx  ecx, word ptr [eax + 4]
        mov    esi, dword ptr [edi + 0x5b8]
        mov    dword ptr [esi + ecx*4], 0
        add    ecx, dword ptr [edi + 0x5bc]
        and    byte ptr [ecx], 1
        or     byte ptr [ecx], dl
        push   eax
        mov    eax, dword ptr [0xf08]        ; -> loc_1_0F08
        or     eax, eax
        je     loc_15_3ED4
        test   byte ptr [eax], 2
        je     loc_15_3ED4
        or     byte ptr [eax], 4
loc_15_3ED4:
        pop    eax
loc_15_3ED5:
        call   sub_15_3EDC
loc_15_3EDA:
        clc    
        ret    
sub_15_3EDC:
        push   ecx
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_3F17
        call   sub_15_3F3C
        jb     loc_15_3F17
        je     loc_15_3EF7
        push   eax
        movzx  eax, word ptr [eax + 4]
        call   sub_15_456A
        pop    eax
loc_15_3EF7:
        xor    ecx, ecx
        xchg   word ptr [eax + 6], cx
        call   sub_15_2EBF
        push   0
        push   7
        push   0
        push   -8
        push   1
        push   ecx
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
loc_15_3F17:
        pop    ecx
        ret    
sub_15_3F19:
        push   edx
        push   eax
        shl    ecx, 0xc
        add    ecx, dword ptr [ebx + 4]
        shr    ecx, 0xc
        sub    esp, 4
        mov    eax, esp
        push   0
        push   eax
        push   1
        push   ecx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        pop    ecx
        pop    eax
        pop    edx
        ret    
sub_15_3F3C:
        push   ecx
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_3F5C
        test   byte ptr [eax + 3], 0x40
        jne    loc_15_3F61
        call   sub_15_3F19
        and    cl, 0x40
        je     loc_15_3F56
        or     byte ptr [eax + 3], cl
loc_15_3F56:
        test   byte ptr [eax + 3], 0x40
        pop    ecx
        ret    
loc_15_3F5C:
        test   byte ptr [eax + 3], 0x40
        stc    
loc_15_3F61:
        pop    ecx
        ret    
sub_15_3F63:
        test   byte ptr [edi + 9], 1
        je     loc_15_3F90
        pushal 
        lea    esi, [edi + 0x543]
        mov    ecx, 4
loc_15_3F75:
        movzx  eax, word ptr [esi + 8]
        cmp    ax, word ptr [esi + 0xa]
        jbe    loc_15_3F86
        call   sub_15_376B
        jb     loc_15_3F8F
loc_15_3F86:
        add    esi, 0xc
        loop   loc_15_3F75
        and    byte ptr [edi + 9], 0xfe
loc_15_3F8F:
        popal  
loc_15_3F90:
        ret    
sub_15_3F91:
        push   eax
        push   edx
        mov    dx, 0x3ce
        cmp    byte ptr [edi + 0x77], 0
        je     loc_15_3FA7
        mov    eax, 1
        call   sub_15_3FF1
loc_15_3FA7:
        cmp    byte ptr [edi + 0x79], 0
        je     loc_15_3FB7
        mov    eax, 3
        call   sub_15_3FF1
loc_15_3FB7:
        mov    al, byte ptr [edi + 0x7b]
        and    al, 0xb
        je     loc_15_3FC8
        mov    eax, 0xf405
        call   sub_15_3FF1
loc_15_3FC8:
        cmp    byte ptr [edi + 0x7e], 0xff
        jne    loc_15_3FD8
        mov    eax, 0xff0008
        call   sub_15_3FF1
loc_15_3FD8:
        mov    dl, 0xc4
        mov    al, byte ptr [esp]
        cmp    al, byte ptr [edi + 0x68]
        je     loc_15_3FEE
        shl    eax, 0x10
        mov    ax, 2
        call   sub_15_3FF1
loc_15_3FEE:
        pop    edx
        pop    eax
        ret    
sub_15_3FF1:
        out    dx, al
        jmp    loc_15_3FF4
loc_15_3FF4:
        inc    edx
        in     al, dx
        jmp    loc_15_3FF8
loc_15_3FF8:
        and    ah, al
        shr    eax, 8
        or     al, ah
        out    dx, al
        jmp    loc_15_4002
loc_15_4002:
        dec    edx
        call   sub_15_1193
        ret    
sub_15_4009:
        mov    ecx, 6
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        and    byte ptr [eax + 3], 0xbf
        call   sub_15_3F63
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_402E
        push   ebx
        xor    ebx, ebx
        call   sub_15_1143
        pop    ebx
loc_15_402E:
        call   sub_15_119E
        test   byte ptr [edi + 0x23], 1
        je     loc_15_4059
        mov    dl, byte ptr [edi + 0x7c]
        and    dl, 3
        cmp    dl, 3
        je     loc_15_40BF
        mov    dl, byte ptr [edi + 0x6a]
        test   dl, 8
        jne    loc_15_40FD
        test   dl, 4
        jne    loc_15_40F8
loc_15_4059:
        xor    edx, edx
        call   sub_15_2E3E
        mov    dl, 3
        call   sub_15_3F91
        lea    esi, [edi + 0x543]
        call   sub_15_43A3
        test   byte ptr [edi + 0x7c], 1
        jne    loc_15_416F
        push   eax
        call   sub_15_18BA
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_4091
        call   dword ptr [edi + 0xc]
loc_15_4091:
        pop    edi
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        mov    dx, 0x402
        call   sub_15_4324
        lea    esi, [edi + 0x55b]
        call   sub_15_4580
        call   sub_15_119E
        or     cl, 0xff
        jmp    loc_15_416F
loc_15_40BF:
        xor    edx, edx
        call   sub_15_2E3E
        call   sub_15_1193
        mov    dl, 3
        call   sub_15_3F91
        xor    edx, edx
        call   sub_15_4324
        lea    esi, [edi + 0x543]
        call   sub_15_43A3
        mov    dl, 2
        call   sub_15_4324
        lea    esi, [edi + 0x55b]
        call   sub_15_43A3
        jmp    loc_15_416F
loc_15_40F8:
        test   dl, 8
        je     loc_15_411B
loc_15_40FD:
        xor    edx, edx
        call   sub_15_2E3E
        mov    dl, 0xf
        call   sub_15_3F91
        lea    esi, [edi + 0x543]
        call   sub_15_43A3
        or     cl, 0xff
        jmp    loc_15_416F
loc_15_411B:
        push   eax
        call   sub_15_18BA
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        lea    esi, [edi + 0x543]
        xor    edx, edx
loc_15_4135:
        call   sub_15_4324
        cmp    word ptr [esi + 8], 0
        jne    loc_15_4158
        cmp    dl, 1
        jne    loc_15_4158
        test   byte ptr [edi], 0x60
        je     loc_15_4158
        sub    esi, 0xc
        call   sub_15_43E3
        add    esi, 0x18
        jmp    loc_15_4160
loc_15_4158:
        call   sub_15_43A3
        add    esi, 0xc
loc_15_4160:
        inc    dl
        cmp    dl, 4
        jb     loc_15_4135
        call   sub_15_119E
        or     cl, 0xff
loc_15_416F:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        clc    
        ret    
sub_15_4177:
        mov    ecx, 6
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        test   byte ptr [esi], 1
        je     loc_15_41A4
        mov    edx, 0x4448                   ; -> loc_15_4448
        mov    dword ptr [0xfbc], edx        ; -> sub_1_0FBC
        mov    dword ptr [0xfc0], edx        ; -> sub_1_0FC0
        mov    dword ptr [0xfc4], 0x4606     ; -> sub_1_0FC4
        jmp    loc_15_41C2
loc_15_41A4:
        mov    dword ptr [0xfbc], 0x4488     ; -> sub_1_0FBC
        mov    dword ptr [0xfc0], 0x44b6     ; -> sub_1_0FC0
        mov    dword ptr [0xfc4], 0x4488     ; -> sub_1_0FC4
loc_15_41C2:
        call   sub_15_119E
        test   byte ptr [edi + 0x23], 1
        je     loc_15_41F1
        mov    dl, byte ptr [edi + 0x7c]
        and    dl, 3
        cmp    dl, 3
        je     loc_15_426B
        mov    dl, byte ptr [edi + 0x6a]
        test   dl, 8
        jne    loc_15_42AF
        test   dl, 4
        jne    loc_15_42AA
loc_15_41F1:
        xor    edx, edx
        call   sub_15_2E3E
        lea    esi, [edi + 0x543]
        mov    dl, 3
        call   sub_15_3F91
        test   byte ptr [edi + 0x7c], 1
        je     loc_15_4216
        call   sub_1_0FBC
        jmp    loc_15_431C
loc_15_4216:
        call   sub_1_0FC0
        movzx  ecx, word ptr [eax + 4]
        shr    ecx, 1
        bt     dword ptr [edi + 0x4a6], ecx
        jae    loc_15_431C
        push   eax
        call   sub_15_18BA
        push   edi
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_15_4244
        call   dword ptr [edi + 0xc]
loc_15_4244:
        pop    edi
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        mov    dx, 0x402
        call   sub_15_4324
        lea    esi, [edi + 0x55b]
        call   sub_1_0FC4
        jmp    loc_15_431C
loc_15_426B:
        xor    edx, edx
        call   sub_15_2E3E
        call   sub_15_1193
        mov    dl, 3
        call   sub_15_3F91
        mov    dx, 0x300
        call   sub_15_4324
        lea    esi, [edi + 0x543]
        call   sub_1_0FBC
        mov    dx, 0xc02
        call   sub_15_4324
        lea    esi, [edi + 0x55b]
        call   sub_1_0FBC
        jmp    loc_15_431C
loc_15_42AA:
        test   dl, 8
        je     loc_15_42CB
loc_15_42AF:
        xor    edx, edx
        call   sub_15_2E3E
        mov    dl, 0xf
        call   sub_15_3F91
        lea    esi, [edi + 0x543]
        call   sub_1_0FBC
        jmp    loc_15_431C
loc_15_42CB:
        push   eax
        call   sub_15_18BA
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        lea    esi, [edi + 0x543]
        xor    edx, edx
        inc    dh
loc_15_42E7:
        call   sub_15_4324
        cmp    word ptr [esi + 8], 0
        jne    loc_15_430A
        cmp    dl, 1
        jne    loc_15_430A
        test   byte ptr [edi], 0x60
        je     loc_15_430A
        sub    esi, 0xc
        call   sub_15_4426
        add    esi, 0x18
        jmp    loc_15_4313
loc_15_430A:
        call   sub_1_0FBC
        add    esi, 0xc
loc_15_4313:
        inc    dl
        shl    dh, 1
        cmp    dl, 4
        jb     loc_15_42E7
loc_15_431C:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        clc    
        ret    
sub_15_4324:
        push   eax
        push   edi
        push   edx
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_15_4337
        call   dword ptr [edi + 0x6c]
        jae    loc_15_435A
loc_15_4337:
        mov    edx, 0x3ce
        mov    al, 4
        out    dx, al
        jmp    loc_15_4341
loc_15_4341:
        inc    dl
        mov    al, byte ptr [esp]
        out    dx, al
        jmp    loc_15_4349
loc_15_4349:
        mov    edx, 0x3c4
        mov    al, 2
        out    dx, al
        jmp    loc_15_4353
loc_15_4353:
        inc    dl
        mov    al, byte ptr [esp + 1]
        out    dx, al
loc_15_435A:
        pop    edx
        pop    edi
        pop    eax
        ret    
sub_15_435E:
        cmp    dx, word ptr [esi + 0xa]
        jb     loc_15_437A
        cmp    dx, word ptr [esi + 8]
        jae    loc_15_437A
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_15_4379
        pushal 
        call   sub_9_0538
        popal  
loc_15_4379:
        clc    
loc_15_437A:
        cmc    
        ret    
sub_15_437C:
        call   sub_15_435E
        jb     loc_15_43A2
        push   edx
        push   ecx
        push   0
        push   edx
        push   1
        push   dword ptr [esi]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        pop    ecx
        pop    edx
        or     eax, eax
        stc    
        je     loc_15_43A2
        pop    eax
        push   dword ptr [esi]
        push   edx
        push   eax
        clc    
loc_15_43A2:
        ret    
sub_15_43A3:
        pushal 
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     loc_15_43E1
        mov    edi, edx
        shl    edi, 0xc
        add    edi, dword ptr [esi + 4]
        shl    ecx, 0xc
        sub    ecx, 0xa0000
        add    ecx, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    esi, ecx
        mov    ecx, 0x1000
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_15_43E1:
        popal  
        ret    
sub_15_43E3:
        pushal 
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     loc_15_4424
        mov    edi, edx
        shl    edi, 0xc
        add    edi, dword ptr [esi + 4]
        shl    ecx, 0xc
        sub    ecx, 0xa0000
        add    ecx, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    esi, ecx
        mov    ecx, 0x800
        cld    
loc_15_440E:
        inc    edi
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    esi
        loop   loc_15_440E
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_15_4424:
        popal  
        ret    
sub_15_4426:
        pushal 
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     loc_15_4442
        shl    edx, 0xc
        add    edx, dword ptr [esi + 4]
        inc    edx
        mov    edi, ecx
        mov    ecx, 0xfff
        jmp    loc_15_4461
loc_15_4442:
        mov    ax, 0x2007
        jmp    loc_15_44BB
loc_15_4448:
        pushal 
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     loc_15_4489
        shl    edx, 0xc
        add    edx, dword ptr [esi + 4]
        mov    edi, ecx
        mov    ecx, 0x1000
loc_15_4461:
        shl    edi, 0xc
        sub    edi, 0xa0000
        add    edi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    esi, edx
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        popal  
        ret    
loc_15_4488:
        pushal 
loc_15_4489:
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        je     loc_15_44B4
        test   dword ptr [edi], 0x2000
        jne    loc_15_44B4
        shl    ecx, 0xc
        sub    ecx, 0xa0000
        add    ecx, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    edi, ecx
        xor    eax, eax
        mov    ecx, 0x1000
        cld    
        rep stosb byte ptr es:[edi], al
loc_15_44B4:
        popal  
        ret    
loc_15_44B6:
        pushal 
        mov    ax, 0x720
loc_15_44BB:
        shl    ecx, 0xc
        sub    ecx, 0xa0000
        add    ecx, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    edi, ecx
        mov    ecx, 0x800
        cld    
loc_15_44D2:
        stosb  byte ptr es:[edi], al
        xchg   al, ah
        stosb  byte ptr es:[edi], al
        xchg   al, ah
        loop   loc_15_44D2
        popal  
        ret    
sub_15_44DC:
        push   esi
        mov    esi, dword ptr [edi + 0x5b8]
        xor    ecx, ecx
loc_15_44E5:
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     loc_15_452D
        push   ecx
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_452C
        call   sub_15_3F19
        test   cl, 0x40
        je     loc_15_452C
        or     byte ptr [eax + 3], 0x40
        movzx  ecx, word ptr [eax + 6]
        push   ecx
        push   0
        push   -1
        push   0
        push   -1
        push   1
        push   ecx
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        pop    ecx
        shl    ecx, 0xc
        add    ecx, dword ptr [ebx + 4]
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [esp]
        call   sub_15_456A
loc_15_452C:
        pop    ecx
loc_15_452D:
        inc    ecx
        cmp    ecx, dword ptr [edi + 0x5b4]
        jb     loc_15_44E5
        pop    esi
        ret    
sub_15_4538:
        cmp    byte ptr [edi + 0x60e], 0
        je     loc_15_4569
        pushal 
        call   sub_15_3A50
        mov    esi, dword ptr [edi + 0x5b8]
        xor    edx, edx
loc_15_454F:
        mov    eax, dword ptr [esi + edx*4]
        or     eax, eax
        je     loc_15_455F
        and    byte ptr [eax + 3], 0xbf
        call   sub_15_2D96
loc_15_455F:
        inc    edx
        cmp    edx, dword ptr [edi + 0x5b4]
        jb     loc_15_454F
        popal  
loc_15_4569:
        ret    
sub_15_456A:
        push   eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [edi + 0x5e8], eax
        pop    eax
        bts    dword ptr [edi + 0x5c0], eax
        ret    
sub_15_4580:
        pushal 
        movzx  edx, word ptr [eax + 4]
        mov    ecx, edx
        shr    edx, 1
        je     loc_15_45C7
        movzx  ecx, byte ptr [edi + edx + 0x573]
        jecxz  loc_15_4597
        jmp    loc_15_45C4
loc_15_4597:
        lea    ebx, [edi + 0x574]
        mov    ecx, 7
        mov    ebp, 0xfe
loc_15_45A7:
        movzx  edx, byte ptr [ebx]
        shr    edx, 1
        btr    ebp, edx
        inc    ebx
        loop   loc_15_45A7
        bsf    ecx, ebp
        shl    ecx, 1
        movzx  edx, word ptr [eax + 4]
        shr    edx, 1
        mov    byte ptr [edi + edx + 0x573], cl
loc_15_45C4:
        adc    ecx, 0
loc_15_45C7:
        mov    edx, ecx
        call   sub_15_437C
        jb     loc_15_4604
        mov    edi, ecx
        shl    edi, 0xc
        add    edi, dword ptr [esi + 4]
        mov    esi, dword ptr [esp + 0x20]
        shl    esi, 0xc
        sub    esi, 0xa0000
        add    esi, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    ecx, 0x1000
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_15_4604:
        popal  
        ret    
loc_15_4606:
        pushal 
        movzx  edx, word ptr [eax + 4]
        mov    eax, edx
        shr    eax, 1
        je     loc_15_4626
        movzx  edx, byte ptr [edi + eax + 0x573]
        mov    eax, edx
        adc    edx, 0
        or     eax, eax
        je     loc_15_4489
loc_15_4626:
        call   sub_15_437C
        jb     loc_15_4489
        shl    edx, 0xc
        add    edx, dword ptr [esi + 4]
        shl    ecx, 0xc
        sub    ecx, 0xa0000
        add    ecx, dword ptr [0xec8]        ; -> obj1:0x0EC8
        mov    edi, ecx
        mov    esi, edx
        mov    ecx, 0x1000
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        popal  
        ret    
        db  000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h
        dd  obj1:0x0FE0              ; fixup
loc_15_4670:
        pushal 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 1
        je     loc_15_469D
        call   sub_15_4780
        and    byte ptr [edi], 0xfb
        test   byte ptr [edi + 1], 8
        je     loc_15_4698
        or     byte ptr [edi + 1], 0x10
        jmp    loc_15_469D
loc_15_4698:
        call   sub_15_46DC
loc_15_469D:
        popal  
        jmp    dword ptr [0xfe0]             ; -> obj1:0x0FE0
loc_15_46A4:
        mov    edi, edx
        mov    dword ptr [edi + 0x5e4], 0
        test   byte ptr [edi], 2
        jne    loc_15_46D8
        and    byte ptr [edi], 0xfb
        test   byte ptr [edi], 1
        je     loc_15_46D8
        xor    eax, eax
        xchg   byte ptr [edi + 0x60d], al
        btr    dword ptr [edi], 0x1a
        jae    sub_15_46DC
        cmp    al, 8
        jae    sub_15_46DC
        inc    eax
        mov    byte ptr [edi + 0x60d], al
        jmp    sub_15_4740
loc_15_46D8:
        ret    
        db  02Eh, 08Bh, 0C0h
sub_15_46DC:
        mov    word ptr [edi + 0x5cc], 0
        bt     dword ptr [edi], 0xb
        jb     loc_15_4737
        call   sub_15_1C57
        jb     loc_15_46F9
        call   sub_15_01E7
        jae    loc_15_4738
loc_15_46F9:
        or     byte ptr [edi + 1], 8
        inc    byte ptr [edi + 0x604]
        push   ebx
        cmp    byte ptr [edi + 0x9d], 0xd
        jae    loc_15_471F
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        mov    dword ptr [edi + 0x5ec], eax
loc_15_471F:
        mov    eax, 0x10
        mov    ecx, 0xb
        mov    edx, edi
        mov    esi, 0x478f                   ; -> loc_15_478F
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        pop    ebx
loc_15_4737:
        ret    
loc_15_4738:
        mov    byte ptr [edi + 0x604], 0
        nop    
sub_15_4740:
        test   byte ptr [edi], 1
        je     loc_15_477C
        cmp    dword ptr [edi + 0x5e4], 0
        jne    loc_15_477C
        mov    eax, dword ptr [0xfd8]        ; -> loc_1_0FD8
        test   byte ptr [edi], 4
        jne    loc_15_4769
        mov    eax, dword ptr [0xfd4]        ; -> loc_1_0FD4
        cmp    byte ptr [edi + 0x604], 3
        jb     loc_15_4769
        shl    eax, 2
loc_15_4769:
        mov    edx, edi
        mov    esi, 0x46a4                   ; -> loc_15_46A4
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        mov    dword ptr [edi + 0x5e4], esi
loc_15_477C:
        ret    
        db  02Eh, 08Bh, 0C0h
sub_15_4780:
        xor    esi, esi
        xchg   dword ptr [edi + 0x5e4], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        ret    
loc_15_478F:
        mov    ebx, edx
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_15_47C1
        mov    esi, 0x47c2                   ; -> loc_15_47C2
        mov    ecx, 0x41a
        mov    eax, 0
        btr    dword ptr [edx], 0x1d
        jae    loc_15_47B9
        or     eax, 0x200000
loc_15_47B9:
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_15_47C2
loc_15_47C1:
        ret    
loc_15_47C2:
        mov    ebx, edx
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_15_47E7
        and    byte ptr [edx + 1], 0xf7
        mov    edi, edx
        btr    dword ptr [edi], 0xc
        jae    sub_15_4740
        jmp    sub_15_46DC
loc_15_47E7:
        ret    
sub_15_47E8:
        bts    dword ptr [edi], 0xa
        jb     loc_15_480D
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0x10
        mov    ecx, 0xb
        mov    edx, edi
        mov    esi, 0x480e                   ; -> loc_15_480E
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        pop    ebx
loc_15_480D:
        ret    
loc_15_480E:
        mov    ebx, edx
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_15_4835
        mov    esi, 0x4836                   ; -> loc_15_4836
        mov    ecx, 0x41a
        mov    eax, 0x1000a0
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_15_4836
loc_15_4835:
        ret    
loc_15_4836:
        and    byte ptr [edx + 1], 0xfb
        mov    edi, edx
        mov    ebx, edx
        sub    ebx, dword ptr [0x6cc]        ; -> obj1:0x06CC
        jmp    sub_15_016E
sub_15_4849:
        test   byte ptr [edi + 2], 2
        je     loc_15_4876
        btr    dword ptr [edi], 7
        jae    loc_15_4876
        cmp    dword ptr [edi + 0x5d4], 0
        jne    loc_15_4876
        xor    eax, eax
        mov    ecx, 3
        mov    esi, 0x4905                   ; -> sub_15_4905
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edi + 0x5d4], esi
loc_15_4876:
        ret    
sub_15_4877:
        test   byte ptr [edi + 2], 0x40
        je     loc_15_4882
        call   sub_15_498E
loc_15_4882:
        ret    
sub_15_4883:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        mov    dword ptr [edi + 0x5d4], 0
        mov    esi, dword ptr [edi + 0x5dc]
        or     esi, esi
        je     loc_15_48AC
        push   edi
        lea    edi, [edi + 0xc]
        mov    ecx, 0x25
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
loc_15_48AC:
        mov    eax, dword ptr [0xefc]        ; -> loc_1_0EFC
        or     eax, eax
        je     loc_15_48BF
        mov    cx, word ptr [0xfb2]          ; -> loc_1_0FB2
        or     word ptr [eax], cx
loc_15_48BF:
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4001
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [edi], 0x80
        je     loc_15_495E
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        ret    
sub_15_4905:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_4938
        test   byte ptr [0x7a2], 5           ; -> loc_1_07A2
        je     loc_15_4938
        xor    eax, eax
        mov    ecx, 0xb
        mov    esi, 0x4905                   ; -> sub_15_4905
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edi + 0x5d4], esi
        jmp    loc_15_498D
loc_15_4938:
        mov    dword ptr [edi + 0x5d4], 0
        test   byte ptr [edi], 0x80
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_15_495E:
        mov    word ptr [ebp + 0x1c], 0x4002
        mov    eax, 0x2f
        or     byte ptr [edi + 3], 1
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        and    byte ptr [edi + 3], 0xfe
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
loc_15_498D:
        ret    
sub_15_498E:
        cmp    dword ptr [edi + 0x5d4], 0
        je     loc_15_49A8
        test   byte ptr [edi], 0x80
        jne    loc_15_49C4
        or     byte ptr [0x6c4], 0x10        ; -> obj1:0x06C4
        or     byte ptr [edi], 0x80
        jmp    loc_15_49C4
loc_15_49A8:
        or     byte ptr [0x6c4], 0x10        ; -> obj1:0x06C4
        or     byte ptr [edi], 0x80
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_15_49C5
        test   byte ptr [ebx], 8
        jne    loc_15_49C5
        call   sub_15_4883
loc_15_49C4:
        ret    
loc_15_49C5:
        xor    eax, eax
        mov    ecx, 3
        mov    esi, 0x4883                   ; -> sub_15_4883
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edi + 0x5d4], esi
        ret    
sub_15_49DE:
        test   byte ptr [edi + 4], 3
        je     loc_15_4A17
        push   edi
        push   ebx
loc_15_49E6:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        je     loc_15_4A15
        test   byte ptr [ebx], 2
        je     loc_15_49E6
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 0x80
        jne    loc_15_49E6
        test   byte ptr [edi + 2], 0x40
        je     loc_15_49E6
        call   sub_15_4538
        call   sub_15_498E
        jmp    loc_15_49E6
loc_15_4A15:
        pop    ebx
        pop    edi
loc_15_4A17:
        ret    
sub_15_4A18:
        call   sub_15_2962
        jae    loc_15_4A7E
        push   edi
        push   ebx
        test   byte ptr [edi + 4], 0x80
        jne    loc_15_4A7B
loc_15_4A27:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        je     loc_15_4A7B
        test   byte ptr [ebx], 2
        je     loc_15_4A27
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 0x80
        jne    loc_15_4A27
        test   byte ptr [edi + 2], 2
        je     loc_15_4A57
        test   byte ptr [edi + 4], 0x10
        jne    loc_15_4A27
        call   sub_15_498E
        jmp    loc_15_4A27
loc_15_4A57:
        call   sub_15_3A9D
        jae    loc_15_4A27
        VMMCall  VMM.Suspend_VM                  ; cd202b000100
        jb     loc_15_4A27
        or     byte ptr [edi], 0x80
        or     byte ptr [edi + 9], 4
        or     byte ptr [0x6c4], 0x10        ; -> obj1:0x06C4
        call   sub_9_051B
        jmp    loc_15_4A27
loc_15_4A7B:
        pop    ebx
        pop    edi
        ret    
loc_15_4A7E:
        btr    dword ptr [0x6c4], 4          ; -> obj1:0x06C4
        jae    loc_15_4B2B
        push   edi
        push   ebx
        jmp    loc_15_4A9F
loc_15_4A90:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        je     loc_15_4B29
loc_15_4A9F:
        cmp    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        je     loc_15_4AAC
        test   byte ptr [ebx], 2
        je     loc_15_4A90
loc_15_4AAC:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 0x80
        je     loc_15_4A90
        test   byte ptr [edi + 2], 2
        je     loc_15_4B03
        cmp    ebx, dword ptr [0x7a4]        ; -> loc_1_07A4
        je     loc_15_4AD6
        test   byte ptr [edi + 2], 0x40
        je     loc_15_4AD6
loc_15_4ACD:
        or     byte ptr [0x6c4], 0x10        ; -> obj1:0x06C4
        jmp    loc_15_4A90
loc_15_4AD6:
        push   eax
        call   sub_15_3E44
        pop    eax
        and    byte ptr [edi], 0x7f
        cmp    dword ptr [edi + 0x5d4], 0
        jne    loc_15_4A90
        xor    eax, eax
        mov    ecx, 3
        mov    esi, 0x4905                   ; -> sub_15_4905
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edi + 0x5d4], esi
        jmp    loc_15_4A90
loc_15_4B03:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        je     loc_15_4B1B
        push   edi
        mov    edi, dword ptr [0xed2]        ; -> loc_1_0ED2
        cmp    edi, dword ptr [0xecc]        ; -> obj1:0x0ECC
        pop    edi
        jbe    loc_15_4ACD
loc_15_4B1B:
        and    byte ptr [edi], 0x7f
        VMMCall  VMM.No_Fail_Resume_VM           ; cd202d000100
        jmp    loc_15_4A90
loc_15_4B29:
        pop    ebx
        pop    edi
loc_15_4B2B:
        ret    
sub_15_4B2C:
        test   byte ptr [edi], 0x80
        je     loc_15_4B44
        test   byte ptr [edi + 2], 2
        je     loc_15_4B44
        call   sub_15_3E44
        and    byte ptr [edi], 0x7f
        call   sub_15_4905
loc_15_4B44:
        ret    
sub_15_4B45:
        push   edi
        push   ebx
loc_15_4B47:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        je     loc_15_4BB6
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        test   byte ptr [edi], 0x80
        jne    loc_15_4B47
        test   byte ptr [edi + 2], 2
        je     loc_15_4B72
        test   byte ptr [edi + 4], 0x10
        jne    loc_15_4B47
        call   sub_15_498E
        jmp    loc_15_4B47
loc_15_4B72:
        test   byte ptr [edi + 1], 0x20
        jne    loc_15_4B8A
        cmp    byte ptr [edi + 0x5b3], 0x56
        je     loc_15_4B8A
        cmp    byte ptr [edi + 0x5b3], 0x58
        jne    loc_15_4B47
loc_15_4B8A:
        VMMCall  VMM.Suspend_VM                  ; cd202b000100
        jb     loc_15_4B47
        or     byte ptr [edi], 0x80
        or     byte ptr [edi + 9], 4
        or     byte ptr [0x6c4], 0x10        ; -> obj1:0x06C4
        test   byte ptr [edi + 1], 0x20
        jne    loc_15_4B47
        cmp    byte ptr [edi + 0x5b3], 0x56
        je     loc_15_4B47
        call   sub_9_051B
        jmp    loc_15_4B47
loc_15_4BB6:
        pop    ebx
        pop    edi
        ret    
        db  000h, 000h, 000h
sub_15_4BBC:
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4C23
        test   byte ptr [edi], 0x12
        jne    loc_15_4C23
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4C23
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_4C23
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_4C23
        test   byte ptr [edi + 2], 2
        jne    loc_15_4C3D
        test   byte ptr [edi + 0x5e2], 0xe
        je     loc_15_4C23
        test   byte ptr [edi + 0x7c], 1
        jne    loc_15_4C03
        test   byte ptr [edi + 0x5e2], 2
        jne    loc_15_4C3D
        jmp    loc_15_4C23
loc_15_4C03:
        test   byte ptr [edi + 0x6a], 4
        je     loc_15_4C0F
        test   byte ptr [edi + 0x23], 1
        jne    loc_15_4C1A
loc_15_4C0F:
        test   byte ptr [edi + 0x5e2], 4
        jne    loc_15_4C3D
        jmp    loc_15_4C23
loc_15_4C1A:
        test   byte ptr [edi + 0x5e2], 8
        jne    loc_15_4C3D
loc_15_4C23:
        or     byte ptr [edi + 2], 1
        call   sub_9_05F8
        mov    edx, 0x3c0
loc_15_4C31:
        call   sub_15_4C7C
        inc    edx
        cmp    dl, 0xe0
        jb     loc_15_4C31
        ret    
loc_15_4C3D:
        btr    dword ptr [edi], 0x10
        jae    loc_15_4C7B
        mov    edx, 0x3c0
        mov    dword ptr [edi + 0x4a7], 0
loc_15_4C52:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    edx
        cmp    dl, 0xe0
        jb     loc_15_4C52
        btr    dword ptr [edi], 0x17
        jae    loc_15_4C7B
        mov    dl, 0xb4
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    edx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    dl, 0xba
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_15_4C7B:
        ret    
sub_15_4C7C:
        push   edx
        and    edx, 0x1f
        bt     dword ptr [0x1014], edx       ; -> loc_1_1014
        jb     loc_15_4C99
loc_15_4C89:
        bts    dword ptr [edi + 0x4a7], edx
        jb     loc_15_4CAF
        pop    edx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098800100
loc_15_4C99:
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4C89
        test   byte ptr [edi], 0x12
        jne    loc_15_4C89
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        jne    loc_15_4C89
loc_15_4CAF:
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
loc_15_4CB4:
        mov    edi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        add    edi, ebx
        cmp    ecx, 4
        jbe    loc_15_4CC7
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_15_4CC7:
        clc    
        pushfd 
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4CDE
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4CDE
        or     byte ptr [esp], 1
loc_15_4CDE:
        popfd  
        pushfd 
        jne    loc_15_4CF4
        test   byte ptr [edi], 0x80
        jne    loc_15_4D0B
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4CF4
        call   sub_15_119E
loc_15_4CF4:
        mov    esi, edx
        sub    esi, 0x3c0
        jns    loc_15_4D03
        mov    esi, 0x20
loc_15_4D03:
        popfd  
        jmp    dword ptr [esi*4 + 0x1018]    ; -> obj1:0x1018
loc_15_4D0B:
        cmp    edx, 0x3d4
        je     loc_15_4CF4
        cmp    edx, 0x3b4
        je     loc_15_4CF4
        popfd  
        ret    
loc_15_4D1D:
        pushfd 
        mov    esi, 0x14
        cmp    dl, 0xb4
        je     loc_15_4D3C
        mov    esi, 0x15
        cmp    dl, 0xb5
        je     loc_15_4D3C
        mov    esi, 0x1a
        cmp    dl, 0xba
        jne    loc_15_4D44
loc_15_4D3C:
        popfd  
        jmp    dword ptr [esi*4 + 0x1018]    ; -> obj1:0x1018
loc_15_4D44:
        popfd  
        mov    al, 0xff
        ret    
loc_15_4D48:
        in     al, dx
        ret    
        db  08Bh, 0C0h
loc_15_4D4C:
        jne    loc_15_4D48
        out    dx, al
        ret    
loc_15_4D50:
        jne    loc_15_4D6B
        jae    loc_15_4D79
        test   byte ptr [edi + 0x4c], 0x80
        je     loc_15_4D78
        test   byte ptr [0x6c4], 2           ; -> obj1:0x06C4
        je     loc_15_4D78
        mov    byte ptr [edi + 0x4c], al
        and    al, 0xdf
        out    dx, al
        jmp    loc_15_4DAC
loc_15_4D6B:
        mov    al, byte ptr [edi + 0x4c]
        and    al, 0x7f
        ret    
        db  02Eh, 08Bh, 0C0h
loc_15_4D74:
        jne    loc_15_4DAD
        jae    loc_15_4D79
loc_15_4D78:
        out    dx, al
loc_15_4D79:
        test   byte ptr [edi + 0x4c], 0x80
        jne    loc_15_4DA7
        movzx  ecx, byte ptr [edi + 0x4c]
        test   cl, 0x10
        jne    loc_15_4D8D
        test   cl, 0x20
        jne    loc_15_4DA1
loc_15_4D8D:
        and    cl, 0xdf
        cmp    cl, 0x15
        ja     loc_15_4DA1
        cmp    byte ptr [edi + 0x4c], 0xf
        ja     loc_15_4D9D
        and    al, 0x3f
loc_15_4D9D:
        mov    byte ptr [ecx + edi + 0x50], al
loc_15_4DA1:
        or     byte ptr [edi + 0x4c], 0x80
        jmp    loc_15_4DAC
loc_15_4DA7:
        and    al, 0x7f
        mov    byte ptr [edi + 0x4c], al
loc_15_4DAC:
        ret    
loc_15_4DAD:
        movzx  eax, byte ptr [edi + 0x4c]
        and    al, 0x1f
        cmp    al, 0x14
        ja     loc_15_4DBC
        mov    al, byte ptr [eax + edi + 0x50]
        ret    
loc_15_4DBC:
        mov    al, 0xff
        ret    
        db  090h
loc_15_4DC0:
        jne    loc_15_4E09
        push   eax
        jb     loc_15_4DF1
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_4DF2
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4DF2
        mov    esi, dword ptr [0x7a8]        ; -> loc_1_07A8
        add    esi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        mov    ah, byte ptr [esi + 0x96]
        mov    ecx, 0xdd22
        and    eax, ecx
        or     al, ah
loc_15_4DF1:
        out    dx, al
loc_15_4DF2:
        pop    eax
        mov    byte ptr [edi + 0x96], al
        test   byte ptr [edi + 4], 4
        jne    loc_15_4E08
        test   al, 1
        jne    loc_15_4E08
        call   sub_9_05F8
loc_15_4E08:
        ret    
loc_15_4E09:
        jae    loc_15_4E0D
        in     al, dx
        ret    
loc_15_4E0D:
        mov    al, byte ptr [edi + 0x9b]
        or     al, al
        jne    loc_15_4E18
        in     al, dx
loc_15_4E18:
        xor    al, 9
        mov    byte ptr [edi + 0x9b], al
        ret    
        db  02Eh, 08Bh, 0C0h
loc_15_4E24:
        jne    loc_15_4E3C
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_4E38
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4E38
        out    dx, al
loc_15_4E38:
        mov    byte ptr [edi + 0x4d], al
        ret    
loc_15_4E3C:
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_4E4F
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4E4F
        in     al, dx
        ret    
loc_15_4E4F:
        mov    al, byte ptr [edi + 0x4d]
        ret    
        db  090h
loc_15_4E54:
        movzx  ecx, byte ptr [edi + 0x4d]
        jne    loc_15_4EFC
        jae    loc_15_4E8D
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x4c], 0
        je     loc_15_4E75
        popfd  
        call   dword ptr [edi + 0x4c]
        jmp    loc_15_4E76
loc_15_4E75:
        popfd  
loc_15_4E76:
        pop    edi
        jae    loc_15_4E7A
        ret    
loc_15_4E7A:
        push   eax
        cmp    cl, 0xf
        ja     loc_15_4E8A
        xchg   dword ptr [esp], eax
        mov    byte ptr [ecx + edi + 0x66], al
        xchg   dword ptr [esp], eax
loc_15_4E8A:
        out    dx, al
loc_15_4E8B:
        pop    eax
        ret    
loc_15_4E8D:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x4c], 0
        je     loc_15_4EA2
        popfd  
        call   dword ptr [edi + 0x4c]
        jmp    loc_15_4EA3
loc_15_4EA2:
        popfd  
loc_15_4EA3:
        pop    edi
        jb     loc_15_4EFB
        push   eax
        cmp    cl, 0xf
        ja     loc_15_4E8B
        cmp    byte ptr [ecx + edi + 0x66], al
        je     loc_15_4E8B
        mov    esi, dword ptr [0x7a8]        ; -> loc_1_07A8
        add    esi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        mov    ah, byte ptr [ecx + esi + 0x66]
        mov    esi, ebx
        mov    bl, byte ptr [ecx + 0xfe4]    ; -> obj1:0x0FE4
        or     bl, bl
        je     loc_15_4EF4
        and    al, bl
        not    bl
        and    ah, bl
        or     al, ah
        mov    ah, al
        xor    ah, byte ptr [ecx + edi + 0x66]
        not    bl
        test   bl, ah
        je     loc_15_4EF4
        cmp    esi, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_4EF4
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4EF4
        out    dx, al
loc_15_4EF4:
        mov    ebx, esi
        pop    eax
        mov    byte ptr [ecx + edi + 0x66], al
loc_15_4EFB:
        ret    
loc_15_4EFC:
        jae    loc_15_4F08
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_4F08
        in     al, dx
loc_15_4F07:
        ret    
loc_15_4F08:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x48], 0
        je     loc_15_4F1D
        popfd  
        call   dword ptr [edi + 0x48]
        jmp    loc_15_4F1E
loc_15_4F1D:
        popfd  
loc_15_4F1E:
        pop    edi
        jb     loc_15_4F07
        mov    al, byte ptr [ecx + edi + 0x66]
        jmp    loc_15_4F07
        db  090h
loc_15_4F28:
        jne    loc_15_4F57
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x78], 0
        je     loc_15_4F3F
        popfd  
        call   dword ptr [edi + 0x78]
        jmp    loc_15_4F40
loc_15_4F3F:
        popfd  
loc_15_4F40:
        pop    edi
        jb     loc_15_4F71
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4F71
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4F71
        out    dx, al
        jmp    loc_15_4F71
loc_15_4F57:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_15_4F6C
        popfd  
        call   dword ptr [edi + 0x7c]
        jmp    loc_15_4F6D
loc_15_4F6C:
        popfd  
loc_15_4F6D:
        pop    edi
        jb     loc_15_4F71
        in     al, dx
loc_15_4F71:
        ret    
        db  08Bh, 0C0h
loc_15_4F74:
        jne    loc_15_4FC3
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x78], 0
        je     loc_15_4F8B
        popfd  
        call   dword ptr [edi + 0x78]
        jmp    loc_15_4F8C
loc_15_4F8B:
        popfd  
loc_15_4F8C:
        pop    edi
        jb     loc_15_4FF5
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4FA1
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4FA1
        out    dx, al
loc_15_4FA1:
        movzx  edx, al
        shl    edx, 2
        lea    edx, [edx + edi + 0xa0]
        mov    dword ptr [edi + 0x4a0], edx
        mov    byte ptr [edi + 0x4a4], 1
        mov    byte ptr [edi + 0x4a5], 0
        ret    
loc_15_4FC3:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_15_4FD8
        popfd  
        call   dword ptr [edi + 0x7c]
        jmp    loc_15_4FD9
loc_15_4FD8:
        popfd  
loc_15_4FD9:
        pop    edi
        jb     loc_15_4FF5
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_4FEF
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_4FEF
        in     al, dx
        ret    
loc_15_4FEF:
        mov    al, byte ptr [edi + 0x4a5]
loc_15_4FF5:
        ret    
        db  08Bh, 0C0h
loc_15_4FF8:
        jne    loc_15_5047
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x78], 0
        je     loc_15_500F
        popfd  
        call   dword ptr [edi + 0x78]
        jmp    loc_15_5010
loc_15_500F:
        popfd  
loc_15_5010:
        pop    edi
        jb     loc_15_5087
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_5025
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_5025
        out    dx, al
loc_15_5025:
        movzx  edx, al
        shl    edx, 2
        lea    edx, [edx + edi + 0xa0]
        mov    dword ptr [edi + 0x4a0], edx
        mov    byte ptr [edi + 0x4a4], 1
        mov    byte ptr [edi + 0x4a5], 3
        ret    
loc_15_5047:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_15_505C
        popfd  
        call   dword ptr [edi + 0x7c]
        jmp    loc_15_505D
loc_15_505C:
        popfd  
loc_15_505D:
        pop    edi
        jb     loc_15_5087
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_5073
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_5073
        in     al, dx
        ret    
loc_15_5073:
        mov    edx, dword ptr [edi + 0x4a0]
        lea    eax, [edi + 0xa0]
        cmp    edx, eax
        mov    al, 0
        jbe    loc_15_5087
        mov    al, byte ptr [edx]
loc_15_5087:
        ret    
loc_15_5088:
        jne    loc_15_50D8
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x78], 0
        je     loc_15_509F
        popfd  
        call   dword ptr [edi + 0x78]
        jmp    loc_15_50A0
loc_15_509F:
        popfd  
loc_15_50A0:
        pop    edi
        jb     loc_15_513D
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_50B9
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_50B9
        out    dx, al
loc_15_50B9:
        mov    esi, dword ptr [edi + 0x4a0]
        movzx  edx, byte ptr [edi + 0x4a4]
        cmp    byte ptr [edi + 0x4a5], 0
        je     loc_15_5114
        or     byte ptr [edi + 3], 0x10
        mov    byte ptr [edx + esi], al
        jmp    loc_15_5114
loc_15_50D8:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_15_50ED
        popfd  
        call   dword ptr [edi + 0x7c]
        jmp    loc_15_50EE
loc_15_50ED:
        popfd  
loc_15_50EE:
        pop    edi
        jb     loc_15_513D
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_5104
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_5104
        in     al, dx
        ret    
loc_15_5104:
        mov    esi, dword ptr [edi + 0x4a0]
        movzx  edx, byte ptr [edi + 0x4a4]
        mov    al, byte ptr [edx + esi]
loc_15_5114:
        cmp    edx, 3
        jb     loc_15_5136
        xor    edx, edx
        add    esi, 4
        push   eax
        lea    eax, [edi + 0x4a0]
        cmp    esi, eax
        pop    eax
        jb     loc_15_5130
        lea    esi, [edi + 0xa0]
loc_15_5130:
        mov    dword ptr [edi + 0x4a0], esi
loc_15_5136:
        inc    edx
        mov    byte ptr [edi + 0x4a4], dl
loc_15_513D:
        ret    
        db  08Bh, 0C0h
loc_15_5140:
        jne    loc_15_4D48
        jae    loc_15_5149
        out    dx, al
loc_15_5149:
        ret    
        db  08Bh, 0C0h
loc_15_514C:
        jne    loc_15_5152
        jae    loc_15_5151
        out    dx, al
loc_15_5151:
        ret    
loc_15_5152:
        jae    loc_15_5156
        in     al, dx
        ret    
loc_15_5156:
        mov    al, byte ptr [edi + 0x96]
        ret    
        db  02Eh, 08Bh, 0C0h
loc_15_5160:
        pushfd 
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_5186
        test   dword ptr [0x109c], 0x80000000 ; -> obj1:0x109C
        je     loc_15_5186
        test   dword ptr [0x109c], 1         ; -> obj1:0x109C
        je     loc_15_5186
        call   sub_15_079E
loc_15_5186:
        popfd  
        jne    loc_15_519F
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_519B
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_519B
        out    dx, al
loc_15_519B:
        mov    byte ptr [edi + 0x4f], al
        ret    
loc_15_519F:
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_51B2
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_51B2
        in     al, dx
        ret    
loc_15_51B2:
        mov    al, byte ptr [edi + 0x4f]
        ret    
        db  08Bh, 0C0h
loc_15_51B8:
        pushfd 
        cmp    ebx, dword ptr [0xef0]        ; -> loc_1_0EF0
        jne    loc_15_51DE
        test   dword ptr [0x109c], 0x80000000 ; -> obj1:0x109C
        je     loc_15_51DE
        test   dword ptr [0x109c], 1         ; -> obj1:0x109C
        je     loc_15_51DE
        call   sub_15_079E
loc_15_51DE:
        popfd  
        movzx  ecx, byte ptr [edi + 0x4f]
        jne    loc_15_52E1
        push   eax
        jae    loc_15_5245
        cmp    ecx, 6
        jne    loc_15_51FD
        test   dword ptr [edi], 0x4000
        je     loc_15_51FD
        and    al, 0xf3
        or     al, 4
loc_15_51FD:
        pop    eax
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x54], 0
        je     loc_15_5213
        popfd  
        call   dword ptr [edi + 0x54]
        jmp    loc_15_5214
loc_15_5213:
        popfd  
loc_15_5214:
        pop    edi
        jb     loc_15_5244
        push   eax
        cmp    cl, 0x1f
        ja     loc_15_5242
        xchg   dword ptr [esp], eax
        cmp    cl, 6
        jne    loc_15_523B
        mov    esi, eax
        xor    al, byte ptr [edi + 0x7c]
        test   al, 0xc
        mov    eax, esi
        je     loc_15_523B
        push   ecx
        push   edx
        push   eax
        call   sub_15_3A50
        pop    eax
        pop    edx
        pop    ecx
loc_15_523B:
        mov    byte ptr [ecx + edi + 0x76], al
        xchg   dword ptr [esp], eax
loc_15_5242:
        out    dx, al
loc_15_5243:
        pop    eax
loc_15_5244:
        ret    
loc_15_5245:
        pop    eax
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x54], 0
        je     loc_15_525B
        popfd  
        call   dword ptr [edi + 0x54]
        jmp    loc_15_525C
loc_15_525B:
        popfd  
loc_15_525C:
        pop    edi
        jb     loc_15_5244
        push   eax
        cmp    cl, 0x1f
        ja     loc_15_5243
        cmp    byte ptr [ecx + edi + 0x76], al
        je     loc_15_5243
        mov    esi, dword ptr [0x7a8]        ; -> loc_1_07A8
        add    esi, dword ptr [0x6cc]        ; -> obj1:0x06CC
        mov    ah, byte ptr [ecx + esi + 0x76]
        mov    esi, ebx
        mov    bl, byte ptr [ecx + 0xff4]    ; -> obj1:0x0FF4
        or     bl, bl
        je     loc_15_52DA
        and    al, bl
        not    bl
        and    ah, bl
        or     al, ah
        mov    ah, al
        xor    ah, byte ptr [ecx + edi + 0x76]
        not    bl
        test   bl, ah
        mov    ebx, esi
        je     loc_15_52DA
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_52C0
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        je     loc_15_52C0
        cmp    ecx, 6
        jne    loc_15_52BF
        test   dword ptr [edi], 0x4000
        je     loc_15_52BF
        and    al, 0xf3
        or     al, 4
loc_15_52BF:
        out    dx, al
loc_15_52C0:
        mov    al, byte ptr [esp]
        cmp    cl, 6
        jne    loc_15_52D4
        test   ah, 0xc
        je     loc_15_52D4
        push   ecx
        call   sub_15_3A50
        pop    ecx
loc_15_52D4:
        pop    eax
        mov    byte ptr [ecx + edi + 0x76], al
loc_15_52D9:
        ret    
loc_15_52DA:
        pop    eax
        mov    byte ptr [ecx + edi + 0x76], al
        jmp    loc_15_52D9
loc_15_52E1:
        jae    loc_15_52ED
        cmp    ebx, dword ptr [0x7ac]        ; -> loc_1_07AC
        jne    loc_15_52ED
        in     al, dx
loc_15_52EC:
        ret    
loc_15_52ED:
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x50], 0
        je     loc_15_5302
        popfd  
        call   dword ptr [edi + 0x50]
        jmp    loc_15_5303
loc_15_5302:
        popfd  
loc_15_5303:
        pop    edi
        jb     loc_15_52EC
        mov    al, byte ptr [ecx + edi + 0x76]
        jmp    loc_15_52EC
loc_15_530C:
        jne    loc_15_5315
        jae    loc_15_5311
        out    dx, al
loc_15_5311:
        mov    byte ptr [edi + 0x4e], al
        ret    
loc_15_5315:
        jae    loc_15_5319
        in     al, dx
        ret    
loc_15_5319:
        mov    al, byte ptr [edi + 0x4e]
        ret    
        db  02Eh, 08Bh, 0C0h
loc_15_5320:
        jne    loc_15_5383
        movzx  ecx, byte ptr [edi + 0x4e]
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x44], 0
        je     loc_15_533B
        popfd  
        call   dword ptr [edi + 0x44]
        jmp    loc_15_533C
loc_15_533B:
        popfd  
loc_15_533C:
        pop    edi
        jb     loc_15_5382
        cmp    ebx, dword ptr [0x7a8]        ; -> loc_1_07A8
        jne    loc_15_5379
        test   byte ptr [0x6c4], 1           ; -> obj1:0x06C4
        jne    loc_15_535F
        cmp    cl, 0x18
        jbe    loc_15_5379
        dec    dl
        mov    ah, al
        mov    al, cl
        out    dx, ax
        jmp    loc_15_5382
loc_15_535F:
        cmp    cl, 0xc
        jb     loc_15_5378
        cmp    cl, 0xd
        ja     loc_15_5378
        cmp    byte ptr [ecx + edi + 0xc], al
        je     loc_15_5378
        mov    byte ptr [ecx + edi + 0xc], al
        call   sub_15_396E
loc_15_5378:
        out    dx, al
loc_15_5379:
        cmp    cl, 0x18
        ja     loc_15_5382
        mov    byte ptr [ecx + edi + 0xc], al
loc_15_5382:
        ret    
loc_15_5383:
        jae    loc_15_5387
        in     al, dx
        ret    
loc_15_5387:
        movzx  ecx, byte ptr [edi + 0x4e]
        cmp    eax, eax
        push   edi
        pushfd 
        mov    edi, 0x6d0                    ; -> obj1:0x06D0
        cmp    dword ptr [edi + 0x40], 0
        je     loc_15_53A0
        popfd  
        call   dword ptr [edi + 0x40]
        jmp    loc_15_53A1
loc_15_53A0:
        popfd  
loc_15_53A1:
        pop    edi
        jb     loc_15_53B7
        cmp    cl, 0x18
        jbe    loc_15_53B3
        dec    dl
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        jmp    loc_15_53B7
loc_15_53B3:
        mov    al, byte ptr [ecx + edi + 0xc]
loc_15_53B7:
        ret    
loc_15_53B8:
        jne    loc_15_53C4
        jae    loc_15_53BD
        out    dx, al
loc_15_53BD:
        mov    byte ptr [edi + 0x99], al
        ret    
loc_15_53C4:
        jae    loc_15_53CA
        or     byte ptr [edi + 0x4c], 0x80
loc_15_53CA:
        mov    al, byte ptr [edi + 0x9c]
        cmp    byte ptr [edi + 0x5e0], 0
        je     loc_15_53E1
        dec    byte ptr [edi + 0x5e0]
        jmp    loc_15_5403
loc_15_53E1:
        dec    byte ptr [edi + 0x5e1]
        je     loc_15_53ED
        xor    al, 9
        jmp    loc_15_53FD
loc_15_53ED:
        mov    byte ptr [edi + 0x5e0], 0xc
        mov    byte ptr [edi + 0x5e1], 0xff
        or     al, 9
loc_15_53FD:
        mov    byte ptr [edi + 0x9c], al
loc_15_5403:
        mov    cl, al
        in     al, dx
        and    al, 0xf6
        or     al, cl
        or     byte ptr [edi + 0x4c], 0x80
        ret    
        db  090h
loc_15_5410:
        jne    loc_15_4D48
        jae    loc_15_5419
        out    dx, al
loc_15_5419:
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 16  vsize=0x249  R-X
        db  081h, 0ECh, 0CCh, 000h, 000h, 000h, 056h, 057h, 08Dh, 044h, 024h, 008h, 068h, 0CCh, 000h, 000h
        db  000h, 050h, 0FFh, 0B4h, 024h, 0E0h, 000h, 000h, 000h, 0E8h
        dd  loc_5_039D               ; fixup
        db  083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 012h, 08Dh, 074h, 024h, 008h, 0BFh
        dd  obj1:0x10D4              ; fixup
        db  0B9h, 033h, 000h, 000h, 000h, 0F3h, 0A5h, 0EBh, 00Ah, 0C7h, 005h
        dd  obj1:0x10D4              ; fixup
        db  0FFh, 0FFh, 0FFh, 0FFh, 0C7h, 005h
        dd  obj1:0x10B0              ; fixup
        db  001h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh, 081h, 0C4h, 0CCh, 000h, 000h
        db  000h, 0C3h
sub_16_0059:
        sub    esp, 4
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [esp + 2], al
        mov    byte ptr [esp + 3], 0
        push   1
        lea    eax, [esp + 6]
        push   2
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   1
        push   dword ptr [eax*4 + 0x38]      ; -> obj17:0x0038
        push   dword ptr [0x28]              ; -> obj17:0x0028
        push   dword ptr [esp + 0x20]
        call   sub_14_1998
        add    esp, 0x20
        ret    
sub_16_0092:
        sub    esp, 0x18
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [esp], 0x14
        mov    al, byte ptr [ecx + 0x40]     ; -> obj17:0x0040
        mov    byte ptr [esp + 4], al
        lea    eax, [esp]
        push   1
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        push   1
        push   dword ptr [ecx*4 + 0x38]      ; -> obj17:0x0038
        push   dword ptr [0x28]              ; -> obj17:0x0028
        push   dword ptr [esp + 0x34]
        call   sub_14_1990
        movsx  eax, byte ptr [esp + 0x20]
        add    esp, 0x1c
        add    esp, 0x18
        ret    
sub_16_00DB:
        sub    esp, 0x14c
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x160]
        cmp    ebx, 2
        jb     loc_16_00F7
        sub    eax, eax
        jmp    loc_16_01F4
loc_16_00F7:
        mov    esi, dword ptr [esp + 0x15c]
        push   ebx
        push   esi
        call   sub_16_0092
        add    esp, 8
        cmp    eax, 0x30
        jne    loc_16_0114
        sub    eax, eax
        jmp    loc_16_01F4
loc_16_0114:
        lea    edi, [esp + 0x8c]
        sub    eax, eax
        mov    ecx, 0x33
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esp + 0x8c], ebx
        push   eax
        lea    eax, [esp + 0x94]
        push   0xc8
        push   eax
        push   esi
        call   sub_14_1948
        add    esp, 0x10
        push   0x44                          ; -> obj17:0x0044
        call   sub_14_18E8
        push   eax
        lea    eax, [esp + 0x14]
        push   0x50                          ; -> obj17:0x0050
        push   eax
        call   sub_14_18B4
        add    esp, 0x10
        cmp    dword ptr [0x10b0], 0         ; -> obj1:0x10B0
        jne    loc_16_0175
        lea    eax, [esp + 0xc]
        push   eax
        call   0
        add    esp, 4
loc_16_0175:
        mov    eax, dword ptr [0x10d4]       ; -> obj1:0x10D4
        cmp    dword ptr [esp + 0x8c], eax
        jne    loc_16_01D5
        mov    eax, 0x10d8                   ; -> obj1:0x10D8
        lea    edx, [esp + 0x90]
loc_16_018F:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_16_01AF
        or     cl, cl
        je     loc_16_01AB
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_16_01AF
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_16_018F
loc_16_01AB:
        sub    eax, eax
        jmp    loc_16_01B4
loc_16_01AF:
        sbb    eax, eax
        sbb    eax, -1
loc_16_01B4:
        or     eax, eax
        jne    loc_16_01D5
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_5_0362
        add    esp, 4
        push   0x30
        push   ebx
        push   esi
        call   sub_16_0059
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_16_01F4
loc_16_01D5:
        lea    eax, [esp + 0x8c]
        push   0xcc
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_5_02F4
        add    esp, 0xc
        mov    eax, 1
loc_16_01F4:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14c
        ret    
sub_16_01FE:
        sub    esp, 0x80
        cmp    dword ptr [esp + 0x88], 2
        mov    eax, 0
        jae    loc_16_0242
        push   0x58                          ; -> obj17:0x0058
        call   sub_14_18E8
        push   eax
        lea    eax, [esp + 8]
        push   0x64                          ; -> obj17:0x0064
        push   eax
        call   sub_14_18B4
        lea    eax, [esp + 0x10]
        add    esp, 0x10
        push   eax
        call   sub_5_0362
        add    esp, 4
        mov    eax, 1
loc_16_0242:
        add    esp, 0x80
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 17  vsize=0x69  RW-
        db  0E0h, 0C7h, 0B1h, 04Ch, 065h, 02Dh, 0CFh, 011h, 08Eh, 030h, 008h, 000h, 02Bh, 036h, 038h, 0A6h
        db  020h, 0AAh, 018h, 0C3h, 079h, 02Dh, 0CFh, 011h, 08Eh, 030h, 008h, 000h, 02Bh, 036h, 038h, 0A6h
        db  044h, 045h, 046h, 041h, 055h, 04Ch, 054h, 000h
        dd  obj17:0x0020             ; fixup
        db  044h, 044h, 043h, 000h, 050h, 04Fh, 053h, 054h, 000h, 000h, 000h, 000h
        dd  obj17:0x002C             ; fixup
        dd  obj17:0x0030             ; fixup
        db  032h, 032h, 000h, 000h, 056h, 044h, 044h, 041h, 053h, 044h, 02Eh, 044h, 041h, 054h, 000h, 000h
        db  025h, 073h, 025h, 073h, 000h, 000h, 000h, 000h, 056h, 044h, 044h, 041h, 053h, 044h, 02Eh, 044h
        db  041h, 054h, 000h, 000h, 025h, 073h, 025h, 073h, 021h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 18  vsize=0xa4  RW-
VxD__LDATA ENDS
