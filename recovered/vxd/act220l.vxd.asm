; ACT220L -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 463 instructions recovered by recursive descent

Declare_Virtual_Device ACT220L, 1, 0, ACT220L_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5c4  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ACT220L '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  ACT220L_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        inc    ecx
        inc    ebx
        push   esp
        sub    eax, 0x32325249
        xor    byte ptr [eax + eax], cl
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
ACT220L_Control:
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
sub_1_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0119
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0132:
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_1_0148
        jmp    loc_1_0132
loc_1_0148:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_014D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0178
        jmp    loc_1_01B5
loc_1_0178:
        push   0x32
        call   sub_1_0120
        add    esp, 4
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01A1
        jmp    loc_1_01B5
loc_1_01A1:
        push   0x32
        call   sub_1_0120
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_01BC
loc_1_01B5:
        xor    eax, eax
        jmp    loc_1_01BC
loc_1_01BC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_01C1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0330
        add    esp, 4
        test   eax, eax
        jne    loc_1_01E0
        jmp    loc_1_01F0
loc_1_01E0:
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x36
        mov    eax, 1
        jmp    loc_1_01F7
loc_1_01F0:
        xor    eax, eax
        jmp    loc_1_01F7
loc_1_01F7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_01FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_024C
loc_1_0210:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0290
loc_1_021C:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_0290
loc_1_0228:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0290
loc_1_0234:
        mov    dword ptr [ebp - 4], 3
        jmp    loc_1_0290
loc_1_0240:
        xor    eax, eax
        jmp    loc_1_02F2
        db  0E9h, 044h, 000h, 000h, 000h
loc_1_024C:
        cmp    dword ptr [ebp - 0xc], 0x4b00
        ja     loc_1_0271
        je     loc_1_021C
        cmp    dword ptr [ebp - 0xc], 0x2580
        je     loc_1_0210
        jmp    loc_1_0240
loc_1_0271:
        cmp    dword ptr [ebp - 0xc], 0xe100
        je     loc_1_0228
        cmp    dword ptr [ebp - 0xc], 0x1c200
        je     loc_1_0234
        jmp    loc_1_0240
loc_1_0290:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_1_014D
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_02A7
loc_1_02A4:
        inc    dword ptr [ebp - 8]
loc_1_02A7:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_02C4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03B0
        add    esp, 4
        jmp    loc_1_02A4
loc_1_02C4:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0430
        add    esp, 8
        test   eax, eax
        jne    loc_1_02E1
        jmp    loc_1_02EB
loc_1_02E1:
        mov    eax, 1
        jmp    loc_1_02F2
loc_1_02EB:
        xor    eax, eax
        jmp    loc_1_02F2
loc_1_02F2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_02F7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        mov    eax, 1
        jmp    loc_1_032B
loc_1_032B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0330:
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
        call   sub_1_0420
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0355
        jmp    loc_1_0398
loc_1_0355:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0374
        jmp    loc_1_0398
loc_1_0374:
        push   0x32
        call   sub_1_0120
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        call   sub_1_014D
        test   eax, eax
        je     loc_1_0398
        mov    eax, 1
        jmp    loc_1_039F
loc_1_0398:
        xor    eax, eax
        jmp    loc_1_039F
loc_1_039F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_03B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_03D5
        jmp    loc_1_0412
loc_1_03D5:
        push   0x32
        call   sub_1_0120
        add    esp, 4
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_03FE
        jmp    loc_1_0412
loc_1_03FE:
        push   0x32
        call   sub_1_0120
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_0419
loc_1_0412:
        xor    eax, eax
        jmp    loc_1_0419
loc_1_0419:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0420:
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
sub_1_0430:
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
        call   sub_1_05A4
        add    esp, 8
        test   eax, eax
        jne    loc_1_0458
        xor    eax, eax
        jmp    loc_1_049E
loc_1_0458:
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
        call   sub_1_059C
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0494
        xor    eax, eax
        jmp    loc_1_049E
loc_1_0494:
        mov    eax, 1
        jmp    loc_1_049E
loc_1_049E:
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
loc_1_04B4:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05B4
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0501
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05AC
        add    esp, 0xc
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jb     loc_1_04FC
        jmp    loc_1_0510
loc_1_04FC:
        jmp    loc_1_050B
loc_1_0501:
        mov    eax, 1
        jmp    loc_1_0517
loc_1_050B:
        jmp    loc_1_04B4
loc_1_0510:
        xor    eax, eax
        jmp    loc_1_0517
loc_1_0517:
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
        call   sub_1_05BC
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0556
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_055D
loc_1_0556:
        xor    eax, eax
        jmp    loc_1_0567
loc_1_055D:
        mov    eax, 1
        jmp    loc_1_0567
loc_1_0567:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_056C:
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
sub_1_059C:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_05A4:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_05AC:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_05B4:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_05BC:
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
        call   sub_1_056C
loc_2_0021:
        add    esp, 8
loc_2_0024:
        clc    
        ret    
loc_2_0026:
        stc    
        ret    
VxD__LTEXT ENDS
