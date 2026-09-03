; QEMMFix -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 210 instructions recovered by recursive descent

Declare_Virtual_Device QEMMFix, 1, 0, QEMMFix_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 000000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x98  R-X
QEMMFix_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_0050
        cmp    eax, 3
        je     loc_1_0019
        clc    
        ret    
loc_1_0019:
        mov    esi, dword ptr [0x8c]         ; -> obj1:0x008C
        or     esi, esi
        je     loc_1_0029
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_1_0029:
        mov    esi, dword ptr [0x90]         ; -> obj1:0x0090
        or     esi, esi
        je     loc_1_0039
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_1_0039:
        clc    
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'QEMMFix '                  ; DDB_Name
        dd  000000000h                  ; DDB_Init_Order
        dd  QEMMFix_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0094:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x238  R-X
loc_2_0000:
        mov    eax, 2
        mov    ecx, 8
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_2_004A
        mov    dword ptr [0x8c], esi         ; -> obj1:0x008C
        mov    eax, 2
        mov    ecx, 0x14
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_2_004A
        mov    dword ptr [0x90], esi         ; -> obj1:0x0090
        mov    eax, 0x150007
        mov    esi, 0x18a                    ; -> loc_2_018A
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_2_004A
        mov    dword ptr [0x94], esi         ; -> loc_1_0094
        clc    
        ret    
loc_2_004A:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0050:
        mov    edi, 0x228                    ; -> loc_2_0228
        xor    esi, esi
        xor    edx, edx
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        mov    dword ptr [0x234], edx        ; -> loc_2_0234
        jb     loc_2_00B0
        jmp    loc_2_0084
loc_2_0069:
        xor    esi, esi
        mov    edx, dword ptr [0x234]        ; -> loc_2_0234
        mov    edi, 0x228                    ; -> loc_2_0228
        VMMCall  VMM.Get_Next_Profile_String     ; cd20b4000100
        mov    dword ptr [0x234], edx        ; -> loc_2_0234
        jb     loc_2_00B0
loc_2_0084:
        mov    esi, dword ptr [0x8c]         ; -> obj1:0x008C
        VMMCall  VMM.List_Allocate               ; cd209d000100
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    edi, eax
        mov    esi, edx
        mov    ecx, 8
        cld    
loc_2_00A0:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_2_00AA
        stosb  byte ptr es:[edi], al
        loop   loc_2_00A0
        jmp    loc_2_0069
loc_2_00AA:
        mov    al, 0x20
        rep stosb byte ptr es:[edi], al
        jmp    loc_2_0069
loc_2_00B0:
        clc    
        ret    
sub_2_00B2:
        push   ebx
        mov    edi, dword ptr [0x230]        ; -> loc_2_0230
        or     edi, edi
        jne    loc_2_0117
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, dword ptr [ebx + 8]
        mov    word ptr [edi + 0x1c], 0x5200
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ebx, word ptr [edi + 0x38]
        shl    ebx, 4
        movzx  edi, word ptr [edi + 0x10]
        add    edi, ebx
        mov    edi, dword ptr [edi + 0x22]
        mov    dword ptr [0x230], edi        ; -> loc_2_0230
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_2_0117:
        pop    ebx
        ret    
sub_2_0119:
        push   ecx
        movzx  ecx, di
        shr    edi, 0x10
        shl    edi, 4
        add    edi, ecx
        test   byte ptr [edi + 5], 0x80
        je     loc_2_013C
        push   esi
        push   edi
        lea    edi, [edi + 0xa]
        mov    ecx, 8
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    edi
        pop    esi
        je     loc_2_013D
loc_2_013C:
        stc    
loc_2_013D:
        pop    ecx
        ret    
sub_2_013F:
        push   edi
loc_2_0140:
        cmp    di, -1
        je     loc_2_0187
        call   sub_2_0119
        jae    loc_2_0151
        mov    edi, dword ptr [edi]
        jmp    loc_2_0140
loc_2_0151:
        mov    eax, dword ptr [edi + 0x12]
        cmp    eax, edi
        je     loc_2_0187
        mov    edi, eax
        call   sub_2_0119
        jb     loc_2_0187
        shr    eax, 0x10
        mov    edx, eax
        dec    eax
        shl    eax, 4
        cmp    byte ptr [eax], 0x4d
        je     loc_2_0174
        cmp    byte ptr [eax], 0x5a
        jne    loc_2_0187
loc_2_0174:
        cmp    dx, word ptr [eax + 1]
        jne    loc_2_0187
        movzx  eax, word ptr [eax + 3]
        shl    eax, 4
        shl    edx, 4
        clc    
        jmp    loc_2_0188
loc_2_0187:
        stc    
loc_2_0188:
        pop    edi
        ret    
loc_2_018A:
        pushal 
        call   sub_2_01E9
        jb     loc_2_01A6
        call   sub_2_00B2
        call   sub_2_013F
        jb     loc_2_01A6
        call   sub_2_01AD
        popal  
        clc    
        ret    
loc_2_01A6:
        popal  
        jmp    loc_1_0094
sub_2_01AD:
        pushal 
        mov    edi, eax
        mov    esi, dword ptr [0x90]         ; -> obj1:0x0090
        VMMCall  VMM.List_Allocate               ; cd209d000100
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 0xc], edi
        mov    dword ptr [eax + 0x10], 0x200
        push   0
        push   eax
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        popal  
        ret    
sub_2_01E9:
        push   esi
        push   edi
        push   ecx
        mov    edi, esi
        mov    esi, dword ptr [0x8c]         ; -> obj1:0x008C
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_2_0216
loc_2_01FC:
        mov    esi, eax
        cld    
        mov    ecx, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_2_0219
        mov    esi, dword ptr [0x8c]         ; -> obj1:0x008C
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_2_01FC
loc_2_0216:
        clc    
        jmp    loc_2_021A
loc_2_0219:
        stc    
loc_2_021A:
        pop    ecx
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0228:
        inc    edi
        dec    esp
        dec    edi
        inc    edx
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
loc_2_0230:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0234:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x15  R-X
        db  03Dh, 00Ah, 003h, 072h, 00Bh, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0C3h
        db  0B8h, 001h, 080h, 0EBh, 0F3h
VxD__LTEXT ENDS
