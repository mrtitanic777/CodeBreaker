; DCGVDD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 72 instructions recovered by recursive descent

Declare_Virtual_Device DCGVDD, 3, 0, DCGVDD_Control, 034D2h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xd5  R-X
DCGVDD_Control:
        clc    
        ret    
DCGVDD_PM_API:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    eax, 6
        jae    loc_1_0014
        shl    eax, 2
        jmp    loc_1_001A
loc_1_0014:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_1_001A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0032:
        mov    word ptr [ebp + 0x1c], 0x100
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_1_003E:
        push   es
        push   esi
        push   eax
        push   ebx
        push   ecx
        and    word ptr [ebp + 0x2c], 0xfffe
        mov    ecx, 1
        mov    eax, dword ptr [ebp + 0x18]
        test   eax, 0x100
        je     loc_1_0064
        shl    eax, 8
        or     eax, 0xe0
        mov    ecx, 2
loc_1_0064:
        mov    dword ptr [0], eax            ; -> obj2:0x0050
        lea    esi, [0]                      ; -> obj2:0x0050
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
        jae    loc_1_007F
        mov    dword ptr [ebp + 0x18], ecx
        or     word ptr [ebp + 0x2c], 1
loc_1_007F:
        pop    ecx
        pop    ebx
        pop    eax
        pop    esi
        pop    es
        ret    
loc_1_0085:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, 0xf
        mov    edx, 0xd
        VMMCall  VMM.System_Control              ; cd2093000100
        ret    
loc_1_0099:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_1_009F:
        push   ebx
        VxDCall  SHELL.SHELL_GetVMInfo           ; cd2005001700
        cmp    dx, word ptr [ebp + 0x14]
        pop    ebx
        je     loc_1_00BD
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_009F
        xor    ebx, ebx
loc_1_00BD:
        mov    dword ptr [ebp + 0x14], ebx
        ret    
loc_1_00C1:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [ebp + 0x10], ebx
        ret    
loc_1_00CB:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp + 0x14], edi
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x54  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  034D2h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DCGVDD  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DCGVDD_Control           ; DDB_Control_Proc
        dd  DCGVDD_PM_API            ; DDB_V86_API_Proc
        dd  DCGVDD_PM_API            ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 021h, 04Dh, 069h, 063h
VxD__LTEXT ENDS
