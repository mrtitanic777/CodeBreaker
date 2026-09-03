; ADAPTEC -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 671 instructions recovered by recursive descent

Declare_Virtual_Device ADAPTEC, 1, 0, ADAPTEC_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x894  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ADAPTEC '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  ADAPTEC_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        inc    ecx
        inc    esp
        inc    ecx
        push   eax
        push   esp
        inc    ebp
        inc    ebx
        add    byte ptr [eax], al            ; -> loc_1_0060
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_013D
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_027A
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0369
        add    byte ptr [eax], al
        db  000h
loc_1_0078:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_007C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ADAPTEC_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_008F
        jmp    dword ptr [eax*4]             ; -> obj1:0x0091
loc_1_008F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
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
        dd  loc_2_0000
sub_1_00FD:
        VxDCall  Dev_04A2.Service_8023h          ; cd202380a204
sub_1_0103:
        VxDCall  Dev_04A2.Service_8024h          ; cd202480a204
sub_1_0109:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        ret    
sub_1_0110:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0109
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0122:
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_1_0138
        jmp    loc_1_0122
loc_1_0138:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_013D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        test   eax, eax
        jne    loc_1_0168
        xor    eax, eax
        jmp    loc_1_0275
loc_1_0168:
        push   0x7d0
        call   sub_1_0110
        add    esp, 4
        push   0x30
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    loc_1_0190
        jmp    loc_1_026D
loc_1_0190:
        push   0x31
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    loc_1_01AB
        jmp    loc_1_026D
loc_1_01AB:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    loc_1_01C6
        jmp    loc_1_026D
loc_1_01C6:
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    loc_1_01E1
        jmp    loc_1_026D
loc_1_01E1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04D3
        add    esp, 4
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        test   eax, eax
        jne    loc_1_0208
        jmp    loc_1_026D
loc_1_0208:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        je     loc_1_022E
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 4
        je     loc_1_0233
loc_1_022E:
        jmp    loc_1_026D
loc_1_0233:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        test   eax, eax
        jne    loc_1_024C
        jmp    loc_1_026D
loc_1_024C:
        mov    dword ptr [0], 0x2580         ; -> loc_1_007C
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x3e
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 4], 7
        mov    dword ptr [ebp - 0xc], 1
loc_1_026D:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_0275
loc_1_0275:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_027A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_065C
        add    esp, 4
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0xff
        jne    loc_1_02AE
        jmp    loc_1_035C
loc_1_02AE:
        cmp    dword ptr [0], 0x2580         ; -> loc_1_007C
        je     loc_1_02DC
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0700
        add    esp, 8
        test   eax, eax
        jne    loc_1_02DC
        jmp    loc_1_035C
loc_1_02DC:
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    loc_1_02F9
        jmp    loc_1_035C
loc_1_02F9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     eax, 1
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    loc_1_031B
        jmp    loc_1_035C
loc_1_031B:
        cmp    dword ptr [ebp + 0xc], 0x2580
        je     loc_1_0345
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0700
        add    esp, 8
        test   eax, eax
        jne    loc_1_0345
        jmp    loc_1_035C
loc_1_0345:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_007C
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0364
loc_1_035C:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0364
loc_1_0364:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0369:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0], 0              ; -> loc_1_007C
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0398
        jmp    loc_1_03C1
loc_1_0398:
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_03B7
        jmp    loc_1_03C1
loc_1_03B7:
        mov    eax, 1
        jmp    loc_1_03C8
loc_1_03C1:
        xor    eax, eax
        jmp    loc_1_03C8
loc_1_03C8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_03CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04D3
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_03F1
loc_1_03EE:
        inc    dword ptr [ebp - 4]
loc_1_03F1:
        cmp    dword ptr [ebp - 4], 5
        jge    loc_1_04BB
        mov    al, byte ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        test   eax, eax
        jne    loc_1_0418
        jmp    loc_1_04BB
loc_1_0418:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        jne    loc_1_0435
        jmp    loc_1_04BB
loc_1_0435:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        je     loc_1_044C
        jmp    loc_1_03EE
loc_1_044C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        shl    eax, 4
        or     eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        test   eax, eax
        jne    loc_1_0471
        jmp    loc_1_04BB
loc_1_0471:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        jne    loc_1_048E
        jmp    loc_1_04BB
loc_1_048E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_04B6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_04CE
loc_1_04B6:
        jmp    loc_1_03EE
loc_1_04BB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_04CE
loc_1_04CE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_04D3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_059E
        add    esp, 4
        test   eax, eax
        jne    loc_1_04F7
        xor    eax, eax
        jmp    loc_1_0599
loc_1_04F7:
        call   sub_1_0109
        mov    dword ptr [ebp - 4], eax
loc_1_04FF:
        push   0xff
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        je     loc_1_0575
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0xc3
        jne    loc_1_0547
        mov    eax, 1
        jmp    loc_1_0599
        db  0E9h, 02Eh, 000h, 000h, 000h
loc_1_0547:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_0556
loc_1_0553:
        inc    dword ptr [ebp - 0xc]
loc_1_0556:
        cmp    dword ptr [ebp - 0xc], 3
        jae    loc_1_0575
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        jmp    loc_1_0553
loc_1_0575:
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xdc
        jb     loc_1_058D
        jmp    loc_1_0592
loc_1_058D:
        jmp    loc_1_04FF
loc_1_0592:
        xor    eax, eax
        jmp    loc_1_0599
loc_1_0599:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_059E:
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
        call   sub_1_06F8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_05C3
        jmp    loc_1_05EC
loc_1_05C3:
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_05E2
        jmp    loc_1_05EC
loc_1_05E2:
        mov    eax, 1
        jmp    loc_1_05F3
loc_1_05EC:
        xor    eax, eax
        jmp    loc_1_05F3
loc_1_05F3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_05F8:
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
        call   sub_1_06F8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_061D
        jmp    loc_1_0650
loc_1_061D:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_063C
        jmp    loc_1_0650
loc_1_063C:
        push   0x14
        call   sub_1_0110
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_0657
loc_1_0650:
        xor    eax, eax
        jmp    loc_1_0657
loc_1_0657:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_065C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0xff
        cmp    dword ptr [ebp + 8], 0x960
        jne    loc_1_067F
        mov    byte ptr [ebp - 4], 0x10
        jmp    loc_1_06E8
loc_1_067F:
        cmp    dword ptr [ebp + 8], 0x2580
        jne    loc_1_0695
        mov    byte ptr [ebp - 4], 0x30
        jmp    loc_1_06E8
loc_1_0695:
        cmp    dword ptr [ebp + 8], 0x4b00
        jne    loc_1_06AB
        mov    byte ptr [ebp - 4], 0x40
        jmp    loc_1_06E8
loc_1_06AB:
        cmp    dword ptr [ebp + 8], 0x9600
        jne    loc_1_06C1
        mov    byte ptr [ebp - 4], 0x50
        jmp    loc_1_06E8
loc_1_06C1:
        cmp    dword ptr [ebp + 8], 0xe100
        jne    loc_1_06D7
        mov    byte ptr [ebp - 4], 0x60
        jmp    loc_1_06E8
loc_1_06D7:
        cmp    dword ptr [ebp + 8], 0x1c200
        jne    loc_1_06E8
        mov    byte ptr [ebp - 4], 0x70
loc_1_06E8:
        mov    al, byte ptr [ebp - 4]
        jmp    loc_1_06F0
loc_1_06F0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_06F8:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        int3   
        int3   
sub_1_0700:
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
        call   sub_1_0874
        add    esp, 8
        test   eax, eax
        jne    loc_1_0728
        xor    eax, eax
        jmp    loc_1_076E
loc_1_0728:
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
        call   sub_1_086C
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0764
        xor    eax, eax
        jmp    loc_1_076E
loc_1_0764:
        mov    eax, 1
        jmp    loc_1_076E
loc_1_076E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0773:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_0109
        mov    dword ptr [ebp - 4], eax
loc_1_0784:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0884
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_07D1
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_087C
        add    esp, 0xc
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jb     loc_1_07CC
        jmp    loc_1_07E0
loc_1_07CC:
        jmp    loc_1_07DB
loc_1_07D1:
        mov    eax, 1
        jmp    loc_1_07E7
loc_1_07DB:
        jmp    loc_1_0784
loc_1_07E0:
        xor    eax, eax
        jmp    loc_1_07E7
loc_1_07E7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_07EC:
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
        call   sub_1_088C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0826
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_082D
loc_1_0826:
        xor    eax, eax
        jmp    loc_1_0837
loc_1_082D:
        mov    eax, 1
        jmp    loc_1_0837
loc_1_0837:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_083C:
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
sub_1_086C:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_0874:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_087C:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_0884:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_088C:
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
        call   sub_1_083C
loc_2_0021:
        add    esp, 8
loc_2_0024:
        clc    
        ret    
loc_2_0026:
        stc    
        ret    
VxD__LTEXT ENDS
