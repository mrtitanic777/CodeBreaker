; DOSMGR -- reconstructed 32-bit disassembly (VxD / LE format)
; 19 objects, 9566 instructions recovered by recursive descent

Declare_Virtual_Device DOSMGR, 4, 0, DOSMGR_Control, 00015h, Undefined_Init_Order
;   DDK version 0400h   init_order 058000000h   flags 00000h
;   service table: obj1:0x158, 19 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x324  R-X
DOSMGR_Control:
        cmp    eax, 0x1a
        jae    loc_1_000C
        jmp    dword ptr [eax*4 + 0xe]       ; -> obj1:0x000E
loc_1_000C:
        clc    
        ret    
        ; ---- dispatch table (26 entries) ----
        dd  loc_4_0000
        dd  loc_4_015C
        dd  loc_4_0457
        dd  loc_11_0000
        dd  loc_11_000F
        dd  loc_11_0165
        dd  loc_1_0117
        dd  loc_14_0BCC
        dd  loc_14_0000
        dd  sub_14_0C38
        dd  loc_1_000C
        dd  loc_15_001A
        dd  loc_15_0000
        dd  loc_17_0000
        dd  loc_18_0000
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_0145
        dd  loc_14_0E42
        dd  loc_15_0230
        dd  loc_11_01A5
        dd  loc_1_000C
        dd  loc_15_0063
DOSMGR_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
DOSMGR_LocalGlobalReg:
        pushal 
        cmp    dword ptr [0x314], 0          ; -> loc_1_0314
        je     loc_1_00E8
        cmp    byte ptr [0x320], 0           ; -> loc_1_0320
        je     loc_1_00E8
        cmp    dword ptr [0x318], 0          ; -> loc_1_0318
        je     loc_1_00E8
        cmp    edx, edi
        ja     loc_1_00E8
        cmp    edx, 0x100
        jae    loc_1_00E8
        mov    esi, 0x214                    ; -> loc_1_0214
        mov    ecx, dword ptr [0x314]        ; -> loc_1_0314
        jecxz  loc_1_00E8
loc_1_00B2:
        test   byte ptr [esi + 5], 1
        jne    loc_1_00E3
        movzx  eax, word ptr [esi]
        dec    eax
        shr    eax, 8
        movzx  ebx, word ptr [esi + 2]
        inc    ebx
        test   byte ptr [esi + 4], 2
        je     loc_1_00CB
        inc    ebx
loc_1_00CB:
        shr    ebx, 8
        add    ebx, eax
        dec    ebx
        cmp    edi, eax
        jb     loc_1_00E3
        cmp    edx, ebx
        ja     loc_1_00E3
        cmp    edi, ebx
        ja     loc_1_00FC
        cmp    edx, eax
        jb     loc_1_00FC
        jmp    loc_1_00F2
loc_1_00E3:
        add    esi, 0x10
        loop   loc_1_00B2
loc_1_00E8:
        mov    dword ptr [esp + 0x1c], 3
loc_1_00F0:
        popal  
        ret    
loc_1_00F2:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_1_00F0
loc_1_00FC:
        mov    dword ptr [esp + 0x1c], 2
        jmp    loc_1_00F0
DOSMGR_Get_DOS_Crit_Status:
        push   eax
        mov    eax, dword ptr [0x1f8]        ; -> loc_1_01F8
        movzx  eax, byte ptr [eax]
        or     eax, dword ptr [0x1fc]        ; -> loc_1_01FC
        pop    eax
        ret    
loc_1_0117:
        call   sub_11_0AB8
        mov    eax, dword ptr [8]            ; -> obj6:0x0008
        mov    dword ptr [0x8c], eax
        mov    eax, dword ptr [0x184]        ; -> obj6:0x0184
        test   eax, eax
        je     loc_1_0135
        xor    edx, edx
        dec    dx
        mov    dword ptr [eax], edx
loc_1_0135:
        mov    edi, dword ptr [0x58]         ; -> obj6:0x0058
        xor    eax, eax
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [edi + 0x10], eax
        clc    
        ret    
loc_1_0145:
        test   dword ptr [0x14b], 0x400000   ; -> obj1:0x014B
        je     loc_1_0156
        jmp    loc_7_0000
loc_1_0156:
        ret    
        db  000h
        dd  DOSMGR_Get_Version       ; fixup
        dd  _DOSMGR_Set_Exec_VM_Data ; fixup
        dd  DOSMGR_Copy_VM_Drive_State ; fixup
        dd  _DOSMGR_Exec_VM          ; fixup
        dd  DOSMGR_Get_IndosPtr      ; fixup
        dd  DOSMGR_Add_Device        ; fixup
        dd  DOSMGR_Remove_Device     ; fixup
        dd  DOSMGR_Instance_Device   ; fixup
        dd  DOSMGR_Get_DOS_Crit_Status ; fixup
        dd  DOSMGR_Enable_Indos_Polling ; fixup
        dd  DOSMGR_BackFill_Allowed  ; fixup
        dd  DOSMGR_LocalGlobalReg    ; fixup
        dd  DOSMGR_Init_UMB_Area     ; fixup
        dd  DOSMGR_Begin_V86_App     ; fixup
        dd  DOSMGR_End_V86_App       ; fixup
        dd  DOSMGR_Alloc_Local_Sys_VM_Mem ; fixup
        dd  DOSMGR_Grow_CDSs         ; fixup
        dd  DOSMGR_Translate_Server_DOS_Call ; fixup
        dd  DOSMGR_MMGR_PSP_Change_Notifier ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00015h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DOSMGR  '                  ; DDB_Name
        dd  058000000h                  ; DDB_Init_Order
        dd  DOSMGR_Control           ; DDB_Control_Proc
        dd  DOSMGR_V86_API           ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x0158              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_01F8:
        or     al, 2
        add    byte ptr [eax], al
loc_1_01FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0200:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0204:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0208:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_020C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0214:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_031C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0320:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x66  R-X
        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        and    eax, 0x5200
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    0x94
        jo     0x91
        outsd  dx, dword ptr [esi]
        jb     0x98
        and    byte ptr fs:[ebp + 0x53], cl
        sub    eax, 0x20534f44
        jbe    0x95
        jb     0xa5
        imul   ebp, dword ptr [edi + 0x6e], 0xa0d2e
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    0xb9
        jo     0xb6
        outsd  dx, dword ptr [esi]
        jb     0xbd
        and    byte ptr fs:[ebx + 0x6f], dh
        je     0xc8
        popal  
        jb     0xb9
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     0xc9
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        db  02Eh, 00Dh, 00Ah, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x71  R-X
        db  060h, 068h
        dd  loc_7_12B0               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x12B4              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x12D2              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  loc_7_12E7               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  loc_7_12FC               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x1311              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x1326              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  loc_7_133B               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x1350              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x1365              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj7:0x1384              ; fixup
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x2138  R-X
loc_4_0000:
        inc    edx
        push   edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_4_004C
        outsb  dx, byte ptr [esi]
        je     loc_4_007D
        and    byte ptr [eax], ah
        add    byte ptr [ecx + eax], cl
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        inc    edx
        push   eax
        inc    edx
        push   esp
        outsd  dx, dword ptr [esi]
        je     loc_4_007C
        insb   byte ptr es:[edi], dx
        push   ebx
        arpl   word ptr gs:[eax], sp
        and    byte ptr [eax], al
        push   cs
        add    dword ptr [edx + 0x64], esp
        jae    0x87
        inc    edx
        push   eax
        inc    edx
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x44], 0x20637365
        add    byte ptr [eax], dl
        add    byte ptr [edx + 0x64], ah
        jae    0x9c
        inc    edx
        push   eax
        inc    edx
        push   ebx
        arpl   word ptr gs:[eax + 0x65], dx
        jb     0x8e
        inc    ecx
        push   esp
        and    byte ptr [eax], al
loc_4_004C:
        adc    dword ptr [ecx], eax
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        inc    edx
        push   eax
        inc    edx
        push   ebx
        arpl   word ptr gs:[eax + 0x65], dx
        jb     loc_4_00B1
        jb     0xca
        and    byte ptr [eax], al
        adc    eax, dword ptr [ecx]
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        inc    edx
        push   eax
        inc    edx
        dec    eax
        popal  
        jae    loc_4_0091
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [0x73646201], dl
        pop    edi
loc_4_007C:
        inc    edx
loc_4_007D:
        push   eax
        inc    edx
        dec    eax
        imul   esp, dword ptr [esp + 0x65], 0x6365536e
        and    byte ptr [eax], al
        pop    ss
        add    ah, byte ptr [edx + 0x64]
        jae    0xf0
loc_4_0091:
        inc    edx
        push   eax
        inc    edx
        inc    edx
        imul   esp, dword ptr [edi + 0x54], 0x6553746f
        arpl   word ptr [eax], sp
        add    byte ptr [ebx], bl
        add    ah, byte ptr [edx + 0x64]
        jae    loc_4_0105
        popaw  
        je     0x11d
        imul   edi, dword ptr [edx + 0x20], 0x20202020
loc_4_00B1:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        cmp    eax, dword ptr [eax]
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        outsd  dx, dword ptr [esi]
        jo     0x121
        outsb  dx, byte ptr [esi]
        je     0xe1
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [ecx + eax], bh
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        outsw  dx, word ptr [esi]
        jb     loc_4_0141
        popaw  
        arpl   word ptr [edi + ebp*2 + 0x72], si
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [esi], bh
        add    byte ptr [edx + 0x64], ah
        jae    loc_4_0144
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x10b
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edi], bh
        add    dword ptr [edx + 0x64], esp
        jae    0x159
        arpl   word ptr [ebx + 0x79], sp
        insb   byte ptr es:[edi], dx
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_0105:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        inc    ecx
        add    dword ptr [edx + 0x64], esp
        jae    0x16e
        push   edx
        inc    edx
        push   eax
        inc    edx
        jns    0x18a
        jae    loc_4_016C
        arpl   word ptr gs:[eax], sp
        and    byte ptr [eax], al
        inc    ebx
        add    dword ptr [edx + 0x64], esp
        jae    0x183
        push   edx
        inc    edx
        push   eax
        push   ebx
        arpl   word ptr gs:[eax + 0x65], dx
        jb     0x172
        insb   byte ptr es:[edi], dx
        jne    0x152
        add    byte ptr [ebp], al
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        push   edx
        inc    edx
        push   eax
        push   edx
        jae    0x194
loc_4_0141:
        arpl   word ptr gs:[eax], sp
loc_4_0144:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        inc    esi
        add    dword ptr [edx + 0x64], esp
        jae    loc_4_01AD
        push   edx
        inc    edx
        push   eax
        dec    esi
        jne    loc_4_01C2
        inc    esi
        inc    ecx
        push   esp
        jae    loc_4_017A
        and    byte ptr [eax], ah
loc_4_015C:
        add    byte ptr [eax], cl
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        push   edx
        inc    edx
        push   eax
        push   edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_4_01B1
loc_4_016C:
        outsb  dx, byte ptr [esi]
        je     loc_4_01E2
        and    byte ptr [eax], ah
        add    byte ptr [ecx + 1], cl
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        push   edx
        inc    edx
loc_4_017A:
        push   eax
        push   esp
        outsd  dx, dword ptr [esi]
        je     loc_4_01E1
        insb   byte ptr es:[edi], dx
        push   ebx
        arpl   word ptr gs:[eax], sp
        and    byte ptr [eax], al
        dec    ebx
        add    dword ptr [edx + 0x64], esp
        jae    0x1ec
        push   edx
        inc    edx
        push   eax
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x44], 0x20637365
        add    byte ptr [ebp], cl
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        push   edx
        inc    edx
        push   eax
        push   ebx
        arpl   word ptr gs:[eax + 0x65], dx
        jb     0x1f3
loc_4_01AD:
        inc    ecx
        push   esp
        and    byte ptr [eax], al
loc_4_01B1:
        dec    esi
        add    dword ptr [edx + 0x64], esp
        jae    0x216
        push   edx
        inc    edx
        push   eax
        push   ebx
        arpl   word ptr gs:[eax + 0x65], dx
        jb     0x216
loc_4_01C2:
        jb     0x22f
        and    byte ptr [eax], al
        push   eax
        add    dword ptr [edx + 0x64], esp
        jae    0x22b
        push   edx
        inc    edx
        push   eax
        dec    eax
        popal  
        jae    loc_4_01F6
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edx + 1], dl
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
loc_4_01E1:
        push   edx
loc_4_01E2:
        inc    edx
        push   eax
        dec    eax
        imul   esp, dword ptr [esp + 0x65], 0x6365536e
        and    byte ptr [eax], al
        push   esp
        add    ah, byte ptr [edx + 0x64]
        jae    loc_4_0255
loc_4_01F6:
        push   edx
        inc    edx
        push   eax
        inc    edx
        imul   esp, dword ptr [edi + 0x54], 0x6553746f
        arpl   word ptr [eax], sp
        add    byte ptr [eax + 2], bl
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        push   edx
        inc    edx
        push   eax
        push   edx
        jae    loc_4_0278
        jb     0x28b
        and    byte ptr fs:[eax], ah
        add    byte ptr [esp + eax + 0x62], bl
        jae    0x27f
        je     0x294
        popal  
        arpl   word ptr [ebx + 0x20], bp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], bh
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        je     0x2a0
        insd   dword ptr es:[edi], dx
        pop    edi
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [ecx + 1], bh
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        je     0x2b5
        insd   dword ptr es:[edi], dx
        pop    edi
        push   0x20202069
        and    byte ptr [eax], ah
loc_4_0255:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        jnp    0x25c
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        jbe    0x2d0
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x20], 0x20202020
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        jge    0x273
        bound  esp, qword ptr [ebx + esi*2 + 0x5f]
        jbe    loc_4_02E5
        insb   byte ptr es:[edi], dx
        pop    edi
loc_4_0278:
        jae    0x2df
        jb     loc_4_02E5
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [ecx + 0x73646202], cl
        pop    edi
        imul   bp, word ptr [ebp + 0x73], 0x7379
        pop    edi
        imul   esp, dword ptr [eax + 0x20], 0x8d002020
        add    esp, dword ptr [eax]
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
        add    eax, 0xa0d2000
        add    byte ptr [ebx + 0x65], dl
        insb   byte ptr es:[edi], dx
        arpl   word ptr gs:[eax + 0x6e], si
        ja     0x2df
        inc    esp
        dec    edi
        push   ebx
        dec    ebp
        inc    edi
        push   edx
        and    byte ptr [esi + 0x75], ah
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        cmp    cl, byte ptr [0x2020000a]
        pop    ebx
        xor    dword ptr [ebp + 0x20], ebx
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x2079616c
        push   ebx
        inc    esi
        push   esp
        jae    0x2f2
loc_4_02E5:
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        xor    bl, byte ptr [ebp + 0x20]
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x2079616c
        inc    esp
        push   eax
        inc    edx
        jae    0x307
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        xor    ebx, dword ptr [ebp + 0x20]
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x2079616c
        dec    ebp
        inc    ebx
        inc    edx
        jae    loc_4_031C
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        xor    al, 0x5d
        and    byte ptr [ecx + ebp*2 + 0x73], al
        jo     0x388
loc_4_031C:
        popal  
        jns    0x33f
        inc    ebx
        inc    esp
        push   ebx
        jae    loc_4_0331
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        xor    eax, 0x6944205d
        jae    loc_4_03A0
        insb   byte ptr es:[edi], dx
loc_4_0331:
        popal  
        jns    loc_4_0354
        push   eax
        inc    esp
        inc    edx
        jae    loc_4_0346
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        pop    ebp
        and    byte ptr [ecx + ebp*2 + 0x73], al
        jo     0x3b2
loc_4_0346:
        popal  
        jns    0x369
        inc    esp
        inc    ebp
        push   esi
        jae    loc_4_035B
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        aaa    
loc_4_0354:
        pop    ebp
        and    byte ptr [ecx + ebp*2 + 0x73], al
        jo     loc_4_03C7
loc_4_035B:
        popal  
        jns    loc_4_037E
        inc    edx
        inc    esp
        push   ebx
        jae    0x370
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        pop    ebx
        cmp    byte ptr [ebp + 0x20], bl
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x2079616c
        dec    ecx
        dec    esi
        push   esp
        and    byte ptr [edx], dh
        xor    dword ptr [eax + 0x20], ebp
        jae    0x3f1
        popal  
loc_4_037E:
        je     loc_4_03F5
        jae    loc_4_038F
        or     al, byte ptr [eax]
        aas    
        and    byte ptr [eax], al
        and    byte ptr [0x2a2a000a], cl
        sub    ch, byte ptr [edx]
loc_4_038F:
        and    byte ptr [ecx + 0x72], al
        outsb  dx, byte ptr gs:[esi]
        popal  
        and    byte ptr [ebx + 0x68], ah
        popal  
        imul   ebp, dword ptr [esi + 0x20], 0x616d6164
loc_4_03A0:
        and    byte ptr fs:[bp + si], ch
        sub    ch, byte ptr [edx]
        sub    cl, byte ptr [0x7250000a]
        jae    0x423
        and    byte ptr [ecx + 0x20], ah
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x428
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x428
        outsb  dx, byte ptr [esi]
        jne    loc_4_0427
        add    byte ptr cs:[eax], ah
loc_4_03C7:
        or     eax, 0x4f44000a
        push   ebx
        dec    ebp
        inc    edi
        push   edx
        pop    edi
        inc    esp
        bound  esi, qword ptr gs:[ebp + 0x67]
        pop    edi
        inc    esi
        imul   esp, dword ptr [ebp + 0x6c], 0x61203a64
        jb     0x453
        popal  
        jns    loc_4_0457
        and    byte ptr [esi + 0x6f], ch
        je     0x409
        jae    loc_4_0460
        jo     0x45d
        outsd  dx, dword ptr [esi]
        jb     loc_4_0464
        and    byte ptr fs:[ecx + 0x74], ah
loc_4_03F5:
        and    byte ptr [ebp + 0x6e], ah
        and    byte ptr fs:[edi + 0x66], ch
        and    byte ptr [ebx + 0x74], dh
        jb     loc_4_0476
        arpl   word ptr [ebp + esi*2 + 0x72], si
        or     eax, 0xa
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [esi + 0x61], cl
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        or     eax, 0x2a2a000a
        sub    ch, byte ptr [edx]
loc_4_0427:
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        and    byte ptr [edi + 0x41], dl
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        and    byte ptr [ecx + eax*2 + 0x4e], al
        inc    edi
        inc    ebp
        push   edx
        and    byte ptr [edx], ch
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    cl, byte ptr [0x202a000a]
        push   edi
        db  069h, 074h
loc_4_0457:
        push   0x6c756d20
        je     0x4c7
        jo     0x4cc
loc_4_0460:
        and    byte ptr gs:[esi + 0x4d], dl
loc_4_0464:
        jae    0x486
        je     loc_4_04D0
        imul   esi, dword ptr [ebx + 0x20], 0x76207369
        jb     loc_4_04EB
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
loc_4_0476:
        outsb  dx, byte ptr [esi]
        and    byte ptr gs:[edi + ebp*2 + 0x20], dh
        and    byte ptr [edx], ch
        or     eax, 0x202a000a
        jo     0x4e6
        and    byte ptr gs:[bp + 0x61], ah
        jne    0x4f8
        je     loc_4_04F7
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x6e], ah
        and    byte ptr fs:[ebx + 0x72], ah
        popal  
        jae    0x502
        imul   ebp, dword ptr [esi + 0x67], 0x65687420
        and    byte ptr [ebx + 0x79], dh
        jae    loc_4_051A
        insd   dword ptr es:[edi], dx
        and    byte ptr cs:[eax], ah
        and    byte ptr [edx], ch
        or     eax, 0x202a000a
        inc    ecx
        jb     loc_4_051A
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_4_04DA
        jae    loc_4_0531
        jb     0x523
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_4_04E3
        ja     0x526
        outsb  dx, byte ptr [esi]
        je     0x4e8
        je     0x539
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x539
loc_4_04D0:
        outsb  dx, byte ptr [esi]
        jne    loc_4_0538
        aas    
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_04DA:
        and    byte ptr [edx], ch
        or     eax, 0x2a2a000a
        sub    ch, byte ptr [edx]
loc_4_04E3:
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
loc_4_04EB:
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
loc_4_04F7:
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        or     eax, 0x455b000a
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 0x20], dh
        js     loc_4_0583
loc_4_051A:
        je     loc_4_0548
        and    byte ptr [ecx + 0x6e], ah
        jns    0x541
        outsd  dx, dword ptr [esi]
        je     loc_4_058C
        jb     0x547
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     loc_4_059C
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
loc_4_0531:
        je     loc_4_059C
        outsb  dx, byte ptr [esi]
        jne    loc_4_059B
        cmp    ah, byte ptr [eax]
loc_4_0538:
        add    byte ptr [eax], ah
        or     eax, 0x4653000a
        push   esp
        and    byte ptr [edx + 0x61], ah
        jae    0x5aa
        and    byte ptr [ecx + 0x64], ah
loc_4_0548:
        jb     0x5b0
        jae    0x5c0
        and    byte ptr [ecx + 0x73], ch
        outsb  dx, byte ptr [esi]
        daa    
        je     0x574
        jae    0x5bb
        je     0x578
        jns    loc_4_05BF
        je     loc_4_055C
loc_4_055C:
        push   ebx
        inc    esi
        push   esp
        and    byte ptr [ecx + 0x74], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 0x2020000a
        and    byte ptr [eax], ah
        push   edx
        inc    bx
        outsb  dx, byte ptr [esi]
        je     loc_4_0595
        cmp    eax, 0x49532320
        or     eax, 0x2020000a
        and    byte ptr [eax], ah
        inc    esi
        dec    esp
loc_4_0583:
        inc    edi
        push   ebx
        and    byte ptr [0x454e0020], ch
        push   esp
loc_4_058C:
        and    byte ptr [eax], al
        inc    esp
        inc    ebp
        push   esi
        and    byte ptr [eax], al
        inc    ebx
        dec    ecx
loc_4_0595:
        dec    esi
        and    byte ptr [eax], al
        inc    ebx
        dec    edi
        push   ebp
loc_4_059B:
        push   esp
loc_4_059C:
        and    byte ptr [eax], al
        and    byte ptr [0x2020000a], cl
        and    byte ptr [eax], ah
        push   ebx
        inc    esi
        push   esp
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [esi + 0x72], ah
        or     eax, 0xd20000a
        or     al, byte ptr [eax]
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
loc_4_05BF:
        and    byte ptr [edi + ecx*2 + 0x53], al
        pop    edi
        inc    ebx
        jb     loc_4_0630
        je     0x628
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0x63b
        je     0x5ef
        cmp    eax, 0x41452320
        pop    eax
        sub    al, 0x20
        push   esp
        jb     loc_4_0648
        and    byte ptr [ecx + 0x64], ah
        jb     loc_4_0601
        aas    
        inc    ebp
        inc    esp
        pop    eax
        or     eax, 0x2020000a
        push   eax
        xor    dword ptr [eax], esi
        push   ebx
        je     0x651
        je     0x657
        and    byte ptr [ebx], ah
        inc    ebx
        dec    esp
        and    byte ptr [edx + 0x65], dl
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebx + 0x6f], ah
        jne    0x66d
        je     loc_4_0621
loc_4_0601:
        and    eax, dword ptr [eax + ebx*2 + 0xd]
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        inc    esp
        jb     loc_4_0675
        jbe    loc_4_0673
        and    byte ptr [ebx + 0x44], al
        and    byte ptr [ebp + 0x70], dh
        popal  
        je     loc_4_067D
        and    byte ptr [edx + 0x69], ah
        je     loc_4_0690
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ebx
loc_4_0621:
        pop    eax
        sub    al, 0x20
        jb     loc_4_0690
        jbe    loc_4_068E
        and    byte ptr [ebp + 0x70], dh
        popal  
        je     loc_4_0695
loc_4_0630:
        and    byte ptr [ebp + 0x76], ah
        outsb  dx, byte ptr gs:[esi]
        je     0x657
        push   0x6c646e61
        and    byte ptr gs:[ebx], ah
        inc    ebp
        inc    esp
        pop    eax
        or     eax, 0x2020000a
        inc    esp
loc_4_0648:
        jb     loc_4_06B3
        jbe    loc_4_06B1
        and    byte ptr [ebx + 0x44], al
        and    byte ptr [ebp + 0x70], dh
        popal  
        je     loc_4_06BB
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x64]
        or     eax, 0x2020000a
        inc    esi
        dec    esp
        inc    ecx
        inc    edi
        push   ebx
        and    byte ptr [0x54530020], ch
        push   ebp
loc_4_0673:
        inc    esi
        inc    esi
loc_4_0675:
        inc    ebp
        pop    eax
        dec    ecx
        push   esp
        and    byte ptr [eax], al
        push   eax
        push   edx
loc_4_067D:
        inc    ebp
        inc    edx
        inc    ecx
        push   esp
        and    byte ptr [eax], al
        inc    ebp
        pop    eax
        inc    ebp
        inc    ebx
        inc    esp
        dec    edi
        dec    esi
        inc    ebp
        and    byte ptr [eax], al
        push   edx
loc_4_068E:
        inc    ebp
        inc    esi
loc_4_0690:
        dec    esp
        inc    ebp
        inc    ebx
        push   esp
        inc    ebx
loc_4_0695:
        je     0x709
        insb   byte ptr es:[edi], dx
        inc    ebx
        and    byte ptr [eax], al
        dec    ecx
        dec    esi
        inc    esp
        dec    edi
        push   ebx
        and    byte ptr [eax], al
        dec    ecx
        dec    esi
        inc    esp
        push   ebx
        inc    ebp
        push   esi
        dec    esi
        push   esp
        and    byte ptr [eax], al
        push   esi
        cmp    byte ptr [esi], dh
        dec    esp
        dec    edi
loc_4_06B1:
        inc    ebx
        dec    ebx
loc_4_06B3:
        and    byte ptr [eax], al
        dec    ecx
        push   edx
        inc    ebp
        push   ebx
        and    byte ptr [eax], al
loc_4_06BB:
        dec    eax
        dec    ecx
        inc    esp
        dec    edi
        push   ebx
        dec    esp
        dec    esi
        dec    ebx
        and    byte ptr [eax], al
        push   edi
        dec    ecx
        dec    esi
        dec    esp
        dec    ecx
        inc    ebp
        and    byte ptr [eax], al
        and    byte ptr [0x2020000a], cl
        dec    eax
        dec    ecx
        and    byte ptr [edi + ecx*2 + 0x53], al
        and    byte ptr [ecx + 0x4c], al
        dec    esp
        dec    edi
        and    byte ptr [esi + 0x4c], al
        inc    ecx
        inc    edi
        push   ebx
        and    byte ptr [0x58412320], ch
        or     eax, 0x455b000a
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 0x20], dh
        js     loc_4_0762
        je     loc_4_0727
        and    byte ptr [ecx + 0x6e], ah
        jns    0x720
        outsd  dx, dword ptr [esi]
        je     0x76b
        jb     0x726
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x77b
        and    byte ptr [ebp + esi*2 + 0x6d], ah
        jo     0x732
        arpl   word ptr [edx + 0x69], si
        je     loc_4_0737
        arpl   word ptr [edi + 0x75], bp
        outsb  dx, byte ptr [esi]
        je     loc_4_0790
        and    byte ptr [eax], al
        and    byte ptr [0x2020000a], cl
        and    byte ptr [eax], ah
loc_4_0727:
        inc    ebx
        jb     0x793
        je     loc_4_074C
        push   ebx
        arpl   word ptr gs:[eax], sp
        and    eax, dword ptr [esp + ecx*2 + 0x20]
        arpl   word ptr [edi + 0x75], bp
loc_4_0737:
        outsb  dx, byte ptr [esi]
        je     loc_4_075A
        cmp    eax, 0x4c412320
        or     eax, 0xa
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_074C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_075A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0762:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0790:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
DOSMGR_BackFill_Allowed:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
DOSMGR_Grow_CDSs:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
DOSMGR_Alloc_Local_Sys_VM_Mem:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl            ; -> obj3:0x0010
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x1079
        jb     loc_4_1084
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x74], dh
        popal  
        arpl   word ptr [ebx + 0x20], bp
        outsd  dx, dword ptr [esi]
        jbe    0x1087
        jb     loc_4_108A
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     loc_4_1035
        or     dl, byte ptr [ebx + 0x79]
        jae    0x10a1
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x61], ch
        insb   byte ptr es:[edi], dx
        je     loc_4_109A
loc_4_1035:
        or     eax, 0x636e490a
        jb     loc_4_10A2
        popal  
        jae    0x10a5
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x69], dh
        jp     loc_4_10AE
        and    byte ptr [ecx + 0x6e], ah
        das    
        outsd  dx, dword ptr [esi]
        jb     0x1071
        outsb  dx, byte ptr [esi]
        jne    0x10c1
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebx + 0x74], dh
        popal  
        arpl   word ptr [ebx + 0x73], bp
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebx + 0x4f], al
        dec    esi
        inc    esi
        dec    ecx
        inc    edi
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1084:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_108A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_109A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_10A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_10AE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_4_1214:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1C4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1C54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1C68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1C76:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1C7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1CA2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1CB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1CC2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1CDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D0A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D3A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D5C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1D78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F82:
        add    byte ptr [eax], al
loc_4_1F84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2000:
        VMMCall  VMM._AllocateThreadDataSlot     ; cd20ff000100
        mov    dword ptr [0x1f4], eax        ; -> obj1:0x01F4
        mov    ecx, 0x703
        movzx  ecx, word ptr [ecx]
        shl    ecx, 4
        add    ecx, dword ptr [ebx + 4]
        lea    eax, [ecx + 0x330]
        mov    dword ptr [0x4d0], eax        ; -> obj6:0x04D0
        lea    eax, [ecx + 0x323]
        mov    dword ptr [0x4d4], eax        ; -> obj6:0x04D4
        mov    eax, edx
        shr    eax, 0x10
loc_4_2034:
        mov    dword ptr [0x204], eax        ; -> loc_1_0204
        btr    edx, 0xf
        adc    byte ptr [0x1c65], 0          ; -> obj4:0x1C65
        push   0
        push   0x64
loc_4_2048:
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        test   eax, eax
        jne    loc_4_205B
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_205B:
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        mov    ecx, 0x1a
        VMMCall  VMM.Allocate_PM_App_CB_Area     ; cd2087000100
        test   eax, eax
        jne    loc_4_2075
loc_4_206F:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_2075:
        mov    dword ptr [4], eax            ; -> obj6:0x0004
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [edi + 0x8c]
        mov    dword ptr [8], eax            ; -> obj6:0x0008
        VMMCall  VMM.GetDOSVectors               ; cd20ba000100
        mov    dword ptr [0x14], eax         ; -> obj6:0x0014
        mov    dword ptr [0x18], edx         ; -> obj6:0x0018
        mov    dword ptr [edi + 0x90], edx
        mov    esi, 0x190                    ; -> loc_19_0190
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_4_206F
        mov    dword ptr [0xc], eax          ; -> obj6:0x000C
        mov    dword ptr [edi + 0x8c], eax
        mov    edi, 0x1d26                   ; -> loc_4_1D26
        xor    esi, esi
        movsx  eax, byte ptr [0x1c]          ; -> obj6:0x001C
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x1c], al           ; -> obj6:0x001C
        mov    edi, 0x1c83                   ; -> obj4:0x1C83
        xor    esi, esi
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x1d], al           ; -> obj6:0x001D
        mov    eax, 0x703
        movzx  eax, word ptr [eax]
        shl    eax, 4
        lea    eax, [eax + 0x12b9]
        mov    dword ptr [0x208], eax        ; -> loc_1_0208
        mov    eax, 0x2a
        mov    esi, 0x2c                     ; -> obj5:0x002C
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x21
        mov    esi, 0                        ; -> loc_10_0000
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x24
        mov    esi, 0x2a4                    ; -> loc_11_02A4
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
loc_4_212A:
        mov    eax, 0x23
        mov    esi, 0x19b                    ; -> loc_19_019B
loc_4_2134:
        int    0x20
        inc    ecx
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x59d  R-X
sub_5_0000:
        mov    word ptr [ebp + 0x1c], 0x4800
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [esi + 4], 8
        je     loc_5_0066
        movzx  eax, word ptr [esi]
        movzx  edx, word ptr [esi + 2]
        test   byte ptr [esi + 4], 2
        je     loc_5_0025
        dec    edx
loc_5_0025:
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x1c], 0x4a00
        mov    word ptr [ebp + 0x10], dx
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0x4800
        mov    word ptr [ebp + 0x10], 0xffff
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0x4800
        mov    eax, 0x21
loc_5_0060:
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_5_0066:
        mov    dword ptr [esi + 8], 0
        test   byte ptr [esi + 4], 2
        je     loc_5_00A7
        push   0
        push   0x10
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_5_0172
        mov    dword ptr [esi + 8], eax
        mov    edi, eax
        push   esi
        movzx  ecx, word ptr [esi]
        movzx  esi, word ptr [esi + 2]
        add    esi, ecx
        dec    esi
        shl    esi, 4
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
loc_5_00A7:
        movzx  eax, word ptr [esi]
        movzx  ecx, word ptr [esi + 2]
        shr    eax, 8
        shr    ecx, 8
        push   ecx
        push   eax
        VxDCall  V86MMGR.V86MMGR_Get_Version     ; cd2000000600
        cmp    eax, 0x30a
        pop    ecx
        pop    edx
        push   ebx
        push   ecx
loc_5_00C5:
        push   edx
        jb     loc_5_00F7
        xor    ebx, ebx
loc_5_00CA:
        VxDCall  V86MMGR.V86MMGR_GetPgStatus     ; cd2010000600
        test   ah, 0x10
        je     loc_5_00F7
        test   ax, 0x280
        jne    loc_5_0178
        test   ah, 1
        je     loc_5_0178
        inc    ecx
        dec    edx
loc_5_00EA:
        jne    loc_5_00CA
        pop    edx
        pop    ecx
        pop    ebx
        or     word ptr [esi + 4], 0x200
        jmp    loc_5_0114
loc_5_00F7:
        cmp    edx, dword ptr [esp]
        jne    loc_5_0178
        pop    edx
        pop    ecx
        pop    ebx
        push   edx
        push   0
        push   0
        push   edx
        push   ecx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    edx
        or     eax, eax
        je     loc_5_017B
loc_5_0114:
        and    byte ptr [esi + 4], 0xfe
        movzx  eax, word ptr [esi]
        dec    eax
        shl    eax, 4
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        pop    ebx
        cmp    byte ptr [eax], 0x5a
        jne    loc_5_0133
        or     byte ptr [esi + 4], 1
loc_5_0133:
        pop    ecx
        add    esi, 0x10
        dec    ecx
        jne    0xffffffbd
        mov    esi, 0x214                    ; -> loc_1_0214
        mov    ecx, dword ptr [0x314]        ; -> loc_1_0314
        or     ecx, ecx
        je     0x1be
loc_5_014D:
        test   byte ptr [esi + 5], 1
        jne    loc_5_016B
        mov    dx, word ptr [esi]
        mov    word ptr [ebp + 0x38], dx
        mov    word ptr [ebp + 0x1c], 0x4900
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_5_016B:
        add    esi, 0x10
        loop   loc_5_014D
        jmp    0x1be
loc_5_0172:
        or     byte ptr [esi + 5], 1
        jmp    loc_5_0133
loc_5_0178:
        pop    edx
        pop    ecx
        pop    ebx
loc_5_017B:
        pop    ecx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        jmp    loc_5_020B
        db  051h, 00Fh, 0B7h, 006h, 00Fh, 0B7h, 04Eh, 002h, 0C1h, 0E8h, 008h, 0C1h, 0E9h, 008h, 051h, 050h
        db  0CDh, 020h, 000h, 000h, 006h, 000h, 03Dh, 00Ah, 003h, 000h, 000h, 058h, 059h, 072h, 006h, 0CDh
        db  020h, 013h, 000h, 006h, 000h, 083h, 0C6h, 010h, 059h, 0E2h
DOSMGR_MMGR_PSP_Change_Notifier:
        aad    0xf6
        add    eax, 0x210                    ; -> loc_1_0210
loc_5_01C4:
        add    al, 0x74
        sub    eax, 0x21005f6                ; -> loc_1_0210
        add    byte ptr [eax], al
        add    dh, byte ptr [ebp + ecx - 0x75]
        adc    eax, 0                        ; -> obj6:0x0000
        or     byte ptr [edx + ebx + 1], 4
        jmp    loc_5_01F4
        db  066h, 0C7h, 045h, 01Ch, 003h, 058h, 066h, 0C7h, 045h, 010h, 000h, 000h, 0B8h, 021h, 000h, 000h
        db  000h, 0CDh, 020h, 084h, 000h, 001h, 000h
loc_5_01F4:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        clc    
        ret    
loc_5_020B:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x1d0], ah    ; -> obj1:0x01D0
        shr    eax, 0x10
        mov    eax, dword ptr [eax + 0x22]
loc_5_021F:
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        mov    edx, eax
        mov    eax, dword ptr [eax]
        cmp    ax, -1
        jne    loc_5_021F
        mov    dword ptr [0x17c], edx        ; -> obj6:0x017C
        mov    dword ptr [0x180], eax        ; -> obj6:0x0180
        ret    
        db  053h, 057h, 056h, 0CDh, 020h, 082h, 000h, 001h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh
        db  06Bh, 008h, 066h, 0C7h, 045h, 01Ch, 006h, 033h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 08Ah, 045h, 015h, 0A8h, 010h, 074h, 013h, 0CDh, 020h, 000h, 000h, 006h, 000h
        db  03Dh, 00Ah, 003h, 000h, 000h, 072h, 006h, 0CDh, 020h, 011h, 000h, 006h, 000h, 066h, 0C7h, 045h
        db  01Ch, 000h, 010h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 08Bh, 045h
        db  01Ch, 0A2h
        dd  obj6:0x019C              ; fixup
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h, 080h, 0FCh, 007h, 00Fh, 082h, 057h, 003h, 000h, 000h, 066h
        db  0C7h, 045h, 01Ch, 000h, 034h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h
        db  00Fh, 0B7h, 045h, 038h, 0C1h, 0E0h, 004h, 00Fh, 0B7h, 05Dh, 010h, 04Bh, 003h, 0C3h, 0A3h
        dd  loc_1_0200               ; fixup
        db  066h, 0C7h, 045h, 01Ch, 000h, 052h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h
        db  000h, 00Fh, 0B7h, 045h, 038h, 0A3h
        dd  obj6:0x0190              ; fixup
        db  0C1h, 0E0h, 004h, 00Fh, 0B7h, 05Dh, 010h, 0A3h
        dd  obj6:0x018C              ; fixup
        db  003h, 0C3h, 0A3h
        dd  loc_1_0204               ; fixup
        db  066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h, 0C7h, 045h, 010h, 015h, 000h, 033h, 0C0h, 066h, 089h
        db  045h, 014h, 066h, 0C7h, 045h, 018h, 000h, 000h, 066h, 089h, 045h, 038h, 0B8h, 02Fh, 000h, 000h
        db  000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h, 045h, 014h, 00Bh, 0C0h, 075h, 005h, 0B8h
        db  070h, 000h, 000h, 000h, 066h, 0A3h
        dd  loc_4_212A               ; fixup
        db  00Fh, 0B7h, 075h, 038h, 0C1h, 0E6h, 004h, 00Fh, 0B7h, 045h, 010h, 003h, 0F0h, 0CDh, 020h, 001h
        db  000h, 001h, 000h, 003h, 073h, 004h, 0BFh
        dd  loc_4_1F74               ; fixup
        db  0B9h, 00Eh, 000h, 000h, 000h, 0FCh, 0F3h, 0A4h, 066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h, 0C7h
        db  045h, 010h, 015h, 000h, 066h, 0C7h, 045h, 018h, 004h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh
        db  020h, 084h, 000h, 001h, 000h, 066h, 081h, 07Dh, 01Ch, 07Ch, 0B9h, 075h, 011h, 066h, 081h, 07Dh
        db  014h, 0ABh, 0A2h, 075h, 009h, 00Fh, 0B7h, 045h, 010h, 0A3h
        dd  loc_4_1F84               ; fixup
        db  066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h, 0C7h, 045h, 010h, 015h, 000h, 066h, 0C7h, 045h, 018h
        db  001h, 000h, 00Fh, 0B7h, 005h
        dd  loc_4_1F82               ; fixup
        db  089h, 045h, 014h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 066h, 081h
        db  07Dh, 01Ch, 07Ch, 0B9h, 075h, 017h, 066h, 081h, 07Dh, 014h, 0ABh, 0A2h, 075h, 00Fh, 00Fh, 0B7h
        db  045h, 010h, 0A3h
        dd  obj6:0x019D              ; fixup
        db  0FEh, 005h
        dd  obj6:0x01A1              ; fixup
        db  066h, 083h, 03Dh
        dd  loc_4_212A               ; fixup
        db  000h, 00Fh, 084h, 01Fh, 002h, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 000h, 051h, 0B8h, 021h, 000h
        db  000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h, 05Dh, 010h, 0C1h, 0E3h, 004h, 089h
        db  01Dh
        dd  loc_4_1D70               ; fixup
        db  0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 05Bh, 004h, 003h, 01Dh
        dd  loc_1_0204               ; fixup
        db  08Bh, 043h, 016h, 00Fh, 0B7h, 0C8h, 0C1h, 0E8h, 010h, 0C1h, 0E0h, 004h, 003h, 0C1h, 0A3h
        dd  obj6:0x0020              ; fixup
        db  00Fh, 0B6h, 043h, 021h, 0A3h
        dd  obj6:0x0028              ; fixup
        db  08Bh, 043h, 022h, 0A3h
        dd  obj4:0x212C              ; fixup
DOSMGR_Get_IndosPtr:
        add    byte ptr [eax], al
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        mov    dword ptr [0x1d60], eax       ; -> loc_4_1D60
        mov    eax, dword ptr [ebx + 4]
        movzx  ebx, word ptr [ebx + 6]
        shl    ebx, 4
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        mov    dword ptr [0x188], eax        ; -> obj6:0x0188
        test   byte ptr [0x1f84], 2          ; -> loc_4_1F84
        jne    loc_5_04C8
        cmp    word ptr [eax + 2], -1
        jne    loc_5_0483
        mov    dword ptr [0x203c], eax       ; -> obj4:0x203C
        mov    ebx, dword ptr [0x204]        ; -> loc_1_0204
        mov    eax, dword ptr [ebx + 0x1a]
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        jmp    loc_5_04C3
loc_5_0483:
        mov    esi, eax
        cmp    dword ptr [0x18c], ebx        ; -> obj6:0x018C
        jne    loc_5_04A3
        movzx  ebx, word ptr [eax + 2]
        mov    eax, dword ptr [eax]
        shl    ebx, 4
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        jmp    loc_5_0483
loc_5_04A3:
        cmp    word ptr [esi], -1
        je     loc_5_04BD
        mov    esi, dword ptr [esi]
        mov    ecx, esi
        db  081h, 0E1h, 0FFh, 0FFh, 000h
sub_5_04B2:
        add    cl, al
        out    dx, al
        adc    cl, al
        out    4, al
        add    esi, ecx
        jmp    loc_5_04A3
loc_5_04BD:
        mov    dword ptr [0x203c], esi       ; -> obj4:0x203C
loc_5_04C3:
        mov    dword ptr [0x1d64], eax       ; -> loc_4_1D64
loc_5_04C8:
        test   byte ptr [0x1f84], 8          ; -> loc_4_1F84
        jne    loc_5_0508
        mov    word ptr [ebp + 0x1c], 0x5d06
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x3c]
        shl    eax, 4
        movzx  ebx, word ptr [ebp + 4]
        add    eax, ebx
        movzx  edx, word ptr [ebp + 0x14]
        movzx  ecx, word ptr [ebp + 0x18]
        mov    dword ptr [0x2000], eax       ; -> loc_4_2000
        mov    dword ptr [0x2004], edx       ; -> obj4:0x2004
        mov    dword ptr [0x1d5c], ecx       ; -> loc_4_1D5C
loc_5_0508:
        test   byte ptr [0x1f84], 8          ; -> loc_4_1F84
        jne    loc_5_0561
        push   0
        push   0x1ff8                        ; -> loc_4_1FF8
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        or     eax, eax
        je     0x60b
        mov    eax, dword ptr [0x2000]       ; -> loc_4_2000
        add    eax, dword ptr [0x2004]       ; -> obj4:0x2004
        mov    dword ptr [0x2014], eax       ; -> obj4:0x2014
        mov    eax, dword ptr [0x1d5c]       ; -> loc_4_1D5C
        sub    eax, dword ptr [0x2004]       ; -> obj4:0x2004
loc_5_0544:
        mov    dword ptr [0x2018], eax       ; -> obj4:0x2018
        push   0
loc_5_054B:
        push   0x200c                        ; -> obj4:0x200C
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        or     eax, eax
        je     0x60b
loc_5_0561:
        call   0x695
        jb     0x5f1
        mov    eax, 0x58
        mov    dword ptr [0x24], eax         ; -> obj6:0x0024
        mul    dword ptr [0x28]              ; -> obj6:0x0028
        mov    dword ptr [0x2c], eax         ; -> obj6:0x002C
        test   byte ptr [0x1f84], 1          ; -> loc_4_1F84
        jne    0x5ad
        mov    dword ptr [0x202c], eax       ; -> obj4:0x202C
        mov    eax, dword ptr [0x20]         ; -> obj6:0x0020
        mov    dword ptr [0x2028], eax       ; -> obj4:0x2028
        push   0
        db  068h
        dd  obj4:0x2020              ; fixup
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x4da  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Eh, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 004h, 000h, 000h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  043h, 04Fh, 04Eh, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x1744  R-X
loc_7_0000:
        pushal 
        movzx  eax, word ptr [edi + 2]
        movzx  edx, word ptr [edi + 4]
        shl    eax, 4
        shl    edx, 4
        movzx  ecx, word ptr [edi]
        add    edi, 8
loc_7_0015:
        movzx  ebx, word ptr [edi]
        mov    ebp, dword ptr [ebx + eax]
        movzx  esi, word ptr [edi + 2]
        xchg   dword ptr [esi + edx], ebp
        mov    dword ptr [ebx + eax], ebp
        add    edi, 4
        loop   loc_7_0015
        popal  
        ret    
        db  08Bh, 045h, 01Ch, 00Fh, 0B6h, 0CCh, 081h, 0E9h, 080h, 000h, 000h, 000h, 072h, 026h, 083h, 0F9h
        db  004h, 077h, 021h, 08Bh, 035h
        dd  obj6:0x0000              ; fixup
        db  08Bh, 03Dh
        dd  loc_1_0208               ; fixup
        db  003h, 07Bh, 004h, 0FFh, 014h, 08Dh
        dd  obj6:0x00E1              ; fixup
        db  072h, 00Ah, 080h, 03Dh
        dd  obj6:0x001D              ; fixup
        db  000h, 074h, 001h, 0F9h, 0C3h, 03Ch, 010h, 073h, 02Ah, 083h, 07Ch, 01Eh, 004h, 000h, 075h, 00Bh
        db  0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 01Fh, 000h, 001h, 000h, 03Ch, 002h, 075h, 002h, 0FEh
        db  007h, 0FFh, 005h
        dd  loc_1_01FC               ; fixup
        db  003h, 0DEh, 0FFh, 043h, 004h, 00Fh, 0B6h, 0C0h, 0FEh, 044h, 018h, 008h, 0F8h, 0C3h, 00Fh, 0B6h
        db  0C0h, 083h, 0F8h, 010h, 073h, 029h, 03Ch, 002h, 075h, 007h, 080h, 03Fh, 000h, 074h, 002h, 0FEh
        db  00Fh, 003h, 0DEh, 080h, 07Ch, 018h, 008h, 000h, 074h, 015h, 0FEh, 04Ch, 018h, 008h, 0FFh, 04Bh
        db  004h, 0FFh, 00Dh
        dd  loc_1_01FC               ; fixup
        db  075h, 006h, 0CDh, 020h, 020h, 000h, 001h, 000h, 0F8h, 0C3h, 0B9h, 004h, 000h, 000h, 000h, 033h
        db  0C0h, 088h, 007h, 08Dh, 07Ch, 01Eh, 008h, 0FCh, 0F3h, 0ABh, 033h, 0C9h, 087h, 04Ch, 01Eh, 004h
        db  0E3h, 00Bh, 0A3h
        dd  loc_1_01FC               ; fixup
        db  0CDh, 020h, 020h, 000h, 001h, 000h, 0F8h, 0C3h, 0CDh, 020h, 038h, 000h, 001h, 000h, 0F9h, 0C3h
        db  08Bh, 0C0h, 08Bh, 045h, 01Ch, 080h, 0FCh, 048h, 074h, 056h, 066h, 03Dh, 022h, 011h, 074h, 00Ch
        db  084h, 0C0h, 074h, 037h, 066h, 03Dh, 080h, 010h, 074h, 040h, 0F9h, 0C3h, 0CDh, 020h, 004h, 000h
        db  001h, 000h, 074h, 0F6h, 08Bh, 015h
        dd  obj6:0x018C              ; fixup
        db  003h, 053h, 004h, 00Fh, 0B7h, 08Ah, 034h, 003h, 000h, 000h, 00Fh, 0B7h, 092h, 030h, 003h, 000h
        db  000h, 08Bh, 0F2h, 0C1h, 0E6h, 004h, 003h, 073h, 004h, 0CDh, 020h, 00Eh, 000h, 015h, 000h, 0F9h
        db  0C3h, 080h, 0FCh, 010h, 074h, 00Ah, 080h, 0FCh, 011h, 074h, 005h, 080h, 0FCh, 014h, 075h, 0C0h
        db  0C6h, 045h, 01Ch, 0FFh, 0F8h, 0C3h, 08Bh, 015h
        dd  obj6:0x0000              ; fixup
        db  0F6h, 004h, 01Ah, 001h, 074h, 0AEh, 03Ch, 010h, 075h, 0AAh, 0A1h
        dd  obj6:0x018C              ; fixup
        db  003h, 043h, 004h, 00Fh, 0B7h, 080h, 030h, 003h, 000h, 000h, 066h, 039h, 044h, 01Ah, 002h, 075h
        db  094h, 0BEh
        dd  obj6:0x00F5              ; fixup
        db  00Fh, 0BAh, 02Ch, 01Ah, 00Fh, 072h, 00Fh, 08Bh, 035h
        dd  obj6:0x0010              ; fixup
        db  003h, 073h, 004h, 081h, 0C6h, 0AFh, 000h, 000h, 000h, 0FCh, 02Bh, 0C0h, 089h, 045h, 01Ch, 0ACh
        db  00Fh, 0B7h, 07Dh, 03Ch, 0C1h, 0E7h, 004h, 00Fh, 0B7h, 04Dh, 014h, 003h, 0F9h, 003h, 07Bh, 004h
        db  00Fh, 0B6h, 00Fh, 047h, 049h, 03Bh, 0C8h, 072h, 002h, 08Bh, 0C8h, 08Ah, 0C1h, 0AAh, 041h, 0F3h
        db  0A4h, 0F8h, 0C3h, 066h, 0A3h
        dd  obj6:0x04D8              ; fixup
        db  0C3h, 060h, 08Bh, 04Dh, 018h, 066h, 0F7h, 003h, 080h, 001h, 075h, 003h, 00Fh, 0B7h, 0C9h, 085h
        db  0C9h, 00Fh, 084h, 051h, 001h, 000h, 000h, 00Fh, 0A0h, 0FFh, 075h, 018h, 0FFh, 075h, 014h, 051h
        db  0CDh, 020h, 00Ah, 000h, 006h, 000h, 081h, 0F9h, 0FFh, 0FFh, 000h, 000h, 076h, 005h, 0B9h, 000h
        db  0F0h, 000h, 000h, 087h, 00Ch, 024h, 066h, 08Bh, 045h, 03Ch, 08Bh, 055h, 014h, 0E8h, 063h, 003h
        db  000h, 000h, 073h, 073h, 081h, 0F9h, 0FFh, 0FFh, 000h, 000h, 077h, 06Bh, 08Bh, 0F0h, 08Bh, 0F9h
        db  049h, 025h, 0FFh, 00Fh, 000h, 000h, 003h, 0C8h, 0C1h, 0E9h, 00Ch, 041h, 08Bh, 0C6h, 0C1h, 0E8h
        db  00Ch, 06Ah, 000h, 051h, 050h, 06Ah, 000h, 051h, 050h, 0CDh, 020h, 063h, 000h, 001h, 000h, 083h
        db  0C4h, 00Ch, 085h, 0C0h, 075h, 007h, 08Bh, 0CFh, 083h, 0C4h, 00Ch, 0EBh, 03Ah, 0C1h, 0E6h, 00Ch
        db  066h, 0C1h, 0EEh, 00Ch, 066h, 089h, 075h, 014h, 0C1h, 0EEh, 010h, 066h, 087h, 075h, 060h, 0E8h
        dd  sub_12_04B0              ; fixup
        db  066h, 089h, 075h, 060h, 0CDh, 020h, 064h, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 066h, 0F7h, 003h
        db  080h, 001h, 00Fh, 084h, 0AFh, 000h, 000h, 000h, 066h, 0C7h, 045h, 01Eh, 000h, 000h, 0E9h, 0A4h
        db  000h, 000h, 000h, 0B8h, 014h, 03Ch, 000h, 000h, 0CDh, 020h, 007h, 000h, 006h, 000h, 08Bh, 055h
        db  010h, 08Bh, 03Dh
        dd  obj6:0x0000              ; fixup
        db  003h, 0FBh, 066h, 039h, 057h, 02Eh, 075h, 04Ah, 08Bh, 0C7h, 0CDh, 020h, 008h, 001h, 001h, 000h
        db  097h, 066h, 08Bh, 040h, 02Ah, 066h, 03Bh, 047h, 034h, 075h, 037h, 08Bh, 047h, 030h, 08Bh, 0D0h
        db  003h, 0C1h, 089h, 047h, 030h, 03Bh, 00Ch, 024h, 076h, 028h, 081h, 0E2h, 0FFh, 001h, 000h, 000h
        db  074h, 020h, 08Bh, 0F9h, 08Bh, 00Ch, 024h, 02Bh, 0CAh, 08Bh, 045h, 01Ch, 08Bh, 0D1h, 0E8h, 061h
        db  000h, 000h, 000h, 072h, 04Bh, 087h, 0D1h, 03Bh, 0D1h, 072h, 037h, 02Bh, 0FAh, 08Bh, 00Ch, 024h
        db  0EBh, 01Fh, 08Bh, 045h, 01Ch, 033h, 0D2h, 03Bh, 00Ch, 024h, 076h, 01Bh, 08Bh, 0F9h, 08Bh, 00Ch
        db  024h, 0E8h, 03Eh, 000h, 000h, 000h, 072h, 028h, 003h, 0D1h, 03Bh, 00Ch, 024h, 072h, 013h, 02Bh
        db  0F9h, 03Bh, 0F9h, 077h, 0ECh, 08Bh, 0CFh, 0E3h, 009h, 0E8h, 026h, 000h, 000h, 000h, 072h, 010h
        db  003h, 0D1h, 066h, 089h, 055h, 01Ch, 066h, 0F7h, 003h, 080h, 001h, 074h, 003h, 089h, 055h, 01Ch
        db  083h, 0C4h, 004h, 08Fh, 045h, 014h, 08Fh, 045h, 018h, 00Fh, 0A1h, 061h, 0C3h, 0E8h
        dd  sub_12_04B0              ; fixup
        db  061h, 0C3h, 052h, 057h, 08Bh, 0D4h, 08Bh, 03Dh
        dd  obj1:0x01F4              ; fixup
        db  085h, 0FFh, 074h, 00Ah, 08Bh, 0D7h, 0CDh, 020h, 008h, 001h, 001h, 000h, 003h, 0D7h, 083h, 0ECh
        db  06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 083h, 0ECh, 008h, 06Ah, 000h, 08Bh, 03Ah
        db  089h, 022h, 051h, 057h, 00Fh, 0BAh, 0E0h, 008h, 0F5h, 0CDh, 020h, 008h, 000h, 006h, 000h, 089h
        db  04Dh, 018h, 066h, 089h, 07Dh, 014h, 0C1h, 0EFh, 010h, 066h, 087h, 07Dh, 060h, 089h, 045h, 01Ch
        db  0E8h
        dd  sub_12_04B0              ; fixup
        db  08Fh, 002h, 066h, 089h, 07Dh, 060h, 0F6h, 045h, 02Ch, 001h, 075h, 045h, 066h, 039h, 04Dh, 01Ch
        db  075h, 01Fh, 00Fh, 0BAh, 0E0h, 008h, 0CDh, 020h, 009h, 000h, 006h, 000h, 083h, 0C4h, 07Ch, 066h
        db  00Fh, 0BAh, 065h, 02Ch, 000h, 072h, 007h, 00Fh, 0B7h, 04Dh, 01Ch, 003h, 0F1h, 0F8h, 05Fh, 05Ah
        db  0C3h, 00Fh, 0BAh, 0E0h, 008h, 073h, 0DFh, 050h, 00Fh, 0B7h, 045h, 01Ch, 085h, 0C0h, 074h, 00Dh
        db  051h, 08Bh, 0C8h, 0F9h, 0CDh, 020h, 009h, 000h, 006h, 000h, 059h, 02Bh, 0C8h, 058h, 0F8h, 0EBh
        db  0C5h, 080h, 07Ch, 024h, 004h, 000h, 074h, 0BEh, 0F8h, 0CDh, 020h, 009h, 000h, 006h, 000h, 0CDh
        db  020h, 004h, 000h, 001h, 000h, 075h, 006h, 0CDh, 020h, 072h, 001h, 001h, 000h, 052h, 08Dh, 054h
        db  024h, 008h, 050h, 056h, 0CDh, 020h, 083h, 000h, 001h, 000h, 0E8h
        dd  sub_11_0B94              ; fixup
        db  0CDh, 020h, 086h, 000h, 001h, 000h, 0CDh, 020h, 004h, 000h, 001h, 000h, 075h, 008h, 033h, 0C9h
        db  0CDh, 020h, 03Ch, 001h, 001h, 000h, 05Eh, 03Ch, 001h, 058h, 05Ah, 075h, 009h, 059h, 083h, 0C4h
        db  00Ch, 0E9h, 02Fh, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 07Ch, 0F9h, 0EBh, 083h, 0A1h
        dd  loc_1_0200               ; fixup
        db  0C3h, 08Bh, 015h
        dd  obj6:0x0088              ; fixup
        db  085h, 0D2h, 074h, 011h, 08Bh, 0C2h, 0C1h, 0E0h, 00Ch, 066h, 0C1h, 0E8h, 00Ch, 087h, 015h
        dd  obj6:0x007C              ; fixup
        db  089h, 002h, 08Bh, 015h
        dd  obj6:0x0084              ; fixup
        db  085h, 0D2h, 074h, 054h, 079h, 031h, 0F7h, 0DAh, 0CDh, 020h, 06Bh, 000h, 001h, 000h, 0C1h, 0E0h
        db  008h, 051h, 08Dh, 00Ch, 002h, 08Bh, 0F9h, 0C1h, 0E7h, 004h, 083h, 0C7h, 006h, 0A1h
        dd  obj6:0x0080              ; fixup
        db  0BAh, 03Bh, 000h, 000h, 000h, 0F7h, 0E2h, 08Bh, 0D1h, 08Dh, 048h, 003h, 0C1h, 0E9h, 002h, 033h
        db  0C0h, 0FCh, 0F3h, 0ABh, 059h, 0C1h, 0E2h, 004h, 0A1h
        dd  obj6:0x0080              ; fixup
        db  066h, 089h, 042h, 004h, 083h, 0C8h, 0FFh, 089h, 002h, 08Bh, 03Dh
        dd  obj6:0x007C              ; fixup
        db  066h, 039h, 007h, 075h, 005h, 0C1h, 0E2h, 00Ch, 089h, 017h, 0C3h, 0CDh, 020h, 006h, 080h, 006h
        db  000h, 033h, 0C9h, 0CDh, 020h, 03Ch, 001h, 001h, 000h, 057h, 0CDh, 020h, 006h, 000h, 006h, 000h
        db  08Bh, 035h
        dd  obj6:0x04D4              ; fixup
        db  08Bh, 03Ch, 024h, 08Dh, 07Fh, 033h, 0FCh, 0A4h, 0A5h, 0A5h, 05Fh, 0CDh, 020h, 072h, 081h, 001h
        db  000h, 057h, 0CDh, 020h, 008h, 001h, 001h, 000h, 0F6h, 047h, 002h, 003h, 05Fh, 0C3h, 0E8h, 0EEh
        db  0FFh, 0FFh, 0FFh, 00Fh, 084h
        dd  loc_11_0CC8              ; fixup
        db  0CDh, 020h, 006h, 080h, 006h, 000h, 0E8h, 0DDh, 0FFh, 0FFh, 0FFh, 00Fh, 084h
        dd  loc_11_0CFA              ; fixup
        db  0CDh, 020h, 006h, 080h, 006h, 000h, 039h, 03Ah, 03Bh, 041h, 043h, 047h, 04Eh, 04Fh, 056h, 06Ch
        db  060h, 0A0h, 0A6h, 0A7h, 0A8h, 0FFh, 005h, 005h, 005h, 005h, 005h, 036h, 039h, 03Ah, 016h, 031h
        db  03Ch, 03Bh, 038h, 03Dh, 037h, 011h, 005h
        dd  loc_10_0266              ; fixup
        db  004h, 014h, 03Ch, 001h, 040h, 001h, 000h, 038h, 000h, 021h, 001h, 034h, 000h, 014h, 03Ch, 000h
        db  021h, 001h, 004h, 001h, 014h, 03Ch, 000h, 021h, 001h, 004h, 001h, 004h, 03Ch, 000h, 021h, 004h
        db  004h, 03Ch, 001h, 004h, 001h, 000h, 038h, 000h, 021h, 005h
        dd  loc_10_028A              ; fixup
        db  001h, 008h, 000h, 004h, 03Ch, 000h, 021h, 001h, 008h, 000h, 000h, 038h, 000h, 021h, 051h, 052h
        db  06Ah, 000h, 050h, 053h, 0CDh, 020h, 06Eh, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 05Ah, 066h, 0F7h
        db  003h, 080h, 001h, 075h, 003h, 00Fh, 0B7h, 0D2h, 003h, 0C2h, 03Dh, 000h, 000h, 010h, 000h, 072h
        db  010h, 02Bh, 043h, 004h, 072h, 007h, 03Dh, 000h, 000h, 010h, 000h, 072h, 004h, 003h, 043h, 004h
        db  0F8h, 059h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 0FFh, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 043h, 04Fh, 04Dh, 053h, 050h, 045h, 043h, 000h
        dd  loc_19_001C              ; fixup
        dd  loc_19_002B              ; fixup
        dd  loc_19_0035              ; fixup
        dd  loc_19_004F              ; fixup
        dd  loc_19_0065              ; fixup
        dd  loc_19_006A              ; fixup
        db  02Fh, 043h, 020h, 02Fh, 04Bh, 020h, 001h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 064h, 000h
        db  000h, 000h, 0E8h, 003h, 000h, 000h, 010h, 027h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h
        dd  obj5:0x0062              ; fixup
        dd  obj5:0x0092              ; fixup
        dd  loc_5_00C5               ; fixup
        dd  loc_5_0060               ; fixup
        dd  loc_5_00EA               ; fixup
        db  004h, 065h, 078h, 069h, 074h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 002h, 000h, 014h, 03Ch, 000h, 021h, 001h, 004h, 000h, 014h, 03Ch, 000h
        db  021h, 005h
        dd  loc_11_0354              ; fixup
        db  005h
        dd  loc_13_00A5              ; fixup
        db  002h, 018h, 014h, 03Ch, 000h, 021h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h
        db  003h, 000h, 004h, 000h, 000h, 013h, 013h, 009h, 009h, 00Ah, 013h, 013h, 013h, 00Bh, 000h, 000h
        db  00Ch, 00Eh, 00Eh, 000h, 000h, 00Eh, 000h, 013h, 013h, 013h, 013h, 014h, 028h, 013h, 013h, 010h
        db  000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 02Fh, 007h, 000h, 00Fh, 015h, 000h, 000h, 008h, 006h
        db  006h, 006h, 006h, 006h, 000h, 019h, 019h, 006h, 030h, 006h, 01Ah, 000h, 000h, 018h, 01Dh, 01Eh
        db  020h, 02Bh, 02Dh, 000h, 01Bh, 01Ch, 024h, 025h, 00Fh, 027h, 000h, 029h, 017h, 000h, 000h, 026h
        db  02Ah, 006h, 000h, 01Fh, 021h, 022h, 023h, 000h, 025h, 033h, 011h, 02Eh, 000h, 000h, 000h, 000h
        db  000h, 000h, 032h, 000h, 000h, 000h, 034h, 035h
        dd  obj6:0x0310              ; fixup
        dd  obj6:0x0310              ; fixup
        dd  obj6:0x031B              ; fixup
        dd  obj6:0x0312              ; fixup
        dd  obj6:0x0320              ; fixup
        dd  obj6:0x0353              ; fixup
        dd  obj6:0x0349              ; fixup
        dd  obj6:0x035D              ; fixup
        dd  obj6:0x0362              ; fixup
        dd  obj6:0x032E              ; fixup
        dd  obj6:0x0325              ; fixup
        dd  obj6:0x0367              ; fixup
        dd  obj6:0x036E              ; fixup
        dd  obj6:0x0373              ; fixup
        dd  obj6:0x0378              ; fixup
        dd  obj6:0x0333              ; fixup
        dd  obj6:0x033D              ; fixup
        dd  obj6:0x0310              ; fixup
        dd  obj6:0x0310              ; fixup
        dd  obj6:0x0310              ; fixup
        dd  obj6:0x0397              ; fixup
        dd  obj6:0x039C              ; fixup
        dd  obj6:0x038F              ; fixup
        dd  obj6:0x037D              ; fixup
        dd  obj6:0x03AB              ; fixup
        dd  obj6:0x03A1              ; fixup
        dd  obj6:0x03A6              ; fixup
        dd  obj6:0x03E9              ; fixup
        dd  obj6:0x03EC              ; fixup
        dd  obj6:0x03DA              ; fixup
        dd  obj6:0x03DF              ; fixup
        dd  obj6:0x03B7              ; fixup
        dd  obj6:0x03E4              ; fixup
        dd  obj6:0x03BC              ; fixup
        dd  obj6:0x03C1              ; fixup
        dd  obj6:0x03C6              ; fixup
        dd  obj6:0x03F1              ; fixup
        dd  obj6:0x03F6              ; fixup
loc_7_12B0:
        sti    
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al            ; -> obj6:0x0400
        add    al, 0
        add    byte ptr [esi], cl            ; -> obj6:0x040E
        add    al, 0
        add    byte ptr [ebx], dl            ; -> obj6:0x0413
        add    al, 0
        add    byte ptr [eax], bl            ; -> obj6:0x0418
        add    al, 0
        add    byte ptr [ebx + 0x21000004], cl ; -> obj6:0x048B
        add    al, 0
        add    byte ptr [esi], ah            ; -> obj6:0x0426
        add    al, 0
        add    byte ptr [ebx], ch            ; -> obj6:0x042B
        add    al, 0
        add    byte ptr [esp + eax], al      ; -> obj6:0x0444
        add    byte ptr [edi], bh            ; -> obj6:0x043F
        add    al, 0
        add    byte ptr [edx + 4], bl        ; -> obj6:0x045A
        add    byte ptr [eax], al
        pop    edi
        add    al, 0
        add    byte ptr [eax], bh            ; -> obj6:0x0338
        add    eax, dword ptr [eax]
loc_7_12E7:
        add    byte ptr [ecx + 4], cl        ; -> obj6:0x0449
        add    byte ptr [eax], al
        daa    
        add    eax, 0x5440000                ; -> loc_5_0544
        add    byte ptr [eax], al
        cmp    eax, 0x36000005               ; -> obj5:0x0536
        db  005h, 000h, 000h
loc_7_12FC:
        sub    al, 5
        add    byte ptr [eax], al
        das    
        add    eax, 0x4300000                ; -> obj6:0x0430
        add    byte ptr [eax], al
        dec    ebx
        add    eax, 0x5550000                ; -> obj5:0x0555
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    eax, eax                      ; -> loc_11_0CC0
        or     al, 0
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    eax, 0xc8a                    ; -> loc_11_0C8A
        add    eax, 0xe2c                    ; -> loc_14_0E2C
        add    eax, esi                      ; -> sub_12_00C6
        add    byte ptr [eax], al
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    eax, 0xd6                     ; -> loc_12_00D6
        push   es
        adc    byte ptr [eax], bh
        add    byte ptr [ecx], ah
        push   es
        add    al, 0x3c
loc_7_133B:
        add    byte ptr [ecx], ah
        add    eax, esi                      ; -> sub_12_00C6
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    byte ptr [0x1d7], al          ; -> loc_12_01D7
        add    al, 0x14
        cmp    al, 5
        in     eax, 4
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    al, 0x14
        cmp    al, 0
        and    dword ptr [eax], ecx
        adc    al, 0x3c
        add    byte ptr [ecx], ah
        push   es
        adc    byte ptr [eax + eax], bh
        and    dword ptr [0xd36], eax        ; -> loc_11_0D36
        add    dword ptr [0x3c1400], esp
        and    dword ptr [0x6c], eax         ; -> obj12:0x006C
        add    eax, 0x94                     ; -> loc_12_0094
        push   es
        adc    byte ptr [eax + eax], bh
        and    dword ptr [esp + edx], eax
        cmp    al, 5
        in     eax, 4
        add    byte ptr [eax], al
        add    al, 0
loc_7_1387:
        cmp    byte ptr [0x4e5], al          ; -> obj5:0x04E5
        add    byte ptr [ecx], ah
        add    al, 0x14
        cmp    al, 4
        add    byte ptr [eax], bh
        add    byte ptr [ecx], ah
        add    eax, 0x17c                    ; -> loc_12_017C
        add    eax, 0x15b                    ; -> loc_12_015B
        add    eax, 0x1c4                    ; -> loc_5_01C4
        add    eax, 0x48                     ; -> obj12:0x0048
loc_7_13AB:
        add    dword ptr [eax], eax
        add    al, 0x3c
        add    eax, 0x4f6                    ; -> obj5:0x04F6
        add    byte ptr [ecx], ah
        add    eax, 0x21d                    ; -> loc_12_021D
        add    eax, 0x239                    ; -> loc_12_0239
        add    eax, 0x255                    ; -> loc_12_0255
loc_7_13C6:
        add    al, 4
        cmp    al, 5
loc_7_13CA:
        in     eax, 4
        add    byte ptr [eax], al
        add    dword ptr [esi + 0x5380000], eax
        test   byte ptr [eax + eax], 0
        add    byte ptr [ecx], ah
        add    eax, 0xd8e                    ; -> loc_11_0D8E
        add    eax, 0xebf                    ; -> loc_11_0EBF
        add    eax, 0xf31                    ; -> loc_11_0F31
        add    al, 0x14
        cmp    al, 5
        jmp    loc_7_13EF
loc_7_13EF:
        add    byte ptr [eax], al
        add    eax, 0x277                    ; -> loc_12_0277
        add    eax, 0x2aa                    ; -> loc_12_02AA
        add    eax, 0x352                    ; -> loc_12_0352
        add    eax, dword ptr [eax]          ; -> loc_11_1100
        adc    dword ptr [eax], eax
        add    byte ptr [esp + edi], al
        add    dword ptr [ecx], esp
        add    byte ptr [eax], cl
        cmp    byte ptr [eax], al
        and    dword ptr [0x304], eax        ; -> loc_12_0304
        add    eax, 0x323                    ; -> loc_12_0323
        add    edi, dword ptr [ebx + eax]    ; -> loc_12_037C
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    eax, 0x3e0                    ; -> loc_12_03E0
        add    eax, 0x311                    ; -> loc_15_0311
        add    eax, 0xd63                    ; -> loc_11_0D63
        add    al, 0x14
        cmp    al, 2
        sbb    byte ptr [eax], al
        cmp    byte ptr [eax], al
        and    dword ptr [edx], eax
        sbb    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    eax, 0x3a2                    ; -> loc_12_03A2
        add    eax, 0x2ed                    ; -> loc_15_02ED
        add    eax, 0xd4d                    ; -> loc_11_0D4D
        add    dword ptr [eax], edx
        add    byte ptr [esp + edi], al
        add    dword ptr [eax + 0x380000], eax
        and    dword ptr [esp + eax], eax
        cmp    al, 0
        and    dword ptr [esp + eax], eax
        cmp    al, 5
        in     eax, 4
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    dword ptr [eax], edx
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    bl, byte ptr [eax]
        add    al, 0x3c
        add    byte ptr [ecx], ah
        add    dword ptr [eax], eax
        add    byte ptr [eax], bh
        add    byte ptr [ecx], ah
        add    dword ptr [edx], esp
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    dword ptr [esi], edx
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    eax, 0x1112                   ; -> loc_11_1112
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], al
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax + 0x4840000], al ; -> obj6:0x0484
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
loc_7_14B0:
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], al
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [ecx + 4], ch        ; -> obj6:0x0469
        add    byte ptr [eax], al
        push   ebx
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 4], dh        ; -> obj6:0x0470
        add    byte ptr [eax], al
        jbe    loc_7_14D2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_14D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_150E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_155C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_156A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_158E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_1598:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_159E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_1670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_16A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_16AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 8  vsize=0x127e  R-X
        db  054h, 068h, 065h, 020h, 066h, 06Fh, 06Ch, 06Ch, 06Fh, 077h, 069h, 06Eh, 067h, 020h, 070h, 072h
        db  06Fh, 067h, 072h, 061h, 06Dh, 020h, 06Dh, 061h, 079h, 020h, 070h, 072h, 065h, 076h, 065h, 06Eh
        db  074h, 020h, 079h, 06Fh, 075h, 020h, 066h, 072h, 06Fh, 06Dh, 020h, 072h, 075h, 06Eh, 06Eh, 069h
        db  06Eh, 067h, 020h, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h, 00Dh, 00Ah, 069h, 06Eh, 020h, 033h
        db  038h, 036h, 020h, 065h, 06Eh, 068h, 061h, 06Eh, 063h, 065h, 064h, 020h, 06Dh, 06Fh, 064h, 065h
        db  03Ah, 00Dh, 00Ah, 05Bh, 04Eh, 061h, 06Dh, 065h, 020h, 06Eh, 06Fh, 074h, 020h, 061h, 076h, 061h
        db  069h, 06Ch, 061h, 062h, 06Ch, 065h, 05Dh, 00Dh, 00Ah, 024h, 041h, 062h, 06Fh, 072h, 074h, 020h
        db  06Fh, 072h, 020h, 049h, 067h, 06Eh, 06Fh, 072h, 065h, 03Fh, 020h, 028h, 041h, 020h, 06Fh, 072h
        db  020h, 049h, 029h, 00Dh, 00Ah, 024h, 04Eh, 06Fh, 06Eh, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h
        db  041h, 070h, 070h, 000h, 04Ch, 06Fh, 063h, 061h, 06Ch, 054h, 053h, 052h, 073h, 000h, 04Eh, 06Fh
        db  044h, 04Fh, 053h, 054h, 053h, 052h, 073h, 000h, 049h, 06Eh, 063h, 06Fh, 06Dh, 070h, 061h, 074h
        db  069h, 062h, 06Ch, 065h, 054h, 053h, 052h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        push   es
        push   ds
        mov    eax, dword ptr [esi + 6]
        mov    dword ptr [0x468b00cb], eax
        or     byte ptr [ebx + 0x468b00cd], ah
        or     ah, byte ptr [ebx + 0x5e8b00cf]
        add    al, 0x89
        push   ds
        ret    0x3300
        db  0C0h, 0A3h, 0C0h, 000h, 08Bh, 0F8h, 0C7h, 007h, 000h, 000h, 0C6h, 047h, 004h, 000h, 033h, 0FFh
        db  08Eh, 0C7h, 0B8h, 00Bh, 016h, 0CDh, 02Fh, 08Ch, 006h, 0C4h, 000h, 089h, 03Eh, 0C6h, 000h, 0B4h
        db  058h, 033h, 0DBh, 0B0h, 002h, 0CDh, 021h, 0A2h, 0C8h, 000h, 0B4h, 058h, 0BBh, 001h, 000h, 0B0h
        db  003h, 0CDh, 021h, 0B4h, 051h, 0CDh, 021h, 089h, 01Eh, 0D1h, 000h, 0B4h, 052h, 0CDh, 021h, 026h
        db  08Bh, 047h, 0FEh, 0A3h, 0BAh, 000h, 08Eh, 006h, 0BAh, 000h, 026h, 083h, 03Eh, 001h, 000h, 000h
        db  074h, 026h, 026h, 080h, 03Eh, 000h, 000h, 05Ah, 074h, 01Eh, 026h, 080h, 03Eh, 000h, 000h, 04Dh
        db  074h, 003h, 0E9h, 09Fh, 000h, 026h, 083h, 03Eh, 001h, 000h, 00Ah, 077h, 00Bh, 08Ch, 0C0h, 040h
        db  026h, 003h, 006h, 003h, 000h, 0A3h, 0BAh, 000h, 0A1h, 0BAh, 000h, 0A3h, 0BCh, 000h, 08Eh, 006h
        db  0BCh, 000h, 026h, 080h, 03Eh, 000h, 000h, 05Ah, 074h, 00Dh, 026h, 080h, 03Eh, 000h, 000h, 04Dh
        db  074h, 005h, 033h, 0C0h, 048h, 0EBh, 06Dh, 026h, 0A1h, 001h, 000h, 00Bh, 0C0h, 074h, 04Ah, 083h
        db  0F8h, 00Ah, 076h, 045h, 039h, 006h, 0D1h, 000h, 074h, 03Fh, 08Bh, 01Eh, 0C2h, 000h, 003h, 01Eh
        db  0C0h, 000h, 08Bh, 0FBh, 08Ch, 0C0h, 089h, 005h, 08Dh, 07Fh, 004h, 0C6h, 005h, 000h, 026h, 0A1h
        db  001h, 000h, 048h, 08Eh, 0C0h, 0BEh, 008h, 000h, 0E8h, 05Eh, 001h, 0E8h, 04Ch, 000h, 08Bh, 01Eh
        db  0C2h, 000h, 003h, 01Eh, 0C0h, 000h, 089h, 057h, 002h, 08Bh, 01Eh, 0C0h, 000h, 083h, 0C3h, 00Eh
        db  089h, 01Eh, 0C0h, 000h, 03Bh, 05Eh, 00Ch, 073h, 01Bh, 0A1h, 0BCh, 000h, 08Eh, 0C0h, 040h, 026h
        db  003h, 006h, 003h, 000h, 0A3h, 0BCh, 000h, 026h, 080h, 03Eh, 000h, 000h, 05Ah, 074h, 003h, 0E9h
        db  07Ch, 0FFh, 033h, 0C0h, 08Ah, 01Eh, 0C8h, 000h, 032h, 0FFh, 0B4h, 058h, 0B0h, 003h, 0CDh, 021h
        db  01Fh, 007h, 05Fh, 05Eh, 059h, 05Bh, 08Bh, 0E5h, 05Dh, 0C3h, 053h, 051h, 056h, 057h, 006h, 01Eh
        db  0A1h, 0BCh, 000h, 08Eh, 0C0h, 026h, 08Bh, 01Eh, 001h, 000h, 040h, 03Bh, 0C3h, 075h, 011h, 08Eh
        db  0C0h, 026h, 081h, 03Eh, 000h, 000h, 0CDh, 020h, 075h, 006h, 0B2h, 002h, 08Ch, 0C1h, 0EBh, 00Fh
        db  0B2h, 008h, 08Eh, 0C3h, 026h, 039h, 006h, 02Ch, 000h, 075h, 002h, 0B2h, 004h, 08Bh, 0CBh, 089h
        db  00Eh, 0C9h, 000h, 08Bh, 03Eh, 0C6h, 000h, 0A1h, 0C4h, 000h, 08Eh, 0C0h, 032h, 0F6h, 08Ch, 0C0h
        db  083h, 0F8h, 000h, 074h, 018h, 026h, 03Bh, 04Dh, 004h, 075h, 005h, 080h, 0CEh, 004h, 0EBh, 00Dh
        db  026h, 08Bh, 045h, 002h, 026h, 08Bh, 01Dh, 08Eh, 0C0h, 08Bh, 0FBh, 0EBh, 0E1h, 08Bh, 01Eh, 0C2h
        db  000h, 003h, 01Eh, 0C0h, 000h, 08Dh, 077h, 004h, 08Ah, 024h, 080h, 0FCh, 000h, 074h, 03Bh, 01Eh
        db  007h, 08Bh, 03Eh, 0CBh, 000h, 08Ah, 025h, 080h, 0FCh, 000h, 074h, 008h, 0E8h, 032h, 000h, 073h
        db  003h, 080h, 0CEh, 001h, 08Bh, 03Eh, 0CDh, 000h, 08Ah, 025h, 080h, 0FCh, 000h, 074h, 008h, 0E8h
        db  01Fh, 000h, 073h, 003h, 080h, 0CEh, 002h, 08Bh, 03Eh, 0CFh, 000h, 08Ah, 025h, 080h, 0FCh, 000h
        db  074h, 008h, 0E8h, 00Ch, 000h, 073h, 003h, 080h, 0CEh, 008h, 01Fh, 007h, 05Fh, 05Eh, 059h, 05Bh
        db  0C3h, 006h, 051h, 052h, 053h, 0F8h, 032h, 0C9h, 08Ah, 03Dh, 080h, 0FFh, 000h, 074h, 03Eh, 057h
        db  056h, 08Ah, 03Dh, 0FFh, 035h, 0E8h, 059h, 000h, 083h, 0C4h, 002h, 08Ah, 0F8h, 08Ah, 01Ch, 0FFh
        db  034h, 0E8h, 04Dh, 000h, 083h, 0C4h, 002h, 08Ah, 0D8h, 038h, 0DFh, 075h, 014h, 080h, 0FFh, 000h
        db  074h, 007h, 080h, 0FFh, 02Eh, 074h, 002h, 0EBh, 004h, 0B1h, 001h, 0EBh, 004h, 046h, 047h, 0EBh
        db  0D0h, 05Eh, 05Fh, 080h, 0F9h, 000h, 075h, 005h, 083h, 0C7h, 00Fh, 0EBh, 0B8h, 0F8h, 080h, 0F9h
        db  000h, 074h, 001h, 0F9h, 05Bh, 05Ah, 059h, 007h, 0C3h, 051h, 056h, 057h, 0B9h, 008h, 000h, 01Eh
        db  006h, 01Fh, 007h, 0FCh, 0F3h, 0A4h, 01Eh, 006h, 01Fh, 007h, 0C6h, 005h, 000h, 05Fh, 05Eh, 059h
        db  0C3h
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, byte ptr [esi + 4]
        test   al, 0x80
        jne    loc_8_034F
        cmp    al, 0x61
        jl     loc_8_034D
        cmp    al, 0x7a
        jg     loc_8_034D
        and    al, 0x5f
loc_8_034D:
        jmp    loc_8_034F
loc_8_034F:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
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
        db  000h, 000h, 000h, 044h, 04Fh, 053h, 045h, 044h, 049h, 054h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 043h, 045h, 044h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 050h, 043h, 045h, 044h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  041h, 04Eh, 053h, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Eh
        db  04Eh, 041h, 04Eh, 053h, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F7h, 0C3h, 001h, 000h, 00Fh, 085h, 0C3h
        db  001h, 066h, 089h, 00Eh, 05Ch, 003h, 060h, 006h, 0BFh, 0A8h, 000h, 0BBh, 06Fh, 00Dh, 0E8h, 0B8h
        db  001h, 0BFh, 09Eh, 000h, 0BBh, 081h, 00Ah, 0E8h, 0AFh, 001h, 0BFh, 094h, 000h, 0BBh, 093h, 007h
        db  0E8h, 0A6h, 001h, 068h, 0BCh, 002h, 068h, 048h, 007h, 068h, 081h, 00Ah, 068h, 06Fh, 00Dh, 068h
        db  060h, 003h, 0E8h, 039h, 0F0h, 083h, 0C4h, 00Ah, 0BFh, 01Ch, 006h, 0BEh, 060h, 003h, 083h, 03Ch
        db  000h, 00Fh, 084h, 0DEh, 000h, 08Bh, 05Ch, 002h, 0F6h, 0C7h, 004h, 00Fh, 085h, 0CAh, 000h, 0F6h
        db  0C7h, 001h, 074h, 06Dh, 080h, 03Eh, 05Dh, 010h, 0FFh, 00Fh, 084h, 0BCh, 000h, 080h, 07Ch, 004h
        db  000h, 074h, 01Dh, 056h, 0B9h, 008h, 000h, 08Dh, 074h, 004h, 0BFh, 053h, 000h, 0FCh, 0ACh, 00Ah
        db  0C0h, 074h, 003h, 0AAh, 0E2h, 0F8h, 0B0h, 00Dh, 0AAh, 0B0h, 00Ah, 0AAh, 0B0h, 024h, 0AAh, 05Eh
        db  0B4h, 009h, 0BAh, 000h, 000h, 0CDh, 021h, 0B4h, 009h, 0BAh, 06Ah, 000h, 0CDh, 021h, 0B4h, 001h
        db  0CDh, 021h, 050h, 0B4h, 002h, 0B2h, 00Dh, 0CDh, 021h, 0B2h, 00Ah, 0CDh, 021h, 058h, 03Ch, 041h
        db  074h, 013h, 03Ch, 061h, 074h, 00Fh, 03Ch, 049h, 074h, 004h, 03Ch, 069h, 075h, 0D9h, 0C6h, 006h
        db  05Dh, 010h, 0FFh, 0EBh, 064h, 007h, 061h, 0B8h, 002h, 080h, 0E9h, 0FFh, 000h, 0CDh, 021h, 0EBh
        db  074h, 0F6h, 0C7h, 002h, 074h, 00Eh, 056h, 08Dh, 074h, 004h, 0B8h, 002h, 000h, 0FFh, 01Eh, 05Ch
        db  003h, 05Eh, 0EBh, 045h, 0F6h, 0C7h, 008h, 074h, 040h, 0F6h, 0C3h, 004h, 075h, 03Bh, 08Bh, 004h
        db  006h, 08Eh, 0C0h, 026h, 08Bh, 016h, 003h, 000h, 0F6h, 0C6h, 0F0h, 074h, 01Bh, 0B9h, 0FFh, 00Fh
        db  0C7h, 045h, 004h, 0F0h, 0FFh, 089h, 045h, 002h, 0C7h, 005h, 000h, 000h, 083h, 0C7h, 006h, 02Bh
        db  0D1h, 003h, 0C1h, 0F6h, 0C6h, 0F0h, 075h, 0E8h, 089h, 045h, 002h, 0C7h, 005h, 000h, 000h, 0C1h
        db  0E2h, 004h, 089h, 055h, 004h, 083h, 0C7h, 006h, 007h, 083h, 0C6h, 00Eh, 083h, 03Ch, 000h, 00Fh
        db  085h, 022h, 0FFh, 0B9h, 01Ch, 006h, 03Bh, 0F9h, 074h, 004h, 089h, 00Eh, 056h, 003h, 007h, 061h
        db  0B8h, 000h, 052h, 0EBh, 088h, 08Ch, 0C2h, 066h, 00Fh, 0B7h, 0D2h, 066h, 0C1h, 0E2h, 004h, 066h
        db  00Fh, 0B7h, 0DBh, 066h, 003h, 0D3h, 066h, 0F7h, 0C2h, 000h, 000h, 0FFh, 0FFh, 075h, 06Ah, 066h
        db  0C1h, 0E2h, 010h, 033h, 0DBh, 033h, 0D2h, 066h, 052h, 053h, 0B8h, 000h, 051h, 0CDh, 021h, 08Eh
        db  0C3h, 026h, 0A1h, 002h, 000h, 08Bh, 0D8h, 081h, 0E3h, 0FFh, 000h, 081h, 0FBh, 0FFh, 000h, 075h
        db  01Fh, 0C1h, 0E8h, 008h, 08Bh, 0DCh, 036h, 083h, 03Fh, 000h, 075h, 005h, 036h, 0C7h, 007h, 058h
        db  003h, 036h, 08Bh, 03Fh, 083h, 03Dh, 000h, 074h, 005h, 08Dh, 07Dh, 002h, 0EBh, 0F6h, 089h, 005h
        db  08Ch, 0C8h, 08Eh, 0D8h, 08Eh, 0C0h, 0BEh, 02Eh, 012h, 08Bh, 0FEh, 0B8h, 002h, 05Fh, 033h, 0DBh
        db  0CDh, 021h, 05Bh, 066h, 05Ah, 073h, 005h, 083h, 0F8h, 001h, 074h, 003h, 080h, 0CEh, 080h, 08Bh
        db  036h, 056h, 003h, 0B8h, 000h, 000h, 0C3h, 066h, 05Ah, 0B8h, 002h, 000h, 033h, 0DBh, 066h, 033h
        db  0D2h, 033h, 0F6h, 0C3h, 0B8h, 001h, 080h, 0EBh, 0F3h, 060h, 033h, 0C0h, 0BEh, 086h, 000h, 0FFh
        db  01Eh, 05Ch, 003h, 072h, 03Ch, 006h, 01Eh, 007h, 01Fh, 08Bh, 0F2h, 0FCh, 057h, 08Bh, 0FBh, 0B9h
        db  008h, 000h, 0ACh, 03Ch, 020h, 074h, 00Eh, 03Ch, 02Ch, 074h, 00Ah, 00Ah, 0C0h, 074h, 010h, 0E3h
        db  0F1h, 0AAh, 049h, 0EBh, 0EDh, 083h, 0F9h, 008h, 074h, 0E8h, 083h, 0C3h, 00Fh, 0EBh, 0DEh, 05Fh
        db  006h, 01Eh, 007h, 01Fh, 083h, 0F9h, 008h, 074h, 003h, 083h, 0C3h, 00Fh, 0B8h, 001h, 000h, 0EBh
        db  0BBh, 08Ch, 0C8h, 08Eh, 0C0h, 08Eh, 0D8h, 061h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x6c  R-X
sub_9_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0004:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    loc_9_0010
        db  0FFh, 025h
        dd  sub_9_0000               ; fixup
loc_9_0010:
        cmp    al, 0x21
        jne    loc_9_0066
        pushal 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        test   byte ptr [ebx], 0x20
        jne    loc_9_0065
        cmp    edi, dword ptr [4]            ; -> loc_9_0004
        jne    loc_9_0065
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, eax
        sub    eax, esp
        cmp    eax, 0x1000
        jae    loc_9_0065
        push   0x80000000
        push   0x10
        push   0x65                          ; -> loc_9_0065
        push   0x14
        push   esi
        VMMCall  VMM._Assert_Range               ; cd2073010100
        lea    esp, [esp + 0x14]
        jb     loc_9_0065
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    eax, dword ptr [esi]
        je     loc_11_13A5
loc_9_0065:
        popal  
loc_9_0066:
        jmp    sub_9_0000
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 10  vsize=0x2a8  R-X
loc_10_0000:
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        call   sub_10_0174
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_10_006A
        cmp    byte ptr [ebp + 0x1d], 0x50
        je     loc_10_0074
        movzx  ecx, word ptr [0x4d8]         ; -> obj6:0x04D8
        jecxz  loc_10_006A
        mov    esi, dword ptr [0x4d0]        ; -> obj6:0x04D0
        mov    si, word ptr [esi]
        cmp    cx, si
        je     loc_10_006A
        mov    edx, 0x5000
        xchg   dword ptr [ebp + 0x1c], edx
        xchg   dword ptr [ebp + 0x10], esi
        mov    eax, 0x21
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        xchg   dword ptr [ebp + 0x10], esi
        mov    word ptr [0x4d8], si          ; -> obj6:0x04D8
        mov    dword ptr [ebp + 0x1c], edx
loc_10_006A:
        cmp    word ptr [ebp + 0x1c], 0x5803
        je     loc_10_0080
loc_10_0072:
        stc    
        ret    
loc_10_0074:
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0x4d8], ax          ; -> obj6:0x04D8
        jmp    loc_10_0072
loc_10_0080:
        cmp    dword ptr [0x314], 0          ; -> loc_1_0314
        je     loc_10_0072
        cmp    byte ptr [0x320], 0           ; -> loc_1_0320
        je     loc_10_0072
        cmp    dword ptr [0x318], 0          ; -> loc_1_0318
        je     loc_10_0072
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [edx + ebx + 0x20], 0
        je     loc_10_0072
        cmp    word ptr [ebp + 0x10], 0
        je     loc_10_00D9
        bts    dword ptr [edx + ebx], 0xa
        jb     loc_10_0072
        test   byte ptr [edx + ebx + 1], 1
        jne    loc_10_0072
        push   0x100
        push   0
        push   dword ptr [0x318]             ; -> loc_1_0318
        push   dword ptr [edx + ebx + 0x20]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        jmp    loc_10_0072
loc_10_00D9:
        btr    dword ptr [edx + ebx], 0xa
        jae    loc_10_0072
        test   byte ptr [edx + ebx + 1], 1
        jne    loc_10_0072
        mov    esi, 0x214                    ; -> loc_1_0214
        mov    ecx, dword ptr [0x314]        ; -> loc_1_0314
        xor    edi, edi
loc_10_00F4:
        test   word ptr [esi + 4], 0x100
        jne    loc_10_014B
        movzx  eax, word ptr [esi]
        dec    eax
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        push   ecx
        movzx  ecx, word ptr [eax + 3]
        cmp    cx, word ptr [esi + 2]
        pop    ecx
        jne    loc_10_0119
        cmp    word ptr [eax + 1], 0
        je     loc_10_014B
loc_10_0119:
        bts    word ptr [edx + ebx + 0x60], di
        jb     loc_10_014B
        push   ecx
        push   edx
        movzx  ecx, word ptr [esi + 2]
        inc    ecx
        test   byte ptr [esi + 4], 2
        je     loc_10_012F
        inc    ecx
loc_10_012F:
        shr    ecx, 8
        sub    eax, dword ptr [ebx + 4]
        shr    eax, 0xc
        push   0x400
        push   ecx
        push   eax
        push   ebx
        VMMCall  VMM._SetResetV86Pageable        ; cd2065000100
        add    esp, 0x10
        pop    edx
        pop    ecx
loc_10_014B:
        add    esi, 0x10
        inc    edi
        dec    ecx
        jne    loc_10_00F4
        push   0x100
        push   0
        push   dword ptr [0x318]             ; -> loc_1_0318
        push   dword ptr [edx + ebx + 0x20]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        jmp    loc_10_0072
        db  000h, 000h, 000h
sub_10_0174:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [edx + ebx + 0x54], 0
        je     loc_10_01AB
        or     byte ptr [edx + ebx + 1], 8
        mov    al, byte ptr [ebp + 0x1d]
        cmp    al, 0x70
        ja     loc_10_01AB
        cmp    al, 0
        je     loc_10_01AB
        cmp    al, 0x4b
        je     loc_10_01AB
        cmp    al, 0x26
        je     loc_10_01AB
        mov    ecx, dword ptr [0xfc]         ; -> obj6:0x00FC
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
loc_10_01AB:
        ret    
loc_10_01AC:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
sub_10_01B2:
        movzx  eax, byte ptr [ebp + 0x1d]
        cmp    eax, 0x71
        ja     loc_10_025C
        movzx  edx, byte ptr [eax + 0x1a6]   ; -> obj6:0x01A6
        mov    edx, dword ptr [edx*4 + 0x218] ; -> obj6:0x0218
loc_10_01CD:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    0xfff604ac                    ; -> obj5:0x04AC
        cmp    byte ptr [ebp + 0x1d], 0x4c
        je     0xfff604ac                    ; -> obj5:0x04AC
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    esi, dword ptr [0x1f4]        ; -> obj1:0x01F4
        test   esi, esi
        je     sub_5_04B2
        push   edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
loc_10_0207:
        sub    esp, 8
        push   0
        mov    eax, dword ptr [esi + edi]
        mov    dword ptr [esi + edi], esp
        push   eax
        push   esi
        call   sub_5_04B2
        pop    esi
        pop    dword ptr [esi + edi]
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_10_0227
loc_10_0223:
        add    esp, 0x7c
        ret    
loc_10_0227:
        cmp    byte ptr [esp], 0
        je     loc_10_0223
        mov    edx, esp
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        call   sub_11_0B94
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        cmp    al, 1
        jne    loc_10_0223
        add    esp, 0xc
        lea    esi, [esp]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        mov    esi, dword ptr [0x1f4]        ; -> obj1:0x01F4
        mov    edx, dword ptr [esp + 0x6c]
        jmp    loc_10_0207
loc_10_025C:
        mov    edx, 0x310                    ; -> obj6:0x0310
        jmp    loc_10_01CD
loc_10_0266:
        mov    al, byte ptr [ebp + 0x1c]
        push   ecx
        push   edi
        mov    ecx, 0x10
        mov    edi, 0x507                    ; -> obj5:0x0507
        repne scasb al, byte ptr es:[edi]
        movzx  edx, byte ptr [edi + 0xf]
        pop    edi
        pop    ecx
        mov    edx, dword ptr [edx*4 + 0x218] ; -> obj6:0x0218
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_10_028A:
        mov    edx, 0x561                    ; -> loc_5_0561
        cmp    word ptr [ebp + 0x10], 1
        ja     loc_10_02A1
        je     loc_10_029B
        sub    edx, 7
loc_10_029B:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_10_02A1:
        mov    edx, 0x310                    ; -> obj6:0x0310
        jmp    loc_10_029B
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 11  vsize=0x1413  R-X
loc_11_0000:
        call   sub_14_0C38
        call   sub_14_0A38
        jmp    loc_14_0015
loc_11_000F:
        pushal 
        movzx  edx, word ptr [0x34]          ; -> obj6:0x0034
        mov    eax, edx
        shl    eax, 0x10
        mov    ax, 0x18
        shl    edx, 4
        mov    dword ptr [edx + 0x34], eax
        mov    word ptr [edx + 0x32], 0x14
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 0x15
        mov    word ptr [ebp + 0x18], 2
        mov    eax, dword ptr [0x19d]        ; -> obj6:0x019D
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        clc    
        ret    
loc_11_0087:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        jecxz  loc_11_00B1
        push   eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   0
        push   ecx
        push   dword ptr [eax + 8]
        push   ebx
        push   dword ptr [eax + 0xc]
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        pop    eax
loc_11_00B1:
        mov    edi, dword ptr [esi + 0x10]
        test   edi, edi
        je     loc_11_00C8
        btr    dword ptr [eax + 0x14], 0
        jae    loc_11_00C8
        call   sub_5_0000
        and    byte ptr [edi + 6], 0xfd
loc_11_00C8:
        btr    dword ptr [eax + 0x14], 1
        jae    loc_11_013F
        sub    edi, edi
        mov    edx, dword ptr [esi]
        sub    edx, 0x10
        mov    eax, dword ptr [0x204]        ; -> loc_1_0204
        movzx  eax, word ptr [eax - 2]
        shl    eax, 4
loc_11_00E2:
        cmp    eax, edx
        ja     loc_11_00F6
        xchg   edi, eax
        movzx  eax, word ptr [edi + 3]
        inc    eax
        shl    eax, 4
        add    eax, edi
        cmp    byte ptr [edi], 0x4d
        je     loc_11_00E2
loc_11_00F6:
        test   edi, edi
        je     loc_11_013F
        shr    edx, 4
        dec    edx
        mov    eax, edi
        shr    eax, 4
        sub    edx, eax
        xchg   word ptr [edi + 3], dx
        sub    dx, word ptr [edi + 3]
        jbe    loc_11_013F
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 0x1f
        shr    ecx, 4
        mov    eax, dword ptr [esi]
        sub    edx, ecx
        jbe    loc_11_0138
        dec    edx
        add    eax, dword ptr [esi + 4]
        push   esi
        mov    esi, edi
        xchg   edi, eax
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    byte ptr [eax], 0x4d
        mov    word ptr [edi - 0xd], dx
        jmp    loc_11_013F
loc_11_0138:
        mov    dl, 0x4d
        xchg   byte ptr [edi], dl
        mov    byte ptr [eax - 0x10], dl
loc_11_013F:
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [eax]
        jecxz  loc_11_0153
        mov    dword ptr [esi + 8], ecx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [esi + 0xc], edx
        push   esi
        call   ecx
        pop    ecx
loc_11_0153:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_11_0157:
        mov    eax, dword ptr [0x5c]         ; -> obj6:0x005C
        mov    ecx, dword ptr [0x60]         ; -> obj6:0x0060
        mov    dword ptr [eax], ecx
        ret    
loc_11_0165:
        cmp    byte ptr [0x1c], 0            ; -> obj6:0x001C
        je     loc_11_01A3
        mov    ecx, dword ptr [0x28]         ; -> obj6:0x0028
        jecxz  loc_11_01A3
        mov    esi, dword ptr [0x20]         ; -> obj6:0x0020
        or     esi, esi
        je     loc_11_01A3
        cmp    dword ptr [0x24], 0           ; -> obj6:0x0024
        je     loc_11_01A3
loc_11_0189:
        test   byte ptr [esi + 0x44], 0x40
        je     loc_11_019B
        test   byte ptr [esi + 0x44], 0xa0
        jne    loc_11_019B
        mov    word ptr [esi + 0x49], 0xffff
loc_11_019B:
        add    esi, dword ptr [0x24]         ; -> obj6:0x0024
        loop   loc_11_0189
loc_11_01A3:
        clc    
        ret    
loc_11_01A5:
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x19
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ch, byte ptr [ebp + 0x1c]
        mov    byte ptr [ebp + 0x14], 0
loc_11_01CC:
        mov    byte ptr [ebp + 0x1d], 0xe
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    cl, byte ptr [ebp + 0x1c]
        mov    byte ptr [ebp + 0x1d], 0xd
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        inc    byte ptr [ebp + 0x14]
        cmp    byte ptr [ebp + 0x14], cl
        jb     loc_11_01CC
        mov    byte ptr [ebp + 0x14], ch
        mov    byte ptr [ebp + 0x1d], 0xe
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        clc    
        ret    
        db  000h
loc_11_020C:
        ja     0x277
        outsb  dx, byte ptr [esi]
        db  064h, 069h, 072h, 000h
sub_11_0213:
        pushal 
        shl    ecx, 4
        add    ecx, dword ptr [ebx + 4]
        xor    esi, esi
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        mov    edi, edx
        mov    esi, 0x20c                    ; -> loc_11_020C
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        jb     loc_11_024C
        sub    edx, edi
        sub    edx, 7
        call   sub_14_036F
        lea    edi, [edx + ecx]
        push   edi
        sub    eax, edx
        mov    ecx, eax
        xor    eax, eax
        repne scasb al, byte ptr es:[edi]
        mov    esi, edi
        pop    edi
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_11_024C:
        popal  
        ret    
loc_11_024E:
        mov    edi, dword ptr [0x10]         ; -> obj6:0x0010
        add    edi, dword ptr [ebx + 4]
        xor    esi, esi
        xchg   dword ptr [0x3c], esi         ; -> obj6:0x003C
        push   0x174
        push   esi
        push   edi
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    esi, dword ptr [ebx + 8]
        mov    byte ptr [esi + 0x1d], 0x48
        or     word ptr [esi + 0x10], 0xffff
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        jmp    sub_14_086C
        db  000h
loc_11_02A4:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [edx + ebx], 0x10
        jne    loc_11_02B6
        mov    byte ptr [ebp + 0x1c], 3
        clc    
        ret    
loc_11_02B6:
        mov    eax, 0x24
        VMMCall  VMM.Get_Instanced_V86_Int_Vector ; cd2041010100
        cmp    eax, dword ptr [0x44]         ; -> obj6:0x0044
        je     loc_11_02D5
        push   ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        pop    ebx
        or     ecx, ecx
        jne    loc_11_02D7
loc_11_02D5:
        stc    
        ret    
loc_11_02D7:
        mov    eax, 0xfa
        mov    esi, 0x2e9                    ; -> loc_11_02E9
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        jmp    loc_11_02D5
loc_11_02E9:
        jae    loc_11_02F7
        push   ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        pop    ebx
        jecxz  loc_11_02F7
        jmp    sub_11_02F8
loc_11_02F7:
        ret    
sub_11_02F8:
        mov    eax, 0xf
        xor    edx, edx
        xor    esi, esi
        VMMCall  VMM.System_Control              ; cd2093000100
        cmp    byte ptr [0xe0], 0            ; -> obj6:0x00E0
        je     loc_11_031A
        cmp    ebx, dword ptr [0xdc]         ; -> obj6:0x00DC
        jne    loc_11_0320
        jmp    loc_11_0339
loc_11_031A:
        inc    byte ptr [0xe0]               ; -> obj6:0x00E0
loc_11_0320:
        mov    dword ptr [0xdc], ebx         ; -> obj6:0x00DC
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    edx, eax
        mov    esi, 0x33b                    ; -> loc_11_033B
        VMMCall  VMM.Call_When_Not_Critical      ; cd2024000100
loc_11_0339:
        stc    
        ret    
loc_11_033B:
        mov    byte ptr [0xe0], 0            ; -> obj6:0x00E0
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, edx
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
        ret    
        db  000h, 000h, 000h
loc_11_0354:
        pushal 
        mov    ax, word ptr [ebp + 0x3c]
        mov    edx, dword ptr [ebp + 0x14]
        call   0xfff50568                    ; -> obj5:0x0568
        jae    loc_11_03CE
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 0x18]
        test   word ptr [ebx], 0x180
        jne    loc_11_0372
        movzx  ecx, cx
loc_11_0372:
        dec    ecx
        and    eax, 0xfff
        add    ecx, eax
        shr    ecx, 0xc
        inc    ecx
        mov    eax, esi
        shr    eax, 0xc
        push   0
        push   ecx
        push   eax
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        je     loc_11_03CB
        mov    ecx, dword ptr [ebp + 0x18]
        shl    esi, 0xc
        shr    si, 0xc
        xchg   word ptr [ebp + 0x14], si
        ror    esi, 0x10
        xchg   word ptr [ebp + 0x60], si
        call   sub_12_04B0
        mov    word ptr [ebp + 0x60], si
        shr    esi, 0x10
        mov    word ptr [ebp + 0x14], si
        mov    dword ptr [ebp + 0x18], ecx
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        popal  
        ret    
loc_11_03CB:
        add    esp, 0xc
loc_11_03CE:
        popal  
        mov    edx, 0x130                    ; -> obj6:0x0130
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_11_03DA:
        add    dword ptr [0x3c1400], eax
        db  021h
loc_11_03E1:
        add    dword ptr [edi], ecx
        adc    al, 0x3c
        add    byte ptr [ecx], ah
loc_11_03E8:
        add    dword ptr [ecx + eax], eax
        adc    al, 0x3c
        add    byte ptr [ecx], ah
loc_11_03EF:
        add    eax, 0xcd                     ; -> loc_13_00CD
        add    edi, ebp                      ; -> loc_11_03FD
        add    eax, dword ptr [eax]
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_11_03FD:
        cmp    byte ptr [ebp + 0x19], 8
        je     loc_11_040F
        movzx  ecx, word ptr fs:[esi + 0x5c]
        shl    ecx, 2
        add    ecx, 0x5e
        ret    
loc_11_040F:
        movzx  ecx, word ptr fs:[esi + 0x26]
        shl    ecx, 2
        add    ecx, 0x28
        ret    
loc_11_041B:
        cmp    byte ptr [ebp + 0x19], 8
        je     loc_11_0427
        mov    ecx, 0x5c
        ret    
loc_11_0427:
        mov    ecx, 0x26
        ret    
sub_11_042D:
        add    esi, dword ptr [esi]          ; -> loc_11_0436
        add    al, 0
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_11_0436:
        movzx  ecx, byte ptr fs:[esi]
        ret    
loc_11_043B:
        add    eax, 0x440                    ; -> loc_11_0440
loc_11_0440:
        pushal 
        push   fs
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x60]
        movzx  edx, byte ptr [ebp + 0x10]
        call   sub_11_09D3
        jb     loc_11_057B
        push   eax
        push   edx
        push   ecx
        mov    ax, 0x3c14
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    ecx, 0xd
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_11_0578
        mov    word ptr [ebp + 0x14], di
        movzx  edx, di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x60], di
        shl    edi, 4
        add    edi, edx
        movzx  eax, word ptr [edi + 7]
        mul    dword ptr [esp]
        mov    ecx, eax
        test   word ptr [ebx], 0x180
        jne    loc_11_04A6
        movzx  edx, word ptr fs:[esi + 9]
        mov    ax, word ptr fs:[esi + 0xb]
        jmp    loc_11_04AF
loc_11_04A6:
        mov    edx, dword ptr fs:[esi + 9]
        mov    ax, word ptr fs:[esi + 0xd]
loc_11_04AF:
        call   0xfff50568                    ; -> obj5:0x0568
        mov    esi, eax
        jae    loc_11_0509
        push   eax
        push   ecx
        dec    ecx
        and    eax, 0xfff
        add    ecx, eax
        shr    ecx, 0xc
        inc    ecx
        mov    eax, esi
        shr    eax, 0xc
        push   0
        push   ecx
        push   eax
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        jne    loc_11_04E7
        add    esp, 0xc
        pop    ecx
        pop    eax
        jmp    loc_11_0509
loc_11_04E7:
        shl    esi, 0xc
        shr    si, 0xc
        mov    word ptr [edi + 9], si
        shr    esi, 0x10
        mov    word ptr [edi + 0xb], si
        call   sub_12_04B0
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0x14
        jmp    loc_11_055F
loc_11_0509:
        mov    ax, ds
        mov    fs, ax
        VxDCall  V86MMGR.V86MMGR_Get_Xlat_Buff_State ; cd200a000600
        mov    eax, ecx
        xor    edx, edx
        div    dword ptr [esp]
        movzx  ecx, word ptr [edi + 7]
loc_11_0520:
        mov    edx, ecx
        cmp    ecx, eax
        jbe    loc_11_0528
        mov    edx, eax
loc_11_0528:
        call   sub_11_058F
        jb     loc_11_055F
        sub    ecx, edx
        je     loc_11_055F
        add    word ptr [edi + 5], dx
loc_11_0537:
        movzx  edx, word ptr [edi + 5]
        sub    edx, dword ptr [esp + 8]
        jb     loc_11_0520
        mov    word ptr [edi + 5], dx
        inc    word ptr [edi + 1]
        mov    edx, dword ptr [esp + 4]
        cmp    word ptr [edi + 1], dx
        jb     loc_11_0537
        mov    word ptr [edi + 1], 0
        inc    word ptr [edi + 3]
        jmp    loc_11_0537
loc_11_055F:
        mov    ecx, 0xd
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        add    esp, 0xc
        pop    dword ptr [ebp + 0x60]
        pop    dword ptr [ebp + 0x14]
        pop    fs
        popal  
        ret    
loc_11_0578:
        add    esp, 0xc
loc_11_057B:
        pop    dword ptr [ebp + 0x60]
        pop    dword ptr [ebp + 0x14]
        pop    fs
        popal  
        mov    word ptr [ebp + 0x1c], 2
        or     byte ptr [ebp + 0x2c], 1
        ret    
sub_11_058F:
        push   eax
        push   ecx
        push   dword ptr [ebp + 0x1c]
        mov    dword ptr [edi + 7], edx
        mov    eax, edx
        push   edx
        mul    dword ptr [esp + 0x14]
        pop    edx
        mov    ecx, eax
        push   edi
        bt     dword ptr [ebp + 0x18], 5
        cmc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_11_05D9
        mov    eax, edi
        pop    edi
        mov    dword ptr [edi + 9], eax
        call   sub_12_04B0
        bt     dword ptr [ebp + 0x18], 5
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        add    esi, ecx
loc_11_05C8:
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_11_05D5
        pop    dword ptr [ebp + 0x1c]
        jmp    loc_11_05D6
loc_11_05D5:
        pop    ecx
loc_11_05D6:
        pop    ecx
        pop    eax
        ret    
loc_11_05D9:
        pop    edi
        or     byte ptr [ebp + 0x2c], 1
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_11_05C8
        db  000h, 000h
loc_11_05E8:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        movzx  eax, word ptr [ebp + 8]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 4]
        mov    dword ptr [ebp + 4], ecx
        add    ecx, 0x12
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        mov    word ptr [ebp + 8], cx
        sub    dword ptr [ebp + 0x30], 0x18
        xchg   edi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    ecx, 6
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, 0x24
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        add    dword ptr [ebp + 0x30], 0x18
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    eax, dword ptr [ebp + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    al, 2
        jne    loc_11_0669
        mov    al, 3
loc_11_0669:
        mov    byte ptr [ebp + 0x1c], al
        ret    
loc_11_066D:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, 0x24
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        je     loc_11_06D5
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_11_05E8
        mov    esi, dword ptr [0x1f4]        ; -> obj1:0x01F4
        test   esi, esi
        je     loc_11_06D5
        mov    edx, dword ptr [esi + edi]
        test   edx, edx
        je     loc_11_06D5
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [edx + 2], ax
        mov    ax, word ptr [ebp]
        mov    word ptr [edx + 4], ax
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [edx + 6], ax
        mov    ax, word ptr [ebp + 4]
        mov    word ptr [edx + 8], ax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    ah, byte ptr [eax + 1]
        mov    al, 0xff
        mov    word ptr [edx], ax
loc_11_06D5:
        mov    esi, dword ptr [0x4d4]        ; -> obj6:0x04D4
        mov    ecx, edi
        lea    edi, [edi + 0x33]
        cld    
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x30
        xor    eax, eax
        mov    al, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    byte ptr [ebp + 0x1d], 0x82
        mov    al, 0x2a
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jecxz  loc_11_0725
        cmp    byte ptr [0x1e], 0            ; -> obj6:0x001E
        je     loc_11_0725
        mov    word ptr [ebp + 0x1c], 0x5d07
        xor    eax, eax
        mov    al, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_11_0725:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edi, ecx
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x1c], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x10], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x18], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x14], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 4], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 8], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x3c], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x38], ax
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        movzx  eax, word ptr [edi + 0x34]
        movzx  ecx, byte ptr [ebp + 0x1d]
        push   eax
        push   ecx
        VxDCall  IFSMgr._GetMappedErr            ; cd206d004000
        add    esp, 8
        mov    word ptr [ebp + 0x1c], ax
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_11_07A9:
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        xchg   edi, eax
        jecxz  loc_11_07C2
        cmp    eax, edi
        jne    loc_11_07C2
        mov    byte ptr [ebp + 0x1c], 3
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
loc_11_07C2:
        mov    cx, word ptr [0x142]          ; -> obj6:0x0142
        mov    edx, dword ptr [0x13e]        ; -> obj6:0x013E
        mov    word ptr [ebp + 0x28], cx
        mov    dword ptr [ebp + 0x24], edx
        ret    
loc_11_07D7:
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        push   fs
        push   edx
        movzx  edx, byte ptr [ebp + 0x1c]
        inc    edx
        call   sub_11_09D3
        jae    loc_11_07F6
        mov    ecx, 0x200
loc_11_07F6:
        mov    ax, 0x3c10
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        push   ecx
        VxDCall  V86MMGR.V86MMGR_Get_Xlat_Buff_State ; cd200a000600
        lea    eax, [ecx - 0xa]
        cmp    eax, 0x1000
        jbe    loc_11_0816
        mov    eax, 0x1000
loc_11_0816:
        xor    edx, edx
        pop    ecx
        div    ecx
        mov    word ptr [0x138], ax          ; -> obj6:0x0138
        pop    eax
        push   dword ptr [ebp + 0x60]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp]
        push   dword ptr [ebp + 8]
        cmp    word ptr [ebp + 0x18], -1
        je     loc_11_0887
loc_11_0841:
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        mov    dx, word ptr [0x138]          ; -> obj6:0x0138
        cmp    dx, word ptr [ebp + 0x18]
        jae    loc_11_085B
        mov    word ptr [ebp + 0x18], dx
loc_11_085B:
        mov    edx, dword ptr [ebp + 0x18]
        call   sub_11_0954
        jb     loc_11_087E
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x18]
        pop    dword ptr [ebp + 0x1c]
        add    word ptr [ebp + 0x14], dx
        sub    word ptr [ebp + 0x18], dx
        ja     loc_11_0841
        clc    
        jmp    loc_11_091C
loc_11_087E:
        add    esp, 0xc
        stc    
        jmp    loc_11_091C
loc_11_0887:
        push   ecx
        mov    ecx, 0xa
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        pop    ecx
        jb     loc_11_0945
        mov    word ptr [ebp + 0x10], di
        movzx  edx, di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x60], di
        shl    edi, 4
        add    edx, edi
        movzx  edi, word ptr fs:[esi + 4]
        push   fs
        push   esi
        push   eax
        mov    ax, word ptr fs:[esi + 8]
        movzx  esi, word ptr fs:[esi + 6]
        mov    fs, ax
        pop    eax
loc_11_08C5:
        push   eax
        mov    ax, word ptr [0x138]          ; -> obj6:0x0138
        cmp    ax, word ptr [edx + 4]
        jae    loc_11_08D6
        mov    word ptr [edx + 4], ax
loc_11_08D6:
        pop    eax
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x10]
        mov    word ptr [ebp + 0x18], 0xffff
        call   sub_11_0954
        jb     loc_11_0905
        pop    dword ptr [ebp + 0x10]
        pop    dword ptr [ebp + 0x1c]
        push   eax
        movzx  eax, word ptr [edx + 4]
        sub    edi, eax
        je     loc_11_090A
        cmc    
        jae    loc_11_090A
        add    dword ptr [edx], eax
        mov    word ptr [edx + 4], di
        pop    eax
        jmp    loc_11_08C5
loc_11_0905:
        add    esp, 8
        push   eax
        stc    
loc_11_090A:
        pop    eax
        pop    esi
        pop    fs
        pushfd 
        mov    ecx, 0xa
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        popfd  
loc_11_091C:
        pop    dword ptr [ebp + 8]
        pop    dword ptr [ebp]
        pop    dword ptr [ebp + 4]
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x18]
        pop    dword ptr [ebp + 0x10]
        pop    eax
        pop    dword ptr [ebp + 0x60]
        jb     loc_11_0937
        mov    dword ptr [ebp + 0x1c], eax
loc_11_0937:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        pop    fs
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
loc_11_0945:
        or     word ptr [ebp + 0x2c], 1
        mov    dword ptr [ebp + 0x1c], 0x204
        stc    
        jmp    loc_11_091C
sub_11_0954:
        push   eax
        push   edi
        push   ecx
        push   eax
        push   edx
        mov    eax, ecx
        cmp    word ptr [ebp + 0x18], -1
        jne    loc_11_0968
        mul    word ptr [edx + 4]
        jmp    loc_11_096C
loc_11_0968:
        mul    word ptr [ebp + 0x18]
loc_11_096C:
        pop    edx
        mov    ecx, eax
        pop    eax
        bt     eax, 0
        cmc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_11_09C4
        cmp    word ptr [ebp + 0x18], -1
        jne    loc_11_0989
        mov    dword ptr [edx + 6], edi
        jmp    loc_11_0994
loc_11_0989:
        mov    word ptr [ebp + 0x10], di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x60], di
loc_11_0994:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        add    word ptr [ebp + 0x30], 2
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        bt     eax, 0
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_11_09C0
        add    esi, ecx
        clc    
loc_11_09C0:
        pop    ecx
        pop    edi
        pop    eax
        ret    
loc_11_09C4:
        or     word ptr [ebp + 0x2c], 1
        mov    dword ptr [ebp + 0x1c], 0x204
        stc    
        jmp    loc_11_09C0
sub_11_09D3:
        push   edi
        push   esi
        push   fs
        cmp    byte ptr [0x164], 0           ; -> obj6:0x0164
        je     loc_11_09FE
        or     dl, dl
        je     loc_11_09FE
        mov    esi, 0x144                    ; -> obj6:0x0144
        cmp    byte ptr [esi], dl
        je     loc_11_0A92
        mov    esi, 0x154                    ; -> obj6:0x0154
        cmp    byte ptr [esi], dl
        je     loc_11_0A92
loc_11_09FE:
        mov    ecx, 0x28
        mov    ax, ds
        mov    fs, ax
        xor    esi, esi
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_11_0A8D
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        movzx  eax, di
        mov    word ptr [ebp + 0x14], di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x3c], di
        shl    edi, 4
        add    edi, eax
        mov    ax, 0x440d
        xchg   word ptr [ebp + 0x1c], ax
        mov    byte ptr [ebp + 0x10], dl
        mov    word ptr [ebp + 0x18], 0x860
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        call   sub_13_00EC
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_11_0A9D
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        movzx  eax, word ptr [edi + 0x14]
        movzx  edx, word ptr [edi + 0x16]
        movzx  esi, word ptr [edi + 7]
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        mov    ecx, esi
loc_11_0A8D:
        pop    fs
        pop    esi
        pop    edi
        ret    
loc_11_0A92:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0xc]
        jmp    loc_11_0A8D
loc_11_0A9D:
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        stc    
        jmp    loc_11_0A8D
        db  000h, 000h
sub_11_0AB8:
        push   eax
        push   ecx
        mov    ecx, dword ptr [0x17c]        ; -> obj6:0x017C
        mov    eax, dword ptr [0x180]        ; -> obj6:0x0180
        jecxz  loc_11_0AC9
        mov    dword ptr [ecx], eax
loc_11_0AC9:
        pop    ecx
        pop    eax
        clc    
        ret    
DOSMGR_Add_Device:
        pushal 
        mov    esi, eax
        shl    esi, 0xc
        rol    si, 4
        or     ebx, ebx
        jne    loc_11_0AE1
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
loc_11_0AE1:
        add    eax, dword ptr [ebx + 4]
        mov    edi, eax
        test   byte ptr [edi + 5], 0x80
        stc    
        je     loc_11_0B1B
        mov    edx, dword ptr [0x17c]        ; -> obj6:0x017C
        or     edx, edx
        jne    loc_11_0AFC
        call   sub_4_1214
loc_11_0AFC:
        add    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [edx]
        cmp    ax, -1
        je     loc_11_0B16
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        mov    edx, eax
        jmp    loc_11_0AFC
loc_11_0B16:
        or     dword ptr [edi], 0xffffffff
        mov    dword ptr [edx], esi
loc_11_0B1B:
        popal  
        ret    
DOSMGR_Remove_Device:
        pushal 
        or     ebx, ebx
        je     loc_11_0B25
        add    eax, dword ptr [ebx + 4]
loc_11_0B25:
        mov    edi, eax
        test   byte ptr [edi + 5], 0x80
        je     loc_11_0B8F
        mov    edx, dword ptr [0x17c]        ; -> obj6:0x017C
        or     edx, edx
        je     loc_11_0B8F
        or     ebx, ebx
        je     loc_11_0B3E
        add    edx, dword ptr [ebx + 4]
loc_11_0B3E:
        mov    eax, dword ptr [edx]
        cmp    ax, -1
        je     loc_11_0B8F
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        or     ebx, ebx
        je     loc_11_0B58
        add    eax, dword ptr [ebx + 4]
loc_11_0B58:
        cmp    eax, edi
        je     loc_11_0B60
        mov    edx, eax
        jmp    loc_11_0B3E
loc_11_0B60:
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        or     ebx, ebx
        jne    loc_11_0B8C
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        jmp    loc_11_0B7E
loc_11_0B70:
        mov    ecx, edi
        mov    esi, edx
        add    ecx, dword ptr [ebx + 4]
        add    esi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
loc_11_0B7E:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_11_0B70
loc_11_0B8C:
        clc    
loc_11_0B8D:
        popal  
        ret    
loc_11_0B8F:
        stc    
        jmp    loc_11_0B8D
        db  000h, 000h
sub_11_0B94:
        mov    eax, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        xchg   edi, eax
        jecxz  loc_11_0BA6
        cmp    eax, edi
        jne    loc_11_0BA6
        mov    al, 3
        ret    
loc_11_0BA6:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        lea    esi, [edx + 0xc]
        push   edi
        mov    edi, ebp
        mov    ecx, 8
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    edi, 0x18
        add    esi, 0x18
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        mov    ax, word ptr [ebp + 0x38]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x3c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 4]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x14]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x10]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x1c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebp + 0x1c], ax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ebp], ax
        movzx  ecx, word ptr [edx + 8]
        movzx  eax, word ptr [edx + 6]
        shl    eax, 4
        mov    dword ptr [ebp + 4], ecx
        add    ecx, 0x12
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        mov    word ptr [ebp + 8], cx
        mov    eax, dword ptr [ebp + 0x2c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    cx, word ptr [0x142]          ; -> obj6:0x0142
        mov    edx, dword ptr [0x13e]        ; -> obj6:0x013E
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 1
        jne    loc_11_0C7E
        add    esp, 0x6c
        ret    
loc_11_0C7E:
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        ret    
loc_11_0C8A:
        push   fs
        push   eax
        push   esi
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x14]
        mov    eax, 0x3c14
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    byte ptr [ebp + 0x1d], 2
loc_11_0CA3:
        mov    al, byte ptr fs:[esi]
        cmp    al, 0x24
        je     loc_11_0CB5
        mov    byte ptr [ebp + 0x14], al
        call   sub_12_04B0
        inc    esi
        jmp    loc_11_0CA3
loc_11_0CB5:
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x1c]
        pop    esi
        pop    eax
        pop    fs
        ret    
loc_11_0CC0:
        movzx  ecx, byte ptr fs:[esi]
        add    ecx, 2
        ret    
loc_11_0CC8:
        pushal 
        movzx  eax, byte ptr [edx - 6]
        movzx  edi, word ptr [eax + ebp + 0x24]
        shl    edi, 4
        movzx  eax, byte ptr [edx - 7]
        movzx  eax, word ptr [eax + ebp]
        add    edi, eax
        push   0x100
        push   0
        push   1
        push   edi
        push   edi
        VxDCall  IFSMgr.BcsToBcsUpper            ; cd2074004000
        add    esp, 0x14
        popal  
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_11_0CFA:
        push   edx
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        pop    edx
        pushal 
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_11_0D33
        movzx  eax, byte ptr [edx - 6]
        movzx  edi, word ptr [eax + ebp + 0x24]
        shl    edi, 4
        movzx  eax, byte ptr [edx - 7]
        movzx  eax, word ptr [eax + ebp]
        add    edi, eax
        movzx  ecx, word ptr [edx - 9]
        push   ecx
        push   1
        push   0
        push   edi
        push   edi
        VxDCall  IFSMgr.BcsToBcs                 ; cd2070004000
        add    esp, 0x14
loc_11_0D33:
        popal  
        clc    
        ret    
loc_11_0D36:
        mov    edx, 0x310                    ; -> obj6:0x0310
        cmp    word ptr [ebp + 0x14], -1
        je     loc_11_0D47
        mov    edx, 0x47d                    ; -> obj6:0x047D
loc_11_0D47:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_11_0D4D:
        mov    edx, 0x433                    ; -> obj6:0x0433
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_11_0D5D
        mov    edx, 0x470                    ; -> obj6:0x0470
loc_11_0D5D:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_11_0D63:
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 7
        mov    edx, 0x433                    ; -> obj6:0x0433
        jbe    loc_11_0D88
        and    al, 0x7f
        cmp    al, 0x21
        mov    edx, 0x439                    ; -> obj6:0x0439
        je     loc_11_0D88
        cmp    al, 0x22
        mov    edx, 0x358                    ; -> obj6:0x0358
        je     loc_11_0D88
        mov    edx, 0x310                    ; -> obj6:0x0310
loc_11_0D88:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_11_0D8E:
        test   word ptr [ebx], 0x180
        jne    loc_11_0DA8
        movzx  edi, word ptr [ebp + 0x10]
        mov    dword ptr [ebp + 0x10], edi
        cmp    di, -1
        je     loc_11_0E89
        jmp    loc_11_0DB4
loc_11_0DA8:
        mov    edi, dword ptr [ebp + 0x10]
        cmp    edi, -1
        je     loc_11_0E89
loc_11_0DB4:
        shl    edi, 4
        add    edi, 0xa
        shr    edi, 0xc
        inc    edi
        push   0
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   edi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, edx
        je     loc_11_0E89
        mov    dword ptr [edx], eax
        VMMCall  VMM.Get_Cur_PM_App_CB           ; cd2088000100
        add    edi, dword ptr [4]            ; -> obj6:0x0004
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edi], edx
        add    edx, 0xa
        mov    esi, edx
        mov    edi, dword ptr [ebp + 0x10]
        shl    edi, 4
        dec    edi
        mov    ecx, edi
        call   sub_11_10C2
        mov    ecx, 1
        test   word ptr [ebx], 0x180
        jne    loc_11_0E17
        mov    ecx, edi
        shr    ecx, 0x10
        inc    ecx
loc_11_0E17:
        push   ecx
        push   0
        push   ecx
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        pop    ecx
        test   eax, eax
        je     loc_11_0E79
        mov    word ptr [ebp + 0x1c], ax
        push   esi
        sub    esi, 0xa
        mov    word ptr [esi + 8], ax
        pop    esi
        and    edi, 0xffff
loc_11_0E3F:
        dec    ecx
        je     loc_11_0E74
        add    eax, 8
        add    esi, 0x10000
        push   ecx
        cmp    ecx, 1
        mov    ecx, edi
        je     loc_11_0E58
        mov    ecx, 0xffff
loc_11_0E58:
        push   eax
        call   sub_11_10C2
        mov    ecx, dword ptr [esp]
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        pop    eax
        pop    ecx
        jmp    loc_11_0E3F
loc_11_0E74:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_11_0E79:
        sub    esi, 4
        push   0
        push   dword ptr [esi]
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_11_0E89:
        push   0
        VMMCall  VMM._PageGetAllocInfo           ; cd2059000100
        add    esp, 4
        test   eax, eax
        je     loc_11_0EB1
        shl    eax, 8
        sub    eax, 1
        test   word ptr [ebx], 0x180
        jne    loc_11_0EB1
        cmp    eax, 0xffff
        jb     loc_11_0EB1
        mov    eax, 0xfffe
loc_11_0EB1:
        mov    dword ptr [ebp + 0x10], eax
        mov    word ptr [ebp + 0x1c], 8
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_11_0EBF:
        xor    edi, edi
        xchg   word ptr [ebp + 0x38], di
        test   di, 4
        je     loc_11_0F26
sub_11_0ECC:
        lea    esi, [ebp + 0x3c]
        mov    ecx, 3
loc_11_0ED4:
        cmp    word ptr [esi], di
        jne    loc_11_0EDE
        mov    word ptr [esi], 0
loc_11_0EDE:
        add    esi, 4
        loop   loc_11_0ED4
        call   sub_11_1072
        jb     loc_11_0F26
        push   0
        push   dword ptr [edx]
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        or     eax, eax
        je     loc_11_0F26
        mov    esi, 1
        test   word ptr [ebx], 0x180
        jne    loc_11_0F0E
        lsl    esi, edi
        shr    esi, 0x10
        inc    esi
loc_11_0F0E:
        push   0
        push   edi
        push   ebx
        VMMCall  VMM._Free_LDT_Selector          ; cd2079000100
        add    esp, 0xc
        add    edi, 8
        dec    esi
        jne    loc_11_0F0E
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_11_0F26:
        mov    word ptr [ebp + 0x1c], 9
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_11_0F31:
        movzx  edi, word ptr [ebp + 0x38]
        call   sub_11_1072
        jb     loc_11_102C
        mov    esi, eax
        test   word ptr [ebx], 0x180
        jne    loc_11_0F83
        lsl    ecx, edi
        shr    ecx, 0x10
        movzx  edx, word ptr [ebp + 0x10]
        mov    dword ptr [ebp + 0x10], edx
        shr    edx, 0xc
        push   ecx
        sub    ecx, edx
        pop    edx
        jb     loc_11_102C
        je     loc_11_0F83
        lea    edx, [edi + edx*8]
loc_11_0F68:
        push   edx
        push   0
        push   edx
        push   ebx
        VMMCall  VMM._Free_LDT_Selector          ; cd2079000100
        add    esp, 0xc
        pop    edx
        je     loc_11_102C
        sub    edx, 8
        dec    ecx
        jne    loc_11_0F68
loc_11_0F83:
        sub    esi, 0xa
        mov    ecx, dword ptr [ebp + 0x10]
        shl    ecx, 4
        push   ecx
        add    ecx, 0xa
        shr    ecx, 0xc
        inc    ecx
        push   0
        push   ecx
        push   dword ptr [esi]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        test   eax, edx
        pop    ecx
        je     loc_11_102C
        dec    ecx
        mov    dword ptr [edx], eax
        push   edi
        push   ecx
        VMMCall  VMM.Get_Cur_PM_App_CB           ; cd2088000100
        add    edi, dword ptr [4]            ; -> obj6:0x0004
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edi], edx
        pop    ecx
        pop    edi
        add    edx, 0xa
        mov    esi, edx
        push   ecx
        call   sub_11_10C2
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        pop    ecx
        test   word ptr [ebx], 0x180
        jne    loc_11_1027
        shr    ecx, 0x10
        je     loc_11_1027
loc_11_0FEC:
        add    edi, 8
        add    esi, 0x10000
        push   ecx
        cmp    ecx, 1
        je     loc_11_1002
        mov    ecx, 0xffff
        jmp    loc_11_100F
loc_11_1002:
        mov    ecx, dword ptr [ebp + 0x10]
        shl    ecx, 4
        dec    ecx
        and    ecx, 0xffff
loc_11_100F:
        call   sub_11_10C2
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        pop    ecx
        dec    ecx
        jne    loc_11_0FEC
loc_11_1027:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_11_102C:
        push   0
        VMMCall  VMM._GetFreePageCount           ; cd205a000100
        add    esp, 4
        shl    eax, 8
        test   word ptr [ebx], 0x180
        jne    loc_11_1064
        cmp    eax, 0xffff
        jb     loc_11_104D
        mov    eax, 0xfffe
loc_11_104D:
        test   word ptr [ebx], 0x180
        jne    loc_11_1064
        lsl    ecx, edi
        mov    cx, 0xffff
        shr    ecx, 4
        cmp    ecx, eax
        ja     loc_11_1064
        mov    eax, ecx
loc_11_1064:
        mov    dword ptr [ebp + 0x10], eax
        mov    word ptr [ebp + 0x1c], 8
        or     byte ptr [ebp + 0x2c], 1
        ret    
sub_11_1072:
        push   0
        push   edi
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        push   eax
        sub    eax, 0xa
        mov    edx, eax
        push   edi
        VMMCall  VMM.Get_Cur_PM_App_CB           ; cd2088000100
        mov    eax, edi
        pop    edi
        add    eax, dword ptr [4]            ; -> obj6:0x0004
        cmp    dword ptr [eax], 0
        je     loc_11_10BF
        cmp    dword ptr [eax], edx
        je     loc_11_10AE
        mov    eax, dword ptr [eax]
loc_11_10A0:
        cmp    dword ptr [eax + 4], edx
        je     loc_11_10B6
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_11_10A0
        jmp    loc_11_10BF
loc_11_10AE:
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        clc    
        jmp    loc_11_10BD
loc_11_10B6:
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], ecx
        clc    
loc_11_10BD:
        pop    eax
        ret    
loc_11_10BF:
        stc    
        jmp    loc_11_10BD
sub_11_10C2:
        cmp    ecx, 0xfffff
        ja     loc_11_10DF
        push   0
        push   0
        push   0x92
        push   ecx
        push   esi
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        ret    
loc_11_10DF:
        add    ecx, 0xfff
        shr    ecx, 0xc
        push   0
        push   0x80
        push   0x92
        push   ecx
        push   esi
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        ret    
loc_11_1100:
        mov    ecx, 0x13
        cmp    byte ptr [0x1a4], 4           ; -> obj6:0x01A4
        jb     loc_11_1111
        add    ecx, 6
loc_11_1111:
        ret    
loc_11_1112:
        cmp    byte ptr [ebp + 0x1c], 0
        jne    loc_11_111A
        jmp    loc_11_1123
loc_11_111A:
        or     byte ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], 1
        ret    
loc_11_1123:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        push   0
        xchg   esi, eax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        push   esi
        mov    ax, 0x3810
        push   fs
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        test   word ptr [ebx], 0x180
        jne    loc_11_1164
        mov    eax, dword ptr fs:[esi + 2]
        movzx  esi, ax
        shr    eax, 0x10
        mov    fs, ax
        jmp    loc_11_1168
loc_11_1164:
        lfs    esi, ptr fs:[esi]
loc_11_1168:
        push   0
        push   fs
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    fs
        add    eax, esi
        push   eax
        push   ebx
        VxDCall  DOSMGR._DOSMGR_Set_Exec_VM_Data ; cd2001001500
        add    esp, 0x10
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
        lea    eax, [edx + 0xb0]
        shl    eax, 0xc
        shr    ax, 0xc
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x60], ax
        lea    eax, [edx + 0x20]
        shl    eax, 0xc
        shr    ax, 0xc
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x5c], ax
        call   sub_15_02AB
        call   sub_12_04B0
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        push   fs
        mov    fs, word ptr [edi + 0x2a]
        mov    word ptr fs:[0x2c], ax
        pop    fs
        xor    eax, eax
        mov    dword ptr [ebx + 0x3c], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x2c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    word ptr [ebp + 0x1c], ax
        and    byte ptr [ebp + 0x2c], 0xfe
        and    ecx, 1
        je     loc_11_1201
        or     dword ptr [ebp + 0x2c], ecx
loc_11_1201:
        popal  
        ret    
DOSMGR_Translate_Server_DOS_Call:
        sub    esp, 0x84
        pushal 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ebp, dword ptr [ebx + 8]
        xor    ecx, ecx
        mov    dword ptr [esp + 0x20], ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        cmp    dword ptr [0], ecx            ; -> sub_9_0000
        jne    loc_11_1253
        call   sub_11_1391
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        je     loc_11_137E
        push   esi
        mov    esi, 0x10                     ; -> loc_9_0010
        mov    eax, 0x10084
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        pop    esi
loc_11_1253:
        push   0x16
        pop    ecx
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jae    loc_11_1261
        xor    ecx, ecx
loc_11_1261:
        cmp    ecx, 0x16
        jb     loc_11_1353
        mov    dword ptr [esp + 0x30], edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Set_PM_Exec_Mode            ; cd208a000100
        VMMCall  VMM.Begin_Use_Locked_PM_Stack   ; cd208b000100
        or     byte ptr [ebx + 1], 1
        mov    byte ptr [ebp + 0x1d], 0x2f
        xor    eax, eax
        mov    al, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        push   dword ptr [ebp + 0x38]
        push   dword ptr [ebp + 0x10]
        mov    edx, dword ptr [esi + 0x20]
        mov    dword ptr [ebp + 0x14], edx
        mov    word ptr [ebp + 0x3c], ds
        mov    byte ptr [ebp + 0x1d], 0x1a
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cld    
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x1c], eax
        mov    dword ptr [esp + 0x2c], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x10], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x18], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x14], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 4], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp], eax
        VMMCall  VMM._GetAppFlatDSAlias          ; cd206d000100
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x40], ax
        mov    word ptr [ebp + 0x44], ax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [esp + 0x30], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [esp + 0x34], eax
        mov    dword ptr [esp + 0x3c], 0x65  ; -> loc_9_0065
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [4], edi            ; -> loc_9_0004
        lea    eax, [esp + 0x2c]
        mov    dword ptr [ebp + 8], eax
        call   sub_10_01B2
        inc    dword ptr [esp + 0x28]
        inc    dword ptr [4]                 ; -> loc_9_0004
        inc    dword ptr [esp + 0x3c]
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [ebp + 8], eax
        lea    edi, [esp + 0x40]
        mov    esi, edi
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x3c]
        mov    byte ptr [ebp + 0x1d], 0x1a
        xor    eax, eax
        mov    al, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        VMMCall  VMM.End_Use_Locked_PM_Stack     ; cd208c000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   0x16
        pop    ecx
loc_11_1353:
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        mov    esi, 0x10                     ; -> loc_9_0010
        mov    eax, 0x10084
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        jb     loc_11_137E
        call   sub_11_1391
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_11_137E:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        popal  
        cmp    dword ptr [esp], 1
        lea    esp, [esp + 0x84]
        ret    
sub_11_1391:
        mov    eax, 0                        ; -> sub_9_0000
        mov    ecx, 0x6b                     ; -> obj9:0x006B
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        ret    
loc_11_13A5:
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, ecx
        shr    eax, 0xc
        movzx  edi, cx
        and    al, 0xf0
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebp + 0x1c]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x10]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x18]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x14]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 4]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x3c]
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x38]
        stosw  word ptr es:[edi], ax
        xor    eax, eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [esi + 8]
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp + 8], eax
        mov    word ptr [ebp + 0x14], cx
        shr    ecx, 0x10
        mov    word ptr [ebp + 0x3c], cx
        mov    word ptr [ebp + 0x1c], 0x5d00
        xor    eax, eax
        mov    al, 0x21
        call   sub_9_0000
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp + 8], esi
        mov    dword ptr [esi + 4], eax
        popal  
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 12  vsize=0x4c9  R-X
loc_12_0000:
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    dword ptr es:[eax], eax
        add    byte ptr [esi], ah            ; -> obj6:0x0126
        add    dword ptr [eax], eax
        add    byte ptr [eax], dh            ; -> obj6:0x0130
        add    dword ptr [eax], eax
        add    byte ptr [eax], dh            ; -> obj6:0x0130
        add    dword ptr [eax], eax
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], bl            ; -> obj6:0x0118
        add    dword ptr [eax], eax
        add    byte ptr [ebx], ch            ; -> obj6:0x012B
        add    dword ptr [eax], eax
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj6:0x0310
        add    eax, dword ptr [eax]
        add    byte ptr [edi], cl
        mov    dh, 0x55
        sbb    al, 0x83
        cli    
        adc    dword ptr [edi + 0xd], esi
        mov    edx, dword ptr [edx*4]        ; -> loc_12_0000
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_12_005E:
        mov    edx, 0x310                    ; -> obj6:0x0310
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x53], dl
        mov    eax, dword ptr [ebp + 0x14]
        test   word ptr [ebx], 0x180
        jne    loc_12_007B
        movzx  eax, ax
loc_12_007B:
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        mov    dword ptr [ebx + 0x38], eax
        mov    ax, word ptr [ebp + 0x3c]
        mov    word ptr [ebx + 0x36], ax
        xor    eax, eax
        mov    dword ptr [ebx + 0x3c], eax
        pop    ebx
        pop    eax
        ret    
loc_12_0094:
        push   eax
        push   ebx
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        movzx  eax, word ptr [ebx + 0x36]
        verw   ax
        je     loc_12_00AE
        xor    eax, eax
        mov    word ptr [ebx + 0x36], ax
        mov    dword ptr [ebx + 0x38], eax
loc_12_00AE:
        mov    word ptr [ebp + 0x38], ax
        mov    eax, dword ptr [ebx + 0x38]
        mov    word ptr [ebp + 0x10], ax
        pop    ebx
        test   word ptr [ebx], 0x180
        je     loc_12_00C4
        mov    dword ptr [ebp + 0x10], eax
loc_12_00C4:
        pop    eax
        ret    
sub_12_00C6:
        mov    ecx, 0x25
        cmp    byte ptr fs:[esi], 0xff
        je     loc_12_00D2
        ret    
loc_12_00D2:
        add    ecx, 7
        ret    
loc_12_00D6:
        push   fs
        pushal 
        mov    eax, 0x3c14
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        call   sub_12_00C6
        jmp    loc_12_00F3
loc_12_00EB:
        push   fs
        pushal 
        mov    ecx, 0x2b
loc_12_00F3:
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        mov    fs, word ptr [eax + ebx + 0x36]
        mov    esi, dword ptr [eax + ebx + 0x38]
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        cmp    dword ptr [eax + ebx + 0x3c], edi
        je     loc_12_0138
        mov    dword ptr [eax + ebx + 0x3c], edi
        push   dword ptr [ebp + 0x60]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x14]
        mov    word ptr [ebp + 0x14], di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x60], di
        mov    byte ptr [ebp + 0x1d], 0x1a
        call   sub_12_04B0
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x1c]
        pop    dword ptr [ebp + 0x60]
loc_12_0138:
        cmp    byte ptr [ebp + 0x1d], 0x12
        ja     loc_12_014B
        mov    edx, 0x325                    ; -> obj6:0x0325
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        jmp    loc_12_0150
loc_12_014B:
        call   sub_12_04B0
loc_12_0150:
        stc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        popal  
        pop    fs
        ret    
loc_12_015B:
        pushal 
        movzx  eax, byte ptr [ebp + 0x1c]
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    word ptr [ebp + 0x38], cx
        test   word ptr [ebx], 0x180
        jne    loc_12_0177
        mov    word ptr [ebp + 0x10], dx
        popal  
        ret    
loc_12_0177:
        mov    dword ptr [ebp + 0x10], edx
        popal  
        ret    
loc_12_017C:
        movzx  eax, byte ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x3c]
        mov    edx, dword ptr [ebp + 0x14]
        test   word ptr [ebx], 0x180
        jne    loc_12_0191
        movzx  edx, dx
loc_12_0191:
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        cmp    al, 0x24
        jne    loc_12_01D6
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_12_01D6
        cmp    dword ptr [0x1f4], 0          ; -> obj1:0x01F4
        je     loc_12_01D6
        push   ecx
        xor    ecx, ecx
        xchg   word ptr [0x13c], cx          ; -> obj6:0x013C
        jecxz  loc_12_01D5
        mov    dword ptr [0x13e], edx        ; -> obj6:0x013E
        mov    edx, dword ptr [esp]
        mov    word ptr [0x142], dx          ; -> obj6:0x0142
        movzx  edx, word ptr [0x13a]         ; -> obj6:0x013A
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
loc_12_01D5:
        pop    ecx
loc_12_01D6:
        ret    
loc_12_01D7:
        push   fs
        pushal 
        mov    ax, 0x3c04
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    ecx, 0x14
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        mov    eax, edi
        mov    word ptr [ebp + 4], ax
        shr    eax, 0x10
        xchg   word ptr [ebp + 0x60], ax
        call   sub_12_04B0
        stc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        mov    word ptr [ebp + 0x60], ax
        movzx  ecx, word ptr [ebp + 4]
        sub    cx, di
        add    esi, ecx
        mov    dword ptr [ebp + 4], esi
        popal  
        pop    fs
        ret    
loc_12_021D:
        mov    edx, 0x310                    ; -> obj6:0x0310
        cmp    byte ptr [ebp + 0x1c], 0xb
        ja     loc_12_0233
        movzx  edx, byte ptr [ebp + 0x1c]
        mov    edx, dword ptr [edx*4 + 0x490] ; -> obj6:0x0490
loc_12_0233:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_12_0239:
        mov    edx, 0x310                    ; -> obj6:0x0310
        cmp    byte ptr [ebp + 0x1c], 3
        ja     loc_12_024F
        movzx  edx, byte ptr [ebp + 0x1c]
        mov    edx, dword ptr [edx*4 + 0x4c0] ; -> obj6:0x04C0
loc_12_024F:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_12_0255:
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    al, 4
        mov    edx, 0x45a                    ; -> obj6:0x045A
        je     loc_12_0270
        sub    edx, 0xc
        sub    al, 2
        cmp    al, 3
        jbe    loc_12_0270
        mov    edx, 0x310                    ; -> obj6:0x0310
loc_12_0270:
        pop    eax
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_12_0277:
        pushal 
        mov    eax, dword ptr [ebp + 0x10]
        xor    edx, edx
        call   0xfff40568                    ; -> obj5:0x0568
        jae    loc_12_02A4
        shr    eax, 4
        xchg   dword ptr [ebp + 0x10], eax
        call   sub_12_04B0
        mov    dword ptr [ebp + 0x10], eax
        mov    word ptr [edi + 0x2a], ax
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        mov    word ptr [ebx + 0x2e], 0
        popal  
        ret    
loc_12_02A4:
        or     byte ptr [ebp + 0x2c], 1
        popal  
        ret    
loc_12_02AA:
        pushal 
        movzx  eax, word ptr [edi + 0x2a]
        mov    esi, eax
        xor    edx, edx
        test   eax, eax
        je     loc_12_02BC
        call   0xfff40568                    ; -> obj5:0x0568
loc_12_02BC:
        mov    ecx, eax
        shr    ecx, 4
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        cmp    cx, word ptr [ebp + 0x10]
        jne    loc_12_02E8
        mov    word ptr [ebp + 0x10], si
loc_12_02E2:
        and    byte ptr [ebp + 0x2c], 0xfe
        popal  
        ret    
loc_12_02E8:
        movzx  eax, word ptr [ebp + 0x10]
        shl    eax, 4
        mov    ecx, 0xff
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        mov    word ptr [edi + 0x2a], cx
        mov    word ptr [ebp + 0x10], cx
        jmp    loc_12_02E2
loc_12_0304:
        pushal 
        mov    eax, dword ptr [ebp + 0x14]
        xor    edx, edx
        call   0xfff40568                    ; -> obj5:0x0568
        jae    loc_12_0321
        shr    eax, 4
        xchg   dword ptr [ebp + 0x14], eax
        call   sub_12_04B0
        mov    dword ptr [ebp + 0x14], eax
        popal  
        ret    
loc_12_0321:
        popal  
        ret    
loc_12_0323:
        pushal 
        mov    eax, dword ptr [ebp + 0x14]
        xor    edx, edx
        call   0xfff40568                    ; -> obj5:0x0568
        jae    loc_12_0350
        shr    eax, 4
        xchg   dword ptr [ebp + 0x14], eax
        call   sub_12_04B0
        mov    dword ptr [ebp + 0x14], eax
        mov    word ptr [edi + 0x2a], ax
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        mov    word ptr [ebx + 0x2e], 0
        popal  
        ret    
loc_12_0350:
        popal  
        ret    
loc_12_0352:
        pushal 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_12_0375
        lea    esi, [edi + 0x33]
        cld    
        lodsb  al, byte ptr [esi]
        mov    byte ptr [ebp + 0x19], al
        lodsw  ax, word ptr [esi]
        mov    word ptr [ebp + 0x1c], ax
        lodsw  ax, word ptr [esi]
        mov    word ptr [ebp + 0x10], ax
        and    byte ptr [ebp + 0x2c], 0xfe
        jmp    loc_12_037A
loc_12_0375:
        call   sub_12_04B0
loc_12_037A:
        popal  
        ret    
loc_12_037C:
        push   eax
        push   esi
        push   fs
        mov    ecx, 1
        mov    eax, 0x3c14
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
loc_12_0390:
        cmp    byte ptr fs:[esi], 0
        je     loc_12_039A
        inc    ecx
        inc    esi
        jmp    loc_12_0390
loc_12_039A:
        add    ecx, 0xd
        pop    fs
        pop    esi
        pop    eax
        ret    
loc_12_03A2:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_12_03BA
        mov    word ptr [edx + ebx + 0x2e], 0
        jmp    sub_12_04B0
loc_12_03BA:
        push   eax
        mov    ax, word ptr [edi + 0x2a]
        mov    word ptr [edx + ebx + 0x34], ax
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [edx + ebx + 0x2e], ax
        mov    eax, dword ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        mov    dword ptr [edx + ebx + 0x30], eax
        pop    eax
        jmp    sub_12_04B0
loc_12_03E0:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    esi, dword ptr [0]            ; -> obj6:0x0000
        add    esi, ebx
        movzx  eax, word ptr [edi + 0x2a]
        push   fs
        mov    fs, ax
        mov    ax, word ptr [ebp + 0x28]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x24]
        xor    edx, edx
        mov    dword ptr fs:[edx + 0xa], eax
        mov    ax, word ptr fs:[edx + 0x16]
        call   0xfff40568                    ; -> obj5:0x0568
        jae    loc_12_045D
        movzx  ecx, word ptr fs:[edx + 0x32]
loc_12_0429:
        dec    ecx
        mov    byte ptr [ebp + 0x1d], 0x3e
        mov    word ptr [ebp + 0x10], cx
        push   fs
        pushal 
        call   sub_10_01B2
        popal  
        pop    fs
        test   ecx, ecx
        jne    loc_12_0429
        mov    ecx, eax
        shr    eax, 4
        mov    word ptr fs:[edx + 0x16], ax
        mov    word ptr [edi + 0x2a], ax
        mov    eax, dword ptr [ebp + 0x32]
        mov    ax, word ptr [ebp + 0x30]
        sub    ax, 0x18
        mov    dword ptr [ecx + 0x2e], eax
loc_12_045D:
        mov    eax, dword ptr [0x40]         ; -> obj6:0x0040
        mov    dword ptr fs:[edx + 0xe], eax
        mov    eax, dword ptr [0x44]         ; -> obj6:0x0044
        mov    dword ptr fs:[edx + 0x12], eax
        pop    fs
        mov    byte ptr [ebp + 0x1d], 0x4c
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [0x40]         ; -> obj6:0x0040
        mov    dword ptr [edx + 0x8c], eax
        mov    eax, dword ptr [0x44]         ; -> obj6:0x0044
        mov    dword ptr [edx + 0x90], eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        popal  
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
        db  02Eh, 08Bh, 0C0h
sub_12_04B0:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   eax
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 13  vsize=0x14c  R-X
        dd  loc_11_03EF              ; fixup
        dd  loc_11_043B              ; fixup
        dd  loc_11_03DA              ; fixup
        dd  loc_13_009E              ; fixup
        dd  obj6:0x0118              ; fixup
        dd  obj6:0x0118              ; fixup
        dd  loc_13_0090              ; fixup
        dd  loc_11_043B              ; fixup
        dd  loc_11_03DA              ; fixup
        dd  loc_13_009E              ; fixup
        dd  obj6:0x0118              ; fixup
        dd  obj6:0x011F              ; fixup
        dd  loc_11_03E8              ; fixup
        dd  loc_11_03E1              ; fixup
        dd  sub_11_042D              ; fixup
        dd  loc_11_03E8              ; fixup
loc_13_0040:
        call   sub_11_042D
        add    al, 0
        add    cl, ah                        ; -> loc_11_03E1
        add    eax, dword ptr [eax]
        add    al, ch                        ; -> loc_11_03E8
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> obj6:0x011F
        add    dword ptr [eax], eax
        add    byte ptr [eax], bl            ; -> obj6:0x0118
        add    dword ptr [eax], eax
        add    byte ptr [esi - 0x26000000], bl ; -> loc_13_009E
        add    eax, dword ptr [eax]
        add    byte ptr [ebx], bh            ; -> loc_11_043B
        add    al, 0
        add    byte ptr [eax + 0x18000000], dl ; -> loc_13_0090
        add    dword ptr [eax], eax
        add    byte ptr [eax], bl            ; -> obj6:0x0118
        add    dword ptr [eax], eax
        add    byte ptr [esi - 0x26000000], bl ; -> loc_13_009E
        add    eax, dword ptr [eax]
        add    byte ptr [ebx], bh            ; -> loc_11_043B
        add    al, 0
        add    bh, ch                        ; -> loc_11_03EF
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 0x41], al
        inc    edx
        inc    esi
        inc    edi
        dec    eax
        pushal 
        popal  
        bound  esp, qword ptr [esi + 0x67]
        push   0x716f6e6d
loc_13_0090:
        add    eax, 0xcd                     ; -> loc_13_00CD
        add    ebx, dword ptr [ebx]          ; -> loc_11_041B
        add    al, 0
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_13_009E:
        add    dword ptr [ecx], ebx
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_13_00A5:
        push   eax
        push   ecx
        push   edi
        mov    al, byte ptr [ebp + 0x18]
        mov    ecx, 0x10
        mov    edi, 0x80                     ; -> obj13:0x0080
        repne scasb al, byte ptr es:[edi]
        mov    edx, dword ptr [ecx*4 + 0x40] ; -> loc_13_0040
        pop    edi
        pop    ecx
        pop    eax
        jne    loc_12_005E
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_13_00CD:
        push   0x20
        pop    ecx
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        jmp    sub_13_00EC
        db  000h, 000h, 000h
sub_13_00EC:
        cmp    byte ptr [0x164], 0           ; -> obj6:0x0164
        je     loc_13_013D
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_13_013D
        pushal 
        mov    ebx, dword ptr [ebp + 0x10]
        test   bl, bl
        je     loc_13_013E
        xchg   edi, eax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        movzx  eax, word ptr [edi + 0x14]
        movzx  edx, word ptr [edi + 0x16]
        movzx  ecx, word ptr [edi + 7]
        mov    esi, 0x144                    ; -> obj6:0x0144
        cmp    byte ptr [esi], bl
        je     loc_13_0131
        mov    edi, 0x154                    ; -> obj6:0x0154
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    esi, 0x10
loc_13_0131:
        mov    byte ptr [esi], bl
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0xc], edx
loc_13_013C:
        popal  
loc_13_013D:
        ret    
loc_13_013E:
        mov    byte ptr [0x144], bl          ; -> obj6:0x0144
        mov    byte ptr [0x154], bl          ; -> obj6:0x0154
        jmp    loc_13_013C
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 14  vsize=0x101b  R-X
loc_14_0000:
        mov    eax, dword ptr [0xc]          ; -> obj6:0x000C
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 0x8c], eax
        call   sub_14_0054
        jb     loc_14_003D
loc_14_0015:
        mov    ecx, dword ptr [0x4c]         ; -> obj6:0x004C
        jecxz  loc_14_002C
        mov    edx, dword ptr [ebx + 4]
        xchg   dword ptr [edx + 0xa0], ecx
        mov    dword ptr [0x64], ecx         ; -> obj6:0x0064
loc_14_002C:
        mov    eax, dword ptr [ebx + 0xc]
        mov    edx, dword ptr [0x68]         ; -> obj6:0x0068
        add    edx, dword ptr [ebx + 4]
        mov    word ptr [edx], ax
        clc    
loc_14_003C:
        ret    
loc_14_003D:
        mov    eax, 0x20008
        lea    edx, [0x1b0]                  ; -> obj1:0x01B0
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        jmp    loc_14_003C
sub_14_0054:
        pushal 
        cmp    dword ptr [0x70], 0           ; -> obj6:0x0070
        je     loc_14_0074
        mov    esi, dword ptr [0x6c]         ; -> obj6:0x006C
        add    esi, dword ptr [ebx + 4]
        cmp    word ptr [esi], -1
        jne    loc_14_0074
        mov    eax, dword ptr [0x78]         ; -> obj6:0x0078
        mov    dword ptr [esi], eax
loc_14_0074:
        popal  
        clc    
        ret    
        db  000h
_DOSMGR_Set_Exec_VM_Data:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
        add    edx, dword ptr [ebx + 4]
        mov    word ptr [edx + 0x20], 0
        cld    
        mov    esi, dword ptr [esp + 0x1c]
        lea    edi, [edx + 0x130]
        mov    ecx, 0x44
        stosb  byte ptr es:[edi], al
        test   esi, esi
        je     loc_14_00AA
        dec    edi
loc_14_00A4:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        loopne loc_14_00A4
loc_14_00AA:
        mov    esi, dword ptr [esp + 0x14]
        lea    edi, [edx + 0x2e]
        mov    eax, edi
        sub    eax, dword ptr [ebx + 4]
        mov    ecx, 0x20
        rep movsd dword ptr es:[edi], dword ptr [esi]
        shl    eax, 0xc
        shr    ax, 0xc
        mov    dword ptr [edx + 0x22], eax
        lea    edi, [edx]
        lea    esi, [edi + 0x26]
        push   0xed                          ; -> loc_14_00ED
sub_14_00D1:
        mov    eax, edi
        sub    eax, dword ptr [ebx + 4]
        shl    eax, 0xc
        shr    ax, 0xc
        mov    dword ptr [esi], eax
        sub    eax, eax
        stosb  byte ptr es:[edi], al
        mov    eax, 0x20202020
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        ret    
loc_14_00ED:
        lea    edi, [edx + 0x10]
        lea    esi, [edi + 0x1a]
        call   sub_14_00D1
        mov    esi, dword ptr [esp + 0x18]
        lea    edi, [edx + 0xb0]
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    dword ptr [edx + ebx + 0x4c], edi
        mov    ecx, 0x80
loc_14_0111:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        loopne loc_14_0111
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_14_011B:
        enter  0x10, 0
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> obj6:0x0000
        bts    word ptr [esi + ebx], 0xe
        jb     loc_14_0293
        cld    
        mov    dword ptr [ebp - 4], edx
        mov    edi, edx
        movzx  edx, word ptr [edi + 0x54]
        or     edx, edx
        jne    loc_14_0147
        mov    edx, dword ptr [0xca]         ; -> obj6:0x00CA
loc_14_0147:
        mov    dword ptr [ebp - 0xc], edx
        call   sub_14_02E9
        jb     loc_14_0293
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x10], edx
        cmp    byte ptr [edi + 4], 0
        jne    loc_14_018A
        mov    edx, dword ptr [ebp - 0xc]
        or     edx, edx
        je     loc_14_0293
        cmp    edx, dword ptr [0x48]         ; -> obj6:0x0048
        jbe    loc_14_0293
        or     al, al
        jne    loc_14_0228
        cmp    word ptr [edi + 0x54], ax
        je     loc_14_0293
loc_14_018A:
        and    al, 3
        cmp    al, 1
        je     loc_14_0219
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + ebx + 0x4c], edx
        mov    esi, dword ptr [0x10]         ; -> obj6:0x0010
        add    esi, dword ptr [ebx + 4]
        lea    edi, [esi + 0xb0]
        lea    edx, [edi - 1]
        add    esi, 0x2e
        sub    eax, eax
        mov    ecx, 0x80
        repne scasb al, byte ptr es:[edi]
        dec    ecx
        jle    loc_14_0293
        lodsb  al, byte ptr [esi]
        cmp    eax, ecx
        jae    loc_14_0293
        push   esi
        mov    byte ptr [edi - 1], 0x20
        cmp    byte ptr [esi], 0x20
        jne    loc_14_01D5
        inc    esi
        dec    eax
loc_14_01D5:
        sub    ecx, eax
        xchg   ecx, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xchg   ecx, eax
        mov    al, 0xd
        stosb  byte ptr es:[edi], al
        not    ecx
        add    ecx, 0x80
        mov    byte ptr [edx], cl
        pop    edi
        dec    edi
        mov    word ptr [edi], 0xd00
        mov    esi, edx
        mov    edx, dword ptr [ebp - 4]
        cmp    byte ptr [edx + 4], 0
        jne    loc_14_020F
        inc    ecx
        inc    ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0xb0                     ; -> obj6:0x00B0
        mov    ecx, 3
        call   sub_14_0297
        jmp    loc_14_0219
loc_14_020F:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        or     byte ptr [edx + ebx], 1
loc_14_0219:
        mov    edx, dword ptr [ebp - 0xc]
        or     edx, edx
        je     loc_14_025C
        cmp    edx, dword ptr [0x48]         ; -> obj6:0x0048
        jbe    loc_14_025C
loc_14_0228:
        test   dword ptr [ebp - 8], 4
        jne    loc_14_0293
        mov    ecx, 9
        call   sub_14_0297
        jb     loc_14_0293
        mov    ax, 0x452f
        stosw  word ptr es:[edi], ax
        mov    al, 0x3a
        stosb  byte ptr es:[edi], al
        mov    ecx, 5
loc_14_024B:
        xchg   edx, eax
        cdq    
        div    dword ptr [ecx*4 + 0xb2]      ; -> obj6:0x00B2
        add    al, 0x30
        stosb  byte ptr es:[edi], al
        loop   loc_14_024B
        mov    al, 0x20
        stosb  byte ptr es:[edi], al
loc_14_025C:
        mov    edx, dword ptr [ebp - 4]
        cmp    byte ptr [edx + 4], 0
        je     loc_14_0293
        mov    esi, 0xb3                     ; -> obj6:0x00B3
        mov    ecx, 0x103
        call   sub_14_0297
        jb     loc_14_0293
        lea    esi, [edx + 4]
        mov    edi, esi
        sub    al, al
        mov    ecx, 0x50
        repne scasb al, byte ptr es:[edi]
        jne    loc_14_0293
        not    ecx
        add    ecx, 0x150
        call   sub_14_0297
loc_14_0293:
        pop    edi
        pop    esi
        leave  
        ret    
sub_14_0297:
        push   edx
        mov    edi, dword ptr [0x10]         ; -> obj6:0x0010
        add    edi, dword ptr [ebx + 4]
        add    edi, 0x2e
        mov    al, byte ptr [edi]
        movzx  edx, al
        add    al, cl
        jb     loc_14_02D1
        cmp    al, 0x7f
        cmc    
        jb     loc_14_02D1
        stosb  byte ptr es:[edi], al
        add    edi, edx
        inc    edx
        or     ch, ch
        movzx  ecx, cl
        je     loc_14_02C2
        mov    edx, 1
loc_14_02C2:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + edi], al
        dec    edi
        dec    edx
        jne    loc_14_02C2
        inc    edi
        push   edi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        clc    
loc_14_02D1:
        pop    edx
        ret    
sub_14_02D3:
        mov    ecx, edx
        jmp    loc_14_02E2
loc_14_02D7:
        inc    edx
        cmp    al, 0x5c
        je     loc_14_02E0
        cmp    al, 0x2f
        jne    loc_14_02E2
loc_14_02E0:
        mov    ecx, edx
loc_14_02E2:
        mov    al, byte ptr [edx]
        cmp    al, 0
        jne    loc_14_02D7
        ret    
sub_14_02E9:
        push   esi
        push   edi
        mov    esi, 0x90                     ; -> obj6:0x0090
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        jb     loc_14_036C
        push   edx
        call   sub_14_02D3
        mov    esi, ecx
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    edx, dword ptr [edx + ebx + 0x4c]
        call   sub_14_02D3
        mov    edi, ecx
        sub    edx, edx
        mov    ecx, 0x80
loc_14_0318:
        lodsb  al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        inc    edi
        or     ax, ax
        je     loc_14_0333
        cmp    al, 0x41
        jb     loc_14_032D
        cmp    al, 0x5a
        ja     loc_14_032D
        or     ax, 0x2020
loc_14_032D:
        cmp    al, ah
        loope  loc_14_0318
        jmp    loc_14_0369
loc_14_0333:
        inc    edx
        mov    esi, dword ptr [0x10]         ; -> obj6:0x0010
        add    esi, dword ptr [ebx + 4]
        add    esi, 0x2e
        lodsb  al, byte ptr [esi]
        movzx  ecx, al
        jecxz  loc_14_0369
loc_14_0346:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x2f
        jne    loc_14_0367
        mov    al, byte ptr [esi]
        or     al, 0x20
        cmp    al, 0x65
        jne    loc_14_0358
        add    edx, 4
        jmp    loc_14_0367
loc_14_0358:
        cmp    al, 0x63
        jne    loc_14_035F
        inc    edx
        jmp    loc_14_0369
loc_14_035F:
        cmp    al, 0x6b
        jne    loc_14_0367
        inc    edx
        inc    edx
        jmp    loc_14_0369
loc_14_0367:
        loop   loc_14_0346
loc_14_0369:
        xchg   edx, eax
        pop    edx
        clc    
loc_14_036C:
        pop    edi
        pop    esi
        ret    
sub_14_036F:
        pushal 
        xor    esi, esi
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        mov    edi, edx
        test   edi, edi
        je     loc_14_038B
        cld    
        xor    eax, eax
        or     ecx, 0xffffffff
loc_14_0384:
        repne scasb al, byte ptr es:[edi]
        cmp    byte ptr [edi], al
        jne    loc_14_0384
        inc    edi
loc_14_038B:
        sub    edi, edx
        mov    dword ptr [esp + 0x1c], edi
        popal  
        ret    
loc_14_0393:
        test   byte ptr [edx + 2], 4
        je     loc_14_03A2
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        or     byte ptr [eax + ebx], 8
loc_14_03A2:
        ret    
_DOSMGR_Exec_VM:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + ebx + 0x18], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + ebx + 0x1c], eax
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        add    eax, dword ptr [ebp + 0xc]
        shl    eax, 2
        mov    esi, dword ptr [ebx + 4]
        mov    word ptr [esi + 0x413], ax
        movzx  ecx, word ptr [0x34]          ; -> obj6:0x0034
        lea    edi, [ecx - 1]
        shl    edi, 4
        test   byte ptr [0x210], 4           ; -> loc_1_0210
        jne    loc_14_03FE
        cmp    byte ptr [edi + esi], 0x5a
        je     loc_14_0652
loc_14_03FE:
        cmp    byte ptr [0x168], 0           ; -> obj6:0x0168
        je     loc_14_05C5
        push   eax
        shr    eax, 2
        dec    eax
        push   eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        cmp    eax, dword ptr [esp]
        je     loc_14_0597
        jb     loc_14_05C3
        push   0x80
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_14_0535
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    dword ptr [edx + ebx + 0x48], eax
        mov    edx, eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        push   eax
        push   0
        push   0
        push   1
        push   eax
        push   ebx
        push   edx
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        or     eax, eax
        pop    eax
        je     loc_14_0519
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [edx + ebx + 1], 1
        jne    loc_14_04A0
        cmp    dword ptr [edx + ebx + 0x48], 0
        je     loc_14_04A0
        push   eax
        push   0
        push   0
        push   1
        push   dword ptr [edx + ebx + 0x48]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        pop    eax
loc_14_04A0:
        shl    eax, 0xc
        mov    byte ptr [eax + esi], 0x4d
        mov    word ptr [eax + esi + 1], 0
        mov    word ptr [eax + esi + 3], 0xfe
        add    eax, 0xff0
        add    eax, esi
        xchg   edi, eax
        push   esi
        mov    esi, 0x169                    ; -> obj6:0x0169
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        xchg   edi, eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        shl    eax, 8
        mov    ecx, eax
        pop    eax
        add    esp, 4
        shl    eax, 8
        or     al, 0xff
        test   byte ptr [0x210], 4           ; -> loc_1_0210
        je     loc_14_04F4
        mov    edx, dword ptr [0x30]         ; -> obj6:0x0030
        mov    word ptr [edx + esi], ax
loc_14_04F4:
        sub    ecx, eax
        dec    ecx
        shl    eax, 4
        mov    byte ptr [eax + esi], 0x4d
        mov    word ptr [eax + esi + 1], 8
        mov    word ptr [eax + esi + 3], cx
        movzx  ecx, word ptr [0x34]          ; -> obj6:0x0034
        shr    eax, 4
        jmp    loc_14_0685
loc_14_0519:
        xor    eax, eax
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        xchg   dword ptr [edx + ebx + 0x48], eax
        or     eax, eax
        je     loc_14_0535
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_14_0535:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        shl    eax, 8
        add    eax, 0x100
        movzx  ecx, word ptr [0x16c]         ; -> obj6:0x016C
        add    ecx, eax
        pop    eax
        add    esp, 4
        shl    eax, 8
        or     al, 0xff
        test   byte ptr [0x210], 4           ; -> loc_1_0210
        je     loc_14_0568
        mov    edx, dword ptr [0x30]         ; -> obj6:0x0030
        mov    word ptr [edx + esi], ax
loc_14_0568:
        sub    ecx, eax
        dec    ecx
        shl    eax, 4
        mov    dl, byte ptr [0x169]          ; -> obj6:0x0169
        mov    byte ptr [eax + esi], dl
        mov    dx, word ptr [0x16a]          ; -> obj6:0x016A
        mov    word ptr [eax + esi + 1], dx
        mov    word ptr [eax + esi + 3], cx
        movzx  ecx, word ptr [0x34]          ; -> obj6:0x0034
        shr    eax, 4
        jmp    loc_14_0685
loc_14_0597:
        pop    eax
        add    esp, 4
        shl    eax, 0xc
        add    eax, 0xff0
        add    eax, esi
        push   eax
        xchg   edi, eax
        push   esi
        push   ecx
        mov    esi, 0x169                    ; -> obj6:0x0169
        mov    ecx, 4
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        pop    esi
        xchg   edi, eax
        pop    eax
        sub    eax, esi
        shr    eax, 4
        jmp    loc_14_0685
loc_14_05C3:
        pop    eax
        pop    eax
loc_14_05C5:
        shl    eax, 6
        cmp    eax, dword ptr [0x54]         ; -> obj6:0x0054
        je     loc_14_0685
        shr    eax, 8
        dec    eax
        shl    eax, 0xc
        add    eax, 0xff0
        mov    byte ptr [eax + esi], 0x4d
        mov    word ptr [eax + esi + 1], 8
        mov    edx, dword ptr [0x54]         ; -> obj6:0x0054
        shr    eax, 4
        push   ecx
        xor    ecx, ecx
        test   byte ptr [0x210], 4           ; -> loc_1_0210
        je     loc_14_063C
        push   edx
        mov    edx, dword ptr [0x30]         ; -> obj6:0x0030
        mov    ecx, edi
        shr    ecx, 4
        inc    ecx
        add    cx, word ptr [edi + esi + 3]
        cmp    word ptr [edx + esi], cx
        mov    ecx, 0
        jne    loc_14_063B
        movzx  ecx, ax
        xchg   word ptr [edx + esi], cx
        shl    ecx, 4
        mov    cx, word ptr [ecx + esi + 3]
        shl    eax, 4
        mov    word ptr [eax + esi + 8], 0x4353
        shr    eax, 4
        inc    cx
loc_14_063B:
        pop    edx
loc_14_063C:
        sub    edx, eax
        dec    edx
        shl    eax, 4
        mov    word ptr [eax + esi + 3], dx
        add    word ptr [eax + esi + 3], cx
        shr    eax, 4
        pop    ecx
        jmp    loc_14_0685
loc_14_0652:
        shl    eax, 6
        push   ecx
        push   eax
        VxDCall  EBIOS.EBIOS_Get_Version         ; cd2000001200
loc_14_065D:
        jb     loc_14_0683
        cmp    eax, 0x200
        jb     loc_14_0683
        VxDCall  EBIOS.EBIOS_Get_Unused_Mem      ; cd2001001200
        jb     loc_14_0683
        jecxz  loc_14_0683
        pushfd 
        cmp    eax, dword ptr [esp + 4]
        jne    loc_14_0682
        popfd  
        je     loc_14_067F
        add    dword ptr [esp], ecx
        jmp    loc_14_0683
loc_14_067F:
        sub    esp, 4
loc_14_0682:
        pop    eax
loc_14_0683:
        pop    eax
        pop    ecx
loc_14_0685:
        sub    eax, ecx
        mov    word ptr [edi + esi + 3], ax
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [0x10]         ; -> obj6:0x0010
        add    eax, dword ptr [ebx + 4]
        lea    edi, [eax + 0x130]
        mov    byte ptr [esi + 0x1d], 0xe
        mov    dl, byte ptr [edi]
        sub    dl, 0x41
        mov    byte ptr [esi + 0x14], dl
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    byte ptr [esi + 0x1d], 0x3b
        shl    edi, 0xc
        shr    di, 0xc
        mov    word ptr [esi + 0x14], di
        shr    edi, 0x10
        mov    word ptr [esi + 0x3c], di
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        xor    ecx, ecx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_14_06F1
        call   0xfff20432                    ; -> obj5:0x0432
        mov    cx, word ptr [0xce]           ; -> obj6:0x00CE
loc_14_06F1:
        VxDCall  DOSNET.DOSNET_Get_Version       ; cd2000001a00
        test   eax, eax
        je     loc_14_070E
        VxDCall  DOSNET.DOSNET_Do_PSP_Adjust     ; cd2002001a00
        jb     loc_14_070A
        or     eax, eax
        jne    loc_14_0722
        inc    eax
        jmp    loc_14_070E
loc_14_070A:
        or     eax, eax
        jne    loc_14_0773
loc_14_070E:
        cmp    byte ptr [0x1f], 2            ; -> obj6:0x001F
        jae    loc_14_071E
        movzx  eax, byte ptr [0x1f]          ; -> obj6:0x001F
loc_14_071E:
        or     eax, eax
        je     loc_14_0773
loc_14_0722:
        mov    eax, dword ptr [ebx + 0xc]
        dec    eax
        mul    dword ptr [0x38]              ; -> obj6:0x0038
        add    ecx, eax
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shl    eax, 8
        add    eax, ecx
        inc    eax
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    edi, dword ptr [edx + ebx + 0x4c]
        dec    edi
        mov    edx, edi
loc_14_0747:
        inc    edx
        cmp    byte ptr [edx], 0
        jne    loc_14_0747
        sub    edx, edi
        push   eax
        call   sub_14_036F
        mov    dword ptr [ebp - 4], eax
        add    edx, eax
        pop    eax
        add    edx, 0x11
        shr    edx, 4
        add    eax, edx
        inc    eax
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_14_076E
        xor    al, 1
loc_14_076E:
        shr    eax, 1
        adc    ecx, 0
loc_14_0773:
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shl    eax, 8
        add    eax, ecx
        mov    dx, word ptr [0x34]           ; -> obj6:0x0034
        sub    eax, edx
        mov    word ptr [esi + 0x38], dx
        mov    word ptr [esi + 0x10], ax
        mov    word ptr [esi + 0x1c], 0x4a00
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        or     byte ptr [edx + ebx], 2
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_14_0844
        mov    edi, dword ptr [0x3c]         ; -> obj6:0x003C
        or     edi, edi
        je     loc_14_0844
        mov    esi, dword ptr [0x10]         ; -> obj6:0x0010
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 0x174
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        and    byte ptr [edx + ebx], 0xfd
        mov    edi, dword ptr [0x10]         ; -> obj6:0x0010
        add    edi, dword ptr [ebx + 4]
        add    edi, 0xb0
        mov    dword ptr [edx + ebx + 0x4c], edi
        mov    esi, dword ptr [0x3c]         ; -> obj6:0x003C
        lea    esi, [esi + 0x1f4]
        mov    ecx, 0x80
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [0x10]         ; -> obj6:0x0010
        add    edi, dword ptr [ebx + 4]
        add    edi, 0x2f
        mov    esi, dword ptr [0x3c]         ; -> obj6:0x003C
        add    esi, 0x174
        mov    eax, 0x2f20642f
        stosd  dword ptr es:[edi], eax
        mov    ax, 0x2063
        stosw  word ptr es:[edi], ax
        push   0x7a
        push   esi
        push   edi
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    byte ptr [edx], 0xd
        sub    edi, 6
        sub    edx, edi
        mov    byte ptr [edi - 1], dl
        jmp    loc_14_085C
loc_14_0844:
        xor    eax, eax
        xchg   dword ptr [0x3c], eax         ; -> obj6:0x003C
        or     eax, eax
        je     loc_14_085C
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_14_085C:
        call   sub_14_086C
        mov    ecx, dword ptr [0]            ; -> obj6:0x0000
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_14_086C:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [0x10]         ; -> obj6:0x0010
        add    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [eax + 0x26]
        mov    word ptr [esi], cx
        shr    ecx, 0x10
        mov    word ptr [esi + 0x38], cx
        lea    ecx, [eax + 0x2f]
        sub    ecx, dword ptr [ebx + 4]
        ror    ecx, 4
        mov    word ptr [esi + 0x3c], cx
        shr    ecx, 0x1c
        mov    word ptr [esi + 4], cx
        mov    word ptr [esi + 0x1c], 0x2901
        push   eax
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    eax
        mov    ecx, dword ptr [eax + 0x2a]
        mov    word ptr [esi], cx
        shr    ecx, 0x10
        mov    word ptr [esi + 0x38], cx
        mov    word ptr [esi + 0x1c], 0x2901
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        movzx  eax, word ptr [0x34]          ; -> obj6:0x0034
        dec    eax
        shl    eax, 4
loc_14_08E0:
        mov    ecx, eax
        movzx  eax, word ptr [ecx + 3]
        inc    eax
        shl    eax, 4
        add    eax, ecx
        cmp    word ptr [ecx + 1], 0
        jne    loc_14_08E0
        cmp    eax, 0x50
        jbe    loc_14_08E0
        mov    ecx, eax
        xor    eax, eax
        mov    dword ptr [ecx - 0x10], eax
        mov    al, 0x10
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_14_0915
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [edx + ebx], 2
        jne    loc_14_093E
loc_14_0915:
        mov    eax, dword ptr [ebp - 4]
        test   eax, eax
        jne    loc_14_0921
        call   sub_14_036F
loc_14_0921:
        add    eax, 0xf
        and    al, 0xf0
        mov    edi, ecx
        sub    edi, eax
        push   ecx
        mov    ecx, eax
        shr    ecx, 2
        xor    esi, esi
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        mov    esi, edx
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
loc_14_093E:
        sub    ecx, eax
        mov    eax, dword ptr [0x10]         ; -> obj6:0x0010
        lea    edx, [eax + 0x20]
        add    eax, dword ptr [ebx + 4]
        shr    ecx, 4
        mov    word ptr [eax + 0x20], cx
        mov    esi, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [esi + ebx], 8
        je     loc_14_0963
        call   sub_11_0213
loc_14_0963:
        mov    esi, dword ptr [ebx + 8]
        shl    edx, 0xc
        shr    dx, 0xc
        mov    word ptr [esi + 0x10], dx
        shr    edx, 0x10
        mov    word ptr [esi + 0x38], dx
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    ecx, dword ptr [edx + ebx + 0x4c]
        sub    ecx, dword ptr [ebx + 4]
        cmp    ecx, 0xffff0
        jb     loc_14_09B7
        push   esi
        mov    esi, 0x90                     ; -> obj6:0x0090
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        xchg   edx, eax
        xor    esi, esi
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        sub    eax, edx
        mov    ecx, dword ptr [0x10]         ; -> obj6:0x0010
        add    ecx, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ecx + 0x20]
        shl    ecx, 4
        add    ecx, eax
        pop    esi
loc_14_09B7:
        shl    ecx, 0xc
        shr    cx, 0xc
        mov    word ptr [esi + 0x14], cx
        shr    ecx, 0x10
        mov    word ptr [esi + 0x3c], cx
        mov    word ptr [esi + 0x1c], 0x4b00
        mov    dx, word ptr [esi + 0x34]
        shl    edx, 0x10
        mov    dx, word ptr [esi + 0x30]
        xor    eax, eax
        mov    esi, 0x15c                    ; -> loc_15_015C
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        mov    al, 0x21
        VMMCall  VMM.Simulate_Int                ; cd2046800100
DOSMGR_Copy_VM_Drive_State:
        pushal 
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [0x20]         ; -> obj6:0x0020
        add    esi, edi
        add    edi, dword ptr [ebx + 4]
        sub    esp, 0x400
        mov    ebp, esp
        mov    edx, dword ptr [0x2c]         ; -> obj6:0x002C
loc_14_0A0C:
        mov    eax, 0x400
        cmp    edx, eax
        jae    loc_14_0A17
        mov    eax, edx
loc_14_0A17:
        mov    ebx, edi
        mov    edi, ebp
        mov    ecx, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   esi
        mov    edi, ebx
        mov    esi, ebp
        mov    ecx, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        sub    edx, eax
        jne    loc_14_0A0C
        add    esp, 0x400
        popal  
        ret    
        db  000h, 000h
sub_14_0A38:
        cmp    byte ptr [0x114], 2           ; -> obj6:0x0114
        jne    loc_14_0A45
        mov    al, 0x20
        jmp    loc_14_0A57
loc_14_0A45:
        cmp    byte ptr [0x114], 1           ; -> obj6:0x0114
        jne    loc_14_0A52
        mov    al, 0x10
        jmp    loc_14_0A57
loc_14_0A52:
        ret    
loc_14_0A53:
        mov    ax, word ptr [edx + 2]
loc_14_0A57:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        and    al, 0x30
        mov    ah, al
        xor    ah, byte ptr [edx + ebx + 1]
        je     loc_14_0A8C
        and    byte ptr [edx + ebx + 1], 0xdf
        test   al, 0x20
        je     loc_14_0A7C
        or     byte ptr [edx + ebx + 1], 0x20
        call   sub_15_0190
        or     al, 0x10
loc_14_0A7C:
        test   al, 0x10
        je     loc_14_0A87
        call   sub_14_0A8D
        jmp    loc_14_0A8C
loc_14_0A87:
        call   sub_15_0190
loc_14_0A8C:
        ret    
sub_14_0A8D:
        push   esi
        push   edi
        cmp    dword ptr [0xfc], 0           ; -> obj6:0x00FC
        jne    loc_14_0ABF
        mov    esi, 0x288                    ; -> loc_19_0288
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_14_0BC9
        mov    dword ptr [0xfc], eax         ; -> obj6:0x00FC
        mov    esi, 0x311                    ; -> loc_19_0311
        VMMCall  VMM.Hook_Invalid_Page_Fault     ; cd20e9000100
        jb     loc_14_0BC9
loc_14_0ABF:
        cmp    dword ptr [0x104], 0          ; -> obj6:0x0104
        jne    loc_14_0B88
        mov    eax, 0x110
        push   eax
        shl    eax, 2
        push   0x100
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    ecx
        or     eax, eax
        je     loc_14_0BC9
        mov    dword ptr [0x104], ecx        ; -> obj6:0x0104
        mov    dword ptr [0x108], eax        ; -> obj6:0x0108
        mov    edi, eax
        mov    eax, 0x80000000
loc_14_0AFF:
        mov    esi, 0x344                    ; -> loc_19_0344
        push   eax
        push   ebx
        push   ecx
        and    eax, 0x1ff
        xchg   ecx, eax
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        cmp    ecx, eax
        jb     loc_14_0B25
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        cmp    ecx, eax
        ja     loc_14_0B25
        sub    edx, edx
        jmp    loc_14_0B3E
loc_14_0B25:
        sub    ebx, ebx
        VxDCall  V86MMGR.V86MMGR_GetPgStatus     ; cd2010000600
        mov    edx, eax
        and    eax, 0x1200
        cmp    eax, 0x1000
        jne    loc_14_0B3E
        or     edx, 2
loc_14_0B3E:
        pop    ecx
        pop    ebx
        pop    eax
        test   dl, 3
        je     loc_14_0B7B
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [edx + ebx + 1], 0x20
        jne    loc_14_0B6D
        push   eax
        push   ecx
        shl    eax, 0xc
        push   0x1000
        push   eax
        VMMCall  VMM._GetInstanceInfo            ; cd2039010100
        add    esp, 8
        cmp    al, 2
        pop    ecx
        pop    eax
        je     loc_14_0B7B
loc_14_0B6D:
        push   edi
        VMMCall  VMM.Hook_V86_Page               ; cd2071000100
        mov    dword ptr [0x110], edi        ; -> obj6:0x0110
        pop    edi
loc_14_0B7B:
        mov    dword ptr [edi], esi
        add    edi, 4
        inc    eax
        dec    ecx
        jne    loc_14_0AFF
loc_14_0B88:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [edi + ebx + 0x54], 0
        jne    loc_14_0BC2
        mov    eax, dword ptr [0x104]        ; -> obj6:0x0104
        add    eax, 0x1f
        shr    eax, 3
        push   0x101
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_14_0BC9
        mov    dword ptr [edi + ebx + 0x54], eax
        inc    dword ptr [0x100]             ; -> obj6:0x0100
        or     byte ptr [edi + ebx + 1], 0x10
loc_14_0BC2:
        sub    eax, eax
        call   sub_19_02A1
loc_14_0BC9:
        pop    edi
        pop    esi
        ret    
loc_14_0BCC:
        sub    eax, eax
        mov    esi, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [0x314], eax        ; -> loc_1_0314
        je     loc_14_0C21
        cmp    byte ptr [0x320], al          ; -> loc_1_0320
        je     loc_14_0C21
        mov    ecx, dword ptr [0x318]        ; -> loc_1_0318
        jecxz  loc_14_0C21
        push   0x100
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_14_0C23
        mov    dword ptr [esi + ebx + 0x20], eax
        push   0x100
        push   0
        push   dword ptr [0x318]             ; -> loc_1_0318
        push   eax
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_14_0C21:
        clc    
loc_14_0C22:
        ret    
loc_14_0C23:
        mov    eax, 0x20005
        xor    edx, edx
        xor    ecx, ecx
        xor    ebx, ebx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        jmp    loc_14_0C22
sub_14_0C38:
        cmp    dword ptr [0x314], 0          ; -> loc_1_0314
        je     loc_14_0DAC
        cmp    byte ptr [0x320], 0           ; -> loc_1_0320
        je     loc_14_0DAC
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_14_0E11
        cmp    dword ptr [0x318], 0          ; -> loc_1_0318
        je     loc_14_0DAC
        VxDCall  SHELL.SHELL_Get_Version         ; cd2000001700
        cmp    eax, 0x30a
        jb     loc_14_0C94
        VxDCall  SHELL.SHELL_GetVMInfo           ; cd2005001700
        test   eax, 0x40000
        je     loc_14_0C94
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        or     byte ptr [edx + ebx + 1], 1
        xor    eax, eax
        jmp    loc_14_0CA8
loc_14_0C94:
        test   byte ptr [0x210], 2           ; -> loc_1_0210
        je     loc_14_0CC6
        mov    eax, 0x100
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
loc_14_0CA8:
        push   eax
        push   0
        push   dword ptr [0x318]             ; -> loc_1_0318
        push   dword ptr [edx + ebx + 0x20]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_14_0E16
loc_14_0CC6:
        mov    esi, 0x214                    ; -> loc_1_0214
        mov    ecx, dword ptr [0x314]        ; -> loc_1_0314
loc_14_0CD1:
        push   ecx
        movzx  eax, word ptr [esi]
        dec    eax
        shr    eax, 8
        movzx  ecx, word ptr [esi + 2]
        inc    ecx
        test   byte ptr [esi + 4], 2
        je     loc_14_0CE5
        inc    ecx
loc_14_0CE5:
        shr    ecx, 8
        push   ecx
        push   eax
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [esi + 5], 1
        jne    loc_14_0D0E
        test   byte ptr [edx + ebx + 1], 1
        jne    loc_14_0D0E
        push   0x200
        push   ecx
        push   eax
        push   ebx
        VMMCall  VMM._SetResetV86Pageable        ; cd2065000100
        add    esp, 0x10
loc_14_0D0E:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        push   0
        push   dword ptr [edx + ebx + 0x20]
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        add    esp, 8
        or     eax, eax
        je     loc_14_0E13
        mov    edi, edx
        mov    eax, dword ptr [esi + 0xc]
        shl    eax, 0xc
        add    edi, eax
        mov    al, 0x4d
        test   byte ptr [esi + 4], 1
        je     loc_14_0D3F
        mov    al, 0x5a
loc_14_0D3F:
        mov    byte ptr [edi], al
        test   byte ptr [esi + 5], 1
        je     loc_14_0D4F
        mov    word ptr [edi + 1], 8
        jmp    loc_14_0D55
loc_14_0D4F:
        mov    word ptr [edi + 1], 0
loc_14_0D55:
        movzx  eax, word ptr [esi + 2]
        mov    word ptr [edi + 3], ax
        test   byte ptr [esi + 4], 2
        je     loc_14_0D80
        mov    eax, dword ptr [esp + 4]
        shl    eax, 0xc
        sub    eax, 0x10
        add    edi, eax
        push   esi
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        je     loc_14_0D7F
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_14_0D7F:
        pop    esi
loc_14_0D80:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        pop    eax
        pop    ecx
        push   0
        push   dword ptr [esi + 0xc]
        push   ecx
        push   eax
        push   ebx
        push   dword ptr [edx + ebx + 0x20]
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        pop    ecx
        or     eax, eax
        je     loc_14_0E16
        add    esi, 0x10
        dec    ecx
        jne    loc_14_0CD1
loc_14_0DAC:
        test   byte ptr [0x210], 4           ; -> loc_1_0210
        je     loc_14_0E11
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 0x1c], 0x5803
        mov    word ptr [ebp + 0x10], 0
        test   byte ptr [0x210], 2           ; -> loc_1_0210
        je     loc_14_0DF1
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        or     byte ptr [edx + ebx + 1], 4
        inc    word ptr [ebp + 0x10]
loc_14_0DF1:
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_14_0E11:
        clc    
loc_14_0E12:
        ret    
loc_14_0E13:
        add    esp, 0xc
loc_14_0E16:
        mov    eax, 0x20005
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        jmp    loc_14_0E12
        db  000h, 000h, 000h
loc_14_0E2C:
        mov    edx, 0x310                    ; -> obj6:0x0310
        cmp    byte ptr [ebp + 0x1c], 0xa
        jne    loc_14_0E3C
        mov    edx, 0x312                    ; -> obj6:0x0312
loc_14_0E3C:
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_14_0E42:
        add    edi, dword ptr [4]            ; -> obj6:0x0004
        xor    esi, esi
        test   dl, 1
        je     loc_14_0E52
        or     esi, 0x40
loc_14_0E52:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x62
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  edx, word ptr [ebp + 0x10]
        mov    word ptr [edi + 0x18], dx
        mov    eax, edx
        shl    eax, 4
        mov    ecx, 0xff
        call   sub_14_0FF2
        jb     loc_14_0FDB
        push   eax
        mov    eax, edx
        shl    eax, 4
        movzx  eax, word ptr [eax + 0x2c]
        shl    eax, 4
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_14_0EDD
        cmp    word ptr [eax], 0
        jne    loc_14_0EDD
        pushal 
        xor    esi, esi
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        mov    dword ptr [esp + 0x1c], edx
        mov    edi, edx
        lea    esi, [eax + 4]
        or     ecx, 0xffffffff
        xor    eax, eax
        cld    
loc_14_0EC6:
        repne scasb al, byte ptr es:[edi]
        cmp    byte ptr [edi], al
        jne    loc_14_0EC6
        add    edi, 3
        push   -1
        push   esi
        push   edi
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        popal  
loc_14_0EDD:
        movzx  ecx, word ptr [eax - 0xd]
        shl    ecx, 4
        dec    ecx
        call   sub_14_0FF2
        jb     loc_14_0FCD
        push   eax
        mov    byte ptr [ebp + 0x1d], 0x2f
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    dx, word ptr [ebp + 0x38]
        jne    loc_14_0F14
        cmp    word ptr [ebp + 0x10], 0x80
        jne    loc_14_0F14
        mov    eax, dword ptr [esp + 4]
        jmp    loc_14_0F2B
loc_14_0F14:
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        mov    ecx, 0xffff
        call   sub_14_0FF2
        jb     loc_14_0FBF
loc_14_0F2B:
        mov    esi, dword ptr [0]            ; -> obj6:0x0000
        add    esi, ebx
        xchg   word ptr [esi + 0x36], ax
        movzx  ecx, word ptr [ebp + 0x10]
        xchg   dword ptr [esi + 0x38], ecx
        mov    word ptr [edi + 0x14], ax
        mov    dword ptr [edi + 0x10], ecx
        xor    eax, eax
        mov    dword ptr [esi + 0x3c], eax
        xor    eax, eax
        mov    al, 0x23
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    word ptr [edi + 8], cx
        mov    dword ptr [edi + 4], edx
        xor    eax, eax
        mov    al, 0x24
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    word ptr [edi + 0xe], cx
        mov    dword ptr [edi + 0xa], edx
        pop    eax
        pop    edx
        push   fs
        mov    fs, dx
        mov    word ptr fs:[0x2c], ax
        pop    fs
        mov    esi, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        xchg   edi, esi
        mov    eax, edx
        xchg   word ptr [esi + 0x2a], ax
        mov    word ptr [edi + 0x16], ax
        mov    eax, dword ptr [0x40]         ; -> obj6:0x0040
        mov    dword ptr [0x8c], eax
        mov    eax, dword ptr [0x44]         ; -> obj6:0x0044
        mov    dword ptr [0x90], eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    word ptr [ebp + 0x38], dx
        clc    
        ret    
loc_14_0FBF:
        pop    eax
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._Free_LDT_Selector          ; cd2079000100
        add    esp, 0xc
loc_14_0FCD:
        pop    eax
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._Free_LDT_Selector          ; cd2079000100
        add    esp, 0xc
loc_14_0FDB:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        stc    
        ret    
sub_14_0FF2:
        push   edx
        push   0
        push   esi
        push   0x92
        push   ecx
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        pop    edx
        cmp    eax, 1
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 15  vsize=0x330  R-X
loc_15_0000:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        btr    word ptr [edx + ebx], 7
        jae    loc_15_0018
        mov    esi, dword ptr [edx + ebx + 0x44]
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
loc_15_0018:
        jmp    loc_15_0031
loc_15_001A:
        call   sub_15_0190
        cmp    ebx, dword ptr [0x50]         ; -> obj6:0x0050
        jne    loc_15_0031
        mov    dword ptr [0x50], 0           ; -> obj6:0x0050
loc_15_0031:
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        xor    eax, eax
        xchg   dword ptr [ebx + 0x48], eax
        or     eax, eax
        je     loc_15_004C
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_15_004C:
        xor    eax, eax
        xchg   dword ptr [ebx + 0x20], eax
        or     eax, eax
        je     loc_15_0061
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_15_0061:
        clc    
        ret    
loc_15_0063:
        sub    esp, 4
        mov    dword ptr [esp], 1
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
        shl    edx, 0xc
        shr    dx, 0xc
        add    dx, 0x174
        mov    word ptr [ebp + 0x30], dx
        shr    edx, 0x10
        mov    word ptr [ebp + 0x34], dx
loc_15_0090:
        mov    byte ptr [ebp + 0x1d], 0x51
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x10]
        cmp    ax, word ptr [0x34]           ; -> obj6:0x0034
        je     loc_15_013A
        shl    eax, 4
        mov    edi, eax
        push   0
        push   0x100
        push   eax
        VMMCall  VMM._TestGlobalV86Mem           ; cd205f000100
        add    esp, 0xc
        test   eax, eax
        jne    loc_15_0150
        mov    ecx, dword ptr [ebx + 4]
        mov    ax, word ptr [ebp + 0x28]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x24]
        mov    dword ptr [ecx + edi + 0xa], eax
        movzx  esi, word ptr [ecx + edi + 0x16]
        shl    esi, 4
        cmp    esi, edi
        jne    loc_15_010C
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shr    esi, 0xc
        cmp    eax, esi
        je     loc_15_00FD
        and    dword ptr [esp], 0
loc_15_00FD:
        movzx  esi, word ptr [0x34]          ; -> obj6:0x0034
        mov    word ptr [ecx + edi + 0x16], si
        shl    esi, 4
loc_15_010C:
        mov    ax, word ptr [ebp + 0x34]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x30]
        sub    ax, 0x18
        xchg   dword ptr [ecx + esi + 0x2e], eax
        mov    word ptr [ebp + 0x1c], 0x4cff
        push   eax
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    dword ptr [ecx + esi + 0x2e]
        jmp    loc_15_0090
loc_15_013A:
        cmp    dword ptr [esp], 0
        je     loc_15_0150
        mov    ecx, 1
        mov    eax, 0x80000000
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
loc_15_0150:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        add    esp, 4
        clc    
        ret    
        db  000h
loc_15_015C:
        mov    word ptr [ebp + 0x30], dx
        shr    edx, 0x10
        mov    word ptr [ebp + 0x34], dx
        mov    ecx, dword ptr [0]            ; -> obj6:0x0000
        bts    word ptr [ecx + ebx], 1
        jae    loc_11_024E
        btr    word ptr [ecx + ebx], 5
        cmp    dword ptr [ecx + ebx + 0x18], 0
        je     loc_15_018E
        mov    edx, dword ptr [ecx + ebx + 0x1c]
        call   dword ptr [ecx + ebx + 0x18]
loc_15_018E:
        ret    
        db  000h
sub_15_0190:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        sub    ecx, ecx
        xchg   dword ptr [edi + ebx + 0x5c], ecx
        jecxz  loc_15_01AA
        push   0
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_15_01AA:
        and    byte ptr [edi + ebx + 1], 0xef
        cmp    dword ptr [0x100], 0          ; -> obj6:0x0100
        je     loc_15_01DF
        mov    ecx, dword ptr [edi + ebx + 0x54]
        jecxz  loc_15_022C
        or     eax, 0xffffffff
        call   sub_19_02A1
        sub    eax, eax
        mov    dword ptr [edi + ebx + 0x54], eax
        push   eax
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        dec    dword ptr [0x100]             ; -> obj6:0x0100
        jne    loc_15_022C
loc_15_01DF:
        mov    ecx, dword ptr [0x104]        ; -> obj6:0x0104
        jecxz  loc_15_022C
        push   ebx
        push   esi
        push   edi
        mov    eax, 0x80000000
        sub    edx, edx
        mov    ebx, dword ptr [0x108]        ; -> obj6:0x0108
        mov    esi, 0x344                    ; -> loc_19_0344
loc_15_01FC:
        mov    edi, dword ptr [ebx + edx*4]
        cmp    edi, esi
        je     loc_15_020E
        VMMCall  VMM.Unhook_V86_Page             ; cd203d010100
        jb     loc_15_0229
        mov    dword ptr [ebx + edx*4], esi
loc_15_020E:
        inc    eax
        inc    edx
        loop   loc_15_01FC
        mov    dword ptr [0x104], ecx        ; -> obj6:0x0104
        mov    dword ptr [0x108], ecx        ; -> obj6:0x0108
        push   ecx
        push   ebx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_15_0229:
        pop    edi
        pop    esi
        pop    ebx
loc_15_022C:
        ret    
        db  000h, 000h, 000h
loc_15_0230:
        mov    esi, dword ptr [0]            ; -> obj6:0x0000
        add    esi, ebx
        add    edi, dword ptr [4]            ; -> obj6:0x0004
        call   sub_15_0284
        call   sub_15_02AB
        push   ebx
        mov    ebx, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        xchg   edi, ebx
        mov    ax, word ptr [edi + 0x16]
        mov    word ptr [ebx + 0x2a], ax
        pop    ebx
        mov    ax, word ptr [edi + 0x14]
        mov    word ptr [esi + 0x36], ax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x38], eax
        xor    eax, eax
        mov    dword ptr [esi + 0x3c], eax
loc_15_026F:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_15_0282
        push   edi
        movzx  edi, word ptr [eax + 8]
        call   sub_11_0ECC
        pop    edi
        jmp    loc_15_026F
loc_15_0282:
        clc    
        ret    
sub_15_0284:
        pushal 
        mov    cx, word ptr [edi + 8]
        mov    edx, dword ptr [edi + 4]
        mov    eax, 0x23
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    cx, word ptr [edi + 0xe]
        mov    edx, dword ptr [edi + 0xa]
        mov    eax, 0x24
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        popal  
        ret    
sub_15_02AB:
        push   edi
        push   fs
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    fs, word ptr [edi + 0x2a]
        movzx  eax, word ptr fs:[0x2c]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, dword ptr [ebx + 4]
        jb     loc_15_02D5
        sub    eax, dword ptr [ebx + 4]
loc_15_02D5:
        shr    eax, 4
        test   eax, 0xffff0000
        je     loc_15_02E1
        xor    eax, eax
loc_15_02E1:
        xchg   word ptr fs:[0x2c], ax
        pop    fs
        pop    edi
        ret    
loc_15_02ED:
        pushal 
        VMMCall  VMM.Get_Cur_PM_App_CB           ; cd2088000100
        add    edi, dword ptr [4]            ; -> obj6:0x0004
        call   sub_15_0284
        call   sub_15_02AB
        mov    eax, 0x21
        VMMCall  VMM.Simulate_Int                ; cd2046000100
        popal  
        ret    
loc_15_0311:
        pushal 
        VMMCall  VMM.Get_Cur_PM_App_CB           ; cd2088000100
        mov    eax, 0x16
        VMMCall  VMM.System_Control              ; cd2093000100
        mov    eax, 0x21
        VMMCall  VMM.Simulate_Int                ; cd2046000100
        popal  
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 16  vsize=0x4  R-X
DOSMGR_End_V86_App:
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 17  vsize=0x6c  R-X
loc_17_0000:
        xor    eax, eax
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [0x314], eax        ; -> loc_1_0314
        je     loc_17_0049
        cmp    byte ptr [0x320], al          ; -> loc_1_0320
        je     loc_17_0049
        cmp    dword ptr [0x318], eax        ; -> loc_1_0318
        je     loc_17_0049
        test   byte ptr [edi + ebx + 1], 1
        jne    loc_17_0033
        mov    eax, 0x100
        test   byte ptr [edi + ebx + 1], 4
        je     loc_17_0049
loc_17_0033:
        push   eax
        push   0
        push   dword ptr [0x318]             ; -> loc_1_0318
        push   dword ptr [edi + ebx + 0x20]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_17_0049:
        test   byte ptr [edi + ebx + 1], 1
        je     loc_17_006A
        cmp    dword ptr [edi + ebx + 0x48], 0
        je     loc_17_006A
        push   0
        push   0
        push   1
        push   dword ptr [edi + ebx + 0x48]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_17_006A:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 18  vsize=0xa0  R-X
loc_18_0000:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [edi + ebx + 1], 1
        je     loc_18_002B
        cmp    dword ptr [edi + ebx + 0x48], 0
        je     loc_18_002B
        push   0
        push   0
        push   1
        push   dword ptr [edi + ebx + 0x48]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_18_009C
loc_18_002B:
        xor    eax, eax
        cmp    dword ptr [0x314], eax        ; -> loc_1_0314
        je     loc_18_007A
        cmp    byte ptr [0x320], al          ; -> loc_1_0320
        je     loc_18_007A
        cmp    dword ptr [0x318], eax        ; -> loc_1_0318
        je     loc_18_007A
        bts    word ptr [edi + ebx], 9
        jae    loc_18_007A
        test   byte ptr [edi + ebx + 1], 1
        jne    loc_18_0060
        mov    eax, 0x100
        test   byte ptr [edi + ebx + 1], 4
        je     loc_18_007A
loc_18_0060:
        push   eax
        push   0
        push   dword ptr [0x318]             ; -> loc_1_0318
        push   dword ptr [edi + ebx + 0x20]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_18_007B
loc_18_007A:
        ret    
loc_18_007B:
        test   byte ptr [edi + ebx + 1], 1
        je     loc_18_009C
        cmp    dword ptr [edi + ebx + 0x48], 0
        je     loc_18_009C
        push   0
        push   0
        push   1
        push   dword ptr [edi + ebx + 0x48]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_18_009C:
        stc    
        jmp    loc_18_007A
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 19  vsize=0x550  R-X
DOSMGR_V86_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 6
        jae    loc_19_0012
        jmp    dword ptr [eax*4 + 0x98]      ; -> obj6:0x0098
loc_19_0012:
        or     word ptr [ebp + 0x1c], 0xffff
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_19_001C:
        VxDCall  DOSMGR.DOSMGR_Get_Version       ; cd2000001500
        mov    word ptr [ebp + 0x1c], ax
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_19_002B:
        call   sub_11_02F8
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_19_0035:
        mov    eax, 0x10007
        lea    edx, [0x1b0]                  ; -> obj1:0x01B0
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        VMMCall  VMM.Crash_Cur_VM                ; cd202f800100
loc_19_004F:
        mov    edx, dword ptr [0x1f8]        ; -> loc_1_01F8
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    byte ptr [edx + 1], 0xff
        inc    byte ptr [edx]
        ret    
loc_19_0065:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_19_006A:
        cmp    byte ptr [0x1c], 0            ; -> obj6:0x001C
        je     loc_19_0082
        or     byte ptr [ebp + 0x2c], 1
        movzx  eax, byte ptr [ebp + 0x10]
        call   sub_19_0087
        jb     loc_19_0086
loc_19_0082:
        and    byte ptr [ebp + 0x2c], 0xfe
loc_19_0086:
        ret    
sub_19_0087:
        pushal 
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        add    edi, ebx
        xor    esi, esi
        xchg   dword ptr [edi + 0x28], esi
        VMMCall  VMM.Cancel_VM_Event             ; cd2013000100
        xor    ecx, ecx
        cmp    dword ptr [0x28], ecx         ; -> obj6:0x0028
        je     loc_19_012E
        cmp    dword ptr [0x20], ecx         ; -> obj6:0x0020
        je     loc_19_012E
        cmp    dword ptr [0x24], ecx         ; -> obj6:0x0024
        je     loc_19_012E
        cmp    eax, 0x1a
        jae    loc_19_00C2
        btr    dword ptr [edi + 0x24], eax
loc_19_00C2:
        call   sub_19_014A
        cmp    eax, dword ptr [0x28]         ; -> obj6:0x0028
        jae    loc_19_0147
        mov    esi, dword ptr [0x20]         ; -> obj6:0x0020
        push   eax
        mul    dword ptr [0x24]              ; -> obj6:0x0024
        lea    esi, [eax + esi]
        pop    eax
        test   byte ptr [esi + 0x44], 0x40
        je     loc_19_0147
        test   byte ptr [esi + 0x44], 0xa0
        jne    loc_19_0147
        mov    ecx, ebx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, ecx
        je     loc_19_012E
loc_19_00F8:
        test   dword ptr [ebx], 0x10
        jne    loc_19_0124
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        add    edi, ebx
        bts    dword ptr [edi + 0x24], eax
        test   dword ptr [ebx], 8
        jne    loc_19_0131
        xor    esi, esi
        xchg   dword ptr [edi + 0x28], esi
        VMMCall  VMM.Cancel_VM_Event             ; cd2013000100
        call   sub_19_014A
loc_19_0124:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, ecx
        jne    loc_19_00F8
loc_19_012E:
        clc    
loc_19_012F:
        popal  
        ret    
loc_19_0131:
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_19_0124
        mov    esi, 0x14a                    ; -> sub_19_014A
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        mov    dword ptr [edi + 0x28], esi
        jmp    loc_19_0124
loc_19_0147:
        stc    
        jmp    loc_19_012F
sub_19_014A:
        pushal 
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        add    edi, ebx
        mov    dword ptr [edi + 0x28], 0
        mov    ecx, dword ptr [0x28]         ; -> obj6:0x0028
        xor    eax, eax
        mov    esi, dword ptr [0x20]         ; -> obj6:0x0020
        add    esi, dword ptr [ebx + 4]
loc_19_016B:
        btr    dword ptr [edi + 0x24], eax
        jae    loc_19_0183
        test   byte ptr [esi + 0x44], 0x40
        je     loc_19_0183
        test   byte ptr [esi + 0x44], 0xa0
        jne    loc_19_0183
        mov    word ptr [esi + 0x49], 0xffff
loc_19_0183:
        add    esi, dword ptr [0x24]         ; -> obj6:0x0024
        inc    eax
        loop   loc_19_016B
        popal  
        ret    
        db  000h, 000h
loc_19_0190:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_19_019B:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [edi + ebx], 0x20
        je     loc_19_01C4
        mov    eax, dword ptr [0xc]          ; -> obj6:0x000C
        mov    edi, dword ptr [ebx + 4]
        cmp    dword ptr [edi + 0x8c], eax
        jne    loc_19_01C6
        mov    eax, dword ptr [ebp + 0x2c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        or     byte ptr [ebp + 0x2c], 1
loc_19_01C4:
        clc    
        ret    
loc_19_01C6:
        stc    
        ret    
loc_19_01C8:
        cmp    ebx, dword ptr [0x50]         ; -> obj6:0x0050
        je     loc_19_0214
        cmp    dword ptr [0x50], 0           ; -> obj6:0x0050
        jne    loc_19_0225
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    dword ptr [0x50], ebx         ; -> obj6:0x0050
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        mov    esi, 0x22b                    ; -> loc_19_022B
        mov    eax, 0x3e8
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        mov    cx, word ptr [0x66]           ; -> obj6:0x0066
        mov    dx, word ptr [0x64]           ; -> obj6:0x0064
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c800100
loc_19_0214:
        mov    eax, dword ptr [0x64]         ; -> obj6:0x0064
        mov    word ptr [ebp + 0x24], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x28], ax
        ret    
loc_19_0225:
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
loc_19_022B:
        mov    dword ptr [0x50], 0           ; -> obj6:0x0050
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
loc_19_023B:
        add    ebx, dword ptr [0]            ; -> obj6:0x0000
        test   byte ptr [ebx], 0x20
        je     loc_19_0248
        stc    
        ret    
loc_19_0248:
        clc    
        ret    
loc_19_024A:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        mov    edx, dword ptr [0x18c]        ; -> obj6:0x018C
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        add    edx, dword ptr [ebx + 4]
        or     byte ptr [eax + ebx], 0x30
        movzx  edx, word ptr [edx + 0x330]
        cmp    word ptr [eax + ebx + 2], 0
        jne    loc_19_0276
        mov    word ptr [eax + ebx + 2], dx
loc_19_0276:
        mov    esi, edx
        shl    esi, 4
        add    esi, dword ptr [ebx + 4]
        VxDCall  DOSMGR.DOSMGR_Begin_V86_App     ; cd200d801500
DOSMGR_Begin_V86_App:
        ret    
        db  000h, 000h, 000h
loc_19_0288:
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        and    byte ptr [edx + ebx + 1], 0xf7
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        sub    eax, eax
        call   sub_19_02A1
        ret    
sub_19_02A1:
        push   ecx
        push   edx
        push   esi
        push   edi
        sub    esi, esi
        mov    ecx, dword ptr [0x104]        ; -> obj6:0x0104
        jecxz  loc_19_030C
        or     eax, eax
        mov    edx, 0xfffffffd
        je     loc_19_02BE
        or     eax, 0xffffffff
        xchg   edx, eax
        not    eax
loc_19_02BE:
        mov    edi, dword ptr [0x108]        ; -> obj6:0x0108
        cmp    dword ptr [edi + esi*4], 0x344 ; -> loc_19_0344
        je     loc_19_0303
        cmp    esi, 0xa0
        jb     loc_19_02E0
        mov    edi, dword ptr [edi + esi*4]
        cmp    edi, dword ptr [0x110]        ; -> obj6:0x0110
        jne    loc_19_0303
loc_19_02E0:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        mov    edi, dword ptr [edi + ebx + 0x54]
        test   eax, eax
        jne    loc_19_02F5
        bts    dword ptr [edi], esi
        jae    loc_19_02FA
        jmp    loc_19_0303
loc_19_02F5:
        btr    dword ptr [edi], esi
        jae    loc_19_0303
loc_19_02FA:
        mov    edi, dword ptr [ebx - 0x10]
        and    dword ptr [edi + esi*4], edx
        or     dword ptr [edi + esi*4], eax
loc_19_0303:
        inc    esi
        loop   loc_19_02BE
        mov    eax, cr3
        mov    cr3, eax
loc_19_030C:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        ret    
loc_19_0311:
        mov    eax, dword ptr [edi + 4]
        mov    esi, dword ptr [edi]
        cmp    eax, dword ptr [0x104]        ; -> obj6:0x0104
        cmc    
        jb     loc_19_033A
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        mov    ecx, dword ptr [edi + ebx + 0x54]
        cmp    ecx, 1
        jb     loc_19_033A
        bt     dword ptr [ecx], eax
        cmc    
        jb     loc_19_033A
        call   sub_19_0357
        clc    
loc_19_033A:
        ret    
loc_19_033B:
        mov    edi, dword ptr [0x108]        ; -> obj6:0x0108
        jmp    dword ptr [edi + eax*4]
loc_19_0344:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        mov    ecx, dword ptr [edi + ebx + 0x54]
        test   ecx, ecx
        je     loc_19_033B
        bt     dword ptr [ecx], eax
        jae    loc_19_033B
sub_19_0357:
        test   byte ptr [edi + ebx + 1], 8
        jne    loc_19_04C4
        mov    ecx, dword ptr [ebx - 0x10]
        bts    dword ptr [ecx + eax*4], 0
        jae    loc_19_04D2
        test   dword ptr [ebp + 0x2c], 0x20000
        je     loc_19_04C4
        movzx  ecx, word ptr [ebp + 0x28]
        shl    ecx, 4
        add    ecx, dword ptr [ebp + 0x24]
        cmp    word ptr [ecx], 0x4730
        jne    loc_19_0398
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_19_04C4
loc_19_0398:
        shr    ecx, 0xc
        mov    edx, dword ptr [0x108]        ; -> obj6:0x0108
        cmp    dword ptr [edx + ecx*4], 0x344 ; -> loc_19_0344
        jne    loc_19_04C4
        movzx  ecx, word ptr [ebp + 0x34]
        shl    ecx, 4
        add    ecx, dword ptr [ebp + 0x30]
        shr    ecx, 0xc
        cmp    dword ptr [edx + ecx*4], 0x344 ; -> loc_19_0344
        jne    loc_19_04C4
        mov    edx, dword ptr [0x10c]        ; -> obj6:0x010C
        or     edx, edx
        je     loc_19_03E1
        mov    ecx, dword ptr [edx]
loc_19_03D4:
        add    edx, 4
        cmp    esi, dword ptr [edx]
        je     loc_19_04C4
        loop   loc_19_03D4
loc_19_03E1:
        push   eax
        test   byte ptr [edi + ebx + 1], 0x20
        jne    loc_19_03FD
        push   1
        push   esi
        VMMCall  VMM._GetInstanceInfo            ; cd2039010100
        add    esp, 8
        or     eax, eax
        jne    loc_19_04C3
loc_19_03FD:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_19_04C3
        mov    eax, ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        xchg   ebx, eax
        jecxz  loc_19_0418
        cmp    eax, ebx
        je     loc_19_0491
loc_19_0418:
        mov    eax, dword ptr [edi + ebx + 0x58]
        or     eax, eax
        jne    loc_19_0432
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        or     eax, eax
        je     loc_19_04C3
        mov    dword ptr [edi + ebx + 0x58], eax
loc_19_0432:
        mov    edx, dword ptr [edi + ebx + 0x5c]
        or     edx, edx
        jne    loc_19_0454
        push   0x100
        push   0x10
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    edx, eax
        or     eax, eax
        je     loc_19_04C3
        mov    dword ptr [edi + ebx + 0x5c], eax
loc_19_0454:
        mov    dword ptr [edx], ebx
        movzx  eax, word ptr [ebp + 0x28]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0xc], esi
        sub    eax, eax
        mov    ecx, 0x427
        mov    esi, 0x4d9                    ; -> loc_19_04D9
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_19_04C3
        mov    eax, dword ptr [edi + ebx + 0x58]
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cmp    dword ptr [edx], 0
        je     loc_19_04C3
        mov    esi, dword ptr [edx + 0xc]
loc_19_0491:
        mov    eax, dword ptr [0x10c]        ; -> obj6:0x010C
        or     eax, eax
        jne    loc_19_04B6
        push   0x101
        push   0x84
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_19_04C3
        mov    dword ptr [0x10c], eax        ; -> obj6:0x010C
loc_19_04B6:
        cmp    dword ptr [eax], 0x20
        jae    loc_19_04C3
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax + ecx*4 + 4], esi
        inc    dword ptr [eax]
loc_19_04C3:
        pop    eax
loc_19_04C4:
        mov    ecx, dword ptr [ebx - 0x10]
        or     byte ptr [ecx + eax*4], 2
        mov    ecx, dword ptr [edi + ebx + 0x54]
        btr    dword ptr [ecx], eax
loc_19_04D2:
        mov    ecx, cr3
        mov    cr3, ecx
        ret    
loc_19_04D9:
        mov    ebx, dword ptr [edx]
        jb     loc_19_0506
        cmp    dword ptr [ebp + 0x1c], 7
        jne    loc_19_0506
        mov    dword ptr [edx], 0
        or     cl, 1
        mov    eax, 0x10006
        sub    edx, edx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        sub    ecx, ecx
        mov    eax, 0x1388
        VMMCall  VMM.Close_VM                    ; cd20ec000100
loc_19_0506:
        mov    edi, dword ptr [0]            ; -> obj6:0x0000
        mov    eax, dword ptr [edi + ebx + 0x58]
        VMMCall  VMM.Signal_Semaphore            ; cd2028800100
        add    byte ptr [eax], al
loc_19_0518:
        mov    eax, 0x23
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        je     loc_19_0547
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    si, word ptr [ebp + 0x30]
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    si, word ptr [ebp + 0x30]
        je     loc_19_0541
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
loc_19_0541:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
loc_19_0547:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        ret    
        db  024h, 044h
VxD__LTEXT ENDS
