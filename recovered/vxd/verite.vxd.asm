; VERITE -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 307 instructions recovered by recursive descent

Declare_Virtual_Device VERITE, 3, 1, VERITE_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x244  R-X
VERITE_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_3_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_3_0000
sub_1_007D:
        mov    ecx, 0xdf
        mov    edx, dword ptr [0x13c]        ; -> obj1:0x013C
        add    edx, ecx
loc_1_008A:
        mov    esi, 0xa1                     ; -> loc_2_00A1
        push   ecx
        push   edx
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        pop    edx
        push   edx
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        pop    edx
        pop    ecx
        dec    edx
        dec    ecx
        jge    loc_1_008A
        mov    ebx, dword ptr [0x14c]        ; -> obj1:0x014C
        call   sub_1_00CD
        ret    
sub_1_00B1:
        mov    ecx, 0xdf
        mov    edx, dword ptr [0x13c]        ; -> obj1:0x013C
        add    edx, ecx
loc_1_00BE:
        push   ecx
        push   edx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        pop    edx
        pop    ecx
        dec    edx
        dec    ecx
        jge    loc_1_00BE
        ret    
sub_1_00CD:
        mov    ecx, 0xdf
        mov    edx, dword ptr [0x13c]        ; -> obj1:0x013C
        add    edx, ecx
loc_1_00DA:
        push   ecx
        push   edx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        pop    edx
        pop    ecx
        dec    edx
        dec    ecx
        jge    loc_1_00DA
        ret    
        db  0CCh, 0CCh, 0CCh
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VERITE  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  VERITE_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 000h
        db  06Ah, 0A0h, 09Ah, 031h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 00Dh, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ah, 0A0h, 09Ah, 031h
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 010h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  010h, 050h, 000h, 000h, 034h, 027h, 034h, 029h, 029h, 029h, 0B1h, 0FFh, 0FFh, 0FFh, 0FFh, 029h
        db  029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 0B1h, 0B1h, 029h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xa2  R-X
loc_2_0000:
        mov    ecx, dword ptr [ebp + 0x18]
        test   ecx, ecx
        je     loc_2_0013
        cmp    ecx, 1
        je     loc_2_0025
        cmp    ecx, 2
        je     loc_2_0047
        jmp    loc_2_0054
loc_2_0013:
        mov    eax, dword ptr [0x13c]        ; -> obj1:0x013C
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [0x140]        ; -> obj1:0x0140
        mov    dword ptr [ebp + 0x10], eax
        jmp    loc_2_0054
loc_2_0025:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, eax
        and    eax, 0xff00
        mov    dword ptr [0x13c], eax        ; -> obj1:0x013C
        and    ecx, 0xff000000
        mov    dword ptr [0x140], ecx        ; -> obj1:0x0140
        call   sub_1_007D
        jmp    loc_2_0054
loc_2_0047:
        mov    ebx, dword ptr [0x14c]        ; -> obj1:0x014C
        call   sub_1_00B1
        jmp    loc_2_0054
loc_2_0054:
        ret    
loc_2_0055:
        pushal 
        mov    dword ptr [0x144], ebx        ; -> obj1:0x0144
        call   sub_1_00B1
        popal  
        ret    
loc_2_0063:
        pushal 
        mov    ebx, dword ptr [0x14c]        ; -> obj1:0x014C
        call   sub_1_00CD
        popal  
        ret    
loc_2_0071:
        pushal 
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        mov    eax, dword ptr [0x144]        ; -> obj1:0x0144
loc_2_007D:
        cmp    eax, edi
        jne    loc_2_0088
        mov    ebx, edi
        call   sub_1_00CD
loc_2_0088:
        mov    ebx, dword ptr [0x14c]        ; -> obj1:0x014C
        call   sub_1_00B1
        xor    eax, eax
        mov    dword ptr [0x144], eax        ; -> obj1:0x0144
        popal  
        ret    
loc_2_009C:
        ret    
loc_2_009D:
        ret    
loc_2_009E:
        ret    
loc_2_009F:
        ret    
loc_2_00A0:
        ret    
loc_2_00A1:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x23a  R-X
loc_3_0000:
        push   ebx
        mov    dword ptr [0x14c], ebx        ; -> obj1:0x014C
        push   0
        push   0x82
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x150], eax        ; -> obj1:0x0150
        or     eax, eax
        je     loc_3_012D
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_3_012D
        mov    dword ptr [edi], 0            ; -> loc_2_0000
        mov    dword ptr [edi + 0x20], 0x9f  ; -> loc_2_009F
        mov    dword ptr [edi + 0x24], 0xa0  ; -> loc_2_00A0
        mov    dword ptr [edi + 0x10], 0x55  ; -> loc_2_0055
        mov    dword ptr [edi + 0x14], 0x63  ; -> loc_2_0063
        mov    dword ptr [edi + 0x18], 0x71  ; -> loc_2_0071
        mov    dword ptr [edi + 0x1c], 0x9c  ; -> loc_2_009C
        mov    dword ptr [edi + 0x34], 0x9d  ; -> loc_2_009D
        mov    dword ptr [edi + 0x38], 0x9e  ; -> loc_2_009E
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x158], edi        ; -> obj1:0x0158
        mov    dword ptr [0x154], esi        ; -> obj1:0x0154
        xor    eax, eax
        mov    dword ptr [0x148], eax        ; -> obj1:0x0148
        lea    eax, [0x130]                  ; -> loc_3_0130
        mov    ecx, 0x32
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        cmp    eax, -1
        je     loc_3_0112
        push   0
        push   dword ptr [0x134]             ; -> loc_3_0134
        push   0x162                         ; -> loc_3_0162
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b003300
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_3_0112
        xor    ecx, ecx
        mov    cx, word ptr [0x162]          ; -> loc_3_0162
        dec    ecx
loc_3_00C5:
        mov    eax, dword ptr [ecx*4 + 0x164] ; -> loc_3_0164
        test   eax, 0xffffff
        je     loc_3_00D6
        dec    ecx
        jge    loc_3_00C5
loc_3_00D6:
        test   eax, 0xffffff
        jne    loc_3_0112
        mov    dword ptr [0x140], eax        ; -> obj1:0x0140
        xor    eax, eax
        xor    ecx, ecx
        mov    cx, word ptr [0x1be]          ; -> loc_3_01BE
        dec    ecx
loc_3_00EE:
        mov    ax, word ptr [ecx*2 + 0x1c0]  ; -> loc_3_01C0
        test   eax, 0xff
        je     loc_3_0100
        dec    ecx
        jge    loc_3_00EE
loc_3_0100:
        test   eax, 0xff
        jne    loc_3_0112
        mov    dword ptr [0x13c], eax        ; -> obj1:0x013C
        inc    dword ptr [0x148]             ; -> obj1:0x0148
loc_3_0112:
        test   dword ptr [0x148], 1          ; -> obj1:0x0148
        jne    loc_3_012A
        xor    eax, eax
        mov    dword ptr [0x13c], eax        ; -> obj1:0x013C
        mov    dword ptr [0x140], eax        ; -> obj1:0x0140
loc_3_012A:
        clc    
        jmp    loc_3_012E
loc_3_012D:
        stc    
loc_3_012E:
        pop    ebx
        ret    
loc_3_0130:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_3_0134:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0162:
        add    byte ptr [eax], al
loc_3_0164:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_01BE:
        add    byte ptr [eax], al
loc_3_01C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
