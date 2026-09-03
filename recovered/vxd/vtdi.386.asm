; VTDI -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 444 instructions recovered by recursive descent

Declare_Virtual_Device VTDI, 3, 0, VTDI_Control, 00488h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054006000h   flags 00000h
;   service table: obj1:0x3e8, 14 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x510  R-X
VTDI_Service_00:
        mov    ax, 0x100
        clc    
        ret    
VTDI_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h, 000h
VTDI_Service_10:
        mov    eax, dword ptr [0x480]        ; -> loc_1_0480
        add    dword ptr [0x480], 1          ; -> loc_1_0480
        adc    dword ptr [0x480], 0          ; -> loc_1_0480
        clc    
        ret    
sub_1_0031:
        mov    ecx, 8
loc_1_0036:
        cmp    dword ptr [edx], eax
        je     loc_1_0041
        add    edx, 8
        loop   loc_1_0036
        stc    
        ret    
loc_1_0041:
        clc    
        ret    
loc_1_0043:
        push   edx
        call   sub_1_0031
        pop    ecx
        jb     loc_1_005C
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 4], eax
        test   eax, eax
        jne    loc_1_0059
        mov    dword ptr [edx], eax
loc_1_0059:
        sub    eax, eax
        ret    
loc_1_005C:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_0059
        mov    edx, ecx
        push   eax
        sub    eax, eax
        call   sub_1_0031
        pop    eax
        jb     loc_1_007B
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 4], eax
        jmp    loc_1_0059
loc_1_007B:
        mov    al, 1
        ret    
VTDI_Service_11:
        mov    edx, 0x4c4                    ; -> loc_1_04C4
        jmp    loc_1_0043
VTDI_Service_12:
        mov    edx, 0x484                    ; -> loc_1_0484
        jmp    loc_1_0043
sub_1_008C:
        mov    ecx, dword ptr [eax]
loc_1_008E:
        jecxz  loc_1_009A
        cmp    ecx, edx
        jne    loc_1_0096
        jmp    loc_1_00A0
loc_1_0096:
        mov    ecx, dword ptr [ecx]
        jmp    loc_1_008E
loc_1_009A:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
loc_1_00A0:
        ret    
sub_1_00A1:
        mov    ecx, dword ptr [eax]
        jecxz  loc_1_00B9
        cmp    ecx, edx
        jne    loc_1_00B5
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], 0
        jmp    loc_1_00B9
loc_1_00B5:
        mov    eax, ecx
        jmp    sub_1_00A1
loc_1_00B9:
        ret    
loc_1_00BA:
        mov    eax, 0x47c                    ; -> loc_1_047C
        call   sub_1_00A1
        mov    dword ptr [edx + 0xc], 0
        jmp    loc_1_00E5
loc_1_00CD:
        mov    dword ptr [edx + 0xc], 0
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        jb     loc_1_00F1
        mov    ecx, dword ptr [ebx + 8]
        test   byte ptr [ecx + 0x2d], 2
        je     loc_1_00F1
loc_1_00E5:
        push   dword ptr [edx + 8]
        push   edx
        cld    
        call   dword ptr [edx + 4]
        add    esp, 8
        ret    
loc_1_00F1:
        mov    esi, 0xba                     ; -> loc_1_00BA
        sub    ebx, ebx
        mov    ecx, 0x801
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   esi, esi
        je     loc_1_0116
        mov    dword ptr [edx + 0xc], esi
        mov    eax, 0x47c                    ; -> loc_1_047C
        call   sub_1_008C
loc_1_0116:
        ret    
VTDI_Service_01:
        mov    edx, dword ptr [esp + 4]
        mov    eax, 0x478                    ; -> loc_1_0478
        call   sub_1_008C
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 8]
        push   esi
        mov    esi, 0x185                    ; -> loc_1_0185
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edx + 0x14], esi
        mov    eax, esi
        pop    esi
        cld    
        ret    
VTDI_Service_02:
        mov    edx, dword ptr [esp + 4]
        mov    eax, 0x478                    ; -> loc_1_0478
        call   sub_1_00A1
        mov    eax, 0x47c                    ; -> loc_1_047C
        call   sub_1_00A1
        push   esi
        mov    eax, dword ptr [edx + 0x14]
        mov    esi, eax
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    esi, dword ptr [edx + 0xc]
        or     eax, esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        sub    esi, esi
        mov    dword ptr [edx + 0x14], esi
        mov    dword ptr [edx + 0xc], esi
        pop    esi
        cld    
        ret    
loc_1_0185:
        mov    dword ptr [edx + 0x14], 0
        mov    eax, 0x478                    ; -> loc_1_0478
        call   sub_1_00A1
        jmp    loc_1_00CD
VTDI_Service_03:
        mov    edx, dword ptr [esp + 4]
        cmp    dword ptr [edx + 0xc], 0
        jne    loc_1_01D8
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 8], ecx
        push   esi
        push   ebx
        mov    esi, 0xba                     ; -> loc_1_00BA
        sub    eax, eax
        mov    ecx, 0x809
        sub    ebx, ebx
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        test   esi, esi
        je     loc_1_01D3
        mov    dword ptr [edx + 0xc], esi
        mov    eax, 0x47c                    ; -> loc_1_047C
        call   sub_1_008C
loc_1_01D3:
        mov    eax, esi
        pop    ebx
        pop    esi
        ret    
loc_1_01D8:
        mov    eax, edx
        ret    
VTDI_Service_04:
        mov    edx, dword ptr [esp + 4]
        mov    eax, 0x47c                    ; -> loc_1_047C
        call   sub_1_00A1
        sub    eax, eax
        xchg   dword ptr [edx + 0xc], eax
        push   esi
        mov    esi, eax
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        pop    esi
        cld    
        ret    
VTDI_Service_05:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [edx + 8], ebx
        pop    ebx
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_1_020F:
        cmp    dword ptr [edx + 4], 0
        jne    loc_1_0225
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        sub    eax, eax
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        jmp    loc_1_020F
loc_1_0225:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    eax, dword ptr [edx]
        cld    
        ret    
VTDI_Service_13:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [edx + 8], ebx
        pop    ebx
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_1_0244:
        cmp    dword ptr [edx + 4], 0
        jne    loc_1_025D
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        mov    eax, 0x3e8
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        jmp    loc_1_0244
loc_1_025D:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    eax, dword ptr [edx]
        cld    
        ret    
VTDI_Service_06:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], eax
        mov    ecx, dword ptr [eax + 8]
        jecxz  loc_1_0283
        xchg   ebx, ecx
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        mov    ebx, ecx
loc_1_0283:
        cld    
        ret    
VTDI_Service_09:
        push   esi
        push   edi
        push   ebx
        mov    edx, 0x478                    ; -> loc_1_0478
        sub    edx, 0
loc_1_0290:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_02AE
        cmp    dword ptr [ecx + 0x10], eax
        jne    loc_1_02AA
        sub    esi, esi
        xchg   dword ptr [ecx + 0x14], esi
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        jmp    loc_1_0290
loc_1_02AA:
        mov    edx, ecx
        jmp    loc_1_0290
loc_1_02AE:
        mov    edx, 0x47c                    ; -> loc_1_047C
        sub    edx, 0
loc_1_02B6:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_02D4
        cmp    dword ptr [ecx + 0x10], eax
        jne    loc_1_02D0
        sub    esi, esi
        xchg   dword ptr [ecx + 0xc], esi
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        jmp    loc_1_02B6
loc_1_02D0:
        mov    edx, ecx
        jmp    loc_1_02B6
loc_1_02D4:
        sub    ebx, ebx
        sub    ecx, ecx
        mov    esi, 0x484                    ; -> loc_1_0484
        mov    edi, 0x4c4                    ; -> loc_1_04C4
loc_1_02E2:
        cmp    dword ptr [ebx + esi], eax
        jne    loc_1_02EE
        mov    dword ptr [ebx + esi], ecx
        mov    dword ptr [ebx + esi + 4], ecx
loc_1_02EE:
        cmp    dword ptr [ebx + edi], eax
        jne    loc_1_02FA
        mov    dword ptr [ebx + edi], ecx
        mov    dword ptr [ebx + edi + 4], ecx
loc_1_02FA:
        add    ebx, 8
        cmp    ebx, 0x40
        jb     loc_1_02E2
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, 0x4c4                    ; -> loc_1_04C4
        mov    ebx, 8
loc_1_0310:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     loc_1_031D
        push   edi
        call   eax
        add    esp, 4
loc_1_031D:
        add    esi, 8
        dec    ebx
        jne    loc_1_0310
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_1_0327:
        push   esi
        push   edx
        push   eax
        mov    esi, eax
loc_1_032C:
        lodsb  al, byte ptr [esi]
        cmp    al, byte ptr [edx]
        jne    loc_1_0338
        test   al, al
        je     loc_1_0338
        inc    edx
        jmp    loc_1_032C
loc_1_0338:
        pop    eax
        pop    edx
        pop    esi
        ret    
VTDI_Service_08:
        cld    
        mov    edx, dword ptr [esp + 4]
sub_1_0341:
        push   esi
        mov    ecx, dword ptr [0x504]        ; -> loc_1_0504
        jecxz  loc_1_035D
        mov    esi, 0x508                    ; -> loc_1_0508
loc_1_034F:
        mov    eax, dword ptr [esi]
        call   sub_1_0327
        je     loc_1_0361
        add    esi, 8
        loop   loc_1_034F
loc_1_035D:
        sub    eax, eax
        pop    esi
        ret    
loc_1_0361:
        mov    eax, dword ptr [esi + 4]
        mov    edx, esi
        pop    esi
        ret    
VTDI_Service_07:
        cld    
        mov    edx, dword ptr [esp + 4]
        call   sub_1_0341
        test   eax, eax
        jne    loc_1_03C4
        mov    edx, dword ptr [0x504]        ; -> loc_1_0504
        imul   edx, edx, 8
        add    edx, 0x508                    ; -> loc_1_0508
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [0x504]             ; -> loc_1_0504
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        push   ebx
        mov    esi, 0x484                    ; -> loc_1_0484
        mov    ebx, 8
        mov    edi, eax
loc_1_03AB:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     loc_1_03B8
        push   edi
        call   eax
        add    esp, 4
loc_1_03B8:
        add    esi, 8
        dec    ebx
        jne    loc_1_03AB
        pop    ebx
        pop    edi
        pop    esi
        mov    eax, esp
        ret    
loc_1_03C4:
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_03E4
        push   esi
        push   edi
        mov    edi, edx
        lea    esi, [edx + 8]
        mov    ecx, 0x510                    ; -> obj1:0x0510
        sub    ecx, esi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        dec    dword ptr [0x504]             ; -> loc_1_0504
        ret    
loc_1_03E4:
        sub    eax, eax
        ret    
        db  000h
        dd  VTDI_Service_00          ; fixup
        dd  VTDI_Service_01          ; fixup
        dd  VTDI_Service_02          ; fixup
        dd  VTDI_Service_03          ; fixup
        dd  VTDI_Service_04          ; fixup
        dd  VTDI_Service_05          ; fixup
        dd  VTDI_Service_06          ; fixup
        dd  VTDI_Service_07          ; fixup
        dd  VTDI_Service_08          ; fixup
        dd  VTDI_Service_09          ; fixup
        dd  VTDI_Service_10          ; fixup
        dd  VTDI_Service_11          ; fixup
        dd  VTDI_Service_12          ; fixup
        dd  VTDI_Service_13          ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00488h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VTDI    '                  ; DDB_Name
        dd  054006000h                  ; DDB_Init_Order
        dd  VTDI_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x03E8              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_0474:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_1_0478:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_047C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0480:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_0484:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0504:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0508:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1c  R-X
loc_2_0000:
        inc    byte ptr [0x474]              ; -> loc_1_0474
        jne    loc_2_001A
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0x470], ebx        ; -> obj1:0x0470
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
loc_2_001A:
        ret    
        db  029h
VxD__LTEXT ENDS
