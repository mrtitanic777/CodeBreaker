; VPICD -- reconstructed 32-bit disassembly (VxD / LE format)
; 9 objects, 5088 instructions recovered by recursive descent

Declare_Virtual_Device VPICD, 4, 10, VPICD_Control, 00003h, Undefined_Init_Order
;   DDK version 0400h   init_order 00C000000h   flags 00000h
;   service table: obj1:0x1d70, 28 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2718  R-X
VPICD_Force_Default_Owner:
        pushal 
        pushfd 
        test   ebx, ebx
        je     loc_1_0014
        cmp    ebx, -1
        je     loc_1_0014
        cmp    dword ptr [ebx + 0x10], 0x62634d56
        jne    loc_1_0076
loc_1_0014:
        mov    ecx, eax
        movzx  eax, ax
        cmp    eax, 0xf
        ja     loc_1_0076
        cli    
        mov    edx, dword ptr [eax*4 + 0x2568] ; -> obj1:0x2568
        mov    di, 0xff3f
        and    word ptr [edx + 6], di
        shr    ecx, 0xa
        not    edi
        and    ecx, edi
        or     word ptr [edx + 6], cx
        test   byte ptr [edx + 6], 4
        jne    loc_1_0076
        mov    ecx, ebx
        xchg   dword ptr [edx + 8], ecx
        cmp    ebx, ecx
        je     loc_1_0072
        jecxz  loc_1_0072
        cmp    ecx, -1
        je     loc_1_0072
        test   ebx, ebx
        je     loc_1_0072
        mov    ebx, ecx
        mov    eax, dword ptr [edx + 0x18]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 0x20
        je     loc_1_0072
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    ebx, dword ptr [edx + 8]
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0072:
        popfd  
        popal  
        clc    
        ret    
loc_1_0076:
        popfd  
        popal  
        stc    
        ret    
VPICD_Force_Default_Behavior:
        pushal 
        pushfd 
        cmp    dword ptr [eax + 0x28], 0x51524953
        jne    loc_1_016E
        cli    
        mov    edi, dword ptr [eax]
        call   sub_1_1D1B
        test   byte ptr [edi + 6], 2
        je     loc_1_011D
        lea    ecx, [edi + 0x10]
loc_1_009E:
        mov    edx, dword ptr [ecx + 8]
        cmp    edx, eax
        je     loc_1_00A9
        mov    ecx, edx
        jmp    loc_1_009E
loc_1_00A9:
        mov    edx, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], edx
        inc    dword ptr [eax + 0x28]
        push   eax
        push   ecx
        xor    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        bts    ecx, eax
        test   word ptr [0x270e], cx         ; -> loc_1_270E
        jne    loc_1_0106
        test   word ptr [0x25f4], cx         ; -> loc_1_25F4
        je     loc_1_0106
        test   word ptr [edi + 6], 0x100
        je     loc_1_0106
        dec    byte ptr [eax + 0x26e0]       ; -> obj1:0x26E0
        cmp    byte ptr [eax + 0x26e0], 0xff ; -> obj1:0x26E0
        jne    loc_1_0106
        push   edi
        mov    ecx, eax
        mov    edi, dword ptr [ecx*4 + 0x25f4] ; -> loc_1_25F4
        cmp    byte ptr [edi], 0xe9
        jne    loc_1_0105
        mov    eax, dword ptr [edi + 1]
        xchg   dword ptr [ecx*4 + 0x2634], eax ; -> loc_1_2634
        mov    dword ptr [edi + 1], eax
        pop    edi
        jmp    loc_1_0106
loc_1_0105:
        pop    edi
loc_1_0106:
        pop    ecx
        pop    eax
        mov    esi, dword ptr [0x1e40]       ; -> loc_1_1E40
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [eax + 4], 1
        jne    loc_1_016E
loc_1_011D:
        xor    ecx, ecx
        mov    dword ptr [eax + 0xc], 0x2e0  ; -> loc_1_02E0
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [eax + 0x18], 0x34c ; -> sub_1_034C
        mov    dword ptr [eax + 0x14], 0x32c ; -> loc_1_032C
        mov    dword ptr [eax + 0x1c], 0x454 ; -> loc_1_0454
        mov    dword ptr [edi + 0x1c], 0x1f4
        mov    word ptr [edi + 6], 8
        cmp    edi, 0x20a8                   ; -> obj1:0x20A8
        jne    loc_1_016E
        mov    dword ptr [eax + 0x14], 0x28d ; -> loc_1_028D
        cmp    dword ptr [0x1d6c], ecx       ; -> loc_1_1D6C
        je     loc_1_016E
        cmp    dword ptr [edi + 8], ecx
        jne    loc_1_016E
        mov    dword ptr [eax + 0xc], 0x19b  ; -> loc_1_019B
loc_1_016E:
        popfd  
        popal  
        ret    
VPICD_Get_Virtualization_Count:
        pushfd 
        push   edx
        cmp    eax, 0xf
        ja     loc_1_0196
        mov    edx, dword ptr [eax*4 + 0x2568] ; -> obj1:0x2568
        test   byte ptr [edx + 6], 4
        je     loc_1_0196
        mov    edx, dword ptr [edx + 0x18]
        xor    al, al
loc_1_018A:
        cmp    dword ptr [edx + 4], 1
        je     loc_1_0198
        inc    eax
        mov    edx, dword ptr [edx + 8]
        jmp    loc_1_018A
loc_1_0196:
        xor    eax, eax
loc_1_0198:
        pop    edx
        popfd  
        ret    
loc_1_019B:
        mov    esi, 0x1a9                    ; -> loc_1_01A9
        mov    edx, eax
        sti    
        VMMCall  VMM.Call_Global_Event           ; cd2010800100
loc_1_01A9:
        mov    eax, dword ptr [edx + 0xc]
        cmp    eax, 0x19b                    ; -> loc_1_019B
        jne    loc_1_0212
        xor    edi, edi
loc_1_01B5:
        mov    ecx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        test   byte ptr [ecx + ebx + 0xa], 0xc
        jne    loc_1_01E3
        test   byte ptr [ebx], 8
        jne    loc_1_01E9
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ecx + 0x28]
        cmp    eax, dword ptr [0x1d58]       ; -> obj1:0x1D58
        jne    loc_1_01E3
        mov    eax, dword ptr [ecx + 0x1c4]
        cmp    eax, dword ptr [0x1d5c]       ; -> obj1:0x1D5C
        je     loc_1_01E9
loc_1_01E3:
        test   edi, edi
        jne    loc_1_0224
        mov    edi, ebx
loc_1_01E9:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_01B5
loc_1_01F7:
        mov    eax, edx
        pushfd 
        cli    
        mov    ecx, 0x2e0                    ; -> loc_1_02E0
        mov    dword ptr [eax + 0xc], ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [edx + 8], edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_1_020E:
        call   ecx
        popfd  
loc_1_0211:
        ret    
loc_1_0212:
        xchg   edx, eax
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 8
        je     loc_1_0211
        mov    ecx, edx
        pushfd 
        cli    
        jmp    loc_1_020E
loc_1_0224:
        xor    edi, edi
        jmp    loc_1_01F7
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_1D6C               ; fixup
loc_1_0230:
        pushal 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  VPICD.VPICD_Convert_Int_To_IRQ  ; cd200e000300
        jb     loc_1_0286
        cmp    al, 2
        je     loc_1_025A
        cmp    al, 9
        jne    loc_1_0286
        mov    si, word ptr [0x1d6a]         ; -> obj1:0x1D6A
        mov    edi, dword ptr [0x1d6a]       ; -> obj1:0x1D6A
        mov    ax, 8
        jmp    loc_1_026B
loc_1_025A:
        mov    si, word ptr [0x1d64]         ; -> obj1:0x1D64
        mov    edi, dword ptr [0x1d64]       ; -> obj1:0x1D64
        mov    ax, 4
loc_1_026B:
        add    ebx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        cmp    cx, si
        jne    loc_1_0282
        cmp    edi, edi
        jne    loc_1_0282
        not    eax
        and    word ptr [ebx + 0xa], ax
        jmp    loc_1_0286
loc_1_0282:
        or     word ptr [ebx + 0xa], ax
loc_1_0286:
        popal  
        jmp    loc_1_1D6C
loc_1_028D:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    eax, 0x22c0                   ; -> loc_1_22C0
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   cl, 2
        jne    loc_1_02AE
        mov    eax, 0x23f4                   ; -> loc_1_23F4
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
loc_1_02AE:
        add    ebx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        or     byte ptr [ebx + 0xa], 2
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_02BC:
        test   byte ptr [0x20ae], 4          ; -> obj1:0x20AE
        jne    loc_1_02DE
        add    ebx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        btr    word ptr [ebx + 0xa], 1
        jae    loc_1_02DE
        mov    eax, 0x23f4                   ; -> loc_1_23F4
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
loc_1_02DE:
        ret    
        db  090h
loc_1_02E0:
        mov    edx, dword ptr [eax]
        mov    ebx, dword ptr [edx + 8]
        test   ebx, ebx
        je     loc_1_02F4
        cmp    ebx, -1
        je     loc_1_031D
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_02F4:
        or     ebx, dword ptr [edx + 0xc]
        jne    loc_1_0308
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        test   byte ptr [edx + 6], 0x80
        jne    loc_1_0311
loc_1_0305:
        mov    dword ptr [edx + 0xc], ebx
loc_1_0308:
        mov    dword ptr [edx + 0x10], ebx
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_0311:
        test   ecx, ecx
        jne    loc_1_0305
        VMMCall  VMM.Get_Execution_Focus         ; cd2030000100
        jmp    loc_1_0305
loc_1_031D:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [edx + 0xc], ebx
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_032C:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
loc_1_0338:
        mov    edx, dword ptr [eax]
        xor    ebx, ebx
        xchg   dword ptr [edx + 0x10], ebx
        test   ebx, ebx
        jne    loc_1_032C
loc_1_0343:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
        mov    eax, eax
sub_1_034C:
        mov    edi, dword ptr [eax]
        mov    edx, dword ptr [edi + 8]
        jecxz  loc_1_0387
        cmp    edx, ebx
        jne    loc_1_037D
        mov    dword ptr [edi + 8], 0xffffffff
        cmp    eax, 0x23f4                   ; -> loc_1_23F4
        jne    loc_1_0381
        cmp    dword ptr [0x1d6c], 0         ; -> loc_1_1D6C
        je     loc_1_0381
        mov    dword ptr [edi + 8], 0
        mov    dword ptr [eax + 0xc], 0x19b  ; -> loc_1_019B
loc_1_037C:
        ret    
loc_1_037D:
        test   edx, edx
        jne    loc_1_037C
loc_1_0381:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008800300
loc_1_0387:
        test   edx, edx
        je     loc_1_03CB
        cmp    edx, -1
        jne    loc_1_03D1
        mov    dword ptr [edi + 8], ebx
        test   byte ptr [edi + 6], 4
        jne    loc_1_03CB
        mov    edx, dword ptr [edi]
        bt     dword ptr [0x1e32], edx       ; -> obj1:0x1E32
        jae    loc_1_03CB
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        push   ebx
loc_1_03AB:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_1_03AB
        pop    ebx
        VxDCall  VPICD.VPICD_Test_Phys_Request   ; cd2007000300
        jae    loc_1_03CB
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_03CB:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009800300
loc_1_03D1:
        cmp    ebx, edx
        je     loc_1_03CB
        test   byte ptr [edi + 6], 0x40
        jne    loc_1_0440
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        bts    word ptr [edi + ebx + 0xa], 4
        jb     loc_1_0440
        sti    
        xor    edi, edi
        mov    ecx, 0x80001030
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0410
        push   0x80001
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   ecx, eax
        or     eax, 4
        jmp    loc_1_0421
loc_1_0410:
        push   0x80002
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   ecx, eax
loc_1_0421:
        push   ecx
        xor    ecx, ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        pop    ecx
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        cmp    eax, 7
        je     loc_1_0441
        cli    
loc_1_0440:
        ret    
loc_1_0441:
        mov    eax, 0xbb8
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    ecx, ecx
        VMMCall  VMM.Close_VM                    ; cd20ec000100
loc_1_0454:
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0xc], 0
        ret    
loc_1_045E:
        push   eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0xc], 0
        mov    esi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    eax, dword ptr [eax]
        btr    word ptr [esi + ebx + 0x12], ax
        jb     loc_1_047A
        pop    eax
        ret    
loc_1_047A:
        cmp    eax, 8
        jb     loc_1_0485
        and    word ptr [esi + ebx + 0x12], 0xfffb
loc_1_0485:
        pop    eax
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
loc_1_0492:
        stc    
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
        add    byte ptr [eax], al
        db  000h
VPICD_Control:
        cmp    eax, 7
        je     loc_1_054A
        cmp    eax, 0xb
        je     loc_1_058A
        cmp    eax, 0x20
        je     loc_1_0629
        cmp    eax, 0
        je     loc_4_0000
        cmp    eax, 0x15
        je     loc_8_0000
        cmp    eax, 6
        je     loc_1_04F7
        test   dword ptr [0x4d4], 0x400000   ; -> obj1:0x04D4
        je     loc_1_04E3
        cmp    eax, 0x14
        je     loc_6_00F1
loc_1_04E3:
        cmp    eax, 2
        je     loc_4_044A
        cmp    eax, 4
        je     loc_7_0000
        clc    
        ret    
loc_1_04F7:
        pushfd 
        cli    
        mov    al, 0x11
        out    0x20, al
        jmp    loc_1_04FF
loc_1_04FF:
        mov    al, 8
        out    0x21, al
        jmp    loc_1_0505
loc_1_0505:
        mov    al, 4
        out    0x21, al
        jmp    loc_1_050B
loc_1_050B:
        mov    al, 1
        out    0x21, al
        jmp    loc_1_0511
loc_1_0511:
        mov    al, 0x4a
        out    0x20, al
        jmp    loc_1_0517
loc_1_0517:
        mov    al, 0x11
        out    0xa0, al
        jmp    loc_1_051D
loc_1_051D:
        mov    al, 0x70
        out    0xa1, al
        jmp    loc_1_0523
loc_1_0523:
        mov    al, 2
        out    0xa1, al
        jmp    loc_1_0529
loc_1_0529:
        mov    al, 1
        out    0xa1, al
        jmp    loc_1_052F
loc_1_052F:
        mov    al, 0x4a
        out    0xa0, al
        mov    ax, word ptr [0x1e36]         ; -> obj1:0x1E36
        mov    word ptr [0x1e32], ax         ; -> obj1:0x1E32
        out    0x21, al
        jmp    loc_1_0543
loc_1_0543:
        mov    al, ah
        out    0xa1, al
        popfd  
        clc    
        ret    
loc_1_054A:
        pushfd 
        cli    
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    word ptr [edi + 0xc], 0xffff
        mov    word ptr [edi + 0x14], 0x7008
        mov    eax, 0x1f4
        mov    esi, 0xcf6                    ; -> loc_1_0CF6
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        mov    dword ptr [edi + 0x18], esi
        mov    ax, word ptr [0x1e36]         ; -> obj1:0x1E36
        or     esi, 0xffffffff
        mov    word ptr [edi + 0x1c], si
        push   ebx
        call   sub_1_0650
        pop    ebx
        popfd  
        clc    
        ret    
loc_1_058A:
        pushfd 
        cli    
        push   ebx
        or     eax, 0xffffffff
        mov    esi, eax
        call   sub_1_0650
        pop    ebx
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        or     esi, 0xffffffff
        xchg   dword ptr [edi + ebx + 4], esi
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        mov    esi, dword ptr [edi + ebx + 0x18]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    esi, ebx
        add    esi, edi
        mov    eax, 0x1e68                   ; -> loc_1_1E68
        xor    ecx, ecx
loc_1_05C0:
        cmp    dword ptr [eax + 8], 0
        jne    loc_1_05FF
        test   byte ptr [eax + 6], 2
        jne    loc_1_05D2
        test   byte ptr [eax + 6], 4
        jne    loc_1_061D
loc_1_05D2:
        test   byte ptr [esi + ecx*4 + 0x20], 1
        je     loc_1_061D
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [eax + 0xc], ebx
        mov    dword ptr [eax + 0x10], ebx
        push   ecx
        push   eax
        mov    eax, dword ptr [eax + 0x18]
loc_1_05EB:
        mov    ecx, dword ptr [eax + 8]
        jecxz  loc_1_05F4
        mov    eax, ecx
        jmp    loc_1_05EB
loc_1_05F4:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    eax
        pop    ecx
        pop    ebx
        jmp    loc_1_061D
loc_1_05FF:
        test   byte ptr [eax + 6], 4
        jne    loc_1_061D
        cmp    dword ptr [eax + 8], ebx
        jne    loc_1_061D
        pushal 
        mov    eax, dword ptr [eax + 0x18]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        xor    ecx, ecx
        dec    ecx
        call   sub_1_034C
        popal  
loc_1_061D:
        add    eax, 0x40
        inc    ecx
        cmp    ecx, 0xf
        jbe    loc_1_05C0
        popfd  
        clc    
        ret    
loc_1_0629:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_064E
        mov    eax, dword ptr [0x25b0]       ; -> obj1:0x25B0
        movzx  eax, byte ptr [edi + eax + 1]
        dec    eax
        js     loc_1_064E
        bts    dword ptr [0x25f0], eax       ; -> obj1:0x25F0
        xor    esi, esi
        mov    dword ptr [eax*4 + 0x25e0], esi ; -> obj1:0x25E0
loc_1_064E:
        clc    
        ret    
sub_1_0650:
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        movzx  ecx, ax
        mov    si, cx
        xchg   word ptr [edi + 0xc], cx
        xor    cx, si
        je     loc_1_074E
        test   al, 4
        je     loc_1_0671
        mov    ah, 0xff
loc_1_0671:
        mov    word ptr [edi + 0xe], ax
loc_1_0675:
        bsf    edi, ecx
        je     loc_1_072B
        btr    ecx, edi
        mov    eax, dword ptr [edi*4 + 0x2568] ; -> obj1:0x2568
        bt     esi, edi
        jae    loc_1_06A6
        dec    word ptr [eax + 0x16]
        jne    loc_1_06C2
        test   byte ptr [eax + 6], 8
        je     loc_1_06C2
        mov    dx, word ptr [eax + 4]
        or     word ptr [0x1e30], dx         ; -> obj1:0x1E30
        jmp    loc_1_06C2
loc_1_06A6:
        call   sub_1_1CFB
        inc    word ptr [eax + 0x16]
        test   byte ptr [eax + 6], 8
        je     loc_1_06C2
        mov    dx, word ptr [eax + 4]
        not    edx
        and    word ptr [0x1e30], dx         ; -> obj1:0x1E30
loc_1_06C2:
        push   eax
        mov    ax, word ptr [0x1e30]         ; -> obj1:0x1E30
        or     ax, word ptr [0x1e32]         ; -> obj1:0x1E32
        cmp    ax, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        je     loc_1_06F6
        mov    dx, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        mov    word ptr [0x1e3a], ax         ; -> obj1:0x1E3A
        cmp    al, dl
        je     loc_1_06F2
        out    0x21, al
        cmp    ah, dh
        je     loc_1_06F6
        jmp    loc_1_06F2
loc_1_06F2:
        mov    al, ah
        out    0xa1, al
loc_1_06F6:
        pop    eax
        test   esi, 0xffff0000
        jne    loc_1_0675
        pushal 
        xor    ecx, ecx
        bt     esi, edi
        rcl    ecx, 1
        mov    eax, dword ptr [eax + 0x18]
loc_1_070E:
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_071E
        push   eax
        push   ebx
        push   ecx
        call   dword ptr [eax + 0x18]
        cli    
        pop    ecx
        pop    ebx
        pop    eax
loc_1_071E:
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    loc_1_070E
        popal  
        jmp    loc_1_0675
loc_1_072B:
        test   esi, 0xffff0000
        jne    loc_1_074E
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xchg   ebx, edx
        mov    ebp, dword ptr [edx + 8]
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        call   sub_1_0764
        cli    
loc_1_074E:
        ret    
        db  090h
loc_1_0750:
        cli    
        xchg   edx, ebx
        mov    ecx, ebx
        add    ecx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    dword ptr [ecx + 4], 0
        jmp    loc_1_0775
sub_1_0764:
        mov    ecx, edi
        xor    esi, esi
        xchg   dword ptr [ecx + 4], esi
        test   esi, esi
        je     loc_1_0775
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
loc_1_0775:
        cmp    byte ptr [ecx + 0x1e], 0
        jne    loc_1_07D5
        mov    ax, word ptr [ecx + 0x12]
        mov    si, word ptr [ecx + 0xe]
        or     esi, eax
        shl    eax, 0x10
        not    esi
        and    si, word ptr [ecx + 0x10]
        mov    ax, si
        je     loc_1_07B2
        mov    byte ptr [ecx + 0x1e], 0xff
        push   eax
        push   ecx
        xor    eax, eax
        mov    ecx, 0x3019
        mov    edi, 0x3e8
        mov    esi, 0x7fc                    ; -> loc_1_07FC
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    eax
loc_1_07B2:
        sti    
        mov    esi, 0x800000
        cmp    word ptr [ecx + 0x16], si
        ja     loc_1_07C7
        shr    esi, 1
        test   ax, ax
        jne    loc_1_07C7
        xor    esi, esi
loc_1_07C7:
        mov    eax, esi
        xchg   dword ptr [ecx], esi
        sub    eax, esi
        je     loc_1_07D6
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e800100
loc_1_07D5:
        sti    
loc_1_07D6:
        ret    
loc_1_07D7:
        xor    eax, eax
        mov    ecx, 0x3009
        mov    esi, 0x7fc                    ; -> loc_1_07FC
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        add    edi, ebx
        xchg   dword ptr [edi], eax
        neg    eax
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e800100
        nop    
loc_1_07FC:
        jb     loc_1_07D7
        sub    esp, 8
        cli    
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        movzx  eax, word ptr [edi + 0xe]
        not    ax
        and    ax, word ptr [edi + 0x10]
        movzx  ecx, word ptr [edi + 0x12]
        not    cx
        and    ax, cx
        je     loc_1_0A32
        ror    al, 3
        ror    ax, 5
        bsf    eax, eax
        not    cx
        jecxz  loc_1_0846
        ror    cl, 3
        ror    cx, 5
        bsf    ecx, ecx
        cmp    ecx, eax
        jbe    loc_1_0A7A
loc_1_0846:
        mov    edx, dword ptr [eax*4 + 0x2528] ; -> loc_1_2528
        cmp    dword ptr [edx + 8], 0
        jne    loc_1_0881
        test   byte ptr [edx + 6], 4
        jne    loc_1_0881
        push   ebx
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        mov    esi, ebx
        pop    ebx
        jecxz  loc_1_0881
        cmp    esi, ebx
        je     loc_1_0881
        mov    eax, dword ptr [edx + 0x18]
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        xchg   esi, ebx
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        xchg   esi, ebx
        jmp    loc_1_0A32
loc_1_0881:
        mov    dword ptr [esp + 4], 0
        lea    esi, [esp + 4]
        mov    eax, dword ptr [edx + 0x18]
loc_1_0890:
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_08AC
        pushal 
        mov    edx, eax
        call   dword ptr [eax + 0x10]
        popal  
        cli    
        jae    loc_1_08AC
        test   word ptr [edx + 6], 0x10
        jne    loc_1_0A32
loc_1_08AC:
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    loc_1_0890
        mov    cx, word ptr [edx + 4]
        or     word ptr [edi + 0x12], cx
        mov    eax, dword ptr [edx]
        xor    ecx, ecx
        btr    eax, 3
        jae    loc_1_08CA
        inc    ecx
        or     byte ptr [edi + 0x12], 4
loc_1_08CA:
        add    al, byte ptr [ecx + edi + 0x14]
        mov    dword ptr [esp], eax
        xor    esi, esi
        mov    ecx, dword ptr [ebx]
        test   cl, 0x40
        je     loc_1_08F2
        cmp    dword ptr [esp + 4], 0
        jne    loc_1_08F2
        push   ecx
        push   edx
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        pop    edx
        pop    ecx
        je     loc_1_08F2
        mov    esi, 0x20
loc_1_08F2:
        xor    ecx, esi
        shr    ecx, 5
        and    ecx, 1
        mov    esi, dword ptr [0x25ac]       ; -> obj1:0x25AC
        or     word ptr [esi + ebx], 0x2020
        mov    esi, dword ptr [edx]
        cmp    word ptr [edi + esi*2 + 0x60], 1
        jb     loc_1_091F
        test   byte ptr [ebx], 0x20
        jne    loc_1_0937
        test   ecx, ecx
        je     loc_1_0A07
        jmp    loc_1_0937
loc_1_091F:
        cmp    dword ptr [edx + 0x1c], 0
        je     loc_1_0937
        mov    esi, dword ptr [edx]
        VMMCall  VMM.Get_Last_Updated_VM_Exec_Time ; cd20d0000100
        or     eax, 1
        mov    dword ptr [edi + esi*4 + 0x80], eax
loc_1_0937:
        bt     dword ptr [ebx], 5
        rcl    ecx, 1
        call   sub_1_2674
        mov    eax, dword ptr [edx]
        inc    word ptr [edi + eax*2 + 0x60]
        inc    word ptr [edi + 0x16]
        inc    word ptr [edi + 0x1c]
        mov    esi, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, 0x800000
        xchg   dword ptr [esi], eax
        neg    eax
        add    eax, dword ptr [esi]
        je     loc_1_096E
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_1_096E:
        mov    eax, 0x400000
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
        add    edi, dword ptr [0x25b0]       ; -> obj1:0x25B0
        inc    byte ptr [edi]
        mov    edi, esi
        btr    dword ptr [ebx], 8
        rcl    ecx, 1
        test   byte ptr [ebx], 0x20
        jne    loc_1_0A46
        movzx  eax, word ptr [ebp + 0x30]
        sub    eax, 0xa
        jb     loc_1_0A46
        mov    dword ptr [ebp + 0x30], eax
        movzx  esi, word ptr [ebp + 0x34]
        shl    esi, 4
        add    esi, eax
        mov    ax, word ptr [ebp + 0x28]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x24]
        mov    dword ptr [esi + 6], eax
        mov    ecx, dword ptr [edx + ecx*4 + 0x20]
        mov    eax, dword ptr [esp]
        bt     dword ptr [0x1e44], eax       ; -> loc_1_1E44
        jb     loc_1_09F5
loc_1_09C9:
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        jne    loc_1_09DB
        VMMCall  VMM.Get_Instanced_V86_Int_Vector ; cd2041010100
        mov    ecx, eax
loc_1_09DB:
        mov    word ptr [ebp + 0x24], cx
        shr    ecx, 0x10
        mov    word ptr [ebp + 0x28], cx
        mov    eax, dword ptr [ebp + 0x2c]
        mov    word ptr [esi + 4], ax
        and    ah, 0xfc
        mov    byte ptr [ebp + 0x2d], ah
        jmp    loc_1_0A74
loc_1_09F5:
        mov    word ptr [ebp + 0x24], cx
        shr    ecx, 0x10
        mov    word ptr [ebp + 0x28], cx
        add    word ptr [ebp + 0x30], 6
        jmp    loc_1_0A56
loc_1_0A07:
        bt     dword ptr [0x1e44], eax       ; -> loc_1_1E44
        jb     loc_1_0A56
        movzx  ecx, word ptr [ebp + 0x30]
        sub    ecx, 6
        jb     loc_1_0A56
        mov    dword ptr [ebp + 0x30], ecx
        movzx  esi, word ptr [ebp + 0x34]
        shl    esi, 4
        add    esi, ecx
        mov    cx, word ptr [ebp + 0x28]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x24]
        jmp    loc_1_09C9
loc_1_0A32:
        cmp    word ptr [edi + 0x12], 0
        jne    loc_1_0A7A
        mov    byte ptr [edi + 0x1e], 0
        mov    edx, ebx
        call   sub_1_0764
        jmp    loc_1_0A7E
loc_1_0A46:
        mov    ecx, dword ptr [edx + ecx*4 + 0x20]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
loc_1_0A56:
        sti    
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_0A6E
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        jmp    loc_1_0A74
loc_1_0A6E:
        VMMCall  VMM.Simulate_Int                ; cd2046000100
loc_1_0A74:
        test   byte ptr [ebx + 1], 0x80
        jne    loc_1_0A83
loc_1_0A7A:
        mov    byte ptr [edi + 0x1e], 0
loc_1_0A7E:
        sti    
        add    esp, 8
        ret    
loc_1_0A83:
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        jmp    loc_1_0A7A
loc_1_0A8B:
        ret    
loc_1_0A8C:
        VMMCall  VMM.Set_PM_Exec_Mode            ; cd208a000100
loc_1_0A92:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0A9F
        call   sub_1_0B47
loc_1_0A9F:
        VMMCall  VMM.Begin_Use_Locked_PM_Stack   ; cd208b800100
loc_1_0AA5:
        VMMCall  VMM.Begin_Use_Locked_PM_Stack   ; cd208b000100
        VMMCall  VMM.Set_V86_Exec_Mode           ; cd2089000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0A8B
        push   esi
        xor    esi, esi
loc_1_0ABC:
        cmp    dword ptr [0x25dc], 0         ; -> obj1:0x25DC
        je     loc_1_0B25
        push   edi
        push   ecx
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0x25b0]       ; -> obj1:0x25B0
        cmp    byte ptr [edi + 1], 0
        jne    loc_1_0B27
        bsf    eax, dword ptr [0x25f0]       ; -> obj1:0x25F0
        je     loc_1_0B3D
        btr    dword ptr [0x25f0], eax       ; -> obj1:0x25F0
        inc    eax
        mov    byte ptr [edi + 1], al
        dec    eax
        sub    edi, dword ptr [0x25b0]       ; -> obj1:0x25B0
        mov    dword ptr [eax*4 + 0x25e0], edi ; -> obj1:0x25E0
        mov    ecx, dword ptr [0x25dc]       ; -> obj1:0x25DC
        shl    eax, 8
        sub    ecx, eax
        mov    eax, ecx
        sub    eax, dword ptr [0x25d8]       ; -> obj1:0x25D8
        xchg   dword ptr [ebp + esi + 0x30], eax
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [0x25d4]       ; -> obj1:0x25D4
        xchg   word ptr [ebp + esi + 0x34], ax
        mov    word ptr [ecx + 2], ax
loc_1_0B23:
        pop    ecx
        pop    edi
loc_1_0B25:
        pop    esi
        ret    
loc_1_0B27:
        mov    ax, word ptr [ebp + esi + 0x34]
        cmp    ax, word ptr [0x25d4]         ; -> obj1:0x25D4
        jne    loc_1_0B23
        sub    word ptr [ebp + esi + 0x30], 2
        jmp    loc_1_0B23
loc_1_0B3D:
        xor    ecx, ecx
        VMMCall  VMM.Begin_V86_Serialization     ; cd203c010100
        jmp    loc_1_0B23
sub_1_0B47:
        push   esi
        mov    esi, 0x24
        jmp    loc_1_0ABC
loc_1_0B52:
        mov    eax, 0x100
        jmp    loc_1_0B5B
loc_1_0B59:
        xor    eax, eax
loc_1_0B5B:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        VMMCall  VMM.End_Use_Locked_PM_Stack     ; cd208c000100
        VMMCall  VMM.Set_V86_Exec_Mode           ; cd2089000100
        or     dword ptr [ebx], eax
        jmp    loc_1_0BA7
loc_1_0B71:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        VMMCall  VMM.End_Use_Locked_PM_Stack     ; cd208c000100
        or     byte ptr [ebx + 1], 1
        jmp    loc_1_0BA7
loc_1_0B83:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        VMMCall  VMM.End_Use_Locked_PM_Stack     ; cd208c000100
        jmp    loc_1_0BA7
loc_1_0B91:
        or     byte ptr [ebx + 1], 1
loc_1_0B95:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        VMMCall  VMM.Set_PM_Exec_Mode            ; cd208a000100
        VMMCall  VMM.End_Use_Locked_PM_Stack     ; cd208c000100
loc_1_0BA7:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0C62
        mov    ecx, dword ptr [0x25dc]       ; -> obj1:0x25DC
        test   ecx, ecx
        je     loc_1_0C62
        mov    eax, dword ptr [0x25b0]       ; -> obj1:0x25B0
        movzx  eax, byte ptr [edi + eax + 1]
        dec    eax
        js     loc_1_0C32
        shl    eax, 8
        sub    ecx, eax
        xor    esi, esi
        test   byte ptr [ebx], 0x20
        je     loc_1_0BDF
        mov    esi, 0x24
loc_1_0BDF:
        mov    ax, word ptr [ebp + esi + 0x34]
        cmp    ax, word ptr [0x25d4]         ; -> obj1:0x25D4
        jne    loc_1_0C62
        mov    eax, ecx
        sub    eax, dword ptr [0x25d8]       ; -> obj1:0x25D8
        cmp    ax, word ptr [ebp + esi + 0x30]
        ja     loc_1_0C2A
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebp + esi + 0x30], ax
        shr    eax, 0x10
        mov    word ptr [ebp + esi + 0x34], ax
        mov    ecx, dword ptr [0x25b0]       ; -> obj1:0x25B0
        xor    eax, eax
        xchg   byte ptr [edi + ecx + 1], al
        dec    eax
        bts    dword ptr [0x25f0], eax       ; -> obj1:0x25F0
        xor    esi, esi
        mov    dword ptr [eax*4 + 0x25e0], esi ; -> obj1:0x25E0
        jmp    loc_1_0C62
loc_1_0C2A:
        add    word ptr [ebp + esi + 0x30], 2
        jmp    loc_1_0C62
loc_1_0C32:
        VMMCall  VMM.End_V86_Serialization       ; cd2072010100
        jmp    loc_1_0C62
loc_1_0C3A:
        or     byte ptr [ebx + 1], 1
loc_1_0C3E:
        movzx  ecx, word ptr [ebp + 0x34]
        shl    ecx, 4
        movzx  esi, word ptr [ebp + 0x30]
        mov    ax, word ptr [esi + ecx]
        mov    word ptr [ebp + 0x24], ax
        add    si, 2
        mov    ax, word ptr [esi + ecx]
        mov    word ptr [ebp + 0x28], ax
        add    word ptr [ebp + 0x30], 4
loc_1_0C62:
        test   byte ptr [ebp + 0x2d], 2
        jne    loc_1_0C6E
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_0C6E:
        clc    
        pushfd 
        mov    eax, dword ptr [0x25b0]       ; -> obj1:0x25B0
        add    eax, edi
        cmp    byte ptr [eax], 0
        je     loc_1_0C89
        dec    byte ptr [eax]
        mov    eax, 0xffc00000
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
loc_1_0C89:
        cli    
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        add    edi, ebx
        mov    ecx, dword ptr [edx]
        dec    word ptr [edi + 0x1c]
        jns    loc_1_0CA4
        mov    eax, dword ptr [0x25ac]       ; -> obj1:0x25AC
        and    byte ptr [eax + ebx + 1], 0xdf
loc_1_0CA4:
        cmp    word ptr [edi + ecx*2 + 0x60], 1
        jb     loc_1_0CF4
        je     loc_1_0CB9
        dec    word ptr [edi + ecx*2 + 0x60]
        dec    word ptr [edi + 0x16]
        jmp    loc_1_0CF4
loc_1_0CB9:
        mov    dword ptr [edi + ecx*4 + 0x80], 0
loc_1_0CC4:
        dec    word ptr [edi + ecx*2 + 0x60]
        push   edi
        mov    eax, dword ptr [edx + 0x18]
loc_1_0CCD:
        push   dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 0x1c]
        jecxz  loc_1_0CE1
        bt     word ptr [esp + 8], 0
        push   ebx
        call   ecx
        pop    ebx
        cli    
loc_1_0CE1:
        pop    eax
        test   eax, eax
        jne    loc_1_0CCD
        pop    edi
        dec    word ptr [edi + 0x16]
        jne    loc_1_0CF4
        mov    edx, ebx
        call   sub_1_0764
loc_1_0CF4:
        popfd  
        ret    
loc_1_0CF6:
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    eax, 0x1f4
        mov    esi, 0xcf6                    ; -> loc_1_0CF6
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        mov    dword ptr [edi + 0x18], esi
        cmp    word ptr [edi + 0x16], 0
        je     loc_1_0D51
        VMMCall  VMM.Get_Last_Updated_VM_Exec_Time ; cd20d0000100
        or     eax, 1
        mov    esi, eax
        lea    edi, [edi + 0x80]
        mov    ecx, 0x10
        cld    
loc_1_0D2F:
        xor    eax, eax
        repe scasd eax, dword ptr es:[edi]
        je     loc_1_0D51
        mov    edx, ecx
        neg    edx
        add    edx, 0xf
        mov    edx, dword ptr [edx*4 + 0x2568] ; -> obj1:0x2568
        mov    eax, esi
        sub    eax, dword ptr [edi - 4]
        cmp    eax, dword ptr [edx + 0x1c]
        jae    loc_1_0D52
        test   ecx, ecx
        jne    loc_1_0D2F
loc_1_0D51:
        ret    
loc_1_0D52:
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edi + ecx*4 + 0x80], 0
        stc    
        pushfd 
        push   edi
        mov    esi, dword ptr [0x25b0]       ; -> obj1:0x25B0
        mov    eax, 0x400000
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        push   edi
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
        neg    eax
        VMMCall  VMM.Get_Initial_Thread_Handle   ; cd200d010100
loc_1_0D8A:
        cmp    byte ptr [esi + edi], 0
        je     loc_1_0D99
        dec    byte ptr [esi + edi]
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
loc_1_0D99:
        VMMCall  VMM.Get_Next_Thread_Handle      ; cd2013010100
        cmp    ebx, dword ptr [edi + 0x14]
        jne    loc_1_0DAC
        VMMCall  VMM.Test_Initial_Thread_Handle  ; cd200e010100
        jne    loc_1_0D8A
loc_1_0DAC:
        pop    edi
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
        pop    edi
        cli    
        jmp    loc_1_0CC4
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_1E64               ; fixup
loc_1_0DC2:
        cmp    eax, 0xff
        ja     loc_1_0DD0
        bts    dword ptr [0x1e44], eax       ; -> loc_1_1E44
loc_1_0DD0:
        jmp    loc_1_1E64
        db  08Bh, 0C0h
loc_1_0DD8:
        out    0xa0, al
        jmp    loc_1_0DDC
loc_1_0DDC:
        mov    al, 0x62
loc_1_0DDE:
        out    0x20, al
        mov    eax, dword ptr [edx + 0x18]
        push   ebx
loc_1_0DE4:
        push   eax
        mov    edx, dword ptr [eax + 0x24]
        call   dword ptr [eax + 0xc]
        cli    
        pop    eax
        jae    loc_1_0DF9
        mov    ebx, dword ptr [esp]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    loc_1_0DE4
loc_1_0DF9:
        pop    ebx
        jmp    loc_1_1E3C
loc_1_0E00:
        mov    al, 0x67
        out    0xa0, al
        jmp    loc_1_0E06
loc_1_0E06:
        mov    al, 0x62
        out    0x20, al
        ret    
loc_1_0E0B:
        mov    al, 0x67
        out    0x20, al
        ret    
loc_1_0E10:
        out    0xa0, al
        jmp    loc_1_0E14
loc_1_0E14:
        mov    al, 0x62
loc_1_0E16:
        out    0x20, al
        mov    eax, dword ptr [edx + 0x18]
        mov    ecx, dword ptr [eax]
        mov    ecx, dword ptr [ecx]
        push   ebx
        push   ecx
        push   eax
loc_1_0E22:
        mov    edx, dword ptr [eax + 0x24]
        push   eax
        call   dword ptr [eax + 0xc]
        cli    
        pop    eax
        jae    loc_1_0E3A
        mov    ebx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    loc_1_0E22
        jmp    loc_1_0E47
loc_1_0E3A:
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        jmp    loc_1_0E22
loc_1_0E47:
        pop    eax
        pop    ecx
        pop    ebx
        jmp    loc_1_1E3C
loc_1_0E50:
        VxDCall  VPICD.VPICD_VM_SlavePIC_Mask_Change ; cd2018800300
        mov    eax, eax
loc_1_0E58:
        or     byte ptr [0x1e32], 1          ; -> obj1:0x1E32
        mov    edx, 0x1e68                   ; -> loc_1_1E68
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0E7E
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0E7E:
        mov    al, 0x60
loc_1_0E80:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0E88:
        or     byte ptr [0x1e32], 2          ; -> obj1:0x1E32
        mov    edx, 0x1ea8                   ; -> obj1:0x1EA8
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0EAE
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0EAE:
        mov    al, 0x61
loc_1_0EB0:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0EB8:
        or     byte ptr [0x1e32], 8          ; -> obj1:0x1E32
        mov    edx, 0x1f28                   ; -> obj1:0x1F28
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0EDE
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0EDE:
        mov    al, 0x63
loc_1_0EE0:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0EE8:
        or     byte ptr [0x1e32], 0x10       ; -> obj1:0x1E32
        mov    edx, 0x1f68                   ; -> obj1:0x1F68
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0F0E
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0F0E:
        mov    al, 0x64
loc_1_0F10:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0F18:
        or     byte ptr [0x1e32], 0x20       ; -> obj1:0x1E32
        mov    edx, 0x1fa8                   ; -> obj1:0x1FA8
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0F3E
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0F3E:
        mov    al, 0x65
loc_1_0F40:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0F48:
        or     byte ptr [0x1e32], 0x40       ; -> obj1:0x1E32
        mov    edx, 0x1fe8                   ; -> obj1:0x1FE8
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0F6E
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0F6E:
        mov    al, 0x66
loc_1_0F70:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0F78:
        mov    al, 0xb
        out    0x20, al
        jmp    loc_1_0F7E
loc_1_0F7E:
        in     al, 0x20
        test   al, 0x80
        je     loc_1_0E0B
        or     byte ptr [0x1e32], 0x80       ; -> obj1:0x1E32
        mov    edx, 0x2028                   ; -> obj1:0x2028
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_0FAE
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_0FAE:
        mov    al, 0x67
loc_1_0FB0:
        jmp    loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0FB8:
        or     byte ptr [0x1e33], 1          ; -> obj1:0x1E33
        mov    edx, 0x2068                   ; -> sub_1_2068
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_0FDE
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_0FDE:
        mov    al, 0x60
loc_1_0FE0:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_0FE8:
        or     byte ptr [0x1e33], 2          ; -> obj1:0x1E33
        mov    edx, 0x20a8                   ; -> obj1:0x20A8
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_100E
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_100E:
        mov    al, 0x61
loc_1_1010:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1018:
        or     byte ptr [0x1e33], 4          ; -> obj1:0x1E33
        mov    edx, 0x20e8                   ; -> obj1:0x20E8
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_103E
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_103E:
        mov    al, 0x62
loc_1_1040:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1048:
        or     byte ptr [0x1e33], 8          ; -> obj1:0x1E33
        mov    edx, 0x2128                   ; -> obj1:0x2128
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_106E
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_106E:
        mov    al, 0x63
loc_1_1070:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1078:
        or     byte ptr [0x1e33], 0x10       ; -> obj1:0x1E33
        mov    edx, 0x2168                   ; -> obj1:0x2168
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_109E
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_109E:
        mov    al, 0x64
loc_1_10A0:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_10A8:
        or     byte ptr [0x1e33], 0x20       ; -> obj1:0x1E33
        mov    edx, 0x21a8                   ; -> obj1:0x21A8
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_10CE
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_10CE:
        mov    al, 0x65
loc_1_10D0:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_10D8:
        or     byte ptr [0x1e33], 0x40       ; -> obj1:0x1E33
        mov    edx, 0x21e8                   ; -> loc_1_21E8
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_10FE
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_10FE:
        mov    al, 0x66
loc_1_1100:
        jmp    loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1108:
        mov    al, 0xb
        out    0xa0, al
        jmp    loc_1_110E
loc_1_110E:
        in     al, 0xa0
        test   al, 0x80
        je     loc_1_0E00
        or     byte ptr [0x1e33], 0x80       ; -> obj1:0x1E33
        mov    edx, 0x2228                   ; -> obj1:0x2228
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_113E
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_113E:
        mov    al, 0x67
loc_1_1140:
        jmp    loc_1_0DD8
        db  090h
loc_1_1146:
        ret    
loc_1_1147:
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
        add    byte ptr [eax], al
        db  000h
VPICD_PM_API:
        mov    eax, dword ptr [ebp + 0x1c]
        test   eax, eax
        je     loc_1_1267
        cmp    ax, 1
        je     loc_1_11CC
        cmp    ax, 2
        jne    loc_1_128D
        xchg   esi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi + 8]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        xor    eax, eax
        xchg   word ptr [esi + 0x30], ax
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        xor    eax, eax
        xchg   word ptr [esi + 0x32], ax
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        movzx  ebx, word ptr [esi + 0x34]
        test   ebx, ebx
        je     loc_1_11C7
        lea    edi, [esi + 0x36]
loc_1_11AF:
        xor    eax, eax
        xchg   word ptr [edi + 4], ax
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        add    edi, 6
        dec    ebx
        jne    loc_1_11AF
loc_1_11C7:
        jmp    loc_1_126D
loc_1_11CC:
        xchg   esi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    edi, 0x2692                   ; -> obj1:0x2692
        mov    ax, word ptr [esi]
        mov    word ptr [edi], ax
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_1_128D
        mov    dword ptr [esi + 8], eax
        movzx  ecx, word ptr [ebp]
        mov    word ptr [esi + 0xc], cx
        movzx  ecx, word ptr [ebp + 0x38]
        mov    word ptr [esi + 0xe], cx
        mov    dword ptr [eax + 0x20], esi
        mov    eax, dword ptr [0x268a]       ; -> obj1:0x268A
        mov    dword ptr [esi + 0x28], eax
        movzx  eax, word ptr [esi + 0x2c]
        mov    ecx, eax
        call   sub_1_1292
        or     eax, eax
        je     loc_1_1284
        mov    word ptr [esi + 0x30], ax
        cmp    cx, word ptr [ebp + 0x38]
        jne    loc_1_122A
        mov    word ptr [esi + 0xe], ax
loc_1_122A:
        movzx  eax, word ptr [esi + 0x2e]
        mov    ecx, eax
        call   sub_1_1292
        or     eax, eax
        je     loc_1_1272
        mov    word ptr [esi + 0x32], ax
        cmp    cx, word ptr [ebp + 0x38]
        jne    loc_1_1247
        mov    word ptr [esi + 0xe], ax
loc_1_1247:
        movzx  ecx, word ptr [esi + 0x34]
        jecxz  loc_1_1265
        lea    edi, [esi + 0x36]
loc_1_1250:
        movzx  eax, word ptr [edi]
        call   sub_1_1292
        or     eax, eax
        je     loc_1_1272
        mov    word ptr [edi + 4], ax
        add    edi, 6
        loop   loc_1_1250
loc_1_1265:
        jmp    loc_1_126D
loc_1_1267:
        mov    word ptr [ebp + 0x1c], 0x40a
loc_1_126D:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_1_1272:
        xor    eax, eax
        xchg   word ptr [esi + 0x30], ax
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
loc_1_1284:
        mov    eax, dword ptr [esi + 8]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_1_128D:
        or     byte ptr [ebp + 0x2c], 1
        ret    
sub_1_1292:
        push   ecx
        push   edx
        push   0
        push   ebx
        push   eax
        VMMCall  VMM._GetDescriptor              ; cd207b000100
        add    esp, 0xc
        test   dh, 8
        je     loc_1_12A9
        and    dh, 0x9f
loc_1_12A9:
        ror    eax, 0x10
        rol    edx, 8
        mov    cl, dh
        mov    ch, dl
        shl    ecx, 0x10
        mov    cx, ax
        cmp    ecx, 0x10ffff
        ja     loc_1_12C4
        add    ecx, dword ptr [ebx + 4]
loc_1_12C4:
        mov    ax, cx
        shr    ecx, 0x10
        mov    dh, cl
        mov    dl, ch
        ror    edx, 8
        rol    eax, 0x10
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        pop    edx
        pop    ecx
        ret    
loc_1_12E4:
        mov    esi, dword ptr [eax + 0x20]
        mov    ecx, dword ptr [ebx + 0xc]
        mov    word ptr [esi + 2], cx
        mov    word ptr [esi + 0x1c], 4
        pop    ecx
        push   cs
        push   ecx
        push   eax
        push   ebx
        push   esi
        push   es
        push   ds
        push   dword ptr [0x268e]            ; -> obj1:0x268E
        movzx  ecx, word ptr [esi + 0x30]
        push   ecx
        movzx  ecx, word ptr [esi + 0x1e]
        push   ecx
        mov    di, word ptr [esi + 0xc]
        mov    es, word ptr [esi + 0xe]
        retf   
loc_1_1315:
        pop    ds
        pop    es
        pop    esi
        pop    ebx
        pop    eax
        mov    word ptr [esi + 0x1c], 0
        retf   
        db  02Eh, 08Bh, 0C0h
loc_1_1324:
        push   ebp
        mov    ebp, esp
        and    esp, 0xfffffffc
        push   ds
        push   es
        pushal 
        movzx  ecx, ax
        movzx  edi, di
        mov    eax, dword ptr es:[edi + 8]
        mov    dx, ss
        mov    ds, dx
        mov    es, dx
        jmp    loc_1_2684
loc_1_1346:
        mov    ebp, esp
        cmp    cx, 6
        ja     loc_1_1355
        call   dword ptr [ecx*4 + 0x26b4]    ; -> obj1:0x26B4
loc_1_1355:
        popal  
        pop    es
        pop    ds
        mov    esp, ebp
        pop    ebp
        retf   
        db  02Eh, 08Bh, 0C0h
loc_1_1360:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
        mov    eax, eax
loc_1_1368:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008800300
        mov    eax, eax
loc_1_1370:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009800300
        mov    eax, eax
loc_1_1378:
        mov    bx, word ptr [0x1e30]         ; -> obj1:0x1E30
loc_1_137F:
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        bt     bx, ax
        ret    
loc_1_1388:
        VxDCall  VPICD.VPICD_Test_Phys_Request   ; cd2007800300
        mov    eax, eax
loc_1_1390:
        mov    bx, word ptr [0x1e32]         ; -> obj1:0x1E32
        jmp    loc_1_137F
        db  02Eh, 08Bh, 0C0h
loc_1_139C:
        mov    esi, 0x13fe                   ; -> loc_1_13FE
        mov    edi, dword ptr [ebp + 0x18]
        shl    edi, 0x10
        mov    di, word ptr [ebp + 0x14]
        mov    edx, dword ptr [eax + 0x20]
        mov    dword ptr [edx + 0x10], edi
        movzx  eax, word ptr [ebp + 0x10]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_1_13BB:
        cmp    eax, dword ptr [ebx + 0xc]
        je     loc_1_13CF
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_13BB
        ret    
loc_1_13CF:
        movzx  ecx, word ptr [ebp + 4]
        mov    eax, 0x400000
        cmp    ecx, 3
        jae    loc_1_13F3
        mov    eax, 0x1000
        cmp    ecx, 2
        je     loc_1_13F3
        mov    eax, 0x10
        cmp    ecx, 1
        je     loc_1_13F3
        mov    eax, ecx
loc_1_13F3:
        mov    ecx, 9
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014800100
loc_1_13FE:
        test   dword ptr [ebx], 0x8000
        je     loc_1_140C
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
loc_1_140C:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    edi, edx
        mov    cx, word ptr [edi + 0xc]
        mov    word ptr [ebp], cx
        mov    cx, word ptr [edi + 0xe]
        mov    word ptr [ebp + 0x38], cx
        movzx  edx, word ptr [edi + 0x10]
        movzx  ecx, word ptr [edi + 0x12]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VMMCall  VMM.Enable_VM_Ints              ; cd201a800100
        add    byte ptr [eax], al
VPICD_Virtual_EOI:
        pushal 
        cli    
        mov    eax, dword ptr [eax]
        mov    ebp, dword ptr [ebx + 8]
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        call   sub_1_14EF
        popal  
        ret    
loc_1_147A:
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1480:
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        lea    edi, [edi + ebx + 1]
        cmp    ecx, 4
        je     loc_1_14CB
        ja     loc_1_147A
loc_1_1491:
        cli    
        test   byte ptr [edi + 0xa], 1
        je     loc_1_149D
        mov    al, byte ptr [edi + 0x12]
        sti    
        ret    
loc_1_149D:
        mov    al, 0xa
        out    dx, al
        jmp    loc_1_14A2
loc_1_14A2:
        in     al, dx
        mov    bl, byte ptr [edi + 0x10]
        mov    ecx, edx
        shr    ecx, 7
        mov    cl, byte ptr [ecx + 0x1e34]   ; -> obj1:0x1E34
        and    al, cl
        not    cl
        and    bl, cl
        or     al, bl
        sti    
        ret    
        db  090h
loc_1_14BC:
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        cmp    ecx, 4
        jb     loc_1_1491
        ja     loc_1_147A
loc_1_14CB:
        cli    
        cmp    al, 0x20
        jne    loc_1_153F
loc_1_14D0:
        movzx  eax, byte ptr [edi + 0x12]
        bsf    eax, eax
        je     loc_1_156B
loc_1_14DD:
        mov    eax, dword ptr [eax*4 + 0x2568] ; -> obj1:0x2568
        cmp    edx, 0x20
        je     sub_1_14EF
        add    eax, 0x200
        dec    edi
sub_1_14EF:
        push   edi
        push   eax
        mov    dx, word ptr [eax + 6]
        mov    eax, dword ptr [eax + 0x18]
loc_1_14F8:
        mov    ecx, dword ptr [eax + 0x14]
        jecxz  loc_1_1537
        push   eax
        push   ebx
        push   edx
        call   ecx
        cli    
        pop    edx
        pop    ebx
        pop    eax
        jae    loc_1_150D
        test   dl, 0x10
        jne    loc_1_1533
loc_1_150D:
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    loc_1_14F8
        pop    eax
        pop    edi
        mov    cx, word ptr [eax + 4]
        not    ecx
        and    word ptr [edi + 0x12], cx
        jne    loc_1_152C
        mov    edx, dword ptr [0x25ac]       ; -> obj1:0x25AC
        and    byte ptr [edx + ebx], 0xdf
loc_1_152C:
        mov    edx, ebx
        jmp    sub_1_0764
loc_1_1533:
        pop    eax
        pop    edi
        jmp    loc_1_152C
loc_1_1537:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        jmp    loc_1_150D
loc_1_153F:
        test   al, 0x10
        jne    loc_1_156D
        test   al, 8
        je     loc_1_157D
        test   al, 2
        je     loc_1_1557
        and    byte ptr [edi + 0xa], 0xfe
        test   al, 1
        je     loc_1_1557
        or     byte ptr [edi + 0xa], 1
loc_1_1557:
        test   al, 0x40
        je     loc_1_1563
        test   al, 0x20
        jne    loc_1_1613
loc_1_1563:
        test   al, 4
        jne    loc_1_1613
loc_1_156B:
        sti    
        ret    
loc_1_156D:
        and    al, 0xb
        cmp    al, 1
        jne    loc_1_1613
        mov    byte ptr [edi + 8], 4
        sti    
        ret    
loc_1_157D:
        movzx  ecx, al
        and    eax, 7
        sub    ecx, eax
        cmp    ecx, 0x60
        je     loc_1_14DD
        cmp    ecx, 0x20
        je     loc_1_14D0
        sti    
        ret    
loc_1_1599:
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
        nop    
loc_1_15A0:
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        lea    edi, [edi + ebx + 1]
        cmp    ecx, 4
        je     loc_1_15C7
        ja     loc_1_1599
loc_1_15B1:
        mov    al, byte ptr [edi + 0xc]
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_15B8:
        mov    edi, ebx
        add    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        cmp    ecx, 4
        jb     loc_1_15B1
        ja     loc_1_1599
loc_1_15C7:
        cli    
        movzx  ecx, byte ptr [edi + 8]
        jmp    dword ptr [ecx + 0x26d0]      ; -> obj1:0x26D0
loc_1_15D2:
        cmp    edx, 0x21
        jne    loc_1_15DC
        mov    ah, byte ptr [edi + 0xd]
        jmp    loc_1_15E1
loc_1_15DC:
        mov    ah, al
        mov    al, byte ptr [edi + 0xb]
loc_1_15E1:
        xor    esi, esi
        call   sub_1_0650
        sti    
        ret    
loc_1_15EA:
        and    al, 0xf8
        mov    byte ptr [edi + 0x14], al
        mov    cl, 8
        jmp    loc_1_160E
loc_1_15F3:
        mov    cl, 0xc
        cmp    edx, 0x21
        jne    loc_1_1600
        cmp    al, 4
        jne    loc_1_1613
        jmp    loc_1_160E
loc_1_1600:
        cmp    al, 2
        jne    loc_1_1613
        jmp    loc_1_160E
loc_1_1606:
        and    al, 0x1b
        cmp    al, 1
        jne    loc_1_1613
        mov    cl, 0
loc_1_160E:
        mov    byte ptr [edi + 8], cl
        sti    
        ret    
loc_1_1613:
        sti    
        ret    
        db  000h, 000h, 000h
VPICD_Get_Version:
        mov    eax, 0x40a
        mov    ecx, 0xf
        mov    ebx, 1
        clc    
        ret    
VPICD_Virtualize_IRQ:
        pushal 
        pushfd 
        movzx  ecx, word ptr [edi]
        cmp    ecx, 0xf
        ja     loc_1_1809
        cli    
        test   byte ptr [edi + 2], 0x20
        je     loc_1_1642
        or     byte ptr [edi + 2], 2
loc_1_1642:
        mov    esi, dword ptr [ecx*4 + 0x2568] ; -> obj1:0x2568
        test   byte ptr [esi + 6], 4
        je     loc_1_1717
        test   byte ptr [esi + 6], 2
        je     loc_1_1809
        test   byte ptr [edi + 2], 2
        je     loc_1_1809
        push   esi
        movzx  ebx, word ptr [esi + 6]
        and    ebx, 0x100
        movzx  esi, word ptr [edi + 2]
        and    esi, 0x100
        cmp    ebx, esi
        pop    esi
        jne    loc_1_1809
loc_1_1685:
        mov    ebx, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [ebx + 4]
        shl    ebx, 1
        jb     loc_1_1809
        push   esi
        mov    esi, dword ptr [0x1e40]       ; -> loc_1_1E40
        VMMCall  VMM.List_Allocate               ; cd209d000100
        pop    esi
        jb     loc_1_1809
        mov    dword ptr [eax], esi
        mov    edx, eax
        xchg   dword ptr [esi + 0x18], edx
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 4], ebx
        push   edx
        xor    edx, edx
        mov    cx, word ptr [edi]
        bts    edx, ecx
        test   word ptr [0x25f4], dx         ; -> loc_1_25F4
        je     loc_1_1710
        test   word ptr [0x270e], dx         ; -> loc_1_270E
        jne    loc_1_1710
        test   word ptr [edi + 2], 0x100
        je     loc_1_1710
        inc    byte ptr [ecx + 0x26e0]       ; -> obj1:0x26E0
        jne    loc_1_1710
        push   edi
        push   eax
        mov    edi, dword ptr [ecx*4 + 0x25f4] ; -> loc_1_25F4
        mov    eax, 0x60b0
        add    ah, cl
        and    ah, 0xf7
        cmp    word ptr [edi - 2], ax
        jne    loc_1_1713
        mov    eax, 0xe9
        cmp    al, byte ptr [edi]
        jne    loc_1_1713
        mov    eax, dword ptr [edi + 1]
        xchg   dword ptr [ecx*4 + 0x2634], eax ; -> loc_1_2634
        mov    dword ptr [edi + 1], eax
        pop    eax
        pop    edi
loc_1_1710:
        pop    edx
        jmp    loc_1_177D
loc_1_1713:
        pop    eax
        pop    edi
        jmp    loc_1_1710
loc_1_1717:
        mov    bx, word ptr [edi + 2]
        and    bx, 0x133
        or     bx, 0xc
        mov    word ptr [esi + 6], bx
        mov    eax, dword ptr [esi + 0x18]
        test   bx, 2
        je     loc_1_1776
        xor    ecx, ecx
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0xc], 0x2e0  ; -> loc_1_02E0
        mov    dword ptr [eax + 0x14], 0x338 ; -> loc_1_0338
        mov    dword ptr [eax + 0x1c], 0x45e ; -> loc_1_045E
        cmp    dword ptr [esi + 8], 0
        je     loc_1_1685
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0xc], 0x492  ; -> loc_1_0492
        mov    ebx, dword ptr [esi + 8]
        cmp    ebx, -1
        je     loc_1_1771
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_1_1771:
        jmp    loc_1_1685
loc_1_1776:
        mov    dword ptr [esi + 0x1c], 0
loc_1_177D:
        mov    ecx, dword ptr [edi + 0x18]
        cmp    ecx, dword ptr [esi + 0x1c]
        jb     loc_1_1788
        mov    dword ptr [esi + 0x1c], ecx
loc_1_1788:
        mov    cx, word ptr [edi + 2]
        and    ecx, 0x20
        je     loc_1_17A9
        or     word ptr [esi + 6], cx
        push   eax
loc_1_1796:
        mov    ecx, dword ptr [eax + 8]
        test   ecx, ecx
        je     loc_1_17A1
        mov    eax, ecx
        jmp    loc_1_1796
loc_1_17A1:
        mov    dword ptr [eax + 0xc], 0x343  ; -> loc_1_0343
        pop    eax
loc_1_17A9:
        test   word ptr [edi + 2], 4
        je     loc_1_17B7
        mov    ecx, dword ptr [edi + 0x1c]
        mov    dword ptr [eax + 0x24], ecx
loc_1_17B7:
        lea    esi, [edi + 4]
        lea    edi, [eax + 0xc]
        mov    ecx, 5
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [eax + 0x28], 0x51524953
        mov    esi, dword ptr [eax + 8]
        test   esi, esi
        jne    loc_1_17F7
loc_1_17D3:
        mov    esi, dword ptr [eax]
        mov    esi, dword ptr [esi]
        bt     word ptr [0x1e32], si         ; -> obj1:0x1E32
        jae    loc_1_17E7
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_1_17E7:
        popfd  
        mov    dword ptr [esp + 0x1c], eax
        mov    edi, dword ptr [esp]
        call   sub_1_1CD4
        popal  
        clc    
        ret    
loc_1_17F7:
        cmp    esi, 0x2268                   ; -> obj1:0x2268
        jb     loc_1_17E7
        cmp    esi, 0x24fc                   ; -> loc_1_24FC
        jbe    loc_1_17D3
        jmp    loc_1_17E7
loc_1_1809:
        popfd  
        popal  
        stc    
        ret    
        db  02Eh, 08Bh, 0C0h
VPICD_Set_Int_Request:
        push   edx
        push   esi
        pushfd 
        cli    
        mov    esi, ebx
        add    esi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    edx, dword ptr [eax]
        mov    edx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [eax + 4]
        or     dword ptr [esi + edx*4 + 0x20], eax
        pop    eax
        bts    word ptr [esi + 0x10], dx
        jb     loc_1_184E
        cmp    dword ptr [esi + 4], 0
        jne    loc_1_184E
        cmp    byte ptr [esi + 0x1e], 0
        jne    loc_1_184E
        mov    edx, ebx
        push   esi
        mov    esi, 0x750                    ; -> loc_1_0750
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    edx
        mov    dword ptr [edx + 4], esi
loc_1_184E:
        popfd  
        pop    esi
        pop    edx
        ret    
        db  08Bh, 0C0h
VPICD_Clear_Int_Request:
        push   edx
        push   esi
        pushfd 
        cli    
        mov    esi, ebx
        add    esi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    edx, dword ptr [eax]
        mov    edx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [eax + 4]
        not    eax
        and    dword ptr [esi + edx*4 + 0x20], eax
        pop    eax
        jne    loc_1_1896
        btr    word ptr [esi + 0x10], dx
        jae    loc_1_1896
        cmp    dword ptr [esi + 4], 0
        jne    loc_1_1896
        cmp    byte ptr [esi + 0x1e], 0
        jne    loc_1_1896
        mov    edx, ebx
        push   esi
        mov    esi, 0x750                    ; -> loc_1_0750
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    edx
        mov    dword ptr [edx + 4], esi
loc_1_1896:
        popfd  
        pop    esi
        pop    edx
        ret    
        db  08Bh, 0C0h
VPICD_Get_Complete_Status:
        push   eax
        push   ebx
        xor    ecx, ecx
        cmp    dword ptr [eax + 0x28], 0x51524953
        jne    loc_1_1944
        cmp    dword ptr [ebx + 0x10], 0x62634d56
        jne    loc_1_1944
        push   eax
        push   edx
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_18C7
        inc    ecx
loc_1_18C7:
        cmp    dword ptr [eax + 8], ebx
        jne    loc_1_18CE
        mov    cl, 2
loc_1_18CE:
        add    ebx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    eax, dword ptr [eax]
        add    ecx, ecx
        test   dword ptr [ebx + eax*4 + 0x20], edx
        je     loc_1_18DF
        inc    ecx
loc_1_18DF:
        pop    edx
        pop    eax
        VxDCall  VPICD.VPICD_Test_Phys_Request   ; cd2007000300
        adc    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        bt     word ptr [ebx + 0x10], ax
        adc    ecx, ecx
        bt     word ptr [ebx + 0xc], ax
        adc    ecx, ecx
        bt     word ptr [0x1e32], ax         ; -> obj1:0x1E32
        adc    ecx, ecx
        bt     word ptr [0x1e30], ax         ; -> obj1:0x1E30
        adc    ecx, ecx
        jmp    loc_1_1934
        db  02Eh, 08Bh, 0C0h
VPICD_Get_Status:
        push   eax
        push   ebx
        xor    ecx, ecx
        cmp    dword ptr [eax + 0x28], 0x51524953
        jne    loc_1_1944
        cmp    dword ptr [ebx + 0x10], 0x62634d56
        jne    loc_1_1944
        mov    eax, dword ptr [eax]
        add    ebx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    eax, dword ptr [eax]
loc_1_1934:
        bt     word ptr [ebx + 0x12], ax
        adc    ecx, ecx
        cmp    word ptr [ebx + eax*2 + 0x60], 1
        cmc    
        adc    ecx, ecx
loc_1_1944:
        pop    ebx
        pop    eax
        ret    
VPICD_Test_Phys_Request:
        push   eax
        push   edx
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax]
        mov    al, 0xa
        cmp    edx, 8
        jb     loc_1_195E
        out    0xa0, al
        jmp    loc_1_1958
loc_1_1958:
        in     al, 0xa0
        mov    ah, al
        jmp    loc_1_1964
loc_1_195E:
        out    0x20, al
        jmp    loc_1_1962
loc_1_1962:
        in     al, 0x20
loc_1_1964:
        bt     eax, edx
        pop    edx
        pop    eax
        ret    
        db  08Bh, 0C0h
VPICD_Phys_EOI:
        push   eax
        pushfd 
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        cli    
        test   al, al
        je     loc_1_199E
        not    al
        and    byte ptr [0x1e32], al         ; -> obj1:0x1E32
        mov    al, byte ptr [0x1e32]         ; -> obj1:0x1E32
        or     al, byte ptr [0x1e30]         ; -> obj1:0x1E30
        cmp    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        je     loc_1_199B
        mov    byte ptr [0x1e3a], al         ; -> obj1:0x1E3A
        out    0x21, al
loc_1_199B:
        popfd  
        pop    eax
        ret    
loc_1_199E:
        not    ah
        and    byte ptr [0x1e33], ah         ; -> obj1:0x1E33
        mov    al, byte ptr [0x1e33]         ; -> obj1:0x1E33
        or     al, byte ptr [0x1e31]         ; -> obj1:0x1E31
        cmp    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        je     loc_1_19C0
        mov    byte ptr [0x1e3b], al         ; -> obj1:0x1E3B
        out    0xa1, al
loc_1_19C0:
        popfd  
        pop    eax
        ret    
VPICD_Convert_IRQ_To_Int:
        cmp    dword ptr [ebx + 0x10], 0x62634d56
        jne    loc_1_19E6
        cmp    eax, 0xf
        ja     loc_1_19E6
        push   ecx
        mov    ecx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        btr    eax, 3
        adc    ecx, 0
        add    al, byte ptr [ecx + ebx + 0x14]
        pop    ecx
        clc    
        ret    
loc_1_19E6:
        stc    
        ret    
VPICD_Convert_Int_To_IRQ:
        cmp    dword ptr [ebx + 0x10], 0x62634d56
        jne    loc_1_1A1E
        cmp    eax, 0xff
        ja     loc_1_1A1E
        push   ecx
        push   ebx
        push   edx
        add    ebx, dword ptr [0x25a8]       ; -> obj1:0x25A8
        mov    dx, word ptr [ebx + 0x14]
        xor    ecx, ecx
        call   sub_1_1A20
        jae    loc_1_1A1A
        mov    ecx, 8
        mov    dl, dh
        call   sub_1_1A20
loc_1_1A1A:
        pop    edx
        pop    ebx
        pop    ecx
        ret    
loc_1_1A1E:
        stc    
        ret    
sub_1_1A20:
        sub    al, dl
        jb     loc_1_1A2C
        cmp    al, 7
        ja     loc_1_1A2C
        add    eax, ecx
        clc    
        ret    
loc_1_1A2C:
        add    al, dl
        stc    
        ret    
VPICD_Get_IRQ_Complete_Status:
        cmp    eax, 0xf
        ja     loc_1_1A5E
        push   eax
        mov    eax, dword ptr [eax*4 + 0x2568] ; -> obj1:0x2568
        push   eax
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [eax + 0x18]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        and    ecx, 0xffffff7f
        pop    ebx
        pop    eax
        bt     word ptr [eax + 6], 2
        pop    eax
        ret    
loc_1_1A5E:
        xor    ecx, ecx
        ret    
VPICD_Convert_Handle_To_IRQ:
        cmp    dword ptr [eax + 0x28], 0x51524953
        jne    loc_1_1A6F
        mov    esi, dword ptr [eax]
        mov    esi, dword ptr [esi]
        ret    
loc_1_1A6F:
        or     esi, 0xffffffff
        ret    
        db  090h
VPICD_Physically_Mask:
        push   eax
        push   edx
        pushfd 
        cli    
        mov    eax, dword ptr [eax]
        and    word ptr [eax + 6], 0xfff7
        mov    dx, word ptr [eax + 4]
        or     word ptr [0x1e30], dx         ; -> obj1:0x1E30
        mov    ax, word ptr [0x1e30]         ; -> obj1:0x1E30
        or     ax, word ptr [0x1e32]         ; -> obj1:0x1E32
        cmp    ax, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        je     loc_1_1ABD
        mov    dx, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        mov    word ptr [0x1e3a], ax         ; -> obj1:0x1E3A
        cmp    al, dl
        je     loc_1_1AB9
        out    0x21, al
        cmp    ah, dh
        je     loc_1_1ABD
        jmp    loc_1_1AB9
loc_1_1AB9:
        mov    al, ah
        out    0xa1, al
loc_1_1ABD:
        popfd  
        pop    edx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h
VPICD_Physically_Unmask:
        push   eax
        push   edx
        pushfd 
        cli    
        mov    eax, dword ptr [eax]
        and    word ptr [eax + 6], 0xfff7
        mov    dx, word ptr [eax + 4]
        not    edx
        and    word ptr [0x1e30], dx         ; -> obj1:0x1E30
        mov    ax, word ptr [0x1e30]         ; -> obj1:0x1E30
        or     ax, word ptr [0x1e32]         ; -> obj1:0x1E32
        cmp    ax, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        je     loc_1_1B0F
        mov    dx, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        mov    word ptr [0x1e3a], ax         ; -> obj1:0x1E3A
        cmp    al, dl
        je     loc_1_1B0B
        out    0x21, al
        cmp    ah, dh
        je     loc_1_1B0F
        jmp    loc_1_1B0B
loc_1_1B0B:
        mov    al, ah
        out    0xa1, al
loc_1_1B0F:
        popfd  
        pop    edx
        pop    eax
        ret    
        db  090h
VPICD_Set_Auto_Masking:
        push   eax
        push   edx
        pushfd 
        cli    
        mov    eax, dword ptr [eax]
        or     word ptr [eax + 6], 8
        mov    dx, word ptr [eax + 4]
        cmp    word ptr [eax + 0x16], 0
        je     loc_1_1B35
        not    edx
        and    word ptr [0x1e30], dx         ; -> obj1:0x1E30
        jmp    loc_1_1B3C
loc_1_1B35:
        or     word ptr [0x1e30], dx         ; -> obj1:0x1E30
loc_1_1B3C:
        mov    ax, word ptr [0x1e30]         ; -> obj1:0x1E30
        or     ax, word ptr [0x1e32]         ; -> obj1:0x1E32
        cmp    ax, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        je     loc_1_1B6F
        mov    dx, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        mov    word ptr [0x1e3a], ax         ; -> obj1:0x1E3A
        cmp    al, dl
        je     loc_1_1B6B
        out    0x21, al
        cmp    ah, dh
        je     loc_1_1B6F
        jmp    loc_1_1B6B
loc_1_1B6B:
        mov    al, ah
        out    0xa1, al
loc_1_1B6F:
        popfd  
        pop    edx
        pop    eax
        ret    
VPICD_Call_When_Hw_Int:
        xchg   dword ptr [0x1e3c], esi       ; -> loc_1_1E3C
loc_1_1B79:
        ret    
VPICD_Auto_Mask_At_Inst_Swap:
        cmp    dword ptr [eax + 0x28], 0x51524953
        jne    loc_1_1B9E
        push   edx
        mov    edx, dword ptr [eax]
        and    word ptr [edx + 6], 0xfff7
        mov    edx, dword ptr [edx]
        bts    word ptr [0x1e38], dx         ; -> obj1:0x1E38
        pop    edx
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        clc    
        ret    
loc_1_1B9E:
        stc    
        ret    
VPICD_Begin_Inst_Page_Swap:
        cli    
        push   eax
        mov    ax, word ptr [0x1e38]         ; -> obj1:0x1E38
        or     word ptr [0x1e30], ax         ; -> obj1:0x1E30
        jmp    loc_1_1BC5
        db  02Eh, 08Bh, 0C0h
VPICD_End_Inst_Page_Swap:
        cli    
        push   eax
        mov    ax, word ptr [0x1e38]         ; -> obj1:0x1E38
        not    eax
        and    word ptr [0x1e30], ax         ; -> obj1:0x1E30
loc_1_1BC5:
        push   edx
        mov    ax, word ptr [0x1e30]         ; -> obj1:0x1E30
        or     ax, word ptr [0x1e32]         ; -> obj1:0x1E32
        cmp    ax, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        je     loc_1_1BF9
        mov    dx, word ptr [0x1e3a]         ; -> obj1:0x1E3A
        mov    word ptr [0x1e3a], ax         ; -> obj1:0x1E3A
        cmp    al, dl
        je     loc_1_1BF5
        out    0x21, al
        cmp    ah, dh
        je     loc_1_1BF9
        jmp    loc_1_1BF5
loc_1_1BF5:
        mov    al, ah
        out    0xa1, al
loc_1_1BF9:
        pop    edx
        pop    eax
        sti    
        ret    
VPICD_VM_SlavePIC_Mask_Change:
        ret    
        db  000h, 000h, 000h, 000h
loc_1_1C02:
        push   eax
        mov    eax, dword ptr [0x1bfe]       ; -> obj1:0x1BFE
        cmp    eax, 8
        jb     loc_1_1C16
        sub    eax, 8
        or     al, 0x60
        out    0xa0, al
        mov    al, 2
loc_1_1C16:
        or     al, 0x60
        out    0x20, al
        mov    al, 0xff
        out    0xa1, al
        out    0x21, al
        pop    eax
        iretd  
sub_1_1C22:
        push   ebx
        push   esi
        sub    esp, 8
        sidt   [esp + 2]
        pop    ebx
        pop    ebx
        mov    dword ptr [0x1bfe], eax       ; -> obj1:0x1BFE
        shl    eax, 3
        add    ebx, eax
        add    ebx, 0x280
        mov    esi, 0x1c02                   ; -> loc_1_1C02
        xchg   word ptr [ebx], si
        ror    esi, 0x10
        xchg   word ptr [ebx + 6], si
        ror    esi, 0x10
        sti    
        nop    
        nop    
        nop    
        cli    
        mov    word ptr [ebx], si
        ror    esi, 0x10
        mov    word ptr [ebx + 6], si
        pop    esi
        pop    ebx
        ret    
VPICD_Service_25:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    bx, word ptr [ebp + 8]
        or     bx, bx
        je     loc_1_1CCE
        pushfd 
        cli    
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        push   eax
        mov    al, 0xff
        out    0xa1, al
        mov    ecx, 1
loc_1_1C84:
        test   cl, bl
        je     loc_1_1C9A
        mov    al, cl
        not    al
        out    0x21, al
        bsf    eax, ecx
        call   sub_1_1C22
        mov    al, 0xff
        out    0x21, al
loc_1_1C9A:
        shl    cl, 1
        jne    loc_1_1C84
        mov    bl, bh
        mov    cl, 1
loc_1_1CA2:
        test   cl, bl
        je     loc_1_1CC2
        mov    al, 0xfb
        out    0x21, al
        mov    al, cl
        not    al
        out    0xa1, al
        xchg   ch, cl
        bsf    eax, ecx
        call   sub_1_1C22
        xchg   ch, cl
        mov    al, 0xff
        out    0xa1, al
        out    0x21, al
loc_1_1CC2:
        shl    cl, 1
        jne    loc_1_1CA2
        pop    eax
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        popfd  
loc_1_1CCE:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        leave  
        ret    
sub_1_1CD4:
        movzx  eax, word ptr [edi]
        bts    dword ptr [0x26fa], eax       ; -> loc_1_26FA
        jb     loc_1_1CFA
        inc    byte ptr [eax + 0x26fe]       ; -> loc_1_26FE
        test   byte ptr [edi + 2], 2
        je     loc_1_1CF3
        bts    dword ptr [0x26fc], eax       ; -> loc_1_26FC
loc_1_1CF3:
        bts    dword ptr [0x2712], eax       ; -> loc_1_2712
loc_1_1CFA:
        ret    
sub_1_1CFB:
        cmp    edi, 0x10
        jae    loc_1_1D09
        bt     dword ptr [0x26fa], edi       ; -> loc_1_26FA
        jae    loc_1_1D0A
loc_1_1D09:
        ret    
loc_1_1D0A:
        bts    dword ptr [0x2714], edi       ; -> loc_1_2714
        jb     loc_1_1D09
        or     byte ptr [0x26f8], 1          ; -> obj1:0x26F8
        ret    
sub_1_1D1B:
        pushal 
        mov    ecx, 0x10
        mov    eax, edi
        mov    edi, 0x2568                   ; -> obj1:0x2568
        cld    
        repne scasd eax, dword ptr es:[edi]
        jne    loc_1_1D53
        neg    ecx
        add    ecx, 0xf
        bt     dword ptr [0x2712], ecx       ; -> loc_1_2712
        jae    loc_1_1D53
        dec    byte ptr [ecx + 0x26fe]       ; -> loc_1_26FE
        jne    loc_1_1D53
        btr    word ptr [0x26fa], cx         ; -> loc_1_26FA
        btr    word ptr [0x26fc], cx         ; -> loc_1_26FC
loc_1_1D53:
        popal  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1D6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  VPICD_Get_Version        ; fixup
        dd  VPICD_Virtualize_IRQ     ; fixup
        dd  VPICD_Set_Int_Request    ; fixup
        dd  VPICD_Clear_Int_Request  ; fixup
        dd  VPICD_Phys_EOI           ; fixup
        dd  VPICD_Get_Complete_Status ; fixup
        dd  VPICD_Get_Status         ; fixup
        dd  VPICD_Test_Phys_Request  ; fixup
        dd  VPICD_Physically_Mask    ; fixup
        dd  VPICD_Physically_Unmask  ; fixup
        dd  VPICD_Set_Auto_Masking   ; fixup
        dd  VPICD_Get_IRQ_Complete_Status ; fixup
        dd  VPICD_Convert_Handle_To_IRQ ; fixup
        dd  VPICD_Convert_IRQ_To_Int ; fixup
        dd  VPICD_Convert_Int_To_IRQ ; fixup
        dd  VPICD_Call_When_Hw_Int   ; fixup
        dd  VPICD_Force_Default_Owner ; fixup
        dd  VPICD_Force_Default_Behavior ; fixup
        dd  VPICD_Auto_Mask_At_Inst_Swap ; fixup
        dd  VPICD_Begin_Inst_Page_Swap ; fixup
        dd  VPICD_End_Inst_Page_Swap ; fixup
        dd  VPICD_Virtual_EOI        ; fixup
        dd  VPICD_Get_Virtualization_Count ; fixup
        dd  VPICD_Post_Sys_Critical_Init ; fixup
        dd  VPICD_VM_SlavePIC_Mask_Change ; fixup
        dd  VPICD_Service_25         ; fixup
        dd  VPICD_Service_26         ; fixup
        dd  VPICD_Service_27         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00003h                      ; DDB_Req_Device_Number
        db  4, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VPICD   '                  ; DDB_Name
        dd  00C000000h                  ; DDB_Init_Order
        dd  VPICD_Control            ; DDB_Control_Proc
        dd  VPICD_PM_API             ; DDB_V86_API_Proc
        dd  VPICD_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x1D70              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1E3C:
        jns    0x1e59
        add    byte ptr [eax], al
loc_1_1E40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E44:
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
loc_1_1E64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        push   0xf4000022
        add    dword ptr [eax], eax
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
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x1f40000], dl ; -> loc_1_2294
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
loc_1_1EE8:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        shl    byte ptr [edx], 0
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    ah, ch                        ; -> loc_1_22EC
        and    al, byte ptr [eax]
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sbb    byte ptr [ebx], ah
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
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
        add    byte ptr [0x20000000], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al            ; -> loc_1_2344
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        jo     loc_1_2025
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
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
loc_1_2025:
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x800], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x1f40000], bl ; -> loc_1_239C
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
sub_1_2068:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        enter  0x23, 0
        hlt    
        add    dword ptr [eax], eax
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
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    ah, dh                        ; -> loc_1_23F4
        and    eax, dword ptr [eax]
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah            ; -> loc_1_2420
        and    al, 0
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [esp], cl            ; -> loc_1_244C
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        js     0x21a6
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
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
        add    byte ptr [0], cl
        and    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [esp + 0x1f40000], ah ; -> loc_1_24A4
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
loc_1_21E8:
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        shl    byte ptr [eax + eax], 1
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        cld    
        and    al, 0
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [eax + 0x1e], ch     ; -> loc_1_1E68
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_2278
        add    byte ptr [eax], al
loc_1_2278:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2294:
        test   al, 0x1e
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_22A4
        add    byte ptr [eax], al
loc_1_22A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_22C0:
        call   0x10022e3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_22EC:
        sub    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_22FC
        add    byte ptr [eax], al
loc_1_22FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2318:
        push   0x100001f
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2344:
        test   al, 0x1f
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_2354
        add    byte ptr [eax], al
loc_1_2354:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2370:
        call   0x1002394
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_239C:
        sub    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_23AC
        add    byte ptr [eax], al
loc_1_23AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_23C8:
        push   0x1000020
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_23F4:
        test   al, 0x20
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2400:
        loopne loc_1_2404
        add    byte ptr [eax], al
loc_1_2404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2408:
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2420:
        call   0x1002445
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_244C:
        sub    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_245C
        add    byte ptr [eax], al
loc_1_245C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2478:
        push   0x1000021
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_24A4:
        test   al, 0x21
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_24B4
        add    byte ptr [eax], al
loc_1_24B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_24D0:
        call   0x10024f6
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_1_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> loc_1_032C
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_24FC:
        sub    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_250C
        add    byte ptr [eax], al
loc_1_250C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        add    byte ptr [esp + eax], dl      ; -> loc_1_0454
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        push   edx
        push   ecx
loc_1_2528:
        push   0xa800001e                    ; -> obj1:0x1EA8
        push   ds
        add    byte ptr [eax], al
        call   sub_1_2068
        and    byte ptr [eax], al
        add    byte ptr [eax - 0x17ffffe0], ch ; -> obj1:0x20A8
        and    byte ptr [eax], al
        add    byte ptr [eax], ch            ; -> obj1:0x2128
        and    dword ptr [eax], eax
        add    byte ptr [eax + 0x21], ch     ; -> obj1:0x2168
        add    byte ptr [eax], al
        test   al, 0x21
        add    byte ptr [eax], al
        call   0x28002572                    ; -> obj1:0x2228
        and    al, byte ptr [eax]
        add    byte ptr [eax], ch            ; -> obj1:0x1F28
        pop    ds
        add    byte ptr [eax], al
        push   0xa800001f                    ; -> obj1:0x1FA8
        pop    ds
        add    byte ptr [eax], al
        call   0x28002584                    ; -> obj1:0x2028
        and    byte ptr [eax], al
        add    byte ptr [eax + 0x1e], ch     ; -> loc_1_1E68
        add    byte ptr [eax], al
        test   al, 0x1e
        add    byte ptr [eax], al
        call   0x28002593                    ; -> obj1:0x1F28
        pop    ds
        add    byte ptr [eax], al
        push   0xa800001f                    ; -> obj1:0x1FA8
        pop    ds
        add    byte ptr [eax], al
        call   0x280025a4                    ; -> obj1:0x2028
        and    byte ptr [eax], al
        add    byte ptr [eax + 0x20], ch     ; -> sub_1_2068
        add    byte ptr [eax], al
        test   al, 0x20
        add    byte ptr [eax], al
        call   0x280025b5                    ; -> obj1:0x2128
        and    dword ptr [eax], eax
        add    byte ptr [eax + 0x21], ch     ; -> obj1:0x2168
        add    byte ptr [eax], al
        test   al, 0x21
        add    byte ptr [eax], al
        call   0x280025c6                    ; -> obj1:0x2228
        and    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [edi + 0x11], al     ; -> loc_1_1147
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    esp, 0x50000002               ; -> loc_1_0E50
        push   cs
        add    byte ptr [eax], al
        inc    edi
        adc    dword ptr [eax], eax
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        db  000h
loc_1_25F4:
        clc    
        fiadd  word ptr [eax]
        add    byte ptr [eax - 0x4ffffff2], al ; -> loc_1_0E80
        push   cs
        add    byte ptr [eax], al
        loopne loc_1_2610
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        inc    eax
        sldt   word ptr [eax]
        jo     loc_1_261D
        add    byte ptr [eax], al
loc_1_2610:
        mov    al, 0xf
        add    byte ptr [eax], al
        loopne 0x2625
        add    byte ptr [eax], al
        adc    byte ptr [eax], dl
        add    byte ptr [eax], al
        inc    eax
loc_1_261D:
        adc    byte ptr [eax], al
        add    byte ptr [eax + 0x10], dh     ; -> loc_1_1070
        add    byte ptr [eax], al
        mov    al, byte ptr [0xd0000010]     ; -> loc_1_10D0
        adc    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_1100
        adc    dword ptr [eax], eax
        add    byte ptr [eax + 0x11], al     ; -> loc_1_1140
        add    byte ptr [eax], al
loc_1_2634:
        xchg   ecx, eax
        db  0FFh, 0FFh, 0FFh, 061h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 031h, 0FFh, 0FFh, 0FFh, 001h
        db  0FFh, 0FFh, 0FFh, 0D1h, 0FEh, 0FFh, 0FFh, 0A1h, 0FEh, 0FFh, 0FFh, 061h, 0FEh, 0FFh, 0FFh, 02Bh
        db  0FEh, 0FFh, 0FFh, 0FBh, 0FDh, 0FFh, 0FFh, 0CBh, 0FDh, 0FFh, 0FFh, 09Bh, 0FDh, 0FFh, 0FFh, 06Bh
        db  0FDh, 0FFh, 0FFh, 03Bh, 0FDh, 0FFh, 0FFh, 00Bh, 0FDh, 0FFh, 0FFh, 0CBh, 0FCh, 0FFh, 0FFh
sub_1_2674:
        mov    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        xchg   edx, eax
        or     al, byte ptr [eax]
        add    byte ptr [edx + ecx + 0xaa50000], cl ; -> loc_1_0A8C
        add    byte ptr [eax], al
loc_1_2684:
        inc    esi
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    ah, ah                        ; -> loc_1_12E4
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x13], ah     ; -> loc_1_1360
        add    byte ptr [eax], al
        push   0x70000013                    ; -> loc_1_1370
        adc    eax, dword ptr [eax]
        add    byte ptr [eax + 0x13], bh     ; -> loc_1_1378
        add    byte ptr [eax], al
        mov    byte ptr [ebx], dl
        add    byte ptr [eax], al
        nop    
        adc    eax, dword ptr [eax]
        add    byte ptr [ebx + edx + 0x15d20000], bl ; -> loc_1_139C
        add    byte ptr [eax], al
        ljmp   0x15:0xf3000015               ; -> loc_1_15F3
        add    byte ptr [esi], al            ; -> loc_1_1606
        push   ss
        add    byte ptr [eax], al
        db  0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh
        dd  loc_1_0E16               ; fixup
        dd  loc_1_0E10               ; fixup
        db  000h, 000h
loc_1_26FA:
        add    al, 0x20
loc_1_26FC:
        add    byte ptr [eax], al
loc_1_26FE:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_1_270E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2712:
        add    al, 0x20
loc_1_2714:
        test   byte ptr [eax], ah
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x15f  R-X
loc_2_0000:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     dword ptr [ecx], eax
        dec    ebx
        add    dword ptr [eax], eax
        db  000h
loc_2_0014:
        push   esp
        push   0x61207369
        jo     0x8c
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [ecx + 0x74], ah
        je     loc_2_008E
        insd   dword ptr es:[edi], dx
        jo     0xa0
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        popal  
        arpl   word ptr [ebx + 0x65], sp
        jae    0xab
        and    byte ptr [ecx + 0x20], ah
        jbe    0xa8
        arpl   word ptr [ebp + 0x20], sp
        je     loc_2_00AC
        popal  
        je     0x67
        imul   esi, dword ptr [ebx + 0x20], 0x6e696562
        and    byte ptr [di + 0x73], dh
        and    byte ptr fs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xc5
        jb     loc_2_0080
        popal  
        jo     loc_2_00D3
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr cs:[ecx + 0x66], cl
        and    byte ptr [ecx + 0x6f], bh
        jne    0x94
        arpl   word ptr [edi + 0x6e], bp
        je     0xe2
        outsb  dx, byte ptr [esi]
        jne    0xe1
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
loc_2_0080:
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x69687420
        jae    0xaa
        popal  
        jo     0xfd
        insb   byte ptr es:[edi], dx
loc_2_008E:
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [ecx + 0x74], ch
        and    byte ptr [ebp + 0x69], ch
        push   0x61632074
        jne    0x116
        and    byte ptr gs:[ecx + 0x6f], bh
        jne    0x11b
        and    byte ptr [ebx + 0x79], dh
loc_2_00AC:
        jae    0x122
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
        jne    0x12b
        or     eax, 0x79540a0a
        jo     0x128
        and    byte ptr [ecx + 0x20], bl
        outsd  dx, dword ptr [esi]
        jb     0xe9
        jo     0x13d
        jae    0x141
        and    byte ptr [ebp + 0x4e], al
        push   esp
        inc    ebp
loc_2_00D3:
        push   edx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        arpl   word ptr [edi + 0x6e], bp
        je     loc_2_0146
        outsb  dx, byte ptr [esi]
        jne    loc_2_0145
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x69687420
        jae    0x10e
        popal  
        jo     0x161
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        or     eax, 0x7079540a
        and    byte ptr gs:[esi + 0x20], cl
        je     0x174
        and    byte ptr [ebp + 0x6e], ah
        and    byte ptr fs:[eax + ebp*2 + 0x69], dh
        jae    0x12f
        popal  
        jo     0x182
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[eax], al
        inc    esp
        jbe    0x18a
        arpl   word ptr [ebp + 0x20], sp
        arpl   word ptr [edi + 0x6e], bp
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x7551202e
        imul   esi, dword ptr [eax + 0x61], 0x61206c6c
        jo     0x1aa
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    0x164
        popal  
loc_2_0145:
        outsb  dx, byte ptr [esi]
loc_2_0146:
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + 0x65], dh
        jae    0x1c5
        popal  
        jb     0x1c8
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x1cf
        add    byte ptr cs:[eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x40  R-X
        db  001h, 000h, 000h, 000h
loc_3_0004:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
loc_3_0010:
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0x80
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_3_003A
        imul   ebp, dword ptr [esi + 0x69], 0x6c616974
        imul   edi, dword ptr [edx + 0x65], 0x65687420
        and    byte ptr [ecx + 0x6e], ch
        je     0x92
        jb     0xa1
        jne    0xa1
        je     0x53
        arpl   word ptr [edi + 0x6e], bp
        je     0xaa
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_3_003A:
        insb   byte ptr es:[edi], dx
        jb     0x6c
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x5b8  R-X
loc_4_0000:
        mov    ax, word ptr [0x26fa]         ; -> loc_1_26FA
        push   0x428                         ; -> obj6:0x0428
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        call   sub_4_03F3
        mov    ax, word ptr [0x2712]         ; -> loc_1_2712
        push   0x440                         ; -> obj6:0x0440
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        call   sub_4_03F3
        mov    ax, word ptr [0x26fc]         ; -> loc_1_26FC
        push   0x458                         ; -> obj6:0x0458
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        call   sub_4_03F3
        mov    ax, word ptr [0x270e]         ; -> loc_1_270E
        push   0x470                         ; -> obj6:0x0470
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        call   sub_4_03F3
        mov    ax, word ptr [0x2714]         ; -> loc_1_2714
        push   0x488                         ; -> obj6:0x0488
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    sub_4_03F3
        db  000h, 000h, 00Dh, 00Ah, 02Ah, 02Ah, 02Ah, 056h, 050h, 049h, 043h, 044h, 020h, 044h, 045h, 042h
        db  055h, 047h, 020h, 049h, 04Eh, 046h, 04Fh, 052h, 04Dh, 041h, 054h, 04Fh, 04Eh, 020h, 02Ah, 02Ah
        db  02Ah, 00Dh, 00Ah, 00Dh, 00Ah, 05Bh, 031h, 05Dh, 020h, 047h, 06Ch, 06Fh, 062h, 061h, 06Ch, 020h
        db  050h, 049h, 043h, 020h, 069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 00Dh
        db  00Ah, 05Bh, 032h, 05Dh, 020h, 050h, 065h, 072h, 02Dh, 056h, 04Dh, 020h, 050h, 049h, 043h, 020h
        db  069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 00Dh, 00Ah, 05Bh, 033h, 05Dh
        db  020h, 049h, 052h, 051h, 020h, 068h, 061h, 06Eh, 064h, 06Ch, 065h, 072h, 020h, 069h, 06Eh, 066h
        db  06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 00Dh, 00Ah, 00Dh, 00Ah, 03Eh, 000h, 00Dh, 00Ah
        db  00Dh, 00Ah, 000h, 0FCh, 064h, 0ACh, 03Ch, 020h, 074h, 0FAh, 084h, 0C0h, 074h, 004h, 03Ch, 03Bh
        db  075h, 024h, 068h
        dd  obj6:0x0070              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0C4h, 000h, 001h, 000h, 074h, 030h, 0CDh, 020h
        db  0C3h, 000h, 001h, 000h, 068h
        dd  obj6:0x00EE              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 02Ch, 031h, 072h, 01Bh, 074h, 01Ah, 048h, 00Fh, 084h, 0FAh
        db  000h, 000h, 000h, 048h, 00Fh, 084h, 024h, 002h, 000h, 000h, 068h
        dd  obj6:0x04A0              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0C3h, 0E8h, 093h, 002h, 000h, 000h, 068h
        dd  obj6:0x04BE              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h
        dd  obj1:0x1E32              ; fixup
        db  0E8h, 094h, 002h, 000h, 000h, 068h
        dd  obj6:0x04D6              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h
        dd  obj1:0x1E30              ; fixup
        db  0E8h, 07Eh, 002h, 000h, 000h, 068h
        dd  obj6:0x04EE              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0B0h, 00Ah, 0E6h, 0A0h, 0EBh, 000h, 0E4h, 0A0h, 08Ah, 0E0h
        db  0B0h, 00Ah, 0E6h, 020h, 0EBh, 000h, 0E4h, 020h, 0E8h, 05Ch, 002h, 000h, 000h, 033h, 0C0h, 033h
        db  0DBh, 033h, 0D2h, 0BEh
        dd  loc_1_1E68               ; fixup
        db  033h, 0C9h, 083h, 07Eh, 008h, 000h, 075h, 003h, 00Fh, 0ABh, 0CBh, 0F6h, 046h, 006h, 004h, 074h
        db  003h, 00Fh, 0ABh, 0C8h, 0F6h, 046h, 006h, 002h, 074h, 000h, 083h, 0C6h, 040h, 041h, 083h, 0F9h
        db  00Fh, 076h, 0DFh, 068h
        dd  obj6:0x0506              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E8h, 01Eh, 002h, 000h, 000h, 068h
        dd  obj6:0x051E              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 0C3h, 0E8h, 00Ch, 002h, 000h, 000h, 068h
        dd  obj6:0x0536              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 0C2h, 0E8h, 0FAh, 001h, 000h, 000h, 0E8h, 002h, 0FEh
        db  0FFh, 0FFh, 068h
        dd  obj6:0x054E              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h
        dd  obj1:0x1E36              ; fixup
        db  0E8h, 0DFh, 001h, 000h, 000h, 068h
        dd  obj6:0x0566              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h
        dd  obj1:0x1E38              ; fixup
        db  0E8h, 0C9h, 001h, 000h, 000h, 0C3h, 0CDh, 020h, 003h, 000h, 001h, 000h, 0EBh, 022h
loc_4_0233:
        push   0x57e                         ; -> obj6:0x057E
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        je     loc_4_0350
        push   0xec                          ; -> obj6:0x00EC
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    edi, dword ptr [0x25a8]       ; -> obj1:0x25A8
        add    edi, ebx
        mov    eax, dword ptr [ebx + 0xc]
        push   0x5ab                         ; -> loc_6_05AB
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        movzx  ecx, word ptr [edi + 0x16]
        jecxz  loc_4_027C
        push   0x5da                         ; -> obj6:0x05DA
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_027C:
        mov    ecx, dword ptr [edi]
        jecxz  loc_4_028B
        push   0x5ff                         ; -> loc_6_05FF
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_028B:
        cmp    dword ptr [edi + 4], 0
        je     loc_4_029C
        push   0x617                         ; -> loc_6_0617
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_029C:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_4_02AD
        push   0x637                         ; -> loc_6_0637
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_02AD:
        mov    ax, word ptr [edi + 0x14]
        cmp    ax, 0x7008
        je     loc_4_02CD
        push   0x65c                         ; -> obj6:0x065C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x690                         ; -> obj6:0x0690
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_02CD:
        push   0xee                          ; -> obj6:0x00EE
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        call   sub_4_03DC
        push   0x6c4                         ; -> obj6:0x06C4
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    ax, word ptr [edi + 0x12]
VPICD_Post_Sys_Critical_Init:
        call   sub_4_03F3
        xor    ecx, ecx
        xor    eax, eax
loc_4_02F5:
        cmp    word ptr [edi + ecx*2 + 0x60], 0
        je     loc_4_0300
        bts    eax, ecx
loc_4_0300:
        inc    ecx
        cmp    ecx, 0xf
        jbe    loc_4_02F5
        push   0x6dc                         ; -> obj6:0x06DC
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        call   sub_4_03F3
        push   0x6f4                         ; -> obj6:0x06F4
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    ax, word ptr [edi + 0x10]
        call   sub_4_03F3
        push   0x70c                         ; -> obj6:0x070C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    ax, word ptr [edi + 0xc]
        call   sub_4_03F3
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_0233
loc_4_0350:
        push   0xee                          ; -> obj6:0x00EE
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        ret    
        db  0BEh
        dd  loc_1_1E68               ; fixup
        db  033h, 0EDh, 08Bh, 07Eh, 018h, 0EBh, 066h, 081h, 07Fh, 010h
        dd  loc_1_0492               ; fixup
        db  074h, 05Ah, 08Bh, 0C5h, 0CDh, 020h, 0C6h, 000h, 001h, 000h, 08Bh, 04Fh, 00Ch, 08Bh, 057h, 024h
        db  068h
        dd  obj6:0x0724              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 04Fh, 010h, 08Bh, 057h, 014h, 068h
        dd  obj6:0x074E              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 04Fh, 018h, 08Bh, 057h, 01Ch, 068h
        dd  obj6:0x077A              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 081h, 07Fh, 028h, 053h, 049h, 052h, 051h, 074h, 00Bh, 068h
        dd  obj6:0x07AA              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj6:0x00EE              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 07Fh, 008h, 085h, 0FFh, 075h, 096h, 083h, 0C6h, 040h
        db  045h, 083h, 0FDh, 00Fh, 076h, 088h, 0C3h
sub_4_03DC:
        push   0x7c4                         ; -> obj6:0x07C4
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x80e                         ; -> obj6:0x080E
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        ret    
sub_4_03F3:
        push   ecx
        xor    ecx, ecx
loc_4_03F6:
        bt     eax, ecx
        jb     loc_4_0408
        push   0x858                         ; -> obj6:0x0858
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_4_0413
loc_4_0408:
        push   0x85c                         ; -> obj6:0x085C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_0413:
        inc    ecx
        cmp    ecx, 0xf
        jbe    loc_4_03F6
        push   0x860                         ; -> obj6:0x0860
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        pop    ecx
        ret    
        db  000h, 000h, 041h, 06Ch, 06Ch, 06Fh, 063h, 027h, 064h, 020h, 062h, 079h, 020h, 061h, 072h, 062h
        db  069h, 074h, 072h, 061h, 074h, 06Fh, 072h, 03Ah, 020h, 000h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 050h, 072h
loc_4_044A:
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], al
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebx
        push   0x61657261
        bound  ebp, qword ptr [ebp + 0x3a]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    esp
        jbe    loc_4_04E1
        insb   byte ptr es:[edi], dx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edx + 0x65], dl
        jae    loc_4_04F9
        jb     0x50c
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        sub    ch, byte ptr [edx]
        and    byte ptr [ecx + 0x6e], ch
        jbe    0x508
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x56], 0x44434950
        and    byte ptr [ebx + 0x75], dh
        bound  esp, qword ptr [ebx + 0x6f]
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        or     eax, 0x6850000a
        jns    0x535
        imul   esp, dword ptr [ebx + 0x61], 0x20796c6c
        imul   ebp, dword ptr [esi + 0x20], 0x76726573
        imul   esp, dword ptr [ebx + 0x65], 0x5000203a
        push   0x63697379
loc_4_04DC:
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        jns    0x501
loc_4_04E1:
        insd   dword ptr es:[edi], dx
        popal  
        jae    loc_4_0550
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax + 0x68], dl
        jns    loc_4_0565
        imul   esp, dword ptr [ebx + 0x61], 0x20796c6c
loc_4_04F9:
        jb     loc_4_0560
        jno    loc_4_0572
        jae    0x574
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], al
        push   esi
        imul   esi, dword ptr [edx + 0x74], 0x696c6175
        jp     loc_4_0575
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edi + 0x6c], al
        outsd  dx, dword ptr [esi]
        bound  esp, qword ptr [ecx + 0x6c]
        insb   byte ptr es:[edi], dx
        jns    0x547
        outsd  dx, dword ptr [esi]
        ja     0x598
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
loc_4_0536:
        push   ebx
        push   0x64657261
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_0544:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_0548:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        dec    ebp
        popal  
loc_4_0550:
        jae    0x5bd
        and    byte ptr fs:[ecx + 0x74], ah
        and    byte ptr [edx + 0x6f], ah
        outsd  dx, dword ptr [esi]
        je     loc_4_057D
        je     0x5c8
        insd   dword ptr es:[edi], dx
loc_4_0560:
        cmp    ah, byte ptr gs:[eax]
        and    byte ptr [eax], ah
loc_4_0565:
        add    byte ptr [ecx + 0x75], al
        je     0x5d9
        and    byte ptr [ebp + 0x61], ch
        jae    0x5da
        and    byte ptr [ecx + 0x74], ah
loc_4_0572:
        and    byte ptr [ecx + 0x6e], ch
loc_4_0575:
        jae    0x5eb
        and    byte ptr [ebx + 0x77], dh
        popal  
        jo     0x5b7
loc_4_057D:
        add    byte ptr [eax + 0x72], dl
        jae    0x5f6
        and    byte ptr [ecx + 0x6e], ah
        jns    0x5a8
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x5fd
        and    byte ptr [ebx + 0x6f], ah
loc_4_0591:
        outsb  dx, byte ptr [esi]
        je     0x5fd
        outsb  dx, byte ptr [esi]
        jne    0x5fc
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ebx + 0x45], bl
        jae    0x602
        pop    ebp
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jno    0x61b
        imul   esi, dword ptr [edx + edi + 0x20], 0x72695600
        je     0x625
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x6e], ch
        je     0x61c
        db  072h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x58  RW-
        db  053h, 06Ah, 000h, 068h, 0C0h, 000h, 000h, 000h, 0CDh, 020h, 0A7h, 000h, 001h, 000h, 083h, 0C4h
        db  008h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 080h, 001h, 000h, 0A3h
        dd  obj1:0x25A8              ; fixup
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h, 085h, 0C0h, 075h, 002h, 0EBh, 0E9h, 0A3h
        dd  obj1:0x25B0              ; fixup
        db  0B8h, 005h, 000h, 000h, 000h
sub_5_0036:
        xor    ecx, ecx
        VMMCall  VMM.Register_PEF_Provider       ; cd205c010100
        jae    loc_5_0048
        mov    eax, dword ptr [0x25a8]       ; -> obj1:0x25A8
        lea    eax, [eax + 0x1f]
loc_5_0048:
        mov    dword ptr [0x25ac], eax       ; -> obj1:0x25AC
        mov    al, 0xa
        out    0x20, al
        jmp    loc_5_0053
loc_5_0053:
        out    0xa0, al
        jmp    0x57
        db  0E4h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x864  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_9_0023               ; fixup
        dd  loc_9_01FB               ; fixup
        dd  loc_9_02DD               ; fixup
        dd  loc_9_0316               ; fixup
        dd  loc_9_0326               ; fixup
        dd  loc_9_0355               ; fixup
        dd  loc_9_02CF               ; fixup
        db  013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_00F1:
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
loc_6_05AB:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_05FF:
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
loc_6_0617:
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
loc_6_0637:
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
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0xc  R-X
loc_7_0000:
        mov    dword ptr [0x25dc], 0         ; -> obj1:0x25DC
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x30  R-X
loc_8_0000:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_8_002E
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shl    eax, 8
        add    eax, dword ptr [0x25d4]       ; -> obj1:0x25D4
        mov    dword ptr [0x25d4], eax       ; -> obj1:0x25D4
        shl    eax, 4
        mov    dword ptr [0x25d8], eax       ; -> obj1:0x25D8
        add    eax, 0x3fc
        mov    dword ptr [0x25dc], eax       ; -> obj1:0x25DC
loc_8_002E:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x478  R-X
loc_9_0000:
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        mov    esi, dword ptr [esp + 0x10]
        cmp    esi, 7
        jae    loc_9_001F
        mov    ecx, dword ptr [esp + 0x1c]
        call   sub_5_0036
loc_9_001F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_9_0023:
        mov    ebx, dword ptr [ecx]
        call   sub_9_035B
        call   sub_9_01DA
        call   sub_9_03EE
        mov    edi, dword ptr [ebx + 0xc]
        call   sub_9_01AC
        clc    
        je     loc_9_004F
        push   -1
        call   sub_9_006C
        jae    loc_9_004F
        push   0
        call   sub_9_006C
loc_9_004F:
        mov    dword ptr [0x52], 0           ; -> obj5:0x0052
        jae    loc_9_0065
        mov    dword ptr [0x52], 0x13        ; -> obj5:0x0052
loc_9_0065:
        mov    eax, dword ptr [0x52]         ; -> obj5:0x0052
        ret    
        db  090h
sub_9_006C:
        enter  8, 0
        mov    eax, 0xffffffff
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], edx
loc_9_007E:
        mov    si, word ptr [edi + 4]
        and    dx, word ptr [0x2714]         ; -> loc_1_2714
        lea    ecx, [esi - 1]
        not    edx
        test   cx, si
        je     loc_9_0187
loc_9_0097:
        and    esi, edx
        test   word ptr [edi], 1
        movzx  eax, word ptr [4]             ; -> obj5:0x0004
        not    eax
        je     loc_9_00C4
        mov    cx, word ptr [0x18]           ; -> obj5:0x0018
        test   word ptr [edi], 2
        jne    loc_9_00BA
        not    cx
loc_9_00BA:
        and    cx, word ptr [6]              ; -> obj5:0x0006
        or     ax, cx
loc_9_00C4:
        and    esi, eax
        je     loc_9_0158
loc_9_00CC:
        mov    cl, 0xff
        push   esi
loc_9_00CF:
        bsr    ax, si
        je     loc_9_00F3
        movzx  eax, ax
        mov    ch, byte ptr [eax + 8]        ; -> obj5:0x0008
        sub    cl, ch
        sbb    dh, dh
        and    cl, dh
        sub    dl, al
        and    dl, dh
        add    dl, al
        add    cl, ch
        je     loc_9_00F3
        btr    esi, eax
        jmp    loc_9_00CF
loc_9_00F3:
        pop    esi
        mov    al, dl
        inc    byte ptr [eax + 8]            ; -> obj5:0x0008
        bts    word ptr [4], ax              ; -> obj5:0x0004
        test   word ptr [edi], 1
        je     loc_9_0113
        bts    word ptr [6], ax              ; -> obj5:0x0006
loc_9_0113:
        test   word ptr [edi], 2
        je     loc_9_0122
        bts    word ptr [0x18], ax           ; -> obj5:0x0018
loc_9_0122:
        pushal 
        call   sub_9_01AC
        je     loc_9_017D
        push   dword ptr [ebp - 8]
        call   sub_9_006C
        jae    loc_9_017D
        popal  
        dec    byte ptr [eax + 8]            ; -> obj5:0x0008
        jne    loc_9_014D
        btr    word ptr [4], ax              ; -> obj5:0x0004
        btr    word ptr [6], ax              ; -> obj5:0x0006
loc_9_014D:
        btr    esi, eax
        or     esi, esi
        jne    loc_9_00CC
loc_9_0158:
        test   dword ptr [ebp + 8], 0xffffffff
        jne    loc_9_0178
        xor    dword ptr [ebp - 4], 0xffffffff
        mov    edx, dword ptr [ebp - 4]
        je     loc_9_007E
        xor    dword ptr [ebp - 8], 0xffffffff
        je     loc_9_007E
loc_9_0178:
        leave  
        stc    
        ret    4
loc_9_017D:
        popal  
        mov    word ptr [edi + 2], ax
        leave  
        clc    
        ret    4
loc_9_0187:
        test   word ptr [0x2714], si         ; -> loc_1_2714
        jne    loc_9_0097
        or     si, si
        je     loc_9_0158
        or     word ptr [0x2714], si         ; -> loc_1_2714
        or     byte ptr [0x26f8], 1          ; -> obj1:0x26F8
        jmp    loc_9_0097
sub_9_01AC:
        push   0
        push   0
        push   4
        push   edi
        push   0                             ; -> obj5:0x0000
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_9_01CF
        mov    edi, dword ptr [0]            ; -> obj5:0x0000
        or     edi, edi
loc_9_01CE:
        ret    
loc_9_01CF:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_9_01CE
        mov    edi, dword ptr [ebx + 0xc]
        jmp    sub_9_01AC
sub_9_01DA:
        pushal 
        cld    
        mov    esi, 0x26fa                   ; -> loc_1_26FA
        mov    edi, 4                        ; -> obj5:0x0004
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ax, word ptr [0x2712]         ; -> loc_1_2712
        mov    word ptr [0x34], ax           ; -> obj5:0x0034
        popal  
        ret    
loc_9_01FB:
        btr    word ptr [0x26f8], 2          ; -> obj1:0x26F8
loc_9_0204:
        mov    ebx, dword ptr [ecx]
        call   sub_9_01DA
        call   sub_9_03EE
        mov    edi, dword ptr [ebx + 0xc]
loc_9_0213:
        call   sub_9_01AC
        je     loc_9_02B2
        movzx  eax, word ptr [edi + 2]
        cmp    eax, 0xf
        ja     loc_9_02C8
        bt     dword ptr [0x34], eax         ; -> obj5:0x0034
        jae    loc_9_0251
        dec    byte ptr [eax + 8]            ; -> obj5:0x0008
        jne    loc_9_0251
        btr    dword ptr [0x34], eax         ; -> obj5:0x0034
        btr    dword ptr [6], eax            ; -> obj5:0x0006
        btr    dword ptr [4], eax            ; -> obj5:0x0004
loc_9_0251:
        bts    dword ptr [4], eax            ; -> obj5:0x0004
        jae    loc_9_0280
        test   word ptr [edi], 1
        je     loc_9_02BD
        bt     dword ptr [6], eax            ; -> obj5:0x0006
        jae    loc_9_02BD
        xor    cl, cl
        bt     word ptr [edi], 1
        adc    cl, 0
        bt     dword ptr [0x18], eax         ; -> obj5:0x0018
        sbb    cl, 0
        jne    loc_9_02BD
loc_9_0280:
        inc    byte ptr [eax + 8]            ; -> obj5:0x0008
        test   word ptr [edi], 1
        je     loc_9_0213
        bts    dword ptr [6], eax            ; -> obj5:0x0006
        bts    dword ptr [0x18], eax         ; -> obj5:0x0018
        test   word ptr [edi], 2
        jne    loc_9_0213
        btr    dword ptr [0x18], eax         ; -> obj5:0x0018
        jmp    loc_9_0213
loc_9_02B2:
        mov    eax, 0
loc_9_02B7:
        mov    dword ptr [0x52], eax         ; -> obj5:0x0052
        ret    
loc_9_02BD:
        bt     word ptr [0x26f8], 2          ; -> obj1:0x26F8
        jb     loc_9_0280
loc_9_02C8:
        mov    eax, 0x13
        jmp    loc_9_02B7
loc_9_02CF:
        bts    word ptr [0x26f8], 2          ; -> obj1:0x26F8
        jmp    loc_9_0204
loc_9_02DD:
        cmp    dword ptr [0x52], 0           ; -> obj5:0x0052
        mov    eax, 0x13
        mov    dword ptr [0x52], eax         ; -> obj5:0x0052
        jne    loc_9_0315
        pushal 
        cld    
        mov    esi, 4                        ; -> obj5:0x0004
        mov    edi, 0x26fa                   ; -> loc_1_26FA
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ax, word ptr [0x34]           ; -> obj5:0x0034
        mov    word ptr [0x2712], ax         ; -> loc_1_2712
        popal  
        mov    eax, 0
loc_9_0315:
        ret    
loc_9_0316:
        mov    dword ptr [0x52], 0x13        ; -> obj5:0x0052
        mov    eax, 0
        ret    
loc_9_0326:
        pushal 
        mov    edi, 0x1c                     ; -> obj5:0x001C
        mov    esi, 0x26fa                   ; -> loc_1_26FA
        cld    
        lodsw  ax, word ptr [esi]
        not    ax
        stosw  word ptr es:[edi], ax
        mov    ecx, 0x16
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        popal  
        mov    dword ptr [ecx], 0x1c         ; -> obj5:0x001C
        mov    dword ptr [ecx + 4], 0x18
        mov    eax, 0
        ret    
loc_9_0355:
        mov    eax, 0
        ret    
sub_9_035B:
        cmp    dword ptr [ebx], 0
        jne    loc_9_0361
        ret    
loc_9_0361:
        pushal 
loc_9_0362:
        mov    edx, dword ptr [ebx + 0xc]
        xor    esi, esi
        xor    edi, edi
loc_9_0369:
        push   0
        push   0
        push   4
        push   edx
        push   0                             ; -> obj5:0x0000
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_9_03C9
        mov    edx, dword ptr [0]            ; -> obj5:0x0000
        movzx  eax, word ptr [edx + 4]
        mov    cx, 0xaaaa
        and    cx, ax
        xor    ax, cx
        add    cx, cx
        adc    ax, cx
        mov    cx, 0xcccc
        and    cx, ax
        xor    ax, cx
        shr    cx, 2
        add    ax, cx
        add    al, ah
        mov    cl, al
        shr    cl, 4
        and    ax, 0xf
        add    al, cl
        test   word ptr [edx], 1
        je     loc_9_03C4
        shl    eax, 1
loc_9_03C4:
        add    esi, eax
        inc    edi
        jmp    loc_9_0369
loc_9_03C9:
        mov    eax, esi
        mov    ecx, edi
        jecxz  loc_9_03D3
        xor    edx, edx
        div    ecx
loc_9_03D3:
        mov    dword ptr [ebx + 0x10], eax
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_9_0362
        mov    eax, dword ptr [esp + 0x18]
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Sort_NodeList ; cd2014003300
        add    esp, 8
        popal  
        ret    
sub_9_03EE:
        pushal 
loc_9_03EF:
        push   2
        push   dword ptr [ebx + 8]
        push   0                             ; -> obj5:0x0000
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_9_044A
loc_9_0407:
        mov    eax, dword ptr [0]            ; -> obj5:0x0000
        push   0
        push   0
        push   4
        push   eax
        push   0                             ; -> obj5:0x0000
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_9_044A
        mov    edi, dword ptr [0]            ; -> obj5:0x0000
        movzx  eax, word ptr [edi + 2]
        dec    byte ptr [eax + 8]            ; -> obj5:0x0008
        jne    loc_9_0448
        btr    word ptr [4], ax              ; -> obj5:0x0004
        btr    word ptr [6], ax              ; -> obj5:0x0006
loc_9_0448:
        jmp    loc_9_0407
loc_9_044A:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_9_03EF
        popal  
        ret    
VPICD_Service_26:
        mov    ax, word ptr [0x270e]         ; -> loc_1_270E
        ret    
VPICD_Service_27:
        mov    ax, word ptr [esp + 4]
        mov    word ptr [0x270e], ax         ; -> loc_1_270E
        not    ax
        and    ax, word ptr [0x26fc]         ; -> loc_1_26FC
        mov    word ptr [0x25f4], ax         ; -> loc_1_25F4
        ret    
        db  023h, 056h, 050h
VxD__LTEXT ENDS
