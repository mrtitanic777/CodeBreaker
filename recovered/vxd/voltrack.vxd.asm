; voltrack -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 1533 instructions recovered by recursive descent

Declare_Virtual_Device voltrack, 3, 10, voltrack_Control, 00090h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5f8  R-X
voltrack_Control:
        cmp    eax, 0x1b
        je     loc_3_0000
        clc    
        ret    
        db  000h
sub_1_000C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0xa8]
        cmp    word ptr [esi + 0x68], 4
        ja     loc_1_0118
loc_1_0027:
        call   sub_1_018A
        jne    loc_1_004B
        test   byte ptr [esi + 0x6c], 0x20
        jne    loc_1_004B
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 0x1d], 0x20
        jne    loc_1_02EE
loc_1_0041:
        mov    ebx, dword ptr [esi + 4]
        cli    
        test   byte ptr [ebx + 0x22], 8
        jne    loc_1_0092
loc_1_004B:
        sti    
        test   byte ptr [ebx + 0x21], 0x80
        inc    byte ptr [ebx + 0x78]
        call   sub_1_018A
        jne    loc_1_0082
        test   dword ptr [esi + 0x6c], 0x28
        jne    loc_1_0082
        or     edi, edi
        je     loc_1_0082
        test   byte ptr [edi + 4], 0x20
        jne    loc_1_013E
        mov    ebx, dword ptr [esi + 8]
        test   dword ptr [ebx + 0x20], 0x101000
        jne    loc_1_008B
        cmp    dword ptr [ebx + 0x18], edi
        jne    loc_1_008B
loc_1_0082:
        call   sub_1_02B0
loc_1_0087:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_008B:
        call   sub_1_033E
        jmp    loc_1_0087
loc_1_0092:
        test   byte ptr [ebx + 0xb8], 2
        jne    loc_1_00EF
        test   byte ptr [esi + 0x6d], 0x40
        jne    loc_1_004B
        test   byte ptr [ebx + 0xb8], 4
        jne    loc_1_004B
        test   byte ptr [ebx + 0x22], 4
        je     loc_1_00BD
        cmp    word ptr [esi + 0x68], 0
        jne    loc_1_00BD
        test   byte ptr [esi + 0x6d], 8
        jne    loc_1_00C3
loc_1_00BD:
        cmp    byte ptr [ebx + 0x78], 1
        ja     loc_1_00EF
loc_1_00C3:
        test   byte ptr [esi + 0x6c], 8
        jne    loc_1_00D8
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_00D8
        cmp    byte ptr [ebx + 0x78], 0
        jne    loc_1_00EF
loc_1_00D8:
        or     byte ptr [ebx + 0xb8], 2
        sti    
        mov    edx, esi
        mov    esi, 0                        ; -> loc_4_0000
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_0087
loc_1_00EF:
        sti    
        test   byte ptr [esi + 0x6c], 8
        jne    loc_1_004B
        or     edi, edi
        je     loc_1_004B
        cli    
        mov    ecx, dword ptr [ebx + 0xaf]
        mov    dword ptr [esi + 0x64], ecx
        mov    dword ptr [ebx + 0xaf], esi
        sti    
        jmp    loc_1_0087
loc_1_0118:
        movzx  eax, word ptr [esi + 0x68]
        cld    
        lea    edi, [0x166]                  ; -> loc_1_0166
        mov    ecx, 6
        repne scasw ax, word ptr es:[edi]
        mov    edi, dword ptr [esi + 0xa8]
        jne    loc_1_0027
        jmp    dword ptr [ecx*4 + 0x172]     ; -> obj1:0x0172
loc_1_013E:
        mov    word ptr [esi + 0x6a], 0x11
        push   esi
        call   sub_1_0484
        add    esp, 4
        jmp    loc_1_0087
loc_1_0152:
        call   sub_1_050C
        jmp    loc_1_0087
loc_1_015C:
        call   sub_1_02CD
        jmp    loc_1_0087
loc_1_0166:
        or     dword ptr [eax], eax
        or     al, byte ptr [eax]
        or     byte ptr [eax], al
        or     eax, 0x28002700
        add    byte ptr [ecx + eax], bl      ; -> loc_1_015C
        db  000h
        dd  loc_1_015C               ; fixup
        dd  loc_4_0373               ; fixup
        dd  loc_1_0282               ; fixup
        dd  loc_1_0209               ; fixup
        dd  loc_1_019A               ; fixup
sub_1_018A:
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_1_0195
        mov    eax, ebx
loc_1_0195:
        test   byte ptr [eax + 0x21], 1
        ret    
loc_1_019A:
        mov    word ptr [esi + 0x6a], 0x16
        test   byte ptr [ebx + 0x1e], 1
        je     loc_1_0152
        mov    word ptr [esi + 0x6a], 0x18
        or     edi, edi
        je     loc_1_01B6
        test   byte ptr [edi + 4], 0x20
        jne    loc_1_0152
loc_1_01B6:
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 0xb8], 1
        jne    loc_1_0152
        mov    word ptr [esi + 0x6a], 0x26
        cmp    byte ptr [ebx + 0x79], 0xff
        je     loc_1_0152
        mov    word ptr [esi + 0x6a], 0
        inc    byte ptr [ebx + 0x79]
        cmp    byte ptr [ebx + 0x79], 1
        je     loc_1_01F4
        inc    byte ptr [ebx + 0x78]
        mov    word ptr [esi + 0x6a], 0x23
        push   esi
        call   sub_1_0484
        add    esp, 4
        jmp    loc_1_0087
loc_1_01F4:
        push   esi
        xor    esi, esi
        xchg   dword ptr [ebx + 0xb3], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        jmp    loc_1_0027
loc_1_0209:
        mov    word ptr [esi + 0x6a], 0x16
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 0x1e], 1
        je     loc_1_0152
        mov    word ptr [esi + 0x6a], 0x22
        cmp    byte ptr [ebx + 0x79], 0
        je     loc_1_0152
        mov    word ptr [esi + 0x6a], 0
        dec    byte ptr [ebx + 0x79]
        jne    loc_1_0152
        test   byte ptr [ebx + 0xb8], 1
        jne    loc_1_0027
        push   esi
        mov    eax, 0x1388
        mov    edx, ebx
        mov    esi, 0x267                    ; -> loc_1_0267
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0xb3], esi
        pop    esi
        jmp    loc_1_0152
loc_1_0267:
        mov    ebx, edx
        xor    eax, eax
        mov    dword ptr [ebx + 0xb3], eax
        cmp    byte ptr [ebx + 0x79], al
        jne    loc_1_0281
        mov    ax, 0xa
        xor    ecx, ecx
        call   sub_4_0726
loc_1_0281:
        ret    
loc_1_0282:
        test   byte ptr [esi + 0x6f], 0x10
        je     loc_1_029E
        push   0
        push   esi
        push   0x22f                         ; -> loc_4_022F
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        jmp    loc_1_0087
loc_1_029E:
        mov    edx, esi
        mov    esi, 0x23f                    ; -> sub_4_023F
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_0087
sub_1_02B0:
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 0x1d], 0x20
        jne    loc_1_02DD
loc_1_02B9:
        cmp    word ptr [esi + 0x68], 0xf
        je     loc_1_02E6
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [eax], 0x484        ; -> sub_1_0484
        add    dword ptr [esi + 0x14], 8
sub_1_02CD:
        push   esi
        mov    eax, dword ptr [esi + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x10], eax
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_02DD:
        mov    byte ptr [esi + 0xb1], 0
        jmp    loc_1_02B9
loc_1_02E6:
        mov    eax, dword ptr [esi + 4]
        dec    byte ptr [eax + 0x78]
        jmp    sub_1_02CD
loc_1_02EE:
        mov    al, byte ptr [esi + 0xb1]
        test   byte ptr [esi + 0x6e], 1
        jne    loc_1_0307
        mov    byte ptr [esi + 0x19], al
        mov    ecx, dword ptr [ebx]
        cmp    byte ptr [ecx + 0xae], al
        jne    loc_1_030C
loc_1_0307:
        jmp    loc_1_0041
loc_1_030C:
        call   sub_4_01D9
        test   byte ptr [esi + 0x6c], 8
        je     loc_1_0041
        test   byte ptr [ebx + 0x21], 0x20
        je     loc_1_0324
        mov    ebx, dword ptr [esi + 8]
loc_1_0324:
        test   byte ptr [ebx + 0x1e], 4
        je     loc_1_0041
        mov    ax, 6
        xor    ecx, ecx
        call   sub_4_0731
        jmp    loc_1_0041
sub_1_033E:
        cli    
        mov    edx, dword ptr [ebx]
        inc    byte ptr [edx + 0x76]
        test   byte ptr [edi + 4], 4
        jne    loc_1_0355
        cmp    dword ptr [ebx + 0x18], edi
        je     sub_1_036C
        cmp    byte ptr [edx + 0x78], 1
        je     sub_1_036C
loc_1_0355:
        dec    byte ptr [edx + 0x78]
        dec    byte ptr [edx + 0x76]
        mov    ecx, dword ptr [edx + 0xaf]
        mov    dword ptr [esi + 0x64], ecx
        mov    dword ptr [edx + 0xaf], esi
        sti    
        ret    
sub_1_036C:
        or     byte ptr [edi + 4], 4
        sti    
        mov    edx, esi
        mov    esi, 0x430                    ; -> loc_4_0430
        VMMCall  VMM.Schedule_Global_Event       ; cd200e800100
        add    byte ptr [eax], al
loc_1_0380:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  esi, word ptr [ebx]
        cmp    si, 7
        je     loc_1_03B8
        cmp    si, 3
        je     loc_4_0940
        cmp    si, 2
        je     loc_4_09BC
        cmp    si, 0xe
        je     loc_4_09CC
        mov    word ptr [ebx + 2], 0xffff
        ret    
loc_1_03B8:
        mov    edx, dword ptr [0x5d8]        ; -> loc_1_05D8
        mov    eax, dword ptr [edx + 0x30]
loc_1_03C1:
        or     eax, eax
        je     loc_1_041B
        cmp    byte ptr [eax + 0x40], 0
        je     loc_1_03DC
        cmp    byte ptr [eax + 0x40], 0xa
        je     loc_1_03DC
        cmp    byte ptr [eax + 0x40], 5
        je     loc_1_03DC
loc_1_03D7:
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_03C1
loc_1_03DC:
        mov    ecx, dword ptr [eax + 0x20]
        and    ecx, 0x48004
        cmp    ecx, 0x8004
        jne    loc_1_03D7
        cmp    byte ptr [eax + 0x78], 0
        jne    loc_1_03D7
        mov    cl, byte ptr [eax + 0xb7]
        inc    byte ptr [eax + 0xb7]
        and    cl, 3
        cmp    cl, 3
        jne    loc_1_03D7
        mov    cl, 1
        test   dword ptr [eax + 0x20], 0x30000
        je     loc_1_0414
        xor    cl, cl
loc_1_0414:
        call   sub_1_041C
        jmp    loc_1_03D7
loc_1_041B:
        ret    
sub_1_041C:
        pushfd 
        or     cl, cl
        mov    ecx, 0x1000
        je     loc_1_0433
        or     ecx, 0x100000
        or     dword ptr [eax + 0x20], 0x80000
loc_1_0433:
        push   edx
        push   eax
        test   dword ptr [eax + 0x20], 0x4000
        jne    loc_1_0441
        mov    eax, dword ptr [eax + 0x10]
loc_1_0441:
        or     eax, eax
        je     loc_1_047F
        test   dword ptr [eax + 0x20], 4
        je     loc_1_0479
        or     dword ptr [eax + 0x20], ecx
        mov    edx, dword ptr [0x5d8]        ; -> loc_1_05D8
        mov    edx, dword ptr [edx + 0x40]
        cli    
loc_1_045B:
        or     edx, edx
        je     loc_1_0479
        cmp    dword ptr [edx + 0x28], eax
        jne    loc_1_0474
        or     dword ptr [edx + 4], 2
        test   ecx, 0x100000
        je     loc_1_0474
        or     dword ptr [edx + 4], 1
loc_1_0474:
        mov    edx, dword ptr [edx + 0x24]
        jmp    loc_1_045B
loc_1_0479:
        sti    
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_1_0441
loc_1_047F:
        pop    eax
        pop    edx
        popfd  
        ret    
        db  000h
sub_1_0484:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0xa8]
        mov    ebx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 0x1d], 0x20
        jne    loc_1_04F4
loc_1_049D:
        cmp    word ptr [esi + 0x6a], 0
        jne    loc_1_051C
        mov    byte ptr [ebx + 0xb7], 0
loc_1_04AB:
        dec    byte ptr [ebx + 0x78]
        test   byte ptr [ebx + 0xb8], 1
        jne    loc_1_04CC
loc_1_04B7:
        push   ebx
        call   sub_1_050C
        pop    ebx
        cmp    dword ptr [ebx + 0xaf], 0
        jne    loc_1_04FF
loc_1_04C7:
        sti    
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_1_04CC:
        cmp    word ptr [esi + 0x68], 8
        jne    loc_1_04B7
        and    byte ptr [ebx + 0xb8], 0xfe
        cmp    word ptr [esi + 0x6a], 0x10
        jae    loc_1_04B7
        push   0
        push   ebx
        push   0xb64                         ; -> loc_4_0B64
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        jmp    loc_1_04B7
loc_1_04F4:
        mov    al, byte ptr [esi + 0x19]
        mov    byte ptr [esi + 0xb1], al
        jmp    loc_1_049D
loc_1_04FF:
        cmp    byte ptr [ebx + 0x78], 0
        jne    loc_1_04C7
        call   sub_4_0B32
        jmp    loc_1_04C7
sub_1_050C:
        mov    eax, dword ptr [esi + 0x14]
        sub    eax, 8
        mov    dword ptr [esi + 0x14], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_051C:
        cmp    word ptr [esi + 0x68], 9
        jne    loc_1_0526
        dec    byte ptr [ebx + 0x79]
loc_1_0526:
        call   sub_1_018A
        jne    loc_1_04AB
        test   byte ptr [esi + 0x6c], 0x20
        jne    loc_1_04AB
        inc    byte ptr [ebx + 0x76]
        mov    edx, esi
        mov    esi, 0xa18                    ; -> loc_4_0A18
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_04C7
sub_1_0550:
        push   ebx
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebx + 8]
loc_1_0557:
        cmp    dword ptr [eax], 0xc          ; -> sub_1_000C
        je     loc_1_0566
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        jne    loc_1_0557
loc_1_0566:
        mov    dword ptr [esi + 0x10], eax
        pop    ebx
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00090h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'voltrack'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  voltrack_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_05C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_05D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_05D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_05F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_05F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1ff  R-X
loc_2_0000:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0004:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        pop    ebp
        add    byte ptr [edi], bh
        inc    dword ptr [eax]
loc_2_0016:
        and    al, 1
loc_2_0018:
        inc    ebx
        db  001h
loc_2_001A:
        sahf   
        add    edi, ebx
        add    dword ptr [eax], eax
        add    byte ptr [ebp + 0x72], al
        jb     0x93
        jb     loc_2_0046
        push   edx
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0x73694420
        imul   esp, dword ptr [eax], 0x69
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    loc_2_00A0
        and    byte ptr [0x50000073], ah
        insb   byte ptr es:[edi], dx
        popal  
        jae    0xab
loc_2_0046:
        and    byte ptr [ecx + 0x6e], cl
        jae    0xb0
        jb     0xc1
        and    byte ptr [ecx + ebp*2 + 0x73], al
        imul   esp, dword ptr [eax], 0x25
        xor    dword ptr [ebx + 0xd], esi
        or     dl, byte ptr [edi + 0x69]
        je     loc_2_00C4
        and    byte ptr [ebx + 0x65], dl
        jb     0xca
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x75], cl
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [0x69207332], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    0xdc
        and    byte ptr [0x7333], ah
        inc    ebp
        jb     0xf2
        outsd  dx, dword ptr [esi]
        jb     loc_2_00A3
        push   edx
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0x2d444320
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    0xff
        and    byte ptr [0x50000073], ah
loc_2_00A0:
        insb   byte ptr es:[edi], dx
        popal  
loc_2_00A3:
        jae    0x10a
        and    byte ptr [ecx + 0x6e], cl
        jae    0x10f
        jb     loc_2_0120
        and    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        and    eax, 0xa0d7331
        push   edi
        imul   esi, dword ptr [eax + ebp*2 + 0x20], 0x69726553
        popal  
        insb   byte ptr es:[edi], dx
loc_2_00C4:
        and    byte ptr [esi + 0x75], cl
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [0x69207332], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    loc_2_013D
        and    byte ptr [0xa0d7333], ah
        or     eax, 0x2066490a
        je     0x14d
        and    byte ptr gs:[ebx + 0x44], al
        sub    eax, 0x204d4f52
        imul   esi, dword ptr [ebx + 0x20], 0x6c697473
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    loc_2_0168
        sub    al, 0x20
        imul   esi, dword ptr [eax + 0x6d], 0x72207961
        jno    loc_2_0185
        imul   esi, dword ptr [edx + 0x65], 0x656c6320
        popal  
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x5000002e
loc_2_0120:
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_2_018A
        and    byte ptr [ecx + 0x6e], cl
        jae    0x18f
        jb     0x1a0
        and    byte ptr [ecx + ebp*2 + 0x73], al
        imul   esp, dword ptr [ebp + 0x74], 0x74
        and    byte ptr gs:[esi + 0x6f], ah
        jb     loc_2_015A
        inc    esp
        jb     loc_2_01A6
loc_2_013D:
        jbe    loc_2_01A4
        and    byte ptr [0x45000073], ah
        push   0x65
        arpl   word ptr [eax + 0x52], si
        jno    0x1c3
        jae    0x1c5
        and    byte ptr [edi + ebp*2 + 0x20], dh
        inc    esp
        jb     0x1c1
        jbe    loc_2_01BF
loc_2_015A:
        and    byte ptr [ecx + 0x6e], cl
        and    byte ptr [ebp + 0x73], dl
        add    byte ptr gs:[eax], al
        inc    ecx
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x4a], al
loc_2_0168:
        inc    ebp
        inc    ebx
        push   esp
        and    byte ptr [edx + 0x65], dh
        jno    0x1e5
        jae    loc_2_01E7
        and    byte ptr [edi + 0x61], dh
        jae    0x198
        jb     loc_2_01DF
        arpl   word ptr [ebp + 0x69], sp
        jbe    loc_2_01E4
        and    byte ptr fs:[esi + 0x6f], ah
        jb     0x1a5
loc_2_0185:
        popal  
        and    byte ptr [edx + esi*2 + 0x69], ah
loc_2_018A:
        jbe    loc_2_01F1
        and    byte ptr [eax + ebp*2 + 0x61], dh
        je     0x1b2
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
        jae    0x200
        or     eax, 0x6e6f430a
        je     0x20c
        outsb  dx, byte ptr [esi]
loc_2_01A4:
        jne    0x20b
loc_2_01A6:
        and    byte ptr [edi + 0x69], dh
        je     0x213
        and    byte ptr [ebp + 0x6a], ah
        arpl   word ptr gs:[eax + 0x6f], si
        jo     0x21a
        jb     0x218
        je     0x222
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        aas    
        add    byte ptr [eax], al
        push   esp
loc_2_01BF:
        push   0x72642065
        imul   esi, dword ptr [esi + 0x65], 0x6e616320
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x1ef
        bound  esp, qword ptr [ebp + 0x20]
        push   0x65
        arpl   word ptr [ebp + 0x64], si
        and    byte ptr [edx + 0x65], ah
        arpl   word ptr [ecx + 0x75], sp
loc_2_01DF:
        jae    0x246
        and    byte ptr [ecx + 0x74], ch
loc_2_01E4:
        and    byte ptr [ecx + 0x73], ch
loc_2_01E7:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
loc_2_01F1:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x79], dh
        jae    0x26e
        insd   dword ptr es:[edi], dx
        add    byte ptr cs:[eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xbc  R-X
loc_3_0000:
        mov    ecx, 1
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        mov    dword ptr [0x5f0], eax        ; -> loc_1_05F0
        push   ebp
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x52
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ebx, word ptr [ebp + 0x38]
        shl    ebx, 4
        mov    dword ptr [0x5f4], ebx        ; -> loc_1_05F4
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        mov    dword ptr [0x90], 0x380       ; -> loc_3_0090
        mov    dword ptr [0x94], 0x5bc       ; -> loc_3_0094
        push   0x84                          ; -> loc_3_0084
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        ret    
loc_3_0084:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0090:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0094:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        jne    0x10a
        and    byte ptr gs:[edx + esi*2 + 0x61], dl
        arpl   word ptr [ebx + 0x69], bp
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + si], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xb88  R-X
loc_4_0000:
        mov    esi, 0                        ; -> loc_4_0000
        mov    edi, 0x12                     ; -> loc_4_0012
        call   sub_4_087E
        jae    loc_4_0012
        ret    
loc_4_0012:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        xor    ecx, ecx
        test   byte ptr [ebx + 0x1e], 4
        je     loc_4_0028
        mov    ax, 6
        call   sub_4_0731
loc_4_0028:
        mov    ax, 0xc
        call   sub_4_0731
        mov    ebx, dword ptr [esi + 4]
        or     eax, eax
        jne    loc_4_018A
        and    dword ptr [ebx + 0x20], 0xfff7ffff
        push   esi
        mov    ebx, dword ptr [esi + 8]
        mov    ax, 0x22
        xor    ecx, ecx
        mov    edx, 0x71b                    ; -> sub_4_071B
        test   byte ptr [ebx + 0x21], 0x20
        je     loc_4_0066
        test   byte ptr [ebx + 0xb8], 0x40
        je     loc_4_0066
        mov    edx, 0x848                    ; -> loc_4_0848
loc_4_0066:
        call   edx
        cmp    ax, 0x10
        jb     loc_4_0114
        and    byte ptr [ebx + 0x20], 0xfd
        mov    eax, ebx
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_0080
        mov    eax, dword ptr [ebx]
loc_4_0080:
        mov    edx, dword ptr [eax + 0xdb]
        sub    esp, 0x10
        mov    esi, esp
        mov    word ptr [esi + 4], dx
        mov    word ptr [esi + 0xa], 9
        mov    word ptr [esi], 3
        mov    byte ptr [esi + 0xc], 8
        push   esi
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        mov    eax, dword ptr [esi + 6]
        add    esp, 0x14
        or     eax, eax
        je     loc_4_0178
        mov    ecx, eax
        test   byte ptr [ebx + 0x21], 0x20
        je     loc_4_00D5
        xor    esi, esi
        mov    ax, 0
        call   sub_4_071B
        cmp    dword ptr [ecx + 3], 0x4d44534d
        jne    loc_4_00D5
        cmp    byte ptr [ecx + 7], 0x46
        je     loc_4_00FD
loc_4_00D5:
        xor    esi, esi
        inc    esi
        mov    ax, 0
        call   sub_4_071B
        cmp    ax, 0x10
        jae    loc_4_00FD
        xor    esi, esi
        inc    esi
        mov    ax, 1
        call   sub_4_071B
        cmp    ax, 0x10
        jae    loc_4_00FD
        or     byte ptr [ebx + 0x20], 2
loc_4_00FD:
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], ecx
        mov    word ptr [esi], 4
        push   esi
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        add    esp, 0xc
loc_4_0114:
        mov    ecx, ebx
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_011E
        mov    ecx, dword ptr [ebx]
loc_4_011E:
        mov    edx, dword ptr [0x5d8]        ; -> loc_1_05D8
        mov    edx, dword ptr [edx + 0x40]
        cli    
loc_4_0128:
        or     edx, edx
        je     loc_4_0158
        cmp    dword ptr [edx + 0x28], ecx
        jne    loc_4_0153
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [edx + 0x20], eax
        or     byte ptr [edx + 2], 0x80
        test   byte ptr [ebx + 0x22], 4
        jne    loc_4_0145
        and    byte ptr [edx + 2], 0x7f
loc_4_0145:
        or     byte ptr [edx + 4], 0x40
        test   byte ptr [ebx + 0x20], 2
        je     loc_4_0153
        and    byte ptr [edx + 4], 0xbf
loc_4_0153:
        mov    edx, dword ptr [edx + 0x24]
        jmp    loc_4_0128
loc_4_0158:
        sti    
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_0178
        mov    ecx, dword ptr [ecx + 0x10]
        jecxz  loc_4_0178
        cmp    ecx, ebx
        je     loc_4_011E
        and    byte ptr [ecx + 0x20], 0xfd
        test   byte ptr [ebx + 0x20], 2
        je     loc_4_011E
        or     byte ptr [ecx + 0x20], 2
        jmp    loc_4_011E
loc_4_0178:
        mov    ebx, dword ptr [ebx]
        and    byte ptr [ebx + 0xb8], 0xfd
        call   sub_1_000C
        add    esp, 4
        ret    
loc_4_018A:
        mov    ebx, dword ptr [esi + 4]
        and    byte ptr [ebx + 0xb8], 0xfd
        inc    byte ptr [ebx + 0x78]
        cmp    word ptr [esi + 0x68], 0xd
        jne    loc_4_01A7
        mov    word ptr [esi + 0x6a], ax
        jmp    sub_1_02B0
loc_4_01A7:
        mov    word ptr [esi + 0x6a], ax
        cmp    ax, 0x1c
        je     loc_4_01B7
        mov    word ptr [esi + 0x6a], 0x18
loc_4_01B7:
        push   esi
        call   sub_1_0484
        add    esp, 4
        ret    
        db  083h, 0ECh, 00Ch, 066h, 0C7h, 004h, 024h, 00Eh, 000h, 089h, 054h, 024h, 004h, 054h, 0FFh, 015h
        dd  obj1:0x05BC              ; fixup
        db  083h, 0C4h, 010h, 0C3h
sub_4_01D9:
        add    al, 0x41
        mov    byte ptr [4], al              ; -> obj5:0x0004
        xor    edi, edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        lea    ecx, [4]                      ; -> obj5:0x0004
        push   ecx
        push   0xc0005
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    ecx, eax
        mov    eax, 0x40001010
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        jmp    loc_4_0211
loc_4_0211:
        mov    ebx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0xb1]
        mov    byte ptr [ebx + 0xae], al
        mov    edi, dword ptr [esi + 0xa8]
        push   word ptr [esi + 0xe]
        pop    word ptr [esi + 0xc]
        ret    
loc_4_022F:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        call   sub_4_023F
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_023F:
        mov    esi, edx
        mov    word ptr [esi + 0x6a], 0x16
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 0x1e], 2
        je     sub_1_050C
        or     byte ptr [ebx + 0xb8], 1
        test   byte ptr [esi + 0x6f], 0x10
        je     loc_4_02C9
        call   sub_1_018A
        je     loc_4_029B
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x10000030
        movzx  edi, word ptr [0x16]          ; -> loc_2_0016
        add    edi, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x1a]          ; -> loc_2_001A
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        mov    ebx, dword ptr [esi + 4]
        jmp    loc_4_0360
loc_4_029B:
        sub    esp, 0xc
        mov    eax, esp
        mov    word ptr [eax], 0xc
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], 1
        push   eax
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        movzx  eax, word ptr [eax + 2]
        add    esp, 0x10
        mov    word ptr [esi + 0x6a], 0x25
        or     eax, eax
        je     loc_4_02ED
        jmp    loc_4_0319
loc_4_02C9:
        mov    ecx, dword ptr [ebx + 0x10]
loc_4_02CC:
        or     ecx, ecx
        je     loc_4_02ED
        cmp    byte ptr [ecx + 0x46], 0
        je     loc_4_02E8
        push   ecx
        push   0
        movzx  ecx, byte ptr [ecx + 0x14]
        push   ecx
        VxDCall  IFSMgr._VolFlush                ; cd204c004000
        add    esp, 8
        pop    ecx
loc_4_02E8:
        mov    ecx, dword ptr [ecx + 0x10]
        jmp    loc_4_02CC
loc_4_02ED:
        cmp    byte ptr [ebx + 0x79], 0
        jne    loc_4_0319
loc_4_02F3:
        mov    ax, 0xa
        xor    ecx, ecx
        call   sub_4_071B
        inc    byte ptr [ebx + 0x78]
        test   byte ptr [ebx + 0x22], 4
        je     sub_1_02B0
        mov    eax, ebx
        mov    cl, 1
        call   sub_1_041C
        jmp    sub_1_02B0
loc_4_0319:
        mov    ebx, dword ptr [esi + 4]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x40001011
        test   byte ptr [esi + 0x6f], 0x10
        je     loc_4_0332
        mov    eax, 0x10000031
loc_4_0332:
        movzx  edi, word ptr [0x16]          ; -> loc_2_0016
        add    edi, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x18]          ; -> loc_2_0018
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        mov    ebx, dword ptr [esi + 4]
        cmp    eax, 1
        jne    loc_4_0360
        mov    byte ptr [ebx + 0x79], 0
        jmp    loc_4_02F3
loc_4_0360:
        inc    byte ptr [ebx + 0x78]
        mov    word ptr [esi + 0x6a], 0x23
        push   esi
        call   sub_1_0484
        add    esp, 4
        ret    
loc_4_0373:
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_1_0027
        cmp    word ptr [esi + 0x92], 0x440d
        jne    loc_1_0027
        mov    eax, dword ptr [esi + 0x94]
        cmp    ax, 0x848
        je     loc_4_03D0
        cmp    ax, 0x849
        jne    loc_1_0027
        mov    dword ptr [esi + 0xa0], 0
        push   ebx
        mov    ebx, dword ptr [esi + 8]
        xor    ecx, ecx
        mov    ax, 8
        call   sub_4_0731
        mov    word ptr [esi + 0x6a], ax
        pop    ebx
        call   sub_4_041D
        mov    dword ptr [esi + 0xa0], eax
        jmp    loc_1_0152
loc_4_03D0:
        mov    word ptr [esi + 0x6a], 0x16
        mov    dword ptr [esi + 0xa0], 1
        mov    ecx, dword ptr [esi + 0x98]
        movzx  eax, byte ptr [ecx]
        cmp    al, 2
        ja     loc_1_0152
        je     loc_4_040C
        add    eax, 9
        xor    ecx, ecx
        call   sub_4_0731
        mov    word ptr [esi + 0x6a], ax
        call   sub_4_041D
        mov    dword ptr [esi + 0xa0], eax
loc_4_040C:
        mov    ecx, dword ptr [esi + 0x98]
        mov    al, byte ptr [ebx + 0x79]
        mov    byte ptr [ecx + 1], al
        jmp    loc_1_0152
sub_4_041D:
        cmp    ax, 0x10
        jb     loc_4_042D
        movzx  ecx, ax
        VxDCall  BLOCKDEV.Service_000Fh          ; cd200f001000
        ret    
loc_4_042D:
        mov    al, 0
        ret    
loc_4_0430:
        mov    esi, 0x430                    ; -> loc_4_0430
        mov    edi, 0x442                    ; -> loc_4_0442
        call   sub_4_087E
        jae    loc_4_0442
        ret    
loc_4_0442:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0xa8]
        xor    edx, edx
        xchg   dword ptr [ebx + 0x18], edx
        sti    
loc_4_0453:
        pushal 
        test   byte ptr [ebx + 0x1e], 4
        je     loc_4_0474
        xor    ecx, ecx
        mov    ax, 6
        call   sub_4_0731
        cmp    eax, 0x10
        jae    loc_4_0474
        cmp    edx, edi
        jne    loc_4_0474
        test   byte ptr [ebx + 0x22], 0x10
        je     loc_4_04B5
loc_4_0474:
        sub    esp, 0x174
        mov    ebx, esp
        push   edi
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 0x1d
        rep stosd dword ptr es:[edi], eax
        pop    edi
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x28], eax
        mov    dword ptr [ebx + 4], 2
        lea    eax, [ebx + 0x74]
        mov    dword ptr [ebx + 0x14], eax
        push   ebx
        call   dword ptr [edi + 0x18]
        cli    
        add    esp, 4
        cmp    word ptr [ebx + 0x1a], 0
        jne    loc_4_0549
        add    esp, 0x174
loc_4_04B5:
        popal  
        and    dword ptr [ebx + 0x20], 0xffefefff
        and    byte ptr [edi + 4], 0xfb
        mov    dword ptr [ebx + 0x18], edi
        mov    edx, dword ptr [esi + 4]
        mov    byte ptr [edx + 0xb7], 0
        dec    byte ptr [edx + 0x76]
        sti    
        mov    ax, 0x23
        xor    ecx, ecx
        call   sub_4_071B
        push   edi
        push   edx
        push   word ptr [esi + 0xe]
        pop    word ptr [esi + 0xc]
        call   sub_1_02B0
        pop    ebx
        pop    edi
        cmp    dword ptr [ebx + 0xaf], 0
        je     loc_4_071A
        cli    
        xor    esi, esi
        lea    edx, [ebx + 0xaf]
loc_4_0504:
        cmp    dword ptr [edx], 0
        je     loc_4_0528
        mov    ebx, dword ptr [edx]
        cmp    dword ptr [ebx + 0xa8], edi
        jne    loc_4_0521
        mov    eax, dword ptr [ebx + 0x64]
        mov    dword ptr [edx], eax
        mov    eax, esi
        mov    esi, ebx
        mov    dword ptr [ebx + 0x64], eax
        jmp    loc_4_0504
loc_4_0521:
        mov    edx, dword ptr [edx]
        lea    edx, [edx + 0x64]
        jmp    loc_4_0504
loc_4_0528:
        sti    
loc_4_0529:
        or     esi, esi
        je     loc_4_071A
        push   word ptr [esi + 0xe]
        pop    word ptr [esi + 0xc]
        push   esi
        mov    eax, dword ptr [esi + 0x64]
        mov    esi, eax
        call   sub_1_000C
        add    esp, 4
        jmp    loc_4_0529
loc_4_0549:
        mov    eax, dword ptr [esi + 8]
        or     dword ptr [eax + 0x20], 0x101000
        sti    
        test   byte ptr [esi + 0x6e], 2
        jne    loc_4_06DA
        test   byte ptr [eax + 0x22], 4
        je     loc_4_0579
        test   byte ptr [eax + 0x1e], 2
        je     loc_4_0579
        push   ebx
        mov    ebx, eax
        xor    ecx, ecx
        mov    ax, 8
        call   sub_4_071B
        pop    ebx
loc_4_0579:
        mov    eax, dword ptr [0x5f0]        ; -> loc_1_05F0
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cld    
        push   esi
        lea    edi, [0x11]                   ; -> obj5:0x0011
        mov    esi, dword ptr [ebx + 0x14]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    al, al
        lea    edi, [0x11]                   ; -> obj5:0x0011
        mov    ecx, 0xb
        repne scasb al, byte ptr es:[edi]
        jne    loc_4_05B2
        dec    edi
        inc    ecx
        mov    al, 0x20
        rep stosb byte ptr es:[edi], al
loc_4_05B2:
        xor    ecx, ecx
        mov    edx, dword ptr [ebx + 0x28]
loc_4_05B7:
        rol    edx, 4
        mov    al, dl
        and    al, 0xf
        cmp    al, 9
        ja     loc_4_05DF
        add    al, 0x30
loc_4_05C4:
        mov    byte ptr [ecx + 7], al        ; -> obj5:0x0007
        inc    ecx
        cmp    ecx, 4
        jne    loc_4_05D8
        mov    byte ptr [ecx + 7], 0x2d      ; -> obj5:0x0007
        inc    ecx
loc_4_05D8:
        cmp    ecx, 9
        je     loc_4_05E3
        jmp    loc_4_05B7
loc_4_05DF:
        add    al, 0x37
        jmp    loc_4_05C4
loc_4_05E3:
        pop    edx
        mov    al, byte ptr [edx + 0xb1]
        add    al, 0x41
        mov    byte ptr [4], al              ; -> obj5:0x0004
        mov    eax, dword ptr [edx + 4]
        cmp    byte ptr [eax + 0x40], 5
        je     loc_4_064A
        lea    edi, [0x1d]                   ; -> obj5:0x001D
        lea    ecx, [4]                      ; -> obj5:0x0004
        push   ecx
        push   1
        push   0                             ; -> loc_2_0000
        push   edi
        VMMCall  VMM._LocalizeSprintf            ; cd2058010100
        lea    esp, [esp + 0x10]
        xor    eax, eax
        pushfd 
        cld    
        repne scasb al, byte ptr es:[edi]
        popfd  
        push   edi
        lea    ecx, [0x11]                   ; -> obj5:0x0011
        lea    edx, [7]                      ; -> obj5:0x0007
        lea    eax, [4]                      ; -> obj5:0x0004
        push   eax
        push   edx
        push   ecx
        push   2
        push   0                             ; -> loc_2_0000
        push   edi
        VMMCall  VMM._LocalizeSprintf            ; cd2058010100
        lea    esp, [esp + 0x18]
        jmp    loc_4_0698
loc_4_064A:
        lea    edi, [0x1d]                   ; -> obj5:0x001D
        lea    ecx, [4]                      ; -> obj5:0x0004
        push   ecx
        push   3
        push   0                             ; -> loc_2_0000
        push   edi
        VMMCall  VMM._LocalizeSprintf            ; cd2058010100
        lea    esp, [esp + 0x10]
        xor    eax, eax
        pushfd 
        cld    
        repne scasb al, byte ptr es:[edi]
        popfd  
        push   edi
        lea    ecx, [0x11]                   ; -> obj5:0x0011
        lea    edx, [7]                      ; -> obj5:0x0007
        lea    eax, [4]                      ; -> obj5:0x0004
        push   eax
        push   edx
        push   ecx
        push   4
        push   0                             ; -> loc_2_0000
        push   edi
        VMMCall  VMM._LocalizeSprintf            ; cd2058010100
        lea    esp, [esp + 0x18]
loc_4_0698:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        pop    ecx
        lea    edi, [0x1d]                   ; -> obj5:0x001D
        mov    eax, 0x40001011
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, 0x174
        mov    ecx, eax
        mov    eax, dword ptr [0x5f0]        ; -> loc_1_05F0
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        cmp    ecx, 1
        popal  
        jne    loc_4_06CE
        jmp    loc_4_0453
loc_4_06CE:
        or     dword ptr [edi + 4], 0x21
        mov    word ptr [esi + 0x6a], 0x14
        jmp    loc_4_06E7
loc_4_06DA:
        add    esp, 0x174
        popal  
        mov    word ptr [esi + 0x6a], 0x2a
loc_4_06E7:
        and    dword ptr [edi + 4], 0xfffffffb
        or     dword ptr [ebx + 0x20], 0x101000
        mov    edx, dword ptr [ebx]
        mov    byte ptr [edx + 0x79], 0
        mov    ax, 0x24
        xor    ecx, ecx
        call   sub_4_071B
        dec    byte ptr [edx + 0x76]
        push   esi
        call   sub_1_0484
        add    esp, 4
        mov    ax, 0x23
        xor    ecx, ecx
        call   sub_4_071B
loc_4_071A:
        ret    
sub_4_071B:
        pushal 
        push   0xc                           ; -> sub_1_000C
        push   ecx
        push   0
        jmp    loc_4_0737
sub_4_0726:
        pushal 
        push   0xc                           ; -> sub_1_000C
        push   ecx
        push   1
        jmp    loc_4_0737
sub_4_0731:
        pushal 
        push   0
        push   ecx
        push   0
loc_4_0737:
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    ecx, 0x68
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        or     edi, edi
        je     loc_4_0835
        add    edi, dword ptr [ebx + 4]
        xor    edx, edx
        mov    dword ptr [edi + 0xa8], edx
        mov    dword ptr [edi + 0x7c], edx
        mov    dword ptr [edi + 0x80], edx
        mov    dword ptr [edi + 0x78], edx
        mov    dword ptr [edi + 0x64], edx
        mov    word ptr [edi + 0x68], ax
        mov    dword ptr [edi + 0x6c], 0x10d09
        pop    edx
        or     edx, edx
        je     loc_4_07A8
        and    dword ptr [edi + 0x6c], 0xfffff6ff
        mov    dword ptr [edi + 0x70], 0x83e ; -> loc_4_083E
        mov    dword ptr [edi + 0xa4], edi
loc_4_07A8:
        pop    edx
        or     edx, edx
        je     loc_4_07E2
        mov    dword ptr [edi + 0x7c], 1
        mov    dword ptr [edi + 0x74], esi
        mov    dword ptr [edi + 0xac], edi
        add    dword ptr [edi + 0xac], 0xbc
        mov    dword ptr [edi + 0x80], edx
        mov    dword ptr [edi + 8], ebx
        push   edi
        call   sub_1_05D0
        add    esp, 4
        or     eax, eax
        je     loc_4_07E2
        or     dword ptr [edi + 0x6c], 0x40
loc_4_07E2:
        mov    al, byte ptr [ebx + 0x14]
        test   byte ptr [ebx + 0x1d], 0x20
        je     loc_4_07F9
        mov    al, 0
        or     edx, edx
        jne    loc_4_07F9
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0xae]
loc_4_07F9:
        mov    byte ptr [edi + 0xb1], al
        pop    edx
        mov    esi, dword ptr [edi + 0x6c]
        call   sub_1_05C8
        test   esi, 0x100
        je     loc_4_0833
loc_4_0811:
        movzx  eax, word ptr [edi + 0x6a]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        add    esp, 0xc
        mov    dword ptr [esp + 0x1c], eax
loc_4_0833:
        popal  
        ret    
loc_4_0835:
        add    esp, 0xc
        mov    ax, 0x1d
        jmp    loc_4_0833
loc_4_083E:
        pushal 
        mov    edi, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [edi + 8]
        jmp    loc_4_0811
loc_4_0848:
        push   ebx
        sub    esp, 0xc
        movzx  ecx, byte ptr [ebx + 0x14]
        test   byte ptr [ebx + 0x1d], 0x20
        je     loc_4_0858
        xor    ecx, ecx
loc_4_0858:
        or     ecx, 0x40000000
        mov    dword ptr [esp + 8], ecx
        mov    word ptr [esp], 7
        push   esp
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        pop    esp
        mov    ebx, dword ptr [esp + 4]
        add    esp, 0xc
        call   sub_4_0731
        pop    ebx
        ret    
sub_4_087E:
        mov    ecx, dword ptr [edx + 8]
        test   byte ptr [ecx + 0x21], 0x20
        je     loc_4_08DC
        mov    dword ptr [edx + 0x60], esi
        mov    dword ptr [edx + 0x20], edi
        mov    esi, 0x8b1                    ; -> loc_4_08B1
        xor    ebx, ebx
        mov    ecx, 0x820
        mov    eax, 0x10
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        stc    
        ret    
loc_4_08A6:
        mov    eax, 0x64
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c800100
loc_4_08B1:
        mov    ecx, dword ptr [0x5d8]        ; -> loc_1_05D8
        mov    eax, dword ptr [ecx + 0x3c]
        mov    esi, dword ptr [edx + 0x60]
        push   eax
        push   ecx
        call   sub_4_08FD
        pop    ecx
        pop    eax
        jb     loc_4_08A6
        cmp    dword ptr [eax + 0x107], 1
        je     loc_4_08A6
        mov    eax, dword ptr [edx + 0x20]
        test   byte ptr [ecx + 0x28], 0x80
        jne    loc_4_08A6
        jmp    eax
loc_4_08DC:
        ret    
        ; ---- dispatch table (8 entries) ----
        dd  loc_4_0933
        dd  loc_4_0933
        dd  loc_4_0939
        dd  loc_4_0939
        dd  loc_4_0935
        dd  loc_4_0935
        dd  loc_4_0933
        dd  loc_4_0935
sub_4_08FD:
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [0x5f4]        ; -> loc_1_05F4
        xor    ecx, ecx
        cmp    byte ptr [eax + 0x12b9], 0
        je     loc_4_0914
        or     ecx, 1
loc_4_0914:
        cmp    byte ptr [eax + 0x12b8], 0
        je     loc_4_0920
        or     ecx, 2
loc_4_0920:
        cmp    byte ptr [eax + 0x321], 0
        je     loc_4_092C
        or     ecx, 4
loc_4_092C:
        jmp    dword ptr [ecx*4 + 0x8dd]     ; -> obj4:0x08DD
loc_4_0933:
        clc    
        ret    
loc_4_0935:
        xor    ecx, ecx
        stc    
        ret    
loc_4_0939:
        or     ecx, 0xffffffff
        stc    
        ret    
        db  000h, 000h
loc_4_0940:
        mov    esi, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [esi + 0x20]
        test   eax, 4
        je     loc_4_09BB
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_4_096A
        test   eax, 0x4000
        je     loc_4_09BB
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_096A
        cmp    byte ptr [esi + 0x40], 5
        je     loc_4_096A
        jmp    loc_4_09BB
loc_4_0968:
        jmp    loc_4_09BB
loc_4_096A:
        test   dword ptr [esi + 0x1c], 0x23
        jne    loc_4_0968
        inc    byte ptr [0]                  ; -> obj5:0x0000
        push   ebp
        sub    esp, 0x18
        mov    ebp, esp
        mov    dword ptr [ebp + 4], esi
        mov    dword ptr [ebp + 8], 0xc      ; -> sub_1_000C
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 0x16], al
        xor    eax, eax
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [esi + 0x1c]
        and    eax, 0xffffffe3
        mov    dword ptr [ebp + 0x12], eax
        mov    word ptr [ebp], 5
        push   ebp
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        add    esp, 0x1c
        pop    ebp
        mov    esi, dword ptr [esi]
        or     dword ptr [esi + 0x20], 0x80000
loc_4_09BB:
        ret    
loc_4_09BC:
        cmp    byte ptr [0], 0               ; -> obj5:0x0000
        jne    loc_4_09CB
        mov    word ptr [ebx + 2], 0xffff
loc_4_09CB:
        ret    
loc_4_09CC:
        mov    edx, dword ptr [0x5d8]        ; -> loc_1_05D8
        mov    eax, dword ptr [edx + 0x30]
loc_4_09D5:
        or     eax, eax
        je     loc_4_0A14
        test   byte ptr [eax + 0x21], 0x80
        je     loc_4_0A0F
        cmp    byte ptr [eax + 0x40], 0
        je     loc_4_09F1
        cmp    byte ptr [eax + 0x40], 0xa
        je     loc_4_09F1
        cmp    byte ptr [eax + 0x40], 5
        jne    loc_4_0A0F
loc_4_09F1:
        cmp    byte ptr [eax + 0x79], 0
        jne    loc_4_0A00
        cmp    dword ptr [eax + 0xb3], 0
        je     loc_4_0A0F
loc_4_0A00:
        push   eax
        mov    ebx, eax
        mov    ax, 0xa
        xor    ecx, ecx
        call   sub_4_071B
        pop    eax
loc_4_0A0F:
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_4_09D5
loc_4_0A14:
        ret    
        db  000h, 000h, 000h
loc_4_0A18:
        mov    esi, 0xa18                    ; -> loc_4_0A18
        mov    edi, 0xa2a                    ; -> loc_4_0A2A
        call   sub_4_087E
        jae    loc_4_0A2A
        ret    
loc_4_0A2A:
        push   esi
        push   edi
        push   ebx
        mov    esi, edx
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0xa8]
        cmp    word ptr [esi + 0x6a], 0x19
        je     loc_4_0A51
        cmp    word ptr [esi + 0x6a], 0x18
        je     loc_4_0A51
        cmp    word ptr [esi + 0x6a], 0x20
        jne    loc_4_0B09
loc_4_0A51:
        test   byte ptr [ebx + 0x22], 8
        jne    loc_4_0A57
loc_4_0A57:
        mov    edx, dword ptr [0x5d8]        ; -> loc_1_05D8
        mov    eax, ebx
        mov    cl, 1
        call   sub_1_041C
        cmp    byte ptr [ebx + 0x79], 0
        je     loc_4_0A7D
        test   byte ptr [ebx + 0x1e], 1
        je     loc_4_0A7D
        mov    ax, 9
        xor    ecx, ecx
        call   sub_4_071B
loc_4_0A7D:
        btr    dword ptr [esi + 0x6c], 0x14
        jae    loc_4_0A8D
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 0x28]
        sub    dword ptr [esi + 0x74], eax
loc_4_0A8D:
        test   byte ptr [esi + 0x6c], 8
        jne    loc_4_0AD0
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_4_0B09
        test   byte ptr [edi + 4], 0x20
        je     loc_4_0AAA
        mov    word ptr [esi + 0x6a], 0x11
        jmp    loc_4_0B09
loc_4_0AAA:
        call   sub_1_0550
        cli    
        test   byte ptr [edi + 4], 4
        je     loc_4_0B23
        dec    byte ptr [ebx + 0x78]
        dec    byte ptr [ebx + 0x76]
        mov    eax, dword ptr [ebx + 0xaf]
        mov    dword ptr [esi + 0x64], eax
        mov    dword ptr [ebx + 0xaf], esi
        sti    
loc_4_0ACC:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_4_0AD0:
        cmp    word ptr [esi + 0x68], 6
        je     loc_4_0B09
        cmp    word ptr [esi + 0x6a], 0x19
        jne    loc_4_0B09
        bts    dword ptr [esi + 0x6c], 0x17
        jb     loc_4_0B09
        dec    byte ptr [ebx + 0x76]
        dec    byte ptr [ebx + 0x78]
        mov    ebx, dword ptr [esi + 8]
        xor    ecx, ecx
        mov    ax, 6
        call   sub_4_0731
        call   sub_1_0550
        push   esi
        call   sub_1_000C
        add    esp, 4
        jmp    loc_4_0ACC
loc_4_0B09:
        dec    byte ptr [ebx + 0x76]
        mov    edx, dword ptr [esi + 8]
        xchg   ebx, edx
        mov    ax, 0x23
        xor    ecx, ecx
        call   sub_4_071B
        mov    ebx, edx
        jmp    loc_1_04AB
loc_4_0B23:
        or     dword ptr [edi + 4], 4
        sti    
        mov    ebx, dword ptr [esi + 8]
        call   sub_1_036C
        jmp    loc_4_0ACC
sub_4_0B32:
        lea    edx, [ebx + 0xaf]
        cmp    dword ptr [edx], 0
        je     loc_4_0B62
loc_4_0B3D:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi + 0x64], 0
        je     loc_4_0B4A
        lea    edx, [esi + 0x64]
        jmp    loc_4_0B3D
loc_4_0B4A:
        mov    dword ptr [edx], 0
        sti    
        push   word ptr [esi + 0xe]
        pop    word ptr [esi + 0xc]
        push   esi
        call   sub_1_000C
        add    esp, 4
loc_4_0B62:
        sti    
        ret    
loc_4_0B64:
        mov    edx, dword ptr [esp + 4]
        sub    esp, 0xc
        mov    ecx, esp
        mov    word ptr [ecx], 0xd
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], 1
        push   ecx
        call   dword ptr [0x5bc]             ; -> obj1:0x05BC
        add    esp, 0x10
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x11d  RW-
        db  000h, 000h, 000h, 000h, 041h, 03Ah, 026h, 076h, 06Fh, 06Ch, 074h, 072h, 061h, 063h, 06Bh, 020h
        db  056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 020h
        db  028h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 00Ch
        db  076h, 06Fh, 06Ch, 074h, 072h, 061h, 063h, 06Bh, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h, 0FFh
        db  010h, 000h, 0FFh, 001h, 000h, 030h, 000h, 0ECh, 001h, 000h, 000h, 0ECh, 001h, 034h, 000h, 056h
        db  053h, 05Fh, 056h, 045h, 052h, 053h, 049h, 04Fh, 04Eh, 05Fh, 049h, 04Eh, 046h, 04Fh, 000h, 0BDh
        db  004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h, 000h, 000h, 004h, 000h, 057h, 004h, 000h, 000h, 000h
        db  000h, 004h, 000h, 057h, 004h, 000h, 000h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 001h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 07Eh, 001h, 000h, 000h, 053h, 074h, 072h, 069h, 06Eh, 067h, 046h, 069h, 06Ch
        db  065h, 049h, 06Eh, 066h, 06Fh, 000h, 000h, 06Ah, 001h, 000h, 000h, 030h, 034h, 030h, 039h, 030h
        db  034h, 045h, 034h, 000h, 000h, 000h, 000h, 026h, 000h, 016h, 000h, 043h, 06Fh, 06Dh, 070h, 061h
        db  06Eh, 079h, 04Eh, 061h, 06Dh, 065h, 000h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 06Fh, 066h, 074h
        db  020h, 043h, 06Fh, 072h, 070h, 06Fh, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h, 03Bh
        db  000h, 027h, 000h, 046h, 069h, 06Ch, 065h, 044h, 065h, 073h, 063h, 072h, 069h, 070h, 074h, 069h
        db  06Fh, 06Eh, 000h, 076h, 06Fh, 06Ch, 074h, 072h, 061h, 063h, 06Bh, 020h, 056h, 069h, 072h, 074h
        db  075h, 061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 020h, 028h, 056h, 065h, 072h
        db  073h, 069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 01Ah, 000h
VxD__LDATA ENDS
