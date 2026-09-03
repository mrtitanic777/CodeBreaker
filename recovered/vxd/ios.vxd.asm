; IOS -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 12118 instructions recovered by recursive descent

Declare_Virtual_Device IOS, 3, 10, IOS_Control, 00010h, Undefined_Init_Order
;   DDK version 0400h   init_order 017000000h   flags 00000h
;   service table: obj1:0x1efc, 18 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x22a4  R-X
IOS_Control:
        cmp    eax, 0
        je     loc_3_068F
        cmp    eax, 1
        je     loc_3_0942
        cmp    eax, 2
        je     loc_3_0BC7
        cmp    eax, 5
        je     loc_1_0044
        cmp    eax, 6
        je     loc_1_0687
        cmp    eax, 0x23
        je     loc_4_3103
        cmp    eax, 0x22
        je     loc_4_1C81
        clc    
        ret    
IOS_Get_Version:
        mov    eax, 0x30a
        clc    
        ret    
loc_1_0044:
        mov    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [esi + 0x29], 0x40
        mov    esi, dword ptr [0x2220]       ; -> obj1:0x2220
        or     esi, esi
        je     loc_1_0087
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edx, dword ptr [edx + 0x30]
loc_1_0061:
        or     edx, edx
        je     loc_1_0087
        test   byte ptr [edx + 0x21], 0x80
        je     loc_1_0082
        test   byte ptr [edx + 0x23], 0x80
        je     loc_1_0082
        cmp    dword ptr [edx + 0xbf], esi
        jb     loc_1_0082
        mov    ax, 0x26
        call   sub_1_1AB1
loc_1_0082:
        mov    edx, dword ptr [edx + 0xc]
        jmp    loc_1_0061
loc_1_0087:
        sub    esp, 0xc
        mov    edi, esp
        mov    word ptr [edi], 0xe
        call   sub_1_175A
        add    esp, 0xc
        ret    
        db  000h, 000h
loc_1_009C:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
loc_1_00A8:
        push   edi
        mov    eax, dword ptr [esp + 8]
        mov    edi, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 0x14]
        mov    dword ptr [edx], 0xce         ; -> loc_1_00CE
        add    dword ptr [eax + 0x14], 8
        push   edi
        push   eax
        call   sub_1_1989
        add    esp, 8
        call   sub_1_010B
        pop    edi
        ret    
loc_1_00CE:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 0x14]
        sub    ecx, 8
        mov    dword ptr [esi + 0x14], ecx
        push   esi
        call   dword ptr [ecx]
        add    esp, 4
        cli    
        cmp    dword ptr [edi + 0x107], 0
        je     loc_1_0102
        cmp    esi, dword ptr [edi + 0x107]
        jne    loc_1_0107
        mov    dword ptr [edi + 0x107], 0
loc_1_0102:
        call   sub_1_010B
loc_1_0107:
        sti    
        pop    edi
        pop    esi
        ret    
sub_1_010B:
        cmp    dword ptr [edi + 0x107], 0
        jne    loc_1_0143
        push   edi
        call   sub_1_18D0
        add    esp, 4
        or     eax, eax
        je     loc_1_0143
        mov    dword ptr [edi + 0x107], eax
        sti    
        push   esi
        push   edi
        push   ebx
        lea    edi, [edi + 0xc3]
        mov    ecx, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [eax + 0x10], ecx
        push   eax
        call   dword ptr [ecx]
        add    esp, 4
        pop    ebx
        pop    edi
        pop    esi
loc_1_0143:
        sti    
        ret    
loc_1_0145:
        mov    ebx, dword ptr [esp + 4]
        movzx  esi, word ptr [ebx]
        cmp    esi, 6
        ja     loc_1_0177
        jmp    dword ptr [esi*4 + 0x15b]     ; -> obj1:0x015B
        ; ---- dispatch table (7 entries) ----
        dd  loc_3_0000
        dd  loc_1_0177
        dd  loc_1_0177
        dd  loc_3_0041
        dd  loc_1_0223
        dd  loc_1_0177
        dd  loc_3_0021
loc_1_0177:
        mov    word ptr [ebx + 2], 0
        ret    
        db  08Bh, 0C0h
loc_1_0180:
        mov    ah, byte ptr [ebp + 0x1d]
        cmp    ah, 0x90
        jb     loc_1_01A6
        je     loc_1_0195
        cmp    ah, 0x91
        ja     loc_1_01A6
        mov    byte ptr [ebp + 0x1d], 0
        clc    
        ret    
loc_1_0195:
        and    word ptr [ebp + 0x2c], 0xfffe
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        mov    byte ptr [ebp + 0x1d], 0
        clc    
        ret    
loc_1_01A6:
        stc    
        ret    
loc_1_01A8:
        mov    edi, dword ptr [0x2203]       ; -> obj1:0x2203
        test   edi, edi
        je     loc_1_01D1
        push   eax
        push   ebx
loc_1_01B4:
        push   dword ptr [edi]
        mov    ecx, dword ptr [edi + 0x30]
        jecxz  loc_1_01C2
        mov    edi, dword ptr [edi + 0x64]
        call   ecx
        jae    loc_1_01F7
loc_1_01C2:
        pop    edi
        mov    ebx, dword ptr [esp]
        mov    eax, dword ptr [esp + 4]
        test   edi, edi
        jne    loc_1_01B4
        add    esp, 8
loc_1_01D1:
        cmp    byte ptr [0x2245], 0          ; -> loc_1_2245
        je     loc_1_01EB
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    esi, 0x1fb                    ; -> loc_1_01FB
        VMMCall  VMM.Call_Global_Event           ; cd2010800100
loc_1_01EB:
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_01F7:
        add    esp, 0xc
        ret    
loc_1_01FB:
        mov    byte ptr [0x48e], 0xff
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        test   byte ptr [ebx + 1], 0x80
        je     loc_1_0214
        VMMCall  VMM.Wake_Up_VM                  ; cd2039800100
loc_1_0214:
        ret    
        db  02Eh, 08Bh, 0C0h
IOS_BD_Command_Complete:
        pushal 
        sub    esi, 0x64
        call   sub_1_024D
        popal  
        ret    
loc_1_0223:
        jmp    loc_1_0177
loc_1_0228:
        mov    esi, dword ptr [esp + 4]
        movzx  edx, word ptr [esi + 0x68]
        cmp    dl, 0xc
        je     loc_1_0247
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [ebx + 0x127]
        add    esi, 0x64
        cli    
        call   dword ptr [edi + 0x2c]
        sti    
        ret    
loc_1_0247:
        mov    word ptr [esi + 0x6a], 0
sub_1_024D:
        mov    edi, dword ptr [esi + 0x14]
        sub    edi, 8
        mov    dword ptr [esi + 0x14], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        ret    
sub_1_025D:
        pushal 
        movzx  eax, byte ptr [edi + 7]
        call   sub_1_128D
        jb     loc_1_031B
        push   edi
        mov    al, 0x11
        movzx  eax, al
        shl    eax, 3
        mov    ecx, eax
        add    eax, dword ptr [ebx + 4]
        add    eax, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], eax
        add    eax, 0x58
        mov    word ptr [edi + 4], ax
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_1310
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        mov    eax, edi
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        mov    dword ptr [edi + 4], ebx
        mov    dword ptr [edi + 8], ebx
        add    edi, 0x64
        mov    dword ptr [edi + 0x20], eax
        mov    dword ptr [edi + 0x48], eax
        cld    
        sub    ecx, ecx
        mov    cl, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        sub    edi, 0x20
        sub    esi, 0x20
        test   byte ptr [esi + 8], 2
        je     loc_1_02E8
        mov    ecx, dword ptr [esi + 0x1c]
        sub    eax, eax
        mov    edx, dword ptr [ecx]
        push   ebx
        sub    ebx, ebx
loc_1_02D5:
        inc    ebx
        add    eax, edx
        add    ecx, 8
        mov    edx, dword ptr [ecx]
        or     edx, edx
        jne    loc_1_02D5
        mov    byte ptr [edi + 0x4c], bl
        pop    ebx
        mov    dword ptr [edi + 0x18], eax
loc_1_02E8:
        or     byte ptr [edi + 9], 0x14
        mov    dword ptr [edi + 0xc], esi
        mov    esi, edi
        sub    edi, 0x64
        push   edi
        call   sub_1_14DA
        add    esp, 4
        or     eax, eax
        je     loc_1_0305
        or     byte ptr [esi + 8], 0x40
loc_1_0305:
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_1_031D
loc_1_030B:
        pop    edi
        mov    dword ptr [esi + 0x40], edi
        mov    al, byte ptr [edi + 7]
        mov    byte ptr [esi + 0x4d], al
        call   IOS_SendCommand
        clc    
loc_1_031B:
        popal  
        ret    
loc_1_031D:
        mov    eax, dword ptr [esi + 0x18]
        movzx  ecx, byte ptr [ebx + 0x3c]
        shl    eax, cl
        add    eax, 0xfff
        shr    eax, 0xc
        dec    eax
        je     loc_1_030B
        or     byte ptr [esi + 8], 0x42
        jmp    loc_1_030B
sub_1_0337:
        push   edi
        push   esi
        mov    edi, dword ptr [esi + 0xc]
        movzx  eax, word ptr [esi + 6]
        mov    word ptr [edi + 6], ax
        xchg   edi, esi
        mov    edi, dword ptr [edi + 0x40]
        pushfd 
        cli    
        call   dword ptr [esi + 0xc]
        popfd  
        mov    esi, dword ptr [esp]
        sub    esp, 8
        mov    edi, esp
        mov    word ptr [edi], 4
        mov    esi, dword ptr [esi + 0x20]
        mov    dword ptr [edi + 4], esi
        push   edi
        call   sub_1_1310
        add    esp, 0xc
        pop    esi
        pop    edi
        ret    
        db  000h, 000h
loc_1_0370:
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        and    byte ptr [ecx + 0x29], 0x7f
        cmp    byte ptr [ebp + 0x1d], 8
        jne    loc_1_0395
        test   byte ptr [ebp + 0x14], 0x80
        je     loc_1_0395
        or     byte ptr [ecx + 0x29], 0x80
loc_1_0395:
        mov    eax, dword ptr [ecx + 0x3c]
        or     eax, eax
        je     loc_1_03B7
        test   byte ptr [ecx + 0x29], 2
        je     loc_1_03B7
loc_1_03A2:
        VxDCall  VTD.VTD_Disable_Trapping        ; cd2005000500
        xor    eax, eax
        mov    esi, 0x53e                    ; -> loc_1_053E
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        stc    
        ret    
loc_1_03B7:
        test   byte ptr [ecx + 0x28], 1
        je     loc_1_04EE
        cmp    byte ptr [ebp + 0x1d], 0x41
        jb     loc_1_03CD
        cmp    byte ptr [ebp + 0x1d], 0x49
        jbe    loc_1_03F7
loc_1_03CD:
        cmp    byte ptr [ebp + 0x1d], 0xd
        jbe    loc_1_03F7
        cmp    byte ptr [ebp + 0x1d], 0x10
        jb     loc_1_04EE
        cmp    byte ptr [ebp + 0x1d], 0x11
        je     loc_1_03F7
        cmp    byte ptr [ebp + 0x1d], 0x14
        jbe    loc_1_04EE
        cmp    byte ptr [ebp + 0x1d], 0x1a
        ja     loc_1_04EE
loc_1_03F7:
        pushal 
        movzx  eax, byte ptr [ebp + 0x14]
        mov    cl, al
        or     eax, 0x40000000
        call   sub_1_128D
        jae    loc_1_0430
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ebx, dword ptr [ebx + 0x2c]
        mov    al, 1
loc_1_0415:
        cmp    byte ptr [ebx], 1
        ja     loc_1_041F
        cmp    byte ptr [ebx + 5], cl
        je     loc_1_042E
loc_1_041F:
        cmp    dword ptr [ebx + 1], 0
        je     loc_1_04C2
        add    ebx, dword ptr [ebx + 1]
        jmp    loc_1_0415
loc_1_042E:
        xor    ebx, ebx
loc_1_0430:
        mov    edi, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        test   dword ptr [ebx], 0x20
        jne    loc_1_0464
        movzx  eax, word ptr [ebp + 0x28]
        shl    eax, 4
        movzx  edx, word ptr [ebp + 0x24]
        add    eax, edx
        push   0
        push   1
        push   eax
        VMMCall  VMM._TestGlobalV86Mem           ; cd205f000100
        add    esp, 0xc
        cmp    eax, 1
        je     loc_1_04ED
loc_1_0464:
        sub    esp, 8
        mov    byte ptr [esp], 7
        mov    al, byte ptr [ebp + 0x14]
        mov    byte ptr [esp + 2], al
        mov    esi, esp
        push   esi
        VxDCall  BLOCKDEV.Service_0008h          ; cd2008001000
        add    esp, 4
        mov    esi, dword ptr [esi + 3]
        movzx  ecx, byte ptr [esp + 1]
        add    esp, 8
        or     ecx, ecx
        jne    loc_1_04ED
        or     esi, esi
        je     loc_1_04ED
        xor    ebx, ebx
        cmp    byte ptr [ebp + 0x1d], 3
        je     loc_1_049F
        cmp    byte ptr [ebp + 0x1d], 0x43
        jne    loc_1_04DA
loc_1_049F:
        mov    ebx, 1
        or     edi, edi
        je     loc_1_04B4
        test   byte ptr [edi + 0x1e], 0x20
        jne    loc_1_04E6
        test   byte ptr [edi + 0x16], 1
        jne    loc_1_04ED
loc_1_04B4:
        movzx  eax, byte ptr [ebp + 0x14]
        VxDCall  IFSMgr.Query_PhysLock           ; cd204b004000
        jae    loc_1_04E6
        mov    al, 3
loc_1_04C2:
        push   eax
        mov    ecx, 5
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    eax
        mov    byte ptr [ebp + 0x1d], al
        or     word ptr [ebp + 0x2c], 1
        popal  
        clc    
        ret    
loc_1_04DA:
        cmp    byte ptr [ebp + 0x1d], 2
        je     loc_1_04E6
        cmp    byte ptr [ebp + 0x1d], 0x42
        jne    loc_1_04ED
loc_1_04E6:
        call   sub_1_0515
        jb     loc_1_04C2
loc_1_04ED:
        popal  
loc_1_04EE:
        or     eax, eax
        je     loc_1_03A2
        pushfd 
        cli    
        or     byte ptr [eax + 0x20], 0x40
        cmp    dword ptr [eax + 0x107], 0
        jne    loc_1_050F
        mov    dword ptr [eax + 0x107], 1
loc_1_050F:
        popfd  
        jmp    loc_1_03A2
sub_1_0515:
        bsf    edi, esi
        je     loc_1_053C
        btr    esi, edi
        push   ebx
        push   edi
        VxDCall  IFSMgr._VolFlush                ; cd204c004000
        add    esp, 8
        or     eax, eax
        je     sub_1_0515
        mov    eax, 5
        or     ebx, ebx
        je     loc_1_053A
        mov    eax, 0xcc
loc_1_053A:
        stc    
        ret    
loc_1_053C:
        clc    
        ret    
loc_1_053E:
        call   sub_1_05C2
        push   ecx
        push   eax
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        btr    dword ptr [ecx + 0x28], 0xf
        jae    loc_1_057A
        mov    al, byte ptr [0x2224]         ; -> obj1:0x2224
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [ecx + 0x30]
loc_1_0560:
        or     ecx, ecx
        je     loc_1_0577
        test   byte ptr [ecx + 0x21], 0x80
        je     loc_1_0572
        test   byte ptr [ecx + 0x16], 0x40
        je     loc_1_0572
        inc    al
loc_1_0572:
        mov    ecx, dword ptr [ecx + 0xc]
        jmp    loc_1_0560
loc_1_0577:
        mov    byte ptr [ebp + 0x14], al
loc_1_057A:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    dword ptr [ecx + 0x3c], 0
        jne    loc_1_0596
loc_1_0586:
        pop    eax
        pop    ecx
        VxDCall  VTD.VTD_Enable_Trapping         ; cd2006000500
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        clc    
        ret    
loc_1_0596:
        pushfd 
        push   edi
        cli    
        mov    edi, dword ptr [ecx + 0x3c]
        or     edi, edi
        je     loc_1_05AD
        and    byte ptr [edi + 0x20], 0xbf
        cmp    dword ptr [edi + 0x107], 1
        je     loc_1_05B1
loc_1_05AD:
        pop    edi
        popfd  
        jmp    loc_1_0586
loc_1_05B1:
        mov    dword ptr [edi + 0x107], 0
        call   sub_1_010B
        jmp    loc_1_05AD
sub_1_05C2:
        pushal 
        test   byte ptr [0x2244], 6          ; -> obj1:0x2244
        jne    loc_1_0607
        or     byte ptr [0x2244], 2          ; -> obj1:0x2244
        mov    eax, dword ptr [0x2234]       ; -> obj1:0x2234
        mov    ecx, eax
        call   sub_1_0609
        mov    dword ptr [0x2238], eax       ; -> obj1:0x2238
        mov    dword ptr [0x223c], ecx       ; -> obj1:0x223C
        mov    edx, eax
        xor    edx, ecx
        test   edx, 0xffff0000
        je     loc_1_0601
        mov    ecx, dword ptr [0x2234]       ; -> obj1:0x2234
        call   sub_1_0609
loc_1_0601:
        VxDCall  INT13.Int13_Hooking_BIOS_Int    ; cd2003002000
loc_1_0607:
        popal  
        ret    
sub_1_0609:
        pushf  
        cli    
        xchg   dword ptr [0x7b4], eax
        test   byte ptr [0x222c], 4          ; -> obj1:0x222C
        jne    loc_1_0624
        xchg   dword ptr [0x806], ecx
loc_1_0621:
        popf   
        ret    
loc_1_0624:
        xchg   dword ptr [0x7b0], ecx
        jmp    loc_1_0621
sub_1_062C:
        pushal 
        btr    word ptr [0x2244], 1          ; -> obj1:0x2244
        jae    loc_1_064E
        VxDCall  INT13.Int13_Unhooking_BIOS_Int  ; cd2004002000
        mov    eax, dword ptr [0x2238]       ; -> obj1:0x2238
        mov    ecx, dword ptr [0x223c]       ; -> obj1:0x223C
        call   sub_1_0609
loc_1_064E:
        popal  
        ret    
loc_1_0650:
        VxDCall  INT13.Int13_Translate_VM_Int    ; cd2002002000
        jae    loc_1_0668
        mov    eax, dword ptr [0x223c]       ; -> obj1:0x223C
        mov    word ptr [ebp + 0x24], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x28], ax
loc_1_0668:
        ret    
loc_1_0669:
        VxDCall  INT13.Int13_Translate_VM_Int    ; cd2002002000
        jae    loc_1_0668
        call   sub_1_062C
        mov    eax, dword ptr [0x21c3]       ; -> obj1:0x21C3
        mov    word ptr [ebp + 0x24], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x28], ax
        ret    
loc_1_0687:
        sub    esp, 0xc
        mov    edi, esp
        mov    word ptr [edi], 1
        call   sub_1_175A
        add    esp, 0xc
        call   sub_1_062C
        pushf  
        cli    
        mov    eax, 0x4c
        mov    edi, dword ptr [0x21c3]       ; -> obj1:0x21C3
        mov    dword ptr [eax], edi
        popf   
        xor    ecx, ecx
loc_1_06B2:
        mov    eax, dword ptr [ecx*4 + 0x370] ; -> obj5:0x0370
        cmp    eax, -1
        je     loc_1_06C4
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_1_06C4:
        inc    ecx
        cmp    ecx, 0x1a
        jb     loc_1_06B2
        cmp    byte ptr [0x2245], 0          ; -> loc_1_2245
        je     loc_1_06DD
        mov    eax, dword ptr [0x2240]       ; -> obj1:0x2240
        mov    dword ptr [0x54], eax
loc_1_06DD:
        clc    
        ret    
        db  000h
loc_1_06E0:
        sti    
        mov    edi, dword ptr [esp + 4]
        movzx  esi, word ptr [edi + 4]
        movzx  ecx, word ptr [edi + 0xa]
        mov    al, byte ptr [edi + 0xc]
        test   al, 8
        jne    loc_1_06FD
loc_1_06F4:
        call   sub_1_072B
loc_1_06F9:
        mov    dword ptr [edi + 6], ebx
        ret    
loc_1_06FD:
        mov    al, 1
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [ebx + 0x29], 0x20
        jne    loc_1_06F4
        add    esi, 0x14
        push   0
        push   esi
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_06F9
        mov    word ptr [ebx + 0x10], 0x20
        add    ebx, 0x14
        jmp    loc_1_06F9
sub_1_072B:
        sub    esp, 4
        pushfd 
        push   edi
        push   edx
        push   ecx
        mov    byte ptr [esp + 0x10], al
loc_1_0736:
        test   byte ptr [esp + 0x10], 0x20
        jne    loc_1_0849
        push   esi
        add    esi, 0xf
        and    esi, 0xfffffff0
        cli    
        cmp    esi, 0x400
        jae    loc_1_0792
        mov    edi, esi
        shr    edi, 2
        mov    ebx, dword ptr [edi + 0x1fa8] ; -> obj1:0x1FA8
        or     ebx, ebx
        je     loc_1_0792
        mov    eax, dword ptr [ebx + 4]
        test   byte ptr [esp + 0x14], 0x40
        jne    loc_1_078E
loc_1_076A:
        mov    dword ptr [edi + 0x1fa8], eax ; -> obj1:0x1FA8
loc_1_0770:
        mov    word ptr [ebx + 0xc], cx
        mov    word ptr [ebx + 0x10], 4
        mov    dword ptr [ebx + 4], 0
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebx + 0x14], eax
        add    ebx, 0x14
        jmp    loc_1_09A9
loc_1_078E:
        or     eax, eax
        jne    loc_1_076A
loc_1_0792:
        xor    ebx, ebx
        test   byte ptr [esp + 0x14], 0x40
        jne    loc_1_09A9
        add    esi, 0x14
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        add    ebx, 0x6e
loc_1_07AB:
        test   word ptr [ebx + 0x10], 1
        je     loc_1_07BB
        cmp    si, word ptr [ebx + 0xe]
        jb     loc_1_07C9
        je     loc_1_07FC
loc_1_07BB:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    loc_1_07AB
        xor    esi, esi
        jmp    loc_1_091B
loc_1_07C9:
        movzx  edi, word ptr [ebx + 0xe]
        sub    edi, esi
        cmp    edi, 0x24
        jbe    loc_1_07FC
        mov    word ptr [ebx + 0xe], di
        mov    eax, dword ptr [ebx + 8]
        add    edi, ebx
        mov    dword ptr [ebx + 8], edi
        mov    ebx, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [0x1fa0]       ; -> obj1:0x1FA0
        add    eax, ebx
        sub    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        add    eax, 0x14
        mov    dword ptr [ebx], eax
        mov    word ptr [ebx + 0xe], si
loc_1_07FC:
        jmp    loc_1_0770
loc_1_0801:
        sti    
        shr    esi, 2
        inc    esi
        cmp    esi, 0x40
        jb     loc_1_091B
        pop    esi
        cli    
        mov    ebx, dword ptr [0x21bf]       ; -> obj1:0x21BF
loc_1_0817:
        or     ebx, ebx
        je     loc_1_083E
        cmp    word ptr [ebx + 0xe], si
        jb     loc_1_0829
        test   word ptr [ebx + 0x10], 0x14
        je     loc_1_082E
loc_1_0829:
        mov    ebx, dword ptr [ebx + 4]
        jmp    loc_1_0817
loc_1_082E:
        or     word ptr [ebx + 0x10], 4
        mov    word ptr [ebx + 0xc], cx
        mov    eax, dword ptr [ebx]
        jmp    loc_1_08E3
loc_1_083E:
        test   byte ptr [esp + 0x10], 1
        jne    loc_1_09A5
loc_1_0849:
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    dword ptr [ebx + 0x54], 0
        jne    loc_1_08EF
        sti    
        mov    ebx, esi
        add    ebx, 0x14
        shl    ebx, 1
        push   ecx
        push   1
        push   ebx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_0999
        shr    ebx, 1
        mov    ecx, eax
        mov    edx, eax
        and    ecx, 0xfff
        mov    edi, 0x1000
        sub    edi, ecx
        cmp    edi, ebx
        ja     loc_1_0897
        add    eax, ebx
        add    ecx, ebx
        and    ecx, 0xfff
loc_1_0897:
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 0x10], 0xc
        pop    ecx
        mov    word ptr [eax + 0xc], cx
        mov    word ptr [eax + 0xe], bx
        mov    ebx, eax
        mov    ecx, eax
        shr    ecx, 0xc
        lea    eax, [eax]
        push   0
        push   eax
        push   1
        push   ecx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    eax, dword ptr [ebx]
        and    eax, 0xfffff000
        add    eax, dword ptr [ebx + 4]
        add    eax, 0x14
        cli    
        mov    ecx, dword ptr [0x21bf]       ; -> obj1:0x21BF
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [0x21bf], ebx       ; -> obj1:0x21BF
        mov    dword ptr [ebx], eax
loc_1_08E3:
        mov    dword ptr [ebx + 0x14], eax
        sti    
        add    ebx, 0x14
        jmp    loc_1_09AA
loc_1_08EF:
        test   byte ptr [esp + 0x10], 2
        je     loc_1_09A5
loc_1_08FA:
        inc    byte ptr [0x21be]             ; -> obj1:0x21BE
        mov    eax, dword ptr [0x21ba]       ; -> obj1:0x21BA
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        dec    byte ptr [0x21be]             ; -> obj1:0x21BE
        jmp    loc_1_0736
loc_1_091B:
        shl    esi, 2
        cli    
        mov    ebx, dword ptr [esi + 0x1fa8] ; -> obj1:0x1FA8
        or     ebx, ebx
        je     loc_1_0801
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0x1fa8], eax ; -> obj1:0x1FA8
        mov    edi, dword ptr [ebx + 8]
        or     edi, edi
        je     loc_1_0959
        mov    word ptr [ebx + 0x10], 1
        test   word ptr [edi + 0x10], 1
        je     loc_1_0959
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 8], eax
        movzx  eax, word ptr [edi + 0xe]
        add    word ptr [ebx + 0xe], ax
loc_1_0959:
        mov    esi, 0x6e
        add    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
loc_1_0964:
        cmp    dword ptr [esi + 8], ebx
        je     loc_1_0975
        mov    esi, dword ptr [esi + 8]
        jb     loc_1_0964
        sti    
        pop    esi
        jmp    loc_1_0736
loc_1_0975:
        test   word ptr [esi + 0x10], 1
        jne    loc_1_0984
        sti    
        pop    esi
        jmp    loc_1_0736
loc_1_0984:
        movzx  eax, word ptr [ebx + 0xe]
        add    word ptr [esi + 0xe], ax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 8], eax
        sti    
        pop    esi
        jmp    loc_1_0736
loc_1_0999:
        pop    ecx
        test   byte ptr [esp + 0x10], 2
        jne    loc_1_08FA
loc_1_09A5:
        xor    ebx, ebx
        jmp    loc_1_09AA
loc_1_09A9:
        pop    esi
loc_1_09AA:
        pop    ecx
        pop    edx
        pop    edi
        popfd  
        add    esp, 4
        ret    
        db  000h, 000h
loc_1_09B4:
        mov    edi, dword ptr [esp + 4]
        mov    edi, dword ptr [edi + 4]
        test   word ptr [edi - 4], 0x30
        jne    loc_1_09C9
        call   sub_1_0A1E
loc_1_09C8:
        ret    
loc_1_09C9:
        test   word ptr [edi - 4], 0x10
        jne    loc_1_09E2
        sub    edi, 0x14
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_1_09C8
loc_1_09E2:
        sub    edi, 0x14
        pushfd 
        cli    
        mov    ebx, dword ptr [0x21bf]       ; -> obj1:0x21BF
        mov    ecx, 0x21bb                   ; -> obj1:0x21BB
loc_1_09F2:
        or     ebx, ebx
        je     loc_1_0A14
        cmp    ebx, edi
        jne    loc_1_0A17
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        cmp    byte ptr [0x21be], 0          ; -> obj1:0x21BE
        je     loc_1_0A14
        mov    eax, dword ptr [0x21ba]       ; -> obj1:0x21BA
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_1_0A14:
        popfd  
        jmp    loc_1_09C8
loc_1_0A17:
        mov    ecx, ebx
        mov    ebx, dword ptr [ebx + 4]
        jmp    loc_1_09F2
sub_1_0A1E:
        push   ebp
        push   esi
        push   edi
        sub    edi, 0x14
        test   byte ptr [edi + 0x10], 8
        jne    loc_1_0A63
        movzx  esi, word ptr [edi + 0xe]
        sub    esi, 0x14
        and    esi, 0xfffffff0
        cmp    esi, 0x400
        jae    loc_1_0A76
        cli    
        mov    word ptr [edi + 0x10], 2
        shr    esi, 2
        mov    ebp, dword ptr [esi + 0x1fa8] ; -> obj1:0x1FA8
        mov    dword ptr [edi + 4], ebp
        mov    dword ptr [esi + 0x1fa8], edi ; -> obj1:0x1FA8
        sti    
loc_1_0A56:
        cmp    byte ptr [0x21be], 0          ; -> obj1:0x21BE
        jne    loc_1_0A69
loc_1_0A5F:
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_0A63:
        and    byte ptr [edi + 0x10], 0xfb
        jmp    loc_1_0A56
loc_1_0A69:
        mov    eax, dword ptr [0x21ba]       ; -> obj1:0x21BA
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_1_0A5F
loc_1_0A76:
        mov    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        add    esi, 0x6e
loc_1_0A7F:
        cmp    dword ptr [esi + 8], edi
        je     loc_1_0A8B
        ja     loc_1_0AA5
        mov    esi, dword ptr [esi + 8]
        jmp    loc_1_0A7F
loc_1_0A8B:
        test   word ptr [esi + 0x10], 1
        je     loc_1_0AA5
        movzx  ebp, word ptr [edi + 0xe]
        add    word ptr [esi + 0xe], bp
        mov    ebp, dword ptr [edi + 8]
        mov    dword ptr [esi + 8], ebp
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_0AA5:
        mov    word ptr [edi + 0x10], 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h
IOS_SendCommand:
        test   byte ptr [esi + 9], 4
        je     loc_1_0B7E
        push   esi
        push   edi
        push   ebx
        sub    esi, 0x64
        mov    ebx, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [esi + 0x1c], edi
        sub    ebx, 0xc3
        cmp    word ptr [esi + 0x68], 0xf
        je     loc_1_0AFB
        test   byte ptr [esi + 0x6f], 0x20
        jne    loc_1_0AFB
        movzx  eax, byte ptr [esi + 0xb1]
        mov    ebx, dword ptr [esi + 0x6c]
        and    ebx, 0x40000000
        or     eax, ebx
        call   sub_1_128D
        jb     loc_1_1013
loc_1_0AFB:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax + 0xbf], 0
        test   byte ptr [eax + 0x23], 4
        jne    loc_1_1013
        mov    eax, dword ptr [eax + 0x41]
        or     eax, dword ptr [ebx + 0x41]
        jne    loc_1_1035
loc_1_0B20:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x10], eax
loc_1_0B26:
        mov    word ptr [esi + 0xe], 0xf
        mov    word ptr [esi + 0xc], 0xf
        mov    dword ptr [esi + 8], ebx
        cmp    byte ptr [esi + 0x68], 0xd
        je     loc_1_0BA3
loc_1_0B3B:
        lea    eax, [esi + 0x2c]
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esi + 0x2c], 0x110d ; -> loc_1_110D
        add    dword ptr [esi + 0x14], 8
        mov    eax, dword ptr [esi + 0x6c]
        test   eax, 0x8000040
        jne    loc_1_0C71
        test   eax, 0x100
        jne    loc_1_0BD3
        push   eax
loc_1_0B62:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esi + 0x10]
        call   dword ptr [eax]
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        test   eax, 0x8000140
        jne    loc_1_0C05
        jmp    loc_1_0C6D
loc_1_0B7E:
        cmp    dword ptr [esi], 0
        je     sub_1_025D
        push   eax
        sub    esp, 4
loc_1_0B8B:
        sub    eax, eax
        xchg   dword ptr [esi], eax
        mov    dword ptr [esp], eax
        call   sub_1_025D
        mov    esi, dword ptr [esp]
        or     esi, esi
        jne    loc_1_0B8B
        add    esp, 4
        pop    eax
        ret    
loc_1_0BA3:
        test   byte ptr [esi + 0x6d], 1
        jne    loc_1_0BC3
        mov    edx, esi
        mov    esi, 0xbbb                    ; -> loc_1_0BBB
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_0C6D
loc_1_0BBB:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        sub    esp, 0xc
loc_1_0BC3:
        call   0xffff0a98                    ; -> obj4:0x0A98
        jae    loc_1_0C6D
        jmp    loc_1_0B3B
loc_1_0BD3:
        mov    eax, dword ptr [0x21b6]       ; -> obj1:0x21B6
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cli    
        movzx  ecx, word ptr [0x2194]        ; -> obj1:0x2194
        bsf    eax, ecx
        btr    word ptr [0x2194], ax         ; -> obj1:0x2194
        sti    
        push   dword ptr [esi + 0x70]
        mov    dword ptr [esi + 0x70], eax
        push   dword ptr [esi + 0x6c]
        jmp    loc_1_0B62
loc_1_0C05:
        mov    ecx, 5
        mov    eax, dword ptr [esi + 0x70]
        mov    eax, dword ptr [eax*4 + 0x2196] ; -> obj1:0x2196
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        test   dword ptr [esi + 0x6c], 0x8000040
        jne    loc_1_0F07
loc_1_0C27:
        mov    eax, dword ptr [esi + 0x70]
        bts    word ptr [0x2194], ax         ; -> obj1:0x2194
        mov    eax, dword ptr [0x21b6]       ; -> obj1:0x21B6
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        pop    dword ptr [esi + 0x70]
        mov    ebx, dword ptr [esi + 8]
        cmp    word ptr [esi + 0x6a], 0x2c
        je     loc_1_0CD7
loc_1_0C4E:
        test   dword ptr [esi + 0x6c], 0x100
        jne    loc_1_0C6D
loc_1_0C57:
        add    esi, 0x64
        test   byte ptr [esi + 9], 0x10
        jne    loc_1_102B
        push   dword ptr [esi + 0x40]
        call   dword ptr [esi + 0xc]
        add    esp, 4
loc_1_0C6D:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_1_0C71:
        test   byte ptr [esi + 0x6d], 1
        jne    loc_1_0CD7
        mov    edx, esi
        mov    esi, 0xccf                    ; -> loc_1_0CCF
        mov    ebx, dword ptr [edx + 8]
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_1_0C8F
        VMMCall  VMM.Call_Global_Event           ; cd2010000100
        jmp    loc_1_0C6D
loc_1_0C8F:
        mov    esi, 0xcbd                    ; -> loc_1_0CBD
        xor    ebx, ebx
        mov    ecx, 0x820
        mov    eax, 0x10
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        jmp    loc_1_0C6D
loc_1_0CA8:
        mov    esi, 0xcb8                    ; -> loc_1_0CB8
        mov    eax, 0x64
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c800100
loc_1_0CB8:
        sub    esp, 0xc
        jmp    loc_1_0C8F
loc_1_0CBD:
        call   sub_1_1E18
        jb     loc_1_0CA8
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x28], 0x80
        jne    loc_1_0CA8
loc_1_0CCF:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        sub    esp, 0xc
loc_1_0CD7:
        mov    edx, esi
        mov    edi, ebx
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x29], 0x20
        mov    al, 1
        jne    loc_1_0CEA
        mov    al, 2
loc_1_0CEA:
        mov    esi, 0x49
        mov    ecx, 9
        call   sub_1_072B
        mov    esi, edx
        or     ebx, ebx
        je     loc_1_0EFC
        test   byte ptr [edi + 0x21], 0x20
        jne    loc_1_0D13
        test   byte ptr [esi + 0x6f], 8
        jne    loc_1_0D93
loc_1_0D13:
        and    byte ptr [esi + 0x6f], 0xf7
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x29], 0x20
        jne    loc_1_0D40
        push   0x80
        push   0
        push   -1
        push   0
        push   0
        push   0
        push   1
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        jmp    loc_1_0D85
loc_1_0D40:
        pushal 
        mov    edi, dword ptr [esi + 4]
        mov    esi, 0x2000
        cmp    dword ptr [edi + 0x50], 0x1000
        je     loc_1_0D57
        mov    esi, 0x1000
loc_1_0D57:
        mov    ecx, 9
        mov    al, 1
        call   sub_1_072B
        mov    dword ptr [esp + 0x14], ebx
        mov    dword ptr [esp + 0x1c], ebx
        cmp    dword ptr [edi + 0x50], 0x1000
        jne    loc_1_0D84
        add    ebx, 0xfff
        and    ebx, 0xfffff000
        mov    dword ptr [esp + 0x14], ebx
loc_1_0D84:
        popal  
loc_1_0D85:
        or     edx, edx
        je     loc_1_0EF5
        mov    dword ptr [ebx + 0x21], edx
        mov    dword ptr [ebx + 0x29], eax
loc_1_0D93:
        mov    ecx, dword ptr [esi + 0x6c]
        mov    dword ptr [ebx + 0x2d], ecx
        mov    ecx, dword ptr [esi + 0x74]
        mov    dword ptr [ebx + 0x35], ecx
        mov    ecx, dword ptr [esi + 0x80]
        test   dword ptr [esi + 0x6c], 2
        je     loc_1_0EE7
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx + 0xc], eax
loc_1_0DBD:
        test   byte ptr [esi + 0x6f], 8
        jne    loc_1_0E8C
        xchg   dword ptr [ebx], ecx
        xchg   dword ptr [esi + 0xac], ecx
        mov    dword ptr [ebx + 4], ecx
        mov    cl, byte ptr [edi + 0x3c]
        shl    dword ptr [ebx + 0xc], cl
        test   dword ptr [edi + 0x1c], 0x1000
        jne    loc_1_0DE7
        mov    dword ptr [esi + 0xac], ebx
loc_1_0DE7:
        add    dword ptr [esi + 0xac], 0x10
        mov    dword ptr [ebx + 0x10], edx
        xchg   dword ptr [esi + 0x80], edx
        mov    dword ptr [ebx + 0x31], edx
        and    byte ptr [esi + 0x6c], 0xfd
        or     byte ptr [esi + 0x6f], 4
        mov    eax, 0x1000
        mov    edx, dword ptr [edi]
        cmp    dword ptr [edx + 0x50], eax
        jae    loc_1_0E11
        mov    eax, dword ptr [edx + 0x50]
loc_1_0E11:
        mov    dword ptr [ebx + 0x14], eax
        shr    eax, cl
        cmp    eax, dword ptr [esi + 0x7c]
        jbe    loc_1_0E23
        mov    eax, dword ptr [esi + 0x7c]
        shl    eax, cl
        mov    dword ptr [ebx + 0x14], eax
loc_1_0E23:
        test   dword ptr [edi + 0x1c], 0x800
        je     loc_1_0E5B
        mov    eax, dword ptr [ebx + 0x21]
        shr    eax, 0xc
        lea    ecx, [ebx + 0x25]
        push   0
        push   ecx
        push   1
        push   eax
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    edx, dword ptr [ebx + 0x25]
        and    edx, 0xfffff000
        mov    ecx, dword ptr [ebx + 0x21]
        and    ecx, 0xfff
        add    edx, ecx
        mov    dword ptr [ebx + 0x10], edx
loc_1_0E5B:
        mov    cl, byte ptr [edi + 0x3c]
        mov    eax, dword ptr [ebx + 0x14]
        shr    eax, cl
        xchg   dword ptr [esi + 0x7c], eax
        mov    dword ptr [ebx + 0x18], eax
        mov    dword ptr [ebx + 0x1c], eax
        mov    al, 1
        xchg   byte ptr [esi + 0xb0], al
        mov    byte ptr [ebx + 0x20], al
        cmp    word ptr [esi + 0x68], 1
        jne    loc_1_0BD3
        call   sub_1_1150
        jmp    loc_1_0BD3
loc_1_0E8C:
        mov    eax, dword ptr [esi + 0x7c]
        mov    dword ptr [ebx + 0x18], eax
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [esi + 0x80]
        mov    dword ptr [ebx + 0x31], eax
        mov    dword ptr [ebx], eax
        bts    dword ptr [esi + 0x6c], 1
        jb     loc_1_0EC0
        xor    eax, eax
        mov    dword ptr [ebx + 0x45], eax
        mov    dword ptr [ebx + 0x41], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 0x3d], eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ebx + 0x39], eax
        lea    eax, [ebx + 0x39]
        mov    dword ptr [ebx], eax
loc_1_0EC0:
        xor    eax, eax
        mov    dword ptr [ebx + 0x25], eax
        call   sub_1_11C9
        jb     loc_1_0ED1
        jmp    loc_1_0BD3
loc_1_0ED1:
        call   sub_1_10C4
        call   sub_1_10E9
        and    byte ptr [esi + 0x6f], 0xf7
        mov    ebx, dword ptr [esi + 8]
        jmp    loc_1_0CD7
loc_1_0EE7:
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [esi + 0x7c]
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_0DBD
loc_1_0EF5:
        mov    edi, ebx
        call   sub_1_0A1E
loc_1_0EFC:
        mov    word ptr [esi + 0x6a], 0x1d
        jmp    loc_1_0C4E
loc_1_0F07:
        cmp    word ptr [esi + 0x6a], 0x10
        jb     loc_1_0F21
        test   byte ptr [esi + 0x6f], 8
        je     loc_1_0FC7
        mov    dword ptr [ebx + 0x18], 0
        jmp    loc_1_0F9C
loc_1_0F21:
        mov    eax, dword ptr [esi + 0x7c]
        sub    dword ptr [ebx + 0x18], eax
        add    dword ptr [esi + 0x74], eax
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0x10], eax
        add    dword ptr [esi + 0x14], 8
        test   byte ptr [esi + 0x6f], 8
        jne    loc_1_0F9C
        cmp    word ptr [esi + 0x68], 1
        je     loc_1_0F49
        call   sub_1_1150
loc_1_0F49:
        mov    dword ptr [esi + 0x64], 0
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_1_0FC7
        mov    eax, 0x1000
        mov    edi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 0x50], eax
        jae    loc_1_0F66
        mov    eax, dword ptr [edi + 0x50]
loc_1_0F66:
        mov    dword ptr [ebx + 0x14], eax
        mov    edi, dword ptr [esi + 8]
        mov    cl, byte ptr [edi + 0x3c]
        shr    eax, cl
        mov    dword ptr [esi + 0x7c], eax
        cmp    eax, dword ptr [ebx + 0x18]
        jbe    loc_1_0F84
        mov    eax, dword ptr [ebx + 0x18]
        mov    dword ptr [esi + 0x7c], eax
        shl    eax, cl
        mov    dword ptr [ebx + 0x14], eax
loc_1_0F84:
        push   dword ptr [esi + 0x6c]
        cmp    word ptr [esi + 0x68], 1
        jne    loc_1_0B62
        call   sub_1_1150
        jmp    loc_1_0B62
loc_1_0F9C:
        call   sub_1_10C4
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_1_0FFE
        call   sub_1_11C9
        jb     loc_1_0FB6
        push   dword ptr [esi + 0x6c]
        jmp    loc_1_0B62
loc_1_0FB6:
        call   sub_1_10C4
        mov    word ptr [esi + 0x6a], 0x2c
        and    byte ptr [ebx + 0x30], 0xf7
        jmp    loc_1_0FFE
loc_1_0FC7:
        mov    al, byte ptr [ebx + 0x20]
        mov    byte ptr [esi + 0xb0], al
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0xac], eax
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x29], 0x20
        jne    loc_1_0FF4
        push   0
        push   dword ptr [ebx + 0x29]
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        jmp    loc_1_0FFE
loc_1_0FF4:
        pushal 
        mov    edi, dword ptr [ebx + 0x29]
        call   sub_1_0A1E
        popal  
loc_1_0FFE:
        call   sub_1_10E9
        jmp    loc_1_0C27
        db  066h, 0C7h, 046h, 06Ah, 01Dh, 000h, 0E9h, 05Ah, 0FCh, 0FFh, 0FFh
loc_1_1013:
        mov    word ptr [esi + 0x6a], 0x17
        test   dword ptr [esi + 0x6c], 0x100
        jne    loc_1_0C6D
        jmp    loc_1_0C57
loc_1_102B:
        call   sub_1_0337
        jmp    loc_1_0C6D
loc_1_1035:
        mov    ecx, ebx
        cmp    dword ptr [ecx + 0x41], 0
        jne    loc_1_103F
        mov    ecx, dword ptr [ecx]
loc_1_103F:
        push   ebx
        mov    ebx, edi
        test   dword ptr [ecx + 0x24], 1
        jne    loc_1_104E
        mov    ebx, dword ptr [edi + 0x14]
loc_1_104E:
        cmp    ebx, eax
        pop    ebx
        je     loc_1_0B20
        cmp    word ptr [esi + 0x68], 0xd
        je     loc_1_1069
loc_1_105E:
        mov    word ptr [esi + 0x6a], 0x29
        jmp    loc_1_0C4E
loc_1_1069:
        mov    eax, dword ptr [esi + 0x92]
        cmp    ah, 0x44
        jne    loc_1_1091
        cmp    al, 0x11
        ja     loc_1_1091
        cmp    al, 0x11
        je     loc_1_0B20
        cmp    al, 0xd
        je     loc_1_109D
        cmp    al, 0xa
        ja     loc_1_1091
        cmp    al, 8
        jb     loc_1_1091
        jmp    loc_1_0B20
loc_1_1091:
        mov    dword ptr [esi + 0xa0], 0x28
        jmp    loc_1_105E
loc_1_109D:
        mov    eax, dword ptr [esi + 0x94]
        cmp    ah, 8
        jne    loc_1_1091
        cmp    al, 0x6f
        je     loc_1_0B20
        cmp    al, 0x60
        jne    loc_1_1091
        mov    eax, dword ptr [esi + 0x98]
        test   byte ptr [eax], 1
        jne    loc_1_1091
        jmp    loc_1_0B20
sub_1_10C4:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebx + 0x21]
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [esi + 0x80]
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 0x10]
        mov    dword ptr [eax], ecx
        ret    
sub_1_10E9:
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [esi + 0x7c], eax
        mov    eax, dword ptr [ebx + 0x35]
        mov    dword ptr [esi + 0x74], eax
        mov    eax, dword ptr [ebx + 0x31]
        mov    dword ptr [esi + 0x80], eax
        mov    eax, dword ptr [ebx + 0x2d]
        mov    dword ptr [esi + 0x6c], eax
        mov    edi, ebx
        jmp    sub_1_0A1E
        db  090h
IOS_Synchronous_Command:
        ret    
loc_1_110D:
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 4]
        test   dword ptr [esi + 0x6c], 0x8000140
        jne    loc_1_1137
        add    esi, 0x64
        test   byte ptr [esi + 9], 0x10
        jne    loc_1_1149
        push   dword ptr [esi + 0x40]
        call   dword ptr [esi + 0xc]
        add    esp, 4
loc_1_1132:
        pop    esi
        pop    edi
        pop    ebx
        sti    
        ret    
loc_1_1137:
        mov    eax, dword ptr [esi + 0x70]
        mov    eax, dword ptr [eax*4 + 0x2196] ; -> obj1:0x2196
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_1_1132
loc_1_1149:
        call   sub_1_0337
        jmp    loc_1_1132
sub_1_1150:
        sub    esp, 8
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [esp + 4], ecx
        mov    edi, dword ptr [ebx + 0x21]
        mov    dword ptr [esp], edi
        mov    eax, dword ptr [ebx + 0xc]
        or     eax, eax
        je     loc_1_11A7
loc_1_1167:
        mov    edx, esi
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [esp]
        cmp    word ptr [edx + 0x68], 0
        jne    loc_1_1178
        xchg   esi, edi
loc_1_1178:
        cmp    eax, ecx
        jae    loc_1_117E
        mov    ecx, eax
loc_1_117E:
        add    dword ptr [ebx + 8], ecx
        sub    dword ptr [ebx + 0xc], ecx
        sub    dword ptr [esp + 4], ecx
        add    dword ptr [esp], ecx
        cld    
        test   ecx, 3
        jne    loc_1_119B
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_119D
loc_1_119B:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_119D:
        mov    esi, edx
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     loc_1_11C5
loc_1_11A7:
        add    dword ptr [ebx], 8
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esi + 8]
        mov    cl, byte ptr [ecx + 0x3c]
        shl    eax, cl
        mov    dword ptr [ebx + 0xc], eax
        mov    ecx, dword ptr [esp + 4]
        jmp    loc_1_1167
loc_1_11C5:
        add    esp, 8
        ret    
sub_1_11C9:
        mov    edi, dword ptr [esi + 8]
        mov    cl, byte ptr [edi + 0x3c]
        mov    eax, 0xffff
        shr    eax, cl
        dec    eax
        mov    ecx, dword ptr [ebx + 0x18]
        sub    ecx, eax
        sbb    edx, edx
        and    ecx, edx
        add    ecx, eax
        mov    dword ptr [esi + 0x7c], ecx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x80], eax
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0x14], edx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebx + 0x10], edx
        mov    ecx, dword ptr [ebx + 0x25]
        or     ecx, ecx
        je     loc_1_120D
        mov    edx, dword ptr [eax]
        sub    edx, ecx
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 0x3c]
        shl    edx, cl
        add    dword ptr [eax + 4], edx
loc_1_120D:
        mov    ecx, dword ptr [esi + 0x7c]
        xor    edx, edx
loc_1_1212:
        add    edx, dword ptr [eax]
        cmp    edx, ecx
        jae    loc_1_121D
        add    eax, 8
        jmp    loc_1_1212
loc_1_121D:
        jne    loc_1_1224
        cmp    edx, 0x7e
        je     loc_1_1269
loc_1_1224:
        mov    dword ptr [ebx], eax
        sub    edx, ecx
        mov    edi, dword ptr [eax]
        mov    dword ptr [ebx + 8], edi
        mov    edi, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0xc], edi
        or     edx, edx
        je     loc_1_124A
        cmp    dword ptr [eax], edx
        jne    loc_1_124A
        xor    edx, edx
        mov    dword ptr [ebx + 0x25], edx
        xchg   dword ptr [eax], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ebx + 0x21], edx
        jmp    loc_1_1257
loc_1_124A:
        sub    dword ptr [eax], edx
        mov    dword ptr [ebx + 0x25], edx
        xor    edx, edx
        xchg   dword ptr [eax + 8], edx
        mov    dword ptr [ebx + 0x21], edx
loc_1_1257:
        push   esi
        call   sub_1_14DA
        add    esp, 4
        or     eax, eax
        jne    loc_1_1266
        clc    
loc_1_1265:
        ret    
loc_1_1266:
        stc    
        jmp    loc_1_1265
loc_1_1269:
        add    eax, 8
        cmp    dword ptr [eax], 0
        jne    loc_1_1212
        jmp    loc_1_1224
loc_1_1273:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x6a], 0x16
        mov    edx, dword ptr [eax + 0x14]
        sub    edx, 8
        mov    dword ptr [eax + 0x14], edx
        push   eax
        call   dword ptr [edx]
        add    esp, 4
        ret    
sub_1_128D:
        test   eax, 0x40000080
        jne    loc_1_12A4
        cmp    al, 0x1a
        jae    loc_1_1307
        mov    ebx, dword ptr [eax*4 + 0x20ac] ; -> obj1:0x20AC
        or     ebx, ebx
        je     loc_1_1307
        ret    
loc_1_12A4:
        sub    al, 0x80
        jb     loc_1_12BF
        cmp    al, 0x18
        ja     loc_1_1307
        push   eax
        and    eax, 0xff
        mov    ebx, dword ptr [eax*4 + 0x2114] ; -> obj1:0x2114
        pop    eax
        or     ebx, ebx
        je     loc_1_1307
        ret    
loc_1_12BF:
        add    al, 0x80
        cmp    al, 8
        jae    loc_1_1307
        and    eax, 0xff
        mov    ebx, dword ptr [eax*4 + 0x2174] ; -> obj1:0x2174
        or     eax, 0x40000000
        or     ebx, ebx
        je     loc_1_12DB
loc_1_12DA:
        ret    
loc_1_12DB:
        cmp    al, 0x1a
        jae    loc_1_1307
        and    eax, 0xff
        mov    ebx, dword ptr [eax*4 + 0x20ac] ; -> obj1:0x20AC
        or     eax, 0x40000000
        or     ebx, ebx
        je     loc_1_1307
        test   dword ptr [ebx + 0x20], 0x2000
        je     loc_1_1307
        test   byte ptr [ebx + 0x1d], 0x20
        je     loc_1_12DA
        or     al, al
        je     loc_1_12DA
loc_1_1307:
        stc    
        ret    
IOS_Send_Next_Command:
        jmp    sub_1_010B
        db  000h, 000h
sub_1_1310:
        pushal 
        mov    edi, dword ptr [esp + 0x24]
        movzx  ebx, word ptr [edi]
        cmp    ebx, 0x14
        ja     0x1387
        mov    word ptr [edi + 2], 0
        push   edi
        call   sub_1_1333
        add    esp, 4
        popal  
        ret    
sub_1_1333:
        in     al, 0xc
        add    byte ptr [eax], al
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        or     eax, 0x138f0000               ; -> obj1:0x138F
        add    byte ptr [eax], al
        loopne loc_1_1347
        add    byte ptr [eax], al
        mov    ah, 9
        add    byte ptr [eax], al
loc_1_1347:
        jo     0x135a
        add    byte ptr [eax], al
        hlt    
        adc    dword ptr [eax], eax
        add    byte ptr [esi], ch            ; -> loc_1_142E
        adc    al, 0
        add    byte ptr [ebp + 0x14], al     ; -> loc_1_1445
        add    byte ptr [eax], al
        sub    dword ptr [esi], ecx
        add    byte ptr [eax], al
        cld    
        push   cs
        add    byte ptr [eax], al
        sub    eax, 0xad000011               ; -> loc_4_12AD
        adc    al, byte ptr [eax]
        add    byte ptr [ebx], dl            ; -> loc_4_1313
        adc    eax, dword ptr [eax]
        add    byte ptr [ecx], ch            ; -> loc_4_1329
        adc    eax, dword ptr [eax]
        add    byte ptr [esi - 0x68ffffee], bh ; -> loc_4_12BE
        adc    eax, dword ptr [eax]
        add    bh, al                        ; -> loc_4_2EC7
        add    byte ptr cs:[eax], al
        cwde   
        adc    al, 0
        add    byte ptr [eax - 0x1cffffec], dh ; -> loc_4_14B0
        adc    al, 0
        add    byte ptr [esi - 0x39], ah
        inc    edi
        add    al, byte ptr [ecx]
        add    bl, ch
        mov    byte ptr [0xa80e478a], al
        adc    al, 0x75
        dec    edx
        movzx  esi, word ptr [edi + 4]
        mov    ecx, 0xc
        call   sub_1_072B
        or     ebx, ebx
        je     loc_1_13DC
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [edi + 6]
        sub    ecx, 0x64
        add    edx, ecx
        mov    word ptr [ebx - 2], cx
        mov    dword ptr [ecx + ebx], edx
        mov    word ptr [ecx + ebx + 0xe], 0
        mov    word ptr [ecx + ebx + 0xc], 0
        mov    dword ptr [ecx + ebx + 0x88], ebx
        mov    dword ptr [ecx + ebx + 4], 0
        mov    word ptr [ebx - 8], 1
loc_1_13DC:
        mov    dword ptr [edi + 0xa], ebx
        ret    
        db  0A8h, 010h, 075h, 01Eh, 08Bh, 05Fh, 00Ah, 08Dh, 043h, 0ECh, 066h, 0C7h, 040h, 010h, 014h, 000h
        db  0FAh, 08Bh, 00Dh
        dd  obj1:0x21BF              ; fixup
        db  089h, 048h, 004h, 0A3h
        dd  obj1:0x21BF              ; fixup
        db  0FBh, 0EBh, 0A6h, 00Fh, 0B7h, 077h, 004h, 083h, 0C6h, 014h, 0B0h, 020h, 0B9h, 00Ch, 000h, 000h
        db  000h, 0E8h, 016h, 0F3h, 0FFh, 0FFh, 00Bh, 0DBh, 075h, 08Fh, 0B0h, 001h, 0B9h, 00Ch, 000h, 000h
        db  000h, 0E8h, 006h, 0F3h, 0FFh, 0FFh, 00Bh, 0DBh, 074h, 0B3h, 0E9h, 07Ah, 0FFh, 0FFh, 0FFh
loc_1_142E:
        push   ebx
        mov    eax, dword ptr [edi + 8]
        call   sub_1_128D
        mov    dword ptr [edi + 4], ebx
        pop    ebx
        jb     loc_1_143E
        ret    
loc_1_143E:
        mov    word ptr [edi + 2], 1
        ret    
loc_1_1445:
        push   ebx
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [ecx + 0x30]
        jecxz  loc_1_1488
        mov    ebx, dword ptr [edi + 4]
        mov    al, byte ptr [edi + 0xc]
        cmp    al, 0xff
        je     loc_1_146F
        or     ebx, ebx
        jne    loc_1_1466
        mov    ebx, ecx
loc_1_1461:
        cmp    byte ptr [ebx + 0x40], al
        je     loc_1_1475
loc_1_1466:
        mov    ebx, dword ptr [ebx + 0xc]
        or     ebx, ebx
        je     loc_1_1488
        jmp    loc_1_1461
loc_1_146F:
        or     ebx, ebx
        jne    loc_1_1481
        mov    ebx, ecx
loc_1_1475:
        mov    dword ptr [edi + 8], ebx
        mov    word ptr [edi + 2], 0
        clc    
        pop    ebx
        ret    
loc_1_1481:
        mov    ebx, dword ptr [ebx + 0xc]
        or     ebx, ebx
        jne    loc_1_1475
loc_1_1488:
        stc    
        mov    word ptr [edi + 2], 1
        mov    dword ptr [edi + 8], 0
        pop    ebx
        ret    
loc_1_1498:
        pushal 
        push   0x186a0
        call   sub_1_1B9A
        add    esp, 4
        popal  
        ret    
sub_1_14A8:
        push   esi
        push   edi
        push   ebx
        push   ecx
        mov    esi, dword ptr [ebx + 8]
        or     edx, edx
        je     loc_1_14C6
loc_1_14B3:
        cmp    dword ptr [esi], edx
        je     loc_1_14C3
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_1_14B3
        mov    esi, dword ptr [ebx + 8]
        jmp    loc_1_14C6
loc_1_14C3:
        mov    esi, dword ptr [esi + 0xc]
loc_1_14C6:
        mov    dword ptr [edi + 0x10], esi
        mov    esi, edi
        or     byte ptr [esi + 0x6d], 4
        pop    ecx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 4], eax
        jmp    loc_1_0B26
sub_1_14DA:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0xc]
        mov    edi, dword ptr [ebx + 8]
        add    ebx, 0x64
        jmp    loc_1_14FA
loc_1_14E8:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0xc]
        mov    edi, dword ptr [ebx + 0x44]
        mov    edi, dword ptr [edi + 0x1c]
        sub    edi, 0xc3
loc_1_14FA:
        test   byte ptr [edi + 0x21], 0x20
        jne    loc_1_1502
        mov    edi, dword ptr [edi]
loc_1_1502:
        and    byte ptr [ebx + 0x4e], 0xfe
        test   dword ptr [edi + 0x1c], 0x600
        jne    loc_1_1579
loc_1_150F:
        mov    cl, byte ptr [edi + 0x3c]
        mov    eax, dword ptr [ebx + 0x18]
        test   byte ptr [ebx + 9], 2
        jne    loc_1_151D
        shl    eax, cl
loc_1_151D:
        test   byte ptr [edi + 0x21], 0x20
        jne    loc_1_1560
loc_1_1523:
        cmp    dword ptr [edi + 0x50], eax
        mov    al, 1
        jb     loc_1_15E1
        test   dword ptr [edi + 0x1c], 0x800
        jne    loc_1_1600
        test   dword ptr [ebx + 8], 2
        je     loc_1_155B
        movzx  ecx, byte ptr [edi + 0x77]
        shl    ecx, 1
        sub    ecx, 1
        mov    edi, dword ptr [ebx + 0x1c]
        add    edi, 8
        xor    eax, eax
        repne scasd eax, dword ptr es:[edi]
        mov    al, 1
        jne    loc_1_155D
loc_1_155B:
        xor    eax, eax
loc_1_155D:
        pop    edi
        pop    ebx
        ret    
loc_1_1560:
        mov    ecx, dword ptr [ebx + 0x1c]
        test   dword ptr [ebx + 8], 2
        je     loc_1_156F
        mov    ecx, dword ptr [ecx + 4]
loc_1_156F:
        and    ecx, 0xfff
        add    eax, ecx
        jmp    loc_1_1523
loc_1_1579:
        mov    al, 3
        test   dword ptr [edi + 0x1c], 0x400
        jne    loc_1_1586
        mov    al, 1
loc_1_1586:
        mov    cl, byte ptr [edi + 0x3c]
        test   dword ptr [ebx + 8], 2
        je     loc_1_15BA
        push   esi
        mov    esi, dword ptr [ebx + 0x1c]
loc_1_1596:
        test   byte ptr [esi + 4], al
        jne    loc_1_15EA
        mov    edx, dword ptr [esi]
        test   dword ptr [ebx + 8], 0x200
        jne    loc_1_15A8
        shl    edx, cl
loc_1_15A8:
        test   al, dl
        jne    loc_1_15EA
        add    esi, 8
        cmp    dword ptr [esi], 0
        jne    loc_1_1596
        pop    esi
        jmp    loc_1_150F
loc_1_15BA:
        cmp    word ptr [ebx + 4], 0xf
        je     loc_1_150F
        test   byte ptr [ebx + 0x1c], al
        jne    loc_1_15EB
        mov    edx, dword ptr [ebx + 0x18]
        test   dword ptr [ebx + 8], 0x200
        jne    loc_1_15D8
        shl    edx, cl
loc_1_15D8:
        test   al, dl
        jne    loc_1_15EB
        jmp    loc_1_150F
loc_1_15E1:
        or     byte ptr [ebx + 0xb], 8
        jmp    loc_1_155D
loc_1_15EA:
        pop    esi
loc_1_15EB:
        or     byte ptr [ebx + 0x4e], 1
        mov    al, 1
        test   byte ptr [edi + 0x1d], 8
        jne    loc_1_150F
        jmp    loc_1_155D
loc_1_1600:
        push   esi
        push   ebp
        push   edi
        mov    eax, dword ptr [ebx + 0x18]
        test   dword ptr [ebx + 8], 0x200
        jne    loc_1_1614
        mov    cl, byte ptr [edi + 0x3c]
        shl    eax, cl
loc_1_1614:
        cmp    eax, 0x80000
        ja     loc_1_1750
        shr    eax, 0xa
        add    eax, 0x15
        sub    esp, eax
        mov    ebp, esp
        mov    dword ptr [ebp], eax
        mov    byte ptr [ebp + 0xb], cl
        mov    al, 0x11
        mov    ah, byte ptr [edi + 0x77]
        cmp    al, ah
        jb     loc_1_163A
        mov    al, ah
loc_1_163A:
        mov    byte ptr [ebp + 4], al
        mov    edi, dword ptr [ebx + 0x48]
        xor    ecx, ecx
        mov    byte ptr [ebx + 0x4c], cl
        mov    edx, dword ptr [ebx + 0x18]
        mov    eax, dword ptr [ebx + 0x1c]
        test   dword ptr [ebx + 8], 2
        je     loc_1_165B
loc_1_1654:
        mov    edx, dword ptr [eax + ecx*8]
        mov    eax, dword ptr [eax + ecx*8 + 4]
loc_1_165B:
        push   ecx
        push   ebx
        test   dword ptr [ebx + 8], 0x200
        jne    loc_1_166B
        mov    cl, byte ptr [ebp + 0xb]
        shl    edx, cl
loc_1_166B:
        mov    esi, eax
        mov    dword ptr [ebp + 5], edx
        shr    edx, 0xc
        add    edx, 2
        shr    eax, 0xc
        lea    ecx, [ebp + 0xc]
        push   0
        push   ecx
        push   edx
        push   eax
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        xor    ebx, ebx
        mov    ecx, 0xffffffff
loc_1_1691:
        inc    ecx
        sub    dword ptr [ebp + 5], ebx
        mov    eax, esi
        and    eax, 0xfff
        mov    ebx, dword ptr [ebp + ecx*4 + 0xc]
        and    ebx, 0xfffff000
        add    ebx, eax
        jecxz  loc_1_171C
        mov    edx, dword ptr [edi - 8]
        add    edx, dword ptr [edi - 4]
        cmp    ebx, edx
        jne    loc_1_171C
        cmp    dword ptr [edi - 4], 0xefff
        jae    loc_1_171C
        mov    ebx, 0x1000
        sub    ebx, eax
        add    dword ptr [edi - 4], ebx
        add    esi, ebx
        cmp    ebx, dword ptr [ebp + 5]
        jb     loc_1_1691
        sub    ebx, dword ptr [ebp + 5]
        sub    dword ptr [edi - 4], ebx
loc_1_16D4:
        pop    ebx
        pop    ecx
        test   dword ptr [ebx + 8], 2
        je     loc_1_16ED
        inc    ecx
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    dword ptr [eax + ecx*8], 0
        jne    loc_1_1654
loc_1_16ED:
        add    esp, dword ptr [ebp]
        pop    edi
        pop    ebp
        pop    esi
        test   byte ptr [ebx + 0x4e], 1
        jne    loc_1_170F
loc_1_16F9:
        or     byte ptr [ebx + 0xb], 4
        test   dword ptr [edi + 0x1c], 0x1000
        je     loc_1_155B
        jmp    loc_1_1579
loc_1_170F:
        cmp    byte ptr [ebx + 0x4c], 1
        je     loc_1_16F9
        mov    al, 1
        jmp    loc_1_155D
loc_1_171C:
        mov    dword ptr [edi], ebx
        pop    ebx
        inc    byte ptr [ebx + 0x4c]
        push   ebx
        mov    bl, byte ptr [ebx + 0x4c]
        cmp    bl, byte ptr [ebp + 4]
        ja     loc_1_174B
        mov    ebx, 0x1000
        sub    ebx, eax
        mov    dword ptr [edi + 4], ebx
        add    esi, ebx
        add    edi, 8
        cmp    ebx, dword ptr [ebp + 5]
        jb     loc_1_1691
        mov    ebx, dword ptr [ebp + 5]
        mov    dword ptr [edi - 4], ebx
        jmp    loc_1_16D4
loc_1_174B:
        pop    ebx
        pop    ecx
        add    esp, dword ptr [ebp]
loc_1_1750:
        mov    al, 1
        pop    edi
        pop    ebp
        pop    esi
        jmp    loc_1_15E1
sub_1_175A:
        pushal 
        xor    eax, eax
        lea    esi, [0x1fa2]                 ; -> obj1:0x1FA2
loc_1_1763:
        mov    esi, dword ptr [esi + 2]
        or     esi, esi
        je     loc_1_1795
        cmp    dword ptr [esi + 8], 0
        je     loc_1_1763
        mov    ebx, dword ptr [esi + 0xc]
loc_1_1773:
        mov    dword ptr [edi + 4], ebx
        pushal 
        push   edi
        call   dword ptr [esi + 8]
        add    esp, 4
        popal  
        cmp    word ptr [edi + 2], 0
        jne    loc_1_1788
        mov    al, 1
loc_1_1788:
        or     ebx, ebx
        je     loc_1_1763
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    loc_1_1773
        jmp    loc_1_1763
loc_1_1795:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_1_179B:
        pushfd 
        cli    
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edx, dword ptr [ecx + 0x40]
        mov    dword ptr [ecx + 0x40], eax
        mov    dword ptr [eax + 0x24], edx
        popfd  
        ret    
sub_1_17AE:
        pushfd 
        cli    
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        lea    eax, [ecx + 0x40]
        sub    eax, 0x24
        mov    ecx, dword ptr [ecx + 0x40]
loc_1_17BF:
        jecxz  loc_1_17D5
        cmp    ecx, edi
        je     loc_1_17CC
        mov    eax, ecx
        mov    ecx, dword ptr [ecx + 0x24]
        jmp    loc_1_17BF
loc_1_17CC:
        mov    ecx, dword ptr [ecx + 0x24]
        mov    dword ptr [eax + 0x24], ecx
        popfd  
        clc    
        ret    
loc_1_17D5:
        popfd  
        stc    
        ret    
sub_1_17D8:
        cli    
        call   sub_1_1E5C
        jae    loc_1_1802
        mov    ebp, eax
        inc    byte ptr [0x21be]             ; -> obj1:0x21BE
        mov    eax, dword ptr [0x21ba]       ; -> obj1:0x21BA
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, ebp
        dec    byte ptr [0x21be]             ; -> obj1:0x21BE
        jmp    sub_1_17D8
loc_1_1802:
        sti    
        ret    
IOSIdleStatus:
        xor    eax, eax
        mov    ecx, dword ptr [0x21bf]       ; -> obj1:0x21BF
loc_1_180C:
        or     ecx, ecx
        je     loc_1_182A
        movzx  edx, word ptr [ecx + 0x12]
        add    edx, ecx
        add    edx, 0x14
        test   byte ptr [ecx + 0x10], 4
        je     loc_1_1825
        cmp    dword ptr [edx + 4], 0
        jne    loc_1_1867
loc_1_1825:
        mov    ecx, dword ptr [ecx + 4]
        jmp    loc_1_180C
loc_1_182A:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        add    ecx, 0x6e
loc_1_1833:
        movzx  edx, word ptr [ecx + 0xe]
        or     edx, edx
        je     loc_1_1868
        cmp    word ptr [ecx + 0xc], 1
        jne    loc_1_185B
        test   word ptr [ecx + 0x10], 4
        je     loc_1_185B
        push   edx
        movzx  edx, word ptr [ecx + 0x12]
        add    edx, ecx
        add    edx, 0x14
        cmp    dword ptr [edx + 4], 0
        pop    edx
        jne    loc_1_1867
loc_1_185B:
        cmp    dword ptr [ecx + 8], 0
        je     loc_1_1868
        add    edx, ecx
        mov    ecx, edx
        jmp    loc_1_1833
loc_1_1867:
        dec    eax
loc_1_1868:
        ret    
        db  02Eh, 08Bh, 0C0h
IOS_Signal_Semaphore_No_Switch:
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031810100
        mov    eax, eax
IOS_Set_Async_Time_Out:
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe800100
IOS_Exclusive_Access:
        push   esi
        push   ebx
        push   ebp
        mov    ebp, ebx
        push   eax
        and    eax, 0x400000ff
        call   sub_1_128D
        pop    eax
        xchg   ebx, ebp
        jb     loc_1_18CB
        cmp    byte ptr [ebp + 0x40], 0xa
        stc    
        jne    loc_1_18CB
        jecxz  loc_1_18AB
        and    byte ptr [ebp + 0x24], 0xfe
        test   eax, 0x100
        je     loc_1_18A7
        or     byte ptr [ebp + 0x24], 1
loc_1_18A7:
        mov    ecx, ebx
        xor    ebx, ebx
loc_1_18AB:
        cli    
        cmp    dword ptr [ebp + 0x41], ebx
        stc    
        jne    loc_1_18CB
        mov    dword ptr [ebp + 0x41], ecx
        mov    ebx, dword ptr [ebp + 0x18]
loc_1_18B8:
        or     ebx, ebx
        je     loc_1_18CB
        and    byte ptr [ebx + 5], 0x7f
        jecxz  loc_1_18C6
        or     byte ptr [ebx + 5], 0x80
loc_1_18C6:
        mov    ebx, dword ptr [ebx + 0x24]
        jmp    loc_1_18B8
loc_1_18CB:
        sti    
        pop    ebp
        pop    ebx
        pop    esi
        ret    
sub_1_18D0:
        mov    edx, dword ptr [esp + 4]
        cli    
        cmp    byte ptr [edx + 0x76], 0
        jne    loc_1_18F6
        test   byte ptr [edx + 0x22], 0x20
        je     loc_1_1906
        cmp    dword ptr [edx + 0xfb], 0
        jne    loc_1_1937
        cmp    dword ptr [edx + 0xf7], 0
        jne    loc_1_191A
loc_1_18F3:
        xor    eax, eax
        ret    
loc_1_18F6:
        mov    eax, dword ptr [edx + 0xf7]
        or     eax, eax
        je     loc_1_18F3
        test   byte ptr [eax + 0x6d], 8
        je     loc_1_18F3
loc_1_1906:
        cmp    dword ptr [edx + 0xf7], 0
        jne    loc_1_191A
        cmp    dword ptr [edx + 0xfb], 0
        jne    loc_1_1937
        jmp    loc_1_18F3
loc_1_191A:
        and    byte ptr [edx + 0x22], 0xdf
        mov    eax, dword ptr [edx + 0xf7]
loc_1_1924:
        mov    ecx, dword ptr [eax + 0x64]
        mov    dword ptr [edx + 0xf7], ecx
        or     ecx, ecx
        jne    loc_1_195C
        or     byte ptr [edx + 0x22], 0x20
        jmp    loc_1_195C
loc_1_1937:
        mov    eax, dword ptr [edx + 0xf7]
        or     eax, eax
        jne    loc_1_1981
loc_1_1941:
        or     byte ptr [edx + 0x22], 0x20
        mov    eax, dword ptr [edx + 0xfb]
        mov    ecx, dword ptr [eax + 0x64]
        mov    dword ptr [edx + 0xfb], ecx
        or     ecx, ecx
        jne    loc_1_195C
        and    byte ptr [edx + 0x22], 0xdf
loc_1_195C:
        cmp    word ptr [eax + 0x68], 2
        ja     loc_1_1975
        mov    ecx, dword ptr [eax + 0x7c]
        test   byte ptr [eax + 0x6d], 2
        jne    loc_1_1976
loc_1_196C:
        add    ecx, dword ptr [eax + 0x74]
        mov    dword ptr [edx + 0x10b], ecx
loc_1_1975:
        ret    
loc_1_1976:
        xchg   ecx, eax
        push   ecx
        mov    cl, byte ptr [edx + 0x3c]
        shr    eax, cl
        pop    ecx
        xchg   ecx, eax
        jmp    loc_1_196C
loc_1_1981:
        test   byte ptr [eax + 0x6d], 8
        jne    loc_1_1924
        jmp    loc_1_1941
sub_1_1989:
        push   esi
        push   ebx
        push   edi
        push   ebp
        pushfd 
        mov    edx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        cmp    word ptr [edx + 0x68], 0x24
        je     loc_1_19B9
loc_1_199D:
        test   byte ptr [edx + 0x6d], 8
        jne    loc_1_19DC
        movzx  ecx, byte ptr [esi + 0xb9]
        jmp    dword ptr [ecx*4 + 0x19b1]    ; -> obj1:0x19B1
        ; ---- dispatch table (2 entries) ----
        dd  loc_1_19EE
        dd  loc_1_1A0C
loc_1_19B9:
        cli    
        xor    ebp, ebp
        xchg   dword ptr [esi + 0xf7], ebp
        xor    edi, edi
        xchg   dword ptr [esi + 0xfb], edi
        call   sub_1_1A8C
        mov    edi, ebp
        call   sub_1_1A8C
        mov    edx, dword ptr [esp + 0x18]
        jmp    loc_1_199D
loc_1_19DC:
        cli    
        mov    ecx, dword ptr [esi + 0xf7]
        mov    dword ptr [edx + 0x64], ecx
        mov    dword ptr [esi + 0xf7], edx
        jmp    loc_1_1A6A
loc_1_19EE:
        lea    ecx, [esi + 0xf7]
        cli    
loc_1_19F5:
        cmp    dword ptr [ecx], 0
        je     loc_1_1A01
        mov    ecx, dword ptr [ecx]
        lea    ecx, [ecx + 0x64]
        jmp    loc_1_19F5
loc_1_1A01:
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 0x64], 0
        jmp    loc_1_1A6A
loc_1_1A0C:
        cmp    word ptr [edx + 0x68], 2
        ja     loc_1_19EE
        mov    ecx, dword ptr [edx + 0x74]
        mov    edi, dword ptr [edx + 0x6c]
        and    edi, 1
        test   dword ptr [esi + 0x20], 0x200000
        jne    loc_1_1A50
        cmp    ecx, dword ptr [esi + 0x10b]
        jae    loc_1_1A58
loc_1_1A2D:
        cli    
        lea    ebx, [esi + 0xfb]
loc_1_1A34:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     loc_1_1A65
        mov    ebp, dword ptr [eax + 0x6c]
        and    ebp, 1
        cmp    edi, ebp
        ja     loc_1_1A65
        jb     loc_1_1A4B
        cmp    ecx, dword ptr [eax + 0x74]
        jae    loc_1_1A65
loc_1_1A4B:
        lea    ebx, [eax + 0x64]
        jmp    loc_1_1A34
loc_1_1A50:
        cmp    ecx, dword ptr [esi + 0x10b]
        jbe    loc_1_1A2D
loc_1_1A58:
        cli    
        lea    ebx, [esi + 0xf7]
loc_1_1A5F:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        jne    loc_1_1A70
loc_1_1A65:
        mov    dword ptr [ebx], edx
        mov    dword ptr [edx + 0x64], eax
loc_1_1A6A:
        popfd  
        pop    ebp
        pop    edi
        pop    ebx
        pop    esi
        ret    
loc_1_1A70:
        test   byte ptr [eax + 0x6d], 8
        jne    loc_1_1A87
        mov    ebp, dword ptr [eax + 0x6c]
        and    ebp, 1
        cmp    edi, ebp
        ja     loc_1_1A65
        jb     loc_1_1A87
        cmp    ecx, dword ptr [eax + 0x74]
        jbe    loc_1_1A65
loc_1_1A87:
        lea    ebx, [eax + 0x64]
        jmp    loc_1_1A5F
sub_1_1A8C:
        or     edi, edi
        je     loc_1_1AAC
        mov    edx, dword ptr [edi + 0x64]
        mov    ecx, dword ptr [edi + 0x14]
        sub    ecx, 8
        mov    dword ptr [edi + 0x14], ecx
        mov    word ptr [edi + 0x6a], 0x11
        push   edx
        push   edi
        call   dword ptr [ecx]
        add    esp, 4
        pop    edi
        jmp    sub_1_1A8C
loc_1_1AAC:
        ret    
loc_1_1AAD:
        xor    ecx, ecx
        jmp    loc_1_1AB6
sub_1_1AB1:
        mov    ecx, 0x100
loc_1_1AB6:
        pushal 
        mov    esi, dword ptr [edx + 4]
        add    esi, 0x64
        sub    esp, 0x10
        mov    ebx, esp
        mov    dword ptr [ebx + 6], esi
        add    esi, 0x58
        mov    word ptr [ebx + 4], si
        mov    byte ptr [ebx + 0xe], 0
        mov    word ptr [ebx], 2
        push   ebx
        call   sub_1_1310
        mov    ebx, dword ptr [ebx + 0xa]
        add    esp, 0x14
        or     ebx, ebx
        je     loc_1_1B29
        add    ebx, dword ptr [edx + 4]
        mov    eax, dword ptr [esp + 0x1c]
        mov    word ptr [ebx + 0x68], ax
        xor    eax, eax
        mov    dword ptr [ebx + 0x64], eax
        mov    dword ptr [ebx + 0xa8], eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [ebx + 0x80], eax
        mov    ecx, dword ptr [esp + 0x18]
        or     ecx, 0x4408
        mov    dword ptr [ebx + 0x6c], ecx
        mov    dword ptr [ebx + 0x70], 0x1b2b ; -> loc_1_1B2B
        mov    dword ptr [ebx + 0xa4], ebx
        mov    edi, ebx
        mov    ebx, edx
        xor    edx, edx
        call   sub_1_14A8
loc_1_1B29:
        popal  
        ret    
loc_1_1B2B:
        mov    edi, dword ptr [esp + 4]
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [edi + 0x88]
        jmp    sub_1_0A1E
loc_1_1B3D:
        sub    esp, 0x10
        cmp    dword ptr [esp + 0x14], 0x219
        jne    loc_1_1B93
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 0x8045
        jne    loc_1_1B76
        mov    eax, dword ptr [esp + 0x1c]
        cmp    ah, 4
        jne    loc_1_1B93
        movzx  eax, al
        push   edi
        lea    edi, [esp + 4]
        mov    word ptr [edi], 0x14
        mov    dword ptr [edi + 0xc], eax
        call   sub_1_175A
        pop    edi
        jmp    loc_1_1B93
loc_1_1B76:
        cmp    eax, 0x20
        jne    loc_1_1B93
        xchg   byte ptr [0x343], ah          ; -> obj5:0x0343
        test   ah, ah
        je     loc_1_1B93
        push   0xf0                          ; -> loc_7_00F0
        VxDCall  SHELL._SHELL_ShellExecute       ; cd2007001700
        add    esp, 4
loc_1_1B93:
        xor    eax, eax
        inc    eax
        add    esp, 0x10
        ret    
sub_1_1B9A:
        mov    eax, dword ptr [esp + 4]
        sub    eax, 2
        ja     loc_1_1BA4
loc_1_1BA3:
        ret    
loc_1_1BA4:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [ecx + 0x14]
        cdq    
        mul    ecx
        shrd   eax, edx, 0x10
        mov    ecx, eax
loc_1_1BB6:
        loop   loc_1_1BB6
        jmp    loc_1_1BA3
        db  000h, 000h
loc_1_1BBC:
        cld    
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    eax, dword ptr [ecx + 0x10]
        inc    eax
        mov    dword ptr [ecx + 0x10], eax
        test   eax, 1
        jne    loc_1_1D27
        mov    eax, 7
        mov    edx, 0x2000
        call   sub_1_1D66
        test   dword ptr [ecx + 0x10], 3
        jne    loc_1_1C00
        mov    eax, 8
        mov    edx, 0x2000
        call   sub_1_1D66
loc_1_1C00:
        test   dword ptr [ecx + 0x10], 7
        jne    loc_1_1C18
        mov    eax, 9
        mov    edx, 0x4000
        call   sub_1_1D66
loc_1_1C18:
        test   dword ptr [ecx + 0x10], 0xf
        jne    loc_1_1C96
        cmp    dword ptr [0x2220], 0         ; -> obj1:0x2220
        je     loc_1_1C66
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edx, dword ptr [edx + 0x30]
loc_1_1C33:
        or     edx, edx
        je     loc_1_1C66
        test   byte ptr [edx + 0x21], 0x80
        je     loc_1_1C61
        test   byte ptr [edx + 0x23], 0x80
        je     loc_1_1C61
        inc    dword ptr [edx + 0xbf]
        mov    eax, dword ptr [0x2220]       ; -> obj1:0x2220
        cmp    dword ptr [edx + 0xbf], eax
        jne    loc_1_1C61
        mov    esi, 0x1dae                   ; -> loc_1_1DAE
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_1_1C61:
        mov    edx, dword ptr [edx + 0xc]
        jmp    loc_1_1C33
loc_1_1C66:
        mov    eax, 0xa
        mov    edx, 0x8000
        call   sub_1_1D66
        cmp    dword ptr [0x21bf], 0         ; -> obj1:0x21BF
        je     loc_1_1C96
        bts    word ptr [0x2248], 0          ; -> obj1:0x2248
        jb     loc_1_1C96
        push   esi
        mov    esi, 0x1db7                   ; -> loc_1_1DB7
        VMMCall  VMM.Call_Global_Event           ; cd2010000100
        pop    esi
loc_1_1C96:
        mov    ebx, 0x6e
        add    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cli    
loc_1_1CA2:
        movzx  eax, word ptr [ebx + 0xe]
        or     eax, eax
        je     loc_1_1CE7
        cmp    word ptr [ebx + 0xc], 1
        jne    loc_1_1CD4
        test   word ptr [ebx + 0x10], 4
        je     loc_1_1CD4
        movzx  edx, word ptr [ebx + 0x12]
        movzx  ecx, word ptr [edx + ebx + 0x20]
        jecxz  loc_1_1CD4
        loop   loc_1_1CC6
loc_1_1CC6:
        mov    word ptr [edx + ebx + 0x20], cx
        or     ecx, ecx
        jne    loc_1_1CD4
        call   sub_1_1D41
loc_1_1CD4:
        cmp    dword ptr [ebx + 8], 0
        je     loc_1_1CE7
        add    eax, ebx
        cmp    eax, dword ptr [ebx + 8]
        je     loc_1_1CE3
        jmp    loc_1_1CE7
loc_1_1CE3:
        mov    ebx, eax
        jmp    loc_1_1CA2
loc_1_1CE7:
        mov    ebx, dword ptr [0x21bf]       ; -> obj1:0x21BF
loc_1_1CED:
        or     ebx, ebx
        je     loc_1_1D27
        test   word ptr [ebx + 0x10], 4
        je     loc_1_1D22
        cmp    word ptr [ebx + 0xc], 1
        jne    loc_1_1D22
        test   word ptr [ebx + 0x10], 4
        je     loc_1_1D22
        movzx  edx, word ptr [ebx + 0x12]
        movzx  ecx, word ptr [edx + ebx + 0x20]
        jecxz  loc_1_1D22
        dec    ecx
        mov    word ptr [edx + ebx + 0x20], cx
        or     ecx, ecx
        jne    loc_1_1D22
        call   sub_1_1D41
loc_1_1D22:
        mov    ebx, dword ptr [ebx + 4]
        jmp    loc_1_1CED
loc_1_1D27:
        sti    
        mov    eax, 0xfa
        mov    esi, 0x1bbc                   ; -> loc_1_1BBC
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        or     esi, esi
        jne    loc_1_1D3C
loc_1_1D3C:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_1_1D41:
        pushal 
        mov    ecx, ebx
        add    ecx, edx
        add    ecx, 0x14
        sub    esp, 0x10
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 0xc], ecx
        call   sub_1_175A
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1D64
loc_1_1D62:
        popal  
        ret    
loc_1_1D64:
        jmp    loc_1_1D62
sub_1_1D66:
        mov    esi, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
loc_1_1D6C:
        or     esi, esi
        je     loc_1_1DAD
        cmp    dword ptr [esi + 8], 0
        je     loc_1_1DA8
        test   dword ptr [esi + 0x38], edx
        je     loc_1_1DA8
        mov    ebx, dword ptr [esi + 0xc]
loc_1_1D7E:
        push   eax
        push   edx
        push   ebx
        push   esi
        push   ecx
        sub    esp, 0xc
        mov    edi, esp
        mov    word ptr [edi], ax
        mov    dword ptr [edi + 4], ebx
        push   edi
        call   dword ptr [esi + 8]
        pop    edi
        add    esp, 0xc
        pop    ecx
        pop    esi
        pop    ebx
        pop    edx
        pop    eax
        or     ebx, ebx
        je     loc_1_1DA8
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    loc_1_1D7E
        jmp    loc_1_1DA8
loc_1_1DA8:
        mov    esi, dword ptr [esi + 2]
        jmp    loc_1_1D6C
loc_1_1DAD:
        ret    
loc_1_1DAE:
        mov    ax, 0x25
        jmp    loc_1_1AAD
loc_1_1DB7:
        cli    
        and    byte ptr [0x2248], 0xfe       ; -> obj1:0x2248
        mov    ebx, dword ptr [0x21bf]       ; -> obj1:0x21BF
        mov    esi, 0x21bb                   ; -> obj1:0x21BB
loc_1_1DCA:
        or     ebx, ebx
        je     loc_1_1DF4
        test   word ptr [ebx + 0x10], 0x14
        je     loc_1_1DDD
        mov    esi, ebx
        mov    ebx, dword ptr [ebx + 4]
        jmp    loc_1_1DCA
loc_1_1DDD:
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], ecx
        sti    
        push   0
        push   dword ptr [ebx + 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_1_1DB7
loc_1_1DF4:
        sti    
        ret    
        db  000h, 000h
loc_1_1DF8:
        dec    esi
        push   ds
        add    byte ptr [eax], al
        dec    esi
        push   ds
        add    byte ptr [eax], al
        push   esp
        push   ds
        add    byte ptr [eax], al
        push   esp
        push   ds
        add    byte ptr [eax], al
        push   eax
        push   ds
        add    byte ptr [eax], al
        push   eax
        push   ds
        add    byte ptr [eax], al
        dec    esi
        push   ds
        add    byte ptr [eax], al
        push   eax
        push   ds
        add    byte ptr [eax], al
sub_1_1E18:
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [0x2225]       ; -> loc_1_2225
        xor    ecx, ecx
        cmp    byte ptr [eax + 0x12b9], 0
        je     loc_1_1E2F
        or     ecx, 1
loc_1_1E2F:
        cmp    byte ptr [eax + 0x12b8], 0
        je     loc_1_1E3B
        or     ecx, 2
loc_1_1E3B:
        cmp    byte ptr [eax + 0x321], 0
        je     loc_1_1E47
        or     ecx, 4
loc_1_1E47:
        jmp    loc_1_1DF8
loc_1_1E4E:
        clc    
        ret    
loc_1_1E50:
        xor    ecx, ecx
        stc    
        ret    
loc_1_1E54:
        or     ecx, 0xffffffff
        stc    
        ret    
        db  000h, 000h, 000h
sub_1_1E5C:
        mov    ecx, dword ptr [0x21bf]       ; -> obj1:0x21BF
loc_1_1E62:
        or     ecx, ecx
        je     loc_1_1E7F
        movzx  edx, word ptr [ecx + 0x12]
        add    edx, ecx
        add    edx, 0x14
        test   byte ptr [ecx + 0x10], 4
        je     loc_1_1E7A
        cmp    dword ptr [edx + 4], eax
        je     loc_1_1EBB
loc_1_1E7A:
        mov    ecx, dword ptr [ecx + 4]
        jmp    loc_1_1E62
loc_1_1E7F:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        add    ecx, 0x6e
loc_1_1E88:
        movzx  edx, word ptr [ecx + 0xe]
        or     edx, edx
        je     loc_1_1EBD
        cmp    word ptr [ecx + 0xc], 1
        jne    loc_1_1EAF
        test   word ptr [ecx + 0x10], 4
        je     loc_1_1EAF
        push   edx
        movzx  edx, word ptr [ecx + 0x12]
        add    edx, ecx
        add    edx, 0x14
        cmp    dword ptr [edx + 4], eax
        pop    edx
        je     loc_1_1EBB
loc_1_1EAF:
        cmp    dword ptr [ecx + 8], 0
        je     loc_1_1EBD
        add    edx, ecx
        mov    ecx, edx
        jmp    loc_1_1E88
loc_1_1EBB:
        stc    
        ret    
loc_1_1EBD:
        clc    
        ret    
        db  000h
IOSMapIORSToI24:
        push   ebx
        mov    ebx, eax
        mov    eax, 0x100b
        cmp    ebx, 0x2c
        jae    loc_1_1ED8
        movzx  eax, word ptr [ebx*2 + 0x2249] ; -> loc_1_2249
loc_1_1ED8:
        jecxz  loc_1_1EE0
        cmp    al, 0xb
        jne    loc_1_1EE0
        mov    al, 0xa
loc_1_1EE0:
        pop    ebx
        ret    
IOSMapIORSToI21:
        mov    eax, 0x14
        sub    ecx, 0xf
        jb     loc_1_1EFB
        cmp    ecx, 0x1d
        jae    loc_1_1EFB
        movzx  eax, byte ptr [ecx + 0x224c]  ; -> obj1:0x224C
loc_1_1EFB:
        ret    
        dd  IOS_Get_Version          ; fixup
        dd  IOS_BD_Register_Device   ; fixup
        dd  IOS_Find_Int13_Drive     ; fixup
        dd  IOS_Get_Device_List      ; fixup
        dd  IOS_SendCommand          ; fixup
        dd  IOS_BD_Command_Complete  ; fixup
        dd  IOS_Synchronous_Command  ; fixup
        dd  IOS_Register             ; fixup
        dd  IOS_Requestor_Service    ; fixup
        dd  IOS_Exclusive_Access     ; fixup
        dd  IOS_Send_Next_Command    ; fixup
        dd  IOS_Set_Async_Time_Out   ; fixup
        dd  IOS_Signal_Semaphore_No_Switch ; fixup
        dd  IOSIdleStatus            ; fixup
        dd  IOSMapIORSToI24          ; fixup
        dd  IOSMapIORSToI21          ; fixup
        dd  PrintLog                 ; fixup
        dd  IOS_Service_17           ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00010h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IOS     '                  ; DDB_Name
        dd  017000000h                  ; DDB_Init_Order
        dd  IOS_Control              ; DDB_Control_Proc
        dd  IOS_V86_API              ; DDB_V86_API_Proc
        dd  IOS_PM_API               ; DDB_PM_API_Proc
        dd  obj1:0x1EFC              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_2207:
        jae    loc_1_221B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
loc_1_221B:
        add    byte ptr [eax], al
loc_1_221D:
        add    byte ptr [eax], al
loc_1_221F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2225:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2245:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2249:
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x1b], dl
        pop    ds
        pop    ds
        pop    ds
        push   ds
        pop    ds
        push   ss
        adc    al, 0x15
        mov    dword ptr [0x80b1f1f], eax
        pop    ds
        adc    edx, dword ptr [0xb2b1b01f]
        mov    bl, 0xb4
        mov    ch, 0x1f
        sub    byte ptr [edi], bl
        jo     0x2272
        add    al, 0xc
        add    dword ptr [ecx + eax], ecx
        or     al, 1
        or     edx, dword ptr [eax]
        or     al, 1
        add    eax, dword ptr [ecx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [eax + 0x10c010f]
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        add    byte ptr [ebx], al
        add    al, byte ptr [eax + 0x10c010c]
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        sidt   [ecx + eax]
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x167  R-X
        db  003h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        je     loc_2_0010
loc_2_0010:
        inc    dword ptr [eax]
        push   ebx
        add    dword ptr [ebp + 0x72], eax
        jb     0x87
        jb     loc_2_0054
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [ebx + 0x75], dh
        bound  esi, qword ptr [ebx + 0x79]
        jae    0x9a
        insd   dword ptr es:[edi], dx
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        jae    loc_2_004F
        insd   dword ptr es:[edi], dx
        imul   esi, dword ptr [ebx + 0x73], 0x20676e69
        outsd  dx, dword ptr [esi]
        jb     0x5a
        pop    esp
        imul   ebp, dword ptr [edi + 0x73], 0x79736275
        jae    loc_2_0065
        imul   esi, dword ptr fs:[edx + 0x65], 0x726f7463
        jns    0x6f
loc_2_004F:
        or     eax, 0x746f6e0a
loc_2_0054:
        and    byte ptr [esi + 0x6f], ah
        jne    0xc7
        or     eax, 0x6e75520a
        and    byte ptr [ecx + 0x4e], cl
        push   ebx
        push   esp
loc_2_0065:
        inc    ecx
        dec    esp
        dec    esp
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x20], 0x63206f74
        outsd  dx, dword ptr [esi]
        jb     0xe8
        arpl   word ptr gs:[eax + 0x74], si
        push   0x72702065
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        add    byte ptr cs:[eax], al
        inc    ebp
        jb     loc_2_00FD
        outsd  dx, dword ptr [esi]
        jb     loc_2_00C8
        and    byte ptr [ecx + 0x6e], al
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [ebx + 0x75], dh
        bound  esi, qword ptr [ebx + 0x79]
        jae    0x111
        insd   dword ptr es:[edi], dx
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x10a
        jb     0xc7
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        or     eax, 0x7469450a
        push   0x61207265
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ecx + 0x6e], ch
loc_2_00C8:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi], ch
        pop    esp
        imul   ebp, dword ptr [edi + 0x73], 0x79736275
        jae    0xf8
        jae    0x14f
        bound  esp, qword ptr [ecx + ebp*2 + 0x72]
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jns    0x105
        imul   esi, dword ptr [ebx + 0x20], 0x72726f63
        jne    loc_2_015E
        je     0x11c
        or     eax, 0x20726f0a
        je     0x15f
        and    byte ptr gs:[ebx + 0x79], dh
        jae    0x171
loc_2_00FD:
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [edi + ebp*2 + 0x77], ch
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x189
        add    byte ptr cs:[eax], al
        inc    ebp
        jb     0x188
        outsd  dx, dword ptr [esi]
        jb     0x153
        and    byte ptr [eax + ebp*2 + 0x65], dl
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [ebx + 0x75], dh
        bound  esi, qword ptr [ebx + 0x79]
        jae    0x19d
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebx + 0x6f], ah
        jne    0x19c
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x156
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        and    byte ptr gs:[esi + 0x65], ch
        and    byte ptr fs:[ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x1c5
        or     eax, 0x756f590a
        and    byte ptr [esi + 0x65], ch
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0x1d2
        outsb  dx, byte ptr [esi]
loc_2_015E:
        and    byte ptr [ebx + 0x65], dh
        je     0x1d8
        jo     0x193
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4310  R-X
loc_3_0000:
        sub    esp, 0xc
        mov    edi, esp
        mov    word ptr [edi], 0
        mov    word ptr [edi + 4], 0x20
        push   edi
        call   sub_4_0000
        add    esp, 0x10
        mov    word ptr [ebx + 2], 0
        ret    
loc_3_0021:
        mov    word ptr [ebx + 2], 2
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0xba], 1
        jae    loc_3_0040
        mov    byte ptr [esi + 0x82], 0
        mov    word ptr [ebx + 2], 0
loc_3_0040:
        ret    
loc_3_0041:
        mov    esi, dword ptr [0x1f94]       ; -> obj1:0x1F94
        push   esi
        push   edi
        lea    edi, [edi + 0xc3]
        mov    ecx, 0x19
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        mov    byte ptr [edi + 0x77], 0x11
        mov    eax, dword ptr [esi + 8]
        and    eax, 0x3f
        or     dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [edi + 0xdb]
        mov    dword ptr [edi + 0x5c], eax
        mov    eax, dword ptr [edi + 0xdf]
        mov    dword ptr [edi + 0x60], eax
        mov    eax, dword ptr [edi + 0xe3]
        mov    dword ptr [edi + 0x64], eax
        mov    eax, dword ptr [edi + 0xe7]
        mov    dword ptr [edi + 0x68], eax
        mov    dword ptr [edi + 0x50], 0xffffffff
        mov    byte ptr [edi + 0x77], 0xf
        mov    eax, dword ptr [edi + 0xd3]
        mov    dword ptr [edi + 0x54], eax
        mov    eax, dword ptr [edi + 0xd7]
        mov    dword ptr [edi + 0x58], eax
        mov    dword ptr [edi + 0x127], esi
        mov    dword ptr [edi + 0x50], 0xffffffff
        mov    al, byte ptr [esi + 7]
        mov    byte ptr [edi + 0x15], al
        mov    byte ptr [edi + 0xca], al
        sub    esp, 0xc
        mov    ebp, esp
        mov    word ptr [ebp], 6
        mov    byte ptr [ebp + 8], al
        mov    byte ptr [ebp + 9], 0
        mov    dword ptr [ebp + 4], edi
        push   ebp
        call   sub_1_1310
        add    esp, 0x10
        sub    esp, 0x18
        mov    esi, esp
        mov    word ptr [esi], 5
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], 0x228    ; -> loc_1_0228
        xor    ecx, ecx
        mov    word ptr [esi + 0x10], cx
        mov    dword ptr [esi + 0x12], ecx
        mov    cl, byte ptr [ebx + 8]
        mov    byte ptr [esi + 0x16], cl
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0xc], ecx
        push   esi
        call   sub_4_0000
        add    esp, 0x1c
        mov    word ptr [ebx + 2], cx
        ret    
sub_3_0116:
        cmp    byte ptr [0x1f98], 1          ; -> obj1:0x1F98
        je     loc_3_013E
        push   eax
        push   edi
        mov    edi, 0x3962                   ; -> loc_3_3962
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_013B
        VxDCall  VPICD.VPICD_Auto_Mask_At_Inst_Swap ; cd2012000300
        mov    byte ptr [0x1f98], 1          ; -> obj1:0x1F98
loc_3_013B:
        pop    edi
        pop    eax
        ret    
loc_3_013E:
        clc    
        ret    
loc_3_0140:
        adc    byte ptr [edi], ah
        db  0E8h, 003h, 064h, 000h
loc_3_0146:
        or     al, byte ptr [eax]
PrintLog:
        mov    byte ptr [0x221f], 1          ; -> loc_1_221F
        push   eax
        mov    eax, 0x3b00                   ; -> loc_3_3B00
        mov    dword ptr [0x3afc], eax       ; -> loc_3_3AFC
        pop    eax
        push   0
        push   ebp
        mov    ebp, esp
        pushal 
        mov    esi, dword ptr [ebp + 0xc]
loc_3_0164:
        lodsb  al, byte ptr [esi]
        or     al, al
        jne    loc_3_019D
        cmp    byte ptr [0x221f], 1          ; -> loc_1_221F
        jne    loc_3_0197
        xor    eax, eax
        mov    byte ptr [0x221f], al         ; -> loc_1_221F
        mov    edi, dword ptr [0x3afc]       ; -> loc_3_3AFC
        stosb  byte ptr es:[edi], al
        call   sub_3_33F4
        push   0x3b00                        ; -> loc_3_3B00
        call   sub_3_38A0
        add    esp, 4
        call   sub_3_346B
loc_3_0197:
        popal  
        pop    ebp
        add    esp, 4
        ret    
loc_3_019D:
        cmp    al, 0x25
        je     loc_3_01A6
        jmp    loc_3_05F7
loc_3_01A6:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x25
        jne    loc_3_01B2
        mov    al, 0x25
        jmp    loc_3_05F7
loc_3_01B2:
        cmp    al, 0x63
        jne    loc_3_01C9
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        mov    al, byte ptr [ebx]
        jmp    loc_3_05F7
loc_3_01C9:
        cmp    al, 0x73
        jne    loc_3_01E7
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
loc_3_01D9:
        mov    al, byte ptr [ebx]
        or     al, al
        je     loc_3_0164
        call   sub_3_0642
        inc    ebx
        jmp    loc_3_01D9
loc_3_01E7:
        cmp    al, 0x75
        jne    loc_3_0250
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        mov    ax, word ptr [ebx]
        xor    dx, dx
        div    word ptr [0x140]              ; -> loc_3_0140
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x142]              ; -> obj3:0x0142
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x144]              ; -> obj3:0x0144
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x146]              ; -> loc_3_0146
        or     al, 0x30
        call   sub_3_0642
        mov    al, dl
        or     al, 0x30
        jmp    loc_3_05F7
loc_3_0250:
        cmp    al, 0x64
        jne    loc_3_036C
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        mov    ax, word ptr [ebx]
        mov    dx, word ptr [ebx + 2]
        cmp    dx, 0x1998
        jbe    loc_3_02BD
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2c
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2c
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2a
        call   sub_3_0642
        mov    al, 0x2a
        jmp    loc_3_05F7
loc_3_02BD:
        div    word ptr [0x140]              ; -> loc_3_0140
        push   edx
        xor    dx, dx
        div    word ptr [0x140]              ; -> loc_3_0140
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x142]              ; -> obj3:0x0142
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x144]              ; -> obj3:0x0144
        or     al, 0x30
        call   sub_3_0642
        mov    al, 0x2c
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x146]              ; -> loc_3_0146
        or     al, 0x30
        call   sub_3_0642
        mov    al, dl
        or     al, 0x30
        call   sub_3_0642
        pop    eax
        xor    dx, dx
        div    word ptr [0x142]              ; -> obj3:0x0142
        or     al, 0x30
        call   sub_3_0642
        mov    al, 0x2c
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x144]              ; -> obj3:0x0144
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x146]              ; -> loc_3_0146
        or     al, 0x30
        call   sub_3_0642
        mov    al, dl
        or     al, 0x30
        jmp    loc_3_05F7
loc_3_036C:
        cmp    al, 0x65
        jne    loc_3_038F
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        add    ebx, 2
        call   sub_3_0635
        sub    ebx, 2
        call   sub_3_0635
        jmp    loc_3_0164
loc_3_038F:
        cmp    al, 0x69
        jne    loc_3_03F5
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        mov    eax, dword ptr [ebx]
        xor    dx, dx
        div    word ptr [0x140]              ; -> loc_3_0140
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x142]              ; -> obj3:0x0142
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x144]              ; -> obj3:0x0144
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x146]              ; -> loc_3_0146
        or     al, 0x30
        call   sub_3_0642
        mov    al, dl
        or     al, 0x30
        jmp    loc_3_05F7
loc_3_03F5:
        cmp    al, 0x6a
        jne    loc_3_0435
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        mov    eax, dword ptr [ebx]
        xor    ah, ah
        xor    dx, dx
        div    word ptr [0x144]              ; -> obj3:0x0144
        or     al, 0x30
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        div    word ptr [0x146]              ; -> loc_3_0146
        or     al, 0x30
        call   sub_3_0642
        mov    al, dl
        or     al, 0x30
        jmp    loc_3_05F7
loc_3_0435:
        cmp    al, 0x78
        jne    loc_3_044F
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        call   sub_3_0635
        jmp    loc_3_0164
loc_3_044F:
        cmp    al, 0x6c
        jne    loc_3_047F
        lodsb  al, byte ptr [esi]
        cmp    al, 0x78
        je     loc_3_045D
        jmp    loc_3_05D7
loc_3_045D:
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        add    ebx, 2
        call   sub_3_0635
        sub    bx, 2
        call   sub_3_0635
        jmp    loc_3_0164
loc_3_047F:
        cmp    al, 0x70
        jne    loc_3_04B7
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        add    ebx, 2
        call   sub_3_0635
        mov    al, 0x3a
        call   sub_3_0642
        sub    ebx, 2
        call   sub_3_0635
        mov    al, 0x20
        call   sub_3_0642
        mov    al, 0x20
        call   sub_3_0642
        jmp    loc_3_0164
loc_3_04B7:
        cmp    al, 0x62
        jne    loc_3_04CF
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        call   sub_3_0626
        jmp    loc_3_0164
loc_3_04CF:
        cmp    al, 0x77
        jne    loc_3_04E7
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        call   sub_3_0635
        jmp    loc_3_0164
loc_3_04E7:
        cmp    al, 0x7a
        je     loc_3_04F0
        jmp    loc_3_05D7
loc_3_04F0:
        lea    ebx, [ebp + 0x10]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 8
        mov    ecx, dword ptr [ebx + 4]
        mov    ebx, dword ptr [ebx]
loc_3_04FF:
        mov    ax, es
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        mov    al, 0x3a
        call   sub_3_0642
        mov    eax, ebx
        rol    eax, 0x10
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        rol    eax, 0x10
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        mov    al, 0x20
        call   sub_3_0642
        mov    al, 0x20
        call   sub_3_0642
        push   ecx
        mov    edx, 0x31
        cmp    ecx, 0x10
        jbe    loc_3_0577
        mov    ecx, 0x10
loc_3_0577:
        push   ebx
        push   ecx
loc_3_0579:
        mov    al, byte ptr [ebx]
        call   sub_3_0626
        mov    al, 0x20
        cmp    edx, 0x1c
        jne    loc_3_0589
        mov    al, 0x2d
loc_3_0589:
        call   sub_3_0642
        sub    edx, 3
        inc    ebx
        loop   loc_3_0579
        mov    ecx, edx
loc_3_0596:
        mov    al, 0x20
        call   sub_3_0642
        loop   loc_3_0596
        pop    ecx
        pop    ebx
loc_3_05A1:
        mov    al, 0x2e
        mov    ah, byte ptr [ebx]
        cmp    ah, 0x20
        jb     loc_3_05B1
        cmp    ah, 0x7f
        ja     loc_3_05B1
        xchg   ah, al
loc_3_05B1:
        call   sub_3_0642
        inc    ebx
        loop   loc_3_05A1
        pop    ecx
        sub    ecx, 0x10
        jle    loc_3_05D2
        mov    al, 0xd
        call   sub_3_0642
        mov    al, 0xa
        call   sub_3_0642
        jmp    loc_3_04FF
loc_3_05D2:
        jmp    loc_3_0164
loc_3_05D7:
        mov    ah, al
        mov    al, 0x3f
        call   sub_3_0642
        mov    al, 0x5c
        call   sub_3_0642
        mov    al, ah
        call   sub_3_0642
        mov    al, 0x5c
        call   sub_3_0642
        mov    al, 7
loc_3_05F7:
        call   sub_3_0642
        jmp    loc_3_0164
sub_3_0601:
        push   eax
        shr    al, 4
        add    al, 0x30
        cmp    al, 0x39
        jbe    loc_3_060D
        add    al, 7
loc_3_060D:
        call   sub_3_0642
        pop    eax
        ret    
sub_3_0614:
        push   eax
        and    al, 0xf
        add    al, 0x30
        cmp    al, 0x39
        jbe    loc_3_061F
        add    al, 7
loc_3_061F:
        call   sub_3_0642
        pop    eax
        ret    
sub_3_0626:
        mov    al, byte ptr [ebx]
        call   sub_3_0601
        mov    al, byte ptr [ebx]
        call   sub_3_0614
        ret    
sub_3_0635:
        inc    ebx
        call   sub_3_0626
        dec    ebx
        call   sub_3_0626
        ret    
sub_3_0642:
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        jne    loc_3_0654
        cmp    byte ptr [0x221f], 0          ; -> loc_1_221F
        jne    loc_3_065B
loc_3_0654:
        VMMCall  VMM.Out_Debug_Chr               ; cd20c3000100
        ret    
loc_3_065B:
        push   edi
        mov    edi, dword ptr [0x3afc]       ; -> loc_3_3AFC
        cmp    edi, 0x3b7f                   ; -> obj3:0x3B7F
        jae    loc_3_066B
        stosb  byte ptr es:[edi], al
loc_3_066B:
        mov    dword ptr [0x3afc], edi       ; -> loc_3_3AFC
        pop    edi
        ret    
        db  000h
loc_3_0674:
        cmp    ah, 0x90
        jb     0x68a
        je     loc_3_0683
        cmp    ah, 0x91
        ja     0x68a
        xor    ah, ah
        iretd  
loc_3_0683:
        xor    ah, ah
        clc    
        sti    
        retf   2
        db  0EAh, 000h, 000h, 000h, 000h
loc_3_068F:
        mov    dword ptr [0x39f7], ebx       ; -> obj3:0x39F7
        mov    eax, dword ptr [edx + 0x775]
        mov    dword ptr [0x222c], eax       ; -> obj1:0x222C
        mov    dword ptr [0x2230], edx       ; -> obj1:0x2230
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        xor    eax, eax
        test   byte ptr [0x222c], 1          ; -> obj1:0x222C
        jne    loc_3_06C4
        jecxz  loc_3_06C4
        test   byte ptr [ecx + 5], 0x82
        je     loc_3_06C4
        mov    eax, 0x40
loc_3_06C4:
        xor    esi, esi
        mov    edi, 0x399c                   ; -> loc_3_399C
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        test   eax, eax
        je     loc_3_06E3
        add    eax, 3
        shr    eax, 2
        xor    ecx, ecx
        VxDCall  VDMAD.VDMAD_Reserve_Buffer_Space ; cd200d000400
loc_3_06E3:
        VxDCall  INT13.Int13_Get_Version         ; cd2000002000
        test   eax, eax
        je     loc_3_0940
        xor    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        mov    dword ptr [0x21ba], eax       ; -> obj1:0x21BA
        mov    edx, 8
        lea    edi, [0x2196]                 ; -> obj1:0x2196
loc_3_0709:
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        stosd  dword ptr es:[edi], eax
        dec    edx
        jne    loc_3_0709
        mov    ecx, 8
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        stosd  dword ptr es:[edi], eax
        xor    ebx, ebx
        push   0
        push   0xff000
        push   0x1000
        VMMCall  VMM._GetPhysPageInfo            ; cd205d010100
        add    esp, 0xc
        or     eax, eax
        je     loc_3_073F
        mov    ebx, 0x40
loc_3_073F:
        push   0xf
        push   0x1fa0                        ; -> obj1:0x1FA0
        push   0xfef
        push   0
        push   0
        push   0
        push   1
        mov    eax, 2
        test   ebx, 0x40
        je     loc_3_0765
        mov    eax, 0x10
loc_3_0765:
        xor    esi, esi
        mov    edi, 0x39cf                   ; -> obj3:0x39CF
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        add    eax, 0xe
        cmp    eax, 0x10
        jbe    loc_3_077F
        mov    eax, 0x10
loc_3_077F:
        push   eax
        mov    edi, eax
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, edx
        je     loc_4_223C
        mov    eax, edx
        and    eax, 0xfff
        add    dword ptr [0x1fa0], eax       ; -> obj1:0x1FA0
        mov    dword ptr [0x1f9c], edx       ; -> obj1:0x1F9C
        shl    edi, 0xc
        sub    edi, 0x6e
        test   ebx, 0x40
        jne    loc_3_07BA
        sub    edi, 0xe000
loc_3_07BA:
        mov    dword ptr [edx + 0x60], edi
        mov    dword ptr [edx + 0x28], ebx
        test   ebx, 0x40
        je     loc_3_081D
        mov    eax, 0x10
        xor    esi, esi
        mov    edi, 0x39be                   ; -> obj3:0x39BE
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        or     eax, eax
        je     loc_4_223C
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [edi + 0x5c], eax
        mov    ecx, 0xc
        shl    dword ptr [edi + 0x5c], cl
        lea    edx, [edi + 0x58]
        push   0xa
        push   edx
        push   0xfff
        push   0
        push   0
        push   0
        push   1
        push   eax
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_4_223C
        mov    dword ptr [edi + 0x54], eax
        mov    edx, edi
loc_3_081D:
        test   byte ptr [0x222c], 8          ; -> obj1:0x222C
        je     loc_3_082A
        or     byte ptr [edx + 0x29], 1
loc_3_082A:
        mov    ecx, dword ptr [edx + 0x60]
        mov    eax, dword ptr [0x1fa0]       ; -> obj1:0x1FA0
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0x44], 0
        mov    ebx, 0x6e
        add    ebx, edx
        mov    eax, dword ptr [0x1fa0]       ; -> obj1:0x1FA0
        add    eax, 0x82
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], 0
        mov    dword ptr [ebx + 8], ecx
        add    dword ptr [ebx + 8], 0x5a
        add    dword ptr [ebx + 8], edx
        mov    word ptr [ebx + 0xc], 0
        mov    word ptr [ebx + 0xe], cx
        sub    word ptr [ebx + 0xe], 0x14
        mov    word ptr [ebx + 0x10], 1
        mov    ebx, dword ptr [ebx + 8]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], 0
        mov    dword ptr [ebx + 8], 0
        mov    word ptr [ebx + 0xc], 0
        mov    word ptr [ebx + 0xe], 0
        mov    word ptr [ebx + 0x10], 1
        xor    eax, eax
        xor    esi, esi
        push   edi
        mov    edi, 0x3984                   ; -> obj3:0x3984
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edi
        test   eax, eax
        je     loc_3_0917
        mov    al, 0x76
        VMMCall  VMM.Get_Debug_Options           ; cd20dc000100
        je     loc_3_0917
        xor    edi, edi
        call   sub_3_0116
        jb     loc_3_0917
        mov    byte ptr [0x2245], 1          ; -> loc_1_2245
        mov    eax, 0x15
        mov    esi, 0x180                    ; -> loc_1_0180
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        push   0
        push   0x1b
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        test   eax, eax
        je     loc_3_0917
        mov    edi, eax
        shl    eax, 0xc
        shr    ax, 0xc
        xchg   dword ptr [0x54], eax
        mov    dword ptr [0x2240], eax       ; -> obj1:0x2240
        mov    dword ptr [0x68b], eax        ; -> obj3:0x068B
        mov    esi, 0x674                    ; -> loc_3_0674
        mov    ecx, 0x1b
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_0917:
        mov    eax, 0x13
        mov    esi, 0x370                    ; -> loc_1_0370
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    esi, 0x650                    ; -> loc_1_0650
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [0x2234], eax       ; -> obj1:0x2234
        jb     loc_3_093A
        ret    
loc_3_093A:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_0940:
        stc    
        ret    
loc_3_0942:
        mov    dword ptr [0x3a58], ecx       ; -> loc_3_3A58
        call   sub_3_361C
        mov    eax, 0xfa
        mov    esi, 0x1bbc                   ; -> loc_1_1BBC
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        or     esi, esi
        jne    loc_3_0961
loc_3_0961:
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edx, dword ptr [0x2230]       ; -> obj1:0x2230
        lea    eax, [edx + 2]
        mov    dword ptr [edi + 0x2c], eax
        mov    al, byte ptr [edx + 0x77a]
        mov    byte ptr [0x2224], al         ; -> obj1:0x2224
        lea    eax, [edx + 0x662]
        mov    dword ptr [edi + 0x38], eax
        mov    al, byte ptr [edx + 0x779]
        mov    byte ptr [edi + 0x22], al
        mov    ecx, 0xf4240
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    esi, eax
loc_3_099D:
        loop   loc_3_099D
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, esi
        push   eax
        mov    eax, 0xf4240
        sub    edx, edx
        mov    ecx, 0x2710
        mul    ecx
        pop    ecx
        div    ecx
        sub    edx, edx
        mov    ecx, 0x99
        div    ecx
        mov    dword ptr [edi + 0x14], eax
        call   sub_3_1A39
        call   sub_3_1214
        call   sub_3_1669
        call   sub_3_1ADC
        call   sub_3_1207
        call   sub_4_2257
        call   sub_3_32F4
        call   sub_3_0A69
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [ecx + 0x28], 0x10
        jne    loc_3_0A12
        call   sub_3_16F4
        call   sub_3_1506
        call   sub_3_18DC
        call   sub_3_1895
        call   sub_3_3562
loc_3_0A12:
        call   sub_3_223F
        call   sub_3_228D
        call   sub_3_0DB6
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [ecx + 0x28], 0x10
        je     loc_3_0A31
        and    dword ptr [ecx + 0x28], 0xfffffff5
loc_3_0A31:
        test   byte ptr [ecx + 0x28], 0xa
        jne    loc_3_0A3E
        mov    al, 1
        call   sub_3_1144
loc_3_0A3E:
        call   sub_1_05C2
        mov    ebx, dword ptr [0x39f7]       ; -> obj3:0x39F7
        call   sub_3_3200
        call   sub_3_0CCC
        push   0
        push   0
        push   0x1b3d                        ; -> loc_1_1B3D
        VxDCall  SHELL._SHELL_HookSystemBroadcast ; cd2011001700
        add    esp, 0xc
        clc    
        ret    
        db  0F8h, 0C3h
sub_3_0A69:
        sub    esp, 0x10
        lea    esi, [0x3a79]                 ; -> obj3:0x3A79
        lea    edi, [esp + 0xc]
        push   edi
        push   esi
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0BA6
        mov    dword ptr [esp], 0
        lea    edx, [esp + 8]
        lea    esi, [esp]
        lea    edi, [esp + 4]
        mov    dword ptr [edi], 4
        lea    eax, [0x3ac8]                 ; -> loc_3_3AC8
        mov    ecx, dword ptr [esp + 0xc]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_0ACC
        mov    eax, dword ptr [esp]
        mov    dword ptr [0], eax            ; -> obj5:0x0000
loc_3_0ACC:
        mov    dword ptr [esp], 0
        lea    edx, [esp + 8]
        lea    esi, [esp]
        lea    edi, [esp + 4]
        mov    dword ptr [edi], 4
        lea    eax, [0x3ad9]                 ; -> obj3:0x3AD9
        mov    ecx, dword ptr [esp + 0xc]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_0B0A
        mov    eax, dword ptr [esp]
        mov    dword ptr [4], eax            ; -> obj5:0x0004
loc_3_0B0A:
        VxDCall  VPEND.Service_0000h             ; cd2000002600
        mov    al, 0
        adc    al, 0
        mov    byte ptr [0xe], al            ; -> obj5:0x000E
        or     al, al
        jne    loc_3_0B2D
        push   0
        push   0x3074                        ; -> loc_4_3074
        VxDCall  VPEND.Service_000Ah             ; cd200a002600
        add    esp, 8
loc_3_0B2D:
        call   sub_4_3086
        mov    dword ptr [esp], 0
        lea    edx, [esp + 8]
        lea    esi, [esp]
        lea    edi, [esp + 4]
        mov    dword ptr [edi], 4
        lea    eax, [0x3aa5]                 ; -> obj3:0x3AA5
        mov    ecx, dword ptr [esp + 0xc]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_0B7A
        cmp    byte ptr [esp], 0
        je     loc_3_0B7A
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x28], 4
        jmp    loc_3_0BC3
loc_3_0B7A:
        lea    edx, [esp + 8]
        lea    eax, [0x3aaf]                 ; -> obj3:0x3AAF
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [edi], 4
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        movzx  eax, byte ptr [esp]
        je     loc_3_0BB5
loc_3_0BA6:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x39ae                   ; -> loc_3_39AE
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
loc_3_0BB5:
        test   eax, eax
        je     loc_3_0BC3
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x28], 0x10
loc_3_0BC3:
        add    esp, 0x10
        ret    
loc_3_0BC7:
        call   sub_3_318F
        call   sub_3_0C25
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [eax + 0x28], 1
        call   sub_4_14EC
        call   sub_4_1BF0
        call   sub_3_3655
        or     byte ptr [0x221c], 4          ; -> obj1:0x221C
        VxDCall  PAGEFILE.PageFile_Get_Size_Info ; cd2007002100
        movzx  eax, byte ptr [esi]
        or     eax, eax
        je     loc_3_0C23
        sub    al, 0x41
        call   sub_1_128D
        jb     loc_3_0C23
        or     byte ptr [ebx + 0x21], 1
        mov    ebx, dword ptr [ebx]
        or     byte ptr [ebx + 0x21], 1
        cmp    byte ptr [ebx + 0x77], 0x10
        jae    loc_3_0C23
        push   0
        push   1
        VMMCall  VMM._GetSetPageOutCount         ; cd2070000100
        add    esp, 8
loc_3_0C23:
        clc    
        ret    
sub_3_0C25:
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        cmp    bl, 2
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        jne    loc_3_0CAD
        or     byte ptr [ebx + 0x29], 0x20
        test   byte ptr [ebx + 0x28], 0x40
        jne    loc_3_0CAC
        cli    
        mov    ecx, dword ptr [ebx + 0x60]
        add    ecx, ebx
        add    ecx, 0x6e
        mov    eax, ecx
        add    ecx, 0xdfec
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    word ptr [ecx + 0xc], 0
        mov    word ptr [ecx + 0xe], 0
        mov    word ptr [ecx + 0x10], 1
        sub    eax, 0x14
        mov    edx, dword ptr [0x1fa0]       ; -> obj1:0x1FA0
        add    edx, dword ptr [ebx + 0x60]
        add    edx, 0x6e
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0xc], 0
        mov    word ptr [eax + 0xe], 0xe000
        mov    word ptr [eax + 0x10], 1
        add    dword ptr [ebx + 0x60], 0xe000
        sti    
loc_3_0CAC:
        ret    
loc_3_0CAD:
        test   byte ptr [ebx + 0x28], 0x40
        jne    loc_3_0CAC
        mov    eax, dword ptr [ebx + 0x60]
        add    eax, 0x6e
        shr    eax, 0xc
        push   0x40
        push   eax
        push   ebx
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        ret    
        db  000h, 000h
sub_3_0CCC:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   dword ptr [ecx + 0x28], 0xa
        jne    loc_3_0CFA
        sub    edx, edx
        mov    esi, 0x669                    ; -> loc_1_0669
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        xor    eax, eax
        mov    al, 0x13
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
loc_3_0CFA:
        ret    
        db  000h
sub_3_0CFC:
        call   sub_3_33F4
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    esi, edx
        mov    edi, 0x3c3a                   ; -> obj3:0x3C3A
        call   sub_4_21E3
        dec    edi
        mov    esi, 0x3cd6                   ; -> obj3:0x3CD6
        call   sub_4_21E3
        mov    edx, 0x3c3a                   ; -> obj3:0x3C3A
        xor    cx, cx
        mov    ax, 0x3d00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_0DAF
        mov    word ptr [0x3cde], ax         ; -> obj3:0x3CDE
        mov    bx, ax
        mov    ax, 0x4202
        xor    cx, cx
        mov    dx, cx
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_0DAF
        or     dx, dx
        movzx  esi, ax
        mov    word ptr [0x3ce8], ax         ; -> obj3:0x3CE8
        push   1
        push   esi
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_3_0DAF
        mov    dword ptr [0x3ce0], eax       ; -> obj3:0x3CE0
        mov    bx, word ptr [0x3cde]         ; -> obj3:0x3CDE
        mov    ax, 0x4200
        xor    cx, cx
        mov    dx, cx
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_0DAF
        mov    edx, dword ptr [0x3ce0]       ; -> obj3:0x3CE0
        mov    bx, word ptr [0x3cde]         ; -> obj3:0x3CDE
        mov    ecx, esi
        mov    ah, 0x3f
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_0DAF
        call   sub_3_346B
        clc    
        ret    
loc_3_0DAF:
        call   sub_3_346B
        stc    
        ret    
sub_3_0DB6:
        cmp    word ptr [0x3cde], -1         ; -> obj3:0x3CDE
        je     loc_3_0DE4
        mov    bx, word ptr [0x3cde]         ; -> obj3:0x3CDE
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        push   0
        push   dword ptr [0x3ce0]            ; -> obj3:0x3CE0
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        or     eax, eax
loc_3_0DE4:
        ret    
sub_3_0DE5:
        cmp    word ptr [0x3cde], -1         ; -> obj3:0x3CDE
        jne    loc_3_0DF5
        call   sub_3_0CFC
        ret    
loc_3_0DF5:
        clc    
        ret    
sub_3_0DF7:
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [esi], 0x52444d44
        jne    loc_3_0E0B
        cmp    dword ptr [esi + 4], 0x20205256
        je     loc_3_0E1C
loc_3_0E0B:
        cmp    dword ptr [esi], 0x4f494258
        jne    loc_3_0E2C
        cmp    dword ptr [esi + 4], 0x20202053
        jne    loc_3_0E2C
loc_3_0E1C:
        push   esi
        xor    esi, esi
        call   sub_3_12A5
        pop    esi
        mov    eax, 0
        jae    loc_3_0E48
loc_3_0E2C:
        xor    eax, eax
        call   sub_3_0E4C
        jae    loc_3_0E48
        call   sub_3_10CB
        xor    edx, edx
        mov    edi, 0x3d00                   ; -> obj3:0x3D00
        call   sub_3_0E90
        mov    eax, edx
loc_3_0E48:
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_3_0E4C:
        mov    edx, esi
        mov    ecx, 7
        mov    edi, 0x3be4                   ; -> loc_3_3BE4
        mov    ebx, edi
loc_3_0E5A:
        push   ecx
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        or     ecx, ecx
        pop    ecx
        je     loc_3_0E71
        mov    esi, edx
        add    ebx, 0xc
        mov    edi, ebx
        loop   loc_3_0E5A
        stc    
loc_3_0E71:
        ret    
sub_3_0E72:
        push   edi
        push   esi
        push   ebx
        call   sub_3_0E4C
        cmc    
        jb     loc_3_0E8C
        call   sub_3_10CB
        mov    edi, 0x3cf7                   ; -> loc_3_3CF7
        call   sub_3_0E90
loc_3_0E8C:
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_3_0E90:
        sub    esp, 8
        push   edi
        push   esi
        push   ebx
        mov    dword ptr [esp + 0x10], edi
        mov    dword ptr [esp + 0xc], esi
        call   sub_3_0DE5
        jb     loc_3_0ECD
        mov    esi, dword ptr [esp + 0x10]
        call   sub_3_0F4A
        jb     loc_3_0ECD
        mov    esi, dword ptr [esp + 0xc]
loc_3_0EB4:
        call   sub_3_107A
        jae    loc_3_0ECD
        mov    edi, 0x3c3a                   ; -> obj3:0x3C3A
        call   sub_3_0F93
        jb     loc_3_0ECD
        cmp    byte ptr [edi], 0x5b
        jne    loc_3_0EB4
        stc    
loc_3_0ECD:
        rcl    ax, 1
        pop    ebx
        pop    esi
        pop    edi
        add    esp, 8
        rcr    ax, 1
        ret    
sub_3_0EDA:
        push   esi
        push   edi
        push   ebx
        sub    esp, 0xc
        mov    dword ptr [esp], esi
        mov    dword ptr [esp + 4], edi
        mov    dword ptr [esp + 8], ecx
        call   sub_3_0DE5
        jb     loc_3_0F47
        mov    esi, 0x3cea                   ; -> loc_3_3CEA
        call   sub_3_0F4A
        jb     loc_3_0F47
loc_3_0EFE:
        mov    esi, edi
loc_3_0F00:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x3d
        jne    loc_3_0F00
        mov    ecx, esi
        dec    esi
        mov    byte ptr [esi], 0
        mov    esi, dword ptr [esp]
        call   sub_4_21EF
        call   sub_3_107A
        jae    loc_3_0F2D
        mov    edi, 0x3c3a                   ; -> obj3:0x3C3A
        call   sub_3_0F93
        jb     loc_3_0F47
        cmp    byte ptr [edi], 0x5b
        je     loc_3_0F47
        jmp    loc_3_0EFE
loc_3_0F2D:
        mov    esi, ecx
        mov    edi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_3_0F37:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        je     loc_3_0F3F
        loop   loc_3_0F37
loc_3_0F3F:
        clc    
loc_3_0F40:
        add    esp, 0xc
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_3_0F47:
        stc    
        jmp    loc_3_0F40
sub_3_0F4A:
        call   sub_4_21EF
        mov    dword ptr [0x3ce4], 0         ; -> obj3:0x3CE4
loc_3_0F59:
        mov    edi, 0x3c3a                   ; -> obj3:0x3C3A
loc_3_0F5E:
        call   sub_3_0F93
        jb     loc_3_0F92
        cmp    byte ptr [edi], 0x5b
        jne    loc_3_0F5E
        inc    edi
        xchg   edi, esi
        push   esi
loc_3_0F6E:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x5d
        jne    loc_3_0F6E
        dec    esi
        mov    byte ptr [esi], 0
        pop    esi
        call   sub_3_107A
        mov    esi, edi
        jb     loc_3_0F59
        mov    edi, 0x3c3a                   ; -> obj3:0x3C3A
loc_3_0F86:
        call   sub_3_0F93
        jb     loc_3_0F92
        cmp    byte ptr [edi], 0
        je     loc_3_0F86
loc_3_0F92:
        ret    
sub_3_0F93:
        movzx  eax, word ptr [0x3ce8]        ; -> obj3:0x3CE8
        cmp    dword ptr [0x3ce4], eax       ; -> obj3:0x3CE4
        jae    loc_3_1078
        xor    edx, edx
        push   esi
        push   edi
        mov    esi, dword ptr [0x3ce0]       ; -> obj3:0x3CE0
        mov    ebx, esi
        add    esi, dword ptr [0x3ce4]       ; -> obj3:0x3CE4
        add    ebx, eax
        mov    dword ptr [0x3ce4], eax       ; -> obj3:0x3CE4
loc_3_0FBF:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_3_0FBF
        cmp    al, 9
        je     loc_3_0FBF
        cmp    al, 0x2c
        je     loc_3_100D
        cmp    al, 0xd
        je     loc_3_0FE8
        cmp    al, 0xa
        je     loc_3_0FE8
        cmp    al, 0x3b
        je     loc_3_0FDF
        cmp    esi, ebx
        jae    loc_3_0FFF
        stosb  byte ptr es:[edi], al
        jmp    loc_3_0FBF
loc_3_0FDF:
        lodsb  al, byte ptr [esi]
        cmp    al, 0xd
        je     loc_3_0FE8
        cmp    al, 0xa
        jne    loc_3_0FDF
loc_3_0FE8:
        lodsb  al, byte ptr [esi]
        cmp    al, 0xd
        je     loc_3_0FE8
        cmp    al, 0xa
        je     loc_3_0FE8
        dec    esi
        mov    eax, dword ptr [0x3ce0]       ; -> obj3:0x3CE0
        sub    esi, eax
        mov    dword ptr [0x3ce4], esi       ; -> obj3:0x3CE4
loc_3_0FFF:
        mov    byte ptr [edi], 0
        pop    edi
        mov    esi, edi
        call   sub_4_21EF
        pop    esi
        clc    
        ret    
loc_3_100D:
        mov    byte ptr [edi], 0
        push   edi
        inc    edi
        mov    edx, edi
loc_3_1014:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_3_1014
        cmp    al, 9
        je     loc_3_1014
        cmp    al, 0xd
        je     loc_3_1035
        cmp    al, 0xa
        je     loc_3_1035
        cmp    esi, ebx
        jae    loc_3_102C
        stosb  byte ptr es:[edi], al
        jmp    loc_3_1014
loc_3_102C:
        bts    word ptr [0x3c38], 0          ; -> obj3:0x3C38
loc_3_1035:
        push   esi
        mov    byte ptr [edi], 0
        mov    esi, edx
        call   sub_4_21EF
        mov    ebx, 0x3b80                   ; -> loc_3_3B80
        xor    edx, edx
        mov    eax, esi
loc_3_1049:
        cmp    edx, 5
        ja     loc_3_1061
        mov    edi, dword ptr [ebx + edx*4]
        movzx  ecx, byte ptr [edi]
        inc    edi
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jecxz  loc_3_1066
        inc    edx
        mov    esi, eax
        jmp    loc_3_1049
loc_3_1061:
        mov    edx, 0xffffffff
loc_3_1066:
        pop    esi
        pop    edi
        btr    word ptr [0x3c38], 0          ; -> obj3:0x3C38
        jb     loc_3_0FFF
        jmp    loc_3_0FE8
loc_3_1078:
        stc    
        ret    
sub_3_107A:
        push   esi
        push   edi
        mov    eax, dword ptr [edi]
        cmp    ax, 0x2e2a
        jne    loc_3_108B
        shr    eax, 0x10
        cmp    al, 0x2a
        je     loc_3_10B2
loc_3_108B:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_3_10B2
        cmp    al, 0x2e
        je     loc_3_10A7
        mov    bl, byte ptr [edi]
        cmp    bl, 0x2a
        je     loc_3_10A7
        cmp    bl, 0x3f
        je     loc_3_10A4
        cmp    al, bl
        jne    loc_3_10C7
loc_3_10A4:
        inc    edi
        jmp    loc_3_108B
loc_3_10A7:
        xchg   edi, esi
loc_3_10A9:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x2e
        je     loc_3_10B6
        or     al, al
        jne    loc_3_10A9
loc_3_10B2:
        pop    edi
        pop    esi
        clc    
        ret    
loc_3_10B6:
        xchg   edi, esi
        cmp    byte ptr [esi - 1], 0x2e
        je     loc_3_108B
loc_3_10BE:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x2e
        je     loc_3_108B
        or     al, al
        jne    loc_3_10BE
loc_3_10C7:
        pop    edi
        pop    esi
        stc    
        ret    
sub_3_10CB:
        mov    edi, 0x3cbe                   ; -> obj3:0x3CBE
        mov    edx, edi
        xor    ebx, ebx
        mov    ecx, 8
loc_3_10D9:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_3_10DF
        stosb  byte ptr es:[edi], al
loc_3_10DF:
        loop   loc_3_10D9
        bts    ebx, 0
        jb     loc_3_10F1
        mov    al, 0x2e
        stosb  byte ptr es:[edi], al
        mov    ecx, 3
        jmp    loc_3_10D9
loc_3_10F1:
        mov    byte ptr [edi], 0
        mov    esi, edx
        call   sub_4_21EF
        ret    
sub_3_10FC:
        push   esi
        push   edi
        mov    edi, 0x3ccb                   ; -> loc_3_3CCB
        mov    edx, edi
        mov    al, 0x20
        mov    ecx, 0xb
        rep stosb byte ptr es:[edi], al
        mov    edi, edx
        mov    ecx, 8
loc_3_1115:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_3_111D
        stosb  byte ptr es:[edi], al
        loop   loc_3_1115
loc_3_111D:
        mov    edi, edx
        mov    word ptr [edi + 8], 0x5845
        mov    byte ptr [edi + 0xa], 0x45
        mov    esi, edx
        call   sub_3_0DF7
        jae    loc_3_1141
        mov    word ptr [edi + 8], 0x4f43
        mov    byte ptr [edi + 0xa], 0x4d
        call   sub_3_0DF7
loc_3_1141:
        pop    edi
        pop    esi
        ret    
sub_3_1144:
        sub    esp, 0x14
        pushal 
        lea    edi, [esp + 0x20]
        mov    byte ptr [edi], al
        lea    esi, [esp + 0x30]
        push   esi
        push   0x3a79                        ; -> obj3:0x3A79
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_11BB
        mov    ebx, dword ptr [esi]
        lea    ecx, [esp + 0x28]
        lea    esi, [esp + 0x24]
        lea    edx, [esp + 0x2c]
        mov    byte ptr [esi], al
        inc    eax
        mov    dword ptr [ecx], eax
        push   ecx
        push   esi
        push   edx
        push   0
        push   0x3ab9                        ; -> obj3:0x3AB9
        push   ebx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_119B
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi], al
        je     loc_3_11B1
loc_3_119B:
        push   1
        push   edi
        push   3
        push   0
        push   0x3ab9                        ; -> obj3:0x3AB9
        push   ebx
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
loc_3_11B1:
        push   ebx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_3_11BB:
        popal  
        mov    ah, byte ptr [esp + 4]
        add    esp, 0x14
        ret    
sub_3_11C4:
        pushal 
        cmp    byte ptr [0x544], 0           ; -> obj5:0x0544
        jne    loc_3_1205
        mov    byte ptr [0x544], cl          ; -> obj5:0x0544
        mov    ecx, dword ptr [esi]
        mov    dword ptr [0x545], ecx        ; -> obj5:0x0545
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [0x549], ecx        ; -> obj5:0x0549
        xor    eax, eax
        call   sub_3_1144
        test   ah, ah
        je     loc_3_1205
        or     eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x3ae9                   ; -> obj3:0x3AE9
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x343], al          ; -> obj5:0x0343
loc_3_1205:
        popal  
        ret    
sub_3_1207:
        mov    eax, 0x4c
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x21c3], eax       ; -> obj1:0x21C3
        ret    
sub_3_1214:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    esi, dword ptr [eax + 0x4c]
loc_3_121C:
        or     esi, esi
        je     loc_3_12A4
        mov    edi, dword ptr [esi]
        call   sub_3_2F6A
        cmp    byte ptr [edi], 7
        je     loc_3_123C
        mov    edi, dword ptr [esi + 4]
        call   sub_3_2F6A
        mov    esi, edi
        jmp    loc_3_121C
loc_3_123C:
        mov    dword ptr [0x3a0b], edi       ; -> obj3:0x3A0B
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
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        mov    dword ptr [0x2225], edi       ; -> loc_1_2225
        mov    edi, dword ptr [edi + 0x1328]
        call   sub_3_2F6A
        mov    dword ptr [0x3a68], edi       ; -> obj3:0x3A68
loc_3_12A4:
        ret    
sub_3_12A5:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        call   sub_3_33F4
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, 6
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        cmp    ecx, 6
        jne    loc_4_2206
        mov    word ptr [ebp + 0x1c], 0xf900
        mov    word ptr [ebp + 0x14], 0x80
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_3_137C
        test   word ptr [ebp + 0x14], 0x8000
        je     loc_3_137C
        push   esi
        mov    esi, edi
        call   sub_3_2F6A
        mov    word ptr [ebp + 0x10], si
        shr    esi, 0x10
        mov    word ptr [ebp + 0x3c], si
        mov    word ptr [ebp + 0x38], si
        pop    esi
        mov    word ptr [ebp + 0x1c], 0xe000
        mov    byte ptr [edi], 0xe
        mov    byte ptr [edi + 1], 0
        mov    word ptr [ebp + 0x18], 0
        mov    byte ptr [ebp + 0x14], 0x80
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_3_137C
        cmp    word ptr [ebp + 0x18], 0x1234
        clc    
        jne    loc_3_137C
        mov    eax, edi
        or     esi, esi
        je     loc_3_136B
        mov    edi, esi
        and    edi, 0xffff0000
        call   sub_3_2F6A
        mov    cx, word ptr [eax + 4]
        cmp    word ptr [edi + 2], cx
        clc    
        jne    loc_3_137C
loc_3_136B:
        mov    edi, dword ptr [eax + 2]
        call   sub_3_2F6A
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [eax + 0x68], edi
        stc    
loc_3_137C:
        setb   dl
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, 6
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        call   sub_3_346B
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        rcr    dl, 1
        cmc    
        popal  
        ret    
sub_3_13B0:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0xff00
        mov    word ptr [ebp + 0x14], 0x80
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_3_144D
        cmp    word ptr [ebp + 0x1c], 0x55aa
        clc    
        jne    loc_3_144D
        mov    di, word ptr [ebp + 0x38]
        rol    esi, 0x10
        cmp    di, si
        clc    
        jne    loc_3_144D
        rol    edi, 0x10
        mov    di, word ptr [ebp + 0x10]
        call   sub_3_2F6A
        cmp    dword ptr [edi], 0x4d524541
        clc    
        jne    loc_3_144D
        cmp    dword ptr [edi + 4], 0x56333148
        clc    
        jne    loc_3_144D
        mov    ax, word ptr [edi + 8]
        xchg   ah, al
        cmp    ax, 0x3336
        ja     loc_3_1440
        cmp    byte ptr [0x2224], 3          ; -> obj1:0x2224
        jb     loc_3_1440
        pushfd 
        push   0x3d26                        ; -> loc_3_3D26
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
        clc    
        jmp    loc_3_144D
loc_3_1440:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     dword ptr [eax + 0x28], 0x400000
        stc    
loc_3_144D:
        setb   dl
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        rcr    dl, 1
        cmc    
        popal  
        ret    
sub_3_146A:
        push   edi
        push   esi
        lea    esi, [edi]
        mov    edi, 0x3a6c                   ; -> loc_3_3A6C
        mov    ecx, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        push   esi
        jecxz  loc_3_1481
        stc    
        jmp    loc_3_1500
loc_3_1481:
        mov    edi, dword ptr [esi + 1]
        mov    dword ptr [0x3a75], edi       ; -> obj3:0x3A75
loc_3_148A:
        mov    esi, edi
        call   sub_3_2F6A
        cmp    byte ptr [edi], 0xe9
        je     loc_3_149B
        cmp    byte ptr [edi], 0xeb
        jne    loc_3_14BE
loc_3_149B:
        cmp    dword ptr [edi + 3], 0x544e4924
        jne    loc_3_14BE
        cmp    dword ptr [edi + 7], 0x46533331
        jne    loc_3_14BE
        mov    edi, dword ptr [edi + 0x13]
        mov    ecx, edi
        rol    ecx, 0x10
        cmp    cx, 0xa000
        jae    loc_3_14FD
        jmp    loc_3_148A
loc_3_14BE:
        call   sub_3_12A5
        jae    loc_3_14FD
        call   sub_3_13B0
        jae    loc_3_14FD
        push   edi
        movzx  eax, si
        rol    esi, 0x10
        movzx  esi, si
        shl    esi, 4
        add    esi, eax
        push   0x12
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        shl    eax, 0xa
        cmp    eax, esi
        pop    edi
        jae    loc_3_14FD
        cmp    byte ptr [edi], 0x9a
        je     loc_3_14F5
        cmp    byte ptr [edi], 0xea
        jne    loc_3_1503
loc_3_14F5:
        cmp    word ptr [edi + 3], 0xa000
        jb     loc_3_1500
loc_3_14FD:
        clc    
        xor    eax, eax
loc_3_1500:
        pop    esi
        pop    edi
        ret    
loc_3_1503:
        stc    
        jmp    loc_3_1500
sub_3_1506:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   dword ptr [eax + 0x28], 2
        jne    loc_3_1668
        mov    edi, dword ptr [0x3a0b]       ; -> obj3:0x3A0B
        movzx  ecx, byte ptr [edi + 5]
        lea    edi, [edi + 6]
loc_3_1525:
        push   ecx
        movzx  ecx, byte ptr [edi + 0xe]
        lea    esi, [edi + 0xf]
loc_3_152D:
        cmp    byte ptr [esi], 2
        je     loc_3_15A7
        cmp    byte ptr [esi], 3
        je     loc_3_15A7
loc_3_1537:
        test   byte ptr [0x221b], 0x80       ; -> loc_1_221B
        jne    loc_3_15A2
        add    esi, 5
        loop   loc_3_152D
        mov    edi, esi
        pop    ecx
        loop   loc_3_1525
        or     byte ptr [0x221b], 0x80       ; -> loc_1_221B
        push   ecx
        mov    edi, dword ptr [0x3a68]       ; -> obj3:0x3A68
        cmp    word ptr [edi + 0xfe], -1
        jne    loc_3_1577
        pushfd 
        push   0x3d6c                        ; -> loc_3_3D6C
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
        jmp    loc_3_165E
loc_3_1577:
        lea    edi, [edi + 0xe]
loc_3_157A:
        movzx  esi, word ptr [edi]
        or     esi, esi
        je     loc_3_1667
        test   byte ptr [edi + 7], 0x80
        je     loc_3_15A2
        dec    esi
        shl    esi, 4
        lea    esi, [esi + 8]
        call   sub_3_10FC
        jae    loc_3_15F3
        or     byte ptr [edi + 7], 0x40
        stc    
        push   ecx
        push   esi
        jmp    loc_3_15C2
loc_3_15A2:
        add    edi, 0xc
        jmp    loc_3_157A
loc_3_15A7:
        push   ecx
        push   esi
        lea    esi, [edi]
        call   sub_3_0DF7
        jae    loc_3_15EF
        or     word ptr [esi + 0xb], 1
        pop    esi
        push   esi
        call   sub_3_146A
        jae    loc_3_15EF
        lea    esi, [edi]
loc_3_15C2:
        pushfd 
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd 
        push   esi
        push   0x3da1                        ; -> obj3:0x3DA1
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        xchg   byte ptr [esi + 8], cl
        mov    cl, 1
        test   byte ptr [0x221b], 0x80       ; -> loc_1_221B
        je     loc_3_15E9
        inc    cl
loc_3_15E9:
        call   sub_3_11C4
        popfd  
loc_3_15EF:
        pop    esi
        pop    ecx
        jb     loc_3_165E
loc_3_15F3:
        cmp    eax, 3
        je     loc_3_1537
        cmp    eax, 5
        jne    loc_3_1603
        xor    eax, eax
loc_3_1603:
        bts    dword ptr [0x3a3f], eax       ; -> obj3:0x3A3F
        cmp    dword ptr [0x3a3f], 3         ; -> obj3:0x3A3F
        jbe    loc_3_1537
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     dword ptr [eax + 0x28], 8
        cmp    dword ptr [0x3a3f], 5         ; -> obj3:0x3A3F
        ja     loc_3_164E
        pushfd 
        push   0x3dbc                        ; -> loc_3_3DBC
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
        pushfd 
        push   0x3df1                        ; -> obj3:0x3DF1
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
        jmp    loc_3_1537
loc_3_164E:
        pushfd 
        push   0x3e32                        ; -> obj3:0x3E32
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
loc_3_165E:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     dword ptr [eax + 0x28], 2
loc_3_1667:
        pop    ecx
loc_3_1668:
        ret    
sub_3_1669:
        pushal 
        mov    edi, dword ptr [0x3a0b]       ; -> obj3:0x3A0B
        movzx  ecx, byte ptr [edi + 5]
        lea    edi, [edi + 6]
loc_3_1677:
        push   ecx
        movzx  ecx, byte ptr [edi + 0xe]
        lea    esi, [edi + 0xf]
loc_3_167F:
        cmp    byte ptr [esi], 2
        je     loc_3_1695
        cmp    byte ptr [esi], 3
        je     loc_3_1695
loc_3_1689:
        add    esi, 5
        loop   loc_3_167F
        mov    edi, esi
        pop    ecx
        loop   loc_3_1677
        popal  
        ret    
loc_3_1695:
        push   ecx
        push   esi
        lea    esi, [edi]
        cmp    dword ptr [esi], 0x44494350
        jne    loc_3_16AA
        cmp    dword ptr [esi + 4], 0x20205652
        je     loc_3_16BB
loc_3_16AA:
        cmp    dword ptr [esi], 0x44534f44
        jne    loc_3_16F0
        cmp    dword ptr [esi + 4], 0x20205652
        jne    loc_3_16F0
loc_3_16BB:
        cmp    dword ptr [0x2203], 0         ; -> obj1:0x2203
        jne    loc_3_16F0
        or     byte ptr [0x2244], 4          ; -> obj1:0x2244
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x28], 2
        or     word ptr [esi + 0xb], 1
        call   sub_1_062C
        pushfd 
        push   esi
        push   0x3e6c                        ; -> obj3:0x3E6C
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
loc_3_16F0:
        pop    esi
        pop    ecx
        jmp    loc_3_1689
sub_3_16F4:
        sub    esp, 8
        call   sub_3_19C3
        mov    edi, dword ptr [0x3a0b]       ; -> obj3:0x3A0B
        movzx  ebx, byte ptr [edi + 5]
        add    edi, 6
        mov    byte ptr [0x13e], 0           ; -> obj5:0x013E
loc_3_1710:
        or     ebx, ebx
        je     loc_3_1824
        cmp    byte ptr [edi + 0xd], 0
        je     loc_3_1828
        mov    esi, edi
        mov    dword ptr [esp + 4], 0
        cmp    dword ptr [esi], 0x49505341
        jne    loc_3_1747
        cmp    dword ptr [esi + 4], 0x4b534944
        jne    loc_3_1747
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x2a], 0x10
loc_3_1747:
        call   sub_3_0DF7
        jb     loc_3_1777
        mov    dword ptr [esp + 4], 1
        cmp    eax, 3
        jne    loc_3_1777
loc_3_175B:
        mov    al, byte ptr [edi + 0xd]
        add    byte ptr [0x13e], al          ; -> obj5:0x013E
        movzx  eax, byte ptr [edi + 0xe]
        mov    ecx, 5
        mul    ecx
        add    eax, 0xf
        add    edi, eax
        dec    ebx
        jmp    loc_3_1710
loc_3_1777:
        mov    dword ptr [esp], eax
        mov    al, byte ptr [0x13e]          ; -> obj5:0x013E
        mov    ah, byte ptr [edi + 0xd]
loc_3_1782:
        call   sub_4_1BC0
        jb     loc_3_17FD
        cmp    dword ptr [esp + 4], 0
        jne    loc_3_180C
        or     word ptr [edi + 0xb], 1
        push   ecx
        xor    cl, cl
        xchg   byte ptr [edi + 8], cl
        pushfd 
        push   eax
        push   edi
        push   0x3eac                        ; -> loc_3_3EAC
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 0xc
        popfd  
        xchg   byte ptr [edi + 8], cl
        pop    ecx
        or     word ptr [esi + 0x10], 4
        call   sub_3_3537
        cmp    byte ptr [esi], 1
        jbe    loc_3_17FD
        cmp    byte ptr [esi + 0xa], 0xff
        jne    loc_3_17FD
        cmp    byte ptr [esi + 0xd], 0xff
        jne    loc_3_17FD
        cmp    byte ptr [esi + 1], 0xff
        jne    loc_3_17FD
        push   ecx
        xor    cl, cl
        xchg   byte ptr [edi + 8], cl
        pushfd 
        push   eax
        push   edi
        push   0x3ed3                        ; -> loc_3_3ED3
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 0xc
        popfd  
        xchg   byte ptr [edi + 8], cl
        xchg   edi, esi
        mov    cl, 1
        call   sub_3_11C4
        xchg   edi, esi
        pop    ecx
loc_3_17F9:
        or     dword ptr [ecx + 0x28], 2
loc_3_17FD:
        inc    al
        dec    ah
        jne    loc_3_1782
        jmp    loc_3_175B
loc_3_180C:
        cmp    dword ptr [esp], 4
        jne    loc_3_1816
        or     byte ptr [esi + 0x10], 0x10
loc_3_1816:
        movzx  edx, word ptr [esi + 0x10]
        and    edx, 0x12
        cmp    edx, 0x12
        je     loc_3_17F9
        jmp    loc_3_17FD
loc_3_1824:
        add    esp, 8
        ret    
loc_3_1828:
        mov    cl, byte ptr [edi + 0xe]
        or     cl, cl
        je     loc_3_185B
        push   edi
        add    edi, 0xf
loc_3_1833:
        mov    al, byte ptr [edi]
        cmp    al, 6
        je     loc_3_185A
        cmp    al, 5
        je     loc_3_185A
        cmp    al, 7
        je     loc_3_185A
        cmp    al, 2
        je     loc_3_185A
        cmp    al, 3
        je     loc_3_185A
        dec    cl
        je     loc_3_1852
        add    edi, 5
        jmp    loc_3_1833
loc_3_1852:
        pop    edi
        call   sub_3_3537
        jmp    loc_3_185B
loc_3_185A:
        pop    edi
loc_3_185B:
        lea    esi, [edi]
        call   sub_3_0E72
        jb     loc_3_175B
        or     word ptr [edi + 0xb], 1
        xor    cl, cl
        xchg   byte ptr [edi + 8], cl
        pushfd 
        push   edi
        push   0x3efe                        ; -> loc_3_3EFE
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        xchg   byte ptr [edi + 8], cl
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x29], 8
        jmp    loc_3_175B
sub_3_1895:
        mov    edx, dword ptr [0x21c7]       ; -> obj1:0x21C7
        or     edx, edx
        je     loc_3_18DB
loc_3_189F:
        bsf    eax, edx
        je     loc_3_18DB
        btr    edx, eax
        call   sub_4_1BC0
        jb     loc_3_189F
        mov    edi, esi
        movzx  eax, byte ptr [eax + 0x21cf]  ; -> obj1:0x21CF
        call   sub_4_1BC0
        jb     loc_3_189F
        mov    bx, word ptr [esi + 0x10]
        mov    cx, word ptr [edi + 0x10]
        and    bx, 0xfff5
        and    cx, 0xfff5
        or     cx, bx
        or     word ptr [esi + 0x10], cx
        or     word ptr [edi + 0x10], cx
        jmp    loc_3_189F
loc_3_18DB:
        ret    
sub_3_18DC:
        xor    eax, eax
loc_3_18DE:
        mov    al, 7
        call   sub_3_2F84
        jb     loc_3_1916
        mov    edi, dword ptr [0x3a1f]       ; -> obj3:0x3A1F
        mov    edi, dword ptr [edi + 1]
        mov    esi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        call   sub_3_0DF7
        jae    loc_3_1912
        or     word ptr [esi + 0xb], 1
        call   sub_3_1982
        mov    edi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        call   sub_3_3537
loc_3_1912:
        mov    ah, 1
        jmp    loc_3_18DE
loc_3_1916:
        mov    edi, dword ptr [0x21c7]       ; -> obj1:0x21C7
        or     edi, edi
        je     loc_3_1931
        mov    esi, 0x3a5c                   ; -> loc_3_3A5C
        call   sub_3_0DF7
        jae    loc_3_1931
        call   sub_3_1982
loc_3_1931:
        mov    ebx, dword ptr [0x3a68]       ; -> obj3:0x3A68
        cmp    word ptr [ebx + 0xfe], -1
        je     loc_3_1981
        lea    ebx, [ebx + 0xe]
loc_3_1944:
        movzx  esi, word ptr [ebx]
        or     esi, esi
        je     loc_3_1981
        mov    edi, dword ptr [ebx + 4]
        and    edi, 0x1fffffff
        or     edi, edi
        je     loc_3_197C
        dec    esi
        shl    esi, 4
        lea    esi, [esi + 8]
        call   sub_3_10FC
        jae    loc_3_197C
        or     byte ptr [ebx + 7], 0x20
        mov    cx, word ptr [ebx + 2]
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     word ptr [eax + 0x6c], cx
        call   sub_3_1982
loc_3_197C:
        add    ebx, 0xc
        jmp    loc_3_1944
loc_3_1981:
        ret    
sub_3_1982:
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd 
        push   esi
        push   0x3f36                        ; -> obj3:0x3F36
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        xchg   byte ptr [esi + 8], cl
loc_3_199B:
        bsf    eax, edi
        je     loc_3_19C2
        btr    edi, eax
        pushfd 
        push   eax
        push   0x3f6a                        ; -> obj3:0x3F6A
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        call   sub_4_1BC0
        jb     loc_3_199B
        or     word ptr [esi + 0x10], 8
        jmp    loc_3_199B
loc_3_19C2:
        ret    
sub_3_19C3:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    esi, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [esi + 0x1d], 0x52
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ebx, word ptr [esi + 0x38]
        shl    ebx, 4
        movzx  ecx, word ptr [esi + 0x10]
        add    ebx, ecx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        movzx  eax, word ptr [ebx + 0x16]
        movzx  edx, word ptr [ebx + 0x18]
        shl    edx, 4
        add    eax, edx
        mov    edi, 0xbe                     ; -> obj5:0x00BE
        mov    edx, 0x58
        movzx  ecx, byte ptr [ebx + 0x21]
        mov    byte ptr [0x342], cl          ; -> obj5:0x0342
loc_3_1A33:
        stosd  dword ptr es:[edi], eax
        add    eax, edx
        loop   loc_3_1A33
        ret    
sub_3_1A39:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x1690
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        xor    ebx, ebx
        test   word ptr [ebp + 0x2c], 1
        jne    loc_3_1A81
        mov    di, word ptr [ebp + 0x38]
        rol    edi, 0x10
        mov    di, word ptr [ebp + 0x10]
        call   sub_3_2F6A
        mov    ebx, edi
loc_3_1A81:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [edi + 0x4c], ebx
        mov    word ptr [edi + 0x24], 0
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        test   ebx, 0x40000
        je     loc_3_1ABA
        or     word ptr [edi + 0x24], 4
        jmp    loc_3_1ADB
loc_3_1ABA:
        or     word ptr [edi + 0x24], 2
        test   ebx, 0x80000
        jne    loc_3_1ACC
        xor    word ptr [edi + 0x24], 3
loc_3_1ACC:
        or     word ptr [edi + 0x24], 9
        cmp    bl, 0xfc
        jne    loc_3_1ADB
        and    word ptr [edi + 0x24], 0xfff7
loc_3_1ADB:
        ret    
sub_3_1ADC:
        mov    eax, 0x2a
        mov    esi, 0x2ff0                   ; -> loc_3_2FF0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        call   sub_3_19C3
        call   sub_3_33F4
        mov    ax, 0x3d00
        lea    edx, [0x39e9]                 ; -> obj3:0x39E9
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_1B2F
        push   eax
        mov    bx, ax
        mov    ax, 0x4402
        lea    edx, [0x39f2]                 ; -> loc_3_39F2
        mov    cx, 4
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ah, 0x3e
        pop    ebx
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_3_1B2F:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, 0x924
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        cmp    ecx, 0x924
        jne    loc_4_2206
        mov    dword ptr [0x39ff], edi       ; -> obj3:0x39FF
        mov    esi, edi
        and    esi, 0xffff0000
        shr    esi, 0xc
        movzx  eax, di
        add    esi, eax
        mov    dword ptr [0x39fb], esi       ; -> obj3:0x39FB
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 0x10
        mov    word ptr [ebp + 0x18], 1
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ah, 0x19
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        push   eax
        mov    dl, 2
loc_3_1BA8:
        mov    ah, 0xe
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        push   eax
        mov    ah, 0xd
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
        inc    dl
        cmp    dl, al
        jb     loc_3_1BA8
        pop    edx
        mov    ah, 0xe
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    byte ptr [ebp + 0x1d], 0x52
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  edx, word ptr [ebp + 0x38]
        shl    edx, 4
        inc    byte ptr [edx + 0x321]
        push   edx
        mov    eax, 0x13
        mov    esi, 0x301e                   ; -> loc_3_301E
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x4f
        mov    esi, 0x312e                   ; -> loc_3_312E
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        push   0
        push   0x1d5
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        push   eax
        mov    dword ptr [0x3a33], eax       ; -> obj3:0x3A33
        push   edi
        mov    edi, eax
        mov    al, 0xff
        mov    ecx, 0x1d5
        rep stosb byte ptr es:[edi], al
        pop    edi
        xor    ebx, ebx
loc_3_1C39:
        cmp    bl, byte ptr [0x342]          ; -> obj5:0x0342
        jae    loc_3_1CA5
        mov    eax, dword ptr [0x3a33]       ; -> obj3:0x3A33
        mov    ecx, dword ptr [ebx*4 + 0xbe] ; -> obj5:0x00BE
        mov    dh, byte ptr [ecx + 0x44]
        test   dh, 0x40
        je     loc_3_1CA1
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        bts    dword ptr [ecx + 0x44], ebx
        test   dh, 0x80
        jne    loc_3_1CA1
        test   dh, 0x10
        je     loc_3_1C8A
        mov    ecx, dword ptr [0x2225]       ; -> loc_1_2225
        mov    ecx, dword ptr [ecx + 0x26]
loc_3_1C72:
        xchg   edi, ecx
        call   sub_3_2F6A
        xchg   edi, ecx
        cmp    byte ptr [ecx], bl
        je     loc_3_1C8A
        mov    ecx, dword ptr [ecx + 0x19]
        cmp    cx, -1
        je     loc_3_1CA1
        jmp    loc_3_1C72
loc_3_1C8A:
        mov    byte ptr [eax], bl
        cmp    bl, 1
        ja     loc_3_1C94
        mov    byte ptr [eax + 1], bl
loc_3_1C94:
        mov    word ptr [eax + 0x10], 0
        add    dword ptr [0x3a33], 0x12      ; -> obj3:0x3A33
loc_3_1CA1:
        inc    bl
        jmp    loc_3_1C39
loc_3_1CA5:
        mov    eax, dword ptr [0x3a33]       ; -> obj3:0x3A33
        inc    eax
        pop    ebx
        sub    eax, ebx
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        or     eax, eax
        je     loc_4_2206
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [ebx + 0x34], eax
        mov    dword ptr [0x3a33], eax       ; -> obj3:0x3A33
loc_3_1CD1:
        mov    esi, dword ptr [0x3a33]       ; -> obj3:0x3A33
        mov    bl, byte ptr [esi]
        cmp    bl, 0xff
        je     loc_3_1F1F
        cmp    bl, 1
        jbe    loc_3_1F01
        mov    byte ptr [0x13e], bl          ; -> obj5:0x013E
        cmp    dword ptr [0x39f2], 0         ; -> loc_3_39F2
        je     loc_3_1D22
        cmp    byte ptr [0x39f6], 0          ; -> loc_3_39F6
        jne    loc_3_1D22
        mov    eax, dword ptr [0x39f2]       ; -> loc_3_39F2
        xor    edx, edx
        mov    esi, 0x30c6                   ; -> loc_3_30C6
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    byte ptr [0x39f6], 1          ; -> loc_3_39F6
        mov    esi, dword ptr [0x3a33]       ; -> obj3:0x3A33
loc_3_1D22:
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        mov    word ptr [ebp + 8], ax
        mov    word ptr [ebp + 0x1c], 0x4a10
        mov    word ptr [ebp + 0x10], 3
        mov    word ptr [ebp + 0x14], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        and    byte ptr [0x221c], 0xfe       ; -> obj1:0x221C
        cmp    byte ptr [ebp + 0x14], 0xff
        je     loc_3_1D8C
        test   byte ptr [ebp + 0x14], 0x80
        jne    loc_3_1D8C
        or     byte ptr [0x221c], 1          ; -> obj1:0x221C
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        mov    word ptr [ebp + 8], ax
        mov    word ptr [ebp + 0x1c], 0x4a10
        mov    word ptr [ebp + 0x10], 3
        mov    word ptr [ebp + 0x14], 2
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_3_1D8C:
        and    byte ptr [0x221b], 0xfb       ; -> loc_1_221B
        mov    dword ptr [0x3a43], 0         ; -> obj3:0x3A43
        call   sub_3_212F
        btr    dword ptr [0x221b], 0xd       ; -> loc_1_221B
        jae    loc_3_1DB9
        call   sub_3_1F97
        mov    esi, dword ptr [0x3a33]       ; -> obj3:0x3A33
        jmp    loc_3_1D8C
loc_3_1DB9:
        push   dword ptr [ebp + 0x2c]
        test   byte ptr [0x221c], 1          ; -> obj1:0x221C
        je     loc_3_1DED
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        mov    word ptr [ebp + 8], ax
        mov    word ptr [ebp + 0x1c], 0x4a10
        mov    word ptr [ebp + 0x10], 3
        mov    word ptr [ebp + 0x14], 1
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_3_1DED:
        pop    dword ptr [ebp + 0x2c]
        test   word ptr [ebp + 0x2c], 1
        je     loc_3_1E98
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        mov    edx, dword ptr [ebp + 0x1c]
        pushfd 
        push   edx
        push   eax
        push   0x3f7e                        ; -> loc_3_3F7E
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 0xc
        popfd  
        mov    word ptr [esi + 0x10], 2
        jmp    loc_3_1F01
loc_3_1E23:
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        inc    eax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, edi
        add    ax, 0x800
        mov    word ptr [ebp + 0x14], ax
        mov    byte ptr [ebp + 0x19], 8
        mov    byte ptr [ebp + 0x18], 0x60
        rol    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 0x1c], 0x440d
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_3_1F01
        mov    eax, dword ptr [0x39fb]       ; -> obj3:0x39FB
        add    eax, 0x800
        movzx  edx, word ptr [eax + 0xf]
        or     edx, edx
        jne    loc_3_1E7A
        mov    edx, dword ptr [eax + 0x1c]
loc_3_1E7A:
        shr    edx, 1
        mov    dword ptr [0x3a43], edx       ; -> obj3:0x3A43
loc_3_1E82:
        call   sub_3_212F
        btr    dword ptr [0x221b], 0xd       ; -> loc_1_221B
        jae    loc_3_1F01
        call   sub_3_1F97
        jmp    loc_3_1E82
loc_3_1E98:
        mov    word ptr [esi + 0x10], 0
        mov    ebx, dword ptr [0x39fb]       ; -> obj3:0x39FB
        cmp    dword ptr [ebx + 0x1f0], 0x14cd13cd
        jne    loc_3_1ED0
        cmp    dword ptr [ebx + 0x1f4], 0x3cd01cd
        jne    loc_3_1ED0
        mov    al, byte ptr [ebx + 0x4c]
        sub    al, 0x41
        push   ebx
        movzx  ebx, byte ptr [0x13e]         ; -> obj5:0x013E
        mov    byte ptr [ebx + 0x21e9], al   ; -> obj1:0x21E9
        pop    ebx
loc_3_1ED0:
        cmp    word ptr [ebx + 0x16], 0
        jne    loc_3_1EE4
        cmp    byte ptr [ebx + 0x42], 0x29
        jne    loc_3_1F0D
        mov    eax, dword ptr [ebx + 0x43]
        or     eax, eax
        jne    loc_3_1EF1
loc_3_1EE4:
        cmp    byte ptr [ebx + 0x26], 0x29
        jne    loc_3_1F0D
        mov    eax, dword ptr [ebx + 0x27]
        or     eax, eax
        je     loc_3_1F0D
loc_3_1EF1:
        mov    dword ptr [esi + 2], eax
        test   byte ptr [0x221b], 4          ; -> loc_1_221B
        je     loc_3_1E23
loc_3_1F01:
        add    dword ptr [0x3a33], 0x12      ; -> obj3:0x3A33
        jmp    loc_3_1CD1
loc_3_1F0D:
        xor    eax, eax
        mov    ecx, 0x200
loc_3_1F14:
        add    eax, dword ptr [ecx + ebx - 4]
        sub    ecx, 4
        jne    loc_3_1F14
        jmp    loc_3_1EF1
loc_3_1F1F:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, 0x924
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        mov    eax, 0x2a
        mov    esi, 0x2ff0                   ; -> loc_3_2FF0
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        mov    eax, 0x4f
        mov    esi, 0x312e                   ; -> loc_3_312E
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        cmp    byte ptr [0x39f6], 0          ; -> loc_3_39F6
        je     loc_3_1F65
        mov    eax, dword ptr [0x39f2]       ; -> loc_3_39F2
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_3_1F65:
        mov    eax, 0x13
        mov    esi, 0x301e                   ; -> loc_3_301E
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        call   sub_3_346B
        pop    ebx
        dec    byte ptr [ebx + 0x321]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_3_1F97:
        sub    esp, 4
        mov    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    esi, dword ptr [esi + 0x2c]
        or     esi, esi
        je     loc_3_212B
loc_3_1FAB:
        cmp    byte ptr [esi], 2
        jne    loc_3_211D
        mov    dl, byte ptr [esi + 5]
        mov    byte ptr [esp], dl
        mov    word ptr [ebp], 0
        mov    word ptr [ebp + 0x38], 0
        mov    byte ptr [ebp + 0x1d], 8
        mov    byte ptr [ebp + 0x14], dl
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        mov    di, word ptr [ebp + 0x38]
        rol    edi, 0x10
        mov    di, word ptr [ebp]
        mov    eax, dword ptr [ebp + 0x2c]
        rcr    eax, 1
        jb     loc_3_212B
        inc    byte ptr [0x2224]             ; -> obj1:0x2224
        mov    byte ptr [esi], 0
        mov    al, cl
        xchg   ch, cl
        and    ch, 0xc0
        shr    ch, 6
        inc    ecx
        mov    word ptr [esi + 6], cx
        and    eax, 0x3f
        mov    word ptr [esi + 0xa], ax
        movzx  eax, dh
        inc    eax
        mov    word ptr [esi + 8], ax
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    eax, dword ptr [eax + 0x38]
loc_3_2026:
        cmp    byte ptr [eax], 0xff
        je     loc_3_2030
        add    eax, 0xb
        jmp    loc_3_2026
loc_3_2030:
        mov    byte ptr [eax + 0xb], 0xff
        mov    dword ptr [0x3a2f], eax       ; -> obj3:0x3A2F
        mov    edi, dword ptr [0x39ff]       ; -> obj3:0x39FF
        mov    word ptr [ebp + 0x1c], 0x201
        rol    edi, 0x10
        mov    word ptr [ebp + 0x38], di
        rol    edi, 0x10
        mov    word ptr [ebp + 0x10], di
        mov    word ptr [ebp + 0x18], 1
        movzx  edx, byte ptr [esp]
        mov    word ptr [ebp + 0x14], dx
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_3_212B
        mov    eax, dword ptr [0x3a2f]       ; -> obj3:0x3A2F
        mov    word ptr [eax + 9], 0
        mov    cl, byte ptr [esp]
        mov    byte ptr [eax], cl
        mov    esi, dword ptr [0x39fb]       ; -> obj3:0x39FB
        cmp    word ptr [esi + 0xda], 0
        jne    loc_3_20FE
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_3_20EE
        mov    ah, 2
        push   0x1a
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        rol    ecx, 0x10
        mov    ch, dh
        mov    cl, byte ptr [esp]
        mov    dword ptr [esi + 0xdc], ecx
        mov    word ptr [ebp + 0x1c], 0x301
        rol    edi, 0x10
        mov    word ptr [ebp + 0x38], di
        rol    edi, 0x10
        mov    word ptr [ebp + 0x10], di
        mov    word ptr [ebp + 0x18], 1
        movzx  edx, byte ptr [esp]
        mov    word ptr [ebp + 0x14], dx
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_3_20FE
loc_3_20EE:
        mov    eax, dword ptr [0x3a2f]       ; -> obj3:0x3A2F
        mov    ecx, dword ptr [esi + 0xdc]
        mov    dword ptr [eax + 1], ecx
        jmp    loc_3_212B
loc_3_20FE:
        xor    edx, edx
        mov    ecx, 0x200
loc_3_2105:
        add    edx, dword ptr [ecx + esi - 4]
        sub    ecx, 4
        jne    loc_3_2105
        mov    eax, dword ptr [0x3a2f]       ; -> obj3:0x3A2F
        or     word ptr [eax + 9], 1
        mov    dword ptr [eax + 5], edx
        jmp    loc_3_20EE
loc_3_211D:
        cmp    dword ptr [esi + 1], 0
        je     loc_3_212B
        add    esi, dword ptr [esi + 1]
        jmp    loc_3_1FAB
loc_3_212B:
        add    esp, 4
        ret    
sub_3_212F:
        or     byte ptr [0x221c], 8          ; -> obj1:0x221C
        mov    byte ptr [0x3a57], 0          ; -> obj3:0x3A57
loc_3_213D:
        mov    ecx, dword ptr [0x39fb]       ; -> obj3:0x39FB
        add    ecx, 0x800
        mov    dword ptr [ecx + 6], edi
        mov    eax, dword ptr [0x3a43]       ; -> obj3:0x3A43
        mov    dword ptr [ecx], eax
        mov    word ptr [ecx + 4], 1
        mov    byte ptr [ebp + 0x1d], 0x30
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        xchg   ah, al
        cmp    ax, 0x70a
        jae    loc_3_21A9
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        mov    word ptr [ebp + 0x1c], ax
        mov    eax, edi
        add    ax, 0x800
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x18], 0xffff
        rol    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    eax, 0x25
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        jmp    loc_3_21E3
loc_3_21A9:
        movzx  eax, byte ptr [0x13e]         ; -> obj5:0x013E
        inc    al
        mov    byte ptr [ebp + 0x14], al
        mov    eax, edi
        add    ax, 0x800
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x18], 0xffff
        rol    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 4], 0
        mov    word ptr [ebp + 0x1c], 0x7305
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_3_21E3:
        test   byte ptr [ebp + 0x2c], 1
        je     loc_3_2237
        inc    byte ptr [0x3a57]             ; -> obj3:0x3A57
        cmp    byte ptr [0x3a57], 1          ; -> obj3:0x3A57
        jne    loc_3_2237
        pushal 
        mov    edx, 0x1fd                    ; -> obj5:0x01FD
        mov    esi, edx
        add    esi, 0x1e
        mov    ah, 0x1a
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_2231
        call   sub_3_32CF
        mov    edx, 0x179                    ; -> obj5:0x0179
        mov    cl, byte ptr [0x13e]          ; -> obj5:0x013E
        add    cl, 0x41
        mov    byte ptr [edx], cl
        xor    cx, cx
        mov    ah, 0x4e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_3_2231:
        popal  
        jmp    loc_3_213D
loc_3_2237:
        and    byte ptr [0x221c], 0xf7       ; -> obj1:0x221C
        ret    
sub_3_223F:
        VxDCall  VCDFSD.Service_0000h            ; cd2000004100
        jb     loc_3_228C
        VxDCall  VCDFSD.Service_0001h            ; cd2001004100
        cmp    word ptr [eax], 4
        jne    loc_3_228C
        test   byte ptr [eax + 4], 1
        je     loc_3_228C
        cmp    word ptr [eax + 8], 0
        je     loc_3_226A
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [edx + 0x29], 0x10
loc_3_226A:
        lea    eax, [eax + 0x14]
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        xor    ecx, ecx
loc_3_2275:
        cmp    dword ptr [eax + 2], 0
        je     loc_3_2283
        bts    dword ptr [edx + 0x48], ecx
        bts    dword ptr [edx + 0x44], ecx
loc_3_2283:
        add    eax, 6
        inc    ecx
        cmp    cl, 0x1a
        jb     loc_3_2275
loc_3_228C:
        ret    
sub_3_228D:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        call   sub_3_33F4
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, 0x94
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        cmp    ecx, 0x94
        jne    loc_4_2206
        mov    esi, edi
        call   sub_3_2F6A
        mov    dword ptr [0x39fb], edi       ; -> obj3:0x39FB
        mov    dword ptr [0x39ff], esi       ; -> obj3:0x39FF
        add    edi, 0x24
        mov    dword ptr [0x3a03], edi       ; -> obj3:0x3A03
        mov    eax, 0x13c
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x3a07], eax       ; -> obj3:0x3A07
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    esi, dword ptr [eax + 0x4c]
loc_3_22F5:
        or     esi, esi
        je     loc_3_2C34
        mov    edi, dword ptr [esi]
        call   sub_3_2F6A
        cmp    byte ptr [edi], 7
        je     loc_3_2315
        mov    edi, dword ptr [esi + 4]
        call   sub_3_2F6A
        mov    esi, edi
        jmp    loc_3_22F5
loc_3_2315:
        mov    dword ptr [0x3a0b], edi       ; -> obj3:0x3A0B
        push   1
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    dword ptr [0x3a0f], eax       ; -> obj3:0x3A0F
        mov    eax, 0x4f
        mov    esi, 0x2e5b                   ; -> loc_3_2E5B
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        or     byte ptr [0x221b], 1          ; -> loc_1_221B
        mov    byte ptr [0x3a19], 0          ; -> loc_3_3A19
        xor    ah, ah
loc_3_2355:
        mov    al, 6
        call   sub_3_2F84
        jb     loc_3_2707
        mov    esi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        call   sub_3_0DF7
        jae    loc_3_2396
        pushfd 
        or     word ptr [esi + 0xb], 1
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd 
        push   esi
        push   0x3fa9                        ; -> loc_3_3FA9
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        xchg   byte ptr [esi + 8], cl
        mov    cl, 1
        call   sub_3_11C4
        popfd  
loc_3_2396:
        jb     loc_3_2C34
        and    byte ptr [0x221d], 0xfd       ; -> loc_1_221D
        cmp    dword ptr [esi], 0x49505341
        jne    loc_3_23BB
        cmp    dword ptr [esi + 4], 0x534f4432
        jne    loc_3_23BB
        or     byte ptr [0x221d], 2          ; -> loc_1_221D
loc_3_23BB:
        mov    byte ptr [0x3d21], 7          ; -> obj3:0x3D21
        cmp    eax, 5
        jne    loc_3_23CE
        mov    byte ptr [0x3d21], 0          ; -> obj3:0x3D21
loc_3_23CE:
        mov    edi, dword ptr [0x3a1f]       ; -> obj3:0x3A1F
        push   1
        push   0x1e
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    ecx, dword ptr [0x3a13]       ; -> obj3:0x3A13
        mov    dword ptr [0x3a13], eax       ; -> obj3:0x3A13
        mov    dword ptr [eax + 1], ecx
        mov    byte ptr [eax], 1
        mov    ebx, dword ptr [edi + 1]
        mov    dword ptr [eax + 9], ebx
        mov    ecx, dword ptr [0x3a37]       ; -> obj3:0x3A37
        mov    dword ptr [eax + 0xd], ecx
        mov    dword ptr [0x3a37], ebx       ; -> obj3:0x3A37
        cmp    dword ptr [esi], 0x49505341
        jne    loc_3_244C
        cmp    dword ptr [esi + 4], 0x204d4143
        jne    loc_3_244C
        bts    dword ptr [0x221b], 0xe       ; -> loc_1_221B
        jb     loc_3_244C
        mov    eax, dword ptr [0x12c]
        mov    dword ptr [0x3d22], eax       ; -> loc_3_3D22
        mov    esi, 0x2eb7                   ; -> loc_3_2EB7
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jae    loc_3_2447
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_2447:
        mov    dword ptr [0x12c], eax
loc_3_244C:
        push   edi
        mov    esi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        mov    ecx, 2
        lea    edi, [0x312]                  ; -> obj5:0x0312
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, 8
        sub    edi, ecx
        mov    al, 0x20
        repne scasb al, byte ptr es:[edi]
        jne    loc_3_2470
        mov    byte ptr [edi], 0
loc_3_2470:
        lea    esi, [0x312]                  ; -> obj5:0x0312
        mov    eax, dword ptr [0x3a13]       ; -> obj3:0x3A13
        lea    edi, [eax + 0x15]
        mov    ecx, 9
        call   sub_3_0EDA
        pop    edi
        cmp    dword ptr [0x39f2], ebx       ; -> loc_3_39F2
        jne    loc_3_2498
        or     byte ptr [0x221b], 2          ; -> loc_1_221B
loc_3_2498:
        mov    byte ptr [0x3a1b], 0          ; -> loc_3_3A1B
loc_3_249F:
        mov    byte ptr [0x3a17], 0          ; -> loc_3_3A17
        mov    byte ptr [0x3a18], 0          ; -> obj3:0x3A18
        call   sub_3_2D6D
        cmp    byte ptr [ebx + 1], 1
        jne    loc_3_2355
        mov    al, byte ptr [ebx + 9]
        mov    byte ptr [0x3a1a], al         ; -> obj3:0x3A1A
        mov    al, byte ptr [ebx + 8]
        dec    al
        mov    byte ptr [0x3a1d], al         ; -> obj3:0x3A1D
loc_3_24CE:
        mov    edx, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    al, byte ptr [0x3a1b]         ; -> loc_3_3A1B
        mov    byte ptr [edx + 0xe], al
        mov    byte ptr [edx + 4], 0
        mov    ebx, dword ptr [0x3a03]       ; -> obj3:0x3A03
        mov    byte ptr [ebx], 1
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        mov    byte ptr [ebx + 2], al
        mov    byte ptr [edx], al
        mov    byte ptr [ebx + 3], 0
        mov    al, byte ptr [0x3a17]         ; -> loc_3_3A17
        cmp    byte ptr [0x3a1a], al         ; -> obj3:0x3A1A
        je     loc_3_26B0
        mov    byte ptr [ebx + 8], al
        mov    byte ptr [edx + 1], al
        mov    al, byte ptr [0x3a18]         ; -> obj3:0x3A18
        mov    byte ptr [ebx + 9], al
        mov    byte ptr [edx + 2], al
        and    byte ptr [0x221b], 0xfb       ; -> loc_1_221B
        mov    ax, word ptr [0x3a01]         ; -> obj3:0x3A01
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [0x39ff]         ; -> obj3:0x39FF
        add    ax, 0x24
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  ecx, word ptr [edi + 3]
        movzx  edx, word ptr [edi + 1]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        cmp    byte ptr [ebx + 1], 0x81
        mov    ah, 1
        je     loc_3_2355
        cmp    byte ptr [ebx + 1], 1
        je     loc_3_2579
        jmp    loc_3_26B0
loc_3_2579:
        test   byte ptr [0x221b], 4          ; -> loc_1_221B
        jne    loc_3_266A
        mov    byte ptr [ebx], 2
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        mov    byte ptr [ebx + 2], al
        mov    al, byte ptr [0x3a17]         ; -> loc_3_3A17
        mov    byte ptr [ebx + 8], al
        mov    al, byte ptr [0x3a18]         ; -> obj3:0x3A18
        mov    byte ptr [ebx + 9], al
        mov    dword ptr [ebx + 0xa], 0x24
        mov    byte ptr [ebx + 0xe], 0
        mov    byte ptr [ebx + 3], 0
        mov    byte ptr [ebx + 0x17], 6
        mov    byte ptr [ebx + 0x40], 0x12
        mov    al, byte ptr [0x3a18]         ; -> obj3:0x3A18
        shl    al, 5
        mov    byte ptr [ebx + 0x41], al
        mov    word ptr [ebx + 0x42], 0
        mov    byte ptr [ebx + 0x44], 0x24
        mov    byte ptr [ebx + 0x45], 0
        mov    eax, dword ptr [0x39ff]       ; -> obj3:0x39FF
        mov    dword ptr [ebx + 0xf], eax
        mov    ax, word ptr [0x3a01]         ; -> obj3:0x3A01
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [0x39ff]         ; -> obj3:0x39FF
        add    ax, 0x24
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  ecx, word ptr [edi + 3]
        movzx  edx, word ptr [edi + 1]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
loc_3_260F:
        cmp    byte ptr [ebx + 1], 0
        jne    loc_3_261D
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        jmp    loc_3_260F
loc_3_261D:
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        test   byte ptr [0x221b], 4          ; -> loc_1_221B
        jne    loc_3_266A
        cmp    byte ptr [ebx + 1], 1
        jne    loc_3_2699
        and    byte ptr [0x221d], 0xfd       ; -> loc_1_221D
        mov    ebx, dword ptr [0x39fb]       ; -> obj3:0x39FB
        test   byte ptr [ebx], 0xe0
        jne    loc_3_26B0
        mov    edx, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    al, byte ptr [ebx]
        mov    byte ptr [edx + 0xf], al
        xor    eax, eax
        mov    al, byte ptr [ebx]
        mov    ecx, 0x23
loc_3_265E:
        add    al, byte ptr [ecx + ebx]
        adc    ah, 0
        loop   loc_3_265E
        mov    word ptr [edx + 0xc], ax
loc_3_266A:
        mov    edx, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    ebx, dword ptr [0x3a13]       ; -> obj3:0x3A13
        mov    eax, dword ptr [ebx + 5]
        mov    dword ptr [edx + 0x10], eax
        mov    dword ptr [ebx + 5], edx
        push   1
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    dword ptr [0x3a0f], eax       ; -> obj3:0x3A0F
loc_3_2699:
        inc    byte ptr [0x3a18]             ; -> obj3:0x3A18
        mov    al, byte ptr [0x3d21]         ; -> obj3:0x3D21
        cmp    byte ptr [0x3a18], al         ; -> obj3:0x3A18
        jbe    loc_3_24CE
loc_3_26B0:
        mov    byte ptr [0x3a18], 0          ; -> obj3:0x3A18
        inc    byte ptr [0x3a17]             ; -> loc_3_3A17
        cmp    byte ptr [0x3a17], 7          ; -> loc_3_3A17
        jbe    loc_3_24CE
        test   byte ptr [0x221d], 2          ; -> loc_1_221D
        je     loc_3_26DC
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [eax + 0x2a], 0x80
loc_3_26DC:
        mov    byte ptr [0x3a17], 0          ; -> loc_3_3A17
        inc    byte ptr [0x3a19]             ; -> loc_3_3A19
        inc    byte ptr [0x3a1b]             ; -> loc_3_3A1B
        mov    ah, 1
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        cmp    al, byte ptr [0x3a1d]         ; -> obj3:0x3A1D
        ja     loc_3_2355
        jmp    loc_3_249F
loc_3_2707:
        mov    eax, 0x4f
        mov    esi, 0x2e5b                   ; -> loc_3_2E5B
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        and    byte ptr [0x221b], 0xfe       ; -> loc_1_221B
        btr    dword ptr [0x221b], 0xe       ; -> loc_1_221B
        jae    loc_3_2732
        mov    eax, dword ptr [0x3d22]       ; -> loc_3_3D22
        mov    dword ptr [0x12c], eax
loc_3_2732:
        call   sub_3_2D04
        or     byte ptr [0x221b], 8          ; -> loc_1_221B
        xor    ah, ah
        mov    byte ptr [0x3a19], 0          ; -> loc_3_3A19
        mov    dword ptr [0x3a23], 0         ; -> obj3:0x3A23
loc_3_2751:
        mov    al, 5
        call   sub_3_2F84
        jb     loc_3_2764
        mov    dword ptr [0x3a23], edi       ; -> obj3:0x3A23
        mov    ah, 1
        jmp    loc_3_2751
loc_3_2764:
        mov    byte ptr [0x3a19], 0xff       ; -> loc_3_3A19
        call   sub_3_2DC1
        mov    al, byte ptr [ebx + 0x2c]
        mov    byte ptr [0x3a1d], al         ; -> obj3:0x3A1D
loc_3_2778:
        mov    ebx, dword ptr [0x3a23]       ; -> obj3:0x3A23
        or     ebx, ebx
        je     loc_3_2BB8
        test   byte ptr [0x221b], 0x20       ; -> loc_1_221B
        je     loc_3_2798
        mov    eax, dword ptr [ebx + 1]
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_3_2798:
        mov    dword ptr [0x3a23], 0         ; -> obj3:0x3A23
        xor    ah, ah
loc_3_27A4:
        mov    al, 5
        call   sub_3_2F84
        cmp    ebx, edi
        je     loc_3_27B9
        mov    dword ptr [0x3a23], edi       ; -> obj3:0x3A23
        mov    ah, 1
        jmp    loc_3_27A4
loc_3_27B9:
        mov    edi, dword ptr [0x3a23]       ; -> obj3:0x3A23
        or     edi, edi
        je     loc_3_27DA
        mov    eax, dword ptr [edi + 1]
        xor    edx, edx
        mov    esi, 0x2f29                   ; -> loc_3_2F29
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        or     byte ptr [0x221b], 0x20       ; -> loc_1_221B
loc_3_27DA:
        mov    edi, dword ptr [0x3a1f]       ; -> obj3:0x3A1F
        mov    ebx, dword ptr [edi + 1]
        mov    ecx, 0x13c
        mov    dword ptr [ecx], ebx
        cmp    dword ptr [0x3a07], ebx       ; -> obj3:0x3A07
        jne    loc_3_27F9
        or     byte ptr [0x221b], 0x10       ; -> loc_1_221B
loc_3_27F9:
        mov    word ptr [ebp + 0x1c], 0x8200
        mov    word ptr [ebp + 0x18], 0x8765
        mov    word ptr [ebp + 0x14], 0xcba9
        mov    eax, 0x4f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ah, 1
        cmp    word ptr [ebp + 0x18], 0x9abc
        jne    loc_3_2778
        cmp    word ptr [ebp + 0x14], 0x5678
        jne    loc_3_2778
        mov    esi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        call   sub_3_0DF7
        jae    loc_3_2864
        pushfd 
        or     word ptr [esi + 0xb], 1
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd 
        push   esi
        push   0x3fdf                        ; -> loc_3_3FDF
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        xchg   byte ptr [esi + 8], cl
        mov    cl, 1
        call   sub_3_11C4
        popfd  
loc_3_2864:
        jb     loc_3_2C34
        and    byte ptr [0x221d], 0xfe       ; -> loc_1_221D
        cmp    dword ptr [esi], 0x494e494d
        jne    loc_3_289B
        cmp    dword ptr [esi + 4], 0x41303034
        je     loc_3_2894
        cmp    dword ptr [esi + 4], 0x49303034
        je     loc_3_2894
        cmp    dword ptr [esi + 4], 0x41363034
        jne    loc_3_289B
loc_3_2894:
        or     byte ptr [0x221d], 1          ; -> loc_1_221D
loc_3_289B:
        mov    byte ptr [0x3d21], 7          ; -> obj3:0x3D21
        cmp    eax, 5
        jne    loc_3_28AE
        mov    byte ptr [0x3d21], 0          ; -> obj3:0x3D21
loc_3_28AE:
        push   1
        push   0x1e
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    ecx, dword ptr [0x3a13]       ; -> obj3:0x3A13
        mov    dword ptr [0x3a13], eax       ; -> obj3:0x3A13
        mov    dword ptr [eax + 1], ecx
        mov    byte ptr [eax], 3
        mov    edi, dword ptr [0x3a1f]       ; -> obj3:0x3A1F
        mov    ebx, dword ptr [edi + 1]
        mov    dword ptr [eax + 9], ebx
        push   edi
        mov    esi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        mov    ecx, 2
        lea    edi, [0x312]                  ; -> obj5:0x0312
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, 8
        sub    edi, ecx
        mov    al, 0x20
        repne scasb al, byte ptr es:[edi]
        jne    loc_3_2904
        mov    byte ptr [edi], 0
loc_3_2904:
        lea    esi, [0x312]                  ; -> obj5:0x0312
        mov    eax, dword ptr [0x3a13]       ; -> obj3:0x3A13
        lea    edi, [eax + 0x15]
        mov    ecx, 9
        call   sub_3_0EDA
        pop    edi
        mov    al, byte ptr [0x3a1d]         ; -> obj3:0x3A1D
        inc    al
        mov    byte ptr [0x3a19], al         ; -> loc_3_3A19
loc_3_2929:
        sub    byte ptr [0x3a19], 1          ; -> loc_3_3A19
        jb     loc_3_2A47
        movzx  eax, byte ptr [0x3a19]        ; -> loc_3_3A19
        bt     dword ptr [0x3a2b], eax       ; -> obj3:0x3A2B
        jb     loc_3_2929
        mov    byte ptr [0x3a17], 0          ; -> loc_3_3A17
        mov    byte ptr [0x3a18], 0          ; -> obj3:0x3A18
        and    byte ptr [0x221b], 0xbf       ; -> loc_1_221B
        call   sub_3_2DC1
        cmp    byte ptr [ebx + 7], 7
        je     loc_3_2BB8
        test   byte ptr [0x221b], 0x40       ; -> loc_1_221B
        jne    loc_3_2929
        movzx  eax, byte ptr [0x3a19]        ; -> loc_3_3A19
        bts    dword ptr [0x3a2b], eax       ; -> obj3:0x3A2B
        mov    byte ptr [0x3a1c], al         ; -> obj3:0x3A1C
        mov    al, byte ptr [ebx + 0x2d]
        mov    byte ptr [0x3a1a], al         ; -> obj3:0x3A1A
loc_3_298E:
        test   byte ptr [0x221d], 1          ; -> loc_1_221D
        jne    loc_3_2929
        mov    al, byte ptr [0x3a17]         ; -> loc_3_3A17
        cmp    byte ptr [0x3a1a], al         ; -> obj3:0x3A1A
        je     loc_3_2B92
        mov    edx, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    al, byte ptr [0x3a1b]         ; -> loc_3_3A1B
        mov    byte ptr [edx + 0xe], al
        mov    byte ptr [edx + 4], 0
        mov    ebx, dword ptr [0x3a03]       ; -> obj3:0x3A03
        mov    byte ptr [ebx + 6], 2
        mov    dword ptr [ebx + 0x10], 0
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        mov    byte ptr [ebx + 9], al
        mov    byte ptr [edx], al
        mov    dword ptr [ebx + 0xc], 0
        mov    al, byte ptr [0x3a17]         ; -> loc_3_3A17
        mov    byte ptr [ebx + 0xa], al
        mov    byte ptr [edx + 1], al
        mov    al, byte ptr [0x3a18]         ; -> obj3:0x3A18
        mov    byte ptr [ebx + 0xb], al
        mov    byte ptr [edx + 2], al
        mov    word ptr [ebx + 4], 0x15
        and    dword ptr [0x221b], 0xffffffbb ; -> loc_1_221B
        mov    ax, word ptr [0x3a01]         ; -> obj3:0x3A01
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebx + 2], ax
        mov    ax, word ptr [0x39ff]         ; -> obj3:0x39FF
        add    ax, 0x24
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebx], ax
        mov    word ptr [ebp + 0x1c], 0x8100
        mov    eax, 0x4f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [0x221b], 0x40       ; -> loc_1_221B
        jne    loc_3_2929
        cmp    byte ptr [ebx + 7], 1
        je     loc_3_2A69
        jmp    loc_3_2B92
loc_3_2A47:
        mov    eax, dword ptr [0x3a13]       ; -> obj3:0x3A13
        mov    eax, dword ptr [eax + 5]
        mov    dl, byte ptr [0x3a1c]         ; -> obj3:0x3A1C
loc_3_2A55:
        or     eax, eax
        je     loc_3_2778
        mov    dh, byte ptr [eax]
        sub    dh, dl
        mov    byte ptr [eax + 0xe], dh
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_3_2A55
loc_3_2A69:
        and    byte ptr [0x221b], 0xfb       ; -> loc_1_221B
        mov    byte ptr [ebx + 6], 1
        mov    word ptr [ebx + 4], 0x58
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        mov    byte ptr [ebx + 9], al
        mov    al, byte ptr [0x3a17]         ; -> loc_3_3A17
        mov    byte ptr [ebx + 0xa], al
        mov    al, byte ptr [0x3a18]         ; -> obj3:0x3A18
        mov    byte ptr [ebx + 0xb], al
        mov    dword ptr [ebx + 0x24], 0x24
        mov    byte ptr [ebx + 0x2c], 0
        mov    dword ptr [ebx + 0xc], 0x8468
        mov    byte ptr [ebx + 0x2d], 6
        mov    byte ptr [ebx + 0x3c], 0x12
        mov    al, byte ptr [0x3a18]         ; -> obj3:0x3A18
        shl    al, 5
        mov    byte ptr [ebx + 0x3d], al
        mov    word ptr [ebx + 0x3e], 0
        mov    byte ptr [ebx + 0x40], 0x24
        mov    byte ptr [ebx + 0x41], 0
        mov    dword ptr [ebx + 0x48], 2
        mov    eax, dword ptr [0x39ff]       ; -> obj3:0x39FF
        mov    dword ptr [ebx + 0x20], eax
        mov    ax, word ptr [0x3a01]         ; -> obj3:0x3A01
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebx + 2], ax
        mov    ax, word ptr [0x39ff]         ; -> obj3:0x39FF
        add    ax, 0x24
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebx], ax
        mov    word ptr [ebp + 0x1c], 0x8100
        mov    eax, 0x4f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_3_2B0A:
        cmp    byte ptr [ebx + 7], 0
        jne    loc_3_2B18
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        jmp    loc_3_2B0A
loc_3_2B18:
        test   byte ptr [0x221b], 4          ; -> loc_1_221B
        jne    loc_3_2B52
        cmp    byte ptr [ebx + 7], 1
        jne    loc_3_2B7B
        mov    ebx, dword ptr [0x39fb]       ; -> obj3:0x39FB
        test   byte ptr [ebx], 0xe0
        jne    loc_3_2B92
        mov    edx, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    al, byte ptr [ebx]
        mov    byte ptr [edx + 0xf], al
        xor    eax, eax
        mov    al, byte ptr [ebx]
        mov    ecx, 0x23
loc_3_2B46:
        add    al, byte ptr [ecx + ebx]
        adc    ah, 0
        loop   loc_3_2B46
        mov    word ptr [edx + 0xc], ax
loc_3_2B52:
        mov    ebx, dword ptr [0x3a13]       ; -> obj3:0x3A13
        mov    eax, dword ptr [ebx + 5]
        mov    dword ptr [edx + 0x10], eax
        mov    dword ptr [ebx + 5], edx
        push   1
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    dword ptr [0x3a0f], eax       ; -> obj3:0x3A0F
loc_3_2B7B:
        inc    byte ptr [0x3a18]             ; -> obj3:0x3A18
        mov    al, byte ptr [0x3d21]         ; -> obj3:0x3D21
        cmp    byte ptr [0x3a18], al         ; -> obj3:0x3A18
        jbe    loc_3_298E
loc_3_2B92:
        mov    byte ptr [0x3a18], 0          ; -> obj3:0x3A18
        inc    byte ptr [0x3a17]             ; -> loc_3_3A17
        cmp    byte ptr [0x3a17], 7          ; -> loc_3_3A17
        jbe    loc_3_298E
        mov    byte ptr [0x3a17], 0          ; -> loc_3_3A17
        jmp    loc_3_2929
loc_3_2BB8:
        mov    esi, dword ptr [0x3a13]       ; -> obj3:0x3A13
loc_3_2BBE:
        or     esi, esi
        je     loc_3_2C3F
        mov    edx, dword ptr [esi + 5]
loc_3_2BC5:
        or     edx, edx
        je     loc_3_2C1A
        cmp    byte ptr [edx + 4], 0
        jne    loc_3_2C1F
        mov    ecx, edx
        mov    ebx, esi
loc_3_2BD3:
        mov    ecx, dword ptr [ecx + 0x10]
loc_3_2BD6:
        or     ecx, ecx
        je     loc_3_2C0E
        cmp    byte ptr [ecx + 4], 0
        jne    loc_3_2BD3
        mov    al, byte ptr [edx + 1]
        cmp    byte ptr [ecx + 1], al
        jne    loc_3_2BD3
        mov    al, byte ptr [edx + 2]
        cmp    byte ptr [ecx + 2], al
        jne    loc_3_2BD3
        mov    ax, word ptr [edx + 0xc]
        cmp    word ptr [ecx + 0xc], ax
        jne    loc_3_2BD3
        mov    al, byte ptr [edx + 0xf]
        cmp    byte ptr [ecx + 0xf], al
        jne    loc_3_2BD3
        cmp    byte ptr [ebx + 0x15], 0
        je     loc_3_2C24
        cmp    byte ptr [esi + 0x15], 0
        je     loc_3_2C24
loc_3_2C0E:
        mov    ebx, dword ptr [ebx + 1]
        or     ebx, ebx
        je     loc_3_2C1A
        mov    ecx, dword ptr [ebx + 5]
        jmp    loc_3_2BD6
loc_3_2C1A:
        mov    esi, dword ptr [esi + 1]
        jmp    loc_3_2BBE
loc_3_2C1F:
        mov    edx, dword ptr [edx + 0x10]
        jmp    loc_3_2BC5
loc_3_2C24:
        pushfd 
        push   0x4014                        ; -> loc_3_4014
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
loc_3_2C34:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [eax + 0x28], 2
        jmp    loc_3_2C82
loc_3_2C3F:
        test   byte ptr [0x221b], 0x10       ; -> loc_1_221B
        jne    loc_3_2C5A
        pushfd 
        pushfd 
        push   0x404d                        ; -> loc_3_404D
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
        popfd  
loc_3_2C5A:
        je     loc_3_2C34
        cmp    dword ptr [0x39f2], 0         ; -> loc_3_39F2
        je     loc_3_2C82
        test   byte ptr [0x221b], 2          ; -> loc_1_221B
        jne    loc_3_2C80
        pushfd 
        pushfd 
        push   0x407c                        ; -> loc_3_407C
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 4
        popfd  
        popfd  
loc_3_2C80:
        je     loc_3_2C34
loc_3_2C82:
        test   dword ptr [0x221b], 1         ; -> loc_1_221B
        je     loc_3_2C9E
        mov    eax, 0x4f
        mov    esi, 0x2e5b                   ; -> loc_3_2E5B
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
loc_3_2C9E:
        test   byte ptr [0x221b], 8          ; -> loc_1_221B
        je     loc_3_2CAC
        call   sub_3_2D3C
loc_3_2CAC:
        mov    eax, dword ptr [0x3a07]       ; -> obj3:0x3A07
        mov    ecx, 0x13c
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [0x3a13]       ; -> obj3:0x3A13
        mov    dword ptr [eax + 0x50], ecx
        push   0
        push   dword ptr [0x3a0f]            ; -> obj3:0x3A0F
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, 0x94
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        call   sub_3_346B
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_3_2D04:
        push   dword ptr [0x3a27]            ; -> obj3:0x3A27
        push   dword ptr [0x3a1f]            ; -> obj3:0x3A1F
        xor    ah, ah
loc_3_2D12:
        mov    al, 6
        call   sub_3_2F84
        jb     loc_3_2D2F
        mov    eax, dword ptr [edi + 1]
        xor    edx, edx
        mov    esi, 0x2e0a                   ; -> loc_3_2E0A
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    ah, 1
        jmp    loc_3_2D12
loc_3_2D2F:
        pop    dword ptr [0x3a1f]            ; -> obj3:0x3A1F
        pop    dword ptr [0x3a27]            ; -> obj3:0x3A27
        ret    
sub_3_2D3C:
        push   dword ptr [0x3a27]            ; -> obj3:0x3A27
        push   dword ptr [0x3a1f]            ; -> obj3:0x3A1F
        xor    ah, ah
loc_3_2D4A:
        mov    al, 6
        call   sub_3_2F84
        jb     loc_3_2D60
        mov    eax, dword ptr [edi + 1]
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
        mov    ah, 1
        jmp    loc_3_2D4A
loc_3_2D60:
        pop    dword ptr [0x3a1f]            ; -> obj3:0x3A1F
        pop    dword ptr [0x3a27]            ; -> obj3:0x3A27
        ret    
sub_3_2D6D:
        mov    ebx, dword ptr [0x3a03]       ; -> obj3:0x3A03
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        mov    byte ptr [ebx + 2], al
        mov    byte ptr [ebx], 0
        mov    ax, word ptr [0x3a01]         ; -> obj3:0x3A01
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [0x39ff]         ; -> obj3:0x39FF
        add    ax, 0x24
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  ecx, word ptr [edi + 3]
        movzx  edx, word ptr [edi + 1]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        ret    
sub_3_2DC1:
        mov    ebx, dword ptr [0x3a03]       ; -> obj3:0x3A03
        mov    al, byte ptr [0x3a19]         ; -> loc_3_3A19
        mov    byte ptr [ebx + 9], al
        mov    byte ptr [ebx + 6], 3
        mov    word ptr [ebx + 4], 0x54
        mov    ax, word ptr [0x3a01]         ; -> obj3:0x3A01
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebx + 2], ax
        mov    ax, word ptr [0x39ff]         ; -> obj3:0x39FF
        add    ax, 0x24
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebx], ax
        mov    word ptr [ebp + 0x1c], 0x8100
        mov    eax, 0x4f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        ret    
loc_3_2E0A:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    eax, dword ptr [eax + 4]
        mov    ebx, eax
        and    eax, 0xffff0000
        and    ebx, 0xffff
        shr    eax, 0xc
        add    eax, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [eax + 8]
        mov    byte ptr [edi + 9], cl
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 0xa], cl
        mov    byte ptr [edi + 4], 1
        or     byte ptr [0x221b], 4          ; -> loc_1_221B
        call   sub_3_2D3C
        ret    
loc_3_2E5B:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        cmp    word ptr [ebx + 0x1c], 0x8100
        jne    loc_3_2EB5
        movzx  eax, word ptr [ebx + 0x38]
        shl    eax, 0x10
        add    ax, word ptr [ebx + 0x10]
        mov    ebx, eax
        and    eax, 0xffff0000
        and    ebx, 0xffff
        shr    eax, 0xc
        add    eax, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [eax + 0xa]
        mov    byte ptr [edi + 9], cl
        mov    cl, byte ptr [eax + 0xb]
        mov    byte ptr [edi + 0xa], cl
        mov    byte ptr [edi + 4], 3
        or     byte ptr [0x221b], 4          ; -> loc_1_221B
loc_3_2EB5:
        stc    
        ret    
loc_3_2EB7:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        cmp    word ptr [ebx + 0x1c], 0xc3d4
        jne    loc_3_2F17
        movzx  eax, word ptr [ebx + 0x38]
        shl    eax, 0x10
        add    ax, word ptr [ebx + 0x10]
        mov    ebx, eax
        and    eax, 0xffff0000
        and    ebx, 0xffff
        shr    eax, 0xc
        add    eax, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        cmp    byte ptr [eax + 2], 2
        jne    loc_3_2F17
        mov    edi, dword ptr [0x3a0f]       ; -> obj3:0x3A0F
        mov    cl, byte ptr [eax + 5]
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [eax + 0xa]
        mov    byte ptr [edi + 9], cl
        mov    cl, byte ptr [eax + 0xb]
        mov    byte ptr [edi + 0xa], cl
        mov    byte ptr [edi + 4], 3
        or     byte ptr [0x221b], 4          ; -> loc_1_221B
loc_3_2F17:
        mov    ecx, dword ptr [0x3d22]       ; -> loc_3_3D22
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_3_2F29:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 0x38]
        shl    eax, 0x10
        add    ax, word ptr [ebx + 0x10]
        mov    ebx, eax
        and    eax, 0xffff0000
        and    ebx, 0xffff
        shr    eax, 0xc
        add    eax, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        or     byte ptr [0x221b], 0x40       ; -> loc_1_221B
        mov    byte ptr [eax + 7], 1
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
sub_3_2F6A:
        push   ebx
        movzx  ebx, di
        rol    edi, 0x10
        movzx  edi, di
        shl    edi, 4
        add    edi, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    edi, dword ptr [ebx + 4]
        pop    ebx
        ret    
sub_3_2F84:
        push   ebx
        or     ah, ah
        jne    loc_3_2FBC
        mov    edi, dword ptr [0x3a0b]       ; -> obj3:0x3A0B
        mov    bl, byte ptr [edi + 5]
        or     bl, bl
        je     loc_3_2FED
        mov    byte ptr [0x3a1e], bl         ; -> loc_3_3A1E
        lea    edi, [edi + 6]
        mov    dword ptr [0x3a27], edi       ; -> obj3:0x3A27
loc_3_2FA5:
        mov    bl, byte ptr [edi + 0xe]
        or     bl, bl
        je     loc_3_2FBC
        add    edi, 0xf
loc_3_2FAF:
        cmp    byte ptr [edi], al
        je     loc_3_2FE4
        dec    bl
        je     loc_3_2FBC
        add    edi, 5
        jmp    loc_3_2FAF
loc_3_2FBC:
        dec    byte ptr [0x3a1e]             ; -> loc_3_3A1E
        je     loc_3_2FED
        mov    edi, dword ptr [0x3a27]       ; -> obj3:0x3A27
        movzx  ecx, byte ptr [edi + 0xe]
        push   eax
        mov    eax, 5
        mul    ecx
        add    edi, eax
        pop    eax
        add    edi, 0xf
        mov    dword ptr [0x3a27], edi       ; -> obj3:0x3A27
        jmp    loc_3_2FA5
loc_3_2FE4:
        mov    dword ptr [0x3a1f], edi       ; -> obj3:0x3A1F
        clc    
loc_3_2FEB:
        pop    ebx
        ret    
loc_3_2FED:
        stc    
        jmp    loc_3_2FEB
loc_3_2FF0:
        cmp    word ptr [ebp + 0x1c], 0x300
        jne    loc_3_301C
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    byte ptr [eax + 1], 0x3a
        jne    loc_3_301C
        mov    al, byte ptr [eax]
        or     al, 0x20
        sub    al, 0x61
        cmp    al, byte ptr [0x13e]          ; -> obj5:0x013E
        jne    loc_3_301C
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_3_301C:
        stc    
        ret    
loc_3_301E:
        test   byte ptr [0x221c], 8          ; -> obj1:0x221C
        je     loc_3_30AB
        or     byte ptr [0x221b], 4          ; -> loc_1_221B
        cmp    byte ptr [ebp + 0x1d], 2
        jne    loc_3_30AB
        mov    edi, dword ptr [0x3a33]       ; -> obj3:0x3A33
        mov    dl, byte ptr [ebp + 0x14]
        mov    byte ptr [edi + 1], dl
        mov    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    esi, dword ptr [esi + 0x2c]
loc_3_304D:
        or     esi, esi
        je     loc_3_30AB
        cmp    byte ptr [esi], 0
        je     loc_3_3060
        cmp    byte ptr [esi], 2
        je     loc_3_3060
        cmp    byte ptr [esi], 1
        jne    loc_3_3065
loc_3_3060:
        cmp    byte ptr [esi + 5], dl
        je     loc_3_3070
loc_3_3065:
        cmp    dword ptr [esi + 1], 0
        je     loc_3_30AD
        add    esi, dword ptr [esi + 1]
        jmp    loc_3_304D
loc_3_3070:
        mov    ch, byte ptr [ebp + 0x15]
        mov    cl, byte ptr [ebp + 0x18]
        mov    al, cl
        and    cl, 0x3f
        and    ax, 0xc0
        shl    ax, 2
        mov    al, byte ptr [ebp + 0x19]
        movzx  ebx, ax
        mov    eax, ebx
        movzx  ebx, word ptr [esi + 8]
        mul    ebx
        movzx  ebx, ch
        add    eax, ebx
        movzx  ebx, word ptr [esi + 0xa]
        mul    ebx
        movzx  ebx, cl
        dec    ebx
        add    eax, ebx
        sub    eax, dword ptr [0x3a43]       ; -> obj3:0x3A43
        mov    dword ptr [edi + 6], eax
loc_3_30AB:
        stc    
        ret    
loc_3_30AD:
        mov    dword ptr [esi + 1], 0x1d
        add    esi, 0x1d
        mov    byte ptr [esi], 2
        mov    byte ptr [esi + 5], dl
        or     byte ptr [0x221c], 0x20       ; -> obj1:0x221C
        jmp    loc_3_30AB
loc_3_30C6:
        test   byte ptr [0x221c], 8          ; -> obj1:0x221C
        je     loc_3_311B
        or     byte ptr [0x221b], 4          ; -> loc_1_221B
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    eax, dword ptr [eax + 4]
        mov    ebx, eax
        and    eax, 0xffff0000
        and    ebx, 0xffff
        shr    eax, 0xc
        add    eax, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        cmp    byte ptr [eax], 2
        jne    loc_3_311B
        mov    edi, dword ptr [0x3a33]       ; -> obj3:0x3A33
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [edi + 0xc], cl
        mov    cl, byte ptr [eax + 8]
        mov    byte ptr [edi + 0xa], cl
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 0xb], cl
loc_3_311B:
        mov    eax, dword ptr [0x39f2]       ; -> loc_3_39F2
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
        mov    byte ptr [0x39f6], 0          ; -> loc_3_39F6
        ret    
loc_3_312E:
        test   byte ptr [0x221c], 8          ; -> obj1:0x221C
        je     loc_3_318D
        or     byte ptr [0x221b], 4          ; -> loc_1_221B
        cmp    word ptr [ebp + 0x1c], 0x8100
        jne    loc_3_318D
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 0x38]
        shl    eax, 0x10
        add    ax, word ptr [ebx + 0x10]
        mov    ebx, eax
        and    eax, 0xffff0000
        and    ebx, 0xffff
        shr    eax, 0xc
        add    eax, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [0x3a33]       ; -> obj3:0x3A33
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 0xf], cl
        mov    cl, byte ptr [eax + 0xa]
        mov    byte ptr [edi + 0xd], cl
        mov    cl, byte ptr [eax + 0xb]
        mov    byte ptr [edi + 0xe], cl
loc_3_318D:
        stc    
        ret    
sub_3_318F:
        xor    eax, eax
loc_3_3191:
        call   sub_4_1BC0
        jb     loc_3_31F8
        test   byte ptr [esi + 0x10], 0x10
        je     loc_3_31F8
        mov    esi, dword ptr [eax*4 + 0x20ac] ; -> obj1:0x20AC
        or     esi, esi
        je     loc_3_31F8
        test   byte ptr [esi + 0x22], 0x40
        jne    loc_3_31F8
        push   eax
        mov    eax, dword ptr [eax*4 + 0xbe] ; -> obj5:0x00BE
        mov    eax, dword ptr [eax + 0x45]
        movzx  ecx, ax
        rol    eax, 0x10
        movzx  eax, ax
        shl    eax, 4
        add    eax, ecx
        mov    eax, dword ptr [eax + 0x13]
        mov    ecx, eax
        movzx  edx, cx
        rol    ecx, 0x10
        movzx  ecx, cx
        shl    ecx, 4
        add    ecx, edx
        mov    ax, word ptr [ecx + 8]
        xor    edx, edx
        mov    esi, 0x1bd9                   ; -> loc_4_1BD9
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        mov    ecx, eax
        pop    eax
        jb     loc_3_31F8
        mov    dword ptr [eax*4 + 0x370], ecx ; -> obj5:0x0370
loc_3_31F8:
        inc    eax
        cmp    al, 0x1a
        jb     loc_3_3191
        ret    
        db  000h, 000h
sub_3_3200:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x29], 4
        call   sub_3_32CF
        call   sub_3_33F4
        call   sub_4_1CB5
        call   sub_3_346B
        mov    esi, 0x550                    ; -> obj5:0x0550
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        call   sub_4_1F25
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [edi + 0x28], 0x40
        jne    loc_3_32AF
loc_3_323F:
        call   sub_4_1E68
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        and    byte ptr [ecx + 0x29], 0xfb
        mov    edi, dword ptr [0x3a75]       ; -> obj3:0x3A75
        or     edi, edi
        je     loc_3_32AE
loc_3_3258:
        call   sub_3_2F6A
        cmp    byte ptr [edi], 0xe9
        je     loc_3_3267
        cmp    byte ptr [edi], 0xeb
        jne    loc_3_32AE
loc_3_3267:
        cmp    dword ptr [edi + 3], 0x544e4924
        jne    loc_3_32AE
        cmp    dword ptr [edi + 7], 0x46533331
        jne    loc_3_32AE
        cmp    dword ptr [edi + 0x17], 1
        je     loc_3_329F
        pushfd 
        push   0x3a6c                        ; -> loc_3_3A6C
        push   0x40ac                        ; -> loc_3_40AC
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [eax + 0x28], 2
        jmp    loc_3_32AE
loc_3_329F:
        mov    edi, dword ptr [edi + 0x13]
        mov    ecx, edi
        rol    ecx, 0x10
        cmp    cx, 0xa000
        jb     loc_3_3258
loc_3_32AE:
        ret    
loc_3_32AF:
        mov    esi, 0x558                    ; -> obj5:0x0558
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        call   sub_4_1DA7
        jb     loc_4_223C
        jmp    loc_3_323F
sub_3_32CF:
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    esi, edx
        mov    edi, 0x179                    ; -> obj5:0x0179
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x2fd                    ; -> obj5:0x02FD
        mov    ecx, 9
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [0x1f9], edi        ; -> obj5:0x01F9
        ret    
        db  000h, 000h, 000h
sub_3_32F4:
        pushal 
        xor    edi, edi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    ecx, 0x408
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        cmp    ecx, 0x408
        jne    loc_4_2206
        mov    esi, edi
        call   sub_3_2F6A
        xor    eax, eax
        mov    word ptr [edi], ax
        mov    word ptr [ebp + 0x14], ax
        dec    ax
        mov    word ptr [edi + 2], ax
        mov    word ptr [ebp + 0x18], 1
        mov    eax, esi
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 8], ax
        rol    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x1c], 0xcdcd
        mov    eax, 0x25
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        cmp    word ptr [edi], 0xcdcd
        jne    loc_3_33D1
        cmp    word ptr [edi + 2], 1
        jne    loc_3_33D1
        mov    esi, edi
        mov    edi, dword ptr [esi + 4]
        call   sub_3_2F6A
        cmp    word ptr [edi], 0xa55a
        jne    loc_3_33D1
        mov    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [esi + 0x2a], 1
        cmp    dword ptr [edi + 0x52], 0x50415753
        jne    loc_3_33D1
        lea    esi, [edi + 0x56]
        mov    edi, 0x21e9                   ; -> obj1:0x21E9
        xor    ecx, ecx
loc_3_33B1:
        movzx  eax, byte ptr [ecx + edi]
        cmp    eax, 0xff
        je     loc_3_33CB
        movzx  edx, byte ptr [ecx + esi]
        cmp    ecx, edx
        je     loc_3_33CB
        cmp    eax, edx
        je     loc_3_33CB
        mov    byte ptr [ecx + edi], dl
loc_3_33CB:
        inc    ecx
        cmp    ecx, 0x1a
        jb     loc_3_33B1
loc_3_33D1:
        mov    ecx, 0x408
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
sub_3_33F4:
        push   eax
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x28], 1
        jne    loc_3_3469
        inc    byte ptr [0x3a3b]             ; -> obj3:0x3A3B
        cmp    byte ptr [0x3a3b], 1          ; -> obj3:0x3A3B
        jne    loc_3_3469
        test   byte ptr [eax + 0x28], 0x20
        jne    loc_3_3469
        push   ebx
        push   ecx
        push   edx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  V86MMGR.V86MMGR_Get_Xlat_Buff_State ; cd200a000600
        mov    dword ptr [0x3a47], ebx       ; -> loc_3_3A47
        mov    dword ptr [0x3a4b], eax       ; -> obj3:0x3A4B
        mov    dword ptr [0x3a4f], ecx       ; -> obj3:0x3A4F
        mov    dword ptr [0x3a53], edx       ; -> obj3:0x3A53
        push   0
        push   0x10000
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        or     eax, eax
        je     loc_4_223C
        add    eax, 0xf
        shr    eax, 4
        mov    ecx, 0x10000
        mov    edx, ecx
        VxDCall  V86MMGR.V86MMGR_Set_Xlat_Buff_State ; cd200b000600
        pop    edx
        pop    ecx
        pop    ebx
loc_3_3469:
        pop    eax
        ret    
sub_3_346B:
        push   eax
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x28], 1
        jne    loc_3_34B7
        cmp    byte ptr [0x3a3b], 0          ; -> obj3:0x3A3B
        je     loc_3_34B7
        dec    byte ptr [0x3a3b]             ; -> obj3:0x3A3B
        jne    loc_3_34B7
        test   byte ptr [eax + 0x28], 0x20
        jne    loc_3_34B7
        push   ebx
        push   ecx
        push   edx
        mov    ebx, dword ptr [0x3a47]       ; -> loc_3_3A47
        mov    eax, dword ptr [0x3a4b]       ; -> obj3:0x3A4B
        mov    ecx, dword ptr [0x3a4f]       ; -> obj3:0x3A4F
        mov    edx, dword ptr [0x3a53]       ; -> obj3:0x3A53
        VxDCall  V86MMGR.V86MMGR_Set_Xlat_Buff_State ; cd200b000600
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
        pop    edx
        pop    ecx
        pop    ebx
loc_3_34B7:
        pop    eax
        ret    
sub_3_34B9:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [ecx + 0x30]
        lea    eax, [0x2203]                 ; -> obj1:0x2203
loc_3_34C8:
        mov    edi, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_3_34C8
loc_3_34D0:
        jecxz  loc_3_3534
        test   byte ptr [ecx + 0x21], 0x20
        jne    loc_3_352F
        test   byte ptr [ecx + 0x21], 0x80
        je     loc_3_352F
        bts    dword ptr [ecx + 0x20], 0x1c
        jb     loc_3_352F
        mov    eax, dword ptr [ecx + 0x20]
        and    eax, 0x3f
        or     dword ptr [ecx + 0xcb], eax
        mov    dl, byte ptr [ecx + 0x15]
        mov    byte ptr [ecx + 0xca], dl
        cmp    byte ptr [ecx + 0x40], 0xa
        je     loc_3_350E
        cmp    byte ptr [ecx + 0x40], 0
        jne    loc_3_352F
        test   dl, 0x80
        je     loc_3_352F
        and    dl, 0x7f
loc_3_350E:
        test   dl, 0xf8
        jne    loc_3_352F
        test   byte ptr [ecx + 0x20], 1
        je     loc_3_352F
        mov    dword ptr [edi], ecx
        add    dword ptr [edi], 0xc3
        mov    edi, ecx
        add    edi, 0xc3
        VxDCall  INT13.Int13_Device_Registered   ; cd2001002000
loc_3_352F:
        mov    ecx, dword ptr [ecx + 0xc]
        jmp    loc_3_34D0
loc_3_3534:
        mov    dword ptr [edi], ecx
        ret    
sub_3_3537:
        push   edi
        push   ecx
        push   eax
        mov    cl, byte ptr [edi + 0xe]
        or     cl, cl
        je     loc_3_355E
        add    edi, 0xf
loc_3_3544:
        cmp    byte ptr [edi], 0xa
        je     loc_3_3552
        dec    cl
        je     loc_3_355E
        add    edi, 5
        jmp    loc_3_3544
loc_3_3552:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [edi + 1]
        or     word ptr [eax + 0x6c], cx
loc_3_355E:
        pop    eax
        pop    ecx
        pop    edi
        ret    
sub_3_3562:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edx, dword ptr [0x3a68]       ; -> obj3:0x3A68
        cmp    word ptr [edx + 0xfe], -1
        je     loc_3_3594
        lea    edx, [edx + 0xe]
loc_3_357B:
        cmp    word ptr [edx], 0
        je     loc_3_3594
        test   byte ptr [edx + 7], 0x80
        jne    loc_3_358F
        mov    ax, word ptr [edx + 2]
        or     word ptr [ecx + 0x6c], ax
loc_3_358F:
        add    edx, 0xc
        jmp    loc_3_357B
loc_3_3594:
        ret    
        db  000h, 000h, 000h
sub_3_3598:
        cmp    word ptr [0x460], -1          ; -> obj5:0x0460
        jne    loc_3_35EE
        call   sub_3_33F4
        mov    esi, 0x3d8                    ; -> obj5:0x03D8
        xor    cx, cx
        mov    bx, 0x4002
        mov    dx, 0x11
        mov    ax, 0x6c00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_35F0
        or     byte ptr [0x221c], 0x10       ; -> obj1:0x221C
        mov    word ptr [0x460], ax          ; -> obj5:0x0460
        mov    bx, ax
        mov    ax, 0x4202
        xor    cx, cx
        xor    dx, dx
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_35F0
        call   sub_3_346B
loc_3_35EE:
        clc    
        ret    
loc_3_35F0:
        call   sub_3_346B
        stc    
        ret    
sub_3_35F7:
        cmp    word ptr [0x460], -1          ; -> obj5:0x0460
        je     loc_3_361B
        mov    bx, word ptr [0x460]          ; -> obj5:0x0460
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    word ptr [0x460], 0xffff      ; -> obj5:0x0460
loc_3_361B:
        ret    
sub_3_361C:
        call   sub_3_33F4
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    esi, edx
        mov    edi, 0x3d8                    ; -> obj5:0x03D8
        call   sub_4_21E3
        dec    edi
        mov    esi, 0x458                    ; -> obj5:0x0458
        call   sub_4_21E3
        mov    edx, 0x3d8                    ; -> obj5:0x03D8
        mov    ax, 0x4100
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        call   sub_3_346B
        ret    
sub_3_3655:
        test   byte ptr [0x221c], 0x10       ; -> obj1:0x221C
        je     loc_3_3859
        and    byte ptr [0x221b], 0x7f       ; -> loc_1_221B
        call   sub_3_3598
        mov    edi, dword ptr [0x3a0b]       ; -> obj3:0x3A0B
        movzx  edx, byte ptr [edi + 5]
        add    edi, 6
loc_3_367B:
        mov    esi, 0x40c8                   ; -> obj3:0x40C8
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, edi
        push   edi
        push   edx
        call   sub_3_10CB
        pop    edx
        pop    edi
        push   0x4203                        ; -> loc_3_4203
        call   sub_3_38A0
        add    esp, 4
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, 0x40c8                   ; -> obj3:0x40C8
        push   esi
        call   sub_3_38A0
        add    esp, 4
        movzx  eax, byte ptr [edi + 0xd]
        or     eax, eax
        je     loc_3_36D3
        pushfd 
        push   eax
        push   0x4211                        ; -> obj3:0x4211
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        jmp    loc_3_36E0
loc_3_36D3:
        push   0x4239                        ; -> loc_3_4239
        call   sub_3_38A0
        add    esp, 4
loc_3_36E0:
        movzx  ecx, byte ptr [edi + 0xe]
        push   0x424e                        ; -> loc_3_424E
        call   sub_3_38A0
        add    esp, 4
        mov    ebx, 0x40cb                   ; -> obj3:0x40CB
        lea    edi, [edi + 0xf]
loc_3_36F9:
        movzx  eax, byte ptr [edi]
        mov    esi, dword ptr [ebx + eax*4]
        cmp    al, 7
        je     loc_3_385A
        mov    eax, dword ptr [edi + 1]
        push   esi
        call   sub_3_38A0
        add    esp, 4
        pushfd 
        push   eax
        push   0x4260                        ; -> obj3:0x4260
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
loc_3_3724:
        test   byte ptr [0x221b], 0x80       ; -> loc_1_221B
        jne    loc_3_384C
        add    edi, 5
        loop   loc_3_36F9
        dec    edx
        jne    loc_3_367B
        or     byte ptr [0x221b], 0x80       ; -> loc_1_221B
        mov    esi, 0x40c8                   ; -> obj3:0x40C8
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    edi, dword ptr [0x3a68]       ; -> obj3:0x3A68
        cmp    word ptr [edi + 0xfe], -1
        je     loc_3_3854
        push   0x4265                        ; -> obj3:0x4265
        call   sub_3_38A0
        add    esp, 4
        mov    eax, dword ptr [edi + 6]
        pushfd 
        push   eax
        push   0x4280                        ; -> loc_3_4280
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        lea    edi, [edi + 0xe]
loc_3_378A:
        movzx  esi, word ptr [edi]
        or     esi, esi
        je     loc_3_3854
        dec    esi
        shl    esi, 4
        lea    esi, [esi + 8]
        push   edi
        mov    edi, 0x3ccb                   ; -> loc_3_3CCB
        mov    edx, edi
        xor    al, al
        mov    ecx, 0xb
        rep stosb byte ptr es:[edi], al
        mov    edi, edx
        mov    ecx, 8
loc_3_37B4:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_3_37BC
        stosb  byte ptr es:[edi], al
        loop   loc_3_37B4
loc_3_37BC:
        pop    edi
        mov    esi, edx
        push   0x4285                        ; -> loc_3_4285
        call   sub_3_38A0
        add    esp, 4
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, 0x40c8                   ; -> obj3:0x40C8
        push   esi
        call   sub_3_38A0
        add    esp, 4
        push   0x4290                        ; -> obj3:0x4290
        call   sub_3_38A0
        add    esp, 4
        mov    eax, dword ptr [edi + 8]
        pushfd 
        push   eax
        push   0x42a9                        ; -> obj3:0x42A9
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        movzx  eax, word ptr [edi + 2]
        or     eax, eax
        je     loc_3_382A
        push   0x42ae                        ; -> loc_3_42AE
        call   sub_3_38A0
        add    esp, 4
        pushfd 
        push   eax
        push   0x42d5                        ; -> obj3:0x42D5
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
loc_3_382A:
        mov    esi, dword ptr [edi + 4]
        test   esi, 0x80000000
        je     loc_3_3842
        push   0x42da                        ; -> loc_3_42DA
        call   sub_3_38A0
        add    esp, 4
loc_3_3842:
        and    esi, 0x1fffffff
        or     esi, esi
        jne    loc_3_3874
loc_3_384C:
        add    edi, 0xc
        jmp    loc_3_378A
loc_3_3854:
        call   sub_3_35F7
loc_3_3859:
        ret    
loc_3_385A:
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, 0x40c8                   ; -> obj3:0x40C8
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, dword ptr [edi + 1]
loc_3_3874:
        bsf    eax, esi
        je     loc_3_3724
        btr    esi, eax
        push   0x42ef                        ; -> obj3:0x42EF
        call   sub_3_38A0
        add    esp, 4
        pushfd 
        push   eax
        push   0x4309                        ; -> loc_3_4309
        VxDCall  BLOCKDEV.Service_0010h          ; cd2010001000
        add    esp, 8
        popfd  
        jmp    loc_3_3874
sub_3_38A0:
        pushal 
        xor    esi, esi
        cmp    word ptr [0x460], -1          ; -> obj5:0x0460
        jne    loc_3_38B7
        call   sub_3_3598
        mov    esi, 1
loc_3_38B7:
        mov    edx, dword ptr [esp + 0x24]
        cld    
        mov    edi, edx
        xor    al, al
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        dec    ecx
        cmp    byte ptr [edi - 2], 0xa
        jne    loc_3_38D1
        dec    ecx
loc_3_38D1:
        mov    bx, word ptr [0x460]          ; -> obj5:0x0460
        mov    ah, 0x40
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        or     esi, esi
        je     loc_3_38EB
        call   sub_3_35F7
loc_3_38EB:
        popal  
        ret    
        db  000h, 000h, 000h
loc_3_38F0:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
loc_3_38FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3900:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    esp
        inc    ebp
        push   esi
        and    byte ptr [ebx + 0x4f], al
        dec    ebp
        push   eax
        inc    ecx
        push   esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [edi], bh
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        aas    
        or     eax, dword ptr [eax]
        add    dword ptr [eax], eax
loc_3_3962:
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], ch        ; -> loc_1_01A8
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl      ; -> loc_1_009C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x69], dl
        jb     loc_3_39FC
        jne    loc_3_39EB
        insb   byte ptr es:[edi], dx
        dec    eax
        inc    esp
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
loc_3_399C:
        dec    eax
        popal  
        jb     loc_3_3A04
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x42414d44
        jne    loc_3_3A10
        jb     loc_3_39AE
loc_3_39AE:
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     loc_3_3A15
        push   eax
        jb     0x3a25
        je     0x3a05
        outsd  dx, dword ptr [esi]
        dec    ecx
        dec    edi
        add    byte ptr [ecx + 0x4f], cl
        push   ebx
        inc    esp
        dec    ebp
        inc    ecx
        inc    edx
        jne    0x3a2d
        jb     loc_3_3A1E
        imul   edi, dword ptr [edx + 0x65], 0x534f4900
        dec    eax
        popal  
        jo     loc_3_3A2A
        imul   edi, dword ptr [edx + 0x65], 0x69724400
        jbe    loc_3_3A45
        push   ebx
        jo     loc_3_3A4C
        outsb  dx, byte ptr [esi]
        inc    esp
        outsd  dx, dword ptr [esi]
        ja     loc_3_3A56
        add    byte ptr [ebx + 0x43], dl
loc_3_39EB:
        push   ebx
        dec    ecx
        dec    ebp
        inc    edi
        push   edx
        and    al, 0
loc_3_39F2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_39F6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_39FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h
loc_3_3A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_3A15:
        add    byte ptr [eax], al
loc_3_3A17:
        add    byte ptr [eax], al
loc_3_3A19:
        add    byte ptr [eax], al
loc_3_3A1B:
        add    byte ptr [eax], al
        db  000h
loc_3_3A1E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
loc_3_3A45:
        add    byte ptr [eax], al
loc_3_3A47:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_3A4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A56:
        add    byte ptr [eax], al
loc_3_3A58:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A5C:
        inc    esp
        inc    edx
        dec    esp
        push   ebx
        push   eax
        inc    ecx
        inc    ebx
        inc    ebp
        inc    edx
        dec    ecx
        dec    esi
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_3_3A6C:
        dec    ebp
        inc    edx
        push   edx
        dec    ecx
        dec    esi
        push   esp
        xor    dword ptr [ebx], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    loc_3_3AF1
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x3af5
        jb     0x3aea
        outsb  dx, byte ptr [esi]
        je     0x3acb
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_3_3AFE
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_3_3AEE
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3b09
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 0x53], 0x65747379
        insd   dword ptr es:[edi], dx
        add    byte ptr [esi + 0x6f], al
        jb     loc_3_3B0C
        push   edx
        dec    ebp
        dec    ecx
        dec    edi
        add    byte ptr [esi + 0x6f], al
        jb     loc_3_3B16
        push   eax
        dec    ebp
        dec    ecx
        dec    edi
        add    byte ptr [ecx + 0x73], cl
        je     loc_3_3B00
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_3_3B12
        dec    ebp
        inc    esp
        jb     loc_3_3B3C
        jae    loc_3_3AC8
loc_3_3AC8:
        inc    edx
        popal  
        je     loc_3_3B10
        jb     0x3b37
        jbe    0x3b35
        push   ebx
        jo     loc_3_3B3C
        outsb  dx, byte ptr [esi]
        inc    esp
        outsd  dx, dword ptr [esi]
        ja     loc_3_3B46
        add    byte ptr [ecx + 0x43], al
        inc    esp
        jb     0x3b47
        jbe    0x3b45
        push   ebx
        jo     loc_3_3B4C
        outsb  dx, byte ptr [esi]
        inc    esp
        outsd  dx, dword ptr [esi]
        ja     loc_3_3B56
        add    byte ptr [edi + 0x61], dl
        jb     0x3b5b
        dec    edi
loc_3_3AEE:
        outsb  dx, byte ptr [esi]
        push   ebp
        outsb  dx, byte ptr [esi]
loc_3_3AF1:
        jae    loc_3_3B54
        inc    sp
        jb     0x3b61
        jbe    0x3b5f
        jb     loc_3_3AFC
loc_3_3AFC:
        add    byte ptr [ebx], bh
loc_3_3AFE:
        add    byte ptr [eax], al
loc_3_3B00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B10:
        add    byte ptr [eax], al
loc_3_3B12:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B16:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B3C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B46:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B54:
        add    byte ptr [eax], al
loc_3_3B56:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B80:
        cwde   
        cmp    eax, dword ptr [eax]
        add    byte ptr [ebp - 0x4affffc5], ah ; -> obj3:0x3BA5
        cmp    eax, dword ptr [eax]
        add    cl, al                        ; -> obj3:0x3BC1
        cmp    eax, dword ptr [eax]
        add    bl, cl                        ; -> obj3:0x3BCB
        cmp    eax, dword ptr [eax]
        add    bh, dl                        ; -> obj3:0x3BD7
        cmp    eax, dword ptr [eax]
        add    byte ptr [esp], cl
        outsd  dx, dword ptr [esi]
        pop    edi
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_3_3BFF
        arpl   word ptr [ecx + 0x72], sp
        add    byte ptr gs:[edi], cl
        insd   dword ptr es:[edi], dx
        jne    0x3c1c
        je     loc_3_3C0A
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_3_3C0E
        arpl   word ptr [eax + 0x61], bp
        db  069h, 06Eh, 000h
loc_3_3BB5:
        or     ebp, dword ptr [ebp + 0x75]
        jae    0x3c2e
        pop    edi
        arpl   word ptr [eax + 0x61], bp
        imul   ebp, dword ptr [esi], 0x6e6f6e09
        pop    edi
        imul   esi, dword ptr fs:[ebx + 0x6b], 0x6f6d0b00
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [eax + ebp*2 + 0x69], 0x6c0c0063
        jne    loc_3_3C49
        jp     0x3c42
        jb     0x3c4e
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        jns    loc_3_3BE4
loc_3_3BE4:
        dec    ecx
        dec    edi
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[eax], ah
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
loc_3_3BFF:
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[eax], ah
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [ecx + 0x46], cl
loc_3_3C0A:
        push   ebx
        dec    eax
        dec    esp
        push   eax
loc_3_3C0E:
        and    byte ptr [eax], ah
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [eax + 0x49], cl
        dec    ebp
        inc    ebp
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x59], dl
        push   ebx
        add    byte ptr [ebx + 0x45], dl
        push   esp
        push   esi
        inc    ebp
        push   edx
        and    byte ptr [eax], ah
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [edx + eax*2 + 0x4c], al
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        and    byte ptr [ebx + 0x59], dl
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3C49:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3CCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        push   ebx
        dec    ecx
        dec    esi
        dec    ecx
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_3CEA:
        push   eax
        dec    ebp
        inc    esp
        jb     loc_3_3D58
        jbe    0x3d56
        jb     0x3d41
        popal  
        insd   dword ptr es:[edi], dx
        db  065h, 000h
loc_3_3CF7:
        inc    ebx
        inc    esp
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    0x3d5e
        add    byte ptr gs:[ebx + 0x61], dl
        dec    sp
        imul   esi, dword ptr [ebx + 0x74], 0
        push   ebp
        dec    esi
        push   ebx
        inc    ecx
        inc    esi
        inc    ebp
        and    byte ptr [ebx + 0x44], al
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [edx + edx*2 + 0x49], al
        push   esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [eax], al
loc_3_3D22:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3D26:
        inc    esp
        outsd  dx, dword ptr [esi]
        ja     0x3d98
        jb     loc_3_3D91
        jbe    loc_3_3D4E
        jbe    0x3d95
        jb     loc_3_3DA5
        imul   ebp, dword ptr [edi + 0x6e], 0x20666f20
        dec    ecx
        inc    esp
        inc    ebp
        and    byte ptr [ebx + 0x6f], dh
        je     0x3db9
        popal  
        jb     0x3daa
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     loc_3_3DBD
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
loc_3_3D4E:
        imul   ebp, dword ptr [esi + 0x67], 0x6f6f7420
        and    byte ptr [ebp + 0x61], ch
loc_3_3D58:
        outsb  dx, byte ptr [esi]
        jns    loc_3_3D7B
        imul   ebp, dword ptr [esi + 0x74], 0x20333120
        jb     loc_3_3DCE
        jbe    loc_3_3DCC
        jae    0x3d97
        db  00Dh, 00Ah, 000h
loc_3_3D6C:
        push   esp
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        and    byte ptr [ebp + 0x61], ch
        outsb  dx, byte ptr [esi]
        jns    0x3d95
        push   esp
        push   ebx
        push   edx
        jae    loc_3_3D9A
        popal  
loc_3_3D7B:
        jb     0x3de2
        and    byte ptr [eax + 0x6f], ch
        outsd  dx, dword ptr [esi]
        imul   ebp, dword ptr [ecx + 0x6e], 0x67
        and    byte ptr [ecx + 0x4e], cl
        push   esp
        and    byte ptr [ecx], dh
        xor    esp, dword ptr [eax]
        and    byte ptr es:[eax + 0x2f], ch
loc_3_3D91:
        ja     loc_3_3DB3
        imul   ebp, dword ptr [esi + 0x74], 0x75727265
loc_3_3D9A:
        jo     0x3e10
        jae    loc_3_3DCC
        or     eax, 0x6e55000a
        dec    ebx
        outsb  dx, byte ptr [esi]
loc_3_3DA5:
        outsd  dx, dword ptr [esi]
        ja     loc_3_3E16
        and    byte ptr [ecx + 0x6e], ch
        je     loc_3_3DCD
        xor    dword ptr [ebx], esi
        and    byte ptr [eax + 0x6f], ch
        outsd  dx, dword ptr [esi]
loc_3_3DB3:
        imul   esp, dword ptr [ebp + 0x72], 0x20
        and    eax, 0xa0d73
loc_3_3DBC:
        inc    esp
loc_3_3DBD:
        dec    edi
        pop    edi
        dec    esi
        dec    edi
        push   esp
        pop    edi
        inc    ebx
        inc    ecx
        push   edx
        inc    ebp
        sub    ecx, dword ptr [ebp + 0x55]
        push   ebx
        push   esp
loc_3_3DCC:
        pop    edi
loc_3_3DCD:
        inc    ebx
loc_3_3DCE:
        dec    eax
        inc    ecx
        dec    ecx
        dec    esi
        and    byte ptr [ecx + 0x6e], ch
        je     0x3e08
        xor    esp, dword ptr [eax]
        jno    0x3e50
        popal  
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [esi + 0x69], 0x63207265
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        bound  ebp, qword ptr [ecx + 0x6e]
        popal  
        je     loc_3_3E55
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        or     eax, 0x5520000a
        jae    loc_3_3E5E
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + si + 0x4d], dl
        dec    ebp
        and    byte ptr [esi + 0x6f], ah
        jb     0x3e20
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x67], ch
        imul   esp, dword ptr [ebx + 0x61], 0x7264206c
        imul   esi, dword ptr [esi + 0x65], 0x43202e73
        popal  
loc_3_3E16:
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x74], dh
        imul   ebp, dword ptr [esp + ebp*2 + 0x20], 0x64616f6c
        and    byte ptr [eax + 0x6f], dl
        jb     0x3e9b
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    0x3e92
        jb     0x3ea2
        or     eax, 0x6c49000a
        insb   byte ptr es:[edi], dx
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x6e], ch
        je     loc_3_3E5E
        xor    dword ptr [ebx], esi
        and    byte ptr [ecx + 0x75], dl
        popal  
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [esi + 0x69], 0x63207265
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        bound  ebp, qword ptr [ecx + 0x6e]
        popal  
        je     0x3ebd
        outsd  dx, dword ptr [esi]
loc_3_3E55:
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[eax + 0x75], dl
        outsb  dx, byte ptr [esi]
        je     0x3e7d
        push   eax
loc_3_3E5E:
        outsd  dx, dword ptr [esi]
        jb     loc_3_3ED5
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    loc_3_3ECC
        jb     0x3edc
        or     eax, 0x6e49000a
        arpl   word ptr [edi + 0x6d], bp
        jo     loc_3_3ED4
        je     0x3ede
        bound  ebp, qword ptr [ebp + 0x20]
        jb     0x3ee5
        jbe    0x3ee3
        jb     0x3ea0
        and    eax, 0x20202e73
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[esi + 0x61], al
        jae    loc_3_3F02
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x726f6620
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x3f01
        jb     0x3ebe
        imul   esp, dword ptr [esi + 0x20], 0x73736f70
        imul   esp, dword ptr [edx + 0x6c], 0xa0d65
loc_3_3EAC:
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    0x3f11
        and    byte ptr gs:[edx + esi*2 + 0x69], ah
        jbe    0x3f1d
        jb     0x3eda
        and    eax, 0x6f632073
        outsb  dx, byte ptr [esi]
        je     0x3f34
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x67], 0x696e7520
loc_3_3ECC:
        je     loc_3_3EEE
        and    eax, 0xa0d62
loc_3_3ED3:
        dec    ebp
loc_3_3ED4:
        outsd  dx, dword ptr [esi]
loc_3_3ED5:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [eax + ebp*2 + 0x69], 0x72642063
        imul   esi, dword ptr [esi + 0x65], 0x73252072
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x3f5f
        outsd  dx, dword ptr [esi]
loc_3_3EEE:
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x67], 0x696e7520
        je     0x3f19
        and    eax, 0xa0d62
loc_3_3EFE:
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    loc_3_3F63
loc_3_3F02:
        and    byte ptr gs:[ebx + 0x44], al
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x3f75
        jb     0x3f3e
        and    byte ptr [0x64202c73], ah
        imul   esi, dword ptr [ebx + 0x61], 0x6e696c62
        and    byte ptr [bx + si + 0x72], dh
        outsd  dx, dword ptr [esi]
        je     loc_3_3F8B
        arpl   word ptr [eax + 0x6d], si
        outsd  dx, dword ptr [esi]
        and    byte ptr gs:[ebx + 0x44], al
        push   edx
        dec    edi
        dec    ebp
        or     eax, 0x6e55000a
        jae    0x3f9b
        and    byte ptr gs:[edx + esi*2 + 0x69], ah
        jbe    0x3fa7
        jb     0x3f64
        and    eax, 0x6f682073
        outsd  dx, dword ptr [esi]
        imul   ebp, dword ptr [ecx + 0x6e], 0x67
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x6f], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr cs:[edi + ebp*2 + 0x67], ch
        imul   esp, dword ptr [ebx + 0x61], 0x6e75206c
loc_3_3F63:
        imul   esi, dword ptr [ebx + esi*2 + 0x2e], 0x20000a0d
        and    byte ptr [ebp + 0x6e], dh
        imul   esi, dword ptr [eax + 0x6e], 0x65626d75
        jb     0x3fb2
        and    byte ptr [0xa0d62], ah
loc_3_3F7E:
        inc    ebp
        jb     0x3ff3
        outsd  dx, dword ptr [esi]
        jb     loc_3_3FA4
        imul   esi, dword ptr [ebx + 0x73], 0x676e6975
loc_3_3F8B:
        and    byte ptr [ecx + 0x6e], ch
        je     0x3fb0
        xor    dh, byte ptr [0x75203a68]
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x25], 0x65202c62
        jb     0x4013
        outsd  dx, dword ptr [esi]
        jb     0x3fc4
loc_3_3FA4:
        and    eax, 0xa0d65
loc_3_3FA9:
        push   eax
        jne    loc_3_401A
        je     loc_3_4017
        outsb  dx, byte ptr [esi]
        and    byte ptr [di + 0x69], ch
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x6f], 0x20737472
        bound  esp, qword ptr [ebp + 0x63]
        popal  
        jne    loc_3_4034
        and    byte ptr gs:[edi + 0x66], ch
        and    byte ptr [ebp + 0x6e], dh
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x53], al
        push   eax
        dec    ecx
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x403d
        jb     0x3ffa
        and    eax, 0xa0d73
loc_3_3FDF:
        push   eax
        jne    loc_3_4050
        je     loc_3_404D
        outsb  dx, byte ptr [esi]
        and    byte ptr [di + 0x69], ch
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x6f], 0x20737472
        bound  esp, qword ptr [ebp + 0x63]
        popal  
        jne    loc_3_406A
        and    byte ptr gs:[edi + 0x66], ch
        and    byte ptr [ebp + 0x6e], dh
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x41], al
        dec    ebp
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x4072
        jb     0x402f
        and    eax, 0xa0d73
loc_3_4014:
        push   eax
        jne    0x4085
loc_3_4017:
        je     0x4082
        outsb  dx, byte ptr [esi]
loc_3_401A:
        and    byte ptr [di + 0x69], ch
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x6f], 0x20737472
        bound  esp, qword ptr [ebp + 0x63]
        popal  
        jne    loc_3_409F
        and    byte ptr gs:[edi + 0x66], ch
        and    byte ptr [edi + esi*2 + 0x6f], dh
loc_3_4034:
        and    byte ptr [ecx + 0x64], ch
        outsb  dx, byte ptr gs:[esi]
        je     0x40a4
        arpl   word ptr [ecx + 0x6c], sp
        and    byte ptr [eax + 0x65], dh
        jb     loc_3_40AC
        jo     loc_3_40AD
        jb     0x40a9
        insb   byte ptr es:[edi], dx
        jae    loc_3_4058
        or     al, byte ptr [eax]
loc_3_404D:
        inc    ebx
        inc    ecx
        dec    ebp
loc_3_4050:
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x40bb
        jb     0x4078
loc_3_4058:
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x55], al
        push   esp
        dec    edi
        inc    ebp
        pop    eax
loc_3_406A:
        inc    ebp
        inc    ebx
        inc    edx
        inc    ecx
        push   esp
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ecx + 0x74], ch
        jb     0x4087
        or     al, byte ptr [eax]
loc_3_407C:
        inc    ecx
        push   ebx
        push   eax
        dec    ecx
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    loc_3_40EB
        jb     0x40a8
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x55], al
        push   esp
        dec    edi
        inc    ebp
        pop    eax
        inc    ebp
        inc    ebx
        inc    edx
        inc    ecx
loc_3_409F:
        push   esp
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ecx + 0x74], ch
        jb     0x40b7
        or     al, byte ptr [eax]
loc_3_40AC:
        push   ebp
loc_3_40AD:
        outsb  dx, byte ptr [esi]
        dec    ebx
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        ja     loc_3_4121
        and    byte ptr [ecx + 0x6e], ch
        je     0x40d8
        xor    dword ptr [ebx], esi
        and    byte ptr [eax + 0x6f], ch
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x72], 0x20
        and    eax, 0xa0d73
        add    byte ptr [0x40f7000a], cl     ; -> loc_3_40F7
        add    byte ptr [eax], al
        push   es
        inc    ecx
        add    byte ptr [eax], al
        sbb    eax, dword ptr [ecx]
        add    byte ptr [0x40000041], ch     ; -> obj3:0x412D
        inc    ecx
        add    byte ptr [eax], al
        push   edx
        inc    ecx
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [eax], al
        jne    0x412a
        add    byte ptr [eax], al
loc_3_40EB:
        mov    dword ptr [0xc0000041], eax   ; -> obj3:0x41C0
        inc    ecx
        add    byte ptr [eax], al
        loope  loc_3_4136
        add    byte ptr [eax], al
loc_3_40F7:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    edx
        popal  
        and    byte ptr fs:[ecx + 0x6e], cl
        outsw  dx, word ptr [esi]
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    0x4174
        jb     0x4131
        inc    ecx
        jb     loc_3_417B
        jae    0x418b
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 0x4e], cl
loc_3_4121:
        push   esp
        and    byte ptr [ecx], dh
        xor    esp, dword ptr [eax]
        dec    eax
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   edi, dword ptr [edx], 0x20
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 0x4f], cl
        push   ebx
        pop    ecx
        push   ebx
loc_3_4136:
        and    byte ptr [ecx + 0x4e], cl
        push   esp
        and    byte ptr [ecx], dh
        xor    edi, dword ptr [edx]
        and    byte ptr [eax], al
loc_3_4140:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    ecx
        dec    esi
        push   esp
        and    byte ptr [edx + eax*2], dh
        and    byte ptr [eax + 0x6f], cl
        outsd  dx, dword ptr [esi]
        imul   edi, dword ptr [edx], 0x20
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 0x4e], cl
        push   esp
        and    byte ptr [esi + eax*2], dh
        and    byte ptr [eax + 0x6f], cl
        outsd  dx, dword ptr [esi]
        imul   edi, dword ptr [edx], 0x20
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 0x53], al
        push   eax
        dec    ecx
        and    byte ptr [ebp + 0x6e], al
        je     loc_3_41E3
        jns    0x41ad
        and    byte ptr [eax], al
loc_3_4175:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    eax
        outsd  dx, dword ptr [esi]
loc_3_417B:
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [ebx + 0x20], 0x74
        push   0x6f462065
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr cs:[edi + ebp*2 + 0x67], ch
        imul   esp, dword ptr [ebx + 0x61], 0x6e75206c
        imul   esi, dword ptr [ebx + esi*2 + 0x20], 0x413d3028
        sub    al, 0x2e
        sub    dword ptr cs:[edx], edi
        and    byte ptr [eax], al
loc_3_41A3:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x2e], 0x7973
        jae    0x41d2
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x65], 0x6d756e20
        bound  esp, qword ptr [ebp + 0x72]
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x2e], 0x7973
        jae    0x41ef
        popal  
        je     0x4238
        das    
        je     0x423f
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[ebx + 0x74], dh
        popal  
        insd   dword ptr es:[edi], dx
        jo     loc_3_4219
        and    byte ptr [eax], al
loc_3_41E1:
        and    byte ptr [eax], ah
loc_3_41E3:
        and    byte ptr [eax], ah
        dec    eax
        popal  
        jb     0x424d
        ja     loc_3_424C
        jb     0x4252
        and    byte ptr [ecx + 0x6e], ch
        je     loc_3_4257
        jb     0x4266
        jne    0x4266
        je     0x4218
        push   0x206b6f6f
        insd   dword ptr es:[edi], dx
        popal  
        jo     loc_3_423B
        and    byte ptr [eax], al
loc_3_4203:
        inc    esp
        jb     loc_3_426F
        jbe    0x426d
        jb     0x422a
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        cmp    ah, byte ptr gs:[eax]
        add    byte ptr [eax], ah
        and    byte ptr [edx + 0x6c], al
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
loc_3_4219:
        inc    esp
        jb     loc_3_4285
        jbe    0x4283
        jb     0x4240
        arpl   word ptr [edi + 0x6e], bp
        je     0x4297
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x67], 0x20622520
        jne    loc_3_429F
        imul   esi, dword ptr [eax + ebp + 0x73], 0xa0d29
loc_3_4239:
        and    byte ptr [eax], ah
loc_3_423B:
        inc    ebx
        push   0x63617261
        je     loc_3_42A8
        jb     0x4265
        inc    esp
        jb     0x42b1
        jbe    0x42af
        jb     loc_3_4259
loc_3_424C:
        or     al, byte ptr [eax]
loc_3_424E:
        and    byte ptr [eax], ah
        inc    esp
        jb     0x42bc
        jbe    0x42ba
        jb     0x4277
loc_3_4257:
        dec    ecx
        outsb  dx, byte ptr [esi]
loc_3_4259:
        outsw  dx, word ptr [esi]
        cmp    ah, byte ptr [eax]
        or     eax, 0x6525000a
        or     eax, 0x7541000a
        je     0x42d8
        js     0x42d1
        arpl   word ptr [eax], sp
        inc    esp
loc_3_426F:
        popal  
        je     0x42d7
        das    
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x61745320
        insd   dword ptr es:[edi], dx
        jo     0x42b8
        and    byte ptr [eax], al
loc_3_4280:
        and    eax, 0xa0d65
loc_3_4285:
        push   esp
        push   ebx
        push   edx
        and    byte ptr [esi + 0x61], cl
        insd   dword ptr es:[edi], dx
        cmp    ah, byte ptr gs:[eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    ecx
        jne    0x430c
        outsd  dx, dword ptr [esi]
        js     0x4301
        arpl   word ptr [eax], sp
        push   eax
loc_3_429F:
        outsd  dx, dword ptr [esi]
        jae    0x430b
        je     0x430d
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
loc_3_42A8:
        add    byte ptr [0xa0d65], ah
loc_3_42AE:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 0x61], cl
        jb     0x431b
        ja     0x431a
        jb     0x4320
        and    byte ptr [ecx + 0x6e], cl
        je     0x4325
        jb     0x4334
        jne    0x4334
        je     loc_3_42E6
        dec    eax
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [eax], 0x42
        imul   esi, dword ptr [eax + 0x4d], 0x203a7061
        add    byte ptr [0xa0d65], ah
loc_3_42DA:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 0x6f], cl
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [ebx + 0x3a], 0x20
loc_3_42E6:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x430a
        xor    dword ptr [ebx], esi
        or     eax, 0x2020000a
        and    byte ptr [eax], ah
        and    byte ptr [eax + 0x6f], cl
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [ebx + 0x3a], 0x20
        jne    0x436b
        imul   esi, dword ptr [eax + 0x6e], 0x65626d75
        jb     0x4341
        and    byte ptr [eax], al
loc_3_4309:
        and    eax, 0xa0d62
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x31ad  R-X
sub_4_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
IOS_BD_Register_Device:
        pushal 
        mov    dword ptr [0x1f94], edi       ; -> obj1:0x1F94
        lea    edi, [edi + 0x34]
        push   0xc
        pop    ecx
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [0x1f94]       ; -> obj1:0x1F94
        mov    dword ptr [0x38fc], 0x145     ; -> loc_3_38FC
        mov    dword ptr [0x3900], 0         ; -> loc_3_3900
        push   0x38f0                        ; -> loc_3_38F0
        call   IOS_Register
        pop    eax
        cmp    word ptr [0x391c], 1          ; -> obj3:0x391C
        je     loc_4_0077
        jmp    loc_4_00A6
loc_4_0077:
        cmp    dword ptr [edi + 0x30], 0
        je     loc_4_0084
        call   sub_3_0116
        jb     loc_4_00A6
loc_4_0084:
        test   dword ptr [edi + 8], 1
        je     loc_4_0099
        call   sub_1_05C2
        push   edi
        call   sub_3_34B9
        pop    edi
loc_4_0099:
        clc    
loc_4_009A:
        mov    dword ptr [0x1f94], 0         ; -> obj1:0x1F94
        popal  
        ret    
loc_4_00A6:
        stc    
        jmp    loc_4_009A
IOS_Get_Device_List:
        mov    edi, dword ptr [0x2203]       ; -> obj1:0x2203
        ret    
IOS_Find_Int13_Drive:
        lea    edi, [0x2203]                 ; -> obj1:0x2203
loc_4_00B6:
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_00C2
        cmp    byte ptr [edi + 7], al
        jne    loc_4_00B6
        ret    
loc_4_00C2:
        stc    
        ret    
sub_4_00C4:
        jl     0xe7
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah            ; -> loc_7_0061
        add    byte ptr [eax], al
IOS_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 4
        ja     loc_4_00F7
        call   sub_4_00C4
        jb     loc_4_00F7
        mov    word ptr [ebp + 0x1c], 0
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_4_00F7:
        mov    word ptr [ebp + 0x1c], 0xffff
        or     byte ptr [ebp + 0x2c], 1
        ret    
        db  000h, 000h
IOS_Register:
        cld    
        pushal 
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0x24]
        lea    esi, [ebx + 0x14]
        call   sub_4_2198
        jb     loc_4_0260
loc_4_0119:
        mov    edi, dword ptr [ebx + 0x10]
        mov    dword ptr [edi + 0x10], 0x14e8 ; -> loc_1_14E8
        mov    dword ptr [edi + 0x14], 0x14da ; -> sub_1_14DA
        mov    dword ptr [edi], 0x1310       ; -> sub_1_1310
        mov    dword ptr [edi + 4], 0x25f    ; -> loc_4_025F
        mov    dword ptr [edi + 8], 0x1498   ; -> loc_1_1498
        mov    dword ptr [edi + 0xc], 0x14a8 ; -> sub_1_14A8
        mov    dword ptr [edi + 0x20], 0x1989 ; -> sub_1_1989
        mov    dword ptr [edi + 0x24], 0x18d0 ; -> sub_1_18D0
        mov    byte ptr [edi + 0x32], 0
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [edi + 0x1c], eax
        push   0
        push   0x58
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0A3F
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [eax + 0x10], 0
        mov    bl, byte ptr [0x32d]          ; -> obj5:0x032D
        add    byte ptr [0x32c], bl          ; -> obj5:0x032C
        mov    byte ptr [0x32d], 0           ; -> obj5:0x032D
        mov    dword ptr [edi + 0x18], eax
        push   eax
        mov    edi, eax
        mov    ebx, dword ptr [ebp + 0x24]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 8], eax
        mov    word ptr [edi + 6], 0
        mov    byte ptr [edi + 0x4f], 0
        mov    byte ptr [edi + 0x14], 0
        mov    eax, edi
        mov    esi, dword ptr [0x36c]        ; -> obj5:0x036C
        or     esi, esi
        je     loc_4_01D2
        mov    dword ptr [esi + 0x18], edi
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edi + 0x45], ecx
        add    edi, 0x14
        lea    esi, [esi + 0xb]
        call   sub_4_21E3
loc_4_01D2:
        lea    edi, [eax + 0x38]
        lea    esi, [ebx + 0x25]
        mov    ecx, 4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0x29]
        mov    ecx, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0x2b]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0x2e]
        mov    ecx, 4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ebx
        mov    dword ptr [0x145], ebx        ; -> obj5:0x0145
        mov    edi, ebx
        mov    ebx, dword ptr [ebp + 0x24]
        mov    word ptr [ebx + 0x2c], 1
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        je     loc_4_0255
        movzx  ecx, byte ptr [0x153]         ; -> obj5:0x0153
loc_4_0217:
        bsr    eax, ebx
        btr    ebx, eax
        loop   loc_4_0217
        lea    esi, [0x1fa2]                 ; -> obj1:0x1FA2
loc_4_0225:
        mov    ecx, esi
        mov    esi, dword ptr [esi + 2]
        or     esi, esi
        je     loc_4_0233
        cmp    byte ptr [esi + 0x44], al
        ja     loc_4_0225
loc_4_0233:
        mov    dword ptr [ecx + 2], edi
        mov    dword ptr [edi + 2], esi
        mov    byte ptr [0x152], al          ; -> obj5:0x0152
        xor    ebx, ebx
        bts    ebx, eax
        mov    dword ptr [0x14e], ebx        ; -> obj5:0x014E
        mov    byte ptr [edi + 0x44], al
        cmp    eax, 0x1f
        jbe    loc_4_0309
loc_4_0255:
        mov    ebx, dword ptr [ebp + 0x24]
        mov    word ptr [ebx + 0x2c], 4
        popal  
loc_4_025F:
        ret    
loc_4_0260:
        cmp    dword ptr [ebx + 8], 0x100000
        jne    loc_4_0119
        mov    word ptr [ebx + 0x2c], 1
        mov    dword ptr [0x145], edi        ; -> obj5:0x0145
        mov    eax, dword ptr [ebx + 0x2e]
        mov    dword ptr [edi + 0x3f], eax
        call   sub_4_0383
        jmp    loc_4_0322
sub_4_0289:
        add    byte ptr [ebx], 0
        add    byte ptr [ebx + eax], ah      ; -> loc_4_0324
        add    byte ptr [eax], al
        add    byte ptr [ebx], 0
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_4_032E
        add    eax, dword ptr [eax]
        add    byte ptr [ebx - 0x7cfffffd], al ; -> sub_4_0383
        add    eax, dword ptr [eax]
        add    byte ptr [edx - 0x7cfffffd], al ; -> loc_4_0382
        add    eax, dword ptr [eax]
        add    byte ptr [ebx + 0x24000003], al ; -> sub_4_0383
        add    eax, dword ptr [eax]
        add    byte ptr [ebx - 0x7cfffffd], al ; -> sub_4_0383
        add    eax, dword ptr [eax]
        add    byte ptr [ebx + 0x24000003], al ; -> sub_4_0383
        add    eax, dword ptr [eax]
        add    byte ptr [ebx - 0x7cfffffd], al ; -> sub_4_0383
        add    eax, dword ptr [eax]
        add    byte ptr [ebx + eax], ah      ; -> loc_4_0324
        add    byte ptr [eax], al
loc_4_0309:
        call   sub_4_0289
        cmp    dword ptr [0x145], 0          ; -> obj5:0x0145
        jne    loc_4_0322
        mov    ebx, dword ptr [ebp + 0x24]
        mov    word ptr [ebx + 0x2c], 3
loc_4_0322:
        popal  
        ret    
loc_4_0324:
        mov    ebx, dword ptr [ebp + 0x24]
        mov    word ptr [ebx + 0x2c], 4
        ret    
loc_4_032E:
        sub    esp, 0x20
        mov    esi, esp
        mov    word ptr [esi], 0
        call   sub_4_0982
        add    esp, 0x20
        mov    edi, dword ptr [0x145]        ; -> obj5:0x0145
        test   byte ptr [edi + 0x3a], 2
        jne    loc_4_0381
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edi, dword ptr [edi + 0x30]
loc_4_0355:
        or     edi, edi
        je     loc_4_0381
        sub    esp, 0x10
        mov    esi, esp
        mov    word ptr [esi], 3
        mov    dword ptr [esi + 0xc], edi
        push   edi
        mov    edi, dword ptr [0x145]        ; -> obj5:0x0145
        mov    ebx, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 4], ebx
        pop    edi
        call   sub_4_0982
        add    esp, 0x10
        mov    edi, dword ptr [edi + 0xc]
        jmp    loc_4_0355
loc_4_0381:
        ret    
loc_4_0382:
        ret    
sub_4_0383:
        push   1
        push   0x12b
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0A3F
        mov    word ptr [eax + 0x3e], 0x4342
        mov    dword ptr [0x33e], eax        ; -> obj5:0x033E
        mov    byte ptr [0x14d], 0           ; -> obj5:0x014D
loc_4_03AD:
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edi, dword ptr [0x145]        ; -> obj5:0x0145
        mov    dword ptr [edi + 0x10], 0
        sub    esp, 0x20
        mov    esi, esp
        mov    word ptr [esi], 0
        mov    edi, dword ptr [0x344]        ; -> obj5:0x0344
        mov    dword ptr [esi + 0x17], edi
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        mov    dword ptr [esi + 0x13], edi
        lea    eax, [0x2207]                 ; -> loc_1_2207
        mov    dword ptr [edi + 8], eax
        mov    ebx, dword ptr [0x145]        ; -> obj5:0x0145
        mov    byte ptr [esi + 0x10], 0
        mov    eax, dword ptr [ebx + 0x3f]
        mov    dword ptr [esi + 0xc], eax
        call   sub_4_0982
        movzx  ecx, word ptr [esi + 2]
        mov    dl, byte ptr [esi + 0x10]
        mov    byte ptr [0x14d], dl          ; -> obj5:0x014D
        test   dl, 1
        je     loc_4_0410
        or     byte ptr [ebx + 0x38], 4
loc_4_0410:
        mov    dx, word ptr [esi + 0x11]
        add    esp, 0x20
        or     ecx, ecx
        jne    loc_4_04F8
        inc    byte ptr [ebx + 0x4f]
        mov    word ptr [ebx + 0x49], dx
        mov    edi, ebx
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [0x149], ecx        ; -> obj5:0x0149
        or     ecx, ecx
        je     loc_4_04F8
        cmp    dword ptr [0x344], -1         ; -> obj5:0x0344
        je     loc_4_045C
        mov    eax, dword ptr [0x344]        ; -> obj5:0x0344
        mov    dword ptr [ecx + 0x18], eax
        mov    ebx, edx
        push   0
        push   ecx
        push   eax
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a003300
        add    esp, 0x10
        mov    edx, ebx
loc_4_045C:
        mov    ebx, edi
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        call   sub_4_09CB
        test   dword ptr [ebx + 0x38], 0x40
        je     loc_4_0488
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        lea    eax, [0x2207]                 ; -> loc_1_2207
        mov    dword ptr [edi + 8], eax
        mov    esi, edi
        call   sub_4_08A9
loc_4_0488:
        mov    edi, ebx
        mov    ecx, dword ptr [0x145]        ; -> obj5:0x0145
        cmp    byte ptr [ecx + 0x3e], 1
        jne    loc_4_04AF
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        movzx  ecx, byte ptr [ecx + 0x10]
loc_4_04A0:
        push   ecx
        call   sub_4_0538
        pop    ecx
        inc    ch
        dec    cl
        jne    loc_4_04A0
        jmp    loc_4_04B4
loc_4_04AF:
        call   sub_4_068F
loc_4_04B4:
        mov    edi, dword ptr [0x145]        ; -> obj5:0x0145
        mov    esi, dword ptr [0x149]        ; -> obj5:0x0149
        or     esi, esi
        je     loc_4_04F8
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [0x149], ecx        ; -> obj5:0x0149
        test   byte ptr [esi + 0x11], 4
        je     loc_4_04F0
        mov    ecx, esi
        call   sub_4_0E2E
        mov    esi, dword ptr [0x145]        ; -> obj5:0x0145
        dec    byte ptr [esi + 0x4f]
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        and    byte ptr [0x14d], 0xfd        ; -> obj5:0x014D
loc_4_04F0:
        or     ecx, ecx
        jne    loc_4_045C
loc_4_04F8:
        test   byte ptr [0x14d], 2           ; -> obj5:0x014D
        jne    loc_4_03AD
        mov    esi, dword ptr [0x145]        ; -> obj5:0x0145
        cmp    byte ptr [esi + 0x44], 0x14
        jne    loc_4_0526
        cmp    byte ptr [esi + 0x4f], 0
        jne    loc_4_0526
        mov    dword ptr [0x145], 0          ; -> obj5:0x0145
        call   IOS_Service_17
loc_4_0526:
        push   0
        push   dword ptr [0x33e]             ; -> obj5:0x033E
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        ret    
sub_4_0538:
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        sub    esp, 0x10
        mov    esi, esp
        mov    word ptr [esi], 6
        call   sub_4_09CB
        mov    byte ptr [edi + 0x75], ch
        mov    byte ptr [edi + 0x7d], 0
        mov    byte ptr [edi + 0x7c], 0
        mov    byte ptr [0x13e], 0           ; -> obj5:0x013E
        mov    dword ptr [esi + 0xc], edi
        test   byte ptr [0x14d], 1           ; -> obj5:0x014D
        je     loc_4_056E
        mov    byte ptr [edi + 0x7c], dl
loc_4_056E:
        mov    ebx, dword ptr [0x145]        ; -> obj5:0x0145
        mov    cl, byte ptr [ebx + 0x3e]
        mov    byte ptr [edi + 0x74], cl
        mov    word ptr [esi + 2], 1
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        mov    dword ptr [esi + 4], ecx
        call   sub_4_0982
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        push   edx
        mov    dx, word ptr [edi + 0x7c]
        mov    eax, dword ptr [ecx + 0xc]
loc_4_059C:
        or     eax, eax
        je     loc_4_05B3
        cmp    word ptr [eax + 0x7c], dx
        jne    loc_4_05AE
        mov    cl, byte ptr [edi + 0x75]
        cmp    byte ptr [eax + 0x75], cl
        je     loc_4_05B3
loc_4_05AE:
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_059C
loc_4_05B3:
        pop    edx
        cmp    word ptr [esi + 2], 0
        jne    loc_4_0677
        test   byte ptr [edi + 0x82], 0xe0
        jne    loc_4_0677
        or     eax, eax
        je     loc_4_05F4
        push   esi
        push   edi
        lea    esi, [eax + 0x82]
        lea    edi, [edi + 0x82]
        mov    ecx, 9
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    edi
        pop    esi
        je     loc_4_0624
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        call   sub_4_0F01
loc_4_05F4:
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        mov    eax, dword ptr [0x145]        ; -> obj5:0x0145
        mov    al, byte ptr [eax + 0x4f]
        dec    al
        mov    byte ptr [edi + 0x7e], al
        mov    al, byte ptr [0x13e]          ; -> obj5:0x013E
        inc    byte ptr [0x13e]              ; -> obj5:0x013E
        mov    byte ptr [edi + 0xba], al
        push   esi
        call   sub_4_0772
        call   sub_4_08A9
        pop    esi
loc_4_0624:
        mov    ch, byte ptr [edi + 0x7d]
        cmp    ch, dh
        je     loc_4_0646
        mov    cl, byte ptr [edi + 0x7c]
        mov    bl, byte ptr [edi + 0x75]
        call   sub_4_09CB
        mov    byte ptr [edi + 0x75], bl
        mov    byte ptr [edi + 0x7c], cl
        inc    ch
        mov    byte ptr [edi + 0x7d], ch
        jmp    loc_4_056E
loc_4_0646:
        mov    cl, byte ptr [edi + 0x7c]
        test   byte ptr [0x14d], 1           ; -> obj5:0x014D
        je     loc_4_065A
        or     cl, cl
        je     loc_4_068B
        dec    cl
        jmp    loc_4_0660
loc_4_065A:
        cmp    cl, dl
        je     loc_4_068B
        inc    cl
loc_4_0660:
        mov    bl, byte ptr [edi + 0x75]
        call   sub_4_09CB
        mov    byte ptr [edi + 0x75], bl
        mov    byte ptr [edi + 0x7c], cl
        mov    byte ptr [edi + 0x7d], 0
        jmp    loc_4_056E
loc_4_0677:
        cmp    word ptr [esi + 2], 2
        je     loc_4_068B
        or     eax, eax
        je     loc_4_0646
        push   edi
        call   sub_4_0F01
        pop    edi
        jmp    loc_4_0646
loc_4_068B:
        add    esp, 0x10
        ret    
sub_4_068F:
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        sub    esp, 0x10
        mov    esi, esp
        mov    word ptr [esi], 6
        mov    dword ptr [esi + 0xc], edi
        mov    byte ptr [edi + 0xba], 0
loc_4_06A9:
        mov    ebx, dword ptr [0x145]        ; -> obj5:0x0145
        mov    cl, byte ptr [ebx + 0x3e]
        mov    byte ptr [edi + 0x74], cl
        mov    word ptr [esi + 2], 1
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        mov    dword ptr [esi + 4], ecx
        call   sub_4_0982
        cmp    word ptr [esi + 2], 2
        je     loc_4_0764
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        mov    dl, byte ptr [edi + 0xba]
        mov    eax, dword ptr [ecx + 0xc]
loc_4_06E3:
        or     eax, eax
        je     loc_4_06F4
        cmp    byte ptr [eax + 0xba], dl
        je     loc_4_06F4
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_06E3
loc_4_06F4:
        cmp    word ptr [esi + 2], 0
        jne    loc_4_0759
        or     eax, eax
        je     loc_4_071D
        push   esi
        push   edi
        lea    esi, [eax + 0x82]
        lea    edi, [edi + 0x82]
        mov    ecx, 9
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    edi
        pop    esi
        je     loc_4_073C
        call   sub_4_0F01
loc_4_071D:
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        mov    eax, dword ptr [0x145]        ; -> obj5:0x0145
        mov    al, byte ptr [eax + 0x4f]
        dec    al
        mov    byte ptr [edi + 0x7e], al
        push   esi
        call   sub_4_0772
        call   sub_4_08A9
        pop    esi
loc_4_073C:
        mov    dl, byte ptr [edi + 0xba]
        cmp    dl, 0x7f
        je     loc_4_076E
        inc    dl
        call   sub_4_09CB
        mov    byte ptr [edi + 0xba], dl
        jmp    loc_4_06A9
loc_4_0759:
        or     eax, eax
        je     loc_4_073C
        call   sub_4_0F01
        jmp    loc_4_073C
loc_4_0764:
        mov    dword ptr [0x149], 0          ; -> obj5:0x0149
loc_4_076E:
        add    esp, 0x10
        ret    
sub_4_0772:
        enter  4, 0
        pushal 
        mov    eax, dword ptr [0x344]        ; -> obj5:0x0344
        cmp    eax, -1
        je     loc_4_0784
        mov    dword ptr [edi + 0x70], eax
loc_4_0784:
        mov    al, byte ptr [edi + 0x82]
        and    al, 0x1f
        mov    ebx, 0x464                    ; -> obj5:0x0464
        mov    ecx, 0xc
loc_4_0796:
        cmp    al, byte ptr [ebx]
        je     loc_4_07A4
        add    ebx, 0xc
        loop   loc_4_0796
        mov    ebx, 0x4e8                    ; -> obj5:0x04E8
loc_4_07A4:
        sub    esp, 0xc
        mov    esi, esp
        mov    ax, word ptr [ebx + 2]
        mov    word ptr [esi + 4], ax
        mov    word ptr [esi], 1
        push   esi
        call   sub_1_1310
        mov    esi, dword ptr [esi + 6]
        add    esp, 0x10
        push   esi
        push   edi
        xchg   esi, edi
        mov    ecx, 0x12b
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        or     byte ptr [esi + 0x21], 4
        lea    eax, [0x2207]                 ; -> loc_1_2207
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], esi
        test   byte ptr [esi + 0x21], 0x40
        je     loc_4_0819
        mov    al, byte ptr [edi + 0x40]
        mov    byte ptr [esi + 0x40], al
        mov    al, byte ptr [esi + 0xba]
        mov    byte ptr [esi + 0x15], al
        mov    byte ptr [esi + 0xca], al
        sub    esp, 0xc
        mov    ebp, esp
        mov    word ptr [ebp], 6
        mov    byte ptr [ebp + 8], al
        mov    byte ptr [ebp + 9], 0
        mov    dword ptr [ebp + 4], esi
        push   ebp
        call   sub_1_1310
        add    esp, 0x10
        jmp    loc_4_081F
loc_4_0819:
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [esi + 0x40], al
loc_4_081F:
        mov    eax, dword ptr [ebx + 4]
        inc    byte ptr [eax]
        movzx  eax, word ptr [ebx + 8]
        mov    dword ptr [esi + 0xdb], eax
        mov    al, byte ptr [ebx + 0xa]
        mov    byte ptr [esi + 0x3c], al
        mov    al, byte ptr [ebx + 0xb]
        mov    byte ptr [esi + 0xb9], al
        mov    ebx, dword ptr [0x149]        ; -> obj5:0x0149
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x6c], eax
        mov    dword ptr [ebx + 0xc], esi
        test   byte ptr [esi + 0x21], 0x20
        jne    loc_4_08A2
        call   sub_4_2F05
        mov    word ptr [esi + 0x48], 0xffff
        lea    ecx, [ebp - 4]
        sub    esp, 0x18
        mov    eax, esp
        mov    word ptr [eax], 0x11
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [eax + 0xc], 1
        mov    dword ptr [eax + 0x10], 3
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 8], 0x98     ; -> obj5:0x0098
        push   eax
        call   sub_1_1310
        movzx  eax, word ptr [eax + 2]
        add    esp, 0x1c
        or     eax, eax
        jne    loc_4_08A2
        mov    ax, word ptr [ebp - 4]
        sub    ax, 0x4141
        mov    word ptr [esi + 0x48], ax
loc_4_08A2:
        mov    dword ptr [esp + 4], esi
        popal  
        leave  
        ret    
sub_4_08A9:
        pushal 
        sub    esp, 0x10
        mov    edi, esp
        mov    word ptr [edi], 3
        mov    dword ptr [edi + 0xc], esi
        xchg   esi, edi
        mov    ecx, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
loc_4_08BF:
        or     ecx, ecx
        je     loc_4_08EE
        test   dword ptr [ecx + 0x38], 0x60000
        je     loc_4_08E9
        mov    edx, dword ptr [ecx + 0x10]
        mov    dword ptr [esi + 4], edx
        push   dword ptr [0x145]             ; -> obj5:0x0145
        mov    dword ptr [0x145], ecx        ; -> obj5:0x0145
        call   sub_4_0982
        pop    dword ptr [0x145]             ; -> obj5:0x0145
loc_4_08E9:
        mov    ecx, dword ptr [ecx + 2]
        jmp    loc_4_08BF
loc_4_08EE:
        mov    ebx, dword ptr [0x149]        ; -> obj5:0x0149
        mov    dword ptr [esi + 4], ebx
        call   sub_4_0982
        add    esp, 0x10
        test   byte ptr [edi + 0x20], 0x10
        je     loc_4_0939
        sub    esp, 0x18
        mov    ecx, esp
        mov    word ptr [ecx], 5
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [ecx + 0x12], eax
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [ecx + 8], 0xa8     ; -> loc_1_00A8
        mov    byte ptr [ecx + 0x16], 0x12
        mov    dword ptr [ecx + 0xc], 0
        mov    word ptr [ecx + 0x10], 0
        push   ecx
        call   sub_1_1310
        add    esp, 0x1c
loc_4_0939:
        sub    esp, 0x18
        mov    ecx, esp
        mov    word ptr [ecx], 0x11
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [ecx + 0xc], 3
        mov    dword ptr [ecx + 0x10], 1
        mov    dword ptr [ecx + 0x14], 0x143 ; -> obj5:0x0143
        mov    dword ptr [ecx + 8], 0x60     ; -> obj5:0x0060
        push   ecx
        call   sub_1_1310
        movzx  ecx, word ptr [ecx + 2]
        add    esp, 0x1c
        or     ecx, ecx
        jne    loc_4_0980
        cmp    byte ptr [0x143], 0           ; -> obj5:0x0143
        je     loc_4_0980
        or     byte ptr [edi + 0x20], 4
loc_4_0980:
        popal  
        ret    
sub_4_0982:
        pushal 
        mov    ebx, dword ptr [0x145]        ; -> obj5:0x0145
        mov    cl, byte ptr [ebx + 0x44]
        mov    byte ptr [esi + 8], cl
        cmp    dword ptr [ebx + 8], 0
        je     loc_4_099C
        push   esi
        call   dword ptr [ebx + 8]
        add    esp, 4
loc_4_099C:
        popal  
        ret    
        db  060h, 08Dh, 00Dh
        dd  obj1:0x1FA4              ; fixup
        db  039h, 001h, 074h, 00Bh, 08Bh, 009h, 00Bh, 0C9h, 074h, 019h, 08Dh, 049h, 002h, 0EBh, 0F1h, 08Bh
        db  050h, 002h, 089h, 011h, 06Ah, 000h, 050h, 0CDh, 020h, 051h, 000h, 001h, 000h, 083h, 0C4h, 008h
        db  061h, 0F8h, 0C3h, 061h, 0F9h, 0C3h
sub_4_09CB:
        push   ebx
        push   ecx
        mov    al, byte ptr [edi + 0x77]
        push   eax
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 0x1c]
        push   dword ptr [edi + 0x50]
        push   dword ptr [edi + 4]
        push   edi
        xor    al, al
        mov    ecx, 0x12b
        rep stosb byte ptr es:[edi], al
        pop    edi
        mov    word ptr [edi + 0x3e], 0x4342
        pop    dword ptr [edi + 4]
        pop    dword ptr [edi + 0x50]
        pop    dword ptr [edi + 0x1c]
        pop    dword ptr [edi + 8]
        pop    eax
        mov    byte ptr [edi + 0x77], al
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 0x20], 0x8000
        pop    ecx
        pop    ebx
        ret    
IOS_Service_17:
        pushal 
loc_4_0A0B:
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        je     loc_4_0A1B
        push   esi
        call   sub_4_0E2E
        pop    esi
        jmp    loc_4_0A0B
loc_4_0A1B:
        lea    ecx, [0x1fa4]                 ; -> obj1:0x1FA4
loc_4_0A21:
        cmp    dword ptr [ecx], esi
        je     loc_4_0A2C
        mov    ecx, dword ptr [ecx]
        lea    ecx, [ecx + 2]
        jmp    loc_4_0A21
loc_4_0A2C:
        mov    edi, dword ptr [esi + 2]
        mov    dword ptr [ecx], edi
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        popal  
        ret    
loc_4_0A3F:
        pushal 
        mov    esi, 0x4f4                    ; -> obj5:0x04F4
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_0A50:
        ror    byte ptr [edx], 1
        add    byte ptr [eax], al
        ror    byte ptr [edx], 1
        add    byte ptr [eax], al
        aam    0xa
        add    byte ptr [eax], al
        aam    0xa
        add    byte ptr [eax], al
        int3   
        or     al, byte ptr [eax]
        add    ah, cl                        ; -> loc_4_0ACC
        or     al, byte ptr [eax]
        add    ah, dl                        ; -> loc_4_0AD4
        or     al, byte ptr [eax]
        add    ah, dl                        ; -> loc_4_0AD4
        or     al, byte ptr [eax]
        add    dh, dl                        ; -> loc_4_0AD6
        or     al, byte ptr [eax]
        add    ah, dl                        ; -> loc_4_0AD4
        or     al, byte ptr [eax]
        add    ah, dl                        ; -> loc_4_0AD4
        or     al, byte ptr [eax]
        add    ah, dl                        ; -> loc_4_0AD4
        or     al, byte ptr [eax]
        add    ah, dl                        ; -> loc_4_0AD4
        or     al, byte ptr [eax]
        add    byte ptr [edi + 0xb], bl      ; -> loc_4_0B5F
        add    byte ptr [eax], al
        add    cl, byte ptr [ebx]
        add    byte ptr [eax], al
        and    cl, byte ptr [ebx]
        add    byte ptr [eax], al
        aam    0xa
        add    byte ptr [eax], al
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        or     al, 0
        add    bh, al
        xchg   byte ptr [eax + 0x1000000], ah
        add    byte ptr [eax], al
        add    byte ptr [eax - 0x75], dl
        xchg   byte ptr [edx - 0x80000000], dl
        cld    
        inc    esp
        jne    loc_4_0AC5
        and    eax, 0xff
        cmp    eax, 0x11
        ja     loc_4_0AC5
        mov    eax, dword ptr [eax*4 + 0xa50] ; -> loc_4_0A50
loc_4_0AC1:
        call   eax
        pop    eax
        ret    
loc_4_0AC5:
        mov    eax, 0xad0                    ; -> loc_4_0AD0
        jmp    loc_4_0AC1
loc_4_0ACC:
        or     byte ptr [esi + 0x6d], 0x40
loc_4_0AD0:
        or     byte ptr [esi + 0x6e], 1
loc_4_0AD4:
        stc    
        ret    
loc_4_0AD6:
        xor    eax, eax
        or     dword ptr [esi + 0x6c], 0x14008
        test   dword ptr [ebx + 0x20], 0x2000
        stc    
        jne    loc_4_0B01
        test   dword ptr [ebx + 0x20], 4
        jne    loc_4_0AF4
        inc    al
loc_4_0AF4:
        mov    dword ptr [esi + 0xa0], eax
        mov    word ptr [esi + 0x6a], 0
        clc    
loc_4_0B01:
        ret    
loc_4_0B02:
        xor    eax, eax
        test   byte ptr [ebx + 0x1d], 0x20
        je     loc_4_0B14
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0xae]
        inc    al
loc_4_0B14:
        mov    dword ptr [esi + 0xa0], eax
        mov    word ptr [esi + 0x6a], 0
        clc    
        ret    
loc_4_0B22:
        mov    eax, dword ptr [ebx]
        xor    ecx, ecx
        test   byte ptr [eax + 0x1d], 0x20
        je     loc_4_0B3D
        movzx  ecx, byte ptr [esi + 0x90]
        dec    cl
        mov    byte ptr [eax + 0xae], cl
        inc    cl
loc_4_0B3D:
        mov    dword ptr [esi + 0xa0], ecx
        mov    word ptr [esi + 0x6a], 0
        clc    
        ret    
loc_4_0B4B:
        inc    dword ptr [eax]
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh
loc_4_0B55:
        inc    dword ptr [eax]
        add    bh, bh
        db  0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh
loc_4_0B5F:
        movzx  eax, byte ptr [esi + 0x94]
        cmp    al, 0x67
        je     loc_4_0BB8
        cmp    al, 0x47
        je     loc_4_0BD7
        cmp    al, 0x6f
        je     loc_4_0BE8
        cmp    al, 0x40
        jb     loc_4_0B9E
loc_4_0B76:
        cmp    al, 0x60
        je     loc_4_0BA0
        lea    ecx, [0xb55]                  ; -> loc_4_0B55
        ja     loc_4_0B88
        lea    ecx, [0xb4b]                  ; -> loc_4_0B4B
loc_4_0B88:
        and    al, 0x9f
        cmp    al, 0xa
        ja     loc_4_0ACC
        movsx  eax, byte ptr [ecx + eax]
        and    eax, 0x14000
        or     dword ptr [esi + 0x6c], eax
loc_4_0B9E:
        stc    
        ret    
loc_4_0BA0:
        mov    eax, dword ptr [esi + 0x98]
        test   byte ptr [eax], 1
        jne    loc_4_0B9E
        or     byte ptr [esi + 0x6e], 1
        or     dword ptr [esi + 0x6c], 0x4000
        stc    
        ret    
loc_4_0BB8:
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_0BD7
        mov    al, 1
        mov    ecx, dword ptr [esi + 0x98]
        cmp    byte ptr [ebx + 0x40], 0xa
        je     loc_4_0BD4
        test   byte ptr [ebx + 0x16], 1
        je     loc_4_0BD4
        xor    al, al
loc_4_0BD4:
        mov    byte ptr [ecx + 1], al
loc_4_0BD7:
        or     byte ptr [esi + 0x6e], 1
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_0B76
        xor    eax, eax
        jmp    loc_4_0AF4
loc_4_0BE8:
        mov    ecx, dword ptr [esi + 0x98]
        mov    ebx, dword ptr [esi + 8]
        movzx  eax, byte ptr [ecx]
        mov    ah, 0x10
        cmp    ah, al
        ja     loc_4_0C95
        ror    eax, 0x10
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_0C64
        mov    edi, dword ptr [esi + 4]
        mov    edx, dword ptr [ebx + 0x28]
        test   byte ptr [ebx + 0x1e], 2
        je     loc_4_0C15
        or     al, 8
loc_4_0C15:
        or     al, 3
        mov    ah, 0xff
        test   byte ptr [edi + 0x20], 1
        je     loc_4_0C22
        mov    ah, byte ptr [edi + 0x15]
loc_4_0C22:
        ror    eax, 0x10
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0xc], 0
        movzx  eax, byte ptr [esi + 0xb1]
        sub    esp, 8
        mov    byte ptr [esp], 6
        mov    byte ptr [esp + 2], al
        push   esi
        lea    esi, [esp + 4]
        call   sub_4_193B
        mov    eax, dword ptr [esi + 3]
        pop    esi
        mov    ecx, dword ptr [esi + 0x98]
        mov    dword ptr [ecx + 4], eax
        add    esp, 8
        xor    eax, eax
        jmp    loc_4_0AF4
loc_4_0C64:
        push   esi
        push   ecx
        push   eax
        movzx  eax, byte ptr [esi + 0xb1]
        call   sub_4_1BC0
        mov    edx, esi
        pop    eax
        pop    ecx
        pop    esi
        mov    ah, byte ptr [edx + 1]
        push   eax
        push   ebx
        movzx  eax, ah
        or     eax, 0x40000000
        call   sub_1_128D
        pop    ebx
        pop    eax
        jb     loc_4_0C90
        or     al, 2
loc_4_0C90:
        mov    edx, dword ptr [edx + 6]
        jmp    loc_4_0C22
loc_4_0C95:
        mov    dword ptr [esi + 0xa0], 0x16
        mov    word ptr [esi + 0x6a], 0x10
        clc    
        ret    
loc_4_0CA7:
        mov    eax, dword ptr [esi + 0x94]
        cmp    ah, 8
        jne    loc_4_0CBE
        cmp    al, 0x67
        je     loc_4_0CD0
        cmp    al, 0x47
        je     loc_4_0CD0
        cmp    al, 0x6f
        je     loc_4_0CDA
loc_4_0CBE:
        or     word ptr [esi + 0x6d], 0x140
        mov    dword ptr [esi + 0xa0], 0x16
        stc    
        ret    
loc_4_0CD0:
        or     byte ptr [esi + 0x6e], 1
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_0CBE
loc_4_0CDA:
        xor    eax, eax
        jmp    loc_4_0AF4
        db  000h, 000h, 000h
loc_4_0CE4:
        test   byte ptr [edi + 0xa], 1
        jne    loc_4_0D54
        movzx  eax, word ptr [edi + 4]
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0D54
        mov    ebx, eax
loc_4_0D00:
        movzx  ecx, word ptr [edi + 4]
        mov    dword ptr [edi + 6], ebx
        sub    ecx, 4
        mov    edi, ebx
        add    edi, 4
        xor    ax, ax
        rep stosb byte ptr es:[edi], al
loc_4_0D14:
        mov    word ptr [ebx + 0x12], 0x4442
        mov    edi, dword ptr [0x145]        ; -> obj5:0x0145
        mov    dword ptr [ebx + 0x14], edi
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_4_0D38
loc_4_0D2A:
        mov    esi, eax
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        jne    loc_4_0D2A
        mov    dword ptr [esi + 4], ebx
        jmp    loc_4_0D3B
loc_4_0D38:
        mov    dword ptr [edi + 0xc], ebx
loc_4_0D3B:
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_4_0D50
loc_4_0D42:
        mov    esi, eax
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        jne    loc_4_0D42
        mov    dword ptr [esi + 8], ebx
        jmp    loc_4_0D53
loc_4_0D50:
        mov    dword ptr [edi + 0x10], ebx
loc_4_0D53:
        ret    
loc_4_0D54:
        movzx  esi, word ptr [edi + 4]
        cmp    esi, 0x7d0
        ja     loc_4_0D6E
loc_4_0D60:
        mov    ecx, 5
        xor    eax, eax
        call   sub_1_072B
        jmp    loc_4_0D00
loc_4_0D6E:
        shr    esi, 0xc
        inc    esi
        push   0xf
        lea    eax, [edi + 6]
        push   eax
        push   0xfff
        push   0
        push   0
        push   0
        push   1
        push   esi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        movzx  esi, word ptr [edi + 4]
        or     eax, edx
        je     loc_4_0D60
        or     byte ptr [edx + 0x11], 2
        mov    ebx, edx
        xchg   dword ptr [edi + 6], edx
        mov    dword ptr [ebx], edx
        jmp    loc_4_0D14
loc_4_0DA7:
        movzx  eax, word ptr [edi + 4]
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        mov    ebx, eax
        jne    loc_4_0DD7
        movzx  esi, word ptr [edi + 4]
        mov    ecx, 4
        xor    eax, eax
        call   sub_1_072B
        movzx  ecx, word ptr [edi + 4]
        mov    edi, ebx
        xor    eax, eax
        rep stosb byte ptr es:[edi], al
loc_4_0DD7:
        mov    dword ptr [edi + 6], ebx
        mov    word ptr [ebx + 0x3e], 0x4342
        or     byte ptr [ebx + 0x21], 4
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edi, dword ptr [edi + 0x30]
        or     edi, edi
        je     loc_4_0DFC
loc_4_0DF1:
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_4_0E07
        mov    edi, eax
        jmp    loc_4_0DF1
loc_4_0DFC:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [ecx + 0x30], ebx
        jmp    loc_4_0E0A
loc_4_0E07:
        mov    dword ptr [edi + 0xc], ebx
loc_4_0E0A:
        mov    dword ptr [ebx + 0xc], 0
        or     edi, edi
        je     loc_4_0E1E
loc_4_0E15:
        mov    edi, dword ptr [edi + 0xc]
        or     edi, edi
        je     loc_4_0E1E
        jmp    loc_4_0E15
loc_4_0E1E:
        mov    edi, dword ptr [0x145]        ; -> obj5:0x0145
        inc    word ptr [edi + 6]
        ret    
loc_4_0E29:
        mov    ecx, dword ptr [edi + 4]
        jmp    loc_4_0E30
sub_4_0E2E:
        xor    edi, edi
loc_4_0E30:
        mov    esi, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
loc_4_0E36:
        lea    edx, [esi + 0xc]
loc_4_0E39:
        cmp    dword ptr [edx], ecx
        je     loc_4_0E4D
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     loc_4_0E48
        lea    edx, [edx + 4]
        jmp    loc_4_0E39
loc_4_0E48:
        mov    esi, dword ptr [esi + 2]
        jmp    loc_4_0E36
loc_4_0E4D:
        mov    dword ptr [0x145], esi        ; -> obj5:0x0145
        push   edx
        mov    eax, dword ptr [ecx + 0xc]
loc_4_0E57:
        or     eax, eax
        je     loc_4_0E64
        or     byte ptr [eax + 0x23], 4
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_0E57
loc_4_0E64:
        mov    eax, dword ptr [ecx + 0xc]
loc_4_0E67:
        or     eax, eax
        je     loc_4_0E7E
        mov    dword ptr [eax + 0x70], 0
        mov    edx, dword ptr [eax + 0x6c]
        call   sub_4_0F01
        mov    eax, edx
        jmp    loc_4_0E67
loc_4_0E7E:
        pop    edx
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx], edi
        sub    esp, 0xc
        mov    esi, esp
        mov    word ptr [esi], 0xf
        mov    dword ptr [esi + 4], ecx
        call   sub_4_0982
        add    esp, 0xc
        mov    eax, dword ptr [ecx + 0x18]
        or     eax, eax
        je     loc_4_0EB1
        push   ecx
        push   0
        push   0
        push   eax
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a003300
        add    esp, 0x10
        pop    ecx
loc_4_0EB1:
        test   byte ptr [ecx + 0x11], 2
        jne    loc_4_0EE2
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    ecx, eax
        jb     loc_4_0ED5
        add    eax, dword ptr [eax + 0x60]
        add    eax, 0x6e
        cmp    ecx, eax
        ja     loc_4_0ED5
        push   edi
        mov    edi, ecx
        call   sub_1_0A1E
        pop    edi
        jmp    loc_4_0EE1
loc_4_0ED5:
        push   0
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0EE1:
        ret    
loc_4_0EE2:
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        jmp    loc_4_0EE1
        db  00Bh, 0FFh, 074h, 0EDh, 066h, 0C7h, 047h, 002h, 0FFh, 0FFh, 0EBh, 0E5h
loc_4_0EFC:
        mov    eax, dword ptr [edi + 4]
        jmp    loc_4_0F03
sub_4_0F01:
        xor    edi, edi
loc_4_0F03:
        push   ecx
        push   edx
        push   eax
        push   ebp
        mov    ecx, edi
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [edi], 0x15
        call   sub_1_175A
        mov    eax, dword ptr [edi + 0xc]
        add    esp, 0x10
        mov    edi, ecx
        call   sub_1_17D8
        sub    esp, 0x10
        mov    ebp, esp
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp], 4
        push   esi
        test   byte ptr [eax + 0x21], 0x40
        jne    loc_4_0F54
        mov    esi, dword ptr [eax + 0x10]
        mov    dword ptr [eax + 0x10], 0
loc_4_0F47:
        or     esi, esi
        je     loc_4_0F6A
        push   eax
        call   sub_4_0F99
        pop    eax
        jmp    loc_4_0F47
loc_4_0F54:
        mov    ebx, dword ptr [eax]
loc_4_0F56:
        or     ebx, ebx
        je     loc_4_0F6A
        cmp    dword ptr [ebx + 0x10], eax
        je     loc_4_0F64
        mov    ebx, dword ptr [ebx + 0x10]
        jmp    loc_4_0F56
loc_4_0F64:
        mov    esi, dword ptr [eax + 0x10]
        mov    dword ptr [ebx + 0x10], esi
loc_4_0F6A:
        mov    esi, eax
        mov    eax, dword ptr [esi + 0x70]
        or     eax, eax
        je     loc_4_0F7F
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d003300
        add    esp, 8
loc_4_0F7F:
        call   sub_4_0F99
        pop    esi
        add    esp, 0x10
        pop    ebp
        pop    eax
        pop    edx
        pop    ecx
        ret    
        db  00Bh, 0FFh, 074h, 0F7h, 066h, 0C7h, 047h, 002h, 0FFh, 0FFh, 0EBh, 0EFh
sub_4_0F99:
        or     byte ptr [esi + 0x23], 4
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [edx + 0x28], 1
        je     loc_4_0FFA
        test   byte ptr [esi + 0x21], 0x80
        je     loc_4_0FCF
        test   byte ptr [esi + 0x20], 1
        je     loc_4_0FCF
        mov    eax, dword ptr [edx + 0x38]
loc_4_0FB8:
        mov    dl, byte ptr [eax]
        cmp    dl, 0xff
        je     loc_4_0FCF
        cmp    byte ptr [esi + 0x15], dl
        jne    loc_4_0FCA
        and    byte ptr [eax + 9], 0xfd
        jmp    loc_4_0FCF
loc_4_0FCA:
        add    eax, 0xb
        jmp    loc_4_0FB8
loc_4_0FCF:
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_0FFA
        movzx  eax, byte ptr [esi + 0x14]
        push   esi
        call   sub_4_1BC0
        mov    edi, esi
        pop    esi
        jb     loc_4_0FF0
        btr    word ptr [edi + 0x10], 0
loc_4_0FF0:
        push   eax
        VxDCall  IFSMgr.NotifyVolumeRemoval      ; cd204e004000
        add    esp, 4
loc_4_0FFA:
        xor    edi, edi
        call   sub_4_293A
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        lea    ebx, [edx + 0x40]
        sub    ebx, 0x24
        mov    edx, dword ptr [edx + 0x40]
loc_4_1010:
        or     edx, edx
        je     loc_4_1036
        cmp    dword ptr [edx + 0x28], esi
        je     loc_4_1020
        mov    ebx, edx
        mov    edx, dword ptr [edx + 0x24]
        jmp    loc_4_1010
loc_4_1020:
        mov    edi, dword ptr [edx + 0x24]
        mov    dword ptr [ebx + 0x24], edi
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    edx, edi
        jmp    loc_4_1010
loc_4_1036:
        mov    dword ptr [ebp + 0xc], esi
        mov    edi, ebp
        call   sub_1_175A
        push   dword ptr [esi + 0x10]
        call   sub_4_105B
        call   sub_4_1082
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    esi
        ret    
sub_4_105B:
        mov    edi, dword ptr [esi + 8]
loc_4_105E:
        or     edi, edi
        je     loc_4_1081
        mov    edx, dword ptr [edi + 0xc]
        cmp    edi, 0x2207                   ; -> loc_1_2207
        je     loc_4_107D
        xchg   edx, edi
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    edx, edi
loc_4_107D:
        mov    edi, edx
        jmp    loc_4_105E
loc_4_1081:
        ret    
sub_4_1082:
        push   ebp
        mov    ebp, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
loc_4_1089:
        or     ebp, ebp
        je     loc_4_10BC
        mov    ecx, dword ptr [ebp + 0xc]
loc_4_1090:
        or     ecx, ecx
        je     loc_4_10B1
        lea    ebx, [ecx + 0xc]
        sub    ebx, 0x6c
        mov    edx, dword ptr [ebx + 0x6c]
loc_4_109D:
        or     edx, edx
        je     loc_4_10AC
        cmp    edx, esi
        je     loc_4_10B6
        mov    ebx, edx
        mov    edx, dword ptr [edx + 0x6c]
        jmp    loc_4_109D
loc_4_10AC:
        mov    ecx, dword ptr [ecx + 4]
        jmp    loc_4_1090
loc_4_10B1:
        mov    ebp, dword ptr [ebp + 2]
        jmp    loc_4_1089
loc_4_10B6:
        mov    edi, dword ptr [edx + 0x6c]
        mov    dword ptr [ebx + 0x6c], edi
loc_4_10BC:
        pop    ebp
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        add    ebx, 0x24
        mov    edx, dword ptr [ebx + 0xc]
loc_4_10C9:
        or     edx, edx
        je     loc_4_112C
        cmp    edx, esi
        je     loc_4_10D8
        mov    ebx, edx
        mov    edx, dword ptr [edx + 0xc]
        jmp    loc_4_10C9
loc_4_10D8:
        mov    edi, dword ptr [edx + 0xc]
        mov    dword ptr [ebx + 0xc], edi
        test   byte ptr [esi + 0x20], 1
        je     loc_4_10F8
        movzx  edx, byte ptr [esi + 0x15]
        sub    dl, 0x80
        jb     loc_4_10F8
        mov    dword ptr [edx*4 + 0x2114], 0 ; -> obj1:0x2114
loc_4_10F8:
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_111A
        dec    byte ptr [esi + 0x46]
        movzx  edx, byte ptr [esi + 0x14]
        mov    dword ptr [edx*4 + 0x20ac], 0 ; -> obj1:0x20AC
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        btr    dword ptr [ebx + 0x44], edx
loc_4_111A:
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    dword ptr [edx + 0x3c], esi
        jne    loc_4_112C
        mov    dword ptr [edx + 0x3c], 0
loc_4_112C:
        ret    
loc_4_112D:
        push   ebp
        push   ebx
        push   esi
        mov    eax, dword ptr [edi + 4]
        mov    word ptr [edi + 2], 0xffff
        test   byte ptr [eax + 0x21], 0x40
        jne    loc_4_116C
        sub    esp, 0x14
        mov    ebp, esp
        mov    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp + 0x10], 0
        mov    word ptr [ebp], 0xc
        push   edi
        mov    edi, ebp
        call   sub_1_175A
        pop    edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edi + 8], eax
        mov    word ptr [edi + 2], 0
        add    esp, 0x14
loc_4_116C:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_4_1170:
        push   1
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_11ED
        mov    ebx, dword ptr [edi + 8]
        mov    dword ptr [eax], ebx
        mov    ebx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [edi + 0x12]
        mov    dword ptr [eax + 4], ebx
        mov    ecx, dword ptr [edi + 4]
        mov    ebx, dword ptr [ecx + 4]
        add    bx, word ptr [edi + 0x10]
        add    ebx, 3
        and    bl, 0xfc
        mov    dword ptr [ecx + 4], ebx
        neg    ebx
        mov    word ptr [eax + 0x10], bx
        mov    bl, byte ptr [edi + 0x16]
        mov    byte ptr [eax + 0x13], bl
        lea    edx, [ecx + 8]
        mov    esi, dword ptr [edx]
loc_4_11B6:
        or     esi, esi
        je     loc_4_11DC
        test   byte ptr [esi + 6], 0x80
        jne    loc_4_11CC
loc_4_11C0:
        cmp    byte ptr [esi + 0x13], bl
        jae    loc_4_11DC
        lea    edx, [esi + 0xc]
        mov    esi, dword ptr [edx]
        jmp    loc_4_11B6
loc_4_11CC:
        push   ebx
        mov    ebx, dword ptr [eax + 4]
        or     ebx, 0x800000
        mov    dword ptr [esi + 4], ebx
        pop    ebx
        jmp    loc_4_11C0
loc_4_11DC:
        mov    esi, dword ptr [edx]
        mov    dword ptr [eax + 0xc], esi
        mov    dword ptr [edx], eax
        mov    ebx, dword ptr [ecx + 8]
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 0x1c], ebx
        ret    
loc_4_11ED:
        mov    word ptr [edi + 2], 1
        ret    
loc_4_11F4:
        movzx  ebx, byte ptr [edi + 8]
        mov    eax, dword ptr [edi + 4]
        test   byte ptr [eax + 0x21], 0x20
        jne    loc_4_127C
        cmp    byte ptr [eax + 0x40], 0xa
        je     loc_4_1292
loc_4_120B:
        cmp    bl, 0xff
        je     loc_4_12A5
        test   bl, 0x80
        je     loc_4_122D
        sub    bl, 0x80
        cmp    bl, 0x18
        ja     loc_4_12A6
        mov    dword ptr [ebx*4 + 0x2114], eax ; -> obj1:0x2114
        ret    
loc_4_122D:
        cmp    bl, 0x1a
        jae    loc_4_12A6
        cmp    byte ptr [eax + 0x46], 0
        jne    loc_4_123B
        mov    byte ptr [eax + 0x14], bl
loc_4_123B:
        inc    byte ptr [eax + 0x46]
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        bts    dword ptr [ecx + 0x44], ebx
        xchg   dword ptr [ebx*4 + 0x20ac], eax ; -> obj1:0x20AC
        or     eax, eax
        je     loc_4_1256
        dec    byte ptr [eax + 0x46]
loc_4_1256:
        test   byte ptr [ecx + 0x29], 4
        jne    loc_4_127B
        push   ebx
        VxDCall  IFSMgr.NotifyVolumeArrival      ; cd204d004000
        pop    ebx
        test   byte ptr [edi + 9], 1
        jne    loc_4_127B
        push   0
        push   ebx
        push   0x1355                        ; -> loc_4_1355
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
loc_4_127B:
        ret    
loc_4_127C:
        test   byte ptr [eax + 0x21], 0x80
        je     loc_4_120B
        test   byte ptr [eax + 0x21], 0x40
        jne    loc_4_120B
        mov    ebx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [ebx + 0x3c], eax
        ret    
loc_4_1292:
        test   byte ptr [eax + 0x21], 0x40
        jne    loc_4_122D
        cmp    bl, 8
        ja     loc_4_12A6
        mov    dword ptr [ebx*4 + 0x2174], eax ; -> obj1:0x2174
        ret    
loc_4_12A5:
        ret    
loc_4_12A6:
        mov    word ptr [edi + 2], 1
        ret    
loc_4_12AD:
        mov    eax, dword ptr [edi + 4]
        call   sub_4_2819
        jae    loc_4_12BD
        mov    word ptr [edi + 2], 0xffff
loc_4_12BD:
        ret    
loc_4_12BE:
        mov    edx, dword ptr [edi + 4]
        cmp    edx, 0x1a
        jae    loc_4_130C
        xor    eax, eax
        xchg   dword ptr [edx*4 + 0x20ac], eax ; -> obj1:0x20AC
        or     eax, eax
        je     loc_4_130B
        dec    byte ptr [eax + 0x46]
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        btr    dword ptr [eax + 0x44], edx
        mov    eax, dword ptr [eax + 0x28]
        xor    al, 1
        test   eax, 0x4001
        jne    loc_4_130B
        push   ecx
        push   edx
        VxDCall  IFSMgr.NotifyVolumeRemoval      ; cd204e004000
        add    esp, 4
        mov    edx, dword ptr [edi + 4]
        push   0
        push   edx
        push   0x137e                        ; -> loc_4_137E
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        pop    ecx
loc_4_130B:
        ret    
loc_4_130C:
        mov    word ptr [edi + 2], 1
        ret    
loc_4_1313:
        mov    esi, dword ptr [edi + 4]
        mov    edi, dword ptr [edi + 8]
        and    edi, 1
loc_4_131C:
        call   sub_4_293A
        mov    esi, dword ptr [esi + 0x10]
        or     esi, esi
        jne    loc_4_131C
        ret    
loc_4_1329:
        mov    esi, dword ptr [edi + 4]
loc_4_132C:
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_134D
        push   edi
        mov    edi, dword ptr [edi + 8]
        and    edi, 1
        mov    edx, 0
        mov    eax, 0x8000
        movzx  ebx, byte ptr [esi + 0x14]
        call   sub_4_28F9
        pop    edi
loc_4_134D:
        mov    esi, dword ptr [esi + 0x10]
        or     esi, esi
        jne    loc_4_132C
        ret    
loc_4_1355:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 0x10]
        call   sub_1_128D
        jb     loc_4_137A
        mov    esi, ebx
        xor    edi, edi
        mov    edx, 0
        mov    eax, 0x8000
        mov    ebx, dword ptr [esp + 0x10]
        call   sub_4_28F9
loc_4_137A:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_4_137E:
        push   esi
        push   edi
        push   ebx
        xor    edi, edi
        mov    edx, edi
        mov    eax, 0x8004
        mov    ebx, dword ptr [esp + 0x10]
        call   sub_4_28F9
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_4_1397:
        sub    esp, 4
        test   byte ptr [edi + 0xa], 0x10
        jne    loc_4_1475
        mov    ebx, dword ptr [edi + 4]
        test   byte ptr [edi + 0xa], 8
        jne    loc_4_1403
        mov    esi, dword ptr [ebx + 0x70]
        or     esi, esi
        je     loc_4_1403
        mov    ax, word ptr [ebx + 0x48]
        cmp    ax, -1
        je     loc_4_1403
loc_4_13BE:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        push   ebx
        movzx  ebx, al
        bts    dword ptr [ecx + 0x44], ebx
        pop    ebx
        jae    loc_4_144E
        test   byte ptr [edi + 0xa], 4
        je     loc_4_13E4
        push   ebx
        movzx  ebx, al
        bt     dword ptr [ecx + 0x48], ebx
        pop    ebx
        jb     loc_4_146E
loc_4_13E4:
        test   byte ptr [edi + 0xa], 2
        je     loc_4_13FB
        push   esi
        call   sub_4_1BC0
        mov    ecx, esi
        pop    esi
        jb     loc_4_13FB
        test   byte ptr [ecx + 0x10], 2
        jne    loc_4_146E
loc_4_13FB:
        inc    al
        je     loc_4_1403
        cmp    al, ah
        jbe    loc_4_13BE
loc_4_1403:
        test   byte ptr [edi + 0xa], 1
        jne    loc_4_1446
        xor    eax, eax
        movzx  ebx, byte ptr [0x342]         ; -> obj5:0x0342
        test   byte ptr [edi + 0xa], 8
        je     loc_4_1428
        movzx  eax, byte ptr [edi + 8]
        dec    eax
        cmp    byte ptr [edi + 9], bl
        jae    loc_4_1441
        movzx  ebx, byte ptr [edi + 9]
        jmp    loc_4_1441
loc_4_1428:
        mov    ecx, dword ptr [eax*4 + 0xbe] ; -> obj5:0x00BE
        test   byte ptr [ecx + 0x44], 0x40
        jne    loc_4_1441
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        bts    dword ptr [edx + 0x44], eax
        jae    loc_4_146E
loc_4_1441:
        inc    eax
        cmp    eax, ebx
        jbe    loc_4_1428
loc_4_1446:
        mov    word ptr [edi + 2], 0xffff
        jmp    loc_4_1471
loc_4_144E:
        movzx  eax, al
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        btr    dword ptr [ecx + 0x44], eax
        push   eax
        mov    eax, dword ptr [eax*4 + 0xbe] ; -> obj5:0x00BE
        test   byte ptr [eax + 0x44], 0x40
        pop    eax
        jne    loc_4_1403
        bts    dword ptr [ecx + 0x44], eax
loc_4_146E:
        mov    byte ptr [edi + 8], al
loc_4_1471:
        add    esp, 4
        ret    
loc_4_1475:
        movzx  edx, byte ptr [edi + 8]
        cmp    byte ptr [0x342], dl          ; -> obj5:0x0342
        jb     loc_4_1446
        cmp    dword ptr [edx*4 + 0x20ac], 0 ; -> obj1:0x20AC
        jne    loc_4_1446
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        btr    dword ptr [eax + 0x44], edx
        jae    loc_4_1446
        jmp    loc_4_1471
loc_4_1498:
        mov    eax, dword ptr [edi + 4]
        cmp    eax, 0x1a
        cmc    
        jb     loc_4_14A6
        call   sub_4_1BC0
loc_4_14A6:
        sbb    eax, eax
        mov    dword ptr [edi + 8], esi
        mov    word ptr [edi + 2], ax
        ret    
loc_4_14B0:
        mov    eax, dword ptr [edi + 4]
        cmp    eax, 0x1a
        cmc    
        jb     loc_4_14DC
        call   sub_4_1BC0
        jb     loc_4_14DC
        mov    eax, edi
        mov    edi, esi
loc_4_14C4:
        add    esi, 0x12
        cmp    byte ptr [esi], 0xff
        jne    loc_4_14C4
        mov    ecx, esi
        sub    ecx, edi
        sub    ecx, 0x11
        lea    esi, [edi + 0x12]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, eax
        clc    
loc_4_14DC:
        sbb    eax, eax
        mov    word ptr [edi + 2], ax
        ret    
loc_4_14E3:
        mov    edi, dword ptr [edi + 4]
        call   sub_1_175A
        ret    
sub_4_14EC:
        push   ebp
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    esi, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
        sub    esp, 0xc
        mov    edi, esp
        mov    word ptr [edi], 2
        mov    dword ptr [edi + 4], 0
loc_4_150D:
        or     esi, esi
        je     loc_4_1553
        cmp    dword ptr [esi + 8], 0
        je     loc_4_154E
        pushal 
        push   edi
        call   dword ptr [esi + 8]
        add    esp, 4
        popal  
        cmp    word ptr [edi + 2], 0
        je     loc_4_154E
        mov    dword ptr [esi + 8], 0
        mov    edx, dword ptr [esi + 0x45]
        or     edx, edx
        je     loc_4_1542
        mov    bx, 0xffff
        mov    edx, dword ptr [edx + 0xb]
        VxDCall  APM.Service_0002h               ; cd2002002700
loc_4_1542:
        mov    ebx, dword ptr [esi + 2]
        call   IOS_Service_17
        mov    esi, ebx
        jmp    loc_4_150D
loc_4_154E:
        mov    esi, dword ptr [esi + 2]
        jmp    loc_4_150D
loc_4_1553:
        add    esp, 0xc
        pop    ebp
        ret    
IOS_Requestor_Service:
        pushal 
        mov    esi, dword ptr [esp + 0x24]
        xor    ebx, ebx
        mov    byte ptr [esi + 1], bl
        mov    bl, byte ptr [esi]
        cmp    bl, 0xa
        ja     loc_4_1572
        call   sub_4_1577
loc_4_1570:
        popal  
        ret    
loc_4_1572:
        inc    byte ptr [esi + 1]
        jmp    loc_4_1570
sub_4_1577:
        sbb    edx, dword ptr [esi]
        add    byte ptr [eax], al
        pop    eax
        pop    ss
        add    byte ptr [eax], al
        mov    dword ptr [0x1d000015], eax   ; -> loc_4_181D
        sbb    byte ptr [eax], al
        add    bh, bl                        ; -> loc_4_18DF
        sbb    byte ptr [eax], al
        add    byte ptr [0x3b000019], al     ; -> loc_4_1905
        sbb    dword ptr [eax], eax
        add    byte ptr [edx], cl            ; -> loc_4_1A0A
        sbb    al, byte ptr [eax]
        add    ch, cl                        ; -> loc_4_19CD
        sbb    dword ptr [eax], eax
        add    byte ptr [edi - 0x13ffffe6], cl ; -> loc_4_1A8F
        sbb    al, byte ptr [eax]
        add    byte ptr [ebx + 0x57], dl
        movzx  ebx, byte ptr [esi + 2]
        test   bl, 0x80
        jne    loc_4_15D1
        cmp    bl, 0x41
        jb     loc_4_1614
        cmp    bl, 0x7a
        ja     loc_4_1614
        and    bl, 0xdf
        cmp    bl, 0x5a
        ja     loc_4_1614
        sub    bl, 0x41
        cmp    bl, 0x1a
        jae    loc_4_1614
        mov    edi, dword ptr [ebx*4 + 0x20ac] ; -> obj1:0x20AC
        jmp    loc_4_15E0
loc_4_15D1:
        sub    bl, 0x80
        cmp    bl, 0x18
        ja     loc_4_1614
        mov    edi, dword ptr [ebx*4 + 0x2114] ; -> obj1:0x2114
loc_4_15E0:
        or     edi, edi
        je     loc_4_1614
        mov    ebx, dword ptr [edi + 0x20]
        mov    dword ptr [esi + 3], ebx
        test   byte ptr [edi + 0x1d], 0x20
        je     loc_4_15F4
        or     byte ptr [esi + 5], 0x80
loc_4_15F4:
        test   byte ptr [edi + 0x1e], 0x40
        je     loc_4_15FE
        or     byte ptr [esi + 5], 0x40
loc_4_15FE:
        test   ebx, 0x2000
        jne    loc_4_1611
        mov    edi, dword ptr [edi]
        mov    ebx, dword ptr [edi + 0x20]
        and    ebx, 1
        or     dword ptr [esi + 3], ebx
loc_4_1611:
        pop    ebx
        pop    edi
        ret    
loc_4_1614:
        pop    ebx
        pop    edi
        mov    byte ptr [esi + 1], 1
        ret    
loc_4_161B:
        mov    edi, esi
        mov    eax, dword ptr [esi + 3]
        test   byte ptr [esi + 2], 1
        jne    loc_4_16F9
        call   sub_1_128D
        jb     loc_4_16F3
loc_4_1635:
        push   0
        push   0x30
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_16F3
        mov    ecx, dword ptr [ebx + 0x1c]
        and    ecx, 0x653e10
        test   dword ptr [ebx + 0x20], 0x40000
        je     loc_4_1662
        or     ecx, 0x800000
loc_4_1662:
        test   dword ptr [ebx + 0x20], 4
        je     loc_4_1671
        or     ecx, 0x4000
loc_4_1671:
        mov    edx, ebx
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_167B
        mov    edx, dword ptr [ebx]
loc_4_167B:
        mov    dword ptr [eax], ecx
        xor    ecx, ecx
        cmp    dword ptr [edx + 0x41], 0
        je     loc_4_168B
        or     ecx, 0x8000
loc_4_168B:
        mov    dword ptr [eax + 4], ecx
        test   byte ptr [ebx + 0x21], 0x20
        je     loc_4_1698
        or     byte ptr [eax + 4], 0x80
loc_4_1698:
        test   byte ptr [ebx + 0x20], 2
        jne    loc_4_16A2
        or     byte ptr [eax + 4], 0x40
loc_4_16A2:
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 0xbc
        mov    word ptr [eax + 0xa], cx
        movzx  ecx, byte ptr [edx + 0x77]
        mov    word ptr [eax + 8], cx
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 0x64
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [edx + 0xdb]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [ebx + 0x28]
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [ebx + 0x18], eax
        mov    dword ptr [eax + 0x28], ebx
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_16DD
        mov    ebx, dword ptr [ebx]
loc_4_16DD:
        add    ebx, 0xc3
        mov    dword ptr [eax + 0x1c], ebx
        call   sub_1_179B
        mov    byte ptr [edi + 1], 0
        mov    dword ptr [edi + 0xb], eax
loc_4_16F2:
        ret    
loc_4_16F3:
        mov    byte ptr [edi + 1], 0xa0
        jmp    loc_4_16F2
loc_4_16F9:
        push   edi
        mov    esi, eax
        call   sub_4_21EF
        mov    edi, esi
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx], 8
        mov    byte ptr [edx + 0xc], 0xff
        mov    dword ptr [edx + 4], 0
        push   edx
loc_4_1719:
        call   sub_1_1310
        movzx  ecx, word ptr [edx + 2]
        or     ecx, ecx
        jne    loc_4_174A
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [edx + 4], ebx
        test   byte ptr [ebx + 0x20], 0x80
        je     loc_4_1719
        test   byte ptr [ebx + 0x21], 0x80
        je     loc_4_1719
        mov    esi, dword ptr [ebx + 0xcf]
        call   sub_4_21EF
        call   sub_4_21D0
        jne    loc_4_1719
loc_4_174A:
        add    esp, 0x14
        pop    edi
        or     ecx, ecx
        je     loc_4_1635
        jmp    loc_4_16F3
loc_4_1758:
        pushfd 
        movzx  eax, byte ptr [esi + 2]
        test   byte ptr [esi + 3], 1
        je     loc_4_176B
        mov    edi, dword ptr [esi + 4]
        mov    ebx, dword ptr [edi + 0x1c]
        jmp    loc_4_177D
loc_4_176B:
        call   sub_1_128D
        jb     loc_4_1817
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     loc_4_17D2
loc_4_177D:
        call   sub_1_17AE
        jb     loc_4_17F3
        mov    ecx, dword ptr [esi + 8]
        or     ecx, ecx
        jne    loc_4_17BD
        cmp    dword ptr [ebx + 0x18], edi
        jne    loc_4_1793
        mov    dword ptr [ebx + 0x18], ecx
loc_4_1793:
        test   byte ptr [esi + 3], 1
        jne    loc_4_17C4
        sub    esp, 0x14
        mov    word ptr [esp], 0x13
        mov    dword ptr [esp + 0xc], edi
        mov    edi, esp
        movzx  ecx, byte ptr [esi + 2]
        mov    dword ptr [edi + 0x10], ecx
        call   sub_1_175A
        mov    edi, dword ptr [esp + 0xc]
        add    esp, 0x14
        jmp    loc_4_17C4
loc_4_17BD:
        mov    dword ptr [ebx + 0x18], ecx
        and    dword ptr [ecx + 4], 0xffffffde
loc_4_17C4:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_4_17F3
loc_4_17D2:
        mov    ecx, dword ptr [esi + 8]
        jecxz  loc_4_17F3
        sub    esp, 0x14
        mov    edi, esp
        mov    word ptr [edi], 0x12
        mov    dword ptr [edi + 0xc], ecx
        movzx  ecx, byte ptr [esi + 2]
        mov    dword ptr [edi + 0x10], ecx
        call   sub_1_175A
        add    esp, 0x14
loc_4_17F3:
        test   byte ptr [esi + 3], 2
        jne    loc_4_1815
        cli    
        mov    ecx, dword ptr [ebx + 0x18]
        or     ecx, ecx
        je     loc_4_1815
        and    dword ptr [ecx + 4], 0xfffffffc
        and    dword ptr [ebx + 0x20], 0xffefefff
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 0xb7], 0
loc_4_1815:
        popfd  
        ret    
loc_4_1817:
        mov    byte ptr [esi + 1], 1
        jmp    loc_4_1815
loc_4_181D:
        mov    edx, dword ptr [esi + 3]
        or     edx, edx
        jne    loc_4_1831
        movzx  eax, byte ptr [esi + 2]
        call   sub_1_128D
        mov    ebp, ebx
        jmp    loc_4_183A
loc_4_1831:
        mov    ebp, dword ptr [edx + 0x1c]
        sub    ebp, 0xc3
loc_4_183A:
        mov    al, byte ptr [esi + 2]
        mov    edi, esi
        mov    esi, dword ptr [ebp + 4]
        add    esi, 0x64
        sub    esp, 0x10
        mov    ebx, esp
        mov    dword ptr [ebx + 6], esi
        add    esi, 0x58
        mov    word ptr [ebx + 4], si
        mov    byte ptr [ebx + 0xe], 2
        mov    word ptr [ebx], 2
        push   ebx
        call   sub_1_1310
        mov    ebx, dword ptr [ebx + 0xa]
        add    esp, 0x14
        add    ebx, dword ptr [ebp + 4]
        mov    esi, edi
        mov    word ptr [ebx + 0x68], 6
        mov    byte ptr [ebx + 0xb1], al
        xor    eax, eax
        mov    dword ptr [ebx + 0x64], eax
        mov    dword ptr [ebx + 0xa8], eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [ebx + 0x6c], 0x500
        mov    edi, ebx
        add    ebx, 0x64
        xchg   esi, ebx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        mov    ax, word ptr [edi + 0x6a]
        mov    edi, dword ptr [edi + 0x88]
        call   sub_1_0A1E
        mov    byte ptr [ebx + 1], 0xa2
        cmp    ax, 0x10
        jae    loc_4_18D1
        mov    byte ptr [ebx + 1], 0
        mov    edx, dword ptr [ebx + 3]
        or     edx, edx
        je     loc_4_18CA
        and    dword ptr [edx + 4], 0xfffffffd
loc_4_18CA:
        and    dword ptr [ebp + 0x20], 0xffffefff
loc_4_18D1:
        ret    
        db  090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
loc_4_18DF:
        mov    byte ptr [esi + 1], 1
        mov    edi, dword ptr [0x20ac]       ; -> obj1:0x20AC
        or     edi, edi
        je     loc_4_1904
        test   byte ptr [edi + 0x1d], 0x20
        je     loc_4_1904
        mov    al, byte ptr [esi + 2]
        cmp    al, 1
        ja     loc_4_1904
        mov    byte ptr [edi + 0xae], al
        mov    byte ptr [esi + 1], 0
loc_4_1904:
        ret    
loc_4_1905:
        mov    byte ptr [0x563], 1           ; -> obj5:0x0563
        push   0
        push   dword ptr [0x3a58]            ; -> loc_3_3A58
        VxDCall  CONFIGMG._CONFIGMG_Register_DevLoader ; cd2032003300
        add    esp, 8
        mov    dword ptr [0x36c], 0          ; -> obj5:0x036C
        sub    esp, 0xc
        mov    word ptr [esp], 0xd
        mov    edi, esp
        call   sub_1_175A
        add    esp, 0xc
        ret    
sub_4_193B:
        mov    dword ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 0xff
        movzx  eax, byte ptr [esi + 2]
        call   sub_1_128D
        jb     loc_4_19B5
        test   byte ptr [ebx + 0x21], 0x40
        je     loc_4_19B5
        sub    esp, 0x18
        mov    word ptr [esp], 0x10
        mov    dword ptr [esp + 0xc], ebx
        mov    dword ptr [esp + 0x10], 0
        mov    byte ptr [esp + 0x14], al
        mov    edi, esp
        call   sub_1_175A
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 3], eax
        mov    byte ptr [esi + 1], 0
        add    esp, 0x18
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    dword ptr [ecx + 0x3c], 0
        je     loc_4_19B5
        call   sub_4_2257
        movzx  edx, byte ptr [esi + 2]
        lea    edi, [0x21cf]                 ; -> obj1:0x21CF
        call   sub_4_19B6
        or     dword ptr [esi + 3], eax
        lea    edi, [0x21e9]                 ; -> obj1:0x21E9
        call   sub_4_19B6
        or     dword ptr [esi + 3], eax
loc_4_19B5:
        ret    
sub_4_19B6:
        xor    ecx, ecx
        xor    eax, eax
loc_4_19BA:
        cmp    byte ptr [ecx + edi], dl
        jne    loc_4_19C6
        cmp    ecx, edx
        je     loc_4_19C6
        bts    eax, ecx
loc_4_19C6:
        inc    ecx
        cmp    ecx, 0x1a
        jb     loc_4_19BA
        ret    
loc_4_19CD:
        sub    esp, 0x20
        mov    edi, esp
        mov    word ptr [edi], 0x11
        movzx  eax, word ptr [esi + 2]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 0xc], eax
        and    dword ptr [edi + 0x10], 0
        call   sub_1_175A
        mov    eax, dword ptr [edi + 0x14]
        mov    word ptr [esi + 2], ax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x10], eax
        add    esp, 0x20
        ret    
loc_4_1A0A:
        mov    dword ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 0xff
        movzx  eax, byte ptr [esi + 2]
        mov    edx, eax
        or     eax, 0x40000000
        call   sub_1_128D
        jb     loc_4_1A50
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_4_1A33
        test   byte ptr [ebx + 0x21], 0x80
        je     loc_4_1A8E
loc_4_1A33:
        xor    eax, eax
        mov    ecx, ebx
        cmp    byte ptr [ebx + 0x40], 0xa
        jne    loc_4_1A42
        bts    eax, edx
        jmp    loc_4_1A87
loc_4_1A42:
        mov    ecx, dword ptr [ecx + 0x10]
        jecxz  loc_4_1A52
        movzx  ebx, byte ptr [ecx + 0x14]
        bts    eax, ebx
        jmp    loc_4_1A42
loc_4_1A50:
        xor    eax, eax
loc_4_1A52:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [ecx + 0x3c]
        jecxz  loc_4_1A87
loc_4_1A5D:
        mov    ecx, dword ptr [ecx + 0x10]
        jecxz  loc_4_1A87
        movzx  ebx, byte ptr [ecx + 0x14]
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edi, dword ptr [edi + 0x34]
loc_4_1A6F:
        cmp    byte ptr [edi], 0xff
        je     loc_4_1A5D
        cmp    byte ptr [edi], bl
        je     loc_4_1A7D
        add    edi, 0x12
        jmp    loc_4_1A6F
loc_4_1A7D:
        cmp    dl, byte ptr [edi + 1]
        jne    loc_4_1A5D
        bts    eax, ebx
        jmp    loc_4_1A5D
loc_4_1A87:
        mov    dword ptr [esi + 3], eax
        mov    byte ptr [esi + 1], 0
loc_4_1A8E:
        ret    
loc_4_1A8F:
        mov    eax, dword ptr [esi + 2]
        mov    byte ptr [esi + 1], 0xff
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx], 8
        mov    byte ptr [edx + 0xc], 0xff
        mov    dword ptr [edx + 4], eax
        push   edx
loc_4_1AA8:
        call   sub_1_1310
        cmp    word ptr [edx + 2], 0
        jne    loc_4_1AE8
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [edx + 4], ebx
        test   byte ptr [ebx + 0x20], 0x80
        je     loc_4_1AA8
        test   byte ptr [ebx + 0x21], 0x80
        je     loc_4_1AA8
        mov    edx, esi
        mov    edi, dword ptr [esi + 6]
        mov    esi, dword ptr [ebx + 0xcf]
        mov    ecx, 0xb
loc_4_1AD6:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_4_1ADE
        stosb  byte ptr es:[edi], al
        loop   loc_4_1AD6
loc_4_1ADE:
        mov    byte ptr [edi], 0
        mov    byte ptr [edx + 1], 0
        mov    dword ptr [edx + 2], ebx
loc_4_1AE8:
        add    esp, 0x14
        ret    
loc_4_1AEC:
        mov    byte ptr [esi + 1], 0xff
        mov    byte ptr [esi + 3], 0
        movzx  eax, byte ptr [esi + 2]
        call   sub_1_128D
        jb     loc_4_1B38
        test   byte ptr [ebx + 0x21], 0x40
        je     loc_4_1B38
        call   sub_4_2257
        movzx  eax, byte ptr [esi + 2]
        movzx  ecx, byte ptr [eax + 0x21cf]  ; -> obj1:0x21CF
        cmp    ecx, 0xff
        je     loc_4_1B39
        cmp    ecx, eax
        je     loc_4_1B39
loc_4_1B21:
        mov    dl, 1
        bt     dword ptr [0x21cb], eax       ; -> obj1:0x21CB
        jae    loc_4_1B2E
        mov    dl, 2
loc_4_1B2E:
        mov    byte ptr [esi + 2], cl
        mov    byte ptr [esi + 3], dl
loc_4_1B34:
        mov    byte ptr [esi + 1], 0
loc_4_1B38:
        ret    
loc_4_1B39:
        movzx  ecx, byte ptr [eax + 0x21e9]  ; -> obj1:0x21E9
        cmp    ecx, 0xff
        je     loc_4_1B4E
        cmp    ecx, eax
        je     loc_4_1B4E
        jmp    loc_4_1B21
loc_4_1B4E:
        test   byte ptr [ebx + 0x21], 2
        je     loc_4_1B34
        mov    cl, byte ptr [ebx + 0x47]
        jmp    loc_4_1B21
        db  000h, 000h, 000h
sub_4_1B5C:
        mov    ds, word ptr [ebx]
        add    byte ptr [eax], al
        popal  
        add    byte ptr [eax], al
        db  000h
IOS_V86_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 1
        ja     loc_4_1B83
        call   sub_4_1B5C
        jb     loc_4_1B83
        mov    word ptr [ebp + 0x1c], 0
        and    dword ptr [ebp + 0x2c], 0xfffffffe
        ret    
loc_4_1B83:
        mov    word ptr [ebp + 0x1c], 0xffff
        or     dword ptr [ebp + 0x2c], 1
        ret    
loc_4_1B8E:
        movzx  eax, word ptr [ebp + 0x10]
        cmp    word ptr [ebp + 0x18], 1
        jne    loc_4_1B9E
        or     eax, 0x40000000
loc_4_1B9E:
        call   sub_1_128D
        jb     loc_4_1BBD
        mov    ax, word ptr [ebx + 0x16]
        and    ax, 0x40
        mov    ecx, dword ptr [ebx + 0x20]
        and    ecx, 0x2000
        or     ax, cx
        mov    word ptr [ebp + 0x14], ax
loc_4_1BBD:
        ret    
        db  000h, 000h
sub_4_1BC0:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    esi, dword ptr [ecx + 0x34]
loc_4_1BC9:
        cmp    byte ptr [esi], 0xff
        je     loc_4_1BD7
        cmp    byte ptr [esi], al
        je     loc_4_1BD8
        add    esi, 0x12
        jmp    loc_4_1BC9
loc_4_1BD7:
        stc    
loc_4_1BD8:
        ret    
loc_4_1BD9:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    word ptr [eax + 3], 0x800c
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a800100
        db  000h
sub_4_1BF0:
        call   sub_4_2145
        cmp    dword ptr [0x320], 0          ; -> obj5:0x0320
        je     loc_4_1C40
        mov    edx, dword ptr [0x328]        ; -> obj5:0x0328
        mov    ecx, dword ptr [0x324]        ; -> obj5:0x0324
        cmp    byte ptr [0x31b], 0           ; -> obj5:0x031B
        jne    loc_4_1C40
        or     bx, 0xffff
        mov    edx, dword ptr [edx + 0xb]
        VxDCall  APM.Service_0002h               ; cd2002002700
        xor    eax, eax
        xchg   dword ptr [0x320], eax        ; -> obj5:0x0320
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_4_1C34
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_1C34:
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_1C40:
        ret    
sub_4_1C41:
        mov    ebx, dword ptr [0x31c]        ; -> obj5:0x031C
        mov    edx, 0x13
loc_4_1C4C:
        dec    edx
        js     loc_4_1C80
        mov    edi, dword ptr [ebx + edx*4]
loc_4_1C52:
        or     edi, edi
        je     loc_4_1C4C
        cmp    byte ptr [edi + 0xa], 1
        je     loc_4_1C7B
        push   edx
        mov    edx, dword ptr [edi + 4]
        VxDCall  APM.Service_0004h               ; cd2004002700
        pop    edx
        mov    byte ptr [edi + 0xa], 1
        mov    esi, dword ptr [edi + 0x20]
loc_4_1C6E:
        or     esi, esi
        je     loc_4_1C7B
        mov    byte ptr [edi + 0xa], 1
        mov    esi, dword ptr [esi + 0x20]
        jmp    loc_4_1C6E
loc_4_1C7B:
        mov    edi, dword ptr [edi + 0x1c]
        jmp    loc_4_1C52
loc_4_1C80:
        ret    
loc_4_1C81:
        mov    eax, 0x21
        cmp    byte ptr [0x563], 0           ; -> obj5:0x0563
        je     loc_4_1CB3
        push   1
        push   0x2960                        ; -> loc_4_2960
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f003300
        add    esp, 0xc
        push   3
        push   0
        push   0x2388                        ; -> loc_4_2388
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
loc_4_1CB3:
        stc    
        ret    
sub_4_1CB5:
        push   1
        push   0x80
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    dword ptr [0x31c], eax        ; -> obj5:0x031C
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        mov    esi, 0x306                    ; -> obj5:0x0306
        call   sub_4_21E3
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        jne    loc_4_1D1B
        mov    edx, 0x1fd                    ; -> obj5:0x01FD
        mov    esi, edx
        add    esi, 0x1e
        mov    ah, 0x1a
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_4_2221
        mov    edx, 0x179                    ; -> obj5:0x0179
        xor    cx, cx
        mov    ah, 0x4e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_4_1D84
        jmp    loc_4_1D51
loc_4_1D1B:
        mov    esi, 0x179                    ; -> obj5:0x0179
        xor    ecx, ecx
        mov    edx, 0x568                    ; -> obj5:0x0568
        mov    eax, 0x4e00
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        jb     loc_4_1D84
        mov    dword ptr [0x564], eax        ; -> obj5:0x0564
loc_4_1D39:
        mov    edx, 0x568                    ; -> obj5:0x0568
        lea    esi, [edx + 0x130]
        cmp    byte ptr [esi], 0
        jne    loc_4_1D51
        lea    esi, [edx + 0x2c]
        cmp    byte ptr [esi], 0
        je     loc_4_1D84
loc_4_1D51:
        push   esi
        call   sub_4_21EF
        call   sub_4_2198
        jb     loc_4_1D6E
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        call   sub_4_1DA7
loc_4_1D6E:
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        jne    loc_4_1D8C
        mov    ah, 0x4f
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    esi
        jae    loc_4_1D51
loc_4_1D84:
        mov    byte ptr [0x153], 1           ; -> obj5:0x0153
        ret    
loc_4_1D8C:
        pop    esi
        mov    ebx, dword ptr [0x564]        ; -> obj5:0x0564
        mov    edx, 0x568                    ; -> obj5:0x0568
        mov    eax, 0x4f00
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        jb     loc_4_1D84
        jmp    loc_4_1D39
sub_4_1DA7:
        push   ebp
        push   0
        push   0
        mov    edx, 0x179                    ; -> obj5:0x0179
        xor    eax, eax
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_4_1E5D
        or     edx, edx
        je     loc_4_1E5D
        mov    ebx, dword ptr [0x31c]        ; -> obj5:0x031C
        mov    byte ptr [0x153], 0           ; -> obj5:0x0153
        mov    esi, dword ptr [eax + 0x2c]
        mov    ebp, dword ptr [esi + 8]
        mov    edi, ebp
        mov    ecx, 0x20
loc_4_1DE1:
        rcr    edi, 1
        jae    loc_4_1DE9
        inc    dword ptr [esp + 4]
loc_4_1DE9:
        loop   loc_4_1DE1
        xor    edi, edi
loc_4_1DED:
        bsr    esi, ebp
        je     loc_4_1E55
        btr    ebp, esi
        inc    byte ptr [0x153]              ; -> obj5:0x0153
        push   eax
        push   edx
        push   1
        push   0x24
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        mov    dword ptr [eax + 0x20], edi
        mov    edi, eax
        pop    edx
        pop    eax
        cmp    esi, 0x13
        jb     loc_4_1E21
        mov    dword ptr [esp], edi
loc_4_1E21:
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi], eax
        mov    cl, byte ptr [0x153]          ; -> obj5:0x0153
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [esp + 4]
        mov    byte ptr [edi + 9], cl
        push   esi
        push   edi
        lea    edi, [edi + 0xb]
        mov    esi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        pop    edi
        pop    esi
        mov    ecx, dword ptr [ebx + esi*4]
        jecxz  loc_4_1E63
        mov    dword ptr [ebx + esi*4], edi
        mov    dword ptr [edi + 0x1c], ecx
        jmp    loc_4_1DED
loc_4_1E55:
        mov    edi, dword ptr [esp]
        add    esp, 8
loc_4_1E5B:
        pop    ebp
        ret    
loc_4_1E5D:
        add    esp, 8
        stc    
        jmp    loc_4_1E5B
loc_4_1E63:
        mov    dword ptr [ebx + esi*4], edi
        jmp    loc_4_1DED
sub_4_1E68:
        push   ebp
        mov    ebx, dword ptr [0x31c]        ; -> obj5:0x031C
        mov    edx, 0x13
loc_4_1E74:
        dec    edx
        js     loc_4_1F0B
        mov    edi, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
loc_4_1E81:
        cmp    byte ptr [edi + 0x44], dl
        jne    loc_4_1E91
        test   byte ptr [edi + 0x3a], 2
        jne    loc_4_1E91
        call   sub_4_2103
loc_4_1E91:
        mov    edi, dword ptr [edi + 2]
        or     edi, edi
        jne    loc_4_1E81
        mov    edi, dword ptr [ebx + edx*4]
loc_4_1E9B:
        or     edi, edi
        je     loc_4_1E74
        mov    ecx, dword ptr [edi + 0x18]
        or     ecx, ecx
        jne    loc_4_1F06
        cmp    byte ptr [edi + 0xa], 0
        jne    loc_4_1F06
        mov    dword ptr [0x36c], edi        ; -> obj5:0x036C
        push   edx
        push   edi
        push   ebx
        mov    ebp, dword ptr [edi]
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [0x153], al          ; -> obj5:0x0153
        mov    eax, 0x1b
        call   dword ptr [ebp + 0x18]
        pop    ebx
        pop    edi
        mov    dword ptr [0x36c], 0          ; -> obj5:0x036C
        mov    edx, dword ptr [edi + 4]
        rcl    byte ptr [edi + 0xa]
        mov    cl, byte ptr [edi + 9]
        cmp    cl, byte ptr [edi + 8]
        jne    loc_4_1F05
        mov    esi, edi
        movzx  ecx, byte ptr [edi + 8]
loc_4_1EE7:
        cmp    byte ptr [esi + 0xa], 0
        je     loc_4_1EFA
        mov    esi, dword ptr [esi + 0x20]
        loop   loc_4_1EE7
        VxDCall  APM.Service_0004h               ; cd2004002700
        jmp    loc_4_1F05
loc_4_1EFA:
        or     word ptr [ebp + 0xa], 2
        VxDCall  APM.Service_0003h               ; cd2003002700
loc_4_1F05:
        pop    edx
loc_4_1F06:
        mov    edi, dword ptr [edi + 0x1c]
        jmp    loc_4_1E9B
loc_4_1F0B:
        pop    ebp
        mov    byte ptr [0x153], 1           ; -> obj5:0x0153
        ret    
        db  0F6h, 041h, 03Ah, 002h, 075h, 0ECh, 087h, 0CFh, 0E8h, 0E2h, 001h, 000h, 000h, 08Bh, 0F9h, 0EBh
        db  0E1h
sub_4_1F25:
        call   sub_4_20EE
        mov    ecx, 9
        mov    edi, esi
        mov    al, 0x2e
        repne scasb al, byte ptr es:[edi]
        cmp    dword ptr [edi - 1], 0x64706d2e
        je     loc_4_1FDB
        call   sub_4_1DA7
        jb     loc_4_2099
        or     edi, edi
        je     loc_4_2099
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    esi, dword ptr [ecx + 0x2c]
        mov    ebx, dword ptr [esi + 8]
        bt     ebx, 0x1e
        jb     loc_4_1F8F
        call   sub_4_209E
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        jb     loc_4_1F8F
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [0x2244], 4          ; -> obj1:0x2244
        jne    loc_4_1F86
        bt     ebx, 0x1b
        jb     loc_4_1F8F
loc_4_1F86:
        test   dword ptr [eax + 0x28], 2
        jne    loc_4_1FD0
loc_4_1F8F:
        mov    dword ptr [0x36c], edi        ; -> obj5:0x036C
        push   edx
        push   ecx
        mov    eax, 0x1b
        call   dword ptr [ecx + 0x18]
        pop    ecx
        pop    edx
        jb     loc_4_1FD0
        mov    ebx, dword ptr [ecx + 0x2c]
        mov    ebx, dword ptr [ebx + 8]
        bt     ebx, 0x15
        jae    loc_4_1FC0
        mov    dword ptr [0x320], eax        ; -> obj5:0x0320
        mov    dword ptr [0x328], edx        ; -> obj5:0x0328
        mov    dword ptr [0x324], ecx        ; -> obj5:0x0324
loc_4_1FC0:
        or     word ptr [ecx + 0xa], 2
        VxDCall  APM.Service_0003h               ; cd2003002700
        jmp    loc_4_207B
loc_4_1FD0:
        VxDCall  APM.Service_0004h               ; cd2004002700
        jmp    loc_4_2099
loc_4_1FDB:
        cmp    dword ptr [0x320], 0          ; -> obj5:0x0320
        je     loc_4_2099
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   dword ptr [eax + 0x28], 2
        je     loc_4_2001
        call   sub_4_209E
        jae    loc_4_2099
loc_4_2001:
        push   esi
        push   edi
        mov    edi, 0x348                    ; -> obj5:0x0348
        mov    dword ptr [0x36c], edi        ; -> obj5:0x036C
        lea    edi, [edi + 0xb]
        call   sub_4_21E3
        sub    esp, 4
        mov    edx, esp
        lea    ecx, [0x179]                  ; -> obj5:0x0179
        mov    eax, dword ptr [0x320]        ; -> obj5:0x0320
        lea    eax, [eax]
        push   1
        push   eax
        push   ecx
        push   edx
        VxDCall  APM.Service_0011h               ; cd2011002700
        add    esp, 0x10
        pop    dword ptr [0x34c]             ; -> obj5:0x034C
        pop    edi
        pop    esi
        or     eax, eax
        jne    loc_4_2099
        mov    byte ptr [edi - 1], 0
        mov    edi, dword ptr [0x34c]        ; -> obj5:0x034C
        push   edi
        VxDCall  APM.Service_000Ah               ; cd200a002700
        add    esp, 4
        or     eax, eax
        je     loc_4_2087
        push   eax
        push   esi
        mov    eax, dword ptr [0x320]        ; -> obj5:0x0320
        call   dword ptr [eax + 4]
        add    esp, 4
        pop    ebx
        or     eax, eax
        jne    loc_4_2087
        mov    ecx, dword ptr [0x145]        ; -> obj5:0x0145
        mov    dword ptr [ecx + 0x4b], ebx
        inc    byte ptr [0x31b]              ; -> obj5:0x031B
loc_4_207B:
        clc    
loc_4_207C:
        mov    dword ptr [0x36c], 0          ; -> obj5:0x036C
        ret    
loc_4_2087:
        mov    eax, dword ptr [0x320]        ; -> obj5:0x0320
        lea    eax, [eax]
        push   eax
        push   edi
        VxDCall  APM.Service_000Eh               ; cd200e002700
        add    esp, 8
loc_4_2099:
        xor    eax, eax
        stc    
        jmp    loc_4_207C
sub_4_209E:
        mov    eax, dword ptr [0x344]        ; -> obj5:0x0344
        mov    dword ptr [0x13f], 1          ; -> obj5:0x013F
        mov    byte ptr [0x144], 0           ; -> obj5:0x0144
        push   1
        push   0x13f                         ; -> obj5:0x013F
        push   0x143                         ; -> obj5:0x0143
        push   3
        push   0xb2                          ; -> obj5:0x00B2
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     eax, eax
        jne    loc_4_20E2
        cmp    byte ptr [0x143], 0           ; -> obj5:0x0143
        je     loc_4_20E2
loc_4_20E0:
        stc    
        ret    
loc_4_20E2:
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x2a], 8
        jne    loc_4_20E0
        ret    
sub_4_20EE:
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    ecx, dword ptr [ecx + 0x30]
loc_4_20F7:
        jecxz  loc_4_2102
        and    byte ptr [ecx + 0x21], 0xfb
        mov    ecx, dword ptr [ecx + 0xc]
        jmp    loc_4_20F7
loc_4_2102:
        ret    
sub_4_2103:
        pushal 
        sub    esp, 0x10
        mov    esi, esp
        mov    cl, byte ptr [edi + 0x44]
        mov    byte ptr [esi + 8], cl
        mov    word ptr [esi], 3
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    ebx, edi
        mov    edi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edi, dword ptr [edi + 0x30]
loc_4_2125:
        or     edi, edi
        je     loc_4_2140
        test   byte ptr [edi + 0x21], 4
        je     loc_4_213B
        mov    dword ptr [esi + 0xc], edi
        push   ebx
        push   edi
        push   esi
        call   dword ptr [ebx + 8]
        pop    esi
        pop    edi
        pop    ebx
loc_4_213B:
        mov    edi, dword ptr [edi + 0xc]
        jmp    loc_4_2125
loc_4_2140:
        add    esp, 0x10
        popal  
        ret    
sub_4_2145:
        mov    ebx, dword ptr [0x31c]        ; -> obj5:0x031C
        mov    edi, 0x20
loc_4_2150:
        dec    edi
        js     loc_4_216D
        mov    ecx, dword ptr [ebx + edi*4]
loc_4_2156:
        jecxz  loc_4_2150
        mov    esi, dword ptr [ecx + 0x1c]
        push   0
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        or     eax, eax
        mov    ecx, esi
        jmp    loc_4_2156
loc_4_216D:
        push   0
        push   ebx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        or     eax, eax
        ret    
loc_4_217C:
        call   sub_4_1CB5
        call   sub_4_20EE
        call   sub_4_1E68
        call   sub_4_1BF0
        call   sub_4_14EC
        clc    
        ret    
        db  000h
sub_4_2198:
        push   ebx
        push   edx
        mov    ebx, esi
        mov    edi, dword ptr [0x1fa4]       ; -> obj1:0x1FA4
        or     edi, edi
        je     loc_4_21CB
        mov    edx, edi
        lea    edi, [edi + 0x14]
        cld    
loc_4_21AC:
        call   sub_4_21D0
        je     loc_4_21C5
        mov    edi, edx
        mov    edi, dword ptr [edi + 2]
        or     edi, edi
        je     loc_4_21CA
        mov    edx, edi
        lea    edi, [edi + 0x14]
        mov    esi, ebx
        jmp    loc_4_21AC
loc_4_21C5:
        stc    
        mov    edi, edx
        jmp    loc_4_21CB
loc_4_21CA:
        clc    
loc_4_21CB:
        mov    esi, ebx
        pop    edx
        pop    ebx
        ret    
sub_4_21D0:
        push   esi
        push   edi
        push   eax
loc_4_21D3:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_4_21DF
        cmp    al, byte ptr [edi]
        jne    loc_4_21DF
        inc    edi
        jmp    loc_4_21D3
loc_4_21DF:
        pop    eax
        pop    edi
        pop    esi
        ret    
sub_4_21E3:
        cld    
        push   esi
        push   eax
loc_4_21E6:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_4_21E6
        pop    eax
        pop    esi
        ret    
sub_4_21EF:
        push   esi
loc_4_21F0:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_4_2204
        cmp    al, 0x41
        jb     loc_4_21F0
        cmp    al, 0x5a
        ja     loc_4_21F0
        add    al, 0x20
        mov    byte ptr [esi - 1], al
        jmp    loc_4_21F0
loc_4_2204:
        pop    esi
        ret    
loc_4_2206:
        movzx  esi, word ptr [0x10]          ; -> loc_2_0010
        add    esi, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, esi
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_2221:
        movzx  esi, word ptr [0xc]           ; -> loc_2_000C
        add    esi, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, esi
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_223C:
        movzx  esi, word ptr [0xe]           ; -> loc_2_000E
        add    esi, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, esi
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
sub_4_2257:
        pushal 
        mov    edi, 0x21cf                   ; -> obj1:0x21CF
        mov    al, 0xff
        mov    ecx, 0x1a
        cld    
        rep stosb byte ptr es:[edi], al
        xor    edi, edi
        xor    esi, esi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_235A
        cmp    word ptr [ebp + 0x10], 0x444d
        jne    loc_4_235A
        movzx  ecx, byte ptr [ebp + 0x18]
        sub    cl, 0x41
        add    cl, byte ptr [ebp + 0x19]
        xor    ebx, ebx
loc_4_22C3:
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 1
        mov    byte ptr [ebp + 0x14], bl
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_2350
        mov    al, byte ptr [ebp + 0x10]
        test   al, 0x80
        je     loc_4_2350
        and    al, 0x7f
        mov    byte ptr [ebx + 0x21cf], al   ; -> obj1:0x21CF
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 1
        mov    byte ptr [ebp + 0x14], al
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_2350
        mov    al, byte ptr [ebp + 0x10]
        and    al, 0x7f
        cmp    al, bl
        je     loc_4_2323
        mov    byte ptr [ebx + 0x21cf], al   ; -> obj1:0x21CF
loc_4_2323:
        bts    edi, ebx
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 0xf
        mov    byte ptr [ebp + 0x14], bl
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_2350
        test   byte ptr [ebp + 0x10], 0x80
        je     loc_4_2350
        bts    esi, ebx
loc_4_2350:
        inc    ebx
        jecxz  loc_4_235A
        dec    ecx
        jne    loc_4_22C3
loc_4_235A:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    dword ptr [0x21c7], edi       ; -> obj1:0x21C7
        mov    dword ptr [0x21cb], esi       ; -> obj1:0x21CB
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    dword ptr [edx + 0x64], edi
        popal  
        ret    
        db  000h, 000h
loc_4_2388:
        sub    esp, 0x10
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [esp + 0x28]
        mov    dword ptr [0x344], ebx        ; -> obj5:0x0344
        mov    edi, dword ptr [esp + 0x20]
        cmp    edi, 1
        jne    loc_4_266D
        lea    esi, [esp + 0x14]
        lea    ecx, [esp + 0x10]
        push   0
        push   ebx
        push   ecx
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Status ; cd2036003300
        add    esp, 0x10
        cmp    eax, 0
        mov    eax, 0x13
        jne    loc_4_26B1
        test   byte ptr [esi + 2], 2
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        jne    loc_4_2411
        lea    eax, [esp + 0x14]
        push   4
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        cmp    eax, 0
        je     loc_4_2411
        lea    eax, [esp + 0x14]
        push   3
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        or     byte ptr [ecx + 0x2a], 8
        cmp    eax, 0
        jne    loc_4_2438
loc_4_2411:
        mov    eax, 0x13
        and    byte ptr [ecx + 0x2a], 0xf3
        and    byte ptr [ecx + 0x2a], 0xfd
        test   byte ptr [ecx + 0x28], 4
        jne    loc_4_26B1
        mov    eax, 0x22
        call   sub_4_27FC
        jb     loc_4_26B1
loc_4_2438:
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        je     loc_4_2448
        pushal 
        call   sub_4_1CB5
        popal  
loc_4_2448:
        lea    edx, [esp + 0xc]
        mov    dword ptr [edx], 0xd
        lea    esi, [0x6c8]                  ; -> obj5:0x06C8
        push   1
        push   edx
        push   esi
        push   1
        push   0x6b5                         ; -> obj5:0x06B5
        push   0
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     eax, eax
        mov    eax, 0x13
        jne    loc_4_26B1
        call   sub_4_21EF
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        push   esi
        lea    esi, [0x154]                  ; -> obj5:0x0154
        call   sub_4_2FF4
        pop    esi
        call   sub_4_2198
        jb     loc_4_251D
        mov    ecx, 9
        mov    edi, esi
        mov    al, 0x2e
        repne scasb al, byte ptr es:[edi]
        cmp    dword ptr [edi - 1], 0x64706d2e
        jne    loc_4_2510
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [ecx + 0x2a], 0x80
        je     loc_4_24DF
        cmp    dword ptr [edi - 8], 0x72617073
        jne    loc_4_24DF
        cmp    dword ptr [edi - 4], 0x2e776f72
        jne    loc_4_24DF
        mov    dword ptr [esp + 0x18], 0
        jmp    loc_4_259C
loc_4_24DF:
        cmp    dword ptr [0x320], 0          ; -> obj5:0x0320
        jne    loc_4_2510
        push   esi
        mov    esi, 0x6a8                    ; -> obj5:0x06A8
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        call   sub_4_1F25
        pop    esi
        jb     loc_4_264C
        mov    edi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
loc_4_2510:
        call   sub_4_1F25
        jb     loc_4_264C
        jmp    loc_4_2558
loc_4_251D:
        call   sub_4_20EE
        mov    dword ptr [0x145], edi        ; -> obj5:0x0145
        xchg   edi, esi
        lea    edi, [esi + 0x14]
        mov    ecx, 9
        mov    al, 0x2e
        repne scasb al, byte ptr es:[edi]
        cmp    dword ptr [edi - 1], 0x64706d2e
        jne    loc_4_2553
        push   dword ptr [esi + 0x4b]
        lea    eax, [esi + 0x14]
        push   eax
        mov    eax, dword ptr [0x320]        ; -> obj5:0x0320
        call   dword ptr [eax + 4]
        add    esp, 8
        jmp    loc_4_2558
loc_4_2553:
        call   sub_4_0383
loc_4_2558:
        call   sub_4_1E68
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        jne    loc_4_265E
        call   sub_3_34B9
loc_4_256F:
        lea    eax, [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x28]
        push   0
        push   ebx
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009003300
        add    esp, 0x10
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_4_259C
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        btr    dword ptr [eax + 0x28], 0x11
        jae    loc_4_2629
loc_4_259C:
        lea    esi, [0x15d]                  ; -> obj5:0x015D
        call   sub_4_2FF4
        mov    eax, 0
        mov    edi, dword ptr [esp + 0x18]
        or     edi, edi
        je     loc_4_26B1
        call   sub_4_2C57
        lea    edx, [esp + 0xc]
        mov    dword ptr [edx], 4
        lea    esi, [0x6c8]                  ; -> obj5:0x06C8
        push   1
        push   edx
        push   esi
        push   1
        push   0x6c0                         ; -> obj5:0x06C0
        push   0
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     eax, eax
        mov    eax, 0
        jne    loc_4_26B1
        call   sub_4_21EF
        cmp    word ptr [esi], 0x6579
        jne    loc_4_2606
        cmp    byte ptr [esi], 0x73
        je     loc_4_26B1
loc_4_2606:
        cmp    word ptr [esi], 0x6f6e
        jne    loc_4_26B1
        mov    esi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esi + 0xc]
loc_4_2618:
        or     esi, esi
        je     loc_4_26B1
        or     byte ptr [esi + 0x23], 8
        mov    esi, dword ptr [esi + 0x6c]
        jmp    loc_4_2618
loc_4_2629:
        lea    esi, [0x16b]                  ; -> obj5:0x016B
        call   sub_4_2FF4
        mov    eax, 0x13
        mov    ecx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [ecx + 0x2a], 4
        je     loc_4_26B1
        mov    eax, 0x2b
        jmp    loc_4_26B1
loc_4_264C:
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        je     loc_4_256F
        call   sub_4_1C41
loc_4_265E:
        call   sub_4_1BF0
        call   sub_4_14EC
        jmp    loc_4_256F
loc_4_266D:
        lea    eax, [esp + 0x18]
        push   0
        push   ebx
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009003300
        add    esp, 0x10
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        je     loc_4_26AC
        cmp    edi, 2
        je     loc_4_275F
        cmp    edi, 4
        je     loc_4_275F
        cmp    edi, 9
        je     loc_4_27DA
        cmp    edi, 0xa
        je     loc_4_26B8
        cmp    edi, 3
        je     loc_4_26D2
loc_4_26AC:
        mov    eax, 1
loc_4_26B1:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
loc_4_26B8:
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, 0x8003
loc_4_26C0:
        or     eax, eax
        je     loc_4_274A
        call   sub_4_288C
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_26C0
loc_4_26D2:
        mov    eax, dword ptr [esi + 0xc]
loc_4_26D5:
        or     eax, eax
        je     loc_4_2721
        cmp    byte ptr [eax + 0x40], 0xa
        je     loc_4_2754
        push   eax
        push   esi
        VxDCall  PAGEFILE.PageFile_Get_Size_Info ; cd2007002100
        movzx  ecx, byte ptr [esi]
        pop    esi
        pop    eax
        jecxz  loc_4_26FD
        push   eax
        mov    eax, ecx
        sub    al, 0x41
        call   sub_1_128D
        pop    eax
        cmp    eax, dword ptr [ebx]
        je     loc_4_2754
loc_4_26FD:
        push   eax
        movzx  eax, byte ptr [0x179]         ; -> obj5:0x0179
        or     al, 0x20
        sub    al, 0x61
        call   sub_1_128D
        pop    eax
        jb     loc_4_2715
        cmp    eax, dword ptr [ebx]
        je     loc_4_2754
loc_4_2715:
        call   sub_4_2819
        jb     loc_4_2754
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_26D5
loc_4_2721:
        cli    
        mov    eax, dword ptr [esi + 0xc]
loc_4_2725:
        or     eax, eax
        je     loc_4_2735
        call   sub_1_1E5C
        jb     loc_4_2754
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_2725
loc_4_2735:
        mov    eax, dword ptr [esi + 0xc]
loc_4_2738:
        or     eax, eax
        je     loc_4_2745
        or     byte ptr [eax + 0x23], 4
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_2738
loc_4_2745:
        or     byte ptr [esi + 0x11], 1
        sti    
loc_4_274A:
        mov    eax, 0
        jmp    loc_4_26B1
loc_4_2754:
        sti    
        mov    eax, 0x17
        jmp    loc_4_26B1
loc_4_275F:
        push   dword ptr [esi + 0x14]
        mov    ecx, esi
        call   sub_4_0E2E
        pop    esi
        dec    byte ptr [esi + 0x4f]
        jne    loc_4_27D0
        cmp    byte ptr [esi + 0x44], 0x14
        jne    loc_4_27D0
        mov    edi, dword ptr [esi + 0x45]
        call   IOS_Service_17
        mov    eax, dword ptr [0x320]        ; -> obj5:0x0320
        lea    eax, [eax]
        push   eax
        push   edi
        VxDCall  APM.Service_000Eh               ; cd200e002700
        add    esp, 8
        mov    edx, dword ptr [0x328]        ; -> obj5:0x0328
        mov    ecx, dword ptr [0x324]        ; -> obj5:0x0324
        dec    byte ptr [0x31b]              ; -> obj5:0x031B
        jne    loc_4_27D0
        or     bx, 0xffff
        mov    edx, dword ptr [edx + 0xb]
        VxDCall  APM.Service_0002h               ; cd2002002700
        xor    eax, eax
        xchg   dword ptr [0x320], eax        ; -> obj5:0x0320
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_4_27C4
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_27C4:
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_27D0:
        call   sub_4_14EC
        jmp    loc_4_274A
loc_4_27DA:
        and    byte ptr [esi + 0x11], 0xfe
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, 0x8002
loc_4_27E6:
        or     eax, eax
        je     loc_4_274A
        and    byte ptr [eax + 0x23], 0xfb
        call   sub_4_288C
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_4_27E6
sub_4_27FC:
        test   byte ptr [0x221c], 4          ; -> obj1:0x221C
        je     loc_4_2818
        push   eax
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    eax, dword ptr [eax + 0x3c]
        or     eax, eax
        je     loc_4_2817
        call   sub_4_2819
loc_4_2817:
        pop    eax
loc_4_2818:
        ret    
sub_4_2819:
        pushal 
        mov    esi, eax
        mov    edi, eax
        xor    ebp, ebp
loc_4_2820:
        xor    edx, edx
        or     esi, esi
        je     loc_4_286B
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_2866
loc_4_282C:
        mov    eax, edx
        call   sub_4_28C1
        jb     loc_4_2866
        push   edi
        push   edx
        xor    edi, edi
        mov    edx, 1
        mov    eax, 0x8001
        call   sub_4_28F9
        pop    edx
        pop    edi
        bts    ebp, ebx
        cmp    eax, 0x2a
        je     loc_4_286E
        push   edx
        mov    eax, ebx
        push   eax
        VxDCall  IFSMgr.QueryVolumeRemoval       ; cd204f004000
        add    esp, 4
        pop    edx
        or     eax, eax
        jne    loc_4_286E
        jmp    loc_4_282C
loc_4_2866:
        mov    esi, dword ptr [esi + 0x10]
        jmp    loc_4_2820
loc_4_286B:
        clc    
loc_4_286C:
        popal  
        ret    
loc_4_286E:
        bsf    ebx, ebp
        je     loc_4_2889
        btr    ebp, ebx
        xor    edi, edi
        mov    edx, 0
        mov    eax, 0x8002
        call   sub_4_28F9
        jmp    loc_4_286E
loc_4_2889:
        stc    
        jmp    loc_4_286C
sub_4_288C:
        pushal 
        mov    esi, eax
        mov    edi, eax
loc_4_2891:
        xor    edx, edx
        or     esi, esi
        je     loc_4_28BF
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_28BA
loc_4_289D:
        mov    eax, edx
        call   sub_4_28C1
        jb     loc_4_28BA
        push   edi
        push   edx
        xor    edi, edi
        mov    edx, 0
        mov    eax, ecx
        call   sub_4_28F9
        pop    edx
        pop    edi
        jmp    loc_4_289D
loc_4_28BA:
        mov    esi, dword ptr [esi + 0x10]
        jmp    loc_4_2891
loc_4_28BF:
        popal  
        ret    
sub_4_28C1:
        push   edi
        push   ecx
        mov    edi, 0x20ac                   ; -> obj1:0x20AC
        mov    ecx, 0x1a
        or     eax, eax
        je     loc_4_28DC
        mov    eax, edx
        sub    eax, edi
        shr    eax, 2
        sub    ecx, eax
        mov    edi, edx
loc_4_28DC:
        jecxz  loc_4_28F6
        mov    eax, esi
        repne scasd eax, dword ptr es:[edi]
        jne    loc_4_28F6
        mov    edx, edi
        sub    edi, 0x20ac                   ; -> obj1:0x20AC
        shr    edi, 2
        dec    edi
        mov    ebx, edi
        clc    
loc_4_28F3:
        pop    ecx
        pop    edi
        ret    
loc_4_28F6:
        stc    
        jmp    loc_4_28F3
sub_4_28F9:
        sub    esp, 0x18
        push   edx
        push   ecx
        lea    ecx, [esp + 8]
        mov    dword ptr [ecx], 0x12
        mov    dword ptr [ecx + 4], 2
        mov    dword ptr [ecx + 0xc], 0
        bts    dword ptr [ecx + 0xc], ebx
        mov    word ptr [ecx + 0x10], 0
        or     edi, edi
        je     loc_4_2928
        or     byte ptr [ecx + 0x10], 1
loc_4_2928:
        push   edx
        push   ecx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Broadcast_Device_Change_Message ; cd2058003300
        add    esp, 0xc
        pop    ecx
        pop    edx
        add    esp, 0x18
        ret    
sub_4_293A:
        pushal 
        VxDCall  SHELL._SHELL_QueryAppyTimeAvailable ; cd200d001700
        or     eax, eax
        je     loc_4_295E
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_295E
        mov    edx, 0
        mov    eax, 0x8004
        movzx  ebx, byte ptr [esi + 0x14]
        call   sub_4_28F9
loc_4_295E:
        popal  
        ret    
loc_4_2960:
        sub    esp, 0x1d8
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [esp + 0x1e8], 5
        mov    eax, 1
        jne    loc_4_2AA5
        mov    eax, dword ptr [esp + 0x1f0]
        mov    dword ptr [0x344], eax        ; -> obj5:0x0344
        lea    eax, [esp + 0x1e0]
        push   0
        push   dword ptr [esp + 0x1f4]
        push   dword ptr [esp + 0x1f8]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009003300
        add    esp, 0x10
        mov    esi, dword ptr [esp + 0x1e0]
        or     esi, esi
        je     loc_4_2A9B
        mov    dword ptr [0x149], esi        ; -> obj5:0x0149
        bts    word ptr [esi + 0x11], 2
        jae    loc_4_2A9B
        mov    eax, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [eax + 0x28], 1
        je     loc_4_2A9B
        mov    ebx, dword ptr [esi + 0x14]
        cmp    byte ptr [ebx + 0x3e], 1
        jne    loc_4_2A9B
        call   sub_4_1CB5
        call   sub_4_20EE
        push   1
        push   0x12b
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        mov    word ptr [eax + 0x3e], 0x4342
        mov    dword ptr [0x33e], eax        ; -> obj5:0x033E
        mov    esi, dword ptr [0x149]        ; -> obj5:0x0149
        mov    ebx, dword ptr [esi + 0x14]
        mov    dword ptr [0x145], ebx        ; -> obj5:0x0145
        mov    edi, dword ptr [0x33e]        ; -> obj5:0x033E
        call   sub_4_09CB
        test   byte ptr [ebx + 0x38], 0x40
        je     loc_4_2A3D
        lea    eax, [0x2207]                 ; -> loc_1_2207
        mov    dword ptr [edi + 8], eax
        mov    esi, edi
        call   sub_4_08A9
loc_4_2A3D:
        mov    edi, ebx
        mov    ecx, dword ptr [0x149]        ; -> obj5:0x0149
        movzx  ecx, byte ptr [ecx + 0x10]
loc_4_2A49:
        push   ecx
        mov    dx, word ptr [ebx + 0x49]
        mov    byte ptr [0x14d], 0           ; -> obj5:0x014D
        test   byte ptr [ebx + 0x38], 4
        je     loc_4_2A62
        mov    byte ptr [0x14d], 1           ; -> obj5:0x014D
loc_4_2A62:
        push   ebx
        call   sub_4_0538
        pop    ebx
        pop    ecx
        inc    ch
        dec    cl
        jne    loc_4_2A49
        mov    eax, dword ptr [0x33e]        ; -> obj5:0x033E
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        call   sub_4_1E68
        call   sub_4_1BF0
        call   sub_4_14EC
        mov    edi, dword ptr [0x149]        ; -> obj5:0x0149
        call   sub_4_2C57
loc_4_2A9B:
        mov    eax, 0
        jmp    loc_4_2C4D
loc_4_2AA5:
        cmp    dword ptr [esp + 0x1e8], 6
        je     loc_4_2B43
        cmp    dword ptr [esp + 0x1e8], 0xf
        jne    loc_4_2C4D
        lea    eax, [esp + 0x1e0]
        push   0
        push   dword ptr [esp + 0x1f4]
        push   dword ptr [esp + 0x1f8]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009003300
        add    esp, 0x10
        mov    esi, dword ptr [esp + 0x1e0]
        or     esi, esi
        je     loc_4_2A9B
        mov    esi, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [esp + 0x1f4]
loc_4_2AF7:
        or     esi, esi
        je     loc_4_2A9B
        test   byte ptr [esi + 0x21], 0x80
        je     loc_4_2B06
        cmp    dword ptr [esi + 0x70], ecx
        je     loc_4_2B0B
loc_4_2B06:
        mov    esi, dword ptr [esi + 0x6c]
        jmp    loc_4_2AF7
loc_4_2B0B:
        lea    edi, [esp + 0xe0]
        call   sub_4_2E82
        lea    eax, [0x55]                   ; -> obj5:0x0055
        lea    ecx, [esp + 0xe0]
        add    edx, 0x13
        push   0
        push   edx
        push   ecx
        push   1
        push   eax
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        or     eax, eax
        jmp    loc_4_2A9B
loc_4_2B43:
        mov    esi, dword ptr [esp + 0x1f4]
        lea    ebx, [esp + 0xdc]
        mov    byte ptr [esp + 0xdd], 0
        lea    ecx, [esp + 0xd8]
        mov    dword ptr [ecx], 1
        push   0
        push   ecx
        push   ebx
        push   3
        push   0x4a                          ; -> obj5:0x004A
        push   0
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     eax, eax
        jne    loc_4_2C4D
        xor    ebx, ebx
        mov    bl, byte ptr [esp + 0xdc]
        lea    eax, [esp + 0xc]
        push   0
        push   esi
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID_Size ; cd2006003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_2C4D
        lea    edi, [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        push   0
        push   eax
        push   edi
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007003300
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_2C4D
        mov    al, 0x5c
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, 1
loc_4_2BD0:
        repne scasb al, byte ptr es:[edi]
        dec    edx
        js     loc_4_2BD9
        mov    esi, edi
        jmp    loc_4_2BD0
loc_4_2BD9:
        mov    byte ptr [edi - 1], 0x2c
        mov    ecx, esi
        lea    esi, [0x6fb]                  ; -> obj5:0x06FB
        mov    esi, dword ptr [esi + ebx*4]
        call   sub_4_21E3
        sub    edi, ecx
        mov    esi, dword ptr [esp + 0x1f4]
        push   0
        push   edi
        push   ecx
        push   1
        push   0x6ea                         ; -> obj5:0x06EA
        push   0
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        mov    edi, 0x73d                    ; -> obj5:0x073D
        or     ebx, ebx
        je     loc_4_2C2A
        cmp    ebx, 0xa
        je     loc_4_2C2A
        mov    edi, 0x747                    ; -> obj5:0x0747
        cmp    ebx, 5
        je     loc_4_2C2A
        mov    edi, 0x735                    ; -> obj5:0x0735
loc_4_2C2A:
        mov    ebx, edi
        xor    ecx, ecx
        dec    ecx
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        dec    ecx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0x6f5                         ; -> obj5:0x06F5
        push   0
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
loc_4_2C4D:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 0x1d8
        ret    
sub_4_2C57:
        sub    esp, 0x114
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [edi + 0xc]
loc_4_2C63:
        or     esi, esi
        je     loc_4_2E78
        cmp    dword ptr [esi + 0x70], 0
        je     loc_4_2E70
        mov    byte ptr [esp + 0x1d], 0
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_4_2CD8
        lea    edx, [0x10]                   ; -> obj5:0x0010
        mov    al, byte ptr [esi + 0x7c]
        call   sub_4_2EFA
        mov    byte ptr [esp + 0x1c], al
        lea    ecx, [esp + 0x1c]
        push   0
        push   2
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        lea    edx, [0x1d]                   ; -> obj5:0x001D
        mov    al, byte ptr [esi + 0x7d]
        call   sub_4_2EFA
        mov    byte ptr [esp + 0x1c], al
        lea    ecx, [esp + 0x1c]
        push   0
        push   2
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
loc_4_2CD8:
        push   esi
        lea    esi, [esi + 0x8a]
        lea    edi, [esp + 0x24]
        mov    ecx, 0x1c
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        lea    edx, [0x25]                   ; -> obj5:0x0025
        lea    ecx, [esp + 0x20]
        add    ecx, 0x18
        mov    byte ptr [ecx + 4], 0
        push   0
        push   5
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        lea    edx, [0x33]                   ; -> obj5:0x0033
        lea    ecx, [esp + 0x20]
        add    ecx, 8
        mov    byte ptr [ecx + 0x10], 0
        push   0
        push   0x11
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        lea    edx, [0x3d]                   ; -> obj5:0x003D
        lea    ecx, [esp + 0x20]
        mov    byte ptr [ecx + 8], 0
        push   0
        push   9
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        lea    edx, [0x4a]                   ; -> obj5:0x004A
        mov    al, byte ptr [esi + 0x40]
        mov    byte ptr [esp + 0x1c], al
        lea    ecx, [esp + 0x1c]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        mov    byte ptr [esp + 0x1c], 0
        cmp    byte ptr [esi + 0x40], 0xa
        jne    loc_4_2D94
        mov    byte ptr [esp + 0x1c], 2
loc_4_2D94:
        test   byte ptr [esi + 0x20], 1
        je     loc_4_2D9F
        mov    byte ptr [esp + 0x1c], 1
loc_4_2D9F:
        cmp    byte ptr [esp + 0x1c], 0
        je     loc_4_2DC6
        lea    edx, [0x87]                   ; -> obj5:0x0087
        lea    ecx, [esp + 0x1c]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
loc_4_2DC6:
        test   byte ptr [esi + 0x16], 0x40
        je     loc_4_2DF1
        mov    byte ptr [esp + 0x1c], 1
        lea    edx, [0x8d]                   ; -> obj5:0x008D
        lea    ecx, [esp + 0x1c]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
loc_4_2DF1:
        test   byte ptr [esi + 0x20], 4
        je     loc_4_2E1C
        lea    edx, [0x60]                   ; -> obj5:0x0060
        mov    byte ptr [esp + 0x1c], 1
        lea    ecx, [esp + 0x1c]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
loc_4_2E1C:
        mov    dword ptr [esp + 0x18], 0
        mov    ecx, esi
loc_4_2E26:
        or     ecx, ecx
        je     loc_4_2E46
        cmp    byte ptr [ecx + 0x46], 0
        je     loc_4_2E41
        mov    al, byte ptr [ecx + 0x14]
        add    al, 0x41
        mov    edx, dword ptr [esp + 0x18]
        mov    byte ptr [esp + edx + 0x20], al
        inc    dword ptr [esp + 0x18]
loc_4_2E41:
        mov    ecx, dword ptr [ecx + 0x10]
        jmp    loc_4_2E26
loc_4_2E46:
        mov    eax, dword ptr [esp + 0x18]
        mov    byte ptr [esp + eax + 0x20], 0
        lea    edx, [0x6a]                   ; -> obj5:0x006A
        lea    ecx, [esp + 0x20]
        push   0
        push   eax
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        jmp    loc_4_2E70
loc_4_2E70:
        mov    esi, dword ptr [esi + 0x6c]
        jmp    loc_4_2C63
loc_4_2E78:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 0x114
        ret    
sub_4_2E82:
        push   esi
        lea    esi, [esi + 0x8a]
        mov    ecx, 8
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        dec    edi
        cmp    byte ptr [edi], 0x20
        jne    loc_4_2EA7
        std    
        mov    al, 0x20
        mov    ecx, 8
        repe scasb al, byte ptr es:[edi]
        cld    
        add    edi, 3
        jmp    loc_4_2EB2
loc_4_2EA7:
        mov    ecx, 9
        inc    edi
        inc    edi
        mov    byte ptr [edi - 1], 0x20
loc_4_2EB2:
        push   esi
        lea    esi, [esi + 0x92]
        mov    edx, ecx
        mov    ecx, 0x10
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        mov    byte ptr [edi], 0
        ret    
loc_4_2EC7:
        mov    esi, dword ptr [edi + 4]
        mov    edx, dword ptr [esi + 0x70]
        or     edx, edx
        je     loc_4_2EF2
        lea    eax, [edi + 0x10]
        push   0
        push   eax
        push   dword ptr [edi + 0x14]
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 8]
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        cmp    eax, 0
        jne    loc_4_2EF2
loc_4_2EF1:
        ret    
loc_4_2EF2:
        mov    word ptr [edi + 2], 0xffff
        jmp    loc_4_2EF1
sub_4_2EFA:
        mov    cl, 0x30
        cmp    al, 0xa
        jb     loc_4_2F02
        mov    cl, 0x37
loc_4_2F02:
        add    al, cl
        ret    
sub_4_2F05:
        sub    esp, 8
        push   esi
        lea    eax, [esp + 8]
        push   0
        push   dword ptr [esi + 0x70]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID_Size ; cd2006003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_2FEC
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x22
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_2206
        push   eax
        mov    ebx, esi
        lea    esi, [0x6d8]                  ; -> obj5:0x06D8
        cmp    byte ptr [ebx + 0x74], 0
        jne    loc_4_2F56
        lea    esi, [0x6de]                  ; -> obj5:0x06DE
        jmp    loc_4_2F62
loc_4_2F56:
        cmp    byte ptr [ebx + 0x74], 2
        jne    loc_4_2F62
        lea    esi, [0x6e4]                  ; -> obj5:0x06E4
loc_4_2F62:
        mov    ecx, 5
        mov    edi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0x8a]
        mov    ecx, 0x19
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, 0x5c
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0xc]
        push   0
        push   eax
        push   edi
        push   dword ptr [ebx + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007003300
        add    esp, 0x10
        pop    esi
        xor    ecx, ecx
        or     eax, eax
        jne    loc_4_2FEC
        lea    edi, [esi + 0x1f]
        mov    al, 0x5c
        mov    ecx, dword ptr [esp + 8]
        mov    edx, ecx
loc_4_2FA1:
        repne scasb al, byte ptr es:[edi]
        jecxz  loc_4_2FAB
        mov    byte ptr [edi - 1], 0x26
        jmp    loc_4_2FA1
loc_4_2FAB:
        lea    eax, [esi + 0x1f]
        add    eax, edx
        movzx  ecx, byte ptr [ebx + 0xba]
        cmp    byte ptr [ebx + 0x74], 1
        jne    loc_4_2FC1
        mov    cx, word ptr [ebx + 0x7c]
loc_4_2FC1:
        add    cx, 0x3030
        mov    word ptr [eax], cx
        lea    edi, [esp + 4]
        push   0
        push   dword ptr [ebx + 0x70]
        push   esi
        push   edi
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b003300
        add    esp, 0x10
        xor    ecx, ecx
        or     eax, eax
        je     loc_4_2FE8
        cmp    eax, 0x10
        jne    loc_4_2FEC
loc_4_2FE8:
        mov    ecx, dword ptr [esp + 4]
loc_4_2FEC:
        pop    esi
        mov    dword ptr [esi + 0x70], ecx
        add    esp, 8
        ret    
sub_4_2FF4:
        mov    eax, 1
        VMMCall  VMM.EnableDisable_Boot_Log      ; cd208b010100
        VMMCall  VMM.Open_Boot_Log               ; cd2088010100
        or     eax, eax
        jne    loc_4_3068
        pushal 
        lea    edi, [0x1fd]                  ; -> obj5:0x01FD
        mov    edx, edi
        call   sub_4_21E3
        dec    edi
        mov    esi, dword ptr [0x1f9]        ; -> obj5:0x01F9
        call   sub_4_21E3
        mov    dword ptr [edi - 1], 0xa0d
        inc    edi
        mov    ecx, edi
        sub    ecx, edx
        VMMCall  VMM.Write_Boot_Log              ; cd2089010100
        VMMCall  VMM.Close_Boot_Log              ; cd208a010100
        mov    esi, dword ptr [0x2225]       ; -> loc_1_2225
        movzx  eax, byte ptr [esi + 0x69]
        dec    eax
        call   sub_1_128D
        jb     loc_4_3067
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_4_3067
        push   0
        push   eax
        VxDCall  IFSMgr._VolFlush                ; cd204c004000
        add    esp, 8
        or     eax, eax
        jne    loc_4_223C
loc_4_3067:
        popal  
loc_4_3068:
        xor    eax, eax
        VMMCall  VMM.EnableDisable_Boot_Log      ; cd208b010100
        ret    
        db  000h, 000h, 000h
loc_4_3074:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 7
        jne    loc_4_3082
        call   sub_4_308D
loc_4_3082:
        xor    eax, eax
        leave  
        ret    
sub_4_3086:
        mov    byte ptr [8], 0xee            ; -> obj5:0x0008
sub_4_308D:
        push   ebx
        mov    bl, 1
        xchg   byte ptr [8], bl              ; -> obj5:0x0008
        cmp    byte ptr [0xe], 0             ; -> obj5:0x000E
        jne    loc_4_30B3
        push   8                             ; -> obj5:0x0008
        push   1
        VxDCall  VPEND.Service_0007h             ; cd2007002600
        add    esp, 8
        or     eax, eax
        jne    loc_4_3101
loc_4_30B3:
        mov    al, byte ptr [8]              ; -> obj5:0x0008
        mov    ecx, dword ptr [0]            ; -> obj5:0x0000
        cmp    al, 0
        je     loc_4_30CC
        cmp    al, 2
        je     loc_4_30CC
        mov    ecx, dword ptr [4]            ; -> obj5:0x0004
loc_4_30CC:
        cmp    al, bl
        je     loc_4_3101
        shr    ecx, 2
        mov    dword ptr [0x2220], ecx       ; -> obj1:0x2220
        mov    edx, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        mov    edx, dword ptr [edx + 0x30]
loc_4_30E2:
        or     edx, edx
        je     loc_4_3101
        test   byte ptr [edx + 0x21], 0x80
        je     loc_4_30FC
        test   byte ptr [edx + 0x23], 0x80
        je     loc_4_30FC
        mov    dword ptr [edx + 0xbf], 0
loc_4_30FC:
        mov    edx, dword ptr [edx + 0xc]
        jmp    loc_4_30E2
loc_4_3101:
        pop    ebx
        ret    
loc_4_3103:
        jecxz  loc_4_3127
        dec    ecx
        cmp    ecx, 1
        mov    eax, 1
        ja     loc_4_311F
        call   sub_5_0750
        or     eax, eax
        jne    loc_4_311F
        ret    
loc_4_311F:
        mov    dword ptr [esi + 0x20], 0
        ret    
loc_4_3127:
        mov    ecx, 0x30a
        xor    eax, eax
        ret    
loc_4_312F:
        cmp    dword ptr [esi + 0x14], 4
        mov    eax, 0xd
        jb     loc_4_3172
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [eax], 0
        mov    eax, 0xf
        jne    loc_4_3172
        cmp    dword ptr [esi + 0x1c], 0xc
        mov    eax, 0x7a
        jb     loc_4_3172
        push   edx
        mov    edx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [0]            ; -> obj5:0x0000
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [4]            ; -> obj5:0x0004
        mov    dword ptr [edx + 8], eax
        xor    eax, eax
        mov    dword ptr [edx], eax
        mov    dword ptr [esi + 0x20], 0xc
        pop    edx
loc_4_3172:
        ret    
loc_4_3173:
        cmp    dword ptr [esi + 0x14], 0xc
        mov    eax, 0x7a
        jb     loc_4_31AC
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [eax], 0
        mov    eax, 0xf
        jne    loc_4_31AC
        push   edx
        push   ecx
        mov    edx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [0], eax            ; -> obj5:0x0000
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [4], eax            ; -> obj5:0x0004
        call   sub_4_3086
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        pop    ecx
        pop    edx
loc_4_31AC:
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x758  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 043h, 053h, 049h, 054h, 061h, 072h, 067h, 065h, 074h, 049h, 044h, 000h, 053h, 043h, 053h
        db  049h, 04Ch, 055h, 04Eh, 000h, 052h, 065h, 076h, 069h, 073h, 069h, 06Fh, 06Eh, 04Ch, 065h, 076h
        db  065h, 06Ch, 000h, 050h, 072h, 06Fh, 064h, 075h, 063h, 074h, 049h, 064h, 000h, 04Dh, 061h, 06Eh
        db  075h, 066h, 061h, 063h, 074h, 075h, 072h, 065h, 072h, 000h, 044h, 065h, 076h, 069h, 063h, 065h
        db  054h, 079h, 070h, 065h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h
        db  052h, 065h, 06Dh, 06Fh, 076h, 061h, 062h, 06Ch, 065h, 000h, 043h, 075h, 072h, 072h, 065h, 06Eh
        db  074h, 044h, 072h, 069h, 076h, 065h, 04Ch, 065h, 074h, 074h, 065h, 072h, 041h, 073h, 073h, 069h
        db  067h, 06Eh, 06Dh, 065h, 06Eh, 074h, 000h, 049h, 06Eh, 074h, 031h, 033h, 000h, 050h, 04Dh, 06Fh
        db  064h, 065h, 049h, 06Eh, 074h, 031h, 033h, 000h, 055h, 073h, 065h, 072h, 044h, 072h, 069h, 076h
        db  065h, 04Ch, 065h, 074h, 074h, 065h, 072h, 041h, 073h, 073h, 069h, 067h, 06Eh, 06Dh, 065h, 06Eh
        db  074h, 000h, 046h, 06Fh, 072h, 063h, 065h, 04Ch, 06Fh, 061h, 064h, 050h, 044h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 049h, 06Eh, 069h, 074h, 069h, 06Eh, 067h, 020h, 000h, 049h, 06Eh, 069h
        db  074h, 020h, 053h, 075h, 063h, 063h, 065h, 073h, 073h, 020h, 000h, 049h, 06Eh, 069h, 074h, 020h
        db  046h, 061h, 069h, 06Ch, 075h, 072h, 065h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 04Fh, 053h
        db  055h, 042h, 053h, 059h, 053h, 05Ch, 02Ah, 02Eh, 076h, 078h, 064h, 000h, 02Ah, 02Eh, 06Dh, 070h
        db  064h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 04Fh, 053h, 02Eh, 04Ch, 04Fh, 047h, 000h
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 031h, 001h
        dd  obj5:0x032E              ; fixup
        db  000h, 002h, 009h, 001h, 01Eh, 00Ah, 033h, 001h
        dd  obj5:0x032F              ; fixup
        db  000h, 002h, 009h, 001h, 01Dh, 000h, 031h, 001h
        dd  obj5:0x032E              ; fixup
        db  000h, 002h, 009h, 001h, 001h, 001h, 02Bh, 001h
        dd  obj5:0x0330              ; fixup
        db  001h, 000h, 000h, 000h, 002h, 002h, 031h, 001h
        dd  obj5:0x0331              ; fixup
        db  001h, 000h, 000h, 000h, 003h, 003h, 02Bh, 001h
        dd  obj5:0x0332              ; fixup
        db  001h, 000h, 000h, 000h, 004h, 004h, 02Bh, 001h
        dd  obj5:0x0333              ; fixup
        db  001h, 000h, 000h, 001h, 005h, 005h, 0A8h, 004h
        dd  obj5:0x0334              ; fixup
        db  000h, 008h, 00Bh, 000h, 006h, 006h, 02Bh, 001h
        dd  obj5:0x0335              ; fixup
        db  001h, 000h, 000h, 000h, 007h, 000h, 02Bh, 001h
        dd  obj5:0x0336              ; fixup
        db  000h, 002h, 009h, 000h, 008h, 008h, 02Bh, 001h
        dd  obj5:0x0337              ; fixup
        db  001h, 000h, 000h, 000h, 01Fh, 000h, 031h, 001h
        dd  obj5:0x0339              ; fixup
        db  001h, 000h, 000h, 000h, 054h, 068h, 065h, 020h, 049h, 02Fh, 04Fh, 020h, 073h, 075h, 062h, 073h
        db  079h, 073h, 074h, 065h, 06Dh, 020h, 063h, 06Fh, 075h, 06Ch, 064h, 020h, 06Eh, 06Fh, 074h, 020h
        db  061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 06Eh, 065h, 065h, 064h, 065h, 064h, 020h
        db  06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 02Eh, 020h, 00Dh, 00Ah, 059h, 06Fh, 075h, 020h, 06Eh, 065h
        db  065h, 064h, 020h, 074h, 06Fh, 020h, 072h, 075h, 06Eh, 020h, 073h, 065h, 074h, 075h, 070h, 02Eh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  072h, 06Dh, 06Dh, 02Eh, 070h, 064h, 072h, 000h, 062h, 069h, 067h, 06Dh, 065h, 06Dh, 02Eh, 064h
        db  072h, 076h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 073h, 063h, 073h, 069h, 070h, 06Fh, 072h, 074h
        db  02Eh, 070h, 064h, 072h, 000h, 050h, 04Fh, 052h, 054h, 044h, 052h, 049h, 056h, 045h, 052h, 000h
        db  041h, 053h, 050h, 049h, 043h, 041h, 04Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 043h, 053h, 049h, 05Ch, 000h, 045h, 053h
        db  044h, 049h, 05Ch, 000h, 046h, 04Ch, 04Fh, 050h, 05Ch, 000h, 048h, 061h, 072h, 064h, 077h, 061h
        db  072h, 065h, 049h, 044h, 000h, 043h, 06Ch, 061h, 073h, 073h, 000h
        dd  obj5:0x0727              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x072F              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0735              ; fixup
        dd  obj5:0x0727              ; fixup
        db  047h, 065h, 06Eh, 044h, 069h, 073h, 06Bh, 000h, 047h, 065h, 06Eh, 043h, 044h, 000h, 055h, 06Eh
        db  06Bh, 06Eh, 06Fh, 077h, 06Eh, 000h, 044h, 069h, 073h, 06Bh, 044h, 072h, 069h, 076h, 065h, 000h
        db  043h, 044h, 052h, 04Fh, 04Dh, 000h, 000h, 000h, 000h
sub_5_0750:
        das    
        xor    dword ptr [eax], eax
        add    byte ptr [ebx + 0x31], dh     ; -> loc_4_3173
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0xe28  R-X
        db  066h, 089h, 00Eh, 0A9h, 00Dh, 0C6h, 006h, 025h, 004h, 000h, 0C6h, 006h, 026h, 004h, 000h, 0C6h
        db  006h, 09Fh, 00Bh, 000h, 0F7h, 0C3h, 003h, 000h, 074h, 006h, 0B8h, 001h, 080h, 0E9h, 0D9h, 000h
        db  03Dh, 00Ah, 003h, 073h, 006h, 0B8h, 001h, 000h, 0E9h, 0CEh, 000h, 066h, 033h, 0F6h, 0B8h, 000h
        db  03Dh, 0BAh, 0AFh, 00Dh, 0CDh, 021h, 072h, 032h, 08Bh, 0D8h, 0B8h, 000h, 044h, 0CDh, 021h, 072h
        db  025h, 0F7h, 0C2h, 080h, 000h, 074h, 01Fh, 0BAh, 0B8h, 00Dh, 0B9h, 02Ch, 000h, 0B8h, 002h, 044h
        db  0CDh, 021h, 072h, 012h, 080h, 03Eh, 0C6h, 00Dh, 003h, 072h, 00Bh, 080h, 03Eh, 0C1h, 00Dh, 000h
        db  074h, 004h, 066h, 083h, 0CEh, 001h, 0B4h, 03Eh, 0CDh, 021h, 01Eh, 006h, 0BBh, 0FEh, 000h, 08Bh
        db  0D3h, 0B4h, 013h, 0CDh, 02Fh, 006h, 050h, 0B8h, 070h, 000h, 08Eh, 0C0h, 026h, 081h, 03Eh, 0B4h
        db  000h, 0FEh, 000h, 075h, 016h, 026h, 081h, 03Eh, 006h, 001h, 0FEh, 000h, 074h, 01Ah, 066h, 083h
        db  0CEh, 004h, 026h, 081h, 03Eh, 0B0h, 000h, 0FEh, 000h, 074h, 00Dh, 058h, 007h, 0B4h, 013h, 0CDh
        db  02Fh, 007h, 01Fh, 0B8h, 001h, 000h, 0EBh, 051h, 058h, 007h, 0B4h, 013h, 0CDh, 02Fh, 066h, 083h
        db  0CEh, 002h, 007h, 01Fh, 0B8h, 04Bh, 035h, 0CDh, 021h, 08Ch, 0C1h, 0E3h, 010h, 081h, 0F9h, 000h
        db  0F0h, 074h, 00Ah, 081h, 0F9h, 000h, 0E0h, 074h, 004h, 066h, 083h, 0CEh, 008h, 066h, 089h, 036h
        db  09Ah, 00Bh, 0E8h, 02Dh, 000h, 0E8h, 08Ah, 000h, 0E8h, 0A2h, 000h, 0E8h, 033h, 002h, 08Dh, 036h
        db  025h, 004h, 0B8h, 006h, 000h, 0B9h, 07Bh, 007h, 0BAh, 001h, 000h, 0FFh, 01Eh, 0A9h, 00Dh, 066h
        db  08Bh, 0D0h, 0B8h, 001h, 000h, 072h, 002h, 033h, 0C0h, 033h, 0DBh, 033h, 0F6h, 0C3h, 0F9h, 0CAh
        db  002h, 000h, 01Eh, 0B8h, 011h, 016h, 033h, 0DBh, 0CDh, 02Fh, 01Fh, 080h, 0E3h, 007h, 088h, 01Eh
        db  023h, 00Eh, 075h, 04Dh, 01Eh, 068h, 024h, 00Eh, 01Eh, 068h, 0E4h, 00Dh, 066h, 0FFh, 036h, 019h
        db  00Eh, 0B8h, 001h, 001h, 0FFh, 01Eh, 0A9h, 00Dh, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 032h, 01Eh
        db  068h, 01Dh, 00Eh, 01Eh, 068h, 023h, 00Eh, 01Eh, 068h, 021h, 00Eh, 06Ah, 000h, 06Ah, 000h, 01Eh
        db  068h, 00Eh, 00Eh, 066h, 0FFh, 036h, 024h, 00Eh, 0B8h, 009h, 001h, 0FFh, 01Eh, 0A9h, 00Dh, 083h
        db  0C4h, 018h, 066h, 0FFh, 036h, 024h, 00Eh, 0B8h, 002h, 001h, 0FFh, 01Eh, 0A9h, 00Dh, 083h, 0C4h
        db  004h, 0C3h, 050h, 006h, 0B8h, 070h, 000h, 08Eh, 0C0h, 067h, 026h, 0F7h, 005h, 008h, 000h, 000h
        db  000h, 004h, 000h, 074h, 005h, 0C6h, 006h, 0ADh, 00Dh, 001h, 007h, 058h, 0C3h, 08Dh, 006h, 027h
        db  004h, 0A3h, 0A7h, 00Bh, 08Ah, 016h, 0A0h, 00Bh, 08Bh, 036h, 0A7h, 00Bh, 088h, 054h, 005h, 0E8h
        db  075h, 001h, 00Fh, 082h, 08Fh, 000h, 08Ch, 0C0h, 00Bh, 0C0h, 00Fh, 084h, 087h, 000h, 08Ah, 016h
        db  0ADh, 00Dh, 038h, 016h, 0A0h, 00Bh, 074h, 07Dh, 0FEh, 006h, 025h, 004h, 0C6h, 004h, 001h, 066h
        db  0C7h, 044h, 001h, 01Dh, 000h, 000h, 000h, 088h, 05Ch, 00Ch, 089h, 07Ch, 019h, 08Ch, 044h, 01Bh
        db  08Ah, 0C1h, 086h, 0CDh, 080h, 0E5h, 0C0h, 0C0h, 0EDh, 006h, 066h, 041h, 089h, 04Ch, 006h, 066h
        db  083h, 0E0h, 03Fh, 089h, 044h, 00Ah, 066h, 00Fh, 0B6h, 0C6h, 066h, 040h, 089h, 044h, 008h, 066h
        db  056h, 087h, 0F7h, 01Eh, 006h, 08Dh, 07Dh, 00Eh, 08Ch, 0D9h, 08Ch, 0C0h, 08Eh, 0D8h, 08Eh, 0C1h
        db  066h, 0B9h, 00Bh, 000h, 000h, 000h, 0F3h, 0A4h, 007h, 01Fh, 066h, 05Eh, 0C6h, 044h, 00Dh, 000h
        db  0B4h, 015h, 08Ah, 016h, 0A0h, 00Bh, 0CDh, 013h, 072h, 009h, 080h, 0FCh, 002h, 075h, 004h, 080h
        db  04Ch, 00Dh, 001h, 083h, 006h, 0A7h, 00Bh, 01Dh, 0FEh, 006h, 0A0h, 00Bh, 080h, 03Eh, 0A0h, 00Bh
        db  018h, 00Fh, 086h, 05Fh, 0FFh, 08Ah, 016h, 0A0h, 00Bh, 088h, 016h, 09Eh, 00Bh, 0C6h, 006h, 0A0h
        db  00Bh, 080h, 006h, 0B9h, 070h, 000h, 08Eh, 0C1h, 026h, 08Ah, 016h, 002h, 000h, 007h, 088h, 016h
        db  0AEh, 00Dh, 08Ah, 016h, 0A0h, 00Bh, 08Ah, 00Eh, 09Fh, 00Bh, 038h, 00Eh, 0AEh, 00Dh, 00Fh, 086h
        db  0A6h, 000h, 0E8h, 0B2h, 000h, 00Fh, 082h, 0A8h, 000h, 08Ah, 016h, 0A0h, 00Bh, 08Bh, 036h, 0A7h
        db  00Bh, 088h, 054h, 005h, 08Ah, 0C1h, 086h, 0CDh, 080h, 0E5h, 0C0h, 0C0h, 0EDh, 006h, 00Bh, 0C9h
        db  00Fh, 084h, 08Dh, 000h, 066h, 041h, 0FEh, 006h, 09Fh, 00Bh, 089h, 04Ch, 006h, 066h, 083h, 0E0h
        db  03Fh, 089h, 044h, 00Ah, 066h, 00Fh, 0B6h, 0C6h, 066h, 040h, 089h, 044h, 008h, 0C6h, 004h, 000h
        db  066h, 0C7h, 044h, 001h, 00Dh, 000h, 000h, 000h, 0C6h, 044h, 00Ch, 000h, 0FEh, 006h, 025h, 004h
        db  083h, 006h, 0A7h, 00Bh, 00Dh, 080h, 03Eh, 023h, 00Eh, 000h, 075h, 03Fh, 0BBh, 0AAh, 055h, 0B4h
        db  041h, 08Ah, 016h, 0A0h, 00Bh, 0CDh, 013h, 072h, 032h, 081h, 0FBh, 055h, 0AAh, 075h, 02Ch, 0F6h
        db  0C1h, 001h, 074h, 027h, 056h, 08Bh, 036h, 0A7h, 00Bh, 0C7h, 004h, 01Ah, 000h, 0C7h, 044h, 002h
        db  000h, 000h, 0B4h, 048h, 08Ah, 016h, 0A0h, 00Bh, 0CDh, 013h, 05Eh, 072h, 00Eh, 080h, 04Ch, 00Ch
        db  001h, 066h, 083h, 044h, 001h, 01Ah, 083h, 006h, 0A7h, 00Bh, 01Ah, 0FEh, 006h, 0A0h, 00Bh, 080h
        db  03Eh, 0A0h, 00Bh, 097h, 00Fh, 086h, 04Ah, 0FFh, 066h, 0C7h, 044h, 001h, 000h, 000h, 000h, 000h
        db  0C3h, 0FEh, 006h, 0A0h, 00Bh, 0EBh, 0E8h, 066h, 033h, 0FFh, 08Eh, 0C7h, 0B4h, 008h, 0CDh, 013h
        db  0C3h, 08Dh, 006h, 087h, 00Ah, 0A3h, 0A1h, 00Bh, 0C6h, 006h, 0A0h, 00Bh, 080h, 080h, 03Eh, 025h
        db  004h, 000h, 00Fh, 084h, 0F7h, 000h, 08Dh, 03Eh, 027h, 004h, 0A0h, 0A0h, 00Bh, 080h, 03Dh, 000h
        db  075h, 005h, 038h, 045h, 005h, 074h, 00Eh, 066h, 083h, 07Dh, 001h, 000h, 00Fh, 084h, 0D0h, 000h
        db  003h, 07Dh, 001h, 0EBh, 0E8h, 0B8h, 001h, 002h, 01Eh, 007h, 08Dh, 01Eh, 0A9h, 00Bh, 0B9h, 001h
        db  000h, 066h, 033h, 0D2h, 08Ah, 016h, 0A0h, 00Bh, 0CDh, 013h, 00Fh, 082h, 0B2h, 000h, 0FEh, 006h
        db  026h, 004h, 08Bh, 03Eh, 0A1h, 00Bh, 0C7h, 045h, 009h, 000h, 000h, 08Ah, 00Eh, 0A0h, 00Bh, 088h
        db  00Dh, 08Dh, 036h, 0A9h, 00Bh, 083h, 0BCh, 0DAh, 000h, 000h, 075h, 077h, 066h, 083h, 0BCh, 0DCh
        db  000h, 000h, 075h, 056h, 0B4h, 002h, 0CDh, 01Ah, 066h, 0C1h, 0C1h, 010h, 08Ah, 0EEh, 08Ah, 00Eh
        db  0A0h, 00Bh, 066h, 089h, 08Ch, 0DCh, 000h, 066h, 089h, 00Eh, 0A3h, 00Bh, 0B8h, 001h, 003h, 01Eh
        db  007h, 08Dh, 01Eh, 0A9h, 00Bh, 0B9h, 001h, 000h, 066h, 033h, 0D2h, 08Ah, 016h, 0A0h, 00Bh, 0CDh
        db  013h, 072h, 040h, 0B8h, 001h, 002h, 01Eh, 007h, 08Dh, 01Eh, 0A9h, 00Bh, 0B9h, 001h, 000h, 066h
        db  033h, 0D2h, 08Ah, 016h, 0A0h, 00Bh, 0CDh, 013h, 072h, 046h, 08Dh, 036h, 0A9h, 00Bh, 066h, 08Bh
        db  00Eh, 0A3h, 00Bh, 066h, 03Bh, 08Ch, 0DCh, 000h, 075h, 019h, 08Bh, 03Eh, 0A1h, 00Bh, 066h, 08Bh
        db  08Ch, 0DCh, 000h, 066h, 089h, 04Dh, 001h, 083h, 006h, 0A1h, 00Bh, 00Bh, 0FEh, 006h, 0A0h, 00Bh
        db  0E9h, 02Ah, 0FFh, 066h, 033h, 0D2h, 0BBh, 000h, 002h, 066h, 003h, 050h, 0FCh, 083h, 0EBh, 004h
        db  075h, 0F7h, 08Bh, 03Eh, 0A1h, 00Bh, 083h, 04Dh, 009h, 001h, 066h, 089h, 055h, 005h, 0EBh, 0CAh
        db  0FEh, 006h, 0A0h, 00Bh, 080h, 03Eh, 0A0h, 00Bh, 084h, 00Fh, 082h, 000h, 0FFh, 08Bh, 036h, 0A1h
        db  00Bh, 0C6h, 004h, 0FFh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 053h
        db  04Dh, 041h, 052h, 054h, 041h, 041h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h
        db  06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 053h, 065h, 072h
        db  076h, 069h, 063h, 065h, 073h, 05Ch, 056h, 078h, 044h, 05Ch, 049h, 04Fh, 053h, 000h, 04Eh, 06Fh
        db  045h, 078h, 074h, 049h, 06Eh, 074h, 031h, 033h, 000h, 002h, 000h, 000h, 080h, 001h, 000h, 000h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x138  R-X
loc_7_0000:
        push   esi
        mov    esi, dword ptr [0x1f9c]       ; -> obj1:0x1F9C
        test   byte ptr [esi + 0x2a], 1
        jne    loc_7_000E
        stc    
loc_7_000E:
        pop    esi
        ret    
loc_7_0010:
        sub    esp, 4
        mov    esi, esp
        mov    byte ptr [esi], 0xa
        mov    al, byte ptr [ebp + 0x14]
        mov    byte ptr [esi + 2], al
        push   esi
        VxDCall  BLOCKDEV.Service_0008h          ; cd2008001000
        add    esp, 4
        cmp    byte ptr [esi + 1], 0
        jne    loc_7_003B
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebp + 0x14], ax
        add    esp, 4
        clc    
        ret    
loc_7_003B:
        add    esp, 4
        stc    
        ret    
loc_7_0040:
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        lea    esi, [0x545]                  ; -> obj5:0x0545
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [0x544]          ; -> obj5:0x0544
        mov    dword ptr [ebp + 0x14], eax
        ret    
loc_7_0061:
        push   ebp
        movzx  eax, byte ptr [ebp + 0x14]
        call   sub_1_128D
        or     ebx, ebx
        je     loc_7_00EC
        mov    ebx, dword ptr [ebx]
        mov    ebp, ebx
        mov    esi, dword ptr [ebp + 4]
        add    esi, 0x64
        sub    esp, 0x10
        mov    ebx, esp
        mov    dword ptr [ebx + 6], esi
        add    esi, 0x58
        mov    word ptr [ebx + 4], si
        mov    byte ptr [ebx + 0xe], 2
        mov    word ptr [ebx], 2
        push   ebx
        call   sub_1_1310
        mov    ebx, dword ptr [ebx + 0xa]
        add    esp, 0x14
        add    ebx, dword ptr [ebp + 4]
        mov    word ptr [ebx + 0x68], 0xc
        mov    byte ptr [ebx + 0xb1], al
        xor    eax, eax
        mov    dword ptr [ebx + 0x64], eax
        mov    dword ptr [ebx + 0xa8], eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [ebx + 0x6c], 0x500
        mov    edi, ebx
        add    ebx, 0x64
        xchg   esi, ebx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        mov    ax, word ptr [edi + 0x6a]
        mov    edi, dword ptr [edi + 0x88]
        call   sub_1_0A1E
        cmp    ax, 0x10
        jae    loc_7_00EC
        clc    
loc_7_00EA:
        pop    ebp
        ret    
loc_7_00EC:
        stc    
        jmp    loc_7_00EA
        db  000h
loc_7_00F0:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edi], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], dl
        dec    esi
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebx + 0x79], dl
        jae    0x15f
        dec    ebp
        inc    ebx
        jo     0x18c
        sub    al, 0x44
        imul   esi, dword ptr [ebx + 0x70], 0x5279616c
        popal  
        insb   byte ptr es:[edi], dx
        dec    ebp
        outsd  dx, dword ptr [esi]
        push   edi
        popal  
        jb     0x1a2
        db  069h, 06Eh, 067h, 021h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 8  vsize=0x70  RW-
VxD__LDATA ENDS
