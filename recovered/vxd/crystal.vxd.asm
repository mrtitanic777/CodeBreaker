; CRYSTAL -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 677 instructions recovered by recursive descent

Declare_Virtual_Device CRYSTAL, 1, 0, CRYSTAL_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x884  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CRYSTAL '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  CRYSTAL_Control          ; DDB_Control_Proc
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
loc_1_006C:
        rol    dword ptr [eax], 1
        add    byte ptr [eax], al
loc_1_0070:
        rol    byte ptr [eax], 1
        add    byte ptr [eax], al
loc_1_0074:
        adc    byte ptr [edi - 0x78efee6b], cl
        xchg   ecx, eax
        adc    dword ptr [eax], edx
        mov    edx, dword ptr [eax - 0x6f7aefef]
        adc    dword ptr [eax], edx
        adc    dword ptr [eax - 0x6f7eefef], 0x11
loc_1_008C:
        inc    ebx
        push   edx
        pop    ecx
        push   ebx
        push   esp
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_008C
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0450
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_04B5
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0641
        add    byte ptr [eax], al
        db  000h
loc_1_00A8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
CRYSTAL_Control:
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
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0152:
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_1_0168
        jmp    loc_1_0152
loc_1_0168:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_016D:
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
        call   sub_1_06E8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_0192
        jmp    loc_1_01EE
loc_1_0192:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01B1
        jmp    loc_1_01EE
loc_1_01B1:
        push   0x32
        call   sub_1_0140
        add    esp, 4
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_01DA
        jmp    loc_1_01EE
loc_1_01DA:
        push   0x32
        call   sub_1_0140
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_01F5
loc_1_01EE:
        xor    eax, eax
        jmp    loc_1_01F5
loc_1_01F5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_01FA:
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
        call   sub_1_06E8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_021F
        jmp    loc_1_0233
loc_1_021F:
        push   0x28
        call   sub_1_0140
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_023A
loc_1_0233:
        xor    eax, eax
        jmp    loc_1_023A
loc_1_023A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_023F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_0248:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0763
        add    esp, 8
        test   eax, eax
        je     loc_1_0265
        jmp    loc_1_0248
loc_1_0265:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0274
loc_1_0271:
        inc    dword ptr [ebp - 4]
loc_1_0274:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_02E5
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07DC
        add    esp, 8
        test   eax, eax
        jne    loc_1_02A3
        jmp    loc_1_02EF
loc_1_02A3:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0763
        add    esp, 8
        test   eax, eax
        jne    loc_1_02C0
        jmp    loc_1_02EF
loc_1_02C0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     loc_1_02E0
        jmp    loc_1_02EF
loc_1_02E0:
        jmp    loc_1_0271
loc_1_02E5:
        mov    eax, 1
        jmp    loc_1_02F6
loc_1_02EF:
        xor    eax, eax
        jmp    loc_1_02F6
loc_1_02F6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_02FB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0                             ; -> loc_1_0060
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0xc
        jmp    loc_1_0319
loc_1_0319:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_031E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0                             ; -> loc_1_006C
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0347
        jmp    loc_1_03BF
loc_1_0347:
        push   0xc0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07DC
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0763
        add    esp, 8
        test   eax, eax
        jne    loc_1_0375
        jmp    loc_1_03BF
loc_1_0375:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xf0
        cmp    al, 0xc0
        je     loc_1_038A
        jmp    loc_1_03BF
loc_1_038A:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xf
        mov    byte ptr [0], al              ; -> obj1:0x0001
        push   0                             ; -> loc_1_0070
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_03B5
        jmp    loc_1_03BF
loc_1_03B5:
        mov    eax, 1
        jmp    loc_1_03C6
loc_1_03BF:
        xor    eax, eax
        jmp    loc_1_03C6
loc_1_03C6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_03CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_03E3
loc_1_03E0:
        inc    dword ptr [ebp - 4]
loc_1_03E3:
        cmp    dword ptr [ebp - 4], 5
        jae    loc_1_043A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_016D
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_02FB
        add    esp, 4
        test   eax, eax
        je     loc_1_0426
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_031E
        add    esp, 4
        test   eax, eax
        je     loc_1_0426
        jmp    loc_1_043A
loc_1_0426:
        cmp    dword ptr [ebp - 4], 4
        jne    loc_1_0435
        jmp    loc_1_0444
loc_1_0435:
        jmp    loc_1_03E0
loc_1_043A:
        mov    eax, 1
        jmp    loc_1_044B
loc_1_0444:
        xor    eax, eax
        jmp    loc_1_044B
loc_1_044B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CB
        add    esp, 4
        test   eax, eax
        jne    loc_1_0472
        jmp    loc_1_04A9
loc_1_0472:
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> obj1:0x0001
        cmp    eax, 1
        jne    loc_1_048D
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x1f
        jmp    loc_1_0493
loc_1_048D:
        mov    eax, dword ptr [ebp + 0xc]
        and    dword ptr [eax], 0x3f
loc_1_0493:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06A0
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_04B0
loc_1_04A9:
        xor    eax, eax
        jmp    loc_1_04B0
loc_1_04B0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_04B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_051D
loc_1_04C9:
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_0074
        jmp    loc_1_0586
loc_1_04D5:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x0078
        jmp    loc_1_0586
loc_1_04E1:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x007C
        jmp    loc_1_0586
loc_1_04ED:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x0080
        jmp    loc_1_0586
loc_1_04F9:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x0084
        jmp    loc_1_0586
loc_1_0505:
        mov    dword ptr [ebp - 4], 0        ; -> obj1:0x0088
        jmp    loc_1_0586
loc_1_0511:
        xor    eax, eax
        jmp    loc_1_063C
        db  0E9h, 069h, 000h, 000h, 000h
loc_1_051D:
        cmp    dword ptr [ebp - 8], 0x2580
        ja     loc_1_0542
        je     loc_1_04D5
        cmp    dword ptr [ebp - 8], 0x960
        je     loc_1_04C9
        jmp    loc_1_0511
loc_1_0542:
        cmp    dword ptr [ebp - 8], 0x9600
        ja     loc_1_0567
        je     loc_1_04ED
        cmp    dword ptr [ebp - 8], 0x4b00
        je     loc_1_04E1
        jmp    loc_1_0511
loc_1_0567:
        cmp    dword ptr [ebp - 8], 0xe100
        je     loc_1_04F9
        cmp    dword ptr [ebp - 8], 0x1c200
        je     loc_1_0505
        jmp    loc_1_0511
loc_1_0586:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_01FA
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0602
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F0
        add    esp, 8
        test   eax, eax
        jne    loc_1_05CA
        jmp    loc_1_0635
loc_1_05CA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CB
        add    esp, 4
        test   eax, eax
        jne    loc_1_05E3
        jmp    loc_1_0635
loc_1_05E3:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0602
        jmp    loc_1_0635
loc_1_0602:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06A0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F0
        add    esp, 8
        test   eax, eax
        jne    loc_1_062B
        jmp    loc_1_0635
loc_1_062B:
        mov    eax, 1
        jmp    loc_1_063C
loc_1_0635:
        xor    eax, eax
        jmp    loc_1_063C
loc_1_063C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0641:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_016D
        add    esp, 4
        push   0x2580
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F0
        add    esp, 8
        push   0                             ; -> loc_1_0068
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06A0
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_068D
loc_1_068D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_06A0:
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
        call   sub_1_06E8
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_06C5
        jmp    loc_1_06D9
loc_1_06C5:
        push   0x32
        call   sub_1_0140
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_06E0
loc_1_06D9:
        xor    eax, eax
        jmp    loc_1_06E0
loc_1_06E0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_06E8:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        int3   
        int3   
sub_1_06F0:
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
        call   sub_1_0864
        add    esp, 8
        test   eax, eax
        jne    loc_1_0718
        xor    eax, eax
        jmp    loc_1_075E
loc_1_0718:
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
        call   sub_1_085C
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0754
        xor    eax, eax
        jmp    loc_1_075E
loc_1_0754:
        mov    eax, 1
        jmp    loc_1_075E
loc_1_075E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0763:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        mov    dword ptr [ebp - 4], eax
loc_1_0774:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0874
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_07C1
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 0xc
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jb     loc_1_07BC
        jmp    loc_1_07D0
loc_1_07BC:
        jmp    loc_1_07CB
loc_1_07C1:
        mov    eax, 1
        jmp    loc_1_07D7
loc_1_07CB:
        jmp    loc_1_0774
loc_1_07D0:
        xor    eax, eax
        jmp    loc_1_07D7
loc_1_07D7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_07DC:
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
        call   sub_1_087C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0816
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_081D
loc_1_0816:
        xor    eax, eax
        jmp    loc_1_0827
loc_1_081D:
        mov    eax, 1
        jmp    loc_1_0827
loc_1_0827:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_082C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_00A8
        push   eax
        push   0                             ; -> obj1:0x0098
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
sub_1_085C:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_0864:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_086C:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_0874:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_087C:
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
        call   sub_1_082C
loc_2_0021:
        add    esp, 8
loc_2_0024:
        clc    
        ret    
loc_2_0026:
        stc    
        ret    
VxD__LTEXT ENDS
