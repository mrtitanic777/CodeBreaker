; VPOWERD -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 2211 instructions recovered by recursive descent

Declare_Virtual_Device VPOWERD, 4, 0, VPOWERD_Control, 00026h, Undefined_Init_Order
;   DDK version 0400h   init_order 001000000h   flags 00000h
;   service table: obj1:0x570, 25 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x694  R-X
VPOWERD_Control:
        cmp    eax, 0x23
        je     loc_5_001D
        cmp    eax, 0
        je     loc_4_0000
        cmp    eax, 1
        je     loc_4_0013
        cmp    eax, 4
        je     sub_8_0078
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0028:
        cmp    byte ptr [0x62c], 0           ; -> loc_1_062C
        je     loc_1_0046
        cmp    byte ptr [0x690], 0           ; -> loc_1_0690
        jne    loc_1_0046
        mov    ax, 0x5305
        call   ptr [0x624]                   ; -> obj1:0x0624
        clc    
        ret    
loc_1_0046:
        stc    
        ret    
_VPOWERD_Set_System_Power_State:
        enter  0x10, 0
        push   ebx
        push   esi
        push   edi
        mov    eax, 0x80000001
        cmp    byte ptr [0x690], 0           ; -> loc_1_0690
        jne    loc_1_0239
        mov    eax, 0x80000007
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        je     loc_1_0239
        cmp    ebx, 3
        ja     loc_1_0239
        mov    eax, 0x80000008
        mov    edi, dword ptr [ebp + 0xc]
        and    edi, 0x7fffffff
        cmp    edi, 3
        ja     loc_1_0239
        je     loc_1_0127
        movzx  esi, byte ptr [edi + 0x681]   ; -> loc_1_0681
        cmp    ebx, 1
        jne    loc_1_00A6
        or     esi, 0x10
loc_1_00A6:
        mov    dword ptr [ebp - 8], 0x80000002
        mov    ebx, 0
        cmp    edi, 2
        lea    edi, [ebp - 4]
        je     loc_1_00CD
        push   edi
        push   esi
        push   ebx
        call   sub_5_0298
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_02B3
loc_1_00CD:
        inc    ebx
        push   edi
        push   esi
        push   ebx
        call   sub_5_0298
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_029E
        test   si, 0x10
        jne    loc_1_0119
        push   ebx
        push   esi
        push   edi
        mov    ah, 0xd
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        pop    esi
        pop    ebx
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        jne    loc_1_0119
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        test   byte ptr [eax + 3], 0x40
        je     loc_1_0119
        call   sub_1_0350
        or     eax, eax
        jne    loc_1_0298
loc_1_0119:
        xor    esi, esi
        xchg   dword ptr [0x638], esi        ; -> loc_1_0638
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_1_0127:
        mov    dword ptr [ebp - 0xc], 0x1000
        mov    dword ptr [ebp - 0x10], 5
        test   byte ptr [ebp + 0xf], 0x80
        je     loc_1_018A
        cmp    dword ptr [ebp + 8], 1
        je     loc_1_018A
        test   byte ptr [0x640], 2           ; -> obj1:0x0640
        jne    loc_1_0232
        call   sub_1_04FC
        jb     loc_1_018A
        cmp    ecx, 0xc
        mov    esi, 0x205                    ; -> loc_8_0205
        ja     loc_1_0169
        mov    esi, dword ptr [ecx*4 + 0x648] ; -> obj1:0x0648
loc_1_0169:
        cmp    esi, 0x1b5                    ; -> loc_5_01B5
        je     loc_1_018D
        cmp    esi, 0x243                    ; -> loc_5_0243
        je     loc_1_0247
        push   0
        push   ecx
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
loc_1_018A:
        mov    ecx, dword ptr [ebp + 8]
loc_1_018D:
        and    ecx, 7
        mov    ax, 0x5307
        mov    bx, 1
        push   ecx
        call   ptr [0x624]                   ; -> obj1:0x0624
        pop    ecx
        jb     loc_1_0271
loc_1_01A6:
        cmp    ecx, 1
        jne    loc_1_01B4
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        je     loc_1_0232
loc_1_01B4:
        dec    dword ptr [ebp - 0xc]
        je     loc_1_0232
        call   sub_1_04FC
        jb     loc_1_01B4
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, 2
        jne    loc_1_01CD
        cmp    ecx, 1
        je     loc_1_01B4
loc_1_01CD:
        cmp    ecx, 0xc
        mov    esi, 0x205                    ; -> loc_8_0205
        ja     loc_1_01E1
        mov    esi, dword ptr [ecx*4 + 0x648] ; -> obj1:0x0648
loc_1_01E1:
        cmp    esi, 0x1b5                    ; -> loc_5_01B5
        je     loc_1_021D
        cmp    esi, 0x243                    ; -> loc_5_0243
        jne    loc_1_020E
        mov    edi, dword ptr [ebp + 0xc]
        and    edi, 0x7fffffff
        cmp    edi, 3
        jne    loc_1_0247
        push   0
        push   ecx
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
        jmp    loc_1_0232
loc_1_020E:
        push   0
        push   ecx
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
        jmp    loc_1_01B4
loc_1_021D:
        dec    dword ptr [ebp - 0x10]
        jne    loc_1_018D
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_018D
loc_1_0232:
        call   sub_1_03D8
        xor    eax, eax
loc_1_0239:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        cmp    eax, 0
        jne    loc_1_032E
        ret    
loc_1_0247:
        shr    ecx, 2
        movzx  ecx, byte ptr [ecx + 0x684]   ; -> obj1:0x0684
        test   ecx, 0x10
        jne    loc_1_0268
        cmp    dword ptr [0x644], 0          ; -> obj1:0x0644
        jne    loc_1_0268
        or     ecx, 0x80
loc_1_0268:
        push   ecx
        call   sub_5_0176
        pop    ecx
        jmp    loc_1_0232
loc_1_0271:
        cmp    ah, 0x53
        je     loc_1_01A6
        movzx  eax, ah
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp + 0xc]
        and    edi, 0x7fffffff
        cmp    edi, 3
        je     loc_1_02FB
        movzx  esi, byte ptr [edi + 0x681]   ; -> loc_1_0681
        lea    edi, [ebp - 4]
loc_1_0298:
        mov    dword ptr [edi], 0x688        ; -> obj1:0x0688
loc_1_029E:
        or     esi, 0xe
        push   edi
        push   esi
        push   4
        call   sub_5_0298
        add    esp, 0xc
        mov    dword ptr [edi], 0x688        ; -> obj1:0x0688
loc_1_02B3:
        or     esi, 0xe
        push   edi
        push   esi
        push   5
        call   sub_5_0298
        add    esp, 0xc
        cmp    dword ptr [ebp - 8], 0x80000002
        jne    loc_1_02FB
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        je     loc_1_02FB
        test   byte ptr [ebp + 0xf], 0x80
        je     loc_1_02FB
        xor    esi, esi
        xchg   dword ptr [0x638], esi        ; -> loc_1_0638
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    ax, 0x5307
        mov    bx, 1
        mov    ecx, 5
        call   ptr [0x624]                   ; -> obj1:0x0624
loc_1_02FB:
        call   sub_1_03D8
        cmp    dword ptr [ebp + 8], 1
        jne    loc_1_0326
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        or     eax, eax
        jne    loc_1_0326
        mov    esi, 0xbc                     ; -> loc_8_00BC
        push   0
        push   dword ptr [ebp - 8]
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
loc_1_0326:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_0239
loc_1_032E:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, 2
        cmp    ecx, 3
        je     loc_1_0347
        mov    edx, 1
        cmp    ecx, 2
        je     loc_1_0347
        ret    
loc_1_0347:
        mov    dword ptr [esp + 4], edx
        jmp    _VPOWERD_Set_System_Power_State
sub_1_0350:
        push   ebx
        mov    ax, 0x44f1
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    eax, 0x80000002
        jb     loc_1_03D6
        mov    ax, 0x44f3
        mov    bx, 0x100
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_03C8
        or     cx, cx
        jne    loc_1_03C8
        mov    ax, 0x44f4
        mov    bx, 0x100
        mov    cx, 1
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        or     byte ptr [0x67c], 1           ; -> obj1:0x067C
        mov    ax, 0x44f3
        mov    bx, 0x10d
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_03C8
        or     cx, cx
        je     loc_1_03C8
        mov    ax, 0x44f4
        mov    bx, 0x10d
        mov    cx, 0
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        or     byte ptr [0x67c], 2           ; -> obj1:0x067C
loc_1_03C8:
        mov    ax, 0x44f2
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        xor    eax, eax
loc_1_03D6:
        pop    ebx
        ret    
sub_1_03D8:
        test   byte ptr [0x67c], 1           ; -> obj1:0x067C
        je     loc_1_0433
        mov    ax, 0x44f1
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_0433
        test   byte ptr [0x67c], 2           ; -> obj1:0x067C
        je     loc_1_040C
        mov    ax, 0x44f4
        mov    bx, 0x10d
        mov    cx, 1
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_1_040C:
        mov    ax, 0x44f4
        mov    bx, 0x100
        mov    cx, 0
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        and    byte ptr [0x67c], 0xfc        ; -> obj1:0x067C
        mov    ax, 0x44f2
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_1_0433:
        ret    
loc_1_0434:
        call   sub_1_04FC
        jb     loc_1_04B2
        cmp    ecx, 8
        je     loc_1_04E6
        cmp    ecx, 0xc
        mov    esi, 0x205                    ; -> loc_8_0205
        ja     loc_1_0458
        mov    esi, dword ptr [ecx*4 + 0x648] ; -> obj1:0x0648
loc_1_0458:
        cmp    esi, 0x1b5                    ; -> loc_5_01B5
        jne    loc_1_0496
        and    ebx, 7
        cmp    ebx, 1
        je     loc_1_0482
        test   byte ptr [0x67c], 1           ; -> obj1:0x067C
        jne    sub_1_04CE
        bts    dword ptr [0x640], 0          ; -> obj1:0x0640
        jb     sub_1_04CE
        and    byte ptr [0x640], 0xfd        ; -> obj1:0x0640
loc_1_0482:
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        je     loc_1_04A5
        push   esi
        push   ecx
        call   sub_1_0545
        pop    ecx
        pop    esi
        jmp    loc_1_04A5
loc_1_0496:
        cmp    esi, 0x243                    ; -> loc_5_0243
        jne    loc_1_04A5
        or     byte ptr [0x640], 2           ; -> obj1:0x0640
loc_1_04A5:
        push   0
        push   ecx
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
loc_1_04B2:
        dec    dword ptr [0x63c]             ; -> loc_1_063C
        jne    sub_1_04CE
        lea    esi, [0x273]                  ; -> loc_5_0273
        push   0
        push   0
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
sub_1_04CE:
        mov    eax, 0x384
        lea    esi, [0x434]                  ; -> loc_1_0434
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x634], esi        ; -> loc_1_0634
        ret    
loc_1_04E6:
        push   0x80000003
        push   2
        VxDCall  VPEND.Service_0005h             ; cd2005002600
        add    esp, 8
        jmp    loc_1_0434
        db  090h
sub_1_04FC:
        mov    ax, 0x530b
        xor    ebx, ebx
        call   ptr [0x624]                   ; -> obj1:0x0624
        sti    
        jae    loc_1_050C
        ret    
loc_1_050C:
        cmp    bx, 3
        jb     loc_1_053C
        cmp    bx, 4
        ja     loc_1_053C
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_1_053C
        mov    dword ptr [0x644], 1          ; -> obj1:0x0644
        bt     cx, 0
        jb     loc_1_053C
        mov    dword ptr [0x644], 0          ; -> obj1:0x0644
loc_1_053C:
        movzx  ecx, bx
        jecxz  loc_1_0543
        clc    
        ret    
loc_1_0543:
        stc    
        ret    
sub_1_0545:
        mov    ax, 0x5307
        mov    bx, 1
        mov    cx, 4
        call   ptr [0x624]                   ; -> obj1:0x0624
        mov    eax, 0xbb8
        lea    esi, [0x545]                  ; -> sub_1_0545
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x638], esi        ; -> loc_1_0638
        ret    
        db  000h
        dd  _VPOWERD_Get_Version     ; fixup
        dd  _VPOWERD_Get_APM_BIOS_Version ; fixup
        dd  _VPOWERD_Get_Power_Management_Level ; fixup
        dd  _VPOWERD_Set_Power_Management_Level ; fixup
        dd  _VPOWERD_Set_Device_Power_State ; fixup
        dd  _VPOWERD_Set_System_Power_State ; fixup
        dd  _VPOWERD_Restore_Power_On_Defaults ; fixup
        dd  _VPOWERD_Get_Power_Status ; fixup
        dd  _VPOWERD_Get_Power_State ; fixup
        dd  _VPOWERD_OEM_APM_Function ; fixup
        dd  _VPOWERD_Register_Power_Handler ; fixup
        dd  _VPOWERD_Deregister_Power_Handler ; fixup
        dd  _VPOWERD_W32_Get_System_Power_Status ; fixup
        dd  _VPOWERD_W32_Set_System_Power_State ; fixup
        dd  VPOWERD_Service_14       ; fixup
        dd  VPOWERD_Service_15       ; fixup
        dd  VPOWERD_Service_16       ; fixup
        dd  VPOWERD_Service_17       ; fixup
        dd  VPOWERD_Service_18       ; fixup
        dd  VPOWERD_Service_19       ; fixup
        dd  VPOWERD_Service_20       ; fixup
        dd  VPOWERD_Service_21       ; fixup
        dd  VPOWERD_Service_22       ; fixup
        dd  VPOWERD_Service_23       ; fixup
        dd  VPOWERD_Service_24       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00026h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VPOWERD '                  ; DDB_Name
        dd  001000000h                  ; DDB_Init_Order
        dd  VPOWERD_Control          ; DDB_Control_Proc
        dd  VPOWERD_PM_API           ; DDB_V86_API_Proc
        dd  VPOWERD_PM_API           ; DDB_PM_API_Proc
        dd  obj1:0x0570              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_062A:
        add    dword ptr [eax], eax
loc_1_062C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0630:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0634:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_063C:
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp - 0x4affffff], dh ; -> loc_5_01B5
        add    dword ptr [eax], eax
        add    byte ptr [ebx + 2], al        ; -> loc_5_0243
        add    byte ptr [eax], al
        inc    ebx
        add    al, byte ptr [eax]
        add    dl, ah                        ; -> loc_8_01E2
        add    dword ptr [eax], eax
        add    byte ptr [edx], ch            ; -> loc_8_022A
        add    al, byte ptr [eax]
        add    cl, dh                        ; -> loc_8_01F1
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp - 0x4affffff], dh ; -> loc_5_01B5
        add    dword ptr [eax], eax
        add    byte ptr [ebx + 2], al        ; -> loc_5_0243
        add    byte ptr [eax], al
        pop    edi
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
loc_1_0681:
        add    dword ptr [eax], eax
        add    cl, byte ptr [edx]
        sub    bl, byte ptr [ebx]
        add    byte ptr [eax - 0x77fffffa], cl ; -> obj1:0x0688
        push   es
        add    byte ptr [eax], al
loc_1_0690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x188  R-X
        db  003h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        sbb    al, byte ptr [eax]
loc_2_0010:
        mov    dh, 0
        je     loc_2_0015
loc_2_0014:
        inc    ecx
loc_2_0015:
        jbe    0x79
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x64], sp
        and    byte ptr [eax + 0x6f], dl
        ja     loc_2_0086
        jb     loc_2_0043
        dec    ebp
        popal  
        outsb  dx, byte ptr [esi]
        popal  
        insd   dword ptr es:[di], dx
        outsb  dx, byte ptr gs:[esi]
        je     loc_2_002E
loc_2_002E:
        pop    ecx
        outsd  dx, dword ptr [esi]
        jne    0xa4
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0xad
        je     loc_2_009F
        jb     0x5c
        arpl   word ptr [ecx + 0x6e], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_0063
loc_2_0043:
        bound  esp, qword ptr [ebp + 0x20]
        jae    loc_2_00BD
        jae    0xba
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr fs:[ecx + 0x74], ah
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0x78
        je     0xc3
        insd   dword ptr es:[edi], dx
        or     eax, 0x72702041
        outsd  dx, dword ptr [esi]
loc_2_0063:
        jb     0xc7
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xd5
        jb     loc_2_0092
        jns    0xe3
        jne    0x96
        push   0x20657661
        jb     loc_2_00F2
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x79616d20
loc_2_0086:
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [eax + 0x72], dh
        jbe    loc_2_00F4
        outsb  dx, byte ptr [esi]
        je     0xfb
loc_2_0092:
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x68], dh
        imul   esi, dword ptr [ebx + 0x20], 0x69746361
        outsd  dx, dword ptr [esi]
loc_2_009F:
        outsb  dx, byte ptr [esi]
        or     eax, 0x736f6c43
        and    byte ptr gs:[ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     0x113
        insd   dword ptr es:[edi], dx
        jae    0xe1
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
loc_2_00BD:
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + esi*2 + 0x79], dh
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x2e], 0x756f5900
        jb     0xef
        arpl   word ptr [edi + 0x6d], bp
        jo     loc_2_0149
        je     0x13b
        jb     0xf8
        arpl   word ptr [ecx + 0x6e], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xff
        bound  esp, qword ptr [ebp + 0x20]
        jae    0x159
        jae    loc_2_0156
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr fs:[ecx + 0x74], ah
        and    byte ptr [eax + ebp*2 + 0x69], dh
loc_2_00F2:
        jae    0x114
loc_2_00F4:
        je     0x15f
        insd   dword ptr es:[edi], dx
        or     eax, 0x736f6c43
        and    byte ptr gs:[ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     0x16b
        insd   dword ptr es:[edi], dx
        jae    0x139
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + esi*2 + 0x79], dh
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x2e], 0x2066490d
        jns    0x196
        jne    loc_2_0149
        arpl   word ptr [edi + 0x6e], bp
        je     0x197
        outsb  dx, byte ptr [esi]
        jne    0x196
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jae    0x19c
        and    byte ptr gs:[eax + ebp*2 + 0x69], dh
        jae    loc_2_015E
        insd   dword ptr es:[edi], dx
        jae    0x1b5
        popal  
        sub    al, 0xd
        jae    0x1ae
loc_2_0149:
        and    byte ptr gs:[ecx + 0x6f], bh
        jne    0x1c1
        and    byte ptr [eax + 0x61], ch
        jb     0x1b8
        ja     0x1b7
loc_2_0156:
        jb     0x1bd
        and    byte ptr [edi + ebp*2 + 0x63], ah
        jne    0x1cb
loc_2_015E:
        outsb  dx, byte ptr gs:[esi]
        je     0x1c3
        je     0x1cd
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x1d0
        arpl   word ptr [eax + 0x79], si
        outsd  dx, dword ptr [esi]
        jne    0x1e8
        or     eax, 0x706d6f63
        jne    0x1f1
        jb     0x1a0
        jbe    0x1e7
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        jb     0x1b5
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x10  R-X
        db  000h, 000h, 000h, 000h
        dd  obj3:0x0010              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x2ec  R-X
loc_4_0000:
        mov    dword ptr [0x630], edx        ; -> loc_1_0630
        lea    esi, [0x28]                   ; -> loc_1_0028
        VMMCall  VMM.Call_When_Idle              ; cd203a000100
        ret    
loc_4_0013:
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        cmp    word ptr [eax], 0x101
        jae    loc_4_0026
        mov    byte ptr [0x62a], 0           ; -> loc_1_062A
loc_4_0026:
        cmp    word ptr [eax], 0x102
        jb     loc_4_0034
        mov    byte ptr [0x62b], 1           ; -> obj1:0x062B
loc_4_0034:
        mov    ax, 0x5303
        xor    ebx, ebx
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_4_02A8
        mov    dword ptr [0x624], ebx        ; -> obj1:0x0624
        mov    esi, 0xffff
        mov    edi, esi
        movzx  eax, ax
        shl    eax, 4
        movzx  ecx, cx
        shl    ecx, 4
        movzx  ebx, dx
        shl    ebx, 4
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        je     loc_4_008C
        mov    edx, 0x100000
        sub    edx, eax
        dec    edx
        cmp    edx, esi
        ja     loc_4_007E
        mov    esi, edx
loc_4_007E:
        mov    edx, 0x100000
        sub    edx, ebx
        dec    edx
        cmp    edx, edi
        ja     loc_4_008C
        mov    edi, edx
loc_4_008C:
        push   ecx
        push   1
        push   0x40
        push   0x9a
        push   esi
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        pop    ecx
        or     eax, eax
        je     loc_4_025B
        mov    word ptr [0x628], ax          ; -> obj1:0x0628
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        je     loc_4_00D9
        mov    esi, 0xffff
        mov    edx, 0x100000
        sub    edx, ecx
        dec    edx
        cmp    edx, esi
        ja     loc_4_00D9
        mov    esi, edx
loc_4_00D9:
        push   1
        push   0
        push   0x9a
        push   esi
        push   ecx
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     eax, eax
        je     loc_4_025B
        push   1
        push   0
        push   0x92
        push   edi
        push   ebx
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     eax, eax
        je     loc_4_025B
        push   es
        mov    es, word ptr [0x628]          ; -> obj1:0x0628
        mov    edi, dword ptr [0x624]        ; -> obj1:0x0624
        cmp    dword ptr es:[edi], 0x6650669c
        jne    loc_4_014A
        cmp    dword ptr es:[edi + 4], 0xec8b6655
loc_4_014A:
        pop    es
        je     loc_4_0272
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        je     loc_4_01E2
        mov    cx, 0x101
        cmp    byte ptr [0x62b], 0           ; -> obj1:0x062B
        je     loc_4_016D
        inc    cx
loc_4_016D:
        mov    ax, 0x530e
        xor    ebx, ebx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_4_0272
        cmp    byte ptr [0x62b], 0           ; -> obj1:0x062B
        je     loc_4_01E2
        mov    ax, 0x5310
        xor    bx, bx
        call   ptr [0x624]                   ; -> obj1:0x0624
        mov    edi, dword ptr [0x630]        ; -> loc_1_0630
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 6], bl
        jae    loc_4_01C6
        mov    byte ptr [0x62b], 0           ; -> obj1:0x062B
        mov    word ptr [edi], 0x101
        mov    cx, 0x101
        mov    ax, 0x530e
        xor    ebx, ebx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_4_0272
loc_4_01C6:
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        test   dword ptr [eax + 3], 0x10
        je     loc_4_01DC
        VxDCall  VPEND.Service_0010h             ; cd2010002600
        jmp    loc_4_01E2
loc_4_01DC:
        VxDCall  VPEND.Service_000Fh             ; cd200f002600
loc_4_01E2:
        mov    byte ptr [0x62c], 0xff        ; -> loc_1_062C
        mov    eax, 0
        mov    esi, 0x2d4                    ; -> loc_4_02D4
        mov    edi, 0x2de                    ; -> loc_4_02DE
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        push   eax
        VxDCall  VPEND.Service_0003h             ; cd2003002600
        add    esp, 4
        or     eax, eax
        jne    loc_4_0272
        push   0x40000000
        push   0x68c                         ; -> loc_5_068C
        VxDCall  VPEND.Service_000Ah             ; cd200a002600
        add    esp, 8
        or     eax, eax
        jne    loc_4_0272
        mov    edi, dword ptr [0x630]        ; -> loc_1_0630
        bt     dword ptr [edi + 3], 7
        jae    loc_4_0243
        mov    eax, 0x2f
        lea    esi, [0]                      ; -> loc_5_0000
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        jb     loc_4_0272
loc_4_0243:
        push   0
        push   0
        push   0xa2                          ; -> loc_5_00A2
        VxDCall  SHELL._SHELL_HookSystemBroadcast ; cd2011001700
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0261
        clc    
        ret    
loc_4_025B:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_0261:
        mov    eax, 0x2f
        lea    esi, [0]                      ; -> loc_5_0000
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
loc_4_0272:
        call   sub_8_0078
        movzx  esi, word ptr [0x628]         ; -> obj1:0x0628
        push   0
        push   esi
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        add    esi, 8
        push   0
        push   esi
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        add    esi, 8
        push   0
        push   esi
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
loc_4_02A8:
        lea    esi, [0x28]                   ; -> loc_1_0028
        VMMCall  VMM.Cancel_Call_When_Idle       ; cd202b010100
        lea    esi, [0x688]                  ; -> obj1:0x0688
        mov    edi, dword ptr [esi]
loc_4_02BC:
        cmp    esi, edi
        je     loc_4_02D2
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_4_02BC
loc_4_02D2:
        stc    
        ret    
loc_4_02D4:
        push   eax
        dec    edi
        push   edi
        inc    ebp
        push   edx
        inc    esp
        push   edx
        push   esi
        db  000h
loc_4_02DE:
        push   eax
        outsd  dx, dword ptr [esi]
        ja     0x347
        jb     0x337
        je     0x347
        je     0x35d
        jae    loc_4_02EA
loc_4_02EA:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x8a0  R-X
loc_5_0000:
        cmp    byte ptr [ebp + 0x1d], 0x54
        je     8
        stc    
        ret    
        db  057h, 08Bh, 03Dh
        dd  loc_1_0630               ; fixup
        db  00Fh, 0BAh, 067h, 003h, 007h, 05Fh, 073h, 0EFh, 080h, 04Dh, 02Ch, 001h, 0F8h, 0C3h
loc_5_001D:
        mov    ecx, dword ptr [esi + 0xc]
        jecxz  loc_5_009F
        cmp    ecx, -1
        je     loc_5_009F
        mov    eax, 1
        dec    ecx
        cmp    ecx, 0x19
        jae    loc_5_00A1
        mov    ebx, ecx
        movzx  ecx, byte ptr [ebx*2]         ; -> obj6:0x0000
        cmp    cl, 0xff
        je     loc_5_00A1
        mov    eax, 0x57
        cmp    dword ptr [esi + 0x18], 0
        je     loc_5_0058
        mov    edi, dword ptr [esi + 0x1c]
        or     edi, edi
        je     loc_5_0058
        cmp    edi, 4
        jne    loc_5_00A1
loc_5_0058:
        mov    edx, dword ptr [esi + 0x10]
        or     edx, edx
        jne    loc_5_0066
        mov    dword ptr [esi + 0x14], 0
loc_5_0066:
        lea    edi, [ecx*4]
        cmp    edi, dword ptr [esi + 0x14]
        jne    loc_5_00A1
        jecxz  loc_5_007E
        add    edx, edi
loc_5_0076:
        sub    edx, 4
        mov    eax, dword ptr [edx]
        push   eax
        loop   loc_5_0076
loc_5_007E:
        call   dword ptr [ebx*4 + 0x570]     ; -> obj1:0x0570
        add    esp, edi
        mov    ecx, dword ptr [esi + 0x18]
        jecxz  loc_5_009F
        cmp    dword ptr [esi + 0x1c], 4
        jne    loc_5_009F
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi + 0x20]
        jecxz  loc_5_009F
        mov    dword ptr [ecx], 4
loc_5_009F:
        xor    eax, eax
loc_5_00A1:
        ret    
loc_5_00A2:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0x219
        jne    loc_5_00DA
        cmp    dword ptr [ebp + 0xc], 0x8100
        jne    loc_5_00DA
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        cmp    word ptr [esi + 4], 0xd
        jne    loc_5_00DA
        cmp    dword ptr [esi], 0xe
        jne    loc_5_00DA
        mov    eax, dword ptr [esi + 0xa]
        push   eax
        push   1
        VxDCall  VPEND.Service_000Dh             ; cd200d002600
        add    esp, 8
        mov    dword ptr [esi + 6], eax
        pop    esi
loc_5_00DA:
        mov    eax, 1
        leave  
        ret    
        db  000h, 000h, 000h
sub_5_00E4:
        push   ebp
        push   ebx
        push   esi
        push   edi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0xd
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ecx, 0x20
loc_5_011A:
        cmp    ecx, 3
        jb     loc_5_015C
        mov    byte ptr [ebp + 0x10], cl
        mov    word ptr [ebp + 0x1c], 0x4409
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_5_015A
        test   byte ptr [ebp + 0x15], 0x90
        jne    loc_5_015A
        mov    word ptr [ebp + 0x14], cx
        mov    word ptr [ebp + 0x18], 1
        mov    word ptr [ebp + 0x1c], 0x710d
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_5_015A:
        loop   loc_5_011A
loc_5_015C:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_5_0176:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   di, 0x10
        jne    loc_5_018E
        and    byte ptr [0x640], 0xfb        ; -> obj1:0x0640
        or     edi, 1
loc_5_018E:
        call   sub_5_00E4
        call   sub_8_029B
        push   0
        push   edi
        push   4
        call   sub_5_0298
        add    esp, 0xc
        push   0
        push   edi
        push   5
        call   sub_5_0298
        add    esp, 0xc
        pop    edi
        leave  
        ret    
loc_5_01B5:
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, ebx
        xor    edx, edx
        cmp    ecx, 2
        ja     loc_5_01C5
        inc    edx
loc_5_01C5:
        or     edx, 0x80000000
        and    ebx, 7
        cmp    ebx, 1
        je     loc_5_01DE
        xor    eax, eax
        test   byte ptr [0x640], 6           ; -> obj1:0x0640
        jne    loc_5_01F0
loc_5_01DE:
        push   ebx
        push   edx
        push   ebx
        VxDCall  VPEND.Service_0005h             ; cd2005002600
        add    esp, 8
        pop    ebx
        cmp    ebx, 1
        je     loc_5_020B
loc_5_01F0:
        and    byte ptr [0x640], 0xfe        ; -> obj1:0x0640
        cmp    eax, 0x60
        jne    loc_5_020B
        dec    byte ptr [0x680]              ; -> obj1:0x0680
        jne    loc_5_0212
        or     byte ptr [0x640], 4           ; -> obj1:0x0640
loc_5_020B:
        mov    byte ptr [0x680], 3           ; -> obj1:0x0680
loc_5_0212:
        leave  
        ret    
sub_5_0214:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    eax, 0x100
        push   eax
        mov    eax, 1
        push   eax
        mov    eax, 0
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        VxDCall  IFSMgr.BcsToBcs                 ; cd2070004000
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 0xc]
        pop    edx
        pop    ecx
        leave  
        ret    
loc_5_0243:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        shr    ecx, 2
        movzx  ecx, byte ptr [ecx + 0x684]   ; -> obj1:0x0684
        test   ecx, 0x10
        jne    loc_5_026A
        cmp    dword ptr [0x644], 0          ; -> obj1:0x0644
        jne    loc_5_026A
        or     ecx, 0x80
loc_5_026A:
        push   ecx
        call   sub_5_0176
        pop    ecx
        leave  
        ret    
loc_5_0273:
        enter  8, 0
        push   esi
        lea    esi, [ebp - 8]
        push   esi
        push   1
        VxDCall  VPEND.Service_0007h             ; cd2007002600
        add    esp, 8
        or     eax, eax
        je     loc_5_0295
        mov    dword ptr [0x63c], 0xffffffff ; -> loc_1_063C
loc_5_0295:
        pop    esi
        leave  
        ret    
sub_5_0298:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        lea    ebx, [0x688]                  ; -> obj1:0x0688
        mov    edi, dword ptr [ebp + 0xc]
        test   di, 4
        je     loc_5_02B3
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebx]
loc_5_02B3:
        xor    esi, esi
        test   di, 8
        je     loc_5_02C1
        mov    esi, 4
loc_5_02C1:
        mov    ebx, dword ptr [esi + ebx]
        xor    eax, eax
        cmp    ebx, 0x688                    ; -> obj1:0x0688
        je     loc_5_02EE
        push   edi
        push   dword ptr [ebp + 8]
        call   dword ptr [ebx + 8]
        pop    ecx
        pop    ecx
        test   di, 2
        jne    loc_5_02C1
        cmp    eax, 0x80000000
        je     loc_5_02C1
        or     eax, eax
        je     loc_5_02C1
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], ebx
loc_5_02EE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
_VPOWERD_Get_Power_Status:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        test   byte ptr [eax + 3], 8
        jne    loc_5_0393
        mov    ax, 0x530a
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, 0xffff
        mov    cx, 0xffff
        push   ebp
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        jb     loc_5_038B
        mov    di, si
        mov    eax, 0x80000003
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        je     loc_5_038E
        mov    dword ptr [0x63c], 0x42       ; -> loc_1_063C
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        mov    byte ptr [esi + 3], cl
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        jne    loc_5_0354
        mov    ch, 0xff
        mov    dx, 0xffff
loc_5_0354:
        mov    byte ptr [esi + 2], ch
        mov    word ptr [esi + 4], dx
        cmp    dword ptr [ebp + 8], 1
        jne    loc_5_0389
        mov    eax, dword ptr [esi]
        cmp    eax, dword ptr [0x49]         ; -> obj6:0x0049
        je     loc_5_0389
        xchg   dword ptr [0x49], eax         ; -> obj6:0x0049
        cmp    eax, -1
        je     loc_5_0389
        mov    eax, 0x22a                    ; -> loc_8_022A
        push   0
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
loc_5_0389:
        xor    eax, eax
loc_5_038B:
        movzx  eax, ah
loc_5_038E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_5_0393:
        mov    dword ptr [0x49], 0xffffffff  ; -> obj6:0x0049
        mov    eax, 9
        jmp    loc_5_038E
_VPOWERD_W32_Get_System_Power_Status:
        push   ebp
        mov    ebp, esp
        push   edi
        push   dword ptr [ebp + 8]
        push   1
        VxDCall  VPEND.Service_0016h             ; cd2016002600
        add    esp, 8
        mov    edi, dword ptr [ebp + 8]
        pop    edi
        leave  
        ret    
VPOWERD_Service_22:
        enter  0xc, 0
        push   esi
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, dword ptr [ebp + 0xc]
        push   esi
        push   dword ptr [ebp + 8]
        VxDCall  VPEND.Service_0007h             ; cd2007002600
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        jne    loc_5_048E
        mov    al, byte ptr [esi]
        cmp    al, 2
        jb     loc_5_03F0
        ja     loc_5_03EE
        mov    al, 0
        jmp    loc_5_03F0
loc_5_03EE:
        mov    al, 0xff
loc_5_03F0:
        mov    byte ptr [edi], al
        xor    ecx, ecx
        cmp    al, byte ptr [0x48]           ; -> obj6:0x0048
        je     loc_5_0406
        mov    byte ptr [0x48], al           ; -> obj6:0x0048
        cmp    al, 0
        jne    loc_5_0406
        inc    ecx
loc_5_0406:
        mov    dword ptr [ebp - 0xc], ecx
        mov    al, byte ptr [esi + 2]
        cmp    al, 0xff
        jne    loc_5_0421
        mov    cl, byte ptr [esi + 1]
        cmp    cl, 0xff
        je     loc_5_0421
        cmp    cl, 3
        ja     loc_5_0421
        mov    al, 1
        shl    al, cl
loc_5_0421:
        cmp    al, 0
        jne    loc_5_0427
        mov    al, 0xff
loc_5_0427:
        mov    byte ptr [edi + 1], al
        mov    al, byte ptr [esi + 3]
        cmp    al, 0x64
        jbe    loc_5_0433
        mov    al, 0xff
loc_5_0433:
        mov    byte ptr [edi + 2], al
        movzx  eax, word ptr [esi + 4]
        cmp    ax, -1
        jne    loc_5_0443
        cwde   
        jmp    loc_5_047A
loc_5_0443:
        btr    eax, 0xf
        jae    loc_5_044C
        imul   eax, eax, 0x3c
loc_5_044C:
        movzx  ecx, byte ptr [edi + 2]
        cmp    cl, 0xff
        je     loc_5_047A
        cmp    dword ptr [0x44], -1          ; -> obj6:0x0044
        je     loc_5_0464
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_047A
loc_5_0464:
        push   eax
        imul   eax, eax, 0x64
        cdq    
        jecxz  loc_5_046F
        idiv   ecx
        jmp    loc_5_0474
loc_5_046F:
        mov    eax, 0xffffffff
loc_5_0474:
        mov    dword ptr [0x44], eax         ; -> obj6:0x0044
        pop    eax
loc_5_047A:
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x44]         ; -> obj6:0x0044
        mov    dword ptr [edi + 8], eax
        mov    byte ptr [edi + 3], 0
        mov    eax, 1
loc_5_048E:
        pop    edi
        pop    esi
        leave  
        ret    
_VPOWERD_W32_Set_System_Power_State:
        enter  4, 0
        lea    eax, [ebp - 4]
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Global_State ; cd2057003300
        add    esp, 8
        mov    eax, 0x80000002
        test   byte ptr [ebp - 4], 2
        jne    loc_5_04C6
        xor    eax, eax
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_04BA
        mov    al, 2
loc_5_04BA:
        push   eax
        push   2
        VxDCall  VPEND.Service_0005h             ; cd2005002600
        add    esp, 8
loc_5_04C6:
        leave  
        ret    
VPOWERD_Service_17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xc
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_5_0565
        mov    eax, 0x80000003
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        je     loc_5_0565
        movzx  eax, word ptr [ebx]
        xor    edx, edx
        mov    ecx, 0x64
        div    ecx
        call   sub_8_0371
        shl    ax, 8
        mov    di, ax
        mov    ax, dx
        call   sub_8_0371
        or     di, ax
        mov    ax, word ptr [ebx + 8]
        xor    edx, edx
        call   sub_8_0371
        mov    dh, al
        mov    ax, word ptr [ebx + 0xa]
        call   sub_8_0371
        or     dx, ax
        mov    ax, word ptr [ebx + 0xc]
        call   sub_8_0371
        mov    ch, al
        mov    ax, word ptr [ebx + 2]
        call   sub_8_0371
        mov    si, ax
        shl    si, 8
        mov    ax, word ptr [ebx + 6]
        call   sub_8_0371
        or     si, ax
        mov    ax, 0x5311
        xor    bx, bx
        mov    cl, 2
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_5_0562
        xor    eax, eax
loc_5_0562:
        movzx  eax, ah
loc_5_0565:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
VPOWERD_Service_18:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xc
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_5_0686
        mov    ax, 0x5311
        xor    ebx, ebx
        mov    ecx, 1
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_5_0683
        mov    eax, 0x80000003
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        je     loc_5_0686
        push   edx
        and    dx, 0xff00
        shr    dx, 8
        mov    al, dl
        call   sub_8_0366
        mov    word ptr [ebx + 8], ax
        pop    edx
        and    dx, 0xff
        mov    al, dl
        call   sub_8_0366
        mov    word ptr [ebx + 0xa], ax
        shr    cx, 8
        xor    ch, ch
        mov    al, cl
        call   sub_8_0366
        mov    word ptr [ebx + 0xc], ax
        push   esi
        and    si, 0xff00
        shr    si, 8
        mov    ax, si
        call   sub_8_0366
        mov    word ptr [ebx + 2], ax
        pop    esi
        and    si, 0xff
        mov    ax, si
        call   sub_8_0366
        mov    word ptr [ebx + 6], ax
        mov    ax, di
        movzx  eax, ah
        call   sub_8_0366
        mov    ecx, 0x64
        mul    ecx
        push   eax
        mov    ax, di
        movzx  eax, al
        call   sub_8_0366
        pop    ecx
        add    ax, cx
        mov    word ptr [ebx], ax
        mov    word ptr [ebx + 0xe], 0
        lea    esi, [0x81]                   ; -> obj6:0x0081
        movzx  edi, ax
        test   edi, 3
        jne    loc_5_064A
        lea    esi, [0x4d]                   ; -> obj6:0x004D
loc_5_064A:
        movzx  eax, word ptr [ebx + 2]
        lea    esi, [esi + eax*4]
        mov    eax, dword ptr [esi]
        movzx  ecx, word ptr [ebx + 6]
        dec    ecx
        add    eax, ecx
        push   eax
        sub    edi, 0x7c5
        push   edi
        mov    eax, edi
        mov    ecx, 0x16d
        mul    ecx
        pop    edx
        shr    edx, 2
        pop    ecx
        add    ecx, edx
        add    eax, ecx
        xor    edx, edx
        mov    ecx, 7
        div    ecx
        mov    word ptr [ebx + 4], dx
        xor    eax, eax
loc_5_0683:
        movzx  eax, ah
loc_5_0686:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
loc_5_068C:
        enter  4, 0
        push   ebx
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 0x80000000
        mov    ecx, 7
        cmp    eax, 9
        ja     loc_5_07A5
        jmp    dword ptr [eax*4 + 0xb8]      ; -> obj6:0x00B8
loc_5_06B8:
        mov    byte ptr [0xf6], 0            ; -> obj6:0x00F6
        test   dl, 2
        jne    loc_5_07B3
        mov    eax, 1
        test   dl, 0x10
        jne    loc_5_06DC
        mov    eax, 0
        mov    ecx, 0xf
loc_5_06DC:
        and    edx, 1
        push   edx
        push   eax
        push   ecx
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_5_07B8
        add    esp, 0x14
        or     eax, eax
        jne    loc_5_07B3
        mov    ebx, 0x80000002
        mov    edx, dword ptr [ebp + 0xc]
loc_5_06FF:
        mov    eax, 3
        test   dl, 0x10
        jne    loc_5_0791
        mov    eax, 2
        jmp    loc_5_078C
loc_5_0714:
        mov    byte ptr [0xf6], 1            ; -> obj6:0x00F6
        mov    eax, 5
        test   dl, 0x10
        jne    loc_5_0791
        mov    eax, 4
        jmp    loc_5_078C
loc_5_072C:
        test   dl, 4
        je     loc_5_0741
        cmp    byte ptr [0xf6], 0            ; -> obj6:0x00F6
        je     loc_5_06FF
        mov    dword ptr [ebp - 4], 1
loc_5_0741:
        mov    eax, 6
        test   dl, 0x20
        jne    loc_5_0791
        mov    eax, 8
        test   dl, 0x10
        jne    loc_5_0791
        mov    eax, 7
        jmp    loc_5_078C
loc_5_075C:
        mov    eax, dword ptr [0x49]         ; -> obj6:0x0049
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0xa
        jmp    loc_5_078C
loc_5_076B:
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0x10
        jmp    loc_5_078C
loc_5_077D:
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0xb
        jmp    loc_5_078C
loc_5_0787:
        mov    eax, 9
loc_5_078C:
        mov    ecx, 0xf
loc_5_0791:
        push   dword ptr [ebp - 4]
        push   eax
        push   ecx
        push   0
        push   dword ptr [ebp + 0xc]
        call   sub_5_07B8
        add    esp, 0x14
        jmp    loc_5_07B3
loc_5_07A5:
        cmp    eax, 0x200
        jb     loc_5_07B3
        cmp    eax, 0x2ff
        jbe    loc_5_077D
loc_5_07B3:
        mov    eax, ebx
        pop    ebx
        leave  
        ret    
sub_5_07B8:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        lea    ebx, [ebp - 4]
        mov    edi, dword ptr [ebp + 0x14]
        mov    esi, 0xe0                     ; -> obj6:0x00E0
        test   byte ptr [ebp + 8], 8
        je     loc_5_07D5
        mov    esi, 0xe6                     ; -> obj6:0x00E6
loc_5_07D5:
        movzx  ecx, byte ptr [esi]
        jecxz  loc_5_080C
        inc    esi
        test   cl, 0x80
        jne    loc_5_0811
        test   cl, 0x40
        jne    loc_5_082F
        and    ecx, dword ptr [ebp + 0x10]
        je     loc_5_07D5
        mov    dword ptr [ebx], ecx
        push   dword ptr [ebp + 0x18]
        push   edi
        push   0x218
        push   ebx
        push   dword ptr [ebp + 0xc]
        VxDCall  SHELL._SHELL_BroadcastSystemMessage ; cd2010001700
        add    esp, 0x14
        test   byte ptr [ebp + 0xc], 1
        je     loc_5_07D5
        or     eax, eax
        jne    loc_5_07D5
loc_5_080C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_5_0811:
        cmp    edi, 4
        jb     loc_5_07D5
        cmp    edi, 7
        ja     loc_5_07D5
        movzx  eax, byte ptr [edi + 0xe8]    ; -> obj6:0x00E8
        or     eax, eax
        je     loc_5_07D5
        push   eax
        call   sub_5_084D
        pop    ecx
        jmp    loc_5_07D5
loc_5_082F:
        cmp    edi, 4
        jb     loc_5_07D5
        cmp    edi, 9
        ja     loc_5_07D5
        movzx  eax, byte ptr [edi + 0xec]    ; -> obj6:0x00EC
        or     eax, eax
        je     loc_5_07D5
        push   eax
        call   sub_5_087D
        pop    ecx
        jmp    loc_5_07D5
sub_5_084D:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 4]
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x1a
        xor    ebx, ebx
        xor    edx, edx
        VMMCall  VMM.System_Control              ; cd2093000100
        xor    eax, eax
        cmp    dword ptr [ebp - 4], -1
        je     loc_5_0878
        inc    eax
loc_5_0878:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_5_087D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0x530b
        mov    ebx, dword ptr [ebp + 8]
        push   0x2f
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        xor    eax, eax
        cmp    bh, 0x80
        je     loc_5_089B
        inc    eax
loc_5_089B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0xf8  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 002h, 000h, 0FFh, 000h, 000h, 000h, 002h, 001h
        db  002h, 001h, 001h, 001h, 0FFh, 000h, 0FFh, 000h, 001h, 001h, 0FFh, 000h, 001h, 001h, 000h, 000h
        db  000h, 000h, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 002h, 001h, 0FFh, 001h
        db  001h, 001h, 000h, 000h, 055h, 053h, 045h, 052h, 000h, 04Dh, 045h, 053h, 053h, 041h, 047h, 045h
        db  042h, 04Fh, 058h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 03Ch, 000h, 000h, 000h, 05Bh, 000h, 000h
        db  000h, 079h, 000h, 000h, 000h, 098h, 000h, 000h, 000h, 0B6h, 000h, 000h, 000h, 0D5h, 000h, 000h
        db  000h, 0F4h, 000h, 000h, 000h, 012h, 001h, 000h, 000h, 031h, 001h, 000h, 000h, 04Fh, 001h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 03Bh, 000h, 000h
        db  000h, 05Ah, 000h, 000h, 000h, 078h, 000h, 000h, 000h, 097h, 000h, 000h, 000h, 0B5h, 000h, 000h
        db  000h, 0D4h, 000h, 000h, 000h, 0F3h, 000h, 000h, 000h, 011h, 001h, 000h, 000h, 030h, 001h, 000h
        db  000h, 04Eh, 001h, 000h, 000h, 000h, 000h, 000h
        ; ---- dispatch table (10 entries) ----
        dd  loc_5_06B8
        dd  loc_5_0714
        dd  loc_5_07B3
        dd  loc_5_07B3
        dd  loc_5_07B3
        dd  loc_5_072C
        dd  loc_5_0787
        dd  loc_5_075C
        dd  loc_5_07B3
        dd  loc_5_076B
        db  008h, 006h, 040h, 080h, 001h, 000h, 001h, 080h, 040h, 006h, 008h, 000h, 001h, 000h, 003h, 002h
        db  002h, 001h, 004h, 003h, 000h, 005h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x1c2  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 010h, 0E0h
        db  000h, 0F0h, 054h, 04Fh, 053h, 048h, 049h, 042h, 041h, 000h, 000h, 053h, 079h, 073h, 074h, 065h
        db  06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch
        db  053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 056h, 078h, 044h
        db  05Ch, 056h, 050h, 04Fh, 057h, 045h, 052h, 044h, 000h, 046h, 06Ch, 061h, 067h, 073h, 000h, 0F7h
        db  0C3h, 001h, 000h, 00Fh, 085h, 095h, 000h, 066h, 089h, 00Eh, 000h, 000h, 0E8h, 0C2h, 000h, 0F6h
        db  006h, 007h, 000h, 001h, 00Fh, 085h, 089h, 000h, 0B8h, 000h, 053h, 033h, 0DBh, 0CDh, 015h, 00Fh
        db  082h, 089h, 000h, 081h, 0FBh, 04Dh, 050h, 00Fh, 085h, 081h, 000h, 0F7h, 0C1h, 002h, 000h, 074h
        db  07Bh, 0F6h, 006h, 007h, 000h, 002h, 074h, 003h, 0B8h, 000h, 001h, 0A3h, 004h, 000h, 0B8h, 000h
        db  054h, 033h, 0DBh, 0CDh, 02Fh, 03Dh, 000h, 054h, 074h, 019h, 081h, 0FBh, 04Dh, 050h, 075h, 013h
        db  0B8h, 001h, 054h, 0BBh, 000h, 001h, 0CDh, 02Fh, 072h, 052h, 088h, 03Eh, 006h, 000h, 080h, 00Eh
        db  007h, 000h, 080h, 0B8h, 003h, 053h, 033h, 0DBh, 0CDh, 015h, 072h, 040h, 0B8h, 004h, 053h, 033h
        db  0DBh, 0CDh, 015h, 072h, 037h, 0E8h, 0C4h, 000h, 075h, 005h, 080h, 00Eh, 007h, 000h, 040h, 08Dh
        db  036h, 004h, 000h, 0B8h, 006h, 000h, 0B9h, 008h, 000h, 0BAh, 002h, 000h, 0FFh, 01Eh, 000h, 000h
        db  072h, 01Ah, 066h, 08Bh, 0D0h, 033h, 0C0h, 033h, 0DBh, 033h, 0F6h, 0C3h, 0B8h, 001h, 080h, 0EBh
        db  0F6h, 0BAh, 001h, 001h, 0B9h, 020h, 000h, 0B8h, 017h, 04Ah, 0CDh, 02Fh, 0B8h, 003h, 080h, 0EBh
        db  0E6h, 041h, 050h, 04Dh, 020h, 064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 064h, 020h, 062h, 079h
        db  020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 04Dh, 061h, 06Eh, 061h, 067h, 065h, 072h, 00Dh
        db  00Ah
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        lea    eax, [esi - 4]
        push   ss
        push   eax
        push   ds
        push   0xb866001b
        add    al, byte ptr [eax]
        add    byte ptr [eax + 0xb85066], al
        add    edi, edi
        push   ds
        add    byte ptr [eax], al
        add    esp, 0xc
        or     eax, edx
        jne    loc_7_0188
        lea    eax, [esi - 0xc]
        push   ss
        push   eax
        lea    eax, [esi - 8]
        push   ss
        push   eax
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   ds
        push   0xff660049
        jbe    0x15a
        mov    eax, 0x1eff0109
        add    byte ptr [eax], al
        add    esp, 0x18
        or     eax, edx
        jne    0x17b
        cmp    word ptr [esi - 0xc], 4
        jne    0x17b
        mov    al, byte ptr [esi - 8]
        and    al, 0x3f
        mov    byte ptr [0xff660007], al
        jbe    0x17b
        mov    eax, 0x1eff0102
        add    byte ptr [eax], al
        pop    ax
loc_7_0188:
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 01Eh, 00Ch, 000h, 0E8h, 00Fh, 000h, 075h, 00Ah, 0B8h, 0F0h, 044h, 0CDh, 015h, 072h, 003h
        db  032h, 0C0h, 0C3h, 00Ch, 001h, 0C3h, 006h, 056h, 057h, 0FCh, 08Bh, 00Fh, 00Bh, 0C9h, 074h, 011h
        db  0C4h, 07Fh, 002h, 08Dh, 077h, 006h, 0F3h, 0A6h, 075h, 007h, 003h, 01Fh, 08Dh, 05Fh, 006h, 0EBh
        db  0E9h, 05Fh, 05Eh, 007h, 0C3h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x718  R-X
VPOWERD_PM_API:
        movzx  ebx, word ptr [ebp + 0x1c]
        mov    eax, 0xff
        cmp    ebx, 0x19
        jae    loc_8_006C
        movzx  ecx, byte ptr [ebx*2]         ; -> obj6:0x0000
        lea    edi, [ecx*4]
        jecxz  loc_8_0063
        mov    eax, 0x8000000c
        cmp    cl, 0xff
        je     loc_8_006C
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        lea    esi, [edi + eax]
        movzx  edx, byte ptr [ebx*2 + 1]     ; -> obj6:0x0001
loc_8_003E:
        sub    esi, 4
        mov    eax, dword ptr [esi]
        rcr    edx, 1
        jae    loc_8_0060
        or     eax, eax
        je     loc_8_0060
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        mov    ax, 0x141c
        VMMCall  VMM.Map_Flat                    ; cd201c000100
loc_8_0060:
        push   eax
        loop   loc_8_003E
loc_8_0063:
        call   dword ptr [ebx*4 + 0x570]     ; -> obj1:0x0570
        add    esp, edi
loc_8_006C:
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        ret    
sub_8_0078:
        xor    esi, esi
        xchg   dword ptr [0x634], esi        ; -> loc_1_0634
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    byte ptr [0x62c], 0           ; -> loc_1_062C
        mov    ax, 0x5304
        xor    ebx, ebx
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    edi, dword ptr [0x630]        ; -> loc_1_0630
        btr    dword ptr [edi + 3], 7
        jae    loc_8_00B9
        mov    ax, 0x5401
        mov    bl, byte ptr [edi + 2]
        mov    bh, 1
        push   0x2f
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_8_00B9:
        clc    
        ret    
        db  000h
loc_8_00BC:
        enter  0x18, 0
        push   1
        push   0xc
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_8_01E0
        push   eax
        push   1
        push   0x40
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_8_01E0
        mov    dword ptr [ebp - 0x14], eax
        push   1
        push   0x100
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_8_01E0
        mov    dword ptr [ebp - 0x18], eax
        movzx  eax, word ptr [0xc]           ; -> loc_2_000C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        mov    ebx, dword ptr [ebp - 0x14]
        push   ebx
        push   eax
        call   sub_5_0214
        add    esp, 8
        mov    ecx, 0x200
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        jb     loc_8_01B5
        mov    word ptr [ebp - 0x10], cx
        mov    dword ptr [ebp - 8], edx
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        cmp    dword ptr [ebp + 8], 0x80000002
        je     loc_8_015E
        movzx  eax, word ptr [0x10]          ; -> loc_2_0010
        add    eax, dword ptr [4]            ; -> loc_2_0004
loc_8_015E:
        mov    ebx, dword ptr [ebp - 0x18]
        push   ebx
        push   eax
        call   sub_5_0214
        add    esp, 8
        mov    ecx, 0x200
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        jb     loc_8_01B5
        pop    eax
        mov    word ptr [ebp - 0xc], cx
        mov    dword ptr [ebp - 4], edx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], cx
        mov    cx, word ptr [ebp - 0x10]
        mov    word ptr [eax + 4], cx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [eax + 6], cx
        mov    cx, word ptr [ebp - 0xc]
        mov    word ptr [eax + 8], cx
        push   eax
        push   eax
        push   0xc
        push   0x39                          ; -> obj6:0x0039
        push   0x34                          ; -> obj6:0x0034
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
loc_8_01B5:
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x14]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x18]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_8_01E0:
        leave  
        ret    
loc_8_01E2:
        push   0
        push   2
        push   6
        call   sub_5_0298
        add    esp, 0xc
        ret    
loc_8_01F1:
        call   sub_8_029B
        push   0
        push   2
        push   8
        call   sub_5_0298
        add    esp, 0xc
        ret    
loc_8_0205:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 0x200
        jb     loc_8_0228
        cmp    ecx, 0x2ff
        ja     loc_8_0228
        push   0
        push   2
        push   ecx
        call   sub_5_0298
        add    esp, 0xc
loc_8_0228:
        leave  
        ret    
loc_8_022A:
        enter  8, 0
        push   esi
        cmp    dword ptr [ebp + 8], 0
        je     loc_8_024E
        lea    esi, [ebp - 8]
        mov    dword ptr [0x49], 0xffffffff  ; -> obj6:0x0049
        push   esi
        push   1
        VxDCall  VPEND.Service_0007h             ; cd2007002600
        add    esp, 8
loc_8_024E:
        push   0
        push   2
        push   7
        call   sub_5_0298
        add    esp, 0xc
        pop    esi
        leave  
        ret    
loc_8_025F:
        enter  8, 0
        mov    ax, 0x5310
        xor    bx, bx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_8_028C
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        mov    word ptr [eax + 4], cx
        mov    byte ptr [eax + 6], bl
        push   0
        push   2
        push   9
        call   sub_5_0298
        add    esp, 0xc
loc_8_028C:
        lea    ecx, [ebp - 8]
        push   ecx
        push   1
        call   _VPOWERD_Get_Power_Status
        pop    ecx
        pop    ecx
        leave  
        ret    
sub_8_029B:
        push   ebx
        push   esi
        push   edi
        clc    
        mov    ah, 2
        push   0x1a
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_8_0362
        mov    al, cl
        call   sub_8_0366
        mov    esi, eax
        mov    al, dh
        call   sub_8_0366
        mov    edi, eax
        mov    al, ch
        call   sub_8_0366
        xor    edx, edx
        mov    ecx, 0x3c
        mul    ecx
        add    eax, esi
        mul    ecx
        add    eax, edi
        mov    ecx, 0x1234dc
        mul    ecx
        mov    ecx, edx
        shr    eax, 0x10
        mov    edx, eax
        mov    ah, 1
        push   0x1a
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_8_02EF:
        cmp    cx, 0x18
        jb     loc_8_0308
        cmp    dx, 0x9c
        jb     loc_8_0308
        xor    eax, eax
        push   0x1a
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jmp    loc_8_02EF
loc_8_0308:
        clc    
        mov    ah, 4
        push   0x1a
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_8_0362
        mov    al, dh
        call   sub_8_0366
        mov    dh, al
        mov    al, dl
        call   sub_8_0366
        mov    dl, al
        mov    al, cl
        call   sub_8_0366
        mov    esi, eax
        mov    cl, al
        mov    al, ch
        call   sub_8_0366
        imul   eax, eax, 0x64
        add    esi, eax
        mov    edi, edx
        mov    ah, 0x2a
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    cx, si
        jne    loc_8_0354
        cmp    dx, di
        je     loc_8_0362
loc_8_0354:
        mov    ecx, esi
        mov    edx, edi
        mov    ah, 0x2b
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_8_0362:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_8_0366:
        mov    ah, al
        and    al, 0xf
        shr    ah, 4
        aad    0xa
        cwde   
        ret    
sub_8_0371:
        aam    0xa
        shl    ah, 4
        or     al, ah
        movzx  eax, al
        ret    
_VPOWERD_Register_Power_Handler:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        lea    eax, [0x688]                  ; -> obj1:0x0688
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, eax
        mov    ecx, eax
loc_8_0392:
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        je     loc_8_03A6
        cmp    dword ptr [eax + 8], esi
        je     loc_8_03D0
        cmp    dword ptr [eax + 0xc], edi
        ja     loc_8_0392
        mov    ebx, eax
        jmp    loc_8_0392
loc_8_03A6:
        push   0
        push   0x10
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_8_03D7
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0xc], edi
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx], eax
        xor    eax, eax
loc_8_03CB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_8_03D0:
        mov    eax, 0x8000000a
        jmp    loc_8_03CB
loc_8_03D7:
        mov    eax, 0x80000009
        jmp    loc_8_03CB
_VPOWERD_Deregister_Power_Handler:
        push   ebp
        mov    ebp, esp
        lea    eax, [0x688]                  ; -> obj1:0x0688
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, eax
loc_8_03EC:
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        je     loc_8_0411
        cmp    dword ptr [eax + 8], edx
        jne    loc_8_03EC
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        xor    eax, eax
loc_8_040F:
        leave  
        ret    
loc_8_0411:
        mov    eax, 0x8000000b
        jmp    loc_8_040F
_VPOWERD_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
_VPOWERD_Get_APM_BIOS_Version:
        mov    eax, dword ptr [0x630]        ; -> loc_1_0630
        movzx  eax, word ptr [eax]
        ret    
_VPOWERD_Get_Power_Management_Level:
        movzx  eax, byte ptr [0x690]         ; -> loc_1_0690
        ret    
_VPOWERD_Set_Power_Management_Level:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ah, 3
        cmp    byte ptr [0x62c], 0           ; -> loc_1_062C
        je     loc_8_04ED
        mov    eax, 0x80000005
        cmp    dword ptr [ebp + 8], 2
        ja     loc_8_04F0
        cmp    byte ptr [0x62a], 0           ; -> loc_1_062A
        jne    loc_8_04F5
        cmp    byte ptr [0x690], 0           ; -> loc_1_0690
        je     loc_8_047C
        mov    ax, 0x5303
        xor    ebx, ebx
        push   ebp
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
        jb     loc_8_04ED
loc_8_047C:
        cmp    dword ptr [ebp + 8], 2
        je     loc_8_0488
        mov    cx, 1
        jmp    loc_8_048A
loc_8_0488:
        xor    ecx, ecx
loc_8_048A:
        mov    ax, 0x5308
        mov    bx, 0xffff
        call   ptr [0x624]                   ; -> obj1:0x0624
        jae    loc_8_04A5
        cmp    dword ptr [ebp + 8], 2
        jne    loc_8_04ED
        cmp    ah, 1
        jne    loc_8_04ED
loc_8_04A5:
        cmp    dword ptr [ebp + 8], 0
        je     loc_8_04C9
        mov    ax, 0x5304
        xor    ebx, ebx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jae    loc_8_04C9
        mov    ax, 0x5304
        xor    ebx, ebx
        push   ebp
        push   0x15
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebp
loc_8_04C9:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [0x690], al          ; -> loc_1_0690
        cmp    eax, 0
        jne    loc_8_04DD
        call   sub_1_04CE
        jmp    loc_8_04EB
loc_8_04DD:
        xor    esi, esi
        xchg   dword ptr [0x634], esi        ; -> loc_1_0634
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_8_04EB:
        xor    eax, eax
loc_8_04ED:
        movzx  eax, ah
loc_8_04F0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_8_04F5:
        mov    cx, 1
        call   sub_8_053B
        mov    cx, 1
        call   sub_8_0541
        jb     loc_8_04ED
        mov    cx, 1
        call   sub_8_053B
        jb     loc_8_04ED
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0
        je     loc_8_04C9
        cmp    eax, 1
        jne    loc_8_052E
        mov    cx, 0
        call   sub_8_0541
        jae    loc_8_04C9
        jmp    loc_8_04ED
loc_8_052E:
        mov    cx, 0
        call   sub_8_053B
        jae    loc_8_04C9
        jmp    loc_8_04ED
sub_8_053B:
        mov    ax, 0x5308
        jmp    loc_8_0545
sub_8_0541:
        mov    ax, 0x530f
loc_8_0545:
        mov    bx, 1
        call   ptr [0x624]                   ; -> obj1:0x0624
        ret    
_VPOWERD_Set_Device_Power_State:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 0x80000006
        cmp    ebx, 1
        je     loc_8_0577
        mov    ax, 0x5307
        mov    ecx, dword ptr [ebp + 0xc]
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_8_0574
        xor    eax, eax
loc_8_0574:
        movzx  eax, ah
loc_8_0577:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
_VPOWERD_Get_Power_State:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ax, 0x530c
        mov    ebx, dword ptr [ebp + 8]
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_8_05A4
        mov    eax, 0x80000003
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        je     loc_8_05A7
        movzx  ecx, cx
        mov    dword ptr [esi], ecx
        xor    eax, eax
loc_8_05A4:
        movzx  eax, ah
loc_8_05A7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
_VPOWERD_Restore_Power_On_Defaults:
        mov    ax, 0x5309
        mov    bx, 0xffff
        call   ptr [0x624]                   ; -> obj1:0x0624
        movzx  eax, ah
        jb     loc_8_05C1
        xor    eax, eax
loc_8_05C1:
        ret    
_VPOWERD_OEM_APM_Function:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pushal 
        mov    eax, 0x80000003
        jecxz  loc_8_0617
        mov    edi, dword ptr [ecx]
        mov    esi, dword ptr [ecx + 4]
        mov    ebp, dword ptr [ecx + 8]
        mov    ebx, dword ptr [ecx + 0xc]
        mov    edx, dword ptr [ecx + 0x10]
        mov    ah, byte ptr [ecx + 0x1c]
        sahf   
        mov    eax, dword ptr [ecx + 0x18]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    ax, 0x5380
        call   ptr [0x624]                   ; -> obj1:0x0624
        xchg   dword ptr [esp + 0x18], ecx
        mov    dword ptr [ecx + 0x18], eax
        lahf   
        mov    byte ptr [ecx + 0x1c], ah
        mov    dword ptr [ecx + 0x10], edx
        mov    dword ptr [ecx + 0xc], ebx
        mov    dword ptr [ecx + 8], ebp
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx], edi
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, 0
loc_8_0617:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        leave  
        ret    
VPOWERD_Service_14:
        push   ebp
        mov    ebp, esp
        mov    eax, 0xc
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_8_0648
        mov    ecx, dword ptr [0x630]        ; -> loc_1_0630
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x80000003
        or     edx, edx
        je     loc_8_0648
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
        xor    eax, eax
loc_8_0648:
        leave  
        ret    
VPOWERD_Service_16:
        xor    ecx, ecx
        jmp    sub_8_0653
VPOWERD_Service_15:
        mov    ecx, 1
sub_8_0653:
        push   ebx
        mov    eax, 0xc
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_8_0676
        mov    eax, 0x5312
        xor    ebx, ebx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_8_0673
        xor    eax, eax
loc_8_0673:
        movzx  eax, ah
loc_8_0676:
        pop    ebx
        ret    
VPOWERD_Service_24:
        push   ebp
        mov    ebp, esp
        mov    eax, 0x80000003
        cmp    dword ptr [ebp + 8], 0
        je     loc_8_0698
        mov    ecx, 2
        call   sub_8_0653
        movzx  ecx, cx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ecx
loc_8_0698:
        leave  
        ret    
VPOWERD_Service_19:
        push   ebx
        mov    eax, 0xc
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_8_06C7
        mov    ax, 0x5311
        xor    ebx, ebx
        xor    ecx, ecx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_8_06BD
        xor    eax, eax
        jmp    loc_8_06C7
loc_8_06BD:
        cmp    ah, 0xd
        jne    loc_8_06C4
        xor    eax, eax
loc_8_06C4:
        movzx  eax, ah
loc_8_06C7:
        pop    ebx
        ret    
VPOWERD_Service_20:
        mov    ecx, 1
        jmp    sub_8_06D2
VPOWERD_Service_21:
        xor    ecx, ecx
sub_8_06D2:
        push   ebx
        mov    eax, 0xc
        cmp    byte ptr [0x62b], 1           ; -> obj1:0x062B
        jne    loc_8_06F4
        mov    ax, 0x5313
        xor    ebx, ebx
        call   ptr [0x624]                   ; -> obj1:0x0624
        jb     loc_8_06F1
        xor    eax, eax
loc_8_06F1:
        movzx  eax, ah
loc_8_06F4:
        pop    ebx
        ret    
VPOWERD_Service_23:
        push   ebp
        mov    ebp, esp
        mov    eax, 0x80000003
        cmp    dword ptr [ebp + 8], 0
        je     loc_8_0716
        mov    ecx, 2
        call   sub_8_06D2
        movzx  ecx, cx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ecx
loc_8_0716:
        leave  
        ret    
VxD__LTEXT ENDS
