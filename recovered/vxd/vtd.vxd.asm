; VTD -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 1238 instructions recovered by recursive descent

Declare_Virtual_Device VTD, 4, 0, VTD_Control, 00005h, Undefined_Init_Order
;   DDK version 0400h   init_order 014000000h   flags 00000h
;   service table: obj1:0x470, 11 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x570  R-X
VTD_Get_Version:
        xor    eax, eax
        mov    ah, 4
        xor    ebx, ebx
        inc    ebx
        push   0x36
        pop    ecx
        clc    
        ret    
VTD_Update_System_Clock:
        pushal 
        pushfd 
        call   sub_1_0117
        call   sub_1_001C
        popfd  
        popal  
loc_1_001A:
        ret    
        db  090h
sub_1_001C:
        sub    eax, dword ptr [esi + 0x28]
        mov    ebx, 0x4a9
        cmp    eax, ebx
        jb     loc_1_001A
        mov    ecx, eax
        xor    edx, edx
        idiv   ebx
        sub    ecx, edx
        add    dword ptr [esi + 0x28], ecx
        mov    ecx, eax
        VMMCall  VMM.Update_System_Clock         ; cd20c0800100
VTD_Get_Interrupt_Period:
        push   ecx
        push   edi
        push   0x36
        pop    ecx
        mov    edi, 0x538                    ; -> loc_1_0538
        xor    eax, eax
        cld    
        repe scasb al, byte ptr es:[edi]
        neg    ecx
        lea    eax, [ecx + 0x36]
        pop    edi
        pop    ecx
        ret    
VTD_Begin_Min_Int_Period:
        pushal 
        xor    ecx, ecx
        inc    ecx
loc_1_0056:
        cmp    eax, 0x36
        ja     loc_1_0074
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        add    cl, byte ptr [eax + esi + 0x4b]
        cmp    cl, ch
        je     loc_1_0074
        mov    byte ptr [eax + esi + 0x4b], cl
        call   sub_1_01F9
        popal  
        clc    
        ret    
loc_1_0074:
        popal  
        stc    
        ret    
VTD_End_Min_Int_Period:
        pushal 
        or     ecx, 0xffffffff
        jmp    loc_1_0056
VTD_Disable_Trapping:
        push   edx
        push   0x40
        push   0x43
loc_1_0082:
        pop    edx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        cmp    edx, 0x40
        jne    loc_1_0082
        mov    edx, dword ptr [0x4f4]        ; -> obj1:0x04F4
        inc    dword ptr [edx + ebx + 0xc]
        pop    edx
        ret    
VTD_Enable_Trapping:
        push   edx
        mov    edx, dword ptr [0x4f4]        ; -> obj1:0x04F4
        dec    dword ptr [edx + ebx + 0xc]
        jl     loc_1_00BB
        jne    loc_1_00B9
        push   0x40
        push   0x43
loc_1_00AD:
        pop    edx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        cmp    edx, 0x40
        jne    loc_1_00AD
loc_1_00B9:
        pop    edx
        ret    
loc_1_00BB:
        inc    dword ptr [edx + ebx + 0xc]
        pop    edx
        ret    
VTD_Delay:
        push   eax
        push   edx
        pushfd 
        call   sub_1_00EE
        popfd  
loc_1_00CA:
        mov    edx, eax
        pushfd 
        call   sub_1_00EE
        sub    edx, eax
        jae    loc_1_00E4
        cmp    edx, 0xfffffe00
        jg     loc_1_00E4
        add    edx, dword ptr [0x508]        ; -> obj1:0x0508
loc_1_00E4:
        popfd  
        sub    ecx, edx
        jg     loc_1_00CA
        neg    ecx
        pop    edx
        pop    eax
        ret    
sub_1_00EE:
        xor    eax, eax
        cli    
        out    0x43, al
        jmp    loc_1_00F5
loc_1_00F5:
        jmp    loc_1_00F7
loc_1_00F7:
        in     al, 0x40
        jmp    loc_1_00FB
loc_1_00FB:
        jmp    loc_1_00FD
loc_1_00FD:
        mov    ah, al
        in     al, 0x40
        xchg   al, ah
        ret    
VTD_Get_Real_Time:
        push   esi
        pushfd 
        xor    edx, edx
        call   sub_1_0117
        adc    edx, dword ptr [esi + 4]
        popfd  
        pop    esi
        ret    
sub_1_0113:
        cli    
        mov    ecx, dword ptr [esi + 0x2c]
sub_1_0117:
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        call   sub_1_00EE
        cmp    eax, dword ptr [esi + 0x24]
        ja     loc_1_0131
loc_1_0126:
        mov    dword ptr [esi + 0x24], eax
        sub    eax, dword ptr [esi + 0x18]
        neg    eax
        add    eax, dword ptr [esi]
        ret    
loc_1_0131:
        xor    eax, eax
        jmp    loc_1_0126
VTD_Control:
        cmp    eax, 2
        ja     loc_1_013F
        jmp    loc_2_0000
loc_1_013F:
        cmp    eax, 3
        je     loc_5_004A
        cmp    eax, 7
        je     sub_6_0000
        cmp    eax, 0xd
        je     loc_7_0000
        cmp    eax, 0xe
        je     loc_6_001A
        cmp    eax, 0xb
        je     loc_7_0000
        cmp    eax, 6
        je     loc_5_00BB
        cmp    eax, 0xf
        je     loc_1_01D4
        test   dword ptr [0x180], 0x400000   ; -> obj1:0x0180
        je     loc_1_018F
        cmp    eax, 0x14
        je     loc_3_0000
loc_1_018F:
        clc    
        ret    
sub_1_0191:
        cmp    byte ptr [esi + 0x4a], 0
        jne    loc_1_01AB
        inc    byte ptr [esi + 0x4a]
        push   0
        push   esi
        push   0xff                          ; -> loc_5_00FF
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
loc_1_01AB:
        ret    
loc_1_01AC:
        mov    eax, 0x80000000
        cmp    dword ptr [esp + 4], 4
        jne    loc_1_01D3
        xor    eax, eax
        pushal 
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        mov    ebx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 8]
        add    edi, ebx
        call   sub_1_0221
        call   sub_1_0191
        popal  
loc_1_01D3:
        ret    
loc_1_01D4:
        test   edx, edx
        je     loc_1_01DD
        cmp    edx, 0xd
        jne    loc_1_018F
loc_1_01DD:
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        cmp    dword ptr [esi + 0x10], ebx
        je     loc_1_018F
        mov    dword ptr [esi + 0x10], ebx
        cmp    dword ptr [esi + 0x38], 0
        jne    sub_1_01F9
        push   esi
        mov    edx, esi
        call   sub_1_03F0
        pop    esi
sub_1_01F9:
        call   VTD_Get_Interrupt_Period
        mov    ecx, 0x10000
        cmp    eax, 0x36
        je     loc_1_0216
        imul   eax, eax, 0x4a9
        bsr    eax, eax
        xor    ecx, ecx
        bts    ecx, eax
loc_1_0216:
        mov    ebx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 8]
        mov    dword ptr [esi + 0x20], ecx
        add    edi, ebx
sub_1_0221:
        mov    ecx, dword ptr [edi + 8]
        test   cx, cx
        jne    loc_1_022E
        mov    ecx, dword ptr [esi + 0x20]
        xor    ebx, ebx
loc_1_022E:
        test   eax, eax
        je     loc_1_0237
        cmp    dword ptr [esi + 0x1c], ecx
        je     loc_1_025C
loc_1_0237:
        sti    
        call   sub_1_00EE
        cmp    eax, dword ptr [esi + 0x30]
        jb     loc_1_0237
        add    eax, 0xa
        sub    eax, dword ptr [esi + 0x1c]
        cmp    eax, 0xa
        jbe    loc_1_0237
        mov    al, cl
        out    0x40, al
        jmp    loc_1_0253
loc_1_0253:
        jmp    loc_1_0255
loc_1_0255:
        mov    al, ch
        out    0x40, al
        mov    dword ptr [esi + 0x1c], ecx
loc_1_025C:
        mov    dword ptr [esi + 0x14], ebx
        sti    
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0264:
        cmp    byte ptr [ebp + 0x1d], 0x10
        jb     loc_1_026C
loc_1_026A:
        stc    
        ret    
loc_1_026C:
        cmp    byte ptr [ebp + 0x1d], 1
        ja     loc_1_029B
        jb     loc_1_0285
        mov    eax, dword ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        mov    dword ptr [0x530], eax        ; -> loc_1_0530
        clc    
        ret    
loc_1_0285:
        mov    eax, dword ptr [0x530]        ; -> loc_1_0530
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        mov    byte ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_1_029B:
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jb     loc_1_026A
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    eax, 0x1f4
        mov    esi, 0x2c0                    ; -> loc_1_02C0
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        stc    
        ret    
loc_1_02C0:
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
        mov    eax, eax
loc_1_02C8:
        xchg   esi, eax
        in     al, 0x61
        or     al, 0x80
        jmp    loc_1_02CF
loc_1_02CF:
        jmp    loc_1_02D1
loc_1_02D1:
        out    0x61, al
        xchg   esi, eax
loc_1_02D4:
        push   eax
        mov    esi, edx
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    eax, dword ptr [esi + 0x18]
        mov    edi, eax
        add    eax, dword ptr [esi]
        adc    dword ptr [esi + 4], 0
        mov    dword ptr [esi], eax
        mov    edx, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x18], edx
        mov    dword ptr [esi + 0x24], edx
        call   sub_1_001C
        pop    eax
        mov    ebx, dword ptr [esi + 0x10]
        test   di, di
        je     loc_1_0307
        add    word ptr [esi + 0x40], di
        jae    loc_1_0326
loc_1_0307:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esi + 0x2c], ecx
        mov    ecx, dword ptr [esi + 0x44]
        inc    ecx
        cmp    ecx, 0x1800b0
        jae    loc_1_032C
loc_1_0318:
        mov    dword ptr [esi + 0x44], ecx
        dec    byte ptr [esi + 0x49]
        je     loc_1_0340
loc_1_0320:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_0326:
        cmp    dword ptr [esi + 0x14], ebx
        je     loc_1_0320
        ret    
loc_1_032C:
        inc    byte ptr [esi + 0x48]
        mov    ecx, dword ptr [esi - 0x24]
        inc    word ptr [ecx]
        pushal 
        call   sub_1_0191
        popal  
        xor    ecx, ecx
        jmp    loc_1_0318
loc_1_0340:
        inc    byte ptr [esi + 0x4b]
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        jmp    loc_1_0320
loc_1_034B:
        mov    esi, 0x537                    ; -> obj1:0x0537
        mov    ecx, 0xcf05ea00
        mov    edi, dword ptr [esi - 0xf]
        cmp    ebx, dword ptr [esi - 0x3b]
        jne    loc_1_036D
loc_1_035D:
        mov    byte ptr [esi], cl
loc_1_035F:
        cmp    byte ptr [edi + 1], cl
        jne    loc_1_0365
        ret    
loc_1_0365:
        mov    byte ptr [edi + 1], cl
        mov    byte ptr [edi + 0x1d], ch
        clc    
        ret    
loc_1_036D:
        cmp    byte ptr [esi], cl
        jne    loc_1_035D
        xor    eax, eax
        shr    ecx, 0x10
        mov    al, 0x1c
        VMMCall  VMM.Get_Instanced_V86_Int_Vector ; cd2041010100
        cmp    dword ptr [edi + 0x13], eax
        je     loc_1_035F
        mov    dword ptr [edi + 0x13], eax
        jmp    loc_1_035F
loc_1_0388:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003800300
loc_1_038E:
        jb     loc_1_0398
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0399
loc_1_0398:
        ret    
loc_1_0399:
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        and    cl, 3
        cmp    cl, 2
        jne    loc_1_0398
        VxDCall  VPICD.VPICD_Virtual_EOI         ; cd2015800300
        mov    eax, eax
loc_1_03B0:
        cld    
        mov    esi, 0x530                    ; -> loc_1_0530
        mov    edi, 0x46c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [esi - 1], 0x900
        stc    
        ret    
loc_1_03C5:
        mov    edi, ebx
        xor    ecx, ecx
        mov    esi, dword ptr [edx + 8]
        mov    eax, dword ptr [edx + 0x10]
loc_1_03CF:
        cmp    ebx, eax
        je     loc_1_03E4
        test   byte ptr [esi + ebx], 4
        je     loc_1_03E4
        xchg   ecx, eax
        mov    eax, dword ptr [edx + 0xc]
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        xchg   ecx, eax
loc_1_03E4:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, edi
        jne    loc_1_03CF
        jecxz  loc_1_0402
sub_1_03F0:
        mov    eax, 0x1f4
        mov    esi, 0x3c5                    ; -> loc_1_03C5
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    ecx, esi
loc_1_0402:
        mov    dword ptr [edx + 0x38], ecx
        ret    
loc_1_0406:
        mov    edx, 0x4ec                    ; -> obj1:0x04EC
        xchg   ebx, eax
        mov    edi, dword ptr [edx + 8]
        mov    eax, dword ptr [edx + 0x34]
        VMMCall  VMM.Get_Inst_V86_Int_Vec_Base   ; cd208d010100
        xor    eax, dword ptr [ecx + 0x20]
        add    edi, ebx
        xor    eax, dword ptr [ecx + 0x70]
        jne    loc_1_0426
        and    byte ptr [edi], 0xfb
        ret    
loc_1_0426:
        or     byte ptr [edi], 4
        cmp    ebx, dword ptr [edx + 0x10]
        je     loc_1_0434
        cmp    dword ptr [edx + 0x38], 0
        je     sub_1_03F0
loc_1_0434:
        ret    
VTD_Get_Date_And_Time:
        push   esi
        push   ecx
        mov    esi, 0x530                    ; -> loc_1_0530
        pushfd 
        cli    
        mov    eax, dword ptr [esi]
        xor    edx, edx
        mov    ecx, dword ptr [esi - 4]
        lea    eax, [eax + eax*4]
        lea    ecx, [ecx + ecx*4]
        lea    eax, [eax + eax*4]
        lea    ecx, [ecx + ecx*4]
        shld   edx, eax, 0x11
        add    ecx, ecx
        shl    eax, 0x11
        mov    esi, dword ptr [esi - 0x68]
        add    eax, ecx
        mov    ecx, 0xe90b
        adc    edx, 0
        div    ecx
        mov    dx, word ptr [esi]
        popfd  
        pop    ecx
        pop    esi
        ret    
        dd  VTD_Get_Version          ; fixup
        dd  VTD_Update_System_Clock  ; fixup
        dd  VTD_Get_Interrupt_Period ; fixup
        dd  VTD_Begin_Min_Int_Period ; fixup
        dd  VTD_End_Min_Int_Period   ; fixup
        dd  VTD_Disable_Trapping     ; fixup
        dd  VTD_Enable_Trapping      ; fixup
        dd  VTD_Get_Real_Time        ; fixup
        dd  VTD_Get_Date_And_Time    ; fixup
        dd  VTD_Adjust_VM_Count      ; fixup
        dd  VTD_Delay                ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00005h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VTD     '                  ; DDB_Name
        dd  014000000h                  ; DDB_Init_Order
        dd  VTD_Control              ; DDB_Control_Proc
        dd  VTD_PM_API               ; DDB_V86_API_Proc
        dd  VTD_PM_API               ; DDB_PM_API_Proc
        dd  obj1:0x0470              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_051C:
        adc    al, 0
        add    byte ptr [eax], al
loc_1_0520:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0528:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_052C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0530:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0536:
        add    byte ptr [eax], al
loc_1_0538:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 2  vsize=0x334  R-X
loc_2_0000:
        mov    eax, dword ptr [0x4f4]        ; -> obj1:0x04F4
        push   0x1d4                         ; -> loc_3_01D4
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x4f8]        ; -> obj1:0x04F8
        push   0x1eb                         ; -> obj3:0x01EB
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x4fc]        ; -> obj1:0x04FC
        push   0x203                         ; -> obj3:0x0203
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x500]        ; -> obj1:0x0500
        push   0x219                         ; -> obj3:0x0219
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x504]        ; -> obj1:0x0504
        push   0x238                         ; -> obj3:0x0238
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x508]        ; -> obj1:0x0508
        push   0x253                         ; -> loc_3_0253
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x50c]        ; -> obj1:0x050C
        push   0x26b                         ; -> obj3:0x026B
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x510]        ; -> obj1:0x0510
        push   0x28c                         ; -> obj3:0x028C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x4ec]        ; -> obj1:0x04EC
        push   0x2aa                         ; -> obj3:0x02AA
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x4f0]        ; -> obj1:0x04F0
        push   0x2c4                         ; -> obj3:0x02C4
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x514]        ; -> obj1:0x0514
        push   0x2de                         ; -> obj3:0x02DE
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x518]        ; -> obj1:0x0518
        push   0x2fc                         ; -> loc_3_02FC
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x51c]        ; -> loc_1_051C
        push   0x31c                         ; -> obj3:0x031C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0]            ; -> obj5:0x0000
        push   0x33a                         ; -> obj3:0x033A
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [0x530]        ; -> loc_1_0530
        push   0x35a                         ; -> obj3:0x035A
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        movzx  eax, word ptr [0x52c]         ; -> loc_1_052C
        push   0x372                         ; -> loc_3_0372
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_2_0108:
        push   0x392                         ; -> loc_3_0392
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   ebx
        call   sub_2_013C
        pop    ebx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        je     loc_2_013B
        push   0x396                         ; -> loc_3_0396
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        jne    loc_2_0108
loc_2_013B:
        ret    
sub_2_013C:
        push   0x39d                         ; -> obj3:0x039D
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        cmp    ebx, dword ptr [0x4fc]        ; -> obj1:0x04FC
        jne    loc_2_016D
        push   0x3b0                         ; -> loc_3_03B0
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        cmp    ebx, dword ptr [0x500]        ; -> obj1:0x0500
        jne    loc_2_016D
        push   0x3be                         ; -> loc_3_03BE
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_2_016D:
        push   0x3d2                         ; -> loc_3_03D2
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        add    ebx, dword ptr [0x4f4]        ; -> obj1:0x04F4
        test   byte ptr [ebx], 4
        je     loc_2_018E
        push   0x3d6                         ; -> loc_3_03D6
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_2_018E:
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 0x10000
        jne    loc_2_01A5
        push   0x3e8                         ; -> loc_3_03E8
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_2_01C5
loc_2_01A5:
        mov    eax, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 8]
        push   0x404                         ; -> loc_3_0404
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        cmp    eax, edx
        je     loc_2_01C5
        push   0x418                         ; -> loc_3_0418
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_2_01C5:
        push   0x431                         ; -> obj3:0x0431
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        ret    
        db  000h, 000h, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 043h, 042h
        db  05Fh, 04Fh, 066h, 066h, 073h, 065h, 074h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h
        db  020h, 056h, 054h, 044h, 05Fh, 049h, 052h, 051h, 05Fh, 048h, 061h, 06Eh, 064h, 06Ch, 065h, 00Dh
        db  00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 046h, 06Fh, 063h
        db  075h, 073h, 05Fh, 056h, 04Dh, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h
        db  054h, 044h, 05Fh, 054h, 069h, 06Dh, 065h, 072h, 05Fh, 041h, 074h, 05Fh, 056h, 04Dh, 05Fh, 053h
        db  070h, 065h, 065h, 064h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h
        db  044h, 05Fh, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 05Fh, 043h, 06Fh, 075h, 06Eh, 074h, 00Dh
        db  00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 04Eh, 065h, 078h
        db  074h, 05Fh, 043h, 06Fh, 075h, 06Eh, 074h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h
        db  020h, 056h, 054h, 044h, 05Fh, 052h, 069h, 06Eh, 067h, 030h, 05Fh, 044h, 065h, 073h, 069h, 072h
        db  065h, 064h, 05Fh, 043h, 06Fh, 075h, 06Eh, 074h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h
        db  020h, 020h, 056h, 054h, 044h, 05Fh, 04Ch, 06Fh, 077h, 065h, 073h, 074h, 05Fh, 04Ch, 061h, 074h
        db  063h, 068h, 05Fh, 056h, 061h, 06Ch, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h
        db  056h, 054h, 044h, 05Fh, 04Ch, 06Fh, 05Fh, 04Eh, 075h, 06Dh, 05Fh, 054h, 069h, 063h, 06Bh, 073h
        db  00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 048h, 069h
        db  05Fh, 04Eh, 075h
loc_2_02D4:
        insd   dword ptr es:[edi], dx
        pop    edi
        push   esp
        db  069h
loc_2_02D8:
        arpl   word ptr [ebx + 0x73], bp
        or     eax, 0x4523000a
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [esi + 0x54], dl
        inc    esp
        pop    edi
        dec    esp
        popal  
        jae    0x361
        pop    edi
        push   esp
        db  069h, 06Dh, 065h, 05Fh, 055h
loc_2_02F4:
        jo     0x35a
        popal  
        je     0x35e
        or     eax, 0x4523000a
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [esi + 0x54], dl
        inc    esp
        pop    edi
        dec    esp
        popal  
        jae    0x37f
        pop    edi
        push   esi
        imul   esi, dword ptr [edx + 0x74], 0x746e495f
        pop    edi
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x23000a0d
        inc    ebp
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [esi + 0x54], dl
        inc    esp
        pop    edi
        push   edx
        jo     0x39d
        outsd  dx, dword ptr [esi]
        pop    edi
loc_2_032E:
        dec    ebp
        db  069h, 06Eh, 05Fh
loc_2_0332:
        inc    ebx
        outsd  dx, dword ptr [esi]
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x435  R-X
loc_3_0000:
        cmp    eax, 0
        je     loc_3_0010
        cmp    eax, 1
        je     loc_3_0183
        clc    
        ret    
loc_3_0010:
        mov    dword ptr [0x4fc], ebx        ; -> obj1:0x04FC
        push   0
        push   0x18
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_3_017D
        mov    dword ptr [0x4f4], eax        ; -> obj1:0x04F4
        call   sub_6_0000
        mov    al, 0x34
        out    0x43, al
        jmp    loc_3_003A
loc_3_003A:
        jmp    loc_3_003C
loc_3_003C:
        xor    eax, eax
        out    0x40, al
        jmp    loc_3_0042
loc_3_0042:
        jmp    loc_3_0044
loc_3_0044:
        out    0x40, al
        xor    eax, eax
        jmp    loc_3_004A
loc_3_004A:
        jmp    loc_3_004C
loc_3_004C:
        out    0x43, al
        jmp    loc_3_0050
loc_3_0050:
        jmp    loc_3_0052
loc_3_0052:
        in     al, 0x40
        mov    ecx, eax
        jmp    loc_3_0058
loc_3_0058:
        jmp    loc_3_005A
loc_3_005A:
        in     al, 0x40
        mov    ch, al
loc_3_005E:
        xor    eax, eax
        jmp    loc_3_0062
loc_3_0062:
        jmp    loc_3_0064
loc_3_0064:
        out    0x43, al
        jmp    loc_3_0068
loc_3_0068:
        jmp    loc_3_006A
loc_3_006A:
        in     al, 0x40
        mov    ah, al
        jmp    loc_3_0070
loc_3_0070:
        jmp    loc_3_0072
loc_3_0072:
        in     al, 0x40
        xchg   al, ah
        cmp    eax, ecx
        je     loc_3_005E
        cmp    ah, 0xff
        jne    loc_3_005E
        mov    ecx, eax
        nop    
        nop    
        nop    
        xor    eax, eax
        out    0x43, al
        jmp    loc_3_008A
loc_3_008A:
        jmp    loc_3_008C
loc_3_008C:
        in     al, 0x40
        jmp    loc_3_0090
loc_3_0090:
        jmp    loc_3_0092
loc_3_0092:
        mov    ah, al
        in     al, 0x40
        xchg   al, ah
        sub    cx, ax
        add    ecx, ecx
        cmp    ecx, 0x14
        jbe    loc_3_00A7
        mov    ecx, 0x14
loc_3_00A7:
        mov    dword ptr [0x51c], ecx        ; -> loc_1_051C
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        test   ebx, 0x40000
        je     loc_3_00CF
        mov    dword ptr [0x2d8], 0x2c8      ; -> loc_2_02D8
        in     al, 0x61
        or     al, 0x80
        jmp    loc_3_00CB
loc_3_00CB:
        jmp    loc_3_00CD
loc_3_00CD:
        out    0x61, al
loc_3_00CF:
        or     eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x2f4                    ; -> loc_2_02F4
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    edi, 0x2d4                    ; -> loc_2_02D4
        test   eax, eax
        jne    loc_3_00EB
        mov    dword ptr [edi + 0x14], eax
loc_3_00EB:
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_017D
        mov    dword ptr [0x4f8], eax        ; -> obj1:0x04F8
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        mov    ax, 0xc3f8
        xchg   word ptr [0x34b], ax          ; -> loc_1_034B
        mov    word ptr [0x332], ax          ; -> loc_2_0332
        or     eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x301                    ; -> obj2:0x0301
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        inc    eax
        mov    byte ptr [0x536], al          ; -> loc_1_0536
        mov    edx, 0x43
        mov    esi, 0xc                      ; -> loc_8_000C
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x40
        mov    esi, 0x78                     ; -> loc_8_0078
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    eax, 0x1a
        mov    esi, 0x264                    ; -> loc_1_0264
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, dword ptr [0x46c]
        mov    dword ptr [0x530], eax        ; -> loc_1_0530
        cmp    byte ptr [0x470], 0
        je     loc_3_017B
        mov    eax, dword ptr [0x4c8]        ; -> obj1:0x04C8
        mov    byte ptr [0x470], 0
        inc    word ptr [eax]
loc_3_017B:
        clc    
        ret    
loc_3_017D:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_0183:
        push   0
        push   2
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_3_019B
        mov    dword ptr [0x4c8], eax        ; -> obj1:0x04C8
loc_3_019B:
        mov    eax, 0x1c
        mov    esi, 0x3b0                    ; -> loc_1_03B0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 2
        xor    eax, eax
        mov    al, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_3_01D4:
        mov    eax, dword ptr [0x20]
        mov    dword ptr [0], eax            ; -> obj5:0x0000
        mov    dword ptr [0x313], eax        ; -> obj2:0x0313
        mov    eax, dword ptr [0x70]
        mov    dword ptr [4], eax            ; -> obj5:0x0004
        mov    dword ptr [0x32e], eax        ; -> loc_2_032E
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 1
        xor    eax, eax
        mov    al, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        push   0x104
        push   0x22
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_3_017D
        mov    dword ptr [0x528], eax        ; -> loc_1_0528
        mov    esi, 0x310                    ; -> obj2:0x0310
        mov    ecx, 0x22
        mov    edi, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        shr    eax, 4
        shl    eax, 0x10
        mov    ecx, eax
        mov    cl, 0x1d
loc_3_0253:
        mov    dword ptr [0x20], eax
        mov    dword ptr [0x70], ecx
        mov    ax, word ptr [0x332]          ; -> loc_2_0332
        mov    word ptr [0x34b], ax          ; -> loc_1_034B
        mov    esi, 0x406                    ; -> loc_1_0406
        VMMCall  VMM.Call_When_Task_Switched     ; cd202a000100
        mov    eax, 0x15000e
        mov    esi, 0x16b                    ; -> loc_8_016B
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        xor    eax, eax
        mov    al, 8
        VxDCall  VPICD.VPICD_Get_IRQ_Complete_Status ; cd200b000300
        jb     loc_3_02B2
        test   ch, 1
        jne    loc_3_02A0
        test   ch, 2
        jne    loc_3_02A7
        or     ebx, 0xffffffff
        jmp    loc_3_02A7
loc_3_02A0:
        xor    ebx, ebx
        or     eax, 0x20000
loc_3_02A7:
        or     eax, 0x10000
        VxDCall  VPICD.VPICD_Force_Default_Owner ; cd2010000300
loc_3_02B2:
        VxDCall  VPEND.Service_0000h             ; cd2000002600
        cmp    ah, 4
        jb     loc_3_02CF
        push   0xe0000000
        push   0x1ac                         ; -> loc_1_01AC
        VxDCall  VPEND.Service_000Ah             ; cd200a002600
        pop    ecx
        pop    ecx
loc_3_02CF:
        clc    
        ret    
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h
        dd  loc_1_02D4               ; fixup
        dd  loc_1_034B               ; fixup
        dd  loc_1_0388               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_038E               ; fixup
        db  0F4h, 001h, 000h, 000h
        dd  obj1:0x04EC              ; fixup
        db  054h, 069h, 06Dh, 065h, 072h, 041h, 075h, 074h
loc_3_02FC:
        outsd  dx, dword ptr [esi]
        inc    ebp
        dec    edi
        dec    ecx
        add    byte ptr [ecx + 0x75], al
        je     loc_3_0374
        inc    esp
        popal  
        je     loc_3_036E
        push   ebp
        jo     loc_3_0370
        popal  
        je     loc_3_0374
        add    bl, ch
        add    dl, ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        mov    eax, cs
        cmp    dword ptr cs:[esi], eax
        adc    eax, 0x9c067400
        lcall  0x20b0, 0
        out    0x20, al
        pop    eax
        iretd  
        db  0EAh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_3_036E:
        add    byte ptr [eax], al
loc_3_0370:
        add    byte ptr [eax], al
loc_3_0372:
        add    byte ptr [eax], al
loc_3_0374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0392:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0396:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03BE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x35  R-X
        db  0F7h, 0C3h, 001h, 000h, 075h, 027h, 0B8h, 012h, 016h, 0CDh, 02Fh, 066h, 033h, 0D2h, 03Bh, 0C2h
        db  075h, 016h, 066h, 026h, 08Bh, 047h, 002h, 08Bh, 0D0h, 066h, 0C1h, 0E8h, 010h, 066h, 0C1h, 0E0h
        db  004h, 066h, 003h, 0D0h, 033h, 0C0h, 0EBh, 008h, 0B8h, 002h, 000h, 0EBh, 003h, 0B8h, 001h, 080h
        db  033h, 0DBh, 033h, 0F6h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x17d  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VTD_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0
        je     loc_5_001B
        cmp    ah, 1
        je     loc_5_0025
loc_5_0016:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_5_001B:
        mov    ah, 4
loc_5_001D:
        mov    dword ptr [ebp + 0x1c], eax
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_5_0025:
        cmp    al, 1
        jb     loc_5_0037
        je     loc_5_0042
        cmp    al, 2
        jne    loc_5_0016
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        jmp    loc_5_001D
loc_5_0037:
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        mov    dword ptr [ebp + 0x14], edx
        jmp    loc_5_001D
loc_5_0042:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        jmp    loc_5_001D
loc_5_004A:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 2
        xor    eax, eax
        mov    al, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [0x20]
        mov    dword ptr [0], eax            ; -> obj8:0x0000
        mov    ecx, dword ptr [0x70]
        mov    dword ptr [4], ecx            ; -> obj8:0x0004
        xor    eax, ecx
        mov    dword ptr [0x520], eax        ; -> loc_1_0520
        mov    word ptr [ebp + 0x1c], 0x160e
        mov    byte ptr [ebp + 0x10], 1
        xor    eax, eax
        mov    al, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        clc    
        ret    
loc_5_00BB:
        mov    ecx, dword ptr [0]            ; -> obj5:0x0000
        jecxz  loc_5_00D3
        mov    eax, dword ptr [4]            ; -> obj5:0x0004
        mov    dword ptr [0x20], ecx
        mov    dword ptr [0x70], eax
loc_5_00D3:
        mov    eax, dword ptr [0x530]        ; -> loc_1_0530
        mov    dword ptr [0x46c], eax
        mov    byte ptr [0x470], 0
        mov    al, 0x36
        out    0x43, al
        jmp    loc_5_00EA
loc_5_00EA:
        jmp    loc_5_00EC
loc_5_00EC:
        xor    al, al
        out    0x40, al
        jmp    loc_5_00F2
loc_5_00F2:
        jmp    loc_5_00F4
loc_5_00F4:
        out    0x40, al
        clc    
        ret    
loc_5_00F8:
        inc    ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        and    al, 0
loc_5_00FF:
        pushal 
        mov    ax, 0x3d00
        mov    edx, 0xf8                     ; -> loc_5_00F8
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_5_0174
        mov    ebx, eax
        mov    ax, 0x4400
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_5_016A
        mov    dh, 0
        or     dl, 0x20
        mov    ax, 0x4401
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_5_016A
        mov    esi, dword ptr [0x4c8]        ; -> obj1:0x04C8
loc_5_013C:
        sti    
        mov    di, word ptr [esi]
        push   6
        pop    ecx
        sub    esp, 8
        mov    edx, esp
        mov    ah, 0x3f
        push   esi
        push   edi
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        pop    esi
        pop    ecx
        pop    edx
        jb     loc_5_016A
        cmp    ax, 6
        jne    loc_5_016A
        cli    
        cmp    word ptr [esi], di
        jne    loc_5_013C
        mov    word ptr [esi], cx
        sti    
loc_5_016A:
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_5_0174:
        popal  
        mov    byte ptr [0x536], 0           ; -> loc_1_0536
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x2b  R-X
sub_6_0000:
        add    ebx, dword ptr [0x4f4]        ; -> obj1:0x04F4
        mov    dword ptr [ebx + 4], 0x10000
        mov    dword ptr [ebx + 8], 0x10000
        mov    byte ptr [ebx + 0x17], 6
        clc    
        ret    
loc_6_001A:
        xchg   ebx, eax
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        cmp    eax, ebx
        je     loc_1_01DD
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x15  R-X
loc_7_0000:
        cmp    ebx, dword ptr [0x4fc]        ; -> obj1:0x04FC
        jne    loc_7_0013
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        jmp    loc_1_01DD
loc_7_0013:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x1b1  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_8_0008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_000C:
        cmp    ecx, 4
        jbe    loc_8_0017
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_8_0017:
        jb     loc_8_0076
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        mov    ah, al
        mov    edi, dword ptr [esi + 8]
        and    ah, 0xc2
        add    edi, ebx
        cmp    ah, 0xc2
        je     loc_8_0050
        test   al, 0xc0
        jne    loc_8_0040
        test   al, 0x30
        jne    loc_8_0042
        cmp    dword ptr [edi + 0x10], 0
        jne    loc_8_0041
        jmp    sub_8_0115
loc_8_0040:
        out    dx, al
loc_8_0041:
        ret    
loc_8_0042:
        and    al, 0xe
        cmp    al, 4
        je     loc_8_004C
        cmp    al, 6
        jne    loc_8_0041
loc_8_004C:
        mov    byte ptr [edi + 0x17], al
        ret    
loc_8_0050:
        cmp    dword ptr [edi + 0x10], 0
        jne    loc_8_0041
        test   al, 0x20
        je     loc_8_0061
        push   eax
        call   sub_8_0115
        pop    eax
loc_8_0061:
        test   al, 0x10
        jne    loc_8_0041
        mov    ecx, dword ptr [edi + 0x10]
        mov    al, byte ptr [edi + 0x17]
        or     al, 0x30
        mov    byte ptr [ecx + edi + 0x14], al
        inc    ecx
        mov    dword ptr [edi + 0x10], ecx
        ret    
loc_8_0076:
        in     al, dx
        ret    
loc_8_0078:
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        mov    edi, dword ptr [esi + 8]
        add    edi, ebx
        cmp    ecx, 4
        jbe    loc_8_008D
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_8_008D:
        jb     loc_8_00FD
        xor    dword ptr [edi], 1
        test   byte ptr [edi], 1
        jne    loc_8_00F9
        mov    byte ptr [edi + 5], al
        xor    ecx, ecx
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        movzx  eax, ax
        shl    eax, 4
        mov    eax, dword ptr [eax - 9]
        xor    al, al
        cmp    eax, 0x505700
        jne    loc_8_00BA
        xor    eax, eax
        mov    word ptr [edi + 4], ax
loc_8_00BA:
        movzx  eax, word ptr [edi + 4]
        VxDCall  VTD.VTD_Adjust_VM_Count         ; cd2009000500
        dec    ax
        inc    eax
        mov    dword ptr [edi + 4], eax
        cmp    eax, 0x4a7
        jb     loc_8_00DE
loc_8_00D1:
        mov    dword ptr [edi + 8], eax
        cmp    ebx, dword ptr [esi + 0x10]
        jne    loc_8_00FC
        jmp    sub_1_0221
loc_8_00DE:
        mov    eax, 0x4a7
        bts    dword ptr [edi], 1
        jb     loc_8_00D1
        pushal 
        push   0
        push   ebx
        VxDCall  SHELL._SHELL_SuggestSingleMSDOSMode ; cd2019001700
        add    esp, 8
        popal  
        jmp    loc_8_00D1
loc_8_00F9:
        mov    byte ptr [edi + 4], al
loc_8_00FC:
        ret    
loc_8_00FD:
        mov    ecx, dword ptr [edi + 0x10]
        test   ecx, ecx
        je     loc_8_010D
        dec    ecx
        mov    al, byte ptr [ecx + edi + 0x14]
        mov    dword ptr [edi + 0x10], ecx
        ret    
loc_8_010D:
        call   sub_8_0115
        jmp    loc_8_00FD
VTD_Adjust_VM_Count:
        ret    
sub_8_0115:
        cmp    dword ptr [esi + 0x14], ebx
        jne    loc_8_0134
        call   sub_1_00EE
        sti    
        mov    ecx, eax
        cmp    byte ptr [edi + 0x17], 4
        je     loc_8_0155
        add    ecx, ecx
        sub    ecx, dword ptr [edi + 8]
        jge    loc_8_0155
        add    ecx, dword ptr [edi + 8]
        jmp    loc_8_0155
loc_8_0134:
        call   sub_1_0113
        sti    
        sub    eax, ecx
        cmp    byte ptr [edi + 0x17], 4
        je     loc_8_0144
        add    eax, eax
loc_8_0144:
        mov    ecx, dword ptr [edi + 4]
        test   cx, cx
        je     loc_8_0152
        xor    edx, edx
        idiv   ecx
        mov    eax, edx
loc_8_0152:
        sub    cx, ax
loc_8_0155:
        xchg   cl, ch
        mov    word ptr [edi + 0x14], cx
        mov    dword ptr [edi + 0x10], 2
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_8_0008               ; fixup
loc_8_016B:
        pushal 
        mov    esi, 0x4ec                    ; -> obj1:0x04EC
        mov    eax, dword ptr [0x20]
        cmp    eax, dword ptr [0]            ; -> obj8:0x0000
        jne    loc_8_01AA
        mov    eax, 8
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        jne    loc_8_01AA
        mov    edi, ebx
        add    edi, dword ptr [esi + 8]
        mov    eax, 0x10000
        cmp    dword ptr [edi + 4], eax
        je     loc_8_01AA
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], eax
        cmp    ebx, dword ptr [esi + 0x10]
        jne    loc_8_01AA
        call   sub_1_0221
loc_8_01AA:
        popal  
        jmp    loc_8_0008
VxD__LTEXT ENDS
