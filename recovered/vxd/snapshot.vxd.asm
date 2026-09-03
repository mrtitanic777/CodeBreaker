; SNAPVXD -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 99 instructions recovered by recursive descent

Declare_Virtual_Device SNAPVXD, 3, 0, SNAPVXD_Control, 00482h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0017FFFh   flags 00000h
;   service table: obj1:0x1e0, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x238  R-X
        ; ---- dispatch table (51 entries) ----
        dd  loc_1_00DB
        dd  loc_3_0000
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00F8
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
        dd  loc_1_00DB
SNAPVXD_Control:
        cmp    eax, 0x33
        jae    loc_1_00DC
        jmp    dword ptr cs:[eax*4]          ; -> obj1:0x0000
loc_1_00DB:
        clc    
loc_1_00DC:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_00F8:
        cmp    byte ptr [0xe3], 0            ; -> obj1:0x00E3
        je     loc_1_01B1
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_01B1
        push   0x80000003
        VMMCall  VMM._RegFlushKey                ; cd2055010100
        add    esp, 4
        push   0x80000002
        VMMCall  VMM._RegFlushKey                ; cd2055010100
        add    esp, 4
        push   0x80000000
        VMMCall  VMM._RegFlushKey                ; cd2055010100
        add    esp, 4
        push   0x80000001
        VMMCall  VMM._RegFlushKey                ; cd2055010100
        add    esp, 4
        push   ebx
        push   4
        push   -1
        VxDCall  IFSMgr.IFSMgr_DelAllUses        ; cd202e004000
        add    esp, 8
        sub    ecx, ecx
loc_1_0159:
        mov    eax, dword ptr [ecx + 0xe4]   ; -> obj1:0x00E4
        cmp    eax, -1
        je     loc_1_0171
        test   eax, eax
        je     loc_1_016C
        push   ecx
        call   eax
        pop    ecx
loc_1_016C:
        add    ecx, 4
        jmp    loc_1_0159
loc_1_0171:
        pop    ebx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    byte ptr [ebp + 0x1d], 0xd
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VxDCall  REBOOT.Service_0003h            ; cd2003000900
loc_1_01B1:
        clc    
        ret    
SNAPVXD_Service_00:
        mov    eax, 0x300
        clc    
        ret    
SNAPVXD_Service_01:
        sub    ecx, ecx
loc_1_01BC:
        cmp    dword ptr [ecx + 0xe4], 0     ; -> obj1:0x00E4
        je     loc_1_01D3
        cmp    dword ptr [ecx + 0xe4], -1    ; -> obj1:0x00E4
        je     loc_1_01DB
        add    ecx, 4
        jmp    loc_1_01BC
loc_1_01D3:
        mov    dword ptr [ecx + 0xe4], eax   ; -> obj1:0x00E4
        clc    
        ret    
loc_1_01DB:
        stc    
        ret    
SNAPVXD_PM_API:
        ret    
        db  000h, 000h
        dd  SNAPVXD_Service_00       ; fixup
        dd  SNAPVXD_Service_01       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00482h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SNAPVXD '                  ; DDB_Name
        dd  0A0017FFFh                  ; DDB_Init_Order
        dd  SNAPVXD_Control          ; DDB_Control_Proc
        dd  SNAPVXD_PM_API           ; DDB_V86_API_Proc
        dd  SNAPVXD_PM_API           ; DDB_PM_API_Proc
        dd  obj1:0x01E0              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x10  R-X
        db  000h, 000h, 000h, 000h
        dd  obj2:0x0010              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x70  R-X
loc_3_0000:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x58
        mov    byte ptr [ebp + 0x1c], 0xd4
        mov    word ptr [ebp + 0x18], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 2
        je     loc_3_0040
        mov    byte ptr [0xe3], 1            ; -> obj1:0x00E3
loc_3_0040:
        mov    word ptr [ebp + 0x1c], 0x58d2
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VxDCall  VPOWERD.Service_0054h           ; cd2054002800
        clc    
        ret    
        db  0F8h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x15  R-X
        db  0B4h, 058h, 0B0h, 0D4h, 0CDh, 02Fh, 072h, 008h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0C3h
        db  0B8h, 001h, 080h, 0EBh, 0F6h
VxD__LTEXT ENDS
