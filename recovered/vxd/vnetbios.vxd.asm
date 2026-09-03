; VNETBIOS -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 3119 instructions recovered by recursive descent

Declare_Virtual_Device VNETBIOS, 3, 0, VNETBIOS_Control, 00014h, Undefined_Init_Order
;   DDK version 0400h   init_order 054000000h   flags 00000h
;   service table: obj1:0x191c, 8 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1b78  R-X
        db  051h, 0B9h
        dd  obj1:0x190C              ; fixup
        db  085h, 0C0h, 074h, 005h, 0B9h
        dd  obj1:0x1910              ; fixup
        db  08Bh, 001h, 085h, 0C0h, 074h, 008h, 0FFh, 070h, 0FCh, 08Fh, 001h, 059h, 0F8h, 0C3h, 0F9h, 059h
        db  0C3h
sub_1_0020:
        push   ecx
        mov    ecx, 0x190c                   ; -> obj1:0x190C
        bt     dword ptr [eax - 8], 9
        jae    loc_1_0032
        mov    ecx, 0x1910                   ; -> obj1:0x1910
loc_1_0032:
        push   dword ptr [ecx]
        mov    dword ptr [ecx], eax
        pop    dword ptr [eax - 4]
        pop    ecx
        ret    
sub_1_003B:
        jecxz  loc_1_005D
        test   esi, esi
        je     loc_1_005D
        pushal 
        test   byte ptr [eax - 7], 4
        jne    loc_1_0060
        test   byte ptr [ebx], 0x20
        jne    loc_1_0074
        movzx  edx, si
        shr    esi, 0x10
        shl    esi, 4
        add    esi, edx
        add    esi, dword ptr [ebx + 4]
        jmp    loc_1_0090
loc_1_005D:
        xor    esi, esi
        ret    
loc_1_0060:
        cmp    esi, 0x100000
        jae    loc_1_0090
        test   byte ptr [eax - 7], 2
        jne    loc_1_0096
        mov    edi, esi
        xor    esi, esi
        jmp    loc_1_00CA
loc_1_0074:
        movzx  edi, si
        shr    esi, 0x10
        push   ecx
        push   0
        push   esi
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    ecx
        lea    esi, [edi + eax]
        mov    eax, dword ptr [esp + 0x1c]
loc_1_0090:
        test   byte ptr [eax - 7], 2
        je     loc_1_00C2
loc_1_0096:
        mov    edi, esi
        and    edi, 0xfff
        add    ecx, edi
        dec    ecx
        shr    ecx, 0xc
        inc    ecx
        mov    dword ptr [esp + 4], ecx
        mov    dword ptr [esp], esi
        shr    esi, 0xc
        push   0
        push   ecx
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        je     loc_1_00DB
        popal  
        ret    
loc_1_00C2:
        VxDCall  V86MMGR.V86MMGR_Map_Pages       ; cd200d000600
        jb     loc_1_00DB
loc_1_00CA:
        mov    dword ptr [esp + 4], esi
        shl    edi, 0xc
        shr    di, 0xc
        mov    dword ptr [esp], edi
        popal  
        clc    
        ret    
loc_1_00DB:
        popal  
        xor    esi, esi
        mov    edi, esi
        stc    
        ret    
sub_1_00E2:
        test   esi, esi
        je     loc_1_00F2
        test   byte ptr [eax - 7], 2
        jne    loc_1_00F3
        VxDCall  V86MMGR.V86MMGR_Free_Page_Map_Region ; cd200e000600
loc_1_00F2:
        ret    
loc_1_00F3:
        push   eax
        push   ecx
        shr    edi, 0xc
        push   0
        push   esi
        push   edi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        pop    ecx
        pop    eax
        ret    
sub_1_0108:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        pop    edi
        ret    
sub_1_0113:
        push   ebx
        push   edi
        push   esi
        push   eax
        push   ecx
        mov    ecx, eax
        and    ecx, 0xfff
        add    ebx, ecx
        shr    eax, 0xc
        add    ebx, 0xfff
        shr    ebx, 0xc
        pop    ecx
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_015D
        cmp    eax, 0xfffffbad
        je     loc_1_0158
        pop    ebx
        and    ebx, 0xfff
        add    eax, ebx
        clc    
loc_1_0154:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_0158:
        int3   
        stc    
        pop    ebx
        jmp    loc_1_0154
loc_1_015D:
        int3   
        stc    
        pop    ebx
        jmp    loc_1_0154
sub_1_0162:
        push   ebx
        push   edi
        push   esi
        push   ecx
        mov    ecx, eax
        and    ecx, 0xfff
        add    ebx, ecx
        shr    eax, 0xc
        add    ebx, 0xfff
        shr    ebx, 0xc
        pop    ecx
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0196
        clc    
loc_1_0192:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_0196:
        int3   
        jmp    loc_1_0192
sub_1_0199:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        push   esi
        push   edi
        push   ebp
        mov    esi, ebx
        mov    edx, 0x5c
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0x23
        cmp    byte ptr [esi], 0x37
        je     loc_1_0216
        movzx  ecx, byte ptr [esi + 0x30]
        cmp    cl, 8
        jae    loc_1_0211
        mov    ecx, dword ptr [ecx*4 + 0x1a8c] ; -> obj1:0x1A8C
        jecxz  loc_1_01DA
        call   sub_1_0223
        jmp    loc_1_0211
loc_1_01DA:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        mov    ecx, 1
        call   sub_1_0F48
        mov    al, byte ptr [ebp + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_0211:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0216:
        call   sub_1_1722
        jmp    loc_1_0211
loc_1_021D:
        mov    eax, 0x22
        ret    
sub_1_0223:
        VxDCall  NDIS.Service_000Dh              ; cd200d002a00
        mov    ecx, eax
        call   0
        jb     loc_1_021D
        and    dword ptr [eax - 8], 0x200
        or     word ptr [eax - 8], 0xa20
        mov    dword ptr [eax - 0x10], esi
        mov    dword ptr [eax - 0x28], ecx
        push   eax
        call   sub_1_0108
        mov    ecx, eax
        pop    eax
        mov    dword ptr [eax - 0x20], ecx
        cmp    dword ptr [0x19dc], 1         ; -> loc_1_19DC
        jb     loc_1_0265
        cmp    dword ptr [0x19d8], 1         ; -> loc_1_19D8
        jb     loc_1_0265
        jmp    loc_1_026E
loc_1_0265:
        pushal 
        xor    ebx, ebx
        call   sub_1_0A57
        popal  
loc_1_026E:
        xor    ecx, ecx
        mov    dword ptr [eax - 0x18], ecx
        mov    dword ptr [eax - 0x1c], ecx
        mov    dword ptr [eax - 0x24], ecx
        mov    dword ptr [eax - 0xc], ebx
        mov    dword ptr [eax - 0x14], ecx
        xchg   esi, eax
        push   ebx
        mov    edi, eax
        mov    ebx, 0x40
        call   sub_1_0113
        pop    ebx
        xchg   esi, eax
        jb     loc_1_048B
        mov    dword ptr [eax - 0x14], esi
        mov    byte ptr [esi + 1], 0xff
        mov    byte ptr [esi + 0x31], 0xff
        mov    edx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        mov    ecx, eax
        xchg   dword ptr [edx + ebx], ecx
        mov    dword ptr [eax - 4], ecx
        mov    edi, eax
        cld    
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movzx  ecx, byte ptr [eax]
        and    cl, 0x7f
        mov    cl, byte ptr [ecx + 0x1a08]   ; -> obj1:0x1A08
        xor    esi, esi
        mov    edi, eax
        cmp    cl, 0
        je     loc_1_031A
        cmp    cl, 4
        je     loc_1_031A
        cmp    cl, 0x18
        je     loc_1_035E
        cmp    cl, 0x14
        jne    loc_1_02FD
        cmp    word ptr [edi + 0xa], 0
        je     loc_1_02FD
        mov    eax, dword ptr [edi + 0xc]
        movzx  ebx, word ptr [edi + 0xa]
        call   sub_1_0113
        jb     loc_1_03BF
        inc    esi
        mov    dword ptr [edi + 0xc], eax
loc_1_02FD:
        cmp    word ptr [edi + 8], 0
        je     loc_1_031A
        mov    eax, dword ptr [edi + 4]
        movzx  ebx, word ptr [edi + 8]
        call   sub_1_0113
        jb     loc_1_03CA
        inc    esi
        mov    dword ptr [edi + 4], eax
loc_1_031A:
        mov    dword ptr [edi - 0x24], esi
        test   byte ptr [edi], 0x80
        je     loc_1_0329
        mov    dword ptr [edi + 0x2c], 0x499 ; -> loc_1_0499
loc_1_0329:
        movzx  ecx, byte ptr [edi + 0x30]
        mov    ecx, dword ptr [ecx*4 + 0x1a8c] ; -> obj1:0x1A8C
        mov    ebx, edi
        push   ebx
        call   ecx
        pop    ebx
        test   byte ptr [ebx], 0x80
        je     loc_1_034F
        cmp    al, 0
        je     loc_1_034B
        cmp    al, 0xff
        jne    loc_1_03DD
loc_1_034B:
        xor    eax, eax
        clc    
        ret    
loc_1_034F:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 0xff
        jne    loc_1_03DD
        xor    eax, eax
        clc    
        ret    
loc_1_035E:
        mov    eax, dword ptr [edi + 4]
        mov    edx, dword ptr [edi - 0x28]
        mov    ecx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        mov    ecx, dword ptr [ebx + ecx]
loc_1_036D:
        jecxz  loc_1_037E
        cmp    dword ptr [ecx - 0x10], eax
        jne    loc_1_0379
        cmp    dword ptr [ecx - 0x28], edx
        je     loc_1_038A
loc_1_0379:
        mov    ecx, dword ptr [ecx - 4]
        jmp    loc_1_036D
loc_1_037E:
        mov    ebx, edi
        mov    al, 0x24
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 0x31], al
        jmp    loc_1_034F
loc_1_038A:
        mov    ebx, edi
        mov    dword ptr [ebx + 4], ecx
        push   ecx
        push   edi
        movzx  ecx, byte ptr [edi + 0x30]
        mov    ecx, dword ptr [ecx*4 + 0x1a8c] ; -> obj1:0x1A8C
        call   ecx
        pop    edi
        pop    ecx
        test   byte ptr [ecx], 0x80
        jne    loc_1_03BD
        test   word ptr [ecx - 8], 0x10f
        jne    loc_1_034F
        pushal 
        mov    edi, ecx
        call   sub_1_058C
        popal  
        xchg   ecx, eax
        call   sub_1_16A7
        xchg   ecx, eax
loc_1_03BD:
        jmp    loc_1_034F
loc_1_03BF:
        mov    eax, 1
        push   ebx
        push   eax
        mov    edx, esi
        jmp    loc_1_0425
loc_1_03CA:
        mov    eax, 1
        push   ebx
        push   eax
        mov    edx, esi
        jmp    loc_1_03F7
        db  0CCh, 0B8h, 0FFh, 000h, 000h, 000h, 090h, 0C3h
loc_1_03DD:
        xor    edx, edx
        mov    edi, ebx
        pushal 
        call   sub_1_058C
        popal  
        push   ebx
        push   eax
        xchg   dword ptr [edi - 0x24], edx
        cmp    edx, 0
        je     loc_1_0425
        cmp    edx, 1
        je     loc_1_040F
loc_1_03F7:
        dec    edx
        push   edx
        mov    eax, dword ptr [edi + 0xc]
        mov    edx, dword ptr [edi - 0x14]
        movzx  ebx, word ptr [edx + 0xa]
        call   sub_1_0162
        pop    edx
        jb     loc_1_0496
loc_1_040F:
        cmp    dl, 0
        je     loc_1_0425
        mov    edx, dword ptr [edi - 0x14]
        mov    eax, dword ptr [edi + 4]
        movzx  ebx, word ptr [edx + 8]
        call   sub_1_0162
        jb     loc_1_0496
loc_1_0425:
        mov    eax, dword ptr [edi - 0x14]
        pop    ecx
        mov    byte ptr [eax + 0x31], cl
        mov    byte ptr [eax + 1], cl
        push   ecx
        mov    cl, byte ptr [edi + 2]
        mov    byte ptr [eax + 2], cl
        mov    cl, byte ptr [edi + 3]
        mov    byte ptr [eax + 3], cl
        movzx  ecx, byte ptr [edi]
        and    cl, 0x7f
        mov    cl, byte ptr [ecx + 0x1a08]   ; -> obj1:0x1A08
        cmp    cl, 0x14
        jne    loc_1_0455
        mov    cx, word ptr [edi + 0xa]
        mov    word ptr [eax + 0xa], cx
loc_1_0455:
        mov    cx, word ptr [edi + 8]
        mov    word ptr [eax + 8], cx
        mov    cl, byte ptr [edi]
        cmp    cl, 0x33
        jne    loc_1_0470
        mov    ecx, dword ptr [edi + 0x1a]
        mov    dword ptr [eax + 0x1a], ecx
        mov    ecx, dword ptr [edi + 0x1e]
        mov    dword ptr [eax + 0x1e], ecx
loc_1_0470:
        mov    ecx, 4
        push   edi
        push   esi
        lea    esi, [edi + 0xa]
        lea    edi, [eax + 0xa]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    edi
        mov    ebx, 0x40
        call   sub_1_0162
loc_1_048B:
        mov    eax, edi
        call   sub_1_16A7
        pop    eax
        pop    ebx
        clc    
        ret    
loc_1_0496:
        int3   
        jmp    loc_1_0425
loc_1_0499:
        push   ebx
        push   eax
        push   edi
        push   esi
        or     byte ptr [ebx - 8], 2
        mov    esi, dword ptr [ebx - 0x14]
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [esi + 1], al
        mov    cl, byte ptr [ebx + 2]
        mov    byte ptr [esi + 2], cl
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [esi + 3], cl
        xor    esi, esi
        mov    edi, ebx
        pushal 
        call   sub_1_058C
        popal  
        push   ebx
        push   eax
        xchg   dword ptr [edi - 0x24], esi
        cmp    esi, 0
        je     loc_1_0504
        cmp    esi, 1
        je     loc_1_04E6
        dec    esi
        mov    eax, dword ptr [edi + 0xc]
        mov    edx, dword ptr [edi - 0x14]
        movzx  ebx, word ptr [edx + 0xa]
        call   sub_1_0162
        jb     loc_1_0586
loc_1_04E6:
        cmp    esi, 0
        je     loc_1_0425
        mov    eax, dword ptr [edi + 4]
        mov    edx, dword ptr [edi - 0x14]
        movzx  ebx, word ptr [edx + 8]
        call   sub_1_0162
        jb     loc_1_0586
loc_1_0504:
        mov    esi, dword ptr [edi - 0x14]
        movzx  ecx, byte ptr [esi]
        and    cl, 0x7f
        mov    cl, byte ptr [ecx + 0x1a08]   ; -> obj1:0x1A08
        cmp    cl, 0x14
        jne    loc_1_0520
        mov    cx, word ptr [edi + 0xa]
        mov    word ptr [esi + 0xa], cx
loc_1_0520:
        mov    cx, word ptr [edi + 8]
        mov    word ptr [esi + 8], cx
        mov    eax, dword ptr [edi - 0x14]
        mov    ecx, 4
        push   edi
        push   esi
        lea    esi, [edi + 0xa]
        lea    edi, [eax + 0xa]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    edi
        mov    ebx, 0x40
        mov    esi, dword ptr [esi + 0x2c]
        call   sub_1_0162
        mov    ecx, esi
        jecxz  loc_1_0566
        test   dword ptr [edi - 8], 0x1000
        jne    loc_1_0566
        push   dword ptr [edi - 0x20]
        push   dword ptr [edi - 0x10]
        push   esi
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
loc_1_0566:
        mov    eax, edi
        and    word ptr [eax - 8], 0xf7dd
        test   word ptr [eax - 8], 0x10f
        jne    loc_1_057E
        mov    ebx, dword ptr [edi - 0xc]
        call   sub_1_16A7
loc_1_057E:
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        pop    eax
        pop    ebx
        clc    
        ret    
loc_1_0586:
        int3   
        jmp    loc_1_0504
sub_1_058C:
        push   ecx
        xor    ch, ch
        mov    cl, byte ptr [edi]
        and    cl, 0x7f
        cmp    byte ptr [edi + 1], 0
        jne    loc_1_0644
        cmp    cl, 0x10
        je     loc_1_05CC
        inc    ch
        cmp    cl, 0x11
        je     loc_1_05CC
        inc    ch
        cmp    cl, 0x30
        je     loc_1_05CC
        inc    ch
        cmp    cl, 0x36
        je     loc_1_05CC
        inc    ch
        cmp    cl, 0x31
        je     loc_1_0644
        inc    ch
        cmp    cl, 0x12
        je     loc_1_0644
loc_1_05CA:
        pop    ecx
        ret    
loc_1_05CC:
        push   esi
        call   sub_1_0746
        or     esi, esi
        jne    loc_1_05D9
        pop    esi
        jmp    loc_1_05CA
loc_1_05D9:
        cmp    ch, 2
        jb     loc_1_0619
        dec    dword ptr [0x19dc]            ; -> loc_1_19DC
        push   ebx
        push   eax
        mov    eax, dword ptr [0x19d4]       ; -> loc_1_19D4
        mov    ebx, dword ptr [eax + 0x10]
        mov    dword ptr [0x19d4], ebx       ; -> loc_1_19D4
        mov    ebx, esi
        push   edi
        lea    esi, [edi + 0x1a]
        lea    edi, [eax]
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        mov    cl, byte ptr [edi + 0x30]
        mov    byte ptr [eax + 0x14], cl
        mov    esi, dword ptr [ebx + 4]
        mov    dword ptr [eax + 0x10], esi
        mov    dword ptr [ebx + 4], eax
        pop    eax
        pop    ebx
        pop    esi
        jmp    loc_1_05CA
loc_1_0619:
        push   edx
        mov    dl, byte ptr [edi + 0x30]
        call   sub_1_0707
        or     edx, edx
        je     loc_1_0640
        mov    ch, byte ptr [edi + 2]
        mov    cl, ch
        shr    cl, 3
        push   eax
        movzx  eax, cl
        add    edx, eax
        pop    eax
        mov    cl, ch
        and    cl, 7
        mov    ch, 1
        shl    ch, cl
        or     byte ptr [edx], ch
loc_1_0640:
        pop    edx
        pop    esi
        jmp    loc_1_05CA
loc_1_0644:
        push   esi
        call   sub_1_0746
        or     esi, esi
        jne    loc_1_064F
        int3   
loc_1_064F:
        cmp    ch, 0
        jne    loc_1_0699
        cmp    cl, 0x15
        je     loc_1_0669
        cmp    cl, 0x14
        je     loc_1_0669
        cmp    cl, 0x17
        je     loc_1_0669
        pop    esi
        jmp    loc_1_05CA
loc_1_0669:
        push   edx
        mov    dl, byte ptr [edi + 0x30]
        call   sub_1_0707
        or     edx, edx
        je     loc_1_0692
        mov    ch, byte ptr [edi + 2]
        mov    cl, ch
        shr    cl, 3
        push   eax
        movzx  eax, cl
        add    edx, eax
        pop    eax
        mov    cl, ch
        and    cl, 7
        mov    ch, 1
        shl    ch, cl
        not    ch
        and    byte ptr [edx], ch
loc_1_0692:
        pop    edx
        pop    esi
        jmp    loc_1_05CA
loc_1_0699:
        cmp    ch, 5
        je     loc_1_0669
        push   ebx
        push   eax
        mov    bl, byte ptr [edi + 0x30]
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_06EC
loc_1_06AC:
        cmp    byte ptr [eax + 0x14], bl
        jne    loc_1_06C6
        push   edi
        push   esi
        push   ecx
        lea    edi, [edi + 0x1a]
        lea    esi, [eax]
        mov    ecx, 4
        cld    
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    ecx
        pop    esi
        pop    edi
        je     loc_1_06D7
loc_1_06C6:
        mov    ecx, eax
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_06AC
        pop    eax
        pop    ebx
        pop    esi
        jmp    loc_1_05CA
loc_1_06D7:
        or     ecx, ecx
        jne    loc_1_06F1
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 4], ecx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_1_06EC:
        pop    eax
        pop    ebx
        pop    esi
        pop    ecx
        ret    
loc_1_06F1:
        mov    ebx, dword ptr [eax + 0x10]
        mov    dword ptr [ecx + 0x10], ebx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    eax
        pop    ebx
        pop    esi
        pop    ecx
        ret    
sub_1_0707:
        push   edi
        mov    edi, dword ptr [esi + 8]
        or     edi, edi
        je     loc_1_071B
loc_1_070F:
        cmp    byte ptr [edi + 0x24], dl
        je     loc_1_0742
        mov    edi, dword ptr [edi + 0x20]
        or     edi, edi
        jne    loc_1_070F
loc_1_071B:
        dec    dword ptr [0x19d8]            ; -> loc_1_19D8
        push   ebx
        mov    edi, dword ptr [0x19d0]       ; -> loc_1_19D0
        mov    ebx, dword ptr [edi + 0x20]
        mov    dword ptr [0x19d0], ebx       ; -> loc_1_19D0
        mov    ebx, dword ptr [esi + 8]
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [edi + 0x20], ebx
        mov    byte ptr [edi + 0x24], dl
        pop    ebx
        mov    edx, edi
        pop    edi
        ret    
loc_1_0742:
        mov    edx, edi
        pop    edi
        ret    
sub_1_0746:
        push   ebx
        mov    esi, dword ptr [0x19cc]       ; -> loc_1_19CC
        mov    ebx, dword ptr [edi - 0x28]
        or     esi, esi
        je     loc_1_075F
loc_1_0754:
        cmp    ebx, dword ptr [esi]
        je     loc_1_075F
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_1_0754
loc_1_075F:
        pop    ebx
        ret    
VNETBIOS_Get_Version:
        mov    eax, 0x30a
        mov    ebx, 1
        clc    
        ret    
VNETBIOS_Register2:
        movzx  eax, al
        cmp    al, 8
        jae    loc_1_07C0
        cmp    dword ptr [eax*4 + 0x1a8c], 0 ; -> obj1:0x1A8C
        jne    loc_1_07DD
        test   ebx, ebx
        jne    loc_1_078B
        bt     dword ptr [0x1ad1], eax       ; -> loc_1_1AD1
        jb     loc_1_07DD
loc_1_078B:
        btr    dword ptr [0x1ad1], eax       ; -> loc_1_1AD1
loc_1_0792:
        mov    dword ptr [eax*4 + 0x1a8c], edx ; -> obj1:0x1A8C
        mov    dword ptr [eax*4 + 0x1aac], ecx ; -> obj1:0x1AAC
        bts    dword ptr [0x1acd], eax       ; -> loc_1_1ACD
        or     ecx, ecx
        jne    loc_1_07B2
        bts    dword ptr [0x1ad9], eax       ; -> loc_1_1AD9
loc_1_07B2:
        pushal 
        push   eax
        push   ecx
        call   sub_1_17E0
        add    esp, 8
        popal  
        clc    
        ret    
loc_1_07C0:
        cmp    al, 0xff
        jne    loc_1_07DD
        mov    al, 7
loc_1_07C6:
        bt     dword ptr [0x1ad1], eax       ; -> loc_1_1AD1
        jb     loc_1_07D9
        cmp    dword ptr [eax*4 + 0x1a8c], 0 ; -> obj1:0x1A8C
        je     loc_1_0792
loc_1_07D9:
        dec    al
        jns    loc_1_07C6
loc_1_07DD:
        stc    
        ret    
VNETBIOS_Register:
        xor    ecx, ecx
        jmp    VNETBIOS_Register2
VNETBIOS_Deregister:
        movzx  eax, al
        cmp    al, 8
        jae    loc_1_0809
        lea    ebx, [eax*4 + 0x1a8c]         ; -> obj1:0x1A8C
        xor    ecx, ecx
        cmp    dword ptr [ebx], ecx
        je     loc_1_0809
        btr    dword ptr [0x1acd], eax       ; -> loc_1_1ACD
        btr    dword ptr [0x1ad9], eax       ; -> loc_1_1AD9
        mov    dword ptr [ebx], ecx
        clc    
        ret    
loc_1_0809:
        stc    
        ret    
VNETBIOS_Submit:
        push   esi
        push   edi
        push   ebx
        push   ebp
        mov    esi, ebx
        mov    edx, 0x5c
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    ebx, esi
        movzx  ecx, byte ptr [ebx + 0x30]
        mov    ecx, dword ptr [ecx*4 + 0x1a8c] ; -> obj1:0x1A8C
        jecxz  loc_1_0832
        call   ecx
        jmp    loc_1_0864
loc_1_0832:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        call   sub_1_0F48
        mov    al, byte ptr [ebp + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_0864:
        pop    ebp
        pop    ebx
        pop    edi
        pop    esi
        ret    
VNETBIOS_Enum:
        mov    eax, dword ptr [0x1acd]       ; -> loc_1_1ACD
        mov    edx, dword ptr [0x1ad1]       ; -> loc_1_1AD1
        ret    
VNETBIOS_Enum2:
        mov    eax, dword ptr [0x1ad9]       ; -> loc_1_1AD9
        ret    
VNETBIOS_Map:
        jcxz   loc_1_088F
        mov    eax, 7
loc_1_0883:
        cmp    dword ptr [eax*4 + 0x1aac], ecx ; -> obj1:0x1AAC
        je     loc_1_0891
        dec    eax
        jns    loc_1_0883
loc_1_088F:
        stc    
        ret    
loc_1_0891:
        clc    
        ret    
VNETBIOS_Control:
        cmp    eax, 0
        je     loc_4_0141
        cmp    eax, 1
        je     loc_4_01E3
        cmp    eax, 0xa
        je     loc_1_0C13
        cmp    eax, 4
        je     loc_1_0C13
        cmp    eax, 0xb
        je     loc_1_0C13
        cmp    eax, 0x13
        je     loc_1_0D6C
        cmp    eax, 0x19
        je     loc_1_0C13
        cmp    eax, 6
        je     loc_1_08EB
        cmp    eax, 0x21
        je     loc_1_099C
        cmp    eax, 0x23
        je     loc_1_09F6
        clc    
        ret    
loc_1_08EB:
        cmp    dword ptr [0x1ad5], 0         ; -> loc_1_1AD5
        jne    loc_1_090D
        mov    eax, 0x5c
        movzx  edx, word ptr [0x1ae1]        ; -> loc_1_1AE1
        mov    cx, word ptr [0x1ae3]         ; -> loc_1_1AE3
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
loc_1_090D:
        clc    
        ret    
loc_1_090F:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0199
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_1_0923:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0108
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
sub_1_0937:
        push   edi
        mov    edi, ebx
        push   esi
        push   ebx
        sub    esp, 0x68
        mov    eax, esp
        mov    edx, ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        lea    esi, [ecx + ebx]
        mov    ecx, dword ptr [ecx + ebx]
        xor    ebx, ebx
loc_1_0957:
        or     ecx, ecx
        je     loc_1_0995
        mov    edx, dword ptr [ecx - 4]
        cmp    dword ptr [ecx - 0x28], edi
        jne    loc_1_0991
        pushal 
        mov    byte ptr [eax], 0x35
        mov    dl, byte ptr [ecx + 0x30]
        mov    byte ptr [eax + 0x30], dl
        mov    dword ptr [eax + 4], ecx
        movzx  edx, dl
        mov    ebx, eax
        mov    esi, dword ptr [edx*4 + 0x1a8c] ; -> obj1:0x1A8C
        or     dword ptr [ecx - 8], 0x1100
        push   ecx
        call   esi
        pop    eax
        and    byte ptr [eax - 7], 0xfe
        call   sub_1_16A7
        popal  
loc_1_0991:
        mov    ecx, edx
        jmp    loc_1_0957
loc_1_0995:
        add    esp, 0x68
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_1_099C:
        push   ebx
        push   esi
        sub    esp, 0x68
        mov    eax, esp
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        mov    ecx, dword ptr [ecx + ebx]
loc_1_09B2:
        or     ecx, ecx
        je     loc_1_09EE
        mov    edx, dword ptr [ecx - 4]
        cmp    dword ptr [ecx - 0x20], edi
        jne    loc_1_09EA
        pushal 
        or     dword ptr [ecx - 8], 0x1100
        mov    byte ptr [eax], 0x35
        mov    dl, byte ptr [ecx + 0x30]
        mov    byte ptr [eax + 0x30], dl
        mov    dword ptr [eax + 4], ecx
        movzx  edx, dl
        mov    ebx, eax
        push   ecx
        call   dword ptr [edx*4 + 0x1a8c]    ; -> obj1:0x1A8C
        pop    eax
        and    byte ptr [eax - 7], 0xfe
        call   sub_1_16A7
        popal  
loc_1_09EA:
        mov    ecx, edx
        jmp    loc_1_09B2
loc_1_09EE:
        add    esp, 0x68
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
loc_1_09F6:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0x100
        jne    loc_1_0A13
        push   dword ptr [esi + 0x10]
        call   sub_1_0199
        add    esp, 4
loc_1_0A0F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0A13:
        cmp    ecx, 0
        je     loc_1_0A24
        cmp    ecx, -1
        je     loc_1_0A35
        mov    eax, 1
        jmp    loc_1_0A0F
loc_1_0A24:
        mov    ebx, dword ptr [esi + 0x2c]
        call   sub_1_0A57
        xor    eax, eax
        mov    ecx, 0
        jmp    loc_1_0A0F
loc_1_0A35:
        mov    ebx, dword ptr [esi + 0x2c]
        call   sub_1_0937
        call   sub_1_0B00
        xor    eax, eax
        jmp    loc_1_0A0F
        db  08Bh, 05Eh, 02Ch, 0E8h, 009h, 000h, 000h, 000h, 033h, 0C0h, 0B9h, 000h, 000h, 000h, 000h, 0EBh
        db  0B8h
sub_1_0A57:
        push   esi
        push   edi
        push   eax
        mov    ecx, ebx
        or     ecx, ecx
        je     loc_1_0A8E
        push   ecx
        push   1
        push   0x10
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_1_0AFD
        mov    dword ptr [eax], ebx
        push   edx
        mov    edx, dword ptr [0x19cc]       ; -> loc_1_19CC
        mov    dword ptr [eax + 0xc], edx
        mov    dword ptr [0x19cc], eax       ; -> loc_1_19CC
        pop    edx
        mov    ebx, eax
        or     ecx, ecx
        jne    loc_1_0A97
loc_1_0A8E:
        cmp    dword ptr [0x19d8], 1         ; -> loc_1_19D8
        jae    loc_1_0AC6
loc_1_0A97:
        push   ecx
        push   1
        push   0x25
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_1_0AFD
        add    dword ptr [0x19d8], 1         ; -> loc_1_19D8
        mov    edi, eax
        push   edx
        mov    edx, dword ptr [0x19d0]       ; -> loc_1_19D0
        mov    dword ptr [edi + 0x20], edx
        pop    edx
        mov    dword ptr [0x19d0], eax       ; -> loc_1_19D0
        or     ecx, ecx
        jne    loc_1_0ACF
loc_1_0AC6:
        cmp    dword ptr [0x19dc], 1         ; -> loc_1_19DC
        jae    loc_1_0AF9
loc_1_0ACF:
        push   1
        push   0x15
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_1_0AFD
        add    dword ptr [0x19dc], 1         ; -> loc_1_19DC
        mov    edi, eax
        push   edx
        mov    edx, dword ptr [0x19d4]       ; -> loc_1_19D4
        mov    dword ptr [edi + 0x10], edx
        pop    edx
        mov    dword ptr [0x19d4], eax       ; -> loc_1_19D4
        clc    
loc_1_0AF9:
        pop    eax
        pop    edi
        pop    esi
        ret    
loc_1_0AFD:
        stc    
        jmp    loc_1_0AF9
sub_1_0B00:
        push   esi
        push   edi
        push   eax
        push   ecx
        mov    ecx, dword ptr [0x19cc]       ; -> loc_1_19CC
        or     ecx, ecx
        je     loc_1_0C0E
loc_1_0B12:
        cmp    dword ptr [ecx], ebx
        je     loc_1_0B22
        mov    ecx, dword ptr [ecx + 0xc]
        or     ecx, ecx
        jne    loc_1_0B12
        jmp    loc_1_0C0E
loc_1_0B22:
        sub    esp, 0x68
        mov    eax, esp
        mov    ebx, eax
        mov    edx, dword ptr [ecx + 8]
        xor    edi, edi
        mov    esi, ecx
        mov    byte ptr [ebx], 0x12
        xor    ch, ch
        or     edx, edx
        je     loc_1_0B91
        mov    edi, edx
        dec    edx
        push   esi
loc_1_0B3D:
        inc    edx
        mov    cl, 1
        mov    al, byte ptr [edx]
loc_1_0B42:
        mov    ah, cl
        not    ah
        test   cl, al
        je     loc_1_0B6D
        and    al, ah
        mov    ah, byte ptr [edi + 0x24]
        mov    byte ptr [ebx + 0x30], ah
        mov    byte ptr [ebx + 2], ch
        push   edi
        push   edx
        movzx  edx, ah
        mov    edi, dword ptr [edx*4 + 0x1a8c] ; -> obj1:0x1A8C
        push   ecx
        push   eax
        test   edi, edi
        je     loc_1_0B69
        call   edi
loc_1_0B69:
        pop    eax
        pop    ecx
        pop    edx
        pop    edi
loc_1_0B6D:
        add    ch, 1
        jb     loc_1_0B78
        shl    cl, 1
        jb     loc_1_0B3D
        jmp    loc_1_0B42
loc_1_0B78:
        xor    ch, ch
        mov    edx, dword ptr [edi + 0x20]
        pushal 
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        popal  
        mov    edi, edx
        or     edx, edx
        jne    loc_1_0B3D
        pop    esi
loc_1_0B91:
        mov    edx, dword ptr [esi + 4]
        mov    byte ptr [ebx], 0x31
loc_1_0B97:
        or     edx, edx
        je     loc_1_0BD9
        mov    ah, byte ptr [edx + 0x14]
        mov    byte ptr [ebx + 0x30], ah
        mov    byte ptr [ebx + 2], ch
        pushal 
        lea    edi, [ebx + 0x1a]
        lea    esi, [edx]
        mov    ecx, 4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movzx  edx, ah
        mov    edi, dword ptr [edx*4 + 0x1a8c] ; -> obj1:0x1A8C
        test   edi, edi
        je     loc_1_0BC2
        call   edi
loc_1_0BC2:
        popal  
        mov    edi, edx
        mov    edx, dword ptr [edx + 0x10]
        pushal 
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        popal  
        or     edx, edx
        jne    loc_1_0B97
loc_1_0BD9:
        mov    eax, dword ptr [0x19cc]       ; -> loc_1_19CC
        cmp    eax, esi
        jne    loc_1_0BEC
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [0x19cc], eax       ; -> loc_1_19CC
        jmp    loc_1_0C00
loc_1_0BEC:
        cmp    dword ptr [eax + 0xc], esi
        je     loc_1_0BFA
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        jne    loc_1_0BEC
        jmp    loc_1_0C0B
loc_1_0BFA:
        mov    edi, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], edi
loc_1_0C00:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_1_0C0B:
        add    esp, 0x68
loc_1_0C0E:
        pop    ecx
        pop    eax
        pop    edi
        pop    esi
        ret    
loc_1_0C13:
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        or     byte ptr [edi + ebx + 4], 1
        cmp    dword ptr [edi + ebx], 0
        je     loc_1_0D52
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        mov    dword ptr [0x19b0], eax       ; -> obj1:0x19B0
        sub    eax, eax
        cmp    dword ptr [0x1ad1], 0         ; -> loc_1_1AD1
        jne    loc_1_0C46
        inc    eax
loc_1_0C46:
        call   0
        jb     loc_1_0D54
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
loc_1_0C66:
        mov    ecx, dword ptr [edi + ebx]
        test   ecx, ecx
        je     loc_1_0D23
        bts    dword ptr [ecx - 8], 6
        jb     loc_1_0D16
        or     byte ptr [ecx - 7], 1
        test   byte ptr [ecx - 7], 2
        je     loc_1_0CAB
        mov    dword ptr [eax + 4], ecx
        pushal 
        movzx  ecx, byte ptr [ecx + 0x30]
        mov    byte ptr [eax + 0x30], cl
        mov    byte ptr [eax], 0x35
        mov    edx, dword ptr [ecx*4 + 0x1a8c] ; -> obj1:0x1A8C
        mov    ebx, eax
        sub    eax, eax
        or     edx, edx
        je     loc_1_0CA5
        call   edx
loc_1_0CA5:
        mov    byte ptr [ebp + 0x1c], al
        popal  
        jmp    loc_1_0CF5
loc_1_0CAB:
        push   edi
        movzx  edi, word ptr [0x1914]        ; -> obj1:0x1914
        mov    word ptr [eax + 6], di
        mov    word ptr [ebp + 0x38], di
        shl    edi, 4
        push   ecx
        sub    ecx, edi
        mov    word ptr [eax + 4], cx
        mov    ecx, eax
        sub    ecx, edi
        mov    word ptr [ebp + 0x10], cx
        pop    ecx
        push   ecx
        mov    cl, byte ptr [ecx + 0x30]
        mov    byte ptr [eax + 0x30], cl
        pop    ecx
        pop    edi
        mov    byte ptr [eax], 0x35
        push   eax
        mov    eax, 0x5c
        test   byte ptr [ecx - 8], 0x80
        je     loc_1_0CEF
        mov    eax, 0x2a
        mov    byte ptr [ebp + 0x1d], 1
loc_1_0CEF:
        call   sub_1_10E7
        pop    eax
loc_1_0CF5:
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_1_0D16
        cmp    byte ptr [ebp + 0x1c], 0xff
        je     loc_1_0D16
        push   ecx
        movzx  ecx, word ptr [0x10]          ; -> obj2:0x0010
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        mov    dword ptr [0x19b0], ecx       ; -> obj1:0x19B0
        pop    ecx
loc_1_0D16:
        xchg   ecx, eax
        call   sub_1_16A7
        mov    eax, ecx
        jmp    loc_1_0C66
loc_1_0D23:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        call   sub_1_0020
loc_1_0D3D:
        mov    eax, 0x80001030
        mov    ecx, dword ptr [0x19b0]       ; -> obj1:0x19B0
        xor    edi, edi
        xor    esi, esi
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
loc_1_0D52:
        clc    
        ret    
loc_1_0D54:
        mov    dword ptr [0x19b0], 3         ; -> obj1:0x19B0
loc_1_0D5E:
        mov    eax, dword ptr [edi + ebx]
        test   eax, eax
        je     loc_1_0D3D
        call   sub_1_16A7
        jmp    loc_1_0D5E
loc_1_0D6C:
        mov    ecx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        cmp    dword ptr [ecx + ebx], 0
        je     loc_1_0D96
        mov    eax, 0x1030
        movzx  ecx, word ptr [0xc]           ; -> loc_2_000C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        xor    edi, edi
        xor    esi, esi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        stc    
        ret    
loc_1_0D96:
        clc    
        ret    
loc_1_0D98:
        mov    eax, 0x2a
        cmp    byte ptr [ebp + 0x1d], 1
        je     loc_1_0DE2
        cmp    byte ptr [ebp + 0x1d], 4
        je     loc_1_0DE2
        mov    ecx, dword ptr [0x19b4]       ; -> obj1:0x19B4
        mov    edx, dword ptr [0x19b8]       ; -> obj1:0x19B8
        mov    eax, ecx
        or     eax, edx
        je     loc_1_0DC1
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_0DC1:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, 0x2a
        call   sub_1_10E7
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
loc_1_0DDD:
        mov    eax, 0x5c
loc_1_0DE2:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_1_0F1E
        db  02Eh, 08Bh, 0C0h
loc_1_0DF0:
        cmp    byte ptr [ebp + 0x1d], 1
        je     loc_1_0E20
        cmp    byte ptr [ebp + 0x1d], 4
        je     loc_1_0E20
        cmp    word ptr [ebp + 0x1c], 0x500
        je     loc_1_0E06
        stc    
        ret    
loc_1_0E06:
        sub    eax, eax
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], 0x10
        mov    word ptr [ebp + 0x18], 0x10
        mov    word ptr [ebp + 0x14], 0x40
        clc    
        ret    
loc_1_0E20:
        mov    si, word ptr [0x1914]         ; -> obj1:0x1914
        cmp    word ptr [ebp + 0x38], si
        jne    loc_1_0E37
        cmp    word ptr [ebp + 0x10], 0x1000
        jae    loc_1_0E37
        stc    
        ret    
loc_1_0E37:
        movzx  esi, word ptr [ebp + 0x38]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    esi, ecx
        movzx  ecx, byte ptr [esi]
        and    ecx, 0xffffff7f
        cmp    ecx, 0xf
        ja     loc_1_0E5F
        cmp    byte ptr [ecx + 0x1a08], 0    ; -> obj1:0x1A08
        je     loc_1_0EE6
loc_1_0E5F:
        movzx  ecx, byte ptr [esi + 0x30]
        cmp    cl, 8
        jae    loc_1_1041
        cmp    dword ptr [ecx*4 + 0x1a8c], 0 ; -> obj1:0x1A8C
        jne    loc_1_0F1E
        push   eax
        push   0
        push   0x40
        push   esi
        VMMCall  VMM._TestGlobalV86Mem           ; cd205f000100
        add    esp, 0xc
        test   eax, eax
        pop    eax
        je     loc_1_0F1E
        VxDCall  DOSMGR.DOSMGR_Get_DOS_Crit_Status ; cd2008001500
        jne    loc_1_0F0E
        movzx  edi, byte ptr [esi]
        and    edi, 0xffffff7f
        cmp    byte ptr [edi + 0x1a08], 4    ; -> obj1:0x1A08
        je     loc_1_0EE6
        cmp    byte ptr [edi + 0x1a08], 0    ; -> obj1:0x1A08
        jne    loc_1_0EC2
        cmp    dword ptr [esi + 4], 0
        je     loc_1_0EE6
        cmp    word ptr [esi + 8], 0
        je     loc_1_0EE6
loc_1_0EC2:
        push   eax
        movzx  eax, word ptr [esi + 6]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 4]
        add    eax, ecx
        movzx  ecx, word ptr [esi + 8]
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._TestGlobalV86Mem           ; cd205f000100
        add    esp, 0xc
        test   eax, eax
        pop    eax
        je     loc_1_0F1E
loc_1_0EE6:
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        bts    dword ptr [edi + ebx + 4], 1
        jb     loc_1_0F0C
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        xor    eax, eax
        mov    esi, 0x10d7                   ; -> loc_1_10D7
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
loc_1_0F0C:
        stc    
        ret    
loc_1_0F0E:
        cmp    byte ptr [esi + 0x2a], 0
        jne    loc_1_0EE6
        mov    al, byte ptr [0x1a8a]         ; -> obj1:0x1A8A
        mov    byte ptr [esi + 0x2a], al
        jmp    loc_1_0EE6
loc_1_0F1E:
        mov    edx, eax
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        mov    cx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x10]
        xchg   esi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        test   byte ptr [edi + ebx + 4], 1
        jne    loc_1_1052
sub_1_0F48:
        movzx  eax, byte ptr [esi + 0x30]
        cmp    al, 8
        jae    loc_1_1041
        bt     dword ptr [0x1ad5], eax       ; -> loc_1_1AD5
        jb     loc_1_0F6A
        bt     dword ptr [0x1acd], eax       ; -> loc_1_1ACD
        jae    loc_1_1041
loc_1_0F6A:
        mov    eax, dword ptr [eax*4 + 0x1a8c] ; -> obj1:0x1A8C
        call   0
        jb     loc_1_1052
        and    dword ptr [eax - 8], 0x200
        or     byte ptr [eax - 8], 0x21
        mov    dword ptr [eax - 0x10], ecx
        test   ecx, ecx
        je     loc_1_0FAA
        push   ecx
        push   eax
        call   sub_1_0108
        mov    ecx, eax
        pop    eax
        mov    dword ptr [eax - 0x20], ecx
        pop    ecx
        cmp    ecx, 1
        jne    loc_1_0FB4
        mov    dword ptr [eax - 0x10], esi
        or     byte ptr [eax - 7], 8
        jmp    loc_1_0FB4
loc_1_0FAA:
        mov    dword ptr [eax - 0x10], esi
        or     byte ptr [eax - 7], 4
        mov    dword ptr [eax - 0x20], ecx
loc_1_0FB4:
        xor    ecx, ecx
        mov    dword ptr [eax - 0x18], ecx
        mov    dword ptr [eax - 0x1c], ecx
        mov    dword ptr [eax - 0xc], ebx
        mov    dword ptr [eax - 0x14], esi
        cmp    edx, 0x2a
        jne    loc_1_0FCB
        or     byte ptr [eax - 8], 0x80
loc_1_0FCB:
        test   byte ptr [eax - 7], 4
        jne    loc_1_0FED
        test   byte ptr [eax - 7], 8
        jne    loc_1_0FED
        mov    ecx, dword ptr [eax - 0x20]
        mov    ecx, dword ptr [ecx + 0x24]
        jecxz  loc_1_0FE4
        sub    ecx, ecx
        mov    dword ptr [eax - 0x20], ecx
loc_1_0FE4:
        test   byte ptr [ebx], 0x20
        je     loc_1_0FED
        or     byte ptr [eax - 8], 0x10
loc_1_0FED:
        mov    ecx, eax
        xchg   dword ptr [edi + ebx], ecx
        mov    dword ptr [eax - 4], ecx
        mov    edi, eax
        cld    
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    esi, esi
        movzx  ecx, byte ptr [eax]
        test   cl, 0x80
        jne    loc_1_102E
        test   byte ptr [eax - 7], 4
        jne    loc_1_106E
        cmp    byte ptr [0x1a8b], 0          ; -> loc_1_1A8B
        je     loc_1_106E
        mov    cl, byte ptr [ecx + 0x1a08]   ; -> obj1:0x1A08
        cmp    cl, 0x14
        je     loc_1_1028
        cmp    cl, 0x1c
        jb     loc_1_106E
loc_1_1028:
        or     byte ptr [eax], 0x80
        mov    esi, dword ptr [eax - 0x14]
loc_1_102E:
        or     byte ptr [eax - 8], 6
        test   byte ptr [eax - 7], 2
        je     loc_1_1065
        mov    dword ptr [eax + 0x2c], 0x1405 ; -> loc_1_1405
        jmp    loc_1_106E
loc_1_1041:
        mov    eax, dword ptr [0x1ad5]       ; -> loc_1_1AD5
        or     eax, dword ptr [0x1acd]       ; -> loc_1_1ACD
        je     loc_1_105F
        mov    al, 0x23
        jmp    loc_1_1054
loc_1_1052:
        mov    al, 0x22
loc_1_1054:
        mov    byte ptr [esi + 1], al
        mov    byte ptr [esi + 0x31], al
        mov    byte ptr [ebp + 0x1c], al
        jmp    loc_1_10D5
loc_1_105F:
        mov    byte ptr [ebp + 0x1c], 0x23
        jmp    loc_1_10D5
loc_1_1065:
        mov    ecx, dword ptr [0x19ac]       ; -> obj1:0x19AC
        mov    dword ptr [eax + 0x2c], ecx
loc_1_106E:
        movzx  edi, byte ptr [eax]
        and    edi, 0x7f
        movzx  edi, byte ptr [edi + 0x1a08]  ; -> obj1:0x1A08
        push   esi
        call   sub_1_19E0
        pop    ecx
        jecxz  loc_1_10D5
        cmp    byte ptr [ecx + 0x31], 0xff
        jne    loc_1_10CF
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_10A6:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    byte ptr [ecx + 0x31], 0xff
        jne    loc_1_10BA
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        jmp    loc_1_10A6
loc_1_10BA:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_10CF:
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp + 0x1c], al
loc_1_10D5:
        clc    
        ret    
loc_1_10D7:
        add    ebx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        and    byte ptr [ebx + 4], 0xfd
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
sub_1_10E7:
        push   ecx
        push   edx
        push   eax
        mov    ax, word ptr [ebp + 0x2c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    eax
        mov    ecx, dword ptr [0x1add]       ; -> loc_1_1ADD
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        mov    eax, dword ptr [eax*4]
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        shr    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    edx
        pop    ecx
        ret    
sub_1_1129:
        mov    edi, eax
        test   byte ptr [eax - 7], 2
        jne    loc_1_119B
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   ecx
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        pop    ecx
        push   dword ptr [ebp + 0x38]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x2c]
        and    byte ptr [ebp + 0x2d], 0xfb
        movzx  esi, word ptr [0x1914]        ; -> obj1:0x1914
        mov    word ptr [ebp + 0x38], si
        shl    esi, 4
        sub    eax, esi
        mov    word ptr [ebp + 0x10], ax
        mov    eax, edx
        call   sub_1_10E7
        mov    eax, edi
        pop    dword ptr [ebp + 0x2c]
        pop    dword ptr [ebp + 0x10]
        pop    dword ptr [ebp + 0x38]
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
loc_1_1183:
        and    byte ptr [eax - 8], 0xfe
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_1_1196
        and    byte ptr [eax - 8], 0xf9
        call   sub_1_15A9
loc_1_1196:
        jmp    sub_1_164D
loc_1_119B:
        pushal 
        mov    ebx, eax
        movzx  ecx, byte ptr [eax + 0x30]
        call   dword ptr [ecx*4 + 0x1a8c]    ; -> obj1:0x1A8C
        mov    byte ptr [ebp + 0x1c], al
        popal  
        jmp    loc_1_1183
loc_1_11AF:
        cmp    dword ptr [eax + 4], 0
        je     sub_1_1129
        cmp    word ptr [esi + 8], 0
        je     sub_1_1129
        test   byte ptr [eax - 7], 6
        jne    loc_1_1261
        test   byte ptr [ebx], 0x20
        je     loc_1_124D
        lar    cx, word ptr [eax + 6]
        je     loc_1_124D
        jmp    sub_1_1129
loc_1_11DF:
        test   byte ptr [eax - 7], 6
        jne    loc_1_1261
        test   byte ptr [ebx], 0x20
        jne    loc_1_1261
        test   byte ptr [eax], 0x80
        je     sub_1_1129
        or     byte ptr [eax - 8], 8
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        inc    dword ptr [0x19c8]            ; -> loc_1_19C8
        cmp    dword ptr [0x19c8], 1         ; -> loc_1_19C8
        ja     sub_1_1129
        mov    dword ptr [0x19c4], ebx       ; -> loc_1_19C4
        mov    esi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        test   byte ptr [esi + ebx + 4], 4
        jne    sub_1_1129
        push   eax
        mov    eax, dword ptr [0x19bc]       ; -> loc_1_19BC
        test   eax, eax
        je     loc_1_1247
        mov    esi, 0x15cf                   ; -> loc_1_15CF
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x19c0], esi       ; -> loc_1_19C0
loc_1_1247:
        pop    eax
        jmp    sub_1_1129
loc_1_124D:
        test   byte ptr [eax - 7], 6
        jne    loc_1_1261
        test   byte ptr [ebx], 0x20
        jne    loc_1_1261
        test   byte ptr [eax], 0x80
        je     sub_1_1129
loc_1_1261:
        movzx  ecx, word ptr [eax + 8]
        mov    esi, dword ptr [eax + 4]
        call   sub_1_003B
        jb     loc_1_127A
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax - 0x18], esi
        jmp    sub_1_1129
loc_1_127A:
        cmp    byte ptr [0x1a89], 0          ; -> loc_1_1A89
        je     loc_1_12AC
        test   dword ptr [eax - 8], 0x600
        jne    loc_1_12AC
        test   byte ptr [ebx], 0x20
        jne    loc_1_12AC
        xor    esi, esi
        xchg   dword ptr [eax - 0x1c], esi
        mov    edi, dword ptr [eax + 0xc]
        call   sub_1_00E2
        mov    esi, dword ptr [eax - 0x14]
        mov    esi, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], esi
        jmp    loc_1_11DF
loc_1_12AC:
        mov    byte ptr [eax + 1], 0x22
        mov    byte ptr [eax + 0x31], 0x22
        and    byte ptr [eax - 8], 0xf8
        mov    byte ptr [ebp + 0x1c], 0x22
        call   sub_1_15A9
        push   ecx
        call   sub_1_164D
        pop    ecx
        movzx  eax, byte ptr [0x1a88]        ; -> obj1:0x1A88
        test   eax, eax
        je     loc_1_1345
        lea    edi, [0x1918]                 ; -> loc_1_1918
        cld    
        mov    byte ptr [0x1a88], 0          ; -> obj1:0x1A88
        add    ecx, 0xfff
        shr    ecx, 0xc
        add    eax, ecx
        shl    eax, 2
        mov    esi, eax
        xor    edx, edx
        mov    ecx, 0x64
        idiv   ecx
        test   al, al
        je     loc_1_1301
        add    al, 0x30
        stosb  byte ptr es:[edi], al
loc_1_1301:
        mov    eax, edx
        aam    0xa
        add    eax, 0x3030
        cmp    esi, 0xa
        jb     loc_1_1314
        xchg   al, ah
        stosb  byte ptr es:[edi], al
        mov    al, ah
loc_1_1314:
        stosb  byte ptr es:[edi], al
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x1918                   ; -> loc_1_1918
        push   eax
        push   0xc0004
        push   0                             ; -> obj2:0x0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    ecx, eax
        mov    eax, 0x1030
        xor    edi, edi
        xor    esi, esi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        add    esp, dword ptr [esp]
loc_1_1345:
        ret    
loc_1_1346:
        test   byte ptr [eax - 7], 6
        jne    loc_1_135A
        test   byte ptr [ebx], 0x20
        jne    loc_1_135A
        test   byte ptr [eax], 0x80
        je     sub_1_1129
loc_1_135A:
        movzx  ecx, word ptr [eax + 0xa]
        mov    esi, dword ptr [eax + 0xc]
        call   sub_1_003B
        jb     loc_1_127A
        mov    dword ptr [eax + 0xc], edi
        mov    dword ptr [eax - 0x1c], esi
        jmp    loc_1_1261
loc_1_1377:
        mov    edi, dword ptr [eax + 4]
        mov    ecx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        mov    ecx, dword ptr [ebx + ecx]
loc_1_1383:
        jecxz  loc_1_139B
        cmp    dword ptr [ecx - 0x10], edi
        je     loc_1_138F
        mov    ecx, dword ptr [ecx - 4]
        jmp    loc_1_1383
loc_1_138F:
        test   byte ptr [ecx - 8], 2
        jne    loc_1_13BA
        test   byte ptr [ecx - 8], 4
        je     loc_1_13BA
loc_1_139B:
        mov    byte ptr [eax + 1], 0x24
        mov    byte ptr [eax + 0x31], 0x24
        mov    byte ptr [ebp + 0x1c], 0x24
        and    byte ptr [eax - 8], 0xf8
        call   sub_1_15A9
        call   sub_1_164D
        and    byte ptr [eax - 8], 0xdf
        ret    
loc_1_13BA:
        test   byte ptr [eax - 7], 2
        je     loc_1_13C5
        mov    dword ptr [eax + 4], ecx
        jmp    loc_1_13DB
loc_1_13C5:
        movzx  esi, word ptr [0x1914]        ; -> obj1:0x1914
        mov    word ptr [eax + 6], si
        shl    esi, 4
        push   ecx
        sub    ecx, esi
        mov    word ptr [eax + 4], cx
        pop    ecx
loc_1_13DB:
        or     byte ptr [ecx - 7], 1
        call   sub_1_1129
        and    byte ptr [ecx - 7], 0xfe
        cmp    byte ptr [ebp + 0x1c], 0
        jne    loc_1_1404
        xchg   ecx, eax
        call   sub_1_15A9
        xchg   ecx, eax
        test   word ptr [ecx - 8], 0x10f
        jne    loc_1_1404
        xchg   ecx, eax
        call   sub_1_16A7
        xchg   ecx, eax
loc_1_1404:
        ret    
loc_1_1405:
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        jmp    loc_1_1428
loc_1_1412:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        movzx  edx, word ptr [0x1914]        ; -> obj1:0x1914
        shl    edx, 4
        movzx  esi, word ptr [ebp + 0x10]
        add    edx, esi
loc_1_1428:
        and    byte ptr [edx - 8], 0xfd
        test   byte ptr [edx - 7], 4
        jne    loc_1_146C
        test   byte ptr [edx - 7], 8
        jne    loc_1_146C
        mov    ebx, dword ptr [edx - 0xc]
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        test   byte ptr [edi + ebx + 4], 1
        jne    loc_1_149C
        test   dword ptr [edx - 8], 0x1000
        jne    loc_1_149D
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        mov    esi, 0x14c6                   ; -> loc_1_14C6
        mov    ecx, 9
        mov    eax, 0x1000
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
loc_1_146C:
        xchg   edx, eax
        and    byte ptr [eax - 8], 0xfb
        call   sub_1_15A9
        test   byte ptr [eax - 8], 0x40
        jne    loc_1_159C
        mov    ebx, dword ptr [eax - 0x14]
        call   sub_1_164D
        and    byte ptr [eax - 8], 0xdf
        mov    ecx, dword ptr [ebx + 0x2c]
        jecxz  loc_1_149C
        test   byte ptr [eax - 7], 8
        jne    loc_1_14A7
        mov    al, byte ptr [ebx + 1]
        call   ecx
loc_1_149C:
        ret    
loc_1_149D:
        push   eax
        mov    eax, edx
        call   sub_1_16A7
        pop    eax
        ret    
loc_1_14A7:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  edx, byte ptr [ebx + 1]
        push   edx
        push   dword ptr [eax - 0x20]
        push   ebx
        push   dword ptr [ebx + 0x2c]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_14C6:
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        mov    edi, dword ptr [0x19a4]       ; -> obj1:0x19A4
        test   byte ptr [edi + ebx + 4], 1
        jne    loc_1_159B
        mov    eax, edx
        and    byte ptr [eax - 8], 0xfb
        call   sub_1_15A9
        test   byte ptr [eax - 8], 0x40
        jne    loc_1_159C
        call   sub_1_16EC
        mov    ecx, dword ptr [eax - 0x14]
        test   byte ptr [ecx], 0x80
        je     loc_1_1592
        mov    ecx, dword ptr [ecx + 0x2c]
        test   ecx, ecx
        je     loc_1_1592
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        test   byte ptr [eax - 8], 0x10
        jne    loc_1_1536
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        jmp    loc_1_153C
loc_1_1536:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_1_153C:
        mov    esi, dword ptr [eax - 0x14]
        mov    esi, dword ptr [esi + 0x2c]
        mov    edx, dword ptr [eax - 0x10]
        mov    word ptr [ebp + 0x10], dx
        shr    edx, 0x10
        mov    word ptr [ebp + 0x38], dx
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [ebp + 0x1c], dl
        call   sub_1_164D
        and    byte ptr [eax - 8], 0xdf
        mov    ecx, esi
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        and    byte ptr [ebp + 0x2d], 0xfb
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
loc_1_1592:
        call   sub_1_164D
        and    byte ptr [eax - 8], 0xdf
loc_1_159B:
        ret    
loc_1_159C:
        test   word ptr [eax - 8], 0x10f
        jne    loc_1_159B
        jmp    sub_1_16A7
sub_1_15A9:
        btr    dword ptr [eax - 8], 3
        jae    loc_1_15CE
        dec    dword ptr [0x19c8]            ; -> loc_1_19C8
        jne    loc_1_15C8
        push   esi
        xor    esi, esi
        xchg   dword ptr [0x19c0], esi       ; -> loc_1_19C0
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
loc_1_15C8:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_1_15CE:
        ret    
loc_1_15CF:
        mov    dword ptr [0x19c0], 0         ; -> loc_1_19C0
        VMMCall  VMM.Get_Time_Slice_Info         ; cd2036000100
        cmp    eax, 1
        ja     loc_1_15F1
        cmp    ebx, dword ptr [0x19c4]       ; -> loc_1_19C4
        jne    loc_1_15F1
        test   byte ptr [ebx], 1
        jne    loc_1_164C
loc_1_15F1:
        mov    ebx, dword ptr [0x19c4]       ; -> loc_1_19C4
        mov    eax, 0x1034
        movzx  ecx, word ptr [0x14]          ; -> obj2:0x0014
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        xor    edi, edi
        xor    esi, esi
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        cmp    eax, 6
        jne    loc_1_162E
        xor    ecx, ecx
        mov    dword ptr [0x19c4], ecx       ; -> loc_1_19C4
        mov    dword ptr [0x19c8], ecx       ; -> loc_1_19C8
        VMMCall  VMM.Nuke_VM                     ; cd202e000100
        jmp    loc_1_164C
loc_1_162E:
        mov    esi, 1
        xor    edi, edi
        xor    edx, edx
        mov    eax, 0xf
        VMMCall  VMM.System_Control              ; cd2093000100
        add    ebx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        or     byte ptr [ebx + 4], 4
loc_1_164C:
        ret    
sub_1_164D:
        pushal 
        test   byte ptr [eax - 8], 0x20
        je     loc_1_1698
        call   sub_1_16EC
        mov    edi, dword ptr [eax - 0x14]
        mov    esi, eax
        cld    
        inc    esi
        inc    edi
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        add    esi, 4
        add    edi, 4
        movsw  word ptr es:[edi], word ptr [esi]
        mov    bl, byte ptr [eax]
        and    ebx, 0x7f
        cmp    ebx, 0x17
        je     loc_1_167D
        mov    ecx, 0x11
        jmp    loc_1_1688
loc_1_167D:
        add    esi, 0x10
        add    edi, 0x10
        mov    ecx, 9
loc_1_1688:
        rep movsw word ptr es:[edi], word ptr [esi]
        add    esi, 4
        add    edi, 4
        mov    ecx, 4
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_1698:
        test   word ptr [eax - 8], 0x10f
        jne    loc_1_16A5
        call   sub_1_16A7
loc_1_16A5:
        popal  
        ret    
sub_1_16A7:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax - 0x18]
        mov    edi, dword ptr [eax + 4]
        call   sub_1_00E2
        mov    esi, dword ptr [eax - 0x1c]
        mov    edi, dword ptr [eax + 0xc]
        call   sub_1_00E2
        mov    ebx, dword ptr [eax - 0xc]
        add    ebx, dword ptr [0x19a4]       ; -> obj1:0x19A4
        lea    ebx, [ebx + 4]
loc_1_16CC:
        cmp    dword ptr [ebx - 4], eax
        je     loc_1_16DA
        mov    ebx, dword ptr [ebx - 4]
        test   ebx, ebx
        jne    loc_1_16CC
        jmp    loc_1_16E5
loc_1_16DA:
        mov    esi, dword ptr [eax - 4]
        mov    dword ptr [ebx - 4], esi
        call   sub_1_0020
loc_1_16E5:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_16EC:
        test   byte ptr [eax - 7], 4
        jne    loc_1_1721
        test   byte ptr [eax - 8], 0x10
        je     loc_1_1721
        push   ebx
        push   ecx
        push   edx
        movzx  edx, word ptr [eax - 0xe]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xchg   ebx, eax
        push   0
        push   edx
        push   eax
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        movzx  edx, word ptr [ebx - 0x10]
        add    edx, eax
        mov    eax, ebx
        mov    dword ptr [eax - 0x14], edx
        pop    edx
        pop    ecx
        pop    ebx
loc_1_1721:
        ret    
sub_1_1722:
        pushal 
        mov    edi, esi
        mov    ebx, dword ptr [edi + 4]
        inc    ebx
        movzx  ecx, word ptr [edi + 8]
        mov    esi, 0x1a8c                   ; -> obj1:0x1A8C
        sub    edx, edx
        jecxz  loc_1_175C
        dec    ecx
loc_1_1737:
        lodsd  eax, dword ptr [esi]
        test   eax, eax
        je     loc_1_1744
        jecxz  loc_1_175C
        mov    byte ptr [ebx], dl
        inc    ebx
        dec    ecx
        inc    dh
loc_1_1744:
        inc    dl
        cmp    esi, 0x1aac                   ; -> obj1:0x1AAC
        jne    loc_1_1737
        mov    byte ptr [edi + 1], 0
loc_1_1752:
        mov    ebx, dword ptr [edi + 4]
        mov    byte ptr [ebx], dh
        popal  
        sub    eax, eax
        clc    
        ret    
loc_1_175C:
        mov    byte ptr [edi + 1], 1
        jmp    loc_1_1752
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1770:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x18]
loc_1_177C:
        mov    ecx, 0xa
        mov    eax, esi
        sub    edx, edx
        div    ecx
        lea    eax, [esp + 0xe]
        sub    eax, edi
        inc    edi
        mov    byte ptr [eax], dl
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    esi, eax
        cmp    edi, 3
        jl     loc_1_177C
        sub    edx, edx
        sub    eax, eax
loc_1_17A1:
        cmp    byte ptr [esp + edx + 0xc], al
        jne    loc_1_17AD
        inc    edx
        cmp    edx, 2
        jl     loc_1_17A1
loc_1_17AD:
        sub    eax, eax
        cmp    edx, 3
        jge    loc_1_17D1
        mov    eax, 3
        sub    eax, edx
        mov    ecx, dword ptr [esp + 0x14]
loc_1_17BF:
        mov    bl, byte ptr [esp + edx + 0xc]
        add    bl, 0x30
        mov    byte ptr [ecx], bl
        inc    ecx
        inc    edx
        cmp    edx, 3
        jl     loc_1_17BF
        jmp    loc_1_17D5
loc_1_17D1:
        mov    ecx, dword ptr [esp + 0x14]
loc_1_17D5:
        mov    byte ptr [ecx], 0
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_17E0:
        sub    esp, 0x4c
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x58]
        or     ecx, ecx
        je     loc_1_1905
        mov    dword ptr [esp + 0x10], 0x40
        lea    eax, [esp + 0x10]
        push   1
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   1
        push   0x1b60                        ; -> loc_1_1B60
        push   0x1b70                        ; -> obj1:0x1B70
        push   ecx
        call   sub_6_0018
        add    esp, 0x1c
        or     eax, eax
        jne    loc_1_1905
        sub    esi, esi
        mov    edi, 0x1b44                   ; -> obj1:0x1B44
loc_1_1829:
        mov    eax, dword ptr [edi]
        lea    ecx, [esp + 0x14]
loc_1_182F:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_184F
        or     dl, dl
        je     loc_1_184B
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_184F
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_182F
loc_1_184B:
        sub    eax, eax
        jmp    loc_1_1854
loc_1_184F:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1854:
        or     eax, eax
        je     loc_1_1864
        add    edi, 0xc
        inc    esi
        cmp    edi, 0x1b5c                   ; -> loc_1_1B5C
        jb     loc_1_1829
loc_1_1864:
        cmp    esi, 2
        je     loc_1_1905
        imul   esi, esi, 0xc
        mov    ax, word ptr [esi + 0x1b4e]   ; -> obj1:0x1B4E
        lea    ecx, [eax + 1]
        mov    word ptr [esi + 0x1b4e], cx   ; -> obj1:0x1B4E
        movsx  eax, ax
        push   eax
        movsx  eax, word ptr [esi + 0x1b4c]  ; -> obj1:0x1B4C
        add    eax, dword ptr [esi + 0x1b48] ; -> obj1:0x1B48
        push   eax
        call   sub_1_1770
        add    esp, 8
        cmp    byte ptr [0x1b5c], 0          ; -> loc_1_1B5C
        je     loc_1_18BD
        push   0x1af0                        ; -> obj1:0x1AF0
        push   0x80000002
        call   sub_6_0008
        add    esp, 8
        mov    byte ptr [0x1b5c], 0          ; -> loc_1_1B5C
loc_1_18BD:
        lea    eax, [esp + 8]
        push   eax
        push   0x1af0                        ; -> obj1:0x1AF0
        push   0x80000002
        call   sub_6_0000
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1905
        lea    eax, [esp + 0xc]
        push   dword ptr [esp + 0x5c]
        push   eax
        call   sub_1_1770
        add    esp, 8
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        push   1
        push   0
        push   dword ptr [esi + 0x1b48]      ; -> obj1:0x1B48
        push   dword ptr [esp + 0x1c]
        call   sub_6_0010
        add    esp, 0x18
loc_1_1905:
        pop    edi
        pop    esi
        add    esp, 0x4c
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1918:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        dd  VNETBIOS_Get_Version     ; fixup
        dd  VNETBIOS_Register        ; fixup
        dd  VNETBIOS_Submit          ; fixup
        dd  VNETBIOS_Enum            ; fixup
        dd  VNETBIOS_Deregister      ; fixup
        dd  VNETBIOS_Register2       ; fixup
        dd  VNETBIOS_Map             ; fixup
        dd  VNETBIOS_Enum2           ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00014h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VNETBIOS'                  ; DDB_Name
        dd  054000000h                  ; DDB_Init_Order
        dd  VNETBIOS_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x191C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_1_090F               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_1_0923               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_19BC:
        mov    byte ptr [ebx], dl
        add    byte ptr [eax], al
loc_1_19C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_19E0:
        scasd  eax, dword ptr es:[edi]
        adc    dword ptr [eax], eax
        add    byte ptr [ecx], ch            ; -> sub_1_1129
        adc    dword ptr [eax], eax
        add    bh, bl                        ; -> loc_1_11DF
        adc    dword ptr [eax], eax
        add    bh, bl                        ; -> loc_1_11DF
        adc    dword ptr [eax], eax
        add    bh, bl                        ; -> loc_1_11DF
        adc    dword ptr [eax], eax
        add    byte ptr [esi + 0x13], al     ; -> loc_1_1346
        add    byte ptr [eax], al
        ja     loc_1_1A0D
        add    byte ptr [eax], al
        dec    ebp
        adc    al, byte ptr [eax]
        add    byte ptr [ebp + 0x12], cl     ; -> loc_1_124D
        add    byte ptr [eax], al
        dec    ebp
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A0D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esp + eax], al
        add    al, 0
        sbb    al, 0x20
        and    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        sbb    al, 0x20
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 4
        add    al, 0xc
        or     al, 0x18
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    al, 0x1c
        add    al, 4
        add    al, 0x1c
        and    byte ptr [eax], bl
        and    byte ptr [eax], ah
        sbb    al, 0
        add    al, 0
loc_1_1A89:
        add    byte ptr [eax], al
loc_1_1A8B:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1ACD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1ADD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AE1:
        add    byte ptr [eax], al
loc_1_1AE3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], dl
        je     loc_1_1B6C
        popal  
        jb     0x1b5d
        pop    esp
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
        je     0x1b5f
        push   edx
        jo     0x1b69
        pop    esp
        dec    esi
        je     0x1b6d
        imul   ebp, dword ptr [edi + 0x73], 0x454e0000
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [esi + 0x63], ch
        popal  
        arpl   word ptr [esi + 0x5f], bp
        outsb  dx, byte ptr [esi]
        bound  ebx, qword ptr [edi + 0x6e]
        bound  edi, qword ptr [edi]
        aas    
        aas    
        add    byte ptr [eax], al
loc_1_1B28:
        dec    esi
        push   edi
        dec    esi
        inc    edx
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B34:
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ecx + 0x63], sp
        outsb  dx, byte ptr [esi]
        pop    edi
        outsb  dx, byte ptr [esi]
        bound  ebx, qword ptr [edi + 0x69]
        jo     0x1bb8
        aas    
        aas    
        aas    
        add    byte ptr [eax], dl            ; -> obj1:0x1B10
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], bl            ; -> obj1:0x1B18
        sbb    eax, dword ptr [eax]
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ch            ; -> loc_1_1B28
        sbb    eax, dword ptr [eax]
        add    byte ptr [ebx + ebx], dh      ; -> loc_1_1B34
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
loc_1_1B5C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1B60:
        dec    esp
        outsd  dx, dword ptr [esi]
        inc    esp
        jb     0x1bcf
        jbe    0x1bcd
        jb     0x1bb8
        popal  
        insd   dword ptr es:[edi], dx
loc_1_1B6C:
        add    byte ptr gs:[eax], al
        add    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x41d  R-X
        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        mov    al, byte ptr [0x22017d00]
        db  002h, 0BEh, 003h, 005h, 004h
loc_2_0018:
        push   esp
        push   0x61207369
        jo     0x90
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [ebx + 0x68], dh
        outsd  dx, dword ptr [esi]
        jne    0x9a
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_0054
        bound  esp, qword ptr [ebp + 0x20]
        je     0x9e
        jb     0xa8
        imul   ebp, dword ptr [esi + 0x61], 0x20646574
        bound  esp, qword ptr [ebp + 0x63]
        popal  
        jne    loc_2_00BB
        and    byte ptr gs:[ecx + 0x74], ch
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ebx + 0x75], ah
        jb     0xc6
loc_2_0054:
        outsb  dx, byte ptr gs:[esi]
        je     0xc4
        jns    loc_2_007A
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        jne    0xce
        imul   esp, dword ptr [ebx + 0x61], 0x676e6974
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], ch
        je     0xea
        outsd  dx, dword ptr [esi]
        jb     loc_2_00E1
        and    byte ptr cs:[ecx + 0x6f], bl
loc_2_007A:
        jne    0x9c
        jae    loc_2_00E6
        outsd  dx, dword ptr [esi]
        jne    0xed
        and    byte ptr fs:[edx + esi*2 + 0x79], dh
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jno    0x101
        imul   esi, dword ptr [eax + 0x74], 0x20736968
        popal  
        jo     loc_2_0107
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [ebp + 0x73], dh
        imul   ebp, dword ptr [esi + 0x67], 0x73746920
        and    byte ptr [ebp + 0x78], ah
        imul   esi, dword ptr [eax + 0x63], 0x616d6d6f
        outsb  dx, byte ptr [esi]
        add    byte ptr cs:[eax + ebp*2 + 0x69], dl
loc_2_00BB:
        jae    loc_2_00DD
        popal  
        jo     0x130
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x61], dh
        jae    0xed
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        jne    0x141
        imul   esp, dword ptr [ebx + 0x61], 0x676e6974
        and    byte ptr [edi + 0x6e], ch
loc_2_00DD:
        and    byte ptr [eax + ebp*2 + 0x65], dh
loc_2_00E1:
        and    byte ptr [esi + 0x65], ch
        je     0x15d
loc_2_00E6:
        outsd  dx, dword ptr [esi]
        jb     0x154
        and    byte ptr [edi + 0x68], dh
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ecx + 0x6f], bh
        jne    0x113
        je     loc_2_015A
        jb     loc_2_0164
        imul   ebp, dword ptr [esi + 0x61], 0x20646574
        imul   esi, dword ptr [esi + ebp + 0x20], 0x68746c41
        outsd  dx, dword ptr [esi]
loc_2_0107:
        jne    0x170
        push   0x6e695720
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_0185
        and    byte ptr [ecx + 0x74], ah
        je     loc_2_017C
        insd   dword ptr es:[edi], dx
        jo     loc_2_018E
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0x187
        jae    loc_2_0198
        outsd  dx, dword ptr [esi]
        jb     0x18c
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     0x196
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], ch
        je     0x1b4
        outsd  dx, dword ptr [esi]
        jb     0x1ab
        sub    al, 0x20
        jns    0x1b3
        jne    0x166
        insd   dword ptr es:[edi], dx
        popal  
        jns    0x16a
        push   0x20657661
        jo     loc_2_01C3
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        jae    loc_2_0178
        ja     loc_2_01C3
loc_2_015A:
        je     loc_2_01C4
        and    byte ptr [esi + 0x65], ch
        je     0x1d8
        outsd  dx, dword ptr [esi]
        jb     0x1cf
loc_2_0164:
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        jne    0x1d9
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    loc_2_0194
        jne    loc_2_01E4
        je     0x1e1
loc_2_0178:
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x6f], bh
loc_2_017C:
        jne    loc_2_019E
        jb     0x1e5
        jae    0x1f6
        popal  
        jb     loc_2_01F9
loc_2_0185:
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_01FC
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
loc_2_018E:
        jo     0x205
        je     loc_2_01F7
        jb     0x1c2
loc_2_0194:
        add    byte ptr [eax + ebp*2 + 0x69], dl
loc_2_0198:
        jae    loc_2_01BA
        popal  
        jo     loc_2_020D
        insb   byte ptr es:[edi], dx
loc_2_019E:
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x61], dh
        jae    0x1ca
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        jne    0x21e
        imul   esp, dword ptr [ebx + 0x61], 0x676e6974
        and    byte ptr [edi + 0x6e], ch
loc_2_01BA:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], ch
        je     0x23a
loc_2_01C3:
        outsd  dx, dword ptr [esi]
loc_2_01C4:
        jb     0x231
        and    byte ptr [edi + 0x68], dh
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ecx + 0x6f], bh
        jne    0x1f0
        je     loc_2_0237
        jb     0x241
        imul   ebp, dword ptr [esi + 0x61], 0x20646574
        imul   esi, dword ptr [esi + ebp + 0x20], 0x646e6957
        outsd  dx, dword ptr [esi]
loc_2_01E4:
        ja     0x259
        and    byte ptr [edi + 0x61], dh
        jae    0x20b
        jne    0x25b
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x263
        and    byte ptr [edx + 0x65], dh
loc_2_01F7:
        jae    0x26d
loc_2_01F9:
        outsd  dx, dword ptr [esi]
        jb     0x261
loc_2_01FC:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     loc_2_026B
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
loc_2_020D:
        and    byte ptr [esi + 0x65], ch
        je     0x289
        outsd  dx, dword ptr [esi]
        jb     loc_2_0280
        and    byte ptr cs:[ecx + 0x6f], bl
        jne    0x23b
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0x28a
        jae    0x29b
        popal  
        jb     0x29e
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_02A1
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     loc_2_02AA
        je     0x29c
loc_2_0237:
        jb     0x267
        add    byte ptr [eax + ebp*2 + 0x69], dl
        jae    0x25f
        popal  
        jo     0x2b2
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edx + 0x65], dh
        jno    0x2c4
        imul   esi, dword ptr [edx + 0x65], 0x20612073
        insb   byte ptr es:[edi], dx
        popal  
        jb     loc_2_02C1
        jb     0x27d
        bound  esi, qword ptr [ebp + 0x66]
        jb     0x284
        outsw  dx, word ptr [esi]
        jb     loc_2_0288
        je     0x2dc
        popal  
loc_2_026B:
        outsb  dx, byte ptr [esi]
        jae    loc_2_02D4
        jb     loc_2_02E3
        imul   ebp, dword ptr [esi + 0x67], 0x666e6920
        outsd  dx, dword ptr [esi]
        jb     0x2e8
        popal  
        je     loc_2_02E7
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_0280:
        and    byte ptr [edi + 0x76], ch
        jb     0x2a6
        je     0x2f0
loc_2_0288:
        and    byte ptr gs:[esi + 0x65], ch
        je     0x305
        outsd  dx, dword ptr [esi]
        jb     0x2fc
        and    byte ptr cs:[ecx + 0x6f], bl
        jne    0x2b7
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [ecx + 0x6e], ch
        arpl   word ptr [edx + 0x65], si
        popal  
loc_2_02A1:
        jae    0x308
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + 0x75], ah
loc_2_02AA:
        jb     0x2cf
        jae    0x31a
        jp     loc_2_0318
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ebp + 0x6f], ch
        imul   esp, dword ptr fs:[esi + 0x79], 0x20676e69
loc_2_02C1:
        je     0x32b
        and    byte ptr gs:[esi + 0x65], cl
        je     loc_2_0311
        popal  
        jo     0x320
        imul   edi, dword ptr [edx + 0x65], 0x74657320
loc_2_02D4:
        je     loc_2_033F
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x6e], ch
        and    byte ptr [ecx + 0x6f], bh
        jne    0x352
        and    byte ptr [ebx + 0x59], dl
loc_2_02E3:
        push   ebx
        push   esp
        inc    ebp
        dec    ebp
loc_2_02E7:
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        or     eax, 0x6e490a0a
        arpl   word ptr [ebp + esi*2 + 0x64], bp
        and    byte ptr gs:[edi + 0x72], ch
        and    byte ptr [ebp + 0x6f], ch
        imul   esp, dword ptr fs:[esi + 0x79], 0x65687420
        and    byte ptr [esi + 0x6f], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     loc_2_037A
loc_2_0311:
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + di + 0x65], dh
        je     loc_2_038C
loc_2_0318:
        imul   ebp, dword ptr [esi + 0x67], 0x206e6920
        je     0x389
        and    byte ptr gs:[ebx + 0x33], bl
        cmp    byte ptr [esi], dh
        inc    ebp
        outsb  dx, byte ptr [esi]
        push   0x6573205d
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ecx + 0x6f], bh
        jne    0x3ad
        and    byte ptr [ebx + 0x59], dl
        push   ebx
loc_2_033F:
        push   esp
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        cmp    cl, byte ptr gs:[0x20200a0a]
        and    byte ptr [eax], ah
        dec    esi
        je     loc_2_039F
        popal  
        jo     0x3ae
        imul   edi, dword ptr [edx + 0x65], 0xd73253d
        or     cl, byte ptr [edx]
        dec    ecx
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], cl
        je     0x3b7
        popal  
        jo     0x3c6
        imul   edi, dword ptr [edx + 0x65], 0x6c617620
loc_2_037A:
        jne    loc_2_03E1
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ebx + 0x74], dh
        imul   ebp, dword ptr [esp + ebp*2 + 0x20], 0x206f6f74
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_2_038C:
        ja     0x3ba
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0x3b4
        insd   dword ptr es:[edi], dx
        jae    loc_2_040B
        popal  
        and    byte ptr gs:[bx + 0x69], dh
        insb   byte ptr es:[edi], dx
loc_2_039F:
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x70], ah
        jo     0x40a
        popal  
        jb     loc_2_03C8
        popal  
        popal  
        imul   ebp, dword ptr [esi + 0x2c], 0x67757320
        jae    0x42a
        imul   ebp, dword ptr [esi + 0x67], 0x6e206120
        ja     0x3e0
        jbe    0x423
        insb   byte ptr es:[edi], dx
        jne    0x42a
        and    byte ptr [esi + 0x6f], ah
loc_2_03C8:
        jb     loc_2_03EA
        jns    0x43b
        jne    0x3ee
        je     0x43f
        and    byte ptr [edx + esi*2 + 0x79], dh
        add    byte ptr cs:[eax + ebp*2 + 0x69], dl
        jae    0x3fb
        jo     0x44f
        outsd  dx, dword ptr [esi]
        jb     0x442
loc_2_03E1:
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ebx + 0x61], ah
        jne    0x45d
loc_2_03EA:
        imul   ebp, dword ptr [esi + 0x67], 0x756f7920
        jb     0x413
        jae    0x46e
        jae    0x46b
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
        jne    0x474
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
        jns    0x47a
loc_2_040B:
        jne    0x42d
        ja     0x470
        outsb  dx, byte ptr [esi]
        je     0x432
        je     0x483
        and    byte ptr [ebp + 0x6e], ah
        and    byte ptr fs:[ecx + 0x74], ch
        aas    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x9f  R-X
        db  002h, 000h, 000h, 000h
loc_3_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
loc_3_000E:
        sbb    eax, 0x8b00
        add    byte ptr [esi + 0x69], dl
        jb     loc_3_008C
        jne    loc_3_007B
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x65], cl
        je     loc_3_0097
        outsd  dx, dword ptr [esi]
        jb     0x8e
        and    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x72724520
        outsd  dx, dword ptr [esi]
        jb     loc_3_0031
loc_3_0031:
        push   esp
        push   0x69562065
        jb     0xad
        jne    loc_3_009C
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x65], cl
        je     0xb8
        outsd  dx, dword ptr [esi]
        jb     0xaf
        and    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x73617720
        and    byte ptr [ebp + 0x6e], dh
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xc8
        and    byte ptr [ecx + 0x6e], ch
        imul   esi, dword ptr [ecx + ebp*2 + 0x61], 0x657a696c
        and    byte ptr cs:[eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        jb     0xcd
        insd   dword ptr es:[edi], dx
        jae    loc_3_008F
        je     0xd9
        popal  
        je     loc_3_0094
        jne    0xe9
        and    byte ptr gs:[eax + ebp*2 + 0x65], dh
loc_3_007B:
        and    byte ptr [esi + 0x65], ch
        je     0xf7
        outsd  dx, dword ptr [esi]
        jb     0xee
        and    byte ptr [ebp + 0x61], ch
        jns    0xa8
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xac
loc_3_008C:
        jne    0xfd
loc_3_008F:
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
loc_3_0094:
        and    byte ptr [eax + 0x72], dh
loc_3_0097:
        outsd  dx, dword ptr [esi]
        jo     0xff
        jb     0x108
loc_3_009C:
        jns    0xcc
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x4a4  R-X
        db  068h, 080h, 000h, 000h, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah
        db  001h, 06Ah, 001h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 085h, 0C0h, 00Fh, 084h
        db  0B1h, 000h, 000h, 000h, 089h, 015h
        dd  obj4:0x0430              ; fixup
        db  08Dh, 082h, 0C0h, 00Fh, 000h, 000h, 083h, 0C2h, 028h, 089h, 015h
        dd  obj1:0x1910              ; fixup
        db  08Dh, 04Ah, 068h, 0C7h, 042h, 0F8h, 000h, 002h, 000h, 000h, 03Bh, 0C8h, 073h, 007h, 089h, 04Ah
        db  0FCh, 08Bh, 0D1h, 0EBh, 0EBh, 0C7h, 042h, 0FCh, 000h, 000h, 000h, 000h, 083h, 03Dh
        dd  loc_1_1AD1               ; fixup
        db  000h, 074h, 056h, 0A1h
        dd  loc_4_0434               ; fixup
        db  06Bh, 0C0h, 068h, 083h, 0C0h, 010h, 083h, 0E0h, 0F0h, 08Bh, 0F8h, 06Ah, 004h, 050h, 0CDh, 020h
        db  0A8h, 000h, 001h, 000h, 059h, 059h, 085h, 0C0h, 074h, 044h, 097h, 0C1h, 0EFh, 004h, 066h, 089h
        db  03Dh
        dd  obj1:0x1914              ; fixup
        db  0C1h, 0E7h, 004h, 08Dh, 044h, 038h, 0C0h, 083h, 0C7h, 028h, 089h, 03Dh
        dd  obj1:0x190C              ; fixup
        db  08Dh, 04Fh, 068h, 0C7h, 047h, 0F8h, 000h, 000h, 000h, 000h, 03Bh, 0C8h, 073h, 007h, 089h, 04Fh
        db  0FCh, 08Bh, 0F9h, 0EBh, 0EBh, 0C7h, 047h, 0FCh, 000h, 000h, 000h, 000h, 0F8h, 0C3h, 06Ah, 000h
        db  050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h, 059h, 0A1h
        dd  obj4:0x0430              ; fixup
        db  085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h, 059h, 0B8h
        db  030h, 000h, 000h, 000h, 00Fh, 0B7h, 03Dh
        dd  loc_3_000C               ; fixup
        db  003h, 03Dh
        dd  loc_3_0004               ; fixup
        db  00Fh, 0B7h, 00Dh
        dd  loc_3_000E               ; fixup
        db  003h, 00Dh
        dd  loc_3_0004               ; fixup
        db  033h, 0F6h, 0CDh, 020h, 004h, 000h, 017h, 000h, 0F9h, 0C3h, 000h, 000h
sub_4_0100:
        sub    esi, esi
        mov    edi, 0x496                    ; -> loc_4_0496
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        jb     loc_4_0140
        sub    eax, eax
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     loc_4_013A
loc_4_0117:
        sub    bl, 0x30
        cmp    bl, 8
        jae    loc_4_0140
        movzx  ebx, bl
        bts    eax, ebx
        inc    edx
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     loc_4_013A
        cmp    bl, 0x2c
        jne    loc_4_0140
        inc    edx
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     loc_4_0140
        jmp    loc_4_0117
loc_4_013A:
        and    dword ptr [0x1ad1], eax       ; -> loc_1_1AD1
loc_4_0140:
        ret    
loc_4_0141:
        mov    dword ptr [0x1ad1], edx       ; -> loc_1_1AD1
        mov    dword ptr [0x1ad5], edx       ; -> loc_1_1AD5
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        xor    eax, eax
        test   ebx, 0x40000
        je     loc_4_0162
        mov    eax, 0x20
loc_4_0162:
        xor    esi, esi
        mov    edi, 0x440                    ; -> loc_4_0440
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        add    eax, 3
        shr    eax, 2
        je     loc_4_017F
        xor    ecx, ecx
        VxDCall  VDMAD.VDMAD_Reserve_Buffer_Space ; cd200d000400
loc_4_017F:
        mov    eax, 0x2a
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x19b4], ecx       ; -> obj1:0x19B4
        mov    dword ptr [0x19b8], edx       ; -> obj1:0x19B8
        mov    esi, 0xd98                    ; -> loc_1_0D98
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_4_01DD
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x2a
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 0xddd                    ; -> loc_1_0DDD
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_4_01DD
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x5c
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        call   sub_4_0100
        clc    
        ret    
loc_4_01DD:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_01E3:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x47a                    ; -> loc_4_047A
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        cmp    eax, 0x78
        jbe    loc_4_01FC
        mov    eax, 0x78
loc_4_01FC:
        add    al, al
        mov    byte ptr [0x1a8a], al         ; -> obj1:0x1A8A
        mov    al, byte ptr [0x1a8b]         ; -> loc_1_1A8B
        xor    esi, esi
        mov    edi, 0x48a                    ; -> obj4:0x048A
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x1a8b], al         ; -> loc_1_1A8B
        mov    eax, 0x20
        xor    esi, esi
        mov    edi, 0x438                    ; -> loc_4_0438
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x434], eax        ; -> loc_4_0434
        push   0
        push   8
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_4_0247
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_0247:
        mov    dword ptr [0x19a4], eax       ; -> obj1:0x19A4
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 0x14
        xor    eax, eax
        mov    word ptr [ebp + 0x18], ax
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp], ax
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebp + 0x38]
        or     ax, word ptr [ebp]
        je     loc_4_02A9
        xchg   esi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    edi, 0x1a08                   ; -> obj1:0x1A08
        mov    ecx, 0x20
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_4_02A9:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    dword ptr [0x1ad1], 0         ; -> loc_1_1AD1
        je     loc_4_0357
        mov    eax, 0xc
        xor    esi, esi
        mov    edi, 0x44b                    ; -> obj4:0x044B
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        add    eax, 3
        shr    eax, 2
        cmp    eax, 0xff
        jbe    loc_4_02EF
        mov    eax, 0xff
loc_4_02EF:
        push   eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    edx, eax
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        sub    edx, eax
        sub    edx, 0x20
        jae    loc_4_030B
        mov    edx, 3
loc_4_030B:
        pop    eax
        cmp    eax, edx
        jbe    loc_4_0312
        mov    eax, edx
loc_4_0312:
        mov    byte ptr [0x1a88], al         ; -> obj1:0x1A88
        mov    bx, 1
        mov    bh, al
        mov    cl, 0x10
        xor    eax, eax
        VxDCall  V86MMGR.V86MMGR_Set_Mapping_Info ; cd2004000600
        mov    eax, 0x1388
        mov    ecx, 3
        xor    esi, esi
        mov    edi, 0x457                    ; -> obj4:0x0457
        VMMCall  VMM.Get_Profile_Fixed_Point     ; cd20ad000100
        mov    dword ptr [0x19bc], eax       ; -> loc_1_19BC
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x468                    ; -> loc_4_0468
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x1a89], al         ; -> loc_1_1A89
loc_4_0357:
        push   0x198c                        ; -> obj1:0x198C
        push   0x193c                        ; -> obj1:0x193C
        VMMCall  VMM._Register_Win32_Services    ; cd2029010100
        add    esp, 8
        clc    
        call   sub_4_0371
        ret    
sub_4_0371:
        push   0
        push   7
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_4_0387
        stc    
        jmp    loc_4_042F
loc_4_0387:
        mov    word ptr [eax], 0xcbfa
        push   eax
        shl    eax, 0xc
        shr    ax, 0xc
        mov    dword ptr [0x1add], eax       ; -> loc_1_1ADD
        call   0
        pop    eax
        mov    edi, eax
        jb     loc_4_042F
        mov    eax, 0x2a
        mov    esi, 0xdf0                    ; -> loc_1_0DF0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x5c
        mov    esi, 0xe20                    ; -> loc_1_0E20
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    esi, 0x1412                   ; -> loc_1_1412
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_4_042F
        mov    dword ptr [0x19ac], eax       ; -> obj1:0x19AC
        xor    eax, eax
        or     eax, dword ptr [0x1ad5]       ; -> loc_1_1AD5
        jne    loc_4_042E
        xor    eax, eax
        mov    al, 0x5c
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        mov    word ptr [0x1ae1], dx         ; -> loc_1_1AE1
        mov    word ptr [0x1ae3], cx         ; -> loc_1_1AE3
        mov    eax, edi
        add    eax, 2
        mov    word ptr [eax + 1], dx
        mov    word ptr [eax + 3], cx
        mov    byte ptr [eax], 0xea
        or     dx, cx
        jne    loc_4_0414
        dec    eax
        mov    byte ptr [eax], 0xcf
loc_4_0414:
        shl    eax, 0xc
        shr    ax, 0xc
        movzx  edx, ax
        shr    eax, 0x10
        mov    ecx, eax
        mov    eax, 0x5c
        VMMCall  VMM.Set_V86_Int_Vector          ; cd2043000100
loc_4_042E:
        clc    
loc_4_042F:
        ret    
        db  000h, 000h, 000h, 000h
loc_4_0434:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0438:
        dec    esi
        jne    0x4a8
        dec    eax
        inc    ebx
        inc    edx
        jae    loc_4_0440
loc_4_0440:
        dec    esi
        je     0x488
        dec    ebp
        inc    ecx
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x74654e00
        dec    eax
        popal  
        jo     0x4a6
        imul   edi, dword ptr [edx + 0x65], 0x74654e00
        inc    ecx
        jae    0x4d6
        outsb  dx, byte ptr [esi]
        arpl   word ptr [eax + 0x54], bp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
loc_4_0468:
        dec    esi
        je     0x4ad
        jae    0x4e7
        outsb  dx, byte ptr [esi]
        arpl   word ptr [eax + 0x46], bp
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        bound  esp, qword ptr [ecx + 0x63]
        db  06Bh, 000h
loc_4_047A:
        dec    esi
        je     0x4d0
        imul   esi, dword ptr fs:[edx + 0x54], 0x6f656d69
        jne    0x4fd
        add    byte ptr [esi + 0x6f], cl
        push   edi
        popal  
        imul   esi, dword ptr [esi + ecx*2 + 0x65], 0x4f4974
loc_4_0496:
        push   esi
        cmp    byte ptr [esi], dh
        dec    ebp
        outsd  dx, dword ptr [esi]
        dec    esp
        popal  
        outsb  dx, byte ptr [esi]
        popal  
        jae    0x4a3
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x1c4  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 042h, 0B8h, 05Ch, 035h, 0CDh, 021h, 08Ch, 0C0h, 00Bh, 0C3h, 074h
        db  029h, 0BEh, 003h, 001h, 02Bh, 0D2h, 0E8h, 037h, 000h, 074h, 005h, 00Fh, 0ABh, 016h, 0C1h, 000h
        db  042h, 083h, 0FAh, 008h, 073h, 014h, 0BEh, 063h, 001h, 0E8h, 024h, 000h, 074h, 0F2h, 0BFh, 003h
        db  001h, 0B9h, 030h, 000h, 0F3h, 0A7h, 075h, 0E3h, 0EBh, 0E6h, 033h, 0DBh, 033h, 0F6h, 066h, 00Fh
        db  0B7h, 016h, 0C1h, 000h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h
        db  08Ch, 0D8h, 08Eh, 0C0h, 0BFh, 0C3h, 000h, 0B9h, 020h, 000h, 033h, 0C0h, 0F3h, 0ABh, 08Bh, 0FEh
        db  0B9h, 030h, 000h, 0F3h, 0ABh, 0C6h, 006h, 0C3h, 000h, 033h, 0C6h, 006h, 0C4h, 000h, 023h, 088h
        db  016h, 0F3h, 000h, 0C7h, 006h, 0EFh, 000h, 000h, 000h, 0C7h, 006h, 0F1h, 000h, 000h, 000h, 0C6h
        db  006h, 0F4h, 000h, 000h, 0C6h, 006h, 0CDh, 000h, 02Ah, 089h, 036h, 0C7h, 000h, 08Ch, 01Eh, 0C9h
        db  000h, 0C7h, 006h, 0CBh, 000h, 060h, 000h, 0BBh, 0C3h, 000h, 01Eh, 056h, 052h, 0CDh, 05Ch, 05Ah
        db  05Eh, 01Fh, 084h, 0C0h, 075h, 003h, 0A0h, 0C4h, 000h, 03Ch, 023h, 074h, 013h, 08Bh, 0FEh, 02Bh
        db  0C0h, 0B9h, 030h, 000h, 0F3h, 0AFh, 074h, 008h, 08Dh, 07Ch, 00Ah, 0B9h, 00Bh, 000h, 0F3h, 0ABh
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x1e  R-X
sub_6_0000:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_6_0008:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        add    byte ptr [eax], al
sub_6_0010:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_6_0018:
        VxDCall  Dev_2533.Service_803Eh          ; cd203e803325
VxD__LTEXT ENDS
