; VPMTD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 131 instructions recovered by recursive descent

Declare_Virtual_Device VPMTD, 1, 0, VPMTD_Control, 0003Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x248  R-X
VPMTD_Control:
        cmp    eax, 1
        je     loc_2_0000
        clc    
        ret    
VPMTD_PM_API:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0025
        movzx  eax, word ptr [ebp + 4]
        cmp    eax, 3
        ja     loc_1_0025
        jmp    dword ptr [eax*4 + 0x238]     ; -> obj1:0x0238
loc_1_0025:
        or     dword ptr [ebp + 0x2c], 1
        ret    
loc_1_002A:
        and    dword ptr [ebp + 0x2c], 0xfffffffe
        ret    
loc_1_002F:
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [0x220], ax          ; -> obj1:0x0220
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0x222], ax          ; -> obj1:0x0222
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [0x224], ax          ; -> obj1:0x0224
        xor    eax, eax
        mov    ax, word ptr [0x224]          ; -> obj1:0x0224
        mov    esi, 0x196                    ; -> loc_1_0196
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        or     esi, esi
        jne    loc_1_006C
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_1_0025
loc_1_006C:
        mov    byte ptr [0x226], 0           ; -> loc_1_0226
        mov    word ptr [ebp + 0x1c], 1
        jmp    loc_1_002A
loc_1_007B:
        mov    byte ptr [0x226], 2           ; -> loc_1_0226
        mov    word ptr [ebp + 0x1c], 1
        jmp    loc_1_002A
loc_1_008A:
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [0x224], ax          ; -> obj1:0x0224
        mov    word ptr [ebp + 0x1c], 1
        jmp    loc_1_002A
loc_1_009C:
        mov    edi, ebx
        xor    eax, eax
        mov    ah, 0x62
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    word ptr [0x232], bx          ; -> obj1:0x0232
        xor    eax, eax
        mov    ah, 0x50
        movzx  ebx, word ptr [ebp + 0x38]
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ebx, edi
        jb     loc_1_0111
        movzx  edx, word ptr [ebp + 0x14]
        movzx  edi, word ptr [ebp + 0x3c]
        or     edi, edi
        je     loc_1_00E9
        push   0
        push   edi
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0111
        movzx  edx, word ptr [ebp + 0x14]
        add    edx, eax
loc_1_00E9:
        movzx  eax, word ptr [ebp + 0x1c]
        movzx  ebx, word ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_0111
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x14], dx
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x10], bx
        jmp    loc_1_011D
loc_1_0111:
        mov    word ptr [ebp + 0x1c], 0xffff
        mov    word ptr [ebp + 0x14], 0xffff
loc_1_011D:
        xor    eax, eax
        mov    ah, 0x50
        movzx  ebx, word ptr [0x232]         ; -> obj1:0x0232
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_0137
        jmp    loc_1_002A
loc_1_0137:
        jmp    loc_1_0025
loc_1_013C:
        cmp    byte ptr [0x226], 0           ; -> loc_1_0226
        je     loc_1_0150
        cmp    byte ptr [0x226], 2           ; -> loc_1_0226
        je     loc_1_0195
        jmp    loc_1_018E
loc_1_0150:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    cx, word ptr [0x220]          ; -> obj1:0x0220
        movzx  edx, word ptr [0x222]         ; -> obj1:0x0222
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        movzx  eax, word ptr [0x224]         ; -> obj1:0x0224
        mov    esi, 0x196                    ; -> loc_1_0196
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_1_018E:
        mov    byte ptr [0x226], 0           ; -> loc_1_0226
loc_1_0195:
        ret    
loc_1_0196:
        cmp    byte ptr [0x226], 0           ; -> loc_1_0226
        je     loc_1_01AA
        cmp    byte ptr [0x226], 2           ; -> loc_1_0226
        je     loc_1_01CE
        jmp    loc_1_01CE
loc_1_01AA:
        xor    eax, eax
        mov    ecx, dword ptr [ebx]
        test   byte ptr [ecx], 1
        jne    loc_1_01B8
        mov    eax, 0x1000
loc_1_01B8:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, 0xb
        mov    esi, 0x13c                    ; -> loc_1_013C
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_01CE:
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0003Ah                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VPMTD   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VPMTD_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VPMTD_PM_API             ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0226:
        add    ah, byte ptr [ebx + 0x3a]
        pop    esp
        inc    esi
        dec    edi
        dec    edi
        push   esp
        pop    eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], ch            ; -> loc_1_002F
        db  000h, 000h, 000h
        dd  loc_1_007B               ; fixup
        dd  loc_1_008A               ; fixup
        dd  loc_1_009C               ; fixup
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x4  R-X
loc_2_0000:
        clc    
        ret    
        db  022h, 056h
VxD__LTEXT ENDS
