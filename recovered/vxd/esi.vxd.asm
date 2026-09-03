; ESI -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 281 instructions recovered by recursive descent

Declare_Virtual_Device ESI, 1, 0, ESI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3d4  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ESI     '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  ESI_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        inc    ebp
        push   ebx
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0060
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0110
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_1_013C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0078:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ESI_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_008F
        jmp    dword ptr [eax*4]             ; -> obj1:0x0091
loc_1_008F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_3_0000
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_1_008F
        dd  loc_3_0000
sub_1_00FD:
        VxDCall  Dev_04A2.Service_8023h          ; cd202380a204
sub_1_0103:
        VxDCall  Dev_04A2.Service_8024h          ; cd202480a204
sub_1_0109:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        ret    
loc_1_0110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_013C
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x26
        mov    eax, 1
        jmp    loc_1_0137
loc_1_0137:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_013C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0195
loc_1_0150:
        mov    dword ptr [ebp - 4], 3
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_01C1
loc_1_0163:
        mov    dword ptr [ebp - 4], 5
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_01C1
loc_1_0176:
        mov    dword ptr [ebp - 4], 3
        mov    dword ptr [ebp - 8], 5
        jmp    loc_1_01C1
loc_1_0189:
        xor    eax, eax
        jmp    loc_1_0231
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_0195:
        cmp    dword ptr [ebp - 0xc], 0x2580
        je     loc_1_0150
        cmp    dword ptr [ebp - 0xc], 0x4b00
        je     loc_1_0163
        cmp    dword ptr [ebp - 0xc], 0x1c200
        je     loc_1_0176
        jmp    loc_1_0189
loc_1_01C1:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0000
loc_1_01D2:
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01E2
        jmp    loc_1_022A
loc_1_01E2:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0000
loc_1_01F3:
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0203
        jmp    loc_1_022A
loc_1_0203:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0240
        add    esp, 8
        test   eax, eax
        jne    loc_1_0220
        jmp    loc_1_022A
loc_1_0220:
        mov    eax, 1
        jmp    loc_1_0231
loc_1_022A:
        xor    eax, eax
        jmp    loc_1_0231
loc_1_0231:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0240:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03B4
        add    esp, 8
        test   eax, eax
        jne    loc_1_0268
        xor    eax, eax
        jmp    loc_1_02AE
loc_1_0268:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x2c], eax
        mov    byte ptr [ebp - 0x1a], 8
        mov    byte ptr [ebp - 0x19], 0
        mov    byte ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x34], 0x55
        mov    eax, dword ptr [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_02A4
        xor    eax, eax
        jmp    loc_1_02AE
loc_1_02A4:
        mov    eax, 1
        jmp    loc_1_02AE
loc_1_02AE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_0109
        mov    dword ptr [ebp - 4], eax
loc_1_02C4:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03C4
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0311
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03BC
        add    esp, 0xc
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jb     loc_1_030C
        jmp    loc_1_0320
loc_1_030C:
        jmp    loc_1_031B
loc_1_0311:
        mov    eax, 1
        jmp    loc_1_0327
loc_1_031B:
        jmp    loc_1_02C4
loc_1_0320:
        xor    eax, eax
        jmp    loc_1_0327
loc_1_0327:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x0000
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   0                             ; -> obj1:0x0000
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CC
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0366
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_036D
loc_1_0366:
        xor    eax, eax
        jmp    loc_1_0377
loc_1_036D:
        mov    eax, 1
        jmp    loc_1_0377
loc_1_0377:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_037C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_0078
        push   eax
        push   0                             ; -> obj1:0x0068
        call   sub_1_00FD
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_0103
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_03AC:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_03B4:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_03BC:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_03C4:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_03CC:
        VxDCall  BIOS.Service_8012h              ; cd2012802b00
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x6  R-X
sub_2_0000:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x28  R-X
loc_3_0000:
        mov    eax, dword ptr [0]            ; -> obj1:0x0008
        cmp    eax, 0
        jne    loc_3_0024
        inc    dword ptr [0]                 ; -> obj1:0x0008
        VxDCall  Dev_04A2.Service_0000h          ; cd200000a204
        jb     loc_3_0026
        push   0
        push   0
        call   sub_1_037C
loc_3_0021:
        add    esp, 8
loc_3_0024:
        clc    
        ret    
loc_3_0026:
        stc    
        ret    
VxD__LTEXT ENDS
