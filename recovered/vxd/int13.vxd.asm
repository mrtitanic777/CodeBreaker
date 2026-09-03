; INT13 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 906 instructions recovered by recursive descent

Declare_Virtual_Device INT13, 3, 10, INT13_Control, 00020h, Undefined_Init_Order
;   DDK version 0400h   init_order 042000000h   flags 00000h
;   service table: obj1:0x96c, 5 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xac4  R-X
sub_1_0000:
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
sub_1_0014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_001C:
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
        nop    
        nop    
INT13_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_0051
        cmp    eax, 2
        je     loc_2_0053
        clc    
        ret    
Int13_Get_Version:
        mov    eax, 0x30a
        clc    
        ret    
Int13_Hooking_BIOS_Int:
        ret    
Int13_Unhooking_BIOS_Int:
        ret    
sub_1_005C:
        pushal 
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        and    byte ptr [eax + 0x2a], 0xdf
        mov    ecx, dword ptr [0x78]
        mov    dword ptr [0xa2b], ecx        ; -> loc_1_0A2B
        mov    eax, ecx
        and    ecx, 0xffff
        and    eax, 0xffff0000
        shr    eax, 0xc
        or     eax, ecx
        push   0
        push   1
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        movzx  eax, byte ptr [eax + 3]
        mov    byte ptr [0xa2f], al          ; -> loc_1_0A2F
        popal  
        ret    
        db  000h, 000h, 000h
Int13_Translate_VM_Int:
        xor    edx, edx
        cmp    byte ptr [ebp + 0x1d], dl
        jne    loc_1_00B0
        cmp    byte ptr [ebp + 0x14], 0x80
        jb     loc_1_00B0
        mov    byte ptr [ebp + 0x14], dl
loc_1_00B0:
        movzx  edx, byte ptr [ebp + 0x14]
        test   dl, 0x78
        jne    loc_1_0160
        rol    dl, 1
        mov    edi, dword ptr [edx*4 + 0x9ea] ; -> obj1:0x09EA
        ror    dl, 1
        or     edi, edi
        je     loc_1_0160
        test   dword ptr [edi + 0x20], 0x2000
        jne    loc_1_0160
        cmp    byte ptr [ebp + 0x1d], 0x18
        je     loc_1_00E9
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
loc_1_00E9:
        call   sub_1_0708
        mov    dword ptr [esi + 0xc], 0x678  ; -> loc_1_0678
        mov    edx, dword ptr [0x1c]         ; -> loc_1_001C
        test   byte ptr [edx + 0x28], 1
        je     loc_1_0107
        cmp    byte ptr [edi + 0x40], 0xa
        je     loc_1_0162
loc_1_0107:
        movzx  edx, byte ptr [ebp + 0x14]
        movzx  ecx, byte ptr [ebp + 0x1d]
        cmp    cl, 0x1a
        ja     loc_1_011B
        jmp    dword ptr [ecx*4 + 0xa30]     ; -> obj1:0x0A30
loc_1_011B:
        sub    cl, 0x41
        cmp    cl, 8
        ja     loc_1_012A
        jmp    dword ptr [ecx*4 + 0xa9c]     ; -> obj1:0x0A9C
loc_1_012A:
        mov    byte ptr [ebp + 0x1d], 1
        jmp    loc_1_0154
loc_1_0130:
        mov    byte ptr [ebp + 0x1d], 0
        and    byte ptr [ebp + 0x2c], 0xfe
loc_1_0138:
        movzx  edx, byte ptr [edi + 0x15]
        rol    dl, 1
        mov    al, byte ptr [ebp + 0x1d]
        mov    byte ptr [edx + 0x9d0], al    ; -> obj1:0x09D0
loc_1_0147:
        call   sub_1_076A
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        clc    
        ret    
loc_1_0154:
        or     byte ptr [ebp + 0x2c], 1
        jmp    loc_1_0138
loc_1_015A:
        and    byte ptr [ebp + 0x2c], 0xfe
        jmp    loc_1_0147
loc_1_0160:
        stc    
        ret    
loc_1_0162:
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        test   byte ptr [eax + 0x2a], 0x20
        jne    loc_1_0107
        mov    edx, dword ptr [0xa2b]        ; -> loc_1_0A2B
        cmp    edx, dword ptr [0x78]
        jne    loc_1_039A
        pushal 
        mov    eax, edx
        and    edx, 0xffff
        and    eax, 0xffff0000
        shr    eax, 0xc
        or     eax, edx
        push   0
        push   1
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        movzx  edx, byte ptr [eax + 3]
        cmp    dl, byte ptr [0xa2f]          ; -> loc_1_0A2F
        popal  
        jne    loc_1_039A
        jmp    loc_1_0107
loc_1_01B6:
        or     dword ptr [esi + 8], 0x100
        mov    word ptr [esi + 4], 0x11
        or     word ptr [esi + 8], 0x4008
        mov    byte ptr [esi + 0x4d], 0
        add    edi, 0xc3
        push   esi
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    esi
        sub    edi, 0xc3
        jmp    loc_1_0130
loc_1_01E6:
        movzx  edx, byte ptr [ebp + 0x14]
        rol    dl, 1
        mov    al, byte ptr [edx + 0x9d0]    ; -> obj1:0x09D0
        mov    byte ptr [ebp + 0x1c], al
        jmp    loc_1_0130
loc_1_01FA:
        mov    word ptr [esi + 4], 1
loc_1_0200:
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        call   sub_1_0611
        cmp    byte ptr [edi + 0x40], 0xa
        je     loc_1_0339
loc_1_0215:
        movzx  eax, word ptr [ebp + 0x38]
        movzx  edx, word ptr [ebp + 0x10]
loc_1_021D:
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        add    eax, edx
        mov    dword ptr [esi + 0x1c], eax
        shl    ecx, 9
        dec    ecx
        mov    edx, eax
        and    edx, 0xfff
        add    ecx, edx
        shr    ecx, 0xc
        inc    ecx
        shr    eax, 0xc
        xor    edx, edx
        push   edx
        cmp    word ptr [esi + 4], 1
        je     loc_1_024C
        mov    edx, 0x80000000
loc_1_024C:
        push   ecx
        push   eax
        push   edx
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        push   eax
        mov    eax, esi
        sub    eax, 0x64
        push   eax
        call   sub_1_0014
        add    esp, 4
        xchg   dword ptr [esp], eax
        test   eax, eax
        je     loc_1_0286
        pop    eax
        or     eax, eax
        jne    loc_1_028B
        call   sub_1_064B
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        jmp    loc_1_029D
loc_1_0286:
        add    esp, 0x10
        jmp    loc_1_0294
loc_1_028B:
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_1_0294:
        or     dword ptr [esi + 8], 0x40
        call   sub_1_064B
loc_1_029D:
        xor    al, al
        mov    cx, word ptr [esi + 6]
        cmp    cx, 2
        jne    loc_1_02AD
        mov    al, 0x11
        jmp    loc_1_02EE
loc_1_02AD:
        cmp    cx, 0x10
        jb     loc_1_02EE
        je     loc_1_02EC
        cmp    cx, 0x18
        je     loc_1_02CB
        cmp    cx, 0x19
        je     loc_1_02CF
        cmp    cx, 0x1f
        je     loc_1_02E8
        mov    al, 0xa
        jmp    loc_1_02EE
loc_1_02CB:
        mov    al, 0x80
        jmp    loc_1_02D1
loc_1_02CF:
        mov    al, 6
loc_1_02D1:
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_1_02EE
        test   byte ptr [edi + 0x20], 4
        je     loc_1_02EE
        push   eax
        push   edx
        call   sub_1_0684
        pop    edx
        pop    eax
        jmp    loc_1_02EE
loc_1_02E8:
        mov    al, 3
        jmp    loc_1_02EE
loc_1_02EC:
        mov    al, 4
loc_1_02EE:
        test   al, al
        jne    loc_1_0322
        test   byte ptr [ebp + 0x14], 0x80
        je     loc_1_030C
        cmp    word ptr [esi + 4], 0
        jne    loc_1_0130
        mov    byte ptr [ebp + 0x1c], 0x50
        jmp    loc_1_0130
loc_1_030C:
        cmp    word ptr [esi + 4], 2
        ja     loc_1_0130
        mov    eax, dword ptr [esi + 0x18]
        mov    byte ptr [ebp + 0x1c], al
        jmp    loc_1_0130
loc_1_0322:
        mov    byte ptr [ebp + 0x1d], al
        cmp    word ptr [esi + 4], 0
        jne    loc_1_0154
        mov    byte ptr [ebp + 0x1c], 0
        jmp    loc_1_0154
loc_1_0339:
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        test   byte ptr [eax + 0x2a], 0x20
        jne    loc_1_0215
        mov    edx, dword ptr [0xa2b]        ; -> loc_1_0A2B
        cmp    edx, dword ptr [0x78]
        jne    loc_1_039A
        pushal 
        mov    eax, edx
        and    edx, 0xffff
        and    eax, 0xffff0000
        shr    eax, 0xc
        or     eax, edx
        push   0
        push   1
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        movzx  edx, byte ptr [eax + 3]
        cmp    dl, byte ptr [0xa2f]          ; -> loc_1_0A2F
        popal  
        jne    loc_1_039A
        movzx  edx, word ptr [ebp + 0x18]
        and    edx, 0x3f
        add    edx, ecx
        dec    edx
        cmp    edx, dword ptr [edi + 0xe7]
        jbe    loc_1_0215
loc_1_039A:
        call   sub_1_076A
        mov    edi, dword ptr [0x9ea]        ; -> obj1:0x09EA
        or     edi, edi
        je     loc_1_03AD
        or     byte ptr [edi + 0x24], 8
loc_1_03AD:
        mov    edi, dword ptr [0x9f2]        ; -> obj1:0x09F2
        or     edi, edi
        je     loc_1_03BB
        or     byte ptr [edi + 0x24], 8
loc_1_03BB:
        xor    eax, eax
        mov    ecx, eax
        inc    al
        VxDCall  VFBACKUP.VFBACKUP_Lock_NEC      ; cd2001003600
        pushfd 
        mov    edi, dword ptr [0x9ea]        ; -> obj1:0x09EA
        or     edi, edi
        je     loc_1_03D6
        and    byte ptr [edi + 0x24], 0xf7
loc_1_03D6:
        mov    edi, dword ptr [0x9f2]        ; -> obj1:0x09F2
        or     edi, edi
        je     loc_1_03E4
        and    byte ptr [edi + 0x24], 0xf7
loc_1_03E4:
        popfd  
        jb     loc_1_03FC
        xor    eax, eax
        mov    esi, 0x419                    ; -> loc_1_0419
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        stc    
        ret    
loc_1_03FC:
        mov    byte ptr [ebp + 0x1d], 8
        or     byte ptr [ebp + 0x2c], 1
        movzx  edx, byte ptr [ebp + 0x14]
        mov    edi, dword ptr [edx*4 + 0x9ea] ; -> obj1:0x09EA
        call   sub_1_0708
        jmp    loc_1_0138
loc_1_0419:
        VxDCall  VFBACKUP.VFBACKUP_UnLock_NEC    ; cd2002003600
        ret    
loc_1_0420:
        mov    word ptr [esi + 4], 2
        call   sub_1_0611
loc_1_042B:
        call   sub_1_064B
        cmp    word ptr [esi + 6], 0x16
        je     loc_1_0130
        jmp    loc_1_029D
loc_1_0440:
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        movzx  edx, word ptr [ebp + 0x10]
        add    eax, edx
        mov    dword ptr [esi + 0x48], eax
        movzx  eax, byte ptr [ebp + 0x1c]
        mov    dword ptr [esi - 4], eax
        mov    al, byte ptr [ebp + 0x15]
        mov    byte ptr [esi - 8], al
        movzx  eax, byte ptr [ebp + 0x19]
        mov    word ptr [esi - 6], ax
        mov    word ptr [esi + 4], 0xe
        mov    dword ptr [esi + 0x18], 0
        call   sub_1_064B
        jmp    loc_1_029D
loc_1_047F:
        adc    byte ptr [esi], 0x40
        or     esp, dword ptr [eax - 0x2ff69ffb]
        db  002h
loc_1_0489:
        mov    al, byte ptr [0x9e8]          ; -> obj1:0x09E8
        mov    ah, byte ptr [0x9e9]          ; -> loc_1_09E9
        sub    al, ah
        push   esi
        lea    esi, [edi + 0xd3]
        test   dl, 0x80
        jne    loc_1_04F7
        mov    ecx, dword ptr [0x1c]         ; -> loc_1_001C
        mov    ecx, dword ptr [ecx + 0x2c]
        or     ecx, ecx
        je     loc_1_04D4
loc_1_04AF:
        cmp    byte ptr [ecx], 1
        jne    loc_1_04D4
        cmp    byte ptr [ecx + 5], dl
        je     loc_1_04C4
        cmp    dword ptr [ecx + 1], 0
        je     loc_1_04D4
        add    ecx, dword ptr [ecx + 1]
        jmp    loc_1_04AF
loc_1_04C4:
        push   ecx
        mov    ecx, dword ptr [ecx + 0x19]
        mov    word ptr [ebp], cx
        ror    ecx, 0x10
        mov    word ptr [ebp + 0x38], cx
        pop    ecx
loc_1_04D4:
        test   byte ptr [edi + 0x24], 0x40
        jne    loc_1_0518
        lea    esi, [edi + 0x54]
        push   eax
        mov    eax, dword ptr [esi]
        push   edi
        lea    edi, [0x47f]                  ; -> loc_1_047F
        mov    ecx, 6
        repne scasw ax, word ptr es:[edi]
        pop    edi
        mov    word ptr [ebp + 0x10], cx
        pop    eax
        xchg   ah, al
loc_1_04F7:
        mov    ah, byte ptr [esi + 0xc]
        dec    ah
        mov    word ptr [ebp + 0x14], ax
        mov    ax, word ptr [esi + 0x10]
        dec    ax
        shl    ah, 6
        or     ah, byte ptr [esi + 0x14]
loc_1_050C:
        xchg   al, ah
        mov    word ptr [ebp + 0x18], ax
        pop    esi
        jmp    loc_1_0130
loc_1_0518:
        mov    al, byte ptr [ecx + 8]
        dec    al
        xchg   al, ah
        mov    word ptr [ebp + 0x14], ax
        movzx  ax, byte ptr [ecx + 0xc]
        mov    word ptr [ebp + 0x10], ax
        mov    ax, word ptr [ecx + 6]
        dec    ax
        shl    ah, 6
        or     ah, byte ptr [ecx + 0xa]
        jmp    loc_1_050C
loc_1_053A:
        cmp    byte ptr [edi + 0x40], 0xa
        je     loc_1_055C
        mov    word ptr [ebp + 0x1c], 0x300
        mov    edx, dword ptr [edi + 0xd3]
        mov    word ptr [ebp + 0x14], dx
        shr    edx, 0x10
        mov    word ptr [ebp + 0x18], dx
        jmp    loc_1_015A
loc_1_055C:
        mov    word ptr [ebp + 0x1c], 0x200
        test   dword ptr [edi + 0x20], 0x10000
        jne    loc_1_015A
        mov    word ptr [ebp + 0x1c], 0x100
        jmp    loc_1_015A
loc_1_057A:
        cmp    byte ptr [edi + 0x40], 0xa
        je     loc_1_0592
loc_1_0580:
        mov    byte ptr [ebp + 0x1d], 0
        jmp    loc_1_015A
loc_1_0589:
        test   dword ptr [edi + 0x20], 4
        je     loc_1_0580
loc_1_0592:
        or     dword ptr [esi + 8], 0x100
        mov    word ptr [esi + 4], 6
        mov    dword ptr [esi + 0x18], 0
        mov    al, byte ptr [edi + 0x15]
        mov    byte ptr [esi + 0x4d], al
        add    edi, 0xc3
        push   esi
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        add    esp, 4
        sub    edi, 0xc3
        cmp    word ptr [esi + 6], 0x10
        jb     loc_1_0580
        mov    byte ptr [ebp + 0x1d], 6
        jmp    loc_1_0154
loc_1_05D2:
        cmp    byte ptr [edi + 0x40], 0xa
        jne    loc_1_05F9
        movzx  eax, byte ptr [ebp + 0x18]
        mov    dword ptr [edi + 0xe7], eax
        mov    dl, byte ptr [ebp + 0x19]
        inc    dl
        mul    dl
        shl    ax, 1
        mov    dword ptr [edi + 0xd3], eax
        call   sub_1_076A
        stc    
        ret    
loc_1_05F9:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        mov    byte ptr [ebp + 0x1d], 1
        jmp    loc_1_0154
loc_1_0608:
        mov    byte ptr [ebp + 0x1d], 1
        jmp    loc_1_0154
sub_1_0611:
        xor    eax, eax
        mov    dword ptr [esi + 0x14], eax
        movzx  eax, word ptr [ebp + 0x18]
        mov    ecx, eax
        shr    al, 6
        xchg   ah, al
        mov    edx, dword ptr [edi + 0xdf]
        imul   eax, edx
        movzx  edx, byte ptr [ebp + 0x15]
        add    eax, edx
        mov    edx, dword ptr [edi + 0xe7]
        imul   eax, edx
        and    ecx, 0x3f
        lea    eax, [ecx + eax - 1]
        mov    dword ptr [esi + 0x10], eax
        movzx  ecx, byte ptr [ebp + 0x1c]
        mov    dword ptr [esi + 0x18], ecx
        ret    
sub_1_064B:
        push   ebx
        push   edx
        mov    al, byte ptr [edi + 0x15]
        mov    byte ptr [esi + 0x4d], al
        add    edi, 0xc3
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        sub    edi, 0xc3
        pop    edx
        pop    ebx
        mov    eax, dword ptr [0x9e0]        ; -> obj1:0x09E0
        mov    ecx, dword ptr [0x9e4]        ; -> loc_1_09E4
        VMMCall  VMM.Wait_Semaphore              ; cd2027800100
loc_1_0678:
        mov    eax, dword ptr [0x9e0]        ; -> obj1:0x09E0
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        ret    
sub_1_0684:
        mov    eax, edi
        or     dword ptr [eax + 0x20], 0x80000
        test   dword ptr [eax + 0x20], 0x4000
        jne    loc_1_0699
        mov    eax, dword ptr [eax + 0x10]
loc_1_0699:
        or     eax, eax
        je     loc_1_06CA
        or     dword ptr [eax + 0x20], 0x100000
        mov    edx, dword ptr [0x1c]         ; -> loc_1_001C
        mov    edx, dword ptr [edx + 0x40]
        cli    
loc_1_06AE:
        or     edx, edx
        je     loc_1_06C4
        cmp    dword ptr [edx + 0x28], eax
        jne    loc_1_06BF
        or     dword ptr [edx + 4], 2
        or     dword ptr [edx + 4], 1
loc_1_06BF:
        mov    edx, dword ptr [edx + 0x24]
        jmp    loc_1_06AE
loc_1_06C4:
        sti    
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_1_0699
loc_1_06CA:
        ret    
        db  000h
loc_1_06CC:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  esi, word ptr [ebx]
        cmp    esi, 3
        je     loc_3_0000
        cmp    esi, 4
        je     loc_3_005E
        cmp    esi, 2
        je     loc_1_06FE
        cmp    esi, 0x16
        je     sub_1_005C
        call   sub_1_06FF
loc_1_06FE:
        ret    
sub_1_06FF:
        mov    word ptr [ebx + 2], 0xffff
        ret    
        db  000h, 000h
sub_1_0708:
        movzx  ecx, byte ptr [edi + 0x77]
        shl    ecx, 3
        mov    eax, dword ptr [edi + 4]
        add    ecx, eax
        add    cx, 0xbc
        add    eax, 0x64
        sub    esp, 0x10
        mov    esi, esp
        mov    word ptr [esi + 4], cx
        mov    dword ptr [esi + 6], eax
        mov    byte ptr [esi + 0xe], 2
        mov    word ptr [esi], 2
        push   esi
        call   sub_1_0000
        mov    esi, dword ptr [esi + 0xa]
        add    esp, 0x14
        mov    ecx, esi
        add    esi, eax
        mov    dword ptr [esi + 0x48], esi
        add    dword ptr [esi + 0x48], 0x58
        mov    dword ptr [esi + 0x20], ecx
        mov    eax, esi
        sub    eax, 0x64
        mov    dword ptr [eax + 8], edi
        xor    eax, eax
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 8], 0x40000401
        ret    
sub_1_076A:
        mov    esi, dword ptr [esi + 0x20]
        sub    esp, 8
        mov    ecx, esp
        mov    dword ptr [ecx + 4], esi
        mov    word ptr [ecx], 4
        push   ecx
        call   sub_1_0000
        add    esp, 0xc
        ret    
        db  000h, 000h, 000h
loc_1_0788:
        mov    byte ptr [ebp + 0x1d], 1
        mov    byte ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x10], 0xaa55
        mov    word ptr [ebp + 0x18], 3
        and    byte ptr [ebp + 0x2c], 0xfe
        jmp    loc_1_0138
loc_1_07A5:
        mov    word ptr [esi + 4], 0
loc_1_07AB:
        call   sub_1_07D7
        jb     loc_1_0154
        movzx  edx, word ptr [eax + 4]
        movzx  eax, word ptr [eax + 6]
        jmp    loc_1_021D
loc_1_07C3:
        mov    word ptr [esi + 4], 1
        test   byte ptr [ebp + 0x1c], 1
        je     loc_1_07AB
        mov    word ptr [esi + 4], 4
        jmp    loc_1_07AB
sub_1_07D7:
        mov    byte ptr [ebp + 0x1d], 7
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    byte ptr [eax], 0x10
        jb     loc_1_07FD
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [esi + 0x18], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 0x10], edx
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x14], edx
loc_1_07FD:
        ret    
loc_1_07FE:
        mov    word ptr [esi + 4], 2
        call   sub_1_07D7
        jb     loc_1_0154
        cmp    cl, 6
        jne    loc_1_042B
        mov    dword ptr [esi + 0x18], 1
        jmp    loc_1_042B
loc_1_0824:
        push   word ptr [ebp + 0x1c]
        mov    dword ptr [esi + 0x34], esp
        mov    word ptr [esi + 0x2e], 0x440d
        mov    dword ptr [esi + 0x30], 0x848
        mov    word ptr [esi + 4], 0xd
        or     dword ptr [esi + 8], 0x100
        mov    al, byte ptr [edi + 0x15]
        mov    byte ptr [esi + 0x4d], al
        push   edx
        push   ebx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    ebx
        pop    edx
        pop    ax
        mov    byte ptr [ebp + 0x1c], ah
        mov    eax, dword ptr [esi + 0x3c]
        mov    byte ptr [ebp + 0x1d], al
        or     eax, eax
        jne    loc_1_0154
        jmp    loc_1_0130
loc_1_086D:
        mov    word ptr [esi + 0x2e], 0x440d
        mov    dword ptr [esi + 0x30], 0x849
        mov    word ptr [esi + 4], 0xd
        or     byte ptr [esi + 9], 1
        mov    al, byte ptr [edi + 0x15]
        mov    byte ptr [esi + 0x4d], al
        push   edx
        push   ebx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    ebx
        pop    edx
        mov    eax, dword ptr [esi + 0x3c]
        mov    byte ptr [ebp + 0x1d], al
        or     eax, eax
        jne    loc_1_0154
        jmp    loc_1_0130
loc_1_08A7:
        xchg   edx, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        mov    byte ptr [ebp + 0x1d], 7
        cmp    word ptr [edx], 0x1a
        jb     loc_1_0154
        call   sub_1_093B
        jb     loc_1_08DA
        push   edi
        push   esi
        mov    esi, eax
        lea    edi, [edx + 2]
        mov    ecx, 0x18
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        jmp    loc_1_0930
loc_1_08DA:
        xor    eax, eax
        mov    al, 0x2d
        test   byte ptr [edi + 0x20], 4
        jne    loc_1_08E6
        and    al, 0xfb
loc_1_08E6:
        test   byte ptr [edi + 0x22], 3
        je     loc_1_08EE
        or     al, 0x10
loc_1_08EE:
        test   byte ptr [ebx + 0x1e], 1
        jne    loc_1_08F6
        and    al, 0xdf
loc_1_08F6:
        test   byte ptr [edi + 0x22], 8
        lea    ecx, [edi + 0x54]
        jne    loc_1_0907
        lea    ecx, [edi + 0xd3]
        or     al, 2
loc_1_0907:
        push   edi
        mov    edi, dword ptr [ecx + 0x10]
        mov    dword ptr [edx + 4], edi
        mov    edi, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 8], edi
        mov    edi, dword ptr [ecx + 0x14]
        mov    dword ptr [edx + 0xc], edi
        mov    edi, dword ptr [ecx]
        mov    dword ptr [edx + 0x10], edi
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0x14], edi
        mov    edi, dword ptr [ecx + 8]
        mov    word ptr [edx + 0x18], di
        mov    word ptr [edx + 2], ax
loc_1_0930:
        mov    word ptr [edx], 0x1a
        pop    edi
        jmp    loc_1_0130
sub_1_093B:
        mov    ecx, dword ptr [0x1c]         ; -> loc_1_001C
        mov    ecx, dword ptr [ecx + 0x2c]
        or     ecx, ecx
        je     loc_1_0960
        mov    al, byte ptr [ebp + 0x14]
loc_1_094B:
        cmp    byte ptr [ecx], 0
        jne    loc_1_0955
        cmp    byte ptr [ecx + 5], al
        je     loc_1_0962
loc_1_0955:
        cmp    dword ptr [ecx + 1], 0
        je     loc_1_0960
        add    ecx, dword ptr [ecx + 1]
        jmp    loc_1_094B
loc_1_0960:
        stc    
        ret    
loc_1_0962:
        test   byte ptr [ecx + 0xc], 1
        je     loc_1_0960
        lea    eax, [ecx + 0xf]
        ret    
        dd  Int13_Get_Version        ; fixup
        dd  Int13_Device_Registered  ; fixup
        dd  Int13_Translate_VM_Int   ; fixup
        dd  Int13_Hooking_BIOS_Int   ; fixup
        dd  Int13_Unhooking_BIOS_Int ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00020h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'INT13   '                  ; DDB_Name
        dd  042000000h                  ; DDB_Init_Order
        dd  INT13_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x096C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_09E4:
        add    eax, 0
loc_1_09E9:
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
loc_1_0A2B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A2F:
        add    byte ptr [esi - 0x19ffffff], dh ; -> loc_1_01B6
        db  001h, 000h, 000h
        dd  loc_1_0200               ; fixup
        dd  loc_1_01FA               ; fixup
        dd  loc_1_0420               ; fixup
        dd  loc_1_0440               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0489               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0130               ; fixup
        dd  loc_1_0130               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0130               ; fixup
        dd  loc_1_0130               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_0130               ; fixup
        dd  loc_1_053A               ; fixup
        dd  loc_1_057A               ; fixup
        dd  loc_1_0608               ; fixup
        dd  loc_1_05D2               ; fixup
        dd  loc_1_0130               ; fixup
        dd  loc_1_0608               ; fixup
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_0788
        dd  loc_1_07A5
        dd  loc_1_07C3
        dd  loc_1_07FE
        dd  loc_1_0824
        dd  loc_1_086D
        dd  loc_1_07FE
        dd  loc_1_08A7
        dd  loc_1_0589
        db  090h, 090h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x11c  R-X
loc_2_0000:
        xor    esi, esi
        lea    eax, [esi - 1]
        mov    edi, 0xd1                     ; -> loc_2_00D1
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        jae    loc_2_0021
        xor    eax, eax
        mov    edi, 0xc4                     ; -> loc_2_00C4
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        not    eax
loc_2_0021:
        xor    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        mov    dword ptr [0x9e0], eax        ; -> obj1:0x09E0
        mov    dword ptr [0xec], 0x6cc       ; -> obj2:0x00EC
        mov    dword ptr [0xf0], 0           ; -> obj2:0x00F0
        push   0xe0                          ; -> loc_2_00E0
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        ret    
loc_2_0051:
        clc    
        ret    
loc_2_0053:
        mov    byte ptr [0xa2a], 1           ; -> obj1:0x0A2A
        cmp    byte ptr [0x9e8], 0           ; -> obj1:0x09E8
        je     loc_2_00B1
        call   sub_1_005C
        push   0
        push   4
        push   0xf003e
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    dword ptr [eax], 0x6c6c6544
        jne    loc_2_00B1
        push   0
        push   1
        push   0x4b4
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        test   byte ptr [eax], 0x20
        jne    loc_2_00B1
        cld    
        mov    esi, 0x118                    ; -> loc_2_0118
        mov    dx, 0x5b
        mov    ecx, 4
loc_2_00A8:
        lodsb  al, byte ptr [esi]
        out    dx, al
        jmp    loc_2_00AC
loc_2_00AC:
        jmp    loc_2_00AE
loc_2_00AE:
        dec    edx
        loop   loc_2_00A8
loc_2_00B1:
        clc    
        ret    
Int13_Device_Registered:
        pushal 
        sub    edi, 0xc3
        call   sub_3_0012
        popal  
        ret    
        db  000h, 000h, 000h
loc_2_00C4:
        dec    ecx
        dec    esi
        inc    esp
        dec    edi
        push   ebx
        push   eax
        dec    edi
        dec    esp
        dec    esp
        dec    ecx
        dec    esi
        inc    edi
        db  000h
loc_2_00D1:
        dec    edi
        jbe    0x139
        jb     0x142
        popal  
        jo     0x149
        dec    ecx
        dec    edi
        add    byte ptr [eax], al
        db  000h
loc_2_00E0:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x69], al
        js     0x15d
        and    byte ptr fs:[ecx + ebp*2 + 0x73], al
        imul   esp, dword ptr [eax], 0x49
        dec    esi
        push   esp
        xor    dword ptr [ebx], esi
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
loc_2_0118:
        push   edi
        dec    ecx
        dec    esi
        db  034h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xac  R-X
loc_3_0000:
        mov    edi, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [edi + 0x20]
        and    eax, 0xa001
        cmp    eax, 0x8001
        jne    loc_3_0054
sub_3_0012:
        movzx  eax, byte ptr [edi + 0x15]
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_3_0024
        test   al, 0x80
        je     loc_3_0056
        cmp    al, 0x87
        jbe    loc_3_002E
loc_3_0024:
        cmp    byte ptr [edi + 0x40], 0xa
        jne    loc_3_0056
        cmp    al, 7
        ja     loc_3_0056
loc_3_002E:
        rol    al, 1
        mov    esi, dword ptr [eax*4 + 0x9ea] ; -> obj1:0x09EA
        or     esi, esi
        jne    loc_3_0058
        mov    dword ptr [eax*4 + 0x9ea], edi ; -> obj1:0x09EA
        inc    byte ptr [0x9e8]              ; -> obj1:0x09E8
        cmp    byte ptr [edi + 0x40], 0xa
        jne    loc_3_0054
        inc    byte ptr [0x9e9]              ; -> loc_1_09E9
loc_3_0054:
        clc    
        ret    
loc_3_0056:
        stc    
        ret    
loc_3_0058:
        cmp    esi, edi
        je     loc_3_0054
        jmp    loc_3_0056
loc_3_005E:
        mov    edi, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [edi + 0x20]
        and    eax, 0xa001
        cmp    eax, 0x8001
        jne    loc_3_00AB
        movzx  eax, byte ptr [edi + 0x15]
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_3_0082
        test   al, 0x80
        je     loc_3_00AB
        cmp    al, 0x87
        jbe    loc_3_008C
loc_3_0082:
        cmp    byte ptr [edi + 0x40], 0xa
        jne    loc_3_00AB
        cmp    al, 7
        ja     loc_3_00AB
loc_3_008C:
        rol    al, 1
        mov    dword ptr [eax*4 + 0x9ea], 0  ; -> obj1:0x09EA
        dec    byte ptr [0x9e8]              ; -> obj1:0x09E8
        cmp    byte ptr [edi + 0x40], 0xa
        jne    loc_3_00AB
        inc    byte ptr [0x9e9]              ; -> loc_1_09E9
loc_3_00AB:
        ret    
VxD__LTEXT ENDS
