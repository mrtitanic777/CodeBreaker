; VTDAPI -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 769 instructions recovered by recursive descent

Declare_Virtual_Device VTDAPI, 4, 0, VTDAPI_Control, 00442h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2f0  R-X
loc_1_0000:
        mov    esi, dword ptr [0x2d8]        ; -> obj1:0x02D8
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0031
loc_1_0010:
        mov    edx, eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    ebx, dword ptr [edx + 0x1c]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_002D
        mov    cx, word ptr [ebp + 0x18]
        cmp    cx, word ptr [edx + 0x22]
        je     loc_1_0033
loc_1_002D:
        or     eax, eax
        jne    loc_1_0010
loc_1_0031:
        popfd  
        ret    
loc_1_0033:
        mov    ecx, dword ptr [edx + 0x24]
        push   eax
        mov    eax, edx
        push   esi
        push   0x45                          ; -> loc_1_0045
        pushfd 
        jmp    loc_3_01FC
loc_1_0045:
        pop    esi
        pop    eax
        jmp    loc_1_002D
loc_1_0049:
        pushfd 
        cli    
        xor    eax, eax
        mov    dword ptr [edx + 0x14], eax
        test   dword ptr [edx + 0x2c], 0x100
        jne    loc_1_00B8
        test   dword ptr [edx + 0x2c], 1
        je     loc_1_0094
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [edx]
        mov    ebx, dword ptr [edx + 8]
        mov    edi, edx
        xor    edx, edx
        idiv   ebx
        mov    ecx, eax
        mov    eax, ebx
        sub    eax, edx
        mov    edx, edi
        mov    esi, 0x49                     ; -> loc_1_0049
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        mov    dword ptr [edx + 0x14], esi
        mov    eax, ecx
        sub    eax, dword ptr [edx + 4]
        add    dword ptr [edx + 0x28], eax
        mov    dword ptr [edx + 4], ecx
loc_1_0094:
        bts    dword ptr [edx + 0x2c], 9
        jb     loc_1_00B8
        popfd  
        mov    esi, 0xba                     ; -> loc_1_00BA
        mov    ebx, dword ptr [edx + 0x1c]
        mov    eax, 0x1000
        mov    ecx, 1
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edx + 0x18], esi
        ret    
loc_1_00B8:
        popfd  
        ret    
loc_1_00BA:
        pushfd 
        cli    
        xor    eax, eax
        mov    dword ptr [edx + 0x18], eax
        test   dword ptr [edx + 0x2c], 0x100
        jne    loc_1_0228
        test   dword ptr [edx + 0x2c], 0x10
        je     loc_1_015D
        test   dword ptr [edx + 0x2c], 1
        jne    loc_1_00EB
        or     dword ptr [edx + 0x2c], 0x100
loc_1_00EB:
        test   dword ptr [edx + 0x2c], 0x60
        je     loc_1_0119
        mov    eax, dword ptr [edx + 0x20]
        or     eax, eax
        je     loc_1_0119
        push   edx
        test   dword ptr [edx + 0x2c], 0x40
        je     loc_1_010D
        VxDCall  NDIS.Service_000Fh              ; cd200f002a00
        jmp    loc_1_0113
loc_1_010D:
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
loc_1_0113:
        pop    edx
        jmp    loc_1_0228
loc_1_0119:
        mov    edi, dword ptr [edx + 0x30]
        VMMCall  VMM.Validate_Thread_Handle      ; cd200c010100
        jb     loc_1_0151
loc_1_0124:
        push   edx
        push   dword ptr [edx + 0x30]
        push   dword ptr [edx + 0x10]
        push   dword ptr [edx + 0x20]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        pop    edx
        je     loc_1_0151
        test   dword ptr [edx + 0x2c], 0x100
        jne    loc_1_0228
        dec    dword ptr [edx + 0x28]
        jne    loc_1_0124
        jmp    loc_1_0228
loc_1_0151:
        mov    dword ptr [edx + 0x28], 0
        jmp    loc_1_0228
loc_1_015D:
        movzx  edi, word ptr [edx + 0x22]
        lar    ax, di
        jne    loc_1_0238
        test   eax, 0x800
        je     loc_1_0238
        popfd  
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        and    dword ptr [ebp + 0x2c], 0xfffffaff
        xor    eax, eax
        mov    dword ptr [ebp + 8], eax
loc_1_0198:
        mov    dword ptr [ebp + 0x1c], edx
        test   dword ptr [edx + 0x2c], 2
        jne    loc_1_01DB
        push   edx
        movzx  esi, word ptr [ebp + 0x34]
        push   0
        push   esi
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        movzx  esi, word ptr [ebp + 0x30]
        add    esi, eax
        mov    eax, dword ptr [edx + 0x24]
        mov    word ptr [esi - 2], ax
        xor    eax, eax
        mov    word ptr [esi - 4], ax
        mov    ecx, dword ptr [edx + 0x10]
        mov    dword ptr [esi - 8], ecx
        mov    dword ptr [esi - 0xc], eax
        mov    dword ptr [esi - 0x10], eax
        sub    word ptr [ebp + 0x30], 0x10
loc_1_01DB:
        mov    eax, edx
        mov    cx, word ptr [eax + 0x22]
        movzx  edx, word ptr [eax + 0x20]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        mov    edx, eax
        test   dword ptr [edx + 0x2c], 1
        jne    loc_1_01FD
        or     dword ptr [edx + 0x2c], 0x100
loc_1_01FD:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        test   dword ptr [edx + 0x2c], 0x100
        jne    loc_1_0211
        dec    dword ptr [edx + 0x28]
        jne    loc_1_0198
loc_1_0211:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pushfd 
        cli    
loc_1_0228:
        and    dword ptr [edx + 0x2c], 0xfffffdff
        test   dword ptr [edx + 0x2c], 0x100
        je     loc_1_023F
loc_1_0238:
        mov    eax, edx
        jmp    loc_3_020C
loc_1_023F:
        popfd  
        ret    
VTDAPI_Control:
        cmp    eax, 0x23
        je     loc_3_02DD
        cmp    eax, 2
        je     loc_2_0000
        cmp    eax, 0x16
        je     loc_3_03BF
        cmp    eax, 0x17
        je     loc_1_0278
        test   dword ptr [0x267], 0x400000   ; -> obj1:0x0267
        je     loc_1_0276
        cmp    eax, 0x14
        je     loc_5_0000
loc_1_0276:
        clc    
        ret    
loc_1_0278:
        mov    al, 0xc3
        mov    byte ptr [0x49], al           ; -> loc_1_0049
        mov    byte ptr [0xba], al           ; -> loc_1_00BA
        clc    
        ret    
        db  000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00442h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VTDAPI  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VTDAPI_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VTDAPI_PM_API            ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x64  R-X
loc_2_0000:
        push   edx
        mov    edx, dword ptr [0x2e4]        ; -> obj1:0x02E4
        push   0x120                         ; -> obj5:0x0120
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    edx, dword ptr [0x2d8]        ; -> obj1:0x02D8
        push   0x136                         ; -> obj5:0x0136
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        or     edx, edx
        je     0x11b
        push   0x14a                         ; -> obj5:0x014A
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    esi, edx
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     0x11b
        push   0x156                         ; -> obj5:0x0156
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    edx, dword ptr [eax]
        push   0x163                         ; -> obj5:0x0163
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    edx, dword ptr [eax + 4]
        push   0x177                         ; -> obj5:0x0177
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x3f8  R-X
VTDAPI_PM_API:
        VxDCall  VTD.VTD_Get_Version             ; cd2000000500
        or     eax, eax
        je     loc_3_0060
        mov    dword ptr [0x2dc], ebx        ; -> obj1:0x02DC
        mov    dword ptr [0x2e0], ecx        ; -> obj1:0x02E0
        mov    eax, 5
        mov    ecx, 0x34
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_0061
        mov    dword ptr [0x2d8], esi        ; -> obj1:0x02D8
        VMMCall  VMM.Get_System_Time_Address     ; cd2044010100
        push   0
        push   0
        push   0xf0
        push   3
        push   eax
loc_3_0040:
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0061
        mov    word ptr [0x2e8], ax          ; -> obj1:0x02E8
loc_3_0060:
        ret    
loc_3_0061:
        stc    
        jmp    loc_3_0060
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_01FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_020C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_02AA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_02BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_02D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_02DD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_02F7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0369:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0383:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_038B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0393:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03BF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LDATA SEGMENT   ; object 4  vsize=0x50  RW-
        db  00Fh, 0B7h, 045h, 01Ch, 03Dh, 00Bh, 000h, 000h, 000h, 077h, 008h, 0FFh, 014h, 085h
        dd  obj4:0x0000              ; fixup
        db  0C3h, 033h, 0C0h, 066h, 089h, 045h, 01Ch, 0C3h, 066h, 0B8h, 010h, 038h, 0CDh, 020h, 01Ch, 000h
        db  001h, 000h, 097h, 0C7h, 007h, 000h, 004h, 000h, 000h, 0C7h, 047h, 004h, 001h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 0FFh, 0FFh, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 001h, 000h, 0C3h, 00Fh, 0B7h
        db  045h, 018h, 03Bh, 005h
        dd  obj1:0x02E0              ; fixup
        db  073h, 010h, 03Bh, 005h
        dd  obj1:0x02DC              ; fixup
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x26f  R-X
loc_5_0000:
        sbb    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], ch            ; -> obj3:0x0069
        add    byte ptr [eax], al
        xchg   edx, eax
        add    byte ptr [eax], al
        add    byte ptr [eax + eax + 0xb60000], ah ; -> obj3:0x00A4
        add    byte ptr [eax], al
        stosb  byte ptr es:[edi], al
        add    al, byte ptr [eax]
        add    al, dl                        ; -> obj3:0x00D0
        add    byte ptr [eax], al
        add    byte ptr [edx + eax + 0x2d20000], bh ; -> loc_3_02BC
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        test   dword ptr [edx], 0x3120000    ; -> obj3:0x0312
        add    byte ptr [eax], al
        cmp    byte ptr [ebx], al
        add    byte ptr [eax], al
        imul   eax, dword ptr [ebx], 0x3830000 ; -> loc_3_0383
        add    byte ptr [eax], al
        mov    eax, dword ptr [ebx]
        add    byte ptr [eax], al
        xchg   ebx, eax
        add    eax, dword ptr [eax]
        add    byte ptr [edx - 0x51fffffd], ah ; -> obj3:0x03A2
        add    esp, dword ptr [ebx]
        push   esi
        push   esp
        inc    esp
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [esi + 0x69], dl
        jb     0xce
        jne    loc_5_00BD
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x65562809
        jb     loc_5_00DD
        imul   ebp, dword ptr [edi + 0x6e], 0x302e3420
        sub    dword ptr [eax], eax
        add    byte ptr [edx], cl
        push   esi
        push   esp
        inc    esp
        inc    ecx
        push   eax
        dec    ecx
        pop    edi
        inc    esp
        inc    esp
        inc    edx
        add    dword ptr [eax], eax
        add    bh, bh
        adc    byte ptr [eax], al
        inc    dword ptr [ecx]
        add    byte ptr [eax], dh
        add    ah, ah
        add    dword ptr [eax], eax
        add    ah, ah
        add    dword ptr [eax + eax], esi
        push   esi
        push   ebx
        pop    edi
        push   esi
        inc    ebp
        push   edx
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        pop    edi
        dec    ecx
        dec    esi
        inc    esi
        dec    edi
        add    byte ptr [ebp + 0xfeef04], bh
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        mov    dh, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        mov    dh, 3
        add    byte ptr [eax], al
        aas    
        add    byte ptr [eax], al
loc_5_00BD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 1], dh
        add    byte ptr [eax], al
        push   ebx
        je     loc_5_014F
loc_5_00DD:
        imul   ebp, dword ptr [esi + 0x67], 0x656c6946
        dec    ecx
        outsb  dx, byte ptr [esi]
        outsw  dx, word ptr [esi]
        add    byte ptr [eax], al
        popal  
        add    dword ptr [eax], eax
        add    byte ptr [eax], dh
        xor    al, 0x30
        cmp    dword ptr [eax], esi
        xor    al, 0x45
        xor    al, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [esi], dl
        add    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
        jo     loc_5_0164
        outsb  dx, byte ptr [esi]
        jns    0x154
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[ebp + 0x69], cl
        arpl   word ptr [edx + 0x6f], si
        jae    loc_5_0180
        je     0x134
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     loc_5_0188
        outsd  dx, dword ptr [esi]
        jb     0x17c
        je     loc_5_0186
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [0x6c694600], ah
        inc    esp
        jae    loc_5_0191
        jb     0x199
        jo     0x1a6
        imul   ebp, dword ptr [edi + 0x6e], 0x44545600
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [esi + 0x69], dl
        jb     0x1b5
        jne    0x1a4
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x56282020
loc_5_014F:
        jb     0x1c5
        imul   ebp, dword ptr [edi + 0x6e], 0x302e3420
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [ecx], cl
        add    byte ptr [esi + 0x69], al
loc_5_0164:
        insb   byte ptr es:[edi], dx
        push   esi
        jb     0x1dd
        imul   ebp, dword ptr [edi + 0x6e], 0x302e3400
        xor    byte ptr [esi], ch
        cmp    dword ptr [0x30], esi
        add    byte ptr [ebx], bl
        add    byte ptr [edi], al
        add    byte ptr [ecx + 0x6e], cl
loc_5_0180:
        je     loc_5_01E7
        jb     loc_5_01F2
        popal  
        insb   byte ptr es:[edi], dx
loc_5_0186:
        dec    esi
        popal  
loc_5_0188:
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_5_018E:
        push   esi
        push   esp
        inc    esp
loc_5_0191:
        inc    ecx
        push   eax
        dec    ecx
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        add    byte ptr es:[ebp + 0x67], cl
        popal  
loc_5_019E:
        insb   byte ptr es:[edi], dx
        inc    ebx
        outsd  dx, dword ptr [esi]
        jo     loc_5_021C
        jb     0x20e
        push   0x43000074
        outsd  dx, dword ptr [esi]
        jo     0x227
        jb     loc_5_0219
        push   0x20a92074
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
loc_5_01BE:
        je     0x1e0
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     0x234
        and    byte ptr cs:[ecx], dh
        cmp    dword ptr [eax], edi
loc_5_01C9:
        cmp    byte ptr [0x35393931], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        add    byte ptr [ebx], cl
        add    byte ptr [edi + 0x72], cl
        imul   esp, dword ptr [edi + 0x69], 0x466c616e
        imul   ebp, dword ptr [ebp + 0x6e], 0x656d61
loc_5_01E7:
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x54], dl
        inc    esp
        inc    ecx
        push   eax
        dec    ecx
        push   esi
loc_5_01F2:
        pop    eax
        inc    esp
        add    byte ptr [eax], al
        xor    eax, 0x50002500
        jb     loc_5_026C
        jne    loc_5_0263
        je     loc_5_0250
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[ebp + 0x69], cl
        arpl   word ptr [edx + 0x6f], si
        jae    0x27c
loc_5_020D:
        je     loc_5_01BE
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x28b
        scasb  al, byte ptr es:[edi]
loc_5_0219:
        and    byte ptr [edi + 0x70], cl
loc_5_021C:
        jb     0x280
loc_5_021F:
        je     0x28a
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + di + 0x79], dl
        jae    0x29c
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    eax, 0x50000900
        jb     0x2a4
        jne    0x29b
        je     0x290
        jb     0x2b0
        imul   ebp, dword ptr [edi + 0x6e], 0x2e340000
        xor    byte ptr [eax], dh
        cmp    dword ptr cs:[0x30], esi
loc_5_024D:
        add    byte ptr [eax + eax], ah
loc_5_0250:
        add    byte ptr [eax], al
        push   esi
        popal  
        jb     0x29c
        imul   ebp, dword ptr [ebp + 0x49], 0x6f666e
        adc    al, 0
        add    al, 0
        push   esp
loc_5_0263:
        jb     0x2c6
        outsb  dx, byte ptr [esi]
        jae    0x2d4
        popal  
        je     0x2d4
        outsd  dx, dword ptr [esi]
loc_5_026C:
        outsb  dx, byte ptr [esi]
        add    byte ptr [ecx], cl
VxD__LTEXT ENDS
