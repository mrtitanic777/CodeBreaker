; ENABLE -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 3322 instructions recovered by recursive descent

Declare_Virtual_Device ENABLE, 4, 0, ENABLE_Control, 00037h, Undefined_Init_Order
;   DDK version 0400h   init_order 038000000h   flags 00000h
;   service table: obj1:0x33c, 10 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3e0  R-X
sub_1_0000:
        xor    eax, eax
        xor    ebx, ebx
        lea    ecx, [ebx + 0x40]
        mov    esi, 0x5b2                    ; -> loc_4_05B2
        xor    edi, edi
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
VMINI_ShutDownItem:
        mov    esi, 0x23c                    ; -> loc_7_023C
        mov    edx, ebx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_1_0021:
        ret    
VMINI_Status:
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        ret    
VMINI_Siren:
        pushal 
        mov    esi, 0x227                    ; -> loc_7_0227
        mov    dh, al
        mov    dl, cl
        shl    edx, 0x10
        mov    dx, bx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x2000
        mov    ecx, 8
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        popal  
        ret    
VMINI_Update:
        mov    esi, 0x3e1                    ; -> loc_4_03E1
        mov    edx, eax
        VMMCall  VMM.Schedule_Global_Event       ; cd200e800100
VMINI_SetTimeStamp:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [0x3c8], eax        ; -> loc_1_03C8
        ret    
VMINI_DisplayError:
        mov    esi, 0x250                    ; -> loc_7_0250
        mov    edx, eax
        VMMCall  VMM.Schedule_Global_Event       ; cd200e800100
ENABLE_Control:
        cmp    eax, 2
        je     loc_3_0000
        cmp    eax, 0x23
        je     loc_4_12EB
        cmp    eax, 0x22
        jne    loc_1_0096
        push   edx
        push   ebx
        call   sub_4_17FC
        stc    
        ret    
loc_1_0096:
        clc    
        ret    
loc_1_0098:
        cmp    byte ptr [0x3c6], 0           ; -> loc_1_03C6
        jne    loc_1_00BF
        pushal 
        or     al, al
        jne    loc_1_00AA
        or     esi, edi
        je     loc_1_00BE
loc_1_00AA:
        mov    esi, 0x1be                    ; -> loc_7_01BE
        mov    edx, eax
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    byte ptr [0x3c6], 0xff        ; -> loc_1_03C6
loc_1_00BE:
        popal  
loc_1_00BF:
        jmp    loc_1_03D4
loc_1_00C5:
        cmp    byte ptr [0x3d8], 1           ; -> loc_1_03D8
        je     loc_1_00DD
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        call   sub_1_013E
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
loc_1_00DD:
        jb     loc_1_00E5
        jmp    loc_1_03D0
loc_1_00E5:
        ret    
sub_1_00E6:
        pushal 
        test   byte ptr [0x3cd], 0x40        ; -> obj1:0x03CD
        je     loc_1_0113
        test   word ptr [0x3cc], 0x77b       ; -> loc_1_03CC
        je     loc_1_0113
        mov    cx, word ptr [ebp + 0x10]
        mov    ah, cl
        mov    al, 0xe0
        call   dword ptr [0x3b4]             ; -> obj1:0x03B4
        jb     loc_1_0113
        mov    al, 0xe0
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
loc_1_0113:
        popal  
        ret    
sub_1_0115:
        cmp    al, 0x45
        je     loc_1_013A
        cmp    al, 0xc5
        jne    loc_1_013C
        mov    byte ptr [0x3c2], 0           ; -> loc_1_03C2
        mov    esi, 0x475                    ; -> sub_7_0475
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        test   dh, 0x40
        je     loc_1_013A
        call   sub_1_03B8
loc_1_013A:
        clc    
        ret    
loc_1_013C:
        stc    
        ret    
sub_1_013E:
        push   ebp
        mov    ebp, 0x3b4                    ; -> obj1:0x03B4
        push   eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [ebp + 0x14], eax
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        and    al, 0xaf
        mov    word ptr [ebp + 0x10], ax
        pop    eax
        push   eax
        cmp    byte ptr [ebp + 0xe], 1
        jne    loc_1_016F
        call   sub_1_0115
        jae    loc_1_01B6
loc_1_016F:
        cmp    byte ptr [ebp + 0x29], 1
        jne    loc_1_017B
        pop    eax
        jmp    loc_1_031D
loc_1_017B:
        test   al, 0x80
        je     loc_1_0186
        mov    dword ptr [ebp + 8], 0
loc_1_0186:
        cmp    word ptr [ebp + 0x10], 0x10b
        je     loc_1_0196
        cmp    word ptr [ebp + 0x10], 0x12b
        jne    loc_1_01E1
loc_1_0196:
        cmp    byte ptr [ebp + 0xd], 0x1d
        je     loc_1_01BC
        cmp    byte ptr [ebp + 0xd], 0x9d
        je     loc_1_01BC
        cmp    al, 0x45
        jne    loc_1_01BC
        mov    edx, 0x40
        call   0
        mov    byte ptr [ebp + 0xe], 1
        jmp    loc_1_01D2
loc_1_01B6:
        pop    eax
        jmp    loc_1_0330
loc_1_01BC:
        cmp    al, 0xe0
        je     loc_1_01DB
        cmp    al, 0x54
        je     loc_1_01C8
        cmp    al, 0x37
        jne    loc_1_01E1
loc_1_01C8:
        mov    edx, 0x800
        call   0
loc_1_01D2:
        pop    eax
        mov    al, 0xff
        push   eax
        jmp    loc_1_0305
loc_1_01DB:
        mov    byte ptr [ebp + 0x2a], 1
        jmp    loc_1_01B6
loc_1_01E1:
        cmp    byte ptr [ebp + 0x2a], 0
        je     loc_1_01F0
        call   sub_1_00E6
        mov    byte ptr [ebp + 0x2a], 0
loc_1_01F0:
        test   byte ptr [ebp + 0x13], 1
        jne    loc_1_02AF
        test   byte ptr [0x3cc], 1           ; -> loc_1_03CC
        jne    loc_1_020F
        test   word ptr [ebp + 0x10], 0xd8c
        jne    loc_1_02AF
loc_1_020F:
        mov    bl, al
        and    al, 0x7f
        cmp    al, 0x2a
        je     loc_1_021B
        cmp    al, 0x36
        jne    loc_1_026D
loc_1_021B:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_0234
        mov    ah, byte ptr [ebp + 0xd]
        and    ah, 0x7f
        cmp    ah, al
        je     loc_1_0234
        mov    byte ptr [ebp + 0xc], 0
        jmp    loc_1_0305
loc_1_0234:
        test   bl, 0x80
        je     loc_1_023C
        inc    byte ptr [ebp + 0xc]
loc_1_023C:
        mov    cl, byte ptr [ebp + 0xd]
        and    cl, 0x7f
        cmp    cl, 0x2a
        je     loc_1_024C
        cmp    cl, 0x36
        jne    loc_1_0271
loc_1_024C:
        pop    eax
        mov    al, bl
        push   eax
        cmp    byte ptr [ebp + 0xc], 5
        jl     loc_1_0271
        mov    byte ptr [ebp + 0xc], 0
        mov    byte ptr [ebp + 0xd], 0
        mov    edx, 1
        call   0
        jmp    loc_1_0305
loc_1_026D:
        mov    byte ptr [ebp + 0xc], 0
loc_1_0271:
        cmp    byte ptr [ebp + 0xd], al
        jne    loc_1_0305
        cmp    al, 0x45
        jne    loc_1_02AF
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_028A
        mov    dword ptr [ebp + 8], eax
loc_1_028A:
        sub    eax, dword ptr [ebp + 8]
        cmp    eax, 0x1388
        jb     loc_1_0305
        xor    eax, eax
        mov    dword ptr [ebp + 8], eax
        mov    byte ptr [ebp + 0xd], al
        mov    edx, 0x200
        call   0
        mov    byte ptr [ebp + 0xe], 1
        jmp    loc_1_01D2
loc_1_02AF:
        cmp    byte ptr [ebp + 0x13], 0xff
        je     loc_1_02D5
        cmp    al, 0x36
        jne    loc_1_02D5
        cmp    al, byte ptr [ebp + 0xd]
        jne    loc_1_0305
        cmp    byte ptr [ebp + 0x13], 1
        je     loc_1_0305
        mov    byte ptr [ebp + 0x13], 1
        mov    esi, 0x424                    ; -> loc_7_0424
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_0305
loc_1_02D5:
        cmp    al, 0xb6
        je     loc_1_02DB
        jmp    loc_1_02ED
loc_1_02DB:
        mov    esi, 0x8b                     ; -> sub_7_008B
        mov    dl, byte ptr [ebp + 0xf]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    byte ptr [ebp + 0x13], 0
loc_1_02ED:
        cmp    byte ptr [ebp + 0x13], 0xff
        je     loc_1_0305
        btr    dword ptr [ebp + 0x13], 0
        jae    loc_1_0305
        mov    esi, 0x44b                    ; -> loc_7_044B
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_1_0305:
        pop    eax
        mov    byte ptr [ebp + 0xd], al
        test   byte ptr [0x3cd], 0x40        ; -> obj1:0x03CD
        je     loc_1_0330
        test   word ptr [0x3cc], 0x77b       ; -> loc_1_03CC
        je     loc_1_0330
loc_1_031D:
        cmp    dword ptr [ebp], 0
        je     loc_1_0330
        mov    cx, word ptr [ebp + 0x10]
        mov    ah, cl
        pop    ebp
        jmp    dword ptr [0x3b4]             ; -> obj1:0x03B4
loc_1_0330:
        mov    byte ptr [ebp + 0xd], al
        pop    ebp
        jmp    loc_1_03D0
        db  000h, 000h
        dd  VMINI_GetVersion         ; fixup
        dd  VMINI_Update             ; fixup
        dd  VMINI_Status             ; fixup
        dd  VMINI_DisplayError       ; fixup
        dd  VMINI_SetTimeStamp       ; fixup
        dd  VMINI_Siren              ; fixup
        dd  VMINI_RegisterAccess     ; fixup
        dd  VMINI_GetData            ; fixup
        dd  VMINI_ShutDownItem       ; fixup
        dd  VMINI_RegisterSK         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00037h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ENABLE  '                  ; DDB_Name
        dd  038000000h                  ; DDB_Init_Order
        dd  ENABLE_Control           ; DDB_Control_Proc
        dd  ENABLE_V86_API           ; DDB_V86_API_Proc
        dd  ENABLE_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x033C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
sub_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C0:
        add    byte ptr [eax], al
loc_1_03C2:
        add    byte ptr [eax], al
loc_1_03C4:
        add    byte ptr [eax], al
loc_1_03C6:
        add    byte ptr [eax], al
loc_1_03C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D8:
        add    byte ptr [eax], al
loc_1_03DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1ab  R-X
        db  00Ah, 000h, 000h, 000h
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        adc    al, 0
        inc    ebx
        add    byte ptr [ebx], ah
loc_2_0014:
        xchg   ebx, eax
        db  000h, 0B9h, 000h
loc_2_0018:
        in     eax, 0
        db  00Dh, 001h, 033h, 001h
loc_2_001E:
        pop    edi
        add    dword ptr [edi + 0x41000001], eax
        arpl   word ptr [ebx + 0x65], sp
        jae    0x9d
        imul   esp, dword ptr [edx + 0x69], 0x7974696c
        and    byte ptr [ebp + 0x72], al
        jb     0xa5
        jb     loc_2_0038
loc_2_0038:
        dec    eax
        imul   esp, dword ptr [edi + 0x68], 0x746e6f43
        jb     loc_2_00A3
        jae    0xb8
        and    byte ptr [ebx + 0x61], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x6e], ch
        insb   byte ptr es:[edi], dx
        jns    0x6e
        bound  esp, qword ptr [ebp + 0x20]
        jae    0xb8
        je     0x75
        outsw  dx, word ptr [esi]
        jb     0x79
        je     loc_2_00C3
        and    byte ptr gs:[ebx + 0x79], dh
        jae    loc_2_00D5
        insd   dword ptr es:[edi], dx
        and    byte ptr [esi + 0x4d], dl
        add    byte ptr [ebx + 0x6f], al
        jne    0xd7
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_0091
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        and    byte ptr gs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ecx + 0x63], al
        arpl   word ptr [ebp + 0x73], sp
        jae    loc_2_00A3
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebx + 0x6f], al
        jne    0xf7
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_00B1
loc_2_0091:
        imul   ebp, dword ptr [esi + 0x69], 0x6c616974
        imul   edi, dword ptr [edx + 0x65], 0x65687420
        and    byte ptr [ebp + 0x69], cl
        outsb  dx, byte ptr [esi]
loc_2_00A3:
        imul   esp, dword ptr [eax], 0x2c447856
        and    byte ptr [edi + ebp*2 + 0x61], ch
        and    byte ptr fs:[ecx + 0x62], ah
loc_2_00B1:
        outsd  dx, dword ptr [esi]
        jb     loc_2_0128
        add    byte ptr fs:[ebx + 0x6f], al
        jne    loc_2_0127
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_00E1
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_2_00C3:
        popal  
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x6e], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x32]
        push   esi
        pop    eax
        inc    esp
loc_2_00D5:
        and    byte ptr [ebp + 0x6f], ch
        jne    loc_2_0147
        add    byte ptr gs:[ebx + 0x6f], al
        jne    loc_2_014D
loc_2_00E1:
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_0107
        imul   ebp, dword ptr [esi + 0x69], 0x6c616974
        imul   edi, dword ptr [edx + 0x65], 0x65687420
        and    byte ptr [ebp + 0x6e], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x32]
        push   esi
        pop    eax
        inc    esp
        and    byte ptr [ebp + 0x6f], ch
        jne    loc_2_0173
loc_2_0107:
        add    byte ptr gs:[ebx + 0x6f], al
        jne    loc_2_0179
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_0133
        jne    0x183
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x6e], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x32]
        push   esi
loc_2_0127:
        pop    eax
loc_2_0128:
        inc    esp
        and    byte ptr [ebp + 0x6f], ch
        jne    loc_2_019B
        add    byte ptr gs:[ebx + 0x6f], al
loc_2_0133:
        jne    loc_2_01A1
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_015B
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x6e], ah
        popal  
loc_2_0147:
        bound  ebp, qword ptr [ebp + 0x34]
        push   esi
loc_2_014D:
        pop    eax
        inc    esp
        and    byte ptr [ebp + 0x6f], ch
        jne    0x1c1
        add    byte ptr gs:[ebx + 0x6f], al
        jne    0x1c7
loc_2_015B:
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_0181
        imul   ebp, dword ptr [esi + 0x69], 0x6c616974
        imul   edi, dword ptr [edx + 0x65], 0x65687420
        and    byte ptr [ebp + 0x6e], ah
        popal  
loc_2_0173:
        bound  ebp, qword ptr [ebp + 0x34]
        push   esi
loc_2_0179:
        pop    eax
        inc    esp
        and    byte ptr [ebp + 0x6f], ch
        jne    0x1ed
loc_2_0181:
        add    byte ptr gs:[ebx + 0x6f], al
        jne    0x1f3
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x1ad
        jne    0x1fd
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x6e], ah
        popal  
loc_2_019B:
        bound  ebp, qword ptr [ebp + 0x34]
        push   esi
loc_2_01A1:
        pop    eax
        inc    esp
        and    byte ptr [ebp + 0x6f], ch
        jne    0x215
        db  065h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4b8  R-X
loc_3_0000:
        call   sub_3_0116
        jb     loc_3_006F
        call   sub_3_009E
        jb     loc_3_009C
        call   sub_3_0114
        call   sub_3_00E3
        push   0                             ; -> loc_8_0000
        VxDCall  SHELL._SHELL_HookSystemBroadcast ; cd2011001700
        pop    ecx
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    esi, edx
        mov    edi, 0xe8                     ; -> obj5:0x00E8
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x128                    ; -> obj5:0x0128
        mov    ecx, 0xc
        rep movsb byte ptr es:[edi], byte ptr [esi]
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    esi, edx
        mov    edi, 0x13c                    ; -> obj5:0x013C
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x17c                    ; -> obj5:0x017C
        mov    ecx, 0xc
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x4a0                    ; -> loc_3_04A0
        xor    edx, edx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        clc    
        ret    
loc_3_006F:
        xchg   ebx, eax
        movzx  ebx, word ptr [0x12]          ; -> obj2:0x0012
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        xchg   ecx, ebx
        xchg   ebx, eax
        movzx  edi, word ptr [0xc]           ; -> loc_2_000C
        add    edi, dword ptr [4]            ; -> loc_2_0004
        mov    esi, 0x232                    ; -> loc_4_0232
        xor    edx, edx
        xor    eax, eax
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_3_009C:
        stc    
        ret    
sub_3_009E:
        VxDCall  VKD.VKD_Get_Version             ; cd2000000d00
        cmp    eax, 0x20a
        jge    loc_3_00C9
loc_3_00AB:
        and    dword ptr [0x224], 0xfffffffd ; -> obj5:0x0224
        and    dword ptr [0x26e], 0xfffffffd ; -> loc_5_026E
        and    dword ptr [0x244], 0xfffffffd ; -> loc_5_0244
        and    dword ptr [0x23c], 0xfffffffd ; -> loc_5_023C
        stc    
        ret    
loc_3_00C9:
        mov    eax, 0xd0012
        mov    esi, 0xc5                     ; -> loc_1_00C5
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_3_00AB
        mov    dword ptr [0x3d0], esi        ; -> loc_1_03D0
        clc    
        ret    
sub_3_00E3:
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jge    loc_3_00FA
        and    dword ptr [0x244], 0xfffffff1 ; -> loc_5_0244
        clc    
        jmp    loc_3_0113
loc_3_00FA:
        mov    eax, 0xc0003
        mov    esi, 0x98                     ; -> loc_1_0098
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_3_0113
        mov    dword ptr [0x3d4], esi        ; -> loc_1_03D4
        clc    
loc_3_0113:
        ret    
sub_3_0114:
        clc    
        ret    
sub_3_0116:
        xor    ah, ah
        mov    al, byte ptr [0xe6]           ; -> obj5:0x00E6
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0xd8                     ; -> obj5:0x00D8
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    byte ptr [0xe6], al           ; -> obj5:0x00E6
        mov    dword ptr [0x220], 0x18       ; -> obj5:0x0220
        mov    dword ptr [0x238], 8          ; -> loc_5_0238
        mov    dword ptr [0x240], 0x1c       ; -> loc_5_0240
        mov    dword ptr [0x25c], 0xe        ; -> loc_5_025C
        mov    dword ptr [0x26a], 8          ; -> loc_5_026A
        mov    dword ptr [0x272], 0xc        ; -> loc_5_0272
        mov    dword ptr [0x27e], 0x18       ; -> loc_5_027E
        mov    dword ptr [0x296], 0x30       ; -> loc_5_0296
        mov    dword ptr [0x2c6], 0xc        ; -> loc_5_02C6
        or     dword ptr [0x2ca], 0x5e       ; -> loc_5_02CA
        mov    al, byte ptr [0xe6]           ; -> obj5:0x00E6
        or     al, al
        je     loc_3_01C1
        or     dword ptr [0x2ca], 8          ; -> loc_5_02CA
        or     dword ptr [0x244], 8          ; -> loc_5_0244
        or     dword ptr [0x23c], 8          ; -> loc_5_023C
        or     dword ptr [0x224], 8          ; -> obj5:0x0224
        or     dword ptr [0x26e], 8          ; -> loc_5_026E
        jmp    loc_3_01E4
loc_3_01C1:
        and    dword ptr [0x2ca], 0xfffffff7 ; -> loc_5_02CA
        and    dword ptr [0x244], 0xfffffff7 ; -> loc_5_0244
        and    dword ptr [0x23c], 0xfffffff7 ; -> loc_5_023C
        and    dword ptr [0x224], 0xfffffff7 ; -> obj5:0x0224
        and    dword ptr [0x26e], 0xfffffff7 ; -> loc_5_026E
loc_3_01E4:
        or     dword ptr [0x244], 0x3e       ; -> loc_5_0244
        mov    dword ptr [0x248], 0x28       ; -> loc_5_0248
        mov    dword ptr [0x24c], 0x12c      ; -> loc_5_024C
        mov    dword ptr [0x250], 0x28       ; -> loc_5_0250
        and    dword ptr [0x244], 0xffffffbf ; -> loc_5_0244
        and    dword ptr [0x23c], 0xfffffffe ; -> loc_5_023C
        or     dword ptr [0x23c], 0x1e       ; -> loc_5_023C
        and    dword ptr [0x26e], 0xfffffffe ; -> loc_5_026E
        or     dword ptr [0x26e], 0x1fe      ; -> loc_5_026E
        and    dword ptr [0x224], 0xfffffffe ; -> obj5:0x0224
        or     dword ptr [0x224], 0x7e       ; -> obj5:0x0224
        mov    dword ptr [0x228], 0x3e8      ; -> obj5:0x0228
        mov    dword ptr [0x22c], 0x1f4      ; -> obj5:0x022C
        mov    dword ptr [0x230], 0x3e8      ; -> loc_5_0230
        mov    dword ptr [0x234], 0          ; -> loc_5_0234
        or     dword ptr [0x276], 3          ; -> loc_5_0276
        mov    dword ptr [0x27a], 0x493e0    ; -> loc_5_027A
        mov    dword ptr [0x29e], 2          ; -> loc_5_029E
        mov    dword ptr [0x2a2], 0x1f4      ; -> loc_5_02A2
        mov    dword ptr [0x2a6], 0          ; -> loc_5_02A6
        mov    dword ptr [0x2aa], 3          ; -> loc_5_02AA
        mov    dword ptr [0x2ae], 0x1f4      ; -> loc_5_02AE
        mov    dword ptr [0x2b2], 0          ; -> loc_5_02B2
        mov    dword ptr [0x2b6], 1          ; -> loc_5_02B6
        mov    dword ptr [0x2ba], 0x1f4      ; -> loc_5_02BA
        mov    dword ptr [0x2be], 0          ; -> loc_5_02BE
        mov    dword ptr [0x2c2], 0          ; -> loc_5_02C2
        or     dword ptr [0x29a], 2          ; -> loc_5_029A
        mov    dword ptr [0x28a], 0          ; -> loc_5_028A
        mov    dword ptr [0x282], 0xa        ; -> loc_5_0282
        mov    dword ptr [0x28e], 0x12c      ; -> loc_5_028E
        mov    dword ptr [0x292], 1          ; -> loc_5_0292
        mov    eax, dword ptr [0x200]        ; -> obj5:0x0200
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x4d                     ; -> obj5:0x004D
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    dword ptr [0x200], eax        ; -> obj5:0x0200
        mov    ax, word ptr [0x216]          ; -> obj5:0x0216
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x5d                     ; -> obj5:0x005D
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    word ptr [0x216], ax          ; -> obj5:0x0216
        mov    eax, dword ptr [0x204]        ; -> obj5:0x0204
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x6e                     ; -> obj5:0x006E
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    dword ptr [0x204], eax        ; -> obj5:0x0204
        mov    ax, word ptr [0x218]          ; -> obj5:0x0218
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x80                     ; -> obj5:0x0080
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    word ptr [0x218], ax          ; -> obj5:0x0218
        mov    eax, dword ptr [0x20c]        ; -> obj5:0x020C
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x8e                     ; -> obj5:0x008E
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    dword ptr [0x20c], eax        ; -> obj5:0x020C
        mov    ax, word ptr [0x21c]          ; -> obj5:0x021C
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x9f                     ; -> obj5:0x009F
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    word ptr [0x21c], ax          ; -> obj5:0x021C
        mov    eax, dword ptr [0x208]        ; -> obj5:0x0208
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0xa8                     ; -> obj5:0x00A8
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    dword ptr [0x208], eax        ; -> obj5:0x0208
        mov    ax, word ptr [0x21a]          ; -> obj5:0x021A
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0xb8                     ; -> obj5:0x00B8
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    word ptr [0x21a], ax          ; -> obj5:0x021A
        mov    eax, dword ptr [0x210]        ; -> obj5:0x0210
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0xca                     ; -> obj5:0x00CA
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    dword ptr [0x210], eax        ; -> obj5:0x0210
        mov    ax, word ptr [0x21e]          ; -> obj5:0x021E
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0xc0                     ; -> obj5:0x00C0
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    word ptr [0x21e], ax          ; -> obj5:0x021E
        mov    eax, dword ptr [0x1fc]        ; -> obj5:0x01FC
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x37                     ; -> obj5:0x0037
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    dword ptr [0x1fc], eax        ; -> obj5:0x01FC
        mov    ax, word ptr [0x214]          ; -> obj5:0x0214
        mov    esi, 0x30                     ; -> obj5:0x0030
        mov    edi, 0x44                     ; -> obj5:0x0044
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        mov    word ptr [0x214], ax          ; -> obj5:0x0214
        mov    esi, 0x3b4                    ; -> obj1:0x03B4
        xor    eax, eax
        mov    dword ptr [esi], 0x96         ; -> loc_1_0096
        mov    dword ptr [esi + 4], 0x21     ; -> loc_1_0021
        mov    dword ptr [esi + 8], eax
        mov    byte ptr [esi + 0xc], al
        mov    byte ptr [esi + 0xd], al
        mov    byte ptr [esi + 0xe], al
        mov    byte ptr [esi + 0xf], al
        mov    word ptr [esi + 0x10], ax
        mov    byte ptr [esi + 0x12], 0
        mov    byte ptr [esi + 0x13], al
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x20], eax
        mov    byte ptr [esi + 0x24], al
        mov    byte ptr [esi + 0x25], 0xe0
        mov    byte ptr [esi + 0x26], 0x45
        mov    byte ptr [esi + 0x27], 0xc5
        mov    byte ptr [esi + 0x29], al
        mov    byte ptr [esi + 0x2a], al
        clc    
        ret    
        db  000h, 000h
loc_3_04A0:
        push   1
        push   0
        push   0x1452                        ; -> sub_4_1452
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        mov    dword ptr [0x8c8], eax        ; -> loc_5_08C8
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x182c  R-X
loc_4_0000:
        mov    byte ptr [0x3d8], 0           ; -> loc_1_03D8
        ret    
loc_4_0008:
        cmp    byte ptr [0x363], 0           ; -> loc_5_0363
        jne    loc_4_002A
        mov    edx, 0x13c                    ; -> obj5:0x013C
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_4_002C
        mov    byte ptr [0x363], 1           ; -> loc_5_0363
loc_4_002A:
        clc    
        ret    
loc_4_002C:
        movzx  eax, word ptr [0x1a]          ; -> obj2:0x001A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        call   VMINI_DisplayError
        stc    
        ret    
loc_4_0046:
        cmp    byte ptr [0x363], 0           ; -> loc_5_0363
        je     loc_4_0068
        mov    edx, 0x188                    ; -> obj5:0x0188
        mov    ebx, 0xffffffff
        VxDCall  APM.Service_0002h               ; cd2002002700
        jb     loc_4_006A
        mov    byte ptr [0x363], 0           ; -> loc_5_0363
loc_4_0068:
        clc    
        ret    
loc_4_006A:
        movzx  eax, word ptr [0x1e]          ; -> loc_2_001E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        call   VMINI_DisplayError
        stc    
        ret    
loc_4_0084:
        push   ebx
        test   byte ptr [0x3cd], 0x40        ; -> obj1:0x03CD
        je     loc_4_0102
        test   byte ptr [0x3cd], 0x80        ; -> obj1:0x03CD
        je     loc_4_00D5
        mov    dword ptr [0x3b4], 0x21       ; -> obj1:0x03B4
        mov    edx, 0x134                    ; -> obj5:0x0134
        mov    ebx, 0xffffffff
        VxDCall  APM.Service_0002h               ; cd2002002700
        jb     loc_4_00E1
        and    dword ptr [0x3cc], 0xffffbfff ; -> loc_1_03CC
        mov    eax, 0x96                     ; -> loc_1_0096
        mov    dword ptr [0x3b4], eax        ; -> obj1:0x03B4
        push   esi
        mov    esi, dword ptr [0x356]        ; -> obj5:0x0356
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
loc_4_00D5:
        mov    byte ptr [0xe7], 0            ; -> obj5:0x00E7
        jmp    loc_4_0166
loc_4_00E1:
        movzx  eax, word ptr [0x18]          ; -> loc_2_0018
        add    eax, dword ptr [4]            ; -> loc_2_0004
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        call   VMINI_DisplayError
        mov    byte ptr [0xe7], 0            ; -> obj5:0x00E7
        jmp    loc_4_0166
loc_4_0102:
        mov    edx, 0xe8                     ; -> obj5:0x00E8
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_4_012A
        or     dword ptr [0x3cc], 0xc000     ; -> loc_1_03CC
        call   VMINI_SetTimeStamp
        call   sub_7_0000
        jmp    loc_4_0166
loc_4_012A:
        mov    byte ptr [0x3d8], 1           ; -> loc_1_03D8
        and    dword ptr [0x3cc], 0xffffbfff ; -> loc_1_03CC
        movzx  eax, word ptr [0x14]          ; -> loc_2_0014
        add    eax, dword ptr [4]            ; -> loc_2_0004
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        call   VMINI_DisplayError
        push   esi
        mov    ebx, dword ptr [0x3cc]        ; -> loc_1_03CC
        and    ebx, 0x77b
        call   VMINI_ShutDownItem
        pop    esi
loc_4_0166:
        pop    ebx
        ret    
VMINI_GetVersion:
        mov    eax, 0x400
        clc    
        ret    
VMINI_RegisterAccess:
        mov    eax, dword ptr [edi]
        mov    dword ptr [0x3b4], eax        ; -> obj1:0x03B4
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [0x35e], eax        ; -> sub_5_035E
        mov    eax, dword ptr [edi + 0x28]
        mov    dword ptr [0x3b8], eax        ; -> sub_1_03B8
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [0x2e2], eax        ; -> obj5:0x02E2
        mov    eax, dword ptr [0x3d0]        ; -> loc_1_03D0
        mov    dword ptr [edi + 0x18], eax
        mov    eax, 0x51a                    ; -> sub_4_051A
        mov    dword ptr [edi + 0x20], eax
        mov    eax, 0x850                    ; -> obj5:0x0850
        mov    dword ptr [edi + 0x2c], eax
        mov    eax, 0x2e6                    ; -> obj5:0x02E6
        mov    dword ptr [edi + 0x30], eax
        mov    eax, 0x2ea                    ; -> obj5:0x02EA
        mov    dword ptr [edi + 0x34], eax
        mov    eax, 0x810                    ; -> obj5:0x0810
        mov    dword ptr [edi + 0x38], eax
        mov    eax, 0x504                    ; -> obj5:0x0504
        mov    dword ptr [edi + 0x3c], eax
        or     dword ptr [0x3cc], 0x4000     ; -> loc_1_03CC
        ret    
VMINI_RegisterSK:
        cmp    dword ptr [edi], 0x14
        jb     loc_4_01F8
        mov    dword ptr [edi], 0x14
        mov    dword ptr [edi + 4], 0x27e    ; -> loc_5_027E
        mov    dword ptr [edi + 8], 0x3cc    ; -> loc_1_03CC
        mov    dword ptr [edi + 0xc], 0x608  ; -> obj5:0x0608
        mov    dword ptr [edi + 0x10], 0x70c ; -> obj5:0x070C
loc_4_01F8:
        ret    
loc_4_01F9:
        mov    ecx, dword ptr [esp + 4]
        pushal 
        xchg   ebx, eax
        movzx  edi, word ptr [0xc]           ; -> loc_2_000C
        add    edi, dword ptr [4]            ; -> loc_2_0004
        mov    esi, 0x232                    ; -> loc_4_0232
        xor    edx, edx
        mov    eax, 0x10001000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        mov    eax, 0x1f4
        xor    edx, edx
        mov    esi, 0                        ; -> loc_4_0000
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        popal  
        ret    
loc_4_0232:
        ret    
loc_4_0233:
        mov    ecx, dword ptr [esp + 4]
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    eax, 0x1b3                    ; -> obj5:0x01B3
        xor    edx, edx
        cmp    ecx, edx
        je     loc_4_0257
        add    eax, 0xffffffe9
loc_4_0257:
        push   edx
        push   edx
        push   eax
        push   0x190                         ; -> obj5:0x0190
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
sub_4_0279:
        sub    byte ptr [eax], dl
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        add    al, byte ptr [eax]
        add    byte ptr [0x4b000010], dh     ; -> loc_4_1035
        adc    byte ptr [eax], al
        add    byte ptr [eax], cl            ; -> loc_4_1108
        adc    dword ptr [eax], eax
        db  000h
ENABLE_PM_API:
        or     byte ptr [ebp + 0x2c], 1
        xor    eax, eax
        mov    ax, word ptr [ebp + 0x1c]
        cmp    eax, 5
        jae    loc_4_02AB
        call   sub_4_0279
        jb     loc_4_02AB
        and    byte ptr [ebp + 0x2c], 0xfe
loc_4_02AB:
        ret    
loc_4_02AC:
        mov    ebx, dword ptr [ebp + 0x10]
        xor    eax, eax
        shr    ebx, 1
        rcr    ax, 1
        bsf    ebx, ebx
        movzx  ebx, byte ptr [ebx + 0x364]   ; -> obj5:0x0364
        bts    eax, ebx
        xchg   ebx, eax
        push   ebx
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        call   sub_4_112C
        pop    ebx
        test   bh, 0x10
        je     loc_4_030F
        test   byte ptr [0x276], 1           ; -> loc_5_0276
        je     loc_4_030F
        mov    eax, dword ptr [0x356]        ; -> obj5:0x0356
        test   eax, eax
        je     loc_4_02F9
        mov    esi, dword ptr [0x356]        ; -> obj5:0x0356
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_4_02F9:
        mov    eax, dword ptr [0x27a]        ; -> loc_5_027A
        mov    esi, 0                        ; -> sub_7_0000
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x356], esi        ; -> obj5:0x0356
loc_4_030F:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_4_031A
        call   sub_4_16B6
loc_4_031A:
        ret    
ENABLE_V86_API:
        xor    eax, eax
        mov    ax, word ptr [ebp + 0x1c]
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    eax, 5
        jae    loc_4_0332
        jmp    sub_4_0279
loc_4_0332:
        or     word ptr [ebp + 0x2c], 1
        ret    
sub_4_0338:
        test   byte ptr [0x3cc], 1           ; -> loc_1_03CC
        je     loc_4_0346
        call   sub_4_0910
loc_4_0346:
        test   byte ptr [0x3cc], 4           ; -> loc_1_03CC
        je     loc_4_0354
        call   sub_4_0B57
loc_4_0354:
        test   byte ptr [0x3cc], 8           ; -> loc_1_03CC
        je     loc_4_0362
        call   sub_4_0CAB
loc_4_0362:
        test   byte ptr [0x3cc], 0x40        ; -> loc_1_03CC
        je     loc_4_0370
        call   sub_4_0AAA
loc_4_0370:
        test   byte ptr [0x3cd], 1           ; -> obj1:0x03CD
        je     loc_4_037E
        call   sub_4_0883
loc_4_037E:
        test   byte ptr [0x3cd], 2           ; -> obj1:0x03CD
        je     loc_4_038C
        call   sub_4_09C3
loc_4_038C:
        test   byte ptr [0x3cd], 8           ; -> obj1:0x03CD
        je     loc_4_039A
        call   sub_4_0C05
loc_4_039A:
        ret    
VMINI_GetData:
        jmp    sub_4_112C
sub_4_03A0:
        bsf    eax, ebx
        je     loc_4_03B0
        cmp    al, 0xd
        ja     loc_4_03B0
        jmp    dword ptr [eax*4 + 0x31e]     ; -> obj5:0x031E
loc_4_03B0:
        clc    
        ret    
sub_4_03B2:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_03CC
        push   0
        push   eax
        push   0x233                         ; -> loc_4_0233
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        ret    
loc_4_03CC:
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        call   VMINI_DisplayError
        xor    eax, eax
        ret    
loc_4_03E1:
        mov    dword ptr [0x374], edx        ; -> obj5:0x0374
        cmp    byte ptr [0x850], 0           ; -> obj5:0x0850
        je     loc_4_03FB
        mov    edx, dword ptr [0x3cc]        ; -> loc_1_03CC
        call   sub_4_051A
loc_4_03FB:
        xchg   edx, eax
        test   eax, 0xfffff7ff
        jne    loc_4_040C
        test   ah, 8
        jne    loc_4_04A8
loc_4_040C:
        test   al, 0x80
        je     loc_4_043D
        test   byte ptr [0x3cc], 0x80        ; -> loc_1_03CC
        je     loc_4_042C
        push   0
        push   eax
        push   8                             ; -> loc_4_0008
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        jmp    loc_4_043D
loc_4_042C:
        push   0
        push   eax
        push   0x46                          ; -> loc_4_0046
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
loc_4_043D:
        test   word ptr [0x3cc], 0x77b       ; -> loc_1_03CC
        jne    loc_4_046C
        test   byte ptr [0x3cd], 0x40        ; -> obj1:0x03CD
        je     loc_4_04A8
        cmp    byte ptr [0x3d8], 1           ; -> loc_1_03D8
        je     loc_4_04A8
        cmp    byte ptr [0xe7], 1            ; -> obj5:0x00E7
        je     loc_4_04A8
        mov    byte ptr [0xe7], 1            ; -> obj5:0x00E7
        jmp    loc_4_0490
loc_4_046C:
        cmp    byte ptr [0x3d8], 1           ; -> loc_1_03D8
        je     loc_4_04A8
        test   byte ptr [0x3cd], 0x40        ; -> obj1:0x03CD
        jne    loc_4_04A8
        test   byte ptr [0x11c7], 1          ; -> obj5:0x11C7
        je     loc_4_0490
        test   byte ptr [0x11c7], 2          ; -> obj5:0x11C7
        jne    loc_4_04A8
loc_4_0490:
        push   0
        push   eax
        push   0x84                          ; -> loc_4_0084
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        or     byte ptr [0x11c7], 2          ; -> obj5:0x11C7
loc_4_04A8:
        mov    edx, dword ptr [0x374]        ; -> obj5:0x0374
        test   word ptr [0x3cc], 0x77b       ; -> loc_1_03CC
        jne    loc_4_04C5
        and    dword ptr [0x3cc], 0xfffeffff ; -> loc_1_03CC
        jmp    loc_4_04FD
loc_4_04C5:
        test   dword ptr [0x3cc], edx        ; -> loc_1_03CC
        je     loc_4_04FD
        bsf    eax, edx
        mov    esi, dword ptr [eax*4 + 0x39a] ; -> obj5:0x039A
        mov    esi, dword ptr [esi]
        test   dword ptr [eax*4], esi        ; -> obj5:0x0000
        je     loc_4_04FD
        or     dword ptr [0x3cc], 0x10000    ; -> loc_1_03CC
        push   0
        push   eax
        push   0x4ff                         ; -> loc_4_04FF
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
loc_4_04FD:
        clc    
        ret    
loc_4_04FF:
        push   ebx
        xor    edx, edx
        mov    eax, 0x1e3                    ; -> obj5:0x01E3
        push   edx
        push   edx
        push   eax
        push   0x190                         ; -> obj5:0x0190
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        pop    ebx
        ret    
sub_4_051A:
        pushal 
        xor    ebx, ebx
        cmp    byte ptr [0x850], bl          ; -> obj5:0x0850
        je     loc_4_0562
        mov    dword ptr [0x2ea], edx        ; -> obj5:0x02EA
loc_4_052B:
        cmp    dword ptr [ebx*8 + 0x810], 0xffff ; -> obj5:0x0810
        je     loc_4_055C
        mov    eax, dword ptr [ebx*8 + 0x810] ; -> obj5:0x0810
        mov    ecx, dword ptr [ebx*8 + 0x814] ; -> obj5:0x0814
        push   ebx
        push   edx
        push   0
        push   0
        push   edx
        push   0
        push   ecx
        push   eax
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        pop    edx
        pop    ebx
loc_4_055C:
        inc    ebx
        cmp    ebx, 8
        jl     loc_4_052B
loc_4_0562:
        popal  
        ret    
        db  060h, 033h, 0DBh, 038h, 01Dh
        dd  obj5:0x0850              ; fixup
        db  074h, 03Dh, 089h, 015h
        dd  obj5:0x02E6              ; fixup
        db  081h, 03Ch, 0DDh
        dd  obj5:0x0810              ; fixup
        db  0FFh, 0FFh, 000h, 000h, 074h, 024h, 08Bh, 004h, 0DDh
        dd  obj5:0x0810              ; fixup
        db  08Bh, 00Ch, 0DDh
        dd  obj5:0x0814              ; fixup
        db  053h, 052h, 06Ah, 000h, 06Ah, 000h, 052h, 06Ah, 001h, 051h, 050h, 0CDh, 020h, 006h, 000h, 017h
        db  000h, 083h, 0C4h, 018h, 05Ah, 05Bh, 043h, 083h, 0FBh, 008h, 07Ch, 0C9h, 061h, 0C3h, 000h, 000h
loc_4_05B0:
        clc    
        ret    
loc_4_05B2:
        mov    dword ptr [0x45e], edx        ; -> loc_5_045E
        xor    ecx, ecx
        bsf    eax, edx
        je     loc_4_05D6
        push   ebx
        mov    ebx, dword ptr [eax*4 + 0x3ca] ; -> obj5:0x03CA
        test   ebx, ebx
        je     loc_4_05D6
        test   dword ptr [ebx], 4
        jne    loc_4_05D6
        pop    ebx
        clc    
        ret    
loc_4_05D6:
        xor    ecx, ecx
        mov    ebx, dword ptr [eax*4 + 0x39a] ; -> obj5:0x039A
        test   byte ptr [ebx], 8
        je     loc_4_05EB
        mov    cx, word ptr [ebx]
        and    cx, 8
loc_4_05EB:
        mov    byte ptr [0xe6], cl           ; -> obj5:0x00E6
        mov    byte ptr [0x463], 0           ; -> obj5:0x0463
        mov    ebx, dword ptr [eax*4 + 0x3fa] ; -> obj5:0x03FA
        test   ebx, ebx
        je     loc_4_060F
        test   byte ptr [ebx], 0x10
        je     loc_4_060F
        mov    byte ptr [0x463], 1           ; -> obj5:0x0463
loc_4_060F:
        VxDCall  VMD.VMD_Get_Mouse_Owner         ; cd2002000c00
        mov    dword ptr [0x469], ebx        ; -> obj5:0x0469
        pop    ebx
        push   ebx
        mov    byte ptr [0x464], 0           ; -> loc_5_0464
        test   dword ptr [0x3cc], edx        ; -> loc_1_03CC
        jne    loc_4_069D
        mov    ecx, dword ptr [eax*4 + 0x42a] ; -> obj5:0x042A
        mov    dword ptr [0x45a], ecx        ; -> sub_5_045A
        cmp    byte ptr [0x463], 0           ; -> obj5:0x0463
        je     loc_4_0660
        push   ebx
        push   edx
        mov    ecx, 1
        mov    byte ptr [0x464], cl          ; -> loc_5_0464
        mov    eax, 1
        mov    ebx, 0xc8
        call   sub_4_0E78
        pop    edx
        pop    ebx
loc_4_0660:
        cmp    byte ptr [0xe6], 0            ; -> obj5:0x00E6
        je     loc_4_068F
        mov    dword ptr [0x465], ebx        ; -> obj5:0x0465
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    byte ptr [0x3dd], 1           ; -> obj1:0x03DD
        push   0
        push   edx
        push   0x6c2                         ; -> loc_4_06C2
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        pop    ebx
        ret    
loc_4_068F:
        bsf    eax, edx
        je     loc_4_06C1
        mov    ecx, dword ptr [eax*4 + 0x42a] ; -> obj5:0x042A
        jmp    loc_4_06A9
loc_4_069D:
        bsf    eax, edx
        je     loc_4_06C1
        mov    ecx, dword ptr [eax*4 + 0x2ee] ; -> obj5:0x02EE
loc_4_06A9:
        mov    dword ptr [0x45a], ecx        ; -> sub_5_045A
        pop    ebx
        or     ecx, ecx
        je     loc_4_06C1
        call   ecx
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
loc_4_06C1:
        ret    
loc_4_06C2:
        test   byte ptr [esp + 5], 0x40
        je     loc_4_06CF
        call   sub_1_03B8
loc_4_06CF:
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [0x46d], cx          ; -> obj5:0x046D
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    eax, 0x384                    ; -> obj5:0x0384
        push   0x46d                         ; -> obj5:0x046D
        push   2
        push   eax
        push   0x190                         ; -> obj5:0x0190
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        mov    byte ptr [0x3dd], 0           ; -> obj1:0x03DD
        cmp    eax, 0x64
        jb     loc_4_071F
        mov    edx, dword ptr [esp + 0x70]
        call   sub_4_07BE
        mov    eax, 2
loc_4_071F:
        cmp    eax, 2
        jne    loc_4_072E
        mov    dword ptr [0x45a], 0          ; -> sub_5_045A
loc_4_072E:
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        cmp    dword ptr [0x45a], 0          ; -> sub_5_045A
        jne    loc_4_076D
        mov    byte ptr [0x464], 0           ; -> loc_5_0464
        cmp    byte ptr [0x463], 0           ; -> obj5:0x0463
        je     loc_4_076B
        mov    eax, 2
        mov    ebx, 0xc8
        mov    ecx, 1
        call   sub_4_0E78
loc_4_076B:
        jmp    loc_4_0782
loc_4_076D:
        push   esi
        push   edi
        call   sub_5_045A
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
        pop    edi
        pop    esi
loc_4_0782:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    ebx, dword ptr [0x465]        ; -> obj5:0x0465
        je     loc_4_07BD
        cmp    dword ptr [0x469], ebx        ; -> obj5:0x0469
        je     loc_4_07BD
        mov    ebx, dword ptr [0x465]        ; -> obj5:0x0465
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_4_07BD
        mov    eax, 0xf
        mov    ebx, dword ptr [0x465]        ; -> obj5:0x0465
        xor    esi, esi
        xor    edi, edi
        xor    edx, edx
        VMMCall  VMM.System_Control              ; cd2093000100
loc_4_07BD:
        ret    
sub_4_07BE:
        pushal 
        test   dword ptr [0x45e], 1          ; -> loc_5_045E
        je     loc_4_07DF
        and    dword ptr [0x26e], 0xfffffffb ; -> loc_5_026E
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
        jmp    loc_4_085D
loc_4_07DF:
        test   dword ptr [0x45e], 0x40       ; -> loc_5_045E
        je     loc_4_07FF
        and    dword ptr [0x244], 0xfffffffb ; -> loc_5_0244
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
        jmp    loc_4_085D
loc_4_07FF:
        test   dword ptr [0x45e], 0x100      ; -> loc_5_045E
        je     loc_4_081F
        and    dword ptr [0x224], 0xfffffffb ; -> obj5:0x0224
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
        jmp    loc_4_085D
loc_4_081F:
        test   dword ptr [0x45e], 0x200      ; -> loc_5_045E
        je     loc_4_083F
        and    dword ptr [0x23c], 0xfffffffb ; -> loc_5_023C
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
        jmp    loc_4_085D
loc_4_083F:
        test   dword ptr [0x45e], 0x800      ; -> loc_5_045E
        je     loc_4_085D
        and    dword ptr [0x2ca], 0xfffffffb ; -> loc_5_02CA
        mov    ebx, dword ptr [0x45e]        ; -> loc_5_045E
        call   sub_4_16B6
loc_4_085D:
        popal  
        ret    
loc_4_085F:
        or     dword ptr [0x224], 1          ; -> obj5:0x0224
        or     dword ptr [0x3cc], 0x100      ; -> loc_1_03CC
        mov    eax, 0x100
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        clc    
        ret    
sub_4_0883:
        and    dword ptr [0x224], 0xfffffffe ; -> obj5:0x0224
        and    dword ptr [0x3cc], 0xfffffeff ; -> loc_1_03CC
        mov    eax, 0x100
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_08A1:
        test   dword ptr [0x224], 1          ; -> obj5:0x0224
        jne    loc_4_08C5
        test   dword ptr [0x3cc], 0x100      ; -> loc_1_03CC
        jne    sub_4_0883
        mov    eax, 0x100
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_08C5:
        test   dword ptr [0x3cc], 0x100      ; -> loc_1_03CC
        je     loc_4_085F
        mov    eax, 0x100
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_08DD:
        or     dword ptr [0x26e], 1          ; -> loc_5_026E
        or     dword ptr [0x3cc], 1          ; -> loc_1_03CC
        mov    eax, 1
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        mov    word ptr [0x3c4], ax          ; -> loc_1_03C4
        clc    
        ret    
sub_4_0910:
        cmp    dword ptr [0x35e], 0          ; -> sub_5_035E
        je     loc_4_0924
        mov    eax, 1
        call   sub_5_035E
loc_4_0924:
        test   dword ptr [0x26e], 0x10       ; -> loc_5_026E
        je     loc_4_0944
        mov    eax, 2
        mov    ebx, 0xc8
        mov    ecx, 1
        call   sub_4_0E78
loc_4_0944:
        and    dword ptr [0x26e], 0xfffffffe ; -> loc_5_026E
        and    dword ptr [0x3cc], 0xfffffffe ; -> loc_1_03CC
        mov    eax, 1
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_095F:
        test   dword ptr [0x26e], 1          ; -> loc_5_026E
        jne    loc_4_0983
        test   dword ptr [0x3cc], 1          ; -> loc_1_03CC
        jne    sub_4_0910
        mov    eax, 1
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0983:
        test   dword ptr [0x3cc], 1          ; -> loc_1_03CC
        je     loc_4_08DD
        mov    eax, 1
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_099F:
        or     dword ptr [0x3cc], 0x200      ; -> loc_1_03CC
        or     dword ptr [0x23c], 1          ; -> loc_5_023C
        mov    eax, 0x200
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        clc    
        ret    
sub_4_09C3:
        test   dword ptr [0x23c], 0x10       ; -> loc_5_023C
        je     loc_4_09E3
        mov    eax, 2
        mov    ebx, 0xc8
        mov    ecx, 1
        call   sub_4_0E78
loc_4_09E3:
        and    dword ptr [0x3cc], 0xfffffdff ; -> loc_1_03CC
        and    dword ptr [0x23c], 0xfffffffe ; -> loc_5_023C
        mov    eax, 0x200
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_0A01:
        test   dword ptr [0x23c], 1          ; -> loc_5_023C
        jne    loc_4_0A25
        test   dword ptr [0x3cc], 0x200      ; -> loc_1_03CC
        jne    sub_4_09C3
        mov    eax, 0x200
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0A25:
        test   dword ptr [0x3cc], 0x200      ; -> loc_1_03CC
        je     loc_4_099F
        mov    eax, 0x200
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0A41:
        cmp    byte ptr [0x473], 1           ; -> loc_5_0473
        je     loc_4_0A82
        mov    eax, dword ptr [0x244]        ; -> loc_5_0244
        and    eax, 0x80
        je     loc_4_0A6A
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        test   eax, 0x20
        je     loc_4_0A82
        call   sub_7_0475
        jmp    loc_4_0A82
loc_4_0A6A:
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        test   eax, 0x20
        jne    loc_4_0A82
        mov    esi, 0x475                    ; -> sub_7_0475
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_4_0A82:
        or     dword ptr [0x244], 1          ; -> loc_5_0244
        or     dword ptr [0x3cc], 0x40       ; -> loc_1_03CC
        mov    eax, 0x40
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        mov    byte ptr [0x473], 0           ; -> loc_5_0473
        clc    
        ret    
sub_4_0AAA:
        test   dword ptr [0x244], 0x10       ; -> loc_5_0244
        je     loc_4_0ACA
        mov    eax, 2
        mov    ebx, 0xc8
        mov    ecx, 1
        call   sub_4_0E78
loc_4_0ACA:
        and    dword ptr [0x244], 0xfffffffe ; -> loc_5_0244
        and    dword ptr [0x3cc], 0xffffffbf ; -> loc_1_03CC
        mov    eax, 0x40
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_0AE5:
        test   dword ptr [0x244], 1          ; -> loc_5_0244
        jne    loc_4_0B09
        test   dword ptr [0x3cc], 0x40       ; -> loc_1_03CC
        jne    sub_4_0AAA
        mov    eax, 0x40
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0B09:
        mov    byte ptr [0x473], 1           ; -> loc_5_0473
        test   dword ptr [0x3cc], 0x40       ; -> loc_1_03CC
        je     loc_4_0A41
        mov    byte ptr [0x473], 0           ; -> loc_5_0473
        mov    eax, 0x40
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0B33:
        mov    dword ptr [0x260], 0xffffffff ; -> loc_5_0260
        or     dword ptr [0x3cc], 4          ; -> loc_1_03CC
        mov    eax, 4
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        clc    
        ret    
sub_4_0B57:
        mov    dword ptr [0x260], 0          ; -> loc_5_0260
        and    dword ptr [0x3cc], 0xfffffffb ; -> loc_1_03CC
        mov    eax, 4
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_0B75:
        cmp    dword ptr [0x260], 0          ; -> loc_5_0260
        jne    loc_4_0B96
        test   dword ptr [0x3cc], 4          ; -> loc_1_03CC
        jne    sub_4_0B57
        mov    eax, 4
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0B96:
        test   dword ptr [0x3cc], 4          ; -> loc_1_03CC
        je     loc_4_0B33
        mov    eax, 4
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0BAE:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        inc    eax
        call   sub_4_03B2
        or     byte ptr [0x2ca], 1           ; -> loc_5_02CA
        or     byte ptr [0x3cd], 8           ; -> obj1:0x03CD
        mov    eax, 0x800
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        test   byte ptr [0x276], 1           ; -> loc_5_0276
        je     loc_4_0BFD
        mov    eax, dword ptr [0x356]        ; -> obj5:0x0356
        test   eax, eax
        jne    loc_4_0BFD
        mov    eax, dword ptr [0x27a]        ; -> loc_5_027A
        mov    esi, 0                        ; -> sub_7_0000
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x356], esi        ; -> obj5:0x0356
loc_4_0BFD:
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        clc    
        ret    
sub_4_0C05:
        test   dword ptr [0x2ca], 0x10       ; -> loc_5_02CA
        je     loc_4_0C25
        mov    eax, 2
        mov    ebx, 0xc8
        mov    ecx, 1
        call   sub_4_0E78
loc_4_0C25:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0
        call   sub_4_03B2
        and    dword ptr [0x2ca], 0xfffffffe ; -> loc_5_02CA
        and    dword ptr [0x3cc], 0xfffff7ff ; -> loc_1_03CC
        mov    eax, 0x800
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_0C53:
        test   byte ptr [0x2ca], 1           ; -> loc_5_02CA
        jne    loc_4_0C71
        test   byte ptr [0x3cd], 8           ; -> obj1:0x03CD
        jne    sub_4_0C05
        mov    eax, 0x800
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0C71:
        test   byte ptr [0x3cd], 8           ; -> obj1:0x03CD
        je     loc_4_0BAE
        mov    eax, 0x800
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0C8A:
        or     dword ptr [0x29a], 1          ; -> loc_5_029A
        or     dword ptr [0x3cc], 8          ; -> loc_1_03CC
        mov    eax, 8
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        clc    
        ret    
sub_4_0CAB:
        and    dword ptr [0x29a], 0xfffffffe ; -> loc_5_029A
        and    dword ptr [0x3cc], 0xfffffff7 ; -> loc_1_03CC
        mov    eax, 8
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_0CC6:
        test   dword ptr [0x29a], 1          ; -> loc_5_029A
        jne    loc_4_0CEA
        test   dword ptr [0x3cc], 8          ; -> loc_1_03CC
        jne    sub_4_0CAB
        mov    eax, 8
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0CEA:
        test   dword ptr [0x3cc], 8          ; -> loc_1_03CC
        je     loc_4_0C8A
        mov    eax, 8
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
sub_4_0D02:
        or     byte ptr [0x282], 1           ; -> loc_5_0282
        or     byte ptr [0x3cc], 0x80        ; -> loc_1_03CC
        mov    eax, 0x80
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        VxDCall  ENABLE.VMINI_SetTimeStamp       ; cd2004003700
        clc    
        ret    
sub_4_0D23:
        and    byte ptr [0x282], 0xfe        ; -> loc_5_0282
        and    byte ptr [0x3cc], 0x7f        ; -> loc_1_03CC
        mov    eax, 0x80
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        clc    
        ret    
loc_4_0D3E:
        test   byte ptr [0x282], 1           ; -> loc_5_0282
        jne    loc_4_0D5C
        test   byte ptr [0x3cc], 0x80        ; -> loc_1_03CC
        jne    sub_4_0D23
        mov    eax, 0x80
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
loc_4_0D5C:
        test   byte ptr [0x3cc], 0x80        ; -> loc_1_03CC
        je     sub_4_0D02
        mov    eax, 0x80
        VxDCall  ENABLE.VMINI_Update             ; cd2001003700
        ret    
        db  000h, 000h, 000h
loc_4_0D74:
        cmp    byte ptr [0x4fc], 0           ; -> obj5:0x04FC
        je     loc_4_0DFB
        cmp    dx, 0
        je     loc_4_0DCA
        sub    word ptr [0x4f8], 3           ; -> obj5:0x04F8
        cmp    word ptr [0x4f8], 0           ; -> obj5:0x04F8
        jle    loc_4_0DCA
        add    word ptr [0x4fa], dx          ; -> obj5:0x04FA
        mov    ax, word ptr [0x4fa]          ; -> obj5:0x04FA
        out    0x42, al
        mov    al, ah
        jmp    loc_4_0DA8
loc_4_0DA8:
        out    0x42, al
        jmp    loc_4_0DAC
loc_4_0DAC:
        in     al, 0x61
        or     al, 3
        out    0x61, al
        mov    eax, 3
        mov    esi, 0xd74                    ; -> loc_4_0D74
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    byte ptr [0x4fc], 0xff        ; -> obj5:0x04FC
        ret    
loc_4_0DCA:
        call   sub_4_100D
        dec    byte ptr [0x4fd]              ; -> loc_5_04FD
        cmp    byte ptr [0x4fd], 0           ; -> loc_5_04FD
        je     loc_4_0DFB
        xor    eax, eax
        xor    ebx, ebx
        xor    ecx, ecx
        mov    al, byte ptr [0x500]          ; -> obj5:0x0500
        mov    bx, word ptr [0x4fe]          ; -> obj5:0x04FE
        mov    cl, byte ptr [0x4fd]          ; -> loc_5_04FD
        jmp    loc_4_0EE4
loc_4_0DFB:
        cmp    dword ptr [0x474], 0          ; -> obj5:0x0474
        je     loc_4_0E38
        mov    edx, dword ptr [0x47c]        ; -> obj5:0x047C
        mov    eax, dword ptr [edx*4 + 0x4d0] ; -> obj5:0x04D0
        mov    ebx, dword ptr [edx*4 + 0x4a8] ; -> obj5:0x04A8
        mov    ecx, dword ptr [edx*4 + 0x480] ; -> obj5:0x0480
        dec    dword ptr [0x474]             ; -> obj5:0x0474
        inc    edx
        cmp    edx, 0xa
        jb     loc_4_0E2D
        xor    edx, edx
loc_4_0E2D:
        mov    dword ptr [0x47c], edx        ; -> obj5:0x047C
        jmp    loc_4_0EE4
loc_4_0E38:
        ret    
sub_4_0E39:
        mov    ebx, dword ptr [0x1fc]        ; -> obj5:0x01FC
        cmp    eax, 0
        je     loc_4_0E76
        mov    ebx, dword ptr [0x200]        ; -> obj5:0x0200
        cmp    eax, 1
        je     loc_4_0E76
        mov    ebx, dword ptr [0x204]        ; -> obj5:0x0204
        cmp    eax, 2
        je     loc_4_0E76
        mov    ebx, dword ptr [0x208]        ; -> obj5:0x0208
        cmp    eax, 3
        je     loc_4_0E76
        mov    ebx, dword ptr [0x20c]        ; -> obj5:0x020C
        cmp    eax, 4
        je     loc_4_0E76
        mov    ebx, dword ptr [0x210]        ; -> obj5:0x0210
loc_4_0E76:
        ret    
        db  090h
sub_4_0E78:
        cmp    byte ptr [0x851], 0           ; -> loc_5_0851
        je     loc_4_0E86
        jmp    loc_4_100B
loc_4_0E86:
        cmp    byte ptr [0x362], 0           ; -> obj5:0x0362
        je     loc_4_0E94
        jmp    loc_4_100B
loc_4_0E94:
        call   sub_4_0E39
        cmp    byte ptr [0x4fc], 0           ; -> obj5:0x04FC
        je     loc_4_0EE4
        cmp    dword ptr [0x474], 0xa        ; -> obj5:0x0474
        jae    loc_4_100B
        mov    edx, dword ptr [0x478]        ; -> obj5:0x0478
        mov    dword ptr [edx*4 + 0x4d0], eax ; -> obj5:0x04D0
        mov    dword ptr [edx*4 + 0x4a8], ebx ; -> obj5:0x04A8
        mov    dword ptr [edx*4 + 0x480], ecx ; -> obj5:0x0480
        inc    dword ptr [0x474]             ; -> obj5:0x0474
        inc    edx
        cmp    edx, 0xa
        jb     loc_4_0ED8
        xor    edx, edx
loc_4_0ED8:
        mov    dword ptr [0x478], edx        ; -> obj5:0x0478
        jmp    loc_4_100B
        db  090h
loc_4_0EE4:
        VxDCall  VSD.VSD_SoundOn                 ; cd2002000b00
        cmp    cl, 0
        jne    loc_4_0EF9
loc_4_0EEF:
        call   sub_4_100D
        jmp    loc_4_100B
loc_4_0EF9:
        cmp    byte ptr [0x4fc], 0           ; -> obj5:0x04FC
        jne    loc_4_0EEF
        push   eax
        mov    al, 1
        VxDCall  VSD.VSD_TakeSoundPort           ; cd2003000b00
        pop    eax
        mov    word ptr [0x4f8], bx          ; -> obj5:0x04F8
        mov    byte ptr [0x4fd], cl          ; -> loc_5_04FD
        mov    word ptr [0x4fe], bx          ; -> obj5:0x04FE
        mov    byte ptr [0x500], al          ; -> obj5:0x0500
        mov    dx, word ptr [0x21e]          ; -> obj5:0x021E
        cmp    al, 5
        je     loc_4_0F53
        mov    dx, word ptr [0x214]          ; -> obj5:0x0214
        cmp    al, 0
        je     loc_4_0F53
        mov    dx, word ptr [0x21a]          ; -> obj5:0x021A
        cmp    al, 4
        je     loc_4_0F53
        mov    dx, word ptr [0x21c]          ; -> obj5:0x021C
        cmp    al, 3
        je     loc_4_0F53
        jmp    loc_4_0F8E
loc_4_0F53:
        mov    byte ptr [0x4fc], 1           ; -> obj5:0x04FC
        mov    ax, dx
        xor    dx, dx
        out    0x42, al
        mov    al, ah
        jmp    loc_4_0F66
loc_4_0F66:
        out    0x42, al
        jmp    loc_4_0F6A
loc_4_0F6A:
        in     al, 0x61
        or     al, 3
        out    0x61, al
        xor    eax, eax
        xor    edx, edx
        mov    ax, word ptr [0x4fe]          ; -> obj5:0x04FE
        mov    esi, 0xd74                    ; -> loc_4_0D74
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    byte ptr [0x4fc], 0xff        ; -> obj5:0x04FC
        jmp    loc_4_100B
loc_4_0F8E:
        mov    byte ptr [0x4fc], 1           ; -> obj5:0x04FC
        mov    word ptr [0x4fe], bx          ; -> obj5:0x04FE
        mov    word ptr [0x4f8], bx          ; -> obj5:0x04F8
        cmp    al, 2
        jne    loc_4_0FDA
        mov    ax, word ptr [0x216]          ; -> obj5:0x0216
        mov    word ptr [0x4fa], ax          ; -> obj5:0x04FA
        out    0x42, al
        mov    al, ah
        jmp    loc_4_0FB9
loc_4_0FB9:
        out    0x42, al
        jmp    loc_4_0FBD
loc_4_0FBD:
        in     al, 0x61
        or     al, 3
        out    0x61, al
        mov    eax, 3
        mov    esi, 0xd74                    ; -> loc_4_0D74
        mov    edx, 0x1e
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        jmp    loc_4_100B
loc_4_0FDA:
        mov    ax, word ptr [0x21a]          ; -> obj5:0x021A
        mov    word ptr [0x4fa], ax          ; -> obj5:0x04FA
        out    0x42, al
        mov    al, ah
        jmp    loc_4_0FEC
loc_4_0FEC:
        out    0x42, al
        jmp    loc_4_0FF0
loc_4_0FF0:
        in     al, 0x61
        or     al, 3
        out    0x61, al
        mov    eax, 3
        mov    esi, 0xd74                    ; -> loc_4_0D74
        mov    edx, 0xffffffe2
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_4_100B:
        clc    
        ret    
sub_4_100D:
        in     al, 0x61
        and    al, 0xfc
        jmp    loc_4_1013
loc_4_1013:
        out    0x61, al
        mov    byte ptr [0x4fc], 0           ; -> obj5:0x04FC
        mov    al, 0
        VxDCall  VSD.VSD_TakeSoundPort           ; cd2003000b00
        ret    
        db  000h, 000h, 000h
loc_4_1028:
        mov    dword ptr [ebp + 0x1c], 0x400
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_4_1035:
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        mov    word ptr [ebp + 0x1c], ax
        ror    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_4_104B:
        xor    eax, eax
        test   dword ptr [0x3cc], 0x4000     ; -> loc_1_03CC
        je     loc_4_106C
        test   dword ptr [0x3cc], 1          ; -> loc_1_03CC
        je     loc_4_106C
        mov    eax, dword ptr [0x2e2]        ; -> obj5:0x02E2
        mov    eax, dword ptr [eax]
loc_4_106C:
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        mov    dword ptr [ebp + 0x10], eax
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
sub_4_107D:
        cmp    eax, 0
        je     loc_4_10B6
        cmp    byte ptr [0x850], 8           ; -> obj5:0x0850
        je     loc_4_1106
loc_4_108B:
        cmp    dword ptr [ebx*8 + 0x810], 0xffff ; -> obj5:0x0810
        je     loc_4_10A0
        inc    ebx
        cmp    ebx, 8
        jge    loc_4_1106
        jmp    loc_4_108B
loc_4_10A0:
        mov    dword ptr [ebx*8 + 0x810], ecx ; -> obj5:0x0810
        mov    dword ptr [ebx*8 + 0x814], edx ; -> obj5:0x0814
        inc    byte ptr [0x850]              ; -> obj5:0x0850
        jmp    loc_4_10E3
loc_4_10B6:
        cmp    dword ptr [ebx*8 + 0x810], ecx ; -> obj5:0x0810
        je     loc_4_10C7
        inc    ebx
        cmp    ebx, 8
        jge    loc_4_1106
        jmp    loc_4_10B6
loc_4_10C7:
        mov    dword ptr [ebx*8 + 0x810], 0xffff ; -> obj5:0x0810
        mov    dword ptr [ebx*8 + 0x814], 0xffff ; -> obj5:0x0814
        dec    byte ptr [0x850]              ; -> obj5:0x0850
loc_4_10E3:
        and    dword ptr [ebx*8 + 0x810], 0xffff ; -> obj5:0x0810
        and    dword ptr [ebx*8 + 0x814], 0xffff ; -> obj5:0x0814
        or     dword ptr [ebx*8 + 0x814], 0x80000 ; -> obj5:0x0814
        clc    
        ret    
loc_4_1106:
        stc    
        ret    
loc_4_1108:
        mov    ebx, 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        call   sub_4_107D
        jb     loc_4_1123
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_4_1123:
        or     word ptr [ebp + 0x2c], 1
        ret    
        db  02Eh, 08Bh, 0C0h
sub_4_112C:
        test   bl, 4
        je     loc_4_1153
        test   ebx, 0x8000
        je     loc_4_1147
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [0x260], eax        ; -> loc_5_0260
        jmp    loc_4_12A9
loc_4_1147:
        mov    eax, dword ptr [0x260]        ; -> loc_5_0260
        mov    dword ptr [edi], eax
        jmp    loc_4_12A9
loc_4_1153:
        test   ebx, 0x20000
        je     loc_4_117E
        test   ebx, 0x8000
        je     loc_4_1172
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        mov    dword ptr [0x379], eax        ; -> obj5:0x0379
        jmp    loc_4_12A9
loc_4_1172:
        mov    eax, dword ptr [0x379]        ; -> obj5:0x0379
        mov    dword ptr [edi], eax
        jmp    loc_4_12A9
loc_4_117E:
        test   ebx, 0x40000
        je     loc_4_11A9
        test   ebx, 0x8000
        je     loc_4_119D
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        mov    dword ptr [0x37d], eax        ; -> obj5:0x037D
        jmp    loc_4_12A9
loc_4_119D:
        mov    eax, dword ptr [0x37d]        ; -> obj5:0x037D
        mov    dword ptr [edi], eax
        jmp    loc_4_12A9
loc_4_11A9:
        mov    byte ptr [0x851], 0xff        ; -> loc_5_0851
        call   sub_4_12BB
        jb     loc_4_12B2
        or     edi, edi
        jne    loc_4_11BF
loc_4_11BF:
        push   ecx
        push   esi
        push   edi
        cmp    dword ptr [edi + 0x20], 4
        jne    loc_4_1206
        mov    eax, dword ptr [edi + 0x28]
        cmp    esi, 0x296                    ; -> loc_5_0296
        je     loc_4_11DE
        cmp    edi, 0x296                    ; -> loc_5_0296
        jne    loc_4_1206
        mov    eax, dword ptr [esi + 0x28]
loc_4_11DE:
        or     eax, eax
        je     loc_4_1206
        mov    dword ptr [0x2be], eax        ; -> loc_5_02BE
        mov    esi, eax
        mov    edi, 0x504                    ; -> obj5:0x0504
        mov    ecx, 0x103
        cld    
        test   ebx, 0x8000
        jne    loc_4_11FE
        xchg   edi, esi
loc_4_11FE:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        je     loc_4_1206
        loop   loc_4_11FE
loc_4_1206:
        pop    edi
        pop    esi
        pop    ecx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [edi + 8]
        cmp    esi, 0x27e                    ; -> loc_5_027E
        je     loc_4_1222
        cmp    edi, 0x27e                    ; -> loc_5_027E
        jne    loc_4_124B
        mov    eax, dword ptr [esi + 8]
loc_4_1222:
        or     eax, eax
        je     loc_4_124B
        mov    esi, eax
        mov    edi, 0x608                    ; -> obj5:0x0608
        mov    dword ptr [0x286], edi        ; -> loc_5_0286
        mov    ecx, 0x103
        cld    
        test   ebx, 0x8000
        jne    loc_4_1243
        xchg   edi, esi
loc_4_1243:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        je     loc_4_124B
        loop   loc_4_1243
loc_4_124B:
        pop    edi
        pop    esi
        pop    ecx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [edi + 0xc]
        cmp    esi, 0x27e                    ; -> loc_5_027E
        je     loc_4_1267
        cmp    edi, 0x27e                    ; -> loc_5_027E
        jne    loc_4_1290
        mov    eax, dword ptr [esi + 0xc]
loc_4_1267:
        or     eax, eax
        je     loc_4_1290
        mov    esi, eax
        mov    edi, 0x70c                    ; -> obj5:0x070C
        mov    dword ptr [0x28a], edi        ; -> loc_5_028A
        mov    ecx, 0x103
        cld    
        test   ebx, 0x8000
        jne    loc_4_1288
        xchg   edi, esi
loc_4_1288:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        je     loc_4_1290
        loop   loc_4_1288
loc_4_1290:
        pop    edi
        pop    esi
        pop    ecx
        cld    
        push   esi
        push   edi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        test   ebx, 0x8000
        je     loc_4_12A9
        xchg   edi, esi
        call   sub_4_03A0
loc_4_12A9:
        clc    
        mov    byte ptr [0x851], 0           ; -> loc_5_0851
        ret    
loc_4_12B2:
        stc    
        mov    byte ptr [0x851], 0           ; -> loc_5_0851
        ret    
sub_4_12BB:
        bsf    eax, ebx
        je     loc_4_12E9
        cmp    eax, 0xd
        jae    loc_4_12E9
        mov    esi, dword ptr [eax*8 + 0x854] ; -> obj5:0x0854
        mov    ecx, dword ptr [eax*8 + 0x858] ; -> loc_5_0858
        cmp    esi, 0
        je     loc_4_12E9
        cmp    ecx, 0
        je     loc_4_12E9
        test   ebx, 0x8000
        je     loc_4_12E8
        xchg   edi, esi
        ret    
loc_4_12E8:
        ret    
loc_4_12E9:
        stc    
        ret    
loc_4_12EB:
        cmp    ecx, 0
        je     loc_4_12F5
        cmp    ecx, 0
        jne    loc_4_12FF
loc_4_12F5:
        mov    ecx, 0x400
        jmp    loc_4_13B7
loc_4_12FF:
        mov    edi, dword ptr [esi + 0x18]
        mov    esi, dword ptr [esi + 0x10]
        cmp    ecx, 0x100
        jne    loc_4_1318
        mov    dword ptr [edi], 0x400
        jmp    loc_4_13B7
loc_4_1318:
        cmp    ecx, 0x102
        jne    loc_4_132C
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        mov    dword ptr [edi], eax
        jmp    loc_4_13B7
loc_4_132C:
        cmp    ecx, 0x103
        jne    loc_4_1363
        xor    eax, eax
        test   dword ptr [0x3cc], 0x4000     ; -> loc_1_03CC
        je     loc_4_1355
        test   dword ptr [0x3cc], 1          ; -> loc_1_03CC
        je     loc_4_1355
        mov    eax, dword ptr [0x2e2]        ; -> obj5:0x02E2
        mov    eax, dword ptr [eax]
loc_4_1355:
        mov    dword ptr [edi], eax
        add    edi, 4
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        mov    dword ptr [edi], eax
        jmp    loc_4_13B7
loc_4_1363:
        cmp    ecx, 0x104
        jne    loc_4_137F
        mov    eax, dword ptr [esi]
        mov    ebx, 0
        mov    ecx, dword ptr [esi + 4]
        mov    edx, dword ptr [esi + 8]
        call   sub_4_107D
        jmp    loc_4_13B7
loc_4_137F:
        cmp    ecx, 0x101
        jne    loc_4_13B7
        bsf    eax, ebx
        je     loc_4_13B7
        cmp    eax, 0xd
        jae    loc_4_13B7
        test   ebx, 0x8000
        je     loc_4_13A2
        mov    edi, dword ptr [eax*8 + 0x854] ; -> obj5:0x0854
        jmp    loc_4_13A9
loc_4_13A2:
        mov    esi, dword ptr [eax*8 + 0x854] ; -> obj5:0x0854
loc_4_13A9:
        mov    ecx, dword ptr [eax*8 + 0x858] ; -> loc_5_0858
        rep movsb byte ptr es:[edi], byte ptr [esi]
        call   sub_4_03A0
loc_4_13B7:
        xor    eax, eax
        ret    
        db  000h, 000h
sub_4_13BC:
        push   eax
        push   esi
        push   edi
        cld    
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
loc_4_13C8:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_4_13C8
        pop    edi
        pop    esi
        pop    eax
        ret    
sub_4_13D2:
        push   eax
        push   edx
        push   esi
        push   edi
        cld    
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi]
        mov    esi, 0xa
        cmp    eax, 0
        jge    loc_4_13F1
        mov    byte ptr [edi], 0x2d
        inc    edi
        neg    eax
loc_4_13F1:
        push   0
loc_4_13F3:
        xor    edx, edx
        div    esi
        mov    dl, byte ptr [edx + 0xdb8]    ; -> obj5:0x0DB8
        push   edx
        or     eax, eax
        jne    loc_4_13F3
loc_4_1402:
        pop    eax
        stosb  byte ptr es:[edi], al
        or     eax, eax
        jne    loc_4_1402
        pop    edi
        pop    esi
        pop    edx
        pop    eax
        ret    
        db  083h, 0E0h, 00Fh, 08Ah, 080h
        dd  obj5:0x0DB8              ; fixup
        db  088h, 007h, 047h, 0C3h
loc_4_141A:
        and    byte ptr [0x11c7], 0          ; -> obj5:0x11C7
        ret    
sub_4_1422:
        push   ebx
        push   ecx
        xor    eax, eax
        xor    ebx, ebx
        mov    cl, byte ptr [edx]
        cmp    cl, 0x2d
        jne    loc_4_1430
        inc    edx
loc_4_1430:
        mov    al, byte ptr [edx]
        inc    edx
        or     al, al
        je     loc_4_1446
        sub    al, 0x30
        jb     loc_4_1446
        cmp    al, 0xa
        jae    loc_4_1446
        imul   ebx, ebx, 0xa
        add    ebx, eax
        jmp    loc_4_1430
loc_4_1446:
        cmp    cl, 0x2d
        jne    loc_4_144D
        neg    ebx
loc_4_144D:
        mov    eax, ebx
        pop    ecx
        pop    ebx
        ret    
sub_4_1452:
        and    dword ptr [0x8c8], 0          ; -> loc_5_08C8
        pushal 
        xor    ebx, ebx
loc_4_145C:
        mov    edx, dword ptr [ebx*8 + 0xd04] ; -> obj5:0x0D04
        or     edx, edx
        je     loc_4_1615
        push   ecx
        push   edx
        push   0x8c4                         ; -> loc_5_08C4
        push   edx
        push   0x80000001
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        pop    edx
        pop    ecx
        test   eax, eax
        jne    loc_4_15D6
        xor    ecx, ecx
        mov    edx, dword ptr [ebx*8 + 0xd08] ; -> obj5:0x0D08
        or     edx, edx
        je     loc_4_15C9
loc_4_149C:
        mov    esi, dword ptr [edx + ecx*4]
        or     esi, esi
        je     loc_4_15C9
        mov    dword ptr [0x8d0], 0x10       ; -> loc_5_08D0
        push   ecx
        push   edx
        cmp    esi, 0x10c9                   ; -> obj5:0x10C9
        jne    loc_4_14E8
        mov    dword ptr [0x8cc], 0x104      ; -> loc_5_08CC
        push   0x8cc                         ; -> loc_5_08CC
        push   0x504                         ; -> obj5:0x0504
        push   0x8d4                         ; -> loc_5_08D4
        push   0
        push   esi
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        jmp    loc_4_153E
loc_4_14E8:
        cmp    esi, 0x1180                   ; -> obj5:0x1180
        jne    loc_4_151D
        mov    dword ptr [0x8cc], 0x104      ; -> loc_5_08CC
        push   0x8cc                         ; -> loc_5_08CC
        push   0x608                         ; -> obj5:0x0608
        push   0x8d4                         ; -> loc_5_08D4
        push   0
        push   esi
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        jmp    loc_4_153E
loc_4_151D:
        push   0x8d0                         ; -> loc_5_08D0
        push   0x8d8                         ; -> loc_5_08D8
        push   0x8d4                         ; -> loc_5_08D4
        push   0
        push   esi
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
loc_4_153E:
        pop    edx
        pop    ecx
        test   eax, eax
        je     loc_4_1578
        push   0x8d8                         ; -> loc_5_08D8
        push   dword ptr [edx + ecx*4 + 4]
        call   sub_4_13BC
        add    esp, 8
        push   ebx
        push   ecx
        push   edx
        push   0x10
        push   0x8d8                         ; -> loc_5_08D8
        push   1
        push   0
        push   dword ptr [edx + ecx*4]
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        pop    edx
        pop    ecx
        pop    ebx
loc_4_1578:
        cmp    esi, 0x10c9                   ; -> obj5:0x10C9
        je     loc_4_15C0
        cmp    esi, 0x1180                   ; -> obj5:0x1180
        je     loc_4_15C0
        push   eax
        push   edx
        mov    edx, 0x8d8                    ; -> loc_5_08D8
        call   sub_4_1422
        pop    edx
        push   edx
        cmp    dword ptr [edx + ecx*4 + 0xc], 0x80000000
        je     loc_4_15B8
        ja     loc_4_15BE
        or     eax, eax
        mov    eax, dword ptr [edx + ecx*4 + 0xc]
        mov    edx, dword ptr [edx + ecx*4 + 8]
        jne    loc_4_15B4
        not    eax
        and    dword ptr [edx], eax
        jmp    loc_4_15BE
loc_4_15B4:
        or     dword ptr [edx], eax
        jmp    loc_4_15BE
loc_4_15B8:
        mov    edx, dword ptr [edx + ecx*4 + 8]
        mov    dword ptr [edx], eax
loc_4_15BE:
        pop    edx
        pop    eax
loc_4_15C0:
        inc    ecx
        inc    ecx
        inc    ecx
        inc    ecx
        jmp    loc_4_149C
loc_4_15C9:
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        pop    ecx
loc_4_15D6:
        pushal 
        mov    esi, dword ptr [ebx*8 + 0xd0c] ; -> obj5:0x0D0C
        mov    ebx, dword ptr [ebx*8 + 0xd10] ; -> obj5:0x0D10
        test   ebx, 0x100
        je     loc_4_1601
        test   byte ptr [esi + 4], 1
        je     loc_4_1601
        mov    dword ptr [0x8bc], 0x100      ; -> loc_5_08BC
        and    byte ptr [esi + 4], 0xfe
loc_4_1601:
        call   sub_4_03A0
        popal  
        mov    byte ptr [0x11c7], 1          ; -> obj5:0x11C7
        inc    ebx
        inc    ebx
        jmp    loc_4_145C
loc_4_1615:
        cmp    dword ptr [0x8bc], 0x100      ; -> loc_5_08BC
        jne    loc_4_1640
        mov    edx, dword ptr [0x8bc]        ; -> loc_5_08BC
        or     edx, 0x8000
        push   0
        push   edx
        push   0x1654                        ; -> loc_4_1654
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        jmp    loc_4_1652
loc_4_1640:
        mov    eax, 0x7d0
        xor    edx, edx
        mov    esi, 0x141a                   ; -> loc_4_141A
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_4_1652:
        popal  
        ret    
loc_4_1654:
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [0x46d], cx          ; -> obj5:0x046D
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    eax, 0x384                    ; -> obj5:0x0384
        push   0x46d                         ; -> obj5:0x046D
        push   2
        push   eax
        push   0x190                         ; -> obj5:0x0190
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        cmp    eax, 2
        je     loc_4_16A5
        or     byte ptr [0x224], 1           ; -> obj5:0x0224
        mov    esi, 0x220                    ; -> obj5:0x0220
        mov    ebx, 0x100
        call   sub_4_03A0
loc_4_16A5:
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
sub_4_16B6:
        pushal 
        mov    dword ptr [0x8c0], ebx        ; -> loc_5_08C0
        xor    ebx, ebx
loc_4_16BF:
        mov    edx, dword ptr [ebx*8 + 0xd04] ; -> obj5:0x0D04
        or     edx, edx
        je     loc_4_17F7
        mov    edx, dword ptr [ebx*8 + 0xd10] ; -> obj5:0x0D10
        and    edx, dword ptr [0x8c0]        ; -> loc_5_08C0
        je     loc_4_17F0
        mov    edx, dword ptr [ebx*8 + 0xd04] ; -> obj5:0x0D04
        push   ebx
        push   ecx
        push   edx
        push   0x8c4                         ; -> loc_5_08C4
        push   edx
        push   0x80000001
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        pop    edx
        pop    ecx
        pop    ebx
        cmp    eax, 0
        jne    loc_4_17F0
        xor    ecx, ecx
        mov    edx, dword ptr [ebx*8 + 0xd08] ; -> obj5:0x0D08
        or     edx, edx
        je     loc_4_17E1
loc_4_171C:
        mov    esi, dword ptr [edx + ecx*4]
        or     esi, esi
        je     loc_4_17E1
        cmp    dword ptr [edx + ecx*4 + 0xc], 0x80000000
        jae    loc_4_1756
        mov    byte ptr [0x8d8], 0x30        ; -> loc_5_08D8
        mov    byte ptr [0x8d9], 0           ; -> obj5:0x08D9
        mov    eax, dword ptr [edx + ecx*4 + 0xc]
        push   edx
        mov    edx, dword ptr [edx + ecx*4 + 8]
        test   dword ptr [edx], eax
        pop    edx
        je     loc_4_1767
        mov    byte ptr [0x8d8], 0x31        ; -> loc_5_08D8
        jmp    loc_4_1767
loc_4_1756:
        push   0x8d8                         ; -> loc_5_08D8
        push   dword ptr [edx + ecx*4 + 8]
        call   sub_4_13D2
        add    esp, 8
loc_4_1767:
        push   ebx
        push   ecx
        push   edx
        cmp    esi, 0x1180                   ; -> obj5:0x1180
        jne    loc_4_1792
        push   0x104
        push   0x608                         ; -> obj5:0x0608
        push   1
        push   0
        push   esi
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        jmp    loc_4_17D5
loc_4_1792:
        cmp    esi, 0x10c9                   ; -> obj5:0x10C9
        jne    loc_4_17BA
        push   0x104
        push   0x504                         ; -> obj5:0x0504
        push   1
        push   0
        push   esi
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        jmp    loc_4_17D5
loc_4_17BA:
        push   0x10
        push   0x8d8                         ; -> loc_5_08D8
        push   1
        push   0
        push   esi
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
loc_4_17D5:
        pop    edx
        pop    ecx
        pop    ebx
        inc    ecx
        inc    ecx
        inc    ecx
        inc    ecx
        jmp    loc_4_171C
loc_4_17E1:
        push   ebx
        push   dword ptr [0x8c4]             ; -> loc_5_08C4
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        pop    ecx
        pop    ebx
loc_4_17F0:
        inc    ebx
        inc    ebx
        jmp    loc_4_16BF
loc_4_17F7:
        popal  
        ret    
        db  000h, 000h, 000h
sub_4_17FC:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 1
        mov    eax, 1
        jne    loc_4_1826
        mov    eax, dword ptr [esp + 4]
        push   0x608                         ; -> obj5:0x0608
        push   eax
        call   sub_8_0020
        add    esp, 8
        call   sub_4_0D23
        call   sub_4_0D02
loc_4_1826:
        ret    8
        db  0C3h, 000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x11d1  RW-
        db  020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        db  020h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        db  045h, 06Eh, 061h, 062h, 06Ch, 065h, 000h, 042h, 065h, 06Ch, 06Ch, 044h, 075h, 072h, 061h, 074h
        db  069h, 06Fh, 06Eh, 000h, 042h, 065h, 06Ch, 06Ch, 054h, 06Fh, 06Eh, 065h, 000h, 055h, 070h, 053h
        db  069h, 072h, 065h, 06Eh, 044h, 075h, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 055h, 070h, 053h
        db  069h, 072h, 065h, 06Eh, 053h, 074h, 061h, 072h, 074h, 054h, 06Fh, 06Eh, 065h, 000h, 044h, 06Fh
        db  077h, 06Eh, 053h, 069h, 072h, 065h, 06Eh, 044h, 075h, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h
        db  044h, 06Fh, 077h, 06Eh, 053h, 069h, 072h, 065h, 06Eh, 054h, 06Fh, 06Eh, 065h, 000h, 048h, 069h
        db  067h, 068h, 054h, 06Fh, 06Eh, 065h, 044h, 075h, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 048h
        db  069h, 067h, 068h, 054h, 06Fh, 06Eh, 065h, 000h, 04Ch, 06Fh, 077h, 054h, 06Fh, 06Eh, 065h, 044h
        db  075h, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 04Ch, 06Fh, 077h, 054h, 06Fh, 06Eh, 065h, 000h
        db  043h, 06Ch, 069h, 063h, 06Bh, 054h, 06Fh, 06Eh, 065h, 000h, 043h, 06Ch, 069h, 063h, 06Bh, 044h
        db  075h, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 057h, 061h, 072h, 06Eh, 04Fh, 06Eh, 046h, 065h
        db  061h, 074h, 075h, 072h, 065h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 04Eh, 041h, 042h, 04Ch, 045h, 032h, 02Eh
        db  056h, 058h, 044h, 000h, 045h, 04Eh, 041h, 042h, 04Ch, 045h, 032h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 04Eh, 041h, 042h
        db  04Ch, 045h, 034h, 02Eh, 056h, 058h, 044h, 000h, 045h, 04Eh, 041h, 042h, 04Ch, 045h, 034h, 000h
        db  045h, 04Eh, 041h, 042h, 04Ch, 045h, 033h, 02Eh, 044h, 04Ch, 04Ch, 000h, 048h, 045h, 04Ch, 050h
        db  045h, 052h, 05Fh, 053h, 045h, 054h, 048h, 049h, 047h, 048h, 043h, 04Fh, 04Eh, 054h, 052h, 041h
        db  053h, 054h, 000h, 048h, 045h, 04Ch, 050h, 045h, 052h, 05Fh, 043h, 04Ch, 045h, 041h, 052h, 048h
        db  049h, 047h, 048h, 043h, 04Fh, 04Eh, 054h, 052h, 041h, 053h, 054h, 000h, 048h, 045h, 04Ch, 050h
        db  045h, 052h, 05Fh, 052h, 045h, 047h, 049h, 053h, 054h, 045h, 052h, 04Dh, 045h, 053h, 053h, 041h
        db  047h, 045h, 000h, 048h, 045h, 04Ch, 050h, 045h, 052h, 05Fh, 053h, 050h, 041h, 057h, 04Eh, 053h
        db  054h, 041h, 054h, 055h, 053h, 041h, 050h, 050h, 000h, 000h, 000h, 000h, 0FAh, 000h, 000h, 000h
        db  028h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 014h, 000h, 000h, 000h, 014h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 038h, 005h, 020h, 005h, 0C0h, 006h, 0C0h, 006h, 020h, 005h, 0C0h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_0230:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0234:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0238:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_023C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0240:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0244:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0248:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_024C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0250:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_025C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0260:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_026A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_026E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0272:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0276:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_027A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_027E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0282:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0286:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_028A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_028E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0292:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0296:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_029A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_029E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02A6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02AA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02AE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02B2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02B6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02BA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02BE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02C2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02C6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_02CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- dispatch table (25 entries) ----
        dd  sub_4_0910
        dd  loc_1_0096
        dd  loc_1_0096
        dd  sub_4_0CAB
        dd  loc_1_0096
        dd  loc_1_0096
        dd  sub_4_0AAA
        dd  sub_4_0D23
        dd  sub_4_0883
        dd  sub_4_09C3
        dd  sub_4_0B57
        dd  sub_4_0C05
        dd  loc_4_095F
        dd  loc_1_0096
        dd  loc_1_0096
        dd  loc_4_0CC6
        dd  loc_1_0096
        dd  loc_1_0096
        dd  loc_4_0AE5
        dd  loc_4_0D3E
        dd  loc_4_08A1
        dd  loc_4_0A01
        dd  loc_4_0B75
        dd  loc_4_0C53
        dd  loc_1_0096
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_5_035E:
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0363:
        add    byte ptr [eax], cl
        or     dword ptr [esi], eax
        add    al, byte ptr [eax]
        or     al, 7
        add    ecx, dword ptr [ebx]
        adc    dword ptr [edx], edx
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 048h, 045h, 04Ch, 050h, 045h, 052h, 05Fh, 044h, 049h, 053h, 050h
        db  04Ch, 041h, 059h, 057h, 041h, 052h, 04Eh, 049h, 04Eh, 047h, 000h
        dd  loc_5_026E               ; fixup
        dd  obj5:0x0224              ; fixup
        dd  loc_5_029A               ; fixup
        dd  loc_5_029A               ; fixup
        dd  obj5:0x0224              ; fixup
        dd  obj5:0x0224              ; fixup
        dd  loc_5_0244               ; fixup
        dd  loc_5_0282               ; fixup
        dd  obj5:0x0224              ; fixup
        dd  loc_5_023C               ; fixup
        dd  loc_5_029A               ; fixup
        dd  loc_5_02CA               ; fixup
        dd  loc_5_026E               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
        dd  loc_5_0244               ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj5:0x0224              ; fixup
        dd  loc_5_023C               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_5_02CA               ; fixup
        dd  loc_5_026E               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
        dd  loc_5_0244               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_023C               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_5_02CA               ; fixup
        dd  loc_4_08DD               ; fixup
        dd  loc_4_05B0               ; fixup
        dd  loc_4_05B0               ; fixup
        dd  loc_4_0C8A               ; fixup
        dd  loc_4_05B0               ; fixup
        dd  loc_4_05B0               ; fixup
        dd  loc_4_0A41               ; fixup
        dd  sub_4_0D02               ; fixup
        dd  loc_4_085F               ; fixup
        dd  loc_4_099F               ; fixup
        dd  loc_4_0B33               ; fixup
        dd  loc_4_0BAE               ; fixup
sub_5_045A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_045E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0464:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4], ch        ; -> obj5:0x046D
        add    byte ptr [eax], al
loc_5_0473:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_04FD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    bh, bh
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_5_0851:
        add    byte ptr [eax], al
        add    byte ptr [edx + 2], ch        ; -> loc_5_026A
        add    byte ptr [eax], al
loc_5_0858:
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
        xchg   esi, eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], al        ; -> loc_5_0240
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        jle    loc_5_0890
        add    byte ptr [eax], al
loc_5_0890:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [edx], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    byte ptr [edx], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        pop    esp
        add    al, byte ptr [eax]
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    dh, al                        ; -> loc_5_02C6
        add    al, byte ptr [eax]
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        jb     loc_5_08B8
        add    byte ptr [eax], al
loc_5_08B8:
        or     al, 0
        add    byte ptr [eax], al
loc_5_08BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_08C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_08C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_08C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_08CC:
        add    al, 1
        add    byte ptr [eax], al
loc_5_08D0:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_08D4:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_5_08D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_08E8:
        or     byte ptr [esi], 0
        add    dl, cl                        ; -> obj5:0x0DCA
        or     eax, 0x26e0000                ; -> loc_5_026E
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        test   dword ptr [esi], ecx
        add    byte ptr [eax], al
        int3   
        or     eax, 0x26e0000                ; -> loc_5_026E
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   ebx, eax
        push   cs
        add    byte ptr [eax], al
        into   
        or     eax, 0x26e0000                ; -> loc_5_026E
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [ebx - 0x2ffffff2], ah ; -> obj5:0x0EA3
        or     eax, 0x26e0000                ; -> loc_5_026E
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [esi + ecx + 0xdd20000], ch ; -> obj5:0x0EAC
        add    byte ptr [eax], al
        outsb  dx, byte ptr [esi]
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [edi - 0x2bfffff2], dh ; -> obj5:0x0EB7
        or     eax, 0x26e0000                ; -> loc_5_026E
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        les    ecx, ptr [esi]
        add    byte ptr [eax], al
        salc   
        or     eax, 0x26e0000                ; -> loc_5_026E
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        into   
        push   cs
        add    byte ptr [eax], al
        fmul   dword ptr [0x26e0000]         ; -> loc_5_026E
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        out    0xe, al
        add    byte ptr [eax], al
        enter  0xd, 0
        outsb  dx, byte ptr [esi]
        add    al, byte ptr [eax]
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx - 0x25fffff2], al ; -> obj5:0x0E82
        or     eax, 0x2240000                ; -> obj5:0x0224
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        test   dword ptr [esi], ecx
        add    byte ptr [eax], al
        fmul   qword ptr [0x2240000]         ; -> obj5:0x0224
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        daa    
        sldt   word ptr [eax]
        fimul  word ptr [0x2240000]          ; -> obj5:0x0224
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edi], ch            ; -> obj5:0x0F2F
        sldt   word ptr [eax]
        loopne 0x9bf
        add    byte ptr [eax], al
        sub    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0xf46], al    ; -> obj5:0x0F46
        in     eax, 0xd
        add    byte ptr [eax], al
        sub    al, 2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0xf55], al    ; -> obj5:0x0F55
        jmp    loc_5_0230
        db  002h, 000h, 000h, 000h, 000h, 000h, 080h
        dd  obj5:0x0F65              ; fixup
        dd  obj5:0x0DEE              ; fixup
        dd  loc_5_0234               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x0EB7              ; fixup
        dd  obj5:0x0DF0              ; fixup
        dd  obj5:0x0224              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj5:0x0EC4              ; fixup
        dd  obj5:0x0DF2              ; fixup
        dd  obj5:0x0224              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj5:0x0ECE              ; fixup
        dd  obj5:0x0DF4              ; fixup
        dd  obj5:0x0224              ; fixup
        db  008h, 000h, 000h, 000h
        dd  obj5:0x0EE6              ; fixup
        dd  obj5:0x0DC8              ; fixup
        dd  obj5:0x0224              ; fixup
        db  020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E82              ; fixup
        dd  obj5:0x0DF6              ; fixup
        dd  loc_5_0244               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj5:0x0E85              ; fixup
        dd  obj5:0x0DF8              ; fixup
        dd  loc_5_0244               ; fixup
        db  010h, 000h, 000h, 000h
        dd  obj5:0x0F96              ; fixup
        dd  obj5:0x0DFA              ; fixup
        dd  loc_5_0248               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x0FA3              ; fixup
        dd  obj5:0x0DFD              ; fixup
        dd  loc_5_024C               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x0EB7              ; fixup
        dd  obj5:0x0E01              ; fixup
        dd  loc_5_0244               ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj5:0x0EC4              ; fixup
        dd  obj5:0x0E03              ; fixup
        dd  loc_5_0244               ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj5:0x0ECE              ; fixup
        dd  obj5:0x0E05              ; fixup
        dd  loc_5_0244               ; fixup
        db  008h, 000h, 000h, 000h
        dd  obj5:0x0EDC              ; fixup
        dd  obj5:0x0E07              ; fixup
        dd  loc_5_0244               ; fixup
        db  040h, 000h, 000h, 000h
        dd  obj5:0x0EE6              ; fixup
        dd  obj5:0x0DC8              ; fixup
        dd  loc_5_0244               ; fixup
        db  020h, 000h, 000h, 000h
        dd  obj5:0x0FB6              ; fixup
        dd  obj5:0x0E09              ; fixup
        dd  loc_5_0244               ; fixup
        db  080h, 000h, 000h, 000h
        dd  obj5:0x0FC5              ; fixup
        dd  obj5:0x0E0B              ; fixup
        dd  loc_5_0250               ; fixup
        db  000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E82              ; fixup
        dd  obj5:0x0E0E              ; fixup
        dd  loc_5_023C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj5:0x0E85              ; fixup
        dd  obj5:0x0E10              ; fixup
        dd  loc_5_023C               ; fixup
        db  010h, 000h, 000h, 000h
        dd  obj5:0x0EB7              ; fixup
        dd  obj5:0x0E12              ; fixup
        dd  loc_5_023C               ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj5:0x0EC4              ; fixup
        dd  obj5:0x0E14              ; fixup
        dd  loc_5_023C               ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj5:0x0ECE              ; fixup
        dd  obj5:0x0E16              ; fixup
        dd  loc_5_023C               ; fixup
        db  008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E82              ; fixup
        dd  obj5:0x0E18              ; fixup
        dd  loc_5_0276               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj5:0x0E85              ; fixup
        dd  obj5:0x0E1A              ; fixup
        dd  loc_5_0276               ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj5:0x101E              ; fixup
        dd  obj5:0x0E1C              ; fixup
        dd  loc_5_027A               ; fixup
        db  000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E82              ; fixup
        dd  obj5:0x0E23              ; fixup
        dd  loc_5_029A               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj5:0x1051              ; fixup
        dd  obj5:0x0E25              ; fixup
        dd  loc_5_029E               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x105C              ; fixup
        dd  obj5:0x0E27              ; fixup
        dd  loc_5_02A2               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x106B              ; fixup
        dd  obj5:0x0E2B              ; fixup
        dd  loc_5_02A6               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x107F              ; fixup
        dd  obj5:0x0E2D              ; fixup
        dd  loc_5_02AA               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x108A              ; fixup
        dd  obj5:0x0E2F              ; fixup
        dd  loc_5_02AE               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x1099              ; fixup
        dd  obj5:0x0E33              ; fixup
        dd  loc_5_02B2               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x10A9              ; fixup
        dd  obj5:0x0E35              ; fixup
        dd  loc_5_02B6               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x10B7              ; fixup
        dd  obj5:0x0E37              ; fixup
        dd  loc_5_02BA               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x10C9              ; fixup
        dd  obj5:0x0E3B              ; fixup
        dd  loc_5_02BE               ; fixup
        db  001h, 000h, 000h, 080h
        dd  obj5:0x10DA              ; fixup
        dd  obj5:0x0E3C              ; fixup
        dd  loc_5_02C2               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x0EC4              ; fixup
        dd  obj5:0x0E3E              ; fixup
        dd  loc_5_029A               ; fixup
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E82              ; fixup
        dd  obj5:0x0E40              ; fixup
        dd  loc_5_02CA               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj5:0x0EB7              ; fixup
        dd  obj5:0x0E42              ; fixup
        dd  loc_5_02CA               ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj5:0x0EC4              ; fixup
        dd  obj5:0x0E44              ; fixup
        dd  loc_5_02CA               ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj5:0x0ECE              ; fixup
        dd  obj5:0x0E46              ; fixup
        dd  loc_5_02CA               ; fixup
        db  008h, 000h, 000h, 000h
        dd  obj5:0x0E85              ; fixup
        dd  obj5:0x0DF8              ; fixup
        dd  loc_5_02CA               ; fixup
        db  010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E82              ; fixup
        dd  obj5:0x0E48              ; fixup
        dd  loc_5_0260               ; fixup
        db  000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        dd  obj5:0x117A              ; fixup
        dd  obj5:0x0E4A              ; fixup
        dd  loc_5_0282               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x1180              ; fixup
        dd  obj5:0x0E4C              ; fixup
        dd  loc_5_0286               ; fixup
        db  001h, 000h, 000h, 080h
        dd  obj5:0x118B              ; fixup
        dd  obj5:0x0E51              ; fixup
        dd  loc_5_028E               ; fixup
        db  000h, 000h, 000h, 080h
        dd  obj5:0x1194              ; fixup
        dd  obj5:0x0E55              ; fixup
        dd  loc_5_0292               ; fixup
        db  000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        dd  obj5:0x1132              ; fixup
        dd  obj5:0x0E57              ; fixup
        dd  obj5:0x0379              ; fixup
        db  000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        dd  obj5:0x1146              ; fixup
        dd  obj5:0x0E59              ; fixup
        dd  obj5:0x037D              ; fixup
        db  000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        dd  obj5:0x0E5B              ; fixup
        dd  loc_5_08E8               ; fixup
        dd  loc_5_026A               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj5:0x0EFA              ; fixup
        dd  obj5:0x097C              ; fixup
        dd  obj5:0x0220              ; fixup
        db  000h, 001h, 000h, 000h
        dd  obj5:0x0F70              ; fixup
        dd  obj5:0x0A30              ; fixup
        dd  loc_5_0240               ; fixup
        db  040h, 000h, 000h, 000h
        dd  obj5:0x0FD3              ; fixup
        dd  obj5:0x0AE4              ; fixup
        dd  loc_5_0238               ; fixup
        db  000h, 002h, 000h, 000h
        dd  obj5:0x0FFA              ; fixup
        dd  obj5:0x0B38              ; fixup
        dd  loc_5_0272               ; fixup
        db  000h, 010h, 000h, 000h
        dd  obj5:0x1029              ; fixup
        dd  obj5:0x0B6C              ; fixup
        dd  loc_5_0296               ; fixup
        db  008h, 000h, 000h, 000h
        dd  obj5:0x10EF              ; fixup
        dd  obj5:0x0C84              ; fixup
        dd  loc_5_025C               ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj5:0x1153              ; fixup
        dd  obj5:0x0C98              ; fixup
        dd  loc_5_027E               ; fixup
        db  080h, 000h, 000h, 000h
        dd  obj5:0x119E              ; fixup
        dd  obj5:0x0C30              ; fixup
        dd  loc_5_02C6               ; fixup
        db  000h, 008h, 000h, 000h
        dd  obj5:0x1116              ; fixup
        dd  obj5:0x0CDC              ; fixup
        dd  obj5:0x0379              ; fixup
        db  000h, 000h, 002h, 000h
        dd  obj5:0x1116              ; fixup
        dd  obj5:0x0CF0              ; fixup
        dd  obj5:0x037D              ; fixup
        db  000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
        db  038h, 039h, 041h, 042h, 043h, 044h, 045h, 046h, 031h, 000h, 030h, 000h, 031h, 000h, 031h, 000h
        db  031h, 000h, 031h, 000h, 030h, 000h, 031h, 000h, 031h, 000h, 030h, 000h, 031h, 000h, 031h, 000h
        db  031h, 030h, 030h, 030h, 000h, 035h, 030h, 030h, 000h, 031h, 030h, 030h, 030h, 000h, 030h, 000h
        db  030h, 000h, 031h, 000h, 031h, 000h, 030h, 000h, 031h, 000h, 034h, 030h, 000h, 033h, 030h, 030h
        db  000h, 030h, 000h, 031h, 000h, 031h, 000h, 030h, 000h, 030h, 000h, 038h, 030h, 000h, 030h, 000h
        db  031h, 000h, 030h, 000h, 031h, 000h, 031h, 000h, 031h, 000h, 031h, 000h, 033h, 030h, 030h, 030h
        db  030h, 030h, 000h, 030h, 000h, 032h, 000h, 035h, 030h, 030h, 000h, 030h, 000h, 033h, 000h, 035h
        db  030h, 030h, 000h, 030h, 000h, 031h, 000h, 035h, 030h, 030h, 000h, 000h, 030h, 000h, 031h, 000h
        db  030h, 000h, 030h, 000h, 031h, 000h, 031h, 000h, 030h, 000h, 032h, 000h, 043h, 04Fh, 04Dh, 032h
        db  000h, 033h, 030h, 030h, 000h, 031h, 000h, 030h, 000h, 030h, 000h, 043h, 06Fh, 06Eh, 074h, 072h
        db  06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h
        db  062h, 069h, 06Ch, 069h, 074h, 079h, 05Ch, 053h, 074h, 069h, 063h, 06Bh, 079h, 06Bh, 065h, 079h
        db  073h, 000h, 04Fh, 06Eh, 000h, 04Fh, 06Eh, 04Fh, 066h, 066h, 046h, 065h, 065h, 064h, 062h, 061h
        db  063h, 06Bh, 000h, 041h, 075h, 064h, 069h, 062h, 06Ch, 065h, 046h, 065h, 065h, 064h, 062h, 061h
        db  063h, 06Bh, 000h, 054h, 072h, 069h, 053h, 074h, 061h, 074h, 065h, 000h, 054h, 077h, 06Fh, 04Bh
        db  065h, 079h, 073h, 04Fh, 066h, 066h, 000h, 048h, 06Fh, 074h, 04Bh, 065h, 079h, 041h, 063h, 074h
        db  069h, 076h, 065h, 000h, 041h, 076h, 061h, 069h, 06Ch, 061h, 062h, 06Ch, 065h, 000h, 043h, 06Fh
        db  06Eh, 066h, 069h, 072h, 06Dh, 048h, 06Fh, 074h, 04Bh, 065h, 079h, 000h, 04Dh, 06Fh, 064h, 069h
        db  066h, 069h, 065h, 072h, 073h, 000h, 053h, 068h, 06Fh, 077h, 053h, 074h, 061h, 074h, 075h, 073h
        db  049h, 06Eh, 064h, 069h, 063h, 061h, 074h, 06Fh, 072h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h, 062h
        db  069h, 06Ch, 069h, 074h, 079h, 05Ch, 04Bh, 065h, 079h, 062h, 06Fh, 061h, 072h, 064h, 052h, 065h
        db  073h, 070h, 06Fh, 06Eh, 073h, 065h, 000h, 043h, 06Ch, 069h, 063h, 06Bh, 04Fh, 06Eh, 000h, 044h
        db  065h, 06Ch, 061h, 079h, 042h, 065h, 066h, 06Fh, 072h, 065h, 041h, 063h, 063h, 065h, 070h, 074h
        db  061h, 06Eh, 063h, 063h, 065h, 000h, 041h, 075h, 074h, 06Fh, 052h, 065h, 070h, 065h, 061h, 074h
        db  052h, 061h, 074h, 065h, 000h, 041h, 075h, 074h, 06Fh, 052h, 065h, 070h, 065h, 061h, 074h, 044h
        db  065h, 06Ch, 061h, 079h, 000h, 042h, 06Fh, 075h, 06Eh, 063h, 065h, 054h, 069h, 06Dh, 065h, 000h
        db  043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h
        db  063h, 065h, 073h, 073h, 069h, 062h, 069h, 06Ch, 069h, 074h, 079h, 05Ch, 04Dh, 06Fh, 075h, 073h
        db  065h, 04Bh, 065h, 079h, 073h, 000h, 04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh, 053h, 070h, 065h
        db  065h, 064h, 000h, 054h, 069h, 06Dh, 065h, 054h, 06Fh, 04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh
        db  053h, 070h, 065h, 065h, 064h, 000h, 052h, 065h, 070h, 06Ch, 061h, 063h, 065h, 04Eh, 075h, 06Dh
        db  062h, 065h, 072h, 073h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 053h, 070h, 065h
        db  065h, 064h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch
        db  05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h, 062h, 069h, 06Ch, 069h, 074h, 079h, 05Ch, 054h
        db  06Fh, 067h, 067h, 06Ch, 065h, 04Bh, 065h, 079h, 073h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h, 062h
        db  069h, 06Ch, 069h, 074h, 079h, 05Ch, 054h, 069h, 06Dh, 065h, 04Fh, 075h, 074h, 000h, 054h, 069h
        db  06Dh, 065h, 054h, 06Fh, 057h, 061h, 069h, 074h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch
        db  020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h, 062h, 069h
        db  06Ch, 069h, 074h, 079h, 05Ch, 053h, 06Fh, 075h, 06Eh, 064h, 053h, 065h, 06Eh, 074h, 072h, 079h
        db  000h, 054h, 065h, 078h, 074h, 045h, 066h, 066h, 065h, 063h, 074h, 000h, 054h, 065h, 078h, 074h
        db  045h, 066h, 066h, 065h, 063h, 074h, 04Dh, 053h, 065h, 063h, 000h, 054h, 065h, 078h, 074h, 045h
        db  066h, 066h, 065h, 063h, 074h, 043h, 06Fh, 06Ch, 06Fh, 072h, 042h, 069h, 074h, 073h, 000h, 047h
        db  072h, 061h, 066h, 045h, 066h, 066h, 065h, 063h, 074h, 000h, 047h, 072h, 061h, 066h, 045h, 066h
        db  066h, 065h, 063h, 074h, 04Dh, 053h, 065h, 063h, 000h, 047h, 072h, 061h, 066h, 045h, 066h, 066h
        db  065h, 063h, 074h, 043h, 06Fh, 06Ch, 06Fh, 072h, 000h, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h
        db  045h, 066h, 066h, 065h, 063h, 074h, 000h, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h, 045h, 066h
        db  066h, 065h, 063h, 074h, 04Dh, 053h, 065h, 063h, 000h, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h
        db  045h, 066h, 066h, 065h, 063h, 074h, 044h, 04Ch, 04Ch, 000h, 057h, 069h, 06Eh, 064h, 06Fh, 077h
        db  073h, 045h, 066h, 066h, 065h, 063h, 074h, 04Fh, 072h, 064h, 069h, 06Eh, 061h, 06Ch, 000h, 043h
        db  06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h
        db  065h, 073h, 073h, 069h, 062h, 069h, 06Ch, 069h, 074h, 079h, 05Ch, 053h, 068h, 06Fh, 077h, 053h
        db  06Fh, 075h, 06Eh, 064h, 073h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h
        db  06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h, 062h, 069h, 06Ch, 069h, 074h
        db  079h, 000h, 04Bh, 065h, 079h, 062h, 06Fh, 061h, 072h, 064h, 020h, 050h, 072h, 065h, 066h, 065h
        db  072h, 065h, 06Eh, 063h, 065h, 000h, 042h, 06Ch, 069h, 06Eh, 064h, 020h, 041h, 063h, 063h, 065h
        db  073h, 073h, 000h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch
        db  05Ch, 041h, 063h, 063h, 065h, 073h, 073h, 069h, 062h, 069h, 06Ch, 069h, 074h, 079h, 05Ch, 053h
        db  065h, 072h, 069h, 061h, 06Ch, 04Bh, 065h, 079h, 073h, 000h, 046h, 06Ch, 061h, 067h, 073h, 000h
        db  041h, 063h, 074h, 069h, 076h, 065h, 050h, 06Fh, 072h, 074h, 000h, 042h, 061h, 075h, 064h, 052h
        db  061h, 074h, 065h, 000h, 050h, 06Fh, 072h, 074h, 053h, 074h, 061h, 074h, 065h, 000h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 020h, 050h, 061h, 06Eh, 065h, 06Ch, 05Ch, 041h, 063h, 063h, 065h
        db  073h, 073h, 069h, 062h, 069h, 06Ch, 069h, 074h, 079h, 05Ch, 048h, 069h, 067h, 068h, 043h, 06Fh
        db  06Eh, 074h, 072h, 061h, 073h, 074h, 000h, 000h, 050h, 06Fh, 072h, 074h, 04Eh, 061h, 06Dh, 065h
        db  000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x14  R-X
        db  0B8h, 001h, 080h, 0F7h, 0C3h, 001h, 000h, 075h, 003h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h
        db  066h, 033h, 0D2h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x488  R-X
sub_7_0000:
        push   esi
        cmp    dword ptr [0x356], 0          ; -> obj5:0x0356
        je     loc_7_0088
        and    dword ptr [0x356], 0          ; -> obj5:0x0356
        test   byte ptr [0x276], 1           ; -> loc_5_0276
        je     loc_7_0088
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        test   eax, 0xffff8fff
        je     loc_7_0088
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ebx, eax
        sub    eax, dword ptr [0x3c8]        ; -> loc_1_03C8
        add    eax, 0x37
        cmp    eax, dword ptr [0x27a]        ; -> loc_5_027A
        jb     loc_7_006D
        test   byte ptr [0x276], 2           ; -> loc_5_0276
        je     loc_7_005F
        xor    ecx, ecx
        inc    ecx
        lea    eax, [ecx + 1]
        mov    ebx, 0xc8
        call   sub_4_0E78
        mov    byte ptr [0x362], 1           ; -> obj5:0x0362
loc_7_005F:
        call   sub_4_0338
        mov    byte ptr [0x362], 0           ; -> obj5:0x0362
        jmp    loc_7_0088
loc_7_006D:
        xchg   ebx, eax
        mov    eax, dword ptr [0x27a]        ; -> loc_5_027A
        sub    eax, ebx
        xor    edx, edx
        mov    esi, 0                        ; -> sub_7_0000
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x356], esi        ; -> obj5:0x0356
loc_7_0088:
        pop    esi
        clc    
        ret    
sub_7_008B:
        push   eax
        cmp    byte ptr [0x3c3], 2           ; -> obj1:0x03C3
        jb     loc_7_01A7
        jne    loc_7_0122
        cmp    dword ptr [0x2d2], -1         ; -> obj5:0x02D2
        je     loc_7_00BC
        mov    eax, dword ptr [0x2d2]        ; -> obj5:0x02D2
        mov    dword ptr [0x230], eax        ; -> loc_5_0230
        mov    dword ptr [0x2d2], 0xffffffff ; -> obj5:0x02D2
loc_7_00BC:
        cmp    dword ptr [0x2d6], -1         ; -> obj5:0x02D6
        je     loc_7_00D9
        mov    eax, dword ptr [0x2d6]        ; -> obj5:0x02D6
        mov    dword ptr [0x22c], eax        ; -> obj5:0x022C
        mov    dword ptr [0x2d6], 0xffffffff ; -> obj5:0x02D6
loc_7_00D9:
        cmp    dword ptr [0x2da], -1         ; -> obj5:0x02DA
        je     loc_7_00F6
        mov    eax, dword ptr [0x2da]        ; -> obj5:0x02DA
        mov    dword ptr [0x228], eax        ; -> obj5:0x0228
        mov    dword ptr [0x2da], 0xffffffff ; -> obj5:0x02DA
loc_7_00F6:
        cmp    dword ptr [0x2de], -1         ; -> obj5:0x02DE
        je     loc_7_0113
        mov    eax, dword ptr [0x2de]        ; -> obj5:0x02DE
        mov    dword ptr [0x234], eax        ; -> loc_5_0234
        mov    dword ptr [0x2de], 0xffffffff ; -> obj5:0x02DE
loc_7_0113:
        mov    edx, 0x100
        call   sub_1_0000
        jmp    loc_7_01A7
loc_7_0122:
        cmp    byte ptr [0x3c3], 3           ; -> obj1:0x03C3
        jne    loc_7_014B
        mov    eax, dword ptr [0x234]        ; -> loc_5_0234
        mov    dword ptr [0x2de], eax        ; -> obj5:0x02DE
        mov    dword ptr [0x234], 0x3e8      ; -> loc_5_0234
        mov    edx, 0x100
        call   sub_1_0000
        jmp    loc_7_01A7
loc_7_014B:
        mov    eax, dword ptr [0x230]        ; -> loc_5_0230
        mov    dword ptr [0x2d2], eax        ; -> obj5:0x02D2
        mov    eax, dword ptr [0x22c]        ; -> obj5:0x022C
        mov    dword ptr [0x2d6], eax        ; -> obj5:0x02D6
        mov    eax, dword ptr [0x228]        ; -> obj5:0x0228
        mov    dword ptr [0x2da], eax        ; -> obj5:0x02DA
        cmp    dword ptr [0x228], 0x7d0      ; -> obj5:0x0228
        jae    loc_7_017F
        mov    dword ptr [0x228], 0x7d0      ; -> obj5:0x0228
loc_7_017F:
        mov    dword ptr [0x230], 0x7ff8     ; -> loc_5_0230
        mov    dword ptr [0x22c], 0x7ff8     ; -> obj5:0x022C
        mov    dword ptr [0x234], 0x7ff8     ; -> loc_5_0234
        mov    edx, 0x100
        call   sub_1_0000
loc_7_01A7:
        mov    byte ptr [0x3c1], 0           ; -> obj1:0x03C1
        mov    byte ptr [0x3c7], 0           ; -> obj1:0x03C7
        mov    byte ptr [0x3c3], 0           ; -> obj1:0x03C3
        pop    eax
        ret    
loc_7_01BE:
        xor    eax, eax
        mov    byte ptr [0x3c0], al          ; -> loc_1_03C0
        mov    byte ptr [0x3c1], al          ; -> obj1:0x03C1
        cmp    byte ptr [0x3c7], 1           ; -> obj1:0x03C7
        jne    loc_7_01DA
        mov    byte ptr [0x3c7], 0xff        ; -> obj1:0x03C7
loc_7_01DA:
        mov    byte ptr [0x3c3], al          ; -> obj1:0x03C3
        mov    dword ptr [0x3bc], eax        ; -> loc_1_03BC
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [0x352], eax        ; -> obj5:0x0352
        mov    dword ptr [0x3c8], eax        ; -> loc_1_03C8
        xchg   edx, eax
        push   eax
        xchg   byte ptr [0x1f9], al          ; -> obj5:0x01F9
        xor    al, byte ptr [0x1f9]          ; -> obj5:0x01F9
        je     loc_7_021D
        test   byte ptr [0x1f9], al          ; -> obj5:0x01F9
        jne    loc_7_021D
        mov    eax, dword ptr [0x3cc]        ; -> loc_1_03CC
        and    eax, 0x4001
        cmp    eax, 0x4001
        jne    loc_7_021D
loc_7_021D:
        pop    eax
        mov    byte ptr [0x3c6], 0           ; -> loc_1_03C6
        clc    
        ret    
loc_7_0227:
        xor    ebx, ebx
        mov    bx, dx
        shr    edx, 0x10
        xor    eax, eax
        xor    ecx, ecx
        mov    cl, dl
        mov    al, dh
        jmp    sub_4_0E78
loc_7_023C:
        bsf    eax, edx
        je     loc_7_024E
        cmp    ax, 0xc
        ja     loc_7_024E
        jmp    dword ptr [eax*4 + 0x2ee]     ; -> obj5:0x02EE
loc_7_024E:
        clc    
        ret    
loc_7_0250:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xchg   ebx, eax
        mov    ebx, edx
        push   0
        push   ebx
        push   0x1f9                         ; -> loc_4_01F9
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        clc    
        ret    
loc_7_026C:
        mov    dword ptr [0x35a], 0          ; -> obj5:0x035A
        cmp    byte ptr [0x3c7], 0xff        ; -> obj1:0x03C7
        jne    loc_7_0284
        jmp    loc_7_0401
loc_7_0284:
        cmp    byte ptr [0x1f9], 0           ; -> obj5:0x01F9
        jne    loc_7_0403
        test   byte ptr [0x224], 4           ; -> obj5:0x0224
        je     loc_7_0403
        test   byte ptr [0x224], 2           ; -> obj5:0x0224
        je     loc_7_0403
        cmp    byte ptr [0x3c3], 0           ; -> obj1:0x03C3
        jne    loc_7_0301
        cmp    byte ptr [0x3c7], 1           ; -> obj1:0x03C7
        jne    loc_7_0403
        test   byte ptr [0x224], 1           ; -> obj5:0x0224
        jne    loc_7_02F0
        test   byte ptr [0x224], 8           ; -> obj5:0x0224
        je     loc_7_02F0
        test   byte ptr [0x224], 0x10        ; -> obj5:0x0224
        je     loc_7_02F0
        mov    eax, 0
        mov    ebx, 0x1f4
        mov    ecx, 3
        call   VMINI_Siren
loc_7_02F0:
        mov    byte ptr [0x3c3], 1           ; -> obj1:0x03C3
        mov    eax, 0xfa0
        jmp    loc_7_03E1
loc_7_0301:
        cmp    byte ptr [0x3c3], 1           ; -> obj1:0x03C3
        jne    loc_7_0379
        cmp    byte ptr [0x3c7], 1           ; -> obj1:0x03C7
        jne    loc_7_0403
        test   byte ptr [0x224], 1           ; -> obj5:0x0224
        je     loc_7_034E
        mov    byte ptr [0x3c3], 2           ; -> obj1:0x03C3
        call   sub_7_008B
        test   byte ptr [0x224], 0x10        ; -> obj5:0x0224
        je     loc_7_0349
        mov    eax, 2
        mov    ebx, 0xc8
        mov    ecx, 1
        call   sub_4_0E78
loc_7_0349:
        jmp    loc_7_0403
loc_7_034E:
        test   byte ptr [0x224], 0x10        ; -> obj5:0x0224
        je     loc_7_036B
        mov    eax, 1
        mov    ebx, 0xc8
        mov    ecx, 1
        call   VMINI_Siren
loc_7_036B:
        mov    byte ptr [0x3c3], 2           ; -> obj1:0x03C3
        mov    eax, 0xfa0
        jmp    loc_7_03E1
loc_7_0379:
        cmp    byte ptr [0x3c3], 2           ; -> obj1:0x03C3
        jne    loc_7_03B6
        cmp    byte ptr [0x3c7], 1           ; -> obj1:0x03C7
        jne    loc_7_0403
        test   byte ptr [0x224], 0x10        ; -> obj5:0x0224
        je     loc_7_03A8
        mov    eax, 1
        mov    ebx, 0xc8
        mov    ecx, 2
        call   VMINI_Siren
loc_7_03A8:
        mov    byte ptr [0x3c3], 3           ; -> obj1:0x03C3
        mov    eax, 0xfa0
        jmp    loc_7_03E1
loc_7_03B6:
        test   byte ptr [0x224], 0x10        ; -> obj5:0x0224
        je     loc_7_03D3
        mov    eax, 1
        mov    ebx, 0xc8
        mov    ecx, 3
        call   VMINI_Siren
loc_7_03D3:
        mov    byte ptr [0x3c3], 4           ; -> obj1:0x03C3
        call   sub_7_008B
        jmp    loc_7_0403
loc_7_03E1:
        pushal 
        mov    esi, 0x26c                    ; -> loc_7_026C
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    edx, edx
        mov    eax, 0xfa0
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x35a], esi        ; -> obj5:0x035A
        popal  
loc_7_0401:
        clc    
        ret    
loc_7_0403:
        mov    byte ptr [0x3c1], 0           ; -> obj1:0x03C1
        mov    byte ptr [0x3c7], 0           ; -> obj1:0x03C7
        mov    byte ptr [0x3c3], 0           ; -> obj1:0x03C3
        mov    dword ptr [0x35a], 0          ; -> obj5:0x035A
        clc    
        ret    
loc_7_0424:
        mov    byte ptr [0x3c3], 0           ; -> obj1:0x03C3
        mov    esi, 0x26c                    ; -> loc_7_026C
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    edx, edx
        mov    eax, 0xfa0
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x35a], esi        ; -> obj5:0x035A
        clc    
        ret    
loc_7_044B:
        mov    byte ptr [0x3c7], 0           ; -> obj1:0x03C7
        cmp    dword ptr [0x35a], 0          ; -> obj5:0x035A
        je     loc_7_0473
        push   esi
        mov    esi, dword ptr [0x35a]        ; -> obj5:0x035A
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    dword ptr [0x35a], 0          ; -> obj5:0x035A
        pop    esi
loc_7_0473:
        clc    
        ret    
sub_7_0475:
        mov    esi, 0x3da                    ; -> loc_1_03DA
        mov    ecx, 2
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x9a  R-X
loc_8_0000:
        cmp    dword ptr [esp + 4], 0x54
        jne    loc_8_0013
        pushal 
        call   sub_4_0338
        call   sub_4_1452
        popal  
loc_8_0013:
        or     eax, 0xffffffff
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_0020:
        sub    esp, 8
        push   0
        lea    eax, [esp + 4]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_8_008C
        add    esp, 0xc
        cmp    eax, 5
        jne    loc_8_003F
        sub    eax, eax
        jmp    loc_8_0088
loc_8_003F:
        cmp    eax, 4
        jne    loc_8_0048
        sub    eax, eax
        jmp    loc_8_0088
loc_8_0048:
        cmp    eax, 3
        jne    loc_8_0051
        sub    eax, eax
        jmp    loc_8_0088
loc_8_0051:
        cmp    eax, 0xd
        mov    eax, 0
        je     loc_8_0088
        mov    dword ptr [esp + 4], 0x46
        lea    eax, [esp + 4]
        push   0
        push   eax
        push   dword ptr [esp + 0x18]
        push   1
        push   0x11c8                        ; -> obj5:0x11C8
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_8_0094
        add    esp, 0x1c
        mov    eax, 1
loc_8_0088:
        add    esp, 8
        ret    
sub_8_008C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        add    byte ptr [eax], al
sub_8_0094:
        VxDCall  Dev_2333.Service_803Eh          ; cd203e803323
VxD__LTEXT ENDS
