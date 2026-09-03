; ACT200L -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 718 instructions recovered by recursive descent

Declare_Virtual_Device ACT200L, 1, 0, ACT200L_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x914  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ACT200L '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  ACT200L_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        or     edx, dword ptr [ebx + 0x47]
        arpl   word ptr [ecx + edx*8 + 0x56], si
        db  0D0h
loc_1_0068:
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        db  08Fh, 095h, 011h, 000h
loc_1_0070:
        xchg   dword ptr [ecx - 0x6f74ffef], edx
        adc    dword ptr [eax], eax
loc_1_0078:
        test   dword ptr [eax - 0x6f7cffef], edx
        adc    dword ptr [eax], eax
loc_1_0080:
        adc    dword ptr [eax + 0x43410011], 0x52492d54
        xor    dh, byte ptr [eax]
        xor    byte ptr [eax + eax], cl
loc_1_0090:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00A0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ACT200L_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_00BF
        jmp    dword ptr [eax*4]             ; -> obj1:0x00C1
loc_1_00BF:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_1_00BF
        dd  loc_2_0000
sub_1_012D:
        VxDCall  Dev_04A2.Service_8023h          ; cd202380a204
sub_1_0133:
        VxDCall  Dev_04A2.Service_8024h          ; cd202480a204
sub_1_0139:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        ret    
sub_1_0140:
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
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0165
        jmp    loc_1_0179
loc_1_0165:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_0180
loc_1_0179:
        xor    eax, eax
        jmp    loc_1_0180
loc_1_0180:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0185:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01AD
        jmp    loc_1_0318
loc_1_01AD:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01CC
        jmp    loc_1_0318
loc_1_01CC:
        push   0
        push   0
        push   0xd
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01EB
        jmp    loc_1_0318
loc_1_01EB:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0214
        jmp    loc_1_0318
loc_1_0214:
        push   0
        push   0
        push   0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0233
        jmp    loc_1_0318
loc_1_0233:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        push   0
        push   0
        push   0xd
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_025C
        jmp    loc_1_0318
loc_1_025C:
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_027B
        jmp    loc_1_0318
loc_1_027B:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_02A4
        jmp    loc_1_0318
loc_1_02A4:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        push   0xf0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 8
        test   eax, eax
        jne    loc_1_02CC
        jmp    loc_1_0318
loc_1_02CC:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07F3
        add    esp, 8
        test   eax, eax
        jne    loc_1_02E9
        jmp    loc_1_0318
loc_1_02E9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0xf4
        je     loc_1_030E
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0xf5
        je     loc_1_030E
        jmp    loc_1_0318
loc_1_030E:
        mov    eax, 1
        jmp    loc_1_031F
loc_1_0318:
        xor    eax, eax
        jmp    loc_1_031F
loc_1_031F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0324:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_033C
loc_1_0339:
        inc    dword ptr [ebp - 4]
loc_1_033C:
        cmp    dword ptr [ebp - 4], 5
        jae    loc_1_039A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0185
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05D4
        add    esp, 4
        test   eax, eax
        je     loc_1_0386
        push   0                             ; -> loc_1_0070
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0xc
        test   eax, eax
        je     loc_1_0386
        jmp    loc_1_039A
loc_1_0386:
        cmp    dword ptr [ebp - 4], 4
        jne    loc_1_0395
        jmp    loc_1_03A4
loc_1_0395:
        jmp    loc_1_0339
loc_1_039A:
        mov    eax, 1
        jmp    loc_1_03AB
loc_1_03A4:
        xor    eax, eax
        jmp    loc_1_03AB
loc_1_03AB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_03B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0324
        add    esp, 4
        test   eax, eax
        jne    loc_1_03CF
        jmp    loc_1_03EB
loc_1_03CF:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x3f
        mov    eax, 1
        jmp    loc_1_03F2
loc_1_03EB:
        xor    eax, eax
        jmp    loc_1_03F2
loc_1_03F2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_03F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_045F
loc_1_040B:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x006C
        jmp    loc_1_04C8
loc_1_0417:
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_0070
        jmp    loc_1_04C8
loc_1_0423:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x0074
        jmp    loc_1_04C8
loc_1_042F:
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_0078
        jmp    loc_1_04C8
loc_1_043B:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x007C
        jmp    loc_1_04C8
loc_1_0447:
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_0080
        jmp    loc_1_04C8
loc_1_0453:
        xor    eax, eax
        jmp    loc_1_057E
        db  0E9h, 069h, 000h, 000h, 000h
loc_1_045F:
        cmp    dword ptr [ebp - 8], 0x2580
        ja     loc_1_0484
        je     loc_1_0417
        cmp    dword ptr [ebp - 8], 0x960
        je     loc_1_040B
        jmp    loc_1_0453
loc_1_0484:
        cmp    dword ptr [ebp - 8], 0x9600
        ja     loc_1_04A9
        je     loc_1_042F
        cmp    dword ptr [ebp - 8], 0x4b00
        je     loc_1_0423
        jmp    loc_1_0453
loc_1_04A9:
        cmp    dword ptr [ebp - 8], 0xe100
        je     loc_1_043B
        cmp    dword ptr [ebp - 8], 0x1c200
        je     loc_1_0447
        jmp    loc_1_0453
loc_1_04C8:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0140
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0544
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        add    esp, 8
        test   eax, eax
        jne    loc_1_050C
        jmp    loc_1_0577
loc_1_050C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0324
        add    esp, 4
        test   eax, eax
        jne    loc_1_0525
        jmp    loc_1_0577
loc_1_0525:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0544
        jmp    loc_1_0577
loc_1_0544:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        add    esp, 8
        test   eax, eax
        jne    loc_1_056D
        jmp    loc_1_0577
loc_1_056D:
        mov    eax, 1
        jmp    loc_1_057E
loc_1_0577:
        xor    eax, eax
        jmp    loc_1_057E
loc_1_057E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0583:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0185
        add    esp, 4
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        add    esp, 8
        push   0                             ; -> loc_1_0068
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0730
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_05CF
loc_1_05CF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_05D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0                             ; -> loc_1_0060
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0xc
        jmp    loc_1_05F2
loc_1_05F2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_05F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0609:
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_1_061F
        jmp    loc_1_0609
loc_1_061F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0624:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_062D:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07F3
        add    esp, 8
        test   eax, eax
        je     loc_1_064A
        jmp    loc_1_062D
loc_1_064A:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0659
loc_1_0656:
        inc    dword ptr [ebp - 4]
loc_1_0659:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_06CA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 8
        test   eax, eax
        jne    loc_1_0688
        jmp    loc_1_06D4
loc_1_0688:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07F3
        add    esp, 8
        test   eax, eax
        jne    loc_1_06A5
        jmp    loc_1_06D4
loc_1_06A5:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     loc_1_06C5
        jmp    loc_1_06D4
loc_1_06C5:
        jmp    loc_1_0656
loc_1_06CA:
        mov    eax, 1
        jmp    loc_1_06DB
loc_1_06D4:
        xor    eax, eax
        jmp    loc_1_06DB
loc_1_06DB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_06E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0705
        jmp    loc_1_0719
loc_1_0705:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_0720
loc_1_0719:
        xor    eax, eax
        jmp    loc_1_0720
loc_1_0720:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0730:
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
        call   sub_1_0778
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0755
        jmp    loc_1_0769
loc_1_0755:
        push   0x32
        call   sub_1_05F7
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_0770
loc_1_0769:
        xor    eax, eax
        jmp    loc_1_0770
loc_1_0770:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_0778:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        int3   
        int3   
sub_1_0780:
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
        call   sub_1_08F4
        add    esp, 8
        test   eax, eax
        jne    loc_1_07A8
        xor    eax, eax
        jmp    loc_1_07EE
loc_1_07A8:
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
        call   sub_1_08EC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_07E4
        xor    eax, eax
        jmp    loc_1_07EE
loc_1_07E4:
        mov    eax, 1
        jmp    loc_1_07EE
loc_1_07EE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_07F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        mov    dword ptr [ebp - 4], eax
loc_1_0804:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0904
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0851
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_08FC
        add    esp, 0xc
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jb     loc_1_084C
        jmp    loc_1_0860
loc_1_084C:
        jmp    loc_1_085B
loc_1_0851:
        mov    eax, 1
        jmp    loc_1_0867
loc_1_085B:
        jmp    loc_1_0804
loc_1_0860:
        xor    eax, eax
        jmp    loc_1_0867
loc_1_0867:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_086C:
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
        call   sub_1_090C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_08A6
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_08AD
loc_1_08A6:
        xor    eax, eax
        jmp    loc_1_08B7
loc_1_08AD:
        mov    eax, 1
        jmp    loc_1_08B7
loc_1_08B7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_08BC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_00A0
        push   eax
        push   0                             ; -> loc_1_0090
        call   sub_1_012D
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
        call   sub_1_0133
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_08EC:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_08F4:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_08FC:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_0904:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_090C:
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
        call   sub_1_08BC
loc_2_0021:
        add    esp, 8
loc_2_0024:
        clc    
        ret    
loc_2_0026:
        stc    
        ret    
VxD__LTEXT ENDS
