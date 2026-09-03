; VMOUSE -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 5438 instructions recovered by recursive descent

Declare_Virtual_Device VMOUSE, 4, 0, VMOUSE_Control, 0000Ch, Undefined_Init_Order
;   DDK version 0400h   init_order 034000000h   flags 00000h
;   service table: obj1:0x60c, 12 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x8b4  R-X
VMOUSE_Control:
        cmp    eax, 0xf
        je     loc_3_0000
        cmp    eax, 8
        je     sub_3_0DE0
        cmp    eax, 9
        je     sub_3_06F3
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 2
        je     loc_2_016D
        cmp    eax, 1
        je     loc_2_00DC
        cmp    eax, 0x22
        je     loc_3_3444
        cmp    eax, 0xc
        je     loc_3_00AD
        cmp    eax, 6
        je     loc_3_00E0
        cmp    eax, 5
        je     loc_3_104C
        cmp    eax, 5
        je     sub_3_0DFD
        clc    
        ret    
        db  000h, 000h, 000h
VMD_Post_Absolute_Pointer_Message:
        pushal 
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_009D
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
        bts    dword ptr [edx], 0
        jb     loc_1_009D
        mov    dword ptr [0x768], esi        ; -> obj1:0x0768
        mov    dword ptr [0x76c], edi        ; -> obj1:0x076C
        mov    byte ptr [0x770], al          ; -> obj1:0x0770
        mov    esi, 0xb4e                    ; -> loc_3_0B4E
        jmp    loc_1_00D7
loc_1_009D:
        popal  
        clc    
        ret    
VMD_Post_Pointer_Message:
        pushal 
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        test   byte ptr [edx], 0xc1
        jne    loc_1_00FF
        or     byte ptr [edx], 1
        mov    dword ptr [0x758], esi        ; -> obj1:0x0758
        mov    dword ptr [0x75c], edi        ; -> obj1:0x075C
        mov    byte ptr [0x767], al          ; -> obj1:0x0767
        mov    esi, dword ptr [esp + 0x14]
        mov    dword ptr [0x760], esi        ; -> obj1:0x0760
        mov    esi, 0x9ec                    ; -> loc_3_09EC
loc_1_00D7:
        cmp    byte ptr [0x771], 0           ; -> loc_1_0771
        je     loc_1_00EC
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        mov    word ptr [0x764], ax          ; -> obj1:0x0764
loc_1_00EC:
        mov    eax, 0x400000
        mov    ecx, 9
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edx + 4], esi
loc_1_00FF:
        cmp    byte ptr [0x698], 1           ; -> loc_1_0698
        jne    loc_1_010E
        VxDCall  SHELL.SHELL_Update_User_Activity ; cd200c001700
loc_1_010E:
        popal  
        clc    
        ret    
        db  000h, 000h, 000h
loc_1_0114:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        push   edx
        mov    esi, 0x23c
        mov    edi, 0x23e
        mov    cl, 4
        mov    al, 0x90
        mov    edx, edi
        out    dx, al
        jmp    loc_1_012E
loc_1_012E:
        mov    edx, esi
        in     al, dx
        jmp    loc_1_0133
loc_1_0133:
        ror    ax, cl
        mov    al, 0xb0
        mov    edx, edi
        out    dx, al
        jmp    loc_1_013D
loc_1_013D:
        mov    edx, esi
        in     al, dx
        jmp    loc_1_0142
loc_1_0142:
        rol    ax, cl
        movsx  eax, al
        push   eax
        mov    al, 0xd0
        mov    edx, edi
        out    dx, al
        jmp    loc_1_0150
loc_1_0150:
        mov    edx, esi
        in     al, dx
        jmp    loc_1_0155
loc_1_0155:
        ror    ax, cl
        mov    al, 0xf0
        mov    edx, edi
        out    dx, al
        jmp    loc_1_015F
loc_1_015F:
        mov    edx, esi
        in     al, dx
        jmp    loc_1_0164
loc_1_0164:
        mov    ch, al
        rol    ax, cl
        movsx  eax, al
        push   eax
        xor    al, al
        mov    edx, edi
        out    dx, al
        jmp    loc_1_0174
loc_1_0174:
        pop    edi
        pop    esi
        not    ch
        xor    al, al
        test   ch, 0x80
        je     loc_1_0181
        or     al, 0x20
loc_1_0181:
        test   ch, 0x20
        je     loc_1_0188
        or     al, 0x10
loc_1_0188:
        pop    edx
        movzx  ecx, al
        xor    cl, byte ptr [0x774]          ; -> obj1:0x0774
        or     ecx, esi
        or     ecx, edi
        je     loc_1_01A3
        mov    byte ptr [0x774], al          ; -> obj1:0x0774
        VxDCall  VMD.Service_8003h               ; cd2003800c00
loc_1_01A3:
        clc    
        ret    
        db  000h, 000h, 000h
loc_1_01A8:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0xc2
        je     loc_1_01C2
        mov    ecx, dword ptr [0x778]        ; -> obj1:0x0778
        mov    edx, dword ptr [0x77c]        ; -> obj1:0x077C
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_01C2:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_1_01D7
        db  08Bh, 0C0h
loc_1_01CC:
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ah, 0xc2
        je     loc_1_01D7
loc_1_01D5:
        stc    
        ret    
loc_1_01D7:
        cmp    byte ptr [0x796], 0xff        ; -> obj1:0x0796
        je     loc_1_01D5
        mov    esi, dword ptr [0x790]        ; -> obj1:0x0790
        add    esi, ebx
        movzx  eax, al
        cmp    eax, 7
        ja     loc_1_0200
        cmp    byte ptr [0x78b], 0           ; -> loc_1_078B
        je     loc_1_022C
        jmp    dword ptr [eax*4 + 0x7a0]     ; -> obj1:0x07A0
loc_1_0200:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 1
        clc    
        ret    
loc_1_020B:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 2
        clc    
        ret    
loc_1_0216:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 5
        clc    
        ret    
loc_1_0221:
        and    word ptr [ebp + 0x2c], 0xfffe
        mov    byte ptr [ebp + 0x1d], 0
        clc    
        ret    
loc_1_022C:
        cmp    eax, 7
        je     loc_1_0221
        mov    al, byte ptr [eax + 0x7c0]    ; -> loc_1_07C0
        mov    byte ptr [ebp + 0x1d], al
        cmp    eax, 4
        jne    loc_1_0243
        mov    byte ptr [ebp + 0x1c], 0xfe
loc_1_0243:
        or     word ptr [ebp + 0x2c], 1
        clc    
        ret    
loc_1_024A:
        cmp    byte ptr [ebp + 0x11], 1
        ja     loc_1_020B
        jb     loc_1_025D
        cmp    dword ptr [esi], 0
        je     loc_1_0216
        mov    byte ptr [esi + 5], 0xff
        jmp    loc_1_0221
loc_1_025D:
        mov    byte ptr [esi + 5], 0
        jmp    loc_1_0221
loc_1_0263:
        mov    byte ptr [ebp + 0x11], 0
        mov    byte ptr [esi + 6], 0
loc_1_026B:
        mov    dword ptr [esi], 0
        mov    byte ptr [esi + 5], 0
        jmp    loc_1_0221
loc_1_0277:
        cmp    byte ptr [ebp + 0x11], 0
        jne    loc_1_0221
        mov    byte ptr [esi + 6], 0xff
        jmp    loc_1_0221
loc_1_0283:
        mov    byte ptr [ebp + 0x11], 0
        cmp    byte ptr [esi + 6], 0xff
        jne    loc_1_0221
        mov    ax, word ptr [0x784]          ; -> obj1:0x0784
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_1_0221
loc_1_0299:
        mov    cl, byte ptr [ebp + 0x11]
        cmp    cl, 2
        ja     loc_1_020B
        je     loc_1_0221
        test   cl, cl
        jne    loc_1_0221
        cmp    byte ptr [esi + 5], 0xff
        jne    loc_1_02BC
        or     cl, 0x20
loc_1_02BC:
        mov    dl, byte ptr [0x794]          ; -> obj1:0x0794
        add    dl, 2
        and    dl, 5
        or     cl, dl
        mov    byte ptr [ebp + 0x10], cl
        mov    byte ptr [ebp + 0x18], 3
        mov    byte ptr [ebp + 0x14], 0x28
        jmp    loc_1_0221
loc_1_02DA:
        mov    cx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x10]
        mov    dword ptr [esi], ecx
        mov    byte ptr [esi + 4], 0
        test   dword ptr [ebx], 0x20
        je     loc_1_0221
        mov    byte ptr [esi + 4], 0xff
        jmp    loc_1_0221
loc_1_0300:
        mov    edi, eax
        mov    esi, edx
        test   byte ptr [0x78a], 1           ; -> obj1:0x078A
        je     loc_1_032C
        mov    dx, 0x311
        in     al, dx
        test   al, 2
        je     loc_1_032C
        mov    ah, al
        mov    dx, 0x310
        in     al, dx
        test   ah, 0x20
        je     loc_1_0342
        cmp    al, 0xaa
        jne    loc_1_048A
        je     loc_1_0342
loc_1_032C:
        in     al, 0x64
        test   al, 1
        je     loc_1_0488
        test   al, 0x20
        je     loc_1_0488
        mov    ah, al
        in     al, 0x60
loc_1_0342:
        cmp    byte ptr [0x797], 0xff        ; -> obj1:0x0797
        je     loc_1_048A
        mov    ecx, dword ptr [0x79a]        ; -> loc_1_079A
        shl    ecx, 0x10
        shr    ecx, 8
        mov    cl, al
        mov    dword ptr [0x79a], ecx        ; -> loc_1_079A
        cmp    byte ptr [0x799], 1           ; -> obj1:0x0799
        jb     loc_1_039A
        cmp    cx, 0xaa00
        jne    loc_1_039A
        cmp    byte ptr [0x78c], 1           ; -> obj1:0x078C
        jne    loc_1_039A
loc_1_037C:
        xor    eax, eax
        mov    byte ptr [0x799], al          ; -> obj1:0x0799
        mov    dword ptr [0x79a], eax        ; -> loc_1_079A
        mov    edx, esi
        mov    esi, 0xec6                    ; -> loc_3_0EC6
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_048A
loc_1_039A:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        xchg   dword ptr [0x786], eax        ; -> obj1:0x0786
        movzx  ebx, byte ptr [0x799]         ; -> obj1:0x0799
        or     ebx, ebx
        je     loc_1_03C2
        sub    eax, dword ptr [0x786]        ; -> obj1:0x0786
        neg    eax
        cmp    eax, 0xc8
        jb     loc_1_03C2
        xor    ebx, ebx
loc_1_03C2:
        inc    ebx
        mov    byte ptr [0x799], bl          ; -> obj1:0x0799
        cmp    bl, 3
        jb     loc_1_048A
        mov    byte ptr [0x799], 0           ; -> obj1:0x0799
        mov    eax, edi
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    eax, ecx
        shr    ecx, 0x10
        mov    byte ptr [0x794], cl          ; -> obj1:0x0794
sub_1_03EC:
        cmp    word ptr [0x784], 0           ; -> obj1:0x0784
        je     loc_1_0413
        mov    ecx, eax
        shr    ecx, 0x10
        or     cl, cl
        jns    loc_1_0403
        or     ah, ah
        jns    loc_1_0408
loc_1_0403:
        jmp    loc_1_037C
loc_1_0408:
        and    ah, 0x7f
        test   cl, 0x40
        je     loc_1_0413
        or     ah, 0x80
loc_1_0413:
        movzx  ecx, al
        jecxz  loc_1_0428
        neg    cl
        xor    eax, 0x200000
        test   eax, 0x200000
        je     loc_1_0428
        dec    ch
loc_1_0428:
        movsx  edi, cx
        movzx  ecx, ah
        test   eax, 0x100000
        je     loc_1_0437
        dec    ch
loc_1_0437:
        mov    ebx, esi
        movsx  esi, cx
        shr    eax, 0x10
        cmp    word ptr [0x784], 0           ; -> obj1:0x0784
        je     loc_1_046B
        mov    dl, al
        xor    al, al
        test   dl, 1
        je     loc_1_0454
        or     al, 0x20
loc_1_0454:
        test   dl, 2
        je     loc_1_045B
        or     al, 0x10
loc_1_045B:
        test   dl, 4
        je     loc_1_0462
        or     al, 8
loc_1_0462:
        test   dl, 8
        je     loc_1_0480
        or     al, 4
        jmp    loc_1_0480
loc_1_046B:
        and    eax, 3
        je     loc_1_0480
        btr    eax, 0
        jae    loc_1_0478
        or     al, 0x20
loc_1_0478:
        btr    eax, 1
        jae    loc_1_0480
        or     al, 0x10
loc_1_0480:
        mov    edx, ebx
        VxDCall  VMD.Service_8003h               ; cd2003800c00
loc_1_0488:
        stc    
        ret    
loc_1_048A:
        mov    eax, edi
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
loc_1_0494:
        push   edx
        mov    edx, 0x238
        jmp    loc_1_04A2
loc_1_049C:
        push   edx
        mov    edx, 0x23c
loc_1_04A2:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 0x20
        out    dx, al
        cmp    byte ptr [0x7d4], 0x11        ; -> loc_1_07D4
        ja     loc_1_04BF
        push   eax
        in     al, dx
        in     al, dx
        in     al, dx
        pop    eax
        out    dx, al
loc_1_04BF:
        dec    edx
        mov    al, 1
        out    dx, al
        inc    edx
        jmp    loc_1_04C6
loc_1_04C6:
        in     al, dx
        movsx  esi, al
        dec    edx
        mov    al, 2
        out    dx, al
        inc    edx
        jmp    loc_1_04D1
loc_1_04D1:
        in     al, dx
        movsx  edi, al
        dec    edx
        mov    al, 0
        out    dx, al
        inc    edx
        jmp    loc_1_04DC
loc_1_04DC:
        in     al, dx
        mov    bl, al
        dec    edx
        mov    al, 7
        out    dx, al
        inc    edx
        jmp    loc_1_04E6
loc_1_04E6:
        in     al, dx
        and    al, 0xdf
        jmp    loc_1_04EB
loc_1_04EB:
        out    dx, al
        xor    eax, eax
        test   bl, 4
        je     loc_1_04F5
        or     al, 0x20
loc_1_04F5:
        test   bl, 1
        je     loc_1_04FC
        or     al, 0x10
loc_1_04FC:
        pop    edx
        sti    
        VxDCall  VMD.Service_8003h               ; cd2003800c00
loc_1_0504:
        mov    edi, edx
        movzx  edx, word ptr [edi]
        add    dl, 2
        push   eax
        in     al, dx
        test   al, 1
        pop    eax
        jne    loc_1_0546
        sub    dl, 2
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        in     al, dx
        mov    ah, al
        add    dl, 5
        in     al, dx
        sub    dl, 5
        xchg   al, ah
        test   ah, 2
        jne    loc_1_0553
        movzx  ecx, byte ptr [edi + 7]
        jecxz  loc_1_0548
        test   al, 0x40
        jne    loc_1_0548
        mov    byte ptr [edi + ecx + 3], al
        inc    ecx
        cmp    byte ptr [edi + 2], cl
        je     loc_1_0559
        mov    byte ptr [edi + 7], cl
loc_1_0544:
        clc    
        ret    
loc_1_0546:
        stc    
        ret    
loc_1_0548:
        mov    byte ptr [edi + 3], al
        mov    byte ptr [edi + 7], 1
        test   al, 0x40
        jne    loc_1_0544
loc_1_0553:
        mov    byte ptr [edi + 7], 0
        jmp    loc_1_0544
loc_1_0559:
        mov    ah, byte ptr [edi + 2]
        mov    byte ptr [edi + 7], 0
        mov    edx, edi
        mov    bx, word ptr [edx + 3]
        mov    al, bl
        shl    al, 6
        or     al, bh
        movsx  esi, al
        mov    al, bl
        shr    al, 2
        shl    al, 6
        or     al, byte ptr [edx + 5]
        movsx  edi, al
        mov    al, bl
        and    al, 0x30
        cmp    ah, 4
        jne    loc_1_058F
        mov    bl, byte ptr [edx + 6]
        and    bl, 0xc
        or     al, bl
loc_1_058F:
        mov    edx, dword ptr [edx + 8]
        VxDCall  VMD.Service_8003h               ; cd2003800c00
        jmp    loc_1_05A0
        db  0FFh, 025h
        dd  loc_1_07E0               ; fixup
loc_1_05A0:
        pushal 
        movzx  ecx, word ptr [edi]
        jecxz  loc_1_0602
        add    edi, 2
loc_1_05A9:
        movzx  edx, word ptr [edi]
        mov    esi, dword ptr [edi + 2]
        cmp    edx, 0x3ce
        je     loc_1_05E7
        cmp    edx, 0x3c4
        je     loc_1_05EF
        cmp    edx, 0x3cf
        je     loc_1_05F7
        cmp    edx, dword ptr [0x7f8]        ; -> loc_1_07F8
        je     loc_1_05DF
        cmp    edx, 0x3c5
        jne    loc_1_05FD
        mov    dword ptr [0x7e8], esi        ; -> sub_1_07E8
        jmp    loc_1_05FD
loc_1_05DF:
        mov    dword ptr [0x7f4], esi        ; -> sub_1_07F4
        jmp    loc_1_05FD
loc_1_05E7:
        mov    dword ptr [0x7ec], esi        ; -> sub_1_07EC
        jmp    loc_1_05FD
loc_1_05EF:
        mov    dword ptr [0x7e4], esi        ; -> sub_1_07E4
        jmp    loc_1_05FD
loc_1_05F7:
        mov    dword ptr [0x7f0], esi        ; -> sub_1_07F0
loc_1_05FD:
        add    edi, 6
        loop   loc_1_05A9
loc_1_0602:
        popal  
        jmp    loc_1_07E0
        db  000h, 000h, 000h
        dd  VMD_Get_Version          ; fixup
        dd  VMD_Set_Mouse_Type       ; fixup
        dd  VMD_Get_Mouse_Owner      ; fixup
        dd  VMD_Post_Pointer_Message ; fixup
        dd  VMD_Set_Cursor_Proc      ; fixup
        dd  VMD_Call_Cursor_Proc     ; fixup
        dd  VMD_Set_Mouse_Data       ; fixup
        dd  VMD_Get_Mouse_Data       ; fixup
        dd  VMD_Manipulate_Pointer_Message ; fixup
        dd  VMD_Set_Middle_Button    ; fixup
        dd  VMD_Enable_Disable_Mouse_Events ; fixup
        dd  VMD_Post_Absolute_Pointer_Message ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Ch                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VMOUSE  '                  ; DDB_Name
        dd  034000000h                  ; DDB_Init_Order
        dd  VMOUSE_Control           ; DDB_Control_Proc
        dd  VMOUSE_PM_API            ; DDB_V86_API_Proc
        dd  VMOUSE_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x060C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0696:
        add    al, byte ptr [eax]
loc_1_0698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_069C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- dispatch table (43 entries) ----
        dd  loc_3_0146
        dd  loc_3_0168
        dd  sub_3_0194
        dd  loc_3_01B1
        dd  loc_3_01C8
        dd  loc_3_01FA
        dd  loc_3_0238
        dd  loc_3_0277
        dd  loc_3_0296
        dd  loc_3_10BF
        dd  loc_3_1069
        dd  loc_3_02B5
        dd  loc_3_02C6
        dd  loc_3_02F6
        dd  loc_3_02FA
        dd  loc_3_02FE
        dd  loc_3_0318
        dd  loc_3_0112
        dd  loc_3_0112
        dd  loc_3_0346
        dd  loc_3_0347
        dd  loc_3_036C
        dd  loc_3_0380
        dd  loc_3_039E
        dd  loc_3_03BE
        dd  loc_3_03EC
        dd  loc_3_0421
        dd  loc_3_0463
        dd  loc_3_047A
        dd  loc_3_047B
        dd  loc_3_048B
        dd  loc_3_0495
        dd  loc_3_04AC
        dd  loc_3_04BE
        dd  loc_3_04D6
        dd  loc_3_04E3
        dd  loc_3_04EC
        dd  loc_3_050A
        dd  loc_3_0535
        dd  loc_3_054E
        dd  loc_3_0570
        dd  loc_3_05C3
        dd  loc_3_05F6
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_0771:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_078B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0795:
        add    byte ptr [eax], al
        db  000h
loc_1_0798:
        inc    dword ptr [eax]
loc_1_079A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_024A
        dd  loc_1_0263
        dd  loc_1_0277
        dd  loc_1_0221
        dd  loc_1_0283
        dd  loc_1_026B
        dd  loc_1_0299
        dd  loc_1_02DA
loc_1_07C0:
        add    eax, dword ptr [ebx + eax]
        add    eax, dword ptr [esp + eax]
        add    al, 0
loc_1_07C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07D4:
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_07D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_07E4:
        mov    ch, 0x26
        add    byte ptr [eax], al
sub_1_07E8:
        mov    ch, 0x26
        add    byte ptr [eax], al
sub_1_07EC:
        mov    ch, 0x26
        add    byte ptr [eax], al
sub_1_07F0:
        mov    ch, 0x26
        add    byte ptr [eax], al
sub_1_07F4:
        mov    ch, 0x26
        add    byte ptr [eax], al
loc_1_07F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07FC:
        cld    
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [0x8a000019], bh     ; -> loc_3_193D
        sbb    dword ptr [eax], eax
        add    byte ptr [edi], ah            ; -> sub_3_2327
        and    eax, dword ptr [eax]
        add    byte ptr [edx], bh            ; -> sub_3_1A3A
        sbb    al, byte ptr [eax]
        add    byte ptr [edx + 0x15], dl     ; -> loc_3_1552
        add    byte ptr [eax], al
        loope  loc_1_0848
        add    byte ptr [eax], al
        leave  
        add    byte ptr es:[eax], al
        add    ebx, dword ptr [ecx]
        add    byte ptr [eax], al
        and    dl, byte ptr [0x15230000]     ; -> loc_3_1523
        add    byte ptr [eax], al
        or     ah, byte ptr [edi]
        add    byte ptr [eax], al
        or     esp, dword ptr [edi]
        add    byte ptr [eax], al
        and    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
loc_1_0848:
        add    eax, dword ptr [edi]
        add    al, 5
        push   es
        or     eax, 0x11100f0e
        adc    dl, byte ptr [ebx]
loc_1_0854:
        add    al, byte ptr [edx]
        add    dword ptr [esp + eax], eax
        add    dword ptr [ecx + eax], eax
        add    al, 8
loc_1_085E:
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
loc_1_0868:
        inc    eax
        add    eax, ecx
        add    byte ptr [eax + 1], al
        enter  -0x8000, 2
        enter  0x4000, 1
        enter  -0x8000, 2
        enter  -0x8000, 2
        pop    esi
        add    dword ptr [eax - 0x7ffea1fe], eax
        add    ah, al
        add    dword ptr [eax + 0x4001e002], eax
        add    eax, ecx
        add    byte ptr [eax + 0x40], al
        inc    eax
        and    byte ptr [eax - 0x80], al
        add    byte ptr [eax], 0
        add    byte ptr [ebp + 0x1f], al
        test   dword ptr [esi], edi
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        add    eax, 0x5960596
        cli    
        push   ebx
        jb     0x915
        popal  
        insb   byte ptr es:[edi], dx
        dec    ebp
        outsd  dx, dword ptr [esi]
        jne    0x925
        db  065h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x598  R-X
loc_2_0000:
        pushal 
        call   sub_2_0180
        popal  
        movzx  eax, word ptr [0x463]
        mov    dword ptr [0x7f8], eax        ; -> loc_1_07F8
        mov    eax, 0x30018
        mov    esi, 0x7c1                    ; -> loc_3_07C1
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    eax, 0x10095
        mov    esi, 0x5a0                    ; -> loc_1_05A0
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_2_005D
        VxDCall  VKD.VKD_Get_Version             ; cd2000000d00
        jb     loc_2_0056
        cmp    eax, 0x20a
        jae    loc_2_005D
        mov    eax, 0x30001
        mov    esi, 0x66                     ; -> loc_2_0066
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_2_005D
loc_2_0056:
        xor    byte ptr [0x771], 1           ; -> loc_1_0771
loc_2_005D:
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  obj2:0x0504              ; fixup
loc_2_0066:
        cmp    word ptr [edi], 1
        je     loc_2_0072
        jmp    dword ptr [0x504]             ; -> obj2:0x0504
loc_2_0072:
        call   dword ptr [0x504]             ; -> obj2:0x0504
        jb     loc_2_007F
        mov    dword ptr [0], eax            ; -> obj4:0x0000
loc_2_007F:
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  obj2:0x0508              ; fixup
loc_2_0088:
        cmp    eax, 0x15
        jne    loc_2_00BA
        cmp    byte ptr [0x1c5], 0           ; -> loc_4_01C5
        jne    loc_2_00BA
        pushal 
        mov    eax, 0x10041
        mov    esi, 0x88                     ; -> loc_2_0088
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        mov    eax, 0x11
        mov    esi, 0xc0                     ; -> loc_2_00C0
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        popal  
        clc    
        ret    
loc_2_00BA:
        jmp    dword ptr [0x508]             ; -> obj2:0x0508
loc_2_00C0:
        xor    eax, eax
        cmp    byte ptr [0x1c5], al          ; -> loc_4_01C5
        jne    loc_2_00D5
        mov    esi, 0xd7                     ; -> loc_2_00D7
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
loc_2_00D5:
        stc    
        ret    
loc_2_00D7:
        and    byte ptr [ebp + 0x1c], 0xfb
        ret    
loc_2_00DC:
        mov    dword ptr [0x690], ebx        ; -> obj1:0x0690
        cmp    dword ptr [0x504], 0          ; -> obj2:0x0504
        je     loc_2_011D
        mov    eax, 0x30001
        mov    esi, 0x66                     ; -> loc_2_0066
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        mov    eax, 0x11
        mov    esi, 0xc0                     ; -> loc_2_00C0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x10041
        mov    esi, 0x88                     ; -> loc_2_0088
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_2_016C
loc_2_011D:
        push   0
        push   0xa9
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_2_0136
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0136:
        mov    dword ptr [0x68c], eax        ; -> obj1:0x068C
        VxDCall  SHELL.SHELL_Get_Version         ; cd2000001700
        jb     loc_2_0150
        cmp    eax, 0x314
        jb     loc_2_0150
        inc    byte ptr [0x698]              ; -> loc_1_0698
loc_2_0150:
        call   sub_2_0407
        or     byte ptr [6], 1               ; -> obj4:0x0006
        push   0
        push   0x63c                         ; -> obj1:0x063C
        VxDCall  CONFIGMG._CONFIGMG_Register_DevLoader ; cd2032003300
        pop    ecx
        pop    ecx
        clc    
loc_2_016C:
        ret    
loc_2_016D:
        push   ebx
        call   sub_2_020C
        pop    ebx
        call   sub_3_06F3
        call   sub_2_0387
        clc    
        ret    
sub_2_0180:
        mov    eax, 0x33
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        shl    ecx, 0x10
        mov    cx, dx
        mov    dword ptr [0x74c], ecx        ; -> obj1:0x074C
        push   ecx
        push   0
        push   6
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_2_01AF
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_01AF:
        inc    eax
        mov    edi, eax
        mov    dword ptr [eax + 1], 0x8ca90
        shr    eax, 4
        movzx  ecx, ax
        mov    edx, edi
        and    edx, 0xf
        mov    eax, 0x33
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
        mov    eax, ecx
        shl    eax, 0x10
        add    eax, edx
        xor    edx, edx
        mov    esi, 0xec                     ; -> loc_3_00EC
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    esi, 0xec                     ; -> loc_3_00EC
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jae    loc_2_01F6
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_01F6:
        mov    ecx, eax
        shr    ecx, 0x10
        movzx  edx, ax
        mov    eax, 0x33
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        clc    
        ret    
        db  000h
sub_2_020C:
        pushal 
        mov    esi, 0x562                    ; -> obj2:0x0562
        push   esi
        push   0
        push   0x566                         ; -> obj2:0x0566
        push   2
        VMMCall  VMM._GetRegistryKey             ; cd206f010100
        add    esp, 0x10
        or     eax, eax
        jne    loc_2_0385
        mov    dword ptr [0x56e], 0xa        ; -> obj2:0x056E
        mov    ebx, 0x56a                    ; -> obj2:0x056A
        mov    edi, 0x573                    ; -> loc_2_0573
        push   0x56e                         ; -> obj2:0x056E
        push   edi
        push   ebx
        push   0
        push   0x50c                         ; -> obj2:0x050C
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_026C
        push   esi
        push   ebx
        mov    edi, 0x573                    ; -> loc_2_0573
        call   sub_3_0FC3
        pop    ebx
        pop    esi
loc_2_026C:
        mov    dword ptr [0x56e], 1          ; -> obj2:0x056E
        push   0x56e                         ; -> obj2:0x056E
        push   0xa4                          ; -> obj4:0x00A4
        push   ebx
        push   0
        push   0x516                         ; -> obj2:0x0516
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_02A7
        cmp    byte ptr [0xa4], 0x64         ; -> obj4:0x00A4
        jbe    loc_2_02A7
        mov    byte ptr [0xa4], 0x64         ; -> obj4:0x00A4
loc_2_02A7:
        mov    dword ptr [0x56e], 1          ; -> obj2:0x056E
        push   0x56e                         ; -> obj2:0x056E
        push   0xa5                          ; -> obj4:0x00A5
        push   ebx
        push   0
        push   0x52c                         ; -> obj2:0x052C
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_02E2
        cmp    byte ptr [0xa5], 0x64         ; -> obj4:0x00A5
        jbe    loc_2_02E2
        mov    byte ptr [0xa5], 0x64         ; -> obj4:0x00A5
loc_2_02E2:
        mov    dword ptr [0x56e], 1          ; -> obj2:0x056E
        push   0x56e                         ; -> obj2:0x056E
        push   0x572                         ; -> obj2:0x0572
        push   ebx
        push   0
        push   0x540                         ; -> obj2:0x0540
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_0320
        movzx  eax, byte ptr [0x572]         ; -> obj2:0x0572
        dec    eax
        mov    al, byte ptr [eax + 0x55e]    ; -> obj2:0x055E
        mov    byte ptr [0xa6], al           ; -> obj4:0x00A6
loc_2_0320:
        mov    dword ptr [0x56e], 1          ; -> obj2:0x056E
        push   0x56e                         ; -> obj2:0x056E
        push   0xa7                          ; -> obj4:0x00A7
        push   ebx
        push   0
        push   0x54e                         ; -> obj2:0x054E
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_035E
        movzx  eax, byte ptr [0x572]         ; -> obj2:0x0572
        dec    eax
        mov    al, byte ptr [eax + 0x55e]    ; -> obj2:0x055E
        mov    byte ptr [0xa7], al           ; -> obj4:0x00A7
loc_2_035E:
        mov    dword ptr [0x56e], 1          ; -> obj2:0x056E
        push   0x56e                         ; -> obj2:0x056E
        push   0x798                         ; -> loc_1_0798
        push   ebx
        push   0
        push   0x584                         ; -> obj2:0x0584
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
loc_2_0385:
        popal  
        ret    
sub_2_0387:
        mov    esi, 0x562                    ; -> obj2:0x0562
        push   esi
        push   1
        push   0x566                         ; -> obj2:0x0566
        push   2
        VMMCall  VMM._GetRegistryKey             ; cd206f010100
        add    esp, 0x10
        or     eax, eax
        jne    loc_2_03E5
        movzx  eax, byte ptr [0x694]         ; -> obj1:0x0694
        dec    eax
        jb     loc_2_03E5
        cmp    eax, 3
        ja     loc_2_03E5
        movzx  edi, byte ptr [eax + 0x57e]   ; -> obj2:0x057E
        add    edi, 0xa9                     ; -> obj4:0x00A9
        movzx  eax, byte ptr [edi]
        inc    edi
        movzx  ebx, byte ptr [eax + edi]
        mov    byte ptr [eax + edi], bh
        push   eax
        push   eax
        push   edi
        push   1
        push   0
        push   0x50c                         ; -> obj2:0x050C
        push   dword ptr [esi]
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        pop    eax
        mov    byte ptr [eax + edi], bl
loc_2_03E5:
        ret    
        db  000h, 000h
loc_2_03E8:
        or     ah, ah
        je     loc_2_03F9
        cmp    ah, 0x11
        jne    0x402
        cmp    al, 3
        je     0x402
        cmp    al, 0x30
        je     0x402
loc_2_03F9:
        nop    
        pushfd 
        lcall  0xcf90, 0
        db  0EAh, 000h, 000h, 000h, 000h
sub_2_0407:
        mov    eax, 0x10
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        shl    ecx, 0x10
        mov    cx, dx
        mov    dword ptr [0x7cc], ecx        ; -> loc_1_07CC
        push   ecx
        push   0
        push   0x1f
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_2_0439
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0439:
        mov    edi, eax
        mov    esi, 0x3e8                    ; -> loc_2_03E8
        mov    ecx, 0x1f
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, eax
        mov    dword ptr [edi + 0x14], edx
        mov    dword ptr [edi + 0x1b], edx
        shr    eax, 4
        movzx  ecx, ax
        mov    edx, edi
        and    edx, 0xf
        mov    eax, 0x10
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
        mov    eax, ecx
        shl    eax, 0x10
        add    eax, edx
        mov    dword ptr [0x7d0], eax        ; -> loc_1_07D0
        add    eax, 0x11
        xor    edx, edx
        mov    esi, 0x120d                   ; -> loc_3_120D
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        add    eax, 7
        mov    esi, 0x1233                   ; -> loc_3_1233
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    eax, 0x10
        mov    esi, 0x11a2                   ; -> loc_3_11A2
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        call   sub_2_04AC
        ret    
        db  000h, 000h
sub_2_04AC:
        push   0
        push   0xf8
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_2_04FB
        mov    dword ptr [0x7d8], eax        ; -> loc_1_07D8
        mov    eax, 0x7fc                    ; -> loc_1_07FC
        mov    esi, 0x845                    ; -> obj1:0x0845
        mov    ecx, 0xf
        VxDCall  VMD.Service_0004h               ; cd2004000c00
        VxDCall  VDD.VDD_Get_Version             ; cd2000000a00
        mov    al, 0x20
        cmp    dword ptr [esi + 4], 0x4147454f
        jne    loc_2_04F5
        test   byte ptr [0x487], 0x60
        jne    loc_2_04F5
        shr    al, 1
loc_2_04F5:
        mov    byte ptr [0x840], al          ; -> obj1:0x0840
        ret    
loc_2_04FB:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x6f], cl
        jne    loc_2_0583
        push   esp
        jns    0x584
        add    byte ptr gs:[eax + 0x6f], cl
        jb     loc_2_0583
        jp     0x58b
        outsb  dx, byte ptr [esi]
        je     0x580
        insb   byte ptr es:[edi], dx
        push   ebx
        outsb  dx, byte ptr gs:[esi]
        jae    loc_2_058E
        je     loc_2_0590
        jbe    loc_2_0592
        je     0x5a4
        add    byte ptr [esi + 0x65], dl
        jb     0x5a4
        imul   esp, dword ptr [ebx + 0x61], 0x6e65536c
        jae    0x5a2
        je     0x5a4
        jbe    0x5a6
        je     0x5b8
        add    byte ptr [eax + 0x72], dl
        imul   ebp, dword ptr [ebp + 0x61], 0x75427972
        je     0x5bf
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [ebx + 0x65], dl
        arpl   word ptr [edi + 0x6e], bp
        popal  
        jb     0x5d0
        inc    edx
        jne    0x5ce
        je     0x5cb
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], ah
        or     byte ptr [eax], dl
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x4d], dl
        inc    esp
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0573:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0xb
        adc    al, byte ptr [eax]
loc_2_0583:
        add    byte ptr [ecx + 0x4e], cl
        dec    ecx
        push   esp
        push   eax
        push   ebx
        xor    cl, byte ptr [ebp + 0x4f]
        push   ebp
loc_2_058E:
        push   ebx
        inc    ebp
loc_2_0590:
        inc    ecx
        push   esp
loc_2_0592:
        inc    ebp
        pop    eax
        dec    ecx
        push   esp
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x3708  R-X
loc_3_0000:
        test   edx, edx
        je     9
        cmp    edx, 0xc
        jne    loc_3_005E
        xchg   dword ptr [0x690], ebx        ; -> obj1:0x0690
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        je     loc_3_0060
        mov    eax, 0x10
        xor    ecx, ecx
        mov    esi, 0x67                     ; -> loc_3_0067
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        xchg   dword ptr [edx + 8], esi
        VMMCall  VMM.Cancel_Priority_VM_Event    ; cd2015000100
        call   sub_3_07F5
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        mov    eax, 0x10
        xor    ecx, ecx
        mov    esi, 0x67                     ; -> loc_3_0067
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        call   sub_3_0DB2
loc_3_005E:
        clc    
        ret    
loc_3_0060:
        mov    dword ptr [0x690], edx        ; -> obj1:0x0690
        ret    
loc_3_0067:
        and    dword ptr [edx + 8], 0
        test   byte ptr [edx], 2
        je     loc_3_00AC
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_00AC
        cmp    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        je     loc_3_0099
        test   byte ptr [edx + 0x7d], 1
        je     loc_3_0091
        mov    eax, 0xffffffff
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
loc_3_0091:
        mov    al, 3
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_0099:
        xor    eax, eax
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
        mov    al, 2
        mov    ecx, dword ptr [edx + 0x18]
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_00AC:
        ret    
loc_3_00AD:
        add    ebx, dword ptr [0x68c]        ; -> obj1:0x068C
        xor    esi, esi
        xchg   dword ptr [ebx + 4], esi
        VMMCall  VMM.Cancel_Priority_VM_Event    ; cd2015000100
        xor    esi, esi
        xchg   dword ptr [ebx + 8], esi
        VMMCall  VMM.Cancel_Priority_VM_Event    ; cd2015000100
        clc    
        ret    
VMD_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
VMD_Set_Mouse_Type:
        xor    eax, eax
        stc    
        ret    
VMD_Get_Mouse_Owner:
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        clc    
        ret    
        db  000h, 000h
loc_3_00E0:
        mov    eax, dword ptr [0x74c]        ; -> obj1:0x074C
        mov    dword ptr [0xcc], eax
        clc    
        ret    
loc_3_00EC:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0x2a
        ja     loc_3_0112
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        jmp    dword ptr [eax*4 + 0x6a0]     ; -> obj1:0x06A0
loc_3_0112:
        ret    
sub_3_0113:
        push   edx
        push   0x21
        pop    edx
        xor    ecx, ecx
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        mov    cl, byte ptr [0x695]          ; -> obj1:0x0695
        cmp    cl, 7
        ja     loc_3_0144
        movzx  edx, al
        xor    eax, eax
        inc    eax
        shl    eax, cl
        not    eax
        and    eax, edx
        push   0x21
        pop    edx
        mov    ecx, 4
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
loc_3_0144:
        pop    edx
        ret    
loc_3_0146:
        mov    word ptr [ebp + 0x1c], 0xffff
        movzx  eax, byte ptr [0x696]         ; -> loc_1_0696
        mov    word ptr [ebp + 0x10], ax
        and    byte ptr [edx], 0x7f
        call   sub_3_0113
        or     al, 1
        jmp    loc_3_0711
        db  08Bh, 0C0h
loc_3_0168:
        xor    edi, edi
        mov    al, 9
        VxDCall  VMD.Service_0005h               ; cd2005000c00
        test   byte ptr [edx], 2
        jne    loc_3_0193
        inc    word ptr [edx + 0x76]
        jne    loc_3_0193
        or     byte ptr [edx], 2
        cmp    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        jne    loc_3_0193
        mov    al, 2
        mov    ecx, dword ptr [edx + 0x18]
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_0193:
        ret    
sub_3_0194:
        dec    word ptr [edx + 0x76]
        test   byte ptr [edx], 2
        je     loc_3_01B0
        and    byte ptr [edx], 0xfd
        cmp    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        jne    loc_3_01B0
        mov    al, 3
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_01B0:
        ret    
loc_3_01B1:
        movzx  eax, byte ptr [edx + 0x55]
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [edx + 0x18]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_01C8:
        mov    eax, dword ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        call   sub_3_0618
        mov    dword ptr [edx + 0x14], eax
        call   sub_3_0653
        mov    dword ptr [edx + 0x18], eax
        test   byte ptr [edx], 2
        je     loc_3_01F9
        cmp    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        jne    loc_3_01F9
        mov    ecx, eax
        mov    al, 2
        VxDCall  VMD.Service_0005h               ; cd2005000c00
loc_3_01F9:
        ret    
loc_3_01FA:
        movzx  eax, word ptr [ebp + 0x10]
        mov    esi, 0x30
        test   eax, eax
        je     loc_3_0216
        mov    esi, 0x3c
        dec    eax
        test   eax, eax
        je     loc_3_0216
        mov    esi, 0x48
loc_3_0216:
        movzx  eax, byte ptr [edx + 0x55]
        mov    word ptr [ebp + 0x1c], ax
        add    esi, edx
        xor    eax, eax
        xchg   word ptr [esi + 8], ax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [esi]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_0238:
        movzx  eax, word ptr [ebp + 0x10]
        mov    esi, 0x30
        test   eax, eax
        je     loc_3_0254
        mov    esi, 0x3c
        dec    eax
        test   eax, eax
        je     loc_3_0254
        mov    esi, 0x48
loc_3_0254:
        movzx  eax, byte ptr [edx + 0x55]
        mov    word ptr [ebp + 0x1c], ax
        add    esi, edx
        xor    eax, eax
        xchg   word ptr [esi + 0xa], ax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_0277:
        movzx  esi, word ptr [ebp + 0x18]
        movzx  edi, word ptr [ebp + 0x14]
        test   byte ptr [ebp + 0x19], 0x80
        je     loc_3_0287
        xor    esi, esi
loc_3_0287:
        cmp    esi, edi
        jle    loc_3_028D
        xchg   edi, esi
loc_3_028D:
        mov    word ptr [edx + 0xe], si
        mov    word ptr [edx + 0x12], di
        ret    
loc_3_0296:
        movzx  esi, word ptr [ebp + 0x18]
        movzx  edi, word ptr [ebp + 0x14]
        test   byte ptr [ebp + 0x19], 0x80
        je     loc_3_02A6
        xor    esi, esi
loc_3_02A6:
        cmp    esi, edi
        jle    loc_3_02AC
        xchg   edi, esi
loc_3_02AC:
        mov    word ptr [edx + 0xc], si
        mov    word ptr [edx + 0x10], di
        ret    
loc_3_02B5:
        xor    eax, eax
        xchg   dword ptr [edx + 0x28], eax
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_02C6:
        and    byte ptr [edx], 0xfb
        test   byte ptr [ebx], 0x20
        jne    loc_3_02E4
        movzx  esi, word ptr [ebp + 0x14]
        mov    ax, word ptr [ebp + 0x38]
loc_3_02D6:
        mov    dword ptr [edx + 0x59], esi
        mov    word ptr [edx + 0x5d], ax
        mov    al, byte ptr [ebp + 0x18]
        mov    byte ptr [edx + 0x5f], al
        ret    
loc_3_02E4:
        or     byte ptr [edx], 4
        mov    ax, 0x3814
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    ax, fs
        jmp    loc_3_02D6
loc_3_02F6:
        and    byte ptr [edx], 0xf7
        ret    
loc_3_02FA:
        or     byte ptr [edx], 8
        ret    
loc_3_02FE:
        movzx  eax, word ptr [ebp + 0x18]
        or     eax, eax
        jne    loc_3_0307
        inc    eax
loc_3_0307:
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        or     ax, ax
        jne    loc_3_0314
        inc    eax
loc_3_0314:
        mov    dword ptr [edx + 0x20], eax
        ret    
loc_3_0318:
        movzx  eax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        call   sub_3_0653
        mov    ecx, eax
        movzx  eax, word ptr [ebp + 4]
        shl    eax, 0x10
        mov    ax, word ptr [ebp]
        call   sub_3_0653
        mov    esi, eax
        mov    al, 9
        mov    edi, eax
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_0346:
        ret    
loc_3_0347:
        mov    ax, word ptr [ebp + 0x38]
        xchg   word ptr [edx + 0x5d], ax
        mov    word ptr [ebp + 0x38], ax
        movzx  eax, word ptr [ebp + 0x14]
        xchg   dword ptr [edx + 0x59], eax
        mov    word ptr [ebp + 0x14], ax
        movzx  eax, byte ptr [ebp + 0x18]
        and    al, 0x1f
        xchg   byte ptr [edx + 0x5f], al
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_036C:
        mov    al, 6
        xor    ecx, ecx
        VxDCall  VMD.Service_0005h               ; cd2005000c00
        add    eax, 0xa9
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_3_0380:
        xchg   edi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    ecx, 0xa9
        mov    esi, edx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, 6
        inc    ecx
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_039E:
        xchg   esi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    ecx, 0xa9
        mov    edi, edx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, 6
        push   2
        pop    ecx
        VxDCall  VMD.Service_8005h               ; cd2005800c00
loc_3_03BE:
        mov    al, byte ptr [ebp + 0x18]
        call   sub_3_0698
        jb     loc_3_03E5
        movzx  esi, word ptr [ebp + 0x14]
        mov    cx, word ptr [ebp + 0x38]
        or     cx, cx
        jne    loc_3_03DB
        or     esi, esi
        jne    loc_3_03DB
        xor    al, al
loc_3_03DB:
        mov    byte ptr [edi + 6], al
        mov    word ptr [edi + 4], cx
        mov    dword ptr [edi], esi
        ret    
loc_3_03E5:
        mov    word ptr [ebp + 0x1c], 0xffff
        ret    
loc_3_03EC:
        mov    al, byte ptr [ebp + 0x18]
        call   sub_3_0698
        jne    loc_3_040D
        movzx  eax, byte ptr [edi + 6]
        mov    word ptr [ebp + 0x18], ax
        mov    eax, dword ptr [edi]
        mov    word ptr [ebp + 0x14], ax
        mov    ax, word ptr [edi + 4]
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_3_040D:
        xor    eax, eax
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x18], ax
        mov    word ptr [ebp + 0x14], ax
        dec    eax
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_3_0421:
        mov    edi, 0x64
        movzx  eax, word ptr [ebp + 0x14]
        cmp    eax, edi
        jb     loc_3_0430
        mov    eax, edi
loc_3_0430:
        mov    byte ptr [edx + 0x58], al
        movzx  eax, word ptr [ebp + 0x10]
        cmp    eax, edi
        jb     loc_3_043D
        mov    eax, edi
loc_3_043D:
        mov    byte ptr [edx + 0x56], al
        call   sub_3_06D1
        mov    esi, eax
        movzx  eax, word ptr [ebp + 0x18]
        cmp    eax, edi
        jb     loc_3_0451
        mov    eax, edi
loc_3_0451:
        mov    byte ptr [edx + 0x57], al
        call   sub_3_06D1
        shl    esi, 0x10
        mov    si, ax
        mov    dword ptr [edx + 0x1c], esi
        ret    
loc_3_0463:
        movzx  eax, byte ptr [edx + 0x56]
        mov    word ptr [ebp + 0x10], ax
        mov    al, byte ptr [edx + 0x57]
        mov    word ptr [ebp + 0x18], ax
        mov    al, byte ptr [edx + 0x58]
        mov    word ptr [ebp + 0x14], ax
        ret    
loc_3_047A:
        ret    
loc_3_047B:
        movzx  ecx, word ptr [ebp + 0x10]
        mov    byte ptr [edx + 0x7e], cl
        mov    al, 5
        VxDCall  VMD.Service_0005h               ; cd2005000c00
        ret    
loc_3_048B:
        movzx  ax, byte ptr [edx + 0x7e]
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_3_0495:
        mov    word ptr [ebp + 0x1c], 0xffff
        mov    eax, dword ptr [0x74c]        ; -> obj1:0x074C
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_04AC:
        mov    word ptr [ebp + 0x1c], 0xffff
        movzx  eax, byte ptr [0x696]         ; -> loc_1_0696
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_3_04BE:
        mov    word ptr [ebp + 0x1c], 0xffff
        movzx  eax, byte ptr [0x696]         ; -> loc_1_0696
        mov    word ptr [ebp + 0x10], ax
        xor    al, al
        jmp    loc_3_0711
loc_3_04D6:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    ax, 8
        ja     loc_3_04E2
        mov    byte ptr [edx + 0x78], al
loc_3_04E2:
        ret    
loc_3_04E3:
        movzx  eax, byte ptr [edx + 0x78]
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_3_04EC:
        mov    word ptr [ebp + 0x10], 0x830
        mov    al, byte ptr [0x694]          ; -> obj1:0x0694
        mov    byte ptr [ebp + 0x19], al
        mov    ah, byte ptr [0x695]          ; -> obj1:0x0695
        cmp    al, 4
        jne    loc_3_0506
        xor    ah, ah
loc_3_0506:
        mov    byte ptr [ebp + 0x18], ah
        ret    
loc_3_050A:
        mov    al, 8
        or     al, 0x80
        test   byte ptr [edx + 0x7d], 1
        je     loc_3_0516
        or     al, 0x10
loc_3_0516:
        test   byte ptr [edx + 0x81], 1
        je     loc_3_0521
        or     al, 0x20
loc_3_0521:
        mov    byte ptr [ebp + 0x1d], al
        xor    ah, ah
        mov    word ptr [ebp + 0x18], ax
        mov    word ptr [ebp + 0x14], ax
        dec    al
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_3_0535:
        test   byte ptr [edx], 0x10
        setne  byte ptr [ebp + 0x10]
        mov    eax, dword ptr [edx + 0x83]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_054E:
        mov    eax, dword ptr [edx + 0xa5]
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x10], ax
        xor    eax, eax
        xchg   dword ptr [edx + 0x24], eax
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_0570:
        movzx  ecx, word ptr [ebp + 0x18]
        jecxz  loc_3_05AE
        mov    eax, ecx
        mov    ecx, dword ptr [0x11e]        ; -> loc_4_011E
        mov    edi, dword ptr [0x11a]        ; -> loc_4_011A
        repne scasb al, byte ptr es:[edi]
        jne    loc_3_05AD
        mov    cx, word ptr [ebp + 0x14]
        mov    word ptr [edx + 0x95], cx
        mov    byte ptr [ebp + 0x18], 0
        or     byte ptr [edx + 1], 1
        cmp    al, 0x20
        jne    loc_3_05A1
        xor    al, al
loc_3_05A1:
        mov    word ptr [edx + 0x9f], ax
        jmp    sub_3_12B2
loc_3_05AD:
        ret    
loc_3_05AE:
        mov    word ptr [edx + 0x95], cx
        mov    word ptr [edx + 0x9f], 0x20
        and    byte ptr [edx + 1], 0xfe
        ret    
loc_3_05C3:
        movzx  ecx, word ptr [ebp + 0x18]
        or     ecx, ecx
        jne    loc_3_05D6
        mov    eax, dword ptr [0x11a]        ; -> loc_4_011A
        mov    dword ptr [edx + 0xa1], eax
loc_3_05D6:
        mov    eax, dword ptr [edx + 0xa1]
        movzx  ecx, byte ptr [eax]
        jecxz  loc_3_05E7
        inc    dword ptr [edx + 0xa1]
loc_3_05E7:
        mov    word ptr [ebp + 0x18], cx
        xor    eax, eax
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x14], ax
        ret    
loc_3_05F6:
        mov    ax, word ptr [edx + 0x76]
        mov    word ptr [ebp + 0x1c], ax
        mov    eax, dword ptr [edx + 0x2c]
        mov    word ptr [ebp + 0x18], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x10], ax
        movzx  eax, byte ptr [0x694]         ; -> obj1:0x0694
        mov    word ptr [ebp + 0x14], ax
        ret    
sub_3_0618:
        push   esi
        push   edi
        mov    esi, dword ptr [edx + 0xc]
        mov    edi, dword ptr [edx + 0x10]
        cmp    ax, si
        jg     loc_3_062A
        mov    ax, si
        jmp    loc_3_0632
loc_3_062A:
        cmp    ax, di
        jl     loc_3_0632
        mov    ax, di
loc_3_0632:
        ror    eax, 0x10
        shr    esi, 0x10
        shr    edi, 0x10
        cmp    ax, si
        jg     loc_3_0645
        mov    ax, si
        jmp    loc_3_064D
loc_3_0645:
        cmp    ax, di
        jl     loc_3_064D
        mov    ax, di
loc_3_064D:
        ror    eax, 0x10
        pop    edi
        pop    esi
        ret    
sub_3_0653:
        push   edx
        push   esi
        push   edi
        push   ebx
        mov    esi, eax
        mov    edi, edx
        movsx  ebx, word ptr [edi + 0x89]
        cmp    ebx, 1
        jbe    loc_3_0673
        movsx  eax, si
        cdq    
        idiv   ebx
        imul   eax, ebx
        mov    si, ax
loc_3_0673:
        ror    esi, 0x10
        movsx  ebx, word ptr [edi + 0x8b]
        cmp    ebx, 1
        jbe    loc_3_068E
        movsx  eax, si
        cdq    
        idiv   ebx
        imul   eax, ebx
        mov    si, ax
loc_3_068E:
        ror    esi, 0x10
        mov    eax, esi
        pop    ebx
        pop    edi
        pop    esi
        pop    edx
        ret    
sub_3_0698:
        push   eax
        push   ebx
        push   ecx
        push   esi
        mov    bl, 0xe0
        mov    bh, bl
        and    bh, al
        je     loc_3_06CA
        mov    ecx, 3
        lea    edi, [edx + 0x59]
        xor    esi, esi
loc_3_06AE:
        add    edi, 7
        mov    al, byte ptr [edi + 6]
        and    al, bl
        jne    loc_3_06BA
        mov    esi, edi
loc_3_06BA:
        cmp    al, bh
        loopne loc_3_06AE
        jne    loc_3_06C2
        jmp    loc_3_06CC
loc_3_06C2:
        or     esi, esi
        je     loc_3_06CA
        mov    edi, esi
        jmp    loc_3_06CC
loc_3_06CA:
        shl    bl, 1
loc_3_06CC:
        pop    esi
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_3_06D1:
        push   edx
        mov    cl, 5
        mov    bl, 2
        idiv   cl
        cmp    al, 0xa
        jae    loc_3_06E4
        cmp    al, bl
        ja     loc_3_06EA
        sub    cl, bl
        jmp    loc_3_06EC
loc_3_06E4:
        inc    cl
        sub    al, cl
        jmp    loc_3_06EC
loc_3_06EA:
        sub    al, bl
loc_3_06EC:
        movzx  eax, al
        shl    eax, cl
        pop    edx
        ret    
sub_3_06F3:
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        push   ebx
        call   sub_3_0F98
        pop    ebx
        mov    eax, dword ptr [0x11a]        ; -> loc_4_011A
        mov    dword ptr [edx + 0xa1], eax
        or     al, 1
        jmp    loc_3_0711
loc_3_0711:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_07B7
        test   byte ptr [edx], 0x20
        jne    loc_3_07B7
        test   byte ptr [edx + 1], 1
        je     loc_3_072E
        xor    al, al
loc_3_072E:
        push   eax
        mov    al, 3
        VxDCall  VMD.Service_0005h               ; cd2005000c00
        pop    eax
        or     al, al
        movzx  eax, word ptr [edx + 0x9f]
        je     loc_3_0750
        mov    eax, 0xf00
        call   sub_3_14B1
        and    eax, 0x7f
loc_3_0750:
        call   sub_3_12B2
        mov    al, 0xc
        VxDCall  VMD.Service_0005h               ; cd2005000c00
        mov    byte ptr [edx + 0x7e], 0
        mov    dword ptr [edx + 0xc], 0
        mov    eax, dword ptr [edx + 0x83]
        mov    dword ptr [edx + 0x10], eax
        add    eax, 0x10001
        shr    ax, 1
        ror    eax, 0x10
        shr    ax, 1
        ror    eax, 0x10
        mov    dword ptr [edx + 0x14], eax
        cmp    dword ptr [edx + 0x89], 0
        je     loc_3_0793
        call   sub_3_0653
loc_3_0793:
        mov    dword ptr [edx + 0x18], eax
        mov    dword ptr [edx + 0x20], 0x80010
        mov    byte ptr [edx + 0x5f], 0
        mov    word ptr [edx + 0x76], 0xffff
        call   sub_3_0DB2
        call   sub_3_07F5
        and    dword ptr [edx], 0xc1
loc_3_07B7:
        clc    
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_069C               ; fixup
loc_3_07C1:
        cmp    byte ptr [0x695], 7           ; -> obj1:0x0695
        jbe    loc_3_07CF
        call   sub_3_07D5
loc_3_07CF:
        jmp    loc_1_069C
sub_3_07D5:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_07F4
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
        and    byte ptr [edx], 0x7f
        test   ecx, ecx
        je     loc_3_07F4
        or     byte ptr [edx], 0x80
        call   sub_3_07F5
loc_3_07F4:
        ret    
sub_3_07F5:
        xor    esi, esi
        xchg   dword ptr [edx + 4], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        and    byte ptr [edx], 0xfe
        ret    
VMD_Set_Mouse_Data:
        mov    byte ptr [0x695], al          ; -> obj1:0x0695
        mov    byte ptr [0x694], ah          ; -> obj1:0x0694
        cmp    ah, 4
        jne    loc_3_081B
        mov    byte ptr [0x78b], 1           ; -> loc_1_078B
loc_3_081B:
        test   bl, 1
        je     loc_3_0827
        mov    word ptr [4], cx              ; -> obj4:0x0004
loc_3_0827:
        mov    byte ptr [0x697], dl          ; -> obj1:0x0697
        mov    byte ptr [0x696], dh          ; -> loc_1_0696
        ret    
VMOUSE_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        test   eax, eax
        jne    loc_3_0847
        mov    word ptr [ebp + 0x1c], 0x400
        jmp    loc_3_08F5
loc_3_0847:
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
        test   ah, ah
        jne    loc_3_08FA
        cmp    al, 2
        jb     loc_3_08A5
        je     loc_3_089B
        cmp    al, 4
        jb     loc_3_0914
        je     loc_3_08AF
        cmp    al, 5
        ja     loc_3_090F
        call   sub_3_3378
        mov    al, byte ptr [0x694]          ; -> obj1:0x0694
        mov    ah, byte ptr [0x695]          ; -> obj1:0x0695
        mov    word ptr [ebp + 0x1c], ax
        mov    ax, word ptr [0x17d]          ; -> loc_4_017D
        mov    word ptr [ebp + 0x18], ax
        movzx  ax, byte ptr [0x697]          ; -> obj1:0x0697
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_3_08F5
loc_3_089B:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_3_090F
        jmp    loc_3_08F5
loc_3_08A5:
        test   byte ptr [edx], 0x20
        je     loc_3_090F
        jmp    loc_3_0940
loc_3_08AF:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_3_090F
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_3_090F
        xchg   dword ptr [0x690], ebx        ; -> obj1:0x0690
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_3_08D6
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
loc_3_08D6:
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        mov    eax, 0x10
        xor    ecx, ecx
        mov    esi, 0x67                     ; -> loc_3_0067
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        call   sub_3_0DB2
loc_3_08F5:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_3_08FA:
        cmp    eax, 0x100
        je     loc_3_08F5
        cmp    eax, 0x101
        jne    loc_3_0927
        call   sub_3_3378
        jmp    loc_3_08F5
loc_3_090F:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_3_0914:
        or     byte ptr [edx], 0x20
        mov    ax, word ptr [ebp + 0x14]
        mov    dword ptr [edx + 0x59], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [edx + 0x5d], ax
        jmp    loc_3_08F5
loc_3_0927:
        cmp    eax, 0x103
        jne    loc_3_090F
        xor    eax, eax
        test   byte ptr [edx], 0x20
        je     loc_3_0937
        inc    al
loc_3_0937:
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_3_08F5
        db  02Eh, 08Bh, 0C0h
loc_3_0940:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        test   byte ptr [edx], 0xc1
        jne    loc_3_09CA
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [edx + 0x55], al
        shr    eax, 0x10
        mov    byte ptr [edx + 0x75], al
        mov    esi, dword ptr [edx + 0x18]
        test   al, 4
        je     loc_3_096C
        inc    word ptr [edx + 0x3a]
        mov    dword ptr [edx + 0x34], esi
        jmp    loc_3_0977
loc_3_096C:
        test   al, 2
        je     loc_3_0977
        inc    word ptr [edx + 0x38]
        mov    dword ptr [edx + 0x30], esi
loc_3_0977:
        test   al, 0x10
        je     loc_3_0984
        inc    word ptr [edx + 0x46]
        mov    dword ptr [edx + 0x40], esi
        jmp    loc_3_098F
loc_3_0984:
        test   al, 8
        je     loc_3_098F
        inc    word ptr [edx + 0x44]
        mov    dword ptr [edx + 0x3c], esi
loc_3_098F:
        mov    eax, dword ptr [ebp]
        mov    ecx, eax
        mov    dword ptr [edx + 0x18], eax
        xchg   dword ptr [edx + 0x14], ecx
        call   sub_3_0CB4
        add    word ptr [edx + 0x26], si
        add    word ptr [edx + 0x24], di
        add    word ptr [edx + 0x2a], si
        add    word ptr [edx + 0x28], di
        mov    esi, 0x9cf                    ; -> loc_3_09CF
        mov    eax, 0x400000
        mov    ecx, 9
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edx + 4], esi
        or     byte ptr [edx], 1
loc_3_09CA:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_3_09CF:
        mov    dword ptr [edx + 4], 0
        movzx  eax, byte ptr [edx + 0x5f]
        and    al, byte ptr [edx + 0x75]
        je     loc_3_09E7
        lea    edi, [edx + 0x59]
        call   sub_3_0CE4
loc_3_09E7:
        and    byte ptr [edx], 0xfe
        ret    
VMD_Manipulate_Pointer_Message:
        ret    
loc_3_09EC:
        mov    dword ptr [edx + 4], 0
        push   edx
        mov    al, byte ptr [0x767]          ; -> obj1:0x0767
        mov    esi, dword ptr [0x758]        ; -> obj1:0x0758
        mov    edi, dword ptr [0x75c]        ; -> obj1:0x075C
        mov    edx, dword ptr [0x760]        ; -> obj1:0x0760
        VxDCall  VMD.Service_0008h               ; cd2008000c00
        pop    edx
        mov    byte ptr [0x766], al          ; -> obj1:0x0766
        mov    dword ptr [0x750], esi        ; -> obj1:0x0750
        mov    dword ptr [0x754], edi        ; -> obj1:0x0754
        mov    bl, byte ptr [0x766]          ; -> obj1:0x0766
        mov    byte ptr [edx + 0x54], bl
        test   bl, 0x80
        jne    loc_3_0A3D
        and    bl, 0x7f
        mov    bh, byte ptr [0x767]          ; -> obj1:0x0767
        mov    byte ptr [edx + 0x54], bh
loc_3_0A3D:
        call   sub_3_0B8C
        call   sub_3_0BB0
        mov    byte ptr [edx + 0x75], al
        mov    esi, dword ptr [0x750]        ; -> obj1:0x0750
        mov    edi, dword ptr [0x754]        ; -> obj1:0x0754
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        test   byte ptr [0x766], 0x80        ; -> obj1:0x0766
        je     loc_3_0A79
        mov    eax, dword ptr [0x758]        ; -> obj1:0x0758
        add    word ptr [edx + 0x26], ax
        mov    eax, dword ptr [0x75c]        ; -> obj1:0x075C
        add    word ptr [edx + 0x24], ax
        jmp    loc_3_0A81
loc_3_0A79:
        add    word ptr [edx + 0x26], si
        add    word ptr [edx + 0x24], di
loc_3_0A81:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_0A8E
        test   byte ptr [edx], 0x20
        je     loc_3_0AA6
loc_3_0A8E:
        mov    eax, esi
        or     eax, edi
        je     loc_3_0A9D
        or     byte ptr [edx + 0x75], 1
        call   sub_3_0C1C
loc_3_0A9D:
        call   sub_3_0D54
loc_3_0AA2:
        and    byte ptr [edx], 0xfe
        ret    
loc_3_0AA6:
        mov    eax, esi
        or     eax, edi
        je     loc_3_0ADD
        or     byte ptr [edx + 0x75], 1
        call   sub_3_0C1C
        add    word ptr [edx + 0x2a], si
        add    word ptr [edx + 0x28], di
        call   sub_3_0C70
        add    word ptr [edx + 0x16], si
        add    word ptr [edx + 0x14], di
        mov    eax, dword ptr [edx + 0x14]
        call   sub_3_0618
        mov    dword ptr [edx + 0x14], eax
        call   sub_3_0653
        mov    dword ptr [edx + 0x18], eax
loc_3_0ADD:
        mov    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        movzx  eax, byte ptr [edx + 0x5f]
        and    al, byte ptr [edx + 0x75]
        je     loc_3_0AF4
        lea    edi, [edx + 0x59]
        call   sub_3_0CE4
loc_3_0AF4:
        pushal 
        movzx  eax, word ptr [0x764]         ; -> obj1:0x0764
        mov    esi, eax
        shr    esi, 7
        shl    esi, 6
        and    al, 1
        shl    al, 5
        or     eax, esi
        call   sub_3_0698
        jne    loc_3_0B28
        mov    ah, byte ptr [edx + 0x75]
        and    ah, 0x1f
        and    ah, byte ptr [edi + 6]
        je     loc_3_0B28
        and    al, 0xe0
        or     al, ah
        xor    ah, ah
        call   sub_3_0CE4
loc_3_0B28:
        popal  
        test   byte ptr [edx], 2
        je     loc_3_0AA2
        cmp    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        jne    loc_3_0AA2
        mov    al, 2
        mov    ecx, dword ptr [edx + 0x18]
        VxDCall  VMD.Service_0005h               ; cd2005000c00
        jmp    loc_3_0AA2
loc_3_0B4E:
        mov    esi, dword ptr [0x768]        ; -> obj1:0x0768
        mov    edi, dword ptr [0x76c]        ; -> obj1:0x076C
        mov    al, byte ptr [0x770]          ; -> obj1:0x0770
        mov    byte ptr [edx + 0x75], al
        call   sub_3_0D54
        jmp    loc_3_0AA2
VMD_Set_Middle_Button:
        ret    
VMD_Enable_Disable_Mouse_Events:
        push   edx
        push   esi
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
        and    byte ptr [edx], 0xbf
        or     eax, eax
        jne    loc_3_0B86
        or     byte ptr [edx], 0x40
        call   sub_3_07F5
loc_3_0B86:
        pop    esi
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_0B8C:
        xor    bh, bh
        VxDCall  VMD.Service_0009h               ; cd2009000c00
        test   byte ptr [0xa6], bl           ; -> obj4:0x00A6
        je     loc_3_0B9E
        inc    bh
loc_3_0B9E:
        test   byte ptr [0xa7], bl           ; -> obj4:0x00A7
        je     loc_3_0BA9
        or     bh, 2
loc_3_0BA9:
        mov    bl, bh
        xchg   byte ptr [edx + 0x55], bl
        ret    
        db  090h
sub_3_0BB0:
        mov    esi, dword ptr [edx + 0x18]
        xor    eax, eax
        mov    ecx, ebx
        inc    ah
        and    bl, ah
        and    bh, ah
        cmp    bl, bh
        je     loc_3_0BD7
        jb     loc_3_0BCE
        inc    word ptr [edx + 0x3a]
        mov    dword ptr [edx + 0x34], esi
        or     al, 4
        jmp    loc_3_0BD7
loc_3_0BCE:
        inc    word ptr [edx + 0x38]
        mov    dword ptr [edx + 0x30], esi
        or     al, 2
loc_3_0BD7:
        inc    ah
        mov    ebx, ecx
        and    bl, ah
        and    bh, ah
        cmp    bl, bh
        je     loc_3_0BF9
        jb     loc_3_0BF0
        inc    word ptr [edx + 0x46]
        mov    dword ptr [edx + 0x40], esi
        or     al, 0x10
        jmp    loc_3_0BF9
loc_3_0BF0:
        inc    word ptr [edx + 0x44]
        mov    dword ptr [edx + 0x3c], esi
        or     al, 8
loc_3_0BF9:
        add    ah, ah
        and    cl, ah
        and    ch, ah
        cmp    cl, ch
        je     loc_3_0C19
        jb     loc_3_0C10
        inc    word ptr [edx + 0x52]
        mov    dword ptr [edx + 0x4c], esi
        or     al, 0x40
        jmp    loc_3_0C19
loc_3_0C10:
        inc    word ptr [edx + 0x50]
        mov    dword ptr [edx + 0x48], esi
        or     al, 0x20
loc_3_0C19:
        ret    
        db  08Bh, 0C0h
sub_3_0C1C:
        push   ebp
        push   edx
        movzx  eax, word ptr [edx + 0x1e]
        push   eax
        movzx  eax, word ptr [edx + 0x1c]
        push   eax
        mov    ebp, dword ptr [0x10]         ; -> obj4:0x0010
        mov    eax, edi
        pop    ebx
        imul   eax, ebx
        movsx  ebx, bp
        add    eax, ebx
        mov    ebx, 0x100
        cdq    
        idiv   ebx
        mov    bp, dx
        mov    edi, eax
        ror    ebp, 0x10
        mov    eax, esi
        pop    ebx
        imul   eax, ebx
        movsx  ebx, bp
        add    eax, ebx
        mov    ebx, 0x100
        cdq    
        idiv   ebx
        mov    bp, dx
        mov    esi, eax
        ror    ebp, 0x10
        mov    dword ptr [0x10], ebp         ; -> obj4:0x0010
        pop    edx
        pop    ebp
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_0C70:
        push   ebp
        push   edx
        mov    ecx, dword ptr [edx + 0x20]
        mov    ebp, dword ptr [0x14]         ; -> obj4:0x0014
        or     edi, edi
        je     loc_3_0C90
        movsx  edx, bp
        lea    eax, [edx + edi*8]
        movzx  ebx, cx
        cdq    
        idiv   ebx
        mov    bp, dx
        mov    edi, eax
loc_3_0C90:
        shr    ecx, 0x10
        ror    ebp, 0x10
        or     esi, esi
        je     loc_3_0CA8
        movsx  edx, bp
        lea    eax, [edx + esi*8]
        cdq    
        idiv   ecx
        mov    bp, dx
        mov    esi, eax
loc_3_0CA8:
        ror    ebp, 0x10
        mov    dword ptr [0x14], ebp         ; -> obj4:0x0014
        pop    edx
        pop    ebp
        ret    
sub_3_0CB4:
        push   ebx
        push   edx
        mov    ebx, dword ptr [edx + 0x20]
        movzx  edx, bx
        mov    edi, eax
        sub    di, cx
        movsx  edi, di
        imul   edi, edx
        shr    edi, 3
        shr    ebx, 0x10
        shr    eax, 0x10
        shr    ecx, 0x10
        mov    esi, eax
        sub    si, cx
        movsx  esi, si
        imul   esi, ebx
        shr    esi, 3
        pop    edx
        pop    ebx
        ret    
sub_3_0CE4:
        push   edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        test   byte ptr [edx], 4
        jne    loc_3_0D01
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        jmp    loc_3_0D07
loc_3_0D01:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_3_0D07:
        mov    dword ptr [ebp + 0x1c], eax
        mov    al, byte ptr [edx + 0x55]
        mov    dword ptr [ebp + 0x10], eax
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp + 0x14], eax
        shr    eax, 0x10
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [edx + 0x28]
        mov    dword ptr [ebp], eax
        shr    eax, 0x10
        mov    dword ptr [ebp + 4], eax
        mov    cx, word ptr [edi + 4]
        mov    edx, dword ptr [edi]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_0D54:
        movzx  ecx, word ptr [edx + 0x5d]
        jecxz  loc_3_0DB1
        push   edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    dword ptr [ebp + 0x10], esi
        mov    dword ptr [ebp + 0x18], edi
        xor    eax, eax
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [ebp], eax
        mov    al, byte ptr [edx + 0x75]
        mov    dword ptr [ebp + 0x1c], eax
        mov    al, byte ptr [0x696]          ; -> loc_1_0696
        mov    dword ptr [ebp + 0x14], eax
        mov    edx, dword ptr [edx + 0x59]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edx
loc_3_0DB1:
        ret    
sub_3_0DB2:
        xor    eax, eax
        mov    dword ptr [0x10], eax         ; -> obj4:0x0010
        mov    dword ptr [0x14], eax         ; -> obj4:0x0014
        clc    
        ret    
VMD_Get_Mouse_Data:
        mov    ah, byte ptr [0x694]          ; -> obj1:0x0694
        mov    al, byte ptr [0x695]          ; -> obj1:0x0695
        mov    cx, word ptr [4]              ; -> obj4:0x0004
        mov    dl, byte ptr [0x697]          ; -> obj1:0x0697
        mov    dh, byte ptr [0x696]          ; -> loc_1_0696
        ret    
        db  000h
sub_3_0DE0:
        cmp    byte ptr [0x78b], 0           ; -> loc_1_078B
        je     loc_3_0DFB
        push   ebx
        add    ebx, dword ptr [0x790]        ; -> obj1:0x0790
        mov    dword ptr [ebx], 0
        mov    byte ptr [ebx + 5], 0
        pop    ebx
loc_3_0DFB:
        clc    
        ret    
sub_3_0DFD:
        cmp    byte ptr [0x78b], 0           ; -> loc_1_078B
        je     loc_3_0EC5
        call   sub_3_32F7
        mov    byte ptr [0x797], 0xff        ; -> obj1:0x0797
        mov    byte ptr [0x796], 0xff        ; -> obj1:0x0796
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0xc201
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        cmp    byte ptr [0x798], 0xff        ; -> loc_1_0798
        jne    loc_3_0EA4
        mov    eax, dword ptr [0x790]        ; -> obj1:0x0790
        add    eax, ebx
        cmp    byte ptr [eax + 4], 0xff
        je     loc_3_0EA4
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_3_0EA4
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x1c], 0xc207
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        mov    word ptr [ebp + 0x1c], 0xc200
        mov    word ptr [ebp + 0x10], 0x100
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
loc_3_0EA4:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    byte ptr [0x796], 0           ; -> obj1:0x0796
        call   sub_3_32B4
loc_3_0EC5:
        ret    
loc_3_0EC6:
        mov    eax, dword ptr [0x780]        ; -> obj1:0x0780
        mov    esi, 0x1c                     ; -> obj4:0x001C
        mov    dword ptr [esi + 0x10], eax
        call   sub_3_3217
        mov    ecx, edx
        call   sub_3_2E22
        ret    
loc_3_0EE0:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    edi, esi
        add    di, 0xa
        mov    al, byte ptr [edi]
        mov    byte ptr [0x794], al          ; -> obj1:0x0794
        cmp    ebx, dword ptr [0x690]        ; -> obj1:0x0690
        jne    loc_3_0F7D
        add    ebx, dword ptr [0x790]        ; -> obj1:0x0790
        cmp    byte ptr [ebx + 5], 0xff
        jne    loc_3_0F84
        cmp    byte ptr [ebx + 4], 0xff
        je     loc_3_0F21
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_3_0F21:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        add    si, 0xa
        mov    ecx, 4
        push   dword ptr [ebp + 0x30]
loc_3_0F42:
        mov    ax, word ptr [esi]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        sub    si, 2
        loop   loc_3_0F42
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    dword ptr [ebp + 0x30]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_3_0F7D:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
loc_3_0F84:
        movzx  eax, word ptr [esi + 0xa]
        shl    eax, 0x10
        mov    ah, byte ptr [esi + 8]
        mov    al, byte ptr [esi + 6]
        call   sub_1_03EC
        jmp    loc_3_0F7D
sub_3_0F98:
        movzx  eax, byte ptr [0xa4]          ; -> obj4:0x00A4
        mov    byte ptr [edx + 0x56], al
        call   sub_3_06D1
        mov    esi, eax
        shl    esi, 0x10
        movzx  eax, byte ptr [0xa5]          ; -> obj4:0x00A5
        mov    byte ptr [edx + 0x57], al
        call   sub_3_06D1
        mov    si, ax
        mov    dword ptr [edx + 0x1c], esi
        clc    
        ret    
sub_3_0FC3:
        mov    esi, 0xa9                     ; -> obj4:0x00A9
        call   sub_3_100E
        jb     loc_3_0FE2
        cmp    dl, 1
        ja     loc_3_0FF0
        je     loc_3_0FE3
        mov    edx, 0x2d1b                   ; -> sub_3_2D1B
        mov    byte ptr [0xa8], 0            ; -> obj4:0x00A8
loc_3_0FE2:
        ret    
loc_3_0FE3:
        mov    byte ptr [0xa8], 3            ; -> obj4:0x00A8
        mov    edx, 0x270c                   ; -> sub_3_270C
        ret    
loc_3_0FF0:
        mov    byte ptr [0xa8], 9            ; -> obj4:0x00A8
        cmp    dl, 2
        mov    edx, 0x2b38                   ; -> sub_3_2B38
        je     loc_3_0FE2
        mov    byte ptr [0xa8], 0xf          ; -> obj4:0x00A8
        mov    edx, 0x2e22                   ; -> sub_3_2E22
        ret    
sub_3_100E:
        mov    ebx, edi
        xor    edx, edx
        xor    ecx, ecx
loc_3_1014:
        movzx  ecx, byte ptr [esi]
        inc    esi
        jecxz  loc_3_1047
loc_3_101A:
        mov    al, byte ptr [edi]
        cmp    al, 0x5a
        ja     loc_3_1026
        cmp    al, 0x41
        jb     loc_3_1026
        or     al, 0x20
loc_3_1026:
        mov    ah, byte ptr [esi]
        cmp    ah, 0x5a
        ja     loc_3_1035
        cmp    ah, 0x41
        jb     loc_3_1035
        or     ah, 0x20
loc_3_1035:
        cmp    ah, al
        jne    loc_3_103F
        inc    esi
        inc    edi
        loop   loc_3_101A
        clc    
        ret    
loc_3_103F:
        mov    edi, ebx
        add    esi, ecx
        inc    dl
        jmp    loc_3_1014
loc_3_1047:
        stc    
        ret    
        db  000h, 000h, 000h
loc_3_104C:
        mov    ecx, dword ptr [0x7cc]        ; -> loc_1_07CC
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x10
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
        call   sub_3_0DFD
        ret    
loc_3_1069:
        test   word ptr [edx + 0x81], 1
        jne    loc_3_10BE
        mov    edi, 0xca                     ; -> loc_4_00CA
        xor    eax, eax
        mov    word ptr [edi], 0x12
        mov    dword ptr [edi + 2], 0xd6     ; -> loc_4_00D6
        mov    byte ptr [edi + 6], al
        cmp    word ptr [ebp + 0x10], 0
        je     loc_3_1099
        or     byte ptr [edx + 0x7d], 1
        or     byte ptr [edi + 6], 1
loc_3_1099:
        mov    byte ptr [edi + 7], 1
        mov    dword ptr [edi + 0xc], eax
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 8], ax
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [edi + 0xa], ax
        mov    dword ptr [edx + 0xa5], eax
        jmp    sub_3_13D8
loc_3_10BE:
        ret    
loc_3_10BF:
        test   byte ptr [edx + 0x81], 1
        je     loc_3_1128
        mov    edi, 0xca                     ; -> loc_4_00CA
        mov    eax, 0x10
        mov    word ptr [edi], 0x50
        mov    dword ptr [edi + 2], 0x10
        mov    word ptr [edi + 6], ax
        mov    word ptr [edi + 8], ax
        mov    cx, word ptr [ebp + 0x10]
        mov    word ptr [edi + 0xa], cx
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x18]
        mov    word ptr [edi + 0xc], cx
        mov    dword ptr [edx + 0x2c], ecx
        mov    word ptr [edi + 0xe], 1
        mov    edi, 0xda                     ; -> loc_4_00DA
        movzx  esi, word ptr [ebp + 0x38]
        movzx  ecx, word ptr [ebp + 0x14]
        shl    esi, 4
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0xca                     ; -> loc_4_00CA
        jmp    sub_3_140A
loc_3_1128:
        ret    
VMD_Set_Cursor_Proc:
        pushal 
        mov    edx, dword ptr [0x7c8]        ; -> loc_1_07C8
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [0x7c8], eax        ; -> loc_1_07C8
        jecxz  loc_3_1183
        push   ecx
        add    ecx, dword ptr [0x11e]        ; -> loc_4_011E
        cmp    dword ptr [0x11a], 0          ; -> loc_4_011A
        jne    loc_3_1157
        push   0
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        jmp    loc_3_1169
loc_3_1157:
        push   0
        push   ecx
        push   dword ptr [0x11a]             ; -> loc_4_011A
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
loc_3_1169:
        pop    ecx
        or     eax, eax
        je     loc_3_1183
        mov    dword ptr [0x11a], eax        ; -> loc_4_011A
        mov    edi, eax
        add    edi, dword ptr [0x11e]        ; -> loc_4_011E
        add    dword ptr [0x11e], ecx        ; -> loc_4_011E
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_1183:
        popal  
        ret    
VMD_Call_Cursor_Proc:
        movzx  eax, al
        push   edx
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
        cmp    dword ptr [edx + 0x97], 0
        je     loc_3_11A0
        call   dword ptr [edx + 0x97]
loc_3_11A0:
        pop    edx
        ret    
loc_3_11A2:
        mov    edx, ebx
        add    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    ah, 4
        je     loc_3_11C8
        cmp    ah, 0x11
        jne    loc_3_11BC
        cmp    al, 0x30
        jne    loc_3_11F0
loc_3_11BC:
        cmp    ah, 1
        jb     loc_3_11D6
        cmp    ah, 0x1c
        ja     loc_3_11CA
loc_3_11C6:
        stc    
        ret    
loc_3_11C8:
        jmp    loc_3_11C6
loc_3_11CA:
        cmp    ax, 0x4f02
        jne    loc_3_11D6
        or     dword ptr [edx + 0x79], 4
        jmp    loc_3_11F0
loc_3_11D6:
        test   dword ptr [edx + 0x79], 8
        je     loc_3_11EC
        call   sub_3_12B0
        jae    loc_3_11EC
        or     dword ptr [edx + 0x79], 0x10
        jmp    loc_3_11F0
loc_3_11EC:
        or     ah, ah
        jne    loc_3_11C6
loc_3_11F0:
        test   byte ptr [edx + 0x79], 1
        jne    loc_3_11C6
        call   sub_3_1219
        mov    eax, 0x1388
        mov    esi, 0x1244                   ; -> loc_3_1244
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        stc    
        ret    
loc_3_120D:
        inc    word ptr [ebp + 0x24]
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
sub_3_1219:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_1232
        bts    dword ptr [edx + 0x79], 0
        jb     loc_3_1232
        test   byte ptr [edx], 2
        je     loc_3_1232
        call   sub_3_0194
loc_3_1232:
        ret    
loc_3_1233:
        inc    word ptr [ebp + 0x24]
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        test   byte ptr [edx + ebx + 0x79], 4
        jne    loc_3_12AF
loc_3_1244:
        mov    edx, dword ptr [0x68c]        ; -> obj1:0x068C
        add    edx, ebx
        test   byte ptr [edx + 0x79], 1
        je     loc_3_12AF
        test   byte ptr [edx + 0x79], 4
        jne    loc_3_126E
        test   byte ptr [edx + 0x79], 0x10
        jne    loc_3_1285
        mov    eax, 0xf00
        call   sub_3_14B1
        and    al, 0x7f
        cbw    
        jmp    loc_3_128C
loc_3_126E:
        mov    eax, 0x4f03
        call   sub_3_14B1
        cmp    ax, 0x4f
        jne    loc_3_12AB
        ror    eax, 0x10
        and    al, 0x7f
        jmp    loc_3_128C
loc_3_1285:
        stc    
        call   sub_4_00C0
loc_3_128C:
        call   sub_3_12B2
        jae    loc_3_12AB
        test   byte ptr [edx + 0x79], 4
        jne    loc_3_12AB
        xor    eax, eax
        mov    al, 3
        cmp    word ptr [edx + 0x9f], ax
        je     loc_3_12AB
        call   sub_3_12B2
loc_3_12AB:
        and    byte ptr [edx + 0x79], 0xfa
loc_3_12AF:
        ret    
sub_3_12B0:
        clc    
        ret    
sub_3_12B2:
        mov    ecx, eax
        mov    edi, dword ptr [0x7c8]        ; -> loc_1_07C8
        mov    dword ptr [0xc4], 0           ; -> loc_4_00C4
        mov    word ptr [0xc8], ax           ; -> loc_4_00C8
loc_3_12CA:
        test   byte ptr [edi + 0xd], 1
        je     loc_3_12DE
        xchg   ecx, eax
        mov    eax, 8
        call   dword ptr [edi]
        xchg   ecx, eax
        cmp    ax, cx
        jne    loc_3_12ED
loc_3_12DE:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    loc_3_12CA
        mov    edi, dword ptr [0x7c8]        ; -> loc_1_07C8
        jmp    loc_3_1314
loc_3_12ED:
        cmp    dword ptr [0xc4], 0           ; -> loc_4_00C4
        je     loc_3_1304
        push   eax
        movzx  eax, byte ptr [0xd0]          ; -> loc_4_00D0
        cmp    al, byte ptr [edi + 0xc]
        pop    eax
        jae    loc_3_12DE
loc_3_1304:
        mov    word ptr [0xc8], ax           ; -> loc_4_00C8
        mov    eax, ecx
        mov    dword ptr [0xc4], edi         ; -> loc_4_00C4
        jmp    loc_3_12DE
loc_3_1314:
        cmp    dword ptr [0xc4], 0           ; -> loc_4_00C4
        je     loc_3_1323
        mov    edi, dword ptr [0xc4]         ; -> loc_4_00C4
loc_3_1323:
        movzx  ecx, word ptr [0xc8]          ; -> loc_4_00C8
loc_3_132A:
        mov    eax, 4
        call   dword ptr [edi]
        or     ax, ax
        jne    loc_3_133F
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    loc_3_132A
        stc    
        ret    
loc_3_133F:
        mov    dword ptr [edx + 0x9b], edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [edx + 0x97], edi
        call   sub_3_1378
        call   sub_3_13A7
        call   sub_3_14FA
        mov    byte ptr [edx + 0x7e], 0
        mov    ax, word ptr [0xc8]           ; -> loc_4_00C8
        mov    word ptr [edx + 0x9f], ax
        mov    eax, dword ptr [edx + 0x83]
        mov    dword ptr [edx + 0x10], eax
        clc    
        ret    
sub_3_1378:
        lea    edi, [edx + 0x7f]
        mov    word ptr [edi], 0x16
        mov    eax, 7
        call   dword ptr [edx + 0x97]
        mov    ax, word ptr [edx + 0x95]
        or     al, al
        je     loc_3_139C
        mov    byte ptr [edx + 0x87], al
loc_3_139C:
        or     ah, ah
        je     loc_3_13A6
        mov    byte ptr [edx + 0x88], ah
loc_3_13A6:
        ret    
sub_3_13A7:
        push   edx
        test   word ptr [edx + 0x81], 1
        jne    loc_3_13C3
        mov    edi, 0x44                     ; -> obj4:0x0044
        and    byte ptr [edx + 0x7d], 0xfe
        call   sub_3_13D8
        jmp    loc_3_13CD
loc_3_13C3:
        mov    edi, 0x54                     ; -> obj4:0x0054
        call   sub_3_140A
loc_3_13CD:
        pop    edx
        and    byte ptr [edx], 0xfd
        mov    word ptr [edx + 0x76], 0xffff
        ret    
sub_3_13D8:
        test   byte ptr [edx + 0x81], 1
        stc    
        jne    loc_3_1409
        call   sub_3_14F5
        jb     loc_3_1409
        and    byte ptr [edx + 0x7d], 0xe5
        cmp    byte ptr [edi + 7], 1
        jne    loc_3_13F8
        call   sub_3_14F7
loc_3_13F8:
        call   sub_3_14F9
        mov    eax, 0
        call   dword ptr [edx + 0x97]
        clc    
loc_3_1409:
        ret    
sub_3_140A:
        test   byte ptr [edx + 0x81], 1
        stc    
        je     loc_3_144D
        call   sub_3_144E
        jb     loc_3_144D
        and    byte ptr [edx + 0x7d], 0xe4
        mov    ax, 0x10
        cmp    cx, ax
        jne    loc_3_143C
        ror    ecx, 0x10
        cmp    cx, ax
        jne    loc_3_143C
        cmp    word ptr [edi + 0xe], 1
        jne    loc_3_143C
        call   sub_3_14F8
loc_3_143C:
        call   sub_3_14F9
        mov    eax, 1
        call   dword ptr [edx + 0x97]
        clc    
loc_3_144D:
        ret    
sub_3_144E:
        cmp    dword ptr [edi + 2], 0x10
        jne    loc_3_14AF
        mov    ax, word ptr [edi + 0xe]
        or     ah, ah
        jne    loc_3_14AF
        cmp    al, 1
        je     loc_3_1471
        test   byte ptr [edx + 0x81], 2
        je     loc_3_14AF
        cmp    al, byte ptr [edx + 0x8d]
        jne    loc_3_14AF
loc_3_1471:
        mov    cx, word ptr [edi + 6]
        test   cx, 0xff07
        jne    loc_3_14AF
        cmp    cl, byte ptr [edx + 0x91]
        ja     loc_3_14AF
        shl    ecx, 0x10
        mov    cx, word ptr [edi + 8]
        or     ch, ch
        jne    loc_3_14AF
        cmp    cl, byte ptr [edx + 0x92]
        ja     loc_3_14AF
        cbw    
        inc    ax
        mul    cl
        shr    ax, 3
        rol    ecx, 0x10
        mul    cl
        add    ax, 0x10
        cmp    word ptr [edi], ax
        je     loc_3_14B0
loc_3_14AF:
        stc    
loc_3_14B0:
        ret    
sub_3_14B1:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], ax
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x10]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_3_14F5:
        clc    
        ret    
sub_3_14F7:
        ret    
sub_3_14F8:
        ret    
sub_3_14F9:
        ret    
sub_3_14FA:
        ret    
        db  000h
loc_3_14FC:
        cmp    eax, 0xd
        jae    loc_3_1520
        pushal 
        mov    dword ptr [0x7dc], ebx        ; -> loc_1_07DC
        add    ebx, dword ptr [0x7d8]        ; -> loc_1_07D8
        shl    eax, 2
        add    eax, 0x80c                    ; -> obj1:0x080C
        call   dword ptr [eax]
        mov    dword ptr [esp + 0x1c], eax
        popal  
        stc    
loc_3_1520:
        cmc    
        ret    
loc_3_1522:
        ret    
loc_3_1523:
        and    byte ptr [ebx + 0x9f], 0xfd
        or     edi, edi
        je     loc_3_1551
        or     byte ptr [ebx + 0x9f], 2
        mov    dword ptr [ebx + 0xa], ecx
        mov    dword ptr [ebx + 0xe], esi
        test   byte ptr [ebx + 0x9d], 2
        je     loc_3_1551
        call   sub_3_1A3A
        mov    ecx, dword ptr [ebx + 2]
        call   sub_3_2327
loc_3_1551:
        ret    
loc_3_1552:
        xor    eax, eax
        test   ch, 0xff
        jne    loc_3_157B
        mov    byte ptr [ebx], cl
        mov    eax, ecx
        mov    edi, 0x845                    ; -> obj1:0x0845
        mov    ecx, 0xf
        repne scasb al, byte ptr es:[edi]
        jne    loc_3_157C
        call   sub_3_15A6
        mov    eax, dword ptr [ebx + 0x1a]
        mov    dword ptr [ebx + 0x16], eax
        mov    eax, 1
loc_3_157B:
        ret    
loc_3_157C:
        xor    eax, eax
        mov    word ptr [ebx + 0x9d], ax
        mov    dword ptr [ebx + 0xd0], eax
        mov    dword ptr [ebx + 0xcc], eax
        mov    esi, 0x124                    ; -> loc_4_0124
        lea    edi, [ebx + 0x87]
        cld    
        push   0x16
        pop    ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    eax, eax
        inc    eax
        ret    
sub_3_15A6:
        xor    eax, eax
        mov    word ptr [ebx + 0x87], 0x16
        mov    word ptr [ebx + 0x89], ax
        mov    word ptr [ebx + 0x9d], ax
        mov    byte ptr [ebx + 0x9b], al
        mov    byte ptr [ebx + 0x9c], 1
        mov    word ptr [ebx + 0x91], 1
        mov    word ptr [ebx + 0x93], 1
        mov    dword ptr [ebx + 0xb4], 0xffffffff
        mov    dword ptr [ebx + 0xd0], eax
        mov    dword ptr [ebx + 0x7f], eax
        mov    byte ptr [ebx + 1], al
        mov    word ptr [ebx + 0x96], 0x1000
        mov    edx, ebx
        sub    edx, dword ptr [0x7d8]        ; -> loc_1_07D8
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [ebx + 0x1a], edx
        add    dword ptr [ebx + 0x1a], 0xb8000
        mov    al, byte ptr [ebx]
        cmp    al, 7
        ja     loc_3_16A9
        je     loc_3_164A
        cmp    al, 3
        ja     loc_3_16A9
        mov    cl, al
        cmp    cl, 2
        jae    loc_3_1651
        mov    word ptr [ebx + 0x93], 0x10
        mov    word ptr [ebx + 0x8d], 0x28
        mov    word ptr [ebx + 0x96], 0x800
        jmp    loc_3_1663
loc_3_164A:
        add    dword ptr [ebx + 0x1a], 0xffff8000
loc_3_1651:
        mov    word ptr [ebx + 0x93], 8
        mov    word ptr [ebx + 0x8d], 0x50
loc_3_1663:
        mov    byte ptr [ebx + 0x98], 1
        movzx  eax, byte ptr [0x484]
        inc    ax
        mov    word ptr [ebx + 0x8b], ax
        mov    word ptr [ebx + 0x91], 8
        mov    byte ptr [ebx + 0x95], 0
        mov    word ptr [ebx + 0x8f], 0x101
        xor    ecx, ecx
        xchg   dword ptr [ebx + 0xe0], ecx
        jecxz  loc_3_16A8
        push   0
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_16A8:
        ret    
loc_3_16A9:
        mov    dword ptr [ebx + 0xcc], 0x1f04 ; -> loc_3_1F04
        mov    dword ptr [ebx + 0xd0], 0x1a1c ; -> loc_3_1A1C
        mov    dword ptr [ebx + 0x7f], 0x2000
        cmp    byte ptr [ebx], 6
        jbe    loc_3_16D7
        mov    dword ptr [ebx + 0x7f], 0
        add    dword ptr [ebx + 0x1a], 0xfffe8000
loc_3_16D7:
        mov    eax, 9
        sub    al, cl
        mov    cl, byte ptr [eax + 0x854]    ; -> loc_1_0854
        mov    byte ptr [ebx + 0x95], cl
        mov    ch, byte ptr [eax + 0x890]    ; -> obj1:0x0890
        xor    cl, cl
        mov    word ptr [ebx + 0x96], cx
        movzx  cx, byte ptr [eax + 0x85e]    ; -> loc_1_085E
        mov    word ptr [ebx + 0x93], cx
        cmp    eax, 3
        jb     loc_3_171B
        movzx  ecx, word ptr [eax*2 + 0x894] ; -> obj1:0x0894
        mov    dword ptr [ebx + 0xdc], ecx
loc_3_171B:
        mov    ecx, dword ptr [eax*4 + 0x868] ; -> loc_1_0868
        mov    word ptr [ebx + 0x8d], cx
        shr    ecx, 0x10
        mov    word ptr [ebx + 0x8b], cx
        or     byte ptr [ebx + 0x89], 1
        cmp    dword ptr [ebx + 0x7f], 0
        je     loc_3_1745
        jmp    loc_3_17DC
loc_3_1745:
        mov    dword ptr [ebx + 0xb4], 0
        mov    dword ptr [ebx + 0xcc], 0x1f1c ; -> loc_3_1F1C
        mov    dword ptr [ebx + 0xd0], 0x1a2c ; -> loc_3_1A2C
        cmp    byte ptr [ebx + 0x95], 8
        jb     loc_3_1789
        or     byte ptr [ebx + 0x9d], 0x40
        mov    dword ptr [ebx + 0xb4], 0xffffffff
        mov    dword ptr [ebx + 0xcc], 0x1fd8 ; -> loc_3_1FD8
        jmp    loc_3_17DC
loc_3_1789:
        cmp    byte ptr [ebx], 0x10
        jne    loc_3_179E
        cmp    byte ptr [0x840], 0x10        ; -> obj1:0x0840
        ja     loc_3_179E
        mov    byte ptr [ebx + 0x95], 2
loc_3_179E:
        mov    al, byte ptr [ebx]
        cmp    al, 0xf
        jb     loc_3_17CE
        cmp    al, 0x10
        ja     loc_3_17CE
        push   eax
        mov    edx, 0x3c4
        mov    ah, 4
        call   sub_3_25A4
        test   ah, 2
        pop    eax
        jne    loc_3_17CE
        or     byte ptr [ebx + 0x9d], 0x20
        mov    dword ptr [ebx + 0xb4], 2
        jmp    loc_3_17DC
loc_3_17CE:
        cmp    al, 0x11
        jne    loc_3_17DC
        mov    dword ptr [ebx + 0xb4], 1
loc_3_17DC:
        mov    al, 0x10
        mov    byte ptr [ebx + 0x99], al
        mov    byte ptr [ebx + 0x9a], al
        movzx  eax, word ptr [ebx + 0x8d]
        cmp    byte ptr [ebx + 0x95], 8
        je     loc_3_17FD
        shr    eax, 3
loc_3_17FD:
        mov    dword ptr [ebx + 0xd8], eax
        cmp    byte ptr [ebx], 5
        ja     loc_3_180E
        add    dword ptr [ebx + 0xd8], eax
loc_3_180E:
        movzx  ecx, word ptr [ebx + 0x8b]
        imul   ecx
        cmp    byte ptr [ebx], 6
        jne    loc_3_181E
        shr    eax, 1
loc_3_181E:
        cmp    byte ptr [ebx], 6
        ja     loc_3_1828
        add    eax, 0x2000
loc_3_1828:
        mov    dword ptr [ebx + 0xd4], eax
        cmp    dword ptr [ebx + 0xe0], 0
        jne    loc_3_1883
        push   0
        push   0x188
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_3_1850
        VMMCall  VMM.Crash_Cur_VM                ; cd202f800100
loc_3_1850:
        mov    dword ptr [ebx + 0xe0], eax
        lea    eax, [eax + 0x30]
        mov    dword ptr [ebx + 0xe4], eax
        lea    eax, [eax + 0x30]
        mov    dword ptr [ebx + 0xe8], eax
        lea    eax, [eax + 0x64]
        mov    dword ptr [ebx + 0xec], eax
        lea    eax, [eax + 0x64]
        mov    dword ptr [ebx + 0xf0], eax
        lea    eax, [eax + 0x30]
        mov    dword ptr [ebx + 0xf4], eax
loc_3_1883:
        mov    eax, dword ptr [ebx + 0xe0]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xe8]
        mov    dword ptr [ebx + 0xbc], eax
        cmp    dword ptr [ebx + 0xcc], 0x1f04 ; -> loc_3_1F04
        je     loc_3_1902
        mov    eax, dword ptr [ebx + 0xd4]
        add    eax, dword ptr [ebx + 0x1a]
        add    eax, 0x16
        mov    dword ptr [ebx + 0xb8], eax
        add    eax, 0x30
        mov    dword ptr [ebx + 0xbc], eax
        cmp    byte ptr [ebx + 0x95], 8
        jb     loc_3_18D5
        add    dword ptr [ebx + 0xbc], 0x150
loc_3_18D5:
        mov    dword ptr [ebx + 0xab], 0
        mov    byte ptr [ebx + 0xaf], 0
        mov    byte ptr [ebx + 0xb0], 1
        mov    byte ptr [ebx + 0xb1], 5
        mov    byte ptr [ebx + 0xb2], 0xf
        mov    byte ptr [ebx + 0xb3], 0xff
loc_3_1902:
        ret    
loc_3_1903:
        push   edi
        lea    esi, [ebx + 0x87]
        mov    ecx, 0x16
        mov    eax, ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        movzx  eax, word ptr [ebx + 0x8d]
        imul   word ptr [ebx + 0x93]
        mov    ecx, eax
        movzx  eax, word ptr [ebx + 0x8b]
        imul   word ptr [ebx + 0x91]
        dec    ecx
        dec    eax
        mov    word ptr [edi + 6], cx
        mov    word ptr [edi + 4], ax
        ret    
loc_3_193D:
        xor    eax, eax
        test   byte ptr [ebx + 0x9d], 2
        je     loc_3_1951
        push   edi
        call   sub_3_1A3A
        pop    edi
        mov    eax, ebx
loc_3_1951:
        push   eax
        lea    esi, [ebx + 0x1e]
        movzx  ecx, word ptr [edi]
        xchg   edi, esi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        call   sub_3_24D2
        pop    eax
        or     eax, eax
        je     loc_3_1989
        movzx  eax, word ptr [ebx + 4]
        mul    word ptr [ebx + 0x93]
        mov    ecx, eax
        shl    ecx, 0x10
        movzx  eax, word ptr [ebx + 2]
        mul    word ptr [ebx + 0x91]
        mov    cx, ax
        call   sub_3_2327
loc_3_1989:
        ret    
loc_3_198A:
        lea    esi, [ebx + 0x1e]
        xchg   edi, esi
        push   esi
        movzx  ecx, word ptr [esi]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        lea    esi, [esi + 0x10]
        call   sub_3_19C4
        mov    edx, 0xffffffff
        mov    edi, dword ptr [ebx + 0xf0]
        call   sub_3_19FC
        xor    edx, edx
        mov    byte ptr [ebx + 0x6e], dl
        lea    esi, [ebx + 0x4e]
        mov    edi, dword ptr [ebx + 0xf4]
        call   sub_3_19FC
        ret    
        db  08Bh, 0C0h
sub_3_19C4:
        and    byte ptr [ebx + 0x9d], 0xfb
        test   byte ptr [ebx + 0x9d], 0x40
        jne    loc_3_19FB
        push   esi
        push   edi
        mov    eax, 0x20
        mov    ecx, eax
        mov    edi, eax
        or     byte ptr [ebx + 0x9d], 4
        mov    eax, eax
loc_3_19E8:
        mov    al, byte ptr [esi]
        and    al, byte ptr [edi + esi]
        jne    loc_3_19F9
        inc    esi
        loop   loc_3_19E8
        and    byte ptr [ebx + 0x9d], 0xfb
loc_3_19F9:
        pop    edi
        pop    esi
loc_3_19FB:
        ret    
sub_3_19FC:
        cmp    dword ptr [ebx + 0xd0], 0
        je     loc_3_1A39
        mov    ecx, 0x10
        test   byte ptr [ebx + 0x9d], 4
        jne    loc_3_1A1C
        jmp    dword ptr [ebx + 0xd0]
        db  02Eh, 08Bh, 0C0h
loc_3_1A1C:
        lodsw  ax, word ptr [esi]
        xchg   ah, al
        stosw  word ptr es:[edi], ax
        mov    al, dl
        stosb  byte ptr es:[edi], al
        loop   loc_3_1A1C
        jmp    loc_3_1A39
        db  02Eh, 08Bh, 0C0h
loc_3_1A2C:
        lodsw  ax, word ptr [esi]
        xchg   ah, al
        xor    eax, edx
        stosw  word ptr es:[edi], ax
        xor    al, al
        stosb  byte ptr es:[edi], al
        loop   loc_3_1A2C
loc_3_1A39:
        ret    
sub_3_1A3A:
        btr    word ptr [ebx + 0x9d], 1
        jae    loc_3_1A78
        test   byte ptr [ebx + 0x9d], 8
        jne    loc_3_1A78
        test   byte ptr [ebx + 0x89], 1
        je     loc_3_1A5D
        call   sub_3_1AEF
        ret    
loc_3_1A5D:
        mov    esi, dword ptr [ebx + 0x16]
        call   sub_3_26F8
        add    esi, dword ptr [ebx + 6]
        mov    ax, word ptr [ebx + 0x14]
        cmp    ax, word ptr [esi]
        jne    loc_3_1A78
        mov    ax, word ptr [ebx + 0x12]
        mov    word ptr [esi], ax
loc_3_1A78:
        ret    
sub_3_1A79:
        mov    edi, esi
        movzx  eax, word ptr [0x843]         ; -> obj1:0x0843
        movsx  ecx, word ptr [ebx + 0x28]
        cmp    dword ptr [ebx + 0xcc], 0x1f04 ; -> loc_3_1F04
        je     loc_3_1A9F
        cmp    word ptr [ebx + 0x8d], 0x140
        jne    loc_3_1A9F
        shr    eax, 1
loc_3_1A9F:
        cmp    dword ptr [ebx + 0xcc], 0x1f04 ; -> loc_3_1F04
        jne    loc_3_1AAD
        shr    ecx, 1
loc_3_1AAD:
        sub    eax, ecx
        mov    dword ptr [ebx + 0x6f], eax
        movzx  eax, word ptr [0x841]         ; -> obj1:0x0841
        movsx  ecx, word ptr [ebx + 0x2a]
        sub    eax, ecx
        mov    dword ptr [ebx + 0x73], eax
        call   sub_3_218D
        call   sub_3_1B12
        call   sub_3_1DC8
        test   byte ptr [ebx + 0x9d], 0x40
        jne    loc_3_1ADF
        call   sub_3_2299
loc_3_1ADF:
        call   sub_3_2003
        call   sub_3_1D50
        call   sub_3_2218
        ret    
sub_3_1AEF:
        test   byte ptr [ebx + 0x9d], 0x10
        je     loc_3_1B11
        mov    edi, dword ptr [ebx + 0x16]
        call   sub_3_218D
        call   sub_3_1C5C
        call   sub_3_2218
        and    byte ptr [ebx + 0x9d], 0xef
loc_3_1B11:
        ret    
sub_3_1B12:
        call   sub_3_1E50
        test   byte ptr [ebx + 0x9d], 0x10
        je     loc_3_1B30
        and    byte ptr [ebx + 0x9d], 0xef
        or     edx, edx
        je     loc_3_1B36
        call   sub_3_1C5C
loc_3_1B30:
        call   sub_3_1B44
        ret    
loc_3_1B36:
        call   sub_3_1B44
        call   sub_3_1CD0
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_1B44:
        test   byte ptr [ebx + 0x9d], 0x20
        je     sub_3_1B80
        mov    al, 0
        call   sub_3_25C3
        mov    eax, dword ptr [ebx + 0xe8]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1B80
        mov    al, 2
        call   sub_3_25C3
        mov    eax, dword ptr [ebx + 0xec]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1B80
        ret    
        db  08Bh, 0C0h
sub_3_1B80:
        push   edi
        push   ebp
        mov    esi, edi
        add    esi, dword ptr [ebx + 6]
        mov    edx, edi
        add    edx, dword ptr [ebx + 0xd4]
        mov    edi, dword ptr [ebx + 0xbc]
        mov    ecx, dword ptr [ebx + 0xc8]
        mov    eax, 5
        test   byte ptr [ebx + 0x9d], 0x40
        je     loc_3_1BAE
        mov    eax, 0x28
loc_3_1BAE:
        neg    eax
        add    eax, dword ptr [ebx + 0xd8]
        mov    ebp, dword ptr [ebx + 0x7f]
        test   byte ptr [ebx + 0x9d], 0x40
        je     loc_3_1BFD
        mov    eax, eax
loc_3_1BC4:
        cmp    esi, edx
        jae    loc_3_1C56
        push   ecx
        sub    ecx, ecx
        cmp    esi, dword ptr [ebx + 0x16]
        jae    loc_3_1BDF
        mov    ecx, dword ptr [ebx + 0x16]
        sub    ecx, esi
        add    esi, ecx
        add    edi, ecx
        neg    ecx
loc_3_1BDF:
        add    ecx, 0x28
        jg     loc_3_1BEC
        add    esi, ecx
        add    edi, ecx
        xor    ecx, ecx
        mov    eax, eax
loc_3_1BEC:
        jecxz  loc_3_1BF6
        push   eax
        nop    
loc_3_1BF0:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        dec    ecx
        jne    loc_3_1BF0
        pop    eax
loc_3_1BF6:
        pop    ecx
        add    esi, eax
        loop   loc_3_1BC4
        jmp    loc_3_1C56
loc_3_1BFD:
        cmp    esi, dword ptr [ebx + 0x16]
        jae    loc_3_1C2C
        push   ecx
        mov    ecx, dword ptr [ebx + 0x16]
        sub    ecx, esi
        add    esi, ecx
        add    edi, ecx
        neg    ecx
        add    ecx, 5
        jg     loc_3_1C1C
        add    esi, ecx
        add    edi, ecx
        sub    ecx, ecx
        mov    eax, eax
loc_3_1C1C:
        jecxz  loc_3_1C26
        push   eax
        nop    
loc_3_1C20:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        dec    ecx
        jne    loc_3_1C20
        pop    eax
loc_3_1C26:
        pop    ecx
        jmp    loc_3_1C38
        db  02Eh, 08Bh, 0C0h
loc_3_1C2C:
        push   eax
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        pop    eax
loc_3_1C38:
        add    esi, eax
        or     ebp, ebp
        je     loc_3_1C54
        sub    esi, dword ptr [ebx + 0xd8]
        add    esi, ebp
        cmp    esi, edx
        jb     loc_3_1C54
        sub    esi, ebp
        sub    esi, ebp
        add    esi, dword ptr [ebx + 0xd8]
loc_3_1C54:
        loop   loc_3_1BFD
loc_3_1C56:
        pop    ebp
        pop    edi
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_1C5C:
        test   byte ptr [ebx + 0x9d], 0x20
        je     sub_3_1CA0
        mov    al, 3
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0xe0]
        mov    dword ptr [ebx + 0xb8], eax
        call   sub_3_1CA0
        mov    al, 3
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0xe4]
        mov    dword ptr [ebx + 0xb8], eax
        call   sub_3_1CA0
        mov    al, 0xf
        call   sub_3_25CE
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_1CA0:
        push   edi
        mov    eax, dword ptr [ebx + 0x77]
        mov    esi, dword ptr [ebx + 0x7b]
        push   eax
        push   esi
        call   sub_3_1ED8
        add    esi, edi
        mov    edx, edi
        add    edx, dword ptr [ebx + 0xd4]
        mov    edi, dword ptr [ebx + 0xb8]
        xchg   edi, esi
        sub    ecx, ecx
        push   0x10
        call   sub_3_20C0
        add    esp, 0xc
        pop    edi
        ret    
        db  08Bh, 0C0h
sub_3_1CD0:
        test   byte ptr [ebx + 0x9d], 0x20
        je     sub_3_1D14
        mov    eax, dword ptr [ebx + 0xe0]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xe8]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1D14
        mov    eax, dword ptr [ebx + 0xe4]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xec]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1D14
        ret    
sub_3_1D14:
        mov    eax, dword ptr [ebx + 0x77]
        mov    esi, dword ptr [ebx + 0x7b]
        call   sub_3_207C
        push   edi
        mov    edi, dword ptr [ebx + 0xb8]
        xchg   edi, esi
        test   byte ptr [ebx + 0x9d], 0x40
        je     loc_3_1D44
        mov    eax, eax
loc_3_1D34:
        push   ecx
        mov    ecx, 0x18
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        add    edi, eax
        loop   loc_3_1D34
        jmp    loc_3_1D4B
        db  090h
loc_3_1D44:
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        add    edi, eax
        loop   loc_3_1D44
loc_3_1D4B:
        pop    edi
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_1D50:
        test   byte ptr [ebx + 0x9d], 0x20
        je     sub_3_1D94
        mov    al, 3
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0xe8]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1D94
        mov    al, 0xc
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0xec]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1D94
        mov    al, 0xf
        call   sub_3_25CE
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_1D94:
        push   edi
        mov    esi, dword ptr [ebx + 0xbc]
        mov    edx, edi
        add    edx, dword ptr [ebx + 0xd4]
        add    edi, dword ptr [ebx + 6]
        mov    ecx, 2
        push   dword ptr [ebx + 0xc0]
        push   dword ptr [ebx + 0xc4]
        push   dword ptr [ebx + 0xc8]
        call   sub_3_20C0
        add    esp, 0xc
        pop    edi
        ret    
        db  090h
sub_3_1DC8:
        test   byte ptr [ebx + 0x9d], 0x20
        je     sub_3_1E0C
        mov    eax, dword ptr [ebx + 0xe0]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xe8]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1E0C
        mov    eax, dword ptr [ebx + 0xe4]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xec]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_1E0C
        ret    
sub_3_1E0C:
        mov    eax, dword ptr [ebx + 0x6f]
        mov    esi, dword ptr [ebx + 0x73]
        mov    dword ptr [ebx + 0x77], eax
        mov    dword ptr [ebx + 0x7b], esi
        call   sub_3_207C
        push   edi
        mov    edi, dword ptr [ebx + 0xb8]
        test   byte ptr [ebx + 0x9d], 0x40
        je     loc_3_1E40
        mov    eax, eax
loc_3_1E30:
        push   ecx
        mov    ecx, 0x18
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        add    esi, eax
        loop   loc_3_1E30
        jmp    loc_3_1E47
        db  090h
loc_3_1E40:
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        add    esi, eax
        loop   loc_3_1E40
loc_3_1E47:
        pop    edi
        or     byte ptr [ebx + 0x9d], 0x10
        ret    
sub_3_1E50:
        sub    edx, edx
        test   byte ptr [ebx + 0x9d], 0x10
        je     loc_3_1E9A
        mov    eax, dword ptr [ebx + 0x6f]
        mov    ecx, dword ptr [ebx + 0x77]
        cmp    eax, ecx
        jge    loc_3_1E66
        xchg   ecx, eax
loc_3_1E66:
        test   byte ptr [ebx + 0x9d], 0x40
        je     loc_3_1E76
        sub    eax, ecx
        cmp    eax, 0x10
        jmp    loc_3_1E7E
loc_3_1E76:
        and    ecx, 0xfffffff8
        sub    eax, ecx
        cmp    eax, 0x18
loc_3_1E7E:
        jge    loc_3_1E9A
        mov    dword ptr [ebx + 0xc0], ecx
        mov    eax, dword ptr [ebx + 0x73]
        mov    ecx, dword ptr [ebx + 0x7b]
        sub    eax, ecx
        jge    loc_3_1E95
        neg    eax
        mov    ecx, dword ptr [ebx + 0x73]
loc_3_1E95:
        cmp    eax, 4
        jle    loc_3_1EB4
loc_3_1E9A:
        mov    eax, dword ptr [ebx + 0x6f]
        and    eax, 0xfffffff8
        mov    dword ptr [ebx + 0xc0], eax
        mov    eax, dword ptr [ebx + 0x73]
        mov    dword ptr [ebx + 0xc4], eax
        sub    eax, eax
        inc    edx
        jmp    loc_3_1EBA
loc_3_1EB4:
        mov    dword ptr [ebx + 0xc4], ecx
loc_3_1EBA:
        add    eax, 0x10
        mov    dword ptr [ebx + 0xc8], eax
        mov    eax, dword ptr [ebx + 0xc0]
        mov    esi, dword ptr [ebx + 0xc4]
        call   sub_3_1ED8
        mov    dword ptr [ebx + 6], esi
        ret    
sub_3_1ED8:
        push   edx
        push   edi
        xchg   esi, eax
        mov    edi, dword ptr [ebx + 0x7f]
        or     edi, edi
        je     loc_3_1EE8
        sar    eax, 1
        jb     loc_3_1EE8
        sub    edi, edi
loc_3_1EE8:
        imul   dword ptr [ebx + 0xd8]
        xchg   esi, eax
        test   byte ptr [ebx + 0x9d], 0x40
        jne    loc_3_1EFB
        sar    eax, 3
loc_3_1EFB:
        add    esi, eax
        add    esi, edi
        pop    edi
        pop    edx
        ret    
        db  08Bh, 0C0h
loc_3_1F04:
        lodsw  ax, word ptr [esi]
        and    eax, dword ptr [edi]
        xor    eax, dword ptr [ebx]
        stosw  word ptr es:[edi], ax
        add    ebx, 2
        lodsb  al, byte ptr [esi]
        and    al, byte ptr [edi]
        xor    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        add    edi, 2
        loop   loc_3_1F04
        ret    
loc_3_1F1C:
        mov    ah, 1
        push   eax
        and    ah, 0xfe
        mov    al, 5
        test   dl, 4
        mov    edx, 0x3ce
        pushfd 
        call   sub_3_2669
        popfd  
        jne    loc_3_1F72
        mov    al, 8
        call   sub_3_267D
        inc    edx
        mov    eax, eax
loc_3_1F40:
        lodsb  al, byte ptr [esi]
        call   sub_3_2685
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        lodsb  al, byte ptr [esi]
        call   sub_3_2685
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        lodsb  al, byte ptr [esi]
        call   sub_3_2685
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        add    edi, 2
        loop   loc_3_1F40
        mov    al, 0xff
        call   sub_3_2685
        jmp    loc_3_1FCC
loc_3_1F72:
        mov    al, 3
        call   sub_3_267D
        inc    edx
        mov    eax, eax
loc_3_1F7C:
        mov    al, 8
        call   sub_3_2685
        mov    al, byte ptr [edi]
        lodsb  al, byte ptr [esi]
        mov    byte ptr [edi], al
        mov    al, 0x18
        call   sub_3_2685
        mov    al, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        mov    al, 8
        call   sub_3_2685
        mov    al, byte ptr [edi]
        lodsb  al, byte ptr [esi]
        mov    byte ptr [edi], al
        mov    al, 0x18
        call   sub_3_2685
        mov    al, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        mov    al, 8
        call   sub_3_2685
        mov    al, byte ptr [edi]
        lodsb  al, byte ptr [esi]
        mov    byte ptr [edi], al
        mov    al, 0x18
        call   sub_3_2685
        mov    al, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb  byte ptr es:[edi], al
        inc    ebx
        add    edi, 2
        loop   loc_3_1F7C
loc_3_1FCC:
        dec    edx
        pop    eax
        mov    al, 5
        call   sub_3_2669
        ret    
        db  08Bh, 0C0h
loc_3_1FD8:
        mov    edx, 3
loc_3_1FDD:
        push   ebx
        mov    bh, byte ptr [ebx]
        mov    bl, byte ptr [esi]
        mov    ah, 8
loc_3_1FE4:
        mov    al, byte ptr [edi]
        shl    bl, 1
        jae    loc_3_1FEC
        xor    al, al
loc_3_1FEC:
        shl    bh, 1
        jae    loc_3_1FF2
        xor    al, 0xf
loc_3_1FF2:
        stosb  byte ptr es:[edi], al
        dec    ah
        jne    loc_3_1FE4
        pop    ebx
        inc    ebx
        inc    esi
        dec    edx
        jne    loc_3_1FDD
        add    edi, 0x10
        loop   loc_3_1FD8
        ret    
sub_3_2003:
        test   byte ptr [ebx + 0x9d], 0x20
        je     sub_3_2048
        mov    eax, dword ptr [ebx + 0xe0]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xe8]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_2048
        mov    eax, dword ptr [ebx + 0xe4]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xec]
        mov    dword ptr [ebx + 0xbc], eax
        call   sub_3_2048
        ret    
        db  090h
sub_3_2048:
        mov    eax, dword ptr [ebx + 0x6f]
        mov    esi, dword ptr [ebx + 0x73]
        call   sub_3_207C
        push   ebx
        push   edi
        mov    edi, esi
        push   0x2076                        ; -> loc_3_2076
        push   dword ptr [ebx + 0xcc]
        movzx  edx, word ptr [ebx + 0x9d]
        mov    esi, dword ptr [ebx + 0xf0]
        mov    ebx, dword ptr [ebx + 0xf4]
        ret    
loc_3_2076:
        pop    edi
        pop    ebx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_207C:
        sub    eax, dword ptr [ebx + 0xc0]
        mov    ecx, 0x28
        mov    edx, 0x10
        test   byte ptr [ebx + 0x9d], 0x40
        jne    loc_3_20A2
        sar    eax, 3
        mov    ecx, 5
        mov    edx, 2
loc_3_20A2:
        xchg   esi, eax
        sub    eax, dword ptr [ebx + 0xc4]
        push   edx
        imul   ecx
        pop    edx
        add    esi, eax
        add    esi, dword ptr [ebx + 0xbc]
        mov    ecx, 0x10
        mov    eax, edx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_3_20C0:
        enter  0xc, 0
        mov    dword ptr [ebp - 4], edx
        add    ecx, 3
        test   byte ptr [ebx + 0x9d], 0x40
        je     loc_3_20D6
        shl    ecx, 3
loc_3_20D6:
        mov    eax, dword ptr [ebx + 0xd8]
        sub    eax, ecx
        mov    edx, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0xc], 0
        mov    ecx, dword ptr [ebp + 0x10]
        test   byte ptr [ebx + 0x9d], 0x40
        jne    loc_3_20F9
        sar    ecx, 3
loc_3_20F9:
        or     ecx, ecx
        jge    loc_3_2105
        neg    ecx
        add    edi, ecx
        add    esi, ecx
        jmp    loc_3_2110
loc_3_2105:
        add    ecx, dword ptr [ebp - 8]
        sub    ecx, dword ptr [ebx + 0xd8]
        jle    loc_3_2119
loc_3_2110:
        mov    dword ptr [ebp - 0xc], ecx
        add    eax, ecx
        sub    edx, ecx
        jle    loc_3_218B
loc_3_2119:
        mov    ecx, dword ptr [ebp + 0xc]
        or     ecx, ecx
        jge    loc_3_2144
        neg    ecx
        push   ecx
        nop    
loc_3_2124:
        add    esi, dword ptr [ebp - 8]
        cmp    dword ptr [ebx + 0x7f], 0
        je     loc_3_213B
        add    edi, dword ptr [ebx + 0x7f]
        cmp    edi, dword ptr [ebp - 4]
        jb     loc_3_213F
        sub    edi, dword ptr [ebx + 0x7f]
        sub    edi, dword ptr [ebx + 0x7f]
loc_3_213B:
        add    edi, eax
        add    edi, edx
loc_3_213F:
        loop   loc_3_2124
        pop    ecx
        jmp    loc_3_2150
loc_3_2144:
        add    ecx, dword ptr [ebp + 8]
        sub    cx, word ptr [ebx + 0x8b]
        jle    loc_3_2155
loc_3_2150:
        sub    dword ptr [ebp + 8], ecx
        jle    loc_3_218B
loc_3_2155:
        mov    dh, byte ptr [ebp + 8]
loc_3_2158:
        sub    ecx, ecx
        mov    cl, dl
        jecxz  loc_3_2166
        push   eax
        nop    
loc_3_2160:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        dec    ecx
        jne    loc_3_2160
        pop    eax
loc_3_2166:
        add    edi, eax
        add    esi, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebx + 0x7f]
        jecxz  loc_3_2187
        sub    edi, dword ptr [ebx + 0xd8]
        add    edi, ecx
        cmp    edi, dword ptr [ebp - 4]
        jb     loc_3_2187
        sub    edi, ecx
        sub    edi, ecx
        add    edi, dword ptr [ebx + 0xd8]
loc_3_2187:
        dec    dh
        jne    loc_3_2158
loc_3_218B:
        leave  
        ret    
sub_3_218D:
        push   edi
        cmp    dword ptr [ebx + 0xcc], 0x1f1c ; -> loc_3_1F1C
        jne    loc_3_2216
        test   byte ptr [ebx + 0x9f], 1
        jne    loc_3_21E8
        or     byte ptr [ebx + 0x9f], 1
        lea    edi, [ebx + 0xa1]
        mov    edx, 0x3c4
        mov    ah, 2
        call   sub_3_25A4
        mov    byte ptr [edi + 9], ah
        mov    eax, 0xf02
        call   sub_3_2655
        mov    edx, 0x3ce
        mov    ecx, 8
        call   sub_3_2563
        mov    ecx, 0x708
        lea    edi, [ebx + 0xab]
        call   sub_3_258E
loc_3_21E8:
        test   byte ptr [ebx + 0x89], 1
        je     loc_3_2206
        mov    eax, 0x105
        call   sub_3_2669
        pop    edi
        push   edi
        mov    ecx, dword ptr [ebx + 0xdc]
        mov    byte ptr [ecx + edi], al
loc_3_2206:
        mov    ecx, 5
        lea    edi, [ebx + 0xab]
        call   sub_3_258E
loc_3_2216:
        pop    edi
        ret    
sub_3_2218:
        push   edi
        cmp    dword ptr [ebx + 0xcc], 0x1f1c ; -> loc_3_1F1C
        jne    loc_3_2297
        test   byte ptr [ebx + 0x89], 1
        je     loc_3_2263
        and    byte ptr [ebx + 0x9f], 0xfe
        mov    edx, 0x3ce
        mov    eax, 3
        call   sub_3_2669
        mov    eax, 0xff08
        call   sub_3_2669
        mov    eax, 0xff02
        mov    dl, 0xc4
        call   sub_3_2669
        mov    edx, dword ptr [ebx + 0xdc]
        mov    al, byte ptr [edx + edi]
loc_3_2263:
        lea    edi, [ebx + 0xa1]
        mov    edx, 0x3c4
        mov    ah, byte ptr [edi + 9]
        mov    al, 2
        call   sub_3_2655
        mov    edx, 0x3ce
        mov    ecx, 5
        call   sub_3_258E
        lea    edi, [ebx + 0xa1]
        mov    ecx, 0x708
        call   sub_3_258E
loc_3_2297:
        pop    edi
        ret    
sub_3_2299:
        mov    dl, 7
        mov    al, byte ptr [ebx + 0x6f]
        and    al, dl
        and    dl, byte ptr [ebx + 0x6e]
        mov    byte ptr [ebx + 0x6e], al
        sub    al, dl
        je     loc_3_22E2
        push   edi
        jl     loc_3_22C7
        push   eax
        mov    esi, dword ptr [ebx + 0xf0]
        call   sub_3_2308
        pop    eax
        mov    esi, dword ptr [ebx + 0xf4]
        call   sub_3_2308
        jmp    loc_3_22E1
loc_3_22C7:
        neg    al
        push   eax
        mov    esi, dword ptr [ebx + 0xf0]
        call   sub_3_22E3
        pop    eax
        mov    esi, dword ptr [ebx + 0xf4]
        call   sub_3_22E3
loc_3_22E1:
        pop    edi
loc_3_22E2:
        ret    
sub_3_22E3:
        mov    dl, al
        add    esi, 0x30
        dec    esi
        std    
        push   esi
        mov    edi, esi
        mov    cl, al
        mov    dh, 0x30
        xor    dl, dl
        nop    
loc_3_22F4:
        xor    ah, ah
        lodsb  al, byte ptr [esi]
        shl    ax, cl
        or     al, dl
        mov    dl, ah
        stosb  byte ptr es:[edi], al
        dec    dh
        jne    loc_3_22F4
        pop    esi
        cld    
        or     byte ptr [esi], dl
        ret    
sub_3_2308:
        push   esi
        mov    edi, esi
        mov    cl, al
        xor    dl, dl
        mov    dh, 0x30
        mov    eax, eax
loc_3_2314:
        xor    ah, ah
        lodsb  al, byte ptr [esi]
        ror    ax, cl
        or     al, dl
        mov    dl, ah
        stosb  byte ptr es:[edi], al
        dec    dh
        jne    loc_3_2314
        pop    esi
        or     byte ptr [esi], dl
        ret    
sub_3_2327:
        mov    dword ptr [0x841], ecx        ; -> obj1:0x0841
        test   byte ptr [ebx + 0x89], 4
        jne    loc_3_2495
        test   byte ptr [ebx + 0x89], 1
        jne    loc_3_236D
        movzx  eax, word ptr [0x843]         ; -> obj1:0x0843
        cdq    
        div    word ptr [ebx + 0x93]
        mov    word ptr [0x843], ax          ; -> obj1:0x0843
        movzx  eax, word ptr [0x841]         ; -> obj1:0x0841
        cdq    
        div    word ptr [ebx + 0x91]
        mov    word ptr [0x841], ax          ; -> obj1:0x0841
loc_3_236D:
        test   byte ptr [ebx + 0x9d], 2
        je     loc_3_23A0
        mov    ax, word ptr [0x843]          ; -> obj1:0x0843
        cmp    ax, word ptr [ebx + 4]
        jne    loc_3_2392
        mov    ax, word ptr [0x841]          ; -> obj1:0x0841
        cmp    ax, word ptr [ebx + 2]
        je     loc_3_2495
loc_3_2392:
        test   byte ptr [ebx + 0x89], 1
        jne    loc_3_23A0
        call   sub_3_1A3A
loc_3_23A0:
        mov    eax, dword ptr [0x841]        ; -> obj1:0x0841
        call   sub_3_2496
        jb     loc_3_2495
        mov    esi, dword ptr [ebx + 0x16]
        test   byte ptr [ebx + 0x89], 1
        je     loc_3_23C6
        call   sub_3_1A79
        jmp    loc_3_2486
loc_3_23C6:
        mov    ax, word ptr [ebx + 0x8d]
        mul    word ptr [ebx + 0x93]
        cmp    word ptr [0x843], ax          ; -> obj1:0x0843
        ja     loc_3_2495
        mov    ax, word ptr [ebx + 0x8b]
        mul    word ptr [ebx + 0x91]
        mov    cx, word ptr [0x841]          ; -> obj1:0x0841
        cmp    cx, ax
        ja     loc_3_2495
        movzx  eax, cx
        movzx  ecx, word ptr [ebx + 0x8d]
        test   byte ptr [ebx + 0x9d], 8
        je     loc_3_2457
        mul    ecx
        movzx  ecx, word ptr [0x843]         ; -> obj1:0x0843
        add    eax, ecx
        pushal 
        push   eax
        mov    al, 0xe
        mov    ecx, 0xc
        mov    edx, dword ptr [0x7f8]        ; -> loc_1_07F8
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07F4
        pop    eax
        mov    ah, al
        mov    al, 0xf
        mov    ecx, 0xc
        mov    edx, dword ptr [0x7f8]        ; -> loc_1_07F8
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07F4
        popal  
        jmp    loc_3_2486
loc_3_2457:
        call   sub_3_26F8
        shl    ecx, 1
        mul    ecx
        movzx  ecx, word ptr [0x843]         ; -> obj1:0x0843
        add    eax, ecx
        add    eax, ecx
        mov    dword ptr [ebx + 6], eax
        add    esi, eax
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 0x12], ax
        and    ax, word ptr [ebx + 0x28]
        xor    ax, word ptr [ebx + 0x26]
        mov    word ptr [ebx + 0x14], ax
        mov    word ptr [esi], ax
loc_3_2486:
        mov    eax, dword ptr [0x841]        ; -> obj1:0x0841
        mov    dword ptr [ebx + 2], eax
        or     byte ptr [ebx + 0x9d], 2
loc_3_2495:
        ret    
sub_3_2496:
        test   byte ptr [ebx + 0x9f], 2
        je     loc_3_24D0
        mov    ecx, eax
        test   byte ptr [ebx + 0x89], 1
        je     loc_3_24B0
        add    ecx, 0x27001f
loc_3_24B0:
        cmp    ax, word ptr [ebx + 0xe]
        ja     loc_3_24D0
        cmp    cx, word ptr [ebx + 0xa]
        jl     loc_3_24D0
        shr    ecx, 0x10
        shr    eax, 0x10
        cmp    ax, word ptr [ebx + 0x10]
        ja     loc_3_24D0
        cmp    cx, word ptr [ebx + 0xc]
        jl     loc_3_24D0
        stc    
        ret    
loc_3_24D0:
        clc    
        ret    
sub_3_24D2:
        test   byte ptr [ebx + 0x89], 1
        je     loc_3_24DC
        ret    
loc_3_24DC:
        and    byte ptr [ebx + 0x9d], 0xf7
        test   byte ptr [ebx + 0x24], 1
        je     loc_3_253D
        or     byte ptr [ebx + 0x9d], 8
        movzx  ecx, word ptr [ebx + 0x26]
        movzx  eax, word ptr [ebx + 0x28]
        mov    ch, al
        and    ecx, 0xf0f
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 1
        mov    word ptr [ebp + 0x18], cx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_3_253D:
        ret    
loc_3_253E:
        xor    eax, eax
        sub    ebx, dword ptr [0x7d8]        ; -> loc_1_07D8
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
        add    ebx, dword ptr [0x7d8]        ; -> loc_1_07D8
        call   sub_3_1A3A
        mov    byte ptr [ebx + 1], 0
        and    byte ptr [ebx + 0x9f], 0xfd
        ret    
sub_3_2563:
        movzx  eax, ch
        add    edi, eax
        call   sub_3_2619
        push   ax
loc_3_256F:
        mov    al, ch
        call   sub_3_267D
        inc    dl
        call   sub_3_2637
        stosb  byte ptr es:[edi], al
        inc    ch
        dec    dl
        cmp    ch, cl
        jbe    loc_3_256F
        pop    ax
        call   sub_3_267D
        ret    
sub_3_258E:
        movzx  eax, ch
        add    edi, eax
loc_3_2593:
        mov    al, ch
        mov    ah, byte ptr [edi]
        inc    edi
        call   sub_3_2669
        inc    ch
        cmp    ch, cl
        jbe    loc_3_2593
        ret    
sub_3_25A4:
        push   ecx
        call   sub_3_25DD
        xchg   al, ah
        call   sub_3_2699
        inc    dl
        call   sub_3_25FB
        xchg   al, ah
        dec    dl
        call   sub_3_2699
        pop    ecx
        ret    
sub_3_25C3:
        mov    ah, al
        mov    al, 4
        mov    edx, 0x3ce
        jmp    loc_3_25D7
sub_3_25CE:
        mov    ah, al
        mov    al, 2
        mov    edx, 0x3c4
loc_3_25D7:
        call   sub_3_2655
        ret    
sub_3_25DD:
        pushal 
        mov    ecx, 0
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07E4
        mov    ebx, dword ptr [esp + 0x1c]
        mov    bl, al
        mov    dword ptr [esp + 0x1c], ebx
        popal  
        ret    
sub_3_25FB:
        pushal 
        mov    ecx, 0
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07E8
        mov    ebx, dword ptr [esp + 0x1c]
        mov    bl, al
        mov    dword ptr [esp + 0x1c], ebx
        popal  
        ret    
sub_3_2619:
        pushal 
        mov    ecx, 0
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07EC
        mov    ebx, dword ptr [esp + 0x1c]
        mov    bl, al
        mov    dword ptr [esp + 0x1c], ebx
        popal  
        ret    
sub_3_2637:
        pushal 
        mov    ecx, 0
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07F0
        mov    ebx, dword ptr [esp + 0x1c]
        mov    bl, al
        mov    dword ptr [esp + 0x1c], ebx
        popal  
        ret    
sub_3_2655:
        pushal 
        mov    ecx, 0xc
loc_3_265B:
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07E4
        popal  
        ret    
sub_3_2669:
        pushal 
        mov    ecx, 0xc
loc_3_266F:
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07EC
        popal  
        ret    
sub_3_267D:
        pushal 
        mov    ecx, 4
        jmp    loc_3_266F
sub_3_2685:
        pushal 
        mov    ecx, 4
        mov    ebx, dword ptr [0x7dc]        ; -> loc_1_07DC
        call   sub_1_07F0
        popal  
        ret    
sub_3_2699:
        pushal 
        mov    ecx, 4
        jmp    loc_3_265B
        db  060h, 0B9h, 004h, 000h, 000h, 000h, 08Bh, 01Dh
        dd  loc_1_07DC               ; fixup
        db  0FFh, 015h
        dd  sub_1_07E8               ; fixup
        db  061h, 0C3h
loc_3_26B5:
        cmp    ecx, 4
        ja     loc_3_26C0
        jb     loc_3_26BE
        out    dx, al
        ret    
loc_3_26BE:
        in     al, dx
        ret    
loc_3_26C0:
        cmp    ecx, 0xc
        jne    loc_3_26C8
        out    dx, ax
        ret    
loc_3_26C8:
        ret    
loc_3_26C9:
        cmp    cx, 1
        mov    ecx, 0xf8
        jb     loc_3_26DE
        je     loc_3_26DA
        mov    edi, ebx
        jmp    loc_3_26DC
loc_3_26DA:
        mov    esi, ebx
loc_3_26DC:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_26DE:
        mov    eax, ecx
        ret    
loc_3_26E1:
        mov    byte ptr [ebx + 1], cl
        movzx  eax, word ptr [ebx + 0x96]
        movzx  ecx, cl
        imul   eax, ecx
        add    eax, dword ptr [ebx + 0x1a]
        mov    dword ptr [ebx + 0x16], eax
        ret    
sub_3_26F8:
        cmp    byte ptr [ebx + 1], 0
        jne    loc_3_2709
        push   eax
        movzx  eax, word ptr [0x44e]
        add    esi, eax
        pop    eax
loc_3_2709:
        ret    
loc_3_270A:
        ret    
loc_3_270B:
        ret    
sub_3_270C:
        mov    dword ptr [0x15c], ecx        ; -> loc_4_015C
        or     ecx, ecx
        je     loc_3_2821
        or     word ptr [ecx], 8
        mov    edi, ecx
        mov    ebx, 0x1ad                    ; -> obj4:0x01AD
        mov    dword ptr [ebx], 4
        lea    eax, [edi + 2]
        push   0
        push   ebx
        push   eax
        push   3
        push   0x1b5                         ; -> obj4:0x01B5
        push   0
        push   dword ptr [edi + 0xc]
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        test   eax, eax
        je     loc_3_27C4
        push   0
        push   dword ptr [edi + 0xc]
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_284B
        mov    esi, 0x1b1                    ; -> obj4:0x01B1
        push   2
        push   dword ptr [ebx]
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_284B
        push   0
        push   0
        push   2
        push   dword ptr [esi]
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_284B
        mov    eax, dword ptr [ebx]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edi + 2], ax
        push   0
        push   0
        push   4
        push   dword ptr [esi]
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_284B
        mov    eax, dword ptr [ebx]
        movzx  eax, word ptr [eax + 2]
        mov    byte ptr [edi + 4], al
loc_3_27C4:
        mov    ecx, edi
        mov    esi, 0x174                    ; -> obj4:0x0174
        movzx  ax, byte ptr [ecx + 4]
        mov    word ptr [esi + 2], ax
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [esi], ax
        lea    edi, [0x164]                  ; -> loc_4_0164
loc_3_27E1:
        cmp    ax, word ptr [edi]
        je     loc_3_27EB
        add    edi, 4
        jmp    loc_3_27E1
loc_3_27EB:
        mov    esi, edi
        call   sub_3_2977
        jb     loc_3_284C
        call   sub_3_28CB
        jb     loc_3_284C
        mov    esi, dword ptr [0x15c]        ; -> loc_4_015C
        mov    word ptr [esi + 6], 2
        and    word ptr [esi], 0xfff7
        or     word ptr [esi], 4
        mov    al, byte ptr [0x697]          ; -> obj1:0x0697
        mov    byte ptr [esi + 5], al
        mov    dword ptr [esi + 0x18], 0x2ad5 ; -> sub_3_2AD5
        clc    
        jmp    loc_3_284C
loc_3_2821:
        mov    esi, 0x164                    ; -> loc_4_0164
        mov    cl, 0
loc_3_2828:
        push   ecx
        call   sub_3_2977
        pop    ecx
        jae    loc_3_283D
        add    esi, 4
        inc    cl
        cmp    cl, 4
        jb     loc_3_2828
        jmp    loc_3_284B
loc_3_283D:
        call   sub_3_28CB
        cmp    byte ptr [0x694], 2           ; -> obj1:0x0694
        je     loc_3_284C
loc_3_284B:
        stc    
loc_3_284C:
        ret    
sub_3_284D:
        pushal 
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        cmp    eax, 0x400
        jb     loc_3_28C8
        movzx  eax, word ptr [esi]
        VxDCall  VCD.VCD_Find_COM_Index          ; cd2009000e00
        jae    loc_3_2870
        cmp    word ptr [esi], 0x2e8
        jne    loc_3_28C8
        stc    
        jmp    loc_3_28C9
loc_3_2870:
        test   edx, edx
        je     loc_3_2896
        push   eax
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        jecxz  loc_3_2882
        mov    eax, dword ptr [ecx + 8]
        jmp    loc_3_2887
loc_3_2882:
        mov    eax, dword ptr [0x179]        ; -> loc_4_0179
loc_3_2887:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VCD.VCD_Free_Port_Windows_Style ; cd2007000e00
        pop    eax
        jmp    loc_3_28C2
loc_3_2896:
        push   eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   4
        pop    ecx
        xor    esi, esi
        xor    edi, edi
        VxDCall  VCD.VCD_Acquire_Port_Windows_Style ; cd2006000e00
        mov    ebx, eax
        pop    eax
        jb     loc_3_28C9
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        jecxz  loc_3_28BC
        mov    dword ptr [ecx + 8], ebx
        jmp    loc_3_28C2
loc_3_28BC:
        mov    dword ptr [0x179], ebx        ; -> loc_4_0179
loc_3_28C2:
        VxDCall  VCD.VCD_Set_Port_Global_Special ; cd200a000e00
loc_3_28C8:
        clc    
loc_3_28C9:
        popal  
        ret    
sub_3_28CB:
        push   1
        push   0xc
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_3_28E1
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_28E1:
        mov    edx, eax
        mov    ax, word ptr [esi]
        mov    word ptr [edx], ax
        mov    al, byte ptr [0x178]          ; -> obj4:0x0178
        mov    byte ptr [edx + 2], al
        mov    edi, 0x13c                    ; -> obj4:0x013C
        movzx  eax, word ptr [esi + 2]
        mov    word ptr [edi], ax
        mov    dword ptr [edi + 4], 0x504    ; -> loc_1_0504
        mov    word ptr [edi + 2], 6
        mov    dword ptr [edi + 0x1c], edx
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        mov    dword ptr [edx + 8], ecx
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jae    loc_3_2928
        mov    edx, esp
        call   sub_3_284D
        stc    
        jmp    loc_3_2976
loc_3_2928:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        jecxz  loc_3_2946
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0x1c], 0x2aff ; -> loc_3_2AFF
loc_3_2946:
        mov    eax, esi
        sub    eax, 0x164                    ; -> loc_4_0164
        shr    al, 2
        inc    al
        mov    byte ptr [0x697], al          ; -> obj1:0x0697
        mov    ax, word ptr [esi]
        mov    word ptr [0x17d], ax          ; -> loc_4_017D
        mov    ax, word ptr [esi + 2]
        mov    byte ptr [0x695], al          ; -> obj1:0x0695
        mov    byte ptr [0x694], 2           ; -> obj1:0x0694
        call   sub_3_2AD5
        clc    
loc_3_2976:
        ret    
sub_3_2977:
        xor    edx, edx
        call   sub_3_284D
        jb     loc_3_2994
        push   esi
        movzx  esi, word ptr [esi]
        call   sub_3_2995
        pop    esi
        jae    loc_3_2994
        mov    edx, esp
        call   sub_3_284D
        stc    
loc_3_2994:
        ret    
sub_3_2995:
        lea    edx, [esi + 3]
        in     al, dx
        push   eax
        mov    al, 0x80
        jmp    loc_3_299E
loc_3_299E:
        out    dx, al
        lea    edx, [esi + 1]
        jmp    loc_3_29A4
loc_3_29A4:
        in     al, dx
        push   eax
        mov    al, 0
        jmp    loc_3_29AA
loc_3_29AA:
        out    dx, al
        lea    edx, [esi]
        jmp    loc_3_29AF
loc_3_29AF:
        in     al, dx
        push   eax
        mov    al, 0x60
        jmp    loc_3_29B5
loc_3_29B5:
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 2
        out    dx, al
        lea    edx, [esi + 1]
        jmp    loc_3_29C1
loc_3_29C1:
        in     al, dx
        push   eax
        xor    al, al
        jmp    loc_3_29C7
loc_3_29C7:
        out    dx, al
        lea    edx, [esi + 3]
        jmp    loc_3_29CD
loc_3_29CD:
        in     al, dx
        lea    edx, [esi + 4]
        jmp    loc_3_29D3
loc_3_29D3:
        in     al, dx
        push   eax
        mov    dword ptr [0x1bd], 0x64       ; -> obj4:0x01BD
        mov    dword ptr [0x1c1], 0x32       ; -> loc_4_01C1
        call   sub_3_2A43
        jae    loc_3_2A3A
        lea    edx, [esi + 4]
        mov    dword ptr [0x1bd], 0x1f4      ; -> obj4:0x01BD
        mov    dword ptr [0x1c1], 0x190      ; -> loc_4_01C1
        call   sub_3_2A43
        jae    loc_3_2A3A
        lea    edx, [esi + 4]
        pop    eax
        jmp    loc_3_2A14
loc_3_2A14:
        out    dx, al
        lea    edx, [esi + 1]
        pop    eax
        jmp    loc_3_2A1B
loc_3_2A1B:
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 0x80
        jmp    loc_3_2A23
loc_3_2A23:
        out    dx, al
        lea    edx, [esi]
        pop    eax
        jmp    loc_3_2A29
loc_3_2A29:
        out    dx, al
        lea    edx, [esi + 1]
        pop    eax
        jmp    loc_3_2A30
loc_3_2A30:
        out    dx, al
        lea    edx, [esi + 3]
        pop    eax
        jmp    loc_3_2A37
loc_3_2A37:
        out    dx, al
        stc    
        ret    
loc_3_2A3A:
        inc    edx
        mov    al, 1
        out    dx, al
        add    esp, 0x14
        clc    
        ret    
sub_3_2A43:
        mov    al, 1
        out    dx, al
        sub    dl, 4
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, eax
        in     al, dx
loc_3_2A52:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        cmp    eax, ecx
        je     loc_3_2A52
        in     al, dx
        mov    edi, dword ptr [0x1bd]        ; -> obj4:0x01BD
loc_3_2A63:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, ecx
        cmp    edi, eax
        jae    loc_3_2A63
        add    dl, 5
        in     al, dx
        dec    dx
        mov    al, 0xb
        out    dx, al
        mov    ebx, 0x4d0000
        mov    bx, 0x4203
loc_3_2A81:
        lea    edx, [esi + 5]
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, eax
        mov    edi, dword ptr [0x1c1]        ; -> loc_4_01C1
loc_3_2A92:
        in     al, dx
        test   al, 1
        jne    loc_3_2AA5
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, ecx
        cmp    edi, eax
        jae    loc_3_2A92
        stc    
        ret    
loc_3_2AA5:
        mov    edx, esi
        in     al, dx
        cmp    al, bh
        jne    loc_3_2ABC
        mov    byte ptr [0x178], 4           ; -> obj4:0x0178
        inc    word ptr [4]                  ; -> obj4:0x0004
        jmp    loc_3_2AD3
loc_3_2ABC:
        ror    ebx, 8
        cmp    al, bh
        je     loc_3_2ACC
        rol    ebx, 8
        dec    bl
        jne    loc_3_2A81
        stc    
        ret    
loc_3_2ACC:
        mov    byte ptr [0x178], 3           ; -> obj4:0x0178
loc_3_2AD3:
        clc    
        ret    
sub_3_2AD5:
        movzx  edx, word ptr [esi]
        add    dl, 4
        mov    al, 3
        out    dx, al
        jmp    loc_3_2AE0
loc_3_2AE0:
        jmp    loc_3_2AE2
loc_3_2AE2:
        sub    dl, 3
        mov    al, 1
        out    dx, al
        jmp    loc_3_2AEA
loc_3_2AEA:
        inc    dl
        xor    al, al
        out    dx, al
        add    dl, 2
        jmp    loc_3_2AF4
loc_3_2AF4:
        jmp    loc_3_2AF6
loc_3_2AF6:
        in     al, dx
        or     al, 8
        jmp    loc_3_2AFB
loc_3_2AFB:
        jmp    loc_3_2AFD
loc_3_2AFD:
        out    dx, al
        ret    
loc_3_2AFF:
        movzx  edx, word ptr [esi + 2]
        or     edx, edx
        je     loc_3_2B0E
        add    dl, 4
        in     al, dx
        and    al, 0xf7
        out    dx, al
loc_3_2B0E:
        call   sub_3_2D0B
        xor    eax, eax
        xchg   dword ptr [esi + 8], eax
        cmp    eax, 0xffff
        ja     loc_3_2B2B
        xor    eax, eax
        xchg   dword ptr [0x179], eax        ; -> loc_4_0179
        or     eax, eax
        je     loc_3_2B37
loc_3_2B2B:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VCD.VCD_Free_Port_Windows_Style ; cd2007000e00
loc_3_2B37:
        ret    
sub_3_2B38:
        mov    dword ptr [0x15c], ecx        ; -> loc_4_015C
        jecxz  loc_3_2BBB
        cmp    byte ptr [0x1c8], 0           ; -> obj4:0x01C8
        je     loc_3_2B6B
        movzx  edx, word ptr [ecx + 2]
        cmp    edx, 0x23c
        mov    esi, 0x49c                    ; -> loc_1_049C
        je     loc_3_2B79
        cmp    edx, 0x238
        mov    esi, 0x494                    ; -> loc_1_0494
        je     loc_3_2B79
loc_3_2B67:
        or     word ptr [ecx], 0x20
loc_3_2B6B:
        stc    
        ret    
loc_3_2B6D:
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        or     word ptr [ecx], 0x40
        stc    
        ret    
loc_3_2B79:
        call   sub_3_2C5A
        jb     loc_3_2B67
        movzx  ecx, byte ptr [ecx + 4]
        cmp    cl, 9
        jne    loc_3_2B8B
        mov    cl, 2
loc_3_2B8B:
        mov    al, 1
        shl    eax, cl
        mov    byte ptr [0x1c9], al          ; -> loc_4_01C9
        call   sub_3_2C86
        jb     loc_3_2B6D
        call   sub_3_2C05
        jb     loc_3_2BBA
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        or     word ptr [ecx], 4
        mov    dword ptr [ecx + 0x18], 0x2cee ; -> sub_3_2CEE
        mov    word ptr [ecx + 6], 3
        clc    
loc_3_2BBA:
        ret    
loc_3_2BBB:
        mov    edx, 0x23c
        mov    esi, 0x49c                    ; -> loc_1_049C
        call   sub_3_2BE1
        mov    edx, 0x238
        mov    esi, 0x494                    ; -> loc_1_0494
        call   sub_3_2BE1
        mov    al, byte ptr [0x1c8]          ; -> obj4:0x01C8
        ror    al, 1
        ret    
sub_3_2BE1:
        call   sub_3_2C5A
        jb     loc_3_2C59
        mov    byte ptr [0x1c9], 0xbc        ; -> loc_4_01C9
        call   sub_3_2C86
        jb     loc_3_2C59
        cmp    esi, 0x49c                    ; -> loc_1_049C
        je     sub_3_2C05
        mov    byte ptr [0x697], 4           ; -> obj1:0x0697
sub_3_2C05:
        mov    byte ptr [0x694], 3           ; -> obj1:0x0694
        mov    byte ptr [0x695], bl          ; -> obj1:0x0695
        cmp    bl, 2
        jne    loc_3_2C19
        mov    bl, 9
loc_3_2C19:
        mov    edi, 0x13c                    ; -> obj4:0x013C
        mov    byte ptr [edi], bl
        mov    dword ptr [edi + 4], esi
        push   edx
        mov    edx, dword ptr [0x15c]        ; -> loc_4_015C
        mov    ecx, edx
        mov    word ptr [edi + 2], 4
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edx
        jb     loc_3_2C59
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jecxz  loc_3_2C4D
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0x1c], 0x2d02 ; -> loc_3_2D02
loc_3_2C4D:
        call   sub_3_2CEE
        mov    byte ptr [0x1c8], 0           ; -> obj4:0x01C8
loc_3_2C59:
        ret    
sub_3_2C5A:
        push   edx
        add    edx, 2
        mov    ah, 0xde
        in     al, dx
        cmp    al, ah
        je     loc_3_2C6D
        in     al, dx
        cmp    al, ah
        je     loc_3_2C6D
loc_3_2C6A:
        pop    edx
        stc    
        ret    
loc_3_2C6D:
        in     al, dx
        mov    bl, al
        in     al, dx
        cmp    al, ah
        jne    loc_3_2C6A
        in     al, dx
        cmp    al, bl
        jne    loc_3_2C6A
        cmp    al, 0x11
        ja     loc_3_2C83
        mov    byte ptr [0x7d4], al          ; -> loc_1_07D4
loc_3_2C83:
        pop    edx
        clc    
        ret    
sub_3_2C86:
        cli    
        in     al, 0x21
        push   eax
        mov    al, 0x80
        out    dx, al
        jmp    loc_3_2C8F
loc_3_2C8F:
        jmp    loc_3_2C91
loc_3_2C91:
        jmp    loc_3_2C93
loc_3_2C93:
        mov    al, 7
        out    dx, al
        mov    ah, byte ptr [0x1c9]          ; -> loc_4_01C9
        mov    al, ah
        out    0x21, al
        jmp    loc_3_2CA2
loc_3_2CA2:
        mov    al, 0xa
        out    0x20, al
        jmp    loc_3_2CA8
loc_3_2CA8:
        inc    edx
        movzx  ebx, ah
        mov    ecx, 0xa
loc_3_2CB1:
        mov    al, 0x10
        out    dx, al
        jmp    loc_3_2CB6
loc_3_2CB6:
        in     al, 0x20
        and    al, ah
        xor    al, ah
        and    bl, al
        je     loc_3_2CEB
        mov    al, 0x16
        out    dx, al
        jmp    loc_3_2CC5
loc_3_2CC5:
        in     al, 0x20
        and    al, ah
        and    bl, al
        je     loc_3_2CEB
        loop   loc_3_2CB1
        bsf    eax, ebx
        je     loc_3_2CEB
        btr    ebx, eax
        test   ebx, ebx
        jne    loc_3_2CEB
        mov    ebx, eax
        clc    
loc_3_2CDE:
        mov    al, 0
        out    dx, al
        dec    edx
        mov    al, 0xb
        out    0x20, al
        pop    eax
        out    0x21, al
        sti    
        ret    
loc_3_2CEB:
        stc    
        jmp    loc_3_2CDE
sub_3_2CEE:
        mov    al, 0x80
        out    dx, al
        jmp    loc_3_2CF3
loc_3_2CF3:
        jmp    loc_3_2CF5
loc_3_2CF5:
        jmp    loc_3_2CF7
loc_3_2CF7:
        mov    al, 7
        out    dx, al
        jmp    loc_3_2CFC
loc_3_2CFC:
        inc    edx
        mov    al, 9
        out    dx, al
        clc    
        ret    
loc_3_2D02:
        mov    byte ptr [0x1c8], 0xff        ; -> obj4:0x01C8
        jmp    sub_3_2D0B
sub_3_2D0B:
        xor    eax, eax
        xchg   dword ptr [esi + 0x10], eax
        or     eax, eax
        je     loc_3_2D1A
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_3_2D1A:
        ret    
sub_3_2D1B:
        mov    dword ptr [0x15c], ecx        ; -> loc_4_015C
        jecxz  sub_3_2D4F
        movzx  edx, word ptr [ecx + 2]
        cmp    edx, 0x23c
        jne    loc_3_2D49
        push   ecx
        call   sub_3_2D4F
        pop    ecx
        jb     loc_3_2D43
        mov    word ptr [ecx + 6], 1
        or     word ptr [ecx], 4
        ret    
loc_3_2D43:
        or     word ptr [ecx], 8
        stc    
        ret    
loc_3_2D49:
        or     word ptr [ecx], 0x20
        stc    
        ret    
sub_3_2D4F:
        mov    edx, 0x23c
        call   sub_3_2C5A
        cmc    
        jb     loc_3_2DAC
        call   sub_3_2DAD
        jb     loc_3_2DAC
        mov    byte ptr [0x694], 1           ; -> obj1:0x0694
        mov    byte ptr [0x695], al          ; -> obj1:0x0695
        cmp    al, 2
        jne    loc_3_2D75
        mov    al, 9
loc_3_2D75:
        mov    edi, 0x13c                    ; -> obj4:0x013C
        mov    byte ptr [edi], al
        mov    dword ptr [edi + 4], 0x114    ; -> loc_1_0114
        mov    word ptr [edi + 2], 4
        mov    edx, dword ptr [0x15c]        ; -> loc_4_015C
        mov    ecx, edx
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_2DAC
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jecxz  loc_3_2DAB
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0x1c], 0x2e14 ; -> loc_3_2E14
loc_3_2DAB:
        clc    
loc_3_2DAC:
        ret    
sub_3_2DAD:
        mov    edx, 0x23f
        mov    al, 0x91
        out    dx, al
        jmp    loc_3_2DB7
loc_3_2DB7:
        mov    al, 0xa5
        mov    ah, al
        dec    edx
        dec    edx
        out    dx, al
        jmp    loc_3_2DC0
loc_3_2DC0:
        mov    al, 0x11
        inc    edx
        inc    edx
        out    dx, al
        jmp    loc_3_2DC7
loc_3_2DC7:
        dec    edx
        dec    edx
        in     al, dx
        cmp    al, ah
        jne    loc_3_2E12
        inc    edx
        push   3
        pop    edi
loc_3_2DD2:
        inc    edx
        mov    al, 0x91
        out    dx, al
        jmp    loc_3_2DD8
loc_3_2DD8:
        jmp    loc_3_2DDA
loc_3_2DDA:
        dec    edx
        xor    al, al
        out    dx, al
        jmp    loc_3_2DE0
loc_3_2DE0:
        jmp    loc_3_2DE2
loc_3_2DE2:
        in     al, dx
        jmp    loc_3_2DE5
loc_3_2DE5:
        mov    ah, al
        xor    ebx, ebx
        mov    ecx, 0x2710
loc_3_2DEE:
        in     al, dx
        jmp    loc_3_2DF1
loc_3_2DF1:
        xor    ah, al
        or     bl, ah
        mov    ah, al
        loop   loc_3_2DEE
        mov    al, 0x10
        out    dx, al
        bsf    ecx, ebx
        jne    loc_3_2E06
        dec    edi
        jne    loc_3_2DD2
        jmp    loc_3_2E12
loc_3_2E06:
        xor    al, al
        out    dx, al
        mov    eax, 5
        sub    eax, ecx
        clc    
        ret    
loc_3_2E12:
        stc    
        ret    
loc_3_2E14:
        mov    edx, 0x23f
        mov    al, 0x10
        out    dx, al
        call   sub_3_2D0B
        ret    
sub_3_2E22:
        mov    dword ptr [0x15c], ecx        ; -> loc_4_015C
        jecxz  loc_3_2E36
        cmp    byte ptr [ecx + 4], 0xc
        je     loc_3_2E36
        or     word ptr [ecx], 0x40
        stc    
        ret    
loc_3_2E36:
        pushal 
        xor    ebx, ebx
        cmp    byte ptr [0x795], bl          ; -> loc_1_0795
        jne    loc_3_319B
        mov    byte ptr [0x78b], bl          ; -> loc_1_078B
        mov    byte ptr [0x799], bl          ; -> obj1:0x0799
        mov    word ptr [0x784], bx          ; -> obj1:0x0784
        mov    byte ptr [0x796], 0xff        ; -> obj1:0x0796
        jecxz  loc_3_2E8F
        mov    edx, 0xfe010
        mov    ebx, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        cmp    ebx, 0x48534f54
        jne    loc_3_2E8F
        and    edx, 0xffffff
        cmp    edx, 0x414249
        jne    loc_3_2E8F
        mov    byte ptr [0x78c], 1           ; -> obj1:0x078C
        mov    byte ptr [0x78b], 2           ; -> loc_1_078B
loc_3_2E8F:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ebp, dword ptr [ebx + 8]
        call   sub_3_32F7
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   ebx
        mov    word ptr [ebp + 0x1c], 0xf963
        mov    word ptr [ebp + 0x10], 0x4954
        mov    word ptr [ebp + 0x14], 0
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_2F33
        mov    word ptr [ebp + 0x38], 0
        mov    word ptr [ebp + 0x1c], 0xf95f
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_2F1E
        xor    ebx, ebx
        xor    eax, eax
        mov    bx, word ptr [ebp + 0x38]
        shl    ebx, 4
        mov    ax, word ptr [ebp + 0x10]
        add    ebx, eax
        cmp    ebx, 0x4954
        je     loc_3_2F33
        cmp    word ptr [ebx], 0xa
        jl     loc_3_2F33
        test   word ptr [ebx + 0x14], 0x40
        je     loc_3_2F33
loc_3_2F1E:
        mov    byte ptr [0x78a], 1           ; -> obj1:0x078A
        mov    byte ptr [0x78b], 2           ; -> loc_1_078B
        mov    byte ptr [0x78c], 1           ; -> obj1:0x078C
loc_3_2F33:
        pop    ebx
        cmp    byte ptr [0x78b], 0           ; -> loc_1_078B
        jne    loc_3_2F64
        inc    byte ptr [0x1c5]              ; -> loc_4_01C5
        mov    eax, 0x11
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        dec    byte ptr [0x1c5]              ; -> loc_4_01C5
        test   byte ptr [ebp + 0x1c], 4
        je     loc_3_30B2
loc_3_2F64:
        mov    word ptr [ebp + 0x1c], 0xc205
        mov    byte ptr [ebp + 0x11], 3
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    word ptr [ebp + 0x1c], 0xc201
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    word ptr [ebp + 0x1c], 0xc202
        mov    byte ptr [ebp + 0x11], 0
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    word ptr [ebp + 0x1c], 0xc204
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        movzx  ax, byte ptr [ebp + 0x11]
        mov    word ptr [0x784], ax          ; -> obj1:0x0784
        or     ax, ax
        je     loc_3_2FF6
        mov    word ptr [4], ax              ; -> obj4:0x0004
loc_3_2FF6:
        mov    word ptr [ebp + 0x1c], 0xc203
        mov    byte ptr [ebp + 0x11], 3
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    eax, dword ptr [0x1ce]        ; -> obj4:0x01CE
        or     eax, eax
        jne    loc_3_3030
        mov    esi, 0xee0                    ; -> loc_3_0EE0
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [0x1ce], eax        ; -> obj4:0x01CE
loc_3_3030:
        mov    word ptr [ebp + 0x1c], 0xc207
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    word ptr [ebp + 0x1c], 0xc206
        mov    byte ptr [ebp + 0x11], 1
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    word ptr [ebp + 0x1c], 0xc202
        mov    byte ptr [ebp + 0x11], 2
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    word ptr [ebp + 0x1c], 0xc200
        mov    byte ptr [ebp + 0x11], 1
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_30B2
        mov    byte ptr [0x78b], 1           ; -> loc_1_078B
loc_3_30B2:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        call   sub_3_32B4
        xor    eax, eax
        mov    byte ptr [0x796], al          ; -> obj1:0x0796
        inc    al
        xchg   byte ptr [0x1cd], al          ; -> loc_4_01CD
        or     al, al
        jne    loc_3_30F8
        mov    al, 0x15
        mov    esi, 0x1cc                    ; -> loc_1_01CC
        inc    byte ptr [0x1c5]              ; -> loc_4_01C5
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        dec    byte ptr [0x1c5]              ; -> loc_4_01C5
loc_3_30F8:
        cmp    byte ptr [0x78b], ah          ; -> loc_1_078B
        je     loc_3_31C6
        mov    edi, 0x13c                    ; -> obj4:0x013C
        mov    word ptr [edi], 0xc
        mov    dword ptr [edi + 4], 0x300    ; -> loc_1_0300
        mov    word ptr [edi + 2], 6
        mov    edx, dword ptr [0x15c]        ; -> loc_4_015C
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_31D3
        mov    dword ptr [0x780], eax        ; -> obj1:0x0780
        cmp    byte ptr [0x1cc], 0           ; -> obj4:0x01CC
        jne    loc_3_3181
        mov    eax, 0x15
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x778], ecx        ; -> obj1:0x0778
        mov    dword ptr [0x77c], edx        ; -> obj1:0x077C
        mov    esi, 0x1a8                    ; -> loc_1_01A8
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x15
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        push   0
        push   7
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        mov    dword ptr [0x790], eax        ; -> obj1:0x0790
loc_3_3181:
        or     byte ptr [0x1cc], 1           ; -> obj4:0x01CC
        call   sub_3_0DE0
        mov    byte ptr [0x694], 4           ; -> obj1:0x0694
        mov    byte ptr [0x695], 0xc         ; -> obj1:0x0695
loc_3_319B:
        mov    ecx, dword ptr [0x15c]        ; -> loc_4_015C
        jecxz  loc_3_31BC
        or     word ptr [ecx], 4
        mov    word ptr [ecx + 6], 4
        mov    eax, dword ptr [0x780]        ; -> obj1:0x0780
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0x1c], 0x31df ; -> loc_3_31DF
loc_3_31BC:
        mov    byte ptr [0x795], 0xff        ; -> loc_1_0795
        popal  
        clc    
        ret    
loc_3_31C6:
        mov    ecx, dword ptr [esp + 0x18]
        jecxz  loc_3_31D0
        or     word ptr [ecx], 8
loc_3_31D0:
        popal  
        stc    
        ret    
loc_3_31D3:
        mov    ecx, dword ptr [esp + 0x18]
        jecxz  loc_3_31D0
        or     word ptr [ecx], 0x40
        jmp    loc_3_31D0
loc_3_31DF:
        push   esi
        xor    eax, eax
        mov    al, 0x15
        mov    esi, 0x1cc                    ; -> loc_1_01CC
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        call   sub_3_0DFD
        pop    ebp
        pop    esi
        call   sub_3_2D0B
        xor    eax, eax
        mov    byte ptr [0x797], al          ; -> obj1:0x0797
        mov    byte ptr [0x795], al          ; -> loc_1_0795
        mov    byte ptr [0x1cd], al          ; -> loc_4_01CD
        ret    
sub_3_3217:
        pushal 
        call   sub_3_2D0B
        call   sub_3_32F7
        mov    byte ptr [0x796], 0xff        ; -> obj1:0x0796
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   0xa
        pop    ecx
loc_3_324A:
        mov    word ptr [ebp + 0x1c], 0xc200
        mov    byte ptr [ebp + 0x11], 0
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jae    loc_3_3269
        loop   loc_3_324A
loc_3_3269:
        mov    cl, 0xa
loc_3_326B:
        mov    word ptr [ebp + 0x1c], 0xc200
        mov    byte ptr [ebp + 0x11], 1
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jae    loc_3_328A
        loop   loc_3_326B
loc_3_328A:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        call   sub_3_32B4
        mov    byte ptr [0x796], 0           ; -> obj1:0x0796
        mov    byte ptr [0x795], 0           ; -> loc_1_0795
        popal  
        ret    
sub_3_32B4:
        mov    edx, 0x61
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        cmp    dword ptr [0], 0              ; -> obj4:0x0000
        jne    loc_3_32CE
        VxDCall  VKD.VKD_Enable_Keyboard         ; cd200f800d00
loc_3_32CE:
        mov    edx, 0x60
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x64
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        mov    al, 0xae
        call   sub_3_3348
        ret    
sub_3_32F7:
        mov    edx, 0x61
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        cmp    dword ptr [0], 0              ; -> obj4:0x0000
        jne    loc_3_3311
        VxDCall  VKD.VKD_Disable_Keyboard        ; cd2010800d00
loc_3_3311:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    edx, 0x60
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x64
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    al, 0xad
        call   sub_3_3348
        pop    ebx
        ret    
sub_3_3348:
        push   eax
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        or     eax, eax
        je     loc_3_3359
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
loc_3_3359:
        mov    ecx, 0x10000
loc_3_335E:
        in     al, 0x64
        test   al, 2
        loopne loc_3_335E
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        or     eax, eax
        je     loc_3_3373
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_3_3373:
        pop    ecx
        pop    eax
        out    0x64, al
        ret    
sub_3_3378:
        mov    al, byte ptr [0x694]          ; -> obj1:0x0694
        test   al, al
        je     loc_3_338E
        cmp    al, 4
        jne    loc_3_33DF
        cmp    byte ptr [0x78b], 1           ; -> loc_1_078B
        je     loc_3_33DF
loc_3_338E:
        mov    esi, dword ptr [0x160]        ; -> loc_4_0160
        test   esi, esi
        je     loc_3_33BE
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_3_33BE
loc_3_33A0:
        mov    edx, dword ptr [eax + 0x20]
        test   edx, edx
        je     loc_3_33B6
        push   eax
        push   ebx
        push   esi
        xor    ecx, ecx
        mov    eax, 0x1b
        call   edx
        pop    esi
        pop    ebx
        pop    eax
loc_3_33B6:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_3_33A0
loc_3_33BE:
        cmp    byte ptr [0x694], 0           ; -> obj1:0x0694
        jne    loc_3_33DF
        mov    al, byte ptr [0xa8]           ; -> obj4:0x00A8
        cmp    al, 0xff
        je     loc_3_3403
        cmp    al, 3
        jb     loc_3_33FA
        ja     loc_3_33E0
        xor    ecx, ecx
        call   sub_3_270C
        jb     loc_3_3403
loc_3_33DF:
        ret    
loc_3_33E0:
        cmp    al, 9
        jne    loc_3_33EF
        xor    ecx, ecx
        call   sub_3_2B38
        jae    loc_3_33DF
        jb     loc_3_3403
loc_3_33EF:
        xor    ecx, ecx
        call   sub_3_2E22
        jae    loc_3_33DF
        jb     loc_3_3403
loc_3_33FA:
        xor    ecx, ecx
        call   sub_3_2D1B
        jae    loc_3_33DF
loc_3_3403:
        xor    ecx, ecx
        call   sub_3_2B38
        jae    loc_3_33DF
        xor    ecx, ecx
        call   sub_3_2D1B
        jae    loc_3_33DF
        xor    ecx, ecx
        call   sub_3_2E22
        jae    loc_3_33DF
        xor    ecx, ecx
        call   sub_3_270C
        jmp    loc_3_33DF
        db  033h, 0C9h, 0E8h, 00Ah, 0F7h, 0FFh, 0FFh, 033h, 0C9h, 0E8h, 0E6h, 0F8h, 0FFh, 0FFh, 033h, 0C9h
        db  0E8h, 0E6h, 0F9h, 0FFh, 0FFh, 033h, 0C9h, 0E8h, 0C9h, 0F2h, 0FFh, 0FFh, 0C3h
loc_3_3444:
        mov    eax, 0x21
        test   byte ptr [6], 1               ; -> obj4:0x0006
        je     loc_3_353C
        cmp    edx, 1
        jne    loc_3_353C
        call   sub_3_3545
        jae    loc_3_3470
        mov    eax, 2
        jmp    loc_3_353C
loc_3_3470:
        mov    edi, eax
        mov    dword ptr [edi + 0xc], ebx
        mov    eax, 0x17f                    ; -> loc_4_017F
        mov    ecx, 1
        mov    edx, ebx
        call   sub_3_3585
        jne    loc_3_34F4
        push   edi
        push   ebx
        push   8
        pop    ecx
        cld    
loc_3_348E:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_3_3499
        cmp    al, 0x2e
        je     loc_3_3499
        loop   loc_3_348E
loc_3_3499:
        jecxz  loc_3_34A2
        mov    edi, esi
        dec    edi
        mov    al, 0x20
        rep stosb byte ptr es:[edi], al
loc_3_34A2:
        mov    edi, 0x195                    ; -> obj4:0x0195
        xor    eax, eax
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        pop    ebx
        pop    edi
        jecxz  loc_3_34BB
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [edi + 0x20], eax
        jmp    loc_3_3528
loc_3_34BB:
        mov    esi, edi
        mov    edi, 0x195                    ; -> obj4:0x0195
        mov    al, 0x20
        mov    ecx, 8
        repne scasb al, byte ptr es:[edi]
        jne    loc_3_34CE
        dec    edi
loc_3_34CE:
        mov    dword ptr [edi], 0x6478762e
        mov    byte ptr [edi + 4], 0
        mov    edx, 0x195                    ; -> obj4:0x0195
        xor    eax, eax
        VxDCall  APM.Service_0001h               ; cd2001002700
        mov    edi, esi
        jb     loc_3_3528
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [edi + 0x20], eax
        mov    dword ptr [edi + 0x24], edx
        jmp    loc_3_3528
loc_3_34F4:
        mov    eax, 0x18b                    ; -> loc_4_018B
        mov    ecx, 1
        call   sub_3_3585
        je     loc_3_3516
        mov    eax, 0x18b                    ; -> loc_4_018B
        mov    ecx, 0
        call   sub_3_3585
        jne    loc_3_353E
loc_3_3516:
        push   edi
        push   ebx
        mov    edi, esi
        call   sub_3_0FC3
        pop    ebx
        pop    edi
        or     edx, edx
        je     loc_3_353E
        mov    dword ptr [edi + 0x20], edx
loc_3_3528:
        push   3
        push   edi
        push   0x35af                        ; -> loc_3_35AF
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        xor    eax, eax
loc_3_353C:
        stc    
        ret    
loc_3_353E:
        mov    eax, 0x13
        stc    
        ret    
sub_3_3545:
        mov    esi, dword ptr [0x160]        ; -> loc_4_0160
        or     esi, esi
        jne    loc_3_3567
        mov    eax, 4
        mov    ecx, 0x28
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_3584
        mov    dword ptr [0x160], esi        ; -> loc_4_0160
loc_3_3567:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_3_3584
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 0xa
        cld    
        rep stosd dword ptr es:[edi], eax
        pop    eax
        clc    
loc_3_3584:
        ret    
sub_3_3585:
        push   edx
        mov    dword ptr [0x1ad], 0x31       ; -> obj4:0x01AD
        mov    esi, 0x195                    ; -> obj4:0x0195
        push   ecx
        push   0x1ad                         ; -> obj4:0x01AD
        push   esi
        push   1
        push   eax
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        pop    edx
        test   eax, eax
        ret    
loc_3_35AF:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, 1
        jne    loc_3_365E
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, 0x1b1                    ; -> obj4:0x01B1
        push   2
        push   dword ptr [edi + 0xc]
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_362A
        mov    eax, 0x1ad                    ; -> obj4:0x01AD
        push   0
        push   0
        push   2
        push   dword ptr [esi]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_3603
        mov    eax, dword ptr [0x1ad]        ; -> obj4:0x01AD
        movzx  eax, word ptr [eax + 4]
        mov    word ptr [edi + 2], ax
loc_3_3603:
        mov    eax, 0x1ad                    ; -> obj4:0x01AD
        push   0
        push   0
        push   4
        push   dword ptr [esi]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_362A
        mov    eax, dword ptr [0x1ad]        ; -> obj4:0x01AD
        movzx  eax, word ptr [eax + 2]
        mov    byte ptr [edi + 4], al
loc_3_362A:
        xor    eax, eax
        cmp    dword ptr [edi + 0x20], 0
        je     loc_3_368A
        push   edi
        mov    ecx, edi
        mov    eax, 0x1b
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        inc    byte ptr [0x1c5]              ; -> loc_4_01C5
        call   dword ptr [edi + 0x20]
        dec    byte ptr [0x1c5]              ; -> loc_4_01C5
        pop    edi
        test   byte ptr [edi], 4
        jne    loc_3_3702
        jmp    loc_3_36FB
loc_3_365E:
        cmp    eax, 2
        je     loc_3_3702
        cmp    eax, 4
        jne    loc_3_368A
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_3683
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esi + 0x1c]
        jecxz  loc_3_3683
        push   esi
        call   ecx
        pop    esi
        and    byte ptr [esi], 0xfb
loc_3_3683:
        mov    eax, 0
        jmp    loc_3_3704
loc_3_368A:
        cmp    eax, 3
        je     loc_3_3702
        cmp    eax, 8
        jne    loc_3_369B
        cmp    dword ptr [esp + 0x14], 7
        je     loc_3_36A5
loc_3_369B:
        cmp    eax, 0xb
        mov    eax, 1
        jne    loc_3_3704
loc_3_36A5:
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esi + 0x1c]
        jecxz  loc_3_36B2
        push   esi
        call   ecx
        pop    esi
loc_3_36B2:
        and    byte ptr [esi], 0xfb
        cmp    dword ptr [esi + 0x20], 0
        je     loc_3_36FB
        mov    ecx, esi
        mov    eax, 0x1b
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   esi
        inc    byte ptr [0x1c5]              ; -> loc_4_01C5
        call   dword ptr [esi + 0x20]
        dec    byte ptr [0x1c5]              ; -> loc_4_01C5
        pop    esi
        test   word ptr [esi], 4
        jne    loc_3_3702
        mov    byte ptr [0x694], 0           ; -> obj1:0x0694
        mov    byte ptr [0xa8], 0xff         ; -> obj4:0x00A8
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        call   sub_3_3378
        jmp    loc_3_3702
loc_3_36FB:
        mov    eax, 0x24
        jmp    loc_3_3704
loc_3_3702:
        xor    eax, eax
loc_3_3704:
        pop    ebx
        pop    edi
        pop    esi
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x1d2  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 008h, 010h, 004h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Eh, 000h, 00Ch, 000h, 000h, 000h, 000h, 001h, 000h, 077h, 0FFh, 077h
        db  000h, 000h, 000h, 000h, 050h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 010h, 000h, 000h, 000h
        db  000h, 000h, 001h, 000h, 0FFh, 03Fh, 0FFh, 01Fh, 0FFh, 00Fh, 0FFh, 007h, 0FFh, 003h, 0FFh, 001h
        db  0FFh, 000h, 07Fh, 000h, 03Fh, 000h, 01Fh, 000h, 0FFh, 001h, 0FFh, 010h, 0FFh, 030h, 07Fh, 0F8h
        db  07Fh, 0F8h, 07Fh, 0FCh, 000h, 000h, 000h, 040h, 000h, 060h, 000h, 070h, 000h, 078h, 000h, 07Ch
        db  000h, 07Eh, 000h, 07Fh, 080h, 07Fh, 000h, 07Ch, 000h, 06Ch, 000h, 046h, 000h, 006h, 000h, 003h
        db  000h, 003h, 000h, 000h, 032h, 032h, 020h, 010h, 0FFh, 003h, 042h, 075h, 073h, 006h, 053h, 065h
        db  072h, 069h, 061h, 06Ch, 006h, 049h, 06Eh, 070h, 06Fh, 072h, 074h, 003h, 050h, 053h, 032h, 000h
sub_4_00C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00C8:
        add    byte ptr [eax], al
loc_4_00CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_011A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_011E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0124:
        push   ss
        add    byte ptr [0x80000], al
        or     byte ptr [eax], cl
        or     byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ch, dl                        ; -> sub_3_07D5
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_015C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0160:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0164:
        clc    
        add    eax, dword ptr [eax + eax]
        clc    
        add    al, byte ptr [ebx]
        add    al, ch
        add    eax, dword ptr [eax + eax]
        call   0x477
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0179:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_017D:
        add    byte ptr [eax], al
loc_4_017F:
        dec    ebp
        outsd  dx, dword ptr [esi]
        jne    0x1f6
        inc    esp
        jb     0x1f0
        jbe    0x1ee
        jb     loc_4_018B
loc_4_018B:
        dec    ebp
        outsd  dx, dword ptr [esi]
        jne    0x202
        push   esp
        jns    0x203
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x66], 0x64006f
        add    byte ptr [eax], al
loc_4_01C1:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_4_01C5:
        add    byte ptr [eax], al
        add    bh, bh
loc_4_01C9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_01CD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0xc  R-X
        db  066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 023h
VxD__LTEXT ENDS
