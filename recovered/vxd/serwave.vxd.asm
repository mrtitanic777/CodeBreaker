; SERWAVE -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 2365 instructions recovered by recursive descent

Declare_Virtual_Device SERWAVE, 4, 0, SERWAVE_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x17c0  R-X
        db  08Bh, 044h, 024h, 004h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 040h, 008h, 000h, 000h, 000h
        db  000h, 0C2h, 004h, 000h
loc_1_0014:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 8], 0
        push   2
        push   dword ptr [esp + 4]
        call   sub_1_0035
        add    esp, 4
        ret    
sub_1_0035:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi], 0xffffffff
        mov    dword ptr [esi + 4], eax
        push   dword ptr [esi + 8]
        call   sub_1_1744
        mov    dword ptr [esi + 8], 0
        push   0x56415753
        call   sub_1_175C
        add    esp, 4
        pop    esi
        ret    8
sub_1_0067:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_171C
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [eax + 0x2c]
        mov    dword ptr [ebp - 8], eax
        test   ah, 2
        jne    loc_1_0096
        int3   
        mov    ecx, dword ptr [ebp - 8]
        or     ecx, 0x200
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x2c], ecx
loc_1_0096:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    loc_1_00EA
        push   esi
        push   dword ptr [ebp + 0xc]
        push   0x14                          ; -> loc_1_0014
        call   sub_1_1728
        mov    dword ptr [esi + 8], eax
        cmp    dword ptr [esi + 8], 0
        jne    loc_1_00BC
        mov    eax, 2
        jmp    loc_1_00ED
loc_1_00BC:
        cmp    dword ptr [esi], 0
        jne    loc_1_00D5
loc_1_00C1:
        push   1
        push   0x56415753
        call   sub_1_1754
        add    esp, 8
        cmp    dword ptr [esi], 0
        je     loc_1_00C1
loc_1_00D5:
        cmp    dword ptr [esi + 8], 0
        je     loc_1_00EA
        push   dword ptr [esi + 8]
        call   sub_1_1744
        mov    dword ptr [esi + 8], 0
loc_1_00EA:
        mov    eax, dword ptr [esi + 4]
loc_1_00ED:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h, 000h
SERWAVE_Control:
        cmp    eax, 0x1b
        je     loc_1_0109
        cmp    eax, 0x1c
        je     loc_1_010B
        cmp    eax, 0x22
        je     loc_1_0134
        clc    
        ret    
loc_1_0109:
        clc    
        ret    
loc_1_010B:
        clc    
        ret    
SERWAVE_PM_API:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    eax, 0x11
        jg     loc_1_012C
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        call   sub_3_0000
        mov    dword ptr [ebp + 0x1c], eax
        ret    
loc_1_012C:
        mov    dword ptr [ebp + 0x1c], 0xffffffff
        ret    
loc_1_0134:
        cmp    edx, 0
        jne    loc_1_014C
        push   0
        push   0x3e7                         ; -> loc_2_03E7
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f003300
        add    esp, 0xc
        stc    
        ret    
loc_1_014C:
        mov    eax, ebx
        mov    ebx, 0x160                    ; -> loc_1_0160
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
loc_1_0160:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_2_0160
        add    esp, 0x14
        leave  
        clc    
        ret    
        db  000h, 000h, 000h
sub_1_0180:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        movzx  edx, ax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        shr    eax, 0x10
        push   edx
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        add    eax, edx
        pop    ebx
        leave  
        ret    4
sub_1_01A9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ecx
        mov    eax, 0x1000
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, 1
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0xffffffff
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    eax, esi
        pop    ecx
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_01DB:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        pop    esi
        leave  
        ret    4
sub_1_01ED:
        push   ebp
        mov    ebp, esp
        push   ecx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 8]
        and    dword ptr [eax + 0x2c], 0xfffffbff
        mov    ax, word ptr [ebp + 0xe]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0xc]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x10]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x16]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x14]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x1a]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    cx, word ptr [ebp + 0xa]
        movzx  edx, word ptr [ebp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ecx
        leave  
        ret    0x14
sub_1_028A:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ecx
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [ebp + 0xc]
        repne scasb al, byte ptr es:[edi]
        jne    loc_1_02A6
        dec    edi
        mov    eax, edi
        pop    ecx
        pop    edi
        leave  
        ret    0xc
loc_1_02A6:
        xor    eax, eax
        pop    ecx
        pop    edi
        leave  
        ret    0xc
        db  000h, 000h
loc_1_02B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   1
        push   dword ptr [esi + 0x10]
        call   sub_1_170C
        add    esp, 8
        mov    dword ptr [esi + 0xfc], eax
        or     eax, eax
        jne    loc_1_02D3
        jmp    loc_1_0421
loc_1_02D3:
        mov    dword ptr [esi + 0x80], 1
        mov    dword ptr [esi + 0x84], 0
        push   dword ptr [esp + 0x14]
        call   sub_1_0180
        mov    dword ptr [esi + 0x88], eax
        lea    edi, [esi + 0x78]
        push   0
        push   edi
        push   0xc0000002
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0411
        cmp    dword ptr [esi + 0x7c], 0
        jne    loc_1_0411
        test   byte ptr [esi + 0x1e], 8
        je     loc_1_0334
        push   0
        mov    ebx, dword ptr [esp + 0x1c]
        push   0
        push   ebx
        push   esi
        call   sub_2_00D6
        jmp    loc_1_0338
loc_1_0334:
        mov    ebx, dword ptr [esp + 0x18]
loc_1_0338:
        mov    dword ptr [esi + 0x80], 1
        mov    dword ptr [esi + 0x84], 0
        push   0
        push   edi
        push   0xc0000003
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_03EB
        cmp    dword ptr [esi + 0x7c], 0
        jne    loc_1_03EB
        test   byte ptr [esi + 0x1e], 8
        jne    loc_1_037D
        push   0
        push   0
        push   ebx
        push   esi
        call   sub_2_00D6
loc_1_037D:
        mov    byte ptr [esi + 0x104], 0
        mov    dword ptr [esi + 0x100], 0
        mov    dword ptr [esi + 0xf8], 1
        lea    eax, [esi + 0x13d]
        mov    dword ptr [esi + 0x12d], eax
        mov    dword ptr [esi + 0x119], 0x200
        push   esi
        push   0x4ca                         ; -> loc_1_04CA
        mov    eax, dword ptr [esi + 0x10]
        shr    eax, 3
        push   eax
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_03CB
        jmp    loc_1_0411
loc_1_03CB:
        push   esi
        push   0x7d0
        push   0x519                         ; -> loc_1_0519
        call   sub_1_1728
        mov    dword ptr [esi + 0x109], eax
        push   esi
        call   sub_1_0632
        sub    eax, eax
        jmp    loc_1_0426
loc_1_03EB:
        mov    dword ptr [esi + 0x80], 1
        mov    dword ptr [esi + 0x84], 0
        push   0
        push   edi
        push   0xc0000004
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
loc_1_0411:
        push   0
        push   dword ptr [esi + 0xfc]
        call   sub_1_1714
        add    esp, 8
loc_1_0421:
        mov    eax, 2
loc_1_0426:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
loc_1_042C:
        push   esi
        push   0
        push   0
        mov    esi, dword ptr [esp + 0x10]
        push   -1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 0x10
        push   dword ptr [esi + 0x109]
        call   sub_1_1744
        mov    dword ptr [esi + 0x109], 0
        push   dword ptr [esi + 0x14]
        call   sub_1_01DB
        test   byte ptr [esi + 0x1e], 0x10
        je     loc_1_0477
        push   0x5f
        lea    eax, [esi + 0xb4]
        push   eax
        push   dword ptr [esi]
        call   sub_1_16C4
        add    esp, 0xc
loc_1_0477:
        mov    dword ptr [esi + 0x80], 1
        mov    dword ptr [esi + 0x84], 0
        push   0
        lea    eax, [esi + 0x78]
        push   eax
        push   0xc0000004
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        push   0
        push   dword ptr [esi + 0xfc]
        call   sub_1_1714
        add    esp, 8
        mov    dword ptr [esi + 0xf8], 0
        mov    dword ptr [esi + 0xfc], 0
        sub    eax, eax
        pop    esi
        ret    0x10
loc_1_04CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_04EB
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0xc]
        popfd  
        jmp    loc_1_0514
loc_1_04EB:
        mov    dword ptr [esi + 0x18], 0xffffffff
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        popfd  
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_050D
        push   esi
        push   0x5ad                         ; -> loc_1_05AD
        call   sub_1_01A9
        mov    dword ptr [esi + 0x14], eax
loc_1_050D:
        mov    dword ptr [esi + 0x18], 0
loc_1_0514:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0519:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    dword ptr [ebp - 4], edx
        pushfd 
        pop    dword ptr [ebp - 8]
        cli    
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x18
        cmp    dword ptr [eax], 0
        je     loc_1_055C
        mov    eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x10]
        popfd  
        push   dword ptr [ebp - 4]
        push   0x7d0
        push   0x519                         ; -> loc_1_0519
        call   sub_1_1728
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x109], eax
        jmp    loc_1_05A8
loc_1_055C:
        mov    dword ptr [eax], 0xffffffff
        mov    dword ptr [ebp - 0xc], ecx
        push   dword ptr [ebp - 0xc]
        popfd  
        mov    esi, dword ptr [ebp - 4]
        add    esi, 0x14
        cmp    dword ptr [esi], 0
        jne    loc_1_0583
        push   dword ptr [ebp - 4]
        push   0x5ad                         ; -> loc_1_05AD
        call   sub_1_01A9
        mov    dword ptr [esi], eax
loc_1_0583:
        push   dword ptr [ebp - 4]
        push   0x7d0
        push   0x519                         ; -> loc_1_0519
        call   sub_1_1728
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x109], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0
loc_1_05A8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_05AD:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x14], 0
        push   dword ptr [esp]
        call   sub_1_0699
        push   dword ptr [esp]
        call   sub_1_0632
        add    esp, 4
        ret    
loc_1_05D5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_1_0180
        mov    edi, eax
        mov    dword ptr [edi + 0x10], esi
        push   dword ptr [edi]
        call   sub_1_0180
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x14], 0
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edi + 0xc], 0
        cmp    dword ptr [eax + 0xe4], 0
        jne    loc_1_0613
        mov    dword ptr [eax + 0xe4], edi
        jmp    loc_1_061C
loc_1_0613:
        mov    ecx, dword ptr [eax + 0xe8]
        mov    dword ptr [ecx + 0xc], edi
loc_1_061C:
        mov    dword ptr [eax + 0xe8], edi
        push   eax
        call   sub_1_0632
        mov    eax, 1
        pop    edi
        pop    esi
        ret    0x10
sub_1_0632:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0xf8], 0
        je     loc_1_0694
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_1_068E
        sub    edi, edi
loc_1_064C:
        mov    eax, dword ptr [esi + 0xe4]
        cmp    eax, edi
        je     loc_1_065F
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0xe4], ecx
loc_1_065F:
        mov    dword ptr [esi + 0xf4], eax
        cmp    eax, edi
        jne    loc_1_0680
        cmp    dword ptr [esi + 0x10d], edi
        je     loc_1_068E
        lea    eax, [esi + 0x115]
        mov    dword ptr [esi + 0xf4], eax
        mov    dword ptr [eax + 0x14], edi
loc_1_0680:
        push   esi
        call   sub_1_0699
        cmp    dword ptr [esi + 0xf4], edi
        je     loc_1_064C
loc_1_068E:
        push   esi
        call   sub_1_092A
loc_1_0694:
        pop    edi
        pop    esi
        ret    4
sub_1_0699:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [esi + 0xf4]
        or     ecx, ecx
        mov    dword ptr [esp + 0x10], ecx
        je     loc_1_0920
        cmp    dword ptr [esi + 0x139], 0
        jne    loc_1_0920
        mov    dword ptr [esi + 0x139], 1
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [ecx + 0x14], eax
        jae    loc_1_0879
loc_1_06D9:
        mov    ecx, dword ptr [esp + 0x10]
        movzx  eax, byte ptr [esi + 0x104]
        mov    edx, dword ptr [ecx + 0x14]
        mov    ebp, dword ptr [ecx + 0x18]
        add    ebp, edx
        or     eax, eax
        je     loc_1_06FE
        cmp    eax, 4
        je     loc_1_07EA
        jmp    loc_1_0869
loc_1_06FE:
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [ecx + 4]
        sub    ecx, edx
        cmp    ecx, eax
        jbe    loc_1_0710
        mov    ecx, eax
loc_1_0710:
        lea    eax, [esp + 0x14]
        push   eax
        push   ecx
        push   dword ptr [esi + 0xfc]
        push   dword ptr [esi]
        call   sub_1_097C
        add    esp, 0x10
        or     eax, eax
        je     loc_1_08A2
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_0916
        mov    edi, dword ptr [esi + 0xfc]
        mov    eax, dword ptr [esp + 0x14]
        add    eax, edi
        cmp    eax, edi
        jbe    loc_1_0869
loc_1_074D:
        mov    al, byte ptr [edi]
        cmp    al, 0x10
        jne    loc_1_07BE
        mov    eax, dword ptr [esi + 0xfc]
        sub    eax, edi
        add    eax, dword ptr [esp + 0x14]
        cmp    eax, 1
        je     loc_1_07E1
        mov    al, byte ptr [edi + 1]
        lea    ebx, [edi + 1]
        cmp    al, 0x10
        je     loc_1_07B6
        cmp    al, 3
        jne    loc_1_0787
        mov    dword ptr [esi + 0xf8], 0
        push   0
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_0035
loc_1_0787:
        mov    dword ptr [esp + 0x1c], 2
        mov    al, byte ptr [edi]
        mov    byte ptr [esp + 0x24], al
        mov    al, byte ptr [ebx]
        mov    byte ptr [esp + 0x25], al
        lea    eax, [esp + 0x18]
        push   0
        add    edi, 2
        push   eax
        push   0xc0000000
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        jmp    loc_1_07CA
loc_1_07B6:
        mov    byte ptr [ebp], al
        lea    edi, [ebx + 1]
        jmp    loc_1_07C2
loc_1_07BE:
        mov    byte ptr [ebp], al
        inc    edi
loc_1_07C2:
        mov    ecx, dword ptr [esp + 0x10]
        inc    ebp
        inc    dword ptr [ecx + 0x14]
loc_1_07CA:
        mov    eax, dword ptr [esi + 0xfc]
        add    eax, dword ptr [esp + 0x14]
        cmp    eax, edi
        ja     loc_1_074D
        jmp    loc_1_0869
loc_1_07E1:
        mov    byte ptr [esi + 0x104], 4
        jmp    loc_1_0869
loc_1_07EA:
        lea    eax, [esp + 0x14]
        push   eax
        push   1
        push   ebp
        push   dword ptr [esi]
        call   sub_1_097C
        add    esp, 0x10
        or     eax, eax
        je     loc_1_089B
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_0916
        mov    al, byte ptr [ebp]
        cmp    al, 0x10
        jne    loc_1_081F
        mov    ecx, dword ptr [esp + 0x10]
        inc    dword ptr [ecx + 0x14]
        jmp    loc_1_0862
loc_1_081F:
        cmp    al, 3
        jne    loc_1_0838
        mov    dword ptr [esi + 0xf8], 0
        push   0
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_0035
loc_1_0838:
        mov    dword ptr [esp + 0x2c], 2
        mov    byte ptr [esp + 0x34], 0x10
        mov    al, byte ptr [ebp]
        mov    byte ptr [esp + 0x35], al
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   0xc0000000
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
loc_1_0862:
        mov    byte ptr [esi + 0x104], 0
loc_1_0869:
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [ecx + 0x14], eax
        jb     loc_1_06D9
loc_1_0879:
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 0x14]
        cmp    dword ptr [ecx + 4], eax
        jne    loc_1_0916
        mov    ecx, dword ptr [esi + 0xf4]
        sub    ecx, esi
        cmp    ecx, 0x115
        jne    loc_1_08E1
        jmp    loc_1_090C
loc_1_089B:
        mov    byte ptr [esi + 0x104], 0
loc_1_08A2:
        mov    edx, dword ptr [esi + 0xf4]
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 0x115
        jne    loc_1_08B5
        jmp    loc_1_090C
loc_1_08B5:
        mov    ecx, dword ptr [esp + 0x10]
        lea    eax, [esi + 0xec]
        mov    dword ptr [ecx + 0x14], 0
        mov    dword ptr [edx + 0xc], 0
        cmp    dword ptr [eax], 0
        jne    loc_1_08D6
        mov    dword ptr [eax], edx
        jmp    loc_1_08DC
loc_1_08D6:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0xc], edx
loc_1_08DC:
        mov    dword ptr [eax + 4], edx
        jmp    loc_1_090C
loc_1_08E1:
        add    dword ptr [esi + 0x100], eax
        mov    eax, dword ptr [esi + 0xf4]
        lea    ecx, [esi + 0xec]
        mov    dword ptr [eax + 0xc], 0
        cmp    dword ptr [ecx], 0
        jne    loc_1_0903
        mov    dword ptr [ecx], eax
        jmp    loc_1_0909
loc_1_0903:
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0xc], eax
loc_1_0909:
        mov    dword ptr [ecx + 4], eax
loc_1_090C:
        mov    dword ptr [esi + 0xf4], 0
loc_1_0916:
        mov    dword ptr [esi + 0x139], 0
loc_1_0920:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    4
sub_1_092A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x135], 1
        je     loc_1_0978
        mov    dword ptr [esi + 0x135], 1
loc_1_0942:
        mov    eax, dword ptr [esi + 0xec]
        or     eax, eax
        je     loc_1_096E
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0xec], ecx
        or     eax, eax
        je     loc_1_096E
        push   dword ptr [eax + 0x14]
        push   dword ptr [eax + 0x10]
        push   2
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   sub_1_01ED
        jmp    loc_1_0942
loc_1_096E:
        mov    dword ptr [esi + 0x135], 0
loc_1_0978:
        pop    esi
        ret    4
sub_1_097C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [esp + 0x30]
loc_1_098F:
        push   ebx
        push   esi
        sub    ebp, ebp
        push   dword ptr [esp + 0x30]
        push   edi
        call   sub_1_16F4
        add    esp, 0x10
        cmp    eax, ebp
        je     loc_1_09CE
        cmp    dword ptr [ebx], ebp
        jne    loc_1_09CA
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   edi
        call   sub_1_16D4
        add    esp, 0xc
        cmp    eax, ebp
        je     loc_1_09CE
        cmp    dword ptr [esp + 0x18], ebp
        je     loc_1_09CA
        mov    ebp, 1
loc_1_09CA:
        or     ebp, ebp
        jne    loc_1_098F
loc_1_09CE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_09D6:
        push   esi
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_0A48
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esi + 0xf4]
        or     edx, edx
        je     loc_1_0B19
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 0x115
        jne    loc_1_0A14
        mov    dword ptr [esi + 0xf4], 0
        mov    dword ptr [esi + 0x10d], 0
        jmp    loc_1_0B19
loc_1_0A14:
        lea    eax, [esi + 0xec]
        mov    dword ptr [edx + 0xc], 0
        cmp    dword ptr [eax], 0
        jne    loc_1_0A2A
        mov    dword ptr [eax], edx
        jmp    loc_1_0A30
loc_1_0A2A:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0xc], edx
loc_1_0A30:
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [esi + 0xf4], 0
        push   esi
        call   sub_1_092A
        jmp    loc_1_0B19
loc_1_0A48:
        mov    esi, dword ptr [esp + 8]
        push   1
        push   dword ptr [esi]
        call   sub_1_16E4
        add    esp, 8
        mov    ecx, dword ptr [esi + 0xf4]
        or     ecx, ecx
        je     loc_1_0AB4
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x115
        jne    loc_1_0A83
        mov    dword ptr [esi + 0xf4], 0
        mov    dword ptr [esi + 0x10d], 0
        jmp    loc_1_0AB4
loc_1_0A83:
        mov    dword ptr [ecx + 0xc], 0
        cmp    dword ptr [esi + 0xec], 0
        jne    loc_1_0A9B
        mov    dword ptr [esi + 0xec], ecx
        jmp    loc_1_0AA4
loc_1_0A9B:
        mov    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0AA4:
        mov    dword ptr [esi + 0xf0], ecx
        mov    dword ptr [esi + 0xf4], 0
loc_1_0AB4:
        mov    edx, dword ptr [esi + 0xe4]
        or     edx, edx
        je     loc_1_0B09
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xe4], eax
        or     edx, edx
        je     loc_1_0B09
        sub    ecx, ecx
loc_1_0ACD:
        mov    dword ptr [edx + 0x14], ecx
        mov    dword ptr [edx + 0xc], ecx
        cmp    dword ptr [esi + 0xec], ecx
        jne    loc_1_0AE3
        mov    dword ptr [esi + 0xec], edx
        jmp    loc_1_0AEC
loc_1_0AE3:
        mov    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [eax + 0xc], edx
loc_1_0AEC:
        mov    dword ptr [esi + 0xf0], edx
        mov    edx, dword ptr [esi + 0xe4]
        cmp    edx, ecx
        je     loc_1_0B09
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xe4], eax
        cmp    edx, ecx
        jne    loc_1_0ACD
loc_1_0B09:
        push   esi
        call   sub_1_092A
        mov    dword ptr [esi + 0x100], 0
loc_1_0B19:
        sub    eax, eax
        pop    esi
        ret    0x10
loc_1_0B1F:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xf4]
        or     eax, eax
        je     loc_1_0B38
        mov    eax, dword ptr [eax + 0x14]
        add    eax, dword ptr [ecx + 0x100]
        jmp    loc_1_0B3E
loc_1_0B38:
        mov    eax, dword ptr [ecx + 0x100]
loc_1_0B3E:
        ret    0x10
loc_1_0B41:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        mov    dword ptr [esi + 0x10d], 1
        push   0x4ca                         ; -> loc_1_04CA
        push   1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 0x10
        lea    edi, [esi + 0x20]
        push   edi
        call   0
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi + 0x44]
        lea    eax, [esi + 0x30]
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 0x10
        mov    dword ptr [esi + 0x100], 0
        push   esi
        call   sub_1_0632
        push   0x2710
        push   edi
        call   sub_1_0067
        pop    edi
        pop    esi
        add    esp, 4
        ret    0x10
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0BB0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        cmp    dword ptr [edi + 0x110], 0
        je     loc_1_0BDF
        mov    dword ptr [edi + 0x110], 0
        push   dword ptr [edi + 0x12d]
        call   sub_1_1744
        mov    dword ptr [edi + 0x12d], 0
loc_1_0BDF:
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_0180
        mov    ebx, eax
        mov    dword ptr [ebx + 0x10], esi
        push   dword ptr [ebx]
        call   sub_1_0180
        mov    dword ptr [ebx + 0x18], eax
        mov    dword ptr [ebx + 0x14], 0
        mov    dword ptr [ebx + 0xc], 0
        cmp    dword ptr [edi + 0xe4], 0
        jne    loc_1_0C17
        mov    dword ptr [edi + 0xe4], ebx
        jmp    loc_1_0C20
loc_1_0C17:
        mov    eax, dword ptr [edi + 0xe8]
        mov    dword ptr [eax + 0xc], ebx
loc_1_0C20:
        mov    dword ptr [edi + 0xe8], ebx
        push   edi
        call   sub_1_0C37
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
sub_1_0C37:
        sub    esp, 0xc
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_0D1E
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_1_0CF3
        sub    ebx, ebx
loc_1_0C5C:
        mov    eax, dword ptr [esi + 0xe4]
        cmp    eax, ebx
        je     loc_1_0C6F
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0xe4], ecx
loc_1_0C6F:
        mov    dword ptr [esi + 0xf4], eax
        cmp    eax, ebx
        je     loc_1_0CAD
        mov    eax, dword ptr [eax + 0x1c]
        add    eax, dword ptr [esi + 0x125]
        mov    dword ptr [esi + 0x125], eax
        mov    ecx, dword ptr [esi + 0x121]
        add    ecx, eax
        mov    eax, dword ptr [esi + 0xf4]
        push   esi
        mov    dword ptr [eax + 0x1c], ecx
        mov    byte ptr [esi + 0x114], bl
        call   sub_1_0D26
        cmp    dword ptr [esi + 0xf4], ebx
        je     loc_1_0C5C
loc_1_0CAD:
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_1_0CF3
        cmp    dword ptr [esi + 0xec], 0
        je     loc_1_0CF3
        cmp    dword ptr [esi + 0x12d], 0
        jne    loc_1_0CF3
        push   esi
        push   0x32
        push   0x1010                        ; -> loc_1_1010
        call   sub_1_1728
        mov    dword ptr [esi + 0x12d], eax
        or     eax, eax
        je     loc_1_0CEB
        mov    dword ptr [esi + 0x110], 1
        jmp    loc_1_0CF3
loc_1_0CEB:
        push   1
        push   esi
        call   sub_1_0ED5
loc_1_0CF3:
        cmp    dword ptr [esi + 0x110], 0
        je     loc_1_0D16
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi]
        call   sub_1_16CC
        add    esp, 8
        or     eax, eax
        je     loc_1_0D16
        cmp    dword ptr [esp + 0x10], 0
        ja     loc_1_0D1E
loc_1_0D16:
        push   0
        push   esi
        call   sub_1_0ED5
loc_1_0D1E:
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    4
sub_1_0D26:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x18], 0
        mov    edi, dword ptr [esi + 0xf4]
        or     edi, edi
        je     loc_1_0ECB
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [edi + 0x18]
        mov    dword ptr [esp + 0x20], eax
loc_1_0D51:
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        mov    ebx, dword ptr [edi + 4]
        cmp    eax, ebx
        jb     loc_1_0D6C
        cmp    byte ptr [esi + 0x114], 0
        je     loc_1_0E69
loc_1_0D6C:
        mov    ebp, dword ptr [edi + 0x18]
        add    ebp, dword ptr [esp + 0x1c]
        movzx  eax, byte ptr [esi + 0x114]
        or     eax, eax
        je     loc_1_0D91
        cmp    eax, 2
        je     loc_1_0E29
        cmp    eax, 3
        je     loc_1_0DF7
        jmp    loc_1_0E5E
loc_1_0D91:
        mov    byte ptr [esi + 0x114], 2
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, ebp
        push   eax
        push   0x10
        push   ebp
        call   sub_1_028A
        or     eax, eax
        je     loc_1_0E29
        sub    eax, ebp
        mov    byte ptr [esi + 0x114], 1
        lea    ebx, [eax + 1]
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0E93
        mov    eax, dword ptr [esp + 0x14]
        add    dword ptr [edi + 0x14], eax
        cmp    dword ptr [esp + 0x14], ebx
        je     loc_1_0DEE
        mov    byte ptr [esi + 0x114], 0
        mov    dword ptr [esp + 0x18], 1
        jmp    loc_1_0E5E
loc_1_0DEE:
        mov    byte ptr [esi + 0x114], 3
        jmp    loc_1_0E5E
loc_1_0DF7:
        mov    byte ptr [esp + 0x13], 0x10
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x17]
        push   1
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0E93
        cmp    dword ptr [esp + 0x14], 1
        jne    loc_1_0E1F
        jmp    loc_1_0E57
loc_1_0E1F:
        mov    dword ptr [esp + 0x18], 1
        jmp    loc_1_0E5E
loc_1_0E29:
        sub    ebx, dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0E93
        mov    eax, dword ptr [esp + 0x14]
        add    dword ptr [edi + 0x14], eax
        cmp    dword ptr [esp + 0x14], ebx
        je     loc_1_0E57
        mov    dword ptr [esp + 0x18], 1
loc_1_0E57:
        mov    byte ptr [esi + 0x114], 0
loc_1_0E5E:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_0D51
loc_1_0E69:
        mov    eax, dword ptr [edi + 0x14]
        cmp    dword ptr [edi + 4], eax
        jne    loc_1_0ECB
        add    dword ptr [esi + 0x115], eax
        mov    eax, dword ptr [esi + 0xf4]
        lea    ecx, [esi + 0xec]
        mov    dword ptr [eax + 0xc], 0
        cmp    dword ptr [ecx], 0
        jne    loc_1_0EB8
        mov    dword ptr [ecx], eax
        jmp    loc_1_0EBE
loc_1_0E93:
        mov    eax, dword ptr [edi + 0x14]
        add    dword ptr [esi + 0x115], eax
        mov    eax, dword ptr [esi + 0xf4]
        lea    ecx, [esi + 0xec]
        mov    dword ptr [eax + 0xc], 0
        cmp    dword ptr [ecx], 0
        jne    loc_1_0EB8
        mov    dword ptr [ecx], eax
        jmp    loc_1_0EBE
loc_1_0EB8:
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0xc], eax
loc_1_0EBE:
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 0xf4], 0
loc_1_0ECB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    4
sub_1_0ED5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x10c], 1
        je     loc_1_0F53
        mov    dword ptr [esi + 0x10c], 1
        mov    edi, dword ptr [ebp + 0xc]
loc_1_0EF6:
        or     edi, edi
        mov    ecx, dword ptr [esi + 0xec]
        je     loc_1_0F0F
        or     ecx, ecx
        je     loc_1_0F31
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0xec], eax
        jmp    loc_1_0F31
loc_1_0F0F:
        or     ecx, ecx
        je     loc_1_0F31
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 0x1c], eax
        jae    loc_1_0F2F
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0xec], eax
        jmp    loc_1_0F31
loc_1_0F2F:
        sub    ecx, ecx
loc_1_0F31:
        or     ecx, ecx
        je     loc_1_0F49
        push   0
        push   dword ptr [ecx + 0x10]
        push   1
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   sub_1_01ED
        jmp    loc_1_0EF6
loc_1_0F49:
        mov    dword ptr [esi + 0x10c], 0
loc_1_0F53:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_0F5B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x24]
loc_1_0F6E:
        push   ebx
        sub    ebp, ebp
        push   dword ptr [esp + 0x24]
        push   edi
        push   esi
        call   sub_1_16EC
        add    esp, 0x10
        cmp    eax, ebp
        je     loc_1_0FAA
        cmp    dword ptr [ebx], ebp
        jne    loc_1_0FA6
        lea    eax, [esp + 0x10]
        push   eax
        push   ebp
        push   esi
        call   sub_1_16D4
        add    esp, 0xc
        cmp    eax, ebp
        je     loc_1_0FAA
        test   byte ptr [esp + 0x11], 1
        jne    loc_1_0FA6
        mov    ebp, 1
loc_1_0FA6:
        or     ebp, ebp
        jne    loc_1_0F6E
loc_1_0FAA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0FB2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0xfc], 0
        je     loc_1_0FD6
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0xc]
        popfd  
        jmp    loc_1_100B
loc_1_0FD6:
        mov    dword ptr [esi + 0xfc], 0xffffffff
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        popfd  
        cmp    dword ptr [esi + 0xf8], 0
        jne    loc_1_1001
        push   esi
        push   0x1082                        ; -> loc_1_1082
        call   sub_1_01A9
        mov    dword ptr [esi + 0xf8], eax
loc_1_1001:
        mov    dword ptr [esi + 0xfc], 0
loc_1_100B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1010:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    dword ptr [ebp - 4], edx
        pushfd 
        pop    dword ptr [ebp - 8]
        cli    
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0xfc
        cmp    dword ptr [ecx], 0
        je     loc_1_1039
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x10]
        popfd  
        jmp    loc_1_107D
loc_1_1039:
        mov    dword ptr [ecx], 0xffffffff
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0xc]
        popfd  
        mov    esi, dword ptr [ebp - 4]
        add    esi, 0xf8
        cmp    dword ptr [esi], 0
        jne    loc_1_1063
        push   dword ptr [ebp - 4]
        push   0x1082                        ; -> loc_1_1082
        call   sub_1_01A9
        mov    dword ptr [esi], eax
loc_1_1063:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x12d], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xfc], 0
loc_1_107D:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1082:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0xf8], 0
        push   dword ptr [esp]
        call   sub_1_0D26
        push   dword ptr [esp]
        call   sub_1_0C37
        add    esp, 4
        ret    
sub_1_10AD:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        push   dword ptr [esi]
        call   sub_1_16E4
        add    esp, 8
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_1_10D0
        cmp    dword ptr [esi + 0xec], 0
        je     loc_1_10DA
loc_1_10D0:
        mov    dword ptr [esi + 0x131], 1
loc_1_10DA:
        mov    eax, dword ptr [esi + 0xf4]
        or     eax, eax
        je     loc_1_1115
        mov    dword ptr [eax + 0xc], 0
        cmp    dword ptr [esi + 0xec], 0
        jne    loc_1_10FC
        mov    dword ptr [esi + 0xec], eax
        jmp    loc_1_1105
loc_1_10FC:
        mov    ecx, dword ptr [esi + 0xf0]
        mov    dword ptr [ecx + 0xc], eax
loc_1_1105:
        mov    dword ptr [esi + 0xf0], eax
        mov    dword ptr [esi + 0xf4], 0
loc_1_1115:
        mov    edx, dword ptr [esi + 0xe4]
        or     edx, edx
        je     loc_1_1167
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xe4], eax
        or     edx, edx
        je     loc_1_1167
        sub    ecx, ecx
loc_1_112E:
        mov    dword ptr [edx + 0xc], ecx
        cmp    dword ptr [esi + 0xec], ecx
        jne    loc_1_1141
        mov    dword ptr [esi + 0xec], edx
        jmp    loc_1_114A
loc_1_1141:
        mov    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [eax + 0xc], edx
loc_1_114A:
        mov    dword ptr [esi + 0xf0], edx
        mov    edx, dword ptr [esi + 0xe4]
        cmp    edx, ecx
        je     loc_1_1167
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xe4], eax
        cmp    edx, ecx
        jne    loc_1_112E
loc_1_1167:
        push   1
        push   esi
        call   sub_1_0ED5
        mov    dword ptr [esi + 0x115], 0
        mov    dword ptr [esi + 0x119], 0
        sub    eax, eax
        pop    esi
        ret    0x10
loc_1_1189:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        lea    edi, [esi + 0x78]
        mov    dword ptr [esi + 0xf4], 0
        mov    dword ptr [esi + 0x80], 0
        mov    dword ptr [esi + 0x84], 0
        call   sub_1_0180
        mov    dword ptr [esi + 0x88], eax
        push   0
        push   edi
        push   0xc0000002
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_12F5
        cmp    dword ptr [esi + 0x7c], 0
        jne    loc_1_12F5
        test   byte ptr [esi + 0x1e], 8
        je     loc_1_11FE
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        push   esi
        call   sub_2_00D6
        jmp    loc_1_1201
loc_1_11FE:
        mov    ebx, dword ptr [ebp + 0x10]
loc_1_1201:
        mov    dword ptr [esi + 0x80], 0
        mov    dword ptr [esi + 0x84], 0
        push   0
        push   edi
        push   0xc0000003
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_12CF
        cmp    dword ptr [esi + 0x7c], 0
        jne    loc_1_12CF
        test   byte ptr [esi + 0x1e], 8
        jne    loc_1_124A
        push   0
        push   0
        push   ebx
        push   esi
        call   sub_2_00D6
loc_1_124A:
        mov    edi, dword ptr [esi]
        push   0
        push   edi
        call   sub_1_16E4
        add    esp, 8
        push   esi
        push   0xfb2                         ; -> loc_1_0FB2
        mov    eax, dword ptr [esi + 0xc]
        shr    eax, 1
        push   eax
        push   edi
        call   sub_1_1704
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_1275
        jmp    loc_1_12F5
loc_1_1275:
        push   esi
        push   0x1475                        ; -> loc_1_1475
        push   2
        push   edi
        call   sub_1_16FC
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_129B
        push   0
        push   0
        push   -1
        push   edi
        call   sub_1_1704
        add    esp, 0x10
        jmp    loc_1_12F5
loc_1_129B:
        mov    dword ptr [esi + 0x110], 0
        mov    dword ptr [esi + 0x115], 0
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x121], eax
        mov    dword ptr [esi + 0x125], 0
        sub    eax, eax
        jmp    loc_1_12FA
loc_1_12CF:
        mov    dword ptr [esi + 0x80], 0
        mov    dword ptr [esi + 0x84], 0
        push   0
        push   edi
        push   0xc0000004
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
loc_1_12F5:
        mov    eax, 2
loc_1_12FA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1303:
        push   esi
        push   edi
        push   0
        push   0
        mov    esi, dword ptr [esp + 0x14]
        push   0
        push   esi
        call   sub_1_10AD
        push   0
        push   0
        push   -1
        push   dword ptr [esi]
        call   sub_1_1704
        add    esp, 0x10
        push   0
        push   0
        push   -1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 0x10
        mov    edi, eax
        push   dword ptr [esi + 0x12d]
        call   sub_1_1744
        mov    dword ptr [esi + 0x12d], 0
        push   dword ptr [esi + 0xf8]
        call   sub_1_01DB
        mov    dword ptr [esi + 0xf8], 0
        push   dword ptr [esi + 0x100]
        call   sub_1_01DB
        mov    dword ptr [esi + 0x100], 0
        test   byte ptr [esi + 0x1e], 0x10
        je     loc_1_138F
        push   0x5f
        lea    eax, [esi + 0xb4]
        push   eax
        push   dword ptr [esi]
        call   sub_1_16C4
        add    esp, 0xc
loc_1_138F:
        mov    dword ptr [esi + 0x80], 0
        mov    dword ptr [esi + 0x84], 0
        push   0
        lea    eax, [esi + 0x78]
        push   eax
        push   0xc0000004
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 0x10
        or     edi, edi
        mov    eax, 2
        je     loc_1_13C3
        sub    eax, eax
loc_1_13C3:
        pop    edi
        pop    esi
        ret    0x10
loc_1_13C8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xf4]
        or     ecx, ecx
        mov    eax, dword ptr [eax + 0x115]
        je     loc_1_13DF
        add    eax, dword ptr [ecx + 0x14]
loc_1_13DF:
        ret    0x10
loc_1_13E2:
        sub    esp, 0xc
        lea    eax, [esp]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax]
        call   sub_1_16CC
        add    esp, 8
        or     eax, eax
        mov    eax, 0xffffffff
        je     loc_1_1405
        mov    eax, dword ptr [esp + 8]
loc_1_1405:
        add    esp, 0xc
        ret    0x10
loc_1_140B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, dword ptr [ebp + 0xc]
        or     edx, edx
        mov    ecx, dword ptr [ebp + 8]
        je     loc_1_1438
        cmp    dword ptr [ecx + 0x119], 0
        jne    loc_1_1438
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x129], eax
        jmp    loc_1_145D
loc_1_1438:
        or     edx, edx
        jne    loc_1_145D
        cmp    dword ptr [ecx + 0x119], 0
        je     loc_1_145D
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [ecx + 0x129]
        add    dword ptr [ecx + 0x125], eax
loc_1_145D:
        mov    dword ptr [ecx + 0x119], edx
        or     edx, edx
        jne    loc_1_146D
        push   ecx
        call   sub_1_0C37
loc_1_146D:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1475:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x104], 0
        je     loc_1_1499
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0xc]
        popfd  
        jmp    loc_1_14CE
loc_1_1499:
        mov    dword ptr [esi + 0x104], 0xffffffff
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        popfd  
        cmp    dword ptr [esi + 0x100], 0
        jne    loc_1_14C4
        push   esi
        push   0x14d3                        ; -> loc_1_14D3
        call   sub_1_01A9
        mov    dword ptr [esi + 0x100], eax
loc_1_14C4:
        mov    dword ptr [esi + 0x104], 0
loc_1_14CE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_14D3:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0x10], edx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x100], 0
        sub    ebx, ebx
        mov    esi, 2
loc_1_14F2:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x13]
        push   1
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax]
        call   sub_1_097C
        add    esp, 0x10
        cmp    eax, ebx
        je     loc_1_160B
        cmp    dword ptr [esp + 0x14], ebx
        je     loc_1_160B
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0x108]
        add    edi, 0x108
        cmp    eax, ebx
        je     loc_1_153D
        cmp    eax, 4
        je     loc_1_15DA
        jmp    loc_1_14F2
loc_1_153D:
        cmp    byte ptr [esp + 0xf], 0x10
        jne    loc_1_154C
        mov    dword ptr [edi], 4
        jmp    loc_1_14F2
loc_1_154C:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x11d], ebx
        je     loc_1_14F2
        mov    cl, byte ptr [esp + 0xf]
        mov    eax, dword ptr [eax + 0x139]
        mov    edx, dword ptr [esp + 0x10]
        mov    byte ptr [eax + edx + 0x141], cl
        mov    eax, dword ptr [esp + 0x10]
        push   ebx
        inc    dword ptr [eax + 0x139]
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0x135
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   0xc0000001
        push   dword ptr [eax]
        call   sub_1_16DC
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0x10
        mov    eax, dword ptr [eax + 0x13d]
        cmp    eax, ebx
        je     loc_1_15AC
        cmp    eax, 3
        jne    loc_1_14F2
loc_1_15AC:
        mov    eax, dword ptr [esp + 0x10]
        push   ebx
        mov    ecx, dword ptr [eax + 0x139]
        mov    byte ptr [eax + ecx + 0x141], bl
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0x20
        push   eax
        call   sub_1_0035
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x11d], ebx
        jmp    loc_1_14F2
loc_1_15DA:
        mov    dword ptr [esp + 0x1c], esi
        mov    byte ptr [esp + 0x24], 0x10
        mov    al, byte ptr [esp + 0xf]
        mov    byte ptr [esp + 0x25], al
        lea    eax, [esp + 0x18]
        push   ebx
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   0xc0000000
        push   dword ptr [eax]
        call   sub_1_16DC
        add    esp, 0x10
        mov    dword ptr [edi], ebx
        jmp    loc_1_14F2
loc_1_160B:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_1612:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    ebx, 2
        mov    edi, dword ptr [esi + 0x131]
        mov    dword ptr [esi + 0x139], 0
        mov    dword ptr [esi + 0x11d], 1
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_10AD
        push   esi
        push   0x1475                        ; -> loc_1_1475
        push   1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 0x10
        lea    ebp, [esi + 0x20]
loc_1_165D:
        push   ebp
        call   0
        mov    byte ptr [esp + 0x13], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x17]
        push   1
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 0x10
        or     edi, edi
        je     loc_1_1687
        mov    ecx, dword ptr [esi + 0x74]
        jmp    loc_1_168A
loc_1_1687:
        mov    ecx, dword ptr [esi + 0x5c]
loc_1_168A:
        or     edi, edi
        lea    eax, [esi + 0x60]
        jne    loc_1_1694
        lea    eax, [esi + 0x48]
loc_1_1694:
        lea    edx, [esp + 0x14]
        push   edx
        push   ecx
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 0x10
        push   0x2710
        push   ebp
        call   sub_1_0067
        dec    ebx
        or     eax, eax
        je     loc_1_16B9
        or     ebx, ebx
        jne    loc_1_165D
loc_1_16B9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0x10
        db  000h
sub_1_16C4:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        add    byte ptr [eax], al
sub_1_16CC:
        VxDCall  BIOS.Service_800Ah              ; cd200a802b00
        add    byte ptr [eax], al
sub_1_16D4:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        add    byte ptr [eax], al
sub_1_16DC:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        add    byte ptr [eax], al
sub_1_16E4:
        VxDCall  BIOS.Service_800Fh              ; cd200f802b00
        add    byte ptr [eax], al
sub_1_16EC:
        VxDCall  BIOS.Service_8012h              ; cd2012802b00
        add    byte ptr [eax], al
sub_1_16F4:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        add    byte ptr [eax], al
sub_1_16FC:
        VxDCall  BIOS.Service_8017h              ; cd2017802b00
        add    byte ptr [eax], al
sub_1_1704:
        VxDCall  BIOS.Service_8018h              ; cd2018802b00
        add    byte ptr [eax], al
sub_1_170C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_1714:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_171C:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_1_1728:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_1_1744:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_1_1754:
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
        add    byte ptr [eax], al
sub_1_175C:
        VMMCall  VMM._SignalID                   ; cd2027810100
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SERWAVE '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  SERWAVE_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  SERWAVE_PM_API           ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x5c2  R-X
loc_2_0000:
        mov    eax, 0x4000000
        ret    0x10
loc_2_0008:
        sub    esp, 0x40
        push   esi
        push   edi
        push   1
        push   0x33d
        call   sub_2_058C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    loc_2_0027
        jmp    loc_2_00B6
loc_2_0027:
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0x50]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 0x58]
        mov    dword ptr [esi + 0x1c], eax
        lea    eax, [esi + 0x2c]
        mov    edi, dword ptr [esi]
        push   0
        push   eax
        push   0xc0000005
        push   edi
        call   sub_2_0584
        add    esp, 0x10
        or     eax, eax
        je     loc_2_00AB
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_2_00AB
        cmp    dword ptr [esi + 0x74], 0
        je     loc_2_00AB
        cmp    dword ptr [esi + 0x44], 0
        je     loc_2_00AB
        lea    eax, [esi + 0xb4]
        push   eax
        push   edi
        call   sub_2_0574
        add    esp, 8
        or     eax, eax
        jne    loc_2_0084
        jmp    loc_2_00AB
loc_2_0084:
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi]
        call   sub_2_057C
        add    esp, 8
        or     eax, eax
        jne    loc_2_0099
        jmp    loc_2_00AB
loc_2_0099:
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, esi
        jmp    loc_2_00B8
loc_2_00AB:
        push   0
        push   esi
        call   sub_2_0594
        add    esp, 8
loc_2_00B6:
        sub    eax, eax
loc_2_00B8:
        pop    edi
        pop    esi
        add    esp, 0x40
        ret    0x10
loc_2_00C0:
        push   0
        push   dword ptr [esp + 8]
        call   sub_2_0594
        add    esp, 8
        mov    eax, 0xffffffff
        ret    0x10
sub_2_00D6:
        sub    esp, 0x30
        push   esi
        lea    eax, [esp + 4]
        mov    esi, dword ptr [esp + 0x38]
        push   eax
        push   dword ptr [esi]
        call   sub_2_0574
        add    esp, 8
        or     eax, eax
        jne    loc_2_00F8
        mov    eax, 2
        jmp    loc_2_0147
loc_2_00F8:
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0xc], 0x4009
        mov    eax, dword ptr [esi + 0x10]
        sub    eax, 0x80
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x14], 0x40
        mov    byte ptr [esp + 0x1a], 8
        mov    byte ptr [esp + 0x1c], 0
        mov    byte ptr [esp + 0x1b], 0
        lea    eax, [esp + 4]
        push   0x5f
        push   eax
        push   dword ptr [esi]
        call   sub_2_056C
        add    esp, 0xc
        or     eax, eax
        mov    eax, 2
        je     loc_2_0147
        sub    eax, eax
loc_2_0147:
        pop    esi
        add    esp, 0x30
        ret    0x10
        push   ebp
        mov    ebp, esp
        push   ecx
        pushfd 
        mov    eax, dword ptr [ebp + 8]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        popfd  
        pop    ecx
        pop    ebp
        ret    
sub_2_0160:
        sub    esp, 0x1ac
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x1b8]
        or     eax, eax
        je     loc_2_0198
        cmp    eax, 1
        je     loc_2_019F
        cmp    eax, 2
        je     loc_2_03D5
        cmp    eax, 3
        je     loc_2_03D9
        cmp    eax, 4
        je     loc_2_03D5
        jmp    loc_2_03D9
loc_2_0198:
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_019F:
        push   dword ptr [0x48]              ; -> loc_3_0048
        lea    eax, [esp + 0x18]
        push   0x4c                          ; -> loc_3_004C
        push   eax
        call   sub_2_059C
        add    esp, 0xc
        mov    eax, dword ptr [0x48]         ; -> loc_3_0048
        inc    eax
        mov    dword ptr [0x48], eax         ; -> loc_3_0048
        lea    edi, [esp + 0x14]
        push   1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        lea    eax, [esp + 0x18]
        push   ecx
        push   eax
        push   1
        push   0x64                          ; -> obj3:0x0064
        mov    esi, dword ptr [esp + 0x1d4]
        push   0
        push   esi
        call   sub_2_05BC
        add    esp, 0x1c
        push   1
        lea    eax, [esp + 0xb8]
        push   0x100
        push   eax
        push   0
        push   esi
        call   sub_2_05AC
        add    esp, 0x14
        or     eax, eax
        je     loc_2_0218
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_0218:
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 4
        jae    loc_2_0237
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_0237:
        lea    edi, [esp + ecx + 0xb0]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0xc]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x70                     ; -> loc_3_0070
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    eax, [esp + 8]
        push   eax
        movsw  word ptr es:[edi], word ptr [esi]
        lea    eax, [esp + 0xb8]
        push   eax
        push   0x80000002
        call   sub_2_054C
        add    esp, 0xc
        or     eax, eax
        je     loc_2_029F
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_029F:
        lea    eax, [esp + 0x34]
        push   0x80
        push   eax
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_2_055C
        add    esp, 0x10
        mov    esi, eax
        push   dword ptr [esp + 8]
        call   sub_2_0554
        add    esp, 4
        or     esi, esi
        je     loc_2_02D0
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_02D0:
        mov    esi, 0x80                     ; -> obj3:0x0080
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xd
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    edi, [esp + 0x34]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0xb8                     ; -> obj3:0x00B8
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        movsw  word ptr es:[edi], word ptr [esi]
        lea    edi, [esp + 0xc]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0xbc                     ; -> loc_3_00BC
        lea    edi, [esp + 0xb4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        lea    eax, [esp + 8]
        push   eax
        movsw  word ptr es:[edi], word ptr [esi]
        lea    eax, [esp + 0xb8]
        push   eax
        push   0x80000002
        call   sub_2_054C
        add    esp, 0xc
        or     eax, eax
        je     loc_2_039F
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_039F:
        push   2
        push   0xc0                          ; -> obj3:0x00C0
        push   1
        push   0
        push   0xc4                          ; -> obj3:0x00C4
        push   dword ptr [esp + 0x1c]
        call   sub_2_0564
        add    esp, 0x18
        mov    esi, eax
        push   dword ptr [esp + 8]
        call   sub_2_0554
        add    esp, 4
        or     esi, esi
        je     loc_2_03D1
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_03D1:
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_03D5:
        sub    eax, eax
        jmp    loc_2_03DE
loc_2_03D9:
        mov    eax, 1
loc_2_03DE:
        pop    edi
        pop    esi
        add    esp, 0x1ac
        ret    
loc_2_03E7:
        cmp    dword ptr [esp + 4], 5
        mov    eax, 1
        jne    loc_2_03FE
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_2_03FF
        sub    eax, eax
loc_2_03FE:
        ret    
sub_2_03FF:
        sub    esp, 0x188
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0xc], 0x80
        lea    eax, [esp + 0xc]
        push   0
        push   eax
        mov    ebx, ecx
        lea    eax, [esp + 0x1c]
        push   eax
        push   1
        push   0xd0                          ; -> obj3:0x00D0
        push   0
        push   ebx
        call   sub_2_05B4
        add    esp, 0x1c
        or     eax, eax
        je     loc_2_043B
        sub    eax, eax
        jmp    loc_2_0540
loc_2_043B:
        sub    ecx, ecx
        cmp    dword ptr [esp + 0xc], ecx
        jbe    loc_2_0456
loc_2_0443:
        lea    eax, [esp + ecx + 0x14]
        cmp    byte ptr [eax], 0x20
        jne    loc_2_044F
        mov    byte ptr [eax], 0x5f
loc_2_044F:
        inc    ecx
        cmp    ecx, dword ptr [esp + 0xc]
        jb     loc_2_0443
loc_2_0456:
        mov    esi, 0xdc                     ; -> loc_3_00DC
        lea    edi, [esp + 0x94]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edi, [esp + 0x14]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0x94]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   0
        lea    eax, [esp + 0x98]
        push   ebx
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_2_05A4
        add    esp, 0x10
        cmp    eax, 2
        jne    loc_2_04BF
        sub    eax, eax
        jmp    loc_2_0540
loc_2_04BF:
        cmp    eax, 0x10
        jne    loc_2_04C6
        jmp    loc_2_053B
loc_2_04C6:
        or     eax, eax
        mov    eax, 0
        jne    loc_2_0540
        push   0
        push   0x1c
        push   0xe8                          ; -> loc_3_00E8
        push   1
        push   0x104                         ; -> obj3:0x0104
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_2_05BC
        add    esp, 0x1c
        push   0
        push   6
        push   0x110                         ; -> loc_3_0110
        push   1
        push   0x118                         ; -> obj3:0x0118
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_2_05BC
        add    esp, 0x1c
        lea    edi, [esp + 0x14]
        push   0
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        lea    eax, [esp + 0x98]
        push   ecx
        push   eax
        push   1
        push   0x120                         ; -> obj3:0x0120
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_2_05BC
        add    esp, 0x1c
loc_2_053B:
        mov    eax, 1
loc_2_0540:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x188
        ret    
        db  000h, 000h
sub_2_054C:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_2_0554:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_2_055C:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        add    byte ptr [eax], al
sub_2_0564:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_2_056C:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        add    byte ptr [eax], al
sub_2_0574:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        add    byte ptr [eax], al
sub_2_057C:
        VxDCall  BIOS.Service_800Dh              ; cd200d802b00
        add    byte ptr [eax], al
sub_2_0584:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        add    byte ptr [eax], al
sub_2_058C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_2_0594:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_2_059C:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        add    byte ptr [eax], al
sub_2_05A4:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        add    byte ptr [eax], al
sub_2_05AC:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        add    byte ptr [eax], al
sub_2_05B4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_2_05BC:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x12c  RW-
sub_3_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax - 0x52fffff5], dh ; -> loc_1_0BB0
        adc    byte ptr [eax], al
        add    byte ptr [ecx + 0x3000011], cl ; -> loc_1_1189
        adc    eax, dword ptr [eax]
        add    byte ptr [eax + 0x2c000002], dh ; -> loc_1_02B0
        add    al, 0
        add    ch, dl                        ; -> loc_1_05D5
        add    eax, 0x9d60000                ; -> loc_1_09D6
        add    byte ptr [eax], al
        enter  0x13, 0
        salc   
        add    byte ptr [eax], al
        add    byte ptr [edi], bl            ; -> loc_1_0B1F
        or     eax, dword ptr [eax]
        add    dl, ah                        ; -> loc_1_13E2
        adc    eax, dword ptr [eax]
        add    byte ptr [ecx + 0xb], al      ; -> loc_1_0B41
        add    byte ptr [eax], al
        or     edx, dword ptr [eax + eax]
        add    byte ptr [edx], dl            ; -> loc_1_1612
        push   ss
        add    byte ptr [eax], al
loc_3_0048:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_004C:
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x646f4d20
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x61], dl
        jbe    loc_3_00C1
        and    byte ptr [ebx], ah
        and    eax, 0x6432
        add    byte ptr [edi + 0x61], dl
        jbe    loc_3_00CD
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_3_0070:
        pop    esp
        inc    esp
        jb     loc_3_00DD
        jbe    0xdb
        jb     0xeb
        pop    esp
        ja     loc_3_00DC
        jbe    loc_3_00E2
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    0xf8
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0xfc
        jb     0xf1
        outsb  dx, byte ptr [esi]
        je     0xd2
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x105
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0xf5
        arpl   word ptr [edi + 0x6e], bp
        je     loc_3_0110
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x52], 0x756f7365
        jb     loc_3_0110
        jae    0x10c
        ja     loc_3_0113
        jbe    0x119
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
loc_3_00BC:
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], dh
loc_3_00C1:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x61], cl
        jo     0x138
        popal  
        bound  ebp, qword ptr [ebp]
loc_3_00CD:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x63736544
        add    byte ptr [eax], al
loc_3_00DC:
        dec    ebp
loc_3_00DD:
        dec    edi
        inc    esp
        inc    ebp
        dec    ebp
        push   edi
loc_3_00E2:
        inc    ecx
        push   esi
        inc    ebp
        pop    esp
        add    byte ptr [eax], al
loc_3_00E8:
        push   edi
        popal  
        jbe    0x151
        and    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x726f6620
        and    byte ptr [esi + 0x6f], dl
        imul   esp, dword ptr [ebx + 0x65], 0x646f4d20
        insd   dword ptr es:[edi], dx
        add    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x63736544
        add    byte ptr [eax], al
loc_3_0110:
        dec    ebp
        inc    ebp
        inc    esp
loc_3_0113:
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6c], al
        popal  
        jae    0x190
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x61], cl
        jb     0x188
        ja     0x187
        jb     0x18d
        dec    ecx
        inc    esp
        and    al, 0x53
VxD__LDATA ENDS
