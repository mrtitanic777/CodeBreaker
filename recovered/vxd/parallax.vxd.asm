; PARALLAX -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 392 instructions recovered by recursive descent

Declare_Virtual_Device PARALLAX, 1, 0, PARALLAX_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x524  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PARALLAX'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  PARALLAX_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        push   eax
        inc    ecx
        push   edx
        inc    ecx
        dec    esp
        dec    esp
        inc    ecx
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0070:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0080:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
PARALLAX_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_009F
        jmp    dword ptr [eax*4]             ; -> obj1:0x00A1
loc_1_009F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_1_009F
        dd  loc_2_0000
sub_1_010D:
        VxDCall  Dev_04A2.Service_8023h          ; cd202380a204
sub_1_0113:
        VxDCall  Dev_04A2.Service_8024h          ; cd202480a204
sub_1_0119:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        ret    
loc_1_0120:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0145
        jmp    loc_1_019C
loc_1_0145:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0164
        jmp    loc_1_019C
loc_1_0164:
        push   1
        call   sub_1_0353
        add    esp, 4
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_01A8
        add    esp, 8
        test   eax, eax
        jne    loc_1_018C
        jmp    loc_1_019C
loc_1_018C:
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x3f
        mov    eax, 1
        jmp    loc_1_01A3
loc_1_019C:
        xor    eax, eax
        jmp    loc_1_01A3
loc_1_01A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_01A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01D0
        jmp    loc_1_0347
loc_1_01D0:
        push   1
        call   sub_1_0353
        add    esp, 4
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01F9
        jmp    loc_1_0347
loc_1_01F9:
        push   1
        call   sub_1_0353
        add    esp, 4
        cmp    dword ptr [ebp + 0xc], 0x1c200
        jne    loc_1_021C
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_02B7
loc_1_021C:
        cmp    dword ptr [ebp + 0xc], 0xe100
        jne    loc_1_0235
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_02B7
loc_1_0235:
        cmp    dword ptr [ebp + 0xc], 0x9600
        jne    loc_1_024E
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_02B7
loc_1_024E:
        cmp    dword ptr [ebp + 0xc], 0x4b00
        jne    loc_1_0267
        mov    dword ptr [ebp - 4], 3
        jmp    loc_1_02B7
loc_1_0267:
        cmp    dword ptr [ebp + 0xc], 0x2580
        jne    loc_1_0280
        mov    dword ptr [ebp - 4], 4
        jmp    loc_1_02B7
loc_1_0280:
        cmp    dword ptr [ebp + 0xc], 0x12c0
        jne    loc_1_0299
        mov    dword ptr [ebp - 4], 5
        jmp    loc_1_02B7
loc_1_0299:
        cmp    dword ptr [ebp + 0xc], 0x960
        jne    loc_1_02B2
        mov    dword ptr [ebp - 4], 6
        jmp    loc_1_02B7
loc_1_02B2:
        jmp    loc_1_0347
loc_1_02B7:
        jmp    loc_1_02BF
loc_1_02BC:
        dec    dword ptr [ebp - 4]
loc_1_02BF:
        cmp    dword ptr [ebp - 4], 0
        jle    loc_1_0320
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_02E8
        jmp    loc_1_0347
loc_1_02E8:
        push   1
        call   sub_1_0353
        add    esp, 4
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0311
        jmp    loc_1_0347
loc_1_0311:
        push   1
        call   sub_1_0353
        add    esp, 4
        jmp    loc_1_02BC
loc_1_0320:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0390
        add    esp, 8
        test   eax, eax
        jne    loc_1_033D
        jmp    loc_1_0347
loc_1_033D:
        mov    eax, 1
        jmp    loc_1_034E
loc_1_0347:
        xor    eax, eax
        jmp    loc_1_034E
loc_1_034E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0353:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0119
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0365:
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_1_037B
        jmp    loc_1_0365
loc_1_037B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0380:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
sub_1_0390:
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
        call   sub_1_0504
        add    esp, 8
        test   eax, eax
        jne    loc_1_03B8
        xor    eax, eax
        jmp    loc_1_03FE
loc_1_03B8:
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
        call   sub_1_04FC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_03F4
        xor    eax, eax
        jmp    loc_1_03FE
loc_1_03F4:
        mov    eax, 1
        jmp    loc_1_03FE
loc_1_03FE:
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
        call   sub_1_0119
        mov    dword ptr [ebp - 4], eax
loc_1_0414:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0514
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0461
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_050C
        add    esp, 0xc
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jb     loc_1_045C
        jmp    loc_1_0470
loc_1_045C:
        jmp    loc_1_046B
loc_1_0461:
        mov    eax, 1
        jmp    loc_1_0477
loc_1_046B:
        jmp    loc_1_0414
loc_1_0470:
        xor    eax, eax
        jmp    loc_1_0477
loc_1_0477:
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
        call   sub_1_051C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_04B6
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_04BD
loc_1_04B6:
        xor    eax, eax
        jmp    loc_1_04C7
loc_1_04BD:
        mov    eax, 1
        jmp    loc_1_04C7
loc_1_04C7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_04CC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_0080
        push   eax
        push   0                             ; -> loc_1_0070
        call   sub_1_010D
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
        call   sub_1_0113
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_04FC:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_0504:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_050C:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_0514:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_051C:
        VxDCall  BIOS.Service_8012h              ; cd2012802b00
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x28  R-X
loc_2_0000:
        mov    eax, dword ptr [0]            ; -> obj1:0x0008
        cmp    eax, 0
        jne    loc_2_0024
        inc    dword ptr [0]                 ; -> obj1:0x0008
        VxDCall  Dev_04A2.Service_0000h          ; cd200000a204
        jb     loc_2_0026
        push   0
        push   0
        call   sub_1_04CC
loc_2_0021:
        add    esp, 8
loc_2_0024:
        clc    
        ret    
loc_2_0026:
        stc    
        ret    
VxD__LTEXT ENDS
