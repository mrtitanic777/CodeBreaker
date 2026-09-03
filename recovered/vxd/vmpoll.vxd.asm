; VMPOLL -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 488 instructions recovered by recursive descent

Declare_Virtual_Device VMPOLL, 4, 0, VMPOLL_Control, 00018h, Undefined_Init_Order
;   DDK version 0400h   init_order 064000000h   flags 00000h
;   service table: obj1:0x110, 4 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x18c  R-X
VMPOLL_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 2
        je     loc_2_01C4
        cmp    eax, 7
        je     loc_7_0054
        cmp    eax, 5
        je     loc_6_0000
        test   dword ptr [0x2a], 0x400000    ; -> obj1:0x002A
        je     loc_1_0039
        cmp    eax, 0x14
        je     loc_4_0000
loc_1_0039:
        clc    
        ret    
        db  090h
VMPoll_Check_Idle:
        mov    esi, dword ptr [0x180]        ; -> loc_1_0180
        add    esi, ebx
        jmp    dword ptr [esi]
loc_1_0046:
        xor    eax, eax
        ret    
sub_1_0049:
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        and    byte ptr [esi + 0x10], 0xfe
        test   byte ptr [esi + 0x10], 0x10
        je     loc_1_0065
        test   byte ptr [ebx + 1], 0x40
        je     loc_1_0065
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
loc_1_0065:
        ret    
        db  08Bh, 0C0h
VMPoll_Reset_Detection:
        call   sub_1_0074
        sub    ebx, dword ptr [0x180]        ; -> loc_1_0180
        ret    
sub_1_0074:
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        cmp    dword ptr [ebx], 0x46         ; -> loc_1_0046
        je     loc_1_008C
        mov    dword ptr [ebx], 1            ; -> loc_8_0001
        or     byte ptr [ebx + 0x10], 2
loc_1_008C:
        stc    
        ret    
        db  08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_0174               ; fixup
loc_1_0098:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        test   byte ptr [ebx + 0x10], 1
        jne    loc_1_00C1
        cmp    dword ptr [ebx], 0x46         ; -> loc_1_0046
        je     loc_1_00C1
        and    byte ptr [ebx + 0x10], 0xf7
        mov    dword ptr [ebx], 1            ; -> loc_8_0001
        or     byte ptr [ebx + 0x10], 2
loc_1_00C1:
        pop    ebx
        jmp    loc_1_0174
loc_1_00C8:
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 0x18
        mov    word ptr [ebp + 0x18], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ecx, dword ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        cmp    cx, 1
        cmc    
        ret    
        dd  VMPoll_Get_Version       ; fixup
        dd  VMPoll_Enable_Disable    ; fixup
        dd  VMPoll_Reset_Detection   ; fixup
        dd  VMPoll_Check_Idle        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00018h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VMPOLL  '                  ; DDB_Name
        dd  064000000h                  ; DDB_Init_Order
        dd  VMPOLL_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x0110              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_0174:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_017C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0180:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0184:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0188:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x210  R-X
loc_2_0000:
        pushal 
        mov    edi, dword ptr [0x180]        ; -> loc_1_0180
        push   0x8c                          ; -> loc_4_008C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0xa5                          ; -> obj4:0x00A5
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_2_0023:
        call   sub_2_0057
        push   0xa9                          ; -> obj4:0x00A9
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        push   0xd8                          ; -> loc_4_00D8
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        je     loc_2_0054
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_2_0023
loc_2_0054:
        popal  
        clc    
        ret    
sub_2_0057:
        lea    edx, [edi + ebx]
        mov    eax, dword ptr [edx]
        push   0xdc                          ; -> loc_4_00DC
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [edx + 0xc]
        mov    edx, dword ptr [edx + 0x10]
        push   0x108                         ; -> loc_4_0108
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x14d                         ; -> obj4:0x014D
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        ret    
        db  000h, 000h, 043h, 042h, 020h, 06Fh, 066h, 066h, 073h, 065h, 074h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 03Dh, 020h, 023h, 045h, 044h, 049h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 05Bh
        db  045h, 053h, 043h, 05Dh, 020h, 074h, 06Fh, 020h, 065h, 078h, 069h, 074h, 02Ch, 020h, 061h, 06Eh
        db  079h, 020h, 06Fh, 074h, 068h, 065h, 072h, 020h, 06Bh, 065h, 079h, 020h, 066h, 06Fh, 072h, 020h
        db  06Eh, 065h, 078h, 074h, 020h, 073h, 063h, 072h, 065h, 065h, 06Eh, 03Ah, 020h, 000h, 020h, 00Dh
        db  00Ah, 000h, 056h, 04Dh, 020h, 023h, 045h, 042h, 058h, 02Ch, 020h, 042h, 061h, 073h, 065h, 020h
        db  061h, 064h, 064h, 072h, 065h, 073h, 073h, 020h, 03Dh, 020h, 023h, 045h, 044h, 058h, 02Ch, 020h
        db  050h, 072h, 06Fh, 063h, 020h, 03Dh, 020h, 03Fh, 045h, 041h, 058h, 00Dh, 00Ah, 000h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 04Ch, 061h, 073h, 074h, 020h, 074h
        db  069h, 06Dh, 065h, 020h, 023h, 045h, 041h, 058h, 02Ch, 020h, 04Ch, 061h, 073h, 074h, 020h, 076h
        db  069h, 064h, 020h, 023h, 045h, 043h, 058h, 02Ch, 020h, 054h, 068h, 072h, 065h, 073h, 068h, 020h
        db  023h, 045h, 053h, 049h, 02Ch, 020h, 046h, 06Ch, 061h, 067h, 073h, 020h, 023h, 045h, 044h, 058h
        db  00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_01C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LDATA SEGMENT   ; object 3  vsize=0x9  RW-
        db  0CDh, 020h, 000h, 000h, 00Dh, 000h, 03Dh, 00Ah, 002h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x151  R-X
loc_4_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_008C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 5  vsize=0xa8  R-X
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_0178               ; fixup
loc_5_0008:
        push   dword ptr [0x178]             ; -> loc_1_0178
        jmp    loc_5_001E
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_017C               ; fixup
loc_5_0018:
        push   dword ptr [0x17c]             ; -> loc_1_017C
loc_5_001E:
        cmp    dword ptr [esi - 8], 0x54534e49
        je     loc_5_0049
        cmp    dword ptr [esi - 8], 0x55544553
        je     loc_5_0031
loc_5_0030:
        ret    
loc_5_0031:
        cmp    word ptr [esi - 4], 0x50
        jne    loc_5_0030
loc_5_0038:
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        xor    byte ptr [ebx + 0x10], 4
        sub    ebx, dword ptr [0x180]        ; -> loc_1_0180
        ret    
loc_5_0049:
        cmp    dword ptr [esi - 4], 0x4c4c41
        je     loc_5_0038
        ret    
        db  090h
loc_5_0054:
        mov    ecx, dword ptr [0x188]        ; -> loc_1_0188
        mov    edx, dword ptr [0x184]        ; -> loc_1_0184
        mov    word ptr [ebp + 0x28], cx
        mov    dword ptr [ebp + 0x24], edx
        nop    
loc_5_0068:
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        cmp    dword ptr [ebx], 0x46         ; -> loc_1_0046
        je     loc_5_008E
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0xb
        jbe    loc_5_0090
        cmp    ax, 0x4406
        je     loc_5_009F
loc_5_0084:
        mov    dword ptr [ebx], 1            ; -> loc_8_0001
        or     byte ptr [ebx + 0x10], 2
loc_5_008E:
        stc    
        ret    
loc_5_0090:
        je     loc_5_008E
        cmp    ah, 6
        jne    loc_5_0084
        cmp    byte ptr [ebp + 0x14], 0xff
        jne    loc_5_0084
        jmp    loc_5_008E
loc_5_009F:
        cmp    word ptr [ebp + 0x10], 0
        jne    loc_5_0084
        jmp    loc_5_008E
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x50  R-X
loc_6_0000:
        mov    ecx, dword ptr [0]            ; -> obj3:0x0000
        jecxz  loc_6_000C
        and    byte ptr [ecx + 1], 0xfb
loc_6_000C:
        clc    
        ret    
        db  08Bh, 0C0h
loc_6_0010:
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        cmp    dword ptr [ebx], 0x46         ; -> loc_1_0046
        je     loc_6_002E
        cmp    byte ptr [ebp + 0x1d], 3
        je     loc_6_002E
        mov    dword ptr [ebx], 1            ; -> loc_8_0001
        or     byte ptr [ebx + 0x10], 2
loc_6_002E:
        stc    
        ret    
loc_6_0030:
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        cmp    dword ptr [ebx], 0x46         ; -> loc_1_0046
        je     loc_6_004E
        cmp    byte ptr [ebp + 0x1d], 2
        je     loc_6_004E
        mov    dword ptr [ebx], 1            ; -> loc_8_0001
        or     byte ptr [ebx + 0x10], 2
loc_6_004E:
        stc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0xb7  R-X
VMPoll_Get_Version:
        mov    eax, 0x400
loc_7_0005:
        clc    
        ret    
VMPoll_Enable_Disable:
        test   ebx, ebx
        jne    sub_7_002C
        mov    byte ptr [8], al              ; -> obj3:0x0008
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_7_0016:
        call   sub_7_002C
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_7_0016
        xor    ebx, ebx
        ret    
sub_7_002C:
        push   ebx
        push   eax
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], 1            ; -> loc_8_0001
        or     byte ptr [ebx + 0x10], 2
        pop    eax
        test   al, al
        jne    loc_7_0052
        mov    dword ptr [ebx], 0x46         ; -> loc_1_0046
loc_7_0052:
        pop    ebx
        ret    
loc_7_0054:
        add    ebx, dword ptr [0x180]        ; -> loc_1_0180
        mov    esi, 1                        ; -> loc_8_0001
        cmp    byte ptr [8], 0               ; -> obj3:0x0008
        jne    loc_7_006D
        mov    esi, 0x46                     ; -> loc_1_0046
loc_7_006D:
        mov    dword ptr [ebx], esi
        clc    
        ret    
loc_7_0071:
        movzx  eax, word ptr [edx + 0xc]
        push   eax
        mov    ecx, 0x46                     ; -> loc_1_0046
        or     eax, eax
        je     loc_7_008F
        imul   eax, eax, 0x14
        cdq    
        mov    ecx, 0x32
        div    ecx
        mov    ecx, 1                        ; -> loc_8_0001
loc_7_008F:
        mov    edx, dword ptr [0x180]        ; -> loc_1_0180
        mov    dword ptr [edx + ebx + 0xc], eax
        mov    dword ptr [edx + ebx], ecx
        pop    eax
        and    byte ptr [edx + ebx + 0x10], 0xef
        cmp    eax, 0x21
        jbe    loc_7_00AC
        or     byte ptr [edx + ebx + 0x10], 0x10
loc_7_00AC:
        and    dword ptr [edx + ebx + 4], 0
        VMMCall  VMM.Wake_Up_VM                  ; cd2039800100
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0xaf  R-X
        db  090h
loc_8_0001:
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        mov    ecx, eax
        xchg   dword ptr [esi + 4], eax
        sub    ecx, eax
        cmp    ecx, dword ptr [esi + 0xc]
        jae    loc_8_003E
        test   dl, 0x40
        je     loc_8_003E
        test   byte ptr [esi + 0x10], 2
        jne    loc_8_003E
        cmp    dword ptr [esi], 1            ; -> loc_8_0001
        jne    loc_8_0033
        VxDCall  VDD.VDD_Get_ModTime             ; cd2005000a00
        mov    dword ptr [esi + 8], eax
        dec    dword ptr [esi]
        jmp    loc_8_0048
loc_8_0033:
        VxDCall  VDD.VDD_Get_ModTime             ; cd2005000a00
        cmp    dword ptr [esi + 8], eax
        je     loc_8_004E
loc_8_003E:
        and    byte ptr [esi + 0x10], 0xfd
        mov    dword ptr [esi], 1            ; -> loc_8_0001
loc_8_0048:
        mov    eax, dword ptr [0x170]        ; -> obj1:0x0170
        ret    
loc_8_004E:
        mov    dword ptr [esi], 0x54         ; -> loc_8_0054
loc_8_0054:
        test   dl, 0x40
        je     loc_8_003E
        VxDCall  VDD.VDD_Get_ModTime             ; cd2005000a00
        cmp    dword ptr [esi + 8], eax
        jne    loc_8_003E
        test   byte ptr [esi + 0x10], 4
        jne    loc_8_003E
        bts    dword ptr [esi + 0x10], 3
        jb     loc_8_0087
        or     byte ptr [ebx + 1], 0x80
        mov    ecx, 0x63
        mov    esi, 0x8b                     ; -> loc_8_008B
        xor    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_8_0087:
        xor    eax, eax
        inc    eax
        ret    
loc_8_008B:
        mov    esi, dword ptr [0x180]        ; -> loc_1_0180
        add    esi, ebx
        btr    dword ptr [esi + 0x10], 3
        jae    loc_8_00AE
        test   byte ptr [ebx + 1], 0x80
        je     loc_8_00AE
        or     byte ptr [esi + 0x10], 1
        mov    eax, 0x7fffffff
        call   sub_1_0049
loc_8_00AE:
        ret    
VxD__LTEXT ENDS
